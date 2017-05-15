// Dialog_HandBarcode.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_HandBarcode.h"
#include "Dialog_Keyboard.h"
#include "Dialog_Select.h"
#include "AMTRegistry.h"
#include "AMTLotManager.h"
#include "SrcPart\DataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_HandBarcode dialog

CDialog_HandBarcode::CDialog_HandBarcode(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_HandBarcode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_HandBarcode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "HandBarcode Input";
}


void CDialog_HandBarcode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_HandBarcode)
	DDX_Control(pDX, IDC_RADIO_DEVICE_TYPE_3, m_radio_device_type_3);
	DDX_Control(pDX, IDC_RADIO_DEVICE_TYPE_2, m_radio_device_type_2);
	DDX_Control(pDX, IDC_RADIO_DEVICE_TYPE_1, m_radio_device_type_1);
	DDX_Control(pDX, IDC_COMBO_LABEL3_CAPA, m_cbolabel3capa);
	DDX_Control(pDX, IDC_COMBO_LABEL2_CAPA, m_cbolabel2capa);
	DDX_Control(pDX, IDC_COMBO_LABEL1_CAPA, m_cbolabel1capa);
	DDX_Control(pDX, IDC_GROUP_DEVICE_TYPE, m_group_device_type);
	DDX_Control(pDX, IDC_BTN_APPLY2, m_btn_denapply);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_bcrapply);
	DDX_Control(pDX, IDC_GROUP_HANDBARCODE1, m_group_handbarcode1);
	DDX_Control(pDX, IDC_GROUP_HANDBARCODE2, m_group_handbarcode2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_HandBarcode, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_HandBarcode)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_APPLY2, OnBtnApply2)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL1_CAPA, OnSelchangeComboLabel1Capa)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL2_CAPA, OnSelchangeComboLabel2Capa)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL3_CAPA, OnSelchangeComboLabel3Capa)
	ON_BN_CLICKED(IDC_BTN_APPLY3, OnBtnApply3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_HandBarcode message handlers
BOOL CDialog_HandBarcode::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// Start Switch의 동작을 막는다.
	if (st_handler.mn_switch_lock == FALSE)
	{
		st_handler.mn_switch_lock = TRUE;
		mn_dialog = 1;
	}
	else	mn_dialog = 0;

	LoadLabelData();
	g_dataMgr.BackupDatas();

	OnHandBarcode_GroupBox_Set();
	OnHandBarcode_EditBox_Set();
	OnHandBarcode_Data_Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_HandBarcode::OnDestroy() 
{
	if (mn_dialog == 1)
	{
		st_handler.mn_switch_lock = FALSE;
	}

	StandardDialog::OnDestroy();
}

void CDialog_HandBarcode::OnHandBarcode_GroupBox_Set()
{
	CSxLogFont maintenance_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

	m_group_handbarcode1.SetFont(maintenance_font);
	m_group_handbarcode1.SetCatptionTextColor(RGB(145,25,0));
	m_group_handbarcode1.SetFontBold(TRUE);

	m_group_handbarcode2.SetFont(maintenance_font);
	m_group_handbarcode2.SetCatptionTextColor(RGB(145,25,0));
	m_group_handbarcode2.SetFontBold(TRUE);

	m_group_device_type.SetFont(maintenance_font);
	m_group_device_type.SetCatptionTextColor(RGB(145,25,0));
	m_group_device_type.SetFontBold(TRUE);

}

void CDialog_HandBarcode::OnHandBarcode_EditBox_Set()
{	
	m_edit_new_barcode_info_1.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_1, this);
	m_edit_new_barcode_info_1.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_1.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_1.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_new_barcode_info_2.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_2, this);
	m_edit_new_barcode_info_2.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_2.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_2.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_3.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_3, this);
	m_edit_new_barcode_info_3.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_3.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_3.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_4.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_4, this);
	m_edit_new_barcode_info_4.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_4.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_4.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_5.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_5, this);
	m_edit_new_barcode_info_5.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_5.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_5.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_6.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_6, this);
	m_edit_new_barcode_info_6.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_6.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_3.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_new_barcode_info_7.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_7, this);
	m_edit_new_barcode_info_7.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_7.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_7.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_8.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_8, this);
	m_edit_new_barcode_info_8.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_8.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_8.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_9.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_9, this);
	m_edit_new_barcode_info_9.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_9.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_9.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_new_barcode_info_10.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_10, this);
	m_edit_new_barcode_info_10.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_10.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_10.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_11.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_11, this);
	m_edit_new_barcode_info_11.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_11.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_11.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_12.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_12, this);
	m_edit_new_barcode_info_12.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_12.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_12.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_new_barcode_info_13.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_13, this);
	m_edit_new_barcode_info_13.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_13.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_13.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	m_edit_new_barcode_info_14.SubclassDlgItem(IDC_EDIT_NEW_BARCODE_INFO_14, this);
	m_edit_new_barcode_info_14.bkColor(RGB(50, 100, 150));
	m_edit_new_barcode_info_14.textColor(RGB(255, 255,255));
	m_edit_new_barcode_info_14.setFont(-15, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
}


void CDialog_HandBarcode::OnHandBarcode_Data_Display()
{
	CString strVal="";

	//2015.0523
	int DeviceType = g_dataMgr.GetBackupData( &st_basic.mn_device_type );
	if (DeviceType < 0 || DeviceType > 2)
	{
		m_cbolabel1capa.EnableWindow(FALSE);
		m_cbolabel2capa.EnableWindow(FALSE);
		m_cbolabel3capa.EnableWindow(FALSE);
	}
	else
	{
		if (DeviceType == 0)	  m_cbolabel1capa.EnableWindow(FALSE);
		else if (DeviceType == 1) m_cbolabel2capa.EnableWindow(FALSE);
		else					  m_cbolabel3capa.EnableWindow(FALSE);
	}

	for ( int j = 0; j < 6; j++)
	{
		strVal.Format( "DENSITY_%02d", j );
		st_handler.m_strDensityLabel[j] = AMTRegistry::RegReadString( REG_VAL_DENSITY, strVal, "" ).c_str();
	}

	//용량라벨
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_1))->SetWindowText(st_handler.m_strDensityLabel[0]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_2))->SetWindowText(st_handler.m_strDensityLabel[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_3))->SetWindowText(st_handler.m_strDensityLabel[2]);	
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_4))->SetWindowText(st_handler.m_strDensityLabel[3]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_5))->SetWindowText(st_handler.m_strDensityLabel[4]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_6))->SetWindowText(st_handler.m_strDensityLabel[5]);
	

	for ( int k = 0; k < 4; k++)
	{
		strVal.Format( "BCR_LABEL_%02d", k );
		st_handler.m_strDistributionLabel[k] = AMTRegistry::RegReadString( REG_VAL_BCR, strVal, "" ).c_str();
		strVal.Format( "BCR_RIBBON_%02d", k );
		st_handler.m_strDistributionRibbon[k] = AMTRegistry::RegReadString( REG_VAL_BCR, strVal, "" ).c_str();
	}

	//유통라벨
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_7))->SetWindowText(st_handler.m_strDistributionLabel[0]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_8))->SetWindowText(st_handler.m_strDistributionRibbon[0]);

	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_9))->SetWindowText(st_handler.m_strDistributionLabel[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_10))->SetWindowText(st_handler.m_strDistributionRibbon[1]);

	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_11))->SetWindowText(st_handler.m_strDistributionLabel[2]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_12))->SetWindowText(st_handler.m_strDistributionRibbon[2]);

	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_13))->SetWindowText(st_handler.m_strDistributionLabel[3]);
	((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_14))->SetWindowText(st_handler.m_strDistributionRibbon[3]);
}
void CDialog_HandBarcode::OnHandBarcode_LogFile_Create()
{
	CString str_msg="";
	CString str_data="";
	CString str_mode="";
	int Ret = RET_GOOD;
	
	if (mstr_new_barcode_info_1 != "")
	{
		str_msg = "[HandBarcode] Density1 Label was changed by " + mstr_new_barcode_info_1;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_2 != "")
	{
		str_msg = "[HandBarcode] Density2 Label was changed by " + mstr_new_barcode_info_2;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_3 != "")
	{
		str_msg = "[HandBarcode] Density3 Label was changed by " + mstr_new_barcode_info_3;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_4 != "")
	{
		str_msg = "[HandBarcode] Density4 Label was changed by " + mstr_new_barcode_info_4;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_5 != "")
	{
		str_msg = "[HandBarcode] Density5 Label was changed by " + mstr_new_barcode_info_5;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_6 != "")
	{
		str_msg = "[HandBarcode] Density6 Label was changed by " + mstr_new_barcode_info_6;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_7 != "")
	{
		str_msg = "[HandBarcode] Distribution1 Label was changed by " + mstr_new_barcode_info_7;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_8 != "")
	{
		str_msg = "[HandBarcode] Distribution1 Ribbon was changed by " + mstr_new_barcode_info_9;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_9 != "")
	{
		str_msg = "[HandBarcode] Distribution2 Label was changed by " + mstr_new_barcode_info_9;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_10 != "")
	{
		str_msg = "[HandBarcode] Distribution2 Ribbon was changed by " + mstr_new_barcode_info_10;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_11 != "")
	{
		str_msg = "[HandBarcode] Distribution3 Label was changed by " + mstr_new_barcode_info_11;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_12 != "")
	{
		str_msg = "[HandBarcode] Distribution3 Ribbon was changed by " + mstr_new_barcode_info_12;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_13 != "")
	{
		str_msg = "[HandBarcode] Distribution4 Label was changed by " + mstr_new_barcode_info_13;
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mstr_new_barcode_info_14 != "")
	{
		str_msg = "[HandBarcode] Distribution4 Ribbon was changed by " + mstr_new_barcode_info_14;
		Func.On_LogFile_Add(99, str_msg);
	}


}

void CDialog_HandBarcode::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	KillTimer(TM_DENSITY_CCS);
	KillTimer(TM_BCR_CCS);
	CDialog::OnOK();
}



void CDialog_HandBarcode::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	int n_response;  // 대화 상자 리턴 플래그
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Dialog_HandBarcode : Change Data Apply?");
	n_response = select_dlg.DoModal();
	if (n_response == IDOK)
	{
		//유통라벨
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_7))->GetWindowText(mstr_new_barcode_info_7);
		mstr_new_barcode_info_7.MakeUpper();
		mstr_new_barcode_info_7.TrimLeft();
		mstr_new_barcode_info_7.TrimRight();
		if( mstr_new_barcode_info_7.GetLength() > 11 )
			mstr_new_barcode_info_7 = mstr_new_barcode_info_7.Left(11);
		st_handler.m_strDistributionLabel[0] = mstr_new_barcode_info_7;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_7))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_7))->SetWindowText(st_handler.m_strDistributionLabel[0]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_8))->GetWindowText(mstr_new_barcode_info_8);
		mstr_new_barcode_info_8.MakeUpper();
		mstr_new_barcode_info_8.TrimLeft();
		mstr_new_barcode_info_8.TrimRight();
		if( mstr_new_barcode_info_8.GetLength() > 11 )
			mstr_new_barcode_info_8 = mstr_new_barcode_info_8.Left(11);
		st_handler.m_strDistributionRibbon[0] = mstr_new_barcode_info_8;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_8))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_8))->SetWindowText(st_handler.m_strDistributionRibbon[0]);

		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_9))->GetWindowText(mstr_new_barcode_info_9);
		mstr_new_barcode_info_9.MakeUpper();
		mstr_new_barcode_info_9.TrimLeft();
		mstr_new_barcode_info_9.TrimRight();
		if( mstr_new_barcode_info_9.GetLength() > 11 )
			mstr_new_barcode_info_9 = mstr_new_barcode_info_9.Left(11);
		st_handler.m_strDistributionLabel[1] = mstr_new_barcode_info_9;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_9))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_9))->SetWindowText(st_handler.m_strDistributionLabel[1]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_10))->GetWindowText(mstr_new_barcode_info_10);
		mstr_new_barcode_info_10.MakeUpper();
		mstr_new_barcode_info_10.TrimLeft();
		mstr_new_barcode_info_10.TrimRight();
		if( mstr_new_barcode_info_10.GetLength() > 11 )
			mstr_new_barcode_info_10 = mstr_new_barcode_info_10.Left(11);
		st_handler.m_strDistributionRibbon[1] = mstr_new_barcode_info_10;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_10))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_10))->SetWindowText(st_handler.m_strDistributionRibbon[1]);

		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_11))->GetWindowText(mstr_new_barcode_info_11);
		mstr_new_barcode_info_11.MakeUpper();
		mstr_new_barcode_info_11.TrimLeft();
		mstr_new_barcode_info_11.TrimRight();
		if( mstr_new_barcode_info_11.GetLength() > 11 )
			mstr_new_barcode_info_11 = mstr_new_barcode_info_11.Left(11);
		st_handler.m_strDistributionLabel[2] = mstr_new_barcode_info_11;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_11))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_11))->SetWindowText(st_handler.m_strDistributionLabel[2]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_12))->GetWindowText(mstr_new_barcode_info_12);
		mstr_new_barcode_info_12.MakeUpper();
		mstr_new_barcode_info_12.TrimLeft();
		mstr_new_barcode_info_12.TrimRight();
		if( mstr_new_barcode_info_12.GetLength() > 11 )
			mstr_new_barcode_info_12 = mstr_new_barcode_info_12.Left(11);
		st_handler.m_strDistributionRibbon[2] = mstr_new_barcode_info_12;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_12))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_12))->SetWindowText(st_handler.m_strDistributionRibbon[2]);

		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_13))->GetWindowText(mstr_new_barcode_info_13);
		mstr_new_barcode_info_13.MakeUpper();
		mstr_new_barcode_info_13.TrimLeft();
		mstr_new_barcode_info_13.TrimRight();
		if( mstr_new_barcode_info_13.GetLength() > 11 )
			mstr_new_barcode_info_13 = mstr_new_barcode_info_13.Left(11);
		st_handler.m_strDistributionLabel[3] = mstr_new_barcode_info_13;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_13))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_13))->SetWindowText(st_handler.m_strDistributionLabel[3]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_14))->GetWindowText(mstr_new_barcode_info_14);
		mstr_new_barcode_info_14.MakeUpper();
		mstr_new_barcode_info_14.TrimLeft();
		mstr_new_barcode_info_14.TrimRight();
		if( mstr_new_barcode_info_14.GetLength() > 11 )
			mstr_new_barcode_info_14 = mstr_new_barcode_info_14.Left(11);
		st_handler.m_strDistributionRibbon[3] = mstr_new_barcode_info_14;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_14))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_14))->SetWindowText(st_handler.m_strDistributionRibbon[3]);


		if( g_lotMgr.GetLotCount() > 0 && st_basic.mn_mode_run == CTL_YES)
		{
			st_var.n_densityccs = CTL_READY;
			st_var.n_bcrccs = CTL_READY;
		}

		//2014,1227		
		CString strVal="";
		CString strValue="";		
		
		for( int k=0; k<4; k++ )
		{
			strValue.Format("BCR_LABEL_%02d", k );
			strVal.Format("%s",st_handler.m_strDistributionLabel[k]);
			AMTRegistry::RegWriteString( REG_VAL_BCR, strValue, strVal );


			strValue.Format("BCR_RIBBON_%02d", k );
			strVal.Format("%s",st_handler.m_strDistributionRibbon[k]);
			AMTRegistry::RegWriteString( REG_VAL_BCR, strValue, strVal );
			
		}

		if(mstr_new_barcode_info_7 != mstr_new_barcode_info_9 || mstr_new_barcode_info_11 != mstr_new_barcode_info_13 ||
			mstr_new_barcode_info_7 != mstr_new_barcode_info_11)
		{
			CString sTmp;
			if(mstr_new_barcode_info_7 != mstr_new_barcode_info_9)
			{
				sTmp = _T("BcrLabel1 and BcrLabel2 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			else if(mstr_new_barcode_info_11 != mstr_new_barcode_info_13)
			{
				sTmp = _T("BcrLabel3 and BcrLabel4 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			else
			{
				sTmp = _T("BcrLabel1 and BcrLabel3 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			
		}
		if(mstr_new_barcode_info_8 != mstr_new_barcode_info_10 || mstr_new_barcode_info_12 != mstr_new_barcode_info_14 ||
			mstr_new_barcode_info_10 != mstr_new_barcode_info_14)
		{
			CString sTmp;
			if(mstr_new_barcode_info_8 != mstr_new_barcode_info_10)
			{
				sTmp = _T("BcrRibbon1 and BcrRibbon2 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			else if(mstr_new_barcode_info_12 != mstr_new_barcode_info_14)
			{
				sTmp = _T("BcrRibbon3 and BcrRibbon4 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			else
			{
				sTmp = _T("BcrRibbon2 and BcrRibbon4 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			
		}
		m_nLabelpos = 0;
		mn_ServerStep = 0;
		m_btn_bcrapply.EnableWindow( TRUE );
		m_btn_denapply.EnableWindow( TRUE );
		//SetTimer( TM_BCR_CCS , 100, 0);


		OnHandBarcode_LogFile_Create();
	}
}

BOOL CDialog_HandBarcode::PreTranslateMessage(MSG* pMsg) 
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

void CDialog_HandBarcode::OnBtnApply2() 
{
	int n_response;  // 대화 상자 리턴 플래그
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Dialog_HandBarcode : Change Data Apply?");
	n_response = select_dlg.DoModal();
	if (n_response == IDOK)
	{	//용량라벨					
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_1))->GetWindowText(mstr_new_barcode_info_1);
		mstr_new_barcode_info_1.MakeUpper();
		mstr_new_barcode_info_1.TrimLeft();
		mstr_new_barcode_info_1.TrimRight();
		if( mstr_new_barcode_info_1.GetLength() > 11 )
			mstr_new_barcode_info_1 = mstr_new_barcode_info_1.Left(11);
		st_handler.m_strDensityLabel[0] = mstr_new_barcode_info_1;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_1))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_1))->SetWindowText(st_handler.m_strDensityLabel[0]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_2))->GetWindowText(mstr_new_barcode_info_2);
		mstr_new_barcode_info_2.MakeUpper();
		mstr_new_barcode_info_2.TrimLeft();
		mstr_new_barcode_info_2.TrimRight();
		if( mstr_new_barcode_info_2.GetLength() > 11 )
			mstr_new_barcode_info_2 = mstr_new_barcode_info_2.Left(11);
		st_handler.m_strDensityLabel[1] = mstr_new_barcode_info_2;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_2))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_2))->SetWindowText(st_handler.m_strDensityLabel[1]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_3))->GetWindowText(mstr_new_barcode_info_3);
		mstr_new_barcode_info_3.MakeUpper();
		mstr_new_barcode_info_3.TrimLeft();
		mstr_new_barcode_info_3.TrimRight();
		if( mstr_new_barcode_info_3.GetLength() > 11 )
			mstr_new_barcode_info_3 = mstr_new_barcode_info_3.Left(11);
		st_handler.m_strDensityLabel[2] = mstr_new_barcode_info_3;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_3))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_3))->SetWindowText(st_handler.m_strDensityLabel[2]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_4))->GetWindowText(mstr_new_barcode_info_4);
		mstr_new_barcode_info_4.MakeUpper();
		mstr_new_barcode_info_4.TrimLeft();
		mstr_new_barcode_info_4.TrimRight();
		if( mstr_new_barcode_info_4.GetLength() > 11 )
			mstr_new_barcode_info_4 = mstr_new_barcode_info_4.Left(11);
		st_handler.m_strDensityLabel[3] = mstr_new_barcode_info_4;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_4))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_4))->SetWindowText(st_handler.m_strDensityLabel[3]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_5))->GetWindowText(mstr_new_barcode_info_5);
		mstr_new_barcode_info_5.MakeUpper();
		mstr_new_barcode_info_5.TrimLeft();
		mstr_new_barcode_info_5.TrimRight();
		if( mstr_new_barcode_info_5.GetLength() > 11 )
			mstr_new_barcode_info_5 = mstr_new_barcode_info_5.Left(11);
		st_handler.m_strDensityLabel[4] = mstr_new_barcode_info_5;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_5))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_5))->SetWindowText(st_handler.m_strDensityLabel[4]);
		
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_6))->GetWindowText(mstr_new_barcode_info_6);
		mstr_new_barcode_info_6.MakeUpper();
		mstr_new_barcode_info_6.TrimLeft();
		mstr_new_barcode_info_6.TrimRight();
		if( mstr_new_barcode_info_6.GetLength() > 11 )
			mstr_new_barcode_info_6 = mstr_new_barcode_info_6.Left(11);
		st_handler.m_strDensityLabel[5] = mstr_new_barcode_info_6;
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_6))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_EDIT_NEW_BARCODE_INFO_6))->SetWindowText(st_handler.m_strDensityLabel[5]);
		
		
		//2014,1227		
		CString strVal="";
		CString strValue="";		
		for( int j=0; j<6; j++ )
		{
			strValue.Format("DENSITY_%02d", j );
			strVal.Format("%s",st_handler.m_strDensityLabel[j]);
			AMTRegistry::RegWriteString( REG_VAL_DENSITY, strValue, strVal );
			
		}

		if(g_lotMgr.GetLotCount() > 0 && st_basic.mn_mode_run == CTL_YES)
		{

			st_var.n_densityccs = CTL_READY;
			st_var.n_bcrccs = CTL_READY;
		}

		if(mstr_new_barcode_info_1 != mstr_new_barcode_info_4 || mstr_new_barcode_info_2 != mstr_new_barcode_info_5 ||
			mstr_new_barcode_info_3 != mstr_new_barcode_info_6)
		{
			CString sTmp;
			if(mstr_new_barcode_info_1 != mstr_new_barcode_info_4)
			{
				sTmp = _T("Label1 and Label4 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}

			}
			else if(mstr_new_barcode_info_2 != mstr_new_barcode_info_5)
			{
				sTmp = _T("Label2 and Label5 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}
			else
			{
				sTmp = _T("Label3 and Label6 is not same");
				if (st_handler.cwnd_list != NULL)
				{
					sprintf( st_msg.c_abnormal_msg, "%s", sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
			}

			return;

		}

		m_nLabelpos = 0;
		mn_ServerStep = 0;
		m_btn_bcrapply.EnableWindow( TRUE );
		m_btn_denapply.EnableWindow( TRUE );
		//SetTimer( TM_DENSITY_CCS , 100, 0);
		
		
		OnHandBarcode_LogFile_Create();
	}	
}


int CDialog_HandBarcode::OnServerInterace( int nInterface)//nInterface:1 density label, 21: distribution label
{
	int nFunRet = CTLBD_RET_PROCEED;

	switch(mn_ServerStep)
	{
	case 0:
		mn_ServerStep = 10;
		m_strRrtry = "NO";
		m_strCnt = "0";
		m_strMode = "NEW";
		m_nRetryCnt = 0;
		
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

	case 50:
		if(st_client[CLS_ECSERVER].n_connect == CTL_NO )
		{
			
			m_dwServerTime = GetCurrentTime();
			mn_ServerStep = 100;
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, 0);
			if( m_nRetryCnt >= 3 ) m_nRetryCnt = 1;
			m_strRrtry = "YES";
			m_strCnt.Format("%d", m_nRetryCnt);
			m_strMode = "NEW";
		}
		else
		{
			mn_ServerStep = 100;
		}
		break;

	case 100:
		if(nInterface == 1)
			clsBcrFormatClient.OnBrandDensityLabelCCS( 0, g_lotMgr.GetLotAt(0).GetLotID(), "0", "", st_handler.m_strDensityLabel[m_nLabelpos], m_strRrtry, m_strCnt);
		else if(nInterface == 2)
			clsBcrFormatClient.OnBrandDTBTLabelCCS(0, g_lotMgr.GetLotAt(0).GetLotID(), "0" , "", st_handler.m_strDistributionLabel[m_nLabelpos], st_handler.m_strDistributionRibbon[m_nLabelpos], m_strRrtry, m_strCnt);
		
		mn_ServerStep = 200;
		m_dwServerTime = GetCurrentTime();
		break;
		

	case 200:
		if(GetCurrentTime() - m_dwServerTime < 0 ) m_dwServerTime = GetCurrentTime();
		if(nInterface == 1)
		{
			if( clsBcrFormatClient.m_nrDensityCCS == NR_RECEIVED )
			{
				mn_ServerStep = 1000;
			}
			else if( clsBcrFormatClient.m_nrDensityCCS == NR_FAIL )
			{
				mn_ServerStep = 1000;
			}
		}
		else if(nInterface == 2)
		{
			if( clsBcrFormatClient.m_nrBcrLabelCCS == NR_RECEIVED )
			{
				mn_ServerStep = 1000;
			}
			else if( clsBcrFormatClient.m_nrBcrLabelCCS == NR_FAIL )
			{
				mn_ServerStep = 1000;
			}
		}
		if(GetCurrentTime() - m_dwServerTime > 3000)
		{
			m_nRetryCnt++;
			mn_ServerStep = 500;
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CLOSE, 0);
			m_dwServerTime = GetCurrentTime();
		}
		break;

	case 500:
		if(GetCurrentTime() - m_dwServerTime < 0 ) m_dwServerTime = GetCurrentTime();
		if(GetCurrentTime() - m_dwServerTime > 3000)
		{
			mn_ServerStep = 50;

		}
		break;

		
	case 1000:
		nFunRet = CTLBD_RET_GOOD;
		mn_ServerStep = 0;
		break;
	}


	return nFunRet;

}

void CDialog_HandBarcode::OnTimer(UINT nIDEvent) 
{
	int nRet = CTL_PROCEED;

	if( nIDEvent == TM_DENSITY_CCS )
	{
		nRet = OnServerInterace( 1 );
		if( nRet == CTL_GOOD)
		{
			m_nLabelpos++;
			mn_ServerStep = 0;
			if(m_nLabelpos >= 6)
			{
				KillTimer( TM_DENSITY_CCS );
				m_btn_bcrapply.EnableWindow( TRUE );
				m_btn_denapply.EnableWindow( TRUE );
			}
			else				 mn_ServerStep = 0;

		}
		else if( nRet == CTL_ERROR)
		{
			KillTimer( TM_DENSITY_CCS );
			m_btn_bcrapply.EnableWindow( TRUE );
			m_btn_denapply.EnableWindow( TRUE );
		}
	}
	else if( nIDEvent == TM_BCR_CCS )
	{
		nRet = OnServerInterace( 2 );
		if( nRet == CTL_GOOD)
		{
			m_nLabelpos++;
			mn_ServerStep = 0;
			if(m_nLabelpos >= 4)
			{
				KillTimer( TM_BCR_CCS );
				m_btn_bcrapply.EnableWindow( TRUE );
				m_btn_denapply.EnableWindow( TRUE );
			}
			else
			{
				KillTimer( TM_BCR_CCS );
				m_btn_bcrapply.EnableWindow( TRUE );
				m_btn_denapply.EnableWindow( TRUE );
			}
		}
	}
		 
	
	StandardDialog::OnTimer(nIDEvent);
}

void CDialog_HandBarcode::LoadLabelData()
{
	Func.LoadRegLabelCapaData();
	for( int i=0; i< st_handler.m_nTotal; i++ )
	{
		if(st_handler.m_strLabelCapa[i] != "")
		{
			m_cbolabel1capa.AddString(st_handler.m_strLabelCapa[i]);
			m_cbolabel2capa.AddString(st_handler.m_strLabelCapa[i]);
			m_cbolabel3capa.AddString(st_handler.m_strLabelCapa[i]);
		}
	}
	m_cbolabel1capa.SetWindowText(st_basic.n_str_capa1label);
	m_cbolabel2capa.SetWindowText(st_basic.n_str_capa2label);
	m_cbolabel3capa.SetWindowText(st_basic.n_str_capa3label);
}

void CDialog_HandBarcode::OnSelchangeComboLabel1Capa() 
{
	CString sText="";
	m_cbolabel1capa.GetWindowText(sText);
	g_dataMgr.SetDataTemp( &st_basic.n_str_capa1label, sText );	
	
}

void CDialog_HandBarcode::OnSelchangeComboLabel2Capa() 
{
	CString sText="";
	m_cbolabel2capa.GetWindowText(sText);
	
	g_dataMgr.SetDataTemp( &st_basic.n_str_capa2label, sText );	
	
}

void CDialog_HandBarcode::OnSelchangeComboLabel3Capa() 
{
	CString sText="";
	m_cbolabel3capa.GetWindowText(sText);
	g_dataMgr.SetDataTemp( &st_basic.n_str_capa3label, sText );	
	
}

void CDialog_HandBarcode::OnBtnApply3() 
{
	CString strVal="";
	CString strValue="";

	g_dataMgr.BackupDatas();
	
	m_cbolabel1capa.GetWindowText(st_basic.n_str_capa1label);
	m_cbolabel2capa.GetWindowText(st_basic.n_str_capa2label);
	m_cbolabel3capa.GetWindowText(st_basic.n_str_capa3label);

	m_cbolabel1capa.SetWindowText(st_basic.n_str_capa1label);
	m_cbolabel2capa.SetWindowText(st_basic.n_str_capa2label);
	m_cbolabel3capa.SetWindowText(st_basic.n_str_capa3label);

	st_handler.m_strDensityCapa[0] = st_basic.n_str_capa1label;
	st_handler.m_strDensityCapa[3] = st_handler.m_strDensityCapa[0];
	
	st_handler.m_strDensityCapa[1] = st_basic.n_str_capa2label;
	st_handler.m_strDensityCapa[4] = st_handler.m_strDensityCapa[1];
	
	st_handler.m_strDensityCapa[2] = st_basic.n_str_capa3label;
	st_handler.m_strDensityCapa[5] = st_handler.m_strDensityCapa[2];
	
	for( int j=0; j<6; j++ )
	{
		strValue.Format("CAPADENSITY_%02d", j );
		strVal.Format("%s",st_handler.m_strDensityCapa[j]);
		AMTRegistry::RegWriteString( REG_VAL_CAPADENSITY, strValue, strVal );
		
	}
	
}
