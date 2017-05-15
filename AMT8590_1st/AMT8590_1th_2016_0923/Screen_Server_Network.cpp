// Screen_Server_Network.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Server_Network.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Server_Network

IMPLEMENT_DYNCREATE(CScreen_Server_Network, CFormView)

CScreen_Server_Network::CScreen_Server_Network()
	: CFormView(CScreen_Server_Network::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Server_Network)
	m_LOT_ID = _T("");
	m_GATE = _T("");
	m_RETRY = _T("");
	m_RETRY_CNT = _T("");
	m_SERIAL = _T("");
	m_PPID = _T("");
	m_WWN = _T("");
	m_C_SERIAL = _T("");
	m_PSID = _T("");
	m_BOAT_ID = _T("");
	m_MODIF_MODE = _T("");
	m_DTBT_LABEL_CODE = _T("");
	m_DTBT_RIBBON_CODE = _T("");
	m_MODE = _T("");
	m_WEIGHT = _T("");
	m_DTBT_LABEL_SERIAL = _T("");
	m_S_BOX_CNT = _T("");
	m_S_BOX_LABEL_SERIAL = _T("");
	m_L_BOX_ID = _T("");
	m_MO_CLOSE_TIME = _T("");
	m_S_BOX_ID = _T("");
	m_TOOLID = _T("");
	m_DEN_LABEL_MAT_CODE = _T("");
	m_DTBT_MAT_CODE = _T("");
	m_RIBBON_MAT_CODE = _T("");
	m_SSD_CNT = _T("");
	m_PROD_SRLNO = _T("");
	m_PIECE_PROD_ID = _T("");
	m_WWN_SRLNO = _T("");
	m_CUST_SRLNO = _T("");
	m_PHYS_SECRTY_ID = _T("");
	//}}AFX_DATA_INIT
}

CScreen_Server_Network::~CScreen_Server_Network()
{
}

void CScreen_Server_Network::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Server_Network)
	DDX_Control(pDX, IDC_BTN_KEYIN_MENUAL, m_btn_keyin_menual);
	DDX_Text(pDX, IDC_EDIT1, m_LOT_ID);
	DDX_Text(pDX, IDC_EDIT3, m_GATE);
	DDX_Text(pDX, IDC_EDIT4, m_RETRY);
	DDX_Text(pDX, IDC_EDIT5, m_RETRY_CNT);
	DDX_Text(pDX, IDC_EDIT6, m_SERIAL);
	DDX_Text(pDX, IDC_EDIT7, m_PPID);
	DDX_Text(pDX, IDC_EDIT8, m_WWN);
	DDX_Text(pDX, IDC_EDIT9, m_C_SERIAL);
	DDX_Text(pDX, IDC_EDIT10, m_PSID);
	DDX_Text(pDX, IDC_EDIT11, m_SSD_CNT);
	DDX_Text(pDX, IDC_EDIT12, m_PROD_SRLNO);
	DDX_Text(pDX, IDC_EDIT13, m_PIECE_PROD_ID);
	DDX_Text(pDX, IDC_EDIT14, m_WWN_SRLNO);
	DDX_Text(pDX, IDC_EDIT15, m_CUST_SRLNO);
	DDX_Text(pDX, IDC_EDIT16, m_PHYS_SECRTY_ID);
	DDX_Text(pDX, IDC_EDIT17, m_BOAT_ID);
	DDX_Text(pDX, IDC_EDIT19, m_MODIF_MODE);
	DDX_Text(pDX, IDC_EDIT20, m_DTBT_LABEL_CODE);
	DDX_Text(pDX, IDC_EDIT21, m_DTBT_RIBBON_CODE);
	DDX_Text(pDX, IDC_EDIT22, m_MODE);
	DDX_Text(pDX, IDC_EDIT23, m_WEIGHT);
	DDX_Text(pDX, IDC_EDIT24, m_DTBT_LABEL_SERIAL);
	DDX_Text(pDX, IDC_EDIT25, m_S_BOX_CNT);
	DDX_Text(pDX, IDC_EDIT26, m_S_BOX_LABEL_SERIAL);
	DDX_Text(pDX, IDC_EDIT27, m_L_BOX_ID);
	DDX_Text(pDX, IDC_EDIT28, m_MO_CLOSE_TIME);
	DDX_Text(pDX, IDC_EDIT29, m_S_BOX_ID);
	DDX_Text(pDX, IDC_EDIT30, m_TOOLID);
	DDX_Text(pDX, IDC_EDIT31, m_DEN_LABEL_MAT_CODE);
	DDX_Text(pDX, IDC_EDIT32, m_DTBT_MAT_CODE);
	DDX_Text(pDX, IDC_EDIT33, m_RIBBON_MAT_CODE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Server_Network, CFormView)
	//{{AFX_MSG_MAP(CScreen_Server_Network)
	ON_BN_CLICKED(IDC_BUTTON1, OnLOT_CLOSE)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_KEYIN_MENUAL, OnBtnKeyinMenual)
	ON_BN_CLICKED(IDC_BUTTON2, OnPEBAG_CHECK)
	ON_BN_CLICKED(IDC_BUTTON4, OnBRAND_PEBAG_CHECK)
	ON_BN_CLICKED(IDC_BUTTON5, OnBOAT_ID_MODIF)
	ON_BN_CLICKED(IDC_BUTTON6, OnBRAND_DTBT_LABEL_ISSUE)
	ON_BN_CLICKED(IDC_BUTTON7, OnBRAND_WEIGHT_SBOX_ISSUE)
	ON_BN_CLICKED(IDC_BUTTON8, OnLBOX_ISSUE)
	ON_BN_CLICKED(IDC_BUTTON9, OnAUTO_SHIP)
	ON_BN_CLICKED(IDC_BUTTON17, OnPROD_SERIAL_INFO)
	ON_BN_CLICKED(IDC_BUTTON20, OnBOAT_ID_INFO)
	ON_BN_CLICKED(IDC_BUTTON21, OnSBOX_INFO)
	ON_BN_CLICKED(IDC_BUTTON22, OnPACKING_MASTER_INFO)
	ON_BN_CLICKED(IDC_BUTTON23, OnPK_XCLOSE)
	ON_BN_CLICKED(IDC_BUTTON24, OnTOOL_CHANGE)
	ON_BN_CLICKED(IDC_BUTTON25, OnTOOL_CHECK)
	ON_BN_CLICKED(IDC_BUTTON26, OnBRAND_DENSITY_LABEL_CCS)
	ON_BN_CLICKED(IDC_BUTTON27, OnBRAND_DTBT_LABEL_CCS)
	ON_BN_CLICKED(IDC_BUTTON28, OnPEBAG_COMPLETE)
	ON_BN_CLICKED(IDC_BUTTON10, OnSRLADD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Server_Network diagnostics

#ifdef _DEBUG
void CScreen_Server_Network::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Server_Network::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Server_Network message handlers

void CScreen_Server_Network::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

BOOL CScreen_Server_Network::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::DestroyWindow();
}

void CScreen_Server_Network::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CScreen_Server_Network::OnBtnKeyinMenual() 
{
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 53, 0);	// KeyIn-Manual 대화상자 SHOW
}

void CScreen_Server_Network::OnLOT_CLOSE() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_GATE == "")
	{
		AfxMessageBox("GATE Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}

	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_GATE = m_GATE;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_LOT_CLOSE);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_GATE = m_GATE = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";
	UpdateData(FALSE);
}

void CScreen_Server_Network::OnPEBAG_CHECK() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_PPID == "")
	{
		AfxMessageBox("PPID Missing");
		return;
	}
	else if (m_WWN == "")
	{
		AfxMessageBox("WWN Missing");
		return;
	}
	else if (m_C_SERIAL == "")
	{
		AfxMessageBox("C_SERIAL Missing");
		return;
	}
	else if (m_PSID == "")
	{
		AfxMessageBox("PSID Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}

	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_PPID = m_PPID;
	st_mes_data.srt_WWN = m_WWN;
	st_mes_data.srt_C_SERIAL = m_C_SERIAL;
	st_mes_data.srt_PSID = m_PSID;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_PEBAG_CHECK);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_SERIAL = m_SERIAL = "";
	st_mes_data.srt_PPID = m_PPID = "";
	st_mes_data.srt_WWN = m_WWN = "";
	st_mes_data.srt_C_SERIAL = m_C_SERIAL = "";
	st_mes_data.srt_PSID = m_PSID = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";
	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBRAND_PEBAG_CHECK() 
{
	UpdateData(TRUE);
	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1);

	int nSRL_Count = pList->GetCount();
	
	if (m_SSD_CNT == "")
	{
		AfxMessageBox("SSD_CNT Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}

	if (atoi(m_SSD_CNT) != nSRL_Count)
	{
		AfxMessageBox("SSD_CNT not Mathing");
		return;
	}

	st_mes_data.srt_SSD_CNT = m_SSD_CNT;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

	for (int i = 0; i < nSRL_Count; i++)
	{
		pList->SetCurSel(i);
		pList->GetText(i, st_mes_data.srt_SRL[i]);
	}

//	evMes.OnCreate_SendFormat(MES_BRAND_PEBAG_CHECK);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_SSD_CNT = m_SSD_CNT = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	pList->ResetContent();

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnPEBAG_COMPLETE() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}

	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_PEBAG_COMPLETE);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBOAT_ID_MODIF() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_BOAT_ID == "")
	{
		AfxMessageBox("BOAT_ID Missing");
		return;
	}
	else if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_MODIF_MODE == "")
	{
		AfxMessageBox("MODIF_MODE Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}

	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_BOAT_ID = m_BOAT_ID;
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_MODIF_MODE = m_MODIF_MODE;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_BOAT_ID_MODIF);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_BOAT_ID = m_BOAT_ID = "";
	st_mes_data.srt_SERIAL = m_SERIAL = "";
	st_mes_data.srt_MODIF_MODE = m_MODIF_MODE = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBRAND_DTBT_LABEL_ISSUE() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_BOAT_ID == "")
	{
		AfxMessageBox("BOAT_ID Missing");
		return;
	}
	else if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_DTBT_LABEL_CODE == "")
	{
		AfxMessageBox("DTBT_LABEL_CODE Missing");
		return;
	}
	else if (m_DTBT_RIBBON_CODE == "")
	{
		AfxMessageBox("DTBT_RIBBON_CODE Missing");
		return;
	}
	else if (m_MODE == "")
	{
		AfxMessageBox("MODE Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_BOAT_ID = m_BOAT_ID;
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_DTBT_LABEL_CODE = m_DTBT_LABEL_CODE;
	st_mes_data.srt_DTBT_RIBBON_CODE = m_DTBT_RIBBON_CODE;
	st_mes_data.srt_MODE = m_MODE;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_BRAND_DTBT_LABEL_ISSUE);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_BOAT_ID = m_BOAT_ID = "";
	st_mes_data.srt_SERIAL = m_SERIAL = "";
	st_mes_data.srt_DTBT_LABEL_CODE = m_DTBT_LABEL_CODE = "";
	st_mes_data.srt_DTBT_RIBBON_CODE = m_DTBT_RIBBON_CODE = "";
	st_mes_data.srt_MODE = m_MODE = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBRAND_WEIGHT_SBOX_ISSUE() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_WEIGHT == "")
	{
		AfxMessageBox("WEIGHT Missing");
		return;
	}
	else if (m_DTBT_LABEL_SERIAL == "")
	{
		AfxMessageBox("DTBT_LABEL_SERIAL Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}

	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_WEIGHT = m_WEIGHT;
	st_mes_data.srt_DTBT_LABEL_SERIAL = m_DTBT_LABEL_SERIAL;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_BRAND_WEIGHT_SBOX_ISSUE);
//	evMes.OnReq_MesWork();
	
	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_WEIGHT = m_WEIGHT = "";
	st_mes_data.srt_DTBT_LABEL_SERIAL = m_DTBT_LABEL_SERIAL = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnLBOX_ISSUE() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_GATE == "")
	{
		AfxMessageBox("GATE Missing");
		return;
	}
	else if (m_S_BOX_CNT == "")
	{
		AfxMessageBox("S_BOX_CNT Missing");
		return;
	}
	else if (m_S_BOX_LABEL_SERIAL == "")
	{
		AfxMessageBox("S_BOX_LABEL_SERIAL Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_GATE = m_GATE;
	st_mes_data.srt_S_BOX_CNT = m_S_BOX_CNT;
	st_mes_data.srt_S_BOX_LABEL_SERIAL = m_S_BOX_LABEL_SERIAL;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_LBOX_ISSUE);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_GATE = m_GATE = "";
	st_mes_data.srt_S_BOX_CNT = m_S_BOX_CNT = "";
	st_mes_data.srt_S_BOX_LABEL_SERIAL = m_S_BOX_LABEL_SERIAL = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnAUTO_SHIP() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_L_BOX_ID == "")
	{
		AfxMessageBox("L_BOX_ID Missing");
		return;
	}
	else if (m_MO_CLOSE_TIME == "")
	{
		AfxMessageBox("MO_CLOSE_TIME Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_L_BOX_ID = m_L_BOX_ID;
	st_mes_data.srt_MO_CLOSE_TIME = m_MO_CLOSE_TIME;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_AUTO_SHIP);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_L_BOX_ID = m_L_BOX_ID = "";
	st_mes_data.srt_MO_CLOSE_TIME = m_MO_CLOSE_TIME = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnPROD_SERIAL_INFO() 
{
	UpdateData(TRUE);
	if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_PROD_SERIAL_INFO);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_SERIAL = m_SERIAL = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBOAT_ID_INFO() 
{
	UpdateData(TRUE);
	if (m_BOAT_ID == "")
	{
		AfxMessageBox("BOAT_ID Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_BOAT_ID = m_BOAT_ID;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_BOAT_ID_INFO);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_BOAT_ID = m_BOAT_ID = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnSBOX_INFO() 
{
	UpdateData(TRUE);
	if (m_S_BOX_ID == "")
	{
		AfxMessageBox("S_BOX_ID Missing");
		return;
	}
	else if (m_GATE == "")
	{
		AfxMessageBox("GATE Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_S_BOX_ID = m_S_BOX_ID;
	st_mes_data.srt_GATE = m_GATE;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_SBOX_INFO);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_S_BOX_ID = m_S_BOX_ID = "";
	st_mes_data.srt_GATE = m_GATE = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnPACKING_MASTER_INFO() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_PACKING_MASTER_INFO);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_SERIAL = m_SERIAL = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnPK_XCLOSE() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_PK_XCLOSE);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnTOOL_CHANGE() 
{
	UpdateData(TRUE);
	if (m_TOOLID == "")
	{
		AfxMessageBox("TOOLID Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_TOOLID = m_TOOLID;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_TOOL_CHANGE);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_TOOLID = m_TOOLID = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnTOOL_CHECK() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_TOOL_CHECK);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBRAND_DENSITY_LABEL_CCS() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_DEN_LABEL_MAT_CODE == "")
	{
		AfxMessageBox("DEN_LABEL_MAT_CODE Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_DEN_LABEL_MAT_CODE = m_DEN_LABEL_MAT_CODE;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_BRAND_DENSITY_LABEL_CCS);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_SERIAL = m_SERIAL = "";
	st_mes_data.srt_DEN_LABEL_MAT_CODE = m_DEN_LABEL_MAT_CODE = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnBRAND_DTBT_LABEL_CCS() 
{
	UpdateData(TRUE);
	if (m_LOT_ID == "")
	{
		AfxMessageBox("LOD_ID Missing");
		return;
	}
	else if (m_SERIAL == "")
	{
		AfxMessageBox("SERIAL Missing");
		return;
	}
	else if (m_DTBT_MAT_CODE == "")
	{
		AfxMessageBox("DTBT_MAT_CODE Missing");
		return;
	}
	else if (m_RIBBON_MAT_CODE == "")
	{
		AfxMessageBox("m_RIBBON_MAT_CODE Missing");
		return;
	}
	else if (m_RETRY == "")
	{
		AfxMessageBox("RETRY Missing");
		return;
	}
	else if (m_RETRY_CNT == "")
	{
		AfxMessageBox("RETRY_CNT Missing");
		return;
	}
	
	st_mes_data.srt_LOT_ID = m_LOT_ID;
	st_mes_data.srt_SERIAL = m_SERIAL;
	st_mes_data.srt_DTBT_MAT_CODE = m_DTBT_MAT_CODE;
	st_mes_data.srt_RIBBON_MAT_CODE = m_RIBBON_MAT_CODE;
	st_mes_data.srt_RETRY = m_RETRY;
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT;

//	evMes.OnCreate_SendFormat(MES_BRAND_DTBT_LABEL_CCS);
//	evMes.OnReq_MesWork();

	st_mes_data.srt_LOT_ID = m_LOT_ID = "";
	st_mes_data.srt_SERIAL = m_SERIAL = "";
			st_mes_data.srt_DTBT_MAT_CODE = m_DTBT_MAT_CODE = "";
	st_mes_data.srt_RIBBON_MAT_CODE = m_RIBBON_MAT_CODE = "";
	st_mes_data.srt_RETRY = m_RETRY = "";
	st_mes_data.srt_RETRY_CNT = m_RETRY_CNT = "";

	UpdateData(FALSE);
}

void CScreen_Server_Network::OnSRLADD() 
{
	UpdateData(TRUE);
	
	if (m_PROD_SRLNO == "")
	{
		AfxMessageBox("PROD_SRLNO Missing");
		return;
	}
	else if (m_PIECE_PROD_ID == "")
	{
		AfxMessageBox("PIECE_PROD_ID Missing");
		return;
	}
	else if (m_WWN_SRLNO == "")
	{
		AfxMessageBox("WWN_SRLNO Missing");
		return;
	}
	else if (m_CUST_SRLNO == "")
	{
		AfxMessageBox("CUST_SRLNO Missing");
		return;
	}
	else if (m_PHYS_SECRTY_ID == "")
	{
		AfxMessageBox("PHYS_SECRTY_ID Missing");
		return;
	}

	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1);
	
	st_mes_data.srt_PROD_SRLNO = m_PROD_SRLNO;
	st_mes_data.srt_PIECE_PROD_ID = m_PIECE_PROD_ID;
	st_mes_data.srt_WWN_SRLNO = m_WWN_SRLNO;
	st_mes_data.srt_CUST_SRLNO = m_CUST_SRLNO;
	st_mes_data.srt_PHYS_SECRTY_ID = m_PHYS_SECRTY_ID;

	int nSRL_Count = pList->GetCount();
	CString str_Temp="";
	str_Temp.Format("SRL%02d=\"PROD_SRLNO=%s,PIECE_PROD_ID=%s,WWN_SRLNO=%s,CUST_CRLNO=%s,PHYS_SECRTY_ID=%s\" ",
		nSRL_Count+1, st_mes_data.srt_PROD_SRLNO, st_mes_data.srt_PIECE_PROD_ID, st_mes_data.srt_WWN_SRLNO, st_mes_data.srt_CUST_SRLNO, st_mes_data.srt_PHYS_SECRTY_ID);

	pList->AddString(str_Temp);
	pList->SetCurSel(pList->GetCount() - 1); 
	
	st_mes_data.srt_PROD_SRLNO = m_PROD_SRLNO = "";
	st_mes_data.srt_PIECE_PROD_ID = m_PIECE_PROD_ID = "";
	st_mes_data.srt_WWN_SRLNO = m_WWN_SRLNO = "";
	st_mes_data.srt_CUST_SRLNO = m_CUST_SRLNO = "";
	st_mes_data.srt_PHYS_SECRTY_ID = m_PHYS_SECRTY_ID = "";

	UpdateData(FALSE);
}

