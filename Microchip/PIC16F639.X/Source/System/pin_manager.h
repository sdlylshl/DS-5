
#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

//// get/set C12IN0Neg aliases
//#define C12IN0Neg_TRIS               TRISA0
//#define C12IN0Neg_LAT                LATA0
//#define C12IN0Neg_PORT               PORTAbits.RA0
//#define C12IN0Neg_ANS                ANSA0
//#define C12IN0Neg_SetHigh()    do { LATA0 = 1; } while(0)
//#define C12IN0Neg_SetLow()   do { LATA0 = 0; } while(0)
//#define C12IN0Neg_Toggle()   do { LATA0 = ~LATA0; } while(0)
//#define C12IN0Neg_GetValue()         PORTAbits.RA0
//#define C12IN0Neg_SetDigitalInput()    do { TRISA0 = 1; } while(0)
//#define C12IN0Neg_SetDigitalOutput()   do { TRISA0 = 0; } while(0)
//
//#define C12IN0Neg_SetAnalogMode()   do { ANSA0 = 1; } while(0)
//#define C12IN0Neg_SetDigitalMode()   do { ANSA0 = 0; } while(0)
//// get/set C2INPos aliases
//#define C2INPos_TRIS               TRISA2
//#define C2INPos_LAT                LATA2
//#define C2INPos_PORT               PORTAbits.RA2
//#define C2INPos_ANS                ANSA2
//#define C2INPos_SetHigh()    do { LATA2 = 1; } while(0)
//#define C2INPos_SetLow()   do { LATA2 = 0; } while(0)
//#define C2INPos_Toggle()   do { LATA2 = ~LATA2; } while(0)
//#define C2INPos_GetValue()         PORTAbits.RA2
//#define C2INPos_SetDigitalInput()    do { TRISA2 = 1; } while(0)
//#define C2INPos_SetDigitalOutput()   do { TRISA2 = 0; } while(0)
//
//#define C2INPos_SetAnalogMode()   do { ANSA2 = 1; } while(0)
//#define C2INPos_SetDigitalMode()   do { ANSA2 = 0; } while(0)
//// get/set C1INPos aliases
//#define C1INPos_TRIS               TRISA3
//#define C1INPos_LAT                LATA3
//#define C1INPos_PORT               PORTAbits.RA3
//#define C1INPos_ANS                ANSA3
//#define C1INPos_SetHigh()    do { LATA3 = 1; } while(0)
//#define C1INPos_SetLow()   do { LATA3 = 0; } while(0)
//#define C1INPos_Toggle()   do { LATA3 = ~LATA3; } while(0)
//#define C1INPos_GetValue()         PORTAbits.RA3
//#define C1INPos_SetDigitalInput()    do { TRISA3 = 1; } while(0)
//#define C1INPos_SetDigitalOutput()   do { TRISA3 = 0; } while(0)
//
//#define C1INPos_SetAnalogMode()   do { ANSA3 = 1; } while(0)
//#define C1INPos_SetDigitalMode()   do { ANSA3 = 0; } while(0)
//// get/set CCP4 aliases
//#define CCP4_TRIS               TRISB0
//#define CCP4_LAT                LATB0
//#define CCP4_PORT               PORTBbits.RB0
//#define CCP4_WPU                WPUB0
//#define CCP4_ANS                ANSB0
//#define CCP4_SetHigh()    do { LATB0 = 1; } while(0)
//#define CCP4_SetLow()   do { LATB0 = 0; } while(0)
//#define CCP4_Toggle()   do { LATB0 = ~LATB0; } while(0)
//#define CCP4_GetValue()         PORTBbits.RB0
//#define CCP4_SetDigitalInput()    do { TRISB0 = 1; } while(0)
//#define CCP4_SetDigitalOutput()   do { TRISB0 = 0; } while(0)
//
//#define CCP4_SetPullup()    do { WPUB0 = 1; } while(0)
//#define CCP4_ResetPullup()   do { WPUB0 = 0; } while(0)
//#define CCP4_SetAnalogMode()   do { ANSB0 = 1; } while(0)
//#define CCP4_SetDigitalMode()   do { ANSB0 = 0; } while(0)
//// get/set P1A aliases
//#define P1A_TRIS               TRISC2
//#define P1A_LAT                LATC2
//#define P1A_PORT               PORTCbits.RC2
//#define P1A_ANS                ANSC2
//#define P1A_SetHigh()    do { LATC2 = 1; } while(0)
//#define P1A_SetLow()   do { LATC2 = 0; } while(0)
//#define P1A_Toggle()   do { LATC2 = ~LATC2; } while(0)
//#define P1A_GetValue()         PORTCbits.RC2
//#define P1A_SetDigitalInput()    do { TRISC2 = 1; } while(0)
//#define P1A_SetDigitalOutput()   do { TRISC2 = 0; } while(0)
//
//#define P1A_SetAnalogMode()   do { ANSC2 = 1; } while(0)
//#define P1A_SetDigitalMode()   do { ANSC2 = 0; } while(0)
//// get/set SCK1 aliases
//#define SCK1_TRIS               TRISC3
//#define SCK1_LAT                LATC3
//#define SCK1_PORT               PORTCbits.RC3
//#define SCK1_ANS                ANSC3
//#define SCK1_SetHigh()    do { LATC3 = 1; } while(0)
//#define SCK1_SetLow()   do { LATC3 = 0; } while(0)
//#define SCK1_Toggle()   do { LATC3 = ~LATC3; } while(0)
//#define SCK1_GetValue()         PORTCbits.RC3
//#define SCK1_SetDigitalInput()    do { TRISC3 = 1; } while(0)
//#define SCK1_SetDigitalOutput()   do { TRISC3 = 0; } while(0)
//
//#define SCK1_SetAnalogMode()   do { ANSC3 = 1; } while(0)
//#define SCK1_SetDigitalMode()   do { ANSC3 = 0; } while(0)
//// get/set SDI1 aliases
//#define SDI1_TRIS               TRISC4
//#define SDI1_LAT                LATC4
//#define SDI1_PORT               PORTCbits.RC4
//#define SDI1_ANS                ANSC4
//#define SDI1_SetHigh()    do { LATC4 = 1; } while(0)
//#define SDI1_SetLow()   do { LATC4 = 0; } while(0)
//#define SDI1_Toggle()   do { LATC4 = ~LATC4; } while(0)
//#define SDI1_GetValue()         PORTCbits.RC4
//#define SDI1_SetDigitalInput()    do { TRISC4 = 1; } while(0)
//#define SDI1_SetDigitalOutput()   do { TRISC4 = 0; } while(0)
//
//#define SDI1_SetAnalogMode()   do { ANSC4 = 1; } while(0)
//#define SDI1_SetDigitalMode()   do { ANSC4 = 0; } while(0)
//// get/set SDO1 aliases
//#define SDO1_TRIS               TRISC5
//#define SDO1_LAT                LATC5
//#define SDO1_PORT               PORTCbits.RC5
//#define SDO1_ANS                ANSC5
//#define SDO1_SetHigh()    do { LATC5 = 1; } while(0)
//#define SDO1_SetLow()   do { LATC5 = 0; } while(0)
//#define SDO1_Toggle()   do { LATC5 = ~LATC5; } while(0)
//#define SDO1_GetValue()         PORTCbits.RC5
//#define SDO1_SetDigitalInput()    do { TRISC5 = 1; } while(0)
//#define SDO1_SetDigitalOutput()   do { TRISC5 = 0; } while(0)
//
//#define SDO1_SetAnalogMode()   do { ANSC5 = 1; } while(0)
//#define SDO1_SetDigitalMode()   do { ANSC5 = 0; } while(0)
//// get/set TX1 aliases
//#define TX1_TRIS               TRISC6
//#define TX1_LAT                LATC6
//#define TX1_PORT               PORTCbits.RC6
//#define TX1_ANS                ANSC6
//#define TX1_SetHigh()    do { LATC6 = 1; } while(0)
//#define TX1_SetLow()   do { LATC6 = 0; } while(0)
//#define TX1_Toggle()   do { LATC6 = ~LATC6; } while(0)
//#define TX1_GetValue()         PORTCbits.RC6
//#define TX1_SetDigitalInput()    do { TRISC6 = 1; } while(0)
//#define TX1_SetDigitalOutput()   do { TRISC6 = 0; } while(0)
//
//#define TX1_SetAnalogMode()   do { ANSC6 = 1; } while(0)
//#define TX1_SetDigitalMode()   do { ANSC6 = 0; } while(0)
//// get/set RX1 aliases
//#define RX1_TRIS               TRISC7
//#define RX1_LAT                LATC7
//#define RX1_PORT               PORTCbits.RC7
//#define RX1_ANS                ANSC7
//#define RX1_SetHigh()    do { LATC7 = 1; } while(0)
//#define RX1_SetLow()   do { LATC7 = 0; } while(0)
//#define RX1_Toggle()   do { LATC7 = ~LATC7; } while(0)
//#define RX1_GetValue()         PORTCbits.RC7
//#define RX1_SetDigitalInput()    do { TRISC7 = 1; } while(0)
//#define RX1_SetDigitalOutput()   do { TRISC7 = 0; } while(0)
//
//#define RX1_SetAnalogMode()   do { ANSC7 = 1; } while(0)
//#define RX1_SetDigitalMode()   do { ANSC7 = 0; } while(0)

void PIN_MANAGER_Initialize (void);

void PIN_MANAGER_IOC(void);
void INT_ISR(void);
#endif
