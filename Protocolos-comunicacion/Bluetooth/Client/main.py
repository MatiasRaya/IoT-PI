
from network import Bluetooth
import binascii
import time
import pycom

pycom.heartbeat(False)

bt = Bluetooth()
bt.start_scan(-1)

while True:
    adv = bt.get_adv()
    if adv:
        # try to get the complete name
        print(bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL))

        # try to get the manufacturer data (Apple's iBeacon data is sent here)
        mfg_data = bt.resolve_adv_data(adv.data, Bluetooth.ADV_MANUFACTURER_DATA)

        if mfg_data:
            # try to get the manufacturer data (Apple's iBeacon data is sent here)
            print(binascii.hexlify(mfg_data))

        if bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL) == 'LoPy':
            conn = bt.connect(adv.mac)
            services = conn.services()
            for service in services:
                time.sleep(0.050)
                if type(service.uuid()) == bytes:
                    print('Reading chars from service = {}'.format(service.uuid()))
                else:
                    print('Reading chars from service1 = %x' % service.uuid())
                chars = service.characteristics()
                for char in chars:
                    if (char.properties() & Bluetooth.PROP_READ):
                        print('char {} value = {}'.format(char.uuid(), char.read()))
                        pycom.rgbled(0x7f0000)
                        time.sleep(2)
                        pycom.rgbled(0x000000)
                    if (char.properties() & Bluetooth.PROP_WRITE):
                        # print('char {} value = {}'.format(char.uuid(), char.read()))
                        char.write("hola mundo")
                        pycom.rgbled(0x007f00)
                        time.sleep(2)
                        pycom.rgbled(0x000000)
            conn.disconnect()
            break
    else:
        time.sleep(0.050)