// CimEvent.h: interface for the CCimEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIMEVENT_H__819DE402_D5C1_4DDE_BF95_CBE2FE5CD8D0__INCLUDED_)
#define AFX_CIMEVENT_H__819DE402_D5C1_4DDE_BF95_CBE2FE5CD8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================//
// 메시지큐 사용하기 위해 추가
//==================================================================//
/*
#include <queue>  // 큐 자료구조체 사용
using namespace std;
*/
#include <iostream>
#include <queue>
#include <string>
using namespace std;
//------------------------------------------------------------------//
#include "Afxmt.h"  // CCriticalSecton으로 CSingleLock을 사용하기 위해 추가

#include <vector>

#include "Variable.h"

enum eWaitReply
{
	EWR_NONE,
	EWR_PROCESSSTATUS,
	EWR_PGET,
	EWR_DGET,	// 규이리 추가 [2013.10.30]
	EWR_MASTERLOT,
	EWR_ENDLOT,
	EWR_LMVIN,
	EWR_LMVOU,
	EWR_EQPSTATUS,
	EWR_COLLECTDATA,
	EWR_BINCODESET,
	EWR_BBAR,
	EWR_OPERATE,
	EWR_INTERFACE,
	EWR_HCON,
};

class CCimEvent  
{
public:
	CCimEvent();
	virtual ~CCimEvent();


// Attributes
private:
	CCriticalSection m_csPutQueue;
	CCriticalSection m_csGetQueue;

	CString m_sLogMsg;
	CString m_sRcvData;

	eWaitReply m_eWR;

	int  m_nArrayPos;
	
// Attributes
public:
	bool m_bflag_INIT;

	//==============================================================//
	// 메시지를 저장할 큐 변수
	//==============================================================//
	queue< CString> m_qReqData;  // 구분자 단위의 문자열 저장 데이터 큐
	//--------------------------------------------------------------//
	
// Operations
private:
	
// Operations
public:
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_INIT(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	//==============================================================//
	// 메시지큐 및 쓰레드 생성 관련 부분
	//==============================================================//
	inline OnClearMsgQueue()	{ while(m_qReqData.size()) m_qReqData.pop(); };  // 메시지 수신용 큐 클리어
	void OnPutToQueue(CString szData);   // 수신된 메시지를 큐에 저장
	int  OnGetFromQueue(CString &spData);  // 큐에 입력된 문자열 반환
	//--------------------------------------------------------------//
	int  OnDivide_FromCIM(int nzSocketNo, CString szCommand);	// [MES->CIM] 수신된 메시지 구분하여 처리

	//==============================================================//
	// [EQP<-CIM] 수신 데이터 파싱하여 처리
	//==============================================================//
	void OnParseComplete(std::string strRecv);

	void OnReceived_Interface(std::string strRecv);
	void OnReceived_Operate(std::string strRecv);
	void OnReceived_HCON(std::string strRecv);	// [CIM<->HOST] 연결 상태 정보 전송

	// 원하는 부위의 데이터 값 반환
	CString OnAnalysisRcvMSG(int nzMidPos, CString szLetter, CString szSeparator, CString szRcvMsg, int nzMode=0);
	//--------------------------------------------------------------//

	void SetWaitReply(eWaitReply ewr)	{ m_eWR = ewr; }
	eWaitReply GetWaitReply()	{ return m_eWR; }

	int PushSendMsg(CString szMsg);	// [EQP->CIM] 메시지 전송
	CString OnPickOut_RcvMsg(CString szFind, CString szOrg);	// 특정한 문자열 사이의 데이터 반환

};
extern CCimEvent	g_client_cim;

#endif // !defined(AFX_CIMEVENT_H__819DE402_D5C1_4DDE_BF95_CBE2FE5CD8D0__INCLUDED_)
