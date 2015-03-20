#include "Types.h"
#include "hardware.h"

uint8_t flag_get_Induction_Car[7] = { 0 };
uint8_t flag_get_key_Card = 0;
uint8_t flag_test_get_key_Card = 0;
uint8_t flag_null_key_Card = 2;
uint8_t flag_test_null_key_Card = 2;
uint8_t flag_down_state = 0;
Boolean flag_check_card_been_got_away_state = FALSE;

Boolean check_test_card_state = FALSE;

uint16_t GPIO_Pin_Input_Array[] = {
	GPIO_Pin_8,
	GPIO_Pin_8,
	GPIO_Pin_9,
	GPIO_Pin_10,
	GPIO_Pin_11,
	GPIO_Pin_12,
	GPIO_Pin_13,
	GPIO_Pin_14,
	GPIO_Pin_15
};

uint8_t sn_Induction_Car = 0;

void NVIC_init(void);
void RCC_init(void);
void GPIO_init(void);
void TIM_init(void);
void spi_init(void);
void Usart_init(void);

void NVIC_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//TIM3_NVIC_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//TIM4_NVIC_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	////�ж��߳�ʼ��	   	 
	//EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	//EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����س���
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ���ж���
	//EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	////�жϷ����ʼ��
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�2
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	//NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//	delay_ms(50);            //�ȴ��ϵ縴λ���	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���  

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���  

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���  

	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);


	//Τ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	////GPIOE	  �ж����Լ��жϳ�ʼ������ �½��ش��� 
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
	//EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	//EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//GPIOE	  �ж����Լ��жϳ�ʼ������ �½��ش��� 
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource8);
	//EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	//EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIOE	  �ж����Լ��жϳ�ʼ������  �½��ش���	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource10);
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIOE	  �ж����Լ��жϳ�ʼ������  �½��ش���	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIOE	  �ж����Լ��жϳ�ʼ������  �½��ش���	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


	//NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2�� 
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	//NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;				//ʹ�����ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

}

void RCC_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);//
	TIM_DeInit(TIM3);
	TIM_DeInit(TIM4);
}

void GPIO_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = NET_INT ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//SPI1
	GPIO_InitStructure.GPIO_Pin =  NET_RST;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MISO | SPI1_MOSI ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 //USART1_TX   PA.9 һ������ ����

	GPIO_InitStructure.GPIO_Pin = USART1_TX | USART2_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART1_RX | USART2_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART3_TX | UART5_TX | SPI2_SCK | SPI2_MOSI;// | SPI2_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
	GPIO_InitStructure.GPIO_Pin = F_CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_MISO;// | SPI2_SCK  | SPI2_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* PB6-I2C1_SCL��PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin = I2C1_SCL | I2C1_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       // ��©���
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART3_RX;// | SPI2_SCK  | SPI2_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D0 | D1 | D2 | D3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		   //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = NET_CS ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RS485_RE1 | RS485_RE2 | RS485_RE3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART4_TX | UART5_TX ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART4_RX | D0 | D1 | D2 | D3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BEEP | LED1_OUT | LED2_OUT | LED3_OUT | LED4_OUT;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��

	GPIO_InitStructure.GPIO_Pin = CPU1 | CPU2 | CPU3 | CPU4 | CPU5 | CPU6 | CPU7 | CPU8  ;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD.6		

	GPIO_InitStructure.GPIO_Pin = UART5_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = WIGEN_1_D0 | WIGEN_1_D1 | WIGEN_2_D0 | WIGEN_2_D1 | WIGEN_3_D0 | WIGEN_3_D1 | WIGEN_4_D0 | WIGEN_4_D1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RELAY1 | RELAY2 | RELAY3 | RELAY4 ;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��

	/* Deselect the FLASH: Chip Select high */
	SPI_FLASH_CS_HIGH();
	GPIO_SetBits(GPIOC, NET_CS);
	GPIO_SetBits(GPIOD, LED1_OUT | LED2_OUT | LED3_OUT | LED4_OUT);			//�����
	GPIO_ResetBits(GPIOD, BEEP);
	GPIO_Write(GPIOD, 0x0000);
	GPIO_Write(GPIOE, 0xffff);
}

void TIM_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM3, 71, TIM_PSCReloadMode_Immediate);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 1000;
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void spi_init()
{
	SPI_InitTypeDef   SPI_InitStructure;
	/* SPI Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI2, ENABLE);
}

void I2C_init()
{
		//I2C_InitTypeDef  I2C_InitStructure;

		///* I2C ���� */
		//I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		//I2C_InitStructure.I2C_OwnAddress1 = I2C1_OWN_ADDRESS7;
		//I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		//I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		//I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

		///* ʹ�� I2C1 */
		//I2C_Cmd(I2C1, ENABLE);

		///* I2C1 ��ʼ�� */
		//I2C_Init(I2C1, &I2C_InitStructure);

}

//void LCD_init()
//{
//	LCD_PSB(1);   //���ڷ�ʽ    
//	LCD_RST(0);   //Һ����λ	�͵�ƽ��Ч
//	delay_ms(3);
//	LCD_RST(1);  	//�øߵ�ƽ�ȴ���λ
//	delay_ms(3);

//	LCD12684_Wcmd(0x34);    //����ָ�����
//	delay_ms(5);
//	LCD12684_Wcmd(0x30);    //����ָ�����
//	delay_ms(5);
//	LCD12684_Wcmd(0x0c);   //��ʾ�����ع��
//	delay_ms(5);
//	LCD12684_Wcmd(0x01);   //���LCD����ʾ����
//	delay_ms(5);
//}

void Usart_init()
{
	USART_InitTypeDef USART_InitStructure;

//#ifndef USE_GPIOA_AS_USART1
//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
//#endif
//	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
                   //ʹ�ܴ��� 

	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�3
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�2   
	USART_Cmd(USART2, ENABLE);

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�1	 
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

	USART_Init(UART4, &USART_InitStructure);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�4	 
	USART_Cmd(UART4, ENABLE); 

	USART_Init(UART5, &USART_InitStructure);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�5	 
	USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 

}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (uint8_t) ch;      
	return ch;
}


void hardware_init()
{
//	users_init();
	delay_init();	    	 //��ʱ������ʼ��	
	
	RCC_init(); 
	GPIO_init();
	NVIC_init();
	TIM_init();
	Usart_init();
	spi_init();
	I2C_init();
//	LCD_init();
	Reset_W5500();

	Set_network();

	//ds1302_set_time_test();
	//Start1302();

	//read_conf_from_flash();
	//boxes_init();

	

}

uint8_t get_uint8_data(uint32_t dat)
{
	uint8_t ret_dat = 0, i = 0;
	for (; i < 8; i++)
	{
		ret_dat |= (dat % 10) << i;
		dat /= 10;
	}
	return  ret_dat;
}
