#ifndef TIME_1_H
#define TIME_1_H

//#include "stm32f10x.h"

#include "version.h"
//#define TIM1_NVIC	1

extern volatile uint32_t time1;

extern void TIM1_Config(void);
extern void TIM1_Start(void);
extern void TIM1_Stop(void);
extern void TIM1_UP_IRQHandle(void);

void TIM1_CC_IRQHandle(void);
#endif	/* TIME_TEST_H */
