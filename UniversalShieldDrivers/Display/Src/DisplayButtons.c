#include "DisplayButtons.h"
//Функция печати кнопок на экране
void printButtons(button_t *buttons, uint8_t count) {
	while(count--) {
		//Вызов функции рисования в зависимости от формы кнопки
		switch(buttons[count].form) {
			case rectangle_f: //Отрисовка прямоугольника
				if(buttons[count].borderdWidth != 0)
				TFT_drawRectangle(buttons[count].posX, buttons[count].posY, 
													buttons[count].length, buttons[count].width, 
													buttons[count].borderdWidth, buttons[count].borderСolor);
			//Если есть внутренняя заливка, то её тоже нужно отрисовать
				if(buttons[count].fillColor != TFT_COLOR_none)
					TFT_fillRectangle(buttons[count].posX+buttons[count].borderdWidth, buttons[count].posY+buttons[count].borderdWidth, 
													buttons[count].length-2*buttons[count].borderdWidth, buttons[count].width-2*buttons[count].borderdWidth, 
													buttons[count].fillColor);
				break;
			case roundRectangle_f:
				if(buttons[count].borderdWidth != 0)
				TFT_drawRoundRect(buttons[count].posX, buttons[count].posY,
													buttons[count].length, buttons[count].width,
													buttons[count].borderdWidth*2.5f, buttons[count].borderdWidth,
													buttons[count].borderСolor);
				//Если есть внутренняя заливка, то её тоже нужно отрисовать
				if(buttons[count].fillColor != TFT_COLOR_none)
				TFT_fillRoundRect(buttons[count].posX+buttons[count].borderdWidth, buttons[count].posY+buttons[count].borderdWidth,
													buttons[count].length-2*buttons[count].borderdWidth, buttons[count].width-2*buttons[count].borderdWidth,
													buttons[count].borderdWidth*2.5f, buttons[count].fillColor);
				break;
			case circle_f:
				if(buttons[count].borderdWidth != 0)
				TFT_drawCircle(buttons[count].posX, buttons[count].posY,
													buttons[count].length, buttons[count].borderdWidth, 
													buttons[count].borderСolor);
				if(buttons[count].fillColor != TFT_COLOR_none)
				TFT_fillCircle(buttons[count].posX, buttons[count].posY,
													buttons[count].length-buttons[count].borderdWidth, 
													buttons[count].fillColor);
				break;
		}
	}
}

//TODO: Статусы нажатия на тачскрин
//Функция обработки нажатий на экран
void buttonsTouchHandler(button_t *buttons, uint8_t count, touch_t t) {
	//Циклическая проверка координат нажатия и кнопки	
	while(count--) {
		if(
			t.state == T_pressed																		//Если кнопка в состоянии "нажат" (не удержан!)
			&& (t.x >= buttons[count].posX)  												//Позиция X больше или равна началу кнопки
			&& (t.x <= buttons[count].posX+buttons[count].length) 	//Позиция X меньше или равна концу кнопки
			&& (t.y >= buttons[count].posY)  												//Позиция Y больше или равна началу кнопки
			&& (t.y <= buttons[count].posY+buttons[count].width)		//Позиция Y меньше или равна концу кнопки
		) {
			buttons[count].action(buttons[count].id); //Вызов функции обработки нажатия на кнопку
		}
	}
}
