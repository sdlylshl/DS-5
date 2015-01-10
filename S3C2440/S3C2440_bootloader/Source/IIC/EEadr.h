#ifndef EEadr
#define EEadr

#define EEAddr_CSystemEEpromstart							0x0000	//

#define EEAddr_CSystemFirstStart_Flag						0x0010	//系统第一次启动标志（55AA=已启动过；其它视为首次启动

#define EEAddr_CUserPassword								0x0020	//用户密码（6字节）
#define EEAddr_CSystemPassword								0x0027	//系统密码（8字节）

#define EEAddr_CLanguage									0x0030	//语言选择（中英文）(1字节)
																	//功能开启设置
#define EEAddr_CUserChangeSpeed_OnOff						0x0038	//用户调速设置
#define EEAddr_CGPS_OnOff									0x0039	//GPS功能开启	
#define EEAddr_CWorkTime_OnOff								0x003a	//工作时间显示（默认为显示累计工作时间）
#define EEAddr_CAutoWarm_OnOff								0x003b	//自动暖机开启
#define EEAddr_CPreserveNotice_OnOff						0x003c	//保养提示开启
#define EEAddr_CBrokenHammer_OnOff							0x003d	//破碎锤功能开启
#define EEAddr_CDayNight_OnOff  							0x003e	//键盘背光昼夜模式功能开启

#define EEAddr_CDayNight_DayHour							0x0040							//键盘背光白昼开始时间（时）
#define EEAddr_CDayNight_DayMinute							EEAddr_CDayNight_DayHour+1		//键盘背光白昼开始时间（分）
#define EEAddr_CDayNight_NightHour							EEAddr_CDayNight_DayHour+1+1	//键盘背光夜晚开始时间（时）
#define EEAddr_CDayNight_NightMinute						EEAddr_CDayNight_DayHour+1+1+1	//键盘背光夜晚开始时间（分）

#define EEAddr_SWorkMode_H_SpeedStepH						0x0050		//H模式 转速微调上限
#define EEAddr_SWorkMode_H_SpeedStepL						0x0052		//H模式 转速微调下限
#define EEAddr_SWorkMode_M_SpeedStepH						0x0054		//M模式 转速微调上限
#define EEAddr_SWorkMode_M_SpeedStepL						0x0056		//M模式 转速微调下限
#define EEAddr_SWorkMode_L_SpeedStepH						0x0058		//L模式 转速微调上限
#define EEAddr_SWorkMode_L_SpeedStepL						0x005a		//L模式 转速微调下限
#define EEAddr_SWorkMode_F_SpeedStepH						0x005c		//F模式 转速微调上限
#define EEAddr_SWorkMode_F_SpeedStepL						0x005e		//F模式 转速微调下限
#define EEAddr_SWorkMode_B_SpeedStepH						0x0060		//B模式 转速微调上限
#define EEAddr_SWorkMode_B_SpeedStepL						0x0062		//B模式 转速微调下限

#define EEAddr_SWorkMode_H_Speed							0x0064   						//转速
#define EEAddr_CWorkMode_H_Ratio							EEAddr_SWorkMode_H_Speed+2		//效率   66
#define EEAddr_CWorkMode_H_SpeedRatio						EEAddr_SWorkMode_H_Speed+1+1	//调速率 67
#define EEAddr_SWorkMode_M_Speed							0x0068 							//转速
#define EEAddr_CWorkMode_M_Ratio							EEAddr_SWorkMode_M_Speed+2		//效率   6a
#define EEAddr_CWorkMode_M_SpeedRatio						EEAddr_SWorkMode_M_Speed+1+1 	//调速率 6b
#define EEAddr_SWorkMode_L_Speed							0x006c   						//转速
#define EEAddr_CWorkMode_L_Ratio							EEAddr_SWorkMode_L_Speed+2		//效率   6e
#define EEAddr_CWorkMode_L_SpeedRatio						EEAddr_SWorkMode_L_Speed+1+1	//调速率 6f

#define EEAddr_SWorkMode_F_Speed							0x0070							//转速
#define EEAddr_CWorkMode_F_Ratio							EEAddr_SWorkMode_F_Speed+2		//效率	72
#define EEAddr_CWorkMode_F_SpeedRatio						EEAddr_SWorkMode_F_Speed+1+1	//调速率73
#define EEAddr_SWorkMode_B_Speed							0x0074   						//转速
#define EEAddr_CWorkMode_B_Ratio							EEAddr_SWorkMode_B_Speed+2 		//效率  75
#define EEAddr_CWorkMode_B_SpeedRatio						EEAddr_SWorkMode_B_Speed+1+1 	//调速率76

#define EEAddr_SWorkMode_MotorGear_Value					0x0080								//发动机齿数
#define EEAddr_CWorkMode_SpeedStep							EEAddr_SWorkMode_MotorGear_Value+2	//转速微调步进 
#define EEAddr_CWorkMode_IdleSpeed_OneTime 					EEAddr_SWorkMode_MotorGear_Value+2+2 //一阶段怠速时间 S	 
#define EEAddr_CWorkMode_IdleSpeed_TwoTime					EEAddr_SWorkMode_MotorGear_Value+2+2+2	//二阶段怠速时间 M 
#define EEAddr_SWorkMode_IdleSpeed_OneValue					EEAddr_SWorkMode_MotorGear_Value+2+2+2+2 //一阶段怠速值  
#define EEAddr_SWorkMode_IdleSpeed_TwoValue					EEAddr_SWorkMode_MotorGear_Value+2+2+2+2+2	//二阶段怠速值 

#define EEAddr_CAlarmSet_ColdWater_Temperature				0x0090				//冷却液温度(H)	
#define EEAddr_CAlarmSet_ColdWater_Temperature_Rank			EEAddr_CAlarmSet_ColdWater_Temperature+1//冷却液温度(H)报警级别
#define EEAddr_CAlarmSet_OilLevel							EEAddr_CAlarmSet_ColdWater_Temperature+1+1	//燃油油位(L)	
#define EEAddr_CAlarmSet_OilLevel_Rank						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1//燃油油位(L)报警级别		
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureH		EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1	//液压油温度(H)
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureH_Rank	EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1	//液压油温度(H)报警级别	
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureL		EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1	//液压油温度(L)
#define EEAddr_CAlarmSet_HydraumaticOil_TemperatureL_Rank	EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1	//液压油温度(L)报警级别	 	 
#define EEAddr_SAlarmSet_System_PressureH					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1					//系统压力(H)	
#define EEAddr_CAlarmSet_System_PressureH_Rank				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2				//系统压力(H)报警级别				
#define EEAddr_SAlarmSet_System_PressureL					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1					//系统压力(L)	
#define EEAddr_SAlarmSet_System_PressureL_Rank				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2				//系统压力(L)报警级别				            			            
#define EEAddr_CAlarmSet_System_PowerH						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1						//系统电压(H)	
#define EEAddr_CAlarmSet_System_PowerH_Rank					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1					//系统电压(H)报警级别			
#define EEAddr_CAlarmSet_System_PowerL						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1						//系统电压(L)	
#define EEAddr_CAlarmSet_System_PowerL_Rank					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1 //a1					//系统电压(L)报警级别		

#define EEAddr_CAlarmSet_Forerunner_PressureH				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1				//先导压力(H)
#define EEAddr_CAlarmSet_Forerunner_PressureH_Rank			EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1			//先导压力(H)报警级别 	 	
#define EEAddr_CAlarmSet_Forerunner_PressureL				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1				//先导压力(L):
#define EEAddr_CAlarmSet_Forerunner_PressureL_Rank			EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1			//先导压力(L)报警级别
 	
#define EEAddr_CAlarmSet_MachineOil_Rank					EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1+1						//机油压力报警级别 	：开关量
#define EEAddr_CAlarmSet_AirFilter_Rank						EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1+1+1						//空气滤清器报警级别 	：开关量
#define EEAddr_CAlarmSet_ColdWaterLevelL_Rank				EEAddr_CAlarmSet_ColdWater_Temperature+1+1+1+1+1+1+1+1+2+1+2+1+1+1+1+1+1+1+1+1+1+1//a8				//冷却液液位(L)报警级别 	：开关量


														//雨刮运行参数设置:
#define EEAddr_CRain_Clean_RunTime1							0x00b0								//模式1//运动时间
#define EEAddr_CRain_Clean_StopTime1						EEAddr_CRain_Clean_RunTime1+1								//停止时间
#define EEAddr_CRain_Clean_RunTime2							EEAddr_CRain_Clean_RunTime1+1+1								////模式2运动时间
#define EEAddr_CRain_Clean_StopTime2						EEAddr_CRain_Clean_RunTime1+1+1+1								//停止时间


														//洗涤器运行参数设置
#define EEAddr_CSpurtWater_WashTime							0x00b4								//冲洗时间
#define EEAddr_CSpurtWater_CleanTimes						EEAddr_CSpurtWater_WashTime+1	//雨刮次数

														//自动暖机参数设置
#define EEAddr_SAutoWarm_ColdWater_Temperature				0x00b6	//冷却液温度≤  
#define EEAddr_SAutoWarm_Speed								EEAddr_SAutoWarm_ColdWater_Temperature+2					//转速  

														//重载运行参数设置
//#define EEAddr_SHighWorkMode_System_Pressure				0x00bb		//系统压力≥   

														//挖掘增力参数设置
#define EEAddr_SIncreasePower_AllowSpeed					0x00c0			//转速≥
#define EEAddr_SIncreasePower_RunTime						EEAddr_SIncreasePower_AllowSpeed+2			//执行时间 S	
#define EEAddr_SCIncreasePower_NextRunTime					EEAddr_SIncreasePower_AllowSpeed+2+2		//间隔时间 S	


#define EEAddr_CPreserveAlarmTime							0x00d0			//保养提示时间 H
#define EEAddr_CGPSCommMistake_JudgeTime					EEAddr_CPreserveAlarmTime+1		//GPS通讯异常判定时间 M
#define EEAddr_CGPSCommMistake_ExecuteTime					EEAddr_CPreserveAlarmTime+1+1	//GPS通讯异常处理时间 M
#define EEAddr_CMenutoMainPicture_Time						EEAddr_CPreserveAlarmTime+1+1+1		//菜单返回主界面时间 S	
#define EEAddr_CLCDDefault_BackLight						EEAddr_CPreserveAlarmTime+1+1+1+1			//LCD默认背光亮度
#define EEAddr_COilUsedData_SendTime						EEAddr_CPreserveAlarmTime+1+1+1+1+1			//油耗数据传递时间	
#define EEAddr_COilUsed_CheckTime							EEAddr_CPreserveAlarmTime+1+1+1+1+1+1			//油耗检测时间		
#define EEAddr_CSystemData_SaveTime							EEAddr_CPreserveAlarmTime+1+1+1+1+1+1+1			//数据保存时间	


#define EEAddr_CAlarmTimeDone_ColdWater						0x00da									//处理冷却液温度高报警时间
#define EEAddr_CAlarmTimeDone_ColdWaterLevel				EEAddr_CAlarmTimeDone_ColdWater+1		//处理冷却液液位低报警时间
#define EEAddr_CAlarmTimeDone_MachinelOilPressure			EEAddr_CAlarmTimeDone_ColdWater+1+1		//处理机油压力低报警时间
#define EEAddr_CAlarmTimeDone_SystemPressure				EEAddr_CAlarmTimeDone_ColdWater+1+1+1	//处理系统压力高报警时间


#define EEAddr_CBucketValue									0x00e0						//铲斗容量(1字节)
#define EEAddr_COilBoxLength								EEAddr_CBucketValue+1		//油箱长度
#define EEAddr_COilBoxWidth									EEAddr_CBucketValue+1+1		//油箱宽度
#define EEAddr_COilBoxHeigth								EEAddr_CBucketValue+1+1+1	//油箱高度

#define EEAddr_CServerTel									0x00f0						//服务电话

#define EEAddr_CServerTel400								0x0100						//400

#define EEAddr_CSensorAdjust								0x0110						//传感器校正

								

//保存的数据：
//保养设置
#define EEAddr_CPreserveSet_0H_1							0x0200			//交机检查
#define EEAddr_CPreserveSet_0H_2							EEAddr_CPreserveSet_0H_1+1
#define EEAddr_CPreserveSet_0H_3							EEAddr_CPreserveSet_0H_1+1+1
#define EEAddr_CPreserveSet_0H_4							EEAddr_CPreserveSet_0H_1+1+1+1
#define EEAddr_CPreserveSet_0H_5							EEAddr_CPreserveSet_0H_1+1+1+1+1
#define EEAddr_CPreserveSet_0H_6							EEAddr_CPreserveSet_0H_1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_0H					EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_0H					EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_0H_Year				EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_0H_Month				EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_0H_Date				EEAddr_CPreserveSet_0H_1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

#define EEAddr_CPreserveSet_250H_1							0x0210			//250H
#define EEAddr_CPreserveSet_250H_2							EEAddr_CPreserveSet_250H_1+1
#define EEAddr_CPreserveSet_250H_3							EEAddr_CPreserveSet_250H_1+1+1
#define EEAddr_CPreserveSet_250H_4							EEAddr_CPreserveSet_250H_1+1+1+1
#define EEAddr_CPreserveSet_250H_5							EEAddr_CPreserveSet_250H_1+1+1+1+1
#define EEAddr_CPreserveSet_250H_6							EEAddr_CPreserveSet_250H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_250H_7							EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_250H_8							EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_250H					EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_250H					EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_250H_Year				EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_250H_Month			EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_250H_Date				EEAddr_CPreserveSet_250H_1+1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

#define EEAddr_CPreserveSet_500H_1							0x0220			//500H
#define EEAddr_CPreserveSet_500H_2							EEAddr_CPreserveSet_500H_1+1
#define EEAddr_CPreserveSet_500H_3							EEAddr_CPreserveSet_500H_1+1+1
#define EEAddr_CPreserveSet_500H_4							EEAddr_CPreserveSet_500H_1+1+1+1
#define EEAddr_CPreserveSet_500H_5							EEAddr_CPreserveSet_500H_1+1+1+1+1
#define EEAddr_CPreserveSet_500H_6							EEAddr_CPreserveSet_500H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_500H_7							EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_500H_8							EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_500H					EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_500H					EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_500H_Year				EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_500H_Month			EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_500H_Date				EEAddr_CPreserveSet_500H_1+1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

#define EEAddr_CPreserveSet_750H_1							0x0230			//750H
#define EEAddr_CPreserveSet_750H_2							EEAddr_CPreserveSet_750H_1+1
#define EEAddr_CPreserveSet_750H_3							EEAddr_CPreserveSet_750H_1+1+1
#define EEAddr_CPreserveSet_750H_4							EEAddr_CPreserveSet_750H_1+1+1+1
#define EEAddr_CPreserveSet_750H_5							EEAddr_CPreserveSet_750H_1+1+1+1+1
#define EEAddr_CPreserveSet_750H_6							EEAddr_CPreserveSet_750H_1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_750H					EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_750H					EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_750H_Year				EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_750H_Month			EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_750H_Date				EEAddr_CPreserveSet_750H_1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

#define EEAddr_CPreserveSet_1000H_1							0x0240			//1000H
#define EEAddr_CPreserveSet_1000H_2							EEAddr_CPreserveSet_1000H_1+1
#define EEAddr_CPreserveSet_1000H_3							EEAddr_CPreserveSet_1000H_1+1+1
#define EEAddr_CPreserveSet_1000H_4							EEAddr_CPreserveSet_1000H_1+1+1+1
#define EEAddr_CPreserveSet_1000H_5							EEAddr_CPreserveSet_1000H_1+1+1+1+1
#define EEAddr_CPreserveSet_1000H_6							EEAddr_CPreserveSet_1000H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1000H_7							EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_1000H_8							EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1000H				EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_1000H					EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_1000H_Year			EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_1000H_Month			EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_1000H_Date			EEAddr_CPreserveSet_1000H_1+1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

#define EEAddr_CPreserveSet_1250H_1							0x0250			//1250H
#define EEAddr_CPreserveSet_1250H_2							EEAddr_CPreserveSet_1250H_1+1
#define EEAddr_CPreserveSet_1250H_3							EEAddr_CPreserveSet_1250H_1+1+1
#define EEAddr_CPreserveSet_1250H_4							EEAddr_CPreserveSet_1250H_1+1+1+1
#define EEAddr_CPreserveSet_1250H_5							EEAddr_CPreserveSet_1250H_1+1+1+1+1
#define EEAddr_CPreserveSet_1250H_6							EEAddr_CPreserveSet_1250H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1250H_7							EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_1250H_8							EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1250H				EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_1250H					EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_1250H_Year			EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_1250H_Month			EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_1250H_Date			EEAddr_CPreserveSet_1250H_1+1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日


#define EEAddr_CPreserveSet_1500H_1							0x0260			//1500H
#define EEAddr_CPreserveSet_1500H_2							EEAddr_CPreserveSet_1500H_1+1
#define EEAddr_CPreserveSet_1500H_3							EEAddr_CPreserveSet_1500H_1+1+1
#define EEAddr_CPreserveSet_1500H_4							EEAddr_CPreserveSet_1500H_1+1+1+1
#define EEAddr_CPreserveSet_1500H_5							EEAddr_CPreserveSet_1500H_1+1+1+1+1
#define EEAddr_CPreserveSet_1500H_6							EEAddr_CPreserveSet_1500H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1500H_7							EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1
#define EEAddr_CPreserveSet_1500H_8							EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1500H				EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_1500H					EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_1500H_Year			EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_1500H_Month			EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_1500H_Date			EEAddr_CPreserveSet_1500H_1+1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

#define EEAddr_CPreserveSet_1750H_1							0x0270			//1750H
#define EEAddr_CPreserveSet_1750H_2							EEAddr_CPreserveSet_1750H_1+1
#define EEAddr_CPreserveSet_1750H_3							EEAddr_CPreserveSet_1750H_1+1+1
#define EEAddr_CPreserveSet_1750H_4							EEAddr_CPreserveSet_1750H_1+1+1+1
#define EEAddr_CPreserveSet_1750H_5							EEAddr_CPreserveSet_1750H_1+1+1+1+1
#define EEAddr_CPreserveSet_1750H_6							EEAddr_CPreserveSet_1750H_1+1+1+1+1+1
#define EEAddr_CPreserveSet_1750H_7							EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1

#define EEAddr_CPreserveInfo_DoneState_1750H				EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_1750H					EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_1750H_Year			EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_1750H_Month			EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_1750H_Date			EEAddr_CPreserveSet_1750H_1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日

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


#define EEAddr_CPreserveInfo_DoneState_2000H				EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1		//完成状态
#define EEAddr_SPreserveInfo_LeftTime_2000H					EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1			//剩余时间
#define EEAddr_CPreserveInfo_DoneTime_2000H_Year			EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+2			//完成日期 年
#define EEAddr_CPreserveInfo_DoneTime_21000H_Month			EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+2+1			//完成日期 月
#define EEAddr_CPreserveInfo_DoneTime_21000H_Date			EEAddr_CPreserveSet_2000H_1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+2+1+1			//完成日期 日



#define EEAddr_WWorkTime_H_Total							0x02b0	//H工作模式时间统计  单位分钟，显示为小时
#define EEAddr_WWorkTime_M_Total							EEAddr_WWorkTime_H_Total+4		//M工作模式时间统计
#define EEAddr_WWorkTime_L_Total							EEAddr_WWorkTime_H_Total+4+4		//L工作模式时间统计
#define EEAddr_WWorkTime_F_Total							EEAddr_WWorkTime_H_Total+4+4+4		//F工作模式时间统计
#define EEAddr_WWorkTime_B_Total							EEAddr_WWorkTime_H_Total+4+4+4+4		//B工作模式时间统计


#define EEAddr_CEngine_Model								0x02d0//机型机号 (10字节）8字节为真正的机号，后两位可用作泵，发动机的编号

//#define EEAddr_CGPS_ID										0x02e0						//GPS机号
//#define EEAddr_CGPS_SIM										EEAddr_CGPS_ID+12			//GPS SIM号

#define EEAddr_SCounter										0x02f0	//计数器存储  2字节


#define EEAddr_SAlarmTimes_OilWaterFilter					0x0300													//油水分离报警次数
#define EEAddr_SAlarmTimes_OilLevel							EEAddr_SAlarmTimes_OilWaterFilter+2						//燃油油位低报警次数302
#define EEAddr_SAlarmTimes_ColdWaterTemperature				EEAddr_SAlarmTimes_OilWaterFilter+2+2					//冷却液温度高报警次数304
#define EEAddr_SAlarmTimes_ColdWaterLevel					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2					//冷却液液位报警次数306
#define EEAddr_SAlarmTimes_AirFilter						EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2				//空气过滤器报警次数308
#define EEAddr_SAlarmTimes_MachinelOilPressure				EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2				//机油压力报警次数310
#define EEAddr_SAlarmTimes_SystemVoltage					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2			//系统电压异常报警次数312
#define EEAddr_SAlarmTimes_MotorSpeed						EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2			//发动机转速异常报警次数314
#define EEAddr_SAlarmTimes_GPSCommState						EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2+2		//GPS通讯状态报警次数316
#define EEAddr_SAlarmTimes_SystemPressure					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2+2+2		//系统压力高报警次数318
#define EEAddr_SAlarmTimes_FirstPressure					EEAddr_SAlarmTimes_OilWaterFilter+2+2+2+2+2+2+2+2+2+2	//先导压力高报警次数320

#define EEAddr_CAuthorizationCode							0x0320				//授权码（8字节）（车号+获取时间）
#define EEAddr_CAuthorizationTime							0x0328				//授权码获取时间（日时分秒）
#endif