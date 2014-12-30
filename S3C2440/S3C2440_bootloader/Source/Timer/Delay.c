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
//���ÿ��Ź���Ϊ��ʱ��
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
��ʱ�����ʱ��Ƶ��=PCLK/ (prescaler+1)/ divider
T=n*t=n*(1/f)
=val/(PCLK/(prescaler+1)/ divider)

����prescalerֵ��TCFG0������dividerֵ��TCFG1��������prescalerֻ��ȡ0~255֮���������dividerֻ��ȡ2��4��8��16��
������֪PCLKΪ50MHz����������õ�ĳһ��ʱ�������ʱ��Ƶ��Ϊ25kHz�������ݹ�ʽ����ʹprescaler����249��divider����8��
����������ʱ��Ƶ�ʣ�����������ͨ�����üĴ���TCNTBn�Ϳ��Եõ�������0.04���루1��25000��1000������������ϵ��ʱ�����ˡ�
����������Ҫ�õ�1���ӵ���ʱ����ʹTCNTBnΪ25000��1000��0.04�����ɡ�


//���ö�ʱ��0��Ϊ��ʱ��
void DelayT3_ms(int time)
{
	//ֵԽСʱ��Խ�� 1000 1ms
	U32 TICKPERSEC= 1000;
	U32 val = (PCLK>>3)/TICKPERSEC-1;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 0<<12;		//divider  = 1/2

	rTCNTB3 = val; 			//�Զ�����ֵ
	rTCMPB3 = val>>1;		// 50%
	
	rTCON &= ~(0xf<<16);
	rTCON |= 0xb<<16;		//interval, inv-off, update TCNTB3&TCMPB3, start timer 3
	rTCON &= ~(2<<16);		//clear manual update bit
	while(time--) {
		while(rTCNTO3>=val>>1);//��ȡ��ǰTCNT�Ĵ�����ֵ ��Ϊ0ʱ�Զ���rTCNTB3������
		while(rTCNTO3<val>>1);//����whileΪһ����������
	};
}*/