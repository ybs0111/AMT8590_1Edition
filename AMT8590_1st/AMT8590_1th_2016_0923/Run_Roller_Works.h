#if !defined(AFX_RUN_ROLLER_WORKS_H__DD9F592A_4F68_4966_92AD_0AADA27183F7__INCLUDED_)
#define AFX_RUN_ROLLER_WORKS_H__DD9F592A_4F68_4966_92AD_0AADA27183F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Roller_Works.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRun_Roller_Works command target

class CRun_Roller_Works : public CObject
{
public:
	DECLARE_SERIAL(CRun_Roller_Works)
	CRun_Roller_Works();           // protected constructor used by dynamic creation
	virtual ~CRun_Roller_Works();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		mn_mot_flag[2];
	DWORD	m_dwTimeInit;

// Operations

protected:
//	ENUM_STEP_CYL m_step_rolling_fwdbwd[3];
//	EFB			  m_fbrollingfwdbwd[3];
//	DWORD		  m_dwTimeRollingFwdBwdStart[3];
//	DWORD		  m_dwTimeRollingFwdBwd[3];
	ENUM_STEP_CYL m_step_rolling_fwdbwd[2][2]; //bcr1,bcr2 Left,right
	EFB			  m_fbrollingfwdbwd[2][2]; //bcr1,bcr2 Left,right
	DWORD		  m_dwTimeRollingFwdBwdStart[2][2]; //bcr1,bcr2 Left,right
	DWORD		  m_dwTimeRollingFwdBwd[2][2]; //bcr1,bcr2 Left,right
	
public:
//	void ReqRollingFwdBwd( int nSite, EFB efb);
//	EFB  GetRollingFwdBwd( int nSite ) { return m_fbrollingfwdbwd[nSite]; }
//	void SetRollingFwdBwd( int nSite, int iOnOff );
//	bool CheckRollingFwdBwd( int nSite, int iOnOff );
	void ReqRollingFwdBwd( int nSite, int pos, EFB efb);
	EFB  GetRollingFwdBwd( int nSite, int pos ) { return m_fbrollingfwdbwd[nSite][pos]; }
	void SetRollingFwdBwd( int nSite, int pos, int iOnOff );
	bool CheckRollingFwdBwd( int nSite, int pos, int iOnOff );


	void ERRI_Check_Rbt_Chk();
	void ERRI_Rolling_Backward();
	void ERRI_Rolling_Backward_Check();

	void ERRI_HomeChk_Rolling( int nAxis );

	void ERRI_Finish(); 

public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();

//	void Run_Move_Rolling_FwdBwd( int nSite);
	void Run_Move_Rolling_FwdBwd(int nSite, int nPos);


};
extern CRun_Roller_Works g_Run_Roller_Works;
/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_RUN_ROLLER_WORKS_H__DD9F592A_4F68_4966_92AD_0AADA27183F7__INCLUDED_)
