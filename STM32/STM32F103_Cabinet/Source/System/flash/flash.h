#ifndef __FLASH_H
#define __FLASH_H
#include "stdint.h"
#include "version.h"

//#define  STARTADDR  0x08010000  //STM32F103
#define		STARTADDR 0x0800C000
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
#define CONF_OFFSET 	0x0
#define BOX_MGR 		0x800  //2k  存放编号0 - 35
#define BOX_MGR_OFFSET_1 		(0x800 + BOX_MGR)  //2k 存放编号 36 - 71
#define BOX_MGR_OFFSET_2 		(0x800 + BOX_MGR_OFFSET_1) //2k 存放编号 72 - 107


#define IP_OFFSET 		0x0
#define GATE_OFFSET 	0x4
#define MASK_OFFSET 	0x8
#define DEST_OFFSET 	0xA
#define PORT_OFFSET 	0xc
#define ID_OFFSET		0x10
#define MAC_OFFSET 		0x14
#define ARM_OFFSET		0x18
#define LINK_IN_OFFSET		0x2A
#define LINK_OUT_OFFSET		0x2C
#define LINK_XOR_OFFSET		0x30

#define CELL_NUM1_OFFSET		0xea
#define CELL_NUM2_OFFSET		0xf0
#define CELL_NUM3_OFFSET		0xf8
#define CELL_NUM4_OFFSET		0xfc
#define CELL_NUM5_OFFSET		0x104

#define TEXT_NUM1_OFFSET		0x10A
#define TEXT_NUM2_OFFSET		0x110
#define TEXT_NUM3_OFFSET		0x118
#define TEXT_NUM4_OFFSET		0x11C
#define TEXT_NUM5_OFFSET		0x124


#define TEXT_CONTENT1_OFFSET		0x12a
#define TEXT_CONTENT2_OFFSET		0x1b6
#define TEXT_CONTENT3_OFFSET		0x242
#define TEXT_CONTENT4_OFFSET		0x2ce
#define TEXT_CONTENT5_OFFSET		0x35a




uint8_t get_ip_addr(uint32_t* ip);
int get_id(uint32_t* id);
uint8_t save_link_to_flash(uint8_t i, uint32_t in, uint16_t out, uint8_t xor);
uint8_t save_cell_num(uint8_t i, uint8_t* src);
uint8_t save_text_content(uint8_t i, uint8_t len, uint8_t* src);
uint8_t save_text_num(uint8_t i, uint8_t* src);
uint8_t save_arming_to_flash(uint32_t arm);

int save_ip_to_flash(uint8_t* addr, uint8_t sep);
int save_mac_to_flash(uint8_t* addr);
int save_port_to_flash(uint16_t port);
int save_id_to_flash(uint32_t id);


#endif
