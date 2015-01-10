/*
*********************************************************************************************************
*   											 uC/GUI
*   					 Universal graphic software for embedded applications
*
*   					(c) Copyright 2002, Micrium Inc., Weston, FL
*   					(c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*   		   礐/GUI is protected by international copyright laws. Knowledge of the
*   		   source code may not be used to write a similar product. This file may
*   		   only be used in accordance with a license and should not be redistributed
*   		   in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File		: GUI2DLib.C
Purpose 	: Main part of the 2D graphics library
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h> 		  /* needed for definition of NULL */
#include "GUI_Protected.h"
#include "GUIDebug.h"

/*********************************************************************
*
*   	defines
*
**********************************************************************
*/

#define ABS(v) ((v>0) ? v : -v)

/*********************************************************************
*
*   	static code, helper functions
*
**********************************************************************
*/
/*********************************************************************
*
*   	_SwapInt
*/
static void _SwapInt(int *pa, int *pb)
{
	int t;
	t = *pa;
	*pa = *pb;
	*pb = t;
}

/*********************************************************************
*
*   	Abs
*/
static int Abs(int v)
{
	return ABS(v);
}

/*********************************************************************
*
*   	trigonometric functions
*
*  NOTE:
*   All trigonometric functions are for internal usage only and
*   use the following conventions:
*
*      Angles: 4096 <==> 360