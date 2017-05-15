// ClientSocket.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "ClientSocket.h"
#include "Variable.h"			// 전역 변수 정의 클래스 추가 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
//{{AFX_MSG_MAP(CClientSocket)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

BOOL CClientSocket::Create(int address, CString szAddress, UINT nPortNo, int nReceiveSize)
{
	if (!CSocket::Create())
	{
		return FALSE;
	}

	CSocket::m_nTimeOut = 1;
	m_Check_Timeout = 0;


	if (!CSocket::Connect(szAddress, nPortNo))
	{
		CSocket::Close();
		return FALSE;
	}
	m_nReceiveLen  = 0;
 
	m_nReceiveSize = nReceiveSize;
//	m_byteReceive = new BYTE[m_nReceiveSize];
	
	m_naddress = address;

	mn_port = nPortNo;			// 추가.

	return TRUE;
}

//BOOL CClientSocket::Create(int n_address, CString str_ip, int n_port, int n_timeout)
//{
//	if (!CSocket::Create())
//	{
//		return FALSE;
//	}
//
//	CSocket::m_nTimeOut = n_timeout;
//	if(!CSocket::Connect(str_ip, n_port))
//	{
//		CSocket::Close();
//		return FALSE;
//	}
//	
//	m_n_address = n_address;
//	return TRUE;
//}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{
	// 데이터 읽기

//	if (m_naddress >= 2)
//	{
		int i;
		memset(st_client[m_naddress].ch_rev, 0x00, 8192);
		// 데이터 읽기
		for (i = 0;i < 200000; i++);
		
		st_client[m_naddress].n_rev_length = Receive(st_client[m_naddress].ch_rev, 8192);

		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG+m_naddress, CLIENT_REV, m_naddress);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
//	}
//	else
//	{
// 		m_nReceiveLen = Receive(m_byteReceive, m_nReceiveSize);
// 
// 		// 통지 : SCM_RECEIVE
// 		g_Net.OnClientReceived( this );
//	}

	CSocket::OnReceive(nErrorCode);
}

int CClientSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CSocket::Receive(lpBuf, nBufLen, nFlags);
}

void CClientSocket::OnClose(int nErrorCode) 
{

//	if (m_naddress >= 2)
//	{
		st_client[m_naddress].n_rev_length = Receive(st_client[m_naddress].ch_rev, 8192);
		
		::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1+m_naddress, CLIENT_CLOSE, m_naddress);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
//	}
//	else
//	{
//		g_Net.OnClientClosed( this );
//	}


	CSocket::OnClose(nErrorCode);
}

void CClientSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnAccept(nErrorCode);
}

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnConnect(nErrorCode);
}

BOOL CClientSocket::IsConnect()
{
	return 0;
}

BOOL CClientSocket::OnMessagePending() 
{
	// TODO: Add your specialized code here and/or call the base class
	MSG Message;
	
	if (::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		m_Check_Timeout++;
		if (m_Check_Timeout > 10)
		{
			m_Check_Timeout = 0;
			CancelBlockingCall();
			Close();
		}
	}
	
	return CSocket::OnMessagePending();
}

BOOL CClientSocket::ConnectHelper(const SOCKADDR *lpSockAddr, int nSockAddrLen)
{
	CTime  curt, st;
	CTimeSpan span(0, 0, 0, m_nTimeOut);

	if (m_pbBlocking != NULL)
	{
		WSASetLastError(WSAEINPROGRESS);
		return  FALSE;
	}

	m_nConnectError = -1;

	if(!CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen))
	{
		if(GetLastError() == WSAEWOULDBLOCK)
		{
			st = CTime().GetCurrentTime();

			while(PumpMessages(FD_CONNECT))
			{
				if(m_nConnectError != -1)
				{
					WSASetLastError(m_nConnectError);
					return (m_nConnectError == 0);
				}

				curt = CTime().GetCurrentTime();
				
				if(curt > (st + span))
				{
					return FALSE;
				}
			}
		}
		return FALSE;
	}
	return TRUE;
}
