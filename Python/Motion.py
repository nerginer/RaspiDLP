import time
import GbrlPort
from pubsub import pub






GbrlPort.init()

#serMotion = serial.Serial(serialportlist2.GrblPort, 115200)
if not GbrlPort.serMotion.isOpen():
    GbrlPort.serMotion.open()

GbrlPort.serMotion.write("\r\n\r\n")
time.sleep(2)   # Wait for grbl to initialize 
GbrlPort.serMotion.flushInput()  # Flush startup text in serial input

#--grbl is ready to use

def processGCode(line):
    if not line.isspace(): 
        print 'GCode: '+line
        GbrlPort.serMotion.write(line+'\n')
        print 'Motion Controller: '+serMotion.readline()


 
def listener1(arg1):
    processGCode(arg1)


# ------------ register listener ------------------

pub.subscribe(listener1, 'Motion')

# usage
# pub.sendMessage(''Motion'', arg1='G1X10')
