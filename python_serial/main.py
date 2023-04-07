#!/usr/bin/env python3

import serial
	
ser = serial.Serial()

ser.baudrate = 115200

ser.port = '/dev/cu.usbmodem1234561'

ser.open()
ser.write(bytes('GENRAND','UTF-8'))

total = 0
while total < 256/8:
    print(ser.read(1))
    total=total+1
ser.close()