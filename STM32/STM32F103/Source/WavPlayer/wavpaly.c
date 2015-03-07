/*
 * wavpaly.c
 *
 *  Created on: 2015年3月2日
 *      Author: star
 */
#include "version.h"
#include "stdio.h"
#include "dear.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"
#if Big-Endian
// 大端模式
const uint32_t DWWAV_RIFF = 0x52494646;
const uint32_t DWWAV_WAVE = 0x57415645;
const uint32_t DWWAV_FMT = 0x666D7420;
const uint32_t DWWAV_FACT = 0x66616374;
const uint32_t DWWAV_DATA = 0x64617461;

#else
// 小端模式 Windows ARM 默认模式
const uint32_t DWWAV_RIFF = 0x46464952;
const uint32_t DWWAV_WAVE = 0x45564157;
const uint32_t DWWAV_FMT = 0x20746D66;
const uint32_t DWWAV_FACT = 0x74636166;
const uint32_t DWWAV_DATA = 0x61746164;
#endif
//注意属性中的比特率是176kbps，而1CH中为22050Byte/s，换算一下就会发现22050*8/1024并不等于176，而是等于172，这里我想可能是通信中的1K并不等于1024而是等于1000的原因（通信原理书中好像有），如果按22050*8/1000这样算，就正好等于176了。其实比特率也可以这样算，总字节除以时长得到每秒字节率，再乘以8除以1000就得到比特率了，即(1325000/60)*8/1000=176kbps。
typedef struct {
	//	 RIFF_HEADER

	//uint8_t chRIFF[4];	//00H 	4byte 52 49 46 46   RIFF，资源交换文件标志。
	uint32_t chriff;
	uint32_t wavlen; //04H	4byte 从下一个地址开始到文件尾的总字节数。高位字节在后面，这里就是001437ECH，换成十进制是1325036byte，算上这之前的8byte就正好1325044byte了。
	//uint8_t chWAV[4];		//08H	4byte 57 41 56 45  	WAVE，代表wav文件格式。
	uint32_t chwav;
	//uint8_t chFMT[4];		//0CH	4byte 66 6D 74 20	FMT ，波形格式标志
	uint32_t chfmt;
	uint32_t PcmSize;//10H	4byte	00000010H，16PCM，这个对应定义中的PCMWAVEFORMAT部分的大小，可以看到后面的这个段内容正好是16个字节。当为16时，最后是没有附加信息的，当为数字18时，最后多了两个字节的附加信息
	//	PCMWAVEFORMAT
	//formart;
	uint16_t FormatTag;		//14H	2byte	为1时表示线性PCM编码，大于1时表示有压缩的编码。这里是0001H。
	//CHnum;
	uint16_t Channels;		//16H	2byte	1为单声道，2为双声道，这里是0001H。
	//SampleRate;
	uint32_t SamplesPerSec;	//18H	4byte	采样频率，这里是00002B11H，也就是11025Hz。
	//speed;				比特率=AvgBytesPerSec*8/1000
	uint32_t AvgBytesPerSec;//1CH	4byte	每秒所需字节数 | =采样频率SamplesPerSec*音频通道数Channels*每次采样得到的样本位数BitsPerSample/8，00005622H，也就是22050Byte/s=11025*1*16/2。
	//ajust;
	uint16_t BlockAlign;//20H	2byte	每个采样需要字节数 | 数据块块对齐单位=通道数*每次采样得到的样本位数/8，0002H，也就是2=1*16/8。
	//SampleBits;
	uint16_t BitsPerSample;	//22H	2byte	每个采样位数BitsPerSample，0010H即16，一个量化样本占2byte。

	//uint8_t chDATA[4];	//24H	4byte	data，一个标志而已。
	uint32_t chdata;
	uint32_t DATAlen;//28H	4byte	Wav文件实际音频数据所占的大小，这里是001437C8H即1325000，再加上2CH就正好是1325044，整个文件的大小。
} WAV_head_t;
typedef union {
	WAV_head_t wavhead;
	uint8_t wavbuf[0x2C];
} WAVE_info_u;
WAV_head_t wav1;
uint8_t CHanalnum;
uint8_t Bitnum;
uint8_t DACdone;
WAVE_info_u wavinfo;

uint8_t Check_Ifo(uint8_t* pbuf1, uint8_t* pbuf2) {
	uint8_t i;
	for (i = 0; i < 4; i++)
		if (pbuf1[i] != pbuf2[i])
			return 1;
	return 0;
}

uint8_t WAV_Init(uint8_t* pbuf) //初始化并显示文件信息	注: 仅适用于没有fact区的wav文件
{
	int i;
	uint8_t *pbuf0 = pbuf;
	//取得WAV文件头
	for (i = 0; i < 0x2C; i++)
		wavinfo.wavbuf[i] = *(pbuf0++);

	if (wavinfo.wavhead.chriff != DWWAV_RIFF) {
		printf("RIFF标志错误\n");
//		return 1;	//RIFF标志错误
	}
	//wav1.wavlen = Get_num(pbuf + 4, 4);		//文件长度，数据偏移4byte
	if (wavinfo.wavhead.chwav != DWWAV_WAVE) {
		printf("WAVE标志错误\n");
//		return 2;		//WAVE标志错误
	}
	if (wavinfo.wavhead.chfmt != DWWAV_FMT) {
		printf("FMT标志错误\n");
//		return 3;		//fmt标志错误
	}
	if (wavinfo.wavhead.chdata != DWWAV_DATA) {
		printf("data标志错误\n");
//		return 4;		//data标志错误
	}
	//offset 必须等于0x2c
	printf("offset:0x%x %d\n",
			wavinfo.wavhead.wavlen - wavinfo.wavhead.DATAlen + 8,
			wavinfo.wavhead.wavlen - wavinfo.wavhead.DATAlen + 8);

	printf("filesize: %d\n", wavinfo.wavhead.wavlen + 8);
	printf("wavlen:   %d\n", wavinfo.wavhead.wavlen);
	printf("DATAlen:  %d\n", wavinfo.wavhead.DATAlen);
	printf("声道Channels: %d\n", wavinfo.wavhead.Channels);
	printf("采样字节数 BlockAlign: %d\n", wavinfo.wavhead.BlockAlign);
	printf("采样位数BitsPerSample: %d\n", wavinfo.wavhead.BitsPerSample);
	printf("采样频率SamplesPerSec: %d\n", wavinfo.wavhead.SamplesPerSec);
	printf("比特率AvgBytesPerSec: %dkps\n",
			wavinfo.wavhead.AvgBytesPerSec * 8 / 1000);

	return 0;
}
typedef struct TS {
	uint8_t Period;
	uint8_t Prescaler;
} TIME_SET_t;
TIME_SET_t timeseting;

uint8_t volume = 100; //音量
uint8_t wav_buf[1024];
uint32_t DApc;
void wavplay() {
	uint32_t times, i,j;
	uint8_t * music = (uint8_t *)dear;
	WAV_Init(music);
//	配置分频系数
	switch (wavinfo.wavhead.SamplesPerSec) {	
		case 8000:
		timeseting.Period = 125;
		timeseting.Prescaler = 71;
		break;
		case 11025:
		timeseting.Period = 181;
		timeseting.Prescaler = 35;
		break;
		case 22050:
		timeseting.Period = 136;
		timeseting.Prescaler = 23;
		break;
	case 24000:
		timeseting.Period = 125;
		timeseting.Prescaler = 23;
		break;
	case 32000:
		timeseting.Period = 125;
		timeseting.Prescaler = 17;
		break;
	case 44100:
		timeseting.Period = 68;
		timeseting.Prescaler = 23;
		break;
	case 48000:
		timeseting.Period = 125;
		timeseting.Prescaler = 11;
		break;
	default:
		return;
	}

	TIM_SetAutoreload(TIM2, timeseting.Period);
	TIM_PrescalerConfig(TIM2, timeseting.Prescaler, TIM_PSCReloadMode_Update);
	
		music+=0x2c;
		for (j = 0; j < 512; j++) {
			wav_buf[j] = *(music++);
		}
	__set_PRIMASK(0);
	times = (wavinfo.wavhead.DATAlen >> 10) - 1;
//	music = (uint8_t *)dear;
	for (i = 0; i < times; i++) {
		
		while (!DACdone);
		DACdone = 0;
		for (; j < 1024; j++) {
			wav_buf[j] = *(music++);
		}
		while (!DACdone);
		DACdone = 0;
		for (j = 0; j < 512; j++) {
			wav_buf[j] = *(music++);
		}
	}
		DAC_SetChannel1Data(DAC_Align_8b_R, 0);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	__set_PRIMASK(1);
	printf("play end ");

}
uint8_t nu = 0;
void TIMx_IRQHandle() {
	
	uint16_t DACL_Value, DACR_Value;
	
	if (wavinfo.wavhead.BitsPerSample == 8) { //8位精度
		if (wavinfo.wavhead.Channels == 1) {	//单声道
			DACL_Value = ((uint16_t) wav_buf[DApc++]<<4 )* volume / 100 ;
			DACR_Value = DACL_Value;
		} else {	//立体声
			DACL_Value = ((uint16_t) wav_buf[DApc++] << 4) * volume / 100;
			DACR_Value = ((uint16_t) wav_buf[DApc++] << 4) * volume / 100;
		}

	} else if (wavinfo.wavhead.BitsPerSample == 16) {	//16位精度(先低位后高位)
		if (wavinfo.wavhead.Channels == 1) {	//单声道
			DACL_Value = (((uint8_t) (wav_buf[DApc + 1] - 0x80) << 4) | (wav_buf[DApc] >> 4))* volume / 100 ;
			DApc+=2;
			DACR_Value = DACL_Value;
		} else {	//立体声

			DACL_Value = (((u8) (wav_buf[DApc + 1] - 0x80) << 4)
					| (wav_buf[DApc] >> 4)) * 10 / volume;
			DApc+=2;
			DACR_Value = (((u8) (wav_buf[DApc + 1] - 0x80) << 4)
					| (wav_buf[DApc] >> 4)) * 10 / volume;
			DApc+=2;
		}
	}
	DAC_SetChannel1Data(DAC_Align_12b_R, DACL_Value);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	if (0) {
		DAC_SetChannel2Data(DAC_Align_12b_R, DACR_Value);
		DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
	}
//	实现双缓冲
	if (DApc == 512)
		DACdone = 1;
	if (DApc == 1024) {
		printf("%d \n",((nu++)*100)/((wavinfo.wavhead.DATAlen >> 10)));
		DApc = 0;
		DACdone = 1;
	}

}
