#if !defined(AFX_SCREEN_MODEL_H__6ED235DB_7D72_4321_A8F8_39DFE4586BF4__INCLUDED_)
#define AFX_SCREEN_MODEL_H__6ED235DB_7D72_4321_A8F8_39DFE4586BF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Model.h : header file
//

#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 

/////////////////////////////////////////////////////////////////////////////
// CScreen_Model form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Model : public CFormView
{
protected:
	CScreen_Model();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Model)

// Form Data
public:
	//{{AFX_DATA(CScreen_Model)
	enum { IDD = IDD_SCREEN_MODEL };
	CComboBox	m_cbo_model;
	CXPGroupBox	m_group_newmodel;
	CXPGroupBox	m_group_regmodel;
	//}}AFX_DATA

// Attributes
public:
	CEditEx	 m_edit_new_partid;

// Operations
public:
	void OnModel_Display();
	void OnModel_NewEdit();
	void OnModel_Combo_Display();
	void OnModel_Select();
	void OnModel_EditBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Model)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Model();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Model)
	afx_msg void OnBtnDvcDelete();
	afx_msg void OnButtonAdd();
	afx_msg void OnSelchangeComboModel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MODEL_H__6ED235DB_7D72_4321_A8F8_39DFE4586BF4__INCLUDED_)
