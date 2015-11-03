import time
import serial
from pubsub import pub
import serial.tools.list_ports


GrblPort=""
serMotion = None
print 'GbrlPort.py started'

pub.sendMessage('Log', arg1='debug', arg2='*** GbrlPort.py started ***')
pub.sendMessage('Log', arg1='debug', arg2='*** ------------------- ***')

def init():
    pub.sendMessage('Log', arg1='debug', arg2='Scaning Serial Ports...')
    global GrblPort
    global serMotion
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
            print p 
            pub.sendMessage('Log', arg1='debug', arg2=p)
            #if (p[2].find("SNR=7413633393235101F182")<>-1) or (p[2].find("SNR=9523834323435171E0A1")<>-1) :
                try:
                        ser = serial.Serial(p[0], 115200)
                        if not ser.isOpen():
                                ser.open()

                      
                        mychar = ser.readline()
                        time.sleep(0.5)
                        mychar = mychar+ser.readline()#get two line
                        
                        print mychar
                        pub.sendMessage('Log', arg1='debug', arg2=mychar)
                        
                        if  (mychar.find("Grbl")<>-1):
                            print "Grbl bulundu"
                            GrblPort = p[0]
                            pub.sendMessage('Log', arg1='debug', arg2="GrblPort: " + GrblPort)
                        ser.close()
                except:
                        print "Cannot open port: " + p[0]
                        pub.sendMessage('Log', arg1='warning', arg2="Cannot open port: " + p[0])
                      



    if (GrblPort <> ""):
        print "GrblPort: " + GrblPort
        serMotion = serial.Serial(GrblPort, 115200)
    else:
        print "No Motion Controller Hardware Found"
      #  raise ValueError("No Motion Controller Hardware Found")
        
    

    
    
    
    
