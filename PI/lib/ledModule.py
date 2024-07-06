import pycom

# Se declara la clase LedModule
class LedModule:
    # Se crea el constructor de la clase
    def __init__(self):
        # Se apaga el modo heartbeat
        pycom.heartbeat(False)    

    # Se declara el método set_color que recibe un color
    def set_color(self, color):
        # Se verifica si el color es rojo
        if color == 'red':
            # Se enciende el LED en color rojo
            pycom.rgbled(0x7F0000)
        # Se verifica si el color es verde
        elif color == 'green':
            # Se enciende el LED en color verde
            pycom.rgbled(0x007F00)
        # Se verifica si el color es azul
        elif color == 'blue':
            # Se enciende el LED en color azul
            pycom.rgbled(0x00007F)
        # Se verifica si el color es blanco
        elif color == 'white':
            # Se enciende el LED en color blanco
            pycom.rgbled(0x7F7F7F)
        # Se verifica si el color es amarillo
        elif color == 'yellow':
            # Se enciende el LED en color amarillo
            pycom.rgbled(0x7F7F00)
        # Se verifica si el color es rosa
        elif color == 'pink':
            # Se enciende el LED en color rosa
            pycom.rgbled(0x7F007F)
        # Se verifica si el color es cyan
        elif color == 'cyan':
            # Se enciende el LED en color cyan
            pycom.rgbled(0x007F7F)
        # Se verifica si el color es naranja
        elif color == 'orange':
            # Se enciende el LED en color naranja
            pycom.rgbled(0xD35400)
        # Se verifica si el color es morado
        elif color == 'purple':
            # Se enciende el LED en color morado
            pycom.rgbled(0x16003F)
        