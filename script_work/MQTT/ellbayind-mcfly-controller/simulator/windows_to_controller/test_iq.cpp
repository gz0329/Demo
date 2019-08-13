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

#define HOST "192.168.0.10"
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

	const char *msg;
	const char *topic;

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, CLEAN_SESSION, NULL);

	if ( mosq )
	{
		printf( "Setting callbacks\n");

		mosquitto_publish_callback_set( mosq, publish_callback );

		rc = mosquitto_connect( mosq, HOST, PORT, KEEPALIVE );

		msg = "Start";
		topic = "Triggers";

		while (1)
		{
				Time = clock();

				while ( ( clock() - Time ) < 1000000 );

				mosquitto_publish( mosq, NULL, topic, strlen( msg ), msg, 0, 0 );

				Time = clock() - Time;

				printf ("It took me %d clicks (%f seconds).\n", Time, ( ( float ) Time ) / CLOCKS_PER_SEC );
			}
		}
	return 0;
}