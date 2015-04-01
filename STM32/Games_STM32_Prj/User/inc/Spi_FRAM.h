/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Spi_FRAM.h
**创   建   人: Xsky
**创 建 日  期: 2011-03-21 16:36
**描        述: SPI FRAM 驱动函数
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: Spi_FRAM.c
**			 
**			 
**-------------------------------------------------------------------------------------------------------
**
**------------------------------------------------------------------------------------------------------
** 最后修改日期: 
** 修改人: Xsky
** 版本: v1.0.00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日期: 
** 版本: v1.0.00
** 修改: 
**			1. 
**			2. 
**			3. 
**			4. 
**			5. 
********************************************************************************************************/
#ifndef __SPI_FRAM_H__
#define __SPI_FRAM_H__

#ifdef SPI_FRAM_GLOBALS
#define  SPI_FRAM_EXT
#else
#define  SPI_FRAM_EXT	extern
#endif

/***************************************************************************************************/
// 常数定义

#define  FRAM_TotalBytes			(2*1024)	/* FRAM容量 */

// FRAM引脚相关定义, SPI1, PA[4..7]
#define  RCC_FRAM_SPI				RCC_APB2Periph_SPI1
#define  RCC_FRAM_GPIO				RCC_APB2Periph_GPIOA
//#define  GPIO_Remap_FRAM			不定义该项,管脚无重映射
#define  GPIO_FRAM					GPIOA
#define  GPIO_FRAM_CS				GPIOA
#define  GPIO_Pin_FRAM_CS			GPIO_Pin_4
#define  GPIO_Pin_FRAM_SCK			GPIO_Pin_5
#define  GPIO_Pin_FRAM_SI			GPIO_Pin_6
#define  GPIO_Pin_FRAM_SO			GPIO_Pin_7
#define  GPIO_Speed_FRAM			GPIO_Speed_50MHz
#define  SPI_FRAM					SPI1
// 片选电平函数定义
#define	 FRAM_CS_LOW()				GPIO_FRAM_CS->BRR = GPIO_Pin_FRAM_CS
#define  FRAM_CS_HIGH()				GPIO_FRAM_CS->BSRR = GPIO_Pin_FRAM_CS

#define  FRAM_MAX_ADDR 				(FRAM_TotalBytes - 1)

//#define SPI_FRAM_BIT_RATE     	10000000	/* 设定波特率 */
#define  SPI_FRAM_BaudRatePrescaler	SPI_BaudRatePrescaler_4	/* SPI1,APB2:36MHz / Prescaler */
#define  SPI_FRAM_DataSize   		SPI_DataSize_8b			/* 设定数据宽度 */

/***************************************************************************************************/
// 函数声明
// 应用层接口函数
SPI_FRAM_EXT  void FRAM_Write(uint16 add,void *pSrc,uint32 len);
SPI_FRAM_EXT  void FRAM_Zero(void);
SPI_FRAM_EXT  void FRAM_Read(void *pDst, uint16 add,uint32 len);


// 初始化函数
SPI_FRAM_EXT void FRAM_Config(void);

/***************************************************************************************************/
// 全局量引用说明
#ifndef SPI_FRAM_GLOBALS

#endif


#endif	// end: __SPI_FRAM_H__

