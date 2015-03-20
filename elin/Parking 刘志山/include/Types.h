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
		uint8_t wigen_count[4];    //wigen信号为 26 34
		Boolean is_off_line_mode;   //配置是否为离线模式
		uint8_t rs232_bind_card_nr; //第nr路wigen读卡器被设置为吐卡机读卡器
		//uint8_t rs232_bind_rs485_card_nr; //第nr路wigen读卡器被设置为吐卡机读卡器
		CONFIG_MSG Config_Msg;
		uint8_t dest_ip[8][4];
		uint16_t dest_port[8];
		uint8_t rs232_bind_key_nr; //配置使用第nr路按键控制吐卡机读卡器
		//uint8_t key_bind_induction_car_nr;  //配置使用第nr路地感与按键绑定
		uint8_t card_reader_bind_induction_car_nr[7];//
		uint8_t card_bind_bar[7];
//		uint8_t rs485_bind_induction_car_nr[3];//
	}Stu_Dat;
	uint16_t arr_dat[100];
}UNI_CONF_DAT;



enum potocolType
{
	CHECK_CONFIG = 9,               //获取板子的配置信息
	INDUCTION_CAR_VALUE_TRUE = 10, //检测到地感信号
	KEY_CARD_VALUE_TRUE,           //检测到取卡按键动作
	READ_CARD_VALUE,			    //检测到刷卡动作
	CARD_RECYCLING,			    //检测到卡片回收动作
	OPEN_THE_DOOR,			    //抬杆信号
	CLOSE_THE_DOOR,	  		    //落杆信号
	RS232_BIND_CARD_NR,         //配置吐卡机绑定第nr路读卡器
	RS232_BIND_KEY_NR,          //配置吐卡机按键绑定第nr路输入
	VIDEO_RS485_NR,             //配置第nr路RS485输出为视频输出
	SET_CARD_OFF_LINE_MSG,      //离线下载的卡号、身份证号、有效日期
	SET_CARD_OFF_LINE_MODE,		//设置机器是否为离线模式
	CARD_BIND_INDUCTION_CAR_NR, //配置读卡器绑定第nr路输入作为地感
	CARD_BIND_BAR,				//配置读卡器绑定第nr路横杆
	CARD_REGIST,             //注册新卡
	CARD_SEARCH,             //查询卡信息
	CARD_SEARCH_ALL,             //查询卡信息
	CARD_DELETE,			//删除卡
	CARD_DELETE_ALL,		//全部删除
	CARD_SIGNAL_MSG,
	CARD_SEC_CLR
};

typedef enum _CARD_SIGNAL_TYPE
{
	SIGNAL_CARD_SIGNAL_NULL,      //空信号
	SIGNAL_ADD_NEW_CARD_OK,
	SIGNAL_ADD_NEW_CARD_ERROR,
	SIGNAL_DELETE_CARD_OK,
	SIGNAL_DELETE_CARD_ERROR,
	SIGNAL_SEARCH_ALL			//查询所有卡的信息
	



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
