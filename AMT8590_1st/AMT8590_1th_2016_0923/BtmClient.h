#if !defined(AFX_BTMCLIENT_H__15AC8C0C_A672_4555_871C_55D20B33A81A__INCLUDED_)
#define AFX_BTMCLIENT_H__15AC8C0C_A672_4555_871C_55D20B33A81A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BtmClient.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CBtmClient command target

class CBtmClient : public CObject
{
public:
	CBtmClient();           // protected constructor used by dynamic creation
	virtual ~CBtmClient();

// Attributes
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;
	int			m_nRetry;

public:
	//btm
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
extern CBtmClient g_btm_client;

#endif // !defined(AFX_BTMCLIENT_H__15AC8C0C_A672_4555_871C_55D20B33A81A__INCLUDED_)
