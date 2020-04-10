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
/* Установка момента срабатывания будильника 1 */
void RTC_alarm1Set(RTC_time time, RTC_date date, uint8_t alarmRate) {
	uint8_t buff[5];																		//Буффер отправки
	buff[0] = 0x07; 																		//Номер записываемого регистра
	buff[1] = (time.sec/10<<4)|(time.sec%10); 					//Запись значения секунд
	buff[2] = (time.min/10<<4)|(time.min%10); 					//Запись значения минут
	buff[3] = (time.hour/10<<4)|(time.hour%10); 				//Запись значения часов
	//В зависимости от настроек, запись дня недели или даты
	if(alarmRate == A1_whenDHMS) { 											//Если по дню месяца
		buff[4] = (date.day/10<<4)|(date.day%10); 				//Запись значения дня
	}
	if(alarmRate == A1_whenWdHMS) {											//Если по дню недели
		buff[4] = (0b01000000 | date.weekday); 						//Запись значения дня недели
	}
	//Запись значения принципа срабатывания будильника
	for(uint8_t i = 0; i < 4; i++) {
		if(alarmRate & (1<<i)) buff[i+1] |= 0b10000000; 	//Установка 1 в старший бит если в маске alarmRate стоит 1 
	}
	//Отправка подготовленных данных в RTC
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 5, 0xFF);
}

/* Установка момента срабатывания будильника 2 */
void RTC_alarm2Set(RTC_time time, RTC_date date, uint8_t alarmRate) {
	uint8_t buff[4];																		//Буффер отправки
	buff[0] = 0x0B; 																		//Номер записываемого регистра
	buff[1] = (time.min/10<<4)|(time.min%10); 					//Запись значения минут
	buff[2] = (time.hour/10<<4)|(time.hour%10); 				//Запись значения часов
	//В зависимости от настроек, запись дня недели или даты
	if(alarmRate == A2_whenDHM) { 											//Если по дню месяца
		buff[3] = (date.day/10<<4)|(date.day%10); 				//Запись значения дня
	}
	if(alarmRate == A2_whenWdHM) {											//Если по дню недели
		buff[3] = (0b01000000 | date.weekday); 						//Запись значения дня недели
	}
	//Запись значения принципа срабатывания будильника
	for(uint8_t i = 0; i < 3; i++) {
		if(alarmRate & (1<<i)) buff[i+1] |= 0b10000000; 	//Установка 1 в старший бит если в маске alarmRate стоит 1 
	}
	//Отправка подготовленных данных в RTC
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 4, 0xFF);
}

/* Включение будильника 1 */
void RTC_alarm1On(void) {
	RTC_alarm1BellReset();
	uint8_t buff[2] = {0x0E};																	//Буффер данных
	//Получение значения регистра контроля (0x0E)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);	
	buff[1] = buff[0]|(1<<0);																	//Запись единицы в бит прерывания (A1IE)
	//Запись полученного значения регистра контроля (0x0E)
	buff[0] = 0x0E;
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 2, 0xFF);
}

/* Выключение будильника 1 */
void RTC_alarm1Off(void) {
	RTC_alarm1BellReset();																		//Сброс будильника
	uint8_t buff[2] = {0x0E};																	//Буффер данных
	//Получение значения регистра контроля (0x0E)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);	
	buff[1] = buff[0]&~(1<<0);																//Запись нуля в бит прерывания (A1IE)
	//Запись полученного значения регистра контроля (0x0E)
	buff[0] = 0x0E;
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 2, 0xFF);
}

/* Включение будильника 2 */
void RTC_alarm2On(void) {
	RTC_alarm2BellReset();																		//Сброс будильника
	uint8_t buff[2] = {0x0E};																	//Буффер данных
	//Получение значения регистра контроля (0x0E)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);	
	buff[1] = buff[0]|(1<<1);																	//Запись единицы в бит прерывания (A2IE)
	//Запись полученного значения регистра контроля (0x0E)
	buff[0] = 0x0E;
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 2, 0xFF);
}
/* Выключение будильника 2 */
void RTC_alarm2Off(void) {
	RTC_alarm2BellReset();																		//Сброс будильника
	uint8_t buff[2] = {0x0E};																	//Буффер данных
	//Получение значения регистра контроля (0x0E)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);	
	buff[1] = buff[0]&~(1<<1);																//Запись нуля в бит прерывания (A2IE)
	//Запись полученного значения регистра контроля (0x0E)
	buff[0] = 0x0E;
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 2, 0xFF);
}

/* Проверка срабатывания будильника 1 */
uint8_t RTC_alarm1IsBell(void) {
	uint8_t buff[1] = {0x0F};																	//Буффер данных
	//Получение значения регистра статуса (0x0F)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);
	if(buff[0] & (1<<0)) return 1; else return 0;							//Возврат значения активности будильника
}

/* Проверка срабатывания будильника 2 */
uint8_t RTC_alarm2IsBell(void) {
	uint8_t buff[1] = {0x0F};																	//Буффер данных
	//Получение значения регистра статуса (0x0F)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);
	if(buff[0] & (1<<1)) return 1; else return 0;							//Возврат значения активности будильника
}

/* Сброс срабатывания будильника 1 */
void RTC_alarm1BellReset(void) {
	uint8_t buff[2] = {0x0F};																	//Буффер данных
	//Получение значения регистра статуса (0x0F)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);	
	buff[1] = buff[0]&~(1<<0);																//Запись нуля в бит флага будильника (A1F)
	//Запись полученного значения регистра контроля (0x0F)
	buff[0] = 0x0F;
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 2, 0xFF);
}
/* Сброс срабатывания будильника 2 */
void RTC_alarm2BellReset(void) {
	uint8_t buff[2] = {0x0F};																	//Буффер данных
	//Получение значения регистра статуса (0x0F)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);	
	buff[1] = buff[0]&~(1<<1);																//Запись нуля в бит флага будильника (A2F)
	//Запись полученного значения регистра контроля (0x0F)
	buff[0] = 0x0F;
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 2, 0xFF);
}
/* Проверка работы будильника 1 */
uint8_t RTC_alarm1IsActive(void) {
	uint8_t buff[1] = {0x0E};																	//Буффер данных
	//Получение значения регистра контроля (0x0E)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);
	if(buff[0] & (1<<0)) return 1; else return 0;							//Возврат значения активности будильника
}
/* Проверка работы будильника 2 */
uint8_t RTC_alarm2IsActive(void) {
	uint8_t buff[1] = {0x0E};																	//Буффер данных
	//Получение значения регистра контроля (0x0E)
	HAL_I2C_Master_Transmit(_i2c, 0b11010001, buff, 1, 0xFF);	
	HAL_I2C_Master_Receive(_i2c, 0b11010001, buff, 1, 0xFF);
	if(buff[0] & (1<<1)) return 1; else return 0;							//Возврат значения активности будильника
}
//TODO:функции для настроек, функция для термометра
//TODO: Вкл/выкл RTC, 
