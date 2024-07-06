import time
import machine

# Se crea la clase NTP
class NTP:
    # Se crea el constructor de la clase
    def __init__(self, ntp_server, update_period, timezone):
        # Se crea un objeto RTC
        self.rtc = machine.RTC()
        
    # Se declara el método sincronize
    def sincronize(self, ntp_server, update_period, timezone):
        # Se sincroniza el reloj RTC con el servidor NTP
        self.rtc.ntp_sync(ntp_server, update_period=update_period)

        # Se espera a que el reloj RTC se sincronice
        while not self.rtc.synced():
            # Se espera un segundo
            time.sleep(1)

        # Se imprime que el RTC se sincronizó
        print('RTC sincronizado')

        # Se establece la zona horaria
        time.timezone((timezone)*60**2)