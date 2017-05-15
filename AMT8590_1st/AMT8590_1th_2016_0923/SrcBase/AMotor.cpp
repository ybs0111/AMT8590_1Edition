// AMotor.cpp: implementation of the AMotor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "AMotor.h"
#include "../ComizoaPublic.h"

#include "..\Cmmsdk.h"

#include "../Run_Barcode_Works.h"
#include "../Run_Barcode_Feeders.h"
#include "../Run_Unload_Works.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AMotor::AMotor()
{
	m_step = MMSTEP_NONE;
	m_bStopReq = false;
	m_MotMoveMode = MMMODE_NONE;

	m_dMotGotoPos = 0;
	m_dMotGotoSpeed = 1;

	m_nMotorId = -1;
}

AMotor::~AMotor()
{

}

EMOTRET AMotor::MotorMoving_repeat()
{
	EMOTRET ret = MOTRET_PROCEED;

	if( m_bStopReq )
	{
		if( m_step < MMSTEP_STOP )
		{
			m_step = MMSTEP_STOP;
		}
	}

	switch( m_step )
	{
	case MMSTEP_NONE:														break;

	case MMSTEP_BCR1_PICKER_UP:
		g_Run_Barcode_Works.ReqM1TopVac( 0, BACKWARD );
		g_Run_Barcode_Works.ReqM1TopVac( 1, BACKWARD );
		g_Run_Barcode_Works.ReqRollingFwdBwd( 0, 0, BACKWARD );
		g_Run_Barcode_Works.ReqRollingFwdBwd( 0, 1, BACKWARD );
		m_step = MMSTEP_BCR1_PICKER_UP_CHK;
		break;

	case MMSTEP_BCR2_PICKER_UP:
		g_Run_Barcode_Works.ReqM2TopVac( 0, BACKWARD );
		g_Run_Barcode_Works.ReqM2TopVac( 1, BACKWARD );
		g_Run_Barcode_Works.ReqRollingFwdBwd( 1, 0, BACKWARD );
		g_Run_Barcode_Works.ReqRollingFwdBwd( 1, 1, BACKWARD );
		m_step = MMSTEP_BCR2_PICKER_UP_CHK;
		break;

	case MMSTEP_REJ_PICKER_UP:
		g_Run_Barcode_Feeders.ReqRejVacDown( BACKWARD );
		m_step = MMSTEP_REJ_PICKER_UP_CHK;
		break;

	case MMSTEP_UNLOAD_PICKER_UP:
		g_Run_Unload_Works.ReqVacDown( 0, BACKWARD);
		g_Run_Unload_Works.ReqVacDown( 1, BACKWARD);
		m_step = MMSTEP_UNLOAD_PICKER_UP_CHK;
		break;

	case MMSTEP_BCR1_PICKER_UP_CHK:
		if( g_Run_Barcode_Works.GetM1TopVac( 0 ) != FB_NONE )	break;
		if( g_Run_Barcode_Works.GetM1BtmVac( 1 ) != FB_NONE )	break;
		if( g_Run_Barcode_Works.GetRollingFwdBwd( 0, 0 ) != FB_NONE )	break;
		if( g_Run_Barcode_Works.GetRollingFwdBwd( 0, 1 ) != FB_NONE )	break;
		m_step = MMSTEP_BCR1_SAFETY;
		break;
	case MMSTEP_BCR2_PICKER_UP_CHK:
		if( g_Run_Barcode_Works.GetM2TopVac( 0 ) != FB_NONE )	break;
		if( g_Run_Barcode_Works.GetM2BtmVac( 1 ) != FB_NONE )	break;
		if( g_Run_Barcode_Works.GetRollingFwdBwd( 1, 0 ) != FB_NONE )	break;
		if( g_Run_Barcode_Works.GetRollingFwdBwd( 1, 1 ) != FB_NONE )	break;
		m_step = MMSTEP_BCR2_SAFETY;
		break;

	case MMSTEP_REJ_PICKER_UP_CHK:
		if( g_Run_Barcode_Feeders.GetRejVacDown() != FB_NONE ) break;
		m_dwMoveWaitTime = GetCurrentTime();
		m_step = MMSTEP_MOTOR_SAFETY;
		break;

	case MMSTEP_UNLOAD_PICKER_UP_CHK:
		if( g_Run_Unload_Works.GetVacDown( 0 ) != FB_NONE )	break;
		if( g_Run_Unload_Works.GetVacDown( 1 ) != FB_NONE )	break;
		m_step = MMSTEP_MOTOR_SAFETY;
		break;

	case MMSTEP_BCR1_SAFETY:	MmStep_Motor_Safety();						break;
	case MMSTEP_BCR2_SAFETY:	MmStep_Motor_Safety();						break;
		
	case MMSTEP_MOTOR_SAFETY:	MmStep_Motor_Safety();						break;

	case MMSTEP_MOVE_START:		MmStep_Move_Start();						break;
	case MMSTEP_MOVE_CHECK:		MmStep_Move_Check();						break;
	case MMSTEP_MOVE_FINISH:	MmStep_Move_Finish();	ret = MOTRET_GOOD;	break;



	case MMSTEP_LINEAR_PICKER_UP_CHK:
		m_step = MMSTEP_LINEAR_SAFETY;
		break;

	case MMSTEP_LINEAR_SAFETY:	MmStep_Linear_Safety();						break;

	case MMSTEP_LINEAR_SAFETY_CHK:	MmStep_Linear_Safety_Chk();				break;

	case MMSTEP_MOTOR_SAFETY_CHK:	MmStep_Motor_Safety_Chk();				break;


	case MMSTEP_LINEAR_MDL_CALC:	MmStep_Linear_Mdl_Calc();				break;

		
	case MMSTEP_LINEAR_LDXSAFETY:	MmStep_Linear_LDXSafety();				break;
	case MMSTEP_LINEAR_ULDXSAFETY:	MmStep_Linear_ULDXSafety();				break;
	case MMSTEP_LINEAR_ZSAFETY:		MmStep_Linear_ZSafety();				break;
	case MMSTEP_LINEAR_MDL_START:	MmStep_Linear_Mdl_Start();				break;
	case MMSTEP_LINEAR_MDL_CHK:		MmStep_Linear_Mdl_Chk();				break;

	case MMSTEP_LINEAR_START:	MmStep_Linear_Start();						break;

	case MMSTEP_LINEAR_CHK:		MmStep_Linear_Chk();						break;
	case MMSTEP_LINEAR_FINISH:	MmStep_Linear_Finish();	ret = MOTRET_GOOD;	break;
	
	case MMSTEP_HOME_HOMECHECK:	ret = MmStep_Home_HomeCheck();				break;
	
	case MMSTEP_STOP:			MmStep_Stop();								break;
	case MMSTEP_STOP_FINISH:	MmStep_Stop_Finish();	ret = MOTRET_GOOD;	break;
	
	case MMSTEP_MOVE_FAIL:		MmStep_Move_Fail();		ret = MOTRET_ERROR;	break;
	case MMSTEP_STOP_FAIL:		MmStep_Stop_Fail();		ret = MOTRET_ERROR;	break;
	}

	return ret;
}

void AMotor::MmStep_Move_Start()
{
	int nRet = COMI.Start_SingleMove( m_nMotorId, m_dMotGotoPos, (int)m_dMotGotoSpeed );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_MOVE_CHECK;
	}
	else if( nRet == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_Move_Check()
{
	int nRet = COMI.Check_SingleMove( m_nMotorId, m_dMotGotoPos );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_MOVE_FINISH;
		st_handler.mn_menu_lock = FALSE;//2014.1017 동작완료 후 메인화면 전환이 안되어서 추가
	}
	else if( nRet == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_Move_Finish()
{
	// "[" + m_str_cmd_motor + "] Motor completed transfer."
	_repeat_Finish();
	_finish_manual();
}

EMOTRET AMotor::MmStep_Home_HomeCheck()
{
	EMOTRET ret = MOTRET_PROCEED;
	int nRet = COMI.HomeCheck_Mot( m_nMotorId, MOT_TIMEOUT);

	if( nRet == MOTRET_GOOD )
	{
		_repeat_Finish();
		_finish_manual();
		ret = MOTRET_GOOD;
	}
	else if( nRet == MOTRET_ERROR )
	{
		_repeat_Finish();
		_finish_manual();
		ret = MOTRET_ERROR;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_NONE;
		_finish_manual();
		ret = MOTRET_ERROR;
	}

	return ret;
}
	
void AMotor::MmStep_Stop()
{
	int nRet = COMI.Set_MotStop( MOTSTOP_SLOWDOWN, m_nMotorId );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_STOP_FINISH;
	}
	else
	{
		// "[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened."
		m_step = MMSTEP_STOP_FAIL;
	}
}

void AMotor::MmStep_Stop_Finish()
{
	// "[" + m_str_cmd_motor + "] Motor shut down during transfer."
	_repeat_Finish();
	_finish_manual();
}

void AMotor::MmStep_Move_Fail()
{
	// "[" + m_str_cmd_motor + "] Transfer command of motor failed."
	_repeat_Finish();
	_finish_manual();
}
	
void AMotor::MmStep_Stop_Fail()
{
	// "[" + m_str_cmd_motor + "] Transfer command of motor failed."
	_repeat_Finish();
	_finish_manual();
}

void AMotor::_repeat_Finish()
{
	if( m_bStopReq )
	{
		m_bStopReq = false;
	}

	m_step = MMSTEP_NONE;
}

void AMotor::_finish_manual()
{
	if( m_MotMoveMode == MMMODE_MANUAL ||
		m_MotMoveMode == MMMODE_HOME )
	{
		if( st_handler.cwnd_motorAxis )
		{
			st_handler.cwnd_motorAxis->PostMessage( WM_MOTOR_COMPLETION, m_MotMoveMode, 0 );
		}
		if( st_handler.cwnd_motor_with_io )
			st_handler.cwnd_motor_with_io->PostMessage( WM_MOTOR_COMPLETION );

		m_MotMoveMode = MMMODE_NONE;
	}
	else if( m_MotMoveMode == MMMODE_WITH_IO_MOVE )
	{
		if( st_handler.cwnd_motor_with_io )
			st_handler.cwnd_motor_with_io->PostMessage( WM_MOTOR_COMPLETION );
	}
}

void AMotor::MotorStepClear()
{
	m_bStopReq = false;
	m_step = MMSTEP_NONE;
}

void AMotor::MotorMoveStart( double dPos, double dSpeed, EMOTMOVE_MODE motMoveMode )
{
	m_dMotGotoPos = dPos;
	m_dMotGotoSpeed = dSpeed;
	m_MotMoveMode = motMoveMode;

	m_step = MMSTEP_MOVE_START;

}

void AMotor::MotorChkMoveStart( double dPos, double dSpeed, EMOTMOVE_MODE motMoveMode )
{
	m_dMotGotoPos = dPos;
	m_dMotGotoSpeed = dSpeed;
	m_MotMoveMode = motMoveMode;

	if( m_nMotorId == M_BCR_ATTACH1_Z1 || m_nMotorId == M_BCR_ATTACH1_Z2)
	{
		m_step = MMSTEP_BCR1_PICKER_UP;
	}
	else if( m_nMotorId == M_BCR_ATTACH2_Z1 || m_nMotorId == M_BCR_ATTACH2_Z2)
	{
		m_step = MMSTEP_BCR2_PICKER_UP;
	}
	else if( m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2)
	{
		m_step = MMSTEP_MOTOR_SAFETY;
	}
	else if( m_nMotorId == M_L_REJECT_X )
	{
		m_step = MMSTEP_REJ_PICKER_UP;
	}
	else if( m_nMotorId == M_TOP_VISION_X || m_nMotorId == M_BTM_VISION_X)
	{
		m_step = MMSTEP_MOVE_START;
	}
	else if( m_nMotorId == M_ULD_Y ||  m_nMotorId == M_ULD_Z)
	{
		m_step = MMSTEP_UNLOAD_PICKER_UP;
	}
	////2014.1017
	else if( m_nMotorId == M_BCR_ATTACH1_X || m_nMotorId == M_BCR_ATTACH2_X)
	{
		m_step = MMSTEP_MOVE_START;
	}
	////
}

void AMotor::MotorHomeStart()
{
	m_MotMoveMode = MMMODE_HOME;
	m_step = MMSTEP_HOME_HOMECHECK;
}

EMOTRET AMotor::JogMoveStart( long lDir )
{
	COMI.Set_MotSpeed(MOT_SPD_JOG, m_nMotorId, cmSMODE_KEEP, COMI.md_spd_jog[m_nMotorId], COMI.md_spd_vel[m_nMotorId][1], COMI.md_spd_vel[m_nMotorId][2]);		// 축의 속도를 변경한다.
	
	long nRet = cmmSxVMoveStart(m_nMotorId, lDir);
	if ( nRet != cmERR_NONE)
	{ 
		return MOTRET_ERROR;
	}
	
	return MOTRET_GOOD;
}

void AMotor::MmStep_Linear_Safety()
{
	int nRet1 = COMI.Start_SingleMove( M_D_ATTACH_Z1, st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)m_dMotGotoSpeed );
	int nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Z2, st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)m_dMotGotoSpeed );
	if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD)
	{
		m_step = MMSTEP_LINEAR_SAFETY_CHK;
	}
	else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_Linear_Safety_Chk()
{
	int nRet1 = COMI.Check_SingleMove( M_D_ATTACH_Z1, st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY] );
	int nRet2 = COMI.Check_SingleMove( M_D_ATTACH_Z2, st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY] );
	if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD )
	{
		m_step = MMSTEP_LINEAR_START;
	}
	else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_Linear_Mdl_Calc()
{
}

void AMotor::MmStep_Motor_Safety()
{
	int nRet1, nRet2;
 	if( m_nMotorId == M_BCR_ATTACH1_R1 || m_nMotorId == M_BCR_ATTACH1_R2 || m_nMotorId == M_BCR_ATTACH1_X || m_nMotorId == M_BCR_ATTACH1_Z1 || m_nMotorId == M_BCR_ATTACH1_Z2 ||
		m_nMotorId == M_BCR_ATTACH2_R1 || m_nMotorId == M_BCR_ATTACH2_R2 || m_nMotorId == M_BCR_ATTACH2_X || m_nMotorId == M_BCR_ATTACH2_Z1 || m_nMotorId == M_BCR_ATTACH2_Z2)
 	{
		if( m_nMotorId == M_BCR_ATTACH1_Z1 || m_nMotorId == M_BCR_ATTACH1_Z2 )
		{
 			nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			nRet2 = COMI.Start_SingleMove( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD )
 			{
 				m_step = MMSTEP_MOTOR_SAFETY_CHK;
 			}
 			else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY)
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
 		if( m_nMotorId == M_BCR_ATTACH2_Z1 || m_nMotorId == M_BCR_ATTACH2_Z2 )
		{
 			nRet1 = COMI.Start_SingleMove( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			nRet2 = COMI.Start_SingleMove( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD )
 			{
 				m_step = MMSTEP_MOTOR_SAFETY_CHK;
 			}
 			else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY)
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
	}
 	else if( m_nMotorId == M_D_ATTACH_X || m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2 ||	m_nMotorId == M_D_ATTACH_Y )
 	{
		if( m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2 )
		{
 			nRet1 = COMI.Start_SingleMove( M_D_ATTACH_Z1, st_motor[M_D_ATTACH_Z1].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			nRet2 = COMI.Start_SingleMove( M_D_ATTACH_Z2, st_motor[M_D_ATTACH_Z2].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD )
 			{
 				m_step = MMSTEP_MOTOR_SAFETY_CHK;
 			}
 			else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY)
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
 	}
 	else if( m_nMotorId == M_ULD_Y || m_nMotorId == M_ULD_Z )
	{
		if( m_nMotorId == M_ULD_Z )//2014.1017 = -> ==
		{
 			nRet1 = COMI.Start_SingleMove( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
 			if( nRet1 == MOTRET_GOOD )
 			{
 				m_step = MMSTEP_MOTOR_SAFETY_CHK;
 			}
 			else if( nRet1 == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet1 == MOTRET_SAFETY )
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
		////2014.1017
		else if( m_nMotorId == M_ULD_Y )
		{
			nRet1 = COMI.Start_SingleMove( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_Z_SAFETY], (int)m_dMotGotoSpeed );
			if( nRet1 == MOTRET_GOOD )
			{
				m_step = MMSTEP_MOTOR_SAFETY_CHK;
			}
			else if( nRet1 == MOTRET_ERROR )
			{
				// "[" + m_str_cmd_motor + "] Motor failed to move."
				m_step = MMSTEP_MOVE_FAIL;
			}
			else if( nRet1 == MOTRET_SAFETY )
			{
				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
				m_step = MMSTEP_MOVE_FAIL;
			}
		}
		////
	}
	else if( m_nMotorId == M_L_REJECT_X )
	{
		if( GetCurrentTime() - m_dwMoveWaitTime < 0 ) m_dwMoveWaitTime = GetCurrentTime();
		if( GetCurrentTime() - m_dwMoveWaitTime < 1000) return;
		m_step = MMSTEP_MOVE_START;		
	}
	// 	else if( m_nMotorId == M_ULD_Y  || m_nMotorId == M_ULD_Z )//2014.1017
	// 	{
	// 		if( m_nMotorId == M_ULD_Y || m_nMotorId == M_ULD_Z )
	// 		{
	//  			nRet1 = COMI.Start_SingleMove( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_ULD_Z_SAFETY], (int)m_dMotGotoSpeed );
	//  			if( nRet1 == MOTRET_GOOD )
	//  			{
	//  				m_step = MMSTEP_MOTOR_SAFETY_CHK;
	//  			}
	//  			else if( nRet1 == MOTRET_ERROR )
	//  			{
	//  				// "[" + m_str_cmd_motor + "] Motor failed to move."
	//  				m_step = MMSTEP_MOVE_FAIL;
	//  			}
	//  			else if( nRet1 == MOTRET_SAFETY )
	//  			{
	//  				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
	//  				m_step = MMSTEP_MOVE_FAIL;
	//  			}
	// 		}
// 	}
}

void AMotor::MmStep_Motor_Safety_Chk()
{
 	int nRet1, nRet2;
	if( m_nMotorId == M_BCR_ATTACH1_R1 || m_nMotorId == M_BCR_ATTACH1_R2 || m_nMotorId == M_BCR_ATTACH1_X || m_nMotorId == M_BCR_ATTACH1_Z1 || m_nMotorId == M_BCR_ATTACH1_Z2 ||
		m_nMotorId == M_BCR_ATTACH2_R1 || m_nMotorId == M_BCR_ATTACH2_R2 || m_nMotorId == M_BCR_ATTACH2_X || m_nMotorId == M_BCR_ATTACH2_Z1 || m_nMotorId == M_BCR_ATTACH2_Z2)
 	{
		if( m_nMotorId == M_BCR_ATTACH1_Z1 || m_nMotorId == M_BCR_ATTACH1_Z2 )
		{
 			nRet1 = COMI.Check_SingleMove( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY] );
  			nRet2 = COMI.Check_SingleMove( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_SAFETY] );
			if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD)
 			{
 				m_step = MMSTEP_MOVE_START;
				st_handler.mn_menu_lock = FALSE;//2014.1017 동작완료 후 메인화면 전환이 안되어서 추가
 			}
 			else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY )
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
 	}
 	else if( m_nMotorId == M_D_ATTACH_X || m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2 || m_nMotorId == M_D_ATTACH_Y )
 	{
		if( m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2 )
		{
 			nRet1 = COMI.Check_SingleMove( M_D_ATTACH_Z1, st_motor[M_D_ATTACH_Z1].d_pos[P_Z_SAFETY] );
 			nRet2 = COMI.Check_SingleMove( M_D_ATTACH_Z2, st_motor[M_D_ATTACH_Z2].d_pos[P_Z_SAFETY] );
			if( nRet1 == MOTRET_GOOD && nRet2 == MOTRET_GOOD)
 			{
 				m_step = MMSTEP_MOVE_START;
				st_handler.mn_menu_lock = FALSE;//2014.1017 동작완료 후 메인화면 전환이 안되어서 추가
			}
 			else if( nRet1 == MOTRET_ERROR || nRet2 == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet1 == MOTRET_SAFETY || nRet2 == MOTRET_SAFETY )
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
 	}
 	else if( m_nMotorId == M_ULD_Y || m_nMotorId == M_ULD_Z )
 	{
		if( m_nMotorId == M_ULD_Z )//2014.1017 = -> ==
		{
 			int nRet = COMI.Check_SingleMove( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_Z_SAFETY] );
 			if( nRet == MOTRET_GOOD )
 			{
 				m_step = MMSTEP_MOVE_START;
				st_handler.mn_menu_lock = FALSE;//2014.1017 동작완료 후 메인화면 전환이 안되어서 추가
 			}
 			else if( nRet == MOTRET_ERROR )
 			{
 				// "[" + m_str_cmd_motor + "] Motor failed to move."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
 			else if( nRet == MOTRET_SAFETY )
 			{
 				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 				m_step = MMSTEP_MOVE_FAIL;
 			}
		}
		////2014.1017
		else if( m_nMotorId == M_ULD_Y )//2014.1017 = -> ==
		{
			int nRet = COMI.Check_SingleMove( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_Z_SAFETY] );
			if( nRet == MOTRET_GOOD )
			{
				m_step = MMSTEP_MOVE_START;
				st_handler.mn_menu_lock = FALSE;//2014.1017
			}
			else if( nRet == MOTRET_ERROR )
			{
				// "[" + m_str_cmd_motor + "] Motor failed to move."
				m_step = MMSTEP_MOVE_FAIL;
			}
			else if( nRet == MOTRET_SAFETY )
			{
				// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
				m_step = MMSTEP_MOVE_FAIL;
			}
		}
		////
	}
}

void AMotor::MmStep_Linear_LDXSafety()
{
// 	if( m_nMotorId == MOTOR_SORTER_X || m_nMotorId == MOTOR_SORTER_Y || m_nMotorId == MOTOR_SORTER_Z)
// 	{
// 		m_step = MMSTEP_LINEAR_ZSAFETY;
// 	}
// 	else
// 	{
// 		int nRet = CTL_Lib.Single_Move( MOTOR_PICKER_X, st_motor[MOTOR_PICKER_X].d_pos[P_XY_LD_SAFETY], st_basic.nRunSpeed);
// 		if( nRet == BD_GOOD )
// 		{
// 			m_step = MMSTEP_LINEAR_ZSAFETY;
// 		}
// 		else if( nRet == BD_ERROR || nRet == BD_SAFETY )
// 		{
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 	}
}

void AMotor::MmStep_Linear_ULDXSafety()
{
// 	if( m_nMotorId == MOTOR_SORTER_X || m_nMotorId == MOTOR_SORTER_Y || m_nMotorId == MOTOR_SORTER_Z)
// 	{
// 		m_step = MMSTEP_LINEAR_ZSAFETY;
// 	}
// 	else
// 	{
// 		int nRet = CTL_Lib.Single_Move( MOTOR_PICKER_X, st_motor[MOTOR_PICKER_X].d_pos[P_XY_ULD_SAFETY], st_basic.nRunSpeed);
// 		if( nRet == BD_GOOD )
// 		{
// 			m_step = MMSTEP_LINEAR_ZSAFETY;
// 		}
// 		else if( nRet == BD_ERROR || nRet == BD_SAFETY )
// 		{
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 	}
}

void AMotor::MmStep_Linear_ZSafety()
{
// 	if( m_nMotorId == MOTOR_SORTER_X || m_nMotorId == MOTOR_SORTER_Y || m_nMotorId == MOTOR_SORTER_Z)
// 	{
// 		m_step = MMSTEP_LINEAR_MDL_START;
// 	}
// 	else
// 	{
// 		int nRet = COMI.Start_SingleMove( MOTOR_PICKER_Z, m_dLinearPos[1], (int)m_dMotGotoSpeed );
// 		if( nRet == MOTRET_GOOD )
// 		{
// 			m_step = MMSTEP_LINEAR_MDL_START;
// 		}
// 		else if( nRet == MOTRET_ERROR )
// 		{
// 			// "[" + m_str_cmd_motor + "] Motor failed to move."
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 		else if( nRet == MOTRET_SAFETY )
// 		{
// 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 	}
}


void AMotor::MmStep_Linear_Mdl_Start()
{
//	double dMdl[2];
// 	if( m_nMotorId == MOTOR_SORTER_X || m_nMotorId == MOTOR_SORTER_Y || m_nMotorId == MOTOR_SORTER_Z)
// 	{
// 		m_step = MMSTEP_LINEAR_MDL_CHK;
// 	}
// 	else
// 	{
// 		if(m_dLinearPos[0] < st_motor[ MOTOR_PICKER_X].d_pos[P_XY_LD_SAFETY] )
// 		{
// 			dMdl[0] = st_motor[ MOTOR_PICKER_X].d_pos[P_XY_LD_SAFETY];
// 		}
// 		else if(m_dLinearPos[0] > st_motor[ MOTOR_PICKER_X].d_pos[P_XY_ULD_SAFETY] )
// 		{
// 			dMdl[0] = st_motor[ MOTOR_PICKER_Z].d_pos[P_XY_ULD_SAFETY];
// 		}
// 		dMdl[1] = m_dLinearPos[1];
// 
// 		COMI.ml_axiscnt[M_ROBOT_XY_INDEX] = 2;
// 		COMI.mp_axisnum[M_ROBOT_XY_INDEX][0] = MOTOR_PICKER_X;
// 		COMI.mp_axisnum[M_ROBOT_XY_INDEX][1] = MOTOR_PICKER_Z;
// 
// 		double dSppedRatio[3];
// 		dSppedRatio[0] = m_dMotGotoSpeed;				// 1/10 속도로 복귀 한다.
// 		dSppedRatio[1] = m_dMotGotoSpeed;
// 		dSppedRatio[2] = m_dMotGotoSpeed;
// 		int nRet = COMI.Start_LinearMove(M_ROBOT_XY_INDEX, dMdl, dSppedRatio );
// 		if( nRet == MOTRET_GOOD )
// 		{
// 			m_step = MMSTEP_LINEAR_MDL_CHK;
// 		}
// 		else if( nRet == MOTRET_ERROR )
// 		{
// 			// "[" + m_str_cmd_motor + "] Motor failed to move."
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 		else if( nRet == MOTRET_SAFETY )
// 		{
// 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 	}
	
}

void AMotor::MmStep_Linear_Mdl_Chk()
{
//	double dMdl[2];
// 	if( m_nMotorId == MOTOR_SORTER_X || m_nMotorId == MOTOR_SORTER_Y || m_nMotorId == MOTOR_SORTER_Z)
// 	{
// 		m_step = MMSTEP_LINEAR_START;
// 	}
// 	else
// 	{
// 		if(m_dLinearPos[0] < st_motor[ MOTOR_PICKER_X].d_pos[P_XY_LD_SAFETY] )
// 		{
// 			dMdl[0] = st_motor[ MOTOR_PICKER_X].d_pos[P_XY_LD_SAFETY];
// 		}
// 		else if(m_dLinearPos[0] > st_motor[ MOTOR_PICKER_X].d_pos[P_XY_ULD_SAFETY] )
// 		{
// 			dMdl[0] = st_motor[ MOTOR_PICKER_Z].d_pos[P_XY_ULD_SAFETY];
// 		}
// 		dMdl[1] = m_dLinearPos[1];
// 
// 		int nRet = COMI.Check_LinearMove( M_ROBOT_XY_INDEX, dMdl );
// 		if( nRet == MOTRET_GOOD )
// 		{
// 			m_step = MMSTEP_LINEAR_START;
// 		}
// 		else if( nRet == MOTRET_ERROR )
// 		{
// 			// "[" + m_str_cmd_motor + "] Motor failed to move."
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 		else if( nRet == MOTRET_SAFETY )
// 		{
// 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
// 			m_step = MMSTEP_MOVE_FAIL;
// 		}
// 	}
}

void AMotor::MmStep_Linear_Start()
{
 	if( m_nMotorId == M_D_ATTACH_X || m_nMotorId == M_D_ATTACH_Y || m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2 )
 	{
 		COMI.ml_axiscnt[M_ROBOT_DENSITY_INDEX] = 2;
 		COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][0] = M_D_ATTACH_X;
 		COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][1] = M_D_ATTACH_Y;
 
 		double dSppedRatio[3];
 		dSppedRatio[0] = m_dMotGotoSpeed;				// 1/10 속도로 복귀 한다.
 		dSppedRatio[1] = m_dMotGotoSpeed;
 		dSppedRatio[2] = m_dMotGotoSpeed;
 		int nRet = COMI.Start_LinearMove(M_ROBOT_DENSITY_INDEX, m_dLinearPos, dSppedRatio );
 		if( nRet == MOTRET_GOOD )
 		{
 			m_step = MMSTEP_LINEAR_CHK;
 		}
 		else if( nRet == MOTRET_ERROR )
 		{
 			// "[" + m_str_cmd_motor + "] Motor failed to move."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 		else if( nRet == MOTRET_SAFETY )
 		{
 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 	}
 	else
 	{
 		COMI.ml_axiscnt[M_ROBOT_DENSITY_INDEX] = 2;
 		COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][0] = M_D_ATTACH_X;
 		COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][1] = M_D_ATTACH_Y;
 
 		double dSppedRatio[3];
 		dSppedRatio[0] = m_dMotGotoSpeed;				// 1/10 속도로 복귀 한다.
 		dSppedRatio[1] = m_dMotGotoSpeed;
 		dSppedRatio[2] = m_dMotGotoSpeed;
 		int nRet = COMI.Start_LinearMove(M_ROBOT_DENSITY_INDEX, m_dLinearPos, dSppedRatio );
 		if( nRet == MOTRET_GOOD )
 		{
 			m_step = MMSTEP_LINEAR_CHK;
 		}
 		else if( nRet == MOTRET_ERROR )
 		{
 			// "[" + m_str_cmd_motor + "] Motor failed to move."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 		else if( nRet == MOTRET_SAFETY )
 		{
 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 	}
}

void AMotor::MmStep_Linear_Chk()
{
 	if( m_nMotorId == M_D_ATTACH_X || m_nMotorId == M_D_ATTACH_Y || m_nMotorId == M_D_ATTACH_Z1 || m_nMotorId == M_D_ATTACH_Z2 )
 	{
 		int nRet = COMI.Check_LinearMove( M_ROBOT_DENSITY_INDEX, m_dLinearPos );
 		if( nRet == MOTRET_GOOD )
 		{
 			m_step = MMSTEP_LINEAR_FINISH;
 		}
 		else if( nRet == MOTRET_ERROR )
 		{
 			// "[" + m_str_cmd_motor + "] Motor failed to move."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 		else if( nRet == MOTRET_SAFETY )
 		{
 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 	}
 	else
 	{
 		int nRet = COMI.Check_LinearMove( M_ROBOT_DENSITY_INDEX, m_dLinearPos );
 		if( nRet == MOTRET_GOOD )
 		{
 			m_step = MMSTEP_LINEAR_FINISH;
 		}
 		else if( nRet == MOTRET_ERROR )
 		{
 			// "[" + m_str_cmd_motor + "] Motor failed to move."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 		else if( nRet == MOTRET_SAFETY )
 		{
 			// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
 			m_step = MMSTEP_MOVE_FAIL;
 		}
 	}
}

void AMotor::MmStep_Linear_Finish()
{
	_repeat_Finish();
	_finish_manual();
}

void AMotor::MotorLinearStart( double dPosX, double dPosY, double dSpeed, EMOTMOVE_MODE motMoveMode /*= MMMODE_NONE */ )
{
	m_dLinearPos[0] = dPosX;
	m_dLinearPos[1] = dPosY;

	m_dMotGotoSpeed = dSpeed;
	m_MotMoveMode = motMoveMode;

	m_step = MMSTEP_LINEAR_PICKER_UP_CHK;
}

void AMotor::MotorCokYStart( double dPos, double dSpeed, EMOTMOVE_MODE motMoveMode /*= MMMODE_NONE */ )
{
	m_dMotGotoPos = dPos;
	m_dMotGotoSpeed = dSpeed;
	m_MotMoveMode = motMoveMode;

	m_step = MMSTEP_COKY_Z_SAFETY;
}

bool AMotor::CheckTeachSafety( int nAxis, int nPos, double dVal )
{
// 	if( nAxis == MOTOR_PICKER_X )
// 	{
// 		if( nPos == P_XY_SITE1_START || 
// 			nPos == P_XY_SITE1_END || 
// 			nPos == P_XY_SITE1_MIDDLE )
// //			||	nPos == P_XY_SITE_1_3 || 
// //			nPos == P_XY_SITE_1_4 
// 		{
// 			if( nPos != P_XY_SITE1_START && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE1_START] - dVal ) > 10.0f )		return false;
// 			if( nPos != P_XY_SITE1_END && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE1_END] - dVal ) > 10.0f )			return false;
// 			if( nPos != P_XY_SITE1_MIDDLE && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE1_MIDDLE] - dVal ) > 10.0f )			return false;
// //			if( nPos != P_XY_SITE_1_3 && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE_1_3] - dVal ) > 10.0f )			return false;
// //			if( nPos != P_XY_SITE_1_4 && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE_1_4] - dVal ) > 10.0f )			return false;
// 		}
// 
// 		if( nPos == P_XY_SITE2_START || 
// 			nPos == P_XY_SITE2_END || 
// //			nPos == P_XY_SITE_2_2 ||
// //			nPos == P_XY_SITE_2_3 || 
// 			nPos == P_XY_SITE1_MIDDLE )
// 		{
// 			if( nPos != P_XY_SITE2_START && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE2_START] - dVal ) > 10.0f )		return false;
// 			if( nPos != P_XY_SITE2_END && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE2_END] - dVal ) > 10.0f )			return false;
// 			if( nPos != P_XY_SITE2_MIDDLE && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE2_MIDDLE] - dVal ) > 10.0f )			return false;
// //			if( nPos != P_XY_SITE_2_3 && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE_2_3] - dVal ) > 10.0f )			return false;
// //			if( nPos != P_XY_SITE_2_4 && abs( st_motor[MOTOR_PICKER_X].d_pos[P_XY_SITE_2_4] - dVal ) > 10.0f )			return false;
// 		}
// 
// 		if( nPos == P_XY_REJ_START ||
// 			nPos == P_XY_REJ_END || 
// 			nPos == P_XY_REJ_CALC )
// 		{
// 			//2013,0225
// //			if (st_handler.mn_mirror_type == CTL_YES)
// //			{
// //				if( st_motor[MOTOR_PICKER_X].d_pos[P_XY_TRAY2_START] < dVal )	
// //					return false;
// //			}
// //			else
// //			{
// //				if( st_motor[MOTOR_PICKER_X].d_pos[P_XY_TRAY2_END] > dVal )	
// //					return false;
// //			}
// 		}
// 	}
// 	else if( nAxis == MOTOR_PICKER_Z )
// 	{
// //		if( nPos == P_XY_TRAY1_START ||
// //			nPos == P_XY_TRAY1_END ||
// //			nPos == P_XY_TRAY1_CALC ||
// //			nPos == P_XY_TRAY2_START ||
// //			nPos == P_XY_TRAY2_END ||
// //			nPos == P_XY_TRAY2_CALC )
// //		{
// //			if( nPos != P_XY_TRAY1_START && st_motor[MOTOR_PICKER_Z].d_pos[P_XY_TRAY1_START] + 50.0f < dVal )					return false;
// //			if( nPos != P_XY_TRAY2_START && st_motor[MOTOR_PICKER_Z].d_pos[P_XY_TRAY2_START] + 50.0f < dVal )					return false;
// //			if( st_motor[MOTOR_PICKER_Z].d_pos[P_XY_SITE1_END] - 50.0f < dVal )													return false;
// //			if( st_motor[MOTOR_PICKER_Z].d_pos[P_XY_SITE2_END] - 50.0f < dVal )													return false;
// //		}
// //ybs
// //		if( nPos == P_XY_TRAY1_START ||
// //			nPos == P_XY_TRAY1_END ||
// //			nPos == P_XY_TRAY1_CALC ||
// //			nPos == P_XY_TRAY2_START ||
// //			nPos == P_XY_TRAY2_END ||
// //			nPos == P_XY_TRAY2_CALC ||
// //			nPos == P_XY_TRAY3_START ||
// //			nPos == P_XY_TRAY3_END ||
// //			nPos == P_XY_TRAY3_CALC )
// //		{
// //			if( nPos != P_XY_TRAY1_START && st_motor[MOTOR_PICKER_Z].d_pos[P_XY_TRAY1_START] + 50.0f < dVal )					return false;
// //			if( nPos != P_XY_TRAY2_START && st_motor[MOTOR_PICKER_Z].d_pos[P_XY_TRAY2_START] + 50.0f < dVal )					return false;
// //			if( nPos != P_XY_TRAY3_START && st_motor[MOTOR_PICKER_Z].d_pos[P_XY_TRAY3_START] + 50.0f < dVal )					return false;
// //			if( st_motor[MOTOR_PICKER_Z].d_pos[P_XY_SITE1_END] - 50.0f < dVal )													return false;
// //			if( st_motor[MOTOR_PICKER_Z].d_pos[P_XY_SITE2_END] - 50.0f < dVal )													return false;
// //		}
// 
// 	}

	return true;
}



