////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Elliott Bay Industries. All rights reserved.
//
// Purpose:
// Row fill, row transmission, and circular buffering.
//
// History:
// 201i8-01-12  1.00  George Zhang   Initial implementation. 
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <mosquitto.h>
#include "row_manager.h"

using namespace std;

static std::deque< pair_samples_t > Row_buffer;
static uint8_t Num_lru_pairs_;
static uint8_t Samples_per_lru_;

////////////////////////////////////////////////////////////////////////////////
void row_manager_init( uint8_t Num_lru_pairs, uint8_t Samples_per_lru )
{
  Num_lru_pairs_ = Num_lru_pairs;
  Samples_per_lru_ = Samples_per_lru;
}

////////////////////////////////////////////////////////////////////////////////
pair_samples_t row_manager_get_zeroed_samples( void )
{
  pair_samples_t Pair_samples;

  for ( uint8_t Sample_idx = 0; Sample_idx < Samples_per_lru_; Sample_idx++ )
  {
    Pair_samples.push_back( DEFAULT_SAMPLE );
  }
  
  return Pair_samples;
}

////////////////////////////////////////////////////////////////////////////////
void row_manager_buffer_pair_samples( pair_samples_t Lru_pair_samples )
{
  Row_buffer.push_back( Lru_pair_samples );
}

////////////////////////////////////////////////////////////////////////////////
void row_manager_send_row( void )
{
  uint32_t Byte_array_size;
  Byte_array_size = Num_lru_pairs_ * Samples_per_lru_ * sizeof( sample_t );
  
  uint8_t* Byte_array = new uint8_t[ Byte_array_size ];
  
  uint8_t* Byte_ptr = Byte_array;
  
  for( uint8_t Lru_pair_idx = 0; Lru_pair_idx < Num_lru_pairs_; Lru_pair_idx++ )
  {
    pair_samples_t Pair_samples = Row_buffer.front();

    for( uint8_t Sample_idx = 0; Sample_idx < Samples_per_lru_; Sample_idx++ )
    {
      sample_t Sample = Pair_samples.front();
      *Byte_ptr = (uint8_t)( ( Sample >> 8 ) & 0xFF );
      Byte_ptr++;
      *Byte_ptr = (uint8_t)( ( Sample >> 0 ) & 0xFF );
      Byte_ptr++;
      
      Pair_samples.pop_front();
    }

    Row_buffer.pop_front();
  } 

  struct mosquitto* mosq_result = mosquitto_new( NULL, CLEAN_SESSION, NULL );

  mosquitto_connect( mosq_result, HOST, PORT, KEEPALIVE ); 

  mosquitto_publish( mosq_result, NULL, "/Sensor/Density/Result", 
                  Byte_array_size, Byte_array, 0, 0 );

  delete [] Byte_array;
}
  