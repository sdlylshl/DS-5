
/**
  HLVD Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
   hlvd.c

  @Summary
    This is the generated driver implementation file for the HLVD driver using MPLAB® Code Configurator

  @Description
    This source file provides implementations for driver APIs for HLVD.
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
#include "hlvd.h"

/**
  Section: HLVD Module APIs
*/

void HLVD_Initialize(void)
{
    // set the HLVD_Initialize module to the options selected in the User Interface
     // BGVST disabled; VDIRMAG Falls Below Trip Point; HLVDEN enabled; IRVST disabled; HLVDL Min:4.39 Max:5.09; 
    HLVDCON = 0x1E;
    PIR2bits.LVDIF = 0;
}

bool HLVD_IsReferenceVoltageStable(void)
{
    //return internal reference voltage status
    return(HLVDCONbits.IRVST);
}

//bool HLVD_IsBandGapVoltageStable(void)
//{
//    //return band gap voltage status
//    return(HLVDCONbits.BGVST);
//}

void HLVD_Enable(void)
{
    // enable HLVD module
    HLVDCONbits.HLVDEN = 1;
    PIR2bits.LVDIF = 0;
}

void HLVD_Disable(void)
{
    // disable HLVD module
    HLVDCONbits.HLVDEN = 0; 
}

void HLVD_TripPointSetup(HLVD_TRIP_DIRECTION direction,
        HLVD_TRIP_POINTS trip_points)
{
   //set Voltage trip direction
   HLVDCONbits.VDIRMAG = direction;
   // Set trip points
   HLVDCONbits.HLVDL = trip_points;
}

void HLVD_Tasks( void )
{
    if(PIR2bits.LVDIF)
    {
        /* TODO : Add interrupt handling code */
        PIR2bits.LVDIF = 0;
    }
}

/**
 End of File
*/
