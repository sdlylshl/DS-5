/*
 * panel.c
 *
 *  Created on: 2015
 *      Author: souls
 */

#include "stddef.h"
#include "../Algorithm/Buffer/buffer.h"
#include "../System/Delay/systick.h"
#include "../System/Usart/usart2.h"
#include "../System/Timer/timer4.h"
#include "../LCD1602/lcd1602a.h"
#include "../GPIO/Beep.h"
#include "../GPIO/led.h"
#include "../KEY/key.h"
#include "./RS485.h"
#include "../Menu/menu.h"

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

//布防延迟时间
uint32_t BFDELAYTIME = 30000;
uint32_t panelcurtime;
uint8_t bufangdelay;
uint32_t bufangdelaytime;

typedef struct panelcmd {
	//uint8_t HEAD;
	uint8_t len;
	uint8_t cmd;
	uint8_t id;
	uint8_t data[PANEL_MAX_LEN];
	uint8_t checksum;
} PANELCMD_t;

PANELCMD_t panelcmd;

void startdidi(){
	BeepDiDiStart(100, 900);
}
void stopdidi(){
	bufangdelay = 0;
	BeepStop();

}
/////////////////////数据处理////////////////////////////////
uint8_t calcfcs(uint8_t *pmsg, uint8_t len) {
	uint8_t result = 0;
	while (len--) {
		result ^= *pmsg++;
	}
	return result;
}
void answer(uint8_t *pmsg, uint8_t len){

//	uint8_t result = 0;
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
		stopdidi();
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
			pan->data[0] = 0xff;
			ansbufang(pan);
			break;
		}
	case 0xFE:
		//判断是否是首次开机 同步布撤防状态
		if (firststart){
			PanelStatus = 1;
			firststart = 0;
			pan->data[0] = 0xff;
			ansbufang(pan);
			break;
		}

	case 0xFF://布防
		if (firststart){
			PanelStatus = 0;
			firststart = 0;
			pan->data[0] = 0xff;
			ansbufang(pan);
			break;
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

void Delaybufang(){
	SyncFlag |= 1;
	PanelStatus = 0;
}
void keyAevent(void){

	keyindex = 0;
}
void keyATimeoutevent(void){
	keyindex = 0;
}
void keyBevent(void){
	if (checkpasswd())
	{

		SyncFlag |= 1;
		PanelStatus = 1;
		stopdidi();
	}

	keyindex = 0;
}
void keyBTimeoutevent(void){
	keyindex = 0;
}
void keyCevent(void){
	if (checkpasswd())
	{
		SyncFlag |= 1;
		lastpanelstatus = PanelStatus;
		PanelStatus = 2;
		stopdidi();
	}
	keyindex = 0;
}
void keyCTimeoutevent(void){

	keyindex = 0;
}
const uint16_t numindex[] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9 };
void changebufangdelaytime(){
	int8_t i, j;
	uint32_t num = 0;
	if (keyindex > 3){
		keyindex = 3;
	}
	for (i = 0; i < keyindex; i++){
		for (j = 0; j < 10; j++){
			if (passwd[i] == numindex[j]){

				switch (keyindex - i){
				case 1:
					num += j;
					break;
				case 2:
					num += j * 10;
					break;
				case 3:
					num += j * 100;
					break;

				}
				break;
			}
		}
	}

	if (num > 300 || num < 5){
		num = 30;
	}
	BFDELAYTIME = num * 1000;
}
void keyF1event(void){
	changebufangdelaytime();
	keyindex = 0;
}
void keyF1Timeoutevent(void){


	//changebufangdelaytime();
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
			//一分钟后布防
			panelcurtime = TIM4_GetCurrentTime();
			if(PanelStatus){
			bufangdelay = 1;
			startdidi();
			}
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
		case KEY_F1:
			//修改延时 报警时间
			normalhandler = keyF1event;
			timehandler = keyF1Timeoutevent;
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

			LCD1602_Write_String(0x0,1, "Arming         ");
			LEDFlashing(0);
			LED2(0);
			break;
		case 1://撤防
			LCD1602_Write_String(0x0,1, "Disarm          ");
			LEDFlashing(0);
			LED2(1);
			break;
		case 2://取消报警状态

			LCD1602_Write_String(0x0,1, "Cancel alarm    ");
			LEDFlashing(500);
			break;
		case 3://取消

			LCD1602_Write_String(0x0,1, "elin            ");
			LEDFlashing(500);
			break;
		default:
			break;
		}
	}
}

void panel_can(void) {
	//注册
	Ebuffer_Init((uint8_t *)&USART2_read, (uint8_t *)&USART2_write, USART2_ReciveBuff);

	//RS485_init();
	KeyInit();
	//DS1302_Init();
	LCD1602_Init();

	while (1) {
		IWDG_ReloadCounter();//喂狗
		panel_ShowStatus();
		panel_keyHandle();
		if (bufangdelay){
			if (PanelStatus){
				//不在布防模式
				if (TIM4_GetDistanceTime(panelcurtime) > BFDELAYTIME){
					Delaybufang();
					stopdidi();
					BeepStart(1000);
				}
			}
			else{
				stopdidi();
			}
		}
		panel_paredata();
	}
}
//***********************************************************************************************
#define PANEL_MAIN_MENU_NUM 6
struct MenuItem PanelMainMenu[PANEL_MAIN_MENU_NUM] =		//结构体主菜单
{
//MenuCount DisplayString pMenuFun ChildrenMenus ParentMenus
	{PANEL_MAIN_MENU_NUM,"www.jhmcu.com ",NullSubs,NULL,PanelMainMenu},
	{PANEL_MAIN_MENU_NUM,"22222222222222",NullSubs,NULL,NULL},
	{PANEL_MAIN_MENU_NUM,"33333333333333",NullSubs,NULL,NULL},
	{PANEL_MAIN_MENU_NUM,"44444444444444",NullSubs,NULL,NULL},
	{PANEL_MAIN_MENU_NUM,"55555555555555",NullSubs,PanelMainMenu,PanelMainMenu},
	{PANEL_MAIN_MENU_NUM,"www.jhmcu.com ",NullSubs,PanelMainMenu,PanelMainMenu}
};

#define PANEL_FIRST_MENU_NUM 6
struct MenuItem PanelFirstMenu[PANEL_FIRST_MENU_NUM] =		//结构体主菜单
{
//MenuCount DisplayString pMenuFun ChildrenMenus ParentMenus
	{PANEL_FIRST_MENU_NUM,"www.jhmcu.com ",NullSubs,NULL,PanelMainMenu},
	{PANEL_FIRST_MENU_NUM,"22222222222222",NullSubs,NULL,NULL},
	{PANEL_FIRST_MENU_NUM,"33333333333333",NullSubs,NULL,NULL},
	{PANEL_FIRST_MENU_NUM,"44444444444444",NullSubs,NULL,NULL},
	{PANEL_FIRST_MENU_NUM,"55555555555555",NullSubs,PanelMainMenu,PanelMainMenu},
	{PANEL_FIRST_MENU_NUM,"www.jhmcu.com ",NullSubs,PanelMainMenu,PanelMainMenu}
};


void panel_can_irq(void){

//		if (TI)
//		{
//			if ((TxNum--)>0)
//			{
//				SBUF = Send232Buffer[TxPos++];
//			}
//			else
//			{
//				TxNum = 0;
//				TxPos = 0;
//			}
//			//   
//			TI = 0;
//		}
//		else
//		{
//			RI = 0;
//			RxNow = SBUF;
//			Rev232Buffer[RxPos++] = RxNow;
//			if (RxPos >= 255)
//			{
//				RxPos = 0;
//			}
//			//   
//			if (!b_ReceiveOver)
//			{
//				switch (RxStep)
//				{
//				case    0://帧头:55 55   
//					if (RxNow == 0x55)
//					{
//						if (RxPrev == 0x55)
//						{
//							RxStep = 1;
//						}
//						else
//						{
//							RxPrev = RxNow;
//							RxPos = 0;
//							Rev232Buffer[RxPos++] = RxNow;
//							RxStep = 0;
//						}
//					}
//					else
//					{
//						RxStep = 0;
//						RxPos = 0;
//						REN = 1;
//					}
//					break;
//				case    1://命令帧和数据帧的判断   
//					if ((RxNow & 0x80) == 0)
//					{
//						RxStep = 3;
//					}
//					else
//					{
//						RxPrev = RxNow;
//						RxStep = 2;
//					}
//					break;
//				case    2://命令帧，第二个命令字（和第一个命令字相同)   
//					if (RxNow == RxPrev)
//					{
//						RxStep = 7;
//					}
//					else
//					{
//						RxStep = 0;
//						RxPos = 0;
//						REN = 1;
//					}
//					break;
//				case    3://数据帧,帧长度   
//					RxDataLenght = RxNow;
//					RxStep = 4;
//					break;
//				case    4://数据帧,数据段   
//					RxDataLenght--;
//					if (RxDataLenght == 0)
//					{
//						RxStep = 5;
//					}
//					break;
//				case    5://数据帧,累加和   
//					RxStep = 6;
//					break;
//				case    6://数据帧,异或和   
//					RxStep = 8;
//					break;
//				case    7://命令帧,累加和   
//					RxStep = 8;
//					break;
//				case    8://帧尾:aa aa   
//					if (RxNow == 0xaa)
//					{
//						if (RxPrev == 0xaa)
//						{
//							b_ReceiveOver = 1;
//							REN = 0;
//						}
//						else
//						{
//							RxPrev = RxNow;
//						}
//					}
//					else
//					{
//						RxStep = 0;
//						RxPos = 0;
//						REN = 1;
//					}
//					break;
//				default:
//					RxStep = 0;
//					RxPos = 0;
//					REN = 1;
//					break;
//				}
//			}
//		}
	

}

