// Run_Top_Vision.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Top_Vision.h"
#include "SrcPart\APartRobot.h"


#include "VisionClient.h"
#include "VisionServer.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Top_Vision

IMPLEMENT_SERIAL(CRun_Top_Vision, CObject, 1)
CRun_Top_Vision g_Run_Top_Vision;

CRun_Top_Vision::CRun_Top_Vision()
{
}

CRun_Top_Vision::~CRun_Top_Vision()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_Top_Vision message handlers
void CRun_Top_Vision::Thread_Variable_Initial()
{
	int i = 0;
	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_RestoreStep = 0;
	m_topVision_skipCnt = 0;
	m_nRetryCnt[0] = m_nRetryCnt[1] = 0;
	for ( i = 0; i < 2; i++ )
	{
		m_continus_vis_error[i] = 0;
	}
	m_nInterfaceStep = 0;
	m_nRetryMdlChg = 0;
}

void CRun_Top_Vision::Thread_Run()
{
// return;
	if( Func.OnIsInterruptCondition() != 1 )
		return;

	switch (COMI.mn_run_status)
	{
	case dINIT:
		Run_Initial();
		break;
		
	case dRUN:
// 		return;
 		Run_Move();
		{
			for ( int i = 0;i < 2; i++)
			{
				Run_Move_TopVision(i);
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

// 	Run_TopVision_Move_Chk_Time();
// 	//for ( int j = 0;j < 2; j++)
// 	//{
// 		Run_Move_TopVision(0);
// 	//}

}

void CRun_Top_Vision::Run_Initial()
{
	if( st_handler.mn_init_state[INIT_TOP_VISION] != CTL_NO )	return;
	switch(mn_InitStep)
	{
	case ETVI_START: mn_InitStep = ETVI_CHECK_ROBOT_CHK;	break;

	case ETVI_CHECK_ROBOT_CHK: ETVI_Robot_Check(); break;

	case ETVI_HOMECHK_VISION_X: ETVI_HomeChk_VisionX(); break;

	case ETVI_FINISH: ETVI_Finish(); break;
	}
}


bool CRun_Top_Vision::Run_TopVision_Move_Chk_Jam( stJobPicker job )
{
//	CString strJamCode;
//
//	Func.ThreadFunctionStepTrace(60, job.tvis_step);
//	switch( job.tvis_step )
//	{
//	}

	return true;
}

bool CRun_Top_Vision::Run_TopVision_Move_Chk_Time()
{
// 	switch(mn_RcvyStep)
// 	{
// 	case 0:
// 		if(st_var.n_use_vis_top != CTL_NO ) break;
// 		
// 		st_var.n_use_vis_top = CTL_YES;
// 		
// 		CommTopVision(0);
// 		mn_RcvyStep = 100;
// 		break;
// 
// 	case 100:
// // 		if( GetTopVisionResult(0) == EMBS_PASS || GetTopVisionResult(0) == EMBS_FAIL)
// 		if( m_cwTopviscomm[0] == ECW_NONE )
// 		{
// 			st_var.n_use_vis_top = CTL_NO;
// 			mn_RcvyStep = 0;		
// 		}
// 		break;
// 	}

	return true;
}

bool CRun_Top_Vision::InTimeRobot( int iMilliSec )
{
	DWORD dwtime = GetCurrentTime();
	if( GetCurrentTime() - m_dwTimeMove < 0 )
	{
		m_dwTimeMove = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwTimeMove < iMilliSec )
		return true;

	return false;
}

void CRun_Top_Vision::CommTopVision( int nSite)
{
	m_cwTopviscomm[nSite] = ECW_WORK;
	m_step_topviscomm[nSite] = ECOMM_START;
	m_resultTopvis[nSite] = EMBS_LOAD;
}

void CRun_Top_Vision::Run_Move_TopVision( int nSite )
{
	if( m_cwTopviscomm[nSite] == ECW_NONE ) return;

	switch( m_step_topviscomm[nSite] )
	{
	case ECOMM_START: 
		m_step_topviscomm[nSite] = ECOMM_MOVE;
		if( st_basic.nVisionUse[2] ) m_step_topviscomm[nSite] = ECOMM_CONNECT;
		else{
			Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin = NVR_PASS;
			CString strname = st_basic.mstr_device_name;
			Func.m_pVsClient[EVP_TOP_POS]->m_strModel = strname.Replace(".TXT", "");
			SetTopVisionResult(nSite, EMBS_PASS);
			SetTopVisionMdl(nSite, Func.m_pVsClient[EVP_TOP_POS]->m_strModel);
		}
		m_nRetryCnt[nSite] = 0;
		break;

	case ECOMM_CONNECT:
		if (st_client[CLS_VIS_TOP].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_topviscomm[nSite] = ECOMM_INSP;
		}
		else
		{
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CONNECT, CLS_VIS_TOP);
			m_step_topviscomm[nSite] = ECOMM_CONNECT_CHK;
		}
		break;

	case ECOMM_CONNECT_CHK:
		if (st_client[CLS_VIS_TOP].n_connect == CTL_YES)
		{
			Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin = NVR_NONE;
			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_ON);
			m_dwTimeNetwork[nSite][0] = GetTickCount();
			m_step_topviscomm[nSite] = ECOMM_INSP;
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
				if (m_nRetryCnt[nSite] >= 2/*st_basic.mn_network_retry_cnt*/)
				{

// 					if( COMI.mn_run_status == dRUN || COMI.mn_run_status == dINIT )
// 					{
						m_nRetryCnt[nSite] = 0;
						// 701000 1 A "Received incorrect topvision1 from server."
						strJamCode = "701000";
						CTL_Lib.Alarm_Error_Occurrence(129, CTL_dWARNING, strJamCode );
						::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, CLS_VIS_TOP);

						m_step_topviscomm[nSite] = ECOMM_CONNECT;
// 					}
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();

					m_step_topviscomm[nSite] = ECOMM_CONNECT_WAIT;
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, CLS_VIS_TOP);
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
			m_step_topviscomm[nSite] = ECOMM_CONNECT;
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
		

// 		if (m_dwTimeNetwork[nSite][2] < 200/*st_time.n_wait_time[26][IO_ON]*//*st_time.n_wait_time[E_WAIT_TRIGGER_TIME][IO_ON]*/) break;
//2015.0412		
// 		g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_OFF);
			
// 		Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin = NVR_NONE;
// 		Func.m_pVsClient[EVP_TOP_POS]->OnVisionBcrResult(12);//2014.1118 9->13

		m_step_topviscomm[nSite] = ECOMM_INSP_COM;
		break;

	case ECOMM_INSP_COM:
		if (Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin == NVR_PASS)
		{
			SetTopVisionResult(nSite, EMBS_PASS);
			SetTopVisionMdl(nSite, Func.m_pVsClient[2]->m_strModel);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, CLS_VIS_TOP);

			m_continus_vis_error[nSite] = 0;
			m_step_topviscomm[nSite] = ECOMM_MOVE;
			//2015.0412
 			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_OFF);
		}
		else if (Func.m_pVsClient[EVP_TOP_POS]->m_nBcrResultBin == NVR_FAIL)
		{
			SetTopVisionResult(nSite, EMBS_FAIL);
			SetTopVisionMdl(nSite, Func.m_pVsClient[EVP_TOP_POS]->m_strModel);
// 			::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, CLS_VIS_TOP);
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[TOP VISION_%d] Vision error.", nSite+1);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
			}

			m_step_topviscomm[nSite] = ECOMM_MOVE;
// 			920100 1 00 "Continuous_vision_error_Top_Left_Vision"
// 			920101 1 00 "Continuous_vision_error_Top_Right_Vision"

			//2015.0412
			m_continus_vis_error[nSite]++;
			if(m_continus_vis_error[nSite] >= st_basic.mn_Label_Print_Error_Cnt)
			{
				m_continus_vis_error[nSite] = 0;//2015.0707
				sprintf(cJamCode,"91000%d", nSite);
				CTL_Lib.Alarm_Error_Occurrence(5196, CTL_dWARNING, cJamCode );
			}
			else if(m_continus_vis_error[nSite] < 2)//2015.0419
			{
				m_dwTimeNetwork[nSite][0] = GetTickCount();	
				m_step_topviscomm[nSite] = ECOMM_INSP_COM_WAIT;
			}


			//2015.0412
 			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_OFF);
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
				if (m_nRetryCnt[nSite] >= 2/*st_basic.mn_network_retry_cnt*/)
				{
					m_nRetryCnt[nSite] = 0;
					
					// 702000 1 A "Topvision1 time out from server."
					strJamCode.Format( "702%d00", nSite);
					CTL_Lib.Alarm_Error_Occurrence(130, CTL_dWARNING, strJamCode );					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, CLS_VIS_TOP);
					m_step_topviscomm[nSite] = ECOMM_CONNECT;
				}
				else
				{
					m_dwTimeNetwork[nSite][0] = GetTickCount();					
					::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CLOSE, CLS_VIS_TOP);
					m_step_topviscomm[nSite] = ECOMM_INSP_COM_WAIT;
				}
				//2015.0412
	 			g_ioMgr.set_out_bit(st_io.o_top_vision_trigger_onoff, IO_OFF);
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
			m_step_topviscomm[nSite] = ECOMM_CONNECT;
		}
		break;
	
	case ECOMM_MOVE:
		{
			m_dwTimeCommVision[nSite] = GetCurrentTime();
			m_step_topviscomm[nSite] = ECOMM_FINISH;
		}
		break;

	case ECOMM_FINISH:
		if( GetCurrentTime() - m_dwTimeCommVision[nSite] < 0 ) m_dwTimeCommVision[nSite] = GetCurrentTime();
		if( GetTopVisionResult(nSite) == EMBS_PASS)
		{
			SetTopVisionResult( nSite, EMBS_PASS);
			m_step_topviscomm[nSite] = ECOMM_NONE;
			m_cwTopviscomm[nSite] = ECW_NONE;
			
			CString strModel = st_basic.mstr_device_name;
			strModel.Replace(".TXT", "");
// 			if( GetTopVisionMdl(nSite) != strModel)
// 			{
// 				SetTopVisionResult( nSite, EMBS_FAIL);
// 				if(st_handler.cwnd_list != NULL)
// 				{
// 					sprintf(st_msg.c_abnormal_msg, "[TOP VISION_%d] Vision error.Model error.", nSite+1);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 					Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
// 				}
// 			}			
		}
		else if( GetTopVisionResult(nSite) == EMBS_FAIL)
		{
			m_cwTopviscomm[nSite] = ECW_NONE;
			m_step_topviscomm[nSite] = ECOMM_NONE;
			SetTopVisionResult( nSite, EMBS_FAIL);
		}
		break;
	}
}

void CRun_Top_Vision::Run_Move()
{
	int nJobCnt = Func.m_pRobot[SITE_TOP_VISION]->GetJobCount();
	if( nJobCnt == 0 )
		return;

	stJobPicker& job = Func.m_pRobot[SITE_TOP_VISION]->GetFirstJob();

	if( tvis_step_old != job.tvis_step)
	{
		tvis_step_old = job.tvis_step;
		m_dwTimeMove = GetCurrentTime();
		if( nJobCnt == 0 )
			return;
			
		if( Run_TopVision_Move_Chk_Jam( job ) == false )
			return;
	}
	if( Run_TopVision_Move_Chk_Time() == false )
		return;
	Func.ThreadFunctionStepTrace(61, job.tvis_step);
	
	switch( job.tvis_step )
	{
	case TVISION_NONE:	break;

	case TVISION_READY:
		{
			CString strLotID="";
			if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_LEFT] == CTL_YES &&
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_LEFT] == ID_LOT_START)
			{
				strLotID.Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_lot_id[CTL_LEFT]);
			}
			else if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[CTL_RIGHT] == CTL_YES &&
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_last_module[CTL_RIGHT] == ID_LOT_START)
			{
				strLotID.Format("%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_lot_id[CTL_RIGHT]);
			}
			else
			{
				m_dwTimeTact = GetCurrentTime();
				job.tvis_step = TVISION_TEST_READY;
				break;
			}


			if(strLotID != "")
			{
				//g_lotMgr.GetLotByLotID(strLotID).SetStrPosVisionModel(false);
				if(g_lotMgr.GetLotByLotID(strLotID).GetStrSetPosVisionModel() == "YES")
				{
					int n_ret = OnVisionModelChange(EVP_BTM_POS);					
					if( n_ret == RET_GOOD)
					{
						m_nRetryMdlChg = 0;
						g_lotMgr.GetLotByLotID(strLotID).SetStrPosVisionModel(false);
						m_dwTimeTact = GetCurrentTime();
						job.tvis_step = TVISION_TEST_READY;
					}
				}
				else
				{
					m_dwTimeTact = GetCurrentTime();
					job.tvis_step = TVISION_TEST_READY;
				}
			}
			else
			{
				//940003 0 00 "Top Vision model change error"
				//940004 0 00 "Bottom Vision model change error"
				CString strErr="";
				strErr.Format("94000%d", EVP_TOP_POS);
				CTL_Lib.Alarm_Error_Occurrence( 5891, CTL_dWARNING, strErr );
			}

//			int nRet = CTL_Lib.Single_Move( M_TOP_VISION_X, st_motor[M_TOP_VISION_X].d_pos[P_VIS1_POS], st_basic.nRunSpeed );
//			if( nRet == BD_GOOD )
//			{
// 				job.tvis_step = TVISION_TEST_READY;
//			}
//			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
//			{
//				CTL_Lib.Alarm_Error_Occurrence( 1000, CTL_dWARNING, alarm.mstr_code );
//			}
// 				m_dwTimeTact = GetCurrentTime();
		}
		break;

	case TVISION_TEST_READY:
		//2015.0326
		if(st_basic.nTopVisionSkip == CTL_YES && st_var.n_use_top_skip == CTL_YES)
		{
			if(job.iRobotFirst != -1)
			{
				st_handler.mn_Top_Vision[job.iRobotFirst] = CTL_GOOD;
// 				sprintf(st_top_rbt[job.iRobotFirst].st_top.c_model_id,"%s",Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
				st_top_rbt[job.iRobotFirst].nState = EMBS_PASS;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotFirst] = CTL_GOOD;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].nTop_vis[job.iRobotFirst] = CTL_GOOD;
			}
			if(job.iRobotSecond != -1)
			{
				st_handler.mn_Top_Vision[job.iRobotSecond] = CTL_GOOD;
// 				sprintf(st_top_rbt[job.iRobotSecond].st_top.c_model_id,"%s",Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
				st_top_rbt[job.iRobotSecond].nState = EMBS_PASS;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotSecond] = CTL_GOOD;
				st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].nTop_vis[job.iRobotSecond] = CTL_GOOD;
			}
			job.tvis_step = TVISION_STEP(82);
			if(st_basic.mn_rightbcr == CTL_YES)
			{
				m_dwTimeReadBcr = GetCurrentTime();
				job.tvis_step = TVISION_STEP(81);
			}
		}
		else
		{
			if(job.iRobotFirst != -1)
			{
				double dPos = COMI.Get_MotCurrentPos( M_TOP_VISION_X);
				int nRet_1 = COMI.Check_MotPosRange(M_TOP_VISION_X, st_motor[M_TOP_VISION_X].d_pos[P_VIS1_POS + job.iRobotFirst], COMI.md_allow_value[M_TOP_VISION_X]);
				if (nRet_1 == BD_GOOD)
				{
					//vision test;
	// 				st_handler.mn_Top_Vision[job.iRobotFirst] = CTL_NO;
					job.tvis_step = TVISION_TEST1_COMM;
				}
				else
				{
					job.tvis_step = TVISION_TEST1_READY;
				}
				
			}
			else
			{
				job.tvis_step = TVISION_TEST2_READY;
			}
		}
		break;

	case TVISION_TEST1_READY:
		{
//			if(job.iRobotFirst == -1) {
//				job.tvis_step = TVISION_TEST_READY;
//				break;
//			}
			int nRet = CTL_Lib.Single_Move( M_TOP_VISION_X, st_motor[M_TOP_VISION_X].d_pos[P_VIS1_POS + job.iRobotFirst], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.tvis_step = TVISION_TEST_READY;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1001, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case TVISION_TEST1_COMM:
		
		if(st_var.n_use_vis_top != CTL_NO ) break;

		st_var.n_use_vis_top = CTL_YES;

// 		m_dwTimeVision = GetCurrentTime();
		job.tvis_step = TVISION_STEP(41);
		break;

	case 41:
// 		if( GetCurrentTime() - m_dwTimeVision <= 0 ) m_dwTimeVision = GetCurrentTime();
// 		if( GetCurrentTime() - m_dwTimeVision < 1000) break;

		Func.m_pVsClient[EVP_TOP_POS]->SetResultBcrTest(NVR_NONE);

		m_strBcrcode[job.iRobotFirst].Format("[TOPVIS_1]%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_serial[job.iRobotFirst]);
		Func.On_LogFile_Add(99, m_strBcrcode[job.iRobotFirst]);
		CommTopVision(job.iRobotFirst);

		job.tvis_step = TVISION_TEST1_RESULT;
		break;

	case TVISION_TEST1_RESULT:

		//2015.0419
		if( m_cwTopviscomm[job.iRobotFirst] == ECW_NONE && GetTopVisionResult(job.iRobotFirst) == EMBS_PASS)
		{
			st_handler.mn_Top_Vision[job.iRobotFirst] = CTL_GOOD;
			sprintf(st_top_rbt[job.iRobotFirst].st_top.c_model_id,"%s",Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
			st_top_rbt[job.iRobotFirst].nState = EMBS_PASS;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotFirst] = CTL_GOOD;//2015.0211
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].nTop_vis[job.iRobotFirst] = CTL_GOOD;//2015.0228

		}
		else if( m_cwTopviscomm[job.iRobotFirst] == ECW_NONE && GetTopVisionResult(job.iRobotFirst) == EMBS_FAIL)
		{
			st_handler.mn_Top_Vision[job.iRobotFirst] = CTL_ERROR;
			sprintf(st_top_rbt[job.iRobotFirst].st_top.c_model_id,"%s",Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
			st_top_rbt[job.iRobotFirst].nState = EMBS_FAIL;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotFirst] = CTL_ERROR;//2015.0211
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].nTop_vis[job.iRobotFirst] = CTL_ERROR;//2015.0228
		}
		if( st_handler.mn_Top_Vision[job.iRobotFirst] != CTL_NO)
		{
// 			st_handler.mn_Top_Vision[1] = CTL_NO;
			if(job.iRobotSecond != -1)
			{
// 				st_handler.mn_Top_Vision[job.iRobotSecond] = CTL_NO;
				job.tvis_step = TVISION_TEST2_READY;
			}
			else
			{
				job.tvis_step = TVISION_STEP(82);//TVISION_FINISH;
				st_var.n_use_vis_top = CTL_NO;
			}
		}
		break;

	case TVISION_TEST2_READY:
		{
			int nRet = CTL_Lib.Single_Move( M_TOP_VISION_X, st_motor[M_TOP_VISION_X].d_pos[P_VIS2_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.tvis_step = TVISION_TEST2_COMM;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1002, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case TVISION_TEST2_COMM:
// 		if(st_var.n_use_vis_top != CTL_NO ) break;
// 		st_var.n_use_vis_top = CTL_YES;

		//네트워크 통신 정송
		Func.m_pVsClient[EVP_TOP_POS]->SetResultBcrTest(NVR_NONE);
		m_strBcrcode[job.iRobotSecond].Format("[TOPVIS_2]%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_serial[job.iRobotSecond]);
		Func.On_LogFile_Add(99, m_strBcrcode[job.iRobotSecond]);
		CommTopVision(job.iRobotSecond);

		job.tvis_step = TVISION_TEST2_RESULT;
		break;

	case TVISION_TEST2_RESULT:
		//2015.0419
		if( m_cwTopviscomm[job.iRobotSecond] == ECW_NONE && Func.m_pVsClient[EVP_TOP_POS]->GetResultBcrTest() == NVR_PASS)
		{
			st_handler.mn_Top_Vision[job.iRobotSecond] = CTL_GOOD;
			sprintf(st_top_rbt[job.iRobotSecond].st_top.c_model_id,"%s",Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
			st_top_rbt[job.iRobotSecond].nState = EMBS_PASS;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotSecond] = CTL_GOOD;//2015.0211
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].nTop_vis[job.iRobotSecond] = CTL_GOOD;//2015.0228
		}
		else if( m_cwTopviscomm[job.iRobotSecond] == ECW_NONE && Func.m_pVsClient[EVP_TOP_POS]->GetResultBcrTest() == NVR_FAIL)
		{
			st_handler.mn_Top_Vision[job.iRobotSecond] = CTL_ERROR;
			sprintf(st_top_rbt[job.iRobotSecond].st_top.c_model_id,"%s",Func.m_pVsClient[EVP_TOP_POS]->m_strModel );
			st_top_rbt[job.iRobotSecond].nState = EMBS_FAIL;
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_RetVision[job.iRobotSecond] = CTL_ERROR;//2015.0211
			st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].nTop_vis[job.iRobotSecond] = CTL_ERROR;//2015.0228
		}

		if( st_handler.mn_Top_Vision[job.iRobotSecond] != CTL_NO)
		{
			job.tvis_step = TVISION_STEP(82);//TVISION_FINISH;
			//2015.0222
			if(st_basic.mn_rightbcr == CTL_YES) //바코드를 사용할때만 동작하자 
			{
				m_dwTimeReadBcr = GetCurrentTime();
				job.tvis_step = TVISION_STEP(81);
			}
// 			st_var.n_use_vis_top = CTL_NO;
		}
		break;

	case 81:
		if(st_basic.mn_rightbcr == CTL_YES && st_var.n_bcr_read_output_flag == CTL_YES) break;
		job.tvis_step = TVISION_STEP(82);//TVISION_FINISH;
		break;

	case 82:
		{
			int nRet = CTL_Lib.Single_Move( M_TOP_VISION_X, st_motor[M_TOP_VISION_X].d_pos[P_VIS1_POS], st_basic.nRunSpeed );
			if( nRet == BD_GOOD )
			{
				job.tvis_step = TVISION_FINISH;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1099, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case TVISION_FINISH:
		{
			if( job.iRobotFirst != -1 )
			{
				int nTrayIdx = g_Tray.GetIdxByPos( IDPOS_TOP_VISION );
				if( st_handler.mn_Top_Vision[job.iRobotFirst] == CTL_GOOD )
				{
//					g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst % MPOS_INDEX_TOPVISION ).GetTopVision().SetTopVisionState(EMBS_PASS);
//					g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst % MPOS_INDEX_TOPVISION ).SetModuleState(EMS_VISION);					
				}
				else
				{
//					g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst % MPOS_INDEX_TOPVISION ).GetTopVision().SetTopVisionState(EMBS_FAIL);
//					g_Tray.GetTray( nTrayIdx ).GetModule( job.iPosFirst % MPOS_INDEX_TOPVISION ).SetModuleState(EMS_REJECT);
				}
				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].st_top[job.iRobotFirst], &st_top_rbt[job.iRobotFirst], sizeof(st_top_param));
				memset( &st_top_rbt[job.iRobotFirst], 0x00, sizeof(st_top_param));
			}
			
			if( job.iRobotSecond != -1 )
			{
				int nTrayIdx = g_Tray.GetIdxByPos( IDPOS_TOP_VISION );
				if( st_handler.mn_Top_Vision[job.iRobotSecond] == CTL_GOOD )
				{
// 					g_Tray.GetTray( nTrayIdx ).GetModule( job.iRobotSecond % MPOS_INDEX_TOPVISION ).GetTopVision().SetTopVisionState(EMBS_PASS);
// 					g_Tray.GetTray( nTrayIdx ).GetModule( job.iRobotSecond % MPOS_INDEX_TOPVISION ).SetModuleState(EMS_VISION);
				}
				else
				{
// 					g_Tray.GetTray( nTrayIdx ).GetModule( job.iRobotSecond % MPOS_INDEX_TOPVISION ).GetTopVision().SetTopVisionState(EMBS_FAIL);
// 					g_Tray.GetTray( nTrayIdx ).GetModule( job.iRobotSecond % MPOS_INDEX_TOPVISION ).SetModuleState(EMS_REJECT);
				}
				memcpy( &st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].st_top[job.iRobotSecond], &st_top_rbt[job.iRobotSecond].st_top, sizeof(st_top_param));
				memset( &st_top_rbt[job.iRobotSecond], 0x00, sizeof(st_top_param));

			}
			//2015.0326
			if(st_basic.nTopVisionSkip == CTL_YES)
			{
				if(st_var.n_use_top_skip == CTL_NO)
				{
					if(job.iRobotFirst != -1 && job.iRobotSecond != -1)
					{
						if( st_handler.mn_Top_Vision[job.iRobotFirst] == CTL_GOOD &&
							st_handler.mn_Top_Vision[job.iRobotSecond] == CTL_GOOD )
						{
							m_topVision_skipCnt++;
							if(m_topVision_skipCnt >= st_basic.nTopVisionSkipCnt)
							{
								m_topVision_skipCnt = 0;
								st_var.n_use_top_skip = CTL_YES;
							}
						}
						else
						{
							m_topVision_skipCnt = 0;//reset
						}
					}
				}
			}
			
			if(nJobCnt <= 1)//2015.0320
				st_sync.n_topvision_work_req = CTL_REQ;

			Func.m_pRobot[SITE_TOP_VISION]->EraseFirstJob();

//			Func.BackupData();//2013,1018
			Func.Handler_Recovery_Data_Write();

			if(GetCurrentTime() - m_dwTimeTact <= 0 ) m_dwTimeTact = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeTact >= 0 && GetCurrentTime() - m_dwTimeTact < 900000 &&
				st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[TOPVISION]Tact time : %d", GetCurrentTime() - m_dwTimeTact);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
//				Func.On_LogFile_Add(LOG_MACHINE, st_msg.c_normal_msg);
			}
			m_dwTimeTact = GetCurrentTime();

			st_var.n_use_vis_top = CTL_NO;

		}
		
		break;
	}
		
}

void CRun_Top_Vision::Run_Reinstate()
{
	if( st_handler.mn_Rcvy_state[RCVY_TOPVISION] == CTL_YES) return;

	switch(mn_RcvyStep) 
	{
	case 0:
		{
			int nRet = COMI.Check_MotPosRange( M_TOP_VISION_X, COMI.md_cmdpos_backup[M_TOP_VISION_X], COMI.md_allow_value[M_TOP_VISION_X]);

			bool nRecy = false;
			if( nRet == BD_ERROR )
			{
				m_nReq_Rcvy = CTL_NO;
				if( nRet == BD_ERROR )
				{
					m_nReq_Rcvy = CTL_YES;
					nRecy = true;
				}
			}

			if( nRecy == false ) mn_RcvyStep = 9000;
			else
			{
				mn_RcvyStep = 100;
			}
		}
		break;

	case 100:
		{
			int nRet1 = CTL_Lib.Single_Move(M_TOP_VISION_X, COMI.md_cmdpos_backup[M_TOP_VISION_X], (int)st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD)
			{
				mn_RcvyStep = 9000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
			}

		}
		break;

	case 9000:
		st_handler.mn_Rcvy_state[RCVY_TOPVISION] = CTL_YES;
		mn_RcvyStep = 0;
		break;
	
	}
}

void CRun_Top_Vision::Run_Restore()
{
}

void CRun_Top_Vision::OnRobot_FinalPos()
{
}

void CRun_Top_Vision::Serialize(CArchive &ar)
{
}

void CRun_Top_Vision::ETVI_Robot_Check()
{
	mn_InitStep = ETVI_HOMECHK_VISION_X;
}

void CRun_Top_Vision::ETVI_HomeChk_VisionX()
{
	int nRet = COMI.HomeCheck_Mot( M_TOP_VISION_X, MOT_TIMEOUT);
	if( nRet == BD_GOOD )
	{
		mn_InitStep = ETVI_FINISH;
//		Func.OnInitBtn( RBTN_SITE1_Z + (m_nSiteID - INIT_TEST_SITE_1 ) * 5 );
	}
	else if( nRet == BD_ERROR )
	{//270001 0 00 "Top Vision X Axis Motor Home Check Error."
		CString strJamCode = "270001";
		CTL_Lib.Alarm_Error_Occurrence(1004, CTL_dWARNING, strJamCode );
	}
}

void CRun_Top_Vision::ETVI_Finish()
{
	Func.OnInitBtn( RBTN_TOP_VISION_X );//2014.1018
	st_handler.mn_init_state[INIT_TOP_VISION] = CTL_YES;
	mn_InitStep = ETVI_START;
}

int	CRun_Top_Vision::OnVisionModelChange(int port)
{
	int nFuncRet = RET_PROCEED;
	int m_nClientPos = 10 + port;
	switch(m_nInterfaceStep)
	{
	case 0:
		if(st_client[m_nClientPos].n_connect == CTL_YES)
		{
			m_nInterfaceStep = 200;
		}
		else
		{
			m_dwTimeChangeWork = GetCurrentTime();
			m_nInterfaceStep = 100;
			
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CONNECT, m_nClientPos);	
		}
		break;
		
	case 100:
		if(st_client[m_nClientPos].n_connect == CTL_YES)
		{
			m_nInterfaceStep = 200;
		}
		else
		{	
			if(GetCurrentTime() - m_dwTimeChangeWork < 0)
			{
				m_dwTimeChangeWork = GetCurrentTime();
				break;
			}
			
			if(GetCurrentTime() - m_dwTimeChangeWork > 5000)
			{
				m_nInterfaceStep = 0;
				m_nRetryMdlChg++;
				if( m_nRetryMdlChg > st_basic.mn_network_retry_cnt)
				{
					m_nRetryMdlChg = 0;
					return RET_ERROR;
				}
			}
		}
		break;
		
	case 200:
		Func.m_pVsClient[EVP_DENSITY+port]->m_nModelChangeBin		= CTL_CLEAR;
		st_client[m_nClientPos].n_rev_info				= CTL_YES;			
		m_dwTimeChangeWork = GetCurrentTime();
		
		//2015.0518
// 		clsVsClient[m_nClientPos].OnVisionModelChange(m_nClientPos, st_basic.mstr_device_name);
		clsVsClient[port].OnVisionModelChange(m_nClientPos, st_basic.mstr_device_name);
		
		m_nInterfaceStep = 300;
		break;
		
	case 300:		
		if(Func.m_pVsClient[EVP_DENSITY+port]->m_nModelChangeBin	 == NVR_PASS)
		{
			m_nRetryMdlChg = 0;
			m_nInterfaceStep = 0;
			
			return RET_GOOD;
		}
		else if(Func.m_pVsClient[EVP_DENSITY+port]->m_nModelChangeBin== NVR_FAIL)
		{		
			m_nInterfaceStep = 0;
			m_nRetryMdlChg++;
			if( m_nRetryMdlChg > st_basic.mn_network_retry_cnt)
			{
				m_nRetryMdlChg = 0;
				CString strErr="";
				strErr.Format("94000%d", port+1);
				CTL_Lib.Alarm_Error_Occurrence( 4491, CTL_dWARNING, strErr );
				return RET_ERROR;
			}
		}
		else 
		{
			if(GetCurrentTime() - m_dwTimeChangeWork < 0)
			{
				m_dwTimeChangeWork = GetCurrentTime();
				break;
			}
			
			if(GetCurrentTime() - m_dwTimeChangeWork  > 5000)
			{
				::SendMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nClientPos, CLIENT_CLOSE, m_nClientPos);

// 				940001 0 00 "Density Vision model change error";
// 				940002 0 00 "Barcode Vision model change error";
// 				940003 0 00 "Top Vision model change error";
// 				940004 0 00 "Bottom Vision model change error";

				m_nRetryMdlChg++;
				if( m_nRetryMdlChg > st_basic.mn_network_retry_cnt)
				{
					m_nRetryMdlChg = 0;
					CString strErr="";
					strErr.Format("94000%d", port+1);
					CTL_Lib.Alarm_Error_Occurrence( 4491, CTL_dWARNING, strErr );
					m_nInterfaceStep = 0;					
					return RET_ERROR;
				}
				else
				{
					m_dwTimeChangeWork = GetCurrentTime();
					m_nInterfaceStep = 400;
				}
			}
		}
		break;

	case 400:
		if(GetCurrentTime() - m_dwTimeChangeWork < 0)
		{
			m_dwTimeChangeWork = GetCurrentTime();
			break;
		}
		if(GetCurrentTime() - m_dwTimeChangeWork  > 3000)
		{
			m_nInterfaceStep = 0;
		}
		break;
	}
	
	return RET_PROCEED;
}