from network import LTE
import time

# Se declara la clase Lte
class Lte:
    # Se crea el constructor de la clase
    def __init__(self):
        # Se crea una instancia de la clase LTE
        self.lte = LTE()

    # Se crea el metodo attach
    def attach(self):
        # Se inicializa el modem LTE
        self.lte.init()
        # Se conecta a la red LTE
        self.lte.attach(band=28, apn="igprs.claro.com.ar")
        # lte.attach(band=28, apn="datos.personal.com")
        # Se consulta hasta que se conecte a la red
        while not self.lte.isattached():
            # Se espera 0.25 segundos
            time.sleep(0.25)

        # Se imprime un mensaje de exito
        print('Antena LTE encendida')

    # Se crea el metodo detach
    def detach(self):
        # Se desconecta de la red LTE
        self.lte.detach()
        # Se imprime un mensaje de exito
        print('Antena LTE apagada')

    # Se crea el metodo connect
    def connect(self):
        # Se conecta a la red LTE
        self.lte.connect()
        # Se consulta hasta que se conecte a la red
        while not self.lte.isconnected():
            # Se espera 0.25 segundos
            time.sleep(0.25)

        # Se imprime un mensaje de exito
        print('Conectado a la red LTE')

    # Se crea el metodo disconnect
    def disconnect(self):
        # Se desconecta de la red LTE
        self.lte.disconnect()
        # Se imprime un mensaje de exito
        print('Desconectado de la red LTE')

    # Se crea el metodo que retorna el rssi de la señal
    def getRssi(self):
        # Se obtiene la calidad de la señal
        response = self.lte.send_at_cmd('AT+CSQ')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el RSSI
            rssi = response.rstrip()[:-2].split(':')[1].split(',')[0].strip()
            # Se retorna el RSSI
            return rssi
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el RSSI.')

    # Se crea el metodo que retorna el IMEI
    def getImei(self):
        # Se obtiene el IMEI
        response = self.lte.send_at_cmd('AT+CGSN')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se imprime el IMEI
            imei = response.strip().rstrip()[:-2]
            return imei.strip()
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el IMEI.')

    # Se crea el metodo que retorna la IP
    def getIp(self):
        # Se obtiene la IP
        response = self.lte.send_at_cmd('AT+CGPADDR')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se imprime la IP
            response = response.strip().rstrip()[:-2].split(':')[1].split(',')[1].replace('"','').strip()
            return response
        else:
            # Se imprime un mensaje de error
            print('Error al obtener la IP.')

    # Se crea el metodo que retorna el registro
    def getRegistration(self):
        # Se obtiene el registro
        response = self.lte.send_at_cmd('AT+CREG?')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el estado del registro y el tipo de registro
            registration_status, registration_type = response.rstrip()[:-2].split(':')[1].split(',')
            return registration_status, registration_type.strip()
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el registro.')

    # Se crea el metodo que retorna el operador
    def getOperator(self):
        # Se obtiene el operador
        response = self.lte.send_at_cmd('AT+COPS=?')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el nombre del operador
            operator_name = response.rstrip()[:-2].split(':')[1].split(',')[2].replace('"','').strip()
            return operator_name
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el nombre del operador.')

    # Se crea el metodo que retorna el numero de telefono
    def getPhoneNumber(self):
        # Se obtiene el numero de telefono
        response = self.lte.send_at_cmd('AT+CNUM')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el numero de telefono
            number = "+" + response.rstrip()[:-2].split(':')[1].replace(',','').strip()
            return number
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el numero de telefono.')

    # Se crea el metodo que retorna el estado de la conexion
    def getConnectionStatus(self):
        # Se obtiene el estado de la conexion
        response = self.lte.send_at_cmd('AT+CGATT?')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el estado de la conexion
            connection_status = response.rstrip()[:-2].split(':')[1].strip()
            return connection_status
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el estado de la conexion.')

    # Se crea el metodo que retorna el modelo del dispositivo
    def getModel(self):
        # Se obtiene el modelo del dispositivo
        response = self.lte.send_at_cmd('AT+CGMM')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el modelo del dispositivo
            model = response.rstrip()[:-2].strip()
            return model
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el modelo del dispositivo.')

    # Se crea el metodo que retorna el nombre del fabricante
    def getManufacturer(self):
        # Se obtiene el nombre del fabricante
        response = self.lte.send_at_cmd('AT+CGMI')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el nombre del fabricante
            manufacturer_name = response.rstrip()[:-2].strip()
            return manufacturer_name
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el nombre del fabricante.')

    # Se crea el metodo que retorna la version del firmware
    def getFirmwareVersion(self):
        # Se obtiene la version del firmware
        response = self.lte.send_at_cmd('AT+CGMR')
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene la version del firmware
            firmware_version = response.rstrip()[:-2].strip()
            return firmware_version
        else:
            # Se imprime un mensaje de error
            print('Error al obtener la version del firmware.')
