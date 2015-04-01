/*-----------------------------------------------------------------------
延时函数
系统时钟：8M
-----------------------------------------------------------------------*/
extern void Delay_1us(void) ;                //1us延时函数

extern void Delay_Nus(unsigned int) ;     //N us延时函数
  
extern void Delay_1ms(void);                 //1ms延时函数

extern void Delay_Nms(unsigned int) ;      //N ms延时函数


#define OS_TICKS_PER_SEC	1000
#define OS_TimeDly(t)	Delay_Nms((t)<1?(1):(t))



