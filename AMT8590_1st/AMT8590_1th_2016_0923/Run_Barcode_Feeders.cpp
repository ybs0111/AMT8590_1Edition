// Run_Barcode_Feeders.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Barcode_Feeders.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "Public_Function.h"


#include "CtlBd_Function.h"
#include "CtlBd_Library.h"
#include "Cmmsdk.h"
#include "CmmsdkDef.h"
#include "COMI_Manager.h"
#include "IO_Manager.h"
#include "AMTLotManager.h"

#include "AMTRegistry.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"

#include "VisionClient.h"
#include "VisionServer.h"
#include "BarcodeFormat.h"

#include "ZebraPrint.h"
#include "RunNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Barcode_Feeders

IMPLEMENT_SERIAL(CRun_Barcode_Feeders, CObject, 1)
AFeedertRobot g_feeder_robot;
CRun_Barcode_Feeders g_Run_Barcode_Feeders;


AFeedertRobot::AFeedertRobot()
{
}

AFeedertRobot::~AFeedertRobot()
{
}

void AFeedertRobot::AddJob( int iRobotFirst, int iPosFirst )
{
	if( iRobotFirst < 0 )
	{
		return;
	}

	stRejectPicker st;
	st.Set( iRobotFirst, iPosFirst );

	m_vecJob.push_back( st );
}

CRun_Barcode_Feeders::CRun_Barcode_Feeders()
{
	Thread_Variable_Initial();
}

CRun_Barcode_Feeders::~CRun_Barcode_Feeders()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_Barcode_Feeders message handlers
void CRun_Barcode_Feeders::Thread_Variable_Initial()
{
	mn_InitStep = 0;
	mn_RunStep1 = 0;
	mn_RunStep2 = 0;
	mn_RestoreStep = 0;
	mn_RejectStep = 0;
	mn_fmt_step = 0;//2014,1222
	mn_comm_pos = 0;
	mn_step_vis = 0;
	m_nRejectConinueErr = 0;

	m_nRejectConvey = CTL_NO;
	st_sync.n_bcr_vision_req = CTL_NO;
	for ( int i = 0; i < 4; i++ )
	{
		mb_BcrComplete[i] = false;
		m_step_bcrfeeder[i] = ESTEP_FEEDER_NONE;
		m_fbFeeder[i] = EPRI_NONE;
		m_bPrint_Pickup[i] = false;
		n_bcr_vision_req[i] = CTL_NO;
		//2015.0219
		m_continus_vis_error[i] = 0;
		m_total_vis_error[i] = 0;

		st_handler.m_dwBcrOutTime[i] = GetCurrentTime();
	}

	st_handler.mn_BcrOutAllow1 = CTL_NO; //1,2 출력 허락
	st_handler.mn_BcrOutAllow2 = CTL_NO; //3,4 출력 허락


	m_vecbcrRev.clear();
	m_vecbcrRev2.clear();

	mn_iBUF = -1;
	mn_iBUF2 = -1;

	for ( int j = 0; j < 4; j++)
	{
		for ( int k = 0; k < 4; k++)
		{
			m_nRetryCnt[j][k] = 0;
		}

	}
}

void CRun_Barcode_Feeders::Thread_Run()
{
	if(COMI.mn_run_status != dRUN/* && COMI.mn_run_status != dINIT*/)
	{
		for ( int i = 0;i < 4; i++)
		{
			Run_Move_Feeder_Lock(i);
		}
		
	}

	if( Func.OnIsInterruptCondition() != 1 )
		return;
	
	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
		{
			ControlRejectRobot();
			if( st_var.n_newbcr_format == CTL_YES)
			{
				int n_ret = OnGetFmtfromServer();
				if( n_ret == CTL_GOOD)
				{
					st_var.n_newbcr_format = CTL_NO;
				}
			}
			for ( int i = 0; i < 2; i++)
			{
				Run_Move(i);
 				Run_Move2(i);
			}
			Run_Move_Vision();
			Run_Move_Reject();
			for ( int j = 0; j < 4; j++)
			{
				Run_Move_Bcrcomm(j);
				Run_Move_Bcr_out(j);
				Run_Move_Feeder(j);//2014.1018 배선 안됨
				//Run_Move_Print_Status(j);
			}
		}
		break;
	
	case dRESTORE:
		//Run_Restore();
		break;
		
	default:
		if (st_handler.mn_menu_num != 501)
		{
			
		}
		
		break;
	}


	for ( int i = 0;i < 4; i++)
	{
		Run_Move_Feeder_Lock(i);
		Run_Move_Bcr_Plate_UpDown(i);
	}
	OnRejectConveyMove();//2014.1018
	Run_Move_Rej_Vac_Down();//2014.1017

//	Thread_Variable_Initial();
}

void CRun_Barcode_Feeders::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_BARCODE_FEEDER] != CTL_NO )	return;
	
	switch(mn_InitStep)
	{
	case EBFI_START: mn_InitStep = EBFI_CHECK_ROBOT_CHK; break;

	case EBFI_CHECK_ROBOT_CHK: EBFI_Check_Rbt_Chk(); break;

	case EBFI_HOMECHK_VISION_X: EBFI_HomeCheck_Vision_X(); break;

	case EBFI_MOVE_SAFETY_X: EBFI_MoveSafety_X(); break;

	case EBFI_FEEDER_UNLOCK: EBFI_BcrFeeder_Lock(FORWARD); break; //2014.1018 BACKWARD -> FORWARD

	case EBFI_FEEDER_UNLOCK_CHK: EBFI_BcrFeeder_Lock_chk(BACKWARD); break;

	case EBFI_FEEDER_LOCK: EBFI_BcrFeeder_Lock(BACKWARD); break;//2014.1018 FORWARD - >BACKWARD

	case EBFI_FEEDER_LOCK_CHK: EBFI_BcrFeeder_Lock_chk(FORWARD); break;

	case EBFI_PLATE_DOWN: EBFI_Plate_Down(); break;

	case EBFI_PLATE_DOWN_CHK: EBFI_Plate_Down_Check(); break;

	case EBFI_HOMECHK_FEERDE1: EBFI_HomeCheck_BCR_Feeder( M_BCR_FEEDER_1 ); break;
	case EBFI_HOMECHK_FEERDE2: EBFI_HomeCheck_BCR_Feeder( M_BCR_FEEDER_2 ); break;
	case EBFI_HOMECHK_FEERDE3: EBFI_HomeCheck_BCR_Feeder( M_BCR_FEEDER_3 ); break;
	case EBFI_HOMECHK_FEERDE4: EBFI_HomeCheck_BCR_Feeder( M_BCR_FEEDER_4 ); break;

	case EBFI_REJECT_PICK_READY_UP:
		ReqRejVacDown( BACKWARD );
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
		mn_InitStep = EBFI_REJECT_PICK_READY_UP_CHK;			
		m_dwTimeInit = GetCurrentTime();
		break;

	case EBFI_REJECT_PICK_READY_UP_CHK:
		if( m_fbRejVacDown != FB_NONE ) break;
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN]) break;
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
			mn_Mot_Rej[0] = mn_Mot_Rej[1] = CTL_NO;
			mn_InitStep = EBFI_HOMECHK_REJECT_X;
		}
		else
		{
			CString strError;
			strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence( 914, dWARNING, strError );
		}
		break;

	case EBFI_HOMECHK_REJECT_X: 
		EBFI_HomeCheck_Reject_X( M_L_REJECT_X ); 
		mn_init_rej = 0;
		mn_init_pos = 0;
		break;

	case EBFI_FEEDER1_REJECT:
		{
			if( EBRI_Bcr_For_Reject(mn_init_pos) == CTLBD_RET_GOOD)
			{
				mn_init_pos++;
				mn_init_rej = 0;
				if(mn_init_pos >= 4) mn_InitStep = EBFI_FEEDER_READY;
			}
		}
		break;

	case EBFI_FEEDER_READY:
		{
			int nRet1 = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F1], st_basic.nManualSpeed);
			if( nRet1 == BD_GOOD )
			{
				mn_InitStep = EBFI_FEEDER_VACUUM_ON;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//160006 1 00 "Distribute Label Reject X Axis Motor Move Error."
				CString strError;
				strError.Format("%02d0006",M_L_REJECT_X);
				CTL_Lib.Alarm_Error_Occurrence(9642, dWARNING, strError );
			}
		}
		break;

	case EBFI_FEEDER_VACUUM_ON: EBFI_Barcode_Plater_Vacuum_OnOff( IO_ON ); 
		m_dwTimeInit = GetCurrentTime();
		break;

	case EBFI_BARCODE_FEEDER_EMPTY_CHK: 
		if( GetCurrentTime() - m_dwTimeInit <= 0 ) m_dwTimeInit = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeInit < ( st_time.n_wait_time[E_WAIT_BCR_PLATE_VACUUM][IO_ON] * 3) ) break;
		EBFI_Barcode_Plater_Vacuum_OnOff_Check(); 
		break;

	case EBFI_FEEDER_VACUUM_OFF: 
		EBFI_Barcode_Plater_Vacuum_OnOff( IO_OFF ); 
		break;

	case EBFI_FEEDER_EJECT_ON: 
		EBFI_Feeder_EjectOnOff( IO_ON ); 
		m_dwTimeInit = GetCurrentTime();
		break;

	case EBFI_FEEDER_EJECT_OFF: 
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_PLATE_EJECT][IO_ON]) break;
		EBFI_Feeder_EjectOnOff( IO_OFF ); break;

	////2014,1220
	////

	case EBFI_FINISH:	EBFI_Finish(); break;
	}

}

void CRun_Barcode_Feeders::EBFI_Check_Rbt_Chk()
{
	mn_InitStep = EBFI_HOMECHK_VISION_X;
}

void CRun_Barcode_Feeders::EBFI_HomeCheck_Vision_X()
{
	int nRet = COMI.HomeCheck_Mot( M_BCR_VISION_X, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EBFI_MOVE_SAFETY_X;
		Func.OnInitBtn( RBTN_BCR_VISION_X );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//150001 0 00 "Distribute Vision X Axis Motor Home Check Error."
		CString strJamCode = "150001";
		CTL_Lib.Alarm_Error_Occurrence(100, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Feeders::EBFI_MoveSafety_X()
{
	int nRet = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_D_VISION_X].d_pos[P_BCR_FEEDER_SAFETY], st_basic.nManualSpeed);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EBFI_FEEDER_UNLOCK;
//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//150006 1 00 "Distribute Vision X Axis Motor Move Error."
		CString strJamCode = "150006";
		CTL_Lib.Alarm_Error_Occurrence(101, CTL_dWARNING, strJamCode );
	}
}


int CRun_Barcode_Feeders::EBRI_Bcr_For_Reject( int nFeeder )
{
	int nFuncRet = CTLBD_RET_PROCEED;

	int nMotorNum = 0;
	if(nFeeder == 0) nMotorNum = M_BCR_FEEDER_1;
	else if(nFeeder == 1) nMotorNum = M_BCR_FEEDER_2;
	else if(nFeeder == 2) nMotorNum = M_BCR_FEEDER_3;
	else if(nFeeder == 3) nMotorNum = M_BCR_FEEDER_4;

	switch( mn_init_rej )
	{
	case 0:
		mn_Mot_Rej[0] = mn_Mot_Rej[1] = CTL_NO;
		mn_init_rej = 100;
		break;

	case 100:
		ReqRejVacDown( BACKWARD );
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
		m_dwTimeInit = GetCurrentTime();
		mn_init_rej = 200;
		break;

	case 200:
		if( m_fbRejVacDown != FB_NONE) break;

		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < ( st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_OFF] * 3 )) break;
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
			mn_Mot_Rej[0] = mn_Mot_Rej[1] = CTL_NO;
			mn_init_rej = 1000;
		}
		else
		{
			CString strError;
			strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence( 915, dWARNING, strError );
			mn_init_rej = 0;
		}
		break;

	case 1000:
		{
			int nRet1 = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F1 + nFeeder], st_basic.nManualSpeed);
			int nRet2 = BD_ERROR;
			double dPos = 0;

			nRet2 = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_WASTE], st_basic.nManualSpeed);


			if( nRet1 == BD_GOOD ) mn_Mot_Rej[0] = CTL_YES;
			if( nRet2 == BD_GOOD ) mn_Mot_Rej[1] = CTL_YES;
			
			if( mn_Mot_Rej[0] == CTL_YES && mn_Mot_Rej[1] == CTL_YES)
			{
				mn_init_rej = 1100;
				mn_Mot_Rej[0] = mn_Mot_Rej[1] = CTL_NO;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{//160006 1 00 "Distribute Label Reject X Axis Motor Move Error."
				if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence(9141, CTL_dWARNING, "160006" );
				else
				{
					CString strError;
					strError.Format("%02d0006",nMotorNum);
					CTL_Lib.Alarm_Error_Occurrence(9142, dWARNING, strError );
				}
			}
		}
		break;

	case 1100:
		ReqPlateUpDn( nFeeder, FORWARD );
		ReqRejVacDown( FORWARD );
		g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[nFeeder], IO_OFF);
		g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nFeeder], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_ON);
		m_dwTimeInit = GetCurrentTime();
		mn_init_rej = 1200;
		break;

	case 1200:
		if( m_fbplateupdn[nFeeder] != FB_NONE) break;
		if( m_fbRejVacDown != FB_NONE) break;

		mn_init_rej = 2000;
		m_dwTimeInit = GetCurrentTime();

	case 2000:
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < (st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_ON]*2)) break;
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_ON) == IO_ON )
		{
			mn_init_rej = 2300;
		}
		else
		{
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_eject, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[nFeeder], IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nFeeder], IO_ON);
			mn_init_rej = 3100;
		}
		break;

	case 2300:
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
		mn_init_rej = 2400;			
		m_dwTimeInit = GetCurrentTime();
		break;

	case 2400:
		ReqRejVacDown(BACKWARD);
		mn_init_rej = 2500;
		break;

	case 2500:
		if( m_fbRejVacDown != FB_NONE ) break;
		mn_init_rej = 2600;
		break;

	case 2600:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
			if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_ON) == IO_ON )
			{
				mn_init_rej = 2700;
			}
			else
			{
				CString strJamCode;
				strJamCode.Format("5%04d%d",st_io.i_reject_rbt_vacuum_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence( 618, dWARNING, (LPCSTR)strJamCode );
			}
		}
		else
		{
			CString strJamCode;
			strJamCode.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_OFF);
			CTL_Lib.Alarm_Error_Occurrence( 618, dWARNING, (LPCSTR)strJamCode );
		}
		break;

	case 2700:
		{	
			int nRet = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_BARCODE_UNLOAD], st_basic.nManualSpeed);
			if( nRet == BD_GOOD )
			{
				mn_init_rej = 2800;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_L_REJECT_X);
				CTL_Lib.Alarm_Error_Occurrence(9142, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case 2800:
		ReqRejVacDown(FORWARD);
		mn_init_rej = 2900;
		break;

	case 2900:
		if( m_fbRejVacDown != FB_NONE ) break;
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_eject, IO_ON);
		m_dwTimeInit = GetCurrentTime();
		mn_init_rej = 3000;
		break;

	case 3000:
		if( GetCurrentTime() - mn_InitStep < 0 ) mn_InitStep = GetCurrentTime();
		if( GetCurrentTime() - mn_InitStep < (st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_OFF] * 3)) break;
		mn_init_rej = 3100;
		break;

	case 3100:
		ReqRejVacDown(BACKWARD);
		mn_init_rej = 3200;
		break;
		
	case 3200:
		if( m_fbRejVacDown != FB_NONE ) break;
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_eject, IO_OFF);
		g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nFeeder], IO_OFF);

		m_dwTimeInit = GetCurrentTime();
		mn_init_rej = 3300;
		break;

	case 3300:
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeInit < (st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_ON]*3)) break;
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
			m_dwTimeInit = GetCurrentTime();
			mn_init_rej = 3400;
		}
		else
		{
			CString strError;
			strError.Format("5%04d%d", st_io.i_reject_rbt_up_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence( 934, dWARNING, (LPCTSTR)strError );
		}
		break;

	case 3400:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_OFF) == IO_OFF )
		{
			mn_init_rej = 4000;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeInit< st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_OFF]) break;
			CString strError;
			strError.Format("5%04d%d", st_io.i_reject_rbt_vacuum_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence( 935, dWARNING, (LPCTSTR)strError );
		}
		break;

	case 4000:
		if( g_ioMgr.get_in_bit( st_io.i_reject_cv_full_chk, IO_OFF) == IO_ON && 
			g_ioMgr.get_in_bit( st_io.i_reject_cv_full_pos_chk, IO_OFF) == IO_ON )
		{//522130 1 00 "Reject Label Conveyor Full Position Check Error."
			CTL_Lib.Alarm_Error_Occurrence(144, CTL_dWARNING, "522130");
		}
		else if( g_ioMgr.get_in_bit( st_io.i_reject_cv_full_chk, IO_OFF) == IO_ON)
		{
			m_nRejectConvey = CTL_YES;
			mn_init_rej = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else
		{
			mn_init_rej = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		break;
	}

	return nFuncRet;
}
		
void CRun_Barcode_Feeders::EBFI_BcrFeeder_Lock( EFB efb )
{
	for ( int i = 0; i < 4; i++ )
	{
		ReqBcrLock( i, efb);
	} //2014.1018 BACKWARD -> FORWARD
	if ( efb == FORWARD) mn_InitStep = EBFI_FEEDER_UNLOCK_CHK;
	else					 mn_InitStep = EBFI_FEEDER_LOCK_CHK;
}

void CRun_Barcode_Feeders::ReqBcrLock( int nSite, EFB efb )
{
	m_fbBcrlock[nSite] = efb;
	m_step_bcrlock[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Feeders::EBFI_BcrFeeder_Lock_chk( EFB efb )
{
	if( GetBcrLock(EBP1) != FB_NONE ) return;
	if( GetBcrLock(EBP2) != FB_NONE ) return;
	if( GetBcrLock(EBP3) != FB_NONE ) return;
	if( GetBcrLock(EBP4) != FB_NONE ) return;

	if( efb == BACKWARD ) mn_InitStep = EBFI_FEEDER_LOCK;
	else				   mn_InitStep = EBFI_PLATE_DOWN;
}

void CRun_Barcode_Feeders::Run_Move_Feeder_Lock( int nSite )
{
	if(m_fbBcrlock[nSite] == FB_NONE) return;

	int iOnOff = IO_ON;
	if( m_fbBcrlock[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_bcrlock[nSite]) {
	case ESTEP_CYL_START: m_step_bcrlock[nSite] = ESTEP_CYL_MOVE; break;
	
	case ESTEP_CYL_MOVE:
		{
			SetBcrLock( nSite, iOnOff);
			if( COMI.mn_run_status == dRUN ){
				st_work.mn_site_bcr_lock[nSite] = iOnOff;
			}
			m_step_bcrlock[nSite] = ESTEP_CYL_CHK;
			m_dwTimeBcrlockStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckBcrLock( nSite, iOnOff ))
			{
				m_step_bcrlock[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeBcrlock[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeBcrlockStart[nSite] > st_time.n_limit_time[E_WAIT_FEEDER_LOCK])
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 102, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeBcrlockStart[nSite] < 0 )
			{
				m_dwTimeBcrlockStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeBcrlock[nSite] < 0 )
				m_dwTimeBcrlock[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeBcrlock[nSite] < st_time.n_wait_time[E_WAIT_FEEDER_LOCK][iOnOff] )
				return;

			m_step_bcrlock[nSite] = ESTEP_CYL_START;
			m_fbBcrlock[nSite] = FB_NONE;
		}
	}
}

void CRun_Barcode_Feeders::SetBcrLock(int nSite, int iOnOff )
{
	int nLock = st_io.o_distribute_p_lock[nSite];

	g_ioMgr.set_out_bit( nLock, iOnOff);
}

bool CRun_Barcode_Feeders::CheckBcrLock( int nSite, int iOnOff )
{
	int nLock_1 = st_io.i_distribute_lock_chk[nSite] ;
	int nLock_2 = st_io.i_distribute_unlock_chk[nSite];

	// 	if( g_ioMgr.get_in_bit( nLock_1 ) != iOnOff) return false;
	// 	if( g_ioMgr.get_in_bit( nLock_2 ) != !iOnOff) return false;
	if( g_ioMgr.get_in_bit( nLock_1 ) != !iOnOff) return false;//2014.1018 
	if( g_ioMgr.get_in_bit( nLock_2 ) != iOnOff) return false;//2014.1018

	return true;
}

void CRun_Barcode_Feeders::EBFI_Finish()
{
	Func.OnInitBtn( RBTN_BCR_FEEDER );//2014.1018
	g_ioMgr.set_out_bit( st_io.o_distribute_plate_updown[0], IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_distribute_plate_updown[1], IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_distribute_plate_updown[2], IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_distribute_plate_updown[3], IO_OFF );
	st_handler.mn_init_state[INIT_BARCODE_FEEDER] = CTL_YES;
	mn_InitStep = EBFI_START;
}

bool CRun_Barcode_Feeders::SetVariable(int pos, int nPos, CString mode, CString strRetry, CString strCnt)
{
//	if( ( pos != IDPOS_DENSITY && pos != IDPOS_BARCODE1 ) || pos < IDPOS_DENSITY ) return false;
	memset(&st_bcrinfo[pos], 0x00, sizeof(st_bcrinfo_param));
	
//	int iIdx = g_Tray.GetIdxByPos(pos);
//	if(iIdx >= 0 )
//	{
//		ATray& trayPri = g_Tray.GetTray( iIdx );
//		sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cLotid, trayPri.GetModule(nPos).GetLotNo());
//		sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cBoatid, trayPri.GetModule(nPos).GetSN());
//		sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cSerial, trayPri.GetModule(nPos).GetSerial());
//		sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cLabelcode, g_lotMgr.GetLotAt(nPos).GetLabelCode());
//		sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cLabelcode, g_lotMgr.GetLotAt(nPos).GetRibbonCode());
//		if(mode == "NEW")//first NEW ,RETRY
//		{
//			strRetry = "NO"; strCnt = "0";
//			sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cMode, mode);
//			sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cRetry, strRetry);				
//			sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cCnt, strCnt);
//
//		}
//		if( strRetry == "YES" || atoi(strCnt) > 0)
//		{
//			mode = "RETRY";
//			strRetry = "YES";
//			strCnt.Format("%d",atoi(strCnt) + 1);
//			sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cMode, mode);
//			sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cMode, strRetry);				
//			sprintf(st_bcrinfo[pos - IDPOS_DENSITY].m_cMode, strCnt);
//		}
//	}
	
	sprintf(st_bcrinfo[pos].m_cLotid,"%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_lot_id[nPos%2] );
	sprintf(st_bcrinfo[pos].m_cBoatid,"%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuff_num[nPos%2] );
	sprintf(st_bcrinfo[pos].m_cSerial,"%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[nPos%2] );
	sprintf(st_bcrinfo[pos].m_cLabelcode, g_lotMgr.GetLotAt(0).GetLabelCode());
	sprintf(st_bcrinfo[pos].m_cRibboncode, g_lotMgr.GetLotAt(0).GetRibbonCode());
	if(mode == "NEW")//first NEW ,RETRY
	{
		strRetry = "NO"; strCnt = "0";
		sprintf(st_bcrinfo[pos].m_cMode, mode);
		sprintf(st_bcrinfo[pos].m_cRetry, strRetry);				
		sprintf(st_bcrinfo[pos].m_cCnt, strCnt);
		
	}
	if( strRetry == "YES" || atoi(strCnt) > 0)
	{
		mode = "RETRY";
		strRetry = "YES";
		strCnt.Format("%d",atoi(strCnt) + 1);
		sprintf(st_bcrinfo[pos].m_cMode, mode);
		sprintf(st_bcrinfo[pos].m_cMode, strRetry);				
		sprintf(st_bcrinfo[pos].m_cMode, strCnt);
	}
	
	return true;
}


//시리얼범호를 이용해 서버에 보내 바코드 format를 받아온다.
void CRun_Barcode_Feeders::CommBarcode( int nSite, CString strSreial)
{
	m_site_Serial[nSite] = "";
	m_site_Serial[nSite] = strSreial;
	m_cwBcrcomm[nSite] = ECW_WORK;
	m_step_bcrcomm[nSite] = ECOMM_START;
}

void CRun_Barcode_Feeders::Run_Move_Bcrcomm(int nSite)
{
	if( m_cwBcrcomm[nSite] == ECW_NONE) return;
	
	switch(m_step_bcrcomm[nSite])
	{
	case ECOMM_START: m_step_bcrcomm[nSite] = ECOMM_READY;
		if(st_basic.mn_mode_run == false) m_step_bcrcomm[nSite] = ECOMM_FINISH;
		m_step_bcrcomm[nSite] = ECOMM_FINISH;
		break;

	case ECOMM_CONNECT:
		if( st_basic.nVisionUse[1] == false )
		{
			//m_step_bcrcomm[nSite] = ECOMM_MOVE;
			m_step_bcrcomm[nSite] = ECOMM_FINISH;//2014,1219
		}
		else
		{
			m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
			::SendMessage( st_handler.hWnd, WM_CLIENT_MSG + CLS_ECSERVER, CLIENT_CONNECT, CLS_ECSERVER);
			m_step_bcrcomm[nSite] = ECOMM_CONNECT_CHK;
		}		
		break;

	case ECOMM_CONNECT_CHK:
		if( st_client[CLS_ECSERVER].n_connect == CTL_YES )
		{
			m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
			m_step_bcrcomm[nSite] = ECOMM_READY;			
		}
		else
		{
			m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
			m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];

			if ( m_dwTimeBcrFNetwork[nSite][2] <= 0 )
			{
				m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
				break;
			}

			if ( m_dwTimeBcrFNetwork[nSite][2] > 3000 )
			{
				m_nRetryCnt[0][nSite]++;
				if (m_nRetryCnt[0][nSite] >= 3)
				{
					m_nRetryCnt[0][nSite] = 0;

					// 700400 1 00 "Barcode1_time_out_from_Zebra_Printer."
					strJamCode.Format( "7004%d0", nSite);
					CTL_Lib.Alarm_Error_Occurrence(102, CTL_dWARNING, strJamCode );

					clsZebra.OnPrintAnswerMode(2, 0, nSite+CLS_BCR_PRINTER1);
					m_step_bcrcomm[nSite] = ECOMM_CONNECT;
				}
				else
				{
					m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();

					m_step_bcrcomm[nSite] = ECOMM_CONNECT_WAIT;
				}
			}
		}
		break;

	case ECOMM_CONNECT_WAIT:
		m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
		m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];
		
		if (m_dwTimeBcrFNetwork[nSite][2] <= 0)
		{
			m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
			break;
		}
		
		if (m_dwTimeBcrFNetwork[nSite][2] > 2000)
		{
			m_step_bcrcomm[nSite] = ECOMM_CONNECT;
		}
		break;

	case ECOMM_READY:
		m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
		m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];
		
		if (m_dwTimeBcrFNetwork[nSite][2] <= 0)
		{
			m_dwTimeBcrFNetwork[nSite][0] = GetTickCount();
			break;
		}
		
		{
			if( st_basic.n_mode_device == false ||
				(g_lotMgr.GetLotAt(0).GetLabelCode() == st_handler.m_strLabelCode[nSite] && g_lotMgr.GetLotAt(0).GetRibbonCode() == st_handler.m_strRibbonCode[nSite] ))
			{
				 m_step_bcrcomm[nSite] = ECOMM_MOVE;
			}
			else
			{
				if( COMI.mn_run_status != dRUN ) return;
				if( g_lotMgr.GetLotAt(0).GetLabelCode() != st_handler.m_strLabelCode[nSite])
				{//620000 1 00 "Lable code is not same."
					CTL_Lib.Alarm_Error_Occurrence(1034, CTL_dWARNING, "620000" );
				}
				if( g_lotMgr.GetLotAt(0).GetRibbonCode() != st_handler.m_strRibbonCode[nSite])
				{//620001 1 00 "Ribbon code is not same."
					CTL_Lib.Alarm_Error_Occurrence(104, CTL_dWARNING, "620001" );
				}
			}
		}
		break;

	case ECOMM_MOVE:
		{
			bool bNext = false;
			if( st_basic.mn_mode_run == false ) bNext = true;

			if( st_basic.mn_mode_run && st_handler.m_strBcrSerial[nSite] != "")
			{
				Func.m_pBctFmtClient[nSite]->ClrVariable();
				Func.m_pBctFmtClient[nSite]->OnBrandBcrLabelIssue( 10 + nSite, st_bcrinfo[nSite].m_cLotid,  st_bcrinfo[nSite].m_cBoatid, st_bcrinfo[nSite].m_cLabelcode, 
					st_bcrinfo[nSite].m_cSerial, st_bcrinfo[nSite].m_cRibboncode, st_bcrinfo[nSite].m_cMode, st_bcrinfo[nSite].m_cRetry, st_bcrinfo[nSite].m_cCnt);

				bNext = true;
			}

			if( bNext == true)
			{
				m_step_bcrcomm[nSite] = ECOMM_RECV;
				m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
			}
		}
		break;

	case ECOMM_RECV:
		m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
		m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];
		
		if (m_dwTimeBcrFNetwork[nSite][2] <= 0)
		{
			m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
		}
		else
		{
			if( st_basic.mn_mode_run == 0 || Func.m_pBctFmtClient[nSite]->GetSerialInReceived())
			{
				CString strlot(st_bcrinfo[nSite].m_cLotid);
				CString strSerial(st_bcrinfo[nSite].m_cSerial);				

				if( Func.CmpStrFile(Func.m_pBctFmtClient[nSite]->GetLotID(), strlot) || 
					Func.CmpStrFile(Func.m_pBctFmtClient[nSite]->GetLabelSerial(), strSerial))
				{
					//700100 1 A "Received incorrect Barcode1 from server."
					CString strError;
					strError.Format("7001%d0", nSite);
					CTL_Lib.Alarm_Error_Occurrence(105, CTL_dWARNING, strError );
					m_step_bcrcomm[nSite] = ECOMM_RECV_RETRY;
					break;
				}
				m_step_bcrcomm[nSite] = ECOMM_FINISH;
			}
			else
			{
				if( Func.m_pBctFmtClient[nSite]->GetSerialInReceived() == NR_WAITING )
				{
					//700100 1 A "Received incorrect Barcode1 from server."
					CString strError;
					strError.Format("7001%d0", nSite);
					CTL_Lib.Alarm_Error_Occurrence(106, CTL_dWARNING, strError );
					m_step_bcrcomm[nSite] = ECOMM_RECV_RETRY;
				}

				if (m_dwTimeBcrFNetwork[nSite][2] > 60000)
				{
					//700400 1 00 "Barcode1_time_out_from_Zebra_Printer."
					CString strError;
					strError.Format("7004%d0", nSite);
					CTL_Lib.Alarm_Error_Occurrence(107, CTL_dWARNING, strError );
					m_step_bcrcomm[nSite] = ECOMM_RECV_RETRY;
				}
			}
		}
		break;

	case ECOMM_RECV_RETRY:
		sprintf(st_bcrinfo[nSite].m_cMode, "RETRY");
		sprintf(st_bcrinfo[nSite].m_cRetry, "YES");
		sprintf(st_bcrinfo[nSite].m_cCnt,"%d", atoi(st_bcrinfo[nSite].m_cCnt) + 1 );
		m_step_bcrcomm[nSite] = ECOMM_START;
		break;


	case ECOMM_FINISH:
		//해당 Feeder에 바코드출력을 한다.
		if(st_basic.nVisionUse[1] == false )
			Func.m_pBctFmtClient[nSite]->SetBcrFormat( Func.GetPrintFormat() );//ybs 2014,1104		
		
//		ReqBcrOutPrint(nSite, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
		m_step_bcrcomm[nSite] = ECOMM_NONE;
		m_cwBcrcomm[nSite] = ECW_NONE;
		break;
	}
}


void CRun_Barcode_Feeders::ReqBcrOutPrint( int nSite, CString strfmtPrint)
{
	m_site_Fromat[nSite] = "";
	m_site_Fromat[nSite] = strfmtPrint;
	m_spoBcrout[nSite] = EBPO_WORK;
	m_step_bcrout[nSite] = EBOS_START;
}

//해당 Feeder에 바코드출력을 시작한다.
void CRun_Barcode_Feeders::Run_Move_Bcr_out( int nSite )
{
	CString strJamCode;
	if( m_spoBcrout[nSite] == EBOS_NONE) return;
	
	int nMotorNum = -1;
	if(nSite == 0) nMotorNum = M_BCR_FEEDER_1;
	else if(nSite == 1) nMotorNum = M_BCR_FEEDER_2;
	else if(nSite == 2) nMotorNum = M_BCR_FEEDER_3;
	else if(nSite == 3) nMotorNum = M_BCR_FEEDER_4;

	switch(	m_step_bcrout[nSite])
	{

	case EBOS_START:

		if( COMI.Get_MotCurrentPos( nMotorNum) > st_motor[nMotorNum].d_pos[P_BCR_FEEDER_PICK] - COMI.md_allow_value[nMotorNum])
		{
			m_step_bcrout[nSite] = EBOS_MOVE;
			if(st_basic.n_mode_device) m_step_bcrout[nSite] = EBOS_CONNECT;
			else
			{
				m_step_bcrout[nSite] = EBOS_PRINT;
				m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			}
			m_nRetryCnt[1][nSite] = 0;
		}

		break;

	case EBOS_CONNECT:
		if (st_client[nSite + CLS_BCR_PRINTER1].n_connect == CTL_YES)
		{
			m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			m_step_bcrout[nSite] = EBOS_PRINT;
		}
		else 
		{
			m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			::PostMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CONNECT, nSite + CLS_BCR_PRINTER1);
			
			m_step_bcrout[nSite] = EBOS_CONNECT_CHK;
		}
		break;

	case EBOS_CONNECT_CHK:
		if (st_client[nSite + CLS_BCR_PRINTER1].n_connect == CTL_YES)
		{
			m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			m_step_bcrout[nSite] = EBOS_PRINT;
		}
		else 
		{
			m_dwBcrOutTimeWait[nSite][1] = GetTickCount();
			m_dwBcrOutTimeWait[nSite][2] = m_dwBcrOutTimeWait[nSite][1] - m_dwBcrOutTimeWait[nSite][0];

			if (m_dwBcrOutTimeWait[nSite][2] <= 0)
			{
				m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
				break;
			}

			if (m_dwBcrOutTimeWait[nSite][2] >= st_basic.mn_network_wait_time)
			{
				m_nRetryCnt[1][nSite]++;

				if (m_nRetryCnt[1][nSite] >= st_basic.mn_network_retry_cnt)
				{

					//2015.0327
					// 700200 1 00 "Barcode1_time_out_from_server."
// 					strJamCode.Format( "7002%d0", nSite);
// // 					strJamCode.Format( "%02d0006", M_BCR_FEEDER_1+nSite);
// 					CTL_Lib.Alarm_Error_Occurrence(121, CTL_dWARNING, strJamCode );

					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					m_step_bcrout[nSite] = EBOS_CONNECT_WAIT;

					//2015.0327
					st_var.n_bcrid_error[nSite] = CTL_YES;
					m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
					m_step_bcrout[nSite] = EBOS_PRINT_COM_WAIT;
				}
				else
				{
					m_dwBcrOutTimeWait[nSite][0] = GetTickCount();

					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					m_step_bcrout[nSite] = EBOS_CONNECT_WAIT;
					//ReqBcrStatusPrint(nSite);
				}
			}
		}
		break;

	case EBOS_CONNECT_WAIT:
		m_dwBcrOutTimeWait[nSite][1] = GetTickCount();
		m_dwBcrOutTimeWait[nSite][2] = m_dwBcrOutTimeWait[nSite][1] - m_dwBcrOutTimeWait[nSite][0];
		
		if (m_dwBcrOutTimeWait[nSite][2] <= 0)
		{
			m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwBcrOutTimeWait[nSite][2] >= 5000)
		{
			m_step_bcrout[nSite] = EBOS_CONNECT;
		}
		break;

	case EBOS_PRINT:
		m_dwBcrOutTimeWait[nSite][1] = GetTickCount();
		m_dwBcrOutTimeWait[nSite][2] = m_dwBcrOutTimeWait[nSite][1] - m_dwBcrOutTimeWait[nSite][0];
		
		if (m_dwBcrOutTimeWait[nSite][2] <= 0)
		{
			m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			break;
		}

//		if( m_dwBcrOutTimeWait[nSite][2] < 2000) break;

		if(st_basic.n_mode_device == 0 ) {
			m_step_bcrout[nSite] = EBOS_MOVE;
			break;
		}

		m_dwBcrOutTimeWait[nSite][0] = GetTickCount();

		Func.m_pZebra[nSite]->m_nPrintStatusBin = NVR_NONE;
		//	(통신방법[0]시리얼[1]LPT[2]네트워크, 네트워크, 라벨 X offset, 라벨 Y offset, 라벨 포맷); 
		
		if(st_handler.n_only_work_bcr1 == CTL_YES)
		{
			Func.m_pZebra[nSite]->OnPrintOutput(2, nSite + CLS_BCR_PRINTER1, 0, 0,  Func.GetPrintFormat());
		}
		else if( st_basic.mn_mode_run == 1 && m_site_Fromat[nSite] != "")
		{
			Func.m_pZebra[nSite]->OnPrintOutput(2, nSite + CLS_BCR_PRINTER1, 0, 0,  m_site_Fromat[nSite]);//Func.m_pBctFmtClient[nSite]->GetBcrFormat());//);m_site_Fromat[nSite] );//Func.GetPrintFormat());//m_site_Fromat[nSite]);
		}
		else if( st_basic.mn_mode_run == 0 )
		{
			Func.m_pZebra[nSite]->OnPrintOutput(2, nSite + CLS_BCR_PRINTER1, 0, 0,  Func.GetPrintFormat());//m_site_Fromat[nSite]);
		}

		m_step_bcrout[nSite] = EBOS_PRINT_COM;
		break;

	case EBOS_PRINT_COM:
		if (Func.m_pZebra[nSite]->m_nPrintStatusBin == NVR_PASS)
		{
			// label이 정상적으로 출력 되었으면 
			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);

			m_step_bcrout[nSite] = EBOS_MOVE;
		}
		else if (Func.m_pZebra[nSite]->m_nPrintStatusBin == NVR_FAIL)
		{
			m_nBcrFailRetryCnt[nSite]++;
			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
			
			if (m_nBcrFailRetryCnt[nSite] >= 3)
			{
				m_nBcrFailRetryCnt[nSite] = 0;
				
				// 700400 1 00 "Barcode1_time_out_from_Zebra_Printer."
				strJamCode.Format( "7004%d0", nSite);
// 				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1+nSite);
				CTL_Lib.Alarm_Error_Occurrence(122, CTL_dWARNING, strJamCode );

				clsZebra.OnPrintAnswerMode(2, 0, nSite+CLS_BCR_PRINTER1);
				
				//::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);				
				//m_step_bcrout[nSite] = EBOS_CONNECT;
			}
			else
			{
				//m_dwBcrOutTimeWait[nSite][0] = GetTickCount();				
				//::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);				
				//m_step_bcrout[nSite] = EBOS_PRINT_COM_WAIT;
			}
			//////////////////////////////////////////////////////////////////////////	
			//ybs2014,1104
//			Func.m_pBctFmtClient[nSite+2]->SetBcrFormat( Func.m_pBctFmtClient[nSite]->GetBcrFormat() );
//			ReqBcrOutPrint(nSite+2, Func.m_pBctFmtClient[nSite+2]->GetBcrFormat());//두번째
//			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + 2);
			m_step_bcrout[nSite] = EBOS_MOVE;
			
		}
		else 
		{
			m_dwBcrOutTimeWait[nSite][1] = GetTickCount();
			m_dwBcrOutTimeWait[nSite][2] = m_dwBcrOutTimeWait[nSite][1] - m_dwBcrOutTimeWait[nSite][0];
			
			if (m_dwBcrOutTimeWait[nSite][2] <= 0)
			{
				m_dwBcrOutTimeWait[nSite][0] = GetTickCount();

				break;
			}
			
			if (m_dwBcrOutTimeWait[nSite][2] > st_basic.mn_network_wait_time)
			{
				m_nRetryCnt[1][nSite]++;
				
				if (m_nRetryCnt[1][nSite] >= st_basic.mn_network_retry_cnt )
				{
// 					700100 1 00 "Barcode1_is not response.Ribbon or Barcode Empty"
// 					700110 1 00 "Barcode2_is not response.Ribbon or Barcode Empty"
// 					700120 1 00 "Barcode3_is not response.Ribbon or Barcode Empty"
// 					700130 1 00 "Barcode4_is not response.Ribbon or Barcode Empty"
// 					strJamCode.Format( "7001%d0", nSite);
// 					CTL_Lib.Alarm_Error_Occurrence(123, CTL_dWARNING, strJamCode );
					m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
					st_var.n_bcrid_error[nSite] = CTL_YES;//2015.0327					
					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					m_step_bcrout[nSite] = EBOS_PRINT_COM_WAIT;
				}
				else
				{
					m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					m_step_bcrout[nSite] = EBOS_PRINT_COM_WAIT;
					//ReqBcrStatusPrint(nSite);
				}
			}
		}
		break;

	case EBOS_PRINT_COM_WAIT:
		m_dwBcrOutTimeWait[nSite][1] = GetTickCount();
		m_dwBcrOutTimeWait[nSite][2] = m_dwBcrOutTimeWait[nSite][1] - m_dwBcrOutTimeWait[nSite][0];
		
		if (m_dwBcrOutTimeWait[nSite][2] <= 0)
		{
			m_dwBcrOutTimeWait[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwBcrOutTimeWait[nSite][2] >= 1000)
		{
			//2015.0120
			m_step_bcrout[nSite] = ENUM_BCROUT_STEP(71);
			g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_OFF);
			ReqPlateUpDn( nSite, FORWARD );
		}

		//2015.0301
// 		if(st_basic.nBcrFeederUse[nSite] != CTL_YES)
// 		{
// 			m_step_bcrout[nSite] = EBOS_MOVE;
// 		}
		break;

	case 71:
		if( m_fbplateupdn[nSite] != FB_NONE) break;
		if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[nSite], IO_ON) == IO_ON)
		{
			st_var.n_bcrid_error[nSite] = CTL_NO;
			m_step_bcrout[nSite] = EBOS_MOVE;
		}
		else
		{
			m_nRetryCnt[1][nSite]++;
			
			if (m_nRetryCnt[1][nSite] > st_basic.mn_network_retry_cnt)
			{
				m_nRetryCnt[1][nSite] = 0;
				m_step_bcrout[nSite] = EBOS_MOVE;
			}
			else
			{
				ReqPlateUpDn( nSite, BACKWARD );
				m_step_bcrout[nSite] = ENUM_BCROUT_STEP(72);
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_ON);
			}
		}
		break;

	case 72:
		if( m_fbplateupdn[nSite] != FB_NONE) break;
		m_step_bcrout[nSite] = EBOS_CONNECT;
		break;

	case EBOS_MOVE:
		//print 출력
		mb_BcrComplete[nSite] = true;//Barcode out end
		m_step_bcrout[nSite] = EBOS_FINISH;
		break;

	case EBOS_FINISH:
		// jtkim print 상태체크 변수 초기화 
 		//ReqBcrStatusPrint(nSite);

		//2015.0316
		st_handler.m_bBcrTimeOutError[nSite] = false;
		if( GetCurrentTime() - st_handler.m_dwBcrOutTime[nSite] <= 0 )  st_handler.m_dwBcrOutTime[nSite] = GetCurrentTime();
		if( GetCurrentTime() - st_handler.m_dwBcrOutTime[nSite] > (st_basic.mn_limit_bcr_time * 60 * 1000) )
		{
			st_handler.m_bBcrTimeOutError[nSite] = true;
		}
		//again counting
		st_handler.m_dwBcrOutTime[nSite] = GetCurrentTime();

		m_step_bcrout[nSite] = EBOS_NONE;
		m_spoBcrout[nSite] = EBPO_NONE;
		break;
	
	}
}

// jtkim print 상태체크하기 위한 변수 초기
void CRun_Barcode_Feeders::ReqBcrStatusPrint( int nSite)
{
	m_spoBcrStatus[nSite]	= EBPO_WORK;
	m_stepBcrStstus[nSite]	= EBOS_START;
}

//해당 Feeder에 바코드 상태를 점검 
void CRun_Barcode_Feeders::Run_Move_Print_Status( int nSite )
{
	CString strJamCode;
	if( m_spoBcrStatus[nSite] == EBPO_NONE) return;
	
	int nMotorNum = -1;
	if(nSite == 0) nMotorNum = M_BCR_FEEDER_1;
	else if(nSite == 1) nMotorNum = M_BCR_FEEDER_2;
	else if(nSite == 2) nMotorNum = M_BCR_FEEDER_3;
	else if(nSite == 3) nMotorNum = M_BCR_FEEDER_4;

	switch(	m_stepBcrStstus[nSite])
	{
	case EBOS_START:
		m_stepBcrStstus[nSite] = EBOS_MOVE;
		if(st_basic.n_mode_device) m_stepBcrStstus[nSite] = EBOS_CONNECT;
		break;

	case EBOS_CONNECT:

		if( st_basic.mn_mode_run == false)
		{
			m_stepBcrStstus[nSite] = EBOS_MOVE;
			break;
		}

		if (st_client[nSite + CLS_BCR_PRINTER1].n_connect == CTL_YES)
		{
			m_stepBcrStstus[nSite] = EBOS_PRINT;
		}
		else 
		{
			m_dwTimeWait[nSite][0] = GetTickCount();
			////2014.1117 ysj
//			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CONNECT, nSite + CLS_BCR_PRINTER1);
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nSite, CLIENT_CONNECT, nSite + CLS_BCR_PRINTER1);
			////
			m_stepBcrStstus[nSite] = EBOS_CONNECT_CHK;
		}
		break;

	case EBOS_CONNECT_CHK:
		if (st_client[nSite + CLS_BCR_PRINTER1].n_connect == CTL_YES)
		{
			m_stepBcrStstus[nSite] = EBOS_PRINT;
		}
		else 
		{
			m_dwTimeWait[nSite][1] = GetTickCount();
			m_dwTimeWait[nSite][2] = m_dwTimeWait[nSite][1] - m_dwTimeWait[nSite][0];

			if (m_dwTimeWait[nSite][2] <= 0)
			{
				m_dwTimeWait[nSite][0] = GetTickCount();
				break;
			}

			if (m_dwTimeWait[nSite][2] >= 3000)
			{
				m_nRetryCnt[2][nSite]++;

				if (m_nRetryCnt[2][nSite] >= 3)
				{
					m_nRetryCnt[2][nSite] = 0;

					strJamCode.Format( "%02d0006", nMotorNum);
					CTL_Lib.Alarm_Error_Occurrence(1210, CTL_dWARNING, strJamCode );

					m_stepBcrStstus[nSite] = EBOS_CONNECT;
				}
				else
				{
					m_dwTimeWait[nSite][0] = GetTickCount();

					m_stepBcrStstus[nSite] = EBOS_CONNECT_WAIT;
				}
			}
		}
		break;

	case EBOS_CONNECT_WAIT:
		m_dwTimeWait[nSite][1] = GetTickCount();
		m_dwTimeWait[nSite][2] = m_dwTimeWait[nSite][1] - m_dwTimeWait[nSite][0];
		
		if (m_dwTimeWait[nSite][2] <= 0)
		{
			m_dwTimeWait[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeWait[nSite][2] >= 5000)
		{
			m_stepBcrStstus[nSite] = EBOS_CONNECT;
		}
		break;

	case EBOS_PRINT://바코드 상태 체크 확인
		m_dwTimeWait[nSite][0] = GetTickCount();

		Func.m_pZebra[nSite]->m_nPrintStatusBin = NVR_NONE;
		//	(통신방법[0]시리얼[1]LPT[2]네트워크, 네트워크, 라벨 X offset, 라벨 Y offset, 라벨 포맷); 
//		Func.m_pZebra[nSite]->OnPrintOutput(2, nSite + CLS_BCR_PRINTER1, 0, 0,  m_site_Fromat[nSite]);
		Func.m_pZebra[nSite]->OnPrintStatusCheck(2, nSite + CLS_BCR_PRINTER1);//바코드 상태 체크 확인

		m_stepBcrStstus[nSite] = EBOS_PRINT_COM;
		break;

	case EBOS_PRINT_COM:
		if (Func.m_pZebra[nSite]->m_nPrintStatusBin == NVR_PASS)
		{
			// label이 정상적으로 출력 되었으면 
			////2014.1117 ysj
//			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
			////

			if (Func.m_pZebra[nSite]->m_nPaperOutFlag	== 1)
			{
				// 제브라 프린터에 용지가 없으면 
				strJamCode.Format( "6400%02d", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(1202, CTL_dWARNING, strJamCode );
				m_stepBcrStstus[nSite] = EBOS_CONNECT;
				
			}
			else if (Func.m_pZebra[nSite]->m_nHeadUpFlag == 1)
			{
				// 제브라 프린터에 헤드가 열려 있으면 
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(1221, CTL_dWARNING, strJamCode );
				m_stepBcrStstus[nSite] = EBOS_CONNECT;
				
			}
			else if (Func.m_pZebra[nSite]->m_nRibbonOutFlag	== 1)
			{
				// 제브라 프린터에 리본이 없으면 
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(1222, CTL_dWARNING, strJamCode );

				m_stepBcrStstus[nSite] = EBOS_CONNECT;
			}
			else
			{
				m_stepBcrStstus[nSite] = EBOS_MOVE;
			}
		}
		else if (Func.m_pZebra[nSite]->m_nPrintStatusBin == NVR_FAIL)
		{
			m_nBcrFailRetryCnt[nSite]++;
			
			if (m_nBcrFailRetryCnt[nSite] >= 1)
			{
				m_nBcrFailRetryCnt[nSite] = 0;
				
				// jtkim 알람코드 연결 timeout error
// 				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1+nSite);//2014,1219
// 				CTL_Lib.Alarm_Error_Occurrence(1223, CTL_dWARNING, strJamCode );//2014,1219
				m_stepBcrStstus[nSite] = EBOS_MOVE;//2014,1219 test 
				break;
			}

			m_dwTimeWait[nSite][0] = GetTickCount();
			////2014.1117 ysj
//			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
			////
			m_stepBcrStstus[nSite] = EBOS_PRINT_COM_WAIT;
			
		}
		else 
		{
			m_dwTimeWait[nSite][1] = GetTickCount();
			m_dwTimeWait[nSite][2] = m_dwTimeWait[nSite][1] - m_dwTimeWait[nSite][0];
			
			if (m_dwTimeWait[nSite][2] <= 0)
			{
				m_dwTimeWait[nSite][0] = GetTickCount();

				break;
			}
			
			if (m_dwTimeWait[nSite][2] >= 3000)
			{
				m_nRetryCnt[2][nSite]++;
				
				if (m_nRetryCnt[2][nSite] >= 3)
				{
					m_nRetryCnt[2][nSite] = 0;
					
					strJamCode.Format( "%02d0006", nMotorNum);

					CTL_Lib.Alarm_Error_Occurrence(124, CTL_dWARNING, strJamCode );
					////2014.1117 ysj
//					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					////
					m_stepBcrStstus[nSite] = EBOS_CONNECT;

				}
				else
				{
					m_dwTimeWait[nSite][0] = GetTickCount();
					//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + nSite, CLIENT_CLOSE, nSite + CLS_BCR_PRINTER1);
					//ReqBcrStatusPrint(nSite);
					m_stepBcrStstus[nSite] = EBOS_PRINT_COM_WAIT;
				}
			}
		}
		break;

	case EBOS_PRINT_COM_WAIT:
		m_dwTimeWait[nSite][1] = GetTickCount();
		m_dwTimeWait[nSite][2] = m_dwTimeWait[nSite][1] - m_dwTimeWait[nSite][0];
		
		if (m_dwTimeWait[nSite][2] <= 0)
		{
			m_dwTimeWait[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeWait[nSite][2] >= 5000)
		{
			m_stepBcrStstus[nSite] = EBOS_CONNECT;
		}
		break;
	
	case EBOS_MOVE:
		m_stepBcrStstus[nSite] = EBOS_FINISH;
		break;

	case EBOS_FINISH:
		m_nRetryCnt[2][nSite] = 0;
		m_stepBcrStstus[nSite] = EBOS_NONE;
		m_spoBcrStatus[nSite] = EBPO_NONE;
		break;
	
	}
}



bool CRun_Barcode_Feeders::Run_Move1_Chk_Time()
{
	switch( rej_picker_step_old )
	{
	case PSTEP_REJECT_PICK_UP_CHK:
 		if( InTimeRobot( st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN] ) == false )
 		{//522080 1 00 "Reject Label Robot Up Check Off Error."
			CTL_Lib.Alarm_Error_Occurrence( 110, dWARNING, "522080" );
			return false;
 		}
		break;
	case PSTEP_REJECT_PICK_START:		
		if( InTimeRobot( 10000 ) == false )
 		{//160006 1 00 "Distribute Label Reject X Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 111, dWARNING, "160006" );
			return false;
 		}
		break;

	case PSTEP_REJECT_PICK_DOWN_CHK:
		if( InTimeRobot( 10000 ) == false )
 		{//522090 1 00 "Reject Label Robot Down Check Off Error."
			CTL_Lib.Alarm_Error_Occurrence( 112, dWARNING, "522090" );
			return false;
 		}
		break;
	}
	return true;
}

void CRun_Barcode_Feeders::Run_Move2( int nSite)
{
	Func.ThreadFunctionStepTrace(14, m_nRun_Print[nSite+2]);
	
	int nIn[4];
	bool bPickInfo[2];
	bPickInfo[0] = bPickInfo[1] = false;

	switch(m_nRun_Print[nSite+2])
	{
	case 0: 
		if( g_lotMgr.GetLotCount() < 1/* || (g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES")*/ ) break;
		m_nRun_Print[nSite+2] = 100;
		break;

	case 100:
		if( st_basic.nBcrFeederUse[nSite+2] == CTL_YES && st_var.n_use_barcode[nSite+2] != EMBS_SKIP && st_var.n_using_barcode == 1)  
		{
			nIn[0] = FAS_IO.get_in_bit(st_io.i_distribute_rbt2_left_top_vacuum_chk, IO_OFF);
			nIn[1] = FAS_IO.get_in_bit(st_io.i_distribute_rbt2_left_btm_vacuum_chk, IO_OFF);
			nIn[2] = FAS_IO.get_in_bit(st_io.i_distribute_rbt2_right_top_vacuum_chk, IO_OFF);
			nIn[3] = FAS_IO.get_in_bit(st_io.i_distribute_rbt2_right_btm_vacuum_chk, IO_OFF);

			
			if ( st_handler.n_without_bcr2_rbt == CTL_NO &&
				(st_basic.mn_mode_run == 1 || st_handler.mn_curr_back_conv == 1) )
			{
				//2014,1222
				if(m_vecbcrRev2.size() == 0)
				{
					//2015.0327
					if( ( g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" || g_lotMgr.GetLotCount() < 1 ) &&						
						( st_var.n_use_barcode[0] != EMBS_SKIP && st_var.n_use_barcode[1] != EMBS_SKIP ) )
					{
						if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_RIGHT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_RIGHT] == 0)
						{
							Move_Barcode_skip(2);
						}//2015.0420
						else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_last_module[CTL_LEFT] == ID_LOT_END ||
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_last_module[CTL_RIGHT] == ID_LOT_END )
						{
							Move_Barcode_skip(2);
						}
						else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_LEFT] == ID_LOT_END ||
							st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_RIGHT] == ID_LOT_END )
						{//2015.0622
							Move_Barcode_skip(2);
						}
					}
					break;
				}
				else//2015.0420
				{
					if( ( g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" ) &&						
						( st_var.n_use_barcode[0] != EMBS_SKIP && st_var.n_use_barcode[1] != EMBS_SKIP ) )
					{
						if( (st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_LEFT] == ID_LOT_END ||
							st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_RIGHT] == ID_LOT_END) && 
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_RIGHT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == 0 )
						{
							Move_Barcode_skip(2);
							break;
						}
						else if( (st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_LEFT] == ID_LOT_END ||
							st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_RIGHT] == ID_LOT_END) && 
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT] == 0 &&
							st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == 0 )
						{//2015.0514
							Move_Barcode_skip(2);
							break;
						}
						else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_LEFT] == ID_LOT_END ||
							st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_RIGHT] == ID_LOT_END)
						{//2015.0622
							Move_Barcode_skip(2);
							break;
						}
					}
				}


				std::vector<stBcrMsg>::iterator it = m_vecbcrRev2.begin();
				for( int i=0; i<m_vecbcrRev2.size(); i++ )
				{
					stBcrMsg& msg = m_vecbcrRev2.at(i);
					//if(msg.iID == nSite + 2 ) //2015.0301
					if(msg.iID == nSite && i < 2)
					{
//						if(mn_iBUF2 == -1)
//						{
//							ReqBcrOutPrint(nSite+2, msg.strfmt);
//							m_nRun_Print[nSite+2] = 1000;
//							m_strCmpSerial[nSite+2].Empty();
//							m_strCmpSerial[nSite+2] = LPCTSTR(msg.strbcr);
//							mn_iBUF2 = msg.iBUF;
//						}
//						else if(msg.iBUF == mn_iBUF2)
//						{
							ReqBcrOutPrint(nSite+2, msg.strfmt);
							m_nRun_Print[nSite+2] = 1000;
							m_strCmpSerial[nSite+2].Empty();
							m_strCmpSerial[nSite+2] = LPCTSTR(msg.strbcr);

//						}
					}
				}
			}
			else
			{
				if ( st_handler.n_without_bcr1_rbt == CTL_NO &&
					(1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[nSite].nCommand == CTL_YES ) )
				{
					mn_buff_place[nSite] = SHIFT_IDBUFF_INDEX_DENSITY;
 					ReqBcrOutPrint(nSite, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
					m_nRun_Print[nSite] = 1000;	
				}
			}
		}
		else//2015.0224
		{
// 			Move_Barcode_skip(2);
// 			if(m_vecbcrRev2.size() == 0) 
// 				break;
// 
// 			std::vector<stBcrMsg>::iterator it = m_vecbcrRev2.begin();
// 			for( int i=0; i<m_vecbcrRev2.size(); i++ )
// 			{
// 				stBcrMsg& msg = m_vecbcrRev2.at(i);
// 				//if(msg.iID == nSite + 2 ) //2015.0301
// 				if(msg.iID == (nSite + 2) && i < 2 )
// 				{					
// 					m_vecbcrRev2.erase(it);//지워버린다.리젝처리한다.
// 				}
// 			}
		}
		break;

	case 1000:
// 		if( st_basic.nBcrFeederUse[nSite+2] == CTL_YES) 
// 		{
// 			//¹UAUμa Aa·A¹®AU Au¼U
// 			ReqBcrOutPrint(nSite+2, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode1[nSite].c_Format);//*/ Func.m_pBctFmtClient[nSite]->GetBcrFormat());
// 			ReqBcrWork(nSite+2, EPRI_FORWARD );
			
			ReqBcrWork(nSite+2, EPRI_FORWARD );
			m_nRun_Print[nSite+2] = 2000;
// 		}
		break;

	case 2000:
		{
			if( mb_BcrComplete[nSite+2] != true ) break;
			if( m_fbFeeder[nSite+2] != EPRI_NONE ) break;			
			
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[nSite].nCommand	= CTL_NO;
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[nSite].nComplete = CTL_YES;

			m_nRun_Print[nSite+2] = 2100;
			
		}
		break;

	case 2100: 
		//바코드 프린터에 출력을 보냈다면 !! m_bPrint_Pickup[nSite] = true
		if( st_basic.nBcrFeederUse[nSite+2] == CTL_YES &&  m_bPrint_Pickup[nSite+2] == true )//Barcode server comm end
		{
			m_bPrint_Pickup[nSite+2] = false;
			m_nRun_Print[nSite+2] = 3000;
		}
		else if(st_basic.nBcrFeederUse[nSite+2] == CTL_NO)//2015.0301
		{
			m_bPrint_Pickup[nSite+2] = false;
			m_nRun_Print[nSite+2] = 3000;			
		}
		break;

	case 3000:
		{			
			mb_BcrComplete[nSite+2] = false;		
			m_nRun_Print[nSite+2] = 4000;
		}
		break;

	case 4000:
		{
			if( GetPrintResult(nSite+2) == EMBS_PASS )
			{
				CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
				CString strBcrInfo = GetPrintSerial(nSite+2);
				
//				ABcrLabel bcrlbl;
//				bcrlbl.SetBcrLabelInfo(strLotID, strBcrInfo, EMBS_PASS );
//				ABcrLabel* bcrtemp = NULL;
//				bcrtemp = &Func.m_pSite[SITE_BARCODE1+nSite]->GetBcrLabel(nSite);
//				*bcrtemp = bcrlbl;
				
//				st_bcr_feeder[nSite+2].bPick = true;
				st_bcr_feeder[nSite+2].st_bcr.bExist = true;
				st_bcr_feeder[nSite+2].st_bcr.bOut = true;
				st_bcr_feeder[nSite+2].st_bcr.nState = EMBS_PASS;
				sprintf(st_bcr_feeder[nSite+2].st_bcr.c_lot_id,"%s",strLotID);
				sprintf(st_bcr_feeder[nSite+2].st_bcr.c_serial,"%s",strBcrInfo);
				
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].c_lot_id,"%s", strLotID);
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].c_serial,"%s", strBcrInfo);
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].c_Format,"%s",Func.m_pBctFmtClient[nSite%2]->GetBcrFormat());
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].bOut = 1;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].bExist = 1;
				//st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[nSite%2].nState = EMBS_FAIL;
				
				m_nRun_Print[nSite+2] = 5000;
			}
			else if( GetPrintResult(nSite+2) == EMBS_FAIL)
			{
				CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
				CString strBcrInfo = GetPrintSerial(nSite);
				
//				ABcrLabel bcrlbl;
//				bcrlbl.SetBcrLabelInfo(strLotID, strBcrInfo, EMBS_FAIL );
//
//				ABcrLabel* bcrtemp = NULL;
//				bcrtemp = &Func.m_pSite[SITE_BARCODE1+nSite]->GetBcrLabel(nSite);
//				*bcrtemp = bcrlbl;
//				st_bcr_feeder[nSite+2].bPick = true;
				st_bcr_feeder[nSite+2].st_bcr.bExist = true;
				st_bcr_feeder[nSite+2].st_bcr.bOut = true;
				st_bcr_feeder[nSite+2].st_bcr.nState = EMBS_FAIL;
				sprintf(st_bcr_feeder[nSite+2].st_bcr.c_lot_id,"%s",strLotID);
				sprintf(st_bcr_feeder[nSite+2].st_bcr.c_serial,"%s",strBcrInfo);
				
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].c_lot_id,"%s", strLotID);
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].c_serial,"%s", strBcrInfo);
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].c_Format,"%s",Func.m_pBctFmtClient[nSite%2]->GetBcrFormat());
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].bOut = 1;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].bExist = 1;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode2[nSite%2].nState = EMBS_FAIL;
				
				m_nRun_Print[nSite+2] = 5000;
			}
		}
		break;
	
	case 5000:
// 		if( st_var.n_use_barcode[nSite+2] == EMBS_SKIP )
// 		{
// 			int aSite = -1;
// 			nSite == 0 ? aSite = 1 : aSite = 0;
// 			if( st_var.n_use_barcode[aSite+2] == EMBS_PASS || st_var.n_use_barcode[aSite+2] == EMBS_FAIL )
// 			{
// 				st_bcr_feeder[aSite+2].st_bcr.nState = EMBS_FAIL;
// 				st_var.n_use_barcode[aSite+2] = EMBS_SKIP;
// 				Move_Barcode_skip(nSite+2);
// 			}
// 		}
		//2015.0327
// 		if( st_var.n_use_barcode[2] == EMBS_SKIP || st_var.n_use_barcode[3] == EMBS_SKIP )
// 		{
// 			if( st_var.n_use_barcode[2] == EMBS_SKIP )
// 			{
// 				if( st_basic.nBcrFeederUse[2] == CTL_YES )
// 				{
// 					st_basic.nBcrFeederUse[2] = CTL_NO;
// 					st_bcr_feeder[2].st_bcr.nState = EMBS_FAIL;
// 					Move_Barcode_skip(2);
// 				}
// 				if( st_basic.nBcrFeederUse[3] == CTL_YES && st_var.n_use_barcode[3] == EMBS_PASS )
// 				{
// 					st_basic.nBcrFeederUse[3] = CTL_NO;
// 					st_bcr_feeder[3].st_bcr.nState = EMBS_FAIL;
// 				}
// 			}
// 			if( st_var.n_use_barcode[3] == EMBS_SKIP )
// 			{
// 				if( st_basic.nBcrFeederUse[3] == CTL_YES )
// 				{
// 					st_basic.nBcrFeederUse[3] = CTL_NO;
// 					st_bcr_feeder[3].st_bcr.nState = EMBS_FAIL;
// 					Move_Barcode_skip(3);
// 				}
// 				if( st_basic.nBcrFeederUse[2] == CTL_YES && st_var.n_use_barcode[3] == EMBS_PASS )
// 				{
// 					st_basic.nBcrFeederUse[2] = CTL_NO;
// 					st_bcr_feeder[2].st_bcr.nState = EMBS_FAIL;
// 				}
// 			}
// 			if( st_var.n_use_barcode[2] == EMBS_SKIP )
// 			{
// 				if( st_basic.nBcrFeederUse[2] == CTL_YES &&
// 					( st_var.n_use_barcode[3] == EMBS_PASS || st_var.n_use_barcode[3] == EMBS_FAIL ) )
// 				{//2번이 skip 이므로 2번 사용 불능
// 					st_basic.nBcrFeederUse[2] = CTL_NO;
// 					st_bcr_feeder[3].st_bcr.nState = EMBS_FAIL;
// 					st_var.n_use_barcode[3] = EMBS_SKIP;
// 					Move_Barcode_skip(3);
// 				}
// 				else if( st_basic.nBcrFeederUse[2] == CTL_YES && st_var.n_use_barcode[3] == EMBS_SKIP )
// 				{
// 					st_basic.nBcrFeederUse[2] = CTL_NO;
// 					st_bcr_feeder[3].st_bcr.nState = EMBS_FAIL;
// 					Move_Barcode_skip(3);
// 				}
// 			}
// 			else if( st_var.n_use_barcode[3] == EMBS_SKIP )
// 			{
// 				if( st_basic.nBcrFeederUse[3] == CTL_YES &&
// 					( st_var.n_use_barcode[2] == EMBS_PASS || st_var.n_use_barcode[2] == EMBS_FAIL ) )
// 				{//3번이 skip 이므로 3번 사용 불능
// 					st_basic.nBcrFeederUse[3] = CTL_NO;
// 					st_bcr_feeder[2].st_bcr.nState = EMBS_FAIL;
// 					st_var.n_use_barcode[2] = EMBS_SKIP;
// 					Move_Barcode_skip(2);
// 				}
// 				else if( st_basic.nBcrFeederUse[3] == CTL_YES && st_var.n_use_barcode[2] == EMBS_SKIP )
// 				{
// 					st_basic.nBcrFeederUse[3] = CTL_NO;
// 					st_bcr_feeder[2].st_bcr.nState = EMBS_FAIL;
// 					Move_Barcode_skip(2);
// 				}
// 			}
//		}
		
		if ( st_bcr_feeder[nSite+2].st_bcr.bExist == false)
		{
			
			bool nRejectUp = false;
			bool bRet = false;
			if(st_rej_rbt.bPick[nSite+2] == true ) 
			{
				nRejectUp = true;
				st_rej_rbt.bPick[nSite+2] = false;
// 				break;
			}
			
			if( st_basic.mn_mode_run == CTL_YES && st_basic.n_mode_device == CTL_YES && m_vecbcrRev2.size() == 0)
			{
				if(st_handler.cwnd_list != NULL )
				{
					sprintf(st_msg.c_abnormal_msg, "[Run_BCR2] vecbcrRev size is zero");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				break;
			}
			
			if( st_basic.mn_mode_run == CTL_YES && st_basic.n_mode_device == CTL_YES )
			{
				std::vector<stBcrMsg>::iterator it = m_vecbcrRev2.begin();
				for( int i=0; i<m_vecbcrRev2.size(); i++ )
				{
					stBcrMsg& msg = m_vecbcrRev2.at(i);
					if( Func.CmpStrFile(msg.strbcr, m_strCmpSerial[nSite+2]) )
					{
						it += i;
						bRet = true;
						if ( nRejectUp == true )
						{

							m_nRun_Print[nSite+2] = 0;
							return;
						}
						break;
					}
				}
				if( st_basic.n_mode_device == CTL_YES && bRet == false)
				{//650000 1 00 "barcode is not same."
					CTL_Lib.Alarm_Error_Occurrence( 1914, dWARNING, "650000" );
					return;
				}
				
				if( st_basic.n_mode_device == CTL_YES )
					m_vecbcrRev2.erase(it);
				
			}
			//2015.0530
			for ( int k = 0; k < 2; k++)
			{
				if( st_work.n_mat_bcr[k+2] <= 1 ) st_work.n_mat_bcr[k+2] = 3000;
				else							  st_work.n_mat_bcr[k+2] -= 1;
				if( st_work.n_mat_ribbon[k+2] <= 1 ) st_work.n_mat_ribbon[k+2] = 5000;
				else								 st_work.n_mat_ribbon[k+2] -= 1;
			}
			
			m_nRun_Print[nSite+2] = 0;
		}
		break;
	}
}

void CRun_Barcode_Feeders::Move_Barcode_skip(int nFromSite)
{
	int nSite = -1;
	if(nFromSite == 0 || nFromSite == 1)
	{
// 		nFromSite == 0 ? nSite = 1 : nSite = 0;
// 
// 		for( int i=0; i<m_vecbcrRev.size(); i++ )
// 		{
// 			stBcrMsg& msg = m_vecbcrRev.at(i);
// 			if(msg.iID == nSite)
// 			{					
// 				m_vecbcrRev2.push_back(msg);
// 			}
// 		}


		if(st_var.n_using_barcode != 1)
		{
			m_vecbcrRev2.clear();
			for( int i=0; i<m_vecbcrRev.size(); i++ )
			{
				stBcrMsg& msg = m_vecbcrRev.at(i);
				m_vecbcrRev2.push_back(msg);
			}
			st_basic.nBcrFeederUse[2] = CTL_YES;
			st_basic.nBcrFeederUse[3] = CTL_YES;
			st_var.n_using_barcode = 1;
		}
	}
	else if(nFromSite == 2 || nFromSite == 3)
	{
		if(st_var.n_using_barcode != 0)
		{
			m_vecbcrRev.clear();
			for( int i=0; i<m_vecbcrRev2.size(); i++ )
			{
				stBcrMsg& msg = m_vecbcrRev2.at(i);
				m_vecbcrRev.push_back(msg);
			}		
			st_basic.nBcrFeederUse[0] = CTL_YES;
			st_basic.nBcrFeederUse[1] = CTL_YES;
			st_var.n_using_barcode = 0;
		}
	}
}

void CRun_Barcode_Feeders::Barcode_Received( int nLot, int nPos, CString strbcr, CString strBcrFormat )
{
	stBcrMsg msg;
	{
		msg.iBUF = nLot;
		msg.iID = nPos;//bcr pos
		msg.strbcr = strbcr;//bcr serial
		msg.strfmt = strBcrFormat;//bcr format
	}
	
	m_vecbcrRev.push_back( msg );
	
	if( m_vecbcrRev.size() > 10 )
	{
		m_vecbcrRev.clear();
	}	
}

void CRun_Barcode_Feeders::Barcode_Received2( int nLot, int nPos, CString strbcr, CString strBcrFormat )
{
	stBcrMsg msg;
	{
		msg.iBUF = nLot;
		msg.iID = nPos;//bcr pos
		msg.strbcr = strbcr;//bcr serial
		msg.strfmt = strBcrFormat;//bcr format
	}
	
	m_vecbcrRev2.push_back( msg );
	
	if( m_vecbcrRev2.size() > 10 )
	{
		m_vecbcrRev2.clear();
	}	
}

void CRun_Barcode_Feeders::Run_Move( int nSite)
{
	Func.ThreadFunctionStepTrace(15, m_nRun_Print[nSite]);
	
	int nIn[4];
	bool bPickInfo[2];
	bPickInfo[0] = bPickInfo[1] = false;	

	if(st_handler.n_only_work_bcr1 == CTL_YES)
	{
		st_sync.n_bcr_1_work_req = CTL_READY;
		st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[nSite].nCommand = CTL_YES;
	}

	switch(m_nRun_Print[nSite])
	{
	case 0: 
		if( g_lotMgr.GetLotCount() < 1 && st_handler.n_only_work_bcr1 == CTL_NO ) break;
		m_nRun_Print[nSite] = 100;
		break;

	case 100:
		if( st_basic.nBcrFeederUse[nSite] == CTL_YES && st_var.n_use_barcode[nSite] != EMBS_SKIP && st_var.n_using_barcode == 0 )
		{
			nIn[0] = FAS_IO.get_in_bit(st_io.i_distribute_rbt1_left_top_vacuum_chk, IO_OFF);
			nIn[1] = FAS_IO.get_in_bit(st_io.i_distribute_rbt1_left_btm_vacuum_chk, IO_OFF);
			nIn[2] = FAS_IO.get_in_bit(st_io.i_distribute_rbt1_right_top_vacuum_chk, IO_OFF);
			nIn[3] = FAS_IO.get_in_bit(st_io.i_distribute_rbt1_right_btm_vacuum_chk, IO_OFF);
			
			if( st_handler.n_without_bcr1_rbt == CTL_NO && 
				(st_basic.mn_mode_run == 1 || st_handler.mn_curr_back_conv == 1) )
			{

				//2014,1222
				if(m_vecbcrRev.size() == 0) 
					break;

				std::vector<stBcrMsg>::iterator it = m_vecbcrRev.begin();
				for( int i=0; i<m_vecbcrRev.size(); i++ )
				{
					stBcrMsg& msg = m_vecbcrRev.at(i);
					//if(msg.iID == nSite) //2015.0301
					if(msg.iID == nSite && i < 2)
					{					
						ReqBcrOutPrint(nSite, msg.strfmt);
						m_nRun_Print[nSite] = 1000;
						m_strCmpSerial[nSite].Empty();
						m_strCmpSerial[nSite] = LPCTSTR(msg.strbcr);
						st_var.n_use_barcode[nSite] = EMBS_LOAD;
					}
				}
			}
			else
			{
				if ( st_handler.n_without_bcr1_rbt == CTL_NO &&
					(1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[nSite].nCommand == CTL_YES ) )
				{
					mn_buff_place[nSite] = SHIFT_IDBUFF_INDEX_DENSITY;
 					ReqBcrOutPrint(nSite, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
					m_nRun_Print[nSite] = 1000;	
				}
			}
			
		}
		else//2015.0224
		{
// 			Move_Barcode_skip(0);
// 			if(m_vecbcrRev.size() == 0) 
// 				break;
// 
// 			std::vector<stBcrMsg>::iterator it = m_vecbcrRev.begin();
// 			for( int i=0; i<m_vecbcrRev.size(); i++ )
// 			{
// 				stBcrMsg& msg = m_vecbcrRev.at(i);
// 				//if(msg.iID == nSite) //2015.0301
// 				if(msg.iID == nSite && i < 2)
// 				{					
// 					m_vecbcrRev.erase(it);//지워버린다.리젝처리한다.
// 				}
// 			}
		}

		break;

	case 1000:
//		if( g_RunNetwork.st_barcode[nSite].nComplete == CTL_YES)
//		{
			//바코드 출력문자 전송
// 			ReqBcrOutPrint(nSite, m_strBcrFormat[nSite]);// Func.m_pBctFmtClient[nSite]->GetBcrFormat());
// 			ReqBcrWork(nSite, EPRI_FORWARD );
			//2014,1222


// 			ReqBcrOutPrint(nSite, msg.strfmt);
			ReqBcrWork(nSite, EPRI_FORWARD );
			m_nRun_Print[nSite] = 2000;
//		}
		break;

	case 2000:
		{
			if( mb_BcrComplete[nSite] != true ) break;
			if( m_fbFeeder[nSite] != EPRI_NONE ) break;	

// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[nSite].nCommand	= CTL_NO;
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[nSite].nComplete = CTL_YES;
// 			st_idbuff_info[mn_buff_place[nSite]].st_barcode1[nSite].nCommand = CTL_NO;
// 			st_idbuff_info[mn_buff_place[nSite]].st_barcode1[nSite].nComplete = CTL_YES;

			m_nRun_Print[nSite] = 2100;
			
		}
		break;

	case 2100: 
		//바코드 프린터에 출력을 보냈다면 !! m_bPrint_Pickup[nSite] = true
		if( st_basic.nBcrFeederUse[nSite] == CTL_YES &&  m_bPrint_Pickup[nSite] == true )//Barcode server comm end
		{
			m_bPrint_Pickup[nSite] = false;
			m_nRun_Print[nSite] = 3000;
		}
		else if(st_basic.nBcrFeederUse[nSite] == CTL_NO)//2015.0301
		{
			m_bPrint_Pickup[nSite] = false;
			m_nRun_Print[nSite] = 3000;			
		}
		else if(st_handler.mn_curr_unload_conv != -1)
		{
			if(st_handler.mn_curr_unload_conv == nSite)
			{
				st_handler.mn_curr_unload_conv = -1;
				m_bPrint_Pickup[nSite] = false;
				m_nRun_Print[nSite] = 0;

			}
		}
		break;

	case 3000:
		{			
			mb_BcrComplete[nSite] = false;		
			m_nRun_Print[nSite] = 4000;
		}
		break;

	case 4000:
		{
			if( GetPrintResult(nSite) == EMBS_PASS )
			{
				CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
				CString strBcrInfo = GetPrintSerial(nSite);
				
				st_bcr_feeder[nSite].st_bcr.bExist = true;
				st_bcr_feeder[nSite].st_bcr.bOut = true;
				st_bcr_feeder[nSite].st_bcr.nState = EMBS_PASS;
				sprintf(st_bcr_feeder[nSite].st_bcr.c_lot_id,"%s",strLotID);
				sprintf(st_bcr_feeder[nSite].st_bcr.c_serial,"%s",strBcrInfo);
				
				st_handler.nCommand[nSite] = CTL_NO;
				st_handler.nComplete[nSite] = CTL_YES;
				m_nRun_Print[nSite] = 5000;
			}
			else if( GetPrintResult(nSite) == EMBS_FAIL)
			{
				CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
				CString strBcrInfo = GetPrintSerial(nSite);
				
				st_bcr_feeder[nSite].st_bcr.bExist = true;
				st_bcr_feeder[nSite].st_bcr.bOut = true;
				st_bcr_feeder[nSite].st_bcr.nState = EMBS_FAIL;
				sprintf(st_bcr_feeder[nSite].st_bcr.c_lot_id,"%s",strLotID);
				sprintf(st_bcr_feeder[nSite].st_bcr.c_serial,"%s",strBcrInfo);
				
				m_nRun_Print[nSite] = 5000;
			}
		}
		break;

	case 5000:
		//2015.0322
// 		if( st_var.n_use_barcode[nSite] == EMBS_SKIP )
// 		{
// 			int aSite = -1;
// 			nSite == 0 ? aSite = 1 : aSite = 0;
// 			if( st_var.n_use_barcode[aSite] == EMBS_PASS || st_var.n_use_barcode[aSite] == EMBS_FAIL )
// 			{
// 				st_bcr_feeder[aSite].st_bcr.nState = EMBS_FAIL;
// 				st_var.n_use_barcode[aSite] = EMBS_SKIP;
// 				Move_Barcode_skip(nSite);
// 			}
// 		}
		//2015.0327
		if( st_var.n_use_barcode[0] == EMBS_SKIP || st_var.n_use_barcode[1] == EMBS_SKIP )
		{
			if( st_var.n_use_barcode[0] == EMBS_SKIP )
			{
				if( st_basic.nBcrFeederUse[0] == CTL_YES )
				{
					st_basic.nBcrFeederUse[0] = CTL_NO;
					st_bcr_feeder[0].st_bcr.nState = EMBS_FAIL;
					Move_Barcode_skip(0);
				}
				if( st_basic.nBcrFeederUse[1] == CTL_YES && st_var.n_use_barcode[1] == EMBS_PASS )
				{
					st_basic.nBcrFeederUse[1] = CTL_NO;
					st_bcr_feeder[1].st_bcr.nState = EMBS_FAIL;
				}
			}
			if( st_var.n_use_barcode[1] == EMBS_SKIP )
			{
				if( st_basic.nBcrFeederUse[1] == CTL_YES )
				{
					st_basic.nBcrFeederUse[1] = CTL_NO;
					st_bcr_feeder[1].st_bcr.nState = EMBS_FAIL;
					Move_Barcode_skip(1);
				}
				if( st_basic.nBcrFeederUse[0] == CTL_YES && st_var.n_use_barcode[0] == EMBS_PASS )
				{
					st_basic.nBcrFeederUse[0] = CTL_NO;
					st_bcr_feeder[0].st_bcr.nState = EMBS_FAIL;
				}
			}
		}
		

		if ( 0 && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[nSite].nComplete != CTL_NO) break;
		if ( st_bcr_feeder[nSite].st_bcr.bExist == false)
		{
			bool nRejectUp = false;
			bool bRet = false;

			if( st_rej_rbt.bPick[nSite] == true ) 
			{
				nRejectUp = true;
				st_rej_rbt.bPick[nSite] = false;
// 				break;
			}

			if( st_basic.mn_mode_run == CTL_YES && st_basic.n_mode_device == CTL_YES && m_vecbcrRev.size() == 0)
			{
				if(st_handler.cwnd_list != NULL )
				{
					sprintf(st_msg.c_abnormal_msg, "[Run_BCR1] vecbcrRev size is zero");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				break;
			}

			if( st_basic.mn_mode_run == CTL_YES && st_basic.n_mode_device == CTL_YES )
			{
				std::vector<stBcrMsg>::iterator it = m_vecbcrRev.begin();
				for( int i=0; i<m_vecbcrRev.size(); i++ )
				{
					stBcrMsg& msg = m_vecbcrRev.at(i);
					if( Func.CmpStrFile(msg.strbcr, m_strCmpSerial[nSite]) )
					{
						it += i;
						bRet = true;
						if ( nRejectUp == true )
						{
							m_nRun_Print[nSite] = 0;
							return;
						}
						break;
					}
				}
				if( st_basic.n_mode_device == CTL_YES && bRet == false)
				{//650000 1 00 "barcode is not same."
					CTL_Lib.Alarm_Error_Occurrence( 914, dWARNING, "650000" );
					return;
				}

				if( st_basic.n_mode_device == CTL_YES )
				{
					m_vecbcrRev.erase(it);
				}

			}

			m_nRun_Print[nSite] = 0;

			//2015.0530
			for ( int k = 0; k < 2; k++)
			{
				if( st_work.n_mat_bcr[k] <= 1 ) st_work.n_mat_bcr[k] = 3000;
				else                            st_work.n_mat_bcr[k] -= 1;
				if( st_work.n_mat_ribbon[k] <= 1 ) st_work.n_mat_ribbon[k] = 5000;
				else								 st_work.n_mat_ribbon[k] -= 1;
			}

		}
		break;
	}
}

void CRun_Barcode_Feeders::Run_Move_Vision()
{
	switch(mn_step_vis)
	{
	case 0:
		mn_step_vis = 100;
		break;

	case 100:
		if( n_bcr_vision_req[0] == CTL_REQ || n_bcr_vision_req[1] == CTL_REQ )
		{
			mn_step_vis = 1000;
		}
		else if( n_bcr_vision_req[2] == CTL_REQ || n_bcr_vision_req[3] == CTL_REQ )
		{
			mn_step_vis = 2000;
		}
		break;

	case 1000:		
		if( n_bcr_vision_req[0] == CTL_REQ  )
		{
			double dpos = COMI.Get_MotCurrentPos(M_BCR_VISION_X) ;
			if( dpos > st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1] - COMI.md_allow_value[M_BCR_VISION_X] &&
				dpos < st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1] + COMI.md_allow_value[M_BCR_VISION_X])
			{
				n_bcr_vision_req[0] = CTL_READY;
				mn_step_vis = 1300;
			}
			else
			{
				mn_step_vis = 1200;
			}
		}
		else if( n_bcr_vision_req[1] == CTL_REQ )
		{
			if( st_basic.nBcrFeederUse[0] == 1 )
			{
				mn_step_vis = 1100;
				m_dwTimeVision = GetCurrentTime();
			}
			else
			{
				mn_step_vis = 1500;
			}
		}
		break;

	case 1100:
		if( GetCurrentTime() - m_dwTimeVision <= 0 ) m_dwTimeVision = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeVision < 1000 ) break;

		if( n_bcr_vision_req[0] == CTL_REQ )
		{
			double dpos = COMI.Get_MotCurrentPos(M_BCR_VISION_X) ;
			if( dpos > st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1] - COMI.md_allow_value[M_BCR_VISION_X] &&
				dpos < st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1] + COMI.md_allow_value[M_BCR_VISION_X])
			{
				n_bcr_vision_req[0] = CTL_READY;
				mn_step_vis = 1300;
			}
			else
			{
				mn_step_vis = 1200;
			}
		}
		else if( n_bcr_vision_req[1] == CTL_REQ )
		{
			mn_step_vis = 1500;
		}
		break;

	case 1200:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD )
			{
				n_bcr_vision_req[0] = CTL_READY;
				mn_step_vis = 1300;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
// 				CTL_Lib.Alarm_Error_Occurrence(1224, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case 1300:
		if(n_bcr_vision_req[0] == CTL_NO )
			mn_step_vis = 1400;
		break;

	case 1400:
		if( st_basic.nBcrFeederUse[1] == 1 )
		{
			//mn_step_vis = 1500;
			double dpos = COMI.Get_MotCurrentPos(M_BCR_VISION_X) ;
			if( dpos > st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F2] - COMI.md_allow_value[M_BCR_VISION_X] &&
				dpos < st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F2] + COMI.md_allow_value[M_BCR_VISION_X])
			{
				m_dwTimeVision = GetCurrentTime();
				mn_step_vis = 1600;
			}
			else
			{
				mn_step_vis = 1500;
			}

		}
		else
		{
			mn_step_vis = 100;
		}
		break;

		if( n_bcr_vision_req[1] == CTL_REQ ) 
			mn_step_vis = 1500;
		else
			mn_step_vis = 100;
		break;

	case 1500:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F2], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD )
			{
				//n_bcr_vision_req[1] = CTL_READY;
				m_dwTimeVision = GetCurrentTime();
				mn_step_vis = 1600;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_2);
				CTL_Lib.Alarm_Error_Occurrence(1224, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case 1600:
		if( GetCurrentTime() - m_dwTimeVision <= 0 ) m_dwTimeVision = GetCurrentTime();
		if( st_basic.nBcrFeederUse[1] == 1 && n_bcr_vision_req[1] == CTL_REQ )
		{
			n_bcr_vision_req[1] = CTL_READY;
			mn_step_vis = 1700;
			break;
		}
		if( st_basic.nBcrFeederUse[1] == 0 )
			mn_step_vis = 100;

		if( GetCurrentTime() - m_dwTimeVision > 1500 )
			mn_step_vis = 100;

		break;

	case 1700:
		if(n_bcr_vision_req[1] == CTL_NO )
			mn_step_vis = 1800;
		break;

	case 1800:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD )
			{
				mn_step_vis = 100;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
				CTL_Lib.Alarm_Error_Occurrence(1229, CTL_dWARNING, strJamCode );
			}
		}
		break;


	case 2000:
		if( n_bcr_vision_req[2] == CTL_REQ  )
		{
			mn_step_vis = 2200;
		}
		else if( n_bcr_vision_req[3] == CTL_REQ )
		{
// 			if( st_basic.nBcrFeederUse[2] == 1 )
// 			{
// 				mn_step_vis = 2100;
// 				m_dwTimeVision = GetCurrentTime();
// 			}
// 			else
// 			{
				mn_step_vis = 2500;
// 			}
		}
		break;

	case 2100:
		if( GetCurrentTime() - m_dwTimeVision <= 0 ) m_dwTimeVision = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeVision < 1000 ) break;

		if( n_bcr_vision_req[2] == CTL_REQ )
		{
			mn_step_vis = 2200;
		}
		else if( n_bcr_vision_req[3] == CTL_REQ )
		{
			mn_step_vis = 2500;
		}
		break;

	case 2200:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F3], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD )
			{
				n_bcr_vision_req[2] = CTL_READY;
				mn_step_vis = 2300;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_3);
				CTL_Lib.Alarm_Error_Occurrence(1224, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case 2300:
		if(n_bcr_vision_req[2] == CTL_NO )
			mn_step_vis = 2400;
		break;

	case 2400:
		if( n_bcr_vision_req[3] == CTL_REQ ) 
			mn_step_vis = 2500;
		else
			mn_step_vis = 100;

	case 2500:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F4], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD )
			{
				n_bcr_vision_req[3] = CTL_READY;
				mn_step_vis = 2600;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
				CTL_Lib.Alarm_Error_Occurrence(1224, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case 2600:
		if(n_bcr_vision_req[3] == CTL_NO )
			mn_step_vis = 100;
		break;

	}
}

void CRun_Barcode_Feeders::ReqBcrWork( int nSite, EPRI esf )
{
	m_fbFeeder[nSite] = esf;
	m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY;
}

void CRun_Barcode_Feeders::Run_Move_Feeder( int nSite)
{
	if( m_fbFeeder[nSite] == EPRI_NONE) return;

	int nMotorNum = 0;
	if(nSite == 0) nMotorNum = M_BCR_FEEDER_1;
	else if(nSite == 1) nMotorNum = M_BCR_FEEDER_2;
	else if(nSite == 2) nMotorNum = M_BCR_FEEDER_3;
	else if(nSite == 3) nMotorNum = M_BCR_FEEDER_4;

	//바코드 비젼검사후 
	switch(m_step_bcrfeeder[nSite])
	{
	case ESTEP_FEEDER_NONE: break;
	case ESTEP_FEEDER_READY:
		{
// 			int nLock_1 = st_io.i_distribute_plate_up_chk[nSite] ;
// 			int nLock_2 = st_io.i_distribute_plate_down_chk[nSite];
// 			if( g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||  g_ioMgr.get_in_bit( nLock_2 ) != IO_ON)
// 				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY_DOWN;
// 			else
// 				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_PICK;				

			int nLock_1 = st_io.i_distribute_plate_up_chk[nSite] ;//2014.1120 ysj
			int nLock_2 = st_io.i_distribute_plate_down_chk[nSite];
			if( g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||  g_ioMgr.get_in_bit( nLock_2 ) != IO_ON)
			{
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY_DOWN;
			}
			else
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_ON);
				double dCurrMotPos = COMI.Get_MotCurrentPos(nMotorNum);
				if(dCurrMotPos < st_motor[nMotorNum].d_pos[P_BCR_FEEDER_PICK])
				{
					m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY_DOWN;				
				}
				else
				{
					m_dwTimerFeeder[nSite] = GetCurrentTime();//2014.1120 ysj
					m_step_bcrfeeder[nSite] = ESTEP_FEEDER_PICK;
				}
			}
		}
		break;

	case ESTEP_FEEDER_READY_DOWN:
		ReqPlateUpDn( nSite, BACKWARD );
		g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_ON);
		m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(21);

// 		m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY_DOWN_CHK;
		break;

	case 21://2015.0220
		{
			double dpos = COMI.Get_MotCurrentPos(nMotorNum);
			if( dpos >= st_motor[nMotorNum].d_pos[P_BCR_FEEDER_LOAD] - COMI.md_allow_value[nMotorNum] &&
				dpos <= st_motor[nMotorNum].d_pos[P_BCR_FEEDER_LOAD] + COMI.md_allow_value[nMotorNum])
			{
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY_DOWN_CHK;
			}
			else
			{
				if( m_fbplateupdn[nSite] != FB_NONE) break;
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_READY_DOWN_CHK;
			}
		}
		break;

	case ESTEP_FEEDER_READY_DOWN_CHK:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_PICK], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
//				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_ON);//2014.1120 ysj
				m_dwTimerFeeder[nSite] = GetCurrentTime();
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_PICK;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
				CTL_Lib.Alarm_Error_Occurrence(124, CTL_dWARNING, strJamCode );
			}
		}
		break;
		
	case ESTEP_FEEDER_PICK:
		{
			if( m_fbplateupdn[nSite] != FB_NONE) break;
			if( m_spoBcrout[nSite] != EBOS_NONE) break;
			
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < 0 ) m_dwTimerFeeder[nSite] = GetCurrentTime();
			if(mb_BcrComplete[nSite] == true )//네트웍으로 받기 바코드 출력완료
			{
//				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_OFF);//2014.1120 ysj
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UP;	
				m_dwTimerFeeder[nSite] = GetCurrentTime();
				////2014,1214
//				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_OFF);
// 				g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[nSite], IO_ON);//2015.0216
				break;
				////
			}
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] > st_time.n_limit_time[E_WAIT_BCR_OUT])//타입아웃
			{
// 				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UP;	
// //				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_OFF);//2014.1120 ysj
// 				m_dwTimerFeeder[nSite] = GetCurrentTime();
				//700200 1 00 "Barcode1_time_out_from_server."
				CString strError;
				strError.Format("7004%d0", nSite);
				CTL_Lib.Alarm_Error_Occurrence(797, dWARNING, strError);

				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(41);
				
				clsZebra.OnPrintAnswerMode(2, 0, nSite+CLS_BCR_PRINTER1);
			}
		}
		break;

	case 41:
		m_dwTimerFeeder[nSite] = GetCurrentTime();
		m_step_bcrfeeder[nSite] = ESTEP_FEEDER_PICK;
		break;

	case ESTEP_FEEDER_UP:
		{
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < 0 ) m_dwTimerFeeder[nSite] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < st_time.n_wait_time[E_WAIT_BCR_OUT][IO_ON] ) break;

			ReqPlateUpDn( nSite, FORWARD );
 			g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[nSite], IO_OFF);
 			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[nSite], IO_ON);//2015.0216

			m_dwTimerFeeder[nSite] = GetCurrentTime();//2014,1219
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UP_CHK;	

		}
		break;

	case ESTEP_FEEDER_UP_CHK:
		{
			if( m_fbplateupdn[nSite] != FB_NONE) break;
			m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(61);
			m_dwTimerFeeder[nSite] = GetCurrentTime();

			if( n_bcr_vision_req[nSite] == CTL_NO)
			{
				n_bcr_vision_req[nSite] = CTL_REQ;
			}

		}
		break;
		
	case 61:
		{
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < 0 ) m_dwTimerFeeder[nSite] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < (st_time.n_wait_time[E_WAIT_BCR_PLATE_VACUUM][IO_ON]) ) break;
// 			if( st_basic.n_mode_device == 0 || g_ioMgr.get_in_bit( st_io.i_distribute_plate_vacuum_chk[nSite], IO_ON) == IO_ON ||
// 				g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[nSite], IO_ON) == IO_ON)
			//2015.0301
			if( st_basic.n_mode_device == 0 || g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[nSite], IO_ON) == IO_ON)
			{
				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(62);
				m_dwTimerFeeder[nSite] = GetCurrentTime();
			}
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] > 800/*st_time.n_limit_time[E_WAIT_BCR_PLATE_VACUUM]*/)
			{
				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(62);
				m_dwTimerFeeder[nSite] = GetCurrentTime();
				//2014.0102 vacuum 3over use -> error
// 				CString strError;
// 				strError.Format("5%04d%d", st_io.i_distribute_plate_vacuum_chk[nSite], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence(757, dWARNING, strError);
// 				m_dwTimerFeeder[nSite] = GetCurrentTime();
				
			}
		}
		break;
		
	case 62:
		{
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < 0 ) m_dwTimerFeeder[nSite] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] < (st_time.n_wait_time[E_WAIT_BCR_PLATE_VACUUM][IO_ON]) ) break;
// 			if( st_basic.n_mode_device == 0 || g_ioMgr.get_in_bit( st_io.i_distribute_plate_vacuum_chk[nSite], IO_ON) == IO_ON ||
// 				g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[nSite], IO_ON) == IO_ON)
			//2015.0301
			if( st_basic.n_mode_device == 0 || g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[nSite], IO_ON) == IO_ON)
			{
				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(63);
			}
			if( GetCurrentTime() - m_dwTimerFeeder[nSite] > 1000/*st_time.n_limit_time[E_WAIT_BCR_PLATE_VACUUM]*/)
			{
				//2015.0327 barcode empty
// 				CString strError;
// 				strError.Format("5%04d%d", st_io.i_bcr_label_chk[nSite], IO_OFF);
// 				CTL_Lib.Alarm_Error_Occurrence(758, dWARNING, strError);
// 				m_dwTimerFeeder[nSite] = GetCurrentTime();
				//2015.0301
				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(63);
				
			}
		}
		break;
		
		
	case 63:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_VISION], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(65);
				st_handler.m_lBcrOutTime[nSite] = GetCurrentTime();
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
				CTL_Lib.Alarm_Error_Occurrence(1210, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case 65:
// 		if( n_bcr_vision_req[nSite] == CTL_NO)
		{
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_VISPOS;		
// 			n_bcr_vision_req[nSite] = CTL_REQ;

			double dpos = COMI.Get_MotCurrentPos(nMotorNum) ;
			if( dpos > st_motor[nMotorNum].d_pos[P_BCR_FEEDER_VISION] - COMI.md_allow_value[nMotorNum] &&
				dpos < st_motor[nMotorNum].d_pos[P_BCR_FEEDER_VISION] + COMI.md_allow_value[nMotorNum])
			{
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_VISION;
			}
			else
			{
				m_step_bcrfeeder[nSite] = ENUM_STEP_FEEDER(63);
			}			
		}
		break;

	case ESTEP_FEEDER_VISPOS:
		{
			int nRet;
			nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_VISION], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_VISION;
			}
			else if( (nRet == BD_ERROR || nRet == BD_SAFETY) )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
				CTL_Lib.Alarm_Error_Occurrence(1210, CTL_dWARNING, strJamCode );
			}
		}
		break;	

	case ESTEP_FEEDER_UNLOAD_VISION:
		{
// 			if(st_var.n_use_vis_density != CTL_NO) break;
// 			st_var.n_use_vis_density = CTL_YES;

			if( n_bcr_vision_req[nSite] == CTL_READY/* && st_var.n_use_vis_density == CTL_NO*/)
			{
				if( COMI.Get_MotCurrentPos( M_BCR_VISION_X ) > st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1+nSite] - COMI.md_allow_value[M_BCR_VISION_X] &&
					COMI.Get_MotCurrentPos( M_BCR_VISION_X ) < st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1+nSite] + COMI.md_allow_value[M_BCR_VISION_X])
				{
					mn_vision_err_retry[nSite] = 0;
					if(st_basic.nVisionUse[1]) m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
					else m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP;
				}
				else
				{
					mn_vision_err_retry[nSite] = 0;
					(ENUM_STEP_FEEDER)101;
				}
// 				st_var.n_use_vis_density = CTL_YES;
// 				if (st_handler.cwnd_list != NULL)
// 				{ 
// 					sprintf(st_msg.c_normal_msg, "[1.Barcode Vis] Site=%d On", nSite +1);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 				}

			}
		}
		break;

	case 101:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1+nSite], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD )
			{
				if(st_basic.nVisionUse[1]) m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
				else m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(1224, CTL_dWARNING, strJamCode );
			}
		}
		break;

		//vision
	case ESTEP_COMM_CONNECT:
		if(st_basic.n_mode_device == 0)
		{
			Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP;
		}
		else
		{
			if (st_client[CLS_VIS_BARCODE].n_connect == CTL_YES)
			{
				if(nSite < 2)
				{
					m_strBcrcode[nSite].Format("[BCR_%d]%s", nSite+1, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[nSite]);
					Func.On_LogFile_Add(99, m_strBcrcode[nSite]);
				}
				else
				{
					m_strBcrcode[nSite].Format("[BCR_%d]%s", nSite+1, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[nSite-2]);
					Func.On_LogFile_Add(99, m_strBcrcode[nSite]);
				}
				Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin = NVR_NONE;
				g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_ON);

				m_dwTimeNetwork[nSite][0] = GetTickCount();
				m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP;
			}
			else
			{
				m_dwTimeNetwork[nSite][0] = GetTickCount();

				//::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1, CLIENT_CONNECT, CLS_VIS_BARCODE);

				if(st_basic.nVisionUse[1])
				{
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CONNECT, CLS_VIS_BARCODE);
					m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT_CHK;
				}
				else
				{
					m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP;
				}

			}
		}
		break;

	case ESTEP_COMM_CONNECT_CHK:
		if (st_client[CLS_VIS_BARCODE].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP;
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
				m_nRetryCnt[3][nSite]++;
				if (m_nRetryCnt[3][nSite] >= 3)
				{
					CString strJamCode;
					m_nRetryCnt[3][nSite] = 0;

					//700200 1 A "Barcode1 time out from server."
					strJamCode.Format( "7002%d0", nSite);
					CTL_Lib.Alarm_Error_Occurrence(195, CTL_dWARNING, strJamCode );

					m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();

					m_step_bcrfeeder[nSite] = ESTEP_CCOMM_CONNECT_WAIT;
				}
			}
		}
		break;

	case ESTEP_CCOMM_CONNECT_WAIT:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeNetwork[nSite][2] > 1000)
		{
			m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
		}
		break;

	case ESTEP_COMM_INSP:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}		
		if (m_dwTimeNetwork[nSite][2]  < 100/*st_time.n_wait_time[25][IO_ON]*/) break;//trigger 타임
//2015.0412		
// 		g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);
	
		if( st_basic.n_mode_device == 0 || st_basic.nVisionUse[1] == false)
		{
			Func.m_pVsClient[EVP_BARCODE]->SetResultBcrTest(NVR_PASS);
			Func.m_pVsClient[EVP_BARCODE]->SetResultSerial(m_site_Serial[nSite]);
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_VISCHK;
			break;
		}

		
// 		Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin = NVR_NONE;
// 		Func.m_pVsClient[EVP_BARCODE]->OnVisionBcrResult(11);//2014.1118 7->11

		m_dwTimeNetwork[nSite][0] = GetTickCount();

		m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP_COM;
		break;

	case ESTEP_COMM_INSP_COM:
		if (Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin == NVR_PASS)
		{
			//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, CLS_VIS_BARCODE);
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_VISCHK;
			//2015.0412		
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);
		}
		else if (Func.m_pVsClient[EVP_BARCODE]->m_nBcrResultBin == NVR_FAIL)
		{
			//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, CLS_VIS_BARCODE);
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_VISCHK;
			//2015.0407
			mn_vision_err_retry[nSite]++;
			if(mn_vision_err_retry[nSite] < 2)
			{
				m_dwTimeNetwork[nSite][0] = GetTickCount();
				
				m_step_bcrfeeder[nSite] = ESTEP_CCOMM_CONNECT_WAIT;
// 				m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
			}			
			//2015.0412		
			g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);
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
			
			if (m_dwTimeNetwork[nSite][2] > (5*st_time.n_wait_time[25][IO_ON]))
			{
				m_nRetryCnt[3][nSite]++;
				if (m_nRetryCnt[3][nSite] >= 3)
				{
					CString strJamCode;
					m_nRetryCnt[3][nSite] = 0;
					
					//700200 1 00 "Barcode1_time_out_from_vision."
					//700210 1 00 "Barcode2_time_out_from_vision."
					//700220 1 00 "Barcode3_time_out_from_vision."
					//700230 1 00 "Barcode4_time_out_from_vision."
					strJamCode.Format( "7002%d0", nSite);
					CTL_Lib.Alarm_Error_Occurrence(126, CTL_dWARNING, strJamCode );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, CLS_VIS_BARCODE);
					m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
					//2015.0412		
					g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();
					
					//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_8, CLIENT_CLOSE, 7);
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CLOSE, CLS_VIS_BARCODE);
					m_step_bcrfeeder[nSite] = ESTEP_COMM_INSP_COM_WAIT;
					//2015.0412		
					g_ioMgr.set_out_bit(st_io.o_distribute_vision_trigger_onoff, IO_OFF);
				}
			}
		}
		break;

	case ESTEP_COMM_INSP_COM_WAIT:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}	
		
		if (m_dwTimeNetwork[nSite][2] > 2000)
		{
			m_step_bcrfeeder[nSite] = ESTEP_COMM_CONNECT;
		}
		break;


	case ESTEP_FEEDER_VISCHK:
		{
			if(st_basic.nVisionUse[1] == false || Func.m_pVsClient[EVP_BARCODE]->GetResultBcrTest() == NVR_PASS )//비젼 동작 완료 
			{
				// 시리얼 체크 
				
				if(st_handler.m_nConformVisAfterModelChange[nSite] == CTL_YES)
				{
					SetPrintResult(nSite, EMBS_FAIL);
					SetPrintSerial( nSite, Func.m_pVsClient[EVP_BARCODE]->GetResultSerial());
					st_handler.m_bBcrTimeOutError[nSite] = false;
					st_handler.m_bstart[nSite] = false;
					m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE_AFTER_LOAD;	
					st_handler.m_nConformVisAfterModelChange[nSite] = CTL_NO;
				}
				else if ( st_basic.n_mode_device == 0 || st_basic.nVisionUse[1] == false || 
					( Func.m_pVsClient[1]->GetResultSerial() != "" && st_handler.m_bBcrTimeOutError[nSite] == false && st_handler.m_bstart[nSite] == false) )
				{
					// 발행된 시리얼이 같으면 
					bool bPass = true;					
					if(st_handler.m_strVisModel != "")
					{
						if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetDenLabelCapa() != Func.m_pVsClient[1]->GetResultCapa())
						{
							bPass = false;
						}
						if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetEan() == true)
						{
							if(Func.m_pVsClient[1]->m_strEan == "" )
							{
								bPass = false;
							}
						}
						if(g_lotMgr.GetLotByLotID(st_handler.m_strVisModel).GetUpc() == true)
						{
							if(Func.m_pVsClient[1]->m_strUpc == "" )
							{
								bPass = false;
							}
						}
					}
					if(bPass == true)
					{
						SetPrintResult(nSite, EMBS_PASS);
						SetPrintSerial( nSite, Func.m_pVsClient[1]->GetResultSerial());
						m_continus_vis_error[nSite] = 0;
						m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD;
						st_var.n_use_barcode[nSite] = EMBS_PASS;
					}
					else
					{
						SetPrintResult(nSite, EMBS_FAIL);
						SetPrintSerial( nSite, Func.m_pVsClient[EVP_BARCODE]->GetResultSerial());
						//2015.0329
// 						Func.m_pSite[SITE_BARCODE1+nSite]->GetModule(nSite).GetBcrLabel().SetBcrLabelState(EMBS_FAIL);
						m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE;
					}
				}
				else
				{
					SetPrintResult(nSite, EMBS_FAIL);
					SetPrintSerial( nSite, Func.m_pVsClient[EVP_BARCODE]->GetResultSerial());
					//2015.0329
// 					Func.m_pSite[SITE_BARCODE1+nSite]->GetModule(nSite).GetBcrLabel().SetBcrLabelState(EMBS_FAIL);
					st_handler.m_bBcrTimeOutError[nSite] = false;
					st_handler.m_bstart[nSite] = false;
					m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE;
				}
				n_bcr_vision_req[nSite] = CTL_NO;
// 				st_var.n_use_vis_density = CTL_NO;
// 				if (st_handler.cwnd_list != NULL)
// 				{ 
// 					sprintf(st_msg.c_normal_msg, "[2.Barcode Vis] Site=%d Off", nSite +1);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 				}

			}
			else if( Func.m_pVsClient[EVP_BARCODE]->GetResultBcrTest() == NVR_FAIL || st_handler.m_bBcrTimeOutError[nSite] == true)
			{
				SetPrintResult(nSite, EMBS_FAIL);
				SetPrintSerial( nSite, Func.m_pVsClient[EVP_BARCODE]->GetResultSerial());
				//2015.0329
// 				Func.m_pSite[SITE_BARCODE1+nSite]->GetModule(nSite).GetBcrLabel().SetBcrLabelState(EMBS_FAIL);

				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE;
				n_bcr_vision_req[nSite] = CTL_NO;
// 				st_var.n_use_vis_density = CTL_NO;
// 				if (st_handler.cwnd_list != NULL)
// 				{ 
// 					sprintf(st_msg.c_normal_msg, "[3.Barcode Vis] Site=%d Off", nSite +1);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 				}
			}
			else//타임 아웃 
			{
			}
		}
		break;

		//2015.0726
	case ESTEP_FEEDER_UNLOAD_WASTE_AFTER_LOAD:
		{
// 			910100 0 A "Model_change_Barcode1_Vision_TOP_DARKNESS_CHECK."
// 			910101 0 A "Model_change_Barcode2_Vision_TOP_DARKNESS_CHECK."
// 			910102 0 A "Model_change_Barcode3_Vision_TOP_DARKNESS_CHECK."
// 			910103 0 A "Model_change_Barcode4_Vision_TOP_DARKNESS_CHECK."
			sprintf(mc_jamcode, "91010%d", nSite);
			CTL_Lib.Alarm_Error_Occurrence(3994, CTL_dWARNING, mc_jamcode );
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE;					
			break;
		}

	case ESTEP_FEEDER_UNLOAD_WASTE:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_WASTE], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE_PICKUP;
				m_continus_vis_error[nSite]++;
				m_total_vis_error[0]++;
				if( st_var.n_bcrid_error[nSite] == CTL_YES)
				{
					//2015.0327
					m_continus_vis_error[nSite] = 0;
// 					640000 1 00 "zebra_Printer1_Label_Empty_Error."
// 					640001 1 00 "zebra_Printer2_Label_Empty_Error."
// 					640002 1 00 "zebra_Printer3_Label_Empty_Error."
// 					640003 1 00 "zebra_Printer4_Label_Empty_Error."
					int aSite = -1;
					st_var.n_bcrid_error[nSite] = CTL_NO;
					if( nSite < 2 )
					{
						if(nSite >= 2) aSite = nSite-2;
						else           aSite = nSite+2;
						if(st_var.n_use_barcode[aSite] == EMBS_SKIP)
						{
							sprintf(mc_jamcode, "64000%d", nSite);
							CTL_Lib.Alarm_Error_Occurrence(1194, CTL_dWARNING, mc_jamcode );
						}
						else
						{
							if( ((g_ioMgr.get_out_bit( st_io.o_distribute_p_lock[2], IO_OFF ) == IO_ON ||
								g_ioMgr.get_in_bit( st_io.i_distribute_lock_chk[2], IO_ON) == IO_OFF ||
								g_ioMgr.get_in_bit( st_io.i_distribute_unlock_chk[2], IO_OFF) == IO_ON) && st_basic.nBcrFeederUse[2] == CTL_YES) ||
								((g_ioMgr.get_out_bit( st_io.o_distribute_p_lock[3], IO_OFF ) == IO_ON ||
								g_ioMgr.get_in_bit( st_io.i_distribute_lock_chk[3], IO_ON) == IO_OFF ||
								g_ioMgr.get_in_bit( st_io.i_distribute_unlock_chk[3], IO_OFF) == IO_ON) && st_basic.nBcrFeederUse[3] == CTL_YES)
								)
							{
								if (st_handler.cwnd_list != NULL)
								{ 
									sprintf(st_msg.c_abnormal_msg, "[Barcode #%d Door open] Door close please", nSite +1);
									st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
								}
								sprintf(mc_jamcode, "64000%d", nSite);
								CTL_Lib.Alarm_Error_Occurrence(1198, CTL_dWARNING, mc_jamcode );
							}
							else
							{
								st_msg.mstr_event_msg->Format( "[BARCODE #%d] is not response.", nSite+1 );
								::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
								st_var.n_use_barcode[nSite] = EMBS_SKIP;
							}
						}
					}
					else//2,3번은 고치고 난뒤 lot이 바뀔때 1,2번으로 옮긴다.
					{
						sprintf(mc_jamcode, "64000%d", nSite);
						CTL_Lib.Alarm_Error_Occurrence(1195, CTL_dWARNING, mc_jamcode );
					}
				}
				else if(m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt)
// 				if(m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt)
				{
					m_continus_vis_error[nSite] = 0;
					sprintf(mc_jamcode, "91000%d", nSite);
					CTL_Lib.Alarm_Error_Occurrence(1196, CTL_dWARNING, mc_jamcode );
				}
				else if(m_total_vis_error[0] > st_basic.mn_Label_Print_Total_Error_Cnt)
				{
					m_total_vis_error[0] = 0;
					sprintf(mc_jamcode, "91001%d", nSite);
					CTL_Lib.Alarm_Error_Occurrence(1197, CTL_dWARNING, mc_jamcode );

				}
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(196, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case ESTEP_FEEDER_UNLOAD:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_LOAD], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_FINISH;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(124, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case ESTEP_FEEDER_UNLOAD_WASTE_PICKUP:
		{	
			m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE_FINISH;
// 			int nRet = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_BCR_FEEDER_1+nSite].d_pos[P_BCR_FEEDER_WASTE], st_basic.nRunSpeed);
// 			if( nRet == BD_GOOD )
// 			{
// 				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_UNLOAD_WASTE_FINISH;
// 			}
// 			else if( nRet == BD_ERROR )
// 			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
// 				CString strJamCode;
// 				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1+nSite);
// 				CTL_Lib.Alarm_Error_Occurrence(125, CTL_dWARNING, strJamCode );
// 			}
//			if( g_feeder_robot.GetJobCount() > 0 || g_feeder_robot.IsPicking() != false )
//				break;		
		}
		break;

	case ESTEP_FEEDER_UNLOAD_WASTE_FINISH:
		m_bPrint_Pickup[nSite] = true;//Barcode out start
		m_fbFeeder[nSite] =  EPRI_NONE;
		m_step_bcrfeeder[nSite] = ESTEP_FEEDER_NONE;
		break;
		
	case ESTEP_FEEDER_FINISH:
		{
// 			int nRet1 = COMI.Start_SingleMove(M_BCR_VISION_X, st_motor[M_BCR_VISION_X].d_pos[P_BARCODE_F1], st_basic.nRunSpeed);
// 			if( nRet1 == BD_GOOD )
// 			{
				m_nRetryCnt[3][nSite] = 0;
				m_bPrint_Pickup[nSite] = true;//Barcode out start
				m_fbFeeder[nSite] = EPRI_NONE;
				m_step_bcrfeeder[nSite] = ESTEP_FEEDER_NONE;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
// 			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
// 				CString strJamCode;
// 				strJamCode.Format( "%02d0006", M_BCR_FEEDER_1);
// 				CTL_Lib.Alarm_Error_Occurrence(126, CTL_dWARNING, strJamCode );
// 			}
		}
		break;

	}
}


void CRun_Barcode_Feeders::ControlRejectRobot()
{
	if( g_feeder_robot.GetJobCount() > 0)
		return;

	// 잡고 있는게 없을 때
//	if( g_feeder_robot.IsPicking() == false )
//	{
		for ( int i = 0; i < 4; i++)
		{
			if(st_bcr_feeder[i].st_bcr.bExist == true &&
				st_bcr_feeder[i].st_bcr.nState == EMBS_FAIL)
			{
				g_feeder_robot.AddJob(i, MPOS_INDEX_REJECT);
				break;
			}
		}		
//	}
}

bool CRun_Barcode_Feeders::Run_Rej_Move_Chk_Jam( stRejectPicker job )
{
	CString strJamCode;

	Func.ThreadFunctionStepTrace(16, job.rej_picker_step);
//	switch( job.rej_picker_step )
//	{
//	}

	return true;
}

bool CRun_Barcode_Feeders::Run_Rej_Move_Chk_Time()
{
	switch( rej_picker_step_old )
	{
	case PSTEP_REJECT_PICK_UP_CHK:
 		if( InTimeRobot( st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN] ) == false )
 		{//522080 1 00 "Reject Label Robot Up Check Off Error."
			CTL_Lib.Alarm_Error_Occurrence( 130, dWARNING, "522080" );
			return false;
 		}
		break;
	case PSTEP_REJECT_PICK_START:		
		if( InTimeRobot( 10000 ) == false )
 		{//160006 1 00 "Distribute Label Reject X Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 131, dWARNING, "160006" );
			return false;
 		}
		break;

	case PSTEP_REJECT_PICK_DOWN_CHK:
		if( InTimeRobot( 10000 ) == false )
 		{//522090 1 00 "Reject Label Robot Down Check Off Error."
			CTL_Lib.Alarm_Error_Occurrence( 132, dWARNING, "522090" );
			return false;
 		}
		break;
	}


	return true;
}

bool CRun_Barcode_Feeders::InTimeRobot( int iMilliSec )
{
	DWORD dwtime = GetCurrentTime();
	if( GetCurrentTime() - m_dwTimeReject < 0 )
	{
		m_dwTime_Robot = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwTimeReject < iMilliSec )
		return true;

	return false;
}

void CRun_Barcode_Feeders::ReqRejVacDown( EFB efb )
{
	m_fbRejVacDown = efb;
	m_step_Rejvac_down = ESTEP_CYL_START;
}

void CRun_Barcode_Feeders::Run_Move_Rej_Vac_Down()
{
	if( m_fbRejVacDown == FB_NONE )	return;
	int iOnOff = IO_ON;
	if( m_fbRejVacDown == BACKWARD )
		iOnOff = IO_OFF;

	Func.ThreadFunctionStepTrace(17, m_step_Rejvac_down);
	switch( m_step_Rejvac_down )
	{
	case ESTEP_CYL_START:	m_step_Rejvac_down = ESTEP_CYL_MOVE; break;
	case ESTEP_CYL_MOVE:	
		{
			int iDown = st_io.o_reject_rbt_updown;
			g_ioMgr.set_out_bit( iDown, iOnOff );
			//2013,0202
			if(COMI.mn_run_status == dRUN) st_work.mn_rej_picker_updn = iOnOff;
			m_step_Rejvac_down = ESTEP_CYL_CHK;
			m_dwTimeRejVacDownStart = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			int iUpChk = st_io.i_reject_rbt_up_chk;
			int iDnChk = st_io.i_reject_rbt_down_chk;

			if( st_handler.mn_virtual_mode == 1 || ( g_ioMgr.get_in_bit( iDnChk ) == iOnOff &&	g_ioMgr.get_in_bit( iUpChk ) != iOnOff ) )
			{
				m_step_Rejvac_down = ESTEP_CYL_FINISH;
				m_dwTimeRejVacDown = GetCurrentTime();
				break;
			}
			if( GetCurrentTime() - m_dwTimeRejVacDownStart > st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode;
					strJamCode.Format("5%04d%d",st_io.i_reject_rbt_up_chk,iOnOff);
					CTL_Lib.Alarm_Error_Occurrence( 618, dWARNING, (LPCSTR)strJamCode );
				}
			}//2012,1220
			else if( GetCurrentTime() - m_dwTimeRejVacDownStart < 0 )
			{
				m_dwTimeRejVacDownStart = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{//2012,1220
			if( GetCurrentTime() - m_dwTimeRejVacDown < 0 )
			{
				m_dwTimeRejVacDown = GetCurrentTime();
			}
			if( GetCurrentTime() - m_dwTimeRejVacDown < st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][iOnOff] )
				return;

			m_fbRejVacDown = FB_NONE;
			m_step_Rejvac_down = ESTEP_CYL_START;
		}
		break;
	}
}


void CRun_Barcode_Feeders::Run_Move_Reject()
{
	//////////////////////////////////////////////////////////////////////////
	int nJobCnt = g_feeder_robot.GetJobCount();
	if( nJobCnt == 0 )
		return;

	stRejectPicker& job = g_feeder_robot.GetFirstJob();

//  	g_feeder_robot.EraseFirstJob();


	if( rej_picker_step_old != job.rej_picker_step )
	{
		rej_picker_step_old = job.rej_picker_step;
		m_dwTimeReject = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
		if( Run_Rej_Move_Chk_Jam( job ) == false )
			return;
	}
	
	int nMotorNum = 0;
	if(job.iRobotFirst == 0) nMotorNum = M_BCR_FEEDER_1;
	else if(job.iRobotFirst == 1) nMotorNum = M_BCR_FEEDER_2;
	else if(job.iRobotFirst == 2) nMotorNum = M_BCR_FEEDER_3;
	else if(job.iRobotFirst == 3) nMotorNum = M_BCR_FEEDER_4;

// 	if( Run_Rej_Move_Chk_Time() == false )
// 		return;

	Func.ThreadFunctionStepTrace(18, job.rej_picker_step);
	
	switch( job.rej_picker_step )
	{
	case PSTEP_REJECT_NONE: break;

	case PSTEP_REJECT_PICK_BEFORE:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_WASTE], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_READY;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(140, CTL_dWARNING, strJamCode );
			}
			if( g_feeder_robot.GetJobCount() > 0 || g_feeder_robot.IsPicking() != false )
				break;		
		}
		break;
		
	case PSTEP_REJECT_PICK_READY:
		{
			double dPos = 0;
			dPos = COMI.Get_MotCurrentPos( nMotorNum );
			if( dPos > st_motor[nMotorNum].d_pos[P_BCR_FEEDER_WASTE] + COMI.md_allow_value[nMotorNum] ||
				dPos < st_motor[nMotorNum].d_pos[P_BCR_FEEDER_WASTE] - COMI.md_allow_value[nMotorNum])
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_BEFORE;
				break;
			}


			if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
				g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_START;
			}
			else
			{
				SetRetryCnt( ERC_REJECT, IO_ON, 0);
				job.rej_picker_step = PSTEP_REJECT_PICK_READY_UP;
			}
		}
		break;

	case PSTEP_REJECT_PICK_READY_UP:
		{
			/*g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);*/
			ReqRejVacDown( BACKWARD );
			job.rej_picker_step = PSTEP_REJECT_PICK_READY_UP_CHK;			
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PICK_READY_UP_CHK:
		if( m_fbRejVacDown != FB_NONE ) break;
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_ON) == IO_OFF )
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN]) break;
			job.rej_picker_step = PSTEP_REJECT_PICK_START;

			mn_Mot_Rej[0] = mn_Mot_Rej[1] = CTL_NO;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject > st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN])
			{
				AddRetryCnt(ERC_REJECT, IO_ON);
				if( m_nReteyCnt[ERC_REJECT][IO_ON] >= st_basic.n_retry_cnt) 
				{
					m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
					CString strError;
					strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
					CTL_Lib.Alarm_Error_Occurrence( 978, dWARNING, strError );
					job.rej_picker_step = PSTEP_REJECT_PICK_READY_UP;		
				}
				else
				{
					job.rej_picker_step = PSTEP_REJECT_PICK_READY_UP;		
				}
			}			
		}
		break;

	case PSTEP_REJECT_PICK_START:
		{
			int nRet1 = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F1+job.iRobotFirst], st_basic.nRunSpeed);
			int nRet2 = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_WASTE], st_basic.nRunSpeed);
			if( nRet1 == BD_GOOD ) mn_Mot_Rej[0] = CTL_YES;
			if( nRet2 == BD_GOOD ) mn_Mot_Rej[1] = CTL_YES;

			if( mn_Mot_Rej[0] == CTL_YES && mn_Mot_Rej[1] == CTL_YES)
			{
				//job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;
				job.rej_picker_step = PICKER_STEP_REJECT(41);//2015,0212 자재 유무확인
				mn_Mot_Rej[0] = mn_Mot_Rej[1] = CTL_NO;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{//160006 1 00 "Distribute Label Reject X Axis Motor Move Error."
				if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence(141, CTL_dWARNING, "160006" );
				else
				{
					CString strError;
					strError.Format("%02d0006",nMotorNum);
					CTL_Lib.Alarm_Error_Occurrence(141, dWARNING, strError );
				}
			}
		}
		break;

	case 41:
		{
			m_nReject_pick = CTL_NO;
			if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON) == IO_ON)
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;
			}
			else
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_UP_FINISH;//없다고 제거
				//2015.0903
				job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;
				m_nReject_pick = CTL_YES;
			}
		}
		break;

	case PSTEP_REJECT_RETRY_PICK_UP:
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
		job.rej_picker_step = PSTEP_REJECT_RETRY_PICK_UP_CHK;	
		m_dwTimeReject = GetCurrentTime();
		break;

	case PSTEP_REJECT_RETRY_PICK_UP_CHK:
		if( GetCurrentTime() - m_dwTimeReject <= 0 ) m_dwTimeReject = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_OFF]) break;
		SetRetryCnt( ERC_REJECT, IO_ON, 0);
		job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;
		break;

	case PSTEP_REJECT_PICK_DOWN:
		{
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_ON);
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst], IO_ON);
			ReqRejVacDown(FORWARD);
			job.rej_picker_step = PSTEP_REJECT_PICK_DOWN_CHK;	
			m_dwTimeReject = GetCurrentTime();
		}
		break;
		
	case PSTEP_REJECT_PICK_DOWN_CHK:
		if( m_fbRejVacDown != FB_NONE ) break;
// 		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
// 			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_OFF) == IO_OFF )
		if( (g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_OFF) ||  
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_OFF)
		{
			job.rej_picker_step = PSTEP_REJECT_PICK_VACUUM;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN] ) break;
			AddRetryCnt(ERC_REJECT, IO_ON);
			if( m_nReteyCnt[ERC_REJECT][IO_ON] >= st_basic.n_retry_cnt) 
			{
				 m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
				CString strError;
				strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 975, dWARNING, strError );
				//m_dwTimeReject = PSTEP_REJECT_RETRY_PICK_UP;
				job.rej_picker_step = PSTEP_REJECT_RETRY_PICK_UP;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_RETRY_PICK_UP;
				job.rej_picker_step = PSTEP_REJECT_RETRY_PICK_UP;//2014,1219
			}
		}
		break;
		
	case PSTEP_REJECT_PICK_VACUUM:
		g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_ON);
		g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[job.iRobotFirst], IO_OFF);
		g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_ON);
		job.rej_picker_step = PSTEP_REJECT_PICK_VACUUM_CHK;
		m_dwTimeReject = GetCurrentTime();
		break;

	case PSTEP_REJECT_PICK_VACUUM_CHK:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_ON) == IO_ON || m_nReject_pick == CTL_YES)
		{
			m_dwTimeReject = GetCurrentTime();
			job.rej_picker_step = PSTEP_REJECT_PICK_UP;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[28]) break;
			AddRetryCnt(ERC_REJECT, IO_ON);
			if( m_nReteyCnt[ERC_REJECT][IO_ON] >= st_basic.n_retry_cnt) 
			{
				 m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
				CString strError;
				strError.Format("5%04d%d",st_io.i_reject_rbt_vacuum_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 976, dWARNING, strError );
				g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
				job.rej_picker_step = PICKER_STEP_REJECT(101);//2015.0211
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PICK_VACUUM;	
				//g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);//2015.0211
				job.rej_picker_step = PSTEP_REJECT_PICK_VACUUM;//2014,1219
				//2015.0325
				g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
				job.rej_picker_step = PICKER_STEP_REJECT(101);
			}
		}
		break;

	//2015.0211
	case 101:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
			job.rej_picker_step = PICKER_STEP_REJECT(102);
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN]) break;
			AddRetryCnt(ERC_REJECT, IO_OFF);
			if( m_nReteyCnt[ERC_REJECT][IO_OFF] >= st_basic.n_retry_cnt) 
			{
				m_nReteyCnt[ERC_REJECT][IO_OFF] = 0;
				CString strError;
				strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 977, dWARNING, strError );
				g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
				job.rej_picker_step = PICKER_STEP_REJECT(101);
			}
			else
			{
				g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
				job.rej_picker_step = PICKER_STEP_REJECT(101);
			}
		}
		break;


	case 102:
		if( g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_ON) == IO_ON )
		{
			job.rej_picker_step = PSTEP_REJECT_PICK_UP_FINISH;
		}
		else
		{
			if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON) == IO_ON)
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;
			}
			else
			{
				job.rej_picker_step = PSTEP_REJECT_PICK_UP_FINISH;//없다고 제거
			}
		}
		break;

	case PSTEP_REJECT_PICK_UP:
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_ON]) break;
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
			job.rej_picker_step = PSTEP_REJECT_PICK_UP_CHK;			
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PICK_UP_CHK:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
			job.rej_picker_step = PSTEP_REJECT_PICK_UP_LABEL_CHK;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN]) break;
			AddRetryCnt(ERC_REJECT, IO_OFF);
			if( m_nReteyCnt[ERC_REJECT][IO_OFF] >= st_basic.n_retry_cnt) 
			{
				 m_nReteyCnt[ERC_REJECT][IO_OFF] = 0;
				CString strError;
				strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 977, dWARNING, strError );
				//m_dwTimeReject = PSTEP_REJECT_PICK_UP;
				job.rej_picker_step = PSTEP_REJECT_PICK_UP;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PICK_UP;		
				job.rej_picker_step = PSTEP_REJECT_PICK_UP;//2014,1219
			}
		}
		break;

	case PSTEP_REJECT_PICK_UP_LABEL_CHK:
		if(  g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_ON) == IO_ON || m_nReject_pick == CTL_YES )
		{
			m_nReject_pick = CTL_NO;
			job.rej_picker_step = PSTEP_REJECT_PICK_UP_FINISH;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < 2000/*st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN]*/) break;
			AddRetryCnt(ERC_REJECT, IO_ON);
			if( m_nReteyCnt[ERC_REJECT][IO_ON] >= st_basic.n_retry_cnt) 
			{
				 m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
				CString strError;
				strError.Format("5%04d%d",st_io.i_reject_rbt_vacuum_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 974, dWARNING, strError );
				//m_dwTimeReject = PSTEP_REJECT_PICK_VACUUM;
				job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PICK_VACUUM;	
				job.rej_picker_step = PSTEP_REJECT_PICK_DOWN;//2014,1219
			}
	
		}
		break;

	case PSTEP_REJECT_PICK_UP_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_ON] ) break;
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[job.iRobotFirst], IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);

//			ABcrLabel* pBcrldl;
//			if( job.GetPosByTarget() == MPOS_INDEX_REJECT )
//			{
//				pBcrldl = &Func.m_pSite[SITE_BARCODE1 + (job.iRobotFirst/2)]->GetBcrLabel( job.iRobotFirst%2 );
//				
//			}
//
//			if( job.iRobotFirst != -1 ) {
//				g_feeder_robot.GetPicker().SetBcrLabel( *pBcrldl );
//				pBcrldl->EraseData(); 
//			}
			st_rej_rbt.bPick[job.iRobotFirst] = st_bcr_feeder[job.iRobotFirst].st_bcr.bOut;
			st_bcr_feeder[job.iRobotFirst].st_bcr.bExist = false;
			memcpy( &st_rej_rbt.st_bcr, &st_bcr_feeder[job.iRobotFirst].st_bcr, sizeof(st_bcr_feeder[job.iRobotFirst].st_bcr));
			//memset( &st_bcr_feeder[job.iRobotFirst], 0x00, sizeof(st_bcr_feeder[job.iRobotFirst]));

			st_handler.mn_curr_unload_conv = job.iRobotFirst;

			job.rej_picker_step = PSTEP_REJECT_PLACE_READY;
		}
		break;

	case PSTEP_REJECT_PLACE_BEFORE:
		{	
			int nRet = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_BARCODE_UNLOAD], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				job.rej_picker_step = PSTEP_REJECT_PLACE_READY;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_L_REJECT_X);
				CTL_Lib.Alarm_Error_Occurrence(142, CTL_dWARNING, strJamCode );
			}
		}
		break;

	case PSTEP_REJECT_PLACE_READY:
		{
			int nRet = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_BARCODE_UNLOAD], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
				job.rej_picker_step = PSTEP_REJECT_PLACE_READY_DN;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", M_L_REJECT_X);
				CTL_Lib.Alarm_Error_Occurrence(142, CTL_dWARNING, strJamCode );
			}

		}
		break;

	case PSTEP_REJECT_PLACE_READY_UP:
		{
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
			job.rej_picker_step = PSTEP_REJECT_PLACE_READY_UP_CHK;			
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PLACE_READY_UP_CHK:
		if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeReject < 2000) break;
		job.rej_picker_step = PSTEP_REJECT_PLACE_READY_DN;	
		break;

	case PSTEP_REJECT_PLACE_READY_DN:
		{
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_ON);//2014,1219 IO_OFF -> IO_ON
//			job.rej_picker_step = PSTEP_REJECT_PLACE_READY_UP_CHK;			
			job.rej_picker_step = PSTEP_REJECT_PLACE_READY_DN_CHK;//2014,1219			
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PLACE_READY_DN_CHK:
// 		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
// 			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_OFF )//2014,1219
		if( (g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_OFF) ||  
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_OFF)
		{
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_eject, IO_ON);
			job.rej_picker_step = PSTEP_REJECT_PLACE_START;
			m_dwTimeReject = GetCurrentTime();
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_REJECT_UPDOWN]) break;
			AddRetryCnt(ERC_REJECT, IO_ON);
			if(  m_nReteyCnt[ERC_REJECT][IO_ON] >= st_basic.n_retry_cnt) 
			{
				m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
				CString strError;
				strError.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 3243, dWARNING, (LPCTSTR)strError);
				//m_dwTimeReject = PSTEP_REJECT_PLACE_READY_UP;
				job.rej_picker_step = PSTEP_REJECT_PLACE_READY_UP;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PLACE_READY_UP;		
				job.rej_picker_step = PSTEP_REJECT_PLACE_READY_UP;//2014,1219
			}
		}
		break;

	case PSTEP_REJECT_PLACE_START:
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < (st_time.n_wait_time[E_WAIT_BCR_REJECT_UPDOWN][IO_ON]*2) ) break;

			//m_dwTimeReject = PSTEP_REJECT_PLACE_DOWN;
			job.rej_picker_step = PSTEP_REJECT_PLACE_DOWN;//2014,1219
			break;
			
			int nRet = CTL_Lib.Single_Move(M_L_REJECT_X, st_motor[M_L_REJECT_X].d_pos[P_BARCODE_SAFETY+job.iRobotFirst], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				job.rej_picker_step = PSTEP_REJECT_PLACE_DOWN;
			}
			else if( nRet == BD_ERROR )
			{//160006 1 00 "Distribute Label Reject X Axis Motor Move Error."
				CTL_Lib.Alarm_Error_Occurrence(143, CTL_dWARNING, "160006" );
			}
		}
		break;

	case PSTEP_REJECT_PLACE_DOWN:
		{
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_ON);
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_eject, IO_OFF);
			SetRetryCnt( ERC_REJECT, IO_ON, 0);
			job.rej_picker_step = PSTEP_REJECT_PLACE_DOWN_CHK;	
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PLACE_DOWN_CHK:
		if( ( g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_ON) == IO_ON && g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_OFF) == IO_OFF) ||
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_OFF) == IO_OFF)
		{
//			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
//			if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_ON]) break;
			m_dwTimeReject = GetCurrentTime();
			job.rej_picker_step = PSTEP_REJECT_PLACE_UP;//vacuum off 포함되어 잇다
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_PLATE_UPDOWN]) break;
			AddRetryCnt(ERC_REJECT, IO_ON);
			if( m_nReteyCnt[ERC_REJECT][IO_ON] >= st_basic.n_retry_cnt) 
			{
				m_nReteyCnt[ERC_REJECT][IO_ON] = 0;
				CString strError;
				strError.Format("5%04d%d", st_io.i_reject_rbt_up_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 434, dWARNING, (LPCTSTR)strError );
				//m_dwTimeReject = PSTEP_REJECT_PLACE_DOWN;	
				job.rej_picker_step = PSTEP_REJECT_PLACE_DOWN;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PLACE_DOWN;		
				job.rej_picker_step = PSTEP_REJECT_PLACE_DOWN;//2014,1219
			}
		}
		break;

	case PSTEP_REJECT_PLACE_UP:
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_OFF]) break;
			
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_updown, IO_OFF);
			job.rej_picker_step = PSTEP_REJECT_PLACE_UP_CHK;			
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PLACE_UP_CHK:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_OFF )
		{
//			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
//			if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_OFF]) break;
			m_dwTimeReject = GetCurrentTime();
			job.rej_picker_step = PSTEP_REJECT_PLACE_VACUUM;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_limit_time[E_WAIT_BCR_PLATE_UPDOWN]) break;
			AddRetryCnt(ERC_REJECT, IO_OFF);
			if( m_nReteyCnt[ERC_REJECT][IO_OFF] >= st_basic.n_retry_cnt) 
			{
				m_nReteyCnt[ERC_REJECT][IO_OFF] = 0;
				CString strError;
				strError.Format("5%04d%d", st_io.i_reject_rbt_up_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 434, dWARNING, (LPCTSTR)strError );
				//m_dwTimeReject = PSTEP_REJECT_PLACE_UP;	
				job.rej_picker_step = PSTEP_REJECT_PLACE_UP;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PLACE_UP;
				job.rej_picker_step = PSTEP_REJECT_PLACE_UP;//2014,1219
			}
		}
		break;

	case PSTEP_REJECT_PLACE_VACUUM:
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject < st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_ON]) break;
			g_ioMgr.set_out_bit( st_io.o_reject_rbt_vacuum, IO_OFF);
			job.rej_picker_step = PSTEP_REJECT_PLACE_VACUUM_CHK;			
			m_dwTimeReject = GetCurrentTime();
		}
		break;

	case PSTEP_REJECT_PLACE_VACUUM_CHK:
		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_vacuum_chk, IO_OFF) == IO_OFF )
		{
			job.rej_picker_step = PSTEP_REJECT_PLACE_FINISH;
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeReject < 0 ) m_dwTimeReject = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeReject< st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][IO_OFF]) break;
			AddRetryCnt(ERC_REJECT, IO_OFF);
			if( m_nReteyCnt[ERC_REJECT][IO_OFF] >= st_basic.n_retry_cnt) 
			{
				m_nReteyCnt[ERC_REJECT][IO_OFF] = 0;
				CString strError;
				strError.Format("5%04d%d", st_io.i_reject_rbt_vacuum_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence( 434, dWARNING, (LPCTSTR)strError );
				//m_dwTimeReject = PSTEP_REJECT_PLACE_VACUUM;	
				job.rej_picker_step = PSTEP_REJECT_PLACE_VACUUM;//2014,1219
			}
			else
			{
				//m_dwTimeReject = PSTEP_REJECT_PLACE_VACUUM;		
				job.rej_picker_step = PSTEP_REJECT_PLACE_VACUUM;//2014,1219
			}
		}
		break;

	case PSTEP_REJECT_PLACE_FINISH:
		job.rej_picker_step = PSTEP_REJECT_LABLE_FULL_CHK;
		break;

	case PSTEP_REJECT_LABLE_FULL_CHK:
		if( g_ioMgr.get_in_bit( st_io.i_reject_cv_full_chk, IO_OFF) == IO_ON && 
			g_ioMgr.get_in_bit( st_io.i_reject_cv_full_pos_chk, IO_OFF) == IO_ON )
		{//522130 1 00 "Reject Label Conveyor Full Position Check Error."
			CTL_Lib.Alarm_Error_Occurrence(144, CTL_dWARNING, "522130");
		}
		else if( g_ioMgr.get_in_bit( st_io.i_reject_cv_full_chk, IO_OFF) == IO_ON)
		{
			job.rej_picker_step = PSTEP_REJECT_LABLE_ONESTEP_MOVE_CHK;
		}
		else
		{
// 			job.rej_picker_step = PSTEP_REJECT_NONE;
			g_feeder_robot.EraseFirstJob();
		}
		break;

	case PSTEP_REJECT_LABLE_ONESTEP_MOVE_CHK:
		m_nRejectConvey = CTL_YES;
// 		job.rej_picker_step = PSTEP_REJECT_NONE;
		g_feeder_robot.EraseFirstJob();
		break;
	}
}

void CRun_Barcode_Feeders::OnRejectConveyMove()
{
	switch(mn_RejectStep) {
	case 0:
		if( m_nRejectConvey == CTL_YES)
		{
			g_ioMgr.set_out_bit( st_io.o_reject_cv_motor_onoff, IO_ON);
			m_dwTimeRejectConvey = GetCurrentTime();
			mn_RejectStep = 1000;
		}
		break;

	case 1000:
		if( GetCurrentTime() - m_dwTimeRejectConvey < 0) m_dwTimeRejectConvey = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeRejectConvey < 500 ) break;

		if( GetCurrentTime() - m_dwTimeRejectConvey > 5000)
		{//522990 1 00 "Reject Label Conveyor time out Error."
			if( COMI.mn_run_status == dRUN )
			{
				CTL_Lib.Alarm_Error_Occurrence(150, CTL_dWARNING, "522990");
				m_nRejectConvey = CTL_NO;
				mn_RejectStep = 0;
			}
			g_ioMgr.set_out_bit( st_io.o_reject_cv_motor_onoff, IO_OFF);
		}
		if(g_ioMgr.get_in_bit( st_io.i_reject_cv_full_pos_chk, IO_ON) == IO_ON)
		{
			g_ioMgr.set_out_bit( st_io.o_reject_cv_motor_onoff, IO_OFF);
			CTL_Lib.Alarm_Error_Occurrence(6144, CTL_dWARNING, "522130");
			m_nRejectConvey = CTL_NO;
			mn_RejectStep = 0;
		}

		if( g_ioMgr.get_in_bit(st_io.i_reject_cv_full_chk, IO_ON) == IO_OFF &&
			 g_ioMgr.get_in_bit( st_io.i_reject_cv_1step_pos_chk, IO_ON) == IO_OFF )
		{
			g_ioMgr.set_out_bit( st_io.o_reject_cv_motor_onoff, IO_OFF);
			m_nRejectConvey = CTL_NO;
			mn_RejectStep = 0;
		}
		break;
	}
}

void CRun_Barcode_Feeders::EBFI_Plate_Down()
{
	for ( int i = 0; i < 4; i++)
	{
		ReqPlateUpDn(i, BACKWARD);
	}
	mn_InitStep = EBFI_PLATE_DOWN_CHK;
}

void CRun_Barcode_Feeders::EBFI_Plate_Down_Check()
{
	if( GetPlateUpDn(EBPS_1) != FB_NONE ) return;
	if( GetPlateUpDn(EBPS_2) != FB_NONE ) return;
	if( GetPlateUpDn(EBPS_3) != FB_NONE ) return;
	if( GetPlateUpDn(EBPS_4) != FB_NONE ) return;

	mn_InitStep = EBFI_HOMECHK_FEERDE1;
}

void CRun_Barcode_Feeders::EBFI_HomeCheck_BCR_Feeder( int nAxis )
{

	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		if( nAxis == M_BCR_FEEDER_1)
		{
			mn_InitStep = EBFI_HOMECHK_FEERDE2; 
			Func.OnInitBtn( RBTN_BCR_FEEDER_1 );//2014.1018
		}
		else if( nAxis == M_BCR_FEEDER_2)
		{
			mn_InitStep = EBFI_HOMECHK_FEERDE3; 
			Func.OnInitBtn( RBTN_BCR_FEEDER_2 );//2014.1018
		}
		else if( nAxis == M_BCR_FEEDER_3)
		{
			mn_InitStep = EBFI_HOMECHK_FEERDE4; 
			Func.OnInitBtn( RBTN_BCR_FEEDER_3 );//2014.1018
		}
		else if( nAxis == M_BCR_FEEDER_4)
		{
			mn_InitStep = EBFI_REJECT_PICK_READY_UP; 
			Func.OnInitBtn( RBTN_BCR_FEEDER_4 );//2014.1018
		}
	}
	else if( nRet == BD_ERROR )
	{//060001 0 00 "Distribute Feeder#1 Axis Motor Home Check Error."
		//070001 0 00 "Distribute Feeder#2 Axis Motor Home Check Error."
		//080001 0 00 "Distribute Feeder#3 Axis Motor Home Check Error."
		//090001 0 00 "Distribute Feeder#4 Axis Motor Home Check Error."
		CString strJamCode = "060001";
		if	   ( nAxis == M_BCR_FEEDER_2) strJamCode = "070001";
		else if( nAxis == M_BCR_FEEDER_3) strJamCode = "080001";
		else if( nAxis == M_BCR_FEEDER_4) strJamCode = "090001";
		CTL_Lib.Alarm_Error_Occurrence(151, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Feeders::EBFI_Barcode_Plater_Vacuum_OnOff( int iOnOff)
{
	for ( int i = 0; i < 4; i++)
	{
		g_ioMgr.set_out_bit( st_io.o_distribute_plate_vacuum[i], iOnOff);
	}
	if( iOnOff == IO_ON) mn_InitStep = EBFI_BARCODE_FEEDER_EMPTY_CHK;
	else				 mn_InitStep = EBFI_FEEDER_EJECT_ON;
}

void CRun_Barcode_Feeders::EBFI_Barcode_Plater_Vacuum_OnOff_Check()
{//513081 1 00 "Distribution Label Printer1 Plate Vacuum On Check Error."
	//513091 1 00 "Distribution Label Printer2 Plate Vacuum On Check Error."
	//513101 1 00 "Distribution Label Printer3 Plate Vacuum On Check Error."
	//513121 1 00 "Distribution Label Printer4 Plate Vacuum On Check Error."
// 	if( g_ioMgr.get_in_bit( st_io.i_distribute_plate_vacuum_chk[0]) == IO_ON)
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence(152, CTL_dWARNING, "513081" );
// 		return;
// 	}
// 	else if( g_ioMgr.get_in_bit( st_io.i_distribute_plate_vacuum_chk[1]) == IO_ON)
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence(153, CTL_dWARNING, "513091" );
// 		return;
// 	}
// 	else if( g_ioMgr.get_in_bit( st_io.i_distribute_plate_vacuum_chk[2]) == IO_ON)
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence(154, CTL_dWARNING, "513101" );
// 		return;
// 	}
// 	else if( g_ioMgr.get_in_bit( st_io.i_distribute_plate_vacuum_chk[3]) == IO_ON)
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence(155, CTL_dWARNING, "513121" );
// 		return;
// 	}
////

	mn_InitStep = EBFI_FEEDER_VACUUM_OFF;
}

void CRun_Barcode_Feeders::EBFI_Feeder_EjectOnOff( int iOnOff )
{
	for ( int i = 0; i < 4; i++)
	{
		g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[i] , iOnOff);
	}
	if( iOnOff == IO_ON )	mn_InitStep = EBFI_FEEDER_EJECT_OFF;
	else                    mn_InitStep = EBFI_FINISH;
}

////2014,1220
void CRun_Barcode_Feeders::EBFI_HomeCheck_Reject_X( int nAxis )
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EBFI_FEEDER1_REJECT; 
		Func.OnInitBtn( RBTN_BCR_REJECT_X );//2014.1018	
	}
	else if( nRet == BD_ERROR )
	{//060001 0 00 "Distribute Feeder#1 Axis Motor Home Check Error."
		CString strJamCode = "060001";
		strJamCode = "090001";
		CTL_Lib.Alarm_Error_Occurrence(1510, CTL_dWARNING, strJamCode );
	}
}
////

void CRun_Barcode_Feeders::ReqPlateUpDn( int nSite, EFB efb )
{
	m_fbplateupdn[nSite] = efb;
	m_step_plateupdn[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Feeders::SetPlateUpDn( int nSite, int iOnOff )
{
	int nUpDn = st_io.o_distribute_plate_updown[nSite];

	g_ioMgr.set_out_bit( nUpDn, iOnOff);
}

bool CRun_Barcode_Feeders::CheckPlateUpDn( int nSite, int iOnOff )
{
	int nLock_1 = st_io.i_distribute_plate_up_chk[nSite] ;
	int nLock_2 = st_io.i_distribute_plate_down_chk[nSite];

	if( g_ioMgr.get_in_bit( nLock_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nLock_2 ) != !iOnOff) return false;

	return true;
}

void CRun_Barcode_Feeders::Run_Move_Bcr_Plate_UpDown( int nSite )
{
	if( m_fbplateupdn[nSite] == FB_NONE ) return;

	int iOnOff = IO_ON;
	if( m_fbplateupdn[nSite] == BACKWARD )
		iOnOff = IO_OFF;

	switch( m_step_plateupdn[nSite] )
	{
	case ESTEP_CYL_START: m_step_plateupdn[nSite] = ESTEP_CYL_MOVE;	break;

	case ESTEP_CYL_MOVE:
		{
			SetPlateUpDn( nSite, iOnOff);
			if( COMI.mn_run_status == dRUN ){
				st_work.mn_bcr_plate_updn[nSite] = iOnOff;
			}
			m_step_plateupdn[nSite] = ESTEP_CYL_CHK;
			m_dwTimePlaterUpDnStart[nSite] = GetCurrentTime();
		}

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckPlateUpDn( nSite, iOnOff) ){
				m_step_plateupdn[nSite] = ESTEP_CYL_FINISH;
				m_dwTimePlaterUpDn[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimePlaterUpDnStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_PLATE_UPDOWN] )
			{
				m_step_plateupdn[nSite] = ESTEP_CYL_FINISH;
				m_dwTimePlaterUpDn[nSite] = GetCurrentTime();
				break;

				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Plate_UoDown_Time(st_io.i_distribute_plate_up_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 160, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimePlaterUpDnStart[nSite] < 0 )
			{
				m_dwTimePlaterUpDnStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimePlaterUpDn[nSite] < 0 )
				m_dwTimePlaterUpDn[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimePlaterUpDn[nSite] < st_time.n_wait_time[E_WAIT_BCR_PLATE_UPDOWN][iOnOff] )
				return;

			m_step_plateupdn[nSite] = ESTEP_CYL_START;
			m_fbplateupdn[nSite] = FB_NONE;
		}
		break;	
	}
}

void CRun_Barcode_Feeders::SetRetryCnt(int nSite, int iOnOff, int nCnt)
{
	m_nReteyCnt[nSite][iOnOff] = nCnt;
}

//2014,1222
int CRun_Barcode_Feeders::OnGetFmtfromServer()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int mn_gleft = 0, mn_gright = 0;
	CString strJamCode;
	
	switch(mn_fmt_step)
	{
	case 0:
		mn_comm_pos = CTL_LEFT;
// 		if( st_handler.mn_Lotcnt < 0) st_handler.mn_Lotcnt = 0;
// 		st_handler.mn_Lotcnt++;
		mn_fmt_step = 1000;
		break;
		
	case 1000:
		if( st_handler.mn_curr_back_conv == 1 || ( st_basic.n_mode_device == 1 && st_basic.mn_mode_run == 1 &&
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[mn_comm_pos] == CTL_NO ) )
		{
			if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[mn_comm_pos] == 1 &&
				( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[mn_comm_pos] == 1) )
			{
				if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[mn_comm_pos] == 1 )
				{
					st_handler.m_strRPropEnd = "NO";
					g_RunNetwork.st_barcodenet[mn_comm_pos].nCommand = CTL_YES;
					mn_fmt_step = 1100;
				}
			}
// 			mn_fmt_step = 1100;
		}
		break;
		
	case 1100:
		if(g_RunNetwork.st_barcodenet[mn_comm_pos].nComplete == CTL_YES)
		{
// 			mn_fmt_step = 2000;
			mn_fmt_step = 1200;
		}
		else if( g_RunNetwork.st_barcodenet[mn_comm_pos].nCommand == CTL_NO)
		{
			//mn_fmt_step = 2000;
		}
		break;

	case 1200:
		mn_comm_pos = CTL_RIGHT;
		mn_fmt_step = 1300;
		break;
		
	case 1300:
		if( st_handler.mn_curr_back_conv == 1 || ( st_basic.n_mode_device == 1 && st_basic.mn_mode_run == 1 &&
			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[mn_comm_pos] == CTL_NO ) )
		{
			if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[mn_comm_pos] == 1 &&
				( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[mn_comm_pos] == 1) )
			{
				if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[mn_comm_pos] == 1 )
				{
					g_RunNetwork.st_barcodenet[mn_comm_pos].nCommand = CTL_YES;
					mn_fmt_step = 2000;
				}
			}
			// 			mn_fmt_step = 1100;
		}
		break;
		
	case 2000:
		if( g_RunNetwork.st_barcodenet[CTL_LEFT].nComplete == CTL_YES &&
			g_RunNetwork.st_barcodenet[CTL_RIGHT].nComplete == CTL_YES ) 
		{
// 			m_dwTime_Netbcr = GetCurrentTime();
			mn_fmt_step = 3000;
		}
// 		else
// 		{
// 			if( g_RunNetwork.st_barcodenet[CTL_LEFT].nComplete != CTL_YES )
// 			{
// 				if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[CTL_LEFT] == 1 &&
// 					( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] == 0) )
// 				{
// 					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 					{
// 						sprintf(st_msg.c_abnormal_msg, "[FmtFromServer] CTL_LEFT is not Device");
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 					}
// 					sprintf(mc_jamcode, "601000"); //601000 1 00 "id buffer top Input index program in Left flag setting error from front machine.
// 					CTL_Lib.Alarm_Error_Occurrence(9102, dWARNING, mc_jamcode);
// 				}
// 				else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[CTL_LEFT] == 1 &&
// 					( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] == 1) )
// 				{
// 					mn_comm_pos = CTL_LEFT;
// 					mn_fmt_step = 1000;
// 				}
// 			}
// 			else if( g_RunNetwork.st_barcodenet[CTL_RIGHT].nComplete != CTL_YES )
// 			{
// 				if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[CTL_RIGHT] == 1 &&
// 					( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] == 0) )
// 				{
// 					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 					{
// 						sprintf(st_msg.c_abnormal_msg, "[FmtFromServer] CTL_RIGHT is not Device");
// 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 					}
// 				}
// 				else if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[CTL_RIGHT] == 1 &&
// 					( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] == 1) )
// 				{
// 					mn_comm_pos = CTL_RIGHT;
// 					mn_fmt_step = 1000;
// 				}				
// 			}
// 		}
		break;

	case 2100:
		if( GetCurrentTime() - m_dwTime_Netbcr <= 0) m_dwTime_Netbcr = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Netbcr < 100) break;
		mn_fmt_step = 3000;
		break;
		
	case 3000:
		if(st_handler.m_strRPropEnd == "YES")
		{
			g_RunNetwork.st_barcodenet[CTL_LEFT].nComplete = CTL_NO;
			g_RunNetwork.st_barcodenet[CTL_RIGHT].nComplete = CTL_NO;
			nFuncRet = CTLBD_RET_GOOD;
			mn_fmt_step = 0;
		}
		break;
	}
	
	return nFuncRet;
}