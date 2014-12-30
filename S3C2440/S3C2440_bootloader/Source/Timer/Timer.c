/******************************************************************************/
// 
// File Name   : Timer.c
// Description : S3C2440 Timer and Watch-dog Function Test 
// Version     : 0.2 
// Date    	   : July 3, 2003
//   0.0 : Programming start (February 27,2002 in 2410) -> SOP
//   0.1 : Modified by Y. H. Lee
//   0.2 : edited by junon
//   
/*******************************************************************************/

/* Include Files */
#include <string.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"

#include "timer.h"
U32 T0_CNT,T1_CNT,T2_CNT,T3_CNT,T4_CNT;

static U32 autorun_10ms;
static U16 autorun_ds;
//全局变量 时间到标志
volatile U16 autorun_trig;

extern void Lcd_Update(void);

static __irq void Timer0_ISR(void)
{
	ClearPending(BIT_TIMER0);
	T0_CNT++;
	
	if(T0_CNT>1500){
		T0_CNT=0;
		Lcd_Update();
		}
/*
	if(autorun_ds)
		DisableIrq(BIT_TIMER0);
	
	autorun_10ms--;
	if(!autorun_10ms) {
		DisableIrq(BIT_TIMER0);
		//CLR_IF();	//in irq service routine, irq is disabled
		autorun_trig = 1;
		//NandLoadRun();
	}
*/
}
extern U32 SendNum;
extern U8	data_write[8];
extern void Can_Write(U32 id, U8 *pdata, unsigned char dlc, int IsExt, int rxRTR);
static __irq void Timer1_ISR(void)
{
	ClearPending(BIT_TIMER1);
	T1_CNT++;
  		SendNum++;
        data_write[4] = SendNum & 0xFF;
        data_write[5] = (SendNum & 0xFF00) >> 8;
        data_write[6] = (SendNum & 0xFF0000) >> 16;
        data_write[7] = (SendNum & 0xFF000000) >> 24;
Can_Write( 0x290, data_write, 8, false, false);//发送报文  标准模式下必须有接收端才能发送成功
if (SendNum>0xFFFFF0)
{
	DisableIrq(BIT_TIMER1);
}
/*
	if(autorun_ds)
		DisableIrq(BIT_TIMER1);
	
	autorun_10ms--;
	if(!autorun_10ms) {
		DisableIrq(BIT_TIMER1);
		//CLR_IF();	//in irq service routine, irq is disabled
		autorun_trig = 1;
		//NandLoadRun();
	}
*/
}

static __irq void Timer2_ISR(void)
{
	ClearPending(BIT_TIMER2);
	T2_CNT++;
/*
	if(autorun_ds)
		DisableIrq(BIT_TIMER2);
	
	autorun_10ms--;
	if(!autorun_10ms) {
		DisableIrq(BIT_TIMER2);
		//CLR_IF();	//in irq service routine, irq is disabled
		autorun_trig = 1;
		//NandLoadRun();
	}
	*/
}

static __irq void Timer3_ISR(void)
{
	ClearPending(BIT_TIMER3);
	T3_CNT++;
/*
	if(autorun_ds)
		DisableIrq(BIT_TIMER3);
	
	autorun_10ms--;
	if(!autorun_10ms) {
		DisableIrq(BIT_TIMER3);
		//CLR_IF();	//in irq service routine, irq is disabled
		autorun_trig = 1;
		//NandLoadRun();
	}
	*/
}

static __irq void Timer4_ISR(void)
{
	ClearPending(BIT_TIMER4);
	
	T4_CNT++;

	if(autorun_ds)
		DisableIrq(BIT_TIMER4);
	
	autorun_10ms--;
	if(!autorun_10ms) {
		DisableIrq(BIT_TIMER4);
		//CLR_IF();	//in irq service routine, irq is disabled
		autorun_trig = 1;
		//NandLoadRun();
	}

}

//****************************************************************88
void Init_Timer0(void)
{
	//1ms
	

	U32 val = (PCLK>>4)/1000-1;
	T0_CNT=0;
	//autorun_10ms = sec*100;
	
	pISR_TIMER0 = (U32)Timer0_ISR;//Timer0_ISR;
	
	rTCFG0 &= ~(0xff);
	rTCFG0 |= 3;			//prescaler = 3+1
	rTCFG1 &= ~(0xf);
	rTCFG1 |= 1;		//mux = 1/4

	rTCNTB0 = val;
	rTCON &= ~(0xf);
	rTCON |= 0xB;			//interval, inv-off, update TCNTB4&TCMPB4, start timer 4
	rTCON &= ~(2);		//clear manual update bit
	EnableIrq(BIT_TIMER0);
}
void Init_Timer1(void)

{
	//10ms
	U32 val = (PCLK>>4)/100-1;
	
	//100ns
	//U32 val = (((PCLK>>4)/100000))-1;

	//autorun_10ms = sec*100;
	pISR_TIMER1 = (U32)Timer1_ISR;
	
	rTCFG0 &= ~(0xff);
	rTCFG0 |= 3;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<4);
	rTCFG1 |= 1<<4;		//mux = 1/4

	rTCNTB1 = val;
	rTCON &= ~(0xf<<8);
	rTCON |= 0xB<<8;			//interval, inv-off, update TCNTB4&TCMPB4, start timer 4
	rTCON &= ~(2<<8);		//clear manual update bit
	EnableIrq(BIT_TIMER1);
}
void Init_Timer2(void)
{
	//100ms
	U32 val = (PCLK>>4)/10-1;
	
	//autorun_10ms = sec*100;
	
	pISR_TIMER2 = (U32)Timer2_ISR;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<8);
	rTCFG1 |= 1<<8;		//mux = 1/4

	rTCNTB2 = val;
	rTCON &= ~(0xf<<12);
	rTCON |= 0xB<<12;			//interval, inv-off, update TCNTB4&TCMPB4, start timer 4
	rTCON &= ~(2<<12);		//clear manual update bit
	EnableIrq(BIT_TIMER2);
}
void Init_Timer3(void)
{
	//1s
	U32 val = (PCLK>>4)/-1;
	
	//autorun_10ms = sec*100;
	
	pISR_TIMER3 = (U32)Timer3_ISR;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 1<<12;		//mux = 1/4

	rTCNTB3 = val;
	rTCON &= ~(0xf<<16);
	rTCON |= 0xf<<16;			//interval, inv-off, update TCNTB4&TCMPB4, start timer 4
	rTCON &= ~(2<<16);		//clear manual update bit
	EnableIrq(BIT_TIMER3);
}

void Init_Timer4(U32 sec)
{
	U32 val = (PCLK>>4)/100-1;
	
	autorun_10ms = sec*10;
	autorun_trig = 0;
	pISR_TIMER4 = (U32)Timer4_ISR;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<16);
	rTCFG1 |= 1<<16;		//mux = 1/4

	rTCNTB3 = val;
	rTCON &= ~(0xf<<20);
	rTCON |= 0x7<<20;			//interval, inv-off, update TCNTB4&TCMPB4, start timer 4
	rTCON &= ~(2<<20);		//clear manual update bit
	EnableIrq(BIT_TIMER4);
}


/************************* Watchdog Timer ********************************/
static U32 intCount;

static void __irq IsrWatchdog(void)
{
	 rINTSUBMSK|=BIT_SUB_WDT;
	 rSRCPND = BIT_WDT_AC97;	//Clear pending bit
	 rINTPND = BIT_WDT_AC97;
//	 rINTPND;	//Prevent an double interrupt pending
	 rSUBSRCPND=BIT_SUB_WDT;
	 intCount++;   	
	 rINTSUBMSK&=~BIT_SUB_WDT;
}

void Timer_InitEx(void)
{
	intCount=0;	
	pISR_WDT_AC97=(U32)IsrWatchdog;
	ClearPending(BIT_WDT_AC97);
	rSUBSRCPND=BIT_SUB_WDT;
	rINTMSK&=~(BIT_WDT_AC97);
	rINTSUBMSK&=~(BIT_SUB_WDT);
}


void Timer_StartEx(void)
{
	//int divider=0;
	rWTCON=((PCLK/1000000-1)<<8)|(0<<3)|(1<<2);	// 16us
	rWTDAT=0xffff;
	rWTCNT=0xffff;   

	// 1/16/(65+1),interrupt enable,reset disable,watchdog enable
	rWTCON=((PCLK/1000000-1)<<8)|(0<<3)|(1<<2)|(0<<0)|(1<<5);   
}


//U32 最大可表示的时间为1.14小时 
U32 Timer_StopEx(void)
{
	
	U32 count;
	rWTCON=((PCLK/1000000-1)<<8);
	rINTMSK|=BIT_WDT_AC97;
	rINTSUBMSK|=BIT_SUB_WDT;
	count=(0xffff-rWTCNT)+(intCount*0xffff);
	return (count*16);//1us
}

