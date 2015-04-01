/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: IOInput.c
**创   建   人: Xsky
**创 建 日  期: 2011-04-23 14:28
**描        述: 输入量检测函数, 使用中断及定时器扫描检测
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: IOInput.h
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

#define  IOINPUT_GLOBALS
#include "Stm32f10x.h"
#include "String.h"
#include "Global.h"
#include "Target.h"
#include "IOInput.h"
#include "Delay.h"
#include "Small_LCD.h"
#include "Function.h"
#include "Game.h"

#ifndef IOINPUT_EN
#define IOINPUT_EN		0
#endif

#if IOINPUT_EN > 0

/***************************************************************************************************/
// 常数定义, 应用于其它UART时需要修改本段定义
#define  TIMx_Update_Period_us		500		/* 单位:ms, 定时器更新周期,最大655ms, 最小为1/100ms */

#define  TIMx						TIM2					/* 定时器,使用了更新中断 */
#define  TIMx_IRQn					TIM2_IRQn
#define  TIMx_RCCCmd(s) 			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, s)/* 时钟 */
#define  TIMx_NVIC_PriorityGroup	NVIC_PriorityGroup_4
#define  TIMx_NVIC_Priority			0
#define  TIMx_NVIC_PrioritySub		0
void 	 TIM2_IRQHandler(void);
#define  TIMx_IRQHandler			TIM2_IRQHandler			/* 定时器中断处理函数 */


// 可重入性开关中断定义
//#define  INPUT_ENTER_CRITICAL()		/* 关中断 */
//#define  INPUT_EXIT_CRITICAL()		/* 开中断 */


/***************************************************************************************************/
// 常量表定义
typedef void t_IOIN_EVENT_DO(T_IOEvent);
typedef uint8 t_IOIN_Counter;
typedef struct t_IOIN_INFO_
{
		uint8		ID;					// 输入信号索引
	T_ValidVoltage	ValidVoltage;		// 有效电平
	__IO uint32_t	*IDR;				// 输入引脚指针
		uint32_t	PinBitMsk;			// 输入引脚位掩码
	t_IOIN_Counter 	CntValid;			// 有效状态计数, 注意如果改变ValidVoltage, 此相应需要修改此参数
	t_IOIN_Counter	CntInvalid;			// 无效状态计数, 注意如果改变ValidVoltage, 此相应需要修改此参数
	t_IOIN_EVENT_DO *EventHandler;		// 执行函数指针
}t_IOIN_INFO;

static const t_IOIN_INFO IOIN_InfoTbl[IOIN_NUMS] = 
{
	// index,			ValidVol,  GPIO->IDR,  IO_MASK, CNT, CNT~, EventHandler
	{IOIN_Key1, 		VOL_Low,  &GPIOB->IDR, BIT( 0), 4, 4, NULL },
	{IOIN_Key2, 		VOL_Low,  &GPIOB->IDR, BIT( 1), 4, 4, NULL },
	{IOIN_Key3, 		VOL_Low,  &GPIOB->IDR, BIT( 6), 4, 4, NULL },
	{IOIN_Key4, 		VOL_Low,  &GPIOB->IDR, BIT( 7), 4, 4, KeyESC_Event },
	{IOIN_CenterSensor, VOL_High, &GPIOC->IDR, BIT( 7),10,10, ColorSensorEvent },	// 使其常态电平5ms推迟于颜色检测线(以检测), 有效电平2ms提早于颜色线
	{IOIN_VacuumHousing,VOL_Low,  &GPIOE->IDR, BIT(12), 4,10, NULL },
	{IOIN_BallCounter, 	VOL_Low,  &GPIOE->IDR, BIT(15), 4, 4, NULL },
	{IOIN_Spin, 		VOL_Low,  &GPIOE->IDR, BIT(14), 3, 3, NULL },
	{IOIN_ColorRed, 	VOL_Low,  &GPIOD->IDR, BIT( 8),16, 6, NULL },	// 使常态电平3ms提前于庶断传感器, 有效电平8ms推迟于遮断传感器
	{IOIN_ColorGreen, 	VOL_Low,  &GPIOD->IDR, BIT( 9),16, 6, NULL },
	{IOIN_ColorBlue, 	VOL_Low,  &GPIOD->IDR, BIT(10),16, 6, NULL }
};

/***************************************************************************************************/
// 局部变量
// 输入量状态
volatile T_IOInState g_InputState[IOIN_NUMS] = {
				IN_Invalid, IN_Invalid, IN_Invalid, IN_Invalid,
				IN_Invalid, IN_Invalid, IN_Invalid, IN_Invalid, 
				};
// 输入量事件记数
volatile uint8 g_InputDownEvents[IOIN_NUMS] = {0, };
volatile uint8 g_InputUpEvents[IOIN_NUMS] = {0, };
// 毫秒计时器
static uint32 nTimer_ms = 0;

/***************************************************************************************************/
// 正文

/*********************************************************************************************************
** 函数名称: TIM2_Config
** 功能描述: 定时器2初始化配置函数, 用于数字输入量滤波
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: 
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2011-04-23 14:51
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void TIM2_Config(void)
{
	/* Datasheet.CN
	** 高级控制定时器: p199, TIM1/TIM8
	** 基本定时器: p298, TIM6/TIM7
	** 通用定时器: p253, TIM2/3/4/5
	*/
	//GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;
	//TIM_ICInitTypeDef		TIM_ICInitStructure;
	
	TIMx_RCCCmd(ENABLE);
	
	TIM_DeInit(TIMx);
	
	// 中断向量及优先级设置
	NVIC_PriorityGroupConfig(TIMx_NVIC_PriorityGroup);
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMx_NVIC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMx_NVIC_PrioritySub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = (CPU_HCLK/100000)-1;		// 预分频为1/100ms
	TIM_TimeBaseInitStructure.TIM_Period = (TIMx_Update_Period_us * 100 / 1000) -1;	// ARR
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	//TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);					// 选择为单脉冲模式
	
	TIM_Cmd(TIMx, ENABLE);
}

static __inline void __CHK_Input(register uint index)	
{
	static t_IOIN_Counter InputCnt[IOIN_NUMS] = {0, };
	static T_IOInState LastState[IOIN_NUMS] = {
					IN_Invalid, IN_Invalid, IN_Invalid, IN_Invalid,
					IN_Invalid, IN_Invalid, IN_Invalid, IN_Invalid, 
					};
	
	register uint8 uReg, *pCnt;
	//register uint8 uReg1;
	register t_IOIN_EVENT_DO *pEventDo;
	register t_IOIN_Counter Count;
	register t_IOIN_INFO *pIOIN;
	register T_IOInState isValid;
	
	pCnt = &InputCnt[index]; 
	pIOIN = (t_IOIN_INFO *)&IOIN_InfoTbl[index];
	isValid = (T_IOInState)CHKBITs(*(pIOIN->IDR), pIOIN->PinBitMsk);
	if(pIOIN->ValidVoltage == VOL_Low) isValid = (T_IOInState)!isValid;
	//if(LastState[index] != isValid) *pCnt = 0;
	LastState[index] = isValid;
	if(isValid != g_InputState[index])	
	{
		/* 如果当前状态与原保存状态不一致 */ 
		
		uReg = *pCnt; 
		*pCnt = ++uReg; 			/* InputCnt[IOIN_CNT_KEY1]++ */ 
		Count = (isValid==IN_Valid? pIOIN->CntValid
											  : pIOIN->CntInvalid);
		if(uReg == Count)
		{
			pEventDo = (t_IOIN_EVENT_DO *)pIOIN->EventHandler;
			g_InputState[index] = isValid; 
			if(isValid){ 	/* 变为有效态 */ 
				g_InputDownEvents[index]++; 
				_DbgPrintf_(( "[%u,IN.%u=%d,%u,%u] ", nTimer_ms % 10000, index, isValid, 
							g_InputUpEvents[index], g_InputDownEvents[index]));
				if(pEventDo != NULL){
					pEventDo(IN_Event_Down); 
				}
			}else{ 			/* 变为无效态 */ 
				g_InputUpEvents[index]++; 
				_DbgPrintf_(( "[%u,IN.%u=%d,%u,%u] ", nTimer_ms % 10000, index, isValid, 
							g_InputUpEvents[index], g_InputDownEvents[index]));
				if(pEventDo != NULL){
					pEventDo(IN_Event_Up); 
				}
			} 
		} 
	}else{ 
		/* 状态一致则清除计数器 */ 
		*pCnt = 0; 
	}
}
#ifdef __CHK_Input2_FUN
static __inline __CHK_Input2(register uint index, register t_bool isValid,
							register uint Count, register uint CountN) 
{
	register uint8 uReg, *pCnt;
	register t_IOIN_EVENT_DO *pEventDo;
	//register uint8 isValid;
	
	pCnt = &InputCnt[index]; 
	if(isValid != g_InputState[index])	
	{
		/* 如果当前状态与原保存状态不一致 */ 
		uReg = *pCnt; 
		*pCnt = ++uReg; 			/* InputCnt[IOIN_CNT_KEY1]++ */ 
		if(uReg == (g_InputState[index]==IN_Valid? Count: CountN) ) 
		{ 
			g_InputState[index] = isValid; 
			pEventDo = (t_IOIN_EVENT_DO *)IOIN_EVENT_Do_Handlers[index];
			if(isValid){ 	/* 当前由无效态变为有效态 */ 
				g_InputDownEvents[index]++; 
				if(pEventDo != NULL){
					pEventDo(IN_Event_Down); 
				}
			}else{ 			/* 当前由有效态变为无效态 */ 
				g_InputUpEvents[index]++; 
				if(pEventDo != NULL){
					pEventDo(IN_Event_Up); 
				}
			} 
		} 
	}else{ 
		/* 状态一致则清除计数器 */ 
		*pCnt = 0; 
	}
}
#endif
#define  CHK_Input(name)	\
		 __CHK_Input(IOIN_##name)
#define  CHK_Input2(name, isValid, Count, CountN)	\
		 __CHK_Input(IOIN_##name, isValid)
/*******************************************************************************
* 函数名: TIMx_IRQHandler	(在startup.s中向量表调用)
* 描述　: 定时器更新中断, 扫描输入量
* 输入　: 无
* 输出　: 无
* 返回　: 无
*******************************************************************************/
void TIMx_IRQHandler (void)//TIM2_IRQHandler (void)
{
	//static uint8 Cnt[IOIN_NUMS] = {0, };		// 滤波计数器
	//register uint8 uReg, *pCnt;
	//register t_bool isValid;
	// 输入量滤波计数器
		
	//register uint32 u32r;
	
	if CHKBITs(TIMx->SR, TIM_FLAG_Update)		/* 检查更新中断标志位 */
	if CHKBITs(TIMx->DIER, TIM_FLAG_Update)		/* 检查中断使用位以排除软件更中断 */
	{
		// 清除中断标志
		CLRBITs(TIMx->SR, TIM_FLAG_Update);
		
		// 累计时间
		nTimer_ms++;
		#ifdef DEBUG_nTimer_ms
		if(nTimer_ms % 2000 == 0)
		{
			_DbgPrintf_(( "[T:%d]", nTimer_ms ));
		}
		#endif
		
		#if COLOR_HARD == COLOR_HARD_RGB
		// 颜色线: 红
		CHK_Input(ColorRed);
		// 颜色线: 绿
		CHK_Input(ColorGreen);
		// 颜色线: 蓝
		CHK_Input(ColorBlue);
		#endif
		// 颜色传感器庶断计数, 注意该函数的调用位置
		CHK_Input(CenterSensor);

		if((nTimer_ms % 2) == 1)
		{
			
			// 毫秒操作
			// 数球光眼
			CHK_Input(BallCounter);
			// 平衡器
			CHK_Input(Spin);

			// 5ms检测一次
			if((nTimer_ms % 10) == 1)
			{
				// 按键1
				CHK_Input(Key1);
				// 按键2
				CHK_Input(Key2);
				// 按键3
				CHK_Input(Key3);
				// 按键4
				CHK_Input(Key4);
				// 
				CHK_Input(VacuumHousing);
			}
		}
	}
}


/*******************************************************************************
* 函数名: InputInit
* 描述　: 初始化输入量扫描函数,必须在输入TIM2_Config初始化之后20ms调用些函数初始化
* 输入　: 无
* 输出　: 无
* 返回　: 无
*******************************************************************************/
void InputInit(void)
{
	OS_TimeDly(OS_TICKS_PER_SEC / 50);
	memset((void *)g_InputDownEvents, 0, sizeof(g_InputDownEvents));
	memset((void *)g_InputUpEvents, 0, sizeof(g_InputUpEvents));
}

#endif	// end: #if IOINPUT_EN > 0 //

