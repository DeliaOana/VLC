#!/usr/bin/env.python
import spidev
import os
import math
import time
import RPi.GPIO as GPIO

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