// Run_IDBuff_Move_Index.cpp: implementation of the CRun_IDBuff_Move_Index class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_IDBuff_Move_Index.h"

#include "Run_Smema_Ctl.h"
#include "Run_IDBuff_Input_Index.h"
#include "RunNetwork.h"
#include "Public_Function.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_IDBuff_Move_Index g_Run_IDBuff_Move_Index;

CRun_IDBuff_Move_Index::CRun_IDBuff_Move_Index()
{
	mn_InitStep = 0;
	mn_run_step = 0; 
	mn_density_step = 0;
	mn_pick_left_step = 0;
	mn_place_right_step = 0;
	mn_bcrIn_step = 0;
	mn_bcrOut_step = 0;
}

CRun_IDBuff_Move_Index::~CRun_IDBuff_Move_Index()
{

}

void CRun_IDBuff_Move_Index::Thread_Run()
{
// return;
//	int i=0,nRet_1=0;
//	double dCurPos = 0;
	int n_ret_1=0;

	//st_handler.n_without_idbuffer = CTL_YES;
// 	st_handler.n_without_smema_8580_mc = CTL_NO;
// 	st_handler.n_without_barcode_read = CTL_YES;
// 	st_handler.n_without_network = CTL_NO;
// 	st_handler.n_without_idbuffer = CTL_NO;
// 
// 	st_handler.n_without_density_label_rbt = CTL_YES;
// 	st_handler.n_without_bcr1_rbt = CTL_YES;
// 	st_handler.n_without_bcr2_rbt = CTL_YES;
// 	st_handler.n_without_top_vision_rbt = CTL_YES;
//	Run_Smema_Ctl.top_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_SITE_ALL_CHK); //data shift & clear 이곳은 
		
	if( Func.OnIsInterruptCondition() != 1 )
		return;
	
	switch (COMI.mn_run_status)
	{		
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
// 		return;
		if(st_handler.n_only_work_density == 1 )
		{
			break;
		}
		//2015.0222
		if(st_var.n_bcr_read_output_flag == CTL_YES)
		{
			n_ret_1 = barcode_read_inout_process(CTL_RIGHT);
			if(n_ret_1 == CTL_GOOD)
			{
				st_var.n_bcr_read_output_flag = CTL_NO; //바코듣 읽기 완료 
			}
		}
		Run_Move_Density();
		Run_Move();
		
		break;
		
	default: 
		
		break;
	}

}

void CRun_IDBuff_Move_Index::Run_Initial()
{
	int nRet_1 = 0, nRet_2 = 0,nRet_3 = 0,nRet_4 = 0,nRet_5 = 0,nRet_6 = 0,nRet_7 = 0,nRet_8 = 0;
	int i = 0, nFlag=0, nIodata_1=0, nIodata_2=0;
	
	if (st_handler.mn_init_state[INIT_TOP_IDBUFFER] != CTL_NO)		return;		// No 일때만 초기화 작업을 한다. 초기화가 끝나면 YES->READY가 되기 떄문에...
	
//	mn_InitStep = st_step.n_init[STEP_MOVE_INDEX]; 
//	Func.ThreadFunctionStepTrace(STEP_MOVE_INDEX + STEP_INIT, mn_InitStep);
	
	switch (mn_InitStep)
	{
	case 0:
 		mn_InitStep = 1000;
		break;

	case 1000: //모든 fix clamp를 잡는다 
		mn_fixclamp_sol_flag[0] = 1; mn_fixclamp_sol_flag[1] = 1; mn_fixclamp_sol_flag[2] = 1; mn_fixclamp_sol_flag[3] = 1;//clear 
		//안전을 위해 무조건 backward 하자  		
		set_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_ON);//clamp
		mn_InitStep = 1010; 
		break;
		
	case 1010: //이때 fix clamp에 id buffer를 체크하는 센서가 있어, unclamp 상태에서는 버퍼를 체크할 수 없다 
		nRet_1 = get_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_ON);//clamp
		if(nRet_1 == CTL_GOOD)
		{ 
			mn_InitStep = 2000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9001, COMI.mn_run_status, mc_jamcode);
			mn_InitStep = 1000;
		}
		break;  

	case 2000:
		g_Run_IDBuff_Input_Index.set_idbuffer_input_clamp_onoff(0, 0, IO_OFF); //클램프를 해제하고 이동 준비를 한다 
		mn_InitStep = 2010;
		break;
		
	case 2010:
		nRet_1 = g_Run_IDBuff_Input_Index.get_idbuffer_input_clamp_onoff(0, 0, IO_OFF); //
		if(nRet_1 == CTL_GOOD) //클램프가 오프되어있는지 확인
		{
			mn_InitStep = 2100;
		}
		else if(nRet_1 == CTL_ERROR) //에러
		{
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9002, COMI.mn_run_status, g_Run_IDBuff_Input_Index.mc_jamcode);
			mn_InitStep = 2000;
		}
		break;

	case 2100:
		set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);		 
		mn_InitStep = 2110; 
		break;
		
	case 2110:
		nRet_1 = get_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			mn_InitStep = 3000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9003, COMI.mn_run_status, mc_jamcode);
			mn_InitStep = 2100;
		}
		break; 

	case 3000:
		nRet_1 = COMI.HomeCheck_Mot( M_TOPID_LD1, MOT_TIMEOUT);
		if( nRet_1 == BD_GOOD )
		{
			Func.OnInitBtn( RBTN_TOPID_LD_1 ); 
			mn_InitStep = 3100;
		}
		else if( nRet_1 == BD_ERROR )
		{//330001 0 00 "Top ID Buffr LD1 Axis Motor Home Check Error."
			sprintf(mc_jamcode, "330001");  
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(90040,  COMI.mn_run_status, mc_jamcode);
		}
		break;

	case 3100:
		nRet_1 = COMI.HomeCheck_Mot( M_TOPID_LD2, MOT_TIMEOUT);
		if( nRet_1 == BD_GOOD )
		{
			Func.OnInitBtn( RBTN_TOPID_LD_2 ); 
			mn_InitStep = 4000;
		}
		else if( nRet_1 == BD_ERROR )
		{///340001 0 00 "Top ID Buffr LD2 Axis Motor Home Check Error."
			sprintf(mc_jamcode, "340001");  
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9005,  COMI.mn_run_status, mc_jamcode);
		}
		break;

	case 4000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[0] , IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[1] , IO_ON); 
		nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[2] , IO_ON); 
		nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[3] , IO_ON); 
		nRet_5 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[4] , IO_ON); 
		nRet_6 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[5] , IO_ON); 
		nRet_7 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[6] , IO_ON); 
		nRet_8 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_exist_sensor_check[7] , IO_ON); 

		if(nRet_1 == IO_ON && nRet_2 == IO_ON) //2개다 있을때 
		{
			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_LEFT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_RIGHT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
			st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuff_num[CTL_RIGHT] = 10;				

			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_product_id[CTL_LEFT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_product_id[CTL_RIGHT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_var[CTL_LEFT], "%s", "02");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_var[CTL_RIGHT], "%s", "02");
			
			
// 			ATray tray;
// 			tray.SetPos( IDPOS_DENSITY );
// 			for( int i=0; i<MAX_MODULE; i++ )
// 			{
// 				CString strProductID;	
// 				strProductID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_product_id[i]);
// 				
// 				CString strLotID;		
// 				strLotID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_product_id[i]);
// 				CString strSkip;		
// 				strSkip.Format("%d",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[i]);
// 				CString strArraySN;		
// 				strArraySN.Format("%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuff_num[i] );
// 				CString strSerial="";//		strSerial.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[i]);
// 				CString strVar;			strVar.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_var[i]);
// 				
// 				// ¸ðμa ¼A
// 				AModule mdl;
// 				mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
// 				tray.SetModule(i, mdl);
// 			}	
// 			tray.SetTargetPos( IDPOS_DENSITY );
// 			tray.SetWorkState( EWS_DENSITY );
// 			g_Tray.AddTray( tray );
		}
		else
		{//어느 하나만 있으면 문제가 있는 것이니 에러 발생하자 
			if(nRet_1 == IO_ON)
			{//527040 1 00 "Top ID Buffer1 Check Off Error."
				sprintf(mc_jamcode, "527040");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9006,  COMI.mn_run_status, mc_jamcode);
				break;
			}
			else if(nRet_2 == IO_ON)
			{//527050 1 00 "Top ID Buffer2 Check Off Error."
				sprintf(mc_jamcode, "527050");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9007,  COMI.mn_run_status, mc_jamcode);
				break;
			}
		}

		if(nRet_3 == IO_ON && nRet_4 == IO_ON) //2개다 있을때 
		{
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_LEFT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_RIGHT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuff_num[CTL_RIGHT] = 10;				
			
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_product_id[CTL_LEFT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_product_id[CTL_RIGHT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_var[CTL_LEFT], "%s", "02");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_var[CTL_RIGHT], "%s", "02");
			
// 			ATray tray;
// 			tray.SetPos( IDPOS_BARCODE1 );
// 			for( int i=0; i<MAX_MODULE; i++ )
// 			{
// 				CString strProductID;	strProductID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_product_id[i]);
// 				
// 				CString strLotID;		strLotID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_product_id[i]);
// 				CString strSkip;		strSkip.Format("%d",st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[i]);
// 				CString strArraySN;		strArraySN.Format("%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuff_num[i] );
// 				CString strSerial="";//		strSerial.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[i]);
// 				CString strVar;			strVar.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_var[i]);
// 				
// 				// ¸ðμa ¼A
// 				AModule mdl;
// 				mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
// 				tray.SetModule(i, mdl);
// 			}	
// 			tray.SetTargetPos( IDPOS_BARCODE1 );
// 			tray.SetWorkState( EWS_BARCODE1 );
// 			g_Tray.AddTray( tray );
		}
		else
		{//어느 하나만 있으면 문제가 있는 것이니 에러 발생하자 
			if(nRet_3 == IO_ON)
			{//527100 1 00 "Top ID Buffer3 Check Off Error."
				sprintf(mc_jamcode, "527100");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9008,  COMI.mn_run_status, mc_jamcode);
				break;
			}
			else if(nRet_4 == IO_ON)
			{//527110 1 00 "Top ID Buffer4 Check Off Error."
				sprintf(mc_jamcode, "527110");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(90090,  COMI.mn_run_status, mc_jamcode);
				break;
			}
		}

		if(nRet_5 == IO_ON && nRet_6 == IO_ON) //2개다 있을때 
		{
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_LEFT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_RIGHT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
			st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuff_num[CTL_RIGHT] = 10;				
			
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_product_id[CTL_LEFT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_product_id[CTL_RIGHT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_var[CTL_LEFT], "%s", "02");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_var[CTL_RIGHT], "%s", "02");
			
			
// 			ATray tray;
// 			tray.SetPos( IDPOS_BARCODE2 );
// 			for( int i=0; i<MAX_MODULE; i++ )
// 			{
// 				CString strProductID;	strProductID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_product_id[i]);
// 				
// 				CString strLotID;		strLotID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_product_id[i]);
// 				CString strSkip;		strSkip.Format("%d",st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[i]);
// 				CString strArraySN;		strArraySN.Format("%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuff_num[i] );
// 				CString strSerial="";//		strSerial.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[i]);
// 				CString strVar;			strVar.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_var[i]);
// 				
// 				// ¸ðμa ¼A
// 				AModule mdl;
// 				mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
// 				tray.SetModule(i, mdl);
// 			}	
// 			tray.SetTargetPos( IDPOS_BARCODE2 );
// 			tray.SetWorkState( EWS_BARCODE2 );
// 			g_Tray.AddTray( tray );
		
		}
		else
		{//어느 하나만 있으면 문제가 있는 것이니 에러 발생하자 
			if(nRet_5 == IO_ON)
			{//530020 1 00 "Top ID Buffer5 Check Off Error."
				sprintf(mc_jamcode, "530020");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9010,  COMI.mn_run_status, mc_jamcode);
				break;
			}
			else if(nRet_6 == IO_ON)
			{//530030 1 00 "Top ID Buffer6 Check Off Error."
				sprintf(mc_jamcode, "530030");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9011,  COMI.mn_run_status, mc_jamcode);
				break;
			}
		}

		if(nRet_7 == IO_ON && nRet_8 == IO_ON) //2개다 있을때 
		{
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_LEFT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_RIGHT] = CTL_YES;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] = 1;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuff_num[CTL_LEFT] = 12; //가상정보 
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuff_num[CTL_RIGHT] = 10;				
			
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_product_id[CTL_LEFT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_product_id[CTL_RIGHT], "%s", "01");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_var[CTL_LEFT], "%s", "02");
			sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_var[CTL_RIGHT], "%s", "02");
			
			
// 			ATray tray;
// 			tray.SetPos( IDPOS_TOP_VISION );
// 			for( int i=0; i<MAX_MODULE; i++ )
// 			{
// 				CString strProductID;	strProductID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_product_id[i]);
// 				
// 				CString strLotID;		strLotID.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_product_id[i]);
// 				CString strSkip;		strSkip.Format("%d",st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[i]);
// 				CString strArraySN;		strArraySN.Format("%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuff_num[i] );
// 				CString strSerial="";//		strSerial.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[i]);
// 				CString strVar;			strVar.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_var[i]);
// 				
// 				// ¸ðμa ¼A
// 				AModule mdl;
// 				mdl.SetModuleInfo( strProductID, strLotID, strSkip, strArraySN, strSerial, strVar );
// 				tray.SetModule(i, mdl);
// 			}	
// 			tray.SetTargetPos( IDPOS_TOP_VISION );
// 			tray.SetWorkState( EWS_TOPVISION );
// 			g_Tray.AddTray( tray );
		
		}
		else
		{//어느 하나만 있으면 문제가 있는 것이니 에러 발생하자 
			if(nRet_7 == IO_ON)
			{//530080 1 00 "Top ID Buffer7 Check Off Error."
				sprintf(mc_jamcode, "530080");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9012,  COMI.mn_run_status, mc_jamcode);
				break;
			}
			else if(nRet_8 == IO_ON)
			{//530090 1 00 "Top ID Buffer8 Check Off Error."
				sprintf(mc_jamcode, "530090");  
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9013,  COMI.mn_run_status, mc_jamcode);
				break;
			}
		}
		mn_InitStep = 5000;
		break;

	case 5000:
		mn_fixclamp_sol_flag[0] = 1; mn_fixclamp_sol_flag[1] = 1; mn_fixclamp_sol_flag[2] = 1; mn_fixclamp_sol_flag[3] = 1;//clear 
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == 1)
		{
			mn_fixclamp_sol_flag[0] = 0;
		}
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT] == 1)
		{
			mn_fixclamp_sol_flag[1] = 0;
		}
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT] == 1)
		{
			mn_fixclamp_sol_flag[2] = 0;
		}
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] == 1 || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] == 1)
		{
			mn_fixclamp_sol_flag[3] = 0;
		}

		//안전을 위해 무조건 backward 하자  		
		set_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//clamp
		mn_InitStep = 5010; 
		break;
		
	case 5010: //이때 fix clamp에 id buffer를 체크하는 센서가 있어, unclamp 상태에서는 버퍼를 체크할 수 없다 
		nRet_1 = get_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//clamp
		if(nRet_1 == CTL_GOOD)
		{ 
			mn_InitStep = 6000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9014, COMI.mn_run_status, mc_jamcode);
			mn_InitStep = 5000;
		}
		break;  

	case 6000:
		nRet_1 = idbuffer_move_index_left_input_process(0);
		if(nRet_1 == CTL_GOOD)
		{
			mn_InitStep = 7000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9015, COMI.mn_run_status = dWARNING, mc_jamcode);
			mn_InitStep = 6000;
		}
		break;

	case 7000:
		Func.OnInitBtn( RBTN_TOPID_LD );//2014.1018
		st_handler.mn_init_state[INIT_TOP_IDBUFFER] = CTL_YES;
		mn_InitStep = 0;
		break;

	default:
		break;
	}
}

void CRun_IDBuff_Move_Index::Run_Move_Density()
{
	int i=0, nRet_1=0, nRet_2=0, nRet_3=0;
	CString str_msg;
	int nCnt=0;

 
 
	Func.ThreadFunctionStepTrace( 9, mn_density_step);
	switch(mn_density_step)
	{
	case 0:
		
		mn_density_step = 1000; 
		break;

////////////////////////////////////////////////////////////////////////////////////////////////
//index move 를 동작하기 위해 조건을 체크한다 , 총 2가지 동작 조건으로 제어된다  
///////////////////////////////////////////////
//동작 방법 크게 3가지 
/////////////////////////////////////////////////
//1. 버퍼가 처음 들어와 input index 효율을 위해 미리 잡기 동작을 할때 : step 2000
//2. tbox 등 작업이 끝난 버퍼나, skip 버퍼를 집을때, sealing 작업후 요청시 (left move & pick) : step 3000
//3. 버퍼를 놓을때(right move & place)                      
//	 - sealing을 할때                                                 : step 6000
//   - output buffer로만 이동할때									  : step 7000
//	 - sealing & ouput buffer 동시할때								  : step 6000
/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	case 1000: //조건 체크 		//i/o 상태와 program flag 를 비교한다 
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == CTL_NO && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == CTL_NO )
		{
			//2014.1201 if(st_var.n_event_inputindex_premove_right_start_flag == 1) break; //2014.1127 

			nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
			if(nRet_1 == CTL_GOOD)
			{
				//mn_work_site_num = 0;
				mn_density_step = 2000; //DENSITY 사이트에 버퍼가 없어야 한다 
			} 
		}
//2014.1201 
// 		else if(st_sync.n_top_load1_work_req == CTL_READY) //2014.1126 //idbuffer index move가 input site에 버퍼를 요청한다 
// 		{
// 			nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
// 			if(nRet_1 == CTL_GOOD) //2014.1127
// 			{
// 				mn_density_step = 2200;
// 			}
// 		}
		else if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == CTL_YES)
		{ //최소한 1개 이상의 사이트에서 버퍼를 가지고 있을때만 동작 및 이동 가능하다 
			
			//SHIFT_IDBUFF_INDEX_DENSITY 작업 여부 확인 
			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_RIGHT] == CTL_YES) //작업하지 않고 스킵하는 버퍼일때 
			{//
				 //st_sync.n_density_work_req = CTL_READY; //작업할 필요없음, 작업 스킵  
				if(st_sync.n_density_work_req == CTL_REQ)
				{
					 mn_density_step = 3000;
				}
			} 
			else if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == CTL_YES) //작업할 자재가 있다 
			{
				nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
				if(nRet_1 == CTL_GOOD)
				{
					
					//서버통신
//					if ( g_RunNetwork.st_barcodenet[CTL_LEFT].nComplete == CTL_YES && g_RunNetwork.st_barcodenet[CTL_RIGHT].nComplete == CTL_YES)
//					{
//						// 상위와 통신이 완료 되었으면 
//						g_RunNetwork.st_barcodenet[CTL_LEFT].nComplete = CTL_NO;
//						g_RunNetwork.st_barcodenet[CTL_RIGHT].nComplete= CTL_NO;
//						
//						if( st_basic.nBcrFeederUse[0] == CTL_YES)
//						{
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_LEFT] = 0; // 1번 barcode label 발행 위치 
//							sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].c_Format, "%s", Func.m_pBctFmtClient[0]->GetBcrFormat() );
//
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nCommand = CTL_YES;
//						}
//						else
//						{
//							if( st_basic.nBcrFeederUse[2] == CTL_YES)
//							{
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_LEFT] = 2; // 3번 barcode label 발행 위치 
//								sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].c_Format, "%s",  Func.m_pBctFmtClient[0]->GetBcrFormat() );
//
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nCommand = CTL_YES;
//							}
//						}
//
//						if( st_basic.nBcrFeederUse[1] == CTL_YES)
//						{
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_RIGHT] = 1; // 2번 barcode label 발행 위치 
//							sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].c_Format, "%s",  Func.m_pBctFmtClient[1]->GetBcrFormat() );
//
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nCommand = CTL_YES;
//						}
//						else
//						{
//							if( st_basic.nBcrFeederUse[3] == CTL_YES)
//							{
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_RIGHT] = 3; // 4번 barcode label 발행 위치 
//								sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].c_Format, "%s",  Func.m_pBctFmtClient[1]->GetBcrFormat() );
//
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nCommand = CTL_YES;
//							}
//						}
//					}

					if(st_sync.n_density_work_req == CTL_REQ) //작업 요청  
					{

//						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_LEFT] == 0)
//						{
//							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nComplete == CTL_YES)
//							{
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nComplete = CTL_NO;
//								nRet_1 = CTL_YES;
//							}
//						}
//						else
//						{
//							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nComplete == CTL_YES)
//							{
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nComplete = CTL_NO;
//								nRet_1 = CTL_YES;
//							}
//						}
//						
//						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_RIGHT] == 1)
//						{
//							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nComplete == CTL_YES)
//							{
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nComplete = CTL_NO;							
//								nRet_2 = CTL_YES;
//							}
//						}
//						else
//						{
//							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nComplete == CTL_YES)
//							{
//								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nComplete = CTL_NO;							
//								nRet_2 = CTL_YES;
//							}
//						}
//						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nCommand == CTL_NO &&
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nCommand == CTL_NO &&
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nCommand == CTL_NO &&
//							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nCommand == CTL_NO )
//						{
							mn_density_step = 3000; //각 사이트에서 CTL_REQ한 부분을 작업한다 //buffer site의 상태가 모두 정상 이어야 한다 
//						}
					}
					
// 					if(st_handler.n_without_density_label_rbt == CTL_YES) //≫c¿eCIAo ¾EA¸¹C·I ½ºAμ 
// 					{
// 						st_sync.n_density_work_req = CTL_READY; //AU¾÷CO CE¿a¾øA½, AU¾÷ ½ºAμ
// 					}

// 					nRet_1 = CTL_NO;
// 					nRet_2 = CTL_NO;
// 					if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_LEFT] == 0)
// 					{
// 						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nComplete == CTL_YES)
// 						{
//  							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nComplete = CTL_NO;
// 							nRet_1 = CTL_YES;
// 						}
// 					}
// 					else
// 					{
// 						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nComplete == CTL_YES)
// 						{
//  							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nComplete = CTL_NO;
// 							nRet_1 = CTL_YES;
// 						}
// 					}
// 
// 					if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_RIGHT] == 1)
// 					{
// 						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nComplete == CTL_YES)
// 						{
//  							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nComplete = CTL_NO;							
// 							nRet_2 = CTL_YES;
// 						}
// 					}
// 					else
// 					{
// 						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nComplete == CTL_YES)
// 						{
//  							st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nComplete = CTL_NO;							
// 							nRet_2 = CTL_YES;
// 						}
// 					}

// 					if (nRet_1 == CTL_YES && nRet_2 == CTL_YES)
// 					{
// 						mn_density_step = 3000;
// 					}
// 					if (nRet_1 == CTL_YES && nRet_2 == CTL_YES)
// 					{
// 						mn_density_step = 3000;
// 						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_LEFT] == 0)
// 						{
// 							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nComplete == CTL_YES)
// 							{
// 								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[0].nComplete = CTL_NO;								
// 							}
// 						}
// 						else
// 						{
// 							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nComplete == CTL_YES)
// 							{
// 								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[0].nComplete = CTL_NO;								
// 							}
// 						}
// 						
// 						if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].nBcrPos[CTL_RIGHT] == 1)
// 						{
// 							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nComplete == CTL_YES)
// 							{
// 								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode1[1].nComplete = CTL_NO;
// 							}
// 						}
// 						else
// 						{
// 							if (st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nComplete == CTL_YES)
// 							{
// 								st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].st_barcode2[1].nComplete = CTL_NO;								
// 							}
// 						}
// 					}
				}
				else
				{		
					alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(9020, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);			
				} 	
			} 
		}		
		break;

	///////////////////////////////////////////////////////////
	// input index에서 버퍼를 받는다  
	///////////////////////////////////////////////////////////
	case 2000:
		st_sync.n_top_load1_work_req = CTL_REQ;

		ml_until_wait[0] = GetCurrentTime();
		mn_density_step = 2100;
		break;

	case 2100:
		if(st_sync.n_top_load1_work_req == CTL_READY) //idbuffer index move가 input site에 버퍼를 요청한다 
		{
			nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
			if(nRet_1 == CTL_GOOD) //2014.1127
			{
				mn_density_step = 2200;
			}
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//인덱스 시간을 줄이기 위해 작업이 끝난 버퍼는 이동 시킨다 
		else if(st_sync.n_top_load1_work_req == CTL_REQ) //아직 작업할 버퍼가 8580 장비에서 도착하지 않은 상태로 일정시간 기가린 후 없으면 다음 진행 
		{
			ml_until_wait[1] = GetCurrentTime();
			ml_until_wait[2] =  ml_until_wait[1] - ml_until_wait[0];
			if(ml_until_wait[2] > 5000) //2014.1111 //5초 이상 로딩 버퍼가 공급되지 않으면 현재 장비  안의 버퍼를 순환할 수 있으면 순환시킨다 
			{
				if(st_sync.n_bcr_1_work_req == CTL_REQ && st_sync.n_bcr_2_work_req == CTL_REQ && st_sync.n_topvision_work_req == CTL_REQ && st_sync.n_unload_elevator_req == CTL_REQ)
				{
					if( ((st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT] == CTL_YES)
				 			||	(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT] == CTL_YES )
				 			||	(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] == CTL_YES) )
							&& (st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_NO && st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_NO) ) //unload elevator에는 버퍼가 없어야 한다 
					{
						st_sync.n_top_load1_work_req = CTL_FREE; //flag protect 하기 위해 변경 
						mn_density_step = 2110;
					}
				}
			}
		}
		break;

	case 2110:
		st_sync.n_density_moveindex_shift_req = CTL_REQ;
		mn_density_step = 2120;
		break;
		
	case 2120:
		if(st_sync.n_density_moveindex_shift_req == CTL_READY) //index 가 우측으로 이동하여 data shift가 된후 해당 변수는 CTL_FREE가 다시 조건에 따라 동작하자 
		{
			//2014,1127
// 			nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
// 			if(nRet_1 == CTL_GOOD)
// 			{	
				st_sync.n_density_moveindex_shift_req = CTL_CLEAR;
				mn_density_step = 2130;
// 			}
// 			else
// 			{
// 				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(9205, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);
// 			}			
		}
		break;
		
	case 2130:
		if(st_sync.n_top_load1_work_req == CTL_FREE) //2014.1201 //index move 명령 
		{
			mn_density_step = 2000;
		}
		else
		{
			if(st_sync.n_top_load1_work_req ==  CTL_POSSIBLE || st_sync.n_top_load1_work_req == CTL_BUSY || st_sync.n_top_load1_work_req == CTL_READY)//2014.1201 input index 명령 
			{
				ml_until_wait[0] = GetCurrentTime();
				mn_density_step = 2100;
			}
			
		}
		
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////2014.1111
//////////////////////////////////////////////////////////////////////////////////////////////////////////2014.1111
 

		//버퍼가 8590 장비 SHIFT_IDBUFF_INDEX_DENSITY 사이트에 도착한 상태 
	case 2200:
		set_idbuff_fix_clamp1_2_onoff(0, IO_ON);
		mn_density_step = 2210;
		break;

	case 2210:
		nRet_1 = get_idbuff_fix_clamp1_2_onoff(0, IO_ON);
		if(nRet_1 == CTL_GOOD)
		{
			mn_density_step = 2300;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9021, COMI.mn_run_status, mc_jamcode);
			mn_density_step = 2200;
		}
		break; 

	case 2300:
		nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
		if(nRet_1 == CTL_GOOD)
		{
			mn_density_step = 2400; //DENSITY 사이트에 버퍼가 없어야 한다 
		}	
		else
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9022, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);
		}
		break;

	case 2400:
		st_sync.n_inputindex_moveindex_clamp_exchange_req = CTL_REQ; //id buffer를 갖다 놓았다는 플레그 
		st_sync.n_top_load1_work_req = CTL_CHANGE;
		mn_density_step = 2500;
		break;

	case 2500:
		if(st_sync.n_top_load1_work_req == CTL_SORT) //input index가 클램프를 풀었다 
		{
			st_sync.n_top_load1_work_req = CTL_CLEAR;
			mn_density_step = 2600;
		}
		break;

	case 2600:
		mn_density_step = 1000;
		break;

///////////////////////////////////////////////////////////
// 작업 완료를 기다린다 
///////////////////////////////////////////////////////////
	case 3000:
		nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
		if(nRet_1 == CTL_GOOD)
		{				 
			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == CTL_YES) //top fold 위치 
			{
				if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_skip_flag[CTL_RIGHT] == CTL_YES)
				{
					//이때는 실링 작업을 하지 않는다 
				}
				else
				{
					if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_LEFT]== CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[CTL_RIGHT] == CTL_YES) //t box 
					{//t box가 존재 할때만 진행 
						if(st_handler.n_without_density_label_rbt == CTL_NO) //CTL_YES이면 사용하지 않으므로 스킵 
						{
							st_sync.n_density_work_req = CTL_READY;
						}
					}
				}
			}
			
			mn_density_step = 3100;
		}
		else
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9023, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);
		}			
		break;

	case 3100:

		if(st_sync.n_density_work_req == CTL_REQ) //new work start req
		{//작업이 다 끝나,  바코드 로보트에서 새로운 버퍼를 요청한 상태 
			mn_density_step = 3200;
		}
		break;

	case 3200:
		if(st_sync.n_top_load1_work_req ==  CTL_READY)//2014.1201 input index 명령 
		{//input에서 id버퍼가 동시 이동중이면 처리때까지 대기 .
			 mn_density_step = 2100;
			 break;
		} 

		st_sync.n_density_moveindex_shift_req = CTL_REQ; //인텍스 이동 요청 
		mn_density_step = 3300;
		break;

	case 3300:
		if(st_sync.n_density_moveindex_shift_req == CTL_READY) //index 가 우측으로 이동하여 data shift가 된후 해당 변수는 CTL_FREE가 다시 조건에 따라 동작하자 
		{
//2014.1431
// 			nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
// 			if(nRet_1 == CTL_GOOD || st_var.n_event_inputindex_premove_right_start_flag == 1)
// 			{	
 				st_sync.n_density_moveindex_shift_req = CTL_CLEAR;
 				mn_density_step = 4000; //2014.1201 1000;
// 			}
// 			else
// 			{
// 				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(9024, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);
// 			}			
		}
		break;


	case 4000://2014.1201 
		if(st_sync.n_top_load1_work_req ==  CTL_POSSIBLE || st_sync.n_top_load1_work_req == CTL_BUSY || st_sync.n_top_load1_work_req == CTL_READY)//2014.1201 input index 명령 
		{
			ml_until_wait[0] = GetCurrentTime();
			mn_density_step = 2100;
		}
		else
		{
			mn_density_step = 1000;
		}
		break;


	default:
		break;

	}
}

void CRun_IDBuff_Move_Index::Run_Move()
{
	int i=0, nRet_1=0, nRet_2=0, nRet_3=0, nRet_4=0;
	CString str_msg;
	int nRunStep = 0, nCnt=0;

//	nRunStep = STEP_MOVE_INDEX;
 
	Func.ThreadFunctionStepTrace( 10, mn_run_step);
	switch(mn_run_step)
	{
	case 0:
		
		mn_run_step = 1000; 
		break;

////////////////////////////////////////////////////////////////////////////////////////////////
//index move 를 동작하기 위해 조건을 체크한다 , 총 2가지 동작 조건으로 제어된다  
///////////////////////////////////////////////
//동작 방법 크게 3가지 
/////////////////////////////////////////////////
//1. 버퍼가 처음 들어와 input index 효율을 위해 미리 잡기 동작을 할때 : step 2000
//2. tbox 등 작업이 끝난 버퍼나, skip 버퍼를 집을때, sealing 작업후 요청시 (left move & pick) : step 3000
//3. 버퍼를 놓을때(right move & place)                      
//	 - sealing을 할때                                                 : step 6000
//   - output buffer로만 이동할때									  : step 7000
//	 - sealing & ouput buffer 동시할때								  : step 6000
/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	case 1000: //조건 체크 		//i/o 상태와 program flag 를 비교한다 
		
		if(st_sync.n_density_moveindex_shift_req == CTL_REQ) //buffer가 하나라도 작없이 끝나  동작 가능할때 CTL_REQ를 한다 
		{
			if(st_sync.n_bcr_1_work_req == CTL_REQ && st_sync.n_bcr_2_work_req == CTL_REQ && st_sync.n_topvision_work_req == CTL_REQ &&  st_sync.n_unload_elevator_req == CTL_REQ)
			{
				nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY);  
				nRet_2 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_SITE_ALL_CHK); 

				if(nRet_1 == CTL_GOOD && nRet_2 == CTL_GOOD)
				{
					st_sync.n_inputindex_moveindex_clamp_exchange_req = CTL_CLEAR;
					mn_run_step = 2000;
				}
				else
				{		
					alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(90250, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);			
				} 
			}
			else if(st_sync.n_inputindex_moveindex_clamp_exchange_req == CTL_REQ) //2014.1201 add //move index￠￢| ￠?iAⓒªA￠￢¡¤I AI￥i￠?EA A¡þ¡¤¡ICA¡Æ￠® AaA￠￢￠￢e, ¨oA¡Æ¡I ￠￥UAaA¡i A¡×C¨ª  gripper￠￥A ⓒøo¡Æi ⓒoI￠￢￠c left¡¤I AI￥i￠?CI￠?¨I gripper￠￢| ⓒoI￠￢￠c Aa￠￥A￠￥U 
			{
				if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == CTL_YES)
				{
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer1_chk, IO_ON); 
					nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer2_chk, IO_ON); 
					if(st_handler.n_without_idbuffer == CTL_YES)
					{
						nRet_1 = IO_ON; nRet_2 = IO_ON;
					}
					if(nRet_1 == IO_ON && nRet_2 == IO_ON)
					{
						st_sync.n_inputindex_moveindex_clamp_exchange_req = CTL_CLEAR;
						mn_run_step = 1100;
					}
				}
			}
		}
		else if(st_sync.n_inputindex_moveindex_clamp_exchange_req == CTL_REQ) //move index를 우측으로 이동후 클램프가 잡으면, 시간 단축을 위해  gripper는 놓고 미리 left로 이동하여 gripper를 미리 잡는다 
		{
			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == CTL_YES)
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer1_chk, IO_ON); 
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer2_chk, IO_ON); 
				if(st_handler.n_without_idbuffer == CTL_YES)
				{
					nRet_1 = IO_ON; nRet_2 = IO_ON;
				}
				if(nRet_1 == IO_ON && nRet_2 == IO_ON)
				{
					st_sync.n_inputindex_moveindex_clamp_exchange_req = CTL_CLEAR;
					mn_run_step = 1100;
				}
			}
		}
		break;

	case 1100:
		nRet_1 = idbuffer_move_index_left_input_process(0);
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 1200;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(90260, COMI.mn_run_status = dWARNING, mc_jamcode);
			mn_run_step = 1100;
		}
		break; 

	case 1200:
		set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_ON);		 
		mn_run_step = 1210; 
		break;
		
	case 1210:
		nRet_1 = get_moveindex_gripper_onoff(CTL_DUAL, 0, IO_ON);
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 1300;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9027, COMI.mn_run_status, mc_jamcode);
			mn_run_step = 1200;
		}
		break; 
		
	case 1300:
//2014.1201 
  		mn_run_step = 1000;
  		break;

		mn_fixclamp_sol_flag[0] = 1; mn_fixclamp_sol_flag[1] = 1; mn_fixclamp_sol_flag[2] = 1; mn_fixclamp_sol_flag[3] = 1;//clear 
		//안전을 위해 무조건 backward 하자  		
		set_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//unclamp
		mn_run_step = 1310; 
		break;
		
	case 1310: //이때 fix clamp에 id buffer를 체크하는 센서가 있어, unclamp 상태에서는 버퍼를 체크할 수 없다 
		nRet_1 = get_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//unclamp
		if(nRet_1 == CTL_GOOD)
		{ 
			mn_run_step = 1400;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9028, COMI.mn_run_status, mc_jamcode);
			mn_run_step = 1300;
		}
		break;  

	case 1400:
		mn_run_step = 1000;
		break;


	case 2000:
		nRet_1 = idbuffer_move_index_left_input_process(0);
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 2100;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9029, COMI.mn_run_status = dWARNING, mc_jamcode);
			mn_run_step = 2000;
		}
		break; 

	case 2100: 
		nRet_1 = COMI.Check_MotPosRange(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK],  COMI.md_allow_value[M_TOPID_LD2]); //(int n_Axis, double d_Distance, int n_Range) 
		if (nRet_1 != BD_GOOD)
		{//080012 0 A "MOTOR ALLOW POSITION CHECK ERROR -[M_IDBUFFER_MOVE_INDEX]."
			sprintf(mc_jamcode, "%02d0012", M_TOPID_LD2); //left move pos에 있어야 하는데 다른 위치이면 에러 
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9030,  COMI.mn_run_status, mc_jamcode);
			break;			 
		}
		
		set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_ON);		 
		mn_run_step = 2110; 
		break;
		
	case 2110:
		nRet_1 = get_moveindex_gripper_onoff(CTL_DUAL, 0, IO_ON);
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 2200;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9031, COMI.mn_run_status, mc_jamcode);
			mn_run_step = 2100;
		}
		break; 

	case 2200:
		mn_fixclamp_sol_flag[0] = 1; mn_fixclamp_sol_flag[1] = 1; mn_fixclamp_sol_flag[2] = 1; mn_fixclamp_sol_flag[3] = 1;//clear 
		 //안전을 위해 무조건 backward 하자  		
		set_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//unclamp
		mn_run_step = 2210; 
		break;
		
	case 2210: //이때 fix clamp에 id buffer를 체크하는 센서가 있어, unclamp 상태에서는 버퍼를 체크할 수 없다 
		nRet_1 = get_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//unclamp
		if(nRet_1 == CTL_GOOD)
		{ 
			mn_run_step = 3000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9032, COMI.mn_run_status, mc_jamcode);
			mn_run_step = 2200;
		}
		break;  

	case 3000: 		
		nRet_1 = idbuffer_move_index_right_output_process(0);
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 4000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9033, COMI.mn_run_status = dWARNING, mc_jamcode);
			mn_run_step = 3000;
		}
		break; 

/*
		if(1) //무조건 에러 처리 //st_var.n_top_idbuffer_exist_status_check_info[28] == CTL_YES || st_var.n_top_idbuffer_exist_status_check_info[29] == CTL_YES) //index gripper 7,8번이 잡고 있으면 
		{//fix clamp가 나와 있으면 안됨, 모터 이동시 안전 조건에도 추가되어 있음 
			
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON); 
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_unclamp_chk, IO_ON); 
			nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_unclamp_chk, IO_ON); 
			nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_unclamp_chk, IO_ON); 

			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_YES)
			{//600248 0 A "top t box check io sensor and program flag status check error[out buffer left] -[PS0514].
				sprintf(mc_jamcode, "600248"); 
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(0, COMI.mn_run_status, mc_jamcode);
				break;
			}
			else if(nRet_1 != IO_ON || nRet_2 != IO_ON || nRet_3 != IO_ON || nRet_4 != IO_ON) //fix clamp #3번 clamp 상태 
			{// 
				//526031 1 00 "Top ID Buffer1.2 UnClamp Cylinder On Error."
				//526041 1 00 "Top ID Buffer3.4 UnClamp Cylinder On Error."
				//526051 1 00 "Top ID Buffer5.6 UnClamp Cylinder On Error."
				//526061 1 00 "Top ID Buffer7.8 UnClamp Cylinder On Error."
				if(nRet_1 != IO_ON)			sprintf(mc_jamcode, "526031"); 
				else if(nRet_2 != IO_ON)	sprintf(mc_jamcode, "526041"); 
				else if(nRet_3 != IO_ON)	sprintf(mc_jamcode, "526051"); 
				else                        sprintf(mc_jamcode, "526061"); 
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(0, COMI.mn_run_status, mc_jamcode);			
				break;
			} 
		}		
			
		nRet_1 = COMI.Start_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD], COMI.mn_runspeed_rate);  // 해당 위치로 이동
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{				
			mn_run_step = 3010; 
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_run_step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(0, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_run_step = 3000;
		}
		break;
		
	case 3010:
		nRet_1 = COMI.Check_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD]);  // 이동 완료 확인
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{	
			mn_run_step = 4000;			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_run_step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(0, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_run_step = 3000;
		}
		break;
*/
	case 4000: //시간 단축을 위해(input index 이동을 위해) 먼저 데이터를 이송한다 
		////////////////////////////////////////////////
		// 정보 저장 및 관리 
		////////////////////////////////////////////////
		
		//모든 위치의 정보가 유효하면 바로 전 사이트의 정보를 다음으로 쉬프트한다(이후 해당 구간별 정보를 동작하면서 개별 update 한다 
		//////////////////////////////////////////////////////////
		// Move index 와 연관된 위치의 자료를 쉬프트 한다 
		//////////////////////////////////////////////////////////
		Run_Smema_Ctl.top_idbuffer_datainfo_shift(0, SHIFT_IDBUFF_SITE_ALL_CHK); //data shift & clear 이곳은 
		
		st_sync.n_density_moveindex_shift_req = CTL_READY; //￠®¡×uO￠®Ii￠®Ii Ca¡E￠cioA¡E￠ci A¡E￠c￠®￠?C￠®¡×¡§¡I ￠®E￠®ⓒ­OAu AI¡E￠c¡§￠®¡E￠cA￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I ￠®¡×￠®ⓒ­?￠®¡×uo￠®E￠®ⓒ­| ￠®¡×uA￠®¡×¡E￠cAC￠®¡×¡§¡I A￠®¡×¡§¡I￠®E￠®IU(￠®¡×￠®ⓒ­ACO¡E￠c¡§ua Aa￠®¡×￠®ⓒ­n￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I ¡§Ioo￠®¡×¡E￠cU￠®E￠®ⓒ­| ¡E￠ci￠®E¡Ec￠®E￠®ⓒ­￠®Ec ¡§Io￠®¡×￠®¨I¡E￠c¡§ua A¡E￠c￠®￠?C￠®¡×¡§¡I ￠®¡×￠®ⓒ­?¡E￠c¡§￠®¡§I¡E￠c 
		
		if(st_var.n_event_moveindex_premove_right_start_flag == 1)
		{
			st_var.n_event_moveindex_premove_right_start_flag = 0; //clear 2014.1121
// 			if(st_handler.cwnd_list != NULL)
// 			{
// 				sprintf(st_msg.c_normal_msg, "[event_move1] moveindex_premove_right_start_flag=%d.inputindex_premove_right_start_flag=%d, %d, %d, %d",
// 					st_var.n_event_moveindex_premove_right_start_flag, st_var.n_event_inputindex_premove_right_start_flag, mn_density_step, mn_pick_left_step, mn_place_right_step);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 				Func.On_LogFile_Add(99, st_msg.c_normal_msg);
// 			}
		}
		else
		{
// 			if(st_handler.cwnd_list != NULL)
// 			{
// 				sprintf(st_msg.c_normal_msg, "[event_move2] moveindex_premove_right_start_flag=0.inputindex_premove_right_start_flag=%d, %d, %d, %d",
// 					st_var.n_event_inputindex_premove_right_start_flag, mn_density_step, mn_pick_left_step, mn_place_right_step);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 				Func.On_LogFile_Add(99, st_msg.c_normal_msg);
// 			}
		}

		mn_run_step = 5000;
		break;

	case 5000:
		//2015.0227
		if(st_var.n_event_inputindex_premove_right_start_flag == 1 || st_sync.n_top_load1_work_req == CTL_BUSY) 
// 		if(st_var.n_event_moveindex_premove_right_start_flag == 1) 
		{
// 			if(st_handler.cwnd_list != NULL)
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "[Move_Index_X1] n_top_load1_work_req=%d, n_event_inputindex_premove_right_start_flag=%d, n_event_moveindex_premove_right_start_flag=%d",	
// 					st_sync.n_top_load1_work_req, st_var.n_event_inputindex_premove_right_start_flag, st_var.n_event_moveindex_premove_right_start_flag);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 				Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
// 			}
			mn_run_step = 5100;
			break;
		}

		nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_INDEX_DENSITY); 
		if(nRet_1 == CTL_GOOD || st_var.n_event_inputindex_premove_right_start_flag == 1) //2014.1127
// 		if(nRet_1 == CTL_GOOD || st_var.n_event_moveindex_premove_right_start_flag == 1) //2014.1127
		{
// 			if(st_handler.cwnd_list != NULL)
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "[Move_Index_X2] n_top_load1_work_req=%d, n_event_inputindex_premove_right_start_flag=%d, n_event_moveindex_premove_right_start_flag=%d",	
// 					st_sync.n_top_load1_work_req, st_var.n_event_inputindex_premove_right_start_flag, st_var.n_event_moveindex_premove_right_start_flag);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 				Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
// 			}
			mn_run_step = 5100; //plate & sealing & output buffer site의 상태가 모두 정상 이어야 한다 
		}
		else
		{		
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9034, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);	
			
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[Move_Index_X3] n_top_load1_work_req=%d, n_event_inputindex_premove_right_start_flag=%d, n_event_moveindex_premove_right_start_flag=%d",	
					st_sync.n_top_load1_work_req, st_var.n_event_inputindex_premove_right_start_flag, st_var.n_event_moveindex_premove_right_start_flag);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
			}

		} 		
		break;
		
	case 5100:
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT] == CTL_YES) //top fold 위치 
		{
			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_skip_flag[CTL_RIGHT] == CTL_YES)
			{
				//이때는 실링 작업을 하지 않는다 
			}
			else
			{
				if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_LEFT]== CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[CTL_RIGHT] == CTL_YES) //t box 
				{//t box가 존재 할때만 진행 
					if(st_handler.n_without_bcr1_rbt == CTL_NO) //CTL_YES이면 사용하지 않으므로 스킵 
					{
						st_sync.n_bcr_1_work_req = CTL_READY;
					}					
				}
			}
		}
		
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT] == CTL_YES) //top fold 위치 
		{
			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_skip_flag[CTL_RIGHT] == CTL_YES)
			{
				//이때는 실링 작업을 하지 않는다 
			}
			else
			{
				if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_LEFT]== CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[CTL_RIGHT] == CTL_YES) //t box 
				{//t box가 존재 할때만 진행 
					if(st_handler.n_without_bcr2_rbt == CTL_NO) //CTL_YES이면 사용하지 않으므로 스킵 
					{
						st_sync.n_bcr_2_work_req = CTL_READY;
					}
				}
			}
		}
		
		
		
		
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] == CTL_YES) //top fold 위치 
		{
			if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_skip_flag[CTL_RIGHT] == CTL_YES)
			{
				//이때는 실링 작업을 하지 않는다 
			}
			else
			{
				st_var.n_bcr_read_output_flag = CTL_NO;
				if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_RIGHT] == CTL_YES) //t box 
				{//t box가 존재 할때만 진행 
					if(st_handler.n_without_top_vision_rbt == CTL_NO) //CTL_YES이면 사용하지 않으므로 스킵 
					{
						st_sync.n_topvision_work_req = CTL_READY;
						//2015.0222
						if(st_basic.mn_rightbcr == CTL_YES) //바코드를 사용할때만 동작하자 
						{
							st_var.n_bcr_read_output_flag = CTL_YES;
						}
					}
					
				}
			}
		}
		
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_YES) //top insert 위치 
		{
			st_sync.n_unload_elevator_req = CTL_READY;
		}
		mn_run_step = 6000;
		break;

	case 6000://버퍼 위치별 상태체크 및 클램프 진행 
		mn_fixclamp_sol_flag[0] = 0; mn_fixclamp_sol_flag[1] = 0; mn_fixclamp_sol_flag[2] = 0; mn_fixclamp_sol_flag[3] = 0; 
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT] == CTL_YES) //기본 조건은 CV_DUAL mode 만 존재한다 
		{//gripper #1,#2,#3
			mn_fixclamp_sol_flag[1] = 1; 
		}
		
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT] == CTL_YES ) //기본 조건은 CV_DUAL mode 만 존재한다 
		{//gripper #4,#5,#6
			mn_fixclamp_sol_flag[2] = 1; 
		} 
		
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] == CTL_YES) //기본 조건은 CV_DUAL mode 만 존재한다 
		{//gripper #7,#8
			mn_fixclamp_sol_flag[3] = 1; 
		}
		
		//clamp 가 잡았다 
		set_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_ON); //clamp 	 
		
		mn_run_step = 6010;
		break;
		
	case 6010:
		nRet_1 = get_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_ON); //clamp
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 6100;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(90350, COMI.mn_run_status, mc_jamcode);
			mn_run_step = 6000;
		}
		break; 	

	case 6100:
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_YES) //top insert 위치 
		{
			if(st_sync.n_unload_elevator_req == CTL_FREE) //elevator gripper clamp 상태 
			{
				mn_run_step = 6200;
			}
		}
		else
		{
			mn_run_step = 6200;
		}
		break;

	case 6200:		
		nRet_1 = Run_Smema_Ctl.top_idbuffer_sensor_flag_status_check(0, SHIFT_IDBUFF_SITE_ALL_CHK); //bcr1~SHIFT_IDBUFF_INDEX_ULD
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 7000; //bcr1 ~ SHIFT_IDBUFF_INDEX_ULD buffer site의 상태가 모두 정상 이어야 한다 
		}
		else
		{		
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9036, COMI.mn_run_status, Run_Smema_Ctl.mc_jamcode);			
		} 	
		break;
		
	case 7000: //gripper는 놓았다 
		set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);		 
		mn_run_step = 7010; 
		break;
		
	case 7010:
		nRet_1 = get_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			mn_run_step = 7100;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9037, COMI.mn_run_status, mc_jamcode);
			mn_run_step = 7000;
		}
		break; 	

	case 7100:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk, IO_ON); 
		
		if(nRet_1 != IO_ON && nRet_2 != IO_ON) 
		{	//525051 1 00 "Top ID Buffer LD2 Shifter Left Hook UnClamp Cylinder On Error."
			//525071 1 00 "Top ID Buffer LD2 Shifter Right Hook UnClamp Cylinder On Error."
			if(nRet_1 != IO_ON) sprintf(mc_jamcode, "525051"); 
			else                sprintf(mc_jamcode, "525071");   
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9038, COMI.mn_run_status, mc_jamcode);
			break;
		}		
		
		//미리 모터는 이동하게 한다 
		nRet_1 = COMI.Start_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK], COMI.mn_runspeed_rate * 1.5);  // 해당 위치로 이동		
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{			
			mn_run_step = 7200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_run_step = 7100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9039, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_run_step = 7100;
		}
		break;   

	case 7200:
		nRet_1 = COMI.Check_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK]);  // 이동 완료 확인
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{	
			mn_run_step = 8000;			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_run_step = 7100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9040, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_run_step = 7100;
		}
		break;

	case 8000:
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_YES) //top insert 위치 
		{
			st_sync.n_unload_elevator_req = CTL_CHANGE;
		}
		

// 	 
// 		if((st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_idbuffer[CTL_RIGHT] == CTL_YES)
// 		||	(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_idbuffer[CTL_RIGHT] == CTL_YES)
// 		||	(st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_idbuffer[CTL_RIGHT] == CTL_YES )
// 		||	(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] == CTL_YES && st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] == CTL_YES)
// 		if(	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_YES || 
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_YES )
// 		{
// 			st_sync.n_unload_elevator_req = CTL_READY;
// 			mn_run_step = 1000;
// 		}	  
		mn_run_step = 1000;
 		break;

	case 9000: //2014.1201 
		if(st_sync.n_top_load1_work_req == CTL_REQ)
		{

		}

		break;


 
	default :
		break;
	}
}


int CRun_IDBuff_Move_Index::idbuffer_move_index_left_input_process(int n_site) 
{
	int nRet_1=0, nRet_2=0, nRet_3=0, nRet_4=0, nRet_5=0, nRet_6=0;
	CString str_msg;
	int nSubStep=0;
	int nFunRet=CTL_PROCEED;

//	nSubStep = sb_INDEX_MOVE_LEFT; 

	Func.ThreadFunctionStepTrace( 11, mn_pick_left_step);

	switch(mn_pick_left_step)
	{
	case 0: //버퍼의 자재 상태를
		nRet_1 = COMI.Check_MotPosRange(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK],  COMI.md_allow_value[M_TOPID_LD2]); //(int n_Axis, double d_Distance, int n_Range) 
		if (nRet_1 == BD_GOOD)
		{//080012 0 A "MOTOR ALLOW POSITION CHECK ERROR -[M_IDBUFFER_MOVE_INDEX]."
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_clamp_chk, IO_ON); 
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_clamp_chk, IO_ON); 
			if(nRet_1 == IO_ON && nRet_2 == IO_ON)
			{
				set_moveindex_gripper_length_onoff(0, 0, IO_ON); //벌린
				mn_pick_left_step = 4000;
				break;
			}			
			 
		}
		mn_pick_left_step = 1000;
		break;

	case 1000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk, IO_ON); 
		if(nRet_1 == IO_ON && nRet_2 == IO_ON)
		{
			mn_pick_left_step = 2000;
			break;
		}

		set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);
		mn_pick_left_step = 1010; 
		break;
		
	case 1010: //이때 fix clamp에 id buffer를 체크하는 센서가 있어, unclamp 상태에서는 버퍼를 체크할 수 없다 
		nRet_1 = get_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);		
		
		if(nRet_1 == CTL_GOOD)
		{
			mn_pick_left_step = 2000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9041, COMI.mn_run_status, mc_jamcode);
			mn_pick_left_step = 1000;
		}
		break; 	

	case 2000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk, IO_ON); 
		
		if(nRet_1 != IO_ON && nRet_2 != IO_ON) 
		{	//525051 1 00 "Top ID Buffer LD2 Shifter Left Hook UnClamp Cylinder On Error."
			//525071 1 00 "Top ID Buffer LD2 Shifter Right Hook UnClamp Cylinder On Error."
			if(nRet_1 != IO_ON) sprintf(mc_jamcode, "525051"); 
			else                sprintf(mc_jamcode, "525071");   
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9042, COMI.mn_run_status, mc_jamcode);
			break;
		}	
		

		nRet_1 = COMI.Start_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK], COMI.mn_runspeed_rate * 1.5);  // 해당 위치로 이동
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{	
			set_moveindex_gripper_length_onoff(0, 0, IO_ON); //벌린
			
			mn_pick_left_step = 2100; 
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_pick_left_step = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9043, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_pick_left_step = 2000;
		}
		break;

	case 2100:
		nRet_1 = COMI.Check_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK]);  // 이동 완료 확인
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{	
			mn_pick_left_step = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_pick_left_step = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9044, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_pick_left_step = 2000;
		}
		break;

	case 3000:  
		nRet_1 = get_moveindex_gripper_length_onoff(0, 0, IO_ON);		
		if(nRet_1 == CTL_GOOD)
		{
			mn_pick_left_step = 4000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9045, COMI.mn_run_status, mc_jamcode);
			mn_pick_left_step = 3100;
		}
		break; 

	case 3100:
		set_moveindex_gripper_length_onoff(0, 0, IO_ON); //벌린
		mn_pick_left_step = 3000;
		break;

	case 4000:
		nFunRet = CTL_GOOD;
		mn_pick_left_step = 0;
		break;

	default :
		AfxMessageBox("idbuffer_move_index_left_input_process ==> Step error");
		break;
	}

	return nFunRet;
}

 
int CRun_IDBuff_Move_Index::idbuffer_move_index_right_output_process(int n_site) 
{
	int nRet_1=0, nRet_2=0, nRet_3=0, nRet_4=0, nRet_5=0, nRet_6=0;
	CString str_msg;
	int nSubStep=0;
	int nFunRet=CTL_PROCEED;
	long lMotionDone=0;//2014.1120 
	double dwCurrentPos =0;

//	nSubStep = sb_INDEX_MOVE_RIGHT; 
	Func.ThreadFunctionStepTrace( 12, mn_place_right_step);
	switch(mn_place_right_step)
	{
	case 0: //버퍼의 자재 상태를 체크한다 		
		mn_place_right_step = 1000;
		break;

	case 1000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_clamp_chk, IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_clamp_chk, IO_ON); 
		if(nRet_1 == IO_ON && nRet_2 == IO_ON)
		{
			mn_place_right_step = 2000;
			break;
		}

		set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_ON);		 
		mn_place_right_step = 1010; 
		break;

	case 1010:
		nRet_1 = get_moveindex_gripper_onoff(CTL_DUAL, 0, IO_ON);
		if(nRet_1 == CTL_GOOD)
		{
			mn_place_right_step = 2000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			set_moveindex_gripper_onoff(CTL_DUAL, 0, IO_OFF);

			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9046, COMI.mn_run_status, mc_jamcode);
			mn_place_right_step = 1000;
		}
		break; 

	case 2000:
		mn_fixclamp_sol_flag[0] = 1; mn_fixclamp_sol_flag[1] = 1; mn_fixclamp_sol_flag[2] = 1; mn_fixclamp_sol_flag[3] = 1;//clear 
		//안전을 위해 무조건 backward 하자  		
		set_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//unclamp

		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_unclamp_chk, IO_ON); 
		nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_unclamp_chk, IO_ON); 
		nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_unclamp_chk, IO_ON); 
		if(nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON && nRet_4 == IO_ON) //이미 open되어 있으면 바로 진행하자 
		{
			set_moveindex_gripper_length_onoff(0, 0, IO_OFF);
			mn_place_right_step = 3000;
			break;
		}

		
		mn_place_right_step = 2010; 
		break;
		
	case 2010: //이때 fix clamp에 id buffer를 체크하는 센서가 있어, unclamp 상태에서는 버퍼를 체크할 수 없다 
		nRet_1 = get_idbuff_fix_clamp_select_onoff(mn_fixclamp_sol_flag, IO_OFF);//unclamp
		//nRet_2 = get_moveindex_gripper_length_onoff(0, 0, IO_OFF);
		if(nRet_1 == CTL_GOOD)// && nRet_2 == CTL_GOOD)
		{ 
			set_moveindex_gripper_length_onoff(0, 0, IO_OFF);
			mn_place_right_step = 3000;
		}
		else if(nRet_1 == CTL_ERROR)// || nRet_2 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9047, COMI.mn_run_status, mc_jamcode);
			mn_place_right_step = 2000;
		}
		break;  

	case 3000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON); 
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_unclamp_chk, IO_ON); 
		nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_unclamp_chk, IO_ON); 
		nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_unclamp_chk, IO_ON); 
			
		if(nRet_1 != IO_ON || nRet_2 != IO_ON || nRet_3 != IO_ON || nRet_4 != IO_ON) //fix clamp #3번 clamp 상태 
		{// 
			//526031 1 00 "Top ID Buffer1.2 UnClamp Cylinder On Error."
			//526041 1 00 "Top ID Buffer3.4 UnClamp Cylinder On Error."
			//526051 1 00 "Top ID Buffer5.6 UnClamp Cylinder On Error."
			//526061 1 00 "Top ID Buffer7.8 UnClamp Cylinder On Error."
			if(nRet_1 != IO_ON)			sprintf(mc_jamcode, "526031"); 
			else if(nRet_2 != IO_ON)	sprintf(mc_jamcode, "526041"); 
			else if(nRet_3 != IO_ON)	sprintf(mc_jamcode, "526051"); 
			else                        sprintf(mc_jamcode, "526061"); 
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9048, COMI.mn_run_status, mc_jamcode);			
			break;
		} 

		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[CTL_RIGHT] == CTL_YES) //unload elevator에는 버퍼가 없어야 한다 
		{//600248 0 A "unload id buffer check io sensor and program flag status check error.
			sprintf(mc_jamcode, "600248"); 
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9049, COMI.mn_run_status, mc_jamcode);
			break;
		}

		//unloader 모터가 작업 가능한 위치가 아니면 동작 하면 안됨 
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_LEFT] == CTL_YES || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_idbuffer[CTL_RIGHT] == CTL_YES) //unload elevator로 들어갈 버퍼가있을때는
		{//우선 안전을 위해 무조건 엘리베이터 모터는 작업 가능한 위치에 있어야 한다 
			nRet_1 = COMI.Check_MotPosRange(M_ULD_Z, st_motor[M_ULD_Z].d_pos[P_ULD_Z_UP],  COMI.md_allow_value[M_ULD_Z]); //(int n_Axis, double d_Distance, int n_Range) 
		//	double dpos = g_comiMgr.Get_MotCurrentPos(M_ULD_Z);
			nRet_2 = COMI.Check_MotPosRange(M_IDBUFFER_EL, st_motor[M_IDBUFFER_EL].d_pos[P_UPBUFFER],  COMI.md_allow_value[M_IDBUFFER_EL]); //(int n_Axis, double d_Distance, int n_Range) 

			if ( COMI.Get_MotCurrentPos(M_ULD_Z) > (st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_POS] + COMI.md_allow_value[M_ULD_Z]) &&
				COMI.Check_MotPosRange( M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS], COMI.md_allow_value[M_ULD_Y] ) == BD_GOOD)
			{//080012 0 A "MOTOR ALLOW POSITION CHECK ERROR -[M_IDBUFFER_MOVE_INDEX]."
				sprintf(mc_jamcode, "%02d0008", M_ULD_Z); //left move pos에 있어야 하는데 다른 위치이면 에러 
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9050,  COMI.mn_run_status, mc_jamcode);
				break;			 
			}
			if ( nRet_2 != BD_GOOD )
			{//080012 0 A "MOTOR ALLOW POSITION CHECK ERROR -[M_IDBUFFER_MOVE_INDEX]."
				sprintf(mc_jamcode, "%02d0008", M_IDBUFFER_EL); //left move pos에 있어야 하는데 다른 위치이면 에러 
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9051,  COMI.mn_run_status, mc_jamcode);
				break;			 
			}
			if( g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_hook_clamp_chk) == IO_ON ||
				g_ioMgr.get_in_bit(st_io.i_idbuffer_elv_hook_unclamp_chk) == IO_OFF )
			{
				CString strError="";
				strError.Format("5%04d%d",st_io.i_idbuffer_elv_hook_unclamp_chk, IO_ON);
				CTL_Lib.Alarm_Error_Occurrence(9052, CTL_dWARNING, strError);
				break;
			}


		 
// 			nRet_1 = COMI.Check_MotPosRange(M_ULD_Y, st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS],  COMI.md_allow_value[M_ULD_Y]); //(int n_Axis, double d_Distance, int n_Range) 
// 			if (nRet_1 != BD_GOOD)
// 			{//080012 0 A "MOTOR ALLOW POSITION CHECK ERROR -[M_IDBUFFER_MOVE_INDEX]."
// 				sprintf(mc_jamcode, "%02d0012", P_ULD_Y_PICK_POS); //left move pos에 있어야 하는데 다른 위치이면 에러 
// 				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING;  COMI.mn_run_status = dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(0,  COMI.mn_run_status, mc_jamcode);
// 				break;			 
// 			}  
		}
		
	

		
		nRet_1 = COMI.Start_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD], COMI.mn_runspeed_rate);  // 해당 위치로 이동
		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{	
			st_var.n_event_moveindex_premove_right_start_flag = 0; //clear 2014.1120 
			mn_place_right_step = 3100; 
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_place_right_step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9053, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_place_right_step = 3000;
		}
		break;
		
	case 3100:
		nRet_1 = COMI.Check_SingleMove(M_TOPID_LD2, st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_LAOD]);  // 이동 완료 확인
		if(nRet_1 == BD_PROCEED) //2014.1120 indx time 관련 
		{
			nRet_3 = g_comiMgr._cmmSxIsDone(M_TOPID_LD2, &lMotionDone); //index move가 이동중이고 //모션 동작 완료 == 1, 동작중은 0
			dwCurrentPos = g_comiMgr.Get_MotCurrentPos(M_TOPID_LD2);
			if(nRet_3 == cmERR_NONE &&  lMotionDone != cmTRUE) //문제 없이 이동중이고 
			{
				if(dwCurrentPos > st_motor[M_TOPID_LD2].d_pos[P_LDBUFFER_PICK] + st_set.n_inputindex_premove_mm_value) //right 위치보다 -5mm이상 작으면 아직 우측에 다 도착하지 않았으면 조건을 체크한다 
				{
					if(st_var.n_event_moveindex_premove_right_start_flag == 0) //index move가 이동중일때는 정상이다 
					{
						 st_var.n_event_moveindex_premove_right_start_flag = 1; //2014.1120 
					}
				}
			}
		}

		if (nRet_1 == BD_GOOD) //모터를 로더 장비에 ID BUFFER가 있는 위치로 이동하여 보퍼를 집어 장비로 가져 온다 
		{	
			mn_place_right_step = 4000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			st_var.n_event_moveindex_premove_right_start_flag = 2; //2014.1120 
			mn_place_right_step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_var.n_event_moveindex_premove_right_start_flag = 3; //2014.1120 
			COMI.mn_run_status = dWARNING;  
			CTL_Lib.Alarm_Error_Occurrence(9054, COMI.mn_run_status, COMI.mc_alarmcode);//alarm.str_code = "090004"; alarm.n_count_mode = 0; alarm.n_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			mn_place_right_step = 3000;
		}
		break;


	case 4000:		
		nRet_1 = get_moveindex_gripper_length_onoff(0, 0, IO_OFF);
		if(nRet_1 == CTL_GOOD)
		{ 
			mn_place_right_step = 5000;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; COMI.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(9055, COMI.mn_run_status, mc_jamcode);
			mn_place_right_step = 4100;
		}
		break;  

	case 4100:
		set_moveindex_gripper_length_onoff(0, 0, IO_OFF);
		mn_place_right_step = 4000; 
		break;

	case 5000:
		nFunRet = CTL_GOOD;
		mn_place_right_step = 0;
		break;




	default :
		AfxMessageBox("idbuffer_move_index_right_output_process ==> Step error");
		break;
	}

	return nFunRet;
}


//gripper clamp  
int CRun_IDBuff_Move_Index::set_moveindex_gripper_onoff(int n_site, int n_ionum, int n_onoff)
{
	if(n_onoff == IO_OFF) //normal 상태 normal backward
	{//unclamp
		if(n_site == CTL_LEFT)//left gripper만 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_clamp, IO_OFF); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_unclamp, IO_ON); 
		}
		else if(n_site == CTL_RIGHT)//left gripper만 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_clamp, IO_OFF); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_unclamp, IO_ON); 
		}
		else //if(n_site == CTL_DUAL)
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_clamp, IO_OFF); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_unclamp, IO_ON); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_clamp, IO_OFF); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_unclamp, IO_ON); 
		}	
	}
	else
	{//clamp 
		if(n_site == CTL_LEFT)//left gripper만 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_clamp, IO_ON); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_unclamp, IO_OFF); 
		}
		else if(n_site == CTL_RIGHT)//left gripper만 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_clamp, IO_ON); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_unclamp, IO_ON); 
		}
		else //if(n_site == CTL_DUAL)
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_clamp, IO_ON); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_left_hook_unclamp, IO_OFF); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_clamp, IO_ON); 
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_right_hook_unclamp, IO_OFF); 
		}
	}
	
	mb_index_grippe_flag[n_site] = true;
	ml_index_gripper_wait[n_site][0] = GetCurrentTime();
	return true;
}

//gripper clamp 
int CRun_IDBuff_Move_Index::get_moveindex_gripper_onoff(int n_site, int n_ionum, int n_onoff)
{
	int nFunRet=CTL_PROCEED;
	int nRet_1, nRet_2, nRet_3, nRet_4; 
	
	//525100 1 00 "Top ID Buffer LD1 Shifter Hook Clamp Check Off Error."
	//525101 1 00 "Top ID Buffer LD1 Shifter Hook Clamp Check On Error."
	//525110 1 00 "Top ID Buffer LD1 Shifter Hook UnClamp Check Off Error."
	//525111 1 00 "Top ID Buffer LD1 Shifter Hook UnClamp Check On Error."
	
	ml_index_gripper_wait[n_site][1] = GetCurrentTime();
	ml_index_gripper_wait[n_site][2] = ml_index_gripper_wait[n_site][1] - ml_index_gripper_wait[n_site][0];
	
	if(n_onoff == IO_ON) //clamp 
	{// 
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_clamp_chk, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_OFF);
		nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_clamp_chk, IO_ON);
		nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk, IO_OFF);

		if(n_site == CTL_LEFT)//left gripper만 
		{			 
			nRet_3 = IO_ON;
			nRet_4 = IO_OFF;
		}
		else if(n_site == CTL_RIGHT)
		{
			nRet_1 = IO_ON;
			nRet_2 = IO_OFF;
		}
		else //if(n_site == CTL_DUAL)
		{			 
		}
		
		if(mb_index_grippe_flag[n_site] == true && nRet_1 == IO_ON && nRet_2 == IO_OFF && nRet_3 == IO_ON && nRet_4 == IO_OFF) 
		{
			mb_index_grippe_flag[n_site] = false; 
			ml_index_gripper_wait[n_site][0] = GetCurrentTime();				
		}
		else if(mb_index_grippe_flag[n_site] == false && nRet_1 == IO_ON && nRet_2 == IO_OFF && nRet_3 == IO_ON && nRet_4 == IO_OFF) 
		{
			if(ml_index_gripper_wait[n_site][2] > st_time.n_wait_time[E_WAIT_HOOK_CLAMP][1])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_index_gripper_wait[n_site][2] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
		{
			//525121 1 00 "Top ID Buffer LD2 Shifter Left Hook Clamp Check On Error."
			//525141 1 00 "Top ID Buffer LD2 Shifter Right Hook Clamp Check On Error."
			if(n_site == CTL_LEFT) sprintf(mc_jamcode, "525121"); 
			else if(n_site == CTL_RIGHT) sprintf(mc_jamcode, "525141"); 
			else //if(n_site == CTL_DUAL)
			{
				if(nRet_1 != IO_ON || nRet_2 != IO_OFF) sprintf(mc_jamcode, "525121"); 
				else									sprintf(mc_jamcode, "525141"); 
			}
			nFunRet = CTL_ERROR;
		}
	}
	else //if(n_onoff == IO_OFF)  //unclamp
	{ 
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_clamp_chk, IO_OFF);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk, IO_ON);
		nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_clamp_chk, IO_OFF);
		nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk, IO_ON);
		if(n_site == CTL_LEFT)//left gripper만 
		{			 
			nRet_3 = IO_OFF;
			nRet_4 = IO_ON;
		}
		else if(n_site == CTL_RIGHT)
		{
			nRet_1 = IO_OFF;
			nRet_2 = IO_ON;
		}
		else //if(n_site == CTL_DUAL)
		{			 
		}
		
		if(mb_index_grippe_flag[n_site] == true && nRet_1 == IO_OFF && nRet_2 == IO_ON && nRet_3 == IO_OFF && nRet_4 == IO_ON) 
		{
			mb_index_grippe_flag[n_site] = false; 
			ml_index_gripper_wait[n_site][0] = GetCurrentTime();				
		}
		else if(mb_index_grippe_flag[n_site] == false && nRet_1 == IO_OFF && nRet_2 == IO_ON && nRet_3 == IO_OFF && nRet_4 == IO_ON) 
		{
			if(ml_index_gripper_wait[n_site][2] > st_time.n_wait_time[E_WAIT_HOOK_CLAMP][0])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_index_gripper_wait[n_site][2] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
		{   //525120 1 00 "Top ID Buffer LD2 Shifter Left Hook Clamp Check Off Error."
			//525140 1 00 "Top ID Buffer LD2 Shifter Right Hook Clamp Check Off Error."
			if(n_site == CTL_LEFT) sprintf(mc_jamcode, "525120"); 
			else if(n_site == CTL_RIGHT) sprintf(mc_jamcode, "525140"); 
			else //if(n_site == CTL_DUAL)
			{
				if(nRet_1 != IO_ON || nRet_2 != IO_OFF) sprintf(mc_jamcode, "525120"); 
				else									sprintf(mc_jamcode, "525140"); 
			}
			nFunRet = CTL_ERROR;
		}
	}
	
	return nFunRet;
}


//length
int CRun_IDBuff_Move_Index::set_moveindex_gripper_length_onoff(int n_site, int n_ionum, int n_onoff)
{
	//normal close
	if(n_onoff == IO_OFF) //normal 상태 normal backward, 오므린 상태 
	{//close
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_hook_fwd, IO_OFF); 
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_hook_bwd, IO_ON); 
	}
	else
	{//open 
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_hook_fwd, IO_ON); 
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer_ld2_shifter_hook_bwd, IO_OFF); 
	}
	
	mb_idbuff_index_length_flag = true;
	ml_idbuff_index_length_wait[0] = GetCurrentTime();
	return true;
}


int CRun_IDBuff_Move_Index::get_moveindex_gripper_length_onoff(int n_site, int n_ionum, int n_onoff)
{
	int nFunRet=CTL_PROCEED;
	int nRet_1, nRet_2; 
	
	//526000 1 00 "Top ID Buffer LD2 Shifter Hook Forward Cylinder Off Error."
	//526001 1 00 "Top ID Buffer LD2 Shifter Hook Forward Cylinder On Error."
	//526010 1 00 "Top ID Buffer LD2 Shifter Hook Backward Cylinder Off Error."
	//526011 1 00 "Top ID Buffer LD2 Shifter Hook Backward Cylinder On Error."

	
	ml_idbuff_index_length_wait[1] = GetCurrentTime();
	ml_idbuff_index_length_wait[2] = ml_idbuff_index_length_wait[1] - ml_idbuff_index_length_wait[0];
	
	if(n_onoff == IO_OFF) //normal close 
	{// 
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_hook_fwd_chk, IO_OFF);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_hook_bwd_chk, IO_ON);
		
		if(mb_idbuff_index_length_flag == true && nRet_1 == IO_OFF && nRet_2 == IO_ON) 
		{
			mb_idbuff_index_length_flag = false; 
			ml_idbuff_index_length_wait[0] = GetCurrentTime();				
		}
		else if(mb_idbuff_index_length_flag == false && nRet_1 == IO_OFF && nRet_2 == IO_ON) 
		{
			if(ml_idbuff_index_length_wait[2] > st_time.n_wait_time[E_WAIT_HOOK_FWDBWD][0])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_idbuff_index_length_wait[2] > st_time.n_limit_time[E_WAIT_HOOK_FWDBWD])
		{//526011 1 00 "Top ID Buffer LD2 Shifter Hook Backward Cylinder On Error."
			sprintf(mc_jamcode, "526011"); 
			nFunRet = CTL_ERROR;
		}
	}
	else //if(n_onoff == IO_ON)  //open
	{ 		
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_hook_fwd_chk, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer_ld2_shifter_hook_bwd_chk, IO_OFF);
		
		if(mb_idbuff_index_length_flag == true && nRet_1 == IO_ON && nRet_2 == IO_OFF) 
		{
			mb_idbuff_index_length_flag = false; 
			ml_idbuff_index_length_wait[0] = GetCurrentTime();				
		}
		else if(mb_idbuff_index_length_flag == false && nRet_1 == IO_ON && nRet_2 == IO_OFF) 
		{
			if(ml_idbuff_index_length_wait[2] > st_time.n_wait_time[E_WAIT_HOOK_FWDBWD][1])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_idbuff_index_length_wait[2] > st_time.n_limit_time[E_WAIT_HOOK_FWDBWD])
		{//526001 1 00 "Top ID Buffer LD2 Shifter Hook Forward Cylinder On Error."
			sprintf(mc_jamcode, "526001"); 
			nFunRet = CTL_ERROR;
		}
		
	}
	
	return nFunRet;
}

int CRun_IDBuff_Move_Index::set_idbuff_fix_clamp1_2_onoff(int n_site, int n_onoff)
{
 
	if(n_onoff == IO_OFF) //normal 상태 normal backward, unclamp 상태 
	{
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_clamp, IO_OFF);  
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_unclamp, IO_ON); 
	}
	else//clamp 상태 
	{
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_clamp, IO_ON);  
		g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_unclamp, IO_OFF); 
	}
 
	mb_fix_clamp1_2_flag = true;
	ml_fix_clamp1_2_wait[0] = GetCurrentTime();
	return true; 
}

int CRun_IDBuff_Move_Index::get_idbuff_fix_clamp1_2_onoff(int n_site, int n_onoff)
{
	int nFunRet=CTL_PROCEED;
	int nRet_1, nRet_2; 
		
	ml_fix_clamp1_2_wait[1] = GetCurrentTime();
	ml_fix_clamp1_2_wait[2] = ml_fix_clamp1_2_wait[1] - ml_fix_clamp1_2_wait[0];
	
	if(n_onoff == IO_OFF) //normal open 
	{// 
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_clamp_chk, IO_OFF);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON);
		
		if(mb_fix_clamp1_2_flag == true && nRet_1 == IO_OFF && nRet_2 == IO_ON) 
		{
			mb_fix_clamp1_2_flag = false; 
			ml_fix_clamp1_2_wait[0] = GetCurrentTime();				
		}
		else if(mb_fix_clamp1_2_flag == false && nRet_1 == IO_OFF && nRet_2 == IO_ON) 
		{
			if(ml_fix_clamp1_2_wait[2] > st_time.n_wait_time[E_WAIT_HOOK_FWDBWD][0])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_fix_clamp1_2_wait[2] > st_time.n_limit_time[E_WAIT_HOOK_FWDBWD])
		{////526031 1 00 "Top ID Buffer1.2 UnClamp Cylinder On Error."
			sprintf(mc_jamcode, "526031"); 
			nFunRet = CTL_ERROR;
		}
	}
	else //if(n_onoff == IO_ON)  //close
	{ 		
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_clamp_chk, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_OFF);
		
		if(mb_fix_clamp1_2_flag == true && nRet_1 == IO_ON && nRet_2 == IO_OFF) 
		{
			mb_fix_clamp1_2_flag = false; 
			ml_fix_clamp1_2_wait[0] = GetCurrentTime();				
		}
		else if(mb_fix_clamp1_2_flag == false && nRet_1 == IO_ON && nRet_2 == IO_OFF) 
		{
			if(ml_fix_clamp1_2_wait[2] > st_time.n_wait_time[E_WAIT_HOOK_FWDBWD][1])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_fix_clamp1_2_wait[2] > st_time.n_limit_time[E_WAIT_HOOK_FWDBWD])
		{////526021 1 00 "Top ID Buffer1.2 Clamp Cylinder On Error."
			sprintf(mc_jamcode, "526021"); 
			nFunRet = CTL_ERROR;
		}
		
	}
	
	return nFunRet; 
}


int CRun_IDBuff_Move_Index::set_idbuff_fix_clamp_select_onoff(int n_site[4], int n_onoff)
{
	if(n_site[0] == 1) //fix clamp #1 동작 
	{
		if(n_onoff == IO_OFF) //normal 상태 normal backward, unclamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_clamp, IO_OFF);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_unclamp, IO_ON); 
		}
		else//clamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_clamp, IO_ON);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer12_unclamp, IO_OFF); 
		}
	}
	if(n_site[1] == 1) //fix clamp #2 동작 
	{
		if(n_onoff == IO_OFF) //normal 상태 normal backward, unclamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer34_clamp, IO_OFF);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer34_unclamp, IO_ON); 
		}
		else//clamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer34_clamp, IO_ON);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer34_unclamp, IO_OFF); 
		}
	}
	if(n_site[2] == 1) //fix clamp #3 동작 
	{
		if(n_onoff == IO_OFF) //normal 상태 normal backward, unclamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer56_clamp, IO_OFF);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer56_unclamp, IO_ON); 
		}
		else//clamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer56_clamp, IO_ON);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer56_unclamp, IO_OFF); 
		}
	}

	if(n_site[3] == 1) //fix clamp #4 동작 
	{
		if(n_onoff == IO_OFF) //normal 상태 normal backward, unclamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer78_clamp, IO_OFF);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer78_unclamp, IO_ON); 
		}
		else//clamp 상태 
		{
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer78_clamp, IO_ON);  
			g_ioMgr.set_out_bit(st_io.o_top_idbuffer78_unclamp, IO_OFF); 
		}
	}
 
	mb_fix_clamp_select_flag = true;
	ml_fix_clamp_select_wait[0] = GetCurrentTime();
	return true;
}


int CRun_IDBuff_Move_Index::get_idbuff_fix_clamp_select_onoff(int n_site[4], int n_onoff)
{
	int nFunRet=CTL_PROCEED;
	int nRet_1, nRet_2=0, nRet_3=0, nRet_4=0; 

	
	ml_fix_clamp_select_wait[1] = GetCurrentTime();
	ml_fix_clamp_select_wait[2] = ml_fix_clamp_select_wait[1] - ml_fix_clamp_select_wait[0];
	
	if(n_onoff == IO_ON) //forward 
	{// 
		if(n_site[0] == 1 ) nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_clamp_chk, IO_ON); 
		else nRet_1 = IO_ON;

		if(n_site[1] == 1 ) nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_clamp_chk, IO_ON); 
		else nRet_2 = IO_ON;

		if(n_site[2] == 1 ) nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_clamp_chk, IO_ON);  
		else nRet_3 = IO_ON;

		if(n_site[3] == 1 ) nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_clamp_chk, IO_ON);  
		else nRet_4 = IO_ON;

		if(mb_fix_clamp_select_flag == true && nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON && nRet_4 == IO_ON) 
		{
			mb_fix_clamp_select_flag = false; 
			ml_fix_clamp_select_wait[0] = GetCurrentTime();				
		}
		else if(mb_fix_clamp_select_flag == false && nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON && nRet_4 == IO_ON) 
		{
			if(ml_fix_clamp_select_wait[2] > st_time.n_wait_time[E_WAIT_HOOK_CLAMP][1])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_fix_clamp_select_wait[2] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
		{   //526021 1 00 "Top ID Buffer1.2 Clamp Cylinder On Error."
			//526041 1 00 "Top ID Buffer3.4 Clamp Cylinder On Error."
			//526051 1 00 "Top ID Buffer5.6 Clamp Cylinder On Error."
			//526061 1 00 "Top ID Buffer7.8 Clamp Cylinder On Error."
			if(nRet_1 != IO_ON)			sprintf(mc_jamcode, "526021"); 
			else if(nRet_2 != IO_ON)	sprintf(mc_jamcode, "526041"); 
			else if(nRet_3 != IO_ON)	sprintf(mc_jamcode, "526061"); 
			else                        sprintf(mc_jamcode, "526081"); 
			nFunRet = CTL_ERROR;
		}
	}
	else //if(n_onoff == IO_OFF)  //backward
	{ 
		if(n_site[0] == 1 ) nRet_1 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer12_unclamp_chk, IO_ON); 
		else nRet_1 = IO_ON;

		if(n_site[1] == 1 ) nRet_2 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer34_unclamp_chk, IO_ON); 
		else nRet_2 = IO_ON;

		if(n_site[2] == 1 ) nRet_3 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer56_unclamp_chk, IO_ON);  
		else nRet_3 = IO_ON; 

		if(n_site[3] == 1 ) nRet_4 = g_ioMgr.get_in_bit(st_io.i_top_idbuffer78_unclamp_chk, IO_ON);  
		else nRet_4 = IO_ON; 

		if(mb_fix_clamp_select_flag == true && nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON  && nRet_4 == IO_ON) 
		{
			mb_fix_clamp_select_flag = false; 
			ml_fix_clamp_select_wait[0] = GetCurrentTime();				
		}
		else if(mb_fix_clamp_select_flag == false && nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON  && nRet_4 == IO_ON) 
		{
			if(ml_fix_clamp_select_wait[2] > st_time.n_wait_time[E_WAIT_HOOK_CLAMP][0])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else if(ml_fix_clamp_select_wait[2] > st_time.n_limit_time[E_WAIT_HOOK_CLAMP])
		{   //526031 1 00 "Top ID Buffer1.2 UnClamp Cylinder On Error."
			//526041 1 00 "Top ID Buffer3.4 UnClamp Cylinder On Error."
			//526051 1 00 "Top ID Buffer5.6 UnClamp Cylinder On Error."
			//526061 1 00 "Top ID Buffer7.8 UnClamp Cylinder On Error."
			if(nRet_1 != IO_ON)			sprintf(mc_jamcode, "526031"); 
			else if(nRet_2 != IO_ON)	sprintf(mc_jamcode, "526041"); 
			else if(nRet_3 != IO_ON)	sprintf(mc_jamcode, "526051"); 
			else                        sprintf(mc_jamcode, "526061"); 
			nFunRet = CTL_ERROR;
		}
	}

	return nFunRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CRun_IDBuff_Move_Index::barcode_read_inout_process(int n_site)
{
 	int nRet_1=0, nRet_2=0, nRet_3=0, nRet_4=0, nRet_5=0, nRet_6=0;
	CString str_msg;
	int nFunRet=CTL_PROCEED;
	int nShift_idbuff_Pos, nCheckSite_Pos;

	if(n_site == CTL_LEFT)
	{
		nShift_idbuff_Pos = SHIFT_IDBUFF_INDEX_DENSITY;
		nCheckSite_Pos = CTL_LEFT;
	}
	else
	{
		nShift_idbuff_Pos = SHIFT_IDBUFF_INDEX_TOP;
		nCheckSite_Pos = CTL_RIGHT;
	}

	Func.ThreadFunctionStepTrace( 13, mn_bcrIn_step);

	switch(mn_bcrIn_step)
	{
	case 0: //버퍼의 자재 상태를 체크한다 	
		if(st_basic.mn_rightbcr == CTL_YES) 
		{
			mn_bcrIn_step = 1000;
		}
		else
		{
			mn_bcrIn_step = 6000;
		}
		mn_barcode_retry_cnt[n_site] = 0;
		break;

	//////////////////////////////////////////////////////////////
	//8570 장비에 있는 idbuffer을 집으러 이동하여 집을 때까지 관장 
	//////////////////////////////////////////////////////////////
	case 1000:	
		if(st_basic.mn_rightbcr == CTL_YES) 
		{
			st_sync.n_barcode_read_serial_num[n_site][CTL_LEFT] = 0; //CLEAR 
			st_sync.n_barcode_read_serial_num[n_site][CTL_RIGHT] = 0; //CLEAR 

			ml_barcode_limit_time[n_site][0] = GetCurrentTime();
			if(n_site == CTL_LEFT)
			{
				Func.OnBcrInputreader();
			}
			else
			{
				Func.OnBcrOutputreader();
			}
			mn_bcrIn_step = 2000;
		} 
		break;
		
	case 2000:		 
		if(st_sync.n_barcode_read_serial_num[n_site][nCheckSite_Pos] > 0)
		{//end
			if(st_idbuff_info[nShift_idbuff_Pos].n_idbuff_num[nCheckSite_Pos] == st_sync.n_barcode_read_serial_num[n_site][nCheckSite_Pos])
			{//== ok
				mn_bcrIn_step = 6000;
			}
			else
			{
				if(st_idbuff_info[nShift_idbuff_Pos].n_skip_flag[CTL_LEFT] == CTL_YES || st_idbuff_info[nShift_idbuff_Pos].n_skip_flag[CTL_RIGHT] == CTL_YES)
				{
					mn_bcrIn_step = 6000;
				}
				else
				{
					mn_barcode_retry_cnt[n_site]++;
					if(mn_barcode_retry_cnt[n_site] > st_basic.mn_network_retry_cnt)
					{
						mn_barcode_retry_cnt[n_site] = 0;
						mn_bcrIn_step = 0;
						
						if(n_site == CTL_LEFT)	sprintf(mc_jamcode, "800021"); //800021 0 A "barcode read serial number mismatch error[load Input site]."
						else                    sprintf(mc_jamcode, "800023"); //800023 0 A "barcode read serial number mismatch error[lable output site]." 
						alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; CTL_Lib.mn_run_status = dWARNING;
						CTL_Lib.Alarm_Error_Occurrence(9128, dWARNING, mc_jamcode);					
					}
					else
					{
						mn_bcrIn_step = 1000;
					}
				}
			}
			
		}
		else if(st_sync.n_barcode_read_serial_num[n_site][CTL_LEFT] == -1)
		{
			mn_barcode_retry_cnt[n_site]++;
			if(mn_barcode_retry_cnt[n_site] > st_basic.mn_network_retry_cnt)
			{
				mn_barcode_retry_cnt[n_site] = 0;
				mn_bcrIn_step = 1000;
				
				if(n_site == CTL_LEFT)	sprintf(mc_jamcode, "800020"); //800020 0 A "barcode read error[load Input site]."
				else                    sprintf(mc_jamcode, "800022"); //800022 0 A "barcode read error[lable output site]."
				alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; CTL_Lib.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(9129, dWARNING, mc_jamcode);					
			}
			else
			{
				mn_bcrIn_step = 1000;
			}
		}
		else
		{
			ml_barcode_limit_time[n_site][1] = GetCurrentTime();
			ml_barcode_limit_time[n_site][2] = ml_barcode_limit_time[n_site][1] - ml_barcode_limit_time[n_site][0];
			if(ml_barcode_limit_time[n_site][2] > st_basic.mn_network_wait_time)
			{
				mn_barcode_retry_cnt[n_site]++;
				if(mn_barcode_retry_cnt[n_site] > st_basic.mn_network_retry_cnt)
				{
					mn_barcode_retry_cnt[n_site] = 0;
					mn_bcrIn_step = 1000;
					
					if(n_site == CTL_LEFT)	sprintf(mc_jamcode, "800020"); //800020 0 A "barcode read error[load Input site]."
					else                    sprintf(mc_jamcode, "800022"); //800022 0 A "barcode read error[lable output site]."
					alarm.mstr_code = _T(mc_jamcode); alarm.mn_count_mode = 0; alarm.mn_type_mode = dWARNING; CTL_Lib.mn_run_status = dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(9130, dWARNING, mc_jamcode);					
				}
				else
				{
					mn_bcrIn_step = 1000;
				}
			}
		}
		break;

	case 6000:
		nFunRet = CTL_GOOD;
		mn_bcrIn_step = 0;
		break;
	}

	return nFunRet;
}