// XGemClient.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "XGemClient.h"
#include "Dialog_Title.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXGemClient
CXGemClient g_xgem_client;

CXGemClient::CXGemClient()
{
	m_nPort = 5;
}

CXGemClient::~CXGemClient()
{
}

/////////////////////////////////////////////////////////////////////////////
// CXGemClient message handlers
void CXGemClient::XgemInitialize()
{
	CString strHead="", strFunc="", strGemInit="";

	CString strPath = "C:\\XGEM\\EqSample.cfg";
	
	strFunc = "FUNC=COMMUNICATION,CONFIG=" + strPath + ",";
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);
	
	sprintf(st_client[m_nPort].ch_send, strHead + strFunc );
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);

// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);

	m_bGemInit = false;
}

void CXGemClient::XgemEqStatus(int nMode)
{
	CString strHead="", strFunc="";
	
	switch(nMode)
	{
	case 0:
		strFunc = "FUNC=OPERATE,CONNECT=0,";
		break;
		
	case 1:
		strFunc = "FUNC=OPERATE,CONNECT=1,";
		break;
		
	case 2:
		strFunc = "FUNC=OPERATE,CONNECT=2,";
		break;
	}
	
	strHead.Format( "HD=%06d,", strFunc.GetLength() + 10 );
	
	sprintf(st_client[m_nPort].ch_send, strHead + strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);

	m_nCnt_GemState++;
}

void CXGemClient::XgemMachineStatus()
{
	CString strHead="", strFunc="";

	strFunc.Format("FUNC=RUN_STATUS,STATUS=%d,", m_nGemMachineState);
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);
	
	sprintf(st_client[m_nPort].ch_send, strHead + strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	m_nCnt_GemMachineState++;

	m_bGemMachineState = false;
}

void CXGemClient::XgemLotStart()
{
	CString strHead="", strFunc="";	

	strFunc.Format("FUNC=CEID,CEID_NUM=1002,REPORT=1,REP_01={CNT=03,[DA_01=01,%s],[DA_02=01,%s],[DA_03=01,%s]},", st_lamp.mstr_equip_id, m_strSPartid, m_strSLotid);
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);
	
	sprintf(st_client[m_nPort].ch_send, strHead + strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);


	m_bGemLotStart = false;

}

void CXGemClient::XgemLotEnd()
{
	CString strHead="", strFunc="";
	
	strFunc.Format("FUNC=CEID,CEID_NUM=1003,REPORT=1,REP_01={CNT=07,[DA_01=01,%s],[DA_02=01,%s],[DA_03=01,%s],[DA_04=01,%s],[DA_05=01,%s],[DA_06=01,%s]},",
		st_lamp.mstr_equip_id, m_strSPartid, m_strSLotid, st_xgem.m_lTotCnt, st_xgem.m_lPassCnt, st_xgem.m_lFailCnt );
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);

	
	sprintf(st_client[m_nPort].ch_send, strHead + strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);

	m_bGemLotEnd = false;
}

void CXGemClient::XgemSendPPID()
{
	CString strHead="", strFunc="", strPPid="", strRecipe="";	

	strRecipe = st_basic.mstr_device_name;//. mstr_revipe_name + ".txt";
	strRecipe.Replace(".TXT", "");
// 	int n_pos = strRecipe.Find(".");  // 확장자 위치 검사
// 	
// 	if (n_pos == -1)
// 	{
// 		strRecipe += _T(".txt");  // 확장자 추가
// 	}
// 	else
// 	{
// 		strRecipe.Replace(".TXT", "");
// 	}
	strFunc.Format( "FUNC=PPID_CHANGE,PP_NAME=%s,PP_PATH=%s%s,", strRecipe, st_path.mstr_recipe_path, st_basic.mstr_device_name);

	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);	
	sprintf(st_client[m_nPort].ch_send, strHead+strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);

	::Sleep(1000);

	strFunc.Format("FUNC=CEID,CEID_NUM=1100,REPORT=1,REP_01={CNT=02,[DA_01=01,%s],[DA_02=01,%s]},", st_lamp.mstr_equip_id, st_basic.mstr_revipe_name);
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);

	sprintf(st_client[m_nPort].ch_send, strHead+strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);


	m_bGemPPid = false;
}

void CXGemClient::XgemAlarmStatus()
{
	CString strHead="", strFunc="";
	
	
	strFunc.Format("FUNC=ALARM,CODE=%s,STATUS=%d,", m_strAlarmCode, m_nAlarmState);
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);
	
	sprintf(st_client[m_nPort].ch_send, strHead);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
	sprintf(st_client[m_nPort].ch_send, strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	m_bGemAlarmState=false;
}

void CXGemClient::XgemBufferMove()
{
	CString strHead="", strFunc="";
	
	
	strFunc.Format("FUNC=CEID,CEID_NUM=1004,REPORT=1,REP_01={CNT=07,[DA_01=01,%s],[DA_02=01,%s],[DA_03=01,%s],[DA_04=01,%s],[DA_05=01,%s],[DA_06=01,%s],[DA_07=01,%s]},",
		st_lamp.mstr_equip_id, st_xgem.m_str_buffer1_id, st_xgem.m_str_buffer1_serial, st_xgem.m_strBuf1TactTime,
		st_xgem.m_str_buffer2_id, st_xgem.m_str_buffer2_serial, st_xgem.m_strBuf2TactTime);
	strHead.Format("HD=%06d,", strFunc.GetLength() + 10);
	
	sprintf(st_client[m_nPort].ch_send, strHead+strFunc);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
// 	sprintf(st_client[m_nPort].ch_send, strFunc);
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	m_bGemBufferMove = false;

}

void CXGemClient::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTmp="";
	CString strData="";

	if (m_strHeader == _T(""))
	{
		strTmp = strMsg.Mid(0, m_nHeader);
	}
	else
	{
		nPos	= strMsg.Find(m_strHeader, 0);
		strTmp	= strMsg.Mid(nPos + m_strHeader.GetLength() + 1, m_nHeader);
	}
	nHead	= atoi(strTmp);

	// Header 
	if (nHead > 0)
	{
		nLength = strMsg.GetLength() - m_nHeader;

		if (nHead == nLength)
		{
			strTmp = strMsg.Mid(m_nHeader, nLength);
			OnDataAnalysis(strTmp);
		}
		else
		{
			if (nHead > nLength)
			{
				m_nHeaderLength		= nHead;
				m_strData			= strMsg.Mid(m_nHeader, nLength);
			}
			else
			{
				strTmp = strMsg.Mid(m_nHeader, nHead);
				OnDataAnalysis(strTmp);
			}
		}
	}
	else
	{
		nLength = strMsg.GetLength();

		if (m_nHeaderLength > 0)
		{
			if (m_nHeaderLength == (m_strData.GetLength() + nLength))
			{
				strTmp		= m_strData + strMsg;
				OnDataAnalysis(strTmp);

				m_nHeaderLength		= 0;
				m_strData			= _T("");
			}
			else
			{
				if (m_nHeaderLength > (m_strData.GetLength() + nLength))
				{
					m_strData		+= strMsg;
				}
				else
				{
					strTmp		= m_strData + strMsg.Mid(0, nLength - (m_nHeaderLength - m_strData.GetLength()));
					OnDataAnalysis(strTmp);

					nPos	= nLength - (m_nHeaderLength - m_strData.GetLength());
					strData = strMsg.Mid(nPos, nLength - nPos);

					m_nHeaderLength		= 0;
					m_strData			= _T("");

					if (m_strHeader == _T(""))
					{
						strTmp	= strData.Mid(0, m_nHeader);
					}
					else
					{
						nPos	= strData.Find(m_strHeader, 0);
						strTmp	= strData.Mid(nPos + m_strHeader.GetLength() + 1, m_nHeader);
					}
					nHead	= atoi(strTmp);

					if (nHead > 0)
					{
						nLength = strData.GetLength() - m_nHeader;

						if (nHead == nLength)
						{
							strTmp = strData.Mid(m_nHeader, nLength);
							OnDataAnalysis(strTmp);
						}
						else
						{
							if (nHead > nLength)
							{
								m_nHeaderLength		= nHead;
								m_strData			= strData.Mid(m_nHeader, nLength);
							}
							else
							{
								strTmp = strData.Mid(m_nHeader, nHead);
								OnDataAnalysis(strTmp);
							}
						}
					}
					else
					{
						m_nHeaderLength		= 0;
						m_strData			= _T("");
					}
				}
			}
		}
	}
}

CString	CXGemClient::OnBodyAnalysis(int nMode, CString strRev, CString strFind)
{
	CString strData = "";
	int nLength;
	int nPos[4];
	
	nLength = strRev.GetLength();
	
	nPos[0] = -1;
	nPos[0] = strRev.Find(strFind, 0);
	
	if(nPos[0] < 0) return strData;
	
	nPos[1] = -1;
	nPos[1] = strRev.Find("=", nPos[0]+1);
	
	if(nPos[1] < 0) return strData;
	
	if(nMode == 0)
	{
		nPos[2] = -1;
		nPos[2] = strRev.Find(" ", nPos[0]+1);
		
		if(nPos[2] == -1)
		{
			strData = strRev.Mid(nPos[1]+1, nLength-(nPos[1]+1));	
		}
		else
		{
			strData = strRev.Mid(nPos[1]+1, nPos[2]-(nPos[1]+1));
		}
	}
//	else if(nMode == 1)
//	{
//		nPos[2] = -1;
//		nPos[2] = strRev.Find(")", nPos[1]+1);
//		
//		strData = strRev.Mid(nPos[1]+1, nPos[2]-(nPos[1]));
//	}
	else if(nMode == 2)
	{
		nPos[2] = -1;
		nPos[2] = strRev.Find("^XZ", nPos[1]+1);
		
		strData = strRev.Mid(nPos[1]+1, nPos[2]-(nPos[1]+1));
	}
	else if(nMode == 3)
	{
		nPos[2] = -1;
		nPos[2] = strRev.Find("\"", nPos[1]+1);
		
		nPos[3] = -1;
		nPos[3] = strRev.Find("\"", nPos[2]+1);
		
		strData = strRev.Mid(nPos[2]+1, nPos[3]-(nPos[2]+1));
	}
	else if(nMode == 4)
	{
		nPos[2] = -1;
		nPos[2] = strRev.GetLength();
		
		strData = strRev.Mid(nPos[1]+1, nPos[2]-(nPos[1]));
	}
	else/* if( nMode == 1 )*/
	{
		nPos[2] = nPos[3]= -1;
		nPos[2] = strRev.Find("(", nPos[1]+1);
		nPos[3] = strRev.Find(")", nPos[1]+2);
		nLength = nPos[3] - nPos[2] -1;
		strData = strRev.Mid( nPos[2] +1, nLength);

//		strData = strRecv.Mid(nPos[1] +1, nPos[2]-(nPos[1]));
	}
	
	return strData;
}

void CXGemClient::OnDataAnalysis(CString strRev)
{
	if( strRev == "" )	return;
	
	// 헤더 정보를 가지고 옴....
// 	strFunc	 = OnBodyAnalysis(0, strRev, "FUNCTION_RPY");

	CString strFunction = OnEqToXgemCommand( strRev);
	if	   ( strFunction == "COMMUNICATION" )			OnReceived_XgemInitialize( strRev );
	else if( strFunction == "OPERATE" )					OnReceived_XgemEqStatus( strRev );
	else if( strFunction == "ALARM" )					OnReceived_XgemAlarmStatus( strRev );
	else if( strFunction == "CEID"	)					OnReceived_XgemAnalysis( strRev );
	else if( strFunction == "RUN_STATUS" )				OnReceived_XgemRunStatus( strRev );
	else if( strFunction == "PPID_CHANGE" )				OnReceived_XgemPPIDChange( strRev );
	else if( strFunction == "RCMD" )					OnReceived_XgemRCMDChange( strRev );

}

void CXGemClient::OnReceived_XgemRCMDChange( CString strRecv)
{
	int		nPos[2];
	int		nLength;
	CString strConfig="";
	CString strMsg="";

	nPos[0] = strRecv.Find("RCMD_NUM=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);

	nLength = (nPos[1] - nPos[0]) - 9;
	strConfig = strRecv.Mid(nPos[0] + 9, nLength);
	if( strConfig == "START")
	{
		st_handler.m_nRemote = CTL_YES;
		if (st_handler.cwnd_title != NULL)	
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, dRUN); 
		}
	}
	else if( strConfig == "STOP")
	{
		nPos[0] = strRecv.Find("MSG=", 0);
		nPos[1] = strRecv.Find(",", nPos[0]);

		nLength = (nPos[1] - nPos[0]) - 4;
		strMsg = strRecv.Mid(nPos[0] + 4, nLength);
		st_handler.m_nRemote = CTL_NO;
		st_handler.m_xgemstrErrMsg = strMsg;

		st_msg.mstr_event_msg[0] = strMsg;
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		if (st_handler.cwnd_title != NULL)	
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, dSTOP); 
		}
	}
	else
	{
		return;
	}
}

//Ex)HD=000048,FUNC=COMMUNICATION,CONFIG=C:\\1234.cfg,
void CXGemClient::OnReceived_XgemInitialize( CString strRecv )
{
	int		nPos[2];
	int		nLength;
	CString strConfig="";
	
	nPos[0] = strRecv.Find("CONFIG=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);

	nLength = (nPos[1] - nPos[0]) - 7;

	strConfig	= "";
	strConfig = strRecv.Mid(nPos[0] + 7, nLength);

	if (strConfig == "") return;

// 	m_bGemInit = false;
	m_dwTime_GemInit_Send = -1;
	st_xgem.m_str_xgmInit = "";

	st_xgem.m_str_xgmInit = (LPCTSTR)strConfig;

	if( st_handler.cwnd_data_xgem )
		st_handler.cwnd_data_xgem->PostMessage( WM_DRAW_DATA_XGEM );
}
//Ex)HD=000023,FUNC=OPERATE,CONNECT=1,
void CXGemClient::OnReceived_XgemEqStatus( CString strRecv )
{
	int		nPos[2];
	int		nLength;
	CString strConnect="";
	nPos[0] = strRecv.Find("CONNECT=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);
	
	nLength = (nPos[1] - nPos[0]) - 8;
	
	strConnect	= "";
	strConnect = strRecv.Mid(nPos[0] + 8, nLength);
	
	if (strConnect == "") return;

	int nStatus = atoi(strConnect);

	m_bGemState[nStatus] = 0;
	m_dwTime_GemState_Send = -1;

	if( nStatus == 0) st_xgem.m_str_connect = "off-line";
	else			  st_xgem.m_str_connect = "on-line";
	
	if( st_handler.cwnd_data_xgem )
		st_handler.cwnd_data_xgem->PostMessage( WM_DRAW_DATA_XGEM );
}
//Ex)HD=000000,FUNC=ALARM,CODE=100000,STATUS=1,
//STATUS	0 - CLEAR
//			1 - SET
void CXGemClient::OnReceived_XgemAlarmStatus( CString strRecv )
{
	int		nPos[2];
	int		nLength;
	long	nCode;
	long	nStatus;	
	CString strValue[10], strAlarm="";

	nPos[0] = strRecv.Find("CODE=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);
	
	nLength = (nPos[1] - nPos[0]) - 5;	
	nCode = atoi(strRecv.Mid(nPos[0] + 5, nLength));

	nPos[0] = strRecv.Find("STATUS=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);
	
	nLength = (nPos[1] - nPos[0]) - 7;
	
	nStatus = atoi(strRecv.Mid(nPos[0] + 7, nLength));
	
//	if (m_hXgem->GEMSetAlarm(nCode, nStatus) < 0)
//	{
//		return;
//	}

	if(nStatus == 1)
	{
		//OnEqMachineStatus(MC_ALARM);
		strAlarm = "MC_ALARM";
	}
	else 
	{
		//OnEqMachineStatus(MC_STOP);
		strAlarm = "MC_STOP";
	}

	m_bGemAlarmState = false;
	m_dwTime_GemAlarmState_Send = -1;


	st_xgem.m_str_alarm.Format("%d %s", nCode, strAlarm);

	
	if( st_handler.cwnd_data_xgem )
		st_handler.cwnd_data_xgem->PostMessage( WM_DRAW_DATA_XGEM );
}
//HD=000025,FUNC=RUN_STATUS,STATUS=0,EPORT=1
//HD=000025,FUNC=RUN_STATUS,STATUS=1,EPORT=1
void CXGemClient::OnReceived_XgemRunStatus( CString strRecv )
{
	int nLength;
	int nPos[2];
	int nStatus;

	nPos[0] = strRecv.Find("STATUS=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);
	
	nLength = (nPos[1] - nPos[0]) - 7;
	
	nStatus = atoi(strRecv.Mid(nPos[0] + 7, nLength));
//	clsXgem.OnEqMachineStatus(nStatus);

	m_bGemMachineState = false;
	m_dwTime_GemMachineState_Send = -1;

	
	if( nStatus == 0) st_xgem.m_str_machine_status = "STOP";
	else			  st_xgem.m_str_machine_status = "RUN";

	
	if( st_handler.cwnd_data_xgem )
		st_handler.cwnd_data_xgem->PostMessage( WM_DRAW_DATA_XGEM );
}

void CXGemClient::OnReceived_XgemPPIDChange( CString strRecv )
{
	CString strJobChange="";
	int nLength;
	int nPos[2];

	nPos[0]				= strRecv.Find("PP_NAME=", 0);
	nPos[1]				= strRecv.Find(",", nPos[0]);
	nLength				= (nPos[1] - nPos[0]) - 8;
	CString m_strPPID	= strRecv.Mid(nPos[0] + 8, nLength);

	nPos[0]				= strRecv.Find("PP_PATH=", 0);
	nPos[1]				= strRecv.Find(",", nPos[0]);
	nLength				= (nPos[1] - nPos[0]) - 8;
	CString strPath		= strRecv.Mid(nPos[0] + 8, nLength);
	st_xgem.m_str_ppid  = strPath;

	m_bGemPPid = false;
	
	if( st_handler.cwnd_data_xgem )
		st_handler.cwnd_data_xgem->PostMessage( WM_DRAW_DATA_XGEM );
}

void CXGemClient::OnReceived_XgemAnalysis( CString strRecv )
{
	int nCeid;
	int nReport;
	int nReportCnt[10];
	int nVidCnt[10][10];
	int nPos[2];
	int nLength;
	int i, j;

	CString strReport[10][10];
	CString strTmp="";
	CString strRpt="";
	CString strVid="";
	CString strValue[10];

	nPos[0] = strRecv.Find("CEID_NUM=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);
	nLength = (nPos[1] - nPos[0]) - 9;
	nCeid = atoi(strRecv.Mid(nPos[0] + 9, nLength));

	nPos[0] = strRecv.Find("REPORT=", 0);
	nPos[1] = strRecv.Find(",", nPos[0]);
	nLength = (nPos[1] - nPos[0]) - 7;
	nReport = atoi(strRecv.Mid(nPos[0] + 7, nLength));

	for(i=0; i<nReport; i++)
	{
		strTmp.Format("REP_%02d={", i+1);
		nPos[0]			= strRecv.Find(strTmp, 0);
		nPos[1]			= strRecv.Find("}", nPos[0]);
		nLength			= (nPos[1] - nPos[0]) - 8;
		strRpt			= strRecv.Mid(nPos[0] + 8, nLength);

		nPos[0]			= strRpt.Find("CNT=", 0);
		nPos[1]			= strRpt.Find(",", nPos[0]);
		nLength			= (nPos[1] - nPos[0]) - 4;
		nReportCnt[i]	= atoi(strRpt.Mid(nPos[0] + 4, nLength));

		for(j=0; j<nReportCnt[i]; j++)
		{
			strTmp.Format("[DA_%02d=", j+1);
			nPos[0]			= strRpt.Find(strTmp, 0);
			nPos[1]			= strRpt.Find("]", nPos[0]);
			nLength			= (nPos[1] - nPos[0]) - 7;
			strVid			= strRpt.Mid(nPos[0] + 7, nLength);

			nVidCnt[i][j]	= atoi(strVid.Mid(0, 2));
			strReport[i][j].Format("[%s]", strVid.Mid(3, strVid.GetLength()-3));
		}
	}

	if	   ( nCeid == 1001)//Equipment State Change RTPID 1000
	{
	}
	else if( nCeid == 1002 )//Lot Start RTPID 1001
	{
		st_xgem.m_str_lotid = "";
		for(i=0; i<nReport; i++)
		{
			for(j=0; j<nReportCnt[i]; j++)
			{
				st_xgem.m_str_lotid += strReport[i][j];
			}			
		}
		m_bGemLotStart = false;
	}
	else if( nCeid == 1003 )//Lot End RTPID 1002
	{
		st_xgem.m_str_lotid = "";
		for(i=0; i<nReport; i++)
		{
			for(j=0; j<nReportCnt[i]; j++)
			{
				st_xgem.m_str_lotid += strReport[i][j];
			}			
		}
		m_bGemLotEnd = false;
	}
	else if ( nCeid == 1004 )//Buffer move
	{
// 		st_xgem.m_str_onSocket = "";
// 		for(i=0; i<nReport; i++)
// 		{
// 			for(j=0; j<nReportCnt[i]; j++)
// 			{
// 				st_xgem.m_str_onSocket += strReport[i][j];
// 			}			
// 		}
		m_bGemBufferMove = false;
	}
	else if ( nCeid == 1005 )//Socket On RTPID 1004
	{
// 		st_xgem.m_str_offSocket = "";
// 		for(i=0; i<nReport; i++)
// 		{
// 			for(j=0; j<nReportCnt[i]; j++)
// 			{
// 				st_xgem.m_str_offSocket += strReport[i][j];
// 			}			
// 		}
// 		m_bGemSokOn = false;
	}
	else if ( nCeid == 1100 )//ProcessProgramSelect(JobChange) RTPID 1005
	{
	}
	
	if( st_handler.cwnd_data_xgem )
		st_handler.cwnd_data_xgem->PostMessage( WM_DRAW_DATA_XGEM );
}



CString CXGemClient::OnEqToXgemFunction(CString strFind, CString strRecv)
{
	int		nPos[2];
	int		nLength;
	CString strData="";

	nPos[0] = strRecv.Find(strFind, 0);
	nPos[1] = strRecv.Find(",", nPos[0]+1);

	nLength	= (nPos[1] - nPos[0]) - (strFind.GetLength()+1);
	strData = strRecv.Mid(nPos[0]+strFind.GetLength()+1, nLength);

	return strData;
}

CString CXGemClient::OnEqToXgemCommand(CString strRecv)
{
	int nLength;
	int nHead;
	int nPos[2];

	CString strFunc="";
	CString strFind="";
	CString strConnect="";
	CString strPath="";

	nLength	= strRecv.GetLength() - 10;

	nPos[0]	= -1;
	nPos[0]	= strRecv.Find("HD=", 0);

	if(nPos[0] > -1)
	{
		nHead = atoi(strRecv.Mid(nPos[0]+3, 6));
		if(nHead == nLength)
		{
			strFunc = OnEqToXgemFunction("FUNC", strRecv);
			return strFunc;
		}
	}

	return "NULL";
}


//void AClient_Xgem::OnEqToXgemCommand(CString strRecv)
//{
//	FILE *fp;
//
//	int nLength;
//	int nHead;
//	int nPos[2];
//	int nStatus;
//
//	char chBody[100];
//
//	CString strFunc;
//	CString strFind;
//	CString strConnect;
//	CString strPath;
//
//	nLength	= strRecv.GetLength() - 10;
//
//	nPos[0]	= -1;
//	nPos[0]	= strRecv.Find("HD=", 0);
//
//	if(nPos[0] > -1)
//	{
//		nHead = atoi(strRecv.Mid(nPos[0]+3, 6));
//		if(nHead == nLength)
//		{
//			strFunc = OnEqToXgemFunction("FUNC", strRecv);
//
//			if (strFunc == "COMMUNICATION")
//			{
//				clsXgem.OnEqCommunication(&m_hXgem, strRecv);
//			}
//			else if (strFunc == "OPERATE")
//			{
//				nPos[0] = strRecv.Find("CONNECT=", 0);
//				nPos[1] = strRecv.Find(",", nPos[0]);
//				
//				nLength = (nPos[1] - nPos[0]) - 8;
//				
//				strConnect	= "";
//				strConnect = strRecv.Mid(nPos[0] + 8, nLength);
//
//				if (strConnect == "0")
//				{
//					clsXgem.OnEqMachineOffLine();
//				}
//				else if(strConnect == "1")
//				{
//					clsXgem.OnEqMachineLocal();
//				}
//				else if(strConnect == "2")
//				{
//					clsXgem.OnEqMachineRemote();
//				}
//				m_bGemState[strConnect] = 0;
//				m_dwTime_GemState_Send = -1;
//				if( st_handler.cwnd_data_xgem )
//					st_handler.cwnd_data_lot->PostMessage( WM_DRAW_DATA_LOT );
//			}
//			else if (strFunc == "ALARM")
//			{
//				clsXgem.OnEqAlarm(strRecv);
//			}
//			else if (strFunc == "CEID")
//			{
//				clsXgem.OnEqCEID(strRecv);
//			}
//			else if (strFunc == "HOST_TIME")
//			{
//				clsXgem.OnHostGetCurrentTime();
//			}
//			else if (strFunc == "RUN_STATUS")
//			{
//				nPos[0] = strRecv.Find("STATUS=", 0);
//				nPos[1] = strRecv.Find(",", nPos[0]);
//				
//				nLength = (nPos[1] - nPos[0]) - 7;
//				
//				nStatus = atoi(strRecv.Mid(nPos[0] + 7, nLength));
//				clsXgem.OnEqMachineStatus(nStatus);
//			}
//			else if (strFunc == "PPID_CHANGE")
//			{
//				nPos[0]				= strRecv.Find("PP_NAME=", 0);
//				nPos[1]				= strRecv.Find(",", nPos[0]);
//				nLength				= (nPos[1] - nPos[0]) - 8;
//				clsXgem.m_strPPID	= strRecv.Mid(nPos[0] + 8, nLength);
//
//				nPos[0]				= strRecv.Find("PP_PATH=", 0);
//				nPos[1]				= strRecv.Find(",", nPos[0]);
//				nLength				= (nPos[1] - nPos[0]) - 8;
//				strPath				= strRecv.Mid(nPos[0] + 8, nLength);
//
//				fp = fopen ( strPath, "r" );
//				
//				if ( fp == (FILE *)NULL ) 
//				{
//					return ;
//				}
//
//				clsXgem.m_strPPBody = "";
//				while ( !feof(fp) )
//				{
//					if ( fgets (chBody, 100, fp ) == NULL )
//					{
//						break;
//					}
//					clsXgem.m_strPPBody += chBody;
//				}
//				fclose(fp);
//
//				if(clsXgem.OnEqPPIDLoad(clsXgem.m_strPPID) == CTLBD_RET_GOOD)
//				{
//					sprintf(st_msg_info.c_normal_msg, "[XGEM->EQ] S7F1 SEND SUCCESS");
//				}
//				else
//				{
//					sprintf(st_msg_info.c_normal_msg, "[XGEM->EQ] S7F1 SEND FAIL");
//				}
//				::PostMessage(this->m_hWnd, WM_LIST_DATA, NORMAL_MSG, 0);
//			}
//
//			sprintf(st_server_info[0].ch_send, strRecv);
//			::PostMessage(this->m_hWnd, WM_SERVER_MSG_1, SERVER_SEND, 0);
//		}
//	}
//}