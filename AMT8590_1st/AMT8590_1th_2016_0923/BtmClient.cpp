// BtmClient.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "BtmClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBtmClient
CBtmClient g_btm_client;

CBtmClient::CBtmClient()
{
	m_strHeader = _T("");
	m_nHeader	= 16;
	m_nPort = CLS_BOTTOM;

}

CBtmClient::~CBtmClient()
{
}


/////////////////////////////////////////////////////////////////////////////
// CBtmClient message handlers
//FUNCTION=	BUFFER_IN	MDL_CNT=	2	MD_01=	( BUF=.. VAR=…)	MD_02=	( BUF=.. VAR=…)
void CBtmClient::OnMove_BufferIn()
{
	
//	int iIdx = g_Tray.GetIdxByPos( IPOOS_UNLOAD_BTM_UNLOAD );
//	if( iIdx < 0 )
//		return;

//	ATray tray = g_Tray.GetTray( iIdx );
	CString strBufferIn;

	int bufMdl = 0;
//
//	if( tray.GetModule(0).GetSN() != "NO") bufMdl++;
//	if( tray.GetModule(1).GetSN() != "NO") bufMdl++;

	if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[0] == 1 ) bufMdl++;
	if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[1] == 1 ) bufMdl++;


	strBufferIn.Format( "FUNCTION=BUFFER_IN MDL_CNT=%d ",bufMdl);

	for( int i=0; i<bufMdl; i++ )
	{
		CString strTemp;
//		AModule mdl = tray.GetModule(i - 1);
//		strTemp.Format( " MD_%02d=(PRODUCT_NAME=%s IDBUF=%s VAR=%s )", i, mdl.GetProductID(),  mdl.GetSN(), mdl.GetVar() );
		strTemp.Format( " MD_%02d=(PRODUCT_NAME=%s IDBUF=%d VAR=%s )", i+1, st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].c_product_id[i],
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuff_num[i], st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].c_var[i]);
		strBufferIn += strTemp;
	}

	CString strData="";
	strData.Format("%016d", strBufferIn.GetLength());
	sprintf(st_client[m_nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);
	
	sprintf(st_client[m_nPort].ch_send, strBufferIn);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nPort, CLIENT_SEND, m_nPort);

	m_nrBufferInfo = NR_WAITING;

}


void CBtmClient::OnDataReceive(CString strMsg)
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
CString	CBtmClient::OnBodyAnalysis(int nMode, CString strRev, CString strFind)
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

void CBtmClient::OnDataAnalysis(CString strRev)
{
	CString strFunc,strFunc1;
	CString strData;
	
	// 헤더 정보를 가지고 옴....
	strFunc	= OnBodyAnalysis(0, strRev, "FUNCTION_RPY");

	if (strFunc == "BUFFER_IN")		OnReceived_BufferInfo( strRev );
}

void CBtmClient::OnReceived_BufferInfo( CString strRecv )
{
	CString strResult = OnBodyAnalysis( 0, strRecv, "RESULT"  );

	if( strResult == "PASS")
	{
		m_nrBufferInfo = NR_RECEIVED;
	}
	else
	{
		m_nrBufferInfo = NR_FAIL;
	}

}
