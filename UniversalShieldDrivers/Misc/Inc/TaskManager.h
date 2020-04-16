#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "main.h"


/* События конечного автомата */
typedef enum {
	// Аппаратные
	//displayTouch_e,	//Нажатие на тачскрин экрана	
	encoderPress_e,		//Нажатие на кнопку энкодера. Устанавливается в обработчике прерывания
	encoderInc_e,			//Инкремент энкодера
	encoderDec_e,			//Декремент энкодера
	// Программые
	//alarm1Bell_e,		//Срабатывание будильника 1
	//alarm2Bell_e,		//Срабатывание будильника 2
	
	eventsSize				//Количество событий
} event_t;
/* Состояния конечного автомата */
typedef enum {
	standbyMode_s,						//Главный экран, режим ожидания
	timeAndDateSetupMode_s,		//Режим установки времени и даты
	
	statesSize								//Количество состояний														
} currentState_t;


/* Статусы событий */
typedef enum {
	noHappen,			//Не произошло
	happen,				//Произошло
	preProcessed,	//Обработано и готово к очистке
} eventState_t;
/* Структура функции обработки событий */
typedef struct {
	void (*function)(event_t event);			//Указатель на функцию
	event_t callEvent;										//Событие, по которому вызывать эту фукнцию
} eventHandler_t; 

/* Статус вызова функции */
typedef enum {
	initial,
	secondary
} callStatus_t;
/* Структура состояний конечного автомата */
typedef struct {
	void (*function)(callStatus_t status, eventState_t *eventStates);	//Указатель на функцию
	uint32_t callPeriod;																								//Вызывать функцию не чаще чем callPeriod мс
} state_t;

/* Регистрация события */
void registerEvent(eventHandler_t eh);
/* Регистрация состояния */
void registerState(currentState_t cs, state_t s);
/* Установка текущего состояния */
void setCurrentState(currentState_t cs);
/* Получить текущее событие */
currentState_t getCurrentState(void);
/* Установка состояния события */
void setEventState(event_t e, eventState_t state);
/* Один тик диспетчера */
void taskManagerTick(void);

#endif
