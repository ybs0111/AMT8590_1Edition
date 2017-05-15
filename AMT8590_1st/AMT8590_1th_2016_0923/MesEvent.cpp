// MesEvent.cpp: implementation of the CMesEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MesEvent.h"
#include "AMTLotManager.h"
#include "RunNetwork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMesEvent	evMes;

CMesEvent::CMesEvent()
{
	// 사용할 소켓 번호는 (WM_CLIENT_MSG_1)
	// - 배열 번호는 0부터 시작하므로 (-1)함
	m_nArray = 0;

	// 규이리 수정 [2014.12.13]
	// 파일에서 읽어서 해당 정보 설정하도록 수정됨
	/*
	// 클라이언트 소켓에 대한 포트 및 IP 할당
	st_client[m_nArray].n_port = 2000;
	st_client[m_nArray].str_ip = "127.0.0.1";
	*/

}

CMesEvent::~CMesEvent()
{
	OnClearMsgQueue();  // 메시지 수신용 큐 클리어

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CMesEvent::OnSeq_RUN(void) 
{
	OnSeq_Init();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CMesEvent::OnSeq_Init(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;
	
	m_nStep_RUN = 0;	// [리셋] 스텝 정보
	
	// 큐에서 메시지 하나씩 꺼내 처리 시 사용
	m_nReqSearch	= CTL_GOOD;	// 큐에서 메시지 찾기 작업 필요
	m_nExecutedFunc	= 0;	// [리셋] 바로전에 실행한 메시지 [Retry 작업시 사용]
	
	//m_sRcvData = "";
	m_sRcvData.Empty();
	
	OnClearMsgQueue();  // 메시지 수신용 큐 클리어
}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CMesEvent::OnSeq_Execute(void) 
{
	CString sRecvData;
	int nSendFunction;
	int nResp = 0;

	// 큐에서 전송할 메시지를 찾아야 하는 경우인지 확인
	if (m_nReqSearch == CTL_GOOD)
	{
		// 큐에서 메시지를 하나 꺼냄
		nResp = OnGetFromQueue(sRecvData);
		if (nResp == MSG_EXIST)
		{
			nSendFunction	= atoi(sRecvData);	// 메시지 생성해서 전송할 Id 정보 얻음
			OnCreate_SendFormat(nSendFunction);	// 전송할 메시지 생성
			m_nExecutedFunc	= nSendFunction;	// 현재 전송할 메시지 Id 설정
			m_nReqStart		= REQ_CALL_;	// [Conv->MES] 생성된 메시지 전송 요청 (OnRun_MesWork 함수에서 전송)

			m_nReqSearch = CTL_NO;	// 큐에서 메시지 찾기 잠시 대기
		}

		// 규이리 수정 [2014.12.13]
		/*
		nResp = OnGetFromQueue(nSendFunction);
		if (nResp == MSG_EXIST)
		{
			OnCreate_SendFormat(nSendFunction);
			m_nExecutedFunc = nSendFunction;

			m_nReqStart = REQ_CALL_;
			m_nReqSearch = CTL_NO;
		}
		*/
	}
	// 전송에 에러가 발생한 경우에는 Retry 동작 진행
	// - 이 처리는 각각의 시컨스에서 담당하도록 함
	else if (m_nReqSearch == CTL_ERROR)
	{
		// 규이리 주석 처리
		// 리트라이는 해당 함수안에서 처리하도록 함
		// - 통신 실패 알람을 출력할 수 없게 되어 수정함
		/*
		OnCreate_SendFormat(m_nExecutedFunc);
		
		m_nReqStart = REQ_CALL_;
		m_nReqSearch = CTL_NO;
		*/
		// 규이리 추가 [2014.12.13]
		m_nReqSearch = CTL_GOOD;	// 큐에서 전송할 메시지를 다시 찾아야 함
	}

	// [Conv->MES] 생성된 메시지 전송 요청이 발생하기를 기다림
	if (m_nReqStart == REQ_CALL_)
	{
		OnRun_MesWork();	// [EQP<->MES] 메시지 전송 및 처리
	}
}

//==================================================================//
// [->큐] 전송 메시지 저장
//==================================================================//
void CMesEvent::OnPutToQueue(int nzData) 
{
	CSingleLock pRecvQLock(&m_csPutQueue);
	pRecvQLock.Lock();
	
	// 규이리 추가 [2014.12.13]
	// 입력된 정보를 (숫자->문자열) 변환
	m_sPutData.Format("%d", nzData);
	
	if (m_qReqData.size() == 0)
	{
		OnClearMsgQueue();
	}
	//m_qReqData.push(nzData);
	// 규이리 수정 [2014.12.13]
	m_qReqData.push(m_sPutData);
	
	pRecvQLock.Unlock();
}

//==================================================================//
// [<-큐] 전송 메시지 꺼내기
//==================================================================//
//int CMesEvent::OnGetFromQueue(int &nzData) 
// 규이리 수정 [2014.12.13]
int CMesEvent::OnGetFromQueue(CString &spData) 
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
		//nzData = m_qReqData.front();
		// 규이리 수정 [2014.12.13]
		spData = m_qReqData.front();
		// 반환한 메시지는 큐에서 삭제함
		m_qReqData.pop();
		
		nRet = MSG_EXIST;
	}
	pQLock.Unlock();
	
	return nRet;
}

//==================================================================//
// [EQP<->MES] 메시지 전송 및 처리
//==================================================================//
int CMesEvent::OnRun_MesWork() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	CString sMsg;
	switch(m_nStep_RUN)
	{
	case 0:
		// [EQP<->MES] 연결되어 있는지 확인
		if (st_client[m_nArray].n_connect == CTL_YES)
		{
// 			m_nStep_RUN = 1000;
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG +m_nArray, CLIENT_CLOSE, m_nArray);
		}
		// 미연결 상태이면 연결 작업 시도
		else
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG +m_nArray, CLIENT_CONNECT, m_nArray);

			// 연결 시작 시간 설정
			m_lWaitResp[0] = GetCurrentTime();
			m_nStep_RUN = 100;
		}
		break;

	case 100:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}

		if (st_client[m_nArray].n_connect == CTL_YES)
		{
			m_nStep_RUN = 1000;
		}
		else
		{
			if (m_lWaitResp[2] > 5000)
			{
				m_nStep_RUN = 5000;	// 에러 반환 스텝
			}
		}
		break;

	case 1000:
		// 헤더 정보 전송
		sMsg = st_client[m_nArray].chSendHead;
		PushSendMsg(sMsg);	// [EQP->LowerStream] 메시지 전송
		// 바디 정보 전송
		sMsg = st_client[m_nArray].chSendBody;
		PushSendMsg(sMsg);	// [EQP->LowerStream] 메시지 전송

		m_nIdx = st_client[m_nArray].nIndex;
		// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
		// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
		if ((st_client[m_nArray].stComm[m_nIdx]).nResponse == 10)
		{
			// 타임아웃 검사할지 여부 [0:검사불필요, 1:검사필요]
			if ((st_client[m_nArray].stComm[m_nIdx]).nTimeOut == 1)
			{
				//m_nStep_RUN = 1500;
				m_nStep_RUN = 1100;
			}
			else
			{
				//m_nStep_RUN = 1100;
				m_nStep_RUN = 1500;
			}
		}
		else
		{
			m_nStep_RUN = 4000;
		}
		break;

	case 1100:
		// 전송한 메시지에 대한 응답 체크 시작 시간 설정
		m_lWaitResp[0] = GetCurrentTime();
		m_nStep_RUN = 1200;
		break;
		
	case 1200:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}
		
		// [EQP->MES] 수신 메시지 파싱 함수에서 설정함
		
		// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
		// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
		if ((st_client[m_nArray].stComm[m_nIdx]).nResponse == CTL_GOOD)
		{
			m_nStep_RUN = 4000;
		}
		else if ((st_client[m_nArray].stComm[m_nIdx]).nResponse == CTL_ERROR)
		{
			m_nStep_RUN = 5000;
		}
		else
		{
			// 통신 타임 아웃 발생
			if (m_lWaitResp[2] > MES_TIME_OUT)
			{
				(st_client[m_nArray].stComm[m_nIdx]).nResponse = CTL_ERROR;
				m_nStep_RUN = 5000;
			}
		}
		break;
		
	case 1500:
		// [EQP->MES] 수신 메시지 파싱 함수에서 설정함
		
		// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
		// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
		if ((st_client[m_nArray].stComm[m_nIdx]).nResponse == CTL_GOOD)
		{
			m_nStep_RUN = 4000;
		}
		else if ((st_client[m_nArray].stComm[m_nIdx]).nResponse == CTL_ERROR)
		{
			m_nStep_RUN = 5000;
		}
		break;
		
		// 정상 완료 스텝
	case 4000:
		m_nReqSearch = CTL_GOOD;	// 큐에서 전송할 메시지를 다시 찾아야 함
		
		m_nReqStart = REPLY_CORRECT_;	// 정상적으로 응답
		m_nStep_RUN = 0;
		FuncRet = RET_GOOD;
		break;
		
		// 에러 반환 스텝
	case 5000:
		m_nReqSearch = CTL_ERROR;	// ReTry 작업이 필요
		
		m_nReqStart = REPLY_ERROR_;	// 응답에 에러 발생함
// 		m_nStep_RUN = 0;
// 		FuncRet = RET_ERROR;

		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG +m_nArray, CLIENT_CLOSE, m_nArray);
		m_lWaitResp[0] = GetCurrentTime();
		m_nStep_RUN = 6000;
		break;

	case 6000:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}
		else if (m_lWaitResp[2] > 4000)
		{
			m_nStep_RUN = 0;
			FuncRet = RET_ERROR;
		}
		break;
	}
	return FuncRet;
}

//==================================================================//
// [MES->EQP] 수신된 메시지 구분하여 처리
//==================================================================//
int CMesEvent::OnDivide_FromMES(int nzSocketNo, CString szCommand) 
{
	int nRet = 0;
	
	// 수신된 데이터가 없으면 강제 리턴
	if (szCommand == "")
	{
		return -1;
	}
	// 수신된 메시지 로그에 남김
	m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nzSocketNo, szCommand);
	//2017.0510
// 	cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
	
	CString sPart_Len, sPart_Body, sData_Process;
	int nPart_Len, nPart_Body, nLen_TotRcv;
	
	// 전송한 명령어의 인덱스 정보 얻음
	// - 전송한 명령어에 대한 응답인지 확인하기 위함
	int nIdx = st_client[m_nArray].nIndex;
	
	m_sRcvData += szCommand;	// 수신 메시지 누적시킴

	do{
		nLen_TotRcv = m_sRcvData.GetLength();
		// 최소한 길이부+데이터(1글자) 이상이어야 함
		if (nLen_TotRcv > 16)
		{
			// 데이터의 길이부 얻기
			sPart_Len = m_sRcvData.Mid(0,16);
			nPart_Len = atoi((LPCSTR)sPart_Len);
			// 실재 데이터부 정보 얻기
			sPart_Body = m_sRcvData.Mid(16);
			nPart_Body = sPart_Body.GetLength();
			
			// 데이터의 정합성 검사
			// - 길이부의 문자가 숫자형인지 검사
			// : 길이부의 첫글자만 비교함
			// : 이 부분은 숫자형으로 구성되어 있어야 함
			if (sPart_Len.GetAt(0) >= '0' && sPart_Len.GetAt(0) <= '9')
			{
				// [길이부+데이터부] 짤라냄
				sData_Process = m_sRcvData.Mid(0, nPart_Len +16);

				if (nPart_Body < nPart_Len)
				{
					nRet = -1;
					break;
				}

				// 수신된 메시지 중에서 현재 처리할 메시지를 제거한 문자열 정보
				// - 수신된 데이터에서 처리할 문자열만큼 삭제시킴
				// - 처리하고 남은 데이터를 버퍼 변수에 재할당
				m_sRcvData = m_sRcvData.Mid(nPart_Len +16);
				
				// 정보가 2개 연달아 (붙어서) 들어오는 경우 이곳에서 곧바로 파싱하는 함수 호출하면 문제가 발생함
				// - 처음에 수신된 명령어는 처리되는데 뒤에 붙어서 수신된 명령어는 처리되지 못하고 다음 명령어 수신된 경우에 처리되게 됨
				// - 즉, 명령어가 밀려서 처리될 수 있음
				// - 하지만 현재 메시지 쳬계로는 [명령어<->응답] 구조이므로 2개 명령어가 연달아 수신되지 않아 문제는 없음
				int nResp = OnAnalysis_MesData(sData_Process);
				if (nResp == RET_ERROR)
				{
					//m_sRcvData = "";
					
					// 수신 데이터 파싱 작업 중 에러 발생함
					// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
					// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
					(st_client[m_nArray].stComm[nIdx]).nResponse = CTL_ERROR;
				}
				else if (nResp == RET_GOOD)
				{
					m_sRcvData = "";

					// 수신 데이터 정상 파싱됨
					// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
					// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
					(st_client[m_nArray].stComm[nIdx]).nResponse = CTL_GOOD;
				}

				m_sRcvData.Empty();
			}
			else
			{
				// 처리해야 할 한개의 명령어 길이보다 수신 데이터의 길이가 크면서 명령어 포멧에 문제가 있는 경우
				// - 강제로 버퍼를 비워 다음 수신 데이터부터 처리되도록 함
				if (nPart_Body > nPart_Len)
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
    }while (nRet != -1);
	
	return 0;
}

//==================================================================//
// 수신된 메시지 분석 작업
//==================================================================//
int CMesEvent::OnAnalysis_MesData(CString szRcvData) 
{
	// 헤더 정보 추출 [공백 또는 찾은 문자열이 반환]
	CString sHeader = Func.OnFindInRcvMsg("FUNCTION_RPY=", szRcvData, " ");	// 특정한 문자열 사이의 데이터 반환
	CString sBody[40], sTmp;
	int i=0, nChk=0, nLoop=0;

	// 작업 공정 종료
	// [Conv] 설비에서 사용됨
	//2016.0915
	if (sHeader == "REWORK_LABEL_CCS")
	{
		// ??? ??? ??? ??? ??
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, "END");
		if(sBody[2] == "PASS")
		{
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[3]);
				alarm.mstr_svr_alarm = sBody[3];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ??? ?? ??
			}
		}
	}
	else if (sHeader == "LOT_CLOSE")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("PART_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("LOT_QTY=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("TK_CODE=",	szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("SERIHAL=",	szRcvData, " ");
		sBody[5]	= Func.OnFindInRcvMsg("PPID=",		szRcvData, " ");
		sBody[6]	= Func.OnFindInRcvMsg("WWN=",		szRcvData, " ");
		sBody[7]	= Func.OnFindInRcvMsg("C_SERIAL=",	szRcvData, " ");
		sBody[8]	= Func.OnFindInRcvMsg("PSID=",		szRcvData, " ");
		sBody[9]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[10]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
	}
	// 자재의 바코드 정보 체크 (개별)
	// [Conv] 설비에서 사용됨
	else if (sHeader == "PEBAG_CHECK")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("SERIAL=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("PPID=",		szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("WWN=",		szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("C_SERIAL=",	szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("PSID=",		szRcvData, " ");
		sBody[5]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");	// (PASS/FAIL)
		sBody[6]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
	}
	// 자재의 바코드 정보 체크 (트레이 단위)
	// [Conv] 설비에서 사용됨
	else if (sHeader == "BRAND_PEBAG_CHECK")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("SSD_CNT=",	szRcvData, " ");

		nLoop = atoi(sBody[3]);	// 불량에 대한 갯수 정보
		for(i=0; i<nLoop; i++)
		{
			sTmp.Format("SRL%02d=", i +1);
			sBody[4+ i]	= Func.OnFindInRcvMsg(sTmp,	szRcvData, " ");
		}
	}
	// (PEBAG_CHECK) 완료 보고
	// - 맨 마지막 트레이에 대한 (PEBAG_CHECK) 후에 전송
	// [Conv] 설비에서 사용됨
	else if (sHeader == "PEBAG_COMPLETE")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
	}
	//==============================================================//
	// 메인 설비에서 사용
	//==============================================================//
	// 설비에서 Id 버퍼 흘러가는 상태 보고
	// New->(change)->End 형식으로 보고
	// - New : Id 버퍼에 자재 내려놓은 경우
	// - Change : Id 버퍼가 변경되거나 해당 자재 시리얼 정보가 변경될 경우
	// - End : 하류 설비로 흘려보낼 경우
	else if (sHeader == "BOAT_ID_MODIF")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",		szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("BOAT_ID=",		szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("EQP_ID=",		szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("SERIAL=",		szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("MODIF_MODE=",	szRcvData, " ");
		sBody[5]	= Func.OnFindInRcvMsg("RESULT=",		szRcvData, " ");
		sBody[6]	= Func.OnFindInRcvMsg("MSG=",			szRcvData, "END");
		if(sBody[5] == "PASS")
		{
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[6]);
				alarm.mstr_svr_alarm = sBody[6];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
			}
		}
	}
	// 작업자 확인용
	// - 시리얼 정보는 아는데 버퍼 정보를 모를 경우에 사용
	else if (sHeader == "PROD_SERIAL_INFO")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("BOAT_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("SERIAL=",	szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("PART_ID=",	szRcvData, " ");
		sBody[5]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[6]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
	}
	// 작업자 확인용
	// - 해당 버퍼에 어떤 자재가 들어있는지 확인 시 사용
	else if (sHeader == "BOAT_ID_INFO")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("BOAT_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("SERIAL=",	szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("PART_ID=",	szRcvData, " ");
		sBody[5]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[6]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, "END");
		if(sBody[5] == "PASS")
		{
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[6]);
				alarm.mstr_svr_alarm = sBody[6];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
			}
		}
	}
	// 설비에서 레시피 정보가 변경되면 보고
	else if (sHeader == "TOOL_CHANGE")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("TOOLID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
	}
	// TMS 서버에 보고된 레시 정보 확인
	// - 설비의 레시피와 서버의 레시피 비교하기 위하여 설비 처음 시작 시 호출
	else if (sHeader == "TOOL_CHECK")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("EQP_ID=",			szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("CURRENT_TOOL_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("PART_TOOL_ID=",		szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("RESULT=",			szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("MSG=",				szRcvData, " ");
	}
	//==============================================================//
	// 필요하면 아무때나 사용 가능
	// (LOT_ID/SERIAL) 정보 중 둘중 하나로 요청
	// - 존재하는 정보만 입력하고 나머지는 공백으로 전송하면 됨
	// [Conv] 설비에서 사용됨
	else if (sHeader == "PACKING_MASTER_INFO")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",				szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("PART_ID=",				szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("LOT_STEP=",				szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("LOT_QTY=",				szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("S_BOX_MOQ=",				szRcvData, " ");
		sBody[5]	= Func.OnFindInRcvMsg("L_BOX_MOQ=",				szRcvData, " ");
		sBody[6]	= Func.OnFindInRcvMsg("PRODUCT_NAME=",			szRcvData, " ");
		sBody[7]	= Func.OnFindInRcvMsg("SSD_SIZE_X=",			szRcvData, " ");
		sBody[8]	= Func.OnFindInRcvMsg("SSD_SIZE_Y=",			szRcvData, " ");
		sBody[9]	= Func.OnFindInRcvMsg("TK_CODE=",				szRcvData, " ");
		// 추가됨
		sBody[10]	= Func.OnFindInRcvMsg("TK_BOX_LENGTH=",			szRcvData, " ");
		sBody[11]	= Func.OnFindInRcvMsg("TK_BOX_WIDTH=",			szRcvData, " ");
		sBody[12]	= Func.OnFindInRcvMsg("TK_BOX_HEIGHT=",			szRcvData, " ");
		sBody[13]	= Func.OnFindInRcvMsg("TRAY_HEIGHT=",			szRcvData, " ");
		sBody[14]	= Func.OnFindInRcvMsg("TRAY_MARKING=",			szRcvData, " ");
		sBody[15]	= Func.OnFindInRcvMsg("TRAY_ARRAY_X=",			szRcvData, " ");
		sBody[16]	= Func.OnFindInRcvMsg("TRAY_ARRAY_Y=",			szRcvData, " ");
		sBody[17]	= Func.OnFindInRcvMsg("POCKET_PITCH_X=",		szRcvData, " ");
		sBody[18]	= Func.OnFindInRcvMsg("POCKET_PITCH_Y=",		szRcvData, " ");
		sBody[19]	= Func.OnFindInRcvMsg("ACCE_MAT_CODE=",			szRcvData, " ");
		sBody[20]	= Func.OnFindInRcvMsg("INNER_BOX_MAT_CODE=",	szRcvData, " ");
		sBody[21]	= Func.OnFindInRcvMsg("INNER_BOX_LENGTH=",		szRcvData, " ");
		sBody[22]	= Func.OnFindInRcvMsg("INNER_BOX_WIDTH=",		szRcvData, " ");
		sBody[23]	= Func.OnFindInRcvMsg("INNER_BOX_HEIGHT=",		szRcvData, " ");
		sBody[24]	= Func.OnFindInRcvMsg("DTBT_LABEL_MAT_CODE=",	szRcvData, " ");
		sBody[25]	= Func.OnFindInRcvMsg("PP_BAND_MAT_CODE=",		szRcvData, " ");
		sBody[26]	= Func.OnFindInRcvMsg("PP_BAND_SPEC_NO=",		szRcvData, " ");
		sBody[27]	= Func.OnFindInRcvMsg("TAPE_MAT_CODE=",			szRcvData, " ");
		sBody[28]	= Func.OnFindInRcvMsg("TAPE_SPEC_NO=",			szRcvData, " ");
		sBody[29]	= Func.OnFindInRcvMsg("EMPTY_BOX_MAT_CODE=",	szRcvData, " ");
		sBody[30]	= Func.OnFindInRcvMsg("EMPTY_BOX_SPEC_NO=",		szRcvData, " ");
		sBody[31]	= Func.OnFindInRcvMsg("RIBBON_MAT_CODE=",		szRcvData, " ");
		sBody[32]	= Func.OnFindInRcvMsg("DENSITY_LABEL_MAT_CODE=",	szRcvData, " ");
		sBody[33]	= Func.OnFindInRcvMsg("DENSITY_LABEL_VALUE=",	szRcvData, " ");
		sBody[34]	= Func.OnFindInRcvMsg("RESULT=",				szRcvData, " ");
		sBody[35]	= Func.OnFindInRcvMsg("MSG=",					szRcvData, " ");


		bool bLot = false;
		for ( int i = 0; i < g_lotMgr.GetLotCount(); i++)
		{
			if(g_lotMgr.GetLotAt(i).GetLotID() == sBody[0])
			{
				bLot = true;
				break;
			}
		}
		
		if( bLot == false )
		{
			g_lotMgr.OnNewLotIn(sBody[0], sBody[1]);
			g_lotMgr.GetLotByLotID(sBody[0]).SetMdlCapa( sBody[1].Mid(5,3) + "GB" );
			st_basic.mstr_device_name.MakeUpper();
			int aa = st_basic.mstr_device_name.Find(".TXT", 0);
			if( aa <= 0 )
			{ 
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s can't find (.txt). check device name please", st_basic.mstr_device_name);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
				return RET_ERROR;
			}
			CString strname = st_basic.mstr_device_name;
			strname.Replace(".TXT", "");

			////2015.0714
			st_work.m_strCurModel = strname;
			////

			//if(sBody[6] == "2.5") strname = "25INCH";

// 			g_lotMgr.GetLotByLotID(sBody[0]).SetLabelVisionModel( strname, ("_" + g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa()) );
// 			
// 			if(sBody[1].GetAt(15) == 'A')//Only UPC Exist //America
// 			{
// 				if(g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("128GB") || 
// 					g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("256GB") ||
// 					g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("512GB"))
// 				{
// 					strname = "25INCHPRO";
// 				}
// 
// 				g_lotMgr.GetLotByLotID(sBody[0]).SetUpcEanType(true,false);
// 				g_lotMgr.GetLotByLotID(sBody[0]).SetTBtmVisionModel( strname, "_UPC" );
// 			}
// 			else if(sBody[1].GetAt(15) == 'E' || sBody[1].GetAt(15) == 'C' || sBody[1].GetAt(15) == 'K')//Europe,China,Korea Only EAN Exist
// 			{
// 				if(g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("128GB") || 
// 					g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("256GB") ||
// 					g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("512GB"))
// 				{
// 					strname = "25INCHPRO";
// 				}
// 				g_lotMgr.GetLotByLotID(sBody[0]).SetUpcEanType(false,true);	
// 				if(sBody[1].GetAt(15) == 'C')//china
// 				{
// 					g_lotMgr.GetLotByLotID(sBody[0]).SetTBtmVisionModel( strname, "_CHINA" );
// 				}
// 				else if(sBody[1].GetAt(15) == 'K')//china
// 				{
// 					g_lotMgr.GetLotByLotID(sBody[0]).SetTBtmVisionModel( strname, "_KOREA" );
// 				}
// 				else
// 				{
// 					g_lotMgr.GetLotByLotID(sBody[0]).SetTBtmVisionModel( strname, "" );
// 				}
// 			}
// 			else//Both of them exist
// 			{
// 				if(g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("128GB") || 
// 					g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("256GB") ||
// 					g_lotMgr.GetLotByLotID(sBody[0]).GetMdlCapa() == _T("512GB"))
// 				{
// 					strname = "25INCHPRO";
// 				}
// 				g_lotMgr.GetLotByLotID(sBody[0]).SetUpcEanType(true,true);
// 				g_lotMgr.GetLotByLotID(sBody[0]).SetTBtmVisionModel( strname, "" );
// 			}
			g_lotMgr.OnNewLotIn(sBody[0], sBody[1]);
			g_lotMgr.GetLotByLotID(sBody[0]).SetProductName(sBody[6]);
			g_lotMgr.GetLotByLotID(sBody[0]).SetLotEnd("NO");
			g_lotMgr.GetLotByLotID(sBody[0]).SetLotTime();
			g_lotMgr.GetLotByLotID(sBody[0]).SetDisLabelCode( sBody[24] );
			g_lotMgr.GetLotByLotID(sBody[0]).SetRibbonCode( sBody[31] );
			g_lotMgr.GetLotByLotID(sBody[0]).SetTotalModule( sBody[3] );
			g_lotMgr.GetLotByLotID(sBody[0]).SetDenLabelCode( sBody[32] );
			g_lotMgr.GetLotByLotID(sBody[0]).SetDenLabelCapa(sBody[33]);
		}

	}
	// Lot에 문제가 있어 배출 시 반드시 전송
	// [Conv] 설비에서 사용됨
	else if (sHeader == "PK_XCLOSE")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, "END");
		if(sBody[2] == "PASS")
		{
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[3]);
				alarm.mstr_svr_alarm = sBody[3];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
			}
			return RET_ERROR;			
		}
	}
	else if (sHeader == "BRAND_DENSITY_LABEL_CCS")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, "END");
		if(sBody[2] == "PASS")
		{
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[3]);
				alarm.mstr_svr_alarm = sBody[3];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
			}
		}
	}
	else if (sHeader == "BRAND_DTBT_LABEL_CCS")
	{
		// 특정한 문자열 사이의 데이터 반환
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, "END");
		if(sBody[2] == "PASS")
		{
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[3]);
				alarm.mstr_svr_alarm = sBody[3];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
			}
		}
	}
	else if (sHeader == "BRAND_DTBT_LABEL_ISSUE")
	{
		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
		sBody[1]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
		sBody[3]	= Func.OnFindInRcvMsg("SERIAL=",	szRcvData, " ");
		sBody[4]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, "END");

		CString strSerialID = Func.OnBodyAnalysis( 0, szRcvData, "SERIAL");
		CString strFormat = Func.OnBodyAnalysis( 3, szRcvData, "FORMAT");
		if(sBody[2] == "PASS")
		{
			if(sBody[3] == g_RunNetwork.st_barcodenet[CTL_LEFT].m_cSerial/*st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[CTL_LEFT]*/)
			{
				ClrSerial( CTL_LEFT );
				ClrBcrFormat( CTL_LEFT );
				SetSerial( CTL_LEFT, sBody[3] );
				SetBcrFormat( CTL_LEFT, strFormat );
			}
			else if(sBody[3] == g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cSerial/*st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[CTL_RIGHT]*/)
			{
				ClrSerial( CTL_RIGHT );
				ClrBcrFormat( CTL_RIGHT );
				SetSerial( CTL_RIGHT, sBody[3] );
				SetBcrFormat( CTL_RIGHT, strFormat );				
			}
			else
			{

				return RET_ERROR;
			}
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf( st_msg.c_abnormal_msg, "%s", sBody[4]);
				alarm.mstr_svr_alarm = sBody[4];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
			}
			return RET_ERROR;

		}
	}
// 	else if (sHeader == "BRAND_DTBT_LABEL_ISSUE_R_")
// 	{
// 		sBody[0]	= Func.OnFindInRcvMsg("LOT_ID=",	szRcvData, " ");
// 		sBody[1]	= Func.OnFindInRcvMsg("EQP_ID=",	szRcvData, " ");
// 		sBody[2]	= Func.OnFindInRcvMsg("RESULT=",	szRcvData, " ");
// 		sBody[3]	= Func.OnFindInRcvMsg("MSG=",		szRcvData, " ");
// 		CString strSerialID = Func.OnBodyAnalysis( 0, szRcvData, "SERIAL");
// 		CString strFormat = Func.OnBodyAnalysis( 3, szRcvData, "FORMAT");
// 		SetSerial( CTL_RIGHT, strSerialID );
// 		SetBcrFormat( CTL_RIGHT, strFormat );
// 	}
	else
	{
		return RET_ERROR;
	}
	return RET_GOOD;
}

//==================================================================//
// [EQP->MES] 메시지 전송
//==================================================================//
int CMesEvent::PushSendMsg(CString szMsg) 
{
	// 전송할 문자열 설정
	sprintf(st_client[m_nArray].ch_send, szMsg);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG +m_nArray, CLIENT_SEND, m_nArray);
	
	return 0;
}

//==================================================================//
// [EQP->TMS] 전송할 메시지 생성
//==================================================================//
void CMesEvent::OnCreate_SendFormat(int nzIdx) 
{
	CString sFunction;
	CString sHead = "", sBody = "", sTemp = "", sSRL = "";
	int i, nLoop;
	// 규이리 추가 [2014.12.14]
	CString sLotId, sBoatId, sSerial, sTboxid, sAcceid, sDensityid;

	// 규이리 추가 [2014.11.11]
	// - 통신이 중간에 에코 등이 없어 계속 대기중인 경우가 있어 추가함
	m_nStep_RUN = 0;

	switch(nzIdx)
	{
	case MES_REWORK_LABEL_CCS_INFO:
		sBody.Format("FUNCTION=REWORK_LABEL_CCS EQP_ID=%s SERIAL=%s DENSITY_MATL_CODE=%s TURNKEYBOX_MATL_CODE=%s OPER_ID=%s ",
			st_lamp.mstr_equip_id, st_mes_data.srt_SERIAL, st_mes_data.srt_DEN_LABEL_MAT_CODE, st_mes_data.srt_TURMKEY_BOX_CODE,"AUTO");
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "REWORK_LABEL_CCS_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "REWORK_LABEL_CCS_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;
		
		// 작업 공정 종료
		// [Conv] 설비에서 사용됨
	case MES_LOT_CLOSE:
		sBody.Format("FUNCTION=LOT_CLOSE LOT_ID=%s OPER_ID=%s GATE=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_LOT_ID, "AUTO", st_mes_data.srt_GATE, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "LOT_CLOSE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "LOT_CLOSE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// 자재의 바코드 정보 체크 (개별)
		// [Conv] 설비에서 사용됨
	case MES_PEBAG_CHECK:
		sBody.Format("FUNCTION=PEBAG_CHECK LOT_ID=%s SERIAL=%s PPID=%s WWN=%s C_SERIAL=%s PSID=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_SERIAL, st_mes_data.srt_PPID, st_mes_data.srt_WWN, st_mes_data.srt_C_SERIAL, st_mes_data.srt_PSID, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "PEBAG_CHECK";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "PEBAG_CHECK";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// 자재의 바코드 정보 체크 (트레이 단위)
		// [Conv] 설비에서 사용됨
	case MES_BRAND_PEBAG_CHECK:
		sBody.Format("FUNCTION=BRAND_PEBAG_CHECK LOT_ID=%s SSD_CNT=%s ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_SSD_CNT);
		// 자재 갯수만큼 반복
		nLoop = atoi(st_mes_data.srt_SSD_CNT);
		for(i=0; i<nLoop; i++)
		{
			sBody += st_mes_data.srt_SRL[i];
		}
		sTemp.Format("RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sBody += sTemp;
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BRAND_PEBAG_CHECK";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BRAND_PEBAG_CHECK";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// (PEBAG_CHECK) 완료 보고
		// - 맨 마지막 트레이에 대한 (PEBAG_CHECK) 후에 전송
		// [Conv] 설비에서 사용됨
	case MES_PEBAG_COMPLETE:
		sBody.Format("FUNCTION=PEBAG_COMPLETE LOT_ID=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "PEBAG_COMPLETE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "PEBAG_COMPLETE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		//==========================================================//
		// 메인 설비에서 사용
		//==========================================================//
		// 설비에서 Id 버퍼 흘러가는 상태 보고
		// New->(change)->End 형식으로 보고
		// - New : Id 버퍼에 자재 내려놓은 경우
		// - Change : Id 버퍼가 변경되거나 해당 자재 시리얼 정보가 변경될 경우
		// - End : 하류 설비로 흘려보낼 경우
	case MES_BOAT_ID_MODIF:
		sBody.Format("FUNCTION=BOAT_ID_MODIF LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s MODIF_MODE=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_BOAT_ID, st_lamp.mstr_equip_id, st_mes_data.srt_SERIAL, st_mes_data.srt_MODIF_MODE, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// 규이리 추가 [2014.12.14]
	case MES_BOAT_ID_MODIF_LIN_:
		sLotId.Format("%s",		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[CTL_LEFT]);
		sBoatId.Format("%d",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[CTL_LEFT]);
		sSerial.Format("%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[CTL_LEFT]);

		sBody.Format("FUNCTION=BOAT_ID_MODIF LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s MODIF_MODE=NEW RETRY=NO RETRY_CNT=0 ",
			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

	case MES_BOAT_ID_MODIF_RIN_:
		sLotId.Format("%s",		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[CTL_RIGHT]);
		sBoatId.Format("%d",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[CTL_RIGHT]);
		sSerial.Format("%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[CTL_RIGHT]);
		
		sBody.Format("FUNCTION=BOAT_ID_MODIF LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s MODIF_MODE=NEW RETRY=NO RETRY_CNT=0 ",
			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

	case MES_BOAT_ID_MODIF_LOUT_:
		sLotId.Format("%s",		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_lot_id[CTL_LEFT]);
		sBoatId.Format("%d",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[CTL_LEFT]);
		sSerial.Format("%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[CTL_LEFT]);
		
		sBody.Format("FUNCTION=BOAT_ID_MODIF LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s MODIF_MODE=END RETRY=NO RETRY_CNT=0 ",
			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;
		
	case MES_BOAT_ID_MODIF_ROUT_:
		sLotId.Format("%s",		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_lot_id[CTL_RIGHT]);
		sBoatId.Format("%d",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[CTL_RIGHT]);
		sSerial.Format("%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[CTL_RIGHT]);
		
		sBody.Format("FUNCTION=BOAT_ID_MODIF LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s MODIF_MODE=END RETRY=NO RETRY_CNT=0 ",
			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BOAT_ID_MODIF";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// 작업자 확인용
		// - 시리얼 정보는 아는데 버퍼 정보를 모를 경우에 사용
	case MES_PROD_SERIAL_INFO:
		sBody.Format("FUNCTION=PROD_SERIAL_INFO EQP_ID=%s SERIAL=%s RETRY=%s RETRY_CNT=%s ",
			st_lamp.mstr_equip_id, st_mes_data.srt_SERIAL, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "PROD_SERIAL_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "PROD_SERIAL_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// 작업자 확인용
		// - 해당 버퍼에 어떤 자재가 들어있는지 확인 시 사용
	case MES_BOAT_ID_INFO:
		sBody.Format("FUNCTION=BOAT_ID_INFO BOAT_ID=%s EQP_ID=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_BOAT_ID, st_lamp.mstr_equip_id, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BOAT_ID_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BOAT_ID_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// 설비에서 레시피 정보가 변경되면 보고
	case MES_TOOL_CHANGE:
		sBody.Format("FUNCTION=TOOL_CHANGE EQP_ID=%s TOOLID=%s RETRY=%s RETRY_CNT=%s ",
			st_lamp.mstr_equip_id, st_mes_data.srt_TOOLID, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "TOOL_CHANGE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "TOOL_CHANGE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;
		
		// TMS 서버에 보고된 레시 정보 확인
		// - 설비의 레시피와 서버의 레시피 비교하기 위하여 설비 처음 시작 시 호출
	case MES_TOOL_CHECK:
		sBody.Format("FUNCTION=TOOL_CHECK EQP_ID=%s LOT_ID=%s RETRY=%s RETRY_CNT=%s ",
			st_lamp.mstr_equip_id, st_mes_data.srt_LOT_ID, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "TOOL_CHECK";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "TOOL_CHECK";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;
		//==========================================================//

		// 필요하면 아무때나 사용 가능
		// (LOT_ID/SERIAL) 정보 중 둘중 하나로 요청
		// - 존재하는 정보만 입력하고 나머지는 공백으로 전송하면 됨
		// [Conv] 설비에서 사용됨
	case MES_PACKING_MASTER_INFO:
		sBody.Format("FUNCTION=PACKING_MASTER_INFO LOT_ID=%s SERIAL=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_SERIAL, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "PACKING_MASTER_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "PACKING_MASTER_INFO";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

		// Lot에 문제가 있어 배출 시 반드시 전송
		// [Conv] 설비에서 사용됨
	case MES_PK_XCLOSE:
		sBody.Format("FUNCTION=PK_XCLOSE LOT_ID=%s OPER_ID=%s EQP_ID=%s RETRY=%s RETRY_CNT=%s ",
			st_mes_data.srt_LOT_ID, "AUTO", st_lamp.mstr_equip_id, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);

		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "PK_XCLOSE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "PK_XCLOSE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

	case MES_BRAND_DENSITY_LABEL_CCS:
		sBody.Format("FUNCTION=BRAND_DENSITY_LABEL_CCS LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s DEN_LABEL_MAT_CODE=%s  RETRY=NO RETRY_CNT=0 ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_BOAT_ID, st_lamp.mstr_equip_id, st_mes_data.srt_C_SERIAL, st_mes_data.srt_DEN_LABEL_MAT_CODE);//, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BRAND_DENSITY_LABEL_CCS";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BRAND_DENSITY_LABEL_CCS";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

	case MES_BRAND_DTBT_LABEL_CCS:
		sBody.Format("FUNCTION=BRAND_DTBT_LABEL_CCS LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s DTBT_MAT_CODE=%s RIBBON_MAT_CODE=%s RETRY=NO RETRY_CNT=0 ",
			st_mes_data.srt_LOT_ID, st_mes_data.srt_BOAT_ID, st_lamp.mstr_equip_id, st_mes_data.srt_C_SERIAL, st_mes_data.srt_DTBT_MAT_CODE, st_mes_data.srt_RIBBON_MAT_CODE);//, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BRAND_DTBT_LABEL_CCS";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BRAND_DTBT_LABEL_CCS";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

	case MES_BRAND_DTBT_LABEL_ISSUE_L_:
// 		sLotId.Format("%s",		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[CTL_LEFT]);
// 		sBoatId.Format("%d",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[CTL_LEFT]);
// 		sSerial.Format("%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[CTL_LEFT]);
		sLotId.Format("%s",		g_RunNetwork.st_barcodenet[CTL_LEFT].m_cLotid);
		sBoatId.Format("%s",	g_RunNetwork.st_barcodenet[CTL_LEFT].m_cBoatid);
		sSerial.Format("%s",	g_RunNetwork.st_barcodenet[CTL_LEFT].m_cSerial);
		sDensityid.Format("%s",	g_RunNetwork.st_barcodenet[CTL_LEFT].m_cDencode);
		sTboxid.Format("%s",	g_RunNetwork.st_barcodenet[CTL_LEFT].m_cTboxcode);
		sAcceid.Format("%s",	g_RunNetwork.st_barcodenet[CTL_LEFT].m_cAccecode);

// 		sBody.Format("FUNCTION=BRAND_DTBT_LABEL_ISSUE LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s DTBT_LABEL_CODE=%s DTBT_RIBBON_CODE=%s MODE=YES RETRY=%s RETRY_CNT=%s ",
// 			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial, st_mes_data.srt_DTBT_LABEL_CODE, st_mes_data.srt_DTBT_RIBBON_CODE, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
//2015.0322		
		sBody.Format("FUNCTION=BRAND_DTBT_LABEL_ISSUE LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s DTBT_LABEL_CODE=%s DTBT_RIBBON_CODE=%s DENS_CODE=%s TURNKEY_CODE=%s TRAY_CODE=%s MODE=YES RETRY=%s RETRY_CNT=%s ",
			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial, st_mes_data.srt_DTBT_LABEL_CODE, st_mes_data.srt_DTBT_RIBBON_CODE, sDensityid, sTboxid, sAcceid, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BRAND_DTBT_LABEL_ISSUE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BRAND_DTBT_LABEL_ISSUE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;

	case MES_BRAND_DTBT_LABEL_ISSUE_R_:
// 		sLotId.Format("%s",		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[CTL_RIGHT]);
// 		sBoatId.Format("%d",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[CTL_RIGHT]);
// 		sSerial.Format("%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[CTL_RIGHT]);
		sLotId.Format("%s",		g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cLotid);
		sBoatId.Format("%s",	g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cBoatid);
		sSerial.Format("%s",	g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cSerial);
		sDensityid.Format("%s",	g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cDencode);
		sTboxid.Format("%s",	g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cTboxcode);
		sAcceid.Format("%s",	g_RunNetwork.st_barcodenet[CTL_RIGHT].m_cAccecode);
		
// 		sBody.Format("FUNCTION=BRAND_DTBT_LABEL_ISSUE LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s DTBT_LABEL_CODE=%s DTBT_RIBBON_CODE=%s MODE=YES RETRY=%s RETRY_CNT=%s ",
// 			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial, st_mes_data.srt_DTBT_LABEL_CODE, st_mes_data.srt_DTBT_RIBBON_CODE, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);

//2015.0322	
		sBody.Format("FUNCTION=BRAND_DTBT_LABEL_ISSUE LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s DTBT_LABEL_CODE=%s DTBT_RIBBON_CODE=%s DENS_CODE=%s TURNKEY_CODE=%s TRAY_CODE=%s MODE=YES RETRY=%s RETRY_CNT=%s ",
			sLotId, sBoatId, st_lamp.mstr_equip_id, sSerial, st_mes_data.srt_DTBT_LABEL_CODE, st_mes_data.srt_DTBT_RIBBON_CODE, sDensityid, sTboxid, sAcceid, st_mes_data.srt_RETRY, st_mes_data.srt_RETRY_CNT);
		
		sHead.Format("%016d ", sBody.GetLength());
		sprintf(st_client[m_nArray].chSendHead, sHead);
		sprintf(st_client[m_nArray].chSendBody, sBody);
		
		// 통신 관련 기본 정보 설정
		st_client[m_nArray].nIndex = nzIdx;
		sFunction = "BRAND_DTBT_LABEL_ISSUE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BRAND_DTBT_LABEL_ISSUE";
		sprintf((st_client[m_nArray].stComm[nzIdx]).chResponse, sFunction);
		(st_client[m_nArray].stComm[nzIdx]).nEcho		= 11;
		(st_client[m_nArray].stComm[nzIdx]).nResponse	= 10;
		(st_client[m_nArray].stComm[nzIdx]).nTimeOut	= 1;
		break;	
	}
}
