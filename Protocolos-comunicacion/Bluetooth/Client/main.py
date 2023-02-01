from network import Bluetooth
from machine import Timer

def char_notify_callback(char, arg):
    char_value = (char.value())
    print("New value: {}".format(char_value))

bluetooth = Bluetooth()
# bluetooth.init(antenna=Bluetooth.EXT_ANT)
print('Start scanning for BLE services')
bluetooth.start_scan(-1)
adv = None
while(True):
    adv = bluetooth.get_adv()
    if adv:
        try:
            print(bluetooth.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL))
            if bluetooth.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL)=="FiPy 45":
                conn = bluetooth.connect(adv.mac)
                print("Connected to FiPy 45")
                try:
                    services = conn.services()
                    for service in services:
                        chars = service.characteristics()
                        for char in chars:
                            c_uuid = char.uuid()
                            print(c_uuid == 0xec0e)
                            if c_uuid == 0xec0e:
                                if (char.properties() & Bluetooth.PROP_NOTIFY):
                                    print(c_uuid)
                                    char.callback(trigger=Bluetooth.CHAR_NOTIFY_EVENT, handler=char_notify_callback)
                                    break
                except:
                    continue
        except:
            continue

bluetooth.stop_scan()
bluetooth.disconnect_client()