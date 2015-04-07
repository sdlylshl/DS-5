/*
 * Beep.h
 *
 *  Created on: 2015年3月17日
 *      Author: souls
 */

#ifndef BEEP_H_
#define BEEP_H_
#include "stm32f10x.h"
//PC14 面板
#define  BEEP_USE					 1
#define  BEEP_PORT     GPIOC
#define  BEEP_PIN      GPIO_Pin_14
#define  BEEP_MODE     GPIO_Mode_Out_PP

#define    BEEPONOFF(x)   x ? GPIO_SetBits(BEEP_PORT,BEEP_PIN):  GPIO_ResetBits(BEEP_PORT,BEEP_PIN)
#define    BEEPON()   GPIO_SetBits(BEEP_PORT,BEEP_PIN)
#define    BEEPOFF()  GPIO_ResetBits(BEEP_PORT,BEEP_PIN)

extern void Beep(uint32_t ms);
extern void BeepStop(void);
extern void BeepStart(uint32_t ms);
extern void (*BeepCallback_ISR)(void); //回调函数
#endif /* BEEP_H_ */
