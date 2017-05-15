// Run_Handler_Check.h: interface for the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_HANDLER_CHECK_H__C7C7B3DE_3046_4089_8C04_0D4F36A1D947__INCLUDED_)
#define AFX_RUN_HANDLER_CHECK_H__C7C7B3DE_3046_4089_8C04_0D4F36A1D947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRun_Handler_Check  
{
public:
	CRun_Handler_Check();
	virtual ~CRun_Handler_Check();

// Attributes
public:
	int StartStep;
	int StopStep;
	int Recycle_zone_Step;//2014,1205
	int LoaderStep;
	int AlaemClearStep;
	int BuzzerStep;
	int BcrLockStep[4];
	int LabelLockStep[6];
	int LabelPaperLockStep[6];
	
	int MachineResetStep;
	
	int DoorStep;
	int AreaStep;
	
	long SwitchWaitTime[3];
	DWORD m_dwWaitBcrLock[4];
	DWORD m_dwWaitLabelLock[4];
	DWORD m_dwWaitLabelPaperLock[4];
	
	int n_emo_chk;
	int n_air_chk;

	int n_light_curtain_chk;

	char cJamcode[10];

	int n_front_key;
	int n_rear_key;
	int n_menu;
	int n_status;

	BOOL m_bJog_Up_old;
	BOOL m_bJog_Dn_old;

	int i_emo_check_flag;
	long EMO_CheckTime[3];
	
	////2015.0114
	int  m_nLockStep;
	long m_lLockWaitTime[3];
	int  m_nDoorOpenTry;
	int  m_nStep_Front_Auto;
	int  m_nStep_Front_Manual;
	long m_lWait_Auto[3];
	int  m_nStep_Rear_Manual;
	int  m_nStep_Rear_Auto;
	long m_lWait_Manual[3];
	////
	DWORD mdTimeXgem;

// Operations
public:
	void ButtonCheck_ManualAutoKey();
	void ButtonCheck_Alarm_Clear();
	void ButtonCheck_BuzzerOff();
	void ButtonCheck_Stop();
	void ButtonCheck_Start();
	void SafetyCheck_Door();
	void ButtonCheck_Recycle_Zone_Door();//2014,1205

	void ButtonCheck_LabelLockUnLock(int i);
	void ButtonCheck_LabelPaperLockUnLock(int i);
	void ButtonCheck_BarcodeLockUnLock( int i);
	void ButtonCheck_Z_Axis_Down();

	////2015.0114
	void OnRun_DoorLock();	// 모든 DOOR 닫힌 경우 강제로 DOOR LOCK 처리 
	void OnTurn_Front_Auto();		// [Auto] 상태로 스위치 돌린 경우 처리
	void OnTurn_Front_Manual();	// [Manual] 상태로 스위치 돌린 경우 처리
	void OnTurn_Rear_Auto();		// [Auto] 상태로 스위치 돌린 경우 처리
	void OnTurn_Rear_Manual();	// [Manual] 상태로 스위치 돌린 경우 처리
	////

//	void SafetyChk();
};
extern CRun_Handler_Check Run_Handler_Check;
extern CRun_Handler_Check Run_Handler_Check_Manual;
#endif // !defined(AFX_RUN_HANDLER_CHECK_H__C7C7B3DE_3046_4089_8C04_0D4F36A1D947__INCLUDED_)
