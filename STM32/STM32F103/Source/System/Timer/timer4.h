#ifndef TIME_4_H
#define TIME_4_H

#include "stm32f10x.h"

extern volatile uint32_t time4;

extern void TIM4_Config(void);
extern void TIM4_Start(void);
extern void TIM4_Stop(void);
extern uint32_t TIM4_GetDistanceTime(uint32_t pretime);
extern uint32_t TIM4_GetCurrentTime(void);

#endif	/* TIME_TEST_H */
