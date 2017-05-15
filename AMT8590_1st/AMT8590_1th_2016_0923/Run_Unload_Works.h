#if !defined(AFX_RUN_UNLOAD_WORKS_H__997B3CFB_FE14_4B55_8F8E_038DEA8724D3__INCLUDED_)
#define AFX_RUN_UNLOAD_WORKS_H__997B3CFB_FE14_4B55_8F8E_038DEA8724D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Unload_Works.h : header file
//
#include "SrcPart/APartRobot.h"

enum ENUM_UNLOAD_ROBOT_INIT
{
	EURI_START,
	EURI_CHECK_ROBOT_CHK,
	EURI_PUSHER_OFF,
	EURI_PUSHER_OFF_CHK,
	EURI_HOMECHK_RBT_Z,
	EURI_VACUUM_ON,
	EURI_UNLOAD_EMPTY_CHK,
	EURI_VACUUM_OFF,

	EURI_PICKER_OFF,
	EURI_PICKER_OFF_CHK,
	EURI_HOMECHK_RBT_Y,
	EURI_HOMECHK_BTM_VISION_Y,
	

	EURI_FINISH,
};

/////////////////////////////////////////////////////////////////////////////
// CRun_Unload_Works command target


class CRun_Unload_Works : public CObject
{
public:
	DECLARE_SERIAL(CRun_Unload_Works)

	CRun_Unload_Works();           // protected constructor used by dynamic creation
	virtual ~CRun_Unload_Works();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RcvyStep;
	int		mn_RestoreStep;
	int		bvis_step_old;
	DWORD	m_dwTimeInit;
	DWORD	m_dwTimeMove;
	DWORD	m_dwTimeRecvy;
	DWORD	m_dwTimeTact;
	int		mn_MotFlag[2];
	int		mn_server_step;
	int		m_nStep_MES;
	int		m_continus_vis_error[2];

	int		m_nReq_MES;

	int		m_nReq_Rcvy[3];
	double m_dPos_TmpRcvyZ;

	static int nPicker;
	int		mn_PickerInfo[D_INFOSIZE][2];
	double			m_zPos;
	int		mn_retry;
	double  m_dPos;		
	DWORD	m_dwTimeCommVision[2];
	char	cJamCode[10];

	std::vector<int> vecTact;
	
	
protected:
	ENUM_STEP_CYL	m_step_vac_down[2];
	EFB				m_fbVacDown[2];
	DWORD			m_dwTimeVacDown[2];
	DWORD			m_dwTimeVacDownStart[2];

	ENUM_STEP_CYL	m_step_pusherbuf_fwdbwd;
	EFB				m_fbBufPusfer;
	DWORD			m_dwTimeBufPusher;
	DWORD			m_dwTimeBufPusherStart;


	ECW				m_cwBtmviscomm[2];
	ENUM_COMM_STEP  m_step_btmviscomm[2];

	bool Run_Unload_Move_Chk_Jam(stJobPicker job);
	bool Run_Unload_Move_Chk_Time();

// Operations
public:
	bool mb_PickerUpDown_Flag[2];
	DWORD m_dwPickerUpDown_Wait[2][3];//[picker][time]


	void EURI_Check_Rbt_Chek();
	void EURI_Pusher_cyl_off();
	void EURI_Pusher_cyl_off_chk();
	void EURI_HomeCheck_Rbt_Z();
	void EURI_HomeCheck_Rbt_Y();
	void EURI_Mdl_Check_Rbt_Chek();
	void EURI_HomeCheck_BtmVision_X();
	void EURI_Vacuun_OnOff( int iOnOff );
//	void EURI_Picker_OnOff( int iOnOff );
	void EURI_Finish();
	void Set_PickerUpDown(int OnOff, int PickerInfo[4][2]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[4][2]);

	void ReqVacDown( int iIdx, EFB efb );
	EFB GetVacDown( int iIdx ) { return m_fbVacDown[iIdx]; }
	void Run_Move_Vac_Down( int iIdx );

	void ReqPusherBuf( EFB efb );
	EFB GetPusherBuf() { return m_fbBufPusfer; }
	void Run_Move_Pusher_Buffer();


	void CommBtmVision( int nSite);
	void Run_Move_BtmVision( int nSite );

	//vision
	int				m_resultBtmvis[2];
	CString			m_strbtmmdl[2];
	DWORD			m_dwTimeNetwork[2][3];
	int				m_nRetryCnt[2];
	CString			strJamCode;

	void	 SetBtmVisionResult(int nSite, int nResult) { m_resultBtmvis[nSite] = nResult; }
	int		 GetBtmVisionResult(int nSite) { return m_resultBtmvis[nSite]; }
	void	 SetBtmVisionMdl(int nSite, CString strmdl) { m_strbtmmdl[nSite] = strmdl; }
	CString	 GetBtmVisionMdl(int nSite) { return m_strbtmmdl[nSite]; }


public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Reinstate();
	void Run_Restore();
	void OnRobot_FinalPos();
	int OnServerSendBoatID();
	int OnTact(int nTact);

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();

	CString m_strbcr_idbuff[2];
	CString m_strbcr_serial[2];
	bool m_bUse_idbuff[2];
	CString m_strbcr_lotid[2];
	CString m_strRetry;
	CString m_strCnt;
	int m_nidRetryCnt;
	int m_nErrRetryCnt;
	int mn_pos;
	DWORD m_dwTime_Network;

	DWORD m_lWaitResp[3];
	int m_nRetryCount;
};
extern CRun_Unload_Works g_Run_Unload_Works;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_UNLOAD_WORKS_H__997B3CFB_FE14_4B55_8F8E_038DEA8724D3__INCLUDED_)

