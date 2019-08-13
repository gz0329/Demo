///////////////////////////////////////////////////////////////////////////////
// Purpose:
// An interface to support coordinated, time tick triggered activity.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>


///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Reflect the number of microseconds in each tick.
//
// Constraint:
// The hardware timer must be set up to match this setting.
//
#define MICROSECONDS_PER_TICK ( 100L )

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// The client code uses this to see what the latest tick count is.
//
// Note:
// This function call is thread safe, and is intended for general purpose use
// in reacting to passage of time.
//
// Note:
// This functionality depends on a regular, low jitter, high accuracy timer
// interrupt service routine which calls the counter function below.
//
// Note:
// The output will wrap around at 2^32 microseconds, or 4,294.967295 s,
// approximately 1.2 hours. Your code must use the following algorithm to
// assure correctness despite wrap-around:
//   static uint32_t Start_time_us = microseconds();
//   ...
//   uint32_t Curr_time_us = microseconds();
//   uint32_t Delta_time_us = Curr_time_us - Start_time_us;
// Because of C/C++ uint32_t rules for a subtraction, the delta in us will
// come out correctly even if it wraps around.
//
// Returns:
// The number of (wrapped) microseconds since the timer init call.
//
uint32_t microseconds( void );

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Keeps track of the current number of ticks that have passed.
//
// Note:
// This function must be called by a hardware timer interrupt service routine.
//
void count_a_tick( void );
