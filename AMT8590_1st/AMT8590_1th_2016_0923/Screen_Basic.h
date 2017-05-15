#if !defined(AFX_SCREEN_BASIC_H__75EFE953_60C7_432F_9759_19FF79697AC2__INCLUDED_)
#define AFX_SCREEN_BASIC_H__75EFE953_60C7_432F_9759_19FF79697AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 

#include "Digit.h"

// ******************************************************************************
// 설정 정보 로딩 및 저장 클래스
// ******************************************************************************
#include "MyBasicData.h"
// ******************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Basic : public CFormView
{
public:
	CScreen_Basic();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Basic)

// Form Data
public:
	//{{AFX_DATA(CScreen_Basic)
	enum { IDD = IDD_SCREEN_BASIC };
	CMacRadioButton	m_radio_vis_density_skip_notuse;
	CMacRadioButton	m_radio_vis_density_skip_use;
	CMacRadioButton	m_radio_rework_ccs_notuse;
	CMacRadioButton	m_radio_rework_ccs_use;
	CXPGroupBox	m_group_vision_density_skip;
	CXPGroupBox	m_group_rework_ccs;
	CXPGroupBox	m_group_reject_offset;
	CMacRadioButton	m_radio_top_vision_notskip;
	CMacRadioButton	m_radio_top_vision_skip;
	CXPGroupBox	m_group_top_skip;
	CMacRadioButton	m_radio_notuse_auto_vision_mdl;
	CMacRadioButton	m_radio_use_auto_vision_mdl;
	CXPGroupBox	m_group_auto_vision_model;
	CXPGroupBox	m_group_bcr_error_time;
	CComboBox	m_cbolabel3capa;
	CComboBox	m_cbolabel2capa;
	CComboBox	m_cbolabel1capa;
	CXPGroupBox	m_group_vision_total_error_cnt;
	CMacRadioButton	m_radio_use_convbox;
	CMacRadioButton	m_radio_use_rejectbox;
	CXPGroupBox	m_group_out_tbox;
	CMacRadioButton	m_radio_ecsvr_notuse;
	CMacRadioButton	m_radio_ecsvr_use;
	CMacRadioButton	m_radio_outconv_notuse;
	CMacRadioButton	m_radio_outconv_use;
	CXPGroupBox	m_group_out_conv;
	CMacRadioButton	m_radio_left_bcr_notuse;
	CMacRadioButton	m_radio_right_bcr_notuse;
	CMacRadioButton	m_radio_right_bcr_use;
	CMacRadioButton	m_radio_left_bcr_use;
	CMacRadioButton	m_radio_btm_vision_notuse;
	CMacRadioButton	m_radio_btm_vision_use;
	CMacRadioButton	m_radio_top_vision_notuse;
	CMacRadioButton	m_radio_top_vision_use;
	CMacRadioButton	m_radio_density_vision_notuse;
	CMacRadioButton	m_radio_density_vision_use;
	CXPGroupBox	m_group_bcr_vision;
	CXPGroupBox	m_group_density_vision;
	CXPGroupBox	m_group_top_vision;
	CXPGroupBox	m_group_btm_vision;
	CXPGroupBox	m_group_server_mode;
	CXPGroupBox	m_group_right_bcr_mode;
	CXPGroupBox	m_group_left_bcr_mode;
	CGradientStatic	m_msg_electrostatic_5;
	CGradientStatic	m_msg_electrostatic_4;
	CGradientStatic	m_msg_electrostatic_3;
	CGradientStatic	m_msg_electrostatic_2;
	CGradientStatic	m_msg_electrostatic_1;
	CGradientStatic	m_label_electrostatic_5;
	CGradientStatic	m_label_electrostatic_4;
	CGradientStatic	m_label_electrostatic_3;
	CGradientStatic	m_label_electrostatic_2;
	CGradientStatic	m_label_electrostatic_1;
	CButton	m_radio_use_rcmd;
	CButton	m_radio_notuse_rcmd;
	CButton	m_radio_notuse_xgem;
	CButton	m_radio_use_xgem;
	CXPGroupBox	m_group_xgem_mode;
	CXPGroupBox	m_group_rcmd_mode;
	CXPGroupBox	m_group_electrostatic_test;
	CXPGroupBox	m_group_out_type;
	CButton	m_radio_pt_1v1m_4use_2;
	CXPGroupBox m_group_picker_offset;
	CXPGroupBox	m_group_socket_max_count;
	CXPGroupBox	m_group_hotfix_pos;
	CButton	m_radio_recovery_notuse;
	CButton	m_radio_recovery_use;
	CXPGroupBox	m_group_recovery_mode;
	CButton	m_radio_jig_notuse;
	CButton	m_radio_jig_use;
	CXPGroupBox	m_group_jig_mode;
	CXPGroupBox	m_group_basic_manual_speed;
	CXPGroupBox	m_group_basic_run_speed;
	CXPGroupBox	m_group_basic_speed;
	CXPGroupBox	m_group_robot_init;
	CButton	m_radio_pogo_notuse;
	CButton	m_radio_pogo_mode;
	CXPGroupBox	m_group_pogo_mode;
	CXPGroupBox	m_group_empty_tray;
	CMacRadioButton	m_radio_bypass_notuse;
	CMacRadioButton	m_radio_bypass_rear;
	CMacRadioButton	m_radio_bypass_front;
	CMacRadioButton	m_radio_bypass_all;
	CXPGroupBox	m_group_time_out;
	CButton	m_radio_online_mode;
	CButton	m_radio_offline_mode;
	CXPGroupBox	m_group_online_mode;
	CButton	m_radio_pt_2v1m_2use;
	CButton	m_radio_pt_1v1m_2use;
	CButton	m_radio_pt_1v1m_4use;
	CXPGroupBox	m_group_pick_type;
	CButton	m_radio_degree_180;
	CButton	m_radio_degree_270;
	CButton	m_radio_degree_90;
	CButton	m_radio_degree_0;
	CXPGroupBox	m_group_degree;
	CButton	m_radio_cok_5;
	CButton	m_radio_cok_4;
	CButton	m_radio_cok_3;
	CButton	m_radio_cok_2;
	CButton	m_radio_cok_1;
	CButton	m_radio_cok_0;
	CXPGroupBox	m_group_cok_type;
	CXPGroupBox	m_group_recipe_select;
	CGradientStatic	m_msg_tray_y;
	CGradientStatic	m_msg_tray_x;
	CGradientStatic	m_msg_tray_y2;
	CGradientStatic	m_msg_tray_x2;
	CXPGroupBox	m_group_tray_size_setting;
	CGradientStatic	m_msg_port_ecserver;
	CXPGroupBox	m_group_alarm_delay_time;
	CXPGroupBox	m_group_network_wait_time;
	CXPGroupBox	m_group_network_retry_count;
	CMacRadioButton	m_radio_device_mode_use;
	CMacRadioButton	m_radio_device_mode_notuse;
	CDigit	m_dgt_electrostatic_check_value;
	CListBoxST	m_list_device_type;
	CXPGroupBox	m_group_electrostatic_check_value;
	CXPGroupBox	m_group_electrostatic_check_gap;
	CXPGroupBox	m_group_device_mode;
	CMacButton	m_btn_electostatic_test;
	CMacButton	m_btn_dvc_delete;
	CMacButton	m_btn_dvc_create;
	CMacButton	m_btn_basic_cancel;
	CMacButton	m_btn_basic_apply;
	CDigit	m_dgt_1D_bar_width;
	CButton	m_radio_illumination_Off;
	CButton	m_radio_illumination_Left;
	CButton	m_radio_illumination_Right;
	CButton	m_radio_illumination_Both;
	CButton	m_radio_imagerotate_0;
	CButton	m_radio_imagerotate_180;
	CButton	m_radio_select_1D;
	CButton	m_radio_select_2D;
	CButton	m_radio_select_both;
	CButton	m_radio_background_1Dwhite;
	CButton	m_radio_background_1Dblack;
	CButton	m_radio_background_1Dboth;
	CButton	m_radio_background_2Dwhite;
	CButton	m_radio_background_2Dblack;
	CButton	m_radio_background_2Dboth;
	CGradientStatic	m_label_distance;
	CGradientStatic	m_label_shutter_time;
	CGradientStatic	m_label_brightness;
	CGradientStatic	m_label_contrast;
	CGradientStatic	m_label_1d_bar_width;
	CGradientStatic	m_msg_port_1;
	CGradientStatic	m_msg_rate_1;
	CGradientStatic	m_msg_data_1;
	CGradientStatic	m_msg_stop_1;
	CGradientStatic	m_msg_parity_1;
	CComboBox	m_cb_port_1;
	CComboBox	m_cb_rate_1;
	CComboBox	m_cb_data_1;
	CComboBox	m_cb_stop_1;
	CComboBox	m_cb_parity_1;
	CXPGroupBox m_group_Barcode;
	CGradientStatic m_label_offset;
	CGradientStatic m_label_picker_offset;
	CGradientStatic m_label_picker_offset2;
	CGradientStatic m_label_picker_offset3;
	CListBoxST  m_list_vision_ml1;
	CListBoxST  m_list_vision_ml2;
	CListBoxST  m_list_vision_ml3;
	CListBoxST  m_list_vision_ml4;
	CXPGroupBox m_group_label_vision_paper;
	CXPGroupBox m_group_label_print_error_cnt;
	CXPGroupBox m_group_alarm_file_read;
	CXPGroupBox m_group_density_feeder;
	CXPGroupBox m_group_density_feeder2;
	CXPGroupBox m_group_density_feeder3;
	CXPGroupBox m_group_density_feeder4;
	CXPGroupBox m_group_density_feeder5;
	CXPGroupBox m_group_density_feeder6;
	CXPGroupBox m_group_bcr_feeder;
	CXPGroupBox m_group_bcr_feeder1;
	CXPGroupBox m_group_bcr_feeder2;
	CXPGroupBox m_group_bcr_feeder3;
	CXPGroupBox m_group_device_type;
	CMacRadioButton	m_radio_device_type_1;
	CMacRadioButton	m_radio_device_type_2;
	CMacRadioButton	m_radio_device_type_3;
	CMacRadioButton	m_radio_density_feeder_use;
	CMacRadioButton	m_radio_density_feeder_notuse;
	CMacRadioButton	m_radio_density_feeder_use1;
	CMacRadioButton	m_radio_density_feeder_notuse1;
	CMacRadioButton	m_radio_density_feeder_use2;
	CMacRadioButton	m_radio_density_feeder_notuse2;
	CMacRadioButton	m_radio_density_feeder_use3;
	CMacRadioButton	m_radio_density_feeder_notuse3;
	CMacRadioButton	m_radio_density_feeder_use4;
	CMacRadioButton	m_radio_density_feeder_notuse4;
	CMacRadioButton	m_radio_density_feeder_use5;
	CMacRadioButton	m_radio_density_feeder_notuse5;
	CMacRadioButton	m_radio_bcr_feeder_use;
	CMacRadioButton	m_radio_bcr_feeder_notuse;
	CMacRadioButton	m_radio_bcr_feeder_use1;
	CMacRadioButton	m_radio_bcr_feeder_notuse1;
	CMacRadioButton	m_radio_bcr_feeder_use2;
	CMacRadioButton	m_radio_bcr_feeder_notuse2;
	CMacRadioButton	m_radio_bcr_feeder_use3;
	CMacRadioButton	m_radio_bcr_feeder_notuse3;
	CMacRadioButton	m_radio_vision_use;//2014.1104
	CMacRadioButton	m_radio_vision_notuse;//2014.1104
	CXPGroupBox	m_group_label_select;
	CMacRadioButton m_radio_label_select_1;
	CMacRadioButton m_radio_label_select_2;
	//}}AFX_DATA

// Attributes
public:
// 	CFont* mp_basic_font;			// 폰트 정보 설정 변수
	////2014.0823
	TSpread			*m_grid_mode;
	GridControlAlg	mp_grid;
	CString mstr_equip_id[2];
	CString mstr_equip_no[2];
	int mn_enable;
	////
	CString str_vision_model_name1[2];   // 선택된 디바이스 종류 저장 변수
	int     n_vision_model_index1[2];
	CString str_vision_model_name2[2];   // 선택된 디바이스 종류 저장 변수
	int     n_vision_model_index2[2];
	CString str_vision_model_name3[2];   // 선택된 디바이스 종류 저장 변수
	int     n_vision_model_index3[2];
	CString str_vision_model_name4[2];   // 선택된 디바이스 종류 저장 변수
	int     n_vision_model_index4[2];
	int		mn_leftbcr[2];
	int		mn_rightbcr[2];

	int mn_bcr_ml_name1;
	int mn_bcr_ml_name2;
	int mn_bcr_ml_name3;
	int mn_bcr_ml_name4;

	int mn_bakDevice_Type;

	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	
	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언
	// **************************************************************************
	CEXDigitST	m_dgt_bcr_limit_time;
	CEXDigitST  m_dgt_electrostatic_check_gap;			// 정전기 확인을 몇분에 한번씩 할것인가.
	CEXDigitST  m_dgt_network_wait_time;		// Network 대기 시간
	CEXDigitST  m_dgt_network_retry_cnt;		// Network Retry 횟수
	CEXDigitST	m_dgt_alarm_delay_time;
	CEXDigitST	m_dgt_time_out;
	CEXDigitST	m_dgt_empty_tray_out;
	CEXDigitST	m_dgt_robot_init_time;
	CEXDigitST	m_dgt_hotfix_pos;

	CEXDigitST	m_dgt_tray_x;
	CEXDigitST	m_dgt_tray_y;

	CEXDigitST	m_dgt_tray_x2;
	CEXDigitST	m_dgt_tray_y2;

	CEXDigitST m_dgt_basic_run_speed;
	CEXDigitST m_dgt_basic_manual_speed;
	CEXDigitST m_dgt_socket_max_count;

	CEditEx	 m_edit_device_type;
	
	CEXDigitST  m_dgt_Label_Print_Error_Cnt;
	CEXDigitST	m_dgt_label_vision_paper;
	CEXDigitST  m_dgt_vision_total_error_cnt;
	CEXDigitST  m_dgt_top_vision_skip_cnt;
	CEXDigitST  m_dgt_reject_offfset;

	// **************************************************************************
	// 리스트 박스 컨트롤 정보 저장 클래스 변수 선언
	// **************************************************************************
	CImageList mp_device_image_list;	// 이미지 리스트 컨트롤 선언
	
	CString mstr_temp_device;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_load_file;				// 로딩할 파일에 대한 [폴더]+[파일명] 저장 변수
	int mn_device_name;					// 선택된 디바이스의 리스트 위치 저장 변수
	
	CString mstr_new_device;			// 새로 생성할 디바이스의 이름.
	// **************************************************************************
	
	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언                                         
	// **************************************************************************
	CEditEx  m_edit_ecserver_port;
	// **************************************************************************
	
	// **************************************************************************
	// 데이터 저장 클래스 변수 선언
	// **************************************************************************
	BYTE nIPFiled0[2], nIPFiled1[2], nIPFiled2[2], nIPFiled3[2];		// 0: ECSERVER, 1: NEXT MACHINE
	CString m_strTempMSG;

	////////20131115
	CEXDigitST	m_dgt_distance;
	CEXDigitST	m_dgt_contrast;
	CEXDigitST	m_dgt_shutter_time;
	CEXDigitST	m_dgt_code_lenth;		
	CEXDigitST	m_dgt_brightness;
	CEXDigitST	m_dgt_offset;
	CEXDigitST	m_dgt_picker_offset;
	CEXDigitST	m_dgt_picker_offset2;
	CEXDigitST	m_dgt_picker_offset3;
	CEXDigitST	m_dgt_picker_offset4;


	int			m_n_port[10][2];
	int			m_n_rate[10][2];
	int			m_n_data[10][2];
	int			m_n_stop[10][2];
	int			m_n_parity[10][2];
	
	////////
	

// Operations
public:
	int OnBasic_Input_Data_Check();
	void OnBasic_Data_Display();
	void OnBasic_DeviceData_Apply();
	void OnBasic_TitleBar_Info_Send();
	void OnBasic_Data_Apply();
	int OnBasic_Input_Device_Check();
	BOOL AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	int OnBasic_Init_List(LPCTSTR pszPath);
	void OnBasic_Create_ImageList();
	void OnBasic_Device_Display(CString str_dvc);
	void OnBasic_Device_Focus_Set();
	void OnBasic_Create_Device();
	int OnBasic_New_Device_Check();
	void OnBasic_DeviceMode_Change();

	void OnBasic_CokType_Change();
	void OnBasic_Data_Change();
	void OnBasic_Label_Set();
	void OnBasic_Data_Set();
	void OnBasic_Digital_Count_Set();
	void OnBasic_EditBox_Set();
	void OnBasic_GroupBox_Set();
	void OnBasic_Degree_Change();
	void OnBasic_RunMode_Change();
	void OnBasic_XgemMode_Change();
	void OnBasic_RcmdMode_Change();
	void OnBasic_LeftBcr_Change();
	void OnBasic_RightBcr_Change();

	void LoadLabelData();
	void OnBasicDensity_Capa_Display();
	void OnBasicDensity_CapaSet();

	////2014.0823
	void OnBasic_Init_Time();
	void OnBasic_Grid_View();
	void On_Controls_Enable(int nState);
	////
	// Grid
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridData(UINT nID, int row, int col, CString data);
	void GridFont(UINT nID, int row, int col, int size);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	CString GetGridData( UINT nID, int row, int col );

	
	int OnBasic_Init_Model_List(LPCTSTR pszPath, int nVisionNo);
	void OnBasic_Model_Focus_Set(int nVisionNo);
	BOOL Model_AddItem(int nVisionNo, int nIndex, WIN32_FIND_DATA *pfd);

	void OnBasic_DeviceType_Change();
	void OnBasic_DensityFeeder_Change1();
	void OnBasic_DensityFeeder_Change2();
	void OnBasic_DensityFeeder_Change3();
	void OnBasic_DensityFeeder_Change4();
	void OnBasic_DensityFeeder_Change5();
	void OnBasic_DensityFeeder_Change6();
	void OnBasic_BCRFeeder_Change1();
	void OnBasic_BCRFeeder_Change2();
	void OnBasic_BCRFeeder_Change3();
	void OnBasic_BCRFeeder_Change4();

	void OnBasic_Vision_Change();
	void OnBasic_Density_Vision_Change();
	void OnBasic_Top_Vision_Change();
	void OnBasic_Btm_Vision_Change();
	void OnBasic_OutConv_Change();
	void OnBasic_ECServer_Change();
	void OnBasic_OutTBox_Change();
	void OnBasic_auto_vision_model();
	void OnBasic_Top_Vision_SkipChange();
	void OnBasic_Vision_Density_Skip_Change();
	

	void OnBasic_Electrostatic_Data_View(int nChannel);
	void OnBasic_Label_Select();//2014,1125 ysj

	void OnBasic_Rework_CCS_Change();
	//////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Basic)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Basic();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Basic)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRadioDeviceModeUse();
	afx_msg void OnRadioDeviceModeNotuse();
	afx_msg void OnBtnDvcCreate();
	afx_msg void OnSelchangeListDeviceType();
	afx_msg void OnBtnDvcDelete();
	afx_msg void OnBtnBasicApply();
	afx_msg void OnBtnBasicCancle();
	afx_msg void OnDgtNetworkWaitTime();
	afx_msg void OnDgtNetworkRetryCount();
	afx_msg void OnDgtAlarmDelayTime();
	afx_msg void OnRadioOnlineMode();
	afx_msg void OnRadioOfflineMode();
	afx_msg void OnDgtTimeOut();
	afx_msg void OnDgtRunSpeed();
	afx_msg void OnDgtManualSpeed();
	afx_msg void OnBtnElectrostaticTest();
	afx_msg void OnRadioUseXgem();
	afx_msg void OnRadioNotuseXgem();
	afx_msg void OnRadioUseRcmd();
	afx_msg void OnRadioNotuseRcmd();
	afx_msg void OnBtnAlarmFileRead();
	afx_msg void OnDgtElectrostaticCheckGap();
	afx_msg void OnDgtElectrostaticCheckValue();
	afx_msg void OnDgtLabelPrintErrorCnt();
	afx_msg void OnDgtLabelVisionPaper();
	afx_msg void OnBtnVisionModel1();
	afx_msg void OnBtnVisionModel2();
	afx_msg void OnBtnVisionModel3();
	afx_msg void OnBtnVisionModel4();
	afx_msg void OnSelchangeListVisionModel1();
	afx_msg void OnSelchangeListVisionModel2();
	afx_msg void OnSelchangeListVisionModel3();
	afx_msg void OnSelchangeListVisionModel4();
	afx_msg void OnRadioDeviceType1();
	afx_msg void OnRadioDeviceType2();
	afx_msg void OnRadioDeviceType3();
	afx_msg void OnRadioDensityFeederModeUse1();
	afx_msg void OnRadioDensityFeederModeNotuse1();
	afx_msg void OnRadioDensityFeederModeUse2();
	afx_msg void OnRadioDensityFeederModeNotuse2();
	afx_msg void OnRadioDensityFeederModeUse3();
	afx_msg void OnRadioDensityFeederModeNotuse3();
	afx_msg void OnRadioDensityFeederModeUse4();
	afx_msg void OnRadioDensityFeederModeNotuse4();
	afx_msg void OnRadioDensityFeederModeUse5();
	afx_msg void OnRadioDensityFeederModeNotuse5();
	afx_msg void OnRadioDensityFeederModeUse6();
	afx_msg void OnRadioDensityFeederModeNotuse6();
	afx_msg void OnRadioBcrFeederModeUse1();
	afx_msg void OnRadioBcrFeederModeNotuse1();
	afx_msg void OnRadioBcrFeederModeUse2();
	afx_msg void OnRadioBcrFeederModeNotuse2();
	afx_msg void OnRadioBcrFeederModeUse3();
	afx_msg void OnRadioBcrFeederModeNotuse3();
	afx_msg void OnRadioBcrFeederModeUse4();
	afx_msg void OnRadioBcrFeederModeNotuse4();
	afx_msg void OnRadioVisionUse();
	afx_msg void OnRadioVisionNotuse();
	afx_msg void OnRadioServerUse();
	afx_msg void OnRadioDensityLabelSelect1();
	afx_msg void OnRadioDensityLabelSelect2();
	afx_msg void OnRadioDensityVisionUse();
	afx_msg void OnRadioDensityVisionNotuse();
	afx_msg void OnRadioTopVisionUse();
	afx_msg void OnRadioTopVisionNotuse();
	afx_msg void OnRadioBtmVisionUse();
	afx_msg void OnRadioBtmVisionNotuse();
	afx_msg void OnRadioServerNotuse();
	afx_msg void OnRadioLeftBcrUse();
	afx_msg void OnRadioLeftBcrNotuse();
	afx_msg void OnRadioRightBcrUse();
	afx_msg void OnRadioRightBcrNotuse();
	afx_msg void OnRadioUseOutconv();
	afx_msg void OnRadioNotuseOutconv();
	afx_msg void OnRadioUseReject();
	afx_msg void OnRadioUseConv();
	afx_msg void OnDgtVisionTotalErrorCnt();
	afx_msg void OnSelchangeComboLabel1Capa();
	afx_msg void OnSelchangeComboLabel2Capa();
	afx_msg void OnSelchangeComboLabel3Capa();
	afx_msg void OnDgtBcrLimitTime();
	afx_msg void OnRadioUseAutoVisionModel();
	afx_msg void OnRadioNotuseAutoVisionModel();
	afx_msg void OnBtnVisDensity();
	afx_msg void OnBtnVisTop();
	afx_msg void OnRadioTopVisionSkip();
	afx_msg void OnDgtTopVisionSkipCnt();
	afx_msg void OnRadioTopVisionNotSkip();
	afx_msg void OnDgtRejectOffset();
	afx_msg void OnRadioReworkCcsUse();
	afx_msg void OnRadioReworkCcsNotuse();
	afx_msg void OnRadioVisDensitySkipUse();
	afx_msg void OnRadioVisDensitySkipNotuse();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam);  // BASIC 화면 정보 갱신 요청 
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_BASIC_H__75EFE953_60C7_432F_9759_19FF79697AC2__INCLUDED_)
