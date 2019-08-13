////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Elliott Bay Industries. All rights reserved.
//
// Purpose:
// Provide an accurate 5ms trigger. 
//
// History:
// 2019-02-15  1.00  George Zhang   Initial implementation. 
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <mosquitto.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include <chrono>
//#include "gpio.h"
//#include <wiringPi.h>

#define HOST "192.168.0.78"
#define PORT 1883
#define COUNT 1
#define KEEPALIVE 60
#define CLEAN_SESSION true
#define PHASE_PERIOD_us 5000

//using namespace std::chrono;
using namespace std;
struct mosquitto *mosq = NULL;

// Callback function for publish
void publish_callback( struct mosquitto *mosq, void *obj, int mid )
{
	//printf( "Published , mid=%d\n", mid );
}

int main() 
{
	clock_t T1;
	struct mosquitto_message *message, *mp;
	int n, rc;

	//GPIOClass* gpio17 = new GPIOClass( "17" );
	//gpio17->export_gpio();
	//gpio17->setdir_gpio( "out" );

	ifstream ifs("mcfly_conf_v3.json");
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj);
	cout << "Number of LRU: " << obj["Num_LRUs"].asUInt() << endl;
	cout << "Number of Cells per LRU: " << obj["Cells_per_LRU"].asUInt() << endl;
	cout << "LRUS Comment: " << obj["LRUs_comment"].asString() << endl;
	cout << endl;

	const char *msg;
	const char *topic;

	const Json::Value& Phase_idx = obj["Phases"];
	const Json::Value& LO = obj["LO"]; 

	cout << "Phases: " << Phase_idx[0]["Phase_idx"].asString() << endl;
	cout << "Topic to publish: " << LO[0]["Channel_sub"].asString() << endl;
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, CLEAN_SESSION, NULL);

	if ( mosq )
	{
		printf( "Setting callbacks\n");

		mosquitto_publish_callback_set( mosq, publish_callback );

		rc = mosquitto_connect( mosq, HOST, PORT, KEEPALIVE );

		topic = LO[0]["Channel_sub"].asCString();

		int Phase = 0;

		while ( 1 )
		{
			/*
			for ( int i = 0; i < Phase_idx.size(); i++ )
			{
				std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

				while ( std::chrono::duration_cast<chrono::microseconds>( std::chrono::high_resolution_clock::now() - t1 ).count() < 5000000 );
				
				//T1 = clock();

				//while ( ( clock() - T1 ) < 5000000 );

				msg = Phase_idx[i]["Phase_idx"].asCString();
				cout << "Message to publish: " << Phase_idx[i]["Phase_idx"].asString() << endl;
				mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 );

				std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

				std::cout << "It took me " << std::chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " microseconds.";
				std::cout << std::endl;
				//T1 = clock() - T1;
				//printf ("It took me %d clicks (%f seconds).\n", T1, ( ( float ) T1 ) / CLOCKS_PER_SEC );
			}
			*/
			//cout << "Message to publish: " << Phase_idx[Phase]["Phase_idx"].asString() << endl;

			switch( Phase )
			{
				case 0: msg = Phase_idx[Phase]["Phase_idx"].asCString(); mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 ); Phase = 1;
					break;
				case 1: msg = Phase_idx[Phase]["Phase_idx"].asCString(); mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 ); Phase = 2;
					break;
				case 2: msg = Phase_idx[Phase]["Phase_idx"].asCString(); mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 ); Phase = 3;
					break;
				case 3: msg = Phase_idx[Phase]["Phase_idx"].asCString(); mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 ); Phase = 0;
					break;
				default:
					break;
			}

			//gpio17->setval_gpio( "1" );
			//gpio17->setval_gpio( "0" );

			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

			while ( std::chrono::duration_cast<chrono::microseconds>( std::chrono::high_resolution_clock::now() - t1 ).count() < PHASE_PERIOD_us );

			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

			//std::cout << "It took me " << std::chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " microseconds.";
			//std::cout << std::endl;
		}
	}
	return 0;
}
