// ServerItem.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "handler.h"
#include "ServerSocket.h"
#include "Variable.h"		// 전역 변수 정의 클래스 추가 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerItem

CServerItem::CServerItem(CSocket* pServerSocket)
{
	m_ServerSocket = pServerSocket;
	m_nReceiveLen  = 0;

//	m_byteReceive = new BYTE[((CServerSocket*)m_ServerSocket)->m_nReceiveDataLen];

}

CServerItem::~CServerItem()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerItem, CSocket)
	//{{AFX_MSG_MAP(CServerItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerItem member functions

// 소켓 닫기
void CServerItem::OnClose(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;
	
	// 소켓 해제
	ServerSocket->CloseClient(this);
	// 규이리 위치 이동시킴 [2013.11.12]
	// 메인 프레임으로 메시지 전송하여 소켓을 종료시킴
//	::PostMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_CLOSE, ServerSocket->m_n_address);
	
	CSocket::OnClose(nErrorCode);
	
}

// 수신 데이터 처리
void CServerItem::OnReceive(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;
	
//	if (ServerSocket->m_n_address >= 2)
//	{
		memset(&st_server[ServerSocket->m_n_address].ch_rev, 0, sizeof(st_server[ServerSocket->m_n_address].ch_rev));
		
		//	sprintf(st_server[ServerSocket->m_n_address].ch_rev,"%s","LOTID=AAA,LEFCNT=00,MGZID=,PARTID=");
		// 데이터 읽기
		st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 8192);
		st_server[ServerSocket->m_n_address].ch_rev[st_server[ServerSocket->m_n_address].n_rev_length+1] = '\r';
		
		::SendMessage(st_handler.hWnd, WM_SERVER_MSG + ServerSocket->m_n_address, SERVER_REV, ServerSocket->m_n_address);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 	
//	}
//	else
//	{
//		m_nReceiveLen = Receive(m_byteReceive, ServerSocket->m_nReceiveDataLen);
//	
//		g_Net.OnServerReceived( this );
//	}
	
	CSocket::OnReceive(nErrorCode);

}
