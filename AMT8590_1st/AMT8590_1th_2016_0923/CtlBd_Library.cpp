// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "SrcPart/APartHandler.h"
#include "IO_Manager.h"

#include "Srcbase\ALocalization.h"//20130930

//2015.0326 james 
#include "Cmmsdk.h"
#include "CmmsdkDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtlBd_Library::CCtlBd_Library()
{

}

CCtlBd_Library::~CCtlBd_Library()
{

}

CCtlBd_Library CTL_Lib;

//////////////////////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 
//알람발생 처리방법, 모터 및 I/O 관련 라이브러리를 초기화 및 응용하여 실제사용하는 함수정리 
//
//참고 기본함수 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{
	//그간은 알람이 발생하면 각 쓰레드에서 jamcode 및 run status 정보를 바뀌어 장비를 정지하고 
	//문제를 해결했으나 앞으로는 이 함수를 모든곳에서 사용하여 알람정보 및 가동 상태를 표준함수 하나를 
	//사용하여 장비 가동 및 알람 문제를 처리한다 
	//
	//각 쓰레드에 각각있던 알람 처리방법은 이 함수를 모두 사용한다 
   //
	//2013,0123
	if(COMI.mn_run_status == dINIT) alarm.stl_alarm_start_time = GetCurrentTime();
	CTL_Lib.mn_jampos = n_jampos; //jam이 발생한 위치를 저장한다 
	CTL_Lib.mn_run_status = n_run_status; //장비의 가동상태를 변경한다 
	COMI.mn_run_status = n_run_status;
	alarm.mstr_code = c_alarmcode;
	alarm.stl_cur_alarm_time = GetCurrentTime();

	strcpy(mc_alarmcode, c_alarmcode);

//	g_handler.AddAlarmCnt();

//	g_ioMgr.set_out_bit( st_io.o_bin_conveyor1, IO_OFF );
//	g_ioMgr.set_out_bit( st_io.o_bin_conveyor2, IO_OFF );
//	g_ioMgr.set_out_bit( st_io.o_bin_conveyor3, IO_OFF ); //2013,0916

	//2012,1220
	// 	CtlBdFunc.Alarm_Error_Occurrence(CTL_Lib.mn_jampos, COMI.mn_run_status, COMI.mn_run_status, alarm.mc_code);
	MyJamData.On_Alarm_Info_Set_to_Variable(alarm.mstr_code);
	
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		//2015.0514
		st_handler.m_strLogEvent = Func.Get_AlarmPartName(atoi(alarm.mstr_code));
	}
// 	g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
	return BD_GOOD;
}

int CCtlBd_Library::Alarm_Error_Occurrence( int n_jampos, int n_run_status, CString strJamCode )
{
	//2013,0123
	if(COMI.mn_run_status == dINIT) alarm.stl_alarm_start_time = GetCurrentTime();
	CTL_Lib.mn_jampos = n_jampos; //jam이 발생한 위치를 저장한다 
	CTL_Lib.mn_run_status = n_run_status; //장비의 가동상태를 변경한다 
	COMI.mn_run_status = n_run_status;
	alarm.mstr_code = strJamCode;
	alarm.stl_cur_alarm_time = GetCurrentTime();

	strcpy(mc_alarmcode, (LPCSTR)strJamCode);

//	g_handler.AddAlarmCnt();

//	g_ioMgr.set_out_bit( st_io.o_bin_conveyor1, IO_OFF );
//	g_ioMgr.set_out_bit( st_io.o_bin_conveyor2, IO_OFF );
//	g_ioMgr.set_out_bit( st_io.o_bin_conveyor3, IO_OFF );

	//2012,1220
// 	CtlBdFunc.Alarm_Error_Occurrence(CTL_Lib.mn_jampos, COMI.mn_run_status, COMI.mn_run_status, alarm.mc_code);
	MyJamData.On_Alarm_Info_Set_to_Variable(alarm.mstr_code);
	
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		//2015.0514
		st_handler.m_strLogEvent = Func.Get_AlarmPartName(atoi(alarm.mstr_code));
	}

	
// 	g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
	return BD_GOOD;
}


int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor 보드 초기화시 사용
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(n_bd_type == 0) //0:파스텍 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{		//모터 보드별 축수를 정의한다 

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 
 	}
	else if(n_bd_type == 1) //1:커미조아 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{
		//1. 보드 정보 읽어들인다 
		//2. 각종 셋팅 정보를 적용한다 
		nRet = COMI.Initialize_MotBoard(s_filename);			// MOTOR 보드 초기화 여부 검사한다
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD] 초기화 에러.");
			LogFile_Write("D:\\AMTLOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 

// 		if(COMI.mn_motorbd_init_end == BD_YES)
// 		{			
// 			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
// 			{//n_simul_mode => 0:시뮬레이션 모드 비활성, 1:시뮬레이션 모드 활성
// 
// 				if(COMI.mn_simulation_mode == 1) //모터를 가상으로 제어하여 실제 모터 출력이 나가고 기구물이 이동한것처럼 동작한다 
// 				{//활성
// 					COMI.Set_Simulation_Mode(i, 1); 
// 				}
// 				else
// 				{//비활성 
// 					COMI.Set_Simulation_Mode(i, 0); 
// 				}
// 			}		
//		}
	}

	for( i = 0; i < 100; i++ )
		mn_single_motmove_step[i] = 0;

	return  BD_GOOD;
}

//아래 내용은 I/O 보드 초기화시 필요한 내용을 예로 들은것이니 
//장비별 필요한 내용을 셋팅하여 사용하면 된다.
int CCtlBd_Library::Initialize_io_Board(int n_bd_type)
{
	//if(n_bd_type == 0) //파스텍 HSSI I/O board 
	//{
	//}
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	

	/////////////////초기에 초기화시 //////////////////////////////////////////////////////////////////////
	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	if (Ret == FALSE)
	{
		return BD_ERROR;
	}

	return BD_GOOD;
}


int CCtlBd_Library::Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos) 	//모터 안전 정보 셋팅시 사용 
{
	//안전 관련하여는 참고하여 장비별 적용한다 

	// 예) 
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog, 4:Length Change
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet1 = 0, nRet2 = 0, nRet3 = 0;
	double d_CurPos[4] = {0,};
	double d_Pos[4] = {0,};
	double d_GapCheck = 0;
	int i = 0;
	CString strJamCode;

	nRet = Func.OnIsDoorOpen();
	if ( nRet != -1)//2015,0207//2017.0510
	{		
		strJamCode.Format("5370%d1",nRet);
		CTL_Lib.Alarm_Error_Occurrence( 991, dWARNING, strJamCode );
		sprintf(st_msg.c_abnormal_msg, "991,[DOOR CHECK] Door Open being done Run become.");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		return BD_ERROR;
	}

//2011.0201 주석처리 	int n_SetTime = 50; //091127 20->50으로 변경 100; //500ms 동안 계속 감기되면 그때 비로소 I/O 상태 return 

	// **************************************************************************
	// 모터 알람 상태 검사한다                                                   
	// -> 알람 발생한 경우 알람 해제한다                                         
	// **************************************************************************
	//software limit 값 셋팅 체크 에러 
	if(d_targetpos < COMI.md_limit_position[n_axis][0] && st_handler.mn_virtual_mode == 0 ) //
	{
		sprintf(cJamcode, "%02d0004", n_axis);
		Alarm_Error_Occurrence(11, CTL_dWARNING, cJamcode);
		
		if (st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return BD_ERROR;
	}	
	if(d_targetpos > COMI.md_limit_position[n_axis][1] && st_handler.mn_virtual_mode == 0 ) //
	{
		sprintf(cJamcode, "%02d0004", n_axis);
		Alarm_Error_Occurrence(12, CTL_dWARNING, cJamcode);
		
		if (st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return BD_ERROR;
	}

	// **************************************************************************
	// 모터 파워 상태 검사한다                                                   
	// -> 모터 POWER OFF 시 POWER ON 상태로 만든다                               
	// **************************************************************************
	if (g_comiMgr.Get_MotPower(n_axis) == CTL_ERROR )          // 모터 POWER 상태 검사 함수 
	{
		if (COMI.Set_MotPower(n_axis, CTL_ON) == CTLBD_RET_GOOD)       // 모터 POWER ON 설정 함수 
		{
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}

			if (COMI.Get_MotPower(n_axis) == CTL_ERROR)  // 모터 POWER 상태 검사 함수 
			{
				if (st_motor[n_axis].n_retry_cnt > (MOT_RTY_CNT))
				{
					sprintf(cJamcode, "%02d0005", n_axis);
					Alarm_Error_Occurrence(13, CTL_dWARNING, cJamcode);	
					return BD_ERROR;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if (st_motor[n_axis].n_retry_cnt > MOT_RTY_CNT)
			{
				printf(cJamcode, "%02d0005", n_axis);
				Alarm_Error_Occurrence(14, CTL_dWARNING, cJamcode);		
				return BD_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return BD_RETRY;
			}
		}
	}

	//모터 알람 발생시 처리 조건 
	if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR)      // 모터 ALARM 상태 검사 함수 
	{
		if (COMI.Set_MotAlarmClear(n_axis) == CTL_GOOD)       // 모터 ALARM CLEAR 함수 
		{
			//091119 james			Sleep(1000);  // 일정 시간 후에 상태 확인하기 위해 SLEEP 사용한다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR )  // 모터 ALARM 상태 검사 함수 
			{
				if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
				{			
					sprintf(cJamcode, "%02d0002",  n_axis); alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					sprintf(alarm.mc_code, "%02d0002", n_axis);
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, cJamcode, CtlBdFunc.ms_ErrMsg);
					//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // 알람 해제 시도 횟수 
					return CTL_ERROR;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return CTL_RETRY;
				}
			}
		}
		else
		{
			if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
			{
				sprintf(cJamcode, "%02d0002",  n_axis); alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
				sprintf(alarm.mc_code, "%02d0002", n_axis);
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Alarm Return Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, cJamcode, CtlBdFunc.ms_ErrMsg);
				
				//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // 알람 해제 시도 횟수 
				return CTL_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return CTL_RETRY;
			}
		}
	}
	// **************************************************************************

	// **************************************************************************
	// 모터의 안전 위치를 체크한다. 장비에 맞게 넣는것 보다. 딴쪽에 지정해 놓고 불러다 써야 할듯.                                           
	//***************************************************************************


 	switch( n_axis )
	{
	case M_D_FEEDER_1:
		break;
 	case M_D_FEEDER_2:
		break;
	case M_D_FEEDER_3:
		break;
	case M_D_FEEDER_4:
		break;
	case M_D_FEEDER_5:
		break;
	case M_D_FEEDER_6:
		break;
	case M_BCR_FEEDER_1:
		if(d_targetpos < st_motor[M_BCR_FEEDER_1].d_pos[P_BCR_FEEDER_VISION])
		{
			if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X])
			{
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z1) > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(15, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
		}
		break;
	case M_BCR_FEEDER_2:
		if(d_targetpos < st_motor[M_BCR_FEEDER_2].d_pos[P_BCR_FEEDER_VISION])
		{
			if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X])
			{
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z2) > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(16, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
		}
		break;

	case M_BCR_FEEDER_3:
		if(d_targetpos < st_motor[M_BCR_FEEDER_3].d_pos[P_BCR_FEEDER_VISION])
		{
			if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X])
			{
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_Z1) > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(17, CTL_dWARNING, "220008" );
					return BD_ERROR;
				}
			}
		}
		break;

	case M_BCR_FEEDER_4:
		if(d_targetpos < st_motor[M_BCR_FEEDER_4].d_pos[P_BCR_FEEDER_VISION])
		{
			if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X])
			{
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_Z2) > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(18, CTL_dWARNING, "220008" );
					return BD_ERROR;
				}
			}
		}
		break;

	case M_D_VISION_X:
		break;
		
	case M_D_ATTACH_X:
	case M_D_ATTACH_Y:
		if( COMI.Get_MotCurrentPos(M_D_ATTACH_Z1) > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z1] && st_handler.mn_virtual_mode == 0 )
		{//130008 0 00 "Density Attach Z1 Axis Motor is Non Safety Pos."
			CTL_Lib.Alarm_Error_Occurrence(19, CTL_dWARNING, "130008" );
			return BD_ERROR;
		}
		if( COMI.Get_MotCurrentPos(M_D_ATTACH_Z2) > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z2] && st_handler.mn_virtual_mode == 0 )
		{//140008 0 00 "Density Attach Z2 Axis Motor is Non Safety Pos."
			CTL_Lib.Alarm_Error_Occurrence(20, CTL_dWARNING, "140008" );
			return BD_ERROR;
		}
		break;
		
	case M_D_ATTACH_Z1:
		break;
	case M_D_ATTACH_Z2:
		break;
	case M_BCR_VISION_X:
		break;
	case M_L_REJECT_X:

		if( g_ioMgr.get_in_bit( st_io.i_reject_rbt_up_chk, IO_ON) == IO_OFF || 
			g_ioMgr.get_in_bit( st_io.i_reject_rbt_down_chk, IO_OFF) == IO_ON)
		{
			CString strErr;
			strErr.Format("5%04d%d",st_io.i_reject_rbt_up_chk, IO_OFF);

			CTL_Lib.Alarm_Error_Occurrence(21, CTL_dWARNING, strErr);
			return BD_ERROR;

		}
		break;

	case M_BCR_ATTACH1_X:
		if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z1) > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] ||
			 COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z2) > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
		{//180008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(22, CTL_dWARNING, "180008" );
			return BD_ERROR;
		}

		if( g_ioMgr.get_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum_fwdback, IO_OFF) == IO_ON ||
			g_ioMgr.get_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum_fwdback, IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_left_btm_vacuum_bwd_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_right_btm_vacuum_bwd_chk, IO_ON) == IO_OFF )
		{
			CString strError;
			if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_left_btm_vacuum_bwd_chk, IO_ON) == IO_OFF)
			{
				strError.Format("5%04d%d",st_io.i_distribute_rbt1_left_btm_vacuum_bwd_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(5022, CTL_dWARNING, strError );
			}
			else
			{
				strError.Format("5%04d%d",st_io.i_distribute_rbt1_right_btm_vacuum_bwd_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(5023, CTL_dWARNING, strError );
			}
			return BD_ERROR;
		}
		break;

	case M_BCR_ATTACH1_Z1:
		{
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_ON);
		
			//3:Jog
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] - 50 - COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1990, CTL_dWARNING, "180008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1990, BCR_ATTACH1_Z1 Place Pos, ATTACH1_Z1 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) >= st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] + 50 +  COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1991, CTL_dWARNING, "180008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1991, BCR_ATTACH1_Z1 Pick Pos, ATTACH1_Z1 value is too low->target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_PICK] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3800, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3801, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3802, CTL_dWARNING, strJamCode );
					return BD_ERROR;
				}
				else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3803, CTL_dWARNING, strJamCode );
					return BD_ERROR;					
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1603, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_DOWN] )
			{
				if(nRet1 != IO_ON || nRet2 != IO_OFF) 
				{
					if(nRet1 != IO_ON)
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3804, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3805, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					CString strErr="";
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
						strErr.Format("5%04d%d", st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_OFF);
					else
						strErr.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(23, CTL_dWARNING, strErr );
					return BD_ERROR;

				}

				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1604, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_ERROR] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3806, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt1_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3807, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3808, CTL_dWARNING, strJamCode );
					return BD_ERROR;					
				}
				if( d_targetpos > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(3809, CTL_dWARNING, "180008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "3809,BCR_ATTACH1_Z1 Error Pos, ATTACH1_Z1 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1605, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
			if( (n_mode != 3 && n_mode != 0 ) && (d_targetpos != st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_PICK] &&
				d_targetpos != st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_DOWN] && 
				d_targetpos != st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_ERROR] &&
				d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY] &&
				d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_UP] ) )
			{
				CTL_Lib.Alarm_Error_Occurrence(3990, CTL_dWARNING, "180008" );
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "3990, BCR_ATTACH1_Z1 Position error, ATTACH1_Z1 value is No Position -> target position: [%4.3f]", d_targetpos);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return BD_ERROR;
			}
			if( ( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				  COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				  COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH1_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				  COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH1_X] ) )
			{
				
			}
			else
			{
				if( n_mode != 0 )//2016.0926 1
				{
					CTL_Lib.Alarm_Error_Occurrence(233, CTL_dWARNING, "170008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "233, BCR_ATTACH1_Z1, BCR_ATTACH1_Y  Position error -> position: [%4.3f]", COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X));
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}				
			}
		}
		break;

	case M_BCR_ATTACH1_R1:
		if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z1) > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] )
		{//180008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(1606, CTL_dWARNING, "180008" );
			return BD_ERROR;
		}
		break;

	case M_BCR_ATTACH1_Z2:
		{
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_ON);

			//3:Jog
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] - 50 - COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1992, CTL_dWARNING, "200008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1992, BCR_ATTACH1_Z2 Place Pos, ATTACH1_Z2 value is too low ->target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) >= st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] + 50 +  COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1993, CTL_dWARNING, "200008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1993, BCR_ATTACH1_Z2 Pick Pos, ATTACH1_Z2 value is too low ->target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			
			if( d_targetpos == st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_PICK] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3810, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3811, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3812, CTL_dWARNING, strJamCode );
					return BD_ERROR;
				}
				else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3813, CTL_dWARNING, strJamCode );
					return BD_ERROR;
					
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1607, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_DOWN] )
			{
				if(nRet1 != IO_ON || nRet2 != IO_OFF) 
				{
					if(nRet1 != IO_ON)
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3814, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3815, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					CString strErr;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
						strErr.Format("5%04d%d", st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_OFF);
					else
						strErr.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(1608, CTL_dWARNING, strErr );
					return BD_ERROR;
					
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1609, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_ERROR] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt1_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3816, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt1_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3817, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3818, CTL_dWARNING, strJamCode );
					return BD_ERROR;					
				}
				if( d_targetpos > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(3819, CTL_dWARNING, "200008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "3819, BCR_ATTACH1_Z2 Error Pos, ATTACH1_Z2 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1610, CTL_dWARNING, "170008" );
					return BD_ERROR;
				}
			}
			if( (n_mode != 3 && n_mode != 0 ) && (d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_PICK] &&
				d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_DOWN] && 
				d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_ERROR] && 
				d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY] && 
				d_targetpos != st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_UP] ) )
			{
				CTL_Lib.Alarm_Error_Occurrence(3991, CTL_dWARNING, "200008" );
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "3991, BCR_ATTACH1_Z2 Position error, ATTACH1_Z2 value is No Position -> target position: [%4.3f]", d_targetpos);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return BD_ERROR;
			}
			if( ( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH1_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH1_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) > st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH1_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH1_X] ) )
			{
				
			}
			else
			{
				if( n_mode != 0 )//2016.0926 2
				{
					CTL_Lib.Alarm_Error_Occurrence(243, CTL_dWARNING, "170008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "243, BCR_ATTACH1_Z2, BCR_ATTACH1_Y  Position error -> position: [%4.3f]", COMI.Get_MotCurrentPos(M_BCR_ATTACH1_X));
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
		}
		break;
	case M_BCR_ATTACH1_R2:
		if( COMI.Get_MotCurrentPos(M_BCR_ATTACH1_Z2) > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z2])
		{//180008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(24, CTL_dWARNING, "180008" );
			return BD_ERROR;
		}
		break;

	case M_BCR_ATTACH2_X:
		if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_Z1) > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] ||
			COMI.Get_MotCurrentPos(M_BCR_ATTACH2_Z2) > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z2])
		{//230008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(25, CTL_dWARNING, "230008" );
			return BD_ERROR;
		}

		if( g_ioMgr.get_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum_fwdback, IO_OFF) == IO_ON ||
			g_ioMgr.get_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum_fwdback, IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_right_btm_vacuum_bwd_chk, IO_ON) == IO_OFF )
		{
			CString strError;
			if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk, IO_ON) == IO_OFF)
			{
				strError.Format("5%04d%d",st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, strError );
			}
			else
			{
				strError.Format("5%04d%d",st_io.i_distribute_rbt2_right_btm_vacuum_bwd_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(5025, CTL_dWARNING, strError );
			}
			return BD_ERROR;
		}

		//2015.0206
		if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk, IO_ON) == IO_OFF )
		{
			CString strError;
			if( g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk, IO_ON) == IO_OFF)
			{
				strError.Format("5%04d%d",st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk, IO_OFF);
				CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, strError );
			}
			return BD_ERROR;
		}


		break;

	case M_BCR_ATTACH2_Z1:
		{
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_ON);

			//3:Jog
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] - 50 - COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1994, CTL_dWARNING, "230008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1994, BCR_ATTACH2_Z1 Place Pos, ATTACH2_Z1 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) >= st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + 50 +  COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1995, CTL_dWARNING, "230008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1995, BCR_ATTACH2_Z1 Pick Pos, ATTACH2_Z1 value is too low  -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			
			if( d_targetpos == st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_PICK] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3820, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt2_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3821, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3822, CTL_dWARNING, strJamCode );
					return BD_ERROR;
				}
				else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					CString strErr;
					strErr.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3823, CTL_dWARNING, strErr );
					return BD_ERROR;
					
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1611, CTL_dWARNING, "220008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_DOWN] )
			{
				if(nRet1 != IO_ON || nRet2 != IO_OFF) 
				{
					if(nRet1 != IO_ON)
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt2_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3824, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3825, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					CString strErr;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
						strErr.Format("5%04d%d", st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_OFF);
					else
						strErr.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(1612, CTL_dWARNING, strErr );
					return BD_ERROR;
					
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH2_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1613, CTL_dWARNING, "220008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_ERROR] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3826, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt2_box_rotator_bwd_chk[0]);
						CTL_Lib.Alarm_Error_Occurrence( 3827, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3828, CTL_dWARNING, strJamCode );
					return BD_ERROR;					
				}
				if( d_targetpos > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(3829, CTL_dWARNING, "230008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "BCR_ATTACH2_Z1 Error Pos, ATTACH2_Z1 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH2_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1614, CTL_dWARNING, "230008" );
					return BD_ERROR;
				}
			}
			if( (n_mode != 3 && n_mode != 0 ) && (d_targetpos != st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_PICK] &&
				d_targetpos != st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_DOWN] && 
				d_targetpos != st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_ERROR] && 
				d_targetpos != st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY] && 
				d_targetpos != st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_UP] ) )
			{
				CTL_Lib.Alarm_Error_Occurrence(3992, CTL_dWARNING, "230008" );
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "3992, BCR_ATTACH2_Z1 Position error, Z1 value is No Position -> target position: [%4.3f]", d_targetpos);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return BD_ERROR;
			}
			if( ( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH2_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH2_X] ) )
			{
				
			}
			else
			{
				if( n_mode != 0 )//2016.0926 3
				{
					CTL_Lib.Alarm_Error_Occurrence(253, CTL_dWARNING, "220008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "253, BCR_ATTACH2_Z1, BCR_ATTACH2_Y  Position error -> position: [%4.3f]", COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X));
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
		}
		break;
	case M_BCR_ATTACH2_R1:
		if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_Z1) > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] )
		{//230008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(26, CTL_dWARNING, "230008" );
			return BD_ERROR;
		}
		break;

	case M_BCR_ATTACH2_Z2:
		{
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_ON);

			//3:Jog
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] - 50 - COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH2_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1996, CTL_dWARNING, "250008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1996, BCR_ATTACH2_Z2 Place Pos, ATTACH2_Z1 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			if( n_mode != 3 && COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) >= st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + 50 +  COMI.md_allow_value[M_BCR_ATTACH1_X] )
			{
				if( d_targetpos > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence(1997, CTL_dWARNING, "250008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "1997, BCR_ATTACH2_Z2 Pick Pos, ATTACH2_Z1 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
			
			if( d_targetpos == st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_PICK] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3830, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt2_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3831, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3832, CTL_dWARNING, strJamCode );
					return BD_ERROR;
				}
				else if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					CString strErr;
					strErr.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3833, CTL_dWARNING, strErr );
					return BD_ERROR;
					
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1615, CTL_dWARNING, "220008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_DOWN] )
			{
				if(nRet1 != IO_ON || nRet2 != IO_OFF) 
				{
					if(nRet1 != IO_ON)
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt2_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3834, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3835, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON) == IO_OFF ||
					g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					CString strErr;
					if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
						strErr.Format("5%04d%d", st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_OFF);
					else
						strErr.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(1616, CTL_dWARNING, strErr );
					return BD_ERROR;
					
				}

				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH2_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1617, CTL_dWARNING, "220008" );
					return BD_ERROR;
				}
			}
			if( d_targetpos == st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_ERROR] )
			{
				if(nRet1 != IO_OFF || nRet2 != IO_ON) 
				{
					if(nRet1 != IO_OFF)
					{
						strJamCode.Format("5%04d0",st_io.i_distribute_rbt2_box_rotator_fwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3836, CTL_dWARNING, strJamCode );
					}
					else
					{
						strJamCode.Format("5%04d1",st_io.i_distribute_rbt2_box_rotator_bwd_chk[1]);
						CTL_Lib.Alarm_Error_Occurrence( 3837, CTL_dWARNING, strJamCode );
					}
				}
				if( g_ioMgr.get_in_bit( st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_ON) == IO_OFF )
				{
					strJamCode.Format("5%04d%d", st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_OFF);
					CTL_Lib.Alarm_Error_Occurrence(3838, CTL_dWARNING, strJamCode );
					return BD_ERROR;					
				}
				if( d_targetpos > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_DOWN] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] )
				{
					CTL_Lib.Alarm_Error_Occurrence(3839, CTL_dWARNING, "250008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "3839, BCR_ATTACH2_Z2 Error Pos, ATTACH2_Z2 value is too low -> target position: [%4.3f]", d_targetpos);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
				if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
					COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH2_X] )
				{
					
				}
				else
				{
					CTL_Lib.Alarm_Error_Occurrence(1618, CTL_dWARNING, "250008" );
					return BD_ERROR;
				}
			}
			if( (n_mode != 3 && n_mode != 0 ) && (d_targetpos != st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_PICK] &&
				d_targetpos != st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_DOWN] && 
				d_targetpos != st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_ERROR] && 
				d_targetpos != st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY] && 
				d_targetpos != st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_UP] ) )
			{
				CTL_Lib.Alarm_Error_Occurrence(3993, CTL_dWARNING, "250008" );
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "3993, BCR_ATTACH2_Z2 Position error, ATTACH2_Z2 value is No Position -> target position: [%4.3f]", d_targetpos);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return BD_ERROR;
			}
			if( ( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK] + COMI.md_allow_value[M_BCR_ATTACH2_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + COMI.md_allow_value[M_BCR_ATTACH2_X] ) ||
				( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) > st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] - COMI.md_allow_value[M_BCR_ATTACH2_X] &&
				COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X) < st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH2_X] ) )
			{
				
			}
			else
			{
				if( n_mode != 0 )//2016.0926 4
				{
					CTL_Lib.Alarm_Error_Occurrence(263, CTL_dWARNING, "220008" );
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "263, BCR_ATTACH2_Z2, BCR_ATTACH2_Y  Position error -> position: [%4.3f]", COMI.Get_MotCurrentPos(M_BCR_ATTACH2_X));
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					return BD_ERROR;
				}
			}
		}
		break;
	
	case M_BCR_ATTACH2_R2:
		if( COMI.Get_MotCurrentPos(M_BCR_ATTACH2_Z2) > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z2])
		{//180008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(27, CTL_dWARNING, "230008" );
			return BD_ERROR;
		}
		break;

	case M_TOP_VISION_X:
		break;
	case M_BTM_VISION_X:
		if( COMI.Get_MotCurrentPos(M_ULD_Z) > st_motor[M_ULD_Z].d_pos[P_ULD_Z_VIS_POS] + COMI.md_allow_value[M_ULD_Z] && st_handler.mn_virtual_mode == 0 )
		{//300008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(28, CTL_dWARNING, "300008" );
			return BD_ERROR;
		}		
		break;

	case M_ULD_Y:
		if( COMI.Get_MotCurrentPos(M_ULD_Z) > st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP] + COMI.md_allow_value[M_ULD_Z] && st_handler.mn_virtual_mode == 0 )
		{//300008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
			CTL_Lib.Alarm_Error_Occurrence(29, CTL_dWARNING, "300008" );
			return BD_ERROR;
		}
		if( (g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_up_chk, IO_ON) == IO_OFF || g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_down_chk, IO_OFF) == IO_ON) && st_handler.mn_virtual_mode == 0 )
		{
			sprintf(cJamcode, "5%04d%d", st_io.i_uld_rbt_left_up_chk, IO_OFF);
			Alarm_Error_Occurrence(30, CTL_dWARNING, cJamcode);
			return BD_ERROR;
		}
		if( (g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_up_chk, IO_ON) == IO_OFF || g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_down_chk, IO_OFF) == IO_ON) && st_handler.mn_virtual_mode == 0 )
		{
			sprintf(cJamcode, "5%04d%d", st_io.i_uld_rbt_right_up_chk, IO_OFF);
			Alarm_Error_Occurrence(31, CTL_dWARNING, cJamcode);
			return BD_ERROR;
		}
		break;

	case M_ULD_Z:
		if( d_targetpos == st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_POS] ||
			d_targetpos == st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_READY_POS] )
		{
			if( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS] - COMI.md_allow_value[M_ULD_Y] &&
				COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS] + COMI.md_allow_value[M_ULD_Y] )
			{

			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(29, CTL_dWARNING, "300008" );
				return BD_ERROR;
			}
		}
		if( d_targetpos == st_motor[M_ULD_Z].d_pos[P_ULD_Z_CONV_POS] )
		{
			if( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS] - COMI.md_allow_value[M_ULD_Y] &&
				COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS] + COMI.md_allow_value[M_ULD_Y] )
			{
				
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(2929, CTL_dWARNING, "300008" );
				return BD_ERROR;
			}
		}
		if( d_targetpos == st_motor[M_ULD_Z].d_pos[P_ULD_Z_REJ_POS] )
		{
			if( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] - COMI.md_allow_value[M_ULD_Y] &&
				COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] + COMI.md_allow_value[M_ULD_Y] )
			{
				
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(2930, CTL_dWARNING, "300008" );
				return BD_ERROR;
			}
		}
		if( (n_mode != 3 && n_mode != 0 ) && (d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_SAFETY] &&
			d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP] && 
			d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_POS] &&
			d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_VIS_POS] &&
			d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_CONV_POS] &&
			d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_REJ_POS] &&
			d_targetpos != st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_READY_POS] ) )
		{
			CTL_Lib.Alarm_Error_Occurrence(3999, CTL_dWARNING, "180008" );
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "3999, M_ULD_Y Position error -> target position: [%4.3f]", d_targetpos);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			return BD_ERROR;
		}

		if( ( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS] - COMI.md_allow_value[M_ULD_Y] &&
			  COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS] + COMI.md_allow_value[M_ULD_Y] ) ||
			( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS] - COMI.md_allow_value[M_ULD_Y] &&
			  COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS] + COMI.md_allow_value[M_ULD_Y] ) ||
			( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS] - COMI.md_allow_value[M_ULD_Y] &&
			  COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS] + COMI.md_allow_value[M_ULD_Y] ) ||
			( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] - COMI.md_allow_value[M_ULD_Y] &&
			  COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] + COMI.md_allow_value[M_ULD_Y] ) )
		{
			
		}
		else
		{
			if( d_targetpos > ( st_motor[M_ULD_Z].d_pos[P_ULD_Z_SAFETY] + COMI.md_allow_value[M_ULD_Z] ) )
			{
				CTL_Lib.Alarm_Error_Occurrence(2939, CTL_dWARNING, "300008" );
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "2939, M_ULD_Y  Position error -> position: [%4.3f]", COMI.Get_MotCurrentPos(M_ULD_Y));
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				return BD_ERROR;
			}
		}
		break;

	case M_ULD_CV_PITCH:
		break;
	case M_IDBUFFER_EL:

		if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_pusher_fwd_chk, IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_pusher_bwd_chk, IO_ON) == IO_OFF)
		{
			CString strError;
			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_pusher_fwd_chk, IO_OFF) == IO_ON)
				strError.Format("5%04d%d",st_io.i_idbuffer_elv_pusher_fwd_chk, IO_ON);
			else
				strError.Format("5%04d%d",st_io.i_idbuffer_elv_pusher_bwd_chk, IO_OFF);

			CTL_Lib.Alarm_Error_Occurrence( 89, dWARNING, strError );
			COMI.Set_MotStop( true, M_IDBUFFER_EL);
			return BD_ERROR;
		}

		if( COMI.Get_MotCurrentPos(M_TOPID_LD2) > st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_SAFETY] + COMI.md_allow_value[M_TOPID_LD2] && st_handler.mn_virtual_mode == 0 )
		{//340008 0 00 "Top ID Buffr LD2 Axis Motor is Non Safety Pos."
			if( d_targetpos > st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER] + COMI.md_allow_value[M_IDBUFFER_EL] ||
				(COMI.Get_MotCurrentPos(M_IDBUFFER_EL) > st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER] + COMI.md_allow_value[M_IDBUFFER_EL]) )
			{
				CTL_Lib.Alarm_Error_Occurrence(25, CTL_dWARNING, "340008");
				COMI.Set_MotStop( 30, M_IDBUFFER_EL);
				return BD_ERROR;
			}

		}
		if( COMI.Get_MotCurrentPos(M_BTMID_ULD2) > st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + COMI.md_allow_value[M_BTMID_ULD1] && st_handler.mn_virtual_mode == 0 )
		{//340008 0 00 "Bottom ID Buffr ULD4 Axis Motor is Non Safety Pos."
//			st_io.i_btm_idbuffer_uld1_shifter_hook_fwd_chk						= 3306;
//			st_io.i_btm_idbuffer_uld1_shifter_hook_back_chk						= 3307;

			if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer_uld1_shifter_hook_fwd_chk, IO_OFF) == IO_ON ||
				g_ioMgr.get_in_bit(st_io.i_btm_idbuffer_uld1_shifter_hook_back_chk, IO_ON) == IO_OFF)
			{
				CString strError;
				strError.Format("5%04d%%d",st_io.i_btm_idbuffer_uld1_shifter_hook_fwd_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(25, CTL_dWARNING, strError);
				COMI.Set_MotStop( 31, M_IDBUFFER_EL);
				return BD_ERROR;
			}
//			CTL_Lib.Alarm_Error_Occurrence(26, CTL_dWARNING, "340008");
		}
		if(COMI.Get_MotCurrentPos(M_BTMID_ULD2) > st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_PICK] + COMI.md_allow_value[M_BTMID_ULD2])
		{
			CString strError;
			strError.Format("%02d0008",M_BTMID_ULD2);
			CTL_Lib.Alarm_Error_Occurrence(26, CTL_dWARNING, strError);
			COMI.Set_MotStop( 32, M_IDBUFFER_EL);
			return BD_ERROR;

		}
		break;

	case M_TOPID_LD1://2014.1018
////2015.0705
// 		if( ( COMI.Get_MotCurrentPos( M_D_ATTACH_Y ) > (st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY] - COMI.md_allow_value[M_D_ATTACH_Y]) ) &&
// 			( COMI.Get_MotCurrentPos( M_D_ATTACH_Y ) < (st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY] + COMI.md_allow_value[M_D_ATTACH_Y]) ) )
		if( COMI.Get_MotCurrentPos( M_D_ATTACH_Y ) < st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY] + 50 + COMI.md_allow_value[M_D_ATTACH_Y] )
		{
			if( COMI.Get_MotCurrentPos( M_D_ATTACH_Z1) > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z1] ||
				COMI.Get_MotCurrentPos( M_D_ATTACH_Z2) > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z2] )
			{
				if( d_targetpos < st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_PICK] - COMI.md_allow_value[M_TOPID_LD1])
				{
					CTL_Lib.Alarm_Error_Occurrence( 1905, dWARNING, "120008");
					return BD_ERROR;
				}
			}
		}
////
		if( d_targetpos < st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_PICK] - COMI.md_allow_value[M_TOPID_LD1])
		{

			if( COMI.Get_MotCurrentPos( M_TOPID_LD2 ) < st_motor[M_TOPID_LD2].d_pos[P_ROLLER_SAGETY] + COMI.md_allow_value[M_TOPID_LD2] && st_handler.mn_virtual_mode == 0 )
			{//340008 0 00 "Top ID Buffr LD2 Axis Motor is Non Safety Pos."
				if( g_ioMgr.get_in_bit( st_io.i_top_idbuffer_ld2_shifter_left_hook_clamp_chk, IO_OFF) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_ON) == IO_OFF)	//2014.1018			
				{
					CTL_Lib.Alarm_Error_Occurrence(37, CTL_dWARNING, "340008");
					return BD_ERROR;
				}
			}
		}


		if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_clamp[0]) == IO_ON || 
			g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_unclamp[0]) == IO_OFF )
		{
			if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_clamp[0]) == IO_ON || g_ioMgr.get_in_bit(st_io.i_top_idbuffer_unclamp[0]) == IO_OFF )
			{
				CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[0], IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 1901, dWARNING, strJamCode );
			}
// 			if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_clamp[1]) == IO_ON || g_ioMgr.get_in_bit(st_io.i_top_idbuffer_unclamp[1]) == IO_OFF )
// 			{				
// 				CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[1], IO_ON );
// 				CTL_Lib.Alarm_Error_Occurrence( 1902, dWARNING, strJamCode );
// 			}
		}

		break;
	case M_TOPID_LD2:
		if( COMI.Get_MotCurrentPos( M_BCR_ATTACH1_X ) < st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE] + COMI.md_allow_value[M_BCR_ATTACH1_X] )
		{
			int nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_ON); 
			int nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk, IO_ON); 
			if( nRet_1 != IO_ON || nRet_2 != IO_ON || d_targetpos == st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD])
			{
				if( COMI.Get_MotCurrentPos( M_BCR_ATTACH1_Z1) > st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z1] ||
					COMI.Get_MotCurrentPos( M_BCR_ATTACH1_Z2) > st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH1_Z2] )
				{
					CTL_Lib.Alarm_Error_Occurrence( 1903, dWARNING, "170008");
					return BD_ERROR;
				}
			}
		}
		if(	COMI.Get_MotCurrentPos( M_BCR_ATTACH2_X ) < ( st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_REJECT] + 50 + COMI.md_allow_value[M_BCR_ATTACH2_X] ) )
		{
			if( COMI.Get_MotCurrentPos( M_BCR_ATTACH2_Z1) > st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z1] ||
				COMI.Get_MotCurrentPos( M_BCR_ATTACH2_Z2) > st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_SAFETY] + COMI.md_allow_value[M_BCR_ATTACH2_Z2] )
			{
				if( d_targetpos >= st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD])
				{
					CTL_Lib.Alarm_Error_Occurrence( 1903, dWARNING, "220008");
					return BD_ERROR;
				}
			}
		}

// 		if( COMI.Get_MotCurrentPos( M_D_ATTACH_Y ) < st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY] - COMI.md_allow_value[M_D_ATTACH_Y] )
// 		{
// 			if( COMI.Get_MotCurrentPos( M_D_ATTACH_Z1) > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z1] ||
// 				COMI.Get_MotCurrentPos( M_D_ATTACH_Z2) > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z2] )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1904, dWARNING, "120008");
// 				return BD_ERROR;
// 			}
// 		}
		
		if( COMI.Get_MotCurrentPos( M_D_ATTACH_Y ) < st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY] + 50 + COMI.md_allow_value[M_D_ATTACH_Y] )
		{
			if( COMI.Get_MotCurrentPos( M_D_ATTACH_Z1) > st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z1] ||
				COMI.Get_MotCurrentPos( M_D_ATTACH_Z2) > st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY] + COMI.md_allow_value[M_D_ATTACH_Z2] )
			{
				if( d_targetpos >= st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD])
				{
					CTL_Lib.Alarm_Error_Occurrence( 1984, dWARNING, "120008");
					return BD_ERROR;
				}
			}
		}		

		if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_clamp[1]) == IO_ON || 
			g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_unclamp[1]) == IO_OFF )
		{
			if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_clamp[0]) == IO_ON || g_ioMgr.get_in_bit(st_io.i_top_idbuffer_unclamp[0]) == IO_OFF )
			{
				CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[0], IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 1901, dWARNING, strJamCode );
				return BD_ERROR;
			}
			if( COMI.Get_MotCurrentPos(M_TOPID_LD1) < st_motor[M_TOPID_LD1].d_pos[P_LDBUFFER_PICK] - COMI.md_allow_value[M_TOPID_LD1] &&
				COMI.Get_MotCurrentPos(M_TOPID_LD2) <= st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK] + COMI.md_allow_value[M_TOPID_LD2] )
			{
				if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_clamp[0]) == IO_ON || 
					g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_unclamp[0]) == IO_OFF )
				{				
					CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[0], IO_ON );
					CTL_Lib.Alarm_Error_Occurrence( 1902, dWARNING, strJamCode );
					return BD_ERROR;
				}
			}

		}
		if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_clamp[2]) == IO_ON || 
			g_ioMgr.get_in_bit(st_io.i_top_idbuffer_shifter_hook_unclamp[2]) == IO_OFF )
		{
			if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_clamp[2]) == IO_ON || g_ioMgr.get_in_bit(st_io.i_top_idbuffer_unclamp[2]) == IO_OFF )
			{
				CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[2], IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 1903, dWARNING, strJamCode );
				return BD_ERROR;
			}
			if( g_ioMgr.get_in_bit(st_io.i_top_idbuffer_clamp[3]) == IO_ON || g_ioMgr.get_in_bit(st_io.i_top_idbuffer_unclamp[3]) == IO_OFF )
			{				
				CString strJamCode = GetJamCode_Load_Clamp_Time(st_io.i_top_idbuffer_clamp[3], IO_ON );
				CTL_Lib.Alarm_Error_Occurrence( 1904, dWARNING, strJamCode );
				return BD_ERROR;
			}			
		}

		if( COMI.Get_MotCurrentPos(M_INDEX_ROLLER1) > st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY] + COMI.md_allow_value[M_INDEX_ROLLER1] && st_handler.mn_virtual_mode == 0 )
		{//380008 0 00 "Index Rolling1 Axis Motor is Non Safety Pos."
			if( d_targetpos >= st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD])
			{
				CTL_Lib.Alarm_Error_Occurrence(38, CTL_dWARNING, "380008");
				return BD_ERROR;
			}
		}
		if( COMI.Get_MotCurrentPos(M_INDEX_ROLLER2) > st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_SAGETY] + COMI.md_allow_value[M_INDEX_ROLLER1] && st_handler.mn_virtual_mode == 0 )
		{//390008 0 00 "Index Rolling2 Axis Motor is Non Safety Pos."
			if( d_targetpos >= st_motor[ M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD])
			{
				CTL_Lib.Alarm_Error_Occurrence(3339, CTL_dWARNING, "390008");
				return BD_ERROR;
			}
		}
		if(d_targetpos > st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK] + 100)
		{
			if( COMI.Get_MotCurrentPos( M_IDBUFFER_EL) < st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER] - COMI.md_allow_value[M_IDBUFFER_EL] )
			{
				CTL_Lib.Alarm_Error_Occurrence(40, CTL_dWARNING, "320008");
				return BD_ERROR;
			}
			else
			{
				if( g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_hook_clamp_chk) == IO_ON ||
					g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_hook_unclamp_chk) == IO_OFF )
				{
					CString strError;
					strError.Format("5%04d%d",st_io.i_idbuffer_elv_hook_unclamp_chk, IO_ON);
					CTL_Lib.Alarm_Error_Occurrence(3439, CTL_dWARNING, strError);
					return BD_ERROR;
				}
			}
		}
		if( d_targetpos > st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK] + 100 &&
			(g_ioMgr.get_in_bit(st_io.i_top_left_box_supportup_chk, IO_OFF) == IO_ON || 
			g_ioMgr.get_in_bit(st_io.i_top_left_box_supportdn_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit(st_io.i_top_right_box_supportup_chk, IO_OFF) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_top_right_box_supportdn_chk, IO_ON) == IO_OFF) )
		{
			CString strError;
			if(st_io.i_top_left_box_supportup_chk == IO_ON)
			{
				strError.Format("5%04d%d", st_io.i_top_left_box_supportup_chk, IO_ON);
			}
			else if(st_io.i_top_left_box_supportdn_chk == IO_OFF)
			{
				strError.Format("5%04d%d", st_io.i_top_left_box_supportdn_chk, IO_OFF);
			}
			else if(st_io.i_top_right_box_supportup_chk == IO_ON)
			{
				strError.Format("5%04d%d", st_io.i_top_right_box_supportup_chk, IO_ON);
			}
			else
			{
				strError.Format("5%04d%d", st_io.i_top_right_box_supportdn_chk, IO_OFF);
			}
// 			COMI.Set_MotStop( 0, M_TOPID_LD2 );

			CTL_Lib.Alarm_Error_Occurrence(3459, CTL_dWARNING, strError);
			return BD_ERROR;

		}
		nRet1 = COMI.Check_MotPosRange(M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS], COMI.md_allow_value[M_ULD_Y]);
		if( nRet1 == BD_GOOD && d_targetpos > st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD] - 10)
		{
			if( COMI.Get_MotCurrentPos(M_ULD_Z) > st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_READY_POS] + COMI.md_allow_value[M_ULD_Z] )
			{//300008 0 00 "Unload Robot Z Axis Motor is Non Safety Pos.
				CTL_Lib.Alarm_Error_Occurrence(98, CTL_dWARNING, "300008" );
				return BD_ERROR;
			}
		}

		break;
	case M_BTMID_ULD1:
		if(d_targetpos > (st_motor[M_BTMID_ULD1].d_pos[P_ULDBUFFER_PICK] + COMI.md_allow_value[M_BTMID_ULD1]) )
		{
			if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer34_clamp_chk) == IO_ON || 
				g_ioMgr.get_in_bit(st_io.i_btm_idbuffer34_unclamp_chk) == IO_OFF )
			{
				CString strError;
				strError.Format("5%04d%d",st_io.i_btm_idbuffer34_clamp_chk, IO_ON);						
				COMI.Set_MotStop(0, M_BTMID_ULD1);
				CTL_Lib.Alarm_Error_Occurrence(441, CTL_dWARNING, strError);
				return BD_ERROR;
			}
		}

		break;
	case M_BTMID_ULD2:
		{
			int iHook_Clamp_1 = st_io.i_btm_idbuffer_shifter_hook_fwd[1];
			int iHook_UnClamp_1 = st_io.i_btm_idbuffer_shifter_hook_bwd[1];
			int iHook_Clamp_2 = st_io.i_btm_idbuffer_shifter_hook_fwd[2];
			int iHook_UnClamp_2 = st_io.i_btm_idbuffer_shifter_hook_bwd[2];
			if( /*(COMI.Get_MotCurrentPos(M_BTMID_ULD2) > st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + 100) &&*/
				(d_targetpos < st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + COMI.md_allow_value[M_BTMID_ULD2]) )
			{
	// 			st_io.i_btm_idbuffer_uld2_shifter_hook_fwd1_chk						= 3404;
	// 			st_io.i_btm_idbuffer_uld2_shifter_hook_back1_chk					= 3405;
	// 			st_io.i_btm_idbuffer_uld2_shifter_hook_fwd2_chk						= 3406;
	// 			st_io.i_btm_idbuffer_uld2_shifter_hook_back2_chk					= 3407;

				if( g_ioMgr.get_in_bit(iHook_Clamp_1) == IO_ON || g_ioMgr.get_in_bit(iHook_UnClamp_1) == IO_OFF ||
					g_ioMgr.get_in_bit(iHook_Clamp_2) == IO_ON || g_ioMgr.get_in_bit(iHook_UnClamp_2) == IO_OFF )
				{
					CString strError;
					if( g_ioMgr.get_in_bit(iHook_Clamp_1) == IO_ON || g_ioMgr.get_in_bit(iHook_UnClamp_1) == IO_OFF )
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_shifter_hook_fwd[1], IO_ON);
					else
						strError.Format("5%04d%d",st_io.i_btm_idbuffer_shifter_hook_fwd[2], IO_ON);

					COMI.Set_MotStop(0, M_BTMID_ULD2);
					CTL_Lib.Alarm_Error_Occurrence(41, CTL_dWARNING, strError);
					return BD_ERROR;
				}
			}

			if( (COMI.Get_MotCurrentPos(M_BTMID_ULD2) > st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + COMI.md_allow_value[M_BTMID_ULD2] ) &&
				(d_targetpos < st_motor[M_BTMID_ULD2].d_pos[P_ULDBUFFER_SAFETY] + COMI.md_allow_value[M_BTMID_ULD2]) &&
				(g_ioMgr.get_in_bit( st_io.i_top_idbuffer_ld1_shifter_hook_clamp_chk, IO_OFF) == IO_ON ||
				 g_ioMgr.get_in_bit( st_io.i_top_idbuffer_ld1_shifter_hook_unclamp_chk, IO_ON) == IO_OFF) ) 
			{
				if( g_ioMgr.get_in_bit(st_io.i_btm_idbuffer12_clamp_chk) == IO_ON || 
					g_ioMgr.get_in_bit(st_io.i_btm_idbuffer12_unclamp_chk) == IO_OFF )
				{
					if( COMI.mn_run_status != dINIT )//2015.0825
					{
						CString strError;
						strError.Format("5%04d%d",st_io.i_btm_idbuffer12_clamp_chk, IO_ON);						
						COMI.Set_MotStop(0, M_BTMID_ULD2);
						CTL_Lib.Alarm_Error_Occurrence(42, CTL_dWARNING, strError);
						return BD_ERROR;
					}
				}
			}			

		}

		break;
	case M_INDEX_X:
		if( COMI.Get_MotCurrentPos(M_INDEX_ROLLER1) > ( st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY] + COMI.md_allow_value[M_INDEX_ROLLER1] ) ||
			COMI.Get_MotCurrentPos(M_INDEX_ROLLER2) > ( st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_SAGETY] + COMI.md_allow_value[M_INDEX_ROLLER2] ) )
		{
			if( COMI.Get_MotCurrentPos(M_INDEX_ROLLER1) > ( st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY] + COMI.md_allow_value[M_INDEX_ROLLER1] ) )
			{//380008 0 A "Index_Rolling1_Axis_Motor_is_Non_Safety_Pos."
				CTL_Lib.Alarm_Error_Occurrence(8142, CTL_dWARNING, "380008");
				sprintf(st_msg.c_abnormal_msg, "8142, [M_INDEX_X] M_INDEX_X can't move because of M_INDEX_ROLLER1.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

				return BD_ERROR;
			}
			else
			{//390008 0 A "Index_Rolling2_Axis_Motor_is_Non_Safety_Pos."
				CTL_Lib.Alarm_Error_Occurrence(8143, CTL_dWARNING, "390008");
				sprintf(st_msg.c_abnormal_msg, "8143, [M_INDEX_X] M_INDEX_X can't move because of M_INDEX_ROLLER2.");
				return BD_ERROR;

			}
		}
		
		break;

	case M_INDEX_ROLLER1:
		{
			int aa = 10;
		}
		break;
	case M_INDEX_ROLLER2:
		break;
	case M_ULD_ROLLER_PITCH:
		break;
	case M_ULD_STACKER_Z:
		break;
	}

	st_motor[n_axis].mn_retry_time_flag = CTL_NO;//2011.0201 추가 

	return BD_GOOD;
}

BOOL CCtlBd_Library::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	CString strTemp;
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

int CCtlBd_Library::FileSizeCheck(char * cp_filename, int n_size, int n_check)
{
	CFileFind finder;
	long nLen=0, nflag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(cp_filename))
	{	
		if (n_check == BD_YES)			// Size를 체크하는것이면...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			if(nLen > n_size)
			{
				nflag = 1;
			}
			finder.Close();	
		}
		else						// Size를 확인하는것이면...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			
			nflag = nLen;
			
			finder.Close();	
		}
	}
	
	return nflag;
}

int CCtlBd_Library::LogFile_Write(char * cp_dir_path, char * cp_msg)
{
//2011.0417	sing.Lock(); //2011.0417 

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name, mstr_dir_path, str_msg;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// 파일 존재 유무 설정 플래그 
	char cfileName[256]={0,};			// 검색할 파일 정보 설정 함수 
	FILE  *fp ;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //저장할 메세지가 없다 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime() ;  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// 현재 시간 정보를 설정한다. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");	// 리스트 파일에 출력할 타이틀 시간 정보 설정 
	// **************************************************************************


	mstr_dir_path = cp_dir_path;
	mstr_file_name = mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = mstr_dir_path + mstr_file_name;

	mstr_create_file += ".TXT";

	sprintf(cfileName, "%s", mstr_create_file);
	mn_existence = access(cfileName,0) ;	
	if (mn_existence == -1)
	{//해당 파일이 존재하지 않는 상태이다 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//해당 파일이 이미 존재한다.
		//정상 
	}
	
	Ret = FileSizeCheck(cfileName, 1048576, BD_YES); //size and rename	
	if(Ret == 1) //file size over
	{
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		//AfxMessageBox("File open failure!..");
		sprintf(mc_normal_msg,"LogFile_Write a+ Error"); 
		
 		return BD_ERROR;
	}
	// **************************************************************************
	
	mstr_content += str_display_time;
	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content) ;
	if (ferror(fp))  
	{
		sprintf(mc_normal_msg,"LogFile_Write ferror return error");

		clearerr(fp); //2011.0418 

		fclose(fp) ;  //2011.0417  파일 종료

//2011.0417		sing.Unlock(); //2011.0417 추가 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // 파일을 종료한다.

//2011.0417	sing.Unlock(); //2011.0417 추가 
	return BD_GOOD;
}

int CCtlBd_Library::Single_Move(int n_MotNum, double d_MotPos, int n_SpeedRate)
{
	int nFuncRet = BD_PROCEED;
	int nRet;

// 	if(n_MotNum == 17 && mn_single_motmove_step[n_MotNum] == 200)
// 	{
// 		nRet = 1;
// 	}
	
	switch(mn_single_motmove_step[n_MotNum])
	{
	case 0:
		nRet = COMI.Check_MotPosRange(n_MotNum, d_MotPos, COMI.md_allow_value[n_MotNum]);
		if (nRet == BD_GOOD) //이미 해당 위치에 와 있으면 동작하지 않고 모터 이동을 끝낸다 
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_mot_retry_cnt[n_MotNum] = 0;
		mn_mot_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
		mn_single_motmove_step[n_MotNum] = 100;
		break;
		
	case 100:
		if(COMI.mn_run_status == dRUN) st_work.md_reinstatement_Pos[0][n_MotNum] = d_MotPos;
		nRet = COMI.Start_SingleMove(n_MotNum, d_MotPos, n_SpeedRate);  // 해당 위치로 이동
		if (nRet == BD_GOOD)
		{
			mn_single_motmove_step[n_MotNum] = 200;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = nRet;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet == BD_RETRY)  // 동작 재시도
		{
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
		
	case 200:
		nRet = COMI.Check_SingleMove(n_MotNum, d_MotPos);  // 이동 완료 확인
		if (nRet == BD_GOOD)  // 정상 완료
		{
			mn_single_motmove_step[n_MotNum] = 0;
			nFuncRet = nRet;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = nRet;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet == BD_RETRY)  // 동작 재시도
		{
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없다
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
	}
	return nFuncRet;
}

//2015.0326 james 수정 
int CCtlBd_Library::Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio)
{
	int nFuncRet = BD_PROCEED;
	int nRet[4] = {0,}, nCnt =0, i=0;
	
	switch(mn_linear_motmove_step[n_LinearMapIndex])
	{
	case 0:

		nCnt = 0;
		st_linearmot[n_LinearMapIndex].l_AxisCnt = l_AxisCnt;
		COMI.ml_axiscnt[n_LinearMapIndex] = l_AxisCnt; //2015.0326 james 라이브러리의 변수와 동기가 필요하니 변수 셋팅해줌 
		for(i=0; i<l_AxisCnt; i++)
		{		
			COMI.mp_axisnum[n_LinearMapIndex][i] = lp_AxisNum[i]; //2015.0326 james 라이브러리의 변수와 동기가 필요하니 변수 셋팅해줌 
			st_linearmot[n_LinearMapIndex].lp_AxisNum[i] = lp_AxisNum[i]; 
			st_linearmot[n_LinearMapIndex].dp_PosList[i] = dp_PosList[i];

			nRet[i] = COMI.Check_MotPosRange(lp_AxisNum[i], dp_PosList[i], COMI.md_allow_value[lp_AxisNum[i]]);
			if(nRet[i] == BD_GOOD) nCnt++;
		}

		if(nCnt == l_AxisCnt)
		{//이미 모든 모터의 현재 위치가 이동하려는 위치와 허용오차 안에 있으면 동작할 필요가 없으니 끝내자.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
		mn_linear_motmove_step[n_LinearMapIndex] = 100;
		break;
		
	case 100:
		if(COMI.mn_run_status == dRUN)
		{
			st_work.md_reinstatement_Pos[0][lp_AxisNum[0]] = dp_PosList[0];
			st_work.md_reinstatement_Pos[0][lp_AxisNum[1]] = dp_PosList[1];
		}
		nRet[0] = COMI.Start_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList, dp_SpdRatio);
		if (nRet[0] == BD_GOOD)
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 200;
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
		{
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				//900000 1 A "Retry 회수 에러초과(3회) 에러."
				alarm.mstr_code = _T("900000");
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		break;
		
	case 200:
		nRet[0] = COMI.Check_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList) ;
		if (nRet[0] == BD_GOOD)  // 정상 완료
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 0;
			nFuncRet = nRet[0];
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
		{
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없다
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				//900000 1 A "Retry 회수 에러초과(3회) 에러."
				alarm.mstr_code = _T("900000");
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}	
		break;
	}
	return nFuncRet;
}

int CCtlBd_Library::SD_Sensor_Enable(int n_Mode, int n_AxisNum, int n_Enable)
{//Constant speed mode 에서는 SD 신호 입력이 무시됩니다.
	int nFuncRet = BD_PROCEED;
	
	if(n_Enable == CTL_YES) 
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmTRUE);  
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmFALSE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmTRUE);
		
		
	}
	else
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  
		//COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmTRUE);
		//COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmFALSE);
		
	}
	
	return BD_GOOD;
}