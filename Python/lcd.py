from pubsub import pub
import lcddriver

lcd = lcddriver.lcd()



def listener_Func(arg1, arg2):
   lcd.lcd_display_string(arg1, 1)
   lcd.lcd_display_string(arg2, 2)

# ------------ register listener ------------------

pub.subscribe(listener_Func, 'LCD')

# usage
# pub.sendMessage('LCD', arg1='First Line', arg2='Second Line')
