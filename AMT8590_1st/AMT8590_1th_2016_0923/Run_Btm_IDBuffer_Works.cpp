// Run_Btm_IDBuffer_Works.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Btm_IDBuffer_Works.h"

#include "AMTRegistry.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"
#include "AMTLotManager.h"
#include "Run_Smema_Ctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Btm_IDBuffer_Works

IMPLEMENT_SERIAL(CRun_Btm_IDBuffer_Works, CObject, 1);	// 복구 동작을 위함...
CRun_Btm_IDBuffer_Works g_Run_Btm_IDBuffers;

CRun_Btm_IDBuffer_Works::CRun_Btm_IDBuffer_Works()
{
	Thread_Variable_Initial();
}

CRun_Btm_IDBuffer_Works::~CRun_Btm_IDBuffer_Works()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_Btm_IDBuffer_Works message handlers
void CRun_Btm_IDBuffer_Works::Thread_Variable_Initial()
{
	int i=0, j=0, k=0;
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;
	mn_InitRetry = 0;

	m_fbStopperUpdn = FB_NONE; 
	for(i = 0; i < 2; i++){
		m_fbHookFwdBwd[i] = FB_NONE;
		m_step_hookfwdbwd[i] = ESTEP_CYL_START;

		m_step_uldclamp[i] = ESTEP_CYL_START;
		m_fbUldClamp[i] = FB_NONE;
		
		st_work.mn_site_uld_hookclamp[i] = IO_OFF;
		st_work.mn_site_uld_hookunclamp[i] = IO_OFF;		
		st_work.mn_site_uldclamp[i] = IO_OFF;
		st_work.mn_site_uldunclamp[i] = IO_OFF;		
	}

	for (i = 0; i < 2; i++){
		for ( j = 0; j < 2; j++){
			for ( k =0; k < 2; k++){
				st_sync.mn_btm_ibuffer_work[i][j][k] = CTL_NO;
			}
			st_handler.mn_move_buffer[i][j] = CTL_NO;
			st_handler.mn_pickup_elv[i][j] = CTL_NO;
		}
	}

	m_step_recvTray = RECV_BTM_IDTRAY_NONE;
}



void CRun_Btm_IDBuffer_Works::Thread_Run()
{
	if( Func.OnIsInterruptCondition() != 1 )
		return;
	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
// 		return;
		if(st_handler.n_only_work_density == 1 )
		{
			break;
		}

 		Run_Move();
		break;
	
	case dRESTORE:
		Run_Restore();
		break;
		
	default:
		if (st_handler.mn_menu_num != 501)
		{
			
		}
		
		break;
	}

	if(st_handler.n_only_work_density == 1 )
	{
	}
	else
	{
		Run_Move_StopperUpdn();
		for (int i = 0; i < HOOKCLAMP_CNT - 1; i++)//2014.1016 HOOKCLAMP_CNT -> HOOKCLAMP_CNT -1
	//	for (int i = 0; i < HOOKCLAMP_CNT; i++)
		{
			Run_Move_HookFwdBwd(i);
			Run_Move_UldClamp(i);//2014.1016
		}
	}



}

void CRun_Btm_IDBuffer_Works::Run_Initial()
{
	int nRet = 0;
	int i = 0;

	if( st_handler.mn_init_state[INIT_BTM_IDBUFFER] != CTL_NO )	return;
	switch(mn_InitStep)
	{
	case EBRI_START: 
		g_ioMgr.set_out_bit( st_io.o_front_btm_machine_stop_signal, IO_ON);
		mn_InitStep = EBRI_CHECK_ROBOT_CHK;	break;

	case EBRI_CHECK_ROBOT_CHK: EBRI_Check_Robot(); break;

	case EBRI_IDBUFFER_EMPTY_CHK: EBRI_IDBuffer_Empty_Chk(); break;

	case EBRI_IDBUFFER_HOOK_ULDCLAMP: EBRI_IDBuffer_HOOK_UldClamp(); break;

	case EBRI_IDBUFFER_HOOK_ULDCLAMP_CHK: EBRI_IDBuffer_HOOK_UldClamp_Chk(); break;

	case EBRI_IDBUFFER_ULDCLAMP:	EBRI_IDBuffer_UldClamp(); break;
		
	case EBRI_IDBUFFER_ULDCLAMP_CHK:	EBRI_IDBuffer_UldClamp_Chk();	break;
		
	case EBRI_IDBUFFER_ULDCLAMP_CHK_AGAIN:	EBRI_IDBuffer_UldUnClamp_Chk_Again();	break;


	case EBRI_HOMECHK_BTMID_ULD1: EBRI_HomeChk_ULD1(); break;

	case EBRI_MONE_SAFETY_UNLOAD1:	EBRI_Move_Safety_Unload1(); break;

	case EBRI_HOMECHK_BTMID_ULD2: EBRI_HomeChk_ULD2(); break;

	case EBRI_MONE_SAFETY_UNLOAD2:	EBRI_Move_Safety_Unload2(); break;

// 	case EBRI_IDBUFFER_ULDUNCLAMP:	EBRI_IDBuffer_UldUnClamp(); break;
// 
// 	case EBRI_IDBUFFER_ULDUNCLAMP_CHK:	EBRI_IDBuffer_UldUnClamp_Chk();	break;

// 	case EBRI_IDBUFFER_ULDCLAMP:	EBRI_IDBuffer_UldClamp(); break;
// 
// 	case EBRI_IDBUFFER_ULDCLAMP_CHK:	EBRI_IDBuffer_UldClamp_Chk();	break;
// 
// 	case EBRI_IDBUFFER_ULDCLAMP_CHK_AGAIN:	EBRI_IDBuffer_UldUnClamp_Chk_Again();	break;
		
	case EBRI_FINISH:	EBRI_Finish();	break;
	}
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_Empty_Chk()
{
	int nRet1 = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer1_chk, IO_ON );
	int nRet2 = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer2_chk, IO_ON );
	int nRet3 = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer3_chk, IO_ON );
	int nRet4 = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer4_chk, IO_ON );
	int nRet5 = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer3_chk, IO_ON );
	int nRet6 = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer4_chk, IO_ON );


	if(nRet1 == IO_OFF && nRet2 == IO_OFF)
	{
	}
	else if( (nRet1 ^ nRet2) == IO_ON)
	{
	}

	mn_InitStep = EBRI_IDBUFFER_HOOK_ULDCLAMP;
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_HOOK_UldClamp()
{
	ReqHookFwdBwd(EBHS_HS_ULD1, BACKWARD);
	ReqHookFwdBwd(EBHS_HS_ULD2, BACKWARD);
	mn_InitStep = EBRI_IDBUFFER_HOOK_ULDCLAMP_CHK;
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_HOOK_UldClamp_Chk()
{
	if(GetHookFwdBwd(EBHS_HS_ULD1) != FB_NONE)	return;
	if(GetHookFwdBwd(EBHS_HS_ULD2) != FB_NONE)	return;

	//mn_InitStep = EBRI_HOMECHK_BTMID_ULD1;
	mn_InitStep = EBRI_IDBUFFER_ULDCLAMP;//2014.1016
}

void CRun_Btm_IDBuffer_Works::ReqHookFwdBwd(int nSite, EFB efb )
{
	m_fbHookFwdBwd[nSite] = efb;
	m_step_hookfwdbwd[nSite] = ESTEP_CYL_START;
}

void CRun_Btm_IDBuffer_Works::EBRI_HomeChk_ULD1()
{
	int nRet = COMI.HomeCheck_Mot( M_BTMID_ULD1, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EBRI_MONE_SAFETY_UNLOAD1;
		Func.OnInitBtn( RBTN_BTMID_ULD_1 );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//350001 0 00 "Bottom ID Buffr ULD1 Axis Motor Home Check Error."
		CString strJamCode = "350001";
		CTL_Lib.Alarm_Error_Occurrence(300, CTL_dWARNING, strJamCode );
	}
}

void CRun_Btm_IDBuffer_Works::EBRI_Move_Safety_Unload1()
{
	int nRet_1 = CTL_Lib.Single_Move(M_BTMID_ULD1, st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_SAFETY], st_basic.nManualSpeed);
	if (nRet_1 == BD_GOOD)
	{			
		mn_InitStep = EBRI_HOMECHK_BTMID_ULD2;
	}
	else if (nRet_1 == BD_RETRY)
	{		
	}
	else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
	{
		CTL_Lib.Alarm_Error_Occurrence(301, CTL_dWARNING, COMI.mc_alarmcode);
	}
}

void CRun_Btm_IDBuffer_Works::EBRI_HomeChk_ULD2()
{
	int nRet = COMI.HomeCheck_Mot( M_BTMID_ULD2, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EBRI_MONE_SAFETY_UNLOAD2;
		Func.OnInitBtn( RBTN_BTMID_ULD_2 );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//360001 0 00 "Bottom ID Buffr ULD2 Axis Motor Home Check Error."
		CString strJamCode = "360001";
		CTL_Lib.Alarm_Error_Occurrence(302, CTL_dWARNING, strJamCode );
	}
}

void CRun_Btm_IDBuffer_Works::EBRI_Move_Safety_Unload2()
{
	int nRet_1 = CTL_Lib.Single_Move(M_BTMID_ULD2, st_motor[M_BTMID_ULD2].d_pos[P_LDBUFFER_SAFETY], st_basic.nManualSpeed);
	if (nRet_1 == BD_GOOD)
	{			
		//mn_InitStep = EBRI_IDBUFFER_ULDUNCLAMP;
		mn_InitStep = EBRI_FINISH;//EBRI_IDBUFFER_ULDCLAMP;//2014.1016
	}
	else if (nRet_1 == BD_RETRY)
	{		
	}
	else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
	{
		CTL_Lib.Alarm_Error_Occurrence(303, CTL_dWARNING, COMI.mc_alarmcode);
	}
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_UldUnClamp()
{
	ReqUldClamp( EBHS_HS_ULD12, BACKWARD );
	ReqUldClamp( EBHS_HS_ULD34, BACKWARD );
	mn_InitStep = EBRI_IDBUFFER_ULDUNCLAMP_CHK;
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_UldUnClamp_Chk()
{
	if( GetUldClamp( EBHS_HS_ULD12 ) != FB_NONE ) return;
	if( GetUldClamp( EBHS_HS_ULD34 ) != FB_NONE	) return;

	//mn_InitStep = EBRI_IDBUFFER_ULDCLAMP;
	mn_InitStep = EBRI_HOMECHK_BTMID_ULD1;//2014.1016
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_UldUnClamp_Chk_Again()
{
	if( GetUldClamp( EBHS_HS_ULD12 ) != FB_NONE ) return;
	if( GetUldClamp( EBHS_HS_ULD34 ) != FB_NONE	) return;
	
	mn_InitStep = EBRI_HOMECHK_BTMID_ULD1;//2014.1016
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_UldClamp()
{
	ReqUldClamp( EBHS_HS_ULD12, FORWARD );
	ReqUldClamp( EBHS_HS_ULD34, FORWARD );
	mn_InitStep = EBRI_IDBUFFER_ULDCLAMP_CHK;
}

void CRun_Btm_IDBuffer_Works::ReqUldClamp( int nSite, EFB efb )
{
	m_fbUldClamp[nSite] = efb;
	m_step_uldclamp[nSite] = ESTEP_CYL_START;
}

void CRun_Btm_IDBuffer_Works::EBRI_IDBuffer_UldClamp_Chk()
{
	if( GetUldClamp( EBHS_HS_ULD12 ) != FB_NONE ) return;
	if( GetUldClamp( EBHS_HS_ULD34 ) != FB_NONE	) return;


	bool bUnClamp = false;
	if( GetUldClamp( EBHS_HS_ULD12 ) == FB_NONE )
	{
		if( CheckUldClamp(EBHS_HS_ULD12, IO_ON) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][0]) == IO_ON &&
				g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][1]) == IO_ON )
			{

				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_skip_flag[CTL_LEFT] = CTL_YES;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_skip_flag[CTL_RIGHT] = CTL_YES;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[CTL_LEFT] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[CTL_RIGHT] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuff_num[CTL_RIGHT] = 10;				
				
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].c_product_id[CTL_LEFT], "%s", "01");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].c_product_id[CTL_RIGHT], "%s", "01");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].c_var[CTL_LEFT], "%s", "02");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].c_var[CTL_RIGHT], "%s", "02");


// 				ATray tray;
// 				tray.SetPos( IPOOS_UNLOAD_BTM_LOAD );
// 				for( int i=0; i<MAX_MODULE; i++ )
// 				{
// 					CString strProductID = "0";				
// 					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
// 					CString strSkip = "1";
// 					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
// 					CString strSerial = "";
// 					CString strBIN = "";
// 					CString strVar = "1";
// 					
// 					// ¡E¡þ¨I¡I¡Iia ¡§uA
// 					AModule mdl;
// 					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
// 					tray.SetModule(i, mdl);
// 				}	
// 				tray.SetTargetPos( IPOOS_UNLOAD_BTM_LOAD );
// 				g_Tray.AddTray( tray );
			}
			else if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][0]) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][1]) == IO_ON )
			{
				mn_InitRetry++;
				if(mn_InitRetry > 3)
				{
					mn_InitRetry = 0;
					CString strError;
					if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][0]) == IO_OFF )
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][0], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 381, dWARNING, strError );
					}
					else
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][1], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 382, dWARNING, strError );
					}
				}
				else
				{
					mn_InitStep = EBRI_IDBUFFER_HOOK_ULDCLAMP;
					CString strError;
					if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][0]) == IO_OFF )
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][0], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 381, dWARNING, strError );
					}
					else
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD12][1], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 382, dWARNING, strError );
					}
				}
			}
			else
			{
				ReqUldClamp( EBHS_HS_ULD12, BACKWARD );
				bUnClamp = true;
			}

		}

	}
	else return;

	if( GetUldClamp( EBHS_HS_ULD34 ) == FB_NONE	)
	{
		if( CheckUldClamp(EBHS_HS_ULD34, IO_ON) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][0]) == IO_ON &&
				g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][1]) == IO_ON )
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_skip_flag[CTL_LEFT] = CTL_YES;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_skip_flag[CTL_RIGHT] = CTL_YES;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[CTL_LEFT] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[CTL_RIGHT] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuff_num[CTL_RIGHT] = 10;				
				
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].c_product_id[CTL_LEFT], "%s", "01");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].c_product_id[CTL_RIGHT], "%s", "01");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].c_var[CTL_LEFT], "%s", "02");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].c_var[CTL_RIGHT], "%s", "02");
// 				ATray tray;
// 				tray.SetPos( IPOOS_UNLOAD_BTM_UNLOAD );
// 				for( int i=0; i<MAX_MODULE; i++ )
// 				{
// 					CString strProductID = "0";				
// 					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
// 					CString strSkip = "1";
// 					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
// 					CString strSerial = "";
// 					CString strBIN = "";
// 					CString strVar = "1";
// 					
// 					// ¸ðμa ¼A
// 					AModule mdl;
// 					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
// 					tray.SetModule(i, mdl);
// 				}	
// 				tray.SetTargetPos( IPOOS_UNLOAD_BTM_UNLOAD );
// 				g_Tray.AddTray( tray );
			}
			else if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][0]) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][1]) == IO_ON )
			{
				mn_InitRetry++;
				if(mn_InitRetry > 3)
				{
					mn_InitRetry = 0;
					CString strError;
					if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][0]) == IO_OFF )
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][0], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 383, dWARNING, strError );
					}
					else
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][1], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 384, dWARNING, strError );
					}
				}
				else
				{
					mn_InitStep = EBRI_IDBUFFER_HOOK_ULDCLAMP;
					CString strError;
					if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][0]) == IO_OFF )
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][0], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 383, dWARNING, strError );
					}
					else
					{
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_chk[EBHS_HS_ULD34][1], IO_OFF);
						CTL_Lib.Alarm_Error_Occurrence( 384, dWARNING, strError );
					}
				}
			}
			else
			{
				ReqUldClamp( EBHS_HS_ULD34, BACKWARD );
				bUnClamp = true;
			}
		}

	}
	else return;

	mn_InitStep = EBRI_HOMECHK_BTMID_ULD1;
	if( bUnClamp == true)
		mn_InitStep = EBRI_IDBUFFER_ULDCLAMP_CHK_AGAIN;
}

void CRun_Btm_IDBuffer_Works::Run_Move_HookFwdBwd(int nSite)
{
	if( m_fbHookFwdBwd[nSite] == FB_NONE ) return;
	int iOnOff = IO_ON;
	if( m_fbHookFwdBwd[nSite] == BACKWARD )
		iOnOff = IO_OFF;

	switch( m_step_hookfwdbwd[nSite] ) {
	case ESTEP_CYL_START: m_step_hookfwdbwd[nSite] = ESTEP_CYL_MOVE; break;

	case ESTEP_CYL_MOVE:
		{
			SetHookFwdBwd( nSite, iOnOff);
			if( COMI.mn_run_status == dRUN ){
				st_work.mn_site_uld_hookfwd[nSite] = iOnOff;
				st_work.mn_site_uld_hookbwd[nSite] = !iOnOff;
			}

			m_step_hookfwdbwd[nSite] = ESTEP_CYL_CHK;
			m_dwTimeHookFwdBwdStart[nSite] = GetCurrentTime();			
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckHookFwdBwd( nSite, iOnOff ) )
			{
				m_step_hookfwdbwd[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeHookFwdBwd[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeHookFwdBwdStart[nSite] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Hook_Clamp_Time(st_io.i_btm_idbuffer_shifter_hook_fwd[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 304, dWARNING, strJamCode );
				}				
			}
			else if( GetCurrentTime() - m_dwTimeHookFwdBwdStart[nSite] < 0 )
			{
				m_dwTimeHookFwdBwdStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeHookFwdBwd[nSite] < 0 )
			{
				m_dwTimeHookFwdBwd[0] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeHookFwdBwd[nSite] < st_time.n_wait_time[E_WAIT_HOOK_CLAMP][iOnOff] )
				return;

			if(COMI.mn_run_status == dINIT)
				if( GetCurrentTime() - m_dwTimeHookFwdBwd[nSite] < 1000 ) return;


			m_fbHookFwdBwd[nSite] = FB_NONE;
			m_step_hookfwdbwd[nSite] = ESTEP_CYL_START;
		}
	}
}

void CRun_Btm_IDBuffer_Works::SetHookFwdBwd( int nSite, int iOnOff )
{
	int iHook_1 = st_io.o_btm_idbuffer_shifter_hook_fwd[nSite];
	int iHook_2 = st_io.o_btm_idbuffer_shifter_hook_bwd[nSite];
	g_ioMgr.set_out_bit( iHook_1, iOnOff );
	g_ioMgr.set_out_bit( iHook_2, !iOnOff );
}

bool CRun_Btm_IDBuffer_Works::CheckHookFwdBwd( int nSite, int iOnOff)
{
	int iHook_Clamp_1 = 0, iHook_UnClamp_1 = 0;
	int iHook_Clamp_2 = 0, iHook_UnClamp_2 = 0;

	if(nSite == EBHS_HS_ULD1)
	{
		iHook_Clamp_1 = st_io.i_btm_idbuffer_shifter_hook_fwd[nSite];
		iHook_UnClamp_1 = st_io.i_btm_idbuffer_shifter_hook_bwd[nSite];
	}
	else
	{

		iHook_Clamp_1 = st_io.i_btm_idbuffer_shifter_hook_fwd[nSite];
		iHook_UnClamp_1 = st_io.i_btm_idbuffer_shifter_hook_bwd[nSite];
		iHook_Clamp_2 = st_io.i_btm_idbuffer_shifter_hook_fwd[nSite+1];
		iHook_UnClamp_2 = st_io.i_btm_idbuffer_shifter_hook_bwd[nSite+1];
	}

	if(nSite == EBHS_HS_ULD1)
	{
		if( g_ioMgr.get_in_bit( iHook_Clamp_1 ) != iOnOff)		return false;
		if( g_ioMgr.get_in_bit( iHook_UnClamp_1) != !iOnOff)	return false;
	}
	else
	{
		if( g_ioMgr.get_in_bit( iHook_Clamp_1 ) != iOnOff)		return false;
		if( g_ioMgr.get_in_bit( iHook_UnClamp_1) != !iOnOff)	return false;
		if( g_ioMgr.get_in_bit( iHook_Clamp_2 ) != iOnOff)		return false;
		if( g_ioMgr.get_in_bit( iHook_UnClamp_2) != !iOnOff)	return false;
	}

	return true;
}

void CRun_Btm_IDBuffer_Works::EBRI_Finish()
{
	for ( int i = 0; i < 2; i++ )
	{
		if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[i][0]) == IO_ON )
			st_sync.mn_btm_ibuffer_work[BTBUFFER][i][0] = CTL_YES;			
		if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_chk[i][1]) == IO_ON )
			st_sync.mn_btm_ibuffer_work[BTBUFFER][i][1] = CTL_YES;
	}
	if ( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer3_chk, IO_OFF) == IO_ON ||
		g_ioMgr.get_in_bit( st_io.i_btm_idbuffer4_chk, IO_OFF) == IO_ON)
	{
		g_ioMgr.set_out_bit( st_io.o_front_btm_machine_stop_signal, IO_ON);
	}

	Func.OnInitBtn( RBTN_BTMID_ULD );//2014.1018
	st_handler.mn_init_state[INIT_BTM_IDBUFFER] = CTL_YES;
	mn_InitStep = EBRI_START;
}

void CRun_Btm_IDBuffer_Works::ReqStopperUpdn( EFB efb )
{
	m_fbStopperUpdn = efb;
	m_step_stopperupdn = ESTEP_CYL_START;
}

void CRun_Btm_IDBuffer_Works::SetStopperUpdn( int iOnOff )
{
	int iUpdn = st_io.o_btm_idbuffer_stopper_updown;
	g_ioMgr.set_out_bit( iUpdn, iOnOff );
}

bool CRun_Btm_IDBuffer_Works::CheckStopperUpdn( int iOnOff )
{
	int iUp = 0, iDn = 0;

	iUp = st_io.i_btm_idbuffer_stopper_up_chk;
	iDn = st_io.i_btm_idbuffer_stopper_down_chk;

	if( g_ioMgr.get_in_bit( iDn) != iOnOff)		return false;
	if( g_ioMgr.get_in_bit( iUp ) != !iOnOff)	return false;

	return true;
}

void CRun_Btm_IDBuffer_Works::Run_Move_StopperUpdn()
{
	if( m_fbStopperUpdn == FB_NONE)	return;

	int iOnOff = IO_OFF;
	if( m_fbStopperUpdn == BACKWARD )
		iOnOff = IO_ON;

	switch( m_step_stopperupdn ) {
	case ESTEP_CYL_START:	m_step_stopperupdn = ESTEP_CYL_MOVE;	break;

	case ESTEP_CYL_MOVE:
		{
			SetStopperUpdn( iOnOff);
			if( COMI.mn_run_status == dRUN )
			{
				st_work.mn_btm_stopper = iOnOff;				
			}

			m_step_stopperupdn = ESTEP_CYL_CHK;
			m_dwTimeStopperUpdnStart = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckStopperUpdn(iOnOff) == true )
			{
				m_step_stopperupdn = ESTEP_CYL_FINISH;
				m_dwTimeStopperUpdn = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeStopperUpdnStart > st_time.n_limit_time[E_WAIT_UNLOAD_PICKER] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode;
					strJamCode.Format("5%04d%d",st_io.i_btm_idbuffer_stopper_up_chk, iOnOff);
					CTL_Lib.Alarm_Error_Occurrence( 305, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeStopperUpdnStart < 0 )
			{
				m_dwTimeStopperUpdnStart = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeStopperUpdn < 0 )
			{
				m_dwTimeStopperUpdn = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeStopperUpdn < st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][iOnOff])
				return;

			m_fbStopperUpdn = FB_NONE;
			m_step_stopperupdn = ESTEP_CYL_START;
		}
		break;	
	}
}

void CRun_Btm_IDBuffer_Works::Run_Move_UldClamp(int nSite)
{
//	if( m_fbUldClamp[nSite] != FB_NONE)	return;
	if( m_fbUldClamp[nSite] == FB_NONE)	return;//2014.1016 != -> ==

	int iOnOff = IO_ON;
	if( m_fbUldClamp[nSite] == BACKWARD )
//		iOnOff = BACKWARD;
		iOnOff = IO_OFF;//2014.1016 BACKWARD -> IO_OFF

	switch( m_step_uldclamp[nSite] )
	{
	case ESTEP_CYL_START:	m_step_uldclamp[nSite] = ESTEP_CYL_MOVE;	break;

	case ESTEP_CYL_MOVE:
		{
			SetUldClamp( nSite, iOnOff);
			if( COMI.mn_run_status == dRUN )
			{
				st_work.mn_site_uldclamp[nSite] = iOnOff;
				st_work.mn_site_uldunclamp[nSite] = !iOnOff;
			}

			m_step_uldclamp[nSite] = ESTEP_CYL_CHK;
			m_dwTimeUldClampStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckUldClamp(nSite, iOnOff) == true )
			{
				m_step_uldclamp[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeUldClamp[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeUldClampStart[nSite] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_btm_idbuffer_clamp[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 306, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeUldClampStart[nSite] < 0 )
			{
				m_dwTimeUldClampStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeUldClamp[nSite] <= 0 )
			{
				m_dwTimeUldClamp[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeUldClamp[nSite] < st_time.n_wait_time[E_WAIT_HOOK_CLAMP][iOnOff])
				return;

			if( COMI.mn_run_status == dINIT )
				if( GetCurrentTime() - m_dwTimeUldClamp[nSite] < 5000 ) return;

			m_fbUldClamp[nSite] = FB_NONE;
			m_step_uldclamp[nSite] = ESTEP_CYL_START;
		}
		break;	
	}
}

void CRun_Btm_IDBuffer_Works::SetUldClamp( int nSite, int iOnOff )
{
	int iUld_1 = st_io.o_btm_idbuffer_clamp[nSite];
	int iUld_2 = st_io.o_btm_idbuffer_unclamp[nSite];

	g_ioMgr.set_out_bit( iUld_1, iOnOff );
	g_ioMgr.set_out_bit( iUld_2, !iOnOff );
}

bool CRun_Btm_IDBuffer_Works::CheckUldClamp( int nSite, int iOnOff )
{
	int iUld_1 = st_io.i_btm_idbuffer_clamp[nSite];
	int iUld_2 = st_io.i_btm_idbuffer_unclamp[nSite];

	if( g_ioMgr.get_in_bit( iUld_1 ) != iOnOff)		return false;
	if( g_ioMgr.get_in_bit( iUld_2 ) != !iOnOff)	return false;

	return true;
}

void CRun_Btm_IDBuffer_Works::EBRI_Check_Robot()
{
// 	if( st_handler.mn_init_state[INIT_IDBUFFER_EL] == CTL_NO ) return;
	mn_InitStep = EBRI_IDBUFFER_EMPTY_CHK;
}

void CRun_Btm_IDBuffer_Works::Run_Move()
{
	Run_Move_Btm_RecvIDBuffer();
	Run_Move_Btm_SendIDBuffer();
}

void CRun_Btm_IDBuffer_Works::ReqRecvTray()
{
	if( m_step_recvTray == RECV_BTM_IDTRAY_NONE )
		m_step_recvTray = RECV_BTM_IDTRAY_READY;
}

void CRun_Btm_IDBuffer_Works::Run_Move_Btm_RecvIDBuffer()
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

	Func.ThreadFunctionStepTrace(30, m_step_recvTray);

	switch( m_step_recvTray )
	{
	case RECV_BTM_IDTRAY_NONE:	break;
		
	case RECV_BTM_IDTRAY_READY:
		{
			if( CheckHookFwdBwd( EHS_HFB_LD1, IO_OFF) &&
				CheckUldClamp(EHS_LCS_LD12, IO_OFF))
			{
				m_step_recvTray = RECV_BTM_IDTRAY_SAFETY;
				break;
			}
			ReqHookFwdBwd( EHS_HFB_LD1, BACKWARD );
			ReqUldClamp( EHS_LCS_LD12, BACKWARD );
			m_step_recvTray = RECV_BTM_IDTRAY_READY_CHK;
		}
		break;

	case RECV_BTM_IDTRAY_READY_CHK:
		{
			if( GetHookFwdBwd(EHS_HFB_LD1) != FB_NONE )		return;
			if( GetUldClamp( EBHS_HS_ULD12 ) != FB_NONE )	return;

			m_step_recvTray = RECV_BTM_IDTRAY_SAFETY;
		}
		break;

	case RECV_BTM_IDTRAY_SAFETY://2014,1205
		{
// 			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD2, st_motor[ M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY], (int)st_basic.nRunSpeed );
// 
// 			double dpos = COMI.Get_MotCurrentPos(M_BTMID_ULD2);
// 
// 			if(dpos < )
// 			if( nRet == BD_GOOD )
// 			{
//// 				m_step_recvTray = RECV_BTM_IDTRAY_RECV_READY;//2014,1205
// 			}
// 			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 306, CTL_dWARNING, alarm.mstr_code );
// 			}

			if(  g_ioMgr.get_in_bit(st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk, IO_OFF) == IO_ON || 
				g_ioMgr.get_in_bit(st_io.i_btm_idbuffer_uld1_shifter_buffer2_chk, IO_OFF) == IO_ON )
			{
				CString strError;
				if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk, IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d",st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk, IO_ON);
				}
				else
				{
					strError.Format("5%04d%d",st_io.i_btm_idbuffer_uld1_shifter_buffer2_chk, IO_ON);
				}
				CTL_Lib.Alarm_Error_Occurrence( 713, CTL_dWARNING, strError );
				break;
			}
			m_step_recvTray = RECV_BTM_IDTRAY_RECV_READY;

		}
		break;

	case RECV_BTM_IDTRAY_RECV_READY:
		//가지고 간다면	
		if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer1_chk, IO_OFF) == IO_ON || g_ioMgr.get_in_bit(st_io.i_btm_idbuffer2_chk, IO_OFF) == IO_ON )
		{
			CString strError;
			if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer1_chk, IO_OFF) == IO_ON)
			{
				strError.Format("5%04d%d",st_io.i_btm_idbuffer1_chk, IO_ON);
			}
			else
			{
				strError.Format("5%04d%d",st_io.i_btm_idbuffer2_chk, IO_ON);
			}
			CTL_Lib.Alarm_Error_Occurrence( 713, CTL_dWARNING, strError );
			break;
		}

  		st_handler.mn_pickup_elv[0][0] = CTL_REQ;
		m_step_recvTray = RECV_BTM_IDTRAY_RECV_READY_CHK;
		break;


	case RECV_BTM_IDTRAY_RECV_READY_CHK:
		{
			if( st_handler.mn_pickup_elv[1][0] != CTL_READY) break;

			if( COMI.Get_MotCurrentPos( M_IDBUFFER_EL ) > st_motor[M_IDBUFFER_EL].d_pos[P_DOWNBUFFER] + COMI.md_allow_value[M_IDBUFFER_EL] )
			{//320008 0 00 "ID Buffer EL Axis Motor is Non Safety Pos."
				CTL_Lib.Alarm_Error_Occurrence( 307, CTL_dWARNING, "320008" );
				break;
			}

			int nRet_1 = COMI.Check_MotPosRange(M_BTMID_ULD2, st_motor[ M_BTMID_ULD2].d_pos[P_ULDBUFFER_PICK], COMI.md_allow_value[M_BTMID_ULD2]);
			if (nRet_1 == BD_GOOD)
			{
				m_step_recvTray = RECV_BTM_IDTRAY_CLAMP;
				m_dwTimeRecv = GetCurrentTime();
			}
			else
			{
				m_step_recvTray = RECV_BTM_IDTRAY_RECV_READY_POS;
			}
		}
		break;

	case RECV_BTM_IDTRAY_RECV_READY_POS:
		{
			
			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD2, st_motor[ M_BTMID_ULD2].d_pos[P_ULDBUFFER_PICK], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_recvTray = RECV_BTM_IDTRAY_CLAMP;
				m_dwTimeRecv = GetCurrentTime();
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 307, CTL_dWARNING, alarm.mstr_code );
			}
		}

		break;

	case RECV_BTM_IDTRAY_CLAMP:
		{
			ReqHookFwdBwd( EHS_HFB_LD1, FORWARD );
			m_step_recvTray = RECV_BTM_IDTRAY_CLAMP_CHK;
		}
		break;

	case RECV_BTM_IDTRAY_CLAMP_CHK:
		{
			if( GetHookFwdBwd(EHS_HFB_LD1) != FB_NONE )			return;

 			Run_Smema_Ctl.btm_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_INDEX_ULD_CLAMP);
			int nRet_1 = btm_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_ULD_CLAMP);
			if(nRet_1 == CTL_GOOD)
			{	
				Run_Smema_Ctl.btm_idbuffer_datainfo_shift(1, SHIFT_IDBUFF_INDEX_ULD_CLAMP); //data clear 
				if ( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP].n_exist[CTL_LEFT]	== CTL_YES ||
					st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP].n_exist[CTL_RIGHT] == CTL_YES )
				{
				}
				m_step_recvTray = RECV_BTM_IDTRAY_MOVE;//RECV_BTM_IDTRAY_CLAMP_FREE;
				st_handler.mn_pickup_elv[0][1] = CTL_CHANGE;
				st_handler.mn_pickup_elv[0][0] = CTL_NO;
			}			
		}
		break;


	case RECV_BTM_IDTRAY_MOVE:
		{
			if(st_handler.mn_pickup_elv[1][1] != CTL_READY) break;
			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD2, st_motor[ M_BTMID_ULD2].d_pos[P_ULDBUFFER_ULD], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_recvTray = RECV_BTM_IDTRAY_LOAD_CLAMP;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 308, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case RECV_BTM_IDTRAY_LOAD_CLAMP:
		ReqUldClamp( EHS_LCS_LD12, FORWARD );
		m_step_recvTray = RECV_BTM_IDTRAY_LOAD_CLAMP_CHK;
		m_dwTimeRecv = GetCurrentTime();

//		Run_Smema_Ctl.btm_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_INDEX_ULD_LOAD); 
//		Run_Smema_Ctl.btm_idbuffer_datainfo_shift(1, SHIFT_IDBUFF_INDEX_ULD_LOAD); 

		//2014,1206 확인후 위로 올린다.
		st_handler.mn_pickup_elv[0][0] = CTL_FREE;
		st_handler.mn_pickup_elv[0][1] = CTL_NO;

		break;

	case RECV_BTM_IDTRAY_LOAD_CLAMP_CHK:
		{
			if( GetUldClamp( EBHS_HS_ULD12 ) != FB_NONE ) return;
//			ReqHookFwdBwd( EHS_HFB_LD1, BACKWARD );
//			m_step_recvTray = RECV_BTM_IDTRAY_CHECK;
//			m_dwTimeRecv = GetCurrentTime();

			//2014,1126
//			if( ( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer1_chk, IO_ON) == IO_OFF ||
//				g_ioMgr.get_in_bit( st_io.i_btm_idbuffer2_chk, IO_ON) == IO_OFF) )
//			{
//				CString strError;
//				if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer1_chk, IO_ON) == IO_OFF)
//					strError.Format( "5%04d%d", st_io.i_btm_idbuffer1_chk, IO_OFF);
//				else
//					strError.Format( "5%04d%d", st_io.i_btm_idbuffer2_chk, IO_OFF);
//
//				CTL_Lib.Alarm_Error_Occurrence( 998, dWARNING,strError );
//				break;
//			}
//
//			if(	btm_idbuffer_sensor_flag_status_check(0 , SHIFT_IDBUFF_INDEX_ULD_LOAD) != CTL_GOOD)
//				break; 
			
			//2014,1206
// 			Run_Smema_Ctl.btm_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_INDEX_ULD_LOAD);
// 			int nRet_1 = btm_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_ULD_LOAD);
// 			if(nRet_1 == CTL_GOOD)
// 			{	
// 				Run_Smema_Ctl.btm_idbuffer_datainfo_shift(1, SHIFT_IDBUFF_INDEX_ULD_LOAD); //data clear 
// 				if ( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_exist[CTL_LEFT]	== CTL_YES ||
// 					st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_exist[CTL_RIGHT] == CTL_YES )
// 				{
// 				}

				ReqHookFwdBwd( EHS_HFB_LD1, BACKWARD );
				m_step_recvTray = RECV_BTM_IDTRAY_CHECK;
				m_dwTimeRecv = GetCurrentTime();

// 				//2014,1206 E®AIEA A§·I ¿A¸°´U.
// 				st_handler.mn_pickup_elv[0][0] = CTL_FREE;
// 				st_handler.mn_pickup_elv[0][1] = CTL_NO;
// 			}
		}
		break;

	case RECV_BTM_IDTRAY_CHECK://2014,1205
		{
			if( GetHookFwdBwd(EHS_HFB_LD1) != FB_NONE )			return;

//			SetIDBufferBoxExist(IPOOS_UNLOAD_BTM_LOAD);

			//2014,1206
			Run_Smema_Ctl.btm_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_INDEX_ULD_LOAD);
			int nRet_1 = btm_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_ULD_LOAD);
			if(nRet_1 == CTL_GOOD)
			{	
				Run_Smema_Ctl.btm_idbuffer_datainfo_shift(1, SHIFT_IDBUFF_INDEX_ULD_LOAD); //data clear 
				if ( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_exist[CTL_LEFT]	== CTL_YES ||
					st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_exist[CTL_RIGHT] == CTL_YES )
				{
				}				
				m_step_recvTray = RECV_BTM_IDTRAY_FINISH;
			}

// 			if( ( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk, IO_ON) == IO_OFF ||
// 				g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_uld1_shifter_buffer2_chk, IO_ON) == IO_OFF) )
// 			{
// 				CString strError;
// 				if( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk, IO_ON) == IO_OFF)
// 					strError.Format( "5%04d%d", st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk, IO_OFF);
// 				else
// 					strError.Format( "5%04d%d", st_io.i_btm_idbuffer_uld1_shifter_buffer2_chk, IO_OFF);
// 				
// 				CTL_Lib.Alarm_Error_Occurrence( 998, dWARNING,strError );
// 				break;
// 			}

		}
		break;

	case RECV_BTM_IDTRAY_FINISH:
		{
// 			st_handler.mn_pickup_elv[0][0] = CTL_FREE;
// 			st_handler.mn_pickup_elv[0][1] = CTL_NO;
			m_step_recvTray = RECV_BTM_IDTRAY_NONE;	
			
			if(CheckHookFwdBwd(EHS_HFB_LD1,IO_OFF))
			{
				COMI.Start_SingleMove( M_BTMID_ULD2, st_motor[ M_BTMID_ULD2].d_pos[P_ULDBUFFER_PICK], (int)st_basic.nRunSpeed );
			}

			if( GetCurrentTime() - m_dwTimeInit  < 0)  m_dwTimeInit = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeInit >= 0 && GetCurrentTime() - m_dwTimeInit < 900000 &&
				st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[BTM_ULD]Tact time : %d", GetCurrentTime() - m_dwTimeInit);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//				Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
			}


			m_dwTimeInit = GetCurrentTime();

		}
		break;
	}
}

void CRun_Btm_IDBuffer_Works::Run_Move_Btm_SendIDBuffer()
{
	int nRet_1=0, nRet_2=0, nRet_3=0, nRet_4=0, nRet_5=0, nRet_6=0;
	long lMotionDone=0;
	double dwCurrentPos =0;

	if( m_step_sendTray_old != m_step_sendTray )
	{
		m_step_sendTray_old = m_step_sendTray;
		m_dwTime_SendTray = GetCurrentTime();

//		if( Run_Move_Chk_Jam_SendTray() == false )
//			return;
	}

//	if( Run_Move_Chk_Time_SendTray() == false )
// 		return;

	Func.ThreadFunctionStepTrace(31, m_step_sendTray);

	switch( m_step_sendTray )
	{
	case SEND_BTM_IDTRAY_NONE:	break;

	case SEND_BTM_IDTRAY_READY:
		{
			ReqHookFwdBwd(EHS_HFB_LD2, BACKWARD);
			ReqUldClamp(EHS_LCS_LD34, BACKWARD );

			m_step_sendTray = SEND_BTM_IDTRAY_READY_CHK;
		}
		break;

	case SEND_BTM_IDTRAY_READY_CHK:
		{
			if( GetHookFwdBwd(EHS_HFB_LD2) != FB_NONE )			return;
			if( GetUldClamp( EHS_LCS_LD34 ) != FB_NONE ) return;
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_SAFETY;
		}
		break;

	case SEND_BTM_IDTRAY_ULD2_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_ULDBUFFER_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_sendTray = SEND_BTM_IDTRAY_SEND_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 309, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case SEND_BTM_IDTRAY_SEND_READY:
		if( st_basic.n_mode_device && ( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer3_chk, IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_btm_idbuffer4_chk, IO_OFF) == IO_ON || 
			g_ioMgr.get_in_bit(st_io.i_btm_idbuffer1_chk, IO_ON) == IO_OFF || 
			g_ioMgr.get_in_bit(st_io.i_btm_idbuffer2_chk, IO_ON) == IO_OFF ) )
		{
			CString strError;
			if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer3_chk, IO_OFF) == IO_ON)
			{
				strError.Format("5%04d%d",st_io.i_btm_idbuffer3_chk, IO_ON);
			}
			else if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer4_chk, IO_OFF) == IO_ON)
			{
				strError.Format("5%04d%d",st_io.i_btm_idbuffer4_chk, IO_ON);
			}
			else if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer1_chk, IO_OFF) == IO_ON)
			{
				strError.Format("5%04d%d",st_io.i_btm_idbuffer1_chk, IO_OFF);
			}
			else{
				strError.Format("5%04d%d",st_io.i_btm_idbuffer2_chk, IO_OFF);
			}
			CTL_Lib.Alarm_Error_Occurrence( 713, CTL_dWARNING, strError );

			EIDBUFFER_POS eTargetPos = IDPOS_NONE;
			if( g_Tray.IsEnablePos( IDPOS_FRONT_TO_DENSITY ) )
			{
				//eTargetPos = IDPOS_FRONT_TO_DENSITY;
			}
			
			break;
		}
		m_step_sendTray = SEND_BTM_IDTRAY_ULD2_PICKMOVE;
		break;

	case SEND_BTM_IDTRAY_ULD2_PICKMOVE:
		{
			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_LDBUFFER_PICK], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_sendTray = SEND_BTM_IDTRAY_ULD2_FORWARD;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 310, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case SEND_BTM_IDTRAY_ULD2_FORWARD:
		{
			ReqHookFwdBwd(EHS_HFB_LD2, FORWARD);
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_FORWARD_CHK;
			m_dwTime_SendTray = GetCurrentTime();
		}
		break;

	case SEND_BTM_IDTRAY_ULD2_FORWARD_CHK:
		{
			if( GetHookFwdBwd(EHS_HFB_LD2) != FB_NONE )			return;

			ReqUldClamp(EHS_LCS_LD12, BACKWARD );
			m_step_sendTray = SEND_BTM_IDTRAY_ULD1_FORWARD_CHK;
			m_dwTime_SendTray = GetCurrentTime();
		}
		break;

	case SEND_BTM_IDTRAY_ULD1_FORWARD_CHK:
		if( GetUldClamp( EHS_LCS_LD12 ) != FB_NONE ) return;
		m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
		break;

	case SEND_BTM_IDTRAY_ULD2_ULDMOVE:
		{
			if(CheckUldClamp( EHS_LCS_LD12, IO_OFF) != true)
			{
				CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_btm_idbuffer_clamp[EHS_LCS_LD12], IO_OFF );
				CTL_Lib.Alarm_Error_Occurrence( 307, dWARNING, strJamCode );
				break;
			}
			//2015.0520
// 			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_ULDBUFFER_ULD], (int)st_basic.nRunSpeed );
// 			if( nRet == BD_GOOD )
// 			{
// 				m_step_sendTray = SEND_BTM_IDTRAY_ULD2_CLAMP_ON;
// 			}
// 			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 311, CTL_dWARNING, alarm.mstr_code );
// 			}
			int nRet = COMI.Start_SingleMove(M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_ULDBUFFER_ULD], (int)st_basic.nRunSpeed);  // 해당 위치로 이동
			if (nRet == BD_GOOD)
			{
				m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE_READY;
			}
			else if (nRet == BD_RETRY)
			{
				m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
			}
			else if (nRet == BD_ERROR || nRet == BD_SAFETY)
			{
				COMI.mn_run_status = dWARNING;  
				CTL_Lib.Alarm_Error_Occurrence(308, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
				m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
			}
		
		}
		break;

	case SEND_BTM_IDTRAY_ULD2_ULDMOVE_READY:
		nRet_1 = COMI.Check_SingleMove(M_BTMID_ULD1, st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_ULD]);  // 이동 완료 확인
		if(nRet_1 == BD_PROCEED)
		{
			nRet_2 = g_comiMgr._cmmSxIsDone(M_BTMID_ULD1, &lMotionDone); //index move가 이동중이고 //모션 동작 완료 == 1, 동작중은 0
			dwCurrentPos = g_comiMgr.Get_MotCurrentPos(M_BTMID_ULD1);
			if(nRet_2 == cmERR_NONE &&  lMotionDone != cmTRUE) //문제 없이 이동중이고 
			{
				if(dwCurrentPos > st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_PICK] + 200)//left 위치보다 -200mm이상 작으면 아직 우측에 다 도착하지 않았으면 조건을 체크한다 
				{
					m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE_CHK;
					break;
				}
			}
		}
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{					
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE_CHK;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(309, COMI.mn_run_status, COMI.mc_alarmcode);
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
		}
		break;

	case SEND_BTM_IDTRAY_ULD2_ULDMOVE_CHK:
		nRet_1 = COMI.Check_SingleMove(M_BTMID_ULD1, st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_ULD]);  // 이동 완료 확인
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{					
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_CLAMP_ON;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(309, COMI.mn_run_status, COMI.mc_alarmcode);
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_ULDMOVE;
		}
		break;
		

	case SEND_BTM_IDTRAY_ULD2_CLAMP_ON:
		{
			ReqUldClamp(EHS_LCS_LD34, FORWARD );
			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_CLAMP_ON_CHK;
			m_dwTime_SendTray = GetCurrentTime();
		}
		break;

	case SEND_BTM_IDTRAY_ULD2_CLAMP_ON_CHK:
		{
			if( GetUldClamp( EHS_LCS_LD34 ) != FB_NONE ) return;
			ReqStopperUpdn(BACKWARD);
			ReqHookFwdBwd( EHS_HFB_LD2, BACKWARD );

			m_step_sendTray = SEND_BTM_IDTRAY_ULD2_CLAMP_OFF;

		}
		break;

	case SEND_BTM_IDTRAY_ULD2_CLAMP_OFF:
		{
			if( GetStopperUpdn() != FB_NONE )				return;
			if( GetHookFwdBwd(EHS_HFB_LD2) != FB_NONE )			return;

			
			Run_Smema_Ctl.btm_idbuffer_datainfo_shift( 0, SHIFT_IDBUFF_INDEX_ULD_UNLOAD);
			Run_Smema_Ctl.btm_idbuffer_datainfo_shift( 1, SHIFT_IDBUFF_INDEX_ULD_UNLOAD);
			m_step_sendTray = SEND_BTM_IDTRAY_MOVE_FINISH;

		}
		break;

	case SEND_BTM_IDTRAY_MOVE_FINISH:
		{
//			g_Tray.TrayMoved( EIDBUFFER_POS(IPOOS_UNLOAD_BTM_LOAD), EIDBUFFER_POS(IPOOS_UNLOAD_BTM_UNLOAD) );
// 			g_Tray.TrayMoved( EIDBUFFER_POS(IPOOS_UNLOAD_BTM_UNLOAD), EIDBUFFER_POS(IDPOS_BTM_UNLOAD_TO_NEXT) );
// 			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_BTM_UNLOAD_TO_NEXT), EIDBUFFER_POS(IDPOS_NEXT) );

// 			int nRet = CTL_Lib.Single_Move( M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_ULDBUFFER_SAFETY], (int)st_basic.nRunSpeed );
			int nRet = COMI.Start_SingleMove(M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_ULDBUFFER_SAFETY], (int)st_basic.nRunSpeed * 2 );
			if( nRet == BD_GOOD )
			{
				if( btm_idbuffer_sensor_flag_status_check( 0, SHIFT_IDBUFF_INDEX_ULD_UNLOAD) != CTL_GOOD)
					break;

				m_step_sendTray = SEND_BTM_IDTRAY_FINISH;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 308, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case SEND_BTM_IDTRAY_FINISH:
		{
			int nRet = COMI.Check_SingleMove(M_BTMID_ULD1, st_motor[ M_BTMID_ULD1].d_pos[P_ULDBUFFER_SAFETY] );
			if( nRet == BD_GOOD )
			{
				m_step_sendTray = SEND_BTM_IDTRAY_NONE;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				m_step_sendTray = SEND_BTM_IDTRAY_MOVE_FINISH;
				CTL_Lib.Alarm_Error_Occurrence( 308, CTL_dWARNING, alarm.mstr_code );
			}
			
		}
		break;

	}

}
void CRun_Btm_IDBuffer_Works::ReqSendTray()
{
	if( m_step_sendTray == SEND_BTM_IDTRAY_NONE )
		m_step_sendTray = SEND_BTM_IDTRAY_READY;

}

void CRun_Btm_IDBuffer_Works::Run_Restore()
{
}

void CRun_Btm_IDBuffer_Works::OnRobot_FinalPos()
{
}

void CRun_Btm_IDBuffer_Works::Serialize(CArchive &ar)
{
}

int CRun_Btm_IDBuffer_Works::btm_idbuffer_sensor_flag_status_check(int n_mode, int n_check_site) //센서와 플레그가 틀리면 에러 
{
	int i=0;
	int nFunRet = CTL_GOOD;
	int nRet_1=0, nRet_2=0;
	char mc_jamcode[10];

	memset(mc_jamcode, 0, sizeof(mc_jamcode));

	if(st_basic.n_mode_device == 0 && st_handler.mn_virtual_idbuffer == 0)//
	{
		return CTL_GOOD;
	}

	for(i=0; i<6; i++)
	{
		if	   (i == 0)	st_var.n_btm_idbuffer_exist_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[CTL_LEFT];
		else if(i == 1) st_var.n_btm_idbuffer_exist_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[CTL_RIGHT];
		else if(i == 2) st_var.n_btm_idbuffer_exist_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[CTL_LEFT];
		else if(i == 3) st_var.n_btm_idbuffer_exist_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[CTL_RIGHT];
		else if(i == 4) st_var.n_btm_idbuffer_exist_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP].n_idbuffer[CTL_LEFT];
		else if(i == 5) st_var.n_btm_idbuffer_exist_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP].n_idbuffer[CTL_RIGHT];
		
		st_var.n_btm_idbuffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_btm_idbuffer_exist_sensor_check[i], IO_OFF);
		if( n_check_site == SHIFT_IDBUFF_INDEX_ULD_LOAD )
		{
			if( i == 0 || i == 1 )
			{
				if(st_var.n_btm_idbuffer_exist_check_info[i] != st_var.n_btm_idbuffer_io_status_read_chk_data[i])
				{
					sprintf(mc_jamcode, "5%04d%d", st_io.i_btm_idbuffer_exist_sensor_check[i], IO_OFF); 	 
					CTL_Lib.Alarm_Error_Occurrence( 330, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}
			
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_ULD_UNLOAD )
		{
			if( i == 2 || i == 3 )
			{
				if(st_var.n_btm_idbuffer_exist_check_info[i] != st_var.n_btm_idbuffer_io_status_read_chk_data[i])
				{
					sprintf(mc_jamcode, "5%04d%d", st_io.i_btm_idbuffer_exist_sensor_check[i], IO_OFF); 	 
					CTL_Lib.Alarm_Error_Occurrence( 331, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}

			}
		}		
		else if(n_check_site == SHIFT_IDBUFF_INDEX_ULD_CLAMP )
		{
			if( i == 4 || i == 5 )
			{
				if(st_var.n_btm_idbuffer_exist_check_info[i] != st_var.n_btm_idbuffer_io_status_read_chk_data[i])
				{
					sprintf(mc_jamcode, "5%04d%d", st_io.i_btm_idbuffer_exist_sensor_check[i], IO_OFF); 	 
					CTL_Lib.Alarm_Error_Occurrence( 331, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}

			}
		}		
		else if(n_check_site == SHIFT_IDBUFF_SITE_ALL_CHK)
		{
//			if( i == 0 || i == 1 || i== 2 || i == 3 )
//			{
//				if(st_var.n_btm_idbuffer_exist_check_info[i] != st_var.n_btm_idbuffer_io_status_read_chk_data[i])
//				{
//					sprintf(mc_jamcode, "5%04d%d", st_io.n_btm_idbuffer_io_status_read_chk_data[i], IO_OFF); 
//					CTL_Lib.Alarm_Error_Occurrence( 332, CTL_dWARNING, mc_jamcode );
//					nFunRet = CTL_ERROR;
//					break;
//				}
//			}
		}
	}
	return nFunRet;

}


void CRun_Btm_IDBuffer_Works::PreIDBufferBoxExist( EIDBUFFER_POS pos )
{
	int i = 0;
	for ( i = 0; i < 4; i++)
	{
		st_var.n_temp_btm_idbuffer_exist_status_check_info[i] = 0;
	}
	
	if( pos == IPOOS_UNLOAD_BTM )
	{
		int iIdx1 = g_Tray.GetIdxByPos(IPOOS_UNLOAD_BTM);
		if( iIdx1 >= 0 )
		{
			for ( i = 0; i < MAX_BUFFDVC; i++)
			{
				if( g_Tray.GetTray(iIdx1).GetModule(i).IsIDBufferExist())
				{
					st_var.n_temp_btm_idbuffer_exist_status_check_info[2+i] = 1;
				}
			}
		}
	}
	else if( pos == IPOOS_UNLOAD_BTM_LOAD )
	{
		int iIdx2 = g_Tray.GetIdxByPos(IPOOS_UNLOAD_BTM_UNLOAD);
		if( iIdx2 >= 0 )
		{
			for ( i = 0; i < MAX_BUFFDVC; i++)
			{
				st_var.n_temp_btm_idbuffer_exist_status_check_info[i] = 1;
			}
		}
	}
}

void CRun_Btm_IDBuffer_Works::SetIDBufferBoxExist( EIDBUFFER_POS pos )
{
	int i = 0;
	if( pos == IPOOS_UNLOAD_BTM_LOAD )
	{
		for ( i = 2; i < 4; i++)
		{
			st_var.n_btm_idbuffer_exist_check_info[i] = st_var.n_temp_btm_idbuffer_exist_status_check_info[i];
		}
	}
	else if( pos == IPOOS_UNLOAD_BTM_UNLOAD )
	{
		for ( i = 0; i < 2; i++)
		{
			st_var.n_btm_idbuffer_exist_check_info[i] = st_var.n_temp_btm_idbuffer_exist_status_check_info[i];
		}
	}
}

