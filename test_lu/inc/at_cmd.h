/*
 * at_cmd.h
 *
 *  Created on: 2024年7月14日
 *      Author: 杨璐
 */

#ifndef INC_AT_CMD_H_
#define INC_AT_CMD_H_

#define CONFIG_OS_LINUX
//#define CONFIG_OS_STM32

#include <string.h>
#ifdef CONFIG_OS_STM32
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "event_groups.h"
#include "stream_buffer.h"
#include "semphr.h"
#include "usart.h"
#elif (defined CONFIG_OS_LINUX)
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include "usart.h"
#include "logger.h"
#endif

#define REPORT_SIZE 	256
#define RECEIVE_SIZE	256
#define ATCMD_SIZE          256
#define ATBUF_SIZE          256
#define ATCMD_REPLY_LEN     256

typedef struct atcmd_s
{
	char		xAtCmd[REPORT_SIZE];
	char		xAtCmdReply[RECEIVE_SIZE];
	char		Asyn_Buf[RECEIVE_SIZE];
	char		AtReply_Buf[RECEIVE_SIZE];
}atcmd_t;

extern atcmd_t	g_atcmd;

#define AT_OKSTR	"\r\nOK\r\n"
#define AT_ERRSTR	"\r\nERROR\r\n"
#define AT_SUFFIX	"\r\n"

#ifdef CONFIG_OS_STM32
#define Receive_EVENT		(0x01<<0)
#define Send_EVENT			(0x01<<1)
#define STREAM_BUFFER_SIZE 	1024
#define	TRIGGER_LEVEL      	0
extern EventGroupHandle_t	Event_Handle;
extern SemaphoreHandle_t	xSemaphore;
#elif (defined CONFIG_OS_LINUX)

enum
{
	ATRES_ERROR,
	ATRES_EXPECT,
	ATRES_TIMEOUT,
};

extern int       SEND_EVENT_G;
extern int       LEDS_EVENT_G;

#endif

extern int atcmd_pars(char *buf);
extern int atcmd_send(comport_t *comport, char *at, uint32_t timeout,char *expect, char *error,char *reply,size_t size);
extern int atcmd_check_OK(comport_t *comport, char *at,uint32_t timeout);
extern int atcmd_check_value(comport_t *comport, char *at, uint32_t timeout,char *reply,size_t size);
extern int parser_async_message(char *buf,char *keystr);
extern int send_atcmd_request(comport_t *comport, char *at, unsigned long timeout, char *reply, int size);
#endif /* INC_AT_CMD_H_ */
