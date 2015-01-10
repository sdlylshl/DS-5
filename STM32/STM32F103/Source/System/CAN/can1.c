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
	/* Enable CAN RX interrupt IRQ channel */
	NVIC_SetPriority(CAN1_RX1_IRQn, 4);
	NVIC_EnableIRQ(CAN1_RX1_IRQn);

	/* Enable CAN Tx interrupt IRQ channel USB_HP_CAN_TX_IRQChannel */
	NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 5);
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);

	/* Enable CAN RX1 interrupt IRQ channel CAN_RX1_IRQChannel*/
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 6);
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

	/* Enable CAN ERR interrupt IRQ channel CAN_SCE_IRQChannel*/
	NVIC_SetPriority(CAN1_SCE_IRQn, 7);
	NVIC_EnableIRQ(CAN1_SCE_IRQn);
}
static void CAN1_IT_Config(void) {
	/*打开所有CAN相关的中断,如果你需要处理各种情况*/

	/*CAN TX INTERRUPT*/
	CAN_ITConfig(CAN1, CAN_IT_TME | CAN_IT_RQCP0 | CAN_IT_RQCP1 | CAN_IT_RQCP2
	, ENABLE);

	/*CAN FIFO0 INTERRUPT*/
	CAN_ITConfig(CAN1, CAN_IT_FF0 | CAN_IT_FMP0 | CAN_IT_FOV0 | CAN_IT_FMP0
	, ENABLE);

	/*CAN FIFO1 INTERRUPT*/
	CAN_ITConfig(CAN1, CAN_IT_FF1 | CAN_IT_FMP1 | CAN_IT_FOV1 | CAN_IT_FMP1
	, ENABLE);

	/*CAN ESR INTERRUPT,ERROR PROCESS*/
	CAN_ITConfig(CAN1,CAN_IT_SLK | CAN_IT_WKU | CAN_IT_ERR | CAN_IT_LEC | CAN_IT_BOF
					| CAN_IT_EPV | CAN_IT_EWG, ENABLE);

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
	CAN_InitStructure.CAN_AWUM = DISABLE;	/*自动唤醒模式	 0-由软件通过清0唤醒，1-检测到报文时，自动唤醒*/
	CAN_InitStructure.CAN_NART = DISABLE;	/*报文自动重传	 0-一直重复发送直到成功，1-不论成功以否只发送一次*/
	CAN_InitStructure.CAN_RFLM = ENABLE;	/*接收FIFO 锁定模式	0-溢出时未锁定，新报文盖掉掉报文，1-FIFO锁定，溢出后新报文直接丢失*/
	CAN_InitStructure.CAN_TXFP = DISABLE;	/*发送FIFO优先级		0-报文发送优先级由标志符决定，1-报文发送优先级由请求先后顺序决定*/
	/*模式-测试模式-正常模式*/
	//CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_Mode=CAN_MODE;

	/*=======================
	 BTR=1/(Tbit)=1/(1+Tbs1+Tbs2)*Tq)=1/((1+8+7)*Tq)=1/16Tq
	 Tq=Tpclk1*(prescale+1)=(prescale+1)/Fpclk1
	 BTR=1/(16*(prescale+1)/Fpclk1)=Fpclk1/(16*(prescale+1))
	 Precalse=(Fpclk1/(16*BTR))-1
	 ========================*/
#if (CAN_BTR==500)
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

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_CellResetFlag = RESET;

#ifdef CAN1_NVIC
	CAN1_NVIC_Config();
	CAN1_IT_Config();
#endif	

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
void USB_HP_CAN_TX_IRQHandle(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_RQCP0)) {

		CAN_ClearITPendingBit(CAN1, CAN_IT_RQCP0);
	}
	if (CAN_GetITStatus(CAN1, CAN_IT_RQCP1)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_RQCP1);
	}
	if (CAN_GetITStatus(CAN1, CAN_IT_RQCP2)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_RQCP2);
	}

	//printf("USB_HP_CAN_TX_IRQHandler");

}

/*******************************************************************************
 * Function Name  : USB_LP_CAN_RX0_IRQHandler
 * Description    : This function handles USB Low Priority or CAN RX0 interrupts 
 *                  requests.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_LP_CAN_RX0_IRQHandle(void) {
	CanRxMsg RxMessage;

	if (CAN_GetITStatus(CAN1, CAN_IT_FF0)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);/**/
	}
	if (CAN_GetITStatus(CAN1, CAN_IT_FOV0)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FOV0);
	}

	RxMessage.StdId = 0x00;
	RxMessage.ExtId = 0x00;
	RxMessage.IDE = 0;
	RxMessage.DLC = 0;
	RxMessage.FMI = 0;
	RxMessage.Data[0] = 0x00;
	RxMessage.Data[1] = 0x00;

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	if ((RxMessage.ExtId == 0x1234) && (RxMessage.IDE == CAN_ID_EXT)
			&& (RxMessage.DLC == 2)
			&& ((RxMessage.Data[1] | RxMessage.Data[0] << 8) == 0xDECA)) {
		ret = 1;
	} else {
		ret = 0;
	}
	//printf("USB_LP_CAN_RX0_IRQHandler (Interrupt Recv 1 Message)");
}

/*******************************************************************************
 * Function Name  : CAN_RX1_IRQHandler
 * Description    : This function handles CAN RX1 interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CAN_RX1_IRQHandle(void) {

	if (CAN_GetITStatus(CAN1, CAN_IT_FOV1)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FOV1);
	}
	if (CAN_GetITStatus(CAN1, CAN_IT_FF1)) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF1);
	}

	/*To DO: receive FIFO1 Message */
	{

	}

	//printf("CAN_RX1_IRQHandler");

}

/*******************************************************************************
 * Function Name  : CAN_SCE_IRQHandler
 * Description    : This function handles CAN SCE interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CAN_SCE_IRQHandle(void) {
	/* 唤醒中断挂号*/
	if (CAN_GetITStatus(CAN1, CAN_IT_WKU)) {
		//CAN_ClearITPendingBit(CAN_IT_WKU);
	}
	/*睡眠确认中断状态*/
	if (CAN_GetITStatus(CAN1, CAN_IT_SLK)) {
		//CAN_ClearITPendingBit(CAN_IT_SLK);
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
		CAN1_Config();

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