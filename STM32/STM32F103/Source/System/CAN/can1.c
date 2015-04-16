#include "stdio.h"
#include "can1.h"

/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t ret = 0; /* for return of the interrupt handling */
ErrorStatus HSEStartUpStatus;
uint8_t CAN_CellResetFlag;

/* Private functions ---------------------------------------------------------*/

static void CAN1_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

#ifdef  CAN1_NONREMAP

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

#endif

#ifdef  CAN1_PARTIALREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
#endif 

#ifdef  CAN1_FULLREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);
#endif 

	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = CAN1_RX_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN1_RX_PORT, &GPIO_InitStructure);

	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = CAN1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = CAN1_TX_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN1_TX_PORT, &GPIO_InitStructure);

}

/*******************************************************************************
 * Function Name  : NVIC_Configuration
 * Description    : Configures the NVIC and Vector Table base address.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void CAN1_NVIC_Config(void) {


	/* Enable CAN Tx interrupt IRQ channel USB_HP_CAN_TX_IRQChannel */
	NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 5);
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);

	/* Enable CAN RX0 FIFO0 interrupt IRQ channel USB_LP_CAN1_RX0_IRQn*/
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 6);
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	
	/* Enable CAN RX1 FIFO1 interrupt IRQ channel */
	NVIC_SetPriority(CAN1_RX1_IRQn, 7);
	NVIC_EnableIRQ(CAN1_RX1_IRQn);
	
	/* Enable CAN ERR interrupt IRQ channel CAN_SCE_IRQChannel*/
	//NVIC_SetPriority(CAN1_SCE_IRQn, 7);
	//NVIC_EnableIRQ(CAN1_SCE_IRQn);
}
static void CAN1_IT_Config(void) {
	/*打开所有CAN相关的中断,如果你需要处理各种情况*/

	/*CAN TX INTERRUPT*/
	CAN_ITConfig(CAN1, CAN_IT_TME | CAN_IT_RQCP0 | CAN_IT_RQCP1 | CAN_IT_RQCP2 , ENABLE);

	/*CAN FIFO0 INTERRUPT*/
	CAN_ITConfig(CAN1, CAN_IT_FF0 | CAN_IT_FMP0 | CAN_IT_FOV0 | CAN_IT_FMP0	, ENABLE);

	/*CAN FIFO1 INTERRUPT*/
	CAN_ITConfig(CAN1, CAN_IT_FF1 | CAN_IT_FMP1 | CAN_IT_FOV1 | CAN_IT_FMP1	, ENABLE);

	/*CAN ESR  ,ERROR PROCESS*/
	//CAN_ITConfig(CAN1,CAN_IT_SLK | CAN_IT_WKU | CAN_IT_ERR | CAN_IT_LEC | CAN_IT_BOF | CAN_IT_EPV | CAN_IT_EWG, ENABLE);

}
uint32_t FilterID  =0x0000E010;
uint32_t FilterMask=0x0;
 void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber = 0;						//过滤器组0
	//过滤器模式设置:(1) 1个32位的屏蔽位模式的过滤器. (2) 2个32位的列表模式的过滤器. (3) 2个16位的屏蔽位模式的过滤器. (4) 4个16位的列表模式的过滤器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

	CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t) (FilterID) << 3) & 0xFFFF0000) >> 16;				//要过滤的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t) (FilterID+1) << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF; //要过滤的ID低位 只接收扩展数据帧
	//屏蔽模式 为1的位必须匹配 ,为0的位 不进行匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((uint32_t) (FilterMask) << 3) & 0xFFFF0000) >> 16;;			//过滤器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((uint32_t) (FilterMask) << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF;			//过滤器低16位每位必须匹配

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
		CAN_FilterInitStructure.CAN_FilterNumber = 7;						//过滤器组0
	//过滤器模式设置:(1) 1个32位的屏蔽位模式的过滤器. (2) 2个32位的列表模式的过滤器. (3) 2个16位的屏蔽位模式的过滤器. (4) 4个16位的列表模式的过滤器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

	CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t) (FilterID) << 3) & 0xFFFF0000) >> 16;				//要过滤的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t) (FilterID) << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF; //要过滤的ID低位 只接收扩展数据帧
	//屏蔽模式 为1的位必须匹配 ,为0的位 不进行匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;			//过滤器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;			//过滤器低16位每位必须匹配

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber = 5;	
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO1。 */
	CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)FilterID << 3) & 0xFFFF0000) >> 16;				//要过滤的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)FilterID << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF; //要过滤的ID低位  
	//屏蔽模式 为1的位必须匹配 ,为0的位 不进行匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;			//过滤器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;			//过滤器低16位每位必须匹配

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
}
void CAN1_Config(void) {
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	CAN1_GPIO_Config();
//	Reset CAN1
	CAN_DeInit(CAN1);

	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;	/*禁止时间触发通讯模式*/
	CAN_InitStructure.CAN_ABOM = ENABLE;	/*自动退出离线状态方式	0-相当于有条件手动离线，1-相当于自动离线*/
	CAN_InitStructure.CAN_AWUM = ENABLE;	/*自动唤醒模式	 0-由软件通过清0唤醒，1-检测到报文时，自动唤醒*/
	CAN_InitStructure.CAN_NART = ENABLE;	/*报文自动重传	 0-一直重复发送直到成功，1-不论成功以否只发送一次*/
	CAN_InitStructure.CAN_RFLM = DISABLE;	/*接收FIFO 锁定模式	0-溢出时未锁定，新报文盖掉掉报文，1-FIFO锁定，溢出后新报文直接丢失*/
	CAN_InitStructure.CAN_TXFP = ENABLE;	/*发送FIFO优先级		0-报文发送优先级由标志符决定，1-报文发送优先级由请求先后顺序决定*/
	/*模式-测试模式-正常模式*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	//CAN_InitStructure.CAN_Mode=CAN_MODE;

	/*=======================
	 BTR=1/(Tbit)=1/(1+Tbs1+Tbs2)*Tq)=1/((1+8+7)*Tq)=1/16Tq
	 Tq=Tpclk1*(prescale+1)=(prescale+1)/Fpclk1
	 BTR=1/(16*(prescale+1)/Fpclk1)=Fpclk1/(16*(prescale+1))
	 Precalse=(Fpclk1/(16*BTR))-1
	 ========================*/
#if (CAN_BTR==800)
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler=4; 
#elif (CAN_BTR==500)
#if 0
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler=6; //4
	//printf("Can BUS Run at 500Kpbs...");
#else
	/* 36 000 000/500 000=72, 4*(1+10+7)=72*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 4; //4
	//printf("Can BUS Run at 500Kpbs...");
#endif
#elif ((CAN_BTR==100))
	/*36000000/100000=360 =20*18=20*(1+15+2)*/
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_15tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler=20;
	//printf("Can BUS Run at 100Kpbs...");
#else	//(CAN_BTR==50)
	/*36000000/50000=720=45*(1+12+3)*/
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_12tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler=45;
	//printf("Can BUS Run at 50Kpbs...");
#endif
	CAN_Init(CAN1, &CAN_InitStructure);

	CAN_Filter_Config();

#ifdef CAN1_NVIC

	CAN1_NVIC_Config();
	CAN1_IT_Config();
#endif	

}
uint8_t TransmitMailbox;
void CAN1_Transmit(){
	CanTxMsg TxMessage;
	//TxMessage.StdId = 0x11;
			TxMessage.ExtId = 11212;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.DLC = 8;
			TxMessage.Data[0] = 0xaa;
			TxMessage.Data[1] = 0xbb;
			TxMessage.Data[2] = 0xcc;
			TxMessage.Data[3] = 0xdd;
			TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
}
void CAN_main(void) {
	KeyStatus NewKeyStaus, OldKeyStatus;
	CanTxMsg TxMessage;
	uint8_t TransmitMailbox, i;
#ifdef DEBUG
	debug();
#endif

	CAN1_Config();

	/*config TJA1054 */
	GPIO_SetBits(GPIOE, TJA1054AT_STB); //GPIO_Pin_1);//STB
	GPIO_ResetBits(GPIOE, TJA1054AT_EN); //EN
	GPIO_SetBits(GPIOE, TJA1054AT_STB); //GPIO_Pin_1);//STB
	GPIO_SetBits(GPIOE, TJA1054AT_EN); //EN

	/* CAN transmit at 100Kb/s and receive by polling in loopback mode */

	//printf("    ***** Can Demo Start..... ****");
	while (1) {
#ifdef Test_Tx
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == Bit_RESET) {
			NewKeyStaus = KeyPressed;
		} else {
			NewKeyStaus = KeyRelease;
		}
		if (1)
		if((NewKeyStaus == KeyRelease) && (OldKeyStatus == KeyPressed))
		{
			i = 0;
			TxMessage.StdId = 0x11;
			TxMessage.ExtId = 0x223344;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.DLC = 4;
			TxMessage.Data[0] = 0xaa;
			TxMessage.Data[1] = 0xbb;
			TxMessage.Data[2] = 0xcc;
			TxMessage.Data[3] = 0xdd;
			TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
			while (1) {
				if (CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK)
				{
					i++;
					printf("Send Message Pending");
				} else {
					printf("Send Message OK");
					i = 0xfd;
				}
				if (i == 0xfd) {
					printf("Send Message Fail");
					break;
				}
			} //end while(1)

			/*LED ON-OFF*/
			if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6) != RESET) {
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			} else {
				GPIO_SetBits(GPIOC, GPIO_Pin_6);
			}

		}
		OldKeyStatus = NewKeyStaus;
#endif  // endif Test_tx
#ifdef Test_Rx

#ifdef Rx_Polling
		if (CAN_CellResetFlag == RESET) {
			CAN_ITConfig(
					CAN1,
					CAN_IT_SLK | CAN_IT_WKU | CAN_IT_ERR | CAN_IT_LEC
							| CAN_IT_BOF | CAN_IT_EPV | CAN_IT_EWG, ENABLE);
			CAN_CellResetFlag = SET;
		}

		if (CAN_Polling() == PASSED) {
			printf("Polling Recv Message OK");
		}
#endif  //endif Rx_Polling
#ifdef Rx_Interrupt
		if(CAN_CellResetFlag == RESET)
		{
			CAN_Interrupt();
			printf("Waitting for Interrupt coming.....");
			CAN_CellResetFlag = SET;
		}
#endif	//endif Test_rx
#endif

	}
//return 0;
}

/*******************************************************************************
 * Function Name  : CAN_Polling
 * Description    : Configures the CAN, transmit and receive by polling
 * Input          : None
 * Output         : None
 * Return         : PASSED if the reception is well done, FAILED in other case
 *******************************************************************************/
TestStatus CAN_Polling(void) {
	CanRxMsg RxMessage;
	u32 i = 0;
	i = 0;

	while ((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF)) {
		i++;
	}
	if (i != 0xFF) /*说明有收到报文*/
	{
		/* receive */
		RxMessage.StdId = 0x00;
		RxMessage.IDE = CAN_ID_STD;
		RxMessage.DLC = 0;
		RxMessage.Data[0] = 0x00;
		RxMessage.Data[1] = 0x00;
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		//printf("Recv a Message...");
		return PASSED;
	} else {
		return FAILED;
	}
}

/*******************************************************************************
 * Function Name  : USB_HP_CAN_TX_IRQHandler
 * Description    : This function handles USB High Priority or CAN TX interrupts 
 *                  requests.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
//─ 发送邮箱0变为空，CAN_TSR寄存器的RQCP0位被置’1’。
//─ 发送邮箱1变为空，CAN_TSR寄存器的RQCP1位被置’1’。
//─ 发送邮箱2变为空，CAN_TSR寄存器的RQCP2位被置’1’。
void USB_HP_CAN1_TX_IRQHandler(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_RQCP0)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_RQCP0);
		
	printf("USB_HP_CAN_TX_IRQHandler TransmitMailbox 0\n");
	}else
	if (CAN_GetITStatus(CAN1, CAN_IT_RQCP1)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_RQCP1);
		
	printf("USB_HP_CAN_TX_IRQHandler TransmitMailbox 1\n");
	}else
	if (CAN_GetITStatus(CAN1, CAN_IT_RQCP2)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_RQCP2);
		
	printf("USB_HP_CAN_TX_IRQHandler TransmitMailbox 2\n");
	}else
	if (CAN_GetITStatus(CAN1, CAN_IT_TME)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
		
	printf("USB_HP_CAN_TX_IRQHandler CAN_IT_TME");
	}

	//printf("USB_HP_CAN_TX_IRQHandler \n");
}

/*******************************************************************************
 * Function Name  : USB_LP_CAN_RX0_IRQHandler
 * Description    : This function handles USB Low Priority or CAN RX0 interrupts 
 *                  requests.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
CanRxMsg RxMessage;
void USB_LP_CAN1_RX0_IRQHandler(void) {

	/*receive FIFO0 Message */
	if (CAN_GetITStatus(CAN1, CAN_IT_FOV0)) {
		//缓冲区溢出		
		CAN_ClearITPendingBit(CAN1, CAN_IT_FOV0);

	}else
	if (CAN_GetITStatus(CAN1, CAN_IT_FF0)) {
		//缓冲区满
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);

	}else 
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0)) {
		//缓冲区接收到数据		
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);

	}
	
	RxMessage.IDE = (uint8_t)0x04 & CAN1->sFIFOMailBox[CAN_FIFO0].RIR;
  if (RxMessage.IDE == CAN_Id_Standard)
  {
    RxMessage.StdId = (uint32_t)0x000007FF & (CAN1->sFIFOMailBox[CAN_FIFO0].RIR >> 21);
  }
  else
  {
    RxMessage.ExtId = (uint32_t)0x1FFFFFFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RIR >> 3);
  }
  
  /* Get the FMI */
  RxMessage.FMI = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDTR >> 8);
  /* Get the data field */
  RxMessage.Data[0] = (uint8_t)0xFF & CAN1->sFIFOMailBox[CAN_FIFO0].RDLR;
  RxMessage.Data[1] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDLR >> 8);
  RxMessage.Data[2] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDLR >> 16);
  RxMessage.Data[3] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDLR >> 24);
  RxMessage.Data[4] = (uint8_t)0xFF & CAN1->sFIFOMailBox[CAN_FIFO0].RDHR;
  RxMessage.Data[5] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDHR >> 8);
  RxMessage.Data[6] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDHR >> 16);
  RxMessage.Data[7] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RDHR >> 24);
  
	/* Release the FIFO0 */
	CAN1->RF0R |= CAN_RF0R_RFOM0;
	
	printf("USB_LP_CAN_RX0_IRQHandler 0x%x\n",RxMessage.ExtId);
}

/*******************************************************************************
 * Function Name  : CAN_RX1_IRQHandler
 * Description    : This function handles CAN RX1 interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CAN1_RX1_IRQHandler(void) {
//		CanRxMsg RxMessage;
	if (CAN_GetITStatus(CAN1, CAN_IT_FOV1)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FOV1);
	}
	if (CAN_GetITStatus(CAN1, CAN_IT_FF1)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF1);
	}
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}
	/*To DO: receive FIFO1 Message */
	{
		//CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
		
		
		RxMessage.IDE = (uint8_t)0x04 & CAN1->sFIFOMailBox[CAN_FIFO1].RIR;
  if (RxMessage.IDE == CAN_Id_Standard)
  {
    RxMessage.StdId = (uint32_t)0x000007FF & (CAN1->sFIFOMailBox[CAN_FIFO1].RIR >> 21);
  }
  else
  {
    RxMessage.ExtId = (uint32_t)0x1FFFFFFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RIR >> 3);
  }
  
  /* Get the FMI */
  RxMessage.FMI = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDTR >> 8);
  /* Get the data field */
  RxMessage.Data[0] = (uint8_t)0xFF & CAN1->sFIFOMailBox[CAN_FIFO1].RDLR;
  RxMessage.Data[1] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDLR >> 8);
  RxMessage.Data[2] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDLR >> 16);
  RxMessage.Data[3] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDLR >> 24);
  RxMessage.Data[4] = (uint8_t)0xFF & CAN1->sFIFOMailBox[CAN_FIFO1].RDHR;
  RxMessage.Data[5] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDHR >> 8);
  RxMessage.Data[6] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDHR >> 16);
  RxMessage.Data[7] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[CAN_FIFO1].RDHR >> 24);
		
		/* Release the FIFO1 */
		CAN1->RF1R |= CAN_RF1R_RFOM1;
	}

	printf("CAN1_RX1_IRQHandler 0x%x\n",RxMessage.ExtId);

}

/*******************************************************************************
 * Function Name  : CAN_SCE_IRQHandler
 * Description    : This function handles CAN SCE interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CAN1_SCE_IRQHandler(void) {
	/* 唤醒中断挂号*/
	if (CAN_GetITStatus(CAN1, CAN_IT_WKU)) {
		CAN_ClearITPendingBit(CAN1,CAN_IT_WKU);
	}
	/*睡眠确认中断状态*/
	if (CAN_GetITStatus(CAN1, CAN_IT_SLK)) {
		CAN_ClearITPendingBit(CAN1,CAN_IT_SLK);
	}
	/*离线中断*/
	if (CAN_GetITStatus(CAN1, CAN_IT_BOF)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);
		CAN_ClearFlag(CAN1, CAN_FLAG_BOF);
	}
	/* 错误被动标志，错误超过阀值*/
	if (CAN_GetITStatus(CAN1, CAN_IT_EPV)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_EPV);
		CAN_ClearFlag(CAN1, CAN_FLAG_EPV);
	}
	/* 错误警告标志，达到警告*/
	if (CAN_GetITStatus(CAN1, CAN_IT_EWG)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_EWG);
		CAN_ClearFlag(CAN1, CAN_FLAG_EWG);
	}
	/*Reset Can Cell */
	{
		//CAN1_Config();

#if 0
		CAN_Interrupt(); /*如果是中断方式*/
#else
		CAN_ITConfig(
				CAN1,
				CAN_IT_SLK | CAN_IT_WKU | CAN_IT_ERR | CAN_IT_LEC | CAN_IT_BOF
						| CAN_IT_EPV | CAN_IT_EWG, ENABLE);
#endif
	}
	//printf("CAN_SCE_IRQHandler,Try to Reset Can Cell");
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
