
#include "lcd1602a.h"
#include "../System/Delay/systick.h"

//LCD1602 的命令状态
//数据
#define Set_RS() GPIO_SetBits(LCD_1602A_PORT,LCD_1602A_RS);
//指令
#define Reset_RS() GPIO_ResetBits(LCD_1602A_PORT,LCD_1602A_RS);
// 读
#define Set_RW() GPIO_SetBits(LCD_1602A_PORT,LCD_1602A_RW);
// 写
#define Reset_RW() GPIO_ResetBits(LCD_1602A_PORT,LCD_1602A_RW);
// 使能
#define Set_EN() GPIO_SetBits(LCD_1602A_PORT,LCD_1602A_E);
// 失能
#define Reset_EN() GPIO_ResetBits(LCD_1602A_PORT,LCD_1602A_E);

//14ns
void LCD1602_Delay(__IO uint32_t t)
{
    while(t--);
}

static void LCD1602_GPIO_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LCD1602_RCC_PORT, ENABLE);
    //1602A
    GPIO_InitStructure.GPIO_Pin	= LCD_1602A_RS | LCD_1602A_RW | LCD_1602A_E ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_Init(LCD_1602A_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LCD_1602A_PORT, LCD_1602A_RS);

//    GPIO_InitStructure.GPIO_Pin	= LCD_1602A_LCD ;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//    GPIO_Init(LCD_1602A_PORT, &GPIO_InitStructure);
//    GPIO_SetBits(LCD_1602A_PORT, LCD_1602A_LCD);

    GPIO_InitStructure.GPIO_Pin	= LCD_1602A_D0 | LCD_1602A_D1 | LCD_1602A_D2 | LCD_1602A_D3 |LCD_1602A_D4 | LCD_1602A_D5 |LCD_1602A_D6 |LCD_1602A_D7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_Init(LCD_1602A_PORT, &GPIO_InitStructure);
}

static void LCD1602_Busy_Wait(void)
{
	
		GPIO_InitTypeDef GPIO_InitStructure;
		//设置为输入
    GPIO_InitStructure.GPIO_Pin=LCD_1602A_D0 | LCD_1602A_D1 | LCD_1602A_D2 | LCD_1602A_D3 |LCD_1602A_D4 | LCD_1602A_D5 |LCD_1602A_D6 |LCD_1602A_D7;;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(LCD_1602A_PORT,&GPIO_InitStructure);
	
    Reset_RS();
    Set_RW();
    Reset_EN();
    //LCD1602_Delay(5);
    Set_EN();
    // LCD1602_Delay(25);
    while(GPIO_ReadInputDataBit(LCD_1602A_PORT,LCD_1602A_D7)==1);
    Reset_EN();
		//恢复为输出
    GPIO_InitStructure.GPIO_Pin=LCD_1602A_D0 | LCD_1602A_D1 | LCD_1602A_D2 | LCD_1602A_D3 |LCD_1602A_D4 | LCD_1602A_D5 |LCD_1602A_D6 |LCD_1602A_D7;;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LCD_1602A_PORT,&GPIO_InitStructure);
}

static void LCD1602_Write_Cmd(uint8_t Cmd)
{
    LCD1602_Busy_Wait();
    Reset_RS();		//RS = 0
	
    Reset_RW();		//RW = 0
    Reset_EN();
		Delay_us(1);  //Tas >140ns
    Set_EN();
    //GPIO_Write(LCD_1602A_PORT,(0xff00&(Cmd<<8)));

    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D7,(BitAction)((Cmd&0x80)>>7));//D7
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D6,(BitAction)((Cmd&0x40)>>6));//D6
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D5,(BitAction)((Cmd&0x20)>>5));//D5
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D4,(BitAction)((Cmd&0x10)>>4));//D4
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D3,(BitAction)((Cmd&0x08)>>3));//D3
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D2,(BitAction)((Cmd&0x04)>>2));//D2
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D1,(BitAction)((Cmd&0x02)>>1));//D1
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D0,(BitAction)((Cmd&0x01)));	 //D0
		
    Delay_us(1);   //Pweh > 450ns
    Reset_EN();
}

static void LCD1602_Write_Data(uint8_t Data)
{
    LCD1602_Busy_Wait();
    Set_RS();				//RS = 1
	
    Reset_RW(); 		//RW = 0
    Reset_EN();		
		Delay_us(1);  //Tas >140ns
		Set_EN();
    //GPIO_Write(GPIOD,(0xff00&(Data<<8)));
		
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D7,(BitAction)((Data&0x80)>>7));//D7
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D6,(BitAction)((Data&0x40)>>6)); //D6
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D5,(BitAction)((Data&0x20)>>5)); //D5
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D4,(BitAction)((Data&0x10)>>4)); //D4
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D3,(BitAction)((Data&0x08)>>3)); //D3
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D2,(BitAction)((Data&0x04)>>2)); //D2
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D1,(BitAction)((Data&0x02)>>1)); //D1
    GPIO_WriteBit(LCD_1602A_PORT,LCD_1602A_D0,(BitAction)((Data&0x01))); //D0
		
		Delay_us(1);   //Pweh > 450ns
    Reset_EN();
}

void LCD1602_SetXY(unsigned char x,unsigned char y)   //字符初始位置设定，x表示列，y表示行
{
    unsigned char addr;

    if(y) {
        addr=0xC0+x;
    } else {
        addr=0x80+x;
    }

    LCD1602_Write_Cmd(addr) ;
}

void LCD1602_Write_String(uint8_t x,uint8_t y,char* string)
{

    LCD1602_SetXY(x,y);
    while(*string) {
        LCD1602_Write_Data(*string);
        string++;
    }
}

/*
延时15mS
写指令38H（不检测忙信号）
延时5mS
以后每次写指令、读/写数据操作均需要检测忙信号
写指令38H： 显示模式设置
写指令08H： 显示关闭
写指令01H： 显示清屏
写指令06H： 显示光标移动设置
写指令 0CH：显示开及光标设置
*/
void LCD1602_Init(void)
{
    LCD1602_GPIO_Init();
    Delay_ms(10);
    LCD1602_Write_Cmd(0x38);
    Delay_ms(5);
    LCD1602_Write_Cmd(0x38);
    LCD1602_Write_Cmd(0x08);
    LCD1602_Write_Cmd(0x01);
    LCD1602_Write_Cmd(0x06);
    LCD1602_Write_Cmd(0x0C);

    LCD1602_Write_String(5,0,"e-Lins");
}
