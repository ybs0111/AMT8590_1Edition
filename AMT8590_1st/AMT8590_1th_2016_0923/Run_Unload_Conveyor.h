#if !defined(AFX_RUN_UNLOAD_CONVEYOR_H__CCEBA703_8C7C_4A44_930C_260EFBC5C301__INCLUDED_)
#define AFX_RUN_UNLOAD_CONVEYOR_H__CCEBA703_8C7C_4A44_930C_260EFBC5C301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Unload_Conveyor.h : header file
//

enum ENUM_UNLOAD_CONVEYOR_INIT
{
	EUCI_START,
	EUCI_CHECK_ROBOT_CHK,
	EURI_HOMECHK_STACKER_RBT,
	EURI_HOMECHK_PITCH_RBT,
	EUCI_CHECK_DEVICE_CHK,
	EUCI_VACUUM_ON,
	EUCI_VACUUMON_CHK,
	EUCI_VACUUM_OFF,
	EUCI_RECYCLE_FWDBWD,
	EUCI_RECYCLE_FWDBWD_CHK,
	EUCI_CONVEYOR_DEVICE_CHK,


	EUCI_FINISH,
};

/////////////////////////////////////////////////////////////////////////////
// Run_Unload_Conveyor command target

class CRun_Unload_Conveyor : public CObject
{
public:
	DECLARE_SERIAL(CRun_Unload_Conveyor)

	CRun_Unload_Conveyor();           // protected constructor used by dynamic creation
	virtual ~CRun_Unload_Conveyor();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RcvyStep;
	int		mn_RestoreStep;
	int		mn_elv_conv_step;
	DWORD	m_dwTimeInit;
	DWORD	m_dwTimeElvPick;
	DWORD	m_dwTimeMove;
	int		mn_mdlnum;
	int		mn_mdlcnt;
	
	int		m_nReq_Rcvy;
	double m_dPos_TmpRcvyZ;

	int		tvis_step_old;

// Operations
protected:
	ENUM_STEP_CYL	m_step_recyclefwdbwd;
	EFB				m_fbRecyclefwdbwd;
	DWORD			m_dwTimeRecycleStart;
	DWORD			m_dwTimeRecycle;

public:
	void ReqRecycleFwdBwd( EFB efb );
	EFB GetRecycleFwdBwd() { return m_fbRecyclefwdbwd; }
	void SetRecycleFwdBwd( int iOnOff );
	bool CheckRecycleFwdBwd( int iOnOff );

	void EUCI_Check_Rbt_Chek();
	void EUCI_HomeCheck_Conveyor_Pitch_Rbt();
	void EUCI_HomeCheck_Conveyor_Stacker_Rbt();
	void EUCI_Decive_check();
	
	void EUCI_Finish();


	void SetVacuumOnOff(int iOnOff);
	int GetVacuumCheck();
	void Run_Recycle_FwdBwd();
	

public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Reinstate();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();
	void Run_ElvMove();
};
extern CRun_Unload_Conveyor g_Run_Unload_Conveyor;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_UNLOAD_CONVEYOR_H__CCEBA703_8C7C_4A44_930C_260EFBC5C301__INCLUDED_)
