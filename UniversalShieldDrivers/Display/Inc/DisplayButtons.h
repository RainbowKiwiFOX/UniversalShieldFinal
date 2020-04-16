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
//Структура визуальной кнопки
typedef struct {
	uint16_t id;					//Индификатор кнопки. Может быть любое число от 0 до 65535
	uint16_t posX;				//Позиция кнопки по X. Если это круг, то это X центра
	uint16_t posY;				//Позиция кнопки по Y. Если это круг, то это Y центра
	uint16_t length;			//Длина по X. Если это круг, то используется как радиус
	uint16_t width;				//Ширина по Y. Если это круг, то не используется
	//Указатель на функцию, которая будет выполняться при нажатии на кнопку
	void (*action)(uint16_t id, touchStates ts);
} button_t;
//Структура кнопки
typedef struct {
	button_t *button;						//Основная кнопка
	uint8_t form;								//Форма кнопки
	uint16_t borderСolor;				//Цвет контура кнопки
	uint16_t activeBorderColor;	//Цвет контура при нажатии на неё
	uint8_t borderdWidth;				//Ширина контура кнопки
	uint16_t fillColor;					//Цвет заливки кнопки
} visualButton_t;

void printButtons(visualButton_t *buttons, uint8_t count);
void buttonsTouchHandler(button_t *buttons, uint8_t count, touch_t t);
void visualizeButton(button_t button, uint16_t color);
void printButton(visualButton_t button, uint8_t activeStatus);
void checkActiveButtons(visualButton_t *buttons, uint8_t count, touch_t t);

#endif
