// Run_Density_Feeders.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Density_Feeders.h"
#include "AMTLotManager.h"
#include "AMTRegistry.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"


#include "VisionClient.h"
#include "VisionServer.h"

#include "Cmmsdk.h"
#include "Public_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Density_Feeders

IMPLEMENT_SERIAL(CRun_Density_Feeders, CObject, 1)
CRun_Density_Feeders g_Run_Density_Feeders;

CRun_Density_Feeders::CRun_Density_Feeders()
{
	Thread_Variable_Initial();
}

CRun_Density_Feeders::~CRun_Density_Feeders()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Density_Feeders message handlers
void CRun_Density_Feeders::Thread_Variable_Initial()
{
	int i = 0, j = 0, m = 0, l = 0;
	mn_InitStep = 0;
	mn_RestoreStep = 0;
	mn_device_type = 0;

	mn_RunStep = 0;
	m_nInterfaceStep = 0;
	
	for ( i = 0; i < MAX_EDP; i++ )
	{
		m_step_dfeeder[i] = ESTEP_CYL_START;
		m_fbDFeeder[i] = FB_NONE;
		m_bPrint_Pickup[i] = false;
		m_nRun_Print[i] = 0;
	}

	for ( j = 0; j < 6; j++)
	{
		m_nLabelResult[j] = CTL_FREE;//초기 비젼 세팅
		m_nRetryPickOut[j] = 0;
		g_ioMgr.set_out_bit( st_io.o_density_lable_start[i], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[i], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_density_lable_length1[i], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_density_lable_length2[i], IO_OFF);
	}

	for ( m = 0; m < 6; m++ )
	{
		for ( l = 0; l < 4; l++)
		{
			memset(st_density_info[m][l].m_cDen_capa, 0x00, sizeof(st_density_info[m][i].m_cDen_capa));
			memset(st_density_info[m][l].m_cDen_2D, 0x00, sizeof(st_density_info[m][i].m_cDen_2D));
			memset(st_density_info[m][l].m_cDen_mdl, 0x00, sizeof(st_density_info[m][i].m_cDen_mdl));
			
			st_density_info[m][l].nResult = EMBS_FAIL;
		}
		memset( &st_den_feeder[m], 0x00, sizeof(st_density_feeder) );
	}
	for( i = 0; i < 6; i++)
	{
		m_continus_vis_error[i] = 0;
	}
}

void CRun_Density_Feeders::Thread_Run()
{
	if( Func.OnIsInterruptCondition() != 1 )
		return;
	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
		{
			Run_DLabel_Print123(st_basic.mn_device_type);
			Run_DLabel_Print123(st_basic.mn_device_type+3);
			Run_Move();
			Run_Move_Label(st_basic.mn_device_type);
			Run_Move_Label(st_basic.mn_device_type+3);
		}
		break;
	
	case dRESTORE:
//		Run_Restore();
		break;
		
	default:
		if (st_handler.mn_menu_num != 501)
		{
			
		}
		
		break;
	}

//	if(COMI.mn_run_status != dINIT)//2014.0105
//	{
//		Run_DLabel_Print123(st_basic.mn_device_type);
//		Run_DLabel_Print123(st_basic.mn_device_type+3);
//	}
	

// 	for ( int i = 0; i < 6; i++)
// 	{
//		Run_Move_Feeder_Reel(i);//2014.1018 cable 작업 미완료
// 		Run_Move_Feeder_Lock(i);
// 	}

}

void CRun_Density_Feeders::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_DENDITY_FEEDER] != CTL_NO )	return;
	switch(mn_InitStep)
	{
	case EDEI_START:	mn_InitStep = EDEI_CHECK_ROBOT_CHK; break;
		
	case EDEI_CHECK_ROBOT_CHK: EDEI_Check_Rbt_Chk(); break;


	case EDEI_HOMECHK_VISION_X: EDEI_HomeCheck_Vision_X(); break;
	case EDEI_MOVE_SAFETY_X:  EDEI_MoveSafety_X(); break;
	case EDEI_FEEDER_UNCYLINDER: 	EDEI_Feeder_Cylinder(BACKWARD); break;
	case EDEI_FEEDER_UNCYLINDER_CHK: 	EDEI_Feeder_Cylinder_Chk( BACKWARD ); break;

	case EDEI_FEEDER_CYLINDER: 	EDEI_Feeder_Cylinder(FORWARD); break;
	case EDEI_FEEDER_CYLINDER_CHK: 	EDEI_Feeder_Cylinder_Chk(FORWARD); break;

	case EDEI_FEEDER_UNLOCK: EDEI_Feeder_LockUnlock( FORWARD ); break;//2014.1018 BACKWARD -> FORWARD

	case EDEI_FEEDER_UNLOCK_CHK:	EDEI_Feeder_LockUnloack_Chk( FORWARD ); break;//2014.1018 BACKWARD -> FORWARD

	case EDEI_FEEDER_LOCK: EDEI_Feeder_LockUnlock( BACKWARD ); break;//2014.1018 FORWARD -> BACKWARD

	case EDEI_FEEDER_LOCK_CHK: EDEI_Feeder_LockUnloack_Chk( BACKWARD ); break;

	case EDEI_FINISH:	EDEI_Finish();
	}
}

void CRun_Density_Feeders::EDEI_Check_Rbt_Chk()
{
	if( st_handler.mn_init_state[INIT_DENDITY_ROBOT] == CTL_NO ) return;

	mn_InitStep = EDEI_HOMECHK_VISION_X;
}

void CRun_Density_Feeders::EDEI_HomeCheck_Vision_X()
{
	int nRet = COMI.HomeCheck_Mot( M_D_VISION_X, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EDEI_MOVE_SAFETY_X;
		Func.OnInitBtn( RBTN_D_VISION_X );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//100006 1 00 "Density Vision X Axis Motor Move Error."
		CString strJamCode = "100006";
		CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, strJamCode );
	}
}

void CRun_Density_Feeders::EDEI_MoveSafety_X()
{	
	int nRet = CTL_Lib.Single_Move(M_D_VISION_X, st_motor[M_D_VISION_X].d_pos[P_DENSITY_SAFETY], st_basic.nManualSpeed);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EDEI_FEEDER_UNCYLINDER;
		mn_InitStep = EDEI_FINISH;
//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//100006 1 00 "Density Vision X Axis Motor Move Error."
		CString strJamCode = "100006";
		CTL_Lib.Alarm_Error_Occurrence(401, CTL_dWARNING, strJamCode );
	}
}

void CRun_Density_Feeders::EDEI_Feeder_Cylinder( EFB efb )
{
	for ( int i = 0; i < MAX_EDP; i++ )
	{
		ReqFeeder( EDP1 + i, efb);
	}

	if( efb == BACKWARD) mn_InitStep = EDEI_FEEDER_UNCYLINDER_CHK;
	else				 mn_InitStep = EDEI_FEEDER_CYLINDER_CHK;
}

void CRun_Density_Feeders::ReqFeeder( int nSite, EFB efb)
{
	m_fbDFeeder[nSite] = efb;
	m_step_dfeeder[nSite] = ESTEP_CYL_START;
}

void CRun_Density_Feeders::EDEI_Feeder_Cylinder_Chk( EFB efb )
{
	////2014.1018
	if( efb == BACKWARD)	mn_InitStep = EDEI_FEEDER_CYLINDER;
	else					mn_InitStep = EDEI_FEEDER_UNLOCK;
	return;
	////
	if( GetFeeder(EDP1) != FB_NONE) return;
	if( GetFeeder(EDP2) != FB_NONE) return;
	if( GetFeeder(EDP3) != FB_NONE) return;
	if( GetFeeder(EDP4) != FB_NONE) return;
	if( GetFeeder(EDP5) != FB_NONE) return;
	if( GetFeeder(EDP6) != FB_NONE) return;

	if( efb == BACKWARD)	mn_InitStep = EDEI_FEEDER_CYLINDER;
	else					mn_InitStep = EDEI_FEEDER_UNLOCK;

}

void CRun_Density_Feeders::Run_Move_Feeder_Reel( int nSite )
{
//	if( m_fbDFeeder[nSite] == FB_NONE ) return;
//
//	int iOnOff = IO_ON;
//	if( m_fbDFeeder[nSite] == BACKWARD)
//		iOnOff = IO_OFF;
//
//	switch( m_step_dfeeder[nSite] ) {
//	case ESTEP_CYL_START: m_step_dfeeder[nSite] = ESTEP_CYL_MOVE; break;
//
//	case ESTEP_CYL_MOVE:
//		{
//			SetFeeder( nSite, iOnOff );
//			if( COMI.mn_run_status == dRUN ){
//				st_work.mn_site_density_fwdreel[nSite] = iOnOff;
//				st_work.mn_site_density_bwdreel[nSite] = !iOnOff;
//			}
//			m_step_dfeeder[nSite] = ESTEP_CYL_CHK;
//			m_dwTimeDFeederStart[nSite] = GetCurrentTime();
//		}
//		break;
//
//	case ESTEP_CYL_CHK:
//		{
//			if( st_handler.mn_virtual_mode == 1 || CheckFeeder( nSite, iOnOff) )
//			{
//				m_step_dfeeder[nSite] = ESTEP_CYL_FINISH;
//				m_dwTimeDFeeder[nSite] = GetCurrentTime();
//			}
//			else if( GetCurrentTime() - m_dwTimeDFeederStart[nSite] > st_time.n_limit_time[E_WAIT_DREEL_PAPER_FWDBWD])
//			{
//				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
//				{
//					CString strJamCode = GetJamCode_Paper_FwdBwd_Time(st_io.i_density_paper_pusher_fwdback_chk[nSite], iOnOff );
//					CTL_Lib.Alarm_Error_Occurrence( 402, dWARNING, strJamCode );
//				}
//			}
//			else if( GetCurrentTime() - m_dwTimeDFeederStart[nSite] < 0 )
//			{
//				m_dwTimeDFeederStart[nSite] = GetCurrentTime();
//			}
//		}
//		break;
//
//	case ESTEP_CYL_FINISH:
//		{
//			if( GetCurrentTime() - m_dwTimeDFeeder[nSite] < 0 )
//				m_dwTimeDFeeder[nSite] = GetCurrentTime();
//			else if( GetCurrentTime() - m_dwTimeDFeeder[nSite] < st_time.n_wait_time[E_WAIT_DREEL_PAPER_FWDBWD][iOnOff] )
//				return;
//
//			m_fbDFeeder[nSite] = FB_NONE;
//			m_step_dfeeder[nSite] = ESTEP_CYL_START;
//		}
//		break;
//	}
}

void CRun_Density_Feeders::SetFeeder( int nSite, int iOnOff)
{
//	int nReel_1 = st_io.o_density_paper_pusher_fwdback[nSite];
//
//	g_ioMgr.set_out_bit( nReel_1, iOnOff);//2014.1022 get_out_bit -> set_out_bit
}

bool CRun_Density_Feeders::CheckFeeder( int nSite, int iOnOff )
{
//	int nReel_1 = st_io.i_density_paper_pusher_fwdback_chk[nSite];
//
//	if( g_ioMgr.get_in_bit( nReel_1) != !iOnOff) return false;//2014.1022 iOnOff -> !iOnOff
//
	return true;
}

void CRun_Density_Feeders::EDEI_Feeder_LockUnlock( EFB efb )
{
	for ( int i = 0; i < 6; i++)
	{
		ReqLock(i, efb);
	}
	if( efb == FORWARD)//2014.1018 BACKWARD - > FORWARD
		mn_InitStep = EDEI_FEEDER_UNLOCK_CHK;
	else
		mn_InitStep = EDEI_FINISH;
}

void CRun_Density_Feeders::ReqLock( int nSite, EFB efb )
{
	m_fbDlock[nSite] = efb;
	m_step_dlock[nSite] = ESTEP_CYL_START;
}

void CRun_Density_Feeders::EDEI_Feeder_LockUnloack_Chk( EFB efb )
{
	if( GetLock( EDP1 ) != FB_NONE ) return;
	if( GetLock( EDP2 ) != FB_NONE ) return;
	if( GetLock( EDP3 ) != FB_NONE ) return;
	if( GetLock( EDP4 ) != FB_NONE ) return;
	if( GetLock( EDP5 ) != FB_NONE ) return;
	if( GetLock( EDP6 ) != FB_NONE ) return;

	if( efb == BACKWARD) mn_InitStep = EDEI_FEEDER_LOCK;
	else				  mn_InitStep = EDEI_FINISH;
}

void CRun_Density_Feeders::Run_Move_Feeder_Lock( int nSite )
{
	if( m_fbDlock[nSite] == FB_NONE ) return;
	int iOnOff = IO_ON;
	if( m_fbDlock[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch( m_step_dlock[nSite]) {
	case ESTEP_CYL_START: m_step_dlock[nSite] = ESTEP_CYL_MOVE; break;
		
	case ESTEP_CYL_MOVE:
		{
			SetLock(nSite, iOnOff);
			if( COMI.mn_run_status == dRUN ){
				st_work.mn_site_density_fwdlock[nSite] = iOnOff;
				st_work.mn_site_density_bwdlock[nSite] = !iOnOff;
			}
			m_step_dlock[nSite] = ESTEP_CYL_CHK;
			m_dwTimeDFeederStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckLock( nSite, iOnOff ) ){//ybs
				m_step_dlock[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeDlock[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeDFeederStart[nSite] > st_time.n_limit_time[E_WAIT_FEEDER_LOCK] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_FeederLock_Time(st_io.o_density_lockunlock[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 403, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeDFeederStart[nSite] < 0 )
			{
				m_dwTimeDFeederStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeDlock[nSite] < 0 )
				m_dwTimeDlock[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeDlock[nSite] < st_time.n_wait_time[E_WAIT_FEEDER_LOCK][iOnOff])
				return;

			m_step_dlock[nSite] = ESTEP_CYL_START;
			m_fbDlock[nSite] = FB_NONE;
		}

	}
}

void CRun_Density_Feeders::SetLock(int nSite, int iOnOff)
{
	int nLock = st_io.o_density_lockunlock[nSite];

	g_ioMgr.set_out_bit( nLock, iOnOff);
}

bool CRun_Density_Feeders::CheckLock( int nSite, int iOnOff )
{
	int iLock = st_io.i_density_lock_chk[nSite];
	int iUnLock = st_io.i_density_unlock_chk[nSite];//2014.1018 o_density_lockunlock[nSite] -> i_density_unlock_chk[nSite]
	
	//  	if( g_ioMgr.get_in_bit( iLock ) != iOnOff ||
	//  		g_ioMgr.get_in_bit( iUnLock ) != !iOnOff ) return false;
	if( g_ioMgr.get_in_bit( iLock ) != !iOnOff ||
		g_ioMgr.get_in_bit( iUnLock ) != iOnOff ) return false;//2014.1018 sensor change

	return true;
}

void CRun_Density_Feeders::EDEI_Finish()
{
	Func.OnInitBtn( RBTN_D_FEEDER );//2014.1018
	st_handler.mn_init_state[INIT_DENDITY_FEEDER] = CTL_YES;
	mn_InitStep = EDEI_START;
}

void CRun_Density_Feeders::CommLabel( int nSite)
{
	m_cwLabelcomm[nSite] = ECW_WORK;
	m_step_labrlcomm[nSite] = ECOMM_START;
}

void CRun_Density_Feeders::Run_Move_Label( int nSite )
{
	CString strJamCode;
	if( m_cwLabelcomm[nSite] == ECW_NONE ) return;

	switch( m_step_labrlcomm[nSite] )
	{
	case ECOMM_START:
		m_nRetryCnt[nSite] = 0;
		m_step_labrlcomm[nSite] = ECOMM_MOVE;
		if( st_basic.nVisionUse[0] ) m_step_labrlcomm[nSite] = ECOMM_CONNECT;
		else{
			Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin = NVR_PASS;
			Func.m_pVsClient[EVP_DENSITY]->m_strCapa = g_lotMgr.GetLotAt(0).GetDenLabelCapa();
			CString strname = st_basic.mstr_device_name;
			strname.Replace(".TXT", "");
			Func.m_pVsClient[EVP_DENSITY]->m_strModel = strname;
			Func.m_pVsClient[EVP_DENSITY]->m_str2D = g_lotMgr.GetLotAt(0).GetDenLabelCode();
			SetLabelResult(nSite, EMBS_PASS);
			SetLabelCapa(nSite, Func.m_pVsClient[EVP_DENSITY]->m_strCapa);
			SetLabelMdl(nSite, Func.m_pVsClient[EVP_DENSITY]->m_strModel);
			SetLabel2D(nSite, Func.m_pVsClient[EVP_DENSITY]->m_str2D);
		}
		break;

	case ECOMM_CONNECT:
		if (st_client[CLS_VIS_DENSITY].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_labrlcomm[nSite] = ECOMM_INSP;
		}
		else
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CONNECT, CLS_VIS_DENSITY);
			m_step_labrlcomm[nSite] = ECOMM_CONNECT_CHK;
		}
		break;

	case ECOMM_CONNECT_CHK:
		if (st_client[CLS_VIS_DENSITY].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_labrlcomm[nSite] = ECOMM_INSP;
		}
		else
		{
			m_dwTimeNetwork[nSite][1] = GetTickCount();
			m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];

			if (m_dwTimeNetwork[nSite][2] <= 0)
			{
				m_dwTimeNetwork[nSite][0] = GetTickCount();
				break;
			}

			if (m_dwTimeNetwork[nSite][2] > st_basic.mn_network_wait_time)
			{
				m_nRetryCnt[nSite]++;
				if (m_nRetryCnt[nSite] >= 3)
				{
					m_nRetryCnt[nSite] = 0;

					// 600100 1 00 "Density_vision_time_out."
					//strJamCode.Format( "7000%d0", nSite+1);
					strJamCode = "600100";
					CTL_Lib.Alarm_Error_Occurrence(127, CTL_dWARNING, strJamCode );
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, CLS_VIS_DENSITY);
					m_step_labrlcomm[nSite] = ECOMM_CONNECT;
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, CLS_VIS_DENSITY);

					m_step_labrlcomm[nSite] = ECOMM_CONNECT_WAIT;
				}
			}
		}
		break;

	case ECOMM_CONNECT_WAIT:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeNetwork[nSite][2] > 1000)
		{
			m_step_labrlcomm[nSite] = ECOMM_CONNECT;
		}
		break;

	case ECOMM_INSP:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}
// 		if (m_dwTimeNetwork[nSite][2] < st_time.n_wait_time[E_WAIT_TRIGGER_TIME][IO_ON]) break;
// 		g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_OFF);
			
// 		Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin = NVR_NONE;
// 		Func.m_pVsClient[EVP_DENSITY]->OnVisionBcrResult(10);//2014.1118 6->10

		m_step_labrlcomm[nSite] = ECOMM_INSP_COM;
		break;

	case ECOMM_INSP_COM:
		if (Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin == NVR_PASS)
		{
			SetLabelResult(nSite, EMBS_PASS);
			SetLabelCapa(nSite, Func.m_pVsClient[EVP_DENSITY]->m_strCapa);
			SetLabelMdl(nSite, Func.m_pVsClient[EVP_DENSITY]->m_strModel);
			SetLabel2D(nSite, Func.m_pVsClient[EVP_DENSITY]->m_str2D);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, CLS_VIS_DENSITY);

			m_step_labrlcomm[nSite] = ECOMM_MOVE;
// 			m_continus_vis_error[nSite] = 0;
			g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_OFF);
		}
		else if ( Func.m_pVsClient[EVP_DENSITY]->m_nBcrResultBin == NVR_FAIL)
		{
			SetLabelResult(nSite, EMBS_FAIL);
			SetLabelCapa(nSite, Func.m_pVsClient[EVP_DENSITY]->m_strCapa);
			SetLabelMdl(nSite, Func.m_pVsClient[EVP_DENSITY]->m_strModel);
			SetLabel2D(nSite, Func.m_pVsClient[EVP_DENSITY]->m_str2D);
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, CLS_VIS_DENSITY);

			m_step_labrlcomm[nSite] = ECOMM_MOVE;

			m_continus_vis_error[nSite]++;
			if( m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt )
			{
// 				901000 0 00 "Continuous_vision_error_Density1."
// 				901001 0 00 "Continuous_vision_error_Density2."
				m_continus_vis_error[nSite] = 0;
				if(nSite == st_basic.mn_device_type)
				{
					CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, "901000" );
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, "901001" );
				}
			}
			g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_OFF);
		}
		else
		{
			m_dwTimeNetwork[nSite][1] = GetTickCount();
			m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
			
			if (m_dwTimeNetwork[nSite][2] <= 0)
			{
				m_dwTimeNetwork[nSite][0] = GetTickCount();
				break;
			}
			
			//if (m_dwTimeNetwork[nSite][2] > 2000)
			if (m_dwTimeNetwork[nSite][2] > st_basic.mn_network_wait_time)
			{
				m_nRetryCnt[nSite]++;
				if (m_nRetryCnt[nSite] >= 3)
				{
					m_nRetryCnt[nSite] = 0;
					
					// 600100 1 00 "Density_vision_time_out."
					//strJamCode.Format( "7000%d0", nSite+1);
					strJamCode = "600100";
					CTL_Lib.Alarm_Error_Occurrence(128, CTL_dWARNING, strJamCode );
					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, CLS_VIS_DENSITY);

					m_step_labrlcomm[nSite] = ECOMM_CONNECT;
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();
					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CLOSE, CLS_VIS_DENSITY);

					m_step_labrlcomm[nSite] = ECOMM_INSP_COM_WAIT;
				}
				g_ioMgr.set_out_bit(st_io.o_density_vision_trigger_onoff, IO_OFF);
			}
		}
		break;

	case ECOMM_INSP_COM_WAIT:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeNetwork[nSite][2] > 3000)
		{
			m_step_labrlcomm[nSite] = ECOMM_CONNECT;
		}
		break;
	
	case ECOMM_MOVE:
		{
			m_dwTimeCommLabel[nSite] = GetCurrentTime();
			m_step_labrlcomm[nSite] = ECOMM_FINISH;
		}
		break;

	case ECOMM_FINISH:
		if( GetCurrentTime() - m_dwTimeCommLabel[nSite] < 0 ) m_dwTimeCommLabel[nSite] = GetCurrentTime();
// 		if( GetCurrentTime() - m_dwTimeCommLabel[nSite] < 1000 ) break;//2015.0414
		if( GetLabelResult(nSite) == EMBS_PASS)
		{
			m_step_labrlcomm[nSite] = ECOMM_NONE;
			m_cwLabelcomm[nSite] = ECW_NONE;
			

			//if(st_basic.nVisionUse[0])
			//{
			
				CString strModel = st_basic.mstr_device_name;
				strModel.Replace(".TXT", "");
//  				st_handler.m_strVisModel = "X0D9UR0536";
// 				g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetDenLabelCapa("250GB");
				//if( st_basic.n_mode_device && GetLabelCapa(nSite) != g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa() )
				//{
				//2016.0919
				if( st_basic.n_mode_device && st_basic.mn_vision_density_skip == CTL_NO && GetLabelCapa(nSite) != g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa() )
				{					 

					SetLabelResult(nSite, EMBS_FAIL);
// 					st_msg.mstr_event_msg[0] = "[DENSITY VISION] Capa is different";
// 					Func.On_LogFile_Add(99,st_msg.mstr_event_msg[0]);
// 					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					if (st_handler.cwnd_list != NULL)
					{ 
						sprintf(st_msg.c_abnormal_msg, "[DENSITY VISION] Capa is different %s", GetLabelCapa(nSite));
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						Func.On_LogFile_Add(99,st_msg.c_abnormal_msg);
					}

					m_continus_vis_error[nSite]++;
					if( m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt )
					{
						m_continus_vis_error[nSite] = 0;
						if(nSite == st_basic.mn_device_type)
						{
							CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, "901000" );
						}
						else
						{
							CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, "901001" );
						}
					}

				}
				if( st_basic.n_mode_device && GetLabel2D(nSite) != g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCode() )
				{
					
					SetLabelResult(nSite, EMBS_FAIL);
// 					st_msg.mstr_event_msg[0] = "[DENSITY VISION] Density2D is different";
// 					Func.On_LogFile_Add(99,st_msg.mstr_event_msg[0]);
// 					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					if (st_handler.cwnd_list != NULL)
					{ 
						sprintf(st_msg.c_abnormal_msg, "[DENSITY VISION] Density2D is different %s", GetLabel2D(nSite));
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						Func.On_LogFile_Add(99,st_msg.c_abnormal_msg);
					}
					m_continus_vis_error[nSite]++;
					if( m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt )
					{
// 						901000 0 00 "Continuous_vision_error_Density1."
// 						901001 0 00 "Continuous_vision_error_Density2."
						m_continus_vis_error[nSite] = 0;
						if(nSite == st_basic.mn_device_type)
						{
							CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, "901000" );
						}
						else
						{
							CTL_Lib.Alarm_Error_Occurrence(400, CTL_dWARNING, "901001" );
						}
					}

				}
				if( st_basic.n_mode_device && GetLabelMdl(nSite) != strModel)
				{
					//SetLabelResult(nSite, EMBS_FAIL);
				}

				st_density_info[nSite][DENSITY_RECEIVE].nResult = GetLabelResult(nSite);

				memset(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_capa, 0x00, sizeof(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_capa));
				memset(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D, 0x00, sizeof(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D));
				memset(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_mdl, 0x00, sizeof(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_mdl));

				st_density_info[nSite][DENSITY_RECEIVE].nResult = GetLabelResult(nSite);			
				sprintf(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_capa, "%s", GetLabelCapa(nSite));
				sprintf(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D, "%s", GetLabel2D(nSite));
				sprintf(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D, "%s", GetLabelMdl(nSite));

			//}
		}
		else if( GetLabelResult(nSite) == EMBS_FAIL )
		{
			m_cwLabelcomm[nSite] = ECW_NONE;
			m_step_labrlcomm[nSite] = ECOMM_NONE;
			st_density_info[nSite][DENSITY_RECEIVE].nResult = GetLabelResult(nSite);

			memset(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_capa, 0x00, sizeof(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_capa));
			memset(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D, 0x00, sizeof(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D));
			memset(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_mdl, 0x00, sizeof(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_mdl));

			st_density_info[nSite][DENSITY_RECEIVE].nResult = GetLabelResult(nSite);			
			sprintf(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_capa, "%s", GetLabelCapa(nSite));
			sprintf(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D, "%s", GetLabel2D(nSite));
			sprintf(st_density_info[nSite][DENSITY_RECEIVE].m_cDen_2D, "%s", GetLabelMdl(nSite));
		}
		break;
	}
}

//2015.0105
void CRun_Density_Feeders::Run_Move()
{

	if(st_handler.n_only_work_density == CTL_YES)
	{
		st_sync.n_density_work_req = CTL_READY;
	}


	switch(mn_RunStep)
	{
	case 0: 
		mn_RunStep = 100;
		break;
		
	case 100:
		{
			if( st_basic.n_mode_device && ( g_lotMgr.GetLotCount() < 1 && st_handler.n_only_work_density == CTL_NO ) ) return;
			
			if(st_basic.mn_device_type <= -1 || st_basic.mn_device_type >= 3)
			{//600600 1 00 'There is no model in Density type."
				CTL_Lib.Alarm_Error_Occurrence(410, CTL_dWARNING, "600600" );
			}
			else
			{
				mn_RunStep = 200;
			}
		}
		break;


	case 200://vision1 check
		{
			double dpos = 0;

			if( st_var.n_use_density_flag[st_basic.mn_device_type] == CTL_YES ||
				st_var.n_use_density_flag[st_basic.mn_device_type+3] == CTL_YES/* ||*/
				/*st_var.mn_new_density_flag == CTL_YES ||*/
				/*st_var.mn_check_density_flag == CTL_YES ||*/
				/*st_var.n_newLabelType_flag == CTL_YES*/ )
			{
				if( st_var.n_use_density_flag[st_basic.mn_device_type] == CTL_YES ||
					st_var.n_use_density_flag[st_basic.mn_device_type+3] == CTL_YES )
				{
					st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
					st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
				}
				st_handler.mn_emptynext_cnt = CTL_YES;
			}
			else
			{
				st_handler.mn_emptynext_cnt = CTL_NO;
				if(st_var.n_use_vis_density == CTL_YES)
					st_handler.mn_emptynext_cnt = CTL_YES;
			}
			if(st_handler.mn_emptynext_cnt == CTL_YES)
			{
				dpos = st_motor[ M_D_VISION_X].d_pos[P_DENSITY_F1 + st_basic.mn_device_type];
				int nRet = CTL_Lib.Single_Move( M_D_VISION_X, dpos, st_basic.nRunSpeed );
				if( nRet == BD_GOOD )
				{
					mn_RunStep = 1000;
				}
				else if( nRet == BD_ERROR || nRet == BD_SAFETY )
				{
					CTL_Lib.Alarm_Error_Occurrence( 411, CTL_dWARNING, alarm.mstr_code );
				}
			}
			else
			{
				mn_RunStep = 1000;
			}
		}
		break;

	case 1000:
		{
			if( m_nLabelResult[st_basic.mn_device_type] == CTL_REQ )
			{
				mn_RunStep = 1100;
			}
		}
		break;

	case 1100://용량비젼 체크
		SetLabelResult(st_basic.mn_device_type,EMBS_NONE);
		if(st_handler.mn_emptynext_cnt == CTL_YES)
			CommLabel(st_basic.mn_device_type);
		mn_RunStep = 1200;
		break;

	case 1200:
		if(st_handler.mn_emptynext_cnt == CTL_NO)
		{
			st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].nResult = EMBS_PASS;
			if(st_handler.m_strVisModel != "")
			{
				sprintf(st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].m_cDen_capa, "%s", g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa());
				sprintf(st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].m_cDen_2D, "%s",  g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCode());
			}
			else
			{
				sprintf(st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].m_cDen_capa, "%s", g_lotMgr.GetLotAt(0).GetDenLabelCapa());
				sprintf(st_density_info[st_basic.mn_device_type][DENSITY_RECEIVE].m_cDen_2D, "%s",  g_lotMgr.GetLotAt(0).GetDenLabelCode());
			}

			SetLabelResult(st_basic.mn_device_type, EMBS_PASS);
			m_cwLabelcomm[st_basic.mn_device_type] = ECW_NONE;
		}
		if( m_cwLabelcomm[st_basic.mn_device_type] == ECW_NONE &&
			( GetLabelResult(st_basic.mn_device_type) == EMBS_PASS || GetLabelResult(st_basic.mn_device_type) == EMBS_FAIL ) )
		{
			//1.capa를 읽고 바코드 정보를 읽는다.
			//2.비젼결과를 읽는다.
			CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();

			m_dwTimeDlockStart[st_basic.mn_device_type] = GetCurrentTime();
			m_nLabelResult[st_basic.mn_device_type] = CTL_READY;
			/*st_var.n_use_vis_density = CTL_NO;*/			
			
			//density교체 및 new_lot
			if(st_var.n_use_density_flag[st_basic.mn_device_type] == CTL_YES)
			{
				if( GetLabelResult(st_basic.mn_device_type) == EMBS_PASS)
				{
					st_var.n_use_density_count[st_basic.mn_device_type]++;
					if( st_var.n_use_density_count[st_basic.mn_device_type] > st_basic.mn_network_retry_cnt )
					{
						st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_NO;
						st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_NO;
						st_var.n_use_density_count[st_basic.mn_device_type] = 0;
						st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;
					}
				}
				else
				{
					st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
					st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
					st_var.n_use_density_count[st_basic.mn_device_type] = 0;//reset
					st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;//reset
				}
			}
// 			if(st_var.mn_check_density_flag == CTL_YES)//over time
// 			{
// 				if( GetLabelResult(st_basic.mn_device_type) == EMBS_PASS)
// 				{
// 					st_var.mn_check_density_count++;
// 					
// 					if( st_var.mn_check_density_count > 3/*st_basic.mn_network_retry_cnt*/)
// 					{
// 						st_var.mn_check_density_count = 0;
// 						st_var.mn_check_density_flag = CTL_NO;
// 					}
// 				}
// 				else
// 				{
// 					st_var.mn_check_density_count = 0;
// 				}
// 			}

// 			if(st_var.mn_new_density_flag == CTL_YES)
// 			{
// 				if( GetLabelResult(st_basic.mn_device_type) == EMBS_PASS)
// 					st_var.mn_new_density_count++;
// 				else
// 					st_var.mn_new_density_count = 0;
// 			}
// 			if(st_var.mn_new_density_count > 3/*st_basic.mn_network_retry_cnt*/)
// 			{
// 				mn_device_type = 0;
// 				st_var.mn_new_density_flag = CTL_NO;//CTL_YES;//CTL_NO;
// 			}
			
// 			if( st_var.n_newLabelType_flag == CTL_YES )
// 			{
// 				if( GetLabelResult(st_basic.mn_device_type) == EMBS_PASS)
// 					st_var.mn_new_density_count++;
// 				else
// 					st_var.mn_new_density_count = 0;
// 
// 				if(st_var.mn_new_density_count > (2*st_basic.mn_network_retry_cnt) )
// 				{
// 					st_var.mn_new_density_count = 0;
// 					st_var.n_newLabelType_flag = CTL_NO;//CTL_YES;//CTL_NO;
// 				}
// 			}

			mn_RunStep = 2100;
		}
		break;

	case 2000:
		if( m_nLabelResult[st_basic.mn_device_type] == CTL_CHANGE )
		{
			m_nLabelResult[st_basic.mn_device_type] = CTL_FREE;
			mn_RunStep = 2100;
		}
		break;

	case 2100:
		{
			if(st_handler.mn_emptynext_cnt == CTL_YES)
			{
				if(GetCurrentTime() - m_dwTimeDlockStart[st_basic.mn_device_type] <= 0 ) m_dwTimeDlockStart[st_basic.mn_device_type] = GetCurrentTime();
				if(GetCurrentTime() - m_dwTimeDlockStart[st_basic.mn_device_type] < 200) break;
			}
// 			if( st_basic.n_mode_device && ( g_lotMgr.GetLotCount() < 1/* || g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES"*/ ) ) return;
			
			if(st_basic.mn_device_type <= -1 || st_basic.mn_device_type >= 3)
			{//600600 1 00 'There is no model in Density type."
				CTL_Lib.Alarm_Error_Occurrence(410, CTL_dWARNING, "600600" );
			}
			else
			{
				mn_RunStep = 2200;
			}
		}
		break;

	case 2200://vision1 check
		{
			double dpos = 0;

			if(st_handler.mn_emptynext_cnt == CTL_YES)
			{
				dpos = st_motor[ M_D_VISION_X].d_pos[P_DENSITY_F1 + st_basic.mn_device_type+3];
				int nRet = CTL_Lib.Single_Move( M_D_VISION_X, dpos, st_basic.nRunSpeed );
				if( nRet == BD_GOOD )
				{
					mn_RunStep = 3000;
				}
				else if( nRet == BD_ERROR || nRet == BD_SAFETY )
				{
					CTL_Lib.Alarm_Error_Occurrence( 411, CTL_dWARNING, alarm.mstr_code );
				}
			}
			else
			{
				mn_RunStep = 3000;
			}
		}
		break;

	case 3000:
		{
			if( m_nLabelResult[st_basic.mn_device_type+3] == CTL_REQ )
			{
				mn_RunStep = 3100;
			}
		}
		break;


	case 3100://용량비젼 체크
		SetLabelResult(st_basic.mn_device_type+3,EMBS_NONE);
		if(st_handler.mn_emptynext_cnt == CTL_YES)
		{
			CommLabel(st_basic.mn_device_type+3);
		}
		mn_RunStep = 3200;
		break;

	case 3200:
		if(st_handler.mn_emptynext_cnt == CTL_NO)
		{
			st_density_info[st_basic.mn_device_type+3][DENSITY_RECEIVE].nResult = EMBS_PASS;			
			sprintf(st_density_info[st_basic.mn_device_type+3][DENSITY_RECEIVE].m_cDen_capa, "%s", g_lotMgr.GetLotAt(0).GetDenLabelCapa());
			sprintf(st_density_info[st_basic.mn_device_type+3][DENSITY_RECEIVE].m_cDen_2D, "%s",  g_lotMgr.GetLotAt(0).GetDenLabelCode());
			SetLabelResult(st_basic.mn_device_type+3, EMBS_PASS);
			m_cwLabelcomm[st_basic.mn_device_type+3] = ECW_NONE;
		}
		if( m_cwLabelcomm[st_basic.mn_device_type+3] == ECW_NONE &&
		    GetLabelResult(st_basic.mn_device_type+3) == EMBS_PASS || GetLabelResult(st_basic.mn_device_type+3) == EMBS_FAIL )
		{
			//1.capa를 읽고 바코드 정보를 읽는다.
			//2.비젼결과를 읽는다.
			CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
			m_nLabelResult[st_basic.mn_device_type+3] = CTL_READY;
			m_dwTimeDlockStart[st_basic.mn_device_type+3] = GetCurrentTime();
			mn_RunStep = 4000;

			//density교체 및 new_lot
			if(st_var.n_use_density_flag[st_basic.mn_device_type+3] == CTL_YES)
			{
				if( GetLabelResult(st_basic.mn_device_type+3) == EMBS_PASS)
				{
					st_var.n_use_density_count[st_basic.mn_device_type+3]++;
					if( st_var.n_use_density_count[st_basic.mn_device_type+3] > st_basic.mn_network_retry_cnt )
					{
						st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_NO;
						st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_NO;
						st_var.n_use_density_count[st_basic.mn_device_type] = 0;
						st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;
					}
				}
				else
				{
					st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
					st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
					st_var.n_use_density_count[st_basic.mn_device_type] = 0;//reset
					st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;//reset
				}
			}
// 			if(st_var.mn_check_density_flag == CTL_YES)//over time
// 			{
// 				if( GetLabelResult(st_basic.mn_device_type+3) == EMBS_PASS)
// 				{
// 					st_var.mn_check_density_count++;
// 					
// 					if( st_var.mn_check_density_count > 3/*st_basic.mn_network_retry_cnt*/)
// 					{
// 						st_var.mn_check_density_count = 0;
// 						st_var.mn_check_density_flag = CTL_NO;
// 					}
// 				}
// 				else
// 				{
// 					st_var.mn_check_density_count = 0;
// 				}
// 			}
			
// 			if(st_var.mn_new_density_flag == CTL_YES)
// 			{
// 				if( GetLabelResult(st_basic.mn_device_type+3) == EMBS_PASS)
// 					st_var.mn_new_density_count++;
// 				else
// 					st_var.mn_new_density_count = 0;
// 			}
// 			if(st_var.mn_new_density_count > 3/*st_basic.mn_network_retry_cnt*/)
// 			{
// 				st_var.mn_new_density_count = 0;
// 				st_var.mn_new_density_flag = CTL_NO;
// 			}		
// 			if( st_var.n_newLabelType_flag == CTL_YES )
// 			{
// 				if( GetLabelResult(st_basic.mn_device_type) == EMBS_PASS)
// 					st_var.mn_new_density_count++;
// 				else
// 					st_var.mn_new_density_count = 0;
// 				
// 				if(st_var.mn_new_density_count > (2*st_basic.mn_network_retry_cnt) )
// 				{
// 					st_var.mn_new_density_count = 0;
// 					st_var.n_newLabelType_flag = CTL_NO;//CTL_YES;//CTL_NO;
// 				}
// 			}
		}
		break;

	case 4000:
		{
			double dpos = 0;
			if(st_handler.mn_emptynext_cnt == CTL_YES)
			{
				if(GetCurrentTime() - m_dwTimeDlockStart[st_basic.mn_device_type+3] <= 0 ) m_dwTimeDlockStart[st_basic.mn_device_type+3] = GetCurrentTime();
				if(GetCurrentTime() - m_dwTimeDlockStart[st_basic.mn_device_type+3] < 400) break;
			}
				
			dpos = st_motor[ M_D_VISION_X].d_pos[P_DENSITY_F1 + st_basic.mn_device_type];
			int nRet = CTL_Lib.Single_Move( M_D_VISION_X, dpos, st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				mn_RunStep = 0;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 8411, CTL_dWARNING, alarm.mstr_code );
			}
// 			}
// 			else
// 			{
// 				mn_RunStep = 0;
// 			}
		}
		break;

	}
}

int CRun_Density_Feeders::Run_DLabel_Print123( int nMotor)
{
	int n_ret = 0, nFuncRet = RET_PROCEED;
	switch(m_nRun_Print[nMotor])
	{
	case 0:
		if( st_handler.mn_manual_lable == CTL_NO && g_lotMgr.GetLotCount() < 1 && st_handler.n_only_work_density == CTL_NO) break;

		if( st_handler.mn_manual_lable == CTL_NO && COMI.mn_run_status != dRUN) break;

		//2015.0212
		if( g_lotMgr.GetLotCount() < 1 ) break;
		
		if( st_var.n_lotstart == ID_LOT_START )
		{
			m_nRetryMdlChg = 0;
			m_nRun_Print[nMotor] = 4;
// 			st_var.n_use_vis_density = CTL_NO;
// 			st_var.n_use_vis_top = CTL_NO;
// 			if (st_handler.cwnd_list != NULL)
// 			{ 
// 				sprintf(st_msg.c_normal_msg, "[3. Density Vis] Site=%d reset", nMotor +1);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 			}
		}
		else if(st_var.n_lotstart == ID_LOT_WORKING)
		{
			m_nRun_Print[nMotor] = 7;
		}
		else if ( st_var.n_lotstart == ID_LOT_END )
		{
			if(st_sync.n_density_work_req == CTL_READY)
			{
				m_nRun_Print[nMotor] = 7;
			}
// 			st_var.n_lotstart++;
		}
// 		if(st_var.n_use_density_key == CTL_YES)
// 		{
// 			st_var.n_use_density_key = CTL_NO;
// 			m_nRun_Print[nMotor] = 7;
// 			Thread_Variable_Initial();
// 		}
		break;

	case 4:
		if( st_handler.mn_model_change == CTL_YES || st_handler.mn_model_change == CTL_READY ) break;
 		if(st_handler.mn_model_change == CTL_NO)
		{

			if(st_handler.m_strVisModel != "")
			{
				if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetStrSetLabelVisionModel() == "YES")
				{
					n_ret = OnVisionModelChange(EVP_DENSITY);					
					if( n_ret == RET_GOOD)
					{
						//2015.0517
						g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetStrLabelVisionModel(false);
						m_nRetryMdlChg = 0;
						m_nRun_Print[nMotor] = 6;
					}
				}
				else
				{
					m_nRetryMdlChg = 0;
					m_nRun_Print[nMotor] = 6;
				}
			}
			else
			{
				CString strErr="";
				strErr.Format("94000%d", EVP_DENSITY+1);
				CTL_Lib.Alarm_Error_Occurrence( 4891, CTL_dWARNING, strErr );
			}
		}
// 		else
// 		{
// 			m_nRun_Print[nMotor] = 5;
// 		}
		break;

	case 5:
		if(st_basic.mn_auto_vision_mdl == CTL_YES)
		{
			if(OnVisionModelChange(EVP_BTM_POS) == RET_GOOD)
			{
				m_nRun_Print[nMotor] = 6;
			}
		}
		else
		{
			m_nRun_Print[nMotor] = 6;
		}
		break;

	case 6:
// 		if(OnVisionModelChange(EVP_BTM_POS) == RET_GOOD)
// 		{
			if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetStrStartNewLot() == "YES")
			{
				n_ret = LoadSetNewLotParam(st_handler.m_strVisModel);
				if( n_ret == CTL_GOOD )
				{
					m_nRun_Print[nMotor] = 7;
					g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetStartNewLot(false);
				}
				else
				{
					CString strError="";
					strError.Format("7000%d0",nMotor+1);
					CTL_Lib.Alarm_Error_Occurrence( 5698, dWARNING, strError );
				}
			}
			else
			{
				n_ret = LoadSetNewLotParam(st_handler.m_strVisModel);
				if( n_ret == CTL_GOOD )
				{
					m_nRun_Print[nMotor] = 7;
					g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).SetStartNewLot(false);
				}
				else
				{
					CString strError="";
					strError.Format("7000%d0",nMotor+1);
					CTL_Lib.Alarm_Error_Occurrence( 5658, dWARNING, strError );
				}
			}
// 		}
		break;		

	case 7:
		if(st_basic.nDensityFeederUse[st_basic.mn_device_type] == CTL_YES && 
			st_basic.nDensityFeederUse[st_basic.mn_device_type +3] == CTL_YES)
		{
			if(st_basic.n_mode_device == 1 )
			{
				g_ioMgr.set_out_bit( st_io.o_density_lable_start[nMotor], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[nMotor], IO_OFF);
			}
			else
			{
				g_ioMgr.set_out_bit( st_io.o_density_lable_start[nMotor], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[nMotor], IO_OFF);
				m_nRun_Print[nMotor] = 10;
				break;
			}

			if(st_basic.nLabelSelect== CTL_YES)//75mm
			{
				g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
			}
			else//55mm
			{
				g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
			}
			m_dwTimeWait[nMotor] = GetCurrentTime();
			m_nRun_Print[nMotor] = 10;
		}
		else
		{
// 			602000 1 00 "Density_Label_Feeder1_is_not_use_Please_check_basic_menu"
// 			602001 1 00 "Density_Label_Feeder2_is_not_use_Please_check_basic_menu"
// 			602002 1 00 "Density_Label_Feeder3_is_not_use_Please_check_basic_menu"
// 			602003 1 00 "Density_Label_Feeder4_is_not_use_Please_check_basic_menu"
			if(st_basic.nDensityFeederUse[st_basic.mn_device_type] != CTL_YES )
			{
				sprintf(m_cAlarm, "%d", 602000 + st_basic.mn_device_type);
				CTL_Lib.Alarm_Error_Occurrence(423, CTL_dWARNING, m_cAlarm );
			}
			else
			{
				sprintf(m_cAlarm, "%d", 602000 + st_basic.mn_device_type + 3);
				CTL_Lib.Alarm_Error_Occurrence(424, CTL_dWARNING, m_cAlarm );
			}
		}
		break;

	case 10:
		if( (st_basic.n_mode_device == 1 && 
			( st_var.n_err_density_flag[nMotor] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) == IO_ON) ) )
		{
			m_nRun_Print[nMotor] = 20;
			m_dwTimeWait[nMotor] = GetCurrentTime();
		}
		else
		{
			m_nRun_Print[nMotor] = 100;
			m_dwTimeWait[nMotor] = GetCurrentTime();
		}
		break;

	case 20:
		if( GetCurrentTime() - m_dwTimeWait[nMotor] < 0) m_dwTimeWait[nMotor] = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeWait[nMotor] > 3000 )
		{
			if( (st_basic.n_mode_device == 1 && 
				( st_var.n_err_density_flag[nMotor] == CTL_YES ||g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) == IO_ON) ) )
			{
// 				CString strError;
// 				strError.Format("7000%d0",nMotor+1);
// 				CTL_Lib.Alarm_Error_Occurrence( 5699, dWARNING, strError );
// 				m_nRun_Print[nMotor] = 10;
				st_var.n_err_density_flag[nMotor] = CTL_YES;
				if( g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) == IO_ON )
				{
					st_msg.mstr_event_msg[0].Format( "[Density Label] Label #%d error happen", nMotor +1 );
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				}
				m_nRun_Print[nMotor] = 21;
			}
			else
			{
				m_nRun_Print[nMotor] = 100;
				m_dwTimeWait[nMotor] = GetCurrentTime();
			}
		}
		if( st_basic.n_mode_device == 1 && 
			(st_var.n_err_density_flag[nMotor] == CTL_NO && g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) != IO_ON ) )
		{
			m_nRun_Print[nMotor] = 100;
			m_dwTimeWait[nMotor] = GetCurrentTime();
		}
		break;
		
	case 21:
		n_ret = CheckLotLabelError();
		if( n_ret == CTL_GOOD )
		{
			m_nRun_Print[nMotor] = 0;
		}
		else
		{
			CString strError="";
			strError.Format("7000%d0",nMotor+1);
			CTL_Lib.Alarm_Error_Occurrence( 5699, dWARNING, strError );
			m_nRun_Print[nMotor] = 0;
		}
		break;

	case 30:
		m_nRun_Print[nMotor] = 10;
		break;

	case 100:
		if( st_basic.n_mode_device == false || g_ioMgr.get_in_bit(st_io.i_density_label_ready_chk[nMotor], IO_ON ) == IO_ON )
		{
			m_nRun_Print[nMotor] = 200;
			m_dwTimeWait[nMotor] = GetCurrentTime();
		}
		else if( st_basic.n_mode_device == 1 && 
			( st_var.n_err_density_flag[nMotor] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) == IO_ON ) )
		{
// 			CString strError;
// 			strError.Format("7000%d0",nMotor+1);
// 			CTL_Lib.Alarm_Error_Occurrence( 4699, dWARNING, strError );
			m_nRun_Print[nMotor] = 30;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeWait[nMotor] < 0) m_dwTimeWait[nMotor] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeWait[nMotor] > (st_basic.mn_network_wait_time*2))
			{//700010 1 A "Density label1 time out from server."
				
// 				730010 1 00 "PS0604_PS0610_Density_label1_Ready or complete time out."
// 				730020 1 00 "PS0605_PS0611_Density_label2_Ready or complete time out."
// 				730030 1 00 "PS0606_PS0612_Density_label3_Ready or complete time out."
// 				730040 1 00 "PS0607_PS0613_Density_label4_Ready or complete time out."
// 				730050 1 00 "PS0608_PS0614_Density_label5_Ready or complete time out."
// 				730060 1 00 "PS0609_PS0615_Density_label6_Ready or complete time out."
// 				CString strError;
// 				strError.Format("7300%d0",nMotor+1);
// 				CTL_Lib.Alarm_Error_Occurrence( 5632, dWARNING, strError );
// 				m_dwTimeWait[nMotor] = GetCurrentTime();
// 				m_nRun_Print[nMotor] = 110;
				st_msg.mstr_event_msg[0].Format( "[Density Label] Label #%d is not ready or complete sgn", nMotor +1 );
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				st_var.n_err_density_flag[nMotor] = CTL_YES;
				m_nRun_Print[nMotor] = 30;
			}			
		}
		break;

	case 110:
		m_dwTimeWait[nMotor] = GetCurrentTime();
		m_nRun_Print[nMotor] = 100;
		break;

	case 200:
		if( GetCurrentTime() - m_dwTimeWait[nMotor] <= 0) m_dwTimeWait[nMotor] = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeWait[nMotor] < 100) break;
		m_nRun_Print[nMotor] = 300;
		break;

	case 300:
		m_nLabelResult[nMotor] = CTL_REQ; //비젼
		m_nRun_Print[nMotor] = 400;
		break;

	case 400:
		if(m_nLabelResult[nMotor] == CTL_READY)
		{
			st_den_feeder[nMotor].st_label.bOut = true;
			st_den_feeder[nMotor].st_label.bExist = true;

			st_den_feeder[nMotor].st_label.nState = st_density_info[nMotor][DENSITY_RECEIVE].nResult;
			sprintf(st_den_feeder[nMotor].st_label.c_mdl_id,"%s",st_density_info[nMotor][DENSITY_RECEIVE].m_cDen_mdl);
			sprintf(st_den_feeder[nMotor].st_label.c_lblCapa,"%s",st_density_info[nMotor][DENSITY_RECEIVE].m_cDen_capa);
			sprintf(st_den_feeder[nMotor].st_label.c_lbl2D,"%s",st_density_info[nMotor][DENSITY_RECEIVE].m_cDen_2D);

			m_nRun_Print[nMotor] = 500;
			st_handler.m_dwTimeOverDensity[nMotor] = GetCurrentTime();//auto out density
		}
		break;
		
	case 410:
		//2015.0219
		st_den_feeder[nMotor].st_label.bOut = true;
		st_den_feeder[nMotor].st_label.bExist = true;
		SetLabelResult(nMotor, EMBS_FAIL);
		m_nRun_Print[nMotor] = 510;
		break;
		
	case 420:
		st_den_feeder[nMotor].st_label.bOut = true;
		st_den_feeder[nMotor].st_label.bExist = true;
		SetLabelResult(nMotor, EMBS_PASS);
		m_nRun_Print[nMotor] = 510;
		break;

	case 500:
		if( st_den_feeder[nMotor].st_label.bExist == false && st_den_feeder[nMotor].st_label.bOut == false && 
			(m_nLabelResult[nMotor] == CTL_READY) )
		{
			m_nLabelResult[nMotor] = CTL_FREE;
			m_nRun_Print[nMotor] = 1000;

			g_ioMgr.set_out_bit( st_io.o_density_lable_start[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
			m_dwTimeWait[nMotor] = GetCurrentTime();

		}
		break;
		
	case 510:
		if( st_den_feeder[nMotor].st_label.bExist == false && st_den_feeder[nMotor].st_label.bOut == false )
		{
			m_nRun_Print[nMotor] = 600;
			
			g_ioMgr.set_out_bit( st_io.o_density_lable_start[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
			m_dwTimeWait[nMotor] = GetCurrentTime();
			
		}
		break;


	case 600:
		if( g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" )
		{
			if( ( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_LEFT] != 1 && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT] == 1 ) || 
				(  st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_RIGHT] != 1 && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == 1) )
			{
				if(	st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_label[CTL_LEFT].bExist == CTL_YES ||
					st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_label[CTL_RIGHT].bExist == CTL_YES)
					break;
				else
				{
					m_nRun_Print[nMotor] = 1000;
				}
			}
			else
			{
				break;
			}			
		}
		else
		{
			m_nRun_Print[nMotor] = 1000;
		}
		break;
		
	case 1000:
		if(GetCurrentTime() - m_dwTimeWait[nMotor] < 0 ) m_dwTimeWait[nMotor] = GetCurrentTime();
		if(GetCurrentTime() - m_dwTimeWait[nMotor] < 200) break;
		//2015.0530
		if( st_work.n_mat_density[nMotor] <= 1 ) st_work.n_mat_density[nMotor] = 2000;
		else                                     st_work.n_mat_density[nMotor] -= 1;

		m_nRun_Print[nMotor] = 0;
		break;
	}

	return nFuncRet;
}

int CRun_Density_Feeders::LoadSetNewLotParam(CString strLotID)
{
	int n_ret = CTL_PROCEED;
	if(g_lotMgr.GetLotCount() < 1) return n_ret;
// 	if( st_var.n_newLabelType_flag == CTL_YES)
// 	{
		bool bWork = false;
// 		Thread_Variable_Initial();

		if(g_lotMgr.GetLotByLotID(strLotID).GetDenLabelCapa() == st_handler.m_strDensityCapa[st_basic.mn_device_type])
		{
			n_ret = CTL_GOOD;
		}
		else
		{
			Thread_Variable_Initial();
			if(g_lotMgr.GetLotByLotID(strLotID).GetDenLabelCapa() == st_handler.m_strDensityCapa[0])
			{
				st_basic.mn_device_type = 0;
				if(GetDensity_LabelError(st_basic.mn_device_type) == CTL_YES)
				{
// 					st_var.n_newLabelType_flag = CTL_NO;
					n_ret = CTL_GOOD;
					bWork = true;
				}
				else
				{
					n_ret = CTL_ERROR;
				}
			}
			if(g_lotMgr.GetLotByLotID(strLotID).GetDenLabelCapa() == st_handler.m_strDensityCapa[1])
			{
				if(bWork == false)
				{
					st_basic.mn_device_type = 1;
					if(GetDensity_LabelError(st_basic.mn_device_type) == CTL_YES)
					{
// 						st_var.n_newLabelType_flag = CTL_NO;
						n_ret = CTL_GOOD;
						bWork = true;
					}
					else
					{
						n_ret = CTL_ERROR;
					}
				}
			}
			if(g_lotMgr.GetLotByLotID(strLotID).GetDenLabelCapa() == st_handler.m_strDensityCapa[2])
			{
				if(bWork == false)
				{
					st_basic.mn_device_type = 2;
					if(GetDensity_LabelError(st_basic.mn_device_type) == CTL_YES)
					{
// 						st_var.n_newLabelType_flag = CTL_NO;
						n_ret = CTL_GOOD;
						bWork = true;
					}
					else
					{
						n_ret = CTL_ERROR;
					}
				}
			}
// 			else
// 			{
// 				n_ret = CTL_ERROR;
// 			}
			if(bWork == false)
			{//940100 0 00 "There is no Density Capacity model"
				CTL_Lib.Alarm_Error_Occurrence( 4492, CTL_dWARNING, "940100" );
				n_ret = CTL_ERROR;
			}
		}

// 	}
	
	return n_ret;
}

int CRun_Density_Feeders::CheckLotLabelError()
{
	int n_ret = CTL_PROCEED;

	if(g_lotMgr.GetLotCount() < 1) return n_ret;

	Thread_Variable_Initial();
	bool bWork = false;

	

	if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa() == st_handler.m_strDensityCapa[0])
	{
		st_basic.mn_device_type = 0;
		if(GetDensity_LabelError(st_basic.mn_device_type) == CTL_YES)
		{
// 			st_var.n_newLabelType_flag = CTL_NO;
			n_ret = CTL_GOOD;
			bWork = true;
		}
		else
		{
			g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type+3], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type+3], IO_OFF);
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
				sprintf(st_msg.c_abnormal_msg, "[Density Label %d %d Error] start: off pick on: off",st_basic.mn_device_type +1, st_basic.mn_device_type +3 +1);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			n_ret = CTL_ERROR;
		}
	}
	if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa() == st_handler.m_strDensityCapa[1])
	{
		if(bWork == false)
		{
			st_basic.mn_device_type = 1;
			if(GetDensity_LabelError(st_basic.mn_device_type) == CTL_YES)
			{
// 				st_var.n_newLabelType_flag = CTL_NO;
				n_ret = CTL_GOOD;
				bWork = true;
			}
			else
			{
				g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type+3], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type+3], IO_OFF);
				if (st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재
					sprintf(st_msg.c_abnormal_msg, "[Density Label %d %d Error] start off pickon off",st_basic.mn_device_type +1, st_basic.mn_device_type +3 +1);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_ret = CTL_ERROR;
			}
		}
	}
	if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa() == st_handler.m_strDensityCapa[2])
	{
		if(bWork == false)
		{
			st_basic.mn_device_type = 2;
			if(GetDensity_LabelError(st_basic.mn_device_type) == CTL_YES)
			{
// 				st_var.n_newLabelType_flag = CTL_NO;
				n_ret = CTL_GOOD;
				bWork = true;
			}
			else
			{
				g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type+3], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type+3], IO_OFF);
				if (st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재
					sprintf(st_msg.c_abnormal_msg, "[Density Label %d %d Error] start off pickon off",st_basic.mn_device_type +1, st_basic.mn_device_type +3 +1);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_ret = CTL_ERROR;
			}
		}
	}

	return n_ret;
}

int CRun_Density_Feeders::GetDensity_LabelError(int nType)
{
	int nFuncRet = CTL_ERROR;
	if( (st_var.n_err_density_flag[nType] == CTL_NO && g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nType], IO_OFF) == IO_OFF ) &&
		(st_var.n_err_density_flag[nType+3] == CTL_NO && g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nType+3], IO_OFF) == IO_OFF ) )
	{
		if( (g_ioMgr.get_out_bit(st_io.o_density_lockunlock[st_basic.mn_device_type], IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit( st_io.i_density_lock_chk[st_basic.mn_device_type], IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_density_unlock_chk[st_basic.mn_device_type], IO_OFF) == IO_ON ) ||
			(g_ioMgr.get_out_bit(st_io.o_density_lockunlock[st_basic.mn_device_type+3], IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit( st_io.i_density_lock_chk[st_basic.mn_device_type+3], IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_density_unlock_chk[st_basic.mn_device_type+3], IO_OFF) == IO_ON ) )
		{//DOOR OPEN CHECK?
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
				sprintf(st_msg.c_abnormal_msg, "[Density LockUnlock] %d or %d Error",st_basic.mn_device_type +1, st_basic.mn_device_type+3 +1);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		else
		{
			if(st_basic.nDensityFeederUse[st_basic.mn_device_type] == CTL_YES && 
				st_basic.nDensityFeederUse[st_basic.mn_device_type +3] == CTL_YES)
			{
				nFuncRet = CTL_GOOD;
			}
			else
			{
				if(st_basic.nDensityFeederUse[st_basic.mn_device_type] != CTL_YES )
				{
					sprintf(m_cAlarm, "%d", 602000 + st_basic.mn_device_type);
					CTL_Lib.Alarm_Error_Occurrence(421, CTL_dWARNING, m_cAlarm );
				}
				else
				{
					sprintf(m_cAlarm, "%d", 602000 + st_basic.mn_device_type + 3);
					CTL_Lib.Alarm_Error_Occurrence(422, CTL_dWARNING, m_cAlarm );
				}

			}
		}

	}

	return nFuncRet;
}

int	CRun_Density_Feeders::OnVisionModelChange(int port)
{
	int nFuncRet = RET_PROCEED;
	int m_nClientPos = 10 + port;
	switch(m_nInterfaceStep)
	{
	case 0:
		if(st_client[m_nClientPos].n_connect == CTL_YES)
		{
			m_nInterfaceStep = 200;
		}
		else
		{
			m_dwTimeChangeWork = GetCurrentTime();
			m_nInterfaceStep = 100;
			
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CONNECT, m_nClientPos);	
		}
		break;
		
	case 100:
		if(st_client[m_nClientPos].n_connect == CTL_YES)
		{
			m_nInterfaceStep = 200;
		}
		else
		{	
			if(GetCurrentTime() - m_dwTimeChangeWork < 0)
			{
				m_dwTimeChangeWork = GetCurrentTime();
				break;
			}
			
			if(GetCurrentTime() - m_dwTimeChangeWork > 5000)
			{
				m_nInterfaceStep = 0;
				m_nRetryMdlChg++;
				if( m_nRetryMdlChg > st_basic.mn_network_retry_cnt)
				{
					m_nRetryMdlChg = 0;
					return RET_ERROR;
				}
			}
		}
		break;
		
	case 200:
		Func.m_pVsClient[EVP_DENSITY+port]->m_nModelChangeBin		= CTL_CLEAR;
		st_client[m_nClientPos].n_rev_info				= CTL_YES;			
		m_dwTimeChangeWork = GetCurrentTime();
		//2015.0518
// 		clsVsClient[m_nClientPos].OnVisionModelChange(m_nClientPos, st_basic.mstr_device_name);
		clsVsClient[port].OnVisionModelChange(m_nClientPos, st_basic.mstr_device_name);
		
		m_nInterfaceStep = 300;
		break;
		
	case 300:		
		if(Func.m_pVsClient[EVP_DENSITY+port]->m_nModelChangeBin	 == NVR_PASS)
		{
			m_nRetryMdlChg = 0;
			m_nInterfaceStep = 0;
			
			return RET_GOOD;
		}
		else if(Func.m_pVsClient[EVP_DENSITY+port]->m_nModelChangeBin== NVR_FAIL)
		{		
			m_nInterfaceStep = 0;
			m_nRetryMdlChg++;
			if( m_nRetryMdlChg > st_basic.mn_network_retry_cnt)
			{
				m_nRetryMdlChg = 0;
				CString strErr="";
				strErr.Format("94000%d", port+1);
				CTL_Lib.Alarm_Error_Occurrence( 4491, CTL_dWARNING, strErr );
				return RET_ERROR;
			}
		}
		else 
		{
			if(GetCurrentTime() - m_dwTimeChangeWork < 0)
			{
				m_dwTimeChangeWork = GetCurrentTime();
				break;
			}
			
			if(GetCurrentTime() - m_dwTimeChangeWork  > 5000)
			{
				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CLOSE, m_nClientPos);

// 				940001 0 00 "Density Vision model change error";
// 				940002 0 00 "Barcode Vision model change error";
// 				940003 0 00 "Top Vision model change error";
// 				940004 0 00 "Bottom Vision model change error";

				m_nRetryMdlChg++;
				if( m_nRetryMdlChg > st_basic.mn_network_retry_cnt)
				{
					m_nRetryMdlChg = 0;
					CString strErr="";
					strErr.Format("94000%d", port+1);
					CTL_Lib.Alarm_Error_Occurrence( 4491, CTL_dWARNING, strErr );
					m_nInterfaceStep = 0;					
					return RET_ERROR;
				}
				else
				{
					m_dwTimeChangeWork = GetCurrentTime();
					m_nInterfaceStep = 400;
				}
			}
		}
		break;

	case 400:
		if(GetCurrentTime() - m_dwTimeChangeWork < 0)
		{
			m_dwTimeChangeWork = GetCurrentTime();
			break;
		}
		if(GetCurrentTime() - m_dwTimeChangeWork  > 5000)
		{
			m_nInterfaceStep = 0;
		}
		break;
	}
	
	return RET_PROCEED;
}