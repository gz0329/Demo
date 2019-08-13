from random import *
import paho.mqtt.publish as publish
import paho.mqtt.subscribe as subscribe
import paho.mqtt.client as mqtt
import time

sample_channel_0 = "/Sensor/Density/LRU_pair_0/Samples"
sample_channel_1 = "/Sensor/Density/LRU_pair_1/Samples"
sample_channel_2 = "/Sensor/Density/LRU_pair_2/Samples"
sample_channel_3 = "/Sensor/Density/LRU_pair_3/Samples"
sample_channel_4 = "/Sensor/Density/LRU_pair_4/Samples"
sample_channel_5 = "/Sensor/Density/LRU_pair_5/Samples"
sample_channel_6 = "/Sensor/Density/LRU_pair_6/Samples"
sample_channel_7 = "/Sensor/Density/LRU_pair_7/Samples"
sample_channel_8 = "/Sensor/Density/LRU_pair_8/Samples"
sample_channel_9 = "/Sensor/Density/LRU_pair_9/Samples"
sample_channel_10 = "/Sensor/Density/LRU_pair_10/Samples"

trigger_channel_0 = "/Sensor/Density/LRU_pair_0/Trigger"
trigger_channel_1 = "/Sensor/Density/LRU_pair_1/Trigger"
trigger_channel_2 = "/Sensor/Density/LRU_pair_2/Trigger"
trigger_channel_3 = "/Sensor/Density/LRU_pair_3/Trigger"
trigger_channel_4 = "/Sensor/Density/LRU_pair_4/Trigger"
trigger_channel_5 = "/Sensor/Density/LRU_pair_5/Trigger"
trigger_channel_6 = "/Sensor/Density/LRU_pair_6/Trigger"
trigger_channel_7 = "/Sensor/Density/LRU_pair_7/Trigger"
trigger_channel_8 = "/Sensor/Density/LRU_pair_8/Trigger"
trigger_channel_9 = "/Sensor/Density/LRU_pair_9/Trigger"
trigger_channel_10 = "/Sensor/Density/LRU_pair_10/Trigger"
trigger_channel_11 = "/Sensor/Density/LRU_pair_11/Trigger"


def get_data( sample_channel ):
	data = bytearray()
	for num_sample in range( 0, 64 ):
		num = randint( 1, 100 )
		data.append( num )

	pub( sample_channel, data )
	print( sample_channel + " done!\n" )

def on_connect( client, userdata, flags, rc ):
	print("Connected with result code " + str( rc ) )
	if rc != 0:
		mqttc.reconnect()

def on_disconnect( client, userdatam, rc ):
	if rc != 0:
		print( " Unexpected disconnection." )
		mqttc.loop_end()

def on_subscribe( client, userdata, mid, granted_qos ):
	print( " In on_subscribe callback " )

def on_unsubscribe( client, userdata, mid ):
	print( " In on_unsubscribe callback " )

def on_publish( client, userdata, mid ):
	print( " In on_publish callback " )

def on_message( client, userdata, message ):

	#print("Received message '" + str( message.payload ) + "' on topic '"
    #    + message.topic + "' with QoS " + str( message.qos ) )
	
	trigger_callback = str( message.payload )
	channel_callback = str( message.topic )

	if ( "start" in trigger_callback and channel_callback == trigger_channel_0 ):
		get_data( sample_channel_0 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_1 ):
		get_data( sample_channel_1 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_2 ):
		get_data( sample_channel_2 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_3 ):
		get_data( sample_channel_3 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_4 ):
		get_data( sample_channel_4 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_5 ):
		get_data( sample_channel_5 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_6 ):
		get_data( sample_channel_6 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_7 ):
		get_data( sample_channel_7 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_8 ):
		get_data( sample_channel_8 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_9 ):
		get_data( sample_channel_9 )
	elif ( "start" in trigger_callback and channel_callback == trigger_channel_10 ):
		get_data( sample_channel_10 )
	else:
		pass

def on_log( client, userdata, level, buf ):
	print( " In on_log callback " )

def pub( topic, msg ):
	mqttc.publish( topic, msg )

def sub( topic ):
	return mqttc.subscribe( topic, 0 )

def channel_init():	
	for channel in range( 0, 11 ):
		sample_channel = "/Sensor/Density/LRU_pair_" + str( channel ) + "/Samples"
		trigger_channel = "/Sensor/Density/LRU_pair_" + str( channel ) + "/Trigger"

		print( "Check "+ trigger_channel )
		print( "Check "+ sample_channel )
		sub ( trigger_channel )

mqttc = mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.on_disconnect = on_disconnect
#mqttc.on_subscribe = on_subscribe
#mqttc.on_publish = on_publish
mqttc.connect( "192.168.0.4" )
mqttc.loop_start()

def main ():

	channel_init()

	while ( 1 ):
		pass

main()