// VisionServer.cpp: implementation of the CVisionServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HANDLER.h"
#include "VisionServer.h"
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
CVisionServer clsVsServer[10];

CVisionServer::CVisionServer()
{
	m_strHeader = _T("");
	m_nHeader	= 16;
}

CVisionServer::~CVisionServer()
{

}

void CVisionServer::OnVisionModelWork(int nPort, CString strModel, CString strMsg)
{
	CString strBody="";
	CString strData="";
	
	strBody = "";

	strBody = "FUNCTION_RPY=BCR_CJ_REQ";
	strBody += " ";
	strModel.MakeUpper();
	strModel.Replace(".TXT", "");

	if( st_handler.m_strVisModel == "" || st_handler.m_strVisModel != g_lotMgr.GetLotAt(0).GetLotID() )
	{
		if(nPort == 6 || nPort == 7)
			strBody += "NAME="	+ st_basic.mstr_bcr_model;
		else
			strBody += "NAME="	+ st_basic.mstr_btm_model;
	}
	else
	{
// 		CString strpart = g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetPartID();
// 		if(strpart.GetAt(15) == 'A')//Only UPC Exist
// 		{
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetUpcEanType(true,false);
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetTBtmVisionModel( strModel, "_UPC" );
// 		}
// 		else if(strpart.GetAt(15) == 'E' || strpart.GetAt(15) == 'C' || strpart.GetAt(15) == 'K')//Only EAN Exist
// 		{
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetUpcEanType(false,true);				
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetTBtmVisionModel( strModel, "" );
// 		}
// 		else//Both of them exist
// 		{
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetUpcEanType(true,true);
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetTBtmVisionModel( strModel, "" );
// 		}
		if(nPort == 6 || nPort == 7)//density,bcr label
		{
			strModel = g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetLabelVisionModel();
		}
		else// if(nPort == 8 || nPort == 9)//top,btm label
		{
// 			g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetTBtmVisionModel("25INCHPRO","");
			strModel = g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetTBtmVisionModel();
		}

		strBody += "NAME="	+ strModel;
	}
	strBody += " ";
	strBody += "MSG="	+ strMsg;
	
	strData.Format("%016d", strBody.GetLength());
	sprintf(st_server[nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + nPort, SERVER_SEND, nPort);
	
	sprintf(st_server[nPort].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + nPort, SERVER_SEND, nPort);
}

void CVisionServer::OnVisionModelList(int nPort, CString strList, CString strMsg)
{
	CString strBody="";
	CString strData="";
//	int nLength;
//	int nPos;

	strBody = "";

	strBody = "FUNCTION_RPY=BCR_ML";
	strBody += " ";
	strList.Replace(".TXT", "");
	strBody += "NAME="	+ strList;
	strBody += " ";
	strBody += "MSG="	+ strMsg;
	
	strData.Format("%016d", strBody.GetLength(), strBody);
	sprintf(st_server[nPort].ch_send, strData);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + nPort, SERVER_SEND, nPort);
	
	sprintf(st_server[nPort].ch_send, strBody);
	::SendMessage(st_handler.hWnd, WM_SERVER_MSG + nPort, SERVER_SEND, nPort);
}

void CVisionServer::OnDataReceive(int nAddr, CString strMsg)
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
			OnDataAnalysis(nAddr, strTmp);
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
				OnDataAnalysis(nAddr, strTmp);
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
				OnDataAnalysis(nAddr, strTmp);

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
					OnDataAnalysis(nAddr, strTmp);

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
							OnDataAnalysis(nAddr, strTmp);
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
								OnDataAnalysis(nAddr, strTmp);
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

CString	CVisionServer::OnBodyAnalysis(int nMode, CString strRev, CString strFind)
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

void CVisionServer::OnDataAnalysis(int nAddr, CString strRev)
{
	CString strFunc="";
	CString strData="";
	
	// 헤더 정보를 가지고 옴....
	strFunc	= OnBodyAnalysis(0, strRev, "FUNCTION");

	if (strFunc == "BCR_CJ_REQ")
	{
		OnVisionModelWork(nAddr+6, st_basic.mstr_device_name, "");
	}
	else if (strFunc == "BCR_ML")
	{
		strData = OnModelList(st_path.mstr_path_dvc);// .mstr_basic);
		OnVisionModelList(nAddr+6, strData, "");	
	}
}


CString	CVisionServer::OnModelList(CString strPath)
{
//	int n_chk;						// 함수 리턴 플래그
    CString str_path = strPath;		// 폴더 설정
	CString strTmp="";
	CString strData = "";

	int nLength;
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			strTmp = OnAddItem(n_count, &fd);
			
			if (strTmp != "")
			{
				strData += strTmp;
			}
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				strTmp = OnAddItem(n_count, &fd);
				
				if (strTmp != "")
				{
					strData += strTmp;
					strData += ",";
				}
				else
				{
					break;
				}
			}
        }
		::FindClose(hFind);
    }

	nLength = strData.GetLength();
	
	strTmp = strData.GetAt(nLength-1);
	if (strTmp == ",")
	{
		strTmp	= strData.Mid(0, nLength-1);
		strData = strTmp;
	}
	return strData;
}


CString CVisionServer::OnAddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_dvc="";
	CString str_device = "";
	CString str_expansion="";
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_dvc = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_dvc.MakeUpper();
	
	/* ************************************************************************** */
	/* 파일 이름 중 확장자 확인                                                   */
	/* ************************************************************************** */
	n_pos = str_temp_dvc.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_dvc.Mid(n_pos + 1);
	/* ************************************************************************** */
	
	if (str_expansion != "TXT")
	{
		return str_device;	
	}
	
	str_device = str_temp_dvc;			// 파일명 전체 화면 출력
	

    return str_device;
}
