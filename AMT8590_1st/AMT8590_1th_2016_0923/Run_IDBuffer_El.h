#if !defined(AFX_RUN_IDBUFFER_EL_H__6B5501C7_F12D_4EE5_AB0F_2CF58C8BC9D6__INCLUDED_)
#define AFX_RUN_IDBUFFER_EL_H__6B5501C7_F12D_4EE5_AB0F_2CF58C8BC9D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_IDBuffer_El.h : header file
//

enum ENUM_IDBUFFER_EL_INIT
{
	EIEI_START,
	EIEI_CHECK_ROBOT_CHK,
	EIEI_IDBUFFER_CHK,
	EIEI_IDBUFFER_UNCLAMP,
	EIEI_IDBUFFER_UNCLAMP_CHK,
	EIEI_IDBUFFER_CLAMP,
	EIEI_IDBUFFER_CLAMP_CHK,
	EIEI_IDBUFFER_CLAMP_AGAIN_CHK,
	EIEI_HOMECHK_ELV,
	EIEI_MONE_SAFETY_ELV,


	EIEI_FINISH,
};

enum ESTEP_IDBUFFER_READYRECV
{
	READYRECV_IDTRAY_NONE,
	READYRECV_IDTRAY_READY,
	READYRECV_IDTRAY_READY_CHK,


	READYRECV_IDTRAY_FINISH,
};

enum ESTEP_IDBUFFER_DONERECV
{
	DONERECV_IDTRAY_NONE,
	DONERECV_IDTRAY_READY,
	DONERECV_IDTRAY_MOVE,
	DONERECV_IDTRAY_MOVE_DELAY,
	DONERECV_IDTRAY_READY_CHK,


	DONERECV_IDTRAY_FINISH,
};

enum ESTEP_ELIDBUFFER_SEND
{
	SEND_ELIDTRAY_NONE,

	SEND_ELIDTRAY_READY,
	SEND_ELIDTRAY_READY_CHK,
	SEND_ELIDTRAY_LD2_FORWARD,
	SEND_ELIDTRAY_LD2_FORWARD_CHK,
	SEND_ELIDTRAY_LD2_CLAMP,
	SEND_ELIDTRAY_LD2_CLAMP_CHK,
	SEND_ELIDTRAY_LD_CLAMP_OFF,
	SEND_ELIDTRAY_LD_CLAMP_OFF_CHK,
	SEND_ELIDTRAY_LD2_MOVE,
	SEND_ELIDTRAY_LD2_MOVE_CHK,
	SEND_ELIDTRAY_LD_CLAMP_ON,
	SEND_ELIDTRAY_LD_CLAMP_ON_CHK,
	SEND_ELIDTRAY_LD2_CLAMP_OFF,
	SEND_ELIDTRAY_LD2_CLAMP_OFF_CHK,

	SEND_ELIDTRAY_FINISH,
};

enum ESTEP_ELVIDBUFFER_RECV
{
	ELV_IDTRAY_NONE,
	ELV_IDTRAY_READY,
	ELV_IDTRAY_READY_CHK,
	ELV_IDTRAY_ELV_READY_CHK,
	ELV_IDTRAY_ELV_MOVE,
	SEND_IDTRAY_ELV_CLAMP_ON,
	ELV_IDTRAY_ELV_CLAMP_ON,
	ELV_IDTRAY_ELV_CLAMP_ON_CHK,
	ELV_IDTRAY_FINISH,
};


/////////////////////////////////////////////////////////////////////////////
// CRun_IDBuffer_El command target


class CRun_IDBuffer_El : public CObject
{
public:
	DECLARE_SERIAL(CRun_IDBuffer_El)
	CRun_IDBuffer_El();           // protected constructor used by dynamic creation
	virtual ~CRun_IDBuffer_El();

// Attributes
public:
	int		mn_InitStep;
	int		mn_RunStep;
	int		mn_RestoreStep;
	int		m_step_sendTray;
	int		elv_step_old;
	int		m_step_readyElv;
	int		m_step_readyElv_old;

	DWORD	m_dwTimeInit;
	DWORD	m_dwTimeRobot;
	DWORD	m_dwTimeMove;
	DWORD	m_dwTimeReadyElvClamp;
	DWORD	m_dwTime_ElvTray;




protected:
	ENUM_STEP_CYL	m_step_elvclamp;
	EFB				m_fbElvClamp;
	DWORD			m_dwTimeElvClampStart;
	DWORD			m_dwTimeElvClamp;

	int m_step_recvReadyTray;
	int m_step_recvReadyTray_old;
	int m_step_recvDoneTray;
	int m_step_recvDoneTray_old;

public:
	ECW				m_fbElvMoveReady;
	ENUM_STEP_CYL	m_step_elvmove;
	

public:	
	void ReqElvClamp( EFB efb );
	EFB GetElvClamp() { return m_fbElvClamp; }
	void SetElvClamp( int iOnOff );
	bool CheckElvClamp( int iOnOff );
	
	void EIEI_Check_Robot();
	void EIEI_Check_IDBufer();
	void EIEI_IDBuffer_UnClamp();
	void EIEI_IDBuffer_UnClamp_Chk();
	void EIEI_IDBuffer_Clamp();
	void EIEI_IDBuffer_Clamp_Chk();
	void EIEI_IDBuffer_Clamp_Again_Chk();
	void EIEI_HomeChk_Elv();
	void EIEI_Move_Safety_Elv();
	void EIEI_Finish();

	bool GetRecvReadyTrayNone() { return (m_step_recvReadyTray == READYRECV_IDTRAY_NONE); }
	void ReqRecvReadyTray();
	void Run_Move_RecvReadyIDBuffer();
	bool GetRecvDoneTrayNone() { return (m_step_recvDoneTray == DONERECV_IDTRAY_NONE); }
	void ReqRecvDoneTray();
	void Run_Move_RecvDoneIDBuffer();
//	bool IsBufferExist(EIDBUFFER_POS nSite);
	bool IsBufferExist();


public:
	void Serialize(CArchive &ar);
	void Run_Move();
	void Run_Restore();
	void OnRobot_FinalPos();

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();


	void Run_Move_ElvClamp();
	void ReqElvMoveReady(ECW ecw);
	void Run_Move_Elv_Ready();

	void Run_Move_SendIDBuffer();

	void ReqReadyElv();
	void Run_Move_ELvReady_IDBuffer();
	bool GetElvReady_IDBuffr() { return (m_step_readyElv == ELV_IDTRAY_NONE); }

};
extern CRun_IDBuffer_El g_Run_IDBuffers_El;
/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_RUN_IDBUFFER_EL_H__6B5501C7_F12D_4EE5_AB0F_2CF58C8BC9D6__INCLUDED_)
