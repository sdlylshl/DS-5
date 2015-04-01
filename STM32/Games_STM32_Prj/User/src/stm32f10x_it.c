/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Global.h"
#include "Function.h"
#include "Small_LCD.h"


volatile t_timeTick_ms g_Timer_ms = 0;		// ���ֵ49.710269618055555555555555555556��

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
		_DbgPrintf_(( "���CpuHrdFlt\r\n" ));
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  	
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
		_DbgPrintf_(( "���CpuBusFlt\r\n" ));
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
	  _DbgPrintf_(( "���CpuUsgFlt\r\n" ));
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


//uint32 g_Timer_ms = 0;
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	register uint32 u32r;
	
	// ��ʱ��++
	u32r = g_Timer_ms + 1;
	g_Timer_ms = u32r;
	_DBG_( if(u32r % 1000 == 0) 
			LCD_SdIntData(u32r / 1000, 5, 0, 5) );
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/*********************************************************************************************************
** ��������: EXTI9_5_IRQHandler
** ��������: �����ж�9:5�жϷ������
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-4-23 10:15
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	
}

/*********************************************************************************************************
** ��������: EXTI9_5_IRQHandler
** ��������: �����ж�9:5�жϷ������
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-4-23 10:22
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
