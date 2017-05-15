// AEC_Client.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "AEC_Client.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AEC_Client
AEC_Client clsEC_Client;

AEC_Client::AEC_Client()
{
}

AEC_Client::~AEC_Client()
{
}

/////////////////////////////////////////////////////////////////////////////
// AEC_Client message handlers
void AEC_Client::OnMove_MasterInfo( int port, CString strLotID, CString strSerial )
{
	m_nrMasterInfo = NR_WAITING;

	if(GetCurrentTime() - m_dwTime_MasterInfo < 0)
	{
		m_dwTime_MasterInfo = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwTime_MasterInfo < 5000 )
		return;

	CString strBody;
	strBody.Format( "FUNCTION=PACKING_MASTER_INFO LOT_ID=%s SERIAL_ID=%s", strLotID, strSerial);

	strBody.Format("%016d", strBody.GetLength());
	sprintf(st_server[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_server[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

}

//BRAND_DENSITY_LABLE_ISSUE
void AEC_Client::OnMove_DensityIn(int port, CString strLotID, CString strBoatID, CString strSerial, CString strRetry, CString strCnt )
{
	m_nrDensityID = NR_WAITING;
	
	CString strBody;
	strBody.Format( "FUNCTION=BRAND_DENSITY_LABLE_ISSUE LOT_ID=%s BOAT_ID=%s EQP_ID=%s SERIAL=%s MODE=%s RETRY=%s RETRY_CNT=%d", 
		m_strDensityLot, m_strBufID, st_lamp.mstr_equip_id, strSerial, strRetry, strCnt);

	strBody.Format("%016d", strBody.GetLength());
	sprintf(st_server[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_server[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

}

void AEC_Client::OnBoatIDModify( int port, CString lotid, CString boatid, CString strSerial, CString strModify, CString strRetry, CString strCnt )
{
	CString strBody;
	CString strData;
	//	int nLength;
	//	int nPos;
	
	strBody = "";
	m_nrBoatID = NR_NONE;
	
	if (st_client[port].n_connect == CTL_NO) return;
	
	strRetry = "NO";
	strCnt = "0";
	
	strBody = "FUNCTION=BOAT_ID_MODIF";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "BOAT_ID=" + boatid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "MODIF_MODE=" + strModify;
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

	m_nrBoatID = NR_WAITING;
}
	
void AEC_Client::OnProd_Serial_Info( int port, CString strSerial, CString strRetry, CString strCnt )
{
	m_nrProdSerial = NR_WAITING;


	CString strBody;
	strBody.Format( "FUNCTION=PROD_SERIAL_INFO EQP_ID=%s SERIAL_ID=%s RETRY=%s RETRY_CNT=%d", 
		st_lamp.mstr_equip_id, m_strSerialInfo, m_strSerialInfoRetry, m_nCnt_SerialInfo);

	strBody.Format("%016d", strBody.GetLength());
	sprintf(st_server[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_server[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

}


void AEC_Client::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTmp;
	CString strData;

	if( m_strHeader == _T("") )
	{
		strTmp = strMsg.Mid( 0 , m_nHeader );
	}
	else
	{
		nPos = strMsg.Find( m_strHeader, 0);
		strTmp = strMsg.Mid( nPos + m_strHeader.GetLength() + 1, m_nHeader);
	}
	nHead = atoi( strTmp );

	//Header
	if( nHead > 0 )
	{
		nLength = strMsg.GetLength() - m_nHeader;
		if( nHead == nLength )
		{
			strTmp = strMsg.Mid( m_nHeader, nLength );
			OnDataAnalysis( strTmp );
		}
		else
		{
			if( nHead > nLength )
			{
				m_nHeaderLength = nHead;
				m_strData = strMsg.Mid(m_nHeader, nLength);
			}
			else
			{
				strTmp = strMsg.Mid( m_nHeader, nHead );
				OnDataAnalysis(strTmp);
			}

		}
	}
	else
	{
		nLength = strMsg.GetLength();
		if( m_nHeaderLength > 0 )
		{
			if( m_nHeaderLength == (m_strData.GetLength() + nLength ) )
			{
				strTmp = m_strData +strMsg;
				OnDataAnalysis(strTmp);

				m_nHeaderLength = 0;
				m_strData = _T("");
			}
			else
			{
				if( m_nHeaderLength > ( m_strData.GetLength() + nLength) )
				{
					m_strData += strMsg;
				}
				else
				{
					strTmp = m_strData + strMsg.Mid( 0, nLength - ( m_nHeaderLength - m_strData.GetLength() ));
					OnDataAnalysis(strTmp);

					nPos = nLength - (m_nHeaderLength - m_strData.GetLength());
					strData = strMsg.Mid( nPos, nLength - nPos );

					m_nHeaderLength = 0;
					m_strData = _T("");

					if( m_strHeader == _T("") )
					{
						strTmp = strData.Mid( 0, m_nHeader );
					}
					else
					{
						nPos = strData.Find(m_strHeader, 0 );
						strTmp = strData.Mid( nPos + m_strHeader.GetLength()+ 1, m_nHeader);
					}
					nHead = atoi(strTmp);

					if( nHead > 0 )
					{
						nLength = strData.GetLength() - m_nHeader;
						if( nHead == nLength )
						{
							strTmp = strData.Mid( m_nHeader, nLength );
							OnDataAnalysis( strTmp );
						}
						else
						{
							if( nHead > nLength )
							{
								m_nHeaderLength = nHead;
								m_strData = strData.Mid( m_nHeader, nLength);
								OnDataAnalysis(strTmp);
							}
							else
							{
								if( nHead > nLength )
								{
									m_nHeaderLength = nHead;
									m_strData = strData.Mid(m_nHeader, nLength);
								}
								else
								{
									strTmp = strData.Mid( m_nHeader, nHead );
									OnDataAnalysis( strTmp );
								}
							}
						}
					}
				}
			}
		}
		else
		{
			m_nHeaderLength = 0;
			m_strData = _T("");
		}

	}
}

void AEC_Client::OnDataAnalysis(CString strRev)
{
	CString strFunc;
	CString strData;

	strFunc = OnBodyAnalysis( 0, strRev, "FUNCTION_RPY" );
	if	   ( strFunc == "PACKING_MASTER_INFO" )			OnReceived_MasterInfo( strRev );
	else if( strFunc == "BRAND_DENSITY_LABLE_ISSUE" )	OnReceived_DensityDataIn( strRev );
	else if( strFunc == "PROD_SERIAL_INFO")				OnReceived_ProdSerialInfo( strRev );
	else if( strFunc == "BOAT_ID_MODIF")				OnReceived_BoatIDModify( strRev );

}

CString AEC_Client::OnBodyAnalysis( int nMode, CString strRecv, CString strFind )
{
	CString strData = "";
	int nLength;
	int nPos[4];

	nLength = strRecv.GetLength();

	nPos[0] = -1;
	nPos[0] = strRecv.Find(strFind, 0);

	if(nPos[0] < 0) return strData;

	nPos[1] = -1;
	nPos[1] = strRecv.Find("=", nPos[0]+1);

	if(nPos[1] < 0) return strData;

	if(nMode == 0)
	{
		nPos[2] = -1;
		nPos[2] = strRecv.Find(" ", nPos[0]+1);

		if(nPos[2] == -1)
		{
			strData = strRecv.Mid(nPos[1]+1, nLength-(nPos[1]+1));
		}
		else
		{
			strData = strRecv.Mid(nPos[1]+1, nPos[2]-(nPos[1]+1));
		}
	}
	else if( nMode == 2 )
	{
		nPos[2] = strRecv.Find("=", nPos[1]+1);
		strData = strRecv.Mid(nPos[2]+1); // 해당 item의 끝 위치를 찾는다.	// DATA의 길이 계산.
	}
	else/* if( nMode == 1 )*/
	{
		nPos[2] = nPos[3]= -1;
		nPos[2] = strRecv.Find("(", nPos[1]+1);
		nPos[3] = strRecv.Find(")", nPos[1]+2);
		nLength = nPos[3] - nPos[2] -1;
		strData = strRecv.Mid( nPos[2] +1, nLength);

//		strData = strRecv.Mid(nPos[1] +1, nPos[2]-(nPos[1]));
	}

	return strData;
}

void AEC_Client::OnReceived_MasterInfo( CString strRecv )
{
	CString strResult = OnBodyAnalysis( 0, strRecv, "RESULT" );
	CString strMsg =	OnBodyAnalysis( 0, strRecv, "MSG" );
	if( strResult == "PASS" )
	{
		CString strEqpID =		OnBodyAnalysis( 0, strRecv, "EQPID" );
		CString strLotID =		OnBodyAnalysis( 0, strRecv, "LOTID" );
		CString strPartID =		OnBodyAnalysis( 0, strRecv, "PARTID" );
		CString strLotStep =	OnBodyAnalysis( 0, strRecv, "LOT_STEP" );
		CString strQty =		OnBodyAnalysis( 0, strRecv, "LOT_QTY" );
		CString strProductName = OnBodyAnalysis( 0, strRecv, "PRODUCT_NAME" );
		CString strDistributeLabelMassCode = OnBodyAnalysis( 0, strRecv, "DTBT_LABEL_MAT_CODE");
		CString	strRibbonCode	= OnBodyAnalysis( 0, strRecv, "RIBBON_MAT_CODE" );
		CString	strCapa	= OnBodyAnalysis( 0, strRecv, "DENSITY" );
		

		if(g_lotMgr.GetLotCount() <= 0)
		{
			g_lotMgr.OnNewLotIn(strLotID, strPartID);
			g_lotMgr.GetLotByLotID(strLotID).SetLotEnd("NO");
			g_lotMgr.GetLotAt(0).SetLotTime();
			g_lotMgr.GetLotAt(0).SetDisLabelCode( strDistributeLabelMassCode );
			g_lotMgr.GetLotAt(0).SetRibbonCode( strRibbonCode );
			g_lotMgr.GetLotAt(0).SetTotalModule( strQty );
			g_lotMgr.GetLotAt(0).SetDenLabelCapa(strCapa);
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

			if( bLot == false )
			{
				g_lotMgr.OnNewLotIn(strLotID, strPartID);
				g_lotMgr.GetLotByLotID(strLotID).SetLotEnd("NO");
				g_lotMgr.GetLotByLotID(strLotID).SetLotTime();
				g_lotMgr.GetLotByLotID(strLotID).SetDisLabelCode( strDistributeLabelMassCode );
				g_lotMgr.GetLotByLotID(strLotID).SetRibbonCode( strRibbonCode );
				g_lotMgr.GetLotByLotID(strLotID).SetTotalModule( strQty );
				g_lotMgr.GetLotByLotID(strLotID).SetDenLabelCapa(strCapa);
			}
		}
		m_nrMasterInfo = NR_RECEIVED;
		m_strMasterInfo = "";
	}
	else if( strResult == "FAIL" )
	{
		CString strErr;
		strErr.Format( "[Server] MasterInfo is fail");
		if ( g_local.GetLocalType() == LOCAL_ENG ) strErr.Format( "[Server] MasterInfo in error");

		if( COMI.mn_run_status == dRUN )
		{
			st_msg.mstr_event_msg[0] = strErr;
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		}
		else
		{
			if( st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"%s", strErr);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 ,ABNORMAL_MSG);
			}
		}		
		m_nrMasterInfo = NR_FAIL;
	}
}


void AEC_Client::OnReceived_DensityDataIn( CString strRecv )
{
	CString strResult = OnBodyAnalysis( 0, strRecv, "RESULT" );
	CString strMsg =	OnBodyAnalysis( 0, strRecv, "MSG" );
	if( strResult == "PASS" )
	{
		CString strEqpID =		OnBodyAnalysis( 0, strRecv, "EQP_ID" );
		CString strLotID =		OnBodyAnalysis( 0, strRecv, "LOT_ID" );
		CString strSerial =		OnBodyAnalysis( 0, strRecv, "SERIAL" );
		CString strDensity =	OnBodyAnalysis( 0, strRecv, "DENSITY" );
		CString strFormat =		OnBodyAnalysis( 0, strRecv, "FORMAT" );

		m_nrDensityID = NR_RECEIVED;

		g_lotMgr.GetLotByLotID(strLotID).SetDenLabelCapa(strDensity);
	}
	else if( strResult == "FAIL" )
	{
		CString strErr;
		strErr.Format( "[Server] Density Info is fail");
		if ( g_local.GetLocalType() == LOCAL_ENG ) strErr.Format( "[Server] Density Info in error");

		if( COMI.mn_run_status == dRUN )
		{
			st_msg.mstr_event_msg[0] = strErr;
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		}
		else
		{
			if( st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"%s", strErr);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 ,ABNORMAL_MSG);
			}
		}
		
		m_nrDensityID = NR_FAIL;
	}
}

void AEC_Client::OnReceived_ProdSerialInfo( CString strRecv )
{

	CString strResult = OnBodyAnalysis( 0, strRecv, "RESULT" );
	CString strMsg =	OnBodyAnalysis( 0, strRecv, "MSG" );
	if( strResult == "PASS" )
	{
		CString strEqpID =		OnBodyAnalysis( 0, strRecv, "EQP_ID" );
		CString strLotID =		OnBodyAnalysis( 0, strRecv, "LOT_ID" );
		CString strPartID =		OnBodyAnalysis( 0, strRecv, "PART_ID" );
		CString strSerial =		OnBodyAnalysis( 0, strRecv, "SERIAL_ID" );
		CString strBoatid =		OnBodyAnalysis( 0, strRecv, "BOAT_ID" );

		m_nrProdSerial = NR_RECEIVED;
	}
	else if( strResult == "FAIL" )
	{
		CString strErr;
		strErr.Format( "[Server] Product Serial is fail");
		if ( g_local.GetLocalType() == LOCAL_ENG ) strErr.Format( "[Server] Product Serial in error");

		if( COMI.mn_run_status == dRUN )
		{
			st_msg.mstr_event_msg[0] = strErr;
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		}
		else
		{
			if( st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"%s", strErr);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 ,ABNORMAL_MSG);
			}
		}
		
		m_nrProdSerial = NR_FAIL;
	}
}


void AEC_Client::OnReceived_BoatIDModify( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		CString strLotID = OnBodyAnalysis( 0, strRecv, "LOT_ID");
		CString strBoatID = OnBodyAnalysis( 0, strRecv, "BOAT_ID");
		CString strSerial = OnBodyAnalysis( 0, strRecv, "SERIAL");
		CString strModify = OnBodyAnalysis( 0, strRecv, "MODIF_MODE");
		
		// 		st_mes_data.srt_LOT_ID = strLotID;
		// 		st_mes_data.srt_PART_ID = strPartID;
		// 		st_mes_data.srt_LotQty = strLotQty;
		// 		st_mes_data.srt_BOAT_ID = strBoatID;
		// 		st_mes_data.srt_PROD_SRLNO = strProductID;
		// 		st_mes_data.srt_TK_CODE = strTKCode;
		// 		st_mes_data.srt_DEN_LABEL_MAT_CODE = strDensity_mat_code;
		// 		st_mes_data.srt_DTBT_MAT_CODE = strDtbt_label_mat_code;
		// 		st_mes_data.srt_RIBBON_MAT_CODE = strRibbon_mat_code;
		// 		st_mes_data.srt_DENSITY = strDensity;
		m_nrBoatID = NR_RECEIVED;		
	}
	else
	{
		m_nrBoatID = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
//		ClrMsg(); SetMsg(strMsg);
	}
}