from lteModule import Lte

# Se crea una instancia de la clase Lte
lte = Lte()

# Se enciende la antena
lte.attach()

# Se obtiene el RSSI
rssi = lte.getRssi()

# Se verifica el valor del rssi con su correspondiente valor en dBm
valueRssi = -113 + 2 * int(rssi)

# Se imprime el RSSI
print('RSSI: ' + str(valueRssi) + ' dBm')

# Se obtiene el IMEI
imei = lte.getImei()

# Se imprime el IMEI
print('IMEI: ' + imei)

# Se obtiene la IP
ip = lte.getIp()

# Se imprime la IP
print('IP: ' + ip)

# Se verifica si se encuentra conectado a la red
registration_status, registration_type = lte.getRegistration()

# Se verifica si se encuentra registrado en la red
if registration_status == '1':
    registration_status = 'Registrado'
elif registration_status == '0':
    registration_status = 'No registrado'

# Se verifica el tipo de registro
if registration_type == '0':
    registration_type = 'Not registered, MT is not currently searching an operator to register to'
elif registration_type == '1':
    registration_type = 'Home Network'
elif registration_type == '2':
    registration_type = 'Not registered, MT is currently searching a new operator to register to'
elif registration_type == '3':
    registration_type = 'Denied'
elif registration_type == '4':
    registration_type = 'Unknown'
elif registration_type == '5':
    registration_type = 'Registered, roaming'
elif registration_type == '6':
    registration_type = 'Registered, SMS only, home network'
elif registration_type == '7':
    registration_type = 'Registered, SMS only, roaming'
elif registration_type == '8':
    registration_type = 'Emergency services only'
elif registration_type == '9':
    registration_type = 'Registered, CSFB not preferred, home network'
elif registration_type == '10':
    registration_type = 'Registered, CSFB not preferred, roaming'

# Se imprime el registro
print('Registro: ' + registration_status + ' - ' + registration_type)

# Se obtiene el operador
operator = lte.getOperator()

# Se imprime el operador
print('Operador: ' + operator)

# Se obtiene el numero de telefono
phone_number = lte.getPhoneNumber()

# Se imprime el numero de telefono
print('Numero de telefono: ' + phone_number)

# Se obtiene el estado de conexion
connection_status = lte.getConnectionStatus()

# Se imprime el estado de conexion
print('Estado de la conexion: ' + connection_status)

# Se obtiene el modelo del modulo
model = lte.getModel()

# Se imprime el modelo del modulo
print('Modelo: ' + model)

# Se obtiene el fabricante del modulo
manufacturer = lte.getManufacturer()

# Se imprime el fabricante del modulo
print('Fabricante: ' + manufacturer)

# Se obtiene la version del firmware
firmware_version = lte.getFirmwareVersion()

# Se imprime la version del firmware
print('Version del firmware: ' + firmware_version)

# Se conecta a la red LTE
lte.connect()

# Se apaga la antena
lte.detach()

# Se desconecta de la red LTE
lte.disconnect()
