#include "MelodyPlayer.h"

extern TIM_HandleTypeDef buzzerPWMTIM;

#define TIM_FREQ 90000000UL	//Частота тактирования таймера
#define TIM_PERIOD 60000		//Период счёта таймера

static volatile uint32_t noteStartTime;
static volatile uint16_t currentNoteNumber = 0;
static melodyBit_t *m;
static uint16_t melodySize;
static uint8_t pauseFlag = 1;

static void __timSetFreq(uint16_t freq) {
	buzzerPWMTIM.Instance->PSC = TIM_FREQ/TIM_PERIOD-1;						//Делитель таймера
	buzzerPWMTIM.Instance->ARR = 1.0f/(float)freq*TIM_PERIOD-1;		//Период
	buzzerPWMTIM.Instance->CCR1= 1.0f/(float)freq*TIM_PERIOD/2-1;	//Заполнение 50%
	buzzerPWMTIM.Instance->CNT = 0;
	noteStartTime = HAL_GetTick();
}

void melodyPlay(melodyBit_t melody[], uint16_t size) {
	m = melody;
	melodySize = size;
	currentNoteNumber = 0;
	__timSetFreq(m[currentNoteNumber].note);
	pauseFlag = 0;
	HAL_TIM_PWM_Start(&buzzerPWMTIM, buzzerPWMChannel);
}

void melodyTick(void) {
	if (melodySize == 0 || pauseFlag) return;
	
	if(HAL_GetTick()-noteStartTime > m[currentNoteNumber].duration) {
		currentNoteNumber++;
		if(currentNoteNumber > melodySize){
			HAL_TIM_PWM_Stop(&buzzerPWMTIM, buzzerPWMChannel);
			melodySize = 0;
			return;
		}
		__timSetFreq(m[currentNoteNumber].note);
	}
}

void melodyPause(void) {
	HAL_TIM_PWM_Stop(&buzzerPWMTIM, buzzerPWMChannel);
	pauseFlag = 1;
	noteStartTime = HAL_GetTick()-noteStartTime;
}

void melodyResume(void) {
	noteStartTime = HAL_GetTick()-noteStartTime;
	pauseFlag = 0;
	HAL_TIM_PWM_Start(&buzzerPWMTIM, buzzerPWMChannel);
}

uint8_t melodyIsPlaying(void) {
	return !pauseFlag;
}

uint32_t melodyGetDuration(melodyBit_t melody[], uint16_t size) {
	uint32_t duration = 0;
	while(size--) {
		duration += melody[size].duration;
	}
	return duration;
}

uint32_t melodyGetCurrentTime(void) {
	uint32_t time = 0;
	for (uint16_t n = 0; n < currentNoteNumber; n++) {
		time += m[n].duration;
	}
	time += HAL_GetTick()-noteStartTime;
	return time;
}

