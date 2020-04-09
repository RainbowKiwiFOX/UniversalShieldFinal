#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include "BD663474.h"

void (*printPurpose)(char); //Указатель на функцию печати текста

//Печать символа в UART
void UART_printChar(char c) {
	uint8_t b[1] = {c};
	HAL_UART_Transmit(&huart2, b, 1, 0xFF);
}

//Стандартная функция печатиы
int fputc(int c, FILE * stream) {
	printPurpose(c);
	return c; 
}
//Форматированная печать на дисплей
void TFT_printf(const char * __restrict format, ...) {
	printPurpose = TFT_printChar;
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}
//Форматированная печать в UART
void UART_printf(const char * __restrict format, ...) {
	printPurpose = UART_printChar;
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}