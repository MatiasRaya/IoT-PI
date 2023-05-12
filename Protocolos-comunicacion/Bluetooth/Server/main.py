from network import Bluetooth
import gc
import pycom
import time

# Se apaga el led de la placa
pycom.heartbeat(False)

# Se configura el nombre del dispositivo
device_name = 'FiPy'

gc.enable()

# Se configura el handler para las conexiones
def conn_cb (bt_o):
    try:
        # Se obtienen los eventos
        events = bt_o.events()
        # Se verifica si el evento es de conexión
        if  events & Bluetooth.CLIENT_CONNECTED:
            # Se enciende el led verde de la placa
            pycom.rgbled(0x007f00)
            # Se imprime en consola que se ha conectado un cliente
            print("Client connected")
            # Se espera un segundo
            time.sleep(1)
            # Se apaga el led de la placa
            pycom.rgbled(0x000000)
        # Se verifica si el evento es de desconexión
        elif events & Bluetooth.CLIENT_DISCONNECTED:
            # Se enciende el led rojo de la placa
            pycom.rgbled(0x7f0000)
            # Se imprime en consola que se ha desconectado un cliente
            print("Client disconnected")
            # Se espera un segundo
            time.sleep(1)
            # Se apaga el led de la placa
            pycom.rgbled(0x000000)
    except Exception as e:
        print("Error in conn_cb: " + str(e))
        # Se libera memoria
        gc.collect()

# Se configura el handler para las escrituras y lecturas
def char1_cb_handler(chr, data):
    try:
        # Se obtienen los eventos y el valor
        events, value = data
        # Se enciende el led azul de la placa
        pycom.rgbled(0x00007f)
        # Se espera 1 segundo
        time.sleep(1)
        # Se apaga el led de la placa
        pycom.rgbled(0x000000)
        # Se imprime en consola el valor recibido
        print("Write request with value = {}".format(value))
        # Se ingresa texto por consola
        text = input("Enter your message: ")
        # Se imprime por pantalla el tamaño del texto
        print("Sending message of size {}".format(len(text)))
        # Se envía el texto ingresado
        chr1.value(text)
        # Se libera memoria
        gc.collect()
    except Exception as e:
        # Se imprime el error
        print("Error: {}".format(e))

# Se configura el bluetooth
bluetooth = Bluetooth()
# Se configura la antena externa
bluetooth.init(antenna=Bluetooth.EXT_ANT)
# Se configura el nombre del dispositivo y su fabricante
bluetooth.set_advertisement(name=device_name, manufacturer_data="Pycom")
# Se configura el handler para las conexiones
bluetooth.callback(trigger=Bluetooth.CLIENT_CONNECTED | Bluetooth.CLIENT_DISCONNECTED, handler=conn_cb)
# Se inicia el servicio
bluetooth.advertise(True)

# Se configura el servicio
srv1 = bluetooth.service(uuid=0xec00, isprimary=True)
# Se configura la característica y su handler
chr1 = srv1.characteristic(uuid=0xec0e, value="Hola desde el server")
char1= chr1.callback(trigger=Bluetooth.CHAR_WRITE_EVENT | Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT, handler=char1_cb_handler)