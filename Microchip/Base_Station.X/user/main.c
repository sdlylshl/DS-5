/* 
 * File:   main.c
 * Author: zwf
 *
 * Created on 2015年1月10日, 上午11:17
 */

#include "main.h"


// CONFIG1H
#pragma config OSC =  HS//HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF       // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)


 _AFE AFE_STR;


void System_Clk_Init(void)
{
    OSCTUNEbits.INTSRC = 0;
    OSCTUNEbits.PLLEN = 0;
    
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b00;

}

void delay_ms(uint16_t t)
{
    uint16_t i,j;

    for(i=0;i<t;i++)
    {
        for(j=0;j<1500;j++);
    }
}
void bsp_Init(void)
{
    TRISBbits.TRISB5 = 0;//output
    TRISBbits.TRISB3 = 0;//output
    TRISCbits.TRISC2 = 0;//PWM Pin output

    /***********************/

    T2CONbits.T2OUTPS = 0;  //1:1 定时器2分频, Timer2 Output Postscale Select bits
    T2CONbits.TMR2ON = 1;  //打开定时器2
    T2CONbits.T2CKPS = 0;  //Timer2 Clock Prescale Select bits

//    TMR2 = 200;
    /************************/
   
    PR2 = 119;        //PWM period 119
    CCPR1L = PR2/2;   //PWM duty
    CCP1CONbits.DC1B = 0;  //0

    CCP1CONbits.CCP1M = 0B1100;  //PWM mode

    Timer0_Init();
    ei();
}

// AFE唤醒时间 >= 4ms, 间隔脉冲 >= 200us
// 滤波TOEH = 2ms, TOEL = 1ms, tOET = 4
// 1:2个高电平单位，0:6个高电平单位，总8个时间单位4ms
// 1个Bit用8个时间单位表示, 1个时间单位250us
uint8_t Send_AFE_Data(uint8_t *p, uint8_t len)
{
    uint16_t i;
    uint8_t j;
//    uint8_t temp;
    uint16_t k;
    
    AFE_STR.Count = 0;
    AFE_STR.AFE_Buf[0] = 0;
    AFE_STR.AFE_Buf[1] = 0;
    AFE_STR.AFE_Buf[2] = 0;
    AFE_STR.AFE_Buf[3] = 0;
    AFE_STR.AFE_Buf[4] = 0;
    AFE_STR.AFE_Buf[5] = 0;
    AFE_STR.AFE_Buf[6] = 0;

    for(i=0;i<len;i++)
    {
        for(j=0;j<8;j++)
        {
            if( (*(p+i)) & (1<<(7-j)) )  //1
            {
                 AFE_STR.AFE_Buf[i * 8 + j + 7 ] = 0B11000000;
            }else  //0
            {
                 AFE_STR.AFE_Buf[i * 8 + j + 7 ] = 0B11111100;
            }
        }
    }
    for(i=0;i<(len*8);i++)
    {
        j = AFE_STR.AFE_Buf[i+7];
        AFE_STR.AFE_Buf[i+7] <<= 4;
        j >>= 4;
        AFE_STR.AFE_Buf[i+6] |= j;
    }
    AFE_STR.AFE_Buf[0] = 0B00000000;
    AFE_STR.AFE_Buf[1] = 0B11111111;
    AFE_STR.AFE_Buf[2] = 0B11111111;
    AFE_STR.AFE_Buf[3] = 0B11111100;
    AFE_STR.AFE_Buf[4] = 0B11111111;
    AFE_STR.AFE_Buf[5] = 0B10000011;
    AFE_STR.AFE_Buf[6] |= 0B11000000;  //低4位空余
    k=0;
    for(i=0;i<len;i++)  //奇校验
    {
        for(j=0;j<8;j++)
        {
            if( (*(p+i)) & (1<<j) )
            {
                k++;
            }
        }
    }    
    if( (k % 2) == 1)  //奇数
    {
        k = 0;
    }else
    {
        k = 1;
    }
    if(k == 1)
    {
        AFE_STR.AFE_Buf[len * 8 + 6] |= 0x0C;  //奇校验位
        AFE_STR.AFE_Buf[len * 8 + 7] |= 0x00;
    }else
    {
        AFE_STR.AFE_Buf[len * 8 + 6] |= 0x0F;  //奇校验位
        AFE_STR.AFE_Buf[len * 8 + 7] |= 0xC0;
    }
    AFE_STR.AFE_Buf[len * 8 + 7] |= 0x0F;  //停止位5ms
    AFE_STR.AFE_Buf[len * 8 + 8] |= 0xFF;
    AFE_STR.AFE_Buf[len * 8 + 9] |= 0xFF;
    AFE_STR.AFE_Buf[len * 8 + 10] |= 0x00;
    AFE_STR.AFE_Buf[len * 8 + 11] |= 0x00;
    AFE_STR.AFE_Buf[len * 8 + 12] |= 0x00;

    AFE_STR.Count = 56 + len * 8 * 8 + 28 + 20;
    AFE_STR.S_Count = 0;
    AFE_STR.AFE_Send_Flag = 1;

    while(AFE_STR.AFE_Send_Flag);
    
    return 0;  //OK
}

int main(int argc, char** argv) {

    uint8_t temp[16];

    System_Clk_Init();
    bsp_Init();

    temp[0] = 0B01011010;
    temp[1] = 0B01110101;
    temp[2] = 0xA7;

    PWM_ON;
 //   CCPR1L = PR2;

    while(1)
    {
        LED = 1;
        delay_ms(200);
       LED = 0;

//        while( Send_AFE_Data(temp,2) );

        delay_ms(20);

    }


    return (EXIT_SUCCESS);
}




