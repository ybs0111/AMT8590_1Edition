// APartHandler.h: interface for the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
#define AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "APartTray.h"

#define ALINE	0
#define BLINE	1

enum ENUM_MACHINE_POSITION
{
	EMPOS_FRONT,
	EMPOS_REAR,
	EMPOS_ALONE,
};

enum ESTEP_CONV
{
	EC_NONE,

	EC_RECEIVE,
	EC_RECEIVE_CHECK,
	

	EC_SEND,
	EC_SEND_CHK,

	EC_ULD_RECEIVE,
	EC_ULD_RECEIVE_CHECK,
	EC_ULD_SEND,
	EC_ULD_SEND_CHK,
};

enum ESTEP_TTRANS
{
	ETT_TO_NONE,

	ETT_TO_REJECT,
	ETT_TO_REJECT_CHK,

	ETT_TO_BIN3,
	ETT_TO_BIN3_CHK,

	ETT_TRANSFER_TO_REJECT,
	ETT_TRANSFER_TO_REJECT_CHK,

	ETT_TRANSFER_TO_BIN3,
	ETT_TRANSFER_TO_BIN3_CHK,
};

enum ESTEP_ROBOT
{
	ER_NONE,

	ER_PICK_FROM_BIN,
	ER_PICK_FROM_BIN_CHK,

	ER_PICK_FROM_TSITE,
	ER_PICK_FROM_TSITE_CHK,

	ER_PLACE_TO_BIN,
	ER_PLACE_TO_BIN_CHK,

	ER_PLACE_TO_TSITE,
	ER_PLACE_TO_TSITE_CHK,
};

enum ESTEP_VIRTUAL_SUPPLY
{
	EVS_NONE,
	EVS_CALC,
	EVS_ADDTRAY,
	EVS_FINISH,
};

enum ESTEP_UNLOAD_ELV_ROBOT
{
	ESTEP_UNLOAD_ELV_NONE,
	ESTEP_UNLOAD_ELV_READY,
	ESTEP_ELV_READY,
	ESTEP_ELV_READY_CHK,
	ESTEP_ELV_MDL_CHECK,
};

class APartHandler  
{
public:
	APartHandler();
	virtual ~APartHandler();

	int m_step_conv_smema;
	int m_step_unloadrobot;
	int n_unload_elevator_req;
	int m_nReryCnt;
	void Thread_Run();
	void Run_Move_Main();
	void Run_Move_SMEMA();
	void Control_byTraySend();
	void Control_UnloadbyTraySend();
	void Control_byTrayRecv();
	void Control_UnloadbyTrayRecv();


	void Run_Move_Conv();
	void Run_Move_Conv(int nIdx);

	void Run_Move_Rear_Smema(int iId);
	void Run_Move_front_Smema();
	void Run_Move_btm_rear_Smema();


	void Run_Move_Virtual_Supply();
	//////////////////////////////////////////////////////////////////////////
	//int mn_run_status;  // 장비 동작 상태 정보 저장 변수 (O:STOP   1:RUN    2:ALARM    3:MAINT    4:INIT)

	void SetMachinePos( ENUM_MACHINE_POSITION empos ) { m_empos = empos; }
	ENUM_MACHINE_POSITION GetMachinePos() { return m_empos; }

	ETRAY_POS GetPriorityBin() { return m_epriority_bin; }
	void SetPriorityBin( ETRAY_POS epos ) { m_epriority_bin = epos; }

	void SetSecondaryBin( ETRAY_POS epos ) { m_esecondary_bin = epos; }
	ETRAY_POS GetSecondaryBin() { return m_esecondary_bin; }


	void ControlConv( int iIdx, ESTEP_CONV ec );
	double CalcTargetPosition( int nAxis, BOOL bPick, int nTarget, int nPickerIdx );
	ESTEP_CONV GetControlConv( int iIdx ) { return m_step_conv[iIdx]; }

	int GetVideo() { return m_nVedio; }
	
	int GetModuleCnt();
	void CheckLotEnd();

	void AddAlarmCnt();
	int GetAlarmCnt() { return m_nAlarmCnt; }
	void ClearAlarmCnt();


	bool CR_DensityforTest( std::vector<int> vecMustTestMdl );
	bool CR_Bcr1PickupTest( std::vector<int> vecMustTestMdl );
	bool CR_Bcr2PickupTest( std::vector<int> vecMustTestMdl );
	bool CR_Bcr1forTest( std::vector<int> vecMustTestMdl );
	bool CR_Bcr2forTest( std::vector<int> vecMustTestMdl );
	bool CR_TopVisforTest( std::vector<int> vecMustTestMdl );
	bool CR_UnloadforTest( std::vector<int> vecMustTestMdl );
	bool CR_UnloadforElv( std::vector<int> vecMustTestMdl );
//	bool CR_BcrFeederforTest( std::vector<int> vecMustTestMdl );
	bool CR_UnloadforConv( std::vector<int> vecGoodMdl, std::vector<int> vecRejEmptyMdl );
	bool CR_ConverforNext( std::vector<int> vecMustTestMdl );


public:
	void SetRejAlarm( bool bVal ) { m_bRejAlarm = bVal; }
	bool GetRejAlarm() { return m_bRejAlarm; }
	void ClearRejAlarm() { if( m_bRejAlarm ) m_bRejAlarm = false; }

	//ybs
	void SetReadySendNext( bool bVal, bool bEmpty=0, CString strLotid="" ) { m_bReady_SendNext = bVal; m_bEmptyTray_SendNext = bEmpty; m_strLotID_SendNext = strLotid;}
	bool GetReadySendNext() { return m_bReady_SendNext; }

	void SetRecvStateNextMachine( bool bVal ) { m_bRecv_NextMachine = bVal; }
	bool GetRecvStateNextMachine() { return m_bRecv_NextMachine; }

	ESTEP_TTRANS GetStepTTrans() { return m_step_ttrans; }
	void Run_Move_rear_Smema();
	void ReqUnloadElvRobot();
	void Unload_Elvatror_Check();
	bool GetUnloadElvatror_Check() { return (m_step_unloadrobot == ESTEP_UNLOAD_ELV_NONE); }

protected:
	void Control_byTray();

	void ControlDensityRobot();
	void ControlBarcode1Robot();
	void ControlBarcode2Robot();
	void ControlVisonRobot();
	void ControlUnloadRobot();
//	void ControlConveyorRobot();
	void ControlUnloadElvRobot();
	void ControlConvyorRobot();

	bool CR_FeederPickupTest( std::vector<int> vecMustTestMdl );
	void ControlBcrFeederRobot();


protected:
	ENUM_MACHINE_POSITION m_empos;	

	ESTEP_CONV				m_step_conv[4];//density (bcr1 bcr2 topvision unload, unload1, unload2)
	ESTEP_TTRANS			m_step_ttrans;

	ETRAY_POS				m_epriority_bin;
	ETRAY_POS				m_esecondary_bin;

	ESTEP_VIRTUAL_SUPPLY	m_step_virtual_supply;
	DWORD					m_dwTime_virtual_supply;

	int						m_nVedio;

	bool					m_bRejAlarm;

	bool					m_bReady_SendNext;
	//ybs
	bool					m_bEmptyTray_SendNext;
	CString					m_strLotID_SendNext;

	bool					m_bRecv_NextMachine;

	int						m_nAlarmCnt;

	DWORD					m_dwTime_front_Smema;
	DWORD					m_dwTime_rearSmema;
	DWORD					m_dwTime_btm_rearSmema;

	int						m_step_lotend_smema;
	DWORD					m_dwTime_LotendSmema;

public:
	int						m_step_front_smema;
	int						m_step_rear_smema;
	int						m_step_btm_rear_smema;

};

extern APartHandler	g_handler;

#endif // !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
