/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "lwip.h"
#include "lwip/apps/mqtt.h"
#include <string.h>
#include "tim.h"
#include "trigger.h"
#include "synthesizer_driver.h"
#include "hrtim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
ip4_addr_t Mcfly_broker_addr;
mqtt_client_t* Mcfly_broker;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t Trigger_flag = 0;
const char* Phase_idx;
QueueHandle_t Cell_sequence_q;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void example_do_connect(mqtt_client_t *client);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_sub_request_cb(void *arg, err_t result);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
void example_publish(mqtt_client_t *client, void *arg);
static void mqtt_pub_request_cb(void *arg, err_t result);

void MQTTTask(void const * argument);
void Synth_sequence_task( void const * argument );

void one_shot_delay_us( uint32_t Delay_us );
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

extern void MX_MBEDTLS_Init(void);
extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  IP4_ADDR( &Mcfly_broker_addr, 192, 168, 0, 10 );
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  //osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  //defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadDef(MQTT, MQTTTask, osPriorityNormal, 0, 128 * 4 );
  MQTTTaskHandle = osThreadCreate(osThread(MQTT), NULL);

  //osThreadDef(Synth_sequence, Synth_sequence_task, osPriorityNormal, 0, 128 * 4 );
  //SynthSequenceTaskHandle = osThreadCreate(osThread(Synth_sequence), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* Up to user define the empty MX_MBEDTLS_Init() function located in mbedtls.c file */
  MX_MBEDTLS_Init();

  /* MX_LWIP_Init() is generated within mbedtls_net_init() function in net_cockets.c file */
  /* Up to user to call mbedtls_net_init() function in MBEDTLS initialization step */

  /* USER CODE BEGIN StartDefaultTask */
  //MX_LWIP_Init();

  //IP4_ADDR( &client1IpAddr, 192, 168, 0, 10 );
  //mqtt_client_t client1;
  //example_do_connect( &client1 );
  //example_publish( &client1, NULL);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void MQTTTask(void const * argument)
{
  MX_LWIP_Init();

  Mcfly_broker = mqtt_client_new();
  if ( Mcfly_broker == NULL )
  {
    printf("MQTT client NULL.\n");
  }
  example_do_connect( Mcfly_broker );
  Cell_sequence_q = xQueueCreate( 1, sizeof( uint8_t ) );
  //HAL_HRTIM_SimpleBaseStart_IT( &hhrtim, HRTIM_TIMERINDEX_MASTER );
  /* Infinite loop */
  for(;;)
  {
    if ( Trigger_flag == 1 )
    {
      //HAL_TIM_Base_Start_IT( &htim1 );
      //vTaskResume( SynthSequenceTaskHandle );
      //vTaskSuspend( MQTTTaskHandle );

      int err = mqtt_publish(Mcfly_broker, "/Sensor/Freq", Phase_idx, strlen( Phase_idx ), 2, 0, mqtt_pub_request_cb, NULL);
      if(err != ERR_OK) {
        printf("Publish err: %d\n", err);
      }
      Trigger_flag = 0;
      example_do_connect( Mcfly_broker );
    }
    //example_publish( client1, NULL);
    //osDelay(5000);
  }
  /* USER CODE END StartDefaultTask */
}

void example_do_connect(mqtt_client_t *client)
{
  struct mqtt_connect_client_info_t ci;
  err_t err;

  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));

  /* Minimal amount of information required is client identifier, so set it here */
  ci.client_id = "lwip_test";

  /* Initiate client and connect to server, if this fails immediately an error code is returned
     otherwise mqtt_connection_cb will be called with connection result after attempting
     to establish a connection with the server.
     For now MQTT version 3.1.1 is always used */

  err = mqtt_client_connect(client, &Mcfly_broker_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);

  /* For now just print the result code if something goes wrong */
  while(err != ERR_OK) {
    printf("mqtt_connect return %d\n", err);
    err = mqtt_client_connect(client, &Mcfly_broker_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);
  }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  err_t err;
  if(status == MQTT_CONNECT_ACCEPTED) {
    printf("mqtt_connection_cb: Successfully connected\n");

    /* Setup callback for incoming publish requests */
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

    /* Subscribe to a topic named "/Sensor/Density/Trigger" with QoS level 0, call mqtt_sub_request_cb with result */
    err = mqtt_subscribe(client, "/Sensor/Density/Trigger", 0, mqtt_sub_request_cb, arg);

    if(err != ERR_OK) {
      printf("mqtt_subscribe return: %d\n", err);
    }
  } else {
    printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);

    /* Its more nice to be connected, so try to reconnect */
    example_do_connect(client);
  }
}

static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  printf("Subscribe result: %d\n", result);
}

static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int)tot_len);

  /* Decode topic string into a user defined reference */
  if(strcmp(topic, "/Sensor/Density/Trigger") == 0) {
    inpub_id = 0;
  } else if(topic[0] == 'A') {
    /* All topics starting with 'A' might be handled at the same way */
    inpub_id = 1;
  } else {
    /* For all other topics */
    inpub_id = 2;
  }
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
  printf("Incoming publish payload with length %d, flags %u\n", len, (unsigned int)flags);

  if(flags & MQTT_DATA_FLAG_LAST) {
    /* Last fragment of payload received (or whole part if payload fits receive buffer
       See MQTT_VAR_HEADER_BUFFER_LEN)  */

    /* Call function or do action depending on reference, in this case inpub_id */
    if(inpub_id == 0) {
      //Trigger_flag = 1;
      Phase_idx = (const char *)data;
      if ( strstr( (const char *)data, "Phase_" ) && strlen( (const char *)data ) == 7 )
      {
        uint8_t Phase_num = (uint8_t)( (uint8_t)Phase_idx[ 6 ] - (uint8_t)'0' );
        if ( Phase_num < NUM_PHASES )
        {
          Trigger_flag = 1;

          xQueueSend( Cell_sequence_q, ( void * ) &Trigger_flag, 0 );
          vTaskResume( SynthSequenceTaskHandle );
          vTaskSuspend( MQTTTaskHandle );
        }
      }

    } else if(inpub_id == 1) {
      /* Call an 'A' function... */
    } else {
      printf("mqtt_incoming_data_cb: Ignoring payload...\n");
      Trigger_flag = 0;
    }
  } else {
    /* Handle fragmented payload, store in buffer, write to file or whatever */
  }
}

void example_publish(mqtt_client_t *client, void *arg)
{
  const char *pub_payload= "Hello from stm32";
  err_t err;
  u8_t qos = 2; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  err = mqtt_publish(client, "pub_topic", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
}

/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result)
{
  if(result != ERR_OK) {
    printf("Publish result: %d\n", result);
  }
}

typedef enum {
    e_synth_state_Standby
  , e_synth_state_0_on
  , e_synth_state_1_on
} e_synth_state;

void Synth_sequence_task( void const * argument )
{
  static e_synth_state State = e_synth_state_Standby;
  static uint8_t Frequency_idx = 0;
  uint8_t Trigger;
  //uint32_t Delay = 80000000;

  for(;;)
  {
    switch ( State )
    {
      case e_synth_state_Standby:
      {
        if ( Cell_sequence_q != 0 )
        {
          if ( xQueuePeek( Cell_sequence_q, &( Trigger ), 0 ) )
          {
            State = e_synth_state_0_on;
            Frequency_idx = 0;
            set_synth_frequency( Frequencies_mhz[ Frequency_idx ], Synth_0_ss_id );
            HAL_HRTIM_SimpleBaseStart_IT( &hhrtim, HRTIM_TIMERINDEX_MASTER );
            //vQueueDelete( Cell_sequence_q );
          }
        } else
        {
          vTaskResume( MQTTTaskHandle );
          vTaskSuspend( SynthSequenceTaskHandle );
        }
      }
      break;
      case e_synth_state_0_on:
      {
        if ( HAL_GPIO_ReadPin( GPIOE, SYNTH0_LD_Pin ) == 1 )
        {
          HAL_GPIO_WritePin( GPIOE, SEL_SYNTH_0_Pin, SET );
          /*
          uint32_t i = 0;
          while ( i < Delay )
          {
            i++;
          }
          */
          Frequency_idx++;

          if ( Frequency_idx < NUM_FREQUENCIES )
          {
            set_synth_frequency( Frequencies_mhz[ Frequency_idx ], Synth_1_ss_id );

            State = e_synth_state_1_on;
          } else
          {
            one_shot_delay_us( 800000 );
            vTaskSuspend( SynthSequenceTaskHandle );

            State = e_synth_state_Standby;
            vTaskResume( MQTTTaskHandle );
            vTaskSuspend( SynthSequenceTaskHandle );
            HAL_HRTIM_SimpleBaseStop_IT( &hhrtim, HRTIM_TIMERINDEX_MASTER );
            break;
          }

          one_shot_delay_us( 800000 );
          vTaskSuspend( SynthSequenceTaskHandle );
        }
      }
      break;
      case e_synth_state_1_on:
      {
        if ( HAL_GPIO_ReadPin( GPIOE, SYNTH1_LD_Pin ) == 1 )
        {
          HAL_GPIO_WritePin( GPIOE, SEL_SYNTH_0_Pin, RESET );
          /*
          uint32_t i = 0;
          while ( i < Delay )
          {
            i++;
          }
          */
          Frequency_idx++;

          if ( Frequency_idx < NUM_FREQUENCIES )
          {
            set_synth_frequency( Frequencies_mhz[ Frequency_idx ], Synth_0_ss_id );

            State = e_synth_state_0_on;
          } else
          {
            one_shot_delay_us( 800000 );
            vTaskSuspend( SynthSequenceTaskHandle );

            State = e_synth_state_Standby;
            vTaskResume( MQTTTaskHandle );
            vTaskSuspend( SynthSequenceTaskHandle );
            HAL_HRTIM_SimpleBaseStop_IT( &hhrtim, HRTIM_TIMERINDEX_MASTER );
            break;
          }
          one_shot_delay_us( 800000 );
          vTaskSuspend( SynthSequenceTaskHandle );

        }
      }
    }
  }
}

void one_shot_delay_us( uint32_t Delay_us )
{
  while ( Is_counting );
  Counter_us = Delay_us;
  Is_counting = true;
  //HAL_HRTIM_SimpleBaseStart_IT( &hhrtim, HRTIM_TIMERINDEX_MASTER );
}
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
