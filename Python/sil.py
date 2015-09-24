import logger
from pubsub import pub


pub.sendMessage('Log', arg1='debug', arg2='json files loaded')

