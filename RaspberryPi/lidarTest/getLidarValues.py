'''
#-----------------!!NOTICE!!-----------------#
YOU NEED CAPACITORS TO POWER THE LIDAR

This is really important since you can do 
some real damage to the devine your testing with
because it does not consistantly draw power.

Capacitance: 500 <= uC <= 1000
Capacitanc on Website: 680uC

Wires Used: Red, Black, Green, Blue

----SETUP----

SINGLE CAPACITOR:
The 5v

Assume . is an unused pin hole.
+  is positive current input
-  is negative current input
X  is capacitor
L  is lidar
IN is input power

IN+ . . . . . X+ . . . . . L+
IN- . . . . . X- . . . . . L-

CAPACITORS IN PARALLEL:

Assume . is an unused pin hole.
+  is positive current input
-  is negative current input
X  is capacitor
L  is lidar
IN is input power
W# is wire connections (W1+ -> W1+)

IN+ . . . . . . . W2+ . . . . L+
IN- . . . . . . . W2- . . . . L-
----------------------------------
.  W1+ .  .  .  W2+ .  .  .  .  .
.  .   .  .  .  W1+ .  .  .  .  . 
.  X+  .  .  .  X+  .  .  .  .  . 
.  X-  .  .  .  X-  .  .  .  .  . 
.  .   .  .  .  W2- .  .  .  .  . 
.  W1- .  .  .  W1- .  .  .  .  .

Connect positive leads for capacitors to eachother
Connect negative leads for capacitors to eachother
Power goes into the first capacitor's positive lead
    and fed back from the last capacitor's positive lead
Ground goes into the first capacitor's negative lead
    and fed back from the last capacitor's negative lead
    
RASPBERRY PI WIRING

The odd pin numbers are the top row starting from
the right side closes to the power and increases to
the left with the farthest pin being closest to the
USB ports. Here is the Pinout:

Red: 6   Black: 4    Blue: 3    Green: 5

use sudo i2cdetect -y 1 to verify if it is connected
correctly. 

#--------------------------------------------#
'''

lidar = Lidar_Lite()
connected = lidar.connect(1)

if connected < -1:
  print("Not Connected")

if __name__ == "__main__":
    print("Starting Test")
    while True:
        try:
            
            print "Distance: "+lidar.getDistance()
            sleep(0.1)
        except:
            print "\tRemote I/O Error Occur, Check connection"
            sleep(0.1)
