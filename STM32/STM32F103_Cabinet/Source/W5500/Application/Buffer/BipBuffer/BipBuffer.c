/*
 ============================================================================
 Name        : BipBuffer.c
 Author      : elines
 Version     :
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
 *������ƣ�init_bipbuffer_struct
 *�����ܣ���ʼ���ṹ
 *����˵����data_len ��ֵ
 *�� �� ֵ��void
 */
static void init_bipbuffer_struct(unsigned long buf_len) {
	//��ʼ���ṹ
	pBipBufferInfo->size_a = 0;
	pBipBufferInfo->size_b = 0;
	pBipBufferInfo->offset_a = 0;
	pBipBufferInfo->offset_b = 0;
	pBipBufferInfo->offset_reserve = 0;
	pBipBufferInfo->size_reserve = 0;
	pBipBufferInfo->buffer_len = buf_len;
}

/*
 *������ƣ�init_bipbuffer
 *�����ܣ������Ч����ʽѭ��������
 *����˵����data_len Ҫ���仺����Ĵ�С��һ������ҳΪ��λ
 *�� �� ֵ���ɹ�����0��ʧ�ܷ��ظ���
 */
extern BipBufferInfo_t bipBufferInfo;
extern char buffers[];
int init_bipbuffer(unsigned long buf_len) {
	//pBipBufferInfo = (BipBufferInfo_t *)malloc(sizeof(BipBufferInfo_t));
	pBipBufferInfo = &bipBufferInfo;
	if (pBipBufferInfo == NULL
	)
		return -1;

	//���仺�����С
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
 *������ƣ�destroy_bipbuffer
 *�����ܣ��ͷŻ�����
 *����˵����void
 *�� �� ֵ���ɹ�����0
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
 *������ƣ�get_space_after_a
 *�����ܣ�����ڴ��A����л�����Ĵ�С
 *����˵����void
 *�� �� ֵ�����л�����Ĵ�С
 */
static unsigned long get_space_after_a(void) {
	return pBipBufferInfo->buffer_len - pBipBufferInfo->offset_a
			- pBipBufferInfo->size_a;
}

/*
 *������ƣ�get_b_free_space
 *�����ܣ�����ڴ��B���л�����Ĵ�С
 *����˵����void
 *�� �� ֵ�����л�����Ĵ�С
 */
static unsigned long get_b_free_space(void) {
	return pBipBufferInfo->offset_a - pBipBufferInfo->offset_b
			- pBipBufferInfo->size_b;
}

/*
 *������ƣ�get_reserve_addr
 *�����ܣ���ÿ��л�����ĵ�ַ
 *����˵����want_size����Ҫ�Ĵ�С��actually_sizeʵ���ܸ���Ĵ�С
 *�� �� ֵ���п��пռ䷵�ص�ַ��û���򷵻�NULL
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

		//����Ŀռ������Ҫ�Ŀռ䣬˵���ж���Ŀռ�
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
		 ����ڴ��B�����ڣ������пռ��Ƿ���ڴ��A�󣬷�������
		 */
		free_space_size = get_space_after_a();
		printf("\nfree space:%ld", free_space_size);
		if (free_space_size >= pBipBufferInfo->offset_a) //�ڴ��A���и��Ŀռ�
				{
			if (free_space_size == 0)
				return NULL;

			//�����Ҫ����Ŀռ�С�ڿ��еĿռ�
			if (want_size < free_space_size)
				free_space_size = want_size;

			pBipBufferInfo->size_reserve = free_space_size;
			*actually_size = free_space_size;
			pBipBufferInfo->offset_reserve = pBipBufferInfo->offset_a
					+ pBipBufferInfo->size_a;
			return pBipBufferInfo->bipbuffer_addr
					+ pBipBufferInfo->offset_reserve;
		} else //���пռ䲻��
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
 *������ƣ�commit_data
 *�����ܣ����д����޸Ļ������ָ��
 *����˵����use_size����С
 *�� �� ֵ��void
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
 *������ƣ�get_contiguous_block
 *�����ܣ���ÿɶ������
 *����˵����data_len����С
 *�� �� ֵ�����ؿɶ����׵�ַ
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
 *������ƣ�commit_block
 *�����ܣ���ݶ������޸Ļ������ָ��
 *����˵����use_size����С
 *�� �� ֵ��void
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
 *������ƣ�get_committed_size
 *�����ܣ���ÿ�A�Ϳ�B���ܴ�С
 *����˵����void
 *�� �� ֵ����С
 */
unsigned long get_committed_size(void) {
	return pBipBufferInfo->size_a + pBipBufferInfo->size_b;
}

/*
 *������ƣ�get_reservation_size
 *�����ܣ���ô洢�Ĵ�С
 *����˵����void
 *�� �� ֵ����С
 */
unsigned long get_reservation_size(void) {
	return pBipBufferInfo->size_reserve;
}

/*
 *������ƣ�get_buffer_size
 *�����ܣ�������������Ĵ�С
 *����˵����void
 *�� �� ֵ����С
 */
unsigned long get_buffer_size(void) {
	return pBipBufferInfo->buffer_len;
}

/*
 *������ƣ�write_data_to_bipbuffer
 *������: �򻺳���д�����
 *����˵����buff:Ҫд�����ݣ�data_len:Ҫд��Ĵ�С
 *�� �� ֵ��ʵ��д��Ĵ�С
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
 *������ƣ�read_data_from_bipbuffer
 *������: �ӻ�����������
 *����˵����buff:Ҫ��������ݣ�data_len:ҪҪ�����Ĵ�С
 *�� �� ֵ��ʵ�ʶ����Ĵ�С
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
