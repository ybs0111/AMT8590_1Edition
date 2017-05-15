// CRun_Roller_Works.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Roller_Works.h"
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
// CRun_Roller_Works

IMPLEMENT_SERIAL(CRun_Roller_Works, CObject, 1)
CRun_Roller_Works g_Run_Roller_Works;
CRun_Roller_Works::CRun_Roller_Works()
{
	Thread_Variable_Initial();
}

CRun_Roller_Works::~CRun_Roller_Works()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_Roller_Works message handlers
void CRun_Roller_Works::Thread_Variable_Initial()
{
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;

}

void CRun_Roller_Works::Thread_Run()
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

	for ( int j = 0; j < 2; j++ )
	{
		for ( int k = 0; k < 2; k++)
		{
// 			if(j != 0) //2014.1018 rolling align pusher ¨ú©ªA¨ö
// 			{
				Run_Move_Rolling_FwdBwd( j , k );
// 			}
		}
	}

}

void CRun_Roller_Works::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_ROLLER_ROBOT] != CTL_NO )	return;

	switch(mn_InitStep)
	{
	case ERRI_START:	mn_InitStep = ERRI_CHECK_ROBOT_CHK; break;

	case ERRI_CHECK_ROBOT_CHK: ERRI_Check_Rbt_Chk(); break;

	case ERRI_ROLLING_BACK: ERRI_Rolling_Backward(); break;

	case ERRI_ROLLING_BACK_CHK: ERRI_Rolling_Backward_Check(); break;
	case ERRI_HOMECHK_ROLLING1: ERRI_HomeChk_Rolling( M_INDEX_ROLLER1 ); break;
	case ERRI_HOMECHK_WAIT: 
			m_dwTimeInit = GetCurrentTime(); 
			mn_InitStep = ERRI_HOMECHK_DONE;
			break;
	case ERRI_HOMECHK_DONE:
		if( GetCurrentTime() - m_dwTimeInit <= 0 )  m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit > 1000) mn_InitStep = ERRI_HOMECHK_ROLLING2;
		break;		

	case ERRI_HOMECHK_ROLLING2: 
		ERRI_HomeChk_Rolling( M_INDEX_ROLLER2 ); 
		break;

	case ERRI_HOMECHK_ROLL: 
		ERRI_HomeChk_Rolling( M_INDEX_X ); 
		mn_mot_flag[0] = mn_mot_flag[1] = CTL_NO;
		break;

	case ERRI_HOMECHK_ROLL_SAFETY:
		{
			int nRet1 = CTL_Lib.Single_Move( M_INDEX_ROLLER1, st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY], st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_INDEX_ROLLER2, st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_SAGETY], st_basic.nManualSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0] = CTL_YES;
			if(nRet2 == BD_GOOD) mn_mot_flag[1] = CTL_YES;
			if( mn_mot_flag[0] == CTL_YES && mn_mot_flag[1] == CTL_YES )
			{
				mn_InitStep = ERRI_FINISH;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 9341, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;		
	
	case ERRI_FINISH: ERRI_Finish(); break;
	}

}

void CRun_Roller_Works::ERRI_Check_Rbt_Chk()
{
	if( st_handler.mn_init_state[INIT_BARCODE_FEEDER] == CTL_NO ) return;//2014.1018

	mn_InitStep = ERRI_ROLLING_BACK;
}

//void CRun_Roller_Works::ERRI_Rolling_Backward()
//{
//	ReqRollingFwdBwd( ERS_1, BACKWARD );
//	ReqRollingFwdBwd( ERS_2, BACKWARD );
//	ReqRollingFwdBwd( ERS_3, BACKWARD );
//	mn_InitStep = ERRI_ROLLING_BACK_CHK;
//}

void CRun_Roller_Works::ERRI_Rolling_Backward()
{
	ReqRollingFwdBwd( 0, 0, BACKWARD );
	ReqRollingFwdBwd( 0, 1, BACKWARD );
	ReqRollingFwdBwd( 1, 1, BACKWARD );
	ReqRollingFwdBwd( 1, 1, BACKWARD );
	mn_InitStep = ERRI_ROLLING_BACK_CHK;
}

void CRun_Roller_Works::ERRI_Rolling_Backward_Check()
{
//	if( GetRollingFwdBwd ( ERS_1 ) != FB_NONE) return;
	if( GetRollingFwdBwd ( 0, 0 ) != FB_NONE) return;
	if( GetRollingFwdBwd ( 0, 1 ) != FB_NONE) return;
	if( GetRollingFwdBwd ( 1, 0 ) != FB_NONE) return;
	if( GetRollingFwdBwd ( 1, 1 ) != FB_NONE) return;
	mn_InitStep = ERRI_HOMECHK_ROLLING1;
}

//void CRun_Roller_Works::ReqRollingFwdBwd( int nSite, EFB efb)
//{
//	m_fbrollingfwdbwd[nSite] = efb;
//	m_step_rolling_fwdbwd[nSite] = ESTEP_CYL_START;
//}

void CRun_Roller_Works::ReqRollingFwdBwd( int nSite, int pos, EFB efb )
{
	m_fbrollingfwdbwd[nSite][pos] = efb;
	m_step_rolling_fwdbwd[nSite][pos] = ESTEP_CYL_START;
}

//void CRun_Roller_Works::SetRollingFwdBwd( int nSite, int iOnOff )
//{
//	int nRoll = st_io.o_rolling_push_fwdback[nSite];
//
//	g_ioMgr.set_out_bit( nRoll, iOnOff);
//}

void CRun_Roller_Works::SetRollingFwdBwd( int nSite, int pos, int iOnOff )
{
	int nRoll = st_io.o_rolling_push_fwdback[nSite][pos];

	g_ioMgr.set_out_bit( nRoll, iOnOff);
}

bool CRun_Roller_Works::CheckRollingFwdBwd( int nSite, int nPos, int iOnOff )
{
	int nRoll_1 = st_io.i_rolling_push_fwd_chk[nSite][nPos];
//	int nRoll_2 = st_io.i_rolling_push_fwd_chk[nSite][ERLP_RIGHT];
	int nRoll_3 = st_io.i_rolling_push_bwd_chk[nSite][nPos];
//	int nRoll_4 = st_io.i_rolling_push_bwd_chk[nSite][ERLP_RIGHT];

	if( g_ioMgr.get_in_bit( nRoll_1 ) != iOnOff) return false;
//	if( g_ioMgr.get_in_bit( nRoll_2 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nRoll_3 ) != !iOnOff) return false;
//	if( g_ioMgr.get_in_bit( nRoll_4 ) != !iOnOff) return false;

	return true;
}

void CRun_Roller_Works::ERRI_HomeChk_Rolling( int nAxis )
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
//	if( nAxis == M_INDEX_ROLLER2) nRet = BD_GOOD;//2014,1201
	if( nRet == BD_GOOD )
	{
		if	   ( nAxis == M_INDEX_ROLLER1)
		{
			Func.OnInitBtn( RBTN_INDEX_ROLLER1 );//2014.1018
			mn_InitStep = ERRI_HOMECHK_WAIT;
		}
		else if( nAxis == M_INDEX_ROLLER2) 
		{
			Func.OnInitBtn( RBTN_INDEX_ROLLER2 );//2014.1018
			mn_InitStep = ERRI_HOMECHK_ROLL;
		}
		else if( nAxis == M_INDEX_X) 
		{
			Func.OnInitBtn( RBTN_INDEX_X );//2014.1018
			mn_InitStep = ERRI_HOMECHK_ROLL_SAFETY;
		}

//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//370001 0 00 "Index X Axis Motor Home Check Error."
		//380001 0 00 "Index Rolling1 Axis Motor Home Check Error."
		//390001 0 00 "Index Rolling2 Axis Motor Home Check Error."
		CString strJamCode = "370001";
		if	   ( nAxis == M_INDEX_ROLLER1) strJamCode = "380001";
		else if( nAxis == M_INDEX_ROLLER2) strJamCode = "390001";
		CTL_Lib.Alarm_Error_Occurrence(800	, CTL_dWARNING, strJamCode );
	}
}

void CRun_Roller_Works::ERRI_Finish()
{
	Func.OnInitBtn( RBTN_INDEX );//2014.1018
	st_handler.mn_init_state[INIT_ROLLER_ROBOT] = CTL_YES;
	mn_InitStep = ERRI_START;
}


void CRun_Roller_Works::Run_Move_Rolling_FwdBwd(int nSite, int nPos)
{
	if(m_fbrollingfwdbwd[nSite][nPos] == FB_NONE) return;
	
	int iOnOff = IO_ON;
	
	if( m_fbrollingfwdbwd[nSite][nPos] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_rolling_fwdbwd[nSite][nPos])
	{
	case ESTEP_CYL_START: m_step_rolling_fwdbwd[nSite][nPos] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetRollingFwdBwd( nSite, nPos, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_rolling_fwdback[nSite][nPos] = iOnOff;
			}
			m_step_rolling_fwdbwd[nSite][nPos] = ESTEP_CYL_CHK;
			m_dwTimeRollingFwdBwdStart[nSite][nPos] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckRollingFwdBwd( nSite, nPos, iOnOff ) )
			{
				m_step_rolling_fwdbwd[nSite][nPos] = ESTEP_CYL_FINISH;
				m_dwTimeRollingFwdBwd[nSite][nPos] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeRollingFwdBwdStart[nSite][nPos] > st_time.n_limit_time[E_WAIT_ROLLING_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Rolling_FwdBwd_Time(st_io.i_rolling_push_fwd_chk[nSite][nPos], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 225, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeRollingFwdBwdStart[nSite][nPos] < 0 )
			{
				m_dwTimeRollingFwdBwdStart[nSite][nPos] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeRollingFwdBwd[nSite][nPos] < 0 )
				m_dwTimeRollingFwdBwd[nSite][nPos] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeRollingFwdBwd[nSite][nPos] < st_time.n_wait_time[E_WAIT_ROLLING_FWDBWD][iOnOff] )
				return;

			m_step_rolling_fwdbwd[nSite][nPos] = ESTEP_CYL_START;
			m_fbrollingfwdbwd[nSite][nPos] = FB_NONE;
		}
		break;	
	}
}


void CRun_Roller_Works::Run_Move()
{

}


void CRun_Roller_Works::Run_Restore()
{
}

void CRun_Roller_Works::OnRobot_FinalPos()
{
}

void CRun_Roller_Works::Serialize(CArchive &ar)
{
}

