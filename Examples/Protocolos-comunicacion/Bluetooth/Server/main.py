from network import Bluetooth
import gc
import pycom
import time

# Se apaga el led de la placa
pycom.heartbeat(False)

# Se configura el nombre del dispositivo
device_name = 'FiPy'

# Se configura el handler para las conexiones
def conn_cb (bt_o):
    # Se obtienen los eventos
    events = bt_o.events()
    # Se verifica si el evento es de conexión
    if  events & Bluetooth.CLIENT_CONNECTED:
        # Se enciende el led verde de la placa
        pycom.rgbled(0x007f00)
        # Se espera 1 segundo
        time.sleep(1)
        # Se apaga el led verde de la placa
        pycom.rgbled(0x000000)
        # Se imprime en consola que se ha conectado un cliente
        print("Client connected")
    # Se verifica si el evento es de desconexión
    elif events & Bluetooth.CLIENT_DISCONNECTED:
        # Se enciende el led rojo de la placa
        pycom.rgbled(0x7f0000)
        print("Client disconnected")
        # Se espera 1 segundo
        time.sleep(1)
        # Se apaga el led rojo de la placa
        pycom.rgbled(0x000000)
        # Se imprime en consola que se ha desconectado un cliente

# Se configura el handler para las escrituras y lecturas
def char1_cb_handler(chr, data):
    # Se obtienen los eventos y el valor
    events, value = data
    # Se verifica si el evento es de escritura
    if  events & Bluetooth.CHAR_WRITE_EVENT:
        # Se enciende el led azul de la placa
        pycom.rgbled(0x00007f)
        # Se espera 1 segundo
        time.sleep(1)
        # Se apaga el led azul de la placa
        pycom.rgbled(0x000000)
        # Se imprime en consola el valor recibido
        print("Write request with value = {}".format(value))
        # Se libera memoria
        gc.collect()
    # Se verifica si el evento es de lectura
    if events & (Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT):
        # Se libera memoria
        gc.collect()
        # Se enciende el led amarillo de la placa
        pycom.rgbled(0x7f7f00)
        # Se espera 1 segundo
        time.sleep(1)
        # Se apaga el led amarillo de la placa
        pycom.rgbled(0x000000)
        # Se ingresa texto por consola
        text = input("Enter your message: ")
        # Se imprime por pantalla el tamaño del texto
        print("Sending message of size {}".format(len(text)))
        # Se envía el texto ingresado
        chr1.value(text)
        # Se libera memoria
        gc.collect()

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