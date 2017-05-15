// Run_Density_Works.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Density_Works.h"
#include "AMTLotManager.h"
#include "Run_Density_Feeders.h"

#include "AMTRegistry.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Density_Works

IMPLEMENT_SERIAL(CRun_Density_Works, CObject, 1)
CRun_Density_Works g_Run_Density_Works;

CRun_Density_Works::CRun_Density_Works()
{
	Thread_Variable_Initial();
}

CRun_Density_Works::~CRun_Density_Works()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Density_Works message handlers
void CRun_Density_Works::Thread_Variable_Initial()
{
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;
	mn_init_rej = 0;
	mn_init_pick_rej = 0;
	m_dPosDown = 0.0;
	mn_error_flag = CTL_NO;


	m_dpDensity = RDP_NONE;
}

void CRun_Density_Works::Thread_Run()
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
		 Run_Move();
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
}

void CRun_Density_Works::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_DENDITY_ROBOT] != CTL_NO )	return;

	switch(mn_InitStep)
	{
	case EDRI_START:	mn_InitStep = EDRI_CHECK_ROBOT_CHK;	break;

	case EDRI_CHECK_ROBOT_CHK:  EDRI_Check_Rbt_Chk(); break;

	case EDRI_HOMECHK_DRBT_Z1:	EDRI_HomeCheck_Z(M_D_ATTACH_Z1); break;
	case EDRI_HOMECHK_DRBT_Z2:	EDRI_HomeCheck_Z(M_D_ATTACH_Z2); break;
	case EDRI_HOMECHK_DRBT_X: EDRI_HomeCheck_XY( M_D_ATTACH_X ); break;		
	case EDRI_HOMECHK_DRBT_Y: EDRI_HomeCheck_XY( M_D_ATTACH_Y ); break;

	case EDRI_VACUUM_FOR_REJECT: EDRI_Vacuum_OnOff_For_Reject(IO_ON);	break;

	case EDRI_LABEL_CHK: EDRI_Label_Check(); break;

	case EDRI_LABEL_REJECT: if( EDRI_Label_For_Reject() == CTLBD_RET_GOOD)
							{
								mn_init_pick_rej = 0;
								//mn_InitStep = EDRI_VACUUM_REJECT_FOR_PICK;
								mn_InitStep = EDRI_VACUUM_ON;
							}
							break;

	case EDRI_VACUUM_REJECT_FOR_PICK: if( EDRI_Label_Reject_For_Pickup() == CTLBD_RET_GOOD)
									  {
										  mn_init_rej = 0;
										  mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
										  mn_InitStep = EDRI_VACUUM_REJECT_FOR_PLACE;
									  }
									  break;


	case EDRI_VACUUM_REJECT_FOR_PLACE: if( EDRI_Label_For_Reject() == CTLBD_RET_GOOD)
									  {
										  mn_InitStep = EDRI_VACUUM_ON;
									  }
									  break;

	case EDRI_VACUUM_ON: EDRI_Vacuum_OnOff(IO_ON);	break;

	case EDRI_LABEL_EMPTY_CHK: EDRI_Label_Empty_Check(); break;

	case EDRI_VACUUM_OFF: EDRI_Vacuum_OnOff(IO_OFF);	break;

	case EDRI_HOMECHK_GUIDE: EDRI_HomeCheck_Guide(M_ULD_ROLLER_PITCH); break;

	case EDRI_GUIDE_MOVE_SAFETY:
		{
			int nRet1 = CTL_Lib.Single_Move( M_ULD_ROLLER_PITCH, st_motor[ M_ULD_ROLLER_PITCH].d_pos[P_GUIDE_UNLOAD_POS], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_InitStep = EDRI_FINISH;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 599, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
			
// 	case EDRI_ERROR_PICK: EDRI_Error_Pick_XY(); break;
// 
// 	case EDRI_ERROR_PLACE: EDRI_Error_Place_XY(); break;
// 
	case EDRI_FINISH: EDRI_Finish(); break;

	}
}

void CRun_Density_Works::EDRI_Check_Rbt_Chk()
{
	mn_InitStep = EDRI_HOMECHK_DRBT_Z1;
}

void CRun_Density_Works::EDRI_HomeCheck_Z(int nAxis)
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		if( nAxis == M_D_ATTACH_Z1 ) //2014.1018 = -> ==
		{
			mn_InitStep = EDRI_HOMECHK_DRBT_Z2;
			Func.OnInitBtn( RBTN_D_ATTACH_Z1 );//2014.1018
		}
		else
		{
			mn_InitStep = EDRI_HOMECHK_DRBT_X;
			Func.OnInitBtn( RBTN_D_ATTACH_Z2 );//2014.1018
		}
	}
	else if( nRet == BD_ERROR )
	{
		//130001 0 00 "Density Attach Z1 Axis Motor Home Check Error."
		//140001 0 00 "Density Attach Z2 Axis Motor Home Check Error."
		CString strJamCode = "140001";
		if( nAxis = M_D_ATTACH_Z1)	strJamCode = "130001";			
		CTL_Lib.Alarm_Error_Occurrence(500, CTL_dWARNING, strJamCode );
	}
}

void CRun_Density_Works::EDRI_HomeCheck_XY(int nAxis)
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		if( nAxis == M_D_ATTACH_X )//2014.1018 = -> ==
		{
			mn_InitStep = EDRI_HOMECHK_DRBT_Y;
			Func.OnInitBtn( RBTN_D_ATTACH_X );//2014.1018
		}
		else
		{
			mn_InitStep = EDRI_VACUUM_FOR_REJECT;
			Func.OnInitBtn( RBTN_D_ATTACH_Y );//2014.1018
		}
	}
	else if( nRet == BD_ERROR )
	{
		//110001 0 00 "Density Attach X Axis Motor Home Check Error."
		//120001 0 00 "Density Attach Y Axis Motor Home Check Error."
		CString strJamCode = "120001";
		if( nAxis == M_D_ATTACH_X)	strJamCode = "110001";			//2014.1018 = -> ==
		CTL_Lib.Alarm_Error_Occurrence(501, CTL_dWARNING, strJamCode );
	}
}

void CRun_Density_Works::EDRI_Error_Pick_XY()
{

}

void CRun_Density_Works::EDRI_Error_Place_XY()
{

}

void CRun_Density_Works::EDRI_Vacuum_OnOff_For_Reject(int iOnOff)
{
	g_ioMgr.get_out_bit( st_io.o_density_rbt_left_vacuum1, iOnOff);
	g_ioMgr.get_out_bit( st_io.o_density_rbt_left_vacuum2, iOnOff);
	g_ioMgr.get_out_bit( st_io.o_density_rbt_right_vacuum1, iOnOff);
	g_ioMgr.get_out_bit( st_io.o_density_rbt_right_vacuum2, iOnOff);
	m_dwTimeInit = GetCurrentTime();
	mn_InitStep = EDRI_LABEL_CHK;
}

void CRun_Density_Works::EDRI_Label_Check()
{
	if( GetCurrentTime() - m_dwTimeInit <= 0 ) m_dwTimeInit = GetCurrentTime();
	if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][1]) return;

	if( g_ioMgr.get_in_bit( st_io.i_density_rbt_left_vacuum1_chk ) == IO_ON || g_ioMgr.get_in_bit( st_io.i_density_rbt_left_vacuum2_chk ) == IO_ON ||
		g_ioMgr.get_in_bit( st_io.i_density_rbt_right_vacuum1_chk ) == IO_ON || g_ioMgr.get_in_bit( st_io.i_density_rbt_right_vacuum1_chk ) == IO_ON )
	{
		mn_init_rej = 0;
		mn_InitStep = EDRI_LABEL_REJECT;
	}
// 	else
// 	{
// 		mn_InitStep = EDRI_VACUUM_REJECT_FOR_PICK;
// 	}
	mn_InitStep = EDRI_LABEL_REJECT;
}

int CRun_Density_Works::EDRI_Label_For_Reject()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	COMI.ml_axiscnt[M_ROBOT_DENSITY_INDEX] = 2;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][0] = M_D_ATTACH_X;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][1] = M_D_ATTACH_Y;

	switch( mn_init_rej) 
	{
	case 0:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nManualSpeed;
			dSpeedRatio[1] = (double)st_basic.nManualSpeed;
			dSpeedRatio[2] = (double)st_basic.nManualSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_init_rej = 100;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 100:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_ERR], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_ERR], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_init_rej = 200;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 200:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		m_dwTimeInit = GetCurrentTime();
		mn_init_rej = 300;
		break;

	case 300:
		if( GetCurrentTime() - m_dwTimeInit <= 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_OFF]) break;
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_ON );
		m_dwTimeInit = GetCurrentTime();
		mn_init_rej = 400;
		break;

	case 400:
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < (st_time.n_wait_time[E_WAIT_BCR_EJECT][IO_ON]*3)) break;
// 		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
// 		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
// 		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
// 		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );
		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		mn_init_rej = 500;
		break;

	case 500:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );

				nFuncRet = CTLBD_RET_GOOD;
				mn_init_rej = 0;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	}

	return nFuncRet;
}

int CRun_Density_Works::EDRI_Label_Reject_For_Pickup()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	COMI.ml_axiscnt[M_ROBOT_DENSITY_INDEX] = 2;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][0] = M_D_ATTACH_X;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][1] = M_D_ATTACH_Y;

	switch( mn_init_pick_rej) 
	{
	case 0:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nManualSpeed;
			dSpeedRatio[1] = (double)st_basic.nManualSpeed;
			dSpeedRatio[2] = (double)st_basic.nManualSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_init_pick_rej = 10;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 10:
		g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_density_lable_start[st_basic.mn_device_type+3], IO_ON);
		if(st_basic.nLabelSelect== CTL_YES)//75mm
		{
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[st_basic.mn_device_type], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[st_basic.mn_device_type], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[st_basic.mn_device_type+3], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[st_basic.mn_device_type+3], IO_OFF);
		}
		else//55mm
		{
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[st_basic.mn_device_type], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[st_basic.mn_device_type], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[st_basic.mn_device_type+3], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[st_basic.mn_device_type+3], IO_OFF);
		}
		mn_init_pick_rej = 20;
		break;

	case 20:
		if( st_basic.n_mode_device == 1 && 
			( st_var.n_err_density_flag[st_basic.mn_device_type] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON ) == IO_ON ) )
		{
			mn_init_pick_rej = 30;
			m_dwTimeLabel[st_basic.mn_device_type] = GetCurrentTime();
		}
		else if( st_basic.n_mode_device == 1 && 
			( st_var.n_err_density_flag[st_basic.mn_device_type+3] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON ) == IO_ON ) )
		{
			mn_init_pick_rej = 40;
			m_dwTimeLabel[st_basic.mn_device_type+3] = GetCurrentTime();
		}
		else
		{
			mn_init_pick_rej = 100;
		}
		break;

	case 30:
		if( GetCurrentTime() - m_dwTimeLabel[st_basic.mn_device_type] <= 0) m_dwTimeLabel[st_basic.mn_device_type] = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeLabel[st_basic.mn_device_type] < 50) break;
		if( st_basic.n_mode_device == 1 && 
			( st_var.n_err_density_flag[st_basic.mn_device_type] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON ) == IO_ON ) )
		{
			CString strError="";
			strError.Format("7000%d0",st_basic.mn_device_type+1);
			CTL_Lib.Alarm_Error_Occurrence( 10011, dWARNING, strError );
			mn_init_pick_rej = 20;
		}
		else
		{
			mn_init_pick_rej = 20;
		}
		break;		
	case 100:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_init_pick_rej = 200;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 200:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_ON );
		m_dwTimeInit = GetCurrentTime();
		mn_init_pick_rej = 300;
		break;

	case 300:
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < (st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_ON]*3)) break;
		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		mn_init_pick_rej = 400;
		break;

	case 400:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				nFuncRet = CTLBD_RET_GOOD;
				mn_init_pick_rej = 0;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	}

	return nFuncRet;
}

void CRun_Density_Works::EDRI_Vacuum_OnOff(int iOnOff)
{
	g_ioMgr.get_out_bit( st_io.o_density_rbt_left_vacuum1, iOnOff);
	g_ioMgr.get_out_bit( st_io.o_density_rbt_left_vacuum2, iOnOff);
	g_ioMgr.get_out_bit( st_io.o_density_rbt_right_vacuum1, iOnOff);
	g_ioMgr.get_out_bit( st_io.o_density_rbt_right_vacuum2, iOnOff);

	if(iOnOff == IO_ON) mn_InitStep = EDRI_LABEL_EMPTY_CHK;//2014.1018
	else                mn_InitStep = EDRI_HOMECHK_GUIDE;//EDRI_HOMECHK_DRBT_X;//2014.1018
	m_dwTimeInit = GetCurrentTime();
}

void CRun_Density_Works::EDRI_HomeCheck_Guide(int nAxis)
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EDRI_GUIDE_MOVE_SAFETY;
	}
	else if( nRet == BD_ERROR )
	{
		//400001 0 00 "M_ULD_ROLLER_PITCH."
		CString strJamCode = "400001";
		CTL_Lib.Alarm_Error_Occurrence(581, CTL_dWARNING, strJamCode );
	}
}

void CRun_Density_Works::EDRI_Label_Empty_Check()
{
//507081 1 00 "Density Label Robot Left Vacuum1 Check On Error."
//507091 1 00 "Density Label Robot Left Vacuum2 Check On Error."
//507101 1 00 "Density Label Robot Right Vacuum1 Check On Error."
//507111 1 00 "Density Label Robot Right Vacuum2 Check On Error."

	if( GetCurrentTime() - m_dwTimeInit <= 0 ) m_dwTimeInit = GetCurrentTime();
	if( GetCurrentTime() - m_dwTimeInit < (st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_ON] * 3) ) return;

	if( g_ioMgr.get_in_bit( st_io.i_density_rbt_left_vacuum1_chk ) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence( 502, dWARNING, "507081" );
		return;
	}
	else if( g_ioMgr.get_in_bit( st_io.i_density_rbt_left_vacuum2_chk ) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence( 503, dWARNING, "507091" );
		return;
	}
	else if( g_ioMgr.get_in_bit( st_io.i_density_rbt_right_vacuum1_chk ) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence( 504, dWARNING, "507101" );
		return;
	}
	else if( g_ioMgr.get_in_bit( st_io.i_density_rbt_right_vacuum1_chk ) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence( 505, dWARNING, "507111" );
		return;
	}

	//	mn_InitStep = EDRI_HOMECHK_DRBT_X;
	mn_InitStep = EDRI_VACUUM_OFF;//2014.1018
	//	Func.OnInitBtn( RBTN_ROBOT_MODULE_EMPTY );
}

void CRun_Density_Works::EDRI_Finish()
{
	Func.OnInitBtn( RBTN_D_ATTACH );//2014.1018
	st_handler.mn_init_state[INIT_DENDITY_ROBOT] = CTL_YES;
	mn_InitStep = EDRI_START;
}

bool CRun_Density_Works::Chk_Robot_Z1_Safety()
{
	if( g_comiMgr.Get_MotCurrentPos( M_D_ATTACH_Z1 ) > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z1] )
	{//130008 0 00 "Density Attach Z1 Axis Motor is Non Safety Pos."
		CTL_Lib.Alarm_Error_Occurrence( 510, dWARNING, "130008" );
		return false;
	}

	return true;
}

bool CRun_Density_Works::Chk_Robot_Z2_Safety()
{
	if( g_comiMgr.Get_MotCurrentPos( M_D_ATTACH_Z2 ) > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z2] )
	{//140008 0 00 "Density Attach Z2 Axis Motor is Non Safety Pos."
		CTL_Lib.Alarm_Error_Occurrence( 511, dWARNING, "130008" );
		return false;
	}

	return true;
}

bool CRun_Density_Works::Run_Density_Move_Chk_Jam( stJobPicker job )
{
	CString strJamCode;

	Func.ThreadFunctionStepTrace(40, job.picker_step);
	switch( job.feeder_step )
	{
	case FEEDER_NONE:
	case FEEDER_PICK_READY:
		break;

	case FEEDER_PICK_Z1_SAFETY:
		break;

	case FEEDER_PICK_MOVE:
		if( Chk_Robot_Z1_Safety() == false )					return false;
		if( Chk_Robot_Z2_Safety() == false )					return false;
		break;

	case FEEDER_PICK_PLACE_READY:
		if( Chk_Robot_Z1_Safety() == false )					return false;
		if( Chk_Robot_Z2_Safety() == false )					return false;
		break;

	case FEEDER_PLACE_MOVE:
		if( Chk_Robot_Z1_Safety() == false )					return false;
		if( Chk_Robot_Z2_Safety() == false )					return false;
		break;
	}
	return true;
}

bool CRun_Density_Works::Run_Density_Move_Chk_Time()
{
	switch( feeder_step_old )
	{
	case FEEDER_PICK_Z1_SAFETY:
 		if( InTimeRobot( 10000 ) == false )
 		{//130006 1 00 "Density Attach Z1 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 512, dWARNING, "130006" );
			return false;
 		}
		break;

	case FEEDER_PICK_RBT_VACUUM_CHK:
		if( InTimeRobot( 10000 ) == false )
		{
			CString strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
			CTL_Lib.Alarm_Error_Occurrence( 525, dWARNING, strJamCode );
		}
		break;

	case FEEDER_PICK_Z2_SAFETY:
 		if( InTimeRobot( 10000 ) == false )
 		{//140006 1 00 "Density Attach Z2 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 513, dWARNING, "140006" );
			return false;
 		}
		break;

	case FEEDER_PICK_MOVE_DOWN:
 		if( InTimeRobot( 10000 ) == false )
 		{//130006 1 00 "Density Attach Z1 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 514, dWARNING, "130006" );
			return false;
 		}
		break;

	case FEEDER_PICK_MOVE:
 		if( InTimeRobot( 20000 ) == false )
 		{//110006 1 00 "Density Attach X Axis Motor Move Error."
			//120006 1 00 "Density Attach Y Axis Motor Move Error."
// 			CTL_Lib.Alarm_Error_Occurrence( 515, dWARNING, "120006" );
// 			return false;
 		}
		break;

	case FEEDER_PLACE_VACUUM_ON_CHK:
		if( InTimeRobot( 10000 ) == false )
		{
			CString strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
			CTL_Lib.Alarm_Error_Occurrence( 530, dWARNING, strJamCode );
		}
		break;

	}

	return true;
}

bool CRun_Density_Works::InTimeRobot( int iMilliSec )
{
	DWORD dwtime = GetCurrentTime();
	//2012,1220
	if( GetCurrentTime() - m_dwTime_Robot < 0 )
	{
		m_dwTime_Robot = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwTime_Robot < iMilliSec )
		return true;

	return false;
}

void CRun_Density_Works::Run_Move()
{
	COMI.ml_axiscnt[M_ROBOT_DENSITY_INDEX] = 2;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][0] = M_D_ATTACH_X;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][1] = M_D_ATTACH_Y;

	//////////////////////////////////////////////////////////////////////////
	int nJobCnt = Func.m_pRobot[SITE_DENSITY]->GetJobCount();
	if( nJobCnt == 0 )
		return;


	stJobPicker& job = Func.m_pRobot[SITE_DENSITY]->GetFirstJob();

	if( feeder_step_old != job.feeder_step )
	{
		feeder_step_old = job.feeder_step;
		m_dwTime_Robot = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
		if( Run_Density_Move_Chk_Jam( job ) == false )
			return;

	}
	if( Run_Density_Move_Chk_Time() == false )
	{
		feeder_step_old = -1;
			return;
	}

	Func.ThreadFunctionStepTrace(41, job.feeder_step);
	switch( job.feeder_step )
	{
	case FEEDER_NONE:	break;

	case FEEDER_PICK_READY:
		{

			if( st_handler.mn_d_piclplce == CTL_YES )
			{
// 				Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();
//				break;
			}

// 			if(g_lotMgr.GetLotCount() < 1 ||
// 				g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" ) break;
			
			double dpos1 = COMI.Get_MotCurrentPos( M_D_ATTACH_Z1);
			double dpos2 = COMI.Get_MotCurrentPos( M_D_ATTACH_Z2);

			if( dpos1 > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY])
			{
				job.feeder_step = FEEDER_PICK_Z1_SAFETY;
			}
			else if( dpos2 > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY])
			{
				job.feeder_step = FEEDER_PICK_Z2_SAFETY;
			}
			else
			{
				mn_RetryCnt = 0;
				m_dPosDown = 0.0;
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_MOVE;
			}

			m_dwTimeTact = GetCurrentTime();
		}
		break;

	case FEEDER_PICK_Z1_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.feeder_step = FEEDER_PICK_Z2_SAFETY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PICK_Z2_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				mn_RetryCnt = 0;
				m_dPosDown = 0.0;
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_MOVE;

			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 521, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

//	case FEEDER_PICK_LOT_READY:
//		if( g_lotMgr.GetLotCount() > 0 && g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES" )
//		{
//			if( g_Run_Density_Feeders.m_bPrint_Pickup[st_basic.mn_device_type] && g_Run_Density_Feeders.m_bPrint_Pickup[st_basic.mn_device_type+3] )
//			{
//				mn_PickStep = FEEDER_PICK_MOVE;
//			}
//		}
//		break;

	case FEEDER_PICK_MOVE:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_MOVE_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 522, CTL_dWARNING, alarm.mstr_code );
			}
// 			int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type], st_basic.nRunSpeed );
// 			int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				//job.feeder_step = FEEDER_PICK_MOVE_DOWN;
// 				job.feeder_step = FEEDER_STEP(41);
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 522, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;

	case 41:
		{
			int nRet1 = COMI.Check_SingleMove(M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type]);
			int nRet2 = COMI.Check_SingleMove(M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type]);
			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_MOVE_DOWN;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				job.feeder_step = FEEDER_PICK_MOVE;
				CTL_Lib.Alarm_Error_Occurrence( 523, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;


	case FEEDER_PICK_MOVE_DOWN:
		{
			if(st_basic.n_mode_device == 1)
			{
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_ON );
	// 			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_ON );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_ON );
	// 			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_ON );
			}

			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			job.feeder_step = FEEDER_PICK_RBT_VACUUM;

		}
		break;

	case FEEDER_PICK_RBT_VACUUM:
		{

			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type] + m_dPosDown, (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			else if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{

				m_dwTime_Pick = GetCurrentTime();
				job.feeder_step = FEEDER_PICK_PLATE_VACUUM;

			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 523, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 524, CTL_dWARNING, alarm.mstr_code );
				
			}
		}
		break;

	case FEEDER_PICK_PLATE_VACUUM:
		if( GetCurrentTime() - mn_PickStep < 0)  mn_PickStep = GetCurrentTime();
		else if( GetCurrentTime() - mn_PickStep < 100) break;		

		//2014,1126
// 		g_ioMgr.set_out_bit( st_io.o_density_label_blow_onoff, IO_ON );
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_btm_blow_onoff[st_basic.mn_device_type], IO_ON );
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_btm_blow_onoff[st_basic.mn_device_type+3], IO_ON );
// 
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_blow_onoff[st_basic.mn_device_type], IO_ON );
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_blow_onoff[st_basic.mn_device_type+3], IO_ON );
		
		m_dwTime_Pick = GetCurrentTime();
		job.feeder_step = FEEDER_PICK_RBT_VACUUM_CHK;
		break;

	case FEEDER_PICK_RBT_VACUUM_CHK:
		if( st_handler.n_dryrun_density == CTL_YES || st_basic.n_mode_device == false )
		{
			job.feeder_step = FEEDER_PICK_RBT_VACUUM_DELAY;
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		if( st_basic.n_mode_device && ( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_ON) &&
			(g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_ON) )
		{
			job.feeder_step = FEEDER_PICK_RBT_VACUUM_DELAY;
			
			m_dwTime_Pick = GetCurrentTime();
		}
		else if( GetCurrentTime() - m_dwTime_Pick > 5000)//st_time.n_limit_time[E_WAIT_FEEDER_VACUUM] )
		{
			if(st_handler.mn_d_piclplce == CTL_YES)
			{
				m_dwTime_Pick = GetCurrentTime();
				job.feeder_step = FEEDER_PICK_RBT_VACUUM_DELAY;
			}
			else
			{
// 				mn_RetryCnt++;
// 				if( mn_RetryCnt >= st_basic.mn_network_retry_cnt)
// 				{
// 					mn_RetryCnt = 0;
// 					job.feeder_step = FEEDER_PICK_RBT_VACUUM_DELAY;
// 					break;
// 				}
// 				CString strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
// 				CTL_Lib.Alarm_Error_Occurrence( 525, dWARNING, strJamCode );
				m_dwTime_Pick = GetCurrentTime();
				job.feeder_step = FEEDER_PICK_RBT_VACUUM_DELAY;
			}
		}
		else if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();		

		break;

	case FEEDER_PICK_RBT_VACUUM_DELAY:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_ON]) break;

		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		job.feeder_step = (FEEDER_STEP)91;
		break;

	case 91:
		if( st_basic.n_mode_device == 1 )
		{
			g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type+3], IO_ON);
		}
		m_dwTime_Pick = GetCurrentTime();
		job.feeder_step = (FEEDER_STEP)92;
		break;

	case 92:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_ON]*10) break;
		if( st_basic.n_mode_device == false || ( g_ioMgr.get_in_bit( st_io.i_density_label_complete_chk[st_basic.mn_device_type], IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_density_label_complete_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON) )
		{
			job.feeder_step = (FEEDER_STEP)93;
		}
		else if( ( st_var.n_err_density_flag[st_basic.mn_device_type] == CTL_YES || g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON ) ||
			( st_var.n_err_density_flag[st_basic.mn_device_type+3] == CTL_YES || g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON ) )
		{
			job.feeder_step = (FEEDER_STEP)93;
			mn_RetryCnt = st_basic.mn_network_retry_cnt;
		}
		else if( ( st_var.n_err_density_flag[st_basic.mn_device_type] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON ) == IO_ON ) ||
			 ( st_var.n_err_density_flag[st_basic.mn_device_type+3] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON ) == IO_ON ) )
		{
			job.feeder_step = (FEEDER_STEP)93;
			mn_RetryCnt = st_basic.mn_network_retry_cnt;
// 			CString strError;
// 			if( g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON ) == IO_ON )
// 				strError.Format("7000%d0",st_basic.mn_device_type);
// 			else
// 				strError.Format("7000%d0",st_basic.mn_device_type+3);
// 			CTL_Lib.Alarm_Error_Occurrence( 5699, dWARNING, strError );
		}
		else
		{
			job.feeder_step = (FEEDER_STEP)93;
		}
		break;

	case 93:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type] - 15, (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				job.feeder_step = FEEDER_PICK_MOVE_UP;
				if( st_basic.n_mode_device && st_handler.n_dryrun_density == CTL_NO)
				{
					job.feeder_step = FEEDER_PICK_MOVE_UP;
// 					if( ( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON || g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_ON ) &&
// 						(g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON || g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_ON) )
// 					{
// 						job.feeder_step = FEEDER_PICK_MOVE_UP;
// 					}
// 					else
// 					{
// 						mn_RetryCnt++;
// 						if( mn_RetryCnt >= st_basic.mn_network_retry_cnt)
// 						{
// 							//mn_RetryCnt = 0;
// 							job.feeder_step = FEEDER_PICK_MOVE_UP;
// 						}
// 						else
// 						{
// 							job.feeder_step = FEEDER_PICK_RBT_VACUUM;
// 						}
// 					}
				}
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;

			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 526, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 527, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PICK_MOVE_UP:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				m_dwTime_Pick = GetCurrentTime();
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_MOVE_UP_CHK;

// 				g_ioMgr.set_out_bit( st_io.o_density_label_blow_onoff, IO_ON );
				if(mn_RetryCnt > 0)
				{
				}
				else
				{
				}
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 526, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 527, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PICK_MOVE_UP_CHK:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_ON]) break;

		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		//////////////////////////////////////////////////////////////////////////
		{
			//2015.0220
			st_basic.mn_overtime_density = st_basic.mn_limit_bcr_time;
			bool bAway = false;
			if( (GetCurrentTime() - st_handler.m_dwTimeOverDensity[st_basic.mn_device_type]) > (st_basic.mn_overtime_density * 1000 * 60) )
			{
// 				st_var.mn_check_density_flag = CTL_YES;
// 				st_var.n_use_density_key = CTL_YES;
// 				st_var.mn_check_density_count = 0;
				st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
				st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
				st_var.n_use_density_count[st_basic.mn_device_type] = 0;
				st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;
				bAway = true;
			}
// 			else if(st_var.mn_new_density_flag == CTL_YES && st_var.mn_new_density_count < 1)
// 			{
// 				bAway = true;
// 			}
// 			else if( st_var.n_newLabelType_flag == CTL_YES && st_var.mn_new_density_count < 2)
// 			{
// 				bAway = true;
// 			}
// 			else if( st_var.mn_check_density_flag == CTL_YES && st_var.mn_check_density_count < 1)
// 			{
// 				bAway = true;
// 			}
			else if( (st_var.n_use_density_flag[st_basic.mn_device_type] == CTL_YES && st_var.n_use_density_count[st_basic.mn_device_type] <= 2 ) ||
				(st_var.n_use_density_flag[st_basic.mn_device_type+3] == CTL_YES && st_var.n_use_density_count[st_basic.mn_device_type+3] <= 2 ) )
			{
				bAway = true;
			}
// 			else if(st_var.mn_new_density_flag == CTL_YES)
// 			{
// 				bAway = true;
// 			}
			else if( GetCurrentTime() - st_handler.m_dwTimeOverDensity[st_basic.mn_device_type] <= 0)
				st_handler.m_dwTimeOverDensity[st_basic.mn_device_type] = GetCurrentTime();

			if( st_handler.n_dryrun_density == CTL_NO && st_basic.n_mode_device )
			{
				if( ( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF &&
					g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_OFF ) ||
					( g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_OFF &&
					g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_OFF ) ) 
				{
					mn_RetryCnt++;
					if( mn_RetryCnt >= st_basic.mn_network_retry_cnt)
					{
						CString strJamCode;
						if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF )
						{
							strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF );
						}
						else if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF )
						{
							strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF );
						}
						else if( g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_OFF )
						{
							strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF );
						}
						else
						{
							strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF );
						}

// 						if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON ||
// 							g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON)
// 						{
// 							//700010 1 A "Density label1 time out from server."
// 							if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON )
// 								strJamCode.Format("7000%d0", st_basic.mn_device_type+1);
// 							else
// 								strJamCode.Format("7000%d0", st_basic.mn_device_type+3+1);
// 
// 						}

						//2015.0725
						g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type], IO_OFF);
						g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type+3], IO_OFF);

						mn_RetryCnt = 0;
						m_dPosDown = 0;
						CTL_Lib.Alarm_Error_Occurrence( 528, dWARNING, strJamCode );
						job.feeder_step = FEEDER_PICK_THROW_AWAY;
					}
					else
					{
// 						if(bAway == true)
// 						{
// 							job.feeder_step = FEEDER_PICK_THROW_AWAY;
// 						}
// 						else
// 						{
// 							m_dPosDown += 0.2;
// 							job.feeder_step = FEEDER_PICK_MOVE;
// 						}
						job.feeder_step = FEEDER_PICK_THROW_AWAY;
					}
					break;

				}
				else if( mn_RetryCnt > 0 || bAway == true)
				{
					job.feeder_step = FEEDER_PICK_THROW_AWAY;
					break;
				}

				//2014,1105
				if( st_handler.mn_d_piclplce == CTL_YES )
				{
					job.feeder_step = FEEDER_PICK_THROW_AWAY;
					break;
				}

			}
		}
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////


		//2014,1105
		if( st_handler.mn_d_piclplce == CTL_YES )
		{
 			job.feeder_step = FEEDER_PICK_THROW_AWAY;
			break;
		}

		if( st_basic.n_mode_device == 1 && ( st_var.n_err_density_flag[st_basic.mn_device_type] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON ) == IO_ON ||
			st_var.n_err_density_flag[st_basic.mn_device_type+3] == CTL_YES || g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type+3 ], IO_ON ) == IO_ON) )
		{
			//2015.0318
// 			job.feeder_step = (FEEDER_STEP)93;
// 			mn_RetryCnt = st_basic.mn_network_retry_cnt;
// 			CString strError;
// 			if( g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON ) == IO_ON )
// 				strError.Format("7000%d0",st_basic.mn_device_type+1);
// 			else
// 				strError.Format("7000%d0",st_basic.mn_device_type+3+1);
// 			CTL_Lib.Alarm_Error_Occurrence( 5689, dWARNING, strError );
			job.feeder_step = FEEDER_PICK_THROW_AWAY;
		}
		
		////2015.0217
		//job.feeder_step = FEEDER_PICK_PLACE_READY;
		if( st_handler.n_dryrun_density == CTL_NO && st_basic.n_mode_device )
		{
			job.feeder_step = (FEEDER_STEP)195;
		}
		else
		{
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			job.feeder_step = FEEDER_PICK_PLACE_READY;
		}
		////

		if( job.iRobotFirst != -1)
		{
			memcpy( &st_den_rbt[job.iRobotFirst].st_label , &st_den_feeder[st_basic.mn_device_type].st_label, sizeof(st_den_rbt[job.iRobotFirst].st_label));
			st_den_rbt[job.iRobotFirst].bPick = 1;			
		}
		if( job.iRobotSecond != -1 )
		{
			memcpy( &st_den_rbt[job.iRobotSecond].st_label , &st_den_feeder[st_basic.mn_device_type+3].st_label, sizeof(st_den_rbt[job.iRobotSecond].st_label));
			st_den_rbt[job.iRobotSecond].bPick = 1;
		}
		if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
		{
			if( job.iRobotFirst != -1 )
				memset( &st_den_feeder[st_basic.mn_device_type], 0x00, sizeof(st_den_feeder[st_basic.mn_device_type]));
			if( job.iRobotSecond != -1 )
				memset( &st_den_feeder[st_basic.mn_device_type+3], 0x00, sizeof(st_den_feeder[st_basic.mn_device_type+3]));
			if( st_den_rbt[job.iRobotFirst].st_label.nState == EMBS_FAIL || st_den_rbt[job.iRobotSecond].st_label.nState == EMBS_FAIL)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_THROW_AWAY;
				break;
			}
		}

		break;

	case FEEDER_PICK_THROW_AWAY:
		{
			if     ( st_var.m_var_reject[0] == CTL_NO ) m_dRejectPos = st_basic.nRejectOffset;
			else if( st_var.m_var_reject[1] == CTL_NO ) m_dRejectPos = 0;
			else										m_dRejectPos = -st_basic.nRejectOffset;
			job.feeder_step = (FEEDER_STEP)121;
		}
		break;

	case 121:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS] + m_dRejectPos;
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_THROW_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
			}
// 			int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS], st_basic.nRunSpeed );
// 			int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_STEP(122);
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
// 			}

		}
		break;

	case 122:
		{
			int nRet1 = COMI.Check_SingleMove(M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS] + m_dRejectPos);
			int nRet2 = COMI.Check_SingleMove(M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS]);
			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_THROW_DOWN;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				CTL_Lib.Alarm_Error_Occurrence( 530, CTL_dWARNING, alarm.mstr_code );
				job.feeder_step = FEEDER_PICK_THROW_AWAY;
			}
		}
		break;
		
	case FEEDER_PICK_THROW_DOWN:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_ERR], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_ERR], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;

				mn_pickderror[0] = mn_pickderror[1] = CTL_NO;
				if( g_ioMgr.get_in_bit( st_io.i_density_left_label_limit1_chk, IO_OFF) == IO_ON) mn_pickderror[0] = CTL_YES;
				if( g_ioMgr.get_in_bit( st_io.i_density_right_label_limit2_chk, IO_OFF) == IO_ON ) mn_pickderror[1] = CTL_YES;
				job.feeder_step = FEEDER_PICK_THROW_DOWN_VACUUM_OFF;

			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case FEEDER_PICK_THROW_DOWN_VACUUM_OFF:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		m_dwTime_move = GetCurrentTime();
		job.feeder_step = FEEDER_PICK_THROW_DOWN_VACUUM_OFF_CHK;
		break;

	case FEEDER_PICK_THROW_DOWN_VACUUM_OFF_CHK:
		if( GetCurrentTime() - m_dwTime_move < 0) m_dwTime_move = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_move < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_OFF]) break;
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_ON );
		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		job.feeder_step = FEEDER_PICK_THROW_UP;
		break;

	case FEEDER_PICK_THROW_UP:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_THROW_Z_UP_CHK;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 535, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 536, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PICK_THROW_Z_UP_CHK:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
				if( st_handler.mn_d_piclplce != CTL_YES)
					CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
				else
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
				}
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_ON );
				if( st_handler.mn_d_piclplce != CTL_YES)
					CTL_Lib.Alarm_Error_Occurrence( 538, dWARNING, strJamCode );
				else
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
				}
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
				if( st_handler.mn_d_piclplce != CTL_YES)
					CTL_Lib.Alarm_Error_Occurrence( 539, dWARNING, strJamCode );
				else
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
				}
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
				if( st_handler.mn_d_piclplce != CTL_YES)
					CTL_Lib.Alarm_Error_Occurrence( 540, dWARNING, strJamCode );
				else
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
					}
				}
				break;
			}

			////2014,1205
			//job.feeder_step = FEEDER_PICK_THROW_Z_FINISH;
			job.feeder_step = FEEDER_STEP(181);
			////
		}
		break;

	////2014,1205
	case 181:
		{
			if( mn_pickderror[0] == CTL_YES )
			{//501150 ["PS0107_Density_Label_Right_Reject_Limit_Check_Error]
				mn_pickderror[0] = CTL_NO;
				CString strError="";
				strError.Format("5%04d0",st_io.i_density_left_label_limit1_chk);
				//CTL_Lib.Alarm_Error_Occurrence( 288, CTL_dWARNING, strError );
				//break;
				//2015.0523
				//st_msg.mstr_event_msg[0] = "[Density] Label_Reject_Limit_Error";
				//::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

				if	   (m_dRejectPos == st_basic.nRejectOffset)	st_var.m_var_reject[0] = CTL_YES;
				else if(m_dRejectPos == 0) st_var.m_var_reject[1] = CTL_YES;
				else											st_var.m_var_reject[2] = CTL_YES;
				if( st_var.m_var_reject[0] == CTL_YES && st_var.m_var_reject[1] == CTL_YES && 
					st_var.m_var_reject[2] == CTL_YES )
				{
					st_var.m_var_reject[0] = st_var.m_var_reject[1] = st_var.m_var_reject[2] = CTL_NO;
					st_msg.mstr_event_msg[0] = "[Density] Label_Reject_Limit_Error";
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				}
			}
			if( mn_pickderror[1] == CTL_YES )
			{//501150 ["PS0107_Density_Label_Right_Reject_Limit_Check_Error]
				mn_pickderror[1] = CTL_NO;
				CString strError="";
				strError.Format("5%04d0",st_io.i_density_right_label_limit2_chk);
				//CTL_Lib.Alarm_Error_Occurrence( 288, CTL_dWARNING, strError );
				//break;
				//2015.0523
// 				st_msg.mstr_event_msg[0] = "[Density] Label_Reject_Limit_Error";
// 				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				if	   (m_dRejectPos == st_basic.nRejectOffset)	st_var.m_var_reject[0] = CTL_YES;
				else if(m_dRejectPos == 0) st_var.m_var_reject[1] = CTL_YES;
				else											st_var.m_var_reject[2] = CTL_YES;
				if( st_var.m_var_reject[0] == CTL_YES && st_var.m_var_reject[1] == CTL_YES && 
					st_var.m_var_reject[2] == CTL_YES )
				{
					st_var.m_var_reject[0] = st_var.m_var_reject[1] = st_var.m_var_reject[2] = CTL_NO;
					st_msg.mstr_event_msg[0] = "[Density] Label_Reject_Limit_Error";
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				}
			}
			job.feeder_step = FEEDER_PICK_THROW_Z_FINISH;		
		}
		break;
	////

	case FEEDER_PICK_THROW_Z_FINISH:
		{

			if(mn_RetryCnt > 0)
			{
				mn_RetryCnt = 0;
				//507080 1 00 "Density Label Robot Left Vacuum1 Check Off Error."
				//2015.0317
// 				CTL_Lib.Alarm_Error_Occurrence( 1540, dWARNING, "507080" );
// 				break;
			}
			
			if( job.iRobotFirst != -1 )
			{
				memset( &st_den_rbt[job.iRobotFirst], 0x00, sizeof(st_den_rbt[job.iRobotFirst]) );
				memset( &st_den_feeder[st_basic.mn_device_type], 0x00, sizeof(st_den_feeder[st_basic.mn_device_type]));
			}
			if( job.iRobotSecond != -1)
			{
				memset( &st_den_rbt[job.iRobotSecond], 0x00, sizeof(st_den_rbt[job.iRobotSecond]) );
				memset( &st_den_feeder[st_basic.mn_device_type+3], 0x00, sizeof(st_den_feeder[st_basic.mn_device_type+3]));
			}

			
//			ALabel* pLdl_1;
//			ALabel* pLdl_2;
//			if( job.GetPosByTarget() == MPOS_INDEX_DENSITY_FEEDER )
//			{
//				pLdl_1 = &Func.m_pSite[SITE_DENSITY]->GetLabel( job.iPosFirst % MPOS_INDEX_DENSITY_FEEDER);
//				pLdl_2 = &Func.m_pSite[SITE_DENSITY]->GetLabel( (job.iPosFirst + 3) % MPOS_INDEX_DENSITY_FEEDER);
//			}
//
//			if( pLdl_1)	{
//				pLdl_1->EraseData(); 
//			}
//			if( pLdl_2) { 
//				pLdl_2->EraseData(); 
//			}
//
//			if( Func.m_pRobot[SITE_DENSITY]->IsLabelPicking() == true)
//			{
//				if( Func.m_pRobot[SITE_DENSITY]->GetPicker(0).GetLabel().IsExist() )
//				{
//					Func.m_pRobot[SITE_DENSITY]->GetPicker(0).GetLabel().EraseData();
//				}
//				if( Func.m_pRobot[SITE_DENSITY]->GetPicker(1).GetLabel().IsExist() )
//				{
//					Func.m_pRobot[SITE_DENSITY]->GetPicker(1).GetLabel().EraseData();
//				}
//			}
			Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();

		}
		break;
		

////2015.0217
	case 195:
		if(  g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF &&
			  g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_OFF ) 
		{
			CString strJamCode;
			if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF );
			}
			else if( g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_OFF )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF );
			}

// 			if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON ||
// 				g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON)
// 			{
// 				//700010 1 A "Density label1 time out from server."
// 				if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON )
// 					strJamCode.Format("7000%d0", st_basic.mn_device_type+1);
// 				else
// 					strJamCode.Format("7000%d0", st_basic.mn_device_type+3+1);
// 
// 			}
			mn_RetryCnt = 0;
			m_dPosDown = 0;
			CTL_Lib.Alarm_Error_Occurrence( 2528, dWARNING, strJamCode );
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			job.feeder_step = FEEDER_PICK_THROW_AWAY;
		}
		else
		{
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			job.feeder_step = FEEDER_PICK_PLACE_READY;
		}
		break;

	case FEEDER_PICK_PLACE_READY:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_FINISH;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
			}

// 			int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY], st_basic.nRunSpeed );
// 			int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_STEP(201);
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;

	case 201:
		{
			int nRet1 = COMI.Check_SingleMove(M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY]);
			int nRet2 = COMI.Check_SingleMove(M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY]);
			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_FINISH;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 530, CTL_dWARNING, alarm.mstr_code );
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PICK_PLACE_READY;
			}
		}
		break;

	case FEEDER_PICK_FINISH:
		{
// 			if( job.iRobotFirst != -1)
// 			{
// 				memcpy( &st_den_rbt[job.iRobotFirst].st_label , &st_den_feeder[st_basic.mn_device_type].st_label, sizeof(st_den_rbt[job.iRobotFirst].st_label));
// 				st_den_rbt[job.iRobotFirst].bPick = 1;			
// 			}
// 			if( job.iRobotSecond != -1 )
// 			{
// 				memcpy( &st_den_rbt[job.iRobotSecond].st_label , &st_den_feeder[st_basic.mn_device_type+3].st_label, sizeof(st_den_rbt[job.iRobotSecond].st_label));
// 				st_den_rbt[job.iRobotSecond].bPick = 1;
// 			}
// 			if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
// 			{
// 				if( st_den_rbt[job.iRobotFirst].st_label.nState == EMBS_FAIL || st_den_rbt[job.iRobotSecond].st_label.nState == EMBS_FAIL)
// 				{
// 					job.feeder_step = FEEDER_PICK_THROW_AWAY;
// 					break;
// 				}
// 				if( job.iRobotFirst != -1 )
// 					memset( &st_den_feeder[st_basic.mn_device_type], 0x00, sizeof(st_den_feeder[st_basic.mn_device_type]));
// 				if( job.iRobotSecond != -1 )
// 					memset( &st_den_feeder[st_basic.mn_device_type+3], 0x00, sizeof(st_den_feeder[st_basic.mn_device_type+3]));
// 			}

			Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();

			Func.Handler_Recovery_Data_Write();
		}
		break;

	case FEEDER_PLACE_READY:
		{
			//NetWork로 유통라벨 바코드를 받아온다
//			if( st_handler.mn_BcrOutAllow1 != CTL_YES)
//				st_handler.mn_BcrOutAllow1 = CTL_YES;
//			if( GetDensityPick() == RDP_PICK)
//			{
				job.feeder_step = FEEDER_PLACE_VACUUM_ON_CHK;
				if( g_lotMgr.GetLotCount() <= 0 && st_work.mn_tl_status == dSELFCHECK )
				{
				}
//				m_dwTime_move = GetCurrentTime();
//			}
		}
		break;

	case FEEDER_PLACE_VACUUM_ON_CHK:
//		if( GetCurrentTime() - m_dwTime_move < 0) m_dwTime_move = GetCurrentTime();
//		else if( GetCurrentTime() - m_dwTime_move < 100) break;

		job.feeder_step = FEEDER_PLACE_MOVE_CHK;
		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;	
		if( st_handler.n_dryrun_density == CTL_NO && st_basic.n_mode_device &&
			( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON || g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_ON ) &&
			(g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON || g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_ON ) )
		{
			job.feeder_step = FEEDER_PLACE_MOVE_CHK;
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		}
		break;

	case FEEDER_PLACE_MOVE_CHK:
		mn_error_flag = CTL_NO;//2015.0222
		job.feeder_step = (FEEDER_STEP)321;// FEEDER_PLACE_MOVE;
		{
			int nPicknum = 0, nPass = 0;
			if( job.iRobotFirst != -1)
			{
				nPicknum++;
				if( st_den_rbt[job.iRobotFirst].st_label.nState == EMBS_PASS )
					nPass++;
			}

			if( job.iRobotSecond != -1)
			{
				nPicknum++;
				if( st_den_rbt[job.iRobotSecond].st_label.nState == EMBS_PASS )
					nPass++;
			}

			if( nPicknum != nPass) 
			{
				mn_error_flag = CTL_YES;
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_ERROR_MOVE;
			}
			else if( (st_var.n_use_density_flag[st_basic.mn_device_type] == CTL_YES && st_var.n_use_density_count[st_basic.mn_device_type] < 1 ) ||
				(st_var.n_use_density_flag[st_basic.mn_device_type+3] == CTL_YES && st_var.n_use_density_count[st_basic.mn_device_type+3] < 1 ) )
			{
// 				st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
// 				st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
// 				st_var.n_use_density_count[st_basic.mn_device_type] = 0;//reset
// 				st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;//reset
				mn_error_flag = CTL_YES;
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_ERROR_MOVE;
			}
//  			else if(st_var.mn_new_density_flag == CTL_YES)
//  			{
// 				mn_error_flag = CTL_YES;
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
//  				job.feeder_step = FEEDER_PLACE_ERROR_MOVE;
//  			}
			else if( st_var.mn_lotend_density_flag == CTL_YES)//2015.0405
			{
				mn_error_flag = CTL_YES;
				st_var.mn_lotend_density_flag = CTL_NO;
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_ERROR_MOVE;
			}

		}
		break;

	case 321:
		{
			int nRet1 = CTL_Lib.Single_Move( M_ULD_ROLLER_PITCH, st_motor[ M_ULD_ROLLER_PITCH].d_pos[P_GUIDE_LOAD_POS], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_MOVE;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 583, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PLACE_MOVE:
		{

			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_Z_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 531, CTL_dWARNING, alarm.mstr_code );
			}

// 			int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY], st_basic.nRunSpeed );
// 			int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_STEP(331);
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 531, CTL_dWARNING, alarm.mstr_code );
// 			}

		}
		break;

	case 331:
		{
			int nRet1 = COMI.Check_SingleMove(M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY]);
			int nRet2 = COMI.Check_SingleMove(M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY]);
			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_Z_DOWN;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 5532, CTL_dWARNING, alarm.mstr_code );
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_MOVE;
			}
		}
		break;

	case FEEDER_PLACE_ERROR_MOVE:
		{
			if	   ( st_var.m_var_reject[0] == CTL_NO ) m_dRejectPos = st_basic.nRejectOffset;
			else if( st_var.m_var_reject[1] == CTL_NO ) m_dRejectPos = 0;
			else										m_dRejectPos = -st_basic.nRejectOffset;
			job.feeder_step = (FEEDER_STEP)341;
		}
		break;

	case 341:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS] + m_dRejectPos;
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_PLACE_Z_DOWN;
				job.feeder_step = FEEDER_STEP(342);
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 532, CTL_dWARNING, alarm.mstr_code );
			}
// 			int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS], st_basic.nRunSpeed );
// 			int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_STEP(342);
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 532, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;

	case 342:
		{
// 			int nRet1 = COMI.Check_SingleMove(M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS]);
// 			int nRet2 = COMI.Check_SingleMove(M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS]);
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_PLACE_Z_DOWN;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 5533, CTL_dWARNING, alarm.mstr_code );
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_PLACE_ERROR_MOVE;
// 			}
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_ERR], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_ERR], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				
				mn_pickderror[0] = mn_pickderror[1] = CTL_NO;
				job.feeder_step = FEEDER_STEP(343);
				
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 591, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 592, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 343:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		m_dwTime_move = GetCurrentTime();
		job.feeder_step = FEEDER_STEP(344);
		break;

	case 344:
		{
			if( GetCurrentTime() - m_dwTime_move < 0) m_dwTime_move = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTime_move < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_OFF]) break;
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_ON );
			
			
			job.feeder_step = FEEDER_STEP(345);
		}
		break;

	case 345:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_STEP(346);
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 593, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 594, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 346:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 595, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 596, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 597, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 598, dWARNING, strJamCode );
				break;
			}
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );
			
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			mn_RetryCnt = 0;

			job.feeder_step = (FEEDER_STEP)347;// FEEDER_PLACE_FINISH;
		}
		break;	
		
	case 347:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;
			
			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;
			
			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				memset( &st_den_rbt[0], 0x00, sizeof(st_den_rbt[0]));
				memset( &st_den_rbt[1], 0x00, sizeof(st_den_rbt[1]));
				
				if(st_handler.n_only_work_density == CTL_YES)
				{
					st_sync.n_density_work_req = CTL_READY;
				}
				
				Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();
				Func.Handler_Recovery_Data_Write();
				m_dwTimeTact = GetCurrentTime();				
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 599, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PLACE_Z_DOWN:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_F], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				job.feeder_step = FEEDER_PLACE_VACUUM_OFF;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PLACE_VACUUM_OFF:
		{
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_F], (int)st_basic.nRunSpeed );
			if( nRet2 == BD_GOOD )
			{
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
				m_dwTime_move = GetCurrentTime();

				job.feeder_step = FEEDER_PLACE_VACUUM_OFF_CHK;
			}
			else if( (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case FEEDER_PLACE_VACUUM_OFF_CHK:
		{
			if( GetCurrentTime() - m_dwTime_move < 0) m_dwTime_move = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTime_move < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_OFF]) break;
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_ON );
			
			
			job.feeder_step = FEEDER_PLACE_UP;
		}
		break;

	case FEEDER_PLACE_UP:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				job.feeder_step = FEEDER_PLACE_UP_CHK;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 535, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 536, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PLACE_UP_CHK:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
				break;
			}
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );

			job.feeder_step = (FEEDER_STEP)391;// FEEDER_PLACE_FINISH;
		}
		break;

	case 391:
		{
			int nRet1 = CTL_Lib.Single_Move( M_ULD_ROLLER_PITCH, st_motor[ M_ULD_ROLLER_PITCH].d_pos[P_GUIDE_UNLOAD_POS], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				job.feeder_step = FEEDER_PLACE_FINISH;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 584, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case FEEDER_PLACE_FINISH:
		{
			//2015.0222
			if(mn_error_flag == CTL_NO)
			{
				st_sync.n_density_work_req = CTL_REQ;
			}
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			mn_RetryCnt = 0;
			job.feeder_step = FEEDER_PLACE_FINISH_READY;

		}
		break;

	case FEEDER_PLACE_FINISH_READY:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;
			
			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;
			
			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				if( job.iRobotFirst != -1 )
				{
					memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_label[job.iRobotFirst], &st_den_rbt[job.iRobotFirst].st_label, sizeof(st_den_rbt[job.iRobotFirst].st_label));
					memset( &st_den_rbt[job.iRobotFirst], 0x00, sizeof(st_den_rbt[job.iRobotFirst]));
				}
				if( job.iRobotSecond != -1)
				{
					memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_label[job.iRobotSecond], &st_den_rbt[job.iRobotSecond].st_label, sizeof(st_den_rbt[job.iRobotSecond].st_label));
					memset( &st_den_rbt[job.iRobotSecond], 0x00, sizeof(st_den_rbt[job.iRobotSecond]));
				}

				if(st_handler.n_only_work_density == CTL_YES)
				{
					st_sync.n_density_work_req = CTL_READY;
				}				
				
				Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();
				Func.Handler_Recovery_Data_Write();
				
// 				g_handler.CheckLotEnd();

				if(GetCurrentTime() - m_dwTimeTact <= 0 ) m_dwTimeTact = GetCurrentTime();
				if( GetCurrentTime() - m_dwTimeTact >= 6100 && GetCurrentTime() - m_dwTimeTact < 900000 &&
					st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "[DENSITY]Tact time : %d", GetCurrentTime() - m_dwTimeTact);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
				}
				m_dwTimeTact = GetCurrentTime();

			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 590, CTL_dWARNING, alarm.mstr_code );
			}

// 			int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type], st_basic.nRunSpeed );
// 			int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_PLACE_FINISH_READY_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 590, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;

	case FEEDER_PLACE_FINISH_READY_CHK:
		{
// 			int nRet1 = COMI.Check_SingleMove(M_D_ATTACH_X, st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type]);
// 			int nRet2 = COMI.Check_SingleMove(M_D_ATTACH_Y, st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type]);
// 			if( nRet1 == BD_GOOD ) mn_MotFlag[0] = CTL_YES;
// 			if( nRet2 == BD_GOOD ) mn_MotFlag[1] = CTL_YES;
// 			if(mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)
// 			{
// 				if( job.iRobotFirst != -1 )
// 				{
// 					memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_label[job.iRobotFirst], &st_den_rbt[job.iRobotFirst].st_label, sizeof(st_den_rbt[job.iRobotFirst].st_label));
// 					memset( &st_den_rbt[job.iRobotFirst], 0x00, sizeof(st_den_rbt[job.iRobotFirst]));
// 				}
// 				if( job.iRobotSecond != -1)
// 				{
// 					memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_label[job.iRobotSecond], &st_den_rbt[job.iRobotSecond].st_label, sizeof(st_den_rbt[job.iRobotSecond].st_label));
// 					memset( &st_den_rbt[job.iRobotSecond], 0x00, sizeof(st_den_rbt[job.iRobotSecond]));
// 				}
// 				
// 				if(st_handler.n_only_work_density == CTL_YES)
// 				{
// 					st_sync.n_density_work_req = CTL_READY;
// 				}				
// 				
// 				Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();
// 				Func.Handler_Recovery_Data_Write();
// 				
// 				// 				g_handler.CheckLotEnd();
// 				
// 				if(GetCurrentTime() - m_dwTimeTact <= 0 ) m_dwTimeTact = GetCurrentTime();
// 				if( GetCurrentTime() - m_dwTimeTact >= 0 && GetCurrentTime() - m_dwTimeTact < 900000 &&
// 					st_handler.cwnd_list != NULL)
// 				{
// 					sprintf(st_msg.c_normal_msg, "[DENSITY]Tact time : %d", GetCurrentTime() - m_dwTimeTact);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 					Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
// 				}
// 				m_dwTimeTact = GetCurrentTime();
// 			}
// 			else if (nRet1 == BD_RETRY || nRet2 == BD_RETRY)
// 			{
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_PLACE_FINISH_READY;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				mn_RetryCnt++;
// 				if(mn_RetryCnt > st_basic.mn_network_retry_cnt)
// 				{
// 					mn_RetryCnt = 0;
// 					CTL_Lib.Alarm_Error_Occurrence( 5590, CTL_dWARNING, alarm.mstr_code );
// 				}
// 				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
// 				job.feeder_step = FEEDER_PLACE_FINISH_READY;
// 			}

		}
		break;

	}
}


//void CRun_Density_Works::CheckReadyBacr()
//{
//	int iIdx = g_Tray.GetIdxByPos(IDPOS_DENSITY);
//	ATray trayPri = g_Tray.GetTray( iIdx );
//	for( int ix = 0; ix < MAX_BUFFDVC; ix++ )
//	{
//		if( trayPri.GetModule(ix).IsExist() && trayPri.GetModule(ix).IsTestLoadState() )
//		{
//			trayPri.GetModule(ix).GetSerial();
//		}
//	}
//
//}
//
void CRun_Density_Works::Robot_Density_Pickup()
{
	switch( mn_PickStep )
	{
	case 0:
		{
			double dpos1 = COMI.Get_MotCurrentPos( M_D_ATTACH_Z1);
			double dpos2 = COMI.Get_MotCurrentPos( M_D_ATTACH_Z2);

			if( dpos1 > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY])
			{
				mn_PickStep = 100;
			}
			else if( dpos2 > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY])
			{
				mn_PickStep = 200;
			}
			else
			{
				mn_PickStep = 1000;
			}
		}
		break;

	case 100:
		{
			int nRet = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				mn_PickStep = 200;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 541, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 200:
		{
			int nRet = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				mn_PickStep = 0;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 542, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1000:
		if( g_lotMgr.GetLotCount() > 0 && g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES" )
		{
			if( g_Run_Density_Feeders.m_bPrint_Pickup[st_basic.mn_device_type] && g_Run_Density_Feeders.m_bPrint_Pickup[st_basic.mn_device_type+3] )
			{
				mn_PickStep = 3000;
			}
		}
		break;

	case 3000:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_PickStep = 3100;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 543, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 3100:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			else if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_PickStep = 3200;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 544, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 545, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 3200:
		if(st_basic.n_mode_device == 1)
		{
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_ON );
	// 		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_ON );
	// 		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_ON );
		}

		m_dwTime_Pick = GetCurrentTime();
		mn_PickStep = 3300;
		break;

	case 3300:
		if( GetCurrentTime() - mn_PickStep < 0)  mn_PickStep = GetCurrentTime();
		else if( GetCurrentTime() - mn_PickStep < 100) break;		

//		g_ioMgr.set_out_bit( st_io.o_density_plate_vacuum1_OnOff[st_basic.mn_device_type], IO_OFF );
//		g_ioMgr.set_out_bit( st_io.o_density_plate_blow_OnOff[st_basic.mn_device_type], IO_ON );
		m_dwTime_Pick = GetCurrentTime();
		mn_PickStep = 3400;
		break;

	case 3400:
		if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_ON )
		{
			mn_PickStep = 3500;
			m_dwTime_Pick = GetCurrentTime();
		}
		else if( GetCurrentTime() - m_dwTime_Pick > st_time.n_limit_time[E_WAIT_FEEDER_VACUUM] )
		{
			CString strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
			CTL_Lib.Alarm_Error_Occurrence( 546, dWARNING, strJamCode );
		}
		else if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();		

		break;

	case 3500:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_ON]) break;

		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		mn_PickStep = 4000;
		break;

	case 4000:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			else if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_PickStep = 4100;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 547, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 548, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 4100:
		if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_ON )
		{
			mn_PickStep = 4200;
		}
		else
		{
			CString strJamCode;
			if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
			}
			else if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
			}
			else if( g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
			}
			else
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
			}
			CTL_Lib.Alarm_Error_Occurrence( 549, dWARNING, strJamCode );
			//버리고 다시한다. 추후
		}
		break;

	case 4200:
		SetDenSityPick( RDP_PICK);
		mn_PickStep = 4300;
		break;

	case 4300:
		if( GetDensityPick() == RDP_NONE )
		{
			mn_PickStep = 0;
		}
		break;
	}
}

void CRun_Density_Works::Run_Reinstate()
{
	if(st_handler.mn_Rcvy_state[RCVY_DENSITY_RBT] == CTL_YES) return;
	int nRet[4];

	switch(mn_RcvyStep) {
	case 0:
		{
			nRet[0] = COMI.Check_MotPosRange(M_D_ATTACH_X, COMI.md_cmdpos_backup[M_D_ATTACH_X], COMI.md_allow_value[M_D_ATTACH_X]);
			nRet[1] = COMI.Check_MotPosRange(M_D_ATTACH_Y, COMI.md_cmdpos_backup[M_D_ATTACH_Y], COMI.md_allow_value[M_D_ATTACH_Y]);
			nRet[2] = COMI.Check_MotPosRange(M_D_ATTACH_Z1, COMI.md_cmdpos_backup[M_D_ATTACH_Z1], COMI.md_allow_value[M_D_ATTACH_Z1]);
			nRet[3] = COMI.Check_MotPosRange(M_D_ATTACH_Z2, COMI.md_cmdpos_backup[M_D_ATTACH_Z2], COMI.md_allow_value[M_D_ATTACH_Z2]);

			bool nRecy = false;
			for( int i = 0; i < 4; i++)
			{
				m_nReq_Rcvy[i] = CTL_NO;
				if(nRet[i] == BD_ERROR)
				{
					m_nReq_Rcvy[i] = CTL_YES;
					nRecy = true;
				}
			}

			if(nRecy == false) mn_RcvyStep = 9000;
			else
			{
				m_dPos_TmpRcvyZ1 = COMI.md_cmdpos_backup[M_D_ATTACH_Z1];
				m_dPos_TmpRcvyZ2 = COMI.md_cmdpos_backup[M_D_ATTACH_Z2];
				mn_RcvyStep = 100;
			}
		}
		break;

	case 100:
		{
			int nRet1 = CTL_Lib.Single_Move(M_D_ATTACH_Z1, st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_D_ATTACH_Z2, st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				COMI.md_cmdpos_backup[M_D_ATTACH_Z1] = m_dPos_TmpRcvyZ1;
				COMI.md_cmdpos_backup[M_D_ATTACH_Z2] = m_dPos_TmpRcvyZ2;
				mn_RcvyStep = 1000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1000:
		{
			if (m_nReq_Rcvy[0] == CTL_NO)
			{
				mn_RcvyStep = 2000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_D_ATTACH_X, COMI.md_cmdpos_backup[M_D_ATTACH_X], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
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
		{
			if (m_nReq_Rcvy[1] == CTL_NO)
			{
				mn_RcvyStep = 3000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_D_ATTACH_Y, COMI.md_cmdpos_backup[M_D_ATTACH_Y], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				mn_RcvyStep = 3000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 3000:
		{
			if (m_nReq_Rcvy[2] == CTL_NO && m_nReq_Rcvy[3] == CTL_NO)
			{
				mn_RcvyStep = 9000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_D_ATTACH_Z1, COMI.md_cmdpos_backup[M_D_ATTACH_Z1], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_D_ATTACH_Z2, COMI.md_cmdpos_backup[M_D_ATTACH_Z2], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_RcvyStep = 9000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 9000:
		st_handler.mn_Rcvy_state[RCVY_DENSITY_RBT] = CTL_YES;
		mn_RcvyStep = 0;
		break;	
	}

}

void CRun_Density_Works::Run_Restore()
{
	if( st_handler.mn_init_state[INIT_DENDITY_ROBOT] != CTL_NO )	return;
	switch(mn_RestoreStep)
	{
	case EDRR_START:			mn_RestoreStep = EDRR_Z_SAFETY; break;
	case EDRR_Z_SAFETY:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_RestoreStep = EDRR_VACUUM_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case EDRR_VACUUM_CHK:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF) == IO_OFF)
			{
				mn_RestoreStep = EDRR_VACUUM_OFF;
			}
			else
			{
				mn_RestoreStep = EDRR_MOVE_ERR;
			}
		}
		break;

	case EDRR_MOVE_ERR:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_RestoreStep = EDRR_Z_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 532, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case EDRR_Z_DOWN:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_ERR], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_ERR], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_RestoreStep = EDRR_DOWN_VACUUM_OFF;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case EDRR_DOWN_VACUUM_OFF:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		m_dwTime_move = GetCurrentTime();
		mn_RestoreStep = EDRR_DOWN_VACUUM_OFF_CHK;
		break;

	case EDRR_DOWN_VACUUM_OFF_CHK:
		if( GetCurrentTime() - m_dwTime_move < 0) m_dwTime_move = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_move < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_OFF]) break;
		mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
		mn_RestoreStep = EDRR_Z_UP;
		break;

	case EDRR_Z_UP:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_MotFlag[0] = CTL_YES;
			}
			if( nRet2 == BD_GOOD )
			{
				mn_MotFlag[1] = CTL_YES;
			}
			if (mn_MotFlag[0] == CTL_YES && mn_MotFlag[1] == CTL_YES)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
				mn_RestoreStep = EDRR_Z_UP_CHK;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 535, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 536, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case EDRR_Z_UP_CHK:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 538, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 539, dWARNING, strJamCode );
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 2540, dWARNING, strJamCode );
				break;
			}

			mn_RestoreStep = EDRR_FINISH;
		}
		break;


	case EDRR_VACUUM_OFF:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		mn_RestoreStep = EDRR_FINISH;
		break;

	case EDRR_FINISH:
		for ( int i = 0; i < Func.m_pRobot[SITE_DENSITY]->GetJobCount(); i++)
		{
			Func.m_pRobot[SITE_DENSITY]->EraseFirstJob();
		}		

		st_handler.mn_init_state[INIT_DENDITY_ROBOT] = CTL_YES;
		mn_RestoreStep = EDRR_START;
		break;

	}
}

void CRun_Density_Works::OnRobot_FinalPos()
{
}

void CRun_Density_Works::Serialize(CArchive &ar)
{
}



