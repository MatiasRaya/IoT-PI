from network import WLAN
import machine
import urequests

# Inicializar el objeto WLAN
wlan = WLAN(mode=WLAN.STA_AP, ssid='Prueba', auth=(WLAN.WPA2, 'PruebadeAP'))

# Configurar la conexión como estación (STA)
wlan.connect(ssid='RAYADOS 2.4', auth=(WLAN.WPA2, 'Rayaplasencia1996'))
while not wlan.isconnected():
    machine.idle()

# Configurar el punto de acceso (AP)
# wlan.init(mode=WLAN.AP, ssid='Prueba', auth=(WLAN.WPA2, 'PruebadeAP'))

# Esperar a que se establezca la conexión como estación (STA)
while not wlan.isconnected():
    pass


print(wlan.ifconfig(id=1))
print(wlan.ifconfig())
print(wlan.Connected_ap_pwd())

# Se configura el dispositivo, la variable, la query y la cantidad de registros a obtener
variable = 'bt'
device = '64594d59d5e9a200092b7ce1'
sort = '-time'
qty = '1'

# Se configura la URL del servicio de TagoIO con los parámetros de la variable y el dispositivo
url = "https://api.tago.io/data?variable={}&device={}&sort={}&qty={}".format(variable, device, sort, qty)

# Se configura el payload
payload = {}

# Se configuran los headers
headers = {
    'device-token': '5ece922f-1549-4bfe-b2c8-616097edcec6'
}

# Se realiza la petición HTTP GET
respose = urequests.get(url, headers=headers, data=payload)

# Se imprime la respuesta
print(respose.text)