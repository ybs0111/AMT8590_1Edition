#if !defined(AFX_DIALOG_HANDBARCODE_H__6892EF77_A0F5_4E90_836B_32A5879AD66B__INCLUDED_)
#define AFX_DIALOG_HANDBARCODE_H__6892EF77_A0F5_4E90_836B_32A5879AD66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_HandBarcode.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"
#include "EXDigitST.h"			// 디지털 카운터 클래스
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"			// XP 칼라 버튼 클래스
#include "FloatST.h"
#include "GridControlAlg.h"
#include "Digit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_HandBarcode dialog

#define TM_DENSITY_CCS	19011
#define TM_BCR_CCS		19012
class CDialog_HandBarcode : public StandardDialog
{
// Construction
public:
	CDialog_HandBarcode(CWnd* pParent = NULL);   // standard constructor
	int mn_dialog;
	CString mstr_new_barcode_info_1;
	CString mstr_new_barcode_info_2;
	CString mstr_new_barcode_info_3;
	CString mstr_new_barcode_info_4;
	CString mstr_new_barcode_info_5;
	CString mstr_new_barcode_info_6;
	CString mstr_new_barcode_info_7;
	CString mstr_new_barcode_info_8;
	CString mstr_new_barcode_info_9;
	CString mstr_new_barcode_info_10;
	CString mstr_new_barcode_info_11;
	CString mstr_new_barcode_info_12;
	CString mstr_new_barcode_info_13;
	CString mstr_new_barcode_info_14;

	int mn_ServerStep;
	DWORD m_dwServerTime;
	CString m_strRrtry;
	CString m_strCnt;
	CString m_strMode;
	int m_nRetryCnt;
	int m_nLabelpos;

// Dialog Data
	//{{AFX_DATA(CDialog_HandBarcode)
	enum { IDD = IDD_DIALOG_HANDBARCODE };
	CMacRadioButton	m_radio_device_type_3;
	CMacRadioButton	m_radio_device_type_2;
	CMacRadioButton	m_radio_device_type_1;
	CComboBox	m_cbolabel3capa;
	CComboBox	m_cbolabel2capa;
	CComboBox	m_cbolabel1capa;
	CXPGroupBox	m_group_device_type;
	CMacButton	m_btn_denapply;
	CMacButton	m_btn_bcrapply;
	CEditEx	 m_edit_new_barcode_info_1;
	CEditEx	 m_edit_new_barcode_info_2;
	CEditEx	 m_edit_new_barcode_info_3;
	CEditEx	 m_edit_new_barcode_info_4;
	CEditEx	 m_edit_new_barcode_info_5;
	CEditEx	 m_edit_new_barcode_info_6;
	CEditEx	 m_edit_new_barcode_info_7;
	CEditEx	 m_edit_new_barcode_info_8;
	CEditEx	 m_edit_new_barcode_info_9;
	CEditEx	 m_edit_new_barcode_info_10;
	CEditEx	 m_edit_new_barcode_info_11;
	CEditEx	 m_edit_new_barcode_info_12;
	CEditEx	 m_edit_new_barcode_info_13;
	CEditEx	 m_edit_new_barcode_info_14;
	CEditEx	 m_edit_new_barcode_info_15;
	CEditEx	 m_edit_new_barcode_info_16;
	CEditEx	 m_edit_new_barcode_info_17;
	CEditEx	 m_edit_new_barcode_info_18;
	CXPGroupBox	m_group_handbarcode1;
	CXPGroupBox	m_group_handbarcode2;	
	//}}AFX_DATA

public:
	void OnHandBarcode_GroupBox_Set();
	void OnHandBarcode_EditBox_Set();
	void OnHandBarcode_LogFile_Create();		
	void OnHandBarcode_Data_Display();
	int OnServerInterace( int nInterface);
	void LoadLabelData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_HandBarcode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_HandBarcode)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnApply2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboLabel1Capa();
	afx_msg void OnSelchangeComboLabel2Capa();
	afx_msg void OnSelchangeComboLabel3Capa();
	afx_msg void OnBtnApply3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_HANDBARCODE_H__6892EF77_A0F5_4E90_836B_32A5879AD66B__INCLUDED_)
