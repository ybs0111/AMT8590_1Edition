#if !defined(AFX_RUN_TOP_IDBUFFER_WORKS_H__CA982DFB_9D86_4704_9321_DAD41259AAD7__INCLUDED_)
#define AFX_RUN_TOP_IDBUFFER_WORKS_H__CA982DFB_9D86_4704_9321_DAD41259AAD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Top_IDBuffer_Works.h : header file
//

enum ENUM_TOPID_ROBOT_INIT
{
	ETRI_START,
	ETRI_CHECK_ROBOT_CHK,
	ETRI_IDBUFFER_EMPTY_CHK,
	ETRI_IDBUFFER_HOOK_LDCLAMP,
	ETRI_IDBUFFER_HOOK_LDCLAMP_CHK,
	ETRI_IDBUFFER_LDFWDBWD,
	ETRI_IDBUFFER_LDFWDBWD_CHK,
	ETRI_HOMECHK_TOPID_LD1,
	ETRI_MONE_SAFETY_LOAD1,
	ETRI_HOMECHK_TOPID_LD2,
	ETRI_MONE_SAFETY_LOAD2,

	ETRI_IDBUFFER_LDUNCLAMP,
	ETRI_IDBUFFER_LDUNCLAMP_CHK,
	ETRI_IDBUFFER_LDCLAMP,
	ETRI_IDBUFFER_LDCLAMP_CHK,
	ETRI_IDBUFFER_LDCLAMP_CHK_AGAIN,

	ETRI_FINISH,
};

enum ENUM_HOOK_SITE
{
	EHS_HS_LD1,
	EHS_HS_LD2_LEFT,
	EHS_HS_LD2_RIGHT,
};

enum ESTEP_IDBUFFER_RECV
{
	RECV_IDTRAY_NONE,
	RECV_IDTRAY_READY,
	RECV_IDTRAY_READY_CHK,
	RECV_IDTRAY_HOOK_ON,
	RECV_IDTRAY_HOOK_ON_CHK,
	
	RECV_IDTRAY_FRONT_ON,
	RECV_IDTRAY_FRONT_ON_CHK,
	RECV_IDTRAY_BUFFER_IN_CHK,
	RECV_IDTRAY_LOT_CHK,

	RECV_IDTRAY_CLAMP,
	RECV_IDTRAY_CLAMP_CHK,

	RECV_IDTRAY_MOVE,
	RECV_IDTRAY_LOAD_CLAMP,
	RECV_IDTRAY_MOVED,
	RECV_IDTRAY_BACR1,
	RECV_IDTRAY_BACR1_READ,
	RECV_IDTRAY_BACR2,
	RECV_IDTRAY_BACR2_READ,
	RECV_IDTRAY_LOAD_BCR_COMPLETED,
	RECV_IDTRAY_LOAD_CLAMP_CHK,
	RECV_IDTRAY_LOAD_READY,
	RECV_IDTRAY_LOAD_READY_CHK,
	RECV_IDTRAY_LOAD_READY_CLAMP,
	RECV_IDTRAY_LOAD_READY_END,

	RECV_IDTRAY_BACRCODE,
	RECV_IDTRAY_FINISH,
};

enum ESTEP_IDBUFFER_SEND
{
	SEND_IDTRAY_NONE,

	SEND_IDTRAY_READY,
	SEND_IDTRAY_READY_CHK,
	SEND_IDTRAY_LD2_FORWARD,
	SEND_IDTRAY_LD2_FORWARD_CHK,
	SEND_IDTRAY_LD2_CLAMP,
	SEND_IDTRAY_LD2_CLAMP_CHK,
	SEND_IDTRAY_LD_CLAMP_OFF,
	SEND_IDTRAY_LD_CLAMP_OFF_CHK,

	SEND_IDTRAY_EL_READY_CHK,
	SEND_IDTRAY_EL_CHK,
	SEND_IDTRAY_EL_MOVE,

	SEND_IDTRAY_LD2_MOVE,
	SEND_IDTRAY_LD2_MOVE_CHK,
	SEND_IDTRAY_LD_CLAMP_ON,
	SEND_IDTRAY_LD_CLAMP_ON_CHK,
	SEND_IDTRAY_LD2_CLAMP_OFF,
	SEND_IDTRAY_LD2_CLAMP_OFF_CHK,

	SEND_IDTRAY_FINISH,
};

/////////////////////////////////////////////////////////////////////////////
// CRun_Top_IDBuffer_Works command target

class CRun_Top_IDBuffer_Works : public CObject
{
public:
	DECLARE_SERIAL(CRun_Top_IDBuffer_Works)

	CRun_Top_IDBuffer_Works();           // protected constructor used by dynamic creation
	virtual ~CRun_Top_IDBuffer_Works();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		mn_RcvyStep;
	int		mn_RetryCnt;
	DWORD	m_dwTimeInit;
	DWORD	m_dwTimeTopIDBuffer;

	int		m_nReq_Rcvy;

	int		m_nMotID;
	int		mn_GetBcrFromServer[2];//서버통신 OK인지 안닌지
	char	mc_jamcode[10];
	
protected:
	ESTEP_IDBUFFER_RECV	m_step_recvTray_old;
	ESTEP_IDBUFFER_RECV	m_step_recvTray;
	ESTEP_IDBUFFER_SEND	m_step_sendTray_old;
	ESTEP_IDBUFFER_SEND	m_step_sendTray;
	DWORD			m_dwTime_RecvTray;
	DWORD			m_dwTime_SendTray;

	ENUM_STEP_CYL	m_step_hookclamp[HOOKCLAMP_CNT];
	EFB				m_fbHookClamp[HOOKCLAMP_CNT];
	DWORD			m_dwTimeHookClampStart[HOOKCLAMP_CNT];
	DWORD			m_dwTimeHookClamp[HOOKCLAMP_CNT];

	ENUM_STEP_CYL	m_step_hookfwdbwd[2];
	EFB				m_fbHookhwdbwd[2];
	DWORD			m_dwTimeHookHwdBwdStart[2];
	DWORD			m_dwTimeHookHwdBwd[2];

	ENUM_STEP_CYL	m_step_ldclamp[LDCLAMP_CNT];
	EFB				m_fbLdClamp[LDCLAMP_CNT];
	DWORD			m_dwTimeLdClampStart[LDCLAMP_CNT];
	DWORD			m_dwTimeLdClamp[LDCLAMP_CNT];



// Operations
public:
	void ReqHookClamp( int nSite, EFB efb );//LD1, LD2_Left, LD2_Right
	EFB GetHookClamp(int nSite) { return m_fbHookClamp[nSite]; }
	void SetHookClamp(int nSite, int iOnOff );
	bool CheckHookClamp(int nSite, int iOnOff );

	void ReqHookFwdBwd( int nSite, EFB efb );
	EFB GetHookFwdBwd(int nSite) { return m_fbHookhwdbwd[nSite]; }
	void SetHookFwdBwd(int nSite, int iOnOff );
	bool CheckHookFwdBwd(int nSite, int iOnOff );

	void ReqLdClamp( int nSite, EFB efb );
	EFB GetLdClamp(int nSite) { return m_fbLdClamp[nSite]; }
	void SetLdClamp(int nSite, int iOnOff );
	bool CheckLdClamp(int nSite, int iOnOff );


	void ReqRecvTray();
	bool GetRecvTrayNone() { return (m_step_recvTray == RECV_IDTRAY_NONE); }
	void SetRecvTrayNone(ESTEP_IDBUFFER_RECV m_recv_tray) { m_step_recvTray = m_recv_tray; }
	void Run_Move_RecvIDBuffer();
	void Run_Move_SendIDBuffer();

	void ReqSendTray();
	bool GetSendTrayNone() { return (m_step_sendTray == SEND_IDTRAY_NONE); }
	void SetSendTrayNone(ESTEP_IDBUFFER_SEND m_send_tray) { m_step_sendTray = m_send_tray; }

	void Run_Move_HookClamp(int nSite);
	void Run_Move_HookFwdBwd(int nSite);
	void Run_Move_LdClamp(int nSite);


public:
	//initialize
	void ETRI_Check_Robot();
	void ETRI_IDBuffer_Empty_Chk();
	void ETRI_IDBuffer_HOOK_LdClamp();
	void ETRI_IDBuffer_HOOK_LdClamp_Chk();
	void ETRI_IDBuffer_FwdBwd();
	void ETRI_IDBuffer_FwdBwd_Chk();
	void ETRI_HomeChk_LD1();
	void ETRI_Move_Safety_Load1();
	void ETRI_HomeChk_LD2();
	void ETRI_Move_Safety_Load2();

	void ETRI_IDBuffer_LdUnClamp();
	void ETRI_IDBuffer_LdUnClamp_Chk();

	void ETRI_IDBuffer_LdClamp();
	void ETRI_IDBuffer_LdClamp_Chk();
	void ETRI_IDBuffer_LdClamp_Chk_Again();

	void ETRI_Finish();



	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Reinstate();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();

	int top_idbuffer_sensor_flag_status_check(int n_mode, int n_check_site); //센서와 플레그가 틀리면 에러 
	void PreIDBufferBoxExist();
	void SetIDBufferBoxExist();
	void RecvShiftData();
	void TempSendShiftData();
	void MoveTempShiftData();

	void RecvDataConvert();
	void SendDataConvert();
	void InPutConvertModuleData( EIDBUFFER_POS nPos);

};
extern CRun_Top_IDBuffer_Works g_Run_Top_IDBuffers;
/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_RUN_TOP_IDBUFFER_WORKS_H__CA982DFB_9D86_4704_9321_DAD41259AAD7__INCLUDED_)

