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
//Обработчик нажатия на кнопку энкодера
void encSWEventHandler(event_t event) {
	//Смена состояния при нажатии на кнопку
	if(getCurrentState() == standbyMode_s) setCurrentState(timeAndDateSetupMode_s);
	else setCurrentState(standbyMode_s);
}
//Обработчик инкремента энкодера
void encIncEventHandler(event_t event) {
	UART_printf("+\r\n");
}
//Обработчик декремента энкодера
void encDecEventHandler(event_t event) {
	UART_printf("-\r\n");
}

/* Главная функция */
void US_main(void) {
	/* Инициализация дисплея и тачскрина */
	TFT_init(TFT_ORIENT_LANDSCAPE, &hspi1);
	XPT2046_init(&hspi1, XPT2046_LANDSCAPE,320,240);
	RTC_init(&hi2c1);	//Инициализация RTC
	
	/* Регистрация состояний */
	registerState(standbyMode_s, (state_t){standbyMode,500});
	registerState(timeAndDateSetupMode_s, (state_t){timeAndDateSetupMode,10});
	
	/* Регистрация событий */
	registerEvent((eventHandler_t){encSWEventHandler,encoderPress_e});
	registerEvent((eventHandler_t){encIncEventHandler,encoderInc_e});
	registerEvent((eventHandler_t){encDecEventHandler,encoderDec_e});
	//Установка текущего состояния
	setCurrentState(standbyMode_s);
	
	while(1) {
		//Обработка событий и вызов состояний
		taskManagerTick();
	}
	//TODO: Календарь, рисование, график изменения температуры, осциллограф по звуку, тестирование пищалки, светодиода, ик-приёмника, освещённости, микрофона
}

//Обработчик прерываний энкодера
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	//Обработка нажатия кнопки энкодера
	if(GPIO_Pin == ENC_SW_Pin) {
		//Простой антидребезг
		static uint32_t lastPressTime = 0xFFFFFF;			//Время последнего нажатия
		if(HAL_GetTick()-lastPressTime < 200) return;	//Если нажатие было менее 250 мс назад, то возврат
		lastPressTime = HAL_GetTick();								//Обновление времени нажатия
		
		//Если кнопка нажата, установка состояния события как "произошло"
		setEventState(encoderPress_e, happen);
	}
	//Обработка поворота энкодера
	if(GPIO_Pin == ENC_CLK_Pin) {
		if (HAL_GPIO_ReadPin(ENC_D_GPIO_Port, ENC_D_Pin)) {
			setEventState(encoderInc_e, happen);
		}	else {
			setEventState(encoderDec_e, happen);
		}
	}
}
