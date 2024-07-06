from network import LTE
import time

# Se declara la clase Lte
class Lte:
    # Se crea el constructor de la clase
    def __init__(self):
        # Se crea una instancia de la clase LTE
        self.lte = LTE()

    # Se crea el metodo attach
    def attach(self, empresa):
        # Se inicializa el modem LTE
        self.lte.init()
        
        # Se verifica que la empresa sea claro o personal
        if empresa == "claro":
            # Se conecta a la red LTE
            self.lte.attach(band=28, apn="igprs.claro.com.ar")
        elif empresa == "personal":
            # Se conecta a la red LTE
            self.lte.attach(band=28, apn="datos.personal.com")
        
        # Se impirme un mensaje con la empresa
        print('Conectando a la red LTE de ' + empresa)
        
        # Se consulta hasta que se conecte a la red
        while not self.lte.isattached():
            # Se espera 0.25 segundos
            time.sleep(0.25)
        
        # Se imprime que se encendio la antena LTE
        print('Antena LTE encendida')

    # Se crea el metodo detach
    def detach(self):
        # Se desconecta de la red LTE
        self.lte.detach()
        
        # Se imprime que se apago la antena LTE
        print('Antena LTE apagada')

    # Se crea el metodo connect
    def connect(self):
        # Se conecta a la red LTE
        self.lte.connect()
        
        # Se consulta hasta que se conecte a la red
        while not self.lte.isconnected():
            # Se espera 0.25 segundos
            time.sleep(0.25)
        
        # Se imprime que se conecto a la red LTE
        print('Conectado a la red LTE')

    # Se crea el metodo disconnect
    def disconnect(self):
        # Se desconecta de la red LTE
        self.lte.disconnect()
        
        # Se imprime que se desconecto de la red LTE
        print('Desconectado de la red LTE')

    # Se crea el metodo que retorna el rssi de la señal
    def getRssi(self):
        # Se ejecuta el comando para obtener el RSSI
        response = self.lte.send_at_cmd('AT+CSQ')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se parsea la respuesta para obtener el RSSI
            rssi = response.rstrip()[:-2].split(':')[1].split(',')[0].strip()
            # Se retorna el RSSI con su correccion
            return -113 + 2 * int(rssi)
        else:
            # Se imprime que hubo un error al obtener el RSSI
            print('Error al obtener el RSSI')

    # Se crea el metodo que retorna el IMEI
    def getImei(self):
        # Se ejecuta el comando para obtener el IMEI
        response = self.lte.send_at_cmd('AT+CGSN')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se parsea la respuesta para obtener el IMEI
            imei = response.strip().rstrip()[:-2]
            # Se retorna el IMEI sin espacios
            return imei.strip()
        else:
            # Se imprime que hubo un error al obtener el IMEI
            print('Error al obtener el IMEI')

    # Se crea el metodo que retorna la IP
    def getIp(self):
        # Se ejecuta el comando para obtener la IP
        response = self.lte.send_at_cmd('AT+CGPADDR')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se parsea la respuesta para obtener la IP
            response = response.strip().rstrip()[:-2].split(':')[1].split(',')[1].replace('"','').strip()
            # Se retorna la IP
            return response
        else:
            # Se imprime que hubo un error al obtener la IP
            print('Error al obtener la IP')

    # Se crea el metodo que retorna el registro
    def getRegistration(self):
        # Se ejecuta el comando para agregar informacion de registro
        response = self.lte.send_at_cmd('AT+CREG=2')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene la respuesta del comando
            response = response.strip()
            
            # Se verifica si se agrego la informacion de registro
            if response == 'OK':
                # Se imprime que se agrego la informacion de registro
                print('Se agrego informacion de registro')
                
                # Se ejecuta el comando para obtener el registro
                response = self.lte.send_at_cmd('AT+CREG?')
                
                # Se verifica que la respuesta no sea vacia
                if response:
                    # Se obtiene la informacion del registro
                    registration_info, registration_type, registration_lac, registration_ci, registration_act = response.rstrip()[:-2].split(':')[1].split(',')

                    # Se convierte la informacion a string
                    registration_info = registration_info.strip()
                    registration_type = registration_type.strip()
                    registration_lac = registration_lac.strip()
                    registration_ci = registration_ci.strip()
                    registration_act = registration_act.strip()

                    # Se verifica el tipo de registro que va a realizar y se cambia el estado
                    if registration_info == '0':
                        registration_info = 'Desactivar el código de resultado no solicitado de registro de red'
                    elif registration_info == '1':
                        registration_info = 'Activar el código de resultado no solicitado de registro de red +CREG: <stat>'
                    elif registration_info == '2':
                        registration_info = 'Activar el código de resultado no solicitado de registro de red e información de ubicación +CREG: <stat>[, [<lac>], [<ci>], [<AcT>]]'
                    elif registration_info == '3':
                        registration_info = 'Activar el código de resultado no solicitado de registro de red, información de ubicación e información de valor de causa +CREG: <stat>[, [<lac>], [<ci>], [<AcT>]][, <cause_type>, <reject_cause>]'

                    # Se verifica el tipo de tecnologia de registro y se cambia el estado
                    if registration_act == '0':
                        registration_act = 'GSM'
                    elif registration_act == '1':
                        registration_act = 'GSM Compact'
                    elif registration_act == '2':
                        registration_act = 'UTRAN'
                    elif registration_act == '3':
                        registration_act = 'GSM w/EGPRS'
                    elif registration_act == '4':
                        registration_act = 'UTRAN w/HSDPA'
                    elif registration_act == '5':
                        registration_act = 'UTRAN w/HSUPA'
                    elif registration_act == '6':
                        registration_act = 'UTRAN w/HSDPA and HSUPA'
                    elif registration_act == '7':
                        registration_act = 'E-UTRAN'

                    # Se verifica el tipo de registro y se cambia el tipo
                    if registration_type == '0':
                        registration_type = 'No registrado, MT no está buscando actualmente un operador para registrarse'
                    elif registration_type == '1':
                        registration_type = 'Registrado, red domestica'
                    elif registration_type == '2':
                        registration_type = 'No registrado, MT está buscando actualmente un nuevo operador para registrarse'
                    elif registration_type == '3':
                        registration_type = 'Rechazado'
                    elif registration_type == '4':
                        registration_type = 'Desconocido'
                    elif registration_type == '5':
                        registration_type = 'Registrado, roaming'
                    elif registration_type == '6':
                        registration_type = 'Registrado, solo SMS, red doméstica'
                    elif registration_type == '7':
                        registration_type = 'Registrado, solo SMS, roaming'
                    elif registration_type == '8':
                        registration_type = 'Sólo servicios de emergencia'
                    elif registration_type == '9':
                        registration_type = 'Registrado, CSFB no preferido red domestica'
                    elif registration_type == '10':
                        registration_type = 'Registrado, CSFB no preferido roaming'

                    # Se retorna el estado del registro y el tipo de registro
                    return registration_info, registration_type, registration_act
                else:
                    # Se imprime que hubo un error al obtener el registro
                    print('Error al obtener el registro')
            else:
                # Se imprime que hubo un error al agregar informacion de registro
                print('Error al agregar informacion de registro')        

    # Se crea el metodo que retorna el operador
    def getOperator(self):
        # Se ejecuta el comando para obtener el operador
        response = self.lte.send_at_cmd('AT+COPS=?')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el nombre del operador
            operator_name = response.rstrip()[:-2].split(':')[1].split(',')[2].replace('"','').strip()
            # Se retorna el nombre del operador
            return operator_name
        else:
            # Se imprime que hubo un error al obtener el nombre del operador
            print('Error al obtener el nombre del operador.')

    # Se crea el metodo que retorna el numero de telefono
    def getPhoneNumber(self):
        # Se ejecuta el comando para obtener el numero de telefono
        response = self.lte.send_at_cmd('AT+CNUM')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el numero de telefono
            number = "+" + response.rstrip()[:-2].split(':')[1].replace(',','').strip()
            # Se retorna el numero de telefono
            return number
        else:
            # Se imprime que hubo un error al obtener el numero de telefono
            print('Error al obtener el numero de telefono.')

    # Se crea el metodo que retorna el estado de la conexion
    def getConnectionStatus(self):
        # Se ejecuta el comando para obtener el estado de la conexion
        response = self.lte.send_at_cmd('AT+CGATT?')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el estado de la conexion
            connection_status = response.rstrip()[:-2].split(':')[1].strip()

            # Se verifica el estado de la conexion y se cambia el estado
            if connection_status == '0':
                connection_status = 'Desconectado'
            elif connection_status == '1':
                connection_status = 'Conectado'
            
            # Se retorna el estado de la conexion
            return connection_status
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el estado de la conexion.')

    # Se crea el metodo que retorna el modelo del dispositivo
    def getModel(self):
        # Se ejecuta el comando para obtener el modelo del dispositivo
        response = self.lte.send_at_cmd('AT+CGMM')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el modelo del dispositivo
            model = response.rstrip()[:-2].strip()

            # Se retorna el modelo del dispositivo
            return model
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el modelo del dispositivo.')

    # Se crea el metodo que retorna el nombre del fabricante
    def getManufacturer(self):
        # Se ejecuta el comando para obtener el nombre del fabricante
        response = self.lte.send_at_cmd('AT+CGMI')
        
        # Se verifica que la respuesta no sea vacia
        if response:
            # Se obtiene el nombre del fabricante
            manufacturer_name = response.rstrip()[:-2].strip()

            # Se retorna el nombre del fabricante
            return manufacturer_name
        else:
            # Se imprime un mensaje de error
            print('Error al obtener el nombre del fabricante.')