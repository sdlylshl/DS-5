#include "hardware.h"

Boolean flag_get_all_frame = FALSE;

Boolean flag_cur_frame_used = FALSE;
uint8_t data_used_cnt = 0;
uint8_t tmp_buf[256] = {0};
Boolean flag_get_data = FALSE;
uint8_t pt2262_buf[256];
uint8_t pt2262_cnt = 0;
Boolean flag_delay_us_700 = FALSE;
Boolean flag_start_delay = FALSE;
uint8_t exit_dat = 0;
uint8_t channel_addr[32][8];


Boolean check_addr_exit(uint8_t addr[])
{
	int i = 0;
	for(;i < 32;i++)
	{
			if( memcmp(channel_addr[i],addr,8) == 0)
			{
				return TRUE;
			}
	}
	return FALSE;
}

void add_a_machine(uint8_t addr[])
{
	int i = 0;
	for(;i < 32;i++)
	{
		uint8_t addr_0[8] = {0};
		if(memcmp(channel_addr[i],addr_0,8) == 0)
		{
			memcpy(channel_addr[i],addr,8);
			break;
		}

	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		flag_start_delay = TRUE;
		flag_get_data = TRUE;
		EXTI_ClearITPendingBit(EXTI_Line14);   //清除 LINE 上的中断标志位
		
	}

}

Boolean check_study_mode()
{
	return FALSE;//get_Induction_Car(1);
}

uint8_t study_addr_dat[8];
uint8_t frame_dat[12];
uint8_t study_addr_cnt = 0;

void decode_pt2262_buf()
{
	if(flag_get_all_frame)
	{
		if ( pt2262_cnt == 0x19 && !flag_cur_frame_used )
		{
			uint8_t i = 0,dat = 0x0d;
			//UART1_Put_Char(0xee);
			for ( i = 0; i < 12; i++ )
			{
				dat = 0x0d;
				if(tmp_buf[i * 2] == 0 && tmp_buf[i * 2 + 1] == 0)
					dat = 0;
				else if(tmp_buf[i * 2] == 1 && tmp_buf[i * 2 + 1] == 1)
					dat = 1;
				else if(tmp_buf[i * 2] == 0 && tmp_buf[i * 2 + 1] == 1)
					dat = 0x0f;
				frame_dat[i] = dat;
			//	UART1_Put_Char(dat);
			}

			if(check_study_mode())
			{
				if(memcmp(frame_dat,study_addr_dat,8) == 0)
				{
					if(study_addr_cnt < 10)
						study_addr_cnt++;
				}
				else
				{
					memcpy( study_addr_dat ,frame_dat , 8);
					study_addr_cnt = 1;
				}

				if(study_addr_cnt == 10)
				{
					if ( !check_addr_exit (study_addr_dat))
					{
						//uint8_t addr_0[8] = {0};
						add_a_machine(study_addr_dat);
						
						//study_addr_cnt = 0;
						UART1_Put_Char(0xdd);
						UART1_Put_Char(0xdd); 
				
						for(i = 0;i< 8;i++)
						{
							UART1_Put_Char(study_addr_dat[i]);
						}
						UART1_Put_Char(0xdd);
						UART1_Put_Char(0xdd);

						//memcpy(study_addr_dat,addr_0,8);
					}
					else
					{
						UART1_Put_Char(0xaa);
						UART1_Put_Char(0xaa);
						for ( i = 0; i< 8; i++ ) {
							UART1_Put_Char(study_addr_dat[i]);
						}
						UART1_Put_Char(0xaa);
						UART1_Put_Char(0xaa);
					}


				
				}
				
			}
			else
			{
				UART1_Put_Char(0xee);
				for ( i = 0; i< 12; i++ ) {
					UART1_Put_Char(frame_dat[i]);
				}
				UART1_Put_Char(0xee);
			}
			
		}
		pt2262_cnt = 0;
		data_used_cnt = 0;
		flag_get_all_frame = FALSE;
		flag_cur_frame_used = TRUE;
	}
}

uint32_t frame_tm = 0;
Boolean flag_light_on = TRUE;
uint16_t light_cnt = 0;
uint8_t short_tm = 1;
uint8_t long_tm = 3;
uint16_t tm_delay_50 = 0;
uint16_t time_count = 0;
uint16_t tm_max = 0;
Boolean flag_on = TRUE;
uint8_t dat_cnt = 0;
uint16_t tm_delay_700 = 0;
uint8_t frame_times = 1;
uint32_t frame_inteval_time = 150;// 60 * 1000 * 3 1min
uint32_t signal_inteval_time = 9000;
uint8_t dat_addr[16] = { 0 , 1 , 0 , 0 , 0 , 0 , 0x01 , 1 , 0 , 1 , 1 , 0 , 1 , 0 , 0x01 , 0x01 };
uint8_t dat_tm_alarm[9] = { 0 , 1 , 0 , 0 , 0 , 1 , 0 , 0x01,0 };// 0,0,f,0


void frame_init()
{
	time_count = 1;
	dat_cnt = 0;
	tm_max = 0;
	flag_on = ( Boolean ) 0;
	tm_delay_50 = 0;
}

void send_dat()
{
	if ( dat_cnt < 25 ) {
		if ( time_count++ >= tm_max ) {
			if ( flag_on == 1 ) {
				tm_max = tm_max == long_tm ? short_tm : long_tm;
				flag_on = ( Boolean ) 0;
			}
			else {
				if ( dat_cnt < 16 )
					tm_max = dat_addr[dat_cnt] == 1 ? long_tm : short_tm;
				else {
					tm_max = dat_tm_alarm[dat_cnt - 16] == 1 ? long_tm : short_tm;
					//putchar(p_dat[dat_cnt - 16]);
				}
				flag_on = ( Boolean ) 1;
			}
			time_count = 1;
		}
		if ( flag_on ) {
			//set_pt2262_high();
			//Beep =1;
		}
		else {
			//Beep = 0;
			if ( time_count == tm_max )
				dat_cnt++;
		}

	}
	else {
		if ( tm_delay_50++ >= frame_inteval_time ) {
			frame_init();
			if ( frame_times++ <= 3 ) {

			}
			else {
				frame_tm = 0;
				frame_times = 1;
				//putchar(0xBB);
			}
		}
		//Beep = 0;

	}

}
