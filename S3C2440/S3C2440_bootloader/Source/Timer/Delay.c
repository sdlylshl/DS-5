/**************************************************************
 NAME: profile.c
 DESC: To measuure the USB download speed, the WDT is used.
       To measure up to large time, The WDT interrupt is used.
 HISTORY:
 MAR.25.2002:purnnamu:  
     S3C2400X profile.c is ported for S3C2410X.
 **************************************************************/
#include "def.h"
#include "2440addr.h"

extern void DelayMS(U32 ms)
{
    U32 val = (PCLK >> 3) / 100 - 1;

    rTCFG0 &= ~(0xff << 8);
    rTCFG0 |= 3 << 8;       //prescaler = 3+1
    rTCFG1 &= ~(0xf << 12);
    rTCFG1 |= 0 << 12;      //mux = 1/2

    /*  while(ms--) {
            rTCNTB3 = val;
            rTCMPB3 = val>>1;       // 50%
            rTCON &= ~(0xf<<16);
            rTCON |= 3<<16;         //one shot, inv-off, update TCNTB3&TCMPB3, start timer 3
            rTCON &= ~(2<<16);      //clear manual update bit
            while(rTCNTO3);
        }*/
    rTCNTB3 = val;
    rTCMPB3 = val >> 1;     // 50%
    rTCON &= ~(0xf << 16);
    rTCON |= 0xb << 16;     //interval, inv-off, update TCNTB3&TCMPB3, start timer 3
    rTCON &= ~(2 << 16);    //clear manual update bit
    while (ms--)
    {
        while (rTCNTO3 >= val >> 1);
        while (rTCNTO3<val >> 1);
    };

}

//***************************[ SYSTEM ]***************************************************
static int delayLoopCount = 400;
//采用看门狗作为计时器
void Delay(int time)
{
      // time=0: adjust the Delay function by WatchDog timer.
      // time>0: the number of loop time
      // resolution of time is 100us.
    int i,adjust=0;
    if(time==0)
    {
        time   = 200;
        adjust = 1;
        delayLoopCount = 400;
            //PCLK/1M,Watch-dog disable,1/64,interrupt disable,reset disable
        rWTCON = ((PCLK/1000000-1)<<8)|(2<<3); 
        rWTDAT = 0xffff;                              //for first update
        rWTCNT = 0xffff;                              //resolution=64us @any PCLK 
        rWTCON = ((PCLK/1000000-1)<<8)|(2<<3)|(1<<5); //Watch-dog timer start
    }
    for(;time>0;time--)
        for(i=0;i<delayLoopCount;i++);
    if(adjust==1)
    {
        rWTCON = ((PCLK/1000000-1)<<8)|(2<<3); //Watch-dog timer stop
        i = 0xffff - rWTCNT;                     //1count->64us, 200*400 cycle runtime = 64*i us
        delayLoopCount = 8000000/(i*64);         //200*400:64*i=1*x:100 -> x=80000*100/(64*i)   
    }
}
/*
定时器输出时钟频率=PCLK/ (prescaler+1)/ divider
T=n*t=n*(1/f)
=val/(PCLK/(prescaler+1)/ divider)

其中prescaler值由TCFG0决定，divider值由TCFG1决定，而prescaler只能取0~255之间的整数，divider只能取2、4、8和16。
比如已知PCLK为50MHz，而我们想得到某一定时器的输出时钟频率为25kHz，则依据公式可以使prescaler等于249，divider等于8。
有了这个输出时钟频率，理论上我们通过设置寄存器TCNTBn就可以得到任意与0.04毫秒（1÷25000×1000）成整数倍关系的时间间隔了。
例如我们想要得到1秒钟的延时，则使TCNTBn为25000（1000÷0.04）即可。


//采用定时器0作为计时器
void DelayT3_ms(int time)
{
	//值越小时间越长 1000 1ms
	U32 TICKPERSEC= 1000;
	U32 val = (PCLK>>3)/TICKPERSEC-1;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 0<<12;		//divider  = 1/2

	rTCNTB3 = val; 			//自动重载值
	rTCMPB3 = val>>1;		// 50%
	
	rTCON &= ~(0xf<<16);
	rTCON |= 0xb<<16;		//interval, inv-off, update TCNTB3&TCMPB3, start timer 3
	rTCON &= ~(2<<16);		//clear manual update bit
	while(time--) {
		while(rTCNTO3>=val>>1);//读取当前TCNT寄存器的值 当为0时自动从rTCNTB3中重载
		while(rTCNTO3<val>>1);//两个while为一个脉冲周期
	};
}*/