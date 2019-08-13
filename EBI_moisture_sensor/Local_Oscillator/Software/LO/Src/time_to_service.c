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
//////////////////////////////////////////////////////////////////////////////////////////////////


#include "time_to_service.h"


//////////////////////////////////////////////////////////////////////////////////////////////////
void time_to_service_init( time_to_service_t* This, uint32_t Period_ms )
{
  // Note: TIME_TO_SERVICE_INFINITE is a dual-use flag. It means never trigger or infinite period
  //       when used in the Period_ms field. It means repeat periods forever when used in the
  //       Num_periods field.
  time_to_service_setup_periods( This, 0, Period_ms, TIME_TO_SERVICE_INFINITE );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void time_to_service_setup_periods( time_to_service_t* This, uint32_t Curr_time_ms,
uint32_t Period_ms, uint32_t Num_periods )
{
  // Fastest possible service period is 1 millisecond.
  if ( Period_ms == 0 ) Period_ms = 1;
  This->Period_ms_ = Period_ms;
  This->Next_service_ms_ = Curr_time_ms + This->Period_ms_;
  This->Num_periods_ = Num_periods;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool is_next_service_time( time_to_service_t* This, uint32_t Curr_time_ms )
{ 
  // Delta is the difference between next service time and current time count. 
  uint32_t Delta = ( This->Next_service_ms_ - Curr_time_ms );

  if ( (int32_t)Delta <= 0 )
  {
    This->Next_service_ms_ = Curr_time_ms + This->Period_ms_;
    if ( This->Num_periods_ > 0 )
    {
      if ( This->Num_periods_ < TIME_TO_SERVICE_INFINITE )
      {
        --This->Num_periods_;
      }

      return true;
    }
    return true;
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t time_to_service_get_num_remaining_periods( time_to_service_t* This )
{
  return This->Num_periods_;
}

