#ifndef __BIPBUFFER_H_
#define __BIPBUFFER_H_

typedef struct __bipbuffer_info{

    unsigned long size_a;       //a空间大小
    unsigned long offset_a;      //b空间偏移

    unsigned long size_b;       //b空间大小
    unsigned long offset_b;      //a空间偏移

    unsigned long buffer_len;   //缓冲区大小
    unsigned long size_reserve; //储存空闲空间大小
    unsigned long offset_reserve;//储存空闲空间偏移

    char *bipbuffer_addr;		//buff地址

}BipBufferInfo_t;

int init_bipbuffer(unsigned long size);
int destroy_bipbuffer(void);

unsigned long write_data_to_bipbuffer(char *buff, unsigned long size);
unsigned long read_data_from_bipbuffer(char *buff, unsigned long size);

void printf_memory(void);
#endif
