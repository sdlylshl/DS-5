#ifndef __BIPBUFFER_H_
#define __BIPBUFFER_H_

typedef struct __bipbuffer_info {

	unsigned long size_a; //a�ռ��С
	unsigned long offset_a; //b�ռ�ƫ��

	unsigned long size_b; //b�ռ��С
	unsigned long offset_b; //a�ռ�ƫ��

	unsigned long buffer_len; //�������С
	unsigned long size_reserve; //������пռ��С
	unsigned long offset_reserve; //������пռ�ƫ��

	char *bipbuffer_addr; //buff��ַ

} BipBufferInfo_t;

int init_bipbuffer(unsigned long size);
int destroy_bipbuffer(void);

unsigned long write_data_to_bipbuffer(char *buff, unsigned long size);
unsigned long read_data_from_bipbuffer(char *buff, unsigned long size);

void printf_memory(void);
#endif
