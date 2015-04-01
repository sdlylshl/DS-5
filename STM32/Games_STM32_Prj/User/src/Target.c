/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Target.h
**��   ��   ��: Xsky
**��������: 2011-02-23 16:05
**��        ��: ��Ӳ�������йصĺ��������, ��Ӳ����Ӧ�ó���������й�
** �汾: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: Target.h
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

#define  TARGET_GLOBALS
#include "Config.h"
//#include "Target.h"

// �ⲿ��չ����Ĭ��״̬����
#define  X_IO_LCDCtrl_Def			(BIT(1) | BIT(5))	/* �ⲿ��չ�Ĵ���,����LCD */
#define  X_IO_LCDData_Def			0x00	/* �ⲿ��չ�Ĵ���,LCD���� */
#define  X_IO_OUT1_Def				0x00	/* �ⲿ��չIO1Ĭ��ֵ */
#define  X_IO_OUT2_Def				0x00	/* �ⲿ��չIO2Ĭ��ֵ */
#define  X_IO_OUT3_Def				0xFC	/* �ⲿ��չIO3Ĭ��ֵ */
#define  X_FPGA_IO_OUT1_Def			0x00	/* FPGA��չIO1Ĭ��ֵ */
#define  X_FPGA_IO_OUT2_Def			0x00	/* FPGA��չIO2Ĭ��ֵ */
// STM32 IOĬ��״̬����
#define  PORTA_Def					( (uint32_t)(BIT(4)|BIT(7)|BIT(9)|BIT(15)) )
#define  PORTB_Def					( (uint32_t)(BIT(0)|BIT(1)|BIT(5)|BIT(9)|BIT(10)|BIT(12)|BIT(14)|BIT(15)) )
#define  PORTC_Def					( (uint32_t)(BIT(1)|BIT(2)|BIT(10)|BIT(12)) )
#define  PORTD_Def					( (uint32_t)(BIT(3)|BIT(4)|BIT(5)|BIT(7)) )
#define  PORTE_Def					( (uint32_t)(BIT(1)) )

unsigned long g_ulOsEnterSum = 0;		// �����ٽ�������


/**
* @brief  Configures the NVIC for CAN.
* @param  None
* @retval None
*/
void NVIC_Config(void)
{
	
}

/*********************************************************************************************************
** ��������: FSMC_Config
** ��������: ��ʼ���ⲿ���߿����������ⲿ��չ�Ĵ���״̬
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: RCC_APB2PeriphClockCmd
** �桡��: 
** ������: Xsky
** �ա���: 
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void FSMC_Config(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	
	FSMC_NORSRAMTimingInitTypeDef  p;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	
#ifdef DEBUG_WRIO
	//GPIO_InitTypeDef GPIO_InitStructure; 
	register __IO uint32 *pIO_1, *pIO_0;
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/* FSMCֻʹ���˶˿�PD,PE */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	
	/*!< SRAM Data lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1 |
	                            GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	/*!< NOE and NWE configuration */  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*!< NE1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	pIO_1 = (__IO uint32 *)&(GPIOD->BSRR);
	pIO_0 = (__IO uint32 *)&(GPIOD->BRR);
	CLI();
	*pIO_1 = BIT(4);
	//__disable_irq;
	*pIO_0 = BIT(5)|BIT(7);
	Delay_Nus(1000);
	*pIO_1 = BIT(5)|BIT(7);
	Delay_Nus(1000);
	*pIO_0 = BIT(5)|BIT(7);
	Delay_Nus(1000);
	*pIO_1 = BIT(5)|BIT(7);
	Delay_Nus(1000 );
	*pIO_0 = BIT(5)|BIT(7);
	Delay_Nus(1000);
	*pIO_1 = BIT(5)|BIT(7);
	
	Delay_Nms(150);
	*pIO_0 = BIT(4)|BIT(5)|BIT(7);
	SEI();
	//__enable_irq;
#endif
	
	/* FSMCֻʹ���˶˿�PD,PE */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/*!< SRAM Data lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1 |
	                            GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | 
								 	GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*!< SRAM Address lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | 
	                            GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*!< NOE and NWE configuration */  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*!< NE1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*!< NBL0, NBL1 configuration */
	/* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	*/
	
	/*-- FSMC Configuration ------------------------------------------------------*/
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); 
	
	p.FSMC_AddressSetupTime = 1;			/* 2, ��ַ����ʱ�� */
	p.FSMC_AddressHoldTime = 0;				/* 0, ��ַ����ʱ��,�˲������� */
	p.FSMC_DataSetupTime = 2;				/* 2, ���ݽ���ʱ�� , �����ʱ������Ϊ0, �򲻿ɿ���JTAG, �Ҳ�����������, ���������ַ����ʱ��С��2���ⲿ���Ϊ0������ȷִ�� */
	p.FSMC_BusTurnAroundDuration = 0;		/*  */
	p.FSMC_CLKDivision = 0;					/* �첽SRAM��ʹ�� */
	p.FSMC_DataLatency = 0;					/* SRAM�������� */
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;  
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	//FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
	/*!< Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
	
	/* ��ʼ������ */
	/* ��ʼ���ⲿ��չ�Ĵ��� */
	//X_IO_Init();
}

// IO���״̬��ʼ��
void IO_Init(void)
{
	GPIOA->BSRR = PORTA_Def;
	GPIOA->BRR  = ~PORTA_Def;
	GPIOB->BSRR = PORTB_Def;
	GPIOB->BRR  = ~PORTB_Def;
	GPIOC->BSRR = PORTC_Def;
	GPIOC->BRR  = ~PORTC_Def;
	GPIOD->BSRR = PORTD_Def;
	GPIOD->BRR  = ~PORTD_Def;
	GPIOE->BSRR = PORTE_Def;
	GPIOE->BRR  = ~PORTE_Def;
}

static uint8 X_IO_Reg[X_IO_Num] = {
		X_IO_LCDCtrl_Def, X_IO_LCDData_Def, 
		X_IO_OUT1_Def, X_IO_OUT2_Def, X_IO_OUT3_Def};
static volatile uint8 * X_IO_Addr[X_IO_Num] = {
		&X_IO_LCDCtrl, &X_IO_LCDData, 
		&X_IO_OUT1, &X_IO_OUT2, &X_IO_OUT3};
/*******************************************************************************
* ������: X_IO_ClrBits
* ������: ��1ָ������չ�˿ڵ�ָ��λ
* ���롡: 1: index: IO����,X_IO_Index_...�������ֵ
*		2: mask: �����Чλ����, ����Ϊ0��λ���ֲ���
* �����: ��
* ���ء�: ��
*******************************************************************************/
void X_IO_SetBits(register t_ureg index, register uint8 mask)
{
	register uint8 r, *pr;
	pr = &X_IO_Reg[index];
	r = *pr | mask;
	*pr = r;
	*X_IO_Addr[index] = r;
}
/*******************************************************************************
* ������: X_IO_ClrBits
* ������: ��0ָ������չ�˿ڵ�ָ��λ
* ���롡: 1: index: IO����,X_IO_Index_...�������ֵ
*		2: mask: �����Чλ����, ����Ϊ0��λ���ֲ���
* �����: ��
* ���ء�: ��
*******************************************************************************/
void X_IO_ClrBits(register t_ureg index, register uint8 mask)
{
	register uint8 r, *pr;
	pr = &X_IO_Reg[index];
	r = *pr & (~mask);
	*pr = r;
	//*pr = r;
	*X_IO_Addr[index] = r;
	//CLRBITs(X_IO_Reg[index], mask);
	// *X_IO_Addr[index] = X_IO_Reg[index];
}
/*******************************************************************************
* ������: X_IO_OutBits
* ������: ����չIO���ָ��λ��ֵ,δָ��λ����
* ���롡: 1: index: IO����,X_IO_Index_...�������ֵ
*		2: mask: �����Чλ����, ����Ϊ0��λ���ֲ���
*		3: val: Ҫ�����ֵ,ֻȡ����Ϊ1��λ������λ��Ч, ��1ʱ����0xFF,��0ʱ����0x00
* �����: ��
* ���ء�: ��
*******************************************************************************/
void X_IO_OutBits(register t_ureg index, register uint8 mask, register uint8 val)
{
	register uint8 r, *pr;
	pr = &X_IO_Reg[index];
	//mask = ~mask;
	r = *pr & ~mask;
	r |= (val & mask);
	*pr = r;
	*X_IO_Addr[index] = r;
}

/*******************************************************************************
* ������: X_FPGA_Out
* ������: ��FPGAָ����ַ���ֵ
* ���롡: 1: : 
*		2: : 
*		3: : 
* �����: ��
* ���ء�: ��
*******************************************************************************/
void X_FPGA_Out(uint8 addr, uint8 *pSrc, t_ureg nBytes)
{
	
}
static uint8 X_FPGA_IO_Reg[2] = {X_FPGA_IO_OUT1_Def, X_FPGA_IO_OUT2_Def};
/*******************************************************************************
* ������: X_FPGA_IO_Out
* ������: ��FPGA��չIO���maskλ���Ϊ1��ֵ,δָ��λ����
* ���롡: 1: adrr: �꺯��X_FPGA_Addr(cs,addr)�е�addr��0��ʼ
*		2: mask: �����Чλ����, ����Ϊ0��λ���ֲ���
*		3: val: Ҫ�����ֵ,ֻȡ����Ϊ1��λ������λ��Ч
* �����: ��
* ���ء�: ��
*******************************************************************************/
void X_FPGA_IO_Out(register t_ureg addr, register uint8 mask, register uint8 val)
{
	register uint8 r, *pr;
	pr = &X_IO_Reg[addr];
	mask = ~mask;
	r = *pr & mask;
	r |= (val & mask);
	*pr = r;
	*X_IO_Addr[addr] = r;
	*(X_FPGA_Addr(X_FPGA_IO_CS_0, addr)) = r;
}
// ��ʼ����չ�Ĵ���
void X_IO_Init(void)
{
	X_IO_LCDCtrl = X_IO_Reg[X_IO_Index_LCDCtrl];
	X_IO_LCDData = X_IO_Reg[X_IO_Index_LCDData];
	X_IO_OUT1 = X_IO_Reg[X_IO_Index_OUT1];
	X_IO_OUT2 = X_IO_Reg[X_IO_Index_OUT2];
	X_IO_OUT3 = X_IO_Reg[X_IO_Index_OUT3];
	
	*(X_FPGA_Addr(X_FPGA_IO_CS_0, 0)) = X_FPGA_IO_Reg[0];
	*(X_FPGA_Addr(X_FPGA_IO_CS_0, 1)) = X_FPGA_IO_Reg[1];

}

/*********************************************************************************************************
** ��������: TargetInit
** ��������: ��ʼ������MCU��Դ
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: 
** ������: Xsky
** �ա���: 2011-4-22 14:39
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void TargetInit(void)
{
	// SysTick����Stm32_Init()������
	// SysTick_CLKSourceConfig();
	
	/* ȡ�������򲿷������޷��ڶ�ʱ����ʹ��
	** EXTI�ⲿ�ж�����Stm32_Init()������ 
	** EXIT 0: 		'PB0, Key1, �������ж�,�½����ж�
	** EXIT 1: 		'PB1, Key2, �������ж�,�½����ж�
	** EXIT 2: 
	** EXIT 3: 
	** EXIT 4: 
	** EXIT 5: 
	** EXIT 6: PD6, IO_IN1,�������ж�,�½����ж�
	** EXIT 7: 'PC7, FRQ2,�������½��ض������ж�; �������ڶϴ�����
	** EXIT 8: PD8, IO_IN2,�������ж�,�½����ж�
	** EXIT 9: PD9, IO_IN3,�������ж�,�½����ж�
	** EXIT10: PD10,IO_IN4,�������ж�,�½����ж�
	** EXIT11: 
	** EXIT12: PD12,IO_IN4,�������ж�,�½����ж�
	** EXIT13: PD13,IO_IN5,�������ж�,�½����ж�
	** EXIT14: PD14,IO_IN6,�������ж�,�½����ж�
	** EXIT15: PD15,IO_IN7,�������ж�,�½����ж�
	** EXIT16: 
	*/
	
	
}

#ifndef OS_EXIT_CRITICAL
void OS_EXIT_CRITICAL(void)
{
	if( (g_ulOsEnterSum == 0) || (--g_ulOsEnterSum == 0) )
	{
		SEI();
	}
}
#endif

