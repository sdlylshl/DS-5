/*******************************************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	Communi.H
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-03-16 15:56
  
  Description:   主分机通信相关函数说明及全局量说明
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2011-03-16 15:56
       Author:		Xsky
       Modification:	建立本模块
**------------------------------------------------------------------------------------------------------
** 最后修改日期: 2011-04-13 14:45
** 修改人: Xsky
** 版本: v1.0.00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日期:  2011-04-13 14:45
** 版本: v1.0.00
** 修改: 
**			1. 增加定义FRM_ID_LShitBITn及相关引用
**			2. 
**			3. 
*******************************************************************************************************/

#ifndef __COMMUNI_H__
#define __COMMUNI_H__

#ifndef COMMUNI_GLOBALS
#define COMMUNI_EXT		extern
#else
#define COMMUNI_EXT
#endif

#define  CAN_FILTER_N		MAX_PLAYER	/* CAN过滤器使用数量为玩家数量 */

/***************************************************************************************************/
// 常数定义
//#define  FRM_ID_PRIO_Msk		( BIT(28)| BIT(27) | BIT(26) | BIT(25) | BIT(24) )
#define  FRM_ID_PRIO_Val		((uint32)0x01 << 24)	/* 本机发送数据优先级定义 */
#define  FRM_BASE_MAC			((uint32)0x10)			/* MAC基地址 */
#define  FRM_HOST_MAC			((uint32)0x10)			/* 本机MAC */
#define  FRM_PLAYER_MAC_BASE	((uint32)0x11)			/* 分机MAC基地址 */
#define  FRM_BROADCAST_MAC		((uint32)0x3F)			/* 广播地址 */
#define  FRM_SRC_MAC_BITn		16						/* 源MAC位号 */
#define  FRM_SRC_MAC_Msk		0x3F
#define  FRM_DST_MAC_BITn		8						/* 目的MAC位号 */
#define  FRM_DST_MAC_Msk		0x3F
#define  FRM_ID_TYPE_BITnH		6						/* 帧类型/功能码位号最高位 */
#define  FRM_ID_TYPE_BITnL		0						/* 帧类型/功能码位号最低位 */
#define  FRM_ID_TYPE_Msk		((uint32)0x7F)			/* 帧类型掩码 */
#define  FRM_ID_LShitBITn		3						/* CAN发送寄存器ID左移位数,Add @2011-04-13 14:45 */
#define  FRM_MAC_BROADCAST		0x36					/* 广播地址 */
// 定义标针对指定MAC源地址的标识符默认值构造函数
// 参数: DstMAC:目的MAC地址, SrcMAC源MAC地址
// 只选择扩展标识符, 数据帧
#define  FRM_CAN_ID_INIT(SrcMAC, DstMAC) ((uint32)(LSHFT(FRM_ID_PRIO_Val | LSHFT((DstMAC),FRM_DST_MAC_BITn) | LSHFT((SrcMAC),FRM_SRC_MAC_BITn), FRM_ID_LShitBITn) | CAN_ID_EXT | CAN_RTR_DATA) )
// 参数, DstID:目的机台号, SrcID: 源机台号
#define  FRM_CAN_ID_INIT_2(SrcID, DstID) FRM_CAN_ID_INIT((DstID)+FRM_BASE_MAC, (SrcID)+FRM_MAC_BASE)
// 定义CAN标识符屏蔽寄存器,1:必须匹配, 0: 不用关心
// 固定两个参数: 扩展标识符, 数据帧
#define  FRM_CAN_ID_VARBITS	 	  ( (uint32)( LSHFT(~FRM_ID_TYPE_Msk, FRM_ID_LShitBITn) | CAN_TIxR_IDE | CAN_TIxR_RTR) ) /* 1:必须匹配; 0:不用关心 */
// 读取本机MAC地址
#define  GET_LOAL_MAC()			FRM_HOST_MAC			/* 主机为固定MAC,若为分机定义为相关读取函数 */

#define  FRM_MAX_BYTES			8						/* CAN最大发送字节数 */

/***************************************************************************************************/



/***************************************************************************************************/
// 类型定义
typedef uint8 T_FRM_Type;

typedef T_CanData t_frame;

__packed typedef struct uint24_
{
	uint8	u08[3];				/* uint24:最大值16,777,215 */
}uint24;

__packed typedef struct T_FrmPlayerErr_
{
	uint8		FrmID;			/* 帧序号 */
	uint8		ErrIndex;		/* 故障索引 */
	uint8		ErrTimes;		/* 故障次数 */
}T_FrmPlayerErr;

/***************************************************************************************************/
// 全局变量发布
#ifndef COMMUNI_GLOBALS

#endif		// end: #ifndef COMMUNI_GLOBALS

/***************************************************************************************************/
// APP应用层接口函数
// 发送CAN帧
COMMUNI_EXT t_err CAN_SendData(register uint8 , register T_FRM_Type , 
							T_CanData *, uint32 );
#define SendToPlayer(DstMachineIDIndex,frmaeType,pData,nBytes)	\
		CAN_SendData(DstMachineIDIndex,frmaeType,pData,nBytes)
// 读取CAN帧
COMMUNI_EXT t_err CAN_ReadFrame(T_CanFrame *pData);
#define ReceiveFromPlayer(pCanFrame)	CAN_ReadFrame(pCanFrame)
// 应用层函数
COMMUNI_EXT t_err SendWagerBeginMsg(void);/* 发送开始押分消息广播 */
COMMUNI_EXT t_err SendWagerEndMsg(void);	/* 发送结束押分消息广播 */
//COMMUNI_EXT t_err SendGameResultMsg(T_FrmGameRusult result);/* 发送结果码消息广播 */

// 因为在SWI服务中处理时main()过程已被中断,但是main()过程不能打断SWI所以CRITICAL()只在main()过程调用即可
// 注意不要在其它异步处理函数中操作g_PlayerWager变量否则需要更改以下_CRITICAL()操作
#define  WAGER_ENTER_CRITICAL()		OS_ENTER_CRITICAL()	//CAN1_SWI_Disable()
#define  WAGER_EXIT_CRITICAL()		OS_EXIT_CRITICAL()	//CAN1_SWI_Enable()

/***************************************************************************************************/
// 初始化函数, 被用于CAN初始化或者服务调用, 不被应用层代码调用
COMMUNI_EXT void CAN_FilterConfig(void);	
COMMUNI_EXT t_err CAN_QueueWriteFull(uint32 TxRx);
COMMUNI_EXT void CAN1_SCE_IRQHandler (void);
#define  CAN1_SWI_IRQHandler_EXT_EN			1	/* 定义该开关以使 */
COMMUNI_EXT void EXTI4_IRQHandler(void);

// 调试函数
COMMUNI_EXT t_err Dbg_CAN_SendData(uint8 SrcMac, register uint8 DstMac, register T_FRM_Type FrmType, 
							T_CanData *pData, uint32 nDataBytes);


#endif		// end: #ifndef __COMMUNI_H__ //
