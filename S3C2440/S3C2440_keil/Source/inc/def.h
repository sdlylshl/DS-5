#ifndef __DEF_H__
#define __DEF_H__

#ifndef U8
#define U32 unsigned long
#define U16 unsigned short
#define U8  unsigned char
#endif

#ifndef I8
#define I32 signed long
#define I16 signed short
#define	I8  signed char
#endif

#ifndef S8
#define S32 long
#define S16 short int
#define	S8  char
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

#ifndef UINT8
#define UINT32 unsigned long
#define UINT16 unsigned short
#define UINT8  unsigned char
#endif

#ifndef uint8
#define uint32 unsigned long
#define uint16 unsigned short
#define uint8  unsigned char
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
#define	NULL	0//(void *)0
#endif
#ifndef null
#define	null	0
#endif
#define	SIZE_1K		0x00000400
#define	SIZE_2K		0x00000800
#define	SIZE_4K		0x00001000
#define	SIZE_8K		0x00002000
#define	SIZE_16K	0x00004000
#define	SIZE_32K	0x00008000
#define	SIZE_64K	0x00010000
#define	SIZE_128K	0x00020000
#define	SIZE_256K	0x00040000
#define	SIZE_512K	0x00080000
#define	SIZE_1M		0x00100000
#define	SIZE_2M		0x00200000
#define	SIZE_4M		0x00400000
#define	SIZE_8M		0x00800000
#define	SIZE_16M	0x01000000
#define	SIZE_32M	0x02000000
#define	SIZE_64M	0x04000000
#define	SIZE_128M	0x08000000
#define	SIZE_256M	0x10000000
#define	SIZE_512M	0x20000000
#define	SIZE_1G		0x40000000
#define	SIZE_2G		0x80000000

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

//5.按照LSB格式把两个字节转化为一个Word
#define FLIPW(ray)((((word)(ray)[0])*256)+(ray)[1])

//6.按照LSB格式 把一个字转换为两个字节
#define FLOPW(ray,val) (ray)[0]=((val)/256);(ray)[1]=((val)&0xFF)

//7.得到一个变量的地址(word 宽度)
#define B_PTR(var) ((byte *)(void *)&(var))
#define W_PTR(var) ((word *)(void *)&(var))

//8.得到一个字的高位和低位字节
#define WORD_LO(xxx) ((byte)((word)(xxx)&0xFF))
#define WORD_HI(xxx) ((byte)((word)(xxx)>>8))

//9.返回一个比X大的最接近的8的倍数
#define RND8(x) ((((x)+7)>>3)<<3)

//10.将一个字母转换为大写
#define UPCASE(c) (((c)>='a'&&(c)<='z')?((c)-0x20):(c))

//11.将一个字母转换为小写
#define LOWCASE(c) (((c)>='A'&&(c)<='Z')?((c)+0x20):(c))

//12.判断字符是不是十进制的数字
#define DECCHK(c) ((c)>='0'&&(c)<='9')

//13.判断字符是不是十六禁止的数字
#define HEXCHK(c) (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))

//14.防止溢出 
#define INC_SAT(val) (val=((val)+1>(val))?(val)+1:(val))

//15返回数组元素的个数 
#define ARR_NUM(a) (sizeof((a))/sizeof((a)[0]))

//16.返回一个无符号数n尾的值 X%(2^n)
#define MOD_BY_POWER_OF_TWO(val,modby) ((dword)(val)&(dword)((modby)-1))

//17.IO空间映射在存储空间的结构
#define INPUT(port) (*((volatile byte *)(port)))
#define INPUTW(port) (*((volatile word *)(port)))
#define INPUTDW(port) (*((volatile dword *)(port)))

#define OUTPUT(port,val) (*((volatile byte *)(port))=((byte)(val)))
#define OUTPUTW(port,val) (*((volatile word *)(port))=((word)(val)))
#define OUTPUTDW(port,val) (*((volatile dword *)(port))=((dword)(val)))

#endif /*__DEF_H__*/

