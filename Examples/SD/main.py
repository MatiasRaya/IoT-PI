import machine
import os
import pycom
import time

# Configure los pines de los LED
LED_ESCRIBIENDO = 0x0000FF  # Azul
LED_LEYENDO = 0xFFA500    # Naranja
LED_MOSTRANDO = 0x00FF00   # Verde

# Configure el pin de la tarjeta SD
sd = machine.SD()

# Montar la tarjeta SD
pycom.rgbled(LED_ESCRIBIENDO)
os.mount(sd, '/sd')
pycom.rgbled(0x000000)  # Apagar los LED
time.sleep(1)  # Esperar 1 segundo

# Escribir el archivo en la tarjeta SD
pycom.rgbled(LED_ESCRIBIENDO)
with open('/sd/archivo.txt', 'w') as f:
    f.write('Hola, mundo!')
pycom.rgbled(0x000000)  # Apagar los LED
time.sleep(1)  # Esperar 1 segundo

# Leer el archivo de la tarjeta SD
pycom.rgbled(LED_LEYENDO)
with open('/sd/archivo.txt', 'r') as f:
    contenido = f.read()
pycom.rgbled(0x000000)  # Apagar los LED
time.sleep(1)  # Esperar 1 segundo

# Mostrar el contenido por pantalla
pycom.rgbled(LED_MOSTRANDO)
print(contenido)
pycom.rgbled(0x000000)  # Apagar los LED
time.sleep(1)  # Esperar 1 segundo

# Desmontar la tarjeta SD
os.umount('/sd')

