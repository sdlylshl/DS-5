
/*****************************************************************************************
* � 2008  Microchip Technology Inc. 
* 
* FileName:		    ClassB_main.c
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
* MVL			13/04/2010
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/

#include <p18cxxx.h>
#include "stdint.h"
#include "Typedef.h"
//#include ".\h\SSL_MarchcStackTest.h"
//#include "..\h\SSL_PcTest.h"
//#include "..\h\SSL_MarchC.h"
//#include "..\h\SSL_MarchB.h"
//#include "..\h\SSL_Flash_CRC.h"
//#include "..\h\SSL_EEPROM_CRC.h"        //comment the line for the devices not having EEPROM
#include ".\SSL_ClockTest.h"
//#include "..\h\SSL_ClockTest_LineFreq.h"
//#include "..\h\SSL_MarchC_RamAndStackTest.h"
#include "./system/mcc.h"

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)
unsigned int testResult=0;

struct  ClassB_Test_Flags 
{

unsigned int  cpuRegister_TestResult: 1 ;
unsigned int  programCounter_TestResult: 1;
unsigned int  checkerboardRam_TestResult: 1;
unsigned int  marchCRam_TestResult: 1;
unsigned int  marchC_minus_Ram_TestResult: 1;
unsigned int  marchBRam_TestResult:1;
unsigned int  flash_TestResult: 1;          //does not get affected directly, user has to see the checksum values obtained at different times and then set the bit if both r same
unsigned int  eeprom_TestResult: 1;			//does not get affected directly, user has to see the checksum values obtained at different times and then set the bit if both r same 
unsigned int  clock_TestResult: 1;			// 32KHz crystal has to be used as TIMER1 OSC to get the test result correctly
unsigned int  line_clock_TestResult: 1;     // connect a 50/60Hz input to CCP1 pin to get the test result correctly
unsigned int  marchCRamStack_TestResult: 1;

} testFlag={0,0,0,0,0,0,0,0,0,0,0};         // initialize the bits as 0

    
    
 void main(void)
{

 
    /* Variables for Flash Test*/
    uint32_t startAddress;
    uint32_t endAddress;
    unsigned int flash_crc_Result=0;
    unsigned int init_CrcValue=0x0345;           //user can change it 

  
   /* Variables for EEPROM Test */

    uReg32 startAddress2;
    uReg32 endAddress2;
    unsigned int  eeprom_crc_Result;
    unsigned int init_CrcValue2=0x0345;			 //user can change it 

  

    /*Variables for RAM Test */
    char * ramStartAddress;
    unsigned int ramSize;


/* setting the internal clock to 8 Mhz for p18f46K20, 
   comment the line for the devices which configuration has to be set in configuration setting option in MPLAB IDE */
	
	OSCCON = 0b01100000;      

        
  
// To invoke the "SSL_8bitsFamily_RAM_STACKtest_MarchC" function include the file "SSL_MarchC_RamAndStackTest.c" 
// and comment the "SSL_8bitsFamily_RAMtest_MarchC" function and remove the "SSL_MarchCRamTest.c"
// from the workspace.

 /****************************/
 /* MarchC RAM and Stack Test*/
 /****************************/
//    ramStartAddress = ( char *) MARCHC_RAMTEST_START_ADDRESS ;
//
//    ramSize = MARCHC_RAMTEST_SIZE;
//
//    if (SSL_8bitsFamily_RAM_STACKtest_MarchC(ramStartAddress,ramSize ))
//        testFlag.marchCRamStack_TestResult = 1;
//    else
//        testFlag.marchCRamStack_TestResult = 0;



/**********************************************************************************/
/*                                  CPU REGISTER TEST                             */                              
/**********************************************************************************/
 //  SSL_8bitsFamily_CPU_RegisterTest();
//    if(WREG)											//(WREG==0x13) 	MVL 13/04/2010
//         testFlag.cpuRegister_TestResult = 0;			//1 			MVL 13/04/2010
//    else
//         testFlag.cpuRegister_TestResult = 1; 			//0				MVL 13/04/2010

/**********************************************************************************/
/*                                  PROGRAM COUNTER TEST                          */                              
/**********************************************************************************

    if (SSL_8bitsFamily_PCtest()) 
         testFlag.programCounter_TestResult = 1;
    else 
         testFlag.programCounter_TestResult = 0; 

/**********************************************************************************/
/*                                  RAM TEST                                      */                              
/**********************************************************************************/
   
   /*************************/
   /* Checker Board RAM test*/
   /*************************/
     
     //SSL_8bitsFamily_Ram_CB_test();
//    if 	(WREG) 											//(WREG==0x13) 	MVL 13/04/2010
//        testFlag.checkerboardRam_TestResult = 0;		//1				MVL 13/04/2010
//    else
//        testFlag.checkerboardRam_TestResult = 1;		//0				MVL 13/04/2010





   /*************************/
   /*    MarchC RAM test    */
   /*************************/
  
//     ramStartAddress = ( char *) MARCHC_RAM_START_ADDRESS ;
//
//     ramSize = MARCHC_RAM_SIZE;
//
//     if (SSL_8bitsFamily_RAMtest_MarchC(ramStartAddress,ramSize ))
//        testFlag.marchCRam_TestResult = 1;
//     else
//        testFlag.marchCRam_TestResult = 0;

   /**************************/
   /* MarchC Minus RAM test  */
   /**************************/

//     ramStartAddress = ( char *) MARCHC_RAM_START_ADDRESS ;
//
//     ramSize = MARCHC_RAM_SIZE;
//
//     if (SSL_8bitsFamily_RAMtest_MarchC_Minus(ramStartAddress,ramSize ))
//        testFlag.marchC_minus_Ram_TestResult = 1;
//     else
//      testFlag.marchC_minus_Ram_TestResult = 0;
        
        
  /****************************/
  /*    March B Ram Test      */
  /****************************/

//      ramStartAddress = (char *) MARCHB_RAM_START_ADDRESS ;
//
//      ramSize = MARCHB_RAM_SIZE;
//
//      if (SSL_8bitsFamily_RAMtest_MarchB(ramStartAddress,ramSize ))
//        testFlag.marchBRam_TestResult = 1;
//      else
//         testFlag.marchBRam_TestResult = 0;

  
/**********************************************************************************/
/*                                  FLASH TEST                                    */                              
/**********************************************************************************/
 // This function can be called at startup to generate the Reference checksum.The 
 // same function can be called periodically and the generated checksum can be 
 // compared with the reference checksum. If both are the same the "flash_TestResult"
 // status bit can be set. 
//    startAddress= FLASH_STARTADDRESS;
//
//    endAddress =  FLASH_ENDADDRESS;
//
//    flash_crc_Result = SSL_8bitsFamily_Flashtest_CRC16(startAddress,endAddress,init_CrcValue);



//comment the below written EEPROM test call for the devices not having EEPROM
    
/**********************************************************************************/
/*                                  EEPROM TEST                                   */                              
/**********************************************************************************/
 // This function can be called at startup to generate the Reference checksum.The 
 // same function can be called periodically and the generated checksum can be 
 // compared with the reference checksum. If both are the same the "eeprom_TestResult"
 // status bit can be set

//    startAddress2.Val32= EEPROM_STARTADDRESS;
//
//    endAddress2.Val32 =  EEPROM_ENDADDRESS;
//
//    eeprom_crc_Result = SSL_8bitsFamily_EEPROMtest_CRC16(startAddress2,endAddress2,init_CrcValue2);   



     
        


/**********************************************************************************/
/*        CLOCK  TEST WITH SECONDARY OSCILLATOR AS REFERENCE CLOCK                */                             
/**********************************************************************************/

   	  // if (SSL_8bitsFamily_CLOCKtest())
         // testFlag.clock_TestResult=1;
      // else
       //   testFlag.clock_TestResult=0;
/**********************************************************************************/
/*      CLOCK  TEST WITH 50HZ/60Hz LINE FREQUENCY AS REFERENCE CLOCK              */                             
/**********************************************************************************/
	   
//        if (SSL_8bitsFamily_CLOCKtest_LineFreq())
//          testFlag.line_clock_TestResult=1;
//        else
//          testFlag.line_clock_TestResult=0;
    TRISBbits.RB5 = 0;//output
      TRISBbits.TRISB3 = 0;//output
      PORTBbits.RB5 = 1;
      PORTBbits.RB3 = 1;
      TMR0_Initialize();
      INTERRUPT_Initialize();
      INTERRUPT_GlobalInterruptEnable();
      INTERRUPT_PeripheralInterruptDisable();
      while(1){
            //Delay_ms();
            if (timer0 >10000){
                //timer0 =0;
                 PORTBbits.RB5 = 0;
            }
            if (timer0 >20000){
                timer0 =0;
                 PORTBbits.RB5 = 1;
            }
      }
      
}
 


