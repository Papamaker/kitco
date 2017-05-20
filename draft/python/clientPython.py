import serial
ser = serial.Serial('/dev/ttyUSB0',9600)
chaine = str()
 
while chaine.lower() != "q":
   chaine = raw_input()
   ser.write(chaine)

