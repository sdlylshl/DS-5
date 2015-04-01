/*******************************************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	Communi.H
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-03-16 15:56
  
  Description:   ���ֻ�ͨ����غ���˵����ȫ����˵��
  Others:         
  Function List: 
  	1. ��
  	2. ��
  	3. ��
  	4. ��
  	5. ��

  History:        // �޸ļ�¼
    1. Date:		2011-03-16 15:56
       Author:		Xsky
       Modification:	������ģ��
**------------------------------------------------------------------------------------------------------
** ����޸�����: 2011-04-13 14:45
** �޸���: Xsky
** �汾: v1.0.00
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** ����:  2011-04-13 14:45
** �汾: v1.0.00
** �޸�: 
**			1. ���Ӷ���FRM_ID_LShitBITn���������
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

#define  CAN_FILTER_N		MAX_PLAYER	/* CAN������ʹ������Ϊ������� */

/***************************************************************************************************/
// ��������
//#define  FRM_ID_PRIO_Msk		( BIT(28)| BIT(27) | BIT(26) | BIT(25) | BIT(24) )
#define  FRM_ID_PRIO_Val		((uint32)0x01 << 24)	/* ���������������ȼ����� */
#define  FRM_BASE_MAC			((uint32)0x10)			/* MAC����ַ */
#define  FRM_HOST_MAC			((uint32)0x10)			/* ����MAC */
#define  FRM_PLAYER_MAC_BASE	((uint32)0x11)			/* �ֻ�MAC����ַ */
#define  FRM_BROADCAST_MAC		((uint32)0x3F)			/* �㲥��ַ */
#define  FRM_SRC_MAC_BITn		16						/* ԴMACλ�� */
#define  FRM_SRC_MAC_Msk		0x3F
#define  FRM_DST_MAC_BITn		8						/* Ŀ��MACλ�� */
#define  FRM_DST_MAC_Msk		0x3F
#define  FRM_ID_TYPE_BITnH		6						/* ֡����/������λ�����λ */
#define  FRM_ID_TYPE_BITnL		0						/* ֡����/������λ�����λ */
#define  FRM_ID_TYPE_Msk		((uint32)0x7F)			/* ֡�������� */
#define  FRM_ID_LShitBITn		3						/* CAN���ͼĴ���ID����λ��,Add @2011-04-13 14:45 */
#define  FRM_MAC_BROADCAST		0x36					/* �㲥��ַ */
// ��������ָ��MACԴ��ַ�ı�ʶ��Ĭ��ֵ���캯��
// ����: DstMAC:Ŀ��MAC��ַ, SrcMACԴMAC��ַ
// ֻѡ����չ��ʶ��, ����֡
#define  FRM_CAN_ID_INIT(SrcMAC, DstMAC) ((uint32)(LSHFT(FRM_ID_PRIO_Val | LSHFT((DstMAC),FRM_DST_MAC_BITn) | LSHFT((SrcMAC),FRM_SRC_MAC_BITn), FRM_ID_LShitBITn) | CAN_ID_EXT | CAN_RTR_DATA) )
// ����, DstID:Ŀ�Ļ�̨��, SrcID: Դ��̨��
#define  FRM_CAN_ID_INIT_2(SrcID, DstID) FRM_CAN_ID_INIT((DstID)+FRM_BASE_MAC, (SrcID)+FRM_MAC_BASE)
// ����CAN��ʶ�����μĴ���,1:����ƥ��, 0: ���ù���
// �̶���������: ��չ��ʶ��, ����֡
#define  FRM_CAN_ID_VARBITS	 	  ( (uint32)( LSHFT(~FRM_ID_TYPE_Msk, FRM_ID_LShitBITn) | CAN_TIxR_IDE | CAN_TIxR_RTR) ) /* 1:����ƥ��; 0:���ù��� */
// ��ȡ����MAC��ַ
#define  GET_LOAL_MAC()			FRM_HOST_MAC			/* ����Ϊ�̶�MAC,��Ϊ�ֻ�����Ϊ��ض�ȡ���� */

#define  FRM_MAX_BYTES			8						/* CAN������ֽ��� */

/***************************************************************************************************/



/***************************************************************************************************/
// ���Ͷ���
typedef uint8 T_FRM_Type;

typedef T_CanData t_frame;

__packed typedef struct uint24_
{
	uint8	u08[3];				/* uint24:���ֵ16,777,215 */
}uint24;

__packed typedef struct T_FrmPlayerErr_
{
	uint8		FrmID;			/* ֡��� */
	uint8		ErrIndex;		/* �������� */
	uint8		ErrTimes;		/* ���ϴ��� */
}T_FrmPlayerErr;

/***************************************************************************************************/
// ȫ�ֱ�������
#ifndef COMMUNI_GLOBALS

#endif		// end: #ifndef COMMUNI_GLOBALS

/***************************************************************************************************/
// APPӦ�ò�ӿں���
// ����CAN֡
COMMUNI_EXT t_err CAN_SendData(register uint8 , register T_FRM_Type , 
							T_CanData *, uint32 );
#define SendToPlayer(DstMachineIDIndex,frmaeType,pData,nBytes)	\
		CAN_SendData(DstMachineIDIndex,frmaeType,pData,nBytes)
// ��ȡCAN֡
COMMUNI_EXT t_err CAN_ReadFrame(T_CanFrame *pData);
#define ReceiveFromPlayer(pCanFrame)	CAN_ReadFrame(pCanFrame)
// Ӧ�ò㺯��
COMMUNI_EXT t_err SendWagerBeginMsg(void);/* ���Ϳ�ʼѺ����Ϣ�㲥 */
COMMUNI_EXT t_err SendWagerEndMsg(void);	/* ���ͽ���Ѻ����Ϣ�㲥 */
//COMMUNI_EXT t_err SendGameResultMsg(T_FrmGameRusult result);/* ���ͽ������Ϣ�㲥 */

// ��Ϊ��SWI�����д���ʱmain()�����ѱ��ж�,����main()���̲��ܴ��SWI����CRITICAL()ֻ��main()���̵��ü���
// ע�ⲻҪ�������첽���������в���g_PlayerWager����������Ҫ��������_CRITICAL()����
#define  WAGER_ENTER_CRITICAL()		OS_ENTER_CRITICAL()	//CAN1_SWI_Disable()
#define  WAGER_EXIT_CRITICAL()		OS_EXIT_CRITICAL()	//CAN1_SWI_Enable()

/***************************************************************************************************/
// ��ʼ������, ������CAN��ʼ�����߷������, ����Ӧ�ò�������
COMMUNI_EXT void CAN_FilterConfig(void);	
COMMUNI_EXT t_err CAN_QueueWriteFull(uint32 TxRx);
COMMUNI_EXT void CAN1_SCE_IRQHandler (void);
#define  CAN1_SWI_IRQHandler_EXT_EN			1	/* ����ÿ�����ʹ */
COMMUNI_EXT void EXTI4_IRQHandler(void);

// ���Ժ���
COMMUNI_EXT t_err Dbg_CAN_SendData(uint8 SrcMac, register uint8 DstMac, register T_FRM_Type FrmType, 
							T_CanData *pData, uint32 nDataBytes);


#endif		// end: #ifndef __COMMUNI_H__ //