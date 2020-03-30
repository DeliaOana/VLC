#!/usr/bin/env.python
import spidev
import os
import math
import time
import pigpio
import math
#import read_PWM
import RPi.GPIO as GPIO
import json
import paho.mqtt.publish as publish

MQTT_HOST = 'mqtt.beia-telemetrie.ro'
MQTT_TOPIC = 'odsi/vlc'


spi=spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz=1000000

def ReadInput(channel):
    adc = spi.xfer2([1,(8+channel)<<4,0])
    data = ((adc[1]&3)<<8)+adc[2]
    return data

def ConvertVolts (data, places):
    volts = (data*3.3)/float(1023)
    volts = round(volts,places)
    return volts

#functia de citire
def readByte(nr_biti):
    vector_biti = []
    time.sleep(.0031)
    for i in range (0,nr_biti):
        sensorValue = ReadInput(7)  #trebuie setatat pinul de input de la fototranzistor
        if sensorValue <= 3*middleValue:  #/True?
            vector_biti.append(0)
        else:
            vector_biti.append(1)
        time.sleep(.0031)
    val_citita = int("".join(str(x) for x in vector_biti), 2)
    time.sleep(0.0031)
    print(vector_biti)
    return val_citita

t = time.time()
suma = 0
cnt = 0
while time.time() - t < 5:
    suma = suma + ReadInput(7)
    cnt = cnt +1
middleValue = suma/cnt
'''
while True:
    x = ReadInput(7)
    print(x)
    #print(ConvertVolts(x,3))
    print("Middle = " + str(middleValue))
    time.sleep(0.5)
'''
#
while True:
    if ReadInput(7) > 3*middleValue:
        temp = readByte(10)
        dust = readByte(10)
        light = readByte(18)
        move = readByte(1)
        sound = readByte(1)

        print('Temp =  ' + str(ConvertVolts(temp,4)*100))
        print('Dust = ' + str(ConvertVolts(dust,4)))
        print('Lumina = ' + str(light))
        if move == 0:
            print("fara miscare")
        else:
            print("miscare")
            
        if sound == 0:
            print("liniste")
        else:
            print("zgomot")
            
        payload_dict = {"TEMP" : ConvertVolts(temp,4)*100,
                        "DUST" : ConvertVolts(dust,4),
                        "LUMINA" : light,
                        "MISCARE" : move,
                        "SUNET" : sound}
        try:
            publish.single(MQTT_TOPIC, qos = 1, hostname = MQTT_HOST, payload = json.dumps(payload_dict))
        except:
            time.sleep(0.01)