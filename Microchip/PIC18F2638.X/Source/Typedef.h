/*****************************************************************************************
* © 2008  Microchip Technology Inc. 
* 
* FileName:		    SSL_Typedef.h
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

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;

typedef union 
{
	UWord32 Val32;

	struct
	{
		UWord16 LW;
		UWord16 HW;
	} Word;

	char Val[4];
} uReg32;

#endif

