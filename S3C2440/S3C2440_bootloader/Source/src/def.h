#ifndef __DEF_H__
#define __DEF_H__

#include <stdint.h>
//#include <stddef.h>

//解决ds-5 编译出错
#define __irq

#define MC216 TRUE
//GPS一体机
//主要影响按键扫描
#define GPS TRUE
//ARM晶振选择 12MHz
//#define XTAL_12M FALSE
#define FIN 	(16000000)
//#define FIN 	(12000000)
//#define FIN		(16934400)
//软件版本
#define version  52

#ifndef U8
#define U32 unsigned long
#define U16 unsigned short
#define U8  unsigned char
#endif

#ifndef I8
#define I32 signed long
#define I16 signed short
#define I8  signed char
#endif

#ifndef S8
#define S32 long
#define S16 short int
#define S8  char
#endif

#ifndef BOOL
#define DWORD  unsigned long
#define WORD   unsigned short
#define BYTE   unsigned char
#define BOOL   unsigned char
#endif

#ifndef bool
#define dword  unsigned long
#define word   unsigned short
#define byte   unsigned char
#define bool   unsigned char
#endif


#ifndef TRUE
#define TRUE 	1
#define FALSE 	0

#define OK		1
#define FAIL	0

#define ON 		1
#define OFF 	0

#define YES 	1
#define NO 		0
#endif


#ifndef true
#define true 	1
#define false 	0

#define ok		1
#define fail	0

#define on 		1
#define off 	0

#define yes 	1
#define no 		0
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define BIT_0 0x01U
#define BIT_1 0x02U
#define BIT_2 0x04U
#define BIT_3 0x08U
#define BIT_4 0x10U
#define BIT_5 0x20U
#define BIT_6 0x40U
#define BIT_7 0x80U
#define BIT_8 0x0100U
#define BIT_9 0x0200U
#define BIT_10 0x0400U
#define BIT_11 0x0800U
#define BIT_12 0x1000U
#define BIT_13 0x2000U
#define BIT_14 0x4000U
#define BIT_15 0x8000U

#define	SIZE_1K		0x00000400U
#define	SIZE_2K		0x00000800U
#define	SIZE_4K		0x00001000U
#define	SIZE_8K		0x00002000U
#define	SIZE_16K	0x00004000U
#define	SIZE_32K	0x00008000U
#define	SIZE_64K	0x00010000U
#define	SIZE_128K	0x00020000U
#define	SIZE_256K	0x00040000U
#define	SIZE_512K	0x00080000U
#define	SIZE_1M		0x00100000U
#define	SIZE_2M		0x00200000U
#define	SIZE_4M		0x00400000U
#define	SIZE_8M		0x00800000U
#define	SIZE_16M	0x01000000U
#define	SIZE_32M	0x02000000U
#define	SIZE_64M	0x04000000U
#define	SIZE_128M	0x08000000U
#define	SIZE_256M	0x10000000U
#define	SIZE_512M	0x20000000U
#define	SIZE_1G		0x40000000U
#define	SIZE_2G		0x80000000U

#define	ENTER_KEY	0x0d
#define	BACK_KEY	0x08
#define BEEP_KEY	0x07
#define UP_KEY		0x41
#define DOWN_KEY	0x42
#define RIGHT_KEY	0x43
#define LEFT_KEY	0x44
#define HOME_KEY	0x48
#define END_KEY		0x4b
#define	ESC_KEY		0x1b

//1.得到指定地址上的一个字或字节
#define MEM_B(x) (*((unsigned char *)(x)))
#define MEM_W(x) (*((unsigned short *)(x)))

//2.求最大最小值
#define MAX(x,y) (((x)>(y))?(x):(y))
#define MIN(x,y) (((x)<(y))?(x):(y))

//3.得到一个field在结构体中的偏移
#define FPOS(type,field) ((unsigned int)&((type *)0)->field) //这里的&是取地址

//4.得到一个结构体中field所占用的字节数
#define FSIZE(type,field) sizeof(((type *)0)->field)

//5.按照LSB格式把两个字节uint8_t转化为一个uint16_t
#define FLIPW(ray)((((uint16_t)(ray)[0])*256)+(ray)[1])

//6.按照LSB格式 把一个uint16_t转换为两个字节uint8_t
#define FLOPW(ray,val) (ray)[0]=((val)/256);(ray)[1]=((val)&0xFF)

//7.得到一个变量的地址(宽度)
#define B_PTR(var) ((uint8_t *)(void *)&(var))
#define W_PTR(var) ((uint16_t *)(void *)&(var))

//8.得到一个字的高位和低位字节
#define WORD_LO(xxx) ((uint8_t)((uint16_t)(xxx)&0xFF))
#define WORD_HI(xxx) ((uint8_t)((uint16_t)(xxx)>>8))
/** The upper 8 bits of a 16 bit value */
#define MSB_16(a) (((a) & 0xFF00U) >> 8)
/** The lower 8 bits (of a 16 bit value) */
#define LSB_16(a) (((a) & 0xFFU))

//9.返回一个比X大的最接近的8的倍数
#define RND8(x) ((((x)+7)>>3)<<3)

//10.将一个字母转换为大写
#define UPCASE(c) (((c)>='a'&&(c)<='z')?((c)-0x20):(c))

//11.将一个字母转换为小写
#define LOWCASE(c) (((c)>='A'&&(c)<='Z')?((c)+0x20):(c))

//12.判断字符是不是十进制的数字
#define DECCHK(c) ((c)>='0'&&(c)<='9')

//13.判断字符是不是十六进制的数字
#define HEXCHK(c) (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))

//14.防止溢出
#define INC_SAT(val) (val=((val)+1>(val))?(val)+1:(val))

//15返回数组元素的个数
#define ARR_NUM(a) (sizeof((a))/sizeof((a)[0]))

//16.返回一个无符号数n尾的值 X%(2^n)
#define MOD_BY_POWER_OF_TWO(val,modby) ((uint32_t)(val)&(uint32_t)((modby)-1))

//17.IO空间映射在存储空间的结构
#define INPUT(port) (*((volatile uint8_t *)(port)))
#define INPUTW(port) (*((volatile uint16_t *)(port)))
#define INPUTDW(port) (*((volatile uint32_t *)(port)))

#define OUTPUT(port,val) (*((volatile uint8_t *)(port))=((uint8_t)(val)))
#define OUTPUTW(port,val) (*((volatile uint16_t *)(port))=((uint16_t)(val)))
#define OUTPUTDW(port,val) (*((volatile uint32_t *)(port))=((uint32_t)(val)))
//
#define SWAP_16(x) ((((x)&0xFFU)<<8)|(((x)>>8)&0xFFU))



#endif /*__DEF_H__*/

