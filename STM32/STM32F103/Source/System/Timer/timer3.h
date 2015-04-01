#ifndef TIME_3_H
#define TIME_3_H

#include "stm32f10x.h"

extern volatile uint32_t time3;

extern void TIM3_Init(void);

extern void TIM3_Start(void);
extern void TIM3_Stop(void);

extern void TIM3_IRQHandle(void);
#endif	/* TIME_TEST_H */
