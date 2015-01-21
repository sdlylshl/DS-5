#include "led.h"
#include "delay.h"		   

void LED_Init()
{
	RCC->APB2ENR|=1<<8;    //ʹ��PORTGʱ��
	GPIOC->CRL&=0XFFFF0000;
	GPIOC->CRL|=0X33333333;//PG.0 1 2 3�������
	GPIOC->ODR|=1<<0|1<<1|1<<2|1<<3|1<<4|1<<5|1<<6|1<<7;      //PG.0 1 2 3����� 
}
void KEY_Init()
{
	u32 temp=0x02;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR|=temp;       //����jtagģʽ
	
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	GPIOB->CRH&=0X0000FFFF;
	GPIOB->CRH|=0X88880000;
	GPIOB->ODR|=1<<12|1<<13|1<<14|1<<15;	 //PA.12 13 14 15��������
}
//����������
//���ذ���ֵ
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2!!
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//�������ɿ���־	

	if(key_up&&(KEY_GET(12)==0||KEY_GET(13)==0||KEY_GET(14)==0||KEY_GET(15)==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_GET(12)==0) return 1;	
		else if(KEY_GET(13)==0) return 2;	
		else if(KEY_GET(14)==0) return 3;	
		else if(KEY_GET(15)==0) return 4;		
	}
	else if(KEY_GET(12)==1&&KEY_GET(13)==1&&KEY_GET(14)==1&&KEY_GET(15)==1)
	  key_up=1; 	    

	return 0;// �ް�������
}
