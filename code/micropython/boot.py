# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()

try:
  import usocket as socket
except:
  import socket

from machine import Pin, PWM
import network

import esp
esp.osdebug(None)

import gc
gc.collect()

from math import floor

ssid = 'Bibliotheek'
password = 'Vakantie'

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())

led = PWM(Pin(10), freq=1000)
