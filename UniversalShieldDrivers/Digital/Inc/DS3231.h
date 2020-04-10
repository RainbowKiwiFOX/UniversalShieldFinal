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

/* Маски моментов срабатывания будильников */
enum alarm1Rates {
	A1_oncePerSecond 	= 0b01111,	//Срабатывание раз в секунду
	A1_whenSec 	 		 	= 0b01110,	//Срабатывание по совпадению секунд
	A1_whenSecMin			= 0b01100,	//Срабатывание по совпадению минут, секунд
	A1_whenHMS				= 0b01000,	//Срабатывание по совпадению часов, минут, секунд 
	A1_whenDHMS				= 0b00000,	//Срабатывание по совпадению дня месяца, часов, минут, секунд
	A1_whenWdHMS			=	0b10000		//Срабатывание по совпадению дню недели, часов, минут, секунд
};
enum alarm2Rates {
	A2_oncePerSecond 	= 0b0111,		//Срабатывание раз в минуту
	A2_whenMin 	 		 	= 0b0110,		//Срабатывание по совпадению минут
	A2_whenHourMin		= 0b0100,		//Срабатывание по совпадению часов и минут
	A2_whenDHM				= 0b0000,		//Срабатывание по совпадению дня месяца, часов, минут
	A2_whenWdHM				= 0b1000		//Срабатывание по совпадению дню недели, часов, минут
};

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
//Установка момента срабатывания будильника 1
void RTC_alarm1Set(RTC_time time, RTC_date date, uint8_t alarmRate);
//Установка момента срабатывания будильника 2
void RTC_alarm2Set(RTC_time time, RTC_date date, uint8_t alarmRate);
//Включение будильника 1
void RTC_alarm1On(void);
//Выключение будильника 1
void RTC_alarm1Off(void);
//Включение будильника 2
void RTC_alarm2On(void);
//Выключение будильника 2
void RTC_alarm2Off(void);
//Проверка срабатывания будильника 1
uint8_t RTC_alarm1IsBell(void);
//Проверка срабатывания будильника 2
uint8_t RTC_alarm2IsBell(void);
//Сброс срабатывания будильника 1
void RTC_alarm1BellReset(void);
//Сброс срабатывания будильника 2
void RTC_alarm2BellReset(void);
// Проверка работы будильника 1
uint8_t RTC_alarm1IsActive(void);
// Проверка работы будильника 2
uint8_t RTC_alarm2IsActive(void);
#endif
