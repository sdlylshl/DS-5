#include "hardware.h"
#define CUR_CARD_DAT_LEN 16

//uint8_t signal_buf[20] = { 0xE0, 0x55, 0xAA, 0, 0, 0/*len*/, CARD_SIGNAL_MSG, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


Stu_Card_Signal stu_card_signal ; 
uint16_t get_card_lst_nr_by_cardId(uint8_t cardId[]);
uint16_t get_card_lst_nr_by_humanId(uint8_t human[]);
uint16_t get_an_empty_card_lst_nr();

void write_to_cardMachine(uint8_t * buf,uint8_t len)
{
	int i = 0;
	for (; i < len; i++)
	{
		UART5_Put_Char(buf[i]);
	}
}


/****************************************************************************
* 名    称：void send_card_to_machine_mouth()
* 功    能：发卡至读卡位置或者从读卡器位置发送至卡嘴
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void send_card_to_machine_mouth()
{
	uint8_t buf[] = { 0x02, 0x44, 0x43, 0x03, 0x06 };
	write_to_cardMachine(buf, 5);
}

/****************************************************************************
* 名    称：uint8_t get_Key_Card(uint8_t nr)
* 功    能：读取指定端口处的是否有按键
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
uint8_t get_Key_Card(uint8_t nr)
{
	if (nr == 0) return 0;
	return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_Input_Array[nr]);
}


uint8_t get_test_Key_Card()
{
	return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15);
	//return PDin(15);
}

/****************************************************************************
* 名    称：uint8_t get_Induction_Car(uint8_t nr)
* 功    能：读取指定端口处的地感信号值
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
uint8_t get_Induction_Car(uint8_t nr)
{
	if (nr == 0) return 0;
	return !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_Input_Array[nr]);
}

/****************************************************************************
* 名    称：void key_scan()
* 功    能：取卡按键检测
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/

void key_scan()
{
	uint8_t key_card = 0;
	uint8_t induction_car = 0;
	if (Conf_Dat.Stu_Dat.rs232_bind_card_nr != 0)
	{
		induction_car = flag_get_Induction_Car[Conf_Dat.Stu_Dat.rs232_bind_card_nr - 1];
	}
	
	if (induction_car)
	{
		key_card = get_Key_Card(Conf_Dat.Stu_Dat.rs232_bind_key_nr);
		if (key_card == 1 && flag_get_key_Card == 0)
		{
			if (flag_null_key_Card >= 2)
			{
				send_card_to_machine_mouth();
				flag_null_key_Card = 0;
			}
			flag_get_key_Card = 1;
			flag_null_key_Card = 0;
		}
	}


	//if (get_test_Key_Card() == 1 && flag_test_get_key_Card == 0)
	//{
	//	if (flag_test_null_key_Card >= 2)
	//	{
	//		//0x02 0x43 0x50 0x03 0x12
	//		//send_check_card_machine_state_cmd();
	//		uint8_t buf[] = { 0x02, 0x43, 0x50, 0x03, 0x12 };
	//		write_to_cardMachine(buf, 5);
	//		flag_test_null_key_Card = 0;
	//		check_test_card_state = TRUE;
	//	}
	//	flag_test_get_key_Card = 1;
	//	flag_test_null_key_Card = 0;
	//}
}

/****************************************************************************
* 名    称：void send_card_nr_to_server(uint8_t nr,uint8_t dat[])
* 功    能：非离线模式下将卡号发送给pc端，离线模式下匹配卡号以后，决定是否抬杆
* 入口参数：nr 通道号 dat 卡号  uint8_t dat[4];
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void send_card_nr_to_server(uint8_t nr,uint8_t dat[])
{
	uint8_t startNr = 6;
	uint8_t buf_send[] = { 0xE0, 0x55, 0xAA, 0, 0, 6/*len*/, 0/*protocol*/, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	if (Conf_Dat.Stu_Dat.is_off_line_mode)
	{
		if (check_card_exist(dat))
		{
			if (Conf_Dat.Stu_Dat.card_bind_bar[nr - 1] > 0)
				flag_Line_Relay_ON[Conf_Dat.Stu_Dat.card_bind_bar[nr - 1] - 1] = 2;
		}
	}
	else
	{
		buf_send[startNr++] = READ_CARD_VALUE;

		buf_send[startNr++] = nr;
		buf_send[startNr++] = dat[0];
		buf_send[startNr++] = dat[1];
		buf_send[startNr++] = dat[2];
		buf_send[startNr++] = dat[3];
		w5500_send(buf_send, 16);
	}

	UART1_Put_Char(dat[0]);
	UART1_Put_Char(dat[1]);
	UART1_Put_Char(dat[2]);
	UART1_Put_Char(dat[3]);

}

/****************************************************************************
* 名    称：void send_card_recycling_msg()
* 功    能：发送卡回收消息给pc端
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void send_card_recycling_msg()
{
	uint8_t buf_send[] = { 0xE0, 0x55, 0xAA, 0, 0, 1/*len*/, (uint8_t)CARD_RECYCLING/*protocol*/, 0, 0, 0, 0 };
	w5500_send( buf_send, 11);
}

uint32_t Induction_Car_Time =  0 ;
uint8_t dat_Induction_Car[] = { 0xE0, 0x55, 0xAA, 0, 0, 3, INDUCTION_CAR_VALUE_TRUE, 0/*nr读卡器通道编号*/, 1/*表示检测到地感还是地感消失*/, 0/*sn*/, 0, 0, 0, 0 };
/****************************************************************************
* 名    称：void check_Induction_Car()
* 功    能：检查并发送地感信号
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void check_Induction_Car() 
{
	uint32_t curTime = get_curTime();
	int i = 0;
	if (curTime - Induction_Car_Time > 5)
	{
		for (; i < 7; i++)
		{
			dat_Induction_Car[7] = i + 1;
			if (Conf_Dat.Stu_Dat.card_reader_bind_induction_car_nr[i] != 0)
			{
				uint8_t induction_car = get_Induction_Car(Conf_Dat.Stu_Dat.card_reader_bind_induction_car_nr[i]);
				if (flag_get_Induction_Car[i] != induction_car)
				{
					if (induction_car)
					{
						dat_Induction_Car[8] = 1;
					}
					else
					{
						dat_Induction_Car[8] = 0;
						sn_Induction_Car++;
					}
					dat_Induction_Car[9] = sn_Induction_Car;
					w5500_send(dat_Induction_Car, 13);
					flag_get_Induction_Car[i] = induction_car;
				}

				if (flag_get_Induction_Car[i])
				{
					w5500_send( dat_Induction_Car, 13);
				}
			}
		}
		Induction_Car_Time = curTime;

	}
	


}

void send_check_card_machine_state_cmd()
{
	uint8_t buf[] = { 0x02, 0x52, 0x46, 0x03, 0x15 };
	write_to_cardMachine(buf, 5);
}

uint32_t check_interval = 0;
void check_card_been_got_away_state()
{
	uint32_t tm = get_curTime();
	if (tm - check_interval > 3)
	{
		//if (flag_check_card_been_got_away_state)
		//{
			uint8_t buf[] = { 0x02, 0x52, 0x46, 0x03, 0x15 };
			write_to_cardMachine(buf, 5);
		//}
		check_interval = tm;
	}
}

Boolean humanId_cmp(uint8_t humanId_9[],uint8_t humanId_18[])
{

}

Boolean humanId_cpy(uint8_t humanId_9[], uint8_t humanId_18[])
{
	int i = 0;
	for (i = 0; i < 9; i++)
	{
		humanId_9[i] = (humanId_18[i * 2] << 4) | humanId_18[i * 2 + 1];
	}
}

void get_BSD_humanId(uint8_t humanId_9[], uint8_t humanId_18[])
{
	int i = 0;
	for (; i < 9; i++)
	{
		humanId_9[i] = (humanId_18[i * 2] << 4) | humanId_18[i * 2 + 1];
	}
}

void get_humanId_from_BSD(uint8_t humanId_18[], uint8_t humanId_9[])
{
	int i = 0;
	for (; i < 9; i++)
	{
		humanId_18[i * 2] = (humanId_9[i] >> 4) & 0x0f;
		humanId_18[i * 2 + 1] = humanId_9[i] & 0x0f;
	}
}

/****************************************************************************
* 名    称：void add_new_card(uint8_t human[], uint8_t cardId[], uint8_t tm[])
* 功    能：添加新卡
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void add_new_card(uint8_t card_dat[])
{
	uint8_t dat[28] = { 0xE0, 0x55, 0xAA, 0, 0, 18/*len*/, CARD_REGIST, 0 };

	if (cardNr == MAX_NUM_CARD_DOWNLOAD)
	{
		dat[7] = 2;//卡已存储满
	}
	else
	{
		int startNr = 0;
		uint8_t tmp_dat[CARD_MSG_SAVED_LEN];
		uint16_t lst_nr = get_an_empty_card_lst_nr();
		uint8_t tmp_buf[CARD_MSG_SAVED_LEN] = { 0 };
		SST25_W_BLOCK(CARD_MSG_START_ADDR + lst_nr * CARD_MSG_SAVED_LEN, card_dat, CARD_MSG_SAVED_LEN);

		SST25_R_BLOCK(CARD_MSG_START_ADDR + lst_nr * CARD_MSG_SAVED_LEN, tmp_buf, CARD_MSG_SAVED_LEN);
		if (memcmp(tmp_buf, card_dat, CUR_CARD_DAT_LEN) == 0)
		{
			cardNr++;
			dat[7] = 1;
			memcpy(&dat[8], card_dat, CUR_CARD_DAT_LEN);
		}
		else
		{
			dat[7] = 0;
		}
	}
	w5500_send(dat, 28);

}
/****************************************************************************
* 名    称：uint16_t search_a_card_by_humanId(uint8_t human[9])
* 功    能：通过身份证号查询卡的存储编号(lst_nr)
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
uint16_t get_card_lst_nr_by_humanId(uint8_t human[])
{
	uint16_t lst_nr = 0;
	uint8_t card_dat[CARD_MSG_SAVED_LEN];
	uint16_t used_card_num = 0;
	for (; lst_nr < MAX_NUM_CARD_DOWNLOAD; lst_nr++)
	{
		SST25_R_BLOCK(CARD_MSG_START_ADDR + lst_nr * CARD_MSG_SAVED_LEN, card_dat, CARD_MSG_SAVED_LEN);
		if (memcmp(human, card_dat, 9) == 0)
		{
			return lst_nr;
		}
		else
		{
			//if (card_dat[9] != 0xFF || card_dat[10] != 0xFF || card_dat[11] != 0xFF || card_dat[12] != 0xFF)
			//	used_card_num++;
			//if (used_card_num >= cardNr)
			//	break;
		}

	}

	return ERROR_CARD_NUM;
}
/****************************************************************************
* 名    称：uint16_t get_card_lst_nr_by_cardId(uint8_t cardId[4])
* 功    能：通过卡号查询卡的存储编号(lst_nr)
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
uint16_t get_card_lst_nr_by_cardId(uint8_t cardId[])
{
	uint16_t lst_nr = 0, j = 0;
	uint8_t card_dat[CARD_MSG_SAVED_LEN];
	uint8_t *p_cardId = &card_dat[9];
	uint16_t used_card_num = 0;
	if (cardId[0] == 0xFF && cardId[1] == 0xFF && cardId[2] == 0xFF && cardId[3] == 0xFF)
	{
		for (; lst_nr < MAX_NUM_CARD_DOWNLOAD; lst_nr++)
		{
			SST25_R_BLOCK(CARD_MSG_START_ADDR + lst_nr * CARD_MSG_SAVED_LEN, card_dat, CARD_MSG_SAVED_LEN);
			if (cardId[0] == p_cardId[0] && cardId[1] == p_cardId[1] && cardId[2] == p_cardId[2] && cardId[3] == p_cardId[3])
				return lst_nr;
		}

	}
	else
	{
		for (; lst_nr < MAX_NUM_CARD_DOWNLOAD; lst_nr++)
		{
			SST25_R_BLOCK(CARD_MSG_START_ADDR + lst_nr * CARD_MSG_SAVED_LEN, card_dat, CARD_MSG_SAVED_LEN);
			if (cardId[0] == p_cardId[0] && cardId[1] == p_cardId[1] && cardId[2] == p_cardId[2] && cardId[3] == p_cardId[3])
				return lst_nr;
			else
			{
				//if (p_cardId[0] != 0xFF || p_cardId[1] != 0xFF || p_cardId[2] != 0xFF || p_cardId[3] != 0xFF)
				//	used_card_num++;
				//if (used_card_num >= cardNr)
				//	break;
			}
		}
	}
	return ERROR_CARD_NUM;
}

Boolean check_card_exist(uint8_t cardId[])
{
	uint16_t lst_nr = get_card_lst_nr_by_cardId(cardId);
	if (lst_nr < MAX_NUM_CARD_DOWNLOAD)
		return TRUE;
	else
		return FALSE;
}
/****************************************************************************
* 名    称：void get_card_Msg_by_lst_nr(uint16_t nr,uint8_t cardId[4],uint8_t tm[3])
* 功    能：查找一块可以记录新卡信息的空间
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void get_card_Msg_by_lst_nr(uint16_t lst_nr,uint8_t card_dat[])
{
	if (lst_nr < MAX_NUM_CARD_DOWNLOAD)
	{
		SST25_R_BLOCK(CARD_MSG_START_ADDR + lst_nr * CARD_MSG_SAVED_LEN, card_dat, CUR_CARD_DAT_LEN);
	}
}

void get_All_card_Msg(uint16_t nr)
{
	uint8_t count = 0;
	uint16_t lst_nr = nr;
	uint8_t card_dat[CARD_MSG_SAVED_LEN];
	uint8_t *p_cardId = &card_dat[9];
	uint8_t dat[27] = { 0xE0, 0x55, 0xaa, 0, 0, 0, CARD_SEARCH_ALL };

	for (; lst_nr < MAX_NUM_CARD_DOWNLOAD; lst_nr++)
	{
		get_card_Msg_by_lst_nr(lst_nr, card_dat);
		if(p_cardId[0] != 0xFF || p_cardId[1] != 0xFF || p_cardId[2] != 0xFF || p_cardId[3] != 0xFF)
		{
			dat[5] = 17;
			memcpy(&dat[7],card_dat,16);
			w5500_send(dat, 27);
			count++;			
		}
		if (count == 10)
			break;
	}
	if (lst_nr == MAX_NUM_CARD_DOWNLOAD)
	{
		stu_card_signal.signal = SIGNAL_CARD_SIGNAL_NULL;
		stu_card_signal.param2 = 0;
		dat[5] = 1;
		w5500_send(dat, 11);
	}
	else
	{
		stu_card_signal.signal = SIGNAL_SEARCH_ALL;
		stu_card_signal.param2 = lst_nr + 1;
	}

}

/****************************************************************************
* 名    称：uint16_t get_an_empty_card_space()
* 功    能：查找一块可以记录新卡信息的空间
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
uint16_t get_an_empty_card_lst_nr()
{
	uint16_t i = 0;
	uint8_t tmp_card[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	return get_card_lst_nr_by_cardId(tmp_card);
}

void delete_a_card_by_cardId(uint8_t cardId[])
{
	uint16_t lst_nr = get_card_lst_nr_by_cardId(cardId);
	if (lst_nr < MAX_NUM_CARD_DOWNLOAD)
		delete_a_card_by_lst_nr(lst_nr);
}

void delete_a_card_by_lst_nr(uint16_t lst_nr)
{
	uint8_t card_dat[EEPROM_FLASH_SECTOR_LEN], i;
	uint32_t lstNrAddr = (lst_nr / CARDS_NUM_IN_A_SECTOR)*EEPROM_FLASH_SECTOR_LEN;
	SST25_R_BLOCK(lstNrAddr, card_dat, EEPROM_FLASH_SECTOR_LEN);
	for (i = 0; i < CARD_MSG_SAVED_LEN; i++)
	{
		card_dat[(lst_nr % CARDS_NUM_IN_A_SECTOR) * CARD_MSG_SAVED_LEN + i] = 0xff;
	}
	sect_clr(lstNrAddr);
	SST25_W_BLOCK(lstNrAddr, card_dat, EEPROM_FLASH_SECTOR_LEN);
}

void delete_a_card_by_humanId(uint8_t humanId[])
{
	uint16_t lst_nr = get_card_lst_nr_by_humanId(humanId);
	if (lst_nr < MAX_NUM_CARD_DOWNLOAD)
		delete_a_card_by_lst_nr(lst_nr);
}

void delete_all_cards()
{
	Bulk_clr();
}
//Boolean check_card_is_exist(uint8_t cardId[])
//{
//	int i = 0;
//	uint8_t tmp_card[4];
//
//	uint16_t lst_nr = get_card_lst_nr_by_cardId(cardId);
//	if (lst_nr == MAX_NUM_CARD_DOWNLOAD)
//		return FALSE;
//	else
//		return TRUE;
//}
void parse_card_signal()
{
	switch (stu_card_signal.signal)
	{
		//case SIGNAL_ADD_NEW_CARD_OK:
		//{
		//		signal_buf[5] = 2;
		//		signal_buf[6] = CARD_REGIST;
		//		signal_buf[7] = 1;
		//		w5500_send(signal_buf,12);
		//		stu_card_signal.signal = SIGNAL_CARD_SIGNAL_NULL;
		//}
		//	break;
		//case SIGNAL_ADD_NEW_CARD_ERROR:
		//{
		//		signal_buf[5] = 3;
		//		signal_buf[6] = CARD_REGIST;
		//		signal_buf[7] = 0;
		//		signal_buf[8] = stu_card_signal.param1;
		//		w5500_send(signal_buf, 13);
		//		stu_card_signal.signal = SIGNAL_CARD_SIGNAL_NULL;
		//}
		//	break;
		case SIGNAL_SEARCH_ALL:
		{
			get_All_card_Msg(stu_card_signal.param2);
		}
			break;
		case SIGNAL_CARD_SIGNAL_NULL:
		default:
			break;
	}
}
