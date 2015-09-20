import string
import json


in_file = open("../json/data.json","r")   
j = json.load(in_file)
in_file.close()

gcodelist = []

gcodelist.append(j["HeaderCode"])

for c in range(0,int(j["number_of_slices"])):

    gcodelist.append(";<Slice> " + str(c))

    if (c < int(j["number_of_bottom_layers"])):
        gcodelist.append(";<Delay> " + j["cure_time_bottom_layers"]);
        gcodelist.append(";<Slice> Blank \r\n")
        gcodelist.append(j["FirstLayerLiftGCode"])
        gcodelist.append(";<Delay> " + j["FirstLayerMotionTime"]+";Wait for Motion Complated");
    else:
        gcodelist.append(";<Delay> " + j["cure_time_nominal_layers"]);
        gcodelist.append(";<Slice> Blank \r\n")
        gcodelist.append(j["LiftGCode"])
        gcodelist.append(";<Delay> " + j["LayerMotionTime"]+";Wait for Motion Complated");
    

gcodelist.append(j["FooterCode"])

gcodefile = open("gcodefile.txt", 'w')

for item in gcodelist:
  gcodefile.write("%s\n" % item)
  
gcodefile.close()

print "GCode List : ", gcodelist
