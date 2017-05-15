#if !defined(AFX_CLIENTSOCKET_H__2A49E7C6_67E3_11D3_8401_006097663D30__INCLUDED_)
#define AFX_CLIENTSOCKET_H__2A49E7C6_67E3_11D3_8401_006097663D30__INCLUDED_

#include "Ctlbd_Variable.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//


// CClientSocket command target
#define MAX_RECEIVE_LEN	4096
#include <afxSock.h>

class CClientSocket : public CSocket
{
// Attributes
public:
	BYTE	m_by_rev[1024];
	int		m_n_rev_length;
	int     m_n_address;
	int		m_Check_Timeout;

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();

//	BOOL Create(int n_address, CString str_ip, int n_port, int n_timeout);
	BOOL Create(int address, CString szAddress, UINT nPortNo, int nReceiveDataSize=MAX_RECEIVE_LEN);

// Overrides
public:
	BOOL IsConnect();
	int     m_naddress;
	BYTE*	m_byteReceive;	// 수신 버퍼
	int		m_nReceiveLen;	// 수신 데이터 길이

	int		mn_port;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual BOOL OnMessagePending();
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	int		m_nReceiveSize;	// 수신 데이터의 최대 길이
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__2A49E7C6_67E3_11D3_8401_006097663D30__INCLUDED_)
