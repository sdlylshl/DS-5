#include "wigend.h"
#include "Types.h"
#include "LED.h"
#include "socket.h"
#include "usart.h"
//#include "cardMachine.h"

uint8_t dat_count[4] = {0};
uint8_t buf_dat[4][4] = {0};
Boolean flag_wigend_input[4] = {0};
uint32_t input_time_count[4] ;
Boolean flag_rs232_bind_wigen_card_nr[4];
uint8_t card_to_sending[4];
/****************************************************************************
* ��    �ƣ�void check_a_wingend_input_flag(uint8_t nr)
* ��    �ܣ�����nr·wigen�����źŵı�־
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�������
****************************************************************************/
void check_a_wingend_input_flag(uint8_t nr)
{
	if(flag_wigend_input[nr])
	{
		input_time_count[nr]++;
		if(input_time_count[nr] > 30)
		{
			input_time_count[nr] = 0;
			buf_dat[nr][0] = 0;   
			buf_dat[nr][1] = 0;   
			buf_dat[nr][2] = 0;   
			buf_dat[nr][3] = 0;   
			dat_count[nr] = 0;			
			flag_wigend_input[nr] = FALSE;
		}
	}
}

/****************************************************************************
* ��    �ƣ�void check_wigend_input_flag()
* ��    �ܣ�������������źŵı�־
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�������
****************************************************************************/
void check_wigend_input_flag()
{
  check_a_wingend_input_flag(0);
	check_a_wingend_input_flag(1);
	check_a_wingend_input_flag(2);
	check_a_wingend_input_flag(3);
}

uint8_t tmp[2][50] = {0};
uint8_t tmp_count = 0;
/****************************************************************************
* ��    �ƣ�void set_bit_to_value(uint8_t sn,uint8_t bit)
* ��    �ܣ�����buf_dat[sn]�ĵ�dat_count[sn]λΪbit
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�������
****************************************************************************/
void set_bit_to_value(uint8_t sn,uint8_t bit)
{
		dat_count[sn]++;
		flag_wigend_input[sn] = TRUE;
		if(dat_count[sn] == 1)
		{
//			buf_dat[sn][0] = 0;   
//			buf_dat[sn][1] = 0;   
//			buf_dat[sn][2] = 0;   
//			buf_dat[sn][3] = 0; 
		}
		else if(dat_count[sn] >= 34)
		{
			flag_wigend_input[sn] = FALSE;
			//dat_count[sn] = 0;
			//UART1_Put_Char(buf_dat[sn][(dat_count[sn] - 2) / 8]);
		}
		else if(dat_count[sn] > 1 )
		{
			if (bit == 1)
				buf_dat[sn][(dat_count[sn] - 2) / 8] |= 1 << (7 - (dat_count[sn] - 2) % 8);
			else
				buf_dat[sn][(dat_count[sn] - 2) / 8] &= ~(1 << (7 - (dat_count[sn] - 2) % 8));
			//UART1_Put_Char(bit);
			//UART1_Put_Char(buf_dat[sn][(dat_count[sn] - 2) / 8]);

		}


}


void EXTI9_5_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line8) == SET)
	{
		set_bit_to_value(0, 1);
		EXTI_ClearITPendingBit(EXTI_Line8);   //��� LINE �ϵ��жϱ�־λ
	}
	if (EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		set_bit_to_value(0, 0);
		EXTI_ClearITPendingBit(EXTI_Line9);   //��� LINE �ϵ��жϱ�־λ
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line10) == SET)
	{
		set_bit_to_value(1, 1);
		EXTI_ClearITPendingBit(EXTI_Line10);   //��� LINE �ϵ��жϱ�־λ
	}
	else if (EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		set_bit_to_value(1, 0);
		EXTI_ClearITPendingBit(EXTI_Line11);   //��� LINE �ϵ��жϱ�־λ
	}
	else if (EXTI_GetITStatus(EXTI_Line12) == SET)
	{
		set_bit_to_value(2, 1);
		EXTI_ClearITPendingBit(EXTI_Line12);   //��� LINE �ϵ��жϱ�־λ
	}
	else if (EXTI_GetITStatus(EXTI_Line13) == SET)
	{

		set_bit_to_value(2, 0);
		EXTI_ClearITPendingBit(EXTI_Line13);   //��� LINE �ϵ��жϱ�־λ
	}

	else if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		set_bit_to_value(3, 1);
		EXTI_ClearITPendingBit(EXTI_Line14);   //��� LINE �ϵ��жϱ�־λ
	}
	else if (EXTI_GetITStatus(EXTI_Line15) == SET)
	{
		set_bit_to_value(3, 0);
		EXTI_ClearITPendingBit(EXTI_Line15);   //��� LINE �ϵ��жϱ�־λ
	}
}

/****************************************************************************
* ��    �ƣ�void check_wigen_buf()
* ��    �ܣ�����wigen�������е�wigenֵ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�������
****************************************************************************/
void check_wigen_buf()
{
	uint8_t nr = 0xff;
	Boolean flag_get_all_data = FALSE;
	
	if(dat_count[0] == 34)
	{
		nr = 1;
		flag_get_all_data = TRUE;
	}

	if (dat_count[1] == 34)
	{
		nr = 2;
		flag_get_all_data = TRUE;
	}
	if (dat_count[2] == 34)
	{
		nr = 3;
		flag_get_all_data = TRUE;
	}

	if (dat_count[3] == 34)
	{
		nr = 4;
		flag_get_all_data = TRUE;
	}

	if (flag_get_all_data)
	{
		if (nr > 4)
			return;
		if (Conf_Dat.Stu_Dat.rs232_bind_card_nr == nr)
		{
			if (card_to_sending[0] != buf_dat[nr - 1][0] || card_to_sending[1] != buf_dat[nr - 1][1] || card_to_sending[2] != buf_dat[nr - 1][2] || card_to_sending[3] != buf_dat[nr - 1][3])
			{
				if (flag_get_Induction_Car[nr - 1])
				{
					card_to_sending[0] = buf_dat[nr - 1][0];
					card_to_sending[1] = buf_dat[nr - 1][1];
					card_to_sending[2] = buf_dat[nr - 1][2];
					card_to_sending[3] = buf_dat[nr - 1][3];
					send_card_to_machine_mouth();
					flag_check_card_been_got_away_state = TRUE;
				}
			}
			else
			{
				

			}
		}
		else
		{
			if (flag_get_Induction_Car[nr - 1])
			{
				send_card_nr_to_server(nr, buf_dat[nr - 1]);
			}
		}
		dat_count[nr - 1] = 0;

		UART1_Put_Char(buf_dat[nr - 1][0]);
		UART1_Put_Char(buf_dat[nr - 1][1]);
		UART1_Put_Char(buf_dat[nr - 1][2]);
		UART1_Put_Char(buf_dat[nr - 1][3]);
	}

}

