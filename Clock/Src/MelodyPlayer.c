#include "MelodyPlayer.h"

extern TIM_HandleTypeDef buzzerPWMTIM;

//TODO: пауза, стоп, играет ли мелодия, длительность мелодии, текущее время

#define TIM_FREQ 90000000UL	//Частота тактирования таймера
#define TIM_PERIOD 60000		//Период счёта таймера

static volatile uint32_t noteStartTime;
static volatile uint16_t currentNoteNumber = 0;
static melodyBit_t *m;
static uint16_t melodySize;
static void __timInit(void) {
	buzzerPWMTIM.Instance->PSC = TIM_FREQ/TIM_PERIOD-1;	//Делитель таймера
	buzzerPWMTIM.Instance->ARR = TIM_PERIOD-1; 					//Период
	buzzerPWMTIM.Instance->CCR1= TIM_PERIOD/2-1;				//Заполнение
}

static void __timSetFreq(uint16_t freq) {
	buzzerPWMTIM.Instance->ARR = 1.0f/(float)freq*TIM_PERIOD-1;
	buzzerPWMTIM.Instance->CCR1= 1.0f/(float)freq*TIM_PERIOD/2-1;
	buzzerPWMTIM.Instance->CNT = 0;
	noteStartTime = HAL_GetTick();
}

void playMelody(melodyBit_t melody[], uint16_t size) {
	__timInit();
	m = melody;
	melodySize = size;
	currentNoteNumber = 0;
	__timSetFreq(m[currentNoteNumber].note);
	HAL_TIM_PWM_Start(&buzzerPWMTIM, TIM_CHANNEL_1);
}

void melodyTick(void) {
	if (melodySize == 0) return;
	
	if(HAL_GetTick()-noteStartTime > m[currentNoteNumber].duration) {
		currentNoteNumber++;
		if(currentNoteNumber > melodySize){
			HAL_TIM_PWM_Stop(&buzzerPWMTIM, TIM_CHANNEL_1);
			melodySize = 0;
			return;
		}
		__timSetFreq(m[currentNoteNumber].note);
	}
}