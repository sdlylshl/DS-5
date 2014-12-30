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
void Get_Rtc(struct tm *tim) //������ǰ��ʱ�亯��
{
    rRTCCON = 0x01; //RTC��дʹ�ܣ�ѡ��BCDʱ�ӡ����������޸�λ
//month = rBCDMON; //��ȡ�꣬�£��գ�ʱ���֣�������Ĵ��������ݡ�
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

    rRTCCON = 0x0; //RTC��д��ֹ��ѡ��BCDʱ�ӡ����������޸�λ
    Uart_Printf("RTC\n");
    Uart_Printf("%d�� ",tim->tm_year);
    Uart_Printf("%d�� ",tim->tm_mon);
    Uart_Printf("%d�� ",tim->tm_mday);
    Uart_Printf("����%d ",tim->tm_wday);
    Uart_Printf("%d:",tim->tm_hour);
    Uart_Printf("%d:",tim->tm_min);
    Uart_Printf("%d\n",tim->tm_sec);

//Uart_Printf("%d��%d��%d��%dʱ",time.tm_year,time.tm_mon,time.tm_mday,time.tm_hour);
}
//******************************************************************************
// �����жϣ�����ʵʱʱ��RTC�����ӵĽ���
//***************************************************************************
void __irq TICKINT_Handler(void) //RTC�жϷ�����
{
//Get_Rtc(); //������ǰ��ʱ�亯��
    Lcd_ClearScr(0);
// Lcd_Clear1616(0,134,78);
//Lcd_ClearScr(0);
//Lcd_ClearScr(0);
    ClearPending(BIT_TICK); //���TICK�жϺ��������ֱ���rSRCPND��
//rGPBDAT &=~(1<<0);

}
/*
void __irq IsrAlarm(void) //�����жϣ�����ʵʱ�����жϺ���
{
ClearPending(BIT_RTC); //SRCPND,INTPND�ֱ���1.
Uart_Printf("mini2440 RTCALM oucer \n");

rGPBDAT |=(1<<0); //��������

CloseAlarm(); //�����ӣ������Ҫ�����ӵ���Ҫ�������ˣ��ٿ���
}*/
//******************************************************************************

//����RTC ALARM ʱ��
/*
void setRTCalm(U8 almmon,U8 almdate)//�ꡢ�¡��ա�ʱ���֡���
{
rRTCCON=0x01; //RTCCONʵʱʱ�ӿ��ƼĴ���
//rALMYEAR = almyear;
rALMMON = almmon;
rALMDATE = almdate;
//rALMHOUR = almhour;
//rALMMIN = almmin;
// rALMSEC = almsec;
rRTCCON = 0; //��ȡ��������ֹʹ���źţ��Է��������
}*/

//��ʱ�Ӷ�ʱʱ��
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
//ʱ�Ӻ����ӵĿ��ز�������:
//OpenRtc(); //RTC������ CloseRtc(); //RTC�غ���
//OpenAlarm():�����ӹ��ܺ��� CloseAlarm():�����ӹ��ܺ���
//****************************************************************************

void OpenRtc(void)
{
    pISR_TICK=(unsigned)TICKINT_Handler; //ִ��RTC�жϷ�����
    rTICNT=0xFF; //TICNT�üĴ��������Ƿ�����ʱ��δ��жϺ�ʱ���жϵļ���ֵ��
//TICNT[7]Ϊ0����ֹ��Ϊ1��ʹ�ܡ�
//TICNT[6:0]ʱ��δ����ֵ��1-127���Զ��ݼ������ܶ�ȡ��ʵʱ��ֵ��
    EnableIrq(BIT_TICK); //��RTC�жϷ���
}

//RTC��
void CloseRtc(void)
{
    rTICNT &= ~(1<<7); //��ֹTICNT
    DisableIrq(BIT_TICK); //��RTC�жϷ���

}
/*
//�����ӹ��ܺ���
void OpenAlarm()
{
pISR_RTC = (unsigned)IsrAlarm; //�жϼĴ���ISR�е�RTC�ж�λ
ClearPending(BIT_RTC);
rRTCALM = (0x7f); //RTCALM���ӿ��ƼĴ�����
//��0-7���ֱ��Ӧ�뵽������� ʹ�ܣ�
//��Ӧλ0��ʾ��ֹ��1��ʾ����
EnableIrq(BIT_RTC); //���ж�

}*/
/*
//�����ӹ��ܺ���
void CloseAlarm(void)
{
rRTCALM = 0; //RTCALM���ӿ��ƼĴ�������λ��ֹ
DisableIrq(BIT_RTC); //���ж�
}*/
//****************************************************************************


U32 Gettime(void)
{
    U32 val;
    U8 buf[20];
    char i;
    Uart_Printf("Press Time sync!\n");
    Uart_GetKey(); //�崮�ڻ���
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
            Uart_Printf("%d�� ",tim1.tm_year);
            Uart_Printf("%d�� ",tim1.tm_mon);
            Uart_Printf("%d�� ",tim1.tm_mday);
            Uart_Printf("����%d ",tim1.tm_wday);
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