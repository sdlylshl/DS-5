#ifndef _CAN_H_
#define _CAN_H_

#include "stm32f10x.h"

/*采用查询方式还是中断方式接收数据开关，屏闭为中断方式*/
#define  Rx_Polling	1
#define Test_Tx
#define Test_Rx
#define CAN_BTR  800

#ifndef Rx_Polling
#define  Rx_Interrupt	1
#endif


#define CAN_MODE CAN_Mode_LoopBack


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


void CAN1_Init(void);
//void USB_HP_CAN_TX_IRQHandle(void);
void USB_LP_CAN_RX0_IRQHandle(void);
void CAN_RX1_IRQHandle(void);
void CAN_SCE_IRQHandle(void);
#endif
