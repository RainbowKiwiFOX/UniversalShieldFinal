#ifndef __UNIVERSALSHIELD_H
#define __UNIVERSALSHIELD_H
#include "main.h"
#include "TaskManager.h"

/*** Прототипы функций ***/
//Главная функция шилда
void US_main(void);
//Форматированная печать на дисплей
void TFT_printf(const char * __restrict format, ...);
//Форматированная печать в UART
void UART_printf(const char * __restrict format, ...);

/* Режим ожидания */
void standbyMode(callStatus_t s, eventStates_t *es);

/* Режим установки времени и даты */
void timeAndDateSetupMode(callStatus_t s, eventStates_t *es);

#endif
