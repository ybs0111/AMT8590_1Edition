// MesEvent.h: interface for the CMesEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESEVENT_H__02F160AB_99DE_4A60_8DBC_CE1CC05F6105__INCLUDED_)
#define AFX_MESEVENT_H__02F160AB_99DE_4A60_8DBC_CE1CC05F6105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "Public_Function.h"

//==================================================================//
// 메시지큐 사용하기 위해 추가
// 규이리 추가 [2014.12.11]
//==================================================================//
#include <iostream>
#include <queue>
#include <string>
using namespace std;
//------------------------------------------------------------------//
#include "Afxmt.h"  // CCriticalSecton으로 CSingleLock을 사용하기 위해 추가
#include <vector>

class CMesEvent  
{
public:
	CMesEvent();
	virtual ~CMesEvent();

// Attributes
private:
	CCriticalSection m_csPutQueue;
	CCriticalSection m_csGetQueue;

	CString m_sLogMsg;
	CString m_sRcvData;

	int  m_nArray;

	int  m_nReqMesWork;

	int  m_nDataParse;
	long m_lWaitResp[3];
	
// Attributes
public:
	bool m_bflag_INIT;
	int  m_nStep_RUN;
	
	int  m_nReqStart;
	
	int  m_nIdx;
	
	int  m_nExecutedFunc;
	int  m_nReqSearch;
	// 규이리 추가 [2014.12.13]
	CString m_sPutData;
	
	//==============================================================//
	// 메시지를 저장할 큐 변수
	//==============================================================//
	//queue< int> m_qReqData;  // 구분자 단위의 문자열 저장 데이터 큐
	// 규이리 추가 [2014.12.13]
	queue< CString> m_qReqData;  // 구분자 단위의 문자열 저장 데이터 큐
	//--------------------------------------------------------------//
	
// Operations
private:
	
// Operations
public:
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_Init(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	//==============================================================//
	// 메시지큐 및 쓰레드 생성 관련 부분
	//==============================================================//
	inline OnClearMsgQueue()	{ while(m_qReqData.size()) m_qReqData.pop(); };  // 메시지 수신용 큐 클리어
	void OnPutToQueue(int nzData);   // 수신된 메시지를 큐에 저장
	//int  OnGetFromQueue(int &nzData);  // 큐에 입력된 문자열 반환
	// 규이리 수정 [2014.12.13]
	int  OnGetFromQueue(CString &spData);  // 큐에 입력된 문자열 반환
	//--------------------------------------------------------------//

	int  OnRun_MesWork();	// [EQP<->MES] 메시지 전송 및 처리

	int OnDivide_FromMES(int nzSocketNo, CString szCommand);	// [MES->EQP] 수신된 메시지 구분하여 처리
	int OnAnalysis_MesData(CString szRcvData);	// 수신된 메시지 분석 작업

	int  PushSendMsg(CString szMsg);	// [EQP->MES] 메시지 전송

	void OnReq_MesWork()	{ m_nReqStart = REQ_CALL_; }
	int  OnGet_MesWork()	{ return m_nReqStart; }

	void OnCreate_SendFormat(int nzIdx);	// [EQP->TMS] 전송할 메시지 생성

	CString		m_strBcrFormat[2];
	CString		m_strSerial[2];
	void		ClrSerial( int nSite) { m_strSerial[nSite] = ""; }
	void		SetSerial(int nSite, CString strSerial) { m_strSerial[nSite] = strSerial; }
	CString		GetSerial( int nSite) { return m_strSerial[nSite]; }
	void		ClrBcrFormat(int nSite) { m_strBcrFormat[nSite] = ""; }
	void		SetBcrFormat(int nSite, CString strBcrFormat) { m_strBcrFormat[nSite] = strBcrFormat; }
	CString		GetBcrFormat(int nSite) { return m_strBcrFormat[nSite];	}


};
extern CMesEvent	evMes;

#endif // !defined(AFX_MESEVENT_H__02F160AB_99DE_4A60_8DBC_CE1CC05F6105__INCLUDED_)
