#include "UniversalShield.h"

#include "BD663474.h"
#include "XPT2046.h"
#include "DS3231.h"					//Часы реального времени
#include "DisplayButtons.h" //Экранные кнопки


/* Макроподстановки */
//Расчёт X для расположения текста по центру, size - кол-во символов
#define getMeanX(size, fontSize) ((320-(size*5*fontSize+(size-1)*fontSize*1))/2)
//Настройки вывода времени
#define fontSize 4											//Размер шрифта
#define startX getMeanX(8,fontSize)			//Расчёт начального X печати времени
#define startY 80
#define startYDate	startY+7*fontSize+30

#define curStartX (startX+6)						//Расчёт начального X печати курсора
#define curStartY (startY-10)						//Начальный Y печати курсора
#define curStep 72 											//Шаг перемещения курсора
#define curWidth 3											//Ширина линии курсора
#define curLength 30										//Длина курсора
#define curHeight (curLength/2)					//Высота курсора
#define curDistance (11*fontSize)				//Расстояние между верхней и нижней стрелочкой
//Точки вершин треугольников курсора
#define curX0 (curStartX+(cursorPos%3)*curStep)
#define curY0 (curStartY+(cursorPos/3)*(startYDate-startY))
#define curX1 curX0+curLength/2
#define curY1_up curY0-curHeight
#define curY1_down curY0+curHeight
#define curX2 curX0+curLength
#define curY2 curY0

#define BACKGROUND_COLOR TFT_COLOR_Blue	//Цвет фона экрана
#define BAR_COLOR	TFT_COLOR_Navy				//Цвет верхней полосы экрана
#define TEXT_COLOR TFT_COLOR_White			//Цвет текста на экране

/* Прототипы функций */
void setCursor(uint16_t pos, touchStates ts);
void incTimeAndDate(uint16_t i, touchStates ts);
void decTimeAndDate(uint16_t i, touchStates ts);
/* Глобальные переменные */
//Значения времени и даты из RTC
extern RTC_time time;
extern RTC_date date;
//Позиция курсора
static uint8_t cursorPos = 0;
//Кнопки перемещения курсора по времени и дате
static button_t cursorMove[] = {
	//ID	posX 	 				poxY			Длина Высота	Вызов функции
	{  0 ,startX, 		startY,				48  ,30		, setCursor}, //Часы
	{  1 ,startX+70,	startY,				48  ,30		, setCursor},	//Минуты
	{  2 ,startX+140, startY, 			48  ,30		, setCursor},	//Секунды
	{  3 ,startX, 		startYDate,		48  ,30		, setCursor}, //День
	{  4 ,startX+70,	startYDate,		48  ,30		, setCursor},	//Месяц
	{  5 ,startX+140, startYDate, 	48  ,30		, setCursor},	//Год
};
//Кнопки инкремента и декремента значений
static button_t upDown[] = {
	//ID	posX poxY		Длина 							Высота								Вызов функции
	{  0 ,0, 		0, 	curLength+curWidth  ,curHeight+curWidth		, incTimeAndDate}, //Увеличить
	{  1 ,0,		0, 	curLength+curWidth  ,curHeight+curWidth		, decTimeAndDate}, //Уменьшить
};
/* Функция печати времени на экране */
void printTime(void) {
	//Закрашивание старых значений
	TFT_fillRectangle(startX, startY, 8*5*fontSize+7*fontSize*1, 7*fontSize, BACKGROUND_COLOR);
	TFT_setCursor(startX,startY);
	TFT_printf("%02d:%02d:%02d",time.hour,time.min,time.sec);
}
/* Функция печати даты на экране */
void printDate(void) {
	//Закрашивание старых значений
	TFT_fillRectangle(startX, startYDate, 8*5*fontSize+7*fontSize*1, 7*fontSize, BACKGROUND_COLOR);
	TFT_setCursor(startX,startYDate);
	TFT_printf("%02d.%02d.%02d",date.day,date.month,date.year);
}
/* Функция печати курсора указанным цветом */
void printCursor(uint16_t color) {
	//Треугольник вверх
	TFT_drawTriangle(curX0, curY0, curX1, curY1_up, curX2, curY2, curWidth, color);
	//Треугольник вниз
	TFT_drawTriangle(curX0, curY0+curDistance, curX1, curY1_down+curDistance, curX2, curY2+curDistance, curWidth, color);
}

/* Установка значения и перемещение курсора на экране */
void setCursor(uint16_t pos, touchStates ts) {
	if (ts != T_pressed) return; //Игнорирование лишних нажатий
	//Стирание предыдущего курсора
	printCursor(BACKGROUND_COLOR);
	//Обновление положения курсора
	cursorPos = pos;
	//Рисование нового курсора
	printCursor(TEXT_COLOR);
	//Перемещение кнопок инкремента и декремента в зависимости от положения курсора
	upDown[0].posX = curX0;
	upDown[0].posY = curY0-curHeight;
	upDown[1].posX = curX0;
	upDown[1].posY = curY0+curDistance;
}

/* Увеличение значения даты или времени в зависимости от значения курсора */
void incTimeAndDate(uint16_t i, touchStates ts) {
	if (!((ts == T_pressed)||(ts == T_longHoldDown))) return; //Игнорирование лишних нажатий
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
		case 3: {
			if(++date.day > 31) date.day = 1;
			break;
		}
		case 4: {
			if(++date.month > 12) date.month = 1;
			break;
		}
		case 5: {
			if(++date.year > 99) date.year = 0;
			break;
		}
	}
	//Обновление времени и даты на экране
	printTime();
	printDate();
}
/* Уменьшение значения даты или времени в зависимости от значения курсора */
void decTimeAndDate(uint16_t i, touchStates ts) {
	if (!((ts == T_pressed)||(ts == T_longHoldDown))) return; //Игнорирование лишних нажатий
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
		case 3: {
			if(--date.day < 1) date.day = 31;
			break;
		}
		case 4: {
			if(--date.month < 1) date.month = 12;
			break;
		}
		case 5: {
			if(--date.year > 99) date.year = 99;
			break;
		}
	}
	//Обновление времени и даты на экране
	printTime();
	printDate();
}
/* Режим установки времени и даты */
void timeAndDateSetupMode(callStatus_t s, eventStates_t *es) {
	//Если функция была вызвана впервые, то
	if(s == initial) {
		time = RTC_getTime(); 	//Обновление текущего времени
		date = RTC_getDate();		//Обновление текущей даты
		TFT_fillDisplay(BACKGROUND_COLOR); //Заливка дисплея фоном
		setCursor(0, T_pressed);					 //Обнуление значения курсора
		//Рисование бара
		TFT_fillRectangle(0,0,320,28,BAR_COLOR);
		TFT_setTextBackColor(TFT_COLOR_none);
		TFT_setColor(TEXT_COLOR);
		TFT_setFontSize(2);
		TFT_print(16,4, "Установка времени и даты");
		TFT_setFontSize(3);
		TFT_print(getMeanX(6,3), 30, "Время:");
		TFT_print(getMeanX(4,3), 140, "Дата:");
		TFT_setFontSize(fontSize);
		printTime();	//Начальная печать времени
		printDate();	//Начальная печать даты
	}
	//Получение значения нажатия на экран
	touch_t t = XPT2046_getTouch();
	//Обработка нажатия на экран
	buttonsTouchHandler(cursorMove, sizeof(cursorMove)/sizeof(button_t), t);
	buttonsTouchHandler(upDown, sizeof(upDown)/sizeof(button_t), t);
}