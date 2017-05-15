// BcrEvent.h: interface for the CBcrEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCREVENT_H__E680F49C_DABB_449D_B535_4524DED94AB5__INCLUDED_)
#define AFX_BCREVENT_H__E680F49C_DABB_449D_B535_4524DED94AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"

#define ID_BUFF_CNT_		2

class CBcrEvent  
{
public:
	CBcrEvent();
	virtual ~CBcrEvent();

// Attributes
private:
	CString m_sLogMsg;
	CString m_sRcvData;

	CString m_sRcvedData[MAX_PORT];  // 수신 데이터 임시 저장

	// 시리얼 통신 요청 플래그
	int  m_nReq_LSSD;
	int  m_nReq_RSSD;
	int  m_nReq_TRIN;
	int  m_nReq_TROUT;

	int m_nPos_TRIN;
	int m_nPos_TROUT;
	
// Attributes
public:
	bool m_bflag_INIT;

	int  m_nStep_LSSD;
	int  m_nStep_RSSD;
	int  m_nStep_TRIN;
	int  m_nStep_TROUT;

	long m_lWait_LSSD[3];
	long m_lWait_RSSD[3];
	long m_lWait_TRIN[3];
	long m_lWait_TROUT[3];
	
// Operations
private:
	int  OnRun_LSSD();
	int  OnRun_RSSD();
	int  OnRun_TRIN();
	int  OnRun_TROUT();
	
// Operations
public:
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_INIT(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	void OnRcvDataFromBCR(WPARAM ch, LPARAM port);	// [EQP<-BCR] 바코드 리더기로부터 메시지 수신
	void OnSet_2Struct(int nzPort);	// 구조체에 수신된 메시지 저장

	// RFID 읽기 명령어 전송하도록 플래그 설정
	void OnReq_ReadLSSD()	{ m_nReq_LSSD	= REQ_CALL_; }
	void OnReq_ReadRSSD()	{ m_nReq_RSSD	= REQ_CALL_; }
	void OnReq_ReadTRIN()	{ m_nReq_TRIN	= REQ_CALL_; }
	void OnReq_ReadTROUT()	{ m_nReq_TROUT	= REQ_CALL_; }

	void OnReq_StopTRIN()	{ m_nReq_TRIN	= REQ_STOP_; }
	void OnReq_StopTROUT()	{ m_nReq_TROUT	= REQ_STOP_; }

	int  OnGet_ReadLSSD()	{ return m_nReq_LSSD; }
	int  OnGet_ReadRSSD()	{ return m_nReq_RSSD; }
	int  OnGet_ReadTRIN()	{ return m_nReq_TRIN; }
	int  OnGet_ReadTROUT()	{ return m_nReq_TROUT; }

	void OnReset_WorkPosTRIN()	{ m_nPos_TRIN	= 0; }
	void OnReset_WorkPosTROUT()	{ m_nPos_TROUT	= 0; }

};
extern CBcrEvent	evBCR;

#endif // !defined(AFX_BCREVENT_H__E680F49C_DABB_449D_B535_4524DED94AB5__INCLUDED_)
