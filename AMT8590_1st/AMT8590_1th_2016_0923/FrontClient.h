#if !defined(AFX_FRONTCLIENT_H__7CF18025_A444_4829_A747_79800206C6F2__INCLUDED_)
#define AFX_FRONTCLIENT_H__7CF18025_A444_4829_A747_79800206C6F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrontClient.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// FrontClient command target

class CFrontClient : public CObject
{
public:
	CFrontClient();           // protected constructor used by dynamic creation
	virtual ~CFrontClient();

// Attributes
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;
	int			m_nPort;
	
// Operations
public:
	void OnMove_EMG_Stop();
	void OnDataReceive(CString strMsg);
	CString	OnBodyAnalysis(int nMode, CString strRev, CString strFind);
	void OnDataAnalysis(CString strRev);
	void OnReceived_EMG_StopRpy( CString strRecv );


protected:
	NET_RECEIVED m_nrEmgInfo;

};

/////////////////////////////////////////////////////////////////////////////
extern CFrontClient g_front_client;

#endif // !defined(AFX_FRONTCLIENT_H__7CF18025_A444_4829_A747_79800206C6F2__INCLUDED_)
