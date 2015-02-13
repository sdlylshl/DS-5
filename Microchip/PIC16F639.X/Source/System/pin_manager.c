
#include <xc.h>
#include "pin_manager.h"

void PIN_MANAGER_Initialize(void)
{ 
    //RA3 仅可作为输入引脚
    TRISCbits.TRISC0 =0;
    TRISCbits.TRISC4 =0;

    PORTCbits.RC0 =0;
    PORTCbits.RC4 =0;

//  通用配置
    OPTION_REGbits.nRAPU = 0;  //启用PORTA上下拉使能    
    WDAbits.WDA2 =1;    //选择上拉
    WPUDAbits.WPUDA2 =1; // 1使能上下拉   
    
  //设置输入状态
    TRISAbits.TRISA0 =1;
    TRISAbits.TRISA2 =1;
    TRISAbits.TRISA3 =1;
    TRISAbits.TRISA4 =1;
    TRISAbits.TRISA5 =1;
    
//  RA2外部中断配置
    OPTION_REGbits.INTEDG =0;   //RA2下降沿触发中断    
    INTCONbits.INTE = 1;    //启用RA2为中断
    
//  PORTA 端口变换中断    
//    INTCONbits.RAIE =1;
//    IOCAbits.IOCA0 = 1;
//    IOCAbits.IOCA1 = 1;
//    IOCAbits.IOCA2 = 1;
   // IOCAbits.IOCA3 = 1;
    //IOCAbits.IOCA4 = 1;
    //IOCAbits.IOCA5 = 1;
    

    //关闭比较器 不再使用任何IO引脚
    CMCON0bits.CM = 0x07;    

   //中断开启
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

}

void INT_ISR(void){

    PORTCbits.RC4 = ~PORTCbits.RC4;
}
/**
 End of File
*/