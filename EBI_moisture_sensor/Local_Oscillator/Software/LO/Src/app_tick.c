///////////////////////////////////////////////////////////////////////////////
// Purpose:
// An interface to support coordinated, time tick triggered activity.
///////////////////////////////////////////////////////////////////////////////

#include "stm32h7xx_hal.h"
#include "app_tick.h"


static uint32_t App_tick = 0;


///////////////////////////////////////////////////////////////////////////////
uint32_t microseconds( void )
{
  uint32_t Tick_count = __LDREXW( &App_tick );
  uint32_t Microseconds = Tick_count * MICROSECONDS_PER_TICK;
  return Microseconds;
}

///////////////////////////////////////////////////////////////////////////////
void count_a_tick( void )
{
  uint32_t Tick_count;
  do {
    Tick_count = __LDREXW( &App_tick );
    Tick_count++;
  } while ( __STREXW( Tick_count, &App_tick ) );
}

