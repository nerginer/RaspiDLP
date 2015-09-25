from time import sleep
import serial
import pygame
from pygame.locals import *
import logger
import json
import os
import sys
from pubsub import pub
from bayeux.bayeux_client import BayeuxClient


#print 'printManager.py started'

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

state = 'play'

def cb(data):
    global state
    print data['data']['text']
    state = data['data']['text']

def doPrint(baseFileName):
   # gameinit()
    pub.sendMessage('Log', arg1='debug', arg2='gameinit complated')
#arguman olarak alinacak
    #faye client *********************
    client = BayeuxClient('http://localhost:8000/faye')
    client.register('/messages', cb)

    client.start()
    #faye client end *****************
#    initSerial()

    fp=open(baseFileName,'r')

    lines=fp.readlines()
    for line in lines:
        #pause
        while (state == 'pause'):
            pass
            #belki buraya siyeh ekran yaip dururum
            
        #cancel or stop
        if  (state == 'stop'):   
            print 'print canceled'
            return

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
       pass
       # print 'GCode: '+line
       # ser.write(line+'\n')


# function to Delay
def processDelay(var):
    #print 'Delaying for druation: '+var
    pygame.time.delay(int(var))

# function to processSliceParam
def processSliceParam(var):
    if 'Blank' in var :
        showBlank()
    else:
    #if var.isdigit():
        showSlide(var.rstrip('\r\n'))
        
#number of image files
number_of_slices = len([name for name in os.listdir(settingsJson["print_data_dir"]) if os.path.isfile(os.path.join(settingsJson["print_data_dir"], name))])
number_of_slices = number_of_slices -1
#burayi duzelt

def showSlide(var):
    print 'Layer: '+var+' of '+str(number_of_slices)
    # Load and set up graphics.
    background_image = pygame.image.load(settingsJson["print_data_dir"]+'/slice_'+var+'.png').convert()
    screen.blit(background_image, background_position)
    pygame.display.flip()

def showBlank():
    #print 'Showing Blank'
    screen.blit(myBlackScreen, background_position)
    pygame.display.flip()


        
doPrint(settingsJson["print_data_dir"]+"/printGcode.gcode")


