import serial
import struct
import time
from datetime import datetime

def time_dif( t1, t2 ):
	t_dif = datetime.strptime( t2, '%H:%M:%S') - datetime.strptime( t1, '%H:%M:%S')
	return t_dif.seconds

file = open("Test_mag_time_temp_humidity.txt", "w")

# Serial port to get data
ser = serial.Serial()
ser.baudrate = 115200
ser.port = "COM" + input( "Serial port number for data? " )
ser.open()

# Serial port to get temperature
ser2 = serial.Serial()
ser2.baudrate = 115200
ser2.port = "COM" + input( "Serial port number for temperature? " )
ser2.open()

if ser.is_open:
	print( ser.port + " opened." )

if ser2.is_open:
	print( ser2.port + " opened." )

Time_ref = time.strftime( "%H:%M:%S", time.localtime() )

while True :
	#if ser.in_waiting == 1:
	ser.reset_input_buffer()
	ser2.reset_input_buffer()
	t = time.localtime()
	now = time.strftime( "%H:%M:%S", t )

	if time_dif( Time_ref, now ) > 60:
		Str = bytearray( ser.read( 9 ) )

		Str2 = bytearray( ser2.read( 13 ) )

		Board_temp = struct.unpack( 'f', Str2[ 0: 4 ] )
		Ambient_temp = struct.unpack( 'f', Str2[ 4: 8 ] )
		Humidity = struct.unpack( 'f', Str2[ 8: 12 ] )

		mag_n = struct.unpack( 'f', Str[ 0 : 4 ] )
		mag_o = struct.unpack( 'f', Str[ 4 : 8 ] )

		Time_ref = now
		print( now + " " + str( mag_n ) + " " +  str( mag_o ) + " " + str( Board_temp )  + " " + str( Ambient_temp )  + " " + str( Humidity ) )
		file.write( now + " " + str( mag_n ) + " " +  str( mag_o ) + " " + str( Board_temp )  + " " + str( Ambient_temp )  + " " + str( Humidity ) + "\n" )
		file.flush()

ser.close();