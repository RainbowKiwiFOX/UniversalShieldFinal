#ifndef __DS3231_H
#define __DS3231_H

#include "main.h"

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
} RTC_time;

typedef struct {
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t weekday;
} RTC_date;

void RTC_init(I2C_HandleTypeDef *i2c);
RTC_time RTC_getTime(void);
RTC_date RTC_getDate(void);
void RTC_setTime(RTC_time time);
void RTC_setDate(RTC_date date);
#endif