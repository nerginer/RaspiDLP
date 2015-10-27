from time import sleep

import logger
import json
import os
import sys

from bayeux.bayeux_client import BayeuxClient
from urllib2 import Request, urlopen



state = 'play'

def cb(data):
    global state
    print data['data']['text']
    state = data['data']['text']

def doPrint():

  

    #faye client *********************
    client = BayeuxClient('http://localhost:8000/faye')
    client.register('/messages', cb)

    client.start()
    #faye client end *****************

    
  

    
    for line in range(0,10):
        #pause
        while (state == 'pause'):
            print 'pause'
            pass
            #belki buraya siyeh ekran yaip dururum

        #cancel or stop
        if  (state == 'stop'):   
            print 'print canceled'
            return

        showSlide(line)
        processDelay(2)
            
      



# function to Delay
def processDelay(var):
    print 'Delaying for druation: '+str(var)
    sleep(var)




def sendFayeMessage(myMessage):
    values = '{"channel":"/status","data":{"text":"'+myMessage+'"}}'
    #values = json.dumps(values)
    headers = {"Content-Type": "application/json"}
    request = Request("http://localhost:8000/faye", data=values, headers=headers)
    response_body = urlopen(request).read()
    print response_body

   

def showSlide(var):
    statusData = 'Layer: '+str(var)+' of 10'
    sendFayeMessage(statusData)
    print statusData
    

def showBlank():
    print 'Showing Blank'
    


        
doPrint()


