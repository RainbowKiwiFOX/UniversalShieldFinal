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
void clockInit(void); //Инициализация циферблата часов

/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE, 320, 240);
	TFT_fillDisplay(TFT_COLOR_Black); //Заливка дисплея чёрным
	
	clockInit();
	
	
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
/* Инициализация циферблата часов */
void clockInit(void) {
	/* Рисование внешней и внутренней окружностей */
	TFT_drawCircle(CLOCK_X, CLOCK_Y, CLOCK_RADIUS_EXT, 2, CLOCK_COLOR_EXT);
	TFT_drawCircle(CLOCK_X, CLOCK_Y, CLOCK_RADIUS_INT, 3, CLOCK_COLOR_INT);
	/* Рисование точек делений */
	for(uint16_t fi = 0; fi <= 360; fi+=30)
	TFT_fillCircle(CLOCK_X+(CLOCK_RADIUS_NUMS-3)*cos(fi*3.14f/180.0f), CLOCK_Y-(CLOCK_RADIUS_NUMS-3)*sin(fi*3.14f/180.0f), 4, TFT_COLOR_Yellow);
	/* Расстановка значений часов */
	TFT_setColor(CLOCK_COLOR_NUMBERS);
	TFT_setTextBackColor(TFT_COLOR_Black);
	TFT_setFontSize(2);
	TFT_print(CLOCK_X-8+CLOCK_RADIUS_NUMS, CLOCK_Y-7,		"3");
	TFT_print(CLOCK_X-4, CLOCK_Y+CLOCK_RADIUS_NUMS-12,	"6");
	TFT_print(CLOCK_X-CLOCK_RADIUS_NUMS, CLOCK_Y-7,			"9");
	TFT_print(CLOCK_X-11, CLOCK_Y-CLOCK_RADIUS_NUMS-1,	"12");
}
