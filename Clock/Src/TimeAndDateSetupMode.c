#include "UniversalShield.h"

#include "BD663474.h"
#include "XPT2046.h"
#include "DS3231.h"					//Часы реального времени
#include "DisplayButtons.h" //Экранные кнопки


/* Макроподстановки */
//Настройки вывода времени
#define fontSize 4																		//Размер шрифта
#define startX ((320-(8*5*fontSize+7*fontSize*1))/2)	//Расчёт начального X печати времени
#define startY 80

#define curStartX (startX+6)						//Расчёт начального X печати курсора
#define curStartY (startY-10)		//Начальный Y печати курсора
#define curStep 72 											//Шаг перемещения курсора
#define curWidth 3											//Ширина линии курсора
#define curLength 30										//Длина курсора
#define curHeight (curLength/2)					//Высота курсора
#define curDistance (11*fontSize)				//Расстояние между верхней и нижней стрелочкой
//Точки вершин треугольников курсора
#define curX0 (curStartX+cursorPos*curStep)
#define curY0 curStartY
#define curX1 curX0+curLength/2
#define curY1_up curStartY-curHeight
#define curY1_down curStartY+curHeight
#define curX2 curX0+curLength
#define curY2 curStartY

#define BACKGROUND_COLOR TFT_COLOR_Blue	//Цвет фона экрана
#define BAR_COLOR	TFT_COLOR_Navy				//Цвет верхней полосы экрана
#define TEXT_COLOR TFT_COLOR_White			//Цвет текста на экране

/* Глобальные переменные */
//Значения времени и даты из RTC
extern RTC_time time;
extern RTC_date date;
//Позиция курсора
static uint8_t cursorPos = 0;


/* Функция печати курсора указанным цветом */
void printCursor(uint16_t color) {
	//Треугольник вверх
	TFT_drawTriangle(curX0, curY0, curX1, curY1_up, curX2, curY2, curWidth, color);
	//Треугольник вниз
	TFT_drawTriangle(curX0, curY0+curDistance, curX1, curY1_down+curDistance, curX2, curY2+curDistance, curWidth, color);
}

/* Установка значения и перемещение курсора на экране */
void setCursor(uint16_t pos) {
	//Стирание предыдущего курсора
	printCursor(BACKGROUND_COLOR);
	//Обновление положения курсора
	cursorPos = pos;
	//Рисование нового курсора
	printCursor(TEXT_COLOR);
}

/* Увеличение значения даты или времени в зависимости от значения курсора */
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
/* Уменьшение значения даты или времени в зависимости от значения курсора */
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
		time = RTC_getTime(); 	//Обновление текущего времени
		date = RTC_getDate();		//Обновление текущей даты
		TFT_fillDisplay(BACKGROUND_COLOR); //Заливка дисплея фоном
		setCursor(0);						//Обнуление значения курсора
		//Рисование бара
		TFT_fillRectangle(0,0,320,28,BAR_COLOR);
		TFT_setTextBackColor(TFT_COLOR_none);
		TFT_setColor(TEXT_COLOR);
		TFT_setFontSize(2);
		TFT_print(16,4, "Установка времени и даты");
		TFT_setFontSize(fontSize);
		TFT_setTextBackColor(BACKGROUND_COLOR);
	}
	
	TFT_setCursor(startX,startY);
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