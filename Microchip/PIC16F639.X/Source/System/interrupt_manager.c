

#include "mcc.h"
#include "interrupt_manager.h"

void  INTERRUPT_Initialize (void)
{

}

void interrupt INTERRUPT_InterruptManager (void)
{
   // interrupt handler
     if(INTCONbits.T0IE == 1 && INTCONbits.T0IF == 1)
    {
        // clear the TMR0 interrupt flag
        INTCONbits.T0IF = 0;
        TMR0_ISR();
    }else if(INTCONbits.INTE == 1 && INTCONbits.INTF == 1)
    {
//        RA2/INT 外部中断

        INTCONbits.INTF = 0;
        INT_ISR();
    }else if(INTCONbits.RAIE == 1 && INTCONbits.RAIF == 1)
    {
//        PORTA 电平变化中断
        INTCONbits.RAIF = 0;
        INT_ISR();
    }
}
