#ifndef _CAN_H_
#define _CAN_H_

#include "stm32f10x.h"

#define CAN_BTR  800
//**************************************************
//高电平 silent mode (监听模式)
//悬空或接低电平(默认)high speed mode
#define CAN1_MODE_USED			//启用TJA1050 S端口功能
#ifdef CAN1_MODE_USED
#define CAN1_MODE_PORT			GPIOA
#define CAN1_MODE_PIN				GPIO_Pin_8
#define CAN1_MODE_MODE			GPIO_Mode_Out_PP
#define CAN1_S_HIGH()         GPIO_SetBits(CAN1_MODE_PORT, CAN1_MODE_PIN)
#define CAN1_S_LOW()         GPIO_ResetBits(CAN1_MODE_PORT, CAN1_MODE_PIN)
#endif
//**************************************************
#define CAN1_NVIC
//终端面板V1.3 PA11 PA12
#define CAN1_NONREMAP
//智能储物柜HOST V1.1 PB8 PB9 
//#define CAN1_PARTIALREMAP
//#define CAN1_FULLREMAP
//复用功能
//PA11  PB8  PD0  CAN1_RX 或 AN_RX
//PA12  PB9  PD1  CAN1_TX 或 AN_TX
#ifdef CAN1_NONREMAP
  // CAN1 PA 11 12	
#define CAN1_RX_PORT			GPIOA
#define CAN1_RX_PIN				GPIO_Pin_11
#define CAN1_RX_MODE			GPIO_Mode_IPU
	
#define CAN1_TX_PORT			GPIOA
#define CAN1_TX_PIN				GPIO_Pin_12
#define CAN1_TX_MODE			GPIO_Mode_AF_PP	
#endif

#ifdef CAN1_PARTIALREMAP
  // CAN1 PB 8 9	
#define CAN1_RX_PORT			GPIOB
#define CAN1_RX_PIN				GPIO_Pin_8
#define CAN1_RX_MODE			GPIO_Mode_IPU
	
#define CAN1_TX_PORT			GPIOB
#define CAN1_TX_PIN				GPIO_Pin_9
#define CAN1_TX_MODE			GPIO_Mode_AF_PP	
#endif

#ifdef CAN1_FULLREMAP
  // CAN1 PD 0 1
#define CAN1_RX_PORT			GPIOD
#define CAN1_RX_PIN				GPIO_Pin_0
#define CAN1_RX_MODE			GPIO_Mode_IPU
	
#define CAN1_TX_PORT			GPIOD
#define CAN1_TX_PIN				GPIO_Pin_1
#define CAN1_TX_MODE			GPIO_Mode_AF_PP	
#endif
//**************************************************

void CAN1_Init(void);
//void USB_HP_CAN_TX_IRQHandle(void);
void USB_LP_CAN_RX0_IRQHandle(void);
void CAN_RX1_IRQHandle(void);
void CAN_SCE_IRQHandle(void);
#endif
