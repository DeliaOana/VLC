#!/usr/bin/env.python
import spidev
import os
import math
import time
import pigpio
import math
#import read_PWM
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(4, GPIO.OUT)
GPIO.setup(24, GPIO.IN)
GPIO.setup(23, GPIO.OUT)
GPIO.output(23, GPIO.HIGH)
GPIO.setup(18, GPIO.IN)


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

def writeByte(val, nr_biti):
    GPIO.output(4, GPIO.LOW)
    #time.sleep(.035)
    format_biti = "{0:0" + str(nr_biti) + "b}"
    bits = format_biti.format(val)
    print("BITI = " + str(bits))
    for i in range (0,nr_biti):
        if bits[i] == '1':
            GPIO.output(4, GPIO.HIGH)
        else:
            GPIO.output(4, GPIO.LOW)
        time.sleep(.0031)
    GPIO.output(4, GPIO.LOW)
    time.sleep(0.0031)

while True:
    temp = ReadInput(0)
    move = ReadInput(1)
    dust = ReadInput(2)
    PI = ReadInput(4)
    print('Temp = ' + str(ConvertVolts(temp,4)*100))
    print('Dust = ' + str(ConvertVolts(dust,4)))

    
    
    ########LIGHT########
    cnt1 = 0
    while cnt1 == 0:
        if GPIO.input(18) == 0:
            while GPIO.input(18)==0:
                d=2
            t = time.time()
            while GPIO.input(18) == 1:
                d=2
            while GPIO.input(18) == 0:
                d=2
            durata_perioada = time.time() - t
            cnt1 = cnt1 + 1         
    f = 1 / durata_perioada
    print('Lumina  = ' + str(math.trunc(f)))
    ##############################
    
    if (move<512):
       print ("fara miscare")
       move = 0
    else:
       print ("miscare")
       move = 1
    #f=p.frequency()
       
    #sound from here
    cnt=0
    cntr=0
    nr_perioade = 0
    contor = 0                          
    t=time.time()
    while time.time()-t < 5:
        #sound
        cnt=cnt+1
        sound = ReadInput(3)
        if sound < 220:
            cntr=cntr+1      
            
    if cntr > cnt/100:
        print ('zgomot')
        sound = 1
    else:
        print ('liniste')
        sound = 0
    #to here
    GPIO.output(4, GPIO.HIGH)
    time.sleep(.0031)
    writeByte(temp,10)
    time.sleep(.0031)
    writeByte(dust,10)
    time.sleep(.0031)
    writeByte(math.trunc(f),18)
    time.sleep(.0031)
    writeByte(move,1)
    time.sleep(.0031)
    writeByte(sound,1)

    #print('Temp = ' + str(ConvertVolts(temp,4)*100))
    #print('Dust = ' + str(ConvertVolts(dust,4)))
    #print('dust = ' + str(dust))
    print('------------------')
    #print('Lumina  = ' + str(math.trunc(f)))
    print(' ')
    #print('PI  = ' + str (PI) + '  ' + str(ConvertVolts(PI,4)))
    time.sleep(1)
