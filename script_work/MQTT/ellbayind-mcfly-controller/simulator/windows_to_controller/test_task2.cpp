////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Elliott Bay Industries. All rights reserved.
//
// Purpose:
//
// History:
//
////////////////////////////////////////////////////////////////////////////////


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
#include "row_manager.h"
#include "test_task2.h"

// TODO - Move all of these to the configuration file.
#define CHANNEL_SIZE 64
#define PAIR_CHANNEL_ID "/Sensor/Density/LRU_pair_"
#define SAMPLE_CHANNEL_ID "Samples"
#define TRIGGER_CHANNEL_ID "Trigger"

using namespace std;
struct mosquitto* mosq = NULL;

std::string msg_callback;
std::string topic_callback;
uint32_t len_callback;

uint8_t transmit_row_idx = 0;
uint8_t Num_samples_per_lru;
uint8_t one_time_flag = 0;

////////////////////////////////////////////////////////////////////////////////
void connect_callback( struct mosquitto* mosq, void* obj, int result )
{
  printf( "Connect callback, rc=%d\n", result );
}

////////////////////////////////////////////////////////////////////////////////
void disconnect_callback( struct mosquitto* mosq, void* obj, int result )
{
  printf( "Unexpected disconnection, rc=%d\n", result );
}

////////////////////////////////////////////////////////////////////////////////
void publish_callback( struct mosquitto* mosq, void* obj, int mid )
{
  printf( "Published , mid=%d\n", mid );
}

static pair_samples_t Curr_pair_samples;
volatile uint32_t Total_bytes_rx;

////////////////////////////////////////////////////////////////////////////////
void message_callback( struct mosquitto* mosq, void* obj,
  const struct mosquitto_message* message )
{
  len_callback = message->payloadlen;
  msg_callback = (char*)( message->payload );
  unsigned char* msg = ( unsigned char* )( message->payload );
  topic_callback = ( message->topic );

  // Parse the message string to know trigger vs sample, and to know pair_idx.
  bool is_trigger = false;
  bool is_samples = false;
  
  std::regex topic_regex_num(
    "/Sensor/Density/LRU_pair_(\\d+)/\\b(Samples|Trigger)" );

  std::regex topic_regex_type( "\\b(Samples|Trigger)([^ ]*)" );
  
  std::smatch result_regex_num;
  std::smatch result_regex_type;
  std::regex_search( topic_callback, result_regex_num, topic_regex_num );
  std::regex_search( topic_callback, result_regex_type, topic_regex_type );
  
  int lru_pair_idx = stoi( result_regex_num.str( 1 ) );
  string result_chan = result_regex_type.str( 1 );
  
  if ( result_chan.compare( TRIGGER_CHANNEL_ID ) == 0 )
  {
    is_trigger = true;
    is_samples = false;
  }
  else if ( result_chan.compare( SAMPLE_CHANNEL_ID ) == 0 )
  {
    is_trigger = false;
    is_samples = true;
  }
  else
  {
    is_trigger = false;
    is_samples = false;
  }
  
  if ( is_trigger )
  {
    row_manager_buffer_pair_samples( Curr_pair_samples );
    //----- Start next pair
    Curr_pair_samples = row_manager_get_zeroed_samples();

    if ( lru_pair_idx == transmit_row_idx )
    {
      if ( one_time_flag )
      {
        row_manager_send_row();
      }
      one_time_flag = 1;  
    }
  }
  
  if ( is_samples )
  {
    Curr_pair_samples.clear();

    const uint8_t* Byte_ptr = (uint8_t*)msg;
    for ( uint8_t Sample_idx = 0; Sample_idx < Num_samples_per_lru; Sample_idx++ )
    {
      sample_t Sample = ( *Byte_ptr << 8 );
      Byte_ptr++;
      Sample += *Byte_ptr;
      Byte_ptr++;
      Curr_pair_samples.push_back( Sample );
    }
    Total_bytes_rx += Num_samples_per_lru * sizeof( sample_t );
  } 
}

////////////////////////////////////////////////////////////////////////////////
int main( void )
{
  int rc;
  Total_bytes_rx = 0;
  ifstream ifs( "test.json" );
  Json::Reader reader;
  Json::Value obj;
  reader.parse( ifs, obj );
  uint8_t Num_pairs = obj[ "Num_LRUs" ].asUInt();
  Num_samples_per_lru =
    obj[ "Cells_per_LRU" ].asUInt() * obj[ "Rotation_samples_per_cell" ].asUInt();
  cout << "Number of LRU: " << Num_pairs << endl;
  cout << "Number of Cells per LRU: " << obj[ "Cells_per_LRU" ].asUInt() << endl;
  cout << "Rotation Samples per Cell: " << obj[ "Rotation_samples_per_cell" ].asUInt() << endl;
  cout << "LRUS Comment: " << obj[ "LRUs_comment" ].asString() << endl;
  cout << endl;
  
  const char* msg;
  const char* trigger_topic;
  const char* sample_topic;
  const Json::Value& LRUs = obj[ "LRUs" ];

  mosquitto_lib_init();

  row_manager_init( Num_pairs, Num_samples_per_lru );
  Curr_pair_samples = row_manager_get_zeroed_samples();
  
  mosq = mosquitto_new( NULL, CLEAN_SESSION, NULL );

  if ( mosq )
  {
    printf( "Setting callbacks\n");

    mosquitto_connect_callback_set( mosq, connect_callback );
    mosquitto_message_callback_set( mosq, message_callback );

    rc = mosquitto_connect( mosq, HOST, PORT, KEEPALIVE );

    for ( int i = 0; i < LRUs.size(); i++ )
    {
      cout << "Channel for trigger: " << LRUs[i]["Channel_trigger"].asString() << endl;
      cout << "Channel for samples: " << LRUs[i]["Channel_sample"].asString() << endl;

      trigger_topic = LRUs[ i ][ "Channel_trigger" ].asCString();
      sample_topic = LRUs[ i ][ "Channel_sample" ].asCString();

      mosquitto_subscribe( mosq, NULL, trigger_topic, 0 );
      mosquitto_subscribe( mosq, NULL, sample_topic, 0 );
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
        rc = mosquitto_connect( mosq, HOST, PORT, KEEPALIVE );
      }
      printf( "\b\b\b\b%lu\n", Total_bytes_rx );
    }
  }
  
  return 0;
}