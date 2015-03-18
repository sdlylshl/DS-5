#include"LCD12864S.h"

uint8_t const Address_Table[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86,
		0x87, //第一行汉字位置     
		0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, //第二行汉字位置
		0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, //第三行汉字位置
		0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, //第四行汉字位置
		};

//LCD12864同步时钟线1/2周期延时,STM32F103xx（72MHz）
void LCD_CLK_Delay(void) {
	uint8_t i;
	for (i = 0; i < 200; i++) {
		__nop();
	}
}
//向LCD12864发送一个字节的数据
void Send_Byte(uint8_t Temp) {
	uint8_t i, n = 0x80;
	CS_H;
	for (i = 0; i < 8; i++) {
		if (n & Temp) //此位数据为1
			SID_H;
		else
			//此位数据为0
			SID_L;
		n = n >> 1; //n由10000000依次右移一位
		CLK_H; //发生一个时钟信号上升沿数据被写入
		LCD_CLK_Delay();
		CLK_L;
	}
	CS_L;
}
//接收一个字节的数据
uint8_t Receive_Byte(void) {
	uint8_t i, temp1, temp2;
	temp1 = 0;
	temp2 = 0;
	for (i = 0; i < 8; i++) {
		temp1 = temp1 << 1;
		CLK_H; //发生一个时钟信号上升沿数据被写入
		LCD_CLK_Delay();
		CLK_L;
		if (SID_Read)
			temp1++;
	}
	for (i = 0; i < 8; i++) {
		temp2 = temp2 << 1;
		CLK_H; //发生一个时钟信号上升沿数据被写入
		LCD_CLK_Delay();
		CLK_L;
		if (SID_Read)
			temp2++;
	}
	return ((0xf0 & temp1) + (0x0f & temp2));
}
//写显示数据 
void LCD12864S_Write_Data(uint8_t Data) {
	Check_Busy(); //检测忙碌       
	Send_Byte(0xfa); //RW=0数据由MCU->LCD;RS=1,表示传的是显示数据		
	Send_Byte(Data & 0xf0); //串口控制格式[11111（RW=0)（RS）0]
	Send_Byte(Data << 4 & 0xf0);
}
//写控制命令
void Write_Comm(uint8_t Comm) {
	Check_Busy(); //检测忙碌
	Send_Byte(0xf8); //RW=0数据由MCU->LCD;RS=0,表示传的是命令		
	Send_Byte(Comm & 0xf0); //串口控制格式[11111（RW=0)（RS）0]
	Send_Byte(Comm << 4 & 0xf0);
}
//LCD12864测忙
void Check_Busy(void) {
	do
		Send_Byte(0xfc); //11111,RW(1),RS(0),0
	while (0x80 & Receive_Byte()); //BF(.7)=1 Busy
}
//LCD12864初始化
void LCD_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能端口时钟，重要！！！ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	LCD_CLK_Delay();
	LCD_CLK_Delay();
	Write_Comm(0x30); //基本指令动作8位数据流    
	Write_Comm(0x01); //清屏，地址指针指向00H 
	Write_Comm(0x06); //光标的移动方向 
	Write_Comm(0x0c); //开显示，关游标 
}
//LCD12864清屏
void LCD_Clear(void) {
	Check_Busy(); //检测忙碌
	Write_Comm(0x30); //基本指令动作  
	Write_Comm(0x01);
}

//指定位置显示字符
void Display_String(uint8_t x, uint8_t y, char *s) {
	if (x > 8)
		x = 8;
	if (y > 3)
		y = 3;
	Write_Comm(Address_Table[8 * y + x]); //根据XY写入地址
//	Write_Comm(Address_Table[8*y+x]); //有时要写两次
	while (*s > 0) {
		LCD12864S_Write_Data(*s);
		s++;
	}
}
//图形显示122*32
void Display_Image(unsigned char *DData) {
	unsigned char x, y, i;
	unsigned int tmp = 0;
	for (i = 0; i < 9;) { //分两屏，上半屏和下半屏，因为起始地址不同，需要分开
		for (x = 0; x < 32; x++) { //32行
			Write_Comm(0x34);
			Write_Comm((0x80 + x)); //列地址
			Write_Comm((0x80 + i)); //行地址，下半屏，即第三行地址0X88
			Write_Comm(0x30);
			for (y = 0; y < 16; y++)
				LCD12864S_Write_Data(DData[tmp + y]); //读取数据写入LCD
			tmp += 16;
		}
		i += 8;
	}
	Write_Comm(0x36); //扩充功能设定
	Write_Comm(0x30);
}
//显示变量的值0~9999
void Show_Num(uint8_t x, uint8_t y, uint16_t Num) {
	if (x > 8)
		x = 8;
	if (y > 3)
		y = 3;

	if (Num == 999 || Num == 99 || Num == 9)
		Display_String(x, y, "    ");

	Write_Comm(Address_Table[8 * y + x]); //根据XY写入地址
//	Write_Comm(Address_Table[8*y+x]); //有时要写两次

	if ((Num / 1000)) //千位不为0就显示千位
		LCD12864S_Write_Data(0x30 + Num / 1000);
	//	Lcd_WriteData(0x2e);//"."
	if ((Num / 1000) || (Num % 1000 / 100)) //千位和百位有一个>0就显示百位
		LCD12864S_Write_Data(0x30 + Num % 1000 / 100);
	//	Lcd_WriteData(0x2e);//"."
	if ((Num % 1000 / 100) || (Num % 100 / 10))
		LCD12864S_Write_Data(0x30 + Num % 100 / 10);
	//	Lcd_WriteData(0x2e);//"."
	if ((Num % 100 / 10) || (0x30 + Num % 10))
		LCD12864S_Write_Data(0x30 + Num % 10);
}

