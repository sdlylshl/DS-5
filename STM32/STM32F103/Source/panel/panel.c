/*
 * panel.c
 *
 *  Created on: 2015
 *      Author: souls
 */

#include "../LCD1602/lcd_1602a.h"
#include "../Algorithm/Buffer/buffer.h"
#include "../System/Usart/usart2.h"
//全局变量 布防状态
//A 0 布防 B 1 撤防 C 2 取消报警状态 
volatile uint8_t PanelStatus =0;


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
typedef struct panelcmd {
	//uint8_t HEAD;
	uint8_t cmd;
	uint8_t len;
	uint8_t index;
	uint8_t data[PANEL_MAX_LEN];
	uint8_t checksum;
} PANELCMD_t;

PANELCMD_t panelcmd;

uint8_t calcfcs(uint8_t *pmsg, uint8_t len) {
	uint8_t result = 0;
	while (len--) {
		result ^= *pmsg++;
	}
	return result;
}


void buchefang(PANELCMD_t * pan){

	switch (pan->data[0])
	{
		//布防
	case 0:
		PanelStatus = 0;
		break;
		//撤防
	case 1:
		PanelStatus = 1;
		break;
		//取消报警
	case 2:
		PanelStatus = 2;
		break;
	default:

		break;
	}
}
void panel_parsedata(PANELCMD_t * pan){
	switch (pan->cmd)
	{
	case POLL_CMD:
		buchefang(pan);
		break;
	case SET_USER_PASSWD:

		break;
	case SET_ADMIN_PASSWD:

		break;
	default:
		break;
	}
}
	int tmp;
void panel(void) {
	uint8_t go = 0;

	Ebuffer_Init((uint8_t *)&USART2_read, (uint8_t *)&USART2_write, USART2_ReciveBuff);
	RS485_init();
	LCD1602_Init();
	while (1) {

		if (go) {
			goto GOON;
		} else {
			if (Ebuffer_getused() > PANEL_CMD_MINLEN) {
					tmp = Ebuffer_getused();//test
				if (Ebuffer_read() == PANEL_CMD_HEAD) {
					panelcmd.len = Ebuffer_read();
					if(panelcmd.len<PANEL_MAX_LEN){
						
					GOON:
					//接收到完整的指令
					tmp = Ebuffer_getused();//test
					if (Ebuffer_getused() > panelcmd.len + 3) {

						//数据校验
						if (calcfcs(Ebuffer_getpbufer(), panelcmd.len + 3)) {
							panelcmd.cmd = Ebuffer_read();
							panelcmd.index = Ebuffer_read();
							Ebuffer_reads(panelcmd.data,panelcmd.len);
							panelcmd.checksum =Ebuffer_read();
							
							//数据解析
							panel_parsedata(panelcmd);
						}
						go = 0;
					}else{
					//未接收到完整指令
					go = 1;
					}
				}
			}
			}
		}
	}
}

