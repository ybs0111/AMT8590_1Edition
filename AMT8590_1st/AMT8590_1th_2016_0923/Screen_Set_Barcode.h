#if !defined(AFX_SCREEN_SET_BARCODE_H__CC1151F5_F500_476C_96E4_278D75D03F8B__INCLUDED_)
#define AFX_SCREEN_SET_BARCODE_H__CC1151F5_F500_476C_96E4_278D75D03F8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Set_Barcode.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"	// 공용 함수 정의 클래스 추가
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editEx.h"				// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "ListBoxST.h"			// 칼라 아이콘 리스트 박스 클래스 
#include "xpbutton.h"			// XP 칼라 버튼 클래스
#include "EXDigitST.h"			// 디지털 카운터 클래스
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "LedButton.h"			// LED 버튼 클래스
			// LED 버튼 클래스

// ******************************************************************************
// 설정 정보 로딩 및 저장 클래스                                                 
// ******************************************************************************
#include "MyBasicData.h"
#include "Digit.h"

#include "Ctlbd_Variable.h"	// Added by ClassView
#include "Srcbase\ALocalization.h"
/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Barcode form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define		TM_MODEL			199
#define		TM_DATA				299

class CScreen_Set_Barcode : public CFormView
{
// Construction
public:
	CScreen_Set_Barcode();   // standard constructor
	DECLARE_DYNCREATE(CScreen_Set_Barcode)
// Form Data
public:
	//{{AFX_DATA(CScreen_Set_Barcode)
	enum { IDD = IDD_SCREEN_SET_BARCODE };
	CEdit	m_ctrlName;
	CButton m_radio_prt_mode1;
	CButton m_radio_prt_mode2;
	CButton m_radio_prt_mode3;
	CButton m_radio_prt_mode4;
	CButton m_radio_prt_complete_use1;
	CButton m_radio_prt_complete_use2;
	CButton m_radio_prt_complete_use3;
	CButton m_radio_prt_complete_use4;
	CButton m_radio_prt_complete_notuse1;
	CButton m_radio_prt_complete_notuse2;
	CButton m_radio_prt_complete_notuse3;
	CButton m_radio_prt_complete_notuse4;
	CMacButton m_btn_prt_apply1;
	CMacButton m_btn_prt_apply2;
	CMacButton m_btn_prt_apply3;
	CMacButton m_btn_prt_apply4;
	CMacButton m_btn_prt_apply5;
	CXPGroupBox m_group_prt_no1;
	CXPGroupBox m_group_prt_no2;
	CXPGroupBox m_group_prt_no3;
	CXPGroupBox m_group_prt_no4;
	CXPGroupBox m_group_prt_mode1;
	CXPGroupBox m_group_prt_mode2;
	CXPGroupBox m_group_prt_mode3;
	CXPGroupBox m_group_prt_mode4;
	CXPGroupBox m_group_prt_complete1;
	CXPGroupBox m_group_prt_complete2;
	CXPGroupBox m_group_prt_complete3;
	CXPGroupBox m_group_prt_complete4;
	CXPGroupBox m_group_prt_pos1;
	CXPGroupBox m_group_prt_pos2;
	CXPGroupBox m_group_prt_pos3;
	CXPGroupBox m_group_prt_pos4;
	CGradientStatic m_msg_labelset_sd1;
	CGradientStatic m_msg_labelset_sd2;
	CGradientStatic m_msg_labelset_sd3;
	CGradientStatic m_msg_labelset_sd4;
	CGradientStatic m_msg_labelset_lt1;
	CGradientStatic m_msg_labelset_lt2;
	CGradientStatic m_msg_labelset_lt3;
	CGradientStatic m_msg_labelset_lt4;
	CGradientStatic m_msg_labelset_lhx1;
	CGradientStatic m_msg_labelset_lhx2;
	CGradientStatic m_msg_labelset_lhx3;
	CGradientStatic m_msg_labelset_lhx4;
	CGradientStatic m_msg_labelset_lhy1;
	CGradientStatic m_msg_labelset_lhy2;
	CGradientStatic m_msg_labelset_lhy3;
	CGradientStatic m_msg_labelset_lhy4;
	//}}AFX_DATA
// Attributes
public:
	CMyBasicData     mcls_barcode;	// 기본 셋팅 정보 로딩 및 저장 클래스
	CFont			* m_p_font;

	int mn_Prt_Mode1[2];
	int mn_Prt_Mode2[2];
	int mn_Prt_Mode3[2];
	int mn_Prt_Mode4[2];
	int mn_Prt_complete_use1[2];
	int mn_Prt_complete_use2[2];
	int mn_Prt_complete_use3[2];
	int mn_Prt_complete_use4[2];

	int mn_labelset_sd1[2];
	int mn_labelset_sd2[2];
	int mn_labelset_sd3[2];
	int mn_labelset_sd4[2];

	int mn_labelset_lt1[2];
	int mn_labelset_lt2[2];
	int mn_labelset_lt3[2];
	int mn_labelset_lt4[2];

	int mn_labelset_lhx1[2];
	int mn_labelset_lhx2[2];
	int mn_labelset_lhx3[2];
	int mn_labelset_lhx4[2];

	int mn_labelset_lhy1[2];
	int mn_labelset_lhy2[2];
	int mn_labelset_lhy3[2];
	int mn_labelset_lhy4[2];

	CEXDigitST	m_dgt_labelset_sd1;
	CEXDigitST	m_dgt_labelset_sd2;
	CEXDigitST	m_dgt_labelset_sd3;
	CEXDigitST	m_dgt_labelset_sd4;
	
	CEXDigitST	m_dgt_labelset_lt1;
	CEXDigitST	m_dgt_labelset_lt2;
	CEXDigitST	m_dgt_labelset_lt3;
	CEXDigitST	m_dgt_labelset_lt4;
	
	CEXDigitST	m_dgt_labelset_lhx1;
	CEXDigitST	m_dgt_labelset_lhx2;
	CEXDigitST	m_dgt_labelset_lhx3;
	CEXDigitST	m_dgt_labelset_lhx4;
	
	CEXDigitST	m_dgt_labelset_lhy1;
	CEXDigitST	m_dgt_labelset_lhy2;
	CEXDigitST	m_dgt_labelset_lhy3;
	CEXDigitST	m_dgt_labelset_lhy4;

	int		m_nClientPos;
	int		m_nInterfaceStep;
	CString m_strModel;
	DWORD	m_dwTimeWait[3];

public:
	void OnBarcode_GroupBox_Set();
	void OnBarcode_Label_Set();
	void OnBarcode_Data_Display();
	void OnBarcode_Data_Set();
	void OnBarcode_Data_Backup();
	void OnBarcode_Dgt_Set();
	void OnBarcode_Button_Set();
	void OnBarcode_Data_Recovery();

	int		OnVisionModelChange();
	int		OnVisionData();

private:

// Operations
public:
	void OnBarcode_LogFile_Create();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Set_Barcode)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Set_Barcode();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CScreen_Set_Barcode)
	afx_msg void OnDestroy();
	afx_msg void OnBtnPrtStatus1();
	afx_msg void OnBtnPrtStatus2();
	afx_msg void OnBtnPrtStatus3();
	afx_msg void OnBtnPrtStatus4();
	afx_msg void OnRadioPrtMode1();
	afx_msg void OnRadioPrtCompleteUse1();
	afx_msg void OnRadioPrtCompleteNotuse1();
	afx_msg void OnRadioPrtMode3();
	afx_msg void OnRadioPrtCompleteUse3();
	afx_msg void OnRadioPrtCompleteNotuse3();
	afx_msg void OnRadioPrtMode2();
	afx_msg void OnRadioPrtCompleteUse2();
	afx_msg void OnRadioPrtCompleteNotuse2();
	afx_msg void OnRadioPrtMode4();
	afx_msg void OnRadioPrtCompleteUse4();
	afx_msg void OnRadioPrtCompleteNotuse4();
	afx_msg void OnDgtLabelsetSd1();
	afx_msg void OnDgtLabelsetLt1();
	afx_msg void OnDgtLabelsetLhx1();
	afx_msg void OnDgtLabelsetLhy1();
	afx_msg void OnDgtLabelsetSd3();
	afx_msg void OnDgtLabelsetLt3();
	afx_msg void OnDgtLabelsetLhx3();
	afx_msg void OnDgtLabelsetLhy3();
	afx_msg void OnDgtLabelsetSd2();
	afx_msg void OnDgtLabelsetLt2();
	afx_msg void OnDgtLabelsetLhx2();
	afx_msg void OnDgtLabelsetLhy2();
	afx_msg void OnDgtLabelsetSd4();
	afx_msg void OnDgtLabelsetLt4();
	afx_msg void OnDgtLabelsetLhx4();
	afx_msg void OnDgtLabelsetLhy4();
	afx_msg void OnBtnPrtApply1();
	afx_msg void OnBtnPrtApply2();
	afx_msg void OnBtnPrtApply3();
	afx_msg void OnBtnPrtApply4();
	afx_msg void OnBtnPrtApply5();
	afx_msg void OnBtnModelDensity();
	afx_msg void OnBtnModelBarcode();
	afx_msg void OnBtnModelDensityPos();
	afx_msg void OnBtnModelBarcodePos();
	afx_msg void OnBtnDataDensity();
	afx_msg void OnBtnDataBarcode();
	afx_msg void OnBtnDataDensityPos();
	afx_msg void OnBtnDataBarcodePos();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnPrtSd1();
	afx_msg void OnBtnPrtSd2();
	afx_msg void OnBtnPrtSd3();
	afx_msg void OnBtnPrtSd4();
	afx_msg void OnBtnPrtLt1();
	afx_msg void OnBtnPrtLt2();
	afx_msg void OnBtnPrtLt3();
	afx_msg void OnBtnPrtLt4();
	afx_msg void OnBtnPrtLhx1();
	afx_msg void OnBtnPrtLhx2();
	afx_msg void OnBtnPrtLhx3();
	afx_msg void OnBtnPrtLhx4();
	afx_msg void OnBtnPrtLhy1();
	afx_msg void OnBtnPrtLhy2();
	afx_msg void OnBtnPrtLhy3();
	afx_msg void OnBtnPrtLhy4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_SET_BARCODE_H__CC1151F5_F500_476C_96E4_278D75D03F8B__INCLUDED_)
