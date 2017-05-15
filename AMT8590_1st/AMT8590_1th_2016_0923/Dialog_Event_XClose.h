#if !defined(AFX_DIALOG_EVENT_XCLOSE_H__42FFDC80_86B5_4B74_9C92_1B97A5CAC0A6__INCLUDED_)
#define AFX_DIALOG_EVENT_XCLOSE_H__42FFDC80_86B5_4B74_9C92_1B97A5CAC0A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Event_XClose.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "MacButtons.h"

#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 

/////////////////////////////////////////////////////////////////////////////
// CDialog_Event_XClose dialog

class CDialog_Event_XClose : public StandardDialog
{
// Construction
public:
	CDialog_Event_XClose(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Event_XClose)
	enum { IDD = IDD_DIALOG_EVENT_XCLOSE };
	CNewLabel	m_label_event_msg;
	CMacButton	m_btn_cancle;
	CMacButton	m_btn_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Event_XClose)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CButton *mp_btn_ok;
	
public:
	void OnEventMsg_Button_Set();
	void OnEventMsg_Text_Set();
	void OnEventMsg_Label_Set();

protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Event_XClose)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCancle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_EVENT_XCLOSE_H__42FFDC80_86B5_4B74_9C92_1B97A5CAC0A6__INCLUDED_)
