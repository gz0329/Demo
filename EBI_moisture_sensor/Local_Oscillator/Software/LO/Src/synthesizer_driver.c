////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017, Elliott Bay Industries. All rights reserved.
//
// Purpose:
// Take care of the register setup and details for using the synthesizer
//
// History:
// 2017-09-20  v1.0  George Zhang   Initial implementation
////////////////////////////////////////////////////////////////////////////////

#include "synthesizer_driver.h"
#include "gpio.h"

// 0-1, 1-2, 2-4, 3-8, 4-16
int Divider = 0;            
//double Step = 50.0;

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Slave select control functions needed by the SPI driver.
//
// Constraint:
// These must match the function signature of spi_slave_select_function.
//
void synth_0_slave_select_activate( void )
{
  HAL_GPIO_WritePin(GPIOE, SYNTH0_CS_Pin, GPIO_PIN_RESET);
}
void synth_0_slave_select_deactivate( void )
{
  HAL_GPIO_WritePin(GPIOE, SYNTH0_CS_Pin, GPIO_PIN_SET);
}

void synth_1_slave_select_activate( void )
{
  HAL_GPIO_WritePin(GPIOE, SYNTH1_CS_Pin, GPIO_PIN_RESET);
}
void synth_1_slave_select_deactivate( void )
{
  HAL_GPIO_WritePin(GPIOE, SYNTH1_CS_Pin, GPIO_PIN_SET);
}

#define SYN_NUM_BYTES_TO_SEND ( 4 )

void synth_delay ( uint32_t c )
{
  uint32_t i = 0;
  while ( i < c )
  {
    i++;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Set the pins related to synthesizer to proper in/out status
void synth_set_pin( void ) 
{
  HAL_GPIO_WritePin(GPIOE, SYNTH0_PDBrf_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, SYNTH0_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, SYNTH1_PDBrf_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, SYNTH1_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, SYNTH0_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, SYNTH1_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, SEL_SYNTH_0_Pin, GPIO_PIN_SET);

  Synth_0_ss_id = spi_4_register_slave( synth_0_slave_select_activate,
                                      synth_0_slave_select_deactivate );
  
  Synth_1_ss_id = spi_4_register_slave( synth_1_slave_select_activate,
                                      synth_1_slave_select_deactivate );
}

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Takes in the data needed to rewrite the registers in specific synthesizer.
// Send the data via SPI and toggled load enable.
void synth_send( uint32_t Data, uint8_t Synth_idx )
{
  uint8_t First =  ( Data >> 24 ) & 0xFF;
  uint8_t Second = ( Data >> 16 ) & 0xFF;
  uint8_t Third =  ( Data >>  8 ) & 0xFF;
  uint8_t Fourth = ( Data >>  0 ) & 0xFF;

  uint32_t Swapped = ( ( (uint32_t)Fourth << 24 )
                       + ( (uint32_t)Third  << 16 )
                       + ( (uint32_t)Second <<  8 )
                       + ( (uint32_t)First  <<  0 ) );

  spi_4_tx( Synth_idx, (uint8_t*)&Swapped, SYN_NUM_BYTES_TO_SEND );
  //synth_delay(100);
  //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
  //HAL_SPI_Transmit( &hspi4, (uint8_t*)&Swapped, 4, 0xff );
  //while(HAL_SPI_GetState(&hspi4) != HAL_SPI_STATE_READY);
  //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);

  return;
}

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Set the specific synthesizer output frequency by revising the data in
// register 0 and 4.
int set_synth_frequency( uint32_t Freq, uint8_t Synth_idx )
{
  uint32_t Data_r4 = 0;
  uint32_t Data_r1 = 0;
  uint32_t Data_r0 = 0;
  uint32_t Divider = 0;
  uint32_t Int = 0;
  uint32_t Frac = 0;
  uint32_t Mod = 0;
  double Freq_over_PFD = 0;

  if ( Freq > 137 && Freq < 300 )
  {
    Divider = 4; //16
    Freq = Freq * 16;
  } else if ( Freq > 200 && Freq < 600 )
  {
    Divider = 3; //8
    Freq = Freq * 8;
  } else if ( Freq > 500 && Freq < 1100 )
  {
    Divider = 2; //4
    Freq = Freq * 4;
  } else if ( Freq > 1000 && Freq < 2200 )
  {
    Divider = 1; //2
    Freq = Freq * 2;
  } else if ( Freq > 2100 && Freq < 4500 )
  {
    Divider = 0; //1
    Freq = Freq * 1;
  } else
  {
    return -1;
  }

  Int = Freq / 12;
  Freq_over_PFD = (double) Freq / 12;
  double Decimal = Freq_over_PFD - Int;

  Frac = (uint32_t)( Decimal * 1000 );
  Mod = 1000;

  // Set divider in register 4
  Data_r4 =
            ( (uint32_t)4                     ) |
            ( (uint32_t)3                << 3 ) |       // Output power ( 0-3 )
            ( (uint32_t)0b1              << 5 ) |       // RF out enable
            ( (uint32_t)3                << 6 ) |       // Aux power ( 0-3 )
            ( (uint32_t)0b1              << 8 ) |       // Aux enable
            ( (uint32_t)0b1              << 9 ) |       // Aux select
            ( (uint32_t)0b1              << 10 ) |      // Mute until locked ( 0 no, 1 yes )
            ( (uint32_t)0b0              << 11 ) |      // VCO power down
            ( (uint32_t)128              << 12 ) |      // Band select clock divider ( R )
            ( (uint32_t)Divider          << 20 ) |      // Output divider
            ( (uint32_t)0b1              << 23 );       // Feedback divider
  synth_send( Data_r4, Synth_idx );                     // Set to new division

  // Set MOD in register 1
  Data_r1 =
            ( ( uint32_t ) 1        ) |
            ( ( uint32_t ) Mod << 3 ) | // MOD
            ( ( uint32_t ) 1  << 15 ) | // PHASE
            ( ( uint32_t ) 1  << 27 );  // 8/9 PRESCALER
  synth_send( Data_r1, Synth_idx );                     // Set to new division

  // Set INT and FRAC in register 0
  Data_r0 =
            ( ( uint32_t ) 0         ) |
            ( ( uint32_t ) Frac << 3 ) |       // FRAC
            ( ( uint32_t ) Int << 15 );   // INT
  // Set to new division
  synth_send( Data_r0, Synth_idx );
  
  if ( Synth_idx == 0 )
  {
    while ( HAL_GPIO_ReadPin( GPIOE, SYNTH0_LD_Pin ) == 0 );
  } else if ( Synth_idx == 1 )
  {
    while ( HAL_GPIO_ReadPin( GPIOE, SYNTH1_LD_Pin ) == 0 );
  } else
  {
    return 0;
  }

  return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Purpose:
// Initialize synthesizer and set the frequency to be 3.4Ghz
void init_synth( void ) 
{
  Synth_0_ss_id = INVALID_SLAVE;
  Synth_1_ss_id = INVALID_SLAVE;

  Frequencies_mhz[ 0 ] = 2814;
  Frequencies_mhz[ 1 ] = 2838;
  Frequencies_mhz[ 2 ] = 2862;
  Frequencies_mhz[ 3 ] = 2886;

  synth_set_pin();
  
  // Data sent to registers 5 to 0 that initialize the synthesizer and set the
  // frequency to 3Ghz with a 24mhz ref freq

  #define R5 0x00580005
  #define R4 0x008603FC
  #define R3 0x000084B3
  #define R2 0x0C009E42
  #define R1 0x00008011
  #define R0 0x007D0000
/*
  synth_send( R5, Synth_1_ss_id );
  synth_send( R4, Synth_1_ss_id );
  synth_send( R3, Synth_1_ss_id );
  synth_send( R2, Synth_1_ss_id );
  synth_send( R1, Synth_1_ss_id );
  synth_send( R0, Synth_1_ss_id );

  synth_send( R5, Synth_0_ss_id );
  synth_send( R4, Synth_0_ss_id );
  synth_send( R3, Synth_0_ss_id );
  synth_send( R2, Synth_0_ss_id );
  synth_send( R1, Synth_0_ss_id );
  synth_send( R0, Synth_0_ss_id );
*/
  // SPI testing
  /*
#define LD_HIGH 0x00D80005
#define LD_LOW  0x00180005


  int i = 0;
  for ( i = 0; i < 100; i++ )
  {
    synth_0_send( LD_LOW );
    synth_0_send( LD_HIGH );
  }

  while( 1 )
  {
    synth_0_send( R5 );
    synth_0_send( R4 );
    synth_0_send( R3 );
    synth_0_send( R2 );
    synth_0_send( R1 );
    synth_0_send( R0 );
  }
*/
  ////////////////////////////////////////////////////////////////////////////////
  // Register 5 Control bit: 101.
  #define SYNTH_REG_5 ( ( uint32_t ) 0b101 )
  // 23:22 Lock Detect Pin Operation 00: Low.
  //                                 01: Digital Lock Detect.
  //                                 11: High.
  #define SYNTH_REG_5_LOCK_DETECT_LOW  ( ( uint32_t ) 0b00 << 22 )
  #define SYNTH_REG_5_LOCK_DETECT_ON   ( ( uint32_t ) 0b01 << 22 )
  #define SYNTH_REG_5_LOCK_DETECT_HIGH ( ( uint32_t ) 0b11 << 22 )
  // 20:19 Default to 11
  #define SYNTH_REG_5_RESERVED  ( ( uint32_t ) 0b11 << 19 )
  synth_send( ( uint32_t ) SYNTH_REG_5_LOCK_DETECT_ON
              | ( uint32_t ) SYNTH_REG_5_RESERVED
              | ( uint32_t ) SYNTH_REG_5
              , Synth_0_ss_id);
  synth_delay(100);
  synth_send( ( uint32_t ) SYNTH_REG_5_LOCK_DETECT_ON
              | ( uint32_t ) SYNTH_REG_5_RESERVED
              | ( uint32_t ) SYNTH_REG_5
              , Synth_1_ss_id);
  synth_delay(100);
  ////////////////////////////////////////////////////////////////////////////////
  // Register 4 Control bit: 100.
  #define SYNTH_REG_4 ( ( uint32_t ) 0b100 )
  // 4:3 Output Power 00: -4.
  //                  01: -1.
  //                  10: +2.
  //                  11: +5.
  #define SYNTH_REG_4_OUTPUT_POWER_N4 ( ( uint32_t ) 0b00 << 3 )
  #define SYNTH_REG_4_OUTPUT_POWER_N1 ( ( uint32_t ) 0b01 << 3 )
  #define SYNTH_REG_4_OUTPUT_POWER_P2 ( ( uint32_t ) 0b10 << 3 ) 
  #define SYNTH_REG_4_OUTPUT_POWER_P5 ( ( uint32_t ) 0b11 << 3 )
  // 5 RF Output Enable 0: Disabled.
  //                    1: Enabled.
  #define SYNTH_REG_4_RF_OUTPUT_OFF ( ( uint32_t ) 0 << 5 )
  #define SYNTH_REG_4_RF_OUTPUT_ON  ( ( uint32_t ) 1 << 5 )
  // 7:6 Aux Output Power 00: -4.
  //                      01: -1.
  //                      10: +2.
  //                      11: +5.
  #define SYNTH_REG_4_AUX_OUTPUT_POWER_N4 ( ( uint32_t ) 0b00 << 6 )
  #define SYNTH_REG_4_AUX_OUTPUT_POWER_N1 ( ( uint32_t ) 0b01 << 6 )
  #define SYNTH_REG_4_AUX_OUTPUT_POWER_P2 ( ( uint32_t ) 0b10 << 6 ) 
  #define SYNTH_REG_4_AUX_OUTPUT_POWER_P5 ( ( uint32_t ) 0b11 << 6 )
  // 8 Aux Output Enable 0: Disabled.
  //                     1: Enabled.
  #define SYNTH_REG_4_AUX_OUTPUT_OFF ( ( uint32_t ) 0 << 8 )
  #define SYNTH_REG_4_AUX_OUTPUT_ON  ( ( uint32_t ) 1 << 8 )
  // 9 Aux Output Select 0: Divided Output.
  //                     1: Fundamental.
  #define SYNTH_REG_4_AUX_OUTPUT_DIVIDED      ( ( uint32_t ) 0 << 9 )
  #define SYNTH_REG_4_AUX_OUTPUT_FUNDAMENTAL  ( ( uint32_t ) 1 << 9 )
  // 10 MTLD Mute Till Lock Detect 0: Mute Disabled.
  //                               1: Mute Enabled.
  #define SYNTH_REG_4_MUTE_OFF ( ( uint32_t ) 0 << 10 )
  #define SYNTH_REG_4_MUTE_ON  ( ( uint32_t ) 1 << 10 )
  // 11 VCO Power-down 0: VCO Powered up.
  //                   1: VCO Powered down.
  #define SYNTH_REG_4_VCO_ON  ( ( uint32_t ) 0 << 11 )
  #define SYNTH_REG_4_VCO_OFF ( ( uint32_t ) 1 << 11 )
  // 19:12 8-BIT BAND SELECT CLOCK DIVIDER VALUE.
  //                  Selected by software.
  #define SYNTH_REG_4_BAND_SEL_CLK_DIVIDER ( ( uint32_t ) 0b01100000 << 12 )
  // 22:20 3-BIT DOUBLE BUFFER RF DIVIDER SELECT.
  //                  Selected by software.
  #define SYNTH_REG_4_DOUBLE_BUFFER_DIVIDER ( ( uint32_t ) 0b000 << 20 )
  // 23 Feedback Select 0: Divided Output.
  //                    1: Fundamental.
  #define SYNTH_REG_4_FEEDBACK_DIVIDED      ( ( uint32_t ) 0 << 23 )
  #define SYNTH_REG_4_FEEDBACK_FUNDAMENTAL  ( ( uint32_t ) 1 << 23 )
  synth_send( ( uint32_t ) SYNTH_REG_4_FEEDBACK_FUNDAMENTAL
              | ( uint32_t ) SYNTH_REG_4_DOUBLE_BUFFER_DIVIDER
              | ( uint32_t ) SYNTH_REG_4_BAND_SEL_CLK_DIVIDER
              | ( uint32_t ) SYNTH_REG_4_VCO_ON
              | ( uint32_t ) SYNTH_REG_4_MUTE_OFF
              | ( uint32_t ) SYNTH_REG_4_AUX_OUTPUT_FUNDAMENTAL
              | ( uint32_t ) SYNTH_REG_4_AUX_OUTPUT_ON
              | ( uint32_t ) SYNTH_REG_4_AUX_OUTPUT_POWER_P5
              | ( uint32_t ) SYNTH_REG_4_RF_OUTPUT_ON
              | ( uint32_t ) SYNTH_REG_4_OUTPUT_POWER_P5
              | ( uint32_t ) SYNTH_REG_4
              , Synth_0_ss_id );
  synth_delay(100);
  synth_send( ( uint32_t ) SYNTH_REG_4_FEEDBACK_FUNDAMENTAL
              | ( uint32_t ) SYNTH_REG_4_DOUBLE_BUFFER_DIVIDER
              | ( uint32_t ) SYNTH_REG_4_BAND_SEL_CLK_DIVIDER
              | ( uint32_t ) SYNTH_REG_4_VCO_ON
              | ( uint32_t ) SYNTH_REG_4_MUTE_OFF
              | ( uint32_t ) SYNTH_REG_4_AUX_OUTPUT_FUNDAMENTAL
              | ( uint32_t ) SYNTH_REG_4_AUX_OUTPUT_ON
              | ( uint32_t ) SYNTH_REG_4_AUX_OUTPUT_POWER_P5
              | ( uint32_t ) SYNTH_REG_4_RF_OUTPUT_ON
              | ( uint32_t ) SYNTH_REG_4_OUTPUT_POWER_P5
              | ( uint32_t ) SYNTH_REG_4
              , Synth_1_ss_id );
  synth_delay(100);
  
  ////////////////////////////////////////////////////////////////////////////////
  // Register 3 Control bit: 011.
  #define SYNTH_REG_3 ( ( uint32_t ) 0b011 )
  // 14:3 12-BIT CLOCK DIVIDER VALUE.
  //                  Selected by software.
  #define SYNTH_REG_3_CLK_DIVIDER ( ( uint32_t ) 0b000010010110 << 3 )
  // 16:15 CLK DIV MODE 00: Clock Divider Off.
  //                    01: Fast-lock Enable.
  //                    10: Resync Enable.
  #define SYNTH_REG_3_CLK_DIVIDER_OFF       ( ( uint32_t ) 0b00 << 15 )
  #define SYNTH_REG_3_CLK_DIVIDER_FAST_LOCK ( ( uint32_t ) 0b01 << 15 )
  #define SYNTH_REG_3_CLK_DIVIDER_RESYNC    ( ( uint32_t ) 0b10 << 15 )
  // 18 CSR CYCLE SLIP REDUCTION 0: CSR Disabled.
  //                             1: CSR Enabled.
  #define SYNTH_REG_3_CSR_OFF ( ( uint32_t ) 0 << 18 )
  #define SYNTH_REG_3_CSR_ON  ( ( uint32_t ) 1 << 18 )  
  synth_send( ( uint32_t ) SYNTH_REG_3_CSR_OFF
              | ( uint32_t ) SYNTH_REG_3_CLK_DIVIDER_OFF
              | ( uint32_t ) SYNTH_REG_3_CLK_DIVIDER
              | ( uint32_t ) SYNTH_REG_3
              , Synth_0_ss_id );
  synth_delay(100);
  synth_send( ( uint32_t ) SYNTH_REG_3_CSR_OFF
              | ( uint32_t ) SYNTH_REG_3_CLK_DIVIDER_OFF
              | ( uint32_t ) SYNTH_REG_3_CLK_DIVIDER
              | ( uint32_t ) SYNTH_REG_3
              , Synth_1_ss_id );
  synth_delay(100);
  
  ////////////////////////////////////////////////////////////////////////////////
  // Register 2 Control bit: 010.
  #define SYNTH_REG_2 ( ( uint32_t ) 0b010 )
  // 3 Counter Reset 0: Counter Reset Disabled.
  //                 1: Counter Reset Enabled.
  #define SYNTH_REG_2_COUNTER_RESET_OFF ( ( uint32_t ) 0 << 3 )
  #define SYNTH_REG_2_COUNTER_RESET_ON  ( ( uint32_t ) 1 << 3 )
  // 4 CP Three-state 0: CP Three-state Disabled.
  //                  1: CP Three-state Enabled.
  #define SYNTH_REG_2_CP_THREE_STATE_OFF ( ( uint32_t ) 0 << 4 )
  #define SYNTH_REG_2_CP_THREE_STATE_ON  ( ( uint32_t ) 1 << 4 )
  // 5 Power Down 0: Power Down Disabled.
  //              1: Power Down Enabled.
  #define SYNTH_REG_2_POWER_DOWN_OFF ( ( uint32_t ) 0 << 5 )
  #define SYNTH_REG_2_POWER_DOWN_ON  ( ( uint32_t ) 1 << 5 )
  // 6 PD POLARITY 0: Negative.
  //               1: Positive.
  #define SYNTH_REG_2_PD_POLARITY_NEGATIVE ( ( uint32_t ) 0 << 6 )
  #define SYNTH_REG_2_PD_POLARITY_POSITIVE ( ( uint32_t ) 1 << 6 )
  // 7 LDP 0: 10ns.
  //       1: 6ns.
  #define SYNTH_REG_2_LDP_10NS ( ( uint32_t ) 0 << 7 )
  #define SYNTH_REG_2_LDP_6NS  ( ( uint32_t ) 1 << 7 )
  // 8 LDF 0: FRAC-N.
  //       1: INT-N.
  #define SYNTH_REG_2_LDF_FRAC ( ( uint32_t ) 0 << 8 )
  #define SYNTH_REG_2_LDP_INT  ( ( uint32_t ) 1 << 8 )
  // 12:9 Charge Pump Current Setting.
  //                  Selected by software.
  #define SYNTH_REG_2_PUMP_CURRENT ( ( uint32_t ) 0b1111 << 9 )
  // 13 Double Buff 0: Double Buff disabled.
  //                1: Double Buff enabled.
  #define SYNTH_REG_2_DOUBLE_BUFF_OFF ( ( uint32_t ) 0 << 13 )
  #define SYNTH_REG_2_DOUBLE_BUFF_ON  ( ( uint32_t ) 1 << 13 )
  // 23:14 10-BIT R COUNTER
  //                  Selected by software.
  #define SYNTH_REG_2_R_COUNTER ( ( uint32_t ) 0b0000000010 << 14 )
  // 24 REFERENCE DIVIDED BY 2 0: REFERENCE DIVIDED BY 2 disabled.
  //                          1: REFERENCE DIVIDED BY 2 enabled.
  #define SYNTH_REG_2_REFERENCE_DIVIDED_OFF ( ( uint32_t ) 0 << 24 )
  #define SYNTH_REG_2_REFERENCE_DIVIDED_ON  ( ( uint32_t ) 1 << 24 )
  // 25 REFERENCE MUTIPLIED BY 2 0: REFERENCE MUTIPLIED BY 2 disabled.
  //                             1: REFERENCE MUTIPLIED BY 2 enabled.
  #define SYNTH_REG_2_REFERENCE_MUTIPLIED_OFF ( ( uint32_t ) 0 << 25 )
  #define SYNTH_REG_2_REFERENCE_MUTIPLIED_ON  ( ( uint32_t ) 1 << 25 )
  // 28:26 MUXOUT 000: THREE-STATE OUTPUT
  //              001: DVDD
  //              010: DGND
  //              011: R DIVIDER OUTPUT
  //              100: N DIVIDER OUTPUT
  //              101: ANALOG LOCK DETECT
  //              110: DIGITAL LOCK DETECT
  #define SYNTH_REG_2_MUXOUT_THREE_STATE         ( ( uint32_t ) 0b000 << 26 )
  #define SYNTH_REG_2_MUXOUT_DVDD                ( ( uint32_t ) 0b001 << 26 )
  #define SYNTH_REG_2_MUXOUT_DGND                ( ( uint32_t ) 0b010 << 26 )
  #define SYNTH_REG_2_MUXOUT_R_DIVIDER           ( ( uint32_t ) 0b011 << 26 )
  #define SYNTH_REG_2_MUXOUT_N_DIVIDER           ( ( uint32_t ) 0b100 << 26 )
  #define SYNTH_REG_2_MUXOUT_ANALOG_LOCK_DETECT  ( ( uint32_t ) 0b101 << 26 )
  #define SYNTH_REG_2_MUXOUT_DIGITAL_LOCK_DETECT ( ( uint32_t ) 0b110 << 26 )
  // 30:29 NOISE MODE 00: LOW NOISE MODE
  //                  11: LOW SPUR MODE
  #define SYNTH_REG_2_LOW_NOISE ( ( uint32_t ) 0b00 << 29 )
  #define SYNTH_REG_2_LOW_SPUR  ( ( uint32_t ) 0b11 << 29 )
  synth_send( ( uint32_t ) SYNTH_REG_2_LOW_NOISE
              | ( uint32_t ) SYNTH_REG_2_MUXOUT_R_DIVIDER
              | ( uint32_t ) SYNTH_REG_2_REFERENCE_MUTIPLIED_OFF
              | ( uint32_t ) SYNTH_REG_2_REFERENCE_DIVIDED_OFF
              | ( uint32_t ) SYNTH_REG_2_R_COUNTER
              | ( uint32_t ) SYNTH_REG_2_DOUBLE_BUFF_OFF
              | ( uint32_t ) SYNTH_REG_2_PUMP_CURRENT
              | ( uint32_t ) SYNTH_REG_2_LDF_FRAC
              | ( uint32_t ) SYNTH_REG_2_LDP_10NS
              | ( uint32_t ) SYNTH_REG_2_PD_POLARITY_POSITIVE
              | ( uint32_t ) SYNTH_REG_2_POWER_DOWN_OFF
              | ( uint32_t ) SYNTH_REG_2_CP_THREE_STATE_OFF
              | ( uint32_t ) SYNTH_REG_2_COUNTER_RESET_OFF
              | ( uint32_t ) SYNTH_REG_2
              , Synth_0_ss_id );
  synth_delay(100);
  synth_send( ( uint32_t ) SYNTH_REG_2_LOW_NOISE
              | ( uint32_t ) SYNTH_REG_2_MUXOUT_R_DIVIDER
              | ( uint32_t ) SYNTH_REG_2_REFERENCE_MUTIPLIED_OFF
              | ( uint32_t ) SYNTH_REG_2_REFERENCE_DIVIDED_OFF
              | ( uint32_t ) SYNTH_REG_2_R_COUNTER
              | ( uint32_t ) SYNTH_REG_2_DOUBLE_BUFF_OFF
              | ( uint32_t ) SYNTH_REG_2_PUMP_CURRENT
              | ( uint32_t ) SYNTH_REG_2_LDF_FRAC
              | ( uint32_t ) SYNTH_REG_2_LDP_10NS
              | ( uint32_t ) SYNTH_REG_2_PD_POLARITY_POSITIVE
              | ( uint32_t ) SYNTH_REG_2_POWER_DOWN_OFF
              | ( uint32_t ) SYNTH_REG_2_CP_THREE_STATE_OFF
              | ( uint32_t ) SYNTH_REG_2_COUNTER_RESET_OFF
              | ( uint32_t ) SYNTH_REG_2
              , Synth_1_ss_id );
  synth_delay(100);
  
  ////////////////////////////////////////////////////////////////////////////////
  // Register 1 Control bit: 001.
  #define SYNTH_REG_1 ( ( uint32_t ) 0b001 )
  // 14:3 12-BIT MODULUS VALUE (MOD).
  //                  Selected by software.
  #define SYNTH_REG_1_MODULUS_VALUE ( ( uint32_t ) 0b000000000010 << 3 )
  // 26:15 12-BIT PHASE VALUE (PHASE).
  //                  Selected by software.
  #define SYNTH_REG_1_PHASE_VALUE ( ( uint32_t ) 0b000000000001 << 15 )
  // 27 Prescaler 0: 4/5
  //              1: 8/9
  #define SYNTH_REG_1_PRESCALER_4_5 ( ( uint32_t ) 0b0 << 27 )
  #define SYNTH_REG_1_PRESCALER_8_9 ( ( uint32_t ) 0b1 << 27 )
  synth_send( ( uint32_t ) SYNTH_REG_1_PRESCALER_8_9
              | ( uint32_t ) SYNTH_REG_1_PHASE_VALUE
              | ( uint32_t ) SYNTH_REG_1_MODULUS_VALUE
              | ( uint32_t ) SYNTH_REG_1
              , Synth_0_ss_id );
  synth_delay(100);
  synth_send( ( uint32_t ) SYNTH_REG_1_PRESCALER_8_9
              | ( uint32_t ) SYNTH_REG_1_PHASE_VALUE
              | ( uint32_t ) SYNTH_REG_1_MODULUS_VALUE
              | ( uint32_t ) SYNTH_REG_1
              , Synth_1_ss_id );
  synth_delay(100);
  
  ////////////////////////////////////////////////////////////////////////////////
  // Register 0 Control bit: 000.
  #define SYNTH_REG_0 ( ( uint32_t ) 0b000 )
  // 14:3 12-BIT FRACTIONAL VALUE (FRAC).
  //                  Selected by software.
  #define SYNTH_REG_0_FRACTIONAL_VALUE ( ( uint32_t ) 0b000000000000 << 3 )
  // 30:15 16-BIT INTEGER VALUE (INT).
  //                  Selected by software.
  #define SYNTH_REG_0_INTEGER_VALUE ( ( uint32_t ) 0b0000000011111010 << 15 )
  synth_send( ( uint32_t ) SYNTH_REG_0_INTEGER_VALUE
              | ( uint32_t ) SYNTH_REG_0_FRACTIONAL_VALUE
              | ( uint32_t ) SYNTH_REG_0
              , Synth_0_ss_id );
  synth_delay(100);
  synth_send( ( uint32_t ) SYNTH_REG_0_INTEGER_VALUE
              | ( uint32_t ) SYNTH_REG_0_FRACTIONAL_VALUE
              | ( uint32_t ) SYNTH_REG_0
              , Synth_1_ss_id );
  synth_delay(100);

  return;
}
