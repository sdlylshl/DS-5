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
		uint16_t dest_std_id;  
		uint16_t filter_id;
		uint16_t tm_online_signal_inteval;

	}Stu_Dat;
	uint16_t arr_dat[1024];
}UNI_CONF_DAT;



enum potocolType
{
	JDQ_STATE_MANUAL = 1,           //0x01 获取板子的配置信息
	GET_JDQ_TIME_SETTING,			//获取继电器时间配置信息
	JDQ_STATE_AUTO_TIME,			//配置开灯的时间
	JDQ_STATE_AUTO_TIME_BY_CRC32,   //通过crc32配置时间
	CLR_JDQ_LINE_TIME_SETTING,      //清楚某一路上的时间配置信息

	SET_JDQ_TIME_MODE_NR = 0x07,
	SET_JDQ_TIME_LINE_NR = 0x08,
	SET_JDQ_TIME_MODE_NRS = 0x09,
	SET_JDQ_TIME_MODE_NRS_BY_WEEK = 0x10,       //按照星期来设定时间模式
	SET_JDQ_TIME_MODE_NRS_BY_DAY,         //按照日期来设定时间模式
	SET_JDQ_TIME_MODE_NRS_NULL,            //禁用当前模式
	GET_JDQ_TIME_MODE,				//获取配置信息

	FLAG_REFRESH_TM_MANUAL = 0xEE,


};

typedef enum _JDQ_SIGNAL_TYPE
{
	SIGNAL_NULL = 0,      //空信号
	SIGNAL_JDQ_TIME_SETTING_Search,   //继电器时间查询信号
	SIGNAL_JDQ_Tm_Mode_GET_ALL,		  //获取配置信息
	SIGNAL_JDQ_Tm_Mode_GET_ONE

	



}JDQ_SIGNAL_TYPE;

typedef struct _Stu_JDQ_Signal
{
	JDQ_SIGNAL_TYPE signal;
	JDQ_SIGNAL_TYPE next_signal;
	uint8_t param1;
	uint16_t param2;
	uint32_t paramH;
	uint8_t param3[20];
}Stu_JDQ_Signal;

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t week;
	uint8_t day;
	uint8_t month;
	uint8_t year;
}STU_TIME;

#define TM_MODE_NUM 8
#define TM_MODE_CONF_NUM 10
#define TM_MODE_LEN 1490

typedef union _Tm_Mode_t{

	struct _stu_tm_mode {
		uint8_t mode_id;
		uint8_t type;// 1:按星期，2:按日
		uint8_t param[48];//按星期：7个字节
						//按日  ：4*12个字节 按二进制位表示
		uint8_t mins_a_day[8][180];

	} stu_dat;
	uint8_t arr_dat[TM_MODE_LEN ];
} Tm_Mode_t;

typedef struct {
	uint16_t ext_id;
	uint8_t times;
	uint8_t len;
	uint8_t data[8];
	
} Stu_Can_Msg;


#endif		/* _TYPE_H_ */
