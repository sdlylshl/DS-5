#include "Types.h"
#include "hardware.h"

uint32_t presentTime;
uint32_t curTime = 0;
uint32_t lastTime = 0;
uint32_t keyTime = 0;
uint8_t flag_cur_key_card = 0;
uint8_t last_wigen_card_nr = 0;
uint8_t last_rs485_card_nr = 0;
uint8_t last_key_card_nr = 0;
uint8_t last_key_bind_induction_car_nr = 0;



 int main()
 {		
	 uint32_t id = 0,i = 0;
	 uint8_t i2c_buf[256];
	 uint8_t i2c_rd_buf[256];
	 uint8_t card_id[] = { 0x01, 0x02, 0x03, 0x04 };
	 uint8_t crc_buf[] = {0},cmdLen = 0;
	 uint32_t crc = 0;
	 uint8_t tmp[4] = { 0 };
	 hardware_init();
	 flash_init();
   
	 CRC_ResetDR();
	 for (i = 0; i < cmdLen + 6; i++)
	 {
			CRC_CalcCRC((uint32_t)crc_buf[i]);
	 }
	 crc = CRC_GetCRC();
	 printf("%d",crc);
//	 Bulk_clr();
	while (1)
	{
		continue;
		check_Induction_Car();  //检查地感
		key_scan();             //检查按键
		check_wigen_buf();      //解析wigen数据缓冲区，检查卡号
		decode_rs232_buf();     //解析吐卡机接口数据
		check_card_been_got_away_state();  //检查吐卡机卡嘴处的卡是否被取走
		bar_scan();             //抬杆

//		loopback_tcps(0, 8080);
		loopback_tcpc(SER_SOCKET, Default_Conf_Dat.Stu_Dat.dest_ip[SER_SOCKET], Default_Conf_Dat.Stu_Dat.dest_port[SER_SOCKET]);

		curTime = get_curTime();
		if(curTime - lastTime > 5)
		{
			LED1 = !LED1;
			lastTime = curTime;
		}		

		parse_card_signal();    //解析卡处理的内部信号数据
	}  
}

 /*
2D DD 93 1C
2D E8 28 CC
2D DD CB 5C
CA 14 61 13
2D DB E8 4C
2D E2 87 2C
2D DE 7D DC
0F CF E1 AC
0F 84 CC 9C
2D E9 5D 3C
2D DC 33 DC
2D E4 F5 9C
2D E1 5E 3C
2D E7 64 5C
2D D0 0D 1C
2D E4 69 CC
 
 
 
 
 
 */


