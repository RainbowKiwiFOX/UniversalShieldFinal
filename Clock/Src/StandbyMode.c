#include "UniversalShield.h"
#include "i2c.h"

#include "DisplayButtons.h" //Экранные кнопки
#include "AnalogClock.h"		//Аналоговые часы
#include "LM75.h"						//Цифровой датчик температуры
#include "DS3231.h"					//Часы реального времени

#define BACKGROUND_COLOR TFT_COLOR_Black //Цвет фона

/* Глобальные переменные */
//Значения времени и даты из RTC
RTC_time time;
RTC_date date;
//Дни недели
const char weekdays[7][23] = {"Понедельник","    Вторник","      Среда","    Четверг","    Пятница","    Суббота","Воскресение"};

/* Режим ожидания */
void standbyMode(callStatus_t s, eventState_t *es) {
	//Если функция была вызвана впервые, то
	if(s == initial) {
		TFT_fillDisplay(BACKGROUND_COLOR); 			//Заливка дисплея чёрным
		TFT_setTextBackColor(BACKGROUND_COLOR); //Чёрный фон текста на экране
		clockInit(BACKGROUND_COLOR); 						//Инициализация аналоговых часов
	}
	
	time = RTC_getTime();
	date = RTC_getDate();
	
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
}