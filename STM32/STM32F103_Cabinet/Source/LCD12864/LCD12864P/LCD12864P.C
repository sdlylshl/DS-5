#include "LCD12864P.h"

u8 LCD12864_XY = 0;

/************************************************************************************************
 @f_name: void LCD12864_InitPort(void)
 @brief:	 初始化硬件端口配置
 @param:	 None
 @return: None
 ************************************************************************************************/

//#define  LCD12864_GPIO_DAT      GPIOE
////#define  LCD12864_GPIO_CMD  GPIOE
//#define  GPIOCLK            RCC_APB2Periph_GPIOC
//#define  GPIOCMD            RCC_APB2Periph_GPIOE
//#define  RS		     GPIO_Pin_6  
//#define  RS_PORT	 GPIOE  
//#define  RW		     GPIO_Pin_5  //E3
//#define  RW_PORT	 GPIOE  
//#define  EN		     GPIO_Pin_4  //E4
//#define  EN_PORT	 GPIOE  
//#define  PSB	     GPIO_Pin_3  //E5
//#define  PSB_PORT	 GPIOE  
//#define  RST	     GPIO_Pin_2  //E1
//#define  RST_PORT	 GPIOE  
//#define  Busy      GPIO_Pin_15 
//#define  Busy_PORT GPIOE 
//#define  LCD_GPIO_DAT  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 //PC
////#define  LCD_GPIO_CMD	 GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 //PE
//#define    LCD_RS(x)   x ? GPIO_SetBits(RS_PORT,RS):  GPIO_ResetBits(RS_PORT,RS)
//#define    LCD_RW(x)   x ? GPIO_SetBits(RW_PORT,RW):  GPIO_ResetBits(RW_PORT,RW)
//#define    LCD_EN(x)   x ? GPIO_SetBits(EN_PORT,EN):  GPIO_ResetBits(EN_PORT,EN)
//#define    LCD_PSB(x)  x ? GPIO_SetBits(PSB_PORT,PSB): GPIO_ResetBits(PSB_PORT,PSB)
//#define    LCD_RST(x)  x ? GPIO_SetBits(RST_PORT,RST): GPIO_ResetBits(RST_PORT,RST)
////数据脚充当BUSY
//#define  Text_Busy	         GPIO_ReadInputDataBit(Busy_PORT,Busy)  //PCin(7)	  
//#define  LCD_WriteData(x)   {GPIOE->BSRR = x<<8 & 0xff00;GPIOE->BRR = ((~x)<<8) & 0xff00;}  //高8位的数据
////#define  LCD_WriteData(x) 		GPIO_Write(LCD12864_GPIO_DAT ,(x<<8))
////#define  LCD_WriteData(x)   {LCD12864_GPIO_DAT->BSRR = 	(((unsigned int)x<<8) | ((unsigned int)(~x)<<24)) ;GPIOE->BRR = ((~x)<<8) & 0xff00;}  //高8位的数据
/************************************************************************************************
 @f_name: void NOP(void)
 @brief:	 延时函数
 @param:	 None
 @return: None
 ************************************************************************************************/
void NOP(void) {
	u8 i;
	for (i = 0; i < 100; i++)
		;
}
/************************************************************************************************
 @f_name: u8 LCD12864_busy(void)
 @brief:	 检测忙状态
 @param:	 None
 @return: None
 ************************************************************************************************/
u8 LCD12864_busy(void) {
	u8 x;
	LCD_EN(0);
	LCD_RS(0);
	LCD_RW(1);
	LCD_EN(1);
	NOP();
	NOP();
	x = Text_Busy;
	LCD_EN(0);

	return x; //x
	//return 0; //x
}
/************************************************************************************************
 @f_name: void LCD12684_Wcmd(u8 dat)
 @brief:	 写指令
 @param:	 u8 dat  输入指令
 @return: None
 ************************************************************************************************/
void LCD12684_Wcmd(u8 dat) {
	while (LCD12864_busy())
		; //忙检测
	LCD_RS(0);
	LCD_RW(0);
	LCD_EN(0);
	NOP();
	NOP();
	LCD_WriteData(dat);
	NOP();
	NOP();
	LCD_EN(1);
	NOP();
	NOP();
	LCD_EN(0);
}
/************************************************************************************************
 @f_name: void LCD12684_Wcmd(u8 dat)
 @brief:	 写数据
 @param:	 u8 dat 输入数据
 @return: None
 ************************************************************************************************/
void LCD12684_Wdat(u8 dat) {
	while (LCD12864_busy())
		; //忙检测
	LCD_RS(1);
	LCD_RW(0);
	LCD_EN(0);
	NOP();
	NOP();
	LCD_WriteData(dat);
	NOP();
	NOP();
	LCD_EN(1);
	NOP();
	NOP();
	LCD_EN(0);
}
/************************************************************************************************
 @f_name: void LCD12864_Init(void)
 @brief:	 液晶初始化
 @param:	 None
 @return: None
 ************************************************************************************************/
void LCD12864P_Init(void) {
	LCD12864P_InitPort();
	Delay_ms(5);

	LCD_PSB(1);
	//并口方式    
	LCD_RST(0);
	//液晶复位	低电平有效
	Delay_ms(3);
	LCD_RST(1);
	//置高电平等待复位
	Delay_ms(3);

	LCD12684_Wcmd(0x34); //扩充指令操作
	Delay_ms(5);
	LCD12684_Wcmd(0x30); //基本指令操作
	Delay_ms(5);
	LCD12684_Wcmd(0x0c); //显示开，关光标
	Delay_ms(5);
	LCD12684_Wcmd(0x01); //清除LCD的显示内容
	Delay_ms(5);
}
/************************************************************************************************
 @f_name: void LCD12864_Clr(void)
 @brief:	 清屏
 @param:	 None
 @return: None
 ************************************************************************************************/
void LCD12864_Clr(void) {
	LCD12684_Wcmd(0x34); //扩充指令操作	“绘图”
	Delay_ms(5);
	LCD12684_Wcmd(0x30); //基本指令操作
	Delay_ms(5);
	LCD12684_Wcmd(0x01); //清屏
	Delay_ms(5);
}
/************************************************************************************************
 @f_name: void LCD12864_Pos(u8 x,u8 y)
 @brief:	 设置显示位置
 @param:	 u8 x：X轴    u8 y:Y轴  
 @return: None
 ************************************************************************************************/
void LCD12864_Pos(u8 x, u8 y) {
	u8 pos;
	if (x == 0) {
		x = 0x80;
	} else if (x == 1) {
		x = 0x90;
	} else if (x == 2) {
		x = 0x88;
	} else if (x == 3) {
		x = 0x98;
	} else
		x = 0x80;
	pos = x + y;
	LCD12684_Wcmd(pos); //设置地址  
}
/************************************************************************************************
 @f_name: void LCD12864_PhotoDis(u8 *bmp)
 @brief:	 显示图形
 @param:	 u8 *bmp 图形数组
 @return: None
 ************************************************************************************************/
void LCD12864_PhotoDis(u8 *bmp) {
	u8 i, j;
	LCD12684_Wcmd(0x34); //关闭图形显示

	for (i = 0; i < 32; i++) {
		LCD12684_Wcmd(0x80 + i); //先写入水平坐标值
		LCD12684_Wcmd(0x80); //写入垂直坐标值
		for (j = 0; j < 16; j++) //再写入两个8位元的数据    
			LCD12684_Wdat(*bmp++);
		Delay_ms(1);
	}

	for (i = 0; i < 32; i++) {
		LCD12684_Wcmd(0x80 + i);
		LCD12684_Wcmd(0x88);
		for (j = 0; j < 16; j++)
			LCD12684_Wdat(*bmp++);
		Delay_ms(1);
	}
	LCD12684_Wcmd(0x36); //写完数据,开图形显示 	
}
//显示一个字符或汉字
void LCD_ShowChar(u8 p) //所在地址是上次设置的指针累加
		{
	LCD12684_Wdat(p);
	LCD12864_XY++;
}
/************************************************************************************************
 @f_name: void LCD_ShowString(u8 x,u8 y,const u8 *p)
 @brief:	 显示字符串
 @param:	 u8 *bmp 图形数组
 @return: None
 ************************************************************************************************/
void LCD_ShowStringXY(u8 x, u8 y, char *p) {
	u8 temp;
	if (x > 4) {
		x = 1;
	}
	if (y > 4) {
		y = 0;
	}
	LCD12864_Pos(x, y);
	temp = *p;
	while (temp != '\0') {
		LCD_ShowChar(temp);
		temp = *(++p);
	}
}

void LCD_ShowString(u8 *p) //所在地址是上次设置的指针累加
		{
	u8 temp;
	temp = *p;
	while (temp != '\0') {
		LCD_ShowChar(temp);
		temp = *(++p);
	}
}

