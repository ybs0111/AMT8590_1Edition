// 8570Client.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "8570Client.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C8570Client
C8570Client g_8570_client;
C8570Client::C8570Client()
{
	m_strHeader = _T("");
	m_nHeader	= 16;
	m_nPort = CLS_8570;

}

C8570Client::~C8570Client()
{
}


/////////////////////////////////////////////////////////////////////////////
// C8570Client message handlers
//FUNCTION=	SOK_SKIP	EQP_ID=	SSD_LABELLER	SOK_CNT=	2	SOK_INFO=	34,24
void C8570Client::OnMove_SokskipIn( int soknum, CString strSock )
{
//FUNCTION=	SOK_SKIP	EQP_ID=	SSD_LABELLER	SOK_CNT=	2	SOK_INFO=	34,24
	CString strSokIn="";

	strSokIn.Format("FUNCTION=SOK_SKIP EQP_ID=%s SOK_CNT=%d SOK_INFO=%s", st_lamp.mstr_equip_id, soknum, strSock);
 	
	CString strData="";
	strData.Format("%016d", strSokIn.GetLength());
	sprintf(st_client[m_nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
	sprintf(st_client[m_nPort].ch_send, strSokIn);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	m_nrSokSkipIn = NR_WAITING;

}


void C8570Client::OnDataReceive(CString strMsg)
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
CString	C8570Client::OnBodyAnalysis(int nMode, CString strRev, CString strFind)
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

void C8570Client::OnDataAnalysis(CString strRev)
{
	CString strFunc,strFunc1;
	CString strData;
	
	// 헤더 정보를 가지고 옴....
	strFunc	= OnBodyAnalysis(0, strRev, "FUNCTION");
	strFunc1	= OnBodyAnalysis(0, strRev, "FUNCTION_RPY");

	if	    ( strFunc == "NEW_LOT_IN" )		OnReceived_NewLotIn( strRev );
	else if (strFunc1 == "SOK_SKIP")		OnReceived_SokSkipRpy( strRev ); //8570
}

void C8570Client::OnReceived_NewLotIn( CString strRecv )
{
	CString strEqpID =		OnBodyAnalysis( 0, strRecv, "EQPID" );
	CString strLotID =		OnBodyAnalysis( 0, strRecv, "LOTID" );
	CString strPartID =		OnBodyAnalysis( 0, strRecv, "PART_ID" );
	CString strLotStep =	OnBodyAnalysis( 0, strRecv, "LOT_STEP" );
	CString strQty =		OnBodyAnalysis( 0, strRecv, "LOT_QTY" );
	CString strProductName = OnBodyAnalysis( 0, strRecv, "PRODUCT_NAME" );
	CString strDistributeLabelMassCode = OnBodyAnalysis( 0, strRecv, "DTBT_LABEL_MAT_CODE");
	CString	strRibbonCode	= OnBodyAnalysis( 0, strRecv, "RIBBON_MAT_CODE" );
	CString	strCapa	= OnBodyAnalysis( 0, strRecv, "DENSITY" );

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
		if( st_basic.mn_mode_run == CTL_NO)
			g_lotMgr.OnNewLotIn(strLotID, strPartID);
		else
		{
			st_handler.m_strNewLot = strLotID;
// 			st_var.n_newlotin = CTL_YES;
		}
	}

// 	st_handler.m_strNewLot = strLotID;
// 	st_var.n_newlotin = CTL_YES;

	return;
	



	if(g_lotMgr.GetLotCount() <= 0)
	{
		g_lotMgr.OnNewLotIn(strLotID, strPartID);
		g_lotMgr.GetLotByLotID(strLotID).SetLotEnd("NO");
		g_lotMgr.GetLotAt(0).SetLotTime();
		g_lotMgr.GetLotAt(0).SetDisLabelCode( strDistributeLabelMassCode );
		g_lotMgr.GetLotAt(0).SetRibbonCode( strRibbonCode );
		g_lotMgr.GetLotAt(0).SetTotalModule( strQty );
		g_lotMgr.GetLotAt(0).SetDenLabelCapa(strCapa);

		st_mes_data.srt_LOT_ID = strLotID;
		st_mes_data.srt_PART_ID = strPartID;
		st_mes_data.srt_LotQty = strQty;
		st_mes_data.srt_PROD_SRLNO = strProductName;
// 		st_mes_data.srt_TK_CODE = strTKCode;
// 		st_mes_data.srt_DEN_LABEL_MAT_CODE = strDensity_mat_code;
		st_mes_data.srt_DTBT_MAT_CODE = strDistributeLabelMassCode;
		st_mes_data.srt_RIBBON_MAT_CODE = strRibbonCode;
		st_mes_data.srt_DENSITY_Type = strCapa;

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
			st_mes_data.srt_LOT_ID = strLotID;
			st_mes_data.srt_PART_ID = strPartID;
			st_mes_data.srt_LotQty = strQty;
			//st_mes_data.srt_BOAT_ID = strBoatID;
			st_mes_data.srt_PROD_SRLNO = strProductName;
			//st_mes_data.srt_TK_CODE = strTKCode;
			//st_mes_data.srt_DEN_LABEL_MAT_CODE = strDensity_mat_code;
			st_mes_data.srt_DTBT_MAT_CODE = strDistributeLabelMassCode;
			st_mes_data.srt_RIBBON_MAT_CODE = strRibbonCode;
			st_mes_data.srt_DENSITY_Type = strCapa;
		}
	}
}


//FUNCTION_RPY=	SOK_SKIP	EQP_ID=	SSD_LABELLER	RESULT=	PASS
void C8570Client::OnReceived_SokSkipRpy( CString strRecv )
{
	CString strResult = OnBodyAnalysis( 0, strRecv, "RESULT" );
	if( strResult == "PASS")
	{
		m_nrSokSkipIn = NR_RECEIVED;
	}
	else
	{
		m_nrSokSkipIn = NR_FAIL;
	}
}
