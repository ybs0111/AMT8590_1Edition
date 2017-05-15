#if !defined(AFX_RUN_DENSITY_WORKS_H__38C21D46_1E5D_4825_894C_ACE5E082F333__INCLUDED_)
#define AFX_RUN_DENSITY_WORKS_H__38C21D46_1E5D_4825_894C_ACE5E082F333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Density_Works.h : header file
//
#include "SrcPart/APartRobot.h"

enum ENUM_DENSITY_ROBOT_INIT
{
	EDRI_START,
	EDRI_CHECK_ROBOT_CHK,
	EDRI_HOMECHK_DRBT_Z1,
	EDRI_HOMECHK_DRBT_Z2,
	EDRI_HOMECHK_DRBT_X,
	EDRI_HOMECHK_DRBT_Y,
	EDRI_VACUUM_FOR_REJECT,
	EDRI_LABEL_CHK,
	EDRI_LABEL_REJECT,
	EDRI_VACUUM_REJECT_FOR_PICK,
	EDRI_VACUUM_REJECT_FOR_PLACE,

	EDRI_VACUUM_ON,
	EDRI_LABEL_EMPTY_CHK,
	EDRI_VACUUM_OFF,
	EDRI_HOMECHK_GUIDE,
	EDRI_GUIDE_MOVE_SAFETY,	

	EDRI_FINISH,
};

enum ENUM_DENSITY_ROBOT_RESTORE
{
	EDRR_START,
	EDRR_Z_SAFETY,
	EDRR_VACUUM_CHK,
	EDRR_MOVE_ERR,
	EDRR_Z_DOWN,
	EDRR_DOWN_VACUUM_OFF,
	EDRR_DOWN_VACUUM_OFF_CHK,
	EDRR_Z_UP,
	EDRR_Z_UP_CHK,
	EDRR_VACUUM_OFF,	
	EDRR_FINISH,
};
/////////////////////////////////////////////////////////////////////////////
// CRun_Density_Works command target

class CRun_Density_Works : public CObject
{
public:
	DECLARE_SERIAL(CRun_Density_Works)
	CRun_Density_Works();           // protected constructor used by dynamic creation
	virtual ~CRun_Density_Works();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		mn_RcvyStep;

	int		mn_PickStep;
	int		mn_init_rej;
	int		mn_init_pick_rej;

	int		feeder_step_old;
	DWORD	m_dwTimeInit;
	DWORD	m_dwTime_Robot;
	DWORD	m_dwTimeLabel[6];

	int		mn_RetryCnt;
	int     mn_pickderror[2];
	int		mn_error_flag;

	int		mn_MotFlag[2];

	DWORD	m_dwTime_Pick;
	DWORD	m_dwTime_move;
	DWORD	m_dwTimeTact;
	double  m_dPosDown;
	double  m_dRejectPos;

private:
	int  m_nReq_Rcvy[4];
	double m_dPos_TmpRcvyZ1;
	double m_dPos_TmpRcvyZ2;


// Operations
protected:
	RDPP	m_dpDensity;


	bool Run_Density_Move_Chk_Jam( stJobPicker job );
	bool Run_Density_Move_Chk_Time();
	bool InTimeRobot( int iMilliSec );

public:
	void EDRI_Check_Rbt_Chk();
	void EDRI_HomeCheck_Z(int nAxis);
	void EDRI_HomeCheck_Guide(int nAxis);
	void EDRI_HomeCheck_XY(int nAxis);
	void EDRI_Label_Empty_Check();
	void EDRI_Vacuum_OnOff(int iOnOff);
	void EDRI_Error_Pick_XY();
	void EDRI_Error_Place_XY();
	void EDRI_Vacuum_OnOff_For_Reject(int iOnOff);
	void EDRI_Label_Check();
	int EDRI_Label_For_Reject();
	int EDRI_Label_Reject_For_Pickup();


	void EDRI_Error_Place_Off();
	void EDRI_Finish();

	void ReqDensityPick(RDPP rdp);
	bool GetDensityPick() { return (m_dpDensity == RDP_NONE); }
	void SetDenSityPick(RDPP rdp) { m_dpDensity = rdp; }	

	bool Chk_Robot_Z1_Safety();
	bool Chk_Robot_Z2_Safety();

public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Reinstate();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();


	void Robot_Density_Pickup();

};
extern CRun_Density_Works g_Run_Density_Works;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_DENSITY_WORKS_H__38C21D46_1E5D_4825_894C_ACE5E082F333__INCLUDED_)
