from lteModule import Lte
from wifiModule import Wifi
from rotatingLogger import RotatingLogger
from ntpModule import NTP
from configModule import ConfigModule
from ledModule import LedModule
from ftpModule import Ftp
from thingsboardModule import Thingsboard

wifi = Wifi('pruebaAP', 'STA')
wifi.scan()
wifi.wifi_connect('Marga', 'Milanesa6124')

thingsboard = Thingsboard('192.168.1.180', 9090)
token = thingsboard.getAuthToken('tenant@thingsboard.org', 'tenant')
print(token)
device = thingsboard.getDeviceData(token, "8d0b7900-34e1-11ef-9fcb-c39c20a6cc8b")
print(device)
data = {
    "phone number": "1234567890"
}
response = thingsboard.postDeviceData(token, "8d0b7900-34e1-11ef-9fcb-c39c20a6cc8b", data)
print(response)

# ftp = Ftp('192.168.1.180', 21, 'ftpuser', 'ftpuser')
# files = ftp.list_files()
# print(files)

# filename = 'prueba.txt'

# ftp.download_file(filename)

# filename2 = 'app.txt'

# ftp.upload_file(filename2)
    
# # Se crea una instancia de la clase LedModule
# ledModule = LedModule()

# ledModule.set_color('purple')

# # Se declara el nombre del archivo de configuracion
# config_file = 'app.txt'

# # Se crea una instancia de la clase ConfigModule
# configModule = ConfigModule(config_file)

# # Se obtiene la configuracion desde el archivo
# config = configModule.get_config(config_file)

# # Se imprime la configuracion
# print(config)

# print('Zona horaria: {}'.format(int(config['NTP']['timezone'])))

# # Se crea una instancia de la clase Lte
# lte = Lte()

# lte.attach(config['LTE']['company'])
# lte.connect()

# # Se crea una instancia de la clase NTP
# ntp = NTP()
# ntp.sincronize(update_period=int(config['NTP']['update_period']), timezone=int(config['NTP']['timezone']), ntp_server=config['NTP']['ntp_server'])

# rotatingLogger = RotatingLogger(log_file=config['Log']['fileName'], log_directory=config['Log']['directoryName'], maxFileSize=int(config['Log']['maxFileSize']), maxFiles=int(config['Log']['maxNumberFiles']))

# count = 0

# while True:
#     rotatingLogger.log('Hola mundo ' + str(count) + '\n')
#     count += 1
#     time.sleep(1)
