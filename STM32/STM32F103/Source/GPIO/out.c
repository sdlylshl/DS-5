
#include "in.h"
#include "out.h"
#include ".\System\System_config.h"
/*
 * 函数名：LOCK_GPIO_Config
 * 描述  ：配置LOCK用到的I/O口
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

void LOCK_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体		

	RCC_APB2PeriphClockCmd(LOCK_RCC_PORT, ENABLE);

	PWR_BackupAccessCmd(ENABLE); //允许修改RTC 和后备寄存器

	RCC_LSEConfig(RCC_LSE_OFF); //关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。

	BKP_TamperPinCmd(DISABLE); //关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用

	PWR_BackupAccessCmd(DISABLE); //修改RTC 和后备寄存器
// BKP_ITConfig(DISABLE);

#if LOCK1_USE
	GPIO_InitStructure.GPIO_Pin = LOCK1_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK1_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK1_PORT, &GPIO_InitStructure);
	//LOCK1(1);
#endif
#if LOCK2_USE
	GPIO_InitStructure.GPIO_Pin = LOCK2_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK2_MODE; //开漏输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK2_PORT, &GPIO_InitStructure);
	//LOCK2(1);
#endif
#if LOCK3_USE	
	GPIO_InitStructure.GPIO_Pin = LOCK3_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK3_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK3_PORT, &GPIO_InitStructure);
	LOCK3(1);
#endif
#if LOCK4_USE
	GPIO_InitStructure.GPIO_Pin = LOCK4_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK4_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK4_PORT, &GPIO_InitStructure);
	//LOCK4(1);
#endif
#if LOCK5_USE
	GPIO_InitStructure.GPIO_Pin = LOCK5_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK5_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK5_PORT, &GPIO_InitStructure);
	//LOCK5(1);
#endif
#if LOCK6_USE
	GPIO_InitStructure.GPIO_Pin = LOCK6_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK6_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK6_PORT, &GPIO_InitStructure);
	//LOCK6(1);	
#endif
#if LOCK7_USE	
	GPIO_InitStructure.GPIO_Pin = LOCK7_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK7_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK7_PORT, &GPIO_InitStructure);
	//LOCK7(1);
#endif
#if LOCK8_USE
	GPIO_InitStructure.GPIO_Pin = LOCK8_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK8_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK8_PORT, &GPIO_InitStructure);
	//LOCK8(1);	
#endif
#if LOCK9_USE
	GPIO_InitStructure.GPIO_Pin = LOCK9_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK9_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK9_PORT, &GPIO_InitStructure);
	//LOCK9(1);
#endif
#if LOCK10_USE
	GPIO_InitStructure.GPIO_Pin = LOCK10_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK10_MODE; //开漏输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK10_PORT, &GPIO_InitStructure);
	//LOCK10(1);
#endif
#if LOCK11_USE	
	GPIO_InitStructure.GPIO_Pin = LOCK11_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK11_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK11_PORT, &GPIO_InitStructure);
	//LOCK11(1);
#endif
#if LOCK12_USE
	GPIO_InitStructure.GPIO_Pin = LOCK12_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK12_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK12_PORT, &GPIO_InitStructure);
	//LOCK4(1);
#endif
#if LOCK13_USE
	GPIO_InitStructure.GPIO_Pin = LOCK13_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK13_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK13_PORT, &GPIO_InitStructure);
	//LOCK5(1);
#endif
#if LOCK14_USE
	GPIO_InitStructure.GPIO_Pin = LOCK14_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK14_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK14_PORT, &GPIO_InitStructure);
	//LOCK14(1);	
#endif
#if LOCK15_USE	
	GPIO_InitStructure.GPIO_Pin = LOCK15_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK15_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK15_PORT, &GPIO_InitStructure);
	//LOCK15(1);
#endif
#if LOCK16_USE
	GPIO_InitStructure.GPIO_Pin = LOCK16_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK16_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK16_PORT, &GPIO_InitStructure);
	//LOCK16(1);	
#endif
#if LOCK17_USE
	GPIO_InitStructure.GPIO_Pin = LOCK17_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK17_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK17_PORT, &GPIO_InitStructure);
	//LOCK17(1);
#endif
#if LOCK18_USE
	GPIO_InitStructure.GPIO_Pin = LOCK18_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK18_MODE; //开漏输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK18_PORT, &GPIO_InitStructure);
	//LOCK18(1);
#endif
#if LOCK19_USE	
	GPIO_InitStructure.GPIO_Pin = LOCK19_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK19_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK19_PORT, &GPIO_InitStructure);
	//LOCK19(1);
#endif
#if LOCK20_USE
	GPIO_InitStructure.GPIO_Pin = LOCK20_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK20_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK20_PORT, &GPIO_InitStructure);
	//LOCK20(1);
#endif
#if LOCK21_USE
	GPIO_InitStructure.GPIO_Pin = LOCK21_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK21_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK21_PORT, &GPIO_InitStructure);
	//LOCK21(1);
#endif
#if LOCK22_USE
	GPIO_InitStructure.GPIO_Pin = LOCK22_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK22_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK22_PORT, &GPIO_InitStructure);
	//LOCK22(1);	
#endif
#if LOCK23_USE	
	GPIO_InitStructure.GPIO_Pin = LOCK23_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK23_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK23_PORT, &GPIO_InitStructure);
	//LOCK23(1);
#endif
#if LOCK24_USE
	GPIO_InitStructure.GPIO_Pin = LOCK24_PIN;
	GPIO_InitStructure.GPIO_Mode = LOCK24_MODE; //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK24_PORT, &GPIO_InitStructure);
	//LOCK24(1);	
#endif
}
void LOCKn_ONOFF(uint8_t n, uint8_t onoff) {
	switch (n) {
	case 1:
		LOCK1(onoff);
		break;
	case 2:
		LOCK2(onoff);
		break;
	case 3:
		LOCK3(onoff);
		break;
	case 4:
		LOCK4(onoff);
		break;
	case 5:
		LOCK5(onoff);
		break;
	case 6:
		LOCK6(onoff);
		break;
	case 7:
		LOCK7(onoff);
		break;
	case 8:
		LOCK8(onoff);
		break;
	case 9:
		LOCK9(onoff);
		break;
	case 10:
		LOCK10(onoff);
		break;
	case 11:
		LOCK11(onoff);
		break;
	case 12:
		LOCK12(onoff);
		break;
	case 13:
		LOCK13(onoff);
		break;
	case 14:
		LOCK14(onoff);
		break;
	case 15:
		LOCK15(onoff);
		break;
	case 16:
		LOCK16(onoff);
		break;
	case 17:
		LOCK17(onoff);
		break;
	case 18:
		LOCK18(onoff);
		break;
	case 19:
		LOCK19(onoff);
		break;
	case 20:
		LOCK20(onoff);
		break;
	case 21:
		LOCK21(onoff);
		break;
	case 22:
		LOCK22(onoff);
		break;
	case 23:
		LOCK23(onoff);
		break;
	case 24:
		LOCK24(onoff);
		break;
	default:
		break;
	}

}

//1.开启时间ms
uint32_t LOCK_OPEN_TIME = 50;
uint32_t lock_base_open;
//2.开关间隔ms
uint32_t LOCK_INTERVALS_TIME = 500;
uint32_t lock_base_intervals;
//uint32_t GetCurrentTime() {
//	return TIM4_GetCurrentTime();
//}
//uint32_t GetDistanceTime(uint32_t prev) {
//	return TIM4_GetDistanceTime(prev);
//}

int8_t Locker_open(uint8_t ch) {
	//1.等待间隔是否到达
	//1.开锁
	//2.检测锁状态
	//3.关锁
	while (GetDistanceTime(lock_base_intervals) < LOCK_INTERVALS_TIME)
		;
	lock_base_open = GetCurrentTime();
	LOCKn_ONOFF(ch, 1);
	Delay_ms(10);
	//采集状态 上报
	GetSensorStatus(1);
	while (GetDistanceTime(lock_base_open) < LOCK_OPEN_TIME)
		;
	LOCKn_ONOFF(ch, 0);
	lock_base_intervals = GetCurrentTime();
	//返回状态
	return (-1);

}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
