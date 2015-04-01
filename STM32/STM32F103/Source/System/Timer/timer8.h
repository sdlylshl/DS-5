#ifndef TIME_8_H
#define TIME_8_H

#include "stm32f10x.h"

extern volatile uint32_t timer8;

extern void TIM8_Start(void);
extern void TIM8_Stop(void);
extern void TIM8_Config(void);
extern uint32_t TIM8_GetDistanceTime(uint32_t pretime);
extern uint32_t TIM8_GetCurrentTime(void);
#endif
