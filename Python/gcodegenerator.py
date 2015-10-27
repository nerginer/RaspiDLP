
import os
import time
import sys
import json

settingsJsonFile="../json/printerSettings.json"


in_file = open(settingsJsonFile,"r")   
settingsJson = json.load(in_file)
in_file.close()



print 'gcodeGenerator.py started'
print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)


settingsJsonFile=sys.argv[1]
meterialJsonFile=sys.argv[2]
printImgFile=sys.argv[3]

gcodefile = open(settingsJson["print_data_dir"]+"/printGcode.gcode", 'w')

gcodefile.write("settingsJsonFile:%s\n" % settingsJsonFile)
gcodefile.write("meterialJsonFile:%s\n" % meterialJsonFile)
gcodefile.write("printImgFile:%s\n" % printImgFile)

gcodefile.close()

time.sleep(3)

print 'gcode file generated'
