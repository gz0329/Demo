from random import *
import paho.mqtt.publish as publish
import paho.mqtt.subscribe as subscribe
import paho.mqtt.client as mqtt
import time
from fractions import Fraction

file = open("Test_mag_phase_1.txt", "w")

def channel_init():
	Sample_chan = "Samples"
	sub ( Sample_chan )

def tobits(s):
    result = []
    for c in s:
        bits = bin(ord(c))[2:]
        bits = '00000000'[len(bits):] + bits
        result.extend([int(b) for b in bits])
    return result

def tohex(s):
    result = ":".join("{:02x}".format(ord(c)) for c in s)
    return result

def on_connect( client, userdata, flags, rc ):
	print("Connected with result code " + str( rc ) )
	mqttc.loop_start()
	channel_init()
	if rc != 0:
		mqttc.reconnect()
		mqttc.loop_start()
		channel_init()

def on_disconnect( client, userdatam, rc ):
	if rc != 0:
		print( " Unexpected disconnection." )
		mqttc.connect( "192.168.0.10", 1883, 60 )
		mqttc.loop_start()
		channel_init()
	mqttc.loop_start()
	channel_init()

def on_subscribe( client, userdata, mid, granted_qos ):
	print( " In on_subscribe callback " )

def on_unsubscribe( client, userdata, mid ):
	print( " In on_unsubscribe callback " )

def on_publish( client, userdata, mid ):
	print( " In on_publish callback " )

def on_message( client, userdata, message ):
	msg_callback = str( message.payload )
	topic_callback = str( message.topic )
	filtered_string = msg_callback[2:len(msg_callback) - 1]
	length = 8
	part_1, part_2 = filtered_string[:length], filtered_string[length: length * 2 ]
	part_3, part_4 = filtered_string[length * 2:length * 3], filtered_string[length * 3:]

	part_1_r = part_1[::-1]
	part_2_r = part_2[::-1]
	part_3_r = part_3[::-1]
	part_4_r = part_4[::-1]

	#print( "o: ",part_1, part_2, part_3, part_4 )
	#print( "r: ",part_1_r, part_2_r, part_3_r, part_4_r )

	phase_chan_4 = part_4[::-1]
	phase_chan_6 = part_3[::-1]

	phase_4 = int(phase_chan_4, 16)
	phase_6 = int(phase_chan_6, 16)

	#print( phase_4, phase_6 )

	mag_chan_4 = part_2[::-1]
	mag_chan_6 = part_1[::-1]

	mag_chan_4_float = float(Fraction(int(mag_chan_4, 16), 4294967295 ) )
	mag_chan_6_float = float(Fraction(int(mag_chan_6, 16), 4294967295 ) )

	#print( mag_chan_4_float * 100, mag_chan_6_float * 100 )


	data = "Phase_4: " + str(phase_4) + " Phase_6: "+ str(phase_6) + " Mag_4: " + str(mag_chan_4_float) + " Mag_6: "+ str(mag_chan_6_float)+ "\n"
	print ( data )
	file.write( data )
	file.flush()
	#file.close()

def on_log( client, userdata, level, buf ):
	print( " In on_log callback " )

def pub( topic, msg ):
	mqttc.publish( topic, msg )

def sub( topic ):
	return mqttc.subscribe( topic, 0 )

mqttc = mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.on_disconnect = on_disconnect
#mqttc.on_subscribe = on_subscribe
#mqttc.on_publish = on_publish
mqttc.connect( "192.168.0.10", 1883, 60 )
mqttc.loop_start()
#mqttc.loop_forever()

def main ():

	channel_init()

	while ( 1 ):
		mqttc.loop_start()

main()