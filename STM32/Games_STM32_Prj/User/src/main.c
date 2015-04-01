
#define MAIN_GLOBALS
#include "Config.h"

/*
static void RCC_Init()
{
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
}

static void GPIO_B_Init()
{

GPIO_InitTypeDef GPIO_InitStructure;

GPIO_DeInit(GPIOB);

 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
GPIO_Init(GPIOB, &GPIO_InitStructure); 

}


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
*/

uint32 g_Dbg_u32;


void DeviceInit(void)
{
	/* STM32_Init.c  ��ʼ����������:ʱ������,GPIO�������, ������ӳ�书��, ϵͳʱ��, 
	** ����ӳ���,�ⲿ�ж� */
	stm32_Init();
	/* MCU IO ��ʼ״̬ */
	IO_Init();
	/* �ⲿ���߳�ʼ�� */
	FSMC_Config();
	/* �ⲿ��չ���,LCD�ӿڵȳ�ʼ�� */
	X_IO_Init();
	
	/* NVIC configuration */
	NVIC_Config();
	/* �����������,SPI��ʼ������ */
	FRAM_Config();
	/* CAN����,CAN������,CAN�շ����г�ʼ�� */
	CAN_Config();
	/* ��ʱ��5���� */
	TIM5_Config();
	
	/* TIM2��ʱ����ʼ��:ȫ�ֶ�ʱ��������ɨ�� */
	TIM2_Config();
	
	/* UART1~4��ʱ����ʼ�� */
//	UART1T4_Rx_TIM_Config();
	/* UARt1 */
	UART1_Config();
	/* UART4 */
	UART4_Config();
	
	/* ���������ʼ�� */
	TargetInit();
	
	/* ��ʼ��������ر���,  */
	InputInit();
	
	/* �����ʼ�� */
	/* ������ʼ�� */
	ParamInit();
	/* ��ʷ��¼��ʼ��,�������TIM2�жϿ���,��TIM2_Config()ִ��֮�󳬹�20ms���� */
	HistoryInit();
	
	LCD_Init();
}

t_err g_Error = ERR_NONE;
#ifdef DEBUG
//static const char szAppTitle0[] = "Dolphin Games\r\n";
static const char szAppTitle[] = "Dolphin Games Test2 Line2 textet CAN_Config()TIM2_Config();" \
								 "UART1_Config();UART4_Config();TargetInit();InputInit();" \
								 "TargetInit();InputInit();ParamInit();InputInit();ParamInit();" \
								 "HistoryInit();HistoryInit();" \
								 "<CR><LF>\r\n";
static uint8 dbgBuf[128];

void Debug(void)
{
	char *pStr;
	uint N, N1;

	//t_err err;
	
	Delay_Nms(1);
	
	_DbgPrintf_(( "Debug()\r\n" ));
	pStr = (char *)szAppTitle;
	while(pStr < &szAppTitle[STRLEN(szAppTitle)] )
	{
		OS_ENTER_CRITICAL();
		N = Uart1CanSendBytes();
		if(N > 0) N--;					// ����ʹ���е���
		if(N > 0)
		{
			N--;
			
			N1 = &szAppTitle[STRLEN(szAppTitle)] - pStr;
			if(N1 > N){
				N1 = N;
			}
			UART1_SendBytes(pStr, N1);
			pStr += N1;
		}
		OS_EXIT_CRITICAL();
		
		Delay_Nus(1);
	}
	
#ifdef DEBUG_SendRcvd
	while(1)
	{
		OS_ENTER_CRITICAL();
		
		N = UART1_RcvdSize();
		if(N > 0)
		{
			if(N > sizeof(dbgBuf))
			{
				N = sizeof(dbgBuf);
			}
			N1 = Uart1CanSendBytes();
			if(N > N1)
			{
				N = N1;
			}
			if(N > 0)
			{
				UART1_ReadBytes(dbgBuf, N);
				UART1_SendBytes(dbgBuf, N);
			}
		}
		
		OS_EXIT_CRITICAL();
		
		Delay_Nus(1);
		//OS_TimeDly(OS_TICKS_PER_SEC / 5);
		NOP(); NOP();
	}
#endif	// end: #ifdef DEBUG_SendRcvd
	
#ifdef DEBUG_LoopSend
	while(1)
	{
		_DbgPrintf_(( "Dolphin Games\r\n" ));
		pStr = (char *)szAppTitle;
		while(pStr < &szAppTitle[STRLEN(szAppTitle)] )
		{
			OS_ENTER_CRITICAL();
			N = Uart1CanSendBytes();
			if(N > 0) N--;					// ����ʹ���е���
			if(N > 0)
			{
				N--;
				
				N1 = &szAppTitle[STRLEN(szAppTitle)] - pStr;
				if(N1 > N){
					N1 = N;
				}
				UART1_SendBytes(pStr, N1);
				pStr += N1;
			}
			OS_EXIT_CRITICAL();
			Delay_Nus(1);
		}
		Delay_Nus(1);
		//OS_TimeDly(OS_TICKS_PER_SEC / 5);
		NOP();
	}
#endif	// end: #ifdef LoopSend
	
}

#endif	// end: #ifdef DEBUG


int main(void)
{
	//register uint32 r;
	//SYS_ReqStateRead(r);
	
	#ifdef DEBUG
	//char *pStr;
	//uint N, N1;
	
	g_Dbg_u32 = sizeof(g_History);
	g_Dbg_u32 = sizeof(g_Param);
	g_Dbg_u32 = sizeof(T_DataStore);
	#endif
	
	DeviceInit();
	
	//LCD_SdStringPos("Dolphin Games", 1, 0);
	_DBG_( LCD_SdStringPos("Dolphin", 0, 0) );
	
	//_DBG_( while(1) );
	
	#ifdef DEBUG
	Debug();
	/*
	Delay_Nms(1);
	while(1)
	{
		N = Uart1CanSendBytes();
		_DbgPrintf_(( "N:%u,", N ));
		Delay_Nus(1);
		NOP();
	}
	*/
	#endif
	
	SYS_StateSet(SYS_STATE_Init);
	g_Error = SysGameInit();
	if(g_Error != ERR_NONE)
	{
		SYS_StateSet(SYS_STATE_Error);
		
		_DbgPrintf_(( "TGmErr:0x%04X\r\n", g_Error ));
		_DbgLcdPrintf_(( 1, 0, "TGmErr:%04X", g_Error ));
	}
	while (1)
	{
		
		if(g_Error == ERR_NONE)
		{
			SYS_StateSet(SYS_STATE_Game);
			
			g_Error = Game();
			
			if(g_Error != ERR_NONE)
			{
				SYS_StateSet(SYS_STATE_Error);
				
				_DbgPrintf_(( "TGmErr:%04X", g_Error ));
				_DbgLcdPrintf_(( 1, 0, "TGmErr:%04X", g_Error ));
			}
		}
		else
		{
			
		}
		
		// ��������������������������ģʽ
		if(SYS_ReqStateGet() == SYS_STATE_Config)
		{
			SYS_StateSet(SYS_STATE_Config);
			// ��������ģʽѭ������
			
			// ���ó�ʼ״̬
			//SYS_StateSet(SYS_STATE_Init);
		}

		#ifdef DBG_COLOR_STAT_F				// ��ɫ���Ա�������
		if(g_Error != ERR_NONE)
		{
			DbgPrintf("��Err:%04X\r\n", g_Error);
			g_Error = ERR_NONE;
			
			GameTest();
		}
		#endif
	}
}

#ifdef  USE_FULL_ASSERT


static void assert_failed(uint8_t* file, uint32_t line)
{ 

	while (1)
	{
	}
}

#endif	// end: #ifdef USE_FULL_ASSERT


