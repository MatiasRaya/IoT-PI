# from network import Bluetooth
# from machine import Timer

# import pycom

# red = 0x7f0000
# green = 0x007f00
# blue = 0x00007f
# yellow = 0x7f7f00
# white = 0x7f7f7f
# pink = 0x7f007f
# cian = 0x007f7f
# orange = 0xd35400
# none= 0x000000

# pycom.heartbeat(False)

# battery = 100
# update = False

# def conn_cb(chr):
#     events = chr.events()
#     print("events conn_cb: ",events)
#     print("events conn_cb: ",Bluetooth.CLIENT_CONNECTED)
#     print("events conn_cb: ",Bluetooth.CLIENT_DISCONNECTED)
#     if events & Bluetooth.CLIENT_CONNECTED:
#         print('client connected')
#         pycom.rgbled(cian)
#     elif events & Bluetooth.CLIENT_DISCONNECTED:
#         print('client disconnected')
#         update = False
#         pycom.rgbled(pink)

# def chr1_handler(chr, data):
#     global battery
#     global update
#     events = chr.events()
#     print("events chr1_handler: ",events)
#     print("events chr1_handler: ",Bluetooth.CHAR_READ_EVENT)
#     print("events chr1_handler: ",Bluetooth.CHAR_SUBSCRIBE_EVENT)
#     if events & (Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT):
#         chr.value(battery)
#         print("transmitted :", battery)
#         if (events & Bluetooth.CHAR_SUBSCRIBE_EVENT):
#             update = True


# bluetooth = Bluetooth()
# bluetooth.set_advertisement(name='FiPy 45', manufacturer_data="Pycom", service_uuid=12345)

# bluetooth.callback(trigger=Bluetooth.CLIENT_CONNECTED | Bluetooth.CLIENT_DISCONNECTED, handler=conn_cb)

# bluetooth.advertise(True)

# srv1 = bluetooth.service(uuid=12345, isprimary=True, nbr_chars=1, start=True)

# chr1 = srv1.characteristic(uuid=54321, properties=Bluetooth.PROP_INDICATE | Bluetooth.PROP_BROADCAST | Bluetooth.PROP_NOTIFY, value='read_from_here') #client reads from here

# chr1.callback(trigger=(Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT), handler=chr1_handler)

# print('Start BLE service')

# def update_handler(update_alarm):
#     global battery
#     global update
#     battery-=1
#     if battery == 1:
#         battery = 100
#     if update:
#         chr1.value(str(battery))

#     # print("update: ", update)

# update_alarm = Timer.Alarm(update_handler, 1, periodic=True)

# pycom.rgbled(blue)


from network import Bluetooth
from machine import Timer

battery = 100
update = False
def conn_cb(chr):
    events = chr.events()
    if events & Bluetooth.CLIENT_CONNECTED:
        print('client connected')
    elif events & Bluetooth.CLIENT_DISCONNECTED:
        print('client disconnected')
        update = False

def chr1_handler(chr, data):
    global battery
    global update
    events = chr.events()
    print("events: ",events)
    if events & (Bluetooth.CHAR_READ_EVENT | Bluetooth.CHAR_SUBSCRIBE_EVENT):
        chr.value(battery)
        print("transmitted :", battery)
        if (events & Bluetooth.CHAR_SUBSCRIBE_EVENT):
            update = True

bluetooth = Bluetooth()
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
    battery-=1
    if battery == 1:
        battery = 100
    if update:
        chr1.value(str(battery))

update_alarm = Timer.Alarm(update_handler, 1, periodic=True)