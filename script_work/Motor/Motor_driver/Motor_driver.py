'''
Copyright (c) 2018, Elliott Bay Industries. All rights reserved.

Purpose:
Use keyboard on pc to control motor

Speed up             Arrow up
Speed down           Arrow down
Clockwise            Arrow right
Counter clockwise    Arrow left
Turn on/off          Space
Reset to default     R enter
Exit program         Esc

History:
2019-04-08  1.00  George Zhang   Initial implementation. 
'''
from msvcrt import getch
import serial

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3'
ser.open()

Flag = True

while True:
	Keycode = ord( getch() ) 

	if Keycode == 27:#Esc
		ser.write( b'off' )
		ser.close()
		break
	elif Keycode == 32:
		if Flag:
			ser.write( b'on' )
			print( "turn on" ) 
		else:
			ser.write( b'off' )
			print( "turn off" )
		Flag = not Flag	
	elif Keycode == 114:
		ser.write( b'reset' )
		print( "r" )
	elif Keycode == 224:
		Keycode = ord( getch() )

		if Keycode == 80:
			print( "down" )
			ser.write( b'down' )
		elif Keycode == 72:
			print( "up" )
			ser.write( b'up' )
		elif Keycode == 75:
			print( "left" )
			ser.write( b'left' )
		elif Keycode == 77:
			print( "right" )
			ser.write( b'right' )