#include "def.h"
#include "2440lib.h"
#include "2440addr.h"
#include "bcd.h"
#include "time.h"

#include "LCD_Driver.h"
#include "Keyboard.h"

//int year, month,date,weekday,hour,min,sec;
extern int pwcount = 0;
void CloseAlarm(void);
extern void Delay(int time);
//********************************************************************
void Get_Rtc(struct tm *tim) //读出当前的时间函数
{
    rRTCCON = 0x01; //RTC读写使能，选择BCD时钟、计数器，无复位
//month = rBCDMON; //读取年，月，日，时，分，秒各个寄存器的数据。
//date = rBCDDATE;
//weekday = rBCDDAY;
//hour = rBCDHOUR;
//min = rBCDMIN;
//sec = rBCDSEC;

    tim->tm_sec=bcd2bin(rBCDSEC);                   /* Seconds.     [0-60] (1 leap second) */
    tim->tm_min=bcd2bin(rBCDMIN);                   /* Minutes.     [0-59] */
    tim->tm_hour=bcd2bin(rBCDHOUR);                  /* Hours.       [0-23] */
    tim->tm_mday=bcd2bin(rBCDDATE);                  /* Day.         [1-31] */
    tim->tm_mon=bcd2bin(rBCDMON);                   /* Month.       [0-11] */
    tim->tm_year=bcd2bin(rBCDYEAR);                  /* Year - 1900.  */
    tim->tm_wday=bcd2bin(rBCDDAY);                  /* Day of week. [0-6] */
//time.tm_yday=DaysInYear(time) ;                  /* Days in year.[0-365] */
//time.tm_isdst;                 /* DST.         [-1/0/1]*/

    rRTCCON = 0x0; //RTC读写禁止，选择BCD时钟、计数器，无复位
    Uart_Printf("RTC\n");
    Uart_Printf("%d年 ",tim->tm_year);
    Uart_Printf("%d月 ",tim->tm_mon);
    Uart_Printf("%d日 ",tim->tm_mday);
    Uart_Printf("星期%d ",tim->tm_wday);
    Uart_Printf("%d:",tim->tm_hour);
    Uart_Printf("%d:",tim->tm_min);
    Uart_Printf("%d\n",tim->tm_sec);

//Uart_Printf("%d年%d月%d日%d时",time.tm_year,time.tm_mon,time.tm_mday,time.tm_hour);
}
//******************************************************************************
// 利用中断，进行实时时钟RTC和闹钟的进行
//***************************************************************************
void __irq TICKINT_Handler(void) //RTC中断服务函数
{
//Get_Rtc(); //读出当前的时间函数
    Lcd_ClearScr(0);
// Lcd_Clear1616(0,134,78);
//Lcd_ClearScr(0);
//Lcd_ClearScr(0);
    ClearPending(BIT_TICK); //清除TICK中断函数。即分别往rSRCPND，
//rGPBDAT &=~(1<<0);

}
/*
void __irq IsrAlarm(void) //利用中断，闹钟实时进行中断函数
{
ClearPending(BIT_RTC); //SRCPND,INTPND分别置1.
Uart_Printf("mini2440 RTCALM oucer \n");

rGPBDAT |=(1<<0); //蜂鸣器响

CloseAlarm(); //关闹钟，即如果要开闹钟的先要把它关了，再开。
}*/
//******************************************************************************

//设置RTC ALARM 时间
/*
void setRTCalm(U8 almmon,U8 almdate)//年、月、日、时、分、秒
{
rRTCCON=0x01; //RTCCON实时时钟控制寄存器
//rALMYEAR = almyear;
rALMMON = almmon;
rALMDATE = almdate;
//rALMHOUR = almhour;
//rALMMIN = almmin;
// rALMSEC = almsec;
rRTCCON = 0; //读取数据完后禁止使能信号，以防误操作，
}*/

//置时钟定时时间
/*void setRTCtime(U8 wRTCmon,U8 wRTCdate,U8 wRTCday,U8 wRTChour ,U8 wRTCmin ,U8 wRTCsec)
{
rRTCCON=0x01;
// rBCDYEAR = wRTCyear;
rBCDMON = wRTCmon;
rBCDDATE = wRTCdate;
rBCDDAY = wRTCday;
rBCDHOUR = wRTChour;
rBCDMIN = wRTCmin;
rBCDSEC = wRTCsec;
rRTCCON = 0x00;
} */
void setRTCtime(struct tm *tim)
{
    rRTCCON=0x01;
    rBCDSEC = bin2bcd((tim->tm_sec));
    rBCDMIN = bin2bcd((tim->tm_min));
    rBCDHOUR = bin2bcd((tim->tm_hour));
    rBCDDATE = bin2bcd((tim->tm_mday));
    rBCDMON =  bin2bcd((tim->tm_mon));
    rBCDYEAR = bin2bcd((tim->tm_year));
    rBCDDAY = bin2bcd((tim->tm_wday));
    rRTCCON = 0x00;

}

//****************************************************************************
//时钟和闹钟的开关操作函数:
//OpenRtc(); //RTC开函数 CloseRtc(); //RTC关函数
//OpenAlarm():开闹钟功能函数 CloseAlarm():关闹钟功能函数
//****************************************************************************

void OpenRtc(void)
{
    pISR_TICK=(unsigned)TICKINT_Handler; //执行RTC中断服务函数
    rTICNT=0xFF; //TICNT该寄存器用于是否允许时间滴答中断和时间中断的计数值。
//TICNT[7]为0，禁止，为1，使能。
//TICNT[6:0]时间滴答计数值，1-127，自动递减，不能读取其实时数值。
    EnableIrq(BIT_TICK); //开RTC中断服务
}

//RTC关
void CloseRtc(void)
{
    rTICNT &= ~(1<<7); //禁止TICNT
    DisableIrq(BIT_TICK); //关RTC中断服务

}
/*
//开闹钟功能函数
void OpenAlarm()
{
pISR_RTC = (unsigned)IsrAlarm; //中断寄存器ISR中的RTC中断位
ClearPending(BIT_RTC);
rRTCALM = (0x7f); //RTCALM闹钟控制寄存器，
//【0-7】分别对应秒到年的闹钟 使能，
//相应位0表示禁止，1表示允许
EnableIrq(BIT_RTC); //开中断

}*/
/*
//关闹钟功能函数
void CloseAlarm(void)
{
rRTCALM = 0; //RTCALM闹钟控制寄存器所有位禁止
DisableIrq(BIT_RTC); //关中断
}*/
//****************************************************************************


U32 Gettime(void)
{
    U32 val;
    U8 buf[20];
    char i;
    Uart_Printf("Press Time sync!\n");
    Uart_GetKey(); //清串口缓存
    val =0;
    for(i=0; i<12; i++) {
        buf[i] = Uart_Getch();
    }
    for(i=0; i<10; i++) {

        if((buf[i]==0x5A)&&(buf[i+1]==0xA5))
            break;
    }

    val=(buf[i+4]<<24)|(buf[i+5]<<16)|(buf[i+6]<<8)|(buf[i+7]);
    Uart_Printf("%x ",val);
    for(i=0; i<12; i++)
        Uart_Printf("%.2x ",buf[i]);
    Uart_Printf("\n");
    return val;
}

extern void Settime(void)
{

    time_t val;
    struct tm tim,tim1;
    char str[30];
    
    while(1) {

		Versions();

        Printf("Starting Time sync\n\n");


        //print(0,3,"System Time is :",0xFF);

        Get_Rtc(&tim);
        //tim.tm_mon-=1;
        asctime_r(&tim,str);

        Printf("System Time is :%s\n\n\n",str);

        //print(0,4,str,0xFF);
        //Delay(25000);
        Lcd_SetColor(RED);
        Printf(" ENTER key to Contiue!\n");
        Printf(" BACK  key to Exit!\n");
        Lcd_SetColor(BLUE);

        if(KEY_BACK==KeyScan())
            return;

     //   if(KEY_ENTER==KeyScan()) {
            //print(0,15,"Wating synchronization !",0xFF);
			Printf("\nWating Time synchronization\n");

            do {

				val =Gettime();
                val+=8*3600;
                Uart_Printf("val=%X\n ",val);
            } while(!(val%60));

            localtime_r(&val,&tim1);
            tim1.tm_mon+=1;
            tim1.tm_year-=100;
            setRTCtime(&tim1);

            Uart_Printf("SET time !\n");
            Uart_Printf("%d年 ",tim1.tm_year);
            Uart_Printf("%d月 ",tim1.tm_mon);
            Uart_Printf("%d日 ",tim1.tm_mday);
            Uart_Printf("星期%d ",tim1.tm_wday);
            Uart_Printf("%d:",tim1.tm_hour);
            Uart_Printf("%d:",tim1.tm_min);
            Uart_Printf("%d\n",tim1.tm_sec);
        }
        Delay(50000);
		//Delay(50000);
        //print(0,5," Press any key is Exit !",0xFF);
		//Delay(50);

  //  }
   while(!(KEY_BACK==KeyScan()));

}