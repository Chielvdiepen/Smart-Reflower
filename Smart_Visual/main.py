import serial
import io
import matplotlib.pyplot as plt
from drawnow import *
import atexit

tijd = []
sensor1 = []
sensor2 = []
gemiddelde = []

plt.ion()
cnt = 0

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.flushInput()

config = [0, 900, 0, 260]


def plotvalues():
    plt.title('Smart Reflower Data logger')
    plt.axis(config)
    plt.grid(True)
    plt.ylabel('Temperatuur')
    plt.xlabel('Tijd')
    plt.plot(tijd, sensor1, 'r.-', label='sensor1')
    plt.plot(tijd, sensor2, 'b.-', label='sensor2')
    plt.plot(tijd, gemiddelde, 'g.-', label='Gemiddelde')
    plt.legend(loc='upper right')


def doAtExit():
    ser.close()
    print("Close serial")
    print("ser.isOpen() = " + str(ser.isOpen()))


atexit.register(doAtExit)

print("ser.isOpen() = " + str(ser.isOpen()))

# pre-load dummy data
for i in range(0, 900):
    tijd.append(0)
    sensor1.append(0)
    sensor2.append(0)
    gemiddelde.append(0)

while True:
    while (ser.inWaiting() == 0):
        pass
    

    Log_read = ser.readline(500)
    x = Log_read.split()

    tijd_waarde = int(x[0])
    sensor1_temp = int(x[1])
    sensor2_temp = int(x[2])
    gemiddelde_temp = int(x[3])

    print(tijd_waarde)
    tijd.append(tijd_waarde)
    sensor1.append(sensor1_temp)
    sensor2.append(sensor2_temp)
    gemiddelde.append(gemiddelde_temp)
   except
    tijd.pop(0)
    sensor1.pop(0)
    sensor2.pop(0)
    gemiddelde.pop(0)

    drawnow(plotvalues)
