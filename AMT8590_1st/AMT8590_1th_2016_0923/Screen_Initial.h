#if !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
#define AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Initial.h : header file
//
#include "Variable.h"				// 전역 변수 정의 클래스 추가 
#include "PictureEx.h"				// GIF 파일 로딩 클래스 추가 
#include "GradientStatic.h"			// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XPButton.h"				// XP 형태 칼라 버튼 생성 클래스 추가 
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 
#include "Public_Function.h"

#include "MacButtons.h"
#include "SuperProgressCtrl.h"		// ProgressCtrl 생성 클래스 추가

#include "Thread.h"					// 쓰레드 생성 클래스 추가 
// ******************************************************************************
// 쓰레드 동작 관련 클래스 추가                                                  
// ******************************************************************************
#include "Run_Handler_Check.h"
#include "Run_TowerLamp.h"
// ******************************************************************************

// ******************************************************************************
// 설정 정보 로딩 및 저장 클래스                                                 
// ******************************************************************************
#include "MyBasicData.h"

// ******************************************************************************
// 초기화 화면 관련 타이머 ID 정의                                               
// ******************************************************************************
#define TMR_INIT	710  // 초기화 작업 진행 타이머 ID 선언 
// ******************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



enum ENUM_INIT_STEP
{
	EIS_START,
	EIS_ERRMSG,
	EIS_IO,
	EIS_MOTOR,
	EIS_CHK_STATE,
	EIS_RECOVERY,
	EIS_CREATE_THREAD,
	EIS_SET_THREAD_STEP,
	EIS_CHK_ALL_FINISH,
	
	EIS_FINISH,
};

class CScreen_Initial : public CFormView
{
public:
	CScreen_Initial();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Initial)

// Form Data
public:
	//{{AFX_DATA(CScreen_Initial)
	enum { IDD = IDD_SCREEN_INITIAL };
	CPictureEx	m_gif_initializing;
	CMacButton	m_btn_init_skip;
	CMacButton	m_btn_init_retry;
	CGradientStatic	m_msg_initial;
	CLedButton	m_radio_mot_bd;
	CLedButton	m_radio_io_bd;


	////2014.1018
	CLedButton	m_radio_top_idbuffer;
	CLedButton	m_radio_top_idbuffer_1;
	CLedButton	m_radio_top_idbuffer_2;
	
	CLedButton	m_radio_btm_idbuffer;
	CLedButton	m_radio_btm_idbuffer_1;
	CLedButton	m_radio_btm_idbuffer_2;
	
	CLedButton  m_radio_idbuffer_el;
	CLedButton  m_radio_top_vision_x;
	
	CLedButton  m_radio_uld_cv;
	CLedButton  m_radio_uld_cv_1;
	CLedButton  m_radio_uld_cv_2;
	CLedButton  m_radio_uld_cv_3;
	CLedButton  m_radio_uld_cv_4;
	CLedButton  m_radio_uld_cv_5;
	
	CLedButton  m_radio_bcr_feeder;
	CLedButton  m_radio_bcr_feeder_1;
	CLedButton  m_radio_bcr_feeder_2;
	CLedButton  m_radio_bcr_feeder_3;
	CLedButton  m_radio_bcr_feeder_4;
	CLedButton  m_radio_bcr_vision_x;
	CLedButton  m_radio_bcr_reject_x;
	
	CLedButton  m_radio_bcr_attach;
	CLedButton  m_radio_bcr_attach1_x;
	CLedButton  m_radio_bcr_attach2_x;
	CLedButton  m_radio_bcr_attach1_z1;
	CLedButton  m_radio_bcr_attach1_r1;
	CLedButton  m_radio_bcr_attach1_z2;
	CLedButton  m_radio_bcr_attach1_r2;
	CLedButton  m_radio_bcr_attach2_z1;
	CLedButton  m_radio_bcr_attach2_r1;
	CLedButton  m_radio_bcr_attach2_z2;
	CLedButton  m_radio_bcr_attach2_r2;
	
	CLedButton  m_radio_d_attach;
	CLedButton  m_radio_d_attach_x;
	CLedButton  m_radio_d_attach_y;
	CLedButton  m_radio_d_attach_z1;
	CLedButton  m_radio_d_attach_z2;
	
	CLedButton  m_radio_d_feeder;
	CLedButton  m_radio_d_feeder_x;
	CLedButton  m_radio_d_feeder_1;
	CLedButton  m_radio_d_feeder_2;
	CLedButton  m_radio_d_feeder_3;
	CLedButton  m_radio_d_feeder_4;
	CLedButton  m_radio_d_feeder_5;
	CLedButton  m_radio_d_feeder_6;
	
	CLedButton  m_radio_index;
	CLedButton	m_radio_index_roller1;
	CLedButton	m_radio_index_roller2;
	CLedButton	m_radio_index_roller_pitch;
	CLedButton  m_radio_index_x;
	//}}AFX_DATA

// Attributes
public:

	// **************************************************************************
	// 환경 설정 데이터 로딩 및 저장 클래스 변수 선언                            
	// **************************************************************************
	CMyBasicData     mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	// **************************************************************************
	
	UINT m_nDataSendCnt;
	
	int mn_pos_step;				// 프로그레서 위치 정보 저장 변수
	
	// **************************************************************************
	// 초기화 관련 : Step 데이터 저장 변수 및 리턴 플래그 선언                   
	// **************************************************************************
	long ml_init_step ;
	int mn_init_flag;

	bool bRecoveryInit;
	// **************************************************************************

// Operations
public:
	int OnInitial_Init_Excution();
	void OnInitial_Initial_Ready();
	void OnInitial_Step_Clear();
	void OnInitial_Change_Status(int n_status);
	void OnInitial_Controls_Enable(int n_init);
	void OnInitial_RadioButton_Set();
	void OnInitial_Progress_Set();
	void OnInitial_Picture_Set();
	void OnInitial_Label_Set();

protected:
	int OnLevelCheck();
	bool ChkInitReady();

	void EIS_Start();
	void EIS_ErrMsg();
	void EIS_InitIO();
	void EIS_Motor();
	void EIS_Chk_State();
	void EIS_Recovery();
	void EIS_Create_Thread();
	void EIS_Set_Thread_Step();
	void EIS_Chk_All_Finish();
	void EIS_Finish();
	
private:
	double NetworkingTime[3];
	char cJamcode[10];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Initial)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSuperProgressCtrl m_ctrlProgress;
	CSuperProgressCtrl m_ctrlProgress1;

	virtual ~CScreen_Initial();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Initial)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnInitSkip();
	afx_msg void OnBtnInitRetry();
	//}}AFX_MSG

	afx_msg LRESULT OnInitBtn(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

enum ENUM_INIT_BTN
{
	RBTN_ALL_OFF,
		
	RBTN_IO,
	RBTN_MOTOR,

	RBTN_TOPID_LD,//2014.1018
	RBTN_TOPID_LD_1,
	RBTN_TOPID_LD_2,
	
	RBTN_BTMID_ULD,//2014.1018
	RBTN_BTMID_ULD_1,
	RBTN_BTMID_ULD_2,
	
	RBTN_IDBUFFER_EL,
	RBTN_TOP_VISION_X,
	
	RBTN_ULD,
	RBTN_ULD_STACKER_Z,
	RBTN_ULD_CV_PITCH,
	RBTN_ULD_Z,
	RBTN_ULD_Y,
	RBTN_BTM_VISION_X,
	
	RBTN_BCR_FEEDER,
	RBTN_BCR_FEEDER_1,
	RBTN_BCR_FEEDER_2,
	RBTN_BCR_FEEDER_3,
	RBTN_BCR_FEEDER_4,
	RBTN_BCR_VISION_X,
	RBTN_BCR_REJECT_X,//2014.1018 initial not 
	
	RBTN_BCR_ATTACH,
	RBTN_BCR_ATTACH1_X,
	RBTN_BCR_ATTACH1_Z1,
	RBTN_BCR_ATTACH1_R1,
	RBTN_BCR_ATTACH1_Z2,
	RBTN_BCR_ATTACH1_R2,
	RBTN_BCR_ATTACH2_X,
	RBTN_BCR_ATTACH2_Z1,
	RBTN_BCR_ATTACH2_R1,
	RBTN_BCR_ATTACH2_Z2,
	RBTN_BCR_ATTACH2_R2,
	
	RBTN_D_ATTACH,
	RBTN_D_ATTACH_X,
	RBTN_D_ATTACH_Y,
	RBTN_D_ATTACH_Z1,
	RBTN_D_ATTACH_Z2,
	
	RBTN_D_FEEDER,
	RBTN_D_VISION_X,
	RBTN_D_FEEDER_1,
	RBTN_D_FEEDER_2,
	RBTN_D_FEEDER_3,
	RBTN_D_FEEDER_4,
	RBTN_D_FEEDER_5,
	RBTN_D_FEEDER_6,
	
	RBTN_INDEX,
	RBTN_INDEX_X,		//2014.1018 initial not 
	RBTN_INDEX_ROLLER1,
	RBTN_INDEX_ROLLER2,
	RBTN_ULD_ROLLER_PITCH,
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
