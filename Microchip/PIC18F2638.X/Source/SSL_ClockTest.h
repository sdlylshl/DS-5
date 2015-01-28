
/*****************************************************************************************
* © 2008  Microchip Technology Inc.
*
* FileName:		    SSL_ClockTest.h
* Dependencies:     Header (.h) files if applicable, see below
* Processor:		PIC18F
* Compiler:		    MPLAB® C18 V3.21 or higher
* Company:	    	Microchip Technology, Inc.
* Version :         1.0
* Date :            10/01/08   (mm/dd/yy)
* Author:           Arpan Kumar
*
* Software License Agreement:
*
* Microchip licenses this software to you solely for use with Microchip products.
* The software is owned by Microchip and its licensors, and is protected under
* applicable copyright laws.  All rights reserved.
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY WARRANTY OF ANY
* KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
* IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST
* OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS BY THIRD
* PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY
* OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
* To the fullest extent allowed by law, Microchip and its licensors liability shall
* not exceed the amount of fees, if any, that you have paid directly to Microchip
* to use this software.
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
*
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* AKA                         First release of include file
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/

#ifndef __SSL_CLOCK_TEST_H__
#define __SSL_CLOCK_TEST_H__



/* 4% tolerence with system clock frequency being 8Mhz*/

#define CLK_MIN_TIME   1920
#define CLK_MAX_TIME   2080



#define CLOCK_ERROR 0

#define CLOCK_NO_ERROR 1

#define timer0_PERIOD_CHECK 30 // Number of interrupts after which the "timer2count" is
                               // to be tested  to be within the boundary limits.
                               // Note: The TMR0 value is set zero in TIMER 1 interrupt
                               // routine after it is saved in global variable .
                               // The ""timer0Count" represents the number of CPU clock cycles
                               //in 1ms(2000 cycles)period of the secondary oscillator.
unsigned int SSL_8bitsFamily_CLOCKtest(void);
void timer0Init(void);
void timer0Start(void);

void Timer1Init(void);
void Timer1Start(void);

#endif
