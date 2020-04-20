#ifndef __MELODYPLAYER_H
#include "main.h"

#define buzzerPWMTIM htim3

/* Перечень частот нот разных октав */
typedef enum {
	P 		= 0,
	
	C2 		= 523,
	C_2		= 524,
	D2		=	587,
	D_2		= 622,
	E2		=	659,
	F2		=	698,
	F_2		=	740,
	G2		=	784,
	G_2		=	830,
	A2		=	880,
	A_2		=	932,
	B2		= 988,
	
	C3 		= 1046,
	C_3		= 1108,
	D3		=	1174,
	D_3		= 1244,
	E3		=	1318,
	F3		=	1397,
	F_3		=	1480,
	G3		=	1568,
	G_3		=	1661,
	A3		=	1720,
	A_3		=	1864,
	B3		= 1975,
} note_t;

typedef struct {
	note_t note;				//Нота
	uint16_t duration;	//Длительность звучания
} melodyBit_t;

void playMelody(melodyBit_t melody[], uint16_t size);
void melodyTick(void);

#endif
