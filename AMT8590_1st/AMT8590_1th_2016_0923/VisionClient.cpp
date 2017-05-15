// VisionClient.cpp: implementation of the CVisionClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HANDLER.h"
#include "VisionClient.h"
#include "Variable.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVisionClient clsVsClient[10];

CVisionClient::CVisionClient()
{
	m_strHeader = _T("");
	m_nHeader	= 16;
}

CVisionClient::~CVisionClient()
{

}

bool CVisionClient::OnVisionModelChange(int nPort, CString strModel)
{
	CString strBody="";
	CString strData="";
	
	strBody = "";
	
	if (st_client[nPort].n_connect == CTL_NO) return false;

	strBody = "FUNCTION=BCR_CJ";
	strBody += " ";
	strModel.MakeUpper();
	strModel.Replace(".TXT", "");
	st_handler.m_strVisModel.TrimLeft(' ');               
	st_handler.m_strVisModel.TrimRight(' ');
	Func.On_LogFile_Add(99, st_handler.m_strVisModel);
// 	strBody += "NAME=" + strModel;
	if( st_handler.m_strVisModel == "" ) strBody += "NAME="	+ strModel;
	else
	{
		if(nPort == 10 || nPort == 11)//density,bcr label
		{
			strModel = g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetLabelVisionModel();
			st_basic.mstr_bcr_model = strModel;
		}
		else// if(nPort == 8 || nPort == 9)//top,btm label
		{
			strModel = g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetTBtmVisionModel();
			st_basic.mstr_btm_model = strModel;
		}
		strBody += "NAME="	+ strModel;
	}
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort);
	
	sprintf(st_client[nPort].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort);

	return true;
}

bool CVisionClient::OnVisionBcrResult(int nPort)
{
	CString strBody="";
	CString strData="";

	strBody = "";

	if (st_client[nPort].n_connect == CTL_NO) return false;

	strBody = "FUNCTION=VISION_DATA";

	strData.Format("%016d", strBody.GetLength());
	sprintf(st_client[nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort);
	
	sprintf(st_client[nPort].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nPort, CLIENT_SEND, nPort);

	return true;
}


void CVisionClient::OnDataReceive(CString strMsg)
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


CString	CVisionClient::OnBodyAnalysis(int nMode, CString strRev, CString strFind)
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
	else if(nMode == 1)
	{
		nPos[2] = -1;
		nPos[2] = strRev.Find(")", nPos[1]+1);
		
		strData = strRev.Mid(nPos[1]+1, nPos[2]-(nPos[1]));
	}
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
	
	return strData;
}

void CVisionClient::OnDataAnalysis(CString strRev)
{
	CString strFunc="";
	CString strData="";

	// 헤더 정보를 가지고 옴....
	strFunc	= OnBodyAnalysis(0, strRev, "FUNCTION_RPY");

	if (strFunc == "BCR_CJ")
	{
		strData = OnBodyAnalysis(0, strRev, "RESULT");
		
		if(strData == "PASS")
		{
			m_nModelChangeBin = NVR_PASS;
		}
		else
		{
			m_nModelChangeBin = NVR_FAIL;
			m_strMsg = OnBodyAnalysis(4, strRev, "MSG");
		}
	}
	else if (strFunc == "VISION_DATA")
	{


		strData = OnBodyAnalysis(0, strRev, "RESULT");
		
		if(strData == "PASS")
		{
			m_strModel		= _T("");	
			m_strBarcode	= _T("");
			m_strMsg		= _T("");	
			m_strCapa		= _T("");
			m_str2D			= _T("");
			m_strSerial		= _T("");
			m_strUpc		= _T("");
			m_strEan		= _T("");
			m_nBcrResultBin = NVR_PASS;

			m_strModel		= OnBodyAnalysis(0,	strRev, "MODEL");
			m_strBarcode	= OnBodyAnalysis(0, strRev, "BCR");
			m_strCapa		= OnBodyAnalysis(0,	strRev, "CAPA");
			m_str2D			= OnBodyAnalysis(0, strRev, "2D");
			//2017.0510
			m_str2D.TrimRight(); m_str2D.TrimLeft();
			m_str2D.TrimRight(' '); m_str2D.TrimLeft(' ');
			if( m_str2D.GetLength() > 11 )
				m_str2D = m_str2D.Left(11);
			m_strSerial		= OnBodyAnalysis(0, strRev, "SERIAL");
			m_strUpc		= OnBodyAnalysis(0, strRev, "UPC_SERIAL");
			m_strEan		= OnBodyAnalysis(0, strRev, "EAN_SERIAL");
		}
		else
		{
			m_nBcrResultBin = NVR_FAIL;
			m_strMsg = OnBodyAnalysis(4, strRev, "MSG");

			m_strModel		= OnBodyAnalysis(0,	strRev, "MODEL");
			m_strBarcode	= OnBodyAnalysis(0, strRev, "BCR");
			m_strCapa		= OnBodyAnalysis(0,	strRev, "CAPA");
			m_str2D			= OnBodyAnalysis(0, strRev, "2D");
			//2017.0510
			m_str2D.TrimRight(); m_str2D.TrimLeft();
			m_str2D.TrimRight(' '); m_str2D.TrimLeft(' ');
			if( m_str2D.GetLength() > 11 )
				m_str2D = m_str2D.Left(11);
			m_strSerial		= OnBodyAnalysis(0, strRev, "SERIAL");
		}
	}
}
