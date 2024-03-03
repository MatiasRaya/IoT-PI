import machine
import time
from network import WLAN, LTE, Bluetooth

class Connection:
    
    # Se crea el constructor de la clase
    def __init__(self):
        self.bt = Bluetooth()    

    # Se crea el método bluetooth_connection
    def bluetooth_connection(self):
        # Se inicializa el módulo Bluetooth con la antena externa
        self.bt.init(antenna=self.bt.EXT_ANT)
        # Se comienza a escanear
        self.bt.start_scan(-1)
        # Se obtienen los dispositivos encontrados
        aux = self.bt.get_adv()
        # Se detiene el escaneo
        self.bt.stop_scan()

        # Se retorna los dispositivos encontrados
        return aux
    

class WiFi:
    
    def __init__(self):
        self.wlan = WLAN()

    def config_STA(self, ssid, password, antenna):
        # Se configura el modo de la red
        self.wlan.init(mode=WLAN.STA)
        # Se verifica si la antena es externa
        if antenna == "ext":
            # Se configura la antena
            self.wlan.antenna(WLAN.EXT_ANT)
        # Se verifica si la antena es interna
        elif antenna == "int":
            # Se configura la antena
            self.wlan.antenna(WLAN.INT_ANT)
        # Se conecta a la red
        self.wlan.connect(ssid, auth=(WLAN.WPA2, password))
        # Se espera a que se conecte
        while not self.wlan.isconnected():
            # Se pone en modo idle para ahorrar energía
            machine.idle()
        # Se imprime un mensaje de que se conectó
        print('WLAN connection succeeded!')

    def config_AP(self, ssid, password, antenna):
        # Se configura el modo de la red
        self.wlan.init(mode=WLAN.AP, ssid=ssid, auth=(WLAN.WPA2, password))
        # Se verifica si la antena es externa
        if antenna == "ext":
            # Se configura la antena
            self.wlan.antenna(WLAN.EXT_ANT)
        # Se verifica si la antena es interna
        elif antenna == "int":
            # Se configura la antena
            self.wlan.antenna(WLAN.INT_ANT)
        # Se imprime un mensaje de que se creó el AP
        print('WLAN AP created!')

    def config_STA_AP(self, ssid, password, ssidAP, passwordAP, antenna):
        # Se verifica si la antena es externa
        if antenna == "ext":
            # Se configura la antena
            self.wlan.antenna(WLAN.EXT_ANT)
        # Se verifica si la antena es interna
        elif antenna == "int":
            # Se configura la antena
            self.wlan.antenna(WLAN.INT_ANT)
        # Se configura el modo de la red
        self.wlan.init(mode=WLAN.STA_AP, ssid=ssidAP, auth=(WLAN.WPA2, passwordAP))
        # Se conecta a la red
        self.wlan.connect(ssid, auth=(WLAN.WPA2, password))
        # Se espera a que se conecte
        while not self.wlan.isconnected():
            # Se pone en modo idle para ahorrar energía
            machine.idle()
        # Se imprime un mensaje de que se conectó
        print('WLAN connection succeeded!')

class LTE:
        
        def __init__(self):
            self.lte = LTE()

        def init(self):
            # Se inicializa el módulo LTE
            self.lte.init(carrier='standard')
            # Se imprime un mensaje de que se inicializó
            print('LTE initialized!')

        def attach(self, band, apn):
            # Se adjunta el módulo LTE al servicio de red
            self.lte.attach(band=band, apn=apn)
            # Se espera a que se adjunte
            while not self.lte.isattached():
                # Se pone en modo idle para ahorrar energía
                machine.idle()
            # Se imprime un mensaje de que se adjuntó
            print('LTE attached!')

        def connect(self):
            # Se conecta al servicio de red
            self.lte.connect()
            # Se espera a que se conecte
            while not self.lte.isconnected():
                # Se pone en modo idle para ahorrar energía
                machine.idle()
            # Se imprime un mensaje de que se conectó
            print('LTE connection succeeded!')

        def disconnect(self):
            # Se desconecta del servicio de red
            self.lte.disconnect()
            # Se espera a que se desconecte
            while self.lte.isconnected():
                # Se pone en modo idle para ahorrar energía
                machine.idle()
            # Se imprime un mensaje de que se desconectó
            print('LTE disconnected!')

        def detach(self):
            # Se desconecta del servicio de red
            self.lte.detach()
            # Se espera a que se desconecte
            while self.lte.isattached():
                # Se pone en modo idle para ahorrar energía
                machine.idle()
            # Se imprime un mensaje de que se desconectó
            print('LTE detached!')

