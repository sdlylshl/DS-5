/*
@file		type.h
*/

#ifndef _TYPE_H_
#define _TYPE_H_
#include "stdint.h"
#include <stdio.h>
#include "string.h"
#include "stm32f10x.h"
#include "config.h"

#define TX_RX_MAX_BUF_SIZE	256
#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket */
#define MAX_INSTRUCTION_LEN 100

#define IINCHIP_ISR_DISABLE()  __disable_fault_irq()
#define IINCHIP_ISR_ENABLE() __enable_fault_irq()

#ifndef NULL
#define NULL		((void *) 0)
#endif
typedef uint8_t SOCKET;
typedef enum { FALSE = 0, TRUE = !FALSE } Boolean;

typedef struct _CONFIG_MSG
{
	uint8_t Mac[6];
	uint8_t Lip[4];
	uint8_t Sub[4];
	uint8_t Gw[4];
	uint8_t DNS_Server_IP[4];
	uint8_t  DHCP;
	uint16_t any_port;
}CONFIG_MSG;


typedef struct _CONFIG_TYPE_DEF
{
	uint16_t port;
	uint8_t destip[4];
}CHCONFIG_TYPE_DEF;

typedef struct
{
	uint8_t link_group;
	uint8_t xor[16];
	uint16_t out[16];
	uint32_t in[16];
}link_para;


typedef struct
{
	uint8_t cell_group;
	uint8_t cell_xor;
	uint8_t cell[5][12];
}cell_para;


typedef struct
{
	uint8_t text_group;
	uint8_t text_length[5];
	uint8_t text[5][12];
	uint8_t text_content[5][140];
	uint8_t send_ok[5];
	uint32_t time[5];

}text_para;



typedef struct
{
	uint8_t tcp_flag;
	uint8_t gprs_flag;
	uint8_t is_net_loss;
	uint8_t cell_alarm;
	uint8_t text_alarm;
	uint8_t gsm_init_flag;
	uint32_t arming;
	uint32_t machine_id;
	CONFIG_MSG net_para;
	CHCONFIG_TYPE_DEF dest_net_para;
	link_para link;
	cell_para cell;
	text_para text;
} sys_para;

typedef union CONFDAT
{
	struct STUDAT
	{
		uint8_t wigen_count[4];    //wigen�ź�Ϊ 26 34
		Boolean is_off_line_mode;   //�����Ƿ�Ϊ����ģʽ
		uint8_t rs232_bind_card_nr; //��nr·wigen������������Ϊ�¿���������
		//uint8_t rs232_bind_rs485_card_nr; //��nr·wigen������������Ϊ�¿���������
		CONFIG_MSG Config_Msg;
		uint8_t dest_ip[8][4];
		uint16_t dest_port[8];
		uint8_t rs232_bind_key_nr; //����ʹ�õ�nr·���������¿���������
		//uint8_t key_bind_induction_car_nr;  //����ʹ�õ�nr·�ظ��밴����
		uint8_t card_reader_bind_induction_car_nr[7];//
		uint8_t card_bind_bar[7];
//		uint8_t rs485_bind_induction_car_nr[3];//
	}Stu_Dat;
	uint16_t arr_dat[100];
}UNI_CONF_DAT;



enum potocolType
{
	CHECK_CONFIG = 9,               //��ȡ���ӵ�������Ϣ
	INDUCTION_CAR_VALUE_TRUE = 10, //��⵽�ظ��ź�
	KEY_CARD_VALUE_TRUE,           //��⵽ȡ����������
	READ_CARD_VALUE,			    //��⵽ˢ������
	CARD_RECYCLING,			    //��⵽��Ƭ���ն���
	OPEN_THE_DOOR,			    //̧���ź�
	CLOSE_THE_DOOR,	  		    //����ź�
	RS232_BIND_CARD_NR,         //�����¿����󶨵�nr·������
	RS232_BIND_KEY_NR,          //�����¿��������󶨵�nr·����
	VIDEO_RS485_NR,             //���õ�nr·RS485���Ϊ��Ƶ���
	SET_CARD_OFF_LINE_MSG,      //�������صĿ��š����֤�š���Ч����
	SET_CARD_OFF_LINE_MODE,		//���û����Ƿ�Ϊ����ģʽ
	CARD_BIND_INDUCTION_CAR_NR, //���ö������󶨵�nr·������Ϊ�ظ�
	CARD_BIND_BAR,				//���ö������󶨵�nr·���
	CARD_REGIST,             //ע���¿�
	CARD_SEARCH,             //��ѯ����Ϣ
	CARD_SEARCH_ALL,             //��ѯ����Ϣ
	CARD_DELETE,			//ɾ����
	CARD_DELETE_ALL,		//ȫ��ɾ��
	CARD_SIGNAL_MSG,
	CARD_SEC_CLR
};

typedef enum _CARD_SIGNAL_TYPE
{
	SIGNAL_CARD_SIGNAL_NULL,      //���ź�
	SIGNAL_ADD_NEW_CARD_OK,
	SIGNAL_ADD_NEW_CARD_ERROR,
	SIGNAL_DELETE_CARD_OK,
	SIGNAL_DELETE_CARD_ERROR,
	SIGNAL_SEARCH_ALL			//��ѯ���п�����Ϣ
	



}CARD_SIGNAL_TYPE;

typedef struct _Stu_Card_Signal
{
	CARD_SIGNAL_TYPE signal;
	uint8_t param1;
	uint16_t param2;
	uint32_t paramH;
	uint8_t param3[20];
}Stu_Card_Signal;
#endif		/* _TYPE_H_ */
