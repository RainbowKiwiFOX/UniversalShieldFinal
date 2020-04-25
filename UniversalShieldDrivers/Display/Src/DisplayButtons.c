#include "DisplayButtons.h"

//TODO: Норм обработка круглых кнопок
//TODO: Добавить треугольные кнопки
//TODO: Кнопки с рисунками
//TODO: Кнопки с текстом
//TODO: Кнопки с кастомной отрисовкой по вызову функции

//Функция проверки наличия точки нажатия внутри кнопки
uint8_t checkPosition(button_t b, touch_t t) {
			return	 (t.x >= b.posX)  		//Позиция X больше или равна началу кнопки
			&& (t.x <= b.posX+b.length) 	//Позиция X меньше или равна концу кнопки
			&& (t.y >= b.posY)  					//Позиция Y больше или равна началу кнопки
			&& (t.y <= b.posY+b.width);		//Позиция Y меньше или равна концу кнопки
}

//Функция печати массива кнопок на экране
void printButtons(visualButton_t *buttons, uint8_t count) {
	while(count--) {
		printButton(buttons[count], 0);
	}
}

//Функция изменения контура кнопки при нажатии или отпускании
void checkActiveButtons(visualButton_t *buttons, uint8_t count, touch_t t) {
	while(count--) {
		//Если точка внутри кнопки, то рисование контура в зависимости от состояния нажатия
		if(checkPosition(*buttons[count].button, t)) {
			if(t.state == T_pressed) printButton(buttons[count], 1); //Рисование активного контура
			if(t.state == T_released) printButton(buttons[count], 0); //Рисование обычного контура
		}			
	}
}

//Функция обработки нажатий на экран
void buttonsTouchHandler(button_t *buttons, uint8_t count, touch_t t) {
	if(t.state == T_noTouch) return;
	//Циклическая проверка координат нажатия и кнопки	
	while(count--) {
		if(checkPosition(buttons[count], t)) {
			buttons[count].action(buttons[count].id, t.state); //Вызов функции обработки нажатия на кнопку
		}
	}
}

//Функция печати контура и фона кнопки
void printButton(visualButton_t button, uint8_t activeStatus) {
	//Вызов функции рисования в зависимости от формы кнопки
	switch(button.form) {
		case rectangle_f: //Отрисовка прямоугольника
			//Рисование контура
			if((button.borderdWidth != 0) && (button.borderColor != TFT_COLOR_none || button.activeBorderColor != TFT_COLOR_none))
			TFT_drawRectangle(button.button->posX, button.button->posY, 
												button.button->length, button.button->width, 
												button.borderdWidth, activeStatus ? button.activeBorderColor : button.borderColor);
		//Если есть внутренняя заливка, то её тоже нужно отрисовать
			if(button.fillColor != TFT_COLOR_none)
				TFT_fillRectangle(button.button->posX+button.borderdWidth, button.button->posY+button.borderdWidth, 
												button.button->length-2*button.borderdWidth, button.button->length-2*button.borderdWidth, 
												button.fillColor);
			break;
		case roundRectangle_f:
			if((button.borderdWidth != 0) && (button.borderColor != TFT_COLOR_none || button.activeBorderColor != TFT_COLOR_none))
			TFT_drawRoundRect(button.button->posX, button.button->posY,
												button.button->length, button.button->width,
												button.borderdWidth*2.5f, button.borderdWidth,
												activeStatus ? button.activeBorderColor : button.borderColor);
			//Если есть внутренняя заливка, то её тоже нужно отрисовать
			if(button.fillColor != TFT_COLOR_none)
			TFT_fillRoundRect(button.button->posX+button.borderdWidth, button.button->posY+button.borderdWidth,
												button.button->length-2*button.borderdWidth, button.button->length-2*button.borderdWidth,
												button.borderdWidth*2.5f, button.fillColor);
			break;
		case circle_f:
			if((button.borderdWidth != 0) && (button.borderColor != TFT_COLOR_none || button.activeBorderColor != TFT_COLOR_none))
			TFT_drawCircle(button.button->posX, button.button->posY,
												button.button->length, button.borderdWidth, 
												activeStatus ? button.activeBorderColor : button.borderColor);
			if(button.fillColor != TFT_COLOR_none)
			TFT_fillCircle(button.button->posX, button.button->posY,
												button.button->length-button.borderdWidth, 
												button.fillColor);
			break;
	}
}

//Функция визуализации обычной кнопки
void visualizeButton(button_t button, uint16_t color) {
	TFT_drawRectangle(button.posX,button.posY, button.length, button.width, 1, color);
}
