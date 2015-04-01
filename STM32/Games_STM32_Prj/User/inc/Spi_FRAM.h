/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Spi_FRAM.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-03-21 16:36
**��        ��: SPI FRAM ��������
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: Spi_FRAM.c
**			 
**			 
**-------------------------------------------------------------------------------------------------------
**
**------------------------------------------------------------------------------------------------------
** ����޸�����: 
** �޸���: Xsky
** �汾: v1.0.00
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** ����: 
** �汾: v1.0.00
** �޸�: 
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
// ��������

#define  FRAM_TotalBytes			(2*1024)	/* FRAM���� */

// FRAM������ض���, SPI1, PA[4..7]
#define  RCC_FRAM_SPI				RCC_APB2Periph_SPI1
#define  RCC_FRAM_GPIO				RCC_APB2Periph_GPIOA
//#define  GPIO_Remap_FRAM			���������,�ܽ�����ӳ��
#define  GPIO_FRAM					GPIOA
#define  GPIO_FRAM_CS				GPIOA
#define  GPIO_Pin_FRAM_CS			GPIO_Pin_4
#define  GPIO_Pin_FRAM_SCK			GPIO_Pin_5
#define  GPIO_Pin_FRAM_SI			GPIO_Pin_6
#define  GPIO_Pin_FRAM_SO			GPIO_Pin_7
#define  GPIO_Speed_FRAM			GPIO_Speed_50MHz
#define  SPI_FRAM					SPI1
// Ƭѡ��ƽ��������
#define	 FRAM_CS_LOW()				GPIO_FRAM_CS->BRR = GPIO_Pin_FRAM_CS
#define  FRAM_CS_HIGH()				GPIO_FRAM_CS->BSRR = GPIO_Pin_FRAM_CS

#define  FRAM_MAX_ADDR 				(FRAM_TotalBytes - 1)

//#define SPI_FRAM_BIT_RATE     	10000000	/* �趨������ */
#define  SPI_FRAM_BaudRatePrescaler	SPI_BaudRatePrescaler_4	/* SPI1,APB2:36MHz / Prescaler */
#define  SPI_FRAM_DataSize   		SPI_DataSize_8b			/* �趨���ݿ�� */

/***************************************************************************************************/
// ��������
// Ӧ�ò�ӿں���
SPI_FRAM_EXT  void FRAM_Write(uint16 add,void *pSrc,uint32 len);
SPI_FRAM_EXT  void FRAM_Zero(void);
SPI_FRAM_EXT  void FRAM_Read(void *pDst, uint16 add,uint32 len);


// ��ʼ������
SPI_FRAM_EXT void FRAM_Config(void);

/***************************************************************************************************/
// ȫ��������˵��
#ifndef SPI_FRAM_GLOBALS

#endif


#endif	// end: __SPI_FRAM_H__

