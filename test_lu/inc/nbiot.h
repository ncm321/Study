/*
 * nbiot.h
 *
 *  Created on: 2024年7月26日
 *      Author: 杨璐
 */

#ifndef INC_NBIOT_H_
#define INC_NBIOT_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "sht20.h"
#include "logger.h"
#include "usart.h"
#include "at_cmd.h"
#include "at-bc28.h"
#include "leds.h"
#define    SERIAL_DEVNAME    "/dev/ttyUSB0"
#define    DATA_SIZE         512
extern enum status_s{
	STATUS_INIT,
	STATUS_PRESEND,
	STATUS_CONFIG,
	STATUS_READY,
}status_t;

typedef struct nb_config_s{
	comport_t     comport;
	int           current_state;
}nb_config_t;

extern pthread_mutex_t   state_mutex;


void *state_machine_thread(void *arg);
void  float_to_hex_string(float f, char hexStr[9]);
void *report_data(void *arg);
void *receive_data(void *arg);
void *asyn_process_leds(void *arg);
void *process_report(void *arg);
int  Linux_Create();
static inline void msleep(unsigned long ms);

#endif /* INC_NBIOT_H_ */
