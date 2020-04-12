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
#include "TaskManager.h"		//Диспетчер задач
/* Прототипы функций */
void standbyMode(callStatus_t s, eventStates_t *es);
void timeAndDateSetupMode(callStatus_t s, eventStates_t *es);
/* Глобальные переменные */
//Значения времени и даты из RTC
static RTC_time time;
static RTC_date date;

/* Функции */

/* Прочее */
#define BACKGROUND_COLOR TFT_COLOR_Black //Цвет фона
const char weekdays[7][23] = {"Понедельник","    Вторник","      Среда","    Четверг","    Пятница","    Суббота","Воскресение"};

/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE,320,240);
	RTC_init(&hi2c1);	//Инициализация RTC
	
	/* Регистрация состояний */
	registerState(standbyMode_s, (state_t){standbyMode,500});
	registerState(timeAndDateSetupMode_s, (state_t){timeAndDateSetupMode,10});
	//Установка текущего состояния
	setCurrentState(timeAndDateSetupMode_s);
	
	while(1) {
		//Обработка событий и вызов состояний
		taskManagerTick();
	}
	//TODO: Календарь, установка времени и даты, рисование, график изменения температуры, осциллограф по звуку, тестирование пищалки, светодиода, ик-приёмника, освещённости, микрофона
}

/* Режим ожидания */
void standbyMode(callStatus_t s, eventStates_t *es) {
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


/* Режим установки времени и даты */
#define startX (320-(8*5*4+7*4*1))/2
uint8_t cursorPos = 0;
void setCursor(uint16_t pos) {
	//TODO: Исправить это говноляпство
	#define dlina 30
	#define visota 15
	#define shag 72
	#define startY 71
	//Стирание предыдущих значков
	TFT_drawTriangle(startX + cursorPos*shag+6, startY, startX + cursorPos*shag + dlina/2+6, startY-visota, startX + cursorPos*shag + dlina+6, startY, 3, TFT_COLOR_Blue);     
	TFT_drawTriangle(startX + cursorPos*shag+6, startY+43, startX + cursorPos*shag + dlina/2+6, startY+43+visota, startX + cursorPos*shag + dlina+6, startY+43, 3, TFT_COLOR_Blue);     
	cursorPos = pos;
	//Рисование новых стрелочек
	TFT_drawTriangle(startX + pos*shag+6, startY, startX + pos*shag + dlina/2+6, startY-visota, startX + pos*shag + dlina+6, startY, 3, TFT_COLOR_White);     
	TFT_drawTriangle(startX + cursorPos*shag+6, startY+43, startX + cursorPos*shag + dlina/2+6, startY+43+visota, startX + cursorPos*shag + dlina+6, startY+43, 3, TFT_COLOR_White);     
}
void incTimeAndDate(uint16_t i) {
	switch(cursorPos) {
		case 0: {
			if(++time.hour > 23) time.hour = 0;
			break;
		}
		case 1: {
			if(++time.min > 59) time.min = 0;
			break;
		}
		case 2: {
			if(++time.sec > 59) time.sec = 0;
			break;
		}
	}
}
void decTimeAndDate(uint16_t i) {
	switch(cursorPos) {
		case 0: {
			if(--time.hour > 23) time.hour = 23;
			break;
		}
		case 1: {
			if(--time.min > 59) time.min = 59;
			break;
		}
		case 2: {
			if(--time.sec > 59) time.sec = 59;
			break;
		}
	}
}
void timeAndDateSetupMode(callStatus_t s, eventStates_t *es) {
	//TODO: Оптимизировать говнокнопки
	//Кнопки перемещения курсора по времени
	button_t cursorMove[] = {
		//ID	posX 	 		poxY	Форма кноки			Длина Ширина	Цвет линии			Ширина линии 	Цвет заливки		Вызов функции
		{  0 ,startX, 		80, rectangle_f, 			48  ,30		, TFT_COLOR_none, 			0			, TFT_COLOR_none, setCursor}, //Часы
		{  1 ,startX+70,	80, rectangle_f, 			48  ,30		, TFT_COLOR_none, 			0			, TFT_COLOR_none, setCursor},	//Минуты
		{  2 ,startX+140, 80, rectangle_f, 			48  ,30		, TFT_COLOR_none, 			0			, TFT_COLOR_none, setCursor},	//Секунды
	};
	//Кнопки инкремента и декремента значений
	button_t upDown[] = {
		//ID	posX 	 		poxY	Форма кноки			Длина Ширина	Цвет линии			Ширина линии 	Цвет заливки		Вызов функции
		{  0 ,startX, 		50, rectangle_f, 			48  ,30		, TFT_COLOR_Gray, 			1			, TFT_COLOR_none, incTimeAndDate}, //Увеличить
		{  1 ,startX,			110, rectangle_f, 		48  ,30		, TFT_COLOR_Gray, 			1			, TFT_COLOR_none, decTimeAndDate},			 //Уменьшить
	};
	//Если функция была вызвана впервые, то
	if(s == initial) {
		time = RTC_getTime(); //Обновление текущего времени
		date = RTC_getDate();	//Обновление текущей даты
		
		TFT_fillDisplay(TFT_COLOR_Blue); 			//Заливка дисплея фоном
		//Рисование бара
		TFT_fillRectangle(0,0,320,28,TFT_COLOR_Navy);
		TFT_setTextBackColor(TFT_COLOR_none);
		TFT_setColor(TFT_COLOR_White);
		TFT_setFontSize(2);
		TFT_print(16,4, "Установка времени и даты");
		TFT_setFontSize(4);
		TFT_setTextBackColor(TFT_COLOR_Blue);
	}
	
	TFT_setCursor(startX,80);
	TFT_printf("%02d:%02d:%02d",time.hour,time.min,time.sec);
	//Перемещение кнопок инкремента и декремента в зависимости от положения курсора
	upDown[0].posX = startX+cursorPos*70;
	upDown[1].posX = startX+cursorPos*70;
	touch_t t = XPT2046_getTouch();
	if(t.state == T_pressed) {
		buttonsTouchHandler(cursorMove, sizeof(cursorMove)/sizeof(button_t), t);
		buttonsTouchHandler(upDown, sizeof(upDown)/sizeof(button_t), t);
	}
}
