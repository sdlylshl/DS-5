/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIType.h
Purpose     : Include file define the types used for GUI
---------------------------END-OF-HEADER------------------------------

Attention : Do not modify this file ! If you do, you will not
            be able do update to a later GUI version !

*/

#ifndef  GUITYPE_H_INCLUDED
#define  GUITYPE_H_INCLUDED

#include "LCD.h"
#include "GUIConf.h"

/*      *************************************************************
        *                                                           *
        *                Simple types                               *
        *                                                           *
        *************************************************************
*/

typedef const char *  GUI_ConstString;


/*      *************************************************************
        *                                                           *
        *                Structures                                 *
        *                                                           *
        *************************************************************
*/

typedef LCD_COLOR       GUI_COLOR;
typedef LCD_LOGPALETTE  GUI_LOGPALETTE;
typedef LCD_DRAWMODE    GUI_DRAWMODE;
typedef LCD_RECT        GUI_RECT;

typedef struct {
  void      (* pfDraw)(int x0,int y0,int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag);
  GUI_COLOR (* pfIndex2Color)(int Index);
} GUI_BITMAP_METHODS;

typedef struct {
  U16P XSize;
  U16P YSize;
  U16P BytesPerLine;
  U16P BitsPerPixel;
  const U8 GUI_UNI_PTR * pData;
  const GUI_LOGPALETTE GUI_UNI_PTR * pPal;
  const GUI_BITMAP_METHODS * pMethods;
} GUI_BITMAP;

/* This structure may not be changed because the data that it
   expects is read in binary form (via any kind of interface,
   at runtime).
   This structure should therefor not be changed.
*/
typedef struct {
  U16 ID;           /* Version 1.00 => 100*/
  U16 Version;
  U16 XSize;
  U16 YSize;
  U16 BytesPerLine;
  U16 BitsPerPixel;
  U16 NumColors;
  U16 HasTrans;
} GUI_BITMAP_STREAM;

typedef struct {
  int x,y;
  unsigned char Pressed;
} GUI_PID_STATE;

/*
      ****************************************
      *                                      *
      *      FONT structures (new in V1.10)  *
      *                                      *
      ****************************************
*/

/* Translation list. Translates a character code into up to 2
   indices of images to display on top of each other;
   '