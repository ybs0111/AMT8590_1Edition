// CRun_Unload_Works.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Unload_Works.h"
#include "AMTRegistry.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "SrcPart/APartTestSite.h"

#include "VisionClient.h"
#include "VisionServer.h"
#include "AEC_Client.h"
#include "AMTLotManager.h"
#include "MesEvent.h"
#include "NextClient.h"
#include "XGemClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Unload_Works

CRun_Unload_Works g_Run_Unload_Works;

IMPLEMENT_SERIAL(CRun_Unload_Works, CObject, 1)

int CRun_Unload_Works::nPicker = 2;


CRun_Unload_Works::CRun_Unload_Works()
{
	Thread_Variable_Initial();
}

CRun_Unload_Works::~CRun_Unload_Works()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Unload_Works message handlers
void CRun_Unload_Works::Thread_Variable_Initial()
{
	int i = 0, j = 0, k = 0;
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;
	vecTact.clear();


	for ( i = 0; i < D_INFOSIZE; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			mn_PickerInfo[i][j] = CTL_NO;//동작시킬 디바이스를 선택
			mb_PickerUpDown_Flag[i] = CTL_NO;
		}
	}

	st_var.n_boatid_out_flag = CTL_NO;
	for( k = 0; k < 2; k++ )
	{
		m_bUse_idbuff[k] = false;
		m_strbcr_idbuff[k].Empty();
		m_strbcr_idbuff[k]	= "";
		m_strbcr_serial[k].Empty();
		m_strbcr_serial[k] = "";
		m_continus_vis_error[k] = 0;
	}
	m_nReq_MES = WORK_NONE_;
	m_nStep_MES = 0;
	mn_server_step = 0;
	st_handler.mn_unloadtact = 0;
	st_var.n_tbox_output_count = 0;

}

void CRun_Unload_Works::Thread_Run()
{
	int n_ret;
	if( Func.OnIsInterruptCondition() != 1 )
		return;
	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
// return;
		Run_Move();
		if( st_var.n_boatid_out_flag == CTL_YES )
		{
			n_ret = OnServerSendBoatID();
			if( n_ret == CTL_GOOD)
			{
				st_var.n_boatid_out_flag = CTL_NO;
			}

		}
		{
			for ( int i = 0; i < 2; i++)
			{
				Run_Move_BtmVision(i);
			}
		}
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

		for ( int i = 0; i < 2; i++)
		{
			Run_Move_Vac_Down(i);
			Run_Move_BtmVision(i);
		}

		Run_Move_Pusher_Buffer();

// 		Run_Unload_Move_Chk_Time();
// 		//for ( int j = 0; j < 2; j++)
// 		//{
// 			Run_Move_BtmVision(0);
// 		//}

}


void CRun_Unload_Works::Run_Initial()
{

	if( st_handler.mn_init_state[INIT_UNLOAD_ROBOT] != CTL_NO )	return;

	switch(mn_InitStep)
	{
	case EURI_START: mn_InitStep = EURI_CHECK_ROBOT_CHK; break;
	case EURI_CHECK_ROBOT_CHK: EURI_Check_Rbt_Chek(); break;
	case EURI_PUSHER_OFF:	EURI_Pusher_cyl_off(); break;
	case EURI_PUSHER_OFF_CHK:	EURI_Pusher_cyl_off_chk(); break;
	case EURI_HOMECHK_RBT_Z:	EURI_HomeCheck_Rbt_Z(); break;
	case EURI_VACUUM_ON:	 EURI_Vacuun_OnOff( IO_ON ); break;
	case EURI_UNLOAD_EMPTY_CHK: EURI_Mdl_Check_Rbt_Chek(); break;
	case EURI_VACUUM_OFF:	EURI_Vacuun_OnOff( IO_OFF ); break;

	case EURI_PICKER_OFF:
		{
// 			for ( int i = 0; i < 2; i++ )
// 			{
// 				mn_PickerInfo[D_EXIST][i] = CTL_YES;//동작시킬 디바이스를 선택
// 			}
// 			Set_PickerUpDown( IO_OFF, mn_PickerInfo);
// 			////2014.1018
// 			if(mb_PickerUpDown_Flag[0] == false && mb_PickerUpDown_Flag[1] == false)
// 			{
// 				mn_InitStep = EURI_PICKER_OFF_CHK;
// 			}
// 
			ReqVacDown( 0, BACKWARD );
			ReqVacDown( 1, BACKWARD );
			mn_InitStep = EURI_PICKER_OFF_CHK;
			////
		}
		break;
		
	case EURI_PICKER_OFF_CHK:
		{
// 			int nRet_1 = Get_PickerUpDown( IO_OFF, mn_PickerInfo);
// 			
// 			if (nRet_1 == RET_GOOD)
// 			{	
// 				mn_InitStep = EURI_HOMECHK_RBT_Y;
// 			}
// 			else if (nRet_1 == RET_ERROR)
// 			{
// 				//mn_InitStep = EURI_PICKER_OFF;
// 			}
			if( m_fbVacDown[0] != FB_NONE) break;
			if( m_fbVacDown[1] != FB_NONE) break;
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[0], IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[1], IO_OFF );
			mn_InitStep = EURI_HOMECHK_RBT_Y;
		}
		break;

	case EURI_HOMECHK_RBT_Y:	EURI_HomeCheck_Rbt_Y();	break;

	case EURI_HOMECHK_BTM_VISION_Y: EURI_HomeCheck_BtmVision_X();	break;

	case EURI_FINISH:	EURI_Finish();	break;
	}
}

void CRun_Unload_Works::EURI_Check_Rbt_Chek()
{
	if( st_handler.mn_init_state[INIT_TOP_VISION] == CTL_NO ) return;
	mn_InitStep = EURI_PUSHER_OFF;

}

void CRun_Unload_Works::EURI_Pusher_cyl_off()
{
	ReqPusherBuf(BACKWARD);
	mn_InitStep = EURI_PUSHER_OFF_CHK;
}

void CRun_Unload_Works::EURI_Pusher_cyl_off_chk()
{
	if( GetPusherBuf() == FB_NONE)
		mn_InitStep = EURI_HOMECHK_RBT_Z;
}


void CRun_Unload_Works::EURI_HomeCheck_Rbt_Z()
{
	int nRet = COMI.HomeCheck_Mot( M_ULD_Z, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EURI_VACUUM_ON;
		Func.OnInitBtn( RBTN_ULD_Z );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//300001 0 00 "Unload Robot Z Axis Motor Home Check Error."
		CTL_Lib.Alarm_Error_Occurrence(1200, CTL_dWARNING, "300001" );
	}
}

void CRun_Unload_Works::EURI_HomeCheck_Rbt_Y()
{
	int nRet = COMI.HomeCheck_Mot( M_ULD_Y, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EURI_HOMECHK_BTM_VISION_Y;
		Func.OnInitBtn( RBTN_ULD_Y );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//290001 0 00 "Unload Robot Y Axis Motor Home Check Error."
		CTL_Lib.Alarm_Error_Occurrence(1201, CTL_dWARNING, "290001" );
	}
}

void CRun_Unload_Works::EURI_HomeCheck_BtmVision_X()//
{
	int nRet = COMI.HomeCheck_Mot( M_BTM_VISION_X, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = EURI_FINISH;
		m_dwTimeTact = GetCurrentTime();
		Func.OnInitBtn( RBTN_BTM_VISION_X );//2014.1018
	}
	else if( nRet == BD_ERROR )
	{//280001 0 00 "Bottom Vision X Axis Motor Home Check Error."
		CTL_Lib.Alarm_Error_Occurrence(1202, CTL_dWARNING, "280001" );
	}
}

void CRun_Unload_Works::EURI_Vacuun_OnOff( int iOnOff )
{
	for ( int i = 0; i < 2; i++ )
	{
		g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[i], iOnOff );
		//2015.0417
		g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[i], !iOnOff );
	}

	if( iOnOff == IO_ON )
	{
		mn_InitStep = EURI_UNLOAD_EMPTY_CHK;
		m_dwTimeInit = GetCurrentTime();
	}
	else				  mn_InitStep = EURI_PICKER_OFF;
}

void CRun_Unload_Works::EURI_Mdl_Check_Rbt_Chek()
{
	if( GetCurrentTime() - m_dwTimeInit < 0 ) m_dwTimeInit = GetCurrentTime();
	if( GetCurrentTime() - m_dwTimeInit < 1000 ) return;

	if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[0] , IO_OFF) == IO_ON )
	{
		CString strError="";
		strError.Format("5%04d1",st_io.i_uld_rbt_vacuum_chk[0]);
		CTL_Lib.Alarm_Error_Occurrence( 1210, CTL_dWARNING, strError );
		return;
	}
	if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[1] , IO_OFF) == IO_ON )
	{
		CString strError="";
		strError.Format("5%04d1",st_io.i_uld_rbt_vacuum_chk[1]);
		CTL_Lib.Alarm_Error_Occurrence( 1211, CTL_dWARNING, strError );
		return;
	}

	mn_InitStep = EURI_VACUUM_OFF;
}

void CRun_Unload_Works::EURI_Finish()
{
	Func.OnInitBtn( RBTN_ULD );//2014.1018
	st_handler.mn_init_state[INIT_UNLOAD_ROBOT] = CTL_YES;
	mn_InitStep = EURI_START;
}

void CRun_Unload_Works::ReqPusherBuf( EFB efb )
{
	m_fbBufPusfer = efb;
	m_step_pusherbuf_fwdbwd = ESTEP_CYL_START;
}

void CRun_Unload_Works::Run_Move_Pusher_Buffer()
{
	if( m_fbBufPusfer == FB_NONE )	return;
	int iOnOff = IO_ON;
	if( m_fbBufPusfer == BACKWARD )
		iOnOff = IO_OFF;

	Func.ThreadFunctionStepTrace(64, m_step_pusherbuf_fwdbwd);
	switch( m_step_pusherbuf_fwdbwd )
	{
	case ESTEP_CYL_START:	m_step_pusherbuf_fwdbwd = ESTEP_CYL_MOVE; break;
	case ESTEP_CYL_MOVE:	
		{
			int iPush = st_io.o_idbuffer_elv_pusher_fwd;
			int iPull = st_io.o_idbuffer_elv_pusher_bwd;
			g_ioMgr.set_out_bit( iPush, iOnOff );
			g_ioMgr.set_out_bit( iPull, !iOnOff );
			
			if(COMI.mn_run_status == dRUN){
				st_work.mn_uld_pusher_buf[0] = iPush;
				st_work.mn_uld_pusher_buf[1] = iPull;
			}
			m_step_pusherbuf_fwdbwd = ESTEP_CYL_CHK;
			m_dwTimeBufPusherStart = GetCurrentTime();
		}
		break;
	
	case ESTEP_CYL_CHK:
		{
			int iPushChk = st_io.i_idbuffer_elv_pusher_fwd_chk;
			int iPullChk = st_io.i_idbuffer_elv_pusher_bwd_chk;

			if( st_handler.mn_virtual_mode == 1 || ( g_ioMgr.get_in_bit( iPushChk ) == iOnOff && g_ioMgr.get_in_bit( iPullChk ) != iOnOff ) )
			{
				m_step_pusherbuf_fwdbwd = ESTEP_CYL_FINISH;
				m_dwTimeBufPusher = GetCurrentTime();
				break;
			}
			if( GetCurrentTime() - m_dwTimeBufPusherStart > st_time.n_limit_time[E_WAIT_UNLOAD_PICKER] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
				{
					CString strJamCode = GetJamCode_VacuumDn_Time( iPushChk, iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 1230, dWARNING, (LPCSTR)strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeBufPusherStart < 0 )
			{
				m_dwTimeBufPusherStart = GetCurrentTime();
			}
		}
		break;
	
	case ESTEP_CYL_FINISH:
		if( GetCurrentTime() - m_dwTimeBufPusher < 0 )
		{
			m_dwTimeBufPusher = GetCurrentTime();
		}
		if( GetCurrentTime() - m_dwTimeBufPusher < st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][iOnOff] )
			return;

		m_fbBufPusfer = FB_NONE;
		m_step_pusherbuf_fwdbwd = ESTEP_CYL_START;
		break;
		
	}

}

void CRun_Unload_Works::ReqVacDown( int iIdx, EFB efb )
{
	m_fbVacDown[iIdx] = efb;
	m_step_vac_down[iIdx] = ESTEP_CYL_START;
}

void CRun_Unload_Works::Run_Move_Vac_Down( int iIdx )
{
	if( m_fbVacDown[iIdx] == FB_NONE )	return;
	int iOnOff = IO_ON;
	if( m_fbVacDown[iIdx] == BACKWARD )
		iOnOff = IO_OFF;

	Func.ThreadFunctionStepTrace(65+iIdx, m_step_vac_down[iIdx]);
	switch( m_step_vac_down[iIdx] )
	{
	case ESTEP_CYL_START:	m_step_vac_down[iIdx] = ESTEP_CYL_MOVE; break;
	case ESTEP_CYL_MOVE:	
		{
			int iDown = st_io.o_uld_rbt_picker[iIdx];
			g_ioMgr.set_out_bit( iDown, iOnOff );
			
			if(COMI.mn_run_status == dRUN) st_work.mn_uld_picker_updn[iIdx] = iOnOff;
			m_step_vac_down[iIdx] = ESTEP_CYL_CHK;
			m_dwTimeVacDownStart[iIdx] = GetCurrentTime();
		}
		break;

	case ESTEP_CYL_CHK:
		{
			int iUpChk = st_io.i_uld_rbt_pickerup_chk[iIdx];
			int iDnChk = st_io.i_uld_rbt_pickerdn_chk[iIdx];

			if( st_handler.mn_virtual_mode == 1 || ( g_ioMgr.get_in_bit( iDnChk ) == iOnOff && g_ioMgr.get_in_bit( iUpChk ) != iOnOff ) )
			{
				m_step_vac_down[iIdx] = ESTEP_CYL_FINISH;
				m_dwTimeVacDown[iIdx] = GetCurrentTime();
				break;
			}
			if( GetCurrentTime() - m_dwTimeVacDownStart[iIdx] > st_time.n_limit_time[E_WAIT_UNLOAD_PICKER] )
			{
				if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT ||  COMI.mn_run_status == dREINSTATE)
				{
					CString strJamCode = GetJamCode_VacuumDn_Time( iUpChk, iOnOff );
					CTL_Lib.Alarm_Error_Occurrence( 1231, dWARNING, (LPCSTR)strJamCode );
				}
			}
			else if( GetCurrentTime() - m_dwTimeVacDownStart[iIdx] < 0 )
			{
				m_dwTimeVacDownStart[iIdx] = GetCurrentTime();
			}
		}
		break;

	case ESTEP_CYL_FINISH:
		{
			if( GetCurrentTime() - m_dwTimeVacDown[iIdx] < 0 )
			{
				m_dwTimeVacDown[iIdx] = GetCurrentTime();
			}
			if( GetCurrentTime() - m_dwTimeVacDown[iIdx] < st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][iOnOff] )
				return;

			m_fbVacDown[iIdx] = FB_NONE;
			m_step_vac_down[iIdx] = ESTEP_CYL_START;
		}
		break;
	}
}

void CRun_Unload_Works::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][2])
{
	for( int i = 0 ; i < nPicker ; i++)
	{
		if(PickerInfo[D_EXIST][i] == CTL_YES)
		{
			mb_PickerUpDown_Flag[i] = false;
			m_dwPickerUpDown_Wait[i][0] = GetCurrentTime();
			
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[i], OnOff);
		}
	}
}

//523080 1 00 "Unloader Robot Left Up Check Off Error."
//523081 1 00 "Unloader Robot Left Up Check On Error."
//523090 1 00 "Unloader Robot Left Down Check Off Error."
//523091 1 00 "Unloader Robot Left Down Check On Error."
//523110 1 00 "Unloader Robot Right Up Check Off Error."
//523111 1 00 "Unloader Robot Right Up Check On Error."
//523120 1 00 "Unloader Robot Right Down Check Off Error."
//523121 1 00 "Unloader Robot Right Down Check On Error."
int CRun_Unload_Works::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][2])
{
	int i, FuncRet;
	int RetVal[2] = {0,};
	 
	FuncRet = RET_PROCEED;

	for(i = 0 ; i < nPicker ; i++)
	{
		RetVal[i] = CTL_NO;

		if(OnOff == IO_ON) //Picker ON (Down이라면..)
		{
			if(PickerInfo[D_EXIST][i] == CTL_YES) //동작 시킨 피커면!
			{
				if(mb_PickerUpDown_Flag[i] == false && g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerdn_chk[i]) == IO_ON
					&& g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerup_chk[i]) == IO_OFF)
				{
					mb_PickerUpDown_Flag[i] = true;
					m_dwPickerUpDown_Wait[i][0] = GetCurrentTime();   
				}
				else if(mb_PickerUpDown_Flag[i] == true && g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerdn_chk[i]) == IO_ON
					&& g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerup_chk[i]) == IO_OFF)
				{
					m_dwPickerUpDown_Wait[i][1] = GetCurrentTime();  
					m_dwPickerUpDown_Wait[i][2] = m_dwPickerUpDown_Wait[i][1] - m_dwPickerUpDown_Wait[i][0];
					
					if(m_dwPickerUpDown_Wait[i][2] > st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][OnOff])
					{
						RetVal[i] = CTL_YES;
					}
				}
				else
				{
					m_dwPickerUpDown_Wait[i][1] = GetCurrentTime();  
					m_dwPickerUpDown_Wait[i][2] = m_dwPickerUpDown_Wait[i][1] - m_dwPickerUpDown_Wait[i][0];
					
					if(m_dwPickerUpDown_Wait[i][2] > st_time.n_limit_time[E_WAIT_UNLOAD_PICKER])
					{
						if(st_handler.cwnd_list != NULL)
						{
							sprintf(st_msg.c_abnormal_msg, "[UPICKER Error] Picker down sensor check error.");
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
							Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
						}
						CString strJamCode = GetJamCode_Bcr_Guide_FwdBwd_Time(st_io.i_uld_rbt_pickerdn_chk[i], OnOff );
						CTL_Lib.Alarm_Error_Occurrence( 1232, dWARNING, strJamCode );
						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal[i] = CTL_YES;
		}
		else //Picker OFF (UP이라면..)
		{
			if(PickerInfo[D_EXIST][i] == CTL_YES) //동작 시킨 피커면!
			{
				if(mb_PickerUpDown_Flag[i] == false && g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerdn_chk[i]) == IO_OFF
					&& g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerup_chk[i]) == IO_ON)
				{
					mb_PickerUpDown_Flag[i] = true;
					m_dwPickerUpDown_Wait[i][0] = GetCurrentTime();   
				}
				else if(mb_PickerUpDown_Flag[i] == true && g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerdn_chk[i]) == IO_OFF
					&& g_ioMgr.get_in_bit(st_io.i_uld_rbt_pickerup_chk[i]) == IO_ON)
				{
					m_dwPickerUpDown_Wait[i][1] = GetCurrentTime();  
					m_dwPickerUpDown_Wait[i][2] = m_dwPickerUpDown_Wait[i][1] - m_dwPickerUpDown_Wait[i][0];
					
					if(m_dwPickerUpDown_Wait[i][2] > st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][OnOff])
					{
						RetVal[i] = CTL_YES;
					}
				}
				else
				{
					m_dwPickerUpDown_Wait[i][1] = GetCurrentTime();  
					m_dwPickerUpDown_Wait[i][2] = m_dwPickerUpDown_Wait[i][1] - m_dwPickerUpDown_Wait[i][0];
					
					if(m_dwPickerUpDown_Wait[i][2] > st_time.n_limit_time[E_WAIT_UNLOAD_PICKER])
					{
						if(st_handler.cwnd_list != NULL)
						{
							sprintf(st_msg.c_abnormal_msg, "[UPICKER Error] Picker up sensor check error.");
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
							Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
						}
						CString strJamCode = GetJamCode_Bcr_Guide_FwdBwd_Time(st_io.i_uld_rbt_pickerdn_chk[i], OnOff );
						CTL_Lib.Alarm_Error_Occurrence( 1233, dWARNING, strJamCode );

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal[i] = CTL_YES;
		}
	}

	int nCnt = 0, nPickerCnt = 0;
	for (i=0; i<nPicker; i++)
	{
		if (PickerInfo[D_EXIST][i] == CTL_YES) nPickerCnt++;
		if(RetVal[i] == CTL_YES) nCnt++;		
	
	}

	if(nCnt == nPickerCnt)
	{
		FuncRet = RET_GOOD;
	}
	
//	if(RetVal[0] == CTL_YES && RetVal[1] == CTL_YES) 
//	{
//		FuncRet = RET_GOOD;
//	}

	return FuncRet;
}

bool CRun_Unload_Works::Run_Unload_Move_Chk_Jam(stJobPicker job)
{
	return true;
}

bool CRun_Unload_Works::Run_Unload_Move_Chk_Time()
{
// 	switch(mn_RcvyStep)
// 	{
// 	case 0:
// 		if(st_var.n_use_vis_top != CTL_NO ) break;
// 		
// 		st_var.n_use_vis_top = CTL_YES;
// 		CommBtmVision(0);
// 		mn_RcvyStep = 100;
// 		break;
// 
// 	case 100:
// // 		if( GetBtmVisionResult(0) == EMBS_PASS || GetBtmVisionResult(0) == EMBS_FAIL)
// 		if( m_cwBtmviscomm[0] == ECW_NONE )
// 		{
// 			st_var.n_use_vis_top = CTL_NO;
// 			mn_RcvyStep = 0;
// 		}
// 		break;
// 
// 	}

	return true;
}

void CRun_Unload_Works::CommBtmVision( int nSite)
{
	m_cwBtmviscomm[nSite] = ECW_WORK;
	m_step_btmviscomm[nSite] = ECOMM_START;
	m_resultBtmvis[nSite] = EMBS_LOAD;
}

void CRun_Unload_Works::Run_Move_BtmVision( int nSite )
{
	if( m_cwBtmviscomm[nSite] == ECW_NONE ) return;

	switch( m_step_btmviscomm[nSite] )
	{
	case ECOMM_START: m_step_btmviscomm[nSite] = ECOMM_MOVE;
		if( st_basic.nVisionUse[3] ) m_step_btmviscomm[nSite] = ECOMM_CONNECT;
		else{
			Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin = NVR_PASS;
			CString strname = st_basic.mstr_device_name;
			Func.m_pVsClient[EVP_BTM_POS]->m_strModel = strname.Replace(".TXT", "");
			SetBtmVisionResult(nSite, EMBS_PASS);
			SetBtmVisionMdl(nSite, Func.m_pVsClient[EVP_BTM_POS]->m_strModel);
		}
		m_nRetryCnt[nSite] = 0;
		break;

	case ECOMM_CONNECT:
		if (st_client[CLS_VIS_BTM].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_btmviscomm[nSite] = ECOMM_INSP;
		}
		else
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();

			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CONNECT, CLS_VIS_BTM);

			m_step_btmviscomm[nSite] = ECOMM_CONNECT_CHK;
		}
		break;

	case ECOMM_CONNECT_CHK:
		if (st_client[CLS_VIS_BTM].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_btmviscomm[nSite] = ECOMM_INSP;
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

			if (m_dwTimeNetwork[nSite][2] > (st_basic.mn_network_wait_time*2))
			{
				m_nRetryCnt[nSite]++;
				if (m_nRetryCnt[nSite] >= 3)
				{

// 					if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
// 					{
						m_nRetryCnt[nSite] = 0;
						// 703000 1 A "Received incorrect btmvision1 from server."
						strJamCode.Format( "703%d00", nSite+1);
						CTL_Lib.Alarm_Error_Occurrence(1121, CTL_dWARNING, strJamCode );
						::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CLOSE, CLS_VIS_BTM);

						m_step_btmviscomm[nSite] = ECOMM_CONNECT;
// 					}
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();

					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CLOSE, CLS_VIS_BTM);
					m_step_btmviscomm[nSite] = ECOMM_CONNECT_WAIT;
				}
			}
		}
		break;

	case ECOMM_CONNECT_WAIT:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeNetwork[nSite][2] > 2000)
		{
			m_step_btmviscomm[nSite] = ECOMM_CONNECT;
		}
		break;

	case ECOMM_INSP:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}

		//2015.0412
// 		if (m_dwTimeNetwork[nSite][2] < 200/*st_time.n_wait_time[27][IO_ON]*//*st_time.n_wait_time[E_WAIT_TRIGGER_TIME][IO_ON]*/) break;
// 		g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_OFF);
			
// 		Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin = NVR_NONE;
// 		Func.m_pVsClient[EVP_BTM_POS]->OnVisionBcrResult(13);//2014.1118 9->13

		m_step_btmviscomm[nSite] = ECOMM_INSP_COM;
		break;

	case ECOMM_INSP_COM:
		if (Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin == NVR_PASS)
		{
			SetBtmVisionResult(nSite, EMBS_PASS);
			SetBtmVisionMdl(nSite, Func.m_pVsClient[EVP_BTM_POS]->m_strModel);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CLOSE, CLS_VIS_BTM);

			m_continus_vis_error[nSite] = 0;
			m_step_btmviscomm[nSite] = ECOMM_MOVE;
			g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_OFF);
		}
		else if ( Func.m_pVsClient[EVP_BTM_POS]->m_nBcrResultBin == NVR_FAIL)
		{ 
			SetBtmVisionResult(nSite, EMBS_FAIL);
			SetBtmVisionMdl(nSite, Func.m_pVsClient[EVP_BTM_POS]->m_strModel);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CLOSE, CLS_VIS_BTM);
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[BTM VISION_%d] Vision error.", nSite+1);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
			}

			m_step_btmviscomm[nSite] = ECOMM_MOVE;

// 			930100 1 00 "Continuous_vision_error_Btm_Left_Vision"
// 			930101 1 00 "Continuous_vision_error_Btm_Right_Vision"
			m_continus_vis_error[nSite]++;

			//2015.0412
			if(m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt)
			{
				m_continus_vis_error[nSite] = 0;
				sprintf( cJamCode, "93010%d", nSite );
				CTL_Lib.Alarm_Error_Occurrence(3202, CTL_dWARNING, cJamCode );
			}
			else if( m_continus_vis_error[nSite] < 2)//2015.0419
			{
				m_dwTimeNetwork[nSite][0] = GetTickCount();					
				m_step_btmviscomm[nSite] = ECOMM_INSP_COM_WAIT;
			}
			g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_OFF);
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
			
			if (m_dwTimeNetwork[nSite][2] > (st_basic.mn_network_wait_time*2))
			{
				m_nRetryCnt[nSite]++;
				if (m_nRetryCnt[nSite] >= 3)
				{
					m_nRetryCnt[nSite] = 0;
					
					// 704000 1 A "Btmvision1 time out from server."
					strJamCode.Format( "704%d00", nSite);
					CTL_Lib.Alarm_Error_Occurrence(1122, CTL_dWARNING, strJamCode );
					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CLOSE, CLS_VIS_BTM);

					m_step_btmviscomm[nSite] = ECOMM_CONNECT;
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();
					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CLOSE, CLS_VIS_BTM);

					m_step_btmviscomm[nSite] = ECOMM_INSP_COM_WAIT;
				}
				g_ioMgr.set_out_bit(st_io.o_btm_vision_trigger_onoff, IO_OFF);
			}
		}
		break;

	case ECOMM_INSP_COM_WAIT:
		m_dwTimeNetwork[nSite][1] = GetTickCount();
		m_dwTimeNetwork[nSite][2] = m_dwTimeNetwork[nSite][1] - m_dwTimeNetwork[nSite][0];
		
		if (m_dwTimeNetwork[nSite][2] <= 0)
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeNetwork[nSite][2] > 1000)
		{
			m_step_btmviscomm[nSite] = ECOMM_CONNECT;
		}
		break;

	case ECOMM_MOVE:
		{
			m_dwTimeCommVision[nSite] = GetCurrentTime();
			m_step_btmviscomm[nSite] = ECOMM_FINISH;
		}
		break;

	case ECOMM_FINISH:
		if( GetCurrentTime() - m_dwTimeCommVision[nSite] < 0 ) m_dwTimeCommVision[nSite] = GetCurrentTime();
//		if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_PASS)
		if( GetBtmVisionResult(nSite) == EMBS_PASS)
		{
			m_step_btmviscomm[nSite] = ECOMM_NONE;
			m_cwBtmviscomm[nSite] = ECW_NONE;
			
			CString strModel = st_basic.mstr_device_name;
			strModel.Replace(".TXT", "");
//			if( g_client_vision[EVP_BTM_POS].GetRecvModelResult() != strModel)
//			{
//				g_client_vision[EVP_BTM_POS].SetResultBcrTest(NVR_FAIL);
//			}
// 			if( GetBtmVisionMdl(nSite) != strModel)
// 			{
// 				SetBtmVisionResult(nSite, EMBS_FAIL);
// 			}
		}
//		else if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_FAIL )
		if( GetBtmVisionResult(nSite) == EMBS_FAIL)
		{
			m_cwBtmviscomm[nSite] = ECW_NONE;
			m_step_btmviscomm[nSite] = ECOMM_NONE;
		}
		break;
	}
}


void CRun_Unload_Works::Run_Move()
{
	int nJobCnt = Func.m_pRobot[SITE_UNLOAD]->GetJobCount();
	if( nJobCnt == 0 )
		return;

//   	Func.m_pRobot[SITE_UNLOAD]->EraseFirstJob();

	stJobPicker& job = Func.m_pRobot[SITE_UNLOAD]->GetFirstJob();

	if( bvis_step_old != job.bvis_step)
	{
		bvis_step_old = job.bvis_step;
		m_dwTimeMove = GetCurrentTime();
		if( nJobCnt == 0 )
			return;

		if( Run_Unload_Move_Chk_Jam( job ) == false )
			return;
	}
	if( Run_Unload_Move_Chk_Time() == false )
		return;
	Func.ThreadFunctionStepTrace(70, job.bvis_step);


	switch( job.bvis_step )
	{
	case BVISION_NONE: break;

	case BVISION_PICK_READY:
		{			
// 			double dpos = COMI.Get_MotCurrentPos(M_ULD_Z);
// 			if ( dpos > st_motor[M_ULD_Z ].d_pos[P_ULD_Z_SAFETY]  + COMI.md_allow_value[M_ULD_Z])
// 			{
// 				job.bvis_step = BVISION_PICK_SAFETY;
// 			}
// 			else
// 			{
// 				job.bvis_step = BVISION_PICK_PUSHER_MDL;
// 			}
			ReqPusherBuf( BACKWARD );
// 			ReqPusherBuf( FORWARD );
			job.bvis_step = BVISION_PICK_SAFETY;
// 			m_dwTimeMove = GetCurrentTime();
		}
		break;

	case BVISION_PICK_SAFETY:
		{
// 			if( GetCurrentTime() - m_dwTimeMove <= 0 ) m_dwTimeMove = GetCurrentTime();
// 			if( GetCurrentTime() - m_dwTimeMove < 100) break;
			int nRet1 = COMI.Check_MotPosRange(M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS], COMI.md_allow_value[M_ULD_Y]);
			if(nRet1 == BD_GOOD)
			{
				job.bvis_step = BVISION_PICK_PICKER_DOWN;
				break;
			}
			int nRet = CTL_Lib.Single_Move( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_ULD_Z_SAFETY], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = BVISION_PICK_MOVE_POS;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1234, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BVISION_PICK_PUSHER_MDL:
		job.bvis_step = BVISION_PICK_MOVE_POS;
		break;


	case BVISION_PICK_MOVE_POS:
		{
			int nRet1 = COMI.Check_MotPosRange(M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS], COMI.md_allow_value[M_ULD_Y]);
			if(nRet1 == BD_GOOD)
			{
				job.bvis_step = BVISION_PICK_PICKER_DOWN;
				break;
			}
			job.bvis_step = BVISION_STEP(41);
		}
		break;

	case 41:
		{
			int nRet = CTL_Lib.Single_Move( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = BVISION_PICK_PICKER_DOWN;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1235, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BVISION_PICK_PICKER_DOWN:
		{
// 			if( GetPusherBuf() != FB_NONE) break;
			if( job.iRobotFirst != -1 )
			{
				g_Run_Unload_Works.ReqVacDown( job.iRobotFirst, FORWARD );
				if(st_basic.n_mode_device == 1)
				{
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[job.iRobotFirst], IO_ON );
					//2015.0417
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotFirst], IO_OFF );
				}
			}
			if( job.iRobotSecond != -1 )
			{
				g_Run_Unload_Works.ReqVacDown( job.iRobotSecond, FORWARD );
				if(st_basic.n_mode_device == 1)
				{
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[job.iRobotSecond], IO_ON );
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotSecond], IO_OFF );
				}
			}

			//2014,1230
			if( st_var.n_boatid_out_flag == CTL_YES ) break;

// 			if( m_bUse_idbuff[job.iRobotFirst] == true || m_bUse_idbuff[job.iRobotSecond] == true )
// 			{
			if(st_basic.n_mode_device == CTL_YES && st_basic.mn_mode_run == CTL_YES)
				st_var.n_boatid_out_flag = CTL_YES;
// 			}
			st_var.n_boatid_out_flag = CTL_NO;//2016.0924

			job.bvis_step = BVISION_PICK_Z_DOWN;
			//2015.0606
			m_dwTimeMove = GetCurrentTime();
			job.bvis_step = BVISION_STEP(51);
		}
		break;

	case 51:
		{
			if( GetCurrentTime() - m_dwTimeMove < 0 ) m_dwTimeMove = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_VACUUM][IO_ON]) break;

			if( st_basic.n_mode_device && job.iRobotFirst != -1 && 
				g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_ON )
			{
				CString sterError="";
				sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 2249, CTL_dWARNING, sterError );
				break;
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1 &&
				g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_ON )
			{
				CString sterError="";
				sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 2250, CTL_dWARNING, sterError );
				break;
			}
			
			job.bvis_step = BVISION_PICK_Z_DOWN;
		}
		break;

	case BVISION_PICK_Z_DOWN:
		{
			m_zPos = st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_POS];
			int nRet = CTL_Lib.Single_Move( M_ULD_Z, m_zPos, st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = BVISION_PICK_Z_DOWN_CHK;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1236, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BVISION_PICK_Z_DOWN_CHK:
		if( st_basic.n_mode_device && job.iRobotFirst != -1 && g_Run_Unload_Works.GetVacDown( job.iRobotFirst ) != FB_NONE )		break;
		if( st_basic.n_mode_device && job.iRobotSecond != -1 && g_Run_Unload_Works.GetVacDown( job.iRobotSecond ) != FB_NONE )		break;
		job.bvis_step = BVISION_PICK_VACUUM;
		break;

	case BVISION_PICK_VACUUM:
		{
			if( job.iRobotFirst != -1 )
			{
				if(st_basic.n_mode_device == 1)
				{
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[job.iRobotFirst], IO_ON );
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotFirst], IO_OFF );
				}
			}
			if( job.iRobotSecond != -1 )
			{
				if(st_basic.n_mode_device == 1)
				{
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[job.iRobotSecond], IO_ON );
					g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotFirst], IO_OFF );
				}
			}
			m_dwTimeMove = GetCurrentTime();
// 			ReqPusherBuf( BACKWARD );
			
			mn_retry = 0;
			job.bvis_step = BVISION_PICK_VACUUM_CHK;			
		}
		break;

	case BVISION_PICK_VACUUM_ON_ZDOWN:
		{
			if(COMI.mn_run_status == dRUN)	st_work.md_reinstatement_Pos[0][M_ULD_Z] = m_zPos;
			int nRet = CTL_Lib.Single_Move(M_ULD_Z, m_zPos, st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				mn_retry = 0;
				job.bvis_step = BVISION_PICK_PULL_MDL;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1237, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BVISION_PICK_PULL_MDL:
// 		ReqPusherBuf( BACKWARD );
		m_dwTimeMove = GetCurrentTime();
		job.bvis_step = BVISION_PICK_VACUUM_CHK;
		break;


	case BVISION_PICK_VACUUM_CHK:
		{
			if( GetCurrentTime() - m_dwTimeMove < 0 ) m_dwTimeMove = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_VACUUM][IO_ON]) break;

			if( st_basic.n_mode_device && job.iRobotFirst != -1 && g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_OFF )
			{
				mn_retry++;
 				m_zPos = g_comiMgr.Get_MotCurrentPos(M_ULD_Z) + 0.2;
//  				job.bvis_step = BVISION_PICK_VACUUM_ON_ZDOWN;
				job.bvis_step = BVISION_STEP(111);
				m_dwTimeMove = GetCurrentTime();//2015.0507
				break;
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1 && g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_OFF )
			{
				mn_retry++;
 				m_zPos = g_comiMgr.Get_MotCurrentPos(M_ULD_Z) + 0.2;
//  				job.bvis_step = BVISION_PICK_VACUUM_ON_ZDOWN;
				job.bvis_step = BVISION_STEP(111);
				m_dwTimeMove = GetCurrentTime();//2015.0507
				break;
			}

			job.bvis_step = BVISION_PICK_PICKER_UP;
		}
		break;

	case 111:
// 		if(mn_retry >= st_basic.mn_network_retry_cnt && 
// 			( GetCurrentTime() - m_dwTimeMove > 5000 ) )
// 		{
// // 			523100 1 00 "PS2302_Unloader_Robot_Left_Vacuum_Check_Off_Error."
// // 			523130 1 00 "PS2305_Unloader_Robot_Right_Vacuum_Check_Off_Error."
//  			if( st_basic.n_mode_device && job.iRobotFirst != -1 && g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_OFF )
// 				CTL_Lib.Alarm_Error_Occurrence( 1298, CTL_dWARNING, "523100" );
// 			else if( st_basic.n_mode_device && job.iRobotSecond != -1 && g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_OFF )
// 				CTL_Lib.Alarm_Error_Occurrence( 1299, CTL_dWARNING, "523130" );
// 			else
// 				job.bvis_step = BVISION_PICK_PULL_MDL;
// 		}
		if(mn_retry >= st_basic.mn_network_retry_cnt/* && GetCurrentTime() - m_dwTimeMove > 5000*/)
		{
			////2015.0507
			if( GetCurrentTime() - m_dwTimeMove < 0 ) m_dwTimeMove = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_VACUUM][IO_ON]) break;
// 			523100 1 00 "PS2302_Unloader_Robot_Left_Vacuum_Check_Off_Error."
// 			523130 1 00 "PS2305_Unloader_Robot_Right_Vacuum_Check_Off_Error."
// 			if( job.iRobotFirst != -1 && g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_OFF )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1298, CTL_dWARNING, "523100" );
// 			}
// 			else
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1299, CTL_dWARNING, "523130" );
// 			}
// 			else if( job.iRobotSecond != -1 && g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_OFF )
// 				CTL_Lib.Alarm_Error_Occurrence( 1299, CTL_dWARNING, "523130" );
			mn_retry = 0;
			if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[0] , IO_ON) == IO_OFF )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1298, CTL_dWARNING, "523100" );
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence( 1299, CTL_dWARNING, "523130" );
			}
			job.bvis_step = BVISION_PICK_PULL_MDL;
		}
		else
		{
			job.bvis_step = BVISION_PICK_VACUUM_ON_ZDOWN;
		}
		break;
		


	case BVISION_PICK_PICKER_UP:
		{
// 			if( GetPusherBuf() != FB_NONE) break;


			if( job.iRobotFirst != -1 )
			{
				g_Run_Unload_Works.ReqVacDown( job.iRobotFirst, BACKWARD );
			}
			if( job.iRobotSecond != -1 )
			{
				g_Run_Unload_Works.ReqVacDown( job.iRobotSecond, BACKWARD );
			}
			job.bvis_step = BVISION_PICK_Z_UP;

			if( st_basic.mn_mode_run )
			{
				int nBoat = 0;
				CString strBoat="";
				if( job.iRobotFirst != -1)
				{
					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[job.iRobotFirst] == 0 &&
					st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[job.iRobotFirst] == 1)
					{
						nBoat = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[job.iRobotFirst];
						m_strbcr_idbuff[job.iRobotFirst].Format("%d", nBoat);
						m_strbcr_serial[job.iRobotFirst].Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[job.iRobotFirst]);
						m_strbcr_lotid[job.iRobotFirst].Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_lot_id[job.iRobotFirst]);
						m_bUse_idbuff[job.iRobotFirst] = true;
					}
				}
				if( job.iRobotSecond != -1)
				{
					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[job.iRobotSecond] == 0 &&
						st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[job.iRobotSecond] == 1)
					{
						nBoat = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuff_num[job.iRobotSecond];
						m_strbcr_idbuff[job.iRobotSecond].Format("%d", nBoat);
						m_strbcr_serial[job.iRobotSecond].Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[job.iRobotSecond]);
						m_strbcr_lotid[job.iRobotSecond].Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_lot_id[job.iRobotSecond]);
						m_bUse_idbuff[job.iRobotSecond] = true;
					}
				}

			}


			if( job.iRobotFirst != -1 )
			{
				st_uld_rbt.bPick[job.iRobotFirst] = true;
				st_uld_rbt.n_exist[job.iRobotFirst] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[job.iRobotFirst] = 0;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[job.iRobotFirst] = 1;
				st_handler.mn_Top_Vision[4] = st_handler.mn_Top_Vision[2];//2015.0318
				st_handler.mn_Top_Vision[2] = CTL_NO;
			}
			if( job.iRobotSecond != -1 )
			{
				st_uld_rbt.bPick[job.iRobotSecond] = true;				
				st_uld_rbt.n_exist[job.iRobotSecond] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[job.iRobotSecond] = 0;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[job.iRobotSecond] = 1;
				st_handler.mn_Top_Vision[5] = st_handler.mn_Top_Vision[3];//2015.0318
				st_handler.mn_Top_Vision[3] = CTL_NO;
			}

			if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_up_chk, IO_ON) == IO_OFF ||
				g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_down_chk, IO_OFF) == IO_ON)
				g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[0], IO_OFF);
			if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_up_chk, IO_ON) == IO_OFF ||
				g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_down_chk, IO_OFF) == IO_ON)
				g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[1], IO_OFF);

		}
		break;

	case BVISION_PICK_Z_UP:
		{
			m_zPos = st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP];
			int nRet = CTL_Lib.Single_Move( M_ULD_Z, m_zPos, st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = BVISION_PICK_PICKER_UP_CHK;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1238, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case BVISION_PICK_PICKER_UP_CHK:
		if( st_basic.n_mode_device && job.iRobotFirst != -1 && g_Run_Unload_Works.GetVacDown( job.iRobotFirst ) != FB_NONE )		break;
		if( st_basic.n_mode_device && job.iRobotSecond != -1 && g_Run_Unload_Works.GetVacDown( job.iRobotSecond ) != FB_NONE )		break;
		m_dwTimeMove = GetCurrentTime();
		job.bvis_step = BVISION_PICK_EMPTY_CHK;
		break;

	case BVISION_PICK_EMPTY_CHK:
		{
			if( GetCurrentTime() - m_dwTimeMove <= 0 )
			{
				m_dwTimeMove = GetCurrentTime();
				break;
			}
			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][IO_ON]) break;
			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
			{
				if(g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_OFF )
				{
					CString strError="";
					strError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
					CTL_Lib.Alarm_Error_Occurrence( 1239, CTL_dWARNING, strError );
					break;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
			{
				if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_OFF )
				{
					CString strError="";
					strError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
					CTL_Lib.Alarm_Error_Occurrence( 1240, CTL_dWARNING, strError );
					break;
				}
			}
// 			ReqPusherBuf( BACKWARD );

			job.bvis_step = BVISION_PICK_FINISH;
		}
		break;

	case BVISION_PICK_FINISH:
		{
			if( GetPusherBuf() != FB_NONE) break;
			
			if( g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box1_chk, IO_OFF) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_idbuffer_elv_box2_chk, IO_OFF) == IO_ON )
			{
					CString strError="";
					strError.Format("5%04d%d",st_io.i_idbuffer_elv_box1_chk, IO_ON);
					CTL_Lib.Alarm_Error_Occurrence( 1241, CTL_dWARNING, strError );
					break;
			}




			if( job.iRobotFirst != -1 )
			{
// 				st_uld_rbt.bPick[job.iRobotFirst] = true;
// 				st_uld_rbt.n_exist[job.iRobotFirst] = 1;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[job.iRobotFirst] = 0;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[job.iRobotFirst] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_RetVision[job.iRobotFirst] = st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotFirst];
			}
			if( job.iRobotSecond != -1 )
			{
// 				st_uld_rbt.bPick[job.iRobotSecond] = true;				
// 				st_uld_rbt.n_exist[job.iRobotSecond] = 1;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[job.iRobotSecond] = 0;
// 				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[job.iRobotSecond] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_RetVision[job.iRobotSecond] = st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotSecond];
			}
			
			// UI 업데이트
			if( st_handler.cwnd_main )
			{
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY);
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDR_PICKER);
			}
				
//			g_unload_robot.EraseFirstJob();
//			Func.BackupData();//2013,1018

			Func.Handler_Recovery_Data_Write();

//			st_handler.mn_pickup_elv[0] = CTL_YES;
			job.bvis_step = BVISION_VISION_POS;
			mn_MotFlag[0] = mn_MotFlag[1] = CTL_NO;



		}
		break;
		

	case BVISION_VISION_POS:
		{
// 			int nRet1 = CTL_Lib.Single_Move( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS], st_basic.nRunSpeed );
// 			if( nRet1 == BD_GOOD )
// 			{
// 				job.bvis_step = BVISION_VISION1_CHK;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 1242, CTL_dWARNING, alarm.mstr_code );
// 			}

			int nRet1 = COMI.Start_SingleMove( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				job.bvis_step = BVISION_STEP(171);//BVISION_VISION1_CHK;
				//2016.0119
				//m_dwTimeMove = GetCurrentTime();
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1242, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 171:
		{
			//2016.0119
// 			if( GetCurrentTime() - m_dwTimeMove <= 0 )
// 			{
// 				m_dwTimeMove = GetCurrentTime();
// 				break;
// 			}
// 			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][IO_ON]) break;
// 			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
// 			{
// 				if(g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_OFF )
// 				{
// 					CString strError;
// 					strError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
// 					CTL_Lib.Alarm_Error_Occurrence( 2239, CTL_dWARNING, strError );
// 					break;
// 				}
// 			}
// 			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
// 			{
// 				if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_OFF )
// 				{
// 					CString strError;
// 					strError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
// 					CTL_Lib.Alarm_Error_Occurrence( 2240, CTL_dWARNING, strError );
// 					break;
// 				}
// 			}

			int nRet1 = COMI.Check_SingleMove( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS] );
			if( nRet1 == BD_GOOD )
			{
				job.bvis_step = BVISION_VISION1_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1292, CTL_dWARNING, alarm.mstr_code );
				job.bvis_step = BVISION_VISION_POS;
			}
		}
		break;


	case BVISION_VISION1_CHK:
		{
			int nRet1;
			if( job.iRobotFirst != -1 /*&& Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotFirst).GetModule().GetModuleState() == EMS_VISION*/) {
				nRet1 = CTL_Lib.Single_Move( M_BTM_VISION_X, st_motor[M_BTM_VISION_X].d_pos[P_VIS1_POS + job.iRobotFirst], st_basic.nRunSpeed );
				if( nRet1 == BD_GOOD )
				{
					job.bvis_step = BVISION_STEP(181);
				}
				else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
				{
					CTL_Lib.Alarm_Error_Occurrence( 1244, CTL_dWARNING, alarm.mstr_code );
				}
			}
			else {
				job.bvis_step = BVISION_VISION2_CHK;
			}
		}
		break;

	case 181:
		if( st_var.n_use_vis_top != CTL_NO ) break;
		st_var.n_use_vis_top = CTL_YES;
		st_handler.mn_Btm_Vision[job.iRobotFirst] = CTL_NO;
		job.bvis_step = BVISION_VISION1_COMM;
		
		Func.m_pVsClient[EVP_BTM_POS]->SetResultBcrTest(NVR_NONE);
		CommBtmVision(job.iRobotFirst);
		break;

	case BVISION_VISION1_COMM:
//		if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_PASS)
		//2015.0419
		if( m_cwBtmviscomm[job.iRobotFirst] == ECW_NONE && GetBtmVisionResult(job.iRobotFirst) == EMBS_PASS )
		{

			//2015.026
			if( st_basic.nVisionUse[2] == CTL_YES )
			{
				//2015.0318
				if(st_handler.mn_Top_Vision[4] == CTL_GOOD/*st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_RetVision[job.iRobotFirst] == CTL_GOOD*/)
				{
					st_handler.mn_Btm_Vision[job.iRobotFirst] = CTL_YES;
					st_uld_rbt.st_btm[job.iRobotFirst].nState = EMBS_PASS;
				}
				else
				{
					st_uld_rbt.st_btm[job.iRobotFirst].nState = EMBS_FAIL;
					st_handler.mn_Btm_Vision[job.iRobotFirst] = CTL_ERROR;
				}
				st_handler.mn_Top_Vision[4] = CTL_NO;
			}
			else
			{
				st_handler.mn_Btm_Vision[job.iRobotFirst] = CTL_YES;
				st_uld_rbt.st_btm[job.iRobotFirst].nState = EMBS_PASS;
			}

			sprintf(st_uld_rbt.st_btm[job.iRobotFirst].c_model_id,"%s", Func.m_pVsClient[EVP_BTM_POS]->m_strModel);
		}
//		else if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_FAIL)
		else if( m_cwBtmviscomm[job.iRobotFirst] == ECW_NONE && GetBtmVisionResult(job.iRobotFirst) == EMBS_FAIL)
		{
			st_handler.mn_Btm_Vision[job.iRobotFirst] = CTL_ERROR;
			st_uld_rbt.st_btm[job.iRobotFirst].nState = EMBS_FAIL;
			sprintf(st_uld_rbt.st_btm[job.iRobotFirst].c_model_id,"%s", Func.m_pVsClient[EVP_BTM_POS]->m_strModel);
		}
		if( m_cwBtmviscomm[job.iRobotFirst] == ECW_NONE && 
			(GetBtmVisionResult(job.iRobotFirst) == EMBS_PASS || GetBtmVisionResult(job.iRobotFirst) == EMBS_FAIL) )
		{
			if( job.iRobotFirst != -1 ) {
				if( st_handler.mn_Btm_Vision[job.iRobotFirst] != CTL_NO ) {
					job.bvis_step = BVISION_VISION2_CHK;
				}
			}
		}
		break;
	

	case BVISION_VISION2_CHK:
		{
			int nRet1;
			if( job.iRobotSecond != -1/*  && Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotSecond).GetModule().GetModuleState() == EMS_VISION*/) 
			{
				nRet1 = CTL_Lib.Single_Move( M_BTM_VISION_X, st_motor[M_BTM_VISION_X].d_pos[P_VIS1_POS + job.iRobotSecond], st_basic.nRunSpeed );
				if( nRet1 ==  BD_GOOD )
				{
					job.bvis_step = BVISION_VISION2_COMM;
					st_handler.mn_Btm_Vision[job.iRobotSecond] = CTL_NO;
					Func.m_pVsClient[EVP_BTM_POS]->SetResultBcrTest(NVR_NONE);
					CommBtmVision(job.iRobotSecond);
//					g_client_vision[EVP_BTM_POS].ClearResultBcrTest();
//					if( st_basic.mn_mode_run)	CommBtmVision(job.iRobotSecond);
//					else						g_client_vision[EVP_BTM_POS].SetResultBcrTest( NVR_PASS );
				}
				else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
				{
					CTL_Lib.Alarm_Error_Occurrence( 1245, CTL_dWARNING, alarm.mstr_code );
				}
			}
			else {
				job.bvis_step = BVISION_VISION_RESULT;
			}
		}
		break;

	case BVISION_VISION2_COMM:
//		if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_PASS)
		//2015.0419
		if( m_cwBtmviscomm[job.iRobotSecond] == ECW_NONE && GetBtmVisionResult(job.iRobotSecond) == EMBS_PASS)
		{
			//2015.026
			if( st_basic.nVisionUse[2] == CTL_YES )
			{
				//2015.0318
				if(st_handler.mn_Top_Vision[5] == CTL_GOOD/*st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_RetVision[job.iRobotSecond] == CTL_GOOD*/)
				{
					st_uld_rbt.st_btm[job.iRobotSecond].nState = EMBS_PASS;
					st_handler.mn_Btm_Vision[job.iRobotSecond] = CTL_YES;
				}
				else
				{
					st_handler.mn_Btm_Vision[job.iRobotSecond] = CTL_ERROR;
					st_uld_rbt.st_btm[job.iRobotSecond].nState = EMBS_FAIL;
				}
				st_handler.mn_Top_Vision[5] = CTL_NO;
			}
			else
			{
				st_handler.mn_Btm_Vision[job.iRobotSecond] = CTL_YES;
				st_uld_rbt.st_btm[job.iRobotSecond].nState = EMBS_PASS;
			}
			sprintf(st_uld_rbt.st_btm[job.iRobotSecond].c_model_id,"%s", Func.m_pVsClient[EVP_BTM_POS]->m_strModel);

		}
//		else if( g_client_vision[EVP_BTM_POS].GetResultBcrTest() == NVR_FAIL)
		else if( m_cwBtmviscomm[job.iRobotSecond] == ECW_NONE && GetBtmVisionResult(job.iRobotSecond) == EMBS_FAIL)
		{
			st_handler.mn_Btm_Vision[job.iRobotSecond] = CTL_ERROR;
			st_uld_rbt.st_btm[job.iRobotSecond].nState = EMBS_FAIL;
			sprintf(st_uld_rbt.st_btm[job.iRobotSecond].c_model_id,"%s", Func.m_pVsClient[EVP_BTM_POS]->m_strModel);
		}
		//2015.0419
		if( m_cwBtmviscomm[job.iRobotSecond] == ECW_NONE && 
			( GetBtmVisionResult(job.iRobotSecond) == EMBS_PASS || GetBtmVisionResult(job.iRobotSecond) == EMBS_FAIL) )
		{
			if( job.iRobotSecond != -1 ) {
				if( st_handler.mn_Btm_Vision[job.iRobotSecond] != CTL_NO ) {
					job.bvis_step = BVISION_VISION_RESULT;
				}
			}
			st_var.n_use_vis_top = CTL_NO;
		}
		break;		

	case BVISION_VISION_RESULT:
//		if( job.iRobotFirst != -1  && Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotFirst).GetModule().GetModuleState() == EMS_VISION)		
//		{
//			if( st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES ) {
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotFirst).GetModule().GetBtmVision().SetBtmVisionState(EMBS_PASS);
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotFirst).GetModule().SetTestUnloadState(EMS_UNLOAD);
//			}
//		}
//		else
//		{
//			if( job.iRobotFirst != -1)
//			{
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotFirst).GetModule().GetBtmVision().SetBtmVisionState(EMBS_FAIL);
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotFirst).GetModule().SetTestUnloadState(EMS_REJECT);
//			}
//		}
//
//		if( job.iRobotSecond != -1  && Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotSecond).GetModule().GetModuleState() == EMS_VISION)
//		{ 			
//			if( st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES ) {
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotSecond).GetModule().GetBtmVision().SetBtmVisionState(EMBS_PASS);
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotSecond).GetModule().SetTestUnloadState(EMS_UNLOAD); 
//			}
//		}
//		else
//		{
//			if( job.iRobotSecond != -1)
//			{
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotSecond).GetModule().GetBtmVision().SetBtmVisionState(EMBS_FAIL);
//				Func.m_pRobot[SITE_UNLOAD]->GetPicker(job.iRobotSecond).GetModule().SetTestUnloadState(EMS_REJECT);
//			}
//		}

		{
			int nRet1 = COMI.Start_SingleMove( M_BTM_VISION_X, st_motor[M_BTM_VISION_X].d_pos[P_VIS1_POS], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				Func.m_pRobot[SITE_UNLOAD]->EraseFirstJob();
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1299, CTL_dWARNING, alarm.mstr_code );
			}
		}

		break;

	case BVISION_PLACE_READY:
		{
			bool lota = false;
			bool lotb = false;
			if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
			{
				if( job.iRobotFirst != -1 ) 
				{
					if( Func.CmpStrFile(m_strbcr_lotid[job.iRobotFirst], g_lotMgr.GetLotAt(0).GetLotID() ) )  lota = true;
				}
				else
				{
					lota = true;
				}
				if( job.iRobotSecond != -1 ) 
				{
					if( Func.CmpStrFile(m_strbcr_lotid[job.iRobotSecond], g_lotMgr.GetLotAt(0).GetLotID() ) )  lotb = true;
				}
				else
				{
					lotb = true;
				}
			}
			if( lotb == true && lota == true)
				job.bvis_step = BVISION_PLACE_MOVE_CALC;
			else
			{
				m_dwTimeMove = GetCurrentTime();
				job.bvis_step = BVISION_STEP(301);
			}
		}
		break;

	case 301:
		{
			bool lota = false;
			bool lotb = false;
			if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
			{
				if( job.iRobotFirst != -1 ) 
				{
					if( Func.CmpStrFile(m_strbcr_lotid[job.iRobotFirst], g_lotMgr.GetLotAt(0).GetLotID() ) )  lota = true;
				}
				else
				{
					lota = true;
				}
				if( job.iRobotSecond != -1 ) 
				{
					if( Func.CmpStrFile(m_strbcr_lotid[job.iRobotSecond], g_lotMgr.GetLotAt(0).GetLotID() ) )  lotb = true;
				}
				else
				{
					lotb = true;
				}
			}
			if( lotb == true && lota == true)
				job.bvis_step = BVISION_PLACE_MOVE_CALC;
			else
			{
				if( GetCurrentTime() - m_dwTimeMove <= 0) m_dwTimeMove = GetCurrentTime();
				else if( GetCurrentTime() - m_dwTimeMove > MES_TIME_OUT )
				{
					st_msg.mstr_event_msg[0] = "[LOT END] There is a previous lot";
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 2);
					job.bvis_step = BVISION_PLACE_READY;
				}
				//2015.0523
// 				job.bvis_step = BVISION_STEP(302);
				st_var.n_index_move_sidecover_fold_flag = CTL_YES;//2015.0311 multilot
			}

		}
		break;

	case 302:
		if( st_basic.mn_outconv == CTL_YES)//2015.0210
		{
			
			if (st_client[CLS_NEXT].n_connect == CTL_YES)
			{
				if(g_lotMgr.GetLotAt(0).GetNewLotInPass() == false )
				{
					//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
					job.bvis_step = BVISION_STEP(303);
				}
				else
					job.bvis_step = BVISION_STEP(305);
			}
			else
			{
				mn_retry = 0;
				m_dwTimeMove = GetCurrentTime();
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CONNECT, CLS_NEXT);
				job.bvis_step = BVISION_STEP(303);
			}
		}
		else// if( st_basic.mn_outconv == CTL_NO )
		{
			job.bvis_step = BVISION_STEP(305);
		}
		break;

	case 303:
		if (st_client[CLS_NEXT].n_connect == CTL_YES)
		{
			m_dwTimeMove = GetCurrentTime();
			job.bvis_step = BVISION_STEP(305);
		}
		else
		{			
			if ( GetCurrentTime() - m_dwTimeMove <= 0)
			{
				m_dwTimeMove = GetCurrentTime();
				break;
			}
			
			if ( GetCurrentTime() - m_dwTimeMove > st_basic.mn_network_wait_time)
			{
				mn_retry++;
				if (mn_retry >= 3)
				{
					CString strJamCode;
					mn_retry = 0;
					
					//750000 1 00 "NEXT_Machine is not response."
					CTL_Lib.Alarm_Error_Occurrence(1195, CTL_dWARNING, "750000" );
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
					job.bvis_step = BVISION_STEP(304);
				}
				else
				{
					m_dwTimeMove = GetCurrentTime();
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
					job.bvis_step = BVISION_STEP(304);
				}
			}
		}
		break;

	case 304:
		if ( GetCurrentTime() - m_dwTimeMove <= 0)
		{
			m_dwTimeMove = GetCurrentTime();
			break;
		}
		
		if (GetCurrentTime() - m_dwTimeMove > 5000)
		{
			job.bvis_step = BVISION_STEP(303);
		}
		break;

	case 305:
		if( st_basic.mn_outconv == CTL_YES)//2015.0210
		{
			if(g_lotMgr.GetLotAt(0).GetNewLotInPass() == false )
			{
				g_next_client.OnMove_BufferIn();
				m_dwTimeMove = GetCurrentTime();
				job.bvis_step = BVISION_STEP(306);
			}
			else
			{
				job.bvis_step = BVISION_STEP(307);
			}
		}
		else// if( st_basic.mn_outconv == CTL_NO )
		{
			job.bvis_step = BVISION_STEP(307);
		}
		break;

	case 306:
		if( g_next_client.GetBufferInfo())
		{
			job.bvis_step = BVISION_STEP(304);
		}
		if( GetCurrentTime() - m_dwTimeMove <= 0) m_dwTimeMove = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeMove > MES_TIME_OUT)
		{
			// 			Func.OnSet_IO_Port_Stop();
			CString strErr;
			strErr.Format( "[NEXT Machine] Next Machine is not response");
			st_msg.mstr_event_msg[0] = strErr;
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			job.bvis_step = BVISION_STEP(307);
		}
		break;

	case 307:
		job.bvis_step = BVISION_PLACE_READY;
		break;


	case BVISION_PLACE_MOVE_CALC:
//		if( job.iPosFirst > MPOS_INDEX_TOPVISION &&  job.iPosFirst <= MPOS_INDEX_UNLOAD)
//		{
//		}

		{
			double dpos;

			job.bvis_step = BVISION_PLACE_MOVE;
			dpos = COMI.Get_MotCurrentPos(M_ULD_Z);
			if( dpos > (st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP] + COMI.md_allow_value[M_ULD_Z]) ||
				( g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_up_chk, IO_ON) == IO_OFF || 
				  g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_down_chk, IO_OFF) == IO_ON ) ||
				( g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_up_chk, IO_ON) == IO_OFF ||
			      g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_down_chk, IO_OFF) == IO_ON) )
			{
				g_Run_Unload_Works.ReqVacDown( 0, BACKWARD );
				g_Run_Unload_Works.ReqVacDown( 1, BACKWARD );
				job.bvis_step = BVISION_STEP(311);
			}

		}

		break;

	case 311:
		{
			m_zPos = st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP];
			int nRet = CTL_Lib.Single_Move( M_ULD_Z, m_zPos, st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = BVISION_STEP(312);
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1938, CTL_dWARNING, alarm.mstr_code );
			}

			st_var.n_index_move_sidecover_fold_flag = CTL_NO;//2015.0311
			
		}
		break;

	case 312:
		if( m_fbVacDown[0] != FB_NONE) break;
		if( m_fbVacDown[1] != FB_NONE) break;

		if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_up_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_uld_rbt_left_down_chk, IO_OFF) == IO_ON)
		{
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[0], IO_OFF);
			job.bvis_step = BVISION_STEP(311);
		}
		else if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_up_chk, IO_ON) == IO_OFF ||
			g_ioMgr.get_in_bit( st_io.i_uld_rbt_right_down_chk, IO_OFF) == IO_ON)
		{
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[1], IO_OFF);
			job.bvis_step = BVISION_STEP(311);
		}
		else
		{
			job.bvis_step = BVISION_PLACE_MOVE;
		}
		break;

//	case BVISION_PLACE_MOVE_CONVEYOR:
	case BVISION_PLACE_MOVE:
		{
			if(st_basic.mn_outconv == CTL_YES)
			{
				m_dPos = g_handler.CalcTargetPosition(M_ULD_Y, PLACE, job.iPosFirst, job.iRobotFirst );
			}
			else
			{
				if(st_basic.mn_outtbox == CTL_NO)//Rejevt Tray
				{
					m_dPos = st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS];
				}
				else//Conveyor Tray
				{
					m_dPos = st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS];
				}
			}

			int nRet1 = CTL_Lib.Single_Move( M_ULD_Y, m_dPos, st_basic.nRunSpeed );
			if( nRet1 ==  BD_GOOD )
			{
				job.bvis_step = BVISION_PLAC_MOVE_PICKER_DOWN;
				//2016.0119
				//m_dwTimeMove = GetCurrentTime();
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1246, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
	


//	case BVISION_PLAC_MOVE_CONVEYOR_PICKER_DOWN:
	case BVISION_PLAC_MOVE_PICKER_DOWN:
		{

			//2016.0119
// 			if( GetCurrentTime() - m_dwTimeMove <= 0 )
// 			{
// 				m_dwTimeMove = GetCurrentTime();
// 				break;
// 			}
// 			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_PICKER][IO_ON]) break;
// 			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
// 			{
// 				if(g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_OFF )
// 				{
// 					CString strError;
// 					strError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
// 					CTL_Lib.Alarm_Error_Occurrence( 3239, CTL_dWARNING, strError );
// 					break;
// 				}
// 			}
// 			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
// 			{
// 				if( g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_OFF )
// 				{
// 					CString strError;
// 					strError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
// 					CTL_Lib.Alarm_Error_Occurrence( 3240, CTL_dWARNING, strError );
// 					break;
// 				}
// 			}


			//2015.0318
			if( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] - COMI.md_allow_value[M_ULD_Y] &&
				COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] + COMI.md_allow_value[M_ULD_Y])
			{
				
			}
			else if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_OFF ) == IO_ON || 
				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk, IO_OFF ) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk, IO_OFF ) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_OFF) == IO_ON )
			{
				break;
			}
 			if(st_handler.mn_OutElv_work == CTL_YES)
			{
				if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk, IO_OFF) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
				{
					break;
				}
				
				if(st_basic.mn_outconv != CTL_YES)
				{//2014.0304
					job.bvis_step = BVISION_PLACE_MOVE_CALC;
					break;
				}
			}


			if( job.iRobotFirst != -1/* && st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES*/)
			{
				//g_Run_Unload_Works.ReqVacDown( job.iRobotFirst, FORWARD );
			}
			if( job.iRobotSecond != -1/* && st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES*/)
			{
				//g_Run_Unload_Works.ReqVacDown( job.iRobotSecond,\ FORWARD );
			}
//			job.bvis_step = BVISION_PLAC_MOVE_CONVEYOR_Z_DOWN;
			job.bvis_step = BVISION_PLAC_MOVE_Z_DOWN;
		}
		break;

	case BVISION_PLAC_MOVE_Z_DOWN:
		{
			if(st_basic.mn_outconv == CTL_YES)
			{
				m_dPos = g_handler.CalcTargetPosition(M_ULD_Z, PLACE, job.iPosFirst, job.iRobotFirst );
			}
			else
			{
				if(st_basic.mn_outtbox == CTL_NO)//Rejevt Tray
				{
					if( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] - COMI.md_allow_value[M_ULD_Y] &&
						COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] + COMI.md_allow_value[M_ULD_Y] )
					{
						m_dPos = st_motor[M_ULD_Z].d_pos[P_ULD_Z_REJ_POS];
					}
					else
					{
						job.bvis_step = BVISION_PLACE_MOVE;
						break;
					}
				}
				else//Conveyor Tray
				{
					m_dPos = st_motor[M_ULD_Z].d_pos[P_ULD_Z_CONV_POS];
				}
			}
			int nRet1 = CTL_Lib.Single_Move( M_ULD_Z, m_dPos, st_basic.nRunSpeed );
			if( nRet1 ==  BD_GOOD )
			{
				job.bvis_step = BVISION_PLACE_MOVE_PICKER_DOWN_CHK;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1247, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BVISION_PLACE_MOVE_PICKER_DOWN_CHK:
		if( st_basic.n_mode_device && job.iRobotFirst != -1 && /*st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES &&*/ g_Run_Unload_Works.GetVacDown( job.iRobotFirst ) != FB_NONE )		break;
		if( st_basic.n_mode_device && job.iRobotSecond != -1 && /*st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES &&*/ g_Run_Unload_Works.GetVacDown( job.iRobotSecond ) != FB_NONE )		break;
		m_dwTimeMove = GetCurrentTime();
		job.bvis_step = BVISION_PLACE_VACUUM;
		break;

	case BVISION_PLACE_VACUUM:
		{
// 			if( st_handler.mn_OutElv_work == CTL_YES || 
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_OFF ) == IO_ON || 
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk, IO_OFF ) == IO_ON ||
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk, IO_OFF ) == IO_ON ||
// 				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_OFF) == IO_ON )
// 			{
// 				break;
// 			}

// 			if(st_handler.mn_OutElv_work == CTL_YES) break;


			//2015.0318
			if( COMI.Get_MotCurrentPos(M_ULD_Y) > st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] - COMI.md_allow_value[M_ULD_Y] &&
				COMI.Get_MotCurrentPos(M_ULD_Y) < st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS] + COMI.md_allow_value[M_ULD_Y])
			{
				
			}
			else if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk, IO_OFF ) == IO_ON || 
				g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk, IO_OFF ) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk, IO_OFF ) == IO_ON ||
				g_ioMgr.get_in_bit( st_io.i_uld_cv_box_out_chk, IO_OFF) == IO_ON )
			{
				break;
			}
// 			if( GetCurrentTime() - m_dwTimeMove <= 0 ) m_dwTimeMove = GetCurrentTime();
// 			if( GetCurrentTime() - m_dwTimeMove < 200 ) break;
			if(st_handler.mn_OutElv_work == CTL_YES)
			{
				if( g_ioMgr.get_in_bit( st_io.i_recycle_zone_box_vacuum_chk, IO_OFF) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_recycle_zone_box1_chk, IO_OFF ) == IO_ON ||
					g_ioMgr.get_in_bit( st_io.i_recycle_zone_box2_chk, IO_OFF ) == IO_ON )
				{
					break;
				}				
			}

			if( st_basic.mn_outconv == CTL_YES)
			{
				job.bvis_step = BVISION_STEP(361);//INLINE
			}
			else
			{
				job.bvis_step = BVISION_STEP(364);
			}
		}
		break;

	case 361:
		if( st_basic.mn_outconv == CTL_YES)//2015.0210
		{

			if (st_client[CLS_NEXT].n_connect == CTL_YES)
			{
				if(g_lotMgr.GetLotAt(0).GetNewLotInPass() == false )
				{
					//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
					job.bvis_step = BVISION_STEP(362);
				}
				else
					job.bvis_step = BVISION_STEP(364);
			}
			else
			{
				mn_retry = 0;
				m_dwTimeMove = GetCurrentTime();
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CONNECT, CLS_NEXT);
				job.bvis_step = BVISION_STEP(362);
			}
		}
		else// if( st_basic.mn_outconv == CTL_NO )
		{
			job.bvis_step = BVISION_STEP(364);
		}
		break;

	case 362:
		if (st_client[CLS_NEXT].n_connect == CTL_YES)
		{
			m_dwTimeMove = GetCurrentTime();
			job.bvis_step = BVISION_STEP(364);
		}
		else
		{			
			if ( GetCurrentTime() - m_dwTimeMove <= 0)
			{
				m_dwTimeMove = GetCurrentTime();
				break;
			}
			
			if ( GetCurrentTime() - m_dwTimeMove > st_basic.mn_network_wait_time)
			{
				mn_retry++;
				if (mn_retry >= 3)
				{
					CString strJamCode;
					mn_retry = 0;
					
					//750000 1 00 "NEXT_Machine is not response."
					CTL_Lib.Alarm_Error_Occurrence(1195, CTL_dWARNING, "750000" );
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
					job.bvis_step = BVISION_STEP(363);
				}
				else
				{
					m_dwTimeMove = GetCurrentTime();
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
					job.bvis_step = BVISION_STEP(363);
				}
			}
		}
		break;

	case 363:		
		if ( GetCurrentTime() - m_dwTimeMove <= 0)
		{
			m_dwTimeMove = GetCurrentTime();
			break;
		}
		
		if (GetCurrentTime() - m_dwTimeMove > 5000)
		{
			job.bvis_step = BVISION_STEP(361);
		}
		break;

	case 364:
		if( st_basic.mn_outconv == CTL_YES)//2015.0210
		{
			if(g_lotMgr.GetLotAt(0).GetNewLotInPass() == false )
			{
				g_next_client.OnMove_BufferIn();
				m_dwTimeMove = GetCurrentTime();
				job.bvis_step = BVISION_STEP(365);
			}
			else
			{
// 				g_next_client.OnMove_BufferIn();
				job.bvis_step = BVISION_STEP(366);
			}
		}
		else// if( st_basic.mn_outconv == CTL_NO )
		{
			job.bvis_step = BVISION_STEP(366);
		}
		break;

	case 365:
		if( g_lotMgr.GetLotAt(0).GetNewLotInPass() == true ||
			g_next_client.GetBufferInfo())
		{
// 			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
			job.bvis_step = BVISION_STEP(363);
			job.bvis_step = BVISION_STEP(366);
		}
		if( GetCurrentTime() - m_dwTimeMove <= 0) m_dwTimeMove = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTimeMove > MES_TIME_OUT)
		{
// 			Func.OnSet_IO_Port_Stop();
			CString strErr;
			strErr.Format( "[NEXT Machine] Next Machine is not response");
			st_msg.mstr_event_msg[0] = strErr;
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			job.bvis_step = BVISION_STEP(367);

			m_dwTimeMove = GetCurrentTime();
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CLOSE, CLS_NEXT);
		}
		break;

	case 366:
		if( job.iRobotFirst != -1 /*&& st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES*/ )
		{
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[job.iRobotFirst], IO_OFF );
			//2015.0417
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotFirst], IO_ON );
			st_handler.mn_buffer_in++;
		}
		if( job.iRobotSecond != -1 /*&& st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES*/ )
		{
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_vacuum[job.iRobotSecond], IO_OFF );
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotSecond], IO_ON );
			st_handler.mn_buffer_in++;
		}
		m_dwTimeMove = GetCurrentTime();
		job.bvis_step = BVISION_PLACE_UP;
		break;

	case 367:
		m_dwTimeMove = GetCurrentTime();
		job.bvis_step = BVISION_STEP(361);
		if( st_basic.mn_outconv == CTL_NO)
		{
			job.bvis_step = BVISION_STEP(366);
		}
		break;

	case BVISION_PLACE_UP:
		{
			if( GetCurrentTime() - m_dwTimeMove < 0) m_dwTimeMove = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeMove < st_time.n_wait_time[E_WAIT_UNLOAD_VACUUM][IO_OFF]) break;

			if( job.iRobotFirst != -1)
			{
				//2015.0417
				//g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotFirst], IO_OFF );
				//g_Run_Unload_Works.ReqVacDown( job.iRobotFirst, BACKWARD );
			}
			if( job.iRobotSecond != -1)
			{
				//g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[job.iRobotSecond], IO_OFF );
				//g_Run_Unload_Works.ReqVacDown( job.iRobotSecond, BACKWARD );
			}

			job.bvis_step = BVISION_PLACE_UP_CHK;
			break;

	case BVISION_PLACE_UP_CHK:
			int nRet1 = CTL_Lib.Single_Move( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP], st_basic.nRunSpeed );
			if( nRet1 ==  BD_GOOD )
			{
// 				m_dwTimeMove = GetCurrentTime();
				job.bvis_step = BVISION_PLACE_VACUUM_CHK;
				g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[0], IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_uld_rbt_eject[1], IO_OFF );
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1248, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case BVISION_PLACE_VACUUM_CHK:
		{
// 			if( GetCurrentTime() - m_dwTimeMove < 0) m_dwTimeMove = GetCurrentTime();
			if( st_basic.n_mode_device && job.iRobotFirst != -1 && /*st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES &&*/ g_Run_Unload_Works.GetVacDown( job.iRobotFirst ) != FB_NONE )		break;
			if( st_basic.n_mode_device && job.iRobotSecond != -1 && /*st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES &&*/ g_Run_Unload_Works.GetVacDown( job.iRobotSecond ) != FB_NONE )		break;

			if( st_basic.n_mode_device && job.iRobotFirst != -1 /*&& st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES*/ && 
				g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst] , IO_ON) == IO_ON )
			{
				CString sterError="";
				sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
				CTL_Lib.Alarm_Error_Occurrence( 1249, CTL_dWARNING, sterError );
				break;
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1 /*&& st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES*/ &&
				g_ioMgr.get_in_bit( st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond] , IO_ON) == IO_ON )
			{
				CString sterError="";
				sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond]);
				CTL_Lib.Alarm_Error_Occurrence( 1250, CTL_dWARNING, sterError );
				break;
			}
// 			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
// 			{ 
// 				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) != IO_ON )
// 				{
// 					if( GetCurrentTime() - m_dwTimeMove < st_time.n_limit_time[E_WAIT_UNLOAD_VACUUM]) break;
// 
// 					CString sterError;
// 					sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
// 					CTL_Lib.Alarm_Error_Occurrence( 1249, CTL_dWARNING, sterError );
// 					break;
// 				}
// 			}
// 			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
// 			{ 
// 				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) != IO_ON )
// 				{
// 					if( GetCurrentTime() - m_dwTimeMove < st_time.n_limit_time[E_WAIT_UNLOAD_VACUUM]) break;
// 					CString sterError;
// 					sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond]);
// 					CTL_Lib.Alarm_Error_Occurrence( 1250, CTL_dWARNING, sterError );
// 					break;
// 				}
// 			}
		}
		job.bvis_step = BVISION_PLACE_FINISH;
// 		job.bvis_step = BVISION_STEP(391);//2014,1207
		//2016.0119
		m_dwTimeMove = GetCurrentTime();
		break;

	////2014,1207
	case 391:
		{
// 			if( g_ioMgr.get_in_bit( st_io.i_uld_cv_box_full_chk , IO_OFF) == IO_ON )
// 			{
// 				CString sterError;
// 				sterError.Format("5%04d0",st_io.i_uld_cv_box_full_chk);
// 				CTL_Lib.Alarm_Error_Occurrence( 1250, CTL_dWARNING, sterError );
// 				break;
// 			}
			//2016.0119
			int i[2]; i[0] = 0; i[1] = 0;
			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
			{ 
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) != IO_ON )
				{
					i[0] = 1;
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
			{ 
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) != IO_ON )
				{
					i[1] = 1;
				}
			}

			if( st_basic.n_mode_device && i[0] != 1 && i[1] != 1 )
			{
				job.bvis_step = BVISION_STEP(392);
			}
			else if( st_basic.n_mode_device && i[0] != 1 )
			{
				job.bvis_step = BVISION_STEP(393);
			}
			else if( st_basic.n_mode_device && i[1] != 1 )
			{
				job.bvis_step = BVISION_STEP(394);
			}
			else
			{
				job.bvis_step = BVISION_STEP(395);
			}
		}
		break;

	case 392:
		{
			int i[2]; i[0] = 0; i[1] = 0;
			job.bvis_step = BVISION_STEP(395);
			if( GetCurrentTime() - m_dwTimeMove <= 0) m_dwTimeMove = GetCurrentTime();
			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
			{ 
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) != IO_ON )
				{
					if( GetCurrentTime() - m_dwTimeMove < 500 ) break;
					job.bvis_step = BVISION_STEP(393);
				}
			}
			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
			{ 
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) != IO_ON )
				{
					if( GetCurrentTime() - m_dwTimeMove < 500 ) break;
					job.bvis_step = BVISION_STEP(394);
				}
			}
		}
		break;

	case 393:
		{
			if( GetCurrentTime() - m_dwTimeMove <= 0) m_dwTimeMove = GetCurrentTime();
			if( st_basic.n_mode_device && job.iRobotFirst != -1 )
			{ 
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_left_box_chk) != IO_ON )
				{
					if( GetCurrentTime() - m_dwTimeMove < 500 ) break;
					CString sterError="";
					sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotFirst]);
					CTL_Lib.Alarm_Error_Occurrence( 1249, CTL_dWARNING, sterError );

					job.bvis_step = BVISION_STEP(395);
				}
			}
		}
		break;

	case 394:
		{
			if( GetCurrentTime() - m_dwTimeMove <= 0) m_dwTimeMove = GetCurrentTime();
			if( st_basic.n_mode_device && job.iRobotSecond != -1 )
			{ 
				if( g_ioMgr.get_in_bit( st_io.i_uld_cv_right_box_chk) != IO_ON )
				{
					if( GetCurrentTime() - m_dwTimeMove < 500 ) break;
					CString sterError="";
					sterError.Format("5%04d0",st_io.i_uld_rbt_vacuum_chk[job.iRobotSecond]);
					CTL_Lib.Alarm_Error_Occurrence( 1250, CTL_dWARNING, sterError );
					job.bvis_step = BVISION_STEP(395);
				}
			}
		}
		break;;

	case 395:
		{
			job.bvis_step = BVISION_PLACE_FINISH;
		}
		break;
		////

	case BVISION_PLACE_FINISH:
		{
			if( job.iRobotFirst != -1 )
			{
				if(st_handler.mn_Btm_Vision[job.iRobotFirst] == CTL_YES)
				{
					g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddPassCnt( PRIME );
					g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddPassCnt( CUM );
				}
				else
				{
					g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddFailCnt( PRIME );
					//g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddFailCnt( CUM );
					st_msg.mstr_event_msg[0] = "[Reject] Reject TBox Check please.";
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 2);

					CString strError="";
					strError.Format("1. LotID: %s, IDBuffer: %s, Serial: %s", 
						m_strbcr_lotid[job.iRobotFirst], m_strbcr_idbuff[job.iRobotFirst], m_strbcr_serial[job.iRobotFirst]);

					Func.On_LogFile_Add(LOG_REJECT, strError);
				}
				
// 				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].st_uld_rbt[job.iRobotFirst], &st_uld_rbt[job.iRobotFirst].st_btm, sizeof(st_btm_param) );
				memset(&st_uld_rbt.st_btm[job.iRobotFirst], 0x00, sizeof(st_uld_rbt.st_btm[job.iRobotFirst]));
				st_uld_rbt.bPick[job.iRobotFirst] = false;
				st_uld_rbt.n_exist[job.iRobotFirst] = 0;
				st_var.n_tbox_output_count++;

			}
			if( job.iRobotSecond != -1 )
			{
				if(st_handler.mn_Btm_Vision[job.iRobotSecond] == CTL_YES)
				{
					g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddPassCnt( PRIME );
					g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddPassCnt( CUM );
				}
				else
				{
					g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddFailCnt( PRIME );
					//g_lotMgr.GetLotByLotID(g_lotMgr.GetLotAt(0).GetLotID()).AddFailCnt( CUM );
					st_msg.mstr_event_msg[0] = "[Reject] Reject TBox Check please.";
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 2);

					CString strError="";
					strError.Format("2. LotID: %s, IDBuffer: %s, Serial: %s", 
						m_strbcr_lotid[job.iRobotSecond], m_strbcr_idbuff[job.iRobotSecond], m_strbcr_serial[job.iRobotSecond]);
					
					Func.On_LogFile_Add(LOG_REJECT, strError);
				}

// 				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].st_uld_rbt[job.iRobotSecond], &st_uld_rbt[job.iRobotSecond].st_btm, sizeof(st_btm_param) );
				memset(&st_uld_rbt.st_btm[job.iRobotSecond], 0x00, sizeof(st_uld_rbt.st_btm[job.iRobotSecond]));
				st_uld_rbt.bPick[job.iRobotSecond] = false;
				st_uld_rbt.n_exist[job.iRobotSecond] = 0;
				st_var.n_tbox_output_count++;
			}

			//2015.0222
			if( st_uld_rbt.n_exist[0] || st_uld_rbt.n_exist[1] || 
				Func.m_pRobot[SITE_UNLOAD]->GetJobCount() >= 2 )//fail 모듈이 있다.
			{
				if( Func.m_pRobot[SITE_UNLOAD]->GetJobCount() >= 2 )
				{
					Func.m_pRobot[SITE_UNLOAD]->EraseFirstJob();
					Func.Handler_Recovery_Data_Write();
					break;
				}

			}
			else
			{				
				if( g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" )
				{
					if( ( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) + g_lotMgr.GetLotAt(0).GetFailCnt(PRIME) ) >= atoi(g_lotMgr.GetLotAt(0).GetStrTotalModule() ) )
					{
						if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < atoi(g_lotMgr.GetLotAt(0).GetStrTotalModule() ) )
						{
							st_msg.mstr_event_msg[0] = "[LOT END] There is a previous lot";
							::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 2);
							//st_handler.mn_chk_lotend = CTL_YES;
							//2016.1215
							st_handler.mn_chk_lotend = CTL_READY;
							st_handler.mn_reworkcnt = 0;
						}
					}
				}					
			}
			job.bvis_step = (BVISION_STEP)401;
		}
		break;

	case 401:
		{
			int nRet = CTL_Lib.Single_Move( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = (BVISION_STEP)402;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 4200, CTL_dWARNING, alarm.mstr_code );
			}
			
		}
		break;

	case 402:
		{
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[0], IO_ON );
			g_ioMgr.set_out_bit( st_io.o_uld_rbt_picker[1], IO_ON );
			int nRet = COMI.Start_SingleMove( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_READY_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.bvis_step = (BVISION_STEP)403;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 4201, CTL_dWARNING, alarm.mstr_code );
			}
			job.bvis_step = (BVISION_STEP)403;
		}
		break;

	case 403:
		{
			int nRet = COMI.Check_SingleMove( M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_READY_POS] );
			if( nRet == BD_GOOD )
			{
				Func.m_pRobot[SITE_UNLOAD]->EraseFirstJob();				
				Func.Handler_Recovery_Data_Write();

				if(GetCurrentTime() - m_dwTimeTact <= 0 ) m_dwTimeTact = GetCurrentTime();
				if( GetCurrentTime() - m_dwTimeTact >= 5900 && GetCurrentTime() - m_dwTimeTact < 9000000 &&
					st_handler.cwnd_list != NULL)
				{
					int nTact = GetCurrentTime() - m_dwTimeTact;
					//if(nTact < 30000)
					//{
						//st_handler.mn_unloadtact = OnTact(nTact);//2015.0221
						st_handler.mn_unloadtact = nTact;
					//}

					if( st_basic.mn_mode_xgem == CTL_YES )
					{
						if( job.iRobotFirst != -1 )
						{
							//st_xgem.m_str_buffer1_id.Format("%d", m_strbcr_idbuff[job.iRobotFirst]);
							//2017.0510
							st_xgem.m_str_buffer1_id.Format("%s", m_strbcr_idbuff[job.iRobotFirst]);
							st_xgem.m_str_buffer1_serial.Format("%s", m_strbcr_serial[job.iRobotFirst]);
							st_xgem.m_strBuf1TactTime.Format("%.3f", (float)st_handler.mn_unloadtact);
						}
						if( job.iRobotSecond != -1 )
						{
							//st_xgem.m_str_buffer2_id.Format("%d", m_strbcr_idbuff[job.iRobotSecond]);
							//2017.0510
							st_xgem.m_str_buffer2_id.Format("%s", m_strbcr_idbuff[job.iRobotSecond]);
							st_xgem.m_str_buffer2_serial.Format("%s", m_strbcr_serial[job.iRobotSecond]);
							st_xgem.m_strBuf2TactTime.Format("%.3f", (float)st_handler.mn_unloadtact);
						}
						if( job.iRobotFirst != -1 || job.iRobotSecond != -1)
						{
							g_xgem_client.XgemBufferMove();
						}
					}

					sprintf(st_msg.c_normal_msg, "[UNLOAD]Tact time : %d", GetCurrentTime() - m_dwTimeTact);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
				}
				m_dwTimeTact = GetCurrentTime();
				g_handler.CheckLotEnd();//2014,1227
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				job.bvis_step = (BVISION_STEP)402;
				CTL_Lib.Alarm_Error_Occurrence( 4202, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;
	}

}

int CRun_Unload_Works::OnTact(int nTact)
{
	vecTact.push_back(nTact);
	int nSum=0, nRet=0, i = 0;
	if(vecTact.size() < 1) return 0;
	if(vecTact.size() <= 10 )
	{
		for ( i = 0; i<vecTact.size(); i++)
		{
			nSum += vecTact[i];
			nRet = nSum/vecTact.size();
		}
	}
	else
	{
		for(;;)
		{
			std::vector<int>::iterator iter = vecTact.begin();
			vecTact.erase(iter);
			if(vecTact.size() <= 10)
			{
				break;
			}
		}

		for ( i = 0; i<vecTact.size(); i++)
		{
			nSum += vecTact[i];
		}
		nRet = nSum/vecTact.size();
	}

	return nRet;
}

int CRun_Unload_Works::OnServerSendBoatID()
{
// 	int nFuncRet = CTL_PROCEED;
// 	switch(mn_server_step)
// 	{
// 	case 0: 
// 		mn_server_step = 10;
// 		mn_pos = CTL_LEFT;
// 		break;
// 
// 	case 10:
// 		m_nErrRetryCnt = 0;
// 		m_nidRetryCnt = 0;
// 		m_strRetry = "NO";
// 		m_strCnt = "0";
// 		mn_server_step = 1000;
// 		break;
// 
// 	case 100:
// 		m_nErrRetryCnt = 0;
// 		m_nidRetryCnt++;
// 		if( m_nidRetryCnt > 3) m_nidRetryCnt = 1;
// 		m_strRetry = "YES";
// 		m_strCnt.Format("%d", m_nidRetryCnt);
// 		mn_server_step = 1000;
// 		break;
// 
// 	case 1000:
// 		if (st_client[CLS_FORMAT].n_connect == CTL_YES)
// 		{
// 			m_dwTime_Network = GetCurrentTime();
// 			if( m_bUse_idbuff[mn_pos] == true )
// 				mn_server_step = 2000;
// 			else
// 				mn_server_step = 1010;
// 
// 		}
// 		else
// 		{
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_5, CLIENT_CONNECT, CLS_FORMAT);
// 			mn_server_step = 1100;
// 		}
// 		break;
// 
// 	case 1010:
// 		if( m_bUse_idbuff[CTL_LEFT] == false && m_bUse_idbuff[CTL_RIGHT] == false )
// 		{
// 			mn_server_step = 0;
// 			nFuncRet = CTL_GOOD;
// 			break;
// 		}
// 		mn_pos = CTL_RIGHT;
// 		mn_server_step = 10;
// 		break;
// 
// 	case 1100:
// 		if (st_client[CLS_FORMAT].n_connect == CTL_YES)
// 		{
// 			m_dwTime_Network = GetCurrentTime();
// 			mn_server_step = 2000;
// 		}
// 		else
// 		{
// 			if( GetCurrentTime() - m_dwTime_Network <= 0 ) m_dwTime_Network = GetCurrentTime();
// 			if( GetCurrentTime() - m_dwTime_Network > (st_basic.mn_network_wait_time*5) )
// 			{
// 				m_nErrRetryCnt++;
// 				if( m_nErrRetryCnt > st_basic.n_retry_cnt)
// 				{
// 					m_nErrRetryCnt = 0;
// 					m_dwTime_Network = GetCurrentTime();
// 					mn_server_step = 1300;
// 				}
// 				else
// 				{
// 					m_dwTime_Network = GetCurrentTime();
// 					mn_server_step = 1300;
// 
// 				}
// 			}
// 		}
// 		break;
// 
// 	case 1300:
// 		if( GetCurrentTime() - m_dwTime_Network <= 0 ) m_dwTime_Network = GetCurrentTime();
// 		if( GetCurrentTime() - m_dwTime_Network > st_basic.mn_network_wait_time )
// 			mn_server_step = 1000;
// 		break;
// 
// 	case 2000:
// 		clsEC_Client.OnBoatIDModify(CLS_FORMAT, m_strbcr_lotid[CTL_LEFT], m_strbcr_idbuff[CTL_LEFT], m_strbcr_serial[CTL_LEFT], "END", m_strRetry, m_strCnt);
// 		m_dwTime_Network = GetCurrentTime();
// 		mn_server_step = 2100;
// 		break;
// 
// 	case 2100:
// 		if( GetCurrentTime() - m_dwTime_Network <= 0 ) m_dwTime_Network = GetCurrentTime();
// 		if( clsEC_Client.m_nrBoatID == NR_RECEIVED || clsEC_Client.m_nrBoatID == NR_FAIL)
// 		{
// 			m_bUse_idbuff[mn_pos] = false;
// 			mn_server_step = 1010;
// 		}
// 		else if( GetCurrentTime() - m_dwTime_Network > (st_basic.mn_network_wait_time*5) )
// 		{
// 			m_nErrRetryCnt++;
// 			if( m_nErrRetryCnt > st_basic.n_retry_cnt)
// 			{
// 				m_nErrRetryCnt = 0;
// 				m_dwTime_Network = GetCurrentTime();
// 				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_5, CLIENT_CLOSE, CLS_FORMAT);
// 				mn_server_step = 2200;
// 			}
// 			else
// 			{
// 				m_dwTime_Network = GetCurrentTime();
// 				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_5, CLIENT_CLOSE, CLS_FORMAT);
// 				mn_server_step = 2200;
// 				
// 			}
// 		}
// 		break;
// 
// 	case 2200:
// 		if( GetCurrentTime() - m_dwTime_Network <= 0 ) m_dwTime_Network = GetCurrentTime();
// 		if( GetCurrentTime() - m_dwTime_Network > st_basic.mn_network_wait_time )
// 			mn_server_step = 100;
// 		break;
// 	}
// 	return nFuncRet;
	int nFuncRet = CTL_PROCEED;

	int nRet = VAR_INIT_;
	CString sSerial = "";

	switch(m_nStep_MES)
	{
	case 0: 
		// [Eqp->MES] 메시지 전송 요청을 기다림
		//if (m_nReq_MES == WORK_SUPPLY_)
		{
			m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
			m_nStep_MES = 100;
		}
		break;

	case 100:
		// - 이곳에서 실재로 자재에 대한 정보를 확인해야 할 부분은 (TRACE_PK_SSD_) 구조체 부분임
		sSerial.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[0]);
		if (sSerial != "")
		{
			evMes.OnPutToQueue(MES_BOAT_ID_MODIF_LOUT_);
			
			m_lWaitResp[0] = GetCurrentTime();
			m_nStep_MES = 110;
		}
		// 공백인 경우에는 메시지 보고하지 않음
		else
		{
			m_nStep_MES = 200;
		}
		break;

	case 110:
		nRet = evMes.OnGet_MesWork();
		// 정상적으로 에코 수신
		if (nRet == REPLY_CORRECT_)
		{
			if ((st_client[0].stComm[MES_BOAT_ID_MODIF_LOUT_]).nResponse == CTL_GOOD)
			{
				m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
				m_nStep_MES = 200;
			}
			else
			{
				m_lWaitResp[1] = GetCurrentTime();
				m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
				if (m_lWaitResp[2] < 0)
				{
					m_lWaitResp[0] = GetCurrentTime();
					break;
				}
				
				if (m_lWaitResp[2] > (MES_TIME_OUT+1000))
				{
					m_nRetryCount++;	// 재시도 횟수 증가
					if (m_nRetryCount > 2)
					{
						m_nRetryCount = 2;
					}
					m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
					
					if (m_nRetryCount >= 2)
					{
						// 통신에 문제 발생 [뾰족한 해결 방법 없음]
						m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
						m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
						
						// 900103 1 0 "MES_NOT_RESPONSE_ERR"
						alarm.mstr_code		= "900103";
						CTL_Lib.Alarm_Error_Occurrence(1290, CTL_dWARNING, alarm.mstr_code );
					}
				}
			}
		}
		// 에코 수신에 에러 발생
		else if (nRet == REPLY_ERROR_)
		{
			if ((st_client[0].stComm[MES_BOAT_ID_MODIF_LOUT_]).nResponse == CTL_ERROR)
			{
				m_nRetryCount++;	// 재시도 횟수 증가
				if (m_nRetryCount > 2)
				{
					m_nRetryCount = 2;
				}
				m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프

				if (m_nRetryCount >= 2)
				{
					// 통신에 문제 발생 [뾰족한 해결 방법 없음]
					m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
					m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
					
					// 900103 1 0 "MES_NOT_RESPONSE_ERR"
					alarm.mstr_code		= "900103";
					CTL_Lib.Alarm_Error_Occurrence(1291, CTL_dWARNING, alarm.mstr_code );
				}
			}
			else
			{
				m_lWaitResp[1] = GetCurrentTime();
				m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
				if (m_lWaitResp[2] < 0)
				{
					m_lWaitResp[0] = GetCurrentTime();
					break;
				}
				
				if (m_lWaitResp[2] > (MES_TIME_OUT+1000))
				{
					m_nRetryCount++;	// 재시도 횟수 증가
					if (m_nRetryCount > 2)
					{
						m_nRetryCount = 2;
					}
					m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
					
					if (m_nRetryCount >= 2)
					{
						// 통신에 문제 발생 [뾰족한 해결 방법 없음]
						m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
						m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
						
						// 900101 1 0 "MES_NOT_RESPONSE_ERR"
						alarm.mstr_code		= "900101";
						CTL_Lib.Alarm_Error_Occurrence(1292, CTL_dWARNING, alarm.mstr_code );
					}
				}
			}
		}
		else
		{
			m_lWaitResp[1] = GetCurrentTime();
			m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
			if (m_lWaitResp[2] < 0)
			{
				m_lWaitResp[0] = GetCurrentTime();
				break;
			}
			
			if (m_lWaitResp[2] > (MES_TIME_OUT+1000))
			{
				m_nRetryCount++;	// 재시도 횟수 증가
				if (m_nRetryCount > 2)
				{
					m_nRetryCount = 2;
				}
				m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
				
				if (m_nRetryCount >= 2)
				{
					// 통신에 문제 발생 [뾰족한 해결 방법 없음]
					m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
					m_nStep_MES = 100;	// 통신 재시도 스텝으로 점프
					
					// 900101 1 0 "MES_NOT_RESPONSE_ERR"
					alarm.mstr_code		= "900101";
					CTL_Lib.Alarm_Error_Occurrence(1293, CTL_dWARNING, alarm.mstr_code );
				}
			}
		}
		break;

	case 200:
		sSerial.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[0]);
		if (sSerial != "")
		{
			evMes.OnPutToQueue(MES_BOAT_ID_MODIF_ROUT_);

			m_lWaitResp[0] = GetCurrentTime();
			m_nStep_MES = 210;
		}
		// 공백인 경우에는 메시지 보고하지 않음
		else
		{
			m_nStep_MES = 300;
		}
		break;

	case 210:
		nRet = evMes.OnGet_MesWork();
		// 정상적으로 에코 수신
		if (nRet == REPLY_CORRECT_)
		{
			if ((st_client[0].stComm[MES_BOAT_ID_MODIF_ROUT_]).nResponse == CTL_GOOD)
			{
				m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
				m_nStep_MES = 300;
			}
			else
			{
				m_lWaitResp[1] = GetCurrentTime();
				m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
				if (m_lWaitResp[2] < 0)
				{
					m_lWaitResp[0] = GetCurrentTime();
					break;
				}
				
				if (m_lWaitResp[2] > (MES_TIME_OUT+1000))
				{
					m_nRetryCount++;	// 재시도 횟수 증가
					if (m_nRetryCount > 2)
					{
						m_nRetryCount = 2;
					}
					m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
					
					if (m_nRetryCount >= 2)
					{
						// 통신에 문제 발생 [뾰족한 해결 방법 없음]
						m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
						m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
						
						// 900103 1 0 "MES_NOT_RESPONSE_ERR"
						alarm.mstr_code		= "900101";
						CTL_Lib.Alarm_Error_Occurrence(1295, CTL_dWARNING, alarm.mstr_code );
					}
				}
			}
		}
		// 에코 수신에 에러 발생
		else if (nRet == REPLY_ERROR_)
		{
			if ((st_client[0].stComm[MES_BOAT_ID_MODIF_ROUT_]).nResponse == CTL_ERROR)
			{
				m_nRetryCount++;	// 재시도 횟수 증가
				if (m_nRetryCount > 2)
				{
					m_nRetryCount = 2;
				}
				m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프

				if (m_nRetryCount >= 2)
				{
					// 통신에 문제 발생 [뾰족한 해결 방법 없음]
					m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
					m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
					
					// 900103 1 0 "MES_NOT_RESPONSE_ERR"
					alarm.mstr_code		= "900103";
					CTL_Lib.Alarm_Error_Occurrence(1299, CTL_dWARNING, alarm.mstr_code );
				}
			}
			else
			{
				m_lWaitResp[1] = GetCurrentTime();
				m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
				if (m_lWaitResp[2] < 0)
				{
					m_lWaitResp[0] = GetCurrentTime();
					break;
				}
				
				if (m_lWaitResp[2] > (MES_TIME_OUT+1000))
				{
					m_nRetryCount++;	// 재시도 횟수 증가
					if (m_nRetryCount > 2)
					{
						m_nRetryCount = 2;
					}
					m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
					
					if (m_nRetryCount >= 2)
					{
						// 통신에 문제 발생 [뾰족한 해결 방법 없음]
						m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
						m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
						
						// 900103 1 0 "MES_NOT_RESPONSE_ERR"
						alarm.mstr_code		= "900101";
						CTL_Lib.Alarm_Error_Occurrence(1295, CTL_dWARNING, alarm.mstr_code );
					}
				}
			}
		}
		else
		{
			m_lWaitResp[1] = GetCurrentTime();
			m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
			if (m_lWaitResp[2] < 0)
			{
				m_lWaitResp[0] = GetCurrentTime();
				break;
			}
			
			if (m_lWaitResp[2] > (MES_TIME_OUT+1000))
			{
				m_nRetryCount++;	// 재시도 횟수 증가
				if (m_nRetryCount > 2)
				{
					m_nRetryCount = 2;
				}
				m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
				
				if (m_nRetryCount >= 2)
				{
					// 통신에 문제 발생 [뾰족한 해결 방법 없음]
					m_nRetryCount = 0;	// [리셋] 작업 재시도 횟수
					m_nStep_MES = 200;	// 통신 재시도 스텝으로 점프
					
					// 900101 1 0 "MES_NOT_RESPONSE_ERR"
					alarm.mstr_code		= "900101";
					CTL_Lib.Alarm_Error_Occurrence(1296, CTL_dWARNING, alarm.mstr_code );
				}
			}
		}
		break;

	case 300:
		m_nReq_MES	= WORK_NONE_;	// [리셋] Id 버퍼 고정 작업 요청 플래그
		m_nStep_MES	= 0;
		nFuncRet = CTL_GOOD;
		break;
	}
	return nFuncRet;
}


void CRun_Unload_Works::Run_Reinstate()
{
	if( st_handler.mn_Rcvy_state[RCVY_ULDROBOT] == CTL_YES) return;
	int nRet[3];
	switch(mn_RcvyStep)
	{
	case 0:
		{
			nRet[0] = COMI.Check_MotPosRange(M_ULD_Y, COMI.md_cmdpos_backup[M_ULD_Y], COMI.md_allow_value[M_ULD_Y]);
			nRet[1] = COMI.Check_MotPosRange(M_ULD_Z, COMI.md_cmdpos_backup[M_ULD_Z], COMI.md_allow_value[M_ULD_Z]);
			nRet[2] = COMI.Check_MotPosRange(M_BTM_VISION_X, COMI.md_cmdpos_backup[M_BTM_VISION_X], COMI.md_allow_value[M_BTM_VISION_X]);

			bool nRecy = false;
			for( int i = 0; i < 3; i++)
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
				m_dPos_TmpRcvyZ = COMI.md_cmdpos_backup[M_ULD_Z];
				mn_RcvyStep = 100;
			}
		}		
		break;

	case 100:
		{
			for ( int i = 0; i < 2; i++)
			{
				ReqVacDown( i, BACKWARD);
			}
			mn_RcvyStep = 110;
		}
		break;

	case 110:
		if( m_fbVacDown[0] != FB_NONE || m_fbVacDown[1] != FB_NONE) break;
		mn_RcvyStep = 200;
		break;

	case 200:
		{
			int nRet1 = CTL_Lib.Single_Move(M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_Z_SAFETY], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
			{
				COMI.md_cmdpos_backup[M_ULD_Z] = m_dPos_TmpRcvyZ;
				mn_RcvyStep = 1000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
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
			int nRet1 = CTL_Lib.Single_Move(M_ULD_Y, COMI.md_cmdpos_backup[M_ULD_Y], (int)st_basic.nRunSpeed );
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
		{
			if (m_nReq_Rcvy[2] == CTL_NO)
			{
				mn_RcvyStep = 3000;
				break;
			}
			int nRet1 = CTL_Lib.Single_Move(M_BTM_VISION_X, COMI.md_cmdpos_backup[M_BTM_VISION_X], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD )
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
			EFB efb;
			for ( int i = 0; i < 2; i++)
			{
				if( st_work.mn_uld_picker_updn[i] == IO_ON) efb = FORWARD;
				else										efb = BACKWARD;
				ReqVacDown( i, efb );
			}
			mn_RcvyStep = 3100;
		}
		break;

	case 3100:
		{
			for ( int i = 0; i < 2; i++ )
			{
				if( g_Run_Unload_Works.GetVacDown( i ) != FB_NONE )		break;
			}
			mn_RcvyStep = 9000;
		}
		break;



	case 9000:
		st_handler.mn_Rcvy_state[RCVY_ULDROBOT] = CTL_YES;
		mn_RcvyStep = 0;
		break;

	}
}

void CRun_Unload_Works::Run_Restore()
{
}

void CRun_Unload_Works::OnRobot_FinalPos()
{
}

void CRun_Unload_Works::Serialize(CArchive &ar)
{
}

