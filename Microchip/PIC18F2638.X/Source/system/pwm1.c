/**
  CCP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    CCP1.c

  @Summary
    This is the generated driver implementation file for the CCP1 driver using MPLAB® Code Configurator

  @Description
    This source file provides implementations for driver APIs for CCP1.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.10
        Device            :  PIC18LF23K22
        Driver Version    :  2.00
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "PWM1.h"

/**
  Section: Macro Declarations
*/

#define PWM1_INITIALIZE_DUTY_VALUE    499

/**
  Section: PWM Module APIs
*/

void PWM1_Initialize(void)
{
    // Set the PWM to the options selected in the MPLAB® Code Configurator

    // CCP1M PWM; DC4B 48;
    CCP1CON = 0x3C;
    
    // CCPR1L 124;
    CCPR1L = 0x7C;
    
    // CCPR1H 0x0;
    CCPR1H = 0x00;
    

    // Selecting Timer2
  //  CCPTMRS1bits.C4TSEL = 0x0;
}

void PWM1_LoadDutyValue(uint16_t dutyValue)
{
   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR1L = ((dutyValue & 0x03FC)>>2);
    
   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP1CON = (CCP1CON & 0xCF) | ((dutyValue & 0x0003)<<4);
}
/**
 End of File
*/

