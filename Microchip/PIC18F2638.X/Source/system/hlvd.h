/**
  HLVD Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    hlvd.h

  @Summary
    This is the generated header file for the HLVD driver using MPLAB® Code Configurator

  @Description
    This header file provides APIs for driver for HLVD.
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

#ifndef _HLVD_H
#define _HLVD_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif

/**
  Section: Enum Declarations
*/

/** HLVD Trip points
 
 @Summary 
   This defines trip point values.
 
 @Description
   This defines trip point values.

 */
typedef enum
{
    HLVD_TRIP_POINT_0 = 15, // external reference voltage
    HLVD_TRIP_POINT_1 = 14,
    HLVD_TRIP_POINT_2 = 13,
    HLVD_TRIP_POINT_3 = 12,
    HLVD_TRIP_POINT_4 = 11,
    HLVD_TRIP_POINT_5 = 10, 
    HLVD_TRIP_POINT_6 = 9,
    HLVD_TRIP_POINT_7 = 8,
    HLVD_TRIP_POINT_8 = 7,
    HLVD_TRIP_POINT_9 = 6,
    HLVD_TRIP_POINT_10 = 5,
    HLVD_TRIP_POINT_11 = 4,
    HLVD_TRIP_POINT_12 = 3,
    HLVD_TRIP_POINT_13 = 2,
    HLVD_TRIP_POINT_14 = 1,
    HLVD_TRIP_POINT_15 = 0
}HLVD_TRIP_POINTS;

/** HLVD voltage direction
 
 @Summary 
   This defines voltage direction.
 
 @Description
   This defines voltage direction.

 */
typedef enum
{
   BELOW_TRIP_POINT  = 0,
   EXCEED_TRIP_POINT = 1
}HLVD_TRIP_DIRECTION;

/**
  Section: HLVD Module APIs
*/

/**
  @Summary
    Initializes the HLVD

  @Description
    This routine initializes the Initializes the HLVD.
    This routine must be called before any other HLVD routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Example
    <code>
    bool gAppShutdown = false;
    {
      //initialize the module
      HLVD_Initialize();
      //check if reference voltage is stable 
      while(!HLVD_IsReferenceVoltageStable());
      while(1)
      {
         //Polling
         HLVD_Tasks();
      }
    }
    void HLVD_TripDetectionCallback(void)
    {
       //process application code here
       gAppShutdown = true; 
    }   
    </code> 
*/
void HLVD_Initialize(void);

/**
  @Summary
    This function enables the HLVD peripheral.

  @Description
    This function enables the HLVD peripheral.

  @Preconditions
    HLVD_Initialize Is_HLVD_Ref_Voltage_Stable
   function should be called first.

  @Param
    None

  @Returns
    None.

  @Example
  <code>
      HLVD_Disable();
      //Set the trip points and voltage direction
      HLVD_TripPointSetup(EXCEED_TRIP_POINT,
        HLVD_TRIP_POINT_1);
      //Enable the module
      HLVD_Enable();
      //check if reference voltage is stable 
      while(!HLVD_IsReferenceVoltageStable());
      while(1)
      {
         
      }
    void HLVD_TripDetectionCallback(void)
    {
       //process application code here      
    }
   </code>     
 */
void HLVD_Enable(void);

/**
  @Summary
    This function disables the HLVD peripheral.

  @Description
    This function disables the HLVD peripheral.

  @Preconditions
    HLVD_Initialize and
    HLVD_IsReferenceVoltageStable
    function should be called first.

  @Param
    None

  @Returns
    None.

  @Example
   Refer the HLVD_Enable code example
*/
void HLVD_Disable(void);

/**
  @Summary
    This function returns the status of internal reference voltage.

  @Description
    This function returns the status of internal reference voltage.

  @Preconditions
    HLVD_Initialize function should be called first

  @Param
    None

  @Returns
    bool - true - internal reference voltage is stable
           false - internal reference voltage is unstable

  @Example
  Refer the HLVD_Enable code example
*/
bool HLVD_IsReferenceVoltageStable(void);

/**
  @Summary
    This function returns the status of band gap voltage.

  @Description
    This function returns the status of band gap voltage.

  @Preconditions
    HLVD_Initialize function should be called first

  @Param
    None

  @Returns
    bool - true - Band gap voltage is stable
           false - Band gap voltage is unstable

  @Example
  Refer the HLVD_Enable code example
*/
bool HLVD_IsBandGapVoltageStable(void);

/**
  @Summary
    This function sets the voltage trip direction and trip value

  @Description
    This function sets the voltage trip direction and trip value

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Example
   Refer the HLVD_Enable code example
*/
void HLVD_TripPointSetup(HLVD_TRIP_DIRECTION direction,
        HLVD_TRIP_POINTS trip_points);

/**
  @Summary
  This is a HLVD task function. This routine is used to poll the HLVD
  interrupt flag bit.

  @Description
    This is a polling function. Interrupt flag is monitored periodicaly
   to check the voltage exceeds or drops below the reference voltage.
   If VDIR bit is set then LVDIF flag is set if voltage exceeds reference voltage.
   If VDIR bit is clear then LVDIF flag is set if voltage drops below the
   reference voltage.

  @Returns
    None

  @Param
    None
*/
void HLVD_Tasks( void );

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif	//_HLVD_H
/**
 End of File
*/

