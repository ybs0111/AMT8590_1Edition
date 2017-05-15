// Screen_Set_Maintenance.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Set_Maintenance.h"

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

#include "Dialog_Pass_Change.h"
// ******************************************************************************

#include "AMTLotManager.h"
#include "AVersion.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartRobot.h"

#include "InterfaceBarcode.h"
#include "Public_Function.h"
#include "VisionClient.h"
#include "VisionServer.h"

#include "Run_Density_Feeders.h"
#include "Run_Smema_Ctl.h"
#include "Run_IDBuff_Input_Index.h"
#include "Run_Density_Feeders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance

IMPLEMENT_DYNCREATE(CScreen_Set_Maintenance, CFormView)

CScreen_Set_Maintenance::CScreen_Set_Maintenance()
	: CFormView(CScreen_Set_Maintenance::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Set_Maintenance)
	//}}AFX_DATA_INIT
}

CScreen_Set_Maintenance::~CScreen_Set_Maintenance()
{
}

void CScreen_Set_Maintenance::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Set_Maintenance)
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_edit_send_data);
	DDX_Control(pDX, IDC_BTN_ID_SEND, m_btn_id_send);
	DDX_Control(pDX, IDC_GROUP_DATA_SEND_8590, m_group_sendto_8580);
	DDX_Control(pDX, IDC_BTN_ID_SAVE, m_btn_idsave);
	DDX_Control(pDX, IDC_MSG_TBOX_CODE, m_msg_tbox_code);
	DDX_Control(pDX, IDC_MSG_ACCE_CODE, m_msg_acce_code);
	DDX_Control(pDX, IDC_GROUP_DATA_CHANGE_8580S, m_group_data_change_8580s);
	DDX_Control(pDX, IDC_EDIT_TBOX_CODE, m_edit_tbox_code);
	DDX_Control(pDX, IDC_EDIT_ACCE_CODE, m_edit_acce_code);
	DDX_Control(pDX, IDC_BTN_DENSITY_RESET, m_btn_density_reset);
	DDX_Control(pDX, IDC_BTN_DENSITY_RUN, m_btn_density_run);
	DDX_Control(pDX, IDC_BTN_DENSITY_MANUAL, m_btn_density_manual);
	DDX_Control(pDX, IDC_GROUP_DENSITY_OUT, m_group_manual_out);
	DDX_Control(pDX, IDC_BTN_FRONT_BTM_RESET, m_btn_front_btm_reset);
	DDX_Control(pDX, IDC_BTN_FRONT_TOP_RESET, m_btn_front_top_reset);
	DDX_Control(pDX, IDC_GROUP_SMEMA_RESET, m_group_smema_result);
	DDX_Control(pDX, IDC_BTN_BARCODE_POS_DATA, m_btn_barcodepos_result);
	DDX_Control(pDX, IDC_BTN_BARCODEPOS_CHANGE, m_btn_barcodepos_change);
	DDX_Control(pDX, IDC_STATIC_BARCODE_POS, m_group_barcode_pos);
	DDX_Control(pDX, IDC_BTN_DENSITY_POS_DATA, m_btn_densitypos_result);
	DDX_Control(pDX, IDC_BTN_DENSITYPOS_MODEL, m_btn_densitypos_model);
	DDX_Control(pDX, IDC_STATIC_DENSITY_POS, m_group_density_pos);
	DDX_Control(pDX, IDC_STATIC_MODEL_NAME, m_label_model_name);
	DDX_Control(pDX, IDC_BTN_BARCODE_DATA, m_btn_bcr_result);
	DDX_Control(pDX, IDC_BTN_BARCODE_CHANGE, m_btn_bcr_change);
	DDX_Control(pDX, IDC_STATIC_BARCODE, m_group_barcode);
	DDX_Control(pDX, IDC_BTN_DENSITY_DATA, m_btn_density_result);
	DDX_Control(pDX, IDC_BTN_DENSITY_MODEL, m_btn_denity_change);
	DDX_Control(pDX, IDC_STATIC_DENSITY, m_group_density_label);
	DDX_Control(pDX, IDC_BTN_BCR6, m_btn_openbcr2);
	DDX_Control(pDX, IDC_BTN_BCR5, m_btn_openbcr1);
	DDX_Control(pDX, IDC_BTN_BCR4, m_btn_closebcr2);
	DDX_Control(pDX, IDC_BTN_BCR3, m_btn_closebcr1);
	DDX_Control(pDX, IDC_BTN_BCR2, m_btn_readbcr2);
	DDX_Control(pDX, IDC_BTN_BCR1, m_btn_readbcr1);
	DDX_Control(pDX, IDC_BTN_CLOSE_GMS, m_btn_closegms);
	DDX_Control(pDX, IDC_BTN_GMS, m_btn_opengms);
	DDX_Control(pDX, IDC_STATIC_COMM, m_group_comm);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_BTN_PASSWORD_CHANGE_LEVLE2, m_btn_password_change_level2);
	DDX_Control(pDX, IDC_BTN_PASSWORD_CHANGE_LEVLE1, m_btn_password_change_level1);
	DDX_Control(pDX, IDC_BTN_DATA_BACKUP, m_btn_data_backup);
	DDX_Control(pDX, IDC_BTN_STEP_SAVE, m_btn_step_save);
	DDX_Control(pDX, IDC_GROUP_LAMP_CHANGE_TIME, m_group_lamp_change_time);
	DDX_Control(pDX, IDC_LAMP_STOP, m_lamp_stop);
	DDX_Control(pDX, IDC_LAMP_SELFCHECK, m_lamp_selfcheck);
	DDX_Control(pDX, IDC_LAMP_WARNING, m_lamp_warning);
	DDX_Control(pDX, IDC_LAMP_RUN, m_lamp_run);
	DDX_Control(pDX, IDC_LAMP_LOTEND, m_lamp_lotend);
	DDX_Control(pDX, IDC_LAMP_LOCK, m_lamp_lock);
	DDX_Control(pDX, IDC_LAMP_INITIAL, m_lamp_initial);
	DDX_Control(pDX, IDC_LAMP_ALARM, m_lamp_alarm);
	DDX_Control(pDX, IDC_GROUP_TOWERLAMP, m_group_towerlamp);
	DDX_Control(pDX, IDC_GROUP_PASSWORD_CHANGE, m_group_password_change);
	DDX_Control(pDX, IDC_GROUP_LAMP_WARNING, m_group_lamp_warning);
	DDX_Control(pDX, IDC_GROUP_LAMP_STOP, m_group_lamp_stop);
	DDX_Control(pDX, IDC_GROUP_LAMP_SELFCHECK, m_group_lamp_selfcheck);
	DDX_Control(pDX, IDC_GROUP_LAMP_RUN, m_group_lamp_run);
	DDX_Control(pDX, IDC_GROUP_LAMP_LOTEND, m_group_lamp_lotend);
	DDX_Control(pDX, IDC_GROUP_LAMP_LOCK, m_group_lamp_lock);
	DDX_Control(pDX, IDC_GROUP_LAMP_INITIAL, m_group_lamp_initial);
	DDX_Control(pDX, IDC_GROUP_LAMP_FLICKER_TIME, m_group_lamp_flicker_time);
	DDX_Control(pDX, IDC_GROUP_LAMP_ALARM, m_group_lamp_alarm);
	DDX_Control(pDX, IDC_GROUP_DATA_BACKUP, m_group_data_backup);
	DDX_Control(pDX, IDC_GROUP_BUZZER, m_group_buzzer);
	DDX_Control(pDX, IDC_CHK_BUZZER, m_chk_buzzer);
	DDX_Control(pDX, IDC_BTN_MAINTENANCE_APPLY, m_btn_maintenance_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Set_Maintenance, CFormView)
	//{{AFX_MSG_MAP(CScreen_Set_Maintenance)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DGT_FLICKER_TIME, OnDgtFlickerTime)
	ON_BN_CLICKED(IDC_CHK_BUZZER, OnChkBuzzer)
	ON_BN_CLICKED(IDC_BTN_DATA_BACKUP, OnBtnDataBackup)
	ON_BN_CLICKED(IDC_BTN_PASSWORD_CHANGE_LEVLE1, OnBtnPasswordChangeLevle1)
	ON_BN_CLICKED(IDC_BTN_PASSWORD_CHANGE_LEVLE2, OnBtnPasswordChangeLevle2)
	ON_BN_CLICKED(IDC_BTN_MAINTENANCE_APPLY, OnBtnMaintenanceApply)
	ON_BN_CLICKED(IDC_DGT_CHANGE_TIME, OnDgtChangeTime)
	ON_BN_CLICKED(IDC_BTN_STEP_SAVE, OnBtnStepSave)
	ON_BN_CLICKED(IDC_BTN_BCR1, OnBtnBcr1)
	ON_BN_CLICKED(IDC_BTN_BCR2, OnBtnBcr2)
	ON_BN_CLICKED(IDC_BTN_BCR4, OnBtnBcr4)
	ON_BN_CLICKED(IDC_BTN_BCR5, OnBtnBcr5)
	ON_BN_CLICKED(IDC_BTN_BCR6, OnBtnBcr6)
	ON_BN_CLICKED(IDC_BTN_BCR3, OnBtnBcr3)
	ON_BN_CLICKED(IDC_BTN_GMS, OnBtnGms)
	ON_BN_CLICKED(IDC_BTN_CLOSE_GMS, OnBtnCloseGms)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BTN_DENSITY_MODEL, OnBtnDensityModel)
	ON_BN_CLICKED(IDC_BTN_DENSITY_DATA, OnBtnDensityData)
	ON_BN_CLICKED(IDC_BTN_BARCODE_CHANGE, OnBtnBarcodeChange)
	ON_BN_CLICKED(IDC_BTN_BARCODE_DATA, OnBtnBarcodeData)
	ON_BN_CLICKED(IDC_BTN_DENSITYPOS_MODEL, OnBtnDensityposModel)
	ON_BN_CLICKED(IDC_BTN_DENSITY_POS_DATA, OnBtnDensityPosData)
	ON_BN_CLICKED(IDC_BTN_BARCODEPOS_CHANGE, OnBtnBarcodeposChange)
	ON_BN_CLICKED(IDC_BTN_BARCODE_POS_DATA, OnBtnBarcodePosData)
	ON_BN_CLICKED(IDC_BTN_DENSITY_1, OnBtnDensity1)
	ON_BN_CLICKED(IDC_BTN_DENSITY_2, OnBtnDensity2)
	ON_BN_CLICKED(IDC_BTN_DENSITY_3, OnBtnDensity3)
	ON_BN_CLICKED(IDC_BTN_DENSITY_4, OnBtnDensity4)
	ON_BN_CLICKED(IDC_BTN_DENSITY_5, OnBtnDensity5)
	ON_BN_CLICKED(IDC_BTN_DENSITY_6, OnBtnDensity6)
	ON_BN_CLICKED(IDC_BTN_FRONT_TOP_RESET, OnBtnFrontTopReset)
	ON_BN_CLICKED(IDC_BTN_FRONT_BTM_RESET, OnBtnFrontBtmReset)
	ON_BN_CLICKED(IDC_BTN_DENSITY_MANUAL, OnBtnDensityManual)
	ON_BN_CLICKED(IDC_BTN_DENSITY_RUN, OnBtnDensityRun)
	ON_BN_CLICKED(IDC_BTN_DENSITY_RESET, OnBtnDensityReset)
	ON_BN_CLICKED(IDC_BTN_ID_SAVE, OnBtnIdSave)
	ON_BN_CLICKED(IDC_BTN_ID_SEND, OnBtnIdSend)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_LAMP_RLAMP, ID_LAMP_RLAMP+SELFCHECK, OnRLampClick)
	ON_COMMAND_RANGE(ID_LAMP_YLAMP, ID_LAMP_YLAMP+SELFCHECK, OnYLampClick)
	ON_COMMAND_RANGE(ID_LAMP_GLAMP, ID_LAMP_GLAMP+SELFCHECK, OnGLampClick)
	ON_MESSAGE(WM_MAINTENANCE_APPLY, OnMaintenance_Info_Apply)				// Tower Lamp DATA를 APPLY 시키기 위한 사용자 정의 메시지 선언
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance diagnostics

#ifdef _DEBUG
void CScreen_Set_Maintenance::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Set_Maintenance::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance message ICTs

void CScreen_Set_Maintenance::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	OnLeveldisplay();
	OnMaintenance_GroupBox_Set();
	OnMaintenance_Button_Set();
	OnMaintenance_EditBox_Set();
	OnMaintenance_Label_Set();
	OnMaintenance_Digital_Count_Set();

	OnMaintenance_Data_Set();
	mb_first = TRUE;
	OnMaintenance_Data_BackUp();

	OnMaintenance_Data_Display();

	OnMaintenance_Buzzer_Change();


	st_handler.cwnd_maintenance = this;  // Tower Lamp 화면에 대한 핸들 정보 설정 
}

void CScreen_Set_Maintenance::OnDestroy() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret;

	CDialog_Select select_dlg;

	//프로그램을 빠져나가면서 Save 하지 않고, Form이 Destroy되는것이라면 Setting을 포기한다는 말이다.
	//비교도 하지 말자.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnMaintenance_Data_Comparison();

		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			mn_response = select_dlg.DoModal();
			
			if (mn_response == IDOK)
			{
				OnMaintenance_Data_Apply();

				OnMaintenance_LogFile_Create();

				OnMaintenance_ListBar_Info_Send();
			}
			else if (mn_response == IDCANCEL)
			{
				OnMaintenance_Data_Recovery();

				OnMaintenance_Data_Display();

				OnMaintenance_Data_Apply();

				OnMaintenance_ListBar_Info_Send();
			}	
		}
	}

	KillTimer(TMR_BCR);
	KillTimer(TMR_BTM);
	KillTimer(TMR_LABEL);
	KillTimer(TMR_TOP);
	KillTimer(TM_MODEL_CHANGE);

	if( st_handler.mn_manual_lable == CTL_YES )
	{
		st_handler.mn_manual_lable = CTL_NO; 
		for ( int i = 0; i < MAX_EDP; i++ )
		{
			g_Run_Density_Feeders.m_bPrint_Pickup[i] = false;
			g_Run_Density_Feeders.m_nRun_Print[i] = 0;
		}
		
		for ( int j = 0; j < 6; j++)
		{
			//g_Run_Density_Feeders.mn_RunStep[j] = 0;
			g_Run_Density_Feeders.m_nLabelResult[j] = CTL_FREE;//초기 비젼 세팅
		}
		g_Run_Density_Feeders.mn_RunStep = 0;//2015.0105
	}

	st_handler.cwnd_maintenance = NULL;	// Maintenance 화면에 대한 핸들 정보 저장 변수 초기화 

	CFormView::OnDestroy();	
}

void CScreen_Set_Maintenance::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (mb_first)          // 타워 램프 처음 생성 유무 검사
	{
		mb_first = FALSE;  // 타워 램프 처음 생성 유무 플래그 설정

		// **********************************************************************
		// 타워 램프 출력할 컨트롤 영역 크기 얻는다                              
		// -> 타워 램프 출력 영역 구한 후에는 해당 컨트롤 보이지 않게 만든다     
		// -> 타워 램프 실질적으로 출력하는 것은 뷰어이기 때문이다               
		// -> GetWindowRect() : 윈도우를 둘러싸는 사각형의 크기를 구해           
		//    lprc가 가리키는 RECT 구조체에 그 값을 저장한다                     
		//  : 얻어진 좌표는 픽셀 단위이며 화면의 좌측 상단을 기준으로 계산한다   
		//  : 현재 윈도우의 크기와 위치를 화면 상대 좌표로 알고 싶을 때 사용한다 
		// **********************************************************************
		CRect r;
		GetWindowRect(&r);

		m_lamp_run.GetWindowRect(&mr_area[RUN]);
		m_lamp_stop.GetWindowRect(&mr_area[STOP]);
		m_lamp_alarm.GetWindowRect(&mr_area[ALARM]);
		m_lamp_lock.GetWindowRect(&mr_area[LOCK]);
		m_lamp_lotend.GetWindowRect(&mr_area[LOTEND]);
		m_lamp_initial.GetWindowRect(&mr_area[INIT]);
		m_lamp_warning.GetWindowRect(&mr_area[WARNING]);
		m_lamp_selfcheck.GetWindowRect(&mr_area[SELFCHECK]);

		for(int i=0; i<=SELFCHECK; i++)
		{
			mr_area[i].left -= r.left;
			mr_area[i].right -= r.left;
			mr_area[i].top -= r.top;
			mr_area[i].bottom -= r.top;
		}

		m_lamp_run.ShowWindow(SW_HIDE);
		m_lamp_stop.ShowWindow(SW_HIDE);
		m_lamp_alarm.ShowWindow(SW_HIDE);
		m_lamp_lock.ShowWindow(SW_HIDE);
		m_lamp_lotend.ShowWindow(SW_HIDE);
		m_lamp_initial.ShowWindow(SW_HIDE);
		m_lamp_warning.ShowWindow(SW_HIDE);
		m_lamp_selfcheck.ShowWindow(SW_HIDE);
		// **********************************************************************

		for(int j=0; j<=SELFCHECK; j++)
			DrawButton(j);							// 타워 램프 버튼 생성 함수

		for(int k=0; k<=SELFCHECK; k++)
		{
			OnMaintenance_Red_State_Change(k);		// RED 타워 램프 버튼 상태 변환 함수
			OnMaintenance_Yellow_State_Change(k);	// YELLOW 타워 램프 버튼 상태 변환 함수
			OnMaintenance_Green_State_Change(k);	// GREEN 타워 램프 버튼 상태 변환 함수
		}
	}
}

void CScreen_Set_Maintenance::OnMaintenance_GroupBox_Set()
{
	CSxLogFont maintenance_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

	m_group_towerlamp.SetFont(maintenance_font);
	m_group_towerlamp.SetCatptionTextColor(RGB(145,25,0));
	m_group_towerlamp.SetFontBold(TRUE);

	m_group_lamp_run.SetFont(maintenance_font);
	m_group_lamp_run.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_run.SetFontBold(TRUE);

	m_group_lamp_stop.SetFont(maintenance_font);
	m_group_lamp_stop.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_stop.SetFontBold(TRUE);

	m_group_lamp_alarm.SetFont(maintenance_font);
	m_group_lamp_alarm.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_alarm.SetFontBold(TRUE);

	m_group_lamp_lock.SetFont(maintenance_font);
	m_group_lamp_lock.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_lock.SetFontBold(TRUE);

	m_group_lamp_lotend.SetFont(maintenance_font);
	m_group_lamp_lotend.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_lotend.SetFontBold(TRUE);

	m_group_lamp_initial.SetFont(maintenance_font);
	m_group_lamp_initial.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_initial.SetFontBold(TRUE);
	m_group_lamp_initial.ShowWindow(FALSE);

	m_group_lamp_warning.SetFont(maintenance_font);
	m_group_lamp_warning.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_warning.SetFontBold(TRUE);

	m_group_lamp_selfcheck.SetFont(maintenance_font);
	m_group_lamp_selfcheck.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_selfcheck.SetFontBold(TRUE);

	m_group_lamp_flicker_time.SetFont(maintenance_font);
	m_group_lamp_flicker_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_flicker_time.SetFontBold(TRUE);

	m_group_buzzer.SetFont(maintenance_font);
	m_group_buzzer.SetCatptionTextColor(RGB(145,25,0));
	m_group_buzzer.SetFontBold(TRUE);

	m_group_password_change.SetFont(maintenance_font);
	m_group_password_change.SetCatptionTextColor(RGB(145,25,0));
	m_group_password_change.SetFontBold(TRUE);

	m_group_data_backup.SetFont(maintenance_font);
	m_group_data_backup.SetCatptionTextColor(RGB(145,25,0));
	m_group_data_backup.SetFontBold(TRUE);

	m_group_lamp_change_time.SetFont(maintenance_font);
	m_group_lamp_change_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_change_time.SetFontBold(TRUE);

	m_group_data_change_8580s.SetFont(maintenance_font);
	m_group_data_change_8580s.SetCatptionTextColor(RGB(145,25,0));
	m_group_data_change_8580s.SetFontBold(TRUE);

	m_group_sendto_8580.SetFont(maintenance_font);
	m_group_sendto_8580.SetCatptionTextColor(RGB(145,25,0));
	m_group_sendto_8580.SetFontBold(TRUE);
}

void CScreen_Set_Maintenance::DrawButton(int i)
{
	// **************************************************************************
    // 타워 램프 출력할 컨트롤에 대한 정보 얻는다                                
    // **************************************************************************
	CRect r;
	r = mr_area[i];  // 컨트롤 영역 크기 설정

	if (i == dINIT)	return;

	mn_pos_x[i][0] = r.left;
	mn_pos_x[i][1] = r.left + ((int)r.Width()-4);
	mn_pos_y[i][0] = r.top;
	mn_pos_y[i][1] = r.bottom-4;
	mn_y_diff[i] = (int) (mn_pos_y[i][1] - mn_pos_y[i][0])/7;
	mn_y_space[i] = (int) mn_y_diff[i]/6;
	// **************************************************************************

	// **************************************************************************
    // 타워 램프 출력 이미지 관련 데이터 저장 변수 선언한다                      
    // **************************************************************************
	CRgn b1, b2, e1, e2;
	COLORREF cr;
	int x1=0, x2, y1=0, y2, w=mn_pos_x[i][1] - mn_pos_x[i][0];
	CPoint p;
	// **************************************************************************

	// **************************************************************************
    // 타워 램프 막대 끝의 둥근 고정 브라켓 영역 계산하여 컨트롤에 출력한다      
	// -> CreateEllipticRgn() : 주어진 사각형 영역으로 타원 영역을 생성한다      
	//  : 성공적으로 영역이 생성되었으면 영역의 핸들 값이 리턴                   
	//  : 영역이 생성되지 않았으면 NULL이 리턴                                   
    // **************************************************************************
	p.x = mn_pos_x[i][0];
	p.y = mn_pos_y[i][1] - mn_y_space[i] * 4;
	x2 = w;	
	y2 = mn_y_space[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	mp_btn_base[i].Create("", WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_BASE+i, 1, RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183));
	mp_btn_base[i].EnableWindow(FALSE);

	e1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // 타워 램프의 하단 막대 이미지 출력 위치 계산하여 컨트롤에 출력한다         
	// -> CreateRectRgn() : 주어진 좌표로 사각 영역을 그린다                     
	// -> OffsetRgn() : 지정된 사각형의 좌표를 이동한다                          
	// -> CombineRgn() : 존재하는 두 개의 여역들을 합쳐서 세 번째 영역을 만든다  
	//  : 세 번째 영역의 핸들은 이 함수가 쓰이기 전에 반드시 만들어져야만        
	//    새로운 영역으로 대체된다                                               
	// -> GetRgnBox() : 주어진 영역을 완전히 둘러싸는 가장 작은 직사각형 그린다  
    // **************************************************************************
	x2 = (int) w / 3;
	y2 = (int) mn_y_space[i] * 4 / 3;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = mn_y_diff[i] * 2 - mn_y_space[i] * 2;
	b1.CreateRectRgn(x1, y1, x2, y2);

	x2 = w;
	y2 = mn_y_space[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, mn_y_diff[i]*2 - mn_y_space[i]*2 - (int)mn_y_space[i]*2/3);
	e2.OffsetRgn(-(int)w/3, -mn_y_space[i]);
	b1.CombineRgn(&e1, &b1, RGN_OR);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	CRect rr;
	b1.GetRgnBox(&rr);

	p.y = rr.top;
	p.x = mn_pos_x[i][0] + (int) w/3;
	p.y += (mn_pos_y[i][1] - mn_y_diff[i] * 2);
	mp_btn_pole[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_POLE+i, 1, RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156));
	mp_btn_pole[i].EnableWindow(FALSE);

	e1.DeleteObject();
	e2.DeleteObject();
	b1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // GREEN 램프 밑에 존재하는 플라스틱 영역 계산하여 컨트롤에 출력한다         
    // **************************************************************************
	x2 = w;
	y2 = mn_y_diff[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	y2 = mn_y_space[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = mn_y_space[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e1, RGN_DIFF);
	e2.OffsetRgn(0, mn_y_diff[i] - mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_OR);

	p.x = mn_pos_x[i][0];
	p.y = mn_pos_y[i][1] - mn_y_diff[i] * 3 + mn_y_space[i];
	mp_btn_lower[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_LOWER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_lower[i].EnableWindow(FALSE);

	e1.OffsetRgn(0, mn_y_space[i]*2);
	e2.OffsetRgn(0, -(mn_y_diff[i] - mn_y_space[i]*2));

	b1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // GREEN 램프 바로 밑에 존재하는 테두리 영역 계산하여 컨트롤에 출력한다      
    // **************************************************************************
	y2 = mn_y_space[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -mn_y_space[i]);
	b1.CombineRgn(&b1, &e1, RGN_OR);
	e2.OffsetRgn(0, -mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	p.y -= mn_y_space[i];
	mp_btn_step1[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP1+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	mp_btn_step1[i].EnableWindow(FALSE);
	e1.OffsetRgn(0, mn_y_space[i]);
	// **************************************************************************

	// **************************************************************************
    // 타워 GREEN 램프 출력 위치 계산하여 컨트롤에 출력한다                      
    // **************************************************************************
	y2 = mn_y_diff[i] - mn_y_space[i];
	b2.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, mn_y_diff[i] - mn_y_space[i]*3);
	b2.CombineRgn(&b2, &e1, RGN_OR);
	b2.CombineRgn(&b2, &e2, RGN_DIFF);

	CString title;  // 램프 상태 저장 변수

	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_g_lamp[1][i]==0) 
	{
		cr = G_OFF;
		title = "OFF";
	}
	else if (mn_g_lamp[1][i]==1)
	{
		cr = G_ON;
		title = "ON";
	}
	else if (mn_g_lamp[1][i]==2)
	{
		cr = G_OFF;
		title = "Flicker";
	}

	mp_btn_g_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_GLAMP+i, 1, cr, G_ON, cr, cr);
	e1.OffsetRgn(0, -(mn_y_diff[i] - mn_y_space[i]*3));
	// **************************************************************************

	// **************************************************************************
    // GREEN 램프와 YELLOW 램프 사이의 테두리 영역 계산하여 컨트롤에 출력한다    
    // **************************************************************************
	p.y -= mn_y_space[i];

	mp_btn_step2[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP2+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	mp_btn_step2[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // 타워 YELLOW 램프 출력 위치 계산하여 컨트롤에 출력한다                     
    // **************************************************************************
	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_y_lamp[1][i]==0) 
	{
		cr = Y_OFF;
		title = "OFF";
	}
	else if (mn_y_lamp[1][i]==1)
	{
		cr = Y_ON;
		title = "ON";
	}
	else if (mn_y_lamp[1][i]==2)
	{
		cr = Y_OFF;
		title = "Flicker";
	}

	mp_btn_y_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_YLAMP+i, 1, cr, Y_ON, cr, cr);
	// **************************************************************************

	// **************************************************************************
    // YELLOW 램프와 RED 램프 사이의 테두리 영역 계산하여 컨트롤에 출력한다      
    // **************************************************************************
	p.y -= mn_y_space[i];

	 mp_btn_step3[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP3+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	 mp_btn_step3[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // 타워 RED 램프 출력 위치 계산하여 컨트롤에 출력한다                        
    // **************************************************************************
	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_r_lamp[1][i]==0) 
	{
		cr = R_OFF;
		title = "OFF";
	}
	else if (mn_r_lamp[1][i]==1)
	{
		cr = R_ON;
		title = "ON";
	}
	else if (mn_r_lamp[1][i]==2)
	{
		cr = R_OFF;
		title = "Flicker";
	}

	mp_btn_r_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_RLAMP+i, 1, cr, R_ON, cr, cr);
	// **************************************************************************

	// **************************************************************************
    // RED 램프 위에 존재하는 플라스틱 영역 계산하여 컨트롤에 출력한다           
    // **************************************************************************
	p.y -= (mn_y_diff[i]-mn_y_space[i]);

	mp_btn_upper[i].Create("", WS_CHILD|WS_VISIBLE, p, b2, this, IDC_LAMP_UPPER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_upper[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // 타워 램프 맨 위 둥근 플라스틱 영역 계산하여 컨트롤에 출력한다             
    // **************************************************************************
	p.y -= mn_y_space[i]*2;

	mp_btn_roof[i].Create("", WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_ROOF+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_roof[i].EnableWindow(FALSE);
	// **************************************************************************
}

void CScreen_Set_Maintenance::OnMaintenance_Red_State_Change(int idx)
{
	//Wanring하고, Self Check는 숨긴다.
	if (idx == dINIT)	return;

	int mn_red = mn_r_lamp[1][idx];  // RED 상태 설정

	switch(mn_red)
	{
	case 0 :  // OFF
		mp_btn_r_lamp[idx].SetWindowText("OFF");
		mp_btn_r_lamp[idx].ChangeColor(R_OFF, R_ON, R_OFF, R_OFF);
		break;
	case 1 :  // ON
		mp_btn_r_lamp[idx].SetWindowText("ON");
		mp_btn_r_lamp[idx].ChangeColor(R_ON, R_ON, R_ON, R_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_r_lamp[idx].SetWindowText("FLICK");
		mp_btn_r_lamp[idx].ChangeColor(FLICK, R_ON, FLICK, R_ON);
		break;
	default :
		break;
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Yellow_State_Change(int idx)
{
	//Wanring하고, Self Check는 숨긴다.
	if (idx == dINIT)	return;

	int mn_yellow = mn_y_lamp[1][idx];  // YELLOW 상태 설정

	switch(mn_yellow)
	{
	case 0 :  // OFF
		mp_btn_y_lamp[idx].SetWindowText("OFF");
		mp_btn_y_lamp[idx].ChangeColor(Y_OFF, Y_ON, Y_OFF, Y_OFF);
		break;
	case 1 :  // ON
		mp_btn_y_lamp[idx].SetWindowText("ON");
		mp_btn_y_lamp[idx].ChangeColor(Y_ON, Y_ON, Y_ON, Y_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_y_lamp[idx].SetWindowText("FLICK");
		mp_btn_y_lamp[idx].ChangeColor(FLICK, Y_ON, FLICK, Y_OFF);
		break;
	default :
		break;
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Green_State_Change(int idx)
{
	//Wanring하고, Self Check는 숨긴다.
	if (idx == dINIT)	return;

	int mn_green = mn_g_lamp[1][idx];  // GREEN 상태 설정

	switch(mn_green)
	{
	case 0 :  // OFF
		mp_btn_g_lamp[idx].SetWindowText("OFF");
		mp_btn_g_lamp[idx].ChangeColor(G_OFF, G_ON, G_OFF, G_OFF);
		break;
	case 1 :  // ON
		mp_btn_g_lamp[idx].SetWindowText("ON");
		mp_btn_g_lamp[idx].ChangeColor(G_ON, G_ON, G_ON, G_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_g_lamp[idx].SetWindowText("FLICK");
		mp_btn_g_lamp[idx].ChangeColor(FLICK, G_ON, FLICK, G_OFF);
		break;
	default :
		break;
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Set()
{
	int i;
	
	for (i = 0; i <= SELFCHECK; i++)
	{
		mn_r_lamp[1][i] = st_lamp.mn_lamp_r[i];
		mn_y_lamp[1][i] = st_lamp.mn_lamp_y[i];
		mn_g_lamp[1][i] = st_lamp.mn_lamp_g[i];
	}

	mn_buzzer_mode[1] = st_lamp.mn_buzzer_mode;

	mn_lamp_flicker_time[1] = st_lamp.mn_lamp_wait_time;
	mn_lamp_change_time[1] = st_lamp.mn_lamp_change_time;

	mstr_equip_no[1] = st_lamp.mstr_equip_no;
	mstr_equip_id[1] = st_lamp.mstr_equip_id;

}

void CScreen_Set_Maintenance::OnMaintenance_Button_Set()
{
	short	shBtnColor = 30;

	// *************************************************************
	//  CHECK BUTTON		                                        
	// *************************************************************
	m_chk_buzzer.SetFont(Func.mp_maintenance_font);
	m_chk_buzzer.SetIcon(IDI_SOUND_ON, IDI_SOUND_OFF);
	m_chk_buzzer.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_buzzer.SetButtonColor(1);

}

void CScreen_Set_Maintenance::OnMaintenance_Buzzer_Change()
{
	if (mn_buzzer_mode[1] < 0 || mn_buzzer_mode[1] > 1)	mn_buzzer_mode[1] = TRUE;

	m_chk_buzzer.SetCheck(mn_buzzer_mode[1]);
	
	if (mn_buzzer_mode[1] == TRUE)	SetDlgItemText(IDC_CHK_BUZZER, "Buzzer On");
	else							SetDlgItemText(IDC_CHK_BUZZER, "Buzzer Off");
}

void CScreen_Set_Maintenance::OnMaintenance_EditBox_Set()
{	
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Apply()
{
	int i;

	for(i = 0; i <= SELFCHECK; i++)
	{
		// **********************************************************************
		// RED 타워 램프 상태 전역 변수에 설정한다                               
		// **********************************************************************
		if (mn_r_lamp[1][i]<0 || mn_r_lamp[1][i]>2) 
		{
			mn_r_lamp[1][i] = 0;
			OnMaintenance_Red_State_Change(i);     // RED 타워 램프 버튼 상태 변환 함수
		}
		st_lamp.mn_lamp_r[i] = mn_r_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW 타워 램프 상태 전역 변수에 설정한다                            
		// **********************************************************************
		if (mn_y_lamp[1][i]<0 || mn_y_lamp[1][i]>2) 
		{
			mn_y_lamp[1][i] = 0;
			OnMaintenance_Yellow_State_Change(i);  // YELLOW 타워 램프 버튼 상태 변환 함수
		}
		st_lamp.mn_lamp_y[i] = mn_y_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN 타워 램프 상태 전역 변수에 설정한다                             
		// **********************************************************************
		if (mn_g_lamp[1][i]<0 || mn_g_lamp[1][i]>2) 
		{
			mn_g_lamp[1][i] = 0;
			OnMaintenance_Green_State_Change(i);   // GREEN 타워 램프 버튼 상태 변환 함수
		}
		st_lamp.mn_lamp_g[i] = mn_g_lamp[1][i];
		// **********************************************************************
	}

	st_lamp.mn_buzzer_mode = mn_buzzer_mode[1];

	st_lamp.mn_lamp_wait_time = mn_lamp_flicker_time[1];
	st_lamp.mn_lamp_change_time = mn_lamp_change_time[1];

}

void CScreen_Set_Maintenance::OnMaintenance_Data_BackUp()
{
	int i;

	for(i=0; i<=SELFCHECK; i++)
	{
		// **********************************************************************
		// RED 타워 램프 상태를 백업한다                                         
		// **********************************************************************
		mn_r_lamp[0][i] = mn_r_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW 타워 램프 상태를 백업한다                                      
		// **********************************************************************
		mn_y_lamp[0][i] = mn_y_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN 타워 램프 상태를 백업한다                                       
		// **********************************************************************
		mn_g_lamp[0][i] = mn_g_lamp[1][i];
		// **********************************************************************
	}

	mn_buzzer_mode[0] = mn_buzzer_mode[1];

	mn_lamp_flicker_time[0] = mn_lamp_flicker_time[1];
	mn_lamp_change_time[0] = mn_lamp_change_time[1];

}

void CScreen_Set_Maintenance::OnMaintenance_ListBar_Info_Send()
{
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Display()
{
	m_dgt_flicker_time.SetValue(mn_lamp_flicker_time[1]);
	m_dgt_change_time.SetValue(mn_lamp_change_time[1]);
}

int CScreen_Set_Maintenance::OnMaintenance_Data_Comparison()
{
	int i;
	int Ret = RET_GOOD;

	for(i=0; i<=SELFCHECK; i++)
	{
		// **********************************************************************
		// RED 타워 램프 상태를 확인한다                                         
		// **********************************************************************
		if (mn_r_lamp[0][i] != mn_r_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************

		// **********************************************************************
		// YELLOW 타워 램프 상태를 확인한다                                      
		// **********************************************************************
		if (mn_y_lamp[0][i] != mn_y_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************

		// **********************************************************************
		// GREEN 타워 램프 상태를 확인한다                                       
		// **********************************************************************
		if (mn_g_lamp[0][i] != mn_g_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************
	}

	if (mn_buzzer_mode[0] != mn_buzzer_mode[1])					Ret = RET_ERROR;
	if (mn_lamp_flicker_time[0] != mn_lamp_flicker_time[1])		Ret = RET_ERROR;
	if (mn_lamp_change_time[0] != mn_lamp_change_time[1])		Ret = RET_ERROR;

	return Ret;
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Recovery()
{
	int i;

	for (i = 0; i <= SELFCHECK; i++)
	{
		// **********************************************************************
		// RED 타워 램프 상태를 원복한다                                         
		// **********************************************************************
		mn_r_lamp[1][i] = mn_r_lamp[0][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW 타워 램프 상태를 원복한다                                      
		// **********************************************************************
		mn_y_lamp[1][i] = mn_y_lamp[0][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN 타워 램프 상태를 원복한다                                       
		// **********************************************************************
		mn_g_lamp[1][i] = mn_g_lamp[0][i];
		// **********************************************************************
	}

	mn_buzzer_mode[1] = mn_buzzer_mode[0];

	mn_lamp_flicker_time[1] = mn_lamp_flicker_time[0];
	mn_lamp_change_time[1] = mn_lamp_change_time[0];
}

void CScreen_Set_Maintenance::OnMaintenance_Label_Set()
{
	CSxLogFont admin_font(15,FW_SEMIBOLD,false,"Arial");

	m_msg_tbox_code.SetFont(Func.mp_admin_font);
	m_msg_tbox_code.SetWindowText(_T("TBOX CODE"));
	m_msg_tbox_code.SetCenterText();
	m_msg_tbox_code.SetColor(RGB(0,0,255));
	m_msg_tbox_code.SetGradientColor(RGB(0,0,0));
	m_msg_tbox_code.SetTextColor(RGB(255,255,255));
	
	m_msg_acce_code.SetFont(Func.mp_admin_font);
	m_msg_acce_code.SetWindowText(_T("ACCY TRAY"));
	m_msg_acce_code.SetCenterText();
	m_msg_acce_code.SetColor(RGB(0,0,255));
	m_msg_acce_code.SetGradientColor(RGB(0,0,0));
	m_msg_acce_code.SetTextColor(RGB(255,255,255));
	if(st_handler.mn_level_teach == TRUE)
	{
		m_btn_id_send.ShowWindow(TRUE);
		
		CString strData="";
		strData.Format("%s", st_client[CLS_BOTTOM].ch_send);
		m_edit_send_data.SetWindowText( (LPCTSTR)strData );
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Digital_Count_Set()
{
	m_dgt_flicker_time.SubclassDlgItem(IDC_DGT_FLICKER_TIME, this);
	m_dgt_flicker_time.SetStyle(IDB_BIG3, 5);

	m_dgt_change_time.SubclassDlgItem(IDC_DGT_CHANGE_TIME, this);
	m_dgt_change_time.SetStyle(IDB_BIG3, 5);

}

void CScreen_Set_Maintenance::OnMaintenance_Lamp_Change()
{
	int i;

	for(i = 0; i <= SELFCHECK; i++)
	{
		OnMaintenance_Red_State_Change(i);      // RED 타워 램프 버튼 상태 변환 함수
		OnMaintenance_Yellow_State_Change(i);   // YELLOW 타워 램프 버튼 상태 변환 함수
		OnMaintenance_Green_State_Change(i);    // GREEN 타워 램프 버튼 상태 변환 함수
	}
}

void CScreen_Set_Maintenance::OnMaintenance_File_Backup(CString strTargetFile, CString strCreateFile, CString strBackupFolder, CString strBackupFile, BOOL b_over_write)
{
	CString str_backup_file="";	// 생성할 백업 파일의 [경로+파일명+확장자] 저장 변수
	int n_existence;			// 파일 존재 유무 플래그
	int n_response;				// 대화 상자 리턴 플래그
	
	char chr_file[256];
	char szDrive[4] = {'\0',};
	char szDir[MAX_PATH] = {'\0',};
	char szName[MAX_PATH] = {'\0',};
	char szExt[MAX_PATH] = {'\0',};

	CDialog_Select  select_dlg;

	// **************************************************************************
	// 백업할 파일이 이미 존재하는지 검사한다                                    
	// -> 이미 존재하는 경우에는 사용자에게 덮어쓸지 여부 묻게 된다              
	// **************************************************************************
	sprintf(chr_file, "%s", strCreateFile);

	n_existence = access(chr_file,0) ;

	if (n_existence != -1)  // 파일 존재
	{
		st_msg.mstr_confirm_msg = _T("Backup File Exist~! Overwrite??");

		n_response = select_dlg.DoModal();
		if (n_response == IDCANCEL)
			return ; 
	}
	// **************************************************************************

	// **************************************************************************
	// 백업 파일 생성한다                                                        
	// **************************************************************************
	_tsplitpath(strTargetFile, szDrive, szDir, szName, szExt); 
	str_backup_file.Format("%s\\%s%s", strBackupFolder, strBackupFile, szExt);
	
	CopyFile(strTargetFile, str_backup_file, !b_over_write);
	// **************************************************************************
}

// ******************************************************************************
//  Tower Lamp 화면 정보 갱신 요청 메시지                                        
// ******************************************************************************
LRESULT CScreen_Set_Maintenance::OnMaintenance_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnMaintenance_Data_Apply();			// Tower Lamp 화면 셋팅 정보 전역 변수에 전송 함수

	OnMaintenance_LogFile_Create();

	OnMaintenance_Data_BackUp();

	OnMaintenance_ListBar_Info_Send();   // 타이틀 바 화면에 정보 출력 요청하는 함수

	return 0;
}

void CScreen_Set_Maintenance::OnRLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_RLAMP;		// 배열 위치 설정

	mn_r_lamp[1][idx]++;
	if (mn_r_lamp[1][idx]<0 || mn_r_lamp[1][idx]>2)
		mn_r_lamp[1][idx] = 0;

	OnMaintenance_Red_State_Change(idx);		// RED 타워 램프 버튼 상태 변환 함수
}

void CScreen_Set_Maintenance::OnYLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_YLAMP;		// 배열 위치 설정
	
	mn_y_lamp[1][idx]++;
	if (mn_y_lamp[1][idx]<0 || mn_y_lamp[1][idx]>2)
		mn_y_lamp[1][idx] = 0;

	OnMaintenance_Yellow_State_Change(idx);	// YELLOW 타워 램프 버튼 상태 변환 함수
}

void CScreen_Set_Maintenance::OnGLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_GLAMP;		// 배열 위치 설정
	
	mn_g_lamp[1][idx]++;
	if (mn_g_lamp[1][idx]<0 || mn_g_lamp[1][idx]>2)
		mn_g_lamp[1][idx] = 0;

	OnMaintenance_Green_State_Change(idx);		// GREEN 타워 램프 버튼 상태 변환 함수
}

void CScreen_Set_Maintenance::OnDgtFlickerTime() 
{
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;

	mn_lamp_flicker_time[1] = m_dgt_flicker_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_lamp_flicker_time[1], chr_buf, 10));

	st_msg.mstr_keypad_msg = _T("Lamp Flicker Time Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(10, 10000, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	mn_lamp_flicker_time[1] = atoi(mstr_temp);
	m_dgt_flicker_time.SetValue(mn_lamp_flicker_time[1]);
}

void CScreen_Set_Maintenance::OnChkBuzzer() 
{
	if (m_chk_buzzer.GetCheck() == TRUE)	mn_buzzer_mode[1] = TRUE;
	else									mn_buzzer_mode[1] = FALSE;

	OnMaintenance_Buzzer_Change();
}

void CScreen_Set_Maintenance::OnBtnDataBackup() 
{
	CString str_year="", str_month="", str_day="";	// 문자형 년, 월, 일 저장 변수
	CString str_back_day="", str_back_time="";
	CString str_back_file="";					// 백업 파일명 저장 변수
	CString str_back_info="";					// 백업할 파일 정보 저장 변수 [폴더+파일명+확장자]
	CString str_temp_file="";					// 파일명 임시 저장 변수
	CString str_chk_ext="";					// 파일 확장자 저장 변수
	CString str_device="";
	int n_response;							// 대화 상자 리턴 플래그
	int n_year, n_month, n_day;				// 년, 월, 일 저장 변수
	CString str_cur_hour="", str_cur_min="", str_cur_sec="";
	
	char chr_buf[20];

	CDialog_Select   select_dlg;
	CDialog_Message  msg_dlg;

	st_msg.mstr_confirm_msg = _T("Do you back up environment Setting information?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		// ******************************************************************************
		// 백업 파일명으로 사용할 날짜 정보 설정한다                                     
		// ******************************************************************************
		COleDateTime cur_time = COleDateTime::GetCurrentTime();
		n_year = cur_time.GetYear();  
		n_month = cur_time.GetMonth();  
		n_day = cur_time.GetDay();  

		str_year = LPCTSTR(_itoa( n_year, chr_buf, 10 ));  
		str_month.Format("%02d", n_month);
		str_day.Format("%02d", n_day);

		str_back_day = str_year;
		str_back_day += str_month;
		str_back_day += str_day;
		// ******************************************************************************

		// **************************************************************************
		// 현재 시간 정보 얻는다                                                     
		// **************************************************************************
		str_cur_hour.Format("%02d",cur_time.GetHour());	// 현재 시간 정보를 설정한다.
		str_cur_min.Format("%02d",cur_time.GetMinute());	// 현재 분 정보를 설정한다. 
		str_cur_sec.Format("%02d",cur_time.GetSecond());	// 현재 초 정보를 설정한다. 

		str_back_time = str_cur_hour;
		str_back_time += str_cur_min;
		str_back_time += str_cur_sec;
		// **************************************************************************

		// ******************************************************************************
		// 베이직 셋팅 정보 저장 파일 백업한다                                           
		// -> 백업할 원본 파일이 존재하는지 검사하게 된다                                
		// ******************************************************************************
		str_back_file = "c:\\Backup\\" + str_back_day + "_" + str_back_time;  // 백업 파일명 설정 [날짜+파일명]

		CopyFolder(st_path.mstr_path_dvc, str_back_file);
		CopyFolder(st_path.mstr_basic_folder, str_back_file);
	}
}

void CScreen_Set_Maintenance::OnBtnPasswordChangeLevle1() 
{
	int n_response;  // 대화 상자 리턴 플래그

	CDialog_Pass_Change  pass_cng_dlg;

	st_handler.mstr_pass_level=_T("Level1");  // 암호 레벨 : 메인트 설정
	n_response = pass_cng_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CScreen_Set_Maintenance::OnBtnPasswordChangeLevle2() 
{
	int n_response;  // 대화 상자 리턴 플래그

	CDialog_Pass_Change  pass_cng_dlg;

	st_handler.mstr_pass_level=_T("Level2");  // 암호 레벨 : 티칭 설정
	n_response = pass_cng_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CScreen_Set_Maintenance::OnBtnMaintenanceApply() 
{
	int n_response;		// 대화 상자 리턴 플래그

	CDialog_Select  select_dlg;

	st_msg.mstr_confirm_msg = _T("Maintenance : Setting Data Apply?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		OnMaintenance_Data_Apply();			// Tower Lamp 화면 셋팅 정보 전역 변수에 전송 함수

		OnMaintenance_LogFile_Create();

		OnMaintenance_Data_BackUp();

		OnMaintenance_ListBar_Info_Send();	// 리스트 바 화면에 정보 출력 요청하는 함수
	}
	else if (n_response == IDCANCEL)
	{

	}
}

void CScreen_Set_Maintenance::OnMaintenance_LogFile_Create()
{
	int i;
	CString str_msg="";
	CString str_data="";
	CString str_mode="";
	int Ret = RET_GOOD;

	for(i=0; i<=SELFCHECK; i++)
	{
		if (i == 0)			str_mode = "STOP";
		else if (i == 1)	str_mode = "RUN";
		else if (i == 2)	str_mode = "JAM";
		else if (i == 3)	str_mode = "LOT END";
		else if (i == 4)	str_mode = "INIT";
		else if (i == 5)	str_mode = "WARNING";
		else if (i == 6)	str_mode = "LOCK";
//		else if (i == 7)	str_mode = "SELF CHECK";
		else if (i == 7)	str_mode = "EMPTY RUN";
		
		// **********************************************************************
		// RED 타워 램프 상태를 확인한다                                         
		// **********************************************************************
		if (mn_r_lamp[0][i] != mn_r_lamp[1][i])
		{
			if (mn_r_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_r_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_r_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : RED LAMP was changed by %s", str_mode, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		// **********************************************************************

		// **********************************************************************
		// YELLOW 타워 램프 상태를 확인한다                                      
		// **********************************************************************
		if (mn_y_lamp[0][i] != mn_y_lamp[1][i])
		{
			if (mn_y_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_y_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_y_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : YELLOW LAMP was changed by %s", str_mode, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		// **********************************************************************

		// **********************************************************************
		// GREEN 타워 램프 상태를 확인한다                                       
		// **********************************************************************
		if (mn_g_lamp[0][i] != mn_g_lamp[1][i])
		{
			if (mn_g_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_g_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_g_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : GREEN LAMP was changed by %s", str_mode, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		// **********************************************************************
	}

	if (mn_buzzer_mode[0] != mn_buzzer_mode[1])
	{
		if (mn_buzzer_mode[1] == 0)
		{
			str_data = _T("On");
		}
		else
		{
			str_data = _T("Off");
		}
		
		str_msg = "[TowerLamp] Buzzer Mode was changed by " + str_data;
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lamp_flicker_time[0] != mn_lamp_flicker_time[1])
	{
		str_data.Format("%d", (int)mn_lamp_flicker_time[1]);
		str_msg = "[TowerLamp] Lamp Flicker Time was changed by " + str_data + "(msec)";
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lamp_change_time[0] != mn_lamp_change_time[1])
	{
		str_data.Format("%d", (int)mn_lamp_change_time[1]);
		str_msg = "[TowerLamp] Lamp Change Time was changed by " + str_data + "(Sec)";
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mstr_equip_no[0] != mstr_equip_no[1])
	{
		str_msg = "[TowerLamp] EQUIP NO was changed by " + mstr_equip_no[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mstr_equip_id[0] != mstr_equip_id[1])
	{
		str_msg = "[TowerLamp] EQUIP ID was changed by " + mstr_equip_id[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
}

void CScreen_Set_Maintenance::OnDgtChangeTime() 
{
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;

	mn_lamp_change_time[1] = m_dgt_change_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_lamp_change_time[1], chr_buf, 10));

	st_msg.mstr_keypad_msg = _T("Lamp Change Time Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(0, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	mn_lamp_change_time[1] = atoi(mstr_temp);
	m_dgt_change_time.SetValue(mn_lamp_change_time[1]);
}

void CScreen_Set_Maintenance::OnBtnStepSave() 
{
	char path[MAX_PATH] = {0};
	//현재의 작업경로를 얻어와 저장 한다.
	GetCurrentDirectory(MAX_PATH, path);
	
	CFileDialogST	dlg(FALSE, NULL, "Step_back", OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, _T("Text File(*.txt)\0*.txt\0"), this);
	CString			sPathName, str_temp_file, str_ext, str_chk_ext;
	int				nRetValue, n_pos;
	
	BOOL			bResult; // = find.FindFile(str_file);
	
	CFileFind find;
	
	nRetValue = dlg.DoModal();
	
	if (nRetValue == IDOK)
	{
		sPathName = dlg.GetPathName();   // 화면에 보여지는 내용을 저장할 경로 및 파일의 이름을 얻었다!!
		
		n_pos = sPathName.Find(".txt");
		if(n_pos == -1)
			n_pos = sPathName.Find(".TXT");
		if(n_pos == -1)
			sPathName += ".txt";
		
		
		bResult = find.FindFile(sPathName);
		find.Close();
		
		if(bResult != 0)
		{
			DeleteFile(sPathName);
		}
		
		(st_msg.str_step_savefile_name).Format("%s", dlg.GetFileName());
		(st_msg.str_step_savefile_path).Format("%s", dlg.GetFileDir());

		OnMaintenance_Step_Save();
		
		SetCurrentDirectory(path);						// 작업 경로를 원래 경로로 복구 한다.
	}
	else if (nRetValue == IDCANCEL)
	{
		
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Step_Save()
{
	CString mstr_file_name="";    // 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file="";  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name="", mstr_temp_data="";  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content="";  // 한 라인에 출력할 문자열 정보 저장 변수 
	int mn_existence, i;      // 파일 존재 유무 설정 플래그 
	char fileName[256];    // 검색할 파일 정보 설정 함수 
	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
	
	CString str_title_time="", str_file_time="", str_new_file="";		// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	int n_cur_hour, n_cur_minute, n_cur_second;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수

	CString str_temp_data="";
	int n_length;

	CString strData="", str_time="";
	
	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("DATA%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_msg.str_step_savefile_path + st_msg.str_step_savefile_name;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* 파일 미존재 */
	{
		mstr_create_file = st_msg.str_step_savefile_path + st_msg.str_step_savefile_name;
		mstr_create_file += ".TXT";
		/* ********************************************************************** */
	}
	
	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("5.File Open Failed!..");
		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 파일에 타이틀 정보 출력한다                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT8590 [SSD Packing Handler] : " + g_ver.GetDate() );
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Thread Step Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_msg.str_step_savefile_path + st_msg.str_step_savefile_name);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content = "+==========================+=============================================================================================================================+";
	fprintf(fp,"%s\r\n",mstr_content);
	
	mstr_content = "|  Step Trace              |                                                    History                                                                  |";
	fprintf(fp,"%s\n", mstr_content );
	
	mstr_content = "+==========================+=============================================================================================================================+";
	fprintf(fp,"%s\r\n",mstr_content) ;
	
	str_temp_data = "";
	
	for (i = 0; i < 100; i++)
	{
		str_temp_data = st_work.mstr_last_change_date[i];
		mstr_content.Format("|  Step %02d Change LastDate |", i);
		Func.TextAndLine(130, mstr_content, str_temp_data, fp);
		
		if (st_work.mstr_history_step_back_up[i] != "")
		{
			str_temp_data = st_work.mstr_history_step_back_up[i];
			n_length = str_temp_data.GetLength();
			mstr_content.Format("|  Step %02d before History  |", i);
			mstr_content += str_temp_data;
			fprintf(fp,"%s\r\n", mstr_content);
			mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
			fprintf(fp,"%s\r\n",mstr_content);
		}
		str_temp_data = st_work.mstr_history_step[i];
		n_length = str_temp_data.GetLength();
		mstr_content.Format("|  Step %02d History         |", i);

		if (str_temp_data == "")
		{
			strData.Format("%-124s |", "");
			mstr_content += strData;
			fprintf(fp,"%s\r\n", mstr_content) ;
		}
		else
		{
			mstr_content += str_temp_data;
			fprintf(fp,"%s\r\n", mstr_content);
		}
		mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		fprintf(fp,"%s\r\n",mstr_content);
	}
	
	if (ferror(fp))  
	{
		Func.MsgLog("file save failed!..") ;
		fclose(fp);
		return ;
	}
	
	fclose(fp);  /* 파일 종료 */
}

BOOL CScreen_Set_Maintenance::CopyFolder(CString &strFrom, CString &strTo)
{	
	CFileFind finder;
	
    CString sWildCard = strFrom + "\\*.*";
	
    BOOL bFound;
    BOOL bWorking = finder.FindFile(sWildCard);
	
    bFound = bWorking;
	
    ::CreateDirectory(strTo, NULL);
	
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		
        // skip . and .. files; otherwise, we"d
        // recur infinitely!
        if(finder.IsDots()) continue;
		
        // 디렉토리이면 재귀 호출
        if(finder.IsDirectory())
        {
            CString sFrom2 = finder.GetFilePath();
            CString sTo2 = strTo + "\\" + finder.GetFileName();
            ::CreateDirectory(sTo2, NULL);
            
            CopyFolder(sFrom2, sTo2);
            continue;
        }
        ExCopyFile(finder.GetFilePath(), strTo);
    }
    return bFound;

}

int CScreen_Set_Maintenance::ExCopyFile(CString strFrom, CString strTo)
{
	SHFILEOPSTRUCT shfo;
	
    char pszFrom[1024] = {0};
    char pszTo[1024] = {0};
	
    WORD wFlag = 0;
	
    strcpy(pszFrom, (LPCTSTR)strFrom);
    strcpy(pszTo, (LPCTSTR)strTo);
	
    pszFrom[lstrlen(pszFrom) + 1] = 0;
	
    shfo.hwnd = m_hWnd;
    shfo.wFunc = FO_COPY;
    shfo.pFrom = pszFrom;
    shfo.pTo = pszTo;
    shfo.fFlags = 0;
    shfo.lpszProgressTitle = "File Copy..";
	
    return SHFileOperation(&shfo);

}



void CScreen_Set_Maintenance::OnBtnBcr1() 
{
//	CString strBcrCommand;
//	
//	st_work.strBarcodeRecive = "";//수신 메세지 초기화
//
//	strBcrCommand.Format("%cR%c", 0x02, 0x03);//바코드 리더기 읽기 시작 명령
//	st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
//	::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
//	::Sleep(100);

// 	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹U E­¸e A¸Ac
// 	{
// 		sprintf(st_msg.c_normal_msg,"[BCR1 Test] Continuous mode");//,strBcrCommand);
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // μ¿AU ½CÆÐ Aa·A ¿aA≫
// 	}
	Func.OnBcrInputreader();
}

void CScreen_Set_Maintenance::OnBtnBcr2() 
{
//	CString strBcrCommand;
//	
//	st_work.strBarcodeRecive = "";//수신 메세지 초기화
//
//	strBcrCommand.Format("%cR%c", 0x02, 0x03);//바코드 리더기 읽기 시작 명령
//	st_serial.mstr_snd[BCR_PORT2-1] = strBcrCommand;
//	::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
//	::Sleep(100);

// 	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹U E­¸e A¸Ac
// 	{
// 		sprintf(st_msg.c_normal_msg,"[BCR2 Test] Continuous mode");//,strBcrCommand);
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // μ¿AU ½CÆÐ Aa·A ¿aA≫
// 	}		
	Func.OnBcrOutputreader();
}

void CScreen_Set_Maintenance::OnBtnBcr4() 
{
//	clsBarcode2.OnBarcodeReset();
	::PostMessage(st_handler.hWnd, WM_COMM_WORK, 3, BCR_PORT2-1);
	
}

void CScreen_Set_Maintenance::OnBtnBcr5() 
{
//	clsBarcode.OnOpen(BCR_PORT-1, 9600, NOPARITY, 8, ONESTOPBIT, ' ');
	::PostMessage(st_handler.hWnd, WM_COMM_WORK, 4, BCR_PORT-1);
	
}

void CScreen_Set_Maintenance::OnBtnBcr6() 
{
//	clsBarcode2.OnOpen(BCR_PORT2-1, 9600, NOPARITY, 8, ONESTOPBIT, ' ');
	::PostMessage(st_handler.hWnd, WM_COMM_WORK, 4, BCR_PORT2-1);
	
}

void CScreen_Set_Maintenance::OnBtnBcr3() 
{
//	clsBarcode.OnBarcodeReset();	
	::PostMessage(st_handler.hWnd, WM_COMM_WORK, 3, BCR_PORT-1);
}

void CScreen_Set_Maintenance::OnBtnGms() 
{
	::PostMessage(st_handler.hWnd, WM_COMM_WORK, 4, GMS_PORT-1);
	
}

void CScreen_Set_Maintenance::OnBtnCloseGms() 
{
	::PostMessage(st_handler.hWnd, WM_COMM_WORK, 3, GMS_PORT-1);
	
}

void CScreen_Set_Maintenance::StepClear()
{
	m_step_labrlcomm = 0;
	m_step_bcrcomm = 0;
	m_step_tviscomm = 0;
	m_step_bviscomm = 0;
	m_nRetryCnt = 0;
	m_bstop = false;
}

int	CScreen_Set_Maintenance::OnVisionModelChange()
{
	int nFuncRet = RET_PROCEED;
	if(m_bstop == true) return RET_ERROR;
	switch(m_nInterfaceStep)
	{
		case 0:
			if(st_client[m_nClientPos].n_connect == CTL_YES)
			{
				m_nInterfaceStep = 200;
			}
			else
			{
				m_dwTimeChangeWork = GetCurrentTime();
				m_nInterfaceStep = 100;

				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CONNECT, m_nClientPos);	
			}
			break;
			
		case 100:
			if(st_client[m_nClientPos].n_connect == CTL_YES)
			{
				m_nInterfaceStep = 200;
			}
			else
			{	
				if(GetCurrentTime() - m_dwTimeChangeWork < 0)
				{
					m_dwTimeChangeWork = GetCurrentTime();
					break;
				}
				
				if(GetCurrentTime() - m_dwTimeChangeWork > 5000)
				{
					m_nInterfaceStep = 0;					
					return RET_ERROR;
				}
			}
			break;
			
		case 200:
			clsVsClient[m_nClientPos].m_nModelChangeBin		= CTL_CLEAR;
			st_client[m_nClientPos].n_rev_info				= CTL_YES;			
			m_dwTimeChangeWork = GetCurrentTime();
			
			clsVsClient[m_nClientPos].OnVisionModelChange(m_nClientPos, st_basic.mstr_device_name);
			
			m_nInterfaceStep = 300;
			break;
			
		case 300:		
			if(clsVsClient[m_nClientPos].m_nModelChangeBin	 == NVR_PASS)
			{
				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CLOSE, m_nClientPos);
				
				m_nInterfaceStep = 0;
				
				return RET_GOOD;
			}
			else if(clsVsClient[m_nClientPos].m_nModelChangeBin== NVR_FAIL)
			{
				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CLOSE, m_nClientPos);
				
				m_nInterfaceStep = 0;
				
				return RET_ERROR;
			}
			else 
			{
				if(GetCurrentTime() - m_dwTimeChangeWork < 0)
				{
					m_dwTimeChangeWork = GetCurrentTime();
					break;
				}
				
				if(GetCurrentTime() - m_dwTimeChangeWork  > 5000)
				{
//					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CLOSE, m_nClientPos);
					m_nInterfaceStep = 0;					
					return RET_ERROR;
				}
			}
			break;
	}

	return RET_PROCEED;
}


int CScreen_Set_Maintenance::Run_Move_Label()
{
	int nFuncRet = RET_PROCEED;

	if(m_bstop == true) return RET_ERROR;

	switch( m_step_labrlcomm)
	{
	case 0:
		if (st_client[CLS_VIS_DENSITY].n_connect == CTL_YES || st_client[CLS_VIS_BARCODE].n_connect == CTL_YES)
		{
// 			g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_ON);
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 11);
// 			m_dwTimeNetwork = GetCurrentTime();
// 			m_step_labrlcomm = 1000;
		}
		else
		{
			m_dwTimeNetwork = GetCurrentTime();

//			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CONNECT, m_nClientPos);	
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CONNECT, 10);
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CONNECT, 11);

			m_step_labrlcomm = 100;
		}
		break;

	case 100:
		if (st_client[CLS_VIS_DENSITY].n_connect == CTL_YES && st_client[CLS_VIS_BARCODE].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_labrlcomm = 1000;
		}
		else
		{
			if (GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}

			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;

					// 700010 1 A "Density label1 time out from server."
					CTL_Lib.Alarm_Error_Occurrence(1200, CTL_dWARNING, "700010" );
					nFuncRet = RET_ERROR;
					m_step_labrlcomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetTickCount();
					m_step_labrlcomm = 200;
				}
			}
		}
		break;

	case 200:		
		if (GetCurrentTime() - m_dwTimeNetwork <= 0)
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}
		
		if (GetCurrentTime() - m_dwTimeNetwork > 5000)
		{
			m_step_labrlcomm = 0;
		}
		break;

	case 1000:
		if (GetCurrentTime() - m_dwTimeNetwork <= 0) m_dwTimeNetwork = GetCurrentTime();
		else if(GetCurrentTime() - m_dwTimeNetwork < 300) break;
		g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_OFF);


		if (GetCurrentTime() - m_dwTimeNetwork <= 0) m_dwTimeNetwork = GetCurrentTime();
		else if(GetCurrentTime() - m_dwTimeNetwork < 500) break;
		g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);


// 		Func.m_pVsClient[EVP_DENSITY]->SetResultBcrTest(NVR_NONE);
// 		Func.m_pVsClient[EVP_DENSITY]->OnVisionBcrResult(10);

		m_step_labrlcomm = 1100;
		break;

	case 1100:
		if (Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin == NVR_PASS)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "DENSITY_Result:PASS, Model:%s, CAPA: %s, 2D:%s", 
					Func.m_pVsClient[EVP_DENSITY]->m_strModel,  Func.m_pVsClient[EVP_DENSITY]->m_strCapa, Func.m_pVsClient[EVP_DENSITY]->m_str2D );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
			m_step_labrlcomm = 3000;
			m_step_labrlcomm = 1200;
		}
		else if (Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin == NVR_FAIL)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "DENSITY_Result:PASS, Model:%s, CAPA: %s, 2D:%s", 
					Func.m_pVsClient[EVP_DENSITY]->m_strModel,  Func.m_pVsClient[0]->m_strCapa, Func.m_pVsClient[0]->m_str2D );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
			m_step_labrlcomm = 3000;
			m_step_labrlcomm = 1200;
		}
		else
		{
			if (GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}
			
			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;					
					// 700010 1 A "Density label1 time out from server."
					CTL_Lib.Alarm_Error_Occurrence(1210, CTL_dWARNING, "700010" );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
					nFuncRet = RET_ERROR;
					m_step_labrlcomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetTickCount();					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
					m_step_labrlcomm = 2000;
				}
			}
		}
		break;

	case 1200:
		if (Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin == NVR_PASS)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "DENSITY_Result:PASS, Model:%s, CAPA: %s, 2D:%s", 
					Func.m_pVsClient[EVP_BARCODE]->m_strModel,  Func.m_pVsClient[EVP_BARCODE]->m_strCapa, Func.m_pVsClient[EVP_BARCODE]->m_str2D );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 10);
			m_step_labrlcomm = 3000;
		}
		else if (Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin == NVR_FAIL)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "DENSITY_Result:PASS, Model:%s, CAPA: %s, 2D:%s", 
					Func.m_pVsClient[EVP_BARCODE]->m_strModel,  Func.m_pVsClient[EVP_BARCODE]->m_strCapa, Func.m_pVsClient[EVP_BARCODE]->m_str2D );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 10);
			m_step_labrlcomm = 3000;
		}
		else
		{
			if (GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}
			
			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;					
					// 700010 1 A "Density label1 time out from server."
					CTL_Lib.Alarm_Error_Occurrence(1210, CTL_dWARNING, "700010" );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
					nFuncRet = RET_ERROR;
					m_step_labrlcomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetTickCount();					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CLOSE, 10);
					m_step_labrlcomm = 2000;
				}
			}
		}
		break;

	case 2000:
		if (GetCurrentTime() - m_dwTimeNetwork <= 0) m_dwTimeNetwork = GetCurrentTime();		
		if (GetCurrentTime() - m_dwTimeNetwork > 5000)
		{
			m_step_labrlcomm = 0;
		}
		break;	
		
	case 3000:
		m_step_labrlcomm = 0;
		//nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CScreen_Set_Maintenance::Run_Move_Barcode()
{
	int nFuncRet = RET_PROCEED;

	if(m_bstop == true) return RET_ERROR;
	switch( m_step_bcrcomm)
	{
	case 0:
		if (st_client[CLS_VIS_BARCODE].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_bcrcomm = 1000;
		}
		else
		{
			m_dwTimeNetwork = GetCurrentTime();
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CONNECT, 11);

			m_step_bcrcomm = 100;
		}
		break;

	case 100:
		if (st_client[CLS_VIS_BARCODE].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_bcrcomm = 1000;
		}
		else
		{
			if (GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}

			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					CString strJamCode;
					m_nRetryCnt = 0;

					CTL_Lib.Alarm_Error_Occurrence(1220, CTL_dWARNING, "700200" );
					m_step_bcrcomm = 0;
					nFuncRet = RET_ERROR;
				}
				else
				{
					m_dwTimeNetwork = GetCurrentTime();
					m_step_bcrcomm = 200;
				}
			}
		}
		break;

	case 200:	
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}
		
		if (GetCurrentTime() - m_dwTimeNetwork > 5000)
		{
			m_step_bcrcomm = 0;
		}
		break;

	case 1000:
		if (GetCurrentTime() - m_dwTimeNetwork <= 0) m_dwTimeNetwork = GetCurrentTime();
		if (GetCurrentTime() - m_dwTimeNetwork < 500) break;
		g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);

		Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin = NVR_NONE;
		Func.m_pVsClient[EVP_BARCODE]->OnVisionBcrResult(11);
		m_dwTimeNetwork = GetCurrentTime();

		m_step_bcrcomm = 1100;
		break;

	case 1100:
		if (Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin == NVR_PASS)
		{
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 11);
			m_step_bcrcomm = 3000;
		}
		else if (Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin == NVR_FAIL)
		{
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 11);
			m_step_bcrcomm = 3000;
		}
		else
		{
			if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}
			
			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					CString strJamCode;
					m_nRetryCnt = 0;
					
					CTL_Lib.Alarm_Error_Occurrence(1230, CTL_dWARNING, "700200" );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 11);
					m_step_bcrcomm = 0;
					nFuncRet = RET_ERROR;
				}
				else
				{
					m_dwTimeNetwork = GetCurrentTime();
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, 11);
					m_step_bcrcomm = 2000;
				}
			}
		}
		break;

	case 2000:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}
		
		if (GetCurrentTime() - m_dwTimeNetwork> 5000)
		{
			m_step_bcrcomm = 0;
		}
		break;
	case 3000:
		if( Func.m_pVsClient[EVP_BARCODE]->GetResultBcrTest() == NVR_PASS)//비젼 동작 완료 
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "BARCODE_Result:PASS Model:%s, Serial:%s", Func.m_pVsClient[EVP_BARCODE]->m_strModel, Func.m_pVsClient[EVP_BARCODE]->GetResultSerial() );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else if(Func.m_pVsClient[EVP_BARCODE]->GetResultBcrTest() == NVR_FAIL)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "BARCODE_Result:FAIL Model:%s, Serial:%s", Func.m_pVsClient[EVP_BARCODE]->m_strModel, Func.m_pVsClient[EVP_BARCODE]->GetResultSerial() );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}

		nFuncRet = RET_GOOD;
		m_step_bcrcomm = 0;
		break;
	}

	return nFuncRet;
}



int CScreen_Set_Maintenance::Run_Move_TopVision()
{
	int nFuncRet = RET_PROCEED;

	if(m_bstop == true) return RET_ERROR;
	switch(m_step_tviscomm) 
	{
	case 0:
		if (st_client[CLS_VIS_TOP].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_tviscomm = 1000;
		}
		else
		{
			m_dwTimeNetwork = GetCurrentTime();
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CONNECT, 12);
			m_step_tviscomm = 100;
		}
		break;

	case 100:
		if (st_client[CLS_VIS_TOP].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_tviscomm = 1000;
		}
		else
		{
			if ( GetCurrentTime() - m_dwTimeNetwork <= 0 )
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}

			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;
					//701000 1 A "Received incorrect topvision1 from server."
					CTL_Lib.Alarm_Error_Occurrence(1240, CTL_dWARNING, "701000" );
					nFuncRet = RET_ERROR;
					m_step_tviscomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetCurrentTime();
					m_step_tviscomm = 200;
				}
			}
		}
		break;

	case 200:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0 )
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}

		if (GetCurrentTime() - m_dwTimeNetwork > 5000)
		{
			m_step_tviscomm = 0;
		}
		break;

	case 1000:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0 )
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}
		if (GetCurrentTime() - m_dwTimeNetwork < 500 ) break;
		g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_OFF);
				
		Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin = NVR_NONE;
		Func.m_pVsClient[EVP_TOP_POS]->OnVisionBcrResult(12);

		m_dwTimeNetwork = GetCurrentTime();
		m_step_tviscomm = 1100;
		break;

	case 1100:
		if (Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin == NVR_PASS)
		{
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, 12);

			m_step_tviscomm = 3000;
		}
		else if (Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin == NVR_FAIL)
		{
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, 12);

			m_step_tviscomm = 3000;
		}
		else
		{
			if ( GetCurrentTime() - m_dwTimeNetwork <= 0 )
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}

			if (GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;					
					//702000 1 A "Topvision1 time out from server."
					CTL_Lib.Alarm_Error_Occurrence(1250, CTL_dWARNING, "702000" );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, 12);
					nFuncRet = RET_ERROR;
					m_step_tviscomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetCurrentTime();					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, 12);
					m_step_tviscomm = 2000;
				}
			}
		}
		break;

	case 2000:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0 )
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}

		if (GetCurrentTime() - m_dwTimeNetwork > 5000)
		{
			m_step_tviscomm = 0;
		}
		break;


	case 3000:
		if( Func.m_pVsClient[EVP_TOP_POS]->GetResultBcrTest() == NVR_PASS)//비젼 동작 완료 
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "TOPVISION_Result:PASS Model:%s, Serial:%s", Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else if(Func.m_pVsClient[EVP_TOP_POS]->GetResultBcrTest() == NVR_FAIL)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "TOPVISION_Result:FAIL Model:%s, Serial:%s", Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		nFuncRet = RET_GOOD;
		m_step_tviscomm = 0;
		break;
	
	}

	return nFuncRet;
}

int CScreen_Set_Maintenance::Run_Move_BtmVision()
{
	int nFuncRet = RET_PROCEED;

	if(m_bstop == true) return RET_ERROR;
	switch(m_step_bviscomm) 
	{

	case 0:
		if (st_client[CLS_VIS_BTM].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_bviscomm = 1000;
		}
		else
		{
			m_dwTimeNetwork = GetCurrentTime();
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CONNECT, 13);
			m_step_bviscomm = 100;
		}
		break;

	case 100:
		if (st_client[CLS_VIS_BTM].n_connect == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork = GetCurrentTime();
			m_step_bviscomm = 1000;
		}
		else
		{
			if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}

			if ( GetCurrentTime() - m_dwTimeNetwork > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;

					// 703000 1 A "Received incorrect btmvision1 from server."
					CTL_Lib.Alarm_Error_Occurrence(1260, CTL_dWARNING, "703000" );
					m_step_bviscomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetCurrentTime();
					m_step_bviscomm = 200;
				}
			}
		}
		break;

	case 200:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}
		
		if ( GetCurrentTime() - m_dwTimeNetwork > 5000)
		{
			m_step_bviscomm = 0;
		}
		break;

	case 1000:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}		
		if ( GetCurrentTime() - m_dwTimeNetwork < 500) break;
		g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_OFF);
			
		Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin = NVR_NONE;
		Func.m_pVsClient[EVP_BTM_POS]->OnVisionBcrResult(13);

		m_step_bviscomm = 1100;
		break;

	case 1100:
		if (Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin == NVR_PASS)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "BTMVISION_Result:PASS, Model:%s ", Func.m_pVsClient[EVP_BTM_POS]->m_strModel );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, 13);

			m_step_bviscomm = 3000;
		}
		else if (Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin == NVR_FAIL)
		{
			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "BTMVISION_Result:FAIL, Model:%s ", Func.m_pVsClient[EVP_BTM_POS]->m_strModel );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, 13);

			m_step_bviscomm = 3000;
		}
		else
		{
			if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
			{
				m_dwTimeNetwork = GetCurrentTime();
				break;
			}		
			if ( GetCurrentTime() - m_dwTimeNetwork  > 3000)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt >= 3)
				{
					m_nRetryCnt = 0;
					
					// 704000 1 A "Btmvision1 time out from server."
					CTL_Lib.Alarm_Error_Occurrence(1270, CTL_dWARNING, "704000" );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, 13);

					m_step_bviscomm = 0;
				}
				else
				{
					m_dwTimeNetwork = GetCurrentTime();					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, 13);
					m_step_bviscomm = 2000;
				}
			}
		}
		break;

	case 2000:
		if ( GetCurrentTime() - m_dwTimeNetwork <= 0)
		{
			m_dwTimeNetwork = GetCurrentTime();
			break;
		}
		if ( GetCurrentTime() - m_dwTimeNetwork> 5000)
		{
			m_step_bviscomm = 0;
		}
		break;

	case 3000:
		{
//			g_client_vision[EVP_BTM_POS].SetBcrTest();
			m_step_bviscomm = 0;
			nFuncRet = RET_GOOD;
		}
		break;

	}

	return nFuncRet;

}

void CScreen_Set_Maintenance::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int nRet = RET_PROCEED;
	CDialog_Message  msg_dlg;
	if( nIDEvent == TMR_LABEL)
	{
		nRet = Run_Move_Label();
		if( nRet == RET_GOOD || nRet == RET_ERROR)
		{
			KillTimer(TMR_LABEL);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
			st_msg.str_fallacy_msg = _T("Label Vision Stop!!!");
			msg_dlg.DoModal();

		}
	}
	else if( nIDEvent == TMR_BCR)
	{
		nRet = Run_Move_Barcode();
		if( nRet == RET_GOOD || nRet == RET_ERROR)
		{
			KillTimer(TMR_BCR);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
			st_msg.str_fallacy_msg = _T("Barcode Vision Stop!!!");
			msg_dlg.DoModal();
		}

	}
	else if( nIDEvent == TMR_TOP)
	{
		nRet = Run_Move_TopVision();
		if( nRet == RET_GOOD || nRet == RET_ERROR)
		{
			KillTimer(TMR_TOP);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
			st_msg.str_fallacy_msg = _T("Top Vision Stop!!!");
			msg_dlg.DoModal();
		}
		
	}
	else if( nIDEvent == TMR_BTM)
	{
		nRet = Run_Move_BtmVision();
		if( nRet == RET_GOOD || nRet == RET_ERROR)
		{
			KillTimer(TMR_BTM);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
			st_msg.str_fallacy_msg = _T("Btm Vision Stop!!!");
			msg_dlg.DoModal();
		}
		
	}
	else if( nIDEvent == TM_MODEL_CHANGE )
	{
		nRet = OnVisionModelChange();

		if ( nRet == RET_GOOD || nRet == RET_ERROR)
		{
			KillTimer(TM_MODEL_CHANGE);
			if (st_handler.mn_menu_lock == TRUE)
			{
				st_handler.mn_menu_lock = FALSE;
			}
			st_msg.str_fallacy_msg = _T("Model Change Stop!!!");
			msg_dlg.DoModal();
		}
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Set_Maintenance::OnButtonStop() 
{
	m_bstop = true;
	
}

void CScreen_Set_Maintenance::OnBtnDensityModel() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 10;//2014.1114  6 -> 10
	m_nInterfaceStep	= 0;
	m_bstop = false;

	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL_CHANGE, 100, NULL);
}

void CScreen_Set_Maintenance::OnBtnDensityData() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;

	StepClear();
	
	SetTimer(TMR_LABEL, 100, NULL);
}

void CScreen_Set_Maintenance::OnBtnBarcodeChange() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 11;
	m_nInterfaceStep	= 0;
	m_bstop = false;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL_CHANGE, 100, NULL);
}

void CScreen_Set_Maintenance::OnBtnBarcodeData() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	StepClear();

	SetTimer(TMR_BCR, 100, NULL);
}

void CScreen_Set_Maintenance::OnBtnDensityposModel() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 12;
	m_nInterfaceStep	= 0;
	m_bstop = false;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL_CHANGE, 100, NULL);
}

void CScreen_Set_Maintenance::OnBtnDensityPosData() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	StepClear();

	SetTimer(TMR_TOP, 100, NULL);

}

void CScreen_Set_Maintenance::OnBtnBarcodeposChange() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	m_nClientPos		= 13;
	m_nInterfaceStep	= 0;
	m_bstop = false;
	
	m_ctrlName.GetWindowText(m_strModel);
	if (m_strModel == "")
	{
		m_strModel = "MSATA";
	}
	SetTimer(TM_MODEL_CHANGE, 100, NULL);	
}

void CScreen_Set_Maintenance::OnBtnBarcodePosData() 
{
	if (st_handler.mn_menu_lock == TRUE) return;
	
	st_handler.mn_menu_lock = TRUE;
	
	StepClear();

	SetTimer(TMR_BTM, 100, NULL);

}

void CScreen_Set_Maintenance::OnBtnDensity1() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to print label#1 again?");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		g_Run_Density_Feeders.m_nRun_Print[0] = 0;
	
}

void CScreen_Set_Maintenance::OnBtnDensity2() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to print label#2 again?");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		g_Run_Density_Feeders.m_nRun_Print[1] = 0;
	
}

void CScreen_Set_Maintenance::OnBtnDensity3() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to print label#3 again?");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		g_Run_Density_Feeders.m_nRun_Print[2] = 0;
	
}

void CScreen_Set_Maintenance::OnBtnDensity4() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to print label#4 again?");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		g_Run_Density_Feeders.m_nRun_Print[3] = 0;
	
}

void CScreen_Set_Maintenance::OnBtnDensity5() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to print label#5 again?");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		g_Run_Density_Feeders.m_nRun_Print[4] = 0;
	
}

void CScreen_Set_Maintenance::OnBtnDensity6() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to print label#6 again?");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		g_Run_Density_Feeders.m_nRun_Print[5] = 0;
	
}

void CScreen_Set_Maintenance::OnBtnFrontTopReset() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to clear Front top smema? it's very dangerous");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		st_handler.mn_8580_top = CTL_YES;
		Run_Smema_Ctl.mn_top_run_step = 0;
		g_Run_IDBuff_Input_Index.mn_run_step = 0;
		g_ioMgr.set_out_bit(st_io.o_front_top_machine_enter_signal, IO_OFF);
		g_ioMgr.set_out_bit(st_io.o_front_top_machine_transfer_signal, IO_OFF);

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg,"[SMEMA RESET] Top Smema reset");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

	}
	
}

void CScreen_Set_Maintenance::OnBtnFrontBtmReset() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to clear bottom smema? it's very dangerous");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		st_handler.mn_8580_btm = CTL_YES;
		if(g_handler.m_step_btm_rear_smema > 0 || g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, IO_ON) == IO_ON ||
			g_ioMgr.get_out_bit( st_io.o_front_btm_machine_ready_signal, IO_ON ) == IO_ON)
		{
			g_handler.m_step_btm_rear_smema = 0;
			g_ioMgr.set_out_bit(st_io.o_rear_ready_signal, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_front_btm_machine_ready_signal, IO_OFF);
		}

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg,"[SMEMA RESET] Bottom Smema reset");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

	}
	
}


void CScreen_Set_Maintenance::OnBtnDensityManual() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to label printer manul run? ");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
		st_handler.mn_manual_lable = CTL_YES;
	
}

void CScreen_Set_Maintenance::OnBtnDensityRun() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to label printer manul run reset? ");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		st_handler.mn_manual_lable = CTL_NO;

		for ( int i = 0; i < MAX_EDP; i++ )
		{
			g_Run_Density_Feeders.m_bPrint_Pickup[i] = false;
			g_Run_Density_Feeders.m_nRun_Print[i] = 0;
		}
		
		for ( int j = 0; j < 6; j++)
		{
// 			g_Run_Density_Feeders.mn_RunStep[j] = 0;
			g_Run_Density_Feeders.m_nLabelResult[j] = CTL_FREE;//초기 비젼 세팅
		}
		g_Run_Density_Feeders.mn_RunStep = 0;//2014.0105
	}
	
}

void CScreen_Set_Maintenance::OnLeveldisplay() 
{
	if( st_handler.mn_level_teach == FALSE && st_handler.mn_level_admin == FALSE )
	{
		m_group_comm.ShowWindow(FALSE);
		m_btn_opengms.ShowWindow(FALSE);
		m_btn_closegms.ShowWindow(FALSE);
		m_btn_readbcr1.ShowWindow(FALSE);
		m_btn_readbcr2.ShowWindow(FALSE);
		m_btn_closebcr1.ShowWindow(FALSE);
		m_btn_closebcr2.ShowWindow(FALSE);
		m_btn_openbcr1.ShowWindow(FALSE);
		m_btn_openbcr2.ShowWindow(FALSE);
		m_btn_density_reset.ShowWindow(FALSE);

		m_group_density_label.ShowWindow(FALSE);
		m_btn_denity_change.ShowWindow(FALSE);
		m_btn_density_result.ShowWindow(FALSE);

		m_group_barcode.ShowWindow(FALSE);
		m_btn_bcr_change.ShowWindow(FALSE);
		m_btn_bcr_result.ShowWindow(FALSE);

		m_label_model_name.ShowWindow(FALSE);
		m_ctrlName.ShowWindow(FALSE);

		m_group_density_pos.ShowWindow(FALSE);
		m_btn_densitypos_model.ShowWindow(FALSE);
		m_btn_densitypos_result.ShowWindow(FALSE);

		m_group_barcode_pos.ShowWindow(FALSE);
		m_btn_barcodepos_change.ShowWindow(FALSE);
		m_btn_barcodepos_result.ShowWindow(FALSE);

		m_group_smema_result.ShowWindow(FALSE);
		m_btn_front_top_reset.ShowWindow(FALSE);
		m_btn_front_btm_reset.ShowWindow(FALSE);

		m_group_manual_out.ShowWindow(FALSE);
		m_btn_density_manual.ShowWindow(FALSE);
		m_btn_density_run.ShowWindow(FALSE);

		m_group_data_change_8580s.ShowWindow(FALSE);
		m_msg_acce_code.ShowWindow(FALSE);
		m_msg_tbox_code.ShowWindow(FALSE);
		m_btn_idsave.ShowWindow(FALSE);
		m_edit_acce_code.ShowWindow(FALSE);
		m_edit_tbox_code.ShowWindow(FALSE);

		m_group_sendto_8580.ShowWindow(FALSE);
		m_btn_idsave.ShowWindow(FALSE);
		m_edit_send_data.ShowWindow(FALSE);

	}
}

void CScreen_Set_Maintenance::OnBtnDensityReset() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to init density label? ");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
// 		st_var.n_newLabelType_flag = CTL_YES;
		st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
		st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
		st_var.n_use_density_count[st_basic.mn_device_type] = 0;//reset
		st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;//reset
		g_Run_Density_Feeders.Thread_Variable_Initial();
	}	
}

void CScreen_Set_Maintenance::OnBtnIdSave() 
{
	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("Do you want to change acccy code and tbox code? ");
	int mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		CString strCode;
		for ( int i = 0; i < 2; i++)
		{
			m_edit_acce_code.GetWindowText(strCode);
			if(strCode != "")
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_acce_code[i], "%s", strCode);
			m_edit_tbox_code.GetWindowText(strCode);
			if(strCode != "")
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_tbox_code[i], "%s", strCode);
		}
	}
}

void CScreen_Set_Maintenance::OnBtnIdSend() 
{
// 	CDialog_Select select_dlg;
// 	st_msg.mstr_confirm_msg = _T("Do you want to send dtata AMT8580S? ");
// 	int mn_response = select_dlg.DoModal();
// 	
// 	if (mn_response == IDOK)
// 	{
// 		CString strCode;
// 		m_edit_send_data.GetWindowText(strCode);
// 		if(strCode != "")
// 		{
// 			
// 			CString strData;
// 			strData.Format("%016d", strCode.GetLength());
// 			sprintf(st_client[CLS_BOTTOM].ch_send, strData);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + CLS_BOTTOM, CLIENT_SEND, CLS_BOTTOM);
// 			
// 			sprintf(st_client[CLS_BOTTOM].ch_send, strCode);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + CLS_BOTTOM, CLIENT_SEND, CLS_BOTTOM);
// 		}
// 		
// 	}		
}
