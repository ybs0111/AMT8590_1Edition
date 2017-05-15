// Dialog_IDBuffer_Info.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_IDBuffer_Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_IDBuffer_Info dialog
CDialog_IDBuffer_Info IDBuffer_Info;


CDialog_IDBuffer_Info::CDialog_IDBuffer_Info(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_IDBuffer_Info::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_IDBuffer_Info)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "ID Buffer Info";
}


void CDialog_IDBuffer_Info::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_IDBuffer_Info)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_GROUP_IDBUFFER_INFO, m_group_idbuffer_info);
	DDX_Control(pDX, IDC_GROUP_RUN_STEP, m_group_run_step);

	DDX_Control(pDX, IDC_MSG_RUN_STEP_1, m_msg_run_step_1);
	DDX_Control(pDX, IDC_MSG_RUN_STEP_2, m_msg_run_step_2);
	DDX_Control(pDX, IDC_MSG_RUN_STEP_3, m_msg_run_step_3);
	DDX_Control(pDX, IDC_MSG_RUN_STEP_4, m_msg_run_step_4);
	DDX_Control(pDX, IDC_MSG_RUN_STEP_5, m_msg_run_step_5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_IDBuffer_Info, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_IDBuffer_Info)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_IDBuffer_Info message handlers
BOOL CDialog_IDBuffer_Info::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// Start Switch의 동작을 막는다.
	if (st_handler.mn_switch_lock == FALSE)
	{
		st_handler.mn_switch_lock = TRUE;
		mn_dialog = 1;
	}
	else	mn_dialog = 0;
	
	OnIDBuffer_GroupBox_Set();
	OnIDBuffer_EditBox_Set();
	OnIDBuffer_Show_Enable();

// 	////2014.1110
	if(st_work.n_IDBuffer_Info > -1)
	{
		OnIDBuffer_Input_Info(st_work.n_IDBuffer_Info);
// 		OnRun_Step_Input_Info(st_work.n_IDBuffer_Info);
	}

// 	if(st_work.n_Btm_IDBuffer_Info > -1) //2014,1217
// 	{
// 		OnBTMIDBuffer_Input_Info(st_work.n_Btm_IDBuffer_Info);
// //		OnRun_Step_Input_Info(6 + st_work.n_BTMIDBuffer_Info);
// 	}
// // 	////


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDialog_IDBuffer_Info::OnDestroy() 
{	
// 	if (mn_dialog == 1)
// 	{
		st_handler.mn_switch_lock = FALSE;
// 	}
	
	StandardDialog::OnDestroy();
}

void CDialog_IDBuffer_Info::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	////2014.1110
	if(st_work.n_IDBuffer_Info > -1) 
	{
		OnIDBuffer_Output_Info(st_work.n_IDBuffer_Info);
// 		OnRun_Step_Output_Info(st_work.n_IDBuffer_Info);
	}
// 
	//del by kwlee 15.01.09
// 	if(st_work.n_Btm_IDBuffer_Info > -1) //2014,1217
// 	{
// 		OnBTMIDBuffer_Output_Info(st_work.n_Btm_IDBuffer_Info);
// //		OnRun_Step_Output_Info(6 + st_work.n_BTMIDBuffer_Info);
// 	}
	////

	
	StandardDialog::OnOK();
}

void CDialog_IDBuffer_Info::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	StandardDialog::OnCancel();	
}

void CDialog_IDBuffer_Info::OnIDBuffer_GroupBox_Set()
{
	CSxLogFont IDBuffer_Font(15,FW_SEMIBOLD,false,"MS Sans Serif");
	
	//	m_group_pass_chg.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
	m_group_idbuffer_info.SetBackgroundColor(RGB(206, 206, 206), RGB(255, 255, 204));
	m_group_idbuffer_info.SetFont(IDBuffer_Font);
	m_group_idbuffer_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_idbuffer_info.SetFontBold(TRUE);

	m_group_run_step.SetBackgroundColor(RGB(206, 206, 206), RGB(255, 255, 204));
	m_group_run_step.SetFont(IDBuffer_Font);
	m_group_run_step.SetCatptionTextColor(RGB(145,25,0));
	m_group_run_step.SetFontBold(TRUE);
}

void CDialog_IDBuffer_Info::OnIDBuffer_EditBox_Set()
{
	m_edit_id_buffer_info_left_1.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_1, this);
	m_edit_id_buffer_info_left_1.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_left_1.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_left_1.setFont(-16);

	m_edit_id_buffer_info_left_2.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_2, this);
	m_edit_id_buffer_info_left_2.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_left_2.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_left_2.setFont(-16);

	m_edit_id_buffer_info_left_3.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_3, this);
	m_edit_id_buffer_info_left_3.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_left_3.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_left_3.setFont(-16);

	m_edit_id_buffer_info_left_4.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_4, this);
	m_edit_id_buffer_info_left_4.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_left_4.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_left_4.setFont(-16);
	
	m_edit_id_buffer_info_left_5.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_5, this);
	m_edit_id_buffer_info_left_5.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_left_5.textColor(RGB(255, 255,255));
    m_edit_id_buffer_info_left_5.setFont(-16);
// 	
// 	m_edit_id_buffer_info_left_6.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_6, this);
// 	m_edit_id_buffer_info_left_6.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_left_6.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_left_6.setFont(-16);	
// 
// 	m_edit_id_buffer_info_left_7.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_7, this);
// 	m_edit_id_buffer_info_left_7.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_left_7.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_left_7.setFont(-16);	
// 
// 	m_edit_id_buffer_info_left_8.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_8, this);
// 	m_edit_id_buffer_info_left_8.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_left_8.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_left_8.setFont(-16);	
// 
// 	m_edit_id_buffer_info_left_11.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_11, this);
// 	m_edit_id_buffer_info_left_11.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_left_11.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_left_11.setFont(-16);	

	m_edit_id_buffer_info_right_1.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_1, this);
	m_edit_id_buffer_info_right_1.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_right_1.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_right_1.setFont(-16);
	
	m_edit_id_buffer_info_right_2.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_2, this);
	m_edit_id_buffer_info_right_2.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_right_2.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_right_2.setFont(-16);
	
	m_edit_id_buffer_info_right_3.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_3, this);
	m_edit_id_buffer_info_right_3.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_right_3.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_right_3.setFont(-16);
	
	m_edit_id_buffer_info_right_4.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_4, this);
	m_edit_id_buffer_info_right_4.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_right_4.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_right_4.setFont(-16);
	
	m_edit_id_buffer_info_right_5.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_5, this);
	m_edit_id_buffer_info_right_5.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_right_5.textColor(RGB(255, 255,255));
    m_edit_id_buffer_info_right_5.setFont(-16);
// 	
// 	m_edit_id_buffer_info_right_6.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_6, this);
// 	m_edit_id_buffer_info_right_6.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_right_6.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_right_6.setFont(-16);	
// 	
// 	m_edit_id_buffer_info_right_7.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_7, this);
// 	m_edit_id_buffer_info_right_7.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_right_7.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_right_7.setFont(-16);	
// 
// 	m_edit_id_buffer_info_right_8.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_8, this);
// 	m_edit_id_buffer_info_right_8.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_right_8.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_right_8.setFont(-16);	
// 
// 	m_edit_id_buffer_info_right_11.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_11, this);
// 	m_edit_id_buffer_info_right_11.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_right_11.textColor(RGB(255, 255,255));
//     m_edit_id_buffer_info_right_11.setFont(-16);	

	m_edit_id_buffer_info_title_1.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_TITLE_1, this);
	m_edit_id_buffer_info_title_1.bkColor(RGB(100, 150, 200));
	m_edit_id_buffer_info_title_1.textColor(RGB(0, 0,0));
    m_edit_id_buffer_info_title_1.setFont(-17);	

// 	m_edit_id_buffer_info_title_2.SubclassDlgItem(IDC_EDIT_IDBUFFER_INFO_TITLE_2, this);
// 	m_edit_id_buffer_info_title_2.bkColor(RGB(100, 150, 200));
// 	m_edit_id_buffer_info_title_2.textColor(RGB(255, 255,255));
//  m_edit_id_buffer_info_title_2.setFont(-17);	

	m_edit_run_step_1.SubclassDlgItem(IDC_EDIT_RUN_STEP_1, this);
	m_edit_run_step_1.bkColor(RGB(100, 150, 200));
	m_edit_run_step_1.textColor(RGB(0,0,0));//(255, 255,255));
    m_edit_run_step_1.setFont(-16);	

	m_edit_run_step_2.SubclassDlgItem(IDC_EDIT_RUN_STEP_2, this);
	m_edit_run_step_2.bkColor(RGB(100, 150, 200));
	m_edit_run_step_2.textColor(RGB(0,0,0));//(255, 255,255));
    m_edit_run_step_2.setFont(-16);	

	m_edit_run_step_3.SubclassDlgItem(IDC_EDIT_RUN_STEP_3, this);
	m_edit_run_step_3.bkColor(RGB(100, 150, 200));
	m_edit_run_step_3.textColor(RGB(0,0,0));//(255, 255,255));
    m_edit_run_step_3.setFont(-16);	

	m_edit_run_step_4.SubclassDlgItem(IDC_EDIT_RUN_STEP_4, this);
	m_edit_run_step_4.bkColor(RGB(100, 150, 200));
	m_edit_run_step_4.textColor(RGB(0,0,0));//(255, 255,255));
    m_edit_run_step_4.setFont(-16);	


}

void CDialog_IDBuffer_Info::OnIDBuffer_Input_Info(int nPos)
{
	CString sTmp="";

	if(nPos == 0) mstr_id_buffer_info_title_1 = "Recv";
	if(nPos == 1) mstr_id_buffer_info_title_1 = "Input";
	if(nPos == 2) mstr_id_buffer_info_title_1 = "Density";
	if(nPos == 3) mstr_id_buffer_info_title_1 = "Barcode1";
	if(nPos == 4) mstr_id_buffer_info_title_1 = "Barcode2";
	if(nPos == 5) mstr_id_buffer_info_title_1 = "Top Vision";
	if(nPos == 6) mstr_id_buffer_info_title_1 = "Unload";

	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_TITLE_1))->SetWindowText(mstr_id_buffer_info_title_1);	

	////////////////////////////////////////////////////////////////////////// Product Type model name
 	sTmp.Format("%s", st_idbuff_info[nPos].c_product_id[0]);
	if(mstr_id_buffer_info_old[nPos][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_1))->SetWindowText(mstr_id_buffer_info_old[nPos][0]);	
	}

	sTmp.Format("%s", st_idbuff_info[nPos].c_product_id[1]);
	if(mstr_id_buffer_info_old[nPos][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_1))->SetWindowText(mstr_id_buffer_info_old[nPos][1]);	
	}

	////////////////////////////////////////////////////////////////////////// lot id
	sTmp.Format("%s", st_idbuff_info[nPos].c_lot_id[0]);
	if(mstr_id_buffer_info_old[nPos][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_2))->SetWindowText(mstr_id_buffer_info_old[nPos][0]);	
	}
	
	sTmp.Format("%s", st_idbuff_info[nPos].c_lot_id[1]);
	if(mstr_id_buffer_info_old[nPos][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_2))->SetWindowText(mstr_id_buffer_info_old[nPos][1]);	
	}

	////////////////////////////////////////////////////////////////////////// input id buffer 
	sTmp.Format("%d", st_idbuff_info[nPos].n_idbuff_num[0]);
	if(mstr_id_buffer_info_old[nPos][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_3))->SetWindowText(mstr_id_buffer_info_old[nPos][0]);	
	}
	
	sTmp.Format("%d", st_idbuff_info[nPos].n_idbuff_num[1]);
	if(mstr_id_buffer_info_old[nPos][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_3))->SetWindowText(mstr_id_buffer_info_old[nPos][1]);	
	}

	////////////////////////////////////////////////////////////////////////// ssd serial
	sTmp.Format("%s", st_idbuff_info[nPos].c_serial[0]);
	if(mstr_id_buffer_info_old[nPos][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_4))->SetWindowText(mstr_id_buffer_info_old[nPos][0]);	
	}
	
	sTmp.Format("%s", st_idbuff_info[nPos].c_serial[1]);
	if(mstr_id_buffer_info_old[nPos][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_4))->SetWindowText(mstr_id_buffer_info_old[nPos][1]);	
	}
 
	////////////////////////////////////////////////////////////////////////// var
	sTmp.Format("%s", st_idbuff_info[nPos].c_var[0]);
	if(mstr_id_buffer_info_old[nPos][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_5))->SetWindowText(mstr_id_buffer_info_old[nPos][0]);	
	}
	
	sTmp.Format("%s", st_idbuff_info[nPos].c_var[1]);
	if(mstr_id_buffer_info_old[nPos][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_5))->SetWindowText(mstr_id_buffer_info_old[nPos][1]);	
	}

}

void CDialog_IDBuffer_Info::OnBTMIDBuffer_Input_Info(int nPos)
{
	CString sTmp="";
	
	if(nPos == 4) mstr_id_buffer_info_title_1 = "Next";
	if(nPos == 3) mstr_id_buffer_info_title_1 = "Unload";
	if(nPos == 2) mstr_id_buffer_info_title_1 = "Load";
	if(nPos == 1) mstr_id_buffer_info_title_1 = "Clamp";
	if(nPos == 0) mstr_id_buffer_info_title_1 = "BTM";
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_TITLE_1))->SetWindowText(mstr_id_buffer_info_title_1);	
	
	////////////////////////////////////////////////////////////////////////// input id buffer 
	sTmp.Format("%d", st_idbuff_info[nPos+9].n_idbuff_num[0]);
	if(mstr_id_buffer_info_old[nPos+9][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos+9][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_3))->SetWindowText(mstr_id_buffer_info_old[nPos+9][0]);	
	}
	
	sTmp.Format("%d", st_idbuff_info[nPos+9].n_idbuff_num[1]);
	if(mstr_id_buffer_info_old[nPos+9][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos+9][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_3))->SetWindowText(mstr_id_buffer_info_old[nPos+9][1]);	
	}
	
	////////////////////////////////////////////////////////////////////////// var
	sTmp.Format("%s", st_idbuff_info[nPos+9].c_var[0]);
	if(mstr_id_buffer_info_old[nPos+9][0] != sTmp)
	{
		mstr_id_buffer_info_old[nPos+9][0] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_5))->SetWindowText(mstr_id_buffer_info_old[nPos+9][0]);	
	}
	
	sTmp.Format("%s", st_idbuff_info[nPos+9].c_var[1]);
	if(mstr_id_buffer_info_old[nPos+9][1] != sTmp)
	{
		mstr_id_buffer_info_old[nPos+9][1] = sTmp;
		((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_5))->SetWindowText(mstr_id_buffer_info_old[nPos+9][1]);	
	}
}

void CDialog_IDBuffer_Info::OnIDBuffer_Output_Info(int nPos)
{
	CString sTmp="";
	CString Temp[2];
	int Len[2];

	////////////////////////////////////////////////////////////////////////// Product Type model name
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_1))->GetWindowText(mstr_id_buffer_info_new[nPos][0]);	
	sprintf(st_idbuff_info[nPos].c_product_id[0], "%s", mstr_id_buffer_info_new[nPos][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_1))->GetWindowText(mstr_id_buffer_info_new[nPos][1]);	
	sprintf(st_idbuff_info[nPos].c_product_id[1], "%s", mstr_id_buffer_info_new[nPos][1]);
	
	////////////////////////////////////////////////////////////////////////// lot id
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_2))->GetWindowText(mstr_id_buffer_info_new[nPos][0]);	
	sprintf(st_idbuff_info[nPos].c_lot_id[0], "%s", mstr_id_buffer_info_new[nPos][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_2))->GetWindowText(mstr_id_buffer_info_new[nPos][1]);	
	sprintf(st_idbuff_info[nPos].c_lot_id[1], "%s", mstr_id_buffer_info_new[nPos][1]);
	
	////////////////////////////////////////////////////////////////////////// input id buffer 
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_3))->GetWindowText(mstr_id_buffer_info_new[nPos][0]);	
	st_idbuff_info[nPos].n_idbuff_num[0] = atoi(mstr_id_buffer_info_new[nPos][0]);
	
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_3))->GetWindowText(mstr_id_buffer_info_new[nPos][1]);	
	st_idbuff_info[nPos].n_idbuff_num[1] = atoi(mstr_id_buffer_info_new[nPos][1]);
	
	////////////////////////////////////////////////////////////////////////// ssd serial
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_4))->GetWindowText(mstr_id_buffer_info_new[nPos][0]);	
	//sprintf(st_idbuff_info[nPos].c_serial[0], "%s", mstr_id_buffer_info_new[nPos][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_4))->GetWindowText(mstr_id_buffer_info_new[nPos][1]);	
	//sprintf(st_idbuff_info[nPos].c_serial[1], "%s", mstr_id_buffer_info_new[nPos][1]);
	
	//2015.0113
	Temp[0].Format("%s",mstr_id_buffer_info_new[nPos][0]);
	Temp[1].Format("%s",mstr_id_buffer_info_new[nPos][1]);
	
	for(int i =0; i<2; i++)
	{
		Temp[i].TrimRight();
		Temp[i].TrimRight();
		Len[i] = Temp[i].GetLength();	
	}
	
	sTmp.Format("%s","0");
	if (Temp[0].Find("S",0) == 0 && Len[0] >10 )
	{	
		sprintf(st_idbuff_info[nPos].c_serial[0], "%s", mstr_id_buffer_info_new[nPos][0]);
	}
	else
	{
		AfxMessageBox("Input Left Serial again");
		sprintf(st_idbuff_info[nPos].c_serial[0], "%s",sTmp);
	}
	
	if (Temp[1].Find("S",0) == 0 && Len[1] >10 )
	{
		sprintf(st_idbuff_info[nPos].c_serial[1], "%s", mstr_id_buffer_info_new[nPos][1]);
	}
	else
	{
		AfxMessageBox("Input Right Serial again");	
		sprintf(st_idbuff_info[nPos].c_serial[1], "%s",sTmp);
	}
	
	////////////////////////////////////////////////////////////////////////// var 
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_5))->GetWindowText(mstr_id_buffer_info_new[nPos][0]);	
	sprintf(st_idbuff_info[nPos].c_var[0], "%s", mstr_id_buffer_info_new[nPos][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_5))->GetWindowText(mstr_id_buffer_info_new[nPos][1]);	
	sprintf(st_idbuff_info[nPos].c_var[1], "%s", mstr_id_buffer_info_new[nPos][1]);
	
	
}

void CDialog_IDBuffer_Info::OnBTMIDBuffer_Output_Info(int nPos)
{
	
	////////////////////////////////////////////////////////////////////////// input id buffer 
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_3))->GetWindowText(mstr_id_buffer_info_new[nPos+ 9][0]);	
	st_idbuff_info[nPos+ 9].n_idbuff_num[0] = atoi(mstr_id_buffer_info_new[nPos+ 9][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_3))->GetWindowText(mstr_id_buffer_info_new[nPos+ 9][1]);	
	st_idbuff_info[nPos+ 9].n_idbuff_num[1] = atoi(mstr_id_buffer_info_new[nPos+ 9][1]);
	
	////////////////////////////////////////////////////////////////////////// var 
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_LEFT_5))->GetWindowText(mstr_id_buffer_info_new[nPos+ 9][0]);	
	sprintf(st_idbuff_info[nPos+ 9].c_var[0], "%s", mstr_id_buffer_info_new[nPos+ 9][0]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_IDBUFFER_INFO_RIGHT_5))->GetWindowText(mstr_id_buffer_info_new[nPos+ 9][1]);	
	sprintf(st_idbuff_info[nPos+ 9].c_var[1], "%s", mstr_id_buffer_info_new[nPos+ 9][1]);

}

////2014.1115
void CDialog_IDBuffer_Info::OnRun_Step_Input_Info(int nPos)//2014.1119
{
// 	CString sTmp;
// 	
// 	if(nPos == 0)
// 	{
// 		m_msg_run_step_1.SetWindowText("Run_IDBuffer_Input_Index"); 
// 		m_msg_run_step_2.SetWindowText("main"); 
// 		m_msg_run_step_3.SetWindowText("idbuffermove_loader_mc_in_clamp_process"); 
// 		m_msg_run_step_4.SetWindowText("idbuffermove_sealing_mc_in_move_process"); 
// 		m_msg_run_step_5.SetWindowText("smema_process_top_8570"); 
// 		
// 		////////////////////////////////////////////////////////////////////////// Run_IDBuffer_Input_Index
// 		sTmp.Format("%d", st_step.n_main[STEP_INPUT_INDEX]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_INPUT_INDEX_LOADER]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_INPUT_INDEX_SEALING]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_3))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_SMEMA_TOP_8570]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->SetWindowText(sTmp);	
// 	}
// 	else if(nPos == 1 || nPos == 2 || nPos == 3 || nPos == 4)
// 	{
// 		m_msg_run_step_1.SetWindowText("Run_IDBuffer_In_Set_Plate"); 
// 		m_msg_run_step_2.SetWindowText("main"); 
// 		m_msg_run_step_3.ShowWindow(SW_HIDE); m_edit_run_step_2.ShowWindow(SW_HIDE);
// 		m_msg_run_step_4.ShowWindow(SW_HIDE); m_edit_run_step_3.ShowWindow(SW_HIDE);
// 		m_msg_run_step_5.ShowWindow(SW_HIDE); m_edit_run_step_4.ShowWindow(SW_HIDE);
// 		////////////////////////////////////////////////////////////////////////// Run_IDBuffer_In_Set_Plate
// 		sTmp.Format("%d", st_step.n_main[STEP_SET_PLATE]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->SetWindowText(sTmp);	
// 	}
// 	else if(nPos == 5)
// 	{
// 		m_msg_run_step_1.SetWindowText("Run_IDBuffer_Outputbuffer_Clamp"); 
// 		m_msg_run_step_2.SetWindowText("main"); 
// 		m_msg_run_step_3.ShowWindow(SW_HIDE); m_edit_run_step_2.ShowWindow(SW_HIDE); 
// 		m_msg_run_step_4.ShowWindow(SW_HIDE); m_edit_run_step_3.ShowWindow(SW_HIDE);
// 		m_msg_run_step_5.SetWindowText("smema_process_top_8590");  
// 		////////////////////////////////////////////////////////////////////////// Run_IDBuffer_Outputbuffer_Clamp
// 		sTmp.Format("%d", st_step.n_main[STEP_OUTBUFF_CLAMP]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_SMEMA_TOP_8590]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->SetWindowText(sTmp);	
// 	}
// 	else if(nPos == 6)
// 	{
// 		m_msg_run_step_1.SetWindowText("Run_BTM_Move_Index"); 
// 		m_msg_run_step_2.SetWindowText("main"); 
// 		m_msg_run_step_3.SetWindowText("idbuffer_move_index_labelMC_process"); 
// 		m_msg_run_step_4.ShowWindow(SW_HIDE); m_edit_run_step_3.ShowWindow(SW_HIDE);
// 		m_msg_run_step_5.SetWindowText("smema_process_btm_8590"); 
// 		////////////////////////////////////////////////////////////////////////// Run BTM Move Index 
// 		sTmp.Format("%d", st_step.n_main[STEP_BTM_MOVE_INDEX]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_BTM_MV_INDDX_LABEL_MC]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_SMEMA_BTM_8590]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->SetWindowText(sTmp);	
// 		
// 	}
// 	else if(nPos == 7)
// 	{
// 		m_msg_run_step_1.SetWindowText("Run_BTM_Move_Index"); 
// 		m_msg_run_step_2.SetWindowText("main"); 
// 		m_msg_run_step_3.SetWindowText("idbuff_move_index_loaderMC_process"); 
// 		m_msg_run_step_4.ShowWindow(SW_HIDE); m_edit_run_step_3.ShowWindow(SW_HIDE);
// 		m_msg_run_step_5.SetWindowText("smema_process_btm_8570"); 
// 		////////////////////////////////////////////////////////////////////////// Run BTM Move Index 
// 		sTmp.Format("%d", st_step.n_main[STEP_BTM_MOVE_INDEX]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_BTM_MV_INDDX_LOADER_MC]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_SMEMA_BTM_8570]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->SetWindowText(sTmp);	
// 	}
// 	else if(nPos == 8 || nPos == 9)
// 	{
// 		m_msg_run_step_1.SetWindowText("Run_BTM_Move_Index"); 
// 		m_msg_run_step_2.SetWindowText("main"); 
// 		m_msg_run_step_3.SetWindowText("idbuffer_move_index_control_process"); 
// 		m_msg_run_step_4.ShowWindow(SW_HIDE); m_edit_run_step_3.ShowWindow(SW_HIDE);
// 		m_msg_run_step_5.ShowWindow(SW_HIDE); m_edit_run_step_4.ShowWindow(SW_HIDE);
// 		////////////////////////////////////////////////////////////////////////// Run BTM Move Index 
// 		sTmp.Format("%d", st_step.n_main[STEP_BTM_MOVE_INDEX]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->SetWindowText(sTmp);	
// 		sTmp.Format("%d", st_step.n_sub[sb_BTM_MV_INDDX]);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->SetWindowText(sTmp);	
// 	}


}

void CDialog_IDBuffer_Info::OnRun_Step_Output_Info(int nPos)//2014.1119
{
// 	CString sTmp;
// 	
// 	if(nPos == 0)
// 	{
// 		////////////////////////////////////////////////////////////////////////// Run_IDBuffer_Input_Index
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->GetWindowText(sTmp);	
// 		st_step.n_main[STEP_INPUT_INDEX] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_INPUT_INDEX_LOADER] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_3))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_INPUT_INDEX_SEALING] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_SMEMA_TOP_8570] = atoi(sTmp);
// 	}
// 	else if(nPos == 1 || nPos == 2 || nPos == 3 || nPos == 4)
// 	{
// 		////////////////////////////////////////////////////////////////////////// Run_IDBuffer_In_Set_Plate
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->GetWindowText(sTmp);	
// 		st_step.n_main[STEP_SET_PLATE] = atoi(sTmp);
// 	}
// 	else if(nPos == 5)
// 	{
// 		////////////////////////////////////////////////////////////////////////// Run_IDBuffer_Outputbuffer_Clamp
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->GetWindowText(sTmp);	
// 		st_step.n_main[STEP_OUTBUFF_CLAMP] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_SMEMA_TOP_8590] = atoi(sTmp);
// 	}
// 	else if(nPos == 6)
// 	{
// 		////////////////////////////////////////////////////////////////////////// Run BTM Move Index 
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->GetWindowText(sTmp);	
// 		st_step.n_main[STEP_BTM_MOVE_INDEX] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_BTM_MV_INDDX_LABEL_MC] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_SMEMA_BTM_8590] = atoi(sTmp);
// 	}
// 	else if(nPos == 7)
// 	{
// 		////////////////////////////////////////////////////////////////////////// Run BTM Move Index 
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->GetWindowText(sTmp);	
// 		st_step.n_main[STEP_BTM_MOVE_INDEX] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_BTM_MV_INDDX_LOADER_MC] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_4))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_SMEMA_BTM_8570] = atoi(sTmp);
// 	}
// 	else if(nPos == 8 || nPos == 9)
// 	{
// 		////////////////////////////////////////////////////////////////////////// Run BTM Move Index 
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_1))->GetWindowText(sTmp);	
// 		st_step.n_main[STEP_BTM_MOVE_INDEX] = atoi(sTmp);
// 		((CEdit*)GetDlgItem(IDC_EDIT_RUN_STEP_2))->GetWindowText(sTmp);	
// 		st_step.n_sub[sb_BTM_MV_INDDX] = atoi(sTmp);
// 	}



}
////

void CDialog_IDBuffer_Info::OnIDBuffer_Show_Enable()
{
	m_msg_run_step_1.ShowWindow(SW_SHOW);
	m_msg_run_step_2.ShowWindow(SW_SHOW);
	m_msg_run_step_3.ShowWindow(SW_SHOW);
	m_msg_run_step_4.ShowWindow(SW_SHOW);
	m_msg_run_step_5.ShowWindow(SW_SHOW);

	m_edit_run_step_1.ShowWindow(SW_SHOW);
	m_edit_run_step_2.ShowWindow(SW_SHOW);
	m_edit_run_step_3.ShowWindow(SW_SHOW);
	m_edit_run_step_4.ShowWindow(SW_SHOW);
}

BOOL CDialog_IDBuffer_Info::PreTranslateMessage(MSG* pMsg) 
{
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
