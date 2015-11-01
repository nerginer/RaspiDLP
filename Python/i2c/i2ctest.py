import smbus
import time

bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

def writeCommand(value):
	bus.write_byte(address, value)
	# bus.write_byte_data(address, 0, value)
	return -1

def readCommand():
	number = bus.read_byte(address)
	# number = bus.read_byte_data(address, 1)
	return number

while True:
	var = input("Enter command: ")
	if not var:
		continue

	writeCommand(var)
	print "RPI: Hi Arduino, I sent you ", var
	# sleep one second
	time.sleep(1)

	number = readCommand()
	print "Arduino: Hey RPI, I received a digit ", number
	print
