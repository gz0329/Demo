////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Elliott Bay Industries. All rights reserved.
//
// Purpose:
// Row fill, row transmission, and circular buffering.
//
// History:
// 201i8-01-12  1.00  George Zhang   Initial implementation. 
////////////////////////////////////////////////////////////////////////////////


#include <stdint.h>
#include <deque>
#include <mosquitto.h>
#include "test_task2.h"

#define HOST "192.168.0.45"
#define PORT 1883
#define COUNT 1
#define KEEPALIVE 60
#define CLEAN_SESSION true

typedef uint16_t sample_t;
typedef std::deque< sample_t > pair_samples_t;

#define DEFAULT_SAMPLE ( (sample_t)0 )

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Set up internal buffering to receive samples.
//
// Param[in]  Num_lru_pairs  Number of LRU pairs in the row.
// Param[in]  Samples_per_lru  Number of samples received for each LRU pair.
//
void row_manager_init( uint8_t Num_lru_pairs, uint8_t Samples_per_lru );

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Provide default (empty) set of samples for an LRU pair. (used for timeout
// situations)
//
// Returns:
// A set of samples, of size Samples_per_lru (as set during initialization),
// containing all DEFAULT_SAMPLE values.
pair_samples_t row_manager_get_zeroed_samples( void );

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Buffer up each LRU pair set of samples, ready to go out with the row.
//
// Param[in]  Lru_pair_samples  A collected set of samples, lined up ready to
//                              transmit.
//
void row_manager_buffer_pair_samples( pair_samples_t Lru_pair_samples );

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Wrap up the current row and send as message to the UI.
//
// Constraint:
// Do not call until at least Num_lru_pairs (from init) sets of samples have
// been placed into the buffer using row_manager_buffer_pair_samples( ... ).
//
void row_manager_send_row( void );
