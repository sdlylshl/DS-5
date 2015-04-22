#include "1602LCD.h"
#include "stm32f10x_lib.h"

/*-----------------------------------------------------------------------
延时函数
系统时钟：8M
-----------------------------------------------------------------------*/
void delay_1us(void)                 //1us延时函数
  {
   asm("nop");
  }

void delay_nus(unsigned int n)       //N us延时函数
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1us();
  }
  
void delay_1ms(void)                 //1ms延时函数
  {
   unsigned int i;
   for (i=0;i<1140;i++);
  }
  
void delay_nms(unsigned int n)       //N ms延时函数
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
  }  
  
/**********************************************************
光标命令
LCD_write_char(0x0e,0);    //光标开
LCD_write_char(0x0d,0);    //光标所在字符闪烁  
LCD_write_char(0x0c,0);    //光标关  
**********************************************************/
/**********************************************************
TC1602B LCD DISPLAY
建立时间：2003年11月9号
修改日期：2003年11月14号
LCD_write函数功能：当command＝0时，向LCD写入数据，否则向LCD写
                   入命令
LCD第一行显示寄存器地址：0X80-0X8F
LCD第二行显示寄存器地址：0XC0-0XCF
**********************************************************/
void LCD_init(void)
  {
	SET_EN();
	SET_RW();
	SET_RS();
	    
    delay_nms(15);
  unsigned int i=0; 
  for (i=0;i<20;i++) 
    { 
      LCD_write_char(0,0x28); //4bit test 
      LCD_en_write();
      delay_nms(15); 
    } 
  LCD_write_char(0,0x0c); //显示开 
  delay_nms(5); 
   
//    LCD_write_char(0x28,0);      //4bit test
    LCD_write_char(0x0c,0);      //显示开
    LCD_write_char(0x01,0);      //显示清屏
//    LCD_write_char(0x06,0);      //显示光标移动设置
  }

void LCD_en_write(void)        //EN端产生一个高电平脉冲，写LCD
  {
    SET_EN();
    delay_nus(1);
    CLR_EN();
  }

/*-----------------------------------------------------------------------
LCD_write_char    : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置，X:0-15，Y:0-1
                    LCD第一行显示寄存器地址：0X80-0X8F
                    LCD第一行显示寄存器地址：0XC0-0XCF

编写日期          ：2003-11-19 
最后修改日期      ：2004-8-19 		
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned command,unsigned data)
  {
    unsigned command_temp,data_temp;        
   
    command_temp = command;
    data_temp = data;
   
//    LCD_wait_Ready();
    delay_nms(160);
   
    CLR_RW();                 //RW=0
   
    if (command == 0)
      {
   	 SET_RS();             //RS=1 
   	 GPIO_Write(LCD_DATA_PORT & 0X0F);
	 GPIO_Write(LCD_DATA_PORT | data_temp&0xf0);   //send high 4bit
      }
    else 
      {
	 CLR_RS();            //RS=0
   	 GPIO_Write(LCD_DATA_PORT & 0X0F);
	 GPIO_Write(LCD_DATA_PORT | command_temp&0xf0);   //send high 4bit
      }
    LCD_en_write();
   
    command_temp=command_temp << 4;         //send low 4bit
    data_temp=data_temp       << 4;
   
    GPIO_Write(LCD_DATA_PORT & 0X0F);
    if (command==0)
      GPIO_Write(LCD_DATA_PORT | data_temp&0xf0);
    else
      GPIO_Write(LCD_DATA_PORT | command_temp&0xf0);
    LCD_en_write();
   
    SET_RW();
    
  }
/*
void LCD_wait_Ready(void)                  //等待LCD空闲
  {
    GPIO_Write(LCD_DATA_DDR & ~0x80);                 //PD7 I/O口方向设置为输入
   
    SET_RW();;                 //RW=1
    CLR_RS;                //RS=0
    SET_EN;                 //EN=1
   
    while (!( GPIO_Write(LCD_DATA_PIN&0x80 )) == 0);   //RW=1,读PD7，为0表示空闲；
   
    CLR_EN;                //EN=0
   
    GPIO_Write(LCD_DATA_DDR | 0xf0);
  }
*/
/*-----------------------------------------------------------------------
LCD_set_xy        : 设置LCD显示的起始位置

输入参数：x、y    : 显示字符串的位置，X:0-15，Y:0-1
                    LCD第一行显示寄存器地址：0X80-0X8F
                    LCD第一行显示寄存器地址：0XC0-0XCF

编写日期          ：2004-8-19 
最后修改日期      ：2004-8-19 		
-----------------------------------------------------------------------*/
void LCD_set_xy( unsigned char x, unsigned char y )
  {
    unsigned char address;
    if (y == 0) address = 0x80 + x;
    else 
       address = 0xc0 + x;
    LCD_write_char( address, 0 );
  }
/*-----------------------------------------------------------------------
LCD_write_string  : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置

编写日期          ：2004-8-19 
最后修改日期      ：2004-8-19 		
-----------------------------------------------------------------------*/
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
  {
    LCD_set_xy( X, Y );
    
    while (*s) 
      {
        LCD_write_char( 0, *s );
	s ++;
      }
  }
