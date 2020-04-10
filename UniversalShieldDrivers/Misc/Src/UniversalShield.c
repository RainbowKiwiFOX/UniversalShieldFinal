#include "UniversalShield.h"
/* Библиотеки для работы с периферией */
#include "spi.h"
#include "i2c.h"
/* Библиотеки для работы с дисплеем */
#include "BD663474.h"
#include "XPT2046.h"
/* Прочие библиотеки */
#include "DisplayButtons.h" //Экранные кнопки
#include "AnalogClock.h"		//Аналоговые часы
#include "LM75.h"						//Цифровой датчик температуры
#include "DS3231.h"					//Часы реального времени
#include <stdio.h>					//Стандартный ввод/вывод
/* Прототипы функций */

/* Глобальные переменные */

/* Функции */

/* Прочее */
#define BACKGROUND_COLOR TFT_COLOR_Black //Цвет фона

/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE, 320, 240);
	TFT_fillDisplay(BACKGROUND_COLOR); //Заливка дисплея чёрным
	TFT_setTextBackColor(BACKGROUND_COLOR); //Чёрный фон текста на экране
	
	clockInit(BACKGROUND_COLOR); //Инициализация аналоговых часов
	RTC_init(&hi2c1);	//Инициализация RTC
	
	char weekdays[7][23] = {"Понедельник","    Вторник","      Среда","    Четверг","    Пятница","    Суббота","Воскресение"};
	
	RTC_alarm1Set((RTC_time){45,42,18},(RTC_date){0,0,0,0},A1_whenHMS);
	RTC_alarm1On();
	
	while(1) {
		RTC_time time = RTC_getTime();
		RTC_date date = RTC_getDate();
		
		printAnalogTime(time.hour,time.min,time.sec);
		
		TFT_setColor(TFT_COLOR_Silver);
		TFT_setFontSize(3);
		TFT_setCursor(178,0);
		TFT_printf("%02d.%02d.%02d", date.day, date.month, date.year);
		TFT_setCursor(232,42);
		TFT_printf("%02d%c%02d",time.hour,time.sec%2 ? ':' : ' ',time.min);
		TFT_setFontSize(2);
		TFT_setCursor(189,24);
		TFT_printf("%11s",weekdays[date.weekday-1]);
		
		TFT_setCursor(200, 200);
		TFT_printf("%3.2f*C %c", LM75_getTemperature(&hi2c1,LM75_DEFAULTADDR), RTC_alarm1IsBell() ? 'A':' ');
		HAL_Delay(1000);
	}
	
	//TODO: Диспетчеризация задач, определение состояний и событий
}
