#include "UniversalShield.h"
/* Библиотеки для работы с периферией */
#include "spi.h"
#include "usart.h"
/* Библиотеки для работы с дисплеем */
#include "BD663474.h"
#include "XPT2046.h"
/* Прочие библиотеки */
#include "DisplayButtons.h" //Экранные кнопки
#include "AnalogClock.h"		//Аналоговые часы
#include <stdio.h>					//Стандартный ввод/вывод
#include <stdarg.h>					//Стандартные аргументы
/* Прототипы функций */

/* Глобальные переменные */
void (*printPurpose)(char); //Указатель на функцию печати текста
/* Функции */
//TODO: Вынести эти функции в отдельный файл
//Печать символа в UART
void UART_printChar(char c) {
	uint8_t b[1] = {c};
	HAL_UART_Transmit(&huart2, b, 1, 0xFF);
}
//Стандартная функция печатиы
int fputc(int c, FILE * stream) {
	printPurpose(c);
	return c; 
}
//Форматированная печать на дисплей
void TFT_printf(const char * __restrict format, ...) {
	printPurpose = TFT_printChar;
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}
//Форматированная печать в UART
void UART_printf(const char * __restrict format, ...) {
	printPurpose = UART_printChar;
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}

/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE, 320, 240);
	TFT_fillDisplay(TFT_COLOR_Black); //Заливка дисплея чёрным
	
	clockInit(); //Инициализация аналоговых часов
	//TODO: В инициализацию класть фон и координаты часов
	TFT_setTextBackColor(TFT_COLOR_Black); //Чёрный фон текста на экране
	char weekdays[7][23] = {"Понедельник","    Вторник","      Среда","    Четверг","    Пятница","    Суббота","Воскресение"};
	while(1) {
		uint8_t sec = HAL_GetTick()/1000%60;
		uint8_t min = 28;
		uint8_t hour = 2;
		uint8_t day= 9, month = 4, year = 20, wd = HAL_GetTick()/1000%7;
		
		printTime(hour,min,sec);
		
		TFT_setColor(TFT_COLOR_Silver);
		TFT_setFontSize(3);
		TFT_setCursor(178,0);
		TFT_printf("%02d.%02d.%02d", day, month, year);
		TFT_setCursor(232,42);
		TFT_printf("%02d%c%02d",hour,sec%2 ? ':' : ' ',min);
		TFT_setFontSize(2);
		TFT_setCursor(189,24);
		TFT_printf("%11s",weekdays[wd]);
		HAL_Delay(1000);
	}
	
	//TODO: Диспетчеризация задач, определение состояний и событий
}
