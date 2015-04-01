/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Target.h
**创   建   人: Xsky
**创建日期: 2011-02-23 16:05
**描        述: 与硬件配置有关的函数定义等, 与硬件及应用程序参数均有关
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

#define  TARGET_GLOBALS
#include "Config.h"
//#include "Target.h"

// 外部扩展输入默认状态定义
#define  X_IO_LCDCtrl_Def			(BIT(1) | BIT(5))	/* 外部扩展寄存器,控制LCD */
#define  X_IO_LCDData_Def			0x00	/* 外部扩展寄存器,LCD数据 */
#define  X_IO_OUT1_Def				0x00	/* 外部扩展IO1默认值 */
#define  X_IO_OUT2_Def				0x00	/* 外部扩展IO2默认值 */
#define  X_IO_OUT3_Def				0xFC	/* 外部扩展IO3默认值 */
#define  X_FPGA_IO_OUT1_Def			0x00	/* FPGA扩展IO1默认值 */
#define  X_FPGA_IO_OUT2_Def			0x00	/* FPGA扩展IO2默认值 */
// STM32 IO默认状态定义
#define  PORTA_Def					( (uint32_t)(BIT(4)|BIT(7)|BIT(9)|BIT(15)) )
#define  PORTB_Def					( (uint32_t)(BIT(0)|BIT(1)|BIT(5)|BIT(9)|BIT(10)|BIT(12)|BIT(14)|BIT(15)) )
#define  PORTC_Def					( (uint32_t)(BIT(1)|BIT(2)|BIT(10)|BIT(12)) )
#define  PORTD_Def					( (uint32_t)(BIT(3)|BIT(4)|BIT(5)|BIT(7)) )
#define  PORTE_Def					( (uint32_t)(BIT(1)) )

unsigned long g_ulOsEnterSum = 0;		// 进入临界区计数


/**
* @brief  Configures the NVIC for CAN.
* @param  None
* @retval None
*/
void NVIC_Config(void)
{
	
}

/*********************************************************************************************************
** 函数名称: FSMC_Config
** 功能描述: 初始化外部总线控制器及化外部扩展寄存器状态
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: RCC_APB2PeriphClockCmd
** 版　本: 
** 作　者: Xsky
** 日　期: 
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
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
	/* FSMC只使用了端口PD,PE */
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
	
	/* FSMC只使用了端口PD,PE */
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
	
	p.FSMC_AddressSetupTime = 1;			/* 2, 地址建立时间 */
	p.FSMC_AddressHoldTime = 0;				/* 0, 地址保持时间,此参数无用 */
	p.FSMC_DataSetupTime = 2;				/* 2, 数据建立时间 , 如果该时间设置为0, 则不可控制JTAG, 且不能正常运行, 测试如果地址建立时间小于2则外部输出为0不能正确执行 */
	p.FSMC_BusTurnAroundDuration = 0;		/*  */
	p.FSMC_CLKDivision = 0;					/* 异步SRAM不使用 */
	p.FSMC_DataLatency = 0;					/* SRAM不用设置 */
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
	
	/* 初始化外设 */
	/* 初始化外部扩展寄存器 */
	//X_IO_Init();
}

// IO输出状态初始化
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
* 函数名: X_IO_ClrBits
* 描述　: 置1指定的扩展端口的指定位
* 输入　: 1: index: IO索引,X_IO_Index_...所定义的值
*		2: mask: 输出有效位掩码, 掩码为0的位保持不变
* 输出　: 无
* 返回　: 无
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
* 函数名: X_IO_ClrBits
* 描述　: 清0指定的扩展端口的指定位
* 输入　: 1: index: IO索引,X_IO_Index_...所定义的值
*		2: mask: 输出有效位掩码, 掩码为0的位保持不变
* 输出　: 无
* 返回　: 无
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
* 函数名: X_IO_OutBits
* 描述　: 向扩展IO输出指定位的值,未指定位不变
* 输入　: 1: index: IO索引,X_IO_Index_...所定义的值
*		2: mask: 输出有效位掩码, 掩码为0的位保持不变
*		3: val: 要输出的值,只取掩码为1的位，其它位无效, 置1时可用0xFF,置0时可用0x00
* 输出　: 无
* 返回　: 无
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
* 函数名: X_FPGA_Out
* 描述　: 向FPGA指定地址输出值
* 输入　: 1: : 
*		2: : 
*		3: : 
* 输出　: 无
* 返回　: 无
*******************************************************************************/
void X_FPGA_Out(uint8 addr, uint8 *pSrc, t_ureg nBytes)
{
	
}
static uint8 X_FPGA_IO_Reg[2] = {X_FPGA_IO_OUT1_Def, X_FPGA_IO_OUT2_Def};
/*******************************************************************************
* 函数名: X_FPGA_IO_Out
* 描述　: 向FPGA扩展IO输出mask位标记为1的值,未指定位不变
* 输入　: 1: adrr: 宏函数X_FPGA_Addr(cs,addr)中的addr从0开始
*		2: mask: 输出有效位掩码, 掩码为0的位保持不变
*		3: val: 要输出的值,只取掩码为1的位，其它位无效
* 输出　: 无
* 返回　: 无
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
// 初始化扩展寄存器
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
** 函数名称: TargetInit
** 功能描述: 初始化部分MCU资源
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: 
** 版　本: 
** 作　者: Xsky
** 日　期: 2011-4-22 14:39
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void TargetInit(void)
{
	// SysTick已在Stm32_Init()中配置
	// SysTick_CLKSourceConfig();
	
	/* 取消配置因部分输入无法在定时器中使用
	** EXTI外部中断已在Stm32_Init()中配置 
	** EXIT 0: 		'PB0, Key1, 上升沿中断,下降沿中断
	** EXIT 1: 		'PB1, Key2, 上升沿中断,下降沿中断
	** EXIT 2: 
	** EXIT 3: 
	** EXIT 4: 
	** EXIT 5: 
	** EXIT 6: PD6, IO_IN1,上升沿中断,下降沿中断
	** EXIT 7: 'PC7, FRQ2,上升沿下降沿都产生中断; 中心球遮断传感器
	** EXIT 8: PD8, IO_IN2,上升沿中断,下降沿中断
	** EXIT 9: PD9, IO_IN3,上升沿中断,下降沿中断
	** EXIT10: PD10,IO_IN4,上升沿中断,下降沿中断
	** EXIT11: 
	** EXIT12: PD12,IO_IN4,上升沿中断,下降沿中断
	** EXIT13: PD13,IO_IN5,上升沿中断,下降沿中断
	** EXIT14: PD14,IO_IN6,上升沿中断,下降沿中断
	** EXIT15: PD15,IO_IN7,上升沿中断,下降沿中断
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

