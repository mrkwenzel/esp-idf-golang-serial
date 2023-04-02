import serial
	
ser = serial.Serial()

ser.baudrate = 115200

ser.port = '/dev/cu.usbmodem14201'

ser.open()
ser.write(bytes('generatefunnyjoke','UTF-8'))

total = 0
 
while total < 82:
    print(ord(ser.read(1)))
    total=total+1
ser.close()