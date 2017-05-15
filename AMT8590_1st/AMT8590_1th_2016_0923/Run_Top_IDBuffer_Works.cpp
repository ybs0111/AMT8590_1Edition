// Run_Top_IDBuffer_Works.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Top_IDBuffer_Works.h"
#include "Run_IDBuffer_El.h"
#include "AMTLotManager.h"

#include "InterfaceBarcode.h"
#include "SrcPart/APartTestSite.h"

#include "Run_Barcode_Feeders.h"
#include "Run_Barcode_Works.h"
#include "FrontServer.h"
#include "Public_Function.h"
#include "SrcPart/APartTray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Top_IDBuffer_Works

IMPLEMENT_SERIAL(CRun_Top_IDBuffer_Works, CObject, 1);	// 복구 동작을 위함...

CRun_Top_IDBuffer_Works g_Run_Top_IDBuffers;

CRun_Top_IDBuffer_Works::CRun_Top_IDBuffer_Works()
{
	Thread_Variable_Initial();
}

CRun_Top_IDBuffer_Works::~CRun_Top_IDBuffer_Works()
{
}



/////////////////////////////////////////////////////////////////////////////
// CRun_Top_IDBuffer_Works message handlers
void CRun_Top_IDBuffer_Works::Thread_Variable_Initial()
{
	int i=0,j=0,k=0;
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;

	for( i=0; i<HOOKCLAMP_CNT; i++ )
	{
		m_fbHookClamp[i] = FB_NONE;
		m_step_hookclamp[i] = ESTEP_CYL_START;
		st_work.mn_site_hookclamp[i] = IO_OFF;
		st_work.mn_site_hookunclamp[i] = IO_OFF;
	}

	for ( i=0; i < 2; i++)
	{
		m_fbHookhwdbwd[i] = FB_NONE;
		m_step_hookfwdbwd[i] = ESTEP_CYL_START;
		st_work.mn_site_hookfwd[i] = IO_OFF;
		st_work.mn_site_hookbwd[i] = IO_OFF;
	}

	for (i=0; i < LDCLAMP_CNT; i++)
	{
		m_step_ldclamp[i] = ESTEP_CYL_START;
		m_fbLdClamp[i] = FB_NONE;
		st_work.mn_site_ldclamp[i] = IO_OFF;
		st_work.mn_site_ldunclamp[i] = IO_OFF;
	}

	for (i=0; i<2; i++){
		for (j=0;j<4;j++){
			for (k=0;k<2;k++){
				st_sync.mn_ibuffer_in[i][k] = CTL_NO;				
				st_sync.mn_top_ibuffer_work[i][j][k] = CTL_NO;
			}			
		}		
	}
}

void CRun_Top_IDBuffer_Works::Thread_Run()
{

// return;
	if( Func.OnIsInterruptCondition() != 1 )
		return;
	switch(COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;

	case dRUN:
// 		return;
		Run_Move();
		break;

	case dRESTORE:
		Run_Restore();
		break;

	default:
		break;
	
	}

	int i=0;
	for ( i = 0; i < HOOKCLAMP_CNT; i++){
		Run_Move_HookClamp(i);
	}
	for ( i = 0; i < 2; i++){
		Run_Move_HookFwdBwd(i);
	}
	for ( i = 0; i < 4; i++ ){
		Run_Move_LdClamp(i);
	}
}


void CRun_Top_IDBuffer_Works::Run_Initial()
{
	int nRet = 0;
	int i = 0;

	if( st_handler.mn_init_state[INIT_TOP_IDBUFFER] != CTL_NO )	return;
	switch(mn_InitStep)
	{
	case ETRI_START: mn_InitStep = ETRI_CHECK_ROBOT_CHK;	break;

	case ETRI_CHECK_ROBOT_CHK:	ETRI_Check_Robot();		break;

	case ETRI_IDBUFFER_EMPTY_CHK: ETRI_IDBuffer_Empty_Chk(); break;

	case ETRI_IDBUFFER_HOOK_LDCLAMP:
		ETRI_IDBuffer_HOOK_LdClamp();
		break;

	case ETRI_IDBUFFER_HOOK_LDCLAMP_CHK: 
		ETRI_IDBuffer_HOOK_LdClamp_Chk();
		break;

	case ETRI_IDBUFFER_LDFWDBWD: 
		ETRI_IDBuffer_FwdBwd();
		mn_InitStep = ETRI_IDBUFFER_LDFWDBWD_CHK;
		break;

	case ETRI_IDBUFFER_LDFWDBWD_CHK:
		ETRI_IDBuffer_FwdBwd_Chk();
		break;

	case ETRI_HOMECHK_TOPID_LD1:
		ETRI_HomeChk_LD1();
		break;

	case ETRI_MONE_SAFETY_LOAD1:
		ETRI_Move_Safety_Load1();
		break;

	case ETRI_HOMECHK_TOPID_LD2:
		ETRI_HomeChk_LD2();
		break;

	case ETRI_MONE_SAFETY_LOAD2:
		ETRI_Move_Safety_Load2();
		break;

	case ETRI_IDBUFFER_LDUNCLAMP:
		ETRI_IDBuffer_LdUnClamp();
		break;

	case ETRI_IDBUFFER_LDUNCLAMP_CHK:
		ETRI_IDBuffer_LdUnClamp_Chk();
		break;

	case ETRI_IDBUFFER_LDCLAMP:
		ETRI_IDBuffer_LdClamp();
		break;

	
	case ETRI_IDBUFFER_LDCLAMP_CHK:
		ETRI_IDBuffer_LdClamp_Chk();
		break;

	case ETRI_IDBUFFER_LDCLAMP_CHK_AGAIN:
		ETRI_IDBuffer_LdClamp_Chk_Again();
		break;

	case ETRI_FINISH:
		ETRI_Finish();
		break;
		

	}
}

void CRun_Top_IDBuffer_Works::ETRI_Check_Robot()
{
	if( st_handler.mn_init_state[INIT_DENDITY_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_ROLLER_ROBOT] == CTL_NO ) return;
//	if( st_handler.mn_init_state[INIT_IDBUFFER_EL] != CTL_NO ) return;

	mn_InitStep = ETRI_IDBUFFER_EMPTY_CHK;
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_Empty_Chk()
{
	int nRet1 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON );
	int nRet2 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_idbuffer2_chk, IO_ON );

	int nRet3 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON );
	int nRet4 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_idbuffer2_chk, IO_ON );

	if(nRet1 == IO_OFF && nRet2 == IO_OFF)
	{
	}
	else if( (nRet1 ^ nRet2) == IO_ON)
	{
	}

	mn_InitStep = ETRI_IDBUFFER_HOOK_LDCLAMP;

}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_HOOK_LdClamp()
{
	ReqHookClamp(EHS_HS_LD1, BACKWARD);
	ReqHookClamp(EHS_HS_LD2_LEFT, BACKWARD);
	ReqHookClamp(EHS_HS_LD2_RIGHT, BACKWARD);
	//mn_InitStep = ETRI_IDBUFFER_LDCLAMP_CHK;
	mn_InitStep = ETRI_IDBUFFER_HOOK_LDCLAMP_CHK;//2014.1016
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_FwdBwd()
{
	ReqHookFwdBwd(EHS_HFB_LD1, BACKWARD);
	ReqHookFwdBwd(EHS_HFB_LD2, BACKWARD);
	mn_InitStep = ETRI_IDBUFFER_LDFWDBWD_CHK;
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_LdUnClamp()
{
	ReqLdClamp(EHS_LCS_LD12, BACKWARD);
	ReqLdClamp(EHS_LCS_LD34, BACKWARD);
	ReqLdClamp(EHS_LCS_LD56, BACKWARD);
	ReqLdClamp(EHS_LCS_LD78, BACKWARD);

	mn_InitStep = ETRI_IDBUFFER_LDUNCLAMP_CHK;
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_LdUnClamp_Chk()
{
	if(GetLdClamp(EHS_LCS_LD12) != FB_NONE) return;
	if(GetLdClamp(EHS_LCS_LD34) != FB_NONE) return;
	if(GetLdClamp(EHS_LCS_LD56) != FB_NONE) return;
	if(GetLdClamp(EHS_LCS_LD78) != FB_NONE) return;

	//mn_InitStep = ETRI_IDBUFFER_LDCLAMP;
	mn_InitStep = ETRI_IDBUFFER_LDCLAMP;//2014.1016
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_LdClamp()
{
	ReqLdClamp(EHS_LCS_LD12, FORWARD);
	ReqLdClamp(EHS_LCS_LD34, FORWARD);
	ReqLdClamp(EHS_LCS_LD56, FORWARD);
	ReqLdClamp(EHS_LCS_LD78, FORWARD);

	mn_InitStep = ETRI_IDBUFFER_LDCLAMP_CHK;
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_LdClamp_Chk()
{
	bool bUnclap = false;
	if(GetLdClamp(EHS_LCS_LD12) == FB_NONE)
	{
		if( CheckLdClamp(EHS_LCS_LD12, IO_ON) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD12][0]) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD12][1]) == IO_ON )
			{
				ATray tray;
				tray.SetPos( IDPOS_DENSITY );
				for( int i=0; i<MAX_MODULE; i++ )
				{
					CString strProductID = "0";				
					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
					CString strSkip = "1";
					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
					CString strSerial = "";
					CString strVar = "1";

//					CString strProductID;	strProductID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_product_id[i]);
//					
//					CString strLotID;		strLotID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_product_id[i]);
//					CString strSkip;		strSkip.Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[i]);
//					CString strArraySN;		strArraySN.Format("%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuff_num[i] );
//					CString strSerial;		strSerial.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[i]);
//					CString strVar;			strVar.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_var[i]);
					
					// 모듈 셋
					AModule mdl;
					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
					tray.SetModule(i, mdl);
				}	
				tray.SetTargetPos( IDPOS_DENSITY );
				tray.SetWorkState( EWS_DENSITY );
				g_Tray.AddTray( tray );


			}
			else
			{
				ReqLdClamp(EHS_LCS_LD12, BACKWARD);
				bUnclap = true;
			}

		}
	}
	else return;
	if(GetLdClamp(EHS_LCS_LD34) == FB_NONE)
	{
		if( CheckLdClamp(EHS_LCS_LD34, IO_ON) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD34][0]) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD34][1]) == IO_ON )
			{
				ATray tray;
				tray.SetPos( IDPOS_BARCODE1 );
				for( int i=0; i<MAX_MODULE; i++ )
				{
					CString strProductID = "0";				
					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
					CString strSkip = "1";
					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
					CString strSerial = "";
					CString strVar = "1";
					
					// 모듈 셋
					AModule mdl;
					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
					tray.SetModule(i, mdl);
				}	
				tray.SetTargetPos( IDPOS_BARCODE1 );
				tray.SetWorkState( EWS_BARCODE1 );
				g_Tray.AddTray( tray );
			}
			else
			{
				ReqLdClamp(EHS_LCS_LD34, BACKWARD);
				bUnclap = true;
			}

		}
	}
	else return;
	if(GetLdClamp(EHS_LCS_LD56) == FB_NONE)
	{
		if( CheckLdClamp(EHS_LCS_LD56, IO_ON) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD56][0]) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD56][1]) == IO_ON )
			{
				ATray tray;
				tray.SetPos( IDPOS_BARCODE2 );
				for( int i=0; i<MAX_MODULE; i++ )
				{
					CString strProductID = "0";				
					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
					CString strSkip = "1";
					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
					CString strSerial = "";
					CString strBIN = "";
					CString strVar = "1";
					
					// 모듈 셋
					AModule mdl;
					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
					tray.SetModule(i, mdl);
				}	
				tray.SetTargetPos( IDPOS_BARCODE2 );
				tray.SetWorkState( EWS_BARCODE2 );
				g_Tray.AddTray( tray );
			}
			else
			{
				ReqLdClamp(EHS_LCS_LD56, BACKWARD);
				bUnclap = true;
			}
		}
	}
	else return;
	if(GetLdClamp(EHS_LCS_LD78) == FB_NONE)
	{
		if( CheckLdClamp(EHS_LCS_LD78, IO_ON) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD78][0]) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[EHS_LCS_LD78][1]) == IO_ON )
			{
				ATray tray;
				tray.SetPos( IDPOS_TOP_VISION );
				for( int i=0; i<MAX_MODULE; i++ )
				{
					CString strProductID = "0";				
					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
					CString strSkip = "1";
					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
					CString strSerial = "";
					CString strBIN = "";
					CString strVar = "1";
					
					// 모듈 셋
					AModule mdl;
					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
					tray.SetModule(i, mdl);
				}	
				tray.SetTargetPos( IDPOS_TOP_VISION );
				tray.SetWorkState( EWS_TOPVISION);
				g_Tray.AddTray( tray );
			}
		}
	}
	else return;
	mn_InitStep = ETRI_FINISH;
	if(bUnclap == true)
	{
		mn_InitStep = ETRI_IDBUFFER_LDCLAMP_CHK_AGAIN;

	}
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_LdClamp_Chk_Again()
{
	if(GetLdClamp(EHS_LCS_LD12) != FB_NONE) return;
	if(GetLdClamp(EHS_LCS_LD34) != FB_NONE) return;
	if(GetLdClamp(EHS_LCS_LD56) != FB_NONE) return;
	if(GetLdClamp(EHS_LCS_LD78) != FB_NONE) return;
	
	mn_InitStep = ETRI_FINISH;

}

void CRun_Top_IDBuffer_Works::ETRI_Finish()
{
	for (int i = 0; i < 4; i++)
	{
		if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[i][0]) == IO_ON )
			st_sync.mn_top_ibuffer_work[BTBUFFER][i][0] = CTL_YES;
		if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_chk[i][1]) == IO_OFF )
			st_sync.mn_top_ibuffer_work[BTBUFFER][i][1] = CTL_YES;
		if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_box[i][0]) == IO_ON )
			st_sync.mn_top_ibuffer_work[BTBOX][i][0] = CTL_YES;
		if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_box[i][1]) == IO_ON )
			st_sync.mn_top_ibuffer_work[BTBOX][i][1] = CTL_YES;
	}

//	Func.OnInitBtn( RBTN_TOPID_LD );//2014.1018
	st_handler.mn_init_state[INIT_TOP_IDBUFFER] = CTL_YES;
	mn_InitStep = ETRI_START;

}

void CRun_Top_IDBuffer_Works::ReqHookClamp( int nSite, EFB efb )
{
	m_fbHookClamp[nSite] = efb;
	m_step_hookclamp[nSite] = ESTEP_CYL_START;
}

void CRun_Top_IDBuffer_Works::ReqHookFwdBwd( int nSite, EFB efb )
{
	m_fbHookhwdbwd[nSite] = efb;
	m_step_hookfwdbwd[nSite] = ESTEP_CYL_START;
}

void CRun_Top_IDBuffer_Works::ReqLdClamp( int nSite, EFB efb )
{
	m_fbLdClamp[nSite] = efb;
	m_step_ldclamp[nSite] = ESTEP_CYL_START;
}

void CRun_Top_IDBuffer_Works::Run_Move_HookClamp(int nSite)
{
	if(m_fbHookClamp[nSite] == FB_NONE) return;

	int iOnOff = IO_ON;
	if(m_fbHookClamp[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch( m_step_hookclamp[nSite])
	{
	case ESTEP_CYL_START: m_step_hookclamp[nSite] = ESTEP_CYL_MOVE; break;

	case ESTEP_CYL_MOVE:
		{
			SetHookClamp( nSite, iOnOff );
			if(COMI.mn_run_status == dRUN) {
				st_work.mn_site_hookclamp[nSite] = iOnOff;
				st_work.mn_site_hookunclamp[nSite] = !iOnOff;
			}

			m_step_hookclamp[nSite] = ESTEP_CYL_CHK;
			m_dwTimeHookClampStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckHookClamp(nSite, iOnOff) )
			{
				m_step_hookclamp[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeHookClamp[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeHookClampStart[nSite] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Hook_Clamp_Time(st_io.o_top_idbuffer_shifter_hook_clamp[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 931, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeHookClampStart[nSite] < 0 )
			{
				m_dwTimeHookClampStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeHookClamp[nSite] < 0 )
			{
				m_dwTimeHookClamp[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeHookClamp[nSite] < st_time.n_wait_time[E_WAIT_HOOK_CLAMP][iOnOff])
				return;

			m_fbHookClamp[nSite] = FB_NONE;
			m_step_hookclamp[nSite] = ESTEP_CYL_START;
		}
		break;	
	}
}

void CRun_Top_IDBuffer_Works::Run_Move_HookFwdBwd(int nSite)
{
	if(m_fbHookhwdbwd[nSite] == FB_NONE) return;

	int iOnOff = IO_ON;
	if(m_fbHookhwdbwd[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch( m_step_hookfwdbwd[nSite])
	{
	case ESTEP_CYL_START: m_step_hookfwdbwd[nSite] = ESTEP_CYL_MOVE; break;

	case ESTEP_CYL_MOVE:
		{
			SetHookFwdBwd( nSite, iOnOff );
			if( COMI.mn_run_status == dRUN ) {
				st_work.mn_site_hookfwd[nSite] = iOnOff;
				st_work.mn_site_hookbwd[nSite] = !iOnOff;
			}
			m_step_hookfwdbwd[nSite] = ESTEP_CYL_CHK;
			m_dwTimeHookHwdBwdStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckHookFwdBwd( nSite, iOnOff) )
			{
				m_step_hookfwdbwd[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeHookHwdBwd[nSite] = GetCurrentTime();
			}
			else if(GetCurrentTime() - m_dwTimeHookHwdBwdStart[nSite] > st_time.n_limit_time[E_WAIT_HOOK_FWDBWD])
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Hook_FwdBwd_Time(st_io.o_top_idbuffer_shifter_hook_fwd[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 901, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeHookHwdBwdStart[nSite] < 0 )
			{
				m_dwTimeHookHwdBwdStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() -  m_dwTimeHookHwdBwd[nSite] < 0 ) m_dwTimeHookHwdBwd[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeHookHwdBwd[nSite] < st_time.n_wait_time[E_WAIT_HOOK_FWDBWD][iOnOff] )
				return;

			m_fbHookhwdbwd[nSite] = FB_NONE;
			m_step_hookfwdbwd[nSite] = ESTEP_CYL_START;
		}
		break;

	}
}

void CRun_Top_IDBuffer_Works::Run_Move_LdClamp(int nSite)
{
	if( m_fbLdClamp[nSite] == FB_NONE) return;

	int iOnOff = IO_ON;
	if( m_fbLdClamp[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_ldclamp[nSite]) {
	case ESTEP_CYL_START: m_step_ldclamp[nSite] = ESTEP_CYL_MOVE; break;
		
	case ESTEP_CYL_MOVE:
		{
			SetLdClamp(nSite, iOnOff);
			if( COMI.mn_run_status == dRUN )
			{
				st_work.mn_site_ldclamp[nSite] = iOnOff;
				st_work.mn_site_ldunclamp[nSite] = !iOnOff;
			}
			m_step_ldclamp[nSite] = ESTEP_CYL_CHK;
			m_dwTimeLdClampStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckLdClamp(nSite, iOnOff) == true )
			{
				m_step_ldclamp[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeLdClamp[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeLdClampStart[nSite] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 902, dWARNING, strJamCode );
				}

			}				
			else if( GetCurrentTime() - m_dwTimeLdClampStart[nSite] < 0)
			{
				m_dwTimeLdClampStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeLdClamp[nSite] < 0 )
			{
				m_dwTimeLdClamp[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeLdClamp[nSite] < st_time.n_wait_time[E_WAIT_HOOK_CLAMP][iOnOff])
				return;

			m_fbLdClamp[nSite] = FB_NONE;
			m_step_ldclamp[nSite] = ESTEP_CYL_START;
		}
		break;
	}
}

void CRun_Top_IDBuffer_Works::SetHookClamp( int nSite, int iOnOff )
{
	int iHook_1 = st_io.o_top_idbuffer_shifter_hook_clamp[nSite];
	int iHook_2 = st_io.o_top_idbuffer_shifter_hook_unclamp[nSite];
	g_ioMgr.set_out_bit( iHook_1, iOnOff );
	g_ioMgr.set_out_bit( iHook_2, !iOnOff );
}

bool CRun_Top_IDBuffer_Works::CheckHookClamp(int nSite, int iOnOff )
{
	int iHook_Clamp = st_io.i_top_idbuffer_shifter_hook_clamp[nSite];
	int iHook_UnClamp = st_io.i_top_idbuffer_shifter_hook_unclamp[nSite];

	if( g_ioMgr.get_in_bit( iHook_Clamp ) != iOnOff)	return false;
	if( g_ioMgr.get_in_bit( iHook_UnClamp) != !iOnOff)	return false;

	return true;
}

void CRun_Top_IDBuffer_Works::SetHookFwdBwd( int nSite, int iOnOff )
{
	int iHook_1 = st_io.o_top_idbuffer_shifter_hook_fwd[nSite];
	int iHook_2 = st_io.o_top_idbuffer_shifter_hook_bwd[nSite];

	g_ioMgr.set_out_bit( iHook_1, iOnOff);
	g_ioMgr.set_out_bit( iHook_2, !iOnOff);
}

bool CRun_Top_IDBuffer_Works::CheckHookFwdBwd( int nSite, int iOnOff )
{
	int iHook_Fwd = st_io.i_top_idbuffer_shifter_hook_fwd[nSite];
	int iHook_Bwd = st_io.i_top_idbuffer_shifter_hook_bwd[nSite];

	if( g_ioMgr.get_in_bit( iHook_Fwd ) != iOnOff ) return false;
	if( g_ioMgr.get_in_bit( iHook_Bwd ) != !iOnOff ) return false;

	return true;
}

void CRun_Top_IDBuffer_Works::SetLdClamp( int nSite, int iOnOff )
{
	int iLd_1 = st_io.o_top_idbuffer_clamp[nSite];
	int iLd_2 = st_io.o_top_idbuffer_unclamp[nSite];

	g_ioMgr.set_out_bit( iLd_1, iOnOff);
	g_ioMgr.set_out_bit( iLd_2, !iOnOff);
}

bool CRun_Top_IDBuffer_Works::CheckLdClamp( int nSite, int iOnOff)
{
	int iLd_1 = st_io.i_top_idbuffer_clamp[nSite];
	int iLd_2 = st_io.i_top_idbuffer_unclamp[nSite];

	if( g_ioMgr.get_in_bit( iLd_1 ) != iOnOff)	return false;
	if( g_ioMgr.get_in_bit( iLd_2 ) != !iOnOff)	return false;

	return true;
}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_HOOK_LdClamp_Chk()
{
	if( GetHookClamp( EHS_HS_LD1 )		 != FB_NONE )	return;
	if( GetHookClamp( EHS_HS_LD2_LEFT )  != FB_NONE )	return;
	if( GetHookClamp( EHS_HS_LD2_RIGHT ) != FB_NONE )	return;

	mn_InitStep = ETRI_IDBUFFER_LDFWDBWD;

}

void CRun_Top_IDBuffer_Works::ETRI_IDBuffer_FwdBwd_Chk()
{
	if( GetHookFwdBwd( EHS_HFB_LD1 )	!= FB_NONE )	return;
	if( GetHookFwdBwd( EHS_HFB_LD2 )	!= FB_NONE )	return;

	mn_InitStep = ETRI_HOMECHK_TOPID_LD1;
}

void CRun_Top_IDBuffer_Works::ETRI_HomeChk_LD1()
{
	int nRet = COMI.HomeCheck_Mot( M_TOPID_LD1, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = ETRI_MONE_SAFETY_LOAD1;
//		Func.OnInitBtn( RBTN_TOPID_LD_1 );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//330001 0 00 "Top ID Buffr LD1 Axis Motor Home Check Error."
		CString strJamCode = "330001";
		CTL_Lib.Alarm_Error_Occurrence(903, CTL_dWARNING, strJamCode );
	}
}

void CRun_Top_IDBuffer_Works::ETRI_Move_Safety_Load1()
{
	int nRet_1 = CTL_Lib.Single_Move(M_TOPID_LD1, st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_SAFETY], st_basic.nManualSpeed);
	if (nRet_1 == BD_GOOD)
	{			
		mn_InitStep = ETRI_HOMECHK_TOPID_LD2;
	}
	else if (nRet_1 == BD_RETRY)
	{		
	}
	else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
	{
		CTL_Lib.Alarm_Error_Occurrence(904, CTL_dWARNING, COMI.mc_alarmcode);
	}
}

void CRun_Top_IDBuffer_Works::ETRI_HomeChk_LD2()
{
	int nRet = COMI.HomeCheck_Mot( M_TOPID_LD2, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = ETRI_MONE_SAFETY_LOAD2;
//		Func.OnInitBtn( RBTN_TOPID_LD_2 );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//340001 0 00 "Top ID Buffr LD2 Axis Motor Home Check Error."
		CString strJamCode = "340001";
		CTL_Lib.Alarm_Error_Occurrence(905, CTL_dWARNING, strJamCode );
	}
}

void CRun_Top_IDBuffer_Works::ETRI_Move_Safety_Load2()
{
	int nRet_1 = CTL_Lib.Single_Move(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_SAFETY], st_basic.nManualSpeed);
	if (nRet_1 == BD_GOOD)
	{			
		mn_InitStep = ETRI_IDBUFFER_LDUNCLAMP;
	}
	else if (nRet_1 == BD_RETRY)
	{		
	}
	else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
	{
		CTL_Lib.Alarm_Error_Occurrence(906, CTL_dWARNING, COMI.mc_alarmcode);
	}
}

void CRun_Top_IDBuffer_Works::Run_Move()
{
	Run_Move_RecvIDBuffer();
	Run_Move_SendIDBuffer();
}

void CRun_Top_IDBuffer_Works::Run_Restore()
{
}

void CRun_Top_IDBuffer_Works::OnRobot_FinalPos()
{
}

void CRun_Top_IDBuffer_Works::Serialize(CArchive &ar)
{
}

void CRun_Top_IDBuffer_Works::ReqSendTray()
{
	if( m_step_sendTray == SEND_IDTRAY_NONE )
		m_step_sendTray = SEND_IDTRAY_READY;

}

void CRun_Top_IDBuffer_Works::ReqRecvTray()
{
	if( m_step_recvTray == RECV_IDTRAY_NONE )
		m_step_recvTray = RECV_IDTRAY_READY;

//	if( g_handler.GetMachinePos() == EMPOS_REAR )
//		g_client_front.SendRecvState();

}


void CRun_Top_IDBuffer_Works::Run_Move_SendIDBuffer()
{
	if( m_step_sendTray_old != m_step_sendTray )
	{
		m_step_sendTray_old = m_step_sendTray;
		m_dwTime_SendTray = GetCurrentTime();

//		if( Run_Move_Chk_Jam_SendTray() == false )
//			return;
	}

//	if( Run_Move_Chk_Time_SendTray() == false )
// 		return;

	Func.ThreadFunctionStepTrace(51, m_step_sendTray);

	switch( m_step_sendTray )
	{
	case SEND_IDTRAY_NONE:	break;

	case SEND_IDTRAY_READY:
		{
			ReqHookClamp(EHS_HS_LD2_LEFT, BACKWARD);
			ReqHookClamp(EHS_HS_LD2_RIGHT, BACKWARD);
			for ( int i = 0; i < 2; i++)
			{
				g_Run_Barcode_Works.ReqBoxUpDn(i, BACKWARD);
			}

			m_step_sendTray = SEND_IDTRAY_READY_CHK;
		}
		break;

	case SEND_IDTRAY_READY_CHK:
		{
			if( m_fbHookClamp[EHS_HS_LD2_LEFT] != FB_NONE ) return;
			if( m_fbHookClamp[EHS_HS_LD2_RIGHT] != FB_NONE ) return;
			for ( int i = 0; i < 2; i++)
			{
				if( g_Run_Barcode_Works.GetBoxUpDn(i)!= FB_NONE ) return;
			}
			m_dwTime_SendTray = GetCurrentTime();
			m_step_sendTray = SEND_IDTRAY_LD2_FORWARD;
		}
		break;

	case SEND_IDTRAY_LD2_FORWARD:
		{
			if( GetCurrentTime() - m_dwTime_SendTray < 0 ) m_dwTime_SendTray = GetCurrentTime();
			if( GetCurrentTime() - m_dwTime_SendTray < 200 ) break;

			int nRet = CTL_Lib.Single_Move( M_TOPID_LD2, st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_PICK], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				ReqHookFwdBwd(EHS_HFB_LD2, FORWARD);
				m_step_sendTray = SEND_IDTRAY_LD2_FORWARD_CHK;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 907, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case SEND_IDTRAY_LD2_FORWARD_CHK:
		if( m_fbHookhwdbwd[EHS_HFB_LD2] != FB_NONE ) return;
		m_step_sendTray = SEND_IDTRAY_LD2_CLAMP;
		break;

	case SEND_IDTRAY_LD2_CLAMP://Clamp ON
		{
			ReqHookClamp(EHS_HS_LD2_LEFT, FORWARD);
			ReqHookClamp(EHS_HS_LD2_RIGHT, FORWARD);

			m_step_sendTray = SEND_IDTRAY_LD2_CLAMP_CHK;
		}
		break;
		
	case SEND_IDTRAY_LD2_CLAMP_CHK:
		if( m_fbHookClamp[EHS_HS_LD2_LEFT] != FB_NONE ) return;
		if( m_fbHookClamp[EHS_HS_LD2_RIGHT] != FB_NONE ) return;
		m_step_sendTray = SEND_IDTRAY_LD_CLAMP_OFF;
		break;

	case SEND_IDTRAY_LD_CLAMP_OFF://LDClamp OFF
		{
			ReqLdClamp(EHS_LCS_LD12, BACKWARD);
			ReqLdClamp(EHS_LCS_LD34, BACKWARD);
			ReqLdClamp(EHS_LCS_LD56, BACKWARD);
			ReqLdClamp(EHS_LCS_LD78, BACKWARD);
			g_Run_IDBuffers_El.ReqRecvReadyTray();
			m_step_sendTray = SEND_IDTRAY_LD_CLAMP_OFF_CHK;
		}
		break;

	case SEND_IDTRAY_LD_CLAMP_OFF_CHK:
		{
			if(m_fbLdClamp[EHS_LCS_LD12] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD34] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD56] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD78] != FB_NONE) return;
			if( !g_Run_IDBuffers_El.GetRecvReadyTrayNone() ) return;


			TempSendShiftData();

			ReqHookFwdBwd(EHS_HFB_LD2, BACKWARD);
			m_step_sendTray = SEND_IDTRAY_EL_READY_CHK;
		}
		break;

	case SEND_IDTRAY_EL_READY_CHK:
			if( COMI.Get_MotCurrentPos( M_IDBUFFER_EL) < st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER] - COMI.md_allow_value[M_IDBUFFER_EL])
			{
				g_Run_IDBuffers_El.ReqElvMoveReady( ECW_WORK );
				m_step_sendTray = SEND_IDTRAY_EL_MOVE;
			}
			else
			{
				m_step_sendTray = SEND_IDTRAY_LD2_MOVE;
			}
			break;

	case SEND_IDTRAY_EL_MOVE:
		if( g_Run_IDBuffers_El.m_fbElvMoveReady != ECW_NONE) return;
		
		m_step_sendTray = SEND_IDTRAY_LD2_MOVE;
		break;

	case SEND_IDTRAY_LD2_MOVE:
		{
			int nRet = CTL_Lib.Single_Move( M_TOPID_LD2, st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_sendTray = SEND_IDTRAY_LD2_MOVE_CHK;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 908, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;	
		
	case SEND_IDTRAY_LD2_MOVE_CHK:
		{
			if(m_fbHookhwdbwd[EHS_HFB_LD2] != FB_NONE) return;
			m_step_sendTray = SEND_IDTRAY_LD_CLAMP_ON;
		}
		break;

	case SEND_IDTRAY_LD_CLAMP_ON:
		{
			ReqLdClamp(EHS_LCS_LD12, FORWARD);
			ReqLdClamp(EHS_LCS_LD34, FORWARD);
			ReqLdClamp(EHS_LCS_LD56, FORWARD);
			ReqLdClamp(EHS_LCS_LD78, FORWARD);
			g_Run_IDBuffers_El.ReqRecvDoneTray();
			m_step_sendTray = SEND_IDTRAY_LD_CLAMP_ON_CHK;
		}
		break;

	case SEND_IDTRAY_LD_CLAMP_ON_CHK:
		{
			if(m_fbLdClamp[EHS_LCS_LD12] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD34] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD56] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD78] != FB_NONE) return;
			if(m_fbLdClamp[EHS_LCS_LD78] != FB_NONE) return;
			if( !g_Run_IDBuffers_El.GetRecvDoneTrayNone() ) return;

			ReqHookClamp(EHS_HS_LD2_LEFT, BACKWARD);
			ReqHookClamp(EHS_HS_LD2_RIGHT, BACKWARD);
			m_step_sendTray = SEND_IDTRAY_LD2_CLAMP_OFF;

		}
		break;

	case SEND_IDTRAY_LD2_CLAMP_OFF:
		{
			if( m_fbHookClamp[EHS_HS_LD2_LEFT] != FB_NONE) return;
			if( m_fbHookClamp[EHS_HS_LD2_RIGHT] != FB_NONE) return;

			MoveTempShiftData();

			m_step_sendTray = SEND_IDTRAY_LD2_CLAMP_OFF_CHK;
		}
		break;

	case SEND_IDTRAY_LD2_CLAMP_OFF_CHK:
		{


			int nRet = CTL_Lib.Single_Move( M_TOPID_LD2, st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_PICK], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				ReqHookFwdBwd(EHS_HFB_LD2, FORWARD);
				m_step_sendTray = SEND_IDTRAY_FINISH;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 909, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;	

	case SEND_IDTRAY_FINISH:
		{
			if( m_fbHookhwdbwd[EHS_HFB_LD2] != FB_NONE ) return;
			
			if( top_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_BCR1) != CTL_GOOD )
				break;
			if( top_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_BCR2) != CTL_GOOD )
				break;
			if( top_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_TOP) != CTL_GOOD )
				break;
			if( top_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_ULD) != CTL_GOOD )
				break;
			
			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_TOP_VISION), EIDBUFFER_POS(IDPOS_UNLOAD) );
			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_BARCODE2), EIDBUFFER_POS(IDPOS_TOP_VISION) );
			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_BARCODE1), EIDBUFFER_POS(IDPOS_BARCODE2) );
			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_DENSITY), EIDBUFFER_POS(IDPOS_BARCODE1) );

			int iIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE1 );
			if( iIdx >= 0)
			{
				//2014,1105
				if( st_handler.mn_d_piclplce == CTL_YES )
				{
					ATray& trayPri = g_Tray.GetTray( iIdx );
					trayPri.SetWorkState( EWS_BARCODE1 );
				}
			}
			iIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE2 );
			if( iIdx >= 0)
			{
				//2014,1105
				if( st_handler.mn_d_piclplce == CTL_YES )
				{
					ATray& trayPri = g_Tray.GetTray( iIdx );
					trayPri.SetWorkState( EWS_BARCODE2 );
				}
			}
			iIdx = g_Tray.GetIdxByPos( IDPOS_TOP_VISION );
			if( iIdx >= 0)
			{
				//2014,1105
				if( st_handler.mn_d_piclplce == CTL_YES )
				{
					ATray& trayPri = g_Tray.GetTray( iIdx );
					trayPri.SetWorkState( EWS_TOPVISION );
				}
			}
			//st_sync.n_unload_elevator_req = CTL_FREE;
			m_step_sendTray = SEND_IDTRAY_NONE;
		}
		break;	

	}

}

void CRun_Top_IDBuffer_Works::Run_Move_RecvIDBuffer()
{
	if( m_step_recvTray_old != m_step_recvTray )
	{
		m_step_recvTray_old = m_step_recvTray;
		m_dwTime_RecvTray = GetCurrentTime();

//		if( Run_Move_Chk_Jam_RecvTray() == false )
//			return;
	}

//	if( Run_Move_Chk_Time_RecvTray() == false )
// 		return;

	Func.ThreadFunctionStepTrace(52, m_step_recvTray);

	switch( m_step_recvTray )
	{
	case RECV_IDTRAY_NONE:
		{
			//int iIdx = g_Tray.GetIdxByPos( IDPOS_DENSITY );
			//if( st_handler.mn_d_piclplce == CTL_YES )
			//{
// 				ALabel* pLdl_1 = NULL;
// 				ALabel* pLdl_2 = NULL;
// 				pLdl_1 = &g_Tray.GetTray( iIdx ).GetModule( 0 ).GetLabel();
// 				pLdl_2 = &g_Tray.GetTray( iIdx ).GetModule( 1 ).GetLabel();
// 				ATray& trayPri = g_Tray.GetTray( iIdx );
// 				
// 				ALabel lal;
// 				lal.SetLabelInfo( g_lotMgr.GetLotAt(0).GetLotID(), "test2D" , g_lotMgr.GetLotAt(0).GetDenLabelCapa(), EMBS_PASS );
// 				
// 				*pLdl_1 = lal;
// 				*pLdl_2 = lal;
// 				
// 				trayPri.SetWorkState(EWS_DENSITY);
// 			}

		}
		break;

	case RECV_IDTRAY_READY:
		{
			st_handler.mn_smema_front = CTL_READY;
			
//			g_front_server.SetBufferIn( NR_WAITING );
			if( st_basic.mn_mode_run)
			{
				clsBarcode.OnBarcodeReset();
			}
			//g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_FRONT), EIDBUFFER_POS(IDPOS_FRONT_TO_DENSITY) );
		
			ReqHookFwdBwd( EHS_HFB_LD1, BACKWARD );
			ReqHookClamp( EHS_HS_LD1, BACKWARD );
			ReqLdClamp( EHS_LCS_LD12, BACKWARD );
			m_step_recvTray = RECV_IDTRAY_READY_CHK;
		}
		break;

	case RECV_IDTRAY_READY_CHK:
		{
			int nRet = CTL_Lib.Single_Move( M_TOPID_LD1, st_motor[ M_TOPID_LD1].d_pos[P_LDBUFFER_PICK], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_recvTray = RECV_IDTRAY_HOOK_ON;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 920, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case RECV_IDTRAY_HOOK_ON:

		if( st_handler.mn_front_ready == CTL_YES || g_ioMgr.get_in_bit( st_io.i_front_top_machine_ready_signal_chk) == IO_ON)
		{
			if( st_handler.mn_front_ready == CTL_YES)//Manual Run
			{
				EIDBUFFER_POS eTargetPos = IDPOS_NONE;
				if( g_Tray.IsEnablePos( IDPOS_FRONT_TO_DENSITY ) )
				{
					eTargetPos = IDPOS_FRONT_TO_DENSITY;
				}

				if( eTargetPos != TPOS_NONE )
				{
					ATray tray;
					tray.SetPos( IDPOS_FRONT_TO_DENSITY );
					tray.SetTrayInfo( g_lotMgr.GetLotIDAt(0), 0, false, false );
					
					for( int i=0; i<MAX_MODULE; i++ )
					{
						CString strProductID = "mSATA";
						
						CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
						CString strSkip = "0";
						CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
						CString strSerial;		strSerial.Format( "S11ZNYAC500%d%02d", eTargetPos - IDPOS_DENSITY, i );
						CString strVar = "1";
						
						// 모듈 셋
						AModule mdl;
 						mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
						tray.SetModule(i, mdl);
					}
					
					tray.SetTargetPos( eTargetPos );
					g_Tray.AddTray( tray );
				}
				g_front_server.SetBufferIn(NR_RECEIVED);
			}


			if( GetHookFwdBwd(EHS_HFB_LD1) != FB_NONE )			return;
			if( GetHookClamp(EHS_HS_LD1) != FB_NONE )			return;

			ReqHookClamp( EHS_HS_LD1, FORWARD );
			m_step_recvTray = RECV_IDTRAY_HOOK_ON_CHK;
		}

		break;

	case RECV_IDTRAY_HOOK_ON_CHK:
		if( GetHookClamp(EHS_HS_LD1) != FB_NONE )			return;
		
		m_step_recvTray = RECV_IDTRAY_FRONT_ON;
		break;

	case RECV_IDTRAY_FRONT_ON:
		st_handler.mn_smema_front = CTL_CHANGE;
		g_ioMgr.set_out_bit( st_io.o_front_top_machine_enter_signal, IO_OFF );//Clamp on 후  Clamp off 요청
		g_ioMgr.set_out_bit( st_io.o_front_top_machine_transfer_signal, IO_ON );//Clamp on 후  Clamp off 요청
		m_step_recvTray = RECV_IDTRAY_FRONT_ON_CHK;
		break;

	case RECV_IDTRAY_FRONT_ON_CHK:
		if( /*st_basic.n_mode_device == false || */g_ioMgr.get_in_bit( st_io.i_front_top_machine_ready_signal_chk, 0, 50) == IO_OFF)////Clamp Off(앞설비)
		{
			m_step_recvTray = RECV_IDTRAY_BUFFER_IN_CHK;

			m_dwTimeTopIDBuffer = GetCurrentTime();
		}
		break;

	case RECV_IDTRAY_BUFFER_IN_CHK:
		if( g_front_server.GetBufferInReceived() && st_handler.mn_smema_front == CTL_CHANGE)//ybs
		{
			int iIdx = g_Tray.GetIdxByPos( IDPOS_FRONT_TO_DENSITY );
			if( iIdx < 0 )//580000 1 00 "There is no tray at Front to density pos."
			{
				CTL_Lib.Alarm_Error_Occurrence( 940, CTL_dWARNING, "580000" );
				break;
			}
			if( g_Tray.GetTray(iIdx).GetModule(0).IsIDBufferExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[0] = CTL_YES;
			}
			else
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[0] = CTL_NO;
			}
			if( g_Tray.GetTray(iIdx).GetModule(1).IsIDBufferExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[1] = CTL_YES;
			}
			else
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[1] = CTL_NO;
			}
			if( g_Tray.GetTray(iIdx).GetModule(0).IsExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[10] =  CTL_YES;
			}
			else
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[10] = CTL_NO;
			}
			if( g_Tray.GetTray(iIdx).GetModule(1).IsExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[11] = CTL_YES;
			}
			else
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[11] = CTL_NO;
			}
			
			if( st_basic.mn_mode_run) g_front_server.SetBufferIn( NR_NONE );

			m_step_recvTray = RECV_IDTRAY_MOVE;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeTopIDBuffer < 0) m_dwTimeTopIDBuffer = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeTopIDBuffer > st_basic.mn_network_wait_time)
			{
				mn_RetryCnt++;
				if ( mn_RetryCnt > st_basic.mn_network_retry_cnt )
				{//800000 1 A "Can't receive Front data from 8580 top. time out."
					CTL_Lib.Alarm_Error_Occurrence( 940, CTL_dWARNING, "580000" );
					m_step_recvTray = RECV_IDTRAY_FRONT_ON_CHK;
					break;
				}
			}
		}

		break;

	case RECV_IDTRAY_MOVE:
		{
			int nRet = CTL_Lib.Single_Move( M_TOPID_LD1, st_motor[ M_TOPID_LD1].d_pos[P_LDBUFFER_LAOD], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_recvTray = RECV_IDTRAY_LOT_CHK;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 921, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case RECV_IDTRAY_LOT_CHK:
		//LOT체크 후 LAST인지 개수도 확인한다.
		//LotCheck()
		//if( m_nSiteID != 0 || (g_ioMgr.get_in_bit( st_io.i_bin_tray1_direction_chk ) == IO_OFF) )
		{
			ReqHookFwdBwd( EHS_HFB_LD1, FORWARD );

			m_step_recvTray = RECV_IDTRAY_CLAMP;
		}
		break;

	case RECV_IDTRAY_CLAMP:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckLdClamp(EHS_LCS_LD12, IO_OFF) )
			{			
				m_step_recvTray = RECV_IDTRAY_MOVED;
			}
			else
			{
				ReqLdClamp( EHS_LCS_LD12, BACKWARD );
				m_step_recvTray = RECV_IDTRAY_CLAMP_CHK;
			}
		}
		break;

	case RECV_IDTRAY_CLAMP_CHK:
		if( GetLdClamp(EHS_LCS_LD12) != FB_NONE ) return;
		m_step_recvTray = RECV_IDTRAY_CLAMP;

		break;

	case RECV_IDTRAY_MOVED:
//		g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_FRONT), EIDBUFFER_POS(IDPOS_DENSITY) );


		m_step_recvTray = RECV_IDTRAY_LOAD_BCR_COMPLETED;
		break;

//	case RECV_IDTRAY_BACR1:
//		{
//			int nRet = CTL_Lib.Single_Move( M_TOPID_LD1, st_motor[ M_TOPID_LD1].d_pos[P_LDBUFFER_BCR1], (int)st_basic.nRunSpeed );
//			if( nRet == BD_GOOD )
//			{
//				m_step_recvTray = RECV_IDTRAY_BACR1_READ;
//			}
//			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
//			{
//				CTL_Lib.Alarm_Error_Occurrence( 922, CTL_dWARNING, alarm.mstr_code );
//			}
//		}
//		break;
//
//	case RECV_IDTRAY_BACR1_READ:
//		{
//			CString strTemp;
//			if( st_basic.mn_mode_run )
//			{
//				int nCount = clsBarcode.OnBarcodeCount();
//
//				if( nCount < 1)
//				{//630000 1 00 "Barcode and Module count is different."
//					CTL_Lib.Alarm_Error_Occurrence( 923, CTL_dWARNING, "630000" );
//					clsBarcode.OnBarcodeReset();
//					m_step_recvTray = RECV_IDTRAY_BACR1;
//					break;
//				}
//
//				for (int i=0; i<nCount; i++)
//				{
//					strTemp = "BARCODE_DATA=";
//					strTemp += clsBarcode.OnBarcodeReadData(i);
//
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_normal_msg, strTemp);
//						st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
//					}
//				}
//
//			}
//			m_step_recvTray = RECV_IDTRAY_BACR2;
//		}
//		
//		break;
//
//
//
//	case RECV_IDTRAY_BACR2:
//		{
//			int nRet = CTL_Lib.Single_Move( M_TOPID_LD1, st_motor[ M_TOPID_LD1].d_pos[P_LDBUFFER_BCR2], (int)st_basic.nRunSpeed );
//			if( nRet == BD_GOOD )
//			{
//				m_step_recvTray = RECV_IDTRAY_BACR2_READ;
//			}
//			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
//			{
//				CTL_Lib.Alarm_Error_Occurrence( 924, CTL_dWARNING, alarm.mstr_code );
//			}
//		}
//		break;
//
//	case RECV_IDTRAY_BACR2_READ:
//		{
//			int iIdx = g_Tray.GetIdxByPos( IDPOS_DENSITY );
//			if( iIdx < 0 )
//			{
//				//630000 1 00 "Barcode and Module count is different."
//				CTL_Lib.Alarm_Error_Occurrence( 925, CTL_dWARNING, "630000" );
//				clsBarcode.OnBarcodeReset();
//				m_step_recvTray = RECV_IDTRAY_BACR1;
//				break;
//			}
//
//			ATray trayPri = g_Tray.GetTray( iIdx );
//			CString strTemp;
//			int nCount = clsBarcode.OnBarcodeCount();
//
//
//			if(trayPri.GetModuleCnt() != clsBarcode.OnBarcodeCount())
//			{//630000 1 00 "Barcode and Module count is different."
//				CTL_Lib.Alarm_Error_Occurrence( 926, CTL_dWARNING, "630000" );
//				clsBarcode.OnBarcodeReset();
//				m_step_recvTray = RECV_IDTRAY_BACR1;
//				break;
//			}
//			for (int i=0; i<nCount; i++)
//			{
//				strTemp = "BARCODE_DATA=";
//				strTemp += clsBarcode.OnBarcodeReadData(i);
//
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_normal_msg, strTemp);
//					st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
//				}
//			}
//			m_step_recvTray = RECV_IDTRAY_LOAD_BCR_COMPLETED;
//		}
//		
//		break;


	case RECV_IDTRAY_LOAD_BCR_COMPLETED:
		{
			g_Tray.TrayMoved( EIDBUFFER_POS (IDPOS_FRONT_TO_DENSITY), EIDBUFFER_POS(IDPOS_DENSITY) );

			if( st_basic.n_mode_device)
			{
				int iIdx = g_Tray.GetIdxByPos( IDPOS_DENSITY );
				if( iIdx < 0 )
				{//630000 1 00 "Barcode and Module count is different."
					CTL_Lib.Alarm_Error_Occurrence( 967, CTL_dWARNING, "630000" );
					clsBarcode.OnBarcodeReset();
					m_step_recvTray = RECV_IDTRAY_BACR1;
					break;
				}
				ATray trayPri = g_Tray.GetTray( iIdx );
				if( trayPri.GetModuleCnt() == clsBarcode.OnBarcodeCount())
				{
					m_step_recvTray = RECV_IDTRAY_LOAD_CLAMP;
				}
				else if( st_basic.nVisionUse[0] == CTL_NO) m_step_recvTray = RECV_IDTRAY_LOAD_CLAMP;
				else
				{//630000 1 00 "Barcode and Module count is different."
					CTL_Lib.Alarm_Error_Occurrence( 927, CTL_dWARNING, "630000" );
					clsBarcode.OnBarcodeReset();
					m_step_recvTray = RECV_IDTRAY_BACR1;
				}
			}
			else
			{
				m_step_recvTray = RECV_IDTRAY_LOAD_CLAMP;
				m_dwTime_RecvTray = GetCurrentTime();
			}
		}
		break;

	case RECV_IDTRAY_LOAD_CLAMP:
		if( GetLdClamp(EHS_LCS_LD12) != FB_NONE ) return;
		if( GetCurrentTime() - m_dwTime_RecvTray < 0) m_dwTime_RecvTray = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_RecvTray < 1000) break;
		ReqLdClamp( EHS_HS_LD1, FORWARD );
		m_step_recvTray = RECV_IDTRAY_LOAD_CLAMP_CHK;
		break;

	case RECV_IDTRAY_LOAD_CLAMP_CHK:
		if( GetLdClamp(EHS_LCS_LD12) != FB_NONE ) return;

		ReqHookClamp(EHS_HS_LD1, BACKWARD);

		RecvShiftData();

		m_step_recvTray = RECV_IDTRAY_LOAD_READY;
		break;

	case RECV_IDTRAY_LOAD_READY:
		if( GetHookClamp( EHS_HS_LD1 )		 != FB_NONE )	return;
		ReqHookFwdBwd( EHS_HFB_LD1, BACKWARD );
		m_step_recvTray = RECV_IDTRAY_LOAD_READY_CHK;




		break;

	case RECV_IDTRAY_LOAD_READY_CHK:
		{
			int nRet = COMI.Start_SingleMove( M_TOPID_LD1, st_motor[ M_TOPID_LD1].d_pos[P_LDBUFFER_PICK], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				st_handler.mn_smema_front = CTL_FREE;
				st_handler.mn_emptyin_cnt = 1;
				m_step_recvTray = RECV_IDTRAY_LOAD_READY_CLAMP;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 920, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case RECV_IDTRAY_LOAD_READY_CLAMP:
		{
			if( GetHookFwdBwd(EHS_HFB_LD1) != FB_NONE )			return;

			if(top_idbuffer_sensor_flag_status_check(0,SHIFT_IDBUFF_INDEX_DENSITY) != CTL_GOOD) break;

			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_FRONT_TO_DENSITY), EIDBUFFER_POS(IDPOS_DENSITY));

			m_step_recvTray = RECV_IDTRAY_LOAD_READY_END;
		}
		break;

	case RECV_IDTRAY_LOAD_READY_END:
		{
// 			if( top_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_DENSITY ) != CTL_GOOD)
// 				break;

		//Get Barcode Format from server
			int iIdx = g_Tray.GetIdxByPos( IDPOS_DENSITY );
			if( iIdx >= 0)
			{
				//2014,1105
				if( st_handler.mn_d_piclplce == CTL_YES )
				{
					ALabel* pLdl_1 = NULL;
					ALabel* pLdl_2 = NULL;
					pLdl_1 = &g_Tray.GetTray( iIdx ).GetModule( 0 ).GetLabel();
					pLdl_2 = &g_Tray.GetTray( iIdx ).GetModule( 1 ).GetLabel();
					ATray& trayPri = g_Tray.GetTray( iIdx );

					ALabel lal;
					lal.SetLabelInfo( g_lotMgr.GetLotAt(0).GetLotID(), "test2D" , g_lotMgr.GetLotAt(0).GetDenLabelCapa(), EMBS_PASS );

					*pLdl_1 = lal;
					*pLdl_2 = lal;

					trayPri.SetWorkState(EWS_DENSITY);
				}				
			}
			
			m_step_recvTray = RECV_IDTRAY_BACRCODE;
			
		}
		break;

	case RECV_IDTRAY_BACRCODE:
		{
//			ABcrLabel* bcrtemp1 = NULL;
//			ABcrLabel* bcrtemp2 = NULL;
//			bcrtemp1 = &Func.m_pSite[SITE_BARCODE1]->GetBcrLabel(0); bcrtemp2 = &Func.m_pSite[SITE_BARCODE1]->GetBcrLabel(1);
//			if( bcrtemp1->IsExist() == true || bcrtemp2->IsExist() == true)
//			{
//				if(st_handler.cwnd_list != NULL )
//				{
//					sprintf(st_msg.c_abnormal_msg, "[BARCODE] Data Exist. Wait Please");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//				}
//				return;
//			}			
//			ATray& trayPri = g_Tray.GetTray( iIdx );
//			{
//				if(trayPri.GetModule(0).IsBufferSkip() == false && trayPri.GetModule(0).IsExist() && !trayPri.GetModule(0).GetBcrLabel().IsExist() ) 
//				{
//					g_Run_Barcode_Feeders.SetVariable( IDPOS_DENSITY, 0 );
//					g_Run_Barcode_Feeders.CommBarcode(0, trayPri.GetModule(0).GetSerial());
//				}
//				if(trayPri.GetModule(1).IsExist() && !trayPri.GetModule(1).GetBcrLabel().IsExist()) 
//				{
//					g_Run_Barcode_Feeders.SetVariable( IDPOS_DENSITY, 1 );
//					g_Run_Barcode_Feeders.CommBarcode(1, trayPri.GetModule(1).GetSerial());
//				}
//			}

			m_step_recvTray = RECV_IDTRAY_FINISH;

		}
		break;

	case RECV_IDTRAY_FINISH:
		{
			int nRet = COMI.Check_SingleMove( M_TOPID_LD1, st_motor[ M_TOPID_LD1].d_pos[P_LDBUFFER_PICK] );
			if( nRet == BD_GOOD )
			{
				m_step_recvTray = RECV_IDTRAY_NONE;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 920, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	}
}


void CRun_Top_IDBuffer_Works::RecvShiftData()
{
	//IDbuffer
	st_var.n_top_idbuffer_exist_status_check_info[0] = st_var.n_temp_top_idbuffer_exist_status_check_info[0] ;
	st_var.n_top_idbuffer_exist_status_check_info[1] = st_var.n_temp_top_idbuffer_exist_status_check_info[1] ;
	//SSD data
	st_var.n_top_idbuffer_exist_status_check_info[10] = st_var.n_temp_top_idbuffer_exist_status_check_info[10] ;
	st_var.n_top_idbuffer_exist_status_check_info[11] = st_var.n_temp_top_idbuffer_exist_status_check_info[11] ;

	st_var.n_temp_top_idbuffer_exist_status_check_info[0] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[1] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[10] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[11] = 0;
}

void CRun_Top_IDBuffer_Works::TempSendShiftData()
{
	//IDbuffer
	for ( int i = 0; i < 8; i++)
	{
		st_var.n_temp_top_idbuffer_exist_status_check_info[i+2] = st_var.n_top_idbuffer_exist_status_check_info[i];	
		st_var.n_temp_top_idbuffer_exist_status_check_info[i+12] = st_var.n_top_idbuffer_exist_status_check_info[i+10];
		st_var.n_top_idbuffer_exist_status_check_info[i] = 0;
		st_var.n_top_idbuffer_exist_status_check_info[i+10] = 0;
	}
}


void CRun_Top_IDBuffer_Works::MoveTempShiftData()
{
	for ( int i = 0; i < 8; i++)
	{
		st_var.n_top_idbuffer_exist_status_check_info[i+2] = st_var.n_temp_top_idbuffer_exist_status_check_info[i + 2];
		st_var.n_top_idbuffer_exist_status_check_info[i+12] = st_var.n_temp_top_idbuffer_exist_status_check_info[i + 12];
	}
}



/////////////////////////////////////////////////////////////////////////////////////
// 동작 flag와 그와 연관된 I/O의 상태를 체크하는 함수 
/////////////////////////////////////////////////////////////////////////////////////
int CRun_Top_IDBuffer_Works::top_idbuffer_sensor_flag_status_check(int n_mode, int n_check_site) //센서와 플레그가 틀리면 에러 
{
	int i=0;
	int nFunRet = CTL_GOOD;
	int nRet_1=0, nRet_2=0;

	 if(st_basic.n_mode_device == 0 && st_handler.mn_virtual_idbuffer == 0)//
	 {
		return CTL_GOOD;
	 }

	for(i=0; i<20; i++)
	{
		st_var.n_top_idbuffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF);
		if(n_check_site == SHIFT_IDBUFF_INPUT_INDEX)
		{
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_DENSITY )
		{
			if(i == 0 || i == 1 || i== 10 || i == 11)
			{
				if( (st_handler.mn_front_ready ||st_handler.mn_virtual_supply) )
				{
					if(i == 0 || i == 1 )
					{
						if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
						{
							sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
							CTL_Lib.Alarm_Error_Occurrence( 990, CTL_dWARNING, mc_jamcode );
							nFunRet = CTL_ERROR;
							break;
						}
					}
				}
				else
				{
					if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
					{
						sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 	 
						CTL_Lib.Alarm_Error_Occurrence( 990, CTL_dWARNING, mc_jamcode );
						nFunRet = CTL_ERROR;
						break;
					}
				}
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_BCR1 )
		{
			if(i == 2 || i == 3 || i== 12 || i == 13)
			{
				if( (st_handler.mn_front_ready ||st_handler.mn_virtual_supply) )
				{
					if(i == 2 || i == 3 )
					{
						if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
						{
							sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 	 
							CTL_Lib.Alarm_Error_Occurrence( 991, CTL_dWARNING, mc_jamcode );
							nFunRet = CTL_ERROR;
							break;
						}
					}
				}
				else
				{
					if(i == 2 || i == 3 || i== 12 || i == 13)
					{
						if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
						{
							sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 	 
							CTL_Lib.Alarm_Error_Occurrence( 991, CTL_dWARNING, mc_jamcode );
							nFunRet = CTL_ERROR;
							break;
						}
					}
				}
			}

		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_BCR2)
		{
			if(i == 4 || i == 5 || i== 14 || i == 15)
			{
				if( (st_handler.mn_front_ready ||st_handler.mn_virtual_supply) )
				{
					if(i == 4 || i == 5 )
					{
						if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
						{
							sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
							CTL_Lib.Alarm_Error_Occurrence( 992, CTL_dWARNING, mc_jamcode );
							nFunRet = CTL_ERROR;
							break;
						}
					}
				}
				else
				{
					if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
					{
						sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
						CTL_Lib.Alarm_Error_Occurrence( 992, CTL_dWARNING, mc_jamcode );
						nFunRet = CTL_ERROR;
						break;
					}
				}
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_TOP)
		{
			if(i == 6 || i == 7 || i== 16 || i == 17)
			{
				if( (st_handler.mn_front_ready ||st_handler.mn_virtual_supply) )
				{
					if(i == 6 || i == 7 )
					{
						if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
						{
							sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
							CTL_Lib.Alarm_Error_Occurrence( 993, CTL_dWARNING, mc_jamcode );
							nFunRet = CTL_ERROR;
							break;
						}
					}
				}
				else
				{
					if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
					{
						sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
						CTL_Lib.Alarm_Error_Occurrence( 993, CTL_dWARNING, mc_jamcode );
						nFunRet = CTL_ERROR;
						break;
					}
				}

			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_ULD)
		{
			if(i == 8 || i == 9 || i== 18 || i == 19)
			{
				if( (st_handler.mn_front_ready ||st_handler.mn_virtual_supply) )
				{
					if(i == 8 || i == 9 )
					{
						if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
						{
							sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
							CTL_Lib.Alarm_Error_Occurrence( 994, CTL_dWARNING, mc_jamcode );
							nFunRet = CTL_ERROR;
							break;
						}
					}
				}
				else
				{
					if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
					{
						sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
						CTL_Lib.Alarm_Error_Occurrence( 994, CTL_dWARNING, mc_jamcode );
						nFunRet = CTL_ERROR;
						break;
					}
				}
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_SITE_ALL_CHK)
		{
			if(i == 2 || i == 3 || i== 4 || i == 5 || i == 6 || i == 7 
			   || i == 12 || i == 13 || i== 14 || i == 15 || i == 16 || i == 17 )
			{
				if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
				{
					sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
					CTL_Lib.Alarm_Error_Occurrence( 995, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}
			}
		}
	}
	return nFunRet;

}


void CRun_Top_IDBuffer_Works::PreIDBufferBoxExist()
{
	int i = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[22] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[23] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[24] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[25] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[26] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[27] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[28] = 0;
	st_var.n_temp_top_idbuffer_exist_status_check_info[29] = 0;
	int iIdx1 = g_Tray.GetIdxByPos(IDPOS_DENSITY);
	if( iIdx1 >= 0 )
	{
		for ( i = 0; i < MAX_BUFFDVC; i++)
		{
			st_var.n_top_idbuffer_exist_status_check_info[i] = 0;
			st_var.n_top_idbuffer_exist_status_check_info[i+10] = 0;
			st_var.n_top_idbuffer_exist_status_check_info[i+20] = 0;

			if( g_Tray.GetTray(iIdx1).GetModule(i).IsExist() )
			{
				
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+2] = 1;
			}
			if( g_Tray.GetTray(iIdx1).GetModule(i).IsIDBufferExist())
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+12] = 1;
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+22] = 1;
			}
		}
	}
	int iIdx2 = g_Tray.GetIdxByPos(IDPOS_BARCODE1);
	if( iIdx2 >= 0 )
	{
		for ( i = 0; i < MAX_BUFFDVC; i++)
		{
			if( g_Tray.GetTray(iIdx2).GetModule(i).IsExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+4] = 1;
			}
			if( g_Tray.GetTray(iIdx2).GetModule(i).IsIDBufferExist())
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+14] = 1;
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+24] = 1;
			}
		}
	}
	int iIdx3 = g_Tray.GetIdxByPos(IDPOS_BARCODE2);
	if( iIdx3 >= 0 )
	{
		for ( i = 0; i < MAX_BUFFDVC; i++)
		{
			if( g_Tray.GetTray(iIdx3).GetModule(i).IsExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+6] = 1;
			}
			if( g_Tray.GetTray(iIdx3).GetModule(i).IsIDBufferExist())
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+16] = 1;
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+26] = 1;
			}
		}
	}
	int iIdx4 = g_Tray.GetIdxByPos(IDPOS_TOP_VISION);
	if( iIdx4 >= 0 )
	{
		for ( i = 0; i < MAX_BUFFDVC; i++)
		{
			if( g_Tray.GetTray(iIdx4).GetModule(i).IsExist() )
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+8] = 1;
			}
			if( g_Tray.GetTray(iIdx4).GetModule(i).IsIDBufferExist())
			{
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+18] = 1;
				st_var.n_temp_top_idbuffer_exist_status_check_info[i+28] = 1;
			}
		}
	}

}


void CRun_Top_IDBuffer_Works::RecvDataConvert()
{
	memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_SEND], sizeof(st_idbuffer_info_param));
		memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_SEND], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_SEND]));
}

void CRun_Top_IDBuffer_Works::SendDataConvert()
{
	memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], sizeof(st_idbuffer_info_param));
		memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP]));
	memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], sizeof(st_idbuffer_info_param));
		memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2]));

	memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], sizeof(st_idbuffer_info_param));
		memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1]));
	memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], sizeof(st_idbuffer_info_param));
		memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY]));
}

void CRun_Top_IDBuffer_Works::InPutConvertModuleData( EIDBUFFER_POS nPos)
{
	if( nPos < SHIFT_IDBUFF_INPUT_INDEX || nPos > SHIFT_IDBUFF_INDEX_ULD) return;

	EIDBUFFER_POS idPos = EIDBUFFER_POS(nPos + 1); //EIDBUFFER_POS 변환
	EIDBUFFER_POS eTargetPos = IDPOS_NONE;
	if( g_Tray.IsEnablePos( idPos ) )
	{
		eTargetPos = idPos;
	}

	if( eTargetPos != IDPOS_NONE )
	{
		ATray tray;
		tray.SetPos( eTargetPos );
		tray.SetTrayInfo( g_lotMgr.GetLotIDAt(0), 0, false, false );
		
		for( int i=0; i<MAX_MODULE; i++ )
		{
			CString strProductID="";
			strProductID.Format("%s", st_idbuff_info[idPos].c_product_id[i]);			
			CString strLotID="";
			strLotID.Format("%s", st_idbuff_info[idPos].c_lot_id[i]);
			CString strSkip="";
			strSkip.Format("%d", st_idbuff_info[idPos].n_skip_flag[i]);
			CString strArraySN="";
			strArraySN.Format("%02d", st_idbuff_info[idPos].n_idbuff_num[i]);
			CString strSerial="";
			strSerial.Format( "%s", st_idbuff_info[idPos].c_serial[i] );
			
			CString strVar="";
			strVar.Format( "%s", st_idbuff_info[idPos].c_var[i] );
			// 모듈 셋
			AModule mdl;
 			mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
			tray.SetModule(i, mdl);
		}
		
		tray.SetTargetPos( eTargetPos );
		g_Tray.AddTray( tray );

	}	
}