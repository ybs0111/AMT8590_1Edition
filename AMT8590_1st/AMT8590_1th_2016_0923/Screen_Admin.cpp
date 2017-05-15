// Screen_Admin.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Admin.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin

IMPLEMENT_DYNCREATE(CScreen_Admin, CFormView)

CScreen_Admin::CScreen_Admin()
	: CFormView(CScreen_Admin::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Admin)
	//}}AFX_DATA_INIT
}

CScreen_Admin::~CScreen_Admin()
{
}

void CScreen_Admin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Admin)
	DDX_Control(pDX, IDC_CHK_DRYRUN_DENSITY, m_chk_dryrun_density);
	DDX_Control(pDX, IDC_CHK_WITHOUT_BTM_8580S, m_chk_without_btm_8580s);
	DDX_Control(pDX, IDC_CHK_ONLY_DENSITY, m_chk_only_density);
	DDX_Control(pDX, IDC_CHK_ONLY_BCR1, m_chk_only_bcr1);
	DDX_Control(pDX, IDC_CHK_WITHOUT_TOP_VISION, m_chk_without_top_vision);
	DDX_Control(pDX, IDC_CHK_WITHOUT_SMEMA_BARCODE_READ, m_chk_without_smema_bcr_read);
	DDX_Control(pDX, IDC_CHK_WITHOUT_NETWORK, m_chk_without_network);
	DDX_Control(pDX, IDC_CHK_WITHOUT_IDBUFFER, m_chk_without_idbuffer);
	DDX_Control(pDX, IDC_CHK_WITHOUT_DENSITY, m_chk_without_density_rbt);
	DDX_Control(pDX, IDC_CHK_WITHOUT_BCR2, m_chk_without_bcr2_rbt);
	DDX_Control(pDX, IDC_CHK_WITHOUT_BCR1, m_chk_without_bcr1_rbt);
	DDX_Control(pDX, IDC_CHK_WITHOUT_SMEMA_8580MC, m_chk_without_smema_8580s);
	DDX_Control(pDX, IDC_CHK_WITH_BUFFER, m_chk_virtual_buffer);
	DDX_Control(pDX, IDC_CHK_VIRTUAL_SUPPLY, m_chk_virtual_supply);
	DDX_Control(pDX, IDC_CHK_VIRTUAL, m_chk_Virtual);
	////
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Admin, CFormView)
	//{{AFX_MSG_MAP(CScreen_Admin)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_VIRTUAL_SUPPLY, OnChkVirtualSupply)
	ON_BN_CLICKED(IDC_CHK_WITH_BUFFER, OnChkWithBuffer)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_SMEMA_BARCODE_READ, OnChkWithoutSmemaBarcodeRead)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_SMEMA_8580MC, OnChkWithoutSmema8580mc)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_NETWORK, OnChkWithoutNetwork)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_IDBUFFER, OnChkWithoutIdbuffer)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_DENSITY, OnChkWithoutDensity)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_BCR1, OnChkWithoutBcr1)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_BCR2, OnChkWithoutBcr2)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_TOP_VISION, OnChkWithoutTopVision)
	ON_BN_CLICKED(IDC_CHK_ONLY_BCR1, OnChkOnlyBcr1)
	ON_BN_CLICKED(IDC_CHK_ONLY_DENSITY, OnChkOnlyDensity)
	ON_BN_CLICKED(IDC_CHK_WITHOUT_BTM_8580S, OnChkWithoutBtm8580s)
	ON_BN_CLICKED(IDC_CHK_DRYRUN_DENSITY, OnChkDryrunDensity)
	ON_BN_CLICKED(IDC_CHK_VIRTUAL, OnChkVirtual)
	ON_BN_CLICKED(IDC_BTN_FILE_EDITOR, OnBtnFileEditor)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin diagnostics

#ifdef _DEBUG
void CScreen_Admin::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Admin::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin message handlers

void CScreen_Admin::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	OnAdmin_Data_Set();
}

void CScreen_Admin::OnDestroy() 
{
	CFormView::OnDestroy();
}

void CScreen_Admin::OnTimer(UINT nIDEvent) 
{
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Admin::OnAdmin_Data_Set()
{
	// 	st_handler.n_without_smema_8580_mc = CTL_NO;
	// 	st_handler.n_without_barcode_read = CTL_YES;
	// 	st_handler.n_without_network = CTL_NO;
	// 	st_handler.n_without_idbuffer = CTL_NO;
	// 	
	// 	st_handler.n_without_density_label_rbt = CTL_NO;
	// 	st_handler.n_without_bcr1_rbt = CTL_YES;
	// 	st_handler.n_without_bcr2_rbt = CTL_YES;
	// 	st_handler.n_without_top_vision_rbt = CTL_YES;

	m_chk_dryrun_density.SetCheck(st_handler.n_dryrun_density);
	m_chk_only_bcr1.SetCheck(st_handler.n_only_work_bcr1);
	m_chk_only_density.SetCheck(st_handler.n_only_work_density);
	m_chk_without_btm_8580s.SetCheck(st_handler.n_without_smema_btm_8580s);
	
	m_chk_without_smema_bcr_read.SetCheck(st_handler.n_without_barcode_read);
	m_chk_without_smema_8580s.SetCheck(st_handler.n_without_smema_8580_mc);
	m_chk_without_network.SetCheck(st_handler.n_without_network);
	m_chk_without_idbuffer.SetCheck(st_handler.n_without_idbuffer);
	m_chk_without_density_rbt.SetCheck(st_handler.n_without_density_label_rbt);
	m_chk_without_bcr1_rbt.SetCheck(st_handler.n_without_bcr1_rbt);
	m_chk_without_bcr2_rbt.SetCheck(st_handler.n_without_bcr2_rbt);
	m_chk_without_top_vision.SetCheck(st_handler.n_without_top_vision_rbt);
}

void CScreen_Admin::OnBtnFileEditor() 
{
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 8, 2);
}

void CScreen_Admin::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
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

void CScreen_Admin::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CScreen_Admin::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

CString CScreen_Admin::GetGridData( UINT nID, int row, int col )
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

void CScreen_Admin::GridFont(UINT nID, int row, int col, int size)
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

void CScreen_Admin::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Admin::OnChkVirtual() 
{
	CString mstr_temp="";
	
	st_handler.mn_virtual_mode = !st_handler.mn_virtual_mode;
	
	m_chk_Virtual.SetCheck(st_handler.mn_virtual_mode);
	
	mstr_temp.Format("%d", st_handler.mn_virtual_mode);
	:: WritePrivateProfileString("BasicData", "Virtual_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);	
}

void CScreen_Admin::OnChkVirtualSupply() 
{
	CString mstr_temp="";
	
	st_handler.mn_virtual_supply = !st_handler.mn_virtual_supply;
	
	m_chk_virtual_supply.SetCheck(st_handler.mn_virtual_supply);
	
	mstr_temp.Format("%d", st_handler.mn_virtual_supply);
	:: WritePrivateProfileString("BasicData", "Virtual_Supply", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
}



void CScreen_Admin::OnChkWithBuffer() 
{
	CString mstr_temp="";
	
	st_handler.mn_virtual_idbuffer = !st_handler.mn_virtual_idbuffer;
	
	m_chk_virtual_buffer.SetCheck(st_handler.mn_virtual_idbuffer);
	
	mstr_temp.Format("%d", st_handler.mn_virtual_idbuffer);
	:: WritePrivateProfileString("BasicData", "Virtual_IDBuffer", LPCTSTR(mstr_temp), st_path.mstr_basic);	
}

void CScreen_Admin::OnChkWithoutSmemaBarcodeRead() 
{
	CString mstr_temp="";
	
	st_handler.n_without_barcode_read = !st_handler.n_without_barcode_read;
	
	m_chk_without_smema_bcr_read.SetCheck(st_handler.n_without_barcode_read);
	
	mstr_temp.Format("%d", st_handler.n_without_barcode_read);
	:: WritePrivateProfileString("BasicData", "WITHOUT_BARCODE_READ", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutSmema8580mc() 
{
	CString mstr_temp="";
	
	st_handler.n_without_smema_8580_mc = !st_handler.n_without_smema_8580_mc;
	
	m_chk_without_smema_8580s.SetCheck(st_handler.n_without_smema_8580_mc);
	
	mstr_temp.Format("%d", st_handler.n_without_smema_8580_mc);
	:: WritePrivateProfileString("BasicData", "WITHOUT_8580S", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutNetwork() 
{
	CString mstr_temp="";
	
	st_handler.n_without_network = !st_handler.n_without_network;
	
	m_chk_without_network.SetCheck(st_handler.n_without_network);
	
	mstr_temp.Format("%d", st_handler.n_without_network);
	:: WritePrivateProfileString("BasicData", "WITHOUT_NETWORK", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutIdbuffer() 
{
	CString mstr_temp="";
	
	st_handler.n_without_idbuffer = !st_handler.n_without_idbuffer;
	
	m_chk_without_idbuffer.SetCheck(st_handler.n_without_idbuffer);
	
	mstr_temp.Format("%d", st_handler.n_without_idbuffer);
	:: WritePrivateProfileString("BasicData", "WITHOUT_IDBUFFER", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutDensity() 
{
	CString mstr_temp="";
	
	st_handler.n_without_density_label_rbt = !st_handler.n_without_density_label_rbt;
	
	m_chk_without_density_rbt.SetCheck(st_handler.n_without_density_label_rbt);
	
	mstr_temp.Format("%d", st_handler.n_without_density_label_rbt);
	:: WritePrivateProfileString("BasicData", "WITHOUT_DENSITY_RBT", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutBcr1() 
{
	CString mstr_temp="";
	
	st_handler.n_without_bcr1_rbt = !st_handler.n_without_bcr1_rbt;
	
	m_chk_without_bcr1_rbt.SetCheck(st_handler.n_without_bcr1_rbt);
	
	mstr_temp.Format("%d", st_handler.n_without_bcr1_rbt);
	:: WritePrivateProfileString("BasicData", "WITHOUT_BCR1_RBT", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutBcr2() 
{
	CString mstr_temp="";
	
	st_handler.n_without_bcr2_rbt = !st_handler.n_without_bcr2_rbt;
	
	m_chk_without_bcr2_rbt.SetCheck(st_handler.n_without_bcr2_rbt);
	
	mstr_temp.Format("%d", st_handler.n_without_bcr2_rbt);
	:: WritePrivateProfileString("BasicData", "WITHOUT_BCR2_RBT", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutTopVision() 
{
	CString mstr_temp;
	
	st_handler.n_without_top_vision_rbt = !st_handler.n_without_top_vision_rbt;
	
	m_chk_without_top_vision.SetCheck(st_handler.n_without_top_vision_rbt);
	
	mstr_temp.Format("%d", st_handler.n_without_top_vision_rbt);
	:: WritePrivateProfileString("BasicData", "WITHOUT_TOP_VISION", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkOnlyBcr1() 
{
	CString mstr_temp="";
	
	st_handler.n_only_work_bcr1 = !st_handler.n_only_work_bcr1;
	
	m_chk_only_bcr1.SetCheck(st_handler.n_only_work_bcr1);
	
	mstr_temp.Format("%d", st_handler.n_only_work_bcr1);
	:: WritePrivateProfileString("BasicData", "ONLY_BCR1", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkOnlyDensity() 
{
	CString mstr_temp="";
	
	st_handler.n_only_work_density = !st_handler.n_only_work_density;
	
	m_chk_only_density.SetCheck(st_handler.n_only_work_density);
	
	mstr_temp.Format("%d", st_handler.n_only_work_density);
	:: WritePrivateProfileString("BasicData", "ONLY_DENSITY", LPCTSTR(mstr_temp), st_path.mstr_basic);	
	
}

void CScreen_Admin::OnChkWithoutBtm8580s() 
{
	
	CString mstr_temp="";

	st_handler.n_without_smema_btm_8580s = !st_handler.n_without_smema_btm_8580s;

	m_chk_without_btm_8580s.SetCheck(st_handler.n_without_smema_btm_8580s);

	mstr_temp.Format("%d", st_handler.n_without_smema_btm_8580s);
	:: WritePrivateProfileString("BasicData", "WITHOUT_BTM_8580S", LPCTSTR(mstr_temp), st_path.mstr_basic);	
}

void CScreen_Admin::OnChkDryrunDensity() 
{
	CString mstr_temp="";
	
	st_handler.n_dryrun_density = !st_handler.n_dryrun_density;
	
	m_chk_dryrun_density.SetCheck(st_handler.n_dryrun_density);
	
	mstr_temp.Format("%d", st_handler.n_dryrun_density);
	:: WritePrivateProfileString("BasicData", "DRYRUN_DENSITY", LPCTSTR(mstr_temp), st_path.mstr_basic);		
}

