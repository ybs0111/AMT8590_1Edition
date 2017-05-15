// Screen_Initial.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Initial.h"

// ******************************************************************************
// 대화 상자 클래스 추가                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_Pass_Check.h"
// ******************************************************************************

#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"

#include "Srcbase\ALocalization.h"
#include "ComizoaPublic.h"
#include "COMI_Manager.h"

#include "SrcPart/APartHandler.h"

#include "Run_Unload_Works.h"
#include "Run_Barcode_Feeders.h"
#include "Run_Barcode_Works.h"
#include "Run_Btm_IDBuffer_Works.h"
#include "Run_Density_Feeders.h"
#include "Run_Density_Works.h"
#include "Run_IDBuffer_El.h"
#include "Run_Top_IDBuffer_Works.h"
#include "Run_Roller_Works.h"
#include "Run_Unload_Conveyor.h"
#include "Run_Top_Vision.h"

#include "ZebraPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial

IMPLEMENT_DYNCREATE(CScreen_Initial, CFormView)

CScreen_Initial::CScreen_Initial()
	: CFormView(CScreen_Initial::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Initial)
	//}}AFX_DATA_INIT
}

CScreen_Initial::~CScreen_Initial()
{
}

void CScreen_Initial::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Initial)
	DDX_Control(pDX, IDC_GIF_INITIALIZING, m_gif_initializing);
	DDX_Control(pDX, IDC_BTN_INIT_SKIP, m_btn_init_skip);
	DDX_Control(pDX, IDC_BTN_INIT_RETRY, m_btn_init_retry);
	DDX_Control(pDX, IDC_MSG_INITIAL, m_msg_initial);
	DDX_Control(pDX, IDC_RADIO_MOT_BD, m_radio_mot_bd);
	DDX_Control(pDX, IDC_RADIO_IO_BD, m_radio_io_bd);

	////2014.1018
	DDX_Control(pDX, IDC_RADIO_TOP_IDBUFFER, m_radio_top_idbuffer);
	DDX_Control(pDX, IDC_RADIO_TOP_IDBUFFER_1, m_radio_top_idbuffer_1);
	DDX_Control(pDX, IDC_RADIO_TOP_IDBUFFER_2, m_radio_top_idbuffer_2);
	
	DDX_Control(pDX, IDC_RADIO_BTM_IDBUFFER, m_radio_btm_idbuffer);
	DDX_Control(pDX, IDC_RADIO_BTM_IDBUFFER_1, m_radio_btm_idbuffer_1);
	DDX_Control(pDX, IDC_RADIO_BTM_IDBUFFER_2, m_radio_btm_idbuffer_2);
	
	DDX_Control(pDX, IDC_RADIO_IDBUFFER_EL, m_radio_idbuffer_el);
	DDX_Control(pDX, IDC_RADIO_TOP_VISION_X, m_radio_top_vision_x);
	
	DDX_Control(pDX, IDC_RADIO_ULD_CV, m_radio_uld_cv);
	DDX_Control(pDX, IDC_RADIO_ULD_CV_1, m_radio_uld_cv_1);
	DDX_Control(pDX, IDC_RADIO_ULD_CV_2, m_radio_uld_cv_2);
	DDX_Control(pDX, IDC_RADIO_ULD_CV_3, m_radio_uld_cv_3);
	DDX_Control(pDX, IDC_RADIO_ULD_CV_4, m_radio_uld_cv_4);
	DDX_Control(pDX, IDC_RADIO_ULD_CV_5, m_radio_uld_cv_5);
	
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER, m_radio_bcr_feeder);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_1, m_radio_bcr_feeder_1);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_2, m_radio_bcr_feeder_2);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_3, m_radio_bcr_feeder_3);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_4, m_radio_bcr_feeder_4);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_5, m_radio_bcr_vision_x);
	DDX_Control(pDX, IDC_RADIO_BCR_FEEDER_6, m_radio_bcr_reject_x);
	
	DDX_Control(pDX, IDC_RADIO_ATTACH, m_radio_bcr_attach);
	DDX_Control(pDX, IDC_RADIO_ATTACH_LEFT_1, m_radio_bcr_attach1_x);
	DDX_Control(pDX, IDC_RADIO_ATTACH_LEFT_2, m_radio_bcr_attach1_z1);
	DDX_Control(pDX, IDC_RADIO_ATTACH_LEFT_3, m_radio_bcr_attach1_r1);
	DDX_Control(pDX, IDC_RADIO_ATTACH_LEFT_4, m_radio_bcr_attach1_z2);
	DDX_Control(pDX, IDC_RADIO_ATTACH_LEFT_5, m_radio_bcr_attach1_r2);
	DDX_Control(pDX, IDC_RADIO_ATTACH_RIGHT_1, m_radio_bcr_attach2_x);
	DDX_Control(pDX, IDC_RADIO_ATTACH_RIGHT_2, m_radio_bcr_attach2_z1);
	DDX_Control(pDX, IDC_RADIO_ATTACH_RIGHT_3, m_radio_bcr_attach2_r1);
	DDX_Control(pDX, IDC_RADIO_ATTACH_RIGHT_4, m_radio_bcr_attach2_z2);
	DDX_Control(pDX, IDC_RADIO_ATTACH_RIGHT_5, m_radio_bcr_attach2_r2);
	
	DDX_Control(pDX, IDC_RADIO_DENSITY_ATTACH, m_radio_d_attach);
	DDX_Control(pDX, IDC_RADIO_DENSITY_ATTACH_1, m_radio_d_attach_x);
	DDX_Control(pDX, IDC_RADIO_DENSITY_ATTACH_2, m_radio_d_attach_y);
	DDX_Control(pDX, IDC_RADIO_DENSITY_ATTACH_3, m_radio_d_attach_z1);
	DDX_Control(pDX, IDC_RADIO_DENSITY_ATTACH_4, m_radio_d_attach_z2);
	
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER, m_radio_d_feeder);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_1, m_radio_d_feeder_x);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_2, m_radio_d_feeder_1);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_3, m_radio_d_feeder_2);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_4, m_radio_d_feeder_3);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_5, m_radio_d_feeder_4);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_6, m_radio_d_feeder_5);
	DDX_Control(pDX, IDC_RADIO_DENSITY_FEEDER_7, m_radio_d_feeder_6);
	
	DDX_Control(pDX , IDC_RADIO_INDEX, m_radio_index);
	DDX_Control(pDX , IDC_RADIO_INDEX_1, m_radio_index_roller1);
	DDX_Control(pDX , IDC_RADIO_INDEX_2, m_radio_index_roller2);
	DDX_Control(pDX , IDC_RADIO_INDEX_3, m_radio_index_roller_pitch);
	DDX_Control(pDX , IDC_RADIO_INDEX_4, m_radio_index_x);	////
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScreen_Initial, CFormView)
	//{{AFX_MSG_MAP(CScreen_Initial)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INIT_SKIP, OnBtnInitSkip)
	ON_BN_CLICKED(IDC_BTN_INIT_RETRY, OnBtnInitRetry)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_INIT_BTN, OnInitBtn )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial diagnostics

#ifdef _DEBUG
void CScreen_Initial::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Initial::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial message handlers

void CScreen_Initial::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	All_Stop = 0 ;  // 쓰레드 상태 플래그 설정: 쓰레드 동작 시작
	st_handler.mn_motor_init_check = CTL_READY;
	
	OnInitial_Progress_Set();
	OnInitial_Picture_Set();
	OnInitial_Label_Set();
	OnInitial_RadioButton_Set();
	OnInitial_Change_Status(RBTN_ALL_OFF);
	OnInitial_Step_Clear();
	OnInitial_Initial_Ready();

	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER1);
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER2);
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER3);
	clsZebra.OnPrintAnswerMode(2, 0, CLS_BCR_PRINTER4);

	st_handler.cwnd_init = this;
}

void CScreen_Initial::OnDestroy() 
{
	st_handler.cwnd_init = NULL;

	CFormView::OnDestroy();
}


void CScreen_Initial::OnTimer(UINT nIDEvent) 
{
	int mn_chk_run;			// 초기화 진행 함수 리턴 플래그 설정 변수
	int n_response;
	
	CDialog_Message  msg_dlg;

	if(nIDEvent == TMR_INIT)  
	{
		if (st_handler.mn_menu_lock != TRUE)  st_handler.mn_menu_lock = TRUE;			// 메뉴 사용 불가능 상태 플래그 설정 

		// **********************************************************************
		// 초기화 작업을 진행한다.                                               
		// **********************************************************************
		mn_chk_run = OnInitial_Init_Excution(); // 초기화 작업 진행 함수

		if (mn_chk_run == RET_GOOD)
		{
			KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 

			if( g_local.GetLocalType() == LOCAL_KOR )
			{
				sprintf(st_msg.c_normal_msg, "[초 기 화] 장비 초기화 완료");
			}
			else
			{
				sprintf(st_msg.c_normal_msg, "[Initialize] check end");
			}

			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); //james 2010.1004 8); 

			// ******************************************************************
			// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
			// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
			// ******************************************************************
			if (COMI.mn_run_status != dSTOP)
			{
				alarm.mn_reset_status = CTL_NO;
				
				Func.OnSet_IO_Port_Stop(); // 장비 상태 : 정지 상태인 경우 I/O 출력 내보내는 함수
				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, 2, dSTOP);
			}
			// ******************************************************************

			if (st_handler.mn_initial != TRUE)  st_handler.mn_initial = TRUE;			// 초기화 작업 완료 플래그 설정 
			if (st_handler.mn_menu_lock != FALSE)  st_handler.mn_menu_lock = FALSE;	// 메뉴 사용 가능 상태 플래그 설정
			

			g_ioMgr.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_alarm_clear_lamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_recycle_zone_door_switch_lamp, IO_OFF);//2014,1205

			
			st_msg.str_fallacy_msg = _T("장비 초기화 완료.");
			if( g_local.GetLocalType() == LOCAL_ENG ) st_msg.str_fallacy_msg = _T("Initial completed.");
			
			n_response = msg_dlg.DoModal();

			Func.Handler_Recovery_Data_Write();

			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면 전환 
		}
		else if (mn_chk_run == RET_ERROR)
		{
			KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제
// 			COMI.Set_MotStop(0, MOTOR_SORTER_X);
// 			COMI.Set_MotStop(0, MOTOR_PICKER_X);

			if ( g_local.GetLocalType() == LOCAL_KOR )	
			{
				sprintf(st_msg.c_abnormal_msg, "[초 기 화] 장비 초기화 실패");
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[initialize] handler initialize check error");
				CTL_Lib.Alarm_Error_Occurrence(1500, CTL_dWARNING, "900003");
			}

			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			// ******************************************************************
			// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
			// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
			// ******************************************************************
			if (COMI.mn_run_status != dSTOP)
			{
				Func.OnSet_IO_Port_Stop(); // 장비 상태 : 정지 상태인 경우 I/O 출력 내보내는 함수
				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, 2, dSTOP);
			}
			// ******************************************************************

			OnInitial_Controls_Enable(TRUE); // 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수

			if (st_handler.mn_initial != FALSE)		st_handler.mn_initial = FALSE;	// 초기화 작업 완료 여부 초기화 
			if (st_handler.mn_menu_lock != FALSE)	st_handler.mn_menu_lock = FALSE; // 메뉴 사용 가능 상태 플래그 설정 
		}
		// ******************************************************************************
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Initial::OnInitial_Label_Set()
{
	m_msg_initial.SetFont(Func.mp_init_font);
	m_msg_initial.SetWindowText(_T("initializing~!! Wait a moment~"));
	m_msg_initial.SetCenterText();
	m_msg_initial.SetColor(RGB(0,0,0));
	m_msg_initial.SetGradientColor(RGB(0,0,255));
	m_msg_initial.SetTextColor(RGB(255,255,255));
}

void CScreen_Initial::OnInitial_Picture_Set()
{
	if (m_gif_initializing.Load(MAKEINTRESOURCE(IDR_GIF_INITIALIZING),_T("GIF")))
		m_gif_initializing.Draw();
}

void CScreen_Initial::OnInitial_Progress_Set()
{
	CSuperProgressCtrl::RegisterClass();
	
	m_ctrlProgress.Create(this, 4, 250, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_1)), 0);
	m_ctrlProgress.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress.SetColours(RGB(255,0,0), RGB(0,0,0));
	m_ctrlProgress.SetRange(0, 50);
	m_ctrlProgress.SetStep(1);

	m_ctrlProgress1.Create(this, 4, 400, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_2)), 0);
	m_ctrlProgress1.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress1.SetColours(RGB(0,0,255), RGB(0,0,0));
	m_ctrlProgress1.SetRange(50, 100);
	m_ctrlProgress1.SetStep(1);
}

void CScreen_Initial::OnInitial_RadioButton_Set()
{
	m_radio_io_bd.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_mot_bd.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);

	////2014.1018
	m_radio_top_idbuffer.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_top_idbuffer_1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_top_idbuffer_2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	m_radio_btm_idbuffer.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_btm_idbuffer_1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_btm_idbuffer_2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	m_radio_idbuffer_el.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	
	m_radio_top_vision_x.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	
	m_radio_uld_cv.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_uld_cv_1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_uld_cv_2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_uld_cv_3.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_uld_cv_4.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_uld_cv_5.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	
	m_radio_bcr_feeder.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_bcr_feeder_1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_feeder_2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_feeder_3.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_feeder_4.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_vision_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_reject_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	m_radio_bcr_attach.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_bcr_attach1_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach2_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach1_z1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach1_r1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach1_z2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach1_r2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach2_z1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach2_r1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach2_z2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_bcr_attach2_r2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	m_radio_d_attach.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_d_attach_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_attach_y.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_attach_z1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_attach_z2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	m_radio_d_feeder.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_d_feeder_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_feeder_1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_feeder_2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_feeder_3.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_feeder_4.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_feeder_5.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_d_feeder_6.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	m_radio_index.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_index_roller1.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_index_roller2.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_index_roller_pitch.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	m_radio_index_x.SetIcons(IDI_CHECK_FALSE, IDI_CHECK_TRUE);
	
	////
}

void CScreen_Initial::OnInitial_Controls_Enable(int n_init)
{
	if (n_init == TRUE)  m_btn_init_retry.ShowWindow(SW_SHOW);  
	else                 m_btn_init_retry.ShowWindow(SW_HIDE);
}

void CScreen_Initial::OnInitial_Change_Status(int n_status)
{
	switch(n_status)
	{
	case RBTN_ALL_OFF:  // 이니셜 초기화인 경우 
		m_radio_io_bd.SetLedState(FALSE);
		m_radio_mot_bd.SetLedState(FALSE);

		m_radio_top_idbuffer.SetLedState(FALSE);		
		m_radio_top_idbuffer_1.SetLedState(FALSE);	
		m_radio_top_idbuffer_2.SetLedState(FALSE);	

		m_radio_btm_idbuffer.SetLedState(FALSE);		
		m_radio_btm_idbuffer_1.SetLedState(FALSE);	
		m_radio_btm_idbuffer_2.SetLedState(FALSE);	

		m_radio_idbuffer_el.SetLedState(FALSE);		
		m_radio_top_vision_x.SetLedState(FALSE);		

		m_radio_uld_cv.SetLedState(FALSE);			
		m_radio_uld_cv_1.SetLedState(FALSE);			
		m_radio_uld_cv_2.SetLedState(FALSE);			
		m_radio_uld_cv_3.SetLedState(FALSE);			
		m_radio_uld_cv_4.SetLedState(FALSE);			
		m_radio_uld_cv_5.SetLedState(FALSE);			

		m_radio_bcr_feeder.SetLedState(FALSE);		
		m_radio_bcr_feeder_1.SetLedState(FALSE);		
		m_radio_bcr_feeder_2.SetLedState(FALSE);		
		m_radio_bcr_feeder_3.SetLedState(FALSE);		
		m_radio_bcr_feeder_4.SetLedState(FALSE);		
		m_radio_bcr_vision_x.SetLedState(FALSE);		
		m_radio_bcr_reject_x.SetLedState(FALSE);		

		m_radio_bcr_attach.SetLedState(FALSE);		
		m_radio_bcr_attach1_x.SetLedState(FALSE);	
		m_radio_bcr_attach2_x.SetLedState(FALSE);	
		m_radio_bcr_attach1_z1.SetLedState(FALSE);	
		m_radio_bcr_attach1_r1.SetLedState(FALSE);	
		m_radio_bcr_attach1_z2.SetLedState(FALSE);	
		m_radio_bcr_attach1_r2.SetLedState(FALSE);	
		m_radio_bcr_attach2_z1.SetLedState(FALSE);	
		m_radio_bcr_attach2_r1.SetLedState(FALSE);	
		m_radio_bcr_attach2_z2.SetLedState(FALSE);	
		m_radio_bcr_attach2_r2.SetLedState(FALSE);	

		m_radio_d_attach.SetLedState(FALSE);			
		m_radio_d_attach_x.SetLedState(FALSE);		
		m_radio_d_attach_y.SetLedState(FALSE);		
		m_radio_d_attach_z1.SetLedState(FALSE);		
		m_radio_d_attach_z2.SetLedState(FALSE);		

		m_radio_d_feeder.SetLedState(FALSE);			
		m_radio_d_feeder_x.SetLedState(FALSE);		
		m_radio_d_feeder_1.SetLedState(FALSE);		
		m_radio_d_feeder_2.SetLedState(FALSE);		
		m_radio_d_feeder_3.SetLedState(FALSE);		
		m_radio_d_feeder_4.SetLedState(FALSE);		
		m_radio_d_feeder_5.SetLedState(FALSE);		
		m_radio_d_feeder_6.SetLedState(FALSE);		

		m_radio_index.SetLedState(FALSE);			
		m_radio_index_x.SetLedState(FALSE);			
		m_radio_index_roller1.SetLedState(FALSE);	
		m_radio_index_roller2.SetLedState(FALSE);	
		m_radio_index_roller_pitch.SetLedState(FALSE);
		break;

	case RBTN_IO:						m_radio_io_bd.SetLedState(TRUE);				break;
	case RBTN_MOTOR:					m_radio_mot_bd.SetLedState(TRUE);				break;
	
		////2014.1018
	case RBTN_TOPID_LD:					m_radio_top_idbuffer.SetLedState(TRUE);				break;
	case RBTN_TOPID_LD_1:				m_radio_top_idbuffer_1.SetLedState(TRUE);				break;
	case RBTN_TOPID_LD_2:				m_radio_top_idbuffer_2.SetLedState(TRUE);				break;
		
	case RBTN_BTMID_ULD:				m_radio_btm_idbuffer.SetLedState(TRUE);				break;
	case RBTN_BTMID_ULD_1:				m_radio_btm_idbuffer_1.SetLedState(TRUE);				break;
	case RBTN_BTMID_ULD_2:				m_radio_btm_idbuffer_2.SetLedState(TRUE);				break;
		
	case RBTN_IDBUFFER_EL:				m_radio_idbuffer_el.SetLedState(TRUE);				break;
	case RBTN_TOP_VISION_X:				m_radio_top_vision_x.SetLedState(TRUE);				break;
		
	case RBTN_ULD:						m_radio_uld_cv.SetLedState(TRUE);				break;
	case RBTN_ULD_STACKER_Z:			m_radio_uld_cv_1.SetLedState(TRUE);				break;
	case RBTN_ULD_CV_PITCH:				m_radio_uld_cv_2.SetLedState(TRUE);				break;
	case RBTN_ULD_Z:					m_radio_uld_cv_3.SetLedState(TRUE);				break;
	case RBTN_ULD_Y:					m_radio_uld_cv_4.SetLedState(TRUE);				break;
	case RBTN_BTM_VISION_X:				m_radio_uld_cv_5.SetLedState(TRUE);				break;
		
	case RBTN_BCR_FEEDER:				m_radio_bcr_feeder.SetLedState(TRUE);				break;
	case RBTN_BCR_FEEDER_1:				m_radio_bcr_feeder_1.SetLedState(TRUE);				break;
	case RBTN_BCR_FEEDER_2:				m_radio_bcr_feeder_2.SetLedState(TRUE);				break;
	case RBTN_BCR_FEEDER_3:				m_radio_bcr_feeder_3.SetLedState(TRUE);				break;
	case RBTN_BCR_FEEDER_4:				m_radio_bcr_feeder_4.SetLedState(TRUE);				break;
	case RBTN_BCR_VISION_X:				m_radio_bcr_vision_x.SetLedState(TRUE);				break;
	case RBTN_BCR_REJECT_X:				m_radio_bcr_reject_x.SetLedState(TRUE);				break;
		
		
	case RBTN_BCR_ATTACH:				m_radio_bcr_attach.SetLedState(TRUE);					break;
	case RBTN_BCR_ATTACH1_X:			m_radio_bcr_attach1_x.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH2_X:			m_radio_bcr_attach2_x.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH1_Z1:			m_radio_bcr_attach1_z1.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH1_R1:			m_radio_bcr_attach1_r1.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH1_Z2:			m_radio_bcr_attach1_z2.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH1_R2:			m_radio_bcr_attach1_r2.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH2_Z1:			m_radio_bcr_attach2_z1.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH2_R1:			m_radio_bcr_attach2_r1.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH2_Z2:			m_radio_bcr_attach2_z2.SetLedState(TRUE);				break;
	case RBTN_BCR_ATTACH2_R2:			m_radio_bcr_attach2_r2.SetLedState(TRUE);				break;
		
		
	case RBTN_D_ATTACH:					m_radio_d_attach.SetLedState(TRUE);					break;
	case RBTN_D_ATTACH_X:				m_radio_d_attach_x.SetLedState(TRUE);				break;
	case RBTN_D_ATTACH_Y:				m_radio_d_attach_y.SetLedState(TRUE);				break;
	case RBTN_D_ATTACH_Z1:				m_radio_d_attach_z1.SetLedState(TRUE);				break;
	case RBTN_D_ATTACH_Z2:				m_radio_d_attach_z2.SetLedState(TRUE);				break;
		
	case RBTN_D_FEEDER:					m_radio_d_feeder.SetLedState(TRUE);					break;
	case RBTN_D_VISION_X:				m_radio_d_feeder_x.SetLedState(TRUE);					break;
	case RBTN_D_FEEDER_1:				m_radio_d_feeder_1.SetLedState(TRUE);					break;
	case RBTN_D_FEEDER_2:				m_radio_d_feeder_2.SetLedState(TRUE);					break;
	case RBTN_D_FEEDER_3:				m_radio_d_feeder_3.SetLedState(TRUE);					break;
	case RBTN_D_FEEDER_4:				m_radio_d_feeder_4.SetLedState(TRUE);					break;
	case RBTN_D_FEEDER_5:				m_radio_d_feeder_5.SetLedState(TRUE);					break;
	case RBTN_D_FEEDER_6:				m_radio_d_feeder_6.SetLedState(TRUE);					break;
		
	case RBTN_INDEX:					m_radio_index.SetLedState(TRUE);					break;
	case RBTN_INDEX_X:					m_radio_index_x.SetLedState(TRUE);					break;
	case RBTN_INDEX_ROLLER1:			m_radio_index_roller1.SetLedState(TRUE);					break;
	case RBTN_INDEX_ROLLER2:			m_radio_index_roller2.SetLedState(TRUE);					break;
	case RBTN_ULD_ROLLER_PITCH:			m_radio_index_roller_pitch.SetLedState(TRUE);					break;
		
	////
	default :
		break;
	}
}

void CScreen_Initial::OnInitial_Step_Clear()
{
	int i = 0;
	
	ml_init_step = EIS_START;			// 초기화 진행 스텝 정보 저장 변수 초기화 
	
	for (i = 0; i < MAXSITE; i++)
	{
		st_handler.mn_init_state[i] = CTL_NO;
	}
}

void CScreen_Initial::OnInitial_Initial_Ready()
{
	int n_response;					// 대화 상자에 대한 리턴 값 저장 변수
	CDialog_Select select_dlg;
	char chr_data[25];	
	CString str_lock_check;
	int nRet=0;
	
	st_handler.mn_initial_error = CTL_READY;

	// Lock상태 였는지 확인을 한다.
	:: GetPrivateProfileString("Password", "SysLockStatus", "NO", chr_data, 20, st_path.mstr_basic);
	str_lock_check = chr_data;

	if (str_lock_check == "YES")			// Lock상태에서 강제 종료한것이라면...
	{
		// **************************************************************************
		// Lock 화면으로 전환하여 장비 사용을 막는다.
		// **************************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // 메인 프레임에 SYSTEM LOCK 화면 출력 요청
		// **************************************************************************
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "System Locking.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		return;
	}
	
	// **************************************************************************
	// 현재 장비 상태 (INIT) 타이틀 바에 출력한다.                               
	// **************************************************************************
	if (COMI.mn_run_status != dINIT)
	{
		Func.OnSet_IO_Port_Init(); // 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
		if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);
	}
	// **************************************************************************
	
	if (st_handler.mn_motor_init_check != CTL_READY)			// 모터 초기화를 할것인지 말것인지 결정한다. 2K4/11/16/ViboX	
	{
		SetTimer(TMR_INIT, 250, NULL);  // 초기화 작업 진행 타이머 생성
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// PC 가 재부팅 된 후 프로그램이 처음 실행된 경우 일때는 모션 컨트롤러의 
	// 엔코더 카운터 및 커맨드 카운터가 리셋 됨으로 복구동작을 하면 안된다.
	// 때문에 방지 조건을 추가 한다.
	// 1. 윈도우 시작 프로그램에 BootUpFlag.bat 파일을 두어 윈도우 재시작 시
	//    C:\ 에 BootUpFlag.txt 파일을 생성 한다.
	// 2. Dumping 기 프로그램 시작시 이 파일 있을 경우 윈도우가 재시작함을 인식.
	// 3. BootUpFlag.txt 파일이 있을 경우 삭제 한다.

	/***** BootUpFlag.bat *****************************************/
	//
	// 		@ECHO OFF
	// 		CLS
	// 			
	// 		SET FILENAME=C:\CellDumping\Setting\BootUpFlag.txt	
	// 			
	// 		ECHO BootUpFlag = 1 >> %FILENAME%
	// 			
	// 		ECHO. >> %FILENAME%
	//
	/**************************************************************/


 	st_handler.m_bRestore = false; //2013,1023
 	BOOL bRecovery = FALSE;
 	CFile file;
 	CString strPath = "C:\\AMT8590\\Setting\\BootUpFlag.txt";
 	if( !file.Open( strPath, CFile::modeRead) )
 	{
 // 		bRecovery = Func.Handler_Recovery_Data_Read();
 // 		if( bRecovery && OnLevelCheck() == -1)
 			bRecovery = FALSE;
 	}
 	else
 	{
 		// 복구동작 불가
 		file.Close();
 		file.Remove( strPath );
 		bRecovery = FALSE;
 		::WritePrivateProfileString("SaveMode", "RecoveryMode", "0", st_path.mstr_file_basic );
 	}
 
 	if( bRecovery )
 	{
 		n_response = IDOK;
 		bRecoveryInit = true;
 		st_handler.m_bRestore = true; //2013,1023
 	}
 	else
 	{
 		bRecoveryInit = false;
		st_msg.mstr_confirm_msg = _T("핸들러 모터를 초기화 하시겠습니까?");
		if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_confirm_msg = _T("Are you initialize Handler motor?");
		n_response = select_dlg.DoModal();
	}
	
	if (n_response == IDOK)
	{
		st_handler.mn_motor_init_check = CTL_YES;
	}
	else if (n_response == IDCANCEL)
	{
		st_handler.mn_motor_init_check = CTL_NO;
	}

	SetTimer(TMR_INIT, 250, NULL);  // 초기화 작업 진행 타이머 생성
}

int CScreen_Initial::OnInitial_Init_Excution()
{
	mn_init_flag = RET_PROCEED;

	if (COMI.mn_run_status != dINIT && bRecoveryInit != true )
	{
		if( COMI.mn_run_status == dWARNING )
		{
			mn_init_flag = RET_ERROR;
		}
		return mn_init_flag;
	}

	switch (ml_init_step)
	{
	case EIS_START:					EIS_Start();			break;
	case EIS_ERRMSG:				EIS_ErrMsg();			break;
	case EIS_IO:					EIS_InitIO();			break;
	case EIS_MOTOR:					EIS_Motor();			break;
	case EIS_CHK_STATE:				EIS_Chk_State();		break;
	case EIS_RECOVERY:				EIS_Recovery();			break;
	case EIS_CREATE_THREAD:			EIS_Create_Thread();	break;
	case EIS_SET_THREAD_STEP:		EIS_Set_Thread_Step();	break;
	case EIS_CHK_ALL_FINISH:		EIS_Chk_All_Finish();	break;
	case EIS_FINISH:				EIS_Finish();			break;
	}

	// Progress
	if (mn_pos_step < 51)
	{
		m_ctrlProgress.SetPos(mn_pos_step);
	}
	else if (mn_pos_step > 49)
	{
		if (mn_pos_step > 100)	mn_pos_step = 100;
		m_ctrlProgress.SetPos(50);
		m_ctrlProgress1.SetPos(mn_pos_step);
	}
	
	//초기화중 문제가 생겼다~
	if (st_handler.mn_initial_error == TRUE)
	{
		mn_pos_step = 0;
		m_ctrlProgress.SetPos(0);
		m_ctrlProgress1.SetPos(50);
		
		ml_init_step = EIS_START;			// 초기화 진행 스텝 정보 초기화 
		mn_init_flag = RET_ERROR;	// 초기화 작업 에러 플래그 설정 
	}

	return mn_init_flag;
}

void CScreen_Initial::OnBtnInitSkip() 
{
	KillTimer(TMR_INIT);  // 초기화 진행 타이머 삭제 
	
	sprintf(st_msg.c_normal_msg, "[initialization] SSD Auto Sorter initialization Skip.");
	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	
	// ******************************************************************
	// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
	// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
	// ******************************************************************
	if (COMI.mn_run_status != dSTOP)
	{
		alarm.mn_reset_status = CTL_NO;
		g_ioMgr.set_out_bit(st_io.o_alarm_clear_lamp, IO_OFF);	// Reset Switch에 Lamp를 Off 시킨다.
		
		Func.OnSet_IO_Port_Stop(); // 장비 상태: 정지 상태인 경우 I/O 출력 내보내는 함수
		if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, dSTOP);
	}
	// ******************************************************************
	
	if (st_handler.mn_initial != TRUE)  st_handler.mn_initial = TRUE;			// 초기화 작업 완료 플래그 설정 
	if (st_handler.mn_menu_lock != FALSE)  st_handler.mn_menu_lock = FALSE;	// 메뉴 사용 가능 상태 플래그 설정
	
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면 전환 

}

void CScreen_Initial::OnBtnInitRetry() 
{
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수
	
	CDialog_Select select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Initialization work progress");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		if (st_handler.mn_initial != FALSE)  st_handler.mn_initial = FALSE;  // 초기화 작업 완료 여부 초기화 
		
		st_handler.mn_manual = CTL_YES;
		mn_pos_step = 0;					// 프로그레서 위치 정보 초기화
		
		OnInitial_Controls_Enable(FALSE);	// 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수
		
		OnInitial_Change_Status(RBTN_ALL_OFF);			// 초기화 화면 이니셜 진행 버튼 상태 변경 함수
		
		OnInitial_Step_Clear();				// 초기화 작업: Step 데이터 저장 변수 초기화 함수
		
		OnInitial_Initial_Ready();			// 초기화 준비 작업 설정 함수
	}
	else if (n_response == IDCANCEL)
	{
		
	}	
}

int CScreen_Initial::OnLevelCheck()
{
	st_handler.mstr_pass_level = "LevelChange";

	CDialog_Pass_Check pass_dlg;
	if( pass_dlg.DoModal() == IDOK )
		return 1;

	return -1;
}

bool CScreen_Initial::ChkInitReady()
{
	if (g_ioMgr.Get_In_Bit(st_io.i_mc1_chk) == CTL_NO)
	{
		st_handler.mstrSelectMessage = "Check MC1.";
		st_handler.mnSelectMessage = 0;
		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
		
		return false;
	}

	int nRet, nRet2;
	
	nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
	
	if(nRet == CTL_OFF || nRet2 == CTL_OFF)
	{
		st_msg.mstr_event_msg[0] = "[AMT8590] Check Front, Rear Key is Auto or Manual please ";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
	}
	

	return true;
}

void CScreen_Initial::EIS_Start()
{
	if (st_handler.mn_motor_init_check == CTL_YES)
	{
		if (ChkInitReady() == false )
		{
			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
			
			mn_pos_step = 0;
			ml_init_step = EIS_ERRMSG;
			return;
		}
	}
	
	mcls_m_basic.OnBasic_Data_Load();

	
	mn_pos_step = 9;
	ml_init_step = EIS_IO;
}

void CScreen_Initial::EIS_ErrMsg()
{
	if (st_handler.mnSelectMessage == 1 )					// YES
	{
		g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
		ml_init_step = EIS_START;
		mn_pos_step = 0;
	}
	else if( st_handler.mnSelectMessage == 2 )
	{
		OnBtnInitSkip();
	}
}

void CScreen_Initial::EIS_InitIO()
{
	int nRet, nRet2;
	
	nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
	
	if(nRet == CTL_OFF || nRet2 == CTL_OFF)
	{
		return;
	}
	::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);


	bool Ret = g_ioMgr.GetIOBoardInit();		// I/O 보드 초기화 여부 검사한다
	
	if (Ret != TRUE)  
	{
		CTL_Lib.Alarm_Error_Occurrence(1500, CTL_dWARNING, "900003");
		
		ml_init_step = EIS_START;						// 초기화 진행 스텝 정보 초기화 
		mn_init_flag = RET_ERROR;				// 초기화 작업 에러 플래그 설정
	}
	else
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "I/O Board Initialized...");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		
		g_ioMgr.OnSet_IO_Out_Port_Clear();
		
		mn_pos_step += 8;
		
		OnInitial_Change_Status(RBTN_IO);				// I/O보드 초기화 완료.
		
		ml_init_step = EIS_MOTOR;
	}
}

void CScreen_Initial::EIS_Motor()
{
	int Ret = g_comiMgr.GetMotBoardInit();
	
	if (Ret != BD_GOOD )
	{
		CTL_Lib.Alarm_Error_Occurrence(1501, CTL_dWARNING, "900002");
		
		ml_init_step = EIS_START;						// 초기화 진행 스텝 정보 초기화 
		mn_init_flag = RET_ERROR;				// 초기화 작업 에러 플래그 설정 
	}
	else
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Motor Board Open Success!");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		
		Func.OnMot_Speed_Setting();					// MOTOR 보드가 정상적으로 OPEN이 된 후에 속도를 세팅한다. 2K5/03/24/ViboX
		
		mn_pos_step += 8;
		
		OnInitial_Change_Status(RBTN_MOTOR);					// MOTOR 보드 초기화 완료.
		
		ml_init_step = EIS_CHK_STATE;
	}
}

void CScreen_Initial::EIS_Chk_State()
{
	for (int i=0; i<MAXSITE; i++)
	{
		st_handler.mn_init_state[i] = CTL_NO;
	}
	
	if (st_handler.mn_motor_init_check == CTL_YES)	
	{
		if (st_handler.mn_manual == CTL_NO)
		{
			ml_init_step = EIS_RECOVERY;
		}
		else if (st_handler.mn_manual == CTL_YES)
		{	
			ml_init_step = EIS_CHK_ALL_FINISH;
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "Check status : EIS_CHK_ALL_FINISH");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
	}
	else
	{
		
		EIS_Create_Thread();
		ml_init_step = EIS_FINISH;
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Check status : EIS_FINISH");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
}

void CScreen_Initial::EIS_Recovery()
{
	if( bRecoveryInit )
	{
		COMI.mn_run_status = dSTOP;//2014.1017 dRESTORE -> dSTOP
		for( int i=0; i<MOTOR_COUNT; i++ )
		{
			COMI.mn_homechk_flag[i] = BD_YES;
			COMI.Set_MotPower( i, TRUE );
		}

//2013,1023
// 각 축에 대하여 초기화가 완료된 것으로 설정.
 			for (i = 0; i < MAXSITE; i++)
 			{
 				st_handler.mn_init_state[i] = CTL_YES;
			}
			////2014.1017
			st_handler.mn_init_state[INIT_TOP_IDBUFFER] = CTL_NO;
			st_handler.mn_init_state[INIT_BTM_IDBUFFER] = CTL_NO;
			st_handler.mn_init_state[INIT_IDBUFFER_EL] = CTL_NO;
			st_handler.mn_init_state[INIT_DENDITY_FEEDER] = CTL_NO;
			st_handler.mn_init_state[INIT_DENDITY_ROBOT] = CTL_NO;
			st_handler.mn_init_state[INIT_BARCODE_FEEDER] = CTL_NO;
			st_handler.mn_init_state[INIT_BARCODE_ROBOT] = CTL_NO;
			st_handler.mn_init_state[INIT_ROLLER_ROBOT] = CTL_NO;
			st_handler.mn_init_state[INIT_TOP_VISION] = CTL_NO;
			st_handler.mn_init_state[INIT_UNLOAD_ROBOT] = CTL_NO;
			st_handler.mn_init_state[INIT_UNLOAD_CONVEYOR] = CTL_NO;
			////
 			if(st_handler.cwnd_title != NULL)
 			{
 				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, 0);
 				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, 0);
 				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
 			}

		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Check status : RESTORE");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		
	}
	ml_init_step = EIS_CREATE_THREAD;
}

void CScreen_Initial::EIS_Create_Thread()
{
	m_thread[THREAD_TOP_IDBUFFER_WORKS] = AfxBeginThread( OnThread_Top_IDBuffer_Works, this );
	if( m_thread[THREAD_TOP_IDBUFFER_WORKS] != NULL )
		hThrHandle[THREAD_TOP_IDBUFFER_WORKS] = m_thread[THREAD_TOP_IDBUFFER_WORKS]->m_hThread;

	m_thread[THREAD_BTM_IDBUFFER_WORKS] = AfxBeginThread( OnThread_Btm_IDBuffer_Works, this );
	if( m_thread[THREAD_BTM_IDBUFFER_WORKS] != NULL )
		hThrHandle[THREAD_BTM_IDBUFFER_WORKS] = m_thread[THREAD_BTM_IDBUFFER_WORKS]->m_hThread;
	
	m_thread[THREAD_IDBUFFER_ELV] = AfxBeginThread( OnThread_IDBuffer_Elevator, this );
	if( m_thread[THREAD_IDBUFFER_ELV] != NULL )
		hThrHandle[THREAD_IDBUFFER_ELV] = m_thread[THREAD_IDBUFFER_ELV]->m_hThread;
	
	m_thread[THREAD_DENSITY_FEEDERS] = AfxBeginThread( OnThread_IDBuffer_Elevator, this );
	if( m_thread[THREAD_DENSITY_FEEDERS] != NULL )
		hThrHandle[THREAD_DENSITY_FEEDERS] = m_thread[THREAD_DENSITY_FEEDERS]->m_hThread;

	m_thread[THREAD_DENSITY_FEEDERS] = AfxBeginThread( OnThread_Density_Feeders, this );
	if( m_thread[THREAD_DENSITY_FEEDERS] != NULL )
		hThrHandle[THREAD_DENSITY_FEEDERS] = m_thread[THREAD_DENSITY_FEEDERS]->m_hThread;

	m_thread[THREAD_DENSITY_WORKS] = AfxBeginThread( OnThread_Density_Works, this );
	if( m_thread[THREAD_DENSITY_WORKS] != NULL )
		hThrHandle[THREAD_DENSITY_WORKS] = m_thread[THREAD_DENSITY_WORKS]->m_hThread;

	m_thread[THREAD_BARCODE_FEEDERS] = AfxBeginThread( OnThread_Barcode_Feeders, this );
	if( m_thread[THREAD_BARCODE_FEEDERS] != NULL )
		hThrHandle[THREAD_BARCODE_FEEDERS] = m_thread[THREAD_BARCODE_FEEDERS]->m_hThread;

	m_thread[THREAD_BARCODE_WORKS] = AfxBeginThread( OnThread_Barcode_Works, this );
	if( m_thread[THREAD_BARCODE_WORKS] != NULL )
		hThrHandle[THREAD_BARCODE_WORKS] = m_thread[THREAD_BARCODE_WORKS]->m_hThread;

	m_thread[THREAD_ROLLER_WORKS] = AfxBeginThread( OnThread_Roller_Works, this );
	if( m_thread[THREAD_ROLLER_WORKS] != NULL )
		hThrHandle[THREAD_ROLLER_WORKS] = m_thread[THREAD_ROLLER_WORKS]->m_hThread;
	
	m_thread[THREAD_TOP_VISION] = AfxBeginThread( OnThread_Top_Vision, this );
	if( m_thread[THREAD_TOP_VISION] != NULL )
		hThrHandle[THREAD_TOP_VISION] = m_thread[THREAD_TOP_VISION]->m_hThread;

	m_thread[THREAD_UNLOAD_WORKS] = AfxBeginThread( OnThread_Unload_Works, this );
	if( m_thread[THREAD_UNLOAD_WORKS] != NULL )
		hThrHandle[THREAD_UNLOAD_WORKS] = m_thread[THREAD_UNLOAD_WORKS]->m_hThread;

	m_thread[THREAD_UNLOAD_CONVEYOR] = AfxBeginThread( OnThread_Unload_Conveyor, this );
	if( m_thread[THREAD_UNLOAD_CONVEYOR] != NULL )
		hThrHandle[THREAD_UNLOAD_CONVEYOR] = m_thread[THREAD_UNLOAD_CONVEYOR]->m_hThread;


	ml_init_step = EIS_SET_THREAD_STEP;

}

void CScreen_Initial::EIS_Set_Thread_Step()
{
	g_Run_Top_IDBuffers.mn_InitStep = ETRI_START;
	g_Run_Btm_IDBuffers.mn_InitStep = EBRI_START;
	g_Run_IDBuffers_El.mn_InitStep = EIEI_START;
	g_Run_Density_Feeders.mn_InitStep = EDEI_START;
	g_Run_Density_Works.mn_InitStep = EDRI_START;
	g_Run_Barcode_Feeders.mn_InitStep = EBFI_START;
	g_Run_Barcode_Works.mn_InitStep = EBRI_START;
	g_Run_Roller_Works.mn_InitStep = ERRI_START;
	g_Run_Top_Vision.mn_InitStep = ETVI_START;

	g_Run_Unload_Works.mn_InitStep = EURI_START;
	g_Run_Unload_Conveyor.mn_InitStep = EUCI_START;

	ml_init_step = EIS_CHK_ALL_FINISH;
}

void CScreen_Initial::EIS_Chk_All_Finish()
{
	int j = 0;
	for (int i = 0; i < MAXSITE; i++)
	{
// 		if(i == 7)//2014.1018 test 
// 		{
// 			st_handler.mn_init_state[i] = CTL_READY;
// 		}


		if (st_handler.mn_init_state[i] == CTL_YES)
		{
			mn_pos_step += 10;
		
			st_handler.mn_init_state[i] = CTL_READY;
		}
	}
	
	for (i = 0; i < MAXSITE; i++)
	{
		if (st_handler.mn_init_state[i] == CTL_READY)
		{
			j++;
		}
	}
// 	
// 	// 전부 홈체크가 끝났으면 초기화 끝~~~~ ^_^
	if (j == MAXSITE)
	{
		for (i = 0; i < MAXSITE; i++)
		{
 			st_handler.mn_init_state[i] = CTL_YES;
		}
		
 		ml_init_step = EIS_FINISH;
	}
}

void CScreen_Initial::EIS_Finish()
{
	mn_pos_step = 100;
	
	ml_init_step = EIS_START; 
	mn_init_flag = RET_GOOD;
}

LRESULT CScreen_Initial::OnInitBtn(WPARAM wParam,LPARAM lParam)
{
	OnInitial_Change_Status( (int)wParam );

	return 0;
}
