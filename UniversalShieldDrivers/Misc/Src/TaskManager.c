#include "TaskManager.h"

//TODO: Добавить задачи

/* Состояния */
//Переменная текущего состояния. По умолчанию - самое первое событие.
static currentState_t currentState = 0;
//Время последнего вызова состояния
static uint32_t lastCallTime = 0xFFFFFF; 
//Массив состояний конечного автомата. Размер зависит от количества элементов в перечислении currentState_t
static state_t states[statesSize];
// Регистрация состояния
void registerState(currentState_t cs, state_t s) {
	states[cs] = s;
}
// Установка текущего состояния
void setCurrentState(currentState_t cs) {
	currentState = cs;
	lastCallTime = 0xFFFFFF;
}
// Получить текущее событие
currentState_t getCurrentState(void) {
	return currentState;
}

/* События */
// Массив событий конечного автомата
static eventState_t eventStates[eventsSize];
// Массив обработчиков событий конечного автомата
static eventHandler_t eventHandlers[eventsSize];

/* Регистрация события */
void registerEvent(eventHandler_t eh) {
	eventHandlers[eh.callEvent] = eh;
}
/* Установка состояния события (событие состоялось, например)*/
void setEventState(event_t e, eventState_t state) {
	eventStates[e] = state;
}

/* Один тик диспетчера */
void taskManagerTick(void) {
	/* Задачи диспетчера:
	1) Проверить все зарегистрированые события
	2) Запустить все зарегистрированые задачи
	3) Перейти в текущее состояние
	4) Сбросить флаги событий
	*/
	/* 1. Вызов обработчиков событий */
	for(uint8_t i = 0; i < eventsSize; i++) {
		//Если событие обработчика в истине, то вызов обработчика
		if(eventStates[eventHandlers[i].callEvent] != noHappen) {
			//Вызов функции обработки
			eventHandlers[i].function(eventHandlers[i].callEvent);
			//Событие будет сброшено при следующей итерации если не произойдёт снова
			eventStates[eventHandlers[i].callEvent] = preProcessed; 		
		}
	}
	/* 3. Переход в текущее состояние */
	//Проверка наличия указателя состояния в массиве и время последнего вызова
	if (HAL_GetTick()-lastCallTime >= states[currentState].callPeriod) {
		//Если время последнего вызова 0xFFFFFF, то вызов первичный, иначе вторичный
		callStatus_t state = (lastCallTime == 0xFFFFFF) ? initial : secondary;
		//Обновление времени последнего вызова
		lastCallTime = HAL_GetTick(); 
		//Вызов функции текущего состояния
		states[currentState].function(state, eventStates);
	}
	
	/* 4. Сброс флагов событий */
	for(uint8_t i = 0; i < eventsSize; i++) {
		if(eventStates[i] == preProcessed) eventStates[i] = noHappen;
	}
}
