#include "stm32f10x.h"
#include "stdio.h"
#include "../../Source/System/USART/usart1.h"
#include "../../Source/System/Delay/systick.h"
#include "../../Source/System/CAN/can1.h"
typedef enum {FAILED = 0, PASSED = !FAILED}TestStatus;
typedef enum {KeyPressed=0,KeyRelease = !KeyPressed}KeyStatus;
uint32_t ret = 0; /* for return of the interrupt handling */
ErrorStatus HSEStartUpStatus;
uint8_t CAN_CellResetFlag;
#define TJA1054AT_STB  GPIO_Pin_8
#define TJA1054AT_EN   GPIO_Pin_9
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
	if (i != 0xFF) /*???????*/
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
void CAN_main(void) {
	KeyStatus NewKeyStaus, OldKeyStatus;
	CanTxMsg TxMessage;
	uint8_t TransmitMailbox, i;
#ifdef DEBUG
	debug();
#endif

	CAN1_Init();

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
int main(){
	USART1_NVIC_Config(7);
	USART1_Init();
	printf("hello");
	CAN1_Init();
		while(1)
	{
		Delay_ms(1000);
	CAN1_Transmit();
	};
}


