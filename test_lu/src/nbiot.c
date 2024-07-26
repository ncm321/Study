/*
 * nbiot.c
 *
 *  Created on: 2024年7月26日
 *      Author: 杨璐
 */
#define CONFIG_OS_LINUX
//#define CONFIG_OS_STM32

#ifdef CONFIG_OS_LINUX

#include "nbiot.h"

static inline void msleep(unsigned long ms)
{
	struct timespec cSleep;
	unsigned long   ulTmp;

	cSleep.tv_sec = ms / 1000;
	if( cSleep.tv_sec == 0)
	{
		ulTmp = ms * 10000;
		cSleep.tv_nsec = ulTmp * 100;
	}
	else
	{
		cSleep.tv_nsec = 0;
	}

	nanosleep(&cSleep, 0);
}
#if 0
void init_mutex() {
	if (pthread_mutex_init(&comport_mutex, NULL) != 0) {
		printf("Mutex init failed\n");
		exit(EXIT_FAILURE);
	}
}
#endif


void *state_machine_thread(void *arg)
{
	nb_config_t*     nbiot_data = (nb_config_t*)arg;
	int              rv;

	nbiot_data->current_state = STATUS_INIT;

	while(1)
	{
		switch(nbiot_data->current_state)
		{
			case STATUS_INIT:
				rv = atcmd_check_OK(&nbiot_data->comport, "AT", 500);
				if( rv<0 )
				{
					printf ("debug:STATUS_INIT failed.\n");
					break;
				}
				else
				{
					nbiot_data->current_state++;
					break;
				}
			case STATUS_PRESEND:
				rv = NB_HDW_OK(&nbiot_data->comport);
				if( rv<0 )
				{
					printf ("debug:STATUS_PRESEND failed.\n");
					break;
				}
				else
				{
					nbiot_data->current_state++;
					break;
				}
			case STATUS_CONFIG:
				rv = NB_CONF_OK(&nbiot_data->comport);
				if( rv<0 )
				{
					printf ("debug : STATUS_CONFIG failed.\n");
					break;
				}
				else
				{
					nbiot_data->current_state++;
					break;
				}
			case STATUS_READY:

				printf ("READY!!!!!!\n");
				sleep(30);
				break;
			default:
				break;
		}
	}

	return NULL;
}


void *report_data(void *arg)
{
	nb_config_t*      nbiot_data = (nb_config_t*)arg;
	int               rv;
	char              bufferW[DATA_SIZE];


	printf ("test state:%d\n", nbiot_data->current_state);

	while(1)
	{
		if(nbiot_data->current_state != STATUS_READY)
		{
			msleep(10);
			continue;
		}

		sht2x_get_temp_rh(bufferW, DATA_SIZE);
	//	pthread_mutex_lock(&comport_mutex);
		rv = comport_send(&nbiot_data->comport, bufferW, DATA_SIZE );
	//	pthread_mutex_unlock(&comport_mutex);
		if( rv<0 )
		{
			printf ("Send AT data failure.\n");
		}
		else
			printf ("Send successful:%s\n",bufferW);
		sleep(10);
	}
}


void *receive_data(void *arg)
{
	nb_config_t*      nbiot_data = (nb_config_t*)arg;
	int               rv;
	int               bytes = 0;
	fd_set            read_fds;
	char              value[256];
	char              bufferR[1024];
	comport_t         comport;
	int               timeout_ms = 10000;
	struct timeval    timeout;

	comport = nbiot_data->comport;


	while(1)
	{

		if(nbiot_data->current_state != STATUS_READY)
		{
			msleep(10);
			continue;
		}

		memset(bufferR, 0, sizeof(bufferR));
		memset(value, 0, sizeof(value));
		//		printf ("Receiveing data...\n");


		FD_ZERO(&read_fds);
		FD_SET(comport.dev, &read_fds);

		timeout.tv_sec = timeout_ms/1000;
		timeout.tv_usec = (timeout_ms % 1000) * 1000;
		//		printf ("debug:comport->fd:%d\n",comport.dev);
		rv =select((comport.dev)+1, &read_fds, NULL, NULL, &timeout);
		if( rv<0 )
		{
			printf ("Select error\n");
		}
		else if( rv==0 )
		{
			printf ("timeuot\n");
		}
		else
		{
			if( FD_ISSET(comport.dev, &read_fds))
			{
				bytes = comport_recv(&comport, bufferR, sizeof(bufferR), 100);

				printf ("debug:test:received buffer:%s\n",bufferR);
				if( rv<0 )
				{
					printf ("Read failure.\n");
				}
				bufferR[bytes] = '\0';

				if( strstr(bufferR, "+NNMI:") )
				{
					memset(g_atcmd.Asyn_Buf, 0, sizeof(g_atcmd.Asyn_Buf));
					//复制接收到的内容，让处理LED线程去执行
					strcpy(g_atcmd.Asyn_Buf, bufferR);
					LEDS_EVENT_G = 1;
					printf ("debug:led parser:%d\n",LEDS_EVENT_G);
				}
				else
				{
					memset(g_atcmd.AtReply_Buf, 0, sizeof(g_atcmd.AtReply_Buf));
					//复制接收到的内容，让处理AT命令的回复的线程去执行
					strcpy(g_atcmd.AtReply_Buf, bufferR);
					if(nbiot_data->current_state == STATUS_READY)
					{
						SEND_EVENT_G = 2;
					}
					else
						SEND_EVENT_G = 1;
				}

			}
		}
	}
}

void *asyn_process_leds(void *arg)
{
	int               rv;
	leds_t            leds =
	{
		.leds = leds_info,
		.count = LED_MAX,

	};

	if( (rv=init_led(&leds))<0 )
	{
		printf ("initial leds gpio failure,rv=%d\n",rv);
	}
	printf ("进入解析\n");
	while(1)
	{
		if(LEDS_EVENT_G ==1)
		{
			printf ("test:Z进循环\n");
			if (strstr(g_atcmd.Asyn_Buf, "0101"))
			{
				printf ("Turning LED on\n");
				open_led(&leds, LED_R);
			}
			else if(strstr(g_atcmd.Asyn_Buf, "0100"))
			{
				printf ("Turning led Off\n");
				close_led(&leds, LED_R);
			}
		}
		else
		{
			continue;
		}
		LEDS_EVENT_G = 0;
	}
}


void *process_report(void *arg)
{

	int           rv;

	while( 1 )
	{
		if( SEND_EVENT_G==1 )
		{
			if( strstr(g_atcmd.AtReply_Buf, "OK") )
			{
				printf("Receive:%s",g_atcmd.AtReply_Buf);
			}
		}
		else
		{
			continue;
		}
		SEND_EVENT_G = 0;
	}
}

int Linux_Create()
{
	pthread_t      report_thread,receive_thread,state_thread,leds_process_thread,send_process_thread;
	const char    *dev = "/dev/ttyUSB0";
	int            rv;
	nb_config_t    nbiot_data;
	comport_t      comport;


	rv = comport_open(&nbiot_data.comport, SERIAL_DEVNAME, 9600, "8n1n");
	if( rv<0 )
	{
		perror("Failed to open comport");
		return -1;
	}
	nbiot_data.current_state = 0;


	if(pthread_create(&state_thread, NULL, state_machine_thread, &nbiot_data) != 0)
	{
		perror("Failed to create state thread");
		return -2;
	}
	if( pthread_create(&report_thread, NULL, report_data,
				&nbiot_data) != 0){
		perror("Failed to create report thread");
		return -3;
	}

	if ( pthread_create(&receive_thread, NULL, receive_data,
				&nbiot_data) !=0 )
	{
		perror("Failed to create receive thread");
		return -4;
	}

	if ( pthread_create(&leds_process_thread, NULL, asyn_process_leds,
				&nbiot_data) !=0 )
	{
		perror("Failed to create receive thread");
		return -5;
	}
	if ( pthread_create(&send_process_thread, NULL, process_report,
				&nbiot_data) !=0 )
	{
		perror("Failed to create receive led_process thread");
		return -6;
	}


	pthread_join(state_thread, NULL);
	pthread_join(report_thread, NULL);
	pthread_join(receive_thread, NULL);
	pthread_join(leds_process_thread, NULL);
	pthread_join(send_process_thread, NULL);

	comport_close(&nbiot_data.comport);


	return 0;
}

#endif
