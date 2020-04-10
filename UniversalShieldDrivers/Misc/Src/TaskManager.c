#include "TaskManager.h"

//TODO: Добавить задачи
//Переменная текущего состояния
static currentState_t currentState = noState_s;

/* Массив состоянияний конечного автомата */
static state_t states[sizeof(state_t)/sizeof(currentState_t)];

/* Массив событий конечного автомата */
static eventStates_t eventStates[sizeof(event_t)];

/* Массив обработчиков событий конечного автомата */
static eventHandler_t eventHandlers[sizeof(eventHandler_t)/sizeof(event_t)];

/* Регистрация события */
void registerEvent(event_t e, eventHandler_t eh) {
	eventHandlers[e] = eh;
}
/* Регистрация состояния */
void registerState(currentState_t cs, state_t s) {
	states[cs] = s;
}
/* Установка текущего состояния */
void setCurrentState(currentState_t cs) {
	currentState = cs;
}
/* Один тик диспетчера */
void taskManagerTick(void) {
	/* Обработка событий */
	uint8_t eventsCounter = sizeof(eventHandlers)/sizeof(eventHandler_t); //Количество обработчиков событий
	//Перебор массива событий и вызов их обработчиков
	while(eventsCounter--){
		//Если флаг события истиный, то вызов функции обработчика с указанием события вызова
		if(eventStates[eventHandlers[eventsCounter].callEvent]) eventHandlers[eventsCounter].function(eventHandlers[eventsCounter].callEvent);
		//Установка флага "предварительно обработан" чтобы была возможность сбросить флаг
		eventStates[eventHandlers[eventsCounter].callEvent] = preProcessed;
		//Если текущее состояние не пустое и время вызова пришло, то вызов состояния
		static uint32_t lastCallTime = 0xFFFFFF;
		static currentState_t lastState = noState_s; 
		if (currentState && (HAL_GetTick()-lastCallTime >= states[currentState].callPeriod)) {
			lastCallTime = HAL_GetTick();
			//Вызов функции. Если предыдущее состояние отлично от настоящего, то статус initial, иначе secondary
			states[currentState].function((lastState != currentState) ? initial : secondary, eventStates);
			lastState = currentState;
		}
	}
	//Сброс флагов событий
	for(uint8_t i = 0; i < sizeof(event_t); i++) {
		eventStates[i] = noHappen;
	}
}
/* Состояния конечного автомата
+ Нет состояния
+ Главный экран
*/