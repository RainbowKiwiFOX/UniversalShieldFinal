#include "AnalogClock.h"
#include "BD663474.h"
#include <math.h>

static void drawSecondArrow(uint8_t s, uint16_t color);						//Рисование секундной стрелки
static void drawMinuteArrow(uint8_t m, uint16_t color);						//Рисование минутной стрелки
static void drawHourArrow(uint8_t h, uint8_t m, uint16_t color); 	//Рисование часовой стрелки

static uint16_t CLOCK_correctAngle(uint16_t angle) {
	return ((360-angle+90));
}
static float CLOCK_getfi(uint16_t angle) {
	return angle*3.14141596f/180.0f;
}

static uint16_t _backColor;

/* Инициализация циферблата часов */
void clockInit(uint16_t backColor) {
	_backColor = backColor;
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
	TFT_setTextBackColor(_backColor);
	TFT_setFontSize(2);
	TFT_print(CLOCK_X-8+CLOCK_RADIUS_NUMS, CLOCK_Y-7,		"3");
	TFT_print(CLOCK_X-4, CLOCK_Y+CLOCK_RADIUS_NUMS-12,	"6");
	TFT_print(CLOCK_X-CLOCK_RADIUS_NUMS, CLOCK_Y-7,			"9");
	TFT_print(CLOCK_X-11, CLOCK_Y-CLOCK_RADIUS_NUMS-1,	"12");
	/* Стирание стрелок если таковые имелись */
	TFT_fillCircle(CLOCK_X, CLOCK_Y, CLOCK_ARROW_LENGTH, _backColor);
}

/* Печать текущего времени на экране */
void printAnalogTime(uint8_t h, uint8_t m, uint8_t s) {
	/* Переменные для запоминания предыдущего значения */
	static uint8_t oldSArrow = 0;
	static uint8_t oldMArrow = 0;
	static uint8_t oldHArrow = 0;
	/* Стирание старых стрелок */
	drawSecondArrow(oldSArrow, _backColor);
	drawMinuteArrow(oldMArrow, _backColor);
	drawHourArrow(oldHArrow, oldMArrow, _backColor);
	/* Рисование стрелок */
	drawSecondArrow(s, CLOCK_COLOR_SECONDARROW);
	drawMinuteArrow(m, CLOCK_COLOR_MINUTEARROW);
	drawHourArrow(h, m, CLOCK_COLOR_HOURARROW);
	/* Запоминание предыдущих значений стрелок */
	oldSArrow = s;
	oldMArrow = m;
	oldHArrow = h;
}
/* Рисование секундной стрелки */
static void drawSecondArrow(uint8_t s, uint16_t color) {
	float fi = CLOCK_getfi(CLOCK_correctAngle(s*6));
	uint16_t x1 = CLOCK_X+(CLOCK_ARROW_LENGTH)*cos(fi);
	uint16_t y1 = CLOCK_Y-(CLOCK_ARROW_LENGTH)*sin(fi);
	TFT_drawLine(CLOCK_X, CLOCK_Y, x1, y1, 1, color);
}

/* Рисование минутной стрелки */
static void drawMinuteArrow(uint8_t m, uint16_t color) {
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
static void drawHourArrow(uint8_t h, uint8_t m, uint16_t color) {
	//Преобразование часов в деление круга на 60
	h = 60/12*(h%12)+5.0f/60.0f*m;
	//Вычисление вершин четырёхугольника
	uint16_t x0,y0,x1,y1,x2,y2,x3,y3;
	float fi = CLOCK_getfi(CLOCK_correctAngle(h*6));
	x0 = CLOCK_X+(CLOCK_ARROW_LENGTH*0.5)*cos(fi);
	y0 = CLOCK_Y-(CLOCK_ARROW_LENGTH*0.5)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(h*6+90));
	x1 = CLOCK_X+(CLOCK_ARROW_LENGTH/8)*cos(fi);
	y1 = CLOCK_Y-(CLOCK_ARROW_LENGTH/8)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(h*6+180));
	x2 = CLOCK_X+(CLOCK_ARROW_LENGTH/8)*cos(fi);
	y2 = CLOCK_Y-(CLOCK_ARROW_LENGTH/8)*sin(fi);
	fi = CLOCK_getfi(CLOCK_correctAngle(h*6+270));
	x3 = CLOCK_X+(CLOCK_ARROW_LENGTH/8)*cos(fi);
	y3 = CLOCK_Y-(CLOCK_ARROW_LENGTH/8)*sin(fi);
	TFT_drawLine(x0,y0,x1,y1,1,color);
	TFT_drawLine(x1,y1,x2,y2,1,color);
	TFT_drawLine(x2,y2,x3,y3,1,color);
	TFT_drawLine(x3,y3,x0,y0,1,color);
}
