//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2017  Auric Consulting, LLC  All rights reserved.
//
// Purpose:
// Provides a convenient and clean interface for checking whether a periodic time has lapsed.
//
// Side-effects:
// The periods are properly tracked such that if the first period doesn't get serviced in time,
// that doesn't matter during the following period. For example, if the periods are set to
// 10 milliseconds, and the first call to is_next_service_time(...) doesn't happen until
// 14 ms, then it returns true, but won't return true again until 24 ms.
//
// History:
// 2017-06-21  v1.0  Dan Tebbs     Converted from C++ to C in order to accommodate a very small uC.
//
// Basic Example:
/*
#include "time_to_service.h"
const int Check_tick_ms = 10;
time_to_service_t Check_tick;

void setup( void )
{
  time_to_service_init( &Check_tick, Check_tick_ms );
}

void loop( void )
{
  uint32_t Curr_time_ms = millis();

  if ( is_next_service_time( &Check_tick, Curr_time_ms ) )
  {
    // It's time to do yer thang again.
  }
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


#include <stdbool.h>
#include <stdint.h>


// Note: TIME_TO_SERVICE_INFINITE is a dual-use flag.
//       1. It means never trigger or infinite period when used in the Period_ms field.
//          (useful to turn off triggering if needed)
//       2. It means repeat periods forever when used in the Num_periods field.
//          (the default / general use case)
#define TIME_TO_SERVICE_INFINITE ( ( uint32_t )-1 )
// The avr-gcc compiler isn't recognizing UINT32_MAX for some reason. Using (uint32_t)-1 as a
// work-around.
#ifndef UINT32_MAX
#define UINT32_MAX ( (uint32_t)( -1 ) )
#endif
// 4294967.294 seconds or 49.71 days.
#define TIME_TO_SERVICE_SLOWEST_PERIOD_ms ( UINT32_MAX - 1 )


typedef struct {
  uint32_t Period_ms_;
  uint32_t Next_service_ms_;
  uint32_t Num_periods_;
} time_to_service_t;


//////////////////////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Provides a convenient and clean interface for checking whether a periodic time has lapsed.
//
// Side-effects:
// The periods are properly tracked such that if the first period doesn't get serviced in time,
// that doesn't matter during the following period. For example, if the periods are set to
// 10 milliseconds, and the first call to is_next_service_time(...) doesn't happen until
// 14 ms, then it returns true, but won't return true again until 24 ms.
//
// Param[in]:  Period_ms  The number of milliseconds to use for each period.
void time_to_service_init( time_to_service_t* This, uint32_t Period_ms );

//////////////////////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Extra feature providing a way to do a single period, or N periods, and then cease to trigger.
// Works well for say turning on an LED for 1 s, and then turning it back off.
//
// Notes:
// The default operation is infinite periods. Calling this overrides that mode, and causes
// calls to is_next_service_time(...) to cease returning true after the specified number of
// periods elapse. For example: set_num_periods( 50, 7 ); will cause is_next_service_time(...)
// to return true after 50 ms, and after 100 ms, and so on until the 7th one at 350 ms has
// elapsed.
// * If you call with Num_periods == TIME_TO_SERVICE_INFINITE, that switches back to
// forever periods mode, disabling this feature.
// * If you want the trigger to happen instantly, use 0 for Curr_time_ms.
// * The next service will occur one new period from the time of this call.
// * You can disable the triggering by setting Num_periods to 0.
//
// Param[in]: Curr_time_ms  The start time against which periods will be measured.
// Param[in]: Period_ms     The number of milliseconds for each "tick".
// Param[in]: Num_periods   The number of times that calls to is_next_service_time(...)
//                          will return true. Use TIME_TO_SERVICE_INFINITE if you want to
//                          forever trigger each period.
void time_to_service_setup_periods( time_to_service_t* This, uint32_t Curr_time_ms,
  uint32_t Period_ms, uint32_t Num_periods );
  
//////////////////////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Checks to see if the currently elapsing period has completed or not.
//
// Side-effects:
// If the setup_periods(...) method has been used, then it becomes possible for this method to
// never return true again. In that case, setup_periods(...) must be called again.
// In the rare case that wrap-around occurs, the trigger is still handled correctly.
//
// Param[in]: Curr_time_ms  The current time at the moment of call.
//
// Returns: false  while waiting for a period to elapse.
//          true   when the current time matches or is greater than the period completion time.
//
// Example:
// If the period is set to 50 ms, and the previous return of true was at 1350 ms, then this will
// return false until the current time reaches 1400 ms, at which point it will return true, just
// once.
bool is_next_service_time( time_to_service_t* This, uint32_t Curr_time_ms );

//////////////////////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Just in case you need to know how many periods are left before it will (or has) quit triggering.
uint32_t time_to_service_get_num_remaining_periods( time_to_service_t* This );
