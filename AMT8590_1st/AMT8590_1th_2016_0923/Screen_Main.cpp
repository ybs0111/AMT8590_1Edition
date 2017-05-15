// Screen_Main.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Main.h"

#include "Dialog_KeyBoard.h"
#include "Dialog_Keypad.h"
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"
#include "Dialog_HandBarcode.h"
#include "Dialog_Select.h"
#include "io.h" 

#include "CtlBd_Function.h"
#include "GridControlAlg.h"

#include "CtlBd_Library.h"
#include "SrcBase\ALocalization.h"

#include "ComizoaPublic.h"

#include "AMTLotManager.h"
//#include "SrcPart/APartHandler_COK.h"
#include "SrcPart/PartFunction.h"
#include "SrcPart/APartTray.h"

#include "SrcPart/APartTestSite.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"

#include "AMTRegistry.h"
#include "SrcPart/APartShortkey.h"

#include "Dialog_XgemInterface.h"
#include "Dialog_Data_Lot.h"

#include "Dialog_Manual_Part_1.h"
#include "Dialog_Data_Module.h"

#include "Dialog_HandBarcode.h"//2014,1125 ysj
#include "Dialog_IDBuffer_Info.h"
#include "Dialog_NetWork.h"
#include "Dialog_Btm_IDBuffer_Info.h"
#include "SrcPart/APartHandler.h"
#include "XGemClient.h"
#include "SrcBase\ACriticalSection.h"
#include "Run_Unload_Conveyor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen_Main
#define	TM_ANIMATION 500 //2014.0821

IMPLEMENT_DYNCREATE(CScreen_Main, CFormView)

CScreen_Main::CScreen_Main()
	: CFormView(CScreen_Main::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Main)
	//}}AFX_DATA_INIT
	st_handler.mn_front_ready = 0;
	st_handler.mn_btm_ready = 0;
	st_handler.mn_d_piclplce = 0;
}

CScreen_Main::~CScreen_Main()
{
}

void CScreen_Main::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Main)
	DDX_Control(pDX, IDC_BUTTON_MODEL, m_btn_model);
	DDX_Control(pDX, IDC_DGT_MAINT, m_dgt_maint);
	DDX_Control(pDX, IDC_MSG_MAINT, m_msg_maint);
	DDX_Control(pDX, IDC_GROUP_TACT_INFO, m_group_tact_info);
	DDX_Control(pDX, IDC_DGT_IDLE, m_dgt_idle);
	DDX_Control(pDX, IDC_DGT_CHANGE, m_dgt_change);
	DDX_Control(pDX, IDC_MSG_CHANGE, m_msg_change);
	DDX_Control(pDX, IDC_MSG_IDLE, m_msg_idle);
	DDX_Control(pDX, IDC_GROUP_USE_BARCODE, m_group_use_barcode);
	DDX_Control(pDX, IDC_BTN_CONVEY_RESET, m_btn_convey_reset);
	DDX_Control(pDX, IDC_GROUP_DENSITY, m_group_density_error);
	DDX_Control(pDX, IDC_GROUP_REAR, m_group_rear);
	DDX_Control(pDX, IDC_GROUP_FRONT, m_group_front);
	DDX_Control(pDX, IDC_GROUP_UNLOAD_CONVEYOR, m_group_unload_conveyor);
	DDX_Control(pDX, IDC_DGT_TACT, m_dgt_tact);
	DDX_Control(pDX, IDC_CHK_DENSITY_PICKPLACE, m_chk_dpickplace);
	DDX_Control(pDX, IDC_CHK_BTM_READY, m_chk_btm_ready);
	DDX_Control(pDX, IDC_CHECK_FRONT_READY, m_chk_front_ready);
	DDX_Control(pDX, IDC_GROUP_BCR_LABEL_INFO, m_group_bcr_label_info);
	DDX_Control(pDX, IDC_BTN_DOOR_OPEN, m_btn_door_open);
	DDX_Control(pDX, IDC_DGT_ALARM, m_dgt_alarm);
	DDX_Control(pDX, IDC_MSG_STOP, m_msg_stop);
	DDX_Control(pDX, IDC_MSG_RUN, m_msg_run);
	DDX_Control(pDX, IDC_MSG_ALARM, m_msg_alarm);
	DDX_Control(pDX, IDC_MSG_MTBI, m_msg_mtbi);
	DDX_Control(pDX, IDC_GROUP_TIME_INFO, m_group_time_info);
	DDX_Control(pDX, IDC_DGT_STOP, m_dgt_stop);
	DDX_Control(pDX, IDC_DGT_RUN, m_dgt_run);
	DDX_Control(pDX, IDC_DGT_MTBI, m_dgt_mtbi);
	DDX_Control(pDX, IDC_GROUP_LOT_INFO, m_group_lot_info);
	DDX_Control(pDX, IDC_GROUP_LOT_YIELD_INFO, m_group_lot_yield_info);
	DDX_Control(pDX, IDC_GROUP_B_LOT_INFO, m_group_b_lot_info);
	DDX_Control(pDX, IDC_GROUP_DAILY_YIELD_INFO, m_group_daily_yield_info);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScreen_Main, CFormView)
	//{{AFX_MSG_MAP(CScreen_Main)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DOOR_OPEN, OnBtnDoorOpen)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN1, OnBtnBufferIn1)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN2, OnBtnBufferIn2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN3, OnBtnBufferIn3)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN4, OnBtnBufferIn4)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN5, OnBtnBufferIn5)
	ON_BN_CLICKED(IDC_BTN_MOVE_CV1, OnBtnMoveCv1)
	ON_BN_CLICKED(IDC_BTN_MOVE_CV2, OnBtnMoveCv2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN6, OnBtnBufferIn6)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN7, OnBtnBufferIn7)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN8, OnBtnBufferIn8)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN9, OnBtnBufferIn9)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN10, OnBtnBufferIn10)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT1, OnBtnBufferOut1)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT2, OnBtnBufferOut2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT3, OnBtnBufferOut3)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT4, OnBtnBufferOut4)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT5, OnBtnBufferOut5)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT6, OnBtnBufferOut6)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT7, OnBtnBufferOut7)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT8, OnBtnBufferOut8)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT9, OnBtnBufferOut9)
	ON_BN_CLICKED(IDC_BTN_BUFFER_OUT10, OnBtnBufferOut10)
	ON_BN_CLICKED(IDC_BTN_MOVE_CV3, OnBtnMoveCv3)
	ON_BN_CLICKED(IDC_BTN_MOVE_CV4, OnBtnMoveCv4)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP1, OnBtnBufferSkip1)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP2, OnBtnBufferSkip2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP3, OnBtnBufferSkip3)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP4, OnBtnBufferSkip4)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP5, OnBtnBufferSkip5)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP6, OnBtnBufferSkip6)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP7, OnBtnBufferSkip7)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP8, OnBtnBufferSkip8)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP9, OnBtnBufferSkip9)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP10, OnBtnBufferSkip10)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP11, OnBtnBufferSkip11)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP12, OnBtnBufferSkip12)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP13, OnBtnBufferSkip13)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP14, OnBtnBufferSkip14)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP15, OnBtnBufferSkip15)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP16, OnBtnBufferSkip16)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP17, OnBtnBufferSkip17)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP18, OnBtnBufferSkip18)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP19, OnBtnBufferSkip19)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SKIP20, OnBtnBufferSkip20)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BTN_DISPLAY, OnBtnDisplay)
	ON_BN_CLICKED(IDC_CHECK_FRONT_READY, OnCheckFrontReady)
	ON_BN_CLICKED(IDC_CHK_BTM_READY, OnChkBtmReady)
	ON_BN_CLICKED(IDC_CHK_DENSITY_PICKPLACE, OnChkDensityPickplace)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BTN_LABEL, OnBtnLabel)
	ON_BN_CLICKED(IDC_BTN_BCR, OnBtnBcr)
	ON_BN_CLICKED(IDC_BTN_BCR_CCS, OnBtnBcrCcs)
	ON_BN_CLICKED(IDC_BTN_LABEL_CCS, OnBtnLabelCcs)
	ON_BN_CLICKED(IDC_BTN_XCLOSE, OnBtnXclose)
	ON_BN_CLICKED(IDC_BTN_ID_BUFFER_INPUT, OnBtnIdBufferInput)
	ON_BN_CLICKED(IDC_BUTTON29, OnButton29)
	ON_BN_CLICKED(IDC_BUTTON30, OnButton30)
	ON_BN_CLICKED(IDC_BUTTON31, OnButton31)
	ON_BN_CLICKED(IDC_BUTTON32, OnButton32)
	ON_BN_CLICKED(IDC_MSG_RUN, OnMsgRun)
	ON_BN_CLICKED(IDC_BTN_NETWORK, OnBtnNetwork)
	ON_BN_CLICKED(IDC_BTN_CONVEY_RESET, OnBtnConveyReset)
	ON_BN_CLICKED(IDC_BTN_XGEM, OnBtnXgem)
	ON_BN_CLICKED(IDC_BTN_DENSITY1, OnBtnDensity1)
	ON_BN_CLICKED(IDC_BTN_DENSITY2, OnBtnDensity2)
	ON_BN_CLICKED(IDC_BTN_DENSITY3, OnBtnDensity3)
	ON_BN_CLICKED(IDC_BTN_DENSITY4, OnBtnDensity4)
	ON_BN_CLICKED(IDC_BTN_DENSITY5, OnBtnDensity5)
	ON_BN_CLICKED(IDC_BTN_DENSITY6, OnBtnDensity6)
	ON_BN_CLICKED(IDC_BTN_BCR1_1, OnBtnBcr11)
	ON_BN_CLICKED(IDC_BTN_BCR1_2, OnBtnBcr12)
	ON_BN_CLICKED(IDC_BTN_BCR2_1, OnBtnBcr21)
	ON_BN_CLICKED(IDC_BTN_BCR2_2, OnBtnBcr22)
	ON_BN_CLICKED(IDC_BTN_RIN1_1, OnBtnRin11)
	ON_BN_CLICKED(IDC_BTN_RIN1_2, OnBtnRin12)
	ON_BN_CLICKED(IDC_BTN_RIN2_1, OnBtnRin21)
	ON_BN_CLICKED(IDC_BTN_RIN2_2, OnBtnRin22)
	ON_BN_CLICKED(IDC_BUTTON_MODEL, OnButtonModel)
	//}}AFX_MSG_MAP
 	ON_MESSAGE(WM_WORK_END, OnMain_Work_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
	ON_MESSAGE( WM_DRAW_UI_MAIN, OnMain_Draw_UI)
 	ON_MESSAGE(SSM_CLICK, OnCellClick)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main diagnostics

#ifdef _DEBUG
void CScreen_Main::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Main::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main message handlers

void CScreen_Main::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	int i = 0;

	OnMain_Display_Lot_Info();
	OnMain_Display_Lot_Yield_info();
	OnMain_Display_Daily_Yield_info();
	//2017\7.0510
	for( i = 0; i < 7; i++)
		OnMain_Display_BuffrInfo(i);
	////2014,1217 ysj
	for( i = 0; i < 5; i++)
		OnMain_Display_BTM_BuffrInfo(i);
	////

	OnMain_Init_Door();
	OnMain_Init_NetUsable();

	OnMain_Init_Lot_Info();
	OnMain_Init_B_Lot_Info();
	OnMain_Init_Lot_Yield_info();
	OnMain_Init_Daily_Yield_info();
	OnMain_Init_SMema();
	OnMain_Init_Btm();

	OnMain_Display_NetUsable();
	OnMain_GroupBox_Set();
	OnMain_Label_Set();
	OnMain_Digital_Count_Set();
	OnMain_Display_Smema();
	OnMain_Display_Front_Btm();

	OnMain_Draw_UI( 0, 0 );

	SetTimer(1234, 500, 0);
	//2017.0510
// 	SetTimer(STATE_DISPLAY, 500, 0);//2016.1210

	////2014.0821
	Init_Animation();
	SetTimer(TM_ANIMATION,100,NULL);
	////
	OnIDBufferInfoInit();
	OnBTMIDBufferInfoInit();//2014,1217

	OnMain_Init_BCR_Info();
	OnMain_Display_BCR_Info();

	mn_time_select = 0;	

	st_handler.cwnd_main = this;		// MAIN 화면 핸들 설정

// 	st_handler.mn_d_piclplce = 1;
	m_chk_dpickplace.SetCheck(st_handler.mn_d_piclplce);
	
}

void CScreen_Main::OnDestroy() 
{
	st_handler.cwnd_main = NULL;		// MAIN 화면 핸들 초기화
	KillTimer(1234);
	KillTimer(STATE_DISPLAY);
	
	while(1)
	{
		if( st_work.b_run_slmulation_md == false )
		{
			KillTimer(TM_ANIMATION);//2014.0821
			break;
		}
	}

	CFormView::OnDestroy();
}

void CScreen_Main::OnTimer(UINT nIDEvent) 
{
	int nResponse = CTLBD_RET_GOOD;
	
	switch(nIDEvent)
	{
	case 1234:
		{
			OnMain_Display_Door();
			OnMain_Display_Smema();
			OnMain_Display_Front_Btm();
		}
		break;
	case TM_ANIMATION:
		{
			st_work.b_run_slmulation_md = true;
			UpdateScreens();
// 			if(st_work.b_run_slmulation_md == TRUE)
// 			{
// 				UpdateMotorMove();
// 			}
// 			else
// 			{
// 				if(st_work.nEqpStatus == dRUN)
// 				{
// 					UpdateMotorMove();
// 				}
// 			}
			st_work.b_run_slmulation_md = false;

		}
		break;

	case STATE_DISPLAY:
// 		OnMain_Display_BuffrInfo(0);
// 		OnMain_Display_BuffrInfo(1);
// 		OnMain_Display_BuffrInfo(2);
// 		OnMain_Display_BuffrInfo(3);
// 		OnMain_Display_BuffrInfo(4);
// 		OnMain_Display_BuffrInfo(5);
		//2017.0510
// 		if( g_Run_IDBuff_Input_Index.mn_run_step != 4000 || 
// 			g_Run_IDBuff_Input_Index.mn_run_step != 6000 ||
// 			g_Run_IDBuff_Move_Index.mn_run_step != 4000 )
// 		{
// 			OnMain_Display_BuffrInfo(0);
// 			OnMain_Display_BuffrInfo(1);
// 			OnMain_Display_BuffrInfo(2);
// 			OnMain_Display_BuffrInfo(3);
// 			OnMain_Display_BuffrInfo(4);
// 			OnMain_Display_BuffrInfo(5);
// 		}
		//OnMain_Display_BuffrInfo(6);
// 		OnMain_Display_BTM_BuffrInfo(0);
// 		OnMain_Display_BTM_BuffrInfo(1);
// 		OnMain_Display_BTM_BuffrInfo(2);
// 		OnMain_Display_BTM_BuffrInfo(3);
// 		OnMain_Display_BTM_BuffrInfo(4);
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CScreen_Main::OnMain_GroupBox_Set()
{
	CSxLogFont main_font(13, FW_THIN, false, "Bitstream Vera Sans Mono");

	m_group_lot_info.SetFont(main_font);
	m_group_lot_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_lot_info.SetFontBold(TRUE);

	m_group_lot_yield_info.SetFont(main_font);
	m_group_lot_yield_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_lot_yield_info.SetFontBold(TRUE);

	m_group_daily_yield_info.SetFont(main_font);
	m_group_daily_yield_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_daily_yield_info.SetFontBold(TRUE);

	m_group_b_lot_info.SetFont(main_font);
	m_group_b_lot_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_b_lot_info.SetFontBold(TRUE);
	
	m_group_time_info.SetFont(main_font);
	m_group_time_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_time_info.SetFontBold(TRUE);

	m_group_bcr_label_info.SetFont(main_font);
	m_group_bcr_label_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_bcr_label_info.SetFontBold(TRUE);

	m_group_front.SetFont(main_font);
	m_group_front.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_front.SetFontBold(TRUE);
	
	m_group_rear.SetFont(main_font);
	m_group_rear.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_rear.SetFontBold(TRUE);

	m_group_unload_conveyor.SetFont(main_font);
	m_group_unload_conveyor.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_unload_conveyor.SetFontBold(TRUE);
	
	m_group_density_error.SetFont(main_font);
	m_group_density_error.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_density_error.SetFontBold(TRUE);
	
	m_group_use_barcode.SetFont(main_font);
	m_group_use_barcode.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_use_barcode.SetFontBold(TRUE);

	m_group_tact_info.SetFont(main_font);
	m_group_tact_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_tact_info.SetFontBold(TRUE);
}

void CScreen_Main::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::GridControl(UINT nID, int type, int row, int col, int pos)
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
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}

	Grid->SetCellType(col,row,&CellType);

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::GridFont(UINT nID, int row, int col, int size)
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

void CScreen_Main::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Label_Set()
{
	m_msg_alarm.SetFont(Func.mp_main_font);
	m_msg_alarm.SetWindowText(_T("Alarm"));
	m_msg_alarm.SetCenterText();
	m_msg_alarm.SetColor(RGB(0, 0, 0));
	m_msg_alarm.SetGradientColor(RGB(255, 255, 0));
	m_msg_alarm.SetTextColor(RGB(255, 255, 255));
	
	m_msg_run.SetFont(Func.mp_main_font);
	m_msg_run.SetWindowText(_T("Run"));
	m_msg_run.SetCenterText();
	m_msg_run.SetColor(RGB(0, 0, 0));
	m_msg_run.SetGradientColor(RGB(0, 0, 255));
	m_msg_run.SetTextColor(RGB(255, 255, 255));
	
	m_msg_idle.SetFont(Func.mp_main_font);
	m_msg_idle.SetWindowText(_T("Run Down"));
	m_msg_idle.SetCenterText();
	m_msg_idle.SetColor(RGB(0, 0, 0));
	m_msg_idle.SetGradientColor(RGB(0, 0, 255));
	m_msg_idle.SetTextColor(RGB(255, 255, 255));
	
	m_msg_change.SetFont(Func.mp_main_font);
	m_msg_change.SetWindowText(_T("Change"));
	m_msg_change.SetCenterText();
	m_msg_change.SetColor(RGB(0, 0, 0));
	m_msg_change.SetGradientColor(RGB(0, 0, 255));
	m_msg_change.SetTextColor(RGB(255, 255, 255));
	
	m_msg_stop.SetFont(Func.mp_main_font);
	m_msg_stop.SetWindowText(_T("Stop"));
	m_msg_stop.SetCenterText();
	m_msg_stop.SetColor(RGB(0, 0, 0));
	m_msg_stop.SetGradientColor(RGB(255, 0, 0));
	m_msg_stop.SetTextColor(RGB(255, 255, 255));
	
	m_msg_mtbi.SetFont(Func.mp_main_font);
	m_msg_mtbi.SetWindowText(_T("MTBI"));
	m_msg_mtbi.SetCenterText();
	m_msg_mtbi.SetColor(RGB(0, 0, 0));
	m_msg_mtbi.SetGradientColor(ORANGE_C);
	m_msg_mtbi.SetTextColor(RGB(255, 255, 255));
	
	m_msg_maint.SetFont(Func.mp_main_font);
	m_msg_maint.SetWindowText(_T("MAINT"));
	m_msg_maint.SetCenterText();
	m_msg_maint.SetColor(RGB(0, 0, 0));
	m_msg_maint.SetGradientColor(ORANGE_C);
	m_msg_maint.SetTextColor(RGB(255, 255, 255));

}

void CScreen_Main::OnMain_Digital_Count_Set()
{
	m_dgt_alarm.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_run.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_idle.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_change.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_stop.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_mtbi.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_tact.SetStyle(CDigit::DS_FLOAT3, 7, CDigit::DC_BLACK);
	m_dgt_maint.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
}

void CScreen_Main::OnMain_Time_Display(int n_state)
{
	m_dgt_run.ActivateTimer(FALSE);
	m_dgt_idle.ActivateTimer(FALSE);
	m_dgt_change.ActivateTimer(FALSE);
	m_dgt_stop.ActivateTimer(FALSE);
	m_dgt_alarm.ActivateTimer(FALSE);
	m_dgt_mtbi.ActivateTimer(FALSE);
	m_dgt_maint.ActivateTimer(FALSE);
	
	mn_time_select = 0;
	switch(n_state)
	{
	case dRUN:
		m_dgt_run.ActivateTimer();
		m_dgt_idle.ActivateTimer();
		m_dgt_change.ActivateTimer();
		m_dgt_mtbi.ActivateTimer();
		if (mn_time_select == 0)
		{
			m_dgt_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
			m_dgt_idle.SetTime(st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds());
			m_dgt_change.SetTime(st_handler.m_tDC.GetTotalHours(), st_handler.m_tDC.GetMinutes(), st_handler.m_tDC.GetSeconds());
			int nSec = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();
			if( g_handler.GetAlarmCnt() == 0 )
				nSec = nSec;
			else
			{
				nSec /= g_handler.GetAlarmCnt();
			}
			
			m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
		}
		else
		{
			m_dgt_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
			m_dgt_idle.SetTime(st_handler.m_tI.GetTotalHours(), st_handler.m_tI.GetMinutes(), st_handler.m_tI.GetSeconds());
			m_dgt_change.SetTime(st_handler.m_tC.GetTotalHours(), st_handler.m_tDC.GetMinutes(), st_handler.m_tC.GetSeconds());
			int nSec = st_handler.m_tR.GetTotalHours() * 3600 + st_handler.m_tR.GetMinutes() * 60 + st_handler.m_tR.GetSeconds();
			if( g_handler.GetAlarmCnt() == 0 )
				nSec = nSec;
			else
			{
				nSec /= g_handler.GetAlarmCnt();
			}
			
			m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
		}
		break;
		
	case dWARNING:
	case dJAM:
		m_dgt_alarm.ActivateTimer();
		if (mn_time_select == 0)
		{
			m_dgt_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
		}
		else
		{
			m_dgt_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
		}
		break;
		
	case dLOCK:
	case dSTOP:
		m_dgt_stop.ActivateTimer();
		m_dgt_maint.ActivateTimer();
		m_dgt_alarm.ActivateTimer();
		m_dgt_idle.ActivateTimer();
		if (mn_time_select == 0)
		{
			m_dgt_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
			m_dgt_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
			m_dgt_maint.SetTime(st_handler.m_tDM.GetTotalHours(), st_handler.m_tDM.GetMinutes(), st_handler.m_tDM.GetSeconds());
			m_dgt_idle.SetTime(st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds());
		}
		else
		{
			m_dgt_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
			m_dgt_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
			m_dgt_maint.SetTime(st_handler.m_tM.GetTotalHours(), st_handler.m_tM.GetMinutes(), st_handler.m_tM.GetSeconds());
			m_dgt_idle.SetTime(st_handler.m_tI.GetTotalHours(), st_handler.m_tI.GetMinutes(), st_handler.m_tI.GetSeconds());
		}
		break;
	}
	m_dgt_run.UpdateTimer();
	m_dgt_idle.UpdateTimer();
	m_dgt_change.UpdateTimer();
	m_dgt_stop.UpdateTimer();
	m_dgt_alarm.UpdateTimer();
	m_dgt_mtbi.UpdateTimer();
	m_dgt_mtbi.UpdateTimer();
}

void CScreen_Main::OnMain_Tact_Display()
{
	float tact = (float)st_handler.mn_unloadtact;
	tact /= 1000;
// 	m_dgt_tact.SetVal(st_handler.mn_unloadtact);
	m_dgt_tact.SetFloatVal(tact);
}

void CScreen_Main::OnMain_TimeInfo_Display()
{
	m_dgt_run.ActivateTimer(FALSE);
	m_dgt_idle.ActivateTimer(FALSE);
	m_dgt_change.ActivateTimer(FALSE);
	m_dgt_stop.ActivateTimer(FALSE);
	m_dgt_alarm.ActivateTimer(FALSE);
	m_dgt_mtbi.ActivateTimer(FALSE);
	m_dgt_maint.ActivateTimer(FALSE);
	
	
	if (mn_time_select == 0)
	{
		m_dgt_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
		m_dgt_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
		m_dgt_idle.SetTime(st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds());
		m_dgt_maint.SetTime(st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds());
		m_dgt_change.SetTime(st_handler.m_tDC.GetTotalHours(), st_handler.m_tDC.GetMinutes(), st_handler.m_tDC.GetSeconds());
		
		int nSec = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();
		if( g_handler.GetAlarmCnt() == 0 )
			nSec = nSec;
		else
		{
			nSec /= g_handler.GetAlarmCnt();
		}
		
		m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
	}
	else
	{
		m_dgt_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
		m_dgt_idle.SetTime(st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds());
		m_dgt_maint.SetTime(st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds());
		m_dgt_change.SetTime(st_handler.m_tDC.GetTotalHours(), st_handler.m_tDC.GetMinutes(), st_handler.m_tDC.GetSeconds());
		
		int nSec = st_handler.m_tR.GetTotalHours() * 3600 + st_handler.m_tR.GetMinutes() * 60 + st_handler.m_tR.GetSeconds();
		if( g_handler.GetAlarmCnt() == 0 )
			nSec = nSec;
		else
		{
			nSec /= g_handler.GetAlarmCnt();
		}
		
		m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
	}
	
	m_dgt_run.UpdateTimer();
	m_dgt_idle.UpdateTimer();
	m_dgt_change.UpdateTimer();
	m_dgt_stop.UpdateTimer();
	m_dgt_alarm.UpdateTimer();
	m_dgt_mtbi.UpdateTimer();
	m_dgt_maint.UpdateTimer();
}

LRESULT CScreen_Main::OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam) 
{
	int mn_state = wParam;
	
	switch(mn_state)
	{//2017.0510	
		case MAIN_IDBUFF_INPUT_INDEX:
 			OnMain_Display_BuffrInfo(0);
			break;
		case MAIN_IDBUFF_INDEX_DENSITY:
 			OnMain_Display_BuffrInfo(1);
			break;
		case MAIN_IDBUFF_INDEX_BCR1:
 			OnMain_Display_BuffrInfo(2);
			break;
		case MAIN_IDBUFF_INDEX_BCR2:
 			OnMain_Display_BuffrInfo(3);
			break;
		case MAIN_IDBUFF_INDEX_TOP:
 			OnMain_Display_BuffrInfo(4);
			break;
		case MAIN_IDBUFF_INDEX_ULD:
			OnMain_Display_BuffrInfo(5);
			break;
		////
		case MAIN_TIMEINFO:
			OnMain_Time_Display(lParam);
			OnMain_Tact_Display();
			break;
			
		case MAIN_COUNTINFO:
			OnMain_Display_Lot_Yield_info();
			OnMain_Display_Daily_Yield_info();
			break;

		case MAIN_LOTINFO:
			OnMain_Display_Lot_Info();
		    break;
		////2014,1217
		case MAIN_IDBUFF_INDEX_ULD_BTM:
			OnMain_Display_BTM_BuffrInfo(4);
			break;
		case MAIN_IDBUFF_INDEX_ULD_CLAMP:
			OnMain_Display_BTM_BuffrInfo(3);
			break;
		case MAIN_IDBUFF_INDEX_ULD_LOAD: 
			OnMain_Display_BTM_BuffrInfo(2);
			break;
		case MAIN_IDBUFF_INDEX_ULD_UNLOAD:
			OnMain_Display_BTM_BuffrInfo(1);
			break;
		case MAIN_IDBUFF_INDEX_ULD_NEXT: 
			OnMain_Display_BTM_BuffrInfo(0);
			break;
		////
		default:
			break;
	}

	return 0;
}

LRESULT CScreen_Main::OnMain_Draw_UI(WPARAM wParam,LPARAM lParam)
{
	m_btn_door_open.SetWindowText( g_local.Get( "L_U_0000") );
	return 0;
}


void CScreen_Main::OnBtnDoorOpen() 
{
	int nRet, nRet2;
	
// 	g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_ON);
// 	
// 	if( st_handler.cwnd_list != NULL)
// 	{
// 		sprintf(st_msg.c_normal_msg, "[Density] vision trigger on..");
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 	}
	
	nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);

	if (nRet == IO_ON && nRet2 == IO_ON)
	{
		if( st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "Manual Mode가 아닙니다.");
			if ( g_local.GetLocalType() == LOCAL_ENG ) sprintf(st_msg.c_abnormal_msg, "It's not manual mode.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			return;
		}
	}

	if (COMI.mn_run_status != dSTOP)
	{
		if( st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "장비가 Stop 상태가 아닙니다..");
			if ( g_local.GetLocalType() == LOCAL_ENG ) sprintf(st_msg.c_abnormal_msg, "The Machine is not Stop.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			return ;
		}
	} 

	////2015.0114
	g_ioMgr.set_out_bit(st_io.o_door_lock, IO_OFF);
// 	Func.OnOpenDoor();
	////

// 	FAS_IO.Set_IO_DefineWord(0, 3, 0, 0xe000);
// 	Func.OnDailyDataSave();

}

////2014.0821
void CScreen_Main::Init_Animation()
{
	CRect rect(0,0,0,0);
	GetDlgItem(IDC_STATIC_ANIVIEW)->GetClientRect(&rect);	
	m_Animate.Create( NULL, WS_VISIBLE | WS_CHILD,rect,GetDlgItem(IDC_STATIC_ANIVIEW), NULL ) ;
	m_Animate.InitAniWnd();
// 	m_Animate.ActionDisplay();
	
	if(m_bExeExcute != 1) 
	{ 
		for(int i=0; i < 10; i++) 
		{	
			st_ani.nSelectMotPos[i] = -1; 
		}	
		m_bExeExcute = TRUE; 
	}
	
	st_work.b_run_slmulation_md = FALSE;
}

void CScreen_Main::UpdateScreens()
{
	if(!CScreen_Main::IsWindowVisible())
		return;
	
	m_Animate.ActionDisplay();
}

void CScreen_Main::UpdateMotorMove()
{
	if(!CScreen_Main::IsWindowVisible())
		return;

//	double dPosTmp,dPosTmp2;
//	double dMaxPos, dMinPos;//, dMotRate;

//	int n_Work1Allow, n_Work2Allow, n_Work3Allow, n_Work4Allow;
//	int n_MDRbtAllow, n_LHSRbtAllow, n_RHSRbtAllow, n_ClipRbtAllow, n_ClampRbtAllow, n_SorterRbtAllow, n_ULDRbtAllow;

// 	if(COMI.mn_simulation_mode == 1)
// 	{
	 	for(int i=0; i < 10; i++)
		{
// 			dMinPos	= st_teach[i].d_limit_position[i][0];
// 			dMaxPos	= st_teach[i].d_limit_position[i][1];
			
			switch(i)
			{	
			case 0://ok  
				{
// 					dMotRate = /*dMaxPos*/ 1000 / 1000;
// 					
// 					dPosTmp = (int) (st_teach[M_WORK_BUFFER_1].md_pos[st_ani.nSelectMotPos[M_WORK_BUFFER_1]] / dMotRate);
// 
// 					if(dPosTmp == abs(m_Animate.m_Workbuffer1.m_dEqCurPos_X))
// 					{
// 						break;
// 					}					 
// 					else
// 					{
// 						if(m_Animate.m_Workbuffer1.m_dEqCurPos_X > dPosTmp) 
// 						{
// 							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X - 10;
// 
// 							if(dPosTmp > m_Animate.m_Workbuffer1.m_dEqCurPos_X)
// 							{
// 								n_Work1Allow = dPosTmp - m_Animate.m_Workbuffer1.m_dEqCurPos_X;
// 
// 								m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + n_Work1Allow;
// 								break;
// 							}
// 						}
// 						else 
// 						{
// 							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + 10;
// 						}
// 					}
				}
				break;


			case 1://ok  
				{
// 					dMotRate = /*dMaxPos*/ 3000 / 1000;
// 					
// 					dPosTmp = (int) (st_motor[M_M_RBT_Y].md_pos[st_ani.nSelectMotPos[M_M_RBT_Y]] / dMotRate);
// 
// 					dPosTmp = -dPosTmp;
// 					
// 					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y == dPosTmp)
// 					{
// 						break;
// 					}					 
// 					else
// 					{
// 						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y > dPosTmp) 
// 						{
// 							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y - 10;
// 							
// 							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Y)
// 							{
// 								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Y;
// 
// 								m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + n_MDRbtAllow;
// 								break;
// 							}
// 						}
// 						else 
// 						{
// 							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + 10;
// 						}
// 					}
				}
				break;

			case 2://ok  
				{
// 					dMotRate = /*dMaxPos*/ 25000 / 1000;
// 					
// 					dPosTmp = (int) (st_motor[M_M_RBT_Z].md_pos[st_ani.nSelectMotPos[M_M_RBT_Z]] / dMotRate);
// 					
// 					dPosTmp = -dPosTmp;
// 					
// 					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z == dPosTmp)
// 					{
// 						break;
// 					}					 
// 					else
// 					{
// 						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z > dPosTmp) 
// 						{
// 							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z - 1;
// 							
// 							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Z)
// 							{
// 								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Z;
// 								
// 								m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + n_MDRbtAllow;
// 								break;
// 							}
// 						}
// 						else 
// 						{
// 							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + 1;
// 						}
// 					}
				}
				break;

			}
		}
// 	}
// 	else
// 	{
// 		//모터 엔코더 값 표시
// 		for(int i=0; i < MAXMOTOR; i++)
// 		{
// 			dMinPos	= st_teach[i].d_limit_position[i][0];
// 			dMaxPos	= st_teach[i].d_limit_position[i][1];
// 			
// 			switch(i)
// 			{	
// 			case 0://ok  
// 				{
// // 					dMotRate = /*dMaxPos*/ 1000 / 1000;
// // 					
// // 					dPosTmp2 = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
// // 
// // 					dPosTmp = (int) (dPosTmp2 / dMotRate);
// // 
// // 					if(dPosTmp == abs(m_Animate.m_Workbuffer1.m_dEqCurPos_X))
// // 					{
// // 						break;
// // 					}					 
// // 					else
// // 					{
// // 						if(m_Animate.m_Workbuffer1.m_dEqCurPos_X > dPosTmp) 
// // 						{
// // 							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X - 4;
// // 
// // 							if(dPosTmp > m_Animate.m_Workbuffer1.m_dEqCurPos_X)
// // 							{
// // 								n_Work1Allow = dPosTmp - m_Animate.m_Workbuffer1.m_dEqCurPos_X;
// // 
// // 								m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + n_Work1Allow;
// // 								break;
// // 							}
// // 						}
// // 						else 
// // 						{
// // 							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + 4;
// // 						}
// // 					}
// 				}
// 				break;
// 
// 
// 			case 1://ok  
// 				{
// // 					dMotRate = /*dMaxPos*/ 3000 / 1000;
// // 
// // 					dPosTmp2 = COMI.Get_MotCurrentPos(M_M_RBT_Y);
// // 					
// // 					dPosTmp = (int) (dPosTmp2 / dMotRate);
// // 
// // 					dPosTmp = -dPosTmp;
// // 					
// // 					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y == dPosTmp)
// // 					{
// // 						break;
// // 					}					 
// // 					else
// // 					{
// // 						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y > dPosTmp) 
// // 						{
// // 							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y - 4;
// // 							
// // 							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Y)
// // 							{
// // 								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Y;
// // 
// // 								m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + n_MDRbtAllow;
// // 								break;
// // 							}
// // 						}
// // 						else 
// // 						{
// // 							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + 4;
// // 						}
// // 					}
// 				}
// 				break;
// 
// 			case 2://ok  
// 				{
// // 					dMotRate = /*dMaxPos*/ 25000 / 1000;
// // 
// // 					dPosTmp2 = COMI.Get_MotCurrentPos(M_M_RBT_Z);
// // 					
// // 					dPosTmp = (int) (dPosTmp2 / dMotRate);
// // 					
// // 					dPosTmp = -dPosTmp;
// // 					
// // 					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z == dPosTmp)
// // 					{
// // 						break;
// // 					}					 
// // 					else
// // 					{
// // 						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z > dPosTmp) 
// // 						{
// // 							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z - 1;
// // 							
// // 							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Z)
// // 							{
// // 								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Z;
// // 								
// // 								m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + n_MDRbtAllow;
// // 								break;
// // 							}
// // 						}
// // 						else 
// // 						{
// // 							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + 1;
// // 						}
// // 					}
// 				}
// 				break;
// 			}
// 		}
// 	}
}
////

void CScreen_Main::OnMain_Init_Lot_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 6;
	int col = 2;
	int i, j;

	//2014,0514
	CString str_tmp[6] = {"Pro.Name", "Lot No", "Part No", "Last Mdl", "Total", "Capa"/*"ByPass"*/};
	CString str_tmp2[6] = {"Pro.Name", "Lot No", "Part No", "Last Mdl", "Total", "Bcr Dir"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 16);
  			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 50);
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, YELLOW_L, BLACK_C);

				GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_LOT_INFO, i, j, 14);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 120);
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, BLACK_C, WHITE_C);

				GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 0);

				if (i == 3)
				{
					GridFont(IDC_CUSTOM_LOT_INFO, i, j, 10);
				}
				else
				{
					GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);
				}
			}
		}
	}


		for (i = 1; i < row + 1; i++)
		{
			GridData(IDC_CUSTOM_LOT_INFO, i, 1, str_tmp[i - 1]);
		}
	
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Init_B_Lot_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 2;
	int col = 2;
	int i, j;
	
	CString str_tmp[2] = {"Lot No", "Part No"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_B_LOT_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 24);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 50);
				GridColor(IDC_CUSTOM_B_LOT_INFO, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_B_LOT_INFO, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_B_LOT_INFO, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 120);
				GridColor(IDC_CUSTOM_B_LOT_INFO, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_B_LOT_INFO, STATIC, i, j, 0);
				
				if (i == 2)
				{
					GridFont(IDC_CUSTOM_B_LOT_INFO, i, j, 13);
				}
				else
				{
					GridFont(IDC_CUSTOM_B_LOT_INFO, i, j, 15);
				}
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_B_LOT_INFO, i, 1, str_tmp[i - 1]);
	}
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_Lot_Info()
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		//2015.0613
		GridData(IDC_CUSTOM_LOT_INFO, 1, 2, g_lotMgr.GetLotAt(0).GetProductName() );
		GridData(IDC_CUSTOM_LOT_INFO, 2, 2, g_lotMgr.GetLotIDAt( 0 ) );
		GridData(IDC_CUSTOM_LOT_INFO, 3, 2, g_lotMgr.GetLotAt(0).GetPartID() );
		GridData(IDC_CUSTOM_LOT_INFO, 4, 2, g_lotMgr.GetLotAt(0).GetStrLastModule());
		GridData(IDC_CUSTOM_LOT_INFO, 5, 2, g_lotMgr.GetLotAt(0).GetStrTotalModule());
		GridData(IDC_CUSTOM_LOT_INFO, 6, 2, g_lotMgr.GetLotAt(0).GetDenLabelCapa());
		////2014,1125 ysj
		GridData(IDC_CUSTOM_BCR_INFO, 1, 2, st_handler.m_strDensityLabel[st_basic.mn_device_type]);
		if( st_var.n_using_barcode == 0 )
		{
			GridData(IDC_CUSTOM_BCR_INFO, 2, 2, st_handler.m_strDistributionLabel[0]);
			GridData(IDC_CUSTOM_BCR_INFO, 3, 2, st_handler.m_strDistributionRibbon[0]);
		}
		else
		{
			GridData(IDC_CUSTOM_BCR_INFO, 2, 2, st_handler.m_strDistributionLabel[2]);
			GridData(IDC_CUSTOM_BCR_INFO, 3, 2, st_handler.m_strDistributionRibbon[2]);
		}
		////
	}
	else
	{
		GridData(IDC_CUSTOM_LOT_INFO, 1, 2, "" );
		GridData(IDC_CUSTOM_LOT_INFO, 2, 2, "" );
		GridData(IDC_CUSTOM_LOT_INFO, 3, 2, "");
		GridData(IDC_CUSTOM_LOT_INFO, 4, 2, "");
		GridData(IDC_CUSTOM_LOT_INFO, 5, 2, "");
		GridData(IDC_CUSTOM_LOT_INFO, 6, 2, "");
		////2014,1125 ysj
		GridData(IDC_CUSTOM_BCR_INFO, 1, 2, "");
		GridData(IDC_CUSTOM_BCR_INFO, 2, 2, "");
		GridData(IDC_CUSTOM_BCR_INFO, 3, 2, "");
		////
	}

	if( g_lotMgr.GetLotCount() > 1 )
	{
		GridData( IDC_CUSTOM_B_LOT_INFO, 1, 2, g_lotMgr.GetLotIDAt( 1 ) );
		GridData( IDC_CUSTOM_B_LOT_INFO, 2, 2, g_lotMgr.GetLotAt( 1 ).GetPartID() );
	}
	else
	{
		GridData( IDC_CUSTOM_B_LOT_INFO, 1, 2, "" );
		GridData( IDC_CUSTOM_B_LOT_INFO, 2, 2, "" );
	}

	m_group_lot_info.ShowWindow( TRUE );
	m_group_b_lot_info.ShowWindow( TRUE );
	m_group_daily_yield_info.ShowWindow( TRUE );
	m_group_lot_yield_info.ShowWindow( TRUE );
	m_group_bcr_label_info.ShowWindow(TRUE);//2014,1125 ysj
		
}

void CScreen_Main::OnMain_Display_Lot_Yield_info()
{	//st_work.nMdlInputCount
	CString strTemp;
	int nLotNum = -1;
	
	// Prime
	strTemp.Format( "%06d", g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) );
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 2, 2, strTemp);		// Input
	strTemp.Format( "%06d", g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) );
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 3, 2, strTemp);		// Pass
	strTemp.Format( "%06d", g_lotMgr.GetLotAt(0).GetFailCnt(PRIME) );
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 4, 2, strTemp);		// Reject

	double dYield = 0;
	if (g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) > 0)
	{
		dYield = (double)g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) / (double)g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) * 100.0f;
	}

	strTemp.Format("%3.2f%%", dYield);
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 5, 2, strTemp);		// Yield

	//2013,0913
	if(st_handler.m_strProcessID == "SMFA")
	{
		for (int i = 0; i < g_lotMgr.GetLotCount(); i++)
		{
			nLotNum = g_lotMgr.IsGetPLotID(g_lotMgr.GetLotIDAt(i));
			if( nLotNum >= 0 && nLotNum < MAX_PLOT)
			{
				st_work.nPLotInputCount[nLotNum][PRIME] = g_lotMgr.GetLotAt(i).GetInputCnt(PRIME);
				st_work.nPLotPassCount[nLotNum][PRIME] = g_lotMgr.GetLotAt(i).GetPassCnt(PRIME);
				st_work.nPLotFailCount[nLotNum][PRIME] = g_lotMgr.GetLotAt(i).GetFailCnt(PRIME);
				st_work.fPLotYield[nLotNum][PRIME] = dYield;
			}
		}
	}


	// Cum
	strTemp.Format("%06d", g_lotMgr.GetLotAt(0).GetInputCnt(CUM));
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 2, 3, strTemp);		// Input
	strTemp.Format("%06d", g_lotMgr.GetLotAt(0).GetPassCnt(CUM));
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 3, 3, strTemp);		// Pass
	strTemp.Format("%06d", g_lotMgr.GetLotAt(0).GetFailCnt(CUM));
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 4, 3, strTemp);		// Reject

	dYield = 0;
	if (g_lotMgr.GetLotAt(0).GetInputCnt(CUM) > 0)
	{
		dYield = (double)g_lotMgr.GetLotAt(0).GetPassCnt(CUM) / (double)g_lotMgr.GetLotAt(0).GetInputCnt(CUM) * 100.0f;
	}
	
	strTemp.Format("%3.2f%%", dYield);

	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 5, 3, strTemp);		// Yield

	//2013,0913
	if(st_handler.m_strProcessID == "SMFA")
	{
		for (int i = 0; i < g_lotMgr.GetLotCount(); i++)
		{
			nLotNum = g_lotMgr.IsGetPLotID(g_lotMgr.GetLotIDAt(i));
			if( nLotNum >= 0 && nLotNum < MAX_PLOT)
			{
				st_work.nPLotInputCount[nLotNum][CUM] = g_lotMgr.GetLotAt(i).GetInputCnt(CUM);
				st_work.nPLotPassCount[nLotNum][CUM] = g_lotMgr.GetLotAt(i).GetPassCnt(CUM);
				st_work.nPLotFailCount[nLotNum][CUM] = g_lotMgr.GetLotAt(i).GetFailCnt(CUM);
				st_work.fPLotYield[nLotNum][CUM] = dYield;
			}
		}
	}
}	

void CScreen_Main::OnMain_Display_Daily_Yield_info()
{
	CString strTemp;
	// Prime
	strTemp.Format( "%06d", g_lotMgr.GetInputCnt(PRIME) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 2, 2, strTemp);		// Input
	strTemp.Format( "%06d", g_lotMgr.GetPassCnt(PRIME) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 3, 2, strTemp);		// Pass
	strTemp.Format( "%06d", g_lotMgr.GetFailCnt(PRIME) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 4, 2, strTemp);		// Reject
	
	double dYield = 0;
	if (g_lotMgr.GetInputCnt(PRIME) > 0)
	{
		dYield = (double)g_lotMgr.GetPassCnt(PRIME) / (double)g_lotMgr.GetInputCnt(PRIME) * 100.0f;
	}
	strTemp.Format("%3.2f%%", dYield);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 5, 2, strTemp);		// Yield
	
	// Retest
	strTemp.Format( "%06d", g_lotMgr.GetInputCnt(CUM) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 2, 3, strTemp);		// Input
	strTemp.Format( "%06d", g_lotMgr.GetPassCnt(CUM) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 3, 3, strTemp);		// Pass
	strTemp.Format( "%06d", g_lotMgr.GetFailCnt(CUM) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 4, 3, strTemp);		// Reject

	dYield = 0;
	if (g_lotMgr.GetInputCnt(CUM) > 0)
	{
		dYield = (double)g_lotMgr.GetPassCnt(CUM) / (double)g_lotMgr.GetInputCnt(CUM) * 100.0f;
	}

	strTemp.Format("%3.2f%%", dYield);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 5, 3, strTemp);		// Yield
}

void CScreen_Main::OnMain_Init_Lot_Yield_info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 3;
	int i, j;
	
	CString str_tmp[4] = {"Input", "Pass", "Fail", "Yield"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_YIELD_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 55);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 58);
			}
			
			if (i == 1 && j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, BLACK_C, GREEN_B);
				}
			}
			
			GridControl(IDC_CUSTOM_LOT_YIELD_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_LOT_YIELD_INFO, i, j, 15);
		}
	}
	
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 1, " ");
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 2, "Prime");
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 3, "Cum");
	
	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_LOT_YIELD_INFO, i, 1, str_tmp[i - 2]);
	}
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Init_Daily_Yield_info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 3;
	int i, j;
	
	CString str_tmp[4] = {"Input", "Pass", "Fail", "Yield"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DAILY_YIELD_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 55);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 58);
			}
			
			if (i == 1 && j == 1)
			{
				GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, BLACK_C, GREEN_B);
				}
			}
			GridControl(IDC_CUSTOM_DAILY_YIELD_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, 15);
		}
	}
	
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 1, " ");
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 2, "Prime");
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 3, "Cum");
	
	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_DAILY_YIELD_INFO, i, 1, str_tmp[i - 2]);
	}
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Init_Door()
{
	TSpread* Grid;
	int row = 5;
	int col = 2;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_DOOR );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for( int ih=0; ih<row; ih++ )
	{
		Grid -> SetRowHeightInPixels(ih + 1, 12);

		for( int iw = 0; iw<col; iw++ )
		{
			Grid -> SetColWidthInPixels(iw + 1, 64 );
//			Grid ->SetFont(iw + 1, ih + 1, font, TRUE);
			GridFont( IDC_CUSTOM_DOOR, ih + 1, iw + 1, 14);
			GridControl(IDC_CUSTOM_DOOR, STATIC, ih + 1, iw + 1, 0);
//			GridFont( IDC_CUSTOM_DOOR, ih + 1, iw + 1, 7);
		}
	}

	GridData( IDC_CUSTOM_DOOR, 1, 1, "DOOR 1" );
	GridData( IDC_CUSTOM_DOOR, 1, 2, "DOOR 2" );
	GridData( IDC_CUSTOM_DOOR, 2, 1, "DOOR 3" );
	GridData( IDC_CUSTOM_DOOR, 2, 2, "DOOR 4" );	
	GridData( IDC_CUSTOM_DOOR, 3, 1, "DOOR 5" );
	GridData( IDC_CUSTOM_DOOR, 3, 2, "DOOR 6" );
	GridData( IDC_CUSTOM_DOOR, 4, 1, "DOOR 7" );
	GridData( IDC_CUSTOM_DOOR, 4, 2, "DOOR 8" );	
	GridData( IDC_CUSTOM_DOOR, 5, 1, "DOOR 9" );
	GridData( IDC_CUSTOM_DOOR, 5, 2, "DOOR 10" );

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_Door()
{
	COLORREF col = RED_C;
	if( g_ioMgr.get_in_bit( st_io.i_door1_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 1, 1, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door2_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 1, 2, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door3_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 2, 1, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door4_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 2, 2, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door5_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 3, 1, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door6_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 3, 2, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door7_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 4, 1, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door8_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 4, 2, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door9_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 5, 1, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_door10_chk, IO_OFF ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 5, 2, col, BLACK_C );
}


void CScreen_Main::OnMain_Init_NetUsable()
{
	TSpread *Grid;
	int row = 4;
	int col = 2;
//	int i;

	int iGrid = IDC_CUSTOM_NET_USABLE;
	Grid = (TSpread*)GetDlgItem(iGrid);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);


	for( int ih=0; ih<row; ih++ )
	{
		Grid -> SetRowHeightInPixels(ih + 1, 25);

		for( int iw = 0; iw<col; iw++ )
		{
			Grid -> SetColWidthInPixels(iw + 1, 98 );
			
			GridFont( IDC_CUSTOM_NET_USABLE, ih + 1, iw + 1, 16);
			GridControl(IDC_CUSTOM_NET_USABLE, STATIC, ih + 1, iw + 1, 0);
		}
	}

//	Grid -> SetColWidthInPixels(1, 120);
//	for (i = 1; i <= row; i++)
//	{
//		Grid -> SetRowHeightInPixels(i, 12);
//		GridControl(iGrid, STATIC, i, 1, 0);
//		GridFont(iGrid, i, 1, 9);
//		GridColor(iGrid, i, 1, BLUE_L, BLACK_C);
//	}
	GridData(iGrid, 1, 1, "EC Server");
	GridData(iGrid, 1, 2, "Front Machine");
	GridData(iGrid, 2, 1, "Next Machine");
	GridData(iGrid, 2, 2, "Barcode1");
	GridData(iGrid, 3, 1, "Barcode2");
	GridData(iGrid, 3, 2, "Barcode3");
	GridData(iGrid, 4, 1, "Barcode4");
	GridData(iGrid, 4, 2, "XGEM");

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_NetUsable()
{
	for (int i = 1; i <= 4; i++)
	{
		GridColor(IDC_CUSTOM_NET_USABLE, i, 1, BLUE_L, BLACK_C);
		GridColor(IDC_CUSTOM_NET_USABLE, i, 2, BLUE_L, BLACK_C);
	}

//	if( g_client_ec.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 1, 1, RED_C, BLACK_C );
//
//	if( g_client_front.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 4, 1, RED_C, BLACK_C );
//
//	if( g_client_next.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 4, 1, RED_C, BLACK_C );
//
//	if( g_client_next.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 5, 1, RED_C, BLACK_C );
	
//	if( g_client_bpc.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 2, 1, RED_C, BLACK_C );

//	if( g_client_ec.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 3, 1, RED_C, BLACK_C );


	//2013,1028
//	if( g_client_xgem.GetUsable() == false )
//		GridColor( IDC_CUSTOM_NET_USABLE, 9, 1, RED_C, BLACK_C );
}

void CScreen_Main::OnCellClick( WPARAM wParam, LPARAM lParam )
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	CDialog_Message msg_dlg;
	CDialog_KeyPad	pad_dlg;
	CDialog_IDBuffer_Info Buffer_Info;
	CDialog_Btm_IDBuffer_Info Btm_Buffer_Info;


	TSpread* Grid;
	int mn_row;
	int mn_col;
	int nResponse;

	if( wParam == IDC_CUSTOM_LOT_INFO || wParam == IDC_CUSTOM_BCR_INFO)
	{
		if( lpcc->Col == 1 && lpcc->Row == 1 )
		{
			CalcPopupMenuByClick( wParam );
		}
	}
	else if (wParam == IDC_CUSTOM_BARCODE_ERROR)
	{
		//if(COMI.mn_run_status != dSTOP) return;//2015.0501
		
		if (lpcc->Row == 0) return;
		int mn_response = 0;
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_BARCODE_ERROR);
		mn_row	= lpcc->Row;
		mn_col	= lpcc->Col;
		
		CDialog_Select select_dlg;
		if(mn_row == 1)
		{
			if(mn_col == 1 && st_var.n_use_barcode[0] == EMBS_SKIP)
			{
				st_msg.mstr_confirm_msg = _T("Do you want to release Barocde#1? ");
				mn_response = select_dlg.DoModal();				
				if (mn_response == IDOK)
					st_var.n_use_barcode[0] = EMBS_NONE;
			}
			if(mn_col == 2 && st_var.n_use_barcode[1] == EMBS_SKIP)
			{
				st_msg.mstr_confirm_msg = _T("Do you want to release Barocde#2? ");
				mn_response = select_dlg.DoModal();				
				if (mn_response == IDOK)
					st_var.n_use_barcode[1] = EMBS_NONE;
			}
		}
		else if(mn_row == 2)
		{
			if(mn_col == 1 && st_var.n_use_barcode[2] == EMBS_SKIP)
			{
				st_msg.mstr_confirm_msg = _T("Do you want to release Barocde#3? ");
				mn_response = select_dlg.DoModal();				
				if (mn_response == IDOK)
					st_var.n_use_barcode[2] = EMBS_NONE;
			}
			if(mn_col == 2 && st_var.n_use_barcode[3] == EMBS_SKIP)
			{
				st_msg.mstr_confirm_msg = _T("Do you want to release Barocde#4? ");
				mn_response = select_dlg.DoModal();				
				if (mn_response == IDOK)
					st_var.n_use_barcode[3] = EMBS_NONE;
			}
		}
		if (Grid != NULL)
		{
			Grid = NULL;
			delete Grid;
		}
	}
	else if (wParam == IDC_CUSTOM_ID_BUFFER_INFO)
	{
		if(COMI.mn_run_status == dRUN) return;//2014,1217 ysj
		if (lpcc->Row == 0) return;

		return; //2016.1210 Can't modify. check resource error
// 		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO);
// 		mn_row	= lpcc->Row;
// 		mn_col	= lpcc->Col;
// 		
// 		if(mn_row == 1)
// 		{
// 			if (mn_col == 2 || mn_col == 3 || mn_col == 4 || mn_col == 5 || mn_col == 6 ||
// 				mn_col == 7 || mn_col == 8 || mn_col == 9 || mn_col == 10 || mn_col == 11 ||
// 				mn_col == 12 || mn_col == 13 || mn_col == 14 || mn_col == 15)
// 			{	
// 				////2014.1110
// 				if(mn_col == 2 || mn_col == 3) st_work.n_IDBuffer_Info = 0;
// 				if(mn_col == 4 || mn_col == 5) st_work.n_IDBuffer_Info = 1;
// 				if(mn_col == 6 || mn_col == 7) st_work.n_IDBuffer_Info = 2;
// 				if(mn_col == 8 || mn_col == 9) st_work.n_IDBuffer_Info = 3;
// 				if(mn_col == 10 || mn_col == 11) st_work.n_IDBuffer_Info = 4;
// 				if(mn_col == 12 || mn_col == 13) st_work.n_IDBuffer_Info = 5;
// 				if(mn_col == 14 || mn_col == 15) st_work.n_IDBuffer_Info = 6;
// 				////
// 				nResponse = Buffer_Info.DoModal();
// 				if(nResponse == IDOK)
// 				{
// 					OnMain_Display_BuffrInfo(st_work.n_IDBuffer_Info);
// 				}
// 				st_work.n_IDBuffer_Info = -1;//2014.1110
// 				//st_work.n_Btm_IDBuffer_Info = -1;//2014,1217 ysj
// 			}
// 			else
// 			{
// 				return;
// 			}
// 		}
// 		else
// 		{
// 			return;
// 		}
	}
	////2014,1217 ysj
	else if (wParam == IDC_CUSTOM_ID_BUFFER_INFO2)
	{
		if(COMI.mn_run_status == dRUN) return;
		if (lpcc->Row == 0) return;

		return; //2016.1210 Can't modify. check resource error

// 		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO2);
// 		mn_row	= lpcc->Row;
// 		mn_col	= lpcc->Col;
// 		
// 		if(mn_row == 1)
// 		{
// 			if (mn_col == 2 || mn_col == 3 || mn_col == 4 || mn_col == 5 || mn_col == 6 ||
// 				mn_col == 7 || mn_col == 8 || mn_col == 9 || mn_col == 10 || mn_col == 11)
// 			{	
// 				////2014.1110
// 				if(mn_col == 2 || mn_col == 3) st_work.n_Btm_IDBuffer_Info = 4;
// 				if(mn_col == 4 || mn_col == 5) st_work.n_Btm_IDBuffer_Info = 3;
// 				if(mn_col == 6 || mn_col == 7) st_work.n_Btm_IDBuffer_Info = 2;
// 				if(mn_col == 8 || mn_col == 9) st_work.n_Btm_IDBuffer_Info = 1;
// 				if(mn_col == 10 || mn_col == 11) st_work.n_Btm_IDBuffer_Info = 0;
// 				////
// 				nResponse = Btm_Buffer_Info.DoModal();
// 				if(nResponse == IDOK)
// 				{
// 					OnMain_Display_BTM_BuffrInfo(st_work.n_Btm_IDBuffer_Info);
// 				}
// 				//st_work.n_IDBuffer_Info = -1;//2014.1110
// 				st_work.n_Btm_IDBuffer_Info = -1;
// 			}
// 			else
// 			{
// 				if (Grid != NULL)
// 				{
// 					Grid = NULL;
// 					delete Grid;
// 				}
// 				return;
// 			}
// 		}
// 		else
// 		{
// 			if (Grid != NULL)
// 			{
// 				Grid = NULL;
// 				delete Grid;
// 			}
// 			return;
// 		}
// 
// 		if (Grid != NULL)
// 		{
// 			Grid = NULL;
// 			delete Grid;
// 		}
	}

}

void CScreen_Main::CalcPopupMenuByClick( int iCustom )
{
	int nResponse;//2014,1125

	//	if( iCustom != IDC_CUSTOM_LOT_INFO)
	if( iCustom != IDC_CUSTOM_LOT_INFO && iCustom != IDC_CUSTOM_BCR_INFO)//2014,1125
	{
		return;
	}
	
	if( iCustom == m_iClick && 
		GetCurrentTime() - m_dwTimeClicked < 1000 )
	{
		m_iClickCnt++;	
	}
	else
	{
		m_iClickCnt = 1;
	}
	
	if( m_iClickCnt >= 3 )
	{
		switch( iCustom )
		{
		case IDC_CUSTOM_LOT_INFO:	
			{
				CDialog_Data_Lot dlgDataLot;
				dlgDataLot.DoModal();
			}
			break;
			
		case IDC_CUSTOM_BCR_INFO:
			{
				CDialog_HandBarcode dlg_hand;
				nResponse = dlg_hand.DoModal();
				if(nResponse == IDOK)
				{
// 					GridData(IDC_CUSTOM_BCR_INFO, 1, 2, st_handler.m_strDensityLabel[0]);// st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].m_cDen_capa);
// 					GridData(IDC_CUSTOM_BCR_INFO, 2, 2, st_handler.m_strLabelCode[0]);//2014,11216 st_bcrinfo[0].m_cLabelcode);
// 					GridData(IDC_CUSTOM_BCR_INFO, 3, 2, st_handler.m_strRibbonCode[0]);//2014,11216 st_bcrinfo[0].m_cRibboncode);
					GridData(IDC_CUSTOM_BCR_INFO, 1, 2, st_handler.m_strDensityLabel[st_basic.mn_device_type]);
					if( st_var.n_using_barcode == 0 )
					{
						GridData(IDC_CUSTOM_BCR_INFO, 2, 2, st_handler.m_strDistributionLabel[0]);
						GridData(IDC_CUSTOM_BCR_INFO, 3, 2, st_handler.m_strDistributionRibbon[0]);
					}
					else
					{
						GridData(IDC_CUSTOM_BCR_INFO, 2, 2, st_handler.m_strDistributionLabel[2]);
						GridData(IDC_CUSTOM_BCR_INFO, 3, 2, st_handler.m_strDistributionRibbon[2]);
					}
				}				
			}
			break;
		}
		m_iClickCnt = 0;
	}
	
	m_iClick = iCustom;
	m_dwTimeClicked = GetCurrentTime();
}

// void CScreen_Main::OnSetfocusEditSerial() 
// {
// 	// TODO: Add your control notification handler code here
// 	
// 
// 	int mn_response;
// 
// 	UpdateData(TRUE);
// 	CDialog_HandBarcode HandBarcode_dlg;
// 
// 	mn_response = HandBarcode_dlg.DoModal();
// 
// 	if(mn_response == IDOK)
// 	{
// 		SetDlgItemText(IDC_EDIT_SERIAL, "Sucesse");
// 	}
// 	else
// 	{
// 		SetDlgItemText(IDC_EDIT_SERIAL, "Fail");
// 	}
// 
// }


void CScreen_Main::OnBtnMoveCv1() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Move_CV1.m_bGlass[0] = TRUE;
}

void CScreen_Main::OnBtnMoveCv2() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Move_CV1.m_bGlass[0] = FALSE;
}

void CScreen_Main::OnBtnMoveCv3() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Move_CV1.m_bGlass[1] = TRUE;		
}

void CScreen_Main::OnBtnMoveCv4() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Move_CV1.m_bGlass[1] = FALSE;	
}

void CScreen_Main::OnBtnBufferIn1() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bGlass[0] = TRUE;
	m_Animate.m_ID_Buffer1.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferIn2() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bGlass[0] = TRUE;
	m_Animate.m_ID_Buffer2.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferIn3() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bGlass[0] = TRUE;
	m_Animate.m_ID_Buffer3.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferIn4() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bGlass[0] = TRUE;
	m_Animate.m_ID_Buffer4.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferIn5() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bGlass[0] = TRUE;
	m_Animate.m_ID_Buffer5.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferIn6() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bGlass[1] = TRUE;	
	m_Animate.m_ID_Buffer1.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferIn7() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bGlass[1] = TRUE;	
	m_Animate.m_ID_Buffer2.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferIn8() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bGlass[1] = TRUE;	
	m_Animate.m_ID_Buffer3.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferIn9() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bGlass[1] = TRUE;
	m_Animate.m_ID_Buffer4.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferIn10() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bGlass[1] = TRUE;	
	m_Animate.m_ID_Buffer5.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferOut1() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferOut2() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferOut3() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferOut4() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferOut5() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferOut6() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bGlass[1] = FALSE;	
}

void CScreen_Main::OnBtnBufferOut7() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bGlass[1] = FALSE;			
}

void CScreen_Main::OnBtnBufferOut8() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bGlass[1] = FALSE;			
}

void CScreen_Main::OnBtnBufferOut9() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bGlass[1] = FALSE;			
}

void CScreen_Main::OnBtnBufferOut10() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bGlass[1] = FALSE;		
}

void CScreen_Main::OnBtnBufferSkip1() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bSkip[0] = TRUE;
	m_Animate.m_ID_Buffer1.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip2() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bSkip[0] = TRUE;
	m_Animate.m_ID_Buffer2.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip3() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bSkip[0] = TRUE;
	m_Animate.m_ID_Buffer3.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip4() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bSkip[0] = TRUE;
	m_Animate.m_ID_Buffer4.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip5() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bSkip[0] = TRUE;
	m_Animate.m_ID_Buffer5.m_bGlass[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip6() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip7() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bSkip[0] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip8() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip9() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip10() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bSkip[0] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip11() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bSkip[1] = TRUE;
	m_Animate.m_ID_Buffer1.m_bGlass[1] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip12() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bSkip[1] = TRUE;	
	m_Animate.m_ID_Buffer2.m_bGlass[1] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip13() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bSkip[1] = TRUE;
	m_Animate.m_ID_Buffer3.m_bGlass[1] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip14() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bSkip[1] = TRUE;
	m_Animate.m_ID_Buffer4.m_bGlass[1] = FALSE;	
}

void CScreen_Main::OnBtnBufferSkip15() //2014,1125 ysj
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bSkip[1] = TRUE;
	m_Animate.m_ID_Buffer5.m_bGlass[1] = FALSE;	
}


void CScreen_Main::OnBtnBufferSkip16() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer1.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip17() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer2.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip18() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer3.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip19() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer4.m_bSkip[1] = FALSE;
}

void CScreen_Main::OnBtnBufferSkip20() 
{
	// TODO: Add your control notification handler code here
	m_Animate.m_ID_Buffer5.m_bSkip[1] = FALSE;
}


void CScreen_Main::OnButtonStart() 
{
	COMI.mn_run_status = dRUN;//테스트
	
}

void CScreen_Main::OnButtonStop() 
{
// 	COMI.mn_run_status = dSTOP;//A×½º
	
// 	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_1, SERVER_CONNECT, 0); // EC SERVER1
	int iIdx = g_Tray.GetIdxByPos( IPOOS_UNLOAD_BTM_LOAD );
	if( iIdx < 0 )											return;

	g_Tray.TrayMoved( EIDBUFFER_POS(IPOOS_UNLOAD_BTM_LOAD), EIDBUFFER_POS(IDPOS_NEXT) );

	return;

	EIDBUFFER_POS eTargetPos = IDPOS_NONE;
	if( g_Tray.IsEnablePos( IPOOS_UNLOAD_BTM_UNLOAD ) )
	{ 
		eTargetPos = IPOOS_UNLOAD_BTM_UNLOAD;
	}
	
	//if( eTargetPos != TPOS_NONE )
	//{
		ATray tray;
		tray.SetPos( IPOOS_UNLOAD_BTM_UNLOAD );
		tray.SetTrayInfo( g_lotMgr.GetLotIDAt(0), 0, false, false );
		
		for( int i=0; i<MAX_MODULE; i++ )
		{
			CString strProductID = "mSATA";
			
			CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
			CString strSkip = "1";
			CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
			CString strSerial;		strSerial.Format( "S11ZNYAC500%d%02d", eTargetPos - IDPOS_DENSITY, i );
			CString strPPID =		"NONE";//OnNetworkBodyAnalysis( strPocket[i], "PPID" );
			CString strWWN =		"NONE";//OnNetworkBodyAnalysis( strPocket[i], "WWN" );
			CString strCSerial =	"NONE";//OnNetworkBodyAnalysis( strPocket[i], "C_SERIAL" );
			CString strPSID =		"NONE";//OnNetworkBodyAnalysis( strPocket[i], "PSID" );
			CString strBIN =		"";
			CString strVar = "1";
			
			// 모듈 셋
			AModule mdl;
			mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
			tray.SetModule(i, mdl);
		}
		
		tray.SetTargetPos( eTargetPos );
		g_Tray.AddTray( tray );
		//g_Tray.TrayMoved( IDPOS_FRONT_TO_DENSITY, eTargetPos );
		
		//FUNCTION_RPY=BUFFER_IN_RPY LOT_ID=ABCD PART_ID=EFGH BUFFER_SKIP=(BUF_01=NO BUF_02=NO) 
		//EQPID=SSD_LABELLER BYPASS=NO LAST_MDL=YES MDL_CNT=2 MD_01=(BUF SERIAL=1234567, …… …) MD_02=(BUF= SERIAL=1234567, …… …) 보낸다
		
	//}
	
}

void CScreen_Main::OnBtnDisplay() 
{
	CDialog_Data_Module dlgDataModule;
	dlgDataModule.DoModal();
	
}

void CScreen_Main::OnCheckFrontReady() 
{
	CString mstr_temp="";
	
	st_handler.mn_front_ready = !st_handler.mn_front_ready;
	
	m_chk_front_ready.SetCheck(st_handler.mn_front_ready);
	
	mstr_temp.Format("%d", st_handler.mn_front_ready);

}

void CScreen_Main::OnChkBtmReady() 
{
	CString mstr_temp="";
	
	st_handler.mn_btm_ready = !st_handler.mn_btm_ready;
	
	m_chk_btm_ready.SetCheck(st_handler.mn_btm_ready);
	
	mstr_temp.Format("%d", st_handler.mn_btm_ready);
	
}

void CScreen_Main::OnChkDensityPickplace() 
{
	CString mstr_temp="";
	
	st_handler.mn_d_piclplce = !st_handler.mn_d_piclplce;
	
	m_chk_dpickplace.SetCheck(st_handler.mn_d_piclplce);
	
	mstr_temp.Format("%d", st_handler.mn_d_piclplce);
	
}

void CScreen_Main::OnButton20() 
{
// 	ALabel* lal1[2];
// 	ALabel* lal2[2];
// 	lal1[0] = &Func.m_pSite[SITE_DENSITY]->GetLabel( st_basic.mn_device_type );
// 	lal2[0] = &Func.m_pSite[SITE_DENSITY]->GetLabel( st_basic.mn_device_type +3 );
// 
// 	lal2[1] = lal2[0];
// 
// 	CString strLotID = "1234";
// 	CString strCapa = "256";
// 	CString str2D = "312";
// 	EMODULE_BCR_STATE ebs = EMBS_LOAD;
// 
// 	ALabel lal3;
// 	lal3.SetLabelInfo(strLotID, str2D, strCapa );
// 	*lal1[0] = lal3;
// 	*lal2[0] = lal3;
// 	
// 	ALabel* pLdl_1;
// 	ALabel* pLdl_2;
// 	pLdl_1 = &Func.m_pSite[SITE_DENSITY]->GetLabel( st_basic.mn_device_type );
// 	pLdl_2 = &Func.m_pSite[SITE_DENSITY]->GetLabel( st_basic.mn_device_type + 3 );
// 
// 	Func.m_pRobot[SITE_DENSITY]->GetPicker(0).SetLabel( *pLdl_1 );
// 	Func.m_pRobot[SITE_DENSITY]->GetPicker(1).SetLabel( *pLdl_2 );
// 	pLdl_1->EraseData(); 
// 	pLdl_2->EraseData(); 

}

void CScreen_Main::OnMain_Init_SMema()
{
	// Front
	TSpread* Grid;
	int row = 3;
	int col = 1;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_SMEMA_FRONT );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	Grid->SetColWidthInPixels( 1, 90 );
	for( int ih=0; ih<row; ih++ )
	{
		Grid->SetRowHeightInPixels(ih + 1, 18);

		GridFont( IDC_CUSTOM_SMEMA_FRONT, ih + 1, 1, 17);
		GridControl(IDC_CUSTOM_SMEMA_FRONT, STATIC, ih + 1, 1, 0);
	}

	GridData( IDC_CUSTOM_SMEMA_FRONT, 1, 1, "F Ready" );
	GridData( IDC_CUSTOM_SMEMA_FRONT, 2, 1, "R Req" );
	GridData( IDC_CUSTOM_SMEMA_FRONT, 3, 1, "R Trans" );

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}

	row = 2;
	col = 2;

	// rear
	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_SMEMA_REAR );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for( ih=0; ih<row; ih++ )
	{
		for (int j = 0; j < col; j++)
		{
			Grid->SetColWidthInPixels( ih +1, 90 );
			Grid->SetRowHeightInPixels(ih + 1, 17);
			Grid->SetColWidthInPixels(j+1, 92);
			
			GridFont( IDC_CUSTOM_SMEMA_REAR, ih + 1, j + 1, 17);
			GridControl(IDC_CUSTOM_SMEMA_REAR, STATIC, ih + 1, j + 1, 0);

		}
	}

	GridData( IDC_CUSTOM_SMEMA_REAR, 1, 1, "F Ready" );
	GridData( IDC_CUSTOM_SMEMA_REAR, 2, 1, "R Req" );

	GridData( IDC_CUSTOM_SMEMA_REAR, 1, 2, "F Enter" );
	GridData( IDC_CUSTOM_SMEMA_REAR, 2, 2, "R Trans Req" );

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
	

	row = 2;
	col = 2;
	
	// rear
	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_UNLOAD_CONVEYOR );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for( ih=0; ih<row; ih++ )
	{
		for (int j = 0; j < col; j++)
		{
			Grid->SetColWidthInPixels( ih +1, 90 );
			Grid->SetRowHeightInPixels(ih + 1, 17);
			Grid->SetColWidthInPixels(j+1, 92);
			
			GridFont( IDC_CUSTOM_UNLOAD_CONVEYOR, ih + 1, j + 1, 17);
			GridControl(IDC_CUSTOM_UNLOAD_CONVEYOR, STATIC, ih + 1, j + 1, 0);
			
		}
	}
	
	GridData( IDC_CUSTOM_UNLOAD_CONVEYOR, 1, 1, "CV Left" );
	GridData( IDC_CUSTOM_UNLOAD_CONVEYOR, 2, 1, "CV Right" );
	
	GridData( IDC_CUSTOM_UNLOAD_CONVEYOR, 1, 2, "CV Full" );
	GridData( IDC_CUSTOM_UNLOAD_CONVEYOR, 2, 2, "CV Out" );
	
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}

	row = 2;
	col = 3;
	
	// rear
	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_DENSITY_ERROR );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for( ih=0; ih<row; ih++ )
	{
		for (int j = 0; j < col; j++)
		{
			Grid->SetColWidthInPixels( ih +1, 60 );
			Grid->SetRowHeightInPixels(ih + 1, 17);
			Grid->SetColWidthInPixels(j+1, 62);
			
			GridFont( IDC_CUSTOM_DENSITY_ERROR, ih + 1, j + 1, 17);
			GridControl(IDC_CUSTOM_DENSITY_ERROR, STATIC, ih + 1, j + 1, 0);
			
		}
	}
	
	GridData( IDC_CUSTOM_DENSITY_ERROR, 1, 1, "  1  " );
	GridData( IDC_CUSTOM_DENSITY_ERROR, 1, 2, "  2  " );	
	GridData( IDC_CUSTOM_DENSITY_ERROR, 1, 3, "  3  " );	
	
	GridData( IDC_CUSTOM_DENSITY_ERROR, 2, 1, "  4  " );
	GridData( IDC_CUSTOM_DENSITY_ERROR, 2, 2, "  5  " );
	GridData( IDC_CUSTOM_DENSITY_ERROR, 2, 3, "  6  " );
	
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}

	// rear
	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_BARCODE_ERROR );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for( ih=0; ih<row; ih++ )
	{
		for (int j = 0; j < col; j++)
		{
			Grid->SetColWidthInPixels( ih +1, 90 );
			Grid->SetRowHeightInPixels(ih + 1, 17);
			Grid->SetColWidthInPixels(j+1, 92);
			
			GridFont( IDC_CUSTOM_BARCODE_ERROR, ih + 1, j + 1, 17);
			GridControl(IDC_CUSTOM_BARCODE_ERROR, STATIC, ih + 1, j + 1, 0);
			
		}
	}
	
	GridData( IDC_CUSTOM_BARCODE_ERROR, 1, 1, "  1  " );
	GridData( IDC_CUSTOM_BARCODE_ERROR, 1, 2, "  2  " );	
	
	GridData( IDC_CUSTOM_BARCODE_ERROR, 2, 1, "  3  " );
	GridData( IDC_CUSTOM_BARCODE_ERROR, 2, 2, "  4  " );
	
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_Smema()
{
	// front
	COLORREF col = GREEN_C;	if( g_ioMgr.get_in_bit( st_io.i_front_top_machine_ready_signal_chk ) == IO_OFF )		col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_SMEMA_FRONT, 1, 1, col, BLACK_C );

	col = RGB(255,128,64);				if( g_ioMgr.get_out_bit( st_io.o_front_top_machine_enter_signal, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_SMEMA_FRONT, 2, 1, col, BLACK_C );

	col = RGB(255,128,64);				if( g_ioMgr.get_out_bit( st_io.o_front_top_machine_transfer_signal, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_SMEMA_FRONT, 3, 1, col, BLACK_C );

	// rear
	col = RGB(255,128,64);		if( g_ioMgr.get_out_bit( st_io.o_rear_ready_signal, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_SMEMA_REAR, 1, 1, col, BLACK_C );

	col = GREEN_C;				if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk ) == IO_OFF )		col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_SMEMA_REAR, 2, 1, col, BLACK_C );

	col = RGB(255,128,64);		if( g_ioMgr.get_out_bit( st_io.o_rear_enter_signal, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_SMEMA_REAR, 1, 2, col, BLACK_C );
	
	col = GREEN_C;				if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk ) == IO_OFF )		col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_SMEMA_REAR, 2, 2, col, BLACK_C );

	// Conveyor
	col = RGB(255,128,64);		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_UNLOAD_CONVEYOR, 1, 1, col, BLACK_C );
	
	col = GREEN_C;				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk ) == IO_OFF )		col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_UNLOAD_CONVEYOR, 2, 1, col, BLACK_C );
	
	col = RGB(255,128,64);		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_UNLOAD_CONVEYOR, 1, 2, col, BLACK_C );
	
	col = GREEN_C;				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk ) == IO_OFF )		col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_UNLOAD_CONVEYOR, 2, 2, col, BLACK_C );

	// Density Error
	col = RGB(255,128,64);		if( st_var.n_err_density_flag[0] == CTL_NO && g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[0], IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_DENSITY_ERROR, 1, 1, col, BLACK_C );
	
	col = RGB(255,128,64);		if( st_var.n_err_density_flag[1] == CTL_NO && g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[1], IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_DENSITY_ERROR, 1, 2, col, BLACK_C );
	
	col = RGB(255,128,64);		if( st_var.n_err_density_flag[2] == CTL_NO && g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[2], IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_DENSITY_ERROR, 1, 3, col, BLACK_C );
	
	col = GREEN_C;				if( st_var.n_err_density_flag[3] == CTL_NO && g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[3], IO_ON ) == IO_OFF )	col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_DENSITY_ERROR, 2, 1, col, BLACK_C );	
	
	col = GREEN_C;				if( st_var.n_err_density_flag[4] == CTL_NO && g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[4], IO_ON ) == IO_OFF )	col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_DENSITY_ERROR, 2, 2, col, BLACK_C );
	
	col = GREEN_C;				if( st_var.n_err_density_flag[5] == CTL_NO && g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[5], IO_ON ) == IO_OFF )	col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_DENSITY_ERROR, 2, 3, col, BLACK_C );
	
	// Barcode Error
	col = RGB(255,128,64);		if( st_var.n_use_barcode[0] != EMBS_SKIP )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_BARCODE_ERROR, 1, 1, col, BLACK_C );
	
	col = RGB(255,128,64);		if( st_var.n_use_barcode[1] != EMBS_SKIP )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_BARCODE_ERROR, 1, 2, col, BLACK_C );
	
	col = GREEN_C;				if( st_var.n_use_barcode[2] != EMBS_SKIP )	col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_BARCODE_ERROR, 2, 1, col, BLACK_C );	
	
	col = GREEN_C;				if( st_var.n_use_barcode[3] != EMBS_SKIP )	col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_BARCODE_ERROR, 2, 2, col, BLACK_C );

}

void CScreen_Main::OnMain_Init_Btm()
{
	TSpread* Grid;
	int row = 3;
	int col = 1;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_SMEMA_FRONT_BTM );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	Grid->SetColWidthInPixels( 1, 90 );
	for( int ih=0; ih<row; ih++ )
	{
		Grid->SetRowHeightInPixels(ih + 1, 18);

		GridFont( IDC_CUSTOM_SMEMA_FRONT_BTM, ih + 1, 1, 17);
		GridControl(IDC_CUSTOM_SMEMA_FRONT_BTM, STATIC, ih + 1, 1, 0);
	}

	GridData( IDC_CUSTOM_SMEMA_FRONT_BTM, 1, 1, "F Req" );
	GridData( IDC_CUSTOM_SMEMA_FRONT_BTM, 2, 1, "F Trans" );
	GridData( IDC_CUSTOM_SMEMA_FRONT_BTM, 3, 1, "R Ready" );

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_Front_Btm()
{
	// front
	COLORREF col = GREEN_C;	if( g_ioMgr.get_in_bit( st_io.i_front_btm_machine_request_signal_chk ) == IO_OFF )		col = RGB(0,64,0);
	GridColor( IDC_CUSTOM_SMEMA_FRONT_BTM, 1, 1, col, BLACK_C );

	col = RGB(255,128,64);				if( g_ioMgr.get_out_bit( st_io.i_front_btm_machine_transfer_signal_chk, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_SMEMA_FRONT_BTM, 2, 1, col, BLACK_C );

	col = RGB(255,128,64);				if( g_ioMgr.get_out_bit( st_io.o_front_btm_machine_ready_signal, IO_ON ) == IO_OFF )	col = RGB(64,32,0);
	GridColor( IDC_CUSTOM_SMEMA_FRONT_BTM, 3, 1, col, BLACK_C );

}


////2014.1027
//==================================================================//
// IDBuffer Info 표시
//==================================================================//
void CScreen_Main::OnIDBufferInfoInit()
{
	TSpread *Grid;
	
//	CString sRowTitle[8] = { "", "Lot ID", "Buf ID", "Buffer Using", "Work Cnt", "Product Type", "SSD Exist", "VAR" };
	CString sRowTitle[8] = { "", "Product Type", "Lot ID", "ID Buf", "SSD Serial", "Work Cnt", "Skip", "VAR"};
	CString sColTitle[15] = { "", "Recv", "", "Input", "", "Density", "", "Barcode1", "",  "Barcode2", "",  "Top Vision", "", "Unload", ""};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO);
	
	// 그리드 헤더 미사용
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE); // col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
	// 스크롤 미생성
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
	// 입력 글자수에 맞게 자동 맞춤 그리드 크기 변경 기능 미사용
	Grid->SetBool(SSB_AUTOSIZE, FALSE); // 상하 스크롤바
	// 그리드 갯수 설정
	Grid->SetMaxRows(8);
	Grid->SetMaxCols(15);
	
	int i, j;
	for (i = 0; i < 15;i++)
	{
		GridMerge(IDC_CUSTOM_ID_BUFFER_INFO, 1, i + 2, 1, 2);
		//m_p_grid.GridCellMerge(mp_gdIDBufferInfo, 1, i+2, 1, 2);
	}
	
	for(i=0; i<8; i++)
	{
		Grid->SetRowHeightInPixels(i+1, 22 );
		
		for(j=0; j<15; j++)
		{
			if(i == 0)
			{
				GridData(IDC_CUSTOM_ID_BUFFER_INFO, 1, j+1, sColTitle[j]);
				//m_p_grid.GridCellText(mp_gdIDBufferInfo, 1, j+1, sColTitle[j]);
			}
			if(j == 0)
			{
				Grid->SetColWidthInPixels(j+1, 84);
				GridColor(IDC_CUSTOM_ID_BUFFER_INFO, i + 1, j + 1, BLUE_D, WHITE_C);
			}
			else 
			{
				Grid->SetColWidthInPixels(j+1, 84);
				GridColor(IDC_CUSTOM_ID_BUFFER_INFO, i + 1, j + 1, BLACK_L, YELLOW_L);
			}
			GridControl(IDC_CUSTOM_ID_BUFFER_INFO, STATIC, i+1, j+1, 3);
			//m_p_grid.GridCellFont(mp_gdIDBufferInfo, i+1, j+1, "Tahoma", 13);
			if(i == 4)
			{
				GridFont(IDC_CUSTOM_ID_BUFFER_INFO, i+1, j+1, 10);
			}
			else
			{
				GridFont(IDC_CUSTOM_ID_BUFFER_INFO, i+1, j+1, 13);
			}
		}
		GridFont(IDC_CUSTOM_ID_BUFFER_INFO, i+1, 1, 12);
		GridData(IDC_CUSTOM_ID_BUFFER_INFO, i+1, 1, sRowTitle[i]);
		// 		m_p_grid.GridCellFont(mp_gdIDBufferInfo, i+1, 1, "Tahoma", 12);
		// 		m_p_grid.GridCellText(mp_gdIDBufferInfo, i+1, 1, sRowTitle[i]);
	}
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_BuffrInfo(int nzDisplayBuffer)
{
	CString sUpdateData="", sTmp="";
	int i;
	int nDisplay_PKNo;

//	if(nzDisplayBuffer == 0) return;
	g_cs.EnterCriticalSection();
	switch(nzDisplayBuffer)
	{
		//add start by kwlee 15.01.09
	case 0:
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD_RECV;
		break;
		//add end by kwlee 15.01.09
	case 1:
		//Setplate
		nDisplay_PKNo = SHIFT_IDBUFF_INPUT_INDEX;
		break;
	case 2:
		//Sealing
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_DENSITY;
		break;
	case 3:
		//out
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_BCR1;
		break;
	case 4:
		//out
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_BCR2;
		break;
		
	case 5:
		//out
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_TOP;
		break;
		
	case 6:
		//out
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD;
		break;
	}

	if( nzDisplayBuffer > 6 || nzDisplayBuffer < 0) return;

	TSpread *Grid;


	////////////////////////////////////////////////////////////////////////// Product Type model name
//	CString strProductID =  "PRODUCT_NAME";
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO);
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%s", st_idbuff_info[nDisplay_PKNo].c_product_id[i]);
		Grid->SetValue( 2+  i + (nzDisplayBuffer * 2), 2, sTmp);
	}

	////////////////////////////////////////////////////////////////////////// lot id
//	CString strProduct =  "LOT_ID";
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%s", st_idbuff_info[nDisplay_PKNo].c_lot_id[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 3, sTmp);
	}

	////////////////////////////////////////////////////////////////////////// input id buffer 
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%d", st_idbuff_info[nDisplay_PKNo].n_idbuff_num[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 4, sTmp);
	}

	////////////////////////////////////////////////////////////////////////// ssd serial
// 	CString strProd =  "SERIAL";
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%s", st_idbuff_info[nDisplay_PKNo].c_serial[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 5, sTmp);
	}
	////////////////////////////////////////////////////////////////////////// in cnt

	sUpdateData.Format("%d", st_idbuff_info[nDisplay_PKNo].n_count[0]);
	Grid->SetValue(2+ 0 + (nzDisplayBuffer * 2), 6, sUpdateData);

	sUpdateData.Format("%d", st_idbuff_info[nDisplay_PKNo].n_count[1]);
	Grid->SetValue(2+ 1 + (nzDisplayBuffer * 2), 6, sUpdateData);
	

	////////////////////////////////////////////////////////////////////////// out cnt
//	if(mn_OutCurrent_Count_Work[nzDisplayBuffer][0] != st_idbuff_info[nDisplay_PKNo].n_out_count[0])
//	{
//		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO);
//		mn_OutCurrent_Count_Work[nzDisplayBuffer][0] = st_idbuff_info[nDisplay_PKNo].n_out_count[0];
//		sUpdateData.Format("%d", mn_OutCurrent_Count_Work[nzDisplayBuffer][0]);
//		Grid->SetValue(0 + (nzDisplayBuffer * 2), 7, sUpdateData);
//	}
//	
//	if(mn_OutCurrent_Count_Work[nzDisplayBuffer][1] != st_idbuff_info[nDisplay_PKNo].n_out_count[1])
//	{
//		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO);
//		mn_OutCurrent_Count_Work[nzDisplayBuffer][1] = st_idbuff_info[nDisplay_PKNo].n_out_count[1];
//		sUpdateData.Format("%d", mn_OutCurrent_Count_Work[nzDisplayBuffer][1]);
//		Grid->SetValue(1 + (nzDisplayBuffer * 2), 7, sUpdateData);
//	}
	
	////////////////////////////////////////////////////////////////////////// buffer by pass cnt
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%d", st_idbuff_info[nDisplay_PKNo].n_bypass[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 7, sTmp);
	}

	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%s", st_idbuff_info[nDisplay_PKNo].c_var[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 8, sTmp);
	}

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}

	g_cs.LeaveCriticalSection();

}

void CScreen_Main::OnBtnLabel() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 1);	
	
}

void CScreen_Main::OnBtnBcr() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 2);	
	
}

void CScreen_Main::OnBtnLabelCcs() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 3);	
	
}

void CScreen_Main::OnBtnBcrCcs() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 4);	
	
}

void CScreen_Main::OnBtnXclose() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 5);	
	
}

////2014,1125 ysj
void CScreen_Main::OnMain_Init_BCR_Info()
{
	TSpread *Grid;
	CString str_pos="";
	int row = 3;
	int col = 2;
	int i, j;
	
	CString str_tmp[3] = {"DLabel", "BLabel", "BRibbon"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_BCR_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 24);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 50);
				GridColor(IDC_CUSTOM_BCR_INFO, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_BCR_INFO, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_BCR_INFO, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 120);
				GridColor(IDC_CUSTOM_BCR_INFO, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_BCR_INFO, STATIC, i, j, 0);
				
				if (i == 2)
				{
					GridFont(IDC_CUSTOM_BCR_INFO, i, j, 15);
				}
				else
				{
					GridFont(IDC_CUSTOM_BCR_INFO, i, j, 15);
				}
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_BCR_INFO, i, 1, str_tmp[i - 1]);
	}
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Main::OnMain_Display_BCR_Info()
{
// 	GridData(IDC_CUSTOM_BCR_INFO, 1, 2, st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].m_cDen_capa);
// 	GridData(IDC_CUSTOM_BCR_INFO, 2, 2, st_bcrinfo[0].m_cLabelcode);
// 	GridData(IDC_CUSTOM_BCR_INFO, 3, 2, st_bcrinfo[0].m_cRibboncode);
}

////

void CScreen_Main::OnBtnIdBufferInput() 
{
	// TODO: Add your control notification handler code here
// 	int mn_response;
// 	int i;
// 	
// 	CDialog_HandBarcode HandBarcode_dlg;
// 	
// 	mn_response = HandBarcode_dlg.DoModal();
// 	if(mn_response == IDOK)
// 	{
// 		for(i=0; i<6; i++)
// 		{
// // 			OnDisplayIDBufferInfo(i);
// 		}
// 		for(i=0; i<4; i++)
// 		{
// // 			OnDisplayBTMIDBufferInfo(i);
// 		}
// 		
// 		
// 	}
// 	else
// 	{
// 	}		
}

void CScreen_Main::OnButton29() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 6);	
	
}

void CScreen_Main::OnButton30() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 7);	
	
}

void CScreen_Main::OnButton31() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 8);	
	
}

void CScreen_Main::OnButton32() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 9);	
	
}

void CScreen_Main::OnMsgRun() 
{
	CDialog_NetWork	dlg;
	dlg.DoModal();
}
////2014,1217
//==================================================================//
// IDBuffer Info 표시
//==================================================================//
void CScreen_Main::OnBTMIDBufferInfoInit()
{
	TSpread *Grid;
	
	//	CString sRowTitle[8] = { "", "Lot ID", "Buf ID", "Buffer Using", "Work Cnt", "Product Type", "SSD Exist", "VAR" };
	CString sRowTitle[3] = { "", "ID Buf", "VAR"};
	CString sColTitle[11] = { "", "Next", "", "Unload", "", "Load", "", "Clamp", "", "BTM", ""};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO2);
	
	// 그리드 헤더 미사용
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE); // col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
	// 스크롤 미생성
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
	// 입력 글자수에 맞게 자동 맞춤 그리드 크기 변경 기능 미사용
	Grid->SetBool(SSB_AUTOSIZE, FALSE); // 상하 스크롤바
	// 그리드 갯수 설정
	Grid->SetMaxRows(3);
	Grid->SetMaxCols(11);
	
	int i, j;
	for (i = 0; i < 11;i++)
	{
		GridMerge(IDC_CUSTOM_ID_BUFFER_INFO2, 1, i + 2, 1, 2);
		//m_p_grid.GridCellMerge(mp_gdIDBufferInfo, 1, i+2, 1, 2);
	}
	
	for(i=0; i<3; i++)
	{
		Grid->SetRowHeightInPixels(i+1, 22 );
		
		for(j=0; j<11; j++)
		{
			if(i == 0)
			{
				GridData(IDC_CUSTOM_ID_BUFFER_INFO2, 1, j+1, sColTitle[j]);
				//m_p_grid.GridCellText(mp_gdIDBufferInfo, 1, j+1, sColTitle[j]);
			}
			if(j == 0)
			{
				Grid->SetColWidthInPixels(j+1, 84);
				GridColor(IDC_CUSTOM_ID_BUFFER_INFO2, i + 1, j + 1, BLUE_D, WHITE_C);
			}
			else 
			{
				Grid->SetColWidthInPixels(j+1, 84);
				GridColor(IDC_CUSTOM_ID_BUFFER_INFO2, i + 1, j + 1, BLACK_L, YELLOW_L);
			}
			GridControl(IDC_CUSTOM_ID_BUFFER_INFO2, STATIC, i+1, j+1, 3);
			//m_p_grid.GridCellFont(mp_gdIDBufferInfo2, i+1, j+1, "Tahoma", 13);
			GridFont(IDC_CUSTOM_ID_BUFFER_INFO2, i+1, j+1, 13);
		}
		GridFont(IDC_CUSTOM_ID_BUFFER_INFO2, i+1, 1, 12);
		GridData(IDC_CUSTOM_ID_BUFFER_INFO2, i+1, 1, sRowTitle[i]);
		// 		m_p_grid.GridCellFont(mp_gdIDBufferInfo, i+1, 1, "Tahoma", 12);
		// 		m_p_grid.GridCellText(mp_gdIDBufferInfo, i+1, 1, sRowTitle[i]);
	}

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}
void CScreen_Main::OnMain_Display_BTM_BuffrInfo(int nzDisplayBuffer)
{
	TSpread *Grid;

	CString sUpdateData="", sTmp="";
	int i;
	int nDisplay_PKNo;

//	if(nzDisplayBuffer == 0) return;
	switch(nzDisplayBuffer)
	{
	case 0:
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD_BTM;
		nzDisplayBuffer = 4;
		break;
	case 1:
		//Setplate
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD_CLAMP;
		
		nzDisplayBuffer = 3;
		break;
	case 2:
		//Sealing
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD_LOAD;
		nzDisplayBuffer = 2;
		break;
	case 3:
		//out
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD_UNLOAD;
		nzDisplayBuffer = 1;
		break;
	case 4:
		
		nDisplay_PKNo = SHIFT_IDBUFF_INDEX_ULD_NEXT;
		nzDisplayBuffer = 0;
		//out
		break;
	}

	if( nzDisplayBuffer < 0 || nzDisplayBuffer > 4) return;

	////////////////////////////////////////////////////////////////////////// input id buffer 
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_ID_BUFFER_INFO2);
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%d", st_idbuff_info[nDisplay_PKNo].n_idbuff_num[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 2, sTmp);
	}

	////////////////////////////////////////////////////////////////////////// var 
	for (i = 0; i < 2; i++)
	{
		sTmp.Format("%s", st_idbuff_info[nDisplay_PKNo].c_var[i]);
		Grid->SetValue(2+ i + (nzDisplayBuffer * 2), 3, sTmp);
	}

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}
////

////2015,0109
void CScreen_Main::OnBtnNetwork() 
{
	// TODO: Add your control notification handler code here
	if(COMI.mn_run_status == dRUN) return;
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 8, 4);
}

void CScreen_Main::OnBtnConveyReset() 
{
	if(COMI.mn_run_status != dSTOP) return;
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to Conveyor seq reset run? ");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		if(	g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_OFF && 
			g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_OFF )
		{
			st_handler.mn_OutElv_work = CTL_NO;
		}
		g_handler.m_step_conv_smema = 0;
		g_Run_Unload_Conveyor.mn_elv_conv_step = 0;
		st_var.n_next_down = CTL_NO;
		
		g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
	}	
}

void CScreen_Main::OnBtnXgem() 
{
	if( COMI.mn_run_status != dSTOP ) return;
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_6, CLIENT_CONNECT, 5);// CLS_OTHERS
	::Sleep(500);

	CDialog_XgemInterface dlg;
	dlg.DoModal();
	
}

void CScreen_Main::OnBtnDensity1() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Density label#1 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
	st_work.n_mat_density[0] = 2000;
}

void CScreen_Main::OnBtnDensity2() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Density label#2 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_density[1] = 2000;
	
}

void CScreen_Main::OnBtnDensity3() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Density label#3 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_density[2] = 2000;
	
}

void CScreen_Main::OnBtnDensity4() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Density label#4 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_density[3] = 2000;
	
}

void CScreen_Main::OnBtnDensity5() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Density label#5 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_density[4] = 2000;
	
}

void CScreen_Main::OnBtnDensity6() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Density label#6 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_density[5] = 2000;
	
}

void CScreen_Main::OnBtnBcr11() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode label#1 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_bcr[0] = 3000;
	
}

void CScreen_Main::OnBtnBcr12() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode label#2 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_bcr[1] = 3000;
	
}

void CScreen_Main::OnBtnBcr21() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode label#3 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_bcr[2] = 3000;
	
}

void CScreen_Main::OnBtnBcr22() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode label#4 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_bcr[3] = 3000;	
}

void CScreen_Main::OnBtnRin11() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode ribbon#1 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_ribbon[0] = 5000;	
}

void CScreen_Main::OnBtnRin12() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode ribbon#2 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_ribbon[1] = 5000;		
}

void CScreen_Main::OnBtnRin21() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode ribbon#3 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_ribbon[2] = 5000;		
	
}

void CScreen_Main::OnBtnRin22() 
{
	if( st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "Barcode ribbon#4 is changed!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}	
	st_work.n_mat_ribbon[3] = 5000;		
	
}

void CScreen_Main::OnButtonModel() 
{
	CWnd* m_pWnd = CWnd::FindWindow(NULL,"RegModel");
	if(m_pWnd){
		m_pWnd->SetForegroundWindow(); 	
		m_pWnd->ShowWindow(SW_RESTORE);	
	}
	else{
		::ShellExecute(NULL, NULL, "C:\\AMT8590\\bin\\RegModel.exe", NULL, NULL, SW_SHOWNORMAL);
	}
	
}
