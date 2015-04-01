
#ifndef __STM32_TYPE1__H
#define __STM32_TYPE1__H

//#define  GPIO_SetBits(GPIOx,BITs)	GPIOx->BSRR = BITs			/* 设置端口位 */
//#define  GPIO_ClrBits(GPIOx,BITs)	GPIOx->BRR = BITs			/* 清除端口位 */
#define  GPIO_ChkBits(GPIOx,BITs)	CHKBITs(GPIOx->IDR, BITs)	/* 检查端口位是否为1 */
#define  GPIO_ChkNBits(GPIOx,BITs)	CHKNBITs(GPIOx->IDR, BITs)	/* 检查端口位是否为0 */


#endif

