from network import Bluetooth
import pycom
import time

pycom.heartbeat(False)

def conn_cb (bt_o):
    events = bt_o.events()
    if  events & Bluetooth.CLIENT_CONNECTED:
        pycom.rgbled(0x007f00) # green
        print("Client connected")
        time.sleep(5)
    elif events & Bluetooth.CLIENT_DISCONNECTED:
        pycom.rgbled(0x7f0000) # red
        char1.value(67)
        time.sleep(5)

def char1_cb_handler(chr, data):

    # The data is a tuple containing the triggering event and the value if the event is a WRITE event.
    # We recommend fetching the event and value from the input parameter, and not via characteristic.event() and characteristic.value()
    events, value = data
    if  events & Bluetooth.CHAR_WRITE_EVENT:
        pycom.rgbled(0x00007f) # blue
        print("Write request with value = {}".format(value))
        time.sleep(5)
    else:
        pycom.rgbled(0x7f7f00) # yellow
        print('Read request on char 1')

def char2_cb_handler(chr, data):
    # The value is not used in this callback as the WRITE events are not processed.
    events, value = data
    if  events & Bluetooth.CHAR_READ_EVENT:
        pycom.rgbled(0x7f3f00) # orange
        print('Read request on char 2')

bluetooth = Bluetooth()
bluetooth.set_advertisement(name='LoPy')
bluetooth.callback(trigger=Bluetooth.CLIENT_CONNECTED | Bluetooth.CLIENT_DISCONNECTED, handler=conn_cb)
bluetooth.advertise(True)

srv1 = bluetooth.service(uuid=0xec00, isprimary=True)
chr1 = srv1.characteristic(uuid=0xec0e, value="Hola desde el server")
char1= chr1.callback(trigger=Bluetooth.CHAR_WRITE_EVENT | Bluetooth.CHAR_READ_EVENT, handler=char1_cb_handler)

# srv2 = bluetooth.service(uuid=1234, isprimary=True)
# chr2 = srv2.characteristic(uuid=4567, value=0x1234)
# char2_cb = chr2.callback(trigger=Bluetooth.CHAR_READ_EVENT, handler=char2_cb_handler)