#ifndef __ANALOGCLOCK_H
#define __ANALOGCLOCK_H

#include "main.h"

/* Настройки часов */
#define CLOCK_X 120																	//Координата X центра часов
#define CLOCK_Y 120																	//Координата Y центра часов
#define CLOCK_RADIUS_EXT 120												//Размер внешнего радиуса часов
#define CLOCK_RADIUS_INT (CLOCK_RADIUS_EXT*0.93f)		//Размер внутреннего радиуса часов
#define CLOCK_RADIUS_NUMS (CLOCK_RADIUS_EXT*0.86f)	//Размер радиуса цифр и точек
#define CLOCK_ARROW_LENGTH (CLOCK_RADIUS_EXT*0.75f)	//Длина секундной стрелки
#define CLOCK_COLOR_EXT 			TFT_COLOR_Silver			//Цвет внешнего радиуса часов
#define CLOCK_COLOR_INT 			TFT_COLOR_Silver			//Цвет внутреннего радиуса часов
#define CLOCK_COLOR_NUMBERS 	TFT_COLOR_Yellow			//Цвет чисел и точек внутри часов
#define CLOCK_COLOR_BACK TFT_COLOR_Black						//Цвет фона часов
#define CLOCK_COLOR_SECONDARROW TFT_COLOR_Red				//Цвет секундной стрелки
#define CLOCK_COLOR_MINUTEARROW TFT_COLOR_White			//Цвет минутной стрелки
#define CLOCK_COLOR_HOURARROW TFT_COLOR_White				//Цвет часовой стрелки

/* Прототипы функций */
void clockInit(void); 														//Инициализация циферблата часов
void printTime(uint8_t h, uint8_t m, uint8_t s); 	//Печать текущего времени на экране

#endif
