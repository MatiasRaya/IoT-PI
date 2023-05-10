import machine
import math
from network import WLAN
import os
import time
import utime
import gc
import pycom
from machine import RTC
from machine import SD
from L76GNSS import L76GNSS
from pycoproc_2 import Pycoproc

pycom.heartbeat(False)
pycom.rgbled(0x0A0A08) # white

time.sleep(2)
gc.enable()

wlan = WLAN(mode=WLAN.STA)
wlan.antenna(WLAN.EXT_ANT)
wlan.connect('RAYADOS 2.4', auth=(WLAN.WPA2, 'Rayaplasencia1996'), timeout=5000)
while not wlan.isconnected():
    machine.idle()
print("Connected to Wifi\n")
print(wlan.ifconfig())

# setup rtc
rtc = machine.RTC()
rtc.ntp_sync("pool.ntp.org", update_period=15)
utime.sleep_ms(750)
print('\nRTC Set from NTP to UTC:', rtc.now())
utime.timezone((-3)*60**2)
print('Adjusted from UTC to EST timezone', utime.localtime(), '\n')

py = Pycoproc()
if py.read_product_id() != Pycoproc.USB_PID_PYTRACK:
    raise Exception('Not a Pytrack')

time.sleep(1)
l76 = L76GNSS(py, timeout=30, buffer=512)

# sd = SD()
# os.mount(sd, '/sd')

while (True):
    # f = open('/sd/gps-record.txt', 'a')
    coord = l76.coordinates()
    # f.write("{} - {}\n".format(coord, rtc.now()))
    # f.flush()
    # f.close()

    # f = open('/sd/gps-record.txt', 'r')
    # print(f.read())
    # f.close()
    print("{} - {} - {} - {}".format(coord, utime.localtime(), rtc.now(), gc.mem_free()))
