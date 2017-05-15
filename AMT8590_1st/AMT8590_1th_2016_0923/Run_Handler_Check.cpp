// Run_Handler_Check.cpp: implementation of the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Handler_Check.h"

#include "Variable.h"				// 전역 변수 정의 Class
#include "Public_Function.h"		// Motor 및 Cylinder 동작 Class
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"

#include "Srcbase\ALocalization.h"

#include "ComizoaPublic.h"
#include "Cmmsdk.h"

#include "Run_Motors.h"
#include "SrcPart/APartHandler.h"

#include "Run_Density_Feeders.h"
#include "Run_Barcode_Feeders.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_Handler_Check Run_Handler_Check;
CRun_Handler_Check Run_Handler_Check_Manual;

CRun_Handler_Check::CRun_Handler_Check()
{
	StartStep = 0;
	StopStep = 0;
	LoaderStep = 0;
	AlaemClearStep = 0;
	BuzzerStep = 0;
	Recycle_zone_Step = 0;//2014,1205
	MachineResetStep = 0;
	
	DoorStep = 0;
	AreaStep = 0;
	
	n_emo_chk = FALSE;
	n_air_chk = FALSE;

	n_light_curtain_chk = CTL_NO;

	n_front_key = -1;
	n_rear_key = -1;

	////2015.0114
	m_nLockStep = 0;
	m_nDoorOpenTry = FALSE;
	////

}

CRun_Handler_Check::~CRun_Handler_Check()
{

}

void CRun_Handler_Check::ButtonCheck_Start()
{
	int nRet, nRet2;
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.

	switch (StartStep)
	{
	case 0:
		// **********************************************************************
		// RUN 키 I/O 입력이 On 상태이고 상태가 STOP 상태였는지 검사             
		// -> 다시 말해, STOP 상태에서만 RUN 키 입력이 동작한다                  
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == TRUE && COMI.mn_run_status == dSTOP)
		{
			// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
			if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE)
			{
				break;
			}

			StartStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함        
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		StartStep = 200;
		break;

	case 200: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

		if(SwitchWaitTime[2] > 50 && g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			StartStep = 300;
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		else if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
		{
			StartStep = 0;
		}
		break;

	case 300:
		// **********************************************************************
		// RUN 키 I/O 입력이 Off 되었는지 검사                                   
		// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
		// **********************************************************************
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
			{
				StartStep = 600;
			}
			else
			{
				StartStep = 0;
			}
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		// **********************************************************************
		break;

	case 600:
		// **********************************************************************
		//  RUN할수 있는 조건인지 검사한다. 이상이 없어야 RUN이 된다.            
		// **********************************************************************

		alarm.mb_bcr_error = false;

		if (st_handler.mn_motor_init_check != CTL_YES)	// 모터 초기화를 하지 않았다면 동작하지 않는다. 2K4/11/23/ViboX
		{
			Func.OnSet_IO_Port_Sound(IO_ON);
//			st_msg.mstr_abnormal_msg = _T("[MOTOR INIT CHECK] Motor Initial..., become Run.");
			sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial..., become Run.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			alarm.mstr_code = _T("900001");		// Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			sprintf(alarm.mc_code, "900001");
			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
			COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			StartStep = 0;
//			g_handler.AddAlarmCnt();
			return;
		}

// 		// JAM이 리셋 되었는지 확인하고 RUN을 시킨다.
		if (alarm.mn_reset_status != CTL_NO)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first Jam Reset.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			StartStep = 0;
			break;
		}

// 		537000 1 00 "Door1_Open_Close_Check_Off_Error."
// 		537001 1 00 "Door1_Open_Close_Check_On_Error."
// 		537010 1 00 "Door2_Open_Close_Check_Off_Error."
// 		537011 1 00 "Door2_Open_Close_Check_On_Error."
// 		537020 1 00 "Door3_Open_Close_Check_Off_Error."
// 		537021 1 00 "Door3_Open_Close_Check_On_Error."
// 		537030 1 00 "Door4_Open_Close_Check_Off_Error."
// 		537031 1 00 "Door4_Open_Close_Check_On_Error."
// 		537040 1 00 "Door5_Open_Close_Check_Off_Error."
// 		537041 1 00 "Door5_Open_Close_Check_On_Error."
// 		537050 1 00 "Door6_Open_Close_Check_Off_Error."
// 		537051 1 00 "Door6_Open_Close_Check_On_Error."
// 		537060 1 00 "Door7_Open_Close_Check_Off_Error."
// 		537061 1 00 "Door7_Open_Close_Check_On_Error."
// 		537070 1 00 "Door8_Open_Close_Check_Off_Error."
// 		537071 1 00 "Door8_Open_Close_Check_On_Error."
// 		537080 1 00 "Door9_Open_Close_Check_Off_Error."
// 		537081 1 00 "Door9_Open_Close_Check_On_Error."
// 		537090 1 00 "Door10_Open_Close_Check_Off_Error."
// 		537091 1 00 "Door10_Open_Close_Check_On_Error."
		nRet = Func.OnIsDoorOpen();
		if ( nRet != -1)//2015,0207//2017.0510
		{
			Func.OnSet_IO_Port_Sound(IO_ON);

			sprintf(st_msg.c_abnormal_msg, "[DOOR CHECK] Door Open being done Run become.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			CString strJamCode="";
			strJamCode.Format("5370%d1",nRet);
			CTL_Lib.Alarm_Error_Occurrence( 620, dWARNING, strJamCode );			
// 			COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			StartStep = 0;
			break;
		}
	
		// Air 감지 센서를 확인한다.
		if (g_ioMgr.Get_In_Bit(st_io.i_main_air_chk) == FALSE)
		{
			Func.OnSet_IO_Port_Sound(IO_ON);
//			st_msg.mstr_abnormal_msg = _T("[AIR CHECK] Though supply Air, become Run.");
			sprintf(st_msg.c_abnormal_msg, "[AIR CHECK] Though supply Air, become Run.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			StartStep = 0;
			return;
		}


		nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
		
		if (nRet != IO_ON || nRet2 != IO_ON)
		{
			if(nRet != IO_ON ) 	st_handler.mstrSelectMessage = "FRONT KEY BUTTON CHECK...";
			else				st_handler.mstrSelectMessage = "REAR KEY BUTTON CHECK...";
			st_handler.mnSelectMessage = 0;
			
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
			
			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
			
			StartStep = 620;
			break;
		}


// 		if (g_ioMgr.Get_In_Bit(st_io.i_servo_motor_power_chk) == CTL_NO)
// 		{
// 			st_handler.mstrSelectMessage = "먼저 RESET 버튼을 눌러 주세요.";
// 			
// 			if ( g_local.GetLocalType() == LOCAL_ENG )
// 			{
// 				st_handler.mstrSelectMessage = "RESET BUTTON CHECK...";
// 			}
// 			st_handler.mnSelectMessage = 0;
// 			
// 			
// 			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
// 			
// 			StartStep = 620;
// 			break;
// 		}
		
		if (st_basic.mn_mode_run == CTL_NO)		// OFFLINE이면 상태를 알리자.
		{
			st_handler.mstrSelectMessage = "OFFLINE 상태입니다. RUN 하시겠습니까?";
			
			if ( g_local.GetLocalType() == LOCAL_ENG )
			{
				st_handler.mstrSelectMessage = "NOW OFFLINE. ARE YOU RUN?";
			}
			st_handler.mnSelectMessage = 0;
			
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
			
//			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
			
			StartStep = 610;
			break;
		}
		//ybs
		if (g_ioMgr.Get_In_Bit(st_io.i_mc1_chk) == CTL_NO)
		{
			st_handler.mstrSelectMessage = "Check MC1. Motor,IO Power... Reset Button Check";
			st_handler.mnSelectMessage = 0;
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
			
			StartStep = 610;
			break;
		}
		

		StartStep = 700;
		break;

	case 610:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			StartStep = 700;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			StartStep = 0;
		}
		break;

	case 620:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			StartStep = 0;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			StartStep = 0;
		}
		break;

	case 700:
		if(COMI.mn_run_status != dSTOP) break;

// 		Ret = Func.Robot_Reinstatement_Position(1);		// 복구 동작 확인 해야 함.
// 
// 		if (Ret == RET_GOOD)
// 		{	//정상완료
// 
// 		}
// 		else
// 		{
// 			if(COMI.mn_run_status != dSTOP)
// 			{
// 				StartStep = 0;
// 			}
// 			break;
// 		}

		//2013,0124
// 		st_work.md_reinstatement_pos[0] = CTL_NO;
// 		st_work.md_reinstatement_pos[1] = CTL_NO;
// 		st_work.md_reinstatement_pos[2] = CTL_NO;

		st_work.n_sync_reinstate = CTL_NO;
		st_work.mn_xyz_safety = CTL_NO;

		st_work.mn_reinstate_xyz_robot		= CTL_NO;
		st_work.mn_reinstate_reject_robot	= CTL_NO;
		st_work.mn_reinstate_tester_robot[0] = st_work.mn_reinstate_tester_robot[1] = CTL_NO;
		st_work.mn_reinstate_stacker_robot[0] = st_work.mn_reinstate_stacker_robot[1] = st_work.mn_reinstate_stacker_robot[2] = CTL_NO;

		if(st_handler.cwnd_title != NULL)
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);
		}


		if(st_handler.mn_virtual_supply)
		{
 			st_work.mn_reinstate_stacker_robot[0] = CTL_YES;
 			st_work.mn_reinstate_stacker_robot[1] = CTL_YES;
			st_work.mn_reinstate_stacker_robot[2] = CTL_YES;
		}
		else
		{
			COMI.mn_run_status					= dREINSTATE;
		}
		StartStep = 800;
		break;

	case 800:
		StartStep = 900;
		break;
		if(st_work.mn_reinstate_xyz_robot == CTL_YES && st_work.mn_reinstate_reject_robot == CTL_YES &&
			st_work.mn_reinstate_tester_robot[0] == CTL_YES && st_work.mn_reinstate_tester_robot[1] == CTL_YES && 
			st_work.mn_reinstate_stacker_robot[0] == CTL_YES &&	st_work.mn_reinstate_stacker_robot[1] == CTL_YES && 
			st_work.mn_reinstate_stacker_robot[2] == CTL_YES)
		{
			StartStep = 900;
		}
		else
		{
			if(COMI.mn_run_status != dREINSTATE)	//복귀 동작이 성공도, 실패도 아닌데 복귀동작 상태가 아니라면..
			{
				COMI.mn_run_status = dSTOP;
				if(st_handler.cwnd_title != NULL)
				{
					st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);
				}
				StartStep = 0;
			}
		}
		break;

	case 900:
		// **********************************************************************
		// RUN 키 I/O 입력이 Off 되었는지 검사                                   
		// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
		// **********************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
	
		if (st_handler.mn_menu_num != 101)
		{
			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
		}
		{			
			//2015.0219
			bool nRun = false;
			if( st_basic.mn_mode_xgem == CTL_YES)
			{
				if(st_basic.mn_mode_rcmd == CTL_YES)
				{
					if( st_handler.m_nRemote == CTL_YES )
					{
						nRun = false;
					}
					else
					{
						nRun = true;
					}
				}
			}
			if(nRun == true)
			{
				StartStep = 910;
			}
			else
			{
				StartStep = 1000;
			}
		}
		break;

	case 910:
		Func.OnSavePPID();
		mdTimeXgem = GetCurrentTime();
		StartStep = 920;
		break;

	case 920:
		{
			bool nRun = false;
			if( st_basic.mn_mode_xgem == CTL_YES)
			{
				if(st_basic.mn_mode_rcmd == CTL_YES)
				{
					if( st_handler.m_nRemote == CTL_YES )
					{
						nRun = false;
					}
					else
					{
						nRun = true;
					}
				}
			}
			if(nRun == false)
			{
				StartStep = 1000;
			}
			else
			{
				if( GetCurrentTime() - mdTimeXgem <= 0 ) mdTimeXgem = GetCurrentTime();
				if( GetCurrentTime() - mdTimeXgem > 10000 )
				{
					sprintf(st_msg.c_abnormal_msg, "[XGEM Check] Remote stop or not response.");
					if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					//950000 0 00 "XGEM Remote stop or not response"
					alarm.mstr_code = _T("950000");
					sprintf(alarm.mc_code, "950000");
					alarm.mn_count_mode = 0;
					alarm.mn_type_mode = eWARNING;
					COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
					StartStep = 0;
				}
			}
		}
		break;

	case 1000:
		Func.OnSet_IO_Port_Run();									// 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
		alarm.mn_emo_alarm = FALSE;

		n_emo_chk = FALSE;
		n_air_chk = FALSE;

		n_light_curtain_chk = CTL_NO;

		StartStep = 0;
		break;
	
	default :
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_Stop()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	switch (StopStep)
	{
	case 0:
		// **********************************************************************
		// STOP 키 I/O 입력이 On 상태이고 현재 STOP 상태가 아닌지 검사           
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE && COMI.mn_run_status != dSTOP)
		{
			// 만일 Start Button이 같이 눌렸다면 동작되지 않는다.
			if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == TRUE)
			{
				break;
			}

			StartStep = 0;
			StopStep = 100;
//			StopStep = 600;
		}
		else if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE)
		{
			StopStep = 100;
//			StopStep = 600;
		}
		break;

	case 100:
		// **********************************************************************
		// START 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함        
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		StopStep = 200;
		break;
		
	case 200: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50 && g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			StopStep = 300;
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		else if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			StopStep = 0;
		}
		break;
		
	case 300:
		// **********************************************************************
		// RUN 키 I/O 입력이 Off 되었는지 검사                                   
		// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
		// **********************************************************************
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
			{
				StopStep = 600;
			}
			else
			{
				StopStep = 0;
			}
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		// **********************************************************************
		break;

	case 600:
		// **********************************************************************
		// STOP 키 I/O 입력이 Off 되었는지 검사                                  
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			StopStep = 0; 

			if (COMI.mn_run_status == dINIT)				// 초기화 중에 Stop 버튼이 눌리면... 2K5/02/23/ViboX
			{
				st_handler.mn_initial_error = TRUE;
				
				alarm.mstr_code = _T("900001");		// Alarm 화면을 띄워준다. 2K5/03/10/ViboX
				sprintf(alarm.mc_code, "900001");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
				COMI.mn_run_status = dWARNING;	// dJAM,dWARNING 

//				g_handler.AddAlarmCnt();
				break;
			}

// 			if (st_handler.mn_menu_num != 101)
// 			{
// 				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
// 			}
			Func.OnSet_IO_Port_Stop();
			{//2015.0513
				CString strLog;
				//strLog.Format(" Stop, Manual stop");
				//2017.0510
				strLog = " Stop, Manual stop";
				Func.On_LogFile_Add(LOG_EVENT, strLog, 1);
			}
		}
		break;

	default : 
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_BuzzerOff()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	switch(BuzzerStep)
	{
	case 0:
		// **********************************************************************
		//  Reset 키 I/O 입력이 On 상태인지 확인                                 
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_buzzer_switch_chk) == TRUE)
		{
			BuzzerStep = 10;
		}
		break;
		
	case 10:
		// **********************************************************************
		//  Reset 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함       
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		BuzzerStep = 20;
		break;
		
	case 20: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50 && g_ioMgr.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			BuzzerStep = 30;
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		else if (g_ioMgr.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
		{
			BuzzerStep = 0;
		}
		break;
		
	case 30: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
			{
				BuzzerStep = 40;
			}
			else
			{
				BuzzerStep = 0;
			}
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		// **********************************************************************
		break;
		
	case 40:
		// **********************************************************************
		// Sound 키 I/O 입력이 Off 되었는지 검사                                 
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
		{
//			st_msg.mstr_normal_msg = _T("[SOUND] OFF.");
			sprintf(st_msg.c_normal_msg, "[SOUND] OFF.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			
			Func.OnSet_IO_Port_Sound(IO_OFF);
			
			BuzzerStep = 0;
		}
		break;
		
	default : 
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_Alarm_Clear()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	switch(AlaemClearStep)
	{
	case 0:
		// **********************************************************************
		//  Reset 키 I/O 입력이 On 상태인지 확인                                 
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_alarm_clear_switch_chk) == TRUE)
		{
			AlaemClearStep = 10;
		}
		break;
		
	case 10:
		// **********************************************************************
		//  Reset 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함       
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		AlaemClearStep = 20;
		break;
		
	case 20: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50 && g_ioMgr.Get_In_Bit(st_io.i_alarm_clear_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			AlaemClearStep = 30;
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		else if (g_ioMgr.Get_In_Bit(st_io.i_alarm_clear_switch_chk) == FALSE)
		{
			AlaemClearStep = 0;
		}
		break;
		
	case 30: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.Get_In_Bit(st_io.i_alarm_clear_switch_chk) == FALSE)
			{
				AlaemClearStep = 40;
			}
			else
			{
				AlaemClearStep = 0;
			}
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		break;
		
	case 40:
		// **********************************************************************
		// Reset 키 I/O 입력이 Off 되었는지 검사                                 
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_alarm_clear_switch_chk) == FALSE)
		{
			AlaemClearStep = 110;
		}
		break;
		
	case 110:
		alarm.mn_reset_status = CTL_NO;
//		st_msg.mstr_normal_msg = _T("[ALARM CLEAR] JAM RESET.");
		sprintf(st_msg.c_normal_msg, "[ALARM CLEAR] JAM RESET.");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		
		if (COMI.mn_run_status != dRUN)	Func.OnSet_IO_Port_Stop();
		g_ioMgr.set_out_bit(st_io.o_alarm_clear_lamp, IO_OFF);	// Reset Switch에 Lamp를 Off 시킨다.
		
		AlaemClearStep = 0;
		break;
		
	default : 
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_ManualAutoKey()//2015.0114
{
	if(g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_ON) == IO_ON)
	{
		OnTurn_Front_Auto();		// [Auto] ¡ioAA¡¤I ¨o¨￢A¡×A￠® ￥iⓒo￠￢¡Æ ¡Æⓒ¡￠?i Aⓒø￠￢￠c
		OnTurn_Front_Manual();	// [Manual] ¡ioAA¡¤I ¨o¨￢A¡×A￠® ￥iⓒo￠￢¡Æ ¡Æⓒ¡￠?i Aⓒø￠￢￠c
	}

	if(g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_ON) == IO_ON)
	{
		OnTurn_Rear_Auto();		// [Auto] ¡E￠cioAA¡E￠c¡E￠￥I ￠®¡×o￠®¡×￠®ⓒ­A¡E￠c￠®￠?A￠®E¡Ec ￠®Ii¡§Io￠®E￠®ⓒ­¡E￠c¡§￠® ¡E￠c¡§￠®¡§I¡E￠c￠®E?i A¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec
		OnTurn_Rear_Manual();	// [Manual] ¡E￠cioAA¡E￠c¡E￠￥I ￠®¡×o￠®¡×￠®ⓒ­A¡E￠c￠®￠?A￠®E¡Ec ￠®Ii¡§Io￠®E￠®ⓒ­¡E￠c¡§￠® ¡E￠c¡§￠®¡§I¡E￠c￠®E?i A¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec
	}

	int nRet, nRet2;

	nRet = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);

	if (n_front_key != nRet)
	{
		n_front_key = nRet;

		if (st_handler.cwnd_title != NULL)	
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FRONT_KEY_MODE, n_front_key);
		}
	}

	if (n_rear_key != nRet2)
	{
		n_rear_key = nRet2;

		if (st_handler.cwnd_title != NULL)	
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, REAR_KEY_MODE, n_rear_key);
		}
	}
// 
// 	if (COMI.mn_run_status == dRUN)
// 	{
// 		if (nRet == IO_ON && nRet2 == IO_ON)
// 		{			
// 			
// 		}
// 	}
// 
// 	nRet3 = st_handler.mn_menu_num;
// // 
// // // #define IDW_SCREEN_MAIN					101
// // // #define IDW_SCREEN_LOCK					102
// // // #define IDW_SCREEN_INIT					103
// // // 	
// // // #define IDW_SCREEN_BASIC		 			201
// // // 	
// // // #define IDW_SCREEN_SET_MAINTENANCE		301
// // // 	
// // // #define IDW_SCREEN_WAIT_TIME				401
// // // 	
// // // #define IDW_SCREEN_MOTOR_TEACH			501
// // // #define IDW_SCREEN_MOTOR_SPEED			502
// // // 	
// // // #define IDW_SCREEN_LIST_OPERATION		601
// // // #define IDW_SCREEN_LIST_DATA_VIEW		602
// // // #define IDW_SCREEN_LIST_STEP				603
// // // 	
// // // #define IDW_SCREEN_IO					701
// // // 	
// // // #define IDW_SCREEN_ADMINISTRATOR			801
// // 	
// 	if (nRet3 == IDW_SCREEN_MOTOR_TEACH || nRet3 == IDW_SCREEN_MOTOR_SPEED || nRet3 == IDW_SCREEN_IO)
// 	{
// 		if (nRet == IO_ON && nRet2 == IO_ON)
// 		{			
// 			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// ￠￢¨­AI CA¡¤ⓒoAOA￠￢¡¤I ￠￢¨­¨oAAo Au¨uUCI￠?¨I ￠￢¨­AI E¡ⓒ￠￢eA￠￢¡¤I AuE? 
// 		}
// 	}
}

void CRun_Handler_Check::ButtonCheck_LabelLockUnLock(int i)
{
	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.

// 	if(COMI.mn_run_status == dRUN) return;//2014.1018

	//2015.0312
	if( COMI.mn_run_status == dRUN &&
		(st_basic.mn_device_type == i || (st_basic.mn_device_type+3) == i ) ) return;

	switch (LabelLockStep[i])
	{
	case 0:
// 		if (g_ioMgr.get_in_bit(st_io.i_density_lock_switch_chk[i], IO_OFF) == IO_ON && COMI.mn_run_status == dSTOP)
// 		{
// 			// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
// 			if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
// 			{
// 				break;
// 			}
// 
// 			LabelLockStep[i] = 100;
// 		}
// 		if(COMI.mn_run_status != dRUN)
		{
			if ( g_ioMgr.get_in_bit(st_io.i_density_lock_switch_chk[i], IO_OFF) == IO_ON &&
				 g_ioMgr.get_out_bit(st_io.o_density_lock_switch[i], IO_ON) == IO_OFF)
			{
				// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
				{
					break;
				}
				st_var.n_err_density_flag[i] = CTL_NO;//density에러이거나 ready or complete가 반응이 없다
				st_var.n_use_density_flag[i] = CTL_YES;
				st_var.n_use_density_count[i] = 0;
// 				st_var.n_use_density_key = CTL_YES;

				st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
				st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
				st_var.n_use_density_count[st_basic.mn_device_type] = 0;
				st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;

				LabelLockStep[i] = 100;
			}
			else if ( g_ioMgr.get_in_bit(st_io.i_density_lock_switch_chk[i], IO_OFF) == IO_ON &&
				      g_ioMgr.get_out_bit(st_io.o_density_lock_switch[i], IO_OFF) == IO_ON)
			{    
				// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
				{
					break;
				}
				st_var.n_use_density_flag[i] = CTL_YES;
				st_var.n_use_density_count[i] = 0;
// 				st_var.n_use_density_key = CTL_YES;

				st_var.n_use_density_flag[st_basic.mn_device_type] = CTL_YES;
				st_var.n_use_density_flag[st_basic.mn_device_type+3] = CTL_YES;
				st_var.n_use_density_count[st_basic.mn_device_type] = 0;
				st_var.n_use_density_count[st_basic.mn_device_type+3] = 0;

				LabelLockStep[i] = 100;
			}
		}
		break;

	case 100:
		m_dwWaitLabelLock[i] = GetCurrentTime();
		LabelLockStep[i] = 200;
		break;

	case 200:
		if( GetCurrentTime() - m_dwWaitLabelLock[i] < 0) m_dwWaitLabelLock[i] = GetCurrentTime();
		if( GetCurrentTime() - m_dwWaitLabelLock[i] > 300  && g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
		{
			m_dwWaitLabelLock[i] = GetCurrentTime();
			LabelLockStep[i] = 300;
		}
		break;

	case 300:
		if( GetCurrentTime() - m_dwWaitLabelLock[i] < 0) m_dwWaitLabelLock[i] = GetCurrentTime();
		if( GetCurrentTime() - m_dwWaitLabelLock[i] > 10 )
		{
			if(g_ioMgr.get_in_bit(st_io.i_density_lock_switch_chk[i], IO_ON) == IO_OFF)
			{
				LabelLockStep[i] = 400;
			}
			if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == TRUE)
			{
				LabelLockStep[i] = 0;
			}
		}
		break;

	case 400:
		if( g_ioMgr.get_out_bit( st_io.o_density_lock_switch[i],IO_OFF) == IO_ON)
		{
			g_Run_Density_Feeders.SetLock( i, IO_OFF);//UNLOCK
			m_dwWaitLabelLock[i] = GetCurrentTime();
			LabelLockStep[i] = 410;
		}
		else if( g_ioMgr.get_out_bit( st_io.o_density_lock_switch[i],IO_ON) == IO_OFF)
		{
// 			if( g_ioMgr.get_in_bit( st_io.i_density_position_chk[i], IO_ON) == IO_OFF)
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "[ALARM] Density don't detect density reel %d th pos sensor.", i+1);
// 				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 				LabelLockStep[i] = 0;
// 				break;
// 			}
			g_Run_Density_Feeders.SetLock( i, IO_ON);//LOCK
			m_dwWaitLabelLock[i] = GetCurrentTime();
			LabelLockStep[i] = 420;
		}
		break;

	case 410:
		if( g_Run_Density_Feeders.CheckLock( i, IO_OFF ) ){//ybs
			LabelLockStep[i] = 600;
		}
		else if( GetCurrentTime() - m_dwWaitLabelLock[i] > st_time.n_limit_time[E_WAIT_FEEDER_LOCK] )
		{
// 			CString strJamCode;
// 			strJamCode.Format("5%04d%d",st_io.i_density_position_chk[i], IO_OFF );
// 			CTL_Lib.Alarm_Error_Occurrence( 403, dWARNING, (LPCTSTR)strJamCode );
 			LabelLockStep[i] = 600;
		}
		else if( GetCurrentTime() - m_dwWaitLabelLock[i] < 0 )
		{
			m_dwWaitLabelLock[i] = GetCurrentTime();
		}

		break;
		
	case 420:
		if( g_Run_Density_Feeders.CheckLock( i, IO_ON ) ){//ybs
			LabelLockStep[i] = 500;
		}
		else if( GetCurrentTime() - m_dwWaitLabelLock[i] > st_time.n_limit_time[E_WAIT_FEEDER_LOCK] )
		{
// 			CString strJamCode;
// 			strJamCode.Format("5%04d%d",st_io.i_density_position_chk[i], IO_ON );
// 			CTL_Lib.Alarm_Error_Occurrence( 403, dWARNING, (LPCTSTR)strJamCode );
			LabelLockStep[i] = 500;
		}
		else if( GetCurrentTime() - m_dwWaitLabelLock[i] < 0 )
		{
			m_dwWaitLabelLock[i] = GetCurrentTime();
		}

		break;
		////

	case 500:
		g_ioMgr.set_out_bit( st_io.o_density_lock_switch[i], IO_ON );
		LabelLockStep[i] = 0;
		break;

	case 600:
		g_ioMgr.set_out_bit( st_io.o_density_lock_switch[i], IO_OFF );
		LabelLockStep[i] = 0;
	}
}

void CRun_Handler_Check::ButtonCheck_LabelPaperLockUnLock(int i)
{
//	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
//	// **************************************************************************
//
//	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.
//
//	if(COMI.mn_run_status == dRUN) return;//2014.1018
//
//	switch (LabelPaperLockStep[i])
//	{
//	case 0:
////		if (g_ioMgr.get_in_bit(st_io.i_density_paper_pusher_switch_chk[i], IO_OFF) == IO_ON && COMI.mn_run_status == dSTOP)
//// 		{
//// 			// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
//// 			if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
//// 			{
//// 				break;
//// 			}
//// 
//// 			LabelPaperLockStep[i] = 100;
//// 		}
//		if(COMI.mn_run_status != dRUN)
//		{
//			if (g_ioMgr.get_in_bit(st_io.i_density_paper_pusher_switch_chk[i], IO_OFF) == IO_ON && 
//				g_ioMgr.get_out_bit(st_io.o_density_paper_pusher_switch[i], IO_ON) == IO_OFF)
//	 		{
//				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
//				{
//					break;
//				}
//
//				LabelPaperLockStep[i] = 100;
//			}
//			else if (g_ioMgr.get_in_bit(st_io.i_density_paper_pusher_switch_chk[i], IO_OFF) == IO_ON && 
//				g_ioMgr.get_out_bit(st_io.o_density_paper_pusher_switch[i], IO_OFF) == IO_ON)
//			{
//				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
//				{
//					break;
//				}
//				
//				LabelPaperLockStep[i] = 100;
//			}
//		}
//		break;
//	case 100:
//		m_dwWaitLabelPaperLock[i] = GetCurrentTime();
//		//BcrLockStep[i] = 200;
//		LabelPaperLockStep[i] = 200;//2014.1018
//		break;
//
//	case 200:
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] < 0) m_dwWaitLabelPaperLock[i] = GetCurrentTime();
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] > 500  && g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
//		{
//			m_dwWaitLabelPaperLock[i] = GetCurrentTime();
//			LabelPaperLockStep[i] = 300;
//		}
//// 		else if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
//// 		{
//// 			LabelPaperLockStep[i] = 0;
//// 		}
//		break;
//
//	case 300:
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] < 0) m_dwWaitLabelPaperLock[i] = GetCurrentTime();
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] > 10 )
//		{
//			if (g_ioMgr.Get_In_Bit(st_io.i_density_paper_pusher_switch_chk[i], IO_ON) == IO_OFF)
//			{
//				LabelPaperLockStep[i] = 400;
//			}
//			else if(g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == TRUE)//2014.1018
//			{
//				LabelPaperLockStep[i] = 0;
//			}
//		}
//		break;
//
//	case 400:
//		if( g_ioMgr.get_out_bit( st_io.o_density_paper_pusher_switch[i],IO_OFF) == IO_ON)
//		{
//			g_Run_Density_Feeders.ReqFeeder(i, BACKWARD);//2014.1018 ReqLock( i, BACKWARD) -> ReqFeeder(i, BACKWARD)
//			//LabelPaperLockStep[i] = GetCurrentTime();
//			m_dwWaitLabelPaperLock[i] = GetCurrentTime();//2014.1018
//			LabelPaperLockStep[i] = 500;
//		}
//		else if( g_ioMgr.get_out_bit( st_io.o_density_paper_pusher_switch[i],IO_ON) == IO_OFF)
//		{
//			g_Run_Density_Feeders.ReqFeeder( i, FORWARD);//2014.1018 ReqLock( i, FORWARD) -> ReqFeeder( i, FORWARD)
//			//LabelPaperLockStep[i] = GetCurrentTime();
//			m_dwWaitLabelPaperLock[i] = GetCurrentTime();//2014.1018
//			LabelPaperLockStep[i] = 600;
//		}
//		break;
//	
//
//	case 500:
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] < 0) m_dwWaitLabelPaperLock[i] = GetCurrentTime();
//		
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] > 10000)
//		{
//			CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_density_paper_pusher_fwdback_chk[i], IO_OFF );
//			CTL_Lib.Alarm_Error_Occurrence( 630+i, dWARNING, strJamCode );
//			LabelPaperLockStep[i] = 0;
//			break;
//		}
////		if( g_Run_Density_Feeders.GetLock(i) != FB_NONE ) break;
//		if( g_Run_Density_Feeders.GetFeeder(i) != FB_NONE ) break;//2014.1018
//		g_ioMgr.set_out_bit( st_io.o_density_paper_pusher_switch[i], IO_OFF);
//		LabelPaperLockStep[i] = 0;
//		break;
//
//	case 600:
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] < 0) m_dwWaitLabelPaperLock[i] = GetCurrentTime();
//		
//		if( GetCurrentTime() - m_dwWaitLabelPaperLock[i] > 10000)
//		{
//			CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_density_paper_pusher_fwdback_chk[i], IO_OFF );
//			CTL_Lib.Alarm_Error_Occurrence( 630+i, dWARNING, strJamCode );
//			LabelPaperLockStep[i] = 0;
//			break;
//		}
////		if( g_Run_Density_Feeders.GetLock(i) != FB_NONE ) break;
//		if( g_Run_Density_Feeders.GetFeeder(i) != FB_NONE ) break;//2014.1018
//		g_ioMgr.set_out_bit( st_io.o_density_paper_pusher_switch[i], IO_ON);
//		LabelPaperLockStep[i] = 0;
//		break;
//	}
}


void CRun_Handler_Check::ButtonCheck_BarcodeLockUnLock(int i)
{
	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.

// 	if(COMI.mn_run_status == dRUN) return;//2014.1018
	if(COMI.mn_run_status == dRUN && st_basic.nBcrFeederUse[i] == CTL_YES) return;

	switch (BcrLockStep[i])
	{
	case 0:
// 		if (g_ioMgr.get_in_bit(st_io.i_distribute_lockunlock_switch_chk[i], IO_OFF) == IO_ON && COMI.mn_run_status == dSTOP)
// 		{
// 			// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
// 			if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
// 			{
// 				break;
// 			}
// 
// 			BcrLockStep[i] = 100;
// 		}
// 		if(COMI.mn_run_status != dRUN)
		{
			if (g_ioMgr.get_in_bit(st_io.i_distribute_lockunlock_switch_chk[i], IO_OFF) == IO_ON &&
				g_ioMgr.get_out_bit( st_io.o_distribute_lockunlock_switch_lamp[i],IO_ON) == IO_OFF)
			{
				// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
				{
					break;
				}
				
				BcrLockStep[i] = 100;
			}
			else if (g_ioMgr.get_in_bit(st_io.i_distribute_lockunlock_switch_chk[i], IO_OFF) == IO_ON &&
				g_ioMgr.get_out_bit( st_io.o_distribute_lockunlock_switch_lamp[i],IO_OFF) == IO_ON)
			{
				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
				{
					break;
				}
				
				BcrLockStep[i] = 100;
			}
		}
		break;
		
	case 100:
		m_dwWaitBcrLock[i] = GetCurrentTime();
		BcrLockStep[i] = 200;
		break;

	case 200:
		if( GetCurrentTime() - m_dwWaitBcrLock[i] < 0) m_dwWaitBcrLock[i] = GetCurrentTime();
		if( GetCurrentTime() - m_dwWaitBcrLock[i] > 500  && g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
		{
			m_dwWaitBcrLock[i] = GetCurrentTime();
			BcrLockStep[i] = 300;
		}
// 		else if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)//2014.1018
// 		{
// 			BcrLockStep[i] = 0;
// 		}
		break;

	case 300:
		if( GetCurrentTime() - m_dwWaitBcrLock[i] < 0) m_dwWaitBcrLock[i] = GetCurrentTime();
		if( GetCurrentTime() - m_dwWaitBcrLock[i] > 10 )
		{
			if (g_ioMgr.get_in_bit(st_io.i_distribute_lockunlock_switch_chk[i], IO_ON) == IO_OFF)
			{
				BcrLockStep[i] = 400;
			}
			else if(g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == TRUE)//2014.1018
			{
				BcrLockStep[i] = 0;
			}
		}
		break;

	case 400:
		if( g_ioMgr.get_out_bit( st_io.o_distribute_lockunlock_switch_lamp[i],IO_OFF) == IO_ON)
		{
			g_Run_Barcode_Feeders.ReqBcrLock(i, BACKWARD);
			m_dwWaitBcrLock[i] = GetCurrentTime();
			BcrLockStep[i] = 410;//2014.1018 500 -> 410
		}
		else if( g_ioMgr.get_out_bit( st_io.o_distribute_lockunlock_switch_lamp[i],IO_ON) == IO_OFF)
		{
			g_Run_Barcode_Feeders.ReqBcrLock(i, FORWARD);
			m_dwWaitBcrLock[i] = GetCurrentTime();
			BcrLockStep[i] = 420;//2014.1018 600 -> 420
		}
		break;

	////2014.1018
	case 410:
		if( g_ioMgr.get_in_bit( st_io.i_distribute_lock_position_chk[i],IO_OFF) == IO_ON)
		{
			BcrLockStep[i] = 500;
		}
		else if( g_ioMgr.get_in_bit( st_io.i_distribute_lock_position_chk[i],IO_ON) == IO_OFF)
		{
			CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[i], IO_OFF );
			CTL_Lib.Alarm_Error_Occurrence( 620+i, dWARNING, strJamCode );
			BcrLockStep[i] = 500;
		}
		break;

	case 420:
		if( g_ioMgr.get_in_bit( st_io.i_distribute_lock_position_chk[i],IO_OFF) == IO_ON)
		{
			BcrLockStep[i] = 600;
		}
		else if( g_ioMgr.get_in_bit( st_io.i_distribute_lock_position_chk[i],IO_ON) == IO_OFF)
		{
			CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[i], IO_OFF );
			CTL_Lib.Alarm_Error_Occurrence( 620+i, dWARNING, strJamCode );
			BcrLockStep[i] = 0;
		}
		break;
	////

	case 500:
		if( GetCurrentTime() - m_dwWaitBcrLock[i] < 0) m_dwWaitBcrLock[i] = GetCurrentTime();
		
		if( GetCurrentTime() - m_dwWaitBcrLock[i] > 10000)
		{
			CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[i], IO_OFF );
			CTL_Lib.Alarm_Error_Occurrence( 620+i, dWARNING, strJamCode );
			BcrLockStep[i] = 0;
			break;
		}
		if( g_Run_Barcode_Feeders.GetBcrLock(i) != FB_NONE ) break;
		g_ioMgr.set_out_bit( st_io.o_distribute_lockunlock_switch_lamp[i],IO_OFF );
		BcrLockStep[i] = 0;
		break;

	case 600:
		if( GetCurrentTime() - m_dwWaitBcrLock[i] < 0) m_dwWaitBcrLock[i] = GetCurrentTime();
		
		if( GetCurrentTime() - m_dwWaitBcrLock[i] > 10000)
		{
			CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[i], IO_OFF );
			CTL_Lib.Alarm_Error_Occurrence( 620+i, dWARNING, strJamCode );
			BcrLockStep[i] = 0;
			break;
		}
		if( g_Run_Barcode_Feeders.GetBcrLock(i) != FB_NONE ) break;
		g_ioMgr.set_out_bit( st_io.o_distribute_lockunlock_switch_lamp[i],IO_ON );
		BcrLockStep[i] = 0;
		break;


	}

}
void CRun_Handler_Check::ButtonCheck_Z_Axis_Down()
{
//	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
//	if (st_handler.cwnd_motorAxis == NULL &&
//		st_handler.cwnd_motor_with_io == NULL )  return;
//
//	BOOL bJogDn = g_ioMgr.get_in_bit( st_io.i_jog_dn_switch_chk );
//
//	if( m_bJog_Dn_old == bJogDn )	return;
//
//	if( bJogDn )
//	{
//		if( m_bJog_Up_old == FALSE )
//		{
//			int nRet = CTL_Lib.Motor_SafetyCheck(0, MOTOR_ROBOT_Z, 0);
//			
//			if (nRet == CTLBD_RET_ERROR)
//				return;
//			
//			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)MOTOR_ROBOT_Z ).JogMoveStart( PLUS );
//			
//
//			if( motRet == MOTRET_ERROR )
//			{
//				HWND hwnd = NULL;
//				if( st_handler.cwnd_motorAxis )
//					hwnd = st_handler.cwnd_motorAxis->GetSafeHwnd();
//				else if( st_handler.cwnd_motor_with_io )
//					hwnd = st_handler.cwnd_motor_with_io->GetSafeHwnd();
//
//				if( hwnd )
//					cmmErrShowLast(hwnd);
//			}
//			else					
//				st_handler.mn_menu_lock = FALSE;
//		}
//		else
//		{
//			cmmSxStop(MOTOR_ROBOT_Z, FALSE, TRUE);
//		}
//	}
//	else
//	{
//		cmmSxStop(MOTOR_ROBOT_Z, FALSE, TRUE);
//	}
//
//	m_bJog_Dn_old = bJogDn;
}


void CRun_Handler_Check::SafetyCheck_Door()
{
	// =============================================================================
    // 쓰레드 강제 리턴 조건 검사한다                                            
	// -> st_handler.n_system_lock : SYSTEM LOCK 상태 플래그                     
	// -> st_work.n_run_status : 장비 상태 플래그                                
    // =============================================================================

//	if( ( g_ioMgr.get_in_bit(st_io.i_emo_switch1_chk) == IO_OFF || g_ioMgr.get_in_bit(st_io.i_emo_switch2_chk) == IO_OFF ) && alarm.mn_emo_alarm != TRUE)
//	{
//		if(i_emo_check_flag == CTL_NO)
//		{
//			i_emo_check_flag = CTL_YES;			// 첫번째 EMO가 감지됐다.
//			EMO_CheckTime[0] = GetCurrentTime();
//		}
//		else
//		{
//			EMO_CheckTime[1] = GetCurrentTime();
//
//			if(EMO_CheckTime[1] < EMO_CheckTime[0])		// 위에 안거치고 온거다. 다시 올라가던가.
//			{
//				i_emo_check_flag = CTL_NO;
//			}
//			else
//			{
//				EMO_CheckTime[2] = EMO_CheckTime[1] - EMO_CheckTime[0];
//
//				if(EMO_CheckTime[2] > 300)		// 0.5초동안 감지되면..
//				{
//					if(COMI.mn_run_status == dINIT)
//					{
//						if(st_handler.cwnd_list != NULL)
//						{
//							sprintf( st_msg.c_abnormal_msg, "[Initialize Error] EMO is not check.");
//							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//						}
//						st_handler.mn_initial_error = TRUE;
//					}
//
//					i_emo_check_flag = CTL_NO;
//
//					//900200 0 90 "EMO Error."
//					alarm.mstr_code = _T("900200");		// 발생한 알람 코드 설정
//					CTL_Lib.Alarm_Error_Occurrence(933, CTL_dWARNING, alarm.mstr_code);
//					alarm.mn_emo_alarm = TRUE;
//				}
//			}
//		}
//	}
//	else
//	{
//		i_emo_check_flag = CTL_NO;
//	}

	if( COMI.mn_run_status != dRUN )
		return;

//537120 1 00 "Bottom Door1 Error."
//537130 1 00 "Bottom Door2 Error."
//537140 1 00 "Bottom Door3 Error."
//537150 1 00 "Bottom Door4 Error."
// 	if( g_ioMgr.get_in_bit( st_io.i_bottom_door1 ) == IO_ON )//20131204 ON->OFF
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence( 600, dWARNING, "537120" );
// 	}
// 	else if( g_ioMgr.get_in_bit( st_io.i_bottom_door2 ) == IO_ON )//20131204 ON->OFF
// 	{
// 		CTL_Lib.Alarm_Error_Occurrence( 601, dWARNING, "537130" );
// 	}
//	if( g_ioMgr.get_in_bit( st_io.i_bottom_door3 ) == IO_OFF )//20131204 ON->OFF
//	{
//		CTL_Lib.Alarm_Error_Occurrence( 602, dWARNING, "537140" );
//	}
//	else if( g_ioMgr.get_in_bit( st_io.i_bottom_door4 ) == IO_OFF )//20131204 ON->OFF
//	{
//		CTL_Lib.Alarm_Error_Occurrence( 604, dWARNING, "537150" );
//	}
	for ( int i = 0; i < 4; i++)
	{
		if(i == 0|| i== 1)
		{
			if( (/*g_ioMgr.get_out_bit( st_io.o_distribute_p_lock[i], IO_OFF ) == IO_ON ||*/
				g_ioMgr.get_in_bit( st_io.i_distribute_lock_chk[i], 0, 50) == IO_OFF ||
				g_ioMgr.get_in_bit( st_io.i_distribute_unlock_chk[i], 0, 50) == IO_ON) && st_basic.nBcrFeederUse[i] == CTL_YES &&
				st_var.n_using_barcode == 0)
			{
				CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[i], IO_OFF );
				CTL_Lib.Alarm_Error_Occurrence( 8102, dWARNING, strJamCode );
			}
		}
		else
		{
			if(i == 0|| i== 1)
			{
				if( (/*g_ioMgr.get_out_bit( st_io.o_distribute_p_lock[i], IO_OFF ) == IO_ON ||*/
					g_ioMgr.get_in_bit( st_io.i_distribute_lock_chk[i], 0, 50) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_unlock_chk[i], 0, 50) == IO_ON) && st_basic.nBcrFeederUse[i] == CTL_YES &&
					st_var.n_using_barcode == 1)
				{
					CString strJamCode = GetJamCode_FeederLock_Time(st_io.i_distribute_lock_chk[i], IO_OFF );
					CTL_Lib.Alarm_Error_Occurrence( 8103, dWARNING, strJamCode );
				}
			}
			
		}
	}
	if( (/*g_ioMgr.get_out_bit(st_io.o_density_lockunlock[st_basic.mn_device_type], IO_OFF) == IO_ON ||*/
		g_ioMgr.get_in_bit( st_io.i_density_lock_chk[st_basic.mn_device_type], 0, 50) == IO_OFF ||
		g_ioMgr.get_in_bit( st_io.i_density_unlock_chk[st_basic.mn_device_type], 0, 50) == IO_ON) )
	{
		CString strJamCode = GetJamCode_FeederLock_Time(st_io.o_density_lockunlock[st_basic.mn_device_type], IO_ON );
		CTL_Lib.Alarm_Error_Occurrence( 8403, dWARNING, strJamCode );

	}
	if( /*g_ioMgr.get_out_bit(st_io.o_density_lockunlock[st_basic.mn_device_type+3], IO_OFF) == IO_ON ||*/
		g_ioMgr.get_in_bit( st_io.i_density_lock_chk[st_basic.mn_device_type+3], 0, 50) == IO_OFF ||
		g_ioMgr.get_in_bit( st_io.i_density_unlock_chk[st_basic.mn_device_type+3], 0, 50) == IO_ON )
	{
		CString strJamCode = GetJamCode_FeederLock_Time(st_io.o_density_lockunlock[st_basic.mn_device_type+3], IO_ON );
		CTL_Lib.Alarm_Error_Occurrence( 8404, dWARNING, strJamCode );
		
	}
}

////2014,1207
void CRun_Handler_Check::ButtonCheck_Recycle_Zone_Door()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	if (COMI.mn_run_status != dSTOP) return;
	// **************************************************************************
	
	switch (Recycle_zone_Step)
	{
	case 0:
		if(COMI.mn_run_status != dRUN)
		{
			if (g_ioMgr.get_in_bit(st_io.i_recycle_zone_door_switch_chk, IO_OFF) == IO_ON && 
				g_ioMgr.get_in_bit(st_io.o_recycle_zone_door_switch_lamp, IO_ON) == IO_OFF)
			{
				Recycle_zone_Step = 100;
			}
			else if (g_ioMgr.get_in_bit(st_io.i_recycle_zone_door_switch_chk, IO_OFF) == IO_ON && 
				g_ioMgr.get_in_bit(st_io.o_recycle_zone_door_switch_lamp, IO_OFF) == IO_ON)
			{
				Recycle_zone_Step = 100;
			}
		}
		break;
		
	case 100:
		SwitchWaitTime[0] = GetCurrentTime();
		Recycle_zone_Step = 200;//2014.1018
		break;
		
	case 200:
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			Recycle_zone_Step = 300;
		}
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		break;
		
	case 300:
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.Get_In_Bit(st_io.i_recycle_zone_door_switch_chk, IO_ON) == IO_OFF)
			{
				Recycle_zone_Step = 400;
			}
		}
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		break;
		
	case 400:
		if( g_ioMgr.get_out_bit( st_io.o_recycle_zone_door_switch_lamp,IO_OFF) == IO_ON)
		{
			g_ioMgr.set_out_bit( st_io.o_recycle_door_onoff_cylinder, IO_ON);
			g_ioMgr.set_out_bit( st_io.o_recycle_zone_door_switch_lamp, IO_OFF);
			SwitchWaitTime[0] = GetCurrentTime();
			Recycle_zone_Step = 500;
		}
		else if( g_ioMgr.get_out_bit( st_io.o_recycle_zone_door_switch_lamp,IO_ON) == IO_OFF)
		{
			g_ioMgr.set_out_bit( st_io.o_recycle_door_onoff_cylinder, IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_recycle_zone_door_switch_lamp, IO_ON);
			Recycle_zone_Step = 0;
		}
		break;

	case 500:
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
		{
			//523041 1 00 "Recycle_Zone_Door_Lock_Unlock_Cylinder_On_Error."
			CString strJamCode;
			CTL_Lib.Alarm_Error_Occurrence( 102, dWARNING, "523041" );
			Recycle_zone_Step = 0;
		}
		if( g_ioMgr.get_in_bit( st_io.recycle_door_lock_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.recycle_door_unlock_chk, IO_ON) == IO_ON )
		{

		}
		else
		{
			Recycle_zone_Step = 0;
		}
	}
}
////

////2015.0114
//==================================================================//
// 모든 DOOR 닫힌 경우 강제로 DOOR LOCK 처리
//==================================================================//
void CRun_Handler_Check::OnRun_DoorLock() 
{
	int nRet = -1;
	int nSensor = -1;

	switch(m_nLockStep)
	{
	case 0:
		// (IDW_SCREEN_IO) 화면이 출력중인 경우에는 강제 리턴
		// - 강제로 도어를 열어놓았는데 다시 닫으면 안되기 때문임
		if (st_handler.mn_menu_num == 701)
		{
			break;
		}
		// 설비 상태가 매뉴얼 모드가 아니면 [오토 모드인 경우]
		// - 매뉴얼 모드에서만 도어 닫힘 상태 검사
		// - 오토 모드에서는 도어가 열리면 이미 모터 전언이 떨어져서 알람이 발생한 상황일것임
		if (st_handler.nModeManual != CTL_YES)
		{
			break;
		}

		nRet = Func.OnWatchDoor();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
		if (nRet > 0)
		{
			m_nDoorOpenTry = TRUE;	// 도어 열기 동작을 진해한 상태임
		}
		else
		{
			// 매뉴얼 모드로 전한되어 도어 잠금 상태를 해제했는데 도어를 열지 않고 있음
			// - 일정 시간 경과후에 도어를 강제로 잠금 상태로 변경함
			if (m_nDoorOpenTry != TRUE)
			{
				// DOOR 열린적이 없음
				// 하지만 도어락 스위치는 해제된 경우 [도어락 설정]
				nSensor = g_ioMgr.Get_Out_Bit(st_io.o_door_lock, IO_OFF);
				// 도어 잠금 해제 상태 [Release 상태]
				if (nSensor == IO_OFF)
				{
					m_lLockWaitTime[0] = GetCurrentTime();
					m_nLockStep = 1000;
				}
				else
				{
					break;
				}
			}
			// 도어가 [열림->닫힘] 상태로 변경된 경우
			else
			{
				m_nLockStep = 100;
			}
		}
		break;

	case 100:
		// 도어 잠금 상태 확인
		// - 도어 잠금이 해제되어 있으면 도어 잠금 신호 출력 [모든 도어 잠금]
		nSensor = g_ioMgr.Get_Out_Bit(st_io.o_door_lock, IO_OFF);
		// 도어 잠금 해제함
		if (nSensor == IO_OFF)
		{
			// 모든 도어가 닫혔으므로 도어 잠금을 설정함
			g_ioMgr.set_out_bit(st_io.o_door_lock, IO_ON);

			// 도어 잠금 신호 출력 후 곧바로 도어가 잠기는 것이 아니기 때문에 일정 시간 경과후에 동작 완료시킴
			m_lLockWaitTime[0] = GetCurrentTime();
			m_nLockStep = 200;
		}
		// 도어 잠금이 설정된 경우 [도어 열림부터 다시 검사 재시작]
		else
		{
			m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
			m_nLockStep = 0;
		}
		break;

	case 200:
		// 도어가 완전히 잠길때까지 일정 시간 기다림 [0.5초 대기]
		m_lLockWaitTime[1] = GetCurrentTime();
		m_lLockWaitTime[2] = m_lLockWaitTime[1] - m_lLockWaitTime[0];
		if (m_lLockWaitTime[2] < 0)
		{
			m_lLockWaitTime[0] = GetCurrentTime();
			break;
		}

		if (m_lLockWaitTime[2] > 500)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "Door [Locking]");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
			m_nLockStep = 0;
		}
		break;

		// 도어 잠금 해제 후 [30초] 이내에 도어를 여는 동작이 수반되지 않으면 도어를 열 의향이 없다고 간주
		// 도어 잠금 해제를 취소하고 도어 잠금 상태로 변경
	case 1000:
		nRet = Func.OnWatchDoor();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
		if (nRet > 0)
		{
			// 도어가 하나 이상 열렸으므로 정상적으로 모든 도어 닫을 때까지 기다림
			m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
			m_nLockStep = 0;
		}
		else
		{
			nSensor = g_ioMgr.Get_Out_Bit(st_io.o_door_lock, IO_OFF);
			// 도어 잠금 해제 상태 [Release 상태]
			if (nSensor == IO_OFF)
			{
				m_lLockWaitTime[1] = GetCurrentTime();
				m_lLockWaitTime[2] = m_lLockWaitTime[1] - m_lLockWaitTime[0];
				if (m_lLockWaitTime[2] < 0)
				{
					m_lLockWaitTime[0] = GetCurrentTime();
					break;
				}
				
				if (m_lLockWaitTime[2] > (600*10))
				{
					m_nLockStep = 100;
				}
			}
			else
			{
				m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
				m_nLockStep = 0;
			}
		}
		break;
	}
}

void CRun_Handler_Check::OnTurn_Front_Auto() 
{
	int nReadSEN = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk);
	
	switch(m_nStep_Front_Auto)
	{
	case 0:
		if (nReadSEN == IO_ON && st_handler.nModeManual == CTL_YES)
		{
			m_lWait_Auto[0] = GetCurrentTime();
			m_nStep_Front_Auto = 100;
		}
		break;
		
	case 100:
		m_lWait_Auto[1] = GetCurrentTime();
		m_lWait_Auto[2] = m_lWait_Auto[1] - m_lWait_Auto[0];
		if (m_lWait_Auto[2] < 0)
		{
			m_lWait_Auto[0] = GetCurrentTime();
			break;
		}
		
		// [50ms] 동안 유지되는 경우에만 처리 [시그널 튀는 경우 방지]
		if (m_lWait_Auto[2] > 50 && nReadSEN == IO_ON)
		{
			m_nStep_Front_Auto = 600;
		}
		else if (nReadSEN == IO_OFF)
		{
			m_nStep_Front_Auto = 0;
		}
		break;
		
	case 600:
		st_handler.nModeManual = CTL_NO;
		// 와치독 대화상자 화면 출력 요청
		//::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 40, DLG_SHOW_);
		m_nStep_Front_Auto = 0;
		break;
	}
}

//==================================================================//
// [Manual] 상태로 스위치 돌린 경우 처리
//==================================================================//
void CRun_Handler_Check::OnTurn_Front_Manual() 
{
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	int nReadSEN = g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk);
	
	switch(m_nStep_Front_Manual)
	{
	case 0:
		if (nReadSEN == IO_OFF && st_handler.nModeManual == CTL_NO)
		{
			m_lWait_Manual[0] = GetCurrentTime();
			m_nStep_Front_Manual = 100;
		}
		break;
		
	case 100:
		m_lWait_Manual[1] = GetCurrentTime();
		m_lWait_Manual[2] = m_lWait_Manual[1] - m_lWait_Manual[0];
		if (m_lWait_Manual[2] < 0)
		{
			m_lWait_Manual[0] = GetCurrentTime();
			break;
		}
		
		// [50ms] 동안 유지되는 경우에만 처리 [시그널 튀는 경우 방지]
		if (m_lWait_Manual[2] > 50 && nReadSEN == IO_OFF)
		{
			m_nStep_Front_Manual = 600;
		}
		else if (nReadSEN == IO_ON)
		{
			m_nStep_Front_Manual = 0;
		}
		break;
		
	case 600:
		st_handler.nModeManual = CTL_YES;
		Func.OnSet_IO_Port_Stop();
		Func.OnOpenDoor();	// 도어 열기
		m_nStep_Front_Manual = 0;
		break;
	}
}

void CRun_Handler_Check::OnTurn_Rear_Auto() 
{
	int nReadSEN = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk);
	
	switch(m_nStep_Rear_Auto)
	{
	case 0:
		if (nReadSEN == IO_ON && st_handler.nModeManual == CTL_YES)
		{
			m_lWait_Auto[0] = GetCurrentTime();
			m_nStep_Rear_Auto = 100;
		}
		break;
		
	case 100:
		m_lWait_Auto[1] = GetCurrentTime();
		m_lWait_Auto[2] = m_lWait_Auto[1] - m_lWait_Auto[0];
		if (m_lWait_Auto[2] < 0)
		{
			m_lWait_Auto[0] = GetCurrentTime();
			break;
		}
		
		// [50ms] 동안 유지되는 경우에만 처리 [시그널 튀는 경우 방지]
		if (m_lWait_Auto[2] > 50 && nReadSEN == IO_ON)
		{
			m_nStep_Rear_Auto = 600;
		}
		else if (nReadSEN == IO_OFF)
		{
			m_nStep_Rear_Auto = 0;
		}
		break;
		
	case 600:
		st_handler.nModeManual = CTL_NO;
		// 와치독 대화상자 화면 출력 요청
		//::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 40, DLG_SHOW_);
		m_nStep_Rear_Auto = 0;
		break;
	}
}

//==================================================================//
// [Manual] 상태로 스위치 돌린 경우 처리
//==================================================================//
void CRun_Handler_Check::OnTurn_Rear_Manual() 
{
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	int nReadSEN = g_ioMgr.get_in_bit(st_io.i_rear_select_switch_chk);
	
	switch(m_nStep_Rear_Manual)
	{
	case 0:
		if (nReadSEN == IO_OFF && st_handler.nModeManual == CTL_NO)
		{
			m_lWait_Manual[0] = GetCurrentTime();
			m_nStep_Rear_Manual = 100;
		}
		break;
		
	case 100:
		m_lWait_Manual[1] = GetCurrentTime();
		m_lWait_Manual[2] = m_lWait_Manual[1] - m_lWait_Manual[0];
		if (m_lWait_Manual[2] < 0)
		{
			m_lWait_Manual[0] = GetCurrentTime();
			break;
		}
		
		// [50ms] 동안 유지되는 경우에만 처리 [시그널 튀는 경우 방지]
		if (m_lWait_Manual[2] > 50 && nReadSEN == IO_OFF)
		{
			m_nStep_Rear_Manual = 600;
		}
		else if (nReadSEN == IO_ON)
		{
			m_nStep_Rear_Manual = 0;
		}
		break;
		
	case 600:
		st_handler.nModeManual = CTL_YES;
		Func.OnSet_IO_Port_Stop();
		Func.OnOpenDoor();	// 도어 열기
		m_nStep_Rear_Manual = 0;
		break;
	}
}
////