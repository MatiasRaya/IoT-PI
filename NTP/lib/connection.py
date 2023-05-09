import machine
import time
from network import WLAN, LTE, Bluetooth

class Connection:
    
    # Se crea el constructor de la clase
    def __init__(self):
        # Se crean las instancias de las clases WLAN, LTE y Bluetooth
        self.wlan = WLAN(mode=WLAN.STA)
        self.lte = LTE()
        self.bt = Bluetooth()
    
    # Se crea el método wifi_connection
    def wifi_connection(self):
        # Se configura la antena externa
        self.wlan.antenna(WLAN.EXT_ANT)
        # Se escanean las redes disponibles
        nets = self.wlan.scan()
        # Se recorren las redes disponibles
        for net in nets:
            # Se verifica si la red es la que se desea conectar
            if net.ssid == "LCD":
                # Se imprime un mensaje de que se encontró la red
                print('Network found!')
                # Se conecta a la red
                self.wlan.connect('LCD', auth=(WLAN.WPA2, '1cdunc0rd0ba'))
                # Se espera a que se conecte
                while not self.wlan.isconnected():
                    # Se pone en modo idle para ahorrar energía
                    machine.idle()
                # Se imprime un mensaje de que se conectó
                print('WLAN connection succeeded!')
                # Se sale del bucle
                break
            # Se verifica si la red es la que se desea conectar
            elif net.ssid == "LCD-IoT":
                # Se imprime un mensaje de que se encontró la red
                print('Network found!')
                # Se conecta a la red
                self.wlan.connect('LCD-IoT', auth=(WLAN.WPA2, '107_1AbCd'))
                # Se espera a que se conecte
                while not self.wlan.isconnected():
                    # Se pone en modo idle para ahorrar energía
                    machine.idle()
                # Se imprime un mensaje de que se conectó
                print('WLAN connection succeeded!')
                # Se sale del bucle
                break
            # Se verifica si la red es la que se desea conectar
            elif net.ssid == "RAYADOS 2.4":
                # Se imprime un mensaje de que se encontró la red
                print('Network found!')
                # Se conecta a la red
                self.wlan.connect('RAYADOS 2.4', auth=(WLAN.WPA2, 'Rayaplasencia1996'))
                # Se espera a que se conecte
                while not self.wlan.isconnected():
                    # Se pone en modo idle para ahorrar energía
                    machine.idle()
                # Se imprime un mensaje de que se conectó
                print('WLAN connection succeeded!')
                # Se sale del bucle
                break

    # Se crea el método lte_connection
    def lte_connection(self):
        # Se adjunta el módulo LTE al servicio de red de Claro
        self.lte.attach(band=28, apn="igprs.claro.com.ar")
        # Se adjunta el módulo LTE al servicio de red de Personal
        #self.lte.attach(band=28, apn="internet.personal.com.ar")
        
        # Se espera a que se adjunte
        while not self.lte.isattached():
            # Se pone en modo idle para ahorrar energía
            machine.idle()
        # Se imprime un mensaje de que se adjuntó
        print('LTE attached!')

        # Se conecta al servicio de red
        self.lte.connect()
        # Se espera a que se conecte
        while not self.lte.isconnected():
            # Se pone en modo idle para ahorrar energía
            machine.idle()
        # Se imprime un mensaje de que se conectó
        print('LTE connection succeeded!')
    

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
