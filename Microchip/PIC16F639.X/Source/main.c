/* 
 * File:   main.c
 * Author: star
 *
 * Created on 2015年1月26日, 上午9:29
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "System/mcc.h"
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//19 RA0(ICSPDAT)   --- CE
//18 RA1(ICSPCLK)  ---- CS
//17 RA2(INT)      ---- IRQ
//4 RA3(MCLR)      ---- MISO
//3 RA4(OSC_CLKOUT) --- CLK
//2 RA5(OSC_CLKIN)  ---MOSI

extern void nrf_main0();
int main(int argc, char** argv) {
    uint32_t time;

    SYSTEM_Initialize();
 nrf_main0();
    
    while(1){

        if(TMR0_GetDistanceTime(time)>5000){
            time = TMR0_GetCurrentTime();
            PORTCbits.RC0 =~PORTCbits.RC0;
            PORTCbits.RC4 = !PORTCbits.RC4;
        }

    }

    //PORTC =0x0;
   // SPI_INIT();
    //nrf_device();
    return (EXIT_SUCCESS);
}

