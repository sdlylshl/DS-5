/*********************************************
  NAME: profile.h
  DESC: 
  HISTORY:
  Mar.25.2002:purnnamu: reuse the source of S3C2400X u24xmon 
 *********************************************/


#ifndef __profile_h__
#define __profile_h__

#ifdef __cplusplus
extern "C" {
#endif
void Init_Timer0(void);
void Init_Timer1(void);
void Init_Timer2(void);
void Init_Timer3(void);
void Init_Timer4(U32 sec);
extern void Timer_InitEx(void);
extern void Timer_StartEx(void);
extern U32 Timer_StopEx(void);

#ifdef __cplusplus
}
#endif

#endif /*__profile_h__*/ 
