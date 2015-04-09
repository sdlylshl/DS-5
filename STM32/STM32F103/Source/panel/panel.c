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
#define ARM_CMD					0x4

#define POLL_ARM_RSP 			0x50
#define POLL_CH_MAP_RSP			0x51
#define SET_USER_PASSWD_RSP		0x52
#define SET_ADMIN_PASSWD_RSP	0x53
#define ARM_CMD_RSP				0x54


#define PANEL_CMD_HEAD 0xFC
#define PANEL_CMD_MINLEN 4
#define PANEL_MAX_LEN 8
//全局变量
uint8_t xintiao[6] = { 0 };
uint8_t bufang[8] = { 0 };
uint8_t tongdao[10] = { 0 };

//首次开机标志，同步状态后失效
uint8_t firststart = 1;
uint16_t last_key = 0;
//撤防 灯灭  正在布防 快闪   布防成功 常量
//布防状态
//A 0 布防 B 1 撤防 C 2 取消报警状态 
volatile uint8_t PanelStatus = 0;
volatile uint8_t lastpanelstatus = 0;
uint8_t LastStatus = 0xFF;
//****用来监视 PanelStatus 状态改变是否是用户操作
//当SyncFlag不为0 则代表 用户进行相关配置
//将状态 上传之后 清零
// 1.有布撤防状态改变 2.设置通道映射
uint8_t SyncFlag = 0;
uint8_t SuccessFlag = 0;
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
	uint8_t id;
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
	pan->checksum ^= pan->id;
	RS485_SendChar(pan->id);
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
void panelcmdpoll(PANELCMD_t * panelcmd){
	static uint8_t lastId;

	static uint8_t cmdbufangId;
	static uint8_t cmdtongdaoId;
	if (lastId == panelcmd->id)
	{
		//重发			
		if (lastId == cmdbufangId)
		{
			panelcmd->data[0] = PanelStatus;
			ansbufang(panelcmd);
		}
		else if (lastId == cmdtongdaoId)
		{

		}
	}
	else
	{
		//收到新指令
		lastId = panelcmd->id;
		//为了保证能有效的判断 cmdId 所以 如果主机发送过来的Id为0 做心跳处理
		if ((SyncFlag & 0x01) && lastId)
		{

			panelcmd->data[0] = PanelStatus;
			ansbufang(panelcmd);
			SyncFlag &= 0xFE;
			cmdbufangId = panelcmd->id;

		}
		else if ((SyncFlag & 0x02) && lastId){

			SyncFlag &= 0xFD;
			cmdtongdaoId = panelcmd->id;

		}
		else{
			if (PanelStatus == 2){
				//取消报警状态成功 切换到原状态
				if (panelcmd->data[0] == 0xFD){
					PanelStatus = 3;
				}
				else if (panelcmd->data[0] == 0xFE){
					PanelStatus = 1;
				}
				else if (panelcmd->data[0] == 0xFF){
					PanelStatus = 0;
				}
			}
			if (cmdbufangId){ cmdbufangId = 0; }
			if (cmdtongdaoId){ cmdtongdaoId = 0; }
			panelcmd->data[0] = 0xFF;
			ansbufang(panelcmd);

		}


	}

}
//接收布防状态
static void recvbuchefang(PANELCMD_t * pan){
	switch (pan->data[0])
	{
		//-------------------------------------------------
		//服务器端更新数据
	case 0://布防
		//PanelStatus = 0;
		//break;		
	case 1://撤防
		//PanelStatus = 1;
		//break;		
	case 2://取消报警
		//PanelStatus = 2;
	case 3://服务器取消部分报警
		//PanelStatus = 3;
		last_key = 0;
		//SyncFlag &= 0xFE;	//1.服务器布撤防状态改变,取消上传
		PanelStatus = pan->data[0];

		//收到应答
		pan->data[0] = 0xff;
		ansbufang(pan);
		//wavplay(yuanchengbufang);
		break;
		//---------------------------------------------------
		//网关当前状态，正常主机查询指令
	case 0xFD:
		//判断是否是首次开机 同步布取消报警状态
		if (firststart){
			PanelStatus = 3;
			firststart = 0;
		}
	case 0xFE:
		//判断是否是首次开机 同步布撤防状态
		if (firststart){
			PanelStatus = 1;
			firststart = 0;
		}

	case 0xFF://布防
		if (firststart){
			PanelStatus = 0;
			firststart = 0;
		}
		panelcmdpoll(pan);
		//printf("sendId%03x\n", pan->id);

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
							panelcmd.id = Ebuffer_read();
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
uint8_t keyindex;
uint8_t checkpasswd(){
	if (keyindex == 4)
	{
		uint8_t i = 0;
		for (i = 0; i < 4; i++)
		{
			if (passwd[i] != userpasswd[i]){
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
void keyAevent(void){

	SyncFlag |= 1;
	PanelStatus = 0;
	keyindex = 0;
}
void keyATimeoutevent(void){
	SyncFlag |= 1;
	PanelStatus = 0;
	keyindex = 0;
}
void keyBevent(void){
	if (checkpasswd())
	{

		SyncFlag |= 1;
		PanelStatus = 1;
	}

	keyindex = 0;
}
void keyBTimeoutevent(void){
	keyindex = 0;
}
void keyCevent(void){

	SyncFlag |= 1;
	lastpanelstatus = PanelStatus;
	PanelStatus = 2;
	keyindex = 0;
}
void keyCTimeoutevent(void){
	SyncFlag |= 1;
	PanelStatus = 2;
	keyindex = 0;
}
void keyNevent(void){
	if (keyindex < 16)
	{
		passwd[keyindex++] = key;
	}
}
void keyNTimeoutevent(void){
	if (keyindex < 16)
	{
		passwd[keyindex++] = key;
	}
}
void(*normalhandler)(void) = (void(*)(void))0;
void(*timehandler)(void) = (void(*)(void))0;
uint16_t panel_keyHandle(){
	uint32_t timescan;
	if (GetKey())
	{
		switch (key)
		{
		case KEY_A:
			normalhandler = keyAevent;
			timehandler = keyATimeoutevent;
			timescan = 1000;
			break;
		case KEY_B:
			normalhandler = keyBevent;
			timehandler = keyBTimeoutevent;
			timescan = 1000;
			break;
		case KEY_C:
			normalhandler = keyCevent;
			timehandler = keyCTimeoutevent;
			timescan = 1000;
			break;
		default:
			normalhandler = keyNevent;
			timehandler = keyNTimeoutevent;
			timescan = 4000;								//调节用户体验
			break;
		}
		//二选一 
		keyEvent(normalhandler, timehandler, timescan);
	}
	return 0;
}

//////////////////状态指示//////////////////////////////
void panel_ShowStatus(){

	if (LastStatus != PanelStatus){
		LastStatus = PanelStatus;
		switch (PanelStatus)
		{
		case 0://布防

			Write_String(0xc0, "bu fang         ");
			LEDFlashing(0);
			LED2(0);
			break;
		case 1://撤防
			Write_String(0xc0, "che fang        ");
			LEDFlashing(0);
			LED2(1);
			break;
		case 2://取消报警状态

			Write_String(0xc0, "qu xiao bao jing");
			LEDFlashing(500);
			break;
		case 3://取消

			Write_String(0xc0, "elin            ");
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
		panel_keyHandle();
		panel_paredata();

	}
}

