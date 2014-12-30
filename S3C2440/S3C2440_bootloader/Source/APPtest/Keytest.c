
#include "def.h"
#include "Delay.h"
#include "2440addr.h"
#include "LCD_Driver.h"
#include "stdio.h"

#include "Keyboard.h"
//*************************************
//#include "ver.h"
extern void Versions(void);
//*************************************

extern void Keytest(void)
{
	U16 keybk=1;
	U16 key1,key2;
	 Printf("DiagNose MODE\n");
  Printf("\nkeyTest\n");
  Lcd_Update();
  while(keybk){
	  key1=KeyScan();
	  if(key1==0)
	  		key2 =0;
	  		
	  if(key1!=key2&&key1!=0){
	 	key2=key1;
	 	Printf("You Press Key num is:%x\n",key1);
	 	
	 	}
	  if(key2 == KEY_BACK){
	  //按键松开
	  while(KeyScan());
	  Printf("Press Back Key again return\n");
	  Lcd_Update();
	  //DelayMS(500);
	  //按键按下
	  while(!(key1=KeyScan()));
	  if(key1==KEY_BACK)
	 	  	keybk=0;
	   Printf("KeyTest is Finished \n");
	  }
	  Lcd_Update();
			
  }

  
}

/*
extern void Keytest(void)
{
	U32 i=0;
	U32 j=0;
	U16 key=0;
	U16 key1,key2;
	char tmp[20];
	
		Versions();
		print(0,2,"Starting keyTest",0xFF);
		//DrawSquare(96*0+20,96*5-20,64*9+10,64*10-10,1,0xFFFF00);//0
		DrawSquare(96*0+20,96*1-20,64*9+10,64*10-10,1,0x555555);//6
		DrawSquare(96*1+20,96*2-20,64*9+10,64*10-10,1,0x555555);//7
		DrawSquare(96*2+20,96*3-20,64*9+10,64*10-10,1,0x555555);//8
		DrawSquare(96*3+20,96*4-20,64*9+10,64*10-10,1,0x555555);//9
		DrawSquare(96*4+20,96*5-20,64*9+10,64*10-10,1,0x555555);//0
		
		DrawSquare(96*0+20,96*1-20,64*8+10,64*9-10,1,0x555555);//1
		DrawSquare(96*1+20,96*2-20,64*8+10,64*9-10,1,0x555555);//2
		DrawSquare(96*2+20,96*3-20,64*8+10,64*9-10,1,0x555555);//3
		DrawSquare(96*3+20,96*4-20,64*8+10,64*9-10,1,0x555555);//4
		DrawSquare(96*4+20,96*5-20,64*8+10,64*9-10,1,0x555555);//5
		
		DrawSquare(96*4+20,480-20,64*0+10,64*1-10,1,0x555555);//H
		DrawSquare(96*4+20,480-20,64*1+10,64*2-10,1,0x555555);//M
		DrawSquare(96*4+20,480-20,64*2+10,64*3-10,1,0x555555);//L
		DrawSquare(96*4+20,480-20,64*3+10,64*4-10,1,0x555555);//F
		
		DrawSquare(96*4+20,480-20,64*4+10,64*5-10,1,0x555555);//S
		DrawSquare(96*4+20,480-20,64*5+10,64*6-10,1,0x555555);//X
		DrawSquare(96*4+20,480-20,64*6+10,64*7-10,1,0x555555);//Z
		DrawSquare(96*4+20,480-20,64*7+10,64*8-10,1,0x555555);//Y
	//	print(0,10,"PRESS ENTER EXIT!",0xFF);
	
		Lcd_Update();
		key1=1;
		i=0;
	while(key1)	
	{	
		
		key2=key;
		key=KeyScan();
		if((i&0x3FFFF)==0x3FFFF)
			{
				print(0,10,"Test is ok !",0xFF);
				print(0,12,"PRESS ENTER EXIT!",0xFF);
			}
		if(key2!=key){
			switch(key){
			case KEY_NUM0 :
			i|=1<<0;
			switch((j&(1<<0)))
			{	
				case 0:j|=(1<<0);
				break;
				default:
				j&=~(1<<0);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"0 is press!",0xFF);
			//print(40+96*4,64*9+8,"0",0xFF);
			if((j&(1<<0)))	
			DrawSquare(96*4+20,96*5-20,64*9+10,64*10-10,1,0xFF);//0	
			else
			DrawSquare(96*4+20,96*5-20,64*9+10,64*10-10,1,0xff0000);//0
			print(36+96*4,64*9+26,"0",0xffffff);	
			
			break;
			
			case KEY_NUM1 :
			i|=1<<1;
			switch((j&(1<<1)))
			{	
				case 0:j|=(1<<1);
				break;
				default:
				j&=~(1<<1);
				break;
			}
			
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"1 is press!",0xFF);
			if((j&(1<<1)))	
			DrawSquare(96*0+20,96*1-20,64*8+10,64*9-10,1,0xff);//1
			else
			DrawSquare(96*0+20,96*1-20,64*8+10,64*9-10,1,0xff0000);//1
			print(36+96*0,64*8+26,"1",0xffffff);	
			break;
						
			case KEY_NUM2 :
			i|=1<<2;
			switch((j&(1<<2)))
			{	
				case 0:j|=(1<<2);
				break;
				default:
				j&=~(1<<2);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"2 is press!",0xFF);
			if((j&(1<<2)))	
			DrawSquare(96*1+20,96*2-20,64*8+10,64*9-10,1,0xff);//2
			else
			DrawSquare(96*1+20,96*2-20,64*8+10,64*9-10,1,0xff0000);//2
			print(36+96*1,64*8+26,"2",0xffffff);	
			break;
			
			case KEY_NUM3 :
			i|=1<<3;
			switch((j&(1<<3)))
			{	
				case 0:j|=(1<<3);
				break;
				default:
				j&=~(1<<3);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"3 is press!",0xFF);
			if((j&(1<<3)))	
			DrawSquare(96*2+20,96*3-20,64*8+10,64*9-10,1,0xff);//3
			else
			DrawSquare(96*2+20,96*3-20,64*8+10,64*9-10,1,0xff0000);//3
			print(36+96*2,64*8+26,"3",0xffffff);	
			break;
			case KEY_NUM4 :
			i|=1<<4;
			switch((j&(1<<4)))
			{	
				case 0:j|=(1<<4);
				break;
				default:
				j&=~(1<<4);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"4 is press!",0xFF);
			if((j&(1<<4)))	
			DrawSquare(96*3+20,96*4-20,64*8+10,64*9-10,1,0xff);//4
			else
			DrawSquare(96*3+20,96*4-20,64*8+10,64*9-10,1,0xff0000);//4
			print(36+96*3,64*8+26,"4",0xffffff);	
			break;
			
			case KEY_NUM5 :
			i|=1<<5;
			switch((j&(1<<5)))
			{	
				case 0:j|=(1<<5);
				break;
				default:
				j&=~(1<<5);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"5 is press!",0xFF);
			if((j&(1<<5)))	
			DrawSquare(96*4+20,96*5-20,64*8+10,64*9-10,1,0xff);//5
			else
			DrawSquare(96*4+20,96*5-20,64*8+10,64*9-10,1,0xff0000);//5
			print(36+96*4,64*8+26,"5",0xffffff);	
			break;
						
			case KEY_NUM6 :
			i|=1<<6;
			switch((j&(1<<6)))
			{	
				case 0:j|=(1<<6);
				break;
				default:
				j&=~(1<<6);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"6 is press!",0xFF);
			if((j&(1<<6)))	
			DrawSquare(96*0+20,96*1-20,64*9+10,64*10-10,1,0xff);//6
			else
			DrawSquare(96*0+20,96*1-20,64*9+10,64*10-10,1,0xff0000);//6
			print(36+96*0,64*9+26,"6",0xffffff);	
			break;
			
			case KEY_NUM7 :
			i|=1<<7;
			switch((j&(1<<7)))
			{	
				case 0:j|=(1<<7);
				break;
				default:
				j&=~(1<<7);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"7 is press!",0xFF);
			if((j&(1<<7)))	
			DrawSquare(96*1+20,96*2-20,64*9+10,64*10-10,1,0xff);//7
			else
			DrawSquare(96*1+20,96*2-20,64*9+10,64*10-10,1,0xff0000);//7
			print(36+96*1,64*9+26,"7",0xffffff);	
			break;			
			case KEY_NUM8 :
			i|=1<<8;
			switch((j&(1<<8)))
			{	
				case 0:j|=(1<<8);
				break;
				default:
				j&=~(1<<8);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"8 is press!",0xFF);
			if((j&(1<<8)))	
			DrawSquare(96*2+20,96*3-20,64*9+10,64*10-10,1,0xff);//8
			else
			DrawSquare(96*2+20,96*3-20,64*9+10,64*10-10,1,0xff0000);//8
			print(36+96*2,64*9+26,"8",0xffffff);	
			break;
			
			case KEY_NUM9 :
			i|=1<<9;
			switch((j&(1<<9)))
			{	
				case 0:j|=(1<<9);
				break;
				default:
				j&=~(1<<9);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"9 is press!",0xFF);
			if((j&(1<<9)))	
			DrawSquare(96*3+20,96*4-20,64*9+10,64*10-10,1,0xff);//9
			else
			DrawSquare(96*3+20,96*4-20,64*9+10,64*10-10,1,0xff0000);//9
			print(36+96*3,64*9+26,"9",0xffffff);	
			break;
						
			case KEY_H :
			i|=1<<10;
			switch((j&(1<<10)))
			{	
				case 0:j|=(1<<10);
				break;
				default:
				j&=~(1<<10);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"H is press!",0xFF);
			if((j&(1<<10)))	
			DrawSquare(96*4+20,480-20,64*0+10,64*1-10,1,0xFF);//H
			else
			DrawSquare(96*4+20,480-20,64*0+10,64*1-10,1,0xFF0000);//H
			print(36+96*4,64*0+26,"H",0xffffff);	
			break;
			
			case KEY_M :
			i|=1<<11;
			switch((j&(1<<11)))
			{	
				case 0:j|=(1<<11);
				break;
				default:
				j&=~(1<<11);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"M is press!",0xFF);
			if((j&(1<<11)))	
			DrawSquare(96*4+20,480-20,64*1+10,64*2-10,1,0xFF);//M
			else
			DrawSquare(96*4+20,480-20,64*1+10,64*2-10,1,0xFF0000);//M
			print(36+96*4,64*1+26,"M",0xffffff);	
			break;
			case KEY_L :
			i|=1<<12;
			switch((j&(1<<12)))
			{	
				case 0:j|=(1<<12);
				break;
				default:
				j&=~(1<<12);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"L is press!",0xFF);
			if((j&(1<<12)))	
			DrawSquare(96*4+20,480-20,64*2+10,64*3-10,1,0xFF);//L
			else
			DrawSquare(96*4+20,480-20,64*2+10,64*3-10,1,0xFF0000);//L
			print(36+96*4,64*2+26,"L",0xffffff);	
			break;
			
			case KEY_F :
			i|=1<<13;
			switch((j&(1<<13)))
			{	
				case 0:j|=(1<<13);
				break;
				default:
				j&=~(1<<13);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"F is press!",0xFF);
			if((j&(1<<13)))	
			DrawSquare(96*4+20,480-20,64*3+10,64*4-10,1,0xFF);//F
			else
			DrawSquare(96*4+20,480-20,64*3+10,64*4-10,1,0xFF0000);//F
			print(36+96*4,64*3+26,"F",0xffffff);	
			break;
						
			case KEY_UP :
			i|=1<<14;
			switch((j&(1<<14)))
			{	
				case 0:j|=(1<<14);
				break;
				default:
				j&=~(1<<14);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"UP is press!",0xFF);
			if((j&(1<<14)))	
			DrawSquare(96*4+20,480-20,64*4+10,64*5-10,1,0xFF);//S
			else
			DrawSquare(96*4+20,480-20,64*4+10,64*5-10,1,0xFF0000);//S
			print(36+96*4,64*4+26,"U",0xffffff);	
			break;
			
			case KEY_DOWN :
			i|=1<<15;
			switch((j&(1<<15)))
			{	
				case 0:j|=(1<<15);
				break;
				default:
				j&=~(1<<15);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"Down is press!",0xFF);
			if((j&(1<<15)))	
			DrawSquare(96*4+20,480-20,64*5+10,64*6-10,1,0xFF);//X
			else
			DrawSquare(96*4+20,480-20,64*5+10,64*6-10,1,0xFF0000);//X
			print(36+96*4,64*5+26,"D",0xffffff);	
			break;
			
			case KEY_BACK :
			i|=1<<16;
			switch((j&(1<<16)))
			{	
				case 0:j|=(1<<16);
				break;
				default:
				j&=~(1<<16);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"Back is press!",0xFF);
			if((j&(1<<16)))	
			DrawSquare(96*4+20,480-20,64*6+10,64*7-10,1,0xFF);//Z
			else
			DrawSquare(96*4+20,480-20,64*6+10,64*7-10,1,0xFF0000);//Z
			print(36+96*4,64*6+26,"B",0xffffff);	
			break;
			
			case KEY_NEXT :
			i|=1<<17;
			switch((j&(1<<17)))
			{	
				case 0:j|=(1<<17);
				break;
				default:
				j&=~(1<<17);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"Next is press!",0xFF);
			if((j&(1<<17)))	
			DrawSquare(96*4+20,480-20,64*7+10,64*8-10,1,0xFF);//Y
			else
			DrawSquare(96*4+20,480-20,64*7+10,64*8-10,1,0xFF0000);//Y
			print(36+96*4,64*7+26,"R",0xffffff);	
			break;
			case KEY_ENTER :
			i|=1<<19;
			switch((j&(1<<19)))
			{	
				case 0:j|=(1<<19);
				break;
				default:
				j&=~(1<<19);
				break;
			}
			DrawSquare(0,16*20,4*16,6*16,1,0);
			print(0,4,"key code is 0x",0xFF);
			sprintf(tmp,"%X",key);
			print(16,4,tmp,0xFF);
			print(0,5,"Enter is press!",0xFF);
			if((j&(1<<19)))	
			DrawCircle(85*4,64*7,30,1,0xFF);
			else
			DrawCircle(85*4,64*7,30,1,0xFF0000);
			
			if((i&0x3FFFF)==0x3FFFF)
				{
					key1=0;
					
				}
			break;
			}
			
		Lcd_Update();
		}
	}
	//Lcd_ClearScr(0x0);   //黑色
	//Versions();
	while(!(KEY_BACK==KeyScan()));
}		
*/