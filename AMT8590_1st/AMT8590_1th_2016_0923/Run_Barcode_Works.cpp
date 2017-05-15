// Run_Barcode_Works.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Barcode_Works.h"

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
// CRun_Barcode_Works

IMPLEMENT_SERIAL(CRun_Barcode_Works, CObject, 1)
CRun_Barcode_Works g_Run_Barcode_Works;
CRun_Barcode_Works::CRun_Barcode_Works()
{
	Thread_Variable_Initial();
}

CRun_Barcode_Works::~CRun_Barcode_Works()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Barcode_Works message handlers
void CRun_Barcode_Works::Thread_Variable_Initial()
{
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;

}

void CRun_Barcode_Works::Thread_Run()
{
// return;
																				
	if( Func.OnIsInterruptCondition() != 1 )
		return;

// 	st_handler.n_only_work_bcr1 = CTL_YES;
	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
// 		return;
 		Run_Move1();
		Run_Move2();
		break;
	
	case dREINSTATE:
		Run_Reinstate1();
		Run_Reinstate2();
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


	for ( int i = 0; i < 2; i++ )
	{
		Run_Move_M1_Top_Vacuum( i );
		Run_Move_M1_Btm_Vacuum( i );
		Run_Move_M2_Top_Vacuum( i );
		Run_Move_M2_Btm_Vacuum( i );
//		Run_Move_M1_Guide_FwdBwd( i );//2014.1018 //2014,1125 ysj
//		Run_Move_M2_Guide_FwdBwd( i );//2014.1018
		Run_Move_Box_UpDn(i);
		Run_Move_M1_RollerFwdBwd(i);
		Run_Move_M2_RollerFwdBwd(i);
	}

	for ( int j = 0; j < 2; j++ )
	{
		for ( int k = 0; k < 2; k++)
		{
// 			if(j != 0) //2014.1018 rolling align pusher ¨ú©ªA¨ö
// 			{
				Run_Move_RollerFwdBwd( j, k );
// 			}
		}
	}

}


void CRun_Barcode_Works::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_BARCODE_ROBOT] != CTL_NO )	return;

	switch(mn_InitStep)
	{
	case EBCRI_START: mn_InitStep = EBCRI_CHECK_ROBOT_CHK; break;

	case EBCRI_CHECK_ROBOT_CHK: EBCRI_Check_Rbot_Chk(); break;

	case EBCRI_HOMECHK_RBT1_Z1: EBCRI_HomeCheck_Z( M_BCR_ATTACH1_Z1 ); break;
	case EBCRI_HOMECHK_RBT1_Z2: EBCRI_HomeCheck_Z( M_BCR_ATTACH1_Z2 ); break;
	case EBCRI_HOMECHK_RBT2_Z1: EBCRI_HomeCheck_Z( M_BCR_ATTACH2_Z1 ); break;
	case EBCRI_HOMECHK_RBT2_Z2: EBCRI_HomeCheck_Z( M_BCR_ATTACH2_Z2 ); break;

	case EBCRI_TOP_VACUUM_BWD: EBCRI_Top_Vacuum_Backward(); break;
	case EBCRI_TOP_VACUUM_BWD_CHK: EBCRI_Top_Vacuum_Backward_Check(); break;

	case EBCRI_BTM_VACUUM_BWD: EBCRI_Btm_Vacuum_Backward(); break;
	case EBCRI_BTM_VACUUM_BWD_CHK: EBCRI_Btm_Vacuum_Backward_Check(); break;

	case EBCRI_VACUUM_ON: 
		EBCRI_VacuumOnOff(IO_ON); 
		m_dwTimeInit = GetCurrentTime();
		break;
	case EBCRI_BARCODE_EMPTY_CHK: 
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_VACUUM][IO_ON]) break;
		EBCRI_Barcode_Empty_Check(); break;

	case EBCRI_VACUUM_OFF: EBCRI_VacuumOnOff(IO_OFF); break;
	case EBCRI_EJECT_ON: 
		EBCRI_EjectOnOff(IO_ON); 
		m_dwTimeInit = GetCurrentTime();
		break;
	case EBCRI_EJECT_OFF: 
		if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_EJECT][IO_ON]) break;
		EBCRI_EjectOnOff(IO_OFF); break;

	case EBCRI_M1_GUIDE_BWD: EBCRI_M1_Guide_Backward(); break;
	case EBCRI_M1_GUIDE_BWD_CHK: EBCRI_M1_Guide_Backward_Check(); break;

	case EBCRI_M2_GUIDE_BWD: EBCRI_M2_Guide_Backward(); break;
	case EBCRI_M2_GUIDE_BWD_CHK: EBCRI_M2_Guide_Backward_Check(); break;

//	case EBCRI_M1_GUIDE_UP: EBCRI_M1_Guide_Up(); break;
//	case EBCRI_M1_GUIDE_UP_CHK: EBCRI_M1_Guide_Up_Check(); break;
//
//	case EBCRI_M2_GUIDE_UP: EBCRI_M2_Guide_Up(); break;
//	case EBCRI_M2_GUIDE_UP_CHK: EBCRI_M2_Guide_Up_Check(); break;		

// 	case EBCRI_HOMECHK_RBT1_R1: EBCRI_HomeCheck_R(M_BCR_ATTACH1_R1); break;
// 	case EBCRI_HOMECHK_RBT1_R2: EBCRI_HomeCheck_R(M_BCR_ATTACH1_R2); break;
// 	case EBCRI_HOMECHK_RBT2_R1: EBCRI_HomeCheck_R(M_BCR_ATTACH2_R1); break;
// 	case EBCRI_HOMECHK_RBT2_R2: EBCRI_HomeCheck_R(M_BCR_ATTACH2_R2); break;

	case EBCRI_HOMECHK_RBT1_X: EBCRI_HomeCheck_X(M_BCR_ATTACH1_X); break;
	case EBCRI_HOMECHK_RBT2_X: EBCRI_HomeCheck_X(M_BCR_ATTACH2_X); break;

	case EBCRI_MOVE_SAFETY_RBT1_X: EBCRI_Move_Safety_X(M_BCR_ATTACH1_X); break;
	case EBCRI_MOVE_SAFETY_RBT2_X: EBCRI_Move_Safety_X(M_BCR_ATTACH2_X); break;

	case EBCRI_BOX_SAFETY:
		ReqBoxUpDn(0, BACKWARD);
		ReqBoxUpDn(1, BACKWARD);
		mn_InitStep = EBCRI_BOX_SAFETY_CHK;

		break;

	case EBCRI_BOX_SAFETY_CHK:
		if(m_fbboxupdn[0] != FB_NONE ) break;
		if(m_fbboxupdn[1] != FB_NONE ) break;
		if( CheckBoxUpDn( 0, IO_OFF) != true || CheckBoxUpDn( 1, IO_OFF) != true)
		{
			CString strJamCode;
			if( CheckBoxUpDn( 0, IO_OFF) != true )
				strJamCode.Format("5%04d%d", st_io.i_top_box_up_clamp_chk[0], IO_OFF);
			else
				strJamCode.Format("5%04d%d", st_io.i_top_box_up_clamp_chk[1], IO_OFF);
			CTL_Lib.Alarm_Error_Occurrence( 1373, dWARNING, strJamCode );
			break;
		}
		mn_InitStep = EBCRI_BOX_ROTATE;
		break;

	case EBCRI_BOX_ROTATE:
		ReqM1RotateFwdBwd( 0 ,BACKWARD);
		ReqM1RotateFwdBwd( 1 ,BACKWARD);
		ReqM2RotateFwdBwd( 0 ,BACKWARD);
		ReqM2RotateFwdBwd( 1 ,BACKWARD);
		mn_InitStep = EBCRI_BOX_ROTATE_CHK;
		break;

	case EBCRI_BOX_ROTATE_CHK:
		if( m_fbm1rotatefwdbwd[0] != FB_NONE) break;
		if( m_fbm1rotatefwdbwd[1] != FB_NONE) break;
		if( m_fbm2rotatefwdbwd[0] != FB_NONE) break;
		if( m_fbm2rotatefwdbwd[1] != FB_NONE) break;

		mn_InitStep = EBCRI_FINISH;
		break;

	case EBCRI_FINISH: EBCRI_Finish(); break;


	}

}

void CRun_Barcode_Works::EBCRI_Check_Rbot_Chk()
{
	if( st_handler.mn_init_state[INIT_ROLLER_ROBOT] != CTL_NO ) return;
	mn_InitStep = EBCRI_HOMECHK_RBT1_Z1;
}

void CRun_Barcode_Works::EBCRI_HomeCheck_Z( int nAxis )
{	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		if( nAxis == M_BCR_ATTACH1_Z1)
		{
			mn_InitStep = EBCRI_HOMECHK_RBT1_Z2;
			Func.OnInitBtn( RBTN_BCR_ATTACH1_Z1 );//2014.1018
		}
		else if( nAxis == M_BCR_ATTACH1_Z2)
		{
			mn_InitStep = EBCRI_HOMECHK_RBT2_Z1;
			Func.OnInitBtn( RBTN_BCR_ATTACH1_Z2 );//2014.1018
		}
		else if( nAxis == M_BCR_ATTACH2_Z1) 
		{
			mn_InitStep = EBCRI_HOMECHK_RBT2_Z2;
			Func.OnInitBtn( RBTN_BCR_ATTACH2_Z1 );//2014.1018
		}
		else if( nAxis == M_BCR_ATTACH2_Z2) 
		{
			mn_InitStep = EBCRI_TOP_VACUUM_BWD;
			Func.OnInitBtn( RBTN_BCR_ATTACH2_Z2 );//2014.1018
		}
	}
	else if( nRet == BD_ERROR )
	{//180001 0 00 "Distribute#1 Attach Z#1 Axis Motor Home Check Error."
		//200001 0 00 "Distribute#1 Attach Z#2 Axis Motor Home Check Error."
		//230001 0 00 "Distribute#2 Attach Z#1 Axis Motor Home Check Error."
		//250001 0 00 "Distribute#2 Attach Z#2 Axis Motor Home Check Error."
		CString strJamCode = "180001";
		if	   ( nAxis == M_BCR_ATTACH1_Z2) strJamCode = "200001";
		else if( nAxis == M_BCR_ATTACH2_Z1) strJamCode = "230001";
		else if( nAxis == M_BCR_ATTACH2_Z2) strJamCode = "250001";
		CTL_Lib.Alarm_Error_Occurrence(200, CTL_dWARNING, strJamCode );
	}

}

void CRun_Barcode_Works::EBCRI_HomeCheck_X( int nAxis )
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);

	if( nRet == BD_GOOD )
	{
		if(nAxis == M_BCR_ATTACH1_X)
		{
			mn_InitStep = EBCRI_HOMECHK_RBT2_X;
//			Func.OnInitBtn( RBTN_BCR_ATTACH1_X );//2014.1018
			Func.OnInitBtn( RBTN_BCR_ATTACH1_R1 );//2014.1018
			Func.OnInitBtn( RBTN_BCR_ATTACH1_R2 );//2014.1018
		}
		else if(nAxis == M_BCR_ATTACH2_X)
		{
			//mn_InitStep = EBCRI_HOMECHK_RBT2_X;
			mn_InitStep = EBCRI_MOVE_SAFETY_RBT1_X;//2014.1018
// 			ReqBoxUpDn(0, BACKWARD);
// 			ReqBoxUpDn(1, BACKWARD);
//			Func.OnInitBtn( RBTN_BCR_ATTACH2_X );//2014.1018
			Func.OnInitBtn( RBTN_BCR_ATTACH2_R1 );//2014.1018
			Func.OnInitBtn( RBTN_BCR_ATTACH2_R2 );//2014.1018
		}
	}
	else if( nRet == BD_ERROR )
	{//170001 0 00 "Distribute#1 Attach X Axis Motor Home Check Error."
		//220001 0 00 "Distribute#2 Attach X Axis Motor Home Check Error."
		CString strJamCode = "170001";
		if	   ( nAxis == M_BCR_ATTACH2_X) strJamCode = "220001";
		CTL_Lib.Alarm_Error_Occurrence(201, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Works::EBCRI_Move_Safety_X( int nAxis )
{
	int nRet = CTL_Lib.Single_Move( nAxis, st_motor[nAxis].d_pos[P_X_BCR_PICK], st_basic.nManualSpeed);
	
	if( nRet == BD_GOOD )
	{
		if(nAxis == M_BCR_ATTACH1_X)
		{
			mn_InitStep = EBCRI_MOVE_SAFETY_RBT2_X;
			Func.OnInitBtn( RBTN_BCR_ATTACH1_X );
		}
		else if(nAxis == M_BCR_ATTACH2_X)
		{
			mn_InitStep = EBCRI_BOX_SAFETY;
			Func.OnInitBtn( RBTN_BCR_ATTACH2_X );
		}
	}
	else if( nRet == BD_ERROR )
	{//000006 1 00 "Density_Feeder1_Axis_Motor_Move_Error."
		CString strJamCode = "170006";
		if	   ( nAxis == M_BCR_ATTACH2_X) strJamCode = "220006";
		CTL_Lib.Alarm_Error_Occurrence(298, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Works::EBCRI_M1_Guide_Backward()
{
	// 	ReqM1GuideFwdBwd( EBVS_LEFT , BACKWARD );
	// 	ReqM1GuideFwdBwd( EBVS_RIGHT, BACKWARD );
	ReqM1GuideFwdBwd( EBVS_LEFT , BACKWARD );//2014.1018
	ReqM1GuideFwdBwd( EBVS_RIGHT, BACKWARD );//2014.1018
	mn_InitStep = EBCRI_M1_GUIDE_BWD_CHK;
}

void CRun_Barcode_Works::ReqM1GuideFwdBwd( int nSite, EFB efb)
{
	m_fbm1guidefwdbwd[nSite] = efb;
	m_step_m1guide_fwdbwd[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Works::EBCRI_M1_Guide_Backward_Check()
{
// 	if( GetM1GuideFwdBwd( EBVS_LEFT ) != FB_NONE ) return;
// 	if( GetM1GuideFwdBwd( EBVS_RIGHT ) != FB_NONE ) return;

	mn_InitStep = EBCRI_M2_GUIDE_BWD;

}

void CRun_Barcode_Works::SetM1GuideFwdBwd( int nSite, int iOnOff )
{
	int nVac_1 = st_io.o_distribute_rbt1_box_guide_fwdback[nSite];
	g_ioMgr.set_out_bit( nVac_1, iOnOff);

}

bool CRun_Barcode_Works::CheckM1GuideFwdBwd( int nSite, int iOnOff )
{
//	int nGuide_1 = st_io.i_distribute_rbt1_box_guide_fwd_chk[nSite] ;
	int nGuide_2 = st_io.i_distribute_rbt1_box_guide_back_chk[nSite];

//	if( g_ioMgr.get_in_bit( nGuide_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nGuide_2 ) != !iOnOff) return false;

	return true;
}

void CRun_Barcode_Works::Run_Move_M1_Guide_FwdBwd( int nSite)
{
	if(m_fbm1guidefwdbwd[nSite] == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbm1guidefwdbwd[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m1guide_fwdbwd[nSite])
	{
	case ESTEP_CYL_START: m_step_m1guide_fwdbwd[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM1GuideFwdBwd( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_m1guide_fwdback[nSite] = iOnOff;
			}
			m_step_m1guide_fwdbwd[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM1GuideFwdBwdStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM1GuideFwdBwd( nSite, iOnOff ) )
			{
				m_step_m1guide_fwdbwd[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM1GuideFwdBwd[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM1GuideFwdBwdStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_GUIDE_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Guide_FwdBwd_Time(st_io.i_distribute_rbt1_box_guide_fwd_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 202, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeM1GuideFwdBwdStart[nSite] < 0 )
			{
				m_dwTimeM1GuideFwdBwdStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM1GuideFwdBwd[nSite] < 0 )
				m_dwTimeM1GuideFwdBwd[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM1GuideFwdBwd[nSite] < st_time.n_wait_time[E_WAIT_BCR_GUIDE_FWDBWD][iOnOff] )
				return;

			m_step_m1guide_fwdbwd[nSite] = ESTEP_CYL_START;
			m_fbm1guidefwdbwd[nSite] = FB_NONE;
		}
		break;	
	}
	
}

void CRun_Barcode_Works::EBCRI_M2_Guide_Backward()
{
	// 	ReqM2GuideFwdBwd( EBVS_LEFT , BACKWARD );
	// 	ReqM2GuideFwdBwd( EBVS_RIGHT, BACKWARD );
	ReqM2GuideFwdBwd( EBVS_LEFT , BACKWARD );
	//2015.0206
	//ReqM2GuideFwdBwd( EBVS_RIGHT, BACKWARD );
	mn_InitStep = EBCRI_M2_GUIDE_BWD_CHK;
}

void CRun_Barcode_Works::ReqM2GuideFwdBwd( int nSite, EFB efb)
{
	m_fbm2guidefwdbwd[nSite] = efb;
	m_step_m2guide_fwdbwd[nSite] = ESTEP_CYL_START;
}


void CRun_Barcode_Works::EBCRI_M2_Guide_Backward_Check()
{
// 	if( GetM2GuideFwdBwd( EBVS_LEFT ) != FB_NONE ) return;
// 	if( GetM2GuideFwdBwd( EBVS_RIGHT ) != FB_NONE ) return;

// 	mn_InitStep = EBCRI_HOMECHK_RBT1_R1;
	mn_InitStep = EBCRI_HOMECHK_RBT1_X;

}

void CRun_Barcode_Works::SetM2GuideFwdBwd( int nSite, int iOnOff )
{
	int nVac_1 = st_io.o_distribute_rbt2_box_guide_fwdback[nSite];
	g_ioMgr.set_out_bit( nVac_1, iOnOff);

}

bool CRun_Barcode_Works::CheckM2GuideFwdBwd( int nSite, int iOnOff )
{
//	int nGuide_1 = st_io.i_distribute_rbt2_box_guide_up_chk[nSite] ;
	int nGuide_2 = st_io.i_distribute_rbt2_box_guide_back_chk[nSite];

//	if( g_ioMgr.get_in_bit( nGuide_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nGuide_2 ) != !iOnOff) return false;

	return true;
}


void CRun_Barcode_Works::Run_Move_M2_Guide_FwdBwd( int nSite)
{
	if(m_fbm2guidefwdbwd[nSite] == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbm2guidefwdbwd[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m2guide_fwdbwd[nSite])
	{
	case ESTEP_CYL_START: m_step_m2guide_fwdbwd[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM2GuideFwdBwd( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_m2guide_fwdback[nSite] = iOnOff;
			}
			m_step_m2guide_fwdbwd[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM2GuideFwdBwdStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM2GuideFwdBwd( nSite, iOnOff ) )
			{
				m_step_m2guide_fwdbwd[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM2GuideFwdBwd[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM2GuideFwdBwdStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_GUIDE_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Guide_FwdBwd_Time(st_io.i_distribute_rbt2_box_guide_fwd_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 203, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeM2GuideFwdBwdStart[nSite] < 0 )
			{
				m_dwTimeM2GuideFwdBwdStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM2GuideFwdBwd[nSite] < 0 )
				m_dwTimeM2GuideFwdBwd[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM2GuideFwdBwd[nSite] < st_time.n_wait_time[E_WAIT_BCR_GUIDE_FWDBWD][iOnOff] )
				return;

			m_step_m2guide_fwdbwd[nSite] = ESTEP_CYL_START;
			m_fbm2guidefwdbwd[nSite] = FB_NONE;
		}
		break;	
	}
}

//void CRun_Barcode_Works::EBCRI_M1_Guide_Up()
//{
//	ReqM1GuideUpDn( EBVS_LEFT , FU_UP );
//	ReqM1GuideUpDn( EBVS_RIGHT, FU_UP );
//	mn_InitStep = EBCRI_M1_GUIDE_UP_CHK;
//}
//
//void CRun_Barcode_Works::ReqM1GuideUpDn( int nSite, EUD eud)
//{
//	m_fbm1guideupdn[nSite] = eud;
//	m_step_m1guide_updn[nSite] = ESTEP_CYL_START;
//}
//
//void CRun_Barcode_Works::EBCRI_M1_Guide_Up_Check()
//{
//	if( GetM1GuideUpDn( EBVS_LEFT ) != FU_NONE ) return;
//	if( GetM1GuideUpDn( EBVS_RIGHT ) != FU_NONE ) return;
//
//	mn_InitStep = EBCRI_M2_GUIDE_UP;
//}

//void CRun_Barcode_Works::SetM1GuideUpDn( int nSite, int iOnOff )
//{
//	int nVac_1 = st_io.o_distribute_rbt1_box_guide_updown[nSite];
//	g_ioMgr.set_out_bit( nVac_1, iOnOff);
//
//}
//
//bool CRun_Barcode_Works::CheckM1GuideUpDn( int nSite, int iOnOff )
//{
//	int nGuide_1 = st_io.i_distribute_rbt1_box_guide_up_chk[nSite] ;
//	int nGuide_2 = st_io.i_distribute_rbt1_box_guide_down_chk[nSite];
//
//	if( g_ioMgr.get_in_bit( nGuide_1 ) != iOnOff) return false;
//	if( g_ioMgr.get_in_bit( nGuide_2 ) != !iOnOff) return false;
//
//	return true;
//}
//
//
//void CRun_Barcode_Works::Run_Move_M1_Guide_UpDn( int nSite)
//{
//	if(m_fbm1guideupdn[nSite] == FU_NONE) return;
//	int iOnOff = IO_ON;
//	if( m_fbm1guideupdn[nSite] == FU_UP)
//		iOnOff = IO_OFF;
//
//	switch(m_step_m1guide_updn[nSite])
//	{
//	case ESTEP_CYL_START: m_step_m1guide_updn[nSite] = ESTEP_CYL_MOVE;	break;
//	case ESTEP_CYL_MOVE:
//		{
//			SetM1GuideUpDn( nSite, iOnOff);
//			if(COMI.mn_run_status == dRUN){
//				st_work.mn_bcr_m1guide_updown[nSite] = iOnOff;
//			}
//			m_step_m1guide_updn[nSite] = ESTEP_CYL_CHK;
//			m_dwTimeM1GuideUpDnStart[nSite] = GetCurrentTime();
//		}
//		break;
//
//	case ESTEP_CYL_CHK:
//		{
//			if( CheckM1GuideUpDn( nSite, iOnOff ) )
//			{
//				m_step_m1guide_updn[nSite] = ESTEP_CYL_FINISH;
//				m_dwTimeM1GuideUpDn[nSite] = GetCurrentTime();
//			}
//			else if( GetCurrentTime() - m_dwTimeM1GuideUpDnStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_GUIDE_UPDOWN] )
//			{
//				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
//				{
//					CString strJamCode = GetJamCode_Bcr_Guide_UpDown_Time(st_io.i_distribute_rbt1_box_guide_up_chk[nSite], iOnOff );
//					CTL_Lib.Alarm_Error_Occurrence( 204, dWARNING, strJamCode );
//				}
//			}
//			else if( GetCurrentTime() - m_dwTimeM1GuideUpDnStart[nSite] < 0 )
//			{
//				m_dwTimeM1GuideUpDnStart[nSite] = GetCurrentTime();
//			}
//		}
//		break;
//
//	case ESTEP_CYL_FINISH:
//		{
//			if( GetCurrentTime() - m_dwTimeM1GuideUpDn[nSite] < 0 )
//				m_dwTimeM1GuideUpDn[nSite] = GetCurrentTime();
//			else if( GetCurrentTime() - m_dwTimeM1GuideUpDn[nSite] < st_time.n_wait_time[E_WAIT_BCR_GUIDE_UPDOWN][iOnOff] )
//				return;
//
//			m_step_m1guide_updn[nSite] = ESTEP_CYL_START;
//			m_fbm1guideupdn[nSite] = FU_NONE;
//		}
//		break;	
//	}
//}

//void CRun_Barcode_Works::EBCRI_M2_Guide_Up()
//{
//	ReqM2GuideUpDn( EBVS_LEFT , FU_UP );
//	ReqM2GuideUpDn( EBVS_RIGHT, FU_UP );
//	mn_InitStep = EBCRI_M2_GUIDE_UP_CHK;
//
//}
//
//void CRun_Barcode_Works::ReqM2GuideUpDn( int nSite, EUD eud)
//{
//	m_fbm2guideupdn[nSite] = eud;
//	m_step_m2guide_updn[nSite] = ESTEP_CYL_START;
//}
//
//
//void CRun_Barcode_Works::EBCRI_M2_Guide_Up_Check()
//{
//	if( GetM2GuideUpDn( EBVS_LEFT ) != FU_NONE ) return;
//	if( GetM2GuideUpDn( EBVS_RIGHT ) != FU_NONE ) return;
//
//	mn_InitStep = EBCRI_HOMECHK_RBT1_R1;
//
//}

//void CRun_Barcode_Works::SetM2GuideUpDn( int nSite, int iOnOff )
//{
//	int nVac_1 = st_io.o_distribute_rbt2_box_guide_updown[nSite];
//	g_ioMgr.set_out_bit( nVac_1, iOnOff);
//
//}
//
//bool CRun_Barcode_Works::CheckM2GuideUpDn( int nSite, int iOnOff )
//{
//	int nGuide_1 = st_io.i_distribute_rbt2_box_guide_up_chk[nSite] ;
//	int nGuide_2 = st_io.i_distribute_rbt2_box_guide_down_chk[nSite];
//
//	if( g_ioMgr.get_in_bit( nGuide_1 ) != iOnOff) return false;
//	if( g_ioMgr.get_in_bit( nGuide_2 ) != !iOnOff) return false;
//
//	return true;
//}

//void CRun_Barcode_Works::Run_Move_M2_Guide_UpDn( int nSite)
//{
//	if(m_fbm2guideupdn[nSite] == FU_NONE) return;
//	int iOnOff = IO_ON;
//	if( m_fbm2guideupdn[nSite] == FU_UP)
//		iOnOff = IO_OFF;
//
//	switch(m_step_m2guide_updn[nSite])
//	{
//	case ESTEP_CYL_START: m_step_m2guide_updn[nSite] = ESTEP_CYL_MOVE;	break;
//	case ESTEP_CYL_MOVE:
//		{
//			SetM2GuideUpDn( nSite, iOnOff);
//			if(COMI.mn_run_status == dRUN){
//				st_work.mn_bcr_m2guide_updown[nSite] = iOnOff;
//			}
//			m_step_m2guide_updn[nSite] = ESTEP_CYL_CHK;
//			m_dwTimeM2GuideUpDnStart[nSite] = GetCurrentTime();
//		}
//		break;
//
//	case ESTEP_CYL_CHK:
//		{
//			if( CheckM2GuideUpDn( nSite, iOnOff ) )
//			{
//				m_step_m2guide_updn[nSite] = ESTEP_CYL_FINISH;
//				m_dwTimeM2GuideUpDn[nSite] = GetCurrentTime();
//			}
//			else if( GetCurrentTime() - m_dwTimeM2GuideUpDnStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_GUIDE_UPDOWN] )
//			{
//				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
//				{
//					CString strJamCode = GetJamCode_Bcr_Guide_UpDown_Time(st_io.i_distribute_rbt2_box_guide_up_chk[nSite], iOnOff );
//					CTL_Lib.Alarm_Error_Occurrence( 205, dWARNING, strJamCode );
//				}
//			}
//			else if( GetCurrentTime() - m_dwTimeM2GuideUpDnStart[nSite] < 0 )
//			{
//				m_dwTimeM2GuideUpDnStart[nSite] = GetCurrentTime();
//			}
//		}
//		break;
//
//	case ESTEP_CYL_FINISH:
//		{
//			if( GetCurrentTime() - m_dwTimeM2GuideUpDn[nSite] < 0 )
//				m_dwTimeM2GuideUpDn[nSite] = GetCurrentTime();
//			else if( GetCurrentTime() - m_dwTimeM2GuideUpDn[nSite] < st_time.n_wait_time[E_WAIT_BCR_GUIDE_UPDOWN][iOnOff] )
//				return;
//
//			m_step_m2guide_updn[nSite] = ESTEP_CYL_START;
//			m_fbm2guideupdn[nSite] = FU_NONE;
//		}
//		break;	
//	}
//}


void CRun_Barcode_Works::EBCRI_HomeCheck_R( int nAxis)
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EBCRI_HOMECHK_RBT1_R2;
		if	   ( nAxis == M_BCR_ATTACH1_R2) mn_InitStep = EBCRI_HOMECHK_RBT2_R1;
		else if( nAxis == M_BCR_ATTACH2_R1) mn_InitStep = EBCRI_HOMECHK_RBT2_R2;
		else if( nAxis == M_BCR_ATTACH2_R2) mn_InitStep = EBCRI_HOMECHK_RBT1_X;

//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//190001 0 00 "Distribute#1 Attach R#1 Axis Motor Home Check Error."
		//210001 0 00 "Distribute#1 Attach R#2 Axis Motor Home Check Error."
		//240001 0 00 "Distribute#2 Attach R#1 Axis Motor Home Check Error."
		//260001 0 00 "Distribute#2 Attach R#2 Axis Motor Home Check Error."
		CString strJamCode = "190001";
		if	   ( nAxis == M_BCR_ATTACH1_R2) strJamCode = "210001";
		else if( nAxis == M_BCR_ATTACH2_R1) strJamCode = "240001";
		else if( nAxis == M_BCR_ATTACH2_R2) strJamCode = "260001";
		CTL_Lib.Alarm_Error_Occurrence(206, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Works::EBCRI_VacuumOnOff(int iOnOff)
{
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_left_top_vacuum , iOnOff );
// 	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_left_btm_vacuum, iOnOff );//2014,1217
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_right_top_vacuum, iOnOff );
// 	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_right_btm_vacuum, iOnOff );//2014,1217
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_left_top_vacuum , iOnOff );
// 	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_left_btm_vacuum, iOnOff );//2014,1217
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_right_top_vacuum, iOnOff );
// 	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_right_btm_vacuum, iOnOff );//2014,1217

	if( iOnOff == IO_ON)
	{
		mn_InitStep = EBCRI_BARCODE_EMPTY_CHK;
		m_dwTimeInit = GetCurrentTime();
	}
	else
	{
		mn_InitStep = EBCRI_EJECT_ON;
	}
}

void CRun_Barcode_Works::EBCRI_Barcode_Empty_Check()
{
//515081 1 00 "Distribution Label Robot1 Left Top Vacuum On Check Error."
//515091 1 00 "Distribution Label Robot1 Left Bottom Vacuum On Check Error."
//516081 1 00 "Distribution Label Robot1 Right Top Vacuum On Check Error."
//516091 1 00 "Distribution Label Robot1 Right Bottom Vacuum On Check Error."
//520081 1 00 "Distribution Label Robot2 Left Top Vacuum On Check Error."
//520091 1 00 "Distribution Label Robot2 Left Bottom Vacuum On Check Error."
//521081 1 00 "Distribution Label Robot2 Right Top Vacuum On Check Error."
//521091 1 00 "Distribution Label Robot2 Right Bottom Vacuum On Check Error."
	if( GetCurrentTime() - m_dwTimeInit <= 0 ) m_dwTimeInit = GetCurrentTime();
	if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][1] ) return;

	if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_left_top_vacuum_chk ) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence( 210, dWARNING, "515081" );
		return;
	}
// 	else if(g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_left_btm_vacuum_chk ) == IO_ON)//2014,1217
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence( 211, dWARNING, "515091" );
// 		return;
// 	}
	else if(g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_right_top_vacuum_chk ) == IO_ON)
	{
		CTL_Lib.Alarm_Error_Occurrence( 212, dWARNING, "516081" );
		return;
	}
// 	else if(g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_right_btm_vacuum_chk ) == IO_ON)//2014,1217
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence( 213, dWARNING, "516091" );
// 		return;
// 	}
	else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_left_top_vacuum_chk ) == IO_ON )
	{
		CTL_Lib.Alarm_Error_Occurrence( 214, dWARNING, "520081" );
		return;
	}
// 	else if(g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_left_btm_vacuum_chk ) == IO_ON)//2014,1217
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence( 215, dWARNING, "520091" );
// 		return;
// 	}
	else if(g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_right_top_vacuum_chk ) == IO_ON)
	{
		CTL_Lib.Alarm_Error_Occurrence( 216, dWARNING, "521081" );
		return;
	}
// 	else if(g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_right_btm_vacuum_chk ) == IO_ON)//2014,1217
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence( 217, dWARNING, "521091" );
// 		return;
// 	}

	mn_InitStep = EBCRI_VACUUM_OFF;
}

void CRun_Barcode_Works::EBCRI_EjectOnOff( int iOnOff )
{
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_left_top_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_left_btm_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_right_top_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_right_btm_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_left_top_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_left_btm_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_right_top_eject, iOnOff );
	g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_right_btm_eject, iOnOff );

	if( iOnOff == IO_ON) mn_InitStep = EBCRI_EJECT_OFF;
	else				 mn_InitStep = EBCRI_M1_GUIDE_BWD;

}

void CRun_Barcode_Works::EBCRI_Top_Vacuum_Backward()
{
	ReqM1TopVac( EBVS_LEFT , BACKWARD );
	ReqM1TopVac( EBVS_RIGHT, BACKWARD );	
	ReqM2TopVac( EBVS_LEFT , BACKWARD );
	ReqM2TopVac( EBVS_RIGHT, BACKWARD );
	mn_InitStep = EBCRI_TOP_VACUUM_BWD_CHK;
}

void CRun_Barcode_Works::EBCRI_Top_Vacuum_Backward_Check()
{
	if( GetM1TopVac( EBVS_LEFT ) != FB_NONE ) return;
	if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) return;
	if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) return;
	if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) return;

	mn_InitStep = EBCRI_BTM_VACUUM_BWD;
}

void CRun_Barcode_Works::EBCRI_Btm_Vacuum_Backward()
{
	ReqM1BtmVac( EBVS_LEFT , BACKWARD );
	ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
	ReqM2BtmVac( EBVS_LEFT , BACKWARD );
	//2015.0206
	//ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
	mn_InitStep = EBCRI_BTM_VACUUM_BWD_CHK;

}

void CRun_Barcode_Works::EBCRI_Btm_Vacuum_Backward_Check()
{
	if( GetM1BtmVac( EBVS_LEFT ) != FB_NONE ) return;
	if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) return;
	if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) return;
	//2015.0206
	//if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) return;

	mn_InitStep = EBCRI_VACUUM_ON;
}

void CRun_Barcode_Works::ReqBoxUpDn( int nSite, EFB efb)
{
	m_fbboxupdn[nSite] = efb;
	m_step_box_updn[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Works::ReqM1TopVac( int nSite, EFB efb )
{
	m_fbm1topvac[nSite] = efb;
	m_step_m1topvac[nSite] = ESTEP_CYL_START;	
}

void CRun_Barcode_Works::ReqM1BtmVac( int nSite, EFB efb )
{
	m_fbm1btmvac[nSite] = efb;
	m_step_m1btmvac[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Works::ReqM2TopVac( int nSite, EFB efb )
{
	m_fbm2topvac[nSite] = efb;
	m_step_m2topvac[nSite] = ESTEP_CYL_START;	
}

void CRun_Barcode_Works::ReqM2BtmVac( int nSite, EFB efb )
{
	m_fbm2btmvac[nSite] = efb;
	m_step_m2btmvac[nSite] = ESTEP_CYL_START;
}

//void CRun_Barcode_Works::ReqRollingFwdBwd( int nSite, EFB efb )
//{
//	m_fbrollingfwdbwd[nSite] = efb;
//	m_step_rolling_fwdbwd[nSite] = ESTEP_CYL_START;
//}

void CRun_Barcode_Works::ReqRollingFwdBwd( int nSite, int pos, EFB efb )
{
	m_fbrollingfwdbwd[nSite][pos] = efb;
	m_step_rolling_fwdbwd[nSite][pos] = ESTEP_CYL_START;
}

void CRun_Barcode_Works::SetM1TopVac( int nSite, int iOnOff )
{
	int nVac_1 = st_io.o_distribute_rbt1_top_vacuum_fwdback[nSite];
	
	g_ioMgr.set_out_bit( nVac_1, iOnOff);
}

void CRun_Barcode_Works::SetM1BtmVac( int nSite, int iOnOff )
{
	int nVac_1 = st_io.o_distribute_rbt1_btm_vacuum_fwdback[nSite];
	g_ioMgr.set_out_bit( nVac_1, iOnOff);
}

void CRun_Barcode_Works::SetM2TopVac( int nSite, int iOnOff )
{
	int nVac_1 = st_io.o_distribute_rbt2_top_vacuum_fwdback[nSite];
	
	g_ioMgr.set_out_bit( nVac_1, iOnOff);
}

void CRun_Barcode_Works::SetM2BtmVac( int nSite, int iOnOff )
{
	int nVac_1 = st_io.o_distribute_rbt2_btm_vacuum_fwdback[nSite];
	g_ioMgr.set_out_bit( nVac_1, iOnOff);
}

bool CRun_Barcode_Works::CheckM1TopVac( int nSite, int iOnOff )
{
	int nLock_1 = st_io.i_distribute_rbt1_top_vacuum_fwd_chk[nSite] ;
	int nLock_2 = st_io.i_distribute_rbt1_top_vacuum_bwd_chk[nSite];

//	if( g_ioMgr.get_in_bit( nLock_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nLock_2 ) != !iOnOff) return false;

	return true;
}

bool CRun_Barcode_Works::CheckM1BtmVac( int nSite, int iOnOff )
{
	int nLock_1 = st_io.i_distribute_rbt1_btm_vacuum_fwd_chk[nSite];
	int nLock_2 = st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[nSite];

//	if( g_ioMgr.get_in_bit( nLock_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nLock_2 ) != !iOnOff) return false;

	return true;
}

bool CRun_Barcode_Works::CheckM2TopVac( int nSite, int iOnOff )
{
	int nLock_1 = st_io.i_distribute_rbt2_top_vacuum_fwd_chk[nSite] ;
	int nLock_2 = st_io.i_distribute_rbt2_top_vacuum_bwd_chk[nSite];

//	if( g_ioMgr.get_in_bit( nLock_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nLock_2 ) != !iOnOff) return false;

	return true;
}

bool CRun_Barcode_Works::CheckM2BtmVac( int nSite, int iOnOff )
{
	int nLock_1 = st_io.i_distribute_rbt2_btm_vacuum_fwd_chk[nSite] ;
	int nLock_2 = st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[nSite];

//	if( g_ioMgr.get_in_bit( nLock_1 ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nLock_2 ) != !iOnOff) return false;

	return true;
}

void CRun_Barcode_Works::SetBoxUpDn( int nSite, int iOnOff )
{
	int nfwd = st_io.o_top_box_up_clamp[nSite];
	int nbwd = st_io.o_top_box_dn_clamp[nSite];

	g_ioMgr.set_out_bit( nfwd, iOnOff);
	g_ioMgr.set_out_bit( nbwd, !iOnOff);
}

bool CRun_Barcode_Works::CheckBoxUpDn( int nSite, int iOnOff )
{
	int ifwd = st_io.i_top_box_up_clamp_chk[nSite] ;
	int ibwd = st_io.i_top_box_dn_clamp_chk[nSite];

	if( g_ioMgr.get_in_bit( ifwd ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( ibwd ) != !iOnOff) return false;

	return true;
}

void CRun_Barcode_Works::Run_Move_Box_UpDn( int nSite)
{
	if( m_fbboxupdn[nSite] == FB_NONE ) return;

	int iOnOff = IO_ON;
	if( m_fbboxupdn[nSite] == BACKWARD )
		iOnOff = IO_OFF;

	switch(m_step_box_updn[nSite])
	{
	case ESTEP_CYL_START: 
		m_step_box_updn[nSite] = ESTEP_CYL_MOVE; 
		mn_retry_cnt = 0;//2015.0218
		break;

	case 5:
		{
			if( GetCurrentTime() - m_dwTimeBoxupdnStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_GUIDE_FWDBWD] )
			{
				m_step_box_updn[nSite] = ESTEP_CYL_MOVE;
			}
			else if( GetCurrentTime() - m_dwTimeBoxupdn[nSite] <= 0 )
			{
				m_dwTimeBoxupdn[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_MOVE:
		{
			SetBoxUpDn( nSite, iOnOff );
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_box_updn[nSite] = iOnOff;
			}
			m_step_box_updn[nSite] = ESTEP_CYL_CHK;
			m_dwTimeBoxupdnStart[nSite] = GetCurrentTime();
		}
		break;


	case ESTEP_CYL_CHK:
		{
			if(CheckBoxUpDn( nSite, iOnOff))
			{
				m_step_box_updn[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeBoxupdn[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeBoxupdnStart[nSite] > (DWORD)st_time.n_limit_time[E_WAIT_BCR_GUIDE_FWDBWD] )
			{
				if(COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					////2015.0218
// 					CString strJamCode;
// 					strJamCode.Format("5%04d%d", st_io.i_top_box_up_clamp_chk[nSite], iOnOff);
// 					CTL_Lib.Alarm_Error_Occurrence( 219, dWARNING, strJamCode );
 
					mn_retry_cnt++;
					if(mn_retry_cnt > st_basic.mn_network_retry_cnt)
					{
						mn_retry_cnt = 0;

						CString strJamCode;
						strJamCode.Format("5%04d%d", st_io.i_top_box_up_clamp_chk[nSite], iOnOff);
						CTL_Lib.Alarm_Error_Occurrence( 219, dWARNING, strJamCode );
					}
					else
					{
						SetBoxUpDn( nSite, !iOnOff );
						m_dwTimeBoxupdn[nSite] = GetCurrentTime();
						m_step_box_updn[nSite] = ENUM_STEP_CYL(5);
					}
					////
				}
			}
			else if( GetCurrentTime() - m_dwTimeBoxupdn[nSite] <= 0 )
			{
				m_dwTimeBoxupdn[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeBoxupdn[nSite] < 0 ) m_dwTimeBoxupdn[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeBoxupdn[nSite] < st_time.n_wait_time[E_WAIT_BCR_GUIDE_FWDBWD][iOnOff] )
				return;

			m_step_box_updn[nSite] = ESTEP_CYL_START;
			m_fbboxupdn[nSite] = FB_NONE;
		}
		break;
	}
}

void CRun_Barcode_Works::Run_Move_M1_Top_Vacuum( int nSite)
{
	if(m_fbm1topvac[nSite] == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbm1topvac[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m1topvac[nSite])
	{
	case ESTEP_CYL_START: m_step_m1topvac[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM1TopVac( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_m1top_vac_fwdback[nSite] = iOnOff;
			}
			m_step_m1topvac[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM1TopVacStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM1TopVac( nSite, iOnOff ) )
			{
				m_step_m1topvac[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM1TopVac[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM1TopVacStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_VACUUM_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 220, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeM1TopVacStart[nSite] < 0 )
			{
				m_dwTimeM1TopVacStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM1TopVac[nSite] < 0 )
				m_dwTimeM1TopVac[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM1TopVac[nSite] < st_time.n_wait_time[E_WAIT_BCR_VACUUM_FWDBWD][iOnOff] )
				return;

			m_step_m1topvac[nSite] = ESTEP_CYL_START;
			m_fbm1topvac[nSite] = FB_NONE;
		}
		break;	
	}

}

void CRun_Barcode_Works::Run_Move_M2_Top_Vacuum( int nSite)
{
	if(m_fbm2topvac[nSite] == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbm2topvac[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m2topvac[nSite])
	{
	case ESTEP_CYL_START: m_step_m2topvac[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM2TopVac( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_m2top_vac_fwdback[nSite] = iOnOff;
			}
			m_step_m2topvac[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM2TopVacStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM2TopVac( nSite, iOnOff ) )
			{
				m_step_m2topvac[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM2TopVac[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM2TopVacStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_VACUUM_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_top_vacuum_fwd_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 221, dWARNING, strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeM2TopVacStart[nSite] < 0 )
			{
				m_dwTimeM2TopVacStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM2TopVac[nSite] < 0 )
				m_dwTimeM2TopVac[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM1TopVac[nSite] < st_time.n_wait_time[E_WAIT_BCR_VACUUM_FWDBWD][iOnOff] )
				return;

			m_step_m2topvac[nSite] = ESTEP_CYL_START;
			m_fbm2topvac[nSite] = FB_NONE;
		}
		break;	
	}

}

void CRun_Barcode_Works::Run_Move_M1_Btm_Vacuum( int nSite)
{
	if(m_fbm1btmvac[nSite] == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbm1btmvac[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m1btmvac[nSite])
	{
	case ESTEP_CYL_START: m_step_m1btmvac[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM1BtmVac( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_m1btm_vac_fwdback[nSite] = iOnOff;
			}
			m_step_m1btmvac[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM1BtmVacStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM1BtmVac( nSite, iOnOff ) )
			{
				m_step_m1btmvac[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM1BtmVac[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM1BtmVacStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_VACUUM_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 297, dWARNING, strJamCode );
					if(st_var.n_tboxmv_forming_cv_mode == CTL_YES)
					{
						SetM1BtmVac( nSite, IO_OFF);//2015.0915
						st_var.n_tboxmv_forming_cv_mode = CTL_NO;
						m_step_m1btmvac[nSite] = ESTEP_CYL_FINISH;//2015.0331
					}
				}
			}
			else if( GetCurrentTime() - m_dwTimeM1BtmVacStart[nSite] < 0 )
			{
				m_dwTimeM1BtmVacStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM1BtmVac[nSite] < 0 )
				m_dwTimeM1BtmVac[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM1BtmVac[nSite] < st_time.n_wait_time[E_WAIT_BCR_VACUUM_FWDBWD][iOnOff] )
				return;

			m_step_m1btmvac[nSite] = ESTEP_CYL_START;
			m_fbm1btmvac[nSite] = FB_NONE;
		}
		break;	
	}

}

void CRun_Barcode_Works::Run_Move_M2_Btm_Vacuum( int nSite)
{
	if(m_fbm2btmvac[nSite] == FB_NONE) return;
	int iOnOff = IO_ON;
	if( m_fbm2btmvac[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m2btmvac[nSite])
	{
	case ESTEP_CYL_START: m_step_m2btmvac[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM2BtmVac( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_bcr_m2btm_vac_fwdback[nSite] = iOnOff;
			}
			m_step_m2btmvac[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM2BtmVacStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM2BtmVac( nSite, iOnOff ) )
			{
				m_step_m2btmvac[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM2BtmVac[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM2BtmVacStart[nSite] > st_time.n_limit_time[E_WAIT_BCR_VACUUM_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[nSite], iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 224, dWARNING, strJamCode );
					if(st_var.n_tboxmv_sealing_cv_mode == CTL_YES)
					{
						SetM2BtmVac( nSite, IO_OFF);//2015.0915
						st_var.n_tboxmv_sealing_cv_mode = CTL_NO;
						m_step_m2btmvac[nSite] = ESTEP_CYL_FINISH;//2015.0331
					}
				}
			}
			else if( GetCurrentTime() - m_dwTimeM2BtmVacStart[nSite] < 0 )
			{
				m_dwTimeM2BtmVacStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM2BtmVac[nSite] < 0 )
				m_dwTimeM2BtmVac[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM2BtmVac[nSite] < st_time.n_wait_time[E_WAIT_BCR_VACUUM_FWDBWD][iOnOff] )
				return;

			m_step_m2btmvac[nSite] = ESTEP_CYL_START;
			m_fbm2btmvac[nSite] = FB_NONE;
		}
		break;	
	}
}

//void CRun_Barcode_Works::SetRollingFwdBwd( int nSite, int iOnOff )
//{
//	int nRoll = st_io.o_rolling_push_fwdback[nSite];
//
//	g_ioMgr.set_out_bit( nRoll, iOnOff);
//}

void CRun_Barcode_Works::SetRollingFwdBwd( int nSite, int pos, int iOnOff )
{
	int nRoll = st_io.o_rolling_push_fwdback[nSite][pos];

	g_ioMgr.set_out_bit( nRoll, iOnOff);
}

bool CRun_Barcode_Works::CheckRollingFwdBwd( int nSite, int nPos, int iOnOff )
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

//void CRun_Barcode_Works::Run_Move_RollerFwdBwd(int nSite)
//{
//	if(m_fbrollingfwdbwd[nSite] == FB_NONE) return;//2014.1018 FU_NONE -> FB_NONE
//	
//	int iOnOff = IO_ON;
//	
//	if( m_fbrollingfwdbwd[nSite] == BACKWARD)
//		iOnOff = IO_OFF;
//
//	switch(m_step_rolling_fwdbwd[nSite])
//	{
//	case ESTEP_CYL_START: m_step_rolling_fwdbwd[nSite] = ESTEP_CYL_MOVE;	break;
//	case ESTEP_CYL_MOVE:
//		{
//			SetRollingFwdBwd( nSite, iOnOff);
//			if(COMI.mn_run_status == dRUN){
//				st_work.mn_rolling_fwdback[nSite] = iOnOff;
//			}
//			m_step_rolling_fwdbwd[nSite] = ESTEP_CYL_CHK;
//			m_dwTimeRollingFwdBwdStart[nSite] = GetCurrentTime();
//		}
//		break;
//
//	case ESTEP_CYL_CHK:
//		{
//			if( st_handler.mn_virtual_mode == 1 || CheckRollingFwdBwd( nSite, iOnOff ) )
//			{
//				m_step_rolling_fwdbwd[nSite] = ESTEP_CYL_FINISH;
//				m_dwTimeRollingFwdBwd[nSite] = GetCurrentTime();
//			}
//			else if( GetCurrentTime() - m_dwTimeRollingFwdBwdStart[nSite] > st_time.n_limit_time[E_WAIT_ROLLING_FWDBWD] )
//			{
//				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
//				{
//					CString strJamCode = GetJamCode_Rolling_FwdBwd_Time(st_io.i_rolling_push_fwd_chk[nSite][ERLP_LEFT], iOnOff );
//					CTL_Lib.Alarm_Error_Occurrence( 225, dWARNING, strJamCode );
//				}
//			}
//			else if( GetCurrentTime() - m_dwTimeRollingFwdBwdStart[nSite] < 0 )
//			{
//				m_dwTimeRollingFwdBwdStart[nSite] = GetCurrentTime();
//			}
//		}
//		break;
//
//	case ESTEP_CYL_FINISH:
//		{
//			if( GetCurrentTime() - m_dwTimeRollingFwdBwd[nSite] < 0 )
//				m_dwTimeRollingFwdBwd[nSite] = GetCurrentTime();
//			else if( GetCurrentTime() - m_dwTimeRollingFwdBwd[nSite] < st_time.n_wait_time[E_WAIT_ROLLING_FWDBWD][iOnOff] )
//				return;
//
//			m_step_rolling_fwdbwd[nSite] = ESTEP_CYL_START;
//			m_fbrollingfwdbwd[nSite] = FB_NONE;
//		}
//		break;	
//	}
//}

void CRun_Barcode_Works::Run_Move_RollerFwdBwd(int nSite, int nPos)
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

void CRun_Barcode_Works::ReqM1RotateFwdBwd( int nSite, EFB efb)
{
	m_fbm1rotatefwdbwd[nSite] = efb;
	m_step_m1rotate_fwdbwd[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Works::SetM1RotateFwdBwd( int nSite, int iOnOff )
{
	int ofwd = st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[nSite];
	int obwd = st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[nSite];

	g_ioMgr.set_out_bit( ofwd, iOnOff);
	g_ioMgr.set_out_bit( obwd, !iOnOff);
}

bool CRun_Barcode_Works::CheckM1RotateFwdBwd( int nSite, int iOnOff )
{
	int nfwd = st_io.i_distribute_rbt1_box_rotator_fwd_chk[nSite] ;
	int nbwd = st_io.i_distribute_rbt1_box_rotator_bwd_chk[nSite];

	if( g_ioMgr.get_in_bit( nfwd ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nbwd ) != !iOnOff) return false;

	return true;
}

void CRun_Barcode_Works::Run_Move_M1_RollerFwdBwd(int nSite)
{
	if(m_fbm1rotatefwdbwd[nSite] == FB_NONE) return;//2014.1018 FU_NONE -> FB_NONE
	
	int iOnOff = IO_ON;	
	if( m_fbm1rotatefwdbwd[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m1rotate_fwdbwd[nSite])
	{
	case ESTEP_CYL_START: m_step_m1rotate_fwdbwd[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM1RotateFwdBwd( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_m1rotate_fwdback[nSite] = iOnOff;
			}
			m_step_m1rotate_fwdbwd[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM1RotatefwdbwdStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM1RotateFwdBwd( nSite, iOnOff ) )
			{
				m_step_m1rotate_fwdbwd[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM1Rotatefwdbwd[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM1RotatefwdbwdStart[nSite] > st_time.n_limit_time[E_WAIT_ROLLING_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strErr;
					strErr.Format("5%04d%d",st_io.i_distribute_rbt1_box_rotator_fwd_chk[nSite],iOnOff);
					CTL_Lib.Alarm_Error_Occurrence( 226, dWARNING, strErr );
				}
			}
			else if( GetCurrentTime() - m_dwTimeM1RotatefwdbwdStart[nSite] < 0 )
			{
				m_dwTimeM1RotatefwdbwdStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM1Rotatefwdbwd[nSite] < 0 )
				m_dwTimeM1Rotatefwdbwd[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM1Rotatefwdbwd[nSite] < st_time.n_wait_time[E_WAIT_ROLLING_FWDBWD][iOnOff] )
				return;

			m_step_m1rotate_fwdbwd[nSite] = ESTEP_CYL_START;
			m_fbm1rotatefwdbwd[nSite] = FB_NONE;
		}
		break;	
	}
}


void CRun_Barcode_Works::ReqM2RotateFwdBwd( int nSite, EFB efb)
{
	m_fbm2rotatefwdbwd[nSite] = efb;
	m_step_m2rotate_fwdbwd[nSite] = ESTEP_CYL_START;
}

void CRun_Barcode_Works::SetM2RotateFwdBwd( int nSite, int iOnOff )
{
	int ofwd = st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[nSite];
	int obwd = st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[nSite];

	g_ioMgr.set_out_bit( ofwd, iOnOff);
	g_ioMgr.set_out_bit( obwd, !iOnOff);
}

bool CRun_Barcode_Works::CheckM2RotateFwdBwd( int nSite, int iOnOff )
{
	int nfwd = st_io.i_distribute_rbt2_box_rotator_fwd_chk[nSite] ;
	int nbwd = st_io.i_distribute_rbt2_box_rotator_bwd_chk[nSite];

	if( g_ioMgr.get_in_bit( nfwd ) != iOnOff) return false;
	if( g_ioMgr.get_in_bit( nbwd ) != !iOnOff) return false;

	return true;
}

void CRun_Barcode_Works::Run_Move_M2_RollerFwdBwd(int nSite)
{
	if(m_fbm2rotatefwdbwd[nSite] == FB_NONE) return;//2014.1018 FU_NONE -> FB_NONE
	
	int iOnOff = IO_ON;	
	if( m_fbm2rotatefwdbwd[nSite] == BACKWARD)
		iOnOff = IO_OFF;

	switch(m_step_m2rotate_fwdbwd[nSite])
	{
	case ESTEP_CYL_START: m_step_m2rotate_fwdbwd[nSite] = ESTEP_CYL_MOVE;	break;
	case ESTEP_CYL_MOVE:
		{
			SetM2RotateFwdBwd( nSite, iOnOff);
			if(COMI.mn_run_status == dRUN){
				st_work.mn_m2rotate_fwdback[nSite] = iOnOff;
			}
			m_step_m2rotate_fwdbwd[nSite] = ESTEP_CYL_CHK;
			m_dwTimeM2RotatefwdbwdStart[nSite] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			if( st_handler.mn_virtual_mode == 1 || CheckM2RotateFwdBwd( nSite, iOnOff ) )
			{
				m_step_m2rotate_fwdbwd[nSite] = ESTEP_CYL_FINISH;
				m_dwTimeM2Rotatefwdbwd[nSite] = GetCurrentTime();
			}
			else if( GetCurrentTime() - m_dwTimeM2RotatefwdbwdStart[nSite] > st_time.n_limit_time[E_WAIT_ROLLING_FWDBWD] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strErr;
					strErr.Format("5%04d%d",st_io.i_distribute_rbt2_box_rotator_fwd_chk[nSite],iOnOff);
					CTL_Lib.Alarm_Error_Occurrence( 227, dWARNING, strErr );
				}
			}
			else if( GetCurrentTime() - m_dwTimeM2RotatefwdbwdStart[nSite] < 0 )
			{
				m_dwTimeM2RotatefwdbwdStart[nSite] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeM2Rotatefwdbwd[nSite] < 0 )
				m_dwTimeM2Rotatefwdbwd[nSite] = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeM2Rotatefwdbwd[nSite] < st_time.n_wait_time[E_WAIT_ROLLING_FWDBWD][iOnOff] )
				return;

			m_step_m2rotate_fwdbwd[nSite] = ESTEP_CYL_START;
			m_fbm2rotatefwdbwd[nSite] = FB_NONE;
		}
		break;	
	}
}

void CRun_Barcode_Works::EBCRI_Finish()
{
// 	if(m_fbboxupdn[0] != FB_NONE ) break;
// 	if(m_fbboxupdn[1] != FB_NONE ) break;

	Func.OnInitBtn( RBTN_BCR_ATTACH );//2014.1018
	st_handler.mn_init_state[INIT_BARCODE_ROBOT] = CTL_YES;
	mn_InitStep = EBCRI_START;
}

bool CRun_Barcode_Works::Run_Bcr_Move_Chk1_Jam( stJobPicker job )
{
	CString strJamCode;

	Func.ThreadFunctionStepTrace(20, job.bcr1_step);
	switch( job.bcr1_step )
	{
	case BCR1_NONE:
		break;

	case BCR1_PICK_LABEL_ROTATE_CHK:
	case BCR1_PICK_R1_SAFETY:
	case BCR1_PICK_R2_SAFETY:
	case BCR1_PICK_LABEL_MOVE_READY_POS:
	case BCR1_PICK_LABEL_MOVE_READY:
// 		if( st_basic.n_mode_device && Chk_Robot1_vacuum_fwdbwd( 0, IO_OFF ) == false )		return false;
// 		if( st_basic.n_mode_device && Chk_Robot1_vacuum_fwdbwd( 1, IO_OFF ) == false )		return false;
// 		if( st_basic.n_mode_device && Chk_Robot_rollingfwdbwd( 1, IO_OFF ) == false )			return false;
//		if( Chk_Robot1_guide_updn( 0, IO_ON ) == false )			return false;//guide up
//		if( Chk_Robot1_guide_updn( 1, IO_ON ) == false )			return false;
		break;

	case BCR1_PICK_LABEL_DOWN_CHK:
	case BCR1_PICK_LABEL_VACUUM:
	case BCR1_PICK_LABEL_UP:
	case BCR1_PICK_LABEL_VACUUM_CHK:
	case BCR1_PICK_LABEL_FINISH:
		if( st_basic.n_mode_device && Chk_Robot_rollingfwdbwd( 0, IO_OFF ) == false )			return false;
		break;

	case BCR1_PLACE_LABEL_PUSH_VACUUM:
// 		if( job.iRobotFirst != -1)
// 			if( st_basic.n_mode_device && Chk_Robot1_vacuum_fwdbwd( 0, IO_ON ) == false )			return false;
// 
// 		if(st_basic.n_mode_device && job.iRobotSecond != -1)
// 			if( st_basic.n_mode_device && Chk_Robot1_vacuum_fwdbwd( 1, IO_ON ) == false )			return false;
//		if( Chk_Robot1_guide_updn( 0, IO_OFF ) == false )			return false;//guide down
//		if( Chk_Robot1_guide_updn( 1, IO_OFF ) == false )			return false;
		break;

	case BCR1_PLACE_LABEL_PULL_CHK:
// 		if( job.iRobotFirst != -1)
// 			if( st_basic.n_mode_device && Chk_Robot1_vacuum_fwdbwd( 0, IO_OFF ) == false )		return false;
// 		if( st_basic.n_mode_device && job.iRobotSecond != -1)
// 			if( st_basic.n_mode_device && Chk_Robot1_vacuum_fwdbwd( 1, IO_OFF ) == false )		return false;
// 		if( st_basic.n_mode_device && Chk_Robot_rollingfwdbwd( 1, IO_ON ) == false )			return false;
//		if( Chk_Robot1_guide_updn( 0, IO_ON ) == false )			return false;
//		if( Chk_Robot1_guide_updn( 1, IO_ON ) == false )			return false;
		break;

// 	case BCR1_PLACE_LABEL_SAFETY:
	case BCR1_PLACE_LABEL_FINISH:	
// 		if( st_basic.n_mode_device && Chk_Robot1_guide_fwdbwd( 0, IO_OFF ) == false )		return false;
// 		if( st_basic.n_mode_device && Chk_Robot1_guide_fwdbwd( 1, IO_OFF ) == false )		return false;
		if( st_basic.n_mode_device && Chk_Robot_rollingfwdbwd( 0, IO_OFF ) == false )			return false;
//		if( Chk_Robot1_guide_updn( 0, IO_ON ) == false )			return false;//guide up
//		if( Chk_Robot1_guide_updn( 1, IO_ON ) == false )			return false;
		break;
	}

	return true;
}

bool CRun_Barcode_Works::Chk_Robot1_vacuum_fwdbwd( int nSite, int iOn )
{
	if( /*g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_fwd_chk[nSite], iOn ) == iOn &&*/
		g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[nSite], !iOn ) == !iOn &&
		/*g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_fwd_chk[nSite], iOn ) == iOn &&*/
		g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[nSite], !iOn ) == !iOn)
	{
		return true;
	}

	CString strJamCode;
	strJamCode.Format("5%04d%d",st_io.i_distribute_rbt1_top_vacuum_fwd_chk[nSite], iOn);
	CTL_Lib.Alarm_Error_Occurrence( 230, dWARNING, strJamCode );
	return false;
}

bool CRun_Barcode_Works::Chk_Robot1_guide_fwdbwd( int nSite, int iOn )
{
	if(/* g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_box_guide_fwd_chk[nSite], iOn ) == iOn &&*/
		g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_box_guide_back_chk[nSite], !iOn ) == !iOn)
	{
		return true;
	}

	CString strJamCode;
	strJamCode.Format("5%04d%d",st_io.i_distribute_rbt1_box_guide_back_chk[nSite], iOn);
	CTL_Lib.Alarm_Error_Occurrence( 231, dWARNING, strJamCode );
	return false;
}

//bool CRun_Barcode_Works::Chk_Robot1_guide_updn( int nSite, int iOn )
//{
//	if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_box_guide_up_chk[nSite], iOn ) == iOn &&
//		g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_box_guide_down_chk[nSite], !iOn ) == !iOn)
//	{
//		return true;
//	}
//
//	CString strJamCode;
//	strJamCode.Format("5%04d%d",st_io.i_distribute_rbt1_box_guide_up_chk[nSite], iOn);
//	CTL_Lib.Alarm_Error_Occurrence( 232, dWARNING, strJamCode );
//	return false;
//}

bool CRun_Barcode_Works::Chk_Robot_rollingfwdbwd( int nSite, int iOn )
{
	if( g_ioMgr.get_in_bit( st_io.i_rolling_push_fwd_chk[nSite][ERLP_LEFT], iOn ) == iOn &&
		g_ioMgr.get_in_bit( st_io.i_rolling_push_fwd_chk[nSite][ERLP_RIGHT], iOn ) == iOn &&
		g_ioMgr.get_in_bit( st_io.i_rolling_push_bwd_chk[nSite][ERLP_LEFT], !iOn ) == !iOn &&
		g_ioMgr.get_in_bit( st_io.i_rolling_push_bwd_chk[nSite][ERLP_RIGHT], !iOn ) == !iOn )
	{
		return true;
	}

	CString strJamCode;
	strJamCode.Format("5%04d%d",st_io.i_rolling_push_fwd_chk[nSite][ERLP_LEFT], iOn);
	CTL_Lib.Alarm_Error_Occurrence( 233, dWARNING, strJamCode );
	return false;
}



bool CRun_Barcode_Works::Chk_Robot2_vacuum_fwdbwd( int nSite, int iOn )
{
	if( /*g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_fwd_chk[nSite], iOn ) == iOn &&*/
		g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[nSite], iOn ) == iOn &&
		/*g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_fwd_chk[nSite], iOn ) == iOn &&*/
		g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[nSite], iOn ) == iOn)
	{
		return true;
	}

	CString strJamCode;
	strJamCode.Format("5%04d%d",st_io.i_distribute_rbt2_top_vacuum_fwd_chk[nSite], iOn);
	CTL_Lib.Alarm_Error_Occurrence( 234, dWARNING, strJamCode );
	return false;
}


bool CRun_Barcode_Works::Chk_Robot2_guide_fwdbwd( int nSite, int iOn )
{
	if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_box_guide_fwd_chk[nSite], iOn ) == iOn &&
		g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_box_guide_back_chk[nSite], iOn ) == iOn)
	{
		return true;
	}

	CString strJamCode;
	strJamCode.Format("5%04d%d",st_io.i_distribute_rbt2_box_guide_fwd_chk[nSite], iOn);
	CTL_Lib.Alarm_Error_Occurrence( 235, dWARNING, strJamCode );
	return false;
}

//bool CRun_Barcode_Works::Chk_Robot2_guide_updn( int nSite, int iOn )
//{
//	if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_box_guide_up_chk[nSite], iOn ) == iOn &&
//		g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_box_guide_down_chk[nSite], iOn ) == iOn)
//	{
//		return true;
//	}
//
//	CString strJamCode;
//	strJamCode.Format("5%04d%d",st_io.i_distribute_rbt2_box_guide_up_chk[nSite], iOn);
//	CTL_Lib.Alarm_Error_Occurrence( 236, dWARNING, strJamCode );
//	return false;
//}


bool CRun_Barcode_Works::Run_Bcr_Move_Chk1_Time()
{
	switch( bcr1_step_old )
	{
	case BCR1_PICK_Z1_SAFETY:
 		if( InTimeRobot1( 10000 ) == false )
 		{//180006 1 00 "Distribute#1 Attach Z#1 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 240, dWARNING, "180006" );
			m_dwTime_Robot1 = GetCurrentTime();
			return false;
 		}
		break;
	case BCR1_PICK_Z2_SAFETY:
 		if( InTimeRobot1( 10000 ) == false )
 		{//200006 1 00 "Distribute#1 Attach Z#2 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 241, dWARNING, "200006" );
			m_dwTime_Robot1 = GetCurrentTime();
			return false;
 		}
		break;
	}

	return true;
}

bool CRun_Barcode_Works::InTimeRobot1( int iMilliSec )
{
	DWORD dwtime = GetCurrentTime();
	//2012,1220
	if( GetCurrentTime() - m_dwTime_Robot1 < 0 )
	{
		m_dwTime_Robot1 = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwTime_Robot1 < iMilliSec )
		return true;

	return false;
}

void CRun_Barcode_Works::Run_Move1()
{
	int nJobCnt = Func.m_pRobot[SITE_BARCODE1]->GetJobCount();
	if( nJobCnt == 0 )
		return;

//  	Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();


	stJobPicker& job = Func.m_pRobot[SITE_BARCODE1]->GetFirstJob();

	if( bcr1_step_old != job.bcr1_step)
	{
		bcr1_step_old = job.bcr1_step;
		m_dwTime_Robot1 = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
		if( Run_Bcr_Move_Chk1_Jam( job ) == false )
			return;
	}
	
	if( Run_Bcr_Move_Chk1_Time() == false )
		return;

	Func.ThreadFunctionStepTrace(21, job.bcr1_step);
	switch( job.bcr1_step )
	{
	case BCR1_NONE:	break;

	case BCR1_PICK_LABEL_READY:
		{
			double dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_Z1);
			if( dPos > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_UP] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
			{
				job.bcr1_step = BCR1_PICK_Z1_SAFETY;
				break;
			}
			dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_Z2);
			if( dPos > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_UP] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
			{
				job.bcr1_step = BCR1_PICK_Z2_SAFETY;
				break;
			}

			job.bcr1_step = BCR1_PICK_LABEL_TOP_CHK;
			
			m_dwTimeTact = GetCurrentTime();
			
		}
		break;

	case BCR1_PICK_Z1_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr1_step = BCR1_PICK_LABEL_PICK_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 250, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR1_PICK_Z2_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr1_step = BCR1_PICK_LABEL_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 251, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR1_PICK_LABEL_TOP_CHK:
		{
			int nLock_1 = st_io.i_distribute_rbt1_top_vacuum_fwd_chk[0];
			int nLock_2 = st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0];

			if( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON  )
			{
				job.bcr1_step = BCR1_PICK_LABEL_TOP_LEFT_BACKWPRD;
				break;
			}
			nLock_1 = st_io.i_distribute_rbt1_top_vacuum_fwd_chk[1] ;
			nLock_2 = st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1];

			if( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON )
			{
				job.bcr1_step = BCR1_PICK_LABEL_TOP_RIGHT_BACKWPRD;
				break;
			}

			job.bcr1_step = BCR1_PICK_LABEL_BTM_CHK;

		}
		break;

	case BCR1_PICK_LABEL_TOP_LEFT_BACKWPRD:
		ReqM1TopVac( EBVS_LEFT , BACKWARD );
		ReqM1TopVac( EBVS_RIGHT , BACKWARD );
		job.bcr1_step = BCR1_PICK_LABEL_TOP_LEFT_BACKWPRD_CHK;
		break;

	case BCR1_PICK_LABEL_TOP_LEFT_BACKWPRD_CHK:
		if( GetM1TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr1_step = BCR1_PICK_LABEL_TOP_CHK;
		break;
		
	case BCR1_PICK_LABEL_TOP_RIGHT_BACKWPRD:
		ReqM1TopVac( EBVS_RIGHT, BACKWARD );
		job.bcr1_step = BCR1_PICK_LABEL_TOP_RIGHT_BACKWPRD_CHK;
		break;

	case BCR1_PICK_LABEL_TOP_RIGHT_BACKWPRD_CHK:
		if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr1_step = BCR1_PICK_LABEL_TOP_CHK;
		break;

	case BCR1_PICK_LABEL_BTM_CHK:
		{
			int nLock_1 = st_io.i_distribute_rbt1_btm_vacuum_fwd_chk[0];
			int nLock_2 = st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0];

			if( ( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON ))
			{
				job.bcr1_step = BCR1_PICK_LABEL_BTM_LEFT_BACKWPRD;
				break;
			}
			nLock_1 = st_io.i_distribute_rbt1_btm_vacuum_fwd_chk[1] ;
			nLock_2 = st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1];

			if( ( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON ) )
			{
				job.bcr1_step = BCR1_PICK_LABEL_BTM_RIGHT_BACKWPRD;
				break;
			}

			job.bcr1_step = BCR1_PICK_LABEL_ROTATE_CHK;

		}
		break;

	case BCR1_PICK_LABEL_BTM_LEFT_BACKWPRD:
		ReqM1BtmVac( EBVS_LEFT , BACKWARD );
		ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
		job.bcr1_step = BCR1_PICK_LABEL_BTM_LEFT_BACKWPRD_CHK;
		break;

	case BCR1_PICK_LABEL_BTM_LEFT_BACKWPRD_CHK:
		if( GetM1BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr1_step = BCR1_PICK_LABEL_BTM_CHK;
		break;

	case BCR1_PICK_LABEL_BTM_RIGHT_BACKWPRD:
 		ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
		job.bcr1_step = BCR1_PICK_LABEL_BTM_RIGHT_BACKWPRD_CHK;
		break;

	case BCR1_PICK_LABEL_BTM_RIGHT_BACKWPRD_CHK:
		if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr1_step = BCR1_PICK_LABEL_BTM_CHK;
		break;

	case BCR1_PICK_LABEL_ROTATE_CHK:
		{
//			double dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_R1);
//			if( dPos > st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_R1] )
//			{
//				job.bcr1_step = BCR1_PICK_R1_SAFETY;
//				break;
//			}
//			dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_R2);
//			if( dPos > st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_R2] )
//			{
//				job.bcr1_step = BCR1_PICK_R2_SAFETY;
//				break;
//			}
//
//			job.bcr1_step = BCR1_PICK_LABEL_MOVE_READY_POS;		
			mn_mot_flag[0][0] = 0; mn_mot_flag[0][1] = 0;
			job.bcr1_step = BCR1_PICK_R1_SAFETY;
		
		}
		break;

	case BCR1_PICK_R1_SAFETY://2014,1205
		{
// 			int nRet1,nRet2;
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R1, st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD ) mn_mot_flag[0][0] = 1;
// 			if( nRet2 == BD_GOOD ) mn_mot_flag[0][1] = 1;
// 			if( mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1 )
// 			{
// 				job.bcr1_step = BCR1_PICK_LABEL_MOVE_READY_POS;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 252, CTL_dWARNING, alarm.mstr_code );
// 			}
//			for(int i=0; i<2; i++)
//			{
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[i], IO_OFF);
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[i], IO_ON);
//			}
			ReqM1RotateFwdBwd( 0 ,BACKWARD);
			ReqM1RotateFwdBwd( 1 ,BACKWARD);
			job.bcr1_step = BCR1_PICK_R2_SAFETY;
     		m_dwTime_Robot1 = GetCurrentTime();
		}
		break;

	case BCR1_PICK_R2_SAFETY://2014,1205
		{
// 			int nRet1;
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			if(1 || nRet1 == BD_GOOD)
// 			{
// 				job.bcr1_step = BCR1_PICK_LABEL_ROTATE_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 253, CTL_dWARNING, alarm.mstr_code );
// 			}
 			int nRet1,nRet2,nRet3,nRet4;
//			if( GetCurrentTime() - m_dwTime_Robot1 < 0 ) m_dwTime_Robot1 = GetCurrentTime();

			if( m_fbm1rotatefwdbwd[0] != FB_NONE) break;
			if( m_fbm1rotatefwdbwd[1] != FB_NONE) break;
			
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_OFF);//place position
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_ON); //pick position
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_ON);
			
			if(nRet1 == IO_OFF && nRet2 == IO_ON && nRet3 == IO_OFF && nRet4 == IO_ON)
			{
				//2015.0827
				mn_bcr_pick_error[0] = mn_bcr_pick_error[1] = CTL_NO;
				st_handler.mn_retest_cnt = 0;
				job.bcr1_step = BCR1_PICK_LABEL_MOVE_READY_POS;
			}
			else
			{
				job.bcr1_step = BCR1_PICK_R1_SAFETY;
			}
		}
		break;

	case BCR1_PICK_LABEL_MOVE_READY_POS:
		{
			double dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_X);
			int nRet_1 = COMI.Check_MotPosRange(M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], COMI.md_allow_value[M_BCR_ATTACH1_X]);//COMI.mn_allow_value[n_MotNum]
			if (nRet_1 != BD_GOOD) //YÃàÀÌ ÀÛ¾÷ °¡´ÉÇÑ À§Ä¡¿¡ ¿Í ÀÖ´ÂÁö È®ÀÎÇÑ´Ù 
			{
				job.bcr1_step = BCR1_PICK_LABEL_MOVE_READY;	//Ã³À½À¸·Î °¡¼­ ´Ù½Ã ½ÃÀÛÇÑ´Ù 		 
				break;
			}

			if( dPos > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				dPos < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_X])
			{
				job.bcr1_step = BCR1_PICK_LABEL_PICK_READY;
				break;
			}
			////2014,1216 ysj 
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[0], IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[1], IO_OFF);
			////
			job.bcr1_step = BCR1_PICK_LABEL_MOVE_READY;	
			
		}
		break;

	case BCR1_PICK_LABEL_MOVE_READY:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr1_step = BCR1_PICK_LABEL_PICK_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 254, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		

	case BCR1_PICK_LABEL_PICK_READY:
		{
			if( job.iRobotFirst != -1)
			{
					g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[job.iRobotFirst], IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_ON);//2014,1117

					ReqM1TopVac( job.iRobotFirst, BACKWARD);
// 					ReqM1BtmVac( job.iRobotFirst, BACKWARD);
			}
			if( job.iRobotSecond != -1)
			{
					g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[job.iRobotSecond], IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_ON);//2014,1117
					ReqM1TopVac( job.iRobotSecond, BACKWARD);
// 					ReqM1BtmVac( job.iRobotSecond, BACKWARD);
			}
			mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
			job.bcr1_step = BCR1_PICK_LABEL_DOWN;	
		}
		break;

	case BCR1_PICK_LABEL_DOWN:
		{
			int nRet1,nRet2;
			nRet1 = nRet2 = BD_ERROR;
			if( job.iRobotFirst != -1)
			{
				if( (st_handler.mn_retest_cnt == 0 && mn_bcr_pick_error[job.iRobotFirst] == CTL_NO) ||
					(st_handler.mn_retest_cnt >= 1 && mn_bcr_pick_error[job.iRobotFirst] == CTL_YES) )
				{
					nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_PICK], st_basic.nRunSpeed );
				}
				else
				{
					nRet1 = BD_GOOD;
				}
			}
			else
			{
				nRet1 = BD_GOOD;
			}
			if( job.iRobotSecond != -1)
			{				
				if( (st_handler.mn_retest_cnt == 0 && mn_bcr_pick_error[job.iRobotSecond] == CTL_NO) ||
					(st_handler.mn_retest_cnt >= 1 && mn_bcr_pick_error[job.iRobotSecond] == CTL_YES) )
				{
					nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_PICK], st_basic.nRunSpeed );
				}
				else
				{
					nRet2 = BD_GOOD;
				}
			}
			else
			{
				nRet2 = BD_GOOD;
			}
			if( nRet1 == BD_GOOD ) mn_mot_flag[0][0] = 1;
			if( nRet2 == BD_GOOD ) mn_mot_flag[0][1] = 1;
			if(mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1)
			{
				////2014.1120 ysj
				if( job.iRobotFirst != -1)
				{
					g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[job.iRobotFirst], IO_ON);
// 					g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[job.iRobotFirst], IO_ON);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_ON);//2014,1117
				}
				if( job.iRobotSecond != -1)
				{
					g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[job.iRobotSecond], IO_ON);
// 					g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[job.iRobotSecond], IO_ON);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_ON);//2014,1117
				}

				job.bcr1_step = BCR1_PICK_LABEL_DOWN_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 255, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR1_PICK_LABEL_DOWN_CHK:
		{
			if( job.iRobotFirst != -1)
			{
				if( GetM1TopVac( job.iRobotFirst ) != FB_NONE ) break;
// 				if( GetM1BtmVac( job.iRobotFirst ) != FB_NONE ) break;
			}
			if( job.iRobotSecond != -1)
			{
 				if( GetM1TopVac( job.iRobotSecond ) != FB_NONE ) break;
// 				if( GetM1BtmVac( job.iRobotSecond ) != FB_NONE ) break;
			}
			job.bcr1_step = BCR1_PICK_LABEL_VACUUM;	
		}
		break;

	case BCR1_PICK_LABEL_VACUUM:
		{
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[job.iRobotFirst], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[job.iRobotFirst], IO_ON);
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_ON);
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[job.iRobotSecond], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[job.iRobotSecond], IO_ON);
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_ON);
			}
			job.bcr1_step = BCR1_PICK_LABEL_VACUUM_CHK;	
			m_dwTime_Robot1 = GetCurrentTime();
		}
		break;

	case BCR1_PICK_LABEL_VACUUM_CHK:
		{
// 			if( GetCurrentTime() - m_dwTime_Robot1 < 0 ) m_dwTime_Robot1 = GetCurrentTime();
// 			if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_ON] ) break;

			
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_ON);

				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
				{
				}
				else
				{
					if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_limit_time[E_WAIT_BCR_PICK_VACUUM] ) break;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 256, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Top Vacuum Off Check Error.";
					}
					else
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
//						CString strError;
//						strError.Format("5%04d0",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst]);
//						CTL_Lib.Alarm_Error_Occurrence( 257, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Bottom Vacuum Off Check Error.";
					}
// 					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
//					break;

				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_ON);
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
				{
				}
				else
				{
					if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_limit_time[E_WAIT_BCR_PICK_VACUUM] ) break;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
//						CString strError;
//						strError.Format("5%04d0",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond]);
//						CTL_Lib.Alarm_Error_Occurrence( 258, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Top Vacuum Off Check Error.";
					}
					else
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
//						CString strError;
//						strError.Format("5%04d0",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond]);
//						CTL_Lib.Alarm_Error_Occurrence( 259, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Bottom Vacuum Off Check Error.";
					}
// 					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
//					break;

				}
			}
			m_dwTime_Robot1 = GetCurrentTime();
			mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
			job.bcr1_step = BCR1_PICK_LABEL_UP;	
		}
		break;

	case BCR1_PICK_LABEL_UP:
		{

			if( GetCurrentTime() - m_dwTime_Robot1 < 0 ) m_dwTime_Robot1 = GetCurrentTime();
			if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_ON] ) break;

			int nnSite = 0;
			if(job.iRobotFirst != -1) nnSite = job.iRobotFirst;
			else if(job.iRobotSecond != -1) nnSite = job.iRobotSecond;
			//2015.0120
			if( GetCurrentTime() - st_handler.m_lBcrOutTime[nnSite] < 0 ) st_handler.m_lBcrOutTime[nnSite]  = GetCurrentTime();
			if( GetCurrentTime() - st_handler.m_lBcrOutTime[nnSite] > 60000 ) 
			{
				if( GetCurrentTime() - m_dwTime_Robot1 < 0 ) m_dwTime_Robot1 = GetCurrentTime();
				if( GetCurrentTime() - m_dwTime_Robot1 < 3500 ) break;
			}
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2 , st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[0][1] = 1;
			if(mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1)
			{
				job.bcr1_step = BCR1_PICK_LABEL_UP_VACUUM_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR1_PICK_LABEL_UP_VACUUM_CHK:
		{
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);

				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
				{
				}
				else 
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
						if(st_handler.mn_retest_cnt >= 2 && mn_bcr_pick_error[job.iRobotFirst] == CTL_YES)
						{
							CString strError;
							strError.Format("5%04d0",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst]);
							CTL_Lib.Alarm_Error_Occurrence( 261, CTL_dWARNING, strError );
						}
						else
						{
							mn_bcr_pick_error[job.iRobotFirst] = CTL_YES;
							st_handler.mn_retest_cnt++;
							mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
							job.bcr1_step = BCR1_PICK_LABEL_DOWN;
							break;
						}
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Top Vacuum Off Check Error.";
					}
					else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
						CString strError;
						strError.Format("5%04d0",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst]);
						CTL_Lib.Alarm_Error_Occurrence( 262, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Bottom Vacuum Off Check Error.";
					}
// 					Func.OnSet_IO_Port_Stop();
//  					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					job.bcr1_step = BCR1_STEP(252);
					break;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
				{
				}
				else
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
						if(st_handler.mn_retest_cnt >= 2 && mn_bcr_pick_error[job.iRobotSecond] == CTL_YES)
						{
							CString strError;
							strError.Format("5%04d0",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond]);
							CTL_Lib.Alarm_Error_Occurrence( 263, CTL_dWARNING, strError );
						}
						else
						{
							mn_bcr_pick_error[job.iRobotSecond] = CTL_YES;
							st_handler.mn_retest_cnt++;
							mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
							job.bcr1_step = BCR1_PICK_LABEL_DOWN;
							break;
						}
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Top Vacuum Off Check Error.";
					}
					else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_OFF )
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
						CString strError;
						strError.Format("5%04d0",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond]);
						CTL_Lib.Alarm_Error_Occurrence( 264, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot1 Left Bottom Vacuum Off Check Error.";
					}
// 					Func.OnSet_IO_Port_Stop();
//  					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					job.bcr1_step = BCR1_STEP(252);
					break;

				}
			}
			job.bcr1_step = BCR1_PICK_LABEL_FINISH;


			if( job.iRobotFirst != -1)
			{
// 				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);
				ReqM1TopVac( job.iRobotFirst, BACKWARD);
				ReqM1BtmVac( job.iRobotFirst, BACKWARD);
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst], IO_OFF);
//				COMI.Start_SingleMove(M_BCR_FEEDER_1+job.iRobotFirst, st_motor[M_BCR_FEEDER_1+job.iRobotFirst].d_pos[P_BCR_FEEDER_PICK], st_basic.nRunSpeed);
			}
			if( job.iRobotSecond != -1)
			{
// 				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
				ReqM1TopVac( job.iRobotSecond, BACKWARD);
				ReqM1BtmVac( job.iRobotSecond, BACKWARD);
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotSecond], IO_OFF);
//				COMI.Start_SingleMove(M_BCR_FEEDER_1+job.iRobotSecond, st_motor[M_BCR_FEEDER_1+job.iRobotSecond].d_pos[P_BCR_FEEDER_PICK], st_basic.nRunSpeed);
			}
		}
		break;

	case 251:
		if( m_fbm1rotatefwdbwd[EBVS_LEFT] != FB_NONE) break;
		if( m_fbm1rotatefwdbwd[EBVS_RIGHT] != FB_NONE) break;
		job.bcr1_step = BCR1_STEP(252);
		break;

	case 252:
		{
			bool nfwd1 = CheckM1RotateFwdBwd( EBVS_LEFT, IO_OFF );
			bool nfwd2 = CheckM1RotateFwdBwd( EBVS_RIGHT, IO_OFF );
			if( nfwd1 == true && nfwd2 == true )
			{
				ReqM1TopVac( EBVS_LEFT , FORWARD );
				ReqM1TopVac( EBVS_RIGHT , FORWARD );
				ReqM1BtmVac( EBVS_LEFT , BACKWARD );
				ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
				job.bcr1_step = BCR1_STEP(253);			
			}
			else
			{
				ReqM1RotateFwdBwd( EBVS_LEFT ,BACKWARD);
				ReqM1RotateFwdBwd( EBVS_RIGHT ,BACKWARD);
				job.bcr1_step = BCR1_STEP(251);			
			}
		}
		break;

	case 253:
		if( GetM1TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
		job.bcr1_step = BCR1_STEP(254);
		break;

	case 254:
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				job.bcr1_step = BCR1_STEP(255);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 271, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 255:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2 , st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[0][1] = 1;
			if(mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_LEFT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_RIGHT], IO_OFF);
				job.bcr1_step = BCR1_STEP(256);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 256:
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_RIGHT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_RIGHT], IO_ON);
		m_dwTime_Robot1 = GetCurrentTime();
		job.bcr1_step = BCR1_STEP(260);
		break;

	case 260:
		if( GetCurrentTime() - m_dwTime_Robot1 < 0) m_dwTime_Robot1 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		mn_mot_flag[0][0] = 0;
		mn_mot_flag[0][1] = 0;
		job.bcr1_step = BCR1_STEP(261);
		break;

	case 261:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2 , st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[0][1] = 1;
			if(mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1)
			{

				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_LEFT], IO_OFF);				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_RIGHT], IO_OFF);

				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_RIGHT], IO_OFF);
				//Func.OnSet_IO_Port_Stop();
// 				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

				job.bcr1_step = BCR1_STEP(262);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 262:
		{
			ReqM1TopVac( EBVS_LEFT , BACKWARD );
			ReqM1TopVac( EBVS_RIGHT , BACKWARD );
			ReqM1BtmVac( EBVS_LEFT , BACKWARD );
			ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
			job.bcr1_step = BCR1_STEP(263);			
		}
		break;

	case 263:
		if( GetM1TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr1_step = BCR1_STEP(264);
		break;

	case 264:
		if( st_basic.n_mode_device && job.iRobotFirst != -1)
		{
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515081 1 00 "Distribution Label Robot1 Left Top Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 261, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515091 1 00 "Distribution Label Robot1 Left Bottom Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 262, CTL_dWARNING, strError );
				break;
			}
			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(265, dWARNING, strError);
				break;
			}
		}
		if( st_basic.n_mode_device && job.iRobotSecond != -1)
		{
			//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 263, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
			{
				//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 264, CTL_dWARNING, strError );
				break;
			}
			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotSecond], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotSecond], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(266, dWARNING, strError);				
				break;
			}
		}
		job.bcr1_step = BCR1_STEP(265);
		break;

	case 265:
		if( job.iRobotFirst != -1)
		{
			ReqM1TopVac( job.iRobotFirst, BACKWARD);
			ReqM1BtmVac( job.iRobotFirst, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst], IO_OFF);
		}
		if( job.iRobotSecond != -1)
		{
			ReqM1TopVac( job.iRobotSecond, BACKWARD);
			ReqM1BtmVac( job.iRobotSecond, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotSecond], IO_OFF);
		}
		job.bcr1_step = BCR1_STEP(266);
		break;

	case 266:
		{			
			if( job.iRobotFirst != -1)
			{
				if( GetM1TopVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM1BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);//2014,1117
				
			}
			if( job.iRobotSecond != -1)
			{
				if( GetM1TopVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM1BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);//2014,1117
			}
			
			if( job.iRobotFirst != -1)
			{
				//st_bcr_feeder[job.iRobotFirst].st_bcr.bExist = false;
				st_bcr_feeder[job.iRobotFirst].st_bcr.nState = EMBS_FAIL;
				//2015.0226 try pickup reject
				//memset( &st_bcr_feeder[job.iRobotFirst], 0x00, sizeof(st_bcr_feeder[job.iRobotFirst]));
			}
			if( job.iRobotSecond != -1)
			{				
				//st_bcr_feeder[job.iRobotSecond].st_bcr.bExist = false;
				st_bcr_feeder[job.iRobotSecond].st_bcr.nState = EMBS_FAIL;
				//2015.0226 try pickup reject
				//memset( &st_bcr_feeder[job.iRobotSecond], 0x00, sizeof(st_bcr_feeder[job.iRobotSecond]));
			}			
			
			Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();
			Func.Handler_Recovery_Data_Write();
		}
		break;

	case BCR1_PICK_LABEL_FINISH:
		{

			if( job.iRobotFirst != -1)
			{
				if( GetM1TopVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM1BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);//2014,1117

			}
			if( job.iRobotSecond != -1)
			{
				if( GetM1TopVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM1BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);//2014,1117
			}
			if( job.iRobotFirst != -1)
			{
				st_bcr_rbt[0].bPick[job.iRobotFirst] = st_bcr_feeder[job.iRobotFirst].st_bcr.bOut;
				st_bcr_feeder[job.iRobotFirst].st_bcr.bExist = false;
				memcpy( &st_bcr_rbt[0].st_bcr[job.iRobotFirst], &st_bcr_feeder[job.iRobotFirst].st_bcr, sizeof(st_bcr_feeder[job.iRobotFirst].st_bcr));
				memset( &st_bcr_feeder[job.iRobotFirst], 0x00, sizeof(st_bcr_feeder[job.iRobotFirst]));
			}
			if( job.iRobotSecond != -1)
			{				
				st_bcr_rbt[0].bPick[job.iRobotSecond] = st_bcr_feeder[job.iRobotSecond].st_bcr.bOut;
				st_bcr_feeder[job.iRobotSecond].st_bcr.bExist = false;
				memcpy( &st_bcr_rbt[0].st_bcr[job.iRobotSecond], &st_bcr_feeder[job.iRobotSecond].st_bcr, sizeof(st_bcr_feeder[job.iRobotSecond].st_bcr));
				memset( &st_bcr_feeder[job.iRobotSecond], 0x00, sizeof(st_bcr_feeder[job.iRobotSecond]));
			}
		
			// UI ¾÷µ¥ÀÌÆ®
			if( st_handler.cwnd_main )
			{
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY);
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDR_PICKER);
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDS_TESTSITE);
			}			
			job.bcr1_step = BCR1_STEP(271);
			mn_mot_flag[0][0]=0;mn_mot_flag[0][1]=0;mn_mot_flag[0][2]=0;
		}
		break;

	case 271://2014,1205
		{
			int nRet1;
			nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_R_BCR_PLACE], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				mn_mot_flag[0][0] = 0; mn_mot_flag[0][1] = mn_mot_flag[0][2] = 0; //clear 
				job.bcr1_step = BCR1_STEP(272);
				//2015.0206
				if( job.iRobotFirst != -1)
				{
					ReqM1RotateFwdBwd( job.iRobotFirst , FORWARD);
				}
				if( job.iRobotSecond != -1)
				{
					ReqM1RotateFwdBwd( job.iRobotSecond , FORWARD);
				}
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 265, CTL_dWARNING, alarm.mstr_code );
			}			
		}
		break;
		
	case 272:
		{
			//	int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_R_BCR_PLACE], st_basic.nRunSpeed );
			//	int nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_R_BCR_PLACE], st_basic.nRunSpeed );
			int nRet1 = COMI.Check_SingleMove(M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_R_BCR_PLACE]);  // ÀÌµ¿ ¿Ï·á ?
			if( nRet1 == BD_GOOD)
			{
				mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
				job.bcr1_step = BCR1_STEP(273);
				m_dwTime_Robot1 = GetCurrentTime();//2014,1205

				//2015.0206
// 				if( job.iRobotFirst != -1)
// 				{
// 					ReqM1RotateFwdBwd( job.iRobotFirst , FORWARD);
// 				}
// 				if( job.iRobotSecond != -1)
// 				{
// 					ReqM1RotateFwdBwd( job.iRobotSecond , FORWARD);
// 				}

			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				job.bcr1_step = BCR1_STEP(271);
				CTL_Lib.Alarm_Error_Occurrence( 266, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
		
	case 273://roller ready//2014,1205
		{
// 			int nRet1 = COMI.Check_SingleMove(M_BCR_ATTACH1_R1, st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_PLACE]);  // ÀÌµ¿ ¿Ï·á È®ÀÎ
// 			int nRet2  = COMI.Check_SingleMove(M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PLACE]);  // ÀÌµ¿ ¿Ï·á È®ÀÎ
// 			if (nRet1 == BD_GOOD) mn_mot_flag[0] = 1;
// 			if (nRet2 == BD_GOOD) mn_mot_flag[1] = 1;
// 			if(mn_mot_flag[0] == 1 && mn_mot_flag[1] == 1)
// 			{
// 				Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();
// 				Func.Handler_Recovery_Data_Write();
// 				mn_mot_flag[0]=0;mn_mot_flag[1]=0;mn_mot_flag[2]=0;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 266, CTL_dWARNING, alarm.mstr_code );
// 			}

			int nRet1,nRet2,nRet3,nRet4;
			CString strError;

			//if( job.iRobotFirst != -1)
				if( m_fbm1rotatefwdbwd[0] != FB_NONE) break;
			//if( job.iRobotSecond != -1)
				if( m_fbm1rotatefwdbwd[1] != FB_NONE) break;

//			if( GetCurrentTime() - m_dwTime_Robot1 < 0 ) m_dwTime_Robot1 = GetCurrentTime();
			if( job.iRobotFirst != -1)
			{
				nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_ON);//place position
				nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_OFF); //pick position
			}
			else
			{
				nRet1 = IO_ON;
				nRet2 = IO_OFF;
			}
			if( job.iRobotSecond != -1)
			{
				nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_ON);
				nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_OFF);
			}
			else
			{
				nRet3 = IO_ON;
				nRet4 = IO_OFF;
			}
			
			if(nRet1 == IO_ON && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_OFF)
			{
				Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();
				Func.Handler_Recovery_Data_Write();
			}
			else
			{
				if(nRet1 != IO_ON) 
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 266, CTL_dWARNING, strError );
				}
				else if(nRet2 != IO_OFF)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 267, CTL_dWARNING, strError );
				}
				else if(nRet3 != IO_ON)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 268, CTL_dWARNING, strError );
				}
				else if(nRet4 != IO_OFF)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 269, CTL_dWARNING, strError );
				}
			}
		}
		break;

	case BCR1_PLACE_LABEL_READY:
		{
			//2015.0327
			bool bWork = true;
			int nRet1;
			CString strError,strRbt,strIDBuff;
			if( job.iRobotFirst != -1)
			{
				if( st_bcr_rbt[0].st_bcr[job.iRobotFirst].nState == EMBS_FAIL )
				{
					bWork = false;
				}
				//2016.0916
				strRbt.Format("%s", st_bcr_rbt[0].st_bcr[0].c_serial);
				strIDBuff.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[0]);
				strError = strIDBuff.Left(strIDBuff.GetLength()-1);
				if( !Func.CmpStrFile(strRbt, strError) )
				{
					strError = _T( "650000" );
					CTL_Lib.Alarm_Error_Occurrence( 7268, CTL_dWARNING, strError );
					break;
				}
			}
			if( job.iRobotSecond != -1)
			{
				if( st_bcr_rbt[0].st_bcr[job.iRobotSecond].nState == EMBS_FAIL )
				{
					bWork = false;
				}
				//2016.0916
				strRbt.Format("%s", st_bcr_rbt[0].st_bcr[1].c_serial);
				strIDBuff.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[1]);
				strError = strIDBuff.Left(strIDBuff.GetLength()-1);
				if( !Func.CmpStrFile(strRbt, strError) )
				{
					strError = _T( "650000" );
					CTL_Lib.Alarm_Error_Occurrence( 7368, CTL_dWARNING, strError );
					break;
				}
			}

			if(bWork == false)//reject
			{
				job.bcr1_step = BCR1_STEP(432);
			}
			else
			{
				nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_R_BCR_PLACE], st_basic.nRunSpeed );
				
				if(  nRet1 == BD_GOOD )
				{
					mn_mot_flag[0][0] = 0; mn_mot_flag[0][1] = mn_mot_flag[0][2] = 0; //clear 
					job.bcr1_step = BCR1_PLACE_LABEL_X_READY;
				}
				else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
				{
					CTL_Lib.Alarm_Error_Occurrence( 270, CTL_dWARNING, alarm.mstr_code );
				}
			}
		}
		break;

	case BCR1_PLACE_LABEL_X_READY:
		{
			int nRet1 = COMI.Check_SingleMove(M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_R_BCR_PLACE]);  // ÀÌµ¿ ¿Ï·á È
			if( nRet1 == BD_GOOD)
			{
				mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
				job.bcr1_step = BCR1_PLACE_LABEL_READY2;
				m_dwTime_Robot1 = GetCurrentTime();//2014,1205
// 				if( job.iRobotFirst != -1)
// 				{
					ReqM1RotateFwdBwd( 0 ,FORWARD);
// 				}
// 				if( job.iRobotSecond != -1)
// 				{
					ReqM1RotateFwdBwd( 0 ,FORWARD);
// 				}
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				job.bcr1_step = BCR1_PLACE_LABEL_READY;
				CTL_Lib.Alarm_Error_Occurrence( 271, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;


	case BCR1_PLACE_LABEL_READY2://roller ready//2014,1205
		{

			int nRet1,nRet2,nRet3,nRet4;
			CString strError;
	
			if( m_fbm1rotatefwdbwd[0] != FB_NONE) break;
			if( m_fbm1rotatefwdbwd[1] != FB_NONE) break;
			
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_ON);//place position
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_OFF); //pick position
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_ON);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_OFF);	
			
			if(nRet1 == IO_ON && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_OFF)
			{
				double dpos = COMI.Get_MotCurrentPos(M_INDEX_X);
				if( dpos > st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS] - COMI.md_allow_value[M_INDEX_X] && dpos < st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS] + COMI.md_allow_value[M_INDEX_X] )
				{
					job.bcr1_step = BCR1_PLACE_LABEL_DOWN;
				}
				else
				{
					job.bcr1_step = BCR1_PLACE_LABEL_ROLLER_X_POS;
				}
			}
			else
			{
				if( job.iRobotFirst != -1)
				{
					if(nRet1 != IO_ON) 
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 273, CTL_dWARNING, strError );
					}
					else if(nRet2 != IO_OFF)
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 274, CTL_dWARNING, strError );
					}
				}
				if( job.iRobotSecond != -1)
				{
					if(nRet3 != IO_ON)
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 275, CTL_dWARNING, strError );
					}
					else if(nRet4 != IO_OFF)
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 276, CTL_dWARNING, strError );
					}
				}
			}
		}
		break;

	case BCR1_PLACE_LABEL_ROLLER_X_POS:
		{
			int	nRet = CTL_Lib.Single_Move( M_INDEX_X, st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr1_step = BCR1_PLACE_LABEL_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 277, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR1_PLACE_LABEL_DOWN:
		{//Á¦Ç°ÀÌ µµ´ÞµÇ¾î ÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù.

			int nRet1,nRet2;
// 			if(job.iRobotFirst != -1)
// 			{
				nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_DOWN], st_basic.nRunSpeed );
// 			}
// 			else
// 			{
// 				nRet1 = BD_GOOD;
// 			}
// 			if(job.iRobotSecond != -1)
// 			{				
				nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_DOWN], st_basic.nRunSpeed );
// 			}
// 			else
// 			{
// 				nRet2 = BD_GOOD;
// 			}
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD/* && nRet3 == BD_GOOD*/)
			{
				job.bcr1_step = BCR1_PLACE_LABEL_PUSH;
				ReqBoxUpDn(0, FORWARD);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY/* || nRet3 == BD_ERROR || nRet3 == BD_SAFETY */)
			{
				CTL_Lib.Alarm_Error_Occurrence( 278, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	
	case BCR1_PLACE_LABEL_PUSH:
		{
			int nRet3 = CTL_Lib.Single_Move( M_INDEX_ROLLER1, st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_POS], st_basic.nRunSpeed );
			if( nRet3 == BD_GOOD)
			{
// 				if( job.iRobotFirst != -1)
// 				{
// 					ReqM1TopVac( job.iRobotFirst, FORWARD);
// 					ReqRollingFwdBwd( 0, job.iRobotFirst, FORWARD);
					ReqM1TopVac( 0, FORWARD);
					ReqRollingFwdBwd( 0, 0, FORWARD);
// 				}
// 				if( job.iRobotSecond != -1)
// 				{
// 					ReqM1TopVac( job.iRobotSecond, FORWARD);
// 					ReqRollingFwdBwd( 0, job.iRobotSecond, FORWARD);
					ReqM1TopVac( 1, FORWARD);
					ReqRollingFwdBwd( 0, 1, FORWARD);
// 				}
				//ReqRollingFwdBwd( 1, FORWARD);
				
				job.bcr1_step = BCR1_PLACE_LABEL_XPLACE;
			}
			else if( nRet3 == BD_ERROR || nRet3 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 279, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case BCR1_PLACE_LABEL_XPLACE:
		{
			if( m_fbboxupdn[0] != FB_NONE ) break;
			if( st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] > (st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] + 5) || st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] < (st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_DOWN]))
			{
				if(st_handler.cwnd_list != NULL)
				{
// 					sprintf(st_msg.c_abnormal_msg, "[BAR ROBOT] Picker Down pos is more than Picker Pick(5mm) or is less than. Position Error Check ");
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					break;
				}
			}
// 			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD )
// 			{
				job.bcr1_step = BCR1_PLACE_LABEL_PUSH_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 280, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;
		

	case BCR1_PLACE_LABEL_PUSH_CHK:
		{
// 			if( job.iRobotFirst != -1)
// 			{
// 				if( GetM1TopVac( job.iRobotFirst ) != FB_NONE ) break;
// 				ReqM1BtmVac( job.iRobotFirst, FORWARD);
// 				if( GetRollingFwdBwd( 0, job.iRobotFirst ) != FB_NONE ) break;
				if( GetM1TopVac( 0 ) != FB_NONE ) break;
				ReqM1BtmVac( 0, FORWARD);
				if( GetRollingFwdBwd( 0, 0 ) != FB_NONE ) break;
// 			}
// 			if( job.iRobotSecond != -1)
// 			{
				if( GetM1TopVac( 1 ) != FB_NONE ) break;
				ReqM1BtmVac( 1, FORWARD);
				if( GetRollingFwdBwd( 0, 1 ) != FB_NONE ) break;
//			}
			st_var.n_tboxmv_forming_cv_mode = CTL_YES;//2015.0331
//			if( GetRollingFwdBwd( 1 ) != FB_NONE ) break;
			job.bcr1_step = BCR1_PLACE_LABEL_PUSH_VACUUM;
		}
		break;

	case BCR1_PLACE_LABEL_PUSH_VACUUM:
		{
// 			if( job.iRobotFirst != -1)
// 				if( GetM1BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM1BtmVac( 0 ) != FB_NONE ) break;
// 			if( job.iRobotSecond != -1)
// 				if( GetM1BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM1BtmVac( 1 ) != FB_NONE ) break;

				st_var.n_tboxmv_forming_cv_mode = CTL_NO;//2015.0331

				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[0], IO_OFF);

				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[1], IO_OFF);
			job.bcr1_step = BCR1_PLACE_LABEL_VACUUM_CHK;	
		}
		break;

	case BCR1_PLACE_LABEL_VACUUM_CHK:
		{

			if( job.iRobotFirst != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[job.iRobotFirst], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[job.iRobotFirst], IO_ON);
// 				ReqM1BtmVac( job.iRobotFirst, BACKWARD);
// 				ReqRollingFwdBwd( 0, job.iRobotFirst, BACKWARD);
			}
			ReqM1BtmVac( 0, BACKWARD);
			ReqRollingFwdBwd( 0, 0, BACKWARD);
			if( job.iRobotSecond != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[job.iRobotSecond], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[job.iRobotSecond], IO_ON);
// 				ReqM1BtmVac( job.iRobotSecond, BACKWARD);
// 				ReqRollingFwdBwd( 0, job.iRobotSecond, BACKWARD);
			}
			ReqM1BtmVac( 1, BACKWARD);
			ReqRollingFwdBwd( 0, 1, BACKWARD);
//			ReqRollingFwdBwd( 1, BACKWARD);
			m_dwTime_Robot1 = GetCurrentTime();
			job.bcr1_step = BCR1_PLACE_LABEL_PULL;
		}
		break;

	case BCR1_PLACE_LABEL_PULL:
		if( GetCurrentTime() - m_dwTime_Robot1 < 0) m_dwTime_Robot1 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		{
			if( job.iRobotFirst != -1)
			{
// 				if( GetM1BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[job.iRobotFirst], IO_OFF);
// 				ReqM1TopVac( job.iRobotFirst, BACKWARD);
// 				if( GetRollingFwdBwd( 0, job.iRobotFirst ) != FB_NONE ) break;
			}
			if( job.iRobotSecond != -1)
			{
// 				if( GetM1BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[job.iRobotSecond], IO_OFF);
// 				ReqM1TopVac( job.iRobotSecond, BACKWARD);
// 				if( GetRollingFwdBwd( 0, job.iRobotSecond ) != FB_NONE ) break;
			}
			if( GetM1BtmVac( 0 ) != FB_NONE ) break;
			ReqM1TopVac( 0, BACKWARD);
			if( GetRollingFwdBwd( 0, 0 ) != FB_NONE ) break;
			if( GetM1BtmVac( 1 ) != FB_NONE ) break;
			ReqM1TopVac( 1, BACKWARD);
			if( GetRollingFwdBwd( 0, 1 ) != FB_NONE ) break;
//			if( GetRollingFwdBwd( 1 ) != FB_NONE ) break;

			job.bcr1_step = BCR1_PLACE_LABEL_PULL_CHK;
		}
		break;
		
	case BCR1_PLACE_LABEL_PULL_CHK:
		{
			if(job.iRobotFirst != -1)
			{
// 				if( GetM1TopVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[job.iRobotFirst], IO_OFF);
			}
			if( GetM1TopVac( 0 ) != FB_NONE ) break;
			if(job.iRobotSecond != -1)
			{
// 				if( GetM1TopVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[job.iRobotSecond], IO_OFF);
 			}
			if( GetM1TopVac( 1 ) != FB_NONE ) break;
			job.bcr1_step = BCR1_PLACE_LABEL_VACUUM_RECHK;	
			m_dwTime_Robot1 = GetCurrentTime();
		}
		break;

	case BCR1_PLACE_LABEL_VACUUM_RECHK:
		if( GetCurrentTime() - m_dwTime_Robot1 < 0) m_dwTime_Robot1 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		{
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
				{
				}
				else 
				{
// 					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_OFF) == IO_ON )
// 					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 281, CTL_dWARNING, strError );
// 					}
// 					else
// 					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 282, CTL_dWARNING, strError );
// 					}
// 					break;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_OFF) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond], IO_OFF) == IO_OFF )
				{
				}
				else if( GetCurrentTime() - m_dwTime_Robot1 > st_time.n_limit_time[E_WAIT_BCR_PICK_VACUUM] )
				{
// 					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_OFF) == IO_ON )
// 					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond]);
// 						CTL_Lib.Alarm_Error_Occurrence( 283, CTL_dWARNING, strError );
// 					}
// 					else
// 					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond]);
// 						CTL_Lib.Alarm_Error_Occurrence( 284, CTL_dWARNING, "515090" );
// 					}
// 					break;
				}
			}
			ReqBoxUpDn(0, BACKWARD);//2014.1120 ysj
			job.bcr1_step = BCR1_PLACE_LABEL_XDOWN_BACK;
		}
		break;

	case BCR1_PLACE_LABEL_XDOWN_BACK:
		{
			job.bcr1_step = BCR1_PLACE_LABEL_UP;
			mn_mot_flag[0][0] = 0;
			mn_mot_flag[0][1] = 0;
			mn_mot_flag[0][2] = 0;
		}
		break;

	case 431:
		if( m_fbm1rotatefwdbwd[EBVS_LEFT] != FB_NONE) break;
		if( m_fbm1rotatefwdbwd[EBVS_RIGHT] != FB_NONE) break;
		job.bcr1_step = BCR1_STEP(432);
		break;
		
	case 432:
		{
			bool nfwd1 = CheckM1RotateFwdBwd( EBVS_LEFT, IO_OFF );
			bool nfwd2 = CheckM1RotateFwdBwd( EBVS_RIGHT, IO_OFF );
			if( nfwd1 == true && nfwd2 == true )
			{
				ReqM1TopVac( EBVS_LEFT , FORWARD );
				ReqM1TopVac( EBVS_RIGHT , FORWARD );
				ReqM1BtmVac( EBVS_LEFT , BACKWARD );
				ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
				job.bcr1_step = BCR1_STEP(433);			
			}
			else
			{
				ReqM1RotateFwdBwd( EBVS_LEFT ,BACKWARD);
				ReqM1RotateFwdBwd( EBVS_RIGHT ,BACKWARD);
				job.bcr1_step = BCR1_STEP(431);			
			}
		}
		break;
		
	case 433:
		if( GetM1TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		mn_mot_flag[0][0] = mn_mot_flag[0][1] = 0;
		job.bcr1_step = BCR1_STEP(434);
		break;
		
	case 434:
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				job.bcr1_step = BCR1_STEP(435);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 371, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 435:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2 , st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[0][1] = 1;
			if(mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_LEFT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_RIGHT], IO_OFF);
				job.bcr1_step = BCR1_STEP(436);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 372, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 436:
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_RIGHT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_RIGHT], IO_ON);
		m_dwTime_Robot1 = GetCurrentTime();
		job.bcr1_step = BCR1_STEP(440);
		break;

	case 440:
		if( GetCurrentTime() - m_dwTime_Robot1 < 0) m_dwTime_Robot1 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot1 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		mn_mot_flag[0][0] = 0;
		mn_mot_flag[0][1] = 0;
		job.bcr1_step = BCR1_STEP(441);
		break;

	case 441:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2 , st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[0][1] = 1;
			if(mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1)
			{
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_LEFT], IO_OFF);				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[EBVS_RIGHT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[EBVS_RIGHT], IO_OFF);
				//Func.OnSet_IO_Port_Stop();
				// 				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				
				job.bcr1_step = BCR1_STEP(442);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 360, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 442:
		{
			ReqM1TopVac( EBVS_LEFT , BACKWARD );
			ReqM1TopVac( EBVS_RIGHT , BACKWARD );
			ReqM1BtmVac( EBVS_LEFT , BACKWARD );
			ReqM1BtmVac( EBVS_RIGHT, BACKWARD );
			job.bcr1_step = BCR1_STEP(443);			
		}
		break;

	case 443:
		if( GetM1TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM1BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr1_step = BCR1_STEP(444);
		break;

	case 444:
		if( st_basic.n_mode_device && job.iRobotFirst != -1)
		{
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515081 1 00 "Distribution Label Robot1 Left Top Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 361, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515091 1 00 "Distribution Label Robot1 Left Bottom Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 362, CTL_dWARNING, strError );
				break;
			}
// 			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON) == IO_ON)
// 			{
// 				CString strError;
// 				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotFirst], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence(365, dWARNING, strError);
// 				break;
// 			}
		}
		if( st_basic.n_mode_device && job.iRobotSecond != -1)
		{
			//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_top_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 363, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
			{
				//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt1_btm_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 364, CTL_dWARNING, strError );
				break;
			}
// 			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotSecond], IO_ON) == IO_ON)
// 			{
// 				CString strError;
// 				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotSecond], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence(366, dWARNING, strError);				
// 				break;
// 			}
		}
		job.bcr1_step = BCR1_STEP(445);
		break;

	case 445:
		if( job.iRobotFirst != -1)
		{
			ReqM1TopVac( job.iRobotFirst, BACKWARD);
			ReqM1BtmVac( job.iRobotFirst, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst], IO_OFF);
		}
		if( job.iRobotSecond != -1)
		{
			ReqM1TopVac( job.iRobotSecond, BACKWARD);
			ReqM1BtmVac( job.iRobotSecond, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotSecond], IO_OFF);
		}
		job.bcr1_step = BCR1_STEP(446);
		break;

	case 446:
		{			
			if( job.iRobotFirst != -1)
			{
				if( GetM1TopVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM1BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);//2014,1117
				
			}
			if( job.iRobotSecond != -1)
			{
				if( GetM1TopVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM1BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);//2014,1117
			}
			
			if( job.iRobotFirst != -1)
			{
				st_bcr_rbt[0].bPick[job.iRobotFirst] = 0;
				st_bcr_rbt[0].st_bcr[job.iRobotFirst].bExist = 0;
				st_bcr_rbt[0].st_bcr[job.iRobotFirst].bOut = 0;
			}
			if( job.iRobotSecond != -1)
			{				
				st_bcr_rbt[0].bPick[job.iRobotSecond] = 0;					
				st_bcr_rbt[0].st_bcr[job.iRobotSecond].bExist = 0;
				st_bcr_rbt[0].st_bcr[job.iRobotSecond].bOut = 0;
			}			
			
			Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();
			Func.Handler_Recovery_Data_Write();
		}
		break;

	case BCR1_PLACE_LABEL_UP:
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			int nRet3 = CTL_Lib.Single_Move( M_INDEX_ROLLER1, st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[0][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[0][1] = 1;
			if(nRet3 == BD_GOOD) mn_mot_flag[0][2] = 1;
			if( mn_mot_flag[0][0] == 1 && mn_mot_flag[0][1] == 1 && mn_mot_flag[0][2] == 1 )
			{
				job.bcr1_step = BCR1_PLACE_LABEL_SAFETY;
				mn_mot_flag[0][0] = 0;
				mn_mot_flag[0][1] = 0;
				mn_mot_flag[0][2] = 0;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 285, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR1_PLACE_LABEL_SAFETY://2014,1205
		{
// 			int nRet1,nRet2,nRet3;
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R1, st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			nRet3 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			if(nRet1 == BD_GOOD) mn_mot_flag[0] = 1;
// 			if(nRet2 == BD_GOOD) mn_mot_flag[1] = 1;
// 			if(nRet3 == BD_GOOD) mn_mot_flag[2] = 1;
// 			if( mn_mot_flag[0] == 1 && mn_mot_flag[1] == 1 && mn_mot_flag[2] == 1 )
// 			{
// 				job.bcr1_step = BCR1_PLACE_LABEL_FINISH;
// 				mn_mot_flag[0] = 0;
// 				mn_mot_flag[1] = 0;
// 				mn_mot_flag[2] = 0;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 286, CTL_dWARNING, alarm.mstr_code );
// 			}

//			int nRet1,nRet2,nRet3;
//			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
//
//			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[0], IO_OFF);//place pos
//			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[0], IO_ON);//pick pos
//			nRet2 = BD_GOOD;
//
//			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[1], IO_OFF);
//			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[1], IO_ON);
//			nRet3 = BD_GOOD;
//
//			if(nRet1 == BD_GOOD) mn_mot_flag[0] = 1;
//			if(nRet2 == BD_GOOD) mn_mot_flag[1] = 1;
//			if(nRet3 == BD_GOOD) mn_mot_flag[2] = 1;
//			if( mn_mot_flag[0] == 1 && mn_mot_flag[1] == 1 && mn_mot_flag[2] == 1 )
//			{
//				job.bcr1_step = BCR1_STEP(451);
//				mn_mot_flag[0] = 0;
//				mn_mot_flag[1] = 0;
//				mn_mot_flag[2] = 0;
//				m_dwTime_Robot1 = GetCurrentTime();
//			}
//			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY/* || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY */)
//			{
//				CTL_Lib.Alarm_Error_Occurrence( 287, CTL_dWARNING, alarm.mstr_code );
//			}

			int nRet1;
			nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );


			if( nRet1 == BD_GOOD )
			{
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[0], IO_OFF);//place pos
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[0], IO_ON);//pick pos
//
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[1], IO_OFF);
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[1], IO_ON);

				//2015.0206
				ReqM1RotateFwdBwd( 0, BACKWARD );
				ReqM1RotateFwdBwd( 1, BACKWARD );
				job.bcr1_step = BCR1_STEP(461);
				mn_mot_flag[0][0] = 0;
				mn_mot_flag[0][1] = 0;
				mn_mot_flag[0][2] = 0;
				m_dwTime_Robot1 = GetCurrentTime();
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 292, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 461://2014,1205
		{			
			int nRet = COMI.Check_SingleMove( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] );
			if( nRet == BD_GOOD )
			{
				//2015.0206
// 				ReqM1RotateFwdBwd( 0, BACKWARD );
// 				ReqM1RotateFwdBwd( 1, BACKWARD );
				job.bcr1_step = BCR1_STEP(462);
// 				if( m_fbm1rotatefwdbwd[0] != FB_NONE) break;
// 				if( m_fbm1rotatefwdbwd[1] != FB_NONE) break;
// 				
// 				int nRet1,nRet2,nRet3,nRet4;
// 				CString strError;
// 				nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_OFF);//place position
// 				nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_ON); //pick position
// 				nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_OFF);
// 				nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_ON);	
// 				
// 				if(nRet1 == IO_OFF && nRet2 == IO_ON && nRet3 == IO_OFF && nRet4 == IO_ON)
// 				{
// 					job.bcr1_step = BCR1_PLACE_LABEL_FINISH;
// 				}
// 				else
// 				{
// 					if(nRet1 != IO_OFF) 
// 					{
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
// 						CTL_Lib.Alarm_Error_Occurrence( 289, CTL_dWARNING, strError );
// 					}
// 					else if(nRet2 != IO_ON)
// 					{
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
// 						CTL_Lib.Alarm_Error_Occurrence( 290, CTL_dWARNING, strError );
// 					}
// 					else if(nRet3 != IO_OFF)
// 					{
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
// 						CTL_Lib.Alarm_Error_Occurrence( 291, CTL_dWARNING, strError );
// 					}
// 					else if(nRet4 != IO_ON)
// 					{
// 						strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
// 						CTL_Lib.Alarm_Error_Occurrence( 292, CTL_dWARNING, strError );
// 					}
// 				}
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				job.bcr1_step = BCR1_PLACE_LABEL_SAFETY;
				mn_mot_flag[0][0] = 0;
				mn_mot_flag[0][1] = 0;
				mn_mot_flag[0][2] = 0;
				CTL_Lib.Alarm_Error_Occurrence( 293, CTL_dWARNING, alarm.mstr_code );
			}			
		}
		break;

	case 462:
		{
			if( m_fbm1rotatefwdbwd[0] != FB_NONE) break;
			if( m_fbm1rotatefwdbwd[1] != FB_NONE) break;
			
			int nRet1,nRet2,nRet3,nRet4;
			CString strError;
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_OFF);//place position
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_ON); //pick position
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_ON);	
			
			if(nRet1 == IO_OFF && nRet2 == IO_ON && nRet3 == IO_OFF && nRet4 == IO_ON)
			{
				job.bcr1_step = BCR1_PLACE_LABEL_FINISH;
			}
			else
			{
				if(nRet1 != IO_OFF) 
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 289, CTL_dWARNING, strError );
				}
				else if(nRet2 != IO_ON)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 290, CTL_dWARNING, strError );
				}
				else if(nRet3 != IO_OFF)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 291, CTL_dWARNING, strError );
				}
				else if(nRet4 != IO_ON)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 292, CTL_dWARNING, strError );
				}
			}
		}

		break;

	case BCR1_PLACE_LABEL_FINISH:
		{
			if(m_fbboxupdn[0] != FB_NONE ) break;
//			ABcrLabel* pbcr_1 = NULL;
//			ABcrLabel* pbcr_2 = NULL;
//			int nTrayIdx = -1;
//			if( job.GetPosByTarget() == MPOS_INDEX_BCR1_LOAD )
//			{
//				nTrayIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE1 );
//				pbcr_1 = &g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst % MPOS_INDEX_BCR1_LOAD ).GetBcrLabel();
//				pbcr_2 = &g_Tray.GetTray( nTrayIdx ).GetModule( (job.iPosFirst + 1) % MPOS_INDEX_BCR1_LOAD ).GetBcrLabel();
//			}
//
//			if( job.iRobotFirst != -1 && pbcr_1)
//			{
// 				*pbcr_1 = Func.m_pRobot[SITE_BARCODE1]->GetPicker( job.iRobotFirst ).GetBcrLabel();
//				Func.m_pRobot[SITE_BARCODE1]->GetPicker( job.iRobotFirst ).GetBcrLabel().EraseData();
//			}
//			if( job.iRobotSecond != -1 && pbcr_2)
//			{
// 				*pbcr_2 = Func.m_pRobot[SITE_BARCODE1]->GetPicker( job.iRobotSecond ).GetBcrLabel();
//				Func.m_pRobot[SITE_BARCODE1]->GetPicker( job.iRobotFirst ).GetBcrLabel().EraseData();
//			}
//			
//			// UI ¾÷µ¥ÀÌÆ®
//			if( st_handler.cwnd_main )
//			{
//				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY);
//				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDR_PICKER);
//
////				if( job.GetPosByTarget() == MPOS_INDEX_TESTSITE )
////					st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDS_TESTSITE);
//
//			}
//			int iIdx = g_Tray.GetIdxByPos(IDPOS_BARCODE1);			
//			ATray& trayPri = g_Tray.GetTray( iIdx );
//			trayPri.SetWorkState(EWS_BARCODE1);		


			if( job.iRobotFirst != -1)
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].nBcrExist[CTL_LEFT] = CTL_YES;
				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode1[job.iRobotFirst], &st_bcr_rbt[0].st_bcr[job.iRobotFirst], sizeof(st_bcrinfo_param));
			}
			if( job.iRobotSecond != -1)
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].nBcrExist[CTL_RIGHT] = CTL_YES;
				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].st_barcode1[job.iRobotSecond], &st_bcr_rbt[0].st_bcr[job.iRobotSecond], sizeof(st_bcrinfo_param));
			}
			if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
			{
				if( job.iRobotFirst != -1)
				{
					st_bcr_rbt[0].bPick[job.iRobotFirst] = 0;
					st_bcr_rbt[0].st_bcr[job.iRobotFirst].bExist = 0;
					st_bcr_rbt[0].st_bcr[job.iRobotFirst].bOut = 0;
				}
				if( job.iRobotSecond != -1)
				{
					st_bcr_rbt[0].bPick[job.iRobotSecond] = 0;					
					st_bcr_rbt[0].st_bcr[job.iRobotSecond].bExist = 0;
					st_bcr_rbt[0].st_bcr[job.iRobotSecond].bOut = 0;
				}
				memset( &st_bcr_rbt[0], 0x00, sizeof(st_bcr_rbt[0]));
			}

			Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();
//			Func.BackupData();//2013,1018

			st_sync.n_bcr_1_work_req = CTL_REQ;

			if(st_handler.n_only_work_bcr1 == CTL_YES)
			{
				//2014,1117
				if( job.iRobotFirst != -1)
					st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[job.iRobotFirst].nComplete = CTL_NO;
				if( job.iRobotSecond != -1)
					st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[job.iRobotSecond].nComplete = CTL_NO;
				st_sync.n_bcr_1_work_req = CTL_READY;
			}

			if(GetCurrentTime() - m_dwTimeTact <= 0 ) m_dwTimeTact = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeTact >= 0 && GetCurrentTime() - m_dwTimeTact < 900000 &&
				st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[BARCODE1]Tact time : %d", GetCurrentTime() - m_dwTimeTact);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 				Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
			}
			m_dwTimeTact = GetCurrentTime();


			Func.Handler_Recovery_Data_Write();
			st_handler.mn_btm_ready = 1;
		}		
		break;
	}
}

void CRun_Barcode_Works::ReqBcrPickWork( int nSite, EPF epf )
{
	m_rbtPickFeeder[nSite] = epf;
	if(epf == EPF_PICK) m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY;
	else				m_step_bcrPickFeeder[nSite] = ESRPF_PLACE_READY;
}


void CRun_Barcode_Works::Run_Move_Robot_PickFeeder( int nSite)//nSite°¡ 1ÀÌ¸é rbt1ÀÇ left,right ·Îº¿ÀÌ µ¿ÀÛÇÑ´Ù. nSite°¡ 2ÀÌ¸é rbt2ÀÇ left,right ·Îº¿ÀÌ µ¿ÀÛÇÑ´Ù.
{
	if( m_rbtPickFeeder[nSite] == EPF_NONE) return;
	
	int iOnOff = IO_ON;
	if( m_rbtPickFeeder[nSite] == EPF_PLACE)
		iOnOff = IO_OFF;

	switch(m_step_bcrPickFeeder[nSite])
	{
	case ESRPF_PICKER_NONE: break;

	case ESRPF_PICKER_READY:
		{
			if(iOnOff == IO_ON)//pickupÀÏ °æ¿ì
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[0], IO_OFF);
			}
			m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_X;
		}
		break;

	case ESRPF_PICKER_READY_Z1:
		{
			double dpos = COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z1);
			if ( dpos > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY]  + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_Z1_CHK;
			}
			else
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_Z2;
			}
		}
		break;

	case ESRPF_PICKER_READY_Z1_CHK:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_Z2;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 294, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case ESRPF_PICKER_READY_Z2:
		{
			double dpos = COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z1);
			if ( dpos > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY]  + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_Z2_CHK;
			}
			else
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_X;
			}
		}
		break;

	case ESRPF_PICKER_READY_Z2_CHK:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_X;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 284, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case ESRPF_PICKER_READY_X:
		{
			double dpos = COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X);
			if ( dpos > st_motor[M_BCR_ATTACH1_X ].d_pos[P_X_BCR_PICK]  - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				dpos < st_motor[M_BCR_ATTACH1_X ].d_pos[P_X_BCR_PICK]  + COMI.md_allow_value[M_BCR_ATTACH1_X])
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_DOWN_CHK;
			}
			else
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_X_CHK;
			}
		}
		break;

	case ESRPF_PICKER_READY_X_CHK:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_X , st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 295, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case ESRPF_PICKER_READY_DOWN:
		ReqM1TopVac( nSite, FORWARD);
		m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_READY_DOWN_CHK;
		break;

	case ESRPF_PICKER_READY_DOWN_CHK:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 + (nSite*2), st_motor[M_BCR_ATTACH1_Z1 + (nSite*2)].d_pos[P_Z_BCR_PICK], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				m_step_bcrPickFeeder[nSite] = ESRPF_PICKER_PICK;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 296, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	}

}

bool CRun_Barcode_Works::Run_Bcr_Move_Chk2_Jam( stJobPicker job )
{
	CString strJamCode;

	Func.ThreadFunctionStepTrace(22, job.bcr2_step);
	switch( job.bcr2_step )
	{
	case BCR2_NONE:
		break;

	case BCR2_PICK_LABEL_ROTATE_CHK:
	case BCR2_PICK_R1_SAFETY:
	case BCR2_PICK_R2_SAFETY:
	case BCR2_PICK_LABEL_MOVE_READY_POS:
	case BCR2_PICK_LABEL_MOVE_READY:
		if( Chk_Robot2_vacuum_fwdbwd( 0, IO_OFF ) == false )		return false;
		if( Chk_Robot2_vacuum_fwdbwd( 1, IO_OFF ) == false )		return false;
		if( Chk_Robot_rollingfwdbwd( 2, IO_OFF ) == false )			return false;
//		if( Chk_Robot2_guide_updn( 0, IO_ON ) == false )			return false;//guide up
//		if( Chk_Robot2_guide_updn( 1, IO_ON ) == false )			return false;
		break;

	case BCR2_PICK_LABEL_DOWN_CHK:
	case BCR2_PICK_LABEL_VACUUM:
	case BCR2_PICK_LABEL_UP:
	case BCR2_PICK_LABEL_VACUUM_CHK:
	case BCR2_PICK_LABEL_FINISH:
		if( Chk_Robot_rollingfwdbwd( 2, IO_OFF ) == false )			return false;
		break;

	case BCR2_PLACE_LABEL_PUSH_VACUUM:
		if(job.iRobotFirst != -1)
			if( Chk_Robot2_vacuum_fwdbwd( 0, IO_ON ) == false )			return false;
		if(job.iRobotSecond != -1)
			if( Chk_Robot2_vacuum_fwdbwd( 1, IO_ON ) == false )			return false;
//		if( Chk_Robot2_guide_updn( 0, IO_OFF ) == false )			return false;//guide down
//		if( Chk_Robot2_guide_updn( 1, IO_OFF ) == false )			return false;
		break;

	case BCR2_PLACE_LABEL_PULL_CHK:
		if(job.iRobotFirst != -1)
			if( Chk_Robot2_vacuum_fwdbwd( 0, IO_OFF ) == false )		return false;
		if(job.iRobotSecond != -1)
			if( Chk_Robot2_vacuum_fwdbwd( 1, IO_OFF ) == false )		return false;
		if( Chk_Robot_rollingfwdbwd( 2, IO_ON ) == false )			return false;
//		if( Chk_Robot2_guide_updn( 0, IO_ON ) == false )			return false;
//		if( Chk_Robot2_guide_updn( 1, IO_ON ) == false )			return false;
		break;

	case BCR2_PLACE_LABEL_SAFETY:
	case BCR2_PLACE_LABEL_FINISH:	
		if( Chk_Robot2_guide_fwdbwd( 0, IO_OFF ) == false )		return false;
		if( Chk_Robot2_guide_fwdbwd( 1, IO_OFF ) == false )		return false;
		if( Chk_Robot_rollingfwdbwd( 2, IO_OFF ) == false )			return false;
//		if( Chk_Robot2_guide_updn( 0, IO_ON ) == false )			return false;//guide up
//		if( Chk_Robot2_guide_updn( 1, IO_ON ) == false )			return false;
		break;
	}
	
	return true;
}

bool CRun_Barcode_Works::Run_Bcr_Move_Chk2_Time()
{
	switch( bcr1_step_old )
	{
	case BCR2_PICK_Z1_SAFETY:
 		if( InTimeRobot2( 10000 ) == false )
 		{//230006 1 00 "Distribute#2 Attach Z#1 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 1301, dWARNING, "230006" );
			return false;
 		}
		break;
	case BCR2_PICK_Z2_SAFETY:
 		if( InTimeRobot2( 10000 ) == false )
 		{//250006 1 00 "Distribute#2 Attach Z#2 Axis Motor Move Error."
			CTL_Lib.Alarm_Error_Occurrence( 1302, dWARNING, "250006" );
			return false;
 		}
		break;
	}

	return true;
}

bool CRun_Barcode_Works::InTimeRobot2( int iMilliSec )
{
	DWORD dwtime = GetCurrentTime();
	//2012,1220
	if( GetCurrentTime() - m_dwTime_Robot2 < 0 )
	{
		m_dwTime_Robot2 = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwTime_Robot2 < iMilliSec )
		return true;
	
	return false;
}

void CRun_Barcode_Works::Run_Move2()
{
	int nJobCnt = Func.m_pRobot[SITE_BARCODE2]->GetJobCount();
	if( nJobCnt == 0 )
		return;

//   	Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();


	stJobPicker& job = Func.m_pRobot[SITE_BARCODE2]->GetFirstJob();

	if( bcr2_step_old != job.bcr2_step)
	{
		bcr2_step_old = job.bcr2_step;
		m_dwTime_Robot2 = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
//		if( Run_Move_Chk1_Jam( job ) == false )
//			return;
	}
//	if( Run_Move_Chk1_Time( job ) == false )
//		return;
	Func.ThreadFunctionStepTrace(23, job.bcr2_step);
	switch( job.bcr2_step )
	{
	case BCR2_NONE:	break;

	case BCR2_PICK_LABEL_READY:
		{
			double dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH2_Z1);
			if( dPos > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_UP] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
			{
				job.bcr2_step = BCR2_PICK_Z1_SAFETY;
				break;
			}
			dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH2_Z2);
			if( dPos > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_UP] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
			{
				job.bcr2_step = BCR2_PICK_Z2_SAFETY;
				break;
			}

			job.bcr2_step = BCR2_PICK_LABEL_TOP_CHK;	
			
		}
		break;

	case BCR2_PICK_Z1_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr2_step = BCR2_PICK_LABEL_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1303, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR2_PICK_Z2_SAFETY:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr2_step = BCR2_PICK_LABEL_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1304, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR2_PICK_LABEL_TOP_CHK:
		{
			int nLock_1 = st_io.i_distribute_rbt2_top_vacuum_fwd_chk[0];
			int nLock_2 = st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0];

			if( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON)//2014,1205
			{
				job.bcr2_step = BCR2_PICK_LABEL_TOP_LEFT_BACKWPRD;
				break;
			}
			nLock_1 = st_io.i_distribute_rbt2_top_vacuum_fwd_chk[1] ;
			nLock_2 = st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1];

			if( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON)//2014,1205
			{
				job.bcr2_step = BCR2_PICK_LABEL_TOP_RIGHT_BACKWPRD;
				break;
			}

			job.bcr2_step = BCR2_PICK_LABEL_BTM_CHK;

		}
		break;

	case BCR2_PICK_LABEL_TOP_LEFT_BACKWPRD:
		ReqM2TopVac( EBVS_LEFT , BACKWARD );
		ReqM2TopVac( EBVS_RIGHT , BACKWARD );
		job.bcr2_step = BCR2_PICK_LABEL_TOP_LEFT_BACKWPRD_CHK;
		break;

	case BCR2_PICK_LABEL_TOP_LEFT_BACKWPRD_CHK:
		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr2_step = BCR2_PICK_LABEL_TOP_CHK;
		break;
		
	case BCR2_PICK_LABEL_TOP_RIGHT_BACKWPRD:
		ReqM2TopVac( EBVS_RIGHT, BACKWARD );
		job.bcr2_step = BCR2_PICK_LABEL_TOP_RIGHT_BACKWPRD_CHK;
		break;

	case BCR2_PICK_LABEL_TOP_RIGHT_BACKWPRD_CHK:
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr2_step = BCR2_PICK_LABEL_TOP_CHK;
		break;

	case BCR2_PICK_LABEL_BTM_CHK:
		{
			int nLock_1 = st_io.i_distribute_rbt2_btm_vacuum_fwd_chk[0];
			int nLock_2 = st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0];

			if( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON)
			{
				job.bcr2_step = BCR2_PICK_LABEL_BTM_LEFT_BACKWPRD;
				break;
			}
			nLock_1 = st_io.i_distribute_rbt2_btm_vacuum_fwd_chk[1] ;
			nLock_2 = st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1];

			if( /*g_ioMgr.get_in_bit( nLock_1 ) != IO_OFF ||*/ g_ioMgr.get_in_bit( nLock_2 ) != IO_ON)
			{
				job.bcr2_step = BCR2_PICK_LABEL_BTM_RIGHT_BACKWPRD;
				break;
			}

			job.bcr2_step = BCR2_PICK_LABEL_ROTATE_CHK;

		}
		break;

	case BCR2_PICK_LABEL_BTM_LEFT_BACKWPRD:
		ReqM2BtmVac( EBVS_LEFT , BACKWARD );
		ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
		job.bcr2_step = BCR2_PICK_LABEL_BTM_LEFT_BACKWPRD_CHK;
		break;

	case BCR2_PICK_LABEL_BTM_LEFT_BACKWPRD_CHK:
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr2_step = BCR2_PICK_LABEL_BTM_CHK;
		break;

	case BCR2_PICK_LABEL_BTM_RIGHT_BACKWPRD:
		ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
		job.bcr2_step = BCR2_PICK_LABEL_BTM_RIGHT_BACKWPRD_CHK;
		break;

	case BCR2_PICK_LABEL_BTM_RIGHT_BACKWPRD_CHK:
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) break;//2014,1205 GetM1BtmVac -> GetM2BtmVac º¯°æ
		job.bcr2_step = BCR2_PICK_LABEL_BTM_CHK;
		break;

	case BCR2_PICK_LABEL_ROTATE_CHK://2014,1205 
		{
// 			double dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_R1);
// 			if( 0 && dPos > st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_R1] )
// 			{
// 				job.bcr2_step = BCR2_PICK_R1_SAFETY;
// 				break;
// 			}
// 			dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH1_R2);
// 			if( 0 && dPos > st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_R2] )
// 			{
// 				job.bcr2_step = BCR2_PICK_R2_SAFETY;
// 				break;
// 			}
// 
// 			job.bcr2_step = BCR2_PICK_LABEL_MOVE_READY_POS;		
			mn_mot_flag[1][0] = 0; mn_mot_flag[1][1] = 0;
			job.bcr2_step = BCR2_PICK_R1_SAFETY;
			
		}
		break;

	case BCR2_PICK_R1_SAFETY://2014,1205
		{
// 			int nRet1,nRet2;
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R1, st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			if( 1 || nRet1 == BD_GOOD && nRet2 == BD_GOOD)
// 			{
// 				job.bcr2_step = BCR2_PICK_LABEL_ROTATE_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1305, CTL_dWARNING, alarm.mstr_code );
// 			}
//			for(int i=0; i<2; i++)
//			{
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[i], IO_OFF);
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[i], IO_ON);
//			}		
			ReqM2RotateFwdBwd( 0 ,BACKWARD);
			ReqM2RotateFwdBwd( 1 ,BACKWARD);
			job.bcr2_step = BCR2_PICK_R2_SAFETY;
			m_dwTime_Robot2 = GetCurrentTime();
		}
		break;

	case BCR2_PICK_R2_SAFETY://2014,1205
		{
// 			int nRet1;
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD)
// 			{
// 				job.bcr2_step = BCR2_PICK_LABEL_ROTATE_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1306, CTL_dWARNING, alarm.mstr_code );
// 			}
			int nRet1,nRet2,nRet3,nRet4;
//			if( GetCurrentTime() - m_dwTime_Robot2 < 0 ) m_dwTime_Robot2 = GetCurrentTime();
			if( m_fbm2rotatefwdbwd[0] != FB_NONE) break;
			if( m_fbm2rotatefwdbwd[1] != FB_NONE) break;

			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_OFF);//place position
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_ON); //pick position
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_ON);
			
			if(nRet1 == IO_OFF && nRet2 == IO_ON && nRet3 == IO_OFF && nRet4 == IO_ON)
			{
				//2015.0827
				mn_bcr_pick_error[0] = mn_bcr_pick_error[1] = CTL_NO;
				st_handler.mn_retest_cnt = 0;
				job.bcr2_step = BCR2_PICK_LABEL_MOVE_READY_POS;
			}		
			else
			{
				job.bcr2_step = BCR2_PICK_R1_SAFETY;
			}
		}
		break;

	case BCR2_PICK_LABEL_MOVE_READY_POS:
		{
			double dPos = COMI.Get_MotCurrentPos( M_BCR_ATTACH2_X);
			int nRet_1 = COMI.Check_MotPosRange(M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK], COMI.md_allow_value[M_BCR_ATTACH2_X]);//COMI.mn_allow_value[n_MotNum]
			if (nRet_1 != BD_GOOD) //YÃàÀÌ ÀÛ¾÷ °¡´ÉÇÑ À§Ä¡¿¡ ¿Í ÀÖ´ÂÁö È®ÀÎÇÑ´Ù 
			{
				job.bcr2_step = BCR2_PICK_LABEL_MOVE_READY;	//Ã³À½À¸·Î °¡¼­ ´Ù½Ã ½ÃÀÛÇÑ´Ù 
				break;
			}

			if( dPos > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				dPos < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_X])
			{
				job.bcr2_step = BCR2_PICK_LABEL_PICK_READY;
				break;
			}
			////2014,1216 ysj 
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[2], IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[3], IO_OFF);
			////

			job.bcr2_step = BCR2_PICK_LABEL_MOVE_READY;	
			
		}
		break;

	case BCR2_PICK_LABEL_MOVE_READY://2014,1205 M_BCR_ATTACH1_X -> M_BCR_ATTACH2_X º¯°æ
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr2_step = BCR2_PICK_LABEL_PICK_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1307, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		

	case BCR2_PICK_LABEL_PICK_READY:
		{
			if(job.iRobotFirst != -1)
			{
					g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[job.iRobotFirst + 2], IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_ON);//2014,1117
					ReqM2TopVac( job.iRobotFirst, BACKWARD);
//					ReqM2BtmVac( job.iRobotFirst, BACKWARD);
			}
			if(job.iRobotSecond != -1)
			{
					g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[job.iRobotSecond + 2], IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_ON);
					ReqM2TopVac( job.iRobotSecond, BACKWARD);
//					ReqM2BtmVac( job.iRobotSecond, FORWARD);
			}
			mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
			job.bcr2_step = BCR2_PICK_LABEL_DOWN;	
		}
		break;

	case BCR2_PICK_LABEL_DOWN:
		{
			int nRet1,nRet2;
			if(job.iRobotFirst != -1)
			{
				if( (st_handler.mn_retest_cnt == 0 && mn_bcr_pick_error[job.iRobotFirst] == CTL_NO) ||
					(st_handler.mn_retest_cnt >= 1 && mn_bcr_pick_error[job.iRobotFirst] == CTL_YES) )
				{
					nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_PICK], st_basic.nRunSpeed );
				}
				else
				{
					nRet1 = BD_GOOD;
				}
			}
			else
			{
				nRet1 = BD_GOOD;
			}
			if(job.iRobotSecond != -1)
			{				
				if( (st_handler.mn_retest_cnt == 0 && mn_bcr_pick_error[job.iRobotSecond] == CTL_NO) ||
					(st_handler.mn_retest_cnt >= 1 && mn_bcr_pick_error[job.iRobotSecond] == CTL_YES) )
				{
					nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_PICK], st_basic.nRunSpeed );
				}
				else
				{
					nRet2 = BD_GOOD;
				}
			}
			else
			{
				nRet2 = BD_GOOD;
			}

			if( nRet1 == BD_GOOD ) mn_mot_flag[1][0] = 1;
			if( nRet2 == BD_GOOD ) mn_mot_flag[1][1] = 1;
			if(mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1)
			{
				////2014.1120 ysj
				if( job.iRobotFirst != -1)
				{
					g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[job.iRobotFirst], IO_ON);
// 					g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[job.iRobotFirst], IO_ON);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_ON);//2014,1117
				}
				if( job.iRobotSecond != -1)
				{
					g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[job.iRobotSecond], IO_ON);
// 					g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[job.iRobotSecond], IO_ON);
					g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_ON);//2014,1117
				}
				
				job.bcr2_step = BCR2_PICK_LABEL_DOWN_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1308, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR2_PICK_LABEL_DOWN_CHK:
		{
			if(job.iRobotFirst != -1)
			{
				if( GetM2TopVac( job.iRobotFirst ) != FB_NONE ) break;
//				if( GetM2BtmVac( job.iRobotFirst ) != FB_NONE ) break;
			}
			if(job.iRobotSecond != -1)
			{
				if( GetM2TopVac( job.iRobotSecond ) != FB_NONE ) break;
//				if( GetM2BtmVac( job.iRobotSecond ) != FB_NONE ) break;
			}
			job.bcr2_step = BCR2_PICK_LABEL_VACUUM;	

		}
		break;

	case BCR2_PICK_LABEL_VACUUM:
		{
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[job.iRobotFirst], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[job.iRobotFirst], IO_ON);
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_ON);
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[job.iRobotSecond], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[job.iRobotSecond], IO_ON);
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_ON);
			}
			job.bcr2_step = BCR2_PICK_LABEL_VACUUM_CHK;	
			m_dwTime_Robot2 = GetCurrentTime();
		}
		break;

	case BCR2_PICK_LABEL_VACUUM_CHK:
		{
// 			if( GetCurrentTime() - m_dwTime_Robot2 < 0 ) m_dwTime_Robot2 = GetCurrentTime();
// 			if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_ON] ) break;

			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_ON);

				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
				{
				}
				else 
				{
					if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_limit_time[E_WAIT_BCR_PICK_VACUUM] ) break;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1309, CTL_dWARNING, strError );
					}
					else
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1310, CTL_dWARNING, strError );
					}
// 					break;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_ON);
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
				{
				}
				else 
				{
					if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_limit_time[E_WAIT_BCR_PICK_VACUUM] ) break;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1311, CTL_dWARNING, strError );
					}
					else
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1312, CTL_dWARNING, strError );
					}
// 					break;
				}
			}
			m_dwTime_Robot2 = GetCurrentTime();//2015.0120 ysj
			mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
			job.bcr2_step = BCR2_PICK_LABEL_UP;			
		}
		break;

	case BCR2_PICK_LABEL_UP:
		{
			if( GetCurrentTime() - m_dwTime_Robot2 < 0 ) m_dwTime_Robot2 = GetCurrentTime();
			if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_wait_time[E_WAIT_BCR_PICK_VACUUM][IO_ON] ) break;

			int nnSite = 0;
			if(job.iRobotFirst != -1) nnSite = job.iRobotFirst;
			else if(job.iRobotSecond != -1) nnSite = job.iRobotSecond;
			if( GetCurrentTime() - st_handler.m_lBcrOutTime[nnSite + 2] < 0 ) st_handler.m_lBcrOutTime[nnSite + 2]  = GetCurrentTime();
			if( GetCurrentTime() - st_handler.m_lBcrOutTime[nnSite + 2] > 60000 ) 
			{
				if( GetCurrentTime() - m_dwTime_Robot2 < 0 ) m_dwTime_Robot2 = GetCurrentTime();
				if( GetCurrentTime() - m_dwTime_Robot2 < 3500 ) break;
			}
			////
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1 , st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2 , st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[1][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[1][1] = 1;
			if(mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1)
			{
				job.bcr2_step = BCR2_PICK_LABEL_UP_VACUUM_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1315, CTL_dWARNING, alarm.mstr_code );
			}
		
		}
		break;

	case BCR2_PICK_LABEL_UP_VACUUM_CHK:
		{
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst+2], IO_OFF);
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
				{
				}
				else 
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
						if(mn_bcr_pick_error[job.iRobotFirst] == CTL_YES && mn_bcr_pick_error[job.iRobotFirst] == CTL_YES)//2016.0919
						{
							CString strError;
							strError.Format("5%04d0",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst]);
							CTL_Lib.Alarm_Error_Occurrence( 1316, CTL_dWARNING, strError );
						}
						else
						{
							mn_bcr_pick_error[job.iRobotFirst] = CTL_YES;
							st_handler.mn_retest_cnt++;
							mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
							job.bcr2_step = BCR2_PICK_LABEL_DOWN;
							break;
						}
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot2 Left Top Vacuum Off Check Error.";
////
					}
					else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
						CString strError;
						strError.Format("5%04d0",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst]);
						CTL_Lib.Alarm_Error_Occurrence( 1317, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot2 Left Bottom Vacuum Off Check Error.";
////
					}
					////2015.0120 ysj
// 					Func.OnSet_IO_Port_Stop();
// 					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					job.bcr2_step = BCR2_STEP(252);
					////
					break;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond+2], IO_OFF);
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
				{
				}
				else
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_OFF )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
						if(st_handler.mn_retest_cnt >= 2 && mn_bcr_pick_error[job.iRobotSecond] == CTL_YES)//2016.0919
						{
							CString strError;
							strError.Format("5%04d0",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond]);
							CTL_Lib.Alarm_Error_Occurrence( 1318, CTL_dWARNING, strError );
						}
						else
						{
							mn_bcr_pick_error[job.iRobotSecond] = CTL_YES;
							st_handler.mn_retest_cnt++;
							mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
							job.bcr2_step = BCR2_PICK_LABEL_DOWN;
							break;
						}
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot2 Left Top Vacuum Off Check Error.";
					}
					else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_OFF )
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
						CString strError;
						strError.Format("5%04d0",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond]);
						CTL_Lib.Alarm_Error_Occurrence( 1319, CTL_dWARNING, strError );
// 						st_msg.mstr_event_msg[0] = "Distribution Label Robot2 Left Bottom Vacuum Off Check Error.";
					}
// 					COMI.mn_run_status = dSTOP;
// 					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					job.bcr2_step = BCR2_STEP(252);
					break;
				}
			}
			job.bcr2_step = BCR2_PICK_LABEL_FINISH;	
			
			if( job.iRobotFirst != -1)
			{
// 				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst], IO_OFF);
				ReqM2TopVac( job.iRobotFirst, BACKWARD);
				ReqM2BtmVac( job.iRobotFirst, BACKWARD);
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst + 2], IO_OFF);
//				COMI.Start_SingleMove(M_BCR_FEEDER_1+job.iRobotFirst, st_motor[M_BCR_FEEDER_1+job.iRobotFirst].d_pos[P_BCR_FEEDER_PICK], st_basic.nRunSpeed);
			}
			if( job.iRobotSecond != -1)
			{
// 				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
				ReqM2TopVac( job.iRobotSecond, BACKWARD);
				ReqM2BtmVac( job.iRobotSecond, BACKWARD);
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotSecond + 2], IO_OFF);
//				COMI.Start_SingleMove(M_BCR_FEEDER_1+job.iRobotSecond, st_motor[M_BCR_FEEDER_1+job.iRobotSecond].d_pos[P_BCR_FEEDER_PICK], st_basic.nRunSpeed);
			}

		}
		break;
		
	case 251:
		if( m_fbm2rotatefwdbwd[EBVS_LEFT] != FB_NONE) break;
		if( m_fbm2rotatefwdbwd[EBVS_RIGHT] != FB_NONE) break;
		job.bcr2_step = BCR2_STEP(252);
		break;
		
	case 252:
		{
			bool nfwd1 = CheckM2RotateFwdBwd( EBVS_LEFT, IO_OFF );
			bool nfwd2 = CheckM2RotateFwdBwd( EBVS_RIGHT, IO_OFF );
			if( nfwd1 == true && nfwd2 == true )
			{
				ReqM2TopVac( EBVS_LEFT , FORWARD );
				ReqM2TopVac( EBVS_RIGHT , FORWARD );
				ReqM2BtmVac( EBVS_LEFT , BACKWARD );
				ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
				job.bcr2_step = BCR2_STEP(253);			
			}
			else
			{
				ReqM2RotateFwdBwd( EBVS_LEFT ,BACKWARD);
				ReqM2RotateFwdBwd( EBVS_RIGHT ,BACKWARD);
				job.bcr2_step = BCR2_STEP(251);			
			}
		}
		break;
		
	case 253:
		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
		job.bcr2_step = BCR2_STEP(254);
		break;

	case 254:
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				job.bcr2_step = BCR2_STEP(255);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 271, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 255:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1 , st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2 , st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[1][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[1][1] = 1;
			if(mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_LEFT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_RIGHT], IO_OFF);
				job.bcr2_step = BCR2_STEP(256);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 256:
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_RIGHT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_RIGHT], IO_ON);
		m_dwTime_Robot2 = GetCurrentTime();
		job.bcr2_step = BCR2_STEP(260);
		break;
		
	case 260:
		if( GetCurrentTime() - m_dwTime_Robot2 < 0) m_dwTime_Robot2 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		mn_mot_flag[1][0] = 0;
		mn_mot_flag[1][1] = 0;
		job.bcr2_step = BCR2_STEP(261);
		break;
		
	case 261:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1 , st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2 , st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[1][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[1][1] = 1;
			if(mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1)
			{
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_LEFT], IO_OFF);				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_RIGHT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_RIGHT], IO_OFF);				
// 				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				job.bcr2_step = BCR2_STEP(262);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 262:
		{
			ReqM2TopVac( EBVS_LEFT , BACKWARD );
			ReqM2TopVac( EBVS_RIGHT , BACKWARD );
			ReqM2BtmVac( EBVS_LEFT , BACKWARD );
			ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
			job.bcr2_step = BCR2_STEP(263);			
		}
		break;
		
	case 263:
		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr2_step = BCR2_STEP(264);
		break;

	case 264:
		if( st_basic.n_mode_device && job.iRobotFirst != -1)
		{
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515081 1 00 "Distribution Label Robot1 Left Top Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 1261, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515091 1 00 "Distribution Label Robot1 Left Bottom Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 1262, CTL_dWARNING, strError );
				break;
			}
			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst+2], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotFirst+2], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(1265, dWARNING, strError);
				break;
			}
		}
		if( st_basic.n_mode_device && job.iRobotSecond != -1)
		{
			//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 1263, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
			{
				//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 1264, CTL_dWARNING, strError );
				break;
			}
			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotSecond+2], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotSecond+2], IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(1266, dWARNING, strError);				
				break;
			}
		}
		job.bcr2_step = BCR2_STEP(265);
		break;
		
	case 265:
		if( job.iRobotFirst != -1)
		{
			ReqM2TopVac( job.iRobotFirst, BACKWARD);
			ReqM2BtmVac( job.iRobotFirst, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst + 2], IO_OFF);
		}
		if( job.iRobotSecond != -1)
		{
			ReqM2TopVac( job.iRobotSecond, BACKWARD);
			ReqM2BtmVac( job.iRobotSecond, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotSecond + 2], IO_OFF);
		}
		job.bcr2_step = BCR2_STEP(266);
		break;
		
	case 266:
		{			
			if( job.iRobotFirst != -1)
			{
				if( GetM2TopVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM2BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_OFF);//2014,1117
				
			}
			if( job.iRobotSecond != -1)
			{
				if( GetM2TopVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM2BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_OFF);//2014,1117
			}
			
			if( job.iRobotFirst != -1)
			{
// 				st_bcr_feeder[job.iRobotFirst + 2].st_bcr.bExist = false;
// 				memset( &st_bcr_feeder[job.iRobotFirst + 2], 0x00, sizeof(st_bcr_feeder[job.iRobotFirst + 2]));
				st_bcr_feeder[job.iRobotFirst].st_bcr.nState = EMBS_FAIL;
			}
			if( job.iRobotSecond != -1)
			{				
// 				st_bcr_feeder[job.iRobotSecond + 2].st_bcr.bExist = false;
// 				memset( &st_bcr_feeder[job.iRobotSecond + 2], 0x00, sizeof(st_bcr_feeder[job.iRobotSecond + 2]));
				st_bcr_feeder[job.iRobotSecond].st_bcr.nState = EMBS_FAIL;
			}			
			
			Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();
			Func.Handler_Recovery_Data_Write();
		}
		break;

////
	case BCR2_PICK_LABEL_FINISH:
		{

			if( job.iRobotFirst != -1)
			{
				if( GetM2TopVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM2BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_OFF);//2014,1117
				
			}
			if( job.iRobotSecond != -1)
			{
				if( GetM2TopVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM2BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_OFF);//2014,1117
			}

			if( job.iRobotFirst != -1)
			{
				st_bcr_rbt[1].bPick[job.iRobotFirst] = st_bcr_feeder[job.iRobotFirst + 2].st_bcr.bOut;
				st_bcr_feeder[job.iRobotFirst + 2].st_bcr.bExist = false;
				memcpy( &st_bcr_rbt[1].st_bcr[job.iRobotFirst], &st_bcr_feeder[job.iRobotFirst + 2].st_bcr, sizeof(st_bcr_feeder[job.iRobotFirst + 2].st_bcr));
				memset( &st_bcr_feeder[job.iRobotFirst + 2], 0x00, sizeof(st_bcr_feeder[job.iRobotFirst + 2]));
			}
			if( job.iRobotSecond != -1)
			{
				st_bcr_rbt[1].bPick[job.iRobotSecond] = st_bcr_feeder[job.iRobotSecond + 2].st_bcr.bOut;
				st_bcr_feeder[job.iRobotSecond + 2].st_bcr.bExist = false;
				memcpy( &st_bcr_rbt[1].st_bcr[job.iRobotSecond], &st_bcr_feeder[job.iRobotSecond + 2].st_bcr, sizeof(st_bcr_feeder[job.iRobotSecond + 2].st_bcr));
				memset( &st_bcr_feeder[job.iRobotSecond + 2], 0x00, sizeof(st_bcr_feeder[job.iRobotSecond + 2]));
			}
		
			// UI ¾÷µ¥ÀÌÆ®
			if( st_handler.cwnd_main )
			{
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY);
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDR_PICKER);
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDS_TESTSITE);
			}
// 			Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();
// 			Func.Handler_Recovery_Data_Write();
			job.bcr2_step = BCR2_STEP(271);
			mn_mot_flag[1][0]=0;mn_mot_flag[1][1]=0;mn_mot_flag[1][2]=0;
		}
		break;

	////2014,1205
	case 271://2014,1205
		{
			int nRet1;
			nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_R_BCR_PLACE], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				mn_mot_flag[1][0] = 0; mn_mot_flag[1][1] = mn_mot_flag[1][2] = 0; //clear 
				job.bcr2_step = BCR2_STEP(272);			
				if( job.iRobotFirst != -1)
				{
					ReqM2RotateFwdBwd( job.iRobotFirst , FORWARD);
				}
				if( job.iRobotSecond != -1)
				{
					ReqM2RotateFwdBwd( job.iRobotSecond , FORWARD);
				}
			}
			else if( nRet1== BD_ERROR || nRet1 == BD_SAFETY  )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1320, CTL_dWARNING, alarm.mstr_code );
			}			
		}
		break;
		
	case 272:
		{
			int nRet1 = COMI.Check_SingleMove(M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_R_BCR_PLACE]);  // ÀÌµ¿ ¿Ï·á ?
			if( nRet1 == BD_GOOD)
			{
				mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
				job.bcr2_step = BCR2_STEP(273);
				m_dwTime_Robot2 = GetCurrentTime();//2014,1205
// 				if( job.iRobotFirst != -1)
// 				{
// 					ReqM2RotateFwdBwd( job.iRobotFirst ,FORWARD);
// 				}
// 				if( job.iRobotSecond != -1)
// 				{
// 					ReqM2RotateFwdBwd( job.iRobotSecond ,FORWARD);
// 				}
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				job.bcr2_step = BCR2_STEP(271);
				CTL_Lib.Alarm_Error_Occurrence( 1321, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
		
	case 273://roller ready//2014,1205
		{
			int nRet1,nRet2,nRet3,nRet4;
			CString strError;

// 			if( job.iRobotFirst != -1)
 				if( m_fbm2rotatefwdbwd[0] != FB_NONE) break;//2015.0120 ysj
// 			if( job.iRobotSecond != -1)
 				if( m_fbm2rotatefwdbwd[1] != FB_NONE) break;//2015.0120 ysj
////2015.0120 ysj
// 			if( m_fbm2rotatefwdbwd[0] != FB_NONE) break;
// 			if( m_fbm2rotatefwdbwd[1] != FB_NONE) break;
////
//			if( GetCurrentTime() - m_dwTime_Robot2 < 0 ) m_dwTime_Robot2 = GetCurrentTime();
			if( job.iRobotFirst != -1)
			{
				nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_ON);//place position
				nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_OFF); //pick position
			}
			else
			{
				nRet1 = IO_ON;
				nRet2 = IO_OFF;
			}
			if( job.iRobotSecond != -1)
			{
				nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_ON);
				nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_OFF);
			}
			else
			{
				nRet3 = IO_ON;
				nRet4 = IO_OFF;
			}			
			
			if(nRet1 == IO_ON && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_OFF)
			{
				Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();
				Func.Handler_Recovery_Data_Write();
			}
			else
			{
				if(nRet1 != IO_ON) 
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 1322, CTL_dWARNING, strError );
				}
				else if(nRet2 != IO_OFF)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 1323, CTL_dWARNING, strError );
				}
				else if(nRet3 != IO_ON)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 1324, CTL_dWARNING, strError );
				}
				else if(nRet4 != IO_OFF)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 1325, CTL_dWARNING, strError );
				}
			}
		}
		break;



	////
	case BCR2_PLACE_LABEL_READY:
		{//2015.0327
			bool bWork = true;
			int nRet1;
			CString strError,strRbt,strIDBuff;
			if( job.iRobotFirst != -1)
			{
				if( st_bcr_rbt[1].st_bcr[job.iRobotFirst].nState == EMBS_FAIL )
				{
					bWork = false;
				}
				//2016.0916
				strRbt.Format("%s", st_bcr_rbt[1].st_bcr[0].c_serial);
				strIDBuff.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[0]);
				strError = strIDBuff.Left(strIDBuff.GetLength()-1);
				if( !Func.CmpStrFile(strRbt, strError) )
				{
					strError = _T( "650000" );
					CTL_Lib.Alarm_Error_Occurrence( 7269, CTL_dWARNING, strError );
					break;
				}
			}
			if( job.iRobotSecond != -1)
			{
				if( st_bcr_rbt[1].st_bcr[job.iRobotSecond].nState == EMBS_FAIL )
				{
					bWork = false;
				}
				//2016.0916
				strRbt.Format("%s", st_bcr_rbt[1].st_bcr[1].c_serial);
				strIDBuff.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[1]);
				strError = strIDBuff.Left(strIDBuff.GetLength()-1);
				if( !Func.CmpStrFile(strRbt, strError) )
				{
					strError = _T( "650000" );
					CTL_Lib.Alarm_Error_Occurrence( 7369, CTL_dWARNING, strError );
					break;
				}
			}
			
			if(bWork == false)//reject
			{
				job.bcr2_step = BCR2_STEP(432);
			}
			else
			{
				nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_R_BCR_PLACE], st_basic.nRunSpeed );
				if( nRet1 == BD_GOOD )
				{
					mn_mot_flag[1][0] = 0; mn_mot_flag[1][1] = mn_mot_flag[1][2] = 0; //clear 
					job.bcr2_step = BCR2_PLACE_LABEL_X_READY;
				}			
				else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
				{
					CTL_Lib.Alarm_Error_Occurrence( 1327, CTL_dWARNING, alarm.mstr_code );
				}
			}			
			
		}
		break;

	////2014,1205
	case BCR2_PLACE_LABEL_X_READY:
		{
			int nRet1 = COMI.Check_SingleMove(M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_R_BCR_PLACE]);  // ÀÌµ¿ ¿Ï·á È
			
			if( nRet1 == BD_GOOD)
			{
				mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
				job.bcr2_step = BCR2_PLACE_LABEL_READY2;
				m_dwTime_Robot2 = GetCurrentTime();
				//if( job.iRobotFirst != -1)
				//{
					ReqM2RotateFwdBwd( 0 ,FORWARD);
				//}
				//if( job.iRobotSecond != -1)
				//{
					ReqM2RotateFwdBwd( 1 ,FORWARD);
				//}
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				job.bcr2_step = BCR2_PLACE_LABEL_READY;
				CTL_Lib.Alarm_Error_Occurrence( 1328, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	////
	case BCR2_PLACE_LABEL_READY2://roller ready//2014,1205
		{			
			int nRet1,nRet2,nRet3,nRet4;
			CString strError;
			
			if( m_fbm2rotatefwdbwd[0] != FB_NONE) break;
			if( m_fbm2rotatefwdbwd[1] != FB_NONE) break;
			
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_ON);//place position
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_OFF); //pick position
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_ON);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_OFF);
			
			if(nRet1 == IO_ON && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_OFF)
			{
				double dpos = COMI.Get_MotCurrentPos(M_INDEX_X);
				if( dpos > st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS] - COMI.md_allow_value[M_INDEX_X] && dpos < st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS] + COMI.md_allow_value[M_INDEX_X] )
				{
					job.bcr2_step = BCR2_PLACE_LABEL_DOWN;
				}
				else
				{
					job.bcr2_step = BCR2_PLACE_LABEL_ROLLER_X_POS;
				}
			}
			else
			{
				if( job.iRobotFirst != -1)
				{
					if(nRet1 != IO_ON) 
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 1329, CTL_dWARNING, strError );
					}
					else if(nRet2 != IO_OFF)
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 1330, CTL_dWARNING, strError );
					}
				}
				if( job.iRobotSecond != -1)
				{
					if(nRet3 != IO_ON)
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 1331, CTL_dWARNING, strError );
					}
					else if(nRet4 != IO_OFF)
					{
						strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 1332, CTL_dWARNING, strError );
					}
				}
			}			

		}
		break;

	case BCR2_PLACE_LABEL_ROLLER_X_POS:
		{
			int	nRet = CTL_Lib.Single_Move( M_INDEX_X, st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bcr2_step = BCR2_PLACE_LABEL_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1333, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;	


	case BCR2_PLACE_LABEL_DOWN:
		{//Á¦Ç°ÀÌ µµ´ÞµÇ¾î ÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù.

			int nRet1,nRet2;
// 			if(job.iRobotFirst != -1)
// 			{
				nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_DOWN], st_basic.nRunSpeed );
// 			}
// 			else
// 			{
// 				nRet1 = BD_GOOD;
// 			}
// 			if(job.iRobotSecond != -1)
// 			{				
				nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_DOWN], st_basic.nRunSpeed );
// 			}
// 			else
// 			{
// 				nRet2 = BD_GOOD;
// 			}
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD/* && nRet3 == BD_GOOD*/)
			{
				job.bcr2_step = BCR2_PLACE_LABEL_PUSH;
				ReqBoxUpDn(1, FORWARD);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY/* || nRet3 == BD_ERROR || nRet3 == BD_SAFETY */)
			{
				CTL_Lib.Alarm_Error_Occurrence( 1334, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;
	
	case BCR2_PLACE_LABEL_PUSH://2014,1205
		{
			int nRet3 = CTL_Lib.Single_Move( M_INDEX_ROLLER2, st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_POS], st_basic.nRunSpeed );
			if( nRet3 == BD_GOOD)
			{
// 				if(job.iRobotFirst != -1)
// 				{
// 					ReqM2TopVac( job.iRobotFirst, FORWARD);
// 					ReqRollingFwdBwd( 1, job.iRobotFirst, FORWARD);
					ReqM2TopVac( 0, FORWARD);
					ReqRollingFwdBwd( 1, 0, FORWARD);
// 				}
// 				if(job.iRobotSecond != -1)
// 				{
// 					ReqM2TopVac( job.iRobotSecond, FORWARD);
// 					ReqRollingFwdBwd( 1, job.iRobotSecond, FORWARD);
					ReqM2TopVac( 1, FORWARD);
					ReqRollingFwdBwd( 1, 1, FORWARD);
//				}
				//ReqRollingFwdBwd( 2, FORWARD);
			
				job.bcr2_step = BCR2_PLACE_LABEL_XPLACE;
			}
			else if( nRet3 == BD_ERROR || nRet3 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1335, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	////2014,1205
	case BCR2_PLACE_LABEL_XPLACE:
		{
			if( m_fbboxupdn[1] != FB_NONE ) break;
			if( st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] > (st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] + 5) || st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] < (st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_DOWN]))
			{
				if(st_handler.cwnd_list != NULL)
				{
// 					sprintf(st_msg.c_abnormal_msg, "[BAR ROBOT] Picker Down pos is more than Picker Pick(5mm) or is less than. Position Error Check ");
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					break;
				}
			}
// 			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD )
// 			{
				job.bcr2_step = BCR2_PLACE_LABEL_PUSH_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1336, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;		
		
	////
	case BCR2_PLACE_LABEL_PUSH_CHK:
		{
// 			if( job.iRobotFirst != -1)
// 			{
// 				if( GetM2TopVac( job.iRobotFirst ) != FB_NONE ) break;
// 				ReqM2BtmVac( job.iRobotFirst, FORWARD);
// 				if( GetRollingFwdBwd( 1, job.iRobotFirst ) != FB_NONE ) break;
				if( GetM2TopVac( 0 ) != FB_NONE ) break;
				ReqM2BtmVac( 0, FORWARD);
				if( GetRollingFwdBwd( 1, 0 ) != FB_NONE ) break;
// 			}
// 			if( job.iRobotSecond != -1)
// 			{
				if( GetM2TopVac( 1 ) != FB_NONE ) break;
				ReqM2BtmVac( 1, FORWARD);
				if( GetRollingFwdBwd( 1, 1 ) != FB_NONE ) break;
// 			}
			st_var.n_tboxmv_sealing_cv_mode = CTL_YES;//2015.0331
			//if( GetRollingFwdBwd( 2 ) != FB_NONE ) break;
			job.bcr2_step = BCR2_PLACE_LABEL_PUSH_VACUUM;

		}
		break;

	case BCR2_PLACE_LABEL_PUSH_VACUUM:
		{
// 			if( job.iRobotFirst != -1)
// 				if( GetM2BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM2BtmVac( 0 ) != FB_NONE ) break;
// 			if( job.iRobotSecond != -1)
// 				if( GetM2BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM2BtmVac( 1 ) != FB_NONE ) break;

				st_var.n_tboxmv_sealing_cv_mode = CTL_NO;//2015.0331
					
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[0], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[1], IO_OFF);
			job.bcr2_step = BCR2_PLACE_LABEL_VACUUM_CHK;	
		}
		break;

	case BCR2_PLACE_LABEL_VACUUM_CHK:
		{
			if(job.iRobotFirst != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[job.iRobotFirst], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[job.iRobotFirst], IO_ON);
// 				ReqM2BtmVac( job.iRobotFirst, BACKWARD);
// 				ReqRollingFwdBwd( 1, job.iRobotFirst, BACKWARD);
			}
			ReqM2BtmVac( 0, BACKWARD);
			ReqRollingFwdBwd( 1, 0, BACKWARD);
			if(job.iRobotSecond != -1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[job.iRobotSecond], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[job.iRobotSecond], IO_ON);
// 				ReqM2BtmVac( job.iRobotSecond, BACKWARD);
// 				ReqRollingFwdBwd( 1, job.iRobotSecond, BACKWARD);
			}
			ReqM2BtmVac( 1, BACKWARD);
			ReqRollingFwdBwd( 1, 1, BACKWARD);
//			ReqRollingFwdBwd( 2, BACKWARD);
			job.bcr2_step = BCR2_PLACE_LABEL_PULL;
			m_dwTime_Robot2 = GetCurrentTime();
		}		
		break;

	case BCR2_PLACE_LABEL_PULL://2014,1205
		if( GetCurrentTime() - m_dwTime_Robot2 < 0) m_dwTime_Robot2 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		{
			if(job.iRobotFirst != -1)
			{
// 				if( GetM2BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[job.iRobotFirst], IO_OFF);
// 				ReqM2TopVac( job.iRobotFirst, BACKWARD);
// 				if( GetRollingFwdBwd( 1, job.iRobotFirst ) != FB_NONE ) break;
			}
			if(job.iRobotSecond != -1)
			{
// 				if( GetM2BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[job.iRobotSecond], IO_OFF);
// 				ReqM2TopVac( job.iRobotSecond, BACKWARD);
// 				if( GetRollingFwdBwd( 1, job.iRobotSecond ) != FB_NONE ) break;
			}
			if( GetM2BtmVac( 0 ) != FB_NONE ) break;
			ReqM2TopVac( 0, BACKWARD);
			if( GetRollingFwdBwd( 1, 0 ) != FB_NONE ) break;
			if( GetM2BtmVac( 1 ) != FB_NONE ) break;
			ReqM2TopVac( 1, BACKWARD);
			if( GetRollingFwdBwd( 1, 1 ) != FB_NONE ) break;
//			if( GetRollingFwdBwd( 2 ) != FB_NONE ) break;

			job.bcr2_step = BCR2_PLACE_LABEL_PULL_CHK;		
		}
		break;
		
	case BCR2_PLACE_LABEL_PULL_CHK:
		{
			if(job.iRobotFirst != -1)
			{
// 				if( GetM2TopVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[job.iRobotFirst], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[job.iRobotFirst], IO_OFF);
			}
			if( GetM2TopVac( 0 ) != FB_NONE ) break;
			if(job.iRobotSecond != -1)
			{
// 				if( GetM2TopVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[job.iRobotSecond], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[job.iRobotSecond], IO_OFF);
			}
			if( GetM2TopVac( 1 ) != FB_NONE ) break;
			job.bcr2_step = BCR2_PLACE_LABEL_VACUUM_RECHK;	
			m_dwTime_Robot2 = GetCurrentTime();
		}
		break;

	case BCR2_PLACE_LABEL_VACUUM_RECHK:
		if( GetCurrentTime() - m_dwTime_Robot2 < 0) m_dwTime_Robot2 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		{
			if( st_basic.n_mode_device && job.iRobotFirst != -1)
			{
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_OFF )
				{
				}
				else 
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_OFF) == IO_ON )
					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1337, CTL_dWARNING, strError );
					}
					else
					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1338, CTL_dWARNING, strError );
					}
					//break;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1)
			{
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_OFF) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond], IO_OFF) == IO_OFF )
				{
				}
				else if( GetCurrentTime() - m_dwTime_Robot2 > st_time.n_limit_time[E_WAIT_BCR_PICK_VACUUM] )
				{
// 					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_OFF) == IO_ON )
// 					{//515080 1 00 "Distribution Label Robot1 Left Top Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1339, CTL_dWARNING, strError );
// 					}
// 					else
// 					{//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
// 						CString strError;
// 						strError.Format("5%04d0",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond]);
// 						CTL_Lib.Alarm_Error_Occurrence( 1340, CTL_dWARNING, "515090" );
// 					}
// 					break;
				}
			}
			ReqBoxUpDn(1, BACKWARD);//2014,1205
			job.bcr2_step = BCR2_PLACE_LABEL_XDOWN_BACK;// BCR2_PLACE_LABEL_UP;//2014,1205	
		}
		break;

	case BCR2_PLACE_LABEL_XDOWN_BACK:
		{
			job.bcr2_step = BCR2_PLACE_LABEL_UP;
			mn_mot_flag[1][0] = 0;
			mn_mot_flag[1][1] = 0;
			mn_mot_flag[1][2] = 0;
		}
		break;
		
	case 431:
		if( m_fbm2rotatefwdbwd[EBVS_LEFT] != FB_NONE) break;
		if( m_fbm2rotatefwdbwd[EBVS_RIGHT] != FB_NONE) break;
		job.bcr2_step = BCR2_STEP(432);
		break;

		
	case 432:
		{
			bool nfwd1 = CheckM2RotateFwdBwd( EBVS_LEFT, IO_OFF );
			bool nfwd2 = CheckM2RotateFwdBwd( EBVS_RIGHT, IO_OFF );
			if( nfwd1 == true && nfwd2 == true )
			{
				ReqM2TopVac( EBVS_LEFT , FORWARD );
				ReqM2TopVac( EBVS_RIGHT , FORWARD );
				ReqM2BtmVac( EBVS_LEFT , BACKWARD );
				ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
				job.bcr2_step = BCR2_STEP(433);			
			}
			else
			{
				ReqM2RotateFwdBwd( EBVS_LEFT ,BACKWARD);
				ReqM2RotateFwdBwd( EBVS_RIGHT ,BACKWARD);
				job.bcr2_step = BCR2_STEP(431);			
			}
		}
		break;
		
	case 433:
		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		mn_mot_flag[1][0] = mn_mot_flag[1][1] = 0;
		job.bcr2_step = BCR2_STEP(434);
		break;

	case 434:
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				job.bcr2_step = BCR2_STEP(435);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 471, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 435:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1 , st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2 , st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_ERROR], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[1][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[1][1] = 1;
			if(mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_LEFT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_RIGHT], IO_OFF);
				job.bcr2_step = BCR2_STEP(436);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 472, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 436:
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_LEFT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_RIGHT], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_RIGHT], IO_ON);
		m_dwTime_Robot2 = GetCurrentTime();
		job.bcr2_step = BCR2_STEP(440);
		break;
		
	case 440:
		if( GetCurrentTime() - m_dwTime_Robot2 < 0) m_dwTime_Robot2 = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Robot2 < st_time.n_wait_time[E_WAIT_BCR_PLACE_EJECT][IO_ON]) break;
		mn_mot_flag[1][0] = 0;
		mn_mot_flag[1][1] = 0;
		job.bcr2_step = BCR2_STEP(441);
		break;
		
	case 441:
		{
			int nRet1,nRet2;
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1 , st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2 , st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[1][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[1][1] = 1;
			if(mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1)
			{
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_LEFT], IO_OFF);				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[EBVS_RIGHT], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_LEFT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[EBVS_RIGHT], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[EBVS_RIGHT], IO_OFF);				
// 				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				job.bcr2_step = BCR2_STEP(442);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 473, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		
	case 442:
		{
			ReqM2TopVac( EBVS_LEFT , BACKWARD );
			ReqM2TopVac( EBVS_RIGHT , BACKWARD );
			ReqM2BtmVac( EBVS_LEFT , BACKWARD );
			ReqM2BtmVac( EBVS_RIGHT, BACKWARD );
			job.bcr2_step = BCR2_STEP(443);			
		}
		break;
		
	case 443:
		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) break;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) break;
		job.bcr2_step = BCR2_STEP(444);
		break;

	case 444:
		if( st_basic.n_mode_device && job.iRobotFirst != -1)
		{
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515081 1 00 "Distribution Label Robot1 Left Top Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 474, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst], IO_ON) == IO_ON )
			{//515091 1 00 "Distribution Label Robot1 Left Bottom Vacuum On Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 475, CTL_dWARNING, strError );
				break;
			}
// 			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotFirst+2], IO_ON) == IO_ON)
// 			{
// 				CString strError;
// 				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotFirst+2], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence(476, dWARNING, strError);
// 				break;
// 			}
		}
		if( st_basic.n_mode_device && job.iRobotSecond != -1)
		{
			//  				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond], IO_OFF);
			if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON)
			{
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_top_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 477, CTL_dWARNING, strError );
				break;
			}
			else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond], IO_ON) == IO_ON )
			{
				//515090 1 00 "Distribution Label Robot1 Left Bottom Vacuum Off Check Error."
				CString strError;
				strError.Format("5%04d1",st_io.i_distribute_rbt2_btm_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 478, CTL_dWARNING, strError );
				break;
			}
// 			else if(g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[job.iRobotSecond+2], IO_ON) == IO_ON)
// 			{
// 				CString strError;
// 				strError.Format("5%04d%d", st_io.i_bcr_label_chk[job.iRobotSecond+2], IO_ON);
// 				CTL_Lib.Alarm_Error_Occurrence(479, dWARNING, strError);				
// 				break;
// 			}
		}
		job.bcr2_step = BCR2_STEP(445);
		break;
		
	case 445:
		if( job.iRobotFirst != -1)
		{
			ReqM2TopVac( job.iRobotFirst, BACKWARD);
			ReqM2BtmVac( job.iRobotFirst, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotFirst + 2], IO_OFF);
		}
		if( job.iRobotSecond != -1)
		{
			ReqM2TopVac( job.iRobotSecond, BACKWARD);
			ReqM2BtmVac( job.iRobotSecond, BACKWARD);
			g_ioMgr.set_out_bit(st_io.o_distribute_plate_updown[job.iRobotSecond + 2], IO_OFF);
		}
		job.bcr2_step = BCR2_STEP(446);
		break;
		
	case 446:
		{			
			if( job.iRobotFirst != -1)
			{
				if( GetM2TopVac( job.iRobotFirst ) != FB_NONE ) break;
				if( GetM2BtmVac( job.iRobotFirst ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotFirst + 2], IO_OFF);//2014,1117
				
			}
			if( job.iRobotSecond != -1)
			{
				if( GetM2TopVac( job.iRobotSecond ) != FB_NONE ) break;
				if( GetM2BtmVac( job.iRobotSecond ) != FB_NONE ) break;
				g_ioMgr.set_out_bit(st_io.o_distribute_eject_vacuum[job.iRobotSecond + 2], IO_OFF);//2014,1117
			}
			
			if( job.iRobotFirst != -1)
			{
				st_bcr_rbt[1].bPick[job.iRobotFirst] = 0;
				st_bcr_rbt[1].st_bcr[job.iRobotFirst].bExist = 0;
				st_bcr_rbt[1].st_bcr[job.iRobotFirst].bOut = 0;
			}
			if( job.iRobotSecond != -1)
			{				
				st_bcr_rbt[1].bPick[job.iRobotSecond] = 0;					
				st_bcr_rbt[1].st_bcr[job.iRobotSecond].bExist = 0;
				st_bcr_rbt[1].st_bcr[job.iRobotSecond].bOut = 0;
			}			
			
			Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();
			Func.Handler_Recovery_Data_Write();
		}
		break;
	////
	case BCR2_PLACE_LABEL_UP://2014,1205
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_UP], st_basic.nRunSpeed );
			int nRet3 = CTL_Lib.Single_Move( M_INDEX_ROLLER2, st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_SAGETY], st_basic.nRunSpeed );
			if(nRet1 == BD_GOOD) mn_mot_flag[1][0] = 1;
			if(nRet2 == BD_GOOD) mn_mot_flag[1][1] = 1;
			if(nRet3 == BD_GOOD) mn_mot_flag[1][2] = 1;
			if( mn_mot_flag[1][0] == 1 && mn_mot_flag[1][1] == 1 && mn_mot_flag[1][2] == 1 )
			{
				job.bcr2_step = BCR2_PLACE_LABEL_SAFETY;
				mn_mot_flag[1][0] = 0;
				mn_mot_flag[1][1] = 0;
				mn_mot_flag[1][2] = 0;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 1341, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BCR2_PLACE_LABEL_SAFETY://2014,1205
		{
// 			int nRet1,nRet2,nRet3;
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
// // 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R1, st_motor[M_BCR_ATTACH1_R1].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// // 			nRet3 = CTL_Lib.Single_Move( M_BCR_ATTACH1_R2, st_motor[M_BCR_ATTACH1_R2].d_pos[P_R_BCR_PICK], st_basic.nRunSpeed );
// 			nRet2 = nRet3 = BD_GOOD;
// 			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD && nRet3 == BD_GOOD)
// 			{
// 				job.bcr2_step = BCR2_PLACE_LABEL_FINISH;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1342, CTL_dWARNING, alarm.mstr_code );
// 			}
	
			int nRet1;
			nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[0], IO_OFF);//place pos
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[0], IO_ON);//pick pos
//				
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[1], IO_OFF);
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[1], IO_ON);
				
				ReqM2RotateFwdBwd( 0, BACKWARD );
				ReqM2RotateFwdBwd( 1, BACKWARD );
				job.bcr2_step = BCR2_STEP(461);
				mn_mot_flag[1][0] = 0;
				mn_mot_flag[1][1] = 0;
				mn_mot_flag[1][2] = 0;
				m_dwTime_Robot2 = GetCurrentTime();
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1343, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	////2014,1205
	case 461://2014,1205
		{
			int nRet = COMI.Check_SingleMove( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] );
			if( nRet == BD_GOOD )
			{
				ReqM2RotateFwdBwd( 0, BACKWARD );
				ReqM2RotateFwdBwd( 1, BACKWARD );
				job.bcr2_step = BCR2_STEP(462);
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				job.bcr2_step = BCR2_PLACE_LABEL_SAFETY;
				CTL_Lib.Alarm_Error_Occurrence( 1348, CTL_dWARNING, alarm.mstr_code );
			}			
			
		}
		break;
	////
		
	case 462:
		{
			if( m_fbm2rotatefwdbwd[0] != FB_NONE) break;
			if( m_fbm2rotatefwdbwd[1] != FB_NONE) break;
			
			int nRet1,nRet2,nRet3,nRet4;
			CString strError;
			
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_OFF);//place position
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_ON); //pick position
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_ON);	
			
			if(nRet1 == IO_OFF && nRet2 == IO_ON && nRet3 == IO_OFF && nRet4 == IO_ON)
			{
				job.bcr2_step = BCR2_PLACE_LABEL_FINISH;
			}
			else
			{
				if(nRet1 != IO_OFF) 
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 1344, CTL_dWARNING, strError );
				}
				else if(nRet2 != IO_ON)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[0]);
					CTL_Lib.Alarm_Error_Occurrence( 1345, CTL_dWARNING, strError );
				}
				else if(nRet3 != IO_OFF)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 1346, CTL_dWARNING, strError );
				}
				else if(nRet4 != IO_ON)
				{
					strError.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[1]);
					CTL_Lib.Alarm_Error_Occurrence( 1347, CTL_dWARNING, strError );
				}
			}
		}

		break;
		
	case BCR2_PLACE_LABEL_FINISH:
		{
			if(m_fbboxupdn[1] != FB_NONE ) break;
//			ABcrLabel* pbcr_1 = NULL;
//			ABcrLabel* pbcr_2 = NULL;
//			int nTrayIdx = -1;
//			if( job.GetPosByTarget() == MPOS_INDEX_BCR2_LOAD )
//			{
//				nTrayIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE2 );
//				pbcr_1 = &g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst % MPOS_DIVISION ).GetBcrLabel();
//				pbcr_2 = &g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst + 1 % MPOS_DIVISION ).GetBcrLabel();
//			}
//
//			if( job.iRobotFirst != -1 && pbcr_1)
//			{
//				*pbcr_1 = Func.m_pRobot[SITE_BARCODE2]->GetPicker(job.iRobotFirst).GetBcrLabel();
//				Func.m_pRobot[SITE_BARCODE2]->GetPicker(job.iRobotFirst).GetBcrLabel().EraseData();
//				//pMdl_1->CalcModuleState_PlaceTray();
//			}
//			if( job.iRobotSecond != -1 && pbcr_2)
//			{
//				*pbcr_2 = Func.m_pRobot[SITE_BARCODE2]->GetPicker(job.iRobotSecond).GetBcrLabel();
//				Func.m_pRobot[SITE_BARCODE2]->GetPicker(job.iRobotSecond).GetBcrLabel().EraseData();
//				//pMdl_1->CalcModuleState_PlaceTray();
//			}
			
			// UI ¾÷µ¥ÀÌÆ®
//			if( st_handler.cwnd_main )
//			{
//				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY);
//				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDR_PICKER);
//
////				if( job.GetPosByTarget() == MPOS_INDEX_TESTSITE )
////					st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDS_TESTSITE);
//
//			}
//			int iIdx = g_Tray.GetIdxByPos(IDPOS_BARCODE2);			
//			ATray& trayPri = g_Tray.GetTray( iIdx );
//			trayPri.SetWorkState(EWS_BARCODE2);		

			if( job.iRobotFirst != -1)
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_LEFT] = CTL_YES;
				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].st_barcode2[job.iRobotFirst], &st_bcr_rbt[1].st_bcr[job.iRobotFirst], sizeof(st_bcrinfo_param));
			}
			if( job.iRobotSecond != -1)
			{				
				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_RIGHT] = CTL_YES;
				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].st_barcode2[job.iRobotSecond], &st_bcr_rbt[1].st_bcr[job.iRobotSecond], sizeof(st_bcrinfo_param));
			}

			////2014,1205
			if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
			{
				if( job.iRobotFirst != -1)
				{
					st_bcr_rbt[1].bPick[job.iRobotFirst] = 0;
					st_bcr_rbt[1].st_bcr[job.iRobotFirst].bExist = 0;
					st_bcr_rbt[1].st_bcr[job.iRobotFirst].bOut = 0;
				}
				if( job.iRobotSecond != -1)
				{
					st_bcr_rbt[1].bPick[job.iRobotSecond] = 0;					
					st_bcr_rbt[1].st_bcr[job.iRobotSecond].bExist = 0;
					st_bcr_rbt[1].st_bcr[job.iRobotSecond].bOut = 0;
				}
				memset( &st_bcr_rbt[1], 0x00, sizeof(st_bcr_rbt[1]));
			}			
			////
			
			Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();
//			Func.BackupData();//2013,1018

			st_sync.n_bcr_2_work_req = CTL_REQ;

			if(st_handler.n_only_work_bcr1 == CTL_YES)
			{
				//2014,1117
				if( job.iRobotFirst != -1)
					st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[job.iRobotFirst].nComplete = CTL_NO;
				if( job.iRobotSecond != -1)
					st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[job.iRobotSecond].nComplete = CTL_NO;
				st_sync.n_bcr_2_work_req = CTL_READY;
			}
			
// 			if(GetCurrentTime() - m_dwTimeTact <= 0 ) m_dwTimeTact = GetCurrentTime();
// 			if( GetCurrentTime() - m_dwTimeTact >= 0 && GetCurrentTime() - m_dwTimeTact < 900000 &&
// 				st_handler.cwnd_list != NULL)
// 			{
// 				sprintf(st_msg.c_normal_msg, "[BARCODE2]Tact time : %d", GetCurrentTime() - m_dwTimeTact);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 				Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
// 			}
// 			m_dwTimeTact = GetCurrentTime();			
			
			Func.Handler_Recovery_Data_Write();
			st_handler.mn_btm_ready = 1;
			////
		}
		break;
	}

}

void CRun_Barcode_Works::Run_Reinstate1()
{
	if(st_handler.mn_Rcvy_state[RCVY_BARCODE1] == CTL_YES) return;
	int nRet[4];

	switch(mn_RcvyStep1)
	{
	case 0:
		{
			nRet[0] = COMI.Check_MotPosRange(M_BCR_ATTACH1_X, COMI.md_cmdpos_backup[M_BCR_ATTACH1_X], COMI.md_allow_value[M_BCR_ATTACH1_X]);
			nRet[1] = COMI.Check_MotPosRange(M_BCR_ATTACH1_R1, COMI.md_cmdpos_backup[M_BCR_ATTACH1_R1], COMI.md_allow_value[M_BCR_ATTACH1_R1]);
			nRet[2] = COMI.Check_MotPosRange(M_BCR_ATTACH1_Z1, COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z1], COMI.md_allow_value[M_BCR_ATTACH1_Z1]);
			nRet[3] = COMI.Check_MotPosRange(M_BCR_ATTACH1_Z2, COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z2], COMI.md_allow_value[M_BCR_ATTACH1_Z2]);

			bool nRecy = false;
			for( int i = 0; i < 4; i++)
			{
				m_nReq_Rcvy[0][i] = CTL_NO;
				if(nRet[i] == BD_ERROR)
				{
					m_nReq_Rcvy[0][i] = CTL_YES;
					nRecy = true;
				}
			}

			if(nRecy == false) mn_RcvyStep1 = 9000;
			else
			{
				m_dPos_TmpRcvyZ1_1 = COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z1];
				m_dPos_TmpRcvyZ1_2 = COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z2];
				mn_RcvyStep1 = 100;
			}
		}
		break;

	case 100:
		{
			for ( int i = 0; i < 2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[i], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[i], IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_guide_fwdback[i], IO_OFF);//2014,1125 ysj
			}
			m_dwTime_Recvy[0] = GetCurrentTime();
			mn_RcvyStep1 = 110;
		}
		break;

	case 110:
		{
			if( GetCurrentTime() - m_dwTime_Recvy[0] < 0 ) m_dwTime_Recvy[0] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTime_Recvy[0] < 500) break;

			mn_RcvyStep1 = 120;
			for ( int i = 0; i < 2; i++)
			{
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[i], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[i], IO_ON) == IO_OFF)// ||
					//g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_box_guide_back_chk[i], IO_ON) == IO_OFF )//2014,1125 ysj
				{
					mn_RcvyStep1 = 110;
					break;
				}
			}
			if( GetCurrentTime() - m_dwTime_Recvy[0] > 10000)
			{
				for ( int i = 0; i < 2; i++)
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[i], IO_ON) == IO_OFF)
					{
						CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt1_top_vacuum_fwd_chk[i], IO_OFF );
						CTL_Lib.Alarm_Error_Occurrence( 1349, dWARNING, strJamCode );
						break;
					}
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[i], IO_ON) == IO_OFF )
					{
						CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt1_btm_vacuum_fwd_chk[i], IO_OFF );
						CTL_Lib.Alarm_Error_Occurrence( 1350, dWARNING, strJamCode );
						break;
					}
// 					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_box_guide_back_chk[i], IO_ON) == IO_OFF )
// 					{
// 						CString strJamCode = GetJamCode_Bcr_Guide_FwdBwd_Time(st_io.i_distribute_rbt1_box_guide_fwd_chk[i], IO_OFF );
// 						CTL_Lib.Alarm_Error_Occurrence( 1351, dWARNING, strJamCode );
// 						break;
// 					}
				}
			}
		}
		break;

	case 120:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD )
			{
				COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z1] = m_dPos_TmpRcvyZ1_1;
				COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z2] = m_dPos_TmpRcvyZ1_2;
				mn_RcvyStep1 = 1000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1352, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1000:
		{
			if (m_nReq_Rcvy[0][1] == CTL_NO)
			{
				mn_RcvyStep1 = 2000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH1_R1, COMI.md_cmdpos_backup[M_BCR_ATTACH1_R1], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_BCR_ATTACH1_R2, COMI.md_cmdpos_backup[M_BCR_ATTACH1_R2], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD )
			{
				mn_RcvyStep1 = 2000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY ||  nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1353, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 2000:
		{
			if (m_nReq_Rcvy[0][0] == CTL_NO)
			{
				mn_RcvyStep1 = 3000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH1_X, COMI.md_cmdpos_backup[M_BCR_ATTACH1_X], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				mn_RcvyStep1 = 3000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1354, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 3000:
		{
			if(m_nReq_Rcvy[0][2] == CTL_NO && m_nReq_Rcvy[0][3] == CTL_NO)
			{
				mn_RcvyStep1 = 4000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH1_Z1, COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z1], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_BCR_ATTACH1_Z2, COMI.md_cmdpos_backup[M_BCR_ATTACH1_Z2], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_RcvyStep1 = 4000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1355, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 4000:
		{
			for (int i =0; i < 2; i++)
			{
				EFB efb;
				if( st_work.mn_bcr_m1top_vac_fwdback[i] == IO_ON) efb = FORWARD;
				else											  efb = BACKWARD;
				ReqM1TopVac(i, efb);
				if( st_work.mn_bcr_m1btm_vac_fwdback[i] == IO_ON) efb = FORWARD;
				else											  efb = BACKWARD;
				ReqM1BtmVac(i, efb);
				if( st_work.mn_bcr_m1guide_fwdback[i] == IO_ON) efb = FORWARD;
				else											  efb = BACKWARD;
				ReqM1GuideFwdBwd( i, efb);
			}
			m_dwTime_Recvy[0] = GetCurrentTime();
			mn_RcvyStep1 = 4100;
		}
		break;
		
	case 4100:
		if( GetCurrentTime() - m_dwTime_Recvy[0] < 0 ) m_dwTime_Recvy[0] = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Recvy[0] < 500) break;

		if( GetCurrentTime() - m_dwTime_Recvy[0] > 10000)
		{
			CString strJamCode;
			for ( int i = 0; i < 2; i++)
			{
				if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i], st_work.mn_bcr_m2top_vac_fwdback[i]) != !st_work.mn_bcr_m2top_vac_fwdback[i] )
				{
					strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i], st_work.mn_bcr_m2top_vac_fwdback[i] );
					CTL_Lib.Alarm_Error_Occurrence( 1356, dWARNING, strJamCode );
					break;
				}
				if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i], st_work.mn_bcr_m2btm_vac_fwdback[i]) != !st_work.mn_bcr_m2btm_vac_fwdback[i] )
				{
					strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i], st_work.mn_bcr_m2btm_vac_fwdback[i] );
					CTL_Lib.Alarm_Error_Occurrence( 1357, dWARNING, strJamCode );
					break;
				}
// 				if( g_ioMgr.get_in_bit(st_io.o_distribute_rbt2_box_guide_fwdback[i], st_work.mn_bcr_m2btm_vac_fwdback[i]) != !st_work.mn_bcr_m2guide_fwdback[i] )
// 				{
// 					strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_box_guide_back_chk[i], st_work.mn_bcr_m2btm_vac_fwdback[i] );
// 					CTL_Lib.Alarm_Error_Occurrence( 225, dWARNING, strJamCode );
// 					break;
// 				}
			}
		}

		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) return;
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) return;
		if( GetM2GuideFwdBwd( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2GuideFwdBwd( EBVS_RIGHT ) != FB_NONE ) return;

		mn_RcvyStep2 = 9000;
		break;
		


	case 9000:
		st_handler.mn_Rcvy_state[RCVY_BARCODE1] = CTL_YES;
		mn_RcvyStep1 = 0;
		break;
	}
}
void CRun_Barcode_Works::Run_Reinstate2()
{
	//2015.0206
	st_handler.mn_Rcvy_state[RCVY_BARCODE2] = CTL_YES;
	return;
	
	if(st_handler.mn_Rcvy_state[RCVY_BARCODE2] == CTL_YES) return;
	int nRet[4];

	switch(mn_RcvyStep2)
	{
	case 0:
		{
			nRet[0] = COMI.Check_MotPosRange(M_BCR_ATTACH2_X, COMI.md_cmdpos_backup[M_BCR_ATTACH2_X], COMI.md_allow_value[M_BCR_ATTACH2_X]);
			nRet[1] = COMI.Check_MotPosRange(M_BCR_ATTACH2_R1, COMI.md_cmdpos_backup[M_BCR_ATTACH2_R1], COMI.md_allow_value[M_BCR_ATTACH2_R1]);
			nRet[2] = COMI.Check_MotPosRange(M_BCR_ATTACH2_Z1, COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z1], COMI.md_allow_value[M_BCR_ATTACH2_Z1]);
			nRet[3] = COMI.Check_MotPosRange(M_BCR_ATTACH2_Z2, COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z2], COMI.md_allow_value[M_BCR_ATTACH2_Z2]);

			bool nRecy = false;
			for( int i = 0; i < 4; i++)
			{
				m_nReq_Rcvy[1][i] = CTL_NO;
				if(nRet[i] == BD_ERROR)
				{
					m_nReq_Rcvy[1][i] = CTL_YES;
					nRecy = true;
				}
			}

			if(nRecy == false) mn_RcvyStep2 = 9000;
			else
			{
				m_dPos_TmpRcvyZ2_1 = COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z1];
				m_dPos_TmpRcvyZ2_2 = COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z2];
				mn_RcvyStep2 = 100;
			}
		}
		break;

	case 100:
		{
			for ( int i = 0; i < 2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[i], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[i], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_guide_fwdback[i], IO_OFF);
			}
			m_dwTime_Recvy[1] = GetCurrentTime();
			mn_RcvyStep2 = 110;
		}
		break;

	case 110:
		{
			if( GetCurrentTime() - m_dwTime_Recvy[1] < 0 ) m_dwTime_Recvy[1] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTime_Recvy[1] < 500) break;

			mn_RcvyStep2 = 120;
			for ( int i = 0; i < 2; i++)
			{
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_box_guide_back_chk[i], IO_ON) == IO_OFF )
				{
					mn_RcvyStep2 = 110;
					break;
				}
			}
			if( GetCurrentTime() - m_dwTime_Recvy[1] > 10000)
			{
				for ( int i = 0; i < 2; i++)
				{
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i], IO_ON) == IO_OFF)
					{
						CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_top_vacuum_fwd_chk[i], IO_OFF );
						CTL_Lib.Alarm_Error_Occurrence( 1358, dWARNING, strJamCode );
						break;
					}
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i], IO_ON) == IO_OFF )
					{
						CString strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_btm_vacuum_fwd_chk[i], IO_OFF );
						CTL_Lib.Alarm_Error_Occurrence( 1359, dWARNING, strJamCode );
						break;
					}
// 					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_box_guide_back_chk[i], IO_ON) == IO_OFF )
// 					{
// 						CString strJamCode = GetJamCode_Bcr_Guide_FwdBwd_Time(st_io.i_distribute_rbt2_box_guide_fwd_chk[i], IO_OFF );
// 						CTL_Lib.Alarm_Error_Occurrence( 1360, dWARNING, strJamCode );
// 						break;
// 					}
				}
			}
		}
		break;

	case 120:
		{
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD )
			{
				COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z1] = m_dPos_TmpRcvyZ2_1;
				COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z2] = m_dPos_TmpRcvyZ2_2;
				mn_RcvyStep2 = 1000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1361, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1000:
		{
			if (m_nReq_Rcvy[1][1] == CTL_NO)
			{
				mn_RcvyStep2 = 2000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH2_R1, COMI.md_cmdpos_backup[M_BCR_ATTACH2_R1], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_BCR_ATTACH2_R2, COMI.md_cmdpos_backup[M_BCR_ATTACH2_R2], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_RcvyStep2 = 2000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1362, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 2000:
		{
			if (m_nReq_Rcvy[1][0] == CTL_NO)
			{
				mn_RcvyStep2 = 3000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH2_X, COMI.md_cmdpos_backup[M_BCR_ATTACH2_X], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				mn_RcvyStep2 = 3000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1362, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 3000:
		{
			if(m_nReq_Rcvy[1][2] == CTL_NO && m_nReq_Rcvy[1][3] == CTL_NO)
			{
				mn_RcvyStep2 = 4000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BCR_ATTACH2_Z1, COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z1], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move(M_BCR_ATTACH2_Z2, COMI.md_cmdpos_backup[M_BCR_ATTACH2_Z2], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_RcvyStep2 = 4000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1364, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 4000:
		{

			for (int i =0; i < 2; i++)
			{
				EFB efb;
				if( st_work.mn_bcr_m2top_vac_fwdback[i] == IO_ON) efb = FORWARD;
				else											  efb = BACKWARD;
				ReqM2TopVac(i, efb);
				if( st_work.mn_bcr_m2btm_vac_fwdback[i] == IO_ON) efb = FORWARD;
				else											  efb = BACKWARD;
				ReqM2BtmVac(i, efb);
				if( st_work.mn_bcr_m2guide_fwdback[i] == IO_ON) efb = FORWARD;
				else											  efb = BACKWARD;
				ReqM2GuideFwdBwd( i, efb);
			}
			m_dwTime_Recvy[1] = GetCurrentTime();
			mn_RcvyStep2 = 4100;
		}
		break;
		
	case 4100:
		if( GetCurrentTime() - m_dwTime_Recvy[1] < 0 ) m_dwTime_Recvy[1] = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_Recvy[1] < 500) break;

		if( GetCurrentTime() - m_dwTime_Recvy[1] > 10000)
		{
			CString strJamCode;
			for ( int i = 0; i < 2; i++)
			{
				if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i], st_work.mn_bcr_m2top_vac_fwdback[i]) != !st_work.mn_bcr_m2top_vac_fwdback[i] )
				{
					strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i], st_work.mn_bcr_m2top_vac_fwdback[i] );
					CTL_Lib.Alarm_Error_Occurrence( 1365, dWARNING, strJamCode );
					break;
				}
				if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i], st_work.mn_bcr_m2btm_vac_fwdback[i]) != !st_work.mn_bcr_m2btm_vac_fwdback[i] )
				{
					strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i], st_work.mn_bcr_m2btm_vac_fwdback[i] );
					CTL_Lib.Alarm_Error_Occurrence( 1366, dWARNING, strJamCode );
					break;
				}
// 				if( g_ioMgr.get_in_bit(st_io.o_distribute_rbt2_box_guide_fwdback[i], st_work.mn_bcr_m2btm_vac_fwdback[i]) != !st_work.mn_bcr_m2guide_fwdback[i] )
// 				{
// 					strJamCode = GetJamCode_Bcr_Vacuum_Fwdbwd_Time(st_io.i_distribute_rbt2_box_guide_back_chk[i], st_work.mn_bcr_m2btm_vac_fwdback[i] );
// 					CTL_Lib.Alarm_Error_Occurrence( 1367, dWARNING, strJamCode );
// 					break;
// 				}
			}
		}

		if( GetM2TopVac( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2TopVac( EBVS_RIGHT ) != FB_NONE ) return;
		if( GetM2BtmVac( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2BtmVac( EBVS_RIGHT ) != FB_NONE ) return;
		if( GetM2GuideFwdBwd( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2GuideFwdBwd( EBVS_RIGHT ) != FB_NONE ) return;

		mn_RcvyStep2 = 9000;
		break;
		

	case 9000:
		st_handler.mn_Rcvy_state[RCVY_BARCODE1] = CTL_YES;
		mn_RcvyStep2 = 0;
		break;
	
	}
}

void CRun_Barcode_Works::EDRR_HomeCheck_R( int nAxis)
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);
	if( 1 || nRet == BD_GOOD )
	{
		mn_InitStep = EBRR_HOMECHK_RBT1_R2;
		if	   ( nAxis == M_BCR_ATTACH1_R2) mn_InitStep = EBRR_HOMECHK_RBT2_R1;
		else if( nAxis == M_BCR_ATTACH2_R1) mn_InitStep = EBRR_HOMECHK_RBT2_R2;
		else if( nAxis == M_BCR_ATTACH2_R2) mn_InitStep = EBRR_HOMECHK_RBT1_X;

//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//190001 0 00 "Distribute#1 Attach R#1 Axis Motor Home Check Error."
		//210001 0 00 "Distribute#1 Attach R#2 Axis Motor Home Check Error."
		//240001 0 00 "Distribute#2 Attach R#1 Axis Motor Home Check Error."
		//260001 0 00 "Distribute#2 Attach R#2 Axis Motor Home Check Error."
		CString strJamCode = "190001";
		if	   ( nAxis == M_BCR_ATTACH1_R2) strJamCode = "210001";
		else if( nAxis == M_BCR_ATTACH2_R1) strJamCode = "240001";
		else if( nAxis == M_BCR_ATTACH2_R2) strJamCode = "260001";
		CTL_Lib.Alarm_Error_Occurrence(1368, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Works::EDRR_HomeCheck_X( int nAxis )
{
	int nRet = COMI.HomeCheck_Mot( nAxis, MOT_TIMEOUT);

	if( nAxis == M_BCR_ATTACH2_X) nRet = BD_GOOD;
	if( nRet == BD_GOOD )
	{
		if(nAxis == M_BCR_ATTACH1_X)
		{
			mn_InitStep = EBRR_HOMECHK_RBT2_X;
			Func.OnInitBtn( RBTN_BCR_ATTACH1_X );//2014.1018
		}
		else if(nAxis == M_BCR_ATTACH2_X)
		{
			//mn_InitStep = EBCRI_HOMECHK_RBT2_X;
			mn_InitStep = EBRR_FINISH;//2014.1018
			Func.OnInitBtn( RBTN_BCR_ATTACH2_X );//2014.1018
		}
	}
	else if( nRet == BD_ERROR )
	{//170001 0 00 "Distribute#1 Attach X Axis Motor Home Check Error."
		//220001 0 00 "Distribute#2 Attach X Axis Motor Home Check Error."
		CString strJamCode = "170001";
		if	   ( nAxis == M_BCR_ATTACH2_X) strJamCode = "220001";
		CTL_Lib.Alarm_Error_Occurrence(1369, CTL_dWARNING, strJamCode );
	}
}

void CRun_Barcode_Works::Run_Restore()
{
	if( st_handler.mn_init_state[INIT_BARCODE_ROBOT] != CTL_NO )	return;
	switch(mn_RestoreStep)
	{
	case EBRR_START: mn_RestoreStep = EBRR_Z_SAFETY;	break;
	case EBRR_Z_SAFETY:
		{
			int nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[ M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY], (int)st_basic.nRunSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[ M_BCR_ATTACH1_Z2].d_pos[P_Z_SAFETY], (int)st_basic.nRunSpeed );
			int nRet3 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[ M_BCR_ATTACH2_Z1].d_pos[P_Z_SAFETY], (int)st_basic.nRunSpeed );
			int nRet4 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[ M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD && nRet3 == BD_GOOD && nRet4 == BD_GOOD)
			{
				mn_RestoreStep = EBRR_VACUUM1_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1370, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case EBRR_VACUUM1_CHK:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_OFF) == IO_OFF)
			{
				mn_RestoreStep = EBRR_VACUUM2_CHK;
			}
			else
			{
				CString strError;
				if(g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON);
				}
				else if(g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON);
				}
				else if(g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_ON);
				}
				else
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_ON);
				}
				CTL_Lib.Alarm_Error_Occurrence( 1371, CTL_dWARNING, strError );
			}
		}
		break;

	case EBRR_VACUUM2_CHK:
		{
			CString strJamCode;
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF) == IO_OFF &&
				g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_OFF) == IO_OFF)
			{
				mn_RestoreStep = EBRR_VACUUM_OFF;
			}
			else
			{
				CString strError;
				if(g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON);
				}
				else if(g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON);
				}
				else if(g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF) == IO_ON)
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_ON);
				}
				else
				{
					strError.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_ON);
				}
				CTL_Lib.Alarm_Error_Occurrence( 1372, CTL_dWARNING, strError );
			}
		}
		break;

	case EBRR_VACUUM_OFF:
		{
			for ( int i = 0;i < 2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[i] , IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[i] , IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[i] , IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[i] , IO_OFF );
			}
			m_dwTimeInit = GetCurrentTime();
			mn_RestoreStep = EBRR_BLOW_ON;
		}
		break;

	case EBRR_BLOW_ON:
		{
			if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_VACUUM][IO_ON]) break;
			for ( int i = 0;i < 2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[i] , IO_ON );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[i] , IO_ON );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[i] , IO_ON );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[i] , IO_ON );
			}
			mn_RestoreStep = EBRR_BLOW_OFF;
			m_dwTimeInit = GetCurrentTime();
		}
		break;

	case EBRR_BLOW_OFF:
		{
			if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTimeInit < st_time.n_wait_time[E_WAIT_BCR_EJECT][IO_ON]) break;
			for ( int i = 0;i < 2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[i] , IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[i] , IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[i] , IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[i] , IO_OFF );
			}
			mn_RestoreStep = EBRR_CYL_OFF;
		}
		break;

	case EBRR_CYL_OFF:
		ReqM1GuideFwdBwd( EBVS_LEFT , BACKWARD );
		ReqM1GuideFwdBwd( EBVS_RIGHT, BACKWARD );
		ReqM2GuideFwdBwd( EBVS_LEFT , BACKWARD );
		ReqM2GuideFwdBwd( EBVS_RIGHT, BACKWARD );
		mn_RestoreStep = EBRR_CYL_OFF_CHK;
		break;

	case EBRR_CYL_OFF_CHK:
		if( GetM1GuideFwdBwd( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM1GuideFwdBwd( EBVS_RIGHT ) != FB_NONE ) return;
		if( GetM2GuideFwdBwd( EBVS_LEFT ) != FB_NONE ) return;
		if( GetM2GuideFwdBwd( EBVS_RIGHT ) != FB_NONE ) return;
		mn_RestoreStep = EBRR_HOMECHK_RBT1_R1;
		break;

	case EBRR_HOMECHK_RBT1_R1: EDRR_HomeCheck_R(M_BCR_ATTACH1_R1); break;
	case EBRR_HOMECHK_RBT1_R2: EDRR_HomeCheck_R(M_BCR_ATTACH1_R2); break;
	case EBRR_HOMECHK_RBT2_R1: EDRR_HomeCheck_R(M_BCR_ATTACH2_R1); break;
	case EBRR_HOMECHK_RBT2_R2: EDRR_HomeCheck_R(M_BCR_ATTACH2_R2); break;
	
	case EBRR_HOMECHK_RBT1_X: EDRR_HomeCheck_X(M_BCR_ATTACH1_X); break;
	case EBRR_HOMECHK_RBT2_X: EDRR_HomeCheck_X(M_BCR_ATTACH2_X); break;

	case EBRR_FINISH:
		for ( int i = 0; i < Func.m_pRobot[SITE_BARCODE1]->GetJobCount(); i++)
		{
			Func.m_pRobot[SITE_BARCODE1]->EraseFirstJob();
		}		
		for ( int j = 0; i < Func.m_pRobot[SITE_BARCODE2]->GetJobCount(); j++)
		{
			Func.m_pRobot[SITE_BARCODE2]->EraseFirstJob();
		}		

		st_handler.mn_init_state[INIT_BARCODE_ROBOT] = CTL_YES;
		mn_RestoreStep = EBRR_START;
		break;
	}
}

void CRun_Barcode_Works::OnRobot_FinalPos()
{
}

void CRun_Barcode_Works::Serialize(CArchive &ar)
{
}

