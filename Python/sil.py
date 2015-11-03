

def getImageNumber(imageName):
     if imageName is None or imageName == '':
        return 0

     out_number = ''
     for ele in imageName:
        if ele.isdigit():
         out_number = out_number+ ele
     return int(out_number) 

def getImageText(imageName):
     if imageName is None or imageName == '':
        return 0

     out_number = ''
     for ele in imageName:
        if not ele.isdigit():
         out_number = out_number+ ele
     return (out_number) 

def myReName(path):
    
    filenames = os.listdir(path)
    
    for filename in filenames:
        imageText = getImageText(filename)
        imageNum = getImageNumber(filename)
        os.rename(filename, imageText+'_'+str(imageNum))

print getImageNumber('geometricbracelet0008.png')
