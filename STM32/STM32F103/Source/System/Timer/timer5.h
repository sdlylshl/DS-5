#ifndef TIME_5_H
#define TIME_5_H

#include "stm32f10x.h"

extern volatile uint32_t timer5;

extern void TIM5_Start(void);
extern void TIM5_Stop(void);
extern void TIM5_Config(void);
extern uint32_t TIM5_GetDistanceTime(uint32_t pretime);
extern uint32_t TIM5_GetCurrentTime(void);

#endif
