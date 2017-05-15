// CimEvent.cpp: implementation of the CCimEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CimEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCimEvent	g_client_cim;

CCimEvent::CCimEvent()
{
	// 소켓에 대한 구조체 배열 정보
	m_nArrayPos = 1;

	//m_sRcvData = "";
	m_sRcvData.Empty();

}

CCimEvent::~CCimEvent()
{
	OnClearMsgQueue();  // 메시지 수신용 큐 클리어

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CCimEvent::OnSeq_RUN(void) 
{
	OnSeq_INIT();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CCimEvent::OnSeq_INIT(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;

	OnClearMsgQueue();  // 메시지 수신용 큐 클리어
}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CCimEvent::OnSeq_Execute(void) 
{
	CString sRecvData="";
	int nResp = 0;

	// [EQP<->CIM} 연결된 경우에만 메시지 꺼내서 처리함
	if (st_client[m_nArrayPos].n_connect == CTL_YES)
	{
		// 큐에서 메시지 하나를 꺼냄
		nResp = OnGetFromQueue(sRecvData);
		if (nResp == MSG_EXIST)
		{
			m_sLogMsg.Format("[Process Start] : %s", sRecvData);
			Func.On_LogFile_Add(99, m_sLogMsg);

			// 큐에서 꺼낸 메시지 처리함
			OnParseComplete((LPCSTR)sRecvData);  // CIM에서 받은 메시지 파싱
		}
	}
}

//==================================================================//
// [->큐] 수신 메시지 저장
//==================================================================//
void CCimEvent::OnPutToQueue(CString szData) 
{
	CSingleLock pRecvQLock(&m_csPutQueue);
	pRecvQLock.Lock();

	if (m_qReqData.size() == 0)
	{
		OnClearMsgQueue();
	}
	m_qReqData.push(szData);

	pRecvQLock.Unlock();
}

//==================================================================//
// [<-큐] 수신된 메시지 꺼내기
//==================================================================//
int CCimEvent::OnGetFromQueue(CString &spData) 
{
	CSingleLock pQLock(&m_csGetQueue);
	pQLock.Lock();

	int nRet = MSG_EMPTY;
	// 큐에 메시지가 존재하는지 검사
	if (m_qReqData.size() == 0)
	{
		;
	}
	else
	{
		// 큐에 저장된 메시지 설정
		spData = m_qReqData.front();
		// 반환한 메시지는 큐에서 삭제함
		m_qReqData.pop();

		nRet = MSG_EXIST;
	}
	pQLock.Unlock();

	return nRet;
}

//==================================================================//
// [MES->CIM] 수신된 메시지 구분하여 처리
//==================================================================//
int CCimEvent::OnDivide_FromCIM(int nzSocketNo, CString szCommand) 
{
	int nRet = 0;
	
	// 수신된 데이터가 없으면 강제 리턴
	if (szCommand == "")
	{
		return -1;
	}
	// 수신된 메시지 로그에 남김
	m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nzSocketNo, szCommand);
	Func.On_LogFile_Add(99, m_sLogMsg);
	
	// 수신된 메시지 누적시킴
	// - 예를 들면 (HD=000025,FUNC=INTERFACE,CONNECT=0,)
	m_sRcvData += szCommand;

	int nLen_TotRcv;
	CString sPart_Head, sPart_Len, sPart_Body, sData_Process;
	int nPart_Len = -1, nPart_Body = -1;

	do{
		// 전제 수신된 문자열 길이 계산
		nLen_TotRcv = m_sRcvData.GetLength();

		// 수신 메시지의 최소 길이 검사
		// - Mid() 함수 사용 시 인자값으로 0이 사용되면 문제 없지만 그 외에는 문제 발생함
		// : 공백 문자열을 Mid(0,?)는 상관 없지만 Mid(4,?)는 문제가 발생함
		if (nLen_TotRcv > 10)
		{
			// 데이터의 헤더부 얻기 [ 예를 들면 (HD=) ]
			sPart_Head	= m_sRcvData.Mid(0,3);
			// 데이터의 길이부 얻기 [예를 들면 (000025) ]
			sPart_Len	= m_sRcvData.Mid(3,6);
			// 수신할 데이터의 길이 정보 설정
			nPart_Len	= atoi((LPCSTR)sPart_Len);
			// 실재 데이터부 얻기 [ 예를 들면 (FUNC=INTERFACE,CONNECT=0,) ]
			sPart_Body	= m_sRcvData.Mid(10);
			// Body(데이터) 정보 설정
			nPart_Body	= sPart_Body.GetLength();

			// 데이터의 정합성 검사
			// - 헤더부 문자열 검사
			// - 길이부의 문자가 숫자형인지 검사
			// : 길이부의 첫글자만 비교함
			// : 이 부분은 숫자형으로 구성되어 있어야 함
			if (sPart_Head == "HD=" &&
				m_sRcvData.GetAt(3) >= '0' && m_sRcvData.GetAt(3) <= '9')
			{
				// 수신할 데이터의 길이 정보가 문제가 없는지 확인
				// - 수신할 데이터의 길이는 최소 1글자 이상은 되어야 함
				if (nPart_Len < 1)
				{
					m_sRcvData.Empty();
					nRet = -1;
				}
				// 수신할 문자열 길이가 실재로 수신한 문자열보다 크면
				// - 계속 데이터를 수신해야 하므로 일단 리턴함
				if (nPart_Body < nPart_Len)
				{
					nRet = -1;
				}
				// 문제가 있는 경우에는 해당 메시지를 처리하지 않음
				if (nRet != 0)
				{
					break;
				}

				// 정상 데이터 [처리 가능]
				// [헤더(9글자)+명령어부] 문자열 구함
				sData_Process = m_sRcvData.Mid(0, nPart_Len +10);
				// 전제 수신된 문자열 정보 설정
				nLen_TotRcv = sData_Process.GetLength();
				// 처리하기 위해 뽑아낸 데이터는 삭제함
				// - 수신된 데이터에서 처리한 문자열만큼 삭제시킴
				// - 처리하고 남은 데이터를 버퍼 변수에 재할담
				m_sRcvData = m_sRcvData.Mid(nLen_TotRcv);

				// 하나의 명령어를 큐에 넣음
				OnPutToQueue(sData_Process);
			}
			else
			{
				// 처리해야 할 한개의 명령어 길이보다 수신 데이터의 길이가 크면서 명령어 포멧에 문제가 있는 경우
				// - 강제로 버퍼를 비워 다음 수신 데이터부터 처리되도록 함
				if (nPart_Body >= nPart_Len)
				{
					// 지금까지 수신된 데이터는 폐기함
					// - 추후에 수신되는 데이터부터 처리되도록 함
					m_sRcvData.Empty();
				}
				// 수신된 데이터가 처리 불가능한 데이터
				// - 다음 수신 메시지의 처리를 위해 강제로 리턴시킴
				nRet = -1;
			}
		}
		// 명령어 최소 길이 포멧을 만족하지 못함
		// - 데이터를 더 수신해야 함
		else
		{
			nRet = -1;
		}
    }while(nRet != -1);

	return 0;
}

void CCimEvent::OnParseComplete(std::string strRecv) 
{
	// 처리할 메시지가 공백이면 강제로 리턴시킴
	if (strRecv == "")
	{
		return;
	}

	// 처리할 명령어 종류 확인
	CString sFunc = OnPickOut_RcvMsg("FUNC", (LPCSTR)strRecv.c_str());

	// [CEID] : [EQ->HOST]으로 전송한 메시지에 대한 에코
	// [RCMD] : [HOST->EQ]으로 보내준 응답
	if (sFunc == "RCMD")
	{
		CString sRCMD = OnPickOut_RcvMsg("RCMD_NUM", (LPCSTR)strRecv.c_str());

		// 이곳에 RCMD에 대한 내용 프로그램 작업
		if (sRCMD == "_RCMD_BBAR")
		{

		}
	}
	// [HOS->EQP] 컨트롤 모드 전송
	else if (sFunc == "INTERFACE")
	{
		OnReceived_Interface(strRecv);
	}
	// [EQ->CIM->EQ] 컨트롤 모드에 대한 에코
	else if (sFunc == "OPERATE")
	{
		OnReceived_Operate(strRecv);
	}
	// 규이리 추가 [2013.09.25]
	// [CIM<->HOST] 연결 상태 정보
	else if (sFunc == "HCON")
	{
		OnReceived_HCON(strRecv);	// [CIM<->HOST] 연결 상태 정보 전송
	}
}

//==================================================================//
// [HOS->EQP] 컨트롤 모드 전송
//==================================================================//
void CCimEvent::OnReceived_Interface(std::string strRecv) 
{
	CString sFunc = OnPickOut_RcvMsg("CONNECT", (LPCSTR)strRecv.c_str());

	if (sFunc == "0")	// OFF LINE
	{
		st_work.nEQ_CtrlMode = MC_OFF_LINE;
	}
	else if (sFunc == "1")	// ONLINE LOCAL
	{
		st_work.nEQ_CtrlMode = MC_ON_LINE_LOCAL;
	}
	else if (sFunc == "2")	// ONLINE REMOTE
	{
		st_work.nEQ_CtrlMode = MC_ON_LINE_REMOTE;
	}

	// [0/1/2] 이외의 값이 들어와도 일단 통신은 종료된 것으로 처리함
	if (m_eWR == EWR_INTERFACE)	{
		m_eWR = EWR_NONE;
	}
}

//==================================================================//
// [EQ->CIM->EQ] 컨트롤 모드에 대한 응답 처리
// - [EQ<->CIM] 데이터 전송 가능 여부를 컨트롤 모드에 대한 응답으로 설정함
// : OFFLINE에서는 메시지를 전송할 필요가 없기 때문임
// : 현재는 필요 없으면 사용하지 않아도 됨
//==================================================================//
void CCimEvent::OnReceived_Operate(std::string strRecv) 
{
	CString sFunc = OnPickOut_RcvMsg("CONNECT", (LPCSTR)strRecv.c_str());
	
	if (sFunc == "0")	// OFF LINE
	{
		st_handler.nEQ2CIM_SendPpossible = 1;
	}
	else if (sFunc == "1")	// ONLINE LOCAL
	{
		//st_handler.nEQ2CIM_SendPpossible = 1;
		// 규이리 수정 [2013.09.01]
		st_handler.nEQ2CIM_SendPpossible = 2;
	}
	else if (sFunc == "2")	// ONLINE REMOTE
	{
		st_handler.nEQ2CIM_SendPpossible = 2;
	}

	if (m_eWR == EWR_OPERATE)	{
		m_eWR = EWR_NONE;
	}
}

//==================================================================//
// [CIM<->HOST] 연결 상태 정보
//==================================================================//
void CCimEvent::OnReceived_HCON(std::string strRecv) 
{
	CString sFunc = OnPickOut_RcvMsg("CONNECT", (LPCSTR)strRecv.c_str());
	
	// [CIM<->HOST] 연결 상태 정보 [미연결]
	if (sFunc == "0")
	{
		st_work.nLink_CIM2TMS = CTL_NO;
	}
	// [CIM<->HOST] 연결 상태 정보 [연결]
	else if (sFunc == "1")
	{
		st_work.nLink_CIM2TMS = CTL_YES;
	}

	if (m_eWR == EWR_HCON)	{
		m_eWR = EWR_NONE;
	}
}


//==================================================================//
// 원하는 부위의 데이터 값 반환
// 규이리 추가 [2013.07.31]
// - int nzMidPos : 잘라낼 문자열
// : 에를 들어, ( "CONNECT=" ) 값이 szLetter 값이면 nzMidPos 값은 ( 8 ) 임
// - CString szLetter    : 검사를 시작할 선두 문자열
// - CString szSeparator : 검사를 종료할 선미 문자열
// - CString szRcvMsg    : 검사할 문자열
//==================================================================//
CString CCimEvent::OnAnalysisRcvMSG(int nzMidPos, CString szLetter, CString szSeparator, CString szRcvMsg, int nzMode) 
{
	int nPos[2], nLength;
	CString sFindVal = "";

	int nSkipPos;
	if (nzMode == 1)
	{
		nSkipPos = nzMidPos;
	}
	else
	{
		// 실재로 제거할 문자열 길이는 [찾을문자열 길이] - [,]임
		// 예를 들면 [RETURN_FLAG,OK] 형태이며 실재 원하는 데이터는 [OK] 임
		// 이런 이유로 (+1) 해줌
		nSkipPos = nzMidPos+1;
	}

	nPos[0] = szRcvMsg.Find(szLetter, 0);
	nPos[1] = szRcvMsg.Find(szSeparator, nPos[0]);
	nLength = (nPos[1] - nPos[0]) - nSkipPos;
	if (nLength < 1)
	{
		// 공백 문자열 반환시킴
		return sFindVal;
	}
	
	sFindVal = szRcvMsg.Mid(nPos[0]+nSkipPos, nLength);
	return sFindVal;
}

//==================================================================//
// [EQP->CIM] 메시지 전송
//==================================================================//
int CCimEvent::PushSendMsg(CString szMsg) 
{
	// 전송할 문자열 설정
	sprintf(st_client[m_nArrayPos].ch_send, szMsg);

	// [EQP->CIM] 메시지 전송 요청함
	//::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_SEND, m_nArrayPos);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_SEND, m_nArrayPos);
	return 0;
}

//==================================================================//
// 특정한 문자열 사이의 데이터 반환
// - szOrg : 예를 들면 (HD=000025,FUNC=INTERFACE,CONNECT=0,)
//==================================================================//
CString CCimEvent::OnPickOut_RcvMsg(CString szFind, CString szOrg) 
{
	// 검사할 문자열 정보 설정
	CString sFindChar;
	sFindChar  = szFind;
	sFindChar += "=";
	
	// 전체 문자열에서 찾고자 하는 문자열을 검색함
	int nFind	= szOrg.Find(sFindChar);
	// 데이터 사이의 구분자 위치 검색함
	// - 찾고자 하는 문자열의 다음 위치부터 찾기 시작함
	int nEnd	= szOrg.Find(",", nFind+1);
	
	if (nFind < 0 || nEnd < 0 ||
		nEnd <= (nFind + sFindChar.GetLength()))
	{
		return "";
	}
	
	CString sSearchChar;
	sSearchChar = szOrg.Mid(nFind + sFindChar.GetLength(), nEnd - (nFind + sFindChar.GetLength()));
	
	return sSearchChar;
}
