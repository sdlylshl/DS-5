#ifndef TIME_7_H
#define TIME_7_H

#include "stm32f10x.h"

extern volatile uint32_t timer7;

extern void TIM7_Start(void);
extern void TIM7_Stop(void);
extern void TIM7_Config(void);
extern uint32_t TIM7_GetDistanceTime(uint32_t pretime);
extern uint32_t TIM7_GetCurrentTime(void);

#endif
