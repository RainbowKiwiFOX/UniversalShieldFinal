#include "UniversalShield.h"
/* Библиотеки для работы с периферией */
#include "spi.h"
/* Библиотеки для работы с дисплеем */
#include "BD663474.h"
#include "XPT2046.h"
/* Прочие библиотеки */
#include "DisplayButtons.h" //Экранные кнопки
#include "AnalogClock.h"		//Аналоговые часы
/* Прототипы функций */

/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE, 320, 240);
	TFT_fillDisplay(TFT_COLOR_Black); //Заливка дисплея чёрным
	
	clockInit();
	
	while(1) {
		uint8_t sec = HAL_GetTick()/1000%60;
		uint8_t min = 28;
		uint8_t hour = 2;
		uint32_t starTime = HAL_GetTick();
		
		printTime(hour,min,sec);
		
		uint16_t time = HAL_GetTick()-starTime;
		char buff[5];
		sprintf(buff,"%2d",sec);
		TFT_print(250, 50, buff);
		HAL_Delay(1000);
	}
	
	//TODO: Диспетчеризация задач, определение состояний и событий
}