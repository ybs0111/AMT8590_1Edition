#if !defined(AFX_SCREEN_SERVER_NETWORK_H__39720750_F2DA_446D_95E0_19E8C43A5E6D__INCLUDED_)
#define AFX_SCREEN_SERVER_NETWORK_H__39720750_F2DA_446D_95E0_19E8C43A5E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Server_Network.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_Server_Network form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Server_Network : public CFormView
{
public:
	CScreen_Server_Network();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Server_Network)

// Form Data
public:
	//{{AFX_DATA(CScreen_Server_Network)
	enum { IDD = IDD_SCREEN_SRVER_NETWORK };
	CMacButton	m_btn_keyin_menual;
	CString	m_LOT_ID;
	CString	m_GATE;
	CString	m_RETRY;
	CString	m_RETRY_CNT;
	CString	m_SERIAL;
	CString	m_PPID;
	CString	m_WWN;
	CString	m_C_SERIAL;
	CString	m_PSID;
	CString	m_BOAT_ID;
	CString	m_MODIF_MODE;
	CString	m_DTBT_LABEL_CODE;
	CString	m_DTBT_RIBBON_CODE;
	CString	m_MODE;
	CString	m_WEIGHT;
	CString	m_DTBT_LABEL_SERIAL;
	CString	m_S_BOX_CNT;
	CString	m_S_BOX_LABEL_SERIAL;
	CString	m_L_BOX_ID;
	CString	m_MO_CLOSE_TIME;
	CString	m_S_BOX_ID;
	CString	m_TOOLID;
	CString	m_DEN_LABEL_MAT_CODE;
	CString	m_DTBT_MAT_CODE;
	CString	m_RIBBON_MAT_CODE;
	CString	m_SSD_CNT;
	CString	m_PROD_SRLNO;
	CString	m_PIECE_PROD_ID;
	CString	m_WWN_SRLNO;
	CString	m_CUST_SRLNO;
	CString	m_PHYS_SECRTY_ID;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Server_Network)
	public:
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Server_Network();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Server_Network)
	afx_msg void OnLOT_CLOSE();
	afx_msg void OnBtnKeyinMenual();
	afx_msg void OnDestroy();
	afx_msg void OnPEBAG_CHECK();
	afx_msg void OnBRAND_PEBAG_CHECK();
	afx_msg void OnBOAT_ID_MODIF();
	afx_msg void OnBRAND_DTBT_LABEL_ISSUE();
	afx_msg void OnBRAND_WEIGHT_SBOX_ISSUE();
	afx_msg void OnLBOX_ISSUE();
	afx_msg void OnAUTO_SHIP();
	afx_msg void OnPROD_SERIAL_INFO();
	afx_msg void OnBOAT_ID_INFO();
	afx_msg void OnSBOX_INFO();
	afx_msg void OnPACKING_MASTER_INFO();
	afx_msg void OnPK_XCLOSE();
	afx_msg void OnTOOL_CHANGE();
	afx_msg void OnTOOL_CHECK();
	afx_msg void OnBRAND_DENSITY_LABEL_CCS();
	afx_msg void OnBRAND_DTBT_LABEL_CCS();
	afx_msg void OnPEBAG_COMPLETE();
	afx_msg void OnSRLADD();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_SERVER_NETWORK_H__39720750_F2DA_446D_95E0_19E8C43A5E6D__INCLUDED_)
