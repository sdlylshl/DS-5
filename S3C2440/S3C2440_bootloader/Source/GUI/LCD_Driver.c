#include <string.h>
#include <stddef.h>           /* needed for definition of NULL */
//#include "LCD_Private.H"      /* private modul definitions & config */
//#include "GUI_Private.H"
//#include "GUIDebug.h"
//#include "LCD_0.h"            /* Defines for first display */


#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"

#include "stdarg.h"         // Va_list
#include "stdio.h"          // Vsprint  

#include "LCD_Driver.h"
#include "LCD_Config.h"

#include "ASCII8x16.h"
// unsigned  char  ASCII[10] = {};
#define TFT_WIDTH       480
#define TFT_HIGHT       640

#define FONT_WIDTH      8
#define FONT_HIGHT      16


#define FONT_PADDING    2


#define FONT_WIDTHNUM       (TFT_WIDTH/FONT_WIDTH) //max number 60 Font one Rows
#define FONT_HIGHTNUM       (TFT_HIGHT/(FONT_HIGHT+FONT_PADDING))  //Max 40 Font one Colums


// Font Position
//****************************************************************************//
//Global Variable
// define the display Color of FrontGround and BackGround
typedef struct
{
    U16 ForeColor;//F
    U16 BackColor;//BackGround Colors
} LCD_Color;

LCD_Color lcdColor;

//define Display Position
typedef struct
{
    U8 DispPosX;
    U8 DispPosY;
} FONT_Point;
FONT_Point fontPoint;

//****************************************************************************//
//
static U8 text_Align = 0;

// struct Text {
//     U32 Xsize;
//     U32 Ysize;
//     char Mode;
// } TEXT;


volatile U16 LCD_BUFFER[SCR_YSIZE_TFT_640480][SCR_XSIZE_TFT_640480];
//volatile  unsigned short LCD_BUFFER_BACK[SCR_YSIZE_TFT_640480][SCR_XSIZE_TFT_640480];
U16 LCD_MEMBUFFER[SCR_YSIZE_TFT_640480][SCR_XSIZE_TFT_640480]={0};

/*
static void S3C2440Lcd_Port_Init( void )
{
    rGPCUP = 0xffffffff; // Disable Pull-up register
    rGPCCON = 0xaaaa02a8; //Initialize VD[7:0],VM,VFRAME,VLINE,VCLK

    rGPDUP = 0xffffffff; // Disable Pull-up register
    rGPDCON = 0xaaaaaaaa; //Initialize VD[15:8]
}
*/
static void S3C2440Lcd_Init(void)
{
    rLCDCON1 = (CLKVAL << 8) | (MMODE << 7) | (PNRMODE << 5) | (BPPMODE << 1) | ENVID;
    // TFT LCD panel,16bpp TFT,ENVID=off
    rLCDCON2 = (VBPD << 24) | (LINEVAL << 14) | (VFPD << 6) | VSPW;
    rLCDCON3 = (HBPD << 19) | (HOZVAL << 8) | HFPD;
    rLCDCON4 = (MVAL << 8) | HSPW;
    rLCDCON5 = (FRM565 << 11) | (INVVCLK << 10) | (INVVLINE << 9) | (INVVFRAME << 8) | (PWREN << 3) | (BSWP << 1) | (HWSWP); //FRM5:6:5,HSYNC and VSYNC are inverted

    rLCDSADDR1 = (((U32)LCD_BUFFER >> 22) << 21) | M5D((U32)LCD_BUFFER >> 1);
    rLCDSADDR2 = M5D( ((U32)LCD_BUFFER + (SCR_XSIZE_TFT_640480 * LCD_YSIZE_TFT_640480 * 2)) >> 1 );
    rLCDSADDR3 = (((SCR_XSIZE_TFT_640480 - LCD_XSIZE_TFT_640480) / 1) << 11) | (LCD_XSIZE_TFT_640480 / 1);
    rLCDINTMSK |= (3); // MASK LCD Sub Interrupt
    rTCONSEL &= ~((1 << 4) | 1); // Disable LCC3600, LPC3600
    rTPAL = 0; // Disable Temp Palette

}


static void S3C2440Lcd_EnvidOnOff(int onoff)
{
    if (onoff == 1)
        rLCDCON1 |= 1; // ENVID=ON
    else
        rLCDCON1 = rLCDCON1 & 0x3fffe; // ENVID Off
}


void S3C2440Lcd_Off(void)
{
    S3C2440Lcd_EnvidOnOff(0);
}

void S3C2440Lcd_On(void)
{
    S3C2440Lcd_EnvidOnOff(1);
}


/**
 * [S3C2440Lcd_PowerEnable description]
 * @param invpwren []
 * @param pwren    []
 */
static void S3C2440Lcd_PowerEnable(int invpwren, int pwren)
{
	//GPB1 配置端口状态
    rGPBCON = rGPBCON & (~(3 << 2)) | (1 << 2); //LCD_PWM GPB1设置为输出
   rGPBDAT &= ~(1 << 1); //开屏
//     rGPBDAT |= (1 << 1); //关屏
    
    //GPG4 is setted as LCD_PWREN
    rGPGUP = rGPGUP | (1 << 4); // Pull-up disable
    rGPGCON = rGPGCON & (~(3 << 8)) | (1 << 8); //LCD_PWREN GPG4设置为输出

	//开屏
    #if MC216
    	rGPGDAT &= ~(1 << 4); //LCD_PWREN_ON  MC216 
    #elif GPS
    	rGPGDAT |= (1 << 4); //LCD_PWREN_ON GPS一体机
    #else	
    	rGPGDAT &= ~(1 << 4); //LCD_PWREN_ON 第五代电控系统
    #endif

    
    //Enable LCD POWER ENABLE Function
    rLCDCON5 = rLCDCON5 & (~(1 << 3)) | (pwren << 3); // PWREN
    rLCDCON5 = rLCDCON5 & (~(1 << 5)) | (invpwren << 5); // INVPWREN
}

//******************************************************************************
void Lcd_Update(void)
{

    U32 x, y ;
    for ( y = 0 ; y < SCR_YSIZE_TFT_640480 ; y++ )
    {
        for ( x = 0 ; x < SCR_XSIZE_TFT_640480 ; x++ )
        {
           // if (LCD_BUFFER[y][x] != LCD_MEMBUFFER[y][x])
            {
                LCD_BUFFER[y][x] = LCD_MEMBUFFER[y][x];
            }
        }
    }
}

//
// Same As Photoshop Colors
static U16 Lcd_ColorRGB2Color565(U32 Color)
{
    U32 r, g, b;
    b = (Color >> (0 + 3))  & 0x1f;
    g = (Color >> (8 + 2)) & 0x3f;
    r = (Color >> (16 + 3)) & 0x1f;
    return (U16)( b + (g << 5) + (r << 11));
}

// Same As Photoshop Colors
static U32 LCD_Color5652ColorRGB(U16 Index)
{
    U32 r, g, b;
    /* Seperate the color masks */
    b = Index & 0x1f;
    g = (Index >> 5) & 0x3f;
    r = ((unsigned)Index >> 11) & 0x1f;
    /* Convert the color masks */
    b = r * 255 / 31;
    g = g * 255 / 63;
    r = b * 255 / 31;
    return b + (g << 8) + (((U32)r) << 16);
}
//******************************************************************************

extern void Lcd_SetColor(U32 fgColor)
{

    lcdColor.ForeColor = Lcd_ColorRGB2Color565(fgColor);

}

extern void Lcd_SetBkColor(U32 bkColor)
{

    lcdColor.BackColor = Lcd_ColorRGB2Color565(bkColor);

}
extern U32 Lcd_GetColor(void)
{

    return lcdColor.ForeColor;

}
extern U32 Lcd_GetBkColor(void)
{

    return lcdColor.BackColor;

}

/**
 * [Lcd_SetFontPoint description]
 * @param x [ROW number]
 * @param y [Cols Position]
 */
extern void Lcd_ResetPoint(void)
{
    fontPoint.DispPosX = 0;
    fontPoint.DispPosY = 0;
}
extern void Lcd_SetPosition(U8 x, U8 y)
{
    fontPoint.DispPosX = x;
    fontPoint.DispPosY = y;
}

extern void Lcd_NextLine(void)
{
    fontPoint.DispPosX = 0;
    fontPoint.DispPosY++;
}

static void Lcd_SetPixel(U32 x, U32 y)
{
    if ( (y < SCR_XSIZE_TFT_640480) && (x < SCR_YSIZE_TFT_640480) )
    {
        LCD_MEMBUFFER[(SCR_YSIZE_TFT_640480 - x)][(y)] = lcdColor.ForeColor ;
    }

}
static void Lcd_SetPixel_Color565(U32 x, U32 y, U16 c )
{

    if ( (y < SCR_XSIZE_TFT_640480) && (x < SCR_YSIZE_TFT_640480) )
    {
        LCD_MEMBUFFER[(SCR_YSIZE_TFT_640480 - x)][(y)] = c ;
    }

}

static void Lcd_SetPixel_Color888(U32 x, U32 y, U32 c )
{
    Lcd_SetPixel_Color565(x, y, Lcd_ColorRGB2Color565(c));
}

/**
 * [S3C2440Lcd_GetPixel description]
 * @param  x [description]
 * @param  y [description]
 * @return   [description]
 */
extern U16 Lcd_GetPixelColor565(U32 x, U32 y)
{
    U16 c;
    if ( (y < SCR_XSIZE_TFT_640480) && (x < SCR_YSIZE_TFT_640480) )
    {
        c = LCD_MEMBUFFER[(SCR_YSIZE_TFT_640480 - x)][(y)];
        return (c);
    }
    return 0;
}


static void Lcd_ClearLine()
{
    U16 i, j;

    for (j = ((FONT_HIGHT + FONT_PADDING) * fontPoint.DispPosY); j < ((FONT_HIGHT + FONT_PADDING) * (1 + fontPoint.DispPosY)); j++)
    {
        for (i = 0; i < TFT_WIDTH ; i++)
        {
            LCD_MEMBUFFER[(SCR_YSIZE_TFT_640480 - i)][(j)] = lcdColor.BackColor;
        }
    }
//    Lcd_Update();
}
void Lcd_ClearScr(U32 colors)
{
    U32 x, y ;
    U16 color;
    color = Lcd_ColorRGB2Color565(colors);
    for ( y = 0 ; y < SCR_YSIZE_TFT_640480 ; y++ )
    {
        for ( x = 0 ; x < SCR_XSIZE_TFT_640480 ; x++ )
        {
            LCD_MEMBUFFER[y][x] = color ;
        }
    }
   // Lcd_Update();
}

extern void Lcd_Clear(void)
{
    unsigned int x, y ;
    for ( y = 0 ; y < SCR_YSIZE_TFT_640480 ; y++ )
    {
        for ( x = 0 ; x < SCR_XSIZE_TFT_640480 ; x++ )
        {
            LCD_MEMBUFFER[y][x] = lcdColor.BackColor ;
        }
    }
    Lcd_ResetPoint();
  //  Lcd_Update();
}

extern void Lcd_TextAlignMid(void)
{

    text_Align = 1;
}

extern void Lcd_Init(void)
{
    // S3C2440Lcd_Port_Init();
    
    Lcd_SetColor(BLUE);
    Lcd_SetBkColor(BLACK);
    Lcd_Clear();
    Lcd_Update();
    S3C2440Lcd_Init();
    S3C2440Lcd_PowerEnable(0, 1);
    S3C2440Lcd_EnvidOnOff(1);

    //Init_Timer0();

}
//******************************************************************************

void Draw_Y_Line(U32 x, U32 y_begin, U32 y_end, U32 Fcolor)
{
    U32 temp, dy, y0;

    if (y_begin > y_end)
    {
        temp = y_begin;
        y_begin = y_end;
        y_end = temp;
    }
    dy = y_end - y_begin;
    for (y0 = 0; y0 < dy; y0++)
        //      TFT_BUFFER[y_begin+y0][x] = Fcolor;
        Lcd_SetPixel_Color888(x, y_begin + y0, Fcolor );
  
   //Lcd_Update();
}

void Draw_X_Line(U32 x_begin, U32 x_end, U32 y, U32 Fcolor)
{
    U32 temp, dx, x0;
    if (x_begin > x_end)
    {
        temp = x_begin;
        x_begin = x_end;
        x_end = temp;
    }
    dx = x_end - x_begin;
    for (x0 = 0; x0 < dx; x0++)
        //TFT_BUFFER[y][x_begin+x0] = Fcolor;
        Lcd_SetPixel_Color888(x_begin + x0, y, Fcolor);
   //  Lcd_Update();
}


void DrawSquare(U32 x_begin, U32 x_end, U32 y_begin, U32 y_end, U32 mode, U32 Fcolor)
{
    U32 temp;
    U32 x0, y0;
    if (x_begin > x_end)
    {
        temp = x_begin;
        x_begin = x_end;
        x_end = temp;
    }
    if (y_begin > y_end)
    {
        temp = y_begin;
        y_begin = y_end;
        y_end = temp;
    }
    Draw_X_Line(x_begin, x_end, y_begin, Fcolor);
    Draw_X_Line(x_begin, x_end, y_end, Fcolor);
    Draw_Y_Line(x_begin, y_begin, y_end, Fcolor);
    Draw_Y_Line(x_end, y_begin, y_end, Fcolor);
    if (mode == 1)
    {
        for (y0 = y_begin; y0 < y_end; y0++)
        {
            for (x0 = x_begin; x0 < x_end; x0++)
            {
                //  TFT_BUFFER[y0][x0] = Fcolor;
                Lcd_SetPixel_Color888(x0, y0, Fcolor );
            }
        }
    }
    
   //  Lcd_Update();
}


void DrawCircle(U32 x, U32 y, U32 r, U32 mode, U32 Fcolor)
{
    U32 dx, dy, tempx, tempy, x0, y0;
    dx = TFT_WIDTH - r;
    dy = TFT_HIGHT - r;
    for (y0 = 0; y0 < TFT_HIGHT; y0++)
    {
        for (x0 = 0; x0 < TFT_WIDTH; x0++)
        {
            if (x0 < dx && y0 < dy)
            {
                tempx = x - x0;
                tempy = y - y0;
            }
            else if (x0 < dx && y0 > dy)
            {
                tempx = x - x0;
                tempy = 0 - dy;
            }
            else if (x0 > dx && y0 < dy)
            {
                tempx = x0 - x;
                tempy = y - y0;
            }
            else
            {
                tempx = x0 - x;
                tempy = y0 - y;
            }
            if (mode == 1)
            {
                if ((tempx * tempx + tempy * tempy) <= r * r)
                    //TFT_BUFFER[y0][x0] = Fcolor;
                    Lcd_SetPixel_Color888(x0, y0, Fcolor );
            }
            else
            {
                if (((tempx * tempx + tempy * tempy) == r * r) || ((tempx * tempx + tempy * tempy) == r * r - 1) || ((tempx * tempx + tempy * tempy) == r * r + 1))
                    //TFT_BUFFER[y0][x0] = Fcolor;
                    Lcd_SetPixel_Color888(x0, y0, Fcolor );
            }

        }
    }
 //Lcd_Update();

}

//******************************************************************************

void DisplayImage(U32 x, U32 y, U32 iWidth, U32 iHeight, U8 bmp[])
{
    U32 p, x0, y0;
    U32 c;
    for (y0 = 0, p = 0; y0 < iHeight; y0++)
    {

        for (x0 = 0; x0 < iWidth; x0++)
        {
            c = (bmp[p + 1]) | (bmp[p] << 8);
            if (((x + x0) < TFT_WIDTH) && ((y + y0) < TFT_HIGHT))
                //TFT_BUFFER[y0+y][x0+x] = c;
                Lcd_SetPixel_Color888(x + x0, y + y0, c );
            p = p + 2;
        }
    }
   //  Lcd_Update();
}

//******************************************************************************

static void Write_8x16_ASCII(U32 x, U32 y, const U8 ch[], U32 Fcolor)
{
    U16 i, j;
    U8 buffer, mask;
    for (i = 0; i < 16; i++)
    {
        mask = 0x80;
        buffer = ch[i];
        for (j = 0; j < 8; j++)
        {
            if (buffer & mask)
                Lcd_SetPixel_Color888(x + j, y + i, Fcolor);
            mask = mask >> 1;
        }
    }
}

void Write_16x16_Hanzi(U32 x, U32 y, const U8 ch[], U32 Fcolor)
{
    U16 i, j;
    U8 buffer, mask;

    for (i = 0; i < 16; i++)
    {
        mask = 0x80;
        buffer = ch[2 * i];
        for (j = 0; j < 8; j++)
        {
            if (buffer & mask)
                Lcd_SetPixel_Color888(x + j, y + i, Fcolor);
            mask = mask >> 1;
        }

        mask = 0x80;
        buffer = ch[2 * i + 1];
        for (j = 0; j < 8; j++)
        {
            if (buffer & mask)
                Lcd_SetPixel_Color888(x + j + 8, y + i, Fcolor);
            mask = mask >> 1;
        }
    }

}


void WriteChar(U32 x, U32 y, U8 ch, U32 Fcolor)
{
    const U8 *mould;
    mould = &ASCII[ch * 16];
    Write_8x16_ASCII(x, y, mould, Fcolor);
}
U32 WriteString(U32 x, U32 y, char *str, U32 Fcolor)
{
    U32 i = 0;
    while (*str)
    {
        WriteChar(x * 8 + i * 8, y * 16, *str++, Fcolor);
        i++;
    }
    return i;
}
//******************************************************************************

/**
 * [WriteOneChar ]
 * @param x  [description]
 * @param y  [description]
 * @param ch [description]
 */
void Write_ASCII_Char(U32 x, U32 y, U8 ch)
{
    U32 i, j;
    U8 buffer;
    U8 mask;
    for (i = 0; i < 16; i++)
    {
        buffer = ASCII[ch * 16 + i];
        mask = 0x80;
        for (j = 0; j < 8; j++)
        {

            if (buffer & mask)
            {
                Lcd_SetPixel(x + j + 8, y + i);
            }
            mask = mask >> 1;
        }
    }
}

static U8 Write_ASCII_String(char *str)
{
    U32 i = 0;
    // U16 ColorsBak;
    // if (lcdColor.BackColor!=Fcolor)
    // {
    //     ColorsBak = lcdColor.ForeColor;
    //     Lcd_SetColor(Fcolor);
    // }
    if (!fontPoint.DispPosX)
    {
        if (text_Align)
        {
            char *s = str;
            while (*s++)
            {
                if (*s == '\n')break;
                i++ ;
            }

            fontPoint.DispPosX = (U8)((FONT_WIDTHNUM - i) / 2);
        }

    }



    while (*str)
    {
        // if ((PosX<FONT_WIDTHNUM)&&(PoxY<FONT_HIGHTNUM))

        if (*str == '\n')
        {
            fontPoint.DispPosY++;
            fontPoint.DispPosX = 0;
            str++;
        }
        else
        {
            if (fontPoint.DispPosY > FONT_HIGHTNUM - 1) //
            {
                Lcd_Clear();
                //Lcd_ResetPoint();
                // Lcd_ClearLine();
            }

            if (fontPoint.DispPosX < FONT_WIDTHNUM - 1)
            {

                if (fontPoint.DispPosX == 0)
                {
                    Lcd_ClearLine();
                }
                Write_ASCII_Char(fontPoint.DispPosX * FONT_WIDTH, (fontPoint.DispPosY * (FONT_HIGHT + FONT_PADDING)), *str++);
                fontPoint.DispPosX++;

            }
            else
            {

                fontPoint.DispPosY++;
                fontPoint.DispPosX = 0;
                if (fontPoint.DispPosY > FONT_HIGHTNUM - 1) //
                {
                    //Lcd_ResetPoint();

                }


            }
        }

    }
    if (text_Align)
    {
        text_Align = 0;
    }
    //Lcd_Update();
    return 0;
}

void Printf(char *fmt, ...)
{
    va_list ap;
    char string[256];

    va_start(ap, fmt);
    vsprintf(string, fmt, ap);
    Write_ASCII_String(string);
    Uart_Printf(string);
    va_end(ap);
}

void Lcd_Test()
{
    // U32 loops;

    Lcd_Init();
    // Draw_Y_Line(200,30,300,0xFFFF);
    // Delay(0x50000);
    // Lcd_ClearScr(0xFF);
    // Delay(0x50000);
    // Lcd_ClearScr(YELLOW);
    //
    Versions();

    Lcd_TextAlignMid();
    Printf("help\n");
    Lcd_TextAlignMid();

    //Printf("BootLoader Menu Versions %d.%d\n", version / 10, version % 10);
    //Printf("BootLoader Menu Biuld time: %s %s\n", __DATE__ , __TIME__);
    // for (loops = 0; loops < 100; loops++)
    //{
    //         Lcd_NextLine();
    //         Printf("help%x\n",lcdColor.ForeColor);
    //         Printf("qwertyuioasdfghjkl;zxcvbnm,1234567890\n\n");
    //         Delay(50000);
    //    // }


    //     //Lcd_ClearLine();

    //     Printf("iHeightfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosXfontPoint.DispPosX");
    // //
    //    Printf("my Name is %d",0x58);
    while (1);
}