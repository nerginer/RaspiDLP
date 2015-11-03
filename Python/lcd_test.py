import lcd
from pubsub import pub


pub.sendMessage('LCD', arg1='First Line', arg2='Second Line')


