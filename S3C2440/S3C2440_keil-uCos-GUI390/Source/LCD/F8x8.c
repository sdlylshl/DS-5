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
File        : F8x8.C
Purpose     : Implementation of 8x8 pixel font
Height      : 8
---------------------------END-OF-HEADER------------------------------
*/

//#include "GUI_FontIntern.h"

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





/*
    ****************************************************************
    *                                                              *
    *                      8 * 8  font                             *
    *                                                              *
    ****************************************************************
*/


const unsigned char GUI_F8x8_acFont[][8] = {

  {________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}  /* char ' '  */

 ,{
   ___XX___,
   __XXXX__,
   __XXXX__,
   ___XX___,
   ___XX___,
   ________,
   ___XX___,
   ________}  /* char '!'  */

 ,{
   _XX__XX_,
   _XX__XX_,
   __X__X__,
   ________,
   ________,
   ________,
   ________,
   ________}  /* char '"'  */

 ,{
   _XX_XX__,
   _XX_XX__,
   XXXXXXX_,
   _XX_XX__,
   XXXXXXX_,
   _XX_XX__,
   _XX_XX__,
   ________}  /* char '#'  */

 ,{
   ___XX___,
   __XXXXX_,
   _XX_____,
   __XXXX__,
   _____XX_,
   _XXXXX__,
   ___XX___,
   ________}  /* char '$'  */

 ,{
   ________,
   XX___XX_,
   XX__XX__,
   ___XX___,
   __XX____,
   _XX__XX_,
   XX___XX_,
   ________}  /* char '%'  */

 ,{
   __XXX___,
   _XX_XX__,
   __XXX___,
   _XXX_XX_,
   XX_XXX__,
   XX__XX__,
   _XXX_XX_,
   ________}  /* char '&'  */

 ,{
   ___XX___,
   ___XX___,
   __XX____,
   ________,
   ________,
   ________,
   ________,
   ________}  /* char '''  */

 ,{
   ____XX__,
   ___XX___,
   __XX____,
   __XX____,
   __XX____,
   ___XX___,
   ____XX__,
   ________}  /* char '('  */

 ,{
   __XX____,
   ___XX___,
   ____XX__,
   ____XX__,
   ____XX__,
   ___XX___,
   __XX____,
   ________}  /* char ')'  */

 ,{
   ________,
   _XX__XX_,
   __XXXX__,
   XXXXXXXX,
   __XXXX__,
   _XX__XX_,
   ________,
   ________}  /* char '*'  */

 ,{
   ________,
   ___XX___,
   ___XX___,
   _XXXXXX_,
   ___XX___,
   ___XX___,
   ________,
   ________}  /* char '+'  */

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   ___XX___,
   ___XX___,
   __XX____}  /* char ','  */

 ,{
   ________,
   ________,
   ________,
   _XXXXXX_,
   ________,
   ________,
   ________,
   ________}  /* char '-'  */

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   ___XX___,
   ___XX___,
   ________}  /* char '.'  */

 ,{
   _____XX_,
   ____XX__,
   ___XX___,
   __XX____,
   _XX_____,
   XX______,
   X_______,
   ________}  /* char '/'  */

 ,{
   __XXX___,
   _XX_XX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   ________}  /* char '0'  */

 ,{
   ___XX___,
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   _XXXXXX_,
   ________}  /* char '1'  */

 ,{
   _XXXXX__,
   XX___XX_,
   _____XX_,
   ___XXX__,
   __XX____,
   _XX__XX_,
   XXXXXXX_,
   ________}  /* char '2'  */

 ,{
   _XXXXX__,
   XX___XX_,
   _____XX_,
   __XXXX__,
   _____XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char '3'  */

 ,{
   ___XXX__,
   __XXXX__,
   _XX_XX__,
   XX__XX__,
   XXXXXXX_,
   ____XX__,
   ___XXXX_,
   ________}  /* char '4'  */

 ,{
   XXXXXXX_,
   XX______,
   XX______,
   XXXXXX__,
   _____XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char '5'  */

 ,{
   __XXX___,
   _XX_____,
   XX______,
   XXXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char '6'  */

 ,{
   XXXXXXX_,
   XX___XX_,
   ____XX__,
   ___XX___,
   __XX____,
   __XX____,
   __XX____,
   ________}  /* char '7'  */

 ,{
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char '8'  */

 ,{
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXXX_,
   _____XX_,
   ____XX__,
   _XXXX___,
   ________}  /* char '9'  */

 ,{
   ________,
   ___XX___,
   ___XX___,
   ________,
   ________,
   ___XX___,
   ___XX___,
   ________}  /* char ':'  */

 ,{
   ________,
   ___XX___,
   ___XX___,
   ________,
   ________,
   ___XX___,
   ___XX___,
   __XX____}  /* char ';'  */

 ,{
   _____XX_,
   ____XX__,
   ___XX___,
   __XX____,
   ___XX___,
   ____XX__,
   _____XX_,
   ________}  /* char '<'  */

 ,{
   ________,
   ________,
   _XXXXXX_,
   ________,
   ________,
   _XXXXXX_,
   ________,
   ________}  /* char '='  */

 ,{
   _XX_____,
   __XX____,
   ___XX___,
   ____XX__,
   ___XX___,
   __XX____,
   _XX_____,
   ________}  /* char '>'  */

 ,{
   _XXXXX__,
   XX___XX_,
   ____XX__,
   ___XX___,
   ___XX___,
   ________,
   ___XX___,
   ________}  /* char '?'  */

 ,{
   _XXXXX__,
   XX___XX_,
   XX_XXXX_,
   XX_XXXX_,
   XX_XXXX_,
   XX______,
   _XXXX___,
   ________}  /* char '@'  */

 ,{
   __XXX___,
   _XX_XX__,
   XX___XX_,
   XXXXXXX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   ________}  /* char 'A'  */

 ,{
   XXXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX__XX_,
   _XX__XX_,
   XXXXXX__,
   ________}  /* char 'B'  */

 ,{
   __XXXX__,
   _XX__XX_,
   XX______,
   XX______,
   XX______,
   _XX__XX_,
   __XXXX__,
   ________}  /* char 'C'  */

 ,{
   XXXXX___,
   _XX_XX__,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   _XX_XX__,
   XXXXX___,
   ________}  /* char 'D'  */

 ,{
   XXXXXXX_,
   _XX___X_,
   _XX_X___,
   _XXXX___,
   _XX_X___,
   _XX___X_,
   XXXXXXX_,
   ________}  /* char 'E'  */

 ,{
   XXXXXXX_,
   _XX___X_,
   _XX_X___,
   _XXXX___,
   _XX_X___,
   _XX_____,
   XXXX____,
   ________}  /* char 'F'  */

 ,{
   __XXXX__,
   _XX__XX_,
   XX______,
   XX______,
   XX__XXX_,
   _XX__XX_,
   __XXX_X_,
   ________}  /* char 'G'  */

 ,{
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XXXXXXX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   ________}  /* char 'H'  */

 ,{
   __XXXX__,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________}  /* char 'I'  */

 ,{
   ___XXXX_,
   ____XX__,
   ____XX__,
   ____XX__,
   XX__XX__,
   XX__XX__,
   _XXXX___,
   ________}  /* char 'J'  */

 ,{
   XXX__XX_,
   _XX__XX_,
   _XX_XX__,
   _XXXX___,
   _XX_XX__,
   _XX__XX_,
   XXX__XX_,
   ________}  /* char 'K'  */

 ,{
   XXXX____,
   _XX_____,
   _XX_____,
   _XX_____,
   _XX___X_,
   _XX__XX_,
   XXXXXXX_,
   ________}  /* char 'L'  */

 ,{
   XX___XX_,
   XXX_XXX_,
   XXXXXXX_,
   XXXXXXX_,
   XX_X_XX_,
   XX___XX_,
   XX___XX_,
   ________}  /* char 'M'  */

 ,{
   XX___XX_,
   XXX__XX_,
   XXXX_XX_,
   XX_XXXX_,
   XX__XXX_,
   XX___XX_,
   XX___XX_,
   ________}  /* char 'N'  */

 ,{
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char 'O'  */

 ,{
   XXXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX_____,
   _XX_____,
   XXXX____,
   ________}  /* char 'P'  */

 ,{
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX__XXX_,
   _XXXXX__,
   ____XXX_}  /* char 'Q'  */

 ,{
   XXXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX_XX__,
   _XX__XX_,
   XXX__XX_,
   ________}  /* char 'R'  */

 ,{
   __XXXX__,
   _XX__XX_,
   __XX____,
   ___XX___,
   ____XX__,
   _XX__XX_,
   __XXXX__,
   ________}  /* char 'S'  */

 ,{
   _XXXXXX_,
   _XXXXXX_,
   _X_XX_X_,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________}  /* char 'T'  */

 ,{
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char 'U'  */

 ,{
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   ________}  /* char 'V'  */

 ,{
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX_X_XX_,
   XX_X_XX_,
   XXXXXXX_,
   _XX_XX__,
   ________}  /* char 'W'  */

 ,{
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   _XX_XX__,
   XX___XX_,
   XX___XX_,
   ________}  /* char 'X'  */

 ,{
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   __XXXX__,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________}  /* char 'Y'  */

 ,{
   XXXXXXX_,
   XX___XX_,
   X___XX__,
   ___XX___,
   __XX__X_,
   _XX__XX_,
   XXXXXXX_,
   ________}  /* char 'Z'  */

 ,{
   __XXXX__,
   __XX____,
   __XX____,
   __XX____,
   __XX____,
   __XX____,
   __XXXX__,
   ________}  /* char '['  */

 ,{
   XX______,
   _XX_____,
   __XX____,
   ___XX___,
   ____XX__,
   _____XX_,
   ______X_,
   ________}  /* char '\'  */

 ,{
   __XXXX__,
   ____XX__,
   ____XX__,
   ____XX__,
   ____XX__,
   ____XX__,
   __XXXX__,
   ________}  /* char ']'  */

 ,{
   ___X____,
   __XXX___,
   _XX_XX__,
   XX___XX_,
   ________,
   ________,
   ________,
   ________}  /* char '^'  */

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   XXXXXXXX}  /* char '_'  */

 ,{
   __XX____,
   ___XX___,
   ____XX__,
   ________,
   ________,
   ________,
   ________,
   ________}  /* char '`'  */

 ,{
   ________,
   ________,
   _XXXX___,
   ____XX__,
   _XXXXX__,
   XX__XX__,
   _XXX_XX_,
   ________}  /* char 'a'  */

 ,{
   XXX_____,
   _XX_____,
   _XXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   XX_XXX__,
   ________}  /* char 'b'  */

 ,{
   ________,
   ________,
   _XXXXX__,
   XX___XX_,
   XX______,
   XX___XX_,
   _XXXXX__,
   ________}  /* char 'c'  */

 ,{
   ___XXX__,
   ____XX__,
   _XXXXX__,
   XX__XX__,
   XX__XX__,
   XX__XX__,
   _XXX_XX_,
   ________}  /* char 'd'  */

 ,{
   ________,
   ________,
   _XXXXX__,
   XX___XX_,
   XXXXXXX_,
   XX______,
   _XXXXX__,
   ________}  /* char 'e'  */

 ,{
   __XXXX__,
   _XX__XX_,
   _XX_____,
   XXXXX___,
   _XX_____,
   _XX_____,
   XXXX____,
   ________}  /* char 'f'  */

 ,{
   ________,
   ________,
   _XXX_XX_,
   XX__XX__,
   XX__XX__,
   _XXXXX__,
   ____XX__,
   XXXXX___}  /* char 'g'  */

 ,{
   XXX_____,
   _XX_____,
   _XX_XX__,
   _XXX_XX_,
   _XX__XX_,
   _XX__XX_,
   XXX__XX_,
   ________}  /* char 'h'  */

 ,{
   ___XX___,
   ________,
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________}  /* char 'i'  */

 ,{
   _____XX_,
   ________,
   _____XX_,
   _____XX_,
   _____XX_,
   _XX__XX_,
   _XX__XX_,
   __XXXX__}  /* char 'j'  */

 ,{
   XXX_____,
   _XX_____,
   _XX__XX_,
   _XX_XX__,
   _XXXX___,
   _XX_XX__,
   XXX__XX_,
   ________}  /* char 'k'  */

 ,{
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________}  /* char 'l'  */

 ,{
   ________,
   ________,
   XXX_XX__,
   XXXXXXX_,
   XX_X_XX_,
   XX_X_XX_,
   XX_X_XX_,
   ________}  /* char 'm'  */

 ,{
   ________,
   ________,
   XX_XXX__,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   ________}  /* char 'n'  */

 ,{
   ________,
   ________,
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________}  /* char 'o'  */

 ,{
   ________,
   ________,
   XX_XXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX_____,
   XXXX____}  /* char 'p'  */

 ,{
   ________,
   ________,
   _XXX_XX_,
   XX__XX__,
   XX__XX__,
   _XXXXX__,
   ____XX__,
   ___XXXX_}  /* char 'q'  */

 ,{
   ________,
   ________,
   XX_XXX__,
   _XXX_XX_,
   _XX_____,
   _XX_____,
   XXXX____,
   ________}  /* char 'r'  */

 ,{
   ________,
   ________,
   _XXXXXX_,
   XX______,
   _XXXXX__,
   _____XX_,
   XXXXXX__,
   ________}  /* char 's'  */

 ,{
   __XX____,
   __XX____,
   XXXXXX__,
   __XX____,
   __XX____,
   __XX_XX_,
   ___XXX__,
   ________}  /* char 't'  */

 ,{
   ________,
   ________,
   XX__XX__,
   XX__XX__,
   XX__XX__,
   XX__XX__,
   _XXX_XX_,
   ________}  /* char 'u'  */

 ,{
   ________,
   ________,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   ________}  /* char 'v'  */

 ,{
   ________,
   ________,
   XX___XX_,
   XX_X_XX_,
   XX_X_XX_,
   XXXXXXX_,
   _XX_XX__,
   ________}  /* char 'w'  */

 ,{
   ________,
   ________,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   _XX_XX__,
   XX___XX_,
   ________}  /* char 'x'  */

 ,{
   ________,
   ________,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXXX_,
   _____XX_,
   XXXXXX__}  /* char 'y'  */

 ,{
   ________,
   ________,
   _XXXXXX_,
   _X__XX__,
   ___XX___,
   __XX__X_,
   _XXXXXX_,
   ________}  /* char 'z'  */

 ,{
   ____XXX_,
   ___XX___,
   ___XX___,
   _XXX____,
   ___XX___,
   ___XX___,
   ____XXX_,
   ________}  /* char '{'  */

 ,{
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ________}  /* char '|'  */

 ,{
   _XXX____,
   ___XX___,
   ___XX___,
   ____XXX_,
   ___XX___,
   ___XX___,
   _XXX____,
   ________}  /* char '}'  */

 ,{
   _XXX_XX_,
   XX_XXX__,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}   /* char '~'  */



/*  additional characters for European character set  */

/* small accent ague, 95 */
 ,{
   ____XX__,
   ___X____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

 /* small accent grave, 96 */
 ,{
   _XX_____,
   ___X____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}


/* small accent circonflex, 97 */
 ,{
   _XXXXX__,
   X_____X_,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}


 /* small umlaut, 98 */
 ,{
   XX___XX_,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}


 /* small tilde, 99  */
 ,{
   _XXX__X_,
   X___XX__,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}


 /* i without dot, 100  */
 ,{
   ________,
   ________,
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________}


 /* sharps eg 