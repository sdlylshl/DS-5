

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/T1G/OSC2/CLKOUT pin, I/O function on RA5/T1CKI/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR pin function select bit (MCLR pin function is alternate function, MCLR function is internally disabled)
#pragma config CP = OFF         // Code Protection bit (Program memory is not code protected)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory is not code protected)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOD enabled and SBOdEN bit disabled)
#pragma config IESO = ON        // Internal-External Switchover bit (Internal External Switchover mode enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config WURE = OFF       // Wake-Up Reset Enable bit (Standard wake-up and continue enabled)


#include "mcc.h"

void OSCILLATOR_Initialize(void)
{
    // SCS INTOSC; IDLEN disabled; IRCF 1MHz_HFINTOSC/16;
    //OSCCON = 0x32;
    OSCCONbits.SCS = 1;         //INTER
    OSCCONbits.IOSCF = 0x07;    //8MHz_HFINTOSC
    // PRISD enabled; MFIOSEL disabled; SOSCGO disabled;
   // OSCCON2 = 0x04;
    // INTSRC disabled; PLLEN enabled; TUN 0x00;
   // OSCTUNE = 0x40;
    // Set the secondary oscillator

}
void SYSTEM_Initialize(void)
{
    //FOSC=8M CLKOUT=FOSC/4
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();
    TMR0_Initialize();
    SPI_INIT();
//    INTERRUPT_PeripheralInterruptEnable();
//    INTERRUPT_GlobalInterruptEnable();
}
