#ifndef EEadr
#define EEadr

#define EEAddr_CSystemEEpromstart							0x0000	//

#define EEAddr_CSystemFirstStart_Flag						0x0010	//ϵͳ��һ��������־��55AA=����������������Ϊ�״�����

#define EEAddr_CUserPassword								0x0020	//�û����루6�ֽڣ�
#define EEAddr_CSystemPassword								0x0027	//ϵͳ���루8�ֽڣ�

#define EEAddr_CLanguage									0x0030	//����ѡ����Ӣ�ģ�(1�ֽ�)
																	//���ܿ�������
#define EEAddr_CUserChangeSpeed_OnOff						0x0038	//�û���������
#define EEAddr_CGPS_OnOff									0x0039	//GPS���ܿ���	
#define EEAddr_CWorkTime_OnOff								0x003a	//����ʱ����ʾ��Ĭ��Ϊ��ʾ�ۼƹ���ʱ�䣩
#define EEAddr_CAutoWarm_OnOff								0x003b	//�Զ�ů������
#define EEAddr_CPreserveNotice_OnOff						0x003c	//������ʾ����
#define EEAddr_CBrokenHammer_OnOff							0x003d	//���鴸���ܿ���
#define EEAddr_CDayNight_OnOff  							0x003e	//���̱�����ҹģʽ���ܿ���

#define EEAddr_CDayNight_DayHour							0x0040							//���̱�����翪ʼʱ�䣨ʱ��
#define EEAddr_CDayNight_DayMinute							EEAddr_CDayNight_DayHour+1		//���̱�����翪ʼʱ�䣨�֣�
#define EEAddr_CDayNight_NightHour							EEAddr_CDayNight_DayHour+1+1	//���̱���ҹ��ʼʱ�䣨ʱ��
#define EEAddr_CDayNight_NightMinute						EEAddr_CDayNight_DayHour+1+1+1	//���̱���ҹ��ʼʱ�䣨�֣�

#define EEAddr_SWorkMode_H_SpeedStepH						0x0050		//Hģʽ ת��΢������
#define EEAddr_SWorkMode_H_SpeedStepL						0x0052		//Hģʽ ת��΢������
#define EEAddr_SWorkMode_M_SpeedStepH						0x0054		//Mģʽ ת��΢������
#define EEAddr_SWorkMode_M_SpeedStepL						0x0056		//Mģʽ ת��΢������
#define EEAddr_SWorkMode_L_SpeedStepH						0x0058		//Lģʽ ת��΢������
#define EEAddr_SWorkMode_L_SpeedStepL						0x005a		//Lģʽ ת��΢������
#define EEAddr_SWorkMode_F_SpeedStepH						0x005c		//Fģʽ ת��΢������
#define EEAddr_SWorkMode_F_SpeedStepL						0x005e		//Fģʽ ת��΢������
#define EEAddr_SWorkMode_B_SpeedStepH						0x0060		//Bģʽ ת��΢������
#define EEAddr_SWorkMode_B_SpeedStepL						0x0062		//Bģʽ ת��΢������

#define EEAddr_SWorkMode_H_Speed							0x0064   						//ת��
#define EEAddr_CWorkMode_H_Ratio							EEAddr_SWorkMode_H_Speed+2		//Ч��   66
#define EEAddr_CWorkMode_H_SpeedRatio						EEAddr_SWorkMode_H_Speed+1+1	//������ 67
#define EEAddr_SWorkMode_M_Speed							0x0068 							//ת��
#define EEAddr_CWorkMode_M_Ratio							EEAddr_SWorkMode_M_Speed+2		//Ч��   6a
#define EEAddr_CWorkMode_M_SpeedRatio						EEAddr_SWorkMode_M_Speed+1+1 	//������ 6b
#define EEAddr_SWorkMode_L_Speed							0x006c   						//ת��
#define EEAddr_CWorkMode_L_Ratio							EEAddr_SWorkMode_L_Speed+2		//Ч��   6e
#define EEAddr_CWorkMode_L_SpeedRatio						EEAddr_SWorkMode_L_Speed+1+1	//������ 6f

#define EEAddr_SWorkMode_F_Speed							0x0070							//ת��
#define EEAddr_CWorkMode_F_Ratio							EEAddr_SWorkMode_F_Speed+2		//Ч��	72
#define EEAddr_CWorkMode_F_SpeedRatio						EEAddr_SWorkMode_F_Speed+1+1	//������73
#define EEAddr_SWorkMode_B_Speed							0x0074   						//ת��
#define EEAddr_CWorkMode_B_Ratio							EEAddr_SWorkMode_B_Speed+2 		//Ч��  75
#define EEAddr_CWorkMode_B_SpeedRatio						EEAddr_SWorkMode_B_Speed+1+1 	//������76

#define EEAddr_SWorkMode_MotorGear_Value					0x0080								//����������
#define EEAddr_CWorkMode_SpeedStep							EEAddr_SWorkMode_MotorGear_Value+2	//ת��΢������ 
#define EEAddr_CWorkMode_IdleSpeed_OneTime 					EEAddr_SWorkMode_MotorGear_Value+2+2 //һ�׶ε���ʱ�� S	 
#define EEAddr_CWorkMode_IdleSpeed_TwoTime					EEAddr_SWorkMode_MotorGear_Value+2+2+2	//���׶ε���ʱ�� M 
#define EEAddr_SWorkMode_IdleSpeed_OneValue					EEAddr_SWorkMode_MotorGear_Value+2+2+2+2 //һ�׶ε���ֵ  
#define EEAddr_SWorkMode_IdleSpeed_TwoValue					EEAddr_SWorkMode_MotorGear_Value+2+2+2+2+2	//���׶ε���ֵ 

#define EEAddr_CAlarmSet_ColdWater_Temperature				0x0090				//��ȴҺ�¶�(H)	
#define EEAddr_CAlarmSet_ColdWater_Temperature_Rank			EEAddr_CAlarmSet_ColdWater_Temperature+1//��ȴҺ�¶�(H)��������
#define EEAddr_CAlarmSet_OilLevel							EEAddr_CAlarmSet_ColdWater_Temperature+1+1	//ȼ����λ(L)	
#define EEAddr_CAlarmSet_OilLevel_Rank						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1//ȼ����λ(L)��������		
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureH		EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1	//Һѹ���¶�(H)
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureH_Rank	EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1	//Һѹ���¶�(H)��������	
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureL		EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1	//Һѹ���¶�(L)
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureL_Rank	EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1	//Һѹ���¶�(L)��������	 	 
#define EEAddr_SAlarmSet_System_PressureH					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1					//ϵͳѹ��(H)	
#define EEAddr_CAlarmSet_System_PressureH_Rank				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2				//ϵͳѹ��(H)��������				
#define EEAddr_SAlarmSet_System_PressureL					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1					//ϵͳѹ��(L)	
#define EEAddr_SAlarmSet_System_PressureL_Rank				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2				//ϵͳѹ��(L)��������				            			            
#define EEAddr_CAlarmSet_System_PowerH						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1						//ϵͳ��ѹ(H)	
#define EEAddr_CAlarmSet_System_PowerH_Rank					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1					//ϵͳ��ѹ(H)��������			
#define EEAddr_CAlarmSet_System_PowerL						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1						//ϵͳ��ѹ(L)	
#define EEAddr_CAlarmSet_System_PowerL_Rank					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1 //a1					//ϵͳ��ѹ(L)��������		

#define EEAddr_CAlarmSet_Forerunner_PressureH				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1				//�ȵ�ѹ��(H)
#define EEAddr_CAlarmSet_Forerunner_PressureH_Rank			EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1			//�ȵ�ѹ��(H)�������� 	 	
#define EEAddr_CAlarmSet_Forerunner_PressureL				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1				//�ȵ�ѹ��(L):
#define EEAddr_CAlarmSet_Forerunner_PressureL_Rank			EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1			//�ȵ�ѹ��(L)��������
 	
#define EEAddr_CAlarmSet_MachineOil_Rank					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1+1						//����ѹ���������� 	��������
#define EEAddr_CAlarmSet_AirFilter_Rank						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1+1+1						//������������������ 	��������
#define EEAddr_CAlarmSet_ColdWaterLevelL_Rank				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1+1+1+1//a8				//��ȴҺҺλ(L)�������� 	��������


														//������в�������:
#define EEAddr_CRain_Clean_RunTime1							0x00b0								//ģʽ1//�˶�ʱ��
#define EEAddr_CRain_Clean_StopTime1						EEAddr_CRain_Clean_RunTime1+1								//ֹͣʱ��
#define EEAddr_CRain_Clean_RunTime2							EEAddr_CRain_Clean_RunTime1+1+1								////ģʽ2�˶�ʱ��
#define EEAddr_CRain_Clean_StopTime2						EEAddr_CRain_Clean_RunTime1+1+1+1								//ֹͣʱ��


														//ϴ�������в�������
#define EEAddr_CSpurtWater_WashTime							0x00b4								//��ϴʱ��
#define EEAddr_CSpurtWater_CleanTimes						EEAddr_CSpurtWater_WashTime+1	//��δ���

														//�Զ�ů����������
#define EEAddr_SAutoWarm_ColdWater_Temperature				0x00b6	//��ȴҺ�¶ȡ�  
#define EEAddr_SAutoWarm_Speed								EEAddr_SAutoWarm_ColdWater_Temperature+2					//ת��  

														//�������в�������
//#define EEAddr_SHighWorkMode_System_Pressure				0x00bb		//ϵͳѹ����   

														//�ھ�������������
#define EEAddr_SIncreasePower_AllowSpeed					0x00c0			//ת�١�
#define EEAddr_SIncreasePower_RunTime						EEAddr_SIncreasePower_AllowSpeed+2			//ִ��ʱ�� S	
#define EEAddr_SCIncreasePower_NextRunTime					EEAddr_SIncreasePower_AllowSpeed+2+2		//���ʱ�� S	


#define EEAddr_CPreserveAlarmTime							0x00d0			//������ʾʱ�� H
#define EEAddr_CGPSCommMistake_JudgeTime					EEAddr_CPreserveAlarmTime+1		//GPSͨѶ�쳣�ж�ʱ�� M
#define EEAddr_CGPSCommMistake_ExecuteTime					EEAddr_CPreserveAlarmTime+1+1	//GPSͨѶ�쳣����ʱ�� M
#define EEAddr_CMenutoMainPicture_Time						EEAddr_CPreserveAlarmTime+1+1+1		//�˵�����������ʱ�� S	
#define EEAddr_CLCDDefault_BackLight						EEAddr_CPreserveAlarmTime+1+1+1+1			//LCDĬ�ϱ�������
#define EEAddr_COilUsedData_SendTime						EEAddr_CPreserveAlarmTime+1+1+1+1+1			//�ͺ����ݴ���ʱ��	
#define EEAddr_COilUsed_CheckTime							EEAddr_CPreserveAlarmTime+1+1+1+1+1+1			//�ͺļ��ʱ��		
#define EEAddr_CSystemData_SaveTime							EEAddr_CPreserveAlarmTime+1+1+1+1+1+1+1			//���ݱ���ʱ��	


#define EEAddr_CAlarmTimeDone_ColdWater						0x00da									//������ȴҺ�¶ȸ߱���ʱ��
#define EEAddr_CAlarmTimeDone_ColdWaterLevel				EEAddr_CAlarmTimeDone_ColdWater+1		//������ȴҺҺλ�ͱ���ʱ��
#define EEAddr_CAlarmTimeDone_MachinelOilPressure			EEAddr_CAlarmTimeDone_ColdWater+1+1		//�������ѹ���ͱ���ʱ��
#define EEAddr_CAlarmTimeDone_SystemPressure				EEAddr_CAlarmTimeDone_ColdWater+1+1+1	//����ϵͳѹ���߱���ʱ��


#define EEAddr_CBucketValue									0x00e0						//��������(1�ֽ�)
#define EEAddr_COilBoxLength								EEAddr_CBucketValue+1		//���䳤��
#define EEAddr_COilBoxWidth									EEAddr_CBucketValue+1+1		//������
#define EEAddr_COilBoxHeigth								EEAddr_CBucketValue+1+1+1	//����߶�

#define EEAddr_CServerTel									0x00f0						//����绰

#define EEAddr_CServerTel400								0x0100						//400

#define EEAddr_CSensorAdjust								0x0110						//������У��

								

//��������ݣ�
//��������
#define EEAddr_CPreserveSet_0H_1							0x0200			//�������
#define EEAddr_CPreserveSet_0H_2							EEAddr_CPreserveSet_0H_1+1
#define EEAddr_CPreserveSet_0H_3							EEAddr_CPreserveSet_0H_1+1+1
#define EEAddr_CPreserveSet_0H_4							EEAddr_CPreserveSet_0H_1+1+1+1
#define EEAddr_CPreserveSet_0H_5							EEAddr_CPreserveSet_0H_1+1+1+1+1
#define EEAddr_CPreserveSet_0H_6							EEAddr_CPreserveSet_0H_1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_0H					EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_0H					EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_0H_Year				EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_0H_Month				EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_0H_Date				EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_250H_1							0x0210			//250H
#define EEAddr_CPreserveSet_250H_2							EEAddr_CPreserveSet_250H_1+1
#define EEAddr_CPreserveSet_250H_3							EEAddr_CPreserveSet_250H_1+1+1
#define EEAddr_CPreserveSet_250H_4							EEAddr_CPreserveSet_250H_1+1+1+1
#define EEAddr_CPreserveSet_250H_5							EEAddr_CPreserveSet_250H_1+1+1+1+1
#define EEAddr_CPreserveSet_250H_6							EEAddr_CPreserveSet_250H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_250H_7							EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_250H_8							EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_250H					EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_250H					EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_250H_Year				EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_250H_Month			EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_250H_Date				EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_500H_1							0x0220			//500H
#define EEAddr_CPreserveSet_500H_2							EEAddr_CPreserveSet_500H_1+1
#define EEAddr_CPreserveSet_500H_3							EEAddr_CPreserveSet_500H_1+1+1
#define EEAddr_CPreserveSet_500H_4							EEAddr_CPreserveSet_500H_1+1+1+1
#define EEAddr_CPreserveSet_500H_5							EEAddr_CPreserveSet_500H_1+1+1+1+1
#define EEAddr_CPreserveSet_500H_6							EEAddr_CPreserveSet_500H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_500H_7							EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_500H_8							EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_500H					EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_500H					EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_500H_Year				EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_500H_Month			EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_500H_Date				EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_750H_1							0x0230			//750H
#define EEAddr_CPreserveSet_750H_2							EEAddr_CPreserveSet_750H_1+1
#define EEAddr_CPreserveSet_750H_3							EEAddr_CPreserveSet_750H_1+1+1
#define EEAddr_CPreserveSet_750H_4							EEAddr_CPreserveSet_750H_1+1+1+1
#define EEAddr_CPreserveSet_750H_5							EEAddr_CPreserveSet_750H_1+1+1+1+1
#define EEAddr_CPreserveSet_750H_6							EEAddr_CPreserveSet_750H_1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_750H					EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_750H					EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_750H_Year				EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_750H_Month			EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_750H_Date				EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_1000H_1							0x0240			//1000H
#define EEAddr_CPreserveSet_1000H_2							EEAddr_CPreserveSet_1000H_1+1
#define EEAddr_CPreserveSet_1000H_3							EEAddr_CPreserveSet_1000H_1+1+1
#define EEAddr_CPreserveSet_1000H_4							EEAddr_CPreserveSet_1000H_1+1+1+1
#define EEAddr_CPreserveSet_1000H_5							EEAddr_CPreserveSet_1000H_1+1+1+1+1
#define EEAddr_CPreserveSet_1000H_6							EEAddr_CPreserveSet_1000H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1000H_7							EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_1000H_8							EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1000H				EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_1000H					EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_1000H_Year			EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1000H_Month			EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1000H_Date			EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_1250H_1							0x0250			//1250H
#define EEAddr_CPreserveSet_1250H_2							EEAddr_CPreserveSet_1250H_1+1
#define EEAddr_CPreserveSet_1250H_3							EEAddr_CPreserveSet_1250H_1+1+1
#define EEAddr_CPreserveSet_1250H_4							EEAddr_CPreserveSet_1250H_1+1+1+1
#define EEAddr_CPreserveSet_1250H_5							EEAddr_CPreserveSet_1250H_1+1+1+1+1
#define EEAddr_CPreserveSet_1250H_6							EEAddr_CPreserveSet_1250H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1250H_7							EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_1250H_8							EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1250H				EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_1250H					EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_1250H_Year			EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1250H_Month			EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1250H_Date			EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1+2+1+1			//������� ��


#define EEAddr_CPreserveSet_1500H_1							0x0260			//1500H
#define EEAddr_CPreserveSet_1500H_2							EEAddr_CPreserveSet_1500H_1+1
#define EEAddr_CPreserveSet_1500H_3							EEAddr_CPreserveSet_1500H_1+1+1
#define EEAddr_CPreserveSet_1500H_4							EEAddr_CPreserveSet_1500H_1+1+1+1
#define EEAddr_CPreserveSet_1500H_5							EEAddr_CPreserveSet_1500H_1+1+1+1+1
#define EEAddr_CPreserveSet_1500H_6							EEAddr_CPreserveSet_1500H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1500H_7							EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_1500H_8							EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1500H				EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_1500H					EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_1500H_Year			EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1500H_Month			EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1500H_Date			EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_1750H_1							0x0270			//1750H
#define EEAddr_CPreserveSet_1750H_2							EEAddr_CPreserveSet_1750H_1+1
#define EEAddr_CPreserveSet_1750H_3							EEAddr_CPreserveSet_1750H_1+1+1
#define EEAddr_CPreserveSet_1750H_4							EEAddr_CPreserveSet_1750H_1+1+1+1
#define EEAddr_CPreserveSet_1750H_5							EEAddr_CPreserveSet_1750H_1+1+1+1+1
#define EEAddr_CPreserveSet_1750H_6							EEAddr_CPreserveSet_1750H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1750H_7							EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1750H				EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_1750H					EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_1750H_Year			EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1750H_Month			EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_1750H_Date			EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1+2+1+1			//������� ��

#define EEAddr_CPreserveSet_2000H_1							0x0280			//2000H
#define EEAddr_CPreserveSet_2000H_2							EEAddr_CPreserveSet_2000H_1+1
#define EEAddr_CPreserveSet_2000H_3							EEAddr_CPreserveSet_2000H_1+1+1
#define EEAddr_CPreserveSet_2000H_4							EEAddr_CPreserveSet_2000H_1+1+1+1
#define EEAddr_CPreserveSet_2000H_5							EEAddr_CPreserveSet_2000H_1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_6							EEAddr_CPreserveSet_2000H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_7							EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1

#define EEAddr_CPreserveSet_2000H_8							EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_9							EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_10						EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_11						EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_12						EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_13						EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_2000H_14						EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1


#define EEAddr_CPreserveInfo_DoneState_2000H				EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1		//���״̬
#define EEAddr_SPreserveInfo_LeftTime_2000H					EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1			//ʣ��ʱ��
#define EEAddr_CPreserveInfo_DoneTime_2000H_Year			EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+2			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_21000H_Month			EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+2+1			//������� ��
#define EEAddr_CPreserveInfo_DoneTime_21000H_Date			EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+2+1+1			//������� ��



#define EEAddr_WWorkTime_H_Total							0x02b0	//H����ģʽʱ��ͳ��  ��λ���ӣ���ʾΪСʱ
#define EEAddr_WWorkTime_M_Total							EEAddr_WWorkTime_H_Total+4		//M����ģʽʱ��ͳ��
#define EEAddr_WWorkTime_L_Total							EEAddr_WWorkTime_H_Total+4+4		//L����ģʽʱ��ͳ��
#define EEAddr_WWorkTime_F_Total							EEAddr_WWorkTime_H_Total+4+4+4		//F����ģʽʱ��ͳ��
#define EEAddr_WWorkTime_B_Total							EEAddr_WWorkTime_H_Total+4+4+4+4		//B����ģʽʱ��ͳ��


#define EEAddr_CEngine_Model								0x02d0//���ͻ��� (10�ֽڣ�8�ֽ�Ϊ�����Ļ��ţ�����λ�������ã��������ı��

//#define EEAddr_CGPS_ID										0x02e0						//GPS����
//#define EEAddr_CGPS_SIM										EEAddr_CGPS_ID+12			//GPS SIM��

#define EEAddr_SCounter										0x02f0	//�������洢  2�ֽ�


#define EEAddr_SAlarmTimes_OilWaterFilter					0x0300													//��ˮ���뱨������
#define EEAddr_SAlarmTimes_OilLevel							EEAddr_SAlarmTimes_OilWaterFilter+2						//ȼ����λ�ͱ�������302
#define EEAddr_SAlarmTimes_ColdWaterTemperature				EEAddr_SAlarmTimes_OilWaterFilter+2+2					//��ȴҺ�¶ȸ߱�������304
#define EEAddr_SAlarmTimes_ColdWaterLevel					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2					//��ȴҺҺλ��������306
#define EEAddr_SAlarmTimes_AirFilter						EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2				//������������������308
#define EEAddr_SAlarmTimes_MachinelOilPressure				EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2				//����ѹ����������310
#define EEAddr_SAlarmTimes_SystemVoltage					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2			//ϵͳ��ѹ�쳣��������312
#define EEAddr_SAlarmTimes_MotorSpeed						EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2			//������ת���쳣��������314
#define EEAddr_SAlarmTimes_GPSCommState						EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2+2		//GPSͨѶ״̬��������316
#define EEAddr_SAlarmTimes_SystemPressure					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2+2+2		//ϵͳѹ���߱�������318
#define EEAddr_SAlarmTimes_FirstPressure					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2+2+2+2	//�ȵ�ѹ���߱�������320

#define EEAddr_CAuthorizationCode							0x0320				//��Ȩ�루8�ֽڣ�������+��ȡʱ�䣩
#define EEAddr_CAuthorizationTime							0x0328				//��Ȩ���ȡʱ�䣨��ʱ���룩
#endif