// Screen_Model.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Model.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Model

IMPLEMENT_DYNCREATE(CScreen_Model, CFormView)

CScreen_Model::CScreen_Model()
	: CFormView(CScreen_Model::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Model)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Model::~CScreen_Model()
{
}

void CScreen_Model::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Model)
	DDX_Control(pDX, IDC_COMBO_MODEL, m_cbo_model);
	DDX_Control(pDX, IDC_GROUP_NEW_MODEL, m_group_newmodel);
	DDX_Control(pDX, IDC_GROUP_REG_MODEL, m_group_regmodel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Model, CFormView)
	//{{AFX_MSG_MAP(CScreen_Model)
	ON_BN_CLICKED(IDC_BTN_DVC_DELETE, OnBtnDvcDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, OnSelchangeComboModel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Model diagnostics

#ifdef _DEBUG
void CScreen_Model::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Model::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Model message handlers

void CScreen_Model::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnModel_EditBox_Set();
	OnModel_NewEdit();
	OnModel_Combo_Display();
}

void CScreen_Model::OnModel_NewEdit()
{
	st_handler.m_strNewPartNmber.TrimLeft();
	st_handler.m_strNewPartNmber.TrimRight();
	m_edit_new_partid.SetWindowText(st_handler.m_strNewPartNmber);
}

void CScreen_Model::OnModel_Combo_Display()
{
	CString strCbomo;
// 	st_work.m_nCboSelModel = 0;
	Func.LoadRegModelData();
	for (int i= 0; i < st_work.m_nMdlTotal; i++ )
	{
		strCbomo.Format("%s", st_work.m_strModelName[i]);
		strCbomo.TrimLeft(); strCbomo.TrimRight();
		m_cbo_model.AddString( (LPCTSTR)strCbomo);
	}
	m_cbo_model.SetCurSel(st_work.m_nCboSelModel);
}

void CScreen_Model::OnModel_Select()
{
	Func.GetModelNum(st_handler.m_strNewPartNmber);	
}

void CScreen_Model::OnModel_EditBox_Set()
{
	m_edit_new_partid.SubclassDlgItem(IDC_EDIT_NEW_PARTID, this);
	m_edit_new_partid.bkColor(RGB(50, 100, 150));
	m_edit_new_partid.textColor(RGB(255, 255,255));
	m_edit_new_partid.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

void CScreen_Model::OnSelchangeComboModel() 
{
	int nSel = m_cbo_model.GetCurSel();
	CString sText;
	m_cbo_model.GetWindowText(sText);
}

void CScreen_Model::OnModel_Display()
{
	CString sText;
	m_cbo_model.GetWindowText(sText);
// 	:: WritePrivateProfileString( sText, "", LPCTSTR(mstr_temp), st_path.mstr_reg_model);

}

void CScreen_Model::OnBtnDvcDelete() 
{
	// TODO: Add your control notification handler code here
	
}

void CScreen_Model::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	
}

