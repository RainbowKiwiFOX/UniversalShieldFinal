#ifndef __DS3231_H
#define __DS3231_H

#include "main.h"
/* Структуры времени и даты */
typedef struct {
	uint8_t sec;		//Секунды
	uint8_t min;		//Минуты
	uint8_t hour;		//Часы
} RTC_time;

typedef struct {
	uint8_t day;		//Число месяца
	uint8_t month;	//Месяц
	uint8_t year;		//Год
	uint8_t weekday;//День недели
} RTC_date;

/* Прототипы функций */
//Инициализация RTC
void RTC_init(I2C_HandleTypeDef *i2c);
//Получение текущего времени из RTC
RTC_time RTC_getTime(void);
//Получение даты из RTC
RTC_date RTC_getDate(void);
//Установка времени в RTC
void RTC_setTime(RTC_time time);
//Установка даты в RTC
void RTC_setDate(RTC_date date);
#endif
