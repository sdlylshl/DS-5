#ifndef __LED_H
#define __LED_H 

#include "stm32f10x.h"


#define LED_ON(n) GPIOC->BRR|=1<<n
#define LED_OFF(n) GPIOC->BSRR|=1<<n
#define LED_NF(n) GPIOC->ODR^=1<<n

#define KEY_GET(i) ((GPIOB->IDR&(1<<i))?1:0)//��ȡ���� 

void LED_Init(void);
void KEY_Init(void);
u8 KEY_Scan(void); //����������
#endif

