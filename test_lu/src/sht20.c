/*********************************************************************************
 *      Copyright:  (C) 2024 niuchunmin 
 *                  All rights reserved.
 *
 *       Filename:  sht20.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(06/07/24)
 *         Author:  niuchunmin<2430815397@qq.com>
 *      ChangeLog:  1, Release initial version on "06/07/24 15:08:07"
 *                 
 ********************************************************************************/

#include "sht20.h"


static inline void msleep(unsigned long ms);
static inline void dump_buf(const char *prompt, uint8_t *buf, int size);
int    sht2x_init(const char *i2c_dev);
int    sht2x_get_serialnumber(int fd, uint8_t *serialnumber, int size);
int    sht2x_get_temp_humi(const char *devname, float *temp, float *rh);

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

/* 打印buf */
static inline void dump_buf(const char *prompt, uint8_t *buf, int size)
{
	int i;

	if( !buf )
	{
		return ;
	}

	if( prompt )
	{

		printf ("%s", prompt);
	}

	for(i=0; i<size; i++)
	{
		printf("%02x", buf[i]);
	}
	printf ("\n");

	return ;
}

/* sht20 reset cmd */
int sht2x_softreset(int fd)
{
	uint8_t    buf[4];

	if( fd<0 )
	{
		printf ("%s line [%d] %s() get invalid input arguments\n", __FILE__, 
				__LINE__, __func__);
		return -1;
	}

	/* software reset SHT2x */
	memset(buf, 0, sizeof(buf));

	buf[0] = SOFTRESET;
	write(fd, buf, 1);

	msleep(50);

	return 0;
}

/* sht20 init 设备新参为I2C总线节点 */
int sht2x_init(const char *i2c_dev)
{
	int     fd;

	if( (fd = open(i2c_dev, O_RDWR)) < 0 )
	{

		printf ("i2c device open failed:%s\n", strerror(errno));
		return -1;
	}

	/* set I2C mode and SHT2x slave address */
	ioctl(fd, I2C_TENBIT, 0);
	ioctl(fd, I2C_SLAVE, 0x40);

	if( sht2x_softreset(fd) < 0 )
	{

		printf ("SHT20 softreset failure\n");
		return -2;
	}

	return fd;
}

int sht2x_get_serialnumber(int fd, uint8_t *serialnumber, int size)
{
	uint8_t          buf[4];

	if( fd<0 || !serialnumber || size != 8)
	{   

		printf ("%s line [%d] %s() get incalid input arguments\n",     __FILE__,
				__LINE__, __func__);
		return -1; 
	}   

	/*  Read SerialNumber from location 1 */
	memset(buf, 0, sizeof(buf));
	buf[0] = 0xfa;
	buf[1] = 0x0f;
	write(fd, buf, 2); 

	memset(buf, 0, sizeof(buf));
	read(fd, buf, 4); 

	serialnumber[5] = buf[0];
	serialnumber[4] = buf[1];
	serialnumber[3] = buf[2];
	serialnumber[2] = buf[3];

	/*  Read SerialNumber from location 2 */
	memset(buf, 0, sizeof(buf));
	buf[0] = 0xfc;
	buf[1] = 0xc9;
	write(fd, buf, 2); 

	memset(buf, 0, sizeof(buf));
	serialnumber[1] = buf[0];
	serialnumber[0] = buf[1];

	dump_buf("SHT2x Serial number:", serialnumber, 8); 

	return 0;

}



/* get temp and rh */
int  sht2x_get_temp_humi(const char *devname, float *temp, float *rh)
{
	uint8_t     buf[4];
	int         fd;
	uint8_t     serialnumber[8];

	if ( !devname )
	{

		printf ("Invaild arguments\n");
		return -1;
	}

	fd = sht2x_init(devname);
	if( fd<0 )
	{

		printf ("SHT2X initialize failure\n");
		return -2;
	}

	if ( sht2x_get_serialnumber(fd, serialnumber, 8) < 0 )
	{
		printf("SHT2x get serial number failure\n");
		return 2;
	}


	if( fd<0 || !temp || !rh)
	{

		printf ("%s line [%d] %s() get incalid input arguments\n",__FILE__,
				__LINE__, __func__);
		return -1;
	}

	/* send trigger temperature measure cmd and read the data */
	memset(buf, 0, sizeof(buf));

	buf[0] = TRIGGER_TEMPERATURE_NO_HOLD;
	write(fd, buf, 1);

	msleep(85);

	memset(buf, 0, sizeof(buf));
	/* 读前三个字节的数据 最后一个字节为crc检验值 */
	read(fd, buf, 3);
	dump_buf("Temperature sample data: ", buf, 3);

	*temp = 175.72 * (((((int)buf[0]) << 8) + (buf[1] & 0xFC)) / 65536.0) - 46.85;

	/* send trigger humidity cmd and read data*/
	memset(buf, 0, sizeof(buf));

	buf[0] = TRIGGER_HUMIDITY_NO_HOLD;
	write(fd, buf, 1);

	msleep(29);
	memset(buf, 0, sizeof(buf));

	read(fd, buf, 3);
	*rh = 125*(((((int)buf[0]) << 8) + (buf[1] & 0xfc)) / 65536.0) - 6;

	return 0;
}

void float_to_hex_string(float f, char hexStr[9]) {
	unsigned char *bytePtr = (unsigned char*)&f;
	sprintf(hexStr, "%02X%02X%02X%02X", bytePtr[3], bytePtr[2], bytePtr[1],     bytePtr[0]);
}

int sht2x_get_temp_rh(char *buf, int size)
{
	int             rv;
	float           temp;
	float           humi;
	char            hex_str_temp[9];
	char            hex_str_humi[9];

	memset(hex_str_temp, 0, sizeof(hex_str_temp));
	memset(hex_str_humi, 0, sizeof(hex_str_humi));

	rv = sht2x_get_temp_humi(SHT2X_DEVNAME, &temp, &humi);
	if( rv<0 )
	{
		log_error("SHT2X get temperature and humidity failure\n");
		return -1;
	}

	float_to_hex_string(temp, hex_str_temp);
	float_to_hex_string(humi, hex_str_humi);

	snprintf(buf, size, "AT+QLWULDATAEX=13,0200250008%s%s,0x0100\r\n", hex_str_temp, hex_str_humi);

	return 0;
}
