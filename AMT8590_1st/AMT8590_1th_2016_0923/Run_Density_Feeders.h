#if !defined(AFX_RUN_DENSITY_FEEDERS_H__3604D700_7677_4328_848D_D9242F5171B3__INCLUDED_)
#define AFX_RUN_DENSITY_FEEDERS_H__3604D700_7677_4328_848D_D9242F5171B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Density_Feeders.h : header file
//

enum ENUM_DENSITY_FEEDER_INIT
{
	EDEI_START,
	EDEI_CHECK_ROBOT_CHK,
	EDEI_HOMECHK_VISION_X,
	EDEI_MOVE_SAFETY_X,
	EDEI_FEEDER_UNCYLINDER,
	EDEI_FEEDER_UNCYLINDER_CHK,
	EDEI_FEEDER_CYLINDER,
	EDEI_FEEDER_CYLINDER_CHK,
	EDEI_FEEDER_UNLOCK,
	EDEI_FEEDER_UNLOCK_CHK,
	EDEI_FEEDER_LOCK,
	EDEI_FEEDER_LOCK_CHK,

	EDEI_FINISH,
};

enum ENUM_DENSITY_POS
{
	EDP1=0,
	EDP2,
	EDP3,
	EDP4,
	EDP5,
	EDP6,
	MAX_EDP,
};

enum ENUM_BCR_VIS_POS
{
	EBVP_READY,
	EBVP_WAIT1,
	EBVP_WAIT2,
	EBVP_WAIT3,
	EBVP_WAIT4,
	EBVP_WAIT5,
};
/////////////////////////////////////////////////////////////////////////////
// CRun_Density_Feeders command target

class CRun_Density_Feeders : public CObject
{
public:
	DECLARE_SERIAL(CRun_Density_Feeders)
	CRun_Density_Feeders();           // protected constructor used by dynamic creation
	virtual ~CRun_Density_Feeders();

// Attributes
public:
	int		m_nRetryCnt[6];
	int		m_nRetryMdlChg;
	DWORD	m_dwTimeNetwork[6][3];
	int		mn_InitStep;
// 	int		mn_RunStep[6];
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		m_nRun_Print[6];
	DWORD	m_dwTimeInit;
	bool	m_bPrint_Pickup[6];
	int		m_nRetryPickOut[6];

	int		m_nLabelResult[6];
	DWORD	m_dwTimeOverDensity[6];
	int     m_nInterfaceStep;
	DWORD   m_dwTimeChangeWork;

	int		m_continus_vis_error[6];

// Operations
protected:
	ENUM_STEP_CYL	m_step_dfeeder[6];
	EFB				m_fbDFeeder[6];
	DWORD			m_dwTimeDFeederStart[6];
	DWORD			m_dwTimeDFeeder[6];

	ENUM_STEP_CYL	m_step_dlock[6];
	EFB				m_fbDlock[6];
	DWORD			m_dwTimeDlockStart[6];
	DWORD			m_dwTimeDlock[6];


	DWORD			m_dwTimeWait[6];

	int				mn_device_type;

	ENUM_COMM_STEP	 m_step_labrlcomm[6];
	ECW				 m_cwLabelcomm[6];
	DWORD			 m_dwTimeCommLabel[6];

	int				 m_resultFeeder[6];
	CString			 m_strlblCapa[6];
	CString			 m_strlblmdl[6];
	CString			 m_strl2D[6];
	char			 m_cAlarm[10];



public:
	void ReqFeeder( int nSite, EFB efb );
	EFB GetFeeder(int nSite) { return m_fbDFeeder[nSite]; }
	void SetFeeder(int nSite, int iOnOff );
	bool CheckFeeder(int nSite, int iOnOff );

	void ReqLock( int nSite, EFB efb );
	EFB GetLock( int nSite) { return m_fbDlock[nSite]; }
	void SetLock(int nSite, int iOnOff );
	bool CheckLock(int nSite, int iOnOff );

	void ReqFeeder( int nSite, CString strSerial );

	void CommLabel( int nSite);
	void Run_Move_Label( int nSite );

	void EDEI_Check_Rbt_Chk();
	void EDEI_HomeCheck_Vision_X();
	void EDEI_MoveSafety_X();
	void EDEI_Feeder_Cylinder( EFB efb );
	void EDEI_Feeder_Cylinder_Chk( EFB efb );
	void EDEI_Feeder_LockUnlock( EFB efb );
	void EDEI_Feeder_LockUnloack_Chk( EFB efb );
	void EDEI_Finish();

public:
// 	void Run_Move(int nSite);
	void Run_Move();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();
	void Shift_VisData( int nSite );
	
	int Run_DLabel_Print123( int nMotor );
	int Run_DLabel_Print456( int nMotor );
	void Run_Move_Feeder_Reel( int nSite );
	void Run_Move_Feeder_Lock( int nSite );
	int LoadSetNewLotParam(CString strLotID);
	int CheckLotLabelError();
	int GetDensity_LabelError(int nType);
	int	OnVisionModelChange(int port);

	void	 SetLabelResult(int nSite, int nResult) { m_resultFeeder[nSite] = nResult; }
	int		 GetLabelResult(int nSite) { return m_resultFeeder[nSite]; }
	void	 SetLabelCapa(int nSite, CString strCapa) { m_strlblCapa[nSite] = strCapa; }
	void	 SetLabel2D(int nSite, CString str2D) { m_strl2D[nSite] = str2D; }
	CString	 GetLabelCapa(int nSite) { return m_strlblCapa[nSite]; }
	void	 SetLabelMdl(int nSite, CString strmdl) { m_strlblmdl[nSite] = strmdl; }
	CString	 GetLabelMdl(int nSite) { return m_strlblmdl[nSite]; }
	CString	 GetLabel2D(int nSite) { return m_strl2D[nSite]; }

};
extern CRun_Density_Feeders g_Run_Density_Feeders;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_DENSITY_FEEDERS_H__3604D700_7677_4328_848D_D9242F5171B3__INCLUDED_)
