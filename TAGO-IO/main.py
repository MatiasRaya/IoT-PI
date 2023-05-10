from connection import Connection
import urequests
import ujson
import utime

# Se crea una instancia de la clase Connection
connection = Connection()

# Se conecta a la red WiFi
connection.wifi_connection('RAYADOS 2.4', 'Rayaplasencia1996')

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

# Se convierte la respuesta a un diccionario
data = ujson.loads(respose.text)

# Se obtiene el valor de la variable
value = data['result'][0]['value']

# Se obtiene la fecha y hora de la variable
time = data['result'][0]['time']

# Se imprime el valor de la variable
print(value)
print(time)

# Se calcula la diferencia horaria entre GMT y GMT-3 en segundos
gmt_offset = -3 * 60 * 60

# Se convierte la cadena de tiempo a una tupla de tiempo
time_tuple = utime.gmtime(utime.mktime((int(time[0:4]), int(time[5:7]), int(time[8:10]), int(time[11:13]), int(time[14:16]), int(time[17:19]), 0, 0, 0)))

# Se suma la tupla de tiempo con la diferencia horaria en segundos
time_gmt3_tuple = utime.localtime(utime.mktime(time_tuple) + gmt_offset)

# Se formatea la tupla de tiempo a una cadena de tiempo
time_gmt3_str = "{:02d}/{:02d}/{:04d} {:02d}:{:02d}:{:02d}".format(time_gmt3_tuple[2], time_gmt3_tuple[1], time_gmt3_tuple[0], time_gmt3_tuple[3], time_gmt3_tuple[4], time_gmt3_tuple[5])

# Se imprime la cadena de tiempo
print(time_gmt3_str)

# Se configura el payload
payload = {
    'variable': 'bt',
    'value': False
}

# Se configuran los headers
headers = {
    'Content-Type': 'application/json',
    'device-token': '5ece922f-1549-4bfe-b2c8-616097edcec6'
}

# Se configura la URL del servicio de TagoIO
url = "https://api.tago.io/data"

# Se realiza la petición HTTP POST
response = urequests.post(url, headers=headers, data=ujson.dumps(payload))

# Se imprime la respuesta
print(response.text)