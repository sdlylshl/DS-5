/*
 * panel.c
 *
 *  Created on: 2015
 *      Author: souls
 */

#include "../LCD1602/lcd_1602a.h"
#include "../Algorithm/Buffer/buffer.h"
#include "../System/Usart/usart2.h"
#include "../GPIO/RS485.h"
#include "../KEY/key.h"
#include "../GPIO/led.h"


#define POLL_CMD 				0
#define SET_USER_PASSWD			0x2
#define SET_ADMIN_PASSWD		0x3
#define ARM_CMD				0x4

#define POLL_ARM_RSP 			0x50
#define POLL_CH_MAP_RSP			0x51
#define SET_USER_PASSWD_RSP		0x52
#define SET_ADMIN_PASSWD_RSP		0x53
#define ARM_CMD_RSP			0x54


#define PANEL_CMD_HEAD 0xFC
#define PANEL_CMD_MINLEN 4
#define PANEL_MAX_LEN 8
//全局变量
uint8_t xintiao[6] = { 0 };
uint8_t bufang[8] = { 0 };
uint8_t tongdao[10] = { 0 };

//首次开机标志，同步状态后失效
uint8_t firststart = 1;
//撤防 灯灭  正在布防 快闪   布防成功 常量
//布防状态
//A 0 布防 B 1 撤防 C 2 取消报警状态 
volatile uint8_t PanelStatus = 0;
uint8_t LastStatus = 0xFF;
//用来监视 PanelStatus 状态改变是否是用户操作
// 1.有布撤防状态改变 2.设置通道映射
uint8_t KeyFlag = 0;
uint8_t bufangFlag = 0;
uint8_t tongdaoFlag = 0;
//密码相关
uint16_t passwd[16] = { 0 };
uint16_t userpasswd[8] = { KEY_1, KEY_2, KEY_3, KEY_4, 0, 0 };
uint16_t syspasswd[8] = { KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, 0, 0, 0 };


typedef struct panelcmd {
	//uint8_t HEAD;
	uint8_t len;
	uint8_t cmd;
	uint8_t index;
	uint8_t data[PANEL_MAX_LEN];
	uint8_t checksum;
} PANELCMD_t;

PANELCMD_t panelcmd;
/////////////////////数据处理////////////////////////////////
uint8_t calcfcs(uint8_t *pmsg, uint8_t len) {
	uint8_t result = 0;
	while (len--) {
		result ^= *pmsg++;
	}
	return result;
}
void answer(uint8_t *pmsg, uint8_t len){

	uint8_t result = 0;
	RS485_TX();

	RS485_SendChar(PANEL_CMD_HEAD);
	while (len--) {
		RS485_SendChar(*(pmsg++));
	}

	RS485_RX();
}
//指令应答
static void ansbufang(PANELCMD_t * pan){
	uint32_t i;

	RS485_TX();
	Delay_ms(1);
		pan->cmd = POLL_ARM_RSP;
	//for(i=0;i<0x4FFFF;i++);
	pan->checksum = 0;
	RS485_SendChar(PANEL_CMD_HEAD);
	pan->checksum ^= pan->len;
	RS485_SendChar(pan->len);
	pan->checksum ^= pan->cmd;
	RS485_SendChar(pan->cmd);
	pan->checksum ^= pan->index;
	RS485_SendChar(pan->index);
	for (i = 0; i < pan->len; i++)
	{
		pan->checksum ^= pan->data[i];
		RS485_SendChar(pan->data[i]);
	}
	RS485_SendChar(pan->checksum);
	//for(i=0;i<0x4FFFF;i++);
	Delay_ms(1);
	RS485_RX();
}
//接收布防状态
static void recvbuchefang(PANELCMD_t * pan){
	switch (pan->data[0])
	{
		//布防
	case 0:
		PanelStatus = 0;
		//break;
		//撤防
	case 1:
		PanelStatus = 1;
		//break;
		//取消报警
	case 2:
		PanelStatus = 2;
		KeyFlag &= 0xFE;
	
		pan->data[0] = 0xff;
	
		ansbufang(pan);
		//wavplay(yuanchengbufang);
		break;
		case 0xFE:
		//判断是否是首次开机 同步布撤防状态
		if(firststart){
				PanelStatus = 1;
			firststart =0;
		}
			
		
		case 0xFF:
				if(firststart){
				PanelStatus = 0;
				firststart =0;
				}
				

		//0xFF	
		printf("sendTimes%03x\n", pan->index);
		if ((bufangFlag == pan->index) && pan->index)
		{
			//布防失败
			pan->data[0] = PanelStatus;
			ansbufang(pan);
			KeyFlag &= 0xFE;
		}
		else if ((tongdaoFlag == pan->index) && pan->index)
		{
			//通道设置失败
			KeyFlag &= 0xFD;


		}
		else
		{
			if ((bufangFlag)){
				//布防成功
				bufangFlag = 0;
			}
			else	if ((tongdaoFlag)){
				tongdaoFlag = 0;
			}
			//因为ID 改变了 所以 一定是数据接收成功了
			//lastId = pan->index;

			//新指令
			if ((KeyFlag & 0x01) && pan->index){
				KeyFlag &= 0xFE;
				pan->data[0] = PanelStatus;
				ansbufang(pan);
				bufangFlag = pan->index;
			}
			else if ((KeyFlag & 0x02) && pan->index){
				//通道设置
				KeyFlag &= 0xFD;
				tongdaoFlag = pan->index;
			}
			else{
				//普通应答 心跳
				pan->data[0] = 0xFF;
				ansbufang(pan);
			}
		}	
		break;
	default:
		break;
	}
}
//指令解析
static void panel_parsedcmd(PANELCMD_t * pan){
	switch (pan->cmd)
	{
	case POLL_CMD:
		recvbuchefang(pan);
		break;
	case SET_USER_PASSWD:
		break;
	case SET_ADMIN_PASSWD:

		break;
	default:
		break;
	}
}
//数据解析
void panel_paredata(){

	static uint8_t go = 0;
	if (go) {
		goto GOON;
	}
	else {
		if (Ebuffer_getused() >= PANEL_CMD_MINLEN) {
			if (Ebuffer_read() == PANEL_CMD_HEAD) {
				panelcmd.len = Ebuffer_read();
				if (panelcmd.len < PANEL_MAX_LEN){

				GOON:
					//接收到完整的指令
					if (Ebuffer_getused() >= panelcmd.len + 3) {

						//数据校验
						if (!calcfcs(Ebuffer_getpbufer(), panelcmd.len + 4)) {
							panelcmd.cmd = Ebuffer_read();
							panelcmd.index = Ebuffer_read();
							Ebuffer_reads(panelcmd.data, panelcmd.len);
							panelcmd.checksum = Ebuffer_read();

							//数据解析
							panel_parsedcmd(&panelcmd);
						}
						go = 0;
					}
					else{
						//未接收到完整指令
						go = 1;
					}
				}
			}
		}
	}
}
//////////////////按键处理//////////////////////////////
uint8_t cmppass(){
	uint8_t i = 0;
	for (i = 1; userpasswd[i]; ++i)
	{
		if (passwd[i] != userpasswd[i]){
			return 0;
		}
	}
	return 1;
}
uint16_t panel_getbufang(){
	uint8_t i;
	static uint8_t index = 0;
	static uint16_t last_key = 0;
	static uint8_t keymflg = 0;
	static uint8_t modeset = 0;		//布防模式设置
	uint32_t timeout = 8000;		//超时时间
	uint16_t ckey;

	if (timeout-- == 0){
		last_key = 0;
		timeout = 10000;
	}
	//ckey=sizeof(passwd);
	ckey = KeySacn();
	if (ckey && (ckey != last_key))
	{
		last_key = ckey;

		if (ckey == KEY_A){
			PanelStatus = 0;
			KeyFlag = 1;
			index = 0;
		}
		else if (ckey == KEY_B){
			if (index && cmppass()){
				PanelStatus = 1;
				KeyFlag = 1;
			}
			index = 0;
		}
		else if (ckey == KEY_C){

			index = 0;
			PanelStatus = 2;
			KeyFlag = 1;
			//先做功能键的处理		
		}
		else
			//密码处理
		if (index){

			if (index < (sizeof(userpasswd) / sizeof(userpasswd[0]))){
				passwd[index++] = ckey;
			}
			else{
				index = 0;
			}
		}
		else if (userpasswd[0] == ckey)
		{
			passwd[0] = ckey;
			for (i = 1; i < (sizeof(passwd) / sizeof(passwd[0])); i++)
			{
				passwd[i] = 0;
			}
			index = 1;
		}
		//密码处理结束

	}
}
//////////////////状态指示//////////////////////////////
void panel_ShowStatus(){

	if (LastStatus != PanelStatus){
		LastStatus = PanelStatus;
		switch (PanelStatus)
		{
		case 0://布防
			
			Write_String(0xc0,"bu fang             ");
			LEDFlashing(0);
			LED2(0);
			break;
		case 1://撤防
			Write_String(0xc0, "che fang            ");
			LEDFlashing(0);
			LED2(1);
			break;
		case 2://取消报警状态

			Write_String(0xc0, "qu xiao bao jing      ");
			LEDFlashing(500);
			break;
		default:
			break;
		}
	}
}

void panel(void) {
	//注册
	Ebuffer_Init((uint8_t *)&USART2_read, (uint8_t *)&USART2_write, USART2_ReciveBuff);

	RS485_init();
	KeyInit();
	//DS1302_Init();
	LCD1602_Init();

	while (1) {
		panel_ShowStatus();
		panel_getbufang();
		panel_paredata();

	}
}

