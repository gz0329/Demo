#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include <sstream>
#include <regex>
#include <mosquitto.h>

#define HOST "192.168.0.45"
#define PORT 1883
#define COUNT 1
#define KEEPALIVE 60
#define CLEAN_SESSION true
#define CHANNEL_SIZE 64

#define PAIR_CHANNEL_ID "/Sensor/Density/LRU_pair_"
#define SAMPLE_CHANNEL_ID "/Samples"

using namespace std;
struct mosquitto *mosq = NULL;
std::string msg_callback;
std::string topic_callback;
uint32_t len_callback;

void mqtt_init()
{
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, CLEAN_SESSION, NULL);

	if( !mosq )
	{
    fprintf(stderr, "Error: Out of memory.\n");
	}

	if( mosquitto_connect( mosq, HOST, PORT, KEEPALIVE ) )
	{
    fprintf( stderr, "Unable to connect.\n" );
	}

	int loop = mosquitto_loop_start( mosq );

	if( loop != MOSQ_ERR_SUCCESS )
	{
  	fprintf( stderr, "Unable to start loop: %i\n", loop );
	}
}

////////////////////////////////////////////////////////////////////////////////
uint8_t* get_data()
{
  uint8_t* Byte_array = new uint8_t[ CHANNEL_SIZE ];
  
  uint8_t* Byte_ptr = Byte_array;

  for( uint8_t Sample_idx = 0; Sample_idx < CHANNEL_SIZE; Sample_idx++ )
  {
    *Byte_ptr = rand() % 100 + 1;
    Byte_ptr++;
  }

  return Byte_ptr;
}

////////////////////////////////////////////////////////////////////////////////
void connect_callback( struct mosquitto* mosq, void* obj, int result )
{
  printf( "Connect callback, rc=%d\n", result );
}

////////////////////////////////////////////////////////////////////////////////
void publish_callback( struct mosquitto *mosq, void *obj, int mid )
{
	printf( "Published , mid=%d\n", mid );
}

////////////////////////////////////////////////////////////////////////////////
void message_callback( struct mosquitto* mosq, void* obj,
  const struct mosquitto_message* message )
{
	len_callback = message->payloadlen;
	//printf( "Lenth is '%d'\n", len_callback);
	msg_callback = (char*)( message->payload );
	//printf( "Message is '%.*s'\n", len_callback, msg_callback);
	topic_callback = ( message->topic );
	
	std::regex topic_regex_num(
    "/Sensor/Density/LRU_pair_(\\d+)/\\b(Samples|Trigger)" );

	std::regex topic_regex_type( "\\b(Samples|Trigger)([^ ]*)" );
	
	std::smatch result_regex_num;
	std::smatch result_regex_type;
	std::regex_search( topic_callback, result_regex_num, topic_regex_num );
	std::regex_search( topic_callback, result_regex_type, topic_regex_type );
	
	int lru_pair_idx = stoi( result_regex_num.str( 1 ) );
	string result_chan = result_regex_type.str( 1 );
	
  std::string Channel_to_send = PAIR_CHANNEL_ID + result_regex_num.str( 1 )
                          + SAMPLE_CHANNEL_ID;

  uint8_t* Byte_array = new uint8_t[ CHANNEL_SIZE ];
  
  uint8_t* Byte_ptr = Byte_array;

  for( uint8_t Sample_idx = 0; Sample_idx < CHANNEL_SIZE; Sample_idx++ )
  {
    *Byte_ptr =( uint8_t )( rand() % 100 + 1 );
    //cout << *Byte_ptr << endl;
    Byte_ptr++;
  }

  mosquitto_publish( mosq, NULL, Channel_to_send.c_str(), CHANNEL_SIZE, Byte_array, 0, 0 );

  delete [] Byte_array;
}

////////////////////////////////////////////////////////////////////////////////
int main() 
{
  int rc;

  ifstream ifs( "test.json" );
  Json::Reader reader;
  Json::Value obj;
  reader.parse( ifs, obj );
  uint8_t Num_pairs = obj[ "Num_LRUs" ].asUInt();
  uint8_t Num_samples_per_lru =
    obj[ "Cells_per_LRU" ].asUInt() * obj[ "Rotation_samples_per_cell" ].asUInt();
  
  const char* msg;
  const char* trigger_topic;
  const char* sample_topic;
  const Json::Value& LRUs = obj[ "LRUs" ];

  //mqtt_init();
  mosquitto_lib_init();
  
  mosq = mosquitto_new( NULL, CLEAN_SESSION, NULL );

  if ( mosq )
  {
    printf( "Setting callbacks\n");

    mosquitto_connect_callback_set( mosq, connect_callback );
    mosquitto_message_callback_set( mosq, message_callback );

    rc = mosquitto_connect( mosq, HOST, PORT, KEEPALIVE );

    for ( int i = 0; i < LRUs.size(); i++ )
    {
      trigger_topic = LRUs[ i ][ "Channel_trigger" ].asCString();
      mosquitto_subscribe( mosq, NULL, trigger_topic, 0 );
      cout << "Subscribed" << endl;
    }

    while ( 1 )
    {     
      rc = mosquitto_loop( mosq, -1, 1 );
      if ( rc )
      {
        printf( "Warning: lost mosquitto connection. Retrying.\n" );
        sleep( 1 );
        mosquitto_reconnect( mosq );
      }
    }
  }
  return 0;
}