#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include "BD663474.h"

void (*printPurpose)(char *ptr, int len); //Указатель на функцию печати текста

//Печать строки в UART
void UART_printStr(char *ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 0xFF);
}
//Печать строки на дисплей
void TFT_printStr(char *ptr, int len) {
	for(int i = 0; i < len; i++) {
		TFT_printChar(ptr[i]);
	}
}
//Переопределение стандартной функции вывода
int _write(int file, char *ptr, int len) {
	printPurpose(ptr, len);
	return len;
}
//Форматированная печать на дисплей
void TFT_printf(const char * __restrict format, ...) {
	printPurpose = TFT_printStr;
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}
//Форматированная печать в UART
void UART_printf(const char * __restrict format, ...) {
	printPurpose = UART_printStr;
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}
