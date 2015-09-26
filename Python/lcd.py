from pubsub import pub



def listener_Func(arg1, arg2):
   pass

# ------------ register listener ------------------

pub.subscribe(listener_Func, 'LCD')

# usage
# pub.sendMessage('LCD', arg1='First Line', arg2='Second Line')
