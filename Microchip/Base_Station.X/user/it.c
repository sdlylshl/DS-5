
#include  "main.h"

uint8_t  Div_Num;
uint8_t  Rem_Num;
extern _AFE AFE_STR;

void interrupt ISR_H(void)
{
    //LED = 0;
    if(INTCONbits.TMR0IF == 1)  //定时器0溢出中断,0.25ms进中断一次
    {
        INTCONbits.TMR0IF = 0;
        TMR0L = 218;   //定时器初始值 178
        TEST_PIN = ~TEST_PIN;

        if( AFE_STR.AFE_Send_Flag == 1 )
        {
            if(AFE_STR.S_Count <= AFE_STR.Count)
            {
                Div_Num = (AFE_STR.S_Count-1) / 8;
                Rem_Num = (AFE_STR.S_Count-1) % 8;
                Rem_Num = 7 - Rem_Num;
                if( AFE_STR.AFE_Buf[Div_Num] & (1<<(Rem_Num)) )
                {
                    PWM_ON;
                }else
                {
                    PWM_OFF;
                }
                AFE_STR.S_Count++;

            }else
            {
                PWM_OFF;
                AFE_STR.AFE_Send_Flag = 0;
            }
        }
    }
}


void interrupt low_priority ISR_L(void)
{
    LED = 0;
    
}







