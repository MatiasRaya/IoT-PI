# from network import Bluetooth
# from machine import Timer
# import ubinascii
# import struct

# def char_notify_callback(char, arg):
#     char_value = (char.value())
#     print("New value: {}".format(char_value))

# bt = Bluetooth()
# print('Start scanning for BLE services')
# bt.start_scan(-1)
# adv = None
# while(True):
#     adv = bt.get_adv()
#     if adv:
#         try:
#             print(bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL))
#             if bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL)=="FiPy 45":
#                 conn = bt.connect(adv.mac)
#                 print('\nName of device: ' + str(bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL)) + '\n')
#                 variable = ubinascii.hexlify(bt.resolve_adv_data(adv.data, Bluetooth.ADV_MANUFACTURER_DATA))
#                 print(variable)
#                 var = variable[12:16]
#                 print(var)
#                 v = (struct.unpack("<H",variable)[0])/16
#                 print("\ndata: " + str(v) + '\n')
#                 print(ubinascii.hexlify(bt.resolve_adv_data(adv.data, Bluetooth.ADV_128SRV_CMPL)))
#                 print(bt.resolve_adv_data(adv.data, Bluetooth.ADV_32SERVICE_DATA))
#                 print("Connected to FiPy 45")
#                 try:
#                     services = conn.services()
#                     for service in services:
#                         print("INSTANCE Service: ")
#                         print(service.instance())
#                         print("UUID Service: ")
#                         print(service.uuid())
#                         print("ISPRIMARY Service: ")
#                         print(service.isprimary())
#                         chars = service.characteristics()
#                         for char in chars:
#                             print("INSTANCE Char: ")
#                             print(char.instance())
#                             print("UUID Char: ")
#                             print(char.uuid())
#                             print("PROPERTIES Char: ")
#                             print(char.properties())
#                             print("Bluetooth.PROP_NOTIFY")
#                             print(Bluetooth.PROP_NOTIFY)
#                             # print("READ Char: ")
#                             # print(char.read())
#                             print("c_uuid")
#                             c_uuid = char.uuid()
#                             print(c_uuid)
#                             # if c_uuid == 10753:
#                             print("char.properties() & Bluetooth.PROP_NOTIFY")
#                             print(char.properties() | Bluetooth.PROP_NOTIFY)
#                             if (char.properties() | Bluetooth.PROP_NOTIFY):
#                                 print("Entre if (char.properties() & Bluetooth.PROP_NOTIFY)")
#                                 char.callback(trigger=Bluetooth.CHAR_NOTIFY_EVENT, handler=char_notify_callback)
#                                 print(c_uuid)
#                                 break
#                 except:
#                     continue
#         except:
#             continue

# bt.stop_scan()
# bt.disconnect_client()




from network import Bluetooth
from machine import Timer

def char_notify_callback(char, arg):
    char_value = (char.value())
    print("New value: {}".format(char_value))

bt = Bluetooth()
print('Start scanning for BLE services')
bt.start_scan(-1)
adv = None
while(True):
    adv = bt.get_adv()
    if adv:
        try:
            print(bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL))
            if bt.resolve_adv_data(adv.data, Bluetooth.ADV_NAME_CMPL)=="FiPy 45":
                conn = bt.connect(adv.mac)
                print("Connected to FiPy 45")
                try:
                    services = conn.services()
                    for service in services:
                        chars = service.characteristics()
                        for char in chars:
                            c_uuid = char.uuid()
                            if c_uuid == 0xec0e:
                                if (char.properties() & Bluetooth.PROP_NOTIFY):
                                    char.callback(trigger=Bluetooth.CHAR_NOTIFY_EVENT, handler=char_notify_callback)
                                    print(c_uuid)
                                    break
                except:
                    continue
        except:
            continue

bt.stop_scan()
bt.disconnect_client()