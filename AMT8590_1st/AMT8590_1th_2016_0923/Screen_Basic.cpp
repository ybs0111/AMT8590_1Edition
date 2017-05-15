// Screen_Basic.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Basic.h"

#include "AMTLotManager.h"
#include "Srcbase\ALocalization.h"
#include "SrcPart\DataManager.h"

#include "io.h"  // 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요

// ******************************************************************************
// 대화 상자 클래스 추가
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************
#include "InterfaceGms.h"
#include "AMTRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic

IMPLEMENT_DYNCREATE(CScreen_Basic, CFormView)

CScreen_Basic::CScreen_Basic()
	: CFormView(CScreen_Basic::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Basic)
	//}}AFX_DATA_INIT
}

CScreen_Basic::~CScreen_Basic()
{
}

void CScreen_Basic::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Basic)
	DDX_Control(pDX, IDC_RADIO_VIS_DENSITY_SKIP_NOTUSE, m_radio_vis_density_skip_notuse);
	DDX_Control(pDX, IDC_RADIO_VIS_DENSITY_SKIP_USE, m_radio_vis_density_skip_use);
	DDX_Control(pDX, IDC_RADIO_REWORK_CCS_NOTUSE, m_radio_rework_ccs_notuse);
	DDX_Control(pDX, IDC_RADIO_REWORK_CCS_USE, m_radio_rework_ccs_use);
	DDX_Control(pDX, IDC_GROUP_VIS_DENSITY_SKIP, m_group_vision_density_skip);
	DDX_Control(pDX, IDC_GROUP_REWORK_CCS, m_group_rework_ccs);
	DDX_Control(pDX, IDC_GROUP_TOP_REJECT_OFFSET, m_group_reject_offset);
	DDX_Control(pDX, IDC_RADIO_TOP_VISION_NOT_SKIP, m_radio_top_vision_notskip);
	DDX_Control(pDX, IDC_RADIO_TOP_VISION_SKIP, m_radio_top_vision_skip);
	DDX_Control(pDX, IDC_GROUP_TOP_VISION_SKIP, m_group_top_skip);
	DDX_Control(pDX, IDC_RADIO_NOTUSE_AUTO_VISION_MODEL, m_radio_notuse_auto_vision_mdl);
	DDX_Control(pDX, IDC_RADIO_USE_AUTO_VISION_MODEL, m_radio_use_auto_vision_mdl);
	DDX_Control(pDX, IDC_GROUP_AUTO_VISION_MODEL, m_group_auto_vision_model);
	DDX_Control(pDX, IDC_GROUP_BCR_ERROR_TIME, m_group_bcr_error_time);
	DDX_Control(pDX, IDC_COMBO_LABEL3_CAPA, m_cbolabel3capa);
	DDX_Control(pDX, IDC_COMBO_LABEL2_CAPA, m_cbolabel2capa);
	DDX_Control(pDX, IDC_COMBO_LABEL1_CAPA, m_cbolabel1capa);
	DDX_Control(pDX, IDC_GROUP_VISION_TOTAL_ERR_CNT, m_group_vision_total_error_cnt);
	DDX_Control(pDX, IDC_RADIO_USE_CONV, m_radio_use_convbox);
	DDX_Control(pDX, IDC_RADIO_USE_REJECT, m_radio_use_rejectbox);
	DDX_Control(pDX, IDC_GROUP_OUT_TBOX, m_group_out_tbox);
	DDX_Control(pDX, IDC_RADIO_SERVER_NOTUSE, m_radio_ecsvr_notuse);
	DDX_Control(pDX, IDC_RADIO_SERVER_USE, m_radio_ecsvr_use);
	DDX_Control(pDX, IDC_RADIO_NOTUSE_OUTCONV, m_radio_outconv_notuse);
	DDX_Control(pDX, IDC_RADIO_USE_OUTCONV, m_radio_outconv_use);
	DDX_Control(pDX, IDC_GROUP_OUT_CONV, m_group_out_conv);
	DDX_Control(pDX, IDC_RADIO_LEFT_BCR_NOTUSE, m_radio_left_bcr_notuse);
	DDX_Control(pDX, IDC_RADIO_RIGHT_BCR_NOTUSE, m_radio_right_bcr_notuse);
	DDX_Control(pDX, IDC_RADIO_RIGHT_BCR_USE, m_radio_right_bcr_use);
	DDX_Control(pDX, IDC_RADIO_LEFT_BCR_USE, m_radio_left_bcr_use);
	DDX_Control(pDX, IDC_RADIO_BTM_VISION_NOTUSE, m_radio_btm_vision_notuse);
	DDX_Control(pDX, IDC_RADIO_BTM_VISION_USE, m_radio_btm_vision_use);
	DDX_Control(pDX, IDC_RADIO_TOP_VISION_NOTUSE, m_radio_top_vision_notuse);
	DDX_Control(pDX, IDC_RADIO_TOP_VISION_USE, m_radio_top_vision_use);
	DDX_Control(pDX, IDC_RADIO_DENSITY_VISION_NOTUSE, m_radio_density_vision_notuse);
	DDX_Control(pDX, IDC_RADIO_DENSITY_VISION_USE, m_radio_density_vision_use);
	DDX_Control(pDX, IDC_GROUP_VISION_MODE, m_group_bcr_vision);
	DDX_Control(pDX, IDC_GROUP_DENSITY_VISION, m_group_density_vision);
	DDX_Control(pDX, IDC_GROUP_TOP_VISION, m_group_top_vision);
	DDX_Control(pDX, IDC_GROUP_BTM_VISION, m_group_btm_vision);
	DDX_Control(pDX, IDC_GROUP_SERVER_COMM, m_group_server_mode);
	DDX_Control(pDX, IDC_GROUP_RIGHT_BCR_MODE, m_group_right_bcr_mode);
	DDX_Control(pDX, IDC_GROUP_LEFT_BCR_MODE, m_group_left_bcr_mode);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_5, m_msg_electrostatic_5);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_4, m_msg_electrostatic_4);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_3, m_msg_electrostatic_3);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_2, m_msg_electrostatic_2);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_1, m_msg_electrostatic_1);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_5, m_label_electrostatic_5);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_4, m_label_electrostatic_4);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_3, m_label_electrostatic_3);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_2, m_label_electrostatic_2);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_1, m_label_electrostatic_1);
	DDX_Control(pDX, IDC_RADIO_USE_RCMD, m_radio_use_rcmd);
	DDX_Control(pDX, IDC_RADIO_NOTUSE_RCMD, m_radio_notuse_rcmd);
	DDX_Control(pDX, IDC_RADIO_NOTUSE_XGEM, m_radio_notuse_xgem);
	DDX_Control(pDX, IDC_RADIO_USE_XGEM, m_radio_use_xgem);
	DDX_Control(pDX, IDC_GROUP_SECS_GEM, m_group_xgem_mode);
	DDX_Control(pDX, IDC_GROUP_SECS_RCMD, m_group_rcmd_mode);
	DDX_Control(pDX, IDC_GROUP_ELECTROSTATIC_TEST, m_group_electrostatic_test);
	DDX_Control(pDX, IDC_GROUP_MANUAL_SPEED, m_group_basic_manual_speed);
	DDX_Control(pDX, IDC_GROUP_RUN_SPEED, m_group_basic_run_speed);
	DDX_Control(pDX, IDC_GROUP_SPEED, m_group_basic_speed);
	DDX_Control(pDX, IDC_GROUP_TIME_OUT, m_group_time_out);
	DDX_Control(pDX, IDC_RADIO_ONLINE_MODE, m_radio_online_mode);
	DDX_Control(pDX, IDC_RADIO_OFFLINE_MODE, m_radio_offline_mode);
	DDX_Control(pDX, IDC_GROUP_ONLINE_MODE, m_group_online_mode);
	DDX_Control(pDX, IDC_GROUP_RECIPE_SELECT, m_group_recipe_select);
	DDX_Control(pDX, IDC_GROUP_ALARM_DELAY_TIME, m_group_alarm_delay_time);
	DDX_Control(pDX, IDC_GROUP_NETWORK_WAIT_TIME, m_group_network_wait_time);
	DDX_Control(pDX, IDC_GROUP_NETWORK_RETRY_COUNT, m_group_network_retry_count);
	DDX_Control(pDX, IDC_RADIO_DEVICE_MODE_USE, m_radio_device_mode_use);
	DDX_Control(pDX, IDC_RADIO_DEVICE_MODE_NOTUSE, m_radio_device_mode_notuse);
	DDX_Control(pDX, IDC_DGT_ELECTROSTATIC_CHECK_VALUE, m_dgt_electrostatic_check_value);
	DDX_Control(pDX, IDC_LIST_DEVICE_TYPE, m_list_device_type);
	DDX_Control(pDX, IDC_GROUP_ELECTROSTATIC_CHECK_VALUE, m_group_electrostatic_check_value);
	DDX_Control(pDX, IDC_GROUP_ELECTROSTATIC_CHECK_GAP, m_group_electrostatic_check_gap);
	DDX_Control(pDX, IDC_GROUP_DEVICE_MODE, m_group_device_mode);
	DDX_Control(pDX, IDC_BTN_ELECTROSTATIC_TEST, m_btn_electostatic_test);
	DDX_Control(pDX, IDC_BTN_DVC_DELETE, m_btn_dvc_delete);
	DDX_Control(pDX, IDC_BTN_DVC_CREATE, m_btn_dvc_create);
	DDX_Control(pDX, IDC_BTN_BASIC_CANCLE, m_btn_basic_cancel);
	DDX_Control(pDX, IDC_BTN_BASIC_APPLY, m_btn_basic_apply);
	DDX_Control(pDX, IDC_LIST_VISION_MODEL1, m_list_vision_ml1);
	DDX_Control(pDX, IDC_LIST_VISION_MODEL2, m_list_vision_ml2);
	DDX_Control(pDX, IDC_LIST_VISION_MODEL3, m_list_vision_ml3);
	DDX_Control(pDX, IDC_LIST_VISION_MODEL4, m_list_vision_ml4);
	DDX_Control(pDX, IDC_GROUP_LABEL_VISION_PAPER, m_group_label_vision_paper);
	DDX_Control(pDX, IDC_GROUP_LABEL_PRINT_ERROR_CNT, m_group_label_print_error_cnt);
	DDX_Control(pDX, IDC_GROUP_ALARM_FILE_READ, m_group_alarm_file_read);
	DDX_Control(pDX, IDC_GROUP_DENSITY_FEEDER, m_group_density_feeder);
	DDX_Control(pDX, IDC_GROUP_DENSITY_FEEDER2, m_group_density_feeder2);
	DDX_Control(pDX, IDC_GROUP_DENSITY_FEEDER3, m_group_density_feeder3);
	DDX_Control(pDX, IDC_GROUP_DENSITY_FEEDER4, m_group_density_feeder4);
	DDX_Control(pDX, IDC_GROUP_DENSITY_FEEDER5, m_group_density_feeder5);
	DDX_Control(pDX, IDC_GROUP_DENSITY_FEEDER6, m_group_density_feeder6);
	DDX_Control(pDX, IDC_GROUP_BCR_FEEDER, m_group_bcr_feeder);
	DDX_Control(pDX, IDC_GROUP_BCR_FEEDER2, m_group_bcr_feeder1);
	DDX_Control(pDX, IDC_GROUP_BCR_FEEDER3, m_group_bcr_feeder2);
	DDX_Control(pDX, IDC_GROUP_BCR_FEEDER4, m_group_bcr_feeder3);
	DDX_Control(pDX, IDC_GROUP_DEVICE_TYPE, m_group_device_type);
	DDX_Control(pDX, IDC_RADIO_DEVICE_TYPE_1, m_radio_device_type_1);
	DDX_Control(pDX, IDC_RADIO_DEVICE_TYPE_2, m_radio_device_type_2);
	DDX_Control(pDX, IDC_RADIO_DEVICE_TYPE_3, m_radio_device_type_3);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_USE_1, m_radio_density_feeder_use);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_1, m_radio_density_feeder_notuse);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_USE_2, m_radio_density_feeder_use1);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_2, m_radio_density_feeder_notuse1);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_USE_3, m_radio_density_feeder_use2);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_3, m_radio_density_feeder_notuse2);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_USE_4, m_radio_density_feeder_use3);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_4, m_radio_density_feeder_notuse3);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_USE_5, m_radio_density_feeder_use4);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_5, m_radio_density_feeder_notuse4);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_USE_6, m_radio_density_feeder_use5);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_6, m_radio_density_feeder_notuse5);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_USE_1, m_radio_bcr_feeder_use);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_1, m_radio_bcr_feeder_notuse);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_USE_2, m_radio_bcr_feeder_use1);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_2, m_radio_bcr_feeder_notuse1);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_USE_3, m_radio_bcr_feeder_use2);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_3, m_radio_bcr_feeder_notuse2);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_USE_4, m_radio_bcr_feeder_use3);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_4, m_radio_bcr_feeder_notuse3);
	DDX_Control(pDX, IDC_RADIO_VISION_USE, m_radio_vision_use);//2014.1104
	DDX_Control(pDX, IDC_RADIO_VISION_NOTUSE, m_radio_vision_notuse);
	DDX_Control(pDX, IDC_GROUP_DENSITY_LABEL_SELECT, m_group_label_select);
	DDX_Control(pDX, IDC_RADIO_DENSITY_LABEL_SELECT_1, m_radio_label_select_1);
	DDX_Control(pDX, IDC_RADIO_DENSITY_LABEL_SELECT_2, m_radio_label_select_2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Basic, CFormView)
	//{{AFX_MSG_MAP(CScreen_Basic)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_DEVICE_MODE_USE, OnRadioDeviceModeUse)
	ON_BN_CLICKED(IDC_RADIO_DEVICE_MODE_NOTUSE, OnRadioDeviceModeNotuse)
	ON_BN_CLICKED(IDC_BTN_DVC_CREATE, OnBtnDvcCreate)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_TYPE, OnSelchangeListDeviceType)
	ON_BN_CLICKED(IDC_BTN_DVC_DELETE, OnBtnDvcDelete)
	ON_BN_CLICKED(IDC_BTN_BASIC_APPLY, OnBtnBasicApply)
	ON_BN_CLICKED(IDC_BTN_BASIC_CANCLE, OnBtnBasicCancle)
	ON_BN_CLICKED(IDC_DGT_NETWORK_WAIT_TIME, OnDgtNetworkWaitTime)
	ON_BN_CLICKED(IDC_DGT_NETWORK_RETRY_COUNT, OnDgtNetworkRetryCount)
	ON_BN_CLICKED(IDC_DGT_ALARM_DELAY_TIME, OnDgtAlarmDelayTime)
	ON_BN_CLICKED(IDC_RADIO_ONLINE_MODE, OnRadioOnlineMode)
	ON_BN_CLICKED(IDC_RADIO_OFFLINE_MODE, OnRadioOfflineMode)
	ON_BN_CLICKED(IDC_DGT_TIME_OUT, OnDgtTimeOut)
	ON_BN_CLICKED(IDC_DGT_RUN_SPEED, OnDgtRunSpeed)
	ON_BN_CLICKED(IDC_DGT_MANUAL_SPEED, OnDgtManualSpeed)
	ON_BN_CLICKED(IDC_BTN_ELECTROSTATIC_TEST, OnBtnElectrostaticTest)
	ON_BN_CLICKED(IDC_RADIO_USE_XGEM, OnRadioUseXgem)
	ON_BN_CLICKED(IDC_RADIO_NOTUSE_XGEM, OnRadioNotuseXgem)
	ON_BN_CLICKED(IDC_RADIO_USE_RCMD, OnRadioUseRcmd)
	ON_BN_CLICKED(IDC_RADIO_NOTUSE_RCMD, OnRadioNotuseRcmd)
	ON_BN_CLICKED(IDC_BTN_ALARM_FILE_READ, OnBtnAlarmFileRead)
	ON_BN_CLICKED(IDC_DGT_ELECTROSTATIC_CHECK_GAP, OnDgtElectrostaticCheckGap)
	ON_BN_CLICKED(IDC_DGT_ELECTROSTATIC_CHECK_VALUE, OnDgtElectrostaticCheckValue)
	ON_BN_CLICKED(IDC_DGT_LABEL_PRINT_ERROR_CNT, OnDgtLabelPrintErrorCnt)
	ON_BN_CLICKED(IDC_DGT_LABEL_VISION_PAPER, OnDgtLabelVisionPaper)
	ON_BN_CLICKED(IDC_BTN_VISION_MODEL1, OnBtnVisionModel1)
	ON_BN_CLICKED(IDC_BTN_VISION_MODEL2, OnBtnVisionModel2)
	ON_BN_CLICKED(IDC_BTN_VISION_MODEL3, OnBtnVisionModel3)
	ON_BN_CLICKED(IDC_BTN_VISION_MODEL4, OnBtnVisionModel4)
	ON_LBN_SELCHANGE(IDC_LIST_VISION_MODEL1, OnSelchangeListVisionModel1)
	ON_LBN_SELCHANGE(IDC_LIST_VISION_MODEL2, OnSelchangeListVisionModel2)
	ON_LBN_SELCHANGE(IDC_LIST_VISION_MODEL3, OnSelchangeListVisionModel3)
	ON_LBN_SELCHANGE(IDC_LIST_VISION_MODEL4, OnSelchangeListVisionModel4)
	ON_BN_CLICKED(IDC_RADIO_DEVICE_TYPE_1, OnRadioDeviceType1)
	ON_BN_CLICKED(IDC_RADIO_DEVICE_TYPE_2, OnRadioDeviceType2)
	ON_BN_CLICKED(IDC_RADIO_DEVICE_TYPE_3, OnRadioDeviceType3)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_USE_1, OnRadioDensityFeederModeUse1)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_1, OnRadioDensityFeederModeNotuse1)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_USE_2, OnRadioDensityFeederModeUse2)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_2, OnRadioDensityFeederModeNotuse2)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_USE_3, OnRadioDensityFeederModeUse3)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_3, OnRadioDensityFeederModeNotuse3)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_USE_4, OnRadioDensityFeederModeUse4)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_4, OnRadioDensityFeederModeNotuse4)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_USE_5, OnRadioDensityFeederModeUse5)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_5, OnRadioDensityFeederModeNotuse5)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_USE_6, OnRadioDensityFeederModeUse6)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_FEEDER_MODE_NOTUSE_6, OnRadioDensityFeederModeNotuse6)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_USE_1, OnRadioBcrFeederModeUse1)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_1, OnRadioBcrFeederModeNotuse1)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_USE_2, OnRadioBcrFeederModeUse2)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_2, OnRadioBcrFeederModeNotuse2)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_USE_3, OnRadioBcrFeederModeUse3)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_3, OnRadioBcrFeederModeNotuse3)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_USE_4, OnRadioBcrFeederModeUse4)
	ON_BN_CLICKED(IDC_RADIO_BCR_FEEDER_MODE_NOTUSE_4, OnRadioBcrFeederModeNotuse4)
	ON_BN_CLICKED(IDC_RADIO_VISION_USE, OnRadioVisionUse)
	ON_BN_CLICKED(IDC_RADIO_VISION_NOTUSE, OnRadioVisionNotuse)
	ON_BN_CLICKED(IDC_RADIO_SERVER_USE, OnRadioServerUse)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_LABEL_SELECT_1, OnRadioDensityLabelSelect1)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_LABEL_SELECT_2, OnRadioDensityLabelSelect2)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_VISION_USE, OnRadioDensityVisionUse)
	ON_BN_CLICKED(IDC_RADIO_DENSITY_VISION_NOTUSE, OnRadioDensityVisionNotuse)
	ON_BN_CLICKED(IDC_RADIO_TOP_VISION_USE, OnRadioTopVisionUse)
	ON_BN_CLICKED(IDC_RADIO_TOP_VISION_NOTUSE, OnRadioTopVisionNotuse)
	ON_BN_CLICKED(IDC_RADIO_BTM_VISION_USE, OnRadioBtmVisionUse)
	ON_BN_CLICKED(IDC_RADIO_BTM_VISION_NOTUSE, OnRadioBtmVisionNotuse)
	ON_BN_CLICKED(IDC_RADIO_SERVER_NOTUSE, OnRadioServerNotuse)
	ON_BN_CLICKED(IDC_RADIO_LEFT_BCR_USE, OnRadioLeftBcrUse)
	ON_BN_CLICKED(IDC_RADIO_LEFT_BCR_NOTUSE, OnRadioLeftBcrNotuse)
	ON_BN_CLICKED(IDC_RADIO_RIGHT_BCR_USE, OnRadioRightBcrUse)
	ON_BN_CLICKED(IDC_RADIO_RIGHT_BCR_NOTUSE, OnRadioRightBcrNotuse)
	ON_BN_CLICKED(IDC_RADIO_USE_OUTCONV, OnRadioUseOutconv)
	ON_BN_CLICKED(IDC_RADIO_NOTUSE_OUTCONV, OnRadioNotuseOutconv)
	ON_BN_CLICKED(IDC_RADIO_USE_REJECT, OnRadioUseReject)
	ON_BN_CLICKED(IDC_RADIO_USE_CONV, OnRadioUseConv)
	ON_BN_CLICKED(IDC_DGT_VISION_TOTAL_ERROR_CNT, OnDgtVisionTotalErrorCnt)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL1_CAPA, OnSelchangeComboLabel1Capa)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL2_CAPA, OnSelchangeComboLabel2Capa)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL3_CAPA, OnSelchangeComboLabel3Capa)
	ON_BN_CLICKED(IDC_DGT_BCR_LIMIT_TIME, OnDgtBcrLimitTime)
	ON_BN_CLICKED(IDC_RADIO_USE_AUTO_VISION_MODEL, OnRadioUseAutoVisionModel)
	ON_BN_CLICKED(IDC_RADIO_NOTUSE_AUTO_VISION_MODEL, OnRadioNotuseAutoVisionModel)
	ON_BN_CLICKED(IDC_BTN_VIS_DENSITY, OnBtnVisDensity)
	ON_BN_CLICKED(IDC_BTN_VIS_TOP, OnBtnVisTop)
	ON_BN_CLICKED(IDC_RADIO_TOP_VISION_SKIP, OnRadioTopVisionSkip)
	ON_BN_CLICKED(IDC_DGT_TOP_VISION_SKIP_CNT, OnDgtTopVisionSkipCnt)
	ON_BN_CLICKED(IDC_RADIO_TOP_VISION_NOT_SKIP, OnRadioTopVisionNotSkip)
	ON_BN_CLICKED(IDC_DGT_REJECT_OFFSET, OnDgtRejectOffset)
	ON_BN_CLICKED(IDC_RADIO_REWORK_CCS_USE, OnRadioReworkCcsUse)
	ON_BN_CLICKED(IDC_RADIO_REWORK_CCS_NOTUSE, OnRadioReworkCcsNotuse)
	ON_BN_CLICKED(IDC_RADIO_VIS_DENSITY_SKIP_USE, OnRadioVisDensitySkipUse)
	ON_BN_CLICKED(IDC_RADIO_VIS_DENSITY_SKIP_NOTUSE, OnRadioVisDensitySkipNotuse)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE(WM_BASIC_APPLY, OnBasic_Info_Apply)  // BASIC DATA를 APPLY 시키기 위한 사용자 정의 메시지 선언
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic diagnostics

#ifdef _DEBUG
void CScreen_Basic::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Basic::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic message handlers

void CScreen_Basic::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
//	int i;
	
	LoadLabelData();
// 	((CEdit*)GetDlgItem(IDC_COMBO_LABEL1_CAPA))->SetWindowText(st_basic.n_str_capa1label);
// 	((CEdit*)GetDlgItem(IDC_COMBO_LABEL2_CAPA))->SetWindowText(st_basic.n_str_capa2label);
// 	((CEdit*)GetDlgItem(IDC_COMBO_LABEL3_CAPA))->SetWindowText(st_basic.n_str_capa3label);

 	OnBasic_Data_Set();				// 전역 변수의 Data를 받아온다.
 	OnBasic_Data_Change();	
	OnBasic_GroupBox_Set();
	OnBasic_EditBox_Set();
 	OnBasic_Digital_Count_Set();
	OnBasic_Label_Set();

// 	LoadLabelData();
 	OnBasic_Data_Display();
	
	// **************************************************************************
    // 칼라 리스트 박스 컨트롤 생성한다
    // **************************************************************************
	OnBasic_Create_ImageList();		// 이미지 리스트 생성 함수	
	// Associate image list to list box
	m_list_device_type.SetImageList(&mp_device_image_list);	
	OnBasic_Init_List(st_path.mstr_path_dvc);  // 파일 폴더 초기화 함수
	OnBasic_Device_Focus_Set();  // 선택된 디바이스명 포커스 설정 함수

// 	for(i=0; i<4; i++)
// 	{
// 		OnBasic_Init_Model_List(st_path.str_Model_List_folder, i);
// 		OnBasic_Model_Focus_Set(i);
// 	}

	////2014.0823
	OnBasic_Init_Time();
	mn_enable = TRUE;
	////
	mn_bakDevice_Type = st_basic.mn_device_type;

	SetTimer(TMR_GMS_CHK, 1000, NULL); // jtkim

	st_handler.cwnd_basic = this;  // BASIC 화면에 대한 핸들 정보 설정

}

void CScreen_Basic::OnDestroy() 
{
	int n_response = 0;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret = 0;
		
	KillTimer(TMR_GMS_CHK); // jtkim

	mp_device_image_list.DeleteImageList();
	st_handler.cwnd_basic = NULL;  // BASIC 화면 핸들 초기화

	CFormView::OnDestroy();	
}

void CScreen_Basic::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
// 	CString strTemp;
// 
// 	switch (nIDEvent)
// 	{
// 		case TMR_GMS_CHK:
// 			m_msg_electrostatic_1.SetWindowText(clsGms.m_strChann[0]);
// 			m_msg_electrostatic_2.SetWindowText(clsGms.m_strChann[1]);
// 			m_msg_electrostatic_3.SetWindowText(clsGms.m_strChann[2]);
// 			m_msg_electrostatic_4.SetWindowText(clsGms.m_strChann[3]);
// 			m_msg_electrostatic_5.SetWindowText(clsGms.m_strChann[4]);
// 			break;
// 	}

	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Basic::OnBasic_GroupBox_Set()
{
	CSxLogFont basic_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_device_mode.SetFont(basic_font);
	m_group_device_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_device_mode.SetFontBold(TRUE);

	m_group_device_type.SetFont(basic_font);
	m_group_device_type.SetCatptionTextColor(RGB(145,25,0));
	m_group_device_type.SetFontBold(TRUE);

	m_group_online_mode.SetFont(basic_font);
	m_group_online_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_online_mode.SetFontBold(TRUE);

	m_group_recipe_select.SetFont(basic_font);
	m_group_recipe_select.SetCatptionTextColor(RGB(145,25,0));
	m_group_recipe_select.SetFontBold(TRUE);

	m_group_alarm_delay_time.SetFont(basic_font);
	m_group_alarm_delay_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_alarm_delay_time.SetFontBold(TRUE);

	m_group_network_retry_count.SetFont(basic_font);
	m_group_network_retry_count.SetCatptionTextColor(RGB(145,25,0));
	m_group_network_retry_count.SetFontBold(TRUE);

	m_group_network_wait_time.SetFont(basic_font);
	m_group_network_wait_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_network_wait_time.SetFontBold(TRUE);

	m_group_basic_speed.SetFont(basic_font);
	m_group_basic_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_speed.SetFontBold(TRUE);

	m_group_time_out.SetFont(basic_font);
	m_group_time_out.SetCatptionTextColor(RGB(145,25,0));
	m_group_time_out.SetFontBold(TRUE);
	
	m_group_rcmd_mode.SetFont(basic_font);
	m_group_rcmd_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_rcmd_mode.SetFontBold(TRUE);

	m_group_xgem_mode.SetFont(basic_font);
	m_group_xgem_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_xgem_mode.SetFontBold(TRUE);

	m_group_basic_run_speed.SetFont(basic_font);
	m_group_basic_run_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_run_speed.SetFontBold(TRUE);
	
	m_group_basic_manual_speed.SetFont(basic_font);
	m_group_basic_manual_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_manual_speed.SetFontBold(TRUE);

	m_group_electrostatic_test.SetFont(basic_font);
	m_group_electrostatic_test.SetCatptionTextColor(RGB(145,25,0));
	m_group_electrostatic_test.SetFontBold(TRUE);

	m_group_electrostatic_check_gap.SetFont(basic_font);
	m_group_electrostatic_check_gap.SetCatptionTextColor(RGB(145,25,0));
	m_group_electrostatic_check_gap.SetFontBold(TRUE);
	
	m_group_electrostatic_check_value.SetFont(basic_font);
	m_group_electrostatic_check_value.SetCatptionTextColor(RGB(145,25,0));
	m_group_electrostatic_check_value.SetFontBold(TRUE);

	m_group_label_print_error_cnt.SetFont(basic_font);
	m_group_label_print_error_cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_label_print_error_cnt.SetFontBold(TRUE);

	m_group_vision_total_error_cnt.SetFont(basic_font);
	m_group_vision_total_error_cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_total_error_cnt.SetFontBold(TRUE);
	
	m_group_label_vision_paper.SetFont(basic_font);
	m_group_label_vision_paper.SetCatptionTextColor(RGB(145,25,0));
	m_group_label_vision_paper.SetFontBold(TRUE);
	if(g_local.GetLocalType() == LOCAL_ENG)
	{
		m_group_label_vision_paper.SetWindowText("Printer<->Vision Paper(EA)");
	}
	
	m_group_alarm_file_read.SetFont(basic_font); // 20140220
	m_group_alarm_file_read.SetCatptionTextColor(RGB(145,25,0));
	m_group_alarm_file_read.SetFontBold(TRUE);

	m_group_device_type.SetFont(basic_font); // 20140220
	m_group_device_type.SetCatptionTextColor(RGB(145,25,0));
	m_group_device_type.SetFontBold(TRUE);

	m_group_density_feeder.SetFont(basic_font); // 20140220
	m_group_density_feeder.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_feeder.SetFontBold(TRUE);

	m_group_density_feeder2.SetFont(basic_font); // 20140220
	m_group_density_feeder2.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_feeder2.SetFontBold(TRUE);

	m_group_density_feeder3.SetFont(basic_font); // 20140220
	m_group_density_feeder3.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_feeder3.SetFontBold(TRUE);

	m_group_density_feeder4.SetFont(basic_font); // 20140220
	m_group_density_feeder4.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_feeder4.SetFontBold(TRUE);
	
	m_group_density_feeder5.SetFont(basic_font); // 20140220
	m_group_density_feeder5.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_feeder5.SetFontBold(TRUE);
	
	m_group_density_feeder6.SetFont(basic_font); // 20140220
	m_group_density_feeder6.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_feeder6.SetFontBold(TRUE);

	m_group_bcr_feeder.SetFont(basic_font); // 20140220
	m_group_bcr_feeder.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr_feeder.SetFontBold(TRUE);

	m_group_bcr_feeder1.SetFont(basic_font); // 20140220
	m_group_bcr_feeder1.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr_feeder1.SetFontBold(TRUE);

	m_group_bcr_feeder2.SetFont(basic_font); // 20140220
	m_group_bcr_feeder2.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr_feeder2.SetFontBold(TRUE);

	m_group_bcr_feeder3.SetFont(basic_font); // 20140220
	m_group_bcr_feeder3.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr_feeder3.SetFontBold(TRUE);

	m_group_server_mode.SetFont(basic_font);
	m_group_server_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_server_mode.SetFontBold(TRUE);

	////2014,1125 ysj
	m_group_label_select.SetFont(basic_font);
	m_group_label_select.SetCatptionTextColor(RGB(145,25,0));
	m_group_label_select.SetFontBold(TRUE);
	////

	m_group_btm_vision.SetFont(basic_font);
	m_group_btm_vision.SetCatptionTextColor(RGB(145,25,0));
	m_group_btm_vision.SetFontBold(TRUE);
	
	m_group_top_vision.SetFont(basic_font);
	m_group_top_vision.SetCatptionTextColor(RGB(145,25,0));
	m_group_top_vision.SetFontBold(TRUE);
	
	m_group_bcr_vision.SetFont(basic_font);
	m_group_bcr_vision.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr_vision.SetFontBold(TRUE);

	m_group_density_vision.SetFont(basic_font);
	m_group_density_vision.SetCatptionTextColor(RGB(145,25,0));
	m_group_density_vision.SetFontBold(TRUE);
	
	m_group_left_bcr_mode.SetFont(basic_font);
	m_group_left_bcr_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_left_bcr_mode.SetFontBold(TRUE);
	
	m_group_right_bcr_mode.SetFont(basic_font);
	m_group_right_bcr_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_right_bcr_mode.SetFontBold(TRUE);

	m_group_out_conv.SetFont(basic_font);
	m_group_out_conv.SetCatptionTextColor(RGB(145,25,0));
	m_group_out_conv.SetFontBold(TRUE);

	m_group_out_tbox.SetFont(basic_font);
	m_group_out_tbox.SetCatptionTextColor(RGB(145,25,0));
	m_group_out_tbox.SetFontBold(TRUE);

	m_group_bcr_error_time.SetFont(basic_font);
	m_group_bcr_error_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr_error_time.SetFontBold(TRUE);

	m_group_auto_vision_model.SetFont(basic_font);
	m_group_auto_vision_model.SetCatptionTextColor(RGB(145,25,0));
	m_group_auto_vision_model.SetFontBold(TRUE);

	m_group_rework_ccs.SetFont(basic_font);
	m_group_rework_ccs.SetCatptionTextColor(RGB(145,25,0));
	m_group_rework_ccs.SetFontBold(TRUE);
	
	m_group_vision_density_skip.SetFont(basic_font);
	m_group_vision_density_skip.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_density_skip.SetFontBold(TRUE);

}


void CScreen_Basic::OnBasic_EditBox_Set()
{
	m_edit_device_type.SubclassDlgItem(IDC_EDIT_DEVICE_TYPE, this);
	m_edit_device_type.bkColor(RGB(50, 100, 150));
	m_edit_device_type.textColor(RGB(255, 255,255));
	m_edit_device_type.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

void CScreen_Basic::OnBasic_Digital_Count_Set()
{	
	m_dgt_electrostatic_check_gap.SubclassDlgItem(IDC_DGT_ELECTROSTATIC_CHECK_GAP, this);
	m_dgt_electrostatic_check_gap.SetStyle(IDB_BIG3, 4);
	m_dgt_electrostatic_check_gap.SetValue(st_basic.mn_electrostatic_check_gap);
	
	m_dgt_electrostatic_check_value.SetStyle(CDigit::DS_FLOAT3, 7, CDigit::DC_BBLACK);

	m_dgt_alarm_delay_time.SubclassDlgItem(IDC_DGT_ALARM_DELAY_TIME, this);
	m_dgt_alarm_delay_time.SetStyle(IDB_BIG3, 4);
	m_dgt_alarm_delay_time.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_alarm_delay_time ) );

	m_dgt_network_retry_cnt.SubclassDlgItem(IDC_DGT_NETWORK_RETRY_COUNT, this);
	m_dgt_network_retry_cnt.SetStyle(IDB_BIG3, 4);
	m_dgt_network_retry_cnt.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_network_retry_cnt ) );

	m_dgt_network_wait_time.SubclassDlgItem(IDC_DGT_NETWORK_WAIT_TIME, this);
	m_dgt_network_wait_time.SetStyle(IDB_BIG3, 4);
	m_dgt_network_wait_time.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_network_wait_time ) );

	m_dgt_time_out.SubclassDlgItem(IDC_DGT_TIME_OUT, this);
	m_dgt_time_out.SetStyle(IDB_BIG3, 4);
	m_dgt_time_out.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_time_out ) );

	m_dgt_basic_run_speed.SubclassDlgItem(IDC_DGT_RUN_SPEED, this);
	m_dgt_basic_run_speed.SetStyle(IDB_BIG3, 3);
	m_dgt_basic_run_speed.SetValue(g_dataMgr.GetBackupData( &st_basic.nRunSpeed ));
	
	m_dgt_basic_manual_speed.SubclassDlgItem(IDC_DGT_MANUAL_SPEED, this);
	m_dgt_basic_manual_speed.SetStyle(IDB_BIG3, 3);
	m_dgt_basic_manual_speed.SetValue(g_dataMgr.GetBackupData( &st_basic.nManualSpeed ));

	m_dgt_Label_Print_Error_Cnt.SubclassDlgItem(IDC_DGT_LABEL_PRINT_ERROR_CNT, this);
	m_dgt_Label_Print_Error_Cnt.SetStyle(IDB_BIG3, 3);
	m_dgt_Label_Print_Error_Cnt.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_Label_Print_Error_Cnt ) );
	
	m_dgt_label_vision_paper.SubclassDlgItem(IDC_DGT_LABEL_VISION_PAPER, this);
	m_dgt_label_vision_paper.SetStyle(IDB_BIG3, 3);
	m_dgt_label_vision_paper.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_Label_vision_paper ) );

	m_dgt_vision_total_error_cnt.SubclassDlgItem(IDC_DGT_VISION_TOTAL_ERROR_CNT, this);
	m_dgt_vision_total_error_cnt.SetStyle(IDB_BIG3, 3);
	m_dgt_vision_total_error_cnt.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_Label_Print_Total_Error_Cnt ) );
	
	m_dgt_bcr_limit_time.SubclassDlgItem(IDC_DGT_BCR_LIMIT_TIME, this);
	m_dgt_bcr_limit_time.SetStyle(IDB_BIG3, 2);
	m_dgt_bcr_limit_time.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_limit_bcr_time ) );

	m_dgt_top_vision_skip_cnt.SubclassDlgItem(IDC_DGT_TOP_VISION_SKIP_CNT, this);
	m_dgt_top_vision_skip_cnt.SetStyle(IDB_BIG3, 2);
	m_dgt_top_vision_skip_cnt.SetValue( g_dataMgr.GetBackupData( &st_basic.nTopVisionSkipCnt ) );

	m_dgt_reject_offfset.SubclassDlgItem(IDC_DGT_REJECT_OFFSET, this);
	m_dgt_reject_offfset.SetStyle(IDB_BIG3, 3);
// 	m_dgt_reject_offfset.SetValue( g_dataMgr.GetBackupData( &st_basic.nRejectOffset ) );
	m_dgt_reject_offfset.SetValue( st_basic.nRejectOffset );
	//st_basic.nRejectOffset = 144;
}

void CScreen_Basic::OnBasic_Data_Set()
{
	g_dataMgr.BackupDatas();
// 	OnBasicDensity_CapaSet();
}

void CScreen_Basic::OnBasic_Data_Change()
{
	OnBasic_DeviceMode_Change();
	OnBasic_RunMode_Change();

	OnBasic_DeviceType_Change();
	OnBasic_DensityFeeder_Change1();
	OnBasic_DensityFeeder_Change2();
	OnBasic_DensityFeeder_Change3();
	OnBasic_DensityFeeder_Change4();
	OnBasic_DensityFeeder_Change5();
	OnBasic_DensityFeeder_Change6();
	OnBasic_BCRFeeder_Change1();
	OnBasic_BCRFeeder_Change2();
	OnBasic_BCRFeeder_Change3();
	OnBasic_BCRFeeder_Change4();

	OnBasic_Vision_Change();//2014.1104
	OnBasic_Density_Vision_Change();
	OnBasic_Top_Vision_Change();
	OnBasic_Btm_Vision_Change();
	OnBasic_ECServer_Change();
	OnBasic_auto_vision_model();
	OnBasic_Top_Vision_SkipChange();
	OnBasic_XgemMode_Change();
	OnBasic_RcmdMode_Change();
	
	OnBasic_Label_Select();	//2014,1125 ysj

	OnBasic_LeftBcr_Change();
	OnBasic_RightBcr_Change();
	OnBasic_OutConv_Change();
	OnBasic_OutTBox_Change();

	//2016.0916
	OnBasic_Rework_CCS_Change();
	OnBasic_Vision_Density_Skip_Change();

	//2015.0207
	m_group_out_tbox.ShowWindow(TRUE);
	m_radio_use_rejectbox.ShowWindow(TRUE);
	m_radio_use_convbox.ShowWindow(TRUE);
	if(st_basic.mn_outconv == CTL_YES)
	{
		m_group_out_tbox.ShowWindow(FALSE);
		m_radio_use_rejectbox.ShowWindow(FALSE);
		m_radio_use_convbox.ShowWindow(FALSE);
	}
	else
	{
		m_group_out_tbox.ShowWindow(TRUE);
		m_radio_use_rejectbox.ShowWindow(TRUE);
		m_radio_use_convbox.ShowWindow(TRUE);
	}
}

void CScreen_Basic::OnBasic_Label_Set()
{
	m_msg_electrostatic_1.SetFont(Func.mp_basic_font);
	m_msg_electrostatic_1.SetWindowText(_T(""));
	m_msg_electrostatic_1.SetCenterText();
	m_msg_electrostatic_1.SetColor(YELLOW_C);
	m_msg_electrostatic_1.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_1.SetTextColor(RGB(0,0,0));
	
	m_msg_electrostatic_2.SetFont(Func.mp_basic_font);
	m_msg_electrostatic_2.SetWindowText(_T(""));
	m_msg_electrostatic_2.SetCenterText();
	m_msg_electrostatic_2.SetColor(YELLOW_C);
	m_msg_electrostatic_2.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_2.SetTextColor(RGB(0,0,0));
	
	m_msg_electrostatic_3.SetFont(Func.mp_basic_font);
	m_msg_electrostatic_3.SetWindowText(_T(""));
	m_msg_electrostatic_3.SetCenterText();
	m_msg_electrostatic_3.SetColor(YELLOW_C);
	m_msg_electrostatic_3.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_3.SetTextColor(RGB(0,0,0));
	
	m_msg_electrostatic_4.SetFont(Func.mp_basic_font);
	m_msg_electrostatic_4.SetWindowText(_T(""));
	m_msg_electrostatic_4.SetCenterText();
	m_msg_electrostatic_4.SetColor(YELLOW_C);
	m_msg_electrostatic_4.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_4.SetTextColor(RGB(0,0,0));
	
	m_msg_electrostatic_5.SetFont(Func.mp_basic_font);
	m_msg_electrostatic_5.SetWindowText(_T(""));
	m_msg_electrostatic_5.SetCenterText();
	m_msg_electrostatic_5.SetColor(YELLOW_C);
	m_msg_electrostatic_5.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_5.SetTextColor(RGB(0,0,0));
	
	m_label_electrostatic_1.SetFont(Func.mp_basic_font);
	m_label_electrostatic_1.SetWindowText(_T(""));
	m_label_electrostatic_1.SetCenterText();
	m_label_electrostatic_1.SetColor(RGB(0,0,255));
	m_label_electrostatic_1.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_1.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_2.SetFont(Func.mp_basic_font);
	m_label_electrostatic_2.SetWindowText(_T(""));
	m_label_electrostatic_2.SetCenterText();
	m_label_electrostatic_2.SetColor(RGB(0,0,255));
	m_label_electrostatic_2.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_2.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_3.SetFont(Func.mp_basic_font);
	m_label_electrostatic_3.SetWindowText(_T(""));
	m_label_electrostatic_3.SetCenterText();
	m_label_electrostatic_3.SetColor(RGB(0,0,255));
	m_label_electrostatic_3.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_3.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_4.SetFont(Func.mp_basic_font);
	m_label_electrostatic_4.SetWindowText(_T(""));
	m_label_electrostatic_4.SetCenterText();
	m_label_electrostatic_4.SetColor(RGB(0,0,255));
	m_label_electrostatic_4.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_4.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_5.SetFont(Func.mp_basic_font);
	m_label_electrostatic_5.SetWindowText(_T(""));
	m_label_electrostatic_5.SetCenterText();
	m_label_electrostatic_5.SetColor(RGB(0,0,255));
	m_label_electrostatic_5.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_5.SetTextColor(RGB(255,255,255));
}

void CScreen_Basic::OnRadioDeviceModeUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.n_mode_device ) == CTL_YES )
		return;

	g_dataMgr.SetDataTemp( &st_basic.n_mode_device, (int)CTL_YES );
	OnBasic_DeviceMode_Change();
}

void CScreen_Basic::OnRadioDeviceModeNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.n_mode_device ) == CTL_NO )
		return;

	g_dataMgr.SetDataTemp( &st_basic.n_mode_device, (int)CTL_NO );
	OnBasic_DeviceMode_Change();
}

void CScreen_Basic::OnBasic_DeviceMode_Change()
{
	int nDeviceMode = g_dataMgr.GetBackupData( &st_basic.n_mode_device );
	if (nDeviceMode < 0 || nDeviceMode > 1)
	{
		nDeviceMode = 0;
		g_dataMgr.SetDataTemp( &st_basic.n_mode_device, (int)0 );
	}
	
	switch (nDeviceMode)
	{
	case 0:
		m_radio_device_mode_use.SetCheck(FALSE);
		m_radio_device_mode_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_device_mode_use.SetCheck(TRUE);
		m_radio_device_mode_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBtnDvcCreate() 
{
//	int n_response;									// 대화 상자 리턴 플래그
	CString str_selected_dvc;						// 선택된 디바이스 이름 저장 변수

	str_selected_dvc = Keyboard.GetEditString("RECEPI NAME", "");

	if (str_selected_dvc != "")
	{
		mstr_new_device = str_selected_dvc;
		
		if (OnBasic_New_Device_Check() == FALSE)	return;
		

		OnBasic_Create_Device();
		
		// ******************************************************************
		// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
		// ******************************************************************
		m_list_device_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
		mp_device_image_list.DeleteImageList();	// 이미지 리스트에 설정된 정보 삭제
		
		OnBasic_Create_ImageList();				// 이미지 리스트 생성 함수
		
		// Associate image list to list box
		m_list_device_type.SetImageList(&mp_device_image_list);
		
		OnBasic_Init_List(st_path.mstr_path_dvc);// 파일 폴더 초기화 함수
		OnSelchangeListDeviceType();
		OnBasic_Device_Focus_Set();				// 선택된 디바이스명 포커스 설정 함수
		
		if (mn_device_name > 0)
		{
			m_list_device_type.GetText(mn_device_name, str_selected_dvc);
		}
		
		OnBasic_Device_Display(str_selected_dvc);	// 선택된 디바이스 이름 출력 함수
	}
}

int CScreen_Basic::OnBasic_New_Device_Check()
{
	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	// **************************************************************************
    // 입력된 디바이스명 설정한다
    // **************************************************************************
	mstr_new_device.MakeUpper();
	mstr_new_device.TrimLeft(' ');               
	mstr_new_device.TrimRight(' ');
	
	if (mstr_new_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	// **************************************************************************
	
	str_chk_file = st_path.mstr_path_dvc + mstr_new_device;  // 생성할 [폴더]+[파일명] 설정

	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}
	
	mstr_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정
	
	return TRUE;
}

void CScreen_Basic::OnBasic_Create_Device()
{
	int n_existence;								// 파일 존재 유무 플래그
	char chr_file_name[256];
	CString str_device_temp="";
	CString str_original_path="";
	CString str_original_file="";
	CString str_copy_file="";
	CString str_chk_ext="";
	CString str_copy_info="",mstr_temp="";
	
	sprintf(chr_file_name, "%s", mstr_load_file);	// [폴더]+[파일명] 설정
	
	n_existence = access(chr_file_name, 0) ;
	
	if (n_existence != -1)							// 파일 존재
		return ;
	else  
	{
		str_device_temp = st_basic.mstr_device_name;
		st_basic.mstr_device_name = mstr_new_device;
		
		:: WritePrivateProfileString("Device", "DeviceFile", LPCTSTR(mstr_new_device), mstr_load_file); 
		
		//2013,0215
		mstr_temp.Format("%d", st_handler.mn_temp_pogo_type);
		:: WritePrivateProfileString("BasicData", "Pogo_Type", LPCTSTR(mstr_temp), mstr_load_file);
		
		mstr_temp.Format("%d", st_handler.mn_temp_alignBuffer_type);
		:: WritePrivateProfileString("BasicData", "AlignBuffer_Type", LPCTSTR(mstr_temp), mstr_load_file);
		
		mcls_basic.On_Teach_Data_Save();
		
		st_basic.mstr_device_name = str_device_temp;
	}
}

void CScreen_Basic::OnSelchangeListDeviceType() 
{
	CString str_selected_dvc="";						// 선택된 디바이스 이름 저장 변수
	
	int n_index = m_list_device_type.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_device_type.GetText(n_index, str_selected_dvc);
		
		OnBasic_Device_Display(str_selected_dvc);	// 선택된 디바이스 이름 출력 함수
	}
	else
		OnBasic_Device_Display("");	// 빈 이름 출력 함수
}

void CScreen_Basic::OnBasic_Device_Focus_Set()
{
	if (mn_device_name == -1)
		return;
	
	m_list_device_type.SetCurSel(mn_device_name);
}

void CScreen_Basic::OnBasic_Device_Display(CString str_dvc)
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(_T(str_dvc));
}

void CScreen_Basic::OnBasic_Create_ImageList()
{
	BOOL bRetValue = FALSE;
	
	// Create image list
	bRetValue = mp_device_image_list.Create(32, 32, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
}

int CScreen_Basic::OnBasic_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_device_name = -1;
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = AddItem(n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if (n_chk != CTL_READY)
				n_count++;
        }
		::FindClose(hFind);
    }
	
	return n_count;
}

BOOL CScreen_Basic::AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_dvc;
	CString str_device;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_dvc = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_dvc.MakeUpper();
	
	// **************************************************************************
	// 파일 이름 중 확장자 확인
	// **************************************************************************
	n_pos = str_temp_dvc.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_dvc.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return CTL_READY;	
	}
	
	str_device = str_temp_dvc;			// 파일명 전체 화면 출력
	
	if (str_device == g_dataMgr.GetBackupData( &st_basic.mstr_device_name ) )
		mn_device_name = nIndex-1;
	// **************************************************************************
	
	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
	mp_device_image_list.Add(hIcon);
	
    if (m_list_device_type.AddString(_T(str_device), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}

void CScreen_Basic::OnBtnDvcDelete() 
{
	int n_response;				// 대화 상자 리턴 플래그
	int n_existence;			// 파일 존재 유무 플래그
	CString str_file_name;
	CString str_selected_dvc;
	int n_pos;
	
	char chr_file_name[256];
	char chr_speed_file_name[256];
	char chr_teach_file_name[256];
	char chr_time_file_name[256];
	
	CDialog_Select  select_dlg;
	CDialog_Message msg_dlg;
	
	if (OnBasic_Input_Device_Check() == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
		return ;
	
	st_msg.mstr_confirm_msg = _T(mstr_temp_device + " Device Type Delete?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		sprintf(chr_file_name, "%s", mstr_load_file);  // [폴더]+[파일명] 설정

		n_pos = mstr_load_file.Find(".");  // 확장자 검사
		
		if (n_pos == -1) 
		{
			sprintf(chr_speed_file_name, "%s_Speed.DAT", mstr_load_file);  // [폴더]+[파일명] 설정
			sprintf(chr_teach_file_name, "%s_Teach.DAT", mstr_load_file);  // [폴더]+[파일명] 설정
			sprintf(chr_time_file_name, "%s_Time.DAT", mstr_load_file);  // [폴더]+[파일명] 설정
		}
		else 
		{
			sprintf(chr_speed_file_name, "%s_Speed.DAT", mstr_load_file.Mid(0, n_pos));  // [폴더]+[파일명] 설정
			sprintf(chr_teach_file_name, "%s_Teach.DAT", mstr_load_file.Mid(0, n_pos));  // [폴더]+[파일명] 설정
			sprintf(chr_time_file_name, "%s_Time.DAT", mstr_load_file.Mid(0, n_pos));  // [폴더]+[파일명] 설정
		}
		
		n_existence = access(chr_file_name, 0) ;
		
		if (n_existence != -1)					// 파일 존재
		{
			DeleteFile(chr_file_name);			// 해당 파일 삭제
			DeleteFile(chr_speed_file_name);	// 해당 파일 삭제
			DeleteFile(chr_teach_file_name);	// 해당 파일 삭제
			DeleteFile(chr_time_file_name);		// 해당 파일 삭제
			
			// ******************************************************************
			// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
			// ******************************************************************
			m_list_device_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
			mp_device_image_list.DeleteImageList();	// 이미지 리스트에 설정된 정보 삭제
			
			OnBasic_Create_ImageList();				// 이미지 리스트 생성 함수
			
			// Associate image list to list box
			m_list_device_type.SetImageList(&mp_device_image_list);
			
			OnBasic_Init_List(st_path.mstr_path_dvc);// 파일 폴더 초기화 함수
			OnSelchangeListDeviceType();
			OnBasic_Device_Focus_Set();				// 선택된 디바이스명 포커스 설정 함수
			
			if (mn_device_name > -1)
			{
				m_list_device_type.GetText(mn_device_name, str_selected_dvc);
				
				OnBasic_Device_Display(str_selected_dvc);	// 선택된 디바이스 이름 출력 함수
			}
			// ******************************************************************
		}
		else  
			return ;
	}
	else if (n_response == IDCANCEL)
	{
	}
}

int CScreen_Basic::OnBasic_Input_Device_Check()
{
	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	// **************************************************************************
    // 입력된 디바이스명 설정한다                                                
    // **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device) ;
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (mstr_temp_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	// **************************************************************************
	
	str_chk_file = st_path.mstr_path_dvc + mstr_temp_device;  // 생성할 [폴더]+[파일명] 설정
	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}
	
	mstr_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정
	
	return TRUE;
}


LRESULT CScreen_Basic::OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	if (wParam == 0)
	{
		g_dataMgr.CheckAndLogAll();
		OnBasic_Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnBasic_Data_Set();
		
		OnBasic_TitleBar_Info_Send();   // 타이틀 바 화면에 정보 출력 요청하는 함수
	}
	else if (wParam == 1)
	{
		OnBasic_Electrostatic_Data_View(lParam);
	}
	
	return 0;
}

void CScreen_Basic::OnBasic_Electrostatic_Data_View(int nChannel)
{
	CString strData="";
	switch (nChannel)
	{
	case 1:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_1.SetWindowText(_T(strData));
		break;
		
	case 2:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_2.SetWindowText(_T(strData));
		break;
		
	case 3:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_3.SetWindowText(_T(strData));
		break;
		
	case 4:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_4.SetWindowText(_T(strData));
		break;
		
	case 5:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_5.SetWindowText(_T(strData));
		m_btn_electostatic_test.EnableWindow(TRUE);
		break;
	}
}

void CScreen_Basic::OnBasic_Data_Apply()
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device);
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (st_basic.mstr_device_name != mstr_temp_device)
	{
		if( g_lotMgr.GetLotCount() > 0 )
		{
			m_strTempMSG = "진행중인 랏이 있습니다.";
			if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
			DoModal_Msg( m_strTempMSG );
			OnBasic_Device_Focus_Set();
			return;
		}

		OnBasic_DeviceData_Apply();
		
		OnBasic_Data_Set();				// 전역 변수의 Data를 받아온다.
		
		OnBasic_Data_Change();

		OnBasicDensity_CapaSet();

		m_cbolabel1capa.SetWindowText(st_basic.n_str_capa1label);
		m_cbolabel2capa.SetWindowText(st_basic.n_str_capa2label);
		m_cbolabel3capa.SetWindowText(st_basic.n_str_capa3label);

		OnBasic_Data_Display();


	}
	else
	{
		g_dataMgr.UpdateDatas();
		OnBasicDensity_CapaSet();

	}

	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}
}

void CScreen_Basic::OnBasic_TitleBar_Info_Send()
{
	
	if(st_handler.cwnd_title != NULL)
	{
		int nDeviceMode = g_dataMgr.GetBackupData( &st_basic.n_mode_device );
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, nDeviceMode );			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, ONLINE_MODE, 0);
// 		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, BCR_READ_MODE, st_basic.n_bcr_Mode);
		if(st_basic.mn_outconv == CTL_YES)
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, CONV_MODE, 2);
		else
		{
			if(st_basic.mn_outtbox == 0)
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, CONV_MODE, 0);
			else
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, CONV_MODE, 1);
		}
		
	}
	
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);
	}
}

void CScreen_Basic::OnBasic_DeviceData_Apply()
{
	int n_response = 0;  // 대화 상자 리턴 플래그
	int n_check;
	CString str_msg;
	
	n_check = OnBasic_Input_Data_Check();  // 입력 정보 정상 유무 검사 함수
	
	if (n_check == TRUE)  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // 디바이스명 출력 요청
		
		
		g_dataMgr.CheckAndLogByPointer( &st_basic.mstr_device_name );

	}
	else if (n_check == FALSE)  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[LOT INFORMATION] An input device setting up error.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return ;
	}
	
	g_dataMgr.UpdateData( &st_basic.mstr_device_name );
	
	// **********************************************************************
	// 디바이스 관련 화면 설정 데이터 로딩 부분                              
	// **********************************************************************
	mcls_basic.OnBasic_Data_Load();
	mcls_basic.On_Teach_Data_Load();
//	mcls_basic.OnBarcode_Label_Data_Load();
	// **********************************************************************
		
	st_work.mn_reinstatement_mode = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	
	// **************************************************************************
	// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다                        
	// **************************************************************************
	m_list_device_type.ResetContent();			// 리스트 박스의 모든 목록 삭제
	mp_device_image_list.DeleteImageList();		// 이미지 리스트에 설정된 정보 삭제
	
	OnBasic_Create_ImageList();					// 이미지 리스트 생성 함수
	
	// Associate image list to list box
	m_list_device_type.SetImageList(&mp_device_image_list);
	
	OnBasic_Init_List(st_path.mstr_path_dvc);	// 파일 폴더 초기화 함수
	
	OnBasic_Device_Focus_Set();					// 선택된 디바이스명 포커스 설정 함수
	// **************************************************************************
	
// 	st_basic.mstr_vision_model_name1 = str_vision_model_name1[1];
// 	st_basic.mstr_vision_model_name2 = str_vision_model_name2[1];
// 	st_basic.mstr_vision_model_name3 = str_vision_model_name3[1];
// 	st_basic.mstr_vision_model_name4 = str_vision_model_name4[1];
// 	
// 	m_list_vision_ml1.ResetContent();
// 	m_list_vision_ml2.ResetContent();
// 	m_list_vision_ml3.ResetContent();
// 	m_list_vision_ml4.ResetContent();
// 	
// 	for(int i=0; i<4; i++)
// 	{
// 		OnBasic_Init_Model_List(st_path.str_Model_List_folder, i);
// 		OnBasic_Model_Focus_Set(i);
// 	}



	if (st_handler.cwnd_list != NULL)			// 리스트 바 화면 존재
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // 디바이스 종류 출력 요청
	
	//2013,0215
	OnBasic_GroupBox_Set();	
}

void CScreen_Basic::OnBasic_Data_Display()
{

	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText( g_dataMgr.GetBackupData( &st_basic.mstr_device_name ) );

	m_dgt_electrostatic_check_gap.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_electrostatic_check_gap ) );
	m_dgt_electrostatic_check_value.SetFloatVal((float)g_dataMgr.GetBackupData( &st_basic.md_electrostatic_value) );
	
	m_dgt_alarm_delay_time.SetValue(g_dataMgr.GetBackupData( &st_basic.mn_alarm_delay_time));
	
	m_dgt_network_wait_time.SetValue(g_dataMgr.GetBackupData( &st_basic.mn_network_wait_time));		// Network 대기 시간
	m_dgt_network_retry_cnt.SetValue(g_dataMgr.GetBackupData( &st_basic.mn_network_retry_cnt));		// Network Retry 횟수
	
	m_dgt_Label_Print_Error_Cnt.SetValue(g_dataMgr.GetBackupData( &st_basic.mn_Label_Print_Error_Cnt));		// Network Retry 횟수
	m_dgt_label_vision_paper.SetValue(g_dataMgr.GetBackupData( &st_basic.mn_Label_vision_paper));
	
	m_dgt_time_out.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_time_out ) );
	m_dgt_basic_manual_speed.SetValue(g_dataMgr.GetBackupData( &st_basic.nManualSpeed ));
	m_dgt_basic_run_speed.SetValue(g_dataMgr.GetBackupData( &st_basic.nRunSpeed ));

	//2015.0211
	m_dgt_vision_total_error_cnt.SetValue( g_dataMgr.GetBackupData( &st_basic.mn_Label_Print_Total_Error_Cnt ) );
	m_dgt_bcr_limit_time.SetValue(g_dataMgr.GetBackupData( &st_basic.mn_limit_bcr_time));		// Network 대기 시간
	m_dgt_top_vision_skip_cnt.SetValue( g_dataMgr.GetBackupData( &st_basic.nTopVisionSkipCnt ) );
	//m_dgt_reject_offfset.SetValue( g_dataMgr.GetBackupData( &st_basic.nRejectOffset ) );
	m_dgt_reject_offfset.SetValue( st_basic.nRejectOffset );

}

int CScreen_Basic::OnBasic_Input_Data_Check()
{
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device) ;
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (mstr_temp_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)			// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */
	
	n_pos = mstr_temp_device.Find(".");				// 확장자 검사
	if (n_pos == -1) 
		mstr_temp_device += _T(".TXT");
	else 
	{
		str_chk_ext = mstr_temp_device.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)		// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}
	
	g_dataMgr.SetDataTemp( &st_basic.mstr_device_name, mstr_temp_device );
	/* ************************************************************************** */
	
	return TRUE;
}

void CScreen_Basic::OnBtnBasicApply() 
{
	CString str_selected_dvc;
	int nDeviceCur = m_list_device_type.GetCurSel();

	// 바꿀 수 없는 상황 고려하시오.

	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : Setting Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		g_dataMgr.CheckAndLogAll();
		//2013,0204
		mcls_basic.OnDaily_Count_Save();
		OnBasic_Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnBasic_Data_Set();
		if(mn_bakDevice_Type != st_basic.mn_device_type)
		{
// 			st_var.n_newLabelType_flag = CTL_YES;
// 			st_var.mn_new_density_count = 0;
			st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
			st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
			st_var.n_use_density_count[st_basic.mn_device_type] = 0;//reset
			st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;//reset
		}
		

		OnBasic_TitleBar_Info_Send();    // 타이틀 바 화면에 정보 출력 요청하는 함수

		Func.OnSavePPID();//2015.0221

	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnBtnBasicCancle() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : Previous Data Recovery?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Data_Set();					// Recovery		// 베이식 화면 셋팅 정보 백업 받아놓은 변수로 복구 함수
		
		OnBasic_Data_Change();				// Data에 맞게 변경.
		
		OnBasic_Data_Display();
		
		OnBasic_TitleBar_Info_Send();		// 타이틀 바 화면에 정보 출력 요청하는 함수
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtNetworkWaitTime() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	int mn_minute;
		
	mn_minute = m_dgt_network_wait_time.GetValue();
	
	mstr_temp = _T("Network 메세지 대기 시간 (초)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Network wait time (sec)");
	
	CRect r;
	
	m_dgt_network_wait_time.GetWindowRect(&r);
	
	mstr_temp = KeyPad.GetNumEditString_I(0, 100, mn_minute, mstr_temp, &r);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_network_wait_time, iTemp );
	m_dgt_network_wait_time.SetValue(iTemp);
}

void CScreen_Basic::OnDgtNetworkRetryCount() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_network_retry_cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Network 메세지 재전송 횟수");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("Network retry count");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(3, 10, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_network_retry_cnt, iTemp );
	m_dgt_network_retry_cnt.SetValue( iTemp );
}

void CScreen_Basic::OnDgtAlarmDelayTime() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Sec;
	
	mn_Sec = m_dgt_alarm_delay_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Sec, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Alarm Delay Time (Sec)");
    
	st_msg.mstr_keypad_val = mstr_temp;
		
	mstr_temp = KeyPad.GetNumEditString_I(1, 1000, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_alarm_delay_time, iTemp ); 
	m_dgt_alarm_delay_time.SetValue( iTemp );
}



void CScreen_Basic::OnBasic_RunMode_Change()
{
	int nRunMode = g_dataMgr.GetBackupData( &st_basic.mn_mode_run );
	if (nRunMode < 0 || nRunMode > 1)
	{
		nRunMode = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_mode_run, (int)0 );
	}
	
	switch (nRunMode)
	{
	case 0:
		m_radio_online_mode.SetCheck(FALSE);
		m_radio_offline_mode.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_online_mode.SetCheck(TRUE);
		m_radio_offline_mode.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioOnlineMode() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_mode_run ) == CTL_YES )
		return;

	g_dataMgr.SetDataTemp( &st_basic.mn_mode_run, (int)CTL_YES );
	OnBasic_RunMode_Change();
}

void CScreen_Basic::OnRadioOfflineMode() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_mode_run ) == CTL_NO )
		return;

	g_dataMgr.SetDataTemp( &st_basic.mn_mode_run, (int)CTL_NO );
	OnBasic_RunMode_Change();
}

void CScreen_Basic::OnDgtTimeOut() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	int mn_sec;
		
	mn_sec = m_dgt_time_out.GetValue();
	
	mstr_temp = _T("테스트 타임아웃 (초)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Test Time Out (sec)");
	
	CRect r;
	
	m_dgt_network_wait_time.GetWindowRect(&r);
	mstr_temp = KeyPad.GetNumEditString_I(20, 9999, mn_sec, mstr_temp, &r);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_time_out, iTemp );
	m_dgt_time_out.SetValue(iTemp);
}



void CScreen_Basic::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	COLORREF bk_old, tk_old;
	Grid->GetColor(col, row, &bk_old, &tk_old);
	if( bk_old != bk || tk_old != tk )
	{
		Grid->SetColor(col, row, bk, tk);
		Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	}

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Basic::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
			Grid->SetTypeComboBox(&CellType, 0, strList);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0, 0, 0, 9999,0,".",",",0,0,1,0,1.001);
			break;
	}
	
	Grid->SetCellType(col,row,&CellType);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Basic::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	char szBuf[1024];
	Grid->GetValue(col, row, szBuf);
	if( data == szBuf )
	{
		if (Grid != NULL)
		{	
			Grid = NULL;
			delete Grid;
		}
		return;
	}

	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

CString CScreen_Basic::GetGridData( UINT nID, int row, int col )
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	char szBuf[1024];
	Grid->GetValue(col, row, szBuf);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}

	return szBuf;
}


void CScreen_Basic::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Basic::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}



void CScreen_Basic::OnDgtRunSpeed() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	int mn_sec;
	
	mn_sec = m_dgt_basic_run_speed.GetValue();
	
	mstr_temp = _T("로봇 Run 동작 속도(%)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Robot Run Speed (%)");
	
	CRect r;
	
	m_dgt_network_wait_time.GetWindowRect(&r);
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, mn_sec, mstr_temp, &r);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.nRunSpeed, iTemp );
	m_dgt_basic_run_speed.SetValue(iTemp);		
}

void CScreen_Basic::OnDgtManualSpeed() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	int mn_sec;
	
	mn_sec = m_dgt_basic_manual_speed.GetValue();
	
	mstr_temp = _T("로봇 Manual 동작 속도(%)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Robot Manual Speed (%)");
	
	CRect r;
	
	m_dgt_network_wait_time.GetWindowRect(&r);
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, mn_sec, mstr_temp, &r);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.nManualSpeed, iTemp );
	m_dgt_basic_manual_speed.SetValue(iTemp);		
}

////2014.0823
void CScreen_Basic::OnBasic_Init_Time()
{

	CString str_tmp[8] = { "EQP Model", "EQP No", "With Mode", "", "Online Mode", "", "BCR Read Mode", "" };
	CString strTemp;

	int nRowCnt = 8;
	int i,j;

	m_grid_mode = (TSpread*)GetDlgItem(IDC_CUSTOM_BASIC);
	mp_grid.GridReset(m_grid_mode);

	// 대문자 
	mp_grid.GridCellSelectDisplay(m_grid_mode, TRUE);
	mp_grid.GridRowHeader(m_grid_mode, FALSE);
	mp_grid.GridColHeader(m_grid_mode, FALSE);
	mp_grid.GridHorizontal(m_grid_mode, FALSE);
	mp_grid.GridVertical(m_grid_mode, FALSE);
	mp_grid.GridAutoSize(m_grid_mode, FALSE);
	
	mp_grid.GridCellRows(m_grid_mode, nRowCnt);
	mp_grid.GridCellCols(m_grid_mode, 2);


	for(i=0; i<nRowCnt; i++)
	{
		mp_grid.GridCellHeight_L(m_grid_mode, i+1, (double)23);
		
		for(j=0; j<2; j++)
		{
			switch(j)
			{
				case 0:
					mp_grid.GridCellWidth_L(m_grid_mode, j+1, (double)18);
					mp_grid.GridCellColor(m_grid_mode, i+1, j+1, BLUE_D, WHITE_C);
					break;

				case 1:
					mp_grid.GridCellWidth_L(m_grid_mode, j+1, (double)20);
					mp_grid.GridCellColor(m_grid_mode, i+1, j+1, RGB(50, 100, 150), WHITE_C);
					break;
			}
			mp_grid.GridCellControlStatic(m_grid_mode, i+1, j+1);
		}
		mp_grid.GridCellFont(m_grid_mode, i+1, 1, "Tahoma", 14);
		mp_grid.GridCellText(m_grid_mode, i+1, 1, str_tmp[i]);
		mp_grid.GridCellFont(m_grid_mode, i+1, 2, "Tahoma", 20);
	}
	mp_grid.GridCellMerge(m_grid_mode, 3, 1, 2, 1);	// With Mode
	mp_grid.GridCellMerge(m_grid_mode, 5, 1, 2, 1);	// Online Mode
	mp_grid.GridCellMerge(m_grid_mode, 7, 1, 2, 1);	// BCR Read Mode
	
	mp_grid.GridCellText(m_grid_mode, 1, 2, st_lamp.mstr_equip_id);
	mp_grid.GridCellText(m_grid_mode, 2, 2, st_lamp.mstr_equip_no);
	mp_grid.GridCellText(m_grid_mode, 3, 2, "With");
	mp_grid.GridCellText(m_grid_mode, 4, 2, "With Out");
	mp_grid.GridCellText(m_grid_mode, 5, 2, "On-Line");
	mp_grid.GridCellText(m_grid_mode, 6, 2, "Off-Line");
	mp_grid.GridCellText(m_grid_mode, 7, 2, "Continuity");
	mp_grid.GridCellText(m_grid_mode, 8, 2, "Step");

	OnBasic_Grid_View();
}

void CScreen_Basic::OnBasic_Grid_View()
{
	m_grid_mode = (TSpread*)GetDlgItem(IDC_CUSTOM_BASIC);

	if(g_dataMgr.GetBackupData( &st_basic.n_mode_device ) == CTL_YES)
	{
		mp_grid.GridCellColor(m_grid_mode, 3, 2, RED_C, WHITE_C);
		mp_grid.GridCellColor(m_grid_mode, 4, 2, BLACK_L, WHITE_C);
	}
	else
	{
		mp_grid.GridCellColor(m_grid_mode, 3, 2, BLACK_L, WHITE_C);
		mp_grid.GridCellColor(m_grid_mode, 4, 2, RED_C, WHITE_C);
	}

	if(g_dataMgr.GetBackupData( &st_basic.mn_mode_run ) == CTL_YES)
	{
		mp_grid.GridCellColor(m_grid_mode, 5, 2, RED_C, WHITE_C);
		mp_grid.GridCellColor(m_grid_mode, 6, 2, BLACK_L, WHITE_C);
	}
	else
	{
		mp_grid.GridCellColor(m_grid_mode, 5, 2, BLACK_L, WHITE_C);
		mp_grid.GridCellColor(m_grid_mode, 6, 2, RED_C, WHITE_C);
	}

	if(st_basic.n_bcr_Mode == CTL_YES)
	{
		mp_grid.GridCellColor(m_grid_mode, 7, 2, RED_C, WHITE_C);
		mp_grid.GridCellColor(m_grid_mode, 8, 2, BLACK_L, WHITE_C);
	}
	else
	{
		mp_grid.GridCellColor(m_grid_mode, 7, 2, BLACK_L, WHITE_C);
		mp_grid.GridCellColor(m_grid_mode, 8, 2, RED_C, WHITE_C);
	}
}

void CScreen_Basic::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	int nRowPos, nColPos;
	int n_response;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;
	
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	if (wParam==IDC_CUSTOM_BASIC)
	{
		if (mn_enable != TRUE)	return;
		if (lpcc->Col == 1)	return;
	
		m_grid_mode = (TSpread*)GetDlgItem(IDC_CUSTOM_BASIC);
		nRowPos = lpcc->Row;
		nColPos = lpcc->Col;

		switch(lpcc->Row)
		{
			case 1:
				st_msg.mstr_typing_msg = mstr_equip_id[1];
				
				n_response = keyboard_dlg.DoModal();
				if (n_response == IDOK)
				{
					mstr_equip_id[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정	
					mstr_equip_id[1].MakeUpper();
					mstr_equip_id[1].TrimLeft(' ');               
					mstr_equip_id[1].TrimRight(' ');
					
					mp_grid.GridCellText(m_grid_mode, 1, 2, mstr_equip_id[1]);
					st_lamp.mstr_equip_id = mstr_equip_id[1];
				}
				break;
				
			case 2:
				st_msg.mstr_typing_msg = mstr_equip_no[1];
				
				n_response = keyboard_dlg.DoModal();
				if (n_response == IDOK)
				{
					mstr_equip_no[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정
					mstr_equip_no[1].MakeUpper();
					mstr_equip_no[1].TrimLeft(' ');               
					mstr_equip_no[1].TrimRight(' ');
					
					mp_grid.GridCellText(m_grid_mode, 2, 2, mstr_equip_no[1]);
					st_lamp.mstr_equip_no = mstr_equip_no[1];
				}
				break;
				
				// With
			case 3:
				if( g_dataMgr.GetBackupData( &st_basic.n_mode_device ) == CTL_YES )
					return;
				
				g_dataMgr.SetDataTemp( &st_basic.n_mode_device, (int)CTL_YES );
				st_basic.n_mode_device = g_dataMgr.GetBackupData( &st_basic.n_mode_device );
				OnBasic_DeviceMode_Change();
				OnBasic_Grid_View();
				break;
				
				// With Out
			case 4:
				if( g_dataMgr.GetBackupData( &st_basic.n_mode_device ) == CTL_NO )
					return;
				
				g_dataMgr.SetDataTemp( &st_basic.n_mode_device, (int)CTL_NO );
				st_basic.n_mode_device = g_dataMgr.GetBackupData( &st_basic.n_mode_device );
				OnBasic_DeviceMode_Change();
				OnBasic_Grid_View();
				break;
			
			case 5:	
				if( g_dataMgr.GetBackupData( &st_basic.mn_mode_run ) == CTL_YES )
					return;
				
				g_dataMgr.SetDataTemp( &st_basic.mn_mode_run, (int)CTL_YES );
				st_basic.mn_mode_run = g_dataMgr.GetBackupData( &st_basic.mn_mode_run );
				OnBasic_RunMode_Change();
				OnBasic_Grid_View();
				break;

			case 6:
				if( g_dataMgr.GetBackupData( &st_basic.mn_mode_run ) == CTL_NO )
					return;
				
				g_dataMgr.SetDataTemp( &st_basic.mn_mode_run, (int)CTL_NO );
				st_basic.mn_mode_run = g_dataMgr.GetBackupData( &st_basic.mn_mode_run );
				OnBasic_RunMode_Change();
				OnBasic_Grid_View();
				break;

				// Continuity
			case 7:
				if( st_basic.n_bcr_Mode == CTL_YES )
					return;
				
				st_basic.n_bcr_Mode = CTL_YES;
				OnBasic_Grid_View();
				break;
				
				// Step
			case 8:
				if( st_basic.n_bcr_Mode == CTL_NO )
					return;
				
				st_basic.n_bcr_Mode = CTL_NO;
				OnBasic_Grid_View();
				break;
		}

	}
}

void CScreen_Basic::On_Controls_Enable(int nState)
{
	mn_enable = nState;
}	

void CScreen_Basic::OnBtnElectrostaticTest() 
{
	m_msg_electrostatic_1.SetWindowText(_T(""));
	m_msg_electrostatic_2.SetWindowText(_T(""));
	m_msg_electrostatic_3.SetWindowText(_T(""));
	m_msg_electrostatic_4.SetWindowText(_T(""));
	m_msg_electrostatic_5.SetWindowText(_T(""));
	
// 	::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 2, 0);
	
	m_btn_electostatic_test.EnableWindow(FALSE);	
}

void CScreen_Basic::OnRadioUseXgem() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_mode_xgem ) == CTL_YES )
		return;

	g_dataMgr.SetDataTemp( &st_basic.mn_mode_xgem, (int)CTL_YES );
	OnBasic_XgemMode_Change();		
}

void CScreen_Basic::OnRadioNotuseXgem() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_mode_xgem ) == CTL_NO )
		return;

	g_dataMgr.SetDataTemp( &st_basic.mn_mode_xgem, (int)CTL_NO );
	OnBasic_XgemMode_Change();		
}

void CScreen_Basic::OnBasic_XgemMode_Change()
{
	int nXgemMode = g_dataMgr.GetBackupData( &st_basic.mn_mode_xgem );
	if (nXgemMode < 0 || nXgemMode > 1)
	{
		nXgemMode = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_mode_xgem, (int)0 );
	}

	//2013,1216
	
	switch (nXgemMode)
	{
	case 0:
		m_radio_use_xgem.SetCheck(FALSE);
		m_radio_notuse_xgem.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_use_xgem.SetCheck(TRUE);
		m_radio_notuse_xgem.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioUseRcmd() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_mode_rcmd ) == CTL_YES )
		return;

	g_dataMgr.SetDataTemp( &st_basic.mn_mode_rcmd, (int)CTL_YES );
	OnBasic_RcmdMode_Change();		
	
}

void CScreen_Basic::OnRadioNotuseRcmd() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_mode_rcmd ) == CTL_NO )
		return;

	g_dataMgr.SetDataTemp( &st_basic.mn_mode_rcmd, (int)CTL_NO );
	OnBasic_RcmdMode_Change();		
	
}

void CScreen_Basic::OnBasic_RcmdMode_Change()
{
	int nRcmdMode = g_dataMgr.GetBackupData( &st_basic.mn_mode_rcmd );
	if (nRcmdMode < 0 || nRcmdMode > 1)
	{
		nRcmdMode = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_mode_rcmd, (int)0 );
	}
	
	switch (nRcmdMode)
	{
	case 0:
		m_radio_use_rcmd.SetCheck(FALSE);
		m_radio_notuse_rcmd.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_use_rcmd.SetCheck(TRUE);
		m_radio_notuse_rcmd.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBtnAlarmFileRead() 
{
	// TODO: Add your control notification handler code here
	MyJamData.On_Alarm_Info_Load();
	
}

void CScreen_Basic::OnDgtElectrostaticCheckGap() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_electrostatic_check_gap.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("정전기 확인 시간 (분)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("Check the electrostatic time (minutes)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 1440, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp(&st_basic.mn_electrostatic_check_gap, iTemp );
	m_dgt_electrostatic_check_gap.SetValue( iTemp );
	
}

void CScreen_Basic::OnDgtElectrostaticCheckValue() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double d_dgt_data;
	
	d_dgt_data = (double)m_dgt_electrostatic_check_value.GetFloatVal();
	
	mstr_temp = _T("그라운드 설정값");
	if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Setting ground");
	
	CRect r;
	
	m_dgt_electrostatic_check_value.GetWindowRect(&r);
	
	mstr_temp = KeyPad.GetNumEditString_D((double)0, (double)120, d_dgt_data, mstr_temp, &r);
	
	d_dgt_data = atof(mstr_temp);
	
	g_dataMgr.SetDataTemp( &st_basic.md_electrostatic_value, d_dgt_data );
	m_dgt_electrostatic_check_value.SetFloatVal((float)d_dgt_data);	
}

void CScreen_Basic::OnDgtLabelPrintErrorCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_Label_Print_Error_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Label Error Count");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("Label Error Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 10, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_Label_Print_Error_Cnt, iTemp );
	m_dgt_Label_Print_Error_Cnt.SetValue( iTemp );
	
}

void CScreen_Basic::OnDgtVisionTotalErrorCnt() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_vision_total_error_cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 500));
	
	st_msg.mstr_keypad_msg = _T("Label Error Count");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("VIsion Total Error Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 500, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_Label_Print_Total_Error_Cnt, iTemp );
	m_dgt_vision_total_error_cnt.SetValue( iTemp );		
}

void CScreen_Basic::OnDgtLabelVisionPaper() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_label_vision_paper.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("From Printer to label");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("From Printer to label");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 20, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_Label_vision_paper, iTemp );
	m_dgt_label_vision_paper.SetValue( iTemp );
	
}

void CScreen_Basic::OnBtnVisionModel1() 
{
	// TODO: Add your control notification handler code here
// 	if(st_handler.mnSerial_Port_Creating[BCR_PORT] == CTL_YES)
// 	{
// 		sprintf(st_msg.c_normal_msg, "[BASIC] Vision Model List Button Click");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}
// 
// 		st_serial.b_ml_select = TRUE;
// 		m_list_vision_ml1.ResetContent(); //20120625	
// 		Func.OnSet_Vision_send(VISION_ML, BCR_PORT);
// 	}
// 	else
// 	{
// 		sprintf(st_msg.c_normal_msg, "[Manual] Please connect the communication port");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}	
// 	}		
}

void CScreen_Basic::OnBtnVisionModel2() 
{
	// TODO: Add your control notification handler code here
// 	if(st_handler.mnSerial_Port_Creating[BCR_PORT2] == CTL_YES)
// 	{
// 		sprintf(st_msg.c_normal_msg, "[BASIC] Vision Model List Button Click");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}
// 		
// 		st_serial.b_ml_select = TRUE;
// 		m_list_vision_ml2.ResetContent(); //20120625	
// 		Func.OnSet_Vision_send(VISION_ML, BCR_PORT2);
// 	}
// 	else
// 	{
// 		sprintf(st_msg.c_normal_msg, "[Manual] Please connect the communication port");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}	
// 	}		
}

void CScreen_Basic::OnBtnVisionModel3() 
{
	// TODO: Add your control notification handler code here
// 	if(st_handler.mnSerial_Port_Creating[BCR_PORT3] == CTL_YES)
// 	{
// 		sprintf(st_msg.c_normal_msg, "[BASIC] Vision Model List Button Click");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}
// 		
// 		st_serial.b_ml_select = TRUE;
// 		m_list_vision_ml3.ResetContent(); //20120625	
// 		Func.OnSet_Vision_send(VISION_ML, BCR_PORT3);
// 	}
// 	else
// 	{
// 		sprintf(st_msg.c_normal_msg, "[Manual] Please connect the communication port");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}	
// 	}		
}

void CScreen_Basic::OnBtnVisionModel4() 
{
	// TODO: Add your control notification handler code here
// 	if(st_handler.mnSerial_Port_Creating[BCR_PORT4] == CTL_YES)
// 	{
// 		sprintf(st_msg.c_normal_msg, "[BASIC] Vision Model List Button Click");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}
// 		
// 		st_serial.b_ml_select = TRUE;
// 		m_list_vision_ml4.ResetContent(); //20120625	
// 		Func.OnSet_Vision_send(VISION_ML, BCR_PORT4);
// 	}
// 	else
// 	{
// 		sprintf(st_msg.c_normal_msg, "[Manual] Please connect the communication port");
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 		}	
// 	}		
}

void CScreen_Basic::OnSelchangeListVisionModel1() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	
	int n_index = m_list_vision_ml1.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_vision_ml1.GetText(n_index, str_selected_lbl);
		str_vision_model_name1[1] = str_selected_lbl;
		n_vision_model_index1[1] = n_index;	
	}
}

void CScreen_Basic::OnSelchangeListVisionModel2() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	
	int n_index = m_list_vision_ml2.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_vision_ml2.GetText(n_index, str_selected_lbl);
		str_vision_model_name2[1] = str_selected_lbl;
		n_vision_model_index2[1] = n_index;		
	}
}

void CScreen_Basic::OnSelchangeListVisionModel3() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	
	int n_index = m_list_vision_ml3.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_vision_ml3.GetText(n_index, str_selected_lbl);
		str_vision_model_name3[1] = str_selected_lbl;
		n_vision_model_index3[1] = n_index;	
	}
}

void CScreen_Basic::OnSelchangeListVisionModel4() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	
	int n_index = m_list_vision_ml4.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_vision_ml4.GetText(n_index, str_selected_lbl);
		str_vision_model_name4[1] = str_selected_lbl;
		n_vision_model_index4[1] = n_index;	
	}
}

int CScreen_Basic::OnBasic_Init_Model_List(LPCTSTR pszPath, int nVisionNo)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수
	
	if(nVisionNo == 0) 
	{
		str_path += "1\\"; 
		mn_bcr_ml_name1 = -1;
	}
	if(nVisionNo == 1) 
	{
		str_path += "2\\"; 
		mn_bcr_ml_name2 = -1;
	}
	if(nVisionNo == 2) 
	{
		str_path += "3\\"; 
		mn_bcr_ml_name3 = -1;
	}
	if(nVisionNo == 3) 
	{
		str_path += "4\\"; 
		mn_bcr_ml_name4 = -1;
	}
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
	
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = Model_AddItem(nVisionNo, n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = Model_AddItem(nVisionNo, n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if (n_chk != CTL_READY)
				n_count++;
        }
		::FindClose(hFind);
    }
	
	return n_count;
}

void CScreen_Basic::OnBasic_Model_Focus_Set(int nVisionNo) // 20130102
{
	if(nVisionNo == 0) 
	{
		if(mn_bcr_ml_name1 == -1)
			return;
		m_list_vision_ml1.SetCurSel(mn_bcr_ml_name1);
	}
	if(nVisionNo == 1) 
	{
		if(mn_bcr_ml_name2 == -1)
			return;
		m_list_vision_ml2.SetCurSel(mn_bcr_ml_name2);
	}
	if(nVisionNo == 2) 
	{
		if(mn_bcr_ml_name3 == -1)
			return;
		m_list_vision_ml3.SetCurSel(mn_bcr_ml_name3);
	}
	if(nVisionNo == 3) 
	{
		if(mn_bcr_ml_name4 == -1)
			return;
		m_list_vision_ml4.SetCurSel(mn_bcr_ml_name4);
	}
}

BOOL CScreen_Basic::Model_AddItem(int nVisionNo, int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_ml;
	CString str_ml;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_ml = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_ml.MakeUpper();
	
	// **************************************************************************
	// 파일 이름 중 확장자 확인
	// **************************************************************************
	n_pos = str_temp_ml.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_ml.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return CTL_READY;	
	}
	
	str_ml=str_temp_ml;			// 파일명 전체 화면 출력
	
	if(nVisionNo == 0) 
	{
		if (str_ml == st_basic.mstr_vision_model_name1) 
			mn_bcr_ml_name1 = nIndex-1;
		if (m_list_vision_ml1.AddString(_T(str_ml), nIndex - 1) == -1)
			return FALSE;
	}
	if(nVisionNo == 1) 
	{
		if (str_ml == st_basic.mstr_vision_model_name2) 
			mn_bcr_ml_name2 = nIndex-1;
		if (m_list_vision_ml2.AddString(_T(str_ml), nIndex - 1) == -1)
			return FALSE;
	}
	if(nVisionNo == 2) 
	{
		if (str_ml == st_basic.mstr_vision_model_name3) 
			mn_bcr_ml_name3 = nIndex-1;
		if (m_list_vision_ml3.AddString(_T(str_ml), nIndex - 1) == -1)
			return FALSE;
	}
	if(nVisionNo == 3) 
	{
		if (str_ml == st_basic.mstr_vision_model_name4) 
			mn_bcr_ml_name4 = nIndex-1;
		if (m_list_vision_ml4.AddString(_T(str_ml), nIndex - 1) == -1)
			return FALSE;
	}	
    return TRUE;
}



void CScreen_Basic::OnRadioDeviceType1() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.mn_device_type ) == 0 )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_device_type, 0 );
	OnBasic_DeviceType_Change();

	st_handler.mn_density_viscnt = 0;
	
}

void CScreen_Basic::OnRadioDeviceType2() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.mn_device_type ) == 1 )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_device_type, 1 );
	OnBasic_DeviceType_Change();

	st_handler.mn_density_viscnt = 0;
	
}

void CScreen_Basic::OnRadioDeviceType3() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.mn_device_type ) == 2 )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_device_type, 2 );
	OnBasic_DeviceType_Change();

	st_handler.mn_density_viscnt = 0;
	
}

void CScreen_Basic::OnBasic_DeviceType_Change()
{
	int DeviceType = g_dataMgr.GetBackupData( &st_basic.mn_device_type );
	if (DeviceType < 0 || DeviceType > 2)
	{
		DeviceType = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_device_type, DeviceType );
	}
	
	switch (DeviceType)
	{
	case 0:
		m_radio_device_type_2.SetCheck(FALSE);
		m_radio_device_type_3.SetCheck(FALSE);
		m_radio_device_type_1.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_device_type_1.SetCheck(FALSE);
		m_radio_device_type_3.SetCheck(FALSE);
		m_radio_device_type_2.SetCheck(TRUE);
		break;

	case 2:
		m_radio_device_type_1.SetCheck(FALSE);
		m_radio_device_type_2.SetCheck(FALSE);
		m_radio_device_type_3.SetCheck(TRUE);
		break;

	}
}


void CScreen_Basic::OnRadioDensityFeederModeUse1() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[0] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[0], (int)CTL_YES );
	OnBasic_DensityFeeder_Change1();
}

void CScreen_Basic::OnRadioDensityFeederModeNotuse1() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[0] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[0], (int)CTL_NO );
	OnBasic_DensityFeeder_Change1();
	
}

void CScreen_Basic::OnRadioDensityFeederModeUse2() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[1] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[1], (int)CTL_YES );
	OnBasic_DensityFeeder_Change2();
	
}

void CScreen_Basic::OnRadioDensityFeederModeNotuse2() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[1] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[1], (int)CTL_NO );
	OnBasic_DensityFeeder_Change2();
	
}

void CScreen_Basic::OnRadioDensityFeederModeUse3() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[2] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[2], (int)CTL_YES );
	OnBasic_DensityFeeder_Change3();
	
}

void CScreen_Basic::OnRadioDensityFeederModeNotuse3() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[2] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[2], (int)CTL_NO );
	OnBasic_DensityFeeder_Change3();
	
}

void CScreen_Basic::OnRadioDensityFeederModeUse4() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[3] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[3], (int)CTL_YES );
	OnBasic_DensityFeeder_Change4();
	
}

void CScreen_Basic::OnRadioDensityFeederModeNotuse4() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[3] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[3], (int)CTL_NO );
	OnBasic_DensityFeeder_Change4();
	
}

void CScreen_Basic::OnRadioDensityFeederModeUse5() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[4] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[4], (int)CTL_YES );
	OnBasic_DensityFeeder_Change5();
	
}

void CScreen_Basic::OnRadioDensityFeederModeNotuse5() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[4] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[4], (int)CTL_NO );
	OnBasic_DensityFeeder_Change5();
	
}

void CScreen_Basic::OnRadioDensityFeederModeUse6() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[5] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[5], (int)CTL_YES );
	OnBasic_DensityFeeder_Change6();
	
}

void CScreen_Basic::OnRadioDensityFeederModeNotuse6() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[5] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[5], (int)CTL_NO );
	OnBasic_DensityFeeder_Change6();
	
}

void CScreen_Basic::OnBasic_DensityFeeder_Change1()
{
	int DensityFeeder = g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[0] );	
	if (DensityFeeder < 0 || DensityFeeder > 1)
	{
		DensityFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[0], (int)0 );
	}
	
	switch (DensityFeeder)
	{
	case 0:
		m_radio_density_feeder_use.SetCheck(FALSE);
		m_radio_density_feeder_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_feeder_use.SetCheck(TRUE);
		m_radio_density_feeder_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_DensityFeeder_Change2()
{
	int DensityFeeder = g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[1] );	
	if (DensityFeeder < 0 || DensityFeeder > 1)
	{
		DensityFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[1], (int)0 );
	}
	
	switch (DensityFeeder)
	{
	case 0:
		m_radio_density_feeder_use1.SetCheck(FALSE);
		m_radio_density_feeder_notuse1.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_feeder_use1.SetCheck(TRUE);
		m_radio_density_feeder_notuse1.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_DensityFeeder_Change3()
{
	int DensityFeeder = g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[2] );	
	if (DensityFeeder < 0 || DensityFeeder > 1)
	{
		DensityFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[2], (int)0 );
	}
	
	switch (DensityFeeder)
	{
	case 0:
		m_radio_density_feeder_use2.SetCheck(FALSE);
		m_radio_density_feeder_notuse2.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_feeder_use2.SetCheck(TRUE);
		m_radio_density_feeder_notuse2.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_DensityFeeder_Change4()
{
	int DensityFeeder = g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[3] );	
	if (DensityFeeder < 0 || DensityFeeder > 1)
	{
		DensityFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[3], (int)0 );
	}
	
	switch (DensityFeeder)
	{
	case 0:
		m_radio_density_feeder_use3.SetCheck(FALSE);
		m_radio_density_feeder_notuse3.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_feeder_use3.SetCheck(TRUE);
		m_radio_density_feeder_notuse3.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_DensityFeeder_Change5()
{
	int DensityFeeder = g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[4] );	
	if (DensityFeeder < 0 || DensityFeeder > 1)
	{
		DensityFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[4], (int)0 );
	}
	
	switch (DensityFeeder)
	{
	case 0:
		m_radio_density_feeder_use4.SetCheck(FALSE);
		m_radio_density_feeder_notuse4.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_feeder_use4.SetCheck(TRUE);
		m_radio_density_feeder_notuse4.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_DensityFeeder_Change6()
{
	int DensityFeeder = g_dataMgr.GetBackupData( &st_basic.nDensityFeederUse[5] );	
	if (DensityFeeder < 0 || DensityFeeder > 1)
	{
		DensityFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nDensityFeederUse[5], (int)0 );
	}
	
	switch (DensityFeeder)
	{
	case 0:
		m_radio_density_feeder_use5.SetCheck(FALSE);
		m_radio_density_feeder_notuse5.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_feeder_use5.SetCheck(TRUE);
		m_radio_density_feeder_notuse5.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioBcrFeederModeUse1() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
	// TODO: Add your control notification handler code here
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] ) == CTL_YES &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] ) == CTL_NO)
// 		return;
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] ) == CTL_YES)
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_NO );
	OnBasic_BCRFeeder_Change1();
	
}

void CScreen_Basic::OnRadioBcrFeederModeNotuse1() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] ) == CTL_NO &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] ) == CTL_YES)
// 		return;
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] ) == CTL_NO)
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_YES );
	OnBasic_BCRFeeder_Change1();
	
}

void CScreen_Basic::OnRadioBcrFeederModeUse2() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] ) == CTL_YES )
		return;
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] ) == CTL_YES &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] ) == CTL_NO)
// 		return;
	
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_YES );
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_NO );
	OnBasic_BCRFeeder_Change2();
	
}

void CScreen_Basic::OnRadioBcrFeederModeNotuse2() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] ) == CTL_NO &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] ) == CTL_YES)
// 		return;
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] ) == CTL_NO)
		return;
	
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_NO );
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_NO );
	OnBasic_BCRFeeder_Change2();
	
}

void CScreen_Basic::OnRadioBcrFeederModeUse3() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] ) == CTL_YES &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] ) == CTL_NO)
// 		return;
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] ) == CTL_YES)
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_NO );
	OnBasic_BCRFeeder_Change3();
	
}

void CScreen_Basic::OnRadioBcrFeederModeNotuse3() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] ) == CTL_NO &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] ) == CTL_YES)
// 		return;
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] ) == CTL_NO)
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_YES );
	OnBasic_BCRFeeder_Change3();
	
}

void CScreen_Basic::OnRadioBcrFeederModeUse4() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] ) == CTL_YES)
		return;
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] ) == CTL_YES &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] ) == CTL_NO)
// 		return;
	
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_YES );
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_NO );
	OnBasic_BCRFeeder_Change4();
	
}

void CScreen_Basic::OnRadioBcrFeederModeNotuse4() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		m_strTempMSG = "진행중인 랏이 있습니다.";
		if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = "There is a Running LOT.";
		DoModal_Msg( m_strTempMSG );
		OnBasic_Device_Focus_Set();
		return;
	}
// 	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] ) == CTL_NO &&
// 		g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] ) == CTL_YES)
// 		return;
	if( g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] ) == CTL_NO)
		return;
	
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)CTL_NO );
	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)CTL_NO );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)CTL_YES );
// 	g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)CTL_YES );
	OnBasic_BCRFeeder_Change4();
	
}

void CScreen_Basic::OnBasic_BCRFeeder_Change1()
{
	int BCRFeeder = g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[0] );	
	if (BCRFeeder < 0 || BCRFeeder > 1)
	{
		BCRFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[0], (int)0 );
	}
	
	switch (BCRFeeder)
	{
	case 0:
		m_radio_bcr_feeder_use.SetCheck(FALSE);
		m_radio_bcr_feeder_notuse.SetCheck(TRUE);
// 		m_radio_bcr_feeder_use2.SetCheck(FALSE);
// 		m_radio_bcr_feeder_notuse2.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_bcr_feeder_use.SetCheck(TRUE);
		m_radio_bcr_feeder_notuse.SetCheck(FALSE);
// 		m_radio_bcr_feeder_use2.SetCheck(TRUE);
// 		m_radio_bcr_feeder_notuse2.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_BCRFeeder_Change2()
{
	int BCRFeeder = g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[1] );	
	if (BCRFeeder < 0 || BCRFeeder > 1)
	{
		BCRFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[1], (int)0 );
	}
	
	switch (BCRFeeder)
	{
	case 0:
		m_radio_bcr_feeder_use1.SetCheck(FALSE);
		m_radio_bcr_feeder_notuse1.SetCheck(TRUE);
// 		m_radio_bcr_feeder_use3.SetCheck(FALSE);
// 		m_radio_bcr_feeder_notuse3.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_bcr_feeder_use1.SetCheck(TRUE);
		m_radio_bcr_feeder_notuse1.SetCheck(FALSE);
// 		m_radio_bcr_feeder_use3.SetCheck(TRUE);
// 		m_radio_bcr_feeder_notuse3.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_BCRFeeder_Change3()
{
	int BCRFeeder = g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[2] );	
	if (BCRFeeder < 0 || BCRFeeder > 1)
	{
		BCRFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[2], (int)0 );
	}
	
	switch (BCRFeeder)
	{
	case 0:
		m_radio_bcr_feeder_use2.SetCheck(FALSE);
		m_radio_bcr_feeder_notuse2.SetCheck(TRUE);
// 		m_radio_bcr_feeder_use.SetCheck(FALSE);
// 		m_radio_bcr_feeder_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_bcr_feeder_use2.SetCheck(TRUE);
		m_radio_bcr_feeder_notuse2.SetCheck(FALSE);
// 		m_radio_bcr_feeder_use.SetCheck(TRUE);
// 		m_radio_bcr_feeder_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_BCRFeeder_Change4()
{
	int BCRFeeder = g_dataMgr.GetBackupData( &st_basic.nBcrFeederUse[3] );	
	if (BCRFeeder < 0 || BCRFeeder > 1)
	{
		BCRFeeder = 0;
		g_dataMgr.SetDataTemp( &st_basic.nBcrFeederUse[3], (int)0 );
	}
	
	switch (BCRFeeder)
	{
	case 0:
		m_radio_bcr_feeder_use3.SetCheck(FALSE);
		m_radio_bcr_feeder_notuse3.SetCheck(TRUE);
// 		m_radio_bcr_feeder_use1.SetCheck(FALSE);
// 		m_radio_bcr_feeder_notuse1.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_bcr_feeder_use3.SetCheck(TRUE);
		m_radio_bcr_feeder_notuse3.SetCheck(FALSE);
// 		m_radio_bcr_feeder_use1.SetCheck(TRUE);
// 		m_radio_bcr_feeder_notuse1.SetCheck(FALSE);
		break;
	}
}

////2014.1104
void CScreen_Basic::OnRadioVisionUse() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[1] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[1], (int)CTL_YES );
	OnBasic_Vision_Change();
	
}

void CScreen_Basic::OnRadioVisionNotuse() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[1] ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[1], (int)CTL_NO );
	OnBasic_Vision_Change();
	
}

void CScreen_Basic::OnBasic_Vision_Change()
{
	int Vision = g_dataMgr.GetBackupData( &st_basic.nVisionUse[1] );	
	if (Vision < 0 || Vision > 1)
	{
		Vision = 0;
		g_dataMgr.SetDataTemp( &st_basic.nVisionUse[1], (int)0 );
	}
	
	switch (Vision)
	{
	case 0:
		m_radio_vision_use.SetCheck(FALSE);
		m_radio_vision_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_vision_use.SetCheck(TRUE);
		m_radio_vision_notuse.SetCheck(FALSE);
		break;
	}
}
////


////2014,1125 ysj
void CScreen_Basic::OnRadioDensityLabelSelect1() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nLabelSelect ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nLabelSelect, (int)CTL_YES );
	OnBasic_Label_Select();	
}

void CScreen_Basic::OnRadioDensityLabelSelect2() 
{
	// TODO: Add your control notification handler code here
	if( g_dataMgr.GetBackupData( &st_basic.nLabelSelect ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nLabelSelect, (int)CTL_NO );
	OnBasic_Label_Select();	
	
}

void CScreen_Basic::OnBasic_Label_Select()
{
	int Label = g_dataMgr.GetBackupData( &st_basic.nLabelSelect );	
	if (Label < 0 || Label > 1)
	{
		Label = 0;
		g_dataMgr.SetDataTemp( &st_basic.nLabelSelect, (int)0 );
	}
	
	switch (Label)
	{
	case 0:
		m_radio_label_select_1.SetCheck(FALSE);
		m_radio_label_select_2.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_label_select_1.SetCheck(TRUE);
		m_radio_label_select_2.SetCheck(FALSE);
		break;
	}
}
////

void CScreen_Basic::OnRadioDensityVisionUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[0] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[0], (int)CTL_YES );
	OnBasic_Density_Vision_Change();
	
}

void CScreen_Basic::OnRadioDensityVisionNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[0] ) == CTL_NO )//2014,1217 CTL_YES -> CTL_NO
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[0], (int)CTL_NO );//2014,1217 CTL_YES -> CTL_NO
	OnBasic_Density_Vision_Change();
	
}

void CScreen_Basic::OnRadioTopVisionUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[2] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[2], (int)CTL_YES );
	OnBasic_Top_Vision_Change();
	
}

void CScreen_Basic::OnRadioTopVisionNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[2] ) == CTL_NO )//2014,1217 CTL_YES -> CTL_NO
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[2], (int)CTL_NO );//2014,1217 CTL_YES -> CTL_NO
	OnBasic_Top_Vision_Change();
	
	
}

void CScreen_Basic::OnRadioBtmVisionUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[3] ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[3], (int)CTL_YES );
	OnBasic_Btm_Vision_Change();
	
}

void CScreen_Basic::OnRadioBtmVisionNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nVisionUse[3] ) == CTL_NO )//2014,1217 CTL_YES -> CTL_NO
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nVisionUse[3], (int)CTL_NO );//2014,1217 CTL_YES -> CTL_NO
	OnBasic_Btm_Vision_Change();
	
}

void CScreen_Basic::OnBasic_Density_Vision_Change()
{
	int Vision = g_dataMgr.GetBackupData( &st_basic.nVisionUse[0] );	
	if (Vision < 0 || Vision > 1)
	{
		Vision = 0;
		g_dataMgr.SetDataTemp( &st_basic.nVisionUse[0], (int)0 );
	}
	
	switch (Vision)
	{
	case 0:
		m_radio_density_vision_use.SetCheck(FALSE);
		m_radio_density_vision_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_density_vision_use.SetCheck(TRUE);
		m_radio_density_vision_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_Top_Vision_Change()
{
	int Vision = g_dataMgr.GetBackupData( &st_basic.nVisionUse[2] );	
	if (Vision < 0 || Vision > 1)
	{
		Vision = 0;
		g_dataMgr.SetDataTemp( &st_basic.nVisionUse[2], (int)0 );
	}
	
	switch (Vision)
	{
	case 0:
		m_radio_top_vision_use.SetCheck(FALSE);
		m_radio_top_vision_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_top_vision_use.SetCheck(TRUE);
		m_radio_top_vision_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasic_Btm_Vision_Change()
{
	int Vision = g_dataMgr.GetBackupData( &st_basic.nVisionUse[3] );	
	if (Vision < 0 || Vision > 1)
	{
		Vision = 0;
		g_dataMgr.SetDataTemp( &st_basic.nVisionUse[3], (int)0 );
	}
	
	switch (Vision)
	{
	case 0:
		m_radio_btm_vision_use.SetCheck(FALSE);
		m_radio_btm_vision_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_btm_vision_use.SetCheck(TRUE);
		m_radio_btm_vision_notuse.SetCheck(FALSE);
		break;
	}
}


void CScreen_Basic::OnRadioServerUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_ECServerUse ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_ECServerUse, (int)CTL_YES );
	OnBasic_ECServer_Change();
}

void CScreen_Basic::OnRadioServerNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_ECServerUse ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_ECServerUse, (int)CTL_NO );
	OnBasic_ECServer_Change();
	
}

void CScreen_Basic::OnBasic_ECServer_Change()
{
	int ec = g_dataMgr.GetBackupData( &st_basic.mn_ECServerUse );	
	if (ec < 0 || ec > 1)
	{
		ec = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_ECServerUse, (int)0 );
	}
	
	switch (ec)
	{
	case 0:
		m_radio_ecsvr_use.SetCheck(FALSE);
		m_radio_ecsvr_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_ecsvr_use.SetCheck(TRUE);
		m_radio_ecsvr_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioLeftBcrUse() 
{
// 	if (mn_leftbcr[1] == CTL_YES)	return;
// 	
// 	mn_leftbcr[1] = CTL_YES;

	if( g_dataMgr.GetBackupData( &st_basic.mn_leftbcr ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_leftbcr, (int)CTL_YES );

	OnBasic_LeftBcr_Change();
	
}

void CScreen_Basic::OnRadioLeftBcrNotuse() 
{
// 	if (mn_leftbcr[1] == CTL_NO)	return;
// 	
// 	mn_leftbcr[1] = CTL_NO;
	if( g_dataMgr.GetBackupData( &st_basic.mn_leftbcr ) == CTL_NO )//2014,1217 CTL_YES -> CTL_NO
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_leftbcr, (int)CTL_NO );//2014,1217 CTL_YES -> CTL_NO
	OnBasic_LeftBcr_Change();
	
}

void CScreen_Basic::OnBasic_LeftBcr_Change()
{
	int leftbcr = g_dataMgr.GetBackupData( &st_basic.mn_leftbcr );	
	if (leftbcr < 0 || leftbcr > 1)
	{
		leftbcr = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_leftbcr, (int)0 );
	}
		
	switch (leftbcr)
	{
	case 0:
		m_radio_left_bcr_use.SetCheck(FALSE);
		m_radio_left_bcr_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_left_bcr_use.SetCheck(TRUE);
		m_radio_left_bcr_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioRightBcrUse() 
{
// 	if (mn_rightbcr[1] == CTL_YES)	return;
// 	
// 	mn_rightbcr[1] = CTL_YES;

	if( g_dataMgr.GetBackupData( &st_basic.mn_rightbcr ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_rightbcr, (int)CTL_YES );

	OnBasic_RightBcr_Change();
	
}

void CScreen_Basic::OnRadioRightBcrNotuse() 
{
// 	if (mn_rightbcr[1] == CTL_NO)	return;
// 	
// 	mn_rightbcr[1] = CTL_NO;
	if( g_dataMgr.GetBackupData( &st_basic.mn_rightbcr ) == CTL_NO )//2014,1217 CTL_YES -> CTL_NO
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_rightbcr, (int)CTL_NO );//2014,1217 CTL_YES -> CTL_NO
	OnBasic_RightBcr_Change();
	
}

void CScreen_Basic::OnBasic_RightBcr_Change()
{
	
	int rightbcr = g_dataMgr.GetBackupData( &st_basic.mn_rightbcr );	
	if (rightbcr < 0 || rightbcr > 1)
	{
		rightbcr = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_rightbcr, (int)0 );
	}

	switch (rightbcr)
	{
	case 0:
		m_radio_right_bcr_use.SetCheck(FALSE);
		m_radio_right_bcr_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_right_bcr_use.SetCheck(TRUE);
		m_radio_right_bcr_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioUseOutconv() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_outconv ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_outconv, (int)CTL_YES );
	
	OnBasic_OutConv_Change();
	
}

void CScreen_Basic::OnRadioNotuseOutconv() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_outconv ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_outconv, (int)CTL_NO );
	
	OnBasic_OutConv_Change();
	
}

void CScreen_Basic::OnBasic_OutConv_Change()
{
	int outconv = g_dataMgr.GetBackupData( &st_basic.mn_outconv );	
	if (outconv < 0 || outconv > 1)
	{
		outconv = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_outconv, (int)0 );
	}
	
	switch (outconv)
	{
	case 0:
		m_radio_outconv_use.SetCheck(FALSE);
		m_radio_outconv_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_outconv_use.SetCheck(TRUE);
		m_radio_outconv_notuse.SetCheck(FALSE);
		break;
	}
	if(outconv == CTL_YES)
	{
		m_group_out_tbox.ShowWindow(FALSE);
		m_radio_use_rejectbox.ShowWindow(FALSE);
		m_radio_use_convbox.ShowWindow(FALSE);
	}
	else
	{
		m_group_out_tbox.ShowWindow(TRUE);
		m_radio_use_rejectbox.ShowWindow(TRUE);
		m_radio_use_convbox.ShowWindow(TRUE);
	}
	
}

void CScreen_Basic::OnRadioUseReject() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_outtbox ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_outtbox, (int)CTL_NO );
	OnBasic_OutTBox_Change();
	
}

void CScreen_Basic::OnRadioUseConv() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_outtbox ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_outtbox, (int)CTL_YES );
	OnBasic_OutTBox_Change();
	
}


void CScreen_Basic::OnBasic_OutTBox_Change()
{
	int outtbox = g_dataMgr.GetBackupData( &st_basic.mn_outtbox );	
	if (outtbox < 0 || outtbox > 1)
	{
		outtbox = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_outtbox, (int)0 );
	}
	
	switch (outtbox)
	{
	case 0:
		m_radio_use_convbox.SetCheck(FALSE);
		m_radio_use_rejectbox.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_use_convbox.SetCheck(TRUE);
		m_radio_use_rejectbox.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBasicDensity_CapaSet()
{
	//2014,1227		
	CString strVal="";
	CString strValue="";

// 	CString str1,str2,str3;
	
// 	((CEdit*)GetDlgItem(IDC_COMBO_LABEL1_CAPA))->GetWindowText(st_basic.n_str_capa1label);
// 	((CEdit*)GetDlgItem(IDC_COMBO_LABEL2_CAPA))->GetWindowText(st_basic.n_str_capa2label);
// 	((CEdit*)GetDlgItem(IDC_COMBO_LABEL3_CAPA))->GetWindowText(st_basic.n_str_capa3label);

	m_cbolabel1capa.GetWindowText(st_basic.n_str_capa1label);
	m_cbolabel2capa.GetWindowText(st_basic.n_str_capa2label);
	m_cbolabel3capa.GetWindowText(st_basic.n_str_capa3label);
	st_handler.m_strDensityCapa[0] = st_basic.n_str_capa1label;
	st_handler.m_strDensityCapa[3] = st_handler.m_strDensityCapa[0];

	st_handler.m_strDensityCapa[1] = st_basic.n_str_capa2label;
	st_handler.m_strDensityCapa[4] = st_handler.m_strDensityCapa[1];

	st_handler.m_strDensityCapa[2] = st_basic.n_str_capa3label;
	st_handler.m_strDensityCapa[5] = st_handler.m_strDensityCapa[2];
	
	for( int j=0; j<6; j++ )
	{
		strValue.Format("CAPADENSITY_%02d", j );
		strVal.Format("%s",st_handler.m_strDensityCapa[j]);
		AMTRegistry::RegWriteString( REG_VAL_CAPADENSITY, strValue, strVal );
		
	}
}

void CScreen_Basic::LoadLabelData()
{
	Func.LoadRegLabelCapaData();
	for( int i=0; i< st_handler.m_nTotal; i++ )
	{
		if(st_handler.m_strLabelCapa[i] != "")
		{
			m_cbolabel1capa.AddString(st_handler.m_strLabelCapa[i]);
			m_cbolabel2capa.AddString(st_handler.m_strLabelCapa[i]);
			m_cbolabel3capa.AddString(st_handler.m_strLabelCapa[i]);
		}
	}
	m_cbolabel1capa.SetWindowText(st_basic.n_str_capa1label);
	m_cbolabel2capa.SetWindowText(st_basic.n_str_capa2label);
	m_cbolabel3capa.SetWindowText(st_basic.n_str_capa3label);
}

void CScreen_Basic::OnSelchangeComboLabel1Capa() 
{
	CString sText="";
	m_cbolabel1capa.GetWindowText(sText);
	g_dataMgr.SetDataTemp( &st_basic.n_str_capa1label, sText );	
	
}

void CScreen_Basic::OnSelchangeComboLabel2Capa() 
{
	CString sText="";
	m_cbolabel2capa.GetWindowText(sText);
	
	g_dataMgr.SetDataTemp( &st_basic.n_str_capa2label, sText );	
	
}

void CScreen_Basic::OnSelchangeComboLabel3Capa() 
{
	UpdateData(FALSE);
	CString sText;
	m_cbolabel3capa.GetWindowText(sText);
	g_dataMgr.SetDataTemp( &st_basic.n_str_capa3label, sText );	
	
}

void CScreen_Basic::OnDgtBcrLimitTime() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	int mn_minute;

	mn_minute = m_dgt_bcr_limit_time.GetValue();

	mstr_temp = _T("Barcode Limt Time Out 대기 시간 (분)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Barcode Limt Time Out (min)");

	CRect r;

	m_dgt_bcr_limit_time.GetWindowRect(&r);

	mstr_temp = KeyPad.GetNumEditString_I(0, 10, mn_minute, mstr_temp, &r);

	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.mn_limit_bcr_time, iTemp );
	m_dgt_bcr_limit_time.SetValue(iTemp);
}

void CScreen_Basic::OnRadioUseAutoVisionModel() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_auto_vision_mdl ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_auto_vision_mdl, (int)CTL_YES );
	OnBasic_auto_vision_model();
	
}

void CScreen_Basic::OnRadioNotuseAutoVisionModel() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_auto_vision_mdl ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_auto_vision_mdl, (int)CTL_NO );
	OnBasic_Top_Vision_SkipChange();
	
}

void CScreen_Basic::OnBasic_auto_vision_model()
{
	int Vision = g_dataMgr.GetBackupData( &st_basic.mn_auto_vision_mdl );	
	if (Vision < 0 || Vision > 1)
	{
		Vision = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_auto_vision_mdl, (int)0 );
	}
	
	switch (Vision)
	{
	case 0:
		m_radio_use_auto_vision_mdl.SetCheck(FALSE);
		m_radio_notuse_auto_vision_mdl.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_use_auto_vision_mdl.SetCheck(TRUE);
		m_radio_notuse_auto_vision_mdl.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnBtnVisDensity() 
{
	st_var.n_use_vis_density = CTL_YES;	
}

void CScreen_Basic::OnBtnVisTop() 
{
	st_var.n_use_vis_top = CTL_NO;	
}

void CScreen_Basic::OnRadioTopVisionSkip() 
{
// 	int nTopVisionSkip;
// 	int nTopVisionCnt;	

	if( g_dataMgr.GetBackupData( &st_basic.nTopVisionSkip ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nTopVisionSkip, (int)CTL_YES );
	OnBasic_Top_Vision_SkipChange();
	
}

void CScreen_Basic::OnRadioTopVisionNotSkip() 
{
	if( g_dataMgr.GetBackupData( &st_basic.nTopVisionSkip ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.nTopVisionSkip, (int)CTL_NO );
	OnBasic_Top_Vision_SkipChange();
	
}

void CScreen_Basic::OnBasic_Top_Vision_SkipChange()
{
	int Vision = g_dataMgr.GetBackupData( &st_basic.nTopVisionSkip );	
	if (Vision < 0 || Vision > 1)
	{
		Vision = 0;
		g_dataMgr.SetDataTemp( &st_basic.nTopVisionSkip, (int)0 );
	}
	
	switch (Vision)
	{
	case 0:
		m_radio_top_vision_skip.SetCheck(FALSE);
		m_radio_top_vision_notskip.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_top_vision_skip.SetCheck(TRUE);
		m_radio_top_vision_notskip.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnDgtTopVisionSkipCnt() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_top_vision_skip_cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 99));
	
	st_msg.mstr_keypad_msg = _T("Top Vision Skip Count");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("Top Vision Skip Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 99, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.nTopVisionSkipCnt, iTemp );
	m_dgt_top_vision_skip_cnt.SetValue( iTemp );		
}

void CScreen_Basic::OnDgtRejectOffset() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	// 	char chr_buf[20] ;
	int mn_minute;
	
	mn_minute = m_dgt_reject_offfset.GetValue();
	//mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 999));
	
	st_msg.mstr_keypad_msg = _T("Reject Offset(mm)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("Reject Offset(mm)");
	
	CRect r;
	
	m_dgt_reject_offfset.GetWindowRect(&r);
	
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 999, /*atoi(st_msg.mstr_keypad_val)*/mn_minute, st_msg.mstr_keypad_msg);
	
	int iTemp = atoi(mstr_temp);
	g_dataMgr.SetDataTemp( &st_basic.nRejectOffset, iTemp );
	m_dgt_reject_offfset.SetValue( iTemp );	
}

void CScreen_Basic::OnRadioReworkCcsUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_rework_ccs_mode ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_rework_ccs_mode, (int)CTL_YES );
	OnBasic_Rework_CCS_Change();	
	
}

void CScreen_Basic::OnRadioReworkCcsNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_rework_ccs_mode ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_rework_ccs_mode, (int)CTL_NO );
	OnBasic_Rework_CCS_Change();	
	
}

void CScreen_Basic::OnBasic_Rework_CCS_Change()
{
	int nRework = g_dataMgr.GetBackupData( &st_basic.mn_rework_ccs_mode );	
	if (nRework < 0 || nRework > 1)
	{
		nRework = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_rework_ccs_mode, (int)0 );
	}
	
	switch (nRework)
	{
	case 0:
		m_radio_rework_ccs_use.SetCheck(FALSE);
		m_radio_rework_ccs_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_rework_ccs_use.SetCheck(TRUE);
		m_radio_rework_ccs_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnRadioVisDensitySkipUse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_vision_density_skip ) == CTL_YES )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_vision_density_skip, (int)CTL_YES );
	OnBasic_Vision_Density_Skip_Change();	
	
}

void CScreen_Basic::OnRadioVisDensitySkipNotuse() 
{
	if( g_dataMgr.GetBackupData( &st_basic.mn_vision_density_skip ) == CTL_NO )
		return;
	
	g_dataMgr.SetDataTemp( &st_basic.mn_vision_density_skip, (int)CTL_NO );
	OnBasic_Vision_Density_Skip_Change();	
	
}

void CScreen_Basic::OnBasic_Vision_Density_Skip_Change()
{
	int nVisSkip = g_dataMgr.GetBackupData( &st_basic.mn_vision_density_skip );	
	if (nVisSkip < 0 || nVisSkip > 1)
	{
		nVisSkip = 0;
		g_dataMgr.SetDataTemp( &st_basic.mn_vision_density_skip, (int)0 );
	}
	
	switch (nVisSkip)
	{
	case 0:
		m_radio_vis_density_skip_use.SetCheck(FALSE);
		m_radio_vis_density_skip_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_vis_density_skip_use.SetCheck(TRUE);
		m_radio_vis_density_skip_notuse.SetCheck(FALSE);
		break;
	}
}
