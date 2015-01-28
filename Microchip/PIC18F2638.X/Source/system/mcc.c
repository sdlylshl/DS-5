/**
  @Generated MPLAB® Code Configurator Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using MPLAB® Code Configurator

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.10
        Device            :  PIC18LF23K22
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.33
        MPLAB             :  MPLAB X 2.26
*/

/*
Copyright (c) 2013 - 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*/

// Configuration bits: selected in the GUI


// CONFIG4L
#pragma config LVP = OFF    // Single-Supply ICSP Enable bit->Single-Supply ICSP disabled
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#pragma config DEBUG = OFF    // Background Debug->Disabled

// CONFIG5L
#pragma config CP1 = OFF    // Code Protection Block 1->Block 1 (001000-001FFFh) not code-protected
#pragma config CP0 = OFF    // Code Protection Block 0->Block 0 (000200-000FFFh) not code-protected

// CONFIG5H
#pragma config CPB = OFF    // Boot Block Code Protection bit->Boot block (000000-0001FFh) not code-protected
#pragma config CPD = OFF    // Data EEPROM Code Protection bit->Data EEPROM not code-protected

// CONFIG6L
#pragma config WRT0 = OFF    // Write Protection Block 0->Block 0 (000200-000FFFh) not write-protected
#pragma config WRT1 = OFF    // Write Protection Block 1->Block 1 (001000-001FFFh) not write-protected

// CONFIG6H
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-3000FFh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block (000000-0001FFh) not write-protected

// CONFIG7L
#pragma config EBTR1 = OFF    // Table Read Protection Block 1->Block 1 (001000-001FFFh) not protected from table reads executed in other blocks
#pragma config EBTR0 = OFF    // Table Read Protection Block 0->Block 0 (000200-000FFFh) not protected from table reads executed in other blocks

// CONFIG7H
#pragma config EBTRB = OFF    // Boot Block Table Read Protection bit->Boot Block (000000-0001FFh) not protected from table reads executed in other blocks

#include "mcc.h"

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();
    INTERRUPT_Initialize();
    TMR2_Initialize();
    PWM1_Initialize();
    EUSART_Initialize();

    HLVD_Initialize();
    SPI_Initialize();
    TMR0_Initialize();
    TMR1_Initialize();
    ADC_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // SCS INTOSC; IDLEN disabled; IRCF 1MHz_HFINTOSC/16; 
    OSCCON = 0x32;
    // PRISD enabled; MFIOSEL disabled; SOSCGO disabled; 
   // OSCCON2 = 0x04;
    // INTSRC disabled; PLLEN enabled; TUN 0x00; 
    OSCTUNE = 0x40;
    // Set the secondary oscillator
    
}

/**
 End of File
*/