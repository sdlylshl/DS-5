#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"
#include "Types.h"

extern Boolean flag_inteval_1000;
extern Boolean flag_inteval_500;

extern void TIM2_Configuration(void);
extern void TIM2_IRQHandle(void);
extern void TIM3_Configuration(void);
extern void TIM4_Configuration(void);

extern uint32_t get_curTime(void);
extern uint32_t get_count(void);


extern volatile uint32_t time4;

extern void TIM4_Config(void);
extern void TIM4_Start(void);
extern void TIM4_Stop(void);
uint32_t TIM4_GetDistanceTime(uint32_t pretime);
uint32_t TIM4_GetCurrentTime(void);
extern void TIM4_IRQHandle(void);
extern void reset_heart_signal_cnt(void);

void timer_init(void);
#endif	/* TIME_TEST_H */
