/*-----------------------------------------------------------------------
��ʱ����
ϵͳʱ�ӣ�8M
-----------------------------------------------------------------------*/
extern void Delay_1us(void) ;                //1us��ʱ����

extern void Delay_Nus(unsigned int) ;     //N us��ʱ����
  
extern void Delay_1ms(void);                 //1ms��ʱ����

extern void Delay_Nms(unsigned int) ;      //N ms��ʱ����


#define OS_TICKS_PER_SEC	1000
#define OS_TimeDly(t)	Delay_Nms((t)<1?(1):(t))


