#include "StdAfx.h"
#include "InterfaceGms.h"
#include "Variable.h"
#include "Public_Function.h"
#include "Comm.h"

CInterfaceGms clsGms;

UINT OnInterfaceGmpMessage(LPVOID lParam);

CInterfaceGms::CInterfaceGms(void)
{
	m_nChann[0]		= 1;
	m_nChann[1]		= 0;
	m_nComPort		= -1;		// ��� ��Ʈ
	m_nBaudRate		= -1;		// ��� �ӵ�
	m_nDataBit		= -1;		// ������ ��Ʈ
	m_nStopBit		= -1;		// stop ��Ʈ
	m_nParityBit	= -1;		// parity ��Ʈ

	m_byEnd			=0x03;

	m_bConnect		= false;	// rs-232c ���� ����
	m_bThread		= false;
	m_bConnect		= false;
	m_bRevFlag		= true;

	m_thrHandle		= NULL;

	m_pCom			= NULL;		// ��� class ������
}


CInterfaceGms::~CInterfaceGms(void)
{
	if (m_thrHandle) 
	{
		m_bThread = false;
		while(WaitForSingleObject(m_thrHandle, 1500))
		{
			if (m_thrHandle == NULL) break;
		}
	}

	if (m_pCom != NULL)
	{
		delete m_pCom;
		m_pCom = NULL;
	}
}


UINT OnInterfaceGmsMessage(LPVOID lParam)
{
	CInterfaceGms *pMsg = (CInterfaceGms *)lParam;

	CString strMsg;
	CString strTemp;
	int		nLength;
	BYTE	byData[100];

	while(pMsg->m_bThread)
	{
		switch (pMsg->m_nThreadStep)
		{
			case 0:
				pMsg->m_dwWaitTime[0] = GetTickCount();

				pMsg->m_nThreadStep = 100;
				break;

			case 100:
				pMsg->m_dwWaitTime[1] = GetTickCount();
				pMsg->m_dwWaitTime[2] = pMsg->m_dwWaitTime[1] - pMsg->m_dwWaitTime[0];

				if (pMsg->m_dwWaitTime[2] <= (DWORD)0)
				{
					pMsg->m_dwWaitTime[0] = GetTickCount();
					break;
				}

				if (pMsg->m_dwWaitTime[2] > (DWORD)100)
				{
					if (pMsg->m_pCom->m_bRevFlag)
					{
						nLength = pMsg->m_pCom->m_nLength;
						if (nLength > 0)
						{
							if (nLength > 100)
							{
								pMsg->m_pCom->Empty();
							}
							else
							{
								pMsg->m_pCom->ReadData(byData, nLength);

								strTemp.Format("%s", byData);
								strMsg	= strTemp.Mid(0, nLength);

								pMsg->OnDataRevice(strMsg);
							}
						}
					}

					pMsg->m_nThreadStep = 0;
				}
				break;
		}
	}

	pMsg->m_thrHandle = NULL;

	return 0;
}


bool CInterfaceGms::OnOpen(int nPort, int nBaudRate, int nParityBit, int nDataBit, int nStopBit, BYTE byEnd)
{
	if (m_pCom != NULL)
	{
		delete m_pCom;
		m_pCom = NULL;
	}

	m_pCom = new CComm;
	if (m_pCom->OpenSerial(nPort, nBaudRate, nDataBit, nStopBit, nParityBit))
	{
		m_bConnect		= true;
		m_nComPort		= nPort;
		m_nBaudRate		= nBaudRate;
		m_nParityBit	= nParityBit;
		m_nDataBit		= nDataBit;
		m_nStopBit		= nStopBit;

		m_pCom->m_byEnd	= byEnd;

		m_nThreadStep	= 0;
		m_bThread		= true;
		m_thrHandle		= AfxBeginThread(OnInterfaceGmsMessage, this);

		return true;
	}
	else
	{
		delete m_pCom;
		m_pCom = NULL;
	}

	m_bConnect = false;

	return false;
}


void CInterfaceGms::OnClose()
{
	if (m_thrHandle) 
	{
		m_bThread = false;
		while(WaitForSingleObject(m_thrHandle, 1500))
		{
			if (m_thrHandle == NULL) break;
			Sleep(5);
		}
	}

//	AfxMessageBox(_T("End"));

	if (m_pCom != NULL)
	{
		m_pCom->CloseConnection();

		while(m_pCom->idComDev != NULL)
		{
			if (m_pCom->idComDev == NULL) break;

			Sleep(5);
		}

		delete m_pCom;
		m_pCom = NULL;

		m_bConnect = false;
	}
}


void CInterfaceGms::OnDataSend(CString strData)
{
	BYTE byData[100];
	int  nLength = strData.GetLength();

	// com�� ����Ǿ� ���� ������
	if (m_pCom == NULL) return; 

	m_pCom->Empty(); // com buffer ����

//	clsFunc.OnStringToChar(strData, (char*)byData);
	sprintf((char*)byData, strData);
	m_pCom->WriteCommBlock((LPSTR)byData, nLength);
}


void CInterfaceGms::OnDataRevice(CString strData)
{
	int     nLength;  // ������ �� ���� ���� ���� 
	CString strBody;  // BODY ������ ���� ���� 
	CString strTemp;
	
	int nChannel;
	double dValue;

	// **************************************************************************
	// ���� ������ �ּ� ���� �˻��Ѵ� [24�ڸ��� �Ǿ� �Ѵ�.]
	// **************************************************************************
	nLength = strData.GetLength();
	if (nLength != 25) return;		
	// **************************************************************************
	
	strBody = strData.Mid(2, nLength - 3);  // [���+����] ���� 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	strTemp = strBody.Mid(11, 1);			// ä��.
	nChannel = atoi(strTemp);
	
	if (nChannel <= 0) return;

	strTemp = strBody.Mid(12, 5);
	dValue = atof(strTemp);
	
	m_dChann[nChannel-1] = dValue;	// ���� �ִ´�.
	
	if (strBody.Mid(12, 2) == "OP")
	{
		m_strChann[nChannel-1] = _T("OPEN");
		m_dChann[nChannel-1] = -99999;	// ���� �ִ´�.
	}
	else if (strBody.Mid(12, 2) == "OV")
	{
		m_strChann[nChannel-1] = _T("OVER");
		m_dChann[nChannel-1] = -99998;	// ���� �ִ´�.
	}
	else
	{
		m_strChann[nChannel-1].Format(_T("%0.2f"), dValue);
	}

	m_nChann[0] = nChannel + 1;
	if (m_nChann[0] > 5)
	{
		m_nChann[0] = 1;
	}

	if (st_handler.cwnd_list != NULL)  //2010.1027 ����Ʈ �� ȭ�� ����
	{
		sprintf(st_msg.c_normal_msg, "[GMS] Recv_GmsData : %s",strData);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	}
}

void CInterfaceGms::OnGmsDataCheck()
{
	CString strComm;

	if (m_nChann[0] == m_nChann[1])
	{
		m_dwWaitTime[1] = GetTickCount();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];

		if (m_dwWaitTime[2] <= 0)
		{
			m_dwWaitTime[0] = GetTickCount();

			return;
		}

		if (m_dwWaitTime[2] > 5000)
		{
			m_nChann[0] = m_nChann[0] + 1;
			strComm.Format(_T("%c%cP%-8sCH%d %c"), 0x10, 0x02, _T("SAMSUNG1"), m_nChann[0], 0x03);
			OnDataSend(strComm);
			m_dwWaitTime[0] = GetTickCount();
		}

		return;
	}

	m_nChann[1] = m_nChann[0];

	m_dwWaitTime[0] = GetTickCount();

	strComm.Format(_T("%c%cP%-8sCH%d %c"), 0x10, 0x02, _T("SAMSUNG1"), m_nChann[0], 0x03);
	OnDataSend(strComm);
}
