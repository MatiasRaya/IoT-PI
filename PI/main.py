from lteModule import Lte
from wifiModule import Wifi
from rotatingLogger import RotatingLogger
from ntpModule import NTP
from configModule import ConfigModule
from ledModule import LedModule

from ftpadvanced import AdvancedFTP

wifi = Wifi('pruebaAP', 'STA')
wifi.scan()
wifi.wifi_connect('Marga', 'Milanesa6124')

ftp = AdvancedFTP('192.168.1.180', 21)
ftp.login('ftpuser', 'ftpuser')
files = ftp.nlst()
print(files)

filename = 'prueba.txt'

with open(filename, 'wb') as file:
    ftp.retrbinary('RETR ' + filename, file.write)

filename2 = 'prueba.py'
with open(filename2, 'wb') as file:
    ftp.retrbinary('RETR ' + filename2, file.write)

ftp.quit()

print('Archivo descargado')

# Se copia el archivo descargado a la SD

import os
import machine

# Se crea un objeto de la clase machine para acceder a la tarjeta SD
sd = machine.SD()

# Se monta la tarjeta SD
os.mount(sd, '/sd')

dirs = os.listdir('/sd')

print(dirs)

destination_dir = '/sd/'

try:
    with open(filename, 'rb') as src_file:
        with open(destination_dir + filename, 'wb') as dest_file:
            while True:
                buf = src_file.read(1024)
                if not buf:
                    break
                dest_file.write(buf)
    print("Archivo copiado a '{}{}' exitosamente.".format(destination_dir, filename))
except OSError as e:
    print("No se pudo copiar el archivo: {}".format(e))

try:
    with open(filename2, 'rb') as src_file:
        with open(destination_dir + filename2, 'wb') as dest_file:
            while True:
                buf = src_file.read(1024)
                if not buf:
                    break
                dest_file.write(buf)
    print("Archivo copiado a '{}{}' exitosamente.".format(destination_dir, filename2))
except OSError as e:
    print("No se pudo copiar el archivo: {}".format(e))
    
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
