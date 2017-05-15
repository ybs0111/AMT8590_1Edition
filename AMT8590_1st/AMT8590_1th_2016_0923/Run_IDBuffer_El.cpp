// Run_IDBuffer_El.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_IDBuffer_El.h"
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
// CRun_IDBuffer_El

IMPLEMENT_SERIAL(CRun_IDBuffer_El, CObject, 1);	// 복구 동작을 위함...

CRun_IDBuffer_El g_Run_IDBuffers_El;

CRun_IDBuffer_El::CRun_IDBuffer_El()
{
}

CRun_IDBuffer_El::~CRun_IDBuffer_El()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_IDBuffer_El message handlers
void CRun_IDBuffer_El::Thread_Variable_Initial()
{
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;

	m_step_elvclamp = ESTEP_CYL_START;
	m_step_elvmove = ESTEP_CYL_START;

	m_fbElvMoveReady = ECW_NONE;

	for (int i = 0; i <2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			st_sync.mn_ibuffer_out[i][j] = CTL_NO;
		}
		st_handler.mn_pickup_elv[i][j] = CTL_FREE;
	}
}

void CRun_IDBuffer_El::Thread_Run()
{
// 	return;
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
		Run_Move_Elv_Ready();
		Run_Move_ELvReady_IDBuffer();
		break;

	case dRESTORE:
		Run_Restore();
		break;

	default:
		break;
	
	}

	Run_Move_ElvClamp();
	Run_Move_RecvReadyIDBuffer();//2014.1018 chk in no
	Run_Move_RecvDoneIDBuffer();//2014.1018 chk in no
}

void CRun_IDBuffer_El::Run_Initial()
{
	int nRet = 0;
	int i = 0;

	if( st_handler.mn_init_state[INIT_IDBUFFER_EL] != CTL_NO )	return;
	switch(mn_InitStep)
	{
	case EIEI_START:				mn_InitStep = EIEI_CHECK_ROBOT_CHK; break;

	case EIEI_CHECK_ROBOT_CHK:		EIEI_Check_Robot();		break;

	case EIEI_IDBUFFER_CHK:			EIEI_Check_IDBufer();	break;

	case EIEI_IDBUFFER_UNCLAMP:		EIEI_IDBuffer_UnClamp();	break;

	case EIEI_IDBUFFER_UNCLAMP_CHK:	EIEI_IDBuffer_UnClamp_Chk(); break;

	case EIEI_IDBUFFER_CLAMP:		EIEI_IDBuffer_Clamp();	break;
	
	case EIEI_IDBUFFER_CLAMP_CHK:	EIEI_IDBuffer_Clamp_Chk();	break;

	case EIEI_IDBUFFER_CLAMP_AGAIN_CHK:	EIEI_IDBuffer_Clamp_Again_Chk();	break;

	case EIEI_HOMECHK_ELV:			EIEI_HomeChk_Elv();		break;

	case EIEI_MONE_SAFETY_ELV:		EIEI_Move_Safety_Elv();	break;

	case EIEI_FINISH:				EIEI_Finish(); break;

	}

}

void CRun_IDBuffer_El::EIEI_Check_Robot()
{
	// 	if( st_handler.mn_init_state[INIT_UNLOAD_ROBOT] != CTL_YES ) return;
	// 	if( st_handler.mn_init_state[INIT_TOP_IDBUFFER] != CTL_YES ) return;
	// 	if( st_handler.mn_init_state[INIT_BTM_IDBUFFER] != CTL_YES ) return;
	if( st_handler.mn_init_state[INIT_UNLOAD_ROBOT] == CTL_NO ) return;//2014.1018
	if( st_handler.mn_init_state[INIT_TOP_IDBUFFER] == CTL_NO ) return;//2014.1018 both yes -> initial no, each axis initial chk difference
	if( st_handler.mn_init_state[INIT_BTM_IDBUFFER] == CTL_NO ) return;

	mn_InitStep = EIEI_IDBUFFER_CHK;
}


void CRun_IDBuffer_El::EIEI_Check_IDBufer()
{
	int nRet1 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON );
	int nRet2 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_idbuffer2_chk, IO_ON );

	int nRet3 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_box1_chk, IO_ON );
	int nRet4 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_box2_chk, IO_ON );

	if(nRet1 == IO_OFF && nRet2 == IO_OFF)
	{
	}
//	else if(nRet1 ^ nRet2 == IO_ON)
//	{
//	}
	//	mn_InitStep = EIEI_IDBUFFER_CLAMP;

	if(nRet3 == IO_ON || nRet4 == IO_ON)
	{
		CString strError="";
		if( nRet3 == IO_ON )
		{
			strError.Format("5%04d%d",st_io.i_idbuffer_elv_box1_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence(790, dWARNING, strError);
		}
		else
		{
			strError.Format("5%04d%d",st_io.i_idbuffer_elv_box2_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence(791, dWARNING, strError);
		}
		return;
	}
	mn_InitStep = EIEI_IDBUFFER_UNCLAMP;//2014.1018
}

void CRun_IDBuffer_El::EIEI_IDBuffer_UnClamp()
{
	ReqElvClamp( BACKWARD);
	mn_InitStep = EIEI_IDBUFFER_UNCLAMP_CHK;
}

void CRun_IDBuffer_El::EIEI_IDBuffer_UnClamp_Chk()
{
	if( GetElvClamp() != FB_NONE) return;
	mn_InitStep = EIEI_IDBUFFER_CLAMP;
}

void CRun_IDBuffer_El::EIEI_IDBuffer_Clamp()
{
	ReqElvClamp( FORWARD );
	mn_InitStep = EIEI_IDBUFFER_CLAMP_CHK;
}

void CRun_IDBuffer_El::ReqElvClamp( EFB efb )
{
	m_fbElvClamp = efb;
	m_step_elvclamp = ESTEP_CYL_START;
}
void CRun_IDBuffer_El::EIEI_IDBuffer_Clamp_Chk()
{
	if( GetElvClamp() != FB_NONE) return;
	bool bUnClamp = false;
	if( GetElvClamp() == FB_NONE )
	{
		if( CheckElvClamp, IO_ON )
		{
			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk ) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer2_chk ) == IO_ON )
			{

				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[CTL_LEFT] = CTL_YES;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[CTL_RIGHT] = CTL_YES;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[CTL_RIGHT] = 10;				
				
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_product_id[CTL_LEFT], "%s", "01");
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_product_id[CTL_RIGHT], "%s", "01");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_var[CTL_LEFT], "%s", "02");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_var[CTL_RIGHT], "%s", "02");

				 st_sync.n_unload_elevator_req = CTL_CHANGE;
				

			}
			else
			{
				ReqElvClamp(BACKWARD);
				bUnClamp = true;
			}

		}

	}
	mn_InitStep = EIEI_HOMECHK_ELV;
	if( bUnClamp == true)
		mn_InitStep = EIEI_IDBUFFER_CLAMP_AGAIN_CHK;
}

void CRun_IDBuffer_El::EIEI_IDBuffer_Clamp_Again_Chk()
{
	if( GetElvClamp() != FB_NONE) return;	
	mn_InitStep = EIEI_HOMECHK_ELV;
}

void CRun_IDBuffer_El::EIEI_HomeChk_Elv()
{
	int nRet = COMI.HomeCheck_Mot( M_IDBUFFER_EL, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EIEI_MONE_SAFETY_ELV;
//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//320001 0 00 "ID Buffer EL Axis Motor Home Check Error."
		CString strJamCode = "320001";
		CTL_Lib.Alarm_Error_Occurrence(700, CTL_dWARNING, strJamCode );
	}
}

void CRun_IDBuffer_El::EIEI_Move_Safety_Elv()
{
	int nRet_1 = CTL_Lib.Single_Move(M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER], st_basic.nManualSpeed);
	if (nRet_1 == BD_GOOD)
	{			
		mn_InitStep = EIEI_FINISH;
	}
	else if (nRet_1 == BD_RETRY)
	{		
	}
	else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
	{
		CTL_Lib.Alarm_Error_Occurrence(701, CTL_dWARNING, COMI.mc_alarmcode);
	}
}

void CRun_IDBuffer_El::EIEI_Finish()
{
	{
		if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk ) == IO_ON )
			st_sync.mn_ibuffer_out[BTBUFFER][0] = CTL_YES;			
		if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer2_chk) == IO_ON )
			st_sync.mn_ibuffer_out[BTBUFFER][1] = CTL_YES;
// 		if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk ) == IO_ON )
// 			st_sync.mn_ibuffer_out[BTBOX][0] = CTL_YES;			
// 		if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk) == IO_ON )
// 			st_sync.mn_ibuffer_out[BTBOX][1] = CTL_YES;
		int nRet3 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_box1_chk, IO_ON );
		int nRet4 = g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_box2_chk, IO_ON );
		
		if(nRet3 == IO_ON || nRet4 == IO_ON)
		{
			CString strError="";
			if( nRet3 == IO_ON )
			{
				strError.Format("5%04d%d",st_io.i_idbuffer_elv_box1_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(792, dWARNING, strError);
			}
			else
			{
				strError.Format("5%04d%d",st_io.i_idbuffer_elv_box2_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(793, dWARNING, strError);
			}
			return;
		}
	}

	

	Func.OnInitBtn( RBTN_IDBUFFER_EL );//2014.1018
	st_handler.mn_init_state[INIT_IDBUFFER_EL] = CTL_YES;
	mn_InitStep = EIEI_START;
}

void CRun_IDBuffer_El::Run_Move_ElvClamp()
{
	if( m_fbElvClamp == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbElvClamp == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_elvclamp) {
	case ESTEP_CYL_START: m_step_elvclamp = ESTEP_CYL_MOVE; break;

	case ESTEP_CYL_MOVE:
		{
			SetElvClamp( iOnOff );
			if( COMI.mn_run_status == dRUN ){
				st_work.mn_site_elvclamp = iOnOff;
				st_work.mn_site_elvunclamp = !iOnOff;
			}
			m_step_elvclamp = ESTEP_CYL_CHK;
			m_dwTimeElvClampStart = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckElvClamp(iOnOff) )
			{
				m_step_elvclamp = ESTEP_CYL_FINISH;
				m_dwTimeElvClamp = GetCurrentTime();
			}
			else if ( GetCurrentTime() - m_dwTimeElvClampStart > st_time.n_limit_time[E_WAIT_HOOK_CLAMP] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_idbuffer_elv_hook_clamp_chk, iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 702, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeElvClampStart < 0 )
			{
				m_dwTimeElvClampStart = GetCurrentTime();
			}
		}
		break;
		
	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeElvClamp < 0 )
			{
				m_dwTimeElvClamp = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeElvClamp < st_time.n_wait_time[E_WAIT_HOOK_CLAMP][iOnOff])
				return;

			m_fbElvClamp = FB_NONE;
			m_step_elvclamp = ESTEP_CYL_START;
		}
		break;
	}
}

void CRun_IDBuffer_El::SetElvClamp(int iOnOff)
{
	g_ioMgr.set_out_bit( st_io.o_idbuffer_elv_hook_clamp, iOnOff);
	g_ioMgr.set_out_bit( st_io.o_idbuffer_elv_hook_unclamp, !iOnOff);
}

bool CRun_IDBuffer_El::CheckElvClamp( int iOnOff )
{
	if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_hook_clamp_chk ) != iOnOff ) return false;//2014.1018 IO No
	if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_hook_unclamp_chk ) != !iOnOff ) return false;

	return true;
}

//bool CRun_IDBuffer_El::IsBufferExist(EIDBUFFER_POS nSite)
bool CRun_IDBuffer_El::IsBufferExist()
{
	bool nRet = true;
	BOOL nExist1 = false, nExist2 = false;
	int nIoNum;
//	int nTrayIdx = g_Tray.GetIdxByPos( nSite );

//	if( nSite == IDPOS_UNLOAD )
//	{
		nIoNum = st_io.i_idbuffer_elv_idbuffer1_chk;
		nExist1 = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[0];
		nExist2 = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[1];
//	}

	if( st_basic.n_mode_device && 
		( g_ioMgr.get_in_bit( nIoNum, nExist1) != nExist1 || g_ioMgr.get_in_bit( nIoNum+1, nExist2) != nExist2 ) )
	{
		nRet = false;
	}

	return nRet;
}

void CRun_IDBuffer_El::ReqElvMoveReady(ECW ecw)
{
	m_fbElvMoveReady = ecw;
	m_step_elvmove = ESTEP_CYL_START;
}

void CRun_IDBuffer_El::Run_Move_Elv_Ready()
{
	if( m_fbElvMoveReady == ECW_NONE) return;

	switch(m_step_elvmove)
	{
	case ESTEP_CYL_START: m_step_elvmove = ESTEP_CYL_MOVE; break;

	case ESTEP_CYL_MOVE:
		{
			int nRet_1 = CTL_Lib.Single_Move(M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER], st_basic.nRunSpeed);
			if (nRet_1 == BD_GOOD)
			{			
				m_step_elvmove = ESTEP_CYL_CHK;
			}
			else if (nRet_1 == BD_RETRY)
			{		
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(701, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		break;

	case ESTEP_CYL_CHK:
		ReqElvClamp(BACKWARD);
		m_step_elvmove = ESTEP_CYL_FINISH;
		break;

	case ESTEP_CYL_FINISH:
		if(m_fbElvClamp != FB_NONE) return;
		m_fbElvMoveReady = ECW_NONE;
		m_step_elvmove = ESTEP_CYL_START;
		break;
	}

}


void CRun_IDBuffer_El::Run_Move()
{
	int nJobCnt = Func.m_pRobot[SITE_ELV]->GetJobCount();
	if( nJobCnt == 0 )
		return;

	if( m_fbElvMoveReady != ECW_NONE) return;

    // Func.m_pRobot[SITE_ELV]->EraseFirstJob();
	stJobPicker& job = Func.m_pRobot[SITE_ELV]->GetFirstJob();


	if( elv_step_old != job.elv_step)
	{
		elv_step_old = job.elv_step;
		m_dwTimeRobot = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
//		if( Run_Elv_Move_Chk_Jam( job ) == false )
//			return;
	}
//	if( Run_Elv_Move_Chk_Time() == false )
//		return;

	CString strError="";
	Func.ThreadFunctionStepTrace(50, job.elv_step);
	
	switch( job.elv_step )
	{
	case ELV_NONE: break;

	case ELV_READY:
		if( st_basic.n_mode_device && ( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer2_chk, IO_ON) == IO_OFF) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON) == IO_OFF )
			{
				strError.Format("5%04d%d", st_io.i_idbuffer_elv_idbuffer1_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 703, dWARNING, strError );
			}
			else
			{
				strError.Format("5%04d%d", st_io.i_idbuffer_elv_idbuffer2_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 704, dWARNING, strError );
			}

			break;
		}
		g_handler.n_unload_elevator_req = CTL_NO;
		m_dwTime_ElvTray = GetCurrentTime();
		job.elv_step = ELV_BUFFER_CHK;
		break;

	case ELV_BUFFER_CHK:		
//		if(st_handler.mn_pickup_elv[0] != CTL_READY) break;//언로더 로봇이 제품을 픽옵완료하면	

		//2014,1130
// 		if( st_basic.n_mode_device && ( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk, IO_OFF) == IO_ON) )
// 		{
// 			if( GetCurrentTime()- m_dwTime_ElvTray < 0) m_dwTime_ElvTray = GetCurrentTime();
// 			if( GetCurrentTime()- m_dwTime_ElvTray < 5000) break;
// 			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_ON) == IO_OFF )
// 			{
// 				strError.Format("5%04d%d", st_io.i_top_idbuffer_box7_chk, IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence( 705, dWARNING, strError );
// 			}
// 			else
// 			{
// 				strError.Format("5%04d%d", st_io.i_idbuffer_elv_box2_chk, IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence( 706, dWARNING, strError );
// 			}
// 			break;
// 		}

// 		if( st_basic.n_mode_device && 
// 		  ( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk, IO_OFF) == IO_ON ) )
// 		{
// 			CString strError;
// 			strError.Format("5%04d%d",st_io.i_idbuffer_elv_box1_chk, IO_ON);
// 			CTL_Lib.Alarm_Error_Occurrence( 705, CTL_dWARNING, strError );
// 			break;
// 		}

		job.elv_step = ELV_MDL_CHK;
		break;

	case ELV_MDL_CHK:
		{
			if( IsBufferExist() ) job.elv_step = ELV_TOP_DATA_CHK;
			else
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_top_idbuffer7_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 707, CTL_dWARNING, strError );
			}
		}
		break;

	case ELV_TOP_DATA_CHK:
		{
//			AModule* pMdl_1;
//			AModule* pMdl_2;
//			int nTrayIdx1 = -1;
//
//			nTrayIdx1 = g_Tray.GetIdxByPos( IDPOS_UNLOAD );
//			pMdl_1 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst % MPOS_DIVISION );
//			pMdl_2 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst + 1 % MPOS_DIVISION );
//
//			if( job.iRobotFirst != -1 && pMdl_1 )
//			{
//				*pMdl_1 = g_unload_robot.GetPicker(job.iRobotFirst).GetModule();
//				pMdl_1->EraseData();
//			}
//			
//			if( job.iRobotSecond != -1 && pMdl_2 )
//			{
//				*pMdl_2 = g_robot.GetPicker(job.iRobotSecond).GetModule();
//				g_unload_robot.GetPicker(job.iRobotSecond).GetModule().EraseData();
//				pMdl_2->EraseData();
//			}			
		}
		job.elv_step = ELV_BTM_SAFETY_CHK;
		break;

	case ELV_BTM_SAFETY_CHK:
		{
			//ID Buffer1이 동작 중인지 체크
			double dPos = COMI.Get_MotCurrentPos(M_BTMID_ULD2);
// 			if( dPos < st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + COMI.md_allow_value[M_BTMID_ULD2])
// 			{
				if( st_basic.n_mode_device && g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_shifter_hook_fwd[0], IO_OFF) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_shifter_hook_bwd[0], IO_ON) == IO_ON)
				{
					job.elv_step = ELV_BTM_MOVE;
				}
				else if( st_basic.n_mode_device == false )
				{
					job.elv_step = ELV_BTM_MOVE;
				}
// 			}
		}
		break;

	case ELV_BTM_MOVE:
		{

			int nRet_1 = CTL_Lib.Single_Move(M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_DOWNBUFFER], st_basic.nRunSpeed);
			if (nRet_1 == BD_GOOD)
			{			
				job.elv_step = ELV_CON_MDL_CHK;
			}
			else if (nRet_1 == BD_RETRY)
			{		
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 708	, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case ELV_CON_MDL_CHK:
//		if(st_handler.mn_pickup_elv[1] == CTL_REQ )
//		{
//			st_handler.mn_pickup_elv[1] = CTL_READY;
//			job.elv_step = ELV_TOP_MDL_CHK;
//		}
		job.elv_step = ELV_TOP_MDL_CHK;
		break;

	case ELV_TOP_MDL_CHK:
//		if(st_handler.mn_pickup_elv[1] != CTL_FREE)  break;

		if( st_handler.n_without_idbuffer == 0 && 
			( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer2_chk, IO_ON) == IO_OFF) )
		{
			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON) == IO_OFF )
			{
				strError.Format("5%04d%d", st_io.i_idbuffer_elv_idbuffer1_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 728, dWARNING, strError );
			}
			else
			{
				strError.Format("5%04d%d", st_io.i_idbuffer_elv_idbuffer2_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 729, dWARNING, strError );
			}
			
			break;
		}


		if( ( st_handler.n_without_idbuffer == CTL_YES || g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer2_chk, IO_OFF) == IO_ON) && 
			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk, IO_OFF) == IO_OFF)
		{
			double dPos = COMI.Get_MotCurrentPos(M_BTMID_ULD2);
// 			if( dPos < st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + COMI.md_allow_value[M_BTMID_ULD2])
// 			{
				if( /*st_basic.n_mode_device == false ||*/ ( g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_shifter_hook_fwd[0], IO_OFF) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_btm_idbuffer_shifter_hook_bwd[0], IO_ON) == IO_ON ) )
				{

					job.elv_step = ELV_BTM_MDL_READY;
				}
				else
				{
					//350008 0 00 "Bottom ID Buffr ULD1 Axis Motor is Non Safety Pos."
					CTL_Lib.Alarm_Error_Occurrence( 709, CTL_dWARNING, "350008" );
				}
// 			}
// 			else
// 			{
// 				CString strError;
// 				strError.Format("5%04d%d",st_io.i_btm_idbuffer_shifter_hook_fwd[0], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence( 710, CTL_dWARNING, strError );
// 			}

		}
		else
		{
			if(	g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk, IO_OFF) == IO_ON)
			{

				CString strError="";
				strError.Format("5%04d%d",st_io.i_top_idbuffer_box7_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 711, CTL_dWARNING, strError );
			}
			
		}
		break;

	case ELV_BTM_MDL_READY:
		st_handler.mn_pickup_elv[1][0] = CTL_READY;

		Run_Smema_Ctl.btm_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_INDEX_ULD_BTM); 

		//2014,1130
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_idbuffer[CTL_LEFT] == CTL_YES && 
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_idbuffer[CTL_RIGHT] == CTL_YES)
		{
			//2015.0226
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] = CTL_NO;
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] = CTL_NO;
			Run_Smema_Ctl.btm_idbuffer_datainfo_shift(1, SHIFT_IDBUFF_INDEX_ULD_BTM); 
			job.elv_step = ELV_BTM_MOVE_READY;
		}
		
		
//		g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_UNLOAD), EIDBUFFER_POS(IPOOS_UNLOAD_BTM) );
//			AModule* pMdl_1;
//			AModule* pMdl_2;
//			int nTrayIdx1 = -1;
//
//			if( job.GetPosByTarget() == IDPOS_CONVEYOR )
//			{
//				nTrayIdx1 = g_Tray.GetIdxByPos( IDPOS_CONVEYOR );
//				pMdl_1 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst % MPOS_DIVISION );
//				pMdl_2 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst + 1 % MPOS_DIVISION );
//			}
//			if( job.GetPosByTarget() == IDPOS_REJECT )
//			{
//				nTrayIdx1 = g_Tray.GetIdxByPos( IDPOS_REJECT );
//				pMdl_1 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst % MPOS_DIVISION );
//				pMdl_2 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst + 1 % MPOS_DIVISION );
//			}
//
//			if( job.iRobotFirst != -1 && pMdl_1/* && st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES*/)
//			{
//				*pMdl_1 = g_unload_robot.GetPicker(job.iRobotFirst).GetModule();
//				g_unload_robot.GetPicker(job.iRobotFirst).GetModule().EraseData();
//			}
//			
//			if( job.iRobotSecond != -1 && pMdl_2/* && st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES*/)
//			{
//				*pMdl_2 = g_robot.GetPicker(job.iRobotSecond).GetModule();
//				g_unload_robot.GetPicker(job.iRobotSecond).GetModule().EraseData();
//			}
//
//			if( job.GetPosByTarget() == IDPOS_REJECT )
//			{
//				if( job.iRobotFirst != -1 ) pMdl_1->EraseData();
//				if( job.iRobotSecond != -1 ) pMdl_2->EraseData();
//			}

			
		job.elv_step = ELV_BTM_MOVE_READY;
		break;

	case ELV_BTM_MOVE_READY:
		if(st_handler.mn_pickup_elv[0][0] != CTL_REQ) break;//언로더 로봇이 제품을 픽옵완료하면	

		job.elv_step = ELV_BTM_MOVE_CLAMP_OFF_REQ;
		break;

	case ELV_BTM_MOVE_CLAMP_OFF_REQ:
		if( st_handler.mn_pickup_elv[0][1] == CTL_CHANGE)//CLAMP를 잡았다.
		{
			m_dwTimeMove = GetCurrentTime();
			job.elv_step = ELV_BTM_MOVE_CLAMP_OFF;
		}
		break;

	case ELV_BTM_MOVE_CLAMP_OFF:
// 		if( GetCurrentTime() - m_dwTimeMove < 0 ) m_dwTimeMove = GetCurrentTime();
// 		if( GetCurrentTime() - m_dwTimeMove < 200) break;
		ReqElvClamp(BACKWARD );
		job.elv_step = ELV_BTM_MOVE_CLAMP_OFF_CHK;
		break;

	case ELV_BTM_MOVE_CLAMP_OFF_CHK:
		if(m_fbElvClamp != FB_NONE) return;
		m_dwTimeMove = GetCurrentTime();
		job.elv_step = ELV_BTM_MOVE_SGN_CLAMP_OFF;
		break;

	case ELV_BTM_MOVE_SGN_CLAMP_OFF:
// 		if( GetCurrentTime() - m_dwTimeMove < 0 ) m_dwTimeMove = GetCurrentTime();
// 		if( GetCurrentTime() - m_dwTimeMove < 200) break;
		st_handler.mn_pickup_elv[1][1] = CTL_READY;

		job.elv_step = ELV_BTM_MOVE_FINISH;
		break;

	case ELV_BTM_MOVE_FINISH:
		if( st_handler.mn_pickup_elv[0][0] == CTL_FREE )
		{
			st_handler.mn_pickup_elv[1][0] = CTL_NO;
			st_handler.mn_pickup_elv[1][1] = CTL_NO;
			job.elv_step = ELV_TOP_MOVE;
		}
		break;
		
	case ELV_TOP_MOVE:
		{
			int nRet_1 = CTL_Lib.Single_Move(M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER], st_basic.nRunSpeed);
			if (nRet_1 == BD_GOOD)
			{			
				job.elv_step = ELV_FINISH;
			}
			else if (nRet_1 == BD_RETRY)
			{		
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(712, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		break;
		
	case ELV_FINISH:
		{
			//2014,1130
			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_OFF) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer2_chk, IO_OFF) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk, IO_OFF) == IO_ON )
			{
				CString strError="";
				if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_ON){
					strError.Format("5%04d%d",st_io.i_idbuffer_elv_box1_chk, IO_ON);
				}
				if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_box8_chk, IO_OFF) == IO_ON){
					strError.Format("5%04d%d",st_io.i_top_idbuffer_box8_chk, IO_ON);
				}
				if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_idbuffer1_chk, IO_OFF) == IO_ON){
					strError.Format("5%04d%d",st_io.i_idbuffer_elv_idbuffer1_chk, IO_ON);
				}
				else{
					strError.Format("5%04d%d",st_io.i_idbuffer_elv_idbuffer2_chk, IO_ON);
				}
				CTL_Lib.Alarm_Error_Occurrence( 713, CTL_dWARNING, strError );
			}
			else {

				Func.m_pRobot[SITE_ELV]->EraseFirstJob();

				

				st_sync.n_unload_elevator_req = CTL_REQ;
			}

		}
		break;
	}
		

}

void CRun_IDBuffer_El::Run_Restore()
{
}

void CRun_IDBuffer_El::OnRobot_FinalPos()
{
}

void CRun_IDBuffer_El::Serialize(CArchive &ar)
{
}


void CRun_IDBuffer_El::ReqRecvReadyTray()
{
	if( m_step_recvReadyTray == READYRECV_IDTRAY_NONE )
		m_step_recvReadyTray = READYRECV_IDTRAY_READY;
}

void CRun_IDBuffer_El::Run_Move_RecvReadyIDBuffer()
{
	switch(m_step_recvReadyTray)
	{
	case READYRECV_IDTRAY_NONE:	break;
	case READYRECV_IDTRAY_READY:

		if( CheckElvClamp( IO_OFF ) != true )
		{
			ReqElvClamp(BACKWARD );
			m_step_recvReadyTray = READYRECV_IDTRAY_READY_CHK;		
		}
		else{
			m_step_recvReadyTray = READYRECV_IDTRAY_FINISH;
		}
		break;

	case READYRECV_IDTRAY_READY_CHK:
		if(m_fbElvClamp != FB_NONE) return;
		m_step_recvReadyTray = READYRECV_IDTRAY_FINISH;
		break;

	case READYRECV_IDTRAY_FINISH:
		m_step_recvReadyTray = READYRECV_IDTRAY_NONE;
		break;

	}
}

void CRun_IDBuffer_El::ReqRecvDoneTray()
{
	if( m_step_recvDoneTray == DONERECV_IDTRAY_NONE )
		m_step_recvDoneTray = DONERECV_IDTRAY_READY;

}

void CRun_IDBuffer_El::Run_Move_RecvDoneIDBuffer()
{
	switch(m_step_recvDoneTray)
	{
	case DONERECV_IDTRAY_NONE:	break;
	case DONERECV_IDTRAY_READY:
		{
			int nRet = COMI.Check_MotPosRange( M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER], COMI.md_allow_value[M_IDBUFFER_EL]);
			if( nRet == BD_ERROR)
			{
				ReqElvClamp(BACKWARD );
				m_step_recvDoneTray = DONERECV_IDTRAY_MOVE;
			}
			else
			{
				ReqElvClamp(FORWARD );
				m_step_recvDoneTray = DONERECV_IDTRAY_READY_CHK;

			}
		}
		break;

	case DONERECV_IDTRAY_MOVE:
		if(m_fbElvClamp != FB_NONE) return;
		{
			int nRet = COMI.Start_SingleMove( M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				m_step_recvDoneTray = DONERECV_IDTRAY_MOVE_DELAY;
				m_dwTimeReadyElvClamp = GetCurrentTime();
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1920, CTL_dWARNING, alarm.mstr_code );
			}
		}		
		break;

	case DONERECV_IDTRAY_MOVE_DELAY:
		if( GetCurrentTime() - m_dwTimeReadyElvClamp < 0 ) m_dwTimeReadyElvClamp = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeReadyElvClamp < 300) break;
		ReqElvClamp(FORWARD );
		m_step_recvDoneTray = DONERECV_IDTRAY_READY_CHK;
		break;

	case DONERECV_IDTRAY_READY_CHK:
		if(m_fbElvClamp != FB_NONE) return;
		m_step_recvDoneTray = DONERECV_IDTRAY_FINISH;
		break;

	case DONERECV_IDTRAY_FINISH:
		m_step_recvDoneTray = DONERECV_IDTRAY_NONE;
		break;

	}
}


void CRun_IDBuffer_El::ReqReadyElv()
{
	if( m_step_readyElv == ELV_IDTRAY_NONE )
		m_step_readyElv = ELV_IDTRAY_READY;
}


void CRun_IDBuffer_El::Run_Move_ELvReady_IDBuffer()
{
	if( m_step_readyElv_old != m_step_readyElv )
	{
		m_step_readyElv_old = m_step_readyElv;
		m_dwTime_ElvTray = GetCurrentTime();

//		if( Run_Move_Chk_Jam_SendTray() == false )
//			return;
	}

//	if( Run_Move_Chk_Time_SendTray() == false )
// 		return;

	Func.ThreadFunctionStepTrace(56, m_step_readyElv);

	switch( m_step_readyElv )
	{
	case ELV_IDTRAY_NONE:	break;

	case ELV_IDTRAY_READY:
		{
			g_Run_IDBuffers_El.ReqRecvReadyTray();
			m_step_readyElv = ELV_IDTRAY_READY_CHK;
		}
		break;

	case ELV_IDTRAY_READY_CHK:
		{
			if( !g_Run_IDBuffers_El.GetRecvReadyTrayNone() ) return;
			m_step_readyElv = ELV_IDTRAY_ELV_READY_CHK;
		}
		break;

	case ELV_IDTRAY_ELV_READY_CHK:
			if( COMI.Get_MotCurrentPos( M_IDBUFFER_EL) < st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER] - COMI.md_allow_value[M_IDBUFFER_EL])
			{
				g_Run_IDBuffers_El.ReqElvMoveReady( ECW_WORK );
				m_step_readyElv = ELV_IDTRAY_ELV_MOVE;
			}
			else
			{
				m_step_readyElv = ELV_IDTRAY_ELV_CLAMP_ON;
			}
			break;

	case ELV_IDTRAY_ELV_MOVE:
		if( g_Run_IDBuffers_El.m_fbElvMoveReady != ECW_NONE) return;
		
		m_step_readyElv = ELV_IDTRAY_ELV_CLAMP_ON;
		break;

	case ELV_IDTRAY_ELV_CLAMP_ON:
		{
			g_Run_IDBuffers_El.ReqRecvDoneTray();
			m_step_readyElv = ELV_IDTRAY_ELV_CLAMP_ON_CHK;
		}
		break;

	case ELV_IDTRAY_ELV_CLAMP_ON_CHK:
		{
			if( !g_Run_IDBuffers_El.GetRecvDoneTrayNone() ) return;
			m_step_readyElv = ELV_IDTRAY_FINISH;
		}
		break;

	case ELV_IDTRAY_FINISH:
		{
			//st_sync.n_unload_elevator_req = CTL_FREE;
			m_step_readyElv = ELV_IDTRAY_NONE;
		}
		break;	

	}

}
