import lib
lcd = lib.lcd(0x27,1)
lcd.lcd_write(0x01);
lcd.lcd_puts("Line1",1) 
lcd.lcd_puts("Line2",2) 
lcd.lcd_backlight(1)
