#if !defined(AFX_DIALOG_MANUAL_PART_1_H__DEF3E95C_BCC8_41B1_9EA7_EB6C4B7B4D2E__INCLUDED_)
#define AFX_DIALOG_MANUAL_PART_1_H__DEF3E95C_BCC8_41B1_9EA7_EB6C4B7B4D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_Part_1.h : header file
//
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"

#include "Dialog_Select.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Part_1 dialog
#define TMR_IO_CHECK				9

class CDialog_Manual_Part_1 : public CDialog
{
// Construction
public:
	CDialog_Manual_Part_1(CWnd* pParent = NULL);   // standard constructor
	BOOL Create();

// Dialog Data
	//{{AFX_DATA(CDialog_Manual_Part_1)
	enum { IDD = IDD_DIALOG_MANUAL_PART_1 };
	CButton	m_btn_go;
	CButton	m_chk_pick;
	//}}AFX_DATA

public:
	CFont*	mp_manual_font;			// 폰트 설정 변수 선언 
	int		mn_part;
	CPoint	mcp_coord;	
	CString m_strTempMSG;
	int	m_nRobot;

	int mn_stop_req;
	int n_response;					// 대화 상자 리턴 플래그
	int mn_cmd_no;
	int mn_motor_no;
	double md_motor_spd;
	double md_motor_pos;
	CString m_strPrintFormat;

	////2014.1025
	int mn_move_step[10];
	DWORD m_dwTimeWait[10];
	int mn_density_label_no;
	////
	
	////2014.1102
	int mn_robot_step[10];
	int mn_density_feeder_no;
	int mn_manual_device_type;
	DWORD	m_dwTime_Pick;
	
	int mn_labrl_robot_step_1;
	int mn_labrl_robot_step_2;
	int mn_distribution_feeder_no1;
	int mn_distribution_feeder_no2;
	int mn_retry_cnt;
	int mn_distribution_label_no;
	int mn_distribution_move_step[10];
	DWORD	m_dwTimeWait_distribution[4][3];

	int		m_nRetryPickOut[6];
	////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_Part_1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	void OnManualPart1_IO_Check();
	void OnPrintFormat(int nPort);
	void OnInitIO();

	int DLabel_Print( int nMotor );//2014.1102
	int DRobot_Move( int nfeederNo );//2014.1102
	void Distribution_Robot_Move1();//2014.1102
	void Distribution_Robot_Move2();//2014.1102
	int Distribution_Label_Print(int label_no);//2014.1102
	
	// jtkim
	int		m_nInCount;
	int		m_nOutCount;
	int		m_nIn[2][50];
	int		m_nInPos[2][50][2];
	int		m_nOut[2][50];
	int		m_nOutPos[2][50][2];

	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Manual_Part_1)
	afx_msg void OnBtnPickDensityLabel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDensityPosSave();
	afx_msg void OnButtonZdown();
	afx_msg void OnBtnPlaceDensityLabel();
	afx_msg void OnButtonGo();
	afx_msg void OnBtnBcrOutLabel();
	afx_msg void OnBtnBcrOutLabel2();
	afx_msg void OnBtnBcrOutLabel3();
	afx_msg void OnBtnBcrOutLabel4();
	afx_msg void OnBtnBcrVisionLabel();
	afx_msg void OnBtnBcrVisionLabel2();
	afx_msg void OnBtnBcrVisionLabel3();
	afx_msg void OnBtnBcrVisionLabel4();
	afx_msg void OnBtnPickBcrLabel1();
	afx_msg void OnBtnPickBcrLabel2();
	afx_msg void OnBtnPlaceBcrLabel12();
	afx_msg void OnBtnPlaceBcrLabel34();
	afx_msg void OnBtnRejBcrLabel();
	afx_msg void OnBtnRejBcrLabel2();
	afx_msg void OnBtnRejBcrLabel3();
	afx_msg void OnBtnRejBcrLabel4();
	afx_msg void OnBtnTopVision();
	afx_msg void OnBtnTopVision2();
	afx_msg void OnBtnBtmVision();
	afx_msg void OnBtnPickUnload();
	afx_msg void OnBtnBtmVision2();
	afx_msg void OnBtnBtmVision3();
	afx_msg void OnBtnBtmVision4();
	afx_msg void OnBtnUnloadStacker();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnUnloadReject();
	afx_msg void OnBtnConvUldPlace();
	afx_msg void OnBtnRejectUldPlace();
	afx_msg void OnBtnStop();
	afx_msg void OnBtn1Exit();
	afx_msg void OnBtnDensityLabelOut();
	afx_msg void OnBtnDensityLabelOut4();
	afx_msg void OnBtnDensityLabelOut2();
	afx_msg void OnBtnDensityLabelOut5();
	afx_msg void OnBtnDensityLabelOut3();
	afx_msg void OnBtnDensityLabelOut6();
	afx_msg void OnBtnDensityAttachRobot1();
	afx_msg void OnBtnDensityAttachRobot2();
	afx_msg void OnBtnDensityAttachRobot3();
	afx_msg void OnBtnDistributionAttachRobot1();
	afx_msg void OnBtnDistributionAttachRobot2();
	//}}AFX_MSG
	afx_msg LRESULT OnMotorCompletion(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MANUAL_PART_1_H__DEF3E95C_BCC8_41B1_9EA7_EB6C4B7B4D2E__INCLUDED_)
