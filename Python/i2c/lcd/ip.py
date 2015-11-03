import lcddriver
from time import *
from subprocess import *

lcd = lcddriver.lcd()

lcd.lcd_display_string("GNXDLP", 1)
#lcd.lcd_display_string("Banu", 2)

myip = "ip addr show wlan0 | grep inet | awk '{print $2}' | cut -d/ -f1"

def run_cmd(cmd): 
	p = Popen(cmd, shell = True, stdout = PIPE) 
	output = p.communicate()[0] 
	print output.splitlines()[0]
	return output.splitlines()[0]

sleep(5)
lcd.lcd_display_string("Starting...", 2) 
sleep(5)

ipaddr = run_cmd(myip)
lcd.lcd_display_string("IP:%s" % ipaddr, 2) 


