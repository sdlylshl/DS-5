/**************************************************************
The initial and control for 320��240 16Bpp TFT LCD----LCD_LTV350QV_F05
**************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h> 

#include "def1.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 
#include "LCD_LTV350QV_F05.h"
#include "os_cpu.h"
#include "draw.h"




#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//TFT 320240
#define LCD_XSIZE_TFT_320240 	(320)
#define LCD_YSIZE_TFT_320240 	(240)

//TFT 320240

#define SCR_XSIZE_TFT_320240 	(320)
#define SCR_YSIZE_TFT_320240 	(240)

//TFT320240
#define HOZVAL_TFT_320240	(LCD_XSIZE_TFT_320240-1)//�ֱ���
#define LINEVAL_TFT_320240	(LCD_YSIZE_TFT_320240-1)

//Timing parameter for LCD LQ035Q7DB02
#define VBPD_320240		(3)		//��ֱͬ���źŵĺ��
#define VFPD_320240		(5)		//��ֱͬ���źŵ�ǰ��
#define VSPW_320240		(15)	//��ֱͬ���źŵ�����

#define HBPD_320240		(5)		//ˮƽͬ���źŵĺ��
#define HFPD_320240		(15)	//ˮƽͬ���źŵ�ǰ��
#define HSPW_320240		(8)		//ˮƽͬ���źŵ�����

#define CLKVAL_TFT_320240	(10) //	
//FCLK=180MHz,HCLK=90MHz,VCLK=6.5MHz

extern unsigned char __CHS[];
extern unsigned char __VGA[];

volatile static unsigned short LCD_BUFFER[SCR_YSIZE_TFT_320240][SCR_XSIZE_TFT_320240];


extern U16  const  COLORS_TAB[];    

//extern void Uart_Printf(char *f, ...) ;
extern void  CLineDemo(void);
extern void  CRectangleDemo(void);
extern void HzDisp_tst(void);






/**************************************************************
320��240 16Bpp TFT LCD����ģ���ʼ��
**************************************************************/
static void Lcd_Init(void)
{  
   
  rGPCUP=0xffffffff; // Disable Pull-up register
  rGPCCON=0xaaaa56a9; //Initialize VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 

  rGPDUP=0xffffffff; // Disable Pull-up register
  rGPDCON=0xaaaaaaaa; //Initialize VD[15:8]

	rLCDCON1=(CLKVAL_TFT_320240<<8)|(MVAL_USED<<7)|(3<<5)|(12<<1)|0;
    	// TFT LCD panel,12bpp TFT,ENVID=off
	rLCDCON2=(VBPD_320240<<24)|(LINEVAL_TFT_320240<<14)|(VFPD_320240<<6)|(VSPW_320240);
	rLCDCON3=(HBPD_320240<<19)|(HOZVAL_TFT_320240<<8)|(HFPD_320240);
	rLCDCON4=(MVAL<<8)|(HSPW_320240);
	rLCDCON5=(1<<11)|(1<<9)|(1<<8)|(1<<3)|(BSWP<<1)|(HWSWP);  	

	rLCDSADDR1=(((U32)LCD_BUFFER>>22)<<21)|M5D((U32)LCD_BUFFER>>1);
	rLCDSADDR2=M5D( ((U32)LCD_BUFFER+(SCR_XSIZE_TFT_320240*LCD_YSIZE_TFT_320240*2))>>1 );
	rLCDSADDR3=(((SCR_XSIZE_TFT_320240-LCD_XSIZE_TFT_320240)/1)<<11)|(LCD_XSIZE_TFT_320240/1);
	rLCDINTMSK|=(3); // MASK LCD Sub Interrupt
   
	rTPAL=0; // Disable Temp Palette	
	
	
}

/**************************************************************
LCD��Ƶ�Ϳ����ź��������ֹͣ��1������Ƶ���
**************************************************************/
void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	rLCDCON1|=1; // ENVID=ON
    else
	rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}

/**************************************************************
320��240 8Bpp TFT LCD ��Դ��������ʹ��
**************************************************************/
static void Lcd_PowerEnable(int invpwren,int pwren)
{
    //GPG4 is setted as LCD_PWREN
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4); // Pull-up disable
    rGPGCON=rGPGCON&(~(3<<8))|(3<<8); //GPG4=LCD_PWREN
    //Enable LCD POWER ENABLE Function
    rLCDCON5=rLCDCON5&(~(1<<3))|(pwren<<3);   // PWREN
    rLCDCON5=rLCDCON5&(~(1<<5))|(invpwren<<5);   // INVPWREN
} 


/**************************************************************
320��240 16Bpp TFT LCD�������ص���ʾ�������
**************************************************************/
void PutPixel(U32 x,U32 y, U32 c )
{   
   
	if ( (x < SCR_XSIZE_TFT_320240) && (y < SCR_YSIZE_TFT_320240) )	   
	    
		LCD_BUFFER[(y)][(x)] = c;		
   
}


U32 GetPixel(U32 x,U32 y)
{
    U32 c=0;
    
	if ( (x < SCR_XSIZE_TFT_320240) && (y < SCR_YSIZE_TFT_320240) )

    c=LCD_BUFFER[(y)][(x)];
	return  c;   //�Ե��������Ǻǣ��򵥰ɣ�

}


U32 GetPixelVal(U32 x,U32 y)
{
    U32 c=0;
    
    if ( (x < SCR_XSIZE_TFT_320240) && (y < SCR_YSIZE_TFT_320240) )

	   c=LCD_BUFFER[(y)][(x)];   //�Ե��������Ǻǣ��򵥰ɣ�
	   
	return c;

}


void LCD2440_SetPixel(U32 x,U32 y,U32 c)

{
   PutPixel(x,y,c);

}

U32 LCD2440_GetPixeIndexl(U32 x,U32 y,U32 c)

{
   c= GetPixel(x,y);
   return c;

}

/**************************************************************
320��240 16Bpp TFT LCDȫ������ض���ɫ��Ԫ������
**************************************************************/
static void Lcd_ClearScr( U32 c)
{
	unsigned int x,y ;
		
    for( y = 0 ; y < SCR_YSIZE_TFT_320240 ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE_TFT_320240 ; x++ )
    	{
			LCD_BUFFER[y][x] = c ;
    	}
    }
}

/**************************************************************
LCD��Ļ��ʾ��ֱ��ת
// LCD display is flipped vertically
// But, think the algorithm by mathematics point.
//   3I2
//   4 I 1
//  --+--   <-8 octants  mathematical cordinate
//   5 I 8
//   6I7
**************************************************************/
void Glib_Line(U32 x1,U32 y1,U32 x2,U32 y2,U16 color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
	
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
  
}



/**************************************************************
��LCD��Ļ�ϻ�һ������
**************************************************************/
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color)
{
    Glib_Line(x1,y1,x2,y1,color);
    Glib_Line(x2,y1,x2,y2,color);
    Glib_Line(x1,y2,x2,y2,color);
    Glib_Line(x1,y1,x1,y2,color);
}

/**************************************************************
��LCD��Ļ������ɫ���һ������
**************************************************************/
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color)
{
    int i;

    for(i=y1;i<=y2;i++)
	Glib_Line(x1,i,x2,i,color);
}

/**************************************************************
��LCD��Ļ��ָ������㻭һ��ָ����С��ͼƬ
**************************************************************/
void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[])
{
	int x,y;
	U32 c;
	int p = 0;
	
    for( y = y0 ; y < l ; y++ )
    {
    	for( x = x0 ; x < h ; x++ )
    	{
    		c = bmp[p+1] | (bmp[p]<<8) ;

			if ( ( (x0+x) < SCR_XSIZE_TFT_320240) && ( (y0+y) < SCR_YSIZE_TFT_320240) )
				LCD_BUFFER[y0+y][x0+x] = c ;
			
    		p = p + 2 ;
    	}
    }
}

void PaintShort_Bmp(int x0,int y0,int h,int l,unsigned short bmp[])
{
	int x,y;
	U32 c;
	int p = 0;
	
    for( y = y0 ; y < l ; y++ )
    {
    	for( x = x0 ; x < h ; x++ )
    	{
    		c = bmp[p];

			if ( ( (x0+x) < SCR_XSIZE_TFT_320240) && ( (y0+y) < SCR_YSIZE_TFT_320240) )
				LCD_BUFFER[y0+y][x0+x] = c ;
			
    		 p ++ ;
    	}
    }
}

/**************************************************************
**************************************************************/
void Lcd_Tft_LTV350QV_F05_Init(void)
{
   Lcd_Init();
	 LcdBkLtSet( 70 ) ;
	 Lcd_PowerEnable(0, 1);
   Lcd_EnvidOnOff(1);		//turn on vedio

	 Lcd_ClearScr(0xffff);		//fill all screen with some color			 
	
}


void Lcd_LTV350QV_F05_Init(void)
{
   Lcd_Init();
	 LcdBkLtSet( 70 ) ;
	 Lcd_PowerEnable(0, 1);
   Lcd_EnvidOnOff(1);		//turn on vedio	
}  





/****************************************************************************
* ���ƣ�Glib_Circle()
* ���ܣ�ָ��Բ��λ�ü��뾶����Բ��
* ��ڲ����� x0		Բ�ĵ�x����ֵ
*           y0		Բ�ĵ�y����ֵ
*           r       Բ�İ뾶
*           color	��ʾ��ɫ
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
****************************************************************************/
void  Glib_Circle(U32 x0, U32 y0, U32 r, TCOLOR color)
{  S32  draw_x0, draw_y0;			// ��ͼ���������
   S32  draw_x1, draw_y1;	
   S32  draw_x2, draw_y2;	
   S32  draw_x3, draw_y3;	
   S32  draw_x4, draw_y4;	
   S32  draw_x5, draw_y5;	
   S32  draw_x6, draw_y6;	
   S32  draw_x7, draw_y7;	
   S32  xx, yy;					// ��Բ���Ʊ���
 
   S32  di;						// ���߱���
   
   /* �������� */
   if(0==r) return;
   
   /* �����8�������(0��45��90��135��180��225��270��)��������ʾ */
   draw_x0 = draw_x1 = x0;
   draw_y0 = draw_y1 = y0 + r;
   if(draw_y0<GUI_LCM_YMAX) PutPixel(draw_x0, draw_y0, color);	// 90��
	
   draw_x2 = draw_x3 = x0;
   draw_y2 = draw_y3 = y0 - r;
   if(draw_y2>=0) PutPixel(draw_x2, draw_y2, color);			// 270��
   
	
   draw_x4 = draw_x6 = x0 + r;
   draw_y4 = draw_y6 = y0;
   if(draw_x4<GUI_LCM_XMAX) PutPixel(draw_x4, draw_y4, color);	// 0��
   
   draw_x5 = draw_x7 = x0 - r;
   draw_y5 = draw_y7 = y0;
   if(draw_x5>=0) PutPixel(draw_x5, draw_y5, color);			// 180��   
   if(1==r) return;					// ���뾶Ϊ1������Բ����
   
   
   /* ʹ��Bresenham�����л�Բ */
   di = 3 - 2*r;					// ��ʼ�����߱���
   
   xx = 0;
   yy = r;	
   while(xx<yy)
   {  if(di<0)
	  {  di += 4*xx + 6;	      
	  }
	  else
	  {  di += 4*(xx - yy) + 10;
	  
	     yy--;	  
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
		 draw_x4--;
		 draw_x5++;
		 draw_x6--;
		 draw_x7++;	 	
	  }
	  
	  xx++;   
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
	  draw_y4++;
	  draw_y5++;
	  draw_y6--;
	  draw_y7--;
		
	
	  /* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
	  if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )	
	  {  PutPixel(draw_x0, draw_y0, color);
	  }	    
	  if( (draw_x1>=0)&&(draw_y1>=0) )	
	  {  PutPixel(draw_x1, draw_y1, color);
	  }
	  if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )	
	  {  PutPixel(draw_x2, draw_y2, color);   
	  }
	  if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )	
	  {  PutPixel(draw_x3, draw_y3, color);
	  }
	  if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )	
	  {  PutPixel(draw_x4, draw_y4, color);
	  }
	  if( (draw_x5>=0)&&(draw_y5>=0) )	
	  {  PutPixel(draw_x5, draw_y5, color);
	  }
	  if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )	
	  {  PutPixel(draw_x6, draw_y6, color);
	  }
	  if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )	
	  {  PutPixel(draw_x7, draw_y7, color);
	  }
   }
}




unsigned short Bmp[SCR_XSIZE_TFT_320240*SCR_YSIZE_TFT_320240/2];

void WriteLcdMems( U32 off, U32 c ) 
{
  *((U16*) (Bmp+(((U32)(off)))))=c;

}


U32 ReadLcdMems( U32 off ) 
{
  return (*((U16*) (Bmp+(((U32)(off))))));

}


/**************************************************************
��LCD��Ļ��ָ�������дASCII��
**************************************************************/
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;
	
	

	pZK = &__VGA[ch*16];
	for( i = 0 ; i < 16 ; i++ )
	{
		mask = 0x80;
		buf = pZK[i];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }
            
            mask = mask >> 1;
        }
	}
}


void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...)
{
    char __LCD_Printf_Buf[256];
    va_list ap;
	unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;

    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
	 
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n' :
				{
			
                    break;
				}

			default:
				{
					if( *pStr > 0xA0 & *(pStr+1) > 0xA0 )  //�������
                    {
                        Lcd_PutHZ( x , y , (*pStr - 0xA0)*0x0100 + *(pStr+1) - 0xA0 , c , bk_c , st);

                        pStr++;
                        i++;

                        x += 16;
                    }else               //Ӣ�����
                    {
                        Lcd_PutASCII( x , y , *pStr , c , bk_c , st );

                        x += 8;

                    }

                    break;
				}
		}
		
		pStr++;
        i++;		

        if( i > 256 ) break;
	}
   
}


/**************************************************************
��LCD��Ļ��ָ�������д����
**************************************************************/
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;

	pZK = &__CHS[ (  ( (QW >> 8) - 1 )*94 + (QW & 0x00FF)- 1 )*32 ];
	for( i = 0 ; i < 16 ; i++ )
	{
		//��
		mask = 0x80;
        buf = pZK[i*2];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        } 
        
		//��
		mask = 0x80;
        buf = pZK[i*2 + 1];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j + 8,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j + 8,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        }                 
	}
}







