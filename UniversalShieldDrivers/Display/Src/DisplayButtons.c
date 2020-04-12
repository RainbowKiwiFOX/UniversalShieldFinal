#include "DisplayButtons.h"
//Функция печати кнопок на экране
void printButtons(visualButton_t *buttons, uint8_t count) {
	while(count--) {
		//Вызов функции рисования в зависимости от формы кнопки
		switch(buttons[count].form) {
			case rectangle_f: //Отрисовка прямоугольника
				if((buttons[count].borderdWidth != 0) && (buttons[count].borderСolor != TFT_COLOR_none))
				TFT_drawRectangle(buttons[count].button.posX, buttons[count].button.posY, 
													buttons[count].button.length, buttons[count].button.width, 
													buttons[count].borderdWidth, buttons[count].borderСolor);
			//Если есть внутренняя заливка, то её тоже нужно отрисовать
				if(buttons[count].fillColor != TFT_COLOR_none)
					TFT_fillRectangle(buttons[count].button.posX+buttons[count].borderdWidth, buttons[count].button.posY+buttons[count].borderdWidth, 
													buttons[count].button.length-2*buttons[count].borderdWidth, buttons[count].button.length-2*buttons[count].borderdWidth, 
													buttons[count].fillColor);
				break;
			case roundRectangle_f:
				if(buttons[count].borderdWidth != 0)
				TFT_drawRoundRect(buttons[count].button.posX, buttons[count].button.posY,
													buttons[count].button.length, buttons[count].button.length,
													buttons[count].borderdWidth*2.5f, buttons[count].borderdWidth,
													buttons[count].borderСolor);
				//Если есть внутренняя заливка, то её тоже нужно отрисовать
				if(buttons[count].fillColor != TFT_COLOR_none)
				TFT_fillRoundRect(buttons[count].button.posX+buttons[count].borderdWidth, buttons[count].button.posY+buttons[count].borderdWidth,
													buttons[count].button.length-2*buttons[count].borderdWidth, buttons[count].button.length-2*buttons[count].borderdWidth,
													buttons[count].borderdWidth*2.5f, buttons[count].fillColor);
				break;
			case circle_f:
				if(buttons[count].borderdWidth != 0)
				TFT_drawCircle(buttons[count].button.posX, buttons[count].button.posY,
													buttons[count].button.length, buttons[count].borderdWidth, 
													buttons[count].borderСolor);
				if(buttons[count].fillColor != TFT_COLOR_none)
				TFT_fillCircle(buttons[count].button.posX, buttons[count].button.posY,
													buttons[count].button.length-buttons[count].borderdWidth, 
													buttons[count].fillColor);
				break;
		}
	}
}

//TODO: Норм обработка круглых кнопок
//Функция обработки нажатий на экран
void buttonsTouchHandler(button_t *buttons, uint8_t count, touch_t t) {
	if(t.state == T_noTouch) return;
	//Циклическая проверка координат нажатия и кнопки	
	while(count--) {
		if(
			   (t.x >= buttons[count].posX)  												//Позиция X больше или равна началу кнопки
			&& (t.x <= buttons[count].posX+buttons[count].length) 	//Позиция X меньше или равна концу кнопки
			&& (t.y >= buttons[count].posY)  												//Позиция Y больше или равна началу кнопки
			&& (t.y <= buttons[count].posY+buttons[count].width)		//Позиция Y меньше или равна концу кнопки
		) {
			buttons[count].action(buttons[count].id, t.state); //Вызов функции обработки нажатия на кнопку
		}
	}
}

//Функция визуализации обычной кнопки
void visualizeButton(button_t button, uint16_t color) {
	TFT_drawRectangle(button.posX,button.posY, button.length, button.width, 1, color);
}
