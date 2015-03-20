#include "stm32f10x.h"
#include "stdio.h"
#include "config.h"
#include "util.h"
#include "ds1302.h"
#include "at24c16.h"
#include "adc.h"

#include <string.h>
#include <stdlib.h>
#include "i2c_ee.h"


sys_para sys;

#if 0
// val:1 output,      val:0 input
void lm3123_dbpin_changed(u8 val)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(val)
	{
		GPIO_InitStructure.GPIO_Pin   = LM3123_DB0 | LM3123_DB1 | LM3123_DB2 | LM3123_DB3 |LM3123_DB4 | LM3123_DB5 |LM3123_DB6 |LM3123_DB7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin   = LM3123_DB0 | LM3123_DB1 | LM3123_DB2 | LM3123_DB3 |LM3123_DB4 | LM3123_DB5 |LM3123_DB6 |LM3123_DB7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
}
#endif

unsigned char calcfcs(unsigned char *pmsg, unsigned char len)
{
	unsigned char result = 0;
	while (len--)
	{
		result ^= *pmsg++;
	}
	return result;
}


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // Port A output
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin   =  USART2_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Configure the USART2_Rx as input floating
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin   = USART2_RX;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//RS485 EN
	GPIO_InitStructure.GPIO_Pin	= RS485_EN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  // Configure the GPIO ports( USART1 Transmit and Receive Lines)
  // Configure the USART1_Tx as Alternate function Push-Pull
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin   =  USART1_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Configure the USART1_Rx as input floating
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin   = USART1_RX ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//SCREEN
	
	//MATRIX key ROW
	GPIO_InitStructure.GPIO_Pin   = PA0_A_B_C;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = PB0_1_2_3_F1 | PB1_4_5_6_F2 
		| PB10_7_8_9_F3 | PB11_STAR_0_WELL_F4 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//COL
	GPIO_InitStructure.GPIO_Pin   = PB12_F1_F2_F3_F4_C
		| PB13_3_6_9_WELL_B | PB14_2_5_8_0_A | PB15_1_4_7_STAR;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	 //Port B bidirection SDA
  GPIO_InitStructure.GPIO_Pin = I2C_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //Port B SCK
  GPIO_InitStructure.GPIO_Pin = I2C_SCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//BEEP
	GPIO_InitStructure.GPIO_Pin   = BEEP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


//KEY LED
  GPIO_InitStructure.GPIO_Pin	= KEY_LED ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, KEY_LED);

//1602A
  GPIO_InitStructure.GPIO_Pin	= L_1602A_RS | L_1602A_RW | L_1602A_E ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, L_1602A_RS);


  GPIO_InitStructure.GPIO_Pin	= LCD_LED ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, LCD_LED);


  GPIO_InitStructure.GPIO_Pin	= L_1602A_D0 | L_1602A_D1 | L_1602A_D2 | L_1602A_D3 |L_1602A_D4 | L_1602A_D5 |L_1602A_D6 |L_1602A_D7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//LED5
	GPIO_InitStructure.GPIO_Pin   = LED5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, LED5);

    //LED
	GPIO_InitStructure.GPIO_Pin   = LED0_P| LED0_S| LED1_P|LED1_S|LED2_P| LED2_S| LED3_P|LED3_S;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

#if 0
	GPIO_InitStructure.GPIO_Pin   = SIM900A_POW | SIM900A_RST ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  
  // Port B
  GPIO_InitStructure.GPIO_Pin   = WIZ_SCS ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOA, WIZ_SCS);
 
  
  // SPI 1
  /* Configure SPIy pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin   = WIZ_SCLK | WIZ_MISO | WIZ_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = WIZ_INT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  GPIO_InitStructure.GPIO_Pin	= WIZ_RESET ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, WIZ_RESET);
  
  //Port B bidirection SDA
  GPIO_InitStructure.GPIO_Pin = I2C_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //Port B SCK
  GPIO_InitStructure.GPIO_Pin = I2C_SCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   =  USART3_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configure the USART3_Rx as input floating
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin   = USART3_RX;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//ALARM OUT
	GPIO_InitStructure.GPIO_Pin   = IO_OUTPUT_4|IO_OUTPUT_5|IO_OUTPUT_6|IO_OUTPUT_7;
										
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//ALARM OUT
	GPIO_InitStructure.GPIO_Pin   = IO_OUTPUT_0|IO_OUTPUT_1|IO_OUTPUT_2|IO_OUTPUT_3;
										
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  	//ALARM IN
	GPIO_InitStructure.GPIO_Pin   = IO_INPUT_0|IO_INPUT_1|IO_INPUT_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = IO_INPUT_3 | IO_INPUT_4 |IO_INPUT_5|IO_INPUT_6|
										IO_INPUT_7 | IO_INPUT_8 |IO_INPUT_9|IO_INPUT_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = IO_INPUT_11|IO_INPUT_12|IO_INPUT_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = IO_INPUT_14|IO_INPUT_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	//KEY
	GPIO_InitStructure.GPIO_Pin   = KEY0|KEY1|KEY2|KEY3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//BEEP
	GPIO_InitStructure.GPIO_Pin   = BEEP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	  //LED
	GPIO_InitStructure.GPIO_Pin   = LED0_P| LED0_S| LED1_P|LED1_S|LED2_P| LED2_S| LED3_P|LED3_S;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
#endif
}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
    
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  
  /* TIM2 clock enable */
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3 |RCC_APB1Periph_USART2 , ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
                         |RCC_APB2Periph_AFIO  | RCC_APB2Periph_USART1 |RCC_APB2Periph_ADC1 |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
  
  
  
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
  
#else  /* VECT_TAB_FLASH  */
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the USART3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable the TIM2 global Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); 
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x20000);
  
  /* Set the Vector Table base location at 0x08002000 -> USE AIP*/ 
  // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);  
  // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);  
#endif
}



void Timer_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  /* Time base configuration */	
  TIM_TimeBaseStructure.TIM_Period = 1000;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;      
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, 71, TIM_PSCReloadMode_Immediate);
  
  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  
  
}

u8 beep_off()
{
	GPIO_WriteBit(GPIOC, BEEP, Bit_RESET);
	return OK;
}

u8 beep_on()
{	
	GPIO_WriteBit(GPIOC, BEEP, Bit_SET);
	return OK;
}


int led_stat()
{	
	static uint32_t led_time=0;
	static u8 is_green=0;
	if(time_return() - led_time >= (10)) 
	{  	
		if(!is_green)
		{	
			key_stat(1);
			//led_green(0);
			//led_green(1);
			//led_green(2);
			//led_green(3);
			is_green = 1;
		}
		else
		{
			key_stat(0);
			//led_off(0);
			//led_off(1);
			//led_off(2);
			//led_off(3);
			is_green = 0;
		}
		led_time = time_return();
	}

	
	return OK;
}


void matrix_event()
{
	static uint32_t matrix_time=0;
	static u8 led_on;
	static u8 led_on_flag;
	
	if(time_return() - matrix_time >= (2)) 
	{  	
		led_on = 0;
		scan_matrix_key(&led_on);
		if(led_on)
		{
			GPIO_SetBits(GPIOC, KEY_LED);
			led_on_flag = 0;
		}
		else
		{
			led_on_flag++;
			if(led_on_flag>50)
				GPIO_ResetBits(GPIOC, KEY_LED);
		}
		matrix_time = time_return();
	}
}




void do_para(sys_para sys)
{
	if(sys.arming)
		display_arm();
	else
		display_disarm();
}


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{  
	//int i;
	//mode: 1:udp mode 0:gprs mode
	u8 mode=1;
	int ret;
	uint32_t gsm_init_time;
	static u8 l_gsm_init=0;


	//while(1)
	//	Delay_ms(500);
	
	//char cell_num[12];
	RCC_Configuration(); // Configure the system clocks
	NVIC_Configuration(); // NVIC Configuration
	GPIO_Configuration();

	//LED5_onoff(ON);
	//RTC_init();
	DS1302_Init();
	//Adc_Init();
	beep_on();
	Delay_ms(1000);
	beep_off();
	
	//DS1302_Init();
	USART1_Init();
	USART2_Init();
	//USART3_Init();
	//Timer_Configuration();

	//Reset_W5500();
	//reset_sim900a();
	//WIZ_SPI_Init();
	//get_para(&sys);
	
	//Set_network(&sys);// network config & Call Set_network ();
	//set_udp_utils(); // For TCP client's connection request delay
	//set_current_time(&gsm_init_time);
	//GetSerialNum(serial);
	//for(i=0; i<12; i++)
	//	printf("serial[%d]: 0x%x\n", i, serial[i]);

	//led_test(0);

	//flash_save();
	//auto_keep_alive(4);
	//alarm_out_test();
	//gsm_test();

	//I2C_Test();
	ds1302_set_time_test();
	ds1302_get_time_test();
	//set_alarm_out(3, 1);
	//lcd_test();
	//l_1602a_test();
	
	
	LCD1602_Init(); 
	//while(1);
	rs485_init();
	key_in_test();
	do_para(sys);
	//l_1602a_test();
	while(1)
	{	
		led_stat();
		matrix_event();
		handle_key_event();
		parse_rs485_cmd(&ret);
		delay_arm();
		deal_485_cmd(ret);
		//Delay_ms(500);
		//rs485_test();
	}
	
}



