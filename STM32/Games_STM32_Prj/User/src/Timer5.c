/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Timer5.c
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-04-13 17:44
**��        ��: ��ʱ��5���������������, CH2���봥��ʹ��DMA����
**			   ע: Ҫ�� CPU_HCLK >= 100KHz
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: Timer5.H
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
// ��������, Ӧ��������UARTʱ��Ҫ�޸ı��ζ���
#define  TIMx_Update_Period_ms		100		/* ��λ:ms, ��ʱ����������,���655ms */

#define  TIMx						TIM5
#define  TIMx_IRQn					TIM5_IRQn
#define  TIMx_RCCCmd(s) 			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, s)/* ʱ�� */
#define  TIMx_NVIC_PriorityGroup	NVIC_PriorityGroup_0
#define  TIMx_NVIC_Priority			0
#define  TIMx_NVIC_PrioritySub		0
void 	 TIM5_IRQHandler(void);
#define  TIMx_IRQHandler			TIM5_IRQHandler			/* ��ʱ���жϴ����� */
#define  TIMx_CHy					TIM_Channel_2
#define  TIMx_CH2_Remap()					/* TIM5 CH2��ӳ�� */
#define  TIMx_CH2_GPIO				GPIOA			/* PA1,TIM5 CH2���ڶ˿� */
#define  TIMx_CH2_Pin				GPIO_Pin_1		/* PA1 */
#define  TIMx_CH2_RCC				(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)
#define  TIMx_CH2_RCC_Cmd(s)		RCC_APB2PeriphClockCmd(TIMx_CH2_RCC, s)

#define  DMAx						DMA2
#define  DMAx_CHy 					DMA2_Channel4
#define  DMAx_RCC					RCC_AHBPeriph_DMA2
#define  DMAx_NVIC_PriorityGroup	NVIC_PriorityGroup_0
#define  DMAx_NVIC_Priority			0
#define  DMAx_NVIC_PrioritySub		0

// �������Կ����ж϶���
#define  COLOR_ENTER_CRITICAL()		/* ���ж� */
#define  COLOR_EXIT_CRITICAL()		/* ���ж� */

/***************************************************************************************************/
// �ֲ�����
#define 		DMA_INIT_Cnt	0xFFFF		/* DMA��ʼ����ֵ */
static uint32 	nCounter = 0;			// �ۼ�DMA��������,��CH2��������
static uint8	Tim5RunFlag = FALSE;	// ���ö�ʱ��������־�����������м���ǲ������
static uint16 	nDMATransmit = 0;		// DMA��ʼ����������
static uint16	nDMADstMem;				// ��ʱ����

/***************************************************************************************************/
// ����

/*********************************************************************************************************
** ��������: TIMx_CHy_DMA_Config
** ��������: ��ʼ������DMA
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2011-04-14 09:52
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static void TIMx_CHy_DMA_Config(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(DMAx_RCC, ENABLE);
	
	/* DMAx_CHy (triggered by DMA�¼� event) Config */
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
	
	// ������������ж�
	DMA_ITConfig(DMAx_CHy, DMA_IT_TC, ENABLE);
	
	// �ж���������
	NVIC_PriorityGroupConfig(DMAx_NVIC_PriorityGroup);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMAx_NVIC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMAx_NVIC_PrioritySub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// ���ڿ���DMA����, �ɷ��ͺ������ÿ���
	/* Enable USARTy DMA TX request */
	//USART_DMACmd(UARTx, USART_DMAReq_Tx, ENABLE);
	/* Enable USARTy TX DMA1 Channel */
	//DMA_Cmd(DMAx_CHy, ENABLE);
	
}

/*********************************************************************************************************
** ��������: TIM5_CH2_DMA_Start
** ��������: ����DMA����������
** �䡡��: ��
** �䡡��: ��
** ������: ������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-14 10:45
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static void TIM5_CH2_DMA_Start(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	/* DMA.Datashett.CN: p147 */
	//register uint32_t N;
	//register uint8 *pOut, *pEnd;
	
	/* Disable the selected DMAy Channelx */
 	CLRBITs(DMAx_CHy->CCR, (uint16_t)(~DMA_CCR1_EN));
	
	/* ���ͻ����� */
	//DMAx_CHy->CMAR = (uint32_t)pDst;
	
	/* ���͵��ֽ��� */
	nDMATransmit = DMA_INIT_Cnt;
	DMAx_CHy->CNDTR = DMA_INIT_Cnt;
	
	/* ���¿�������DMAͨ�� */
	SETBITs(DMAx_CHy->CCR, DMA_CCR1_EN);
	
	/* ����UART����DMA���� */
	//UARTx->CR3 |= USART_DMAReq_Rx;	// �����ѿ���
}

/*********************************************************************************************************
** ��������: DMA2_CH4_IRQHandler
** ��������: DMA2_CH4_�жϴ���, �亯�������ⲿ���ã�����ʱ��Ҫע��ͬ��
** �䡡��: ��
** �䡡��: ��
** ������: ������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-02 15:49
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void DMA2_CH4_IRQHandler (void)
{
	register t_ureg u32r;
	register t_ureg N;
	
	u32r = DMAx->ISR;
	if(u32r & DMA2_FLAG_TC4)	// DMA��������жϱ�־
	{
	    // ��ȡDMA�Ѵ���ֵ
		N = nDMATransmit;
		u32r = DMAx_CHy->CNDTR; /* ��ȡ��ǰδ������ */
		// ��������DMA 
		TIM5_CH2_DMA_Start();
		if(N > 0)
		{
			N -= u32r;
		}
		nCounter += N;
		
		COLOR_ENTER_CRITICAL();
	    COLOR_EXIT_CRITICAL();
	    
	    // ����жϱ�־
	    DMAx->IFCR = DMA2_FLAG_TC4;
	}
	
}

// ֹͣ��ʱ��
#define  TIMx_Stop() 			CLRBITs(TIMx->CR1, TIM_CR1_CEN); \
								COLOR_ENTER_CRITICAL(); \
								Tim5RunFlag = FALSE; \
								COLOR_EXIT_CRITICAL()

// ������ʱ��
#define  TIMx_Start()			TIMx->CNT = 0x0000; \
								SETBITs(TIMx->CR1, TIM_CR1_CEN); \
								COLOR_ENTER_CRITICAL(); \
								Tim5RunFlag = TRUE; \
								nDMATransmit = 0; \
								nCounter = 0; \
								COLOR_EXIT_CRITICAL()

/*********************************************************************************************************
** ��������: Timer5_Config
** ��������: ��ʱ��5��ʼ�����ú���, ���ڼ������Ƶ��, ����DMA����
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2011-04-14 09:52
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void TIM5_Config(void)
{
	/* Datasheet.CN
	** �߼����ƶ�ʱ��: p199, TIM1/TIM8
	** ������ʱ��: p298, TIM6/TIM7
	** ͨ�ö�ʱ��: p253, TIM2/3/4/5
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;
	TIM_ICInitTypeDef		TIM_ICInitStructure;
	
	/* Enable GPIO clock */
	TIMx_CH2_RCC_Cmd(ENABLE);
	
	/* ����CH2Ϊ���� */
	GPIO_InitStructure.GPIO_Pin = TIMx_CH2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(TIMx_CH2_GPIO, &GPIO_InitStructure);
	
	/* ʱ�� */
	TIMx_RCCCmd(ENABLE);
	/* �������мĴ���λ */
	TIM_DeInit(TIMx);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 
	TIM_TimeBaseInitStructure.TIM_Prescaler = (CPU_HCLK/100000)-1;					// Ԥ��ƵΪ1/00ms
	TIM_TimeBaseInitStructure.TIM_Period = TIMx_Update_Period_ms * 100;	// ARR
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);			// ѡ��Ϊ������ģʽ, ִ��һ�μ���
	
	// ���ö�ʱ����������ͨ��2; Datasheet.CN:v10,p267
	TIM_ICInitStructure.TIM_Channel = TIMx_CHy;
	TIM_ICInitStructure.TIM_ICFilter = 							/* �˲�������, Datasheet.CN:v10.p290 */
		#if 	CPU_HCLK == 72000000
			BIN(1111);		// 1111������Ƶ��fSAMPLING=fDTS/32��N=8; 256, 281.25KHz
		#elif 	CPU_HCLK == 36000000
			BIN(1101);		// 1101������Ƶ��fSAMPLING=fDTS/32��N=5; 160, 225KHz
		#elif	CPU_HCLK == 24000000
			BIN(1100);		// 1100������Ƶ��fSAMPLING=fDTS/16��N=8; 128, 187.5KHz
		#elif	CPU_HCLK == 12000000
			BIN(1001);		// 1001������Ƶ��fSAMPLING=fDTS/8��N=8; 64,187.5KHz
		#elif	CPU_HCLK ==  8000000
			BIN(0111);		// 0111������Ƶ��fSAMPLING=fDTS/4��N=8; 32,250KHz
		#else
			#error	"Timer5.c TIM5_Config() Unknown CPU_HCLK!"	// 
		#endif
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = 0x0;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIMx, &TIM_ICInitStructure);
	
	// �ж����������ȼ�����
	NVIC_PriorityGroupConfig(TIMx_NVIC_PriorityGroup);
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMx_NVIC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMx_NVIC_PrioritySub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// DMA����
	TIMx_CHy_DMA_Config();
	
	//TIM_DMAConfig(TIM_TypeDef * TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
	TIM_DMACmd(TIMx, TIM_DMA_CC2, ENABLE);
	//TIM_Cmd(UARTx_Rx_TIM, ENABLE); 
}

/*******************************************************************************
* ������: TIMx_IRQHandler	(��startup.s�����������)
* ������: ��ʱ�������ж�, ֹͣ��ʱ���������ַ���ʱ��־
* ���롡: ��
* �����: ��
* ���ء�: ��
*******************************************************************************/
void TIMx_IRQHandler (void)
{
	if CHKBITs(TIMx->SR, TIM_FLAG_Update)		/* �������жϱ�־λ */
	if CHKBITs(TIMx->DIER, TIM_FLAG_Update)		/* ����ж�ʹ��λ���ų�������ж� */
	{
		/* ֹͣ DMAx Channelx,��ֹͣ���� */
		CLRBITs(DMAx_CHy->CCR, (uint16_t)(~DMA_CCR1_EN));
		
		// ����жϱ�־
		//TIM_ClearFlag(UARTx_Rx_TIM,TIM_FLAG_Update);
		CLRBITs(TIMx->SR, TIM_FLAG_Update);
		
		// ֹͣ��ʱ��
		TIMx_Stop();
		// ���ó�ʱ��־
	}
}

/*********************************************************************************************************
** ��������: GetFrequency
** ��������: �������Timer5 CH2Ƶ������
** �䡡��: ��
** �䡡��: ��
** ������: TIM5 CH2 ͨ�������Ƶ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2011-04-15 09:06
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
uint32 GetTim5Ch2Frequency(void)
{
	// �ȿ���DMAͨ��
	TIM5_CH2_DMA_Start();
	// ������ʱ��
	TIMx_Start();
	
	// �ȴ�������
	while(Tim5RunFlag == FALSE){
		WdtFeed();
	};
	
	return (nCounter * (1000 / TIMx_Update_Period_ms));
}

#endif	/* end: #if TIMER5_EN > 0 */

