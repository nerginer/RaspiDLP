from time import sleep
import serial
import pygame
from pygame.locals import *
import logger
import json
from pubsub import pub

print 'printManager.py started'

pub.sendMessage('Log', arg1='debug', arg2='*** printManager.py started ***')
pub.sendMessage('Log', arg1='debug', arg2='*** ------------------------- ***')

settingsJsonFile="../json/printerSettings.json"

in_file = open(settingsJsonFile,"r")   
settingsJson = json.load(in_file)
in_file.close()

pub.sendMessage('Log', arg1='debug', arg2='json files loaded')


#def gameinit():
pub.sendMessage('Log', arg1='debug', arg2='gameinit started')
# Call this function so the Pygame library can initialize itself
pygame.init()
# Create an 1024x768 sized screen
#screen = pygame.display.set_mode((1024, 768),FULLSCREEN) gercegi bu
H = int(settingsJson["projector_res_h"])
W = int(settingsJson["projector_res_w"])
screen = pygame.display.set_mode((H,W))

background_position = [0, 0]
#Siyah Blank Screen
myBlackScreen = pygame.Surface(screen.get_size())
myBlackScreen = myBlackScreen.convert()
myBlackScreen.fill((0,0,0))
#Mouse gozukmesin
pygame.mouse.set_visible(0)





def doPrint(baseFileName):
   # gameinit()
    pub.sendMessage('Log', arg1='debug', arg2='gameinit complated')
#arguman olarak alinacak
    
#    initSerial()

    fp=open(baseFileName,'r')

    lines=fp.readlines()
    for line in lines:
        if ';<Slice>' in line :
            processSliceParam(line.split(' ')[1])
        if ';<Delay>' in line :
            processDelay(line.split(' ')[1])
        if not';' in line :
            processGCode(line)
            
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit ()

    pygame.quit ()

# function gcode sender
def processGCode(line):
    if not line.isspace(): 
        print 'GCode: '+line
       # ser.write(line+'\n')


# function to Delay
def processDelay(var):
    print 'Delaying for druation: '+var
    pygame.time.delay(int(var))

# function to processSliceParam
def processSliceParam(var):
    print ('nuriiii:'+var)
    if 'Blank' in var :
        showBlank()
    else:
    #if var.isdigit():
        showSlide(var.rstrip('\r\n'))
        

def showSlide(var):
    print 'Showing slide number: '+var
    # Load and set up graphics.
    background_image = pygame.image.load(settingsJson["print_data_dir"]+'/slice_'+var+'.png').convert()
    screen.blit(background_image, background_position)
    pygame.display.flip()

def showBlank():
    print 'Showing Blank'
    screen.blit(myBlackScreen, background_position)
    pygame.display.flip()


        
doPrint(settingsJson["print_data_dir"]+"/printGcode.gcode")


