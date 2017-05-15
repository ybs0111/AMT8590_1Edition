// Dialog_NetWork.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_NetWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_NetWork dialog


CDialog_NetWork::CDialog_NetWork(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_NetWork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_NetWork)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Server Test"; 
}


void CDialog_NetWork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_NetWork)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_NetWork, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_NetWork)
	ON_BN_CLICKED(IDC_BUTTON_PACK_MASTER_INFO, OnButtonPackMasterInfo)
	ON_BN_CLICKED(IDC_BUTTON_PROD_SERIAL_INFO, OnButtonProdSerialInfo)
	ON_BN_CLICKED(IDC_BUTTON_BOATID_MODIFY, OnButtonBoatidModify)
	ON_BN_CLICKED(IDC_BUTTON_BOATID_INFO, OnButtonBoatidInfo)
	ON_BN_CLICKED(IDC_BTN_LABEL_SIZE, OnBtnLabelSize)
	ON_BN_CLICKED(IDC_BTN_LABEL_CCS, OnBtnLabelCcs)
	ON_BN_CLICKED(IDC_BTN_BCR_CCS, OnBtnBcrCcs)
	ON_BN_CLICKED(IDC_BTN_BCR_FORMAT, OnBtnBcrFormat)
	ON_BN_CLICKED(IDC_BTN_XCLOSE, OnBtnXclose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_NetWork message handlers

BOOL CDialog_NetWork::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_NetWork::OnButtonPackMasterInfo() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 9);	
	
}

void CDialog_NetWork::OnButtonProdSerialInfo() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 6);	
	
}

void CDialog_NetWork::OnButtonBoatidModify() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 9);	
	
}

void CDialog_NetWork::OnButtonBoatidInfo() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 7);	
	
}

void CDialog_NetWork::OnBtnLabelSize() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 1);	
	
}

void CDialog_NetWork::OnBtnLabelCcs() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 3);	
	
}

void CDialog_NetWork::OnBtnBcrCcs() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 4);	
	
}

void CDialog_NetWork::OnBtnBcrFormat() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 2);	
	
}

void CDialog_NetWork::OnBtnXclose() 
{
	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1003, 5);	
	
}
