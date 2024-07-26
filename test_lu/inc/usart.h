/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

//#define CONFIG_OS_STM32
#define CONFIG_OS_LINUX

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#ifdef CONFIG_OS_STM32
#include "main.h"
#endif

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#ifdef CONFIG_OS_STM32
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "stream_buffer.h"
#elif (defined CONFIG_OS_LINUX)
#include  <stdlib.h>
#include  <unistd.h>
#include  <getopt.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <termios.h>
#include  <sys/stat.h>
#include  <sys/wait.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/select.h>
#include  <pthread.h>
#include  <stdint.h>
#endif
/* USER CODE END Includes */

#ifdef CONFIG_OS_STM32
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);
#endif

/* USER CODE BEGIN Private defines */

#ifdef CONFIG_OS_LINUX
typedef int						UART_Handle_t;
#elif (defined CONFIG_OS_STM32)
typedef UART_HandleTypeDef *	UART_Handle_t;
#endif

typedef struct comport_s
{
	UART_Handle_t 		 dev;
    char              devname[32];
	long              baudrate;
	unsigned char     databit,parity,stopbit,flowctrl;
	int               fragsize;
}comport_t;

#ifdef CONFIG_OS_STM32
extern comport_t 				comport;
extern uint8_t					data;
extern StreamBufferHandle_t		xStreamBuffer;
#elif (defined CONFIG_OS_LINUX)
#define   CONFIG_DEF_FRAGSIZE   128
#define   REC_SIZE              256
#define   TIMEOUT      			5
extern    pthread_mutex_t     	comport_mutex;
#endif

int comport_open(comport_t *comport, void *devname, long baudrate, const char *settings);

int comport_send(comport_t *comport, char *data, int bytes);

int comport_recv(comport_t *comport, char *buf, int bytes, uint32_t timeout);

void comport_close(comport_t *comport);
void set_settings(comport_t * comport, const char *settings);

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

