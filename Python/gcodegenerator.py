import string
import json
import zipfile
import shutil
import os


myInputFile="Archive.zip"
imageOutPath="/Users/gnexlab_imac/code/dlp_raspi/RaspiDLP/Python/Temp"
settingsJsonFile="../json/printerSettings.json"
meterialJsonFile="../json/meterial_b9.json"


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

#unzip image files
myUnZip(myInputFile,imageOutPath)


#number of image files
number_of_slices = len([name for name in os.listdir(imageOutPath) if os.path.isfile(os.path.join(imageOutPath, name))])



in_file = open(settingsJsonFile,"r")   
settingsJson = json.load(in_file)
in_file.close()

in_file = open(meterialJsonFile,"r")   
meterialJson = json.load(in_file)
in_file.close()




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

gcodefile = open("gcodefile.txt", 'w')

for item in gcodelist:
  gcodefile.write("%s\n" % item)
  
gcodefile.close()

print "GCode List : ", gcodelist



