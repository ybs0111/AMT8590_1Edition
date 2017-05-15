#if !defined(AFX_RUN_TOP_VISION_H__BF17ECDB_C054_4E56_83DD_70615B92AD44__INCLUDED_)
#define AFX_RUN_TOP_VISION_H__BF17ECDB_C054_4E56_83DD_70615B92AD44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Top_Vision.h : header file
//
#include "SrcPart/APartRobot.h"

enum ENUM_TOP_VISION_INIT
{
	ETVI_START,
	ETVI_CHECK_ROBOT_CHK,

	ETVI_HOMECHK_VISION_X,

	ETVI_FINISH,
};

/////////////////////////////////////////////////////////////////////////////
// CRun_Top_Vision command target

class CRun_Top_Vision : public CObject
{
public:
	DECLARE_SERIAL(CRun_Top_Vision)

	CRun_Top_Vision();           // protected constructor used by dynamic creation
	virtual ~CRun_Top_Vision();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		tvis_step_old;
	int		mn_RcvyStep;
	int		m_nInterfaceStep;

	int		m_nReq_Rcvy;
	int		m_continus_vis_error[2];
	int		m_topVision_skipCnt;
	int		m_nRetryMdlChg;
	char	cJamCode[10];

	DWORD	m_dwTimeInit;
	DWORD	m_dwTimeMove;
	DWORD	m_dwTimeRobot;
	DWORD	m_dwTimeReadBcr;
	DWORD	m_dwTimeCommVision[2];
	DWORD	m_dwTime_Recvy;
	DWORD	m_dwTimeTact;
	DWORD	m_dwTimeVision;
	DWORD	m_dwTimeChangeWork;

	int		mn_result_v[2];
	CString m_strBcrcode[2];

protected:
	bool Run_TopVision_Move_Chk_Jam( stJobPicker job );
	bool Run_TopVision_Move_Chk_Time();
	bool InTimeRobot( int iMilliSec );

	ECW m_cwTopviscomm[2];
	ENUM_COMM_STEP m_step_topviscomm[2];


// Operations
public:

	void ETVI_Robot_Check();
	void ETVI_HomeChk_VisionX();

	void ETVI_Finish();

	void CommTopVision( int nSite);
	void Run_Move_TopVision( int nSite );
	int	OnVisionModelChange(int port);

public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Reinstate();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();


	//vision
	int				m_resultTopvis[2];
	CString			m_strtopmdl[2];
	DWORD			m_dwTimeNetwork[2][3];
	int				m_nRetryCnt[2];
	CString			strJamCode;

	void	 SetTopVisionResult(int nSite, int nResult) { m_resultTopvis[nSite] = nResult; }
	int		 GetTopVisionResult(int nSite) { return m_resultTopvis[nSite]; }
	void	 SetTopVisionMdl(int nSite, CString strmdl) { m_strtopmdl[nSite] = strmdl; }
	CString	 GetTopVisionMdl(int nSite) { return m_strtopmdl[nSite]; }

};
extern CRun_Top_Vision g_Run_Top_Vision;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_TOP_VISION_H__BF17ECDB_C054_4E56_83DD_70615B92AD44__INCLUDED_)
