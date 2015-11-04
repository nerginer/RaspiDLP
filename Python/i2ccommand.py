#have to run 'sudo apt-get install python-smbus'
#in Terminal to install smbus
import smbus
import time
import os
from pubsub import pub

bus = smbus.SMBus(1)

# I2C address of Arduino Slave
i2c_address = 0x04
i2c_cmd = 0x01

def ConvertStringToBytes(src):
    converted = []
    for b in src:
        converted.append(ord(b))
    return converted
    
def sendI2CCommand(I2CCommand)    
    bytesToSend = ConvertStringToBytes(I2CCommand)
    bus.write_i2c_block_data(i2c_address, i2c_cmd, bytesToSend)
    
def listener1(arg1):
    sendI2CCommand(arg1)


# ------------ register listener ------------------

pub.subscribe(listener1, 'I2C')

# usage
# pub.sendMessage('I2C', arg1='PON')
