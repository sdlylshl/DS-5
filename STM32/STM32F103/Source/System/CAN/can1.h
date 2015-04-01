#ifndef _CAN_H_
#define _CAN_H_

#include "version.h"


/*采用查询方式还是中断方式接收数据开关，屏闭为中断方式*/
#define  Rx_Polling	1
#define Test_Tx
#define Test_Rx
#define CAN_BTR  800

#ifndef Rx_Polling
#define  Rx_Interrupt	1
#endif


#define CAN_MODE CAN_Mode_LoopBack

typedef enum {FAILED = 0, PASSED = !FAILED}TestStatus;
typedef enum {KeyPressed=0,KeyRelease = !KeyPressed}KeyStatus;

#define TJA1054AT_STB  GPIO_Pin_8
#define TJA1054AT_EN   GPIO_Pin_9

#define CAN1_NVIC

//#define CAN1_NONREMAP
#define CAN1_PARTIALREMAP
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



TestStatus CAN_Polling(void);
void CAN_Interrupt(void);
void Can_Init(void);
void CAN_main(void);
void CAN1_Config(void) ;
void USB_HP_CAN_TX_IRQHandle(void);
void USB_LP_CAN_RX0_IRQHandle(void);
void CAN_RX1_IRQHandle(void);
void CAN_SCE_IRQHandle(void);
#endif
