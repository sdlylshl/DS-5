#include "stm32f10x.h"
#include "stdio.h"
#include "config.h"
#include "util.h"

#include "stm32f10x_flash.h"  
#include "stm32f10x_it.h"
#include "delay.h"
#include "Types.h"
//#include "boxManager.h"

#define FLASHPAGESIZE 0x800 // 2k
//#define  STARTADDR  0x08010000  //STM32F103
#define		STARTADDR 0x08070000
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE; 
#define CONF_OFFSET 	STARTADDR + 0x0

#define BOX_MGR_0_15 			(STARTADDR + 0x800)  //2k  存放编号0-15
#define BOX_MGR_16_31 		(0x800 + BOX_MGR_0_15)  //2k 存放编号 16-31
#define BOX_MGR_32_47 		(0x800 + BOX_MGR_16_31) //2k 存放编号 32-47
#define BOX_MGR_48_63 		(0x800 + BOX_MGR_32_47) //2k 存放编号 48-63
#define BOX_MGR_64_79 		(0x800 + BOX_MGR_48_63) //2k 存放编号 64-79 
#define BOX_MGR_80_95 		(0x800 + BOX_MGR_64_79) //2k 存放编号 80-95 	
#define BOX_MGR_96_107 		(0x800 + BOX_MGR_80_95) //2k 存放编号 96-107 	
//#define BOX_MGR_80_95 		(0x800 + BOX_MGR_96_107) //2k 存放编号 72 - 107

#define BOX_COMPANY_MSG_OFFSET  (0x800 + BOX_MGR_96_107)

uint16_t conf_arr_len = 1024;
UNI_CONF_DAT Conf_Dat;
UNI_CONF_DAT Default_Conf_Dat = {
	26,26,26,26,
	0,//配置是否为离线模式
	1,//第nr路读卡器被设置为吐卡机读卡器,1-4为wigen读卡器,5-7为rs485读卡器
	{
		{ 0x11, 0x08, 0xDC, 0x01, 0x15, 0x15 },//Mac[6]
		{ 192, 168, 88, 15 },//Lip[4]
		{ 255, 255, 255, 0 },//Sub[4]
		{ 192, 168, 88, 1 },//Gw[4]
		{ 202, 102, 152, 3 },//DNS_Server_IP[4]
		0,//DHCP
		8081 //port
	},
	{
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 },
		{ 192, 168, 88, 222 }
	},
	{ 8080, 8081, 8080, 8083, 8084, 8085, 8086, 8087 },
	7,//配置使用第nr路按键控制吐卡机读卡器
	//1,//配置使用第nr路地感与按键绑定
	{1,3,5,0,0,0,0}, //配置读卡器绑定的地感编号,1-4为wigen读卡器,5-7为rs485读卡器
	{1,2,3,4,1,2,3}    //配置读卡器绑定第nr路横杆
};


uint16_t  box_timeout_close_door;
uint16_t  box_timeout_get_express;
uint16_t  box_timeout_get_user;
uint16_t  box_timeout_get_temp;
uint16_t  box_timeout_express_mode;
uint16_t  box_timeout_input;
uint8_t   boxes_num_conf;

/***********************************************************************
使用stm32芯片的序列号的CRC32值 + 柜子编号 作为每个柜子的唯一ID号
************************************************************************/
uint8_t serial[12];
void GetSerialNum(uint8_t *Serial)
{
	uint16_t Device_Serial0, Device_Serial1;
	uint32_t Device_Serial2, Device_Serial3;

	Device_Serial0 = *(uint16_t*)(0x1FFFF7E8);
	Device_Serial1 = *(uint16_t*)(0x1FFFF7EA);
	Device_Serial2 = *(uint32_t*)(0x1FFFF7EC);
	Device_Serial3 = *(uint32_t*)(0x1FFFF7F0);

	Serial[0] = (uint8_t)((Device_Serial0 & 0x00FF));
	Serial[1] = (uint8_t)((Device_Serial0 & 0xFF00) >> 8);
	Serial[2] = (uint8_t)((Device_Serial1 & 0x00FF) >> 8);
	Serial[3] = (uint8_t)(Device_Serial1 & 0xFF00);

	Serial[4] = (uint8_t)((Device_Serial2 & 0x000000FF));
	Serial[5] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
	Serial[6] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
	Serial[7] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 16);

	Serial[8] = (uint8_t)((Device_Serial3 & 0x000000FF));
	Serial[9] = (uint8_t)((Device_Serial3 & 0x0000FF00) >> 8);
	Serial[10] = (uint8_t)((Device_Serial3 & 0x00FF0000) >> 16);
	Serial[11] = (uint8_t)((Device_Serial3 & 0xFF000000) >> 16);
}

int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum)
{
	int DataNum = 0;
	while (DataNum < ReadNum)
	{
		*(ReadBuf + DataNum) = *(__IO uint8_t*) ReadAddress++;
		DataNum++;
	}
	return DataNum;
}

void save_conf()
{
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_BSY |FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
	FLASHStatus = FLASH_ErasePage(CONF_OFFSET);


	if (FLASHStatus == FLASH_COMPLETE)
	{
		int i = 0;
		uint32_t writeAddress = CONF_OFFSET;
		for (; i < conf_arr_len; i++)
		{
			FLASH_ProgramHalfWord(writeAddress + i * 2, Conf_Dat.arr_dat[i]);
			//FLASHStatus = FLASH_ProgramWord(writeAddress + i * 4, Default_Conf_Dat.arr_dat[i]);    //flash.c 中API函数
		}
	}
	FLASH_LockBank1();
}

void save_default_conf()
{
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_ErasePage(CONF_OFFSET);


	if (FLASHStatus == FLASH_COMPLETE)
	{
		int i = 0;
		uint32_t writeAddress = CONF_OFFSET;
		for (; i < conf_arr_len; i++)
		{
			FLASH_ProgramHalfWord(writeAddress + i * 2, Default_Conf_Dat.arr_dat[i]);
			//FLASHStatus = FLASH_ProgramWord(writeAddress + i * 4, Default_Conf_Dat.arr_dat[i]);    //flash.c 中API函数
		}
	}
	FLASH_LockBank1();
}



void read_a_BoxMgr_Msg_from_flash(uint8_t boxNr)
{
//	uint32_t ReadAddress = BOX_MGR_0_15 + boxNr / 16 * 2048 + boxNr % 16 * 128;
//	uint8_t i = 0;

//	for (; i < boxManager_group_arr_len; i++)
//	{
//		boxMgr[boxNr].arrMsg[i] = *(__IO uint16_t*) ReadAddress;
//		ReadAddress += 2;
//	}
}

void write_a_boxMgr_Msg_no_erase(uint8_t boxNr)
{
//	FLASH_Status status ;
//	uint32_t WriteAddress = BOX_MGR_0_15 + boxNr / 16 * 2048 + boxNr % 16 * 128;
//	int j = 0; 
//	for (; j < boxManager_group_arr_len; j++)
//	{
//		status = FLASH_ProgramHalfWord(WriteAddress , boxMgr[boxNr].arrMsg[j]); 
//		if(status == FLASH_COMPLETE)
//		{
//			WriteAddress+= 2;
//		}
//		else
//		{
//			return;
//		}
//		
//	}
}
void write_a_boxMgr_Msg_to_flash(uint8_t boxNr)
{
	uint32_t WriteAddress = BOX_MGR_0_15 + boxNr / 16 * FLASHPAGESIZE;
	uint8_t  startNr = boxNr - boxNr % 16;
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_ErasePage(WriteAddress);

	if (FLASHStatus == FLASH_COMPLETE)
	{
		int i = startNr;
		for (; i < startNr + 16; i++)
		{
			write_a_boxMgr_Msg_no_erase(i);
		}
	}
	FLASH_LockBank1();
}

void read_all_BoxMgrs_Msg_from_flash()
{
	int i = 0;
	for (; i < boxes_num_conf; i++)
	{
		read_a_BoxMgr_Msg_from_flash(i);
	}
}

void read_conf()
{
	uint16_t * ReadAddress = (uint16_t *)CONF_OFFSET;
	int i = 0;
	for (; i < conf_arr_len; i++)
	{
		Conf_Dat.arr_dat[i] = *(__IO uint16_t*) ReadAddress++;
	}
}

void decode_conf_msg()
{
//	int i = 0;
//	for (; i < boxes_num_conf; i++)
//	{
//		decode_boxMgr_msg(&boxMgr[i].stuMsg);
//	}
}

void write_a_user_msg_no_erase(uint8_t nr)
{
//	FLASH_Status status ;
//	uint32_t WriteAddress = BOX_COMPANY_MSG_OFFSET + nr / 16 * FLASHPAGESIZE;
//	int j = 0; 
//	for (; j < MAXUSERMGRARRLEN; j++)
//	{
//		status = FLASH_ProgramHalfWord(WriteAddress , userMgr[nr].arrMsg[j]); 
//		if(status == FLASH_COMPLETE)
//		{
//			WriteAddress+= 2;
//		}
//		else
//		{
//			return;
//		}
//		
//	}

}

void save_user_msg_to_flash(uint8_t nr)
{
	uint32_t WriteAddress = BOX_COMPANY_MSG_OFFSET + nr / 16 * FLASHPAGESIZE;
	uint8_t startNr = nr - nr % 16;
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_ErasePage(WriteAddress);

	if (FLASHStatus == FLASH_COMPLETE)
	{
		int i = startNr;
		for(;i < startNr + 16;i++)
		{
			write_a_user_msg_no_erase(i);
		}
	}
	FLASH_LockBank1();

}

void read_user_msg_from_flash()
{
//	uint32_t ReadAddress = BOX_COMPANY_MSG_OFFSET ;
//	uint8_t i = 0;
//	uint8_t j = 0;

//	for (; i < MAXUSERCOMPANYNUM; i++)
//	{
//		for(;j < MAXUSERMGRARRLEN;j++)
//		{
//			userMgr[i].arrMsg[j] = *(__IO uint16_t*) ReadAddress;
//			ReadAddress += 2;
//			
//		}
//	}

}

void flash_init()
{
	
	conf_arr_len = (sizeof(Conf_Dat.Stu_Dat) + 1) / 2;
//	boxManager_group_arr_len = sizeof(boxMgr[0].stuMsg)/2;
	save_default_conf();
////	delay_ms(500);
//	
	read_conf();

//	boxes_num_conf = Conf_Dat.Stu_Dat.boxes_num_conf;
//	box_timeout_close_door = (Conf_Dat.Stu_Dat.box_timeout_close_door[0] << 8) * 60 + Conf_Dat.Stu_Dat.box_timeout_close_door[1];
//	box_timeout_get_express = (Conf_Dat.Stu_Dat.box_timeout_get_express[0] << 8) * 60 + Conf_Dat.Stu_Dat.box_timeout_get_express[1];
//	box_timeout_get_user = (Conf_Dat.Stu_Dat.box_timeout_get_user[0] << 8) * 60 + Conf_Dat.Stu_Dat.box_timeout_get_user[1];
//	box_timeout_get_temp = (Conf_Dat.Stu_Dat.box_timeout_get_temp[0] << 8) * 60 + Conf_Dat.Stu_Dat.box_timeout_get_temp[1];
//	box_timeout_express_mode = Conf_Dat.Stu_Dat.box_timeout_express_mode * 60;
//	box_timeout_input = Conf_Dat.Stu_Dat.box_timeout_input;
//	if (box_timeout_input < 20)
//		box_timeout_input = 20;
//	

//	read_all_BoxMgrs_Msg_from_flash();
//	decode_conf_msg();
//	boxes_init();

//	read_user_msg_from_flash();
//	Conf_Dat = Default_Conf_Dat;
}




