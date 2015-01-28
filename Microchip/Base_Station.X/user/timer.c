
#include  "main.h"





void Timer0_Init(void)  //¸ø¶¨Ê±Æ÷5Mhz
{
    T0CONbits.T08BIT = 1;  //1 8bit,0 16bit
    T0CONbits.T0CS   = 0;  // internal(clko)
    T0CONbits.T0SE   = 0;
    T0CONbits.PSA    = 0;
    T0CONbits.T0PS   = 0b100;  //8fen

    T0CONbits.TMR0ON = 1;  // ON



    RCONbits.IPEN = 1;  //Enable priority levels on interrupts
    INTCONbits.GIE = 1;  //When IPEN = 1,1 = Enables all high priority interrupts
    INTCONbits.PEIE = 1;  //When IPEN = 1,1 = Enables all low priority peripheral interrupts
    INTCONbits.TMR0IE = 1;  //Enables the TMR0 overflow interrupt
    INTCON2bits.TMR0IP = 1;  //1 = High priority



}

