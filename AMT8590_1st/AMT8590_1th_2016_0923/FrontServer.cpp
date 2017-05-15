// FrontServer.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "FrontServer.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrontServer
CFrontServer g_front_server;

CFrontServer::CFrontServer()
{
	m_strHeader = _T("");
	m_nHeader	= 16;
	m_nPort = SVR_FRONT;

}

CFrontServer::~CFrontServer()
{
}

const int m_nPort = 0;

/////////////////////////////////////////////////////////////////////////////
// CFrontServer message handlers

void CFrontServer::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTmp;
	CString strData;

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

CString	CFrontServer::OnBodyAnalysis(int nMode, CString strRev, CString strFind)
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

void CFrontServer::OnDataAnalysis(CString strRev)
{
	CString strFunc,strFunc1;
	CString strData;
	
	// 헤더 정보를 가지고 옴....
	strFunc	= OnBodyAnalysis(0, strRev, "FUNCTION");

	if (strFunc == "BUFFER_IN")		OnReceived_BufferIn( strRev );
	else if (strFunc == "EMG_COMPLETE")		OnReceived_EMG_Stop( strRev ); //8580

}

//FUNCTION=	EMG_COMPLETE	EQP_ID=	SSD_LABELLER
//FUNCTION_RPY=	EMG_COMPLETE	RESULT=	PASS
void CFrontServer::OnReceived_EMG_Stop(CString strRecv)
{
	// 응답
 	CString strPass = strRecv;
 	strPass.Replace( "FUNCTION=", "FUNCTION_RPY=" );
 	strPass += "RESULT=PASS";

	CString strData;
	strData.Format("%016d", strPass.GetLength());
	sprintf(st_server[m_nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + m_nPort, SERVER_SEND, 0);
	
	sprintf(st_server[m_nPort].ch_send, strPass);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + m_nPort, SERVER_SEND, 0);

	m_nrEMGStopIn = NR_RECEIVED;
}




void CFrontServer::OnReceived_BufferIn( CString strRecv )
{
//  	if( m_nrBufferIn != NR_WAITING )
//  	{
//  		CString strPass = strRecv;
//  		strPass.Replace( "FUNCTION=", "FUNCTION_RPY=" );
//  		strPass += "RESULT=FAIL MSG=\"NOT READY\"";
//  		
// 		CString strData;
// 		strData.Format("%016d", strPass.GetLength());
// 		sprintf(st_server[m_nPort].ch_send, strData);
// 		::SendMessage(st_handler.hWnd, WM_SERVER_MSG + m_nPort, SERVER_SEND, 0);
// 		
// 		sprintf(st_server[m_nPort].ch_send, strPass);
// 		::SendMessage(st_handler.hWnd, WM_SERVER_MSG + m_nPort, SERVER_SEND, 0);
//  		return;
//  	}

 	//////////////////////////////////////////////////////////////////////////
 	CString strLotID =		OnBodyAnalysis( 0, strRecv, "LOT_ID" );
 	CString strPartID =		OnBodyAnalysis( 0, strRecv, "PART_ID" );
 	CString strLastMdl =	OnBodyAnalysis( 0, strRecv, "LAST_MDL" );
 	CString strBypass =     OnBodyAnalysis( 0, strRecv, "BYPASS" );

	CString strBufferNo = _T("");
	strBufferNo =	OnBodyAnalysis( 0, strRecv, "BUFFER_NO" );
	if(strBufferNo == "") strBufferNo = _T("01");
	//////////////////////////////////////////////////////////////////////////
	
	if(strLotID == "") strLotID = "test";
	
 	// Lot LastModule Set
	int nLastMdl = atoi(strLastMdl);
 	if( nLastMdl == 3/*"YES"*/ )
 	{
 		g_lotMgr.GetLotByLotID( strLotID ).SetLastModule( "YES" );
 		if( st_handler.cwnd_main )
 			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
 	}
	
 	// 바이패스
 	bool bBypass = false;
 	if( st_basic.mn_bypass_mode == BYPASS_ALL )
 	{
 		bBypass = true;
 	}
 


  
 	CString strPocket[MAX_BUFFDVC];
 	for( int i=0; i<MAX_BUFFDVC; i++ )
 	{
 		CString strPocketIdx;
 		strPocketIdx.Format( "MD_%02d", i + 1);
 
 		strPocket[i] = OnBodyAnalysis( 1, strRecv, strPocketIdx );//, NET_OPT_BRACKET );
 		if( strPocket[i] == "NONE" || strPocket[i] == "" ) continue;
 
 		CString strProductID =	OnBodyAnalysis( 0, strPocket[i], "PRODUCT_NAME" );
 		CString strSkip =		OnBodyAnalysis( 0, strPocket[i], "BUFFER_SKIP" );
		CString strArraySN =	OnBodyAnalysis( 0, strPocket[i], "IDBUF" );
 		CString strSerial =		OnBodyAnalysis( 0, strPocket[i], "SERIAL" );
 		CString strVar =		OnBodyAnalysis( 0, strPocket[i], "VAR" );
		CString strAcceID =		OnBodyAnalysis( 0, strPocket[i], "ACCE_CODE" );
		CString strTboxID =		OnBodyAnalysis( 0, strPocket[i], "TBOX_CODE" );


		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_last_module[i] = nLastMdl;

		int nSkip = atoi(strSkip);//0:skip 1:exist
		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[i] = nSkip;
		if( strArraySN != "" )
		{
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[i] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[i] = atoi(strArraySN);
		}
		else
		{
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[i] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[i] = 11;
		}
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[i], "%s", strLotID);
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_part_num[i], "%s", strPartID);
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[i], "%s", strSerial);
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_var[i], "%s", strVar);
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_product_id[i], "%s", strProductID);
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_acce_code[i], "%s", strAcceID);
		sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_tbox_code[i], "%s", strTboxID);

		if(i == 0 )
		{
			if(strSerial != "")
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] = 1;
				//2014,1227
// 				g_lotMgr.GetLotByLotID(strLotID).AddInputCnt( PRIME );
// 				g_lotMgr.GetLotByLotID(strLotID).AddInputCnt( CUM );
				//2015.0606
				//st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_count[CTL_LEFT] = g_lotMgr.GetLotByLotID(strLotID).GetInputCnt( PRIME );
			}
			else
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] = 0;
		}
		if(i == 1)
		{
			if(strSerial != "")
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] = 1;
				//2014,1227
// 				g_lotMgr.GetLotByLotID(strLotID).AddInputCnt( PRIME );
// 				g_lotMgr.GetLotByLotID(strLotID).AddInputCnt( CUM );
				//2015.0606
				//st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_count[CTL_RIGHT] = g_lotMgr.GetLotByLotID(strLotID).GetInputCnt( PRIME ) ;
			}
			else
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] = 0;
		}
 	}
 	
	//2014,1227
	if (st_handler.cwnd_main)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
	}

	// 	st_var.n_newlotin = CTL_NO;
	if(g_lotMgr.GetLotCount() <= 0)
	{
		if( st_basic.mn_mode_run == CTL_NO)
			g_lotMgr.OnNewLotIn(strLotID, strPartID);
		else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[0] == 0 &&
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[1] == 0 )
		{
			CString strLog="";
			st_handler.m_strNewLot = strLotID;
			// 			st_handler.m_strVisModel = strLotID;
			st_work.mn_chg_job = CTL_YES;
			st_var.n_densityccs = CTL_READY;
			st_var.n_bcrccs = CTL_READY;
			st_var.n_newlotin = CTL_YES;
			strLog.Format("[NEW_LOT] 1.LOTID: %s. Newlotin is YES", strLotID);
			Func.On_LogFile_Add(99, strLog);
		}
	}
	else
	{
		bool bLot = false;
		for ( int i = 0; i < g_lotMgr.GetLotCount(); i++)
		{
			if(g_lotMgr.GetLotAt(i).GetLotID() == strLotID)
			{
				bLot = true;
				break;
			}
		}
		if(bLot == false)
		{
			if( st_basic.mn_mode_run == CTL_NO)
				g_lotMgr.OnNewLotIn(strLotID, strPartID);
			else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[0] == 0 &&
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[1] == 0 )
			{
				CString strLog;
				st_handler.m_strNewLot = strLotID;
// 				st_handler.m_strVisModel = strLotID;
				st_work.mn_chg_job = CTL_YES;
				st_var.n_densityccs = CTL_READY;
				st_var.n_bcrccs = CTL_READY;
				st_var.n_newlotin = CTL_YES;
				strLog.Format("[NEW_LOT] 2.LOTID: %s. Newlotin is YES", strLotID);
				Func.On_LogFile_Add(99, strLog);
			}
		}
	} 
	

 	// 응답
 	CString strPass = strRecv;
 	strPass.Replace( "FUNCTION=", "FUNCTION_RPY=" );
 	strPass += "RESULT=PASS";
 	
	CString strData;
	strData.Format("%016d", strPass.GetLength());
	sprintf(st_server[m_nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + m_nPort, SERVER_SEND, 0);
	
	sprintf(st_server[m_nPort].ch_send, strPass);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + m_nPort, SERVER_SEND, 0);

 	m_nrBufferIn = NR_RECEIVED;
}

