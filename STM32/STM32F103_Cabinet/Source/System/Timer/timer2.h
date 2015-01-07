#ifndef TIME_2_H
#define TIME_2_H

#include "version.h"

extern volatile uint32_t time2;
extern volatile uint32_t Zigbee_time;
extern volatile uint32_t Net_time;
extern volatile uint32_t time_out;
extern volatile uint32_t timer_Device_update;
extern volatile uint32_t timer_Zigbee_getStatus;
extern volatile uint32_t timer_Zigbee_getBattery;
extern volatile uint32_t timer_Zigbee_sendBuff;
extern volatile uint32_t timer_Zigbee_ClearRequest;
extern volatile uint32_t timer_Zigbee_operate_default;
extern volatile uint32_t timer_Zigbee_reset_default;
extern volatile uint32_t timer_curtain_stop;

extern void TIM2_Init(void);
extern void TIM2_Start(void);
extern void TIM2_Stop(void);

extern void TIM2_ISR(void);

uint32_t TIM2_GetDistanceTime(uint32_t pretime);

uint32_t TIM2_GetCurrentTime(void);

#endif	/* TIME_TEST_H */
