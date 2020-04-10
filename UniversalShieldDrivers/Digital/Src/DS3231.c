#include "DS3231.h"

//Интерфейс I2C, по которому будет происходить обмен данными
I2C_HandleTypeDef *_i2c;

/* Инициализация RTC */
void RTC_init(I2C_HandleTypeDef *i2c) {
	_i2c = i2c;
}
/* Получение текущего времени из RTC */
RTC_time RTC_getTime(void) {
	RTC_time time = {0,0,0};
	uint8_t buff[3] = {0,0,0};
	HAL_I2C_Master_Transmit(_i2c, 0b11010000, buff, 1, 0xFF);
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 3, 0xFF);
	buff[0] &= ~(1<<7);
	time.sec = (buff[0]>>4)*10 + (buff[0] & 0x0F);
	buff[1] &= ~(1<<7);
	time.min = (buff[1]>>4)*10 + (buff[1] & 0x0F);
	buff[2] &= ~(3<<6);
	time.hour = (buff[2]>>4)*10 + (buff[2] & 0x0F);

	return time;
}
/* Получение даты из RTC */
RTC_date RTC_getDate(void) {
	RTC_date date = {0,0,0,0};
	uint8_t buff[4] = {3,0,0,0};
	HAL_I2C_Master_Transmit(_i2c, 0b11010000, buff, 1, 0xFF);
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 4, 0xFF);
	buff[0] &= 0b00000111;
	date.weekday = buff[0] & 0x0F;
	buff[1] &= ~(3<<6);
	date.day = (buff[1]>>4)*10 + (buff[1] & 0x0F);
	buff[2] &= ~(7<<5);
	date.month = (buff[2]>>4)*10 + (buff[2] & 0x0F);
	date.year = (buff[3]>>4)*10 + (buff[3] & 0x0F);
	return date;
}
/* Установка времени в RTC */
void RTC_setTime(RTC_time time) {
	uint8_t buff[4] = {0x00,(time.sec/10<<4)|(time.sec%10),(time.min/10<<4)|(time.min%10),(time.hour/10<<4)|(time.hour%10)};
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 4, 0xFF); 
}
/* Установка даты в RTC */
void RTC_setDate(RTC_date date) {
	uint8_t buff[5] = {0x03,date.weekday,(date.day/10<<4)|(date.day%10),(date.month/10<<4)|(date.month%10),(date.year/10<<4)|(date.year%10)};
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 5, 0xFF); 
}
//TODO: функции для будильников, функции для настроек, функция для термометра
