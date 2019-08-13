/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

/* USER CODE BEGIN 0 */
#include "sockets.h"
#include "math.h"
#include "tim.h"
#include <string.h>
#include "../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy/stm32_hal_legacy.h"
#include "../Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_tim.h"

int socket_receiver ( void );
double compute_std( void );
/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/
void _Error_Handler(char * file, int line);

/* USER CODE BEGIN 1 */
#define SOCKET_PORT 4444
//#define DEST_ADDR "192.168.0.10"
#define BUFLEN 512
#define NPACK 10
/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

/* USER CODE BEGIN 2 */
#define SAMPLE_SIZE 200
int16_t Sample_buffer[ SAMPLE_SIZE ];
uint16_t Sample_count = 0;
volatile uint16_t Current_us = 0;
volatile uint16_t Previous_us = 0;
uint8_t Is_first_trigger = 1;

double compute_std( void )
{
  uint32_t Sample_sum = 0;

  uint16_t i = 0;
  for( i = 0; i < SAMPLE_SIZE; i++ )
  {
    Sample_sum += Sample_buffer[ i ];
  }

  double Sample_average = (double)Sample_sum / SAMPLE_SIZE;

  //float* Sample_delta_sqr;
  double Sample_delta_sqr_sum = 0;

  for( i = 0; i < SAMPLE_SIZE; i++ )
  {
    Sample_delta_sqr_sum += ( (double)Sample_buffer[ i ] - Sample_average )
        * ( (double)Sample_buffer[ i ] - Sample_average );
  }
  double Result = sqrt( Sample_delta_sqr_sum / ( SAMPLE_SIZE - 1 ) );

  //double Result = 0;
  printf("%f\n", Result);
  return Result;
}

int socket_receiver ( void )
{
  struct sockaddr_in si_me, si_other;
  int s;
  //int slen = sizeof( si_other );
  char buf[BUFLEN];


  if( ( s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == -1 )
  {
    printf( "create socket error\n" );
    return -1;
  }

  memset( (char *) &si_me, 0, sizeof( si_me ) );
  bzero( &si_me, sizeof( si_me ) );

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons( SOCKET_PORT );
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  //si_me.sin_addr.s_addr = htonl(0xC0A80055);

  /*
  struct timeval timeV;
  timeV.tv_sec = 2;
  timeV.tv_usec = 0;

  if ( setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &timeV, sizeof( timeV ) ) == -1 )
  {
      return -1;
  }
*/
  struct sockaddr_in sockaddr;
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_len = sizeof(sockaddr);
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons( 4444 );
  sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );

  if ( bind( s, (struct sockaddr *)&sockaddr, sizeof( sockaddr ) ) == -1 )
  {
    printf( "bind error\n" );
    return -1;
  }

  struct sockaddr_in Receiveaddr;
  socklen_t Receivelen = sizeof( Receiveaddr );
  size_t Bufsize = 9216;
  void *Buffer = malloc( Bufsize );

  while ( 1 )
  {
     ssize_t n = recvfrom( s, Buffer, Bufsize, 0, (struct sockaddr *)&Receiveaddr, &Receivelen);
     //int n = recvfrom( s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen );
      //int n = recv( s, buf, BUFLEN, 0);
       //printf( "Received error %d\n", n );
       if ( n > 0 )
       {
         if ( strstr( (const char *)Buffer, "Phase_" ) )
         {
           if ( Is_first_trigger )
          {
            //Counter_0001s = 0;
            //HAL_TIM_Base_Start_IT( &htim1 );
            Previous_us = __HAL_TIM_GetCounter( &htim1 );
            Is_first_trigger = 0;
          } else
          {
            Current_us = __HAL_TIM_GetCounter( &htim1 );

            //Counter_0001s = 0;
            Sample_buffer[ Sample_count ] = (int16_t)( (int32_t)Current_us - (int32_t)Previous_us );
            //Sample_buffer[ Sample_count ] = __HAL_TIM_GetCounter( &htim1 );
            //Sample_buffer[ Sample_count ] = __HAL_TIM_GET_COUNTER( &htim1 );
            Previous_us = __HAL_TIM_GetCounter(&htim1);
            Sample_count++;
          }

          if ( Sample_count == SAMPLE_SIZE )
          {
            Sample_count = 0;
            compute_std();
          }
          HAL_GPIO_TogglePin( GPIOF, GPIO_PIN_14 );
         }
       } else
       {
         char* msg = (char*)"what?";
         sendto( s, msg, strlen( msg ), 0, ( struct sockaddr* ) &si_me, sizeof( si_me ) );
         free( Buffer );
       }
         /*
       if ( strstr( (const char *)buf, "Phase_" ) )
       {
         if ( Is_first_trigger )
          {
            //Counter_0001s = 0;
            //HAL_TIM_Base_Start_IT( &htim1 );
            Previous_us = __HAL_TIM_GetCounter( &htim1 );
            Is_first_trigger = 0;
          } else
          {
            Current_us = __HAL_TIM_GetCounter( &htim1 );

            //Counter_0001s = 0;
            Sample_buffer[ Sample_count ] = (int16_t)( (int32_t)Current_us - (int32_t)Previous_us );
            //Sample_buffer[ Sample_count ] = __HAL_TIM_GetCounter( &htim1 );
            //Sample_buffer[ Sample_count ] = __HAL_TIM_GET_COUNTER( &htim1 );
            Previous_us = __HAL_TIM_GetCounter(&htim1);
            Sample_count++;
          }

          if ( Sample_count == SAMPLE_SIZE )
          {
            Sample_count = 0;
            compute_std();
          }
         HAL_GPIO_TogglePin( GPIOF, GPIO_PIN_14 );
       } else
       {
         char* msg = (char*)"what?";
         size_t nBytes = sendto( s, msg, strlen( msg ), 0, ( struct sockaddr* ) &si_me, sizeof( si_me ) );
       }*/
       /*
       if ( buf != NULL )
       {
        printf( "Received packet from %s:%d\nData: %s\n\n",
        inet_ntoa( si_other.sin_addr ), ntohs( si_other.sin_port ), buf );
       }
       */
     }

}
/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
  IP_ADDRESS[0] = 192;
  IP_ADDRESS[1] = 168;
  IP_ADDRESS[2] = 0;
  IP_ADDRESS[3] = 84;
  NETMASK_ADDRESS[0] = 255;
  NETMASK_ADDRESS[1] = 255;
  NETMASK_ADDRESS[2] = 255;
  NETMASK_ADDRESS[3] = 0;
  GATEWAY_ADDRESS[0] = 192;
  GATEWAY_ADDRESS[1] = 168;
  GATEWAY_ADDRESS[2] = 0;
  GATEWAY_ADDRESS[3] = 1;
  
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

/* USER CODE BEGIN 3 */
  //while( !dhcp_supplied_address(&gnetif) );
  socket_receiver();
  //socket_transmitter();
/* USER CODE END 3 */
}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */
	
  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */	
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */	
  return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
