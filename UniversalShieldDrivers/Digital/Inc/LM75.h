#ifndef __LM75_H
#define __LM75_H
#include "main.h"

#define LM75_DEFAULTADDR 0x90 	//Стандартный адрес датчика
#define LM75_SMOOTHRATIO 0.75f 	//Коэфициент сглаживания фильтра

/* Функция получения температуры из датчика */
float LM75_getTemperature(I2C_HandleTypeDef *i2c, uint8_t addr);
/* Функция получения сглаженной температуры */
float LM75_getSmoothedTemperature(I2C_HandleTypeDef *i2c, uint8_t addr);

#endif
