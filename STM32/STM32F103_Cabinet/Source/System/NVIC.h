#ifndef _NVIC_H_
#define _NVIC_H_
#include "version.h"
void NVIC_EXTI0_init(void);
void NVIC_EXTI1_init(void);
void NVIC_EXTI2_init(void);
void NVIC_EXTI3_init(void);
void NVIC_EXTI4_init(void);
void NVIC_EXTI5_init(void);
void NVIC_EXTI6_init(void);
void NVIC_EXTI7_init(void);
void NVIC_EXTI8_init(void);
void NVIC_EXTI9_init(void);
void NVIC_EXTI10_init(void);
void NVIC_EXTI11_init(void);
void NVIC_EXTI12_init(void);
void NVIC_EXTI13_init(void);
void NVIC_EXTI14_init(void);
void NVIC_EXTI15_init(void);

void NVIC_Config(void);
void EXTI1_IRQHandle(void);
void EXTI9_5_IRQHandle(void);

#endif
