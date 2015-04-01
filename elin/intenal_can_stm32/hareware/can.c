#include "can.h"
#include "hardware.h"
extern   CanTxMsg TxMessage;
extern   CanRxMsg RxMessage;
uint8_t ext_id[2];

/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置,PB8上拉输入，PB9推挽输出
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
   	
  	/*外设时钟设置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  	/*IO设置*/
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	/* Configure CAN pin: RX */									               // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure CAN pin: TX */									               // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*中断设置*/
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Mode_Config(void)
{
   	CAN_InitTypeDef        CAN_InitStructure;
	 	/************************CAN通信参数设置**********************************/
		/*CAN寄存器初始化*/
		CAN_DeInit(CAN1);
		CAN_StructInit(&CAN_InitStructure);
		/*CAN单元初始化*/
		CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
    CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  自动离线管理 
    CAN_InitStructure.CAN_AWUM=ENABLE;			   //MCR-AWUM  使用自动唤醒模式
    CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
    CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
    CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常工作模式
    CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
    CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;		   //BTR-TS1 时间段1 占用了6个时间单元
    CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 时间段2 占用了3个时间单元
    CAN_InitStructure.CAN_Prescaler =4;		   ////BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+6+3)/4=0.8Mbps
		CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//过滤器组0
	//过滤器模式设置:(1) 1个32位的屏蔽位模式的过滤器. (2) 2个32位的列表模式的过滤器. (3) 2个16位的屏蔽位模式的过滤器. (4) 4个16位的列表模式的过滤器
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

	CAN_FilterInitStructure.CAN_FilterIdHigh = ( ( ( u32 ) Default_Conf_Dat.Stu_Dat.filter_id << 3 ) & 0xFFFF0000 ) >> 16;				//要过滤的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow = ( ( ( u32 ) Default_Conf_Dat.Stu_Dat.filter_id << 3 ) | CAN_ID_EXT | CAN_RTR_DATA ) & 0xFFFF; //要过滤的ID低位 
  //屏蔽模式 为1的位必须匹配 ,为0的位 不进行匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0;			//过滤器高16位每位必须匹配
  CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0;			//过滤器低16位每位必须匹配
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN通信中断使能*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void can_init(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}

/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */	 
uint8_t can_sn = 0;
void CAN_SetMsg(void)
{	  
  //TxMessage.StdId=0x00;						 
  TxMessage.ExtId=Default_Conf_Dat.Stu_Dat.dest_std_id;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_EXT;					 //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=8;							 //数据长度为2字节
  TxMessage.Data[0]=(Default_Conf_Dat.Stu_Dat.dest_std_id>>8) & 0xff;
  TxMessage.Data[1]=Default_Conf_Dat.Stu_Dat.dest_std_id & 0xff;
	TxMessage.Data[7]=can_sn++;
CAN_Transmit(CAN1, &TxMessage);
}

void can_send_Msg(Stu_Can_Msg * msg)
{
	TxMessage.ExtId = msg->ext_id == 0 ? Default_Conf_Dat.Stu_Dat.dest_std_id : msg->ext_id;		
  TxMessage.IDE=CAN_ID_EXT;					 
  TxMessage.RTR=CAN_RTR_DATA;				 
  TxMessage.DLC=msg->len;
	memcpy(	TxMessage.Data,msg->data,msg->len);
	CAN_Transmit(CAN1, &TxMessage);
	msg->times = 0;
	reset_heart_signal_cnt();
}



void USB_LP_CAN1_RX0_IRQHandler(void)
{
 
 /*从邮箱中读出报文*/
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  /* 比较ID和数据是否为0x1314及DCBA */
  //if((RxMessage.ExtId==Default_Conf_Dat.Stu_Dat.filter_id) && (RxMessage.IDE==CAN_ID_EXT) )
  if(1)
	{
		int i = 0;
		for(;i < RxMessage.DLC;i++)
		{
			printf("0x%2x ",RxMessage.Data[i]);
		}
//		Stu_Can_Msg * msg = get_a_rx_msg();
//		if(msg != NULL)
//		{
//				msg->len = RxMessage.DLC;
//				memcpy(msg->data,RxMessage.Data,	RxMessage.DLC);	
//		}
	}
  else
  {
    	//接收失败
  }
}

void decode_can_rx_msg()
{
		int i = 0;
		for(;i < CAN_MSG_RX_LEN;i++)
		{
			if( can_msg_rx_lst[i].times == 1)
			{
				  switch(can_msg_rx_lst[i].data[0])
					{
							case NET_UDP_ALIVE:
							{
								if(Default_Conf_Dat.Stu_Dat.filter_id == 0xE0E0)
								{
									Stu_Can_Msg * msg = get_a_msg();
									if(msg != NULL)
									{
											msg->ext_id = can_msg_rx_lst[i].data[1] << 8 | can_msg_rx_lst[i].data[2];
											msg->len = 3;
											msg->data[0] = NET_UDP_ALIVE;
											msg->data[1] = Default_Conf_Dat.Stu_Dat.filter_id>>8 & 0xff;
											msg->data[2] = Default_Conf_Dat.Stu_Dat.filter_id & 0xff;
											can_send_Msg(msg);
									}
									else
									{
										printf("[heart_signal_send]Can Msg Lst is full !\r\n");
									}
								}
									printf("Get Heart Signal from 0x%2x%2x\r\n",can_msg_rx_lst[i].data[1],can_msg_rx_lst[i].data[2]);
							
							}
							break;
							default:
								break;
					}
					if(times_heart_signal_lost > 0)
							times_heart_signal_lost--;
					if(times_heart_signal_lost > 5)
						times_heart_signal_lost = 0;
					can_msg_rx_lst[i].times = 0;
			}
		}
		
//		if(times_heart_signal_lost > 5)
//		{
//			//心跳超时
//			printf("心跳超时");
//		}
}

