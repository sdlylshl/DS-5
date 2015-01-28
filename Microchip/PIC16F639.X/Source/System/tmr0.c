

#include "tmr0.h"
#include <stdint.h>

volatile uint8_t timer0ReloadVal8bit;

volatile uint32_t time0;


void TMR0_Initialize(void)
{
    OPTION_REGbits.T0CS = 0;//FOSC/4
    OPTION_REGbits.T0SE =0; //LOW->HIEGH UPCASE
    OPTION_REGbits.PSA =0;
    OPTION_REGbits.PS =0;   //1:2
   // TMR0ON enabled; T0SE Increment_hi_lo; PSA assigned; T0CS FOSC/4; T08BIT 8-bit; T0PS 1:2;

    // Set TMR0 to the options selected in the User Interface

    // TMR0L 131; 
    TMR0 = 0x9B;

    // Load TMR0 value to the 8-bit reload variable
    timer0ReloadVal8bit  = 155;

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.T0IF = 0;
    // Enabling TMR0 interrupt.
    INTCONbits.T0IE = 1;

}



uint8_t TMR0_Read8bitTimer(void)
{
    uint8_t readVal;

    // read Timer0, low register only
    readVal = TMR0;

    return readVal;
}

void TMR0_Write8bitTimer(uint8_t timerVal)
{
    // Write to the Timer0 registers, low register only
    TMR0 = timerVal;
 }

void TMR0_Reload8bit(void)
{
    //Write to the Timer0 register
    TMR0 = timer0ReloadVal8bit;
}

uint32_t TMR0_GetDistanceTime(uint32_t pretime) {
	return (UINT32_MAX - pretime + time0 + 1);
}

uint32_t TMR0_GetCurrentTime(void) {
	return (time0);
}

void TMR0_ISR(void)
{

    // clear the TMR0 interrupt flag
    INTCONbits.T0IF = 0;

    // reload TMR0
    TMR0+ = timer0ReloadVal8bit;
    time0 +=1;

    // add your TMR0 interrupt custom code
}

/**
  End of File
*/