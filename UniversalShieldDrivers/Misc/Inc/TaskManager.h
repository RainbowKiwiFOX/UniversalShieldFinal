#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "main.h"


/* События конечного автомата */
typedef enum {
	noEvent_e,			//Нет события
	// Аппаратные
	//displayTouch_e,	//Нажатие на тачскрин экрана	
	//encoderPress_e,	//Нажатие на кнопку энкодера
	//encoderInc_e,		//Инкремент энкодера
	//encoderDec_e,		//Декремент энкодера
	// Программые
	//alarm1Bell_e,		//Срабатывание будильника 1
	//alarm2Bell_e,		//Срабатывание будильника 2
	
	eventsSize				//Количество событий
} event_t;
/* Статусы событий */
typedef enum {
	noHappen,			//Не произошло
	happen,				//Произошло
	preProcessed,	//Обработано обработчиком прерываний
} eventStates_t;

/* Состояния конечного автомата */
typedef enum {
	standbyMode_s,						//Главный экран, режим ожидания
	timeAndDateSetupMode_s,		//Режим установки времени и даты
	
	statesSize								//Количество состояний														
} currentState_t;

/* Статус вызова функции */
typedef enum {
	initial,
	secondary
} callStatus_t;
/* Структура состояний конечного автомата */
typedef struct {
	void (*function)(callStatus_t status, eventStates_t *eventStates);	//Указатель на функцию
	uint32_t callPeriod;																								//Вызывать функцию не чаще чем callPeriod мс
} state_t;

/* Регистрация состояния */
void registerState(currentState_t cs, state_t s);
/* Установка текущего состояния */
void setCurrentState(currentState_t cs);
/* Один тик диспетчера */
void taskManagerTick(void);

#endif

/* Структура функции обработки событий 
typedef struct {
	void (*function)(event_t event);			//Указатель на функцию
	event_t callEvent;										//Событие, по которому вызывать эту фукнцию
} eventHandler_t; */

//Регистрация события
//void registerEvent(event_t e, eventHandler_t eh);
