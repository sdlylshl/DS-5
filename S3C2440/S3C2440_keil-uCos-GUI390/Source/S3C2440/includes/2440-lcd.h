/**************************************************************
The initial and control for 240×320 16Bpp TFT LCD----LtS350Q1
**************************************************************/

void Lcd_N35_Init(void);

extern unsigned char iphone_pic[];

void Lcd_ClearScr( unsigned short c);
void PutPixel(unsigned int x,unsigned int y, unsigned short c );
void GUI_Point(unsigned int x,unsigned int y, unsigned short c );
//void PutPixel(unsigned int x , unsigned int y , unsigned int c);
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_Rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_FilledRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_Line(int x1,int y1,int x2,int y2,int color);
//void Glib_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[]);

//================================
// 字体输出
//================================
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st); 
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);  
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...); 



