#include "stm32f10x.h"
#include "config.h"
#include "hardware.h"

#define tick_second 1
#define MAX_TIMES_CHECK_FRAME 50
//指令接收表
//struct msgStu netRecvDataCMDS[RECV_CMDS_NUM];

extern uint8_t ch_status[MAX_SOCK_NUM];
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def;
extern uint32_t presentTime;
extern uint32_t keep_alive_time;
extern uint32_t cruise_interval;
extern uint32_t net_check_time;


uint16_t any_port = 1000;


//u8 cmd1[32];
u8 tcp_flag=0;
u8 gprs_flag=0;
u32 arming;
u8 is_net_loss;

u32 in[16];
u16 out[16];
u8 xor[16];
u8 group;

u8 cell[5][12];
u8 cell_group;
u8 cell_xor;

u8 text[5][12];
u8 text_group;
u8 text_content[5][140];

typedef struct
{
	u8 head;
	u8 len;
	u8 cmd;
	u8 checkout;
}FRAME;

void parse_cmd_item(uint8_t * buf,uint8_t count)
{
	switch (buf[0])
	{
		case CHECK_CONFIG:
		{
			uint8_t buf[21] = {0xE0,0x55,0xAA,0,0,11,CHECK_CONFIG};
			uint8_t startNr = 7,i = 0;
			buf[startNr++] = Conf_Dat.Stu_Dat.is_off_line_mode;
			buf[startNr++] = Conf_Dat.Stu_Dat.rs232_bind_card_nr;
			buf[startNr++] = Conf_Dat.Stu_Dat.rs232_bind_key_nr;
			for (; i < 7; i++)
			{
				buf[startNr++] = Conf_Dat.Stu_Dat.card_reader_bind_induction_car_nr[i];
			}

			send(SER_SOCKET, buf, 21,0);
		}
		break;
		// E0 55 AA 00 00 2/*len*/ OPENTHEDOOR/*protocol*/ nr 00 00 00 00
		case OPEN_THE_DOOR://抬杆
		{
			uint8_t nr = buf[1];
			switch (buf[1])
			{
				case 4:
				case 1:
				case 2:
				case 3:
					flag_Line_Relay_ON[buf[1] - 1] = 2;
				break;
				default:
				break;
			}
		}
		break;
		case CLOSE_THE_DOOR://落杆
			LED4 = 1;
			break;
		case RS232_BIND_KEY_NR:
		{
			if (buf[1] < 9) Conf_Dat.Stu_Dat.rs232_bind_key_nr = buf[1];
				save_conf();
		}
			break;
		case RS232_BIND_CARD_NR://配置吐卡机使用第nr路wigeng读卡器
		{
			if (buf[1] < 8)	Conf_Dat.Stu_Dat.rs232_bind_card_nr = buf[1];
				save_conf();
		}
			break;
		case CARD_BIND_INDUCTION_CAR_NR://配置读卡器绑定第nr路输入作为地感
		{
			if (buf[1] > 0 && buf[1] < 8)
			{
				if (buf[2] >= 0 && buf[2] < 9)
				{
					Conf_Dat.Stu_Dat.card_reader_bind_induction_car_nr[buf[1] - 1] = buf[2];
					save_conf();
				}
			}
		}
			break;
		case SET_CARD_OFF_LINE_MODE:
		{
			if (buf[1] < 2)	Conf_Dat.Stu_Dat.is_off_line_mode = buf[1];
				save_conf();
		}
			break;
		case CARD_BIND_BAR:
		{
			if (buf[1] < 8 && buf[1] > 0)
			{
				if (buf[2] >= 0 && buf[2] < 5)
				Conf_Dat.Stu_Dat.card_bind_bar[buf[1] - 1] = buf[2];
			}
		}
			break;
		case CARD_REGIST:
		{
			add_new_card(&buf[1]);
		}
			break;
		case CARD_SEARCH:
		{
			uint8_t dat[27] = { 0xE0, 0x55, 0xaa, 0, 0, 17, CARD_SEARCH };
			uint16_t lst_nr = 0xFF;
			if (count == 5)
			{
				lst_nr = get_card_lst_nr_by_cardId(&buf[1]);
			}
			else if (count == 10)
			{
				lst_nr = get_card_lst_nr_by_humanId(&buf[1]);
			}
			else
			{
				dat[5] = 2;
				dat[7] = 2;//参数错误
				w5500_send(dat, 12);
				break;
			}

			if (lst_nr >= MAX_NUM_CARD_DOWNLOAD)
			{
				dat[5] = 2;
				dat[7] = 0;//当前卡不存在
				w5500_send(dat, 12);
			}
			else
			{
				get_card_Msg_by_lst_nr(lst_nr, &dat[7]);
				w5500_send(dat, 27);
			}
			
		}
			break;
		case CARD_DELETE:
		{
			uint8_t dat[12] = { 0xE0, 0x55, 0xaa, 0, 0, 2, CARD_DELETE, 0, 0, 0, 0, 0 };
			uint16_t lst_nr = 0xFFFF;
			if (count == 5)
			{
				lst_nr = get_card_lst_nr_by_cardId(&buf[1]);
			}
			else if (count == 10)
			{
				lst_nr = get_card_lst_nr_by_humanId(&buf[1]);
			}
			else
			{
				dat[7] = 0;//参数错误
				w5500_send(dat, 12);
				break;
			}

			if (lst_nr >= MAX_NUM_CARD_DOWNLOAD)
			{
				dat[7] = 2;//当前卡不存在
			}
			else
			{
				uint16_t tmp_lst_nr = 0;

				delete_a_card_by_lst_nr(lst_nr);
				if (count == 5)
				{
					tmp_lst_nr = get_card_lst_nr_by_cardId(&buf[1]);
				}
				else if (count == 10)
				{
					tmp_lst_nr = get_card_lst_nr_by_humanId(&buf[1]);
				}
				if (lst_nr != tmp_lst_nr)
					dat[7] = 1;//成功删除一张卡
				else
					dat[7] = 3;//删除失败
			}
			w5500_send(dat, 12);
		}
			break;
		case CARD_DELETE_ALL:
		{
			uint8_t dat[11] = { 0xE0, 0x55, 0xaa, 0, 0, 1, CARD_DELETE_ALL,  0, 0, 0, 0 };
			delete_all_cards();
			w5500_send(dat, 11);
		}
			break;
		case CARD_SEARCH_ALL:
		{
			get_All_card_Msg(0);
		}
			break;
		case CARD_SEC_CLR:
		{
			//	sect_clr(4096 * buf[1]);
		}
			break;
		case 0x64:
		{
					 uint32_t i = 0,crc_calc = 0;
					 uint8_t buf[] = {0xE0 ,0x55,0xAA,0,0, 5,0x64,0,0,0,0 ,0,0,0,0};
					 CRC_ResetDR();
					 for (i = 0; i < count - 1; i++)
					 {
						 CRC_CalcCRC((uint32_t)buf[i]);
					 }
					 crc_calc = CRC_GetCRC();
					 buf[7] = (crc_calc >> 24) & 0xff;
					 buf[8] = (crc_calc >> 16) & 0xff;
					 buf[9] = (crc_calc >> 8) & 0xff;
					 buf[10] = crc_calc & 0xff;
					 w5500_send(buf, 15);
		}
			break;
		default:
			break;
	}
}

void parse_buf_to_cmd_lst(SOCKET s)
{
	uint8_t * data_buf = TX_BUF[s];
	uint8_t cmdLen = 0;
	while ((BUF_WRITE[s] + 256 - BUF_READ[s]) % 256 > 4)
	{
		if (data_buf[BUF_READ[s]] == 0xE0 && data_buf[(uint8_t)(BUF_READ[s] + 1)] == 0x55 && data_buf[(uint8_t)(BUF_READ[s] + 2)] == 0xaa)
		{
			cmdLen = data_buf[(uint8_t)(BUF_READ[s] + 5)];
			if ((BUF_WRITE[s] + 256 - BUF_READ[s]) % 256 >= cmdLen + 10)
			{
				uint8_t lstBuf[256] = { 0 };
				uint32_t crc_stm32 = 0,crc_buf = 0;
				uint8_t * pCrcData = &data_buf[(uint8_t)(BUF_READ[s] + 6 + cmdLen)];
				int i = 0;
				BUF_READ[s] += 6;
				for (; i < cmdLen + 4; i++)
				{
					lstBuf[i] = data_buf[BUF_READ[s]++];
				}
				
				CRC_ResetDR();
				for (i = 0; i < cmdLen + 6; i++)
				{
						CRC_CalcCRC((uint32_t)data_buf[(uint8_t)(BUF_READ[s] + i)]);
				}
				crc_buf = (pCrcData[0] << 24) | (pCrcData[1] << 16) | (pCrcData[2] << 8) | (pCrcData[3]);
				//if (crc_buf == CRC_GetCRC())
				//{
						parse_cmd_item(lstBuf, cmdLen);
				//}

				//if (parseCmdHandle != null)
				//	parseCmdHandle(lstBuf, cmdLen);
				//parseCMD(lstBuf, cmdLen);

				//BUF_READ[s] += cmdLen + 4;
			}
			else
			{
				break;
			}
		}
		else
		{
			BUF_READ[s]++;
		}
	}


}

void loopback_tcps(SOCKET s, uint16_t port)
{
	uint16_t RSR_len;
	uint16_t received_len;
	uint8_t * data_buf = TX_BUF[s];
	uint8_t buf[256];
	int i = 0;


	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:              /* if connection is established */
		if (ch_status[s] == 1)
		{
			printf("\r\n%d : Connected", s);
			//printf("\r\n Peer IP : %d.%d.%d.%d", IINCHIP_READ(Sn_DIPR0(s)), IINCHIP_READ(Sn_DIPR1(s)), IINCHIP_READ(Sn_DIPR2(s)), IINCHIP_READ(Sn_DIPR3(s)));
			//printf("\r\n Peer Port : %d", ((uint16_t)(IINCHIP_READ(Sn_DPORT0(s))) << 8) + (uint16_t)IINCHIP_READ(Sn_DPORT1(s)));
			ch_status[s] = 2;
			LED4 = 0;
		}

		if ((RSR_len = getSn_RX_RSR(s)) > 0)        /* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */                                                                           /* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, buf, RSR_len);      /* read the received data */
			//send(s, data_buf, received_len, (Boolean)0);         /* sent the received data */
			for (; i < received_len; i++)
			{
				data_buf[BUF_WRITE[s]++] = buf[i];
				//BUF_WRITE[s]++;
			}
			parse_buf_to_cmd_lst(s);
		}
		parse_buf_to_cmd_lst(s);
		break;
	case SOCK_CLOSE_WAIT:                              /* If the client request to close */
		///printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0)     /* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;  /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */                                                                     /* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, data_buf, RSR_len);     /* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED:                                       /* if a socket is closed */
		if (!ch_status[s])
		{
			//printf("\r\n%d : Loop-Back TCP Server Started. port : %d", s, port);
			ch_status[s] = 1;
			LED4 = 1;
		}
		if (socket(s, (Sn_MR_ND | Sn_MR_TCP), port, 0x00) == 0)    /* reinitialize the socket */
		{
			//printf("\r\n%d : Fail to create socket.", s);
			ch_status[s] = 0;
		}
		break;
	case SOCK_INIT:   /* if a socket is initiated */

		listen(s);
		//printf("\r\n%x :LISTEN socket %d ", getSn_SR(s), s);
		break;
	default:
		break;
	}
}

void loopback_tcpc(SOCKET s,uint8_t * destIp, uint16_t destPort)
{
	uint16_t RSR_len = 0;
	uint16_t received_len;
	uint8_t * data_buf = TX_BUF[s];
	uint8_t buf[256];
	int i = 0;

	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:                 /* if connection is established */
		if (ch_status[s] == 1)
		{
			//printf("\r\n%d : Connected", s);
			ch_status[s] = 2;
			LED3 = 0;
		}

       
	   if ((RSR_len = getSn_RX_RSR(s)) > 0)        /* check Rx data */
	   {
		   if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */                                                                           /* the data size to read is MAX_BUF_SIZE. */
		   received_len = recv(s, buf, RSR_len);      /* read the received data */
		   for (; i < received_len; i++)
		   {
			   data_buf[BUF_WRITE[s]++] = buf[i];
		   }
		   parse_buf_to_cmd_lst(s);
	   }

		break;
	case SOCK_CLOSE_WAIT:                                 /* If the client request to close */
		//printf("\r\n%d : CLOSE_WAIT", s);
		//if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
		//{
		//	if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
		//	/* the data size to read is MAX_BUF_SIZE. */
		//	received_len = recv(s, data_buf, RSR_len);         /* read the received data */
		//}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED:                                               /* if a socket is closed */
		if (!ch_status[s])
		{
			//printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
			ch_status[s] = 1;
			LED3 = 1;
		}
		if (socket(s, Sn_MR_TCP, any_port++, 0x00) == 0)    /* reinitialize the socket */
		{
			//printf("\a%d : Fail to create socket.", s);
			ch_status[s] = 0;
		}
		break;
	case SOCK_INIT:     /* if a socket is initiated */
		if (time_return() - presentTime >= (tick_second * 3)) {  /* For TCP client's connection request delay : 3 sec */
			connect(s, destIp, destPort); /* Try to connect to TCP server(Socket, DestIP, DestPort) */
			//connect(s, Chconfig_Type_Def.destip, Chconfig_Type_Def.port); /* Try to connect to TCP server(Socket, DestIP, DestPort) */
			presentTime = time_return();
		}
		break;
	default:
		break;
	}
}

void w5500_send(const uint8_t * buf, uint16_t len)
{
	SOCKET s = SER_SOCKET;
	if (getSn_SR(s) == SOCK_ESTABLISHED)
	{
		send(s, buf, len, 0);
	}
}




