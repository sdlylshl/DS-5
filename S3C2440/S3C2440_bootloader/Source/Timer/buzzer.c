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
extern void DelayMs(U32 ms);

static void buzzer(int freq, int ms)
{
    rGPBCON &= ~3;          //set GPB0 as tout0, pwm output
    rGPBCON |= 2;

    rTCFG0 &= ~0xff;
    rTCFG0 |= 15;           //prescaler = 15+1
    rTCFG1 &= ~0xf;
    rTCFG1 |= 2;            //mux = 1/8
    rTCNTB0 = (PCLK >> 7) / freq;
    rTCMPB0 = rTCNTB0 >> 1; // 50%
    rTCON &= ~0x1f;
    rTCON |= 0xb;           //disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 0
    rTCON &= ~2;            //clear manual update bit

    DelayMs(ms);


    rGPBCON &= ~3;          //set GPB0 as output
    rGPBCON |= 1;
    rGPBDAT &= ~1;
}