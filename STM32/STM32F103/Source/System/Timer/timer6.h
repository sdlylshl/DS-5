#ifndef TIME_6_H
#define TIME_6_H

#include "stm32f10x.h"

extern volatile uint32_t time4;

extern void TIM6_Config(void);
extern void TIM6_Start(void);
extern void TIM6_Stop(void);
extern uint32_t TIM6_GetDistanceTime(uint32_t pretime);
extern uint32_t TIM6_GetCurrentTime(void);

#endif
