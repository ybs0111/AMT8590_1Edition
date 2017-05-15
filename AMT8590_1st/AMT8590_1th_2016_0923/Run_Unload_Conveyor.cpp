// CRun_Unload_Conveyor.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Unload_Conveyor.h"
#include "AMTRegistry.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"
#include "AMTLotManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Unload_Conveyor

IMPLEMENT_SERIAL(CRun_Unload_Conveyor, CObject, 1)
CRun_Unload_Conveyor g_Run_Unload_Conveyor;
CRun_Unload_Conveyor::CRun_Unload_Conveyor()
{
}

CRun_Unload_Conveyor::~CRun_Unload_Conveyor()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_Unload_Conveyor message handlers
void CRun_Unload_Conveyor::Thread_Variable_Initial()
{
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;
	mn_elv_conv_step = 0;

	m_step_recyclefwdbwd = ESTEP_CYL_START;
	m_fbRecyclefwdbwd = FB_NONE;
	st_work.mn_recycle_pusher_forward = IO_OFF;
	st_work.mn_recycle_pusher_backward = IO_OFF;

}

void CRun_Unload_Conveyor::Thread_Run()
{
//  	return;
	if( Func.OnIsInterruptCondition() != 1 )
		return;
	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
 		Run_Move();
		Run_ElvMove();
		break;
	
	case dREINSTATE:
		Run_Reinstate();
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

	Run_Recycle_FwdBwd();
// 	Run_ElvMove();

}
void CRun_Unload_Conveyor::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_UNLOAD_CONVEYOR] != CTL_NO )	return;
	switch(mn_InitStep)
	{
	case EUCI_START: mn_InitStep = EUCI_CHECK_ROBOT_CHK; break;

	case EUCI_CHECK_ROBOT_CHK:	EUCI_Check_Rbt_Chek();	break;
	case EURI_HOMECHK_STACKER_RBT: EUCI_HomeCheck_Conveyor_Stacker_Rbt(); break;

	case EURI_HOMECHK_PITCH_RBT: EUCI_HomeCheck_Conveyor_Pitch_Rbt(); break;

	case EUCI_VACUUM_ON: SetVacuumOnOff( IO_ON ); mn_InitStep = EUCI_VACUUMON_CHK;	break;

	case EUCI_VACUUMON_CHK: 
		{
			int nRet = GetVacuumCheck();
			if(nRet == RET_GOOD)
			{//524071 1 00 "Recycle Zone Box Vacuum Check On Error."
				CTL_Lib.Alarm_Error_Occurrence(1100, CTL_dWARNING, "524071" );
			}
			else
			{
				mn_InitStep = EUCI_VACUUM_OFF;
			}
		}
		break;

	case EUCI_VACUUM_OFF: SetVacuumOnOff( IO_OFF );	mn_InitStep = EUCI_RECYCLE_FWDBWD;	break;

	case EUCI_RECYCLE_FWDBWD: 
		{
			//ReqRecycleFwdBwd( BACKWARD ); 
			ReqRecycleFwdBwd( FORWARD ); //2014.1120 ysj
			mn_InitStep = EUCI_RECYCLE_FWDBWD_CHK;//2014.1018
			break;
		}

	case EUCI_RECYCLE_FWDBWD_CHK: 
		{
			if( GetRecycleFwdBwd() == FB_NONE)
				mn_InitStep = EUCI_CONVEYOR_DEVICE_CHK;
		}
		break;

	case EUCI_CONVEYOR_DEVICE_CHK: EUCI_Decive_check(); break;

	case EUCI_FINISH:	EUCI_Finish();	break;
	}
}

void CRun_Unload_Conveyor::Run_ElvMove()
{
	if( g_ioMgr.get_in_bit( st_io.recycle_door_lock_chk, IO_ON) == IO_OFF ||
		g_ioMgr.get_in_bit( st_io.recycle_door_unlock_chk, IO_ON) == IO_ON || 
		g_ioMgr.get_out_bit( st_io.o_recycle_door_onoff_cylinder, IO_ON) == IO_OFF )
	{
		//523041 1 00 "Recycle_Zone_Door_Lock_Unlock_Cylinder_On_Error."
		CString strJamCode;
		CTL_Lib.Alarm_Error_Occurrence( 6102, dWARNING, "523041" );
		return;
	}


	Func.ThreadFunctionStepTrace(62, mn_elv_conv_step);	


	switch( mn_elv_conv_step )
	{
	case 0:
		mn_elv_conv_step = 1000;
		//st_handler.mn_OutElv_work = CTL_NO;
		break;
			
	case 1000:
// 		if( st_handler.mn_OutElv_work == CTL_YES || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_OFF ) == IO_ON || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk, IO_OFF ) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_OFF) == IO_ON )
// 		{
// 			break;
// 		}

		if(	g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_ON || 
			g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
		{
			mn_elv_conv_step = 1100;
		}
		break;

	case 1100:
		if(	g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_ON || 
			g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
		{
			mn_elv_conv_step = 2000;
		}
		else if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_OFF && 
			g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
		{
			CString strError="";
			strError.Format("5%04d%d", st_io.i_recycle_zone_box1_chk,IO_OFF);
			CTL_Lib.Alarm_Error_Occurrence(1101, CTL_dWARNING, strError );
			mn_elv_conv_step = 1200;
		}
		break;

	case 1200:
		if(	g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_OFF && 
			g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_OFF )
		{
			mn_elv_conv_step = 1000;
		}
		break;

	case 2000:
		if(	g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_ON && 
			g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
		{
			mn_elv_conv_step = 2100;
		}
		else if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
		{
			mn_elv_conv_step = 2100;
		}
		else
		{
			mn_elv_conv_step = 2600;
		}
		break;

	case 2100:
		{
			int nRet1 = CTL_Lib.Single_Move( M_ULD_STACKER_Z, st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_elv_conv_step = 2200;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1102, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 2200:
		{
			g_ioMgr.set_out_bit( st_io.o_recycle_zone_box_vacuum, IO_ON);
			m_dwTimeElvPick = GetCurrentTime();
			mn_elv_conv_step = 2300;
		}
		break;

	case 2300:
		{
			if( GetCurrentTime() - m_dwTimeElvPick <= 0) m_dwTimeElvPick = GetCurrentTime();

			if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk, IO_ON) == IO_ON)
			{
				m_dwTimeElvPick = GetCurrentTime();
				mn_elv_conv_step = 2400;
			}
			else if( GetCurrentTime() - m_dwTimeElvPick  > st_time.n_limit_time[E_WAIT_CONVEYOR_VACUUM] )//2015.0224
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_recycle_zone_box_vacuum_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 1103, CTL_dWARNING, strError );
			}
		}
		break;

	case 2400:
		{
			if( m_dwTimeElvPick - GetCurrentTime() < 0) m_dwTimeElvPick = GetCurrentTime();
			if( m_dwTimeElvPick - GetCurrentTime() < st_time.n_wait_time[E_WAIT_CONVEYOR_VACUUM][IO_ON]) break;
			
			int nRet1 = CTL_Lib.Single_Move( M_ULD_STACKER_Z, st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK_UP], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_elv_conv_step = 2500;
				m_dwTimeElvPick = GetCurrentTime();
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1104, CTL_dWARNING, alarm.mstr_code );
			}
			
		}
		break;

	case 2500:
		{
			if( m_dwTimeElvPick - GetCurrentTime() < 0) m_dwTimeElvPick = GetCurrentTime();

			if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk, IO_ON) == IO_ON)
			{
				m_dwTimeElvPick = GetCurrentTime();
				mn_elv_conv_step = 2600;
			}
			else if( m_dwTimeElvPick - GetCurrentTime() > st_time.n_limit_time[E_WAIT_CONVEYOR_VACUUM] )
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_recycle_zone_box_vacuum_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 1105, CTL_dWARNING, strError );
			}
		}
		break;

	case 2600:
		if( Func.m_pRobot[SITE_UNLOAD]->GetJobCount() > 0 &&
			st_var.n_index_move_sidecover_fold_flag == CTL_NO) return;
		if( st_handler.mn_OutElv_work == CTL_NO &&
			g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_ON) == IO_OFF &&
			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk, IO_ON) == IO_OFF &&
			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_ON) == IO_OFF &&
			g_ioMgr.get_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF) == IO_OFF )
		{
			st_handler.mn_OutElv_work = CTL_YES;
			mn_elv_conv_step = 3000;
		}
		break;

	case 3000:
		ReqRecycleFwdBwd( BACKWARD );
		mn_elv_conv_step = 3100;
		break;

	case 3100:
		{
			if( m_fbRecyclefwdbwd != FB_NONE ) break;
			{
				m_dwTimeElvPick = GetCurrentTime();
				mn_elv_conv_step = 3200;
			}
		}
		break;

//	st_io.i_uld_cv_left_box_chk											= 2412;
//	st_io.i_uld_cv_right_box_chk										= 2413;
//	st_io.i_uld_cv_box_out_chk											= 2414;
	case 3200:
		{
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_ON) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk, IO_ON) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk, IO_ON) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_ON) == IO_ON)
			{
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
				g_lotMgr.GetLotAt(0).AddInputCnt( CUM );
				g_lotMgr.GetLotAt(0).AddPassCnt( CUM );
				m_dwTimeElvPick = GetCurrentTime();
				mn_elv_conv_step = 3300;
			}
			else
			{
				if( m_dwTimeElvPick - GetCurrentTime() < 0 ) m_dwTimeElvPick = GetCurrentTime();
				else if( m_dwTimeElvPick - GetCurrentTime() < st_time.n_wait_time[E_WAIT_CONVEYOR_DVC_CHK][IO_ON] ) break;

				SetRecycleFwdBwd(IO_OFF);
				CString strError="";
				strError.Format("5%04d%d",st_io.i_uld_cv_box_out_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 1106, CTL_dWARNING, strError );
			}
		}
		break;

	case 3300:
		{
			if( m_dwTimeElvPick - GetCurrentTime() < 0 ) m_dwTimeElvPick = GetCurrentTime();
			else if( m_dwTimeElvPick - GetCurrentTime() < (st_time.n_wait_time[E_WAIT_CONVEYOR_DVC_CHK][IO_ON] * 2) ) break;
			ReqRecycleFwdBwd( FORWARD );
			mn_elv_conv_step = 3400;
		}
		break;
			
	case 3400:
		{
			if( m_fbRecyclefwdbwd != FB_NONE ) break;
			{
				m_dwTimeElvPick = GetCurrentTime();
				mn_elv_conv_step = 3500;
				st_handler.mn_OutElv_work = CTL_NO;
			}
		}
		break;


	case 3500:
		{
			if( m_dwTimeElvPick - GetCurrentTime() <= 0 ) m_dwTimeElvPick = GetCurrentTime();
			if( m_dwTimeElvPick - GetCurrentTime() < 100 ) break;
			if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk, IO_ON) == IO_ON ||
				g_ioMgr.get_out_bit( st_io.o_recycle_zone_box_vacuum, IO_ON) == IO_ON )
			{
				mn_elv_conv_step = 3600;
			}
			else
			{
				mn_elv_conv_step = 4000;
			}
		}
		break;

	case 3600:
		{
			int nRet1 = CTL_Lib.Single_Move( M_ULD_STACKER_Z, st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK_DOWN], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_elv_conv_step = 3700;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1109, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 3700:
		{
			g_ioMgr.set_out_bit( st_io.o_recycle_zone_box_vacuum, IO_OFF);
			m_dwTimeElvPick = GetCurrentTime();
			mn_elv_conv_step = 3800;
		}
		break;

	case 3800:
		{
			if( GetCurrentTime() - m_dwTimeElvPick <= 0) m_dwTimeElvPick = GetCurrentTime();

			if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk, IO_OFF) == IO_OFF)
			{
				m_dwTimeElvPick = GetCurrentTime();
				mn_elv_conv_step = 3900;
			}
			else if( GetCurrentTime() - m_dwTimeElvPick > (st_time.n_limit_time[E_WAIT_CONVEYOR_VACUUM]*2) )
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_recycle_zone_box_vacuum_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 1110, CTL_dWARNING, strError );
			}
		}
		break;

	case 3900:
		{
			if( m_dwTimeElvPick - GetCurrentTime() <= 0) m_dwTimeElvPick = GetCurrentTime();
			if( m_dwTimeElvPick - GetCurrentTime() < st_time.n_wait_time[E_WAIT_CONVEYOR_VACUUM][IO_ON]) break;
			
			int nRet1 = CTL_Lib.Single_Move( M_ULD_STACKER_Z, st_motor[M_ULD_STACKER_Z].d_pos[P_CONV_Z_PICK_UP], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_elv_conv_step = 4000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1111, CTL_dWARNING, alarm.mstr_code );
			}
			
		}
		break;

	case 3950:
		mn_elv_conv_step = 3000;
		break;

	case 4000:
		g_handler.CheckLotEnd();//2015.0224
		mn_elv_conv_step = 0;
		break;

		
	}
}

void CRun_Unload_Conveyor::Run_Move()
{
	int nJobCnt = Func.m_pRobot[SITE_ELV]->GetJobCount();
	if( nJobCnt == 0 )
		return;

	stJobPicker& job = Func.m_pRobot[SITE_ELV]->GetFirstJob();


	if( tvis_step_old != job.conv_step)
	{
		tvis_step_old = job.conv_step;
		m_dwTimeMove = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
//		if( Run_Move_Chk1_Jam( job ) == false )
//			return;
	}

//  	Func.m_pRobot[SITE_ELV]->EraseFirstJob();

//	if( Run_Move_Chk1_Time( job ) == false )
//		return;
	Func.ThreadFunctionStepTrace(63, job.conv_step);
	
	switch( job.conv_step )
	{
	case CONV_NONE:	break;

	case CONV_READY:
		mn_mdlnum = mn_mdlcnt = 0;
		if(job.iRobotFirst != -1)
		{
			if(g_ioMgr.get_in_bit( st_io.i_uld_cv_box[job.iRobotFirst], IO_ON) == IO_OFF)
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_uld_cv_box[job.iRobotFirst], IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 1120, CTL_dWARNING, strError );
				break;
			}
			mn_mdlnum++;
		}
		if(job.iRobotSecond != -1)
		{
			if(g_ioMgr.get_in_bit( st_io.i_uld_cv_box[job.iRobotSecond], IO_ON) == IO_OFF)
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_uld_cv_box[job.iRobotSecond], IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 1121, CTL_dWARNING, strError );
				break;
			}
			mn_mdlnum++;
		}
		job.conv_step = CONV_MDL_CHK;
		break;
	case CONV_MDL_CHK:
		g_ioMgr.get_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
		m_dwTimeMove = GetCurrentTime();
		job.conv_step = CONV_MOVE;
		break;
	case CONV_MOVE:
		if(mn_mdlcnt >= mn_mdlnum)
		{
			job.conv_step = CONV_FINISH;
			break;
		}
		if( GetCurrentTime() - m_dwTimeMove < 0) m_dwTimeMove = GetCurrentTime();
		if(g_ioMgr.get_in_bit( st_io.i_uld_cv_box[2], IO_OFF) == IO_ON)
		{
			m_dwTimeMove = GetCurrentTime();
			job.conv_step = CONV_MOVE_CHK;
			break;
		}
		if( GetCurrentTime() - m_dwTimeMove > st_time.n_limit_time[E_WAIT_CONVEYOR_MOVE])
		{
			//524991 1 00 "Unloader Conveyor time out Error."
			CTL_Lib.Alarm_Error_Occurrence( 1122, CTL_dWARNING, "524991" );
			break;
		}
		break;

	case CONV_MOVE_CHK:
		if( GetCurrentTime() - m_dwTimeMove < 0) m_dwTimeMove = GetCurrentTime();
		if(g_ioMgr.get_in_bit( st_io.i_uld_cv_box[2], 0, 100) == IO_ON)
		{
			mn_mdlcnt++;
			job.conv_step = CONV_OUT_CHK;
			break;
		}
		if( GetCurrentTime() - m_dwTimeMove > st_time.n_limit_time[E_WAIT_CONVEYOR_MOVE])
		{
			//524141 1 00 "Unloader Conveyor Box Out On Check Error."
			CTL_Lib.Alarm_Error_Occurrence( 1123, CTL_dWARNING, "524141" );
			break;
		}
		break;

	case CONV_OUT_CHK:
		if( GetCurrentTime() - m_dwTimeMove < 0) m_dwTimeMove = GetCurrentTime();
		if(g_ioMgr.get_in_bit( st_io.i_uld_cv_box[2], 0, 100) == IO_OFF)
		{
			m_dwTimeMove = GetCurrentTime();
			job.conv_step = CONV_MOVE;
			break;
		}
		if( GetCurrentTime() - m_dwTimeMove > st_time.n_limit_time[E_WAIT_CONVEYOR_MOVE])
		{
			//524141 1 00 "Unloader Conveyor Box Out On Check Error."
			CTL_Lib.Alarm_Error_Occurrence( 1124, CTL_dWARNING, "524141" );
			break;
		}
		break;

	case CONV_FINISH:
		{
			AModule* pMdl_1;
			AModule* pMdl_2;
			int nTrayIdx1 = -1;

			nTrayIdx1 = g_Tray.GetIdxByPos( IDPOS_CONVEYOR );
			pMdl_1 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst % MPOS_DIVISION );
			pMdl_2 = &g_Tray.GetTray( nTrayIdx1 ).GetModule( job.iPosFirst + 1 % MPOS_DIVISION );

			if( job.iRobotFirst != -1 && pMdl_1)
			{
				pMdl_1->EraseData();
			}
			
			if( job.iRobotSecond != -1 && pMdl_2)
			{
				pMdl_2->EraseData();
			}
			
			Func.m_pRobot[SITE_CONV]->EraseFirstJob();

//			Func.BackupData();//2013,1018
			Func.Handler_Recovery_Data_Write();
		}
		break;
	}
}

void CRun_Unload_Conveyor::Run_Reinstate()
{
	if( st_handler.mn_Rcvy_state[RCVY_ULDCONVEYOR] == CTL_YES) return;
	switch(mn_RcvyStep)
	{
	case 0:
		{
			int nRet = COMI.Check_MotPosRange(M_ULD_STACKER_Z, COMI.md_cmdpos_backup[M_ULD_STACKER_Z], COMI.md_allow_value[M_ULD_STACKER_Z]);

			bool nRecy = false;
			m_nReq_Rcvy = CTL_NO;
			if(nRet == BD_ERROR)
			{
				m_nReq_Rcvy = CTL_YES;
				nRecy = true;
			}

			if(nRecy == false) mn_RcvyStep = 8000;
			else
			{
				m_dPos_TmpRcvyZ = COMI.md_cmdpos_backup[M_ULD_STACKER_Z];
				mn_RcvyStep = 100;
			}
		}		
		break;

	case 100:
		{
			int nRet1 = CTL_Lib.Single_Move(M_ULD_STACKER_Z, COMI.md_cmdpos_backup[M_ULD_STACKER_Z], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_RcvyStep = 2000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 2000:
		if( st_work.mn_recycle_pusher_forward == IO_ON ) ReqRecycleFwdBwd( FORWARD );
		else                                             ReqRecycleFwdBwd( BACKWARD );
		mn_RcvyStep = 2100;
		break;

	case 2100:
		if( m_fbRecyclefwdbwd != FB_NONE ) break;
		mn_RcvyStep = 9000;
		break;		

	case 9000:
		st_handler.mn_Rcvy_state[RCVY_ULDCONVEYOR] = CTL_YES;
		mn_RcvyStep = 0;
		break;
	}
}


void CRun_Unload_Conveyor::Run_Restore()
{
}

void CRun_Unload_Conveyor::OnRobot_FinalPos()
{
}

void CRun_Unload_Conveyor::Serialize(CArchive &ar)
{
}

void CRun_Unload_Conveyor::ReqRecycleFwdBwd( EFB efb )
{
	m_fbRecyclefwdbwd = efb;
	m_step_recyclefwdbwd = ESTEP_CYL_START;

}

void CRun_Unload_Conveyor::SetRecycleFwdBwd( int iOnOff )
{
	int iRecycle_1 = st_io.o_recycle_zone_box_pusher_fwd;
	int iRecycle_2 = st_io.o_recycle_zone_box_pusher_back;
	g_ioMgr.set_out_bit( iRecycle_1, iOnOff );
	g_ioMgr.set_out_bit( iRecycle_2, !iOnOff );

}

bool CRun_Unload_Conveyor::CheckRecycleFwdBwd( int iOnOff )
{
	int iRecycle_1 = st_io.i_recycle_zone_box_pusher_fwd_chk;
	int iRecycle_2 = st_io.i_recycle_zone_box_pusher_bwd_chk;

	if( g_ioMgr.get_in_bit( iRecycle_1 ) != iOnOff)	return false;
	if( g_ioMgr.get_in_bit( iRecycle_2) != !iOnOff)	return false;

	return true;
}

void CRun_Unload_Conveyor::Run_Recycle_FwdBwd()
{
	if( m_fbRecyclefwdbwd == FB_NONE ) return;

	int iOnOff = IO_ON;
	if( m_fbRecyclefwdbwd == BACKWARD)
		iOnOff = IO_OFF;

	switch( m_step_recyclefwdbwd)
	{
	case ESTEP_CYL_START: m_step_recyclefwdbwd = ESTEP_CYL_MOVE;	break;

	case ESTEP_CYL_MOVE:
		{
			SetRecycleFwdBwd( iOnOff );
			if( COMI.mn_run_status == dRUN )
			{
				st_work.mn_recycle_pusher_forward = st_io.o_recycle_zone_box_pusher_fwd;
				st_work.mn_recycle_pusher_backward = st_io.o_recycle_zone_box_pusher_back;
			}
			m_step_recyclefwdbwd = ESTEP_CYL_CHK;
			m_dwTimeRecycleStart = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckRecycleFwdBwd( iOnOff ) == true )
			{
				m_step_recyclefwdbwd = ESTEP_CYL_FINISH;
				m_dwTimeRecycle = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeRecycleStart > st_time.n_limit_time[E_WAIT_CONVEYOR_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT || COMI.mn_run_status == dREINSTATE )
				{
					CString strJamCode = GetJamCode_Unload_Convey_Time(st_io.i_recycle_zone_box_pusher_fwd_chk, iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 1125, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeRecycleStart < 0 )
			{
				m_dwTimeRecycleStart = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeRecycle < 0 )
			{
				m_dwTimeRecycle = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeRecycle < st_time.n_wait_time[E_WAIT_CONVEYOR_FWDBWD][iOnOff]*3 )
				return;

			m_fbRecyclefwdbwd = FB_NONE;
			m_step_recyclefwdbwd = ESTEP_CYL_START;
		}
		break;
	}
}

void CRun_Unload_Conveyor::EUCI_Check_Rbt_Chek()
{
	if( st_handler.mn_init_state[INIT_UNLOAD_ROBOT] == CTL_NO) return;

	g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
	//	mn_InitStep = EUCI_CHECK_ROBOT_CHK;
	mn_InitStep = EURI_HOMECHK_STACKER_RBT;//2014.1018
//	mn_InitStep = EURI_HOMECHK_PITCH_RBT;//2014,1207
}

void CRun_Unload_Conveyor::EUCI_HomeCheck_Conveyor_Stacker_Rbt()
{
	int nRet = COMI.HomeCheck_Mot( M_ULD_STACKER_Z, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EURI_HOMECHK_PITCH_RBT;
		Func.OnInitBtn( RBTN_ULD_STACKER_Z );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//410001 0 00 "Unload Robot Z Axis Motor Home Check Error."
		CTL_Lib.Alarm_Error_Occurrence(1130, CTL_dWARNING, "410001" );
	}
}

void CRun_Unload_Conveyor::EUCI_HomeCheck_Conveyor_Pitch_Rbt()
{
	int nRet = COMI.HomeCheck_Mot( M_ULD_CV_PITCH, MOT_TIMEOUT);
	nRet = BD_GOOD;
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EUCI_VACUUM_ON;
		Func.OnInitBtn( RBTN_ULD_CV_PITCH );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//310001 0 00 "Unload CV Pitch Axis Motor Home Check Error."
		CTL_Lib.Alarm_Error_Occurrence(1131, CTL_dWARNING, "310001" );
	}
}

void CRun_Unload_Conveyor::EUCI_Decive_check()
{
	//524081 1 00 "Recycle Zone Box Check On Error."
	//524091 1 00 "Recycle Zone Box1 Check On Error."
	//524101 1 00 "Recycle Zone Box2 Check On Error."
	//524121 1 00 "Unloader Conveyor Left Box On Check."
	//524131 1 00 "Unloader Conveyor Right Box On Check."
	//524141 1 00 "Unloader Conveyor Box Out On Check."
	if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_chk) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence(1140, CTL_dWARNING, "524081" );
		return;
	}
	else if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence(1141, CTL_dWARNING, "524091" );
		return;
	}
	else if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence(1142, CTL_dWARNING, "524101" );
		return;
	}
	else if( g_ioMgr.get_in_bit(st_io.i_uld_cv_left_box_chk) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence(1143, CTL_dWARNING, "524121" );
		return;
	}
	else if( g_ioMgr.get_in_bit(st_io.i_uld_cv_right_box_chk) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence(1144, CTL_dWARNING, "524131" );
		return;
	}
	else if( g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence(1145, CTL_dWARNING, "524141" );
		return;
	}

	mn_InitStep = EUCI_FINISH;
}

void CRun_Unload_Conveyor::EUCI_Finish()
{
	st_handler.mn_init_state[INIT_UNLOAD_CONVEYOR] = CTL_YES;

	mn_InitStep = EUCI_START;
}

void CRun_Unload_Conveyor::SetVacuumOnOff( int iOnOff )
{
	g_ioMgr.set_out_bit( st_io.o_recycle_zone_box_vacuum, iOnOff );
}

int CRun_Unload_Conveyor::GetVacuumCheck()
{
	int nRet = RET_PROCEED;
	if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk) == IO_ON)
	{
		nRet = RET_GOOD;
	}
	else
	{
		nRet = RET_ERROR;
	}

	return nRet;
}