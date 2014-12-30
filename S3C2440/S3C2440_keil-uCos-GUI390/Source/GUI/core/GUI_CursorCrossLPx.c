/*
*********************************************************************************************************
*   											 uC/GUI
*   					 Universal graphic software for embedded applications
*
*   					(c) Copyright 2002, Micrium Inc., Weston, FL
*   					(c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*   		   �C/GUI is protected by international copyright laws. Knowledge of the
*   		   source code may not be used to write a similar product. This file may
*   		   only be used in accordance with a license and should not be redistributed
*   		   in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File		: GUI_CursorCrossLPx.C
Purpose 	: Defines the pixel offset of the cross cursor, large
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include "GUI_Protected.h"

GUI_CONST_STORAGE unsigned char GUI_Pixels_CrossL[248] = {
0x00, 0x00, 0x00, 0x05, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x56, 0x55, 0x55, 0x55, 0x54,
0x6A, 0xAA, 0xAA, 0xA9, 0xAA, 0xAA, 0xAA, 0xA4,
0x55, 0x55, 0x55, 0x56, 0x55, 0x55, 0x55, 0x54,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x05, 0x40, 0x00, 0x00, 0x00
};

/*************************** End of file ****************************/
