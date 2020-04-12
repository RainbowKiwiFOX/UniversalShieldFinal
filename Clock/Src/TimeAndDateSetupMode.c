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
#define startX 120			//Расчёт начального X печати времени
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
void saveTimeAndDate(uint16_t i, touchStates ts);
/* Глобальные переменные */
//Значения времени и даты из RTC
extern RTC_time time;
extern RTC_date date;
//Позиция курсора
static uint8_t cursorPos = 0;
//Максимальные значения дней в месяцах
uint8_t monthsMax[] = {31,28,31,30,31,30,31,31,30,31,30,31};
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
//Кнопки инкремента и декремента значений, "сохранить"
static button_t upDownSave[] = {
	//ID	posX 					 poxY		Длина 							Высота								Вызов функции
	{  0 , 0, 						0, 	curLength+curWidth  ,curHeight+curWidth		, incTimeAndDate}, //Увеличить
	{  1 , 0,							0, 	curLength+curWidth  ,curHeight+curWidth		, decTimeAndDate}, //Уменьшить
	{  3 ,getMeanX(9,3)-5,200, 	 170,									31, 								saveTimeAndDate}, //Сохранение
};
//Визуальная часть кнопки "Сохранить"
static visualButton_t save[] = {
	{&upDownSave[2], roundRectangle_f, TFT_COLOR_Silver, 2, TFT_COLOR_none}
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
	upDownSave[0].posX = curX0;
	upDownSave[0].posY = curY0-curHeight;
	upDownSave[1].posX = curX0;
	upDownSave[1].posY = curY0+curDistance;
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
/* Сохранение значений в RTC */
void saveTimeAndDate(uint16_t i, touchStates ts) {
	if (ts != T_pressed) return; //Игнорирование лишних нажатий
	
	/* Вычисление дня недели из указанной даты */
	uint16_t days = 0; //Количество дней прошедших с 1 января 2000 года (суббота)
	//Подсчёт дней в предыдущих годах
	for(uint8_t y = 0; y < date.year; y++) {
		days += 365 + !(y%4); //Прибавление 365 дней и 1 если год високосный
	}
	//Подсчёт дней в предыдущих месяцах
	for(uint8_t m = 1; m < date.month; m++) {
		days += monthsMax[m-1];
	}
	if((date.month > 2) && (date.year%4)) days++;	//Если текущий год високосный и февраль уже был, то +1 к дням
	//Прибавление текущего дня календаря
	days += date.day;
	
	/* Вычисление дня недели из количества дней прошедших с 1 января 2000 года (суббота) */
	days -= 2; //Уменьшение дней до понедельника 3 января 2000 года
	date.weekday = days%7+1; //Вычисление дня недели и смещение +1 дня того, чтобы понедельник был днём №1 (RTC хранит значения от 1 до 7)
	
	/* Отправка времени и даты в RTC */
	RTC_setTime(time);
	RTC_setDate(date);
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
		TFT_print(10, startY+4, "Время");
		TFT_print(10, startYDate+4, "Дата");
		TFT_print(getMeanX(9,3), 205, "Сохранить");
		printButtons(save, sizeof(save)/sizeof(visualButton_t));
		TFT_setFontSize(fontSize);
		printTime();	//Начальная печать времени
		printDate();	//Начальная печать даты
	}
	//Получение значения нажатия на экран
	touch_t t = XPT2046_getTouch();
	//Обработка нажатия на экран
	buttonsTouchHandler(cursorMove, sizeof(cursorMove)/sizeof(button_t), t);
	buttonsTouchHandler(upDownSave, sizeof(upDownSave)/sizeof(button_t), t);
}