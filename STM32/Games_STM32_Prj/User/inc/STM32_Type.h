
#ifndef __STM32_TYPE1__H
#define __STM32_TYPE1__H

//#define  GPIO_SetBits(GPIOx,BITs)	GPIOx->BSRR = BITs			/* ���ö˿�λ */
//#define  GPIO_ClrBits(GPIOx,BITs)	GPIOx->BRR = BITs			/* ����˿�λ */
#define  GPIO_ChkBits(GPIOx,BITs)	CHKBITs(GPIOx->IDR, BITs)	/* ���˿�λ�Ƿ�Ϊ1 */
#define  GPIO_ChkNBits(GPIOx,BITs)	CHKNBITs(GPIOx->IDR, BITs)	/* ���˿�λ�Ƿ�Ϊ0 */


#endif

