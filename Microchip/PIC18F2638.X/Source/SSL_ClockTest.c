
/*****************************************************************************************
* � 2008  Microchip Technology Inc.
*
* FileName:		    SSL_ClockTest.c
* Dependencies:     Header (.h) files if applicable, see below
* Processor:		PIC18F
* Compiler:		    MPLAB� C18 V3.21 or higher
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
* SOFTWARE IS PROVIDED �AS IS.�  MICROCHIP EXPRESSLY DISCLAIMS ANY WARRANTY OF ANY
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
* AKA                         First release of source file
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include ".\SSL_ClockTest.h"
#include <p18cxxx.h>
#include ".\Typedef.h"

volatile unsigned int clockTestFlag=0;

uReg32 timer0Count;
void InterruptHandlerHigh (void);


  /************************************************************************************************
  * Description:
  *     The SSL_8bitsFamily_CLOCKtest function is used to verify proper operation of the CPU clock when
  *      the secondary oscillator is used as the reference clock.
  *       The test performs the following major tasks:
  *			1. The LP secondary oscillator is used as the independent clock source or the reference clock
  *            source. The secondary oscillator, which is designed with 32 kHz crystal, is used as a clock
  *            source to the Timer1.
  *         2. The Primary oscillator with PLL is the clock source to the CPU. The timer0 runs at the CPU
  *            clock frequency.
  *         3. The Timer1 is configured to generate an interrupt at specified intervals of time (e.g., 1 ms).
  *
  *         4. The contents of TMR3 register in timer0 module are saved in the Timer1 interrupt handler. This
  *             value represents the number of CPU Clock cycles of the secondary clock in 1ms. If the number
  *             of clock cycles crosses the defined boundary limits, the function sets an error flag.
  *             For example, the following parameters are used to calculate CLK_MIN_TIME and CLK_MAX_TIME for a
  *             PIC18F device:
  *				Primary Oscillator: INTOSC
  *				FOSC: 8 Mhz
  *				FCY: FOSC/4: (8 * 10^6) / 4
  *				FCY: 2000000
  *				Secondary oscillator: 32KHz
  *			    Timer1 period: 31
  *             Therefore, with 4% tolerance, the number of CPU clock cycles in 1ms (2000 cycles) are:
  *					� CLK_MIN_TIME: 1920
  *					� CLK_MAX_TIME: 2080
  *  Input: None
  *
  * Return Values:
  *   	CLOCK_NO_ERROR : CPU clock is operating within its limits.
  *     CLOCK_ERROR    : CPU clock is not operating within limits
  ****************************************************************************************************/



unsigned int SSL_8bitsFamily_CLOCKtest(void )
{

  INTCON= 0XC0;                       // enable global interrupts
  timer0Init();  					  // Initialize Timer 0

  Timer1Init();                       // Initialze Timer 1

  T0CONbits.TMR0ON=1; 				  // Enable Timer 0

  Timer1Start();                      // Enable Timer 1

  clockTestFlag=0;

    while ( clockTestFlag <timer0_PERIOD_CHECK);     // Wait till clockTestFlag=30(let the clock stablized till then) and
                                                     // read timer0count value

	PIE1bits.TMR1IE = 0;         // disable Timer1 interrupts

    clockTestFlag=0;

    if (timer0Count.Word.LW > CLK_MAX_TIME || timer0Count.Word.LW < CLK_MIN_TIME )
  		 return CLOCK_ERROR;
    else
         return CLOCK_NO_ERROR;
}



void Timer1Init(void )
{

    T1CON = 0x0A;            	 //do not run the Timer1, Timer1 oscillator(32khz) is enabled
	TMR1H = 0xff;  		     	 //Initialize timer1 reg so that it interrupts after 1ms
	TMR1L = 0xE0;

}

void Timer1Start(void)
{
    
	PIR1bits.TMR1IF=0;           //Clear the Timer1 interrupt status flag
	PIE1bits.TMR1IE = 1;         //Enable Timer1 interrupts
	T1CONbits.TMR1ON=1;          // Enable Timer
}



void timer0Init(void)
{
    T0CON = 0x08;
	TMR0H = 0x00;            	 //Clear contents of the timer0 register
	TMR0L = 0x00;

}


// High priority interrupt vector

// interrupt sub-routine is written in the src file "SSL_ClockTest_LineFreq.c"




