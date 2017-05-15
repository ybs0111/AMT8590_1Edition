#if !defined(AFX_NEXTCLIENT_H__B715BF17_3EA2_413E_87E3_9176D413694E__INCLUDED_)
#define AFX_NEXTCLIENT_H__B715BF17_3EA2_413E_87E3_9176D413694E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NextClient.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CNextClient command target

class CNextClient : public CObject
{
public:
	CNextClient();           // protected constructor used by dynamic creation
	virtual ~CNextClient();

// Attributes
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;
	int			m_nRetry;
	

// Operations
public:
	void OnMove_BufferIn();
	void OnReceived_BufferInfo( CString strRecv );
	void SetBufferInfo( NET_RECEIVED nr ) { m_nrBufferInfo = nr; }
	bool GetBufferInfo() { return (m_nrBufferInfo == NR_RECEIVED); }
	
	
	void OnDataReceive(CString strMsg);
	CString	OnBodyAnalysis(int nMode, CString strRev, CString strFind);
	void OnDataAnalysis(CString strRev);

	// Operations
protected:
	int			m_nPort;
	NET_RECEIVED	m_nrBufferIn;
	NET_RECEIVED	m_nrBufferInfo;
	NET_RECEIVED	m_nrEMGStopIn;

};

/////////////////////////////////////////////////////////////////////////////
extern CNextClient g_next_client;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEXTCLIENT_H__B715BF17_3EA2_413E_87E3_9176D413694E__INCLUDED_)
