#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

// 子项目引用
// #include "..\System_config.h"
// #include "config.h"

#include "version.h"

//#include "stm32f10x.h"
#include ".\Malloc\malloc.h"
//#include ".\Delay\delay.h"
#include ".\Delay\SysTick.h"

#include ".\CRC\crc.h"

#ifdef CABINET_DEVICE 
#include ".\GPIO\in.h"
#include ".\GPIO\out.h"
#endif // CABINET_DEVICE 

#include "./NVIC.h"
#include ".\SPI\spi.h"
#include ".\SPI\spi1.h"
#include ".\SPI\spi2.h"
#include ".\SPI\spi3.h"

#include ".\flash\SST25_Flash.h"

#include ".\Usart\usart.h"
#include ".\Usart\usart1.h"
#include ".\Usart\usart2.h"
#include ".\Usart\usart3.h"

#include ".\Timer\timer1.h"
#include ".\Timer\timer2.h"
#include ".\Timer\timer3.h"
#include ".\Timer\timer4.h"
#include ".\DAC\DAC.h"
#include ".\RTC\rtc.h"

#include ".\CAN\can1.h"

typedef enum {
  POLLING,
  INTERRUPT,
  DMA
} WorkMode;


extern uint32_t (*GetCurrentTime)(void);
extern uint32_t (*GetDistanceTime)(uint32_t );


uint8_t * GetSID(void);
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,uint32_t PeripheralBaseAddr,uint32_t MemoryBaseAddr,uint32_t BufferSize);
void NVIC_Config(void);
void RCC_Config(void);
//core_cm3.h
//__STATIC_INLINE void NVIC_SystemReset(void);
//__STATIC_INLINE int32_t ITM_ReceiveChar (void);
//__STATIC_INLINE int32_t ITM_CheckChar (void);
//__STATIC_INLINE uint32_t ITM_SendChar (uint32_t ch);
//__STATIC_INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn); //清除中断标志位
//__STATIC_INLINE void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);
//__STATIC_INLINE void NVIC_DisableIRQ(IRQn_Type IRQn); //禁止一个中断
//__STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn); //使能一个中断
////返回一个中断优先级 可作为NVIC_SetPriority（）的参数
//__STATIC_INLINE uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority);
//__STATIC_INLINE uint32_t NVIC_GetActive(IRQn_Type IRQn);
//__STATIC_INLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn);
//__STATIC_INLINE uint32_t NVIC_GetPriority(IRQn_Type IRQn);
//__STATIC_INLINE uint32_t NVIC_GetPriorityGrouping(void);
//__STATIC_INLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn);
//__STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority); //设置一个中断的优先级0~15
//__STATIC_INLINE void NVIC_SetPriorityGrouping(uint32_t PriorityGroup); //设置优先级分组 0~7
//__STATIC_INLINE void NVIC_SystemReset(void);
//__STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks);

#endif
