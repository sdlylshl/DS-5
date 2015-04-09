/**
 ******************************************************************************
 * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"

#include <stdio.h>
/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

__INLINE void NMI_Handle(void) {
}
__INLINE void HardFault_Handle(void) {
}
__INLINE void MemManage_Handle(void) {
}
__INLINE void BusFault_Handle(void) {
}
__INLINE void UsageFault_Handle(void) {
}
__INLINE void SVC_Handle(void) {
}
__INLINE void DebugMon_Handle(void) {
}
__INLINE void PendSV_Handle(void) {
}
//__INLINE void SysTick_Handle(void){}

//__INLINE void TIM2_IRQHandle(void){}

//__INLINE void USART1_IRQHandle(void){}
//__INLINE void USART2_IRQHandle(void){}
//__INLINE void USART3_IRQHandle(void){}

extern void EXTI1_IRQHandle(void);
extern void RTC_IRQHandle(void);
void EXTI9_5_IRQHandle(void);
void TIM2_IRQHandle(void);
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
#ifdef DEBUG
	printf("NMI_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
#ifdef DEBUG
	printf("HardFault_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
	/* Go to infinite loop when Hard Fault exception occurs */

}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void) {
#ifdef DEBUG
	printf("MemManage_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
	/* Go to infinite loop when Memory Manage exception occurs */

}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void) {
#ifdef DEBUG
	printf("BusFault_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
	/* Go to infinite loop when Bus Fault exception occurs */

}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void) {
#ifdef DEBUG
	printf("UsageFault_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
	/* Go to infinite loop when Usage Fault exception occurs */

}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
#ifdef DEBUG
	printf("SVC_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {
#ifdef DEBUG
	printf("DebugMon_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
#ifdef DEBUG
	printf("PendSV_Handler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}


/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void WWDG_IRQHandler() {
#ifdef DEBUG
	printf("WWDG_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void PVD_IRQHandler() {
#ifdef DEBUG
	printf("PVD_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void TAMPER_IRQHandler() {
#ifdef DEBUG
	printf("TAMPER_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void RTC_IRQHandler() {
#ifdef DEBUG
	printf("RTC_IRQHandler \r\n");
#endif
	RTC_IRQHandle();
}
void FLASH_IRQHandler() {
#ifdef DEBUG
	printf("FLASH_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void RCC_IRQHandler() {
#ifdef DEBUG
	printf("RCC_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}


void ADC1_2_IRQHandler() {
#ifdef DEBUG
	printf("ADC1_2_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void I2C1_EV_IRQHandler() {
#ifdef DEBUG
	printf("I2C1_EV_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void I2C1_ER_IRQHandler() {
#ifdef DEBUG
	printf("I2C1_ER_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void I2C2_EV_IRQHandler() {
#ifdef DEBUG
	printf("I2C2_EV_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void I2C2_ER_IRQHandler() {
#ifdef DEBUG
	printf("I2C2_ER_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}

//在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求，
//因此必须在中断服务程序中对中断源进行判别，然后分别进行处理。
//当然，如果只涉及到一个中断请求，是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯
/*
//usart1 === net
void USART1_IRQHandler(void) {
#ifdef DEBUG
	printf("USART1_IRQHandler \r\n");
#endif
	USART1_IRQHandle();

}
//usart2 === zigbee
void USART2_IRQHandler(void) {
#ifdef DEBUG
	printf("USART2_IRQHandler \r\n");
#endif
	USART2_IRQHandle();
}

void USART3_IRQHandler(void) {
#ifdef DEBUG
	printf("USART3_IRQHandler \r\n");
#endif
	USART3_IRQHandle();
}
*/

void RTCAlarm_IRQHandler() {
#ifdef DEBUG
	printf("RTCAlarm_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
void USBWakeUp_IRQHandler() {
#ifdef DEBUG
	printf("USBWakeUp_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
void PPP_IRQHandler(void) {

#ifdef DEBUG
	printf("PPP_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
