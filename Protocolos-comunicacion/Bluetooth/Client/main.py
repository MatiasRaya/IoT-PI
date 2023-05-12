from network import Bluetooth
import gc
import time
import pycom

# Se desactiva el led de la placa
pycom.heartbeat(False)

# Se configura el nombre del dispositivo
device_name = 'FiPy'

gc.enable()

def callback(char, data):
    try:
        # Se enciende el led rojo de la placa
        pycom.rgbled(0x7f0000)
        # Se espera 1 segundo
        time.sleep(1)
        # Se apaga el led rojo de la placa
        pycom.rgbled(0x000000)
        # Se imprime en consola el valor recibido
        print("Write request with value = {}".format(char.value()))
        # Se almacena el texto ingresado por el usuario
        text = input("Ingrese el texto a enviar: ")
        # Se imprime por pantalla el tamaño del texto
        print("Sending message of size {}".format(len(text)))
        # Se escribe en la caracteristica
        char.write(text)
        print("Message sent")
        # Se libera memoria
        gc.collect()
    except Exception as e:
        # Se imprime el error
        print("Error: {}".format(e))

# Se configura el Bluetooth
bluetooth = Bluetooth()
# Se configura la antena externa
bluetooth.init(antenna=Bluetooth.EXT_ANT)
# Se inicia a escanear por tiempo indefinido
bluetooth.start_scan(-1)

# Se inicia un bucle infinito
while True:
    # Se obtinen los datos del dispositivo que se esta publicitando
    adv = bluetooth.get_adv()

    # Se comprueba que se hayan obtenido datos
    if adv:
        # Se obtiene el fabricante del dispositivo
        mfg_data = bluetooth.resolve_adv_data(adv.data, Bluetooth.ADV_MANUFACTURER_DATA)

        # Se verifica que se haya obtenido el fabricante
        if mfg_data == "Pycom":
            # Se imprime el fabricante
            print('mfg_data = {}'.format(mfg_data))

        # Se obtiene el nombre del dispositivo y se verifica que sea el correcto
        if bluetooth.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL) == device_name:
            # Se conecta al dispositivo
            conn = bluetooth.connect(adv.mac)

            # Se obtienen los servicios del dispositivo
            services = conn.services()

            # Se recorren los servicios
            for service in services:

                # Se verifica que el tipo de dato sea bytes
                if type(service.uuid()) == bytes:
                    # Se imprime el uuid del servicio como cadena de texto
                    print('Reading chars from service = {}'.format(service.uuid()))
                else:
                    # Se imprime el uuid del servicio en hexadecimal
                    print('Reading chars from service1 = %x' % service.uuid())

                # Se obtienen las caracteristicas del servicio
                chars = service.characteristics()

                # Se recorren las caracteristicas
                for char in chars:
                    # Se verifica que el uuid de la caracteristica sea el correcto
                    if(char.uuid() == 0xec0e):
                        if(char.properties() & Bluetooth.PROP_NOTIFY):
                            char.callback(trigger=Bluetooth.CHAR_NOTIFY_EVENT, handler=callback)
                            print("Se ha configurado la notificacion")
                            break

    else:
        # Se espera 50 milisegundos
        time.sleep(0.050)