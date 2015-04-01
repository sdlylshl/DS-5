
#include "lcd_1602a.h"
#include "../System/Timer/timer4.h"
extern void Delay_ms(__IO uint32_t ms);
#if 0

/***********************基本指令***********************************/
#define SET_EN  GPIO_SetBits(GPIOC, L_1602A_E)		//EN 使能  输出1
#define CLE_EN  GPIO_ResetBits(GPIOC, L_1602A_E)	//      输出0 
#define SET_RW  GPIO_SetBits(GPIOC, L_1602A_RW)      //RW 读写  输出1
#define CLE_RW  GPIO_ResetBits(GPIOC, L_1602A_RW)	//      输出0
#define SET_RS  GPIO_SetBits(GPIOC, L_1602A_RS)		//RS 指令  输出1
#define CLE_RS  GPIO_ResetBits(GPIOC, L_1602A_RS)	//      输出0
/******************************************************************/



void Lcd_Init( void )  //初始化
{  
 	//Lcd_GPIO_init();
	Delay_us(1500);                     //延时15ms
	Lcd_Write_Command( 0x38,0);       // 写指令38H 不检测忙信号
	Delay_us(500);				      //延时5ms
    Lcd_Write_Command( 0x38,0);       // 写指令38H 不检测忙信号
	Delay_us(500);					  //延时5ms
    Lcd_Write_Command( 0x38,0);       // 写指令38H 不检测忙信号
   									  //以后每次写指令、读/写数据操作之前需检测忙信号
	Lcd_Write_Command( 0x38,1);       //显示模式设置 
    Lcd_Write_Command( 0x08,1);		  //显示关闭
	Lcd_Write_Command( 0x01,1);       //显示清屏
    Lcd_Write_Command( 0x06,1);       //显示光标移动设置 
    Lcd_Write_Command( 0x0C,1); 	  //显示开、光标不显示
} 
/******************************************************/

void Lcd_En_Toggle(void) //发使能脉冲
{
	SET_EN;        //使能1
	Delay_us(5);   //延时160us
	CLE_EN;
}


void Lcd_Busy(void)//判断忙
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);    //打开DATA端口时钟 

     GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //输入模式 = 上拉输入
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure);      //打开忙检测端口

 	 CLE_RS; //RS = 0
	 //Delay_us(1);   //延时10us
	 SET_RW; //RW = 1
	 //Delay_us(1);   //延时10us
	 SET_EN; //EN = 1
	 //Delay_us(2);   //延时20us
     while (GPIO_ReadInputDataBit(GPIO_DATA_7,GPIO_DATA_7_PIN)); //循环等待忙检测端口 = 0
	 CLE_EN; //EN = 0

     //恢复端口为输出状态 
   	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);    //打开DATA端口时钟 
	 GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure);

} 

void Gpio_data(unsigned char x)  //端口置入数据
{  
	GPIO_ResetBits(GPIOC, L_1602A_D0);  //DB0
	GPIO_ResetBits(GPIOC, L_1602A_D1);  //DB1
	GPIO_ResetBits(GPIOC, L_1602A_D2);  //DB2
	GPIO_ResetBits(GPIOC, L_1602A_D3);  //DB3
	GPIO_ResetBits(GPIOC, L_1602A_D4);  //DB4
	GPIO_ResetBits(GPIOC, L_1602A_D5);  //DB5
	GPIO_ResetBits(GPIOC, L_1602A_D6);  //DB6
	GPIO_ResetBits(GPIOC, L_1602A_D7);  //DB7
	if(x&0X01)
		GPIO_SetBits(GPIOC, L_1602A_D0);//DB0
	if(x&0X02)
		GPIO_SetBits(GPIOC, L_1602A_D1);//DB1
	if(x&0X04)
		GPIO_SetBits(GPIOC, L_1602A_D2);//DB2
	if(x&0X08)
		GPIO_SetBits(GPIOC, L_1602A_D3);//DB3
	if(x&0X10)
		GPIO_SetBits(GPIOC, L_1602A_D4);//DB4
	if(x&0X20)
		GPIO_SetBits(GPIOC, L_1602A_D5);//DB5
	if(x&0X40)
		GPIO_SetBits(GPIOC, L_1602A_D6);//DB6
	if(x&0X80)
		GPIO_SetBits(GPIOC, L_1602A_D7);//DB7
}


//向液晶里面写入指令  时序：RS=L,RW=L,Data0-Data7=指令码，E=高脉冲
void Lcd_Write_Command(unsigned char x,char Busy) 
{ 
    if(Busy) 
		Lcd_Busy();
    //Delay_us(1);   //延时10us
	CLE_RS;  //RS = 0 
    //Delay_us(1);   //延时10us
	CLE_RW;  //RW = 0 
    //Delay_us(4);   //延时40us
    Gpio_data(x);  //端口置入数据
    //Delay_us(4);   //延时40us
	Lcd_En_Toggle();  //发使能脉冲
    //Delay_us(1);   //延时100us
	Lcd_Busy(); //测忙

}
//向液晶里面写入数据  时序：RS=H,RW=L,Data0-Data7=指令码，E=高脉冲
void Lcd_LCD1206_Write_Data( unsigned char x) //向液晶里面写入数据
{ 
	Lcd_Busy(); //测忙
    //Delay_us(1);   //延时10us
	SET_RS;   //RS = 1 
    //Delay_us(1);   //延时10us
    CLE_RW;   //RW = 0
    //Delay_us(4);   //延时40us
    Gpio_data(x);
    //Delay_us(4);   //延时40us
	Lcd_En_Toggle();  //发使能脉冲
    //Delay_us(1);   //延时100us
	Lcd_Busy(); //测忙

} 


void Lcd_SetXY(unsigned char x,unsigned char y)   //字符初始位置设定，x表示列，y表示行 
{ 
     unsigned char addr; 
     if(y==0) 
          addr=0x80+x; 
     else if(y==1)
          addr=0xC0+x; 
     Lcd_Write_Command(addr,1) ; 
} 
/******************************************************/

void Lcd_Puts(unsigned char x,unsigned char y, unsigned char *string) //向1602写一个字符串 
{ 
	//unsigned char i=0;
	Lcd_SetXY(x,y); 
	while(*string) 
	{ 
		Lcd_LCD1206_Write_Data(*string);
		string++; 
	} 
}



unsigned char strTemp2[2];
unsigned char strTemp3[3];
unsigned char strTemp4[4];
unsigned char strTemp5[5];
unsigned char strTemp6[6];
unsigned char strTemp7[7];

unsigned char *tr_2(unsigned int outData_2)	
{
	strTemp2[0]=(unsigned char)(((outData_2/10) % 10) + 0x30);
	strTemp2[1]=(unsigned char)((outData_2%10) + 0x30);
	return strTemp2;
}

unsigned char *tr_3(unsigned int outData3)
{
	strTemp3[0]=(unsigned char)(((outData3/100) % 10) + 0x30);
	strTemp3[1]=(unsigned char)(((outData3/10) % 10) + 0x30);
	strTemp3[2]=(unsigned char)((outData3%10) + 0x30);
	return strTemp3;
}



unsigned char *tr_4(unsigned int outData4)
{
	strTemp4[0]=(unsigned char)(((outData4/1000)% 10) + 0x30);
	strTemp4[1]=(unsigned char)(((outData4/100)% 10) + 0x30);
	strTemp4[2]=(unsigned char)(((outData4/10)% 10) + 0x30);
	strTemp4[3]=(unsigned char)((outData4%10) + 0x30);
	return strTemp4;
}


unsigned char *tr_5(unsigned int outData5)
{
	strTemp5[0]=(unsigned char)(((outData5/10000)% 10) + 0x30);
	strTemp5[1]=(unsigned char)(((outData5/1000)% 10) + 0x30);
	strTemp5[2]=(unsigned char)(((outData5/100)% 10) + 0x30);
	strTemp5[3]=(unsigned char)(((outData5/10)% 10) + 0x30);
	strTemp5[4]=(unsigned char)((outData5%10) + 0x30);
	return strTemp5;
}

unsigned char *tr_6(unsigned int outData6)
{
	strTemp6[0]=(unsigned char)(((outData6/100000)% 10) + 0x30);
	strTemp6[1]=(unsigned char)(((outData6/10000)% 10) + 0x30);
	strTemp6[2]=(unsigned char)(((outData6/1000)% 10) + 0x30);
	strTemp6[3]=(unsigned char)(((outData6/100)% 10) + 0x30);
	strTemp6[4]=(unsigned char)(((outData6/10)% 10) + 0x30);
	strTemp6[5]=(unsigned char)((outData6%10) + 0x30);
	return strTemp6;
}



unsigned char *tr_7(unsigned int outData7)
{
	strTemp7[0]=(unsigned char)(((outData7/1000000)% 10) + 0x30);
	strTemp7[1]=(unsigned char)(((outData7/100000)% 10) + 0x30);
	strTemp7[2]=(unsigned char)(((outData7/10000)% 10) + 0x30);
	strTemp7[3]=(unsigned char)(((outData7/1000)% 10) + 0x30);
	strTemp7[4]=(unsigned char)(((outData7/100)% 10) + 0x30);
	strTemp7[5]=(unsigned char)(((outData7/10)% 10) + 0x30);
	strTemp7[6]=(unsigned char)((outData7%10) + 0x30);
	return strTemp7;
}


/******************************************************/

#endif


//LCD1602 的命令状态
//数据
#define Set_RS() GPIO_SetBits(GPIOC,L_1602A_RS); 
//指令
#define Reset_RS() GPIO_ResetBits(GPIOC,L_1602A_RS); 
// 读
#define Set_RW() GPIO_SetBits(GPIOC,L_1602A_RW); 
// 写
#define Reset_RW() GPIO_ResetBits(GPIOC,L_1602A_RW);
// 使能
#define Set_E() GPIO_SetBits(GPIOC,L_1602A_E); 
// 失能
#define Reset_E() GPIO_ResetBits(GPIOC,L_1602A_E);

uint8_t arm_string[] = {"arm ok   "};
uint8_t disarm_string[] = {"disarm ok"};
uint8_t elin[] ={ "     e-Lin      " };

uint8_t elinsharp[] = { "     e-Lin     *" };


GPIO_InitTypeDef GPIO_InitStructure;

void GPIO_Config(void); 
void Busy_Wait(void); 
void Write_Cmd(uint8_t Cmd); 
void LCD1602_Write_Data(uint8_t Data);
void Write_String(uint8_t cmd,uint8_t* p); 
void LCD1602_Init(void); 

void LCD1602_Delay(uint32_t t);


uint32_t LCD1602TIM=500, lcd1602time,flag;
void LCD1602_Flashing(uint32_t ms){
//LCD1602TIM = ms;
if (flag&&(LCD1602TIM < TIM4_GetDistanceTime(lcd1602time))){
	Write_String(0x8f, "*");
	flag =0;
}
else if ((LCD1602TIM * 1.8) < TIM4_GetDistanceTime(lcd1602time)){
	Write_String(0x8f, " ");
	flag =1;
	lcd1602time = TIM4_GetCurrentTime();
}
}
//uint8_t Read_Data(void); 

void LCD1602_GPIO_Init(){

	//RCC_APB2PeriphClockCmd(LCD1602_RCC_PORT, ENABLE);
	//1602A
	  GPIO_InitStructure.GPIO_Pin	= L_1602A_RS | L_1602A_RW | L_1602A_E ;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  GPIO_SetBits(GPIOC, L_1602A_RS);


	  GPIO_InitStructure.GPIO_Pin	= L_1602A_LCD ;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  GPIO_SetBits(GPIOC, L_1602A_LCD);


	  GPIO_InitStructure.GPIO_Pin	= L_1602A_D0 | L_1602A_D1 | L_1602A_D2 | L_1602A_D3 |L_1602A_D4 | L_1602A_D5 |L_1602A_D6 |L_1602A_D7;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
int l_1602a_test(void) 
{ 
//	printf("1602a test\n");
	
	Write_String(0x80,elin); 
	Write_String(0xc0,disarm_string); 
	while(1);  
}


void Busy_Wait(void) 
{ 
	GPIO_InitStructure.GPIO_Pin=L_1602A_D0 | L_1602A_D1 | L_1602A_D2 | L_1602A_D3 |L_1602A_D4 | L_1602A_D5 |L_1602A_D6 |L_1602A_D7;; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	Reset_RS(); 
	Set_RW(); 
	Reset_E(); 
	//LCD1602_Delay(5); 
	Set_E(); 
	// LCD1602_Delay(25); 
	while(GPIO_ReadInputDataBit(GPIOC,L_1602A_D7)==1)
		;  
	Reset_E();
	GPIO_InitStructure.GPIO_Pin=L_1602A_D0 | L_1602A_D1 | L_1602A_D2 | L_1602A_D3 |L_1602A_D4 | L_1602A_D5 |L_1602A_D6 |L_1602A_D7;;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}


void Write_Cmd(uint8_t Cmd)
{
	Busy_Wait();
	Reset_RS();
	Reset_RW();
	Reset_E();
	Delay_ms(5);
	Set_E();
	//GPIO_Write(GPIOC,(0xff00&(Cmd<<8)));
	
	GPIO_WriteBit(GPIOC,L_1602A_D7,(BitAction)((Cmd&0x80)>>7));//D7
	GPIO_WriteBit(GPIOC,L_1602A_D6,(BitAction)((Cmd&0x40)>>6));//D6
	GPIO_WriteBit(GPIOC,L_1602A_D5,(BitAction)((Cmd&0x20)>>5));//D5
	GPIO_WriteBit(GPIOC,L_1602A_D4,(BitAction)((Cmd&0x10)>>4));//D4
	GPIO_WriteBit(GPIOC,L_1602A_D3,(BitAction)((Cmd&0x08)>>3));//D3
	GPIO_WriteBit(GPIOC,L_1602A_D2,(BitAction)((Cmd&0x04)>>2));//D2
	GPIO_WriteBit(GPIOC,L_1602A_D1,(BitAction)((Cmd&0x02)>>1));//D1
	GPIO_WriteBit(GPIOC,L_1602A_D0,(BitAction)((Cmd&0x01)));//D0
	
	Delay_ms(25);
	Reset_E();
}

void LCD1602_Write_Data(uint8_t Data)
{
	Busy_Wait();
	Set_RS();
	Reset_RW();
	Reset_E();
	Delay_ms(5); 
	Set_E(); 
	//GPIO_Write(GPIOD,(0xff00&(Data<<8))); 

	
	GPIO_WriteBit(GPIOC,L_1602A_D7,(BitAction)((Data&0x80)>>7));//D7 
	GPIO_WriteBit(GPIOC,L_1602A_D6,(BitAction)((Data&0x40)>>6)); //D6 
	GPIO_WriteBit(GPIOC,L_1602A_D5,(BitAction)((Data&0x20)>>5)); //D5 
	GPIO_WriteBit(GPIOC,L_1602A_D4,(BitAction)((Data&0x10)>>4)); //D4 
	GPIO_WriteBit(GPIOC,L_1602A_D3,(BitAction)((Data&0x08)>>3)); //D3 
	GPIO_WriteBit(GPIOC,L_1602A_D2,(BitAction)((Data&0x04)>>2)); //D2 
	GPIO_WriteBit(GPIOC,L_1602A_D1,(BitAction)((Data&0x02)>>1)); //D1 
	GPIO_WriteBit(GPIOC,L_1602A_D0,(BitAction)((Data&0x01))); //D0 
	

	Delay_ms(25); 

	Reset_E(); 
} 


void Write_String(uint8_t cmd,uint8_t* p) 
{ 
	//uint8_t i=0; 
	Write_Cmd(cmd); 

	while(*p!='\0') 
	{ 
		LCD1602_Write_Data(*p++);
		// Buffer[i++]=Read_Data(); 
	} 
} 

void display_arm()
{
	//Write_Cmd(0x01); 
	Write_String(0xc0, arm_string);
}

void display_disarm()
{
	//Write_Cmd(0x01); 
	Write_String(0xc0, disarm_string);
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
	Delay_ms(15);
	Write_Cmd(0x38); 
	Delay_ms(5);
	Write_Cmd(0x38);
	Write_Cmd(0x08); 
	Write_Cmd(0x01); 
	Write_Cmd(0x06); 
	Write_Cmd(0x0C);
	
 
	Write_String(0x80,elin);
	Write_String(0xC0,elin);
} 

 

//14ns 

void LCD1602_Delay(__IO uint32_t t) 
{  
	while(t--); 
} 
	
#if 0
uint8_t Read_Data(void) 
{ 	
	uint8_t Value; 	 
	Busy_Wait(); 	 
	Reset_RW(); 
	Reset_E(); 
	LCD1602_Delay(5); 
	Set_E(); 
	Value=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_10)||GPIO_ReadOutputDataBit(GPIOE,GPI
	
	O_Pin_9)||GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_8)||GPIO_ReadOutputDataBit(GPIOE,G
	
	PIO_Pin_7)|| GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_1)||GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_
	
	0)||GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_15)||GPIO_ReadOutputDataBit(GPIOD,GPIO_
	
	Pin_14); 
	
	LCD1602_Delay(25); 
	Reset_E(); 
	return Value; 
} 
#endif 


