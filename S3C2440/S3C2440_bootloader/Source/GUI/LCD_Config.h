#ifndef _LCD_Config_H_
#define _LCD_Config_H_
/*
 *��ɫ����
 */

//FCLK = 180MHz, HCLK = PCLK = 90MHz
//VCLK = HCLK / [(CLKVAL+1) * 2]     ( CLKVAL >= 0 )
//VCLK = 45MHz		//  34MHz < VCLK < 40MHz



//TFT 640480 Һ�����ߴ�
#define LCD_XSIZE_TFT_640480 	(640)	
#define LCD_YSIZE_TFT_640480 	(480)

//TFT 640480ͼƬ�ߴ�
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



//LCD����1�Ĵ���(0X4D000000) rLCDCON1
#define CLKVAL	    (1)
#define MMODE   	(1)
#define PNRMODE 	(3)
#define BPPMODE 	(12)
#define ENVID	    (0)

//LCD����2�Ĵ�������0X4D000004�� rLCDCON2
#define VBPD		(31)						//��ֱͬ���źŵĺ��
#define LINEVAL	    (LCD_YSIZE_TFT_640480-1)	//LCD���Ĵ�ֱ�ߴ�
#define VFPD		(10)						//��ֱͬ���źŵ�ǰ��
#define VSPW		(1)							//��ֱͬ���źŵ�����

//LCD����3�Ĵ�������0X4D000008��rLCDCON3
#define HBPD		(39)						//ˮƽͬ���źŵĺ��
#define HOZVAL		(LCD_XSIZE_TFT_640480-1)	//LCD����ˮƽ�ߴ�
#define HFPD		(23)						//ˮƽͬ���źŵ�ǰ��


//LCD����4�Ĵ�������0X4D00000C��rLCDCON4
#define MVAL		(13)
#define HSPW		(95)						//ˮƽͬ���źŵ�����

//LCD����5�Ĵ�������0X4D000010��rLCDCON5

#define FRM565		(1)		//
#define INVVCLK		(1)		//
#define INVVLINE	(0)		//
#define INVVFRAME	(0)		//
#define PWREN		(1)		//
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control
//#define BPP24BL     (1)		//0 = LSB valid   1 = MSB Valid

#define M5D(n) ((n) & 0x1fffff)					// ��ȡ�� 21bits


//TFT640480


//���²���ֻ���VGA
//# 640x400 @ 70 Hz, 31.5 kHz hsync
//Modeline "640x400" 25.175 640 664 760 800 400 409 411 450
//#define HFPD_640480		(23)		//ˮƽͬ���źŵ�ǰ��
//#define HSPW_640480		(95)		//ˮƽͬ���źŵ�����
//#define HBPD_640480		(39)		//ˮƽͬ���źŵĺ��

//#define VFPD_640480		(10)		//��ֱͬ���źŵ�ǰ��
//#define VSPW_640480		(1)		//��ֱͬ���źŵ�����
//#define VBPD_640480		(31)	//��ֱͬ���źŵĺ��

//extern unsigned char girl3_640_480[];	//��640����480
#define PIC_BUFFER LCD_BUFFER - 0x44	//����ͼƬ��Ϣ
#endif /*_LCD_Config_H_*/ 