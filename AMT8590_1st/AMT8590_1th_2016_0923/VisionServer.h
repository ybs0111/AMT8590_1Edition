// VisionServer.h: interface for the CVisionServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISIONSERVER_H__F6113075_D321_4B37_8BE8_8213D63F2A2C__INCLUDED_)
#define AFX_VISIONSERVER_H__F6113075_D321_4B37_8BE8_8213D63F2A2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVisionServer  
{
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;

	void		OnDataReceive(int nAddr, CString strMsg);
	CString		OnAddItem(int nIndex, WIN32_FIND_DATA *pfd);
	CString		OnModelList(CString strPath);
	void		OnVisionModelWork(int nPort, CString strModel, CString strMsg);
	void		OnVisionModelList(int nPort, CString strList, CString strMsg);
	CString		OnBodyAnalysis(int nMode, CString strRev, CString strFind);
	void		OnDataAnalysis(int nAddr, CString strRev);
public:
	CVisionServer();
	virtual ~CVisionServer();

};

extern CVisionServer clsVsServer[10];
#endif // !defined(AFX_VISIONSERVER_H__F6113075_D321_4B37_8BE8_8213D63F2A2C__INCLUDED_)
