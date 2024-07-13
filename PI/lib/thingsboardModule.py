import urequest

# Se declara la clase Thingsboard
class Thingsboard:
    # Se crea el constructor de la clase
    def __init__(self, tb_host, tb_port):
        # Se almacenan los parametros en variables de la clase
        self.tb_host = tb_host
        self.tb_port = tb_port
    
    # Se crea el metodo getAuthToken
    def getAuthToken(self, username, password):
        # Se declara el arreglo headers con los valores de Content-Type y Accept
        headers = {
            "Content-Type": "application/json",
            "Accept": "application/json"
        }

        # Se declara el arreglo data con el usuario y contraseña
        data = {
            "username": username,
            "password": password
        }

        # Se declara la variable url con la direccion del servidor Thingsboard
        url = "http://" + self.tb_host + ":" + str(self.tb_port) + "/api/auth/login"

        # Se realiza la peticion POST al servidor Thingsboard
        response = urequest.post(url, json=data, headers=headers)

        # Se obtiene el token de la respuesta
        token = response.json()["token"]

        # Se retorna el token
        return token
    
    # Se crea el metodo getDeviceData
    def getDeviceData(self, token, device_id):
        # Se declara el arreglo headers con los valores de Content-Type y Accept
        headers = {
            "Content-Type": "application/json",
            "Accept": "application/json",
            "X-Authorization": "Bearer " + token
        }

        # Se declara la variable url con la direccion del servidor Thingsboard
        url = "http://" + self.tb_host + ":" + str(self.tb_port) + "/api/plugins/telemetry/DEVICE/" + device_id + "/values/attributes/SHARED_SCOPE"

        # Se realiza la peticion GET al servidor Thingsboard
        response = urequest.get(url, headers=headers)

        # Se obtiene el dispositivo de la respuesta
        device = response.json()

        # Se retorna el dispositivo
        return device
    
    # Se crea el metodo postDeviceData
    def postDeviceData(self, token, device_id, data):
        # Se declara el arreglo headers con los valores de Content-Type y Accept
        headers = {
            "Content-Type": "application/json",
            "Accept": "application/json",
            "X-Authorization": "Bearer " + token
        }

        # Se declara la variable url con la direccion del servidor Thingsboard
        url = "http://" + self.tb_host + ":" + str(self.tb_port) + "/api/plugins/telemetry/DEVICE/" + device_id + "/SHARED_SCOPE"

        # Se realiza la peticion POST al servidor Thingsboard
        response = urequest.post(url, json=data, headers=headers)

        # Se llama al metodo getDeviceData para obtener el dispositivo actualizado
        device = self.getDeviceData(token, device_id)

        # Se retorna el dispositivo
        return device