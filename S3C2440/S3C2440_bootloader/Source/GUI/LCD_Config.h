#ifndef _LCD_Config_H_
#define _LCD_Config_H_
/*
 *颜色定义
 */

//FCLK = 180MHz, HCLK = PCLK = 90MHz
//VCLK = HCLK / [(CLKVAL+1) * 2]     ( CLKVAL >= 0 )
//VCLK = 45MHz		//  34MHz < VCLK < 40MHz



//TFT 640480 液晶屏尺寸
#define LCD_XSIZE_TFT_640480 	(640)	
#define LCD_YSIZE_TFT_640480 	(480)

//TFT 640480图片尺寸
#define SCR_XSIZE_TFT_640480 	(LCD_XSIZE_TFT_640480)
#define SCR_YSIZE_TFT_640480 	(LCD_YSIZE_TFT_640480)


#define BLACK         0x000000
#define BLUE          0x0000FF
#define GREEN         0x00ff00
#define CYAN          0x00ffff
#define RED           0xFF0000
#define MAGENTA       0x8b008b
#define BROWN         0xa52a2a
#define DARKGRAY      0x404040
#define GRAY          0x808080
#define LIGHTGRAY     0xd3d3d3
#define LIGHTBLUE     0x8080ff
#define LIGHTGREEN    0x80ff80
#define LIGHTCYAN     0xff80ff
#define LIGHTRED      0xff8080
#define LIGHTMAGENTA  0xff80ff
#define YELLOW        0xffff00
#define WHITE         0xffffff



//LCD控制1寄存器(0X4D000000) rLCDCON1
#define CLKVAL	    (1)
#define MMODE   	(1)
#define PNRMODE 	(3)
#define BPPMODE 	(12)
#define ENVID	    (0)

//LCD控制2寄存器：（0X4D000004） rLCDCON2
#define VBPD		(31)						//垂直同步信号的后肩
#define LINEVAL	    (LCD_YSIZE_TFT_640480-1)	//LCD面板的垂直尺寸
#define VFPD		(10)						//垂直同步信号的前肩
#define VSPW		(1)							//垂直同步信号的脉宽

//LCD控制3寄存器：（0X4D000008）rLCDCON3
#define HBPD		(39)						//水平同步信号的后肩
#define HOZVAL		(LCD_XSIZE_TFT_640480-1)	//LCD面板的水平尺寸
#define HFPD		(23)						//水平同步信号的前肩


//LCD控制4寄存器：（0X4D00000C）rLCDCON4
#define MVAL		(13)
#define HSPW		(95)						//水平同步信号的脉宽

//LCD控制5寄存器：（0X4D000010）rLCDCON5

#define FRM565		(1)		//
#define INVVCLK		(1)		//
#define INVVLINE	(0)		//
#define INVVFRAME	(0)		//
#define PWREN		(1)		//
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control
//#define BPP24BL     (1)		//0 = LSB valid   1 = MSB Valid

#define M5D(n) ((n) & 0x1fffff)					// 获取低 21bits


//TFT640480


//以下参数只针对VGA
//# 640x400 @ 70 Hz, 31.5 kHz hsync
//Modeline "640x400" 25.175 640 664 760 800 400 409 411 450
//#define HFPD_640480		(23)		//水平同步信号的前肩
//#define HSPW_640480		(95)		//水平同步信号的脉宽
//#define HBPD_640480		(39)		//水平同步信号的后肩

//#define VFPD_640480		(10)		//垂直同步信号的前肩
//#define VSPW_640480		(1)		//垂直同步信号的脉宽
//#define VBPD_640480		(31)	//垂直同步信号的后肩

//extern unsigned char girl3_640_480[];	//宽640，高480
#define PIC_BUFFER LCD_BUFFER - 0x44	//跳过图片信息
#endif /*_LCD_Config_H_*/ 