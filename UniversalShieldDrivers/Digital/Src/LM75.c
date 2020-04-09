#include "LM75.h"

/* Функция получения температуры из датчика */
float LM75_getTemperature(I2C_HandleTypeDef *i2c, uint8_t addr) {
	uint8_t payload[2] = {0,0}; //Буфер для хранения информации, которая будет отправляться/приниматься с датчика
	//Отправка запроса к датчику
	HAL_I2C_Master_Transmit(i2c, (uint16_t)addr, (uint8_t *)payload, 1, 0xFF);
	//Прием ответа от датчика
	HAL_I2C_Master_Receive(i2c, (uint16_t)addr, (uint8_t *)payload, 2, 0xFF);
	//Преобразование принятых значений в температуру в градусах Цельсия с последующим возвратом
	//ВНИМАНИЕ! ЗНАКОВЫЙ БИТ НЕ УЧИТЫВАЕТСЯ
	return ((((uint16_t)payload[0]<<8) | payload[1])>>5)*0.125f;
}

/* Функция получения сглаженной температуры */
float LM75_getSmoothedTemperature(I2C_HandleTypeDef *i2c, uint8_t addr) {
	static float temp_buffer = -128.0f;
	//Получение начальных данных о температуре для нормального сглаживания при первом вызове 
	if (temp_buffer == -128.0f) temp_buffer = LM75_getTemperature(i2c, addr);
	//Экспоненциальное сглаживание с коэффициентом LM75_SMOOTHRATIO
	temp_buffer = (temp_buffer * LM75_SMOOTHRATIO) + LM75_getTemperature(i2c, addr) * (1.0f-LM75_SMOOTHRATIO);
	//Возврат значения
	return temp_buffer;
}
