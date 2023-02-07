from network import Bluetooth
from machine import Timer
import ujson

information = {
    'posLat': 0,
    'posLon': 0,
    'battery': 0,
}

def char_notify_callback(char, arg):
    char_value = (char.value())
    pos = char_value.decode('utf-8').replace("'", '"')
    information['posLat'] = ujson.loads(pos)['posLat']
    information['posLon'] = ujson.loads(pos)['posLon']
    information['battery'] = ujson.loads(pos)['battery']
    print("Se recibio {}".format(information))
    print("La latitud es: {}".format(information['posLat']))
    print("La longitud es: {}".format(information['posLon']))
    print("La bateria es: {}".format(information['battery']))

bluetooth = Bluetooth()
print('Bluetooth init')
bluetooth.init(antenna=Bluetooth.EXT_ANT)
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