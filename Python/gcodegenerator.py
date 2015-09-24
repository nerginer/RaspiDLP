import string
import json
import zipfile
import shutil
import os
from sys import exit
import logger
from pubsub import pub


pub.sendMessage('Log', arg1='debug', arg2='*** gcodegenerator.py started ***')
pub.sendMessage('Log', arg1='debug', arg2='*** ------------------------- ***')

settingsJsonFile="../json/printerSettings.json"
meterialJsonFile="../json/meterial_b9.json"


in_file = open(settingsJsonFile,"r")   
settingsJson = json.load(in_file)
in_file.close()

in_file = open(meterialJsonFile,"r")   
meterialJson = json.load(in_file)
in_file.close()


pub.sendMessage('Log', arg1='debug', arg2='json files loaded')


#unzip input file
def myUnZip(inputfile,outpath):
    
    #del outpath if exists
    if os.path.exists(outpath):
        shutil.rmtree(outpath)
    #end

    # open zip file

    with zipfile.ZipFile(inputfile,"r") as z:
        z.extractall(outpath)

    # end   


myInputFile = ""

for file in os.listdir(settingsJson["download_dir"]):
    if file.endswith(".zip"):
        myInputFile=file

if (myInputFile == ""):
 print ('no imput .zip file')
 #log here
 exit('no imput .zip file')

#"/Users/gnexlab_imac/code/dlp_raspi/RaspiDLP/Python/Temp"
pub.sendMessage('Log', arg1='debug', arg2='input file is: '+settingsJson["download_dir"]+'/'+myInputFile)

#unzip image files
myUnZip(settingsJson["download_dir"]+'/'+myInputFile,settingsJson["print_data_dir"])

pub.sendMessage('Log', arg1='debug', arg2='input file unziped to: '+settingsJson["print_data_dir"])
#number of image files
number_of_slices = len([name for name in os.listdir(settingsJson["print_data_dir"]) if os.path.isfile(os.path.join(settingsJson["print_data_dir"], name))])

pub.sendMessage('Log', arg1='debug', arg2='number number_of_slices: '+ str(number_of_slices))

gcodelist = []

gcodelist.append(settingsJson["header_code"])

for c in range(0,int(number_of_slices)):

    gcodelist.append(";<Slice> " + str(c))

    if (c < int(meterialJson["number_of_bottom_layers"])):
        gcodelist.append(";<Delay> " + meterialJson["cure_time_bottom_layers"]);
        gcodelist.append(";<Slice> Blank \r\n")
        gcodelist.append(settingsJson["first_layer_lift_code"])
        gcodelist.append(";<Delay> " + settingsJson["first_layer_motion_time"]+";Wait for Motion Complated");
    else:
        gcodelist.append(";<Delay> " + meterialJson["cure_time_nominal_layers"]);
        gcodelist.append(";<Slice> Blank \r\n")
        gcodelist.append(settingsJson["lift_code"])
        gcodelist.append(";<Delay> " + settingsJson["layer_motion_time"]+";Wait for Motion Complated");
    

gcodelist.append(settingsJson["footer_code"])

gcodefile = open(settingsJson["print_data_dir"]+"/gcodefile.txt", 'w')

for item in gcodelist:
  gcodefile.write("%s\n" % item)
  
gcodefile.close()

pub.sendMessage('Log', arg1='debug', arg2='gcode generated and saved to '+settingsJson["print_data_dir"]+"/gcodefile.txt")

pub.sendMessage('Log', arg1='debug', arg2='*** gcodegenerator.py ended ***')
#print "GCode List : ", gcodelist



