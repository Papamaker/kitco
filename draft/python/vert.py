#!/usr/bin/python
import serial
ser = serial.Serial('/dev/ttyUSB0',9600)
ser.write('v')

