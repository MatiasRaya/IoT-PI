from connection import Connection
import urequests
import ujson

# Se crea una instancia de la clase Connection
connection = Connection()

# Se conecta a la red WiFi
connection.wifi_connection('RAYADOS 2.4', 'Rayaplasencia1996')

# Se configura el dispositivo, la variable y la query
device = '64594d59d5e9a200092b7ce1'
variable = 'bt'
query = 'last_item'

# Se configura la URL del servicio de TagoIO con los parámetros de la variable y el dispositivo
url = "https://api.tago.io/data?variable={}&device={}&query={}".format(variable, device, query)

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

# Se convierte la respuesta a un diccionario
data = ujson.loads(respose.text)

# Se obtiene el valor de la variable
value = data['result'][0]['value']

# Se imprime el valor de la variable y el nombre de la variable
print('Value de {}: {}'.format(variable, value))