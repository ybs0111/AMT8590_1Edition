// Run_Smema_Ctl.cpp: implementation of the CRun_Smema_Ctl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Smema_Ctl.h"

#include "IO_Manager.h"
#include "FrontServer.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_Smema_Ctl  Run_Smema_Ctl;
CRun_Smema_Ctl::CRun_Smema_Ctl()
{
	mn_top_run_step = 0;
}

CRun_Smema_Ctl::~CRun_Smema_Ctl()
{

}



void CRun_Smema_Ctl::smema_process_top_8580(int n_mode) //server
{
	int nRet_1=0, nRet_2=0, nRet_3=0, nRet_4=0, nRet_5=0, nRet_6=0;
	CString str_msg;
	int nSubStep =0;
	int nFunRet=CTL_PROCEED;
	bool bResult = false;
	int nBarcodeCount =0;

	
	
	 
 	Func.ThreadFunctionStepTrace(90, mn_top_run_step);

// 	if( g_lotMgr.GetLotCount() > 0)
// 	{
// 		if(st_var.n_tboxmv_sealing_cv_mode != st_step.n_sub[nSubStep])
// 		{
// 			st_var.n_tboxmv_sealing_cv_mode = st_step.n_sub[nSubStep];
// 			st_var.n_tboxmv_forming_cv_mode = 1;
// // 			if( st_var.n_next_down == CTL_YES)
// // 				st_var.n_next_down = CTL_NO;
// 		}
// 		else if( st_var.n_tboxmv_sealing_cv_mode == st_step.n_sub[nSubStep])
// 		{
// 			if(st_var.n_tboxmv_forming_cv_mode == 1)
// 			{
// 				st_testreference.n_off_time_w = GetCurrentTime();
// 				st_var.n_tboxmv_forming_cv_mode = 0;
// 			}
// 			else
// 			{
// 				if( GetCurrentTime() - st_testreference.n_off_time_w <= 0 ) st_testreference.n_off_time_w = GetCurrentTime();
// 				if( GetCurrentTime() - st_testreference.n_off_time_w > (MES_TIME_OUT*3) )
// 				{
// // 					if( st_var.n_next_down == CTL_NO)
// // 						st_var.n_next_down = CTL_YES;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if( g_lotMgr.GetLotCount() > 1 && st_var.n_tboxmv_sealing_cv_mode == st_step.n_sub[nSubStep])
// 			if(st_var.n_tboxmv_forming_cv_mode == 0)
// 				st_testreference.n_off_time_w = GetCurrentTime();
// 	}
	
	switch(mn_top_run_step)
	{
	case 0: //버퍼의 자재 상태를 체크한다 	
		mn_retry_cnt = 0;
		mn_top_run_step = 1000;
		break;

	case 1000:
		if(st_sync.n_smema_input_index_req == CTL_REQ)//smema 통신 시작 , 8580 <- 8590장비에서 버퍼를 요청한다 
		{
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, IO_OFF); //8580 -> 8590 //버퍼가 준비되었다고 8590에서 알려 줌 
			if( nRet_1 == IO_OFF || st_handler.n_without_smema_8580_mc == CTL_YES) 
			{
				if(st_handler.n_without_smema_8580_mc == CTL_NO) g_ioMgr.set_out_bit(st_io.o_front_top_machine_enter_signal, IO_ON); //8580 <- 8590 에서 버퍼를 잡고,  8580에서 버퍼 놓기를 기다린다 
				mn_smema_io_onoff_flag[0] = 0; 
						
				sprintf(st_msg.c_normal_msg, " [SMEMA-작업시작][SMEMA_0 AMT8580 <- AMT8590] st_io.o_front_top_machine_enter_signal IO_ON"); //버퍼 사이트에 버퍼를 가져다 놓았다.
				Func.On_LogFile_Add(99, st_msg.c_normal_msg);	
	
				if(st_handler.n_without_smema_8580_mc == CTL_NO)
				{
					g_front_server.SetBufferIn(NR_WAITING);

				}
				mn_top_run_step = 2000;
			}
		}
		break;	

	case 2000:		 
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, IO_ON); //8570 -> 8580 //버퍼가 준비되었다고 8570에서 알려 줌 
		if(nRet_1 == IO_ON || st_handler.n_without_smema_8580_mc == CTL_YES)
		{//자재를 버퍼에 놓았다는 플레그 
			if(mn_smema_io_onoff_flag[0] == 0)
			{
				ml_smema_io_onoff_wait[0][0] = GetCurrentTime();
				mn_smema_io_onoff_flag[0] = 1;
			}
			else if(mn_smema_io_onoff_flag[0] == 1)
			{
				ml_smema_io_onoff_wait[0][1] = GetCurrentTime();
				ml_smema_io_onoff_wait[0][2] = ml_smema_io_onoff_wait[0][1] - ml_smema_io_onoff_wait[0][0];
				if(ml_smema_io_onoff_wait[0][2] > MAX_IO_STATUS_WAIT) //i/o 통신의 안정시간 추가 
				{			
 						sprintf(st_msg.c_normal_msg, "[SMEMA_0 AMT8580 -> AMT8590] st_io.i_front_top_machine_ready_signal_chk IO_ON"); //버퍼 사이트에 버퍼를 가져다 놓았다.
						Func.On_LogFile_Add(99, st_msg.c_normal_msg); 
						
						mn_smema_io_onoff_flag[0] = 0;  
						mn_smema_netwotk_retry[0] = 0;
						mn_top_run_step = 2100; 
				}
				else if(ml_smema_io_onoff_wait[0][2] < 0)
				{
					ml_smema_io_onoff_wait[0][0] = GetCurrentTime();
				}
			}			
		}
		else
		{
			mn_smema_io_onoff_flag[0] = 0;
		}
	 
		break;

	case 2100:
		if(st_handler.n_without_network == CTL_NO)
		{					
			dw_smema_network_retry_waittime[0][0] = GetCurrentTime();
			mn_top_run_step = 2200;
		}	
		else
		{
			mn_top_run_step = 3000;
		}
		break;

	case 2200: 
		bResult = false;
		bResult = g_front_server.GetBufferInReceived(); //== true
		if( /*1||*/ bResult == true)
		{
			mn_top_run_step = 3000;
		}
		else //if(bResult == false)
		{
			dw_smema_network_retry_waittime[0][1] = GetCurrentTime();
			dw_smema_network_retry_waittime[0][2] = dw_smema_network_retry_waittime[0][1] - dw_smema_network_retry_waittime[0][0];
			if(dw_smema_network_retry_waittime[0][2] > (st_basic.mn_network_wait_time* 1000) )
			{
				mn_smema_netwotk_retry[0]++;
				if(mn_smema_netwotk_retry[0] > st_basic.mn_network_retry_cnt)
				{
					mn_top_run_step = 2300;
					
				}
				else
				{
					mn_top_run_step = 2100;
				}
			}
		}
		break;
		
	case 2300:
		//800000 0 A " Network Time Out Erorr[TOP LOADER_MC]."
		sprintf(mc_jamcode, "800000"); 
		COMI.mn_run_status = dWARNING;  
		CTL_Lib.Alarm_Error_Occurrence(9206, COMI.mn_run_status, mc_jamcode);

		
		mn_top_run_step = 2400;
		break;

	case 2400: //장비가 가동되면 retry 시작 
		if(COMI.mn_run_status == dRUN)
		{
			bResult = false;
			bResult = g_front_server.GetBufferInReceived(); //== true
			if( /*1||*/ bResult == true)
			{
				mn_top_run_step = 3000;
				break;
			}

			mn_smema_netwotk_retry[0] = 0;			 
			g_front_server.SetBufferIn(NR_WAITING); 
			mn_top_run_step = 2100;
		}
		break;
		///////////////////2014.1107 


	case 3000: 
		if( st_handler.mn_curr_back_conv == 1)
		{
			for ( int i = 0; i < 2; i++)
			{
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_last_module[i] = 0;
				
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_skip_flag[i] = 0;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuffer[i] = 1;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[i] = 11;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_exist[i] = 1;
				
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[i], "%s", "X005600447");
// 					sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_part_num[i], "%s", strPartID);
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[i], "%s", /*"S21GNWAFB00002B");//*/"S21GNWAFB00052T");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_var[i], "%s", "1");
				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_product_id[i], "%s", "2.5");

			}
			bool bLot = false;
			for ( i = 0; i < g_lotMgr.GetLotCount(); i++)
			{
				if(g_lotMgr.GetLotAt(i).GetLotID() == "X005600447")
				{
					bLot = true;
					break;
				}
			}
			if(bLot == false)
			{
				if( st_basic.mn_mode_run == CTL_NO)
					g_lotMgr.OnNewLotIn("X005600447", "PARTNUM_JKFLD324");
				else
				{
					st_var.n_newlotin = CTL_YES;
				}
			}


		}
		st_sync.n_smema_input_index_req = CTL_READY;//8580 장비가 버퍼를 놓은 상태, 8590 장비가 집어가면 된다 	//8590 장비는 버퍼를 집을 수 있다 

		mn_top_run_step = 4000;
		break;

	case 4000:	 
		if( st_sync.n_smema_input_index_req == CTL_FREE) //8580 <- 8590  8580 장비사 버퍼를 잡았다. 8590이 잡고 있는 버퍼를 놓기를 기다린다 
		{
			if(st_handler.n_without_smema_8580_mc == CTL_NO) g_ioMgr.set_out_bit(st_io.o_front_top_machine_transfer_signal, IO_ON); //8570 <- 8580 에서 버퍼를 잡고,  8570에서 버퍼 놓기를 기다린다 
			mn_smema_io_onoff_flag[0] = 0; 
		 
			g_ioMgr.set_out_bit(st_io.o_front_top_machine_enter_signal, IO_OFF); //clear

			sprintf(st_msg.c_normal_msg, "[SMEMA_0 AMT8580 <- AMT8590] st_io.o_front_top_machine_transfer_signal IO_ON"); //버퍼 사이트에 버퍼를 가져다 놓았다.
			Func.On_LogFile_Add(99, st_msg.c_normal_msg); 
			mn_top_run_step = 5000;
		}
		break;

	case 5000:
	 
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_front_top_machine_ready_signal_chk, IO_OFF); //8570 -> 8580, 8570에서 잡고있던 그리퍼를 놓았다는 신호 
		if(nRet_1 == IO_OFF || st_handler.n_without_smema_8580_mc == CTL_YES) //로더 장비를 사용안할때 
		{//8570이 잡고있던 버퍼를 놓았다는 신호  
			if(mn_smema_io_onoff_flag[0] == 0)
			{
				ml_smema_io_onoff_wait[0][0] = GetCurrentTime();
				mn_smema_io_onoff_flag[0] = 1;
			}
			else if(mn_smema_io_onoff_flag[0] == 1)
			{
				ml_smema_io_onoff_wait[0][1] = GetCurrentTime();
				ml_smema_io_onoff_wait[0][2] = ml_smema_io_onoff_wait[0][1] - ml_smema_io_onoff_wait[0][0];
				if(ml_smema_io_onoff_wait[0][2] > MAX_IO_STATUS_WAIT) //i/o 통신의 안정시간 추가 
				{
					mn_smema_io_onoff_flag[0] = 0;
					st_sync.n_smema_input_index_req = CTL_CHANGE; 
					
					sprintf(st_msg.c_normal_msg, "[SMEMA_0 AMT8580 -> AMT8590] st_io.i_front_top_machine_ready_signal_chk IO_OFF"); //버퍼 사이트에 버퍼를 가져다 놓았다.
					Func.On_LogFile_Add(99, st_msg.c_normal_msg);  

					mn_top_run_step = 6000;
				}
				else if(ml_smema_io_onoff_wait[0][2] < 0)
				{
					ml_smema_io_onoff_wait[0][0] = GetCurrentTime();
				}
			}
		}
		else
		{
			mn_smema_io_onoff_flag[0] = 0;
		}		 
		break;

	case 6000:
		if(st_sync.n_smema_input_index_req == CTL_SORT) //8580에서 잡고 있는 버퍼를 8580 장비에 갔다 놓았을때 보냄 
		{
			//8580이 버퍼를 완전히 장비에 끌고 들어간 상태 
			g_ioMgr.set_out_bit(st_io.o_front_top_machine_transfer_signal, IO_OFF); //스메마로 라우터 장비에 보트를 받을 수 있다는 시그널을 날린다  
			mn_smema_io_onoff_flag[0] = 0; 
			st_sync.n_smema_input_index_req = CTL_CLEAR;
			sprintf(st_msg.c_normal_msg, " [SMEMA-작업완료][SMEMA_0 AMT8580 <- AMT8590] id buffer o_front_top_machine_transfer_signal IO_OFF"); //버퍼 사이트에 버퍼를 가져다 놓았다.
			Func.On_LogFile_Add(99, st_msg.c_normal_msg);  
			mn_top_run_step = 7000;			 
		} 
		break;

	case 7000:
		mn_top_run_step = 1000;
		break;

	default :
		break;
	}

}


/////////////////////////////////////////////////////////////////////////////////////
// top 사이트의 모든 정보를 관리한다, struct 로 전체 데이터를 관리한다 
/////////////////////////////////////////////////////////////////////////////////////
int CRun_Smema_Ctl::top_idbuffer_datainfo_shift(int n_mode, int n_shift_site)
{

	int nFunRet=CTL_GOOD;
	int nFlag = 0; 
	int Len[2];
	int AccLen[2];
	CString Temp[2];
	CString AccCode[2];

/*
enum idbuffer_move_info_shift //id buffer가 이동하면서 각각의 정보를 유지 및 생성하면서 쉬프트한다 
{	
  SHIFT_IDBUFF_INPUT_INDEX = 0,  

  SHIFT_IDBUFF_INDEX_DENSITY,
  SHIFT_IDBUFF_INDEX_BCR1,	
  SHIFT_IDBUFF_INDEX_BCR2,
  SHIFT_IDBUFF_INDEX_TOP,
  SHIFT_IDBUFF_INDEX_ULD,
  

	
	SHIFT_IDBUFF_INDEX_ULD_SEND,
	SHIFT_IDBUFF_INDEX_ULD_RECV,
	  
		SHIFT_IDBUFF_SITE_ALL_CHK //
};
*/
	 
	if(n_shift_site == SHIFT_IDBUFF_INDEX_ULD_RECV)
	{
		if(n_mode == 0) // 정보를 쉬프트 받는 모드 
		{
 			if(st_handler.n_without_network == CTL_YES) //loader 장비는 사용하지 않고 단독을 가동체크시, smema 통신은 없으니 강제로 만든다  
			{//st_idbuff_info[SHIFT_IDBUFF_LOADER_MC_NETWORKDATA_RECEIVE]	
				//정상적으로 가동시 이 내용은 network 통해 loader 장비에서 받음 

				st_idbuff_info[n_shift_site].n_skip_flag[CTL_LEFT] = CTL_NO;
				st_idbuff_info[n_shift_site].n_skip_flag[CTL_RIGHT] = CTL_NO;
				st_idbuff_info[n_shift_site].n_idbuffer[CTL_LEFT] = 1;
				st_idbuff_info[n_shift_site].n_idbuffer[CTL_RIGHT] = 1;
				st_idbuff_info[n_shift_site].n_idbuff_num[CTL_LEFT] = 26; //가상정보 
				st_idbuff_info[n_shift_site].n_idbuff_num[CTL_RIGHT] = 18;				
				st_idbuff_info[n_shift_site].n_exist[CTL_LEFT] = 1;
				st_idbuff_info[n_shift_site].n_exist[CTL_RIGHT] = 1;

				st_idbuff_info[n_shift_site].n_count[CTL_LEFT] = 300;
				st_idbuff_info[n_shift_site].n_count[CTL_RIGHT] = 301;

				st_idbuff_info[n_shift_site].n_mode = CTL_DUAL;
				st_idbuff_info[n_shift_site].n_totalcnt = 1000;
//				st_idbuff_info[n_shift_site].n_bin[CTL_LEFT] = 1;
//				st_idbuff_info[n_shift_site].n_bin[CTL_RIGHT] = 1;

//				sprintf(st_idbuff_info[n_shift_site].c_product_id[CTL_LEFT], "%s", "01");
//				sprintf(st_idbuff_info[n_shift_site].c_product_id[CTL_RIGHT], "%s", "01");
//				
//				sprintf(st_idbuff_info[n_shift_site].c_lot_id[CTL_LEFT], "%s", "ssd_lotid_12345_7");
//				sprintf(st_idbuff_info[n_shift_site].c_lot_id[CTL_RIGHT], "%s", "ssd_lotid_12345_7");
//
//				sprintf(st_idbuff_info[n_shift_site].c_part_num[CTL_LEFT], "%s", "ssd_partnum_12345_7");
//				sprintf(st_idbuff_info[n_shift_site].c_part_num[CTL_RIGHT], "%s", "ssd_partnum_12345_7");
//
				sprintf(st_idbuff_info[n_shift_site].c_serial[CTL_LEFT], "%s", "Serial88");
				sprintf(st_idbuff_info[n_shift_site].c_serial[CTL_RIGHT], "%s", "Serial77");
//				
//				sprintf(st_idbuff_info[n_shift_site].c_var[CTL_LEFT], "%s", "02");
//				sprintf(st_idbuff_info[n_shift_site].c_var[CTL_RIGHT], "%s", "02");
//
//				EIDBUFFER_POS eTargetPos = IDPOS_NONE;
//
//				if( g_Tray.IsEnablePos( IDPOS_FRONT_TO_DENSITY ) )
//				{
//					eTargetPos = IDPOS_FRONT_TO_DENSITY;
//				}
//
//				if( eTargetPos != TPOS_NONE )
//				{
//					ATray tray;
//					tray.SetPos( IDPOS_FRONT_TO_DENSITY );
//					tray.SetTrayInfo( g_lotMgr.GetLotIDAt(0), 0, false, false );
//					
//					for( int i=0; i<MAX_MODULE; i++ )
//					{
//						CString strProductID;	strProductID.Format("%s", st_idbuff_info[n_shift_site].c_product_id[i]);
//						
//						CString strLotID;		strLotID.Format("%s", st_idbuff_info[n_shift_site].c_product_id[i]);
//						CString strSkip;		strSkip.Format("%s",st_idbuff_info[n_shift_site].n_skip_flag[i]);
//						CString strArraySN;		strArraySN.Format("%02d", st_idbuff_info[n_shift_site].n_idbuff_num[i] );
//						CString strSerial;		strSerial.Format( "%s",st_idbuff_info[n_shift_site].c_serial[i]);
//						CString strVar;			strVar.Format( "%s",st_idbuff_info[n_shift_site].c_var[i]);
//						
//						// 모듈 셋
//						AModule mdl;
// 						mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
//						tray.SetModule(i, mdl);
//					}
//					
//					tray.SetTargetPos( eTargetPos );
//					g_Tray.AddTray( tray );
//				}

			}
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX], &st_idbuff_info[n_shift_site], sizeof(st_idbuffer_info_param)); //data copy

			//2015.0113
			for(int i =0; i<2; i++)
			{
				Temp[i].Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[i]);
				Temp[i].TrimRight();
				Temp[i].TrimRight();
				Len[i] = Temp[i].GetLength();
				AccCode[i].Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_acce_code[i]);
				AccCode[i].TrimRight();
				AccCode[i].TrimRight();
				AccLen[i] = AccCode[i].GetLength();
				
			}
			if ( st_basic.mn_mode_run == CTL_YES && Temp[0].Find("S",0) == 0 && Len[0] >10 && Temp[1].Find("S",0) == 0 && Len[1] >10 )
			{
				memcpy(&st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX], &st_idbuff_info[n_shift_site], sizeof(st_idbuffer_info_param)); //data copy
				
			}
			else
			{
				if ( st_idbuff_info[n_shift_site].n_skip_flag[0] != 1 && st_basic.mn_mode_run == CTL_YES && (Temp[0].Find("S",0) != 0 || Len[0] <10) )
				{
					AfxMessageBox("[datainfo_shift_error[Left]] -SHIFT_IDBUFF_INDEX_ULD_RECV");
					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
					{
						
						sprintf(st_msg.c_abnormal_msg, "[ERROR] SHIFT_IDBUFF_INDEX_ULD_RECV LEFT DATA");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						
					}
				}
				
				if ( st_idbuff_info[n_shift_site].n_skip_flag[1] != 1 && st_basic.mn_mode_run == CTL_YES && (Temp[1].Find("S",0) != 0 || Len[1] <10 ) )
				{
					
					AfxMessageBox("[datainfo_shift_error[Right]] -SHIFT_IDBUFF_INDEX_ULD_RECV");
					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
					{
						sprintf(st_msg.c_abnormal_msg, "[ERROR] SHIFT_IDBUFF_INDEX_ULD_RECV RIGHT DATA");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);		
					}
				}
				
			}

			//2014.1105
			if(st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].n_idbuffer[CTL_RIGHT] == CTL_YES)				 
			{
				st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].n_mode = CTL_DUAL;

				//2014.1108 memset(&st_idbuff_info[n_shift_site], 0x00, sizeof(st_idbuff_info[n_shift_site])); //clear 
				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_normal_msg, "[DATA SHIFT] SHIFT_IDBUFF_LOADER_MC_NETWORKDATA_RECEIVE DATA");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else
			{
				AfxMessageBox("[datainfo_shift_error] -SHIFT_IDBUFF_INDEX_ULD_RECV");

				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_normal_msg, "[ERROR] SHIFT_IDBUFF_INDEX_ULD_RECV DATA");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				//2014.1105 디버깅을 위해 초기화 생략 memset(&st_idbuff_info[SHIFT_IDBUFF_LOADER_MC_NETWORKDATA_RECEIVE], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_LOADER_MC_NETWORKDATA_RECEIVE])); //clear 

			
			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_ULD_RECV DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INPUT_INDEX, 0);
		}
	}
	else if(n_shift_site == SHIFT_IDBUFF_INPUT_INDEX) //input index -> set plate에 정보 전송 //SHIFT_IDBUFF_LOADER_MC_NETWORKDATA_RECEIVE)//loader MC에서 network을 통해 받은 정보를 백업시 사용 
	{
		if(n_mode == 0) //data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], &st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX], sizeof(st_idbuffer_info_param)); //data copy
//2014.1202			memset(&st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX])); //clear 

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INPUT_INDEX");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
//			g_Tray.TrayMoved( EIDBUFFER_POS (IDPOS_FRONT_TO_DENSITY), EIDBUFFER_POS(IDPOS_DENSITY) );

		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX])); //clear  
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INPUT_INDEX DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}

		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_DENSITY, 0);
		}
	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_DENSITY) //output buffer -> label MC에 전송 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], sizeof(st_idbuffer_info_param)); //data copy
			//2014.1108 memset(&st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER])); //clear 

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_DENSITY");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_DENSITY DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_BCR1, 0);
		}

	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_BCR1) //output buffer -> label MC에 전송 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], sizeof(st_idbuffer_info_param)); //data copy
			//2014.1108 memset(&st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER])); //clear 
			
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_BCR1");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_BCR1 DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_BCR2, 0);
		}
	
	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_BCR2) //output buffer -> label MC에 전송 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], sizeof(st_idbuffer_info_param)); //data copy
			//2014.1108 memset(&st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER])); //clear 
			
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_BCR2");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_BCR2 DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_TOP, 0);
		}
	
	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_TOP) //output buffer -> label MC에 전송 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], sizeof(st_idbuffer_info_param)); //data copy
			//2014.1108 memset(&st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER])); //clear 
			
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_TOP");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_TOP DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD, 0);
		}

	}

	else if(n_shift_site == SHIFT_IDBUFF_SITE_ALL_CHK) //SHIFT_IDBUFF_INDEX_DENSITY ~ SHIFT_IDBUFF_INDEX_ULD 까지의 정보를 한나씩 뒤로 전송 ; 최종 side fold ~ out buffer 까지 정보가 남음 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			st_handler.mn_Top_Vision[2] = st_handler.mn_Top_Vision[0];
			st_handler.mn_Top_Vision[3] = st_handler.mn_Top_Vision[1];
			st_handler.mn_Top_Vision[0] = CTL_NO;
			st_handler.mn_Top_Vision[1] = CTL_NO;
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], sizeof(st_idbuffer_info_param)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP]));	//data clear 

			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], sizeof(st_idbuffer_info_param)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2]));	//data clear 

			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], &st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], sizeof(st_idbuffer_info_param)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1]));	//data clear 

			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], &st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], sizeof(st_idbuffer_info_param)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY]));	//data clear 		

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data & clear] SHIFT_IDBUFF_SITE_ALL_CHK & CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}

//			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_TOP_VISION), EIDBUFFER_POS(IDPOS_UNLOAD) );
//			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_BARCODE2), EIDBUFFER_POS(IDPOS_TOP_VISION) );
//			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_BARCODE1), EIDBUFFER_POS(IDPOS_BARCODE2) );
//			g_Tray.TrayMoved( EIDBUFFER_POS(IDPOS_DENSITY), EIDBUFFER_POS(IDPOS_BARCODE1) );

		}
		else //if(n_mode == 1) //2014.1108 사용안함, 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP]));	//data clear 
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2]));	//data clear
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1]));	//data clear
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY]));	//data clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_SITE_ALL_CHK DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INPUT_INDEX, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_DENSITY, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_BCR1, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_BCR2, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_TOP, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD, 0);
		}

	}

	return nFunRet;

}


int CRun_Smema_Ctl::btm_idbuffer_datainfo_shift(int n_mode, int n_shift_site)
{

	int nFunRet=CTL_GOOD;
	int nFlag = 0; 	 

	if(n_shift_site == SHIFT_IDBUFF_INDEX_ULD_BTM) //input index -> set plate에 정보 전송 //SHIFT_IDBUFF_LOADER_MC_NETWORKDATA_RECEIVE)//loader MC에서 network을 통해 받은 정보를 백업시 사용 
	{
		if(n_mode == 0) //data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD], sizeof(st_idbuffer_info_param)); //data copy

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_ULD_BTM");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}

		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD])); //clear  
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_ULD DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}	
	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_ULD_CLAMP)
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM], sizeof(st_idbuffer_info_param)); 

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_ULD_CLAMP");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_ULD_BTM DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_ULD_LOAD) //output buffer -> label MC에 전송 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP], sizeof(st_idbuffer_info_param)); //data copy
			//2014.1108 memset(&st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER])); //clear 

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_ULD_LOAD");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_ULD_CLAMP DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//			}
		}

	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_ULD_UNLOAD) //output buffer -> label MC에 전송 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], sizeof(st_idbuffer_info_param)); //data copy
			//2014.1108 memset(&st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_OUTPUTREADY_BUFFER])); //clear 
			
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data] SHIFT_IDBUFF_INDEX_ULD_UNLOAD");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD])); //clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_ULD_LOAD DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}
	}
	else if(n_shift_site == SHIFT_IDBUFF_INDEX_ULD_NEXT) //output buffer -> label MC에 전송 
	{
		memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD])); //clear 
// 		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 		{
// 			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_INDEX_ULD_UNLOAD DATA CLEAR");
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 		}
	}


	else if(n_shift_site == SHIFT_IDBUFF_UNLOAD_SITE_ALL_CHK) //SHIFT_IDBUFF_INDEX_DENSITY ~ SHIFT_IDBUFF_INDEX_ULD 까지의 정보를 한나씩 뒤로 전송 ; 최종 side fold ~ out buffer 까지 정보가 남음 
	{
		if(n_mode == 0) //2014.1108 data shift 
		{
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP], sizeof(idbuffer_unload_move_info_shift)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_CLAMP]));	//data clear 
			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_UNLOAD], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], sizeof(idbuffer_unload_move_info_shift)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD]));	//data clear 

			memcpy(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_BTM], &st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], sizeof(st_idbuffer_info_param)); //data copy
			memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_LOAD]));	//data clear 

// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[shift data & clear] SHIFT_IDBUFF_SITE_ALL_CHK & CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}

		}
		else //if(n_mode == 1) //2014.1108 사용안함, 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP]));	//data clear 
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2]));	//data clear
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1]));	//data clear
			//memset(&st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY], 0x00, sizeof(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY]));	//data clear 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_SITE_ALL_CHK DATA CLEAR");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
		}

		//2017.0510
		if (st_handler.cwnd_main != NULL)
		{
			////2014,1217
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD_BTM, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD_CLAMP, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD_LOAD, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD_UNLOAD, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, SHIFT_IDBUFF_INDEX_ULD_NEXT, 0);
			////
		}
 	}

	return nFunRet;

}


/////////////////////////////////////////////////////////////////////////////////////
// 동작 flag와 그와 연관된 I/O의 상태를 체크하는 함수 
/////////////////////////////////////////////////////////////////////////////////////
int CRun_Smema_Ctl::top_idbuffer_sensor_flag_status_check(int n_mode, int n_check_site) //센서와 플레그가 틀리면 에러 
{
	int i=0;
	int nFunRet = CTL_GOOD;
	int nRet_1=0, nRet_2=0;

	 if(st_basic.n_mode_device == 0 && st_handler.mn_virtual_idbuffer == 0)//
	 {
		return CTL_GOOD;
	 }

	for(i=0; i<20; i++)
	{
		if(i == 0)		st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT];
		else if(i == 1) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT];
		else if(i == 2) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT];
		else if(i == 3) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT];
		else if(i == 4) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT];
		else if(i == 5) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT];
		else if(i == 6) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT];
		else if(i == 7) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT];
		else if(i == 8) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT];
		else if(i == 9) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT];
		
		else if(i == 10) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT];
		else if(i == 11) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT];
		else if(i == 12) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_LEFT];
		else if(i == 13) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_RIGHT];
		else if(i == 14) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT];
		else if(i == 15) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT];
		else if(i == 16) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_LEFT];
		else if(i == 17) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_RIGHT];
		else if(i == 18) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_LEFT];
		else if(i == 19) st_var.n_top_idbuffer_exist_status_check_info[i] = st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[CTL_RIGHT];



		st_var.n_top_idbuffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF);
		if(n_check_site == SHIFT_IDBUFF_INPUT_INDEX)
		{
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_DENSITY )
		{
			if(i == 0 || i == 1 || i== 10 || i == 11)
			{				
				if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
				{
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						if(i == 0 || i == 1) continue;								
					}

					if(st_basic.n_mode_device == 0/* || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_LEFT] == CTL_YES
						|| st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_RIGHT] == CTL_YES*/)
					{
						if(i== 10 || i == 11) continue;
					}

					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 0 || i == 1) continue;				
					}

					sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 	 
					CTL_Lib.Alarm_Error_Occurrence( 990, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}			
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_BCR1 )
		{
			if(i == 2 || i == 3 || i== 12 || i == 13)
			{					
				if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
				{
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						if(i == 2 || i == 3) continue;								
					}

// 					if(st_basic.n_mode_device == 0 || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_LEFT] == CTL_YES
// 						|| st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_RIGHT] == CTL_YES)
					
					if(st_basic.n_mode_device == 0)
					{
						if(i== 12 || i == 13) continue;
					}

					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 2 || i == 3) continue;				
					}

					sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 	 
					CTL_Lib.Alarm_Error_Occurrence( 991, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}			 
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_BCR2)
		{
			if(i == 4 || i == 5 || i== 14 || i == 15)
			{
				if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
				{
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						if(i == 4 || i == 5) continue;								
					}
					
// 					if(st_basic.n_mode_device == 0 || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_LEFT] == CTL_YES
// 						|| st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_RIGHT] == CTL_YES)
					if(st_basic.n_mode_device == 0 )
					{
						if(i== 14 || i == 15) continue;
					}
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 4 || i == 5) continue;				
					}

					sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
					CTL_Lib.Alarm_Error_Occurrence( 992, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				} 
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_TOP)
		{
			if(i == 6 || i == 7 || i== 16 || i == 17)
			{				
				if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
				{
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						if(i == 6 || i == 7) continue;								
					}
					
// 					if(st_basic.n_mode_device == 0 || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_LEFT] == CTL_YES
// 						|| st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_RIGHT] == CTL_YES)
					if(st_basic.n_mode_device == 0)
					{
						if(i== 16 || i == 17) continue;
					}

					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 6 || i == 7) continue;				
					}

					sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
					CTL_Lib.Alarm_Error_Occurrence( 993, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_INDEX_ULD)
		{
			if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
			{
				if(i == 8 || i == 9 || i== 18 || i == 19)
				{
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						if(i == 8 || i == 9) continue;								
					}
					
// 					if(st_basic.n_mode_device == 0 || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[CTL_LEFT] == CTL_YES
// 						|| st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_skip_flag[CTL_RIGHT] == CTL_YES)
					if(st_basic.n_mode_device == 0)
					{
						if(i== 18 || i == 19) continue;
					}
				}
				sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
				CTL_Lib.Alarm_Error_Occurrence( 994, CTL_dWARNING, mc_jamcode );
				nFunRet = CTL_ERROR;
				break;
			}
		}
		else if(n_check_site == SHIFT_IDBUFF_SITE_ALL_CHK)
		{
			if(i == 2 || i == 3 || i== 4 || i == 5 || i == 6 || i == 7 
			   || i == 12 || i == 13 || i== 14 || i == 15 || i == 16 || i == 17 )
			{
				if(st_var.n_top_idbuffer_exist_status_check_info[i] != st_var.n_top_idbuffer_io_status_read_chk_data[i])
				{
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						if(i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7) continue;								
					}

					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 0 || i == 1) continue;				
					}
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 2 || i == 3) continue;				
					}
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 4 || i == 5) continue;				
					}
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_clamp_chk, IO_OFF);  
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_unclamp_chk, IO_ON); 
					if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
					{
						if(i == 6 || i == 7) continue;				
					}

// 					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_RIGHT] == CTL_YES)
// 					{
// 						if(i == 10 || i == 11) continue;
// 					}
// 
// 					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_RIGHT] == CTL_YES)
// 					{
// 						if(i == 12 || i == 13) continue;
// 					}
// 					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_RIGHT] == CTL_YES)
// 					{
// 						if(i == 14 || i == 15) continue;
// 					}
// 
// 					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_RIGHT] == CTL_YES)
// 					{
// 						if(i == 16 || i == 17) continue;
// 					}

					if(st_basic.n_mode_device == 0)
					{
						if(i == 12 || i == 13 || i == 14 || i == 15 || i == 16 || i == 17) continue;
					}

					sprintf(mc_jamcode, "5%04d%d", st_io.i_top_idbuffer_exist_sensor_check[i], IO_OFF); 
					CTL_Lib.Alarm_Error_Occurrence( 995, CTL_dWARNING, mc_jamcode );
					nFunRet = CTL_ERROR;
					break;
				}
			}
		}
	}
	return nFunRet;

}