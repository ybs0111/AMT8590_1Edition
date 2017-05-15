// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Handler.h"

#include "MainFrm.h"

// ******************************************************************************
// 대화 상자 및 외부 클래스 추가                                                 
// ******************************************************************************
#include "MenuHead.h"		// 화면에 대한 헤더 정의된 클래스 추가 
#include "Dialog_Menu.h"	// 메뉴 선택 대화 상자 클래스 추가 
#include "Dialog_HandBarcode.h"
#include "Dialog_About.h"	// 프로그램 About 대화 상자 클래스 추가 
#include "Dialog_Exit.h"	// 응용 프로그램 종료 화면 클래스 추가 
#include "Dialog_Pass_Check.h"
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_Pass_Check.h"
#include "Public_Function.h"	// 모터 보드 및 I/O 보드 관련 클래스
#include "RUN_IO_ReadWrite.h"

#include "ComizoaPublic.h"
#include "Cmmsdk.h"

#include "AVersion.h"

#include "io.h" 
#include "FAS_HSSI.h"
#include "FastechPublic_IO.h"

#include "SrcPart/APartShortkey.h"
#include "SrcPart/APartDatabase.h"
#include "SrcPart//PartFunction.h"
#include "SrcPart//APartHandler.h"

#include "COMI_Manager.h"

#include "AMTRegistry.h"
#include "Srcbase/ALocalization.h"

#include "MesEvent.h"
#include "CimEvent.h"
#include "BcrEvent.h"

#include "InterfaceBarcode2.h"
#include "InterfaceBarcode.h"
#include "InterfaceGms.h"

#include "ZebraPrint.h"
#include "VisionClient.h"
#include "VisionServer.h"
#include "BarcodeFormat.h"
#include "FrontServer.h"
#include "BtmClient.h"
#include "8570Client.h"
#include "Screen_Server_Network.h"
#include "AEC_Client.h"
#include "Screen_Set_Network.h"//2015,0109
#include "NextClient.h"
#include "AMTLotManager.h"
#include "XGemClient.h"
#include "SrcBase\ACriticalSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

st_handler_param			st_handler;

st_setting_param			st_set;
st_message_param			st_msg;
st_time_param				st_time;
st_lamp_param				st_lamp;
st_basic_param				st_basic;
st_alarm_param				alarm;
st_dsp_alarm_param			st_alarm;
st_work_param				st_work;
st_modulemap_size			st_modulemap;
st_motor_param				st_motor[MOTOR_COUNT];
st_path_param				st_path;
st_nw_param					st_NW;
st_sync_param				st_sync;
st_ctlbd_param				st_ctlbd;
st_ctlbd_param				st_reco_ctlbd;  
st_serial_param				st_serial;
struct st_serial_info		rs_232;//20131115
st_linearmotion_param		st_linearmot[8];
st_coordmotion_param		st_coordmot[16];
st_barcode_param 			st_barcode;
st_ani_param				st_ani;//2014.0821
_st_Color				    g_Color;

CAnimate					m_Animate;

st_time_database			st_timedb;
st_alarm_database			st_alarmdb;
st_ground_database			st_grounddb;

st_buffer_database			st_bufferdb;

st_part_motor_info			st_motor_info;
st_part_info				st_part;
st_buffer_tray_param		st_buffer_tray[MAX_SITE_INFO];

st_boat_database			st_boatdb;
st_xgemdata					st_xgem;
st_vision					st_vis;
st_variable_param			st_var;
// 규이리 추가 [2013.11.12]
// 소셋 통신 관련 구조체 추가
st_client_param	st_client[15];
st_server_param	st_server[15];

st_testreference_param      st_testreference;	// TEST SITE화면 환경 설정 정보 저장 구조체 선언 
st_test_param			st_test;			// 작업 결과 정보 저장 구조체 선언
tagBCR_PARAM	stBCR[5];
tagLOT_PARAM	stLOT[MAX_LOT_];
st_step_param				st_step;
st_other_param				st_other;
st_bcrinfo_param st_bcrinfo[4];
st_density_info_param st_density_info[6][4];
st_idbuffer_info_param	st_idbuff_info[MAX_SHIFT_DATA_NUM];

st_density_feeder st_den_feeder[6];
st_density_rbt st_den_rbt[2];
st_barcode_rbt st_bcr_rbt[2];
st_barcode_feeder st_bcr_feeder[4];
st_top_rbt_param st_top_rbt[2];
st_unload_rbt st_uld_rbt;
st_reject_rbt st_rej_rbt;
st_mes_data_param	st_mes_data;
tagPRODUCT_PARAM	stMapPk[13];
st_copy_data			*st_copydata;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDBLCLK()
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_MAIN, OnMain)
	ON_COMMAND(ID_BASIC, OnBasic)
	ON_COMMAND(ID_SETTING, OnSetting)
	ON_COMMAND(ID_WAIT, OnWait)
	ON_COMMAND(ID_MOTOR, OnMotor)
	ON_COMMAND(ID_IO, OnIo)
	ON_COMMAND(ID_LIST, OnList)
	ON_COMMAND(ID_LEVEL, OnLevel)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_ALARM, OnAlarm)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_LOCK, OnLock)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_MOT_TEACH, OnMotTeach)
	ON_WM_TIMER()
	ON_COMMAND(ID_MOT_SPEED, OnMotSpeed)
	ON_COMMAND(ID_SET_INTERFACE, OnSetInterface)
	ON_COMMAND(ID_SET_MAINTENANCE, OnSetMaintenance)
	ON_COMMAND(ID_ADMIN, OnAdmin)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_LIST_OPER, OnListOper)
	ON_COMMAND(ID_LIST_DATAVIEW, OnListDataview)
	ON_COMMAND(ID_FILE_EDITOR, OnFileEditor)
	ON_WM_NCMBUTTONDBLCLK()
	ON_COMMAND(ID_SET_BARCODE, OnSetBarcode)
	ON_COMMAND(ID_ECSERVER, OnEcserver)
	ON_COMMAND(ID_SET_NETWORK, OnSetNetwork)//2015,0109
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FORM_CHANGE, OnViewChangeMode)					// Post Message에 대한 화면 전환 사용자 사용자 정의 메시지 선언
	ON_MESSAGE(WM_MAINFRAME_WORK, OnMainframe_Work)
	ON_MESSAGE(WM_MAIN_EVENT, OnMainMessageEvent)					// 이벤트 제어 메시지
	ON_MESSAGE(WM_MAIN_XEVENT, OnMainMessageXCloseEvent)					// 이벤트 제어 메시지
//	ON_MESSAGE(WM_COMM_DATA, OnCommunication)						// RS-232C 시리얼 포트 제어 메시지
	ON_MESSAGE(WM_COMM_EVENT, OnCommunicationEvent)					// RS-232C 시리얼 포트 이벤트 설정 메시지
	ON_MESSAGE(WM_DATA_SEND, OnDataSend)							// RS-232C 시리얼 포트 통한 데이터 송신 메시지
	ON_MESSAGE(WM_SERIAL_PORT, OnSerialPort)	//20131210
	ON_MESSAGE(WM_CLIENT_MSG_1, OnCommand_Client_1)					// server
	ON_MESSAGE(WM_CLIENT_MSG_2, OnCommand_Client_2)					// Rear Client
	ON_MESSAGE(WM_CLIENT_MSG_3, OnCommand_Client_3)					// g_8570_client
	ON_MESSAGE(WM_CLIENT_MSG_4, OnCommand_Client_4)					// g_next_client
	ON_MESSAGE(WM_CLIENT_MSG_5, OnCommand_Client_5)					// clsEC_Client
	ON_MESSAGE(WM_CLIENT_MSG_6, OnCommand_Client_6)					// 여분 관련된 작업(Reserved)
	ON_MESSAGE(WM_COMM_WORK, OnBcrCommSerial)					// Network관련된 작업을 담당한다.
	ON_MESSAGE(WM_CLIENT_MSG_7, OnZebraPrint_Client_1)				// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_8, OnZebraPrint_Client_2)				// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_9, OnZebraPrint_Client_3)				// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_10, OnZebraPrint_Client_4)				// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_11, OnLabel_Client)						// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_12, OnBarcode_Client)					// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_13, OnTop_Client)						// jtkim barcode print 
	ON_MESSAGE(WM_CLIENT_MSG_14, OnBottom_Client)					// jtkim barcode print 
// 	ON_MESSAGE(WM_CLIENT_MSG_11, OnBcrFormat1_Client)				// jtkim barcode print 
// 	ON_MESSAGE(WM_CLIENT_MSG_12, OnBcrFormat2_Client)				// jtkim barcode print 
// 	ON_MESSAGE(WM_CLIENT_MSG_13, OnBcrFormat3_Client)				// jtkim barcode print 
// 	ON_MESSAGE(WM_CLIENT_MSG_14, OnBcrFormat4_Client)				// jtkim barcode print 
	ON_MESSAGE(WM_SERVER_MSG_1, OnCommand_Server_1)					// Front Server
	ON_MESSAGE(WM_SERVER_MSG_3, OnZebraPrint_Server_1)				// jtkim barcode print //2014,1104 ysj WM_SERVER_MSG_2 -> 3
	ON_MESSAGE(WM_SERVER_MSG_4, OnZebraPrint_Server_2)				// jtkim barcode print //2014,1104 ysj WM_SERVER_MSG_3 -> 4
	ON_MESSAGE(WM_SERVER_MSG_5, OnZebraPrint_Server_3)				// jtkim barcode print //2014,1104 ysj WM_SERVER_MSG_4 -> 5
	ON_MESSAGE(WM_SERVER_MSG_6, OnZebraPrint_Server_4)				// jtkim barcode print //2014,1104 ysj WM_SERVER_MSG_5 -> 6
	ON_MESSAGE(WM_SERVER_MSG_7, OnLabel_Server)						// jtkim barcode print 
	ON_MESSAGE(WM_SERVER_MSG_8, OnBarcode_Server)					// jtkim barcode print 
	ON_MESSAGE(WM_SERVER_MSG_9, OnTop_Server)						// jtkim barcode print 
	ON_MESSAGE(WM_SERVER_MSG_10, OnBottom_Server)					// jtkim barcode print 


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	g_db.ChangeSection( ST_STOP );
	st_timedb.mnSectionBuffer = dSTOP;

//	FAS_IO.mn_simulation_mode = 0;
//	COMI.mn_simulation_mode = 0;

	OnMain_Var_Default_Set();

	MyJamData.On_Alarm_Info_Load();

	//==============================================================//
	// [초기화] 소켓 관련 변수
	// 규이리 추가 [2014.08.26]
	//==============================================================//
	int i=0;
	for(i=0; i<15; i++)
	{
		m_pClient[i] = NULL;
		m_pServer[i] = NULL;
		
		st_client[i].n_connect = CTL_NO;
		st_server[i].n_connect = CTL_NO;
	}
	//--------------------------------------------------------------//


	nSmaCount1 = nSmaCount2 = 0;

	for ( int j = 0; j < 10; j++)
	{
		nSmaCount3[j] = nSmaServer[j] = 0;
	}
	
	m_timeLastDay = AMTRegistry::RegReadTime();

	cLOG.OnSetPathLogEvent();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString str_temp;
	int i = 0, j = 0;

	st_handler.hWnd = GetSafeHwnd() ;  // 메인 프레임에 대한 핸들 정보 설정

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndToolBar.SetBorders(1, 1, 1, 1);

	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_wndToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
								   IDB_TOOLBAR_DRAW,
		                           IDB_TOOLBAR_DRAW_HOT,
								   IDB_TOOLBAR_DRAW_DISABLED);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
//	DockControlBar(&m_wndToolBar);					// Docking 방식이 아니다.

	// **************************************************************************
    // Text 툴바 Icon 사이즈 설정
    // **************************************************************************
	SIZE size1={32, 32}, size2={62, 49};
	m_wndToolBar.SetSizes(size2, size1);
	// **************************************************************************

	// **************************************************************************
    // 타이틀 정보 출력 Initial Dialog bar 생성
    // **************************************************************************
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my init bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_title = &m_wndTitleBar;
	// **************************************************************************
	
	mcls_m_basic.OnAdmin_Data_Load();
	mcls_m_basic.OnMotorSpeed_Set_Data_Load();
	mcls_m_basic.OnBasic_Data_Load();							// 베이직 셋팅 정보 전역 변수에 로딩하는 함수
	mcls_m_basic.OnWaitTime_Data_Load();
	mcls_m_basic.OnMaintenance_Data_Load();
	mcls_m_basic.OnInterface_Data_Load();//20131115
//	mcls_m_basic.OnBarcode_Label_Data_Load();

	//////////////////////////////////////////////////////////////////////////
	st_var.n_using_barcode = -1;
	if( st_basic.nBcrFeederUse[0] == CTL_YES && st_basic.nBcrFeederUse[1] == CTL_YES )
	{
		st_var.n_use_barcode[0] = EMBS_PASS;
		st_var.n_use_barcode[1] = EMBS_PASS;
		st_var.n_using_barcode = 0;
	}
	else if( st_basic.nBcrFeederUse[2] == CTL_YES && st_basic.nBcrFeederUse[3] == CTL_YES )
	{
		st_var.n_use_barcode[2] = EMBS_PASS;
		st_var.n_use_barcode[3] = EMBS_PASS;
		st_var.n_using_barcode = 1;
	}
	
	if( st_var.n_using_barcode == -1 )
	{
		st_basic.nBcrFeederUse[0] = CTL_YES;
		st_basic.nBcrFeederUse[1] = CTL_YES;
		st_var.n_use_barcode[0] = EMBS_PASS;
		st_var.n_use_barcode[1] = EMBS_PASS;
		st_var.n_using_barcode = 0;
	}
	//////////////////////////////////////////////////////////////////////////

	// **************************************************************************
    // 리스트 정보 출력 Initial Dialog bar 생성
	// -> 상태 바 밑에 위치하도록 하려면 이 부분을 상태 바 생성 부분 위로 올린다.
    // **************************************************************************
	if (!m_wndListBar.Create(this, IDD_DIALOG_LIST, CBRS_ALIGN_BOTTOM, IDD_DIALOG_LIST))
	{
		TRACE0("Failed to create my information bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_list = &m_wndListBar;
	// **************************************************************************

	g_ver.SetVersion();
	
	// **************************************************************************
    // 응용 프로그램에 대한 타이틀 정보 출력
    // ************************************************************************** 
	SetWindowText( g_ver.GetStrVersion() );	// 타이틀 정보 출력 
	CenterWindow();					// 현재 응용 프로그램을 정 중앙에 배치
	// **************************************************************************

	// **************************************************************************
    // 모터 보드 및 I/O 보드 초기화 후 출력 포트 클리어 작업 진행한다        
    // **************************************************************************
	g_ioMgr.IO_Board_Initialize();				// IO 보드 라이브러리 초기화 함수
	RUN_IO_ReadWrite.Init_Total_IORead(0);
	g_ioMgr.OnSet_IO_Out_Port_Clear();			// I/O 보드의 출력 포트 초기화 함수
	
	g_comiMgr.SetMotBoardInit( COMI.Initialize_MotBoard(st_path.mstr_motor) );	// MOTOR 보드 초기화 여부 검사한다
	if( st_handler.mn_virtual_mode )
		g_comiMgr.SetMotBoardInit( BD_GOOD );

	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}

	if (st_handler.cwnd_title != NULL)	
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, dSTOP); 
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, ONLINE_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, EMPTYTRAY_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, XGEM_MODE, dSTOP); 

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

	g_comiMgr.OnMain_Motor_Setting();
	
	OnMain_Thread_Creating();
	
	OnMain_Port_Create();					// Serial Port를 생성한다.

//
//
//	st_client[7].str_ip = "127.0.0.1";
//	st_client[7].n_port = 20001;
//
//	st_client[8].str_ip = "127.0.0.1";
//	st_client[8].n_port = 20001;
//
//	st_client[9].str_ip = "127.0.0.1";
//	st_client[9].n_port = 20001;
//
//	st_server[0].n_port = 9900;
//	st_server[1].n_port = 9901;
//	st_server[2].n_port = 9902;
//	st_server[3].n_port = 9903;
//	st_server[4].n_port = 9904;
//	st_server[5].n_port = 9905;
// 	st_server[6].n_port = 9906;
// 	st_server[7].n_port = 9907;
// 	st_server[8].n_port = 9908;
// 	st_server[9].n_port = 9909;

	st_client[CLS_OTHERS].str_ip = "127.0.0.1";
	st_client[CLS_OTHERS].n_port = 10056;


	// **************************************************************************
    // 초기화 진행 화면으로 전환하여 장비 초기화 작업 진행한다                   
    // **************************************************************************
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);  // 초기화 진행 화면 전환 
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, 0);// CLS_ECSERVER비전 클라이언트
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_CONNECT, 1);// CLS_BOTTOM
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_3, CLIENT_CONNECT, 2);// CLS_8570
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CONNECT, 3);// CLS_NEXT
// 	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_5, CLIENT_CONNECT, 4);// CLS_FORMAT
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_6, CLIENT_CONNECT, 5);// CLS_OTHERS
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_7, CLIENT_CONNECT, 6);// CLS_BCR_PRINTER1
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_8, CLIENT_CONNECT, 7);// CLS_BCR_PRINTER2
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_9, CLIENT_CONNECT, 8);// CLS_BCR_PRINTER3
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CONNECT, 9);// CLS_BCR_PRINTER4
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_1, SERVER_CONNECT, 0); // EC SERVER1
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_2, SERVER_CONNECT, 1); // EC SERVER2
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_3, SERVER_CONNECT, 2); // barcode print
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_4, SERVER_CONNECT, 3); // barcode print
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_5, SERVER_CONNECT, 4); // barcode print
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_6, SERVER_CONNECT, 5); // barcode print
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_7, SERVER_CONNECT, 6); // Label Vision
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_8, SERVER_CONNECT, 7); // Barcode
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_9, SERVER_CONNECT, 8); // Top
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_10, SERVER_CONNECT, 9); // Btm

	// **************************************************************************

	SetTimer(TMR_MAIN_REFRESH, 500, NULL);  // 리스트 파일 생성 타이머
	SetTimer(TMR_FILE_CREATE, 1000, NULL);  // 리스트 파일 생성 타이머
// 	SetTimer(TMR_SMEMA_REAR_CHK, 2000, NULL);
	SetTimer(TMR_GMS_CHK, 10000*60, NULL);
	SetTimer(TMR_MAT_CHK, 60000, NULL);
	if (st_handler.cwnd_title != NULL)	
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, SMEMA_ONOFF, 0);
	}

	::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1005, 0);//Xgem
	// 테스트용.
//	st_handler.mn_level_teach =  TRUE;//2014,0606 ybs

	if( st_handler.cwnd_title )
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);

	st_handler.cwnd_data_lot = NULL;
	st_handler.cwnd_data_testsite = NULL;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU | WS_MAXIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;								// 화면 타이틀 정보 제어 가능하도록 스타일 설정 
	cs.lpszClass=((CHandlerApp*)AfxGetApp())->GetClassName();	// 현재 응용 프로그램에 대한 클래스 이름을 얻는다(Need for Single Instance)

	if (cs.hMenu != NULL)
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	//타이틀바를 클릭 했을때 리턴시킨다.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	//타이틀바를 클릭 했을때 리턴시킨다.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDblClk(UINT nHitTest, CPoint point) 
{
	//타이틀바를 클릭 했을때 리턴시킨다.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcRButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	//타이틀바를 클릭 했을때 리턴시킨다.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcRButtonDown(nHitTest, point);
}

void CMainFrame::OnNcMButtonDblClk(UINT nHitTest, CPoint point) 
{
	if(nHitTest == HTCAPTION)		// 타이틀바 더블클릭이면 리턴
		return;
	
	//     if(nHitTest == HTMAXBUTTON)    // MAX버튼 클릭이면 리턴
	//         return;		
	
	CFrameWnd::OnNcMButtonDblClk(nHitTest, point);
}

//SysCommand의 최대화를 막아야 ..
// void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
// {	
//     if(nID == SC_MAXIMIZE)    // 최대화 명령이면 리턴		
//         return;
// 	
//     CMDIFrameWnd::OnSysCommand(nID, lParam);
// 	
// }


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->hwnd == m_wndToolBar.m_hWnd)
	{
		if (m_wndToolBar.IsFloating())  // 툴바가 플로팅 된 경우에 다시 화면 상단에 정렬 
		{
			m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
			EnableDocking(CBRS_ALIGN_TOP);
			DockControlBar(&m_wndToolBar);
		}
	}


	g_ShortKey.CheckKeyboard( pMsg );
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnMain() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	// int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	// if (nmenu_chk != TRUE)  return;
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Main)))   return;
	OnSwitchToForm(IDW_SCREEN_MAIN);
}

void CMainFrame::OnBasic() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Basic)))   return;
	OnSwitchToForm(IDW_SCREEN_BASIC);
}

void CMainFrame::OnSetting() 
{
	CDialog_Menu menu_dlg;
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	 
// 	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))   return;
// 	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);	
//
	st_msg.mstr_parent_menu = "Setting Screen";  // 현재 선택된 주 메뉴 정보 저장
	int nResponse = menu_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

}

void CMainFrame::OnWait() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Wait_Time)))   return;
	OnSwitchToForm(IDW_SCREEN_WAIT_TIME);
}

void CMainFrame::OnMotor() 
{
	int nRet, nRet2;

	CDialog_Menu menu_dlg;
	
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
//		if (st_handler.mn_level_maint !=  TRUE) 
//		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				return;
//				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
//		}
	}
	// **************************************************************************

	nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
	
	if (nRet == IO_ON && nRet2 == IO_ON)
	{			
		return;
	}
	
	st_msg.mstr_parent_menu = "Motor Screen";  // 현재 선택된 주 메뉴 정보 저장
	int nResponse = menu_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnIo() 
{
	int nRet, nRet2;
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
	
	if (nRet == IO_ON && nRet2 == IO_ON)
	{			
		return;
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_IO)))   return;
	OnSwitchToForm(IDW_SCREEN_IO);
}

void CMainFrame::OnList() 
{
	CDialog_Menu menu_dlg;
	
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// 누구나 볼수 있게 해제. -이상명 대리- 2K4/11/05/ViboX
	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	/* ************************************************************************** */
	
	st_msg.mstr_parent_menu = "List Screen";  // 현재 선택된 주 메뉴 정보 저장
	int nResponse = menu_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnLevel() 
{
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수 선언 
	
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.mn_menu_lock != FALSE)  return;
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	
	st_handler.mstr_pass_level = "LevelChange";
	
	CDialog_Pass_Check pass_dlg;
	
	n_response = pass_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CMainFrame::OnSave() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.mn_menu_lock != FALSE)  return;
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Main : All Setting Data Save?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_handler.mn_menu_lock = TRUE;
		
		OnExitData_Saving();					// 화면에 설정된 모든 데이터 저장 함수
		
		st_handler.mn_menu_lock = FALSE;
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CMainFrame::OnAlarm() 
{
	/* ************************************************************************** */
    /* 모달리스 알람 화면 출력한다                                                */
	/* -> 현재 알람 화면 출력된 경우에는 해당 화면에 포커스만 전달                */
    /* ************************************************************************** */
	if (mp_alarm_dlg != NULL)  mp_alarm_dlg->SetFocus();
	else
	{
		mp_alarm_dlg = new CDialog_Alarm;
		mp_alarm_dlg->Create(this);
		mp_alarm_dlg->ShowWindow(SW_SHOW);
	}
	/* ************************************************************************** */
}

void CMainFrame::OnAbout() 
{
	CDialog_About about_dlg;
	//2012,1015 ybs
	st_handler.mn_menu_lock = FALSE;
	st_handler.mn_system_lock = FALSE;
	
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	int nResponse = about_dlg.DoModal();
	
	if (nResponse == IDOK)
	{
//		ShellExecute( NULL, NULL, TEXT("UpdateList.txt"), NULL, TEXT("ReadMe"), SW_SHOW );
	}
}

void CMainFrame::OnReset() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnLock() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Pass_Check  pass_chk_dlg;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	st_handler.mstr_pass_level=_T("SystemLock");  // 암호 레벨 설정
	
	n_response = pass_chk_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // 메인 프레임에 SYSTEM LOCK 화면 출력 요청
		
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_normal_msg = "System Locking.";
			st_msg.mstr_barcode = "System Locking.";
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
		}
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CMainFrame::OnExit() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수 선언 
	
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_menu_lock != FALSE)	return;
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	
	CDialog_Select select_dlg;
	CDialog_Message msg_dlg;

	// **************************************************************************
	// 현재 장비 상태가 STOP 상태인지 검사                                       
	// **************************************************************************
	if (COMI.mn_run_status != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("SSD Auto Sorter is active.");
		mn_response = msg_dlg.DoModal();
		if (mn_response == IDOK)  return ;
	} 
	// **************************************************************************

	st_handler.mn_level_teach =  FALSE;
	st_handler.mn_level_maint =  FALSE;
	st_handler.mn_level_ver = FALSE;//2014,0606 ybs

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
		
	// **************************************************************************
    // 응용 프로그램 종료 화면 출력                                              
    // **************************************************************************
	CDialog_Exit  dlg;
	
	mn_response = dlg.DoModal();
	
	if (mn_response == IDOK)     
	{
		sprintf(st_msg.c_normal_msg, "[Exit OK]");
		//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		Func.On_LogFile_Add(LOG_TOTAL, st_msg.c_normal_msg);

		st_timedb.mnTime_Section	= dSTOP;		
		st_timedb.mole_date			= COleDateTime::GetCurrentTime();
		
		g_db.DB_Write_Time(st_timedb);

		All_Stop = 1 ;				// 쓰레드 상태 정보 저장 변수 초기화
		OnMain_Thread_Destroy();	// 쓰레드 삭제 함수

		OnMain_Motor_Destroy();						//모든 모터 동작 종료.
		mcls_m_basic.OnDaily_Count_Save();			// Daily Data Save 2K4/08/20/ViboX
		
		OnExitData_Saving();						// 화면에 설정된 모든 데이터 저장 함수
		mcls_m_basic.OnDaily_Count_Save();			// Daily Data Save 2K4/08/20/ViboX
		
		// 도어를 잠근다.
		g_ioMgr.set_out_bit(st_io.o_door_lock,					IO_OFF);
		OnMain_Port_Close();			// 생성한 시리얼 포트 해제 함수

		CView* pOldActiveView = GetActiveView();	// 현재 화면에 출력된 뷰 정보 설정 (save old view)
		
		pOldActiveView->ShowWindow(SW_HIDE);		// 기존에 출력된 뷰를 감춘다 (hide the old)
		pOldActiveView->DestroyWindow();			// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
		pOldActiveView = NULL;						// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화 
		
		delete	pOldActiveView;						// 기존에 출력된 뷰 정보를 제거한다 (kill old view)

		CFrameWnd::OnClose();
	}
}

int CMainFrame::OnMenu_Change_Checking()
{
	int nRet;
	
	nRet = TRUE;  // 초기 리턴 값 설정 
	
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_menu_lock != FALSE)	nRet = FALSE;
	if (st_handler.mn_system_lock != FALSE)  nRet = FALSE;  // 현재 시스템 Lock 상태인 경우 리턴 
	
	// **************************************************************************
	// 현재 장비 상태가 STOP 상태인지 검사                                       
	// **************************************************************************
	if (COMI.mn_run_status != dSTOP && COMI.mn_run_status != dLOCK)
	{
//		st_msg.mstr_normal_msg = _T("Handler is active. Stop first.");
		sprintf(st_msg.c_normal_msg, "Handler is active. Stop first.");
// 		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		Func.On_LogFile_Add(LOG_TOTAL, st_msg.c_normal_msg);
		
		nRet = FALSE;
	}
	// **************************************************************************
	
	return nRet;
}

void CMainFrame::OnSwitchToForm(int nForm)
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_menu_lock != FALSE)	return;
	if (st_handler.mn_system_lock != FALSE)  return;		// 현재 시스템 Lock 상태인 경우 리턴 

	CView* pOldActiveView = GetActiveView();			// 현재 화면에 출력된 뷰 정보 설정 (save old view)
	CView* pNewActiveView = (CView*)GetDlgItem(nForm);  // 새로 출력할 뷰 정보 설정 (get new view)

	if (pNewActiveView == NULL)							// 새로 출력할 뷰가 아직 출력되지 않았는지 검사 (if it hasn't been created yet)
	{
		switch (nForm)			// 뷰에 설정된 ID 정보 비교 (these IDs are the dialog IDs of the view but can use anything)
		{
		case IDW_SCREEN_MAIN:				// 메인 화면 
			pNewActiveView = (CView*)new CScreen_Main;
			break;
		case IDW_SCREEN_LOCK:				// System Lock 화면
			pNewActiveView = (CView*)new CScreen_Lock;
			break;
		case IDW_SCREEN_INIT:				// 초기화 화면 
			pNewActiveView = (CView*)new CScreen_Initial;
			break;
		case IDW_SCREEN_BASIC:				// Basic 화면
			pNewActiveView = (CView*)new CScreen_Basic;
			break;
		case IDW_SCREEN_SET_MAINTENANCE:	// 메인트 화면 
			pNewActiveView = (CView*)new CScreen_Set_Maintenance;
			break;
//
		case IDW_SCREEN_WAIT_TIME:			// 대기 시간 화면 
 			pNewActiveView = (CView*)new CScreen_Wait_Time;
 			break;
		case IDW_SCREEN_MOTOR_TEACH:		// Motor Teach 화면
			pNewActiveView = (CView*)new CScreen_Motor;
			break;
		case IDW_SCREEN_MOTOR_SPEED:		// Motor Speed 화면 
			pNewActiveView = (CView*)new CScreen_Motor_Speed;
			break;

		case IDW_SCREEN_LIST_OPERATION:		// 오퍼레이션 정보 출력 화면 
			pNewActiveView = (CView*)new CScreen_List_Operation;
			break;

		case IDW_SCREEN_LIST_DATA_VIEW:			// 쓰레드 스텝 정보 출력 화면 
			pNewActiveView = (CView*)new CScreen_List_Data_View;
			break;

		case IDW_SCREEN_SET_BARCODE:	// Barcode 화면 
			pNewActiveView = (CView*)new CScreen_Set_Barcode;
			break;

		case IDW_SCREEN_IO:					// I/O 출력 화면 
			pNewActiveView = (CView*)new CScreen_IO;
			break;

		case IDW_SCREEN_ADMINISTRATOR:		// ADMINISTRATOR 출력 화면 
			pNewActiveView = (CView*)new CScreen_Admin;
			break;
		case IDW_SCREEN_FILE_EDITOR:		// File Editor 출력 화면 
			pNewActiveView = (CView*)new CScreen_File_Editor;
			break;

		case IDW_SCREEN_ECSERVER:
			pNewActiveView = (CView*)new CScreen_Server_Network;
			break;

		////2015,0109
		case IDW_SCREEN_SET_NETWORK:	// Network 화면 
			pNewActiveView = (CView*)new CScreen_Set_Network;
			break;

		////
		}

		CCreateContext context;  // 새로운 뷰에 도큐먼트 Attach ( attach the document to the new view)
		context.m_pCurrentDoc = pOldActiveView->GetDocument();

		pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nForm, &context);
		pNewActiveView->OnInitialUpdate();       // 새로 수정한 방식을 쓸때는 이 부분을 없앤다.
	}

//  새로 수정한 방식.
//	pOldActiveView->ShowWindow(SW_HIDE);	// 기존에 출력된 뷰를 감춘다 (hide the old)
//	pOldActiveView->DestroyWindow();		// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
//	pOldActiveView = NULL;					// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화 
//	
//	pNewActiveView->OnInitialUpdate();      // 파괴되는 폼에서 바뀐 정보를 새로 출력되는 뷰에서 받질 못해서 이리로 빼 놓음.
//	SetActiveView(pNewActiveView);			// 새로 출력된 뷰를 액티브 뷰로 설정 (change the active view)
//	pNewActiveView->ShowWindow(SW_SHOW);	// 새로운 뷰를 출력 (show the new window)
//
//	::SetWindowLong(pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
//	RecalcLayout();							// 프레임을 조율한다 (adjust frame)
//	delete	pOldActiveView;					// 기존에 출력된 뷰 정보를 제거한다 (kill old view)
//
//  기존 방식.
	SetActiveView(pNewActiveView);			// 새로 출력된 뷰를 액티브 뷰로 설정 (change the active view)
	pNewActiveView->ShowWindow(SW_SHOW);	// 새로운 뷰를 출력 (show the new window)

	pOldActiveView->ShowWindow(SW_HIDE);	// 기존에 출력된 뷰를 감춘다 (hide the old)
	pOldActiveView->DestroyWindow();		// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
	pOldActiveView = NULL;					// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화 

	::SetWindowLong(pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
	RecalcLayout();							// 프레임을 조율한다 (adjust frame)
	delete	pOldActiveView;					// 기존에 출력된 뷰 정보를 제거한다 (kill old view)
	
	st_handler.mn_menu_num = nForm; 

	// **************************************************************************
	// 현재 화면 정보 타이틀바에 출력한다.                                       
	// **************************************************************************
	if (st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nForm);
	// **************************************************************************
}

// ******************************************************************************
// 사용자 정의 메시지에 따른 뷰 화면 전환 함수                                   
// ******************************************************************************
LRESULT CMainFrame::OnViewChangeMode(WPARAM wParam,LPARAM lParam)
{

	if( GetActiveView() == NULL )
	{
		int a = 1;
		return 0;
	}
	int mn_chg_num = wParam;  // 뷰 화면에 대한 ID 값 설정 
	
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
	// -> 알람 화면을 제외한 화면에 대해서는 메뉴 사용 가능 여부 검사            
    // **************************************************************************
	if (mn_chg_num != 9)  
	{
		if (st_handler.mn_menu_lock != FALSE)  return 0;
		if (st_handler.mn_system_lock != FALSE)  return 0;  // 현재 시스템 Lock 상태인 경우 리턴 
	}
	// **************************************************************************
	
	switch(mn_chg_num)
	{
	case 1: 
		if (lParam==1)  OnMain();					// 메인 화면 전환 
		else if (lParam==2)							// System Locking 화면 전환 
		{
			if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Lock)))   return 0;
			OnSwitchToForm(IDW_SCREEN_LOCK) ;
		}
		else if (lParam==3)							// 초기화 화면 전환 
		{
			if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Initial)))   return 0;
			OnSwitchToForm(IDW_SCREEN_INIT) ;
		}
		break;
	case 2: 
		OnBasic();									// 베이직 화면 전환 
		break;
 	case 3:
 		if (lParam==1)       OnSetMaintenance();	// 메인트 화면 전환 
// 		else if (lParam==2)  OnSetInterface();		// Interface 관련 설정 화면 전환
		else if (lParam==3)  OnSetBarcode();
 		else if (lParam==4)  OnSetNetwork();		//2015,0109 Network 화면 전환.
		break;
 	case 4 :
 		OnWait();
 		break;
 	case 5 : 
 		if (lParam==1)       OnMotTeach();			// 모터 위치 화면 전환 
 		else if (lParam==2)	 OnMotSpeed();			// 모터 속도 화면 전환 
 		break;
	case 6 :
		if (lParam==1)       OnListOper();			// Operation 결과 리스트 화면 전환 
// 		else if (lParam == 2)OnListDataview();

		break;
	case 7 : 
		if (lParam==1)       OnAlarm();				// 알람 화면 전환 
		else if (lParam==2)  OnAlarm_Destroy();		// 알람 화면 종료 함수
		break;
	case 8 : 
		if (lParam==1)       OnAdmin();				// Administrator 화면 전환 
		else if (lParam==2)  OnFileEditor();		// File Editor 화면 전환.
		else if (lParam==3)  OnEcserver();		// Server Network 화면 전환.
		break;
	default:
		break;
	}
	
	return 0 ;
}
// ******************************************************************************

void CMainFrame::OnMain_Path_Set()
{
	CString strTempPath;

// 	CString mstr_basic;			// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
// 	
// 	CString mstr_path_dvc;		// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정 변수
// 	CString mstr_socket_lot;	// 소켓별...
// 	CString mstr_socket_day;	// 소켓별...
// 	CString mstr_socket_month;	// 소켓별...
// 	CString mstr_socket_shift;	// 소켓별...
// 	CString mstr_path_alarm;	// 출력 알람 정보 존재 폴더 설정 변수
// 	CString mstr_path_op;		// 오퍼레이터 정보 저장 폴더 설정 변수
// 	CString mstr_path_tray;		// Tray별 정보 저장 파일 생성 폴더 설정 변수
// 	CString mstr_path_serial;	// Socket 정보 저장 파일 생성 폴더 설정 변수
// 	
// 	CString mstr_path_file;		// 파일 생성 폴더 설정 변수
// 	CString mstr_path_back;		// BACKUP 파일 생성 폴더 설정 변수
// 	
// 	CString mstr_file_basic;	// 기본 셋팅 정보 저장 파일명 설정 변수
// 	CString mstr_file_wait;		// 대기 시간 정보 저장 파일명 설정 변수
// 	
// 	CString mstr_operation;		// Operation Log 파일 폴더 설정.
// 	CString mstr_interface;		// Interface Log 파일 폴더 설정.
// 	CString mstr_machine;		// Machine Log 파일 폴더 설정.
// 	CString mstr_barcode;		// Barcode Log 파일 폴더 설정.
// 	CString mstr_gpib;			// Total Log 파일 폴더 설정.
// 	CString mstr_total;			// Total Log 파일 폴더 설정.
// 	
// 	CString mstr_io_map;		// IO MAP 파일 위치 폴더.
// 	
// 	CString mstr_handler;

// [Folder_Data]
// Alarm=C:\\AMT8500\\Alarm\\
// Bmp=C:\\AMT8500\\Bmp\\
// Data=C:\\AMT8500\\Data\\
// Device=C:\\AMT8500\\Device\\
// Log=C:\\AMT8500\\Log\\
// Setting=C:\\AMT8500\\Setting\\;

	CString strAlarmPath;
	CString strBmpPath;
	CString strDataPath;
	CString strDevicePath;
	CString strLogPath;
	CString strSettingPath;
	CString strModelPath;
	char chr_data[100];

	CString str_LoadFile;       // 임시 저장 변수
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = "C:\\AMT8590\\Setting\\PathInfo.TXT";
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[%s] file open error.", str_LoadFile);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.c_normal_msg);
			}
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[%s] file is not exist.", str_LoadFile);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			Func.On_LogFile_Add(LOG_TOTAL, st_msg.c_normal_msg);
		}
		return;
	}

	:: GetPrivateProfileString("Folder_Data", "Alarm",		"D:\\AMTLOG\\Alarm\\", chr_data, 100, ".\\PathInfo.TXT");
	strAlarmPath = chr_data;
	strAlarmPath.TrimLeft(' ');               
	strAlarmPath.TrimRight(' ');

	:: GetPrivateProfileString("Folder_Data", "Bmp",		"C:\\AMT8590\\Bmp\\", chr_data, 100, ".\\PathInfo.TXT");
	strBmpPath = chr_data;
	strBmpPath.TrimLeft(' ');               
	strBmpPath.TrimRight(' ');

	:: GetPrivateProfileString("Folder_Data", "Data",		"C:\\AMT8590\\Data\\", chr_data, 100, ".\\PathInfo.TXT");
	strDataPath = chr_data;
	strDataPath.TrimLeft(' ');               
	strDataPath.TrimRight(' ');

	:: GetPrivateProfileString("Folder_Data", "Device",		"C:\\AMT8590\\Device\\", chr_data, 100, ".\\PathInfo.TXT");
	strDevicePath = chr_data;
	strDevicePath.TrimLeft(' ');               
	strDevicePath.TrimRight(' ');

	:: GetPrivateProfileString("Folder_Data", "Log",		"D:\\AMTLOG\\Log\\", chr_data, 100, ".\\PathInfo.TXT");
	strLogPath = chr_data;
	strLogPath.TrimLeft(' ');               
	strLogPath.TrimRight(' ');

	:: GetPrivateProfileString("Folder_Data", "Setting",	"C:\\AMT8590\\Setting\\", chr_data, 100, ".\\PathInfo.TXT");
	strSettingPath = chr_data;
	strSettingPath.TrimLeft(' ');               
	strSettingPath.TrimRight(' ');

	:: GetPrivateProfileString("Folder_Data", "Model",		"C:\\AMT8590\\Setting\\ModelName\\", chr_data, 100, ".\\PathInfo.TXT");
	strModelPath = chr_data;
	strModelPath.TrimLeft(' ');               
	strModelPath.TrimRight(' ');

// 	st_path.mstr_reg_model = _T(strSettingPath + "RegModel.TXT");
// 	Func.CreateFolder(strSettingPath);

	st_path.mstr_basic =			_T(strSettingPath + "BasicTerm.TXT");		// 환경 설정 정보 저장 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_basic);

	st_path.mstr_basic_folder =		_T(strSettingPath);					// 환경 설정 정보 저장 파일에 대한 폴더 설정 

	st_path.mstr_path_dvc =		_T(strDevicePath);						// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정
	Func.CreateFolder(st_path.mstr_path_dvc);

	st_path.mstr_motor =			_T(strSettingPath + "AMT8590.cme2");

	st_path.mstr_file_basic =		_T("BasicTerm");								// 기본 셋팅 정보 저장 파일명 설정
	st_path.mstr_file_wait =		_T("WaitTime");									// 대기 시간 정보 저장 파일명 설정

	CString FolderPath;
	
	st_path.mstr_path_alarm =		_T(strBmpPath);									// 출력 알람 정보 존재 폴더 설정
	Func.CreateFolder(st_path.mstr_path_alarm);
	
	st_path.mstr_path_file =		_T("C:\\AMT8590\\");							// 파일 생성 폴더 설정
	st_path.mstr_path_back =		_T("c:\\BackUp\\");									// BACKUP 파일 생성 폴더 설정
	Func.CreateFolder(st_path.mstr_path_back);
	
	st_path.mstr_operation =		_T(strLogPath + "Operation");			// Operation Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_operation);
	
	st_path.mstr_interface =		_T(strLogPath + "Interface");			// Interface Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_interface);

	st_path.mstr_tcpip =		_T(strLogPath + "Tcpip");			// Interface Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_tcpip);	
	
	st_path.mstr_machine   =		_T(strLogPath + "Machine");				// Machine Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_machine);
	
	st_path.mstr_barcode   =		_T(strLogPath + "Barcode\\");				// Machine Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_barcode);
	
	st_path.mstr_total     =		_T(strLogPath + "Total");				// Total Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_total);

	st_path.mstr_uph     =		_T(strLogPath + "UPH\\UPH\\");				// Total Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_uph);

	st_path.mstr_uph_total     =		_T(strLogPath + "UPH\\Total\\");				// Total Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_uph_total);
	
	st_path.mstr_reject     =		_T(strLogPath + "UPH\\Reject\\");				// Total Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_reject);

	st_path.str_log_path	=		_T(strLogPath + "EqLog\\");	
	Func.CreateFolder(st_path.str_log_path);

	st_path.str_lot_data_path	=	_T(strLogPath + "Time\\");	
	Func.CreateFolder(st_path.str_lot_data_path);

	Func.CreateFolder(strLogPath + "AlarmCodeDebug\\");
	Func.CreateFolder(strAlarmPath + "Daily\\");
	//2013,0306
	Func.CreateFolder(strAlarmPath + "UnderDaily\\");

	///////20131115
	st_path.str_interface_folder = _T("c:\\AMT8590\\File\\File.TXT"); //20120516
	Func.CreateFolder(st_path.str_interface_folder);
	///////

	////2014,1217 ysj
	st_path.str_eqp_log_path = _T("c:\\EQP_LOG\\");
	Func.CreateFolder(st_path.str_eqp_log_path);
	////

	st_path.mstr_io_map				= _T(strSettingPath + "AMT8590_IO_MAP.xls");
	st_path.mstr_motor_axis_map		= _T(strSettingPath + "AMT8590_MOTOR_AXIS_MAP.xls");
	if ( g_local.GetLocalType() == LOCAL_ENG ) st_path.mstr_motor_axis_map		= _T(strSettingPath + "AMT8590_MOTOR_AXIS_MAP_ENG.xls");
	st_path.mstr_motor_part_map		= _T(strSettingPath + "AMT8590_MOTOR_PART_MAP.xls");
	if ( g_local.GetLocalType() == LOCAL_ENG ) st_path.mstr_motor_part_map		= _T(strSettingPath + "AMT8590_MOTOR_PART_MAP_ENG.xls");
	st_path.mstr_io_part_map		= _T(strSettingPath + "AMT8590_IO_PART_MAP.xls");
	st_path.mstr_wait_time_map		= _T(strSettingPath + "AMT8590_WAITTIME_MAP.xls");
	if ( g_local.GetLocalType() == LOCAL_ENG ) st_path.mstr_wait_time_map		= _T(strSettingPath + "AMT8590_WAITTIME_MAP_ENG.xls");

	////2015.0221
	st_path.mstr_recipe_path = _T("C:\\XGEM\\AMT8590\\AMT8590\\XWork\\Recipe\\");
	Func.CreateFolder(st_path.mstr_recipe_path);
	////

	//2015.0513
	st_path.mstr_eqp_file =		_T("C:\\EQP_LOG\\");							// 파일 생성 폴더 설정
	Func.CreateFolder(st_path.mstr_eqp_file);
	
	st_path.mstr_xgem_path =	_T(st_path.mstr_eqp_file + "Comm\\");
	Func.CreateFolder(st_path.mstr_xgem_path);
	st_path.mstr_error_path =	_T(st_path.mstr_eqp_file + "Error\\");
	Func.CreateFolder(st_path.mstr_xgem_path);
	st_path.mstr_event_path =	_T(st_path.mstr_eqp_file + "Event\\");
	Func.CreateFolder(st_path.mstr_event_path);
	st_path.mstr_product_path =	_T(st_path.mstr_eqp_file + "Product\\");
	Func.CreateFolder(st_path.mstr_product_path);
	st_path.mstr_mat_path =	_T(st_path.mstr_eqp_file + "Mat\\");
	Func.CreateFolder(st_path.mstr_mat_path);

	fclose(fp);

}

void CMainFrame::OnMain_Var_Default_Set()
{
	int i = 0, j = 0, mn_chk = 0;
	CString mstr_temp;
	OnMain_Path_Set();

	nElectrostaticCheckStep = 0;
	nElectChannel = 1;

	mn_BarcodeSetValueChangeStep = 0;//20131115

	for (i = 0; i < MAXSITE; i++)
	{
		st_handler.mn_init_state[i] = CTL_NO;
	}

	mp_alarm_dlg = NULL;
	mp_msg_dlg = NULL;
	mp_xmsg_dlg = NULL;

	mn_new_lot_in_step  = 0;
	mn_lot_name_step = 0;

	for (i = 0; i < 3; i++)
	{
		st_handler.mstr_lot_name[i] = "";
	}

	nElectrostaticCheckStep = 0;					// 정전기 확인 스텝 클리어.
	nElectChannel = 1;						// 정전기 확인 채널 1~5

	st_ctlbd.n_motorbd_init_end = CTLBD_NO; 
	g_comiMgr.SetMotBoardInit( false );

	CString str_load_device, str_chk_ext;
	int mn_pos;
	char chr_data[50];
	char chr_buf[50];
	
	:: GetPrivateProfileString("BasicData", "Device_Type", "", chr_data, 50, st_path.mstr_basic);
	str_load_device = chr_data;
	str_load_device.TrimLeft(' ');               
	str_load_device.TrimRight(' ');
	
	mn_pos = str_load_device.Find(".");								// 확장자 위치 검사
	if (mn_pos == -1)
		st_basic.mstr_device_name = str_load_device + _T(".TXT");	// 확장자 추가
	else 
	{
		str_chk_ext = str_load_device.Mid(mn_pos);					// 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
			st_basic.mstr_device_name = _T("DEFAULT.TXT");			// 초기 디바이스 파일명 설정
		else 
			st_basic.mstr_device_name = str_load_device;				// 디바이스 파일명 설정
	}

	:: GetPrivateProfileString("BasicData", "Virtual_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_virtual_mode = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_virtual_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Virtual_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_virtual_mode = mn_chk;

	//2013,1028
//	st_handler.mn_virtual_mode = 0;

	:: GetPrivateProfileString("BasicData", "Virtual_Supply", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_virtual_supply = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_virtual_supply, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Virtual_Supply", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_virtual_supply = mn_chk;

	//2013,1028
	st_handler.mn_virtual_supply = 0;
	
	:: GetPrivateProfileString("BasicData", "Buffer_In", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_buffer_in = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_buffer_in, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Buffer_In", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_buffer_in = mn_chk;

	//2013,1028
	st_handler.mn_buffer_in = 0;

	//2013,0715
	:: GetPrivateProfileString("BasicData", "FullInline_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_fullinline_mode = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_fullinline_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "FullInline_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_fullinline_mode = mn_chk;

	//2013,1028
	st_handler.mn_fullinline_mode = 1;
	
// 	:: GetPrivateProfileString("BasicData", "PICKER_POSITION_0", "0", chr_data, 10, st_path.mstr_basic);
// 	st_handler.md_picker_position[0] = atof(chr_data);
// 
// 	:: GetPrivateProfileString("BasicData", "PICKER_POSITION_1", "0", chr_data, 10, st_path.mstr_basic);
// 	st_handler.md_picker_position[1] = atof(chr_data);
// 
// 	:: GetPrivateProfileString("BasicData", "PICKER_POSITION_2", "0", chr_data, 10, st_path.mstr_basic);
// 	st_handler.md_picker_position[2] = atof(chr_data);
// 
// 	:: GetPrivateProfileString("BasicData", "PICKER_POSITION_3", "0", chr_data, 10, st_path.mstr_basic);
// 	st_handler.md_picker_position[3] = atof(chr_data);
// 
// 	:: GetPrivateProfileString("BasicData", "TRAY_PLACE_OFFSET_0", "0", chr_data, 10, st_path.mstr_basic);
// 	st_handler.md_tray_place_offset[0] = atof(chr_data);
// 
// 	:: GetPrivateProfileString("BasicData", "TRAY_PLACE_OFFSET_1", "0", chr_data, 10, st_path.mstr_basic);
// 	st_handler.md_tray_place_offset[1] = atof(chr_data);
// 
// 	ReadBasicData_Int( st_handler.mn_retest_cnt, "RETEST_CNT", 1, 0, 3, st_path.mstr_basic );
// 	ReadBasicData_Int( st_handler.mn_beat_cnt, "BEAT_CNT", 0, 0, 3, st_path.mstr_basic );
// 	ReadBasicData_Int( st_handler.mn_AutoSocketOff_Cnt, "AUTO_SOCKET_OFF_CNT", 5, 0, 10, st_path.mstr_basic );
// 
// 	ReadBasicData_Int( st_handler.mn_cokIndex[COKINDEX_GS1], "COKINDEX_GS1", -1, -1, 5, st_path.mstr_basic );
// 	ReadBasicData_Int( st_handler.mn_cokIndex[COKINDEX_GS2], "COKINDEX_GS2", -1, -1, 5, st_path.mstr_basic );
// 	ReadBasicData_Int( st_handler.mn_cokIndex[COKINDEX_MSATA], "COKINDEX_MSATA", -1, -1, 5, st_path.mstr_basic );
// 	ReadBasicData_Int( st_handler.mn_cokIndex[COKINDEX_25_REVERSE], "COKINDEX_25_REVERSE", -1, -1, 5, st_path.mstr_basic );
// 	ReadBasicData_Int( st_handler.mn_cokIndex[COKINDEX_25_FRONT], "COKINDEX_25_FRONT", -1, -1, 5, st_path.mstr_basic );

// 	for( i=0; i<COMMON_TEACHING_MAX; i++ )
// 	{
// 		CString strKeyName;
// 		strKeyName.Format( "COMMON_TEACHING_%02d", i );
// 		ReadBasicData_Double( st_handler.md_common_teaching[i], strKeyName, 0.0f, -4000.0f, 4000.0f, st_path.mstr_basic );
// 	}

	mcls_m_basic.On_Teach_Data_Load();
	
	//2015.0313
	CString strVal;
	for ( j = 0; j < 6; j++)
	{
		strVal.Format( "CAPADENSITY_%02d", j );
		st_handler.m_strDensityCapa[j] = AMTRegistry::RegReadString( REG_VAL_CAPADENSITY, strVal, "" ).c_str();
	}
	for ( j = 0; j < 6; j++)
	{
		strVal.Format( "DENSITY_%02d", j );
		st_handler.m_strDensityLabel[j] = AMTRegistry::RegReadString( REG_VAL_DENSITY, strVal, "" ).c_str();
	}
	for ( j = 0; j < 4; j++)
	{
		strVal.Format( "BCR_LABEL_%02d", j );
		st_handler.m_strDistributionLabel[j] = AMTRegistry::RegReadString( REG_VAL_BCR, strVal, "" ).c_str();
		strVal.Format( "BCR_RIBBON_%02d", j );
		st_handler.m_strDistributionRibbon[j] = AMTRegistry::RegReadString( REG_VAL_BCR, strVal, "" ).c_str();

// 		strVal.Format( "BCR_SD_%02d", j );
// 		st_barcode.mn_labelset_sd[j] = AMTRegistry::RegReadInt( REG_VAL_LABEL_BCR_SD, strVal);
// 		strVal.Format( "BCR_LT_%02d", j );
// 		st_barcode.mn_labelset_lt[j] = AMTRegistry::RegReadInt( REG_VAL_LABEL_BCR_LT, strVal);
// 		strVal.Format( "BCR_LHX_%02d", j );
// 		st_barcode.mn_labelset_lhx[j] = AMTRegistry::RegReadInt( REG_VAL_LABEL_BCR_LHX, strVal);
// 		strVal.Format( "BCR_LHY_%02d", j );
// 		st_barcode.mn_labelset_lhy[j] = AMTRegistry::RegReadInt( REG_VAL_LABEL_BCR_LHY, strVal);
// 		strVal.Format( "BCR_MODE_%02d", j );
// 		st_barcode.mn_Prt_Mode[j] = AMTRegistry::RegReadInt( REG_VAL_LABEL_BCR_MODE, strVal);
// 		strVal.Format( "BCR_COMPLETE_%02d", j );
// 		st_barcode.mn_Prt_complete_use[j] = AMTRegistry::RegReadInt( REG_VAL_LABEL_BCR_COMPLETE, strVal);
	}

	for (i = 0; i < 3; i++)
	{
		st_handler.mstr_lot_name[i] = "";
		st_var.m_var_reject[i] = CTL_NO;//reject reset
	}

	st_NW.mb_buffer_in_received = true;

	st_basic.n_retry_cnt = 3;

	st_handler.mn_emptyout_cnt = 0; //ybs
	st_handler.mn_emptynext_cnt = 0;//2012,1128
	st_handler.mn_emptyin_cnt = 0;//2012,1128
	st_handler.mb_rej_3stacker = FALSE;

	st_handler.mn_smema_front = CTL_NO;

	//2013,0307
	st_handler.mn_robot_motorX_safety = CTL_NO;

	if (st_handler.cwnd_title != NULL)	
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, SMEMA_ONOFF, 0);
	}



	st_handler.mn_curr_back_conv = 0;
	st_handler.mn_smema_rear_run = 1;
	nRearSmemaStep = 0;

	st_handler.mn_unload_cycle = CTL_NO;
	st_handler.mn_load_cycle = CTL_NO;	

	st_handler.mn_chk_lotend = CTL_NO;
	st_handler.mn_smema_rear = CTL_NO;

	alarm.mb_bcr_error = false;
	alarm.mn_bcr_num = 0;
	alarm.mn_emo_alarm = FALSE;

	st_work.mstr_op_id = "12345678";

	for(i=0; i<2; i++)
	{
		m_Animate.m_ID_Buffer1.m_bGlass[i] = FALSE;	
		m_Animate.m_ID_Buffer2.m_bGlass[i] = FALSE;	
		m_Animate.m_ID_Buffer3.m_bGlass[i] = FALSE;	
		m_Animate.m_ID_Buffer4.m_bGlass[i] = FALSE;	
		m_Animate.m_ID_Buffer5.m_bGlass[i] = FALSE;	

		m_Animate.m_ID_Buffer1.m_bSkip[i] = FALSE;
		m_Animate.m_ID_Buffer2.m_bSkip[i] = FALSE;
		m_Animate.m_ID_Buffer3.m_bSkip[i] = FALSE;
		m_Animate.m_ID_Buffer4.m_bSkip[i] = FALSE;
		m_Animate.m_ID_Buffer5.m_bSkip[i] = FALSE;
		
		m_Animate.m_ID_Move_CV1.m_bGlass[i] = FALSE;
	}

	st_work.n_IDBuffer_Info = -1;//2014.1110
// 	st_var.n_newlotin = CTL_NO;
	st_handler.mn_curr_back_conv = 0;
	st_handler.mn_manual_lable = CTL_NO;
	st_var.n_lotstart = ID_LOT_EMPTY;

	st_handler.mn_8580_top = 0;
	st_handler.mn_8580_btm = 0;
	st_handler.m_strVisModel = "";

	st_var.n_use_vis_density = CTL_NO;
	st_var.n_use_vis_top = CTL_NO;
	for(i=0; i<4; i++)
	{
		st_handler.m_bstart[i] = true;
		st_var.n_bcrid_error[i] = CTL_NO;//2015.0327
	}

	for(i=0; i<6; i++)
	{
		st_var.n_use_density_flag[i] = CTL_YES;
		st_var.n_use_density_count[i] = 0;
		st_var.n_err_density_flag[i] = CTL_NO;
	}

	st_var.n_use_density_key = CTL_NO;
	st_var.n_use_top_skip = CTL_NO;

	st_var.mn_new_density_flag = CTL_NO;
	st_var.mn_new_density_count = 0;
	st_var.mn_check_density_flag = CTL_NO;
	st_var.mn_check_density_count = 0;
	
	st_var.n_using_barcode = -1;
	if( st_basic.nBcrFeederUse[0] == CTL_YES && st_basic.nBcrFeederUse[1] == CTL_YES )
	{
		st_var.n_use_barcode[0] = EMBS_PASS;
		st_var.n_use_barcode[1] = EMBS_PASS;
		st_var.n_using_barcode = 0;
	}
	else if( st_basic.nBcrFeederUse[2] == CTL_YES && st_basic.nBcrFeederUse[3] == CTL_YES )
	{
		st_var.n_use_barcode[2] = EMBS_PASS;
		st_var.n_use_barcode[3] = EMBS_PASS;
		st_var.n_using_barcode = 1;
	}
	
	if( st_var.n_using_barcode == -1 )
	{
		st_basic.nBcrFeederUse[0] = CTL_YES;
		st_basic.nBcrFeederUse[1] = CTL_YES;
		st_var.n_use_barcode[0] = EMBS_PASS;
		st_var.n_use_barcode[1] = EMBS_PASS;
		st_var.n_using_barcode = 0;
	}
	

	st_work.mn_run_down = CTL_NO;
	st_work.mn_chg_job = CTL_NO;
	st_work.mn_jam = CTL_NO;
	st_handler.m_dwJamStartTime = GetCurrentTime();
	st_var.n_next_down = CTL_NO;
	st_var.n_tboxmv_forming_cv_mode = CTL_NO;
	st_var.n_tboxmv_sealing_cv_mode = CTL_NO;
	
	st_var.mn_lotend_density_flag = CTL_NO;

	st_handler.mn_model_change = CTL_NO;
	
// 	st_work.n_BTMIDBuffer_Info = -1;//2014.1110				

//	OnIsExistDvcFie();	// 작업할 모델명이 존재하는지 확인
	
	//2015.0530	
	st_work.n_mat_density[0] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_1 );
	st_work.n_mat_density[1] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_2 );
	st_work.n_mat_density[2] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_3 );
	st_work.n_mat_density[3] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_4 );
	st_work.n_mat_density[4] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_5 );
	st_work.n_mat_density[5] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_6 );

	st_work.n_mat_bcr[0] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_1 );
	st_work.n_mat_bcr[1] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_2 );
	st_work.n_mat_bcr[2] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_3 );
	st_work.n_mat_bcr[3] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_4 );
	
	st_work.n_mat_ribbon[0] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_1 );
	st_work.n_mat_ribbon[1] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_2 );
	st_work.n_mat_ribbon[2] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_3 );
	st_work.n_mat_ribbon[3] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_4 );
	

	for ( i =0; i < 6; i++)
	{
		if( st_work.n_mat_density[i] <= 0) st_work.n_mat_density[i] = 2000;
	}
	for ( i =0; i < 4; i++)
	{
		if( st_work.n_mat_bcr[i] <= 0) st_work.n_mat_bcr[i] = 3000;
		if( st_work.n_mat_ribbon[i] <= 0) st_work.n_mat_ribbon[i] = 5000;
	}


	//3주분 삭제
	Func.DeleteOldFiles(st_path.mstr_total, 21);
	Func.DeleteOldFiles(st_path.mstr_tcpip, 21);
	Func.DeleteOldFiles(st_path.mstr_operation, 21);
	Func.DeleteOldFiles(st_path.mstr_interface, 21);
	Func.DeleteOldFiles(st_path.mstr_machine, 21);

	nXgemStep = 0;

	st_handler.m_strNewPartNmber = "";

	//2015.0726
// 	st_handler.m_nConformVisAfterModelChange = CTL_NO;
	st_handler.m_nLotServer = CTL_NO;

	for ( int kk = 0; kk < 4; kk++)
	{
		clsZebra.SetDarkness_TCP(kk);
		clsZebra.LabelTop_TCP(kk);
		clsZebra.OnPrintAnswerMode(2, 0, kk);
		clsZebra.Rewind_Mode_TCP(kk);
		st_handler.m_nConformVisAfterModelChange[i] = CTL_NO;
	}
}	

void CMainFrame::OnMotTeach() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor)))   return;
	OnSwitchToForm(IDW_SCREEN_MOTOR_TEACH);
}

//==================================================================//
// 작업할 모델명이 존재하는지 확인
//==================================================================//
int CMainFrame::OnIsExistDvcFie() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	//==============================================================//
	// 디바이스 정보가 저장된 파일이 존재하는지 검사
	// - 티칭 정보 저장되어 있음
	//==============================================================//
	CString sLoadDvc;	// 로딩할 디바이스명 저장
	CString sExtension;	// 파일 확장자 저장
	int nPos;
	char chData[50];

	// 로딩한 디바이스 정보가 공백인지 검사
	if (st_basic.sDvcName == "")
	{
		::GetPrivateProfileString("BasicData", "DeviceType", "-1", chData, 50, st_path.mstr_basic);
		sLoadDvc = chData;
		sLoadDvc.TrimLeft(' ');
		sLoadDvc.TrimRight(' ');

		// 확장자 위치 검사
		nPos = sLoadDvc.Find(".TXT");
		if (nPos == -1)
		{
			st_basic.sDvcName = sLoadDvc + ".TXT";
		}
		else
		{
			sExtension = sLoadDvc.Mid(nPos);  // 확장자 정보 얻음
			// 확장자가 틀리면 기본 디바이스명을 설정
			if (sExtension != ".TXT")
			{
				st_basic.sDvcName = "DEFAULT.TXT";

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "[DEVICE FILE[DEFAULT.DAT]] The error happened at a file Exist-1.");
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.c_normal_msg);
				}
			}
			else
			{
				st_basic.sDvcName = sLoadDvc;
			}
		}
	}
	//--------------------------------------------------------------//

	//==============================================================//
	// 디바이스 파일이 존재하는지 확인
	//==============================================================//
	CFileFind finder;
	
	BOOL bFlag = finder.FindFile(st_path.mstr_path_dvc + st_basic.sDvcName);
	finder.Close();

	if (bFlag == 1)
	{
		// 정상적으로 디바이스 파일이 존재함
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Device Type File Not Exist] The error happened at a file Exist-1.");
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			Func.On_LogFile_Add(LOG_TOTAL, st_msg.c_normal_msg);
		}

		// 파일이 존재하지 않는다고 경고 알람을 출력시킴
		g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);

		// 900005 0 21 "사용하려는 Device Type이 설정된 파일경로에 없습니다[Device Type 파일 Copy요망]."
//		alarm.mstr_code		= "900005";
//		alarm.mn_count_mode	= 0;
//		alarm.mn_type_mode	= eWARNING;
//		COMI.mn_run_status	= dWARNING;
		CTL_Lib.Alarm_Error_Occurrence(660, CTL_dWARNING, "900005" );

		FuncRet = RET_ERROR;
	}
	//--------------------------------------------------------------//

	return FuncRet;
}


LRESULT CMainFrame::OnMainframe_Work(WPARAM wParam, LPARAM lParam)
{	
	int mn_command_num = wParam;  // 네트워크 작업을 할 구분 변수

	int mn_inter = lParam;
	
	switch (mn_command_num)
	{		
	case 2:		// 베이직 화면에서 정전기 확인
		SetTimer(TMR_MAIN_ELEC_CHK, 100, NULL);
		break;
				
	case 1001:
		OnMainFrame_SelectMessageDisplay();
		break;

	case 1002://ybs
// 		SetTimer(TMR_SMEMA_CLIENT1, 2000, NULL);
// 		SetTimer(TMR_SMEMA_CLIENT2, 2000, NULL);
// 		SetTimer(TMR_SMEMA_CLIENT2, 2000, NULL);
// 		SetTimer(TMR_SMEMA_SERVER, 2000, NULL);
// 		KillTimer(TMR_SMEMA_CLIENT1);
// 		KillTimer(TMR_SMEMA_CLIENT2);
// 		KillTimer(TMR_SMEMA_CLIENT2);
// 		KillTimer(TMR_SMEMA_SERVER);
		break;

	case 1003:
		mn_ServerStep = 0;
		mn_Sinterface = mn_inter;
// 		SetTimer(TMR_SERVERM_CHK, 100, NULL);

	case 1004:
		OnMainFrame_RejectMessageDisplay();
		break;

	case 1005:
		SetTimer(TMR_XGEM_CHK, 1400, 0);
		break;
	}
	
	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CString str_display_time;
	int n_hour, n_minute, n_second;
	int n_min_temp;	// 총 시간 [분] 저장 변수 
	int n_year, n_month, n_day;				// 년, 월, 일 저장 변수
	int nRet;

	int nIndexMin = 0, nIndexSec = 0;
	int nCyclexMin = 0, nCycleSec = 0;

	CString strTimeLog;
	CString mstr_temp;

	CString mStartTime, mEndTime; 
//	char cStartTime[60], cEndTime[60];

	COleDateTime cur_time;
	CTime bin_time;

	if (nIDEvent == TMR_MAIN_REFRESH)
	{
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan diff = cur - st_handler.m_tRef;

		st_handler.m_tRef = cur;

		switch(COMI.mn_run_status)
		{			
		case dRUN://2013,0215
			if(st_work.mn_tl_status == dSELFCHECK)
			{//dSTOP
// 				if( g_lotMgr.GetLotCount() > 0 ) st_work.mn_chg_job = CTL_YES;
// 				else							 st_work.mn_chg_job = CTL_NO;

				if(st_work.mn_chg_job == CTL_YES)
				{
					st_handler.m_tC = st_handler.m_tC + diff;
					st_handler.m_tDC = st_handler.m_tDC + diff;
				}
				st_handler.m_tI = st_handler.m_tI + diff;
				st_handler.m_tDI = st_handler.m_tDI + diff;
			}
			else
			{
				st_handler.m_tR = st_handler.m_tR + diff;
				st_handler.m_tDR = st_handler.m_tDR + diff;
			}
			break;

		case dWARNING:
		case dJAM:		
			st_handler.m_tJ = st_handler.m_tJ + diff;
			st_handler.m_tDJ = st_handler.m_tDJ + diff;
			break;

		case dLOCK:
			st_handler.m_tM = st_handler.m_tM + diff;
			st_handler.m_tDM = st_handler.m_tDM + diff;
			break;
		
		case dLOTEND:												// Lot End 시에도 Stop으로 시간을 올린다.
		case dSTOP:
			if( st_work.mn_jam == CTL_YES ) 
			{
				if( GetCurrentTime() - st_handler.m_dwJamStartTime <= 0 )
				{
					st_handler.m_dwJamStartTime = GetCurrentTime();
				}
				if( GetCurrentTime() - st_handler.m_dwJamStartTime > 60*3*1000 )
				{
					st_work.mn_jam = CTL_READY;
				}				
				st_handler.m_tJ = st_handler.m_tJ + diff;
				st_handler.m_tDJ = st_handler.m_tDJ + diff;
			}
			else if( st_work.mn_jam == CTL_READY )
			{
				st_handler.m_tM = st_handler.m_tM + diff;
				st_handler.m_tDM = st_handler.m_tDM + diff;
			}
			else
			{
				if(st_work.mn_tl_status == dSELFCHECK)
				{
					st_handler.m_tI = st_handler.m_tI + diff;
					st_handler.m_tDI = st_handler.m_tDI + diff;
				}
				else
				{
					st_handler.m_tS = st_handler.m_tS + diff;
					st_handler.m_tDS = st_handler.m_tDS + diff;
				}
			}
			break;
		}

		if (st_handler.cwnd_main != NULL)
		{//2013,0215
			if(st_work.mn_tl_status == dSELFCHECK)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, dSTOP);
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, dRUN);
			}
			else
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, COMI.mn_run_status);
			}
		}
	}
	else if (nIDEvent == TMR_MAT_CHK)
	{
		//2015.0530
		CString strmat="";
		int nDen[6]={0,};
		nDen[0] = 2000 - st_work.n_mat_density[0];
		if(nDen[0] <= 0) nDen[0] = 2000;
		nDen[1] = 2000 - st_work.n_mat_density[1];
		if(nDen[10] <= 0) nDen[1] = 2000;
		nDen[2] = 2000 - st_work.n_mat_density[2];
		if(nDen[2] <= 0) nDen[2] = 2000;
		nDen[3] = 2000 - st_work.n_mat_density[3];
		if(nDen[3] <= 0) nDen[3] = 2000;
		nDen[4] = 2000 - st_work.n_mat_density[4];
		if(nDen[4] <= 0) nDen[4] = 2000;
		nDen[5] = 2000 - st_work.n_mat_density[5];
		if(nDen[5] <= 0) nDen[5] = 2000;
		
		int nBcr1[2] = {0,};
		int nBcr2[2] = {0,};
		int nRin1[2] = {0,};
		int nRin2[2] = {0,};
		nBcr1[0] = 3000 - st_work.n_mat_bcr[0];
		if(nBcr1[0] <= 0) nBcr1[0] = 3000;
		nBcr1[1] = 3000 - st_work.n_mat_bcr[1];
		if(nBcr1[1] <= 0) nBcr1[1] = 3000;
		
		nBcr2[0] = 3000 - st_work.n_mat_bcr[2];
		if(nBcr2[0] <= 0) nBcr2[0] = 3000;
		nBcr2[1] = 3000 - st_work.n_mat_bcr[3];
		if(nBcr2[1] <= 0) nBcr2[1] = 3000;
		
		nRin1[0] = 5000 - st_work.n_mat_ribbon[0];
		if(nRin1[0] <= 0) nRin1[0] = 5000;
		nRin1[1] = 5000 - st_work.n_mat_ribbon[1];
		if(nRin1[1] <= 0) nRin1[1] = 5000;
		nRin2[0] = 5000 - st_work.n_mat_ribbon[2];
		if(nRin2[0] <= 0) nRin2[0] = 5000;
		nRin2[1] = 5000 - st_work.n_mat_ribbon[3];
		if(nRin2[1] <= 0) nRin2[1] = 5000;
		
		
		strmat.Format("DENSITY1, %d, %d", st_work.n_mat_density[0], nDen[0]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 2);
		strmat.Format("DENSITY2, %d, %d", st_work.n_mat_density[1], nDen[1]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("DENSITY3, %d, %d", st_work.n_mat_density[2], nDen[2]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("DENSITY4, %d, %d", st_work.n_mat_density[3], nDen[3]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("DENSITY5, %d, %d", st_work.n_mat_density[4], nDen[4]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("DENSITY6, %d, %d", st_work.n_mat_density[5], nDen[5]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("BCR1, %d, %d", st_work.n_mat_bcr[0], nBcr1[0]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("BCR2, %d, %d", st_work.n_mat_bcr[1], nBcr1[1]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("BCR3, %d, %d", st_work.n_mat_bcr[2], nBcr2[0]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);			
		strmat.Format("BCR4, %d, %d", st_work.n_mat_bcr[3], nBcr2[1]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);			
		strmat.Format("RIBBON1, %d, %d", st_work.n_mat_ribbon[0], nRin1[0]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("RIBBON2, %d, %d", st_work.n_mat_ribbon[1], nRin1[1]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);
		strmat.Format("RIBBON3, %d, %d", st_work.n_mat_ribbon[2], nRin2[0]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);		
		strmat.Format("RIBBON4, %d, %d", st_work.n_mat_ribbon[2], nRin2[0]);
		Func.On_LogFile_Add(LOG_MAT, strmat, 1);		


		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_1, st_work.n_mat_density[0] );
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_2, st_work.n_mat_density[1] );
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_3, st_work.n_mat_density[2]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_4, st_work.n_mat_density[3]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_5, st_work.n_mat_density[4]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_DENSITY_6, st_work.n_mat_density[5]);

		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_1, st_work.n_mat_bcr[0]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_2, st_work.n_mat_bcr[1]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_3, st_work.n_mat_bcr[2]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_BCR_4, st_work.n_mat_bcr[3]);

		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_1, st_work.n_mat_ribbon[0]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_2, st_work.n_mat_ribbon[1]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_3, st_work.n_mat_ribbon[2]);
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_RIBBON_4, st_work.n_mat_ribbon[3]);

	}
	else if (nIDEvent == TMR_SERVERM_CHK)
	{
		nRet = OnServerInterace(mn_Sinterface);
		if (nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_SERVERM_CHK);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_SERVERM_CHK);
		}
	}	
	else if (nIDEvent == TMR_FILE_CREATE)
	{

		// **********************************************************************
		// 파일 생성 시 필요한 시간 설정한다                                     
		// **********************************************************************
		cur_time = COleDateTime::GetCurrentTime();

		COleDateTimeSpan span( 0, 22, 0, 0 ); // 하루.
		COleDateTime timeLastDay( m_timeLastDay );
		COleDateTime timeSpan = COleDateTime::GetCurrentTime() - span;
		if( timeSpan >= timeLastDay )
		{
			Func.OnDailyDataSave();
			Func.UnloadDataClear();
			Func.OnDayDataReset();

			COleDateTimeSpan span2( 1, 0, 0, 0 );
			timeSpan += span2;
			timeSpan.GetAsSystemTime( m_timeLastDay );
			m_timeLastDay.wHour = 0;
			m_timeLastDay.wMinute = 0;
			m_timeLastDay.wSecond = 0;
			m_timeLastDay.wMilliseconds = 0;

			AMTRegistry::RegWriteTime( m_timeLastDay );
		}
		
		n_hour = cur_time.GetHour();
		n_minute = cur_time.GetMinute();
		n_second = cur_time.GetSecond();

		n_year = cur_time.GetYear();  
		n_month = cur_time.GetMonth();  
		n_day = cur_time.GetDay();  

// 		if (n_hour == 21 && n_minute >= 59 && n_second >= 59)
// 		{
// 			Func.OnDailyDataSave();
// 			Func.OnDayDataReset();
// 		}

		n_min_temp = n_hour * 60 + n_minute;

		// 정전기 확인을 위한 시간 확인.
		if (n_second == 0)
		{
			if (n_min_temp % st_basic.mn_electrostatic_check_gap == 0)
			{
//				SetTimer(TMR_MAIN_ELEC_CHK, 200, NULL);
				OnGmsDataSave();
			}
		}
		if ( n_hour != 22 && n_second == 0 )
		{
			if ( n_min_temp % 60 == 0)
			{
				Func.UnloadDataClear();
			}
		}
	}
	else if (nIDEvent == TMR_MAIN_ELEC_CHK)
	{
		nRet = OnElectrostatic_Check();

		if (nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_MAIN_ELEC_CHK);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_MAIN_ELEC_CHK);
		}

	}
	/////////20131115
	else if (nIDEvent == TMR_BCR_SETTING)
	{
		nRet = OnBarcode_SetValue_Change();
		
		if (nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_BCR_SETTING);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_BCR_SETTING);
		}
	}
	/////////
	else if( nIDEvent == TMR_SMEMA_REAR_CHK)
	{
		nRet = OnRear_Smema_Check();
		if (1 || nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_SMEMA_REAR_CHK);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_SMEMA_REAR_CHK);
		}
	}
	else if (nIDEvent == TMR_GMS_CHK)
	{
		clsGms.OnGmsDataCheck();
	}
	else if( nIDEvent == TMR_SMEMA_CLIENT1)//ybs
	{
		if( st_client[CLS_BOTTOM].n_connect == CTL_YES) KillTimer(TMR_SMEMA_CLIENT1);
		KillTimer(TMR_SMEMA_CLIENT1);
		return;
		nSmaCount1++;
		if( nSmaCount1 > 3)
		{
			nSmaCount1 = 0;
			KillTimer(TMR_SMEMA_CLIENT1);
			return;
		}
		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + CLS_BOTTOM, CLIENT_CONNECT, CLS_BOTTOM);	
	}
	else if( nIDEvent == TMR_SMEMA_CLIENT2)
	{
// 		if( st_client[CLS_8570].n_connect == CTL_YES) KillTimer(TMR_SMEMA_CLIENT2);
// 		KillTimer(TMR_SMEMA_CLIENT2);
// 		return;
// 		nSmaCount2++;
// 		if( nSmaCount2 > 3)
// 		{
// 			nSmaCount2 = 0;
// 			KillTimer(TMR_SMEMA_CLIENT2);
// 			return;
// 		}
// 		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + CLS_8570, CLIENT_CONNECT, CLS_8570);	
	}
	else if( nIDEvent == TMR_SMEMA_CLIENT3)
	{
// 		for ( int i = CLS_BCR_PRINTER1; i < CLS_VIS_DENSITY; i++)
// 		{
// 			if( st_client[i].n_connect == CTL_NO)
// 			{
// 				nSmaCount3[i]++;
// 				if( nSmaCount3[i] > 3)
// 				{
// 					nSmaCount3[i] = 0;
// 					KillTimer(TMR_SMEMA_CLIENT2);
// 					break;
// 				}
// 				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + i, CLIENT_CONNECT, i);
// 				break;
// 			}
// 			KillTimer(TMR_SMEMA_CLIENT3);
// 		}
	}
	else if( nIndexSec == TMR_SMEMA_SERVER )
	{
// 		for ( int j = 0; j < 3; j++)
// 		{
// 			if( st_server[j].n_connect == CTL_NO)
// 			{
// 				nSmaCount3[j]++;
// 				if( nSmaCount3[j] > 3)
// 				{
// 					nSmaCount3[j] = 0;
// 					KillTimer(TMR_SMEMA_SERVER);
// 					break;
// 				}
// 				::SendMessage(st_handler.hWnd, WM_SERVER_MSG + j, SERVER_CONNECT, j);
// 				break;
// 			}
// 			KillTimer(TMR_SMEMA_SERVER);
// 		}
		
	}
	////2015.0221
	else if (nIDEvent == TMR_XGEM_CHK)
	{
		nRet = OnXgmInterface();
		if (nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_XGEM_CHK);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_XGEM_CHK);
		}
	}	
	////

	CFrameWnd::OnTimer(nIDEvent);
}

int CMainFrame::OnRear_Smema_Check()
{
	int nFuncRet = CTLBD_RET_PROCEED;

	switch(nRearSmemaStep)
	{
	case 0:
//		if(g_ioMgr.get_in_bit( st_io.i_rear_shuttle_aline_request_signal_chk, IO_ON) == IO_ON ||
//			g_ioMgr.get_in_bit( st_io.i_rear_shuttle_bline_request_signal_chk, IO_ON) == IO_ON )
//		{
//			m_dwTime_RearSmema = GetCurrentTime();
			nRearSmemaStep = 1000;
//		}
//		else// if(g_ioMgr.get_in_bit( st_io.i_rear_shuttle_aline_request_signal_chk, IO_ON) == IO_OFF && g_ioMgr.get_in_bit( st_io.i_rear_shuttle_bline_request_signal_chk, IO_ON == IO_OFF))
//		{
//			m_dwTime_RearSmema = GetCurrentTime();
//			nRearSmemaStep = 2000;
//		}
		break;

	case 1000:
//		if( GetCurrentTime() - m_dwTime_RearSmema < 0 ) m_dwTime_RearSmema = GetCurrentTime();
//		if( GetCurrentTime() - m_dwTime_RearSmema < 1000) break;
//		if(g_ioMgr.get_in_bit( st_io.i_rear_shuttle_aline_request_signal_chk, IO_ON) == IO_ON ||
//			g_ioMgr.get_in_bit( st_io.i_rear_shuttle_bline_request_signal_chk, IO_ON == IO_ON) )
//		{
//			if( st_handler.mn_smema_rear_run != 1)
//			{
//				st_handler.mn_smema_rear_run = 1;
//				if( st_handler.cwnd_title !=NULL)
//				{
//					st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, SMEMA_ONOFF, 0);
//				}
//			}
//		}
		nRearSmemaStep = 0;
		break;

	case 2000:
//		if( GetCurrentTime() - m_dwTime_RearSmema < 0 ) m_dwTime_RearSmema = GetCurrentTime();
//		if( GetCurrentTime() - m_dwTime_RearSmema < 1000) break;
//		if(g_ioMgr.get_in_bit( st_io.i_rear_shuttle_aline_request_signal_chk, IO_ON) == IO_OFF &&
//			g_ioMgr.get_in_bit( st_io.i_rear_shuttle_bline_request_signal_chk, IO_ON) == IO_OFF )
//		{
//			if( st_handler.mn_smema_rear_run != 0)
//			{
//				st_handler.mn_smema_rear_run = 0;
//				if( st_handler.cwnd_title !=NULL)
//				{
//					st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, SMEMA_ONOFF, 0);
//				}
//			}
//		}
		nRearSmemaStep = 0;
		break;
	}

	return nFuncRet;

}

int CMainFrame::OnElectrostatic_Check()
{
	int nFunRet = CTLBD_RET_PROCEED;
	int i;

	// 시리얼 포트가 생성되지 않았다면 그냥 패스...
	if (st_handler.mnSerial_Port_Creating[GMS_PORT-1] != CTL_YES)
	{
		return CTLBD_RET_GOOD;
	}
	
	switch (nElectrostaticCheckStep)
	{
	case 0:
		strElectCommand.Format("%c%cP%-8sCH%d %c", 0x10, 0x02, "SAMSUNG1", nElectChannel, 0x03);
		st_serial.mstr_snd[GMS_PORT-1] = strElectCommand;
		st_work.mn_elec_serial_work = CTL_READY;
		lElectSerialTime[0] = GetCurrentTime();
		OnDataSend(GMS_PORT, 0);
		nElectrostaticCheckStep = 100;
		break;
		
	case 100:
		if (st_work.mn_elec_serial_work == CTL_YES)				// 정상 완료
		{
			nElectrostaticCheckStep = 200;
		}
		else if (st_work.mn_elec_serial_work == CTL_NO)			// 비정상 완료
		{
			nFunRet = CTLBD_RET_ERROR;
			nElectrostaticCheckStep = 0;
		}
		else													// 이도저도 아닐때...
		{
			lElectSerialTime[1] = GetCurrentTime();
			lElectSerialTime[2] = lElectSerialTime[1] - lElectSerialTime[0];
			
			if (lElectSerialTime[2] > 100000)					// 10초 대기
			{
				nFunRet = CTLBD_RET_ERROR;
				nElectrostaticCheckStep = 0;
			}
			else if(lElectSerialTime[2] < 0)//2013,0608
			{
				lElectSerialTime[0] = GetCurrentTime();
			}
		}
		break;
		
	case 200:
		if (nElectChannel == 5)									// 마지막 채널까지 갔으면...
		{
			nElectChannel = 1;									// 리셋하고.
			nElectrostaticCheckStep = 300;
		}
		else if (nElectChannel < 5)
		{
			nElectChannel++;
			nElectrostaticCheckStep = 0;
		}
		break;
		
	case 300:
		st_grounddb.mole_date = COleDateTime::GetCurrentTime();
// 		Func.DB_Write_Ground(st_grounddb);			// 저장한다.
		
		for (i = 0; i < 5; i++)
		{
			if (st_work.mdReadElectData[i]  > st_basic.md_electrostatic_value)		// 읽은값 + 공차가 기본값 보다 크면
			{
				nFunRet = CTLBD_RET_ERROR;
				nElectrostaticCheckStep = 0;
				break;
			}
		}
		
		// 정상이니까 리셋.
		for (i = 0; i < 5; i++)
		{
			st_work.mdReadElectData[i] = 0;
			st_grounddb.mstrChannel[i] = "";
		}
		
		nElectrostaticCheckStep = 0;
		nFunRet = CTLBD_RET_GOOD;
		break;
	}
	
	return nFunRet;
}

void CMainFrame::OnMotSpeed() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor_Speed)))   return;
	OnSwitchToForm(IDW_SCREEN_MOTOR_SPEED);	
}

void CMainFrame::OnSetInterface() 
{
// 	// **************************************************************************
//     // 화면 뷰 전환 불가능한 정보 검사한다.                                      
//     // **************************************************************************
// 	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
// 	if (nmenu_chk != TRUE)  return;
// 	
// 	// **************************************************************************
// 	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
// 	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
// 	// **************************************************************************
// 	if (st_handler.mn_level_teach !=  TRUE)
// 	{
// 		if (st_handler.mn_level_maint !=  TRUE) 
// 		{
// 			OnLevel();
// 			if (st_handler.mn_level_teach !=  TRUE)
// 			{
// 				if (st_handler.mn_level_maint !=  TRUE)  return;
// 			}
// 		}
// 	}
// 	// **************************************************************************
// 	
// 	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Interface)))   return;
// 	OnSwitchToForm(IDW_SCREEN_SET_INTERFACE);	
}

void CMainFrame::OnSetMaintenance() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
// 	// **************************************************************************
// 	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
// 	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
// 	// **************************************************************************
// 	if (st_handler.mn_level_teach !=  TRUE)
// 	{
// 		if (st_handler.mn_level_maint !=  TRUE) 
// 		{
// 			OnLevel();
// 			if (st_handler.mn_level_teach !=  TRUE)
// 			{
// 				if (st_handler.mn_level_maint !=  TRUE)  return;
// 			}
// 		}
// 	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))   return;
	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);	
}
////2015,0109
void CMainFrame::OnSetNetwork()
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Network)))   return;
	OnSwitchToForm(IDW_SCREEN_SET_NETWORK);
}
////
void CMainFrame::OnSetBarcode() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Barcode)))   return;
	OnSwitchToForm(IDW_SCREEN_SET_BARCODE);
}

void CMainFrame::OnAdmin() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Admin)))   return;
	OnSwitchToForm(IDW_SCREEN_ADMINISTRATOR);
}



void CMainFrame::OnMain_Thread_Creating()
{
	// **********************************************************************
	// 스위치 검사 동작 제어 쓰레드 생성한다                                 
	// **********************************************************************
	m_thread[THREAD_HANDLER_CHK]=AfxBeginThread(OnThread_Handler_Check, this);
	if (m_thread[THREAD_HANDLER_CHK] != NULL) 	
		hThrHandle[THREAD_HANDLER_CHK] = m_thread[THREAD_HANDLER_CHK]->m_hThread;
	// **********************************************************************

	// **********************************************************************
	// 타워 램프 출력 동작 제어 쓰레드 생성한다                              
	// **********************************************************************
	m_thread[THREAD_TOWER_LAMP_CHK]=AfxBeginThread(OnThread_Tower_Lamp_Check, this);
	if (m_thread[THREAD_TOWER_LAMP_CHK] != NULL) 	
		hThrHandle[THREAD_TOWER_LAMP_CHK] = m_thread[THREAD_TOWER_LAMP_CHK]->m_hThread;
	// **********************************************************************
	
	// **********************************************************************
	// 장비 동작 중 발생한 알람 출력하기 위한 쓰레드 생성한다                
	// **********************************************************************
	m_thread[THREAD_ALARM_DISPLAY]=AfxBeginThread(OnThread_Alarm_Display, this);
	if (m_thread[THREAD_ALARM_DISPLAY] != NULL) 	
		hThrHandle[THREAD_ALARM_DISPLAY] = m_thread[THREAD_ALARM_DISPLAY]->m_hThread;
	// **********************************************************************	

	// **********************************************************************
	//  IO 동작을 위한 쓰레드를 생성한다.                         
	// **********************************************************************
	m_thread[THREAD_IO]=AfxBeginThread(OnThread_IO,this);
	if (m_thread[THREAD_IO] != NULL) 	
		hThrHandle[THREAD_IO] = m_thread[THREAD_IO]->m_hThread;
	// **********************************************************************

	m_thread[THREAD_NETWORK] = AfxBeginThread( OnThread_Network, this );
	if( m_thread[THREAD_NETWORK] != NULL )
		hThrHandle[THREAD_NETWORK] = m_thread[THREAD_NETWORK]->m_hThread;

	m_thread[THREAD_MOTORS] = AfxBeginThread( OnThread_Motors, this );
	if( m_thread[THREAD_MOTORS] != NULL )
		hThrHandle[THREAD_MOTORS] = m_thread[THREAD_MOTORS]->m_hThread;
}

void CMainFrame::OnMain_Thread_Destroy()
{
	for( int i=0; i<THREAD_COUNT; i++ )
	{
		if (hThrHandle[i])  // 쓰레드 존재
		::WaitForSingleObject(hThrHandle[i], 1500);
	}
}

///////////////20131115
LRESULT CMainFrame::OnSerialPort(WPARAM wParam,LPARAM lParam)
{
	if(wParam == CTL_YES)
	{
		OnMain_Port_Create();
	}
	else if(wParam == CTL_NO)
	{
		OnMain_Port_Close();
	}
	return 0;
}
///////////////

void CMainFrame::OnMain_Port_Create()
{
//	char parity;
	int i;
//	DWORD dwCommEvents[MAX_PORT];

	for ( i = 0; i < 3; i++)
	{
  		OnOpen(i);
	}
	return;

//	for (i = 0; i < MAX_PORT; i++)
//	{		
//		if (st_handler.mnSerial_Port_Creating[i] == CTL_NO)		// 생성되어 있지 않을때만, 생성한다.
//		{
//			// **************************************************************************
//			//  포트 생성한다                                                    
//			// **************************************************************************
//			dwCommEvents[i] = m_ports[i].MmdSerialGetCommEvents();  // 시리얼 이벤트 설정
//			dwCommEvents[i] = 0;
//			dwCommEvents[i] |= EV_CTS;
//			dwCommEvents[i] |= EV_RXCHAR;
//// 		
//// 			if ( i == 0 )
//// 			{			
//// 				if (m_ports[i].MmdSerialInitializePort(this, i + 1, 9600, 78, 8, 1, 1024, dwCommEvents[i])) 
//// 					m_ports[i].MmdSerialStartMonitoring();
//// 				else 
//// 					st_serial.mstr_port_chk[i] = "NOT FOUND";				
//// 			}
//// 			else
//// 			{
//// 				if (m_ports[i].MmdSerialInitializePort(this, i + 1, 57600, 78, 8, 1, 1024, dwCommEvents[i])) 
//// 					m_ports[i].MmdSerialStartMonitoring();
//// 				else 
//// 					st_serial.mstr_port_chk[i] = "NOT FOUND";				
//// 			}
////////20131210
//			switch(rs_232.n_serial_parity[i])
//			{
//				case 0:
//					parity = 'N';
//					break;
//		
//				case 1:
//					parity = 'O';
//					break;
//
//				case 2:
//					parity = 'E';
//					break;						
//					
//				default:
//					parity = 'N';
//					break;
//			}
//
//			if (m_ports[i].MmdSerialInitializePort(this, rs_232.n_serial_port[i], rs_232.n_serial_baudrate[i], parity, rs_232.n_serial_data[i], rs_232.n_serial_stop[i], 1024, dwCommEvents[i])) 
//				m_ports[i].MmdSerialStartMonitoring();
//			else 
//				st_serial.mstr_port_chk[i] = "NOT FOUND";
////////
//
//			mstr_received[i].Empty();  // 수신 메시지 임시 저장 변수 초기화
//			// **************************************************************************
//			
//			if (st_serial.mstr_port_chk[i] == "NOT FOUND")
//			{
//				st_handler.mnSerial_Port_Creating[i] = CTL_READY;
//			}
//			else
//			{
//				st_handler.mnSerial_Port_Creating[i] = CTL_YES;
//			}
//		}
//	}
}

void CMainFrame::OnExitData_Saving()
{
	st_handler.mn_menu_lock = TRUE;
	
	if (st_handler.cwnd_basic != NULL)				// BASIC 화면 존재
	{
		st_handler.cwnd_basic->SendMessage(WM_BASIC_APPLY, 0, 0);					// BASIC DATA APPLY 요청
	}
	else if (st_handler.cwnd_maintenance != NULL)	// Tower Lamp 화면 존재
	{
		st_handler.cwnd_maintenance->SendMessage(WM_MAINTENANCE_APPLY, 0, 0);		// Tower Lamp DATA APPLY 요청
	}
	else if (st_handler.cwnd_testreference != NULL)	// Test Reference 화면 존재
	{
		st_handler.cwnd_testreference->SendMessage(WM_TESTREFERENCE_APPLY, 0, 0);	// Test Reference DATA APPLY 요청
	}
	else if (st_handler.cwnd_waittime != NULL)		// Wait Time 화면 존재
	{
		st_handler.cwnd_waittime->SendMessage(WM_WAITTIME_APPLY, 0, 0);				// Wait Time DATA APPLY 요청
	}
	else if (st_handler.cwnd_motorspeed != NULL)	// Motor Speed 화면 존재
	{
		st_handler.cwnd_motorspeed->SendMessage(WM_MOTORSPEED_APPLY, 0, 0);			// Motor Speed DATA APPLY 요청
	}
	
	/* ************************************************************************** */
    /* 화면에 설정된 모든 데이터 파일에 저장한다                                  */
	/* -> 프로그램 종료 시점에서 오퍼레이터가 데이터 저장 파일 삭제하는 경우 대비 */
	/* -> 장비 동작 중에 파일 삭제되더라도 현재 설정 정보 유지된다                */
    /* ************************************************************************** */
	mcls_m_basic.OnBasic_Data_Save();				// 전역 변수에 저장된 Basic 셋팅 정보를 파일에 저장하는 함수
	mcls_m_basic.OnWaitTime_Data_Save();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 저장하는 함수
	mcls_m_basic.OnMotorSpeed_Set_Data_Save();		// 전역 변수에 저장된 Motor Speed 셋팅 정보를 파일에 저장하는 함수.
	mcls_m_basic.On_Teach_Data_Save();
	mcls_m_basic.OnMaintenance_Data_Save();
	mcls_m_basic.OnInterface_Data_Save();//20131115
//	mcls_m_basic.OnBarcode_Label_Data_Save();

	st_handler.mn_menu_lock = FALSE;
}

void CMainFrame::OnMain_Port_Close()
{
	int i;
	// **************************************************************************
	// 생성한 RS-232C 시리얼 포트 해제한다                                       
	// -> 동일한 RS-232C 시리얼 포트를 다른 화면에서 사용하고자 할 경우에는      
	//    반드시 어느 한 부분의 포트는 일단 해제한 후 사용해야 한다              
	//  : 이미 해당 RS-232C 시리얼 포트 열린 상태에서 다시 열 수 없기 때문이다   
	// **************************************************************************

	for ( i = 0; i < 3; i++)
	{
		OnClose(i);
	}
// 	if(st_serial.b_Manual_Port_close == CTL_NO)
// 	{
//		for(i = 0; i < MAX_PORT; i++)	//20120620
//		{
//			if(st_handler.mnSerial_Port_Creating[i] == CTL_YES)
//			{
//				m_ports[i].MmdSerialDestroyObject();
//				st_serial.mstr_port_chk[i] = "NOT FOUND";
//				st_handler.mnSerial_Port_Creating[i] = CTL_NO;
//			}	
//		}
// 	}

}

void CMainFrame::OnMain_Motor_Destroy()
{

}

LRESULT CMainFrame::OnMainMessageEvent(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	CString strTemp;
	
	if (wParam == CTL_YES)
	{
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->SetFocus();	// 대화상자를 활성화
			mp_msg_dlg->OnEventMsg_Text_Set();
			if(lParam == 2)
				g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
		}
		else
		{
			mp_msg_dlg = new CDialog_Event_Msg;
			//2017.0510
// 			mp_msg_dlg = new CDialog_Event_Msg( st_msg.mstr_event_msg[0], st_msg.mstr_event_msg[1], st_msg.mstr_event_msg[2]);
			mp_msg_dlg->Create();
			mp_msg_dlg->ShowWindow(SW_SHOW);
			g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_ON);
			if (st_lamp.mn_buzzer_mode == CTL_YES)
			{
				if(lParam == 1)
					g_ioMgr.set_out_bit(st_io.o_buzzer_2, IO_ON);
				else
					g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);

			}

		}

	}
	else if (wParam == CTL_NO)
	{
		for (i = 0; i < 3; i++)
		{
			st_msg.mstr_event_msg[i] = "";
		}
		
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->ShowWindow(SW_HIDE);
			mp_msg_dlg->DestroyWindow();
			delete mp_msg_dlg;
			mp_msg_dlg = NULL;
		}
		
		g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_OFF);
		if (st_lamp.mn_buzzer_mode == CTL_YES)
			g_ioMgr.set_out_bit(st_io.o_buzzer_2, IO_OFF);
	}
	
	return 0;
}

void CMainFrame::OnMainFrame_SelectMessageDisplay()
{
	int ReturnVal;
	
	CDialog_Select select_dlg;
	
	st_msg.mstr_confirm_msg = _T(st_handler.mstrSelectMessage);
	ReturnVal = select_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.mnSelectMessage = 1;
	}
	else
	{
		st_handler.mnSelectMessage = 2;
	}
}


void CMainFrame::OnMainFrame_RejectMessageDisplay()
{
	int ReturnVal;
	
	CDialog_Select select_dlg;
	
	g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
	st_msg.mstr_confirm_msg = _T(st_handler.mstrRejectMessage);
	ReturnVal = select_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.mnRejectMessage = 1;
	}
	else
	{
		st_handler.mnRejectMessage = 2;
	}
	g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
}

// ******************************************************************************
// RS-232C 시리얼 포트 제어 함수                                                 
// ******************************************************************************
LONG CMainFrame::OnCommunication(WPARAM port, LPARAM ch)
{
//	// **************************************************************************
//    // RS-232C 시리얼 포트 검사한다                                              
//    // **************************************************************************
//	if (port <= 0 || port > MAX_PORT)  // COMM 포트 검사
//		return -1;
//	// **************************************************************************
//    
//	if (ch == -1)  // 읽기 에러
//	{
//		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//		{
////			st_serial.mcomm_err[port-1] = _T("[수신 데이터] 시리얼 포트 읽기 에러 발생");
//			st_serial.mcomm_err[port-1] = _T("[RCV DATA] Serial port read error.");
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, RECEIVE_MSG);  // 에러 메시지 출력 요청
//		}
//
//		st_serial.mn_rec_chk[port-1] = COM_ERROR;  // 수신 에러 플래그 설정
//		return 0;
//	}
//	else if (ch == -2)  // 쓰기 에러
//	{
//		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//		{
////			st_serial.mcomm_err[port-1] = _T("[수신 데이터] 시리얼 포트 쓰기 에러 발생");
//			st_serial.mcomm_err[port-1] = _T("[RCV DATA] Serial port write error.");
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, SEND_MSG);  // 에러 메시지 출력 요청
//		}
//
//		st_serial.mn_rec_chk[port-1] = COM_ERROR;  // 수신 에러 플래그 설정
//		return 0;
//	}
//
//	if (port == GMS_PORT)
//	{
//		OnMain_Rcv_GMS(port, ch);
//	}
//	/////20131115
//	else if (port == BCR_PORT || port == BCR_PORT2)
//	{
//		OnMain_Rcv_BCR(port, ch);
//	}
//	else if (port == BCR_PORT3 || port == BCR_PORT4 || port == BCR_PORT5 || port == BCR_PORT6)
//	{
//		OnMain_Rcv_LabelPrinter(port, ch);
//	}

	/////

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C 시리얼 포트 이벤트 설정 함수                                          
// ******************************************************************************
LONG CMainFrame::OnCommunicationEvent(WPARAM port, LPARAM event)
{
	// **************************************************************************
    // 이벤트 설정할 시리얼 포트 포트 검사한다                                   
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM 포트 검사
		return -1;
	// **************************************************************************

	if (event & EV_BREAK)
		Func.MsgLog ("EV_BREAK event");
	else if (event & EV_CTS)
		Func.MsgLog ("EV_CTS event");
	else if (event & EV_DSR)
		Func.MsgLog ("EV_DSR event");
	else if (event & EV_ERR)
		Func.MsgLog ("EV_ERR event");
	else if (event & EV_RING)
		Func.MsgLog ("EV_RING event");
	else if (event & EV_RLSD)
		Func.MsgLog ("EV_RLSD event");
	else if (event & EV_RXCHAR)
		Func.MsgLog ("EV_RXCHAR event");
	else if (event & EV_RXFLAG)
		Func.MsgLog ("EV_RXFLAG event");
	else if (event & EV_TXEMPTY)
		Func.MsgLog ("EV_TXEMPTY event");

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C 시리얼 포트를 통한 메시지 전송 함수                                   
// -> wParam : 메시지 전송할 포트 넘버                                           
// ******************************************************************************
LRESULT CMainFrame::OnDataSend(WPARAM wParam, LPARAM lParam)
{	
	int port = wParam; 
	int pos = lParam;
	int i = 0;
	CString data, temp;
	
	switch(port)
	{
		case GMS_PORT:
		case BCR_PORT:
		case BCR_PORT2:
		case BCR_PORT3:
		case BCR_PORT4:
		case BCR_PORT5:
		case BCR_PORT6:
//			OnMain_Snd_Serial(wParam, lParam);
			break;
	}
	return 0;
}

void CMainFrame::OnMain_Snd_Serial(WPARAM wParam, LPARAM lParam)
{
//	int n_serial_chk;  // 데이터 송신 플래그
//	
//	char buf[1024];
//	
//	// **************************************************************************
//	// 송신 데이터 임시 저장 변수 초기화                                         
//	// **************************************************************************
//	memset(&buf, 0, sizeof(buf));
//	//buf[0] = '\0';
//	// **************************************************************************
//	
//	// **************************************************************************
//	// 송신 데이터 중 존재할지 모를 선두/선미 공백 문자 제거한다                 
//	// **************************************************************************
//	//(st_serial.str_snd[wParam-1]).TrimLeft(' ');
//	//(st_serial.str_snd[wParam-1]).TrimRight(' ');
//	// **************************************************************************
//	
//	// **************************************************************************
//	// 송신 데이터 미존재 유무 검사한다                                          
//	// **************************************************************************
//	if (st_serial.mstr_snd[wParam-1] == _T(""))  // 송신 데이터 공백 검사
//	{
//		if (st_handler.cwnd_list != NULL)       // 리스트 바 화면 존재
//		{
////			st_serial.mcomm_err[wParam-1] = _T("[송신 데이터] 전송 데이터 공백 에러 발생");
//			st_serial.mcomm_err[wParam-1] = _T("[Send Data Error] DATA Empty transfer error.");
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, SEND_MSG);  // 에러 메시지 출력 요청
//		}
//		
//		st_serial.mn_snd_chk[wParam-1] = COM_ERROR;  // 송신 에러 플래그 설정
//		return;
//	}
//	// **************************************************************************
//	
//	st_serial.mcomm_snd[wParam-1] = st_serial.mstr_snd[wParam-1];  
//	sprintf(buf, st_serial.mstr_snd[wParam-1]);  // 송신 데이터 설정
//	
//	// **************************************************************************
//	// 입력된 송신 데이터 시리얼 포트를 통해 전송한다                            
//	// **************************************************************************
//	n_serial_chk = (m_ports[wParam-1]).MmdSerialWriteData(buf, strlen(buf), SERIAL_WAIT, SERIAL_TIME_INFINITE);
//	
//	if (n_serial_chk == 0)        // 정상 송신
//	{
//		//if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//		//    st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, 2);  // 송신 메시지 출력 요청
//		
//		st_serial.mn_snd_chk[wParam-1] = CTL_YES;
//	}
//	else if (n_serial_chk == -1)  // 쓰기 에러
//	{
//		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//		{
////			st_serial.mcomm_err[wParam-1] = _T("[송신 데이터] 시리얼 포트 쓰기 에러 발생");
//			st_serial.mcomm_err[wParam-1] = _T("[TR DATA] Serial Port write error.");
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, SEND_MSG);  // 에러 메시지 출력 요청
//		}
//		
//		st_serial.mn_snd_chk[wParam-1] = -1;  
//	}
//	else if (n_serial_chk == -2)  // TIME OUT
//	{
//		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//		{
////			st_serial.mcomm_err[wParam-1] = _T("[송신 데이터] 시리얼 포트 TIME OUT 발생");
//			st_serial.mcomm_err[wParam-1] = _T("[TR DATA] Serial port TIME OUT.");
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, SEND_MSG);  // 에러 메시지 출력 요청
//		}
//		
//		st_serial.mn_snd_chk[wParam-1] = -2;
//	}
//	// **************************************************************************
}


void CMainFrame::OnMain_Rcv_GMS(WPARAM port, LPARAM ch)
{
//	CString str_check;
//	int nRet;
//	
//	if (ch == 0x03)  // 데이터 구분자 검사
//	{
//		mstr_received[port-1] += (char)ch;
//		mstr_received[port-1].TrimLeft(' ');
//		mstr_received[port-1].TrimRight(' ');
//		
//		st_serial.mstr_rec[port-1] = mstr_received[port-1];
//		
////		st_msg.mstr_normal_msg = mstr_received[port-1];
//		sprintf(st_msg.c_normal_msg, mstr_received[port-1]);
//		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//		
//		(mstr_received[port-1]).Empty();
//		
//		nRet = Func.OnElect_CheckData(st_serial.mstr_rec[port-1]);
//		
//		if (nRet == CTLBD_RET_GOOD)
//		{
//			st_work.mn_elec_serial_work = CTL_YES;		// 정상적인 답변이 왔다.
//			st_serial.mn_rec_chk[port-1] = TRUE;
//		}
//		else if (nRet == CTLBD_RET_ERROR)			// 길이가 안맞아서 에러.
//		{
//			st_work.mn_elec_serial_work = CTL_NO;		// 비정상적인 답변이 왔다.
//			st_serial.mn_rec_chk[port-1] = FALSE;
//		}
//		
//		st_serial.mcomm_rec[port-1] = st_serial.mstr_rec[port-1];  // 리스트 화면에 출력할 수신 데이터 저장
//	}
//	else
//	{
//		mstr_received[port-1] += (char)ch;
//	}
}	

void CMainFrame::OnClose() 
{
	OnExit();
}

void CMainFrame::OnDestroy() 
{
	/* ************************************************************************** */
	/* 만약 모달리스 알람 화면 삭제되지 않았다면 재 삭제 작업                     */
	/* -> mp_modal_dlg : 알람 화면에 대한 포인터 저장 변수                        */
	/* ************************************************************************** */
	if (mp_alarm_dlg)
	{
		OnAlarm_Destroy();			// 알람 화면 종료 함수 */
		delete this;
	}
	if(mp_msg_dlg)
	{
		mp_msg_dlg->DestroyWindow();
		delete mp_msg_dlg;
		mp_msg_dlg = NULL;
		
	}
	if(mp_xmsg_dlg)
	{
		mp_xmsg_dlg->DestroyWindow();
		delete mp_xmsg_dlg;
		mp_xmsg_dlg = NULL;
	}
	/* ************************************************************************** */
	// 규이리 추가 [2014.08.12]
	// 소켓 관련 정보 삭제
	int i;
	for(i=0; i<15; i++)
	{
		if (m_pServer[i] != NULL)
		{
			delete m_pServer[i];
			m_pServer[i] = NULL;
		}
		
		if (m_pClient[i] != NULL)
		{
			delete m_pClient[i];
			m_pClient[i] = NULL;
		}
	}
	
	OnMain_Port_Close();			// 생성한 시리얼 포트 해제 함수
	
	KillTimer(TMR_MAIN_REFRESH);	// Time 출력 타이머 삭제
	KillTimer(TMR_FILE_CREATE);
	KillTimer(TMR_GMS_CHK);
	KillTimer(TMR_MAT_CHK);

	CFrameWnd::OnDestroy();
}

void CMainFrame::OnAlarm_Destroy()
{
	if (mp_alarm_dlg)
	{
		if (mp_alarm_dlg->IsWindowVisible())
		{
			mp_alarm_dlg->DestroyWindow();
		}
		
		if (st_handler.mn_alarm_screen != FALSE)  // 알람 화면 출력
			st_handler.mn_alarm_screen = FALSE;
	}
}


void CMainFrame::OnListOper() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Operation)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_OPERATION);
}

void CMainFrame::OnListDataview() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Data_View)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_DATA_VIEW);
}

void CMainFrame::OnFileEditor() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_File_Editor)))   return;
	OnSwitchToForm(IDW_SCREEN_FILE_EDITOR);	
}

void CMainFrame::OnEcserver()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Server_Network)))   return;
	OnSwitchToForm(IDW_SCREEN_ECSERVER);	
}


/////////20131115
void CMainFrame::OnMain_Rcv_BCR(WPARAM port, LPARAM ch)
{
//	CString str_check;
//	int nRet;
//	
//	if (ch == 0x03)  // 데이터 구분자 검사
//	{
//		//mstr_received[port-1] += (char)ch;
//		mstr_received[port-1].TrimLeft(' ');
//		mstr_received[port-1].TrimRight(' ');
//		
//		st_serial.mstr_rec[port-1] = mstr_received[port-1];
//		
//		sprintf(st_msg.c_normal_msg, mstr_received[port-1]);
//		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//		
//		(mstr_received[port-1]).Empty();
//		
//		nRet = Func.OnBCR_CheckData(st_serial.mstr_rec[port-1]);
//		
//		if (nRet == CTLBD_RET_GOOD)
//		{
//			st_work.n_barcode_recive = CTL_YES;		// 정상적인 답변이 왔다.
//			st_serial.mn_rec_chk[port-1] = TRUE;
//		}
//		else if (nRet == CTLBD_RET_ERROR)					// 길이가 안맞아서 에러.
//		{
//			st_work.strBarcodeRecive = "NoRead";
//			st_work.n_barcode_recive = CTL_NO;		// 비정상적인 답변이 왔다.
//			st_serial.mn_rec_chk[port-1] = FALSE;
//		}
//		
//		st_serial.mcomm_rec[port-1] = st_serial.mstr_rec[port-1];  // 리스트 화면에 출력할 수신 데이터 저장
//	}
//	else
//	{
//		mstr_received[port-1] += (char)ch;
//	}
//	
}


void CMainFrame::OnMain_Rcv_LabelPrinter(WPARAM port, LPARAM ch)
{
	CString str_check;
	int nRet;
	
	if (ch == 0x03)   // 데이터 구분자 검사 [ETX]
	{
		mstr_received[port-1] += (char)ch;
		mstr_received[port-1].TrimLeft(' ');
		mstr_received[port-1].TrimRight(' ');
		
		st_serial.mstr_rec[port-1] = mstr_received[port-1];

		sprintf(st_msg.c_normal_msg, mstr_received[port-1]);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		
		(mstr_received[port-1]).Empty();
		
		nRet = Func.OnLabel_CheckData(st_serial.mstr_rec[port-1], port-1);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			st_work.n_label_recive[port-1] = CTL_YES;		// 정상적인 답변이 왔다.
			st_serial.mn_rec_chk[port-1] = TRUE;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			st_work.n_label_recive[port-1] = CTL_NO;		// 비정상적인 답변이 왔다.
			st_serial.mn_rec_chk[port-1] = FALSE;
		}
	}
	//////////////////////////////////////////////////////// 20120707
	else if(ch == 0x5d)   // "]"
	{
		/////////////// 20120906
		mstr_received[port-1] += (char)ch;
		st_serial.mstr_rec[port-1] = mstr_received[port-1];
		
		str_check.Empty();
		AfxExtractSubString(str_check, mstr_received[port-1], 3, ' ');//st_serial.comm_rec[port], 2, ' ');
		str_check.TrimLeft(' ');
		str_check.TrimLeft(' ');
		mstr_received[port-1].Empty();
		
		if (str_check == "COMPLETED")
		{
			sprintf(st_msg.c_normal_msg, mstr_received[port-1]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			st_serial.mn_rec_chk[port-1] = TRUE;
			st_barcode.b_print_complete[port-1] = TRUE;
		}
		else
		{
			st_serial.mn_rec_chk[port-1] = FALSE;
			st_barcode.b_print_complete[port-1] = FALSE;
		}
	}
	else
	{
		mstr_received[port] += (char)ch;
	}
}



int CMainFrame::OnBarcode_SetValue_Change()
{
	int nFunRet = CTLBD_RET_PROCEED;

	CString strBcrCommand="";

	switch ( mn_BarcodeSetValueChangeStep )
	{
	case 0:
		strBcrCommand.Format("%csMN SetAccessMode 3 7A99FDC6%c", 0x02, 0x03);
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);

		mn_BarcodeSetValueChangeStep = 10;
		break;

	case 10:
		if ( st_work.str_barcode_success == "SAN SETACCESSMODE 1" )
		{
			mn_BarcodeSetValueChangeStep = 100;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);

			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;

	case 100:	// 거리
		strBcrCommand.Format("%csWN CAMFocPos %X%c", 0x02, st_basic.n_distance, 0x03);
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);

		mn_BarcodeSetValueChangeStep = 110;
		break;

	case 110:
		if ( st_work.str_barcode_success == "SWA CAMFOCPOS" )
		{
			mn_BarcodeSetValueChangeStep = 200;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);

			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;
	
	case 200:	// 셔터 속도
		strBcrCommand.Format("%csWN CAMShutterTime %X%c", 0x02, st_basic.n_shutter_time, 0x03);
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
		
		mn_BarcodeSetValueChangeStep = 210;
		break;
		
	case 210:
		if ( st_work.str_barcode_success == "SWA CAMSHUTTERTIME" )
		{
			mn_BarcodeSetValueChangeStep = 300;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
			
			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;
		
	case 300:	// 조명
// 		strBcrCommand.Format("%csWN CAMBrightness %X%c", 0x02, st_basic.n_brightness, 0x03);
// 		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
// 		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
// 		
		mn_BarcodeSetValueChangeStep = 310;
		break;
		
	case 310:
// 		if ( st_work.str_barcode_success == "SWA CAMBRIGHTNESS" )
// 		{
			mn_BarcodeSetValueChangeStep = 400;
// 		}
// 		else
// 		{
// 			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
// 			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
// 			
// 			mn_BarcodeSetValueChangeStep = 0;
// 			return CTLBD_RET_GOOD;
// 		}
		break;

	case 400:	// 명암
		strBcrCommand.Format("%csWN CAMContrast %X%c", 0x02, st_basic.n_contrast, 0x03);
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);

		mn_BarcodeSetValueChangeStep = 410;
		break;
		
	case 410:
		if ( st_work.str_barcode_success == "SWA CAMCONTRAST" )
		{
			mn_BarcodeSetValueChangeStep = 500;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);

			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;	

	case 500:	// 바코드 굵기
		strBcrCommand.Format("%csWN D1DDMinModSize %X%c", 0x02, OnMain_writeFloat(st_basic.d_1d_width), 0x03);// 20130611 kjh
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
		
		mn_BarcodeSetValueChangeStep = 510;
		break;

	case 510:
		if ( st_work.str_barcode_success == "SWA D1DDMINMODSIZE" )
		{
			mn_BarcodeSetValueChangeStep = 900;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
			
			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;

	case 900:	// 쓰기
		strBcrCommand.Format("%csMN mEEwriteall%c", 0x02, 0x03);
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
		
		mn_BarcodeSetValueChangeStep = 910;
		break;
		
	case 910:
		if ( st_work.str_barcode_success == "SAN MEEWRITEALL 1" )
		{
			mn_BarcodeSetValueChangeStep = 1000;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);

			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;
		
	case 1000:	// Run Mode
		strBcrCommand.Format("%csMN Run%c", 0x02, 0x03);
		st_serial.mstr_snd[BCR_PORT-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
		
		mn_BarcodeSetValueChangeStep = 1010;
		break;
		
	case 1010:
		if ( st_work.str_barcode_success == "SAN RUN 1" )
		{
			mn_BarcodeSetValueChangeStep = 1100;
		}
		else
		{
			st_msg.str_fallacy_msg = _T("BCR 설정이 실패하였습니다. 다시 하세요.");
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);

			mn_BarcodeSetValueChangeStep = 0;
			return CTLBD_RET_GOOD;
		}
		break;
		
	case 1100:
//		sprintf(st_msg.c_normal_msg, "바코드 설정 완료.");
		st_msg.mstr_barcode = "BCR Setting completed.";
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);

		nFunRet = CTLBD_RET_GOOD;
		mn_BarcodeSetValueChangeStep = 0;
		break;
	}

	return nFunRet;
}

int CMainFrame::OnMain_writeFloat(float f)
{ 
	UINT32 u;
	int sign; 
	int exponent; 
	int significand; 
	
	// Extract bits  
	u = *(UINT32*)&f; 
	
	// Extract sign 
	sign = (u & 0x80000000) >> 31;     //  1 bit 
	
	// Extract exponent 
	exponent = (u & 0x7F800000) >> 23; //  8 bit 
	exponent -= 127;                   // exponent bias 
	
	// Extract significand 
	significand = (u & 0x007FFFFF);    // 23 bit 
	significand |= 0x00800000;         // implicit leading bit with value 1 
	
	return u;
}
//////////

/////////////////////////////////////////////////////////////////////////////////////
// 소켓 관련 클래스
// 규이리 추가 [2013.11.12]
/////////////////////////////////////////////////////////////////////////////////////

//==================================================================//
// [EQP<->MES] 통신 담당
//==================================================================//
LRESULT CMainFrame::OnCommand_Client_1(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

			// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
			{
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					st_msg.mstr_barcode.Format("%s", sTmp); 
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;

			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				st_msg.mstr_barcode.Format("%s", sTmp); 
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		break;

	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			// 규이리 주석 처리 [2012.10.04]
			/*
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_other.str_op_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			*/
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, sTmp);
			Func.On_LogFile_Add(LOG_TCPIP, sTmp);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, m_sLogMsg);
		}
		break;

	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		//2017.0510
// 		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

// 		clsBcrFormatClient.OnDataReceive(sTmp);	// [MES->EQP] ¼o½AμE ¸Þ½AAo ±¸ºÐCI¿ⓒ A³¸®

		//2014,1229
		evMes.OnDivide_FromMES(nAddr, sRcv);	// [MES->EQP] 수신된 메시지 구분하여 처리

		Func.On_LogFile_Add(99, m_sLogMsg);
		Func.On_LogFile_Add(LOG_TCPIP, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}
		break;

		// 규이리 추가 [2014.08.13]
	case CLIENT_RECONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			SetTimer(TMR_MES_, 5000, NULL);
		}
		break;
	}
	return 0;
}

//==================================================================//
// [EQP<->CIM] 통신 당당
//==================================================================//
LRESULT CMainFrame::OnCommand_Client_2(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="", sHead="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성
			
			// 시스템 내부 소켓으로 연결 시도
			// - 로컬 어드레스 사용시에는 ("127.0.0.1")
			//if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1000))
			// 규이리 수정 [2013.11.23]
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
			{
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;
			
			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				st_msg.mstr_barcode.Format("%s", sTmp); 
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		SetTimer(TMR_SMEMA_CLIENT1, 5000, 0);
		break;
		
	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			// 규이리 주석 처리 [2012.10.04]
			/*
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_other.str_op_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			*/
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, sTmp);
			Func.On_LogFile_Add(LOG_MACHINE, sTmp);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, m_sLogMsg);
		}
		break;
		
	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
		
		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		//2017.0510
// 		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

		g_btm_client.OnDataReceive(sRcv);
		Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);
		Func.On_LogFile_Add(99, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}
		break;

		// 규이리 추가 [2014.08.13]
	case CLIENT_RECONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
//			SetTimer(TMR_CIM_, 5000, NULL);
		}
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnCommand_Client_3(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="", sHead="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성
			
			// 시스템 내부 소켓으로 연결 시도
			// - 로컬 어드레스 사용시에는 ("127.0.0.1")
			//if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1000))
			// 규이리 수정 [2013.11.23]
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
			{
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
// 					sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;
			
			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				st_msg.mstr_barcode.Format("%s", sTmp); 
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		SetTimer(TMR_SMEMA_CLIENT1, 5000, 0);
		break;
		
	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			// 규이리 주석 처리 [2012.10.04]
			/*
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_other.str_op_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			*/
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, sTmp);
			Func.On_LogFile_Add(LOG_MACHINE, sTmp);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			Func.On_LogFile_Add(99, m_sLogMsg);
			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", m_sLogMsg);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				//st_msg.mstr_barcode.Format("%s", sTmp); 
				//2017.0510
				st_msg.mstr_barcode.Format("%s", m_sLogMsg);
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
		}
		break;
		
	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
		
		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

		g_8570_client.OnDataReceive(sRcv);
		Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);
		Func.On_LogFile_Add(99, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}
		break;

		// 규이리 추가 [2014.08.13]
	case CLIENT_RECONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			SetTimer(TMR_CIM_, 5000, NULL);
		}
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnCommand_Client_4(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="", sHead="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성
			
			// 시스템 내부 소켓으로 연결 시도
			// - 로컬 어드레스 사용시에는 ("127.0.0.1")
			//if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1000))
			// 규이리 수정 [2013.11.23]
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
			{
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;
			
			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				st_msg.mstr_barcode.Format("%s", sTmp); 
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		break;
		
	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			// 규이리 주석 처리 [2012.10.04]
			/*
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_other.str_op_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			*/
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(LOG_MACHINE, sTmp);
			Func.On_LogFile_Add(99, sTmp);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, m_sLogMsg);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		break;
		
	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
		
		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		//2017.0510
// 		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

//		g_client_cim.OnDivide_FromCIM(nAddr, sRcv);	// [MES->CIM] 수신된 메시지 구분하여 처리
		g_next_client.OnDataReceive(sRcv);
		Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);
		Func.On_LogFile_Add(99, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}
		break;

		// 규이리 추가 [2014.08.13]
	case CLIENT_RECONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			SetTimer(TMR_CIM_, 5000, NULL);
		}
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnCommand_Client_5(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="", sHead="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성
			
			// 시스템 내부 소켓으로 연결 시도
			// - 로컬 어드레스 사용시에는 ("127.0.0.1")
			//if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1000))
			// 규이리 수정 [2013.11.23]
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
			{
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 

					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;
			
			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				st_msg.mstr_barcode.Format("%s", sTmp); 
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		break;
		
	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			// 규이리 주석 처리 [2012.10.04]
			/*
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_other.str_op_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			*/
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(LOG_MACHINE, sTmp);
			Func.On_LogFile_Add(99, sTmp);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, m_sLogMsg);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		break;
		
	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
		
		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		//2017.0510
// 		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);
		Func.On_LogFile_Add(99, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}

		clsEC_Client.OnDataReceive(sRcv);	// [MES->CIM] 수신된 메시지 구분하여 처리
		break;

		// 규이리 추가 [2014.08.13]
	case CLIENT_RECONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			SetTimer(TMR_CIM_, 5000, NULL);
		}
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnCommand_Client_6(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="", sHead="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성
			
			// 시스템 내부 소켓으로 연결 시도
			// - 로컬 어드레스 사용시에는 ("127.0.0.1")
			//if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1000))
			// 규이리 수정 [2013.11.23]
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
			{
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 

					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					//sprintf( st_msg.mstr_barcode, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
					st_msg.mstr_barcode.Format("%s", sTmp); 
					Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
				}
				//2017.0510
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;
			
			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				//sprintf( st_msg.mstr_barcode, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				st_msg.mstr_barcode.Format("%s", sTmp); 
				Func.On_LogFile_Add(LOG_TOTAL, st_msg.mstr_barcode);
			}
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		break;
		
	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			// 규이리 주석 처리 [2012.10.04]
			/*
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_other.str_op_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			*/
			//2017.0510
// 			m_sLogMsg = sTmp;
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(LOG_MACHINE, sTmp);
			Func.On_LogFile_Add(99, sTmp);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, m_sLogMsg);
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
		}
		break;
		
	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
		
		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		//2017.0510
// 		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);
		Func.On_LogFile_Add(99, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}
		g_xgem_client.OnDataAnalysis(sRcv);	// [MES->CIM] 수신된 메시지 구분하여 처리
		break;

		// 규이리 추가 [2014.08.13]
	case CLIENT_RECONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			SetTimer(TMR_CIM_, 5000, NULL);
		}
		break;
	}
	return 0;
}

//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnCommand_Server_1(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
	case SERVER_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_server[nAddr].n_connect == CTL_NO)
		{
			m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//			if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
			if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
			{
				// 규이리 추가 [2013.11.12]
				st_server[nAddr].n_connect = CTL_YES;

				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Listening. Server [%02d]..", nAddr);
// 					sprintf( st_msg.mstr_barcode, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
				//2017.0510
// 				m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case SERVER_CLOSE:
		// 연결을 종료시킴
		if (st_server[nAddr].n_connect == CTL_YES)
		{
			st_server[nAddr].n_connect = CTL_NO;

			// 규이리 추가 [2013.11.12]
			if (st_handler.cwnd_list != NULL)
			{
				sTmp.Format("Closed. Server [%02d]..", nAddr);
				//sprintf( st_msg.c_normal_msg, "%s", sTmp);
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);			
			}
			//2017.0510
// 			m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		SetTimer(TMR_SMEMA_SERVER, 5000, 0);
		break;

	case SERVER_SEND:
		sSnd.Format("%s", st_server[nAddr].ch_send);

		// 전송할 데이터가 존재하는지 확인
		if (sSnd != "")
		{
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				// 연결되어 있는 클라이언트 정보 얻음
				nCount = m_pServer[nAddr]->GetClientCount();
				// [서버->클라이언트] 데이터 전송
				m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

				/*
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
				m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
				//2017.0510
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
				//2017.0510
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
		}
		else
		{
			m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(99, m_sLogMsg);
		}
		break;

	case SERVER_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_server[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

		m_sLogMsg.Format("[SRcv] [%02d] %s", nAddr, sRcv);
		//2017.0510
// 		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		Func.On_LogFile_Add(LOG_MACHINE, m_sLogMsg);

		g_front_server.OnDataReceive(sRcv);
		Func.On_LogFile_Add(99, m_sLogMsg);
// 		if (st_handler.cwnd_list != NULL)
// 		{
// 			sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 		}
		break;

	case SERVER_CLIENT_COMM:
		if (st_server[nAddr].n_connect == CTL_NO)
		{
			st_server[nAddr].n_connect = CTL_YES;

			// 규이리 추가 [2013.11.12]
// 			if (st_handler.cwnd_list != NULL)
// 			{
// 				sTmp.Format("Connected. Server [%02d]..", nAddr);
// 				sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 			}
			m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);

			Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
			//2017.0510
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		break;
	}
	return 0;
}

//==================================================================//
// Zebra print 통신 담당
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Client_1(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

// 	st_client[nAddr].str_ip = "192.168.1.101";
// 	st_client[nAddr].n_port = 9100;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
// 					if (st_handler.cwnd_list != NULL)
// 					{
						//sprintf( st_msg.c_normal_msg, "%s", sTmp);
						//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
					//m_sLogMsg = sTmp;
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, sTmp);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			if (Func.m_pZebra[0] != NULL)
			{
				Func.m_pZebra[0]->OnPrintStatus(sRcv, st_client[nAddr].n_port);//2014,1125 ysj
				Func.m_pZebra[0]->OnPrintComplete(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// Zebra print 통신 담당
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Client_2(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				//2017.0510
				Func.On_LogFile_Add(99, sTmp);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
//				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			if (Func.m_pZebra[1] != NULL)
			{
				Func.m_pZebra[1]->OnPrintStatus(sRcv, st_client[nAddr].n_port);//2014,1125 ysj
				Func.m_pZebra[1]->OnPrintComplete(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// Zebra print 통신 담당
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Client_3(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, sTmp);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			if (Func.m_pZebra[2] != NULL)
			{
				Func.m_pZebra[2]->OnPrintStatus(sRcv, st_client[nAddr].n_port);//2014,1125 ysj
				Func.m_pZebra[2]->OnPrintComplete(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// Zebra print 통신 담당
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Client_4(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, sTmp);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			if (Func.m_pZebra[3] != NULL)
			{
				Func.m_pZebra[3]->OnPrintStatus(sRcv, st_client[nAddr].n_port);//2014,1125 ysj
				Func.m_pZebra[3]->OnPrintComplete(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// label vision 통신 담당
//==================================================================//
LRESULT CMainFrame::OnLabel_Client(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsClient[EVP_DENSITY] != NULL)
			{
				Func.m_pVsClient[EVP_DENSITY]->OnDataReceive(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// barcode vision 통신 담당
//==================================================================//
LRESULT CMainFrame::OnBarcode_Client(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsClient[EVP_BARCODE] != NULL)
			{
				Func.m_pVsClient[EVP_BARCODE]->OnDataReceive(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}

//==================================================================//
// top vision 통신 담당
//==================================================================//
LRESULT CMainFrame::OnTop_Client(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);
				Func.On_LogFile_Add(99, sTmp);

// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsClient[EVP_TOP_POS] != NULL)
			{
				Func.m_pVsClient[EVP_TOP_POS]->OnDataReceive(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// bottom vision 통신 담당
//==================================================================//
LRESULT CMainFrame::OnBottom_Client(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
		case CLIENT_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
				{
					st_client[nAddr].n_connect = CTL_YES;
					sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					// 생성한 소켓 인스턴스 해제
					delete m_pClient[nAddr];
					m_pClient[nAddr] = NULL;

					st_client[nAddr].n_connect = CTL_NO;
					sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case CLIENT_CLOSE:
			// 연결을 종료시킴
			if (st_client[nAddr].n_connect == CTL_YES)
			{
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
				Func.On_LogFile_Add(LOG_TOTAL, sTmp);

// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case CLIENT_SEND:
			sSnd.Format("%s", st_client[nAddr].ch_send);

			if (st_client[nAddr].n_connect == CTL_YES)
			{
				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
				sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

				// 규이리 주석 처리 [2012.10.04]
				/*
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_other.str_op_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				*/
				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			else
			{
				m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

		case CLIENT_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_client[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

			m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsClient[EVP_BTM_POS] != NULL)
			{
				Func.m_pVsClient[EVP_BTM_POS]->OnDataReceive(sRcv);
				Func.On_LogFile_Add(99, m_sLogMsg);
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", m_sLogMsg);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
// 				}
			}
			break;

			// 규이리 추가 [2014.08.13]
		case CLIENT_RECONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_client[nAddr].n_connect == CTL_NO)
			{
				SetTimer(TMR_MES_, 5000, NULL);
			}
			break;
	}
	return 0;
}


//==================================================================//
// Get Barcode Format1 from server
//==================================================================//
// LRESULT CMainFrame::OnBcrFormat1_Client(WPARAM wParam, LPARAM lParam)
// {
// 	CString sSnd, sRcv, sTmp;
// 
// 	int nStatus	= (int)wParam;
// 	int nAddr	= (int)lParam;
// 
// 	switch(nStatus)
// 	{
// 		case CLIENT_CONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				m_pClient[nAddr] = new CClientSocket;  // ¼OAI ≫y¼º
// 
// 				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
// 				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
// 				{
// 					st_client[nAddr].n_connect = CTL_YES;
// 					sTmp.Format("BcrFormat1_Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 				else
// 				{
// 					// ≫y¼ºCN ¼OAI AI½ºAI½º CØA|
// 					delete m_pClient[nAddr];
// 					m_pClient[nAddr] = NULL;
// 
// 					st_client[nAddr].n_connect = CTL_NO;
// 					sTmp.Format("BcrFormat1_Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 			}
// 			break;
// 
// 		case CLIENT_CLOSE:
// 			// ¿￢°aA≫ A¾·a½AA´
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				delete m_pClient[nAddr];
// 				m_pClient[nAddr] = NULL;
// 
// 				st_client[nAddr].n_connect = CTL_NO;
// 				sTmp.Format("BcrFormat1_Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_SEND:
// 			sSnd.Format("%s", st_client[nAddr].ch_send);
// 
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
// 				sTmp.Format("BcrFormat1_Client_[%02d] Data Send    - %s", nAddr, sSnd);
// 
// 				// ±OAI¸® AO¼® A³¸® [2012.10.04]
// 				/*
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf(st_other.str_op_msg, sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			else
// 			{
// 				m_sLogMsg.Format("BcrFormat1_Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_REV:
// 			// ¼o½AμE μ￥AIAI¸| ±æAI¸¸A­ ¹Þ¾ÆμeAO
// 			sTmp.Format("%s", st_client[nAddr].ch_rev);
// 			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
// 
// 			m_sLogMsg.Format("BcrFormat1_Client[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 
// 			if (Func.m_pBctFmtClient[0] != NULL)
// 			{
// 				Func.m_pBctFmtClient[0]->OnDataReceive(sRcv);
// 			}
// 			break;
// 
// 			// ±OAI¸® Aß°¡ [2014.08.13]
// 		case CLIENT_RECONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				SetTimer(TMR_MES_, 5000, NULL);
// 			}
// 			break;
// 	}
// 	return 0;
// }
// 
// //==================================================================//
// // Get Barcode Format2 from server
// //==================================================================//
// LRESULT CMainFrame::OnBcrFormat2_Client(WPARAM wParam, LPARAM lParam)
// {
// 	CString sSnd, sRcv, sTmp;
// 
// 	int nStatus	= (int)wParam;
// 	int nAddr	= (int)lParam;
// 
// 	switch(nStatus)
// 	{
// 		case CLIENT_CONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				m_pClient[nAddr] = new CClientSocket;  // ¼OAI ≫y¼º
// 
// 				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
// 				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
// 				{
// 					st_client[nAddr].n_connect = CTL_YES;
// 					sTmp.Format("BcrFormat2_Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 				else
// 				{
// 					// ≫y¼ºCN ¼OAI AI½ºAI½º CØA|
// 					delete m_pClient[nAddr];
// 					m_pClient[nAddr] = NULL;
// 
// 					st_client[nAddr].n_connect = CTL_NO;
// 					sTmp.Format("BcrFormat2_Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 			}
// 			break;
// 
// 		case CLIENT_CLOSE:
// 			// ¿￢°aA≫ A¾·a½AA´
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				delete m_pClient[nAddr];
// 				m_pClient[nAddr] = NULL;
// 
// 				st_client[nAddr].n_connect = CTL_NO;
// 				sTmp.Format("BcrFormat2_Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_SEND:
// 			sSnd.Format("%s", st_client[nAddr].ch_send);
// 
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
// 				sTmp.Format("BcrFormat2_Client_[%02d] Data Send    - %s", nAddr, sSnd);
// 
// 				// ±OAI¸® AO¼® A³¸® [2012.10.04]
// 				/*
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf(st_other.str_op_msg, sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			else
// 			{
// 				m_sLogMsg.Format("BcrFormat2_Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_REV:
// 			// ¼o½AμE μ￥AIAI¸| ±æAI¸¸A­ ¹Þ¾ÆμeAO
// 			sTmp.Format("%s", st_client[nAddr].ch_rev);
// 			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
// 
// 			m_sLogMsg.Format("BcrFormat2_Client[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 
// 			if (Func.m_pBctFmtClient[1] != NULL)
// 			{
// 				Func.m_pBctFmtClient[1]->OnDataReceive(sRcv);
// 			}
// 			break;
// 
// 			// ±OAI¸® Aß°¡ [2014.08.13]
// 		case CLIENT_RECONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				SetTimer(TMR_MES_, 5000, NULL);
// 			}
// 			break;
// 	}
// 	return 0;
// }
// 
// //==================================================================//
// // Get Barcode Format2 from server
// //==================================================================//
// LRESULT CMainFrame::OnBcrFormat3_Client(WPARAM wParam, LPARAM lParam)
// {
// 	CString sSnd, sRcv, sTmp;
// 
// 	int nStatus	= (int)wParam;
// 	int nAddr	= (int)lParam;
// 
// 	switch(nStatus)
// 	{
// 		case CLIENT_CONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				m_pClient[nAddr] = new CClientSocket;  // ¼OAI ≫y¼º
// 
// 				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
// 				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
// 				{
// 					st_client[nAddr].n_connect = CTL_YES;
// 					sTmp.Format("BcrFormat3_Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 				else
// 				{
// 					// ≫y¼ºCN ¼OAI AI½ºAI½º CØA|
// 					delete m_pClient[nAddr];
// 					m_pClient[nAddr] = NULL;
// 
// 					st_client[nAddr].n_connect = CTL_NO;
// 					sTmp.Format("BcrFormat3_Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 			}
// 			break;
// 
// 		case CLIENT_CLOSE:
// 			// ¿￢°aA≫ A¾·a½AA´
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				delete m_pClient[nAddr];
// 				m_pClient[nAddr] = NULL;
// 
// 				st_client[nAddr].n_connect = CTL_NO;
// 				sTmp.Format("BcrFormat3_Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_SEND:
// 			sSnd.Format("%s", st_client[nAddr].ch_send);
// 
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
// 				sTmp.Format("BcrFormat3_Client_[%02d] Data Send    - %s", nAddr, sSnd);
// 
// 				// ±OAI¸® AO¼® A³¸® [2012.10.04]
// 				/*
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf(st_other.str_op_msg, sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			else
// 			{
// 				m_sLogMsg.Format("BcrFormat3_Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_REV:
// 			// ¼o½AμE μ￥AIAI¸| ±æAI¸¸A­ ¹Þ¾ÆμeAO
// 			sTmp.Format("%s", st_client[nAddr].ch_rev);
// 			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
// 
// 			m_sLogMsg.Format("BcrFormat3_Client[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 
// 			if (Func.m_pBctFmtClient[2] != NULL)
// 			{
// 				Func.m_pBctFmtClient[2]->OnDataReceive(sRcv);
// 			}
// 			break;
// 
// 			// ±OAI¸® Aß°¡ [2014.08.13]
// 		case CLIENT_RECONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				SetTimer(TMR_MES_, 5000, NULL);
// 			}
// 			break;
// 	}
// 	return 0;
// }
// 
// //==================================================================//
// // Get Barcode Format2 from server
// //==================================================================//
// LRESULT CMainFrame::OnBcrFormat4_Client(WPARAM wParam, LPARAM lParam)
// {
// 	CString sSnd, sRcv, sTmp;
// 
// 	int nStatus	= (int)wParam;
// 	int nAddr	= (int)lParam;
// 
// 	switch(nStatus)
// 	{
// 		case CLIENT_CONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				m_pClient[nAddr] = new CClientSocket;  // ¼OAI ≫y¼º
// 
// 				// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
// 				if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port))//, 1000))
// 				{
// 					st_client[nAddr].n_connect = CTL_YES;
// 					sTmp.Format("BcrFormat4_Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 				else
// 				{
// 					// ≫y¼ºCN ¼OAI AI½ºAI½º CØA|
// 					delete m_pClient[nAddr];
// 					m_pClient[nAddr] = NULL;
// 
// 					st_client[nAddr].n_connect = CTL_NO;
// 					sTmp.Format("BcrFormat4_Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 					if (st_handler.cwnd_list != NULL)
// 					{
// 						sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 					}
// 					m_sLogMsg = sTmp;
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 				}
// 			}
// 			break;
// 
// 		case CLIENT_CLOSE:
// 			// ¿￢°aA≫ A¾·a½AA´
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				delete m_pClient[nAddr];
// 				m_pClient[nAddr] = NULL;
// 
// 				st_client[nAddr].n_connect = CTL_NO;
// 				sTmp.Format("BcrFormat4_Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);
// 
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_SEND:
// 			sSnd.Format("%s", st_client[nAddr].ch_send);
// 
// 			if (st_client[nAddr].n_connect == CTL_YES)
// 			{
// 				m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
// 				sTmp.Format("BcrFormat4_Client_[%02d] Data Send    - %s", nAddr, sSnd);
// 
// 				// ±OAI¸® AO¼® A³¸® [2012.10.04]
// 				/*
// 				if (st_handler.cwnd_list != NULL)
// 				{
// 					sprintf(st_other.str_op_msg, sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ¸Þ½AAo Aa·A ¿aA≫
// 				}
// 				*/
// 				m_sLogMsg = sTmp;
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			else
// 			{
// 				m_sLogMsg.Format("BcrFormat2_Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 			}
// 			break;
// 
// 		case CLIENT_REV:
// 			// ¼o½AμE μ￥AIAI¸| ±æAI¸¸A­ ¹Þ¾ÆμeAO
// 			sTmp.Format("%s", st_client[nAddr].ch_rev);
// 			sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);
// 
// 			m_sLogMsg.Format("BcrFormat4_Client[%02d] Data Receive - %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
// 
// 			if (Func.m_pBctFmtClient[3] != NULL)
// 			{
// 				Func.m_pBctFmtClient[3]->OnDataReceive(sRcv);
// 			}
// 			break;
// 
// 			// ±OAI¸® Aß°¡ [2014.08.13]
// 		case CLIENT_RECONNECT:
// 			// DISCONNECT ½A¿¡ ¿￢°a AU¾÷ ½Aμμ
// 			if (st_client[nAddr].n_connect == CTL_NO)
// 			{
// 				SetTimer(TMR_MES_, 5000, NULL);
// 			}
// 			break;
// 	}
// 	return 0;
// }

//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Server_1(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
// 					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
// 					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv_PrintSvr1] [%02d] %s", nAddr, sRcv);
			//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			Func.On_LogFile_Add(99, m_sLogMsg);
			if (Func.m_pZebra[0] != NULL)
			{
				Func.m_pZebra[0]->OnPrintComplete(sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Server_2(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
						//sprintf( st_msg.c_normal_msg, "%s", sTmp);
						//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
					//m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv_PrintSvr2] [%02d] %s", nAddr, sRcv);
			//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			
			Func.On_LogFile_Add(99, m_sLogMsg);

			if (Func.m_pZebra[1] != NULL)
			{
				Func.m_pZebra[1]->OnPrintComplete(sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Server_3(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
// 					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv_PrintSvr3] [%02d] %s", nAddr, sRcv);
			//cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			
			Func.On_LogFile_Add(99, m_sLogMsg);

			if (Func.m_pZebra[2] != NULL)
			{
				Func.m_pZebra[2]->OnPrintComplete(sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
					//sprintf( st_msg.c_normal_msg, "%s", sTmp);
					//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnZebraPrint_Server_4(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
// 					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv_PrintSvr4] [%02d] %s", nAddr, sRcv);
			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			
			Func.On_LogFile_Add(99, m_sLogMsg);

			if (Func.m_pZebra[3] != NULL)
			{
				Func.m_pZebra[3]->OnPrintComplete(sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnLabel_Server(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
// 					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					if(nCount < 1 ) break;
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv] [%02d] %s", nAddr, sRcv);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);

			if (Func.m_pVsServer[0] != NULL)
			{
				Func.m_pVsServer[0]->OnDataReceive(0, sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnBarcode_Server(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv] [%02d] %s", nAddr, sRcv);
			Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsServer[1] != NULL)
			{
				Func.m_pVsServer[1]->OnDataReceive(1, sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnTop_Server(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
// 					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					if(nCount < 1 ) break;
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					Func.On_LogFile_Add(99, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv] [%02d] %s", nAddr, sRcv);
			Func.On_LogFile_Add(99, m_sLogMsg);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsServer[2] != NULL)
			{
				Func.m_pVsServer[2]->OnDataReceive(2, sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}


//==================================================================//
// TCP/IP (서버) 알림 처리
//==================================================================//
LRESULT CMainFrame::OnBottom_Server(WPARAM wParam, LPARAM lParam)
{
	CString sSnd="", sRcv="", sTmp="";

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;
	int nCount;
	CString sHead, sFunc;

	switch(nStatus)
	{
		case SERVER_CONNECT:
			// DISCONNECT 시에 연결 작업 시도
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				m_pServer[nAddr] = new CServerSocket;	// 소켓 생성

//				if (m_pServer[nAddr]->Create(nAddr, st_server[nAddr].n_port))
				if (m_pServer[nAddr]->Create(st_server[nAddr].n_port, nAddr))
				{
					// 규이리 추가 [2013.11.12]
					st_server[nAddr].n_connect = CTL_YES;

					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("Listening. Server [%02d]..", nAddr);
// 						sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
						Func.On_LogFile_Add(LOG_TOTAL, sTmp);
					}
// 					m_sLogMsg.Format("Listening. Server [%02d]..", nAddr);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			break;

		case SERVER_CLOSE:
			// 연결을 종료시킴
			if (st_server[nAddr].n_connect == CTL_YES)
			{
				st_server[nAddr].n_connect = CTL_NO;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Closed. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Closed. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_SEND:
			sSnd.Format("%s", st_server[nAddr].ch_send);

			// 전송할 데이터가 존재하는지 확인
			if (sSnd != "")
			{
				if (st_server[nAddr].n_connect == CTL_YES)
				{
					// 연결되어 있는 클라이언트 정보 얻음
					nCount = m_pServer[nAddr]->GetClientCount();
					// [서버->클라이언트] 데이터 전송
					if(nCount < 1 ) break;
					m_pServer[nAddr]->SendClientAt(m_pServer[nAddr]->GetClient(nCount-1), sSnd, sSnd.GetLength());

					/*
					if (st_handler.cwnd_list != NULL)
					{
						sTmp.Format("[SSend] [%02d] %s", nAddr, sSnd);
						sprintf(st_other.str_op_msg, sTmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
					*/
					m_sLogMsg.Format("[SSend] [%02d] %s", nAddr, sSnd);
					Func.On_LogFile_Add(99, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
				else
				{
					m_sLogMsg.Format("[SSend Fail] (not Connected %02d) %s", nAddr, sSnd);
					Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 					cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
				}
			}
			else
			{
				m_sLogMsg.Format("[SSend Fail] [Empty Data %02d] %s", nAddr, sSnd);
				Func.On_LogFile_Add(LOG_TOTAL, m_sLogMsg);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;

		case SERVER_REV:
			// 수신된 데이터를 길이만큼 받아들임
			sTmp.Format("%s", st_server[nAddr].ch_rev);
			sRcv = sTmp.Mid(0, st_server[nAddr].n_rev_length);

			m_sLogMsg.Format("[SRcv] [%02d] %s", nAddr, sRcv);
			Func.On_LogFile_Add(99, m_sLogMsg);
// 			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			if (Func.m_pVsServer[3] != NULL)
			{
				Func.m_pVsServer[3]->OnDataReceive(3, sRcv);
			}
			break;

		case SERVER_CLIENT_COMM:
			if (st_server[nAddr].n_connect == CTL_NO)
			{
				st_server[nAddr].n_connect = CTL_YES;

				// 규이리 추가 [2013.11.12]
				if (st_handler.cwnd_list != NULL)
				{
					sTmp.Format("Connected. Server [%02d]..", nAddr);
// 					sprintf( st_msg.c_normal_msg, "%s", sTmp);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, sTmp);
				}
// 				m_sLogMsg.Format("Connected. Server [%02d]..", nAddr);
// 				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
			break;
	}
	return 0;
}



LRESULT CMainFrame::OnBcrCommSerial(WPARAM wParam,LPARAM lParam)
{
	int mn_comm = wParam; 
	int port = lParam;
	switch(mn_comm)
	{
	case 2:
		OnClear(port);
		break;

	case 3:
		OnClose(port);
		break;

	case 4:
		OnOpen(port);
		break;

	case 5:
		OnReadCount(port);
		break;
	}
	return 0;
}

void CMainFrame::OnClear(int nPort) 
{
	// TODO: Add your control notification handler code here
	if( nPort == BCR_PORT - 1)   clsBarcode.OnBarcodeReset();
	else						 clsBarcode2.OnBarcodeReset();
}

void CMainFrame::OnClose(int nPort) 
{
	// TODO: Add your control notification handler code here
	if( nPort == GMS_PORT - 1)	  clsGms.OnClose();
	else if( nPort == BCR_PORT-1) clsBarcode.OnClose();
	else						  clsBarcode2.OnClose();
}

void CMainFrame::OnOpen(int nPort) 
{
	// TODO: Add your control notification handler code here
	if( nPort == GMS_PORT - 1)		clsGms.OnOpen(GMS_PORT, 9600, NOPARITY, 8, ONESTOPBIT, 0x03);
	else if( nPort == BCR_PORT-1)	clsBarcode.OnOpen(BCR_PORT, 9600, NOPARITY, 8, ONESTOPBIT, 0x03);
	else if( nPort == BCR_PORT2-1)	clsBarcode2.OnOpen(BCR_PORT2, 9600, NOPARITY, 8, ONESTOPBIT, 0x03);
}

void CMainFrame::OnReadCount(int nPort) 
{
	// TODO: Add your control notification handler code here
	int nCount;
	CString strTemp;

	if( nPort == BCR_PORT-1) nCount = clsBarcode.OnBarcodeCount();
	else					 nCount = clsBarcode2.OnBarcodeCount();

	strTemp.Format("%d", nCount);
	if(st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, strTemp);
		st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
	}
}

void CMainFrame::OnGmsDataSave()
{
	int i;
	st_grounddb.mole_date = COleDateTime::GetCurrentTime();
	
	for (i=0; i<5; i++)
	{
		st_grounddb.mstrChannel[i] = clsGms.m_strChann[i];
	}

// 	Func.DB_Write_Ground(st_grounddb);			// 저장한다.
}


int CMainFrame::OnServerInterace( int nInterface)
{
	int nFunRet = CTLBD_RET_PROCEED;

//	FUNCTION=BRAND_DTBT_LABEL_ISSUE LOT_ID=X005600447 BOAT_ID=01 EQP_ID=AP-01 SERIAL=S21GNWAFB00052T 
// 	DTBT_LABEL_CODE=Q550-003629 DTBT_RIBBON_CODE=Q550-002237 MODE=NEW RETRY=0 RETRY_CNT=0

	CString str_lotid = "X00QR30450";//"X005C00447";
	CString str_boatid = "34";
	CString str_Serial = "S1SMNSAFC07070X";//"S1SRNYAF900202D";
	
	CString str_labelcode = "Q550-002482";//"LA68-00037A";	
	CString str_ribboncode = "Q550-001004";//"Q550-0002236";

	CString str_labelmatcode = "Q550-00248";//"Q310-221183";

	CString str_dtntmatcode = "Q550-002482";//"LA96-2144";
	CString str_ribboncode1 = "Q550-001004";//"LA96-2141";

	CString str_oper = "08025454";

	switch(mn_ServerStep)
	{
	case 0:
		mn_ServerStep = 10;
		break;

	case 10:
		if(st_client[CLS_ECSERVER].n_connect == CTL_NO )
		{

			m_dwServerTime = GetCurrentTime();
			mn_ServerStep = 20;
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, 0);
		}
		else
		{
			mn_ServerStep = 100;
		}
		break;

	case 20:
		if(GetCurrentTime() - m_dwServerTime < 0 ) m_dwServerTime = GetCurrentTime();
		else if( GetCurrentTime() - m_dwServerTime > 5000) 
		{
			mn_ServerStep = 10;
		}
		break;

	case 100:
		if(nInterface == 1)
			clsBcrFormatClient.OnBrandDensityLabelIssue(0, str_lotid, str_boatid, str_Serial, "0","0");
		else if(nInterface == 2)
			clsBcrFormatClient.OnBrandBcrLabelIssue(0, str_lotid, str_boatid, str_Serial,str_labelcode, str_ribboncode, "NEW", "0","0");
		else if(nInterface == 3)
			clsBcrFormatClient.OnBrandDensityLabelCCS(0, str_lotid, str_boatid, str_Serial,str_labelmatcode, "0","0");
		else if(nInterface == 4)
		{
//			clsBcrFormatClient.OnBrandDTBTLabelCCS(0, str_lotid, str_boatid, str_Serial,str_dtntmatcode, str_ribboncode1, "0","0");
			if	   ( st_idbuff_info[0].n_exist[0] == 1) str_Serial.Format("%s", st_idbuff_info[0].c_serial[0]);
			else if( st_idbuff_info[0].n_exist[1] == 1) str_Serial.Format("%s", st_idbuff_info[0].c_serial[1]);
			else if( st_idbuff_info[1].n_exist[0] == 1) str_Serial.Format("%s", st_idbuff_info[1].c_serial[0]);
			else if( st_idbuff_info[1].n_exist[1] == 1) str_Serial.Format("%s", st_idbuff_info[1].c_serial[1]);
			else if( st_idbuff_info[2].n_exist[0] == 1) str_Serial.Format("%s", st_idbuff_info[2].c_serial[0]);
			else if( st_idbuff_info[2].n_exist[1] == 1) str_Serial.Format("%s", st_idbuff_info[2].c_serial[1]);
			nFunRet = CTLBD_RET_GOOD;
			mn_ServerStep = 0;
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "NO Serial int Test Site.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			break;
			clsBcrFormatClient.OnBrandDTBTLabelCCS(0, str_lotid, str_boatid, str_Serial, st_handler.m_strLabelCode[0], st_handler.m_strRibbonCode[0], "0","0");
		}
		else if(nInterface == 5)
			clsBcrFormatClient.PX_XClose(0, str_lotid, str_oper, "0", "0");
		else if( nInterface == 6)
			clsBcrFormatClient.OnProdSerial_Info(0, str_lotid, str_boatid, str_Serial, "NO", st_mes_data.srt_RETRY_CNT);
		else if( nInterface == 7 )
			clsBcrFormatClient.OnBoatID_Info(0, str_boatid, "NO", st_mes_data.srt_RETRY_CNT);
		else if( nInterface == 8 )
			clsBcrFormatClient.OnPacking_Master_Info(0, str_lotid, str_Serial, "NO", "0");
		else if( nInterface == 9 )
		{
			int nBoat = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[0];
			clsEC_Client.OnBoatIDModify(CLS_FORMAT, str_lotid, str_boatid, 
				str_Serial, "END", "NO", "0");
		}

			//clsBcrFormatClient.OnBoatIDModify(0, str_lotid, str_boatid, str_Serial, "NEW", "NO", "0");

		
// 		clsBcrFormatClient.SetSerialIn();

		mn_ServerStep = 200;
		m_dwServerTime = GetCurrentTime();
		break;
		

	case 200:
		if(GetCurrentTime() - m_dwServerTime < 0 ) m_dwServerTime = GetCurrentTime();
// 		if( clsBcrFormatClient.GetSerialInReceived() )
// 		{
// 			mn_ServerStep = 1000;
// 		}
		if(GetCurrentTime() - m_dwServerTime > 1000) mn_ServerStep = 1000;
		break;

		
	case 1000:
		nFunRet = CTLBD_RET_GOOD;
		mn_ServerStep = 0;
		break;
	}


	return nFunRet;

}

////2015.0413
int CMainFrame::OnXgmInterface()
{
	int nFunRet = CTLBD_RET_PROCEED;
	
	switch(nXgemStep)
	{
	case 0:
		nXgemStep = 100;
		break;
		
	case 100:
		if( st_basic.mn_mode_xgem == CTL_YES )
		{
			CWnd* m_pWnd = CWnd::FindWindow(NULL,"1");
			CWnd* m_pWnd1 = CWnd::FindWindow(NULL,"XgemInterface");
			if(m_pWnd){
				//기존의 Program을 활성화 시킨다.
				m_pWnd->SetForegroundWindow(); 	
				m_pWnd->ShowWindow(SW_RESTORE);	
			}
			else if(m_pWnd1){
				//기존의 Program을 활성화 시킨다.
				m_pWnd1->SetForegroundWindow(); 	
				m_pWnd1->ShowWindow(SW_RESTORE);	
			}
			else{
				//WinExec("C:\\AMT8520\\bin\\XgemInterface.exe",SW_SHOW);
				::ShellExecute(NULL, NULL, "C:\\AMT8590\\bin\\XgemInterface.exe", NULL, NULL, SW_SHOWNORMAL);
			}
			lXgemTime[0] = GetCurrentTime();
			nXgemStep = 200;
		}
		else
		{
			nFunRet = CTLBD_RET_ERROR;
			nXgemStep = 0;
		}
		break;
		
		
	case 200:
		if(GetCurrentTime() - lXgemTime[0] > 5000) nXgemStep = 300;
		else if(GetCurrentTime() - lXgemTime[0] < 0 ) lXgemTime[0] = GetCurrentTime();
		
		break;
		
	case 300:
		g_xgem_client.XgemInitialize();
		lXgemTime[0] = GetCurrentTime();
		nXgemStep = 400;
		break;
		
	case 400:
		if(GetCurrentTime() - lXgemTime[0] > 3000) nXgemStep = 500;
		else if(GetCurrentTime() - lXgemTime[0] < 0 ) lXgemTime[0] = GetCurrentTime();
		break;
		
	case 500:
		Func.OnSavePPID();
		lXgemTime[0] = GetCurrentTime();
		nXgemStep = 600;
		break;
		
	case 600:
		if(GetCurrentTime() - lXgemTime[0] > 2000) nXgemStep = 1000;
		else if(GetCurrentTime() - lXgemTime[0] < 0 ) lXgemTime[0] = GetCurrentTime();
		break;
		
	case 1000:
		//		g_client_ec.OnSendToolInform(true);
		nFunRet = CTLBD_RET_GOOD;
		nXgemStep = 0;
		break;
	}
	
	
	return nFunRet;
	
}
////


LRESULT CMainFrame::OnMainMessageXCloseEvent(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	CString strTemp;
	
	if (wParam == CTL_YES)
	{
		if (mp_xmsg_dlg != NULL && IsWindow(mp_xmsg_dlg->m_hWnd))
		{
			mp_xmsg_dlg->SetFocus();	// 대화상자를 활성화
			mp_xmsg_dlg->OnEventMsg_Text_Set();
		}
		else
		{
			mp_xmsg_dlg = new CDialog_Event_XClose;
			mp_xmsg_dlg->Create();
			mp_xmsg_dlg->ShowWindow(SW_SHOW);
			g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_ON);
			if (st_lamp.mn_buzzer_mode == CTL_YES)
				g_ioMgr.set_out_bit(st_io.o_buzzer_3, IO_ON);
		}
		
	}
	else if (wParam == CTL_NO)
	{
		for (i = 0; i < 3; i++)
		{
			st_msg.mstr_event_xmsg[i] = "";
		}
		
		if (mp_xmsg_dlg != NULL && IsWindow(mp_xmsg_dlg->m_hWnd))
		{
			mp_xmsg_dlg->ShowWindow(SW_HIDE);
			mp_xmsg_dlg->DestroyWindow();
			delete mp_xmsg_dlg;
			mp_xmsg_dlg = NULL;
		}
		
		g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_OFF);
		if (st_lamp.mn_buzzer_mode == CTL_YES)
			g_ioMgr.set_out_bit(st_io.o_buzzer_3, IO_OFF);
	}
	
	return 0;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	COPYDATASTRUCT cds;	
	
	switch( pCopyDataStruct->dwData)
	{ 
	case WM_PARTID_DATA:
		{					
			g_cs.EnterCriticalSection();
			st_copydata = new st_copy_data;			
			st_copydata->mn_mdl = 19;
			
			CString strPartID="";
			strPartID.Format("%s", g_lotMgr.GetLotAt(0).GetPartID());
			strPartID.TrimLeft(' '); strPartID.TrimRight(' ');
			strPartID.TrimLeft(); strPartID.TrimRight();			
			
			memset(st_copydata->str_partid, 0x00, sizeof(st_copydata->str_partid));
			sprintf(st_copydata->str_partid,"%s", (LPCTSTR)strPartID);
			
			cds.dwData = WM_MDL_PART_DATA;
			cds.cbData = sizeof(st_copy_data);
			cds.lpData = st_copydata;
			
			HWND hwnd = ::FindWindow(NULL,"RegModel");
			if (hwnd!= NULL)
			{
				HANDLE handle = AfxGetInstanceHandle();
				::SendMessage(hwnd, WM_COPYDATA, (WPARAM)handle, (LPARAM)&cds);
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "RegModel.exe can't find program(C:\\AMT859\\BIN\\RegModel.exe");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메폗Ao Aa텮 풹A�
					
				}
			}
			
			delete st_copydata;
			g_cs.LeaveCriticalSection();			
		}
		break;	
	}
	
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}
