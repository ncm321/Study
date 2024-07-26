/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/

#define CONFIG_OS_LINUX
//#define CONFIG_OS_STM32
#include "usart.h"

#ifdef CONFIG_OS_STM32
/* USER CODE BEGIN 0 */
uint8_t					data;
StreamBufferHandle_t	xStreamBuffer;
comport_t				comport;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  HAL_UART_Receive_IT(&huart3, &data, 1);
  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return ch;
}
#endif

#ifdef CONFIG_OS_LINUX
#define CONFIG_PRINT_STDOUT

#if ( defined CONFIG_PRINT_LOGGER )
#include "logger.h"
#define dbg_print(format,args...) log_error(format, ##args)

#elif ( defined CONFIG_PRINT_STDOUT )
#define dbg_print(format,args...) printf(format, ##args)

#else
#define dbg_print(format,args...) do{} while(0);
#endif
#endif

int comport_open(comport_t *comport, void *devname, long baudrate, const char *settings)
{
#ifdef CONFIG_OS_STM32
	if(!comport)
	{
		printf("Input invalid argument\r\n");
		return -1;
	}
   comport->dev = devname;
   printf("Open uart ok\r\n");
   return 0;
#else
   int                         rv = -1;
   	struct termios              old_cfg, new_cfg;
   	int                         old_flags;
   	long                        tmp;

   	if( !comport || !devname )
   	{
   		dbg_print("invalid input arugments\n");
   		return -1;
   	}

   	/* --------------Init comport------------- */
   	memset(comport, 0, sizeof(*comport));
   	strncpy(comport->devname, devname, sizeof(comport->devname));
   	comport->baudrate = baudrate;
   	comport->dev = -1;
   	comport->fragsize = CONFIG_DEF_FRAGSIZE;
   	set_settings(comport, settings);

   	/* -------------Open comport-------------- */
   	if( !strstr(comport->devname, "tty") )
   	{
   		dbg_print("comport device \"%s\" is not tty device\n", comport->devname);
   		return -2;
   	}

   	comport->dev = open(comport->devname, O_RDWR | O_NOCTTY | O_NONBLOCK);
   	if( comport->dev<0 )
   	{
   		dbg_print("comport open \"%s\" failed:%s\n", comport->devname, strerror(errno));
   		return -3;
   	}

   	if(   (-1 != (old_flags = fcntl(comport->dev, F_GETFL, 0)))
   			&& (-1 != fcntl(comport->dev, F_SETFL, old_flags & ~O_NONBLOCK)) )
   	{
   		/*  Flush input and output */
   		tcflush(comport->dev, TCIOFLUSH);
   	}
   	else
   	{
   		rv = -4;
   		goto cleanup;
   	}

   	if (0 != tcgetattr(comport->dev, &old_cfg))
   	{
   		rv = -5;
   		goto cleanup;
   	}
   	/* -------------configure comport---------------- */
   	memset(&new_cfg, 0, sizeof(new_cfg));
   	new_cfg.c_cflag &= ~CSIZE;
   	new_cfg.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
   	new_cfg.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
   	new_cfg.c_oflag &= ~(OPOST);

   	/* --------------set databit------------- */
   	switch (comport->databit)
   	{
   		case 0x07:
   			new_cfg.c_cflag |= CS7;
   			break;
   		case 0x06:
   			new_cfg.c_cflag |= CS6;
   			break;
   		case 0x05:
   			new_cfg.c_cflag |= CS5;
   			break;
   		default:
   			new_cfg.c_cflag |= CS8;
   			break;
   	}

   	/* --------------set parity------------- */
   	switch (comport->parity)
   	{
   		case 0x01:
   			new_cfg.c_cflag |= (PARENB | PARODD);
   			new_cfg.c_cflag |= (INPCK | ISTRIP);
   			break;
   		case 0x02:
   			new_cfg.c_cflag |= PARENB;
   			new_cfg.c_cflag &= ~PARODD;;
   			new_cfg.c_cflag |= (INPCK | ISTRIP);
   			break;
   		case 0x03:
   			new_cfg.c_cflag &= ~PARENB;
   			new_cfg.c_cflag &= ~CSTOPB;
   			break;
   		default:
   			new_cfg.c_cflag &= ~PARENB;
   	}

   	/* --------------set stopbit------------- */
   	if (0x01 != comport->stopbit)
   	{
   		new_cfg.c_cflag |= CSTOPB;
   	}
   	else
   	{
   		new_cfg.c_cflag &= ~CSTOPB;
   	}

   	/* ------------set conctrl--------------- */
   	switch (comport->flowctrl)
   	{
   		case 1:
   		case 3:
   			new_cfg.c_cflag &= ~(CRTSCTS);
   			new_cfg.c_iflag |= (IXON | IXOFF);
   			break;
   		case 2:
   			new_cfg.c_cflag |= CRTSCTS;
   			new_cfg.c_iflag &= ~(IXON | IXOFF);
   			break;
   		default:
   			new_cfg.c_cflag &= ~(CRTSCTS);
   			new_cfg.c_iflag &= ~(IXON | IXOFF);
   			break;
   	}

   	/*--------------set baudrate-------------  */
   	switch (comport->baudrate)
   	{
   		case 4000000:
   			tmp = B4000000;
   			break;
   		case 3500000:
   			tmp = B3500000;
   			break;
   		case 3000000:
   			tmp = B3000000;
   			break;
   		case 2500000:
   			tmp = B2500000;
   			break;
   		case 2000000:
   			tmp = B2000000;
   			break;
   		case 1500000:
   			tmp = B1500000;
   			break;
   		case 115200:
   			tmp = B115200;
   			break;
   		case 9600:
   			tmp = B9600;
   			break;
   		default:
   			tmp = B115200;

   	}
   	cfsetispeed(&new_cfg, tmp);
   	cfsetospeed(&new_cfg, tmp);

   	/* ------------set comport timeout settings----------- */
   	new_cfg.c_cc[VMIN] = 0;
   	new_cfg.c_cc[VTIME] = 0;

   	tcflush(comport->dev, TCIFLUSH);
   	if (0 != tcsetattr(comport->dev, TCSANOW, &new_cfg))
   	{
   		rv = -6;
   		goto cleanup;
   	}

   	rv = comport->dev;
   cleanup:
   	return rv;
#endif
}

void comport_close(comport_t *comport)
{
#ifdef CONFIG_OS_STM32
#else
	if( !comport )
		{
			dbg_print("invalid input arugments\n");
			return ;
		}

		if ( comport->dev >= 0 )
		{
			close(comport->dev);
		}

		comport->dev = -1;
		return ;
#endif
}

int comport_send(comport_t *comport, char *data, int bytes)
{
	if(!comport || !data || bytes<=0)
	{
		printf("Invalid input arguments\n");
		return -1;
	}
#ifdef	CONFIG_OS_STM32

	if( HAL_UART_Transmit(comport->dev, (uint8_t *)data, bytes,0xFFFF) != HAL_OK)
		return -1;
	else
		return 0;

#else
	char                       *ptr;
	int                         left = 0;
	int                         rv = 0;

	if( comport->dev < 0 )
	{
		dbg_print("Serail port not opened\n");
		return -2;
	}

	tcflush( comport->dev, TCIFLUSH);

	ptr = data;
	left = bytes;

	while( left > 0 )
	{

		bytes = left>comport->fragsize ? comport->fragsize : left;

		rv = write(comport->dev, ptr, bytes);
		if( rv<0 )
		{
			rv = -3;
			break;
		}

		left -= rv;
		ptr += rv;
	}

	return rv;
#endif
}

int comport_recv(comport_t *comport, char *buf, int bytes, uint32_t timeout)
{
	if(!comport || !buf || bytes<=0)
	{
		printf("Invalid input argument\n");
		return -1;
	}

#ifdef	CONFIG_OS_STM32

	xStreamBufferReceive(xStreamBuffer,buf,bytes,pdMS_TO_TICKS(timeout));

	return 0;
#else
	fd_set                      rdfds, exfds;
	struct timeval              to, *to_ptr = NULL;
	int                         ret, rv = 0;
	int                         rec_bytes = 0;

	if ( comport->dev < 0 )
	{
		dbg_print("Serail port not opened\n");
		return -2;
	}

	memset(buf, 0, REC_SIZE);

	FD_ZERO(&rdfds);
	FD_ZERO(&exfds);
	FD_SET(comport->dev, &rdfds);
	FD_SET(comport->dev, &exfds);

	if( TIMEOUT != timeout )
	{
		to.tv_sec = (time_t) (timeout / 1000);
		to.tv_usec = (long)(1000 * (timeout % 1000));
		to_ptr = &to;
	}

	while( 1 )
	{

		ret = select(comport->dev+1, &rdfds, 0, &exfds, to_ptr);
		if( ret<0 )
		{
			dbg_print("comport select() failed: %s\n", strerror(errno));
			rv = EINTR==errno ? 0 : -3;
			break;
		}
		else if( 0 == ret )
		{
			break;
		}

		ret = read(comport->dev, buf+rec_bytes, REC_SIZE-rec_bytes);
		if(ret <= 0)
		{
			dbg_print("comport read() failed: %s\n", strerror(errno));
			break;
		}

		rec_bytes += ret;
		if( rec_bytes >= REC_SIZE )
			break;

		to.tv_sec = 0;
		to.tv_usec = 10000;
		to_ptr = &to;
	}

	if( !rv )
		rv = rec_bytes;

	return rv;
#endif
}

#ifdef CONFIG_OS_LINUX

void set_settings(comport_t * comport, const char *settings)
{
	if( !settings || !comport )
	{
		dbg_print("invalid input arugments\n");
		return ;
	}

	switch (settings[0])
	{
		case '7':
			comport->databit = 7;
			break;
		case '8':
		default:
			comport->databit = 8;
			break;
	}

	switch (settings[1])
	{
		case 'O':
		case 'o':
			comport->parity = 1;
			break;
		case 'E':
		case 'e':
			comport->parity = 2;
			break;
		case 'S':
		case 's':
			comport->parity = 3;
			break;
		case 'N':
		case 'n':
		default:
			comport->parity = 0;
			break;
	}

	switch (settings[2])
	{
		case '0':
			comport->stopbit = 0;
			break;
		case '1':
		default:
			comport->stopbit = 1;
			break;
	}

	switch (settings[3])
	{
		case 'S':
		case 's':
			comport->flowctrl = 1;
			break;
		case 'H':
		case 'h':
			comport->flowctrl = 2;
			break;
		case 'B':
		case 'b':
			comport->flowctrl = 3;
			break;
		case 'N':
		case 'n':
		default:
			comport->flowctrl = 0;
			break;
	}
}

#endif
/* USER CODE END 1 */
