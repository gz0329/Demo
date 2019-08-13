//sudo g++ -o test_task1 test_task1.cpp -ljsoncpp -lmosquitto

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

#define HOST "192.168.0.45"
#define PORT 1883
#define COUNT 1
#define KEEPALIVE 60
#define CLEAN_SESSION true

using namespace std;
struct mosquitto *mosq = NULL;

void publish_callback( struct mosquitto *mosq, void *obj, int mid )
{
	printf( "Published , mid=%d\n", mid );
}

int main() 
{
	clock_t Time;
	struct mosquitto_message *message, *mp;
	int n, rc;

	ifstream ifs("test.json");
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj);
	cout << "Number of LRU: " << obj["Num_LRUs"].asUInt() << endl;
	cout << "Number of Cells per LRU: " << obj["Cells_per_LRU"].asUInt() << endl;
	cout << "Rotation Samples per Cell: " << obj["Rotation_samples_per_cell"].asUInt() << endl;
	cout << "LRUS Comment: " << obj["LRUs_comment"].asString() << endl;
	cout << endl;
	const char *msg;
	const char *topic;
	const char *sample_topic;
	const Json::Value& LRUs = obj["LRUs"]; 

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, CLEAN_SESSION, NULL);

	if ( mosq )
	{
		printf( "Setting callbacks\n");

		mosquitto_publish_callback_set( mosq, publish_callback );

		rc = mosquitto_connect( mosq, HOST, PORT, KEEPALIVE );

		msg = "start";

		while (1)
		{
			for ( int i = 0; i < LRUs.size(); i++ )
			{
				cout << "Number of LRUs: " << LRUs.size() << endl;
				Time = clock();

				while ( ( clock() - Time ) < 2500 );

				topic = LRUs[i]["Channel_trigger"].asCString();

				mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 );

				Time = clock() - Time;

				printf ("It took me %d clicks (%f seconds).\n", Time, ( ( float ) Time ) / CLOCKS_PER_SEC );
			}
		}
	}
	return 0;
}