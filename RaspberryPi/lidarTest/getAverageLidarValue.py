from __future__ import division
'''
#-----------------!!NOTICE!!-----------------#

The robot capacitor is hooked up the same way 
as described in the other lidar test files. 

Be careful of touching wires, the negative lead
touched the capacitor and caused the other wires
near by to melt together. 

This file takes an average value of the four lidars 

PLEASE READ, values inside the DIGITALOUTPUTDEVICE is the
GPIO pin numbers, NOT actual pin number!! (i.e.
GPIO 26 = pin 37
GPIO 19 = pin 35
GPIO 13 = pin 33
GPIO  6 = pin 31
)

#--------------------------------------------#
'''
from lidar_lite import Lidar_Lite
from time import sleep
from gpiozero import DigitalOutputDevice 

lidarController = [DigitalOutputDevice(26), 
                   DigitalOutputDevice(19),
                   DigitalOutputDevice(13),
                   DigitalOutputDevice(6) ]

avgValues = [0, 0, 0, 0]

lidar = Lidar_Lite()

connected = lidar.connect(1)

if connected < -1:
  print("Not Connected")

if __name__ == "__main__":
    print("Starting Test")

    count = 10
    c = 0
    while c != count:
        for i in range(0,4):
            if i == 0:
                lidarController[3].off()
            else:
                lidarController[i-1].off()
            lidarController[i].on()
            print( str(i) + " " + str(lidarController[i].value) )
            
            j = 0
            while j != 50: 
                try:
                    dist = lidar.getDistance()
                    avg = avgValues[i]
                    avgValues[i] = avg+dist
                    #print("Average Value for "+ str(i) + " " + str(avgValues[i]))
                    sleep(0.01)
                    print("Distance: "+ str(i) + " " + str(dist))
                except:
                    print("\tRemote I/O Error Occur, Check connection")
                    sleep(0.01)
                j += 1
        c += 1

    for i in range(4):
        print("Average Value for Lidar "+str(i)+": "+str(avgValues[i]/(count*50)))
    #loopedVal += 50