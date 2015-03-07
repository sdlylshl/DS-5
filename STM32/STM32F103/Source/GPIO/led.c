/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |   PC3 - LED1     |
 *          |   PC4 - LED2     |
 *          |   PC5 - LED3     |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
 **********************************************************************************/
#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 
 typedef enum
 { GPIO_Mode_AIN = 0x0,					//模拟输入
 GPIO_Mode_IN_FLOATING = 0x04, //浮空输入
 GPIO_Mode_IPD = 0x28,					//下拉输入
 GPIO_Mode_IPU = 0x48,					//上拉输入
 GPIO_Mode_Out_OD = 0x14,			//开漏输出
 GPIO_Mode_Out_PP = 0x10,			//推挽输出
 GPIO_Mode_AF_OD = 0x1C,				//开漏复用输出
 GPIO_Mode_AF_PP = 0x18				//推挽复用输出
 }GPIOMode_TypeDef;
 
 typedef enum
 { 
 GPIO_Speed_10MHz = 1,
 GPIO_Speed_2MHz, 
 GPIO_Speed_50MHz
 }GPIOSpeed_TypeDef;
 */

void LED_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体		

	RCC_APB2PeriphClockCmd(LED_RCC_PORT, ENABLE);
#if LED1_USE
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	//LED1(1);
#endif
#if LED2_USE
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //开漏输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
	LED2(0);
#endif
#if LED3_USE	
	GPIO_InitStructure.GPIO_Pin = LED3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);
	LED3(1);
#endif
#if LED4_USE
	GPIO_InitStructure.GPIO_Pin = LED4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED4_PORT, &GPIO_InitStructure);
	//LED4(1);
#endif
#if LED5_USE
	GPIO_InitStructure.GPIO_Pin = LED5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED5_PORT , &GPIO_InitStructure);
	//LED5(1);
#endif
#if LED6_USE
	GPIO_InitStructure.GPIO_Pin = LED6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED6_PORT , &GPIO_InitStructure);
	//LED6(1);	
#endif
#if LED7_USE	
	GPIO_InitStructure.GPIO_Pin = LED7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED7_PORT, &GPIO_InitStructure);
	//LED7(1);
#endif
#if LED8_USE
	GPIO_InitStructure.GPIO_Pin = LED8_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED8_PORT, &GPIO_InitStructure);
	//LED8(1);	
#endif
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
