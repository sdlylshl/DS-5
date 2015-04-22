#include "1602LCD.h"
#include "stm32f10x_lib.h"

/*-----------------------------------------------------------------------
��ʱ����
ϵͳʱ�ӣ�8M
-----------------------------------------------------------------------*/
void delay_1us(void)                 //1us��ʱ����
  {
   asm("nop");
  }

void delay_nus(unsigned int n)       //N us��ʱ����
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1us();
  }
  
void delay_1ms(void)                 //1ms��ʱ����
  {
   unsigned int i;
   for (i=0;i<1140;i++);
  }
  
void delay_nms(unsigned int n)       //N ms��ʱ����
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
  }  
  
/**********************************************************
�������
LCD_write_char(0x0e,0);    //��꿪
LCD_write_char(0x0d,0);    //��������ַ���˸  
LCD_write_char(0x0c,0);    //����  
**********************************************************/
/**********************************************************
TC1602B LCD DISPLAY
����ʱ�䣺2003��11��9��
�޸����ڣ�2003��11��14��
LCD_write�������ܣ���command��0ʱ����LCDд�����ݣ�������LCDд
                   ������
LCD��һ����ʾ�Ĵ�����ַ��0X80-0X8F
LCD�ڶ�����ʾ�Ĵ�����ַ��0XC0-0XCF
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
  LCD_write_char(0,0x0c); //��ʾ�� 
  delay_nms(5); 
   
//    LCD_write_char(0x28,0);      //4bit test
    LCD_write_char(0x0c,0);      //��ʾ��
    LCD_write_char(0x01,0);      //��ʾ����
//    LCD_write_char(0x06,0);      //��ʾ����ƶ�����
  }

void LCD_en_write(void)        //EN�˲���һ���ߵ�ƽ���壬дLCD
  {
    SET_EN();
    delay_nus(1);
    CLR_EN();
  }

/*-----------------------------------------------------------------------
LCD_write_char    : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ�ã�X:0-15��Y:0-1
                    LCD��һ����ʾ�Ĵ�����ַ��0X80-0X8F
                    LCD��һ����ʾ�Ĵ�����ַ��0XC0-0XCF

��д����          ��2003-11-19 
����޸�����      ��2004-8-19 		
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
void LCD_wait_Ready(void)                  //�ȴ�LCD����
  {
    GPIO_Write(LCD_DATA_DDR & ~0x80);                 //PD7 I/O�ڷ�������Ϊ����
   
    SET_RW();;                 //RW=1
    CLR_RS;                //RS=0
    SET_EN;                 //EN=1
   
    while (!( GPIO_Write(LCD_DATA_PIN&0x80 )) == 0);   //RW=1,��PD7��Ϊ0��ʾ���У�
   
    CLR_EN;                //EN=0
   
    GPIO_Write(LCD_DATA_DDR | 0xf0);
  }
*/
/*-----------------------------------------------------------------------
LCD_set_xy        : ����LCD��ʾ����ʼλ��

���������x��y    : ��ʾ�ַ�����λ�ã�X:0-15��Y:0-1
                    LCD��һ����ʾ�Ĵ�����ַ��0X80-0X8F
                    LCD��һ����ʾ�Ĵ�����ַ��0XC0-0XCF

��д����          ��2004-8-19 
����޸�����      ��2004-8-19 		
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
LCD_write_string  : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��

��д����          ��2004-8-19 
����޸�����      ��2004-8-19 		
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
