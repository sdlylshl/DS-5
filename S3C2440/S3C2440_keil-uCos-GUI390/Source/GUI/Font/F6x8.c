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
File        : F6x8.C
Purpose     : Contains standard font
Height      : 8
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_FontIntern.h"

#ifndef GUI_NOFCHARS
  #define GUI_NOFCHARS (163)
#endif

/*      *********************************
        *                               *
        *   Special character codes     *
        *                               *
        *********************************
*/

/* No 95 to 125 for European character set  */

#define CODE_SACCAGUE     95     /*  small accent ague */
#define CODE_SACCGRAV     96
#define CODE_SACCCIRC     97
#define CODE_SUMLAUT      98
#define CODE_STILDE       99
#define CODE_I_NOPOINT    100

#define CODE_SHARPS       101
#define CODE_A_RING       102
#define CODE_SA_RING      103
#define CODE_AE           104
#define CODE_ETH          105
#define CODE_THORN        106
#define CODE_SMALLAE      107
#define CODE_SMALLETH     108
#define CODE_SMALLTHORN   109
#define CODE_OSLASH       110
#define CODE_SOSLASH      111
#define CODE_LITTLE_A     112
#define CODE_LITTLE_E     113
#define CODE_LITTLE_I     114
#define CODE_LITTLE_O     115
#define CODE_LITTLE_U     116
#define CODE_LITTLE_N     117

#define CODE_INVEXCLAM    118
#define CODE_INVQUEST     119

#define CODE_CACCAGUE     120    /* capital accent ague */
#define CODE_CACCGRAV     121
#define CODE_CACCCIRC     122
#define CODE_CUMLAUT      123
#define CODE_CTILDE       124
#define CODE_CEDILLA      125

/* No 126 to 156 for complete ISO 8859_1 western latin character set  */
#define CODE_NB_SPACE     126
#define CODE_CENT         127
#define CODE_POUND        128
#define CODE_CURRENCY     129
#define CODE_YEN          130
#define CODE_BROKEN_BAR   131
#define CODE_SECTION      132
#define CODE_DIERESIS     133
#define CODE_COPYRIGHT    134
#define CODE_FEMININE     135
#define CODE_LEFT_QUOTE   136
#define CODE_NOT          137
#define CODE_HYPHEN       138
#define CODE_TRADEMARK    139
#define CODE_MACRON       140
#define CODE_DEGREE       141
#define CODE_PLUS_MINUS   142
#define CODE_SUPER_TWO    143
#define CODE_SUPER_THREE  144
#define CODE_ACUTE        145
#define CODE_MICRO        146
#define CODE_PARAGRAPH    147
#define CODE_MIDDLE_DOT   148
#define CODE_SUPER_ONE    149
#define CODE_MASCULINE    150
#define CODE_RIGHT_QUOTE  151
#define CODE_ONE_FOURTH   152
#define CODE_ONE_HALF     153
#define CODE_THREE_FOURTH 154
#define CODE_MULTIPLY     155
#define CODE_DIVISION     156
/* The following are extensions to ISO 8859-1 in the area which is not
   defined by the standard.
*/
#define CODE_ARROW_LEFT   157
#define CODE_ARROW_RIGHT  158
#define CODE_ARROW_UP     159
#define CODE_ARROW_DOWN   160
#define CODE_ENTER        161
#define CODE_CHECKMARK    162



GUI_CONST_STORAGE unsigned char GUI_acFont6x8[GUI_NOFCHARS][8] = {
  {
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

/*  !  */
 ,{
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   ________,
   __X_____,
   ________}


/*  "  */
 ,{
   _X__X___,
   _X__X___,
   _X__X___,
   ________,
   ________,
   ________,
   ________,
   ________}

/* #  */
 ,{
   _X_X____,
   _X_X____,
   XXXXX___,
   _X_X____,
   XXXXX___,
   _X_X____,
   _X_X____,
   ________}

/*  $  */
 ,{
   __X_____,
   _XXXX___,
   X_X_____,
   _XXX____,
   __X_X___,
   XXXX____,
   __X_____}

/*  %  */
 ,{
   XX______,
   XX__X___,
   ___X____,
   __X_____,
   _X______,
   X__XX___,
   ___XX___,
   ________}

/*  &  */
 ,{
   _XX_____,
   X__X____,
   X_X_____,
   _X______,
   X_X_X___,
   X__X____,
   _XX_X___,
   ________}

/*  '  */
 ,{
   _XX_____,
   __X_____,
   _X______,
   ________,
   ________,
   ________,
   ________,
   ________}

/*  (  */
 ,{
   ___X____,
   __X_____,
   _X______,
   _X______,
   _X______,
   __X_____,
   ___X____,
   ________}

/*  )  */
 ,{
   _X______,
   __X_____,
   ___X____,
   ___X____,
   ___X____,
   __X_____,
   _X______,
   ________}

/*  *  */
 ,{
   ________,
   _X_X____,
   __X_____,
   XXXXX___,
   __X_____,
   _X_X____,
   ________,
   ________}

 ,{
   ________,
   __X_____,
   __X_____,
   XXXXX___,
   __X_____,
   __X_____,
   ________,
   ________}

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   _XX_____,
   __X_____,
   _X______}

 ,{
   ________,
   ________,
   ________,
   XXXXX___,
   ________,
   ________,
   ________,
   ________}

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   _XX_____,
   _XX_____,
   ________}

 ,{
   ________,
   ____X___,
   ___X____,
   __X_____,
   _X______,
   X_______,
   ________,
   ________}

/* 0 */
 ,{
   _XXX____,
   X___X___,
   X__XX___,
   X_X_X___,
   XX__X___,
   X___X___,
   _XXX____,
   ________}

/* 1 */
 ,{
   __X_____,
   _XX_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   _XXX____,
   ________}

/* 2 */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   __XX____,
   _X______,
   X_______,
   XXXXX___,
   ________}

/* 3 */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   __XX____,
   ____X___,
   X___X___,
   _XXX____,
   ________}

/* 4 */
 ,{
   ___X____,
   __XX____,
   _X_X____,
   X__X____,
   XXXXX___,
   ___X____,
   ___X____,
   ________}

/* 5 */
 ,{
   XXXXX___,
   X_______,
   XXXX____,
   ____X___,
   ____X___,
   X___X___,
   _XXX____,
   ________}

/* 6 */
 ,{
   __XX____,
   _X______,
   X_______,
   XXXX____,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* 7 */
 ,{
   XXXXX___,
   ____X___,
   ___X____,
   __X_____,
   _X______,
   _X______,
   _X______,
   ________}

/* 8 */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   _XXX____,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* 9 */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   _XXXX___,
   ____X___,
   ___X____,
   _XX_____,
   ________}

/* ':' 3a */
 ,{
   ________,
   _XX_____,
   _XX_____,
   ________,
   _XX_____,
   _XX_____,
   ________,
   ________}

/* ';' 3b */
 ,{
   ________,
   ________,
   _XX_____,
   _XX_____,
   ________,
   _XX_____,
   __X_____,
   _X______}


/* '<' 3c */
 ,{
   ___X____,
   __X_____,
   _X______,
   X_______,
   _X______,
   __X_____,
   ___X____,
   ________}

/* '=' 3d */
 ,{
   ________,
   ________,
   XXXXX___,
   ________,
   XXXXX___,
   ________,
   ________,
   ________}

/* '>' */
 ,{
   X_______,
   _X______,
   __X_____,
   ___X____,
   __X_____,
   _X______,
   X_______,
   ________}

/* '?' */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   ___X____,
   __X_____,
   ________,
   __X_____,
   ________}

/* @ */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   _XX_X___,
   X_X_X___,
   X_X_X___,
   _XXX____,
   ________}

/* A */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   XXXXX___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* B */
 ,{
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   ________}

/* C */
 ,{
   _XXX____,
   X___X___,
   X_______,
   X_______,
   X_______,
   X___X___,
   _XXX____,
   ________}

/* D */
 ,{
   XXX_____,
   X__X____,
   X___X___,
   X___X___,
   X___X___,
   X__X____,
   XXX_____,
   ________}

/* E */
 ,{
   XXXXX___,
   X_______,
   X_______,
   XXXX____,
   X_______,
   X_______,
   XXXXX___,
   ________}

/* 1 */
 ,{
   XXXXX___,
   X_______,
   X_______,
   XXXX____,
   X_______,
   X_______,
   X_______,
   ________}

/* 1 */
 ,{
   _XXX____,
   X___X___,
   X_______,
   X_______,
   X__XX___,
   X___X___,
   _XXXX___,
   ________}

/* 1 */
 ,{
   X___X___,
   X___X___,
   X___X___,
   XXXXX___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* I */
 ,{
   _XXX____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   _XXX____,
   ________}

/* J */
 ,{
   __XXX___,
   ___X____,
   ___X____,
   ___X____,
   ___X____,
   X__X____,
   _XX_____,
   ________}

/* K */
 ,{
   X___X___,
   X__X____,
   X_X_____,
   XX______,
   X_X_____,
   X__X____,
   X___X___,
   ________}

/* L */
 ,{
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   XXXXX___,
   ________}

/* M */
 ,{
   X___X___,
   XX_XX___,
   X_X_X___,
   X_X_X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* N */
 ,{
   X___X___,
   X___X___,
   XX__X___,
   X_X_X___,
   X__XX___,
   X___X___,
   X___X___,
   ________}

/* O */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* P */
 ,{
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X_______,
   X_______,
   X_______,
   ________}

/* Q */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   X___X___,
   X_X_X___,
   X__X____,
   _XX_X___,
   ________}

/* R */
 ,{
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X_X_____,
   X__X____,
   X___X___,
   ________}

/* S */
 ,{
   _XXX____,
   X___X___,
   X_______,
   _XXX____,
   ____X___,
   X___X___,
   _XXX____,
   ________}

/* T */
 ,{
   XXXXX___,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   ________}

/* U */
 ,{
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* V */
 ,{
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   ________}

/* W */
 ,{
   X___X___,
   X___X___,
   X___X___,
   X_X_X___,
   X_X_X___,
   X_X_X___,
   _X_X____,
   ________}

/* X */
 ,{
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   _X_X____,
   X___X___,
   X___X___,
   ________}

/* Y */
 ,{
   X___X___,
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   __X_____,
   __X_____,
   ________}

/* Z */
 ,{
   XXXXX___,
   ____X___,
   ___X____,
   __X_____,
   _X______,
   X_______,
   XXXXX___,
   ________}

/* 5b */
 ,{
   _XXX____,
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   _XXX____,
   ________}

/* 5c */
 ,{
   ________,
   X_______,
   _X______,
   __X_____,
   ___X____,
   ____X___,
   ________,
   ________}

/* 5d */
 ,{
   _XXX____,
   ___X____,
   ___X____,
   ___X____,
   ___X____,
   ___X____,
   _XXX____,
   ________}

/* 5e */
 ,{
   __X_____,
   _X_X____,
   X___X___,
   ________,
   ________,
   ________,
   ________,
   ________}

/* 5f */
 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   XXXXX___}

/* 60 */
 ,{
   _X______,
   __X_____,
   ___X____,
   ________,
   ________,
   ________,
   ________,
   ________}

/* a */
 ,{
   ________,
   ________,
   _XXX____,
   ____X___,
   _XXXX___,
   X___X___,
   _XXXX___,
   ________}

/* b */
 ,{
   X_______,
   X_______,
   X_XX____,
   XX__X___,
   X___X___,
   X___X___,
   XXXX____,
   ________}

/* c */
 ,{
   ________,
   ________,
   _XXX____,
   X_______,
   X_______,
   X___X___,
   _XXX____,
   ________}

/* d */
 ,{
   ____X___,
   ____X___,
   _XX_X___,
   X__XX___,
   X___X___,
   X___X___,
   _XXXX___,
   ________}

/* e */
 ,{
   ________,
   ________,
   _XXX____,
   X___X___,
   XXXXX___,
   X_______,
   _XXX____,
   ________}

/* f */
 ,{
   __XX____,
   _X__X___,
   _X______,
   XXX_____,
   _X______,
   _X______,
   _X______,
   ________}

/* g */
 ,{
   ________,
   ________,
   _XXXX___,
   X___X___,
   X___X___,
   _XXXX___,
   ____X___,
   _XXX____}

/* h */
 ,{
   X_______,
   X_______,
   X_XX____,
   XX__X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* i */
 ,{
   __X_____,
   ________,
   _XX_____,
   __X_____,
   __X_____,
   __X_____,
   _XXX____,
   ________}

/* j */
 ,{
   ___X____,
   ________,
   __XX____,
   ___X____,
   ___X____,
   ___X____,
   X__X____,
   _XX_____}

/* k */
 ,{
   X_______,
   X_______,
   X__X____,
   X_X_____,
   XX______,
   X_X_____,
   X__X____,
   ________}

/* l */
 ,{
   _XX_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   _XXX____,
   ________}

/* m */
 ,{
   ________,
   ________,
   XX_X____,
   X_X_X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* n */
 ,{
   ________,
   ________,
   X_XX____,
   XX__X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* o */
 ,{
   ________,
   ________,
   _XXX____,
   X___X___,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* p */
 ,{
   ________,
   ________,
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X_______,
   X_______}

/* q */
 ,{
   ________,
   ________,
   _XXXX___,
   X___X___,
   X___X___,
   _XXXX___,
   ____X___,
   ____X___}

/* r */
 ,{
   ________,
   ________,
   X_XX____,
   XX__X___,
   X_______,
   X_______,
   X_______,
   ________}

/* s */
 ,{
   ________,
   ________,
   _XXXX___,
   X_______,
   _XXX____,
   ____X___,
   XXXX____,
   ________}

/* t */
 ,{
   _X______,
   _X______,
   XXX_____,
   _X______,
   _X______,
   _X__X___,
   __XX____,
   ________}

/* u */
 ,{
   ________,
   ________,
   X___X___,
   X___X___,
   X___X___,
   X__XX___,
   _XX_X___,
   ________}

/* v */
 ,{
   ________,
   ________,
   X___X___,
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   ________}

/* w */
 ,{
   ________,
   ________,
   X___X___,
   X___X___,
   X_X_X___,
   X_X_X___,
   _X_X____,
   ________}

/* X */
 ,{
   ________,
   ________,
   X___X___,
   _X_X____,
   __X_____,
   _X_X____,
   X___X___,
   ________}

/* y */
 ,{
   ________,
   ________,
   X___X___,
   X___X___,
   X___X___,
   _XXXX___,
   ____X___,
   _XXX____}

/* z */
 ,{
   ________,
   ________,
   XXXXX___,
   ___X____,
   __X_____,
   _X______,
   XXXXX___,
   ________}

/* 0x7b */
 ,{
   ___X____,
   __X_____,
   __X_____,
   _X______,
   __X_____,
   __X_____,
   ___X____,
   ________}

/* 0x7c */
 ,{
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   ________}

/* 0x7d */
 ,{
   _X______,
   __X_____,
   __X_____,
   ___X____,
   __X_____,
   __X_____,
   _X______,
   ________}

/* 0x7e */
 ,{
   _XX_X___,
   X__X____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}


/*  additional characters for European character set  */

#if (GUI_NOFCHARS>95)
/* small accent ague, 95*/
 ,{
   ___XX___,
   __X_____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

#endif
#if (GUI_NOFCHARS>96)

/* small accent grave, 96 */
 ,{
   XX______,
   __X_____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

#endif
#if (GUI_NOFCHARS>97)

/* small accent circonflex, 97 */
 ,{
   _XXX____,
   X___X___,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

#endif
#if (GUI_NOFCHARS>98)

/* small umlaut, 98 */
 ,{
   _X_X____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

#endif
#if (GUI_NOFCHARS>99)

/* small tilde, 99 */
 ,{
   _XX_X___,
   X__X____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

#endif
#if (GUI_NOFCHARS>100)

/* i without dot, 100 */
 ,{
   ________,
   ________,
   ________,
   _XX_____,
   __X_____,
   __X_____,
   _XXX____,
   ________}

#endif
#if (GUI_NOFCHARS>101)

/* sharps eg 