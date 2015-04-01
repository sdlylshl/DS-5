/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Target.h
**创   建   人: Xsky
**创建日期: 2011-02-23 16:05
**描        述: 与硬件配置有关的函数定义等
** 版本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: Target.h
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
// 编译开关
// CAN
#define  CAN1_EN				1
// UART
#define  UART_EN				1
#define  UART1T4_RX_TIM_EN		UART_EN		/* 开启接收字符超时中断功能 */
#define  UART1_EN				1
#define  UART2_EN				1
#define  UART3_EN				1
#define  UART4_EN				1
#define  UART5_EN				1
// 定时器
#define  COLOR_SENSOR_EN		1			/* 1:开启颜色传感器代码 */
#define  TIMER5_EN				COLOR_SENSOR_EN	/* 定时器5用于频率输入检测 */
// IO输入滤波
#define  IOINPUT_EN				1

/***************************************************************************************************/
// 板级定义


#define  CPU_FREQUENCY				72000000
#define  CPU_HCLK					72000000

// CAN相关定义
#define  NVIC_PriorityGroup_CAN		NVIC_PriorityGroup_2	/* CAN中断 */
#define  NVIC_PriorityGroup_CAN_SWI	NVIC_PriorityGroup_3	/* CAN接收处理软中断 */
#define  NVIC_PriorityGroup_CAN_SCE	NVIC_PriorityGroup_4	/* CAN状态改变中断,分配在低优先级 */

#define  NVIC_PriorityGroup_USART1	NVIC_PriorityGroup_3	/* USART1 */

#define  NVIC_PriorityGroup_USART4	NVIC_PriorityGroup_3	/* USART4 */



/* DMA分配  */
/* DMA1 */
// CH1: 

/* DMA2 */
/* .CH4: TIM5_CH2触发测量频率,Timer5.h */
extern void DMA2_CH4_IRQHandler (void);
//#define		DMA2_Channel4_IRQHook()		DMA2_CH4_IRQHandler()
/* .CH5: UART4_TX */

// TIM分配
/* TIM1:  */
/* TIM2: UpdateINT, Input.h, 用于: 全局1ms定时，定时扫描输入量函数 */
/* TIM3: CH1~4: Uart.h,用于UART1~4接收字符超时中断定时 */
/* TIM4: */
/* TIM5: CH2: Timer5.h,频率测量 */
/* TIM6: */
/* TIM7: */
/* TIM8: */

/* UART */
/* UART4: TFT屏,接收使用DMA2_CH5, 发送使用中断 */

// 输入Port函数定义
#define  IOIN_ChkBallCounter()		

// SPI-FRAM: PA[4..7], SPI1

// 外设总线扩展地址定义
#define  XPeriph_Base				0x60000000		/* 外设基地址 */
#define  XPeriph_AddrStep			0x00010000		/* 外设地址增长基数 */
#define  XPeriph_GetAddr(addr_h)	((volatile uint8 *)(XPeriph_Base + ((addr_h) << 16)))/* 计算外设地址 */
#define  X_FPGA_Base				XPeriph_GetAddr(0x00)	/* FPGA地址 */
#define  X_FPGA_AddrMask			0x7F
//#define  X_FPGA_GetAddr(addr)		XPeriph_GetAddr((addr) & X_FPGA_AddrMask) /* FPGA地址空间 */
// cs:0~0xF,低三位addr:0~7
#define  X_FPGA_Addr(cs, addr)		XPeriph_GetAddr((((cs)<<3)|(addr)) & X_FPGA_AddrMask) /* FPGA地址空间 */
#define  X_IO_LCDCtrl				*(XPeriph_GetAddr(0xC6))	/* LCD控制寄存器 */
#define  X_IO_LCDData				*(XPeriph_GetAddr(0xC8))	/* LCD数据寄存器 */
#define  X_IO_OUT1					*(XPeriph_GetAddr(0xCA))	/* IO_OUT1 */
#define  X_IO_OUT2					*(XPeriph_GetAddr(0xCC))	/* IO_OUT2 */
#define  X_IO_OUT3					*(XPeriph_GetAddr(0xCE))	/* IO_OUT3 */
#define  X_IO_Num					5	/* 总输出数目 */
#define  X_IO_Index_LCDCtrl			0
#define  X_IO_Index_LCDData			1
#define  X_IO_Index_OUT1			2
#define  X_IO_Index_OUT2			3
#define  X_IO_Index_OUT3			4

// 颜色传感器TCS3200: S2:S3, 与输出相对应IO_EXOUT26:25, A6:3为cs,A2:0为地址
// 地址A6:0,OUT25~32：0x78
// 地址A6:0,OUT33~36：0x79
#define  X_FPGA_IO_CS_0				0x0F
#define  COLORSENS_RED				0x00	//(0b000)
#define  COLORSENS_BLUE				0x02	//(0b010)
#define  COLORSENS_CLEAR			0x04	//(0b100)
#define  COLORSENS_GREEN			0x06	//(0b110)
#define  COLORSENS_MASK				0x06	//(0b110)
#define  COLORSENS_LED				0x10


// LCD相关位控制位定义
#define  LCD_PIN_LEDA				6
#define  LCD_PIN_LEDK				7

/***************************************************************************************************/
// CPU相关函数说明

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
	//;CPSID I		;//PRIMASK=1, 关中断
	//;bx lr
	register uint32 __regPriMask		   __asm("primask");
	__regPriMask = (1);
}
static __inline  void SEI(void)
{
	//msr primask, 0
	//;CPSIE I 		;//PRIMASK=0;开中断
	//;bx lr
	register uint32 __regPriMask		   __asm("primask");
	__regPriMask = (0);
}
//#define __enable_irq                              __enable_interrupt        /*!< global Interrupt enable */
//#define __disable_irq                             __disable_interrupt       /*!< global Interrupt disable */
//#define CLI()	__disable_interrupt
//#define SEI()	__enable_interrupt
//#define CLI()	__set_PRIMASK(1) //NVIC_SETPRIMASK();NVIC_SETFAULTMASK();	// 关中断
//#define SEI()	__set_PRIMASK(0) //NVIC_RESETPRIMASK();NVIC_RESETFAULTMASK(); 	// 开中断
#define NOP()	__nop()           //NOP 

//#define OS_ENTER_CRITICAL()
//#define OS_EXIT_CRITICAL()

TARGET_EXT unsigned long g_ulOsEnterSum;
#ifndef OS_ENTER_CRITICAL
//#define OS_ENTER_CRITICAL()			CLI(); \
//									g_ulOsEnterSum++
static __inline void OS_ENTER_CRITICAL(void)
{
	//CPSID I		;//关中断
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

#define  WdtFeed()		/* 看门狗喂狗 */
#define  WdtFeedQuickly()
#define  WdtFeed_Start()	
#define  WdtFeed_End()

#define  Reset()					((void *)0)()

/***************************************************************************************************/
// 函数说明
// 应用层函数接口
// STM32 IO输出初始化
TARGET_EXT void IO_Init(void);
// 设置外部扩展寄存器,位
TARGET_EXT void X_IO_SetBits(register t_ureg index, register uint8 mask);
// 清除外部扩展寄存器,位
TARGET_EXT void X_IO_ClrBits(register t_ureg index, register uint8 mask);
// 直接按掩码输出扩展寄存器指定位
TARGET_EXT void X_IO_OutBits(register t_ureg index, register uint8 mask, register uint8 val);
#define  X_LCD_CTRL_OutBits(bitsMask, val)	X_IO_OutBits(X_IO_Index_LCDCtrl, bitsMask, val)
#define  X_LCD_DATA_OutBits(bitsMask, val)	X_IO_OutBits(X_IO_Index_LCDData, bitsMask, val)

// 输出指定位的值到外扩FPGA OUT
TARGET_EXT void X_FPGA_IO_Out(register t_ureg addr, register uint8 mask, register uint8 val);

// 初始化函数

// 初始化外部总线控制器及化外部扩展寄存器状态
TARGET_EXT void FSMC_Config(void);
// 初始化中断向量控制器
TARGET_EXT void NVIC_Config(void);			// NVIC配置
// 初始化外部扩展寄存器
TARGET_EXT void X_IO_Init(void);
// 其它外设初始化
TARGET_EXT void TargetInit(void);



#endif	// end: #ifndef __TARGET_H__

