#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"

//extern __IO uint32_t time;
//extern __IO uint32_t Zigbee_time;
//extern __IO uint32_t Net_time;
//extern __IO uint32_t time_out;
//extern __IO uint32_t timer_Device_update;
//extern __IO uint32_t timer_Zigbee_getStatus;
//extern __IO uint32_t timer_Zigbee_getBattery;
//extern __IO uint32_t timer_Zigbee_sendBuff;
//extern __IO uint32_t timer_Zigbee_ClearRequest;
//extern __IO uint32_t timer_Zigbee_operate_default;
//#define START_TIME()  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
//#define STOP_TIME()  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

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


#endif	/* TIME_TEST_H */
