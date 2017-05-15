// Run_Handler_Check.cpp: implementation of the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Handler_Check.h"

#include "Variable.h"				// ���� ���� ���� Class
#include "Public_Function.h"		// Motor �� Cylinder ���� Class
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
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return; // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// ��� â�� �� ������ �������� �ʴ´�.

	switch (StartStep)
	{
	case 0:
		// **********************************************************************
		// RUN Ű I/O �Է��� On �����̰� ���°� STOP ���¿����� �˻�             
		// -> �ٽ� ����, STOP ���¿����� RUN Ű �Է��� �����Ѵ�                  
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_start_switch_chk) == TRUE && COMI.mn_run_status == dSTOP)
		{
			// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
			if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE)
			{
				break;
			}

			StartStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START Ű�� ���������� On/Off �Ǵ� ���� �����ϱ� ���Ͽ� ��� ��        
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
		// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
		// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
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
		//  RUN�Ҽ� �ִ� �������� �˻��Ѵ�. �̻��� ����� RUN�� �ȴ�.            
		// **********************************************************************

		alarm.mb_bcr_error = false;

		if (st_handler.mn_motor_init_check != CTL_YES)	// ���� �ʱ�ȭ�� ���� �ʾҴٸ� �������� �ʴ´�. 2K4/11/23/ViboX
		{
			Func.OnSet_IO_Port_Sound(IO_ON);
//			st_msg.mstr_abnormal_msg = _T("[MOTOR INIT CHECK] Motor Initial..., become Run.");
			sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial..., become Run.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			alarm.mstr_code = _T("900001");		// Alarm ȭ���� ����ش�. 2K4/11/21/ViboX
			sprintf(alarm.mc_code, "900001");
			alarm.mn_count_mode = 0;				// �˶� ī��Ʈ ���� �÷��� ���� (�˶� ī��Ʈ �۾� ������)
			alarm.mn_type_mode = eWARNING;		// ���� �߻��� �˶� ���� �÷��� ����
			COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm ȭ���� ����ش�. 2K4/11/21/ViboX
			StartStep = 0;
//			g_handler.AddAlarmCnt();
			return;
		}

// 		// JAM�� ���� �Ǿ����� Ȯ���ϰ� RUN�� ��Ų��.
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
// 			COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm ȭ���� ����ش�. 2K4/11/21/ViboX
			StartStep = 0;
			break;
		}
	
		// Air ���� ������ Ȯ���Ѵ�.
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
// 			st_handler.mstrSelectMessage = "���� RESET ��ư�� ���� �ּ���.";
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
		
		if (st_basic.mn_mode_run == CTL_NO)		// OFFLINE�̸� ���¸� �˸���.
		{
			st_handler.mstrSelectMessage = "OFFLINE �����Դϴ�. RUN �Ͻðڽ��ϱ�?";
			
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

// 		Ret = Func.Robot_Reinstatement_Position(1);		// ���� ���� Ȯ�� �ؾ� ��.
// 
// 		if (Ret == RET_GOOD)
// 		{	//����Ϸ�
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
			if(COMI.mn_run_status != dREINSTATE)	//���� ������ ������, ���е� �ƴѵ� ���͵��� ���°� �ƴ϶��..
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
		// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
		// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
		// **********************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// ���� ���������� �޽��� �����Ͽ� �˶� ȭ�� ���� ��û 
	
		if (st_handler.mn_menu_num != 101)
		{
			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// ���� ���������� �޽��� �����Ͽ� ���� ȭ������ ��ȯ 
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
					COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm ȭ���� ����ش�. 2K4/11/21/ViboX
					StartStep = 0;
				}
			}
		}
		break;

	case 1000:
		Func.OnSet_IO_Port_Run();									// ��� ���� : ���� ������ ��� I/O ��� �������� �Լ�
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
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	switch (StopStep)
	{
	case 0:
		// **********************************************************************
		// STOP Ű I/O �Է��� On �����̰� ���� STOP ���°� �ƴ��� �˻�           
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE && COMI.mn_run_status != dSTOP)
		{
			// ���� Start Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
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
		// START Ű�� ���������� On/Off �Ǵ� ���� �����ϱ� ���Ͽ� ��� ��        
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
		// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
		// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
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
		// STOP Ű I/O �Է��� Off �Ǿ����� �˻�                                  
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			StopStep = 0; 

			if (COMI.mn_run_status == dINIT)				// �ʱ�ȭ �߿� Stop ��ư�� ������... 2K5/02/23/ViboX
			{
				st_handler.mn_initial_error = TRUE;
				
				alarm.mstr_code = _T("900001");		// Alarm ȭ���� ����ش�. 2K5/03/10/ViboX
				sprintf(alarm.mc_code, "900001");
				alarm.mn_count_mode = 0;				// �˶� ī��Ʈ ���� �÷��� ���� (�˶� ī��Ʈ �۾� ������)
				alarm.mn_type_mode = eWARNING;		// ���� �߻��� �˶� ���� �÷��� ����
				COMI.mn_run_status = dWARNING;	// dJAM,dWARNING 

//				g_handler.AddAlarmCnt();
				break;
			}

// 			if (st_handler.mn_menu_num != 101)
// 			{
// 				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // ���� ���������� �޽��� �����Ͽ� ���� ȭ������ ��ȯ 
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
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************
	
	switch(BuzzerStep)
	{
	case 0:
		// **********************************************************************
		//  Reset Ű I/O �Է��� On �������� Ȯ��                                 
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_buzzer_switch_chk) == TRUE)
		{
			BuzzerStep = 10;
		}
		break;
		
	case 10:
		// **********************************************************************
		//  Reset Ű�� ���������� On/Off �Ǵ� ���� �����ϱ� ���Ͽ� ��� ��       
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
		// Sound Ű I/O �Է��� Off �Ǿ����� �˻�                                 
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
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************
	
	switch(AlaemClearStep)
	{
	case 0:
		// **********************************************************************
		//  Reset Ű I/O �Է��� On �������� Ȯ��                                 
		// **********************************************************************
		if (g_ioMgr.Get_In_Bit(st_io.i_alarm_clear_switch_chk) == TRUE)
		{
			AlaemClearStep = 10;
		}
		break;
		
	case 10:
		// **********************************************************************
		//  Reset Ű�� ���������� On/Off �Ǵ� ���� �����ϱ� ���Ͽ� ��� ��       
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
		// Reset Ű I/O �Է��� Off �Ǿ����� �˻�                                 
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
		g_ioMgr.set_out_bit(st_io.o_alarm_clear_lamp, IO_OFF);	// Reset Switch�� Lamp�� Off ��Ų��.
		
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
		OnTurn_Front_Auto();		// [Auto] ��ioAA����I ��o����A����A�ˢ� ��i��o�ˡ����� �����Ϣ���?i A�ϩ��ˡ���c
		OnTurn_Front_Manual();	// [Manual] ��ioAA����I ��o����A����A�ˢ� ��i��o�ˡ����� �����Ϣ���?i A�ϩ��ˡ���c
	}

	if(g_ioMgr.get_in_bit(st_io.i_front_select_switch_chk, IO_ON) == IO_ON)
	{
		OnTurn_Rear_Auto();		// [Auto] ��E��cioAA��E��c��E�ˡ�I �ˢ碮��o�ˢ碮���ˢ�ϡ�A��E��c�ˢ��?A�ˢ�E��Ec �ˢ�Ii����Io�ˢ�E�ˢ�ϡ���E��c���סˢ� ��E��c���סˢ碮��I��E��c�ˢ�E?i A����I��I���̡ˢ�E�ˢ�ϡ��ˢ�Ec
		OnTurn_Rear_Manual();	// [Manual] ��E��cioAA��E��c��E�ˡ�I �ˢ碮��o�ˢ碮���ˢ�ϡ�A��E��c�ˢ��?A�ˢ�E��Ec �ˢ�Ii����Io�ˢ�E�ˢ�ϡ���E��c���סˢ� ��E��c���סˢ碮��I��E��c�ˢ�E?i A����I��I���̡ˢ�E�ˢ�ϡ��ˢ�Ec
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
// 			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// �ˡ�����AI CA������oAOA�ˡ�����I �ˡ�������oAAo Au��uUCI��?��I �ˡ�����AI E���ϡˡ�eA�ˡ�����I AuE? 
// 		}
// 	}
}

void CRun_Handler_Check::ButtonCheck_LabelLockUnLock(int i)
{
	if (st_handler.mn_system_lock != FALSE)  return; // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// ��� â�� �� ������ �������� �ʴ´�.

// 	if(COMI.mn_run_status == dRUN) return;//2014.1018

	//2015.0312
	if( COMI.mn_run_status == dRUN &&
		(st_basic.mn_device_type == i || (st_basic.mn_device_type+3) == i ) ) return;

	switch (LabelLockStep[i])
	{
	case 0:
// 		if (g_ioMgr.get_in_bit(st_io.i_density_lock_switch_chk[i], IO_OFF) == IO_ON && COMI.mn_run_status == dSTOP)
// 		{
// 			// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
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
				// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
				if (g_ioMgr.get_in_bit(st_io.i_start_switch_chk, IO_OFF) == TRUE)
				{
					break;
				}
				st_var.n_err_density_flag[i] = CTL_NO;//density�����̰ų� ready or complete�� ������ ����
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
				// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
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
//	if (st_handler.mn_system_lock != FALSE)  return; // ���� �ý��� Lock ������ ��� ���� 
//	// **************************************************************************
//
//	if (st_handler.mn_switch_lock != FALSE)	return;	// ��� â�� �� ������ �������� �ʴ´�.
//
//	if(COMI.mn_run_status == dRUN) return;//2014.1018
//
//	switch (LabelPaperLockStep[i])
//	{
//	case 0:
////		if (g_ioMgr.get_in_bit(st_io.i_density_paper_pusher_switch_chk[i], IO_OFF) == IO_ON && COMI.mn_run_status == dSTOP)
//// 		{
//// 			// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
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
	if (st_handler.mn_system_lock != FALSE)  return; // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// ��� â�� �� ������ �������� �ʴ´�.

// 	if(COMI.mn_run_status == dRUN) return;//2014.1018
	if(COMI.mn_run_status == dRUN && st_basic.nBcrFeederUse[i] == CTL_YES) return;

	switch (BcrLockStep[i])
	{
	case 0:
// 		if (g_ioMgr.get_in_bit(st_io.i_distribute_lockunlock_switch_chk[i], IO_OFF) == IO_ON && COMI.mn_run_status == dSTOP)
// 		{
// 			// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
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
				// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
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
//	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
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
    // ������ ���� ���� ���� �˻��Ѵ�                                            
	// -> st_handler.n_system_lock : SYSTEM LOCK ���� �÷���                     
	// -> st_work.n_run_status : ��� ���� �÷���                                
    // =============================================================================

//	if( ( g_ioMgr.get_in_bit(st_io.i_emo_switch1_chk) == IO_OFF || g_ioMgr.get_in_bit(st_io.i_emo_switch2_chk) == IO_OFF ) && alarm.mn_emo_alarm != TRUE)
//	{
//		if(i_emo_check_flag == CTL_NO)
//		{
//			i_emo_check_flag = CTL_YES;			// ù��° EMO�� �����ƴ�.
//			EMO_CheckTime[0] = GetCurrentTime();
//		}
//		else
//		{
//			EMO_CheckTime[1] = GetCurrentTime();
//
//			if(EMO_CheckTime[1] < EMO_CheckTime[0])		// ���� �Ȱ�ġ�� �°Ŵ�. �ٽ� �ö󰡴���.
//			{
//				i_emo_check_flag = CTL_NO;
//			}
//			else
//			{
//				EMO_CheckTime[2] = EMO_CheckTime[1] - EMO_CheckTime[0];
//
//				if(EMO_CheckTime[2] > 300)		// 0.5�ʵ��� �����Ǹ�..
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
//					alarm.mstr_code = _T("900200");		// �߻��� �˶� �ڵ� ����
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
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
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
// ��� DOOR ���� ��� ������ DOOR LOCK ó��
//==================================================================//
void CRun_Handler_Check::OnRun_DoorLock() 
{
	int nRet = -1;
	int nSensor = -1;

	switch(m_nLockStep)
	{
	case 0:
		// (IDW_SCREEN_IO) ȭ���� ������� ��쿡�� ���� ����
		// - ������ ��� ������Ҵµ� �ٽ� ������ �ȵǱ� ������
		if (st_handler.mn_menu_num == 701)
		{
			break;
		}
		// ���� ���°� �Ŵ��� ��尡 �ƴϸ� [���� ����� ���]
		// - �Ŵ��� ��忡���� ���� ���� ���� �˻�
		// - ���� ��忡���� ��� ������ �̹� ���� ������ �������� �˶��� �߻��� ��Ȳ�ϰ���
		if (st_handler.nModeManual != CTL_YES)
		{
			break;
		}

		nRet = Func.OnWatchDoor();	// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
		if (nRet > 0)
		{
			m_nDoorOpenTry = TRUE;	// ���� ���� ������ ������ ������
		}
		else
		{
			// �Ŵ��� ���� ���ѵǾ� ���� ��� ���¸� �����ߴµ� ��� ���� �ʰ� ����
			// - ���� �ð� ����Ŀ� ��� ������ ��� ���·� ������
			if (m_nDoorOpenTry != TRUE)
			{
				// DOOR �������� ����
				// ������ ����� ����ġ�� ������ ��� [����� ����]
				nSensor = g_ioMgr.Get_Out_Bit(st_io.o_door_lock, IO_OFF);
				// ���� ��� ���� ���� [Release ����]
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
			// ��� [����->����] ���·� ����� ���
			else
			{
				m_nLockStep = 100;
			}
		}
		break;

	case 100:
		// ���� ��� ���� Ȯ��
		// - ���� ����� �����Ǿ� ������ ���� ��� ��ȣ ��� [��� ���� ���]
		nSensor = g_ioMgr.Get_Out_Bit(st_io.o_door_lock, IO_OFF);
		// ���� ��� ������
		if (nSensor == IO_OFF)
		{
			// ��� ��� �������Ƿ� ���� ����� ������
			g_ioMgr.set_out_bit(st_io.o_door_lock, IO_ON);

			// ���� ��� ��ȣ ��� �� ��ٷ� ��� ���� ���� �ƴϱ� ������ ���� �ð� ����Ŀ� ���� �Ϸ��Ŵ
			m_lLockWaitTime[0] = GetCurrentTime();
			m_nLockStep = 200;
		}
		// ���� ����� ������ ��� [���� �������� �ٽ� �˻� �����]
		else
		{
			m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
			m_nLockStep = 0;
		}
		break;

	case 200:
		// ��� ������ ��涧���� ���� �ð� ��ٸ� [0.5�� ���]
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

			m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
			m_nLockStep = 0;
		}
		break;

		// ���� ��� ���� �� [30��] �̳��� ��� ���� ������ ���ݵ��� ������ ��� �� ������ ���ٰ� ����
		// ���� ��� ������ ����ϰ� ���� ��� ���·� ����
	case 1000:
		nRet = Func.OnWatchDoor();	// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
		if (nRet > 0)
		{
			// ��� �ϳ� �̻� �������Ƿ� ���������� ��� ���� ���� ������ ��ٸ�
			m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
			m_nLockStep = 0;
		}
		else
		{
			nSensor = g_ioMgr.Get_Out_Bit(st_io.o_door_lock, IO_OFF);
			// ���� ��� ���� ���� [Release ����]
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
				m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
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
		
		// [50ms] ���� �����Ǵ� ��쿡�� ó�� [�ñ׳� Ƣ�� ��� ����]
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
		// ��ġ�� ��ȭ���� ȭ�� ��� ��û
		//::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 40, DLG_SHOW_);
		m_nStep_Front_Auto = 0;
		break;
	}
}

//==================================================================//
// [Manual] ���·� ����ġ ���� ��� ó��
//==================================================================//
void CRun_Handler_Check::OnTurn_Front_Manual() 
{
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
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
		
		// [50ms] ���� �����Ǵ� ��쿡�� ó�� [�ñ׳� Ƣ�� ��� ����]
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
		Func.OnOpenDoor();	// ���� ����
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
		
		// [50ms] ���� �����Ǵ� ��쿡�� ó�� [�ñ׳� Ƣ�� ��� ����]
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
		// ��ġ�� ��ȭ���� ȭ�� ��� ��û
		//::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 40, DLG_SHOW_);
		m_nStep_Rear_Auto = 0;
		break;
	}
}

//==================================================================//
// [Manual] ���·� ����ġ ���� ��� ó��
//==================================================================//
void CRun_Handler_Check::OnTurn_Rear_Manual() 
{
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
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
		
		// [50ms] ���� �����Ǵ� ��쿡�� ó�� [�ñ׳� Ƣ�� ��� ����]
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
		Func.OnOpenDoor();	// ���� ����
		m_nStep_Rear_Manual = 0;
		break;
	}
}
////