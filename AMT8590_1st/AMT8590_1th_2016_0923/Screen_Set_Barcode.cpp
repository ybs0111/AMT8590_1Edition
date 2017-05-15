// Screen_Set_Barcode.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Set_Barcode.h"

#include "io.h"  // 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요
#include "FileDialogST.h"
// ******************************************************************************
// 대화 상자 클래스 추가                                                         
// ******************************************************************************
//#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
#include "Dialog_Keyboard.h"

#include "Ctlbd_Variable.h"
#include "Variable.h"
#include "ZebraPrint.h"
#include "AMTRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Barcode dialog
IMPLEMENT_DYNCREATE(CScreen_Set_Barcode, CFormView)

CScreen_Set_Barcode::CScreen_Set_Barcode()
	: CFormView(CScreen_Set_Barcode::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Set_Barcode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Set_Barcode::~CScreen_Set_Barcode()
{
}

void CScreen_Set_Barcode::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Set_Barcode)
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_RADIO_PRT_MODE1, m_radio_prt_mode1);
	DDX_Control(pDX, IDC_RADIO_PRT_MODE2, m_radio_prt_mode2);
	DDX_Control(pDX, IDC_RADIO_PRT_MODE3, m_radio_prt_mode3);
	DDX_Control(pDX, IDC_RADIO_PRT_MODE4, m_radio_prt_mode4);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_USE1, m_radio_prt_complete_use1);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_USE2, m_radio_prt_complete_use2);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_USE3, m_radio_prt_complete_use3);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_USE4, m_radio_prt_complete_use4);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_NOTUSE1, m_radio_prt_complete_notuse1);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_NOTUSE2, m_radio_prt_complete_notuse2);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_NOTUSE3, m_radio_prt_complete_notuse3);
	DDX_Control(pDX, IDC_RADIO_PRT_COMPLETE_NOTUSE4, m_radio_prt_complete_notuse4);
	DDX_Control(pDX, IDC_BTN_PRT_APPLY1, m_btn_prt_apply1);
	DDX_Control(pDX, IDC_BTN_PRT_APPLY2, m_btn_prt_apply2);
	DDX_Control(pDX, IDC_BTN_PRT_APPLY3, m_btn_prt_apply3);
	DDX_Control(pDX, IDC_BTN_PRT_APPLY4, m_btn_prt_apply4);
	DDX_Control(pDX, IDC_BTN_PRT_APPLY5, m_btn_prt_apply5);
	DDX_Control(pDX, IDC_GROUP_PRINTER_NO1, m_group_prt_no1);
	DDX_Control(pDX, IDC_GROUP_PRINTER_NO2, m_group_prt_no2);
	DDX_Control(pDX, IDC_GROUP_PRINTER_NO3, m_group_prt_no3);
	DDX_Control(pDX, IDC_GROUP_PRINTER_NO4, m_group_prt_no4);
	DDX_Control(pDX, IDC_GROUP_PRT_MODE1, m_group_prt_mode1);
	DDX_Control(pDX, IDC_GROUP_PRT_MODE2, m_group_prt_mode2);
	DDX_Control(pDX, IDC_GROUP_PRT_MODE3, m_group_prt_mode3);
	DDX_Control(pDX, IDC_GROUP_PRT_MODE4, m_group_prt_mode4);
	DDX_Control(pDX, IDC_GROUP_PRT_COMPLETE1, m_group_prt_complete1);
	DDX_Control(pDX, IDC_GROUP_PRT_COMPLETE2, m_group_prt_complete2);
	DDX_Control(pDX, IDC_GROUP_PRT_COMPLETE3, m_group_prt_complete3);
	DDX_Control(pDX, IDC_GROUP_PRT_COMPLETE4, m_group_prt_complete4);
	DDX_Control(pDX, IDC_GROUP_PRT_POS1, m_group_prt_pos1);
	DDX_Control(pDX, IDC_GROUP_PRT_POS2, m_group_prt_pos2);
	DDX_Control(pDX, IDC_GROUP_PRT_POS3, m_group_prt_pos3);
	DDX_Control(pDX, IDC_GROUP_PRT_POS4, m_group_prt_pos4);
	DDX_Control(pDX, IDC_MSG_LABELSET_SD1, m_msg_labelset_sd1);
	DDX_Control(pDX, IDC_MSG_LABELSET_SD2, m_msg_labelset_sd2);
	DDX_Control(pDX, IDC_MSG_LABELSET_SD3, m_msg_labelset_sd3);
	DDX_Control(pDX, IDC_MSG_LABELSET_SD4, m_msg_labelset_sd4);
	DDX_Control(pDX, IDC_MSG_LABELSET_LT1, m_msg_labelset_lt1);
	DDX_Control(pDX, IDC_MSG_LABELSET_LT2, m_msg_labelset_lt2);
	DDX_Control(pDX, IDC_MSG_LABELSET_LT3, m_msg_labelset_lt3);
	DDX_Control(pDX, IDC_MSG_LABELSET_LT4, m_msg_labelset_lt4);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHX1, m_msg_labelset_lhx1);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHX2, m_msg_labelset_lhx2);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHX3, m_msg_labelset_lhx3);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHX4, m_msg_labelset_lhx4);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHY1, m_msg_labelset_lhy1);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHY2, m_msg_labelset_lhy2);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHY3, m_msg_labelset_lhy3);
	DDX_Control(pDX, IDC_MSG_LABELSET_LHY4, m_msg_labelset_lhy4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Set_Barcode, CFormView)
	//{{AFX_MSG_MAP(CScreen_Set_Barcode)
		ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PRT_STATUS1, OnBtnPrtStatus1)
	ON_BN_CLICKED(IDC_BTN_PRT_STATUS2, OnBtnPrtStatus2)
	ON_BN_CLICKED(IDC_BTN_PRT_STATUS3, OnBtnPrtStatus3)
	ON_BN_CLICKED(IDC_BTN_PRT_STATUS4, OnBtnPrtStatus4)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE1, OnRadioPrtMode1)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_USE1, OnRadioPrtCompleteUse1)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_NOTUSE1, OnRadioPrtCompleteNotuse1)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE3, OnRadioPrtMode3)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_USE3, OnRadioPrtCompleteUse3)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_NOTUSE3, OnRadioPrtCompleteNotuse3)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE2, OnRadioPrtMode2)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_USE2, OnRadioPrtCompleteUse2)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_NOTUSE2, OnRadioPrtCompleteNotuse2)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE4, OnRadioPrtMode4)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_USE4, OnRadioPrtCompleteUse4)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMPLETE_NOTUSE4, OnRadioPrtCompleteNotuse4)
	ON_BN_CLICKED(IDC_DGT_LABELSET_SD1, OnDgtLabelsetSd1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LT1, OnDgtLabelsetLt1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHX1, OnDgtLabelsetLhx1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHY1, OnDgtLabelsetLhy1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_SD3, OnDgtLabelsetSd3)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LT3, OnDgtLabelsetLt3)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHX3, OnDgtLabelsetLhx3)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHY3, OnDgtLabelsetLhy3)
	ON_BN_CLICKED(IDC_DGT_LABELSET_SD2, OnDgtLabelsetSd2)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LT2, OnDgtLabelsetLt2)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHX2, OnDgtLabelsetLhx2)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHY2, OnDgtLabelsetLhy2)
	ON_BN_CLICKED(IDC_DGT_LABELSET_SD4, OnDgtLabelsetSd4)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LT4, OnDgtLabelsetLt4)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHX4, OnDgtLabelsetLhx4)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LHY4, OnDgtLabelsetLhy4)
	ON_BN_CLICKED(IDC_BTN_PRT_APPLY1, OnBtnPrtApply1)
	ON_BN_CLICKED(IDC_BTN_PRT_APPLY2, OnBtnPrtApply2)
	ON_BN_CLICKED(IDC_BTN_PRT_APPLY3, OnBtnPrtApply3)
	ON_BN_CLICKED(IDC_BTN_PRT_APPLY4, OnBtnPrtApply4)
	ON_BN_CLICKED(IDC_BTN_PRT_APPLY5, OnBtnPrtApply5)
	ON_BN_CLICKED(IDC_BTN_MODEL_DENSITY, OnBtnModelDensity)
	ON_BN_CLICKED(IDC_BTN_MODEL_BARCODE, OnBtnModelBarcode)
	ON_BN_CLICKED(IDC_BTN_MODEL_DENSITY_POS, OnBtnModelDensityPos)
	ON_BN_CLICKED(IDC_BTN_MODEL_BARCODE_POS, OnBtnModelBarcodePos)
	ON_BN_CLICKED(IDC_BTN_DATA_DENSITY, OnBtnDataDensity)
	ON_BN_CLICKED(IDC_BTN_DATA_BARCODE, OnBtnDataBarcode)
	ON_BN_CLICKED(IDC_BTN_DATA_DENSITY_POS, OnBtnDataDensityPos)
	ON_BN_CLICKED(IDC_BTN_DATA_BARCODE_POS, OnBtnDataBarcodePos)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PRT_SD1, OnBtnPrtSd1)
	ON_BN_CLICKED(IDC_BTN_PRT_SD2, OnBtnPrtSd2)
	ON_BN_CLICKED(IDC_BTN_PRT_SD3, OnBtnPrtSd3)
	ON_BN_CLICKED(IDC_BTN_PRT_SD4, OnBtnPrtSd4)
	ON_BN_CLICKED(IDC_BTN_PRT_LT1, OnBtnPrtLt1)
	ON_BN_CLICKED(IDC_BTN_PRT_LT2, OnBtnPrtLt2)
	ON_BN_CLICKED(IDC_BTN_PRT_LT3, OnBtnPrtLt3)
	ON_BN_CLICKED(IDC_BTN_PRT_LT4, OnBtnPrtLt4)
	ON_BN_CLICKED(IDC_BTN_PRT_LHX1, OnBtnPrtLhx1)
	ON_BN_CLICKED(IDC_BTN_PRT_LHX2, OnBtnPrtLhx2)
	ON_BN_CLICKED(IDC_BTN_PRT_LHX3, OnBtnPrtLhx3)
	ON_BN_CLICKED(IDC_BTN_PRT_LHX4, OnBtnPrtLhx4)
	ON_BN_CLICKED(IDC_BTN_PRT_LHY1, OnBtnPrtLhy1)
	ON_BN_CLICKED(IDC_BTN_PRT_LHY2, OnBtnPrtLhy2)
	ON_BN_CLICKED(IDC_BTN_PRT_LHY3, OnBtnPrtLhy3)
	ON_BN_CLICKED(IDC_BTN_PRT_LHY4, OnBtnPrtLhy4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Barcode diagnostics
#ifdef _DEBUG
void CScreen_Set_Barcode::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Set_Barcode::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance message ICTs

void CScreen_Set_Barcode::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	

	OnBarcode_GroupBox_Set();
	OnBarcode_Label_Set();
	OnBarcode_Data_Display();
	OnBarcode_Dgt_Set();
	OnBarcode_Button_Set();

	
	st_handler.cwnd_barcode = this;  
}

void CScreen_Set_Barcode::OnDestroy() 
{
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	st_handler.cwnd_barcode = NULL;	// Barcode 화면에 대한 핸들 정보 저장 변수 초기화 
	
	CFormView::OnDestroy();	
}


void CScreen_Set_Barcode::OnBarcode_GroupBox_Set()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");

	m_group_prt_no1.SetFont(main_font);
	m_group_prt_no1.SetCatptionTextColor(BLUE_C);
	m_group_prt_no1.SetBorderColor(BLUE_C);
	m_group_prt_no1.SetFontBold(TRUE);

	m_group_prt_no2.SetFont(main_font);
	m_group_prt_no2.SetCatptionTextColor(BLUE_C);
	m_group_prt_no2.SetBorderColor(BLUE_C);
	m_group_prt_no2.SetFontBold(TRUE);

	m_group_prt_no3.SetFont(main_font);
	m_group_prt_no3.SetCatptionTextColor(BLUE_C);
	m_group_prt_no3.SetBorderColor(BLUE_C);
	m_group_prt_no3.SetFontBold(TRUE);

	m_group_prt_no4.SetFont(main_font);
	m_group_prt_no4.SetCatptionTextColor(BLUE_C);
	m_group_prt_no4.SetBorderColor(BLUE_C);
	m_group_prt_no4.SetFontBold(TRUE);
	
	m_group_prt_mode1.SetFont(main_font);
	m_group_prt_mode1.SetCatptionTextColor(BLUE_C);
	m_group_prt_mode1.SetBorderColor(BLUE_C);
	m_group_prt_mode1.SetFontBold(TRUE);

	m_group_prt_mode2.SetFont(main_font);
	m_group_prt_mode2.SetCatptionTextColor(BLUE_C);
	m_group_prt_mode2.SetBorderColor(BLUE_C);
	m_group_prt_mode2.SetFontBold(TRUE);

	m_group_prt_mode3.SetFont(main_font);
	m_group_prt_mode3.SetCatptionTextColor(BLUE_C);
	m_group_prt_mode3.SetBorderColor(BLUE_C);
	m_group_prt_mode3.SetFontBold(TRUE);

	m_group_prt_mode4.SetFont(main_font);
	m_group_prt_mode4.SetCatptionTextColor(BLUE_C);
	m_group_prt_mode4.SetBorderColor(BLUE_C);
	m_group_prt_mode4.SetFontBold(TRUE);

	m_group_prt_complete1.SetFont(main_font);
	m_group_prt_complete1.SetCatptionTextColor(BLUE_C);
	m_group_prt_complete1.SetBorderColor(BLUE_C);
	m_group_prt_complete1.SetFontBold(TRUE);

	m_group_prt_complete2.SetFont(main_font);
	m_group_prt_complete2.SetCatptionTextColor(BLUE_C);
	m_group_prt_complete2.SetBorderColor(BLUE_C);
	m_group_prt_complete2.SetFontBold(TRUE);

	m_group_prt_complete3.SetFont(main_font);
	m_group_prt_complete3.SetCatptionTextColor(BLUE_C);
	m_group_prt_complete3.SetBorderColor(BLUE_C);
	m_group_prt_complete3.SetFontBold(TRUE);

	m_group_prt_complete4.SetFont(main_font);
	m_group_prt_complete4.SetCatptionTextColor(BLUE_C);
	m_group_prt_complete4.SetBorderColor(BLUE_C);
	m_group_prt_complete4.SetFontBold(TRUE);
	
	m_group_prt_pos1.SetFont(main_font);
	m_group_prt_pos1.SetCatptionTextColor(BLUE_C);
	m_group_prt_pos1.SetBorderColor(BLUE_C);
	m_group_prt_pos1.SetFontBold(TRUE);
	
	m_group_prt_pos2.SetFont(main_font);
	m_group_prt_pos2.SetCatptionTextColor(BLUE_C);
	m_group_prt_pos2.SetBorderColor(BLUE_C);
	m_group_prt_pos2.SetFontBold(TRUE);

	m_group_prt_pos3.SetFont(main_font);
	m_group_prt_pos3.SetCatptionTextColor(BLUE_C);
	m_group_prt_pos3.SetBorderColor(BLUE_C);
	m_group_prt_pos3.SetFontBold(TRUE);

	m_group_prt_pos4.SetFont(main_font);
	m_group_prt_pos4.SetCatptionTextColor(BLUE_C);
	m_group_prt_pos4.SetBorderColor(BLUE_C);
	m_group_prt_pos4.SetFontBold(TRUE);

}

void CScreen_Set_Barcode::OnBarcode_Label_Set()
{
	m_msg_labelset_sd1.SetFont(m_p_font);
 	m_msg_labelset_sd1.SetWindowText(_T("Set Darkness (SD)"));
	m_msg_labelset_sd1.SetCenterText();
	m_msg_labelset_sd1.SetColor(YELLOW_L);
	m_msg_labelset_sd1.SetGradientColor(YELLOW_L);
	m_msg_labelset_sd1.SetTextColor(BLACK_C);

	m_msg_labelset_sd2.SetFont(m_p_font);
 	m_msg_labelset_sd2.SetWindowText(_T("Set Darkness (SD)"));
	m_msg_labelset_sd2.SetCenterText();
	m_msg_labelset_sd2.SetColor(YELLOW_L);
	m_msg_labelset_sd2.SetGradientColor(YELLOW_L);
	m_msg_labelset_sd2.SetTextColor(BLACK_C);

	m_msg_labelset_sd3.SetFont(m_p_font);
 	m_msg_labelset_sd3.SetWindowText(_T("Set Darkness (SD)"));
	m_msg_labelset_sd3.SetCenterText();
	m_msg_labelset_sd3.SetColor(YELLOW_L);
	m_msg_labelset_sd3.SetGradientColor(YELLOW_L);
	m_msg_labelset_sd3.SetTextColor(BLACK_C);

	m_msg_labelset_sd4.SetFont(m_p_font);
 	m_msg_labelset_sd4.SetWindowText(_T("Set Darkness (SD)"));
	m_msg_labelset_sd4.SetCenterText();
	m_msg_labelset_sd4.SetColor(YELLOW_L);
	m_msg_labelset_sd4.SetGradientColor(YELLOW_L);
	m_msg_labelset_sd4.SetTextColor(BLACK_C);

	m_msg_labelset_lt1.SetFont(m_p_font);
 	m_msg_labelset_lt1.SetWindowText(_T("Label Top (LT)"));
	m_msg_labelset_lt1.SetCenterText();
	m_msg_labelset_lt1.SetColor(YELLOW_L);
	m_msg_labelset_lt1.SetGradientColor(YELLOW_L);
	m_msg_labelset_lt1.SetTextColor(BLACK_C);
	
	m_msg_labelset_lt2.SetFont(m_p_font);
 	m_msg_labelset_lt2.SetWindowText(_T("Label Top (LT)"));
	m_msg_labelset_lt2.SetCenterText();
	m_msg_labelset_lt2.SetColor(YELLOW_L);
	m_msg_labelset_lt2.SetGradientColor(YELLOW_L);
	m_msg_labelset_lt2.SetTextColor(BLACK_C);

	m_msg_labelset_lt3.SetFont(m_p_font);
 	m_msg_labelset_lt3.SetWindowText(_T("Label Top (LT)"));
	m_msg_labelset_lt3.SetCenterText();
	m_msg_labelset_lt3.SetColor(YELLOW_L);
	m_msg_labelset_lt3.SetGradientColor(YELLOW_L);
	m_msg_labelset_lt3.SetTextColor(BLACK_C);

	m_msg_labelset_lt4.SetFont(m_p_font);
 	m_msg_labelset_lt4.SetWindowText(_T("Label Top (LT)"));
	m_msg_labelset_lt4.SetCenterText();
	m_msg_labelset_lt4.SetColor(YELLOW_L);
	m_msg_labelset_lt4.SetGradientColor(YELLOW_L);
	m_msg_labelset_lt4.SetTextColor(BLACK_C);

	m_msg_labelset_lhx1.SetFont(m_p_font);
 	m_msg_labelset_lhx1.SetWindowText(_T("Label Home X (LH)"));
	m_msg_labelset_lhx1.SetCenterText();
	m_msg_labelset_lhx1.SetColor(YELLOW_L);
	m_msg_labelset_lhx1.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhx1.SetTextColor(BLACK_C);
	
	m_msg_labelset_lhx2.SetFont(m_p_font);
 	m_msg_labelset_lhx2.SetWindowText(_T("Label Home X (LH)"));
	m_msg_labelset_lhx2.SetCenterText();
	m_msg_labelset_lhx2.SetColor(YELLOW_L);
	m_msg_labelset_lhx2.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhx2.SetTextColor(BLACK_C);

	m_msg_labelset_lhx3.SetFont(m_p_font);
 	m_msg_labelset_lhx3.SetWindowText(_T("Label Home X (LH)"));
	m_msg_labelset_lhx3.SetCenterText();
	m_msg_labelset_lhx3.SetColor(YELLOW_L);
	m_msg_labelset_lhx3.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhx3.SetTextColor(BLACK_C);

	m_msg_labelset_lhx4.SetFont(m_p_font);
 	m_msg_labelset_lhx4.SetWindowText(_T("Label Home X (LH)"));
	m_msg_labelset_lhx4.SetCenterText();
	m_msg_labelset_lhx4.SetColor(YELLOW_L);
	m_msg_labelset_lhx4.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhx4.SetTextColor(BLACK_C);

	m_msg_labelset_lhy1.SetFont(m_p_font);
 	m_msg_labelset_lhy1.SetWindowText(_T("Label Home Y (LH)"));
	m_msg_labelset_lhy1.SetCenterText();
	m_msg_labelset_lhy1.SetColor(YELLOW_L);
	m_msg_labelset_lhy1.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhy1.SetTextColor(BLACK_C);
	
	m_msg_labelset_lhy2.SetFont(m_p_font);
 	m_msg_labelset_lhy2.SetWindowText(_T("Label Home Y (LH)"));
	m_msg_labelset_lhy2.SetCenterText();
	m_msg_labelset_lhy2.SetColor(YELLOW_L);
	m_msg_labelset_lhy2.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhy2.SetTextColor(BLACK_C);
	
	m_msg_labelset_lhy3.SetFont(m_p_font);
 	m_msg_labelset_lhy3.SetWindowText(_T("Label Home Y (LH)"));
	m_msg_labelset_lhy3.SetCenterText();
	m_msg_labelset_lhy3.SetColor(YELLOW_L);
	m_msg_labelset_lhy3.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhy3.SetTextColor(BLACK_C);
	
	m_msg_labelset_lhy4.SetFont(m_p_font);
 	m_msg_labelset_lhy4.SetWindowText(_T("Label Home Y (LH)"));
	m_msg_labelset_lhy4.SetCenterText();
	m_msg_labelset_lhy4.SetColor(YELLOW_L);
	m_msg_labelset_lhy4.SetGradientColor(YELLOW_L);
	m_msg_labelset_lhy4.SetTextColor(BLACK_C);
	
	CString strModel = st_basic.mstr_device_name;
	strModel.Replace(".TXT", "");
	m_ctrlName.SetWindowText(strModel);
}

void CScreen_Set_Barcode::OnBarcode_Data_Display()
{
	mn_labelset_sd1[1] = st_barcode.mn_labelset_sd[0];
	mn_labelset_sd2[1] = st_barcode.mn_labelset_sd[1];
	mn_labelset_sd3[1] = st_barcode.mn_labelset_sd[2];
	mn_labelset_sd4[1] = st_barcode.mn_labelset_sd[3];
	
////2015.0705
// 	mn_labelset_lt1[1] = st_barcode.mn_labelset_lt[0]*2;
// 	mn_labelset_lt2[1] = st_barcode.mn_labelset_lt[1]*2;
// 	mn_labelset_lt3[1] = st_barcode.mn_labelset_lt[2]*2;
// 	mn_labelset_lt4[1] = st_barcode.mn_labelset_lt[3]*2;

	mn_labelset_lt1[1] = st_barcode.mn_labelset_lt[0];
	mn_labelset_lt2[1] = st_barcode.mn_labelset_lt[1];
	mn_labelset_lt3[1] = st_barcode.mn_labelset_lt[2];
	mn_labelset_lt4[1] = st_barcode.mn_labelset_lt[3];	
////
	mn_labelset_lhx1[1] = st_barcode.mn_labelset_lhx[0];
	mn_labelset_lhx2[1] = st_barcode.mn_labelset_lhx[1];
	mn_labelset_lhx3[1] = st_barcode.mn_labelset_lhx[2];
	mn_labelset_lhx4[1] = st_barcode.mn_labelset_lhx[3];
	
	mn_labelset_lhy1[1] = st_barcode.mn_labelset_lhy[0];
	mn_labelset_lhy2[1] = st_barcode.mn_labelset_lhy[1];
	mn_labelset_lhy3[1] = st_barcode.mn_labelset_lhy[2];
	mn_labelset_lhy4[1] = st_barcode.mn_labelset_lhy[3];
	
	mn_Prt_complete_use1[1] = st_barcode.mn_Prt_complete_use[0];
	mn_Prt_complete_use2[1] = st_barcode.mn_Prt_complete_use[1];
	mn_Prt_complete_use3[1] = st_barcode.mn_Prt_complete_use[2];
	mn_Prt_complete_use4[1] = st_barcode.mn_Prt_complete_use[3];
	
	mn_Prt_Mode1[1] = st_barcode.mn_Prt_Mode[0];
	mn_Prt_Mode2[1] = st_barcode.mn_Prt_Mode[1];
	mn_Prt_Mode3[1] = st_barcode.mn_Prt_Mode[2];
	mn_Prt_Mode4[1] = st_barcode.mn_Prt_Mode[3];
	
}

void CScreen_Set_Barcode::OnBarcode_Data_Set()
{
	if(st_barcode.nApplyButton[0] == 1)
	{
		st_barcode.mn_labelset_sd[0] = mn_labelset_sd1[1];
		st_barcode.mn_labelset_lt[0] = mn_labelset_lt1[1];
		st_barcode.mn_labelset_lhx[0] = mn_labelset_lhx1[1];
		st_barcode.mn_labelset_lhy[0] = mn_labelset_lhy1[1];
		st_barcode.mn_Prt_complete_use[0] = mn_Prt_complete_use1[1];
		st_barcode.mn_Prt_Mode[0] = mn_Prt_Mode1[1];
		////2015.0413
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER1);
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER1);
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER1);
		////

	}
	if(st_barcode.nApplyButton[1] == 1)
	{
		st_barcode.mn_labelset_sd[1] = mn_labelset_sd2[1];
		st_barcode.mn_labelset_lt[1] = mn_labelset_lt2[1];
		st_barcode.mn_labelset_lhx[1] = mn_labelset_lhx2[1];
		st_barcode.mn_labelset_lhy[1] = mn_labelset_lhy2[1];
		st_barcode.mn_Prt_complete_use[1] = mn_Prt_complete_use2[1];
		st_barcode.mn_Prt_Mode[1] = mn_Prt_Mode2[1];
		////2015.0413
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER2);
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER2);
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER2);		
		////
	}
	if(st_barcode.nApplyButton[2] == 1)
	{
		st_barcode.mn_labelset_sd[2] = mn_labelset_sd3[1];
		st_barcode.mn_labelset_lt[2] = mn_labelset_lt3[1];
		st_barcode.mn_labelset_lhx[2] = mn_labelset_lhx3[1];
		st_barcode.mn_labelset_lhy[2] = mn_labelset_lhy3[1];
		st_barcode.mn_Prt_complete_use[2] = mn_Prt_complete_use3[1];
		st_barcode.mn_Prt_Mode[2] = mn_Prt_Mode3[1];
		////2015.0413
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER3);
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER3);
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER3);		
		////
	}
	if(st_barcode.nApplyButton[3] == 1)
	{
		st_barcode.mn_labelset_sd[3] = mn_labelset_sd4[1];
		st_barcode.mn_labelset_lt[3] = mn_labelset_lt4[1];
		st_barcode.mn_labelset_lhx[3] = mn_labelset_lhx4[1];
		st_barcode.mn_labelset_lhy[3] = mn_labelset_lhy4[1];
		st_barcode.mn_Prt_complete_use[3] = mn_Prt_complete_use4[1];
		st_barcode.mn_Prt_Mode[3] = mn_Prt_Mode4[1];
		////2015.0413
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER4);
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER4);
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER4);		
		////
	}
	if(st_barcode.nApplyButton[4] == 1)
	{
		st_barcode.mn_labelset_sd[0] = mn_labelset_sd1[1];
		st_barcode.mn_labelset_lt[0] = mn_labelset_lt1[1];
		st_barcode.mn_labelset_lhx[0] = mn_labelset_lhx1[1];
		st_barcode.mn_labelset_lhy[0] = mn_labelset_lhy1[1];
		st_barcode.mn_Prt_complete_use[0] = mn_Prt_complete_use1[1];
		st_barcode.mn_Prt_Mode[0] = mn_Prt_Mode1[1];
		
		st_barcode.mn_labelset_sd[1] = mn_labelset_sd2[1];
		st_barcode.mn_labelset_lt[1] = mn_labelset_lt2[1];
		st_barcode.mn_labelset_lhx[1] = mn_labelset_lhx2[1];
		st_barcode.mn_labelset_lhy[1] = mn_labelset_lhy2[1];
		st_barcode.mn_Prt_complete_use[1] = mn_Prt_complete_use2[1];
		st_barcode.mn_Prt_Mode[1] = mn_Prt_Mode2[1];
		
		st_barcode.mn_labelset_sd[2] = mn_labelset_sd3[1];
		st_barcode.mn_labelset_lt[2] = mn_labelset_lt3[1];
		st_barcode.mn_labelset_lhx[2] = mn_labelset_lhx3[1];
		st_barcode.mn_labelset_lhy[2] = mn_labelset_lhy3[1];
		st_barcode.mn_Prt_complete_use[2] = mn_Prt_complete_use3[1];
		st_barcode.mn_Prt_Mode[2] = mn_Prt_Mode3[1];
		
		st_barcode.mn_labelset_sd[3] = mn_labelset_sd4[1];
		st_barcode.mn_labelset_lt[3] = mn_labelset_lt4[1];
		st_barcode.mn_labelset_lhx[3] = mn_labelset_lhx4[1];
		st_barcode.mn_labelset_lhy[3] = mn_labelset_lhy4[1];
		st_barcode.mn_Prt_complete_use[3] = mn_Prt_complete_use4[1];
		st_barcode.mn_Prt_Mode[3] = mn_Prt_Mode4[1];
		////2015.0413
		for(int i=0; i<4; i++)
		{
			clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER1 + i);
			clsZebra.LabelTop_TCP(CLS_BCR_PRINTER1 + i);
			clsZebra.LabelHome_TCP(CLS_BCR_PRINTER1 + i);
		}
		////
	}
	
	OnBarcode_Data_Backup();
}

void CScreen_Set_Barcode::OnBarcode_Data_Backup()
{
	mn_labelset_sd1[0] = mn_labelset_sd1[1];
	mn_labelset_lt1[0] = mn_labelset_lt1[1];
	mn_labelset_lhx1[0] = mn_labelset_lhx1[1];
	mn_labelset_lhy1[0] = mn_labelset_lhy1[1];
	mn_Prt_complete_use1[0] = mn_Prt_complete_use1[1];
	mn_Prt_Mode1[0] = mn_Prt_Mode1[1];

	mn_labelset_sd2[0] = mn_labelset_sd2[1];
	mn_labelset_lt2[0] = mn_labelset_lt2[1];
	mn_labelset_lhx2[0] = mn_labelset_lhx2[1];
	mn_labelset_lhy2[0] = mn_labelset_lhy2[1];
	mn_Prt_complete_use2[0] = mn_Prt_complete_use2[1];
	mn_Prt_Mode2[0] = mn_Prt_Mode2[1];

	mn_labelset_sd3[0] = mn_labelset_sd3[1];
	mn_labelset_lt3[0] = mn_labelset_lt3[1];
	mn_labelset_lhx3[0] = mn_labelset_lhx3[1];
	mn_labelset_lhy3[0] = mn_labelset_lhy3[1];
	mn_Prt_complete_use3[0] = mn_Prt_complete_use3[1];
	mn_Prt_Mode3[0] = mn_Prt_Mode3[1];

	mn_labelset_sd4[0] = mn_labelset_sd4[1];
	mn_labelset_lt4[0] = mn_labelset_lt4[1];
	mn_labelset_lhx4[0] = mn_labelset_lhx4[1];
	mn_labelset_lhy4[0] = mn_labelset_lhy4[1];
	mn_Prt_complete_use4[0] = mn_Prt_complete_use4[1];
	mn_Prt_Mode4[0] = mn_Prt_Mode4[1];
}

void CScreen_Set_Barcode::OnBarcode_Data_Recovery()
{
	mn_labelset_sd1[1] = mn_labelset_sd1[0];
	mn_labelset_lt1[1] = mn_labelset_lt1[0];
	mn_labelset_lhx1[1] = mn_labelset_lhx1[0];
	mn_labelset_lhy1[1] = mn_labelset_lhy1[0];
	mn_Prt_complete_use1[1] = mn_Prt_complete_use1[0];
	mn_Prt_Mode1[1] = mn_Prt_Mode1[0];
	
	mn_labelset_sd2[1] = mn_labelset_sd2[0];
	mn_labelset_lt2[1] = mn_labelset_lt2[0];
	mn_labelset_lhx2[1] = mn_labelset_lhx2[0];
	mn_labelset_lhy2[1] = mn_labelset_lhy2[0];
	mn_Prt_complete_use2[1] = mn_Prt_complete_use2[0];
	mn_Prt_Mode2[1] = mn_Prt_Mode2[0];
	
	mn_labelset_sd3[1] = mn_labelset_sd3[0];
	mn_labelset_lt3[1] = mn_labelset_lt3[0];
	mn_labelset_lhx3[1] = mn_labelset_lhx3[0];
	mn_labelset_lhy3[1] = mn_labelset_lhy3[0];
	mn_Prt_complete_use3[1] = mn_Prt_complete_use3[0];
	mn_Prt_Mode3[1] = mn_Prt_Mode3[0];
	
	mn_labelset_sd4[1] = mn_labelset_sd4[0];
	mn_labelset_lt4[1] = mn_labelset_lt4[0];
	mn_labelset_lhx4[1] = mn_labelset_lhx4[0];
	mn_labelset_lhy4[1] = mn_labelset_lhy4[0];
	mn_Prt_complete_use4[1] = mn_Prt_complete_use4[0];
	mn_Prt_Mode4[1] = mn_Prt_Mode4[0];
}
void CScreen_Set_Barcode::OnBarcode_Dgt_Set()
{
	m_dgt_labelset_sd1.SubclassDlgItem(IDC_DGT_LABELSET_SD1, this);
	m_dgt_labelset_sd1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_sd1.SetValue(mn_labelset_sd1[1]);

	m_dgt_labelset_sd2.SubclassDlgItem(IDC_DGT_LABELSET_SD2, this);
	m_dgt_labelset_sd2.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_sd2.SetValue(mn_labelset_sd2[1]);

	m_dgt_labelset_sd3.SubclassDlgItem(IDC_DGT_LABELSET_SD3, this);
	m_dgt_labelset_sd3.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_sd3.SetValue(mn_labelset_sd3[1]);

	m_dgt_labelset_sd4.SubclassDlgItem(IDC_DGT_LABELSET_SD4, this);
	m_dgt_labelset_sd4.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_sd4.SetValue(mn_labelset_sd4[1]);

	m_dgt_labelset_lt1.SubclassDlgItem(IDC_DGT_LABELSET_LT1, this);
	m_dgt_labelset_lt1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lt1.SetValue(mn_labelset_lt1[1]);

	m_dgt_labelset_lt2.SubclassDlgItem(IDC_DGT_LABELSET_LT2, this);
	m_dgt_labelset_lt2.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lt2.SetValue(mn_labelset_lt2[1]);

	m_dgt_labelset_lt3.SubclassDlgItem(IDC_DGT_LABELSET_LT3, this);
	m_dgt_labelset_lt3.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lt3.SetValue(mn_labelset_lt3[1]);

	m_dgt_labelset_lt4.SubclassDlgItem(IDC_DGT_LABELSET_LT4, this);
	m_dgt_labelset_lt4.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lt4.SetValue(mn_labelset_lt4[1]);

	m_dgt_labelset_lhx1.SubclassDlgItem(IDC_DGT_LABELSET_LHX1, this);
	m_dgt_labelset_lhx1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhx1.SetValue(mn_labelset_lhx1[1]);

	m_dgt_labelset_lhx2.SubclassDlgItem(IDC_DGT_LABELSET_LHX2, this);
	m_dgt_labelset_lhx2.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhx2.SetValue(mn_labelset_lhx2[1]);

	m_dgt_labelset_lhx3.SubclassDlgItem(IDC_DGT_LABELSET_LHX3, this);
	m_dgt_labelset_lhx3.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhx3.SetValue(mn_labelset_lhx3[1]);

	m_dgt_labelset_lhx4.SubclassDlgItem(IDC_DGT_LABELSET_LHX4, this);
	m_dgt_labelset_lhx4.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhx4.SetValue(mn_labelset_lhx4[1]);

	m_dgt_labelset_lhy1.SubclassDlgItem(IDC_DGT_LABELSET_LHY1, this);
	m_dgt_labelset_lhy1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhy1.SetValue(mn_labelset_lhy1[1]);
	
	m_dgt_labelset_lhy2.SubclassDlgItem(IDC_DGT_LABELSET_LHY2, this);
	m_dgt_labelset_lhy2.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhy2.SetValue(mn_labelset_lhy2[1]);
	
	m_dgt_labelset_lhy3.SubclassDlgItem(IDC_DGT_LABELSET_LHY3, this);
	m_dgt_labelset_lhy3.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhy3.SetValue(mn_labelset_lhy3[1]);
	
	m_dgt_labelset_lhy4.SubclassDlgItem(IDC_DGT_LABELSET_LHY4, this);
	m_dgt_labelset_lhy4.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lhy4.SetValue(mn_labelset_lhy4[1]);
	
}

void CScreen_Set_Barcode::OnBarcode_Button_Set()
{
	short	shBtnColor = 30;

    m_radio_prt_mode1.SetCheck(CTL_YES);
	m_radio_prt_mode2.SetCheck(CTL_YES);
	m_radio_prt_mode3.SetCheck(CTL_YES);
	m_radio_prt_mode4.SetCheck(CTL_YES);

	if(mn_Prt_complete_use1[1] == 0)
	{
		m_radio_prt_complete_use1.SetCheck(CTL_YES);
		m_radio_prt_complete_notuse1.SetCheck(CTL_NO);
	}
	else
	{
		m_radio_prt_complete_use1.SetCheck(CTL_NO);
		m_radio_prt_complete_notuse1.SetCheck(CTL_YES);
	}

	if(mn_Prt_complete_use2[1] == 0)
	{
		m_radio_prt_complete_use2.SetCheck(CTL_YES);
		m_radio_prt_complete_notuse2.SetCheck(CTL_NO);
	}
	else
	{
		m_radio_prt_complete_use2.SetCheck(CTL_NO);
		m_radio_prt_complete_notuse2.SetCheck(CTL_YES);
	}

	if(mn_Prt_complete_use3[1] == 0)
	{
		m_radio_prt_complete_use3.SetCheck(CTL_YES);
		m_radio_prt_complete_notuse3.SetCheck(CTL_NO);
	}
	else
	{
		m_radio_prt_complete_use3.SetCheck(CTL_NO);
		m_radio_prt_complete_notuse3.SetCheck(CTL_YES);
	}

	if(mn_Prt_complete_use4[1] == 0)
	{
		m_radio_prt_complete_use4.SetCheck(CTL_YES);
		m_radio_prt_complete_notuse4.SetCheck(CTL_NO);
	}
	else
	{
		m_radio_prt_complete_use4.SetCheck(CTL_NO);
		m_radio_prt_complete_notuse4.SetCheck(CTL_YES);
	}
}

void CScreen_Set_Barcode::OnBtnPrtStatus1() 
{
	// TODO: Add your control notification handler code here
	//Func.Printer_Status(BCR_PORT3 - 1);
	clsZebra.Printer_Status_TCP(CLS_BCR_PRINTER1);
}

void CScreen_Set_Barcode::OnBtnPrtStatus2() 
{
	// TODO: Add your control notification handler code here
	//Func.Printer_Status(BCR_PORT4 - 1);
	clsZebra.Printer_Status_TCP(CLS_BCR_PRINTER2);
}

void CScreen_Set_Barcode::OnBtnPrtStatus3() 
{
	// TODO: Add your control notification handler code here
	//Func.Printer_Status(BCR_PORT5 - 1);
	clsZebra.Printer_Status_TCP(CLS_BCR_PRINTER3);
}

void CScreen_Set_Barcode::OnBtnPrtStatus4() 
{
	// TODO: Add your control notification handler code here
	//Func.Printer_Status(BCR_PORT6 - 1);
	clsZebra.Printer_Status_TCP(CLS_BCR_PRINTER4);
}

void CScreen_Set_Barcode::OnRadioPrtMode1() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_mode1.SetCheck(CTL_YES);
	mn_Prt_Mode1[1] = 0;	
	//Func.Rewind_Mode(BCR_PORT3 - 1);
	clsZebra.Rewind_Mode_TCP(CLS_BCR_PRINTER1);
}

void CScreen_Set_Barcode::OnRadioPrtMode2() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_mode2.SetCheck(CTL_YES);
	mn_Prt_Mode2[1] = 1;
//	Func.Rewind_Mode(BCR_PORT4 - 1);	
	clsZebra.Rewind_Mode_TCP(CLS_BCR_PRINTER2);

}

void CScreen_Set_Barcode::OnRadioPrtMode3() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_mode3.SetCheck(CTL_YES);
	mn_Prt_Mode3[1] = 1;
// 	Func.Rewind_Mode(BCR_PORT5 - 1);	
	clsZebra.Rewind_Mode_TCP(CLS_BCR_PRINTER3);

}

void CScreen_Set_Barcode::OnRadioPrtMode4() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_mode4.SetCheck(CTL_YES);
	mn_Prt_Mode4[1] = 1;
//	Func.Rewind_Mode(BCR_PORT6 - 1);	
	clsZebra.Rewind_Mode_TCP(CLS_BCR_PRINTER4);

}
#include "ZebraPrint.h"

void CScreen_Set_Barcode::OnRadioPrtCompleteUse1() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use1.SetCheck(CTL_YES);
	m_radio_prt_complete_notuse1.SetCheck(CTL_NO);
	mn_Prt_complete_use1[1] = 0;
//	Func.Print_Complete_Set(BCR_PORT3 - 1);	
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER1);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteUse2() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use2.SetCheck(CTL_YES);
	m_radio_prt_complete_notuse2.SetCheck(CTL_NO);
	mn_Prt_complete_use2[1] = 0;
//	Func.Print_Complete_Set(BCR_PORT4 - 1);	
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER2);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteUse3() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use3.SetCheck(CTL_YES);
	m_radio_prt_complete_notuse3.SetCheck(CTL_NO);
	mn_Prt_complete_use3[1] = 0;
//	Func.Print_Complete_Set(BCR_PORT5 - 1);	
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER3);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteUse4() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use4.SetCheck(CTL_YES);
	m_radio_prt_complete_notuse4.SetCheck(CTL_NO);
	mn_Prt_complete_use4[1] = 0;
//	Func.Print_Complete_Set(BCR_PORT6 - 1);	
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER4);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteNotuse1() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use1.SetCheck(CTL_NO);
	m_radio_prt_complete_notuse1.SetCheck(CTL_YES);
	mn_Prt_complete_use1[1] = 1;
//	Func.Print_Complete_Reset(BCR_PORT3 - 1);	
	clsZebra.OnPrintAnswerMode(2, 1, CLS_BCR_PRINTER1);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteNotuse2() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use2.SetCheck(CTL_NO);
	m_radio_prt_complete_notuse2.SetCheck(CTL_YES);
	mn_Prt_complete_use2[1] = 1;
//	Func.Print_Complete_Reset(BCR_PORT4 - 1);
	clsZebra.OnPrintAnswerMode(2, 1, CLS_BCR_PRINTER2);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteNotuse3() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use3.SetCheck(CTL_NO);
	m_radio_prt_complete_notuse3.SetCheck(CTL_YES);
	mn_Prt_complete_use3[1] = 1;
//	Func.Print_Complete_Reset(BCR_PORT5 - 1);	
	clsZebra.OnPrintAnswerMode(2, 1, CLS_BCR_PRINTER3);
}

void CScreen_Set_Barcode::OnRadioPrtCompleteNotuse4() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_complete_use4.SetCheck(CTL_NO);
	m_radio_prt_complete_notuse4.SetCheck(CTL_YES);
	mn_Prt_complete_use4[1] = 1;
//	Func.Print_Complete_Reset(BCR_PORT6 - 1);	
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER4);
}

void CScreen_Set_Barcode::OnDgtLabelsetSd1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_sd1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("SD LABEL 1 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_keypad_msg = _T("SD LABEL 1 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "80";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_sd1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_sd1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_sd1.SetValue(mn_labelset_sd1[1]);
	}
}

void CScreen_Set_Barcode::OnDgtLabelsetSd2() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_sd2.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("SD LABEL 2 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_keypad_msg = _T("SD LABEL 2 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "80";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_sd2.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_sd2[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_sd2.SetValue(mn_labelset_sd2[1]);
	}	
}

void CScreen_Set_Barcode::OnDgtLabelsetSd3() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_sd3.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("SD LABEL 3 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_keypad_msg = _T("SD LABEL 3 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "80";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_sd3.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_sd3[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_sd3.SetValue(mn_labelset_sd3[1]);
	}		
}

void CScreen_Set_Barcode::OnDgtLabelsetSd4() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_sd4.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("SD LABEL 4 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_keypad_msg = _T("SD LABEL 4 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "80";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_sd4.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_sd4[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_sd4.SetValue(mn_labelset_sd4[1]);
	}			
}

void CScreen_Set_Barcode::OnDgtLabelsetLt1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lt1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LT LABEL 1 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LT LABEL 1 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";//2015,0109
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lt1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lt1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lt1.SetValue(mn_labelset_lt1[1]);
//		mn_labelset_lt1[1] /= 2;//2015.0705
	}	
}

void CScreen_Set_Barcode::OnDgtLabelsetLt2() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lt2.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LT LABEL 2 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LT LABEL 2 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";//2015,0109
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lt2.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lt2[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lt2.SetValue(mn_labelset_lt2[1]);
//		mn_labelset_lt2[1] /= 2;//2015.0705
	}		
}

void CScreen_Set_Barcode::OnDgtLabelsetLt3() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lt3.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LT LABEL 3 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LT LABEL 3 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";//2015,0109
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lt3.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lt3[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lt3.SetValue(mn_labelset_lt3[1]);
//		mn_labelset_lt3[1] /= 2;//2015.0705
	}		
}

void CScreen_Set_Barcode::OnDgtLabelsetLt4() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lt4.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LT LABEL 4 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LT LABEL 4 SET (EA)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";//2015,0109
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lt4.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lt4[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lt4.SetValue(mn_labelset_lt4[1]);
//		mn_labelset_lt4[1] /= 2;//2015.0705
	}		
}

void CScreen_Set_Barcode::OnDgtLabelsetLhx1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhx1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH X LABEL 1 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH X LABEL 1 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhx1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhx1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhx1.SetValue(mn_labelset_lhx1[1]);
	}	
}

void CScreen_Set_Barcode::OnDgtLabelsetLhx2() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhx2.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH X LABEL 2 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH X LABEL 2 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhx2.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhx2[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhx2.SetValue(mn_labelset_lhx2[1]);
	}		
}

void CScreen_Set_Barcode::OnDgtLabelsetLhx3() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhx3.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH X LABEL 3 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH X LABEL 3 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhx3.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhx3[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhx3.SetValue(mn_labelset_lhx3[1]);
	}			
}

void CScreen_Set_Barcode::OnDgtLabelsetLhx4() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhx4.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH X LABEL 4 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH X LABEL 4 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhx4.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhx4[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhx4.SetValue(mn_labelset_lhx4[1]);
	}			
}

void CScreen_Set_Barcode::OnDgtLabelsetLhy1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhy1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH Y LABEL 1 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH Y LABEL 1 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhy1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhy1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhy1.SetValue(mn_labelset_lhy1[1]);
	}			
}

void CScreen_Set_Barcode::OnDgtLabelsetLhy2() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhy2.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH Y LABEL 2 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH Y LABEL 2 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhy2.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhy2[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhy2.SetValue(mn_labelset_lhy2[1]);
	}				
}

void CScreen_Set_Barcode::OnDgtLabelsetLhy3() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhy3.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH Y LABEL 3 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH Y LABEL 3 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhy3.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhy3[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhy3.SetValue(mn_labelset_lhy3[1]);
	}				
}

void CScreen_Set_Barcode::OnDgtLabelsetLhy4() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lhy4.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH Y LABEL 4 SET (EA)");
	if (g_local.GetLocalType() == LOCAL_ENG)	st_msg.mstr_keypad_msg = _T("LH Y LABEL 4 SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lhy4.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lhy4[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lhy4.SetValue(mn_labelset_lhy4[1]);
	}				
}

void CScreen_Set_Barcode::OnBtnPrtApply1() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "1번 프린터 설정값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.1?...";
		
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.nApplyButton[0] = 1;
		OnBarcode_Data_Set();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수	
		st_barcode.nApplyButton[0] = 0;
	}
	else if (n_response == IDCANCEL)
	{
		OnBarcode_Data_Recovery();
		OnBarcode_Button_Set();
	}		
}

void CScreen_Set_Barcode::OnBtnPrtApply2() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "2번 프린터 설정값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.2?...";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.nApplyButton[1] = 1;
		OnBarcode_Data_Set();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		st_barcode.nApplyButton[1] = 0;
	}
	else if (n_response == IDCANCEL)
	{
		OnBarcode_Data_Recovery();
		OnBarcode_Button_Set();
	}			
}

void CScreen_Set_Barcode::OnBtnPrtApply3() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "3번 프린터 설정값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.3?...";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.nApplyButton[2] = 1;
		OnBarcode_Data_Set();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		st_barcode.nApplyButton[2] = 0;
	}
	else if (n_response == IDCANCEL)
	{
		OnBarcode_Data_Recovery();
		OnBarcode_Button_Set();
	}			
}

void CScreen_Set_Barcode::OnBtnPrtApply4() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "4번 프린터 설정값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.4?...";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.nApplyButton[3] = 1;
		OnBarcode_Data_Set();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		st_barcode.nApplyButton[3] = 0;
	}
	else if (n_response == IDCANCEL)
	{
		OnBarcode_Data_Recovery();
		OnBarcode_Button_Set();
	}			
}

void CScreen_Set_Barcode::OnBtnPrtApply5() 
{
	return;
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "모든 프린터 설정값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the All printer ?...";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.nApplyButton[4] = 1;
		OnBarcode_Data_Set();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		st_barcode.nApplyButton[4] = 0;
	}
	else if (n_response == IDCANCEL)
	{
		OnBarcode_Data_Recovery();
		OnBarcode_Button_Set();
	}				
}

void CScreen_Set_Barcode::OnBtnModelDensity() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 1;
	m_nInterfaceStep	= 0;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL, 100, NULL);
}

void CScreen_Set_Barcode::OnBtnModelBarcode() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 2;
	m_nInterfaceStep	= 0;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL, 100, NULL);
}

void CScreen_Set_Barcode::OnBtnModelDensityPos() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 3;
	m_nInterfaceStep	= 0;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL, 100, NULL);	
}

void CScreen_Set_Barcode::OnBtnModelBarcodePos() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 4;
	m_nInterfaceStep	= 0;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL, 100, NULL);	
}

void CScreen_Set_Barcode::OnBtnDataDensity() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 1;
	m_nInterfaceStep	= 0;

	SetTimer(TM_DATA, 100, NULL);	
}

void CScreen_Set_Barcode::OnBtnDataBarcode() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 2;
	m_nInterfaceStep	= 0;

	SetTimer(TM_DATA, 100, NULL);	
}

void CScreen_Set_Barcode::OnBtnDataDensityPos() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 3;
	m_nInterfaceStep	= 0;
	
	SetTimer(TM_DATA, 100, NULL);	
}

void CScreen_Set_Barcode::OnBtnDataBarcodePos() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 4;
	m_nInterfaceStep	= 0;
	
	SetTimer(TM_DATA, 100, NULL);	
}

int	CScreen_Set_Barcode::OnVisionModelChange()
{
//	switch(m_nInterfaceStep)
//	{
//		case 0:
//			{
//				CString strModel = st_basic.mstr_device_name;
//				strModel.Replace(".TXT", "");
//				g_client_vision[m_nClientPos].ClearResultChgMdl();
//				g_client_vision[m_nClientPos].ClearRecvResultcj();
//				g_client_vision[m_nClientPos].SetBcrChgMdl(strModel);
//				m_dwTimeWait[0] = GetTickCount();
//				m_nInterfaceStep = 100;
//			}
//			break;
//
//		case 100:
//			if( g_client_vision[m_nClientPos].GetResultChgMdl() == NVR_PASS)
//			{
//				
//				CString strModel = st_basic.mstr_device_name;
//				strModel.Replace(".TXT", "");
//				if( g_client_vision[m_nClientPos].GetRecvResultcj() != strModel)
//				{
//					g_client_vision[m_nClientPos].SetResultChgMdl(NVR_FAIL);
//				}	
//				return CTLBD_RET_GOOD;
//			}
//			else if( g_client_vision[m_nClientPos].GetResultChgMdl() == NVR_FAIL )
//			{				
//				m_nInterfaceStep = 0;
//				return CTLBD_RET_ERROR;
//			}
//			m_dwTimeWait[1] = GetTickCount();
//			m_dwTimeWait[2] = m_dwTimeWait[1] - m_dwTimeWait[0];
//
//			if(m_dwTimeWait[2] < 0)
//			{
//				m_dwTimeWait[0] = GetTickCount();
//				break;
//			}
//
//			if(m_dwTimeWait[2] > 10000)
//			{
//				m_nInterfaceStep = 0;
//
//				return CTLBD_RET_ERROR;
//			}
//			break;
//	}

	return CTLBD_RET_PROCEED;
}


int	CScreen_Set_Barcode::OnVisionData()
{
//	switch(m_nInterfaceStep)
//	{
//		case 0:
//			g_client_vision[m_nClientPos].ClearResultBcrTest();
//			g_client_vision[m_nClientPos].SetBcrTest();
//			m_dwTimeWait[0] = GetTickCount();
//			m_nInterfaceStep = 100;
//			break;
//
//		case 100:
//			if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_PASS)
//			{
//				
//				CString strModel = st_basic.mstr_device_name;
//				strModel.Replace(".TXT", "");
//				if( g_client_vision[EVP_BTM_POS].GetRecvModelResult() != strModel)
//				{
//					g_client_vision[EVP_BTM_POS].SetResultBcrTest(NVR_FAIL);
//				}	
//				return CTLBD_RET_GOOD;
//			}
//			else if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_FAIL )
//			{				
//				m_nInterfaceStep = 0;
//				return CTLBD_RET_ERROR;
//			}
//			m_dwTimeWait[1] = GetTickCount();
//			m_dwTimeWait[2] = m_dwTimeWait[1] - m_dwTimeWait[0];
//
//			if(m_dwTimeWait[2] < 0)
//			{
//				m_dwTimeWait[0] = GetTickCount();
//				break;
//			}
//
//			if(m_dwTimeWait[2] > 10000)
//			{
//				m_nInterfaceStep = 0;
//
//				return CTLBD_RET_ERROR;
//			}
//			break;
//	}

	return CTLBD_RET_PROCEED;
}

void CScreen_Set_Barcode::OnTimer(UINT nIDEvent) 
{
	int n_ret;

	CDialog_Message msg_dlg;

	if (nIDEvent == TM_MODEL)
	{
		n_ret = OnVisionModelChange();

		if (n_ret == CTLBD_RET_GOOD)
		{
			KillTimer(TM_MODEL);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
		}
		else if (n_ret == CTLBD_RET_ERROR)
		{
			KillTimer(TM_MODEL);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
		}
	}
	else if (nIDEvent == TM_DATA)
	{
		n_ret = OnVisionData();
		
		if (n_ret == CTLBD_RET_GOOD)
		{
			KillTimer(TM_DATA);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
		}
		else if (n_ret == CTLBD_RET_ERROR)
		{
			KillTimer(TM_DATA);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
		}
	}
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Set_Barcode::OnBtnPrtSd1() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "1번 프린터 Darkness값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.1 Darkness?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_sd[0] = mn_labelset_sd1[1];
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER1);

		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext;
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_SD_00" );
		//AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_SD, strVal, st_barcode.mn_labelset_sd[0]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_sd[0]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);

	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_sd1[1] = mn_labelset_sd1[0];
		OnBarcode_Button_Set();
	}			
}

void CScreen_Set_Barcode::OnBtnPrtSd2() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "2번 프린터 Darkness값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.2 Darkness?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_sd[1] = mn_labelset_sd2[1];
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER2);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}


		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_SD_01" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_SD, strVal, st_barcode.mn_labelset_sd[1]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_sd[1]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_sd2[1] = mn_labelset_sd2[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtSd3() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "3번 프린터 Darkness값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.3 Darkness?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_sd[2] = mn_labelset_sd3[1];
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER3);

		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}
		
		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_SD_02" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_SD, strVal, st_barcode.mn_labelset_sd[2]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_sd[2]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_sd3[1] = mn_labelset_sd3[0];
		OnBarcode_Button_Set();
	}		
}

void CScreen_Set_Barcode::OnBtnPrtSd4() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "4번 프린터 Darkness값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.4 Darkness?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_sd[3] = mn_labelset_sd4[1];
		clsZebra.SetDarkness_TCP(CLS_BCR_PRINTER4);
		

		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_SD_03" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_SD, strVal, st_barcode.mn_labelset_sd[3]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_sd[3]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_sd4[1] = mn_labelset_sd4[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtLt1() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "1번 프린터 LabelTop값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.1 LabelTop?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lt[0] = mn_labelset_lt1[1];
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER1);
	
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}
		
		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LT_00" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LT, strVal, st_barcode.mn_labelset_lt[0]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lt[0]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lt1[1] = mn_labelset_lt1[0];
		OnBarcode_Button_Set();
	}
}

void CScreen_Set_Barcode::OnBtnPrtLt2() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "2번 프린터 LabelTop값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.2 LabelTop?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lt[1] = mn_labelset_lt2[1];
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER2);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LT_01" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LT, strVal, st_barcode.mn_labelset_lt[1]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lt[1]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lt2[1] = mn_labelset_lt2[0];
		OnBarcode_Button_Set();
	}
}

void CScreen_Set_Barcode::OnBtnPrtLt3() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "3번 프린터 LabelTop값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.3 LabelTop?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lt[2] = mn_labelset_lt3[1];
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER3);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}


		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LT_02" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LT, strVal, st_barcode.mn_labelset_lt[2]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lt[2]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lt3[1] = mn_labelset_lt3[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtLt4() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "4번 프린터 LabelTop값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.4 LabelTop?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lt[3] = mn_labelset_lt4[1];
		clsZebra.LabelTop_TCP(CLS_BCR_PRINTER4);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LT_03" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LT, strVal, st_barcode.mn_labelset_lt[3]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lt[3]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lt4[1] = mn_labelset_lt4[0];
		OnBarcode_Button_Set();
	}		
}

void CScreen_Set_Barcode::OnBtnPrtLhx1() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "1번 프린터 LabelX값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.1 LabelX?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhx[0] = mn_labelset_lhx1[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER1);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHX_00" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHX, strVal, st_barcode.mn_labelset_lhx[0]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhx[0]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhx1[1] = mn_labelset_lhx1[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtLhx2() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "2번 프린터 LabelX값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.2 LabelX?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhx[1] = mn_labelset_lhx2[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER2);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHX_01" );
//		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHX, strVal, st_barcode.mn_labelset_lhx[1]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhx[1]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhx2[1] = mn_labelset_lhx2[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtLhx3() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "3번 프린터 LabelX값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.3 LabelX?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhx[2] = mn_labelset_lhx3[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER3);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHX_02" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHX, strVal, st_barcode.mn_labelset_lhx[2]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhx[2]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhx3[1] = mn_labelset_lhx3[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtLhx4() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "4번 프린터 LabelX값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.4 LabelX?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhx[3] = mn_labelset_lhx4[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER4);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHX_03" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHX, strVal, st_barcode.mn_labelset_lhx[3]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhx[3]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhx4[1] = mn_labelset_lhx4[0];
		OnBarcode_Button_Set();
	}		
}

void CScreen_Set_Barcode::OnBtnPrtLhy1() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "1번 프린터 LabelHome값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.1 LabelHome?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhy[0] = mn_labelset_lhy1[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER1);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHY_00" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHY, strVal, st_barcode.mn_labelset_lhy[0]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhy[0]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhy1[1] = mn_labelset_lhy1[0];
		OnBarcode_Button_Set();
	}	
}

void CScreen_Set_Barcode::OnBtnPrtLhy2() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "2번 프린터 LabelHome값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.2 LabelHome?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhy[1] = mn_labelset_lhy2[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER2);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHY_01" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHY, strVal, st_barcode.mn_labelset_lhy[1]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhy[1]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhy2[1] = mn_labelset_lhy2[0];
		OnBarcode_Button_Set();
	}		
}

void CScreen_Set_Barcode::OnBtnPrtLhy3() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "3번 프린터 LabelHome값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.3 LabelHome?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhy[2] = mn_labelset_lhy3[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER3);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHY_02" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHY, strVal, st_barcode.mn_labelset_lhy[2]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhy[2]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);		
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhy3[1] = mn_labelset_lhy3[0];
		OnBarcode_Button_Set();
	}		
}

void CScreen_Set_Barcode::OnBtnPrtLhy4() 
{
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "4번 프린터 LabelHome값을 적용하시겠습니까?...";
	if(g_local.GetLocalType() == LOCAL_ENG) st_msg.mstr_confirm_msg = "Do you want to apply the printer No.4 LabelHome?..";
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		st_barcode.mn_labelset_lhy[3] = mn_labelset_lhy4[1];
		clsZebra.LabelHome_TCP(CLS_BCR_PRINTER4);
		
		CString str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;
		CString str_chk_ext="";
		int n_pos = str_save_file.Find("."); 
		if (n_pos == -1)
			str_save_file += _T(".TXT");
		else 
		{
			str_chk_ext = str_save_file.Mid(n_pos);
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");
				
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
		}

		CString strVal="", mstr_temp="";
		strVal = _T( "BCR_LHY_03" );
// 		AMTRegistry::RegWriteInt( REG_VAL_LABEL_BCR_LHY, strVal, st_barcode.mn_labelset_lhy[3]);
		mstr_temp.Format("%d",st_barcode.mn_labelset_lhy[3]);
		:: WritePrivateProfileString("BasicData", strVal, LPCTSTR(mstr_temp), str_save_file);
	}
	else if (n_response == IDCANCEL)
	{
		mn_labelset_lhy4[1] = mn_labelset_lhy4[0];
		OnBarcode_Button_Set();
	}	
}
