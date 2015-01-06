#include "ps2.h" 
#include "sys.h"
#include "stdio.h"
#include "stm32f10x_exti.h"
extern void Delay_us(__IO uint32_t us);
//extern void Delay_ms(__IO uint32_t ms);

//PS2产生的时钟频率在10~20Khz(最大33K)
//高/低电平的持续时间为25~50us之间.	  

//PS2_Status当前状态标志
//[7]:接收到一次数据;[6]:校验错误;[5:4]:当前工作的模式;[3:0]:收到的数据长度;		   
uint8_t PS2_Status = CMDMODE; //默认为命令模式
uint8_t PS2_DATA_BUF[16]; //ps2数据缓存区
uint8_t KeyBoard_Buf[8];
uint8_t KeyBoard_ASCII;
//位计数器
uint8_t BIT_Count = 0;
uint8_t KEY;

static void PS2_GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
//	1.       设置好相应的时钟；
//	2.       IO口初始化；
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PS2_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = PS2_CLK_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PS2_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PS2_DAT_PIN;
	GPIO_InitStructure.GPIO_Mode = PS2_DAT_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PS2_DAT_PORT, &GPIO_InitStructure);
}

static void PS2_NVIC_Config(void) {
	//	3.       设置相应的中断；
	NVIC_SetPriority(EXTI1_IRQn, 5);
	NVIC_EnableIRQ(EXTI1_IRQn);
}
//STM32中，每一个GPIO都可以触发一个外部中断
//但是，GPIO的中断是以组位一个单位的，同组间的外部中断同一时间只能使用一个
//比如说，PA0，PB0，PC0，PD0，PE0，PF0，PG0这些为1组
//如果我们使用PA0作为外部中断源，那么别的就不能够再使用了
//在此情况下，我们只能使用类似于PB1，PC2这种末端序号不同的外部中断源
//每一组使用一个中断标志EXTIx
//EXTI0 – EXTI4这5个外部中断有着自己的单独的中断响应函数
//EXTI5-9	共用一个中断响应函数
//EXTI10-15	共用一个中断响应函数

static void PS2_EXIT_Config(void) {
//	4.       把相应的IO口设置为中断线路（要在设置外部中断之前）并初始化；
	EXTI_InitTypeDef EXTI_InitStructure;
	//清空中断标志
	EXTI_ClearITPendingBit(EXTI_Line1);
	//同组间的外部中断同一时间只能使用一个 GPIOB1挂到中断线1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能中断线
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

}


void PS2_Init(void) {

	PS2_GPIO_Config();
	//PS2_NVIC_Config();
	//PS2_EXIT_Config();

}

//每11个bit,为接收1个字节
//每接收完一个包(11位)后,设备至少会等待50ms再发送下一个包
//只做了鼠标部分,键盘部分暂时未加入
void PS2_IRQHandle(void) {

//	5.       在选择的中断通道的响应函数中中断函数。
	static uint8_t tempdata = 0;
	static uint8_t parity = 0;


		if (BIT_Count == 0) {
			parity = 0;
			tempdata = 0;
		}
		BIT_Count++;
		if (BIT_Count > 1 && BIT_Count < 10) //这里获得数据
				{
			tempdata >>= 1;
			if (PS2_SDA) {
				tempdata |= 0x80;
				parity++; //记录1的个数
			}
		} else if (BIT_Count == 10) //得到校验位
				{
			if (PS2_SDA) {
				parity |= 0x80; //校验位为1
			}
		} else if (BIT_Count == 11) //接收到1个字节的数据了
				{
			BIT_Count = parity & 0x7f; //取得1的个数
			//奇偶校验OK
			if (((BIT_Count % 2 == 0) && (parity & 0x80))
					|| ((BIT_Count % 2 == 1) && (parity & 0x80) == 0)) {
				KEY = tempdata;
//				键盘处理
				//Decode(tempdata);
				printf("按键:%x",KEY);
				//UART1_Put_Char(tempdata);
			}

			BIT_Count = 0;
		}
	
}

//禁止数据传输
//把时钟线拉低,禁止数据传输	   
void PS2_Dis_Data_Report(void) {
	PS2_Set_Int(0); //关闭中断
	PS2_SET_SCL_OUT(); //设置SCL为输出
	PS2_SCL_OUT = 0; //抑制传输
}
//使能数据传输
//释放时钟线		    
void PS2_En_Data_Report(void) {
	PS2_SET_SCL_IN(); //设置SCL为输入
	PS2_SET_SDA_IN(); //SDA IN
	PS2_SCL_OUT = 1; //上拉
	PS2_SDA_OUT = 1;
	PS2_Set_Int(1); //开启中断
}

//PS2中断屏蔽设置
//en:1，开启;0，关闭;	 
void PS2_Set_Int(uint8_t en) {
	EXTI->PR = 1 << 11; //清除LINE11上的中断标志位
	if (en)
		EXTI->IMR |= 1 << 11; //不屏蔽line11上的中断
	else
		EXTI->IMR &= ~(1 << 11); //屏蔽line11上的中断
}
////等待PS2时钟线sta状态改变
//sta:1，等待变为1;0，等待变为0;
//返回值:0，时钟线变成了sta;1，超时溢出;	 
uint8_t Wait_PS2_Scl(uint8_t sta) {
	u16 t = 0;
	sta = !sta;
	while (PS2_SCL == sta) {
		Delay_us(1);
		t++;
		if (t > 16000)
			return 1; //时间溢出 (设备会在10ms内检测这个状态)
	}
	return 0; //被拉低了
}
//在发送命令/数据之后,等待设备应答,该函数用来获取应答
//返回得到的值
//返回0，且PS2_Status.6=1，则产生了错误
uint8_t PS2_Get_Byte(void) {
	u16 t = 0;
	uint8_t temp = 0;
	while (1) //最大等待55ms
	{
		t++;
		Delay_us(10);
		if (PS2_Status & 0x80) //得到了一次数据
				{
			temp = PS2_DATA_BUF[PS2_Status & 0x0f - 1];
			PS2_Status &= 0x70; //清除计数器，接收到数据标记
			break;
		} else if (t > 5500 || (PS2_Status & 0x40))
			break; //超时溢出/接收错误
	}
	PS2_En_Data_Report(); //使能数据传输
	return temp;
}
//发送一个命令到PS2.
//返回值:0，无错误,其他,错误代码
uint8_t PS2_Send_Cmd(uint8_t cmd) {
	uint8_t i;
	uint8_t high = 0; //记录1的个数
	PS2_Set_Int(0); //屏蔽中断
	PS2_SET_SCL_OUT(); //设置SCL为输出
	PS2_SET_SDA_OUT(); //SDA OUT
	PS2_SCL_OUT = 0; //拉低时钟线
	Delay_us(120); //保持至少100us
	PS2_SDA_OUT = 0; //拉低数据线
	Delay_us(10);
	PS2_SET_SCL_IN(); //释放时钟线,这里PS2设备得到第一个位,开始位
	PS2_SCL_OUT = 1;
	if (Wait_PS2_Scl(0) == 0) //等待时钟拉低
			{
		for (i = 0; i < 8; i++) {
			if (cmd & 0x01) {
				PS2_SDA_OUT = 1;
				high++;
			} else
				PS2_SDA_OUT = 0;
			cmd >>= 1;
			//这些地方没有检测错误,因为这些地方不会产生死循环
			Wait_PS2_Scl(1); //等待时钟拉高 发送8个位
			Wait_PS2_Scl(0); //等待时钟拉低
		}
		if ((high % 2) == 0)
			PS2_SDA_OUT = 1; //发送校验位 10
		else
			PS2_SDA_OUT = 0;
		Wait_PS2_Scl(1); //等待时钟拉高 10位
		Wait_PS2_Scl(0); //等待时钟拉低
		PS2_SDA_OUT = 1; //发送停止位  11
		Wait_PS2_Scl(1); //等待时钟拉高 11位
		PS2_SET_SDA_IN(); //SDA in
		Wait_PS2_Scl(0); //等待时钟拉低
		if (PS2_SDA == 0)
			Wait_PS2_Scl(1); //等待时钟拉高 12位
		else {
			PS2_En_Data_Report();
			return 1; //发送失败
		}
	} else {
		PS2_En_Data_Report();
		return 2; //发送失败
	}
	PS2_En_Data_Report();
	return 0; //发送成功
}
//PS2初始化				 

