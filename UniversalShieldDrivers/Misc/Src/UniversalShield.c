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
#include "MelodyPlayer.h"		//Воспроиведение мелодий
#include "niggers.c"
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
	
	TFT_drawImage(320,240, niggers1);
	melodyBit_t astronomia[] = {
		//1
		{A2,220},
		{P,20},
		{A2,220},
		{P,20},
		{A2,220},
		{P,20},
		{A2,220},
		{P,20},
		{C_3,220},
		{P,20},
		{C_3,220},
		{P,20},
		{C_3,220},
		{P,20},
		{C_3,220},
		{P,20},
		//2
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{E3,220}, //Е3
		{P,20},
		{E3,220},
		{P,20},
		{E3,220},
		{P,20},
		{E3,220},
		{P,20}, 
		//3
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		//4
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{B2,220},
		{P,20},
		{A2,220},
		{P,20},
		{G_2,220},
		{P,20},
		{E2,220},
		{P,20},
		//5 
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{C_3,220},
		{P,20},
		{B2,500},
		{P,20},
		{A2,500},
		{P,20},
		//6
		{G_2,500},
		{P,20},
		{G_2,220},
		{P,20},
		{G_2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{A2,220},
		{P,20},
		{G_2,220},
		{P,20},
		//7
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		//8
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		//9
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{C_3,220},
		{P,20},
		{B2,500},
		{P,20},
		{A2,500},
		{P,20},
		//10
		{G_2,500},
		{P,20},
		{G_2,220},
		{P,20},
		{G_2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{A2,220},
		{P,20},
		{G_2,220},
		{P,20},
		//11
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		//12
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},

		//13
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{C_3,220},
		{P,20},
		{B2,500},
		{P,20},
		{A2,500},
		{P,20},
		//14
		{G_2,500},
		{P,20},
		{G_2,220},
		{P,20},
		{G_2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{A2,220},
		{P,20},
		{G_2,220},
		{P,20},
		//15
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		//16
		{F_2,500},
		{P,20},
		{F_2,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		{G_3,220},
		{P,20},
		{A3,220},
		{P,20},
		//1
		{A2,220},
		{P,20},
		{A2,220},
		{P,20},
		{A2,220},
		{P,20},
		{A2,220},
		{P,20},
		{C_3,220},
		{P,20},
		{C_3,220},
		{P,20},
		{C_3,220},
		{P,20},
		{C_3,220},
		{P,20},
		//2
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{B2,220},
		{P,20},
		{E3,220}, //Е3
		{P,20},
		{E3,220},
		{P,20},
		{E3,220},
		{P,20},
		{E3,220},
		{P,20}, 
		//3
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		//4
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
		{F_3,220},
		{P,20},
/**/
	};
	playMelody(astronomia, sizeof(astronomia)/sizeof(melodyBit_t));
	
	HAL_Delay(7500);
	TFT_drawImage(320,240, niggers2);
	HAL_Delay(8000);
	TFT_drawImage(320,240, niggers3);
	HAL_Delay(7500);
	TFT_drawImage(320,240, niggers2);
	HAL_Delay(8000);
	TFT_drawImage(320,240, niggers3);
	HAL_Delay(7500);
	TFT_drawImage(320,240, niggers2);
	HAL_Delay(8000);
	TFT_drawImage(320,240, niggers3);
	while(1) {
		//Обработка событий и вызов состояний
		//taskManagerTick();
	}
	//TODO: Календарь, рисование, график изменения температуры, осциллограф по звуку, тестирование пищалки, светодиода, ик-приёмника, освещённости, микрофона
}

//Обработчик прерываний энкодера
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	//Обработка нажатия кнопки энкодера
	if(GPIO_Pin == ENC_SW_Pin) {
		//Простой антидребезг
		static uint32_t lastPressTime = 0xFFFFFF;			//Время последнего нажатия
		if(HAL_GetTick()-lastPressTime < 250) return;	//Если нажатие было менее 250 мс назад, то возврат
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
