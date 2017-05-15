// Public_Function.cpp: implementation of the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Variable.h"			// Àü¿ª º¯¼ö Á¤ÀÇ Å¬·¡½º
#include "Public_Function.h"	// ¸ðÅÍ º¸µå ¹× I/O º¸µå °ü·Ã Å¬·¡½º
#include <nb30.h>
#include <wininet.h>
#include "Alg_Mysql.h"
#include "FastechPublic_IO.h"
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "Cmmsdkdef.h"
#include "AMTLotManager.h"
#include "CtlBd_Function.h"
#include "CtlBd_Library.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"

// #include "stdafx.h"
// #include "iostream.h"
// #include "afxmt.h"

#include "SrcBase\ACriticalSection.h"
#include "SrcPart/APartDatabase.h"
#include "ABpcManager.h"
#include "AMTRegistry.h"

#include "AMTRegistry.h"
#include "AVersion.h"

// ******************************************************************************
// ´ëÈ­ »óÀÚ Å¬·¡½º Ãß°¡                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************
#include "Srcbase\ALocalization.h"

#include "ZebraPrint.h"
#include "VisionClient.h"
#include "VisionServer.h"
#include "BarcodeFormat.h"
#include "InterfaceBarcode.h"
#include "InterfaceBarcode2.h"
#include "XGemClient.h"

typedef struct _ASTAT_ 
{ 

ADAPTER_STATUS adapt; 
NAME_BUFFER NameBuff [30]; 

}ASTAT, * PASTAT; 

ASTAT Adapter; 

#include "math.h"
#include "io.h"			// ÆÄÀÏ Á¸Àç À¯¹« °Ë»ç ÇÔ¼ö È£ÃâÇÏ±â À§ÇØ¼­´Â ¹Ýµå½Ã ÇÊ¿ä

//CMyBasicData     mcls_p_basic;		// ±âº» ¼ÂÆÃ Á¤º¸ ·Îµù ¹× ÀúÀå Å¬·¡½º
CMutex pmutex;
CMutex plogmutex;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_LINE	500

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPublic_Function,CObject,1);	// º¹±¸ µ¿ÀÛÀ» À§ÇÔ...2K12/02/23/ViboX

CPublic_Function  Func;				// Àü¿ª Å¬·¡½º Á¢±ÙÀÚ ¼±¾ð

CPublic_Function::CPublic_Function()
{
	Mot_RetryCount = 3;	
	mp_main_font = NULL;
	mp_main_font = new CFont;
	mp_main_font->CreateFont(15,0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "µ¸À½Ã¼");

	mp_main_big_font = NULL;
	mp_main_big_font = new CFont;
	mp_main_big_font->CreateFont(21,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"2002L");

	mp_motor_font = NULL;
	mp_motor_font = new CFont;
	mp_motor_font->CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_motorspeed_font = NULL;
	mp_motorspeed_font = new CFont;
	mp_motorspeed_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_motoraxis1_font = NULL;
	mp_motoraxis1_font = new CFont;
	mp_motoraxis1_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_motorpart_font = NULL;
	mp_motorpart_font = new CFont;
	mp_motorpart_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_iopart_font = NULL;
	mp_iopart_font = new CFont;
	mp_iopart_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_partmanual_font = NULL;
	mp_partmanual_font = new CFont;
	mp_partmanual_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_fileEditer_font = NULL;
	mp_fileEditer_font = new CFont;
	mp_fileEditer_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_pgminfo_font = NULL;
	mp_pgminfo_font = new CFont;
	mp_pgminfo_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	mp_alarm_font = NULL;
	mp_alarm_font = new CFont;
	mp_alarm_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_listdataview_font = NULL;
	mp_listdataview_font = new CFont;
	mp_listdataview_font->CreateFont(24,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_buffer_font = NULL;
	mp_buffer_font = new CFont;
	mp_buffer_font->CreateFont(15,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_exit_font = NULL;
	mp_exit_font = new CFont;
	mp_exit_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	mp_info_font = NULL;
	mp_info_font = new CFont;
	mp_info_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_board_font = NULL;
	mp_board_font = new CFont;
	mp_board_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_pad_font = NULL;
	mp_pad_font = new CFont;
	mp_pad_font->CreateFont(12,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_list_font = NULL;
	mp_list_font = new CFont;
	mp_list_font->CreateFont(14,6,0,0,900,0,0,0,0,0,0,PROOF_QUALITY,0,"MS Sans Serif");

	mp_msg_font = NULL;
	mp_msg_font = new CFont;
	mp_msg_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_change_font = NULL;
	mp_change_font = new CFont;
	mp_change_font->CreateFont(16,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_pass_font = NULL;
	mp_pass_font = new CFont;
	mp_pass_font->CreateFont(17,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_select_font = NULL;
	mp_select_font = new CFont;
	mp_select_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_title_font = NULL;
	mp_title_font = new CFont;
	mp_title_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_menu_font = NULL;
	mp_menu_font = new CFont;
	mp_menu_font->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_m_msg_font = NULL;
	mp_m_msg_font = new CFont;
	mp_m_msg_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_io_font = NULL;
	mp_io_font = new CFont;
	mp_io_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_init_font = NULL;
	mp_init_font = new CFont;
	//	mp_init_font->CreateFont(35,20,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	mp_init_font->CreateFont(35,20,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial Black Italic");

	mp_lock_font = NULL;
	mp_lock_font = new CFont;
	mp_lock_font->CreateFont(42,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial Black Italic");

	mp_wait_time_font = NULL;
	mp_wait_time_font = new CFont;
	mp_wait_time_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_maintenance_font = NULL;
	mp_maintenance_font = new CFont;
	mp_maintenance_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	
	mp_combo_font = NULL;
	mp_combo_font = new CFont;
	mp_combo_font->CreateFont(23, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Arial");

	mp_basic_font = NULL;
	mp_basic_font = new CFont;
	mp_basic_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_operation_list_font = NULL;
	mp_operation_list_font = new CFont;
	mp_operation_list_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_admin_font = NULL;
	mp_admin_font = new CFont;
	mp_admin_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	for (int i=0; i<4; i++)
	{
		m_pZebra[i]		= NULL;
		m_pZebra[i]		= new CZebraPrint;

		m_pVsClient[i] = NULL;
		m_pVsClient[i] = new CVisionClient;

		m_pVsServer[i] = NULL;
		m_pVsServer[i] = new CVisionServer;

		m_pBctFmtClient[i] = NULL;
		m_pBctFmtClient[i] = new CBarcodeFormatClient;

//		g_client_comm_ec[i] = NULL;
//		g_client_comm_ec[i] = new AClient_Comm_EC;

	}

	for ( int j = 0; j < 20; j++)
	{
		m_pSite[j] = NULL;
		m_pSite[j] = new APartTestSite;
	}

	for ( int k = 0; k < 10; k++)
	{
		m_pRobot[k] = NULL;
		m_pRobot[k] = new APartRobot;
	}

}

CPublic_Function::~CPublic_Function()
{
	delete mp_main_font;
	mp_main_font = NULL;

	delete mp_main_big_font;
	mp_main_big_font = NULL;

	delete mp_motor_font;
	mp_motor_font = NULL;

	delete mp_motorspeed_font;
	mp_motorspeed_font = NULL;

	delete mp_motoraxis1_font;
	mp_motoraxis1_font = NULL;

	delete mp_motorpart_font;
	mp_motorpart_font = NULL;

	delete mp_iopart_font;
	mp_iopart_font = NULL;

	delete mp_partmanual_font;
	mp_partmanual_font = NULL;

	delete mp_fileEditer_font;
	mp_fileEditer_font = NULL;
	
	delete mp_pgminfo_font;
	mp_pgminfo_font = NULL;
	
	delete mp_alarm_font;
	mp_alarm_font = NULL;

	delete mp_listdataview_font;
	mp_listdataview_font = NULL;
	
	delete mp_buffer_font;
	mp_buffer_font = NULL;

	delete mp_exit_font;
	mp_exit_font = NULL;
	
	delete mp_info_font;
	mp_info_font = NULL;
	
	delete mp_board_font;
	mp_board_font = NULL;
	
	delete mp_pad_font;
	mp_pad_font = NULL;

	delete mp_list_font;
	mp_list_font = NULL;

	delete mp_msg_font;
	mp_msg_font = NULL;

	delete mp_change_font;
	mp_change_font = NULL;

	delete mp_pass_font;
	mp_pass_font = NULL;

	delete mp_select_font;
	mp_select_font = NULL;

	delete mp_title_font;
	mp_title_font = NULL;

	delete mp_menu_font;
	mp_menu_font = NULL;

	delete mp_m_msg_font;
	mp_m_msg_font = NULL;

	delete mp_io_font;
	mp_io_font = NULL;

	delete mp_init_font;
	mp_init_font = NULL;

	delete mp_lock_font;
	mp_lock_font = NULL;

	delete mp_wait_time_font;
	mp_wait_time_font = NULL;

	delete mp_maintenance_font;
	mp_maintenance_font = NULL;
	
	delete mp_combo_font;
	mp_combo_font = NULL;

	delete mp_basic_font;
	mp_basic_font = NULL;

	delete mp_operation_list_font;
	mp_operation_list_font = NULL;

	delete mp_admin_font;
	mp_admin_font = NULL;	

	for (int i=0; i<4; i++)
	{
		if (m_pZebra[i] != NULL)
		{
			delete m_pZebra[i];
			m_pZebra[i] = NULL;
		}

		if (m_pVsClient[i] != NULL)
		{
			delete m_pVsClient[i];
			m_pVsClient[i] = NULL;
		}

		if (m_pVsServer[i] != NULL)
		{
			delete m_pVsServer[i];
			m_pVsServer[i] = NULL;
		}

		if( m_pBctFmtClient[i] != NULL )
		{
			delete m_pBctFmtClient[i];
			m_pBctFmtClient[i] = NULL;
		}

// 		if( g_client_comm_ec[i] != NULL)
// 		{
// 			delete g_client_comm_ec[i];
// 			g_client_comm_ec[i] = NULL;
// 		}
	}
	for ( int j = 0; j < 20; j++)
	{
		if(m_pSite[j] != NULL)
		{
			delete  m_pSite[j];
			m_pSite[j] = NULL;
		}
	}
	for ( int k = 0; k < 10; k++)
	{
		if(m_pRobot[k] != NULL)
		{
			delete  m_pRobot[k];
			m_pRobot[k] = NULL;
		}
	}

}

void CPublic_Function::On_LogFile_Add(int n_msg_select, CString str_msg, int n_Opt)
{
	CSingleLock sing(&pmutex);

	sing.Lock();

	CString mstr_cur_year="", mstr_cur_month="", mstr_cur_day="", str_display_time=""; // ÇöÀç ³â, ¿ù, ÀÏ Á¤º¸ ¹®ÀÚÇüÀ¸·Î º¯È¯ÇÏ¿© ÀúÀåÇÒ º¯¼ö 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour, mn_cur_min, mn_cur_sec; // ÇöÀç ³â, ¿ù, ÀÏ Á¤º¸ ÀúÀå º¯¼ö 
	CString mstr_file_name="";		// ¸¶Áö¸·À¸·Î »ý¼ºµÈ ÆÄÀÏ ÀÌ¸§ ÀúÀå º¯¼ö 
	CString mstr_create_file="";	// ¾Ë¶÷ Á¤º¸ ÀúÀåÇÒ ÆÄÀÏ¿¡ ´ëÇÑ [Æú´õ]+[ÆÄÀÏ¸í]+[È®ÀåÀÚ] ¼³Á¤ º¯¼ö 
	CString mstr_list_name="", mstr_temp_data="";  // °¢ ºÎºÐº° ¾Ë¶÷ ¹ß»ý È½¼ö Á¤º¸ ÀúÀåÇÒ ·Î±× ÆÄÀÏ Á¤º¸ ÀúÀå º¯¼ö 
	CString mstr_content="";		// ÇÑ ¶óÀÎ¿¡ Ãâ·ÂÇÒ ¹®ÀÚ¿­ Á¤º¸ ÀúÀå º¯¼ö 
	COleDateTime time_cur;		// °Ë»çÇÒ ½Ã°£ Á¤º¸ ÀúÀå º¯¼ö 
	CTime m_time_current;		// °£·«ÇÑ Çë½ÄÀÇ ÇöÀç ½Ã°£ Á¤º¸ ÀúÀå º¯¼ö
	int mn_existence;			// ÆÄÀÏ Á¸Àç À¯¹« ¼³Á¤ ÇÃ·¡±× 
	char fileName[256];			// °Ë»öÇÒ ÆÄÀÏ Á¤º¸ ¼³Á¤ ÇÔ¼ö 
	FILE  *fp ;					// ÆÄÀÏ¿¡ ´ëÇÑ Æ÷ÀÎÅÍ ¼³Á¤ º¯¼ö 

	CString BackupName="";
	CString mstr_cur_hour="", mstr_cur_min="", mstr_cur_sec="";
	CString mstr_folder="", mstr_file="";
	int Ret;

	CString sTime;
	
	SYSTEMTIME csTime;
	
	GetLocalTime(&csTime);
	
	csTime.wYear;
	csTime.wMonth;
	csTime.wDay;
	csTime.wHour;
	csTime.wMinute;
	csTime.wSecond;
	csTime.wMilliseconds;
	
	if(n_Opt >= 1)
		sTime.Format("%04d/%02d/%02d %02d:%02d:%02d.%03d ", csTime.wYear, csTime.wMonth, csTime.wDay, csTime.wHour, csTime.wMinute, csTime.wSecond, csTime.wMilliseconds);
	else
		sTime.Format("[%04d/%02d/%02d %02d:%02d:%02d.%03d] ", csTime.wYear, csTime.wMonth, csTime.wDay, csTime.wHour, csTime.wMinute, csTime.wSecond, csTime.wMilliseconds);

	if (str_msg.IsEmpty() == TRUE)  
	{
		sing.Unlock();
		return;
	}

	// **************************************************************************
	// ÆÄÀÏ ÀÌ¸§À¸·Î »ç¿ëÇÒ ³¯Â¥ Á¤º¸¸¦ ¾ò´Â´Ù                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // ÇöÀç ½Ã°£ Á¤º¸¸¦ ¾ò´Â´Ù. 

	m_time_current = CTime::GetCurrentTime();  // °£·«ÇÑ Çü½ÄÀÇ ÇöÀç ½Ã°£ Á¤º¸ ¾ò´Â´Ù. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();

	mn_cur_min = time_cur.GetMinute();
	mn_cur_sec = time_cur.GetSecond();
	// **************************************************************************

	// **************************************************************************
	// ³¯Â¥ Á¤º¸¸¦ ¹®ÀÚÇüÀ¸·Î º¯È¯ÇÏ¿© º¯¼ö¿¡ ¼³Á¤ÇÑ´Ù                           
	// **************************************************************************
	mstr_cur_year.Format("%04d", mn_cur_year);
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// ÇöÀç ½Ã°£ Á¤º¸ ¾ò´Â´Ù                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();	// ÇöÀç ½Ã°£ Á¤º¸¸¦ ¼³Á¤ÇÑ´Ù. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// ÇöÀç ½Ã°£ Á¤º¸¸¦ ¼³Á¤ÇÑ´Ù.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// ÇöÀç ºÐ Á¤º¸¸¦ ¼³Á¤ÇÑ´Ù. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// ÇöÀç ÃÊ Á¤º¸¸¦ ¼³Á¤ÇÑ´Ù. 
	str_display_time = m_time_current.Format("%c");  // ¸®½ºÆ® ÆÄÀÏ¿¡ Ãâ·ÂÇÒ Å¸ÀÌÆ² ½Ã°£ Á¤º¸ ¼³Á¤ 
	// **************************************************************************

	mstr_file.Format("%d.txt",time_cur.GetHour());

	switch (n_msg_select)			//Operation Setting
	{		
	case LOG_OPERATION:
		mstr_file_name = "OP" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_operation + mstr_file_name;
		break;
	case LOG_TESTINTERFACE:							//GPIB
		mstr_file_name = "TI" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_interface + mstr_file_name;
		break;
	case LOG_TCPIP:								//Àåºñ µ¿ÀÛ °ü·Ã.
		mstr_file_name = "TCP" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_tcpip + mstr_file_name;
		break;
	case LOG_MACHINE:								//Àåºñ µ¿ÀÛ °ü·Ã.
		mstr_file_name = "MA" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_DEBUG:									// Àåºñ µð¹ö±ë °ü·Ã.
		mstr_file_name = "DEBUG" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_BARCODE:								// ¹ÙÄÚµå °ü·Ã.
		mstr_file_name = "BARCODE" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_barcode + mstr_file_name;
		break;
	case LOG_TIME:									// TIME°ü·Ã
		mstr_file_name = "TIME" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_AUTOSOCKETOFF:							//Àåºñ µ¿ÀÛ °ü·Ã.
		mstr_file_name = "AUTOSOCKETOFF" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_WORK:									//Àåºñ µ¿ÀÛ °ü·Ã.
		mstr_file_name = "WORK" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_ADJUSTMENT:							//Àåºñ µ¿ÀÛ °ü·Ã.
		mstr_file_name = "ADJUSTMENT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_HISTORY:								//Àåºñ µ¿ÀÛ °ü·Ã.
		mstr_file_name = "HISTORY" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;

	case LOG_UPH:
		mstr_file_name = "UPH" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_uph + mstr_file_name;
		
		break;

	case LOG_UPH_TOT:
		mstr_file_name = "PROTOT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_uph_total + mstr_file_name;
		break;

	case LOG_REJECT:
		mstr_file_name = "REJ" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_reject + mstr_file_name;
		
		break;

	case LOG_ERROR:
		mstr_file_name = "Error_" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_error_path + mstr_file_name;
		
		break;

	case LOG_EVENT:
		mstr_file_name = "Event_" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_event_path + mstr_file_name;
		break;

	case LOG_PRODUCT:
		mstr_file_name = "Product_" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_product_path + mstr_file_name;
		
		break;

	case LOG_MAT:
		mstr_file_name = "Mat_" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_mat_path + mstr_file_name;
		
		break;

	case LOG_TOTAL:									//ÀüÃ¼ ¸Þ¼¼Áö.
		mstr_file_name = "TOT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_total + mstr_file_name;

		break;

	default:
		return;
	}

	mstr_create_file += ".TXT";

	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName, 0);
	
	if (mn_existence == -1)
	{
		switch (n_msg_select)			//Operation Setting
		{		
		case LOG_OPERATION:
			mstr_file_name = "OP" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_operation + mstr_file_name;
			break;
		case LOG_TESTINTERFACE:							//GPIB
			mstr_file_name = "TI" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_interface + mstr_file_name;
			break;
		case LOG_TCPIP:								//Àåºñ µ¿ÀÛ °ü·Ã.
			mstr_file_name = "TCP" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_tcpip + mstr_file_name;
			break;
		case LOG_MACHINE:								//Àåºñ µ¿ÀÛ °ü·Ã.
			mstr_file_name = "MA" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_DEBUG:									// Àåºñ µð¹ö±ë °ü·Ã.
			mstr_file_name = "DEBUG" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_BARCODE:								// ¹ÙÄÚµå °ü·Ã.
			mstr_file_name = "BARCODE" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_barcode + mstr_file_name;
			break;
		case LOG_TIME:									// TIME°ü·Ã
			mstr_file_name = "TIME" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_AUTOSOCKETOFF:							//Àåºñ µ¿ÀÛ °ü·Ã.
			mstr_file_name = "AUTOSOCKETOFF" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_WORK:									//Àåºñ µ¿ÀÛ °ü·Ã.
			mstr_file_name = "WORK" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_ADJUSTMENT:							//Àåºñ µ¿ÀÛ °ü·Ã.
			mstr_file_name = "ADJUSTMENT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_HISTORY:								//Àåºñ µ¿ÀÛ °ü·Ã.
			mstr_file_name = "HISTORY" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;

		case LOG_UPH:
			mstr_file_name = "UPH" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_uph + mstr_file_name;
			
			break;

		case LOG_UPH_TOT:
			mstr_file_name = "PROTOT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_uph_total + mstr_file_name;
			break;

		case LOG_REJECT:
			mstr_file_name = "REJ" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_reject + mstr_file_name;			
			break;

		case LOG_ERROR:
			mstr_file_name = "Error_" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_error_path + mstr_file_name;
			break;
			
		case LOG_EVENT:
			mstr_file_name = "Event_" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_event_path + mstr_file_name;
			break;

		case LOG_PRODUCT:
			mstr_file_name = "Product_" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_product_path + mstr_file_name;
			
			break;

		case LOG_MAT:
			mstr_file_name = "Mat_" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_mat_path + mstr_file_name;
			
			break;

		case LOG_TOTAL:									//ÀüÃ¼ ¸Þ¼¼Áö.
			mstr_file_name = "TOT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_total + mstr_file_name;
			
			break;
			
		default:
			return;
		}
		mstr_create_file += ".TXT";
		// **********************************************************************
	}

	Ret = FileSizeCheck(mstr_create_file, 1000000, CTL_YES); //size and rename
	
	if (Ret == 1) //file size over
	{
		CString strTmp = mstr_create_file.Left( mstr_create_file.GetLength() - 4 );
		BackupName.Format("%s_%02d%02d%02d.bak",strTmp, mn_cur_hour, mn_cur_min, mn_cur_sec );
// 		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
// 		BackupName = strTmp + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}	// **************************************************************************
	// ¾Ë¶÷ ¹ß»ý È½¼ö Á¤º¸ ÀúÀå ÆÄÀÏ¿¡ Ãß°¡ °¡´ÉÇÑ ÇüÅÂ ÆÄÀÏ·Î »ý¼º              
	// **************************************************************************

	if(n_msg_select == LOG_MAT && n_Opt == 2)
	{
		mn_existence = access(mstr_create_file, 0);			
		if (mn_existence != -1)
		{
			DeleteFile(mstr_create_file);
		}
	}

	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		CString strLog;
		strLog.Format("File Open Failue .. [%s]", mstr_create_file);
		Func.MsgLog(strLog);
		sing.Unlock();
		return;
	}
	// **************************************************************************

	// **************************************************************************
	// ·Î±× ÆÄÀÏ¿¡ ÇöÀç ¹ß»ýÇÑ ¾Ë¶÷ Á¤º¸ ÀúÀåÇÑ´Ù                                
	// **************************************************************************
//	mstr_content += str_display_time;
// 	mstr_content += sTime;
// 
// 	mstr_content += " : " + str_msg;

	if(n_msg_select == LOG_ERROR || n_msg_select == LOG_EVENT || n_msg_select == LOG_PRODUCT || n_msg_select == LOG_MAT)
	{
		mstr_content.Format("%s, %s",sTime,str_msg);
	}
	else
	{
		mstr_content.Format("%s : %s",sTime,str_msg);
	}

	fprintf(fp,"%s\n",mstr_content);

	if (ferror(fp))  
	{
		if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
		{
//			st_msg.mstr_abnormal_msg = _T("File save failure.");
			sprintf(st_msg.c_abnormal_msg, "File save failure.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
		}
		fclose(fp);
		sing.Unlock();
		return ;
	}
	// **************************************************************************
	fclose(fp);  // ÆÄÀÏÀ» Á¾·áÇÑ´Ù.

	sing.Unlock();
}

BOOL CPublic_Function::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            CString strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

CString CPublic_Function::Get_MotorName(int n_motor_no)
{
	CString strRet="";

 	switch (n_motor_no)
 	{
		case M_D_FEEDER_1:			strRet = "FEEDER_1";				break;
		case M_D_FEEDER_2:			strRet = "FEEDER_2";				break;
		case M_D_FEEDER_3:			strRet = "FEEDER_3";				break;
		case M_D_FEEDER_4:			strRet = "FEEDER_4";				break;
		case M_D_FEEDER_5:			strRet = "FEEDER_5";				break;
		case M_D_FEEDER_6:			strRet = "FEEDER_6";				break;
		case M_BCR_FEEDER_1:		strRet = "BCR_FEEDER_1";			break;
		case M_BCR_FEEDER_2:		strRet = "BCR_FEEDER_2";			break;
		case M_BCR_FEEDER_3:		strRet = "BCR_FEEDER_3";			break;
		case M_BCR_FEEDER_4:		strRet = "BCR_FEEDER_4";			break;
		case M_D_VISION_X:			strRet = "Density VISION_X";		break;
		case M_D_ATTACH_X:			strRet = "Density X";				break;
		case M_D_ATTACH_Y:			strRet = "Density Y";				break;
		case M_D_ATTACH_Z1:			strRet = "Density Z1";				break;
		case M_D_ATTACH_Z2:			strRet = "Density Z2";				break;
		case M_BCR_VISION_X:		strRet = "BCR_VISION_X";			break;
		case M_L_REJECT_X:			strRet = "REJECT_X";				break;
		case M_BCR_ATTACH1_X:		strRet = "BCR_ATTACH1_X";			break;
		case M_BCR_ATTACH1_Z1:		strRet = "BCR_ATTACH1_Z1";			break;
		case M_BCR_ATTACH1_R1:		strRet = "BCR_ATTACH1_R1";			break;
		case M_BCR_ATTACH1_Z2:		strRet = "BCR_ATTACH1_Z2";			break;
		case M_BCR_ATTACH1_R2:		strRet = "BCR_ATTACH1_R2";			break;
		case M_BCR_ATTACH2_X:		strRet = "BCR_ATTACH2_X";			break;
		case M_BCR_ATTACH2_Z1:		strRet = "BCR_ATTACH2_Z1";			break;
		case M_BCR_ATTACH2_R1:		strRet = "BCR_ATTACH2_R1";			break;
		case M_BCR_ATTACH2_Z2:		strRet = "BCR_ATTACH2_Z2";			break;
		case M_BCR_ATTACH2_R2:		strRet = "BCR_ATTACH2_R2";			break;
		case M_TOP_VISION_X:		strRet = "TOP_VISION_X";			break;
		case M_BTM_VISION_X:		strRet = "BTM_VISION_X";			break;
		case M_ULD_Y:				strRet = "ULD_Y";					break;
		case M_ULD_Z:				strRet = "ULD_Z";					break;
		case M_ULD_CV_PITCH:		strRet = "ULD_CV_PITCH";			break;
		case M_IDBUFFER_EL:			strRet = "IDBUFFER_EL";				break;
		case M_TOPID_LD1:			strRet = "TOPID_LD1";				break;
		case M_TOPID_LD2:			strRet = "TOPID_LD2";				break;
		case M_BTMID_ULD2:			strRet = "BTMID_ULD2";				break;
		case M_BTMID_ULD1:			strRet = "BTMID_ULD1";				break;
		case M_INDEX_X:				strRet = "INDEX_X";					break;
		case M_INDEX_ROLLER1:		strRet = "INDEX_ROLLER1";			break;
		case M_INDEX_ROLLER2:		strRet = "INDEX_ROLLER2";			break;
		case M_ULD_ROLLER_PITCH:	strRet = "ULD_ROLLER_PITCH";		break;
		case M_ULD_STACKER_Z:		strRet = "ULD_STACKER_Z	";			break;	

 		default:
 			strRet = "Unknown_Motor";
 			break;
 	}
	
	return strRet;
}

CString CPublic_Function::Get_AlarmPartName(int n_motor_no)
{
	CString strRet="";

	if(n_motor_no < 10000) strRet = "Density_Feeder1";	
	else if(n_motor_no > 10000 && n_motor_no < 20000) strRet = "Density_Feeder2";
	else if(n_motor_no > 20000 && n_motor_no < 30000) strRet = "Density_Feeder3";
	else if(n_motor_no > 30000 && n_motor_no < 40000) strRet = "Density_Feeder4";
	else if(n_motor_no > 40000 && n_motor_no < 50000) strRet = "Density_Feeder5";
	else if(n_motor_no > 50000 && n_motor_no < 60000) strRet = "Density_Feeder6";
	else if(n_motor_no > 60000 && n_motor_no < 70000) strRet = "Distribute_Feeder1";
	else if(n_motor_no > 70000 && n_motor_no < 80000) strRet = "Distribute_Feeder2";
	else if(n_motor_no > 80000 && n_motor_no < 90000) strRet = "Distribute_Feeder3";
	else if(n_motor_no > 90000 && n_motor_no < 100000) strRet = "Distribute_Feeder4";
	else if(n_motor_no > 100000 && n_motor_no < 110000) strRet = "Density_Vision_X";
	else if(n_motor_no > 110000 && n_motor_no < 120000) strRet = "Density_Attach_X";
	else if(n_motor_no > 120000 && n_motor_no < 130000) strRet = "Density_Attach_Y";
	else if(n_motor_no > 130000 && n_motor_no < 140000) strRet = "Density_Attach_Z1";
	else if(n_motor_no > 140000 && n_motor_no < 150000) strRet = "Density_Attach_Z2";
	else if(n_motor_no > 150000 && n_motor_no < 160000) strRet = "Distribute_Vision_X";
	else if(n_motor_no > 160000 && n_motor_no < 170000) strRet = "Distribute_Label_Reject_X";
	else if(n_motor_no > 170000 && n_motor_no < 180000) strRet = "Distribute1_Attach_X";
	else if(n_motor_no > 180000 && n_motor_no < 190000) strRet = "Distribute1_Attach_Z1";
	else if(n_motor_no > 190000 && n_motor_no < 200000) strRet = "Distribute1_Attach_R1";
	else if(n_motor_no > 200000 && n_motor_no < 210000) strRet = "Distribute1_Attach_Z2";
	else if(n_motor_no > 210000 && n_motor_no < 220000) strRet = "Distribute1_Attach_R2";
	else if(n_motor_no > 220000 && n_motor_no < 230000) strRet = "Distribute2_Attach_X";
	else if(n_motor_no > 230000 && n_motor_no < 240000) strRet = "Distribute2_Attach_Z1";
	else if(n_motor_no > 240000 && n_motor_no < 250000) strRet = "Distribute2_Attach_R1";
	else if(n_motor_no > 250000 && n_motor_no < 260000) strRet = "Distribute2_Attach_Z2";
	else if(n_motor_no > 260000 && n_motor_no < 270000) strRet = "Distribute2_Attach_R2";
	else if(n_motor_no > 270000 && n_motor_no < 280000) strRet = "Top_Vision_X";
	else if(n_motor_no > 280000 && n_motor_no < 290000) strRet = "Bottom_Vision_X";
	else if(n_motor_no > 290000 && n_motor_no < 300000) strRet = "Unload_Robot_Y";
	else if(n_motor_no > 300000 && n_motor_no < 310000) strRet = "Unload_Robot_Z";
	else if(n_motor_no > 310000 && n_motor_no < 320000) strRet = "Unload_CV_Pitch";
	else if(n_motor_no > 320000 && n_motor_no < 330000) strRet = "ID_Buffer_EL";
	else if(n_motor_no > 330000 && n_motor_no < 340000) strRet = "Top_ID_Buffr_LD1";
	else if(n_motor_no > 340000 && n_motor_no < 350000) strRet = "Top_ID_Buffr_LD2";
	else if(n_motor_no > 350000 && n_motor_no < 360000) strRet = "Bottom_ID_Buffr_ULD2";
	else if(n_motor_no > 360000 && n_motor_no < 370000) strRet = "Bottom_ID_Buffr_ULD1";
	else if(n_motor_no > 370000 && n_motor_no < 380000) strRet = "Index_X";
	else if(n_motor_no > 380000 && n_motor_no < 390000) strRet = "Index_Rolling1";
	else if(n_motor_no > 390000 && n_motor_no < 400000) strRet = "Index_Rolling2";
	else if(n_motor_no > 400000 && n_motor_no < 420000) strRet = "Unload_Stacker_Z";
	else if(n_motor_no == 500000)						strRet = " Density XY";
	else if(n_motor_no > 500000 && n_motor_no <= 506080) strRet = "Density_Label_Feeder";
	else if(n_motor_no >= 507000 && n_motor_no <= 507111) strRet = "Density_Label_Robot";
	else if(n_motor_no >= 507120 && n_motor_no <= 507151) strRet = "Barcode_feeder";
	else if(n_motor_no >= 510000 && n_motor_no <= 513111) strRet = "Distribution_Label_Printer";
	else if(n_motor_no >= 514000 && n_motor_no <= 521131) strRet = "Reject_Label_Robot";
	else if(n_motor_no >= 522000 && n_motor_no <= 522021) strRet = "Density_Attach_Z1";
	else if(n_motor_no >= 522040 && n_motor_no <= 522071) strRet = "Vision_Trigger";
	else if(n_motor_no >= 522080 && n_motor_no <= 522990) strRet = "Reject_Label";

	else if(n_motor_no >= 523000 && n_motor_no <= 523031) strRet = "Unloader_Robot";
	else if(n_motor_no >= 523040 && n_motor_no <= 523041) strRet = "Recycle_Zone_Door";
	else if(n_motor_no >= 523080 && n_motor_no <= 523131) strRet = "Unloader_Robot";

	else if(n_motor_no >= 523140 && n_motor_no <= 523151) strRet = "Recycle_Zone_Door";
	else if(n_motor_no >= 524000 && n_motor_no <= 524111) strRet = "Recycle_Zone";
	else if(n_motor_no >= 524120 && n_motor_no <= 524991) strRet = "Unloader_Conveyor";

	else if(n_motor_no >= 525000 && n_motor_no <= 530111) strRet = "Top_BTM_ID_Buffer";
	else if(n_motor_no >= 531000 && n_motor_no <= 531151) strRet = "Rolling1_Rolling2_Pusher";
	else if(n_motor_no >= 532000 && n_motor_no <= 532151) strRet = "Buffer_Elevator";

	else if(n_motor_no >= 533000 && n_motor_no <= 533111) strRet = "Bottom_ID_Buffer";
	else if(n_motor_no >= 533120 && n_motor_no <= 533151) strRet = "TOP_ID_Buffer_Support";
	else if(n_motor_no >= 534000 && n_motor_no <= 534150) strRet = "Bottom_ID_Buffer";
	
	else if(n_motor_no >= 535040 && n_motor_no <= 535041) strRet = "Reject_Label_Conveyor";
	else if(n_motor_no >= 535050 && n_motor_no <= 535051) strRet = "Unloader_Label_Conveyor";
	else if(n_motor_no >= 535060 && n_motor_no <= 535121) strRet = "Recycle_Zone_Door";

	else if(n_motor_no >= 536000 && n_motor_no <= 537130) strRet = "Machine_Switch";
	else if(n_motor_no >= 540000 && n_motor_no <= 540130) strRet = "SMEMA";
	else if(n_motor_no >= 600000 && n_motor_no <= 600200) strRet = "Density_feeder_vision";
	
	else if(n_motor_no >= 600220 && n_motor_no <= 600248) strRet = "id_buffer";
	else if(n_motor_no >= 600300  && n_motor_no <= 600500) strRet = "Density_feerder";
	else if(n_motor_no >= 601000 && n_motor_no <= 601100) strRet = "top_id_buffer";
	
	else if(n_motor_no >= 602000 && n_motor_no <= 602003) strRet = "Density_feerder";
	else if(n_motor_no >= 610000  && n_motor_no <= 610400) strRet = "Distribution_feeder";
	else if(n_motor_no >= 620000 && n_motor_no <= 630000) strRet = "Label_Ribbon_Barcode_code";
	
	else if(n_motor_no >= 640000 && n_motor_no <= 640003) strRet = "zebra_Printer";
	else if(n_motor_no == 650000 )						  strRet = " barcode";
	else if(n_motor_no == 700000 )						  strRet = " Server_connect";
	else if(n_motor_no >= 700010  && n_motor_no <= 700430) strRet = "Barcode";
	else if(n_motor_no >= 701000 && n_motor_no <= 720003) strRet = "Vision";
	
	else if(n_motor_no >= 730010 && n_motor_no <= 730060) strRet = "Density_label";
	else if(n_motor_no >= 740000  && n_motor_no <= 750000) strRet = "smema";
	else if(n_motor_no >= 770000 && n_motor_no <= 772000) strRet = "Server_Comm";
	
	else if(n_motor_no >= 800000 && n_motor_no <= 810006) strRet = "Barcode_read";
	else if(n_motor_no == 900000 )						  strRet = "Retry";
	else if(n_motor_no > 900000  && n_motor_no <= 900003) strRet = "Board";
	else if(n_motor_no >= 900101 && n_motor_no <= 900105) strRet = "Server_Comm";
	
	else if(n_motor_no >= 900200 && n_motor_no <= 940004) strRet = "Vision";
	else if(n_motor_no == 940100 )						  strRet = "Density";
	else if(n_motor_no == 950000 )						  strRet = "Xgem";
	else if(n_motor_no >= 96000  && n_motor_no <= 960005) strRet = "Buffer_Tray_Serial";

	else
		strRet = "Etc";
	
	return strRet;
}

int CPublic_Function::OnElect_CheckData(CString str_data)
{
	int nFuncRet = CTLBD_RET_GOOD;
	
	char	cp_cmd[1024];
	int     nLength;  // µ¥ÀÌÅÍ ÃÑ ±æÀÌ ÀúÀå º¯¼ö 
	CString strBody="";  // BODY µ¥ÀÌÅÍ ÀúÀå º¯¼ö 
	CString strTemp="";
	
	int nChannel;
	double dValue;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // µ¥ÀÌÅÍ ÀúÀå º¯¼ö ÃÊ±âÈ­ 
	
	// **************************************************************************
	// ¼ö½Å µ¥ÀÌÅÍ ÃÖ¼Ò ±æÀÌ °Ë»çÇÑ´Ù [24ÀÚ¸®°¡ µÇ¾ß ÇÑ´Ù.]
	// **************************************************************************
	nLength = str_data.GetLength();
	if (nLength != 25)			
		return CTLBD_RET_ERROR;
	// **************************************************************************
	
	strBody = str_data.Mid(2, nLength - 3);  // [Çì´õ+Å×ÀÏ] Á¦°Å 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	strTemp = strBody.Mid(11, 1);			// Ã¤³Î.
	nChannel = atoi(strTemp);
	if(nChannel < 1 || nChannel > 5)
	{
		if( st_handler.cwnd_list != NULL )
		{
			sprintf(st_msg.c_abnormal_msg, "%dPort is error", nChannel);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return nFuncRet;
	}
	
	strTemp = strBody.Mid(12, 5);
	dValue = atof(strTemp);
	
	st_work.mdReadElectData[nChannel-1] = dValue;	// °ªÀ» ³Ö´Â´Ù.
	
	if (strBody.Mid(12, 2) == "OP")
	{
//		st_msg.mstr_normal_msg.Format("%dPort : OPEN", nChannel);
		sprintf(st_msg.c_normal_msg, "%dPort : OPEN", nChannel);
		st_grounddb.mstrChannel[nChannel-1].Format("OPEN");
		st_work.mdReadElectData[nChannel-1] = -99999;	// °ªÀ» ³Ö´Â´Ù.
	}
	else if (strBody.Mid(12, 2) == "OV")
	{
//		st_msg.mstr_normal_msg.Format("%dPort : OVER", nChannel);
		sprintf(st_msg.c_normal_msg, "%dPort : OVER", nChannel);
		st_grounddb.mstrChannel[nChannel-1].Format("OVER");
		st_work.mdReadElectData[nChannel-1] = -99998;	// °ªÀ» ³Ö´Â´Ù.
	}
	else
	{
//		st_msg.mstr_normal_msg.Format("%dPort : %0.2f", nChannel, dValue);
		sprintf(st_msg.c_normal_msg, "%dPort : %0.2f", nChannel, dValue);
		st_grounddb.mstrChannel[nChannel-1].Format("%0.2f", dValue);
	}
	
	if( st_handler.cwnd_list != NULL )
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	}
	
	if (st_handler.cwnd_basic != NULL)											// BASIC È­¸é Á¸Àç
	{
		st_handler.cwnd_basic->SendMessage(WM_BASIC_APPLY, 1, nChannel);		// DATA APPLY ¿äÃ»
	}
	
	return nFuncRet;
}

void CPublic_Function::OnMot_Speed_Setting()
{
	int i;

	for (i = 0; i < MOTOR_COUNT; i++)
	{
		if (COMI.md_spd_vel[i][1] < 1)
		{
			COMI.md_spd_vel[i][1] = 10000;

			if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
			{
				sprintf(st_msg.c_abnormal_msg, "%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
			}
		}
		
		if (COMI.md_spd_vel[i][2] < 1)
		{
			COMI.md_spd_vel[i][2] = 10000;

			if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
			{
				sprintf(st_msg.c_abnormal_msg, "%s Motor Dec is under 1 -> 10000 Set up", Get_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
			}
		}

		COMI.Set_MotSpeed(MOT_SPD_VEL, i, cmSMODE_T, COMI.md_spd_vel[i][0], COMI.md_spd_vel[i][1], COMI.md_spd_vel[i][2]);
		COMI.Set_MotSpeed(MOT_SPD_ORG, i, cmSMODE_T, COMI.md_spd_home[i], COMI.md_spd_vel[i][1]/10, COMI.md_spd_vel[i][2]/10); 
		//st_motor[i].d_spd_home[0] * 10, st_motor[i].d_spd_home[0] * 10);

		COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		Sleep(100);
	}
}

int CPublic_Function::DoorOpenCheckSpot()
{
	int i;
	int Ret;
	
	Ret = CTLBD_RET_GOOD;
	// ¾î´À¹® ¿­·È´ÂÁö ¸Þ¼¼ÁöÃ¢ º¸¿©ÁØ´Ù.
	for( i = 0; i < 6; i++)
	{
		
		if( g_ioMgr.get_in_bit( st_io.i_door_chk[i] ) == IO_OFF )
		{
			Ret = CTLBD_RET_ERROR;

		}
	}

	////2014,1205
	if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_door_chk ) == IO_OFF )
	{
		Ret = CTLBD_RET_ERROR;	
	}
	////

	return Ret;
}

int CPublic_Function::OnMotor_Go_Check(int n_motor, double d_pos)
{
	CString str_msg="", str_motor="";
	
	int n_response, nRet = CTL_NO;
	
	CDialog_Select	select_dlg;
	
	str_motor = Get_MotorName(n_motor);
	
	str_msg.Format("%s Motor, Move value is %3.3f, Go?",str_motor, (float)d_pos);
	
	st_msg.mstr_confirm_msg = _T(str_msg);
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		nRet = CTL_YES;
	}
	else
	{
		nRet = CTL_NO;
	}
	
	return nRet;
}

int CPublic_Function::On_Encoder_Check(double d_pos)
{
	CString str_msg="";
	
	int n_response, nRet = CTL_NO;
	
	CDialog_Select	select_dlg;
	
	str_msg.Format("Now Reading Value is %.03f, Setting?", (double)d_pos);
	st_msg.mstr_confirm_msg = _T(str_msg);
	n_response = select_dlg.DoModal();
	if (n_response == IDOK) 
	{
		nRet = CTL_YES;
	}
	else
	{
		nRet = CTL_NO;
	}
	
	return nRet;
}

void CPublic_Function::OnSet_IO_Port_Stop()
{
	if ( st_timedb.mnTime_Section != dSTOP )
	{
		st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
		st_timedb.mnSectionBuffer = dSTOP;
		
		st_timedb.mole_date		= COleDateTime::GetCurrentTime();
		
		g_db.DB_Write_Time(st_timedb);
	}
	COMI.mn_run_status = dSTOP;
	
	st_handler.mn_status =  CTL_NO;					// Àåºñ°¡ °¡µ¿ÁßÀÌ³Ä.. 2K9/12/29/ViboX
	
//	g_ioMgr.set_out_bit(st_io.o_front_machine_request_sginal, CTL_NO);	// ¼ÅÆ²¿¡¼­ ¹Ð¾î ³ÖÀ»±îºÁ ÀÏ´Ü OFF½ÃÅ²´Ù. 2K10/08/10/ViboX

	g_ioMgr.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);

	////2015.0114
	if (st_handler.nModeManual == CTL_YES)
	{
		g_ioMgr.set_out_bit(st_io.o_door_lock, IO_OFF);
	}
	////

	//2013,1024
	Func.Handler_Recovery_Data_Write();
		
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);

	st_handler.mn_alarm_screen = FALSE;

	//2015.0413
	if( st_basic.mn_mode_xgem == CTL_YES )
	{
		g_xgem_client.SetMachineStatus(MC_STOP);
	}
}

void CPublic_Function::OnSet_IO_Port_Run()
{
	
	COMI.mn_run_status = dRUN;
	if ( st_timedb.mnTime_Section != st_timedb.mnSectionBuffer )
	{
		st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
		
		st_timedb.mnSectionBuffer = dRUN;
		
		st_timedb.mole_date		= COleDateTime::GetCurrentTime();
		
		g_db.DB_Write_Time(st_timedb);
	}
	
	//2015.0413
	if( st_basic.mn_mode_xgem == CTL_YES)
	{
		if(st_basic.mn_mode_rcmd == CTL_YES)
		{
			if( st_handler.m_nRemote == CTL_YES )
			{
				g_xgem_client.SetMachineStatus(MC_RUN);
			}
			else
			{
				CString strErr = "Remote Stop.Recipe is not send or not allowed from server.";
				st_msg.mstr_event_msg[0] = strErr;
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				
				return;
			}
		}
		else
		{
			g_xgem_client.SetMachineStatus(MC_RUN);
		}
		
	}
		
	//·¹º§ ÃÊ±âÈ­.
	st_handler.mn_level_teach = FALSE;
	st_handler.mn_level_maint = FALSE;
	st_handler.mn_level_ver = FALSE;//2014,0606 ybs

	st_handler.mn_safety = CTL_YES;						// ¾ÈÀü ÀåÄ¡ ÀÛµ¿ 2K6/06/09/ViboX
	
	st_handler.mn_status =  CTL_YES;					// Àåºñ°¡ °¡µ¿ÁßÀÌ³Ä.. 2K9/12/29/ViboX
	
	
	st_work.mn_jam = CTL_NO;//2015.0327

	g_ioMgr.set_out_bit(st_io.o_start_switch_lamp,	IO_ON);
	g_ioMgr.set_out_bit(st_io.o_stop_switch_lamp,	IO_OFF);
	
	////2015.0114
	// µµ¾î¸¦ Àá±Ù´Ù.
//	g_ioMgr.set_out_bit(st_io.o_door_lock,				IO_ON);
	OnCloseDoor();
	////

	// COK °Ë»ç.
//	g_handler_cok.CheckCokTypeChange();

	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);

	alarm.stl_alarm_start_time = GetCurrentTime();


	{//2015.0513
		CString strLog="";
		//strLog.Format(" Run, Labeller Start");
		//2017.0510
		strLog = "Run, Labeller Start";
		Func.On_LogFile_Add(LOG_EVENT, strLog, 1);
		st_work.mn_run_down = CTL_NO;
	}

	::SendMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1002, 0);
}

void CPublic_Function::OnSet_IO_Port_Init()
{
	COMI.mn_run_status = dINIT;

	// µµ¾î¸¦ Àá±Ù´Ù.
	g_ioMgr.set_out_bit(st_io.o_door_lock,				IO_ON);
}

int CPublic_Function::Handler_Recovery_Data(int n_Mode)
{
	//////////////////////////////////////////////////////////////////////////
	// °¢ Run ClassÀÇ º¯¼ö ÀÐ±â ¹× ¾²±â
	//////////////////////////////////////////////////////////////////////////
	CFile file;
	int nFileMode,nArchiveMode;
	CString sFileName="";
	int nFuncRet = CTL_GOOD;
	
	sFileName = "c:\\AMT8590\\Setting\\RecoveryRunData.dat";
	
	TRY
	{
		if(n_Mode == DATA_WRITE) //file write
		{
			nFileMode = CFile::modeWrite | CFile::modeCreate;
			nArchiveMode = CArchive::store;
		}
		else
		{
			nFileMode = CFile::modeRead;
			nArchiveMode = CArchive::load;
		}
		
		file.Open(LPCTSTR(sFileName),nFileMode);
		CArchive ar(&file,nArchiveMode);
		
		GlobalDataSerialize(ar);
	}
	CATCH(CFileException, e) 
	{
		e->ReportError();
		sprintf(st_msg.c_abnormal_msg, "Fail in accessing [%s] FILE.", sFileName);
		nFuncRet = CTL_ERROR;
	}
	END_CATCH
		
		if(nFuncRet == CTL_ERROR && st_handler.cwnd_list != NULL) 
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
		
	return nFuncRet;
}

void CPublic_Function::OnSet_IO_Port_Alarm()
{
	if ( st_timedb.mnTime_Section != CTL_dWARNING )
	{
		st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
		st_timedb.mnSectionBuffer = CTL_dWARNING;
		
		st_timedb.mole_date			= COleDateTime::GetCurrentTime();		
		g_db.DB_Write_Time(st_timedb);
	}
	OnSet_IO_Port_Sound(IO_ON);							// Buzzer¸¦ ON ½ÃÅ²´Ù.
	
	if (COMI.mn_run_status != dLOTEND)		
	{
		g_ioMgr.set_out_bit(st_io.o_alarm_clear_lamp, IO_ON);	// Reset Switch¿¡ Lamp¸¦ On ½ÃÅ²´Ù.
	}
	
	st_handler.mn_reinstatement = CTL_NO;					// º¹±¸ µ¿ÀÛÁß¿¡ ¾Ë¶÷ÀÌ ¹ß»ý ÇßÀ» °æ¿ì¿¡ º¹±¸ µ¿ÀÛÀÓÀ» ÇØÁ¦ ½ÃÅ²´Ù.
	
	// µµ¾î¸¦ ¿¬´Ù.
//	g_ioMgr.set_out_bit(st_io.o_door_lock_cyl,	IO_OFF);

}

void CPublic_Function::OnSet_IO_Port_Sound(int n_status, int n_opt)
{
	switch (n_status)
	{
	case 0:
		g_ioMgr.set_out_bit(st_io.o_buzzer_1,	IO_OFF);	// Buzzer ¸ðµå »ó°ü¾øÀÌ ²ö´Ù.
		g_ioMgr.set_out_bit(st_io.o_buzzer_2,	IO_OFF);	// Buzzer ¸ðµå »ó°ü¾øÀÌ ²ö´Ù.
		g_ioMgr.set_out_bit(st_io.o_buzzer_3,	IO_OFF);	// Buzzer ¸ðµå »ó°ü¾øÀÌ ²ö´Ù.
		g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_OFF);

		break;
		
	case 1:
		if (st_lamp.mn_buzzer_mode == CTL_YES)			// Buzzer »ç¿ë ¸ðµå¿¡¼­¸¸ ÄÒ´Ù.
		{
			if( n_opt == 1)	g_ioMgr.set_out_bit(st_io.o_buzzer_2,	IO_ON);
			else			g_ioMgr.set_out_bit(st_io.o_buzzer_1,	IO_ON);
			g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_ON);
		}
		break;
	}
}

CString CPublic_Function::OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt)
{
	CString strFuncRet="";
	int n_title, n_equal, n_end, n_length, n_qw = 0;    
	
	n_title =	strBody.Find(strFind, 0);							// TITLEÀÇ À§Ä¡¸¦ Ã£´Â´Ù.
	n_equal =	strBody.Find("=", n_title + 1);						// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
	
	if (n_title == -1 || n_equal == -1)		return "NULL";
	
	if (nOpt == 0)
	{
		n_end =		strBody.Find(" ", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_length =	n_end - n_equal - 1;							// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 7)
	{
		strFuncRet = strBody.Mid(n_title + 5);
	}
	else if (nOpt == 2)
	{
		n_end =		strBody.Find("=", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.						// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_end + 1);
	}
	else if (nOpt == 3)
	{
		n_end =		strBody.Find("=", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.						// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_end + 1, 1);
	}
	else if (nOpt == 4)
	{
		n_qw =		strBody.Find("((", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_end =		strBody.Find("))", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_length =	n_end - n_qw - 1;								// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_qw + 2, n_length - 1);
	}
	else if (nOpt == 5)
	{
		n_qw =		strBody.Find("FAIL_REGNUMBER", n_title + 22);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_length =	n_end - n_equal - 1;							// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 6)
	{
		n_qw =		strBody.Find("ID", n_title + 6);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_length =	n_end - n_equal - 1;							// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if ( nOpt == 8)
	{
		n_qw =		strBody.Find("\"", n_equal + 1 );
		n_end =		strBody.Find("\"", n_equal + 2 );
		n_length =	n_end - n_qw - 1;
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	else
	{
		n_qw =		strBody.Find("(", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_end =		strBody.Find(")", n_title + 1);					// ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.
		n_length =	n_end - n_qw - 1;								// DATAÀÇ ±æÀÌ °è»ê.
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	
	strFuncRet.MakeUpper();
	strFuncRet.TrimLeft(' ');
	strFuncRet.TrimRight(' ');

	return strFuncRet;
}

int CPublic_Function::OnNetworkDataAnalysis(CString strBody)
{
	int nFuncRet = CTLBD_RET_GOOD;

//	int i;
//	int n_check_num = 0, n_check_string = 0, n_check_extra = 0;
//	CString strFunction, strTemp;
//	int nID = -1;
//	CString strTmp[10];
//	CString strPCBOXID;
//	CString strpcbox_id;
//	CString strEnable;
//	CString strTestResult;
//	int nByPassCheck = 0;
//	CString str;
//	CString strBN;
//	CString strBinTemp;
//
//	int nOperCnt = 0;
//	CString strOperInfo;
//	BOOL bRet;
//	COleDateTime mtime_cur;						// ÇöÀç ½Ã°£ ÀúÀå º¯¼ö
//
//	strFunction = OnNetworkBodyAnalysis(strBody, "FUNCTION_RPY");
//
//	st_NW.mnID = -1;
//	if (strFunction == "LABEL_INFO")	st_NW.mnID = NW_LABEL_INFO_RPY;
//
//	switch (st_NW.mnID)
//	{
//		case NW_LABEL_INFO_RPY:
//			st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
//			st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
//			st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
//			st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");	
//			st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG",2);//20120913
//			st_NW.mstr_qty[1] = "10";//20120913
//
//			st_sync.mstr_lot_display = st_NW.mstr_result[1];
//
//			st_work.n_lotdisply_qty = atoi(st_NW.mstr_qty[1]);
//			
//			if (st_NW.mstr_result[1] == "PASS")
//			{
//				st_work.strlabelFormat.RemoveAll();
//				for (i = 0; i < atoi(st_NW.mstr_qty[1]); i++)
//				{
//					st_NW.mstr_Label_Pasing[i] = "";//2014,0215
//					strTemp.Format("FORMAT_%d", i + 1);
//					st_NW.mstr_temp[1] =	OnNetworkBodyAnalysis(strBody, strTemp, 9);
//
//					/////////////////////////////////////////////////// 20120826
//	//				st_work.strlabelFormatCode.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "FORMAT", 2));
//					st_NW.mstr_Recive_Data[i] = OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "FORMAT", 2);
//					bRet = OnLabelPrint_Parsing(st_NW.mstr_Recive_Data[i], i);	
//					if (bRet == TRUE)
//					{//ÇÊ¿äÇÒ¶§¸¸ Ãâ·Â
//					}
//					else if (bRet == FALSE)
//					{
//						sprintf(st_msg.c_normal_msg, "¶óº§ Àü»ê µ¥ÀÌÅ¸ ÆÄ½Ì ½ÇÆÐ");
//						if(g_local.GetLocalType() == LOCAL_ENG) 
//						{
//							sprintf(st_msg.c_normal_msg, "Computerized data parsing failed the label!!");
//						}
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//						st_NW.mstr_Label_Pasing[i] = "FAIL"; //2014,0215
//					}
//					///////////////////////////////////////////////////
//					st_work.strlabelFormat.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL"));
//
//				}
//				for (i = 0; i < atoi(st_NW.mstr_qty[1]); i++)
//				{
//					if(st_NW.mstr_Label_Pasing[i] == "FAIL")
//					{
//						st_NW.mstr_result[1] = "FAIL";
//						break;
//					}
//				}
//				st_handler.mn_lot_label_info = CTL_YES;
//			}
//			break;
//	}
	return nFuncRet;
}

CString CPublic_Function::OnNetworkDataMake(int nID)
{
	CString strHeader="";
	CString strBody="";
	int nBody = 0, i;

	CString strTemp;

	strBody = "";
	switch (nID)
	{
// ¿©±â¼­ºÎÅÍ´Â Àåºñ¿¡¼­ ´äº¯À» ÁÖ´Â ºÎºÐÀÌ´Ù.

	case NW_DELETE_SCRAP:
		break;

	case NW_SORTER_LOT_END:
		break;

	case NW_LOT_START:
		break;
	case NW_LOT_CANCEL:
		break;
	
	case NW_NEW_LOT_IN_SEND:			// 0000000000000050 FUNCTION=NEW_LOT_IN LOT_NO=TEST_LOT EQP_ID=IR8130
		strBody		= "FUNCTION=NEW_LOT_IN";
		strBody		+= " LOT_NO=" + st_NW.mstr_new_lot_no[1];
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;
		break;

	case NW_LOT_NAME:					// 0000000000000050 FUNCTION=LOT_NAME LOT_NO=TEST_LOT
		strBody		= "FUNCTION=LOT_NAME";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		break;

	case NW_LOT_START_ENABLE:
		st_NW.mstr_function[0]		= "LOT_START_ENABLE";
		st_NW.mstr_lot_id[0]		= st_NW.mstr_lot_id[1];
		st_NW.mstr_eqp_id[0]		= st_NW.mstr_eqp_id[1];
		st_NW.mstr_step_cnt[0]		= st_NW.mstr_step_cnt[1];
		st_NW.mstr_result[0]		= st_NW.mstr_result[1];
		st_NW.mstr_fail_log[0]		= st_NW.mstr_fail_log[1];

		strBody		= "FUNCTION_RPY=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "STEP_COUNT" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "RESULT=PASS ";
		strBody		+= "FAIL_LOG" + st_NW.mstr_fail_log[0];
		break;
// ¿©±â±îÁö´Â Àåºñ¿¡¼­ ´äº¯À» ÁÖ´Â ºÎºÐÀÌ´Ù.
	case NW_LABEL_INFO:
		st_NW.mstr_function[0]			= "LABEL_NEW_INFO";//= "LABEL_INFO";//
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]			= st_NW.mstr_lot_id[1];
		st_NW.mstr_oper_id[0]			= st_work.mstr_op_id;
		st_NW.mstr_format_retry[0]		= st_work.mstr_format_retry = _T("10");
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "OPERID=" + st_NW.mstr_oper_id[0] + " ";	
		strBody		+= "LABEL_QTY=" + st_work.mstr_format_retry;
		break;

// ¿©±â¼­ºÎÅÍ´Â Àåºñ°¡ Áú¹®À» ÇÏ°Å³ª Åëº¸¸¦ ÇÏ´Â ºÎºÐÀÌ´Ù.
	case NW_TRACK_IN:		
		break;

	case NW_BPC_BUFFER_READY:
		strBody		= "FUNCTION=BUFFER_READY ";
		strBody	   += st_NW.mstr_bpc_buffer_ready;
		break;

	case NW_BPC_BUFFER_IN:
		strBody		= "FUNCTION=BUFFER_IN ";
		strBody	   += st_NW.mstr_bpc_buffer_in;
		break;

	case NW_TRACK_OUT:
		break;

	case NW_LOT_CANCEL_SEND:
		break;
	// »õ·Î Ãß°¡µÊ. ÆÄ¿ö¸¦ ²¨µµ ÁÁ³Ä´Â Áú¹®Àº ÀÌ°É·Î ÇÑ´Ù. 2K11/03/02/ViboX

	case NW_UNLOAD_COMPT:
		break;
	case NW_CANCEL_END:
		break;

	case NW_LOT_END:
		st_NW.mstr_function[0]		= "LOT_END";
		st_NW.mstr_lot_id[0]		= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]		= st_NW.mstr_step_cnt[1];
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_fail_cnt[0]		= st_NW.mstr_fail_cnt[1];
		st_NW.mstr_novideo_cnt[0]	= st_NW.mstr_novideo_cnt[1];


		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "STEP_CNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "FAIL_CNT=" + st_NW.mstr_fail_cnt[0] + " ";
		strBody		+= "NOVIDEO_CNT=" + st_NW.mstr_novideo_cnt[0];
		break;

	case NW_FAIL_INFO_SEND:
		break;
	case NW_FAIL_REGNUMBER_REQUEST:
		st_NW.mstr_function[0]		= "FAIL_REGNUMBER_REQUEST";
		st_NW.mstr_pcbox_id[0]		= st_NW.mstr_pcbox_id[1];
		st_NW.mstr_lot_id[0]		= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]		= st_NW.mstr_step_cnt[1];
		st_NW.mstr_slot_no[0]		= st_NW.mstr_slot_no[1];
		st_NW.mstr_test_idx[0]		= st_NW.mstr_test_idx[1];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "PCBOX_ID=" + st_NW.mstr_pcbox_id[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_CNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "SLOT_NO=" + st_NW.mstr_slot_no[0] + " ";
		strBody		+= "TEST_IDX=" + st_NW.mstr_test_idx[0];
		break;
	case NW_FAIL_INFO_SERACH:
		break;
	case NW_FAIL_INFO_SAVE:
		st_NW.mstr_function[0]			= "FAIL_INFO_SAVE";
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]			= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]			= st_NW.mstr_step_cnt[1];
		st_NW.mstr_fail_regnumber[0]	= st_NW.mstr_fail_regnumber[1];
		st_NW.mstr_pcbox_id[0]			= st_NW.mstr_pcbox_id[1];
		st_NW.mstr_slot_no[0]			= st_NW.mstr_slot_no[1];
		st_NW.mstr_test_idx[0]			= st_NW.mstr_test_idx[1];
		st_NW.mstr_fail_desc[0]			= st_NW.mstr_fail_desc[1];
		st_NW.mstr_fail_code[0]			= st_NW.mstr_fail_code[1];
		st_NW.mstr_fail_cnt[0]			= st_NW.mstr_fail_cnt[1];
		st_NW.mstr_retest_flag[0]		= st_NW.mstr_retest_flag[1];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_CNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "FAIL_REGNUMBER=" + st_NW.mstr_fail_regnumber[0] + " ";
		strBody		+= "PCBOX_ID=" + st_NW.mstr_pcbox_id[0] + " ";
		strBody		+= "SLOT_NO=" + st_NW.mstr_slot_no[0] + " ";
		strBody		+= "TEST_IDX=" + st_NW.mstr_test_idx[0] + " ";
		strBody		+= "FAIL_DESC=" + st_NW.mstr_fail_desc[0] + " ";
		strBody		+= "FAIL_CODE=" + st_NW.mstr_fail_code[0] + " ";
		strBody		+= "FAIL_CNT=" + st_NW.mstr_fail_cnt[0] + " ";
		strBody		+= "RETEST_FLAG=" + st_NW.mstr_retest_flag[0];
		break;
	case NW_FAIL_INFO_DELETE:
		st_NW.mstr_function[0]			= "FAIL_INFO_DELETE";
		st_NW.mstr_lot_id[0]			= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]			= st_NW.mstr_step_cnt[1];
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_fail_regnumber[0]	= st_NW.mstr_fail_regnumber[1];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "FAIL_REGNUMBER=" + st_NW.mstr_fail_regnumber[0];
		break;

	case NW_LOT_DISPLAY:
		break;

	case NW_EQP_LOT_START:
		break;

	case NW_EQP_LOT_CANCEL:
		break;

	case NW_EQP_LOT_END:
		break;
	case NW_PGM_START:
		break;
// ¿©±â±îÁö´Â Àåºñ°¡ Áú¹®À» ÇÏ°Å³ª Åëº¸¸¦ ÇÏ´Â ºÎºÐÀÌ´Ù.

	case NW_BUFFER_IN_SEND:
		strBody		= "FUNCTION=BUFFER_IN";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		strBody		+= " PCT_BYPASS=" + st_NW.mstr_pct_bypass[1];
		strBody		+= " MDL_CNT=" + st_NW.mstr_m_module_cnt[1];

		for (i = 0; i < 20; i++)
		{
			strTemp.Format(" MDL_%02d=%s", i + 1, st_NW.mstr_m_module_data[i][1]);
			strBody		+= strTemp;
		}
		break;

	case NW_BUFFER_READY_SEND:
		strBody		= "FUNCTION=BUFFER_READY";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		strBody		+= " PART_NO=" + st_NW.mstr_m_part_no[1];
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		strBody		+= " LAST_MODULE=" + st_NW.mstr_m_last_module[1];
		strBody		+= " BYPASS=" + st_NW.mstr_m_bypass[1];
		strBody		+= " RETEST=" + st_NW.mstr_m_retestcheck[1];				// »õ·Î Ãß°¡ µÆÀ½.

		{
			CString strTemp;
			strTemp.Format("[LAST MODULE] NW_BUFFER_READY_SEND	LOT:[%s]	LASTMODULE=[%s]", st_NW.mstr_m_lot_no[1], st_NW.mstr_m_last_module[1] );
			On_LogFile_Add(3, strTemp);
		}

		break;

	case NW_RETEST_REQ:
		strBody		= "FUNCTION=RETEST_REQ";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		strBody		+= " SOCKET_OFF=" + st_NW.mstr_m_socket_off[1];
		break;

	case NW_BLOT_COUNT:
		strBody		= "FUNCTION=BLOT_COUNT";
		break;

	}

	nBody = strBody.GetLength();

	if (nBody > 0)
	{
		if (nID == NW_BUFFER_READY_SEND || nID == NW_BUFFER_IN_SEND || nID == NW_RETEST_REQ
			 || nID == NW_NEW_LOT_IN_SEND || nID == NW_LOT_NAME || nID == NW_LOT_DISPLAY || nID == NW_TRACK_OUT
			 || nID == NW_BPC_BUFFER_READY ||  nID == NW_BPC_BUFFER_IN || nID == NW_DELETE_SCRAP)
		{
			strHeader.Format("%016d", nBody);
		}
		if(nID == NW_LABEL_INFO)
		{
			strHeader.Format("%d", nBody);
			int a = strHeader.GetLength();
			for(int i=0; i<16-a; i++)
			{
				strHeader += " ";
			}
		}
		else
		{
			strHeader.Format("%010d", nBody);
		}

		st_NW.mstr_header_msg[0] = strHeader;
		st_NW.mstr_data_msg[0] = strBody;
		st_NW.mstr_full_msg[0] = strHeader + strBody;
	}
	else
	{
		st_NW.mstr_header_msg[0] = "";
		st_NW.mstr_data_msg[0] = "";
		st_NW.mstr_full_msg[0] = "";
	}

	return st_NW.mstr_full_msg[0];
}

CString CPublic_Function::OnDeviceInfoAnalysis(CString strBody, int nInfo)
{
	CString strFuncRet = "NONE";
	int nLength;

	int n_start[5], n_check[5], n_length[5];    

	nLength = strBody.GetLength();
	if (nLength < 23)		return "FAIL";		// ±æÀÌ°¡ 22º¸´Ù ÂªÀ¸¸é FAILÀÌ´Ù.

	n_start[0] = 0;
	n_check[0] = strBody.Find("-", n_start[0]);

	if (n_check[0] != -1)
	{
		n_length[0] = n_check[0];
	}
	else
	{
		n_length[0] = 10;
		n_check[0] = 9;
	}

	n_start[1] = n_check[0] + 1;
	n_check[1] = strBody.Find("_", n_start[1]);
	n_length[1] = n_check[1] - n_start[1];

	n_start[2] = n_check[1] + 1;
	n_check[2] = strBody.Find("_", n_start[2]);
	n_length[2] = n_check[2] - n_start[2];

	n_start[3] = n_check[2] + 1;
	n_check[3] = strBody.Find("_", n_start[3]);
	n_length[3] = n_check[3] - n_start[3];

	n_start[4] = n_check[3] + 1;
	n_check[4] = strBody.Find("_", n_start[4]);
	n_length[4] = n_check[4] - n_start[4];

	if (nInfo < 4)
	{
		strFuncRet = strBody.Mid(n_start[nInfo], n_length[nInfo]);
	}
	else
	{
		strFuncRet = strBody.Mid(n_start[nInfo]);
	}
	return strFuncRet;
}

int CPublic_Function::FileSizeCheck(CString FileName, long size, int n_check)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;
	
	if (bContinue = finder.FindFile(FileName))
	{	
		if (n_check == CTL_YES)			// Size¸¦ Ã¼Å©ÇÏ´Â°ÍÀÌ¸é...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			if (Len > size)
			{
				flag = 1;
			}
			finder.Close();	
		}
		else						// Size¸¦ È®ÀÎÇÏ´Â°ÍÀÌ¸é...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			
			flag = Len;
			
			finder.Close();	
		}
	}
	
	return flag;
}

CString CPublic_Function::GetLocalMacAddress()
{
	NCB Ncb; 
	UCHAR uRetCode; 
	LANA_ENUM lenum; 
	int i; 
	CString str_value=""; 
	
	memset( &Ncb, 0, sizeof(Ncb) ); 
	Ncb.ncb_command = NCBENUM; 
	Ncb.ncb_buffer = (UCHAR *)&lenum; 
	Ncb.ncb_length = sizeof(lenum); 
	uRetCode = Netbios( &Ncb ); 
	
	for (i=0; i < lenum.length ;i++) 
	{ 
		memset( &Ncb, 0, sizeof(Ncb) ); 
		Ncb.ncb_command = NCBRESET; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		uRetCode = Netbios( &Ncb ); 
		memset( &Ncb, 0, sizeof (Ncb) ); 
		Ncb.ncb_command = NCBASTAT; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		strcpy( (char*)Ncb.ncb_callname, "* " ); 
		Ncb.ncb_buffer = (unsigned char *) &Adapter; 
		Ncb.ncb_length = sizeof(Adapter); 
		
		uRetCode = Netbios( &Ncb ); 
		
		if ( uRetCode == 0 ) 
		{ 
			str_value.Format("%02x-%02x-%02x-%02x-%02x-%02x", 
				Adapter.adapt.adapter_address[0], 
				Adapter.adapt.adapter_address[1], 
				Adapter.adapt.adapter_address[2], 
				Adapter.adapt.adapter_address[3], 
				Adapter.adapt.adapter_address[4], 
				Adapter.adapt.adapter_address[5] ); 
		} 
	} 	
	return str_value;
}



int CPublic_Function::Robot_Reinstatement_Position(int mode)
{
	int Ret = 0, i = 0, n_response = 0;
	int FuncRet = RET_PROCEED;
	int nRet = 0, nRet2 = 0;

	return RET_GOOD;

	CDialog_Select	select_dlg;
 
	if (mode == 0 && st_work.mn_reinstatement_mode == 0) // ¿ÏÀü ÃÊ±âÈ­ ÈÄ¸¸ °ü¸®ÇÔ 
	{	// ÇöÀç À§Ä¡ ¹é¾÷ 
		for (i = 0; i < MOTOR_COUNT; i++)
		{
// 			st_work.md_reinstatement_Pos[0][i] = st_motor[i].mcommandposbackup;
			st_work.md_reinstatement_Pos[0][i] = COMI.md_cmdpos_backup[i];
		}
						
		st_work.mn_reinstatement_mode = 1; //ÇÑ¹ø µ¥ÀÌÅÍ¸¦ ÀúÀåÇÑ´Ù 
		reinstatement_Step = 0;

		return RET_GOOD;
	}	 
	else if (mode == 0)
	{
		return RET_GOOD;
	}

	switch (reinstatement_Step)
	{
	case 0:
		if (st_work.mn_reinstatement_mode == 1) // ½ºÅ¸Æ® Å°¸¦ ´©¸£¸é 2ÀÌ´Ù 
		{
			reinstatement_Step = 1000;
		}
		break;

	case 1000:
		for (i = 0; i < MOTOR_COUNT; i++)
		{
			st_work.md_reinstatement_Pos[0][i] = COMI.Get_MotCurrentPos(i);
		}

		mn_front_change = 0;
		mn_rear_change = 0;
		
		//º¯°æµÈ À§Ä¡°ª Ãâ·Â
//		if (st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_X] > st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_X] + COMI.md_allow_value[MOTOR_ROBOT_X]
//			|| st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_X] < st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_X] - COMI.md_allow_value[MOTOR_ROBOT_X])
//		{
//			mn_front_change++;
//			sprintf(st_msg.c_abnormal_msg, "Robot X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_X], (long)st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_X], (long)COMI.md_allow_value[MOTOR_ROBOT_X]);
//		}
//		else if (st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_Y] > st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_Y] + COMI.md_allow_value[MOTOR_ROBOT_Y]
//			|| st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_Y] < st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_Y] - COMI.md_allow_value[MOTOR_ROBOT_Y])
//		{
//			sprintf(st_msg.c_abnormal_msg, "Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_Y], (long)st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_Y], (long)COMI.md_allow_value[MOTOR_ROBOT_Y]);
//		}
//		else if (st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_Z] > st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_Z] + COMI.md_allow_value[MOTOR_ROBOT_Z]
//			|| st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_Z] < st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_Z] - COMI.md_allow_value[MOTOR_ROBOT_Z])
//		{
//			mn_rear_change++;
//			sprintf(st_msg.c_abnormal_msg, "Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][MOTOR_ROBOT_Z], (long)st_work.md_reinstatement_Pos[1][MOTOR_ROBOT_Z], (long)COMI.md_allow_value[MOTOR_ROBOT_Z]);
//		}
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		st_work.mn_reinstatement_mode = 3; //ÃÊ±âÈ­ Á¤º¸¸¦ ¿Ï·áÇß´Ù, Å¬¸®¾î 

		if (mn_front_change > 0 && mn_rear_change > 0)
		{
			reinstatement_Step = 2000;
		}
		else if (mn_rear_change > 0)
		{
			reinstatement_Step = 2000;
		}
		else
		{
			reinstatement_Step = 2000;
		}
		break;

	case 1100:
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 1200;
		break;

	case 1200:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 500)
		{
			reinstatement_Step = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 1300:		
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 1400;
		break;
		
	case 1400:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 500)
		{
			reinstatement_Step = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 2000:
		//¿©±â¼­ ÀÌÀü¿¡ µ¿ÀÛÇß´ø À§Ä¡¸¦ ´Ù½Ã °è»êÇØ¾ßÇÑ´Ù..
		Robot_BackMovePosValCheck();

		for (i = 0; i < MOTOR_COUNT; i++)
		{
			COMI.Set_MotPower(i, IO_ON);
		}
		
		mn_check[0] = VARIABLE_INIT;
		mn_check[1] = VARIABLE_INIT;

		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 2010;
		break;

	case 2010:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 1000)
		{
			reinstatement_Step = 2020;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 2020:
//		nRet = COMI.HomeCheck_Mot(MOTOR_ROBOT_Z, MOT_TIMEOUT);
//		
//		if (nRet == CTLBD_RET_GOOD)
//		{
//			reinstatement_Step = 2100;
//		}		
//		else if (nRet != CTLBD_RET_PROCEED)
//		{
//			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
//			alarm.mstr_code = "000001";
//			sprintf(alarm.mc_code, "000001");
//			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; COMI.mn_run_status = CTL_dWARNING;
//			CtlBdFunc.Alarm_Error_Occurrence(10, alarm.mn_type_mode, COMI.mn_run_status, alarm.mstr_code);
//		}
		break;

	case 2100:
//		nRet = COMI.Start_SingleMove(MOTOR_ROBOT_Z, st_motor[MOTOR_ROBOT_Z].d_pos[P_ROBOT_Z_SAFETY], 10);
//		
//		if (nRet == CTLBD_RET_GOOD)
//		{
//			reinstatement_Step = 2200;
//		}
//		else if (nRet != CTLBD_RET_PROCEED)
//		{
//			// 000100 1 A "ROBOT Z Safety Pos Move Error."
//			alarm.mstr_code = "090100";
//			sprintf(alarm.mc_code, "090100");
//			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; COMI.mn_run_status = CTL_dWARNING;
//			CtlBdFunc.Alarm_Error_Occurrence(11, alarm.mn_type_mode, COMI.mn_run_status, alarm.mstr_code);
//		}
		break;
		
	case 2200:
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 2300;
		break;
		
	case 2300:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 500)
		{
			reinstatement_Step = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 50000:
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);

		reinstatement_Step = 0;
		st_handler.mn_reinstatement = CTL_NO;			// º¹±¸ µ¿ÀÛÀÌ ¾Æ´Ï´Ù.
		st_work.mn_reinstatement_mode = 0;	// ÃÊ±âÈ­ Á¤º¸¸¦ ¿Ï·áÇß´Ù, Å¬¸®¾î 		
		FuncRet = RET_GOOD;
		break;
	}
 	return FuncRet;
}

void CPublic_Function::Robot_BackMovePosValCheck()
{
	int i = 0;
	double d_gap_pos = 0;

// 	st_work.d_reinstatement_x_pos = 0;
// 	st_work.d_reinstatement_y_pos = 0;
// 	st_work.d_reinstatement_z_pos = st_motor[MOTOR_ROBOT_Z].d_pos[P_ROBOT_Z_SAFETY];
// 
// 	if (st_work.n_back_type == BACK_SET_POS)
// 	{
// 		switch (st_work.n_back_type)
// 		{
// 		case BT_LENGTH_VARIABLE:
// 			st_work.d_reinstatement_x_pos = st_motor[MOTOR_ROBOT_X].d_pos[P_ROBOT_Z_SAFETY];
// 			st_work.d_reinstatement_y_pos = st_motor[MOTOR_ROBOT_Y].d_pos[P_ROBOT_Z_SAFETY];
// 			break;
// 			
// 		case BT_SAFETY_ZONE:
// 			st_work.d_reinstatement_x_pos = st_motor[MOTOR_ROBOT_X].d_pos[P_ROBOT_Z_SAFETY];
// 			st_work.d_reinstatement_y_pos = st_motor[MOTOR_ROBOT_Y].d_pos[P_ROBOT_Z_SAFETY];
// 			break;
// 		}
// 	}
// 	else
// 	{
// // 		st_work.d_reinstatement_x_pos = Run_Sort_Robot_Manual.XYRobot_MovePosValCheck(st_work.n_back_type, M_ROBOT_X, st_work.n_back_x_pos);
// // 		st_work.d_reinstatement_y_pos = Run_Sort_Robot_Manual.XYRobot_MovePosValCheck(st_work.n_back_type, M_ROBOT_Y, st_work.n_back_y_pos);
// 		
// 		switch (st_work.n_back_type)
// 		{
// 		case BT_REAR_BUFFER:
// 			break;
// 		case BT_FRONT_BUFFER:
// 			break;
// 		case BT_RETEST_BUFFER:
// 			break;
// 		case BT_REJECT_BUFFER:
// 			break;
// 		case BT_LOB_BUFFER:
// 			break;
// 		case BT_TRAY_1:
// 			break;
// 		case BT_TRAY_2:
// 			break;
// 		case BT_CUSTOMER_BUFFER:
// 			break;
// 		case BT_LENGTH_VARIABLE:
// 			break;
// 		case BT_SAFETY_ZONE:
// 			break;
// 		}
// 	}
// 	
// 	// z axis´Â µû·Î ´Ù½ÃÇÑ¹øÇÑ´Ù...
// 	st_work.d_reinstatement_z_pos = st_motor[M_ROBOT_Z].d_pos[st_work.n_back_z_movepos];
}

void CPublic_Function::ThreadFunctionStepTrace(int nPos, int nStep)
{
	if (nStep == st_work.mn_prev_step[nPos]) // stepÀÌ µ¿ÀÏÇÏ±º~
	{
		return;
	}

// 	CString str_ybs;
// 	str_ybs.Format("[Pos:%02d] Prev : %d -> now %d",nPos,st_work.mn_prev_step[nPos],nStep);
// 	Func.On_LogFile_Add(3, str_ybs);


	st_work.mn_prev_step[nPos] = nStep;
	StepChangeTime[nPos] = CTime::GetCurrentTime();	 
	st_work.mstr_last_change_date[nPos] = StepChangeTime[nPos].Format("%y/%m/%d %H:%M:%S");	
	
	if (st_work.mstr_history_step[nPos].GetLength() == 120 || st_work.mstr_history_step[nPos].GetLength() == 276)
	{
		str_step_temp[nPos].Format("%05d|", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];
		st_work.mstr_history_step[nPos] += "\r\n|                          |";
	}
	else if (st_work.mstr_history_step[nPos].GetLength() == 432)
	{
		str_step_temp[nPos].Format("%05d|", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];

		st_work.mstr_history_step_back_up[nPos] = st_work.mstr_history_step[nPos];
		st_work.mstr_history_step[nPos] = "";
	}
	else
	{
		str_step_temp[nPos].Format("%05d,", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];
	}
}

void CPublic_Function::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line="", str_temp_data_temp="", str_content="";
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch (dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";   // +----------------------------------------+----------------------------------+
		break;
		
	case 15:
		str_temp_data_temp.Format("%33s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+----------------------------------+";
		break;
		
	case 24:
		str_temp_data_temp.Format("%24s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 131:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-----------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 51:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 49:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;
		
	case 50:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;

	case 130:
		str_temp_data_temp.Format("%124s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}

void CPublic_Function::DB_Write_Alarm(st_alarm_database st_alarmdb)
{	
	if (st_basic.n_mode_device == CTL_NO)			return;

	g_cs.EnterCriticalSection();

	CString strTmp[6];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_alarmdb.mole_date.GetYear(), 
		st_alarmdb.mole_date.GetMonth(),
		st_alarmdb.mole_date.GetDay(),
		st_alarmdb.mole_date.GetHour(),
		st_alarmdb.mole_date.GetMinute(),
		st_alarmdb.mole_date.GetSecond());
	
	
	strTmp[2].Format("%-1s", st_alarmdb.mstrlot_no);
	strTmp[3].Format("%-1s", st_alarmdb.mstrpart_no);
	
	strTmp[4].Format("%-1s", st_alarmdb.mstrAlarmCode);
	strTmp[5].Format("%-1s", st_alarmdb.mstrAlarmMsg);
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8590"))
		{	
			if (Mysql.Table_Select("AMT8590", "ALARM_DATA"))
			{
				Mysql.Record_Add("ALARM_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8590", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
				{
					Mysql.Record_Add("ALARM_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8590"))
			{
				if (Mysql.Table_Create("AMT8590", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
				{
					Mysql.Record_Add("ALARM_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();

	g_cs.LeaveCriticalSection();
}

void CPublic_Function::DB_Write_Ground(st_ground_database st_grounddb)
{
	CString strTmp[7];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_grounddb.mole_date.GetYear(), 
		st_grounddb.mole_date.GetMonth(),
		st_grounddb.mole_date.GetDay(),
		st_grounddb.mole_date.GetHour(),
		st_grounddb.mole_date.GetMinute(),
		st_grounddb.mole_date.GetSecond());
		
	strTmp[2].Format("%-1s", st_grounddb.mstrChannel[0]);
	strTmp[3].Format("%-1s", st_grounddb.mstrChannel[1]);
	strTmp[4].Format("%-1s", st_grounddb.mstrChannel[2]);
	strTmp[5].Format("%-1s", st_grounddb.mstrChannel[3]);
	strTmp[6].Format("%-1s", st_grounddb.mstrChannel[4]);

	if ((strTmp[2] == "" || strTmp[2] == " ") && (strTmp[3] == "" || strTmp[3] == " ") && (strTmp[4] == "" || strTmp[4] == " ")
		&& (strTmp[5] == "" || strTmp[5] == " ") && (strTmp[6] == "" || strTmp[6] == " "))
	{
		return;
	}

	g_cs.EnterCriticalSection();

	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8590"))
		{	
			if (Mysql.Table_Select("AMT8590", "GROUND_DATA"))
			{
				Mysql.Record_Add("GROUND_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8590", "GROUND_DATA", 7, Mysql.mstr_ground_table_name, Mysql.mn_ground_table_type, Mysql.mn_ground_table_num))
				{
					Mysql.Record_Add("GROUND_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8590"))
			{
				if (Mysql.Table_Create("AMT8590", "GROUND_DATA", 7, Mysql.mstr_ground_table_name, Mysql.mn_ground_table_type, Mysql.mn_ground_table_num))
				{
					Mysql.Record_Add("GROUND_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();

	g_cs.LeaveCriticalSection();
}

void CPublic_Function::DB_Write_Buffer(st_buffer_database st_bufferdb)
{
	CString strTmp[9];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_bufferdb.mole_date.GetYear(), 
		st_bufferdb.mole_date.GetMonth(),
		st_bufferdb.mole_date.GetDay(),
		st_bufferdb.mole_date.GetHour(),
		st_bufferdb.mole_date.GetMinute(),
		st_bufferdb.mole_date.GetSecond());
	
	strTmp[2].Format("%-1s", st_bufferdb.mstrlot_no);
	strTmp[3].Format("%-1s", st_bufferdb.mstrpart_no);
	strTmp[4].Format("%d", st_bufferdb.mnbuffer_pos);
	strTmp[5].Format("%d", st_bufferdb.mnlast_module);
	strTmp[6].Format("%d", st_bufferdb.mnbypass);
	strTmp[7].Format("%d", st_bufferdb.mnmodule_cnt);
	strTmp[8].Format("%d", st_bufferdb.mnretest);
	
	g_cs.EnterCriticalSection();
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8590"))
		{	
			if (Mysql.Table_Select("AMT8590", "BUFFER_DATA"))
			{
				Mysql.Record_Add("BUFFER_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8590", "BUFFER_DATA", 9, Mysql.mstr_buffer_table_name, Mysql.mn_buffer_table_type, Mysql.mn_buffer_table_num))
				{
					Mysql.Record_Add("BUFFER_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8590"))
			{
				if (Mysql.Table_Create("AMT8590", "BUFFER_DATA", 9, Mysql.mstr_buffer_table_name, Mysql.mn_buffer_table_type, Mysql.mn_buffer_table_num))
				{
					Mysql.Record_Add("BUFFER_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();
	
	g_cs.LeaveCriticalSection();
}

CString CPublic_Function::GetStrCurrentTime()
{
	COleDateTime timeCur;
	int iYear, iMonth, iDay, iHour, iMin, iSec;
	CString strTime="";
	
	timeCur = COleDateTime::GetCurrentTime();

	iYear = timeCur.GetYear();
	iMonth = timeCur.GetMonth();
	iDay = timeCur.GetDay();

	iHour = timeCur.GetHour();
	iMin = timeCur.GetMinute();
	iSec = timeCur.GetSecond();

	strTime.Format("%04d/%02d/%02d_%02d:%02d:%02d", iYear, iMonth, iDay, iHour, iMin, iSec );
	return strTime;
}

void CPublic_Function::OnLogGalmg_PushLot( CString strlot, CString strtmp )
{
	CString strTemp="";
	strTemp.Format( "[PUSH LOT] (%s) 0: [%s] / 1: [%s] / 2: [%s] : %s", strlot, st_handler.mstr_lot_name[0], st_handler.mstr_lot_name[1], st_handler.mstr_lot_name[2], strtmp );
	On_LogFile_Add(3, strTemp);
}

void CPublic_Function::OnLogWorkStep( CString strClass, CString strFunc, int iOld, int iCur )
{
	CString strLog="";
	strLog.Format( "%s	%s			%d -------> %d", strClass, strFunc, iOld, iCur );
	On_LogFile_Add( LOG_WORK, strLog );
}

void CPublic_Function::MsgLog( LPCTSTR strMsg )
{
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_abnormal_msg, strMsg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
}

void CPublic_Function::DeleteOldFiles( LPCTSTR lpDirPath, int iDays )
{
	if ( lpDirPath == NULL )
		return;

	CString strNextDirPath = "";
	CFileFind find;
	BOOL bFind = find.FindFile( lpDirPath );

	if ( bFind == FALSE )
		return;

	while( bFind )
	{
		bFind = find.FindNextFile();

		// . or .. pass
		if ( find.IsDots() == TRUE )
			continue;

		if ( find.IsDirectory() )
		{
			CString strDir;
			strDir.Format("%s\\*.*", find.GetFilePath() );
			DeleteOldFiles( strDir, iDays );
		}
		else
		{
			HANDLE hF = CreateFile(find.GetFileName(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

			SYSTEMTIME st;
			GetLocalTime(&st);
			
			CTime tNow(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
			CTime tTime;
			find.GetLastWriteTime(tTime);
			
			CTimeSpan span = tNow - tTime;
			int iDayss = span.GetDays();

			if ( span.GetDays() > iDays )
			{
				DeleteFile( find.GetFilePath() );

				CString strLog;
				strLog.Format("Delete File - Lost %d Days : %s", iDays, find.GetFilePath() );
				Func.On_LogFile_Add(3, strLog);
			}
		}
	}

	find.Close();
}

void CPublic_Function::SetRBufferStation( int iPos, int iVal, bool bWrite )
{
	if ( st_sync.mn_rbuffer_station[iPos] != iVal )
	{
		st_sync.mn_rbuffer_station[iPos] = iVal;

		if ( st_handler.cwnd_main != NULL )
		{
			st_handler.cwnd_main->SendMessage( WM_DRAW_WORK_SITE, 0, 0 );
		}

		if ( bWrite  )
		{
			std::string strPos = "Front";
			if ( iPos == 1 )
				strPos = "Rear";
			
//			char strKey[256];
//			sprintf( strKey, "%s%s_RBuffer", REG_KEY_SITE, strPos.c_str() );
//			AMTRegistry::RegWriteInt( strKey, REG_VALUE_RBUFFER_STATION, st_sync.mn_rbuffer_station[iPos] );
		}
	}
}

bool CPublic_Function::CheckWorkingLot( CString strLotNo )
{
	for( int i=0; i<3; i++ )
	{
		if ( st_handler.mstr_lot_name[i] == strLotNo )
			return true;
	}
	return false;
}

void CPublic_Function::On_Log_Title_Msg( int iLogType, CString strTitle, CString strMsg )
{
	CSingleLock singlog(&plogmutex);
	
	singlog.Lock();
	
	CString strTemp;
	strTemp.Format("|%s| [%s]", strTitle, strMsg);
	On_LogFile_Add( iLogType, strTemp );

	singlog.Unlock();
}

void CPublic_Function::SetActionParam()
{
	pMainFrm = (CMainFrame*)::AfxGetMainWnd();   ////////ÇÁ·Î±×·¥ÀÌ ½ÇÇà µÉ¶§...MainFrmÀ» ÃÊ±âÈ­ ÇØÁØ´Ù..
	ASSERT(pMainFrm);
}

//==================================================================//
// ÇÁ·Î¼¼½ºÀÇ ±ÇÇÑ ¼³Á¤
//==================================================================//
char CPublic_Function::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;
	
	// ÇöÀç ÇÁ·Î¼¼½ºÀÇ ±ÇÇÑ°ú °ü·ÃµÈ Á¤º¸¸¦ º¯°æÇÏ±â À§ÇØ ÅäÅ«Á¤º¸¸¦ ¿¬´Ù
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &h_token))
	{
		// ±ÇÇÑ°ú °ü·ÃµÈ Á¤º¸ Á¢±Ù¿¡ ½ÇÆÐÇÔ
		return 0;
	}
	
	// ÇöÀç ÇÁ·Î¼¼½º°¡ SE_SHUTDOWN_NAME ±ÇÇÑÀ» »ç¿ëÇÒ¼ö ÀÖµµ·Ï ¼³Á¤ÇÑ´Ù
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	// ÁöÁ¤ÇÑ °ªÀ¸·Î ±ÇÇÑÀ» Á¶Á¤ÇÑ´Ù
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
	{
		// ±ÇÇÑ Á¶Á¤¿¡ ½ÇÆÐÇÑ °æ¿ì
		return 0;
	}
	return 1;
}

void CPublic_Function::DateTimeChange(CString sDateTime)
{
	// ½Ã°£ º¯°æ
	SYSTEMTIME csTime;
	csTime.wYear	= atoi(sDateTime.Mid(0,4));
	csTime.wMonth	= atoi(sDateTime.Mid(4,2));
	csTime.wDay		= atoi(sDateTime.Mid(6,2));
	csTime.wHour	= atoi(sDateTime.Mid(8,2));
	csTime.wMinute	= atoi(sDateTime.Mid(10,2));
	csTime.wSecond	= atoi(sDateTime.Mid(12,2));
	csTime.wMilliseconds = 0;
	
	GetNtPrivilege();  // ÇÁ·Î¼¼½ºÀÇ ±ÇÇÑ ¼³Á¤
	
	if (SetLocalTime(&csTime))
	{
		sprintf(st_msg.c_normal_msg, "½Ã½ºÅÛ ³¯Â¥°¡ º¯°æµÇ¾ú½À´Ï´Ù. : %s", sDateTime);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
	else
	{
		sprintf(st_msg.c_abnormal_msg, "½Ã½ºÅÛ ³¯Â¥¸¦ º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù. : %s", sDateTime);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
}

void CPublic_Function::OnLotDataSave()
{
	CString mstr_file_name="";    // ¸¶Áö¸·À¸·Î »ý¼ºµÈ ÆÄÀÏ ÀÌ¸§ ÀúÀå º¯¼ö 
	CString mstr_create_file="";  // ¾Ë¶÷ Á¤º¸ ÀúÀåÇÒ ÆÄÀÏ¿¡ ´ëÇÑ [Æú´õ]+[ÆÄÀÏ¸í]+[È®ÀåÀÚ] ¼³Á¤ º¯¼ö 
	CString mstr_list_name="", mstr_temp_data="";  // °¢ ºÎºÐº° ¾Ë¶÷ ¹ß»ý È½¼ö Á¤º¸ ÀúÀåÇÒ ·Î±× ÆÄÀÏ Á¤º¸ ÀúÀå º¯¼ö 
	CString mstr_content="";  // ÇÑ ¶óÀÎ¿¡ Ãâ·ÂÇÒ ¹®ÀÚ¿­ Á¤º¸ ÀúÀå º¯¼ö 
	int mn_existence;      // ÆÄÀÏ Á¸Àç À¯¹« ¼³Á¤ ÇÃ·¡±× 
	char fileName[256];    // °Ë»öÇÒ ÆÄÀÏ Á¤º¸ ¼³Á¤ ÇÔ¼ö 
	FILE  *fp ;            // ÆÄÀÏ¿¡ ´ëÇÑ Æ÷ÀÎÅÍ ¼³Á¤ º¯¼ö 
	
	CString str_title_time="", str_file_time="", str_new_file="";		// ÆÄÀÏ¿¡ Ãâ·ÂÇÒ Å¸ÀÌÆ² ½Ã°£ ÀúÀå º¯¼ö
	int n_cur_year, n_cur_month, n_cur_day;		// ³â, ¿ù, ÀÏ ÀúÀå º¯¼ö
	int n_cur_hour, n_cur_minute, n_cur_second;		// ³â, ¿ù, ÀÏ ÀúÀå º¯¼ö
	
	COleDateTime mtime_cur;						// ÇöÀç ½Ã°£ ÀúÀå º¯¼ö
	CTime mp_time_cur;							// Å¸ÀÌÆ² ½Ã°£ ÀúÀå º¯¼ö

	CString str_temp_data="", strTemp="";

	CString strData="", str_time="";
	
	/* ************************************************************************** */
	/* ÆÄÀÏ ÀÌ¸§À¸·Î »ç¿ëÇÒ ³¯Â¥ ¼³Á¤ÇÑ´Ù                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("LOTDATA%04d%02d%02d[%s]", n_cur_year, n_cur_month, n_cur_day, st_handler.mstr_lot_name[0]);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_path.str_lot_data_path + str_new_file;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* ÆÄÀÏ ¹ÌÁ¸Àç */
	{
		mstr_create_file = st_path.str_lot_data_path + str_new_file;
		mstr_create_file += ".TXT";
	}
	
	/* ************************************************************************** */
	/* ¾Ë¶÷ ¹ß»ý È½¼ö Á¤º¸ ÀúÀå ÆÄÀÏ¿¡ Ãß°¡ °¡´ÉÇÑ ÇüÅÂ ÆÄÀÏ·Î »ý¼º               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("3.File Open Failed!..");
		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ÆÄÀÏ¿¡ Å¸ÀÌÆ² Á¤º¸ Ãâ·ÂÇÑ´Ù                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT8590 [SSD Packing Handler] : " + g_ver.GetDate() );
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Lot Data Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_path.str_lot_data_path + str_new_file);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Lot Name  : " + st_handler.mstr_lot_name[0]);
	fprintf(fp,"%s\r\n", mstr_content) ;

	if (st_handler.str_main_part.GetLength() >= 16)
	{
		mstr_content.Format("| %-100s |", "Part No.  : " + st_handler.str_main_part.Mid(0, 16));
		fprintf(fp,"%s\r\n", mstr_content) ;
	}

	strTemp.Format("%06d", st_work.nMdlInputCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Input    : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlPassCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Pass     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlRejectCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Reject   : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlBtoCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Bto      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlNtoCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Nto      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlHtoCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Hto      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlFailCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Fail     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	if (st_work.nMdlPassCount[0][0] == 0)
	{
		st_work.fMdlyield[0][0] = 0;
	}
	else
	{
		st_work.fMdlyield[0][0] = (double)((double)st_work.nMdlPassCount[0][0] / (double)((double)st_work.nMdlPassCount[0][0] + (double)st_work.nMdlRejectCount[0][0])) * (double)100;
	}
	
	strTemp.Format("%3.2f%%", st_work.fMdlyield[0][0]);
	
	mstr_content.Format("| %-100s |", "Prime Yield    : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content);

	mstr_content.Format("| %-100s |", "" + strTemp);
	fprintf(fp,"%s\r\n", mstr_content);

	strTemp.Format("%06d", st_work.nMdlInputCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Input      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlPassCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Pass       : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlRejectCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Reject     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlBtoCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Bto        : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlNtoCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Nto        : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlHtoCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Hto        : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlFailCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Fail       : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	if (st_work.nMdlPassCount[1][0] == 0)
	{
		st_work.fMdlyield[1][0] = 0;
	}
	else
	{
		st_work.fMdlyield[1][0] = (double)((double)st_work.nMdlPassCount[1][0] / (double)((double)st_work.nMdlPassCount[1][0] + (double)st_work.nMdlRejectCount[1][0])) * (double)100;
	}
	
	strTemp.Format("%3.2f%%", st_work.fMdlyield[1][0]);

	mstr_content.Format("| %-100s |", "Cum Yield      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	str_temp_data = "";
	
	if (ferror(fp))  
	{
		Func.MsgLog("file save failed!..") ;
		fclose(fp);
		return ;
	}
	
	fclose(fp);  /* ÆÄÀÏ Á¾·á */
}

void CPublic_Function::OnDailyDataSave()
{
	CString mstr_file_name="";    // ¸¶Áö¸·À¸·Î »ý¼ºµÈ ÆÄÀÏ ÀÌ¸§ ÀúÀå º¯¼ö 
	CString mstr_create_file="";  // ¾Ë¶÷ Á¤º¸ ÀúÀåÇÒ ÆÄÀÏ¿¡ ´ëÇÑ [Æú´õ]+[ÆÄÀÏ¸í]+[È®ÀåÀÚ] ¼³Á¤ º¯¼ö 
	CString mstr_list_name="", mstr_temp_data="";  // °¢ ºÎºÐº° ¾Ë¶÷ ¹ß»ý È½¼ö Á¤º¸ ÀúÀåÇÒ ·Î±× ÆÄÀÏ Á¤º¸ ÀúÀå º¯¼ö 
	CString mstr_content="";  // ÇÑ ¶óÀÎ¿¡ Ãâ·ÂÇÒ ¹®ÀÚ¿­ Á¤º¸ ÀúÀå º¯¼ö 
	int mn_existence;      // ÆÄÀÏ Á¸Àç À¯¹« ¼³Á¤ ÇÃ·¡±× 
	char fileName[256];    // °Ë»öÇÒ ÆÄÀÏ Á¤º¸ ¼³Á¤ ÇÔ¼ö 
	FILE  *fp ;            // ÆÄÀÏ¿¡ ´ëÇÑ Æ÷ÀÎÅÍ ¼³Á¤ º¯¼ö 
	
	CString str_title_time="", str_file_time="", str_new_file="";		// ÆÄÀÏ¿¡ Ãâ·ÂÇÒ Å¸ÀÌÆ² ½Ã°£ ÀúÀå º¯¼ö
	int n_cur_year, n_cur_month, n_cur_day;		// ³â, ¿ù, ÀÏ ÀúÀå º¯¼ö
	int n_cur_hour, n_cur_minute, n_cur_second;		// ³â, ¿ù, ÀÏ ÀúÀå º¯¼ö
	
	COleDateTime mtime_cur;						// ÇöÀç ½Ã°£ ÀúÀå º¯¼ö
	CTime mp_time_cur;							// Å¸ÀÌÆ² ½Ã°£ ÀúÀå º¯¼ö

	CString str_temp_data="", strTemp="";

	CString strData="", str_time="";

	float fMdlyield;
	
	/* ************************************************************************** */
	/* ÆÄÀÏ ÀÌ¸§À¸·Î »ç¿ëÇÒ ³¯Â¥ ¼³Á¤ÇÑ´Ù                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("DAY_DATA%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_path.str_lot_data_path + str_new_file;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* ÆÄÀÏ ¹ÌÁ¸Àç */
	{
		mstr_create_file = st_path.str_lot_data_path + str_new_file;
		mstr_create_file += ".TXT";
	}
	
	/* ************************************************************************** */
	/* ¾Ë¶÷ ¹ß»ý È½¼ö Á¤º¸ ÀúÀå ÆÄÀÏ¿¡ Ãß°¡ °¡´ÉÇÑ ÇüÅÂ ÆÄÀÏ·Î »ý¼º               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("4.ÆÄÀÏ ¿­±â ½ÇÆÐ!..");
		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ÆÄÀÏ¿¡ Å¸ÀÌÆ² Á¤º¸ Ãâ·ÂÇÑ´Ù                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT8590 [SSD Packing Handler] : " + g_ver.GetDate() );
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Day Data Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_path.str_lot_data_path + str_new_file);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", g_lotMgr.GetInputCnt(PRIME));
	mstr_content.Format("| %-100s |", "Input Prime : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", g_lotMgr.GetPassCnt(PRIME));
	mstr_content.Format("| %-100s |", "Pass  Prime : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", g_lotMgr.GetFailCnt(PRIME));
	mstr_content.Format("| %-100s |", "Fail Prime : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content);

	fMdlyield = 0;
	
	if (g_lotMgr.GetPassCnt(PRIME) == 0)
	{
		fMdlyield = 0;
	}
	else
	{
		fMdlyield = (double)g_lotMgr.GetPassCnt(PRIME) / (double)g_lotMgr.GetInputCnt(PRIME) * 100.0f;
	}
	
	strTemp.Format("%3.2f%%", fMdlyield);
	
	mstr_content.Format("| %-100s |", "Prime Yield  : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;


	// CUM
	strTemp.Format("%06d", g_lotMgr.GetInputCnt(CUM));
	mstr_content.Format("| %-100s |", "Input Cum   : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", g_lotMgr.GetPassCnt(CUM));
	mstr_content.Format("| %-100s |", "Pass  Cum   : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", g_lotMgr.GetFailCnt(CUM));
	mstr_content.Format("| %-100s |", "Fail Cum  : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	fMdlyield = 0;

	if (g_lotMgr.GetPassCnt(CUM) == 0)
	{
		fMdlyield = 0;
	}
	else
	{
		fMdlyield = (double)g_lotMgr.GetPassCnt(CUM) / (double)g_lotMgr.GetInputCnt(CUM) * 100.0f;
	}
	
	strTemp.Format("%3.2f%%", fMdlyield);
	
	mstr_content.Format("| %-100s |", "Cum   Yield  : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	mstr_content.Format("| %-100s |", "");
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds() );
	mstr_content.Format("| %-100s |", "Run Time     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDI.GetTotalHours(), st_handler.m_tDI.GetMinutes(), st_handler.m_tDI.GetSeconds() );
	mstr_content.Format("| %-100s |", "RunDown Time : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDC.GetTotalHours(), st_handler.m_tDC.GetMinutes(), st_handler.m_tDC.GetSeconds() );
	mstr_content.Format("| %-100s |", "JobChange Time: " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds() );
	mstr_content.Format("| %-100s |", "Stop Time    : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds() );
	mstr_content.Format("| %-100s |", "Alarm Time   : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDM.GetTotalHours(), st_handler.m_tDM.GetMinutes(), st_handler.m_tDM.GetSeconds() );
	mstr_content.Format("| %-100s |", "Maint Time   : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format( "%d", g_handler.GetAlarmCnt() );
	mstr_content.Format("| %-100s |", "Alarm Count  : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	int nSec_MTBI = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();
	if( g_handler.GetAlarmCnt() == 0 )
		nSec_MTBI = 0;
	else
	{
		nSec_MTBI /= g_handler.GetAlarmCnt();
	}

	strTemp.Format( "%02d:%02d:%02d", nSec_MTBI / 3600, (nSec_MTBI % 3600) / 60, nSec_MTBI % 60 );
	mstr_content.Format("| %-100s |", "MTBI : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	str_temp_data = "";
	
	if (ferror(fp))  
	{
		Func.MsgLog("DailyData file save failed!..") ;
		fclose(fp);
		return ;
	}
	
	fclose(fp);  /* ÆÄÀÏ Á¾·á */
}

void CPublic_Function::OnDayDataReset()
{
	CString mstr_temp="";

	st_handler.m_tDR = 0;
	st_handler.m_tDI = 0;
	st_handler.m_tDC = 0;
	st_handler.m_tDJ = 0;
	st_handler.m_tDM = 0;
	st_handler.m_tDS = 0;

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDI);
	:: WritePrivateProfileString("BasicData", "Daily_Idle_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDC);
	:: WritePrivateProfileString("BasicData", "Daily_JobChange_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	g_lotMgr.ClearCountData();
	g_handler.ClearAlarmCnt();

	if (st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
	}
	
}

void CPublic_Function::GlobalDataSerialize(CArchive &ar)
{
	int i=0; int j=0; int k=0; int l=0; int m=0, nCnt = 0;
	CString strLotID="", strPartID="", strLastModule="", strRProtyID="", strTotal="", strByPass="", strLabelCode="", strRibbonCode="", strDenCapa="";

	CObject::Serialize(ar);
	
	if (ar.IsStoring())
	{
		ar << st_handler.mn_menu_num;					// °¢ È­¸é¿¡ ¼³Á¤µÈ È­¸é ¹øÈ£ ÀúÀå º¯¼ö 
		ar << st_handler.b_program_exit;				// ÇÁ·Î±×·¥À» ºüÁ®³ª°¡´Â°ÇÁö È®ÀÎÇÑ´Ù.
		
		// *************************************************************************
		// Àåºñ »óÅÂ Á¤º¸ ¼³Á¤ º¯¼ö ¼±¾ð                                            
		// *************************************************************************
		ar << st_handler.mn_initial;					// ÃÊ±âÈ­ ÀÛ¾÷ ¿Ï·á ÇÃ·¡±×
		ar << st_handler.mn_manual;

		ar << st_handler.mn_menu_lock;					// ¸Þ´º »ç¿ë °¡´É »óÅÂ ÇÃ·¡±× 
		ar << st_handler.mn_system_lock;				// SYSTEM LOCK »óÅÂ ÇÃ·¡±×
		ar << st_handler.mn_switch_lock;				// Dialog°¡ ¶° ÀÖ´Â »óÅÂ ÇÃ·¡±× (Start Switch Lock)

		ar << st_handler.mstr_pass_level;
		ar << st_handler.mn_level_maint;				// ¸ÞÀÎÆ® ·¹º§(Level 1) ¼³Á¤ ÇÃ·¡±×
		ar << st_handler.mn_level_teach;				// Æ¼Äª ·¹º§(Level 2) ¼³Á¤ ÇÃ·¡±×
		ar << st_handler.mn_level_speed;				// ½ºÇÇµå ·¹º§(Level 3) ¼³Á¤ ÇÃ·¡±×
		ar << st_handler.mn_level_admin;				// Admin ·¹º§(Level 4) ¼³Á¤ ÇÃ·¡±×

		for (i = 0; i < MAXSITE; i++)
		{
			ar << st_handler.mn_init_state[i];			// ÃÊ±âÈ­ ¿Ï·á ÇÃ·¡±×
		}

		ar << st_handler.mn_initial_error;

		int nCnt = g_lotMgr.GetLotCount();
		ar << nCnt;
		for ( i = 0; i < nCnt; i++ )
		{
			ar << g_lotMgr.GetLotAt(i).GetLotID();
			ar << g_lotMgr.GetLotAt(i).GetPartID();
			ar << g_lotMgr.GetLotAt(i).GetStrLastModule();
//			ar << g_lotMgr.GetLotAt(i).GetTimeStart();
			ar << g_lotMgr.GetLotAt(i).GetStrTotalModule();
			ar << g_lotMgr.GetLotAt(i).GetStrBypass();
			ar << g_lotMgr.GetLotAt(i).GetLabelCode();
			ar << g_lotMgr.GetLotAt(i).GetRibbonCode();
			ar << g_lotMgr.GetLotAt(i).GetDenLabelCapa();
		}
	}
	else
	{
		ar >> st_handler.mn_menu_num;					// °¢ È­¸é¿¡ ¼³Á¤µÈ È­¸é ¹øÈ£ ÀúÀå º¯¼ö 
		ar >> st_handler.b_program_exit;				// ÇÁ·Î±×·¥À» ºüÁ®³ª°¡´Â°ÇÁö È®ÀÎÇÑ´Ù.
		
		// *************************************************************************
		// Àåºñ »óÅÂ Á¤º¸ ¼³Á¤ º¯¼ö ¼±¾ð                                            
		// *************************************************************************
		ar >> st_handler.mn_initial;					// ÃÊ±âÈ­ ÀÛ¾÷ ¿Ï·á ÇÃ·¡±×
		ar >> st_handler.mn_manual;

		ar >> st_handler.mn_menu_lock;					// ¸Þ´º »ç¿ë °¡´É »óÅÂ ÇÃ·¡±× 
		ar >> st_handler.mn_system_lock;				// SYSTEM LOCK »óÅÂ ÇÃ·¡±×
		ar >> st_handler.mn_switch_lock;				// Dialog°¡ ¶° ÀÖ´Â »óÅÂ ÇÃ·¡±× (Start Switch Lock)

		ar >> st_handler.mstr_pass_level;
		ar >> st_handler.mn_level_maint;				// ¸ÞÀÎÆ® ·¹º§(Level 1) ¼³Á¤ ÇÃ·¡±×
		ar >> st_handler.mn_level_teach;				// Æ¼Äª ·¹º§(Level 2) ¼³Á¤ ÇÃ·¡±×
		ar >> st_handler.mn_level_speed;				// ½ºÇÇµå ·¹º§(Level 3) ¼³Á¤ ÇÃ·¡±×
		ar >> st_handler.mn_level_admin;				// Admin ·¹º§(Level 4) ¼³Á¤ ÇÃ·¡±×

		for (i = 0; i < MAXSITE; i++)
		{
			ar >> st_handler.mn_init_state[i];			// ÃÊ±âÈ­ ¿Ï·á ÇÃ·¡±×
		}

		ar >> st_handler.mn_initial_error;

		ar >> nCnt;

		for ( i =0; i < nCnt; i++)
		{
			ar >> strLotID;
			ar >> strPartID;
			g_lotMgr.AddLot(strLotID, strPartID);
			ar >> strLastModule;
			g_lotMgr.GetLotAt(i).SetLastModule(strLastModule);
			ar >> strTotal;
			g_lotMgr.GetLotAt(i).SetTotalModule(strTotal);
			ar >> strByPass;
			g_lotMgr.GetLotAt(i).SetByPass(strByPass);
			ar >> strLabelCode;
			g_lotMgr.GetLotAt(i).SetDisLabelCode(strLabelCode);
			ar >> strRibbonCode;
			g_lotMgr.GetLotAt(i).SetRibbonCode(strRibbonCode);
			ar >> strDenCapa;
			g_lotMgr.GetLotAt(i).SetDenLabelCapa(strDenCapa);
		}

	}
}

void CPublic_Function::Handler_Recovery_Data_Write()
{	
	int nFuncRet, nRet, nCancleFlag=CTL_NO;
	
	:: WritePrivateProfileString("SaveMode", "RecoveryMode", LPCTSTR("1"), st_path.mstr_file_basic);
	
	do{
		nFuncRet = Handler_Recovery_Data(DATA_WRITE);
		
		if (nFuncRet == CTL_ERROR)
		{
			nRet = AfxMessageBox("Recovery Data SaveµµÁß¿¡ ¿¡·¯°¡ ¹ß»ýÇÏ¿´½À´Ï´Ù. Ãë¼ÒÇÏ¸é º¹±¸ µ¥ÀÌÅÍ ÀúÀåÀº Ãë¼Ò µË´Ï´Ù. Àç½ÃµµÇÏ½Ã°Ú½À´Ï±î ?", MB_OKCANCEL);
			if(nRet != IDOK)
			{
				nCancleFlag = CTL_YES;
			}
		}
		else
			
			if(nCancleFlag == CTL_YES)
			{
				:: WritePrivateProfileString("SaveMode", "RecoveryMode", LPCTSTR("0"), st_path.mstr_file_basic);
			}
			
	}
	while(nFuncRet == CTL_ERROR && nCancleFlag == CTL_NO);
}

void CPublic_Function::DB_Write_Boat(st_boat_database st_boatdb)
{
	CString strTmp[14];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_bufferdb.mole_date.GetYear(), 
		st_bufferdb.mole_date.GetMonth(),
		st_bufferdb.mole_date.GetDay(),
		st_bufferdb.mole_date.GetHour(),
		st_bufferdb.mole_date.GetMinute(),
		st_bufferdb.mole_date.GetSecond());
	
	strTmp[2].Format("%-1s", st_boatdb.mstrlot_no);
	strTmp[3].Format("%-1s", st_boatdb.mstrpart_no);
	strTmp[4].Format("%d", st_boatdb.mn_boat_id);
	strTmp[5].Format("%d", st_boatdb.mn_product_id);
	strTmp[6].Format("%d", st_boatdb.mn_pcb_cnt);

	for (int i = 0; i < 8; i++)
	{
		strTmp[7 + i].Format("%d", st_boatdb.mn_bin[i]);
	}
	
	g_cs.EnterCriticalSection();
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8590"))
		{	
			if (Mysql.Table_Select("AMT8590", "BOAT_DATA"))
			{
				Mysql.Record_Add("BOAT_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8590", "BOAT_DATA", 14, Mysql.mstr_boat_table_name, Mysql.mn_boat_table_type, Mysql.mn_boat_table_num))
				{
					Mysql.Record_Add("BOAT_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8590"))
			{
				if (Mysql.Table_Create("AMT8590", "BOAT_DATA", 14, Mysql.mstr_boat_table_name, Mysql.mn_boat_table_type, Mysql.mn_boat_table_num))
				{
					Mysql.Record_Add("BOAT_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();
	
	g_cs.LeaveCriticalSection();
}

BOOL CPublic_Function::Handler_Recovery_Data_Read()
{
//	////////////////////////////////////////////////////////////////////////////////////
//	//james 2014,1002  º¹±¸ ¸ðµå¸¦ È®ÀÎ ÇÑ´Ù.
//	////////////////////////////////////////////////////////////////////////////////////
//	int i, nFlag = 0;
//	int n_response;
//	char chr_data[25];
//	CDialog_Select select_dlg;
//
//	:: GetPrivateProfileString("SaveMode", "RecoveryMode", "0", chr_data, 20, st_path.mstr_basic);
//	st_var.l_handler_recovery_flag[0] = long(atoi(chr_data));
//
//	//[0]=>º¹±¸¸ðµå ÀúÀå ¸ðµå(0:ÀúÀåµÇÁö ¾ÊÀº »óÅÂ, 1:º¹±¸¸ðµå°¡ ÀúÀå »óÅÂ(ÀÌ¶§¸¸ ÃÊ±âÈ­½Ã º¹±¸¿©ºÎ¸¦ ¹°¾îº¼ ¼ö  ÀÖ´Ù) 
//	//[1]=>ÃÊ±âÈ­½Ã º¹±¸¸ðµå ¼±ÅÃ À¯/¹« È®ÀÎ (0:º¹±¸¸ðµå ¼±ÅÃ¾ÈÇÔ, 1:º¹±¸¸ðµå ¼±ÅÃ)
//	if (st_var.l_handler_recovery_flag[0] == YES)			// ±âÁ¸ »óÅÂ·Î º¹±¸ ÇÒ°ÍÀÎÁö ¸»°ÍÀÎÁö °áÁ¤ÇÑ´Ù. 2K9/08/25/ViboX
//	{
//		st_msg.mstr_confirm_msg = _T("Handler Recovery Mode(YES), Are You sure ?"); //JAMES 2010.0916 _T("ÀÌÀü »óÅÂ·Î Àåºñ¸¦ º¹±¸ ÇÏ½Ã°Ú½À´Ï±î?");
//		
//		n_response = select_dlg.DoModal();
//		
//		if (n_response == IDOK)
//		{		 
//			nFlag = NO;
//			for (i = 0; i < HANDLER_BACKUP_DATA_RW; i++)
// 			{
// 				n_response = Func.Handler_Recovery_Data(0, i, DATA_READ);
// 				
// 				if (n_response == RET_ERROR)
// 				{
//					if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
//					{
//						sprintf( st_msg.c_abnormal_msg , "[ERROR_READ] Handler Recovery Mode SKIP[%02d]. Handler ALL Initialize. ", i);
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
//					nFlag = YES;
// 				}
// 			}
//
//			if(nFlag == YES)
//			{
//				st_var.l_handler_recovery_flag[1] = NO;		
//				if (st_handler.cwnd_list != NULL) //james 2010,0708 ·Î°íÆÄÀÏ Ãß°¡ 
//				{
//					sprintf( st_msg.c_abnormal_msg ,"[skip] recovery data save-init[read]");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
//			}
//			else
//			{
//				if (st_handler.cwnd_list != NULL) //james 2010,0708 ·Î°íÆÄÀÏ Ãß°¡ 
//				{
//					sprintf( st_msg.c_abnormal_msg ,"[ok]recovery data save-init[read]");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//				}
//			}
//			//st_var.n_lotstart_count = 0; //º¹±Í µ¿ÀÛÀ¸·Î ÁøÇà½Ã lot ÁøÇà ¼ö·® Á¤º¸¸¦ Å¬¸®¾î ÇÑ´Ù 
//		}
//		else if (n_response == IDCANCEL)
//		{
//			st_var.l_handler_recovery_flag[1] = NO;
//			if (st_handler.cwnd_list != NULL) //james 2010,0708 ·Î°íÆÄÀÏ Ãß°¡ 
//			{
//				sprintf( st_msg.c_normal_msg ,"[Init_Menu_SKIP]recovery data save-init[read]");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//			}
//		}
//	}
 	return st_var.l_handler_recovery_flag[1];


}


void CPublic_Function::OnInitBtn(int iVal)
{
	if( st_handler.cwnd_init )
		st_handler.cwnd_init->SendMessage( WM_INIT_BTN, iVal );
}

CString CPublic_Function::GetFileString( LPCSTR lpApp, LPCSTR lpKey, LPCSTR lpDef, DWORD dwSize, LPCSTR lpFilePath )
{
	char chr_data[MAX_PATH];
	:: GetPrivateProfileString(lpApp, lpKey, lpDef, chr_data, dwSize, st_path.mstr_basic);
	CString mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // ¹®ÀÚ¿­ ´ë¹®ÀÚ·Î º¯°æ
	mstr_temp.TrimLeft(' ');   // ÁÂÃø ¹®ÀÚ¿­ °ø¹é Á¦°Å              
	mstr_temp.TrimRight(' ');  // ¿ìÃø ¹®ÀÚ¿­ °ø¹é Á¦°Å

	return mstr_temp;
}

int CPublic_Function::GetCurrentRbtPos()
{
// 	double dCurrPos[2];
	int nFuncRet = CRBT_NONE;
//
//	dCurrPos[0] = COMI.Get_MotCurrentPos(MOTOR_ROBOT_X);
//	dCurrPos[1] = COMI.Get_MotCurrentPos(MOTOR_ROBOT_Y);
//
//	if(dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_TRAY1_END] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 5.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_TRAY1_START] + st_handler.md_picker_position[3] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 5.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_TRAY1_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_TRAY1_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_TRAY1;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_TRAY2_END] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 5.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_TRAY2_START] + st_handler.md_picker_position[3] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 5.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_TRAY2_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_TRAY2_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_TRAY2;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_TRAY3_END] - COMI.md_allow_value[MOTOR_ROBOT_Y] &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_TRAY3_START] + st_handler.md_picker_position[3] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 5.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_TRAY3_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_TRAY3_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_TRAY3;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_REJ_END] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 5.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_REJ_START] + st_handler.md_picker_position[3] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 5.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_REJ_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_REJ_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_REJECT;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE1_START] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE1_START] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_START] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT1;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT1;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_1_2] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_1_2] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_2] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_2] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT2;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_2] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_2] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT2;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_1_3] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_1_3] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_3] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_3] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT3;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_3] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_3] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT3;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_1_4] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_1_4] + st_handler.md_picker_position[1] +  COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_4] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_4] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT4;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_4] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_4] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT4;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE1_END] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE1_END] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_END] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT5;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_END] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT5;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE2_START] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE2_START] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_START] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT1;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_START] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_START] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT1;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_2_2] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_2_2] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_2] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_2] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT2;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_2] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_2] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT2;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_2_3] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 10.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_2_3] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 10.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_3] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_3] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT3;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_3] - COMI.md_allow_value[MOTOR_ROBOT_X] - 10.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_3] + COMI.md_allow_value[MOTOR_ROBOT_X] + 10.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT3;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_2_4] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 5.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE_2_4] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 5.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_4] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_2_4] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT4;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_4] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE_1_4] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT4;
//		}
//	}
//	if(nFuncRet == -1 && dCurrPos[1] > st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE2_END] - COMI.md_allow_value[MOTOR_ROBOT_Y] - 5.0f &&
//		dCurrPos[1] < st_motor[MOTOR_ROBOT_Y].d_pos[P_XY_SITE2_END] + st_handler.md_picker_position[1] + COMI.md_allow_value[MOTOR_ROBOT_Y] + 5.0f)
//	{
//		if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_END] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE2_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE2_INSERT5;
//		}
//		else if(dCurrPos[0] > st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_END] - COMI.md_allow_value[MOTOR_ROBOT_X] - 5.0f &&
//			dCurrPos[0] < st_motor[MOTOR_ROBOT_X].d_pos[P_XY_SITE1_END] + COMI.md_allow_value[MOTOR_ROBOT_X] + 5.0f)
//		{
//			nFuncRet = CRBT_SITE1_INSERT5;
//		}
//	}
	return nFuncRet;
}

void CPublic_Function::BackupData()
{
// 	int i, iIdx;
//	char strSData[TSITE_SOCKET_CNT][10][50] = {0,};
//	char strPicker[PICKER_CNT][10][50] = {0,};
//	//#define MAX_TRAY		12
//	//#define MAX_MODULE		20
//	char strTData[MAX_TPOS][20][10][50] = {0,};
//	for ( i = 0; i < TSITE_SOCKET_CNT; i++ )
//	{
//		if( g_site.GetModule(i).IsExist() || g_site.GetEnableSocket(i) == false )
//		{
//			sprintf( strSData[i][0], "%s", g_site.GetModule(i).GetSerial() );
//			sprintf( strSData[i][1], "%s", g_site.GetModule(i).GetLotNo() );
//			sprintf( strSData[i][2], "%s", g_site.GetModule(i).GetSN() );
//			sprintf( strSData[i][3], "%s", g_site.GetModule(i).GetPPID() );
//			sprintf( strSData[i][4], "%s", g_site.GetModule(i).GetWWN() );
//			sprintf( strSData[i][5], "%s", g_site.GetModule(i).GetBin() );
//			sprintf( strSData[i][6], "%s", g_site.GetModule(i).GetScrapCode() );
//			sprintf( strSData[i][7], "%d", g_site.GetModule(i).GetTestCnt() );
//			int nTestCnt = g_site.GetModule(i).GetTestCnt();
//			if(nTestCnt > 0 ) sprintf( strSData[i][8], "%d", g_site.GetModule(i).GetTestedPos(nTestCnt - 1 ) + 1 );
//			sprintf( strSData[i][9], "%d", g_site.GetEnableSocket(i) );
//		}			
//	}
//
//	for ( i = 0; i < PICKER_CNT; i++ )
//	{
//		if( g_robot.GetPicker(i).GetModule().IsExist() )
//		{
//			sprintf( strPicker[i][0], "%s", g_robot.GetPicker(i).GetModule().GetSerial() );
//			sprintf( strPicker[i][1], "%s", g_robot.GetPicker(i).GetModule().GetLotNo() );
//			sprintf( strPicker[i][2], "%s", g_robot.GetPicker(i).GetModule().GetSN() );
//			sprintf( strPicker[i][3], "%s", g_robot.GetPicker(i).GetModule().GetPPID() );
//			sprintf( strPicker[i][4], "%s", g_robot.GetPicker(i).GetModule().GetWWN() );
//			sprintf( strPicker[i][5], "%s", g_robot.GetPicker(i).GetModule().GetBin() );
//			sprintf( strPicker[i][6], "%s", g_robot.GetPicker(i).GetModule().GetScrapCode() );
//			sprintf( strPicker[i][7], "%d", g_robot.GetPicker(i).GetModule().GetTestCnt() );
//			int nTestCnt = g_robot.GetPicker(i).GetModule().GetTestCnt();
//			if(nTestCnt > 0) sprintf( strPicker[i][8], "%d", g_robot.GetPicker(i).GetModule().GetTestedPos(nTestCnt - 1 ) + 1 );
//		}
//	}
//
//	for ( iIdx = 0; iIdx < MAX_TPOS; iIdx++ )
//	{
//		int iTray = g_Tray.GetIdxByPos( (ETRAY_POS)(TPOS_FRONT + iIdx) );
//		int iTargetTray = g_Tray.GetIdxByTargetPos( (ETRAY_POS)(TPOS_FRONT + iIdx) );
//		
//		ATray tray;
//		if( iTray >= 0 )
//		{
//			tray = g_Tray.GetTray( iTray );
//		}
//
//		int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//		for( i=0; i<iMdlCnt; i++ )
//		{
//			if( tray.GetModule(i).IsExist() )
//			{
//				sprintf( strTData[iTray][i][0], "%s", tray.GetModule(i).GetSerial() );
//				sprintf( strTData[iTray][i][1], "%s", tray.GetModule(i).GetLotNo() );
//				sprintf( strTData[iTray][i][2], "%s", tray.GetModule(i).GetSN() );
//				sprintf( strTData[iTray][i][3], "%s", tray.GetModule(i).GetPPID() );
//				sprintf( strTData[iTray][i][4], "%s", tray.GetModule(i).GetWWN() );
//				sprintf( strTData[iTray][i][5], "%s", tray.GetModule(i).GetBin() );
//				sprintf( strTData[iTray][i][6], "%s", tray.GetModule(i).GetScrapCode() );
//				sprintf( strTData[iTray][i][7],  "%d", tray.GetModule(i).GetTestCnt() );
//				int nTestCnt = tray.GetModule(i).GetTestCnt();
//				if(nTestCnt > 0) sprintf( strTData[iTray][i][8], "%d", tray.GetModule(i).GetTestedPos(nTestCnt - 1 ) + 1 );
//				sprintf( strTData[iTray][i][9], "%d", iTargetTray);
//			}
//		}
//	}
}


///////20131115
int CPublic_Function::OnBCR_CheckData(CString str_data)
{
	int nFuncRet = CTLBD_RET_GOOD;
	
	char	cp_cmd[1024];
	int     nLength;  // µ¥ÀÌÅÍ ÃÑ ±æÀÌ ÀúÀå º¯¼ö 
	CString strBody;  // BODY µ¥ÀÌÅÍ ÀúÀå º¯¼ö 
	CString strTemp;
	
	int comma_pos = 0, comma_pos2 = 0;
	
	CString strBarcodeData[10];
	int iBarcodeData = 0;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // µ¥ÀÌÅÍ ÀúÀå º¯¼ö ÃÊ±âÈ­ 
	
	for ( int i = 0; i < 10; i++ )
	{
		strBarcodeData[i] = "";
	}
	
	nLength = str_data.GetLength();

		strBody = str_data.Mid(1, nLength - 1);  // [Çì´õ+Å×ÀÏ] Á¦°Å //2014.0630 YSJ
		strBody.TrimLeft(' ');
		strBody.TrimRight(' ');
		strBody.MakeUpper();
		
		st_work.str_barcode_success = strBody;
		st_work.strBarcodeRecive = strBody;

	if ( strBody == "NOREAD" )
	{
		return CTLBD_RET_ERROR;
	}
	
// 	comma_pos = strBody.Find(",");    //±¸ºÐÀÚÀÇ À§Ä¡¸¦ Ã£´Â´Ù..
// 	
// 	if ( comma_pos == -1 )  //±¸ºÐÀÚ°¡ ¾ø´Ù (µ¥ÀÌÅÍ°¡ Àß¸øµÆ´Ù!)
// 	{
// 		return CTLBD_RET_ERROR;
// 	}
// 	
// 	while ( comma_pos )
// 	{
// 		strBarcodeData[iBarcodeData] = strBody.Mid( 0, comma_pos );
// 		iBarcodeData++;
// 		
// 		if ( strBody.GetLength() > comma_pos + 1 )
// 		{
// 			comma_pos=comma_pos + 1;
// 			strBody = strBody.Mid( comma_pos );
// 			comma_pos = strBody.Find(",");    //±¸ºÐÀÚÀÇ À§Ä¡¸¦ Ã£´Â´Ù..
// 		}
// 		else
// 		{
// 			comma_pos = 0;
// 		}
// 	}
// 
// 	st_work.strBarcodeRecive = "";
// 	for ( i = 0; i < iBarcodeData; i++ )
// 	{
// 		st_work.strBarcodeRecive = strBarcodeData[i];
// 		break;
// 
// 	}

	if ( st_work.strBarcodeRecive == "" )	return CTLBD_RET_ERROR;
	
	return nFuncRet;
}
////////


int CPublic_Function::OnLabel_CheckData(CString str_data, int port)
{
	int nFuncRet = CTLBD_RET_GOOD;

	//3.22 Keyence
	int     nLength;  // µ¥ÀÌÅÍ ÃÑ ±æÀÌ ÀúÀå º¯¼ö 
	CString strBody;  // BODY µ¥ÀÌÅÍ ÀúÀå º¯¼ö 
	
	char  cp_cmd[1024];
	
	int     j;
	int     nStartIndex, nEndIndex;  // BCR µ¥ÀÌÅÍÀÇ ½ÃÀÛ ¹× Á¾·á À§Ä¡ ÀúÀå º¯¼ö 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // µ¥ÀÌÅÍ °¹¼ö ÀúÀå º¯¼ö 
	CString strResultData[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // µ¥ÀÌÅÍ ÀúÀå º¯¼ö ÃÊ±âÈ­ 
	
	// **************************************************************************
	// ¼ö½Å µ¥ÀÌÅÍ ÃÖ¼Ò ±æÀÌ °Ë»çÇÑ´Ù [ÃÖ¼Ò 3ÀÚ¸® ÀÌ»ó]                          
	// **************************************************************************
	nLength = str_data.GetLength();
	if(nLength < 3)
	{
		nFuncRet = CTLBD_RET_ERROR;
	}
	// **************************************************************************
	str_data.TrimLeft("\r\n");
	str_data.TrimRight("\r\n");

	strBody = str_data;//.Mid(0, nLength - 1);  // [Å×ÀÏ] Á¦°Å 
	strBody.TrimLeft(0x02);
	strBody.TrimRight(0x03);
	strBody = strBody + ",";
	
	// **************************************************************************
	// ¿¡·¯ ¸Þ½ÃÁö ¼ö½Å À¯¹« °Ë»çÇÑ´Ù                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Label Print ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		nFuncRet = CTLBD_RET_ERROR;
	}
	// **************************************************************************
	
	sprintf(cp_cmd, "%s", strBody);
	nLength = strBody.GetLength();
		
	nReceivedDataCount = 0;
	nStartIndex = 0;
	// ==============================================================================
	// ½ÇÀç·Î ÀÐÀº BCR µ¥ÀÌÅÍ Àü¿ª º¯¼ö¿¡ ¼³Á¤ÇÑ´Ù
	// ==============================================================================
	for(j = 0 ; j < 13 ; j++)
	{
		nEndIndex   = strBody.Find(",", nStartIndex);  // BCR µ¥ÀÌÅÍ Á¾·á À§Ä¡ ¼³Á¤ 
		if (nEndIndex == -1)
		{
			if (nLength - nStartIndex > 0)
			{
				nEndIndex = nLength - 1;
			}
			else
			{
				break;
			}
		}
		strResultData[j] = "";
		strResultData[j] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR µ¥ÀÌÅÍ ¹®ÀÚ¿­ ÆÄ½Ì 
		nReceivedDataCount++;
		nStartIndex = nEndIndex + 1;
		
		if (st_handler.cwnd_barcode != NULL)
		{
 			if (st_barcode.n_barcode_hs_cnt == 0)
 			{
				if (j == 1 ) 
				{
					st_barcode.n_barcode_hs_paper_out_error[port] = atoi(strResultData[j]);
				}
				else if (j == 2 ) 
				{
					st_barcode.n_barcode_hs_pause_error[port] = atoi(strResultData[j]);
				}					
				if (j == 11) 
				{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
				}
 			}
 			else if (st_barcode.n_barcode_hs_cnt == 1)
 			{
				if (j == 2 ) 
				{
					st_barcode.n_barcode_hs_head_up_error[port] = atoi(strResultData[j+12]);
				}
				else if (j == 3 ) 
				{
					st_barcode.n_barcode_hs_ribbon_out_error[port] = atoi(strResultData[j+12]);
				}					
				else if (j == 5 ) 
				{
					st_barcode.n_barcode_hs_print_mode[port] = atoi(strResultData[j+12]);
				}					
				
				if (j == 10) 
				{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
				}
 			}
 			else if (st_barcode.n_barcode_hs_cnt == 2)
 			{
				if (j == 1) 
				{
					st_barcode.n_barcode_hs_cnt = 0;
					break;
				}
 			}
		}
		else if (st_handler.cwnd_main != NULL)
		{
			if (st_barcode.n_barcode_hs_cnt == 0)
			{
				if (j == 1 ) 
				{
					st_barcode.n_barcode_hs_paper_out_error[port] = atoi(strResultData[j]);
				}
				else if (j == 2 ) 
				{
					st_barcode.n_barcode_hs_pause_error[port] = atoi(strResultData[j]);
				}					
				if (j == 11) 
				{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
				}
			}
			else if (st_barcode.n_barcode_hs_cnt == 1)
			{
				if (j == 2 ) 
				{
					st_barcode.n_barcode_hs_head_up_error[port] = atoi(strResultData[j+12]);
				}
				else if (j == 3 ) 
				{
					st_barcode.n_barcode_hs_ribbon_out_error[port] = atoi(strResultData[j+12]);
				}					
				else if (j == 5 ) 
				{
					st_barcode.n_barcode_hs_print_mode[port] = atoi(strResultData[j+12]);
				}					
				
				if (j == 10) 
				{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
				}
			}
			else if (st_barcode.n_barcode_hs_cnt == 2)
			{
				if (j == 1) 
				{
					st_barcode.n_barcode_hs_cnt = 0;
					break;
				}
 			}
		}
	}
	return nFuncRet;
}




///////20131125
int CPublic_Function::Barcode_Reading_Start(int n_barcode_col, bool UpDown)
{
	int nBcrPort = BCR_PORT;

	int i = 0, n_response = 0;
	int FuncRet =  BD_PROCEED;
	int nRet = 0;
	CString strBcrCommand;
	
	CDialog_Select	select_dlg;
	
	switch (barcode_reading_step)
	{
	case 0:
		barcode_reading_step = 100;
		break;

	case 100:
		st_work.strBarcodeRecive = "";//¼ö½Å ¸Þ¼¼Áö ÃÊ±âÈ­
		st_work.n_barcode_recive = CTL_READY;//¼ö½Å ÃÊ±â»óÅÂ 

		strBcrCommand.Format("%cR%c", 0x02, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
		st_serial.mstr_snd[nBcrPort-1] = strBcrCommand;
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, nBcrPort, 0);
		
		barcode_reading_step = 200;
		break;
		
	case 200:
			if (st_work.n_barcode_recive == CTL_YES)
			{
				st_modulemap.ReciveBcrData[0][n_barcode_col] = st_work.strBarcodeRecive;
				barcode_reading_step = 300;
			}
			// ºñÁ¤»óÀûÀ¸·Î ¼ö½Å ¹ÞÀ½
			else if (st_work.n_barcode_recive == CTL_NO)
			{
				nBarcodeRetryCnt++;		
				if (nBarcodeRetryCnt > 2)
				{
					nBarcodeRetryCnt = 0;
					st_modulemap.ReciveBcrData[0][n_barcode_col] = st_work.strBarcodeRecive;
					barcode_reading_step = 250;
					FuncRet = BD_ERROR;
				}
				else							// Àç½Ãµµ.
				{
					barcode_reading_step = 100;
				}
			}


		break;

	case 210:
		if(GetCurrentTime() - m_dwBcrTime < 0) m_dwBcrTime = GetCurrentTime();
		if(GetCurrentTime() -  m_dwBcrTime > 500)
		{
			if (st_work.n_barcode_recive == CTL_YES)
			{
				st_modulemap.ReciveBcrData[0][n_barcode_col] = st_work.strBarcodeRecive;
				barcode_reading_step = 300;
			}
			else if (st_work.n_barcode_recive == CTL_NO)
			{
				nBarcodeRetryCnt++;		
				if (nBarcodeRetryCnt > 2)
				{
					nBarcodeRetryCnt = 0;
					st_modulemap.ReciveBcrData[0][n_barcode_col] = st_work.strBarcodeRecive;
					barcode_reading_step = 250;
					FuncRet = BD_ERROR;
				}

			}
		}
		break;

	case 250://2013,1223
		barcode_reading_step = 0;
		if(st_modulemap.ReciveBcrData[0][n_barcode_col].GetLength() > 10 && st_modulemap.ReciveBcrData[0][n_barcode_col] < 20)
		{
			barcode_reading_step = 300;
		}
		break;

	case 300:
		FuncRet = BD_GOOD;
		barcode_reading_step = 0;
		break;	

	}
	
	return FuncRet;
}
///////

std::vector<int> CPublic_Function::LoadConv()
{
//	int nRet;
	std::vector<int> vecRtn;
//	std::multimap<int, int> mapCntSite;
	vecRtn.clear();
//	mapCntSite.clear();
//
//
//	if( /*0 &&*/ st_handler.mn_smema_load == CTL_READY)// g_ioMgr.get_in_bit(st_io.i_front_shuttle_transfer_signal_chk, 0, 100) == IO_ON)
//	{
//		if( /*0 &&*/  g_ioMgr.get_in_bit(st_io.i_convin_chk_1) == IO_ON)
//		{
//			nRet = 0x01;
//			mapCntSite.insert( std::pair<int,int>( nRet, 0 ) );
//		}
//		if( /*0 &&*/  g_ioMgr.get_in_bit(st_io.i_convin_chk_2) == IO_ON)
//		{
//			nRet = 0x02;
//			mapCntSite.insert( std::pair<int,int>( nRet, 1 ) );
//		}
//		if(  /*0 &&*/  g_ioMgr.get_in_bit(st_io.i_convin_chk_3) == IO_ON)
//		{
//			nRet = 0x04;
//			mapCntSite.insert( std::pair<int,int>( nRet, 2 ) );
//		}
//		if(  /*0 &&*/ g_ioMgr.get_in_bit(st_io.i_convin_chk_4) == IO_ON)
//		{
//			nRet = 0x08;
//			mapCntSite.insert( std::pair<int,int>( nRet, 3 ) );
//		}
//
//	}
//	
//	std::multimap<int,int>::iterator it = mapCntSite.begin();
//	for( it; it != mapCntSite.end(); it++ )
//	{
//		vecRtn.push_back( MPOS_INDEX_CONV + it->second );
//	}
	return vecRtn;
}

std::vector<int> CPublic_Function::UnloadConv( int nLotIdx )
{
	std::vector<int> vecRtn;
//	std::multimap<int, int> mapCntSite;
//	vecRtn.clear();
//	mapCntSite.clear();
//	int iIndexBin = MPOS_INDEX_UNLOAD;
//
//	int nTrayIdx = -1,nRet;
//	nTrayIdx = g_Tray.GetIdxByPos( TPOS_UNLOAD );
//	if(nTrayIdx < 0) return vecRtn;
//
//	//2013,1223
//	if( g_lotMgr.GetLotIDAt(nLotIdx) == g_Tray.GetTray(nTrayIdx).GetStrLotID() )
//	{//Let me know first the DVC is Alot or B lot,
//
//	}
//
//	if( g_ioMgr.get_in_bit( st_io.i_rear_shuttle_aline_request_signal_chk, IO_ON ) == IO_OFF &&
//		g_ioMgr.get_in_bit( st_io.i_rear_shuttle_bline_request_signal_chk, IO_ON ) == IO_OFF )
//	{
//		nTrayIdx = g_Tray.GetIdxByPos( TPOS_UNLOAD );
//		ATray& tray = g_Tray.GetTray( nTrayIdx );
//
//		std::vector<int> vecFailRtn;
//		std::multimap<int, int> mapFailSite;
//		mapFailSite.clear();
//		vecFailRtn.clear();
//
//		if( tray.IsExistModule(0))
//		{
//			nRet = 0x01;
//			mapCntSite.insert( std::pair<int,int>( nRet, 0 ) );
//			if( tray.GetModule(0).GetModuleState() != EMS_GOOD )
//			{
//				mapFailSite.insert( std::pair<int,int>( nRet, 0 ) );
//
//			}
//		}
//		if( tray.IsExistModule(1))
//		{
//			nRet = 0x02;
//			mapCntSite.insert( std::pair<int,int>( nRet, 1 ) );
//			if( tray.GetModule(1).GetModuleState() != EMS_GOOD )
//			{
//				mapFailSite.insert( std::pair<int,int>( nRet, 1 ) );
//				
//			}
//		}
//		if( tray.IsExistModule(2))
//		{
//			nRet = 0x04;
//			mapCntSite.insert( std::pair<int,int>( nRet, 2 ) );
//			if( tray.GetModule(2).GetModuleState() != EMS_GOOD )
//			{
//				mapFailSite.insert( std::pair<int,int>( nRet, 2 ) );
//				
//			}
//		}
//		if( tray.IsExistModule(3))
//		{
//			nRet = 0x08;
//			mapCntSite.insert( std::pair<int,int>( nRet, 3 ) );
//			if( tray.GetModule(3).GetModuleState() != EMS_GOOD )
//			{
//				mapFailSite.insert( std::pair<int,int>( nRet, 3 ) );
//				
//			}
//		}
//
//		std::multimap<int,int>::iterator it = mapFailSite.begin();
//		for( it; it != mapFailSite.end(); it++ )
//		{
//			vecFailRtn.push_back( iIndexBin + it->second );
//		}
//		it = mapCntSite.begin();
//		for( it; it != mapCntSite.end(); it++ )
//		{
//			vecRtn.push_back( iIndexBin + it->second );
//		}
//
//		if( vecRtn.size() > 0 && vecRtn.size() == mapFailSite.size() )
//		{
//			return vecRtn;
//		}
//
//	}
//	else if( g_ioMgr.get_in_bit( st_io.i_rear_shuttle_aline_request_signal_chk, IO_ON ) == IO_ON ||
//		 g_ioMgr.get_in_bit( st_io.i_rear_shuttle_bline_request_signal_chk, IO_ON ) == IO_ON )
//	{
//		vecRtn.clear();
//		mapCntSite.clear();
//
//		nTrayIdx = g_Tray.GetIdxByPos( TPOS_UNLOAD );
//		ATray& tray = g_Tray.GetTray( nTrayIdx );
////		for (int i = 0; i < MAX_BUFFDVC; i++ )
////		{
////			if( tray.IsExistModule(i)) mapCntSite.insert( std::pair<int,int>( nRet, i ) );
////		}
//
//// 		if( g_Tray.GetTray(nTrayIdx).GetModule(i).IsExist() == true)// tray.GetStrLotID() == g_lotMgr.GetLotIDAt( nLotIdx) )
//// 		{
//			if( tray.IsExistModule(0) )
//			{
//				nRet = 0x01;
//				mapCntSite.insert( std::pair<int,int>( nRet, 0 ) );
//			}
//			if( tray.IsExistModule(1) )
//			{
//				nRet = 0x02;
//				mapCntSite.insert( std::pair<int,int>( nRet, 1 ) );
//			}
//			if( tray.IsExistModule(2) )
//			{
//				nRet = 0x04;
//				mapCntSite.insert( std::pair<int,int>( nRet, 2 ) );
//			}
//			if( tray.IsExistModule(3) )
//			{
//				nRet = 0x08;
//				mapCntSite.insert( std::pair<int,int>( nRet, 3 ) );
//			}
//
//			std::multimap<int,int>::iterator it = mapCntSite.begin();
//			for( it; it != mapCntSite.end(); it++ )
//			{
//				vecRtn.push_back( iIndexBin + it->second );
//			}
//// 		}
//
//	}
//	
	return vecRtn;
}

std::vector<int> CPublic_Function::UnloadRetest()
{
	std::vector<int> vecRetestRtn;
//	std::multimap<int, int> mapRetestCnt;
//	vecRetestRtn.clear();
//	mapRetestCnt.clear();
//
//	int nTrayIdx = -1,nRet;
//	nTrayIdx = g_Tray.GetIdxByPos( TPOS_UNLOAD );
//	ATray& tray = g_Tray.GetTray( nTrayIdx );
//
//	int nBin = -1;
//	nBin = atoi(tray.GetModule(0).GetBin());
//	if( nBin > 1)
//	{
//		nRet = 0x01;
//		if( tray.GetModule(0).GetTestCnt() <= st_handler.mn_retest_cnt )
//		{
//			mapRetestCnt.insert( std::pair<int,int>( nRet, 0 ) );
//		}
//	}
//	nBin = atoi(tray.GetModule(1).GetBin());
//	if( nBin > 1)
//	{
//		nRet = 0x02;
//		if( tray.GetModule(1).GetTestCnt() <= st_handler.mn_retest_cnt )
//		{
//			mapRetestCnt.insert( std::pair<int,int>( nRet, 1 ) );
//		}
//	}
//	nBin = atoi(tray.GetModule(2).GetBin());
//	if( nBin > 1)
//	{
//		nRet = 0x04;
//		if( tray.GetModule(2).GetTestCnt() <= st_handler.mn_retest_cnt )
//		{
//			mapRetestCnt.insert( std::pair<int,int>( nRet, 2 ) );
//		}
//	}
//	nBin = atoi(tray.GetModule(3).GetBin());
//	if( nBin > 1)
//	{
//		nRet = 0x08;
//		if( tray.GetModule(3).GetTestCnt() <= st_handler.mn_retest_cnt )
//		{
//			mapRetestCnt.insert( std::pair<int,int>( nRet, 3 ) );
//		}
//	}
//
//	std::multimap<int,int>::iterator it = mapRetestCnt.begin();
//	for( it; it != mapRetestCnt.end(); it++ )
//	{
//		vecRetestRtn.push_back( it->second );
//	}
	return vecRetestRtn;
}

std::vector<int> CPublic_Function::UnloadFail()
{
	std::vector<int> vecFailRtn;
//	std::multimap<int, int> mapFailCnt;
//	vecFailRtn.clear();
//	mapFailCnt.clear();
//
//	int nTrayIdx = -1, nRet;
//	nTrayIdx = g_Tray.GetIdxByPos( TPOS_UNLOAD );
//	ATray& tray = g_Tray.GetTray( nTrayIdx );
//	if( atoi(tray.GetModule(0).GetBin()) > 1)
//	{
//		nRet = 0x01;
//		if( tray.GetModule(0).GetTestCnt() > st_handler.mn_retest_cnt )
//		{
//			mapFailCnt.insert( std::pair<int,int>( nRet, 0 ) );
//		}
//	}
//	if( atoi(tray.GetModule(1).GetBin()) > 1)
//	{
//		nRet = 0x02;
//		if( tray.GetModule(1).GetTestCnt() > st_handler.mn_retest_cnt )
//		{
//			mapFailCnt.insert( std::pair<int,int>( nRet, 1 ) );
//		}
//	}
//	if( atoi(tray.GetModule(2).GetBin()) > 1)
//	{
//		nRet = 0x04;
//		if( tray.GetModule(2).GetTestCnt() > st_handler.mn_retest_cnt )
//		{
//			mapFailCnt.insert( std::pair<int,int>( nRet, 2 ) );
//		}
//	}
//	if( atoi(tray.GetModule(3).GetBin()) > 1)
//	{
//		nRet = 0x08;
//		if( tray.GetModule(3).GetTestCnt() > st_handler.mn_retest_cnt )
//		{
//			mapFailCnt.insert( std::pair<int,int>( nRet, 3 ) );
//		}
//	}
//
//	std::multimap<int,int>::iterator it = mapFailCnt.begin();
//	for( it; it != mapFailCnt.end(); it++ )
//	{
//		vecFailRtn.push_back( it->second );
//	}

	return vecFailRtn;
}

void CPublic_Function::OnLoadBufferIn(std::vector<int> vecTest)
{	
//	ATray tray;
//	CString strLotID = "";
//	for( int i=0; i<MAX_BUFFDVC/* vecTest.size()*/; i++ )
//	{
//		CString strArraySN = "";
//		CString strSerial = "";
//		CString strPPID = "";
//		CString strWWN = "";
//		CString strCSerial = "";
//		CString strPSID = "";
//		CString strBIN = "";
//		CString strVar = "0";
//		
//		// ¸ðµâ ¼Â
//		AModule mdl;
//		mdl.SetModuleInfo( strLotID, strArraySN, strSerial, strPPID, strWWN, strCSerial, strPSID, strVar, strBIN );
//		
//		if( vecTest[i] > -1 )//  mdl.IsExist() && strBIN == "" )
//		{
//			mdl.SetModuleState( EMS_LOAD );
//		}
//		else
//		{
//			mdl.SetModuleState( EMS_NONE );
//		}
//		//		tray.SetModule(vecTest[i], mdl);
//		tray.SetModule( i, mdl );
//	}
//	
//	strLotID = "ABCDEF";
//	tray.SetTrayInfo( strLotID, 1, false, false, false );
//	g_Tray.AddTray(tray);	
//	
}
//203000 0 A "Loading Buffer Device Check #1 Off Error."
//203001 0 A "Loading Buffer Device Check #1 On Error."
//203010 0 A "Loading Buffer Device Check #2 Off Error."
//203011 0 A "Loading Buffer Device Check #2 On Error."
//203020 0 A "Loading Buffer Device Check #3 Off Error."
//203021 0 A "Loading Buffer Device Check #3 On Error."
//203030 0 A "Loading Buffer Device Check #4 Off Error."
//203031 0 A "Loading Buffer Device Check #4 On Error."

//203040 0 A "UnLoading Buffer Device Check #1 Off Error."
//203041 0 A "UnLoading Buffer Device Check #1 On Error."
//203050 0 A "UnLoading Buffer Device Check #2 Off Error."
//203051 0 A "UnLoading Buffer Device Check #2 On Error."
//203060 0 A "UnLoading Buffer Device Check #3 Off Error."
//203061 0 A "UnLoading Buffer Device Check #3 On Error."
//203070 0 A "UnLoading Buffer Device Check #4 Off Error."
//203071 0 A "UnLoading Buffer Device Check #4 On Error."

///////20131203
int CPublic_Function::Reject_Tray_UnclampCheck()
{
	int Ret = 0;
	
//	Ret = CTLBD_RET_GOOD;
//	if( g_ioMgr.get_in_bit( st_io.i_reject1_unclamp_chk ) == IO_ON ||
//		g_ioMgr.get_in_bit( st_io.i_reject2_unclamp_chk ) == IO_ON)
//	{
//		Ret = CTLBD_RET_ERROR;
//	}
	
	return Ret;
}
///////

void CPublic_Function::UnloadDataClear()
{
	if (st_handler.cwnd_list != NULL)	// ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		CString strLog;
		strLog.Format("Product per hour : %d, Total: %d\r\n", st_handler.mn_buffer_in, g_lotMgr.GetInputCnt(CUM));
		Func.On_LogFile_Add(LOG_UPH_TOT, strLog);
		st_handler.mn_buffer_in = 0;
	}
}

bool CPublic_Function::CmpStrFile( CString strCmp1, CString strCmp2 )
{
	strCmp1.TrimLeft();
	strCmp1.TrimRight();
	strCmp1.MakeUpper();
	strCmp2.TrimLeft();
	strCmp2.TrimRight();
	strCmp2.MakeUpper();
	if( strCmp1 == strCmp2) return true;

	return false;

}

int CPublic_Function::BcrPickerOff_Chk()
{
	int nRet = CTL_GOOD;
//	if(g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_bcr_picker1_dn_chk) == IO_OFF &&
//		g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_bcr_picker1_dn_chk) == IO_OFF &&
//		g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_bcr_picker1_dn_chk) == IO_OFF &&
//		g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_bcr_picker1_dn_chk) == IO_OFF)
//	{
		nRet = CTL_GOOD;
//	}
//	else
//	{
//		if	   (g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) != IO_ON || g_ioMgr.get_in_bit( st_io.i_bcr_picker1_dn_chk) != IO_OFF)
//		{
//			if  (g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) != IO_ON)	alarm.mstr_code = "223060";
//			else															alarm.mstr_code = "223071";
//		}
//		else if(g_ioMgr.get_in_bit( st_io.i_bcr_picker2_up_chk) != IO_ON || g_ioMgr.get_in_bit( st_io.i_bcr_picker2_dn_chk) != IO_OFF)
//		{
//			if  (g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) != IO_ON)	alarm.mstr_code = "223080";
//			else															alarm.mstr_code = "223091";
//		}
//		else if(g_ioMgr.get_in_bit( st_io.i_bcr_picker3_up_chk) != IO_ON || g_ioMgr.get_in_bit( st_io.i_bcr_picker3_dn_chk) != IO_OFF)
//		{
//			if  (g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) != IO_ON)	alarm.mstr_code = "223100";
//			else															alarm.mstr_code = "223111";
//		}
//		else if(g_ioMgr.get_in_bit( st_io.i_bcr_picker4_up_chk) != IO_ON || g_ioMgr.get_in_bit( st_io.i_bcr_picker4_dn_chk) != IO_OFF)
//		{
//			if  (g_ioMgr.get_in_bit( st_io.i_bcr_picker1_up_chk) != IO_ON)	alarm.mstr_code = "223120";
//			else															alarm.mstr_code = "223131";
//		}
//		nRet = CTL_ERROR;
//		CTL_Lib.Alarm_Error_Occurrence( 701, CTL_dWARNING, alarm.mstr_code );
//	}
	
	return nRet;
}

////2014.0826
void CPublic_Function::Rewind_Mode(int port)
{
	CString strLabelPrint;

	CString str_print_data;
	
	str_print_data = "^XA^MMR^XZ";	//Rewind
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port + 1, 0);
	::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
	

// 	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
// 	{
// 		return CTLBD_RET_ERROR;
// 	}
// 	
// 	return CTLBD_RET_GOOD;
}

void CPublic_Function::Print_Complete_Set(int port)
{
	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "^XA^SXK,A,Y,Y^XZ";//20120906
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port + 1, 0);
	::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}

//	clsZebra.OnPrintCalibration(0,)

	
	
	
	// 	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	// 	{
	// 		return CTLBD_RET_ERROR;
	// 	}
	// 	
	// 	return CTLBD_RET_GOOD;
}

void CPublic_Function::Print_Complete_Reset(int port)
{
	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "^XA^SXK,A,N,N^XZ";//20120906
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port + 1, 0);
	::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
	
	
	// 	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	// 	{
	// 		return CTLBD_RET_ERROR;
	// 	}
	// 	
	// 	return CTLBD_RET_GOOD;
}

void CPublic_Function::Printer_Status(int port)
{
	CString strLabelPrint;
	
	CString str_print_data;
	
	str_print_data = "~HS";//20120906
	strLabelPrint.Format("%c%s%c", 0x02, str_print_data, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
	
	st_serial.mstr_snd[port] = strLabelPrint;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, port + 1, 0);
	::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strLabelPrint);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
	
	
	// 	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	// 	{
	// 		return CTLBD_RET_ERROR;
	// 	}
	// 	
	// 	return CTLBD_RET_GOOD;
}

int CPublic_Function::OutLptPort(CString str_text)
{
	HANDLE hLpt;
	DWORD dwWritten; 
	
    hLpt = CreateFile("LPT1:", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
    if (hLpt == INVALID_HANDLE_VALUE)
	{ 
		if(st_handler.cwnd_list != NULL)
		{		
			sprintf(st_msg.c_abnormal_msg,"Can not open LPT1");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		//       AfxMessageBox("Can not open LPT1 !!!");
		int a = GetLastError();
		CloseHandle(hLpt); //20120918
		return CTLBD_RET_ERROR;
	}
    else
	{ 
        WriteFile(hLpt, str_text, str_text.GetLength(), &dwWritten, NULL);
        CloseHandle(hLpt); 
    }
	
	return CTLBD_RET_GOOD;
}

int CPublic_Function::OnLabelPrint_Parsing(CString strCheckData, int i)
{

	//3.22 Keyence
	int     nLength;  // µ¥ÀÌÅÍ ÃÑ ±æÀÌ ÀúÀå º¯¼ö 

	CString strBody;  // BODY µ¥ÀÌÅÍ ÀúÀå º¯¼ö 
	
	char  cp_cmd[1024];
	
	int     j;
	int     nStartIndex, nEndIndex;  // BCR µ¥ÀÌÅÍÀÇ ½ÃÀÛ ¹× Á¾·á À§Ä¡ ÀúÀå º¯¼ö 
	int     nStartIndex1, nEndIndex1;  // BCR µ¥ÀÌÅÍÀÇ ½ÃÀÛ ¹× Á¾·á À§Ä¡ ÀúÀå º¯¼ö 

	int     nReceivedDataCount=0;
	CString strTempDataCount;  // µ¥ÀÌÅÍ °¹¼ö ÀúÀå º¯¼ö 
	CString strResultData[100];
	CString strResultData1[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	CString str_LabelStartCode;

	memset(&cp_cmd, 0, sizeof(cp_cmd));  // µ¥ÀÌÅÍ ÀúÀå º¯¼ö ÃÊ±âÈ­ 
	
	// **************************************************************************
	// ¼ö½Å µ¥ÀÌÅÍ ÃÖ¼Ò ±æÀÌ °Ë»çÇÑ´Ù [ÃÖ¼Ò 3ÀÚ¸® ÀÌ»ó]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// **************************************************************************
	// ¿¡·¯ ¸Þ½ÃÁö ¼ö½Å À¯¹« °Ë»çÇÑ´Ù                                            
	// **************************************************************************
	if(strCheckData.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Label Print ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return FALSE;
	}
	// **************************************************************************
	
	//20140213
	str_LabelStartCode = strCheckData.Mid(0,3);
	if(str_LabelStartCode != "^XA")
	{
		return FALSE;
	}
	//

	sprintf(cp_cmd, "%s", strCheckData);
	nLength = strCheckData.GetLength();
		
	nReceivedDataCount = 0;
	nStartIndex = 0;
	nStartIndex1 = 0;

	// ==============================================================================
	// ½ÇÀç·Î ÀÐÀº BCR µ¥ÀÌÅÍ Àü¿ª º¯¼ö¿¡ ¼³Á¤ÇÑ´Ù
	// ==============================================================================
		for(j = 0 ; j < nLength ; j++)
		{

			nEndIndex   = strCheckData.Find("^", nStartIndex);  // BCR µ¥ÀÌÅÍ Á¾·á À§Ä¡ ¼³Á¤ 
			
			if (nEndIndex == -1)
			{
				if (nLength - nStartIndex > 0)
				{
					nEndIndex = nLength - 1;
				}
				else
				{
					break;
				}
			}
			strResultData[j] = "";
			strResultData[j] = strCheckData.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR µ¥ÀÌÅÍ ¹®ÀÚ¿­ ÆÄ½Ì 
			nReceivedDataCount++;
			nStartIndex = nEndIndex + 1;
			
//////////////////// 20120906
			if (j == 3 || j == 4) //Unkown1 À§Ä¡ ¹× Unkown1 ³»¿ë 
			{
				if (j == 3) st_NW.mstr_Unkown1[i] = "^" + strResultData[j];
				else if (j == 4) st_NW.mstr_Unkown1[i] += "^" + strResultData[j];	//^PON^FS
			}			
			else if (j == 5 || j == 6 || j == 7 || j == 8) //1¹ø À§Ä¡ ¹× 1¹ø ³»¿ë
			{
				if (j == 5) //^FO387.5,12.5
				{
					st_NW.mstr_1Position[i] = "^" + strResultData[j]; 
									
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_1PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  
					st_NW.mstr_1PosX_1[i] = st_NW.mstr_1PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_1PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;
				}
				else if (j == 6) //A0,57.5,20
				{
					st_NW.mstr_1Position[i] += "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_1Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
				
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_1High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;

				}
				else if (j == 7) //^FD32GB 4Rx4 PC3L-12800L-11-12-C0
				{
					st_NW.mstr_1Contents[i] = "^" + strResultData[j];
					st_NW.mstr_1Contents_1[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 8) st_NW.mstr_1Contents[i] += "^" + strResultData[j];
			}
			else if (j == 9 || j == 10 || j == 11 || j == 12) //2¹ø À§Ä¡ ¹× 2¹ø ³»¿ë
			{
				if (j == 9) //^FO50,87.5
				{
					st_NW.mstr_2Position[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_2PosX_1[i] = st_NW.mstr_2PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;
				}
				else if (j == 10) //A0,37.5,37.5
				{
					st_NW.mstr_2Position[i] += "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width 
				
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;					
				}
				else if (j == 11)//MADE IN KOREA  FDMade in China^FS
				{
					st_NW.mstr_2Contents[i] = "^" + strResultData[j];
					st_NW.mstr_2Contents_1[i] = strResultData[j].Mid(2,100);					
				}
				else if (j == 12) st_NW.mstr_2Contents[i] += "^" + strResultData[j];
			}
			else if (j == 13 || j == 14 || j == 15 || j == 16) //3¹ø À§Ä¡ ¹× 3¹ø ³»¿ë
			{
				if (j == 13) //^FO285,80
				{
					st_NW.mstr_3Position[i] = "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_3PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  
					st_NW.mstr_3PosX_1[i] = st_NW.mstr_3PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_3PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				
				}
				else if (j == 14) //^A0,50,40
				{
					st_NW.mstr_3Position[i] += "^" + strResultData[j];
				
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_3Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
				
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_3High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;					
				
				}
				else if (j == 15) //PARTNO FDM386B4G70DM0-YK03^FS
				{
					st_NW.mstr_3Contents[i] = "^" + strResultData[j];
					st_NW.mstr_3Contents_1[i] = strResultData[j].Mid(2, 100);
					st_NW.mstr_Recive_PartNo[i] = st_NW.mstr_3Contents_1[i];//2012,0911
				}
				else if (j == 16) st_NW.mstr_3Contents[i] += "^" + strResultData[j];
			}
			else if (j == 17 || j == 18 || j == 19 || j == 20) //3¹ø µÚ À§Ä¡ ¹× 3¹ø µÚ ³»¿ë
			{
				if (j == 17) //FO730,75
				{
					st_NW.mstr_3Position_back[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_3PosX_back_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_3PosX_back_1[i] = st_NW.mstr_3PosX_back_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_3PosY_back_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
									
				}
				else if (j == 18) 
				{
					st_NW.mstr_3Position_back[i] += "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_3Wid_back_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
				
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_3High_back_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				}
				else if (j == 19) 
				{
					st_NW.mstr_3Contents_back[i] = "^" + strResultData[j];
					st_NW.mstr_3Contents_back_1[i] = strResultData[j].Mid(2,100);
				
					int m_d_data = 	atoi(st_NW.mstr_3Contents_back_1[i]);
					st_NW.mstr_3Contents_back_1[i] = atoi(st_NW.mstr_3Contents_back_1[i]) + 1;
					st_NW.mstr_3Contents_back_1[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 20) st_NW.mstr_3Contents_back[i] += "^" + strResultData[j];
			}
			else if (j == 21 || j == 22 || j == 23 || j == 24 || j == 25) //4¹ø À§Ä¡ ¹× 4¹ø ³»¿ë(¹ÙÄÚµå)
			{
				if (j == 21) 
				{
					st_NW.mstr_4Position[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_4PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_4PosX_1[i] = st_NW.mstr_4PosX_1[i].Mid(2,100);// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_4PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				}
				else if (j == 22) 
				{
					st_NW.mstr_4Position[i] += "^" + strResultData[j];
				
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // BY3
					st_NW.mstr_4Etc_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  

 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_4Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
				
					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);  
					st_NW.mstr_4High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				}
				else if (j == 23) 
				{
					st_NW.mstr_4Position[i] += "^" + strResultData[j];
					st_NW.mstr_4Contents_1[i] = strResultData[j];  // BC,N,N,0,N,N  					
				}
				else if (j == 24)
				{
					st_NW.mstr_4Contents[i] = "^" + strResultData[j];
					st_NW.mstr_4Contents_2[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 25) st_NW.mstr_4Contents[i] += "^" + strResultData[j];
			}
			else if (j == 26 || j == 27 || j == 28 || j == 29) //2D À§Ä¡ ¹× 2D ³»¿ë
			{
				if (j == 26) 
				{
					st_NW.mstr_2DPosition[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2DPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_2DPosX_1[i] = st_NW.mstr_2DPosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_2DPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				


				}
				else if (j == 27) 
				{
					st_NW.mstr_2DPosition[i] += "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // BXN
					st_NW.mstr_2DEtc_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  

 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2DWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
				
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2DHigh_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // High  

					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_2DEtc_2[i] = strResultData[j].Mid(nStartIndex1, 100);  // etc
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				

				}
				else if (j == 28) 
				{
					st_NW.mstr_2DContents[i] = "^" + strResultData[j];
					st_NW.mstr_2DContents_1[i] = strResultData[j];
				}
				else if (j == 29) st_NW.mstr_2DContents[i] += "^" + strResultData[j];
			}
			else if (j == 30 || j == 31 || j == 32 || j == 33) //Unkown2 À§Ä¡ ¹× Unkown2 ³»¿ë
			{
				if (j == 30) //M393B4G70BM0-YF8Q8
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] = "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
						st_NW.mstr_Unkown2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
						st_NW.mstr_Unkown2PosX_1[i] = st_NW.mstr_Unkown2PosX_1[i].Mid(2,100);	// FO X
						
						nStartIndex1 = nEndIndex1 + 1;
						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
						st_NW.mstr_Unkown2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;			
					}
					else
					{
						st_NW.mstr_LotNoPosition[i] = "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
						st_NW.mstr_LotNoPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
						st_NW.mstr_LotNoPosX_1[i] = st_NW.mstr_LotNoPosX_1[i].Mid(2,100);	// FO X
						
						nStartIndex1 = nEndIndex1 + 1;
						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
						st_NW.mstr_LotNoPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;				
					}
				}
				else if (j == 31) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
						
 						nStartIndex1 = nEndIndex1 + 1;
 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
						st_NW.mstr_Unkown2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
						nStartIndex1 = nEndIndex1 + 1;
						st_NW.mstr_Unkown2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;				
					}
					else
					{
						st_NW.mstr_LotNoPosition[i] += "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0R
						
 						nStartIndex1 = nEndIndex1 + 1;
 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
						st_NW.mstr_LotNoWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
						nStartIndex1 = nEndIndex1 + 1;
						st_NW.mstr_LotNoHigh_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;			
					}
				}
				else if (j == 32) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
						st_NW.mstr_Unkown2_1[i] = strResultData[j].Mid(2,100);
					}
					else
					{
						st_NW.mstr_LotNoContents[i] = "^" + strResultData[j];
						st_NW.mstr_LotNoContents_1[i] = strResultData[j].Mid(2, 100);


					}
				}
				else if (j == 33) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
					}
					else
					{
						st_NW.mstr_LotNoContents[i] += "^" + strResultData[j];
					}

				}
			}
			else if (j == 34 || j == 35 || j == 36 || j == 37) //Lot No À§Ä¡ ¹× Lot No ³»¿ë
			{
				if (j == 34) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_LotNoPosition[i] = "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
						st_NW.mstr_LotNoPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
						st_NW.mstr_LotNoPosX_1[i] = st_NW.mstr_LotNoPosX_1[i].Mid(2,100);	// FO X
						
						nStartIndex1 = nEndIndex1 + 1;
						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
						st_NW.mstr_LotNoPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;				
					}
					else
					{
						st_NW.mstr_Unkown2[i] = "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
						st_NW.mstr_Unkown2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
						st_NW.mstr_Unkown2PosX_1[i] = st_NW.mstr_Unkown2PosX_1[i].Mid(2,100);	// FO X
						
						nStartIndex1 = nEndIndex1 + 1;
						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
						st_NW.mstr_Unkown2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;			
					}			
				}
				else if (j == 35) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_LotNoPosition[i] += "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0R
						
 						nStartIndex1 = nEndIndex1 + 1;
 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
						st_NW.mstr_LotNoWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
						nStartIndex1 = nEndIndex1 + 1;
						st_NW.mstr_LotNoHigh_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;			
					}
					else
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
						
 						nStartIndex1 = nEndIndex1 + 1;
 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
						st_NW.mstr_Unkown2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
						nStartIndex1 = nEndIndex1 + 1;
						st_NW.mstr_Unkown2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;				
					}
				}
				else if (j == 36) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_LotNoContents[i] = "^" + strResultData[j];
						st_NW.mstr_LotNoContents_1[i] = strResultData[j].Mid(2, 100);
					}
					else
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
						st_NW.mstr_Unkown2_1[i] = strResultData[j].Mid(2,100);
					}
				}
				else if (j == 37) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_LotNoContents[i] += "^" + strResultData[j];
						st_NW.mstr_Print_Data[i] = st_NW.mstr_Unkown1[i]; 
						st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_1Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_2Position[i] + st_NW.mstr_2Contents[i]; 
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position[i] + st_NW.mstr_3Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position_back[i] + st_NW.mstr_3Contents_back[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_4Position[i] + st_NW.mstr_4Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_2DPosition[i] + st_NW.mstr_2DContents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_Unkown2[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_LotNoPosition[i] + st_NW.mstr_LotNoContents[i];
					}
					else
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
						st_NW.mstr_Print_Data[i] = st_NW.mstr_Unkown1[i]; 
						st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_1Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_2Position[i] + st_NW.mstr_2Contents[i]; 
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position[i] + st_NW.mstr_3Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position_back[i] + st_NW.mstr_3Contents_back[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_4Position[i] + st_NW.mstr_4Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_2DPosition[i] + st_NW.mstr_2DContents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_LotNoPosition[i] + st_NW.mstr_LotNoContents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_Unkown2[i];
					}	 
					Func.On_LogFile_Add(0, st_NW.mstr_Print_Data[i]);
					Func.On_LogFile_Add(99, st_NW.mstr_Print_Data[i]);
					break;
////////////////////
				}
			}
		}
	return TRUE;

}

////
int CPublic_Function::OnBcrRev_Excution()
{
	int FuncRet = RET_PROCEED;
	CString str_serial,str_partname;

	switch (n_bcrlot_step)
	{
	case 0:
		// **********************************************************************
		//  ¼­¹ö¿ÍÀÇ Åë½ÅÀü¿¡ Á¢¼Ó »óÅÂ¸¦ È®ÀÎÇÑ´Ù.
		// **********************************************************************
// 		if(COMI.mn_simulation_mode == 1) // 20130509
// 		{ 
// 			n_bcrlot_step = 10000;
// 			break;
// 		}

		if (st_client[NETWORK_CLIENT_1].n_connect == CTL_NO)		// Á¢¼ÓÁßÀÌ ¾Æ´Ï¸é Á¢¼ÓÀ» ½ÃµµÇÑ´Ù.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
			NetworkConnectTime[0] = GetCurrentTime();
			n_bcrlot_step = 10;
		}
		else													// Á¢¼ÓÁßÀÌ¸é...
		{
			n_bcrlot_step = 100;
		}
		break;

	case 10:
		// **********************************************************************
		//  Á¢¼ÓÀ» È®ÀÎÇÑ´Ù.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != CTL_YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 10000)	// 10ÃÊ µ¿¾È ¿¬°áÀÌ µÇÁö ¾ÊÀ¸¸é...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
				
				n_bcrlot_step = 5000;		
				
				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] ¿¬°áÀÌ µÇÁö ¾Ê½À´Ï´Ù.");
				if(g_local.GetLocalType() == LOCAL_ENG) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] is not connected");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ¿Ï·á Ãâ·Â ¿äÃ»
				//FuncRet = RET_ERROR;
			}
		}
		else											// ¿¬°áÀÌ µÇ¾ú´Ù.
		{
			n_bcrlot_step = 100;
		}
		break;

	case 100:
		// **********************************************************************
		//  ½ÃÀÛÀ» ¾Ë¸°´Ù.
		// **********************************************************************
		{
			sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NW_LABEL_INFO));

			//if (Ret == RET_GOOD)
			//{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_1);	// ¸ÞÀÎ ÇÁ·¹ÀÓÀ¸·Î ¸Þ½ÃÁö Àü¼Û
				NetworkConnectTime[0] = GetCurrentTime();
				n_bcrlot_step = 200;
			//}
		}
		break;

	case 200:
		// **********************************************************************
		//  ¿Ï·á°¡ µÉ¶§±îÁö ´ë±â.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != CTL_NO)	// ÀÛ¾÷ÀÌ ³¡³ªÁö ¾Ê¾Ò´Ù¸é...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 13000)	// 10ÃÊ µ¿¾È µé¾î¿ÀÁö ¾Ê´Â´Ù¸é...
			{
				n_bcrlot_step = 5000;

				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] 10ÃÊ°£ ´äº¯ÀÌ ¿ÀÁö ¾Ê¾Ò½À´Ï´Ù.");
				if(g_local.GetLocalType() == LOCAL_ENG) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] Answer did not come 10 seconds");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ¿Ï·á Ãâ·Â ¿äÃ»
//				FuncRet = RET_ERROR;
				break;

			}
		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == CTL_NO)	// ÀÛ¾÷ÀÌ ³¡³µ´Ù¸é...
		{
			n_bcrlot_step = 300;
		}	
		break;

	case 300:
		// **********************************************************************
		//  Reply¸¦ ºÐ¼®ÇÑ´Ù.
		// **********************************************************************
		if(st_NW.mstr_result[1] == "PASS")
		{
			n_bcrlot_step = 400;
		}
		else								// ºñÁ¤»óÀÌ¸é...
		{
			n_bcrlot_step = 5000;

			if (st_NW.mstr_msg[1] == "")
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER - LIST] ºñÁ¤»ó µ¥ÀÌÅÍ°¡ Àü¼ÛµÇ¾ú½À´Ï´Ù.");
				if(g_local.GetLocalType() == LOCAL_ENG) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER - LIST] data was sent to the abnormal");
				}

			}
			else
			{	
				st_NW.mstr_msg[1].TrimRight(' ');
				st_NW.mstr_msg[1].TrimLeft(' ');
				sprintf(st_msg.c_abnormal_msg, ("[LABEL SERVER ERROR] : " + st_NW.mstr_msg[1]));
				

			}			
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ¿Ï·á Ãâ·Â ¿äÃ»
			//FuncRet = RET_ERROR;

		}
		break;

	case 400:
		n_bcrlot_step = 500;
		break;

	case 500:
		// ´Ù Á¤»óÀÌ´Ï±î, Å×½ºÅÍ¿¡°Ô »õ·Î¿î LOTÀÓÀ» ¾Ë·ÁÁØ´Ù.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		}
		ml_untilwait_time[0] = GetCurrentTime();
		n_bcrlot_step = 600;
		
		break;

	case 600:
		ml_untilwait_time[1] = GetCurrentTime();
		ml_untilwait_time[2] = ml_untilwait_time[1] - ml_untilwait_time[0];
		if(st_handler.mn_receive == CTL_YES)
		{
			n_bcrlot_step = 10000;
		}
		if(ml_untilwait_time[2] > 5000)
		{
			n_bcrlot_step = 5000;
		}
		break;

	case 5000:
		FuncRet = RET_ERROR;
		n_bcrlot_step = 0;
		break;

	case 10000:
		FuncRet = RET_GOOD;
		n_bcrlot_step = 0;
		break;
	}

	return FuncRet;
}


//////////////////////////////////////////////////////////////////////////////////////////
int CPublic_Function::variable_data_initialize(int n_mode)
{
//Àü¿ª º¯¼ö Å¬¸®¾î 
//	int i=0;
//	memset(&st_step, 0x00, sizeof(st_step_param));	//data clear 
//	memset(&st_sync, 0x00, sizeof(st_sync_param));	//data clear 
//	memset(&st_var, 0x00, sizeof(st_variable_param));	//data clear 
//	for(i=0; i<3; i++) // t box Á¤º¸ Å¬¸®¾î 
//	{
//		memset(&st_tbox_info[i], 0x00, sizeof(st_tbox_info_param));	//data clear 
//	}
//	for(i=0; i<11; i++) //id buffer Á¤º¸ Å¬¸®¾î 
//	{
//		memset(&st_idbuff_info[i], 0x00, sizeof(st_idbuffer_info_param));	//data clear 
//	}
	return 1;
}

///////////////////////////////////////////////////////////////////////
// James 2010,0726 Àåºñ º¹±Í µ¿ÀÛ¿¡ »ç¿ëÇÏ±â À§ÇÏ¿© Ãß°¡ 
///////////////////////////////////////////////////////////////////////
int CPublic_Function :: Handler_Recovery_Data(int n_debug, int n_site, int n_mode)
{//int n_debug //091206 Ãß°¡ steplist¿Í ÇÔ²² ºñ±³½Ã »ç¿ëÇÏ±â À§ÇØ Ãß°¡ 
	int i=0, j=0;
	FILE *fp;
	int nExistFile, nFuncRet = RET_GOOD;
	CString sFileName, sFolderName;

	CString sDebug_FileName, sTimeName; //091206 Ãß°¡ 
	COleDateTime mtime_cur;						// ÇöÀç ½Ã°£ ÀúÀå º¯¼ö
	int nYear, nMonth, nDay, nHour, nMinite;

	mtime_cur = COleDateTime::GetCurrentTime(); 
	nYear   = mtime_cur.GetYear();  
	nMonth  = mtime_cur.GetMonth();  
    nDay    = mtime_cur.GetDay(); 
	nHour   = mtime_cur.GetHour(); 
	nMinite = mtime_cur.GetMinute(); 
	sTimeName.Format("%04d%02d%02d%02d%02d_", nYear, nMonth, nDay, nHour, nMinite);
	sFolderName.Format("%04d%02d%02d%02d%02d", nYear, nMonth, nDay, nHour, nMinite);

//james 2010.0824	sDebug_FileName = (st_path.str_path_back) + "Step\\" + sFolderName + "\\";

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2010.0913  Ãß°¡, ¾ÈÀü»ó ±â´É Ãß°¡ , ÃßÈÄ ¿øÀÎ ºÐ¼® ¿ä¸ÁµÊ 
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	st_path.str_handler = _T("d:\\AMT85890\\"); //james 2010.0913 °æ·Î°¡ ¾ø¾îÁö´Â ¹®Á¦·Î ÀÏ´Ü °­Á¦·Î Ãß°¡ 
	st_path.str_path_back =		st_path.str_handler + _T("BackUp\\");		//c:\\Backup\\ // BACKUP ÆÄÀÏ »ý¼º Æú´õ ¼³Á¤


	sDebug_FileName = (st_path.str_path_back) + "Step\\" + sFolderName + "\\";

	switch(n_site)
	{
	
	case 0:	//°ø¹é 
		break;

////////////////////////////////////////////////////////////////////////////////////////////////////
//Àü¿ª º¯¼öÀ» °ü¸®ÇÑ´Ù  100~ 109 ±îÁö ÃÑ 10°³ÀÇ µ¥ÀÌÅÍ¸¦ °ü¸®ÇÑ´Ù 
////////////////////////////////////////////////////////////////////////////////////////////////////
	case 1: //step ÀúÀå 
		if(n_debug == 1)
		{
			Func.CreateFolder(sDebug_FileName);

			sDebug_FileName += "st_step.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = st_path.str_handler + "Recovery\\st_step.dat";	
		}
		
		if(n_mode == DATA_WRITE) //file write
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //Á¤»ó 
				fwrite(&st_step, sizeof(st_step_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}
					clearerr(fp);
					fclose(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}
				nFuncRet = RET_ERROR;
				break;
			}

			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //Á¤»ó 
				fread(&st_step, sizeof(st_step_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}
					clearerr(fp);
					fclose(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}			
		}
		break;

	case 2: //st_sync Àåºñ ÀÛ¾÷°ü·Ã º¯¼öµé °ü¸®
		if(n_debug == 1)
		{
			sDebug_FileName += "st_sync.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = st_path.str_handler + "Recovery\\st_sync.dat";	
		}
		
		if(n_mode == DATA_WRITE) //file write
		{			
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //Á¤»ó 
				fwrite(&st_sync, sizeof(st_sync_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}
					clearerr(fp);
					fclose(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}
				nFuncRet = RET_ERROR;
				break;
			}

			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //Á¤»ó 
				fread(&st_sync, sizeof(st_sync_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}
					clearerr(fp);
					fclose(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);

				//james 2010.0829 ÃÊ±âÈ­ º¹±¸½Ã ¾ÈÀü ÇÃ·¹±×¸¦ Å¬¸®¾îÇÏÁö ¾ÊÀ¸¸é ¿¡·¯ ¹ß»ý 
				//st_work.n_safety_dclz_axis = NO;
				//st_work.n_safety_dcuz_axis = NO;
				
				//st_work.n_safety_uldfz_axis = NO; 
				//st_work.n_safety_uldrz_axis = NO;				
			}		
		}
		break;

	case 3: //st_variable_param  °ü¸®
		if(n_debug == 1)
		{
			sDebug_FileName += "st_var.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = st_path.str_handler + "Recovery\\st_var.dat";
		}
		
		if(n_mode == DATA_WRITE) //file write
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //Á¤»ó 
				fwrite(&st_var, sizeof(st_variable_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}
					clearerr(fp);
					fclose(fp);
					nFuncRet = RET_ERROR;
					break;
				}
				fclose(fp);
			}				 
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}
				nFuncRet = RET_ERROR;
				break;
			}

			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //Á¤»ó 
				fread(&st_var, sizeof(st_variable_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}
					clearerr(fp);
					fclose(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}	
		}
		break;

	case 4: //st_tbox_info[i] t box Á¤º¸ µ¥ÀÌÅÍ °ü¸® 
 
//		if(n_debug == 1)
//		{
//			sDebug_FileName += "st_tbox_info.dat";
//			sFileName = sDebug_FileName;
//		}
//		else
//		{
//			sFileName = st_path.str_handler + "Recovery\\st_tbox_info.dat";
//		}
//	 
//		if(n_mode == DATA_WRITE) //file write
//		{
//			if((fp=fopen(sFileName,"wb")) == NULL)
//			{			
//				if (st_handler.cwnd_list != NULL)
//				{
//					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//				}				
//				nFuncRet = RET_ERROR;
//				break;
//			}
//			else
//			{ //Á¤»ó 
//				for(i=0; i<3; i++)
//				{
//					fwrite(&st_tbox_info[i], sizeof(st_tbox_info_param), 1, fp);
//					if (ferror(fp)) 
//					{
//						if (st_handler.cwnd_list != NULL)
//
//						{
//							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
//							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//						}
//						clearerr(fp);
//						nFuncRet = RET_ERROR;
//						break;
//					}	
//				}
//				fclose(fp);
//			}		
//		}
//		else if(n_mode == DATA_READ) //file read
//		{
//			nExistFile = access(sFileName, 0);	
//			if (nExistFile)
//			{
//				if (st_handler.cwnd_list != NULL)
//				{
//					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//				}
//				nFuncRet = RET_ERROR;
//				break;
//			}
//
//			if((fp=fopen(sFileName,"rb")) == NULL)
//			{			
//				if (st_handler.cwnd_list != NULL)
//				{
//					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//				}				
//				nFuncRet = RET_ERROR;
//				break;
//			}
//			else
//			{ //Á¤»ó 
//				for(i=0; i<3; i++)
//				{
//					fread(&st_tbox_info[i], sizeof(st_tbox_info_param), 1, fp);
//					if (ferror(fp)) 
//					{
//						if (st_handler.cwnd_list != NULL)
//						{
//							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
//							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//						}
//						clearerr(fp);
//						nFuncRet = RET_ERROR;
//						break;
//					}	
//				}
//				fclose(fp);
//			}		
//		}
		break;

	case 5: //st_idbuff_info  °¢Á¾ »ç¿ë º¯¼öµé¿¡ °ü¸® 
 

//		if(n_debug == 1)
//		{
//			sDebug_FileName += "st_idbuff_info.dat";
//			sFileName = sDebug_FileName;
//		}
//		else
//		{
//			sFileName = st_path.str_handler + "Recovery\\st_idbuff_info.dat";
//		}
//		
//		if(n_mode == DATA_WRITE) //file write
//		{
//			if((fp=fopen(sFileName,"wb")) == NULL)
//			{			
//				if (st_handler.cwnd_list != NULL)
//				{
//					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//				}				
//				nFuncRet = RET_ERROR;
//				break;
//			}
//			else
//			{ //Á¤»ó 
//				for(i=0; i<9; i++) //id buffer Á¤º¸
//				{
//					fwrite(&st_idbuff_info[i], sizeof(st_idbuffer_info_param), 1, fp);
//					if (ferror(fp)) 
//					{
//						if (st_handler.cwnd_list != NULL)
//						{
//							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
//							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//						}
//						clearerr(fp);
//						nFuncRet = RET_ERROR;
//						break;
//					}
//					fclose(fp);
//				}
//			}				 
//		}
//		else if(n_mode == DATA_READ) //file read
//		{
//			nExistFile = access(sFileName, 0);	
//			if (nExistFile)
//			{
//				if (st_handler.cwnd_list != NULL)
//				{
//					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//				}
//				nFuncRet = RET_ERROR;
//				break;
//			}
//
//			if((fp=fopen(sFileName,"rb")) == NULL)
//			{			
//				if (st_handler.cwnd_list != NULL)
//				{
//					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//				}				
//				nFuncRet = RET_ERROR;
//				break;
//			}
//			else
//			{ //Á¤»ó 
//				for(i=0; i<9; i++) //id buffer Á¤º¸
//				{
//					fread(&st_idbuff_info[i], sizeof(st_idbuffer_info_param), 1, fp);
//					if (ferror(fp)) 
//					{
//						if (st_handler.cwnd_list != NULL)
//						{
//							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
//							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
//						}
//						clearerr(fp);
//						nFuncRet = RET_ERROR;
//						break;
//					}	
//				}
//
//			}	
//		}
		break;



		
	case 10: //¹é¾÷ÇÑ ÀÚ·á¸¦ ÃÊ±âÈ­ ÇØ¾ß ÇÏ´Â º¯¼ö´Â ÀÌ°÷¿¡¼­ ÃÊ±âÈ­ ÇÑ´Ù 
		if (n_mode == DATA_READ) //file read
		{
 
			//Àç ÃÊ±âÈ­°¡ ÇÊ¿äÇÑ º¯¼ö 
			st_handler.mn_alarm_screen = FALSE;  // ÇöÀç ¾Ë¶÷ È­¸éÀÌ Ãâ·ÂµÇÁö ¾Ê¾Ò´Ù°í ÇØ´ç ÇÃ·¡±× ¼³Á¤ 
			st_var.l_handler_recovery_flag[1] = CTL_YES;
			COMI.mn_run_status = dINIT;
 
		}
		break;



	}

	return nFuncRet;
}

CString CPublic_Function::GetPrintFormat()
{
// 	CString strFotmat_1="^XA^LH200,047^MMT^PON^FS^CI28^CW2,E:KR.TTF^FS^FO5,95^A2,50,50^FR^FDSAMSUNG SSD 840 EVO mSATA^FS^FO5,160^A2,50,50^FR^FD512GB^FS^FO5,220^A2,50,50^FR^FDMade in Korea^FS";
// 	
// 	CString strFotmat_2="^FO900,95^GB685,100,100^FS^FO945,95^BY3,3,100^FR^BC,,N^FDS1AXNEAD908028^FS^FO900,215^A0,50,70^FR^FDS/N: 1AXNEAD908028^FS^FO70,470^A2I,48,48^FR^FDModel Code: MZ-7PD512BW^FS^FO1195,470^A2I,48,48^FR^FDModel: MZ-7PD512^FS";
// 	
// 	CString strFotmat_3="^FO5,557^GB780,275,180^FS^FO895,557^GB780,275,180^FS^FO1575,590^FPV^A2I,75,75^FR^FDUPC^FS^FO950,630^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276001180^FS^FO680,590^FPV^A2I,75,75^FR^FDEAN^FS";
// 	
// 	CString strFotmat_4="^FO58,630^BY6,3,180^FR^BEI,180,Y,N^FD8806085216716^FS^PQ1^XZ";

// 	CString strFotmat_1="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSAMSUNG SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD500GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS";
// 	CString strFotmat_2="^FO900,65^BY3,3,100^FR^BC,,N^FDS21GNWAFB00052^FS^FO900,185^A0,55,65^FR^FDS/N:S21GNWAFB00052^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E500BW^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E500^FS";
// 	CString strFotmat_3="^FO900,510^GB780,275,5^FS^FO1600,550^FPV^A0I,70,70^FR^FDUPC^FS^FO970,580^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276086040^FS^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS";
// 	CString strFotmat_4="^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522922^FS^PQ1^XZ";
	
	CString m_strPrintFormat = "";

// 	CString strFotmat_1="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD250GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS";
// 	CString strFotmat_2="^FO900,65^BY3,3,100^FR^BC,,N^FDS21PNYAG302044^FS^FO900,185^A0,55,65^FR^FDS/N:S21PNYAG302044^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E250B/EU^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E250^FS";
// 	CString strFotmat_3="^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522977^FS^PQ1^XZ";
	
// 	CString strFotmat_1="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD250GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS";
// 	CString strFotmat_2="^FO900,65^BY3,3,100^FR^BC,,N^FDS21PNYAG314625^FS^FO900,185^A0,55,65^FR^FDS/N:S21PNYAG314625^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E250B/EU^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E250^FS";
// 	CString strFotmat_3="^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522977^FS^PQ1^XZ";

	CString strFotmat_1="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD250GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS";
	CString strFotmat_2="^FO900,65^BY3,3,100^FR^BC,,N^FDS21PNYAG314488^FS^FO900,185^A0,55,65^FR^FDS/N:S21PNYAG314488^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E250B/EU^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E250^FS";
	CString strFotmat_3="^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522977^FS^PQ1^XZ";
	m_strPrintFormat = strFotmat_1 + strFotmat_2 + strFotmat_3;// + strFotmat_4;
	
	return m_strPrintFormat;

}

CString CPublic_Function::GetBcrPrintFormat( int opt, CString strSerial)
{
	// 	CString strFotmat_1="^XA^LH200,047^MMT^PON^FS^CI28^CW2,E:KR.TTF^FS^FO5,95^A2,50,50^FR^FDSAMSUNG SSD 840 EVO mSATA^FS^FO5,160^A2,50,50^FR^FD512GB^FS^FO5,220^A2,50,50^FR^FDMade in Korea^FS";
	// 	
	// 	CString strFotmat_2="^FO900,95^GB685,100,100^FS^FO945,95^BY3,3,100^FR^BC,,N^FDS1AXNEAD908028^FS^FO900,215^A0,50,70^FR^FDS/N: 1AXNEAD908028^FS^FO70,470^A2I,48,48^FR^FDModel Code: MZ-7PD512BW^FS^FO1195,470^A2I,48,48^FR^FDModel: MZ-7PD512^FS";
	// 	
	// 	CString strFotmat_3="^FO5,557^GB780,275,180^FS^FO895,557^GB780,275,180^FS^FO1575,590^FPV^A2I,75,75^FR^FDUPC^FS^FO950,630^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276001180^FS^FO680,590^FPV^A2I,75,75^FR^FDEAN^FS";
	// 	
	// 	CString strFotmat_4="^FO58,630^BY6,3,180^FR^BEI,180,Y,N^FD8806085216716^FS^PQ1^XZ";
	
	CString strFotmat_1="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSAMSUNG SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD500GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS";
	CString strFotmat_2="^FO900,65^BY3,3,100^FR^BC,,N^FD";
	CString strSerial1 = "S21GNWAFB00052";
	if(opt != 0) strSerial1 = (LPCTSTR)strSerial;
	CString strFotmat_3="^FS^FO900,185^A0,55,65^FR^FDS/N:";
	CString strSerial2 = "S21GNWAFB00052";
	if(opt != 0) strSerial2 = (LPCTSTR)strSerial;
	CString strFotmat_4="^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E500BW^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E500^FS";
	CString strFotmat_5="^FO900,510^GB780,275,5^FS^FO1600,550^FPV^A0I,70,70^FR^FDUPC^FS^FO970,580^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276086040^FS^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS";
	CString strFotmat_6="^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522922^FS^PQ1^XZ";
	
	
	CString m_strPrintFormat = "";
	
	m_strPrintFormat = strFotmat_1 + strFotmat_2 + strSerial1 + strFotmat_3 + strSerial2 + strFotmat_4 + strFotmat_5 + strFotmat_6;
	
	return m_strPrintFormat;
	
}

////2014.1114 ysj
void CPublic_Function::OnBcrInputreader() //input bcr reader
{
	CString strBcrCommand;
	
	st_sync.n_barcode_read_serial_num[0][CTL_LEFT] = 0;
	st_sync.n_barcode_read_serial_num[0][CTL_RIGHT] = 0;
	
	st_work.strBarcodeRecive = "";//¼ö½Å ¸Þ¼¼Áö ÃÊ±âÈ­
	
	clsBarcode.OnBarcodeReset();
	
	strBcrCommand.Format("%cR%c", 0x02, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
	clsBarcode.OnDataSend(strBcrCommand);
	
	//	st_serial.mstr_snd[BCR_PORT -1] = strBcrCommand;
	//	::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
	//::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strBcrCommand);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
	
}

void CPublic_Function::OnBcrOutputreader() //input bcr reader
{
	// TODO: Add your control notification handler code here
	CString strBcrCommand;
	
	st_sync.n_barcode_read_serial_num[1][CTL_LEFT] = 0;
	st_sync.n_barcode_read_serial_num[1][CTL_RIGHT] = 0;
	
	st_work.strBarcodeRecive = "";//¼ö½Å ¸Þ¼¼Áö ÃÊ±âÈ­
	clsBarcode2.OnBarcodeReset();
	
	strBcrCommand.Format("%cR%c", 0x02, 0x03);//¹ÙÄÚµå ¸®´õ±â ÀÐ±â ½ÃÀÛ ¸í·É
	clsBarcode2.OnDataSend(strBcrCommand);
	
	
	//	st_serial.mstr_snd[BCR_PORT2 -1] = strBcrCommand;
	//	::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT2, 0);
	//::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test2] %s",strBcrCommand);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
	}
	
}

//==================================================================//
// Æ¯Á¤ÇÑ ¹®ÀÚ¿­ »çÀÌÀÇ µ¥ÀÌÅÍ ¹ÝÈ¯
//==================================================================//
CString CPublic_Function::OnFindInRcvMsg(CString szFind, CString szOrg, CString szDelimiter) 
{
	CString sRetChar;	// ¸®ÅÏÇÒ ¹®ÀÚ¿­ ÀúÀå º¯¼ö
	
	int nOrgLen		= szOrg.GetLength();	// ÀÔ·ÂµÈ ¹®ÀÚ¿­ÀÇ ÀüÃ¼ ±æÀÌ
	// ÀüÃ¼ ¹®ÀÚ¿­ Áß¿¡¼­ Ã£°íÀÚ ÇÏ´Â ¹®ÀÚ¿­ÀÇ ½ÃÀÛ À§Ä¡ °Ë»ö
	int nFindPos	= szOrg.Find(szFind);
	int nFindLen	= szFind.GetLength();	// Ã£À» ¹®ÀÚ¿­ÀÇ ÃÑ ±æÀÌ °è»ê
	// µ¥ÀÌÅÍ »çÀÌÀÇ ±¸ºÐÀÚ À§Ä¡ °Ë»ö
	// - Ã£°íÀÚ ÇÏ´Â ¹®ÀÚ¿­ÀÇ ´ÙÀ½ À§Ä¡ºÎÅÍ Ã£±â ½ÃÀÛÇÔ
	//int nEndPos	= szOrg.Find(szDelimiter, nFindPos+1);
	int nEndPos;
	//2015.0419
	if(szDelimiter =="END")
	{
		nEndPos= szOrg.GetLength();
	}
	else
	{
		nEndPos= szOrg.Find(szDelimiter, (nFindPos+nFindLen));
	}
	// ¿À·ù Á¶°Ç È®ÀÎ
	//if ((nFindPos<0 || nEndPos<0) || (nEndPos <= (nFindPos+nFindLen)))
	// ±ÔÀÌ¸® ¼öÁ¤ [2014.11.24]
	// ¸¶Áö¸· À§Ä¡ÀÇ Á¤º¸´Â ±¸ºÐÀÚ°¡ Á¸ÀçÇÏÁö ¾Ê¾Æ ¼öÁ¤
	if (nFindPos<0 ||
		(nEndPos >= 0 && (nEndPos < (nFindPos+nFindLen)+1)))
	{
		sRetChar = "";
	}
	// ¸¶Áö¸· Á¤º¸´Â ±¸ºÐÀÚ°¡ ¾øÀ½
	else if (nEndPos<0)
	{
		if(szDelimiter =="END")
		{
			sRetChar = szOrg.Mid(nFindPos+nFindLen+1, nOrgLen-(nFindPos+nFindLen));
		}
		else
		{
			sRetChar = szOrg.Mid(nFindPos+nFindLen, nOrgLen-(nFindPos+nFindLen));
		}
	}
	else
	{
		// [°Ë»ö¹®ÀÚ¿­~±¸ºÐÀÚ] »çÀÌÀÇ Á¤º¸ ¼³Á¤
		sRetChar = szOrg.Mid(nFindPos+nFindLen, nEndPos-(nFindPos+nFindLen));
	}
	return sRetChar;
}

CString CPublic_Function::OnBodyAnalysis( int nMode, CString strRecv, CString strFind )
{
	CString strData = "";
	int nLength;
	int nPos[4];
	
	nLength = strRecv.GetLength();
	
	nPos[0] = -1;
	nPos[0] = strRecv.Find(strFind, 0);
	
	if(nPos[0] < 0) return strData;
	
	nPos[1] = -1;
	nPos[1] = strRecv.Find("=", nPos[0]+1);
	
	if(nPos[1] < 0) return strData;
	
	if(nMode == 0)
	{
		nPos[2] = -1;
		nPos[2] = strRecv.Find(" ", nPos[0]+1);
		
		if(nPos[2] == -1)
		{
			strData = strRecv.Mid(nPos[1]+1, nLength-(nPos[1]+1));
		}
		else
		{
			strData = strRecv.Mid(nPos[1]+1, nPos[2]-(nPos[1]+1));
		}
	}
	else if( nMode == 2 )
	{
		nPos[2] = strRecv.Find("=", nPos[1]+1);
		strData = strRecv.Mid(nPos[2]+1); // ÇØ´ç itemÀÇ ³¡ À§Ä¡¸¦ Ã£´Â´Ù.	// DATAÀÇ ±æÀÌ °è»ê.
	}
	else if( nMode == 3 )
	{
		nPos[2] = -1;
		nPos[2] = strRecv.Find("^XZ", nPos[1]+1);
		
		strData = strRecv.Mid(nPos[1]+1, (nPos[2] +3)-(nPos[1]+1));
	}
	else/* if( nMode == 1 )*/
	{
		nPos[2] = nPos[3]= -1;
		nPos[2] = strRecv.Find("(", nPos[1]+1);
		nPos[3] = strRecv.Find(")", nPos[1]+2);
		nLength = nPos[3] - nPos[2] -1;
		strData = strRecv.Mid( nPos[2] +1, nLength);
		
		//		strData = strRecv.Mid(nPos[1] +1, nPos[2]-(nPos[1]));
	}
	
	return strData;
}

////2015.0114
//==================================================================//
// µµ¾îÀÇ [¿­¸²/´ÝÈû] »óÅÂ ¸ð´ÏÅÍ¸µ
//==================================================================//
void CPublic_Function::OnMonitoring_Door() 
{
	int nRet = 0;
	int i=0, iSensor;
	CString sTemp="";
	
	// Auto ¸ðµåÀÎ °æ¿ì
	if (st_handler.nModeManual == CTL_NO)
	{
		// ÇöÀç ¾Ë¶÷ »óÅÂÀÌ¸é °­Á¦ ¸®ÅÏ
		if (COMI.mn_run_status == CTL_dJAM || COMI.mn_run_status == CTL_dWARNING || 
			COMI.mn_run_status == CTL_dLOTEND)
		{
			return;
		}
		
		nRet = OnWatchDoor();	// µµ¾î ¿­¸² »óÅÂ Á¶»ç [0:¸ðµÎ ´ÝÈû, 0ÀÌ»ó:¿­¸° µµ¾î Á¸Àç]
		if (nRet > 0)	// ¿­¸° µµ¾î°¡ 1°³ ÀÌ»ó Á¸ÀçÇÔ
		{
			for(i=0; i<10; i++)
			{
				iSensor =  g_ioMgr.get_in_bit( st_io.i_door_chk[i] );
				if (iSensor == IO_OFF)	// µµ¾î ¿­¸²
				{
// 					// 537000 1 00 "Door1_Open_Close_Check_Off_Error."
					if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
					{
						sprintf(st_msg.c_abnormal_msg, "Door_%d_Open_Close_Error_PS370%d", i+1, i+1);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					break;
				}
			}
		}
	}
	else
	{
		for(i=0; i<4; i++)
		{
			iSensor = g_ioMgr.get_in_bit( st_io.i_door_chk[i] );
			if (iSensor == IO_OFF)	// µµ¾î ¿­¸²
			{
				if (st_handler.nDetect_DoorOpen[i] != TRUE)
				{
					st_handler.nDetect_DoorOpen[i] = TRUE;
				}
			}
			else
			{
				if (st_handler.nDetect_DoorOpen[i] != FALSE)
				{
					st_handler.nDetect_DoorOpen[i] = FALSE;
				}
			}
		}
	}
}

int CPublic_Function::OnWatchDoor()
{
	int i;
	int Ret = 0, iSensor;
	
	// ¾î´À¹® ¿­·È´ÂÁö ¸Þ¼¼ÁöÃ¢ º¸¿©ÁØ´Ù.
	for( i = 0; i < 10; i++)
	{	
		iSensor = g_ioMgr.get_in_bit( st_io.i_door_chk[i] );
		{
			if (iSensor == IO_OFF)	// µµ¾î ¿­¸®
			{
				Ret = i+1;
				break;
			}
		}
	}
	
	return Ret;
}

//==================================================================//
// µµ¾î ¿­±â
//==================================================================//
void CPublic_Function::OnOpenDoor() 
{
	// ¼³ºñ »óÅÂ [Stop]
	// µ¿ÀÛ ¸ðµå [¸Å´º¾ó]

	if (st_handler.mn_menu_num == 701)
	{
	}
	else
	{
		if (COMI.mn_run_status == dSTOP && st_handler.nModeManual == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_door_lock, IO_OFF);
			
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[Unlock] All Door Unlocked.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[STOP+MANUAL], You Do.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
	}
}

//==================================================================//
// µµ¾î ´Ý±â
//==================================================================//
void CPublic_Function::OnCloseDoor() 
{
	CString sDoorState = OnCheck_DoorState();	// µµ¾îÀÇ [¿­¸²/´ÝÈû] »óÅÂ Ã¼Å©
	if (sDoorState == "[CLOSE]")
	{
		g_ioMgr.set_out_bit(st_io.o_door_lock, IO_ON);
		
		sprintf(st_msg.c_normal_msg, "[LOCK] All Door Locked.");
	}
	else
	{
		sprintf(st_msg.c_normal_msg, "%s", sDoorState);
	}
	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
}

//==================================================================//
// µµ¾îÀÇ [¿­¸²/´ÝÈû] »óÅÂ Ã¼Å©
//==================================================================//
CString CPublic_Function::OnCheck_DoorState() 
{
	CString sRetMsg = "[OPEN]", sTemp = "";
	int i=0;
	
	// ´ÝÈ÷¸é ON
	for(i=0; i<4; i++)
	{
		// µµ¾î°¡ ¿­·ÁÀÖÀ½
		if (g_ioMgr.get_in_bit( st_io.i_door_chk[i] ) != IO_ON)
		{
			sTemp.Format("<PS00%02d>", 1+i);
			sRetMsg += sTemp;
		}
	}
	
	int iLen = sRetMsg.GetLength();
	if (iLen <= 6)
	{
		sRetMsg = "[CLOSE]";
	}
	return sRetMsg;
}

//==================================================================//
// µµ¾î ¿­¸² »óÅÂ Á¶»ç [0:¸ðµÎ ´ÝÈû, 0ÀÌ»ó:¿­¸° µµ¾î Á¸Àç]
//==================================================================//
int CPublic_Function::OnIsDoorOpen()
{
	int i;
	int Ret = -1;
	
	// ¾î´À¹® ¿­·È´ÂÁö ¸Þ¼¼ÁöÃ¢ º¸¿©ÁØ´Ù.
	for( i = 0; i < 10; i++)
	{	
		if( g_ioMgr.get_in_bit( st_io.i_door_chk[i] ) != IO_ON )
		{
			Ret = i;
			break;
		}
	}
	
	return Ret;
}
////
//2015.0211
int CPublic_Function::OnIsInterruptCondition() 
{
	if (COMI.mn_motorbd_init_end	== BD_YES &&
		st_handler.nModeManual		== CTL_NO &&
		st_handler.mn_system_lock	== FALSE)
	{
		return 1;
	}
	return 0;
}

////2015.0221
void CPublic_Function::OnSavePPID()
{
	CString str_content="";    // ÇÑ ¶óÀÎ¿¡ Ãâ·ÂÇÒ ¹®ÀÚ¿­ ÀúÀå º¯¼ö
	CString str_temp_data="";  // ÀúÀåÇÒ Á¤º¸ ÀÓ½Ã ÀúÀå º¯¼ö
	CString str_folder_data="";					// ¾²·¹µå ½ºÅÜ ÀúÀå ÆÄÀÏ »ý¼ºÇÒ Æú´õ ÀúÀå º¯¼ö
	CString str_new_file="";						// »ý¼ºÇÒ ÆÄÀÏ¸í ÀúÀå º¯¼ö
	CString str_title_time="";						// ÆÄÀÏ¿¡ Ãâ·ÂÇÒ Å¸ÀÌÆ² ½Ã°£ ÀúÀå º¯¼ö
	
	CString str_temp_onoff="";
	
	CString str_file_folder="",mstr_temp=""; 
	
	str_folder_data = (LPCTSTR)st_path.mstr_recipe_path;
	str_temp_data.Format( "%s%s", str_folder_data, st_basic.mstr_device_name);
	
	DeleteFile(str_temp_data);
	str_file_folder.Format( "%s%s",str_folder_data,st_basic.mstr_device_name);
	
	// ************************************************************************** //
	// ÆÄÀÏ¿¡ Å¸ÀÌÆ² Á¤º¸ Ãâ·ÂÇÑ´Ù                                                //
	// ************************************************************************** //
	char chr_buf[20];
	
	mstr_temp = (LPCTSTR)st_lamp.mstr_equip_no;
	:: WritePrivateProfileString("Machine Serial", "Name", LPCTSTR(mstr_temp), str_file_folder);
	
	mstr_temp = (LPCTSTR)st_basic.mstr_device_name;
	:: WritePrivateProfileString("Recipe", "Name", LPCTSTR(mstr_temp), str_file_folder);		
	
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_run, chr_buf, 10));
	:: WritePrivateProfileString("Mode", "Mode", LPCTSTR(mstr_temp), str_file_folder);
	
	mstr_temp.Format("%d", st_basic.n_mode_device);
	:: WritePrivateProfileString("Mode", "Device mode", LPCTSTR(mstr_temp), str_file_folder);
	// ************************************************************************** //
// 	mstr_temp.Format("%s", st_basic.mstr_revipe_name);
// 	:: WritePrivateProfileString("Mode", "RECIPE_NAME", LPCTSTR(mstr_temp), str_file_folder);
	
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("Mode", "Network_Wait_Time", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_retry_cnt, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Network_Retry_Count", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp.Format("%d", st_basic.mn_Label_Print_Error_Cnt);
	:: WritePrivateProfileString("BasicData", "LabelPrintErrorCnt", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp.Format("%d", st_basic.mn_outconv);
	:: WritePrivateProfileString("BasicData", "Out_Conv", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp.Format("%d", st_basic.mn_overtime_density);
	:: WritePrivateProfileString("BasicData", "Over_Time_Density", LPCTSTR(mstr_temp), str_file_folder);

	if( g_lotMgr.GetLotCount() <= 0)
	{
		mstr_temp="";
		:: WritePrivateProfileString("Mode", "LOT_NAME", LPCTSTR(mstr_temp), str_file_folder);
	}
	else
	{
		mstr_temp=g_lotMgr.GetLotAt(0).GetLotID();
		:: WritePrivateProfileString("Mode", "LOT_NAME", LPCTSTR(mstr_temp), str_file_folder);
	}


////DENSITY FEEDER
	mstr_temp.Format("%0.3f", st_motor[M_D_FEEDER_1].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_FEEDER", "D_FEEDER_M_D_FEEDER_1", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_FEEDER_2].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_FEEDER", "D_FEEDER_M_D_FEEDER_2", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_FEEDER_3].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_FEEDER", "D_FEEDER_M_D_FEEDER_3", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_FEEDER_4].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_FEEDER", "D_FEEDER_M_D_FEEDER_4", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_FEEDER_5].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_FEEDER", "D_FEEDER_M_D_FEEDER_5", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_FEEDER_6].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_FEEDER", "D_FEEDER_M_D_FEEDER_6", LPCTSTR(mstr_temp), str_file_folder);
////	
////BCR FEEDER
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_1].d_pos[P_BCR_FEEDER_SAFETY]);
	:: WritePrivateProfileString("BCR_FEEDER1", "BCR_FEEDER1_P_BCR_FEEDER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_1].d_pos[P_BCR_FEEDER_PICK]);
	:: WritePrivateProfileString("BCR_FEEDER1", "BCR_FEEDER1_P_BCR_FEEDER_PICK", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_1].d_pos[P_BCR_FEEDER_VISION]);
	:: WritePrivateProfileString("BCR_FEEDER1", "BCR_FEEDER1_P_BCR_FEEDER_VISION", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_1].d_pos[P_BCR_FEEDER_WASTE]);
	:: WritePrivateProfileString("BCR_FEEDER1", "BCR_FEEDER1_P_BCR_FEEDER_WASTE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_1].d_pos[P_BCR_FEEDER_LOAD]);
	:: WritePrivateProfileString("BCR_FEEDER1", "BCR_FEEDER1_P_BCR_FEEDER_LOAD", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_2].d_pos[P_BCR_FEEDER_SAFETY]);
	:: WritePrivateProfileString("BCR_FEEDER2", "BCR_FEEDER2_P_BCR_FEEDER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_2].d_pos[P_BCR_FEEDER_PICK]);
	:: WritePrivateProfileString("BCR_FEEDER2", "BCR_FEEDER2_P_BCR_FEEDER_PICK", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_2].d_pos[P_BCR_FEEDER_VISION]);
	:: WritePrivateProfileString("BCR_FEEDER2", "BCR_FEEDER2_P_BCR_FEEDER_VISION", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_2].d_pos[P_BCR_FEEDER_WASTE]);
	:: WritePrivateProfileString("BCR_FEEDER2", "BCR_FEEDER2_P_BCR_FEEDER_WASTE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_2].d_pos[P_BCR_FEEDER_LOAD]);
	:: WritePrivateProfileString("BCR_FEEDER2", "BCR_FEEDER2_P_BCR_FEEDER_LOAD", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_3].d_pos[P_BCR_FEEDER_SAFETY]);
	:: WritePrivateProfileString("BCR_FEEDER3", "BCR_FEEDER3_P_BCR_FEEDER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_3].d_pos[P_BCR_FEEDER_PICK]);
	:: WritePrivateProfileString("BCR_FEEDER3", "BCR_FEEDER3_P_BCR_FEEDER_PICK", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_3].d_pos[P_BCR_FEEDER_VISION]);
	:: WritePrivateProfileString("BCR_FEEDER3", "BCR_FEEDER3_P_BCR_FEEDER_VISION", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_3].d_pos[P_BCR_FEEDER_WASTE]);
	:: WritePrivateProfileString("BCR_FEEDER3", "BCR_FEEDER3_P_BCR_FEEDER_WASTE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_3].d_pos[P_BCR_FEEDER_LOAD]);
	:: WritePrivateProfileString("BCR_FEEDER3", "BCR_FEEDER3_P_BCR_FEEDER_LOAD", LPCTSTR(mstr_temp), str_file_folder);

	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_4].d_pos[P_BCR_FEEDER_SAFETY]);
	:: WritePrivateProfileString("BCR_FEEDER4", "BCR_FEEDER4_P_BCR_FEEDER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_4].d_pos[P_BCR_FEEDER_PICK]);
	:: WritePrivateProfileString("BCR_FEEDER4", "BCR_FEEDER4_P_BCR_FEEDER_PICK", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_4].d_pos[P_BCR_FEEDER_VISION]);
	:: WritePrivateProfileString("BCR_FEEDER4", "BCR_FEEDER4_P_BCR_FEEDER_VISION", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_4].d_pos[P_BCR_FEEDER_WASTE]);
	:: WritePrivateProfileString("BCR_FEEDER4", "BCR_FEEDER4_P_BCR_FEEDER_WASTE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_FEEDER_4].d_pos[P_BCR_FEEDER_LOAD]);
	:: WritePrivateProfileString("BCR_FEEDER4", "BCR_FEEDER4_P_BCR_FEEDER_LOAD", LPCTSTR(mstr_temp), str_file_folder);
////
////DENSITY VISION
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_F1]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_F1", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_F2]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_F2", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_F3]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_F3", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_F4]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_F4", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_F5]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_F5", LPCTSTR(mstr_temp), str_file_folder);	
	mstr_temp.Format("%0.3f", st_motor[M_D_VISION_X].d_pos[P_DENSITY_F6]);
	:: WritePrivateProfileString("D_VISION_X", "D_VISION_X_P_DENSITY_F6", LPCTSTR(mstr_temp), str_file_folder);
////
////DENSITY ATTACH X
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_ATTACH_X", "D_ATTACH_X_P_XY_DENSITY_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14]);
	:: WritePrivateProfileString("D_ATTACH_X", "D_ATTACH_X_P_XY_DENSITY_F14", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F25]);
	:: WritePrivateProfileString("D_ATTACH_X", "D_ATTACH_X_P_XY_DENSITY_F25", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F36]);
	:: WritePrivateProfileString("D_ATTACH_X", "D_ATTACH_X_P_XY_DENSITY_F36", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY]);
	:: WritePrivateProfileString("D_ATTACH_X", "D_ATTACH_X_P_XY_PLACE_DENSITY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS]);
	:: WritePrivateProfileString("D_ATTACH_X", "D_ATTACH_X_P_XY_ERROR_POS", LPCTSTR(mstr_temp), str_file_folder);
////	
////DENSITY ATTACH Y
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_ATTACH_Y", "D_ATTACH_Y_P_XY_DENSITY_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14]);
	:: WritePrivateProfileString("D_ATTACH_Y", "D_ATTACH_Y_P_XY_DENSITY_F14", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F25]);
	:: WritePrivateProfileString("D_ATTACH_Y", "D_ATTACH_Y_P_XY_DENSITY_F25", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F36]);
	:: WritePrivateProfileString("D_ATTACH_Y", "D_ATTACH_Y_P_XY_DENSITY_F36", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY]);
	:: WritePrivateProfileString("D_ATTACH_Y", "D_ATTACH_Y_P_XY_PLACE_DENSITY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS]);
	:: WritePrivateProfileString("D_ATTACH_Y", "D_ATTACH_Y_P_XY_ERROR_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////DENSITY ATTACH Z1
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_ATTACH_Z1", "D_ATTACH_Z1_P_Z_DENSITY_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14]);
	:: WritePrivateProfileString("D_ATTACH_Z1", "D_ATTACH_Z1_P_Z_DENSITY_F14", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F25]);
	:: WritePrivateProfileString("D_ATTACH_Z1", "D_ATTACH_Z1_P_Z_DENSITY_F25", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F36]);
	:: WritePrivateProfileString("D_ATTACH_Z1", "D_ATTACH_Z1_P_Z_DENSITY_F36", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z1].d_pos[P_Z_PLACE_F]);
	:: WritePrivateProfileString("D_ATTACH_Z1", "D_ATTACH_Z1_P_Z_PLACE_F", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z1].d_pos[P_Z_PLACE_ERR]);
	:: WritePrivateProfileString("D_ATTACH_Z1", "D_ATTACH_Z1_P_Z_PLACE_ERR", LPCTSTR(mstr_temp), str_file_folder);
////
////DENSITY ATTACH Z2
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY]);
	:: WritePrivateProfileString("D_ATTACH_Z2", "D_ATTACH_Z2_P_Z_DENSITY_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14]);
	:: WritePrivateProfileString("D_ATTACH_Z2", "D_ATTACH_Z2_P_Z_DENSITY_F14", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F25]);
	:: WritePrivateProfileString("D_ATTACH_Z2", "D_ATTACH_Z2_P_Z_DENSITY_F25", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F36]);
	:: WritePrivateProfileString("D_ATTACH_Z2", "D_ATTACH_Z2_P_Z_DENSITY_F36", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z2].d_pos[P_Z_PLACE_F]);
	:: WritePrivateProfileString("D_ATTACH_Z2", "D_ATTACH_Z2_P_Z_PLACE_F", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_D_ATTACH_Z2].d_pos[P_Z_PLACE_ERR]);
	:: WritePrivateProfileString("D_ATTACH_Z2", "D_ATTACH_Z2_P_Z_PLACE_ERR", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR VISION
	mstr_temp.Format("%0.3f", st_motor[M_BCR_VISION_X].d_pos[P_BCR_FEEDER_SAFETY]);
	:: WritePrivateProfileString("BCR_VISION_X", "BCR_VISION_X_P_BCR_FEEDER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_VISION_X].d_pos[P_BCR_FEEDER_PICK]);
	:: WritePrivateProfileString("BCR_VISION_X", "BCR_VISION_X_P_BCR_FEEDER_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_VISION_X].d_pos[P_BCR_FEEDER_VISION]);
	:: WritePrivateProfileString("BCR_VISION_X", "BCR_VISION_X_P_BCR_FEEDER_VISION", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_VISION_X].d_pos[P_BCR_FEEDER_WASTE]);
	:: WritePrivateProfileString("BCR_VISION_X", "BCR_VISION_X_P_BCR_FEEDER_WASTE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_VISION_X].d_pos[P_BCR_FEEDER_LOAD]);
	:: WritePrivateProfileString("BCR_VISION_X", "BCR_VISION_X_P_BCR_FEEDER_LOAD", LPCTSTR(mstr_temp), str_file_folder);
////	
////BCR REJECT X
	mstr_temp.Format("%0.3f", st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_SAFETY]);
	:: WritePrivateProfileString("BCR_REJECT_X", "BCR_REJECT_X_P_REJ_BARCODE_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F1]);
	:: WritePrivateProfileString("BCR_REJECT_X", "BCR_REJECT_X_P_REJ_BARCODE_F1", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F2]);
	:: WritePrivateProfileString("BCR_REJECT_X", "BCR_REJECT_X_P_REJ_BARCODE_F2", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F3]);
	:: WritePrivateProfileString("BCR_REJECT_X", "BCR_REJECT_X_P_REJ_BARCODE_F3", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F4]);
	:: WritePrivateProfileString("BCR_REJECT_X", "BCR_REJECT_X_P_REJ_BARCODE_F4", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR Attach 1X
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_SAFETY]);
	:: WritePrivateProfileString("BCR_ATTACH1_X", "BCR_ATTACH1_X_P_X_BCR_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK]);
	:: WritePrivateProfileString("BCR_ATTACH1_X", "BCR_ATTACH1_X_P_X_BCR_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_DOWN]);
	:: WritePrivateProfileString("BCR_ATTACH1_X", "BCR_ATTACH1_X_P_X_BCR_DOWN", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE]);
	:: WritePrivateProfileString("BCR_ATTACH1_X", "BCR_ATTACH1_X_P_X_BCR_PLACE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT]);
	:: WritePrivateProfileString("BCR_ATTACH1_X", "BCR_ATTACH1_X_P_X_BCR_REJECT", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR Attach 1 Z1
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z1", "BCR_ATTACH1_Z1_P_Z_BCR_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z1].d_pos[P_X_BCR_PICK]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z1", "BCR_ATTACH1_Z1_P_X_BCR_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z1].d_pos[P_X_BCR_DOWN]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z1", "BCR_ATTACH1_Z1_P_X_BCR_DOWN", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z1].d_pos[P_X_BCR_PLACE]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z1", "BCR_ATTACH1_Z1_P_X_BCR_PLACE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z1].d_pos[P_X_BCR_REJECT]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z1", "BCR_ATTACH1_Z1_P_X_BCR_REJECT", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR Attach 1 Z2
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z2", "BCR_ATTACH1_Z2_P_Z_BCR_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z2].d_pos[P_X_BCR_PICK]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z2", "BCR_ATTACH1_Z2_P_X_BCR_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z2].d_pos[P_X_BCR_DOWN]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z2", "BCR_ATTACH1_Z2_P_X_BCR_DOWN", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z2].d_pos[P_X_BCR_PLACE]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z2", "BCR_ATTACH1_Z2_P_X_BCR_PLACE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH1_Z2].d_pos[P_X_BCR_REJECT]);
	:: WritePrivateProfileString("BCR_ATTACH1_Z2", "BCR_ATTACH1_Z2_P_X_BCR_REJECT", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR Attach 2X
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_SAFETY]);
	:: WritePrivateProfileString("BCR_ATTACH2_X", "BCR_ATTACH2_X_P_X_BCR_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK]);
	:: WritePrivateProfileString("BCR_ATTACH2_X", "BCR_ATTACH2_X_P_X_BCR_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_DOWN]);
	:: WritePrivateProfileString("BCR_ATTACH2_X", "BCR_ATTACH2_X_P_X_BCR_DOWN", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE]);
	:: WritePrivateProfileString("BCR_ATTACH2_X", "BCR_ATTACH2_X_P_X_BCR_PLACE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT]);
	:: WritePrivateProfileString("BCR_ATTACH2_X", "BCR_ATTACH2_X_P_X_BCR_REJECT", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR Attach 2 Z1
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z1", "BCR_ATTACH2_Z1_P_Z_BCR_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z1].d_pos[P_X_BCR_PICK]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z1", "BCR_ATTACH2_Z1_P_X_BCR_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z1].d_pos[P_X_BCR_DOWN]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z1", "BCR_ATTACH2_Z1_P_X_BCR_DOWN", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z1].d_pos[P_X_BCR_PLACE]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z1", "BCR_ATTACH2_Z1_P_X_BCR_PLACE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z1].d_pos[P_X_BCR_REJECT]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z1", "BCR_ATTACH2_Z1_P_X_BCR_REJECT", LPCTSTR(mstr_temp), str_file_folder);
////
////BCR Attach 2 Z2
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z2", "BCR_ATTACH2_Z2_P_Z_BCR_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z2].d_pos[P_X_BCR_PICK]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z2", "BCR_ATTACH2_Z2_P_X_BCR_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z2].d_pos[P_X_BCR_DOWN]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z2", "BCR_ATTACH2_Z2_P_X_BCR_DOWN", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z2].d_pos[P_X_BCR_PLACE]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z2", "BCR_ATTACH2_Z2_P_X_BCR_PLACE", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BCR_ATTACH2_Z2].d_pos[P_X_BCR_REJECT]);
	:: WritePrivateProfileString("BCR_ATTACH2_Z2", "BCR_ATTACH2_Z2_P_X_BCR_REJECT", LPCTSTR(mstr_temp), str_file_folder);
////
////TOP VISION X
	mstr_temp.Format("%0.3f", st_motor[M_TOP_VISION_X].d_pos[P_VIS_SAFETY]);
	:: WritePrivateProfileString("TOP_VISION_X", "TOP_VISION_X_P_VIS_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOP_VISION_X].d_pos[P_VIS1_POS]);
	:: WritePrivateProfileString("TOP_VISION_X", "TOP_VISION_X_P_VIS1_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOP_VISION_X].d_pos[P_VIS2_POS]);
	:: WritePrivateProfileString("TOP_VISION_X", "TOP_VISION_X_P_VIS2_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////BTM VISION X
	mstr_temp.Format("%0.3f", st_motor[M_BTM_VISION_X].d_pos[P_VIS_SAFETY]);
	:: WritePrivateProfileString("BTM_VISION_X", "BTM_VISION_X_P_VIS_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BTM_VISION_X].d_pos[P_VIS1_POS]);
	:: WritePrivateProfileString("BTM_VISION_X", "BTM_VISION_X_P_VIS1_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BTM_VISION_X].d_pos[P_VIS2_POS]);
	:: WritePrivateProfileString("BTM_VISION_X", "BTM_VISION_X_P_VIS2_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////ULD Y
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Y].d_pos[P_ULD_Y_SAFETY]);
	:: WritePrivateProfileString("ULD_Y", "ULD_Y_P_ULD_Y_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS]);
	:: WritePrivateProfileString("ULD_Y", "ULD_Y_P_ULD_Y_PICK_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS]);
	:: WritePrivateProfileString("ULD_Y", "ULD_Y_P_ULD_Y_VIS_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS]);
	:: WritePrivateProfileString("ULD_Y", "ULD_Y_P_ULD_Y_CONV_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS]);
	:: WritePrivateProfileString("ULD_Y", "ULD_Y_P_ULD_Y_REJ_POS", LPCTSTR(mstr_temp), str_file_folder);
//// 
////ULD Z
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_SAFETY]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_UP", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_POS]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_PICK_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_VIS_POS]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_VIS_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_CONV_POS]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_CONV_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_REJ_POS]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_REJ_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_READY_POS]);
	:: WritePrivateProfileString("ULD_Z", "ULD_Z_P_ULD_Z_PICK_READY_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////ULD CV PITCH
	mstr_temp.Format("%0.3f", st_motor[M_ULD_CV_PITCH].d_pos[P_CONV_PITCH_SAFETY]);
	:: WritePrivateProfileString("ULD_CV_PITCH", "ULD_CV_PITCH_P_CONV_PITCH_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_CV_PITCH].d_pos[P_CONV_PITCH_MOVE]);
	:: WritePrivateProfileString("ULD_CV_PITCH", "ULD_CV_PITCH_P_CONV_PITCH_MOVE", LPCTSTR(mstr_temp), str_file_folder);
////
////ULD STACKER Z
	mstr_temp.Format("%0.3f", st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_SAFETY]);
	:: WritePrivateProfileString("ULD_STACKER_Z", "ULD_STACKER_Z_P_CONV_Z_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK_UP]);
	:: WritePrivateProfileString("ULD_STACKER_Z", "ULD_STACKER_Z_P_CONV_Z_PICK_UP", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK]);
	:: WritePrivateProfileString("ULD_STACKER_Z", "ULD_STACKER_Z_P_CONV_Z_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK_DOWN]);
	:: WritePrivateProfileString("ULD_STACKER_Z", "ULD_STACKER_Z_P_CONV_Z_PICK_DOWN", LPCTSTR(mstr_temp), str_file_folder);
////
////IDBUFFER EL
	mstr_temp.Format("%0.3f", st_motor[M_IDBUFFER_EL].d_pos[P_BUFFER_SAFETY]);
	:: WritePrivateProfileString("IDBUFFER_EL", "IDBUFFER_EL_P_BUFFER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER]);
	:: WritePrivateProfileString("IDBUFFER_EL", "IDBUFFER_EL_P_UPBUFFER", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_IDBUFFER_EL].d_pos[P_DOWNBUFFER]);
	:: WritePrivateProfileString("IDBUFFER_EL", "IDBUFFER_EL_P_DOWNBUFFER", LPCTSTR(mstr_temp), str_file_folder);
////
////TOPID LD1
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_SAFETY]);
	:: WritePrivateProfileString("TOPID_LD1", "TOPID_LD1_P_LDBUFFER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_PICK]);
	:: WritePrivateProfileString("TOPID_LD1", "TOPID_LD1_P_LDBUFFER_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_LAOD]);
	:: WritePrivateProfileString("TOPID_LD1", "TOPID_LD1_P_LDBUFFER_LAOD", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_BCR1]);
	:: WritePrivateProfileString("TOPID_LD1", "TOPID_LD1_P_LDBUFFER_BCR1", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_BCR2]);
	:: WritePrivateProfileString("TOPID_LD1", "TOPID_LD1_P_LDBUFFER_BCR2", LPCTSTR(mstr_temp), str_file_folder);
////
////TOPID LD2
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_SAFETY]);
	:: WritePrivateProfileString("TOPID_LD2", "TOPID_LD2_P_LDBUFFER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK]);
	:: WritePrivateProfileString("TOPID_LD2", "TOPID_LD2_P_LDBUFFER_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD]);
	:: WritePrivateProfileString("TOPID_LD2", "TOPID_LD2_P_LDBUFFER_LAOD", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_BCR1]);
	:: WritePrivateProfileString("TOPID_LD2", "TOPID_LD2_P_LDBUFFER_BCR1", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_BCR2]);
	:: WritePrivateProfileString("TOPID_LD2", "TOPID_LD2_P_LDBUFFER_BCR2", LPCTSTR(mstr_temp), str_file_folder);
////
////BTMID ULD2
	mstr_temp.Format("%0.3f", st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY]);
	:: WritePrivateProfileString("BTMID_ULD2", "BTMID_ULD2_P_ULDBUFFER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_PICK]);
	:: WritePrivateProfileString("BTMID_ULD2", "BTMID_ULD2_P_ULDBUFFER_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_ULD]);
	:: WritePrivateProfileString("BTMID_ULD2", "BTMID_ULD2_P_ULDBUFFER_ULD", LPCTSTR(mstr_temp), str_file_folder);
////
////BTMID ULD1
	mstr_temp.Format("%0.3f", st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_SAFETY]);
	:: WritePrivateProfileString("BTMID_ULD1", "BTMID_ULD1_P_ULDBUFFER_SAFETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_PICK]);
	:: WritePrivateProfileString("BTMID_ULD1", "BTMID_ULD1_P_ULDBUFFER_PICK", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_ULD]);
	:: WritePrivateProfileString("BTMID_ULD1", "BTMID_ULD1_P_ULDBUFFER_ULD", LPCTSTR(mstr_temp), str_file_folder);
////
////INDEX X
	mstr_temp.Format("%0.3f", st_motor[M_INDEX_X].d_pos[P_X_ROLLER_SAGETY]);
	:: WritePrivateProfileString("INDEX_X", "INDEX_X_P_X_ROLLER_SAGETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS]);
	:: WritePrivateProfileString("INDEX_X", "INDEX_X_P_X_ROLLER_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////INDEX ROLLER1
	mstr_temp.Format("%0.3f", st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY]);
	:: WritePrivateProfileString("INDEX_ROLLER1", "INDEX_ROLLER1_P_ROLLER_SAGETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_POS]);
	:: WritePrivateProfileString("INDEX_ROLLER1", "INDEX_ROLLER1_P_ROLLER_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////INDEX ROLLER2
	mstr_temp.Format("%0.3f", st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_SAGETY]);
	:: WritePrivateProfileString("INDEX_ROLLER2", "INDEX_ROLLER2_P_ROLLER_SAGETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_POS]);
	:: WritePrivateProfileString("INDEX_ROLLER2", "INDEX_ROLLER2_P_ROLLER_POS", LPCTSTR(mstr_temp), str_file_folder);
////
////ULD ROLLER PITCH
	mstr_temp.Format("%0.3f", st_motor[M_ULD_ROLLER_PITCH].d_pos[P_GUIDE_SAGETY]);
	:: WritePrivateProfileString("ULD_ROLLER_PITCH", "ULD_ROLLER_PITCH_P_GUIDE_SAGETY", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_ROLLER_PITCH].d_pos[P_GUIDE_LOAD_POS]);
	:: WritePrivateProfileString("ULD_ROLLER_PITCH", "ULD_ROLLER_PITCH_P_GUIDE_LOAD_POS", LPCTSTR(mstr_temp), str_file_folder);
	mstr_temp.Format("%0.3f", st_motor[M_ULD_ROLLER_PITCH].d_pos[P_GUIDE_UNLOAD_POS]);
	:: WritePrivateProfileString("ULD_ROLLER_PITCH", "ULD_ROLLER_PITCH_P_GUIDE_UNLOAD_POS", LPCTSTR(mstr_temp), str_file_folder);
////	

// 	//2014,1205
// 	for ( int i = 0; i < TSITE_SOCKET_CNT; i++)
// 	{
// 		mstr_temp.Format("SOCKET_%d", i+1);
// 		str_temp_onoff = "OFF";
// 		if( g_site.GetEnableSocket(i) ) str_temp_onoff = "ON";
// 		:: WritePrivateProfileString("Mode", LPCTSTR(mstr_temp), LPCTSTR(str_temp_onoff), str_file_folder);
// 	}
	
	
	//2015.0413
	if( st_basic.mn_mode_xgem == CTL_YES)
	{
		g_xgem_client.SetPPID();
		g_xgem_client.XgemSendPPID();
	}

// 	//2014,0623
// 	if(g_client_ec.GetToolInform() == false)
// 		g_client_ec.OnSendToolInform(true);
	
}
////
void CPublic_Function::LoadRegLabelCapaData()
{
	int nTotal = 0;
	FILE    *fp ;
	int     existence;
	char chr_data[100];
	
	CString str_LoadFile = "C:\\AMT8590\\Setting\\DensityCapa.ini";
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
			}
			st_handler.m_nTotal = 0;
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
		}
		st_handler.m_nTotal = 0;
		return;
	}
	:: GetPrivateProfileString("Label_CapaData", "Total", "0", chr_data, 100, str_LoadFile);
	nTotal = atoi(chr_data);
	
	st_handler.m_nTotal = nTotal;
	
	CString str="",strLabel="";
	for (int i = 0; i < nTotal; i++)
	{
		str.Format("%d",i+1);
		:: GetPrivateProfileString("Label_CapaData", str, "0", chr_data, 100, str_LoadFile);
		strLabel = chr_data;
		strLabel.TrimLeft(' ');               
		strLabel.TrimRight(' ');
		st_handler.m_strLabelCapa[i] = strLabel;
	}
	fclose(fp);
}

int CPublic_Function::LoadRegModelData()
{
	int nFuncRet = RET_ERROR;
	int nTotal = 0;
	FILE    *fp ;
	int     existence;
	char chr_data[100];
	
	CString str_LoadFile = "C:\\AMT8590\\Setting\\ModelName.ini";
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
			}
			st_work.m_nMdlTotal = 0;
			return nFuncRet;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
		}
		st_work.m_nMdlTotal = 0;
		return nFuncRet;
	}
	:: GetPrivateProfileString("Model_Name", "Total", "0", chr_data, 100, str_LoadFile);
	nTotal = atoi(chr_data);
	
	st_work.m_nMdlTotal = nTotal;
	
	CString str,strLabel;
	for (int i = 0; i < nTotal; i++)
	{
		str.Format("%d",i+1);
		:: GetPrivateProfileString("Model_Name", str, "0", chr_data, 100, str_LoadFile);
		strLabel = chr_data;
		strLabel.TrimLeft(' ');               
		strLabel.TrimRight(' ');
		st_work.m_strModelName[i] = strLabel;
	}
	fclose(fp);

	nFuncRet = RET_GOOD;

	return nFuncRet;
}

int CPublic_Function::CheckLotSerial()
{
	int nFunRet=CTL_GOOD;
	int Len[2], AccLen[2], TKLen[2];
	CString Temp[2], AccTemp[2], TKTemp[2];;
	
	if( st_handler.n_without_network == CTL_YES || 
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[CTL_RIGHT] == CTL_YES ) ||
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] == 0 && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] == 0 ) )
		return nFunRet;
	
	for(int i =0; i<2; i++)
	{
		Temp[i].Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[i]);
		Temp[i].TrimRight();
		Temp[i].TrimRight();
		Len[i] = Temp[i].GetLength();
		AccTemp[i].Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_acce_code[i]);
		AccTemp[i].TrimRight();
		AccTemp[i].TrimRight();
		AccLen[i] = AccTemp[i].GetLength();
		TKTemp[i].Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_tbox_code[i]);
		TKTemp[i].TrimRight();
		TKTemp[i].TrimRight();
		TKLen[i] = TKTemp[i].GetLength();
		
		if (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[i] == 1 )
		{
			if( Temp[i].Find("S",0) == 0 && Len[i] >10 )
			{			
			}
			else/* if (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[i] == 1 )*/
			{
				if (Temp[i].Find("S",0) != 0 || Len[i] <10 )
				{
// 					960000 0 00 "Input Left serial is not correct"
// 					960001 0 00 "Input Right serial is not correct"
					if( i == 0)
						CTL_Lib.Alarm_Error_Occurrence(7970, CTL_dWARNING, "960000" );
// 						AfxMessageBox("[datainfo_shift_error[Left]] - Serial is not correct");
					else
						CTL_Lib.Alarm_Error_Occurrence(7971, CTL_dWARNING, "960000" );
// 						AfxMessageBox("[datainfo_shift_error[Right]] - Serial is not correct");
					
					if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹U E­¸e A¸Ac
					{						
						sprintf(st_msg.c_abnormal_msg, "[ERROR] - Serial is not correct");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						
					}
					nFunRet = CTL_ERROR;
				}
			}

			if( AccTemp[i].Find("S",0) == 0 && AccLen[i] >10 )
			{			
			}
			else/* if (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[i] == 1 )*/
			{
				if (AccTemp[i].Find("L",0) != 0 || AccLen[i] <10 )
				{
// 					960002 0 00 "Input Left Accy Tray Code is not correct"
// 					960003 0 00 "Input Right Accy Tray Code is not correct"
					if( i == 0)
						CTL_Lib.Alarm_Error_Occurrence(7972, CTL_dWARNING, "960002" );
// 						AfxMessageBox("[datainfo_shift_error[Left]] - Accy Tray Code is not correct");
					else
						CTL_Lib.Alarm_Error_Occurrence(7973, CTL_dWARNING, "960003" );
// 						AfxMessageBox("[datainfo_shift_error[Right]] - Accy Tray Code is not correct");
					
					if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹U E­¸e A¸Ac
					{
						
						sprintf(st_msg.c_abnormal_msg, "[ERROR] - Accy Tray Code is not correct");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						
					}
					nFunRet = CTL_ERROR;
				}
			}

			if( TKTemp[i].Find("S",0) == 0 && TKLen[i] >10 )
			{			
			}
			else/* if (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[i] == 1 )*/
			{
				if (TKTemp[i].Find("L",0) != 0 || TKLen[i] <10 )
				{
// 					960004 0 00 "Input Left TK Code is not correct"
// 					960005 0 00 "Input Right TK Code is not correct"
					if( i == 0)
						CTL_Lib.Alarm_Error_Occurrence(7974, CTL_dWARNING, "960004" );
// 						AfxMessageBox("[datainfo_shift_error[Left]] - TK Code is not correct");
					else
						CTL_Lib.Alarm_Error_Occurrence(7975, CTL_dWARNING, "960005" );
// 						AfxMessageBox("[datainfo_shift_error[Right]] - TK Code is not correct");
					
					if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹U E­¸e A¸Ac
					{						
						sprintf(st_msg.c_abnormal_msg, "[ERROR] - TK Code Code is not correct");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);						
					}
					nFunRet = CTL_ERROR;
				}	
			}
		}
	}
	
	return nFunRet;
}

int CPublic_Function::GetModelNum(CString strPartName)
{
	int nFuncRet = -1;
	
	if( strPartName.GetAt(2) == 'M' )//mSATA
	{
		nFuncRet = DT_MSATA;
	}
	else if( strPartName.GetAt(2) == 'N' )//M.2
	{
		nFuncRet = DT_M2;
	}
	else if( strPartName.GetAt(3) == 'K' || strPartName.GetAt(3) == 'L')//2.5 PRO,EVO
	{
		if( strPartName.GetAt(3) == 'K' && strPartName.GetAt(2) == '7')//2.5'7mmT
		{
			nFuncRet = DT_25PRO;
		}
		else if( strPartName.GetAt(3) == 'L' && strPartName.GetAt(2) == '7')//2.5'7mmT
		{
			nFuncRet = DT_25EVO;
		}
	}
	else if( strPartName.Mid(16,2) == "DW" )//LAPTOP
	{		
		nFuncRet = DT_LAPTOP;
	}
	
	return nFuncRet;
}

CString CPublic_Function::GetModelName( int nNum)
{
	CString strRetMdl = "NONE";
	if	   (nNum == 0) strRetMdl = "25EVO";
	else if(nNum == 1) strRetMdl = "25PRO";
	else if(nNum == 2) strRetMdl = "MSATA";
	else if(nNum == 3) strRetMdl = "M2";
	else if(nNum == 4) strRetMdl = "LAPTOP";

	return strRetMdl;
}

int CPublic_Function::GetModelNumFromModel( CString strModel)
{
	int nFuncRet = -1;
	int nRet = RET_ERROR;
	nRet = LoadRegModelData();
	if(nRet == RET_GOOD)
	{
		for ( int i = 0; i < st_work.m_nMdlTotal; i++ )
		{
			if( st_work.m_strModelName[i].Compare( (LPCTSTR) strModel) == 0 )
			{
				nFuncRet = i;
				break;
			}
		}
	}
	return nFuncRet;

}


int CPublic_Function::GetModelFromPartID( CString PartID, CString& strModel)
{
	int nFuncRet = RET_ERROR, nRet = 0, nMdl = 0;

	PartID.TrimLeft(' ');	
	PartID.TrimRight(' ');

	nRet = LoadRegModelData();
	if( nRet == RET_GOOD)
	{
		nItemLength = 0;
		nMdl = GetModelNumFromModel(st_work.m_strCurModel);
		nRet = ModelFileload(st_work.m_strCurModel);
		if( nMdl>=0 && nRet == RET_GOOD && nItemLength > 0) 
		{
			for (int ii = 0; ii < nItemLength; ii++)
			{
				m_strModel[nMdl][ii] = m_strItemValue[ii];
				if(m_strItemValue[ii].Compare( (LPCTSTR)PartID) == 0)
				{
					strModel = st_work.m_strCurModel;
					nFuncRet = RET_GOOD;
					if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
					{
						sprintf(st_msg.c_normal_msg, "%s Model is same", st_work.m_strCurModel);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
					}

					break;
				}
			}
		}

		if(nFuncRet != RET_GOOD)
		{
			for (int i= 0; i < st_work.m_nMdlTotal; i++ )
			{
				nItemLength = 0;
				nRet = ModelFileload(st_work.m_strModelName[i]);
				if( nRet == RET_GOOD && nItemLength > 0) 
				{
					for (int ii = 0; ii < nItemLength; ii++)
					{
						m_strModel[i][ii] = m_strItemValue[ii];
						if(m_strItemValue[ii].Compare( (LPCTSTR)PartID) == 0)
						{
							strModel = st_work.m_strModelName[i];
							nFuncRet = RET_GOOD;
							if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
							{
								sprintf(st_msg.c_normal_msg, "%s Model is different", strModel);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆÐ Ãâ·Â ¿äÃ»
							}
							break;
						}
					}
				}
			}
		}	
	}
	return nFuncRet;
}


int CPublic_Function::ModelFileload(CString strMdl)
{
	int nFunRet = RET_ERROR;
	CString strFilePath="";
	strFilePath = _T("C:\\AMT8590\\Setting\\ModelName\\");
	strMdl.TrimLeft(); strMdl.TrimRight();
	CString strFile = strMdl + _T(".TXT");
	//FileRead(strFilePath + strFile);
	
	int nTotal = 0;
	FILE    *fp ;
	int     existence;
	char chr_data[100];	
	
	strFilePath += strFile;
	
	existence = access(strFilePath, 0);
	
	if (!existence)
	{
		if ((fp=fopen(strFilePath,"rb")) == NULL)
		{
			return nFunRet;
		}
	}
	else
	{
		//m_nMdlTotal = 0;
		return nFunRet;
	}
	
	for(int i = 0; i < MAX_LINE; i++)  
	{
		m_strItemValue[i].Empty();
	}
	
	
	:: GetPrivateProfileString(strMdl, "Total", "0", chr_data, 100, strFilePath);
	nTotal = atoi(chr_data);
	
	nItemLength = nTotal;
	
	CString str,strLabel;
	for (i = 0; i < nTotal; i++)
	{
		str.Format("%d",i+1);
		:: GetPrivateProfileString(strMdl, str, "0", chr_data, 100, strFilePath);
		strLabel = chr_data;
		strLabel.TrimLeft(); strLabel.TrimRight();
		m_strItemValue[i] = strLabel;
	}
	fclose(fp);
	nFunRet = RET_GOOD;

	return nFunRet;
}

void CPublic_Function::RunRegPartID()
{
	CWnd* m_pWnd = CWnd::FindWindow(NULL,"RegModel");
	if(m_pWnd){
		m_pWnd->SetForegroundWindow(); 	
		m_pWnd->ShowWindow(SW_RESTORE);	
	}
	else{
		::ShellExecute(NULL, NULL, "C:\\AMT8590\\bin\\RegModel.exe", NULL, NULL, SW_SHOWNORMAL);
	}
}

void CPublic_Function::SendRegPartID() 
{	
	COPYDATASTRUCT cds;
	
	st_copydata = new st_copy_data;
	
	st_copydata->mn_mdl = 19;
	
	memset(st_copydata->str_partid, 0x00, sizeof(st_copydata->str_partid));
	sprintf(st_copydata->str_partid,"%s", (LPCTSTR)g_lotMgr.GetLotAt(0).GetPartID());
	
	cds.dwData = WM_MDL_PART_DATA;
	cds.cbData = sizeof(st_copy_data);/*(sizeof(st_copy_data))*///(DWORD)&st_copydata;
	cds.lpData = st_copydata;
	
	
	HWND hwnd = ::FindWindow(NULL,"RegModel");//"ExText");//"RegModel");
	if (hwnd!= NULL)
	{
		HANDLE handle = AfxGetInstanceHandle();
		::SendMessage(hwnd, WM_COPYDATA, (WPARAM)handle, (LPARAM)&cds);
	}
	else
	{
		CDialog_Message msg_dlg;
		st_msg.str_fallacy_msg = _T("RegModel.exe can't find program(C:\\AMT8590\\BIN\\RegModel.exe");
		
		msg_dlg.DoModal();
	}
	
	delete st_copydata;	
}

