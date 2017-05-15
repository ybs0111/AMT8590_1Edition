// XgemCommand.h: interface for the CXgemCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XGEMCOMMAND_H__3504B0D1_4F11_4CFF_BECD_3E9189E14FB3__INCLUDED_)
#define AFX_XGEMCOMMAND_H__3504B0D1_4F11_4CFF_BECD_3E9189E14FB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XGem.h"

class CXgemCommand  
{
public:
	CXgemCommand();
	virtual ~CXgemCommand();

// Attributes
private:
	
// Attributes
public:
	CXGem *m_hXgem;
	CWnd  *m_hXgemInterfaceDlg;

// Operations
private:
	
// Operations
public:
	void XgemMachineStatus(int nMode);
	void XgemAlarmStatus(CString strAlarmCode, int nStatus);
	void XgemEqCtrlStatus(int nMode);
	void XgemInitialize(CWnd *hWnd, CString strPath);

	void XgemPPID(CString strPPID, CString strPath);
	void XgemRecipeSelect();
	void XgemRecipeChange();

	void XgemLotStart();
	void XgemLotEnd();

	void XgemTrayIn();
	void XgemTrayOut();

};
extern CXgemCommand	clsXgem;

#endif // !defined(AFX_XGEMCOMMAND_H__3504B0D1_4F11_4CFF_BECD_3E9189E14FB3__INCLUDED_)
