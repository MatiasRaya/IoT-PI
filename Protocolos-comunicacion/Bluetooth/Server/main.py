from network import Bluetooth
from machine import Timer

from sensors import Sensors
from pycoproc_1 import Pycoproc

import pycom

red = 0x7f0000
green = 0x007f00
blue = 0x00007f
yellow = 0x7f7f00
white = 0x7f7f7f
pink = 0x7f007f
cian = 0x007f7f
orange = 0xd35400
none= 0x000000

pycom.heartbeat(False)

py = Pycoproc(Pycoproc.PYTRACK)
pySensor = Sensors(py)

battery = 100
position = pySensor.get_position().coordinates()
update = False


def conn_cb(chr):
    events = chr.events()
    if events & Bluetooth.CLIENT_CONNECTED:
        print('client connected')
        pycom.rgbled(cian)
    elif events & Bluetooth.CLIENT_DISCONNECTED:
        print('client disconnected')
        pycom.rgbled(pink)
        update = False

def chr1_handler(chr, data):
    global battery
    global update
    global position
    events = chr.events()
    print("events: ",events)
    if events & (Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT):
        print("transmitted :", position)
        if (events & Bluetooth.CHAR_SUBSCRIBE_EVENT):
            update = True

bluetooth = Bluetooth()
bluetooth.init(antenna=Bluetooth.EXT_ANT)
bluetooth.set_advertisement(name='FiPy 45', manufacturer_data="Pycom", service_uuid=0xec00)

bluetooth.callback(trigger=Bluetooth.CLIENT_CONNECTED | Bluetooth.CLIENT_DISCONNECTED, handler=conn_cb)
bluetooth.advertise(True)

srv1 = bluetooth.service(uuid=0xec00, isprimary=True,nbr_chars=1)

chr1 = srv1.characteristic(uuid=0xec0e, value='read_from_here') #client reads from here
chr1.callback(trigger=(Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT), handler=chr1_handler)

print('Start BLE service')

def update_handler(update_alarm):
    global battery
    global update
    global position
    battery-=1
    if battery == 1:
        battery = 100
    position = pySensor.get_position().coordinates()
    if update:
        print(position)
        chr1.value(str(position))

update_alarm = Timer.Alarm(update_handler, 1, periodic=True)

pycom.rgbled(blue)