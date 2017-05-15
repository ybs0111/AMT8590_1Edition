// Dialog_Btm_IDBuffer_Info.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Btm_IDBuffer_Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Btm_IDBuffer_Info dialog


CDialog_Btm_IDBuffer_Info::CDialog_Btm_IDBuffer_Info(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Btm_IDBuffer_Info::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Btm_IDBuffer_Info)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Btm ID Buffer Info";
}


void CDialog_Btm_IDBuffer_Info::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Btm_IDBuffer_Info)
	DDX_Control(pDX, IDC_GROUP_BTM_IDBUFFER_INFO, m_group_btm_idbuffer_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Btm_IDBuffer_Info, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Btm_IDBuffer_Info)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Btm_IDBuffer_Info message handlers

BOOL CDialog_Btm_IDBuffer_Info::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	if (st_handler.mn_switch_lock == FALSE)
	{
		st_handler.mn_switch_lock = TRUE;
		mn_btm_dialog = 1;
	}
	else	mn_btm_dialog = 0;
	
	OnIDBuffer_Btm_GroupBox_Set();
	OnIDBuffer_Btm_EditBox_Set();
	if(st_work.n_Btm_IDBuffer_Info > -1) 
	{
		OnBTMIDBuffer_Input_Info(st_work.n_Btm_IDBuffer_Info);	
		// // 	OnRun_Step_Input_Info(6 + st_work.n_BTMIDBuffer_Info);
	}
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Btm_IDBuffer_Info::OnDestroy() 
{
	StandardDialog::OnDestroy();
	st_handler.mn_switch_lock = FALSE;
	// TODO: Add your message handler code here
	
}

BOOL CDialog_Btm_IDBuffer_Info::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_SYSKEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN :
			return FALSE;
			break;  // Exit나 Return 눌러 종료되지 않도록 처리
		case VK_ESCAPE :
			return FALSE;
			break;
		case VK_SPACE :
			pMsg->wParam = NULL;
			return FALSE;
			break;
		case VK_F4 :
			pMsg->wParam = NULL;
			return FALSE;
			break;
		case VK_F9 :
			SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	return StandardDialog::PreTranslateMessage(pMsg);
}

void CDialog_Btm_IDBuffer_Info::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	if(st_work.n_Btm_IDBuffer_Info > -1) 
   	{
		
		OnBTMIDBuffer_Output_Info(st_work.n_Btm_IDBuffer_Info);

		// 		OnRun_Step_Output_Info(6 + st_work.n_BTMIDBuffer_Info);
	}	
	StandardDialog::OnOK();
}

void CDialog_Btm_IDBuffer_Info::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	StandardDialog::OnCancel();
}
	void CDialog_Btm_IDBuffer_Info::OnIDBuffer_Btm_GroupBox_Set()
{
		CSxLogFont IDBuffer_Font(15,FW_SEMIBOLD,false,"MS Sans Serif");
		
		//	m_group_pass_chg.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
		m_group_btm_idbuffer_info.SetBackgroundColor(RGB(206, 206, 206), RGB(255, 255, 204));
		m_group_btm_idbuffer_info.SetFont(IDBuffer_Font);
		m_group_btm_idbuffer_info.SetCatptionTextColor(RGB(145,25,0));
		m_group_btm_idbuffer_info.SetFontBold(TRUE);
		
		

	
}

void CDialog_Btm_IDBuffer_Info::OnIDBuffer_Btm_EditBox_Set()
{
	

	m_edit_btm_id_buffer_info_left_3.SubclassDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_LEFT_3, this);
	m_edit_btm_id_buffer_info_left_3.bkColor(RGB(100, 150, 200));
	m_edit_btm_id_buffer_info_left_3.textColor(RGB(255, 255,255));
    m_edit_btm_id_buffer_info_left_3.setFont(-16);


	m_edit_btm_id_buffer_info_left_5.SubclassDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_LEFT_5, this);
	m_edit_btm_id_buffer_info_left_5.bkColor(RGB(100, 150, 200));
	m_edit_btm_id_buffer_info_left_5.textColor(RGB(255, 255,255));
    m_edit_btm_id_buffer_info_left_5.setFont(-16);	
	
	

	m_edit_btm_id_buffer_info_right_3.SubclassDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_RIGHT_3, this);
	m_edit_btm_id_buffer_info_right_3.bkColor(RGB(100, 150, 200));
	m_edit_btm_id_buffer_info_right_3.textColor(RGB(255, 255,255));
    m_edit_btm_id_buffer_info_right_3.setFont(-16);
	
	
	m_edit_btm_id_buffer_info_right_5.SubclassDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_RIGHT_5, this);
	m_edit_btm_id_buffer_info_right_5.bkColor(RGB(100, 150, 200));
	m_edit_btm_id_buffer_info_right_5.textColor(RGB(255, 255,255));
    m_edit_btm_id_buffer_info_right_5.setFont(-16);
	

	m_edit_btm_id_buffer_info_title_1.SubclassDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_TITLE_1, this);
	m_edit_btm_id_buffer_info_title_1.bkColor(RGB(100, 150, 200));
	m_edit_btm_id_buffer_info_title_1.textColor(RGB(0, 0,0));
    m_edit_btm_id_buffer_info_title_1.setFont(-17);	

	//add end by kwlee
	


}

void CDialog_Btm_IDBuffer_Info::OnBTMIDBuffer_Input_Info(int nPos)
{
	CString sTmp="";
	
	
	if(nPos == 0) mstr_btm_id_buffer_info_title_1 = "BTM";
	if(nPos == 1) mstr_btm_id_buffer_info_title_1 = "CLAMP";
	if(nPos == 2) mstr_btm_id_buffer_info_title_1 = "Load";
  	if(nPos == 3) mstr_btm_id_buffer_info_title_1 = "Unload";
	if(nPos == 4) mstr_btm_id_buffer_info_title_1 = "Next";
		//14.12.16 add modi end by kwlee 
		
		((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_TITLE_1))->SetWindowText(mstr_btm_id_buffer_info_title_1);	
		
		
		
		////////////////////////////////////////////////////////////////////////// Btm input id buffer 
		sTmp.Format("%d", st_idbuff_info[nPos + 9].n_idbuff_num[0]);
		if(mstr_btm_id_buffer_info_old[nPos + 9][0] != sTmp)
		{
			mstr_btm_id_buffer_info_old[nPos + 9][0] = sTmp;
			((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_LEFT_3))->SetWindowText(mstr_btm_id_buffer_info_old[nPos + 9][0]);	
		}
		
		sTmp.Format("%d", st_idbuff_info[nPos + 9].n_idbuff_num[1]);
		if(mstr_btm_id_buffer_info_old[nPos + 9][1] != sTmp)
		{
			mstr_btm_id_buffer_info_old[nPos + 9][1] = sTmp;
			((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_RIGHT_3))->SetWindowText(mstr_btm_id_buffer_info_old[nPos + 9][1]);	
		}
		
		//////////////////////////////////////////////////////////////////////// Btm input id buffer 
		sTmp.Format("%s", st_idbuff_info[nPos + 9].c_var[0]);
		if(mstr_btm_id_buffer_info_old[nPos + 9][0] != sTmp)
		{
			mstr_btm_id_buffer_info_old[nPos + 9][0] = sTmp;
			((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_LEFT_5))->SetWindowText(mstr_btm_id_buffer_info_old[nPos + 9][0]);	
		}
		
		sTmp.Format("%s", st_idbuff_info[nPos + 9].c_var[1]);
		if(mstr_btm_id_buffer_info_old[nPos + 9][1] != sTmp)
		{
			mstr_btm_id_buffer_info_old[nPos + 9][1] = sTmp;
			((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_RIGHT_5))->SetWindowText(mstr_btm_id_buffer_info_old[nPos + 9][1]);	
		}
}

void CDialog_Btm_IDBuffer_Info::OnBTMIDBuffer_Output_Info(int nPos)
{


	//////////////////////////////////////////////////////////////////////////// input id buffer 
	((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_LEFT_3))->GetWindowText(mstr_btm_id_buffer_info_new[nPos + 9][0]);	
	st_idbuff_info[nPos + 9].n_idbuff_num[0] = atoi(mstr_btm_id_buffer_info_new[nPos + 9][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_RIGHT_3))->GetWindowText(mstr_btm_id_buffer_info_new[nPos+ 9][1]);	
	st_idbuff_info[nPos + 9].n_idbuff_num[1] = atoi(mstr_btm_id_buffer_info_new[nPos + 9][1]);
	// 
	// 	///////////////////////////////////////////////////////////////////////// Var
	((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_LEFT_5))->GetWindowText(mstr_btm_id_buffer_info_new[nPos + 9][0]);	
	sprintf(st_idbuff_info[nPos + 9].c_var[0], "%s", mstr_btm_id_buffer_info_new[nPos + 9][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_BTM_IDBUFFER_INFO_RIGHT_5))->GetWindowText(mstr_btm_id_buffer_info_new[nPos + 9][1]);	
	sprintf(st_idbuff_info[nPos + 9].c_var[1], "%s", mstr_btm_id_buffer_info_new[nPos + 9][1]);
	
}
