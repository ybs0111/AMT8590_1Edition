#if !defined(AFX_8570CLIENT_H__A387EA97_2B36_4F50_AEC2_CF8242FBD6D1__INCLUDED_)
#define AFX_8570CLIENT_H__A387EA97_2B36_4F50_AEC2_CF8242FBD6D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 8570Client.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// C8570Client command target

class C8570Client : public CObject
{
public:
	C8570Client();           // protected constructor used by dynamic creation
	virtual ~C8570Client();

// Attributes
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;

// Operations
public:
	void OnReceived_NewLotIn( CString strRecv );	

	void OnMove_SokskipIn( int soknum, CString strSock );
	void OnReceived_SokSkipRpy( CString strRecv );
	void SetSokSkipIn( NET_RECEIVED nr ) { m_nrSokSkipIn = nr; }
	bool GetSokSkipIn() { return (m_nrSokSkipIn == NR_RECEIVED); }

	void OnDataReceive(CString strMsg);
	CString	OnBodyAnalysis(int nMode, CString strRev, CString strFind);
	void OnDataAnalysis(CString strRev);

protected:
	NET_RECEIVED	m_nrSokSkipIn;
	int m_nPort;

};

/////////////////////////////////////////////////////////////////////////////
extern C8570Client g_8570_client;

#endif // !defined(AFX_8570CLIENT_H__A387EA97_2B36_4F50_AEC2_CF8242FBD6D1__INCLUDED_)
