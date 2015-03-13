
#include "flash.h"
#include "stdio.h"

//extern void Delay_us(__IO uint32_t us);
extern void Delay_ms(__IO uint32_t ms);


uint32_t conf_data[100];


int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum)
{
	int DataNum = 0;
	while (DataNum < ReadNum)
	{
		*(ReadBuf + DataNum) = *(volatile uint8_t*) ReadAddress++;
		DataNum++;
	}
	return DataNum;
}

void save_conf()
{
//	FLASH_UnlockBank1();
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//	FLASHStatus = FLASH_ErasePage(CONF_OFFSET);


//	if (FLASHStatus == FLASH_COMPLETE)
//	{
//		int i = 0;
//		conf_data[0] = conf_boxes_num_used;
//		for (; i < 100; i++)
//		{
//			FLASHStatus = FLASH_ProgramWord(CONF_OFFSET, conf_data[i]);    //flash.c 中API函数
//		}
//	}
//	FLASH_LockBank1();
}


void flash_save(void)
{  
	uint8_t Temp_Data[20] = {0};
	
	
	int i = 0;

//    WriteFlashOneWord(0x4,0x12355678);
	Delay_ms(10);
	ReadFlashNBtye(0x4, Temp_Data,4); 
	
             
	Delay_ms(10);

	for(i=0; i<4; i++)
		printf("Temp_Data[%d]: 0x%x\n", i, Temp_Data[i]);
	
}


