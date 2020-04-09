#ifndef DISPLAYBUTTONS_H_
#define DISPLAYBUTTONS_H_

#include "main.h"
#include "XPT2046.h"
#include "BD663474.h"

//Формы кнопок
enum buttonForm {
	rectangle_f,			//Четырёхугольник
	roundRectangle_f,	//Скруглённый прямоугольник
	circle_f					//Круг
};

//Структура кнопки
typedef struct {
	uint16_t id;						//Индификатор кнопки. Может быть любое число от 0 до 255
	uint16_t posX;				//Позиция кнопки по X. Если это круг, то это X центра
	uint16_t posY;				//Позиция кнопки по Y. Если это круг, то это Y центра
	uint8_t form;					//Форма кнопки
	uint16_t length;			//Длина по X. Если это круг, то используется как радиус
	uint16_t width;				//Ширина по Y. Если это круг, то не используется
	uint16_t borderСolor;	//Цвет линии кнопки
	uint8_t borderdWidth;	//Ширина линии кнопки
	uint16_t fillColor;		//Цвет заливки кнопки
	void (*action)(uint16_t id);	//Указатель на функцию, которая будет выполняться при нажатии на кнопку
} button_t;

void printButtons(button_t *buttons, uint8_t count);
void buttonsTouchHandler(button_t *buttons, uint8_t count, touch_t t);

#endif
