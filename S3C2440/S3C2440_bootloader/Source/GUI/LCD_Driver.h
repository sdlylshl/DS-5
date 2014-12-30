#ifndef _LCD_Driver_H_
#define _LCD_Driver_H_
#include "def.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "LCD_Config.h"

//#include "ver.h"
extern void Versions(void);



#define print WriteString

//#define Versions() print(0,0,"PCB6V3 BootMenu <V1.8>",0xFF)
extern void Lcd_TextAlignMid(void);
extern void Lcd_Update(void);
extern void Lcd_SetColor(U32 fgColor);
extern void Lcd_SetBkColor(U32 bgColor);
extern U32 Lcd_GetColor(void);
extern U32 Lcd_GetBkColor(void);
extern void Lcd_ResetPoint(void);
extern void Lcd_SetPosition(U8 x,U8 y);
extern void Lcd_NextLine(void);
extern void Lcd_ClearScr(U32 colors);
extern void Lcd_Clear(void);
extern void Lcd_Init(void);

//extern void Glib_Line(int x1,int y1,int x2,int y2, U16 color);
extern void Draw_Y_Line(U32 x,U32 y_begin,U32 y_end,U32 Fcolor);
extern void Draw_X_Line(U32 x_begin,U32 x_end,U32 y,U32 Fcolor);
extern void DrawSquare(U32 x_begin,U32 x_end,U32 y_begin,U32 y_end,U32 mode,U32 Fcolor);
extern void DrawCircle(U32 x,U32 y,U32 r,U32 mode,U32 Fcolor);
extern void DisplayImage(U32 x,U32 y,U32 iWidth,U32 iHeight,U8 bmp[]);
extern U32 WriteString(U32 x,U32 y,char *str,U32 Fcolor);
extern void LoadPic(U32 PicBuffer); //Nand.c
extern void Printf(char *fmt, ...);
extern void Printf(char *fmt, ...);

#ifdef __cplusplus
}
#endif


#endif /*__*/ 