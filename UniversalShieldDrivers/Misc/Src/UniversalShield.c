#include "UniversalShield.h"
/* Библиотеки для работы с периферией */
#include "spi.h"
/* Библиотеки для работы с дисплеем */
#include "BD663474.h"
#include "XPT2046.h"
#include "DisplayButtons.h"
/* Прочие библиотеки */
#include <math.h>
/* Прототипы функций */
void clockInit(void); 														//Инициализация циферблата часов
void printTime(uint8_t h, uint8_t m, uint8_t s); 	//Печать текущего времени на экране
void drawSecondArrow(uint8_t s, uint16_t color);	//Рисование секундной стрелки
void drawMinuteArrow(uint8_t m, uint16_t color);	//Рисование минутной стрелки
void drawHourArrow(uint8_t h, uint16_t color); 		//Рисование часовой стрелки
/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE, 320, 240);
	TFT_fillDisplay(TFT_COLOR_Black); //Заливка дисплея чёрным
	
	clockInit();
	
	while(1) {
		uint8_t sec = HAL_GetTick()/10%60;
		uint8_t min = HAL_GetTick()/10/60;
		uint8_t hour = HAL_GetTick()/10/360;
		uint32_t starTime = HAL_GetTick();
		
		printTime(hour,min,sec);
		
		uint16_t time = HAL_GetTick()-starTime;
		char buff[5];
		sprintf(buff,"%2d",sec);
		TFT_print(250, 50, buff);
		//HAL_Delay(1000);
	}
	
	//TODO: Диспетчеризация задач, определение состояний и событий
}

#define CLOCK_COLOR_EXT 			TFT_COLOR_Silver
#define CLOCK_COLOR_INT 			TFT_COLOR_Silver
#define CLOCK_COLOR_NUMBERS 	TFT_COLOR_Yellow
#define CLOCK_X 120
#define CLOCK_Y 120
#define CLOCK_RADIUS_EXT 120
#define CLOCK_RADIUS_INT (CLOCK_RADIUS_EXT*0.93f)
#define CLOCK_RADIUS_NUMS (CLOCK_RADIUS_EXT*0.86f)
#define CLOCK_ARROW_LENGTH (CLOCK_RADIUS_EXT*0.75f)
#define CLOCK_COLOR_BACK TFT_COLOR_Black
#define CLOCK_COLOR_SECONDARROW TFT_COLOR_Red
#define CLOCK_COLOR_MINUTEARROW TFT_COLOR_White
#define CLOCK_COLOR_HOURARROW TFT_COLOR_White
uint16_t CLOCK_correctAngle(uint16_t angle) {
	return ((360-angle+90));
}
float CLOCK_getfi(uint16_t angle) {
	return angle*3.14141596f/180.0f;
}
/* Инициализация циферблата часов */
void clockInit(void) {
	/* Рисование внешней и внутренней окружностей */
	TFT_drawCircle(CLOCK_X, CLOCK_Y, CLOCK_RADIUS_EXT, 2, CLOCK_COLOR_EXT);
	TFT_drawCircle(CLOCK_X, CLOCK_Y, CLOCK_RADIUS_INT, 3, CLOCK_COLOR_INT);
	/* Рисование точек делений по часам */
	for(uint16_t angle = 0; angle <= 360; angle += 30)
	TFT_fillCircle(CLOCK_X+(CLOCK_RADIUS_NUMS-3)*cos(CLOCK_getfi(angle)), CLOCK_Y-(CLOCK_RADIUS_NUMS-3)*sin(CLOCK_getfi(angle)), 4, TFT_COLOR_Yellow);
	/* Рисование точек делений по минутам */
	for(uint16_t angle = 0; angle <= 360; angle += 6)
	TFT_fillCircle(CLOCK_X+(CLOCK_RADIUS_NUMS-3)*cos(CLOCK_getfi(angle)), CLOCK_Y-(CLOCK_RADIUS_NUMS-3)*sin(CLOCK_getfi(angle)), 1, TFT_COLOR_Yellow);

	/* Расстановка значений часов */
	TFT_setColor(CLOCK_COLOR_NUMBERS);
	TFT_setTextBackColor(CLOCK_COLOR_BACK);
	TFT_setFontSize(2);
	TFT_print(CLOCK_X-8+CLOCK_RADIUS_NUMS, CLOCK_Y-7,		"3");
	TFT_print(CLOCK_X-4, CLOCK_Y+CLOCK_RADIUS_NUMS-12,	"6");
	TFT_print(CLOCK_X-CLOCK_RADIUS_NUMS, CLOCK_Y-7,			"9");
	TFT_print(CLOCK_X-11, CLOCK_Y-CLOCK_RADIUS_NUMS-1,	"12");
	/* Стирание стрелок если таковые имелись */
	TFT_fillCircle(CLOCK_X, CLOCK_Y, CLOCK_ARROW_LENGTH, CLOCK_COLOR_BACK);
}

/* Печать текущего времени на экране */
void printTime(uint8_t h, uint8_t m, uint8_t s) {
	/* Переменные для запоминания предыдущего значения */
	static uint8_t oldSArrow = 0;
	static uint8_t oldMArrow = 0;
	static uint8_t oldHArrow = 0;
	/* Стирание старых стрелок */
	drawSecondArrow(oldSArrow, CLOCK_COLOR_BACK);
	drawMinuteArrow(oldMArrow, CLOCK_COLOR_BACK);
	drawHourArrow(oldHArrow, CLOCK_COLOR_BACK);
	/* Рисование стрелок */
	drawSecondArrow(s, CLOCK_COLOR_SECONDARROW);
	drawMinuteArrow(m, CLOCK_COLOR_MINUTEARROW);
	drawHourArrow(h, CLOCK_COLOR_HOURARROW);
	/* Запоминание предыдущих значений стрелок */
	oldSArrow = s;
	oldMArrow = m;
	oldHArrow = h;
}
/* Рисование секундной стрелки */
void drawSecondArrow(uint8_t s, uint16_t color) {
	float fi = CLOCK_getfi(CLOCK_correctAngle(s*6));
	uint16_t x1 = CLOCK_X+(CLOCK_ARROW_LENGTH)*cos(fi);
	uint16_t y1 = CLOCK_Y-(CLOCK_ARROW_LENGTH)*sin(fi);
	TFT_drawLine(CLOCK_X, CLOCK_Y, x1, y1, 1, color);
}

/* Рисование минутной стрелки */
void drawMinuteArrow(uint8_t m, uint16_t color) {
	//Вычисление вершин четырёхугольника
	uint16_t x0,y0,x1,y1,x2,y2,x3,y3;
	float fi = CLOCK_getfi(CLOCK_correctAngle(m*6));
	x0 = CLOCK_X+(CLOCK_ARROW_LENGTH*0.75)*cos(fi);
	y0 = CLOCK_Y-(CLOCK_ARROW_LENGTH*0.75)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(m*6+90));
	x1 = CLOCK_X+(CLOCK_ARROW_LENGTH*0.10)*cos(fi);
	y1 = CLOCK_Y-(CLOCK_ARROW_LENGTH*0.10)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(m*6+180));
	x2 = CLOCK_X+(CLOCK_ARROW_LENGTH*0.10)*cos(fi);
	y2 = CLOCK_Y-(CLOCK_ARROW_LENGTH*0.10)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(m*6+270));
	x3 = CLOCK_X+(CLOCK_ARROW_LENGTH*0.10)*cos(fi);
	y3 = CLOCK_Y-(CLOCK_ARROW_LENGTH*0.10)*sin(fi);
	TFT_drawLine(x0,y0,x1,y1,1,color);
	TFT_drawLine(x1,y1,x2,y2,1,color);
	TFT_drawLine(x2,y2,x3,y3,1,color);
	TFT_drawLine(x3,y3,x0,y0,1,color);
}
/* Рисование часовой стрелки */
void drawHourArrow(uint8_t h, uint16_t color) {
	//Вычисление вершин четырёхугольника
	uint16_t x0,y0,x1,y1,x2,y2,x3,y3;
	float fi = CLOCK_getfi(CLOCK_correctAngle(h*30));
	x0 = CLOCK_X+(CLOCK_ARROW_LENGTH*0.5)*cos(fi);
	y0 = CLOCK_Y-(CLOCK_ARROW_LENGTH*0.5)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(h*30+90));
	x1 = CLOCK_X+(CLOCK_ARROW_LENGTH/8)*cos(fi);
	y1 = CLOCK_Y-(CLOCK_ARROW_LENGTH/8)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(h*30+180));
	x2 = CLOCK_X+(CLOCK_ARROW_LENGTH/8)*cos(fi);
	y2 = CLOCK_Y-(CLOCK_ARROW_LENGTH/8)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(h*30+270));
	x3 = CLOCK_X+(CLOCK_ARROW_LENGTH/8)*cos(fi);
	y3 = CLOCK_Y-(CLOCK_ARROW_LENGTH/8)*sin(fi);
	TFT_drawLine(x0,y0,x1,y1,1,color);
	TFT_drawLine(x1,y1,x2,y2,1,color);
	TFT_drawLine(x2,y2,x3,y3,1,color);
	TFT_drawLine(x3,y3,x0,y0,1,color);
}