/*
 ============================================================================
 Name        : BipBuffer.c
 Author      : elines
 Version     :高效的两段式循环缓冲区
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>
#include "bipbuffer.h"

//struct __bipbuffer_info *pBipBufferInfo = NULL;

BipBufferInfo_t *pBipBufferInfo = NULL;
/*
 *函数名称：init_bipbuffer_struct
 *函数功能：初始化结构
 *参数说明：data_len 传值
 *返 回 值：void
 */
static void init_bipbuffer_struct(unsigned long buf_len) {
	//初始化结构
	pBipBufferInfo->size_a = 0;
	pBipBufferInfo->size_b = 0;
	pBipBufferInfo->offset_a = 0;
	pBipBufferInfo->offset_b = 0;
	pBipBufferInfo->offset_reserve = 0;
	pBipBufferInfo->size_reserve = 0;
	pBipBufferInfo->buffer_len = buf_len;
}

/*
 *函数名称：init_bipbuffer
 *函数功能：分配高效两段式循环缓冲区
 *参数说明：data_len 要分配缓冲区的大小，一般是以页为单位
 *返 回 值：成功返回0，失败返回负数
 */
extern BipBufferInfo_t bipBufferInfo;
extern char buffers[];
int init_bipbuffer(unsigned long buf_len) {
	//pBipBufferInfo = (BipBufferInfo_t *)malloc(sizeof(BipBufferInfo_t));
	pBipBufferInfo = &bipBufferInfo;
	if (pBipBufferInfo == NULL)
		return -1;

	//分配缓冲区大小
	//pBipBufferInfo->bipbuffer_addr = (char*)malloc(buf_len);
	pBipBufferInfo->bipbuffer_addr = (char*) buffers;

	if (pBipBufferInfo->bipbuffer_addr == NULL) {
		free(pBipBufferInfo);
		pBipBufferInfo = NULL;
		return -2;
	}

	init_bipbuffer_struct(buf_len);

	return 0;
}

/*
 *函数名称：destroy_bipbuffer
 *函数功能：释放缓冲区
 *参数说明：void
 *返 回 值：成功返回0
 */
int destroy_bipbuffer(void) {
	if (pBipBufferInfo->bipbuffer_addr) {
		free(pBipBufferInfo->bipbuffer_addr);
		pBipBufferInfo->bipbuffer_addr = NULL;
	}

	if (pBipBufferInfo) {
		free(pBipBufferInfo);
		pBipBufferInfo = NULL;
	}

	return 0;
}

/*
 *函数名称：get_space_after_a
 *函数功能：获得内存块A后空闲缓冲区的大小
 *参数说明：void
 *返 回 值：空闲缓冲区的大小
 */
static unsigned long get_space_after_a(void) {
	return pBipBufferInfo->buffer_len - pBipBufferInfo->offset_a
			- pBipBufferInfo->size_a;
}

/*
 *函数名称：get_b_free_space
 *函数功能：获得内存块B空闲缓冲区的大小
 *参数说明：void
 *返 回 值：空闲缓冲区的大小
 */
static unsigned long get_b_free_space(void) {
	return pBipBufferInfo->offset_a - pBipBufferInfo->offset_b
			- pBipBufferInfo->size_b;
}

/*
 *函数名称：get_reserve_addr
 *函数功能：获得空闲缓冲区的地址
 *参数说明：want_size：需要的大小，actually_size实际能给出的大小
 *返 回 值：有空闲空间返回地址，没有则返回NULL
 */
static char *get_reserve_addr(unsigned long want_size,
		unsigned long *actually_size) {
	unsigned long free_space_size;
	/*
	 We always allocate on B if B exists;
	 this means we have two blocks and our buffer is filling.
	 */
	if (pBipBufferInfo->size_b) {
		free_space_size = get_b_free_space();
		if (free_space_size == 0)
			return NULL;

		//多余的空间大于需要的空间，说明有多余的空间
		if (want_size < free_space_size)
			free_space_size = want_size;

		pBipBufferInfo->offset_reserve = free_space_size;
		*actually_size = free_space_size;
		pBipBufferInfo->offset_reserve = pBipBufferInfo->offset_b
				+ pBipBufferInfo->size_b;
		return pBipBufferInfo->bipbuffer_addr + pBipBufferInfo->offset_reserve;
	} else {
		/* Block b does not exist, so we can check
		 if the space after a is bigger than the space before A,
		 and allocate the bigger one.
		 */
		/*
		 如果内存块B不存在，检查空闲空间是否比内存块A大，分配最大的
		 */
		free_space_size = get_space_after_a();
		printf("\nfree space:%ld", free_space_size);
		if (free_space_size >= pBipBufferInfo->offset_a)    //内存块A后有更大的空间
				{
			if (free_space_size == 0)
				return NULL;

			//如果需要分配的空间小于空闲的空间
			if (want_size < free_space_size)
				free_space_size = want_size;

			pBipBufferInfo->size_reserve = free_space_size;
			*actually_size = free_space_size;
			pBipBufferInfo->offset_reserve = pBipBufferInfo->offset_a
					+ pBipBufferInfo->size_a;
			return pBipBufferInfo->bipbuffer_addr
					+ pBipBufferInfo->offset_reserve;
		} else    //空闲空间不够
		{
			if (pBipBufferInfo->offset_a == 0)
				return NULL;

			if (pBipBufferInfo->offset_a < want_size)
				want_size = pBipBufferInfo->offset_a;

			pBipBufferInfo->size_reserve = want_size;
			*actually_size = want_size;
			pBipBufferInfo->offset_reserve = 0;
			return pBipBufferInfo->bipbuffer_addr;
		}
	}

	//return NULL;
}

/*
 *函数名称：commit_data
 *函数功能：数据写入后，修改缓冲区的指针
 *参数说明：use_size：大小
 *返 回 值：void
 */
static void commit_data(unsigned long use_size) {
	if (use_size == 0) {
		// decommit any reservation
		pBipBufferInfo->size_reserve = 0;
		pBipBufferInfo->offset_reserve = 0;
		return;
	}

	/* If we try to commit more space than we asked for,
	 clip to the use_size we asked for.
	 */
	if (use_size > pBipBufferInfo->size_reserve)
		use_size = pBipBufferInfo->size_reserve;

	// If we have no blocks being used currently, we create one in A.
	if (pBipBufferInfo->size_a == 0 && pBipBufferInfo->size_b == 0) {
		pBipBufferInfo->offset_a = pBipBufferInfo->offset_reserve;
		pBipBufferInfo->size_a = use_size;

		pBipBufferInfo->offset_reserve = 0;
		pBipBufferInfo->size_reserve = 0;
		return;
	}

	if (pBipBufferInfo->offset_reserve
			== pBipBufferInfo->size_a + pBipBufferInfo->offset_a)
		pBipBufferInfo->size_a += use_size;
	else
		pBipBufferInfo->size_b += use_size;

	pBipBufferInfo->offset_reserve = 0;
	pBipBufferInfo->size_reserve = 0;
}

/*
 *函数名称：get_contiguous_block
 *函数功能：获得可读的数据
 *参数说明：data_len：大小
 *返 回 值：返回可读的首地址
 */
static char *get_contiguous_block(unsigned long *data_len) {
	if (pBipBufferInfo->size_a == 0) {
		*data_len = 0;
		return NULL;
	}

	*data_len = pBipBufferInfo->size_a;
	return pBipBufferInfo->bipbuffer_addr + pBipBufferInfo->offset_a;
}

/*
 *函数名称：commit_block
 *函数功能：数据读出后，修改缓冲区的指针
 *参数说明：use_size：大小
 *返 回 值：void
 */
static void commit_block(unsigned long use_size) {
	if (use_size >= pBipBufferInfo->size_a) {
		pBipBufferInfo->offset_a = pBipBufferInfo->offset_b;
		pBipBufferInfo->size_a = pBipBufferInfo->size_b;
		pBipBufferInfo->offset_a = 0;
		pBipBufferInfo->size_b = 0;
	} else {
		pBipBufferInfo->size_a -= use_size;
		pBipBufferInfo->offset_a += use_size;
	}
}

/*
 *函数名称：get_committed_size
 *函数功能：获得块A和块B的总大小
 *参数说明：void
 *返 回 值：大小
 */
unsigned long get_committed_size(void) {
	return pBipBufferInfo->size_a + pBipBufferInfo->size_b;
}

/*
 *函数名称：get_reservation_size
 *函数功能：获得存储的大小
 *参数说明：void
 *返 回 值：大小
 */
unsigned long get_reservation_size(void) {
	return pBipBufferInfo->size_reserve;
}

/*
 *函数名称：get_buffer_size
 *函数功能：获得整个缓冲区的大小
 *参数说明：void
 *返 回 值：大小
 */
unsigned long get_buffer_size(void) {
	return pBipBufferInfo->buffer_len;
}

/*
 *函数名称：write_data_to_bipbuffer
 *函数功能: 向缓冲区写入数据
 *参数说明：buff:要写入的数据，data_len:要写入的大小
 *返 回 值：实际写入的大小
 */
unsigned long write_data_to_bipbuffer(char *buff, unsigned long data_len) {
	char *data = NULL;
	unsigned long actually_size = 0;

	data = (char*) get_reserve_addr(data_len, &actually_size);
	if (data != NULL && actually_size > 0) {
		commit_data(actually_size);
		memcpy(data, buff, actually_size);
		return actually_size;
	}

	return 0;
}

/*
 *函数名称：read_data_from_bipbuffer
 *函数功能: 从缓冲区读出数据
 *参数说明：buff:要读出的数据，data_len:要要读出的大小
 *返 回 值：实际读出的大小
 */
unsigned long read_data_from_bipbuffer(char *buff, unsigned long data_len) {
	char *data = NULL;
	unsigned long actually_size = 0;

	data = (char *) get_contiguous_block(&actually_size);
	if (data != NULL && actually_size > 0) {
		if (actually_size <= data_len)
			data_len = actually_size;
		else
			//data_len = data_len;

			commit_block(data_len);
		memcpy(buff, data, data_len);
		memset(data, 0x00, data_len);
		return data_len;
	}

	return 0;
}

/***************************************************************
 *
 *for debug
 *2012-03-24
 ****************************************************************/
void printf_memory(void) {
	unsigned long i = 0;
	char *buff = (char*) pBipBufferInfo->bipbuffer_addr;
	printf("memory:\n");
	for (i = 0; i < pBipBufferInfo->buffer_len; i++)
		printf("%02X ", buff[i] & 0xFF);
	printf("\n---------------\n");
}
