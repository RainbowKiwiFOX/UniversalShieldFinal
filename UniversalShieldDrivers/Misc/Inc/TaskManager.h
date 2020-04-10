#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "main.h"

/* События конечного автомата */
typedef enum {
	noEvent_e,			//Нет события
	/* Аппаратные */
	displayTouch_e,	//Нажатие на тачскрин экрана	
	encoderPress_e,	//Нажатие на кнопку энкодера
	encoderInc_e,		//Инкремент энкодера
	encoderDec_e,		//Декремент энкодера
	/* Программые */
	alarm1Bell_e,		//Срабатывание будильника 1
	alarm2Bell_e,		//Срабатывание будильника 2
} event_t;
/* Состояния конечного автомата */
#define noState_s 0
typedef enum {
	standbyMode_s = 1,		//Главный экран, режим ожидания
} currentState_t;

/* Статус вызова функции */
typedef enum {
	initial,
	secondary
} callStatus_t;
/* Статусы событий */
typedef enum {
	noHappen,			//Не произошло
	happen,				//Произошло
	preProcessed,	//Обработано обработчиком прерываний
} eventStates_t;
/* Структура состояний конечного автомата */
typedef struct {
	void (*function)(callStatus_t status, eventStates_t *eventStates);	//Указатель на функцию
	uint32_t callPeriod;																					//Вызывать функцию не чаще чем callPeriod мс
} state_t;
/* Структура функции обработки событий */
typedef struct {
	void (*function)(event_t event);			//Указатель на функцию
	event_t callEvent;										//Событие, по которому вызывать эту фукнцию
} eventHandler_t;

//Регистрация события
void registerEvent(event_t e, eventHandler_t eh);
/* Регистрация состояния */
void registerState(currentState_t cs, state_t s);
/* Установка текущего состояния */
void setCurrentState(currentState_t cs);
/* Один тик диспетчера */
void taskManagerTick(void);

#endif
