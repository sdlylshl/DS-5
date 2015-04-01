
/*
#include "Delay.H"
#include <iom64v.h>
//#include <iom128v.h>
//#include<iom32v.h>
#include <macros.h>
*/
#include "Global.h"


#if  (CPU_FREQUENCY == 72000000)

void Delay_Nms(unsigned long N)
{
	long count;//=14200;
	while(N)
	{
		count = 7998;
		/* 逻辑分析仪测试, 包含引脚取反时间
			14200, 80ms: 142.026ms, 
			10000, 80ms: 100.025ms 
			 8000, 80ms: 80.25ms
			 7975, 80ms: 79.775ms
			 7990, 100ms: 99.907ms
			 7998, 150ms: 150.01ms
			 7997, 150ms: 149.992ms
		*/
		while(count--);
		/*
		while(count--);
		0x08001236 000A 	 MOVS	  r2,r1
		0x08001238 F1A10301  SUB	  r3,r1,#0x01
		0x0800123C 4619 	 MOV	  r1,r3
		0x0800123E D1FA 	 BNE	  0x08001236
		*/
		N--;
	}
}

void Delay_Nus(unsigned long N)
{
	long count;//=14;
	while(N)
	{
		count = 6;			
		/* 逻辑分析仪测试, 包含引脚取反时间
			6, 100us: 106.205us 
			6,1000us: 1.068ms
			5,1000us: 945.825us
			6,100000us: 106.944ms
		*/
		while(count--);
		/*
		while(count--);
		0x08001256 000A      MOVS     r2,r1
		0x08001258 F1A10301  SUB      r3,r1,#0x01
		0x0800125C 4619      MOV      r1,r3
		0x0800125E D1FA      BNE      0x08001256
		*/
		/* 喂狗操作 */
		//WdtFeed();	调试占用时间
		N--;
	}
}


#elif (CPU_FREQUENCY == 8000000)
/*-----------------------------------------------------------------------
延时函数
系统时钟：8M
-----------------------------------------------------------------------*/
void Delay_1us(void)                 //1us延时函数
{
	//asm("nop");
	WDR();
}

void Delay_Nus(unsigned int n)       //N us延时函数
{
	unsigned int i=0;
	for (i=0;i<n;i++)
	Delay_1us();
}


/*
void Delay_1ms(void)                 //1ms延时函数
{
	// Call + Delay_1ms + RET : 7996 Cycle;  RCall:7995
	unsigned int i;
	for (i=0;i<1140;i++);
}
void Delay_1ms(void)                 //1ms延时函数
{
	// Call + Delay_1ms + RET : 7991 Cycle
	unsigned int i;
	NOP();
	NOP();
	for (i=0;i<1139;i++);
}
*/
 
// Call: 4;   RCall: 3
// RET: 4
void Delay_1ms(void)                 //1ms延时函数
{
	unsigned int i;
	for (i=0;i<997;i++);
	{
		WDR();
	}
	// Call + Delay_1ms + RET : 7992 Cycle; RCall: 7991
 	NOP();NOP();NOP();
}

// Call + Cycle Body + Ret:  1000
// Call Delay_Nms: 31 Cycle
// Exit Call: 15 Cycle
void Delay_Nms(unsigned int n)       //N ms延时函数
{
	unsigned int i=0;
	// 以下循环体 (n*8000-1) Cycle : 
	for (i=0;i<n;i++)				// int i: 6 Cycle
	Delay_1ms();					// RCall: 7994 Cycle
}

#elif (CPU_FREQUENCY == 11059200)
/*-----------------------------------------------------------------------
延时函数
系统时钟：11.0592M
-----------------------------------------------------------------------*/
void Delay_1us(void)                 //1us延时函数
{
	//asm("nop");
	WDR();
	asm("nop");
}

void Delay_Nus(unsigned int n)       //N us延时函数
{
	unsigned int i=0;
	for (i=0;i<n;i++)
		Delay_1us();
}

// Call: 4;   RCall: 3
// RET: 4
void Delay_1ms(void)                 //1ms延时函数
{
	unsigned int i;					// 2T
	for (i=0;i<1379;i++)
	{
		WDR();						// 单次for: 8T
	}								// 2T
	// Call + Delay_1ms + RET : 7992 Cycle; RCall: 7991
 	NOP();NOP();NOP();
 	NOP();NOP();NOP();
}

// Call + Cycle Body + Ret:  1000
// Call Delay_Nms: 31 Cycle
// Exit Call: 15 Cycle
void Delay_Nms(unsigned int n)       //N ms延时函数
{
	unsigned int i=0;
	// 以下循环体 (n*8000-1) Cycle : 
	for (i=0;i<n;i++)				// int i: 6 Cycle
	Delay_1ms();					// RCall: 7994 Cycle
}


#else
	// CPU频率未定义
	// #error	"Delay.C: Not Except value of 'CPU_FREQENCY'!"
#endif
