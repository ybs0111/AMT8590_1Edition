#if !defined(AFX_RUN_BTM_IDBUFFER_WORKS_H__7DE9C43B_08A5_4BF1_9E94_842D5524FB06__INCLUDED_)
#define AFX_RUN_BTM_IDBUFFER_WORKS_H__7DE9C43B_08A5_4BF1_9E94_842D5524FB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Btm_IDBuffer_Works.h : header file
//

enum ENUM_BTMID_ROBOT_INIT
{
	EBRI_START,
	EBRI_CHECK_ROBOT_CHK,
	EBRI_IDBUFFER_EMPTY_CHK,
	EBRI_IDBUFFER_HOOK_ULDCLAMP,
	EBRI_IDBUFFER_HOOK_ULDCLAMP_CHK,
	EBRI_HOMECHK_BTMID_ULD1,
	EBRI_MONE_SAFETY_UNLOAD1,
	EBRI_HOMECHK_BTMID_ULD2,
	EBRI_MONE_SAFETY_UNLOAD2,
	EBRI_IDBUFFER_ULDUNCLAMP,
	EBRI_IDBUFFER_ULDUNCLAMP_CHK,
	EBRI_IDBUFFER_ULDCLAMP,
	EBRI_IDBUFFER_ULDCLAMP_CHK,
	EBRI_IDBUFFER_ULDCLAMP_CHK_AGAIN,

	EBRI_FINISH,
};

enum ENUM_BTM_HOOK_SITE
{
	EBHS_HS_ULD1,
	EBHS_HS_ULD2,
};

enum ENUM_BTM_ULDCLAMP_SITE
{
	EBHS_HS_ULD12,
	EBHS_HS_ULD34,
};

enum ESTEP_BTM_IDBUFFER_RECV
{
	RECV_BTM_IDTRAY_NONE,
	RECV_BTM_IDTRAY_READY=10,
	RECV_BTM_IDTRAY_READY_CHK=20,
	RECV_BTM_IDTRAY_SAFETY=30,

	RECV_BTM_IDTRAY_RECV_READY=40,
	RECV_BTM_IDTRAY_RECV_CLAMP_OFF=50,
	RECV_BTM_IDTRAY_RECV_READY_CHK=60,
	RECV_BTM_IDTRAY_RECV_READY_POS=70,
	RECV_BTM_IDTRAY_CLAMP=80,
	RECV_BTM_IDTRAY_CLAMP_CHK=90,

	RECV_BTM_IDTRAY_MOVE=100,
	RECV_BTM_IDTRAY_LOAD_CLAMP=110,
	RECV_BTM_IDTRAY_LOAD_CLAMP_CHK=120,
	RECV_BTM_IDTRAY_CHECK=130,
	RECV_BTM_IDTRAY_FINISH=140,

};

enum ESTEP_BTM_IDBUFFER_SEND
{
	SEND_BTM_IDTRAY_NONE,
	SEND_BTM_IDTRAY_READY=10,
	SEND_BTM_IDTRAY_READY_CHK=20,
	SEND_BTM_IDTRAY_ULD2_SAFETY=30,
	SEND_BTM_IDTRAY_SEND_READY=40,
	RECV_BTM_IDTRAY_SEND_SGN_READY=50,
	RECV_BTM_IDTRAY_SEND_READY=60,

	RECV_BTM_IDTRAY_SEND_CLAMP_OFF=70,
	RECV_BTM_IDTRAY_SEND_CLAMP_OFF_CHK=80,
	RECV_BTM_IDTRAY_SEND_CLAMP_OFF_DELAY=90,
	RECV_BTM_IDTRAY_SEND_TRAY=100,

	RECV_BTM_IDTRAY_SEND_ULD2_READY=110,

	SEND_BTM_IDTRAY_ULD2_PICKMOVE_READY=120,
	SEND_BTM_IDTRAY_ULD2_PICKMOVE=130,
	SEND_BTM_IDTRAY_ULD2_FORWARD=140,
	SEND_BTM_IDTRAY_ULD2_FORWARD_CHK=150,
	SEND_BTM_IDTRAY_ULD1_FORWARD_CHK=160,
	SEND_BTM_IDTRAY_ULD2_ULDMOVE=170,
	SEND_BTM_IDTRAY_ULD2_ULDMOVE_READY=180,
	SEND_BTM_IDTRAY_ULD2_ULDMOVE_CHK=190,
	SEND_BTM_IDTRAY_ULD2_CLAMP_ON=200,
	SEND_BTM_IDTRAY_ULD2_CLAMP_ON_CHK=210,
	SEND_BTM_IDTRAY_ULD2_CLAMP_OFF=220,

	SEND_BTM_IDTRAY_MOVE_FINISH=230,
	SEND_BTM_IDTRAY_FINISH=240,
};
/////////////////////////////////////////////////////////////////////////////
// CRun_Btm_IDBuffer_Works command target

class CRun_Btm_IDBuffer_Works : public CObject
{
public:
	DECLARE_SERIAL(CRun_Btm_IDBuffer_Works)
	CRun_Btm_IDBuffer_Works();           // protected constructor used by dynamic creation
	virtual ~CRun_Btm_IDBuffer_Works();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		m_step_recvTray;
	int		m_step_sendTray;
	int		m_step_recvTray_old;
	int		m_step_sendTray_old;
	int		mn_InitRetry;

	DWORD	m_dwTimeInit;
	DWORD	m_dwTimeRecv;
	DWORD	m_dwTime_RecvTray;
	DWORD	m_dwTime_SendTray;


// Operations
protected:
	ENUM_STEP_CYL	m_step_stopperupdn;
	EFB				m_fbStopperUpdn;
	DWORD			m_dwTimeStopperUpdnStart;
	DWORD			m_dwTimeStopperUpdn;

	ENUM_STEP_CYL	m_step_hookfwdbwd[2];
	EFB				m_fbHookFwdBwd[2];
	DWORD			m_dwTimeHookFwdBwdStart[2];
	DWORD			m_dwTimeHookFwdBwd[2];

	ENUM_STEP_CYL	m_step_uldclamp[2];
	EFB				m_fbUldClamp[2];
	DWORD			m_dwTimeUldClampStart[2];
	DWORD			m_dwTimeUldClamp[2];



public:
	void ReqStopperUpdn( EFB efb );
	EFB GetStopperUpdn() { return m_fbStopperUpdn; }
	void SetStopperUpdn( int iOnOff );
	bool CheckStopperUpdn( int iOnOff );

	void ReqHookFwdBwd( int nSite, EFB efb );
	EFB GetHookFwdBwd(int nSite) { return m_fbHookFwdBwd[nSite]; }
	void SetHookFwdBwd(int nSite, int iOnOff );
	bool CheckHookFwdBwd(int nSite, int iOnOff );

	void ReqUldClamp( int nSite, EFB efb );
	EFB GetUldClamp(int nSite) { return m_fbUldClamp[nSite]; }
	void SetUldClamp(int nSite, int iOnOff );
	bool CheckUldClamp(int nSite, int iOnOff );
	
	void			ReqRecvTray();
	bool			GetRecvTrayNone() { return (m_step_recvTray == RECV_BTM_IDTRAY_NONE); }
	void			SetRecvTrayNone(ESTEP_BTM_IDBUFFER_RECV m_recv_tray) { m_step_recvTray = m_recv_tray; }

	void			ReqSendTray();
	bool			GetSendTrayNone() { return (m_step_sendTray == SEND_BTM_IDTRAY_NONE); }
	void			SetSendTrayNone(ESTEP_BTM_IDBUFFER_SEND m_send_tray) { m_step_sendTray = m_send_tray; }

	void			Run_Move_Btm_SendIDBuffer();
	void			Run_Move_Btm_RecvIDBuffer();


	


	void EBRI_Check_Robot();
	void EBRI_IDBuffer_Empty_Chk();
	void EBRI_IDBuffer_HOOK_UldClamp();
	void EBRI_IDBuffer_HOOK_UldClamp_Chk();
	void EBRI_HomeChk_ULD1();
	void EBRI_HomeChk_ULD2();
	void EBRI_Move_Safety_Unload1();
	void EBRI_Move_Safety_Unload2();
	void EBRI_IDBuffer_UldUnClamp();
	void EBRI_IDBuffer_UldUnClamp_Chk();
	void EBRI_IDBuffer_UldUnClamp_Chk_Again();
	void EBRI_IDBuffer_UldClamp();
	void EBRI_IDBuffer_UldClamp_Chk();
	void EBRI_Finish();



	void Run_Move_HookFwdBwd(int nSite);
	void Run_Move_UldClamp(int nSite);
	void Run_Move_StopperUpdn();

public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();

	int btm_idbuffer_sensor_flag_status_check(int n_mode, int n_check_site); //센서와 플레그가 틀리면 에러 
	void PreIDBufferBoxExist( EIDBUFFER_POS pos );
	void SetIDBufferBoxExist( EIDBUFFER_POS pos );

};
extern CRun_Btm_IDBuffer_Works g_Run_Btm_IDBuffers;
/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_RUN_BTM_IDBUFFER_WORKS_H__7DE9C43B_08A5_4BF1_9E94_842D5524FB06__INCLUDED_)
