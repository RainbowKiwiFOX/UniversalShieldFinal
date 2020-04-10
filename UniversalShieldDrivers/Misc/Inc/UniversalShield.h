#ifndef __UNIVERSALSHIELD_H
#define __UNIVERSALSHIELD_H
#include "main.h"

//Главная функция шилда
void US_main(void);
//Форматированная печать на дисплей
void TFT_printf(const char * __restrict format, ...);
//Форматированная печать в UART
void UART_printf(const char * __restrict format, ...);

#endif
