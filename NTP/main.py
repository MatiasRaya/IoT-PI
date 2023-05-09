import time
import machine
import connection

# Se crea una instancia de la clase Connection
connection = connection.Connection()

# Se llama al método wifi_connection
connection.wifi_connection()

# Se crea una instancia de la clase RTC
rtc = machine.RTC()

# Se sincroniza el RTC con el NTP
rtc.ntp_sync("pool.ntp.org", update_period=15)

# Se espera a que el RTC se sincronice
while not rtc.synced():
    # Se pone en modo idle para ahorrar energía
    machine.idle()

# Se imprime un mensaje de que el RTC se sincronizó
print("RTC synced with NTP time")

# Se ajusta el RTC a la hora local
time.timezone((-3)*60**2)

# Se realiza un bucle infinito
while True:
    # Se imprime la hora local
    print(time.localtime())
    # Se espera un segundo
    time.sleep(1)