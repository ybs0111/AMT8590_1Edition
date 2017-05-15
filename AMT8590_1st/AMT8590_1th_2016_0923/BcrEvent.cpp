// BcrEvent.cpp: implementation of the CBcrEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BcrEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBcrEvent	evBCR;

CBcrEvent::CBcrEvent()
{

}

CBcrEvent::~CBcrEvent()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CBcrEvent::OnSeq_RUN(void) 
{
	OnSeq_INIT();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CBcrEvent::OnSeq_INIT(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;
	
	// 스텝 정보 초기화
	m_nStep_LSSD	= 0;
	m_nStep_RSSD	= 0;
	m_nStep_TRIN	= 0;
	m_nStep_TROUT	= 0;

	int i=0;
	for(i=0; i<MAX_PORT; i++)
	{
		m_sRcvedData[i].Empty();	// 수신 메시지 버퍼 초기화
	}
}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CBcrEvent::OnSeq_Execute(void) 
{
	if (m_nReq_LSSD == REQ_CALL_)
	{
		OnRun_LSSD();
	}
	else if (m_nReq_RSSD == REQ_CALL_)
	{
		OnRun_RSSD();
	}
	else if (m_nReq_TRIN == REQ_CALL_ ||
		m_nReq_TRIN == REQ_STOP_)
	{
		OnRun_TRIN();
	}
	else if (m_nReq_TROUT == REQ_CALL_ ||
		m_nReq_TROUT == REQ_STOP_)
	{
		OnRun_TROUT();
	}
}

int CBcrEvent::OnRun_LSSD() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	CString sTemp;

	switch(m_nStep_LSSD)
	{
	case 0:
		// BCR 읽기 요청 전송
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_LSSD_, 0);

		// st_serial.n_rec_chk[nzPort -1]	= FALSE;
		// 위 부분은 메인 프레임에서 메시지 전송하면서 설정하게 됨
		// - OnSend_BcrRead() 함수 참조
		// 메시지 전송 시작 시간 설정
		m_lWait_LSSD[0] = GetCurrentTime();
		m_nStep_LSSD = 1100;
		break;

	case 1100:
		// 응답을 기다림
		if (st_serial.mn_rec_chk[PORT_BCR_LSSD_ -1] == CTL_YES)
		{
			sTemp = st_serial.mcomm_rec[PORT_BCR_LSSD_ -1];
			// 읽기 작업 실패했는지 검사
			if (sTemp == "No-Read")
			{
				m_nStep_LSSD = 5000;	// 에러 반환 스텝
			}
			else
			{
				m_nStep_LSSD = 4000;
			}
		}
		else
		{
			m_lWait_LSSD[1] = GetCurrentTime();
			m_lWait_LSSD[2] = m_lWait_LSSD[1] - m_lWait_LSSD[0];
			if (m_lWait_LSSD[2] < 0)
			{
				m_lWait_LSSD[0] = GetCurrentTime();
				break;
			}
			
			// 2초 동안 기다림
			if (m_lWait_LSSD[2] > 2000)
			{
				m_nStep_LSSD = 5000;	// 에러 반환 스텝
			}
		}
		break;

	case 4000:
		m_nReq_LSSD	= REPLY_CORRECT_;
		
		m_nStep_LSSD = 0;
		FuncRet = RET_GOOD;
		break;
		
		// 에러 반환 스텝
	case 5000:
		m_nReq_LSSD	= REPLY_ERROR_;
		
		m_nStep_LSSD = 0;
		FuncRet = RET_ERROR;
		break;
	}
	return FuncRet;
}

int CBcrEvent::OnRun_RSSD() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;
	
	CString sTemp;
	
	switch(m_nStep_RSSD)
	{
	case 0:
		// BCR 읽기 요청 전송
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_RSSD_, 0);
		
		// st_serial.n_rec_chk[nzPort -1]	= FALSE;
		// 위 부분은 메인 프레임에서 메시지 전송하면서 설정하게 됨
		// - OnSend_BcrRead() 함수 참조
		// 메시지 전송 시작 시간 설정
		m_lWait_RSSD[0] = GetCurrentTime();
		m_nStep_RSSD = 1100;
		break;
		
	case 1100:
		// 응답을 기다림
		if (st_serial.mn_rec_chk[PORT_BCR_RSSD_ -1] == CTL_YES)
		{
			sTemp = st_serial.mcomm_rec[PORT_BCR_RSSD_ -1];
			// 읽기 작업 실패했는지 검사
			if (sTemp == "No-Read")
			{
				m_nStep_RSSD = 5000;	// 에러 반환 스텝
			}
			else
			{
				m_nStep_RSSD = 4000;
			}
		}
		else
		{
			m_lWait_RSSD[1] = GetCurrentTime();
			m_lWait_RSSD[2] = m_lWait_RSSD[1] - m_lWait_RSSD[0];
			if (m_lWait_RSSD[2] < 0)
			{
				m_lWait_RSSD[0] = GetCurrentTime();
				break;
			}
			
			// 2초 동안 기다림
			if (m_lWait_RSSD[2] > 2000)
			{
				m_nStep_RSSD = 5000;	// 에러 반환 스텝
			}
		}
		break;
		
	case 4000:
		m_nReq_RSSD	= REPLY_CORRECT_;
		
		m_nStep_RSSD = 0;
		FuncRet = RET_GOOD;
		break;
		
		// 에러 반환 스텝
	case 5000:
		m_nReq_RSSD	= REPLY_ERROR_;
		
		m_nStep_RSSD = 0;
		FuncRet = RET_ERROR;
		break;
	}
	return FuncRet;
}

// 포트 열림 상태 검사는 메인 프레임의 전송 함수에서 담당함
int CBcrEvent::OnRun_TRIN() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	CString sTemp;
	int i=0, nPos;

	switch(m_nStep_TRIN)
	{
	case 0:
		// [1:BCR Continue]
		// (Self TEST) 모드 추가함
		// - 이 모드에서는 통신 메시지 날릴 필요 없음
		// - 바코드 위치로 이동하면 자동으로 읽어서 해당 정보 전송해줌
		if (st_basic.nMode_BCR == 1)
		{
			// 입력되는 데이터를 받아들이기 시작하라고 요청함
			// - 아무때나 받아들이면 쓰레기 정보를 받아들일 수 있기 때문
			st_serial.nDataAccept = REQ_ACCEPT_;
			// 데이터를 저장할 배열 위치 정보 초기화
			OnReset_WorkPosTRIN();
			
			// 모터 이동을 시작한 상태임
			m_nStep_TRIN = 2100;
		}
		// [0:BCR Step]
		else
		{
			// BCR 읽기 요청 전송
			::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_TRIN_, 0);
			
			// st_serial.n_rec_chk[nzPort -1]	= FALSE;
			// 위 부분은 메인 프레임에서 메시지 전송하면서 설정하게 됨
			// - OnSend_BcrRead() 함수 참조
			// 메시지 전송 시작 시간 설정
			m_lWait_TRIN[0] = GetCurrentTime();
			m_nStep_TRIN = 1100;
		}
		break;

	case 1100:
		// 응답을 기다림
		if (st_serial.mn_rec_chk[PORT_BCR_TRIN_ -1] == CTL_YES)
		{
			sTemp = st_serial.mcomm_rec[PORT_BCR_TRIN_ -1];
			// 읽기 작업 실패했는지 검사
			if (sTemp == "No-Read")
			{
				m_nStep_TRIN = 5000;	// 에러 반환 스텝
			}
			else
			{
				m_nStep_TRIN = 4000;
			}
		}
		else
		{
			m_lWait_TRIN[1] = GetCurrentTime();
			m_lWait_TRIN[2] = m_lWait_TRIN[1] - m_lWait_TRIN[0];
			if (m_lWait_TRIN[2] < 0)
			{
				m_lWait_TRIN[0] = GetCurrentTime();
				break;
			}

			// 2초 동안 기다림
			if (m_lWait_TRIN[2] > 2000)
			{
				m_nStep_TRIN = 5000;	// 에러 반환 스텝
			}
		}
		break;

	case 2100:
		// 모터 이동을 완료한 상태임
		if (m_nReq_TRIN == REQ_STOP_)
		{
			// 마지막 바코드 읽을 때까지 기다리기 위한 시간 설정
			m_lWait_TRIN[0] = GetCurrentTime();
			m_nStep_TRIN = 2200;
		}
		break;

	case 2200:
		// 마지막 바코드 읽기 위한 시간 딜레이
		m_lWait_TRIN[1] = GetCurrentTime();
		m_lWait_TRIN[2] = m_lWait_TRIN[1] - m_lWait_TRIN[0];
		if (m_lWait_TRIN[2] < 0)
		{
			m_lWait_TRIN[0] = GetCurrentTime();
			break;
		}

		if (m_lWait_TRIN[2] > 200)
		{
			// 입력되는 바코드 정보 무시하라고 설정
			st_serial.nDataAccept = REQ_FREE_;
			m_nStep_TRIN = 2300;
		}
		break;

	case 2300:
		// 모든 바코드 정보 읽음
		if (m_nPos_TRIN >= (ID_BUFF_CNT_ -1))
		{
			// 읽은 바코드 갯수만큼 반복
			for(i=0; i<ID_BUFF_CNT_; i++)
			{
				sTemp.Format("%s", stBCR[PORT_BCR_TRIN_ -1].stMat[i].chData_IDBUF);
				// 바코드 정보 중에 오류가 발생한 곳이 존재하는지 확인
				nPos = sTemp.Find("No-Read");
				if (nPos != -1)
				{
					m_nStep_TRIN = 5000;
					break;
				}
			}

			m_nStep_TRIN = 4000;
		}
		// 모든 바코드 정보 읽지 못함 (에러)
		else
		{
			m_nStep_TRIN = 5000;
		}
		break;

	case 4000:
		m_nReq_TRIN	= REPLY_CORRECT_;

		m_nStep_TRIN = 0;
		FuncRet = RET_GOOD;
		break;

		// 에러 반환 스텝
	case 5000:
		m_nReq_TRIN	= REPLY_ERROR_;

		m_nStep_TRIN = 0;
		FuncRet = RET_ERROR;
		break;
	}
	return FuncRet;
}

int CBcrEvent::OnRun_TROUT() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	CString sTemp;
	int i=0, nPos;

	switch(m_nStep_TROUT)
	{
	case 0:
		// [1:BCR Continue]
		// (Self TEST) 모드 추가함
		// - 이 모드에서는 통신 메시지 날릴 필요 없음
		// - 바코드 위치로 이동하면 자동으로 읽어서 해당 정보 전송해줌
		if (st_basic.nMode_BCR == 1)
		{
			// 입력되는 데이터를 받아들이기 시작하라고 요청함
			// - 아무때나 받아들이면 쓰레기 정보를 받아들일 수 있기 때문
			st_serial.nDataAccept = REQ_ACCEPT_;
			// 데이터를 저장할 배열 위치 정보 초기화
			OnReset_WorkPosTROUT();
			
			// 모터 이동을 시작한 상태임
			m_nStep_TROUT = 2100;
		}
		// [0:BCR Step]
		else
		{
			// BCR 읽기 요청 전송
			::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_TROUT_, 0);
			
			// st_serial.n_rec_chk[nzPort -1]	= FALSE;
			// 위 부분은 메인 프레임에서 메시지 전송하면서 설정하게 됨
			// - OnSend_BcrRead() 함수 참조
			// 메시지 전송 시작 시간 설정
			m_lWait_TROUT[0] = GetCurrentTime();
			m_nStep_TROUT = 1100;
		}
		break;

	case 1100:
		// 응답을 기다림
		if (st_serial.mn_rec_chk[PORT_BCR_TROUT_ -1] == CTL_YES)
		{
			sTemp = st_serial.mcomm_rec[PORT_BCR_TROUT_ -1];
			// 읽기 작업 실패했는지 검사
			if (sTemp == "No-Read")
			{
				m_nStep_TROUT = 5000;	// 에러 반환 스텝
			}
			else
			{
				m_nStep_TROUT = 4000;
			}
		}
		else
		{
			m_lWait_TROUT[1] = GetCurrentTime();
			m_lWait_TROUT[2] = m_lWait_TROUT[1] - m_lWait_TROUT[0];
			if (m_lWait_TROUT[2] < 0)
			{
				m_lWait_TROUT[0] = GetCurrentTime();
				break;
			}

			// 2초 동안 기다림
			if (m_lWait_TROUT[2] > 2000)
			{
				m_nStep_TROUT = 5000;	// 에러 반환 스텝
			}
		}
		break;

	case 2100:
		// 모터 이동을 완료한 상태임
		if (m_nReq_TROUT == REQ_STOP_)
		{
			// 마지막 바코드 읽을 때까지 기다리기 위한 시간 설정
			m_lWait_TROUT[0] = GetCurrentTime();
			m_nStep_TROUT = 2200;
		}
		break;

	case 2200:
		// 마지막 바코드 읽기 위한 시간 딜레이
		m_lWait_TROUT[1] = GetCurrentTime();
		m_lWait_TROUT[2] = m_lWait_TROUT[1] - m_lWait_TROUT[0];
		if (m_lWait_TROUT[2] < 0)
		{
			m_lWait_TROUT[0] = GetCurrentTime();
			break;
		}

		if (m_lWait_TROUT[2] > 200)
		{
			// 입력되는 바코드 정보 무시하라고 설정
			st_serial.nDataAccept = REQ_FREE_;
			m_nStep_TROUT = 2300;
		}
		break;

	case 2300:
		// 모든 바코드 정보 읽음
		if (m_nPos_TROUT >= (ID_BUFF_CNT_ -1))
		{
			// 읽은 바코드 갯수만큼 반복
			for(i=0; i<ID_BUFF_CNT_; i++)
			{
				sTemp.Format("%s", stBCR[PORT_BCR_TROUT_ -1].stMat[i].chData_IDBUF);
				// 바코드 정보 중에 오류가 발생한 곳이 존재하는지 확인
				nPos = sTemp.Find("No-Read");
				if (nPos != -1)
				{
					m_nStep_TROUT = 5000;
					break;
				}
			}

			m_nStep_TROUT = 4000;
		}
		// 모든 바코드 정보 읽지 못함 (에러)
		else
		{
			m_nStep_TROUT = 5000;
		}
		break;

	case 4000:
		m_nReq_TROUT	= REPLY_CORRECT_;

		m_nStep_TROUT = 0;
		FuncRet = RET_GOOD;
		break;

		// 에러 반환 스텝
	case 5000:
		m_nReq_TROUT	= REPLY_ERROR_;

		m_nStep_TROUT = 0;
		FuncRet = RET_ERROR;
		break;
	}
	return FuncRet;
}

//==================================================================//
// [EQP<-BCR] 바코드 리더기로부터 메시지 수신
//==================================================================//
void CBcrEvent::OnRcvDataFromBCR(WPARAM ch, LPARAM port) 
{
	// 포트 정보 얻음
	int nPort = (int)port;
	int nLen;

	// 마지막 문자 입력 [CR] 입력인지 확인
	if (ch == 0x0a)
	{
		m_sRcvedData[nPort -1] += (char)ch;
		//m_sRcvedData[nPort-1].TrimLeft(' ');
		//m_sRcvedData[nPort-1].TrimRight(' ');
		st_serial.mcomm_rec[nPort -1] = m_sRcvedData[nPort -1];
		(m_sRcvedData[nPort -1]).Empty();

		m_sLogMsg.Format( "[RCV]BCR:%s", st_serial.mcomm_rec[nPort -1]);
		Func.On_LogFile_Add(99, m_sLogMsg);
		

		// 수신 메시지의 전체 길이 계산
		// 최소 3글자 이상 수신되어야 정상적인 메시지로 인정
		nLen = (st_serial.mcomm_rec[nPort -1]).GetLength();
		if (nLen > 2)
		{
			// CR+LF 제거한 문자열만 저장함
			st_serial.mcomm_rec[nPort -1] = (st_serial.mcomm_rec[nPort-1]).Mid(0, nLen-2);

			OnSet_2Struct(nPort);	// 구조체에 수신된 메시지 저장
			st_serial.mn_rec_chk[nPort -1] = TRUE;
		}
		else
		{
			st_serial.mn_rec_chk[nPort -1] = FALSE;
		}

		// 화면에 수신 메시지 출력 요청
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, st_serial.mcomm_rec[nPort -1]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, BCR_PORT, RECEIVE_MSG);
		}
	}
	else
	{
		m_sRcvedData[port -1] += (char)ch;
	}
}

//==================================================================//
// 구조체에 수신된 메시지 저장
//==================================================================//
void CBcrEvent::OnSet_2Struct(int nzPort) 
{
	if (nzPort == PORT_BCR_TRIN_)
	{
		// [1:BCR Continue]
		if (st_basic.nMode_BCR == 1)
		{
			if (st_serial.nDataAccept == REQ_ACCEPT_)
			{
				sprintf(stBCR[nzPort -1].stMat[m_nPos_TRIN].chData_IDBUF, st_serial.mcomm_rec[nzPort -1]);
				
				// 다음에 읽은 정보를 저장할 배열 위치 계산
				(m_nPos_TRIN)++;
				// 한번에 읽어야 할 (ID 버퍼) 갯수는 최대 2개임
				if ((m_nPos_TRIN) > (ID_BUFF_CNT_ -1))
				{
					m_nPos_TRIN = (ID_BUFF_CNT_ -1);
				}
			}
		}
		// [0:BCR Step]
		else
		{
			sprintf(stBCR[nzPort -1].stMat[m_nPos_TRIN].chData_IDBUF, st_serial.mcomm_rec[nzPort -1]);
			
			// 다음에 읽은 정보를 저장할 배열 위치 계산
			(m_nPos_TRIN)++;
			// 한번에 읽어야 할 (ID 버퍼) 갯수는 최대 2개임
			if ((m_nPos_TRIN) > (ID_BUFF_CNT_ -1))
			{
				m_nPos_TRIN = (ID_BUFF_CNT_ -1);
			}
		}
	}
	else if (nzPort == PORT_BCR_TROUT_)
	{
		// [1:BCR Continue]
		if (st_basic.nMode_BCR == 1)
		{
			if (st_serial.nDataAccept == REQ_ACCEPT_)
			{
				sprintf(stBCR[nzPort -1].stMat[m_nPos_TROUT].chData_IDBUF, st_serial.mcomm_rec[nzPort -1]);
				
				// 다음에 읽은 정보를 저장할 배열 위치 계산
				(m_nPos_TROUT)++;
				// 한번에 읽어야 할 (ID 버퍼) 갯수는 최대 2개임
				if ((m_nPos_TROUT) > (ID_BUFF_CNT_ -1))
				{
					m_nPos_TROUT = (ID_BUFF_CNT_ -1);
				}
			}
		}
		// [0:BCR Step]
		else
		{
			sprintf(stBCR[nzPort -1].stMat[m_nPos_TROUT].chData_IDBUF, st_serial.mcomm_rec[nzPort -1]);
			
			// 다음에 읽은 정보를 저장할 배열 위치 계산
			(m_nPos_TROUT)++;
			// 한번에 읽어야 할 (ID 버퍼) 갯수는 최대 2개임
			if ((m_nPos_TROUT) > (ID_BUFF_CNT_ -1))
			{
				m_nPos_TROUT = (ID_BUFF_CNT_ -1);
			}
		}
	}
	else if (nzPort == PORT_BCR_LSSD_ ||
		nzPort == PORT_BCR_RSSD_)
	{
		sprintf(stBCR[nzPort -1].stMat[0].chData_SSD, st_serial.mcomm_rec[nzPort -1]);
	}
	// 매뉴얼 동작으로 (ID 버퍼)와 자재를 읽기 위해서는 다음 순서대로 읽어야 함
	// - 1) (ID 버퍼) 읽음
	// - 2) 자재 읽음
	else if (nzPort == PORT_BCR_MANUAL_)
	{
		// USB 타입으로 선정한다고 하여 삭제함
	}

	// 읽은 데이터를 화면에 보여주기 위해서는 아래 부분 수정
	//st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_CARRIER_MARK, 0);
}
