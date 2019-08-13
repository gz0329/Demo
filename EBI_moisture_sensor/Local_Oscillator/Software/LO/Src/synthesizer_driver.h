////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017, Elliott Bay Industries. All rights reserved.
//
// Purpose:
// Take care of the register setup and details for using the synthesizer
//
// History:
// 2017-09-20  v1.0  George Zhang   Initial implementation
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "spi.h"
#include "trigger.h"

uint8_t Synth_0_ss_id;
uint8_t Synth_1_ss_id;

uint32_t Frequencies_mhz[ NUM_FREQUENCIES ];

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Set GPIO pins of slave selections to output. Register slave selections. 
// Set output frequency to 3Ghz on both synthesizer.
//
void init_synth( void );

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Turn on the synthesizer.
//
//void synth_on( void );

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Turn off the synthesizer.
//
//void synth_off( void );

////////////////////////////////////////////////////////////////////////////////
// Purpose:
// Set the output frequency on specific synthesizer.
//
// Param[ in ]  Freq   A uint32_t of frequency.
// Param[ in ]  Synth_idx   A uint8_t of the index of the synthesizer.
//
// Return true if the new frequency is set successfully.
// Return false if the frequency is out of range.
//
int set_synth_frequency( uint32_t Freq, uint8_t Synth_idx );
