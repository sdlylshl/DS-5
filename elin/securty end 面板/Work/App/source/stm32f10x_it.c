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
#include "config.h"
#include "util.h"
#include "flash.h"
#include "gsm.h"
#include "APPs\loopback.h"

#include <stdio.h>

#include "stdarg.h"	 	 
 	 
#include "string.h"	
#include "i2c_ee.h"


extern sys_para sys;

#define MAX_SERIAL_PARA_BUF 256
u8 buf[MAX_SERIAL_PARA_BUF] ;
u8 read = 0;
u8 write = 0;
u8 cmd[32];

u8 find_one_cmd=0;
u8 u2_buf[512];

#define MAX_GSM_BUF 512
u8 gsm_buf[MAX_GSM_BUF] ;
u16 gsm_read = 0;
u16 gsm_write = 0;
u8 gsm_cmd[64];


u8 rs485_buf[MAX_SERIAL_PARA_BUF] ;
u8 rs485_read = 0;
u8 rs485_write = 0;
u8 rs485_cmd[32];


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//extern vu32 Timer2_Counter;
//extern u32 my_time;

extern void Timer3_ISR(void);

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

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
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

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	#if 0
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		Timer2_ISR();
		
	}
	#endif
}
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		Timer3_ISR();
		
	}
}





/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)  
	{		 
		 //USART_SendData(USART1, USART_ReceiveData(USART1)); 
		c=USART1->DR;
		if (c != ' ')
		{
			buf[write] = c;
				write++;
		}
	}
}


void USART2_IRQHandler(void)
{	
	#if 0
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
	{		 
		 USART_SendData(USART2, USART_ReceiveData(USART2)); 			
	}
	#endif
	#if 1
	u8 c;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
	{		 
		c=USART2->DR;
		
		//if (c != ' ')
		{
			rs485_buf[rs485_write] = c;
			rs485_write++;
		}
		//printf("get cmd: 0x%x", c);
	}
	#endif
}

int show_debug_msg(int len)
{	
	int i,lread;
	
	printf("******************************\ngsm_write: %d, gsm_read: %d\n", rs485_write, rs485_read);
	lread = rs485_read;
	#if 0
	for(i=0; i<len; i++)
	{	
		//printf("%d: ", i);
		if(lread+i > MAX_SERIAL_PARA_BUF-1)
			printf("%c", rs485_buf[lread+i-MAX_SERIAL_PARA_BUF]);
		else
			printf("%c", rs485_buf[lread+i]);
	}
	printf("\n");
	#endif
	for(i=0; i<len; i++)
	{	
		//printf("%d: ", i);
		//if(lread+i > MAX_SERIAL_PARA_BUF-1)
		//	printf("0x%02x ", rs485_buf[lread+i-MAX_SERIAL_PARA_BUF]);
		//else
			printf("0x%02x ", rs485_buf[lread+i]);
	}
	printf("\n******************************\n");
	return OK;
}


void parse_rs485_cmd(int* ret)
{
	u8 len, i;
	u8 cmd[5];
	*ret = 10;
	
	len = (rs485_write+MAX_SERIAL_PARA_BUF-rs485_read)%MAX_SERIAL_PARA_BUF;
	
	if(len>4)
	{
		show_debug_msg(len);
		if(rs485_buf[rs485_read] == 0xfc)
		{
			for(i=0; i<5; i++)
			{
				cmd[i] = rs485_buf[rs485_read];
				rs485_read++;
			}
			if(cmd[2] == 0)
			{
				*ret = 0;
				printf("get arm order\n");
			}
			else if(cmd[2] == 1)
			{
				*ret = 1;
				printf("get disarm order\n");
			}
			else if(cmd[2] == 2)
			{
				*ret = 2;
				//printf("get POLL order\n");
			}
		}
		else
			rs485_read++;
	}
	//return ret;
}






/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{	
	#if 0
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
	{		 
		 USART_SendData(USART2, USART_ReceiveData(USART2)); 			
	}
	#endif
	#if 1
	u8 c;
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)  
	{		 
		c=USART3->DR;
		
		
		
	}
	#endif
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
