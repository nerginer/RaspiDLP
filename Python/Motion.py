import time
import serial
from pubsub import pub


serMotion = serial.Serial('/dev/ttyAMA0',9600)



if not serMotion.isOpen():
	serMotion.open()

serMotion.write("\r\n\r\n")
time.sleep(2)   # Wait for grbl to initialize 
serMotion.flushInput()  # Flush startup text in serial input

#--grbl is ready to use

def processGCode(line):
    if not line.isspace(): 
        print 'GCode: '+line
        serMotion.write(line+'\n')
        print 'Motion Controller: '+serMotion.readline()


 
def listener1(arg1):
    processGCode(arg1)


# ------------ register listener ------------------

pub.subscribe(listener1, 'Motion')

# usage
# pub.sendMessage(''Motion'', arg1='G1X10')