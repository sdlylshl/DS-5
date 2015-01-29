
#include <xc.h>
#include "pin_manager.h"

void PIN_MANAGER_Initialize(void)
{
    //RA3 仅可作为输入引脚
    
    //使能PORTA上下拉
    OPTION_REGbits.nRAPU = 0;
    //开启上拉
    WPUDAbits.WPUDA2 =1;
    WDAbits.WDA2 =1;
    TRISAbits.TRISA2 =1;
    //上升沿触发中断
    OPTION_REGbits.INTEDG =1;
    //启用RA2为中断
   // INTCONbits.INTE = 1;

  //设置输入状态
    TRISAbits.TRISA0 =1;
    TRISAbits.TRISA2 =1;
    TRISAbits.TRISA3 =1;
   // TRISAbits.TRISA4 =1;
   // TRISAbits.TRISA5 =1;

    //PORTA 端口变换中断
   // IOCAbits.IOCA0 = 1;
   // IOCAbits.IOCA1 = 1;
   // IOCAbits.IOCA2 = 1;
    IOCAbits.IOCA3 = 1;
    //IOCAbits.IOCA4 = 1;
    //IOCAbits.IOCA5 = 1;
    


    INTCONbits.RAIE =1;

    //关闭比较器
    CMCON0bits.CM = 0x07;    
    TRISCbits.TRISC0 =0;
    TRISCbits.TRISC4 =0;
    PORTCbits.RC0 =1;
    PORTCbits.RC4 =1;

    TRISA = 0xFF;
}

void INT_ISR(void){

    PORTCbits.RC4 = !PORTCbits.RC4;
}
/**
 End of File
*/