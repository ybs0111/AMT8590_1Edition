// BarcodeFormat.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "BarcodeFormat.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarcodeFormatClient
CBarcodeFormatClient clsBcrFormatClient;

CBarcodeFormatClient::CBarcodeFormatClient()
{
	m_strHeader = _T("");
	m_nHeader	= 16;
}

CBarcodeFormatClient::~CBarcodeFormatClient()
{
}


/////////////////////////////////////////////////////////////////////////////
// CBarcodeFormatClient message handlers
//Pk_XCLOSE
void CBarcodeFormatClient::PX_XClose( int port, CString lotid, CString strOPer, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
//	int nLength;
//	int nPos;

	strBody = "";
	m_nrSerialFormat = NR_NONE;

	if (st_client[port].n_connect == CTL_NO) return;

	strBody = "FUNCTION=PK_XCLOSE";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "OPER_ID=" + lotid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;

	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
}


//BRAND_DTBT_LABEL_ISSUE

void CBarcodeFormatClient::OnBrandBcrLabelIssue( int port, CString lotid, CString boatid, CString strSerial, CString strLabel, CString strRibbon, CString mode, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
//	int nLength;
//	int nPos;

	strBody = "";
	m_nrSerialFormat = NR_NONE;

	if (st_client[port].n_connect == CTL_NO) return;

	strBody = "FUNCTION=BRAND_DTBT_LABEL_ISSUE";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "BOAT_ID=" + boatid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "DTBT_LABEL_CODE=" + strLabel;
	strBody += " ";
	strBody += "DTBT_RIBBON_CODE=" + strRibbon;
	strBody += " ";
	strBody += "MODE=" + mode;//RETRY
	strBody += " ";
// 	strRetry = "NO";
	strCnt = "0";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;

	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

	m_nrSerialFormat = NR_WAITING;

}

void CBarcodeFormatClient::OnBrandDensityLabelIssue( int port, CString lotid, CString boatid, CString strSerial, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
//	int nLength;
//	int nPos;

	strBody = "";
	m_nrSerialFormat = NR_NONE;

	if (st_client[port].n_connect == CTL_NO) return;

	strBody = "FUNCTION=BRAND_DENSITY_LABEL_ISSUE";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "BOAT_ID=" + boatid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;

	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
}

void CBarcodeFormatClient::OnBrandDensityLabelCCS( int port, CString lotid, CString boatid, CString strSerial, CString strMatCode, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
//	int nLength;
//	int nPos;

	strBody = "";
	m_nrDensityCCS = NR_NONE;

	if (st_client[port].n_connect == CTL_NO) return;

	strBody = "FUNCTION=BRAND_DENSITY_LABEL_CCS";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "BOAT_ID=" + boatid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "DEN_LABEL_MAT_CODE=" + strMatCode;	
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

	m_nrDensityCCS = NR_WAITING;
}

void CBarcodeFormatClient::OnBrandDTBTLabelCCS( int port, CString lotid, CString boatid, CString strSerial, CString strMatCode, CString strRibonCode, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
//	int nLength;
//	int nPos;

	strBody = "";
	m_nrBcrLabelCCS = NR_NONE;

	if (st_client[port].n_connect == CTL_NO) return;

	strBody = "FUNCTION=BRAND_DTBT_LABEL_CCS";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "BOAT_ID=" + boatid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "DTBT_MAT_CODE=" + strMatCode;	
	strBody += " ";
	strBody += "RIBBON_MAT_CODE=" + strRibonCode;	
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);

	m_nrBcrLabelCCS = NR_WAITING;
}


void CBarcodeFormatClient::OnProdSerial_Info( int port, CString lotid, CString boatid, CString strSerial, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
	//	int nLength;
	//	int nPos;
	
	strBody = "";
	m_nrSerialFormat = NR_NONE;
	
	if (st_client[port].n_connect == CTL_NO) return;

	strRetry = "NO";
	strCnt = "0";
	
	strBody = "FUNCTION=PROD_SERIAL_INFO";
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
}

void CBarcodeFormatClient::OnBoatID_Info( int port, CString boatid, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
	//	int nLength;
	//	int nPos;
	
	strBody = "";
	m_nrSerialFormat = NR_NONE;
	
	if (st_client[port].n_connect == CTL_NO) return;
	
	strRetry = "NO";
	strCnt = "0";

	strBody = "FUNCTION=BOAT_ID_INFO";
	strBody += " ";
	strBody += "BOAT_ID=" + boatid;
	strBody += " ";
	strBody += "EQP_ID=" + st_lamp.mstr_equip_id;
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
}


void CBarcodeFormatClient::OnPacking_Master_Info( int port, CString lotid, CString strSerial, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
	//	int nLength;
	//	int nPos;
	
	strBody = "";
	m_nrMasterInfo = NR_NONE;
	
	if (st_client[port].n_connect == CTL_NO) return;
	
	strRetry = "NO";
	strCnt = "0";

	strBody = "FUNCTION=PACKING_MASTER_INFO";
	strBody += " ";
	strBody += "LOT_ID=" + lotid;
	strBody += " ";
	strBody += "SERIAL=" + strSerial;
	strBody += " ";
	strBody += "RETRY=" + strRetry;//YES
	strBody += " ";
	strBody += "RETRY_CNT=" + strCnt;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[port].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	sprintf(st_client[port].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + port, SERVER_SEND, port);
	
	m_nrMasterInfo = NR_WAITING;
}

void CBarcodeFormatClient::OnBoatIDModify( int port, CString lotid, CString boatid, CString strSerial, CString strModify, CString strRetry, CString strCnt )
{
	CString strBody="";
	CString strData="";
	//	int nLength;
	//	int nPos;
	
	strBody = "";
	m_nrSerialFormat = NR_NONE;
	
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
	
}


void CBarcodeFormatClient::OnDataReceive(CString strMsg)
{
	int nHead;
	int nLength;
	int nPos;

	CString strTmp="";
	CString strData="";

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

void CBarcodeFormatClient::OnDataAnalysis( CString strRecv )
{
	CString strFunc;
	CString strData;

	strFunc = OnBodyAnalysis( 0, strRecv, "FUNCTION_RPY" );
	if	   ( strFunc == "PK_XCLOSE")				 OnReceived_DensityLabelIssue( strRecv );
	else if( strFunc == "BRAND_DTBT_LABEL_ISSUE" )	 OnReceived_BcrLabelIssue( strRecv );
	else if( strFunc == "BRAND_DTBT_LABEL_CCS" )	 OnReceived_DTBT_Label_CCS( strRecv );
	else if( strFunc == "BRAND_DENSITY_LABEL_ISSUE") OnReceived_DensityLabelIssue( strRecv );
	else if( strFunc == "BRAND_DENSITY_LABEL_CCS")	 OnReceived_Density_Label_CCS( strRecv );
	else if( strFunc == "PROD_SERIAL_INFO")			 OnReceived_ProdSerialInfo( strRecv );
	else if( strFunc == "BOAT_ID_INFO")				 OnReceived_BoatIDInfo( strRecv );
	else if( strFunc == "PACKING_MASTER_INFO")		 OnReceived_PackingMasterInfo( strRecv );
	else if( strFunc == "BOAT_ID_MODIF")			 OnReceived_BoatIDModify( strRecv );

}

CString CBarcodeFormatClient::OnBodyAnalysis( int nMode, CString strRecv, CString strFind )
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
	else if( nMode == 3 )
	{
		nPos[2] = -1;
		nPos[2] = strRecv.Find("^XZ", nPos[1]+1);
		
		strData = strRecv.Mid(nPos[1]+1, (nPos[2] +3)-(nPos[1]+1));
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

void CBarcodeFormatClient::OnReceived_BcrLabelIssue( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		CString strLotID = OnBodyAnalysis( 0, strRecv, "LOT_ID");
		CString strBoatID = OnBodyAnalysis( 0, strRecv, "BOAT_ID");
		CString strSerialID = OnBodyAnalysis( 0, strRecv, "SERIAL");
		CString strLabelSerial = OnBodyAnalysis( 0, strRecv, "DRBT_LABEL_SERIAL");
		CString strFormat = OnBodyAnalysis( 3, strRecv, "FORMAT");
		ClrVariable();

		SetLotID( strLotID ); SetBoatID( strBoatID ); SetSerial( strSerialID );
		SetLabelSerial( strSerialID );
		SetBcrFormat( strFormat );

		m_nrSerialFormat = NR_RECEIVED;

	}
	else
	{
		m_nrSerialFormat = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_DensityLabelIssue( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		m_nrSerialFormat = NR_RECEIVED;
	}
	else
	{
		m_nrSerialFormat = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}

	
}

void CBarcodeFormatClient::OnReceived_XClose( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		m_nrSerialFormat = NR_RECEIVED;
	}
	else
	{
		m_nrSerialFormat = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_Density_Label_CCS( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		m_nrDensityCCS = NR_RECEIVED;
	}
	else
	{
		m_nrDensityCCS = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_DTBT_Label_CCS( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		m_nrBcrLabelCCS = NR_RECEIVED;
	}
	else
	{
		m_nrBcrLabelCCS = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_ProdSerialInfo( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		CString strLotID = OnBodyAnalysis( 0, strRecv, "LOT_ID");
		CString strBoatID = OnBodyAnalysis( 0, strRecv, "BOAT_ID");
		CString strSerialID = OnBodyAnalysis( 0, strRecv, "SERIAL");
		CString strPartID = OnBodyAnalysis( 0, strRecv, "PART_ID");
		ClrVariable();

// 		st_mes_data.srt_LOT_ID = strLotID;
// 		st_mes_data.srt_BOAT_ID = strBoatID;
// 		st_mes_data.srt_PART_ID = strPartID;

		m_nrSerialFormat = NR_RECEIVED;
	}
	else
	{
		m_nrSerialFormat = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_BoatIDInfo( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		CString strLotID = OnBodyAnalysis( 0, strRecv, "LOT_ID");
		CString strBoatID = OnBodyAnalysis( 0, strRecv, "BOAT_ID");
		CString strSerialID = OnBodyAnalysis( 0, strRecv, "SERIAL");
		CString strPartID = OnBodyAnalysis( 0, strRecv, "PART_ID");
		ClrVariable();

// 		st_mes_data.srt_LOT_ID = strLotID;
// 		st_mes_data.srt_BOAT_ID = strBoatID;
// 		st_mes_data.srt_SERIAL = strSerialID;
// 		st_mes_data.srt_PART_ID = strPartID;

		m_nrSerialFormat = NR_RECEIVED;
	}
	else
	{
		m_nrSerialFormat = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_BoatIDModify( CString strRecv )
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
		
		
	}
	else
	{
		m_nrSerialFormat = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}

void CBarcodeFormatClient::OnReceived_PackingMasterInfo( CString strRecv )
{
	CString strData;
	
	strData = OnBodyAnalysis(0, strRecv, "RESULT");
	if( strData == "PASS" )
	{
		CString strLotID = OnBodyAnalysis( 0, strRecv, "LOT_ID");
		CString strPartID = OnBodyAnalysis( 0, strRecv, "PART_ID");
		CString strLotQty = OnBodyAnalysis( 0, strRecv, "LOT_QTY");
		CString strBoatID = OnBodyAnalysis( 0, strRecv, "BOAT_ID");
		CString strProductID = OnBodyAnalysis( 0, strRecv, "PRODUCT_NAME");
		CString strTKCode = OnBodyAnalysis( 0, strRecv, "TK_CODE");
		CString strDensity_mat_code = OnBodyAnalysis( 0, strRecv, "DENSITY_MAT_CODE");
		CString strDtbt_label_mat_code = OnBodyAnalysis( 0, strRecv, "DTBT_LABEL_MAT_CODE");
		CString strRibbon_mat_code = OnBodyAnalysis( 0, strRecv, "RIBBON_MAT_CODE");
		CString strDensity_type = OnBodyAnalysis( 0, strRecv, "DENSITY_LABEL_VALUE");//2016.0722 DENSITY_LABEL_TYPE
		ClrVariable();

		st_mes_data.srt_LOT_ID = strLotID;
		st_mes_data.srt_PART_ID = strPartID;
		st_mes_data.srt_LotQty = strLotQty;
		st_mes_data.srt_BOAT_ID = strBoatID;
		st_mes_data.srt_PROD_SRLNO = strProductID;
		st_mes_data.srt_TK_CODE = strTKCode;
		st_mes_data.srt_DEN_LABEL_MAT_CODE = strDensity_mat_code;
		st_mes_data.srt_DTBT_MAT_CODE = strDtbt_label_mat_code;
		st_mes_data.srt_RIBBON_MAT_CODE = strRibbon_mat_code;
		st_mes_data.srt_DENSITY_Type = strDensity_type;


// 		if(g_lotMgr.GetLotCount() <= 0)
// 		{
// 			g_lotMgr.OnNewLotIn(strLotID, strPartID);
// 			g_lotMgr.GetLotByLotID(strLotID).SetLotEnd("NO");
// 			g_lotMgr.GetLotAt(0).SetLotTime();
//  			g_lotMgr.GetLotAt(0).SetDenLabelCode( strDensity_mat_code );
// 			g_lotMgr.GetLotAt(0).SetDisLabelCode( strDtbt_label_mat_code );
// 			g_lotMgr.GetLotAt(0).SetRibbonCode( strRibbon_mat_code );
// 			g_lotMgr.GetLotAt(0).SetTotalModule( strLotQty );
// 			g_lotMgr.GetLotAt(0).SetDenLabelCapa(strDensity_type);
// 		}


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
			g_lotMgr.GetLotByLotID(strLotID).SetDisLabelCode( strDtbt_label_mat_code );
			g_lotMgr.GetLotByLotID(strLotID).SetRibbonCode( strRibbon_mat_code );
			g_lotMgr.GetLotByLotID(strLotID).SetTotalModule( strLotQty );
			g_lotMgr.GetLotByLotID(strLotID).SetDenLabelCapa(strDensity_type);
		}

		m_nrMasterInfo = NR_RECEIVED;
	}
	else
	{
		m_nrMasterInfo = NR_FAIL;
		CString strMsg = OnBodyAnalysis( 0, strRecv, "MSG");
		ClrMsg(); SetMsg(strMsg);
	}
}