// APartHandler.cpp: implementation of the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartHandler.h"
#include "../IO_Manager.h"
#include "../ComizoaPublic.h"
#include "APartTray.h"
#include "APartRobot.h"
#include "APartTestSite.h"
#include "../AMTLotManager.h"
#include "../AMTRegistry.h"

#include "../SrcBase\ALocalization.h"
#include "../Run_Top_IDBuffer_Works.h"
#include "../Run_Btm_IDBuffer_Works.h"
#include "../Run_Barcode_Feeders.h"

#include "../FrontServer.h"
#include "../BtmClient.h"

#include "../Public_Function.h"
#include "../Run_Smema_Ctl.h"
#include "../Run_IDBuffer_El.h"
#include "../MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartHandler	g_handler;

APartHandler::APartHandler()
{
	m_empos = EMPOS_FRONT;

	
	m_step_rear_smema = 0;
	st_handler.mn_smema_rear = CTL_NO;

	m_step_ttrans = ETT_TO_NONE;

	m_epriority_bin = TPOS_NONE;
	m_esecondary_bin = TPOS_NONE;

	m_step_virtual_supply = EVS_NONE;

	m_nVedio = -1;

	m_bRejAlarm = false;

	m_bReady_SendNext = false;

	m_bRecv_NextMachine = true;

	m_nAlarmCnt = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM );

	st_handler.mn_smema_stop = 0;
	st_handler.mn_smema_load = CTL_NO;

	m_step_rear_smema = 0;
	m_step_conv_smema = 0;

	st_sync.n_unload_sub_elev_req = CTL_NO;
	m_nReryCnt = 0;


}

APartHandler::~APartHandler()
{

}

void APartHandler::Thread_Run()
{
	st_basic.mn_network_wait_time = 5000;
	Run_Move_SMEMA();

	Run_Move_Main();


	for ( int i = 0;i < 4; i++ )
	{
		Run_Move_Conv(i);
	}

}

void APartHandler::Run_Move_SMEMA()
{
	BOOL bRequest = IO_ON;

//	Run_Move_front_Smema();
	Run_Move_rear_Smema();
	Run_Move_btm_rear_Smema();
	Run_Move_Conv();

}

void APartHandler::Run_Move_Conv()
{

	int nRet = 0;
	CString mstr_msg;
	Func.ThreadFunctionStepTrace(71, m_step_conv_smema);
	switch( m_step_conv_smema )
	{
	case 0:
// 		if( COMI.mn_run_status != dRUN ) return;
//		if(st_handler.mn_OutElv_work == CTL_YES)
//		{
		if( st_handler.mn_chk_lotend == CTL_YES && g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" && st_basic.mn_rework_ccs_mode == CTL_YES )
		{
			m_dwTime_LotendSmema = GetCurrentTime();
			m_step_conv_smema = 5;
		}
		else
		{
			if( st_handler.mn_OutElv_work == CTL_NO &&
				g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
			{
				st_handler.mn_OutElv_work = CTL_YES;
// 				FAS_IO.Set_IO_LatchBit(st_io.i_uld_cv_box_out_chk, TRUE);
				FAS_IO.Set_IO_LatchBit(st_io.i_uld_cv_box_full_chk, TRUE);
				m_step_conv_smema = 10;
			}
			else if( st_handler.mn_OutElv_work == CTL_YES )
			{
				FAS_IO.Set_IO_LatchBit(st_io.i_uld_cv_box_full_chk, TRUE);
				m_step_conv_smema = 10;
			}
		}
//		}
		break;

	case 5:
		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
		if( g_lotMgr.GetLotAt(0).GetReworkCnt(PRIME) > st_handler.mn_reworkcnt )
		{
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk, IO_ON ) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_ON ) == IO_ON )
			{
				st_handler.mn_reworkcnt++;
				m_step_conv_smema = 10;
			}
		}
		else
		{
			if( GetCurrentTime() - m_dwTime_LotendSmema > MES_TIME_OUT )
			{
				m_dwTime_LotendSmema = GetCurrentTime();
				st_msg.mstr_event_msg[0] = "[REWORK CCS] Please run rework ccs or You must cancle rework ccs ";
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				m_step_conv_smema = 0;
				
			}
			else if( st_basic.mn_rework_ccs_mode == CTL_NO )
			{
				m_step_conv_smema = 0;
			}
		}
		break;

	case 8:
		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
		if( g_lotMgr.GetLotAt(0).GetReworkCnt(PRIME) > st_handler.mn_reworkcnt )
		{
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			m_step_conv_smema = 5;
		}
		else if( st_basic.mn_rework_ccs_mode == CTL_NO )
		{
			m_step_conv_smema = 0;
		}
		break;

	case 10:
		if( st_basic.mn_outconv == CTL_YES )
		{
			//g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
			m_dwTime_LotendSmema = GetCurrentTime();
			m_step_conv_smema = 20;
		}
		else
		{
			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
			m_dwTime_LotendSmema = GetCurrentTime();
			m_step_conv_smema = 300;
		}
		break;

	case 20:
// 		nRet = FAS_IO.Get_IO_LatchBit(st_io.i_uld_cv_box_out_chk);
		nRet = FAS_IO.Get_IO_LatchBit(st_io.i_uld_cv_box_full_chk);
		if (nRet == FALSE || g_ioMgr.get_in_bit(st_io.i_uld_cv_box_full_chk) == IO_ON)
		{
			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 			if (g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON)
			if (g_ioMgr.get_in_bit(st_io.i_uld_cv_box_full_chk) == IO_ON)
			{
				m_step_conv_smema = 30;
			}
			m_step_conv_smema = 30;
		}
		else
		{
			if( g_ioMgr.get_out_bit(st_io.o_uld_cv_motor_onoff, IO_ON) != IO_ON)
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTime_LotendSmema > MES_TIME_OUT)
			{
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_OFF)
				{
					st_handler.mn_OutElv_work = CTL_NO;
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
					m_step_conv_smema = 0;
				}
				else
				{
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
					m_step_conv_smema = 21;
				}

			}
		}
		break;

	case 21:
		m_dwTime_LotendSmema = GetCurrentTime();
		m_step_conv_smema = 22;
		break;

	case 22:
		nRet = FAS_IO.Get_IO_LatchBit(st_io.i_uld_cv_box_full_chk);
		if (nRet == FALSE || g_ioMgr.get_in_bit(st_io.i_uld_cv_box_full_chk) == IO_ON)
		{
			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
			// 			if (g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON)
			if (g_ioMgr.get_in_bit(st_io.i_uld_cv_box_full_chk) == IO_ON)
			{
				m_step_conv_smema = 30;
			}
			m_step_conv_smema = 30;
		}
		else
		{
			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTime_LotendSmema > MES_TIME_OUT)
			{
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_OFF &&
					g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_OFF)
				{
					st_handler.mn_OutElv_work = CTL_NO;
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
					m_step_conv_smema = 0;
				}
				else
				{//524160 1 00 "PS2407_PS2408_PS2409_PS2410_Conveyor_Box_Check_Error."
					st_handler.mn_OutElv_work = CTL_NO;
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
					m_step_conv_smema = 23;
					CTL_Lib.Alarm_Error_Occurrence( 240, CTL_dWARNING, "524160");
				}
				
			}
		}
		break;

	case 23:
		if( COMI.mn_run_status != dRUN ) return;
		m_step_conv_smema = 0;
		break;		

	case 30:
		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON )
		{
			mstr_msg = "Rear req sgn on";
			Func.On_LogFile_Add(1, mstr_msg);
			m_step_conv_smema = 40;
		}
		else
		{

			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
			else if( GetCurrentTime() - m_dwTime_LotendSmema > (MES_TIME_OUT*3) )
			{
				m_dwTime_LotendSmema = GetCurrentTime();
				st_msg.mstr_event_msg[0] = "[NEXT MACHINE] is not response";
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
// 				st_var.n_next_down = CTL_YES;
				m_step_conv_smema = 31;
			}
			if( st_basic.mn_outconv == CTL_NO )
			{
				m_step_conv_smema = 0;
			}

		}
		break;

	case 31:
		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON )
		{
			st_var.n_next_down = CTL_NO;
			mstr_msg = "Rear req sgn on";
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			Func.On_LogFile_Add(1, mstr_msg);
			m_step_conv_smema = 40;
		}
		else if( st_basic.mn_outconv == CTL_NO )
		{
			st_var.n_next_down = CTL_NO;
// 			if( ((CMainFrame*)AfxGetMainWnd())->mp_msg_dlg != NULL )
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			m_step_conv_smema = 0;
		}
// 		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 		else if( GetCurrentTime() - m_dwTime_LotendSmema > (MES_TIME_OUT*3) )
// 		{
// 			m_step_conv_smema = 32;
// 		}
		break;

	case 32:
		st_msg.mstr_event_msg[0] = "[NEXT MACHINE] is not response";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		// 				st_var.n_next_down = CTL_YES;
		m_step_conv_smema = 31;
		break;

	case 40:
		g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_ON);
		mstr_msg = "Front ready sgn on";
		Func.On_LogFile_Add(1, mstr_msg);

		m_dwTime_LotendSmema = GetCurrentTime();
		m_step_conv_smema = 50;
		break;

	case 50:
		if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
		{
			m_dwTime_LotendSmema = GetCurrentTime();
// 			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			mstr_msg = "Rear transfer req sgn on";
			Func.On_LogFile_Add(1, mstr_msg);
			st_var.n_next_down = CTL_NO;
			m_step_conv_smema = 60;
		}
		else
		{
			if(GetCurrentTime() - m_dwTime_LotendSmema > (MES_TIME_OUT*3))
			{
// 				st_var.n_next_down = CTL_YES;
				st_msg.mstr_event_msg[0] = "[NEXT MACHINE] is not response.";
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 51;
				if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF )
				{
					st_var.n_next_down = CTL_NO;
					g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
					m_step_conv_smema = 0;
				}
			}
			else if(GetCurrentTime() - m_dwTime_LotendSmema <= 0)
			{
				m_dwTime_LotendSmema = GetCurrentTime();
			}
			
		}
		break;

	case 51:
		if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
		{
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			mstr_msg = "Rear transfer req sgn on";
			Func.On_LogFile_Add(1, mstr_msg);
			st_var.n_next_down = CTL_NO;
			m_step_conv_smema = 60; 
		}
		else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF )
		{
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			st_var.n_next_down = CTL_NO;
			m_step_conv_smema = 0;
		}
		else if( st_basic.mn_outconv == CTL_NO )
		{
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			st_var.n_next_down = CTL_NO;
			m_step_conv_smema = 0;
		}
		break;

	case 60:
		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_LotendSmema > 100)
		{
			if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
			{
				st_handler.mn_receive = 0;
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
					st_handler.mn_receive++;
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
					st_handler.mn_receive++;
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON )
					st_handler.mn_receive++;
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
					st_handler.mn_receive++;
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
					st_handler.mn_receive--;
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON)
					st_handler.mn_receive--;

// 				FAS_IO.Set_IO_LatchBit(st_io.i_uld_cv_box_out_chk, TRUE);
				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_ON);
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);

				mstr_msg = "Front enter sgn on";
				Func.On_LogFile_Add(1, mstr_msg);

				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 100;
				if(st_handler.mn_receive == 1 && g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON)
				{
					m_step_conv_smema = 70;
				}
			}
// 			else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// 				g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// 			{
// 				m_step_conv_smema = 0;
// 			}
		}
		break;

	case 70:
		{
			//2015.0523
			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
			if (/*nRet == FALSE || */g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON)
			{
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 80;
			}
			else if(GetCurrentTime() - m_dwTime_LotendSmema > 5000)
			{
				st_msg.mstr_event_msg[0] = "[CONVEYOR] Conveyor is blocked.";
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 71;
			}
		}
		break;

	case 71:
		if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
		if (g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON)
		{
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			m_step_conv_smema = 72;
		}
		else if(GetCurrentTime() - m_dwTime_LotendSmema > 10000)
		{
			m_dwTime_LotendSmema = GetCurrentTime();
			m_step_conv_smema = 70;
		}
		break;

	case 80:
		{
			if(g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_OFF)
			{
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 110;
			}
			else
			{
				if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
				if(GetCurrentTime() - m_dwTime_LotendSmema < 700) break;
				
				FAS_IO.Set_IO_LatchBit(st_io.i_uld_cv_box_out_chk, TRUE);
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
				m_step_conv_smema = 70;
			}
		}
		break;

	case 100:
		{
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 850) break;
// 
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 			m_step_conv_smema = 110;

// 			nRet = FAS_IO.Get_IO_LatchBit(st_io.i_uld_cv_box_out_chk);
			if (/*nRet == FALSE || */g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON)
			{
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 105;
			}
		}
		break;

	case 105:
		{
			if(g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_OFF)
			{
				m_step_conv_smema = 110;
				m_dwTime_LotendSmema = GetCurrentTime();
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_step_conv_smema = 106;
			}
			else
			{
				if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
				if(GetCurrentTime() - m_dwTime_LotendSmema < 700) break;
				
				m_dwTime_LotendSmema = GetCurrentTime();
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_step_conv_smema = 106;
			}
		}
		break;

	case 106:
		if(g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_OFF)
			m_step_conv_smema = 110;
		else
		{
			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
			if(GetCurrentTime() - m_dwTime_LotendSmema < 1000) break;
			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
			m_step_conv_smema = 100;
		}
		break;

	case 110:
		{
			int num = 0;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
				num++;

			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
				num--;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON && g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON)
				num--;


			if(st_handler.mn_receive >= 1/* && st_var.n_tbox_output_count >= 2*/)
			{
				if( num <= 1 )
				{
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
					g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
					g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);

					mstr_msg = "Front ready sgn off, Front enter sgn off_1";
					Func.On_LogFile_Add(1, mstr_msg);

					st_var.n_tbox_output_count--;
					m_dwTime_LotendSmema = GetCurrentTime();
					m_step_conv_smema = 200;
				}
				else
				{
					if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
					if(GetCurrentTime() - m_dwTime_LotendSmema < 3000) break;

					FAS_IO.Set_IO_LatchBit(st_io.i_uld_cv_box_out_chk, TRUE);
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
					m_dwTime_LotendSmema = GetCurrentTime();
					m_step_conv_smema = 120;
				}

			}
			else
			{
				if( num <= 0 /*&& st_var.n_tbox_output_count <= 1*/)
				{
					g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
					g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
					g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
					st_var.n_tbox_output_count--;
					m_dwTime_LotendSmema = GetCurrentTime();
					m_step_conv_smema = 200;
					mstr_msg = "Front ready sgn off, Front enter sgn off_2";
					Func.On_LogFile_Add(1, mstr_msg);
				}
			}


		}
		break;

	case 120:
		{
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 700) break;
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 			m_step_conv_smema = 110;
// 			nRet = FAS_IO.Get_IO_LatchBit(st_io.i_uld_cv_box_out_chk);
			if (/*nRet == FALSE || */g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_ON)
			{
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 130;
			}
		}
		break;

	case 130:
		{
			if(g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_OFF)
			{
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 140;
			}
			else
			{
				if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
				if(GetCurrentTime() - m_dwTime_LotendSmema < 700) break;
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 140;
			}
		}
		break;

	case 140:
		if(g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_OFF)
			m_step_conv_smema = 110;
		else
		{
			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
			if(GetCurrentTime() - m_dwTime_LotendSmema < 1000) break;
			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
			m_step_conv_smema = 120;
		}
		break;

	case 200:
		if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
		if(GetCurrentTime() - m_dwTime_LotendSmema < 1000) break;
		if( /*g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_OFF) == IO_OFF &&*/
			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_OFF )
		{

			mstr_msg = "Rear transfer sgn off__sgn all reset";
			Func.On_LogFile_Add(1, mstr_msg);
			
			m_step_conv_smema = 210;
			m_dwTime_LotendSmema = GetCurrentTime();
// 			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
		}
		else
		{
			if(GetCurrentTime() - m_dwTime_LotendSmema > (MES_TIME_OUT * 3))
			{
// 				st_msg.mstr_event_msg[0] = "[NEXT MACHINE] is not response.";
// 				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				m_step_conv_smema = 201;
			}

		}
		if( st_basic.mn_outconv == CTL_NO )
		{
			st_basic.mn_outconv = CTL_NO;
			m_step_conv_smema = 0;
		}
		break;

	case 201:
		m_dwTime_LotendSmema = GetCurrentTime();
		m_step_conv_smema = 200;
		break;

	case 210:
		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_OFF &&
			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_OFF && 
			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk) == IO_OFF)
		{
			//2016.1215
			if( st_handler.mn_chk_lotend == CTL_READY )
			{
				st_handler.mn_chk_lotend = CTL_YES;
			}
			st_handler.mn_OutElv_work = CTL_NO;
		}
		m_step_conv_smema = 0;

		break;

	case 300:
		{
			int num = 0;
			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
			if(GetCurrentTime() - m_dwTime_LotendSmema < 100) break;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk , IO_OFF) == IO_ON )
				num++;
				
			if(num <= 0)
			{
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 310;

				g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);

				st_handler.mn_OutElv_work = CTL_NO;

			}
			else
			{
				st_msg.mstr_event_msg[0] = "Conveyor is not inline";
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);				
				m_step_conv_smema = 305;
			}
		}
		break;
		
	case 305:
		{
			int num = 0;
			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
			if(GetCurrentTime() - m_dwTime_LotendSmema < 100) break;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
				num++;
			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk , IO_OFF) == IO_ON )
				num++;
			
			if(num <= 0)
			{
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);				
				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
				m_dwTime_LotendSmema = GetCurrentTime();
				m_step_conv_smema = 310;
				
				g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
				
				st_handler.mn_OutElv_work = CTL_NO;
				
			}
		}
		break;

	case 310:
		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_OFF )
		{
			m_step_conv_smema = 0;
			//2016.1215
			if( st_handler.mn_chk_lotend == CTL_READY )
			{
				st_handler.mn_chk_lotend = CTL_YES;
			}

		}
		else if( st_basic.mn_outconv == CTL_NO )
		{
			m_step_conv_smema = 0;
		}
		break;
	}

// 	Func.ThreadFunctionStepTrace(71, m_step_conv_smema);
// 	switch( m_step_conv_smema )
// 	{
// 	case 0:
// 		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
// 		{
// 			st_handler.mn_OutElv_work = CTL_YES;
// 			m_step_conv_smema = 10;
// 		}
// 		break;
// 
// 	case 10:
// 		if( st_basic.mn_outconv == CTL_YES )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 20;
// 		}
// 		else
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 100;
// 		}
// 		break;
// 
// 	case 20:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON )
// 		{
// 			m_step_conv_smema = 50;
// 		}
// 		else
// 		{
// 
// 			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 			else if( GetCurrentTime() - m_dwTime_LotendSmema > 10000)
// 			{
// 				//error, smema time out error;
// 				//m_step_conv_smema = 40;
// 			}
// 			if( st_basic.mn_outconv == CTL_NO )
// 			{
// 				m_step_conv_smema = 0;
// 			}
// 
// 		}
// 		break;
// 
// 	case 50:
// 		g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_ON);
// 		m_dwTime_LotendSmema = GetCurrentTime();
// 		m_step_conv_smema = 51;
// 		break;
// 
// 	case 51:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 60;
// 		}
// // 		else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// // 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// // 		{
// // 			m_step_conv_smema = 0;
// // 		}
// 		break;
// 
// 	case 60:
// 		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 		else if( GetCurrentTime() - m_dwTime_LotendSmema > 30)
// 		{
// 			if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 				m_step_conv_smema = 100;
// 			}
// // 			else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// // 				g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// // 			{
// // 				m_step_conv_smema = 0;
// // 			}
// 		}
// 		break;
// 		
// 	case 100:
// 		{
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 1500) break;
// 
// 			int num = 0;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
// 				num++;
// 
// 			if(num <= 1)
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
// 				m_dwTime_LotendSmema = GetCurrentTime();
// 				m_step_conv_smema = 200;
// 			}
// 		}
// 		break;
// 
// 	case 200:
// 		if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 		if(GetCurrentTime() - m_dwTime_LotendSmema < 100) break;
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_OFF) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_OFF )
// 		{
// 
// 			m_step_conv_smema = 201;
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 		}
// 		if( st_basic.mn_outconv == CTL_NO )
// 		{
// 			m_step_conv_smema = 210;
// 		}
// 		break;
// 
// 	case 201:
// 		if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 		if(GetCurrentTime() - m_dwTime_LotendSmema < 100) break;
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_OFF) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_OFF )
// 		{
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
// 			{
// 				m_step_conv_smema = 210;
// 			}
// 			else
// 			{
// 				m_step_conv_smema = 202;//move a little if devc is or not
// 
// 			}
// 
// 		}
// 		break;
// 
// 	case 202:
// 		g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 		m_dwTime_LotendSmema = GetCurrentTime();
// 		m_step_conv_smema = 203;
// 
// 	case 203:
// 		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk) == IO_ON )
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 			m_step_conv_smema = 210;
// 		}
// 		else
// 		{
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 300) break;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_OFF &&
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_OFF && 
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_OFF)
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 				st_handler.mn_OutElv_work = CTL_NO;
// 				m_step_conv_smema = 0;
// 			}
// 		}
// 		break;
// 
// 	case 210:
// 		if( st_basic.mn_outconv == CTL_YES )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 220;
// 		}
// 		else
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 300;
// 		}
// 		break;
// 
// 	case 220:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 250;
// 		}
// 		else
// 		{
// 			
// 			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 			else if( GetCurrentTime() - m_dwTime_LotendSmema > 10000)
// 			{
// 				//error, smema time out error;
// 				//m_step_conv_smema = 40;
// 			}
// 		}
// 		break;
// 
// 	case 250:
// 		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 		else if( GetCurrentTime() - m_dwTime_LotendSmema < 100) break;
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON )
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 251;
// 		}
// 		break;
// 		
// 	case 251:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 260;
// 		}
// // 		else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// // 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// // 		{
// // 			m_step_conv_smema = 210;
// // 		}
// 		else if( st_basic.mn_outconv == CTL_NO )
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 300;
// 		}
// 		break;
// 		
// 	case 260:
// 		if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 		else if( GetCurrentTime() - m_dwTime_LotendSmema > 30)
// 		{
// 			if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 				m_step_conv_smema = 300;
// 			}
// // 			else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// // 				g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// // 			{
// // 				m_step_conv_smema = 0;
// // 			}
// 		}
// 		break;
// 
// 	case 300:
// 		{
// 			int num = 0;
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 1000) break;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk , IO_OFF) == IO_ON )
// 				num++;
// 				
// 			if(num <= 0)
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 				m_dwTime_LotendSmema = GetCurrentTime();
// 				m_step_conv_smema = 310;
// 
// 				g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
// 
// 				st_handler.mn_OutElv_work = CTL_NO;
// 
// 			}
// 		}
// 		break;
// 
// 	case 310:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_OFF) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_OFF )
// 		{
// 			m_step_conv_smema = 0;
// 		}
// 		else if( st_basic.mn_outconv == CTL_NO )
// 		{
// 			m_step_conv_smema = 0;
// 		}
// 		break;
// 
// 	}
	
// 	Func.ThreadFunctionStepTrace(71, m_step_front_smema);
// 	switch( m_step_conv_smema )
// 	{
// 	case 0:
// 		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
// 		{
// 
// 			m_step_conv_smema = 10;
// 		}
// 		break;
// 
// 	case 5:
// 		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
// 		{
// 			
// 			m_step_conv_smema = 10;
// 		}
// 		break;		
// 
// 	case 10:
// 		if( st_basic.mn_outconv == CTL_YES )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 20;
// 		}
// 		else
// 		{
// // 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// // 			m_dwTime_LotendSmema = GetCurrentTime();
// // 			m_step_conv_smema = 100;
// 		}
// 		break;
// 
// 	case 20:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_ON )
// 		{
// 			m_step_conv_smema = 50;
// 		}
// 		else
// 		{
// 
// 			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 			else if( GetCurrentTime() - m_dwTime_LotendSmema > 10000)
// 			{
// 				//error, smema time out error;
// 				//m_step_conv_smema = 40;
// 			}
// 		}
// 		break;
// 
// 	case 40:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, 0, 30) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, 0, 30 ) == IO_ON )
// 		{
// 			m_step_conv_smema = 50;
// 		}
// 		else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// 		{
// 			m_step_conv_smema = 5;
// 		}
// 		break;
// 
// 	case 50:
// 		g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_ON);
// 		m_step_conv_smema = 60;
// 		break;
// 
// 	case 60:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 100;
// 		}
// 		else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// 		{
// 			m_step_conv_smema = 5;
// 		}
// 		break;
// 		
// 	case 100:
// 		{
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 1600) break;
// 
// 			int num = 0;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
// 				num++;
// 
// 			if(num <= 1)
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 				m_dwTime_LotendSmema = GetCurrentTime();
// 				m_step_conv_smema = 200;
// 			}
// 		}
// 		break;
// 
// 	case 200:
// 		if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 		if(GetCurrentTime() - m_dwTime_LotendSmema < 1600) break;
// 		m_step_conv_smema = 201;
// 		break;
// 
// 	case 201:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// 		{
// 			m_step_conv_smema = 0;
// 		}
// 		break;
// 		
// 
// 		if( st_basic.mn_outconv == CTL_YES )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 220;
// 		}
// 		else
// 		{
// 			//g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 300;
// 		}
// 		break;
// 
// 	case 205:
// 		if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
// 			g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON)
// 		{
// 			
// 			m_step_conv_smema = 210;
// 		}
// 		break;		
// 
// 	case 210:
// 		if( st_basic.mn_outconv == CTL_YES )
// 		{
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 220;
// 		}
// 		else
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			m_dwTime_LotendSmema = GetCurrentTime();
// 			m_step_conv_smema = 300;
// 		}
// 		break;
// 
// 	case 220:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_ON ||
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_OFF ) == IO_ON )
// 		{
// 			m_step_conv_smema = 250;
// 		}
// 		else
// 		{
// 			
// 			if( GetCurrentTime() - m_dwTime_LotendSmema <= 0 ) m_dwTime_LotendSmema = GetCurrentTime();
// 			else if( GetCurrentTime() - m_dwTime_LotendSmema > 10000)
// 			{
// 				//error, smema time out error;
// 				//m_step_conv_smema = 40;
// 			}
// 		}
// 		break;
// 		
// 	case 250:
// 		g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_ON);
// 		m_step_conv_smema = 260;
// 		break;
// 		
// 	case 260:
// 		if( g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_ON )
// 		{
// 			g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_ON);
// 			g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_ON);
// 			m_step_conv_smema = 300;
// 		}
// 		else if( g_ioMgr.get_in_bit( st_io.i_rear_req_signal_chk, IO_ON) == IO_OFF &&
// 			g_ioMgr.get_in_bit( st_io.i_rear_transfer_signal_chk, IO_ON ) == IO_OFF )
// 		{
// 			m_step_conv_smema = 205;
// 		}
// 		break;
// 
// 	case 300:
// 		{
// 			int num = 0;
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 0) m_dwTime_LotendSmema = GetCurrentTime();
// 			if(GetCurrentTime() - m_dwTime_LotendSmema < 1000) break;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON )
// 				num++;
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON )
// 				num++;
// 			
// 			if(num <= 0)
// 			{
// 				g_ioMgr.set_out_bit( st_io.o_uld_cv_motor_onoff, IO_OFF);
// 				m_dwTime_LotendSmema = GetCurrentTime();
// 				m_step_conv_smema = 0;
// 
// 				g_ioMgr.set_out_bit( st_io.o_rear_ready_signal, IO_OFF);
// 				g_ioMgr.set_out_bit( st_io.o_rear_enter_signal, IO_OFF);
// 
// 			}
// 		}
// 		break;

}

void APartHandler::Run_Move_front_Smema()
{
	Func.ThreadFunctionStepTrace(71, m_step_front_smema);
	switch( m_step_front_smema )
	{
	case -1:
		st_handler.mn_smema_front = CTL_NO;
		m_dwTime_front_Smema = GetCurrentTime();
		m_step_front_smema = -2;
		break;

	case -2:
		if(m_dwTime_front_Smema - GetCurrentTime() < 0) m_dwTime_front_Smema = GetCurrentTime();
		if(m_dwTime_front_Smema - GetCurrentTime() < 1000) break;
		m_step_front_smema = 0;
		break;
		
	case 0:
		if( st_basic.mn_mode_run == false || g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, 0, 50) == IO_OFF )
		{
			m_step_front_smema = 100;
		}
		break;

	case 100:
		if ( g_Tray.GetIdxByPos(IDPOS_DENSITY) == -1 && g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, IO_OFF) == IO_OFF)
		{
			st_handler.mn_smema_front = CTL_REQ;
			m_step_front_smema = 200;
		}
		break;

	case 200:
		if( st_handler.mn_smema_front == CTL_REQ || st_handler.mn_smema_front == CTL_READY)
		{
			g_ioMgr.set_out_bit( st_io.o_front_top_machine_enter_signal, IO_ON );
			m_step_front_smema = 300;
		}
//		else
//		{
//			g_ioMgr.set_out_bit( st_io.o_front_top_machine_enter_signal, IO_OFF );
//			st_handler.mn_smema_front = CTL_NO;
//			m_step_front_smema = 0;
//		}
		break;

	case 300:
		if( st_handler.mn_smema_front == CTL_READY)
		{
			m_step_front_smema = 400;
		}
		else if(st_handler.mn_smema_front == CTL_NO || g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, IO_OFF) == IO_OFF)
		{
//			g_ioMgr.set_out_bit( st_io.o_front_top_machine_enter_signal, IO_OFF );
//			st_handler.mn_smema_front = CTL_NO;
			m_step_front_smema = 200;
		}
		break;

	case 400:
		if( st_handler.mn_smema_front == CTL_CHANGE)
		{
			if(g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, IO_OFF) == IO_OFF)
			{
				m_step_front_smema = 500;
			}			
		}
		break;

	case 500:
		if( st_handler.mn_smema_front == CTL_FREE)
		{			
			g_ioMgr.set_out_bit( st_io.o_front_top_machine_enter_signal, IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_front_top_machine_transfer_signal, IO_OFF );
			m_dwTime_front_Smema = GetCurrentTime();
			m_step_front_smema = 600;
		}
		break;
		
	case 600:
		if( GetCurrentTime() - m_dwTime_front_Smema < 0 ) m_dwTime_front_Smema = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_front_Smema < 500 ) break;

		st_handler.mn_smema_front = CTL_NO; 
		m_step_front_smema = 0;
		break;
	}

}

void APartHandler::Run_Move_btm_rear_Smema()
{
	if( COMI.mn_run_status != dRUN ) return;


	Func.ThreadFunctionStepTrace(79, m_step_btm_rear_smema);
	switch( m_step_btm_rear_smema )
	{
	case 0:
		if( st_handler.n_without_smema_btm_8580s || g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, 0, 50) == IO_ON )
		{
			if(st_handler.mn_initial != TRUE)
			{
				g_ioMgr.set_out_bit( st_io.o_front_btm_machine_stop_signal, IO_ON);
				return;
			}

			g_ioMgr.set_out_bit( st_io.o_front_btm_machine_stop_signal, IO_OFF);
			m_nReryCnt = 0;
			m_step_btm_rear_smema = 100;
		}
		else
		{
			////2014,1203 처음 초기화 후 시작 시 인터락 걸려서 설비 진행 못함
// 			if(g_ioMgr.get_out_bit( st_io.o_front_btm_machine_stop_signal, IO_ON) != IO_ON)
// 				g_ioMgr.set_out_bit( st_io.o_front_btm_machine_stop_signal, IO_ON);

		}
		break;

	case 100:
		if( st_handler.n_without_smema_btm_8580s ||
			( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[1] == 1 ) &&
			g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, 0, 50) == IO_ON  )
		{
			m_dwTime_btm_rearSmema = GetCurrentTime();
			m_step_btm_rear_smema = 200;
		}
		break;

	case 200:
		if(st_handler.mn_initial == TRUE)
		{
			g_ioMgr.set_out_bit( st_io.o_front_btm_machine_stop_signal, IO_OFF);
		}
		else
		{
			m_step_btm_rear_smema = 0;
		}

		if( st_handler.n_without_smema_btm_8580s )
		{
			m_step_btm_rear_smema = 400;
			break;

		}

		if( GetCurrentTime() - m_dwTime_btm_rearSmema < 0 ) m_dwTime_btm_rearSmema = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_btm_rearSmema < 300 ) break;
		if( ( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[1] == 1 ) && g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, 0, 50) == IO_ON )
		{
			m_step_btm_rear_smema = 210;
		}
		break;

	case 210:
		if( st_client[CLS_BOTTOM].n_connect == CTL_YES )
		{
			m_step_btm_rear_smema = 250;			
		}
		else
		{
			::PostMessage( st_handler.hWnd, WM_CLIENT_MSG + CLS_BOTTOM, CLIENT_CONNECT,  CLS_BOTTOM);
			m_dwTime_btm_rearSmema = GetCurrentTime();
			m_step_btm_rear_smema = 220;
		}
		break;

	case 220:
		if( st_client[CLS_BOTTOM].n_connect == CTL_YES )
		{
			m_step_btm_rear_smema = 250;
		}
		else
		{
			if ( m_dwTime_btm_rearSmema <= 0 )
			{
				m_dwTime_btm_rearSmema = GetCurrentTime();
				break;
			}

			if ( GetCurrentTime() - m_dwTime_btm_rearSmema > st_basic.mn_network_wait_time )
			{
				m_nReryCnt++;
				if (m_nReryCnt > st_basic.mn_network_retry_cnt )
				{
					m_nReryCnt = 0;
					//740000 1 00 "Can't receive response from Front machine(8580S BTM)."
					CTL_Lib.Alarm_Error_Occurrence( 223, CTL_dWARNING, "740000");
					::SendMessage( st_handler.hWnd, WM_CLIENT_MSG + CLS_BOTTOM, CLIENT_CLOSE,  CLS_BOTTOM);
					m_step_btm_rear_smema = 210;
				}
				else
				{
					m_dwTime_btm_rearSmema = GetCurrentTime();
					::SendMessage( st_handler.hWnd, WM_CLIENT_MSG + CLS_BOTTOM, CLIENT_CLOSE,  CLS_BOTTOM);
					m_step_btm_rear_smema = 230;
				}
			}
		}
		break;

	case 230:
		if ( m_dwTime_btm_rearSmema <= 0 )
		{
			m_dwTime_btm_rearSmema = GetCurrentTime();
			break;
		}
		
		if ( GetCurrentTime() - m_dwTime_btm_rearSmema > st_basic.mn_network_wait_time )
		{
			m_step_btm_rear_smema = 210;
		}
		break;

	case 250:
		g_btm_client.OnMove_BufferIn();
		g_ioMgr.set_out_bit( st_io.o_front_btm_machine_ready_signal, IO_ON );
		m_dwTime_btm_rearSmema = GetCurrentTime();
		m_step_btm_rear_smema = 300;
		break;;

	case 300:
		if( /*1 ||*/g_btm_client.GetBufferInfo() == true)//clamp 해제요청
		{
			m_dwTime_btm_rearSmema = GetCurrentTime();
			m_step_btm_rear_smema = 400;
		}
		else if( GetCurrentTime() - m_dwTime_btm_rearSmema < 0) m_dwTime_btm_rearSmema = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_btm_rearSmema > (st_basic.mn_network_wait_time) )
		{
			m_step_btm_rear_smema = 310;
		}
// 		else if( g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, IO_ON) == IO_OFF )
// 		{
// 			m_step_btm_rear_smema = 0;
// 		}
		break;

	case 310:
		m_nReryCnt++;
		if( m_nReryCnt >= st_basic.mn_network_retry_cnt)
		{//740000 1 00 "Can't receive response from Front machine(8580S BTM)."
			m_nReryCnt = 0;
			CTL_Lib.Alarm_Error_Occurrence( 222, CTL_dWARNING, "740000");
			m_step_btm_rear_smema = 320;
		}
		else
		{
			m_dwTime_btm_rearSmema = GetCurrentTime();
			m_step_btm_rear_smema = 300;
		}
		break;

	case 320:
		if( COMI.mn_run_status == dRUN)
		{
			if( /*1 ||*/ g_btm_client.GetBufferInfo() == true)//clamp 해제요청
			{
				m_dwTime_btm_rearSmema = GetCurrentTime();
				m_step_btm_rear_smema = 400;
				break;
			}
			m_step_btm_rear_smema = 250;
			m_step_btm_rear_smema = 210;
		}
		break;


	case 400:
		if( GetCurrentTime() - m_dwTime_btm_rearSmema < 0 ) m_dwTime_btm_rearSmema = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_btm_rearSmema < 300) break;
		if( st_handler.n_without_smema_btm_8580s || 
			g_ioMgr.get_in_bit(st_io.i_front_btm_machine_transfer_signal_chk, 0, 50 ) == IO_ON )
		{
			m_dwTime_btm_rearSmema = GetCurrentTime();
			m_step_btm_rear_smema = 500;
		}
		break;

	case 500:
		if( st_handler.n_without_smema_btm_8580s || g_ioMgr.get_in_bit(st_io.i_front_btm_machine_transfer_signal_chk, 0, 50 ) == IO_ON )
		{
			g_Run_Btm_IDBuffers.ReqStopperUpdn(BACKWARD);//down
			g_Run_Btm_IDBuffers.ReqUldClamp(EHS_LCS_LD34, BACKWARD);
			m_step_btm_rear_smema = 600;
		}
		break;

	case 600:
		{
			if( g_Run_Btm_IDBuffers.GetStopperUpdn() != FB_NONE )				return;
			if( g_Run_Btm_IDBuffers.GetHookFwdBwd(EHS_HFB_LD2) != FB_NONE )			return;
			m_dwTime_btm_rearSmema = GetCurrentTime();
			m_step_btm_rear_smema = 700;

		}
		break;

	case 700:
		if( GetCurrentTime() - m_dwTime_btm_rearSmema < 0 ) m_dwTime_btm_rearSmema = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_btm_rearSmema < 300) break;
		g_ioMgr.set_out_bit( st_io.o_front_btm_machine_ready_signal, IO_OFF );

		if(  st_handler.n_without_smema_btm_8580s ||
			(g_ioMgr.get_in_bit( st_io.i_btm_idbuffer3_chk, IO_OFF) == IO_OFF && g_ioMgr.get_in_bit( st_io.i_btm_idbuffer4_chk, IO_OFF) == IO_OFF) )
		{
			m_step_btm_rear_smema = 1000;
			break;
		}
		if(g_ioMgr.get_in_bit(st_io.i_front_btm_machine_transfer_signal_chk, 0, 50 ) == IO_OFF )
		{
			CString strError="";
			strError.Format("5%04d%d",st_io.i_btm_idbuffer3_chk, IO_ON);
			CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, strError );
		}
		break;

	case 1000:
		if( st_handler.n_without_smema_btm_8580s || 
			g_ioMgr.get_in_bit(st_io.i_front_btm_machine_transfer_signal_chk, 0, 50 ) == IO_OFF )
		{
			Run_Smema_Ctl.btm_idbuffer_datainfo_shift( 0, SHIFT_IDBUFF_INDEX_ULD_NEXT);			
			m_step_btm_rear_smema = 0;
		}
		break;
	}

}

void APartHandler::Run_Move_rear_Smema()
{
	Func.ThreadFunctionStepTrace(72, m_step_rear_smema);
	
	switch( m_step_rear_smema )
	{
	case -1:
		st_handler.mn_smema_rear = CTL_NO;
		m_dwTime_rearSmema = GetCurrentTime();
		m_step_rear_smema = -2;
		break;

	case -2:
		if(m_dwTime_rearSmema - GetCurrentTime() < 0) m_dwTime_rearSmema = GetCurrentTime();
		if(m_dwTime_rearSmema - GetCurrentTime() > 5000) m_step_rear_smema = 0;
		break;


	case 0:
		if( g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, 0, 50) == IO_ON )
		{
			st_handler.mn_smema_rear = CTL_REQ;
			m_step_rear_smema = 100;
		}
		else
		{
			st_handler.mn_smema_rear = CTL_NO;
		}
		break;

	case 100:
		if( st_handler.mn_smema_rear == CTL_NO )
		{
			if( g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, 0, 20) == IO_ON )
			{
				st_handler.mn_smema_rear = CTL_REQ;
			}
		}
		if( st_handler.mn_smema_rear == CTL_REQ )
		{
			m_step_rear_smema = 200;
		}
		break;

	case 200:
		if( st_handler.mn_smema_rear == CTL_READY && 
			g_ioMgr.get_out_bit(st_io.o_front_btm_machine_ready_signal, IO_ON) == IO_ON)
		{
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[Rear SMEMA] Rear ready dignal on.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}
			m_step_rear_smema = 300;
		}
		break;

	case 300:
		if( g_ioMgr.get_in_bit(st_io.i_front_btm_machine_request_signal_chk, 0, 50) == IO_OFF )
		{
			st_handler.mn_smema_rear = CTL_CHANGE;// CLAMP OFF
			m_step_rear_smema = 400;
		}
		break;

	case 400:
		if(st_handler.mn_smema_rear == CTL_CHANGE)
		{
			m_step_rear_smema = 500;
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[Rear SMEMA] Rear request off.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 500:
		if( m_step_rear_smema == CTL_FREE)
		{
			m_step_rear_smema = 600;
			m_dwTime_rearSmema = GetCurrentTime();
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[Rear SMEMA] Clamp off.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 600:
		if( GetCurrentTime() - m_dwTime_rearSmema < 0 ) m_dwTime_rearSmema = GetCurrentTime();
		if( GetCurrentTime() - m_dwTime_rearSmema < 200 ) break;

// 		g_ioMgr.set_out_bit(st_io.o_front_btm_machine_ready_signal, IO_OFF);
		m_step_rear_smema = 0;
		st_handler.mn_smema_rear = CTL_NO;
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "[Rear SMEMA A End] Rear Completed.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}
}

void APartHandler::Run_Move_Conv( int nIdx )
{
	Func.ThreadFunctionStepTrace(73+nIdx, m_step_conv[nIdx]);
	switch( m_step_conv[nIdx] )
	{
	case EC_NONE:		break;

	// Receive
	case EC_RECEIVE://nIdx : 0
		{
			g_Run_Top_IDBuffers.ReqRecvTray();
			m_step_conv[nIdx] = EC_RECEIVE_CHECK;
		}
		break;

	case EC_RECEIVE_CHECK:
		{
			if( g_Run_Top_IDBuffers.GetRecvTrayNone() )
			{
				m_step_conv[nIdx] = EC_NONE;
			}
		}
		break;

	// Send
	case EC_SEND://nIdx : 1
		{
			g_Run_Top_IDBuffers.ReqSendTray();
			m_step_conv[nIdx] = EC_SEND_CHK;
		}
		break;

	case EC_SEND_CHK:
		{
			if( g_Run_Top_IDBuffers.GetSendTrayNone() )
			{
				m_step_conv[nIdx] = EC_NONE;
			}
		}
		break;


	// Receive
	case EC_ULD_RECEIVE://nIdx : 0
		{
			g_Run_Btm_IDBuffers.ReqRecvTray();
			m_step_conv[nIdx] = EC_ULD_RECEIVE_CHECK;
		}
		break;

	case EC_ULD_RECEIVE_CHECK:
		{
			if( g_Run_Btm_IDBuffers.GetRecvTrayNone() )
			{
				m_step_conv[nIdx] = EC_NONE;
			}
		}
		break;

	// Send
	case EC_ULD_SEND://nIdx : 1
		{
			g_Run_Btm_IDBuffers.ReqSendTray();
			m_step_conv[nIdx] = EC_ULD_SEND_CHK;
		}
		break;

	case EC_ULD_SEND_CHK:
		{
			if( g_Run_Btm_IDBuffers.GetSendTrayNone() )
			{
				m_step_conv[nIdx] = EC_NONE;
			}
		}
		break;
	}
}

void APartHandler::Run_Move_Main()
{
//	if( m_step_conv[0] == EC_NONE && g_Tray.GetIdxByPos(IDPOS_DENSITY) == -1  && st_handler.mn_smema_front == CTL_REQ )
//	{
//		if( (st_handler.mn_smema_front == CTL_REQ && st_basic.mn_mode_run == false) || 
//			(st_handler.mn_smema_front == CTL_REQ && (g_ioMgr.get_in_bit( st_io.i_front_top_machine_ready_signal_chk, 0, 50 ) == IO_ON || st_handler.mn_front_ready == CTL_YES) ) )
//		{
//			g_front_server.SetBufferIn( NR_WAITING );
//			Control_byTrayRecv();
//			if (st_handler.cwnd_list != NULL)	// ¸®½ºÆ® ¹U E­¸e A¸Ac
//			{
//				sprintf(st_msg.c_normal_msg, "[Front Tray IN] Tray IN.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // μ¿AU ½CÆÐ Aa·A ¿aA≫
//			}
//			return;
//		}
//	}
//	if( m_step_conv[2] == EC_NONE &&  g_Tray.GetIdxByPos(IPOOS_UNLOAD_BTM) >= 0 && /*st_handler.mn_pickup_elv[0][1] == CTL_READY &&*/
//		g_Tray.GetIdxByPos(IPOOS_UNLOAD_BTM_LOAD) <= -1 )
	if( m_step_conv[2] == EC_NONE &&  ( st_basic.n_mode_device == false || (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_exist[0] == false &&  st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_exist[1] == false) ||
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_skip_flag[0] == 1 &&  st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_skip_flag[1] == 1) ) &&
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[0] == 0 && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[1] == 0) &&
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_idbuffer[1] == 1 ) )
	{
		Control_UnloadbyTrayRecv();
// 		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
// 		{
// 			sprintf(st_msg.c_normal_msg, "[BTM EL Tray] EL Receive.");
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 		}
		return;
		
	}
//	if( m_step_conv[3] == EC_NONE && g_Tray.GetIdxByPos(IPOOS_UNLOAD_BTM_LOAD) >= 0 && g_Tray.GetIdxByPos(IPOOS_UNLOAD_BTM_UNLOAD) <= -1 && 
//		g_Tray.GetIdxByPos(IDPOS_BTM_UNLOAD_TO_NEXT) <= -1)
	if( m_step_conv[3] == EC_NONE && ( st_basic.n_mode_device == false || (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_exist[0] == false &&  st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_exist[1] == false) ||
		(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_skip_flag[0] == 1 &&  st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_skip_flag[1] == 1) ) &&
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[1] == 1) &&
		( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[0] == 0 && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[1] == 0) )
	{				
		Control_UnloadbyTraySend();
// 		if (st_handler.cwnd_list != NULL)	// ¸®½ºÆ® ¹U E­¸e A¸Ac
// 		{
// 			sprintf(st_msg.c_normal_msg, "[BTM EL Tray] EL OUT.");
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // μ¿AU ½CÆÐ Aa·A ¿aA≫
// 		}
		return;
		
	}	
//	Control_byTraySend();

// 	ControlDensityRobot();
	
	if( COMI.mn_run_status == dRUN)
	{
		Unload_Elvatror_Check();
		ControlDensityRobot();
		ControlBarcode1Robot();
		ControlBarcode2Robot();
		ControlVisonRobot();
		ControlUnloadRobot();
		ControlUnloadElvRobot();
		ControlConvyorRobot();
	}


	Run_Move_Virtual_Supply();
}

void APartHandler::ControlConv( int iIdx, ESTEP_CONV ec )
{
	m_step_conv[iIdx] = ec;
}

void APartHandler::Control_byTraySend()
{
	bool bConvUsable=false;
	int iBin1 = g_Tray.GetIdxByPos( IDPOS_DENSITY );
	int iBin2 = g_Tray.GetIdxByPos( IDPOS_BARCODE1 );
	int iBin3 = g_Tray.GetIdxByPos( IDPOS_BARCODE2 );
	int iBin4 = g_Tray.GetIdxByPos( IDPOS_TOP_VISION );
	int iBin5 = g_Tray.GetIdxByPos( IDPOS_UNLOAD );
	int iBin6 = g_Tray.GetIdxByPos( IPOOS_UNLOAD_BTM );
	bConvUsable = (m_step_conv[0] == EC_NONE && m_step_conv[1] == EC_NONE && m_step_conv[2] == EC_NONE);

	if( bConvUsable != true ) return;

	if( Func.m_pRobot[SITE_UNLOAD]->GetJobCount() !=0 ) return;

	if( iBin5 != -1 || iBin6 != -1) return;

	if( iBin5 >= 0)return;
 	if( iBin1 < 0 && iBin2 < 0 && iBin3 < 0 && iBin4 < 0 ) return;

	if( iBin1>= 0){
		ATray tray = g_Tray.GetTray( iBin1 );
		if( tray.GetModule(0).IsBufferSkip() && tray.GetModule(1).IsBufferSkip() )
		{
			tray.SetWorkState(EWS_DENSITY);
		}
		if( tray.GetWorkState() != EWS_DENSITY ){
				return;
		}
	}
	if( iBin2>= 0){
		ATray tray = g_Tray.GetTray( iBin2 );
		if( tray.GetModule(0).IsBufferSkip() && tray.GetModule(1).IsBufferSkip() )
		{
			tray.SetWorkState(EWS_BARCODE1);
		}
		if( tray.GetWorkState() != EWS_BARCODE1 ){
				return;
		}
	}
	if( iBin3>= 0){
		ATray tray = g_Tray.GetTray( iBin3 );
		if( tray.GetModule(0).IsBufferSkip() && tray.GetModule(1).IsBufferSkip() )
		{
			tray.SetWorkState(EWS_BARCODE2);
		}
		if( tray.GetWorkState() != EWS_BARCODE2 ){
 			return;
 		}
	}
	if( iBin4>= 0){
		ATray tray = g_Tray.GetTray( iBin4 );
		if( tray.GetModule(0).IsBufferSkip() && tray.GetModule(1).IsBufferSkip() )
		{
			tray.SetWorkState(EWS_TOPVISION);
		}
		if( tray.GetWorkState() != EWS_TOPVISION ){
			return;
		}
	}
	if( iBin5>= 0){
		ATray tray = g_Tray.GetTray( EWS_BTMVISION );
		if(Func.m_pRobot[SITE_UNLOAD]->GetJobCount() <=0)
		{
			if( tray.GetModule(0).IsBufferSkip() && tray.GetModule(1).IsBufferSkip() )
			{
				tray.SetWorkState(EWS_BTMVISION);
			}
		}
		if( tray.GetWorkState() != EWS_BTMVISION ){
			return;
		}
	}

	ControlConv( 1, EC_SEND );
// 	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 	{
// 		sprintf(st_msg.c_normal_msg, "ControlByTray) EC_SEND EC_RECEIVE");
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 	}

	return;

}

void APartHandler::Control_byTrayRecv()
{
	bool bConvUsable=false;
	bConvUsable = (m_step_conv[0] == EC_NONE && m_step_conv[1] == EC_NONE);
	
	int iBin1 = g_Tray.GetIdxByPos( IDPOS_DENSITY );
	if( iBin1>= 0){
		return;
	}

	if( bConvUsable != true ) return;	
	
	ControlConv( 0, EC_RECEIVE );
// 	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 	{
// 		sprintf(st_msg.c_normal_msg, "ControlByTray) EC_SEND EC_RECEIVE");
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 	}
	return;
}

void APartHandler::Control_UnloadbyTraySend()
{
	bool bConvUsable=false;
	int iBin1 = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD].n_idbuffer[1] == 1;//g_Tray.GetIdxByPos( IPOOS_UNLOAD_BTM_LOAD );
	int iBin2 = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD].n_idbuffer[1] == 1;//g_Tray.GetIdxByPos( IPOOS_UNLOAD_BTM_UNLOAD );
	bConvUsable = (m_step_conv[2] == EC_NONE && m_step_conv[3] == EC_NONE);

	if( bConvUsable != true ) return;
	if( iBin1 < 1) return;
	if( iBin2 > 1) return;

	ControlConv( 3, EC_ULD_SEND );
// 	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 	{
// 		sprintf(st_msg.c_normal_msg, "ControlUnloadByTray) EC_SEND EC_RECEIVE");
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 	}
	return;

}

void APartHandler::Control_UnloadbyTrayRecv()
{
	bool bConvUsable=false;
// 	bConvUsable = (m_step_conv[2] == EC_NONE && 
// 		(m_step_conv[3] == EC_NONE || g_Run_Btm_IDBuffers.m_step_sendTray >= SEND_BTM_IDTRAY_ULD2_CLAMP_ON) );
	//2015.0520
	bConvUsable = (m_step_conv[2] == EC_NONE && 
		(m_step_conv[3] == EC_NONE || g_Run_Btm_IDBuffers.m_step_sendTray >= SEND_BTM_IDTRAY_ULD2_ULDMOVE_CHK) );
	
//	int iBin1 = g_Tray.GetIdxByPos( IPOOS_UNLOAD_BTM_LOAD );
//	if( iBin1>= 0){
//		return;
//	}

	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_idbuffer[0] == 0 && 
		st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM].n_idbuffer[1] == 0)
	{
		return;
	}
	if( bConvUsable != true ) return;	
	
	ControlConv( 2, EC_ULD_RECEIVE );
// 	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 	{
// 		sprintf(st_msg.c_normal_msg, "ControlUnloadByTray) EC_SEND EC_RECEIVE");
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 	}
	return;
}


void APartHandler::ControlDensityRobot()
{
	
	if( st_sync.n_density_work_req == CTL_READY)
	{

	//{
		if( Func.m_pRobot[SITE_DENSITY]->GetJobCount() > 0)
			return;

		if( st_den_rbt[0].bPick == false &&  st_den_rbt[1].bPick == false &&
			st_den_feeder[st_basic.mn_device_type].st_label.bOut == true && 
			st_den_feeder[st_basic.mn_device_type+3].st_label.bOut == true)
		{
			std::vector<int> vecModule = Func.m_pSite[SITE_DENSITY]->GetVecLabelFeeder();
			if( CR_FeederPickupTest( vecModule ) )								return;	
		}
		else
		{
				std::vector<int> vecModule = g_Tray.GetVecDensityTestModule(0);		
				if( CR_DensityforTest( vecModule ) )								return;	
		}
	}
	else
	{
		st_sync.n_density_work_req = CTL_REQ;
// 		if(st_var.n_lotstart == ID_LOT_END) return;//2014.0317
		if(st_handler.n_without_density_label_rbt == CTL_NO)
		{
			if( Func.m_pRobot[SITE_DENSITY]->GetJobCount() > 0)
				return;
			
			if( st_den_rbt[0].bPick == false &&  st_den_rbt[1].bPick == false &&
				st_den_feeder[st_basic.mn_device_type].st_label.bOut == true && 
				st_den_feeder[st_basic.mn_device_type+3].st_label.bOut == true)
			{
				std::vector<int> vecModule = Func.m_pSite[SITE_DENSITY]->GetVecLabelFeeder();
				if( CR_FeederPickupTest( vecModule ) )								return;	
			}
			else
			{//2015.0405

				if( st_den_rbt[0].bPick == true &&  st_den_rbt[1].bPick == true/* && st_work.mn_tl_status == dSELFCHECK*/ )
				{
// 					if( g_lotMgr.GetLotCount() < 1 && st_var.mn_lotend_density_flag == CTL_NO/* && g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES"*/ )
// 					{
						if( st_var.n_lotstart == ID_LOT_END )
						{//2015.0426
							if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT] == CTL_NO &&
								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == CTL_NO &&
								st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_last_module[CTL_LEFT] == ID_LOT_END &&
								st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_last_module[CTL_RIGHT] == ID_LOT_END )
							{
								st_var.mn_lotend_density_flag = CTL_YES;
								std::vector<int> vecModule = g_Tray.GetVecDensityTestModule(0);		
								if( CR_DensityforTest( vecModule ) )								return;	
							}
						}
// 					}
				}
			}
		}
	}
	//}
}

void APartHandler::ControlBcrFeederRobot()
{
	if( Func.m_pRobot[SITE_BARCODE1]->GetJobCount() > 0)
		return;

	// 잡고 있는게 없을 때
	if( Func.m_pRobot[SITE_BARCODE1]->IsPicking() == false )
	{
		std::vector<int> vecModule = g_Tray.GetVecBcr1TestModule(0);// always have to test lotno 0 or lotno 1
		
		if( CR_Bcr1forTest( vecModule ) )								return;				// 테스트 할 모듈을 사이트에 보낸다.
	}
}

void APartHandler::ControlBarcode1Robot()
{
	if( st_sync.n_bcr_1_work_req == CTL_READY)
	{
		if( Func.m_pRobot[SITE_BARCODE1]->GetJobCount() > 0)
			return;

		int nCnt = 0, nnCnt = 0, nEnable= 0;
	//	if( Func.m_pRobot[SITE_BARCODE1]->IsBcr1Picking() == false )
		if( st_bcr_rbt[0].bPick[0] == false && st_bcr_rbt[0].bPick[1] == false )
		{
			if( st_basic.nBcrFeederUse[0] == CTL_YES ) nEnable++;
			if( st_basic.nBcrFeederUse[1] == CTL_YES ) nEnable++;

			//2015.0228
			if( st_bcr_feeder[0].st_bcr.bExist == true && st_bcr_feeder[0].st_bcr.bOut == true &&
				st_bcr_feeder[0].st_bcr.nState == EMBS_PASS )
			{
				if( st_basic.nBcrFeederUse[0] != CTL_YES ) st_bcr_feeder[0].st_bcr.nState = EMBS_FAIL;
				else									   nCnt++;
			}
			if( st_bcr_feeder[1].st_bcr.bExist == true && st_bcr_feeder[1].st_bcr.bOut == true &&
				st_bcr_feeder[1].st_bcr.nState == EMBS_PASS )
			{
				if( st_basic.nBcrFeederUse[1] != CTL_YES ) st_bcr_feeder[1].st_bcr.nState = EMBS_FAIL;
				else									   nCnt++;
			}
			
// 			if( nEnable <= 0 && nCnt <= 0 )
// 			{
// 				st_sync.n_bcr_1_work_req = CTL_REQ;
// 				return;
// 			}

			//2015.0521
			if( (st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_LEFT] == ID_LOT_END ||
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_RIGHT] == ID_LOT_END) && 
				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT] == 0 &&
				st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == 0 && 
				st_var.n_using_barcode == 1 && st_var.n_use_barcode[0] != EMBS_SKIP && st_var.n_use_barcode[1] != EMBS_SKIP)
			{
				return;
			}

			//2015.0327
			if( (nEnable <= 0 && nCnt <= 0) || (st_var.n_using_barcode == 1 && nCnt <= 0))
			{
				st_sync.n_bcr_1_work_req = CTL_REQ;
				return;
			}
			if( nEnable != nCnt ) return;


			std::vector<int> vecModule = Func.m_pSite[SITE_BARCODE1]->GetVecBarcode1Feeder();
			if( CR_Bcr1PickupTest( vecModule ) )								return;	
		}
		else
		{
			std::vector<int> vecMdl = g_Tray.GetVecBcr1TestModule(0);// always have to test lotno 0 or lotno 1
			//2015.0607
			if(vecMdl.size() == 0 && st_handler.m_nBcrRbtResult[0] == RET_SKIP)
			{
				st_sync.n_bcr_1_work_req = CTL_REQ;
				return;
			}
			if( CR_Bcr1forTest( vecMdl ) )										return;				// A¡¿¨o¨￢¨¡￠c CO ￠￢ⓒ￡￥iaA¡i ¡icAI¨¡￠c￠?￠® ¨￢￠￢ⓒø¨o￠￥U.
		}
	}
	else
	{
		st_sync.n_bcr_1_work_req = CTL_REQ;
		if(st_handler.n_without_bcr1_rbt == CTL_NO)
		{
			if( Func.m_pRobot[SITE_BARCODE1]->GetJobCount() > 0)
				return;

			int nCnt = 0, nnCnt = 0, nEnable= 0;
			if( st_bcr_rbt[0].bPick[0] == false && st_bcr_rbt[0].bPick[1] == false )
			{
				if( st_basic.nBcrFeederUse[0] == CTL_YES ) nEnable++;
				if( st_basic.nBcrFeederUse[1] == CTL_YES ) nEnable++;

				//2015.0228
				if( st_bcr_feeder[0].st_bcr.bExist == true && st_bcr_feeder[0].st_bcr.bOut == true &&
					st_bcr_feeder[0].st_bcr.nState == EMBS_PASS)
				{
					if( st_basic.nBcrFeederUse[0] != CTL_YES ) st_bcr_feeder[0].st_bcr.nState = EMBS_FAIL;
					else									   nCnt++;
				}
				if( st_bcr_feeder[1].st_bcr.bExist == true && st_bcr_feeder[1].st_bcr.bOut == true &&
					st_bcr_feeder[1].st_bcr.nState == EMBS_PASS)
				{
					if( st_basic.nBcrFeederUse[1] != CTL_YES ) st_bcr_feeder[1].st_bcr.nState = EMBS_FAIL;
					else									   nCnt++;
				}				

// 				if( (nEnable <= 0 && nCnt <= 0) )
// 				{
// 					st_sync.n_bcr_1_work_req = CTL_REQ;
// 					return;
// 				}
				//2015.0327
				if( (nEnable <= 0 && nCnt <= 0) || (st_var.n_using_barcode == 0 && nCnt <= 0) )
				{
					st_sync.n_bcr_1_work_req = CTL_REQ;
					return;
				}
				
				if( nEnable != nCnt ) return;
				std::vector<int> vecModule = Func.m_pSite[SITE_BARCODE1]->GetVecBarcode1Feeder();
				if( CR_Bcr1PickupTest( vecModule ) )								return;	
			}
		}
	}	
}

void APartHandler::ControlBarcode2Robot()
{
	if( st_sync.n_bcr_2_work_req == CTL_READY)
	{
		if( Func.m_pRobot[SITE_BARCODE2]->GetJobCount() > 0)
			return;
		
		int nCnt = 0, nnPCnt = 0, nnFCnt = 0, nEnable= 0, nnWCnt = 0;
	//	if( Func.m_pRobot[SITE_BARCODE2]->IsBcr1Picking() == false )
		if( st_bcr_rbt[1].bPick[0] == false && st_bcr_rbt[1].bPick[1] == false )
		{
			if( st_basic.nBcrFeederUse[2] == CTL_YES ) nEnable++;
			if( st_basic.nBcrFeederUse[3] == CTL_YES ) nEnable++;

			if( st_bcr_feeder[2].st_bcr.bExist == true && st_bcr_feeder[2].st_bcr.bOut == true &&
				st_bcr_feeder[2].st_bcr.nState == EMBS_PASS)
			{
				if( st_basic.nBcrFeederUse[2] != CTL_YES ) st_bcr_feeder[2].st_bcr.nState = EMBS_FAIL;
				else									   nCnt++;
			}
			
			if( st_bcr_feeder[3].st_bcr.bExist == true && st_bcr_feeder[3].st_bcr.bOut == true &&
				st_bcr_feeder[3].st_bcr.nState == EMBS_PASS)
			{
				if( st_basic.nBcrFeederUse[3] != CTL_YES ) st_bcr_feeder[3].st_bcr.nState = EMBS_FAIL;
				else									   nCnt++;
			}

			if( nEnable <= 0 && nCnt <= 0)
			{
				st_sync.n_bcr_2_work_req = CTL_REQ;
				return;
			}

			if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_LEFT] == 0 ) nnWCnt++;
			if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_RIGHT] == 0 ) nnWCnt++;

			if( nnWCnt == 0 )
			{
				st_sync.n_bcr_2_work_req = CTL_REQ;
				return;
			}

			if( nEnable != nCnt ) return;
			std::vector<int> vecModule = Func.m_pSite[SITE_BARCODE2]->GetVecBarcode2Feeder();
			if( CR_Bcr2PickupTest( vecModule ) )								return;	
		}
		else
		{
			std::vector<int> vecMdl = g_Tray.GetVecBcr2TestModule(0);
			//2015.0607
			if(vecMdl.size() == 0 && st_handler.m_nBcrRbtResult[1] == RET_SKIP)
			{
				st_sync.n_bcr_2_work_req = CTL_REQ;
				return;
			}			
			
			if( CR_Bcr2forTest( vecMdl ) )							    	return;

// 			if( vecMdl.size() < 1 )
// 			{
// 				st_sync.n_bcr_2_work_req = CTL_REQ;
// 				return;
// 			}

			
		}
	}
	else
	{
		st_sync.n_bcr_2_work_req = CTL_REQ;
		if(st_handler.n_without_bcr2_rbt == CTL_NO)
		{
			if( Func.m_pRobot[SITE_BARCODE2]->GetJobCount() > 0)
				return;
			
			int nCnt = 0, nnPCnt = 0, nnFCnt = 0, nEnable= 0, nnWCnt = 0;
			if( st_bcr_rbt[1].bPick[0] == false && st_bcr_rbt[1].bPick[1] == false )
			{
				if( st_basic.nBcrFeederUse[2] == CTL_YES ) nEnable++;
				if( st_basic.nBcrFeederUse[3] == CTL_YES ) nEnable++;

				if( st_bcr_feeder[2].st_bcr.bExist == true && st_bcr_feeder[2].st_bcr.bOut == true &&
					st_bcr_feeder[2].st_bcr.nState == EMBS_PASS)
				{
					if( st_basic.nBcrFeederUse[2] != CTL_YES ) st_bcr_feeder[2].st_bcr.nState = EMBS_FAIL;
					else									   nCnt++;
				}
				
				if( st_bcr_feeder[3].st_bcr.bExist == true && st_bcr_feeder[3].st_bcr.bOut == true &&
					st_bcr_feeder[3].st_bcr.nState == EMBS_PASS)
				{
					if( st_basic.nBcrFeederUse[3] != CTL_YES ) st_bcr_feeder[3].st_bcr.nState = EMBS_FAIL;
					else									   nCnt++;
				}
				
				if( nEnable <= 0 && nCnt <= 0)
				{
					st_sync.n_bcr_2_work_req = CTL_REQ;
					return;
				}
				
// 				if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_LEFT] == 0 ) nnWCnt++;
// 				if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_RIGHT] == 0 ) nnWCnt++;
				
// 				if( nnWCnt == 0 )
// 				{
// 					st_sync.n_bcr_2_work_req = CTL_REQ;
// 					return;
// 				}
				
				if( nEnable != nCnt ) return;
				std::vector<int> vecModule = Func.m_pSite[SITE_BARCODE2]->GetVecBarcode2Feeder();
				if( CR_Bcr2PickupTest( vecModule ) )								return;	
			}
		}
	}
}

void APartHandler::ControlVisonRobot()
{
	if( st_sync.n_topvision_work_req == CTL_READY)
	{
	
		if( Func.m_pRobot[SITE_TOP_VISION]->GetJobCount() > 0)
			return;

		if( Func.m_pRobot[SITE_TOP_VISION]->IsTopVisionPicking() == true )
		{
			std::vector<int> vecMustTestModule_A = g_Tray.GetVecTopVisTestModule(0);
			
			if( CR_TopVisforTest( vecMustTestModule_A ) )				return;
		}
	}
	else
	{
		st_sync.n_topvision_work_req = CTL_REQ;
	}
}

void APartHandler::ControlUnloadRobot()
{
// 	if( st_sync.n_unload_elevator_req == CTL_CHANGE)
// 	{
		if( Func.m_pRobot[SITE_UNLOAD]->GetJobCount() > 0)
			return;

		if( ( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[CTL_LEFT] != 1 && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[CTL_RIGHT] != 1) &&
			( /*st_basic.n_mode_device == 1 && */
			( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_LEFT] == 1 ||	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_RIGHT] == 1 )  ) &&
			( st_uld_rbt.n_exist[CTL_LEFT] == 0 && st_uld_rbt.n_exist[CTL_RIGHT] == 0 ) )
		{
			std::vector<int> vecMdl = g_Tray.GetVecUnloadTestModule(0);
			
			if( CR_UnloadforTest( vecMdl ) )							return;	
		}
		else if( st_uld_rbt.n_exist[0] || st_uld_rbt.n_exist[1] )
		{
			std::vector<int> vecGoodCell = g_Tray.GetVecPickerUnloadConvey(0);
			std::vector<int> vecRejEmptyCell = g_Tray.GetVecRejEmptyCell();
			if( CR_UnloadforConv( vecGoodCell, vecRejEmptyCell ) )		return;				
		}
// 	}
}

void APartHandler::ControlUnloadElvRobot()
{
	if(st_sync.n_unload_elevator_req == CTL_READY )
	{
		
		if( st_sync.n_unload_sub_elev_req != CTL_REQ)
		{
			st_sync.n_unload_sub_elev_req = CTL_REQ;
			ReqUnloadElvRobot();
		}
		else
		{
			if( GetUnloadElvatror_Check() == true)
			{
				n_unload_elevator_req = CTL_READY;
				st_sync.n_unload_sub_elev_req = CTL_NO;
				//st_sync.n_unload_elevator_req = CTL_REQ;
			}
		}
		
	}
	else if( st_sync.n_unload_elevator_req == CTL_CHANGE)
	{
		if( Func.m_pRobot[SITE_ELV]->GetJobCount() > 0)
			return;	

		if( ( st_basic.n_mode_device == false || 
			( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[0] == 0 && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[1] == 0) ) &&
			( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[0] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[1] == 1 ) ) 
		{
			std::vector<int> vecMdl = g_Tray.GetVecUnloadElvTray(0);
			
			if( CR_UnloadforElv( vecMdl ) )
				return;
		}
	}
	if( st_sync.n_unload_elevator_req == CTL_NO) st_sync.n_unload_elevator_req = CTL_REQ;
}

void APartHandler::ControlConvyorRobot()
{
// 	if( Func.m_pRobot[SITE_CONV]->GetJobCount() > 0)
// 		return;
// 	// Aa°i AO´A°O ¾øA≫ ¶§
// 	if( Func.m_pRobot[SITE_CONV]->IsPicking() == true )
// 	{
// 		std::vector<int> vecMdl = g_Tray.GetVecUnloadConveyorModule(0);// always have to test lotno 0 or lotno 1
// 		
// 		if( CR_ConverforNext( vecMdl ) )								return;				// A×½ºÆ® CO ¸ðμaA≫ ≫cAIÆ®¿¡ º¸³½´U.
// 	}
}

void APartHandler::Run_Move_Virtual_Supply()
{
	if( st_handler.mn_virtual_supply == 0 )	
		return;

	Func.ThreadFunctionStepTrace(78, m_step_virtual_supply);
	switch( m_step_virtual_supply )
	{
	case EVS_NONE:		m_step_virtual_supply = EVS_CALC;	break;

	case EVS_CALC://2013,0131
		if( g_Tray.IsEnablePos( IDPOS_DENSITY ) )
		{
			m_dwTime_virtual_supply = GetCurrentTime();
			m_step_virtual_supply = EVS_ADDTRAY;
		}
		break;
		
	case EVS_ADDTRAY:
		st_handler.mn_emptyin_cnt = 0;
		if( GetCurrentTime() - m_dwTime_virtual_supply > 500 )
		{
//			if( st_handler.mn_smema_front == CTL_READY || g_server_front.GetBufferInReceived() == NR_WAITING)
//				g_server_front.SetBufferIn( NR_RECEIVED );
//			else
//				return;
			


			EIDBUFFER_POS eTargetPos = IDPOS_NONE;
			if( g_Tray.IsEnablePos( IDPOS_FRONT_TO_DENSITY ) )
			{
				eTargetPos = IDPOS_FRONT_TO_DENSITY;
			}

			if( eTargetPos != TPOS_NONE )
			{
				ATray tray;
				tray.SetPos( IDPOS_FRONT_TO_DENSITY );
				tray.SetTrayInfo( g_lotMgr.GetLotIDAt(0), 0, false, false );
				
				for( int i=0; i<MAX_MODULE; i++ )
				{
					CString strProductID = "mSATA";
					
					CString strLotID = g_lotMgr.GetLotAt(0).GetLotID();
					CString strSkip = "1";
					CString strArraySN;		strArraySN.Format("%02d", 1 + rand()%33 );
					CString strSerial;		strSerial.Format( "S11ZNYAC500%d%02d", eTargetPos - IDPOS_DENSITY, i );
					CString strPPID =		"NONE";//OnNetworkBodyAnalysis( strPocket[i], "PPID" );
 					CString strWWN =		"NONE";//OnNetworkBodyAnalysis( strPocket[i], "WWN" );
 					CString strCSerial =	"NONE";//OnNetworkBodyAnalysis( strPocket[i], "C_SERIAL" );
 					CString strPSID =		"NONE";//OnNetworkBodyAnalysis( strPocket[i], "PSID" );
 					CString strBIN =		"";
					CString strVar = "1";
					
					// 모듈 셋
					AModule mdl;
 					mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
					tray.SetModule(i, mdl);
				}
				
				tray.SetTargetPos( eTargetPos );
				g_Tray.AddTray( tray );
				//g_Tray.TrayMoved( IDPOS_FRONT_TO_DENSITY, eTargetPos );

				//FUNCTION_RPY=BUFFER_IN_RPY LOT_ID=ABCD PART_ID=EFGH BUFFER_SKIP=(BUF_01=NO BUF_02=NO) 
				//EQPID=SSD_LABELLER BYPASS=NO LAST_MDL=YES MDL_CNT=2 MD_01=(BUF SERIAL=1234567, …… …) MD_02=(BUF= SERIAL=1234567, …… …) 보낸다

				m_step_virtual_supply = EVS_FINISH;
			}
		}//2012,1220
		else if( GetCurrentTime() - m_dwTime_virtual_supply < 0 )
		{
			m_dwTime_virtual_supply = GetCurrentTime();
		}

		break;

	case EVS_FINISH:
		m_step_virtual_supply = EVS_NONE;
		break;
		
	}
}

double APartHandler::CalcTargetPosition( int nAxis, BOOL bPick, int nTarget, int nPickerIdx )
{
	int nRobot = nPickerIdx;
	if( nPickerIdx < 0 )
		nPickerIdx = 0;

	int nTargetPart = GetPosPartByTarget( nTarget );

	int nPosStart = 0;
	double nRtn = 0;

 	if( nAxis == M_ULD_Z)
 	{
 		switch( nTargetPart )
 		{
		case MPOS_INDEX_UNLOAD:
		case MPOS_INDEX_CONVEYOR:
		case MPOS_INDEX_CONVEYOR + 1:
			nPosStart = P_ULD_Z_CONV_POS;
			break;
 		case MPOS_INDEX_REJECT:			
		case MPOS_INDEX_REJECT + 1:			
			nPosStart = P_ULD_Z_REJ_POS;	
 			break;
 			
 		}
 	}
 	else if( nAxis == M_ULD_Y )
 	{
 		switch( nTargetPart )
 		{
		case MPOS_INDEX_UNLOAD:
 		case MPOS_INDEX_CONVEYOR:
		case MPOS_INDEX_CONVEYOR + 1:
			nPosStart = P_ULD_Y_CONV_POS;
			break;
 		case MPOS_INDEX_REJECT:
		case MPOS_INDEX_REJECT + 1:
			nPosStart = P_ULD_Y_REJ_POS;
			break;
 		}
 	}

	nRtn = st_motor[nAxis].d_pos[ nPosStart];

	return nRtn;
}
//2014,1227
int APartHandler::GetModuleCnt()
{
	int nCount = 0;
	
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] == 1 )	nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] == 1 )	nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT] == 1 )		nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == 1 )	nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_LEFT] == 1 )		nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_RIGHT] == 1 )		nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT] == 1 )		nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == 1 )		nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_LEFT] == 1 )			nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_RIGHT] == 1 )		nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_LEFT] == 1 )			nCount++;
	if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_RIGHT] == 1 )		nCount++;

	if( Func.m_pRobot[SITE_UNLOAD]->GetJobCount() > 0 ) nCount++;
	if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) == IO_ON ||
		g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) == IO_ON || 
		g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk) == IO_ON) nCount++;

	if( g_handler.m_step_conv_smema > 0 ) nCount++;

	return nCount;
}

void APartHandler::CheckLotEnd()
{
 	if( g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) >= atoi(g_lotMgr.GetLotAt(0).GetStrTotalModule() ) )
 	{
 		g_lotMgr.GetLotByLotID( g_lotMgr.GetLotIDAt(0) ).SetLastModule( "YES" );
 	}
	if( g_lotMgr.GetLotCount() == 0 )								return;
	if( g_Tray.GetTrayCnt_byLotID( g_lotMgr.GetLotIDAt(0) ) > 0 )	return;
	if( g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES" )			return;

//	if( g_handler.GetMachinePos() == EMPOS_CONVEYOR )
//		g_client_ec.OnTestEnd( g_lotMgr.GetLotIDAt(0), g_lotMgr.GetLotAt(0).GetPartID() );

//	g_client_bpc.SetOnlineEnd( g_lotMgr.GetLotIDAt(0) );
// 	st_handler.mn_chk_lotend = CTL_YES;

	CString strLotid="";
	int iMdlCnt = GetModuleCnt();
	if( iMdlCnt > 0)
	{

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_LEFT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[CTL_LEFT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[CTL_RIGHT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[CTL_RIGHT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_lot_id[CTL_LEFT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_lot_id[CTL_RIGHT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_LEFT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_lot_id[CTL_LEFT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_RIGHT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_lot_id[CTL_RIGHT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_lot_id[CTL_LEFT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_lot_id[CTL_RIGHT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_LEFT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_lot_id[CTL_LEFT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_RIGHT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_lot_id[CTL_RIGHT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_LEFT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_lot_id[CTL_LEFT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_RIGHT] == 1 )
		{
			strLotid.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_lot_id[CTL_RIGHT]);
			if(strLotid == g_lotMgr.GetLotIDAt(0)) return;
		}

	}

	if( g_lotMgr.GetLotAt(0).GetXClose() == "YES" )
	{
		st_msg.mstr_event_xmsg[0] = "[LOT] Is this lot XCLOSE?";
		::PostMessage(st_handler.hWnd, WM_MAIN_XEVENT, CTL_YES, 0);
// 		 evMes.OnPutToQueue(MES_PK_XCLOSE);
		return;
	}
	if( g_lotMgr.GetLotAt(0).GetPassCnt(CUM) < atoi(g_lotMgr.GetLotAt(0).GetStrTotalModule() ) )
		return;


	//2015.0317
	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		CString strLog="";
		int nSec = (GetCurrentTime() - g_lotMgr.GetLotAt(0).GetLotTime())/1000;
		if( nSec < 1) nSec = 1;
		int nUPH = g_lotMgr.GetLotAt(0).GetInputCnt(CUM) * 3600 / nSec;
		strLog.Format("[Lotend : %s] Total time; %d sec. Input:%d. Good:%d. Fail:%d. UPH:%d ea.\r\n", g_lotMgr.GetLotIDAt(0), nSec,
			g_lotMgr.GetLotAt(0).GetInputCnt(CUM), g_lotMgr.GetLotAt(0).GetPassCnt(CUM), g_lotMgr.GetLotAt(0).GetFailCnt(PRIME), nUPH );
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 .출력 요청
		Func.On_LogFile_Add(LOG_TOTAL, strLog);
		Func.On_LogFile_Add(LOG_UPH, strLog);		
	}


	g_lotMgr.DeleteLot( 0 );
	st_handler.mn_chk_lotend = CTL_NO;
	g_Run_Barcode_Feeders.m_total_vis_error[0] = 0;
// 	st_handler.mn_Lotcnt = 0;
	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);

}

void APartHandler::AddAlarmCnt()
{
	 m_nAlarmCnt++;
	 AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM, m_nAlarmCnt );
}

void APartHandler::ClearAlarmCnt()
{
	m_nAlarmCnt = 0;
	 AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM, m_nAlarmCnt );
}


bool APartHandler::CR_FeederPickupTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i=0; i< vecMustTestMdl.size(); i++ )
	{
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_DENSITY]->AddJob( JOB_PICK, 0, 1, vecEnableTest[0]);
		
	}
	else if( vecEnableTest.size() == 1 )
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_DENSITY_FEEDER;
		Func.m_pRobot[SITE_DENSITY]->AddJob( JOB_PICK, nRobotIdx_forWork, -1, vecEnableTest[0]);
	}
	
	return false;
}

bool APartHandler::CR_DensityforTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i=0; i< vecMustTestMdl.size(); i++ )
	{
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_DENSITY]->AddJob( JOB_PLACE, 0, 1, vecEnableTest[0]);
		
	}
	else if( vecEnableTest.size() == 1 )
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_DENSITY;
		Func.m_pRobot[SITE_DENSITY]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableTest[0]);
	}
	
	return false;
}


bool APartHandler::CR_Bcr1PickupTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i=0; i< vecMustTestMdl.size(); i++ )
	{
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_BARCODE1]->AddJob( JOB_PICK, 0, 1, vecEnableTest[0]);
		
	}
	else if( vecEnableTest.size() == 1 )
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_BCR1_FEEDER;
		if(nRobotIdx_forWork == 0)
			Func.m_pRobot[SITE_BARCODE1]->AddJob( JOB_PICK, nRobotIdx_forWork, -1, vecEnableTest[0]);
		else
			Func.m_pRobot[SITE_BARCODE1]->AddJob( JOB_PICK, -1, nRobotIdx_forWork, vecEnableTest[0]);
	}
	
	return false;
}

bool APartHandler::CR_Bcr2PickupTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i=0; i< vecMustTestMdl.size(); i++ )
	{
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_BARCODE2]->AddJob( JOB_PICK, 0, 1, vecEnableTest[0]);
		
	}
	else if( vecEnableTest.size() == 1 )
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_BCR2_FEEDER;
		if(nRobotIdx_forWork == 0)
			Func.m_pRobot[SITE_BARCODE2]->AddJob( JOB_PICK, nRobotIdx_forWork, -1, vecEnableTest[0]);
		else
			Func.m_pRobot[SITE_BARCODE2]->AddJob( JOB_PICK, -1, nRobotIdx_forWork, vecEnableTest[0]);
	}
	
	return false;
}

bool APartHandler::CR_Bcr1forTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i = 0; i < vecMustTestMdl.size(); i++ )
	{
		EIDBUFFER_POS tpos = IDPOS_BARCODE1;

//		int nTrayIdx = g_Tray.GetIdxByPos( tpos);
//		ATray& tray = g_Tray.GetTray( nTrayIdx );
//		AModule& tmdl = tray.GetModule( vecMustTestMdl[i] );
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_BARCODE1]->AddJob( JOB_PLACE, 0 ,1 , vecEnableTest[0]);
	}
	else if( vecEnableTest.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_BCR1_LOAD;
		if(nRobotIdx_forWork == 0)
			Func.m_pRobot[SITE_BARCODE1]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableTest[0]);
		else
			Func.m_pRobot[SITE_BARCODE1]->AddJob( JOB_PLACE,  -1, nRobotIdx_forWork,vecEnableTest[0]);
	}
	return false;
}

bool APartHandler::CR_Bcr2forTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i = 0; i < vecMustTestMdl.size(); i++ )
	{
		EIDBUFFER_POS tpos = IDPOS_BARCODE2;

// 		int nTrayIdx = g_Tray.GetIdxByPos( tpos);
// 		ATray& tray = g_Tray.GetTray( nTrayIdx );
// 		AModule& tmdl = tray.GetModule( vecMustTestMdl[i] % MPOS_DIVISION );
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_BARCODE2]->AddJob( JOB_PLACE, 0 ,1 , vecEnableTest[0]);
	}
	else if( vecEnableTest.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_BCR2_LOAD;
		if(nRobotIdx_forWork == 0)
			Func.m_pRobot[SITE_BARCODE2]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableTest[0]);
		else
			Func.m_pRobot[SITE_BARCODE2]->AddJob( JOB_PLACE, -1, nRobotIdx_forWork, vecEnableTest[0]);
	}
	return false;
}

bool APartHandler::CR_TopVisforTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i = 0; i < vecMustTestMdl.size(); i++ )
	{
		EIDBUFFER_POS tpos = IDPOS_TOP_VISION;

//		int nTrayIdx = g_Tray.GetIdxByPos( tpos);
//		ATray& tray = g_Tray.GetTray( nTrayIdx );
//		AModule& tmdl = tray.GetModule( vecMustTestMdl[i] % MPOS_DIVISION );
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
// 		Func.m_pRobot[SITE_TOP_VISION]->AddJob( JOB_PLACE, 0 ,-1 , vecEnableTest[0]);
// 		Func.m_pRobot[SITE_TOP_VISION]->AddJob( JOB_PLACE, 1 ,-1 , vecEnableTest[1]);
		Func.m_pRobot[SITE_TOP_VISION]->AddJob( JOB_PLACE, 0 ,1 , vecEnableTest[0]);

	}
	else if( vecEnableTest.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_TOPVISION;
		Func.m_pRobot[SITE_TOP_VISION]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableTest[0]);
	}
	return false;
}

bool APartHandler::CR_UnloadforTest( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i = 0; i < vecMustTestMdl.size(); i++ )
	{
		EIDBUFFER_POS tpos = IDPOS_UNLOAD;

//		int nTrayIdx = g_Tray.GetIdxByPos( tpos);
//		ATray& tray = g_Tray.GetTray( nTrayIdx );
//		AModule& tmdl = tray.GetModule( vecMustTestMdl[i] % MPOS_DIVISION );
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_UNLOAD]->AddJob( JOB_PICK, 0 , 1, vecEnableTest[0]);
	}
	else if( vecEnableTest.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_UNLOAD;
		Func.m_pRobot[SITE_UNLOAD]->AddJob( JOB_PICK, nRobotIdx_forWork, -1, vecEnableTest[0]);
	}
	return false;
}

bool APartHandler::CR_UnloadforElv( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i = 0; i < vecMustTestMdl.size(); i++ )
	{
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_ELV]->AddJob( JOB_PICK, 0 , 1, vecEnableTest[0]);
	}
	else if( vecEnableTest.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_UNLOAD;
		Func.m_pRobot[SITE_ELV]->AddJob( JOB_PICK, nRobotIdx_forWork, -1, vecEnableTest[0]);
	}
	return false;
}

bool APartHandler::CR_UnloadforConv( std::vector<int> vecGoodMdl, std::vector<int> vecRejEmptyMdl )
{
	std::vector<int> vecEnableGood;
	int i = 0;
	for( i = 0; i < vecGoodMdl.size(); i++ )
	{
		vecEnableGood.push_back( vecGoodMdl[i] );
	}

	if( vecEnableGood.size() >= 2 )
	{
		Func.m_pRobot[SITE_UNLOAD]->AddJob( JOB_PLACE, 0 , 1, vecEnableGood[0]);
	}
	else if( vecEnableGood.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableGood[0] % MPOS_INDEX_CONVEYOR;
		Func.m_pRobot[SITE_UNLOAD]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableGood[0]);
	}
	
	std::vector<int> vecEnableReject;
	for( i = 0; i < vecRejEmptyMdl.size(); i++ )
	{
		vecEnableReject.push_back( vecRejEmptyMdl[i] );
	}

	if( vecEnableReject.size() >= 2 )
	{
		Func.m_pRobot[SITE_UNLOAD]->AddJob( JOB_PLACE, 0 , 1, vecEnableReject[0]);
	}
	else if( vecEnableReject.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableReject[0] % MPOS_INDEX_REJECT;
		Func.m_pRobot[SITE_UNLOAD]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableReject[0]);
	}
	
	return false;
}

bool APartHandler::CR_ConverforNext( std::vector<int> vecMustTestMdl )
{
	std::vector<int> vecEnableTest;
	for( int i = 0; i < vecMustTestMdl.size(); i++ )
	{
		vecEnableTest.push_back( vecMustTestMdl[i] );
	}

	if( vecEnableTest.size() >= 2 )
	{
		Func.m_pRobot[SITE_CONV]->AddJob( JOB_PLACE, 0 , 1, vecEnableTest[0]);
	}
	else if( vecEnableTest.size() == 1)
	{
		int nRobotIdx_forWork;
		nRobotIdx_forWork = vecEnableTest[0] % MPOS_INDEX_CONVEYOR;
		Func.m_pRobot[SITE_CONV]->AddJob( JOB_PLACE, nRobotIdx_forWork, -1, vecEnableTest[0]);
	}
	return false;
}

void APartHandler::ReqUnloadElvRobot()
{
	if( m_step_unloadrobot == ESTEP_UNLOAD_ELV_NONE)
		m_step_unloadrobot = ESTEP_UNLOAD_ELV_READY;
}

void APartHandler::Unload_Elvatror_Check()
{
	switch(m_step_unloadrobot)
	{
	case ESTEP_UNLOAD_ELV_NONE: break;

		case ESTEP_UNLOAD_ELV_READY:
			{
				int nRet_1 = COMI.Check_MotPosRange(M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER], COMI.md_allow_value[M_IDBUFFER_EL]);
				int nRet_2 = g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_hook_clamp_chk );
				int nRet_3 = g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_hook_unclamp_chk );
				if( nRet_1 == BD_GOOD && nRet_2 == IO_ON && nRet_3 == IO_OFF)
				{
					m_step_unloadrobot = ESTEP_ELV_MDL_CHECK;
				}
				else
				{
					m_step_unloadrobot = ESTEP_ELV_READY;
				}
			}
			break;

		case ESTEP_ELV_READY:
			g_Run_IDBuffers_El.ReqReadyElv();
			m_step_unloadrobot = ESTEP_ELV_READY_CHK;
			break;

		case ESTEP_ELV_READY_CHK:
			if( !g_Run_IDBuffers_El.GetElvReady_IDBuffr() ) return;
			st_sync.n_unload_elevator_req = CTL_FREE;
			m_step_unloadrobot = ESTEP_UNLOAD_ELV_NONE;			
			break;

	}
}
