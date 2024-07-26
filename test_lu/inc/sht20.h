/********************************************************************************
 *      Copyright:  (C) 2024 niuchunmin 
 *                  All rights reserved.
 *
 *       Filename:  sht20.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(16/07/24)
 *         Author:  Niu Chunmin <niuchunmin@2430815397@qq.com>
 *      ChangeLog:  1, Release initial version on "16/07/24 19:10:25"
 *                 
 ********************************************************************************/

#ifndef _SHT20_H_
#define _SHT20_H_


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include "logger.h"

#define   SOFTRESET                    0xFE
#define   TRIGGER_TEMPERATURE_NO_HOLD  0xF3
#define   TRIGGER_HUMIDITY_NO_HOLD     0xF5
#define   SHT2X_DEVNAME                "/dev/i2c-0"

static inline void msleep(unsigned long ms);
static inline void dump_buf(const char *prompt, uint8_t *buf, int size);
extern int    sht2x_init(const char *i2c_dev);
extern int    sht2x_get_serialnumber(int fd, uint8_t *serialnumber, int size);
extern int    sht2x_get_temp_humi(const char *devname, float *temp, float *rh);
extern int    sht2x_get_temp_rh(char *buf, int size);


#endif
