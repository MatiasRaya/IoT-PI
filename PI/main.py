from lteModule import Lte
from wifiModule import Wifi
from rotatingLogger import RotatingLogger
from ntpModule import NTP
from configModule import ConfigModule
from ledModule import LedModule

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
