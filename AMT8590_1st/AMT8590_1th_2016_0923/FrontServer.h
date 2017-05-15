#if !defined(AFX_FRONTSERVER_H__A27D1234_41C3_43E7_84C3_BA2A39E8E771__INCLUDED_)
#define AFX_FRONTSERVER_H__A27D1234_41C3_43E7_84C3_BA2A39E8E771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrontServer.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CFrontServer command target

class CFrontServer : public CObject
{
public:
	CFrontServer();           // protected constructor used by dynamic creation
	virtual ~CFrontServer();

// Attributes
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;

// Operations
public:
	int GetPort() { return m_nPort; }
	void SetPort( int port) { m_nPort = port;}
	
	//8580
	void OnReceived_EMG_Stop(CString strRecv);
	bool GetEmgStop() { return (m_nrEMGStopIn == NR_RECEIVED); }
	void SetEmgStop( NET_RECEIVED nr ) { m_nrEMGStopIn = nr; }

	void OnDataReceive(CString strMsg);
	CString	OnBodyAnalysis(int nMode, CString strRev, CString strFind);
	void OnDataAnalysis(CString strRev);


	//top
	void OnReceived_BufferIn( CString strRecv );
	void SetBufferIn( NET_RECEIVED nr ) { m_nrBufferIn = nr; }
	bool GetBufferInReceived() { return (m_nrBufferIn == NR_RECEIVED); }

protected:
	int			m_nPort;
	NET_RECEIVED	m_nrBufferIn;
	NET_RECEIVED	m_nrBufferInfo;
	NET_RECEIVED	m_nrEMGStopIn;

};

extern CFrontServer g_front_server;

#endif // !defined(AFX_FRONTSERVER_H__A27D1234_41C3_43E7_84C3_BA2A39E8E771__INCLUDED_)
