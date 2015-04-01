/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Timer5.c
**创   建   人: Xsky
**创 建 日  期: 2011-04-13 17:44
**描        述: 定时器5驱动相关驱动函数, CH2输入触发使用DMA计数
**			   注: 要求 CPU_HCLK >= 100KHz
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: Timer5.H
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

#define  TIMER5_GLOBALS
#include "Global.h"
#include "Stm32f10x.h"
#include "Stm32f10x_rcc.h"
#include "Target.h"
#include "Timer5.h"

#ifndef TIMER5_EN
#define  TIMER5_EN		0
#endif
#if TIMER5_EN > 0

/***************************************************************************************************/
// 常数定义, 应用于其它UART时需要修改本段定义
#define  TIMx_Update_Period_ms		100		/* 单位:ms, 定时器更新周期,最大655ms */

#define  TIMx						TIM5
#define  TIMx_IRQn					TIM5_IRQn
#define  TIMx_RCCCmd(s) 			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, s)/* 时钟 */
#define  TIMx_NVIC_PriorityGroup	NVIC_PriorityGroup_0
#define  TIMx_NVIC_Priority			0
#define  TIMx_NVIC_PrioritySub		0
void 	 TIM5_IRQHandler(void);
#define  TIMx_IRQHandler			TIM5_IRQHandler			/* 定时器中断处理函数 */
#define  TIMx_CHy					TIM_Channel_2
#define  TIMx_CH2_Remap()					/* TIM5 CH2无映射 */
#define  TIMx_CH2_GPIO				GPIOA			/* PA1,TIM5 CH2所在端口 */
#define  TIMx_CH2_Pin				GPIO_Pin_1		/* PA1 */
#define  TIMx_CH2_RCC				(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)
#define  TIMx_CH2_RCC_Cmd(s)		RCC_APB2PeriphClockCmd(TIMx_CH2_RCC, s)

#define  DMAx						DMA2
#define  DMAx_CHy 					DMA2_Channel4
#define  DMAx_RCC					RCC_AHBPeriph_DMA2
#define  DMAx_NVIC_PriorityGroup	NVIC_PriorityGroup_0
#define  DMAx_NVIC_Priority			0
#define  DMAx_NVIC_PrioritySub		0

// 可重入性开关中断定义
#define  COLOR_ENTER_CRITICAL()		/* 关中断 */
#define  COLOR_EXIT_CRITICAL()		/* 开中断 */

/***************************************************************************************************/
// 局部变量
#define 		DMA_INIT_Cnt	0xFFFF		/* DMA初始化赋值 */
static uint32 	nCounter = 0;			// 累计DMA传输数量,即CH2触发次数
static uint8	Tim5RunFlag = FALSE;	// 设置定时器开启标志以在主程序中检测是不否完成
static uint16 	nDMATransmit = 0;		// DMA初始化传输数量
static uint16	nDMADstMem;				// 临时代码

/***************************************************************************************************/
// 正文

/*********************************************************************************************************
** 函数名称: TIMx_CHy_DMA_Config
** 功能描述: 初始化配置DMA
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: 
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2011-04-14 09:52
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static void TIMx_CHy_DMA_Config(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(DMAx_RCC, ENABLE);
	
	/* DMAx_CHy (triggered by DMA事件 event) Config */
	DMA_DeInit(DMAx_CHy);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DMAx_CHy->CNDTR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&nDMADstMem;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = DMA_INIT_Cnt;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = 0;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMAx_CHy, &DMA_InitStructure);
	
	// 开启传输完成中断
	DMA_ITConfig(DMAx_CHy, DMA_IT_TC, ENABLE);
	
	// 中断向量配置
	NVIC_PriorityGroupConfig(DMAx_NVIC_PriorityGroup);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMAx_NVIC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMAx_NVIC_PrioritySub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// 现在开启DMA传输, 由发送函数调用开启
	/* Enable USARTy DMA TX request */
	//USART_DMACmd(UARTx, USART_DMAReq_Tx, ENABLE);
	/* Enable USARTy TX DMA1 Channel */
	//DMA_Cmd(DMAx_CHy, ENABLE);
	
}

/*********************************************************************************************************
** 函数名称: TIM5_CH2_DMA_Start
** 功能描述: 重置DMA并开启传输
** 输　入: 无
** 输　出: 无
** 返　回: 错误码
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-14 10:45
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static void TIM5_CH2_DMA_Start(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	/* DMA.Datashett.CN: p147 */
	//register uint32_t N;
	//register uint8 *pOut, *pEnd;
	
	/* Disable the selected DMAy Channelx */
 	CLRBITs(DMAx_CHy->CCR, (uint16_t)(~DMA_CCR1_EN));
	
	/* 发送缓冲区 */
	//DMAx_CHy->CMAR = (uint32_t)pDst;
	
	/* 发送的字节数 */
	nDMATransmit = DMA_INIT_Cnt;
	DMAx_CHy->CNDTR = DMA_INIT_Cnt;
	
	/* 重新开启接收DMA通道 */
	SETBITs(DMAx_CHy->CCR, DMA_CCR1_EN);
	
	/* 开启UART接收DMA请求 */
	//UARTx->CR3 |= USART_DMAReq_Rx;	// 本来已开启
}

/*********************************************************************************************************
** 函数名称: DMA2_CH4_IRQHandler
** 功能描述: DMA2_CH4_中断处理, 其函数名被外部引用，更改时需要注意同步
** 输　入: 无
** 输　出: 无
** 返　回: 错误码
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-02 15:49
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void DMA2_CH4_IRQHandler (void)
{
	register t_ureg u32r;
	register t_ureg N;
	
	u32r = DMAx->ISR;
	if(u32r & DMA2_FLAG_TC4)	// DMA传输完成中断标志
	{
	    // 读取DMA已传输值
		N = nDMATransmit;
		u32r = DMAx_CHy->CNDTR; /* 获取当前未传输数 */
		// 尽快重启DMA 
		TIM5_CH2_DMA_Start();
		if(N > 0)
		{
			N -= u32r;
		}
		nCounter += N;
		
		COLOR_ENTER_CRITICAL();
	    COLOR_EXIT_CRITICAL();
	    
	    // 清除中断标志
	    DMAx->IFCR = DMA2_FLAG_TC4;
	}
	
}

// 停止定时器
#define  TIMx_Stop() 			CLRBITs(TIMx->CR1, TIM_CR1_CEN); \
								COLOR_ENTER_CRITICAL(); \
								Tim5RunFlag = FALSE; \
								COLOR_EXIT_CRITICAL()

// 开启定时器
#define  TIMx_Start()			TIMx->CNT = 0x0000; \
								SETBITs(TIMx->CR1, TIM_CR1_CEN); \
								COLOR_ENTER_CRITICAL(); \
								Tim5RunFlag = TRUE; \
								nDMATransmit = 0; \
								nCounter = 0; \
								COLOR_EXIT_CRITICAL()

/*********************************************************************************************************
** 函数名称: Timer5_Config
** 功能描述: 定时器5初始化配置函数, 用于检测输入频率, 采用DMA计数
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: 
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2011-04-14 09:52
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void TIM5_Config(void)
{
	/* Datasheet.CN
	** 高级控制定时器: p199, TIM1/TIM8
	** 基本定时器: p298, TIM6/TIM7
	** 通用定时器: p253, TIM2/3/4/5
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;
	TIM_ICInitTypeDef		TIM_ICInitStructure;
	
	/* Enable GPIO clock */
	TIMx_CH2_RCC_Cmd(ENABLE);
	
	/* 配置CH2为输入 */
	GPIO_InitStructure.GPIO_Pin = TIMx_CH2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(TIMx_CH2_GPIO, &GPIO_InitStructure);
	
	/* 时钟 */
	TIMx_RCCCmd(ENABLE);
	/* 重置所有寄存器位 */
	TIM_DeInit(TIMx);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 
	TIM_TimeBaseInitStructure.TIM_Prescaler = (CPU_HCLK/100000)-1;					// 预分频为1/00ms
	TIM_TimeBaseInitStructure.TIM_Period = TIMx_Update_Period_ms * 100;	// ARR
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);			// 选择为单脉冲模式, 执行一次即可
	
	// 配置定时器捕获输入通道2; Datasheet.CN:v10,p267
	TIM_ICInitStructure.TIM_Channel = TIMx_CHy;
	TIM_ICInitStructure.TIM_ICFilter = 							/* 滤波器设置, Datasheet.CN:v10.p290 */
		#if 	CPU_HCLK == 72000000
			BIN(1111);		// 1111：采样频率fSAMPLING=fDTS/32，N=8; 256, 281.25KHz
		#elif 	CPU_HCLK == 36000000
			BIN(1101);		// 1101：采样频率fSAMPLING=fDTS/32，N=5; 160, 225KHz
		#elif	CPU_HCLK == 24000000
			BIN(1100);		// 1100：采样频率fSAMPLING=fDTS/16，N=8; 128, 187.5KHz
		#elif	CPU_HCLK == 12000000
			BIN(1001);		// 1001：采样频率fSAMPLING=fDTS/8，N=8; 64,187.5KHz
		#elif	CPU_HCLK ==  8000000
			BIN(0111);		// 0111：采样频率fSAMPLING=fDTS/4，N=8; 32,250KHz
		#else
			#error	"Timer5.c TIM5_Config() Unknown CPU_HCLK!"	// 
		#endif
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = 0x0;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIMx, &TIM_ICInitStructure);
	
	// 中断向量及优先级设置
	NVIC_PriorityGroupConfig(TIMx_NVIC_PriorityGroup);
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMx_NVIC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMx_NVIC_PrioritySub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// DMA配置
	TIMx_CHy_DMA_Config();
	
	//TIM_DMAConfig(TIM_TypeDef * TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
	TIM_DMACmd(TIMx, TIM_DMA_CC2, ENABLE);
	//TIM_Cmd(UARTx_Rx_TIM, ENABLE); 
}

/*******************************************************************************
* 函数名: TIMx_IRQHandler	(在startup.s中向量表调用)
* 描述　: 定时器更新中断, 停止定时器并设置字符超时标志
* 输入　: 无
* 输出　: 无
* 返回　: 无
*******************************************************************************/
void TIMx_IRQHandler (void)
{
	if CHKBITs(TIMx->SR, TIM_FLAG_Update)		/* 检查更新中断标志位 */
	if CHKBITs(TIMx->DIER, TIM_FLAG_Update)		/* 检查中断使用位以排除软件更中断 */
	{
		/* 停止 DMAx Channelx,以停止计数 */
		CLRBITs(DMAx_CHy->CCR, (uint16_t)(~DMA_CCR1_EN));
		
		// 清除中断标志
		//TIM_ClearFlag(UARTx_Rx_TIM,TIM_FLAG_Update);
		CLRBITs(TIMx->SR, TIM_FLAG_Update);
		
		// 停止定时器
		TIMx_Stop();
		// 设置超时标志
	}
}

/*********************************************************************************************************
** 函数名称: GetFrequency
** 功能描述: 阻塞检测Timer5 CH2频率输入
** 输　入: 无
** 输　出: 无
** 返　回: TIM5 CH2 通道输入的频率
** 全局变量: 无
** 调用模块: 
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2011-04-15 09:06
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
uint32 GetTim5Ch2Frequency(void)
{
	// 先开启DMA通道
	TIM5_CH2_DMA_Start();
	// 开启定时器
	TIMx_Start();
	
	// 等待检测完成
	while(Tim5RunFlag == FALSE){
		WdtFeed();
	};
	
	return (nCounter * (1000 / TIMx_Update_Period_ms));
}

#endif	/* end: #if TIMER5_EN > 0 */

