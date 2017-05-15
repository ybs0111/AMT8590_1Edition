#if !defined(AFX_DIALOG_NETWORK_H__F4353650_5AE8_49D5_A6C5_AB01DA338800__INCLUDED_)
#define AFX_DIALOG_NETWORK_H__F4353650_5AE8_49D5_A6C5_AB01DA338800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_NetWork.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_NetWork dialog

class CDialog_NetWork : public StandardDialog
{
// Construction
public:
	CDialog_NetWork(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_NetWork)
	enum { IDD = IDD_DIALOG_NETWORK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_NetWork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_NetWork)
	afx_msg void OnButtonPackMasterInfo();
	afx_msg void OnButtonProdSerialInfo();
	afx_msg void OnButtonBoatidModify();
	afx_msg void OnButtonBoatidInfo();
	afx_msg void OnBtnLabelSize();
	afx_msg void OnBtnLabelCcs();
	afx_msg void OnBtnBcrCcs();
	afx_msg void OnBtnBcrFormat();
	afx_msg void OnBtnXclose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_NETWORK_H__F4353650_5AE8_49D5_A6C5_AB01DA338800__INCLUDED_)
