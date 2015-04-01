#ifndef		__TYPE__H
#define		__TYPE__H

/*********************************************************************************/ 
// ���ö���

typedef unsigned char 		uint8;        		/* uchar  �������ֲ���޷���8λ�����ؼ��� 	*/
typedef signed   char   	int8;           	/* �������ֲ���з���8λ�����ؼ���        	*/
typedef unsigned short   	uint16;       		/* �������ֲ���޷���16λ�����ؼ���       	*/
typedef signed   short   	int16;        		/* �������ֲ���з���16λ�����ؼ���       	*/
typedef unsigned int 		uint32;     		/* �������ֲ���޷���32λ�����ؼ���       	*/
typedef signed   int 		int32;         	  	/* �������ֲ���з���32λ�����ؼ���       	*/
typedef uint32  			uint;				/* ����ָ��ת����, Add by Xsky 2011-07-29 11:25 */
typedef signed long long   	int64;     			/* �޷���64λ���ͱ���                       */
typedef unsigned long long	uint64;       		/* �޷���64λ���ͱ���                       */
typedef uint32 				t_ureg;				/* �޷��żĴ������� */
typedef int32				t_sreg;				/* �з��żĴ������� */
typedef uint16				t_ureg_opt;			/* �޷����Ż��Ĵ������� */
typedef int16				t_sreg_opt;			/* �з����Ż��Ĵ������� */

typedef t_ureg				t_bool;				/* boolean���� */
typedef t_ureg				t_err;				/* ���������� */
typedef uint32				t_size;				/* ���ݳ������� */
typedef uint16				t_size_opt;			/* �Ż����ݳ��� */

typedef uint16				bool;				/* ������ */
typedef uint16				BOOL;				/* ������ */

typedef void (*t_pFunc)();						/* ����ָ�� */
typedef void *(*t_pFuncR)();					/* ָ����ָ���ָ�� */
//typedef const void (*p_CnstFunc_t)();			/* ��������ָ�� */
//typedef const void * (*p_CnstFuncR_t)();		/* ����ָ����ָ���ָ�� */

//typedef unsigned char  u08 ; 
//typedef unsigned int   u16 ;
//typedef unsigned long  u32 ;

#define __flash const					

#define TCPU_REG_U		t_ureg					// CPU�Ĵ������ݳ���
#define TCPU_REG_S		t_sreg					// CPU�Ĵ���
#define TCPU_REG_SIZE	2


#define outb(a, b) (a=b)


#ifndef NONE
#define  NONE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef MIN 
#define MIN(a,b) ((a<b)?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) ((a>b)?(a):(b))
#endif
#ifndef ABS
#define ABS(x) ((x>)?(x):(-x))
#endif

#ifndef BIT
#define BIT(n)			(1<<(n))
#endif
#define LSHFT(x,n)		((x) << (n))
#define RSHFT(x,n)		((x) >> (n))
#define SETBIT(x,n) 	x |= (1<<(n))
#define CLRBIT(x,n) 	x &= (~(1<<(n)))
#define SETBITs(x,msk)	x|=(msk)
#define CLRBITs(x,msk)	x&=(~(msk))

#define CHKBIT(x,n) 	( ((x) & (1<<(n))) != 0 )
#define CHKNBIT(x,n)	( ((x)&(1<<(n)))==0 )
#define CHKBITs(x,msk)	( ((x)&(msk))==(msk) )
#define CHKNBITs(x,msk)	( ((x)&(msk))==(0) )

#define CPLBIT(x, n)		x ^= BIT(n)
#define CPLBITs(x, msk)		x ^= (msk)

#define lenof(x) 	( sizeof((x)) / sizeof(*(x)) )	// Add By Xsky 2008-9-03 15:56
#define MAXOF(x,y)	(((x)>(y))?(x):(y))
#define	MINOF(x,y)	(((x)<(y))?(x):(y))			/* Add by XSky 2010-09-11 10:48 */
#define STRLEN(str)	(sizeof(str)-1)

#define _def_(x)			defined(x)
#define BETWEEN(v,x,y) 		(((x)<(v)) && ((v)<(y)))
#define BETWEENINC(v,x,y) 	(((x)<=(v)) && ((v)<=(y)))
#define NOTBETWEENINC(val,l,h)  (( (val)<(l)) || ((h)<(val)) )	// Add By Xsky 2008-9-03 15:56
#define NOTIN(v,l,h)  		((v)<(l)) || ((h)<(v))
#define NOTINDEF(v,l,h,d)  	v=(((l)<(v)) && ((v)<(h)))?v:d

#define LONG_TO_BIN(n) \
(\
  ((n >> 21)& 0x80) | ((n >> 18)& 0x40) | \
  ((n >> 15)& 0x20) | ((n >> 12)& 0x10) | \
  ((n >> 9) & 0x08) | ((n >> 6) & 0x04) | \
  ((n >> 3) & 0x02) | ((n) & 0x01) \
)
#define BIN(n) LONG_TO_BIN(0x##n##l)

/* ѭ������ */
//#define RSHL(x,n,N)		( (((x)<<(n)) & ((BIT(N)-1)<<(n)) ) | ( ((x)>>((N)-(n))) & ~((BIT(N)-1)<<(n)) ) )
#define RSHL(x,n,N)		(((x)<<(n)) | ((x)>>((N)-(n))))
/* ѭ������ */
//#define RSHR(x,n,N)		( ( ((x)>>(n)) & ((BIT(N)-1)>>(n)) ) | ( ((x)<<((N)-(n))) & ~((BIT(N)-1)>>(n)) ) )
#define RSHR(x,n,N)		(((x)>>(n)) | ((x)<<((N)-(n))))

#define SWAP(x,y,tmp)	tmp = (x); \
						x = (y); \
						y = (tmp)

#define SQUARE(x)		((x)*(x))				// Add by Xsky 2008-09-06 10:44

#define SECOND_HOUR			3600			// һ��Сʱ������
#define SECOND_DAY			(3600*24)		// һ�������
#define SECOND_MONTH		(3600*24*30)	// һ���µ�����
#define SECOND_HALFMONTH	(3600*24*15)	// ����µ�����

#define PI_CONVERT 1.745329 //  0.017453292519943295769236907684886 = (180/PI)57.295779513082320876798154814105


// T: sturct name; x: struct member name 
#define  STRUCT_OFFSET(T,x)		((uint32)(&(((T *)0)->x)))
// T: sturct name; x: the front member name of Sturct; y: the behand member name of Struct
#define  STRUCT_BYTES(T,x,y)	( STRUCT_OFFSET(T,y) - STRUCT_OFFSET(T,x) )
// T: sturct name; x: struct member name
#define  STRUCT_SIZEOF(T,x)		(sizeof(((T *)0)->x))
// T: sturct name; x: struct member name
#define  STRUCT_LENOF(T,x)		(lenof(((T *)0)->x))

#define  STOP();  while(1){;}

#define  INC(x)	OS_ENTER_CRITICAL();x++;OS_EXIT_CRITICAL()
#define  DEC(x)	OS_ENTER_CRITICAL();x--;OS_EXIT_CRITICAL()
#define  IF0INC(x)	OS_ENTER_CRITICAL(); if(x==0){x++;}OS_EXIT_CRITICAL()
#define  IF1DEC(x)	OS_ENTER_CRITICAL(); if(x>0){x--;}OS_EXIT_CRITICAL()

#define  SETuint16PA(pDst, val)	*((uint8 *)(pDst)++) = ((uint8)(val));\
									*((uint8 *)(pDst)++) = ((uint8)((val)>>8))

#define  SETuint16P(pDst, val)		*((uint8 *)(pDst)) = ((uint8)(val));\
									*((uint8 *)(((uint8 *)(pDst))+1)) = ((uint8)((val)>>8))
#define	 GETuint16P(pSrc)		( ( (uint8)*((uint8 *)(pSrc)) ) | (( (uint8)*((uint8 *)((uint8 *)(pSrc)+1)) ) << 8) )
#define	 GETuint16PA(pSrc)		( ( (uint8)*((uint8 *)(pSrc)++) ) | (( (uint8)*((uint8 *)((uint8 *)(pSrc)++)) ) << 8) )

#define  SETPackedValPA(T, pDst, val)	*((__packed T *)(pDst)) = val; \
										pDst += sizeof(T);
#define  SETPackedVal(T, pDst, val)		*((__packed T *)(pDst)) = val;

#define	 EXEC_POINT_FUNC(pf, func, param)	pf = func; \
											if(pf != NULL){ \
												pf(param); \
											}

/*
#ifndef OS_ENTER_CRITICAL
#define OS_ENTER_CRITICAL()	
#endif
#ifndef OS_EXIT_CRITICAL
#define OS_EXIT_CRITICAL()	
#endif
*/

#endif		// end: #ifndef		__TYPE__H //
