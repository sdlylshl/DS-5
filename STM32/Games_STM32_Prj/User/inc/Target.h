/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Target.h
**��   ��   ��: Xsky
**��������: 2011-02-23 16:05
**��        ��: ��Ӳ�������йصĺ��������
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
#ifndef __TARGET_H__
#define __TARGET_H__

#ifdef TARGET_GLOBALS
#define  TARGET_EXT
#else
#define  TARGET_EXT		extern
#endif

//#include "stm32f10x.h"
//#include "core_cm3.h"

/***************************************************************************************************/
// ���뿪��
// CAN
#define  CAN1_EN				1
// UART
#define  UART_EN				1
#define  UART1T4_RX_TIM_EN		UART_EN		/* ���������ַ���ʱ�жϹ��� */
#define  UART1_EN				1
#define  UART2_EN				1
#define  UART3_EN				1
#define  UART4_EN				1
#define  UART5_EN				1
// ��ʱ��
#define  COLOR_SENSOR_EN		1			/* 1:������ɫ���������� */
#define  TIMER5_EN				COLOR_SENSOR_EN	/* ��ʱ��5����Ƶ�������� */
// IO�����˲�
#define  IOINPUT_EN				1

/***************************************************************************************************/
// �弶����


#define  CPU_FREQUENCY				72000000
#define  CPU_HCLK					72000000

// CAN��ض���
#define  NVIC_PriorityGroup_CAN		NVIC_PriorityGroup_2	/* CAN�ж� */
#define  NVIC_PriorityGroup_CAN_SWI	NVIC_PriorityGroup_3	/* CAN���մ������ж� */
#define  NVIC_PriorityGroup_CAN_SCE	NVIC_PriorityGroup_4	/* CAN״̬�ı��ж�,�����ڵ����ȼ� */

#define  NVIC_PriorityGroup_USART1	NVIC_PriorityGroup_3	/* USART1 */

#define  NVIC_PriorityGroup_USART4	NVIC_PriorityGroup_3	/* USART4 */



/* DMA����  */
/* DMA1 */
// CH1: 

/* DMA2 */
/* .CH4: TIM5_CH2��������Ƶ��,Timer5.h */
extern void DMA2_CH4_IRQHandler (void);
//#define		DMA2_Channel4_IRQHook()		DMA2_CH4_IRQHandler()
/* .CH5: UART4_TX */

// TIM����
/* TIM1:  */
/* TIM2: UpdateINT, Input.h, ����: ȫ��1ms��ʱ����ʱɨ������������ */
/* TIM3: CH1~4: Uart.h,����UART1~4�����ַ���ʱ�ж϶�ʱ */
/* TIM4: */
/* TIM5: CH2: Timer5.h,Ƶ�ʲ��� */
/* TIM6: */
/* TIM7: */
/* TIM8: */

/* UART */
/* UART4: TFT��,����ʹ��DMA2_CH5, ����ʹ���ж� */

// ����Port��������
#define  IOIN_ChkBallCounter()		

// SPI-FRAM: PA[4..7], SPI1

// ����������չ��ַ����
#define  XPeriph_Base				0x60000000		/* �������ַ */
#define  XPeriph_AddrStep			0x00010000		/* �����ַ�������� */
#define  XPeriph_GetAddr(addr_h)	((volatile uint8 *)(XPeriph_Base + ((addr_h) << 16)))/* ���������ַ */
#define  X_FPGA_Base				XPeriph_GetAddr(0x00)	/* FPGA��ַ */
#define  X_FPGA_AddrMask			0x7F
//#define  X_FPGA_GetAddr(addr)		XPeriph_GetAddr((addr) & X_FPGA_AddrMask) /* FPGA��ַ�ռ� */
// cs:0~0xF,����λaddr:0~7
#define  X_FPGA_Addr(cs, addr)		XPeriph_GetAddr((((cs)<<3)|(addr)) & X_FPGA_AddrMask) /* FPGA��ַ�ռ� */
#define  X_IO_LCDCtrl				*(XPeriph_GetAddr(0xC6))	/* LCD���ƼĴ��� */
#define  X_IO_LCDData				*(XPeriph_GetAddr(0xC8))	/* LCD���ݼĴ��� */
#define  X_IO_OUT1					*(XPeriph_GetAddr(0xCA))	/* IO_OUT1 */
#define  X_IO_OUT2					*(XPeriph_GetAddr(0xCC))	/* IO_OUT2 */
#define  X_IO_OUT3					*(XPeriph_GetAddr(0xCE))	/* IO_OUT3 */
#define  X_IO_Num					5	/* �������Ŀ */
#define  X_IO_Index_LCDCtrl			0
#define  X_IO_Index_LCDData			1
#define  X_IO_Index_OUT1			2
#define  X_IO_Index_OUT2			3
#define  X_IO_Index_OUT3			4

// ��ɫ������TCS3200: S2:S3, ��������ӦIO_EXOUT26:25, A6:3Ϊcs,A2:0Ϊ��ַ
// ��ַA6:0,OUT25~32��0x78
// ��ַA6:0,OUT33~36��0x79
#define  X_FPGA_IO_CS_0				0x0F
#define  COLORSENS_RED				0x00	//(0b000)
#define  COLORSENS_BLUE				0x02	//(0b010)
#define  COLORSENS_CLEAR			0x04	//(0b100)
#define  COLORSENS_GREEN			0x06	//(0b110)
#define  COLORSENS_MASK				0x06	//(0b110)
#define  COLORSENS_LED				0x10


// LCD���λ����λ����
#define  LCD_PIN_LEDA				6
#define  LCD_PIN_LEDK				7

/***************************************************************************************************/
// CPU��غ���˵��

/*
__inline void __set_PRIMASK(uint32 priMask)
{
  register uint32 __regPriMask         __asm("primask");
  __regPriMask = (priMask);
}
__inline __asm void __set_PriMask(uint32 priMask)
{
  msr primask, r0
  bx lr
}
*/

static __inline  void CLI(void)
{
	//;msr primask, 1
	//;CPSID I		;//PRIMASK=1, ���ж�
	//;bx lr
	register uint32 __regPriMask		   __asm("primask");
	__regPriMask = (1);
}
static __inline  void SEI(void)
{
	//msr primask, 0
	//;CPSIE I 		;//PRIMASK=0;���ж�
	//;bx lr
	register uint32 __regPriMask		   __asm("primask");
	__regPriMask = (0);
}
//#define __enable_irq                              __enable_interrupt        /*!< global Interrupt enable */
//#define __disable_irq                             __disable_interrupt       /*!< global Interrupt disable */
//#define CLI()	__disable_interrupt
//#define SEI()	__enable_interrupt
//#define CLI()	__set_PRIMASK(1) //NVIC_SETPRIMASK();NVIC_SETFAULTMASK();	// ���ж�
//#define SEI()	__set_PRIMASK(0) //NVIC_RESETPRIMASK();NVIC_RESETFAULTMASK(); 	// ���ж�
#define NOP()	__nop()           //NOP 

//#define OS_ENTER_CRITICAL()
//#define OS_EXIT_CRITICAL()

TARGET_EXT unsigned long g_ulOsEnterSum;
#ifndef OS_ENTER_CRITICAL
//#define OS_ENTER_CRITICAL()			CLI(); \
//									g_ulOsEnterSum++
static __inline void OS_ENTER_CRITICAL(void)
{
	//CPSID I		;//���ж�
	//bx lr
	register uint32 __regPriMask		   __asm("primask");
	__regPriMask = (1);
	g_ulOsEnterSum++;
}
#endif
#ifndef OS_EXIT_CRITICAL
//#define OS_EXIT_CRITICAL()			if ((g_ulOsEnterSum == 0) || (--g_ulOsEnterSum==1)) \
//									{ \
//										SEI(); \
//									}
TARGET_EXT void OS_EXIT_CRITICAL(void);
#endif

#define  CRITICAL_EXEC(code)		OS_ENTER_CRITICAL(); \
									code; \
									OS_EXIT_CRITICAL()

#define  WdtFeed()		/* ���Ź�ι�� */
#define  WdtFeedQuickly()
#define  WdtFeed_Start()	
#define  WdtFeed_End()

#define  Reset()					((void *)0)()

/***************************************************************************************************/
// ����˵��
// Ӧ�ò㺯���ӿ�
// STM32 IO�����ʼ��
TARGET_EXT void IO_Init(void);
// �����ⲿ��չ�Ĵ���,λ
TARGET_EXT void X_IO_SetBits(register t_ureg index, register uint8 mask);
// ����ⲿ��չ�Ĵ���,λ
TARGET_EXT void X_IO_ClrBits(register t_ureg index, register uint8 mask);
// ֱ�Ӱ����������չ�Ĵ���ָ��λ
TARGET_EXT void X_IO_OutBits(register t_ureg index, register uint8 mask, register uint8 val);
#define  X_LCD_CTRL_OutBits(bitsMask, val)	X_IO_OutBits(X_IO_Index_LCDCtrl, bitsMask, val)
#define  X_LCD_DATA_OutBits(bitsMask, val)	X_IO_OutBits(X_IO_Index_LCDData, bitsMask, val)

// ���ָ��λ��ֵ������FPGA OUT
TARGET_EXT void X_FPGA_IO_Out(register t_ureg addr, register uint8 mask, register uint8 val);

// ��ʼ������

// ��ʼ���ⲿ���߿����������ⲿ��չ�Ĵ���״̬
TARGET_EXT void FSMC_Config(void);
// ��ʼ���ж�����������
TARGET_EXT void NVIC_Config(void);			// NVIC����
// ��ʼ���ⲿ��չ�Ĵ���
TARGET_EXT void X_IO_Init(void);
// ���������ʼ��
TARGET_EXT void TargetInit(void);



#endif	// end: #ifndef __TARGET_H__

