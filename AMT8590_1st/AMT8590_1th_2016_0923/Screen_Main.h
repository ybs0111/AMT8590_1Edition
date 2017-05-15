//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_SCREEN_MAIN_H__7A483F1B_61FF_4EB9_B41D_251A637D6074__INCLUDED_)
#define AFX_SCREEN_MAIN_H__7A483F1B_61FF_4EB9_B41D_251A637D6074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Main.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"

#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "LedButton.h"      // LED 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 생성 클래스 추가 
#include "MacButtons.h"
#include "Digit.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 

#include "FloatST.h"
#include "Animate.h"
#include "ComizoaPublic.h"

#include "Picture.h"

#define STATE_DISPLAY	809

class AModule;

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Main : public CFormView
{
public:
	CScreen_Main();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Main)

// Form Data
public:
	//{{AFX_DATA(CScreen_Main)
	enum { IDD = IDD_SCREEN_MAIN };
	CMacButton	m_btn_model;
	CDigit	m_dgt_maint;
	CGradientStatic	m_msg_maint;
	CXPGroupBox	m_group_tact_info;
	CDigit	m_dgt_idle;
	CDigit	m_dgt_change;
	CGradientStatic	m_msg_change;
	CGradientStatic	m_msg_idle;
	CXPGroupBox	m_group_use_barcode;
	CMacButton	m_btn_convey_reset;
	CXPGroupBox	m_group_density_error;
	CXPGroupBox	m_group_rear;
	CXPGroupBox	m_group_front;
	CXPGroupBox	m_group_unload_conveyor;
	CDigit	m_dgt_tact;
	CButton	m_chk_dpickplace;
	CButton	m_chk_btm_ready;
	CButton	m_chk_front_ready;
	CXPGroupBox	m_group_bcr_label_info;
	CMacButton	m_btn_door_open;
	CDigit	m_dgt_alarm;
	CGradientStatic	m_msg_stop;
	CGradientStatic	m_msg_run;
	CGradientStatic	m_msg_alarm;
	CGradientStatic	m_msg_mtbi;
	CXPGroupBox	m_group_time_info;
	CDigit	m_dgt_stop;
	CDigit	m_dgt_run;
	CDigit	m_dgt_mtbi;
	CXPGroupBox	m_group_lot_info;
	CXPGroupBox	m_group_lot_yield_info;
	CXPGroupBox	m_group_b_lot_info;
	CXPGroupBox	m_group_daily_yield_info;
	CEditEx	 m_edit_bcr_id;
	//}}AFX_DATA

// Attributes
public:
	int mn_time_select;
	CAnimate         m_Animate;
	int m_bExeExcute;
	CPicture		m_p_bmp_view;


/////

protected:
	void CalcPopupMenuByClick( int iCustom );
	int m_iClick;
	DWORD m_dwTimeClicked;
	int m_iClickCnt;
	
// Operations
public:
	void OnMain_GroupBox_Set();
	void OnMain_Label_Set();
	void OnMain_Digital_Count_Set();
	void OnMain_TimeInfo_Display();
	void OnMain_Time_Display(int n_state);
	void OnMain_Tact_Display();

	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

	////2014.0821
	void UpdateScreens();
	void UpdateMotorMove();
	void Init_Animation();
	void OnIDBufferInfoInit();
	////
	void OnBTMIDBufferInfoInit();//2014,1217

	void OnMain_Init_Door();
	void OnMain_Display_Door();
	void OnMain_Init_NetUsable();
	void OnMain_Display_NetUsable();

	void OnMain_Display_Daily_Yield_info();
	void OnMain_Display_Lot_Yield_info();
	void OnMain_Display_Lot_Info();
	void OnMain_Init_B_Lot_Info();
	void OnMain_Init_Lot_Info();
	void OnMain_Init_Lot_Yield_info();
	void OnMain_Init_Daily_Yield_info();
// 	void OnMain_Display_BuffrInfo();
	void OnMain_Display_BuffrInfo(int nzDisplayBuffer);
	void OnMain_Display_BTM_BuffrInfo(int nzDisplayBuffer);//2014,1217

	void OnMain_Init_SMema();
	void OnMain_Display_Smema();
	void OnMain_Init_Btm();
	void OnMain_Display_Front_Btm();

	////2014,1125 ysj
	void OnMain_Init_BCR_Info();
	void OnMain_Display_BCR_Info();
	////

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Main)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Main();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Main)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnDoorOpen();
	afx_msg void OnBtnBufferReady();
	afx_msg void OnBtnBufferIn1();
	afx_msg void OnBtnBufferIn2();
	afx_msg void OnBtnBufferIn3();
	afx_msg void OnBtnBufferIn4();
	afx_msg void OnBtnBufferIn5();
	afx_msg void OnBtnMoveCv1();
	afx_msg void OnBtnMoveCv2();
	afx_msg void OnBtnBufferIn6();
	afx_msg void OnBtnBufferIn7();
	afx_msg void OnBtnBufferIn8();
	afx_msg void OnBtnBufferIn9();
	afx_msg void OnBtnBufferIn10();
	afx_msg void OnBtnBufferOut1();
	afx_msg void OnBtnBufferOut2();
	afx_msg void OnBtnBufferOut3();
	afx_msg void OnBtnBufferOut4();
	afx_msg void OnBtnBufferOut5();
	afx_msg void OnBtnBufferOut6();
	afx_msg void OnBtnBufferOut7();
	afx_msg void OnBtnBufferOut8();
	afx_msg void OnBtnBufferOut9();
	afx_msg void OnBtnBufferOut10();
	afx_msg void OnBtnMoveCv3();
	afx_msg void OnBtnMoveCv4();
	afx_msg void OnBtnBufferSkip1();
	afx_msg void OnBtnBufferSkip2();
	afx_msg void OnBtnBufferSkip3();
	afx_msg void OnBtnBufferSkip4();
	afx_msg void OnBtnBufferSkip5();
	afx_msg void OnBtnBufferSkip6();
	afx_msg void OnBtnBufferSkip7();
	afx_msg void OnBtnBufferSkip8();
	afx_msg void OnBtnBufferSkip9();
	afx_msg void OnBtnBufferSkip10();
	afx_msg void OnBtnBufferSkip11();
	afx_msg void OnBtnBufferSkip12();
	afx_msg void OnBtnBufferSkip13();
	afx_msg void OnBtnBufferSkip14();
	afx_msg void OnBtnBufferSkip15();
	afx_msg void OnBtnBufferSkip16();
	afx_msg void OnBtnBufferSkip17();
	afx_msg void OnBtnBufferSkip18();
	afx_msg void OnBtnBufferSkip19();
	afx_msg void OnBtnBufferSkip20();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnBtnDisplay();
	afx_msg void OnCheckFrontReady();
	afx_msg void OnChkBtmReady();
	afx_msg void OnChkDensityPickplace();
	afx_msg void OnButton20();
	afx_msg void OnBtnLabel();
	afx_msg void OnBtnBcr();
	afx_msg void OnBtnBcrCcs();
	afx_msg void OnBtnLabelCcs();
	afx_msg void OnBtnXclose();
	afx_msg void OnBtnIdBufferInput();
	afx_msg void OnButton29();
	afx_msg void OnButton30();
	afx_msg void OnButton31();
	afx_msg void OnButton32();
	afx_msg void OnMsgRun();
	afx_msg void OnBtnNetwork();
	afx_msg void OnBtnConveyReset();
	afx_msg void OnBtnXgem();
	afx_msg void OnBtnDensity1();
	afx_msg void OnBtnDensity2();
	afx_msg void OnBtnDensity3();
	afx_msg void OnBtnDensity4();
	afx_msg void OnBtnDensity5();
	afx_msg void OnBtnDensity6();
	afx_msg void OnBtnBcr11();
	afx_msg void OnBtnBcr12();
	afx_msg void OnBtnBcr21();
	afx_msg void OnBtnBcr22();
	afx_msg void OnBtnRin11();
	afx_msg void OnBtnRin12();
	afx_msg void OnBtnRin21();
	afx_msg void OnBtnRin22();
	afx_msg void OnButtonModel();
	//}}AFX_MSG
	afx_msg LRESULT OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
	afx_msg LRESULT OnMain_Draw_UI(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MAIN_H__7A483F1B_61FF_4EB9_B41D_251A637D6074__INCLUDED_)
