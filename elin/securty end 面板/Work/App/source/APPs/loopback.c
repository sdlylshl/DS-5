#include "stm32f10x.h"
#include "config.h"
#include "W5500\w5500.h"
#include "W5500\socket.h"
#include "util.h"
#include "alarm.h"
#include "flash.h"
#include "adc.h"
#include "gsm.h"
#include "ds1302.h"
#include "APPs\loopback.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x_it.h"
#include "i2c_ee.h"

uint8 server_ch_status[MAX_SOCK_NUM] = { 0, };	/** 0:close, 1:ready, 2:connected 3:init*/
uint8 ch_status[MAX_SOCK_NUM] = { 0, };	/** 0:close, 1:ready, 2:connected 3:init*/
uint8 TX_BUF[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
uint8 TX_BUF1[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications

uint8 udp_status[MAX_SOCK_NUM] = {0, };
uint32_t presentTime;
uint32_t presentTime1;
uint32_t try_again_time;
peer_info l_peer;

//extern int test_data;

u8 try_again[MAX_NEED_RECV];
u32 try_data[MAX_NEED_RECV];

extern sys_para sys;



unsigned char calcfcs(unsigned char *pmsg, unsigned char len)
{
	unsigned char result = 0;
	while (len--)
	{
		result ^= *pmsg++;
	}
	return result;
}



int wrap_udp_sendto(SOCKET s, u8* buf, int len)
{
	
	sendto(s, buf, len, sys.dest_net_para.destip, sys.dest_net_para.port);
	return OK;
}
int do_common_rsp(SOCKET s, u8 mode, u8 cmd)
{
	int ret=ERROR, a;
	u8 recv_flag;
	
	recv_flag=10;
	while(((a=recv_loop(s, mode, cmd, 0))!= OK && recv_flag))
	{	
		recv_flag--;
		//printf("wait for live rsp in while\n");
		Delay_ms(100);
	}
	if(a != OK)
	{
		printf("recv RSP timeout\n");
	}
	else
		ret=OK;

	return ret;
}

u8 do_udp_get_time(SOCKET s, u8 mode)
{
	u8 cmd[16];
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 7;
	cmd[2] = NET_UDP_GET_TIME_RSP;
	*(u32*)(cmd+3) = sys.machine_id;
	ds1302_get_time(cmd+7);
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	//do_common_rsp(s);
    return OK;
}

u8 do_udp_stat_get(SOCKET s, u8 mode)
{	
	u8 i;
	u32 a;

	u8 cmd[30]={0};
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 22;
	cmd[2] = NET_UDP_STAT_GET_RSP;
	*(u32*)(cmd+3) = sys.machine_id;
	a = 0xffff0000;
	for(i=0; i<16; i++)
		a |= (get_alarm_in(i)<<i);
	*(u32*)(cmd+7) = ~a;

	
	for(i=0; i<4; i++)
		*(u16*)(cmd+11) |= (get_alarm_out(i)<<i);
	
	
	get_ip_addr_from_eeprom((cmd+13));
	get_id_from_eeprom(((u32*)(cmd+17)));
	get_mac_from_eeprom(cmd+21);

	*(u16*)(cmd+27) |= sys.is_net_loss;
	//*(u16*)(cmd+27) |= is_poweroff() << 1;
	*(u16*)(cmd+27) |= is_power_low() << 2;
	
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	//do_common_rsp(s);
	return OK;
}


u8 do_udp_link_set(SOCKET s, u8* rcv, u8 mode)
{
	u8 a, i;

	a = (rcv[0])/7;
	sys.link.link_group = a;
	save_link_group_to_eeprom(a);
	for(i=0; i<sys.link.link_group; i++)
	{
		sys.link.in[i] = *(u32*)(rcv+6+(7)*i);
		sys.link.out[i] = *(u16*)(rcv+10+(7)*i);
		sys.link.xor[i] = *(rcv+12+(7)*i);
		printf("link_in[%d]: 0x%x, link_out[%d]: 0x%x, link_xor[%d]: 0x%x\n", i, sys.link.in[i], i, sys.link.out[i], i, sys.link.xor[i]);
		save_link_to_eeprom(i, sys.link.in[i], sys.link.out[i], sys.link.xor[i]);
	}
	do_send_ok(s, mode, NET_UDP_LINK_SET_RSP);
	return OK;
}

u8 do_udp_get_arm_stat(SOCKET s, u8 mode)
{
	u8 cmd[16];
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_GET_ARM_STAT_RSP;
	*(u32*)(cmd+3) = sys.machine_id;

	*(u32*)(cmd+7) = sys.arming;
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
}

u8 do_udp_get_link_stat(SOCKET s, u8 mode)
{
	u8 cmd[128];
	u8 i;
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = sys.link.link_group*7;
	cmd[2] = NET_UDP_GET_LINK_STAT_RSP;
	*(u32*)(cmd+3) = sys.machine_id;
	
	for(i=0; i<sys.link.link_group; i++)
	{
		*(u32*)(cmd+7+(7)*i) = sys.link.in[i];
		*(u16*)(cmd+11+(7)*i) = sys.link.out[i];
		*(cmd+13+(7)*i) = sys.link.xor[i];
		
		printf("link_in[%d]: 0x%x, link_out[%d]: 0x%x, link_xor[%d]: 0x%x\n", i, sys.link.in[i], i, sys.link.out[i], i, sys.link.xor[i]);
	}
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
}

u8 do_udp_set_cell_num(SOCKET s, u8* rcv, u8 mode)
{
	u8 len,i,j;

	len = rcv[0];
	//for(i=0; i<len+3; i++)
	//	printf("recv: %d\n", rcv[i]);
	sys.cell.cell_group = (len-1)/11;

	sys.cell.cell_xor = rcv[6];
	save_cell_group_to_eeprom(sys.cell.cell_group);
	save_cell_xor_to_eeprom(sys.cell.cell_xor);
	for(i=0; i<sys.cell.cell_group; i++)
	{
		for(j=0; j<11; j++)
		{
			sys.cell.cell[i][j] = *(rcv+7+i*11+j);
			printf("sys.cell.cell[%d][%d]: %d\n", i, j, sys.cell.cell[i][j]);
		}
		save_cell_num_to_eeprom(i, sys.cell.cell[i]);
	}
	do_send_ok(s, mode, NET_UDP_SET_CELL_NUM_RSP);
	return OK;
}

u8 do_udp_set_cell_text(SOCKET s, u8* rcv, u8 mode, u8 index)
{
	u8 len[5],i,j;

	sys.text.text_enable |= 1<<index;
	save_text_enable_to_eeprom(sys.text.text_enable);
	//sys.text.text_group++;
	//printf("sys.text.text_group: %d\n", sys.text.text_group);
	//save_text_group_to_eeprom(sys.text.text_group);

	

	for(i=index; i<index+1; i++)
	{	
		len[i] = *(rcv+19);
		sys.text.text_length[i] = len[i];
		save_text_length_to_eeprom(i,len[i]);
		for(j=0; j<11; j++)
			sys.text.text[i][j] = *(rcv+8+j);
		save_text_num_to_eeprom(i, sys.text.text[i]);
		sys.text.text_enable |= 1<<i;
		for(j=0; j<len[i]; j++)
			sys.text.text_content[i][j] = *(rcv+20+j);
		printf("len[%d]: %d\n", i, len[i]);
		save_text_content_to_eeprom(i, len[i], sys.text.text_content[i]);
		printf("text_content[%d]: %s\n", i, sys.text.text_content[i]);
		
	}
	do_send_text_ok(s, mode, index, NET_UDP_SET_TEXT_NUM_RSP);
	return OK;
}

u8 do_udp_cancel_text_alarm(SOCKET s, u8 mode, u8 data)
{
	u8 i;

	if(data & (1<<i))
	{
		sys.text.text_enable |=  1<<i;
		//sys.text.text_group--;
		
	}
	save_text_enable_to_eeprom(sys.text.text_enable);
	//save_text_group_to_eeprom(sys.text.text_group);
	do_send_ok(s, mode, NET_UDP_CANCEL_TEXT_ALARM_RSP);
	return OK;
}

u8 do_udp_cancel_cell_alarm(SOCKET s, u8 mode, u8 data)
{
	u8 i;

	if(data & (1<<i))
	{
		sys.cell.cell_enable |=  1<<i;
		sys.cell.cell_group--;
	}
	save_cell_enable_to_eeprom(sys.cell.cell_enable);
	save_cell_group_to_eeprom(sys.cell.cell_group);
	do_send_ok(s, mode, NET_UDP_CANCEL_CELL_ALARM_RSP);
	return OK;
}

u8 do_udp_get_balance_rsp(SOCKET s, u8 mode, u8 data)
{
	u8 cmd[209];
	u8 l_text[512]={0};
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 200;
	cmd[2] = NET_UDP_GET_BALANCE_RSP;
	*(u32*)(cmd+3) = sys.machine_id;
	

	if(gsm_get_balance(l_text) == OK)
		memcpy(cmd+7, l_text, 200);
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
}

u8 do_udp_set_arm_link(SOCKET s, u8 mode, u8* cmd)
{
	sys.arm_link = (*(u16*)(cmd+7))<<16;
	sys.arm_link += (*(u16*)(cmd+9));
	printf("sys.arm_link: %x\n", sys.arm_link);
	save_arm_link_to_eeprom(sys.arm_link);
	do_send_ok(s, mode, NET_UDP_SET_ARM_LINK_RSP);

	return OK;
}
u8 do_udp_set_disarm_link(SOCKET s, u8 mode, u8* cmd)
{
	sys.disarm_link = (*(u16*)(cmd+7))<<16;
	sys.disarm_link += (*(u16*)(cmd+9));
	save_disarm_link_to_eeprom(sys.disarm_link);
	do_send_ok(s, mode, NET_UDP_SET_DISARM_LINK_RSP);
	return OK;
}

u8 do_udp_get_arm_link_rsp(SOCKET s, u8 mode)
{
	u8 cmd[12];
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_GET_ARM_LINK_RSP;
	*(u32*)(cmd+3) = sys.machine_id;

	printf(" get sys.arm_link: %x\n", sys.arm_link);
	*(u16*)(cmd+7) = (sys.arm_link)>>16;
	*(u16*)(cmd+9) = (sys.arm_link)&0xFFFF;
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
}
u8 do_udp_get_disarm_link_rsp(SOCKET s, u8 mode)
{	
	u8 cmd[12];
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_GET_DISARM_LINK_RSP;
	*(u32*)(cmd+3) = sys.machine_id;

	*(u16*)(cmd+7) = (sys.disarm_link)>>16;
	*(u16*)(cmd+9) = (sys.disarm_link)&0xFFFF;
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
	
}


u8 do_udp_get_cell_rsp(SOCKET s, u8 mode)
{	
	u8 cmd[72];
	u8 i, j;
	u8 cell[12];
	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	
	cmd[2] = NET_UDP_GET_CELL_NUM_RSP;
	*(u32*)(cmd+3) = sys.machine_id;

	cmd[7] = sys.cell.cell_xor;

	if(sys.cell.cell_group > 5)
		sys.cell.cell_group = 0;
	cmd[1] = 1 + 11*sys.cell.cell_group;

	
	for(i=0; i<sys.cell.cell_group; i++)
	{
		get_cell_num_from_eeprom(i, cell);
		for(j=0; j<11; j++)
			cmd[8+11*i+j] = cell[j];
	}
		
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
	
}


u8 do_udp_get_text_rsp(SOCKET s, u8 mode, u8 index)
{	
	u8 cmd[86];
	u8 len;
	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 1;
	cmd[2] = NET_UDP_GET_TEXT_NUM_RSP;
	*(u32*)(cmd+3) = sys.machine_id;

	cmd[7] = index;
	
	printf("sys.text.text_enable: %d, index: %d\n", sys.text.text_enable, index);
	
	if(sys.text.text_enable & (1<<index))
	{
		get_text_num_from_eeprom(index, cmd+8);
		get_text_length_from_eeprom(index,&len);
		cmd[19] = len;
		get_text_content_from_eeprom(index, len, cmd+20);
		cmd[1] += 12+len;
	}
	
	cmd[cmd[1]+7] = calcfcs(cmd+1, cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	return OK;
	
}


u8 do_send_text_ok(SOCKET s, u8 mode, u8 index, u8 cmd)
{
	u8 snd[10];

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 0x01;
	snd[2] = cmd;
	*(u32*)(snd+3) = sys.machine_id;
	snd[7] = index;
	snd[8] = NET_UDP_RSP_OK;
	snd[snd[1]+7] = calcfcs(snd+1, snd[1]+6);

	if(mode)
		wrap_udp_sendto(s, snd, snd[1]+8);
	else
		gsm_gprs_send(snd, snd[1]+8);
	
	return OK;
}


u8 do_send_ok(SOCKET s, u8 mode, u8 cmd)
{
	u8 snd[9];

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 0x01;
	snd[2] = cmd;
	*(u32*)(snd+3) = sys.machine_id;
	snd[7] = NET_UDP_RSP_OK;
	snd[snd[1]+7] = calcfcs(snd+1, snd[1]+6);

	if(mode)
		wrap_udp_sendto(s, snd, snd[1]+8);
	else
		gsm_gprs_send(snd, snd[1]+8);
	
	return OK;
}


u8 do_tcp_send_ok(SOCKET s, u8 mode, u8 cmd)
{
	u8 snd[5];

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 0x01;
	snd[2] = cmd;
	snd[3] = NET_TCP_RSP_OK;
	snd[4] = calcfcs(snd+1, snd[1]+2);

	if(mode)
		send(s, snd, snd[1]+4, (bool)0);
	else
		gsm_gprs_send(snd, snd[1]+4);
	
	return OK;
}

u8 do_upgrade_end_rsp(SOCKET s, u8 mode, u8 cmd, u8 cmd1)
{
	u8 snd[5];

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 0x01;
	snd[2] = cmd;
	snd[3] = cmd1;
	snd[4] = calcfcs(snd+1, snd[1]+2);

	if(mode)
		send(s, snd, snd[1]+4, (bool)0);
	else
		gsm_gprs_send(snd, snd[1]+4);
	
	return OK;
}


u8 do_tcp_upgrade_start_rsp(SOCKET s, u8 mode, int index)
{
	u8 snd[6];

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 0x02;
	snd[2] = NET_TCP_UPGRADE_RSP;
	snd[3] = index;
	snd[4] = NET_TCP_RSP_OK;
	snd[5] = calcfcs(snd+1, snd[1]+2);

	if(mode)
		send(s, snd, snd[1]+4, (bool)0);
	else
		gsm_gprs_send(snd, snd[1]+4);
	
	return OK;
}

u8 do_tcp_send_live_rsp(SOCKET s, u8 mode)
{
	u8 snd[5];

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 0;
	snd[2] = NET_TCP_ALIVE_RSP;
	snd[3] = calcfcs(snd+1, snd[1]+2);

	if(mode)
		send(s, snd, snd[1]+4, (bool)0);
	else
		gsm_gprs_send(snd, snd[1]+4);
	
	return OK;
}

u8 do_tcp_get_para(SOCKET s, u8 mode)
{
	u8 snd[33];
	u32 id=0;

	printf("get_para\n");

	if(mode)
		snd[0] = FRAME_HEAD;
	else
		snd[0] = GPRS_FRAME_HEAD;
	snd[1] = 28;
	snd[2] = NET_TCP_GET_PARA_RSP;

	get_ip_addr_from_eeprom(snd+3);
	get_mask_from_eeprom(snd+7);
	get_gate_from_eeprom(snd+11);
	get_id_from_eeprom(&id);
	*(u32*)(snd+15) = id;
	get_dest_ip_from_eeprom(snd+19);
	get_port_from_eeprom((u16*)(snd+23));
	get_mac_from_eeprom(snd+25);

	snd[snd[1]+3] = calcfcs(snd+1, snd[1]+2);

	if(mode)
		send(s, snd, snd[1]+4, (bool)0);
	else
		gsm_gprs_send(snd, snd[1]+4);
	
	return OK;
}


int deal_recv_one_cmd(SOCKET s, u8 mode, peer_info* l_peer, unsigned char* cmd)
{
	uint16 len;
	u8 rcv[512];
	u8 cell_num[12];
	u8 text_content[17];
	int i;

	len = cmd[1];
	
	if(cmd[2] == NET_UDP_SET_DETECTOR_SENS)
		printf("recv NET_UDP_SET_DETECTOR_SENS\n");
	else if(cmd[2] == NET_UDP_ALIVE_RSP)
		//printf("recv udp live in recv_loop\n");
		;
	else if(cmd[2] == NET_UDP_ARMING)
	{
		printf("recv arming order in recv_loop\n");
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
		{
			sys.arming = *(u32*)(cmd+7);
			printf("recv arming: 0x%x\n", sys.arming);
			//save_arming_to_flash(arming);
			save_arming_to_eeprom(sys.arming);
			if(!sys.arming)
				do_disarm_link();
			else
				do_arm_link();
			do_send_ok(s, mode, NET_UDP_ARMING_RSP);
		}
	}
	else if(cmd[2] == NET_UDP_DISARMING)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
		{
			sys.arming = *(u32*)(cmd+7);
			printf("recv disarming: 0x%x\n", sys.arming);
			save_arming_to_eeprom(sys.arming);
			do_send_ok(s, mode, NET_UDP_DISARMING_RSP);
		}
	}
	else if(cmd[2] == NET_UDP_SET_TIME)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
		{
			ds1302_set_time(cmd+7);
			do_send_ok(s, mode, NET_UDP_SET_TIME_RSP);
		}
	}
	else if(cmd[2] == NET_UDP_GET_TIME)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
			do_udp_get_time(s, mode);
			;
	}
	else if(cmd[2] == NET_UDP_STAT_GET)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
			do_udp_stat_get(s, mode);
	}	
	else if(cmd[2] == NET_UDP_LINK_SET)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
			do_udp_link_set(s, cmd+1, mode);
	}	

	else if(cmd[2] == NET_UDP_SET_CELL_NUM)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
		{
			if(cmd[1] > 1)
			{
				sys.cell_alarm = 1;
				printf("recv set sell num\n");
				do_udp_set_cell_num(s, cmd+1, mode);
				
			
				Delay_ms(20);
				get_cell_num_from_eeprom(0, cell_num);
				for(i=0; i<11; i++)
					printf("cell_num: %d\n", cell_num[i]);
			}
			else 
				sys.cell_alarm = 0;
		}
	}
	else if(cmd[2] == NET_UDP_SET_TEXT_NUM)
	{
		printf("recv set text num 1\n");
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
		{	
			printf("recv set text num 2\n");
			do_udp_set_cell_text(s, cmd, mode, cmd[7]);;

			
			Delay_ms(20);
			get_text_num_from_eeprom(cmd[7], cell_num);
			for(i=0; i<11; i++)
				printf("text_num: %d\n", cell_num[i]);
			get_text_content_from_eeprom(cmd[7], sys.text.text_length[0], text_content);
			printf("text_content: %s\n", text_content);
			
		}
	}
	else if(cmd[2] == NET_UDP_SET_DETECTOR_SENS)
		;
	else if(cmd[2] == NET_UDP_GET_ARM_STAT)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
			do_udp_get_arm_stat(s, mode);
	}
	else if(cmd[2] == NET_UDP_GET_LINK_STAT)
	{
		if(cmd[len+7] == calcfcs(cmd+1, len+6))
			do_udp_get_link_stat(s, mode);
	}
	else if(cmd[2] == NET_UDP_GET_DETECTOR_SENS)
		;
	else if(cmd[2] == NET_UDP_CANCEL_TEXT_ALARM)
		do_udp_cancel_text_alarm(s, mode, cmd[7]);
	else if(cmd[2] == NET_UDP_CANCEL_CELL_ALARM)
		do_udp_cancel_cell_alarm(s, mode, cmd[7]);
	else if(cmd[2] == NET_UDP_GET_BALANCE)
		do_udp_get_balance_rsp(s, mode, cmd[7]);
	else if(cmd[2] == NET_UDP_SET_ARM_LINK)
		do_udp_set_arm_link(s, mode, cmd);
	else if(cmd[2] == NET_UDP_SET_DISARM_LINK)
		do_udp_set_disarm_link(s, mode, cmd);
	else if(cmd[2] == NET_UDP_GET_ARM_LINK)
		do_udp_get_arm_link_rsp(s, mode);
	else if(cmd[2] == NET_UDP_GET_DISARM_LINK)
		do_udp_get_disarm_link_rsp(s, mode);
	else if(cmd[2] == NET_UDP_GET_CELL_NUM)
		do_udp_get_cell_rsp(s, mode);
	else if(cmd[2] == NET_UDP_GET_TEXT_NUM)
	{
		printf("do_udp_get_text_rsp\n");
		do_udp_get_text_rsp(s, mode, cmd[7]);
	}
	else if(cmd[2] == NET_UDP_INPUT_ALARM_STAT_RSP)
		try_again[NET_UDP_INPUT_ALARM_STAT] = 0;
	else if(cmd[2] == NET_UDP_DETECTOR_LOSS_RSP)
		try_again[NET_UDP_DETECTOR_LOSS] = 0;
	else if(cmd[2] == NET_UDP_ALARM_BATTERY_LOW_POWER_RSP)
		try_again[NET_UDP_ALARM_BATTERY_LOW_POWER] = 0;
	else if(cmd[2] == NET_UDP_ALARM_NET_LOSS_RSP)
		try_again[NET_UDP_ALARM_NET_LOSS] = 0;
	else if(cmd[2] == NET_UDP_ALARM_POWEROFF_RSP)
		try_again[NET_UDP_ALARM_POWEROFF] = 0;
	else if(cmd[2] == NET_UDP_UPLOAD_ARM_STAT_RSP)
		try_again[NET_UDP_UPLOAD_ARM_STAT] = 0;
	else 
		;

	return OK;
}


int recv_loop(SOCKET s, u8 mode, u8 cmd, u8 need_recv)
{
	int i, a, b;
	int ret=ERROR;
	char* ptr;
	int len;
	int parse_len;
	u8 one_command = 0;
	int RSR_len;
	u8 dest_ip[4];
	uint16 port;
	unsigned char one_cmd[512]={0};
	unsigned char buf[512]= {0};
	ptr = l_peer.recv_buf;

	if(!mode)
	{
		recv_one_cmd_from_gprs(one_cmd, &one_command, need_recv);
		if(one_command)
		{
			deal_recv_one_cmd(s, mode, &l_peer, one_cmd);
			if(one_cmd[2] == cmd)
				ret = OK;
		}
	}
	else
	{
		if((RSR_len = getSn_RX_RSR(s)) > 0)
		{
			ret = recvfrom(s, buf, RSR_len, dest_ip, &port);
			if(ret <= 0)
				goto error;
			debug_msg("recv len: %d, ip:%d.%d.%d.%d, port: %d\n", ret, dest_ip[0],dest_ip[1],dest_ip[2],dest_ip[3], port);
			for(i=0; i<ret; i++)
				debug_msg("%02x ", buf[i]);
			debug_msg("\n");

			if(dest_ip[0] != sys.dest_net_para.destip[0] &&
				dest_ip[1] != sys.dest_net_para.destip[1] &&
				dest_ip[2] != sys.dest_net_para.destip[2] &&
				dest_ip[3] != sys.dest_net_para.destip[3] &&
				port != sys.dest_net_para.port)
			goto error;
			
			parse_len = ret;
			b = parse_len;
			
			//debug_msg("line: %d\n", __LINE__);
			while(parse_len > 0)
			{
				if(l_peer.index != 0)
				{
					//debug_msg("line: %d, l_peer->index: %d\n", __LINE__, l_peer.index);
					memcpy(one_cmd, ptr, l_peer.index);
					if(l_peer.index == 1)
					{
						len = buf[0];
						if(parse_len >= len+7)
						{
							//debug_msg("line: %d, l_peer->index: %d\n", __LINE__, l_peer.index);
							memcpy(one_cmd+l_peer.index, buf, len+7);
							parse_len -= (len+7);
							deal_recv_one_cmd(s, mode, &l_peer, one_cmd);
							if(one_cmd[2] == cmd)
								ret = OK;
							l_peer.index = 0;
						}
						else
						{
							memcpy(one_cmd+l_peer.index, buf, parse_len);
							memcpy(ptr, one_cmd, parse_len+l_peer.index);
							l_peer.index = parse_len+l_peer.index;
							parse_len -= parse_len;
							//debug_msg("line: %d, l_peer->index: %d\n", __LINE__, l_peer.index);
						}
					}
					else
					{
						len = one_cmd[1];
						a = len+8-l_peer.index;
						if(parse_len >= a)
						{	
							//debug_msg("line: %d, l_peer->index: %d\n", __LINE__, l_peer.index);
							memcpy(one_cmd+l_peer.index, buf, a);
							parse_len -= a;
							deal_recv_one_cmd(s, mode, &l_peer, one_cmd);
							if(one_cmd[2] == cmd)
								ret = OK;
							l_peer.index = 0;
						}
						else
						{
							memcpy(one_cmd+l_peer.index, buf, parse_len);
							memcpy(ptr, one_cmd, parse_len+l_peer.index);
							l_peer.index = parse_len+l_peer.index;
							parse_len -= parse_len;
							//debug_msg("line: %d, l_peer->index: %d\n", __LINE__, l_peer.index);
						}	
					}
				}
				else
				{	
					/*
					while((buf[b-parse_len] != 0xFD) && (buf[b-parse_len] != 0xFE))
						parse_len--;
					*/
					
					//debug_msg("line: %d, b: %d, parse_len: %d\n", __LINE__, b, parse_len);
					if((buf[b-parse_len] == 0xFE))
					{	
						//memcpy(one_cmd, buf+(b-parse_len), 1);
						one_cmd[0] = buf[b-parse_len];
						parse_len--;
						//debug_msg("line: %d\n", __LINE__);
						if(parse_len > 0)
						{	
							//memcpy(one_cmd+1, buf+(b-parse_len), 1);
							one_cmd[1] = buf[b-parse_len];
							parse_len--;
							len = one_cmd[1];
							//debug_msg("line: %d, len: %d, \n", __LINE__, len);
							if(parse_len < len+6)
							{
								//debug_msg("line: %d\n", __LINE__);
								l_peer.index = 2+parse_len;
								if(parse_len != 0)
								{
									memcpy(ptr, one_cmd, 2);
									memcpy(ptr+2, buf+(b-parse_len), parse_len);
									parse_len -= parse_len;
								}
							}
							else
							{
							//debug_msg("line: %d\n", __LINE__);
								memcpy(one_cmd+2, buf+(b-parse_len), len+6);
								parse_len -= len+6;
								deal_recv_one_cmd(s, mode, &l_peer, one_cmd);
								if(one_cmd[2] == cmd)
									ret = OK;
							}
						}
						else
						{
							l_peer.index = 1;
							*ptr = buf[b-parse_len-1];
						}
					}
					else
						parse_len--;
				}
			}
		}
	}
error:
	return ret;
}


int recv_one_command(SOCKET s, u8 mode, u8* rcv, u8* one_command)
{
	uint16 len=0;
	u8 ch=0;
	u8 ret=ERROR;
	u8 recv_flag=10;

	if(mode)
	{	
		if((len = getSn_RX_RSR(s))>0)
		{
			while(ch != FRAME_HEAD && (len>0))
			{
				if(recv_n_char(s, &ch, 1) == OK)
					len--;
				else
					goto error;
			}
			if((ch == FRAME_HEAD) && (len > 0))
			{
				if(recv_n_char(s, rcv, 1) == OK)
					len--;
				else
					goto error;
			}
			else 
				goto error;
			
			while((len = getSn_RX_RSR(s)) < (rcv[0]+2) && recv_flag )
			{
				Delay_ms(100);
				recv_flag--;
			}

			if(len >= rcv[0]+2)
			{
				if(recv_n_char(s, rcv+1, rcv[0]+2) == OK)
				{
					*one_command = 1;
					ret = OK;
				}
			}
				
		}

	}
	else
		recv_one_cmd_from_gprs(rcv, one_command, 0);

error:
	
	return ret;
}
int server_recv_loop(SOCKET s, u8 mode, u8 cmd)
{	
	int ret=ERROR;
	uint16 len;
	int l_len;
	u8 rcv[512];
	int i;
	static u8 upgrade_buf[2048];
	static u8 index=0;
	static int offset=0;
	static u32 img_len=0;
	static u8 upgrade_flag=NET_TCP_RSP_OK;
	static int flash_page_index=0;
	
	u8 one_command=0;


	recv_one_command(s, mode, rcv, &one_command);
	if(one_command)
	{	
		if (rcv[1] == cmd)
			ret = OK;
			
		len = rcv[0];
		switch(rcv[1])
		{	
			case NET_TCP_SET_PARA:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					//ip
					printf("set ip:");
					for(i=0; i<4; i++)
					{
						sys.net_para.Lip[i]=rcv[i+2];
						printf("%d.", rcv[i+2]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.net_para.Lip,'#');
					
					//mask
					printf("set mask:");
					for(i=0; i<4; i++)
					{
						sys.net_para.Sub[i]=rcv[i+6];
						printf("%d.", rcv[i+6]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.net_para.Sub,'%');
					//gateway
					printf("set gateway:");
					for(i=0; i<4; i++)
					{
						sys.net_para.Gw[i]=rcv[i+10];
						printf("%d.", rcv[i+10]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.net_para.Gw,'$');
					//id
					sys.machine_id = *(u32*)(rcv+14);
					printf("set machine id: %u\n", sys.machine_id);
					save_id_to_eeprom(sys.machine_id);

					//D IP
					printf("set destip:");
					for(i=0; i<4; i++)
					{
						sys.dest_net_para.destip[i]=rcv[i+18];
						printf("%d.", rcv[i+18]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.dest_net_para.destip,'!');

					//D PORT
					sys.dest_net_para.port = *(u16*)(rcv+22);
					printf("set dest port: %u\n", sys.dest_net_para.port);
					save_port_to_eeprom(sys.dest_net_para.port);

					//MAC
					printf("set mac:");
					for(i=0; i<6; i++)
					{
						sys.net_para.Mac[i]=rcv[i+24];
						printf("%d.", rcv[i+24]);
					}
					printf("\n");
					save_mac_to_eeprom(sys.net_para.Mac);
					do_tcp_send_ok(s,mode,NET_TCP_SET_PARA_RSP);
				}
				break;
			case NET_TCP_UPGRADE_START:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{
					printf("upgrade start index: %d, rcv[2]: %d\n", index, rcv[2]);
					//index = rcv[2];
					//if(index == 0)
					//{
					//	for(i=0; i<len-1; i++)
					//		printf("%x", rcv[3+i]);
					//	printf("\n");
					//}

					if(index != rcv[2])
						upgrade_flag = NET_TCP_RSP_ERROR;

					
					
					if(MAX_FLASH_PAGE_CAPICITY-offset >= len-1)
					{
						memcpy(upgrade_buf+offset, rcv+3, len-1);
						offset += len-1;
					}
					else
					{
						if(MAX_FLASH_PAGE_CAPICITY-offset > 0)
						{
							l_len = MAX_FLASH_PAGE_CAPICITY-offset;
							memcpy(upgrade_buf+offset, rcv+3, l_len);
							do_flash_upgrade(upgrade_buf, MAX_FLASH_PAGE_CAPICITY, flash_page_index);
							flash_page_index++;
							offset = 0;
							memcpy(upgrade_buf+offset, rcv+3+l_len, (len-1)-l_len);
							offset += (len-1)-l_len;
						}
						else
						{
							if(flash_page_index == 0)
							{
								//for(i=0; i<MAX_FLASH_PAGE_CAPICITY; i++)
								//	printf("%x", upgrade_buf[i]);
								//printf("\n");
							}
							do_flash_upgrade(upgrade_buf, MAX_FLASH_PAGE_CAPICITY, flash_page_index);
							flash_page_index++;
							offset = 0;
							memcpy(upgrade_buf+offset, rcv+3, (len-1));
							offset += len-1;
						}
							
					}
					img_len += (len-1);
					do_tcp_upgrade_start_rsp(s, mode, index);
					index++;
				}
				break;
			case NET_TCP_UPGRADE_END:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("upgrade end img_len: %d\n", img_len);
					if(offset != 0)
						do_flash_upgrade(upgrade_buf, offset, flash_page_index);
					flash_page_index = 0;
					offset = 0;
					do_write_upgrade_flag(img_len);
					img_len = 0;
					do_upgrade_end_rsp(s, mode, NET_TCP_UPGRADE_END_RSP, upgrade_flag);
					upgrade_flag = NET_TCP_RSP_OK;
					index=0;
				}
				break;
			case NET_TCP_ALIVE:
				printf("recv client live\n");
				do_tcp_send_live_rsp(s, mode);
				break;
			case NET_TCP_SET_IP:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("set ip:");
					for(i=0; i<4; i++)
					{
						sys.net_para.Lip[i]=rcv[i+2];
						printf("%d.", rcv[i+2]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.net_para.Lip,'#');
					do_tcp_send_ok(s,mode,NET_TCP_SET_IP_RSP);
				}
				break;
			case NET_TCP_SET_GATEWAY:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("set gateway:");
					for(i=0; i<4; i++)
					{
						sys.net_para.Gw[i]=rcv[i+2];
						printf("%d.", rcv[i+2]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.net_para.Gw,'$');
					do_tcp_send_ok(s,mode,NET_TCP_SET_GATEWAY_RSP);
				}
				break;
			case NET_TCP_SET_MASK:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("set mask:");
					for(i=0; i<4; i++)
					{
						sys.net_para.Sub[i]=rcv[i+2];
						printf("%d.", rcv[i+2]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.net_para.Sub,'%');
					do_tcp_send_ok(s,mode,NET_TCP_SET_MASK_RSP);
				}
				break;
			case NET_TCP_SET_MAC:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("set mac:");
					for(i=0; i<6; i++)
					{
						sys.net_para.Mac[i]=rcv[i+2];
						printf("%d.", rcv[i+2]);
					}
					printf("\n");
					save_mac_to_eeprom(sys.net_para.Mac);
					do_tcp_send_ok(s,mode,NET_TCP_SET_MAC_RSP);
				}
				break;
			case NET_TCP_SET_ID:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					sys.machine_id = *(u32*)(rcv+2);
					printf("set machine id: %u\n", sys.machine_id);
					save_id_to_eeprom(sys.machine_id);
					do_tcp_send_ok(s,mode,NET_TCP_SET_ID_RSP);
				}
				break;
			case NET_TCP_SET_DESTINATION_IP:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("set destip:");
					for(i=0; i<4; i++)
					{
						sys.dest_net_para.destip[i]=rcv[i+2];
						printf("%d.", rcv[i+2]);
					}
					printf("\n");
					save_ip_to_eeprom(sys.dest_net_para.destip,'!');
					do_tcp_send_ok(s,mode,NET_TCP_SET_DESTINATION_IP_RSP);
				}
				break;
			case NET_TCP_SET_DESTINATION_PORT:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					sys.dest_net_para.port = *(u16*)(rcv+2);
					printf("set dest port: %u\n", sys.dest_net_para.port);
					save_port_to_eeprom(sys.dest_net_para.port);
					do_tcp_send_ok(s,mode,NET_TCP_SET_DESTINATION_PORT_RSP);
				}
				
				break;
			case NET_TCP_GET_PARA:
				if(rcv[len+2] == calcfcs(rcv, len+2))
					do_tcp_get_para(s, mode);
				break;
			case NET_TCP_REBOOT:
				if(rcv[len+2] == calcfcs(rcv, len+2))
				{	
					printf("reboot\n");
					SoftReset();
					reset_sim900a();
					//Reset_W5500();
					//reset_sim900a();
				}
				break;
			
			default:
				break;
		}
	
			
	}
	return ret;
}


int do_udp_alarm_input_stat(SOCKET s, u32 data, u8 mode)
{	
	u8 cmd[13];


	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_INPUT_ALARM_STAT;
	*(u32*)(cmd+3) = sys.machine_id;
	cmd[7] = data & 0xff;
	cmd[8] = (data & 0xff00)>>8;
	cmd[9] = (data & 0xff0000)>>16;
	cmd[10] = (data & 0xff000000)>>24;
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);

	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	if(!do_common_rsp(s, mode, NET_UDP_INPUT_ALARM_STAT_RSP))
		try_again[NET_UDP_INPUT_ALARM_STAT] = 0;
	else
	{	
		try_again[NET_UDP_INPUT_ALARM_STAT] = 1;
		try_data[NET_UDP_INPUT_ALARM_STAT] = data;
	}
	
	return OK;
		
}

int do_udp_detector_loss(SOCKET s, u32 data, u8 mode)
{	
	u8 cmd[13];

	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_DETECTOR_LOSS;
	*(u32*)(cmd+3) = sys.machine_id;
	cmd[7] = data & 0xff;
	cmd[8] = (data & 0xff00)>>8;
	cmd[9] = (data & 0xff0000)>>16;
	cmd[10] = (data & 0xff000000)>>24;
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);

	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	if(!do_common_rsp(s, mode, NET_UDP_DETECTOR_LOSS_RSP))
		try_again[NET_UDP_DETECTOR_LOSS] = 0;
	else
	{	
		try_again[NET_UDP_DETECTOR_LOSS] = 1;
		try_data[NET_UDP_DETECTOR_LOSS] = data;
	}
	
	return OK;
	
}

int do_udp_net_loss()
{	
	u8 cmd[8];

	
	cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 0;
	cmd[2] = NET_UDP_ALARM_NET_LOSS;
	*(u32*)(cmd+3) = sys.machine_id;
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);

	gsm_gprs_send(cmd, cmd[1]+8);
	if(!do_common_rsp(0, 0, NET_UDP_ALARM_NET_LOSS_RSP))
		try_again[NET_UDP_ALARM_NET_LOSS]=0;
	else
		try_again[NET_UDP_ALARM_NET_LOSS]=1;
	
	return OK;
}

int do_udp_poweroff(SOCKET s, u8 mode)
{
	u8 cmd[8];

	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 0;
	cmd[2] = NET_UDP_ALARM_POWEROFF;
	*(u32*)(cmd+3) = sys.machine_id;
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);

	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	if(!do_common_rsp(s, mode, NET_UDP_ALARM_POWEROFF_RSP))
		try_again[NET_UDP_ALARM_POWEROFF] = 0;
	else
		try_again[NET_UDP_ALARM_POWEROFF] = 1;
	
	return OK;
}

int do_udp_lowbatterypow(SOCKET s, u8 mode)
{
	u8 cmd[4];


	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 0;
	cmd[2] = NET_UDP_ALARM_BATTERY_LOW_POWER;
	*(u32*)(cmd+3) = sys.machine_id;
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);

	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	if(!do_common_rsp(s, mode, NET_UDP_ALARM_BATTERY_LOW_POWER_RSP))
		try_again[NET_UDP_ALARM_BATTERY_LOW_POWER] = 0;
	else
		try_again[NET_UDP_ALARM_BATTERY_LOW_POWER] = 1;
	
	return OK;
}


int do_udp_upload_arm_stat(SOCKET s, u8 val, u8 mode)
{	
	u8 cmd[13];

	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_UPLOAD_ARM_STAT;
	*(u32*)(cmd+3) = sys.machine_id;
	if(val)
	{
		cmd[7] = 0xff;
		cmd[8] = 0xff;
		cmd[9] = 0xff;
		cmd[10] = 0xff;
	}
	else
	{
		cmd[7] = 0x00;
		cmd[8] = 0x00;
		cmd[9] = 0x00;
		cmd[10] = 0x00;
	}
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+8);
	else
		gsm_gprs_send(cmd, cmd[1]+8);
	if(!do_common_rsp(s, mode, NET_UDP_UPLOAD_ARM_STAT_RSP))
		try_again[NET_UDP_UPLOAD_ARM_STAT]= 0;
	else
	{
		try_data[NET_UDP_UPLOAD_ARM_STAT] = val;
		try_again[NET_UDP_UPLOAD_ARM_STAT]= 1;
	}
	
	return OK;
}


u8 check_live_rsp(u8* cmd, int len)
{
	u8 ret=ERROR;

	if((len == 4) && cmd[0] == 0xfe && cmd[2] == NET_UDP_ALIVE_RSP)
	{
		ret = OK;
	}


	return ret;
}

int do_udp_alive(SOCKET s, u8 mode)
{	
	u8 cmd[8];
	int time_flag, gprs_time_flag, a=ERROR;
	static u8 i=0;
	
	static uint32_t keep_alive_time;
	
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 0;
	cmd[2] = 0;
	*(u32*)(cmd+3) = sys.machine_id;
	cmd[cmd[1]+7] = calcfcs(cmd+1,cmd[1]+6);
	
	if (time_return()- keep_alive_time > (tick_second*LIVE_INTERVAL))
	{	
		if(!mode)
		{
			gsm_gprs_send(cmd, cmd[1]+8);
			gprs_time_flag=10;
			while(((a=recv_loop(s, mode, NET_UDP_ALIVE_RSP, 0))!= OK && gprs_time_flag))
			{	
				gprs_time_flag--;
				//printf("wait for live rsp in while\n");
				Delay_ms(500);
			}
			if(a==OK)
			{
				printf("recv gprs live\n");
				sys.gprs_flag=0;
			
			}
			else
			{
				printf("recv gprs live timeout\n");
				sys.gprs_flag++;
			}
			if(i==LIVE_INTERVAL*2)
			{
				i=0;
				cmd[0] = FRAME_HEAD;
				printf("..........check udp in gprs mode\n");
				wrap_udp_sendto(s, cmd, cmd[1]+8);
				time_flag=10;
				while(((a=recv_loop(s, UDP_MODE, NET_UDP_ALIVE_RSP, 0))!= OK && time_flag))
				{	
					time_flag--;
					//printf("wait for live rsp in while\n");
					Delay_ms(100);
				}
				if(a==OK)
				{
					printf("..............recv udp live in gprs mode\n");
					sys.udp_flag=0;
				}
				else
					printf("..............recv udp live timeout in gprs mode\n");
			}
			else
				i++;
		}
		else
		{
			wrap_udp_sendto(s, cmd, cmd[1]+8);
			time_flag=10;
			while(((a=recv_loop(s, mode, NET_UDP_ALIVE_RSP, 0))!= OK && time_flag))
			{	
				time_flag--;
				//printf("wait for live rsp in while\n");
				Delay_ms(100);
			}
			if(a==OK)
			{
				printf("recv udp live in net mode\n");
				sys.udp_flag=0;
			}
			else
			{	
				printf("recv udp live in net mode timeout\n");
				sys.udp_flag++;
			}
		}
		keep_alive_time = time_return();
	}
	
	return OK;
}

u8 get_alarm_stat(SOCKET s, u32* status)
{
	u8 i;
	u32 stat=0xffff0000;

	for(i=0; i<16; i++)
		stat |= (get_alarm_in(i)<<i);
	//printf("send alarm, stat: 0x%x\n", stat);
	//if ((stat & 0xffff) != 0xffff)

	*status = ~stat;
	
	return OK;
}

u8 get_lowbatterypow_stat(SOCKET s, u8 mode)
{
	u8 ret;
	static u8 stat=0;
	//ret = is_battery_lowpow();
	
	if(stat != ret)
	{
		if (ret)
			do_udp_lowbatterypow(s, mode);
	}
	stat = ret;
	return OK;
}

u8 get_poweroff_stat(SOCKET s, u8 mode)
{
	u8 ret;
	static u8 stat=0;
	//ret = is_poweroff();
	if(ret)
	{
		get_lowbatterypow_stat(s, mode);
	}
	if(stat != ret)
	{
		if (ret)
			do_udp_poweroff(s, mode);
	}
	stat = ret;
	return OK;
}


u8 get_arm_stat(SOCKET s, u8 mode)
{
	u8 ret;
	static u8 stat=1;
	static u8 disarm=1;

	
	ret = get_key(0);
	if(stat != ret)
	{
		if (ret == 0)
			do_udp_upload_arm_stat(s, 1, mode);
	}
	stat = ret;
	
	ret = get_key(1);
	if(disarm != ret)
	{
		if (ret == 0)
			do_udp_upload_arm_stat(s, 0, mode);
	}
	disarm = ret;
	
	return OK;
}

u8 cruise_io(SOCKET s, u8 mode)
{
	static uint32_t cruise_interval;
	u32 status=0;

	
	if(sys.arming != 0)
	{	
		//printf("in arimng cruise\n");
		if (time_return()- cruise_interval > (tick_second*1))
		{	
			get_alarm_stat(s, &status);
			if(status)

			{
				do_udp_alarm_input_stat(s, status, mode);
				do_alarm_link(status);
			}
			cruise_interval = time_return();
		}
	}

	get_poweroff_stat(s, mode);
	//get_lowbatterypow_stat(s, mode);
	get_arm_stat(s, mode);

	return OK;
}

//0:GPRS mode 1:UDP mode

u8 check_net_mode(SOCKET s, u8* mode)
{
	u8 ret=ERROR;

	if(*mode)
	{
		if(sys.udp_flag > MAX_LIVE_TIMES)
		{
			*mode = 0;
			printf("going in gprs mode\n");
			if(sys.gprs_flag > MAX_LIVE_TIMES)
			{
				if(sys.gsm_stat == GSM_STAT_GPRS_INIT || sys.gsm_stat == GSM_STAT_INIT)
				{
					do_cell_alarm();
					text_alarm();
				}
			}
		}
		
	}
	else
	{
		if(sys.gprs_flag > MAX_LIVE_TIMES)
		{
			*mode = 1;
			printf("going in udp mode cause of recv live timeout in gprs mode\n");
		}
		else if(sys.udp_flag == 0)
		{	
			*mode = 1;
			printf("going in udp mode cause of recv live in gprs mode\n");
		}
	}
	
	ret = OK;
	return ret;
}


u8 set_udp_utils()
{
	set_current_time(&presentTime);
	set_current_time(&presentTime1);
	set_current_time(&try_again_time);
	return OK;
}


u8 check_try_again(SOCKET s, u8 mode)
{
	
	if(time_return() - try_again_time >= (tick_second * 5)) 
	{
		try_again_time = time_return();
		//printf("SOCK_ESTABLISHED\n");
			
		if(try_again[NET_UDP_INPUT_ALARM_STAT])
			do_udp_alarm_input_stat(s,try_data[NET_UDP_INPUT_ALARM_STAT],mode);

		if(try_again[NET_UDP_DETECTOR_LOSS])
			do_udp_detector_loss(s,try_data[NET_UDP_DETECTOR_LOSS], mode);

		if(try_again[NET_UDP_ALARM_BATTERY_LOW_POWER])
			do_udp_lowbatterypow(s,mode);

		if(try_again[NET_UDP_ALARM_NET_LOSS])
			do_udp_net_loss();
		
		if(try_again[NET_UDP_ALARM_POWEROFF])
			do_udp_poweroff(s,mode);

		if(try_again[NET_UDP_UPLOAD_ARM_STAT])
			do_udp_upload_arm_stat(s,try_data[NET_UDP_UPLOAD_ARM_STAT], mode);
	}
	return OK;
}

void loopback_tcpc(SOCKET s, uint16 port, u8 mode)
{
	uint16 RSR_len;
	uint8 * data_buf = TX_BUF;

	
	if(!mode && (sys.gsm_stat==GSM_STAT_GPRS_INIT))
	{
		do_udp_alive(s, mode);
		cruise_io(s, mode);
		recv_loop(s, mode, NET_UDP_VALID_CMD, 0);
		check_try_again(s, mode);
	}
	
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:                 /* if connection is established */
		//printf("SOCK_ESTABLISHED\n");
		if(time_return() - presentTime >= (tick_second * 2)) 
		{  
			presentTime = time_return();
			printf("SOCK_ESTABLISHED\n");
		}
		if(ch_status[s]==1 || ch_status[s] == 4)
		{
			 printf("\r\n%d : Connected",s);
			 ch_status[s] = 2;
		}

		do_udp_alive(s, mode);
		cruise_io(s, mode);
		recv_loop(s, mode, NET_UDP_VALID_CMD, 0);
		check_try_again(s, mode);

		break;
	case SOCK_CLOSE_WAIT:  
		if(time_return() - presentTime >= (tick_second * 2)) 
		{  
			presentTime = time_return();
			printf("SOCK_CLOSE_WAIT\n");
		}
		/* If the client request to close */
		//printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
		{
			 if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			                                                                            /* the data size to read is MAX_BUF_SIZE. */
			 recv(s, data_buf, RSR_len);         /* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED: 
		//printf("SOCK_CLOSED\n");
		if(time_return() - presentTime >= (tick_second * 2)) 
		{  
			presentTime = time_return();
			printf("SOCK_CLOSED\n");
		}
		/* if a socket is closed */
		if(!ch_status[s])
		{
			 printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
			 ch_status[s] = 1;
		}
		if(socket(s, Sn_MR_TCP, sys.net_para.any_port++, 0x00) == 0)    /* reinitialize the socket */
		{
			 printf("\a%d : Fail to create socket.",s);
			 ch_status[s] = 0;
		}
		break;
	case SOCK_INIT:     /* if a socket is initiated */
		//printf("SOCK_INIT\n");
		if(ch_status[s] == 2)
			ch_status[s] = 4;
		if(time_return() - presentTime >= (tick_second * 2)) 
		{  
			connect(s, sys.dest_net_para.destip, sys.dest_net_para.port);
			presentTime = time_return();
			printf("SOCK_INIT destip: %d.%d.%d.%d. port: %d\n", sys.dest_net_para.destip[0], sys.dest_net_para.destip[1], 
				sys.dest_net_para.destip[2], sys.dest_net_para.destip[3], sys.dest_net_para.port);
		}
		break;
	default:
		if(time_return() - presentTime >= (tick_second * 2)) 
		{  
			presentTime = time_return();
			printf("default\n");
		}
		break;
		
	}
}


void handle_unhandled_cmd()
{
	if(sys.unsettled_flag)
	{
		printf("handle_unhandled_cmd\n");
		deal_recv_one_cmd(1, 0, NULL, sys.unsettled_cmd);
		sys.unsettled_flag = 0;
	}
}



void loopback_udp(SOCKET s, uint16 port, u8 mode)
{
		
	if(!mode && (sys.gsm_stat==GSM_STAT_GPRS_INIT))
	{
		handle_unhandled_cmd();
		do_udp_alive(s, mode);
		cruise_io(s, mode);
		recv_loop(s, mode, NET_UDP_VALID_CMD, 0);
		check_try_again(s, mode);
	}
	else
	{
	   switch (getSn_SR(s))
	   {
	   case SOCK_UDP:
	   		if(time_return() - presentTime >= (tick_second * 2)) 
			{  
				presentTime = time_return();
				printf("SOCK_UDP\n");
			}
	   		do_udp_alive(s, mode);
			//cruise_io(s, mode);
			recv_loop(s, mode, NET_UDP_VALID_CMD, 0);
			//check_try_again(s, mode);
	  
	      break;
	   case SOCK_CLOSED: 
			/* if a socket is closed */
			if(time_return() - presentTime >= (tick_second * 2)) 
			{  
				presentTime = time_return();
				printf("\r\n%d : Loop-Back UDP Started. port :%d", s, port);
			}
			if(socket(s, Sn_MR_UDP, port, 0x00)== 0)    /* reinitialize the socket */
			 	printf("\a%d : Fail to create socket.",s);
			break;
		default:
			if(time_return() - presentTime >= (tick_second * 2)) 
			{  
				presentTime = time_return();
				printf("default\n");
			}
			break;
	   }
	}
}


void loopback_tcps(SOCKET s, uint16 port, u8 mode)
{
	uint16 RSR_len;
	uint16 received_len;
	uint8  data_buf[64];
	
	switch (getSn_SR(s))
	{
		case SOCK_ESTABLISHED:              /* if connection is established */
			if(server_ch_status[s]==1)
			{
				 printf("IP: %d.%d.%d.%d, port:%d connected\n", IINCHIP_READ(Sn_DIPR0(s)),  IINCHIP_READ(Sn_DIPR1(s)), IINCHIP_READ(Sn_DIPR2(s)), IINCHIP_READ(Sn_DIPR3(s)), ( (uint16)(IINCHIP_READ(Sn_DPORT0(s)))<<8) +(uint16)IINCHIP_READ( Sn_DPORT1(s)) );
				 server_ch_status[s] = 2;
			}

			server_recv_loop(s, 1, NET_TCP_VALID_CMD);

			break;
		case SOCK_CLOSE_WAIT:                              /* If the client request to close */
			printf("%d : CLOSE_WAIT\n", s);
			if ((RSR_len = getSn_RX_RSR(s)) > 0)     /* check Rx data */
			{
				if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;  /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */                                                                     /* the data size to read is MAX_BUF_SIZE. */
				received_len = recv(s, data_buf, RSR_len);     /* read the received data */
			}
			disconnect(s);
			server_ch_status[s] = 0;
			break;
		case SOCK_CLOSED:                                       /* if a socket is closed */
			if(!server_ch_status[s])
			{
				printf("%d : Loop-Back TCP Server Started. port : %d\n", s, port);
				server_ch_status[s] = 1;
			}
			if(socket(s,(Sn_MR_ND|Sn_MR_TCP),port,0x00) == 0)    /* reinitialize the socket */
			{
				printf("%d : Fail to create socket.\n",s);
				server_ch_status[s] = 0;
			}
			break;
		case SOCK_INIT:   /* if a socket is initiated */
			listen(s);
			printf("%x :LISTEN socket %d\n",getSn_SR(s), s);
			break;
		default:
		    break;
	}
}


u8 do_udp_test_data(SOCKET s, u8 mode)
{
	u8 cmd[12];
	if(mode)
		cmd[0] = FRAME_HEAD;
	else
		cmd[0] = GPRS_FRAME_HEAD;
	cmd[1] = 4;
	cmd[2] = NET_UDP_TEST;

	//*(s32*)(cmd+3) = test_data;
	cmd[10] = calcfcs(cmd+1, cmd[1]+2);
	if(mode)
		wrap_udp_sendto(s, cmd, cmd[1]+4);
	else
		gsm_gprs_send(cmd, cmd[1]+4);
	return OK;
}

void loopback_udp_test(SOCKET s, uint16 port, u8 mode)
{
	uint16 RSR_len;
	uint8 * data_buf = TX_BUF1;
	u16 l_port = sys.net_para.any_port+100;
	

	
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:                 /* if connection is established */
		//printf("SOCK_ESTABLISHED\n");
		if(time_return() - presentTime1 >= (tick_second * 2)) 
		{  
			presentTime1 = time_return();
			do_udp_test_data(s, mode);
			printf("SOCK_ESTABLISHED\n");
		}
		if(ch_status[s]==1 || ch_status[s] == 4)
		{
			 printf("\r\n%d : Connected",s);
			 ch_status[s] = 2;
		}

		

		break;
	case SOCK_CLOSE_WAIT:  
		if(time_return() - presentTime1 >= (tick_second * 2)) 
		{  
			presentTime1 = time_return();
			printf("SOCK_CLOSE_WAIT\n");
		}
		/* If the client request to close */
		//printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
		{
			 if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			                                                                            /* the data size to read is MAX_BUF_SIZE. */
			 recv(s, data_buf, RSR_len);         /* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED: 
		//printf("SOCK_CLOSED\n");
		if(time_return() - presentTime >= (tick_second * 2)) 
		{  
			presentTime = time_return();
			printf("SOCK_CLOSED\n");
		}
		/* if a socket is closed */
		if(!ch_status[s])
		{
			 printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
			 ch_status[s] = 1;
		}
		if(socket(s, Sn_MR_TCP, l_port++, 0x00) == 0)    /* reinitialize the socket */
		{
			 printf("\a%d : Fail to create socket.",s);
			 ch_status[s] = 0;
		}
		break;
	case SOCK_INIT:     /* if a socket is initiated */
		//printf("SOCK_INIT\n");
		if(ch_status[s] == 2)
			ch_status[s] = 4;
		if(time_return() - presentTime1 >= (tick_second * 2)) 
		{  
			connect(s, sys.dest_net_para.destip, sys.dest_net_para.port+1);
			presentTime1 = time_return();
			printf("SOCK_INIT destip: %d.%d.%d.%d. port: %d\n", sys.dest_net_para.destip[0], sys.dest_net_para.destip[1], 
				sys.dest_net_para.destip[2], sys.dest_net_para.destip[3], sys.dest_net_para.port+1);
		}
		break;
	default:
		if(time_return() - presentTime1 >= (tick_second * 2)) 
		{  
			presentTime1 = time_return();
			printf("default\n");
		}
		break;
	}
}

void loopback_udp_standard(SOCKET s, uint16 port)
{
   uint16 RSR_len;
        uint16 received_len;
	uint8 * data_buf = TX_BUF;
   uint32 destip = 0;
   uint16 destport;
   

   switch (getSn_SR(s))
   {
   case SOCK_UDP:
      if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
      {
         if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */

         /* the data size to read is MAX_BUF_SIZE. */
         received_len = recvfrom(s, data_buf, RSR_len, (uint8*)&destip, &destport);       /* read the received data */
			printf("received_len: %d", received_len);
		 if(sendto(s, data_buf, received_len,(uint8*)&destip, destport) == 0) /* send the received data */
         {
            printf("\a\a\a%d : System Fatal Error.", s);
         }
      }
      break;
   case SOCK_CLOSED:                                               /* if a socket is closed */
      printf("\r\n%d : Loop-Back UDP Started. port :%d", s, port);
      if(socket(s,Sn_MR_UDP,port,0x00)== 0)    /* reinitialize the socket */
         printf("\a%d : Fail to create socket.",s);
      break;
   }
}


