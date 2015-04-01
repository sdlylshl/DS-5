/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: IOInput.c
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-04-23 14:28
**��        ��: ��������⺯��, ʹ���жϼ���ʱ��ɨ����
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: IOInput.h
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
// ��������, Ӧ��������UARTʱ��Ҫ�޸ı��ζ���
#define  TIMx_Update_Period_us		500		/* ��λ:ms, ��ʱ����������,���655ms, ��СΪ1/100ms */

#define  TIMx						TIM2					/* ��ʱ��,ʹ���˸����ж� */
#define  TIMx_IRQn					TIM2_IRQn
#define  TIMx_RCCCmd(s) 			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, s)/* ʱ�� */
#define  TIMx_NVIC_PriorityGroup	NVIC_PriorityGroup_4
#define  TIMx_NVIC_Priority			0
#define  TIMx_NVIC_PrioritySub		0
void 	 TIM2_IRQHandler(void);
#define  TIMx_IRQHandler			TIM2_IRQHandler			/* ��ʱ���жϴ����� */


// �������Կ����ж϶���
//#define  INPUT_ENTER_CRITICAL()		/* ���ж� */
//#define  INPUT_EXIT_CRITICAL()		/* ���ж� */


/***************************************************************************************************/
// ��������
typedef void t_IOIN_EVENT_DO(T_IOEvent);
typedef uint8 t_IOIN_Counter;
typedef struct t_IOIN_INFO_
{
		uint8		ID;					// �����ź�����
	T_ValidVoltage	ValidVoltage;		// ��Ч��ƽ
	__IO uint32_t	*IDR;				// ��������ָ��
		uint32_t	PinBitMsk;			// ��������λ����
	t_IOIN_Counter 	CntValid;			// ��Ч״̬����, ע������ı�ValidVoltage, ����Ӧ��Ҫ�޸Ĵ˲���
	t_IOIN_Counter	CntInvalid;			// ��Ч״̬����, ע������ı�ValidVoltage, ����Ӧ��Ҫ�޸Ĵ˲���
	t_IOIN_EVENT_DO *EventHandler;		// ִ�к���ָ��
}t_IOIN_INFO;

static const t_IOIN_INFO IOIN_InfoTbl[IOIN_NUMS] = 
{
	// index,			ValidVol,  GPIO->IDR,  IO_MASK, CNT, CNT~, EventHandler
	{IOIN_Key1, 		VOL_Low,  &GPIOB->IDR, BIT( 0), 4, 4, NULL },
	{IOIN_Key2, 		VOL_Low,  &GPIOB->IDR, BIT( 1), 4, 4, NULL },
	{IOIN_Key3, 		VOL_Low,  &GPIOB->IDR, BIT( 6), 4, 4, NULL },
	{IOIN_Key4, 		VOL_Low,  &GPIOB->IDR, BIT( 7), 4, 4, KeyESC_Event },
	{IOIN_CenterSensor, VOL_High, &GPIOC->IDR, BIT( 7),10,10, ColorSensorEvent },	// ʹ�䳣̬��ƽ5ms�Ƴ�����ɫ�����(�Լ��), ��Ч��ƽ2ms��������ɫ��
	{IOIN_VacuumHousing,VOL_Low,  &GPIOE->IDR, BIT(12), 4,10, NULL },
	{IOIN_BallCounter, 	VOL_Low,  &GPIOE->IDR, BIT(15), 4, 4, NULL },
	{IOIN_Spin, 		VOL_Low,  &GPIOE->IDR, BIT(14), 3, 3, NULL },
	{IOIN_ColorRed, 	VOL_Low,  &GPIOD->IDR, BIT( 8),16, 6, NULL },	// ʹ��̬��ƽ3ms��ǰ�����ϴ�����, ��Ч��ƽ8ms�Ƴ����ڶϴ�����
	{IOIN_ColorGreen, 	VOL_Low,  &GPIOD->IDR, BIT( 9),16, 6, NULL },
	{IOIN_ColorBlue, 	VOL_Low,  &GPIOD->IDR, BIT(10),16, 6, NULL }
};

/***************************************************************************************************/
// �ֲ�����
// ������״̬
volatile T_IOInState g_InputState[IOIN_NUMS] = {
				IN_Invalid, IN_Invalid, IN_Invalid, IN_Invalid,
				IN_Invalid, IN_Invalid, IN_Invalid, IN_Invalid, 
				};
// �������¼�����
volatile uint8 g_InputDownEvents[IOIN_NUMS] = {0, };
volatile uint8 g_InputUpEvents[IOIN_NUMS] = {0, };
// �����ʱ��
static uint32 nTimer_ms = 0;

/***************************************************************************************************/
// ����

/*********************************************************************************************************
** ��������: TIM2_Config
** ��������: ��ʱ��2��ʼ�����ú���, ���������������˲�
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2011-04-23 14:51
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void TIM2_Config(void)
{
	/* Datasheet.CN
	** �߼����ƶ�ʱ��: p199, TIM1/TIM8
	** ������ʱ��: p298, TIM6/TIM7
	** ͨ�ö�ʱ��: p253, TIM2/3/4/5
	*/
	//GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;
	//TIM_ICInitTypeDef		TIM_ICInitStructure;
	
	TIMx_RCCCmd(ENABLE);
	
	TIM_DeInit(TIMx);
	
	// �ж����������ȼ�����
	NVIC_PriorityGroupConfig(TIMx_NVIC_PriorityGroup);
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMx_NVIC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMx_NVIC_PrioritySub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = (CPU_HCLK/100000)-1;		// Ԥ��ƵΪ1/100ms
	TIM_TimeBaseInitStructure.TIM_Period = (TIMx_Update_Period_us * 100 / 1000) -1;	// ARR
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	//TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);					// ѡ��Ϊ������ģʽ
	
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
		/* �����ǰ״̬��ԭ����״̬��һ�� */ 
		
		uReg = *pCnt; 
		*pCnt = ++uReg; 			/* InputCnt[IOIN_CNT_KEY1]++ */ 
		Count = (isValid==IN_Valid? pIOIN->CntValid
											  : pIOIN->CntInvalid);
		if(uReg == Count)
		{
			pEventDo = (t_IOIN_EVENT_DO *)pIOIN->EventHandler;
			g_InputState[index] = isValid; 
			if(isValid){ 	/* ��Ϊ��Ч̬ */ 
				g_InputDownEvents[index]++; 
				_DbgPrintf_(( "[%u,IN.%u=%d,%u,%u] ", nTimer_ms % 10000, index, isValid, 
							g_InputUpEvents[index], g_InputDownEvents[index]));
				if(pEventDo != NULL){
					pEventDo(IN_Event_Down); 
				}
			}else{ 			/* ��Ϊ��Ч̬ */ 
				g_InputUpEvents[index]++; 
				_DbgPrintf_(( "[%u,IN.%u=%d,%u,%u] ", nTimer_ms % 10000, index, isValid, 
							g_InputUpEvents[index], g_InputDownEvents[index]));
				if(pEventDo != NULL){
					pEventDo(IN_Event_Up); 
				}
			} 
		} 
	}else{ 
		/* ״̬һ������������� */ 
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
		/* �����ǰ״̬��ԭ����״̬��һ�� */ 
		uReg = *pCnt; 
		*pCnt = ++uReg; 			/* InputCnt[IOIN_CNT_KEY1]++ */ 
		if(uReg == (g_InputState[index]==IN_Valid? Count: CountN) ) 
		{ 
			g_InputState[index] = isValid; 
			pEventDo = (t_IOIN_EVENT_DO *)IOIN_EVENT_Do_Handlers[index];
			if(isValid){ 	/* ��ǰ����Ч̬��Ϊ��Ч̬ */ 
				g_InputDownEvents[index]++; 
				if(pEventDo != NULL){
					pEventDo(IN_Event_Down); 
				}
			}else{ 			/* ��ǰ����Ч̬��Ϊ��Ч̬ */ 
				g_InputUpEvents[index]++; 
				if(pEventDo != NULL){
					pEventDo(IN_Event_Up); 
				}
			} 
		} 
	}else{ 
		/* ״̬һ������������� */ 
		*pCnt = 0; 
	}
}
#endif
#define  CHK_Input(name)	\
		 __CHK_Input(IOIN_##name)
#define  CHK_Input2(name, isValid, Count, CountN)	\
		 __CHK_Input(IOIN_##name, isValid)
/*******************************************************************************
* ������: TIMx_IRQHandler	(��startup.s�����������)
* ������: ��ʱ�������ж�, ɨ��������
* ���롡: ��
* �����: ��
* ���ء�: ��
*******************************************************************************/
void TIMx_IRQHandler (void)//TIM2_IRQHandler (void)
{
	//static uint8 Cnt[IOIN_NUMS] = {0, };		// �˲�������
	//register uint8 uReg, *pCnt;
	//register t_bool isValid;
	// �������˲�������
		
	//register uint32 u32r;
	
	if CHKBITs(TIMx->SR, TIM_FLAG_Update)		/* �������жϱ�־λ */
	if CHKBITs(TIMx->DIER, TIM_FLAG_Update)		/* ����ж�ʹ��λ���ų�������ж� */
	{
		// ����жϱ�־
		CLRBITs(TIMx->SR, TIM_FLAG_Update);
		
		// �ۼ�ʱ��
		nTimer_ms++;
		#ifdef DEBUG_nTimer_ms
		if(nTimer_ms % 2000 == 0)
		{
			_DbgPrintf_(( "[T:%d]", nTimer_ms ));
		}
		#endif
		
		#if COLOR_HARD == COLOR_HARD_RGB
		// ��ɫ��: ��
		CHK_Input(ColorRed);
		// ��ɫ��: ��
		CHK_Input(ColorGreen);
		// ��ɫ��: ��
		CHK_Input(ColorBlue);
		#endif
		// ��ɫ���������ϼ���, ע��ú����ĵ���λ��
		CHK_Input(CenterSensor);

		if((nTimer_ms % 2) == 1)
		{
			
			// �������
			// �������
			CHK_Input(BallCounter);
			// ƽ����
			CHK_Input(Spin);

			// 5ms���һ��
			if((nTimer_ms % 10) == 1)
			{
				// ����1
				CHK_Input(Key1);
				// ����2
				CHK_Input(Key2);
				// ����3
				CHK_Input(Key3);
				// ����4
				CHK_Input(Key4);
				// 
				CHK_Input(VacuumHousing);
			}
		}
	}
}


/*******************************************************************************
* ������: InputInit
* ������: ��ʼ��������ɨ�躯��,����������TIM2_Config��ʼ��֮��20ms����Щ������ʼ��
* ���롡: ��
* �����: ��
* ���ء�: ��
*******************************************************************************/
void InputInit(void)
{
	OS_TimeDly(OS_TICKS_PER_SEC / 50);
	memset((void *)g_InputDownEvents, 0, sizeof(g_InputDownEvents));
	memset((void *)g_InputUpEvents, 0, sizeof(g_InputUpEvents));
}

#endif	// end: #if IOINPUT_EN > 0 //

