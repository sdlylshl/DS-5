
#include "keyboard.h"


//extern void Delay_us(__IO uint32_t us);
extern void Delay_ms(__IO uint32_t ms);
void Keyboard_Data_Pro(void)
{		


}


void Decode(uint8_t Rev_code)
{
	static uint8_t Break_code=0;

	if((Rev_code == 0xF0) || (Rev_code == 0xE0))
	{
		if(Rev_code == 0xF0)
		{
			Break_code = 1;
			//UART1_Put_Char(0x0b);
		}
	}else
	{
		if(Break_code == 1)
		{
			Break_code = 0;
			//UART1_Put_Char(0xb0);
			//	LED1 = ~LED1;
			//return;
			//UART1_Put_Char(0x0a);
			//UART1_Put_Char(Rev_code);
			//return;
			//UART1_Put_Char(0x0b);
			switch (Rev_code)//开始翻译扫描码
			{
//				0A 70 0B 70
//0A 69 0B 69
//0A 72 0B 72
//0A 7A 0B 67
//0A 6B 0B 6B
//0A 73 0B 73
//0A 74 0B 74
//0A 6C 0B 6C
//0A 75 0B 75
//0A 7D 0B 73
//0A 71 0B 71
				case 0x45:
					KeyBoard_ASCII = Key_0;
				break;
				case 0x16://1
					KeyBoard_ASCII = Key_1;
				break;
				case 0x1e:
					KeyBoard_ASCII = Key_2;
				break;
				case 0x26:
					KeyBoard_ASCII = Key_3;
				break;
				case 0x25:
					KeyBoard_ASCII = Key_4;
				break;
				case 0x2e:
					KeyBoard_ASCII = Key_5;
				break;
				case 0x36:
					KeyBoard_ASCII = Key_6;
				break;
				case 0x3d:
					KeyBoard_ASCII = Key_7;
				break;
				case 0x3e:
					KeyBoard_ASCII = Key_8;
				break;
				case 0x46:
					KeyBoard_ASCII = Key_9;
				break;
				case 0x49:
					KeyBoard_ASCII = Key_Dot;
				break;
				case 0x7d://up
					KeyBoard_ASCII = Key_SAVE_ASCII;
				break;
				case 0x7a://down
					KeyBoard_ASCII = Key_GET_ASCII;
				break;
				case 0x5a:
					KeyBoard_ASCII = Key_ENTER_ASCII;   //回车
				break;
				case 0x76: //backSpace
					KeyBoard_ASCII = Key_BACK_ASCII;  //退格
				break;
				case 0x66://del
					KeyBoard_ASCII = Key_DEL_ASCII;
				break;
				default:
					KeyBoard_ASCII = Rev_code;
				break;
			}
			//return;
//			switch_page(KeyBoard_ASCII);
		}
		else
		{

			//Keyboard_Data_Pro();
		}

	}

}
//初始化键盘
//返回:0,初始化成功
//其他:错误代码
//CHECK OK 2010/5/2
void Init_Keyboard(void)
{	 
	PS2_Init();							     
	Delay_ms(50);            //等待上电复位完成
	PS2_Status = KEYBOARD;//进入鼠标模式

} 











