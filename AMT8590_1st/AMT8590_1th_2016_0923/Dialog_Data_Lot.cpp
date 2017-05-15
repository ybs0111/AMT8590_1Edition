// Dialog_Data_Lot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Data_Lot.h"
#include "SrcPart/PartFunction.h"
#include "AMTLotManager.h"
#include "ComizoaPublic.h"
#include "NextClient.h"
#include "XGemClient.h"
#include "Run_Barcode_Feeders.h"
#include "MesEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Lot dialog


CDialog_Data_Lot::CDialog_Data_Lot(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Data_Lot::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Data_Lot)
	//}}AFX_DATA_INIT
	m_caption_main = "DATA (LOT)"; 
}


void CDialog_Data_Lot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Data_Lot)
	DDX_Control(pDX, IDC_STATIC_DISTRIBUTE_CODE, m_static_distribute_code);
	DDX_Control(pDX, IDC_BUTTON_SERVER_SEND, m_btn_send);
	DDX_Control(pDX, IDC_EDIT_DISTRIBUTE, m_edit_distribute_code);
	DDX_Control(pDX, IDC_EDIT_SERIAL, m_edit_serial);
	DDX_Control(pDX, IDC_EDIT_TURNKEY_BOX_CODE, m_edit_turnkey_box_code);
	DDX_Control(pDX, IDC_EDIT_DENSITY_CODE, m_edit_density_code);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edit_msg);
	DDX_Control(pDX, IDC_STATIC_TURNKEY, m_static_turnkey);
	DDX_Control(pDX, IDC_STATIC_SERIAl, m_static_serial);
	DDX_Control(pDX, IDC_STATIC_DENSITY, m_static_density);
	DDX_Control(pDX, IDC_STATIC_CCS, m_static_ccs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Data_Lot, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Data_Lot)
	ON_BN_CLICKED(IDC_BTN_CREATE, OnBtnCreate)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_NEW_LOT_IN, OnBtnNewLotIn)
	ON_BN_CLICKED(IDC_BTN_CONNECT_NEXT, OnBtnConnectNext)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_SEND, OnButtonServerSend)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE( WM_DRAW_DATA_LOT, OnDraw_Data_Lot)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Lot message handlers

BOOL CDialog_Data_Lot::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nSelectedLotIdx = -1;

	OnInit_Lot();
	OnInit_LotInfo();

	OnDisplay_Lot();

	st_handler.cwnd_data_lot = this;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Data_Lot::OnInit_Lot()
{
	TSpread* Grid;
	int row = 5 + 1;
	int col = 1;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_LOT_LIST );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for( int iw=0; iw<row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 20);
		Grid -> SetColWidthInPixels(1, 135 );
			
		GridFont(IDC_CUSTOM_LOT_LIST, iw + 1, 1, 15);
		GridControl(IDC_CUSTOM_LOT_LIST, STATIC, iw + 1, 1, 0);
	}

	GridColor(IDC_CUSTOM_LOT_LIST, 1, 1, BLACK, WHITE_C );
	GridData(IDC_CUSTOM_LOT_LIST, 1, 1, "LOT LIST" );
	
	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Lot::OnInit_LotInfo()
{
	TSpread* Grid;
	int row = 15;
	int col = 2;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_LOT_INFO );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	Grid -> SetColWidthInPixels(1, 150 );
	Grid -> SetColWidthInPixels(2, 150 );
	for( int iw=0; iw<row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 15);
			
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 1, 13);
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 2, 13);
		GridControl(IDC_CUSTOM_LOT_INFO, STATIC, iw + 1, 1, 0);

		GridColor( IDC_CUSTOM_LOT_INFO, iw + 1, 1, ORANGE_C, BLACK_C );
	}

	GridMerge( IDC_CUSTOM_LOT_INFO, 1, 1, 1, 2 );
	GridColor(IDC_CUSTOM_LOT_INFO, 1, 1, BLACK, WHITE_C );
	GridData(IDC_CUSTOM_LOT_INFO, 1, 1, "LOT INFO" );

	GridData( IDC_CUSTOM_LOT_INFO, 2, 1, "LOT NO" );
	GridData( IDC_CUSTOM_LOT_INFO, 3, 1, "PART NO" );
	GridData( IDC_CUSTOM_LOT_INFO, 4, 1, "NEW_LOT_IN PASS (NEXT)" );
	GridData( IDC_CUSTOM_LOT_INFO, 5, 1, "START TIME" );
	GridData( IDC_CUSTOM_LOT_INFO, 6, 1, "LAST MODULE" );
	GridData( IDC_CUSTOM_LOT_INFO, 7, 1, "Total" ); //2013,0910
//	GridData( IDC_CUSTOM_LOT_INFO, 7, 1, "BYPASS" );
//	GridData( IDC_CUSTOM_LOT_INFO, 8, 1, "COK TYPE" );
	GridData( IDC_CUSTOM_LOT_INFO, 8, 1, "BYPASS" );
	GridData( IDC_CUSTOM_LOT_INFO, 9, 1, "Density Label" );
	GridData( IDC_CUSTOM_LOT_INFO, 10, 1, "Density Capa" );
	GridData( IDC_CUSTOM_LOT_INFO, 11, 1, "Label Code" );
	GridData( IDC_CUSTOM_LOT_INFO, 12, 1, "Ribbon Code" );
	GridData( IDC_CUSTOM_LOT_INFO, 13, 1, "XCLOSE" );
	GridData( IDC_CUSTOM_LOT_INFO, 14, 1, "LABEL MDL" );
	GridData( IDC_CUSTOM_LOT_INFO, 15, 1, "TOPBTM MDL" );

	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 4, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 5, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 6, 2, 0);
//	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 7, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 8, 2, 0);
// 	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 13, 2, 0);
// 	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 14, 2, 0);
// 	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 15, 2, 0);
	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Lot::OnDisplay_Lot()
{
	GridColor( IDC_CUSTOM_LOT_LIST, 2, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 2, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 3, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 3, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 4, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 4, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 5, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 5, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 6, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 6, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 7, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 7, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 8, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 8, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 9, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 9, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 10, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 10, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 11, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 11, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 12, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 12, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 13, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 13, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 14, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 14, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 15, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 15, 1, "" );

	int nCnt = g_lotMgr.GetLotCount();
	for( int i=0; i<nCnt; i++ )
	{
		GridColor( IDC_CUSTOM_LOT_LIST, i + 2, 1, SKY_C, BLACK_C );
		GridData( IDC_CUSTOM_LOT_LIST, i + 2, 1, g_lotMgr.GetLotIDAt(i) );
	}

	if( m_nSelectedLotIdx >= 0 )
	{
		GridColor( IDC_CUSTOM_LOT_LIST, m_nSelectedLotIdx + 2, 1, YELLOW_C, BLACK_C );
	}

	m_edit_msg.SetWindowText("");
	m_edit_serial.SetWindowText("");
	m_edit_turnkey_box_code.SetWindowText("");
	m_edit_density_code.SetWindowText("");
	
	m_static_ccs.EnableWindow( FALSE );
	m_static_density.EnableWindow( FALSE );
	m_static_serial.EnableWindow( FALSE );
	m_static_turnkey.EnableWindow( FALSE );
	m_static_distribute_code.EnableWindow( FALSE );
	
	m_edit_density_code.EnableWindow( FALSE );
	m_edit_msg.EnableWindow( FALSE );
	m_edit_serial.EnableWindow( FALSE );
	m_edit_turnkey_box_code.EnableWindow( FALSE );
	m_btn_send.EnableWindow( FALSE );
	
	if( st_basic.mn_rework_ccs_mode == CTL_YES)
	{
		m_static_ccs.EnableWindow( TRUE );
		m_static_density.EnableWindow( TRUE );
		m_static_serial.EnableWindow( TRUE );
		m_static_turnkey.EnableWindow( TRUE );
		m_static_distribute_code.EnableWindow( TRUE );
		
		m_edit_density_code.EnableWindow( TRUE );
		m_edit_msg.EnableWindow( TRUE );
		m_edit_serial.EnableWindow( TRUE );
		m_edit_turnkey_box_code.EnableWindow( TRUE );
		m_btn_send.EnableWindow( TRUE );
	}
}

void CDialog_Data_Lot::OnDisplay_LotInfo()
{
	if( m_nSelectedLotIdx < 0 ||
		m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
	{
		GridData( IDC_CUSTOM_LOT_INFO, 2, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 3, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 4, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 5, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 6, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 7, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 8, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 9, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 10, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 11, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 12, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 13, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 14, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 15, 2, "" );
		return;
	}

	GridData( IDC_CUSTOM_LOT_INFO, 2, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetLotID() );
	GridData( IDC_CUSTOM_LOT_INFO, 3, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetPartID() );
	GridData( IDC_CUSTOM_LOT_INFO, 4, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrNewLotInPass() );
	GridData( IDC_CUSTOM_LOT_INFO, 5, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeStart() );
	GridData( IDC_CUSTOM_LOT_INFO, 6, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrLastModule() );
	GridData( IDC_CUSTOM_LOT_INFO, 7, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrTotalModule() );
	//	GridData( IDC_CUSTOM_LOT_INFO, 7, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrBypass() );
	GridData( IDC_CUSTOM_LOT_INFO, 8, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrBypass() );
	GridData( IDC_CUSTOM_LOT_INFO, 9, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetDenLabelCode() );
	GridData( IDC_CUSTOM_LOT_INFO, 10, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetDenLabelCapa() );
	GridData( IDC_CUSTOM_LOT_INFO, 11, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetLabelCode() );
	GridData( IDC_CUSTOM_LOT_INFO, 12, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetRibbonCode() );
	GridData( IDC_CUSTOM_LOT_INFO, 13, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetXClose() );
	GridData( IDC_CUSTOM_LOT_INFO, 14, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetLabelVisionModel() );
	GridData( IDC_CUSTOM_LOT_INFO, 15, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTBtmVisionModel() );


	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
}

void CDialog_Data_Lot::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
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

void CDialog_Data_Lot::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CDialog_Data_Lot::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Lot::GridFont(UINT nID, int row, int col, int size)
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

void CDialog_Data_Lot::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Lot::OnCellClick( WPARAM wParam, LPARAM lParam )
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	if( wParam == IDC_CUSTOM_LOT_LIST )
	{
		int nCnt = g_lotMgr.GetLotCount();
		if( nCnt > lpcc->Row - 2 )
		{
			m_nSelectedLotIdx = lpcc->Row - 2;
			OnDisplay_Lot();
			OnDisplay_LotInfo();
		}
		else
		{
			m_nSelectedLotIdx = -1;
		}
	}

	else if( wParam == IDC_CUSTOM_LOT_INFO )
	{
		if( lpcc->Col == 1 )	return;

		switch( lpcc->Row )
		{
		case 4:
		case 6:
		case 8:
		case 13:
			{
				CString strData;
				TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
				
				char strGet[16];
				Grid->GetData( lpcc->Col, lpcc->Row, strGet );

				strData = strGet;
				if( strData == "YES" )
				{
					GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, "NO" );
				}
				else
				{
					GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, "YES" );
				}
				
				if (Grid != NULL)
				{	
					Grid = NULL;
					delete Grid;
				}
			}
			
				
		
		
		}
	}
}


void CDialog_Data_Lot::OnBtnCreate() 
{
	// TODO: Add your control notification handler code here
	if( COMI.mn_run_status != dSTOP )
		return;

	g_lotMgr.AddLot("", "");
	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);

	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, "[LOT] LOT Create.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
}

LRESULT CDialog_Data_Lot::OnDraw_Data_Lot( WPARAM wParam,LPARAM lParam )
{
	m_nSelectedLotIdx = -1;

	OnDisplay_Lot();
	OnDisplay_LotInfo();

	return 0;
}

void CDialog_Data_Lot::OnDestroy() 
{
	StandardDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(TMR_REWORK_CCS);//2016.1207
	st_handler.cwnd_data_lot = NULL;
}

void CDialog_Data_Lot::OnBtnDelete() 
{
	if( COMI.mn_run_status != dSTOP )
	{
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[LOT] Machine is not stop");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}

	if( m_nSelectedLotIdx < 0 ||
		m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
		return;
	
	if(g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrLastModule() != "YES")
	{
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "[LOT] LOT Can't Delete. Last Module is not YES");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}

	//2015.0317
	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		CString strLog;
		int nSec = (GetCurrentTime() - g_lotMgr.GetLotAt(0).GetLotTime())/1000;
		if( nSec < 1) nSec = 1;
		int nUPH = g_lotMgr.GetLotAt(0).GetInputCnt(CUM) * 3600 / nSec;
		strLog.Format("[Lotend : %s] Total time; %d sec. Input:%d. Good:%d. Fail:%d. UPH:%d ea.\r\n", g_lotMgr.GetLotIDAt(0), nSec,
			g_lotMgr.GetLotAt(0).GetInputCnt(CUM), g_lotMgr.GetLotAt(0).GetPassCnt(CUM), g_lotMgr.GetLotAt(0).GetFailCnt(PRIME), nUPH );
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 .출력 요청
		Func.On_LogFile_Add(LOG_TOTAL, strLog);
		Func.On_LogFile_Add(LOG_UPH, strLog);

		CString strTimeEnd;
		SYSTEMTIME  time;
		::GetLocalTime(&time);

		strTimeEnd.Format( "%04d/%02d/%02d %02d:%02d:%02d.%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 
			time.wSecond, time.wMilliseconds );
		
		strLog.Format("%s, %s, %s, %d, %d, %d, UPH:%d ea", g_lotMgr.GetLotIDAt(0), g_lotMgr.GetLotAt(0).GetTimeStart(),
			strTimeEnd,	g_lotMgr.GetLotAt(0).GetInputCnt(CUM), g_lotMgr.GetLotAt(0).GetPassCnt(CUM), g_lotMgr.GetLotAt(0).GetFailCnt(PRIME), nUPH );
		Func.On_LogFile_Add(LOG_PRODUCT, strLog, 1);		
	}

	if( st_basic.mn_mode_xgem == CTL_YES )
	{
		g_xgem_client.SetLotEndStatus(g_lotMgr.GetLotAt(0).GetLotID(), g_lotMgr.GetLotAt(0).GetPartID());
	}

	g_lotMgr.DeleteLot( m_nSelectedLotIdx );
	g_Run_Barcode_Feeders.m_total_vis_error[0] = 0;
	st_handler.mn_chk_lotend = CTL_NO;

	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);

	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, "[LOT] LOT Delete.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}

	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);

}

void CDialog_Data_Lot::OnBtnUp() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx <= 0 )
		return;

	ALot& prev = g_lotMgr.GetLotAt(m_nSelectedLotIdx - 1);
	ALot& current = g_lotMgr.GetLotAt(m_nSelectedLotIdx);
	ALot tmp = current;
	current = prev;
	prev = tmp;

	m_nSelectedLotIdx -= 1;
	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnDown() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx >= g_lotMgr.GetLotCount() - 1 )
		return;

	ALot& next = g_lotMgr.GetLotAt(m_nSelectedLotIdx + 1);
	ALot& current = g_lotMgr.GetLotAt(m_nSelectedLotIdx);
	ALot tmp = current;
	current = next;
	next = tmp;

	m_nSelectedLotIdx += 1;
	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnUpdate() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx < 0 ||
		m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
		return;

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
				
	char strGet[256];
	Grid->GetData( 2, 2, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetLotID( strGet );
	Grid->GetData( 2, 3, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetPartID( strGet );
	Grid->GetData( 2, 4, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetNewLotInPass( (strcmp( strGet, "YES" ) == 0) );
	Grid->GetData( 2, 6, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetLastModule( strGet );
	Grid->GetData( 2, 7, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetTotalModule( strGet );
	Grid->GetData( 2, 8, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetByPass( strGet );
	Grid->GetData( 2, 9, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetDenLabelCode(strGet);
	Grid->GetData( 2, 10, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetDenLabelCapa(strGet);
	Grid->GetData( 2, 11, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetDisLabelCode(strGet);
	Grid->GetData( 2, 12, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetRibbonCode(strGet);
	Grid->GetData( 2, 13, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetXClose(strGet);
 	Grid->GetData( 2, 14, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetLabelVisionModelUpdate(strGet);
 	Grid->GetData( 2, 15, strGet );  g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetTBtmVisionModelUpdate(strGet);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}

	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).CalcCokType();

	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnNewLotIn() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		if( m_nSelectedLotIdx >= 0 )
		{
			g_next_client.OnMove_BufferIn();
		}
	}	
}


void CDialog_Data_Lot::OnBtnConnectNext() 
{
	::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
	::Sleep(1000);
	if (st_client[CLS_NEXT].n_connect == CTL_YES)
	{
	}
	else
	{			
		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CONNECT, CLS_NEXT);
	}	
}

void CDialog_Data_Lot::OnButtonServerSend() 
{
	CString strDistribute = "", strSerail = "", strMsg = "", strTurnkeyBoxCode = "", strDensityCode = "";
	
	((CEdit*)GetDlgItem(IDC_EDIT_DISTRIBUTE))->GetWindowText(strDistribute);
	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->GetWindowText(strSerail);
	((CEdit*)GetDlgItem(IDC_EDIT_MSG))->GetWindowText(strMsg);
	((CEdit*)GetDlgItem(IDC_EDIT_DENSITY_CODE))->GetWindowText(strDensityCode);
	((CEdit*)GetDlgItem(IDC_EDIT_TURNKEY_BOX_CODE))->GetWindowText(strTurnkeyBoxCode);
	
	strSerail.TrimLeft(' '); strSerail.TrimRight(' ');
	strSerail.TrimLeft(); strSerail.TrimRight();
	strMsg.TrimLeft(' '); strMsg.TrimRight(' ');
	strMsg.TrimLeft(); strMsg.TrimRight();
	
	strDensityCode.TrimLeft(' '); strDensityCode.TrimRight(' ');
	strDensityCode.TrimLeft(); strDensityCode.TrimRight();
	strTurnkeyBoxCode.TrimLeft(' '); strTurnkeyBoxCode.TrimRight(' ');
	strTurnkeyBoxCode.TrimLeft(); strTurnkeyBoxCode.TrimRight();

// 	if( strDensityCode.GetLength() > 11 )
// 		strDensityCode = strDensityCode.Left(11);
// 	((CEdit*)GetDlgItem(IDC_EDIT_DENSITY_CODE))->SetWindowText("");
// 	((CEdit*)GetDlgItem(IDC_EDIT_DENSITY_CODE))->SetWindowText(strDensityCode);


	CString strBcr = strSerail.Left( strSerail.GetLength() -1 );

	if( strDistribute != strBcr )
	{
		AfxMessageBox("Product serial and distribute serial are not same.");
		return;
	}
	
	if( strSerail == "" || strSerail.GetLength() < 14)
	{
		
		AfxMessageBox("There is no serial.");
		return;
	}
	if( strDensityCode == "" || strDensityCode.GetLength() < 10)
	{
		AfxMessageBox("There is no DensityCode.");
		return;
	}
	if( strTurnkeyBoxCode == "" || strTurnkeyBoxCode.GetLength() < 10)
	{
		AfxMessageBox("There is no TurnkeyBoxCode.");
		return;
	}
	
	st_mes_data.srt_SERIAL.Format("%s", strSerail);
	st_mes_data.srt_DEN_LABEL_MAT_CODE.Format("%s", strDensityCode);
	st_mes_data.srt_TURMKEY_BOX_CODE.Format("%s", strTurnkeyBoxCode);
	m_rework_step = 0;
	m_nRetryCnt = 0;
	
	SetTimer( TMR_REWORK_CCS, 100, NULL );
	
	// 	sBody.Format("FUNCTION=REWORK_LABEL_CCS EQP_ID=%s SERIAL=%s DENSITY_MATL_CODE=%s TURNKEYBOX_MATL_CODE=%s OPER_ID=%s ",
	// 		st_lamp.mstr_equip_id, st_mes_data.srt_SERIAL, st_mes_data.srt_DEN_LABEL_MAT_CODE, st_mes_data.srt_TURMKEY_BOX_CODE,"AUTO");
	
}

void CDialog_Data_Lot::OnTimer(UINT nIDEvent) 
{
	int nRet = CTLBD_RET_PROCEED;
	if (nIDEvent == TMR_REWORK_CCS)
	{
		
		nRet = OnServerReworkCCS();
		if (nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_REWORK_CCS);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_REWORK_CCS);
		}
		
	}	
	StandardDialog::OnTimer(nIDEvent);
}

int CDialog_Data_Lot::OnServerReworkCCS()
{
	int nRet = CTLBD_RET_PROCEED;
	CString strAlarm;

	switch( m_rework_step )
	{
	case 0:
		m_rework_step = 1000;
		break;

	case 1000:
		evMes.OnPutToQueue(MES_REWORK_LABEL_CCS_INFO);		
		(st_client[0].stComm[MES_REWORK_LABEL_CCS_INFO]).nResponse = CTL_PROCEED;
		m_dwReworkCCS[0] = GetCurrentTime();
		m_rework_step = 2000;
		break;

	case 2000:
		nRet = evMes.OnGet_MesWork();
		if (nRet == REPLY_CORRECT_)
		{
			if ((st_client[0].stComm[MES_BRAND_DENSITY_LABEL_CCS]).nResponse == CTL_GOOD )
			{
				m_nRetryCnt = 0;
				((CEdit*)GetDlgItem(IDC_EDIT_MSG))->SetWindowText("");
				m_edit_msg.SetWindowText( "PASS" );
				g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddReworkCnt(PRIME);

				((CEdit*)GetDlgItem(IDC_EDIT_DISTRIBUTE))->SetWindowText("");
				((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->SetWindowText("");
				((CEdit*)GetDlgItem(IDC_EDIT_MSG))->SetWindowText("");
				((CEdit*)GetDlgItem(IDC_EDIT_DENSITY_CODE))->SetWindowText("");
				((CEdit*)GetDlgItem(IDC_EDIT_TURNKEY_BOX_CODE))->SetWindowText("");

				nRet = CTLBD_RET_GOOD;
				return nRet;
			}
			else
			{
				m_dwReworkCCS[1] = GetCurrentTime();
				m_dwReworkCCS[2] = m_dwReworkCCS[1] - m_dwReworkCCS[0];
				if (m_dwReworkCCS[2] < 0)
				{
					m_dwReworkCCS[0] = GetCurrentTime();
					break;
				}
				
				if (m_dwReworkCCS[2] > 1000)
				{
					m_nRetryCnt++;
					if (m_nRetryCnt > 5)
					{
						m_nRetryCnt = 5;
					}
					m_rework_step = 1000;
					if (m_nRetryCnt >= 5)
					{
						m_nRetryCnt = 0;
						m_rework_step = 0;						
						// 900101 1 0 "MES_NOT_RESPONSE_ERR"
						m_edit_msg.SetWindowText("MES_NOT_RESPONSE_ERR");
						nRet = CTLBD_RET_ERROR;
						return nRet;
					}
				}				
			}
		}
		else if (nRet == REPLY_ERROR_)
		{
			if ((st_client[0].stComm[MES_BRAND_DENSITY_LABEL_CCS]).nResponse == CTL_ERROR)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt > 5)
				{
					m_nRetryCnt = 5;
				}
				m_rework_step = 1000;
				if (m_nRetryCnt >= 5)
				{
					m_nRetryCnt = 0;
					m_rework_step = 0;
					// 900101 1 0 "MES_NOT_RESPONSE_ERR"
					strAlarm.Format("%s", alarm.mstr_svr_alarm );
					m_edit_msg.SetWindowText( strAlarm );
					nRet = CTLBD_RET_ERROR;
					return nRet;
				}
			}
			else
			{
				m_dwReworkCCS[1] = GetCurrentTime();
				m_dwReworkCCS[2] = m_dwReworkCCS[1] - m_dwReworkCCS[0];
				if (m_dwReworkCCS[2] < 0)
				{
					m_dwReworkCCS[0] = GetCurrentTime();
					break;
				}
				
				if (m_dwReworkCCS[2] > (1000))
				{
					m_nRetryCnt++;
					if (m_nRetryCnt > 5)
					{
						m_nRetryCnt = 5;
					}
					m_rework_step = 1000;
					if (m_nRetryCnt >= 5)
					{
						m_nRetryCnt = 0;
						m_rework_step = 0;
						
						// 900101 1 0 "MES_NOT_RESPONSE_ERR"
						m_edit_msg.SetWindowText("MES_NOT_RESPONSE_ERR");
						nRet = CTLBD_RET_ERROR;
						return nRet;
					}
				}
			}
		}
		else
		{
			m_dwReworkCCS[1] = GetCurrentTime();
			m_dwReworkCCS[2] = m_dwReworkCCS[1] - m_dwReworkCCS[0];
			if (m_dwReworkCCS[2] < 0)
			{
				m_dwReworkCCS[0] = GetCurrentTime();
				break;
			}
			
			if (m_dwReworkCCS[2] > (1000))
			{
				m_nRetryCnt++;	// 재시도 횟수 증가
				if (m_nRetryCnt > 5)
				{
					m_nRetryCnt = 5;
				}
				m_rework_step = 0;	// 통신 재시도 스텝으로 점프
				
				if (m_nRetryCnt >= 5)
				{
					m_nRetryCnt = 0;
					m_rework_step = 0;
					// 900101 1 0 "MES_NOT_RESPONSE_ERR"
					m_edit_msg.SetWindowText("MES_NOT_RESPONSE_ERR");
					nRet = CTLBD_RET_ERROR;
					return nRet;
				}
			}
		}
		break;

	}

	return CTLBD_RET_PROCEED;
}

