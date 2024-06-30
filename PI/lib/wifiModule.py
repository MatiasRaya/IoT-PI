from network import WLAN
import time

class Wifi:
    # Se crea el constructor de la clase
    def __init__(self, ap_ssid, mode):
        # Se almacena el modo en el que se quiere configurar la antena WiFi y el SSID del AP
        self.ap_ssid = ap_ssid
        self.mode = mode

        # Se verifica que modo se quiere configurar, si se quiere configurar como STA_AP o como STA y se crea una instancia de tal
        if mode == 'STA':
            self.wlan = WLAN(mode=WLAN.STA)
        elif mode == 'STA_AP':
            self.wlan = WLAN(mode=WLAN.STA_AP, ssid=ap_ssid)
        # Se configura la antena exterior
        self.wlan.antenna(WLAN.EXT_ANT)

    # Se crea el metodo para conectarse a la red WiFi
    def wifi_connect(self, ssid, password):
        # Se conecta a la red WiFi pasada por parametria
        self.wlan.connect(ssid, auth=(WLAN.WPA2, password), timeout=100000)
        # Se consulta hasta que se conecte a la red
        while not self.wlan.isconnected():
            # Se espera 0.25 segundos
            time.sleep(0.25)

        # Se imprime un mensaje de exito
        print('Conectado a la red WiFi')
        # Se imprime la direccion IP de la conexion a la red
        print('IP address:', self.wlan.ifconfig()[0])

    # Se crea el metodo para desconectarse de la red WiFi
    def wifi_disconnect(self):
        # Se desconecta de la red WiFi
        self.wlan.disconnect()
        # Se imprime un mensaje de exito
        print('Desconectado de la red WiFi')

    # Se declara el metodo para apagar la antena WiFi
    def wifi_off(self):
        # Se apaga la antena WiFi
        self.wlan.deinit()
        # Se imprime un mensaje de exito
        print('Antena WiFi apagada')

    # Se define el metodo para setear el modo de conexion wifi
    def set_mode(self, mode):
        # Se setea el modo de conexion wifi
        self.mode = mode

    # Se define el metodo para setear el ssid del AP
    def set_ap_ssid(self, ap_ssid):
        # Se setea el ssid del AP
        self.ap_ssid = ap_ssid

    # Se declarara el metodo para encender la antena WiFi
    def wifi_on(self):
        # Se verifica el modo en el que se quiere configurar la antena WiFi
        if self.mode == 'STA':
            # Se crea una instancia de la antena WiFi en modo STA
            self.wlan = WLAN(mode=WLAN.STA)
        elif self.mode == 'STA_AP':
            # Se crea una instancia de la antena WiFi en modo STA_AP
            self.wlan = WLAN(mode=WLAN.STA_AP, ssid=self.ap_ssid)
        # Se imprime un mensaje de exito
        print('Antena WiFi encendida')

    # Se crea el metodo para listar las propiedades de los dispositivos conectados al AP
    def listAP(self):
        # Se obtiene la informacion de los dispositivos conectados al AP
        data = self.wlan.ap_sta_list()
        # Se retorna la lista de dispositivos conectados al AP
        return data
