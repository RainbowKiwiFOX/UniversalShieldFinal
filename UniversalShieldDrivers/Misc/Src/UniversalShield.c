#include "UniversalShield.h"
/* Библиотеки для работы с периферией */
#include "spi.h"
#include "i2c.h"
/* Библиотеки для работы с дисплеем */
#include "BD663474.h"
#include "XPT2046.h"
/* Прочие библиотеки */
#include "TaskManager.h"		//Диспетчер задач
#include "DS3231.h"					//Часы реального времени
/* Прототипы функций */

/* Глобальные переменные */

/* Функции */

/* Прочее */

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
	//TODO: Календарь, рисование, график изменения температуры, осциллограф по звуку, тестирование пищалки, светодиода, ик-приёмника, освещённости, микрофона
}
