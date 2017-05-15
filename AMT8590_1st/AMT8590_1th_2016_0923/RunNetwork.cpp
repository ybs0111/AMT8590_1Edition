// RunNetwork.cpp: implementation of the CRunNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "RunNetwork.h"
#include "Variable.h"
#include "AMTLotManager.h"
#include "Public_Function.h"
#include "BarcodeFormat.h"
#include "Run_Barcode_Feeders.h"
#include "MesEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRunNetwork g_RunNetwork;

CRunNetwork::CRunNetwork()
{
	for (int i=0; i<2; i++)
	{
		// 통신관련 초기화
		st_barcodenet[i].nReady	= CTL_YES; // barcode label 대기중 
		st_barcodenet[i].nCommand	= CTL_NO;
		st_barcodenet[i].nComplete	= CTL_NO;

		st_barcodenet[i].strBarcodeFormat	= _T("");
		st_barcodenet[i].strSerial			= _T("");
	}

// 	st_handler.mn_Lotcnt = 0;
}

CRunNetwork::~CRunNetwork()
{

}

void CRunNetwork::OnRunMove()
{
	int i;
	if(COMI.mn_run_status != dRUN) return;
	for (i=0; i<=1; i++)
	{
		OnRunLabelCheck(i);
	}
}


void CRunNetwork::OnRunLabelCheck(int nSite)
{

	int nRet = 0;
// 	Func.ThreadFunctionStepTrace(91, m_nRunStep[nSite]);
	switch (m_nRunStep[nSite])
	{
		case 0:
			// density work에 label 밠행 요청이 있으면 
			if (st_barcodenet[nSite].nCommand != CTL_YES) break;

			if (st_barcodenet[nSite].nCommand == CTL_NO && st_barcodenet[nSite].nComplete == CTL_NO) break;
			
			st_barcodenet[nSite].nCommand	= CTL_NO;
			m_nRunStep[nSite] = 100;
			break;

		case 100:
			if(st_basic.mn_mode_run == CTL_NO && st_handler.mn_curr_back_conv == 0)
			{
				m_nRunStep[nSite] = 70000;
				break;
			}

			sprintf(st_barcodenet[nSite].m_cMode,			"RETRY");
			sprintf(st_barcodenet[nSite].m_cRetry,			"NO");
			sprintf(st_barcodenet[nSite].m_cCnt,	"%d",	0 );

			m_nRunStep[nSite] = 200;
			break;

		case 110:
			sprintf(st_barcodenet[nSite].m_cRetry,			"YES");
			if( atoi(st_barcodenet[nSite].m_cCnt) + 1 > 3 ) sprintf(st_barcodenet[nSite].m_cCnt,	"%d",	2 );
			else											sprintf(st_barcodenet[nSite].m_cCnt,	"%d",	atoi(st_barcodenet[nSite].m_cCnt) + 1 );
			m_nRunStep[nSite] = 200;
			break;

		case 200:

			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLotid, "%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[nSite] );
			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cBoatid, "%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[nSite] );
			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cSerial,	"%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[nSite] );
			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cAccecode,	"%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_acce_code[nSite] );
			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cTboxcode,	"%s",	st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_tbox_code[nSite] );
// 			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cDencode,	"%s",	g_lotMgr.GetLotAt(0).GetDenLabelCode() );
// 			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode,		g_lotMgr.GetLotAt(0).GetLabelCode());
// 			sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode,		g_lotMgr.GetLotAt(0).GetRibbonCode());
// 			st_handler.m_strLabelCode[nSite] = 	g_lotMgr.GetLotAt(0).GetLabelCode();
// 			st_handler.m_strRibbonCode[nSite] = g_lotMgr.GetLotAt(0).GetRibbonCode();

// 			{
// 				CString str;
// 				if(nSite == 1 ) str = "S1SMNSAFC07052F";
// 				else			str = "S21PNSBFC13419P";
// 				sprintf(st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[nSite], "%s",str);
// 			}

// 			st_mes_data.srt_LOT_ID.Format( "%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_lot_id[nSite] );
// 			st_mes_data.srt_BOAT_ID.Format( "%02d", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].n_idbuff_num[nSite] );
// 			st_mes_data.srt_C_SERIAL.Format( "%s",st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[nSite] );
			//2015.0404
// 			st_mes_data.srt_DTBT_LABEL_CODE.Format( "%s", g_lotMgr.GetLotAt(0).GetLabelCode() );
// 			st_mes_data.srt_DTBT_RIBBON_CODE.Format( "%s", g_lotMgr.GetLotAt(0).GetRibbonCode() );

			//2015.0428
// 			if( g_lotMgr.GetLotCount() <= 0 )
// 			{
// 				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cDencode,	"%s",	g_lotMgr.GetLotAt(0).GetDenLabelCode() );
// 				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode, "%s",	g_lotMgr.GetLotAt(0).GetLabelCode());
// 				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode, "%s",	g_lotMgr.GetLotAt(0).GetRibbonCode());
// 
// 				st_mes_data.srt_DTBT_LABEL_CODE.Format( "%s", g_lotMgr.GetLotAt(0).GetLabelCode() );
// 				st_mes_data.srt_DTBT_RIBBON_CODE.Format( "%s", g_lotMgr.GetLotAt(0).GetRibbonCode() );
// 			}
// 			else
// 			{
// 				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cDencode,	"%s",	g_lotMgr.GetLotByLotID(g_RunNetwork.st_barcodenet[nSite].m_cLotid).GetDenLabelCode() );
// 				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode, "%s",	g_lotMgr.GetLotByLotID(g_RunNetwork.st_barcodenet[nSite].m_cLotid).GetLabelCode());
// 				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode, "%s",	g_lotMgr.GetLotByLotID(g_RunNetwork.st_barcodenet[nSite].m_cLotid).GetRibbonCode());
// 				st_mes_data.srt_DTBT_LABEL_CODE.Format( "%s", g_lotMgr.GetLotByLotID(g_RunNetwork.st_barcodenet[nSite].m_cLotid).GetLabelCode() );
// 				st_mes_data.srt_DTBT_RIBBON_CODE.Format( "%s", g_lotMgr.GetLotByLotID(g_RunNetwork.st_barcodenet[nSite].m_cLotid).GetRibbonCode() );
// 			}
			st_mes_data.srt_RETRY.Format( "%s", st_barcodenet[nSite].m_cRetry );
			st_mes_data.srt_RETRY_CNT.Format( "%s", st_barcodenet[nSite].m_cCnt );

			//2015.0421
			if(nSite == CTL_LEFT)
			{
				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cDencode,	"%s",	st_handler.m_strDensityLabel[st_basic.mn_device_type] );
				if( st_var.n_using_barcode == 0 )
				{
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode,		st_handler.m_strDistributionLabel[0]);
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode,		st_handler.m_strDistributionRibbon[0]);
				}
				else
				{
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode,		st_handler.m_strDistributionLabel[2]);
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode,		st_handler.m_strDistributionRibbon[2]);
				}
				st_mes_data.srt_DTBT_LABEL_CODE.Format( "%s", g_RunNetwork.st_barcodenet[nSite].m_cLabelcode );
				st_mes_data.srt_DTBT_RIBBON_CODE.Format( "%s", g_RunNetwork.st_barcodenet[nSite].m_cRibboncode );
				evMes.OnPutToQueue(MES_BRAND_DTBT_LABEL_ISSUE_L_);
			}
			else
			{
				sprintf( g_RunNetwork.st_barcodenet[nSite].m_cDencode,	"%s",	st_handler.m_strDensityLabel[st_basic.mn_device_type+3] );
				if( st_var.n_using_barcode == 0 )
				{
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode,		st_handler.m_strDistributionLabel[1]);
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode,		st_handler.m_strDistributionRibbon[1]);
				}
				else
				{
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cLabelcode,		st_handler.m_strDistributionLabel[3]);
					sprintf( g_RunNetwork.st_barcodenet[nSite].m_cRibboncode,		st_handler.m_strDistributionRibbon[3]);
				}
				st_mes_data.srt_DTBT_LABEL_CODE.Format( "%s", g_RunNetwork.st_barcodenet[nSite].m_cLabelcode );
				st_mes_data.srt_DTBT_RIBBON_CODE.Format( "%s", g_RunNetwork.st_barcodenet[nSite].m_cRibboncode );
				evMes.OnPutToQueue(MES_BRAND_DTBT_LABEL_ISSUE_R_);
			}
			m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
			m_nRunStep[nSite] = 210;
			break;

		case 210:
			nRet = evMes.OnGet_MesWork();
			// 정상적으로 에코 수신
			if (nRet == REPLY_CORRECT_)
			{
				if ( nSite == CTL_LEFT && (st_client[0].stComm[MES_BRAND_DTBT_LABEL_ISSUE_L_]).nResponse == CTL_GOOD )
				{
					m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
					m_nRunStep[nSite] = 300;
				}
				else if ( nSite == CTL_RIGHT && (st_client[0].stComm[MES_BRAND_DTBT_LABEL_ISSUE_R_]).nResponse == CTL_GOOD )
				{
					m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
					m_nRunStep[nSite] = 300;
				}
				else
				{
					m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
					m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];
					if (m_dwTimeBcrFNetwork[nSite][2] < 0)
					{
						m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
						break;
					}
					
					if (m_dwTimeBcrFNetwork[nSite][2] > (MES_TIME_OUT+1000))
					{
						m_nRetryCnt[nSite]++;	// 재시도 횟수 증가
						if (m_nRetryCnt[nSite] > 2)
						{
							m_nRetryCnt[nSite] = 2;
						}
						m_nRunStep[nSite] = 110;	// 통신 재시도 스텝으로 점프
						
						if (m_nRetryCnt[nSite] >= 2)
						{
							// 통신에 문제 발생 [뾰족한 해결 방법 없음]
							m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
							m_nRunStep[nSite] = 110;	// 통신 재시도 스텝으로 점프
							
							// 900101 1 0 "MES_NOT_RESPONSE_ERR"
							alarm.mstr_code		= "900102";
							CTL_Lib.Alarm_Error_Occurrence(2292, CTL_dWARNING, alarm.mstr_code );
						}
					}
				}
			}
			// 에코 수신에 에러 발생
			else if (nRet == REPLY_ERROR_)
			{
				if ( (nSite == CTL_LEFT && (st_client[0].stComm[MES_BRAND_DTBT_LABEL_ISSUE_L_]).nResponse == CTL_ERROR) ||
					 (nSite == CTL_RIGHT && (st_client[0].stComm[MES_BRAND_DTBT_LABEL_ISSUE_R_]).nResponse == CTL_ERROR))
				{
					m_nRetryCnt[nSite]++;	// 재시도 횟수 증가
					if (m_nRetryCnt[nSite] > 2)
					{
						m_nRetryCnt[nSite] = 2;
					}
					m_nRunStep[nSite] = 110;	// 통신 재시도 스텝으로 점프

					if (m_nRetryCnt[nSite] >= 2)
					{
						// 통신에 문제 발생 [뾰족한 해결 방법 없음]
						m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
						m_nRunStep[nSite] = 110;	// 통신 재시도 스텝으로 점프
						
						// 900102 1 00 "Barcode formt is not correct from server or Server Barcode Error."
						alarm.mstr_code		= "900102";
						CTL_Lib.Alarm_Error_Occurrence(1294, CTL_dWARNING, alarm.mstr_code );
					}
				}
				else
				{
					m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
					m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];
					if (m_dwTimeBcrFNetwork[nSite][2] < 0)
					{
						m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
						break;
					}
					
					if (m_dwTimeBcrFNetwork[nSite][2] > (MES_TIME_OUT+1000))
					{
						m_nRetryCnt[nSite]++;	// 재시도 횟수 증가
						if (m_nRetryCnt[nSite] > 2)
						{
							m_nRetryCnt[nSite] = 2;
						}
						m_nRunStep[nSite] = 110;	// 통신 재시도 스텝으로 점프
						
						if (m_nRetryCnt[nSite] >= 2)
						{
							// 통신에 문제 발생 [뾰족한 해결 방법 없음]
							m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
							m_nRunStep[nSite] = 110;	// 통신 재시도 스텝으로 점프
							
							// 900101 1 0 "MES_NOT_RESPONSE_ERR"
							alarm.mstr_code		= "900102";
							CTL_Lib.Alarm_Error_Occurrence(1295, CTL_dWARNING, alarm.mstr_code );
						}
					}
				}
			}
			else
			{
				m_dwTimeBcrFNetwork[nSite][1] = GetCurrentTime();
				m_dwTimeBcrFNetwork[nSite][2] = m_dwTimeBcrFNetwork[nSite][1] - m_dwTimeBcrFNetwork[nSite][0];
				if (m_dwTimeBcrFNetwork[nSite][2] <= 0)
				{
					m_dwTimeBcrFNetwork[nSite][0] = GetCurrentTime();
					break;
				}
				
				if (m_dwTimeBcrFNetwork[nSite][2] > (MES_TIME_OUT+1000))
				{
					m_nRetryCnt[nSite]++;	// 재시도 횟수 증가
					if (m_nRetryCnt[nSite] > 2)
					{
						m_nRetryCnt[nSite] = 2;
					}
					m_nRunStep[nSite] = 200;	// 통신 재시도 스텝으로 점프
					
					if (m_nRetryCnt[nSite] >= 2)
					{
						// 통신에 문제 발생 [뾰족한 해결 방법 없음]
						m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
						m_nRunStep[nSite] = 200;	// 통신 재시도 스텝으로 점프
						
						// 900101 1 0 "MES_NOT_RESPONSE_ERR"
						alarm.mstr_code		= "900102";
						CTL_Lib.Alarm_Error_Occurrence(1296, CTL_dWARNING, alarm.mstr_code );
					}
				}
			}
			break;

		case 300:
			{
				m_nRetryCnt[nSite] = 0;	// [리셋] 작업 재시도 횟수
 				//m_nRunStep[nSite] = 70000;
				if( Func.CmpStrFile( evMes.m_strSerial[nSite], g_RunNetwork.st_barcodenet[nSite].m_cSerial) )
				{
					//nSite = 0;
					m_nRunStep[nSite] = 70000;
				}
				else
				{
					m_nRunStep[nSite] = 200;
				}
			}
			break;

		case 70000:
			//해당 Feeder에 바코드출력을 한다.
		
			if( st_basic.mn_mode_run == 0 )
				Func.m_pBctFmtClient[nSite]->SetBcrFormat( Func.GetPrintFormat() );//ybs 2014,1104	
			else
			{
				if( evMes.m_strBcrFormat[nSite].GetLength() < 10)
				{
					Func.m_pBctFmtClient[nSite]->SetBcrFormat(Func.GetBcrPrintFormat( 1, evMes.m_strSerial[nSite] ) );
					m_nRunStep[nSite] = 200;
					break;					
				}
				else
				{
					Func.m_pBctFmtClient[nSite]->SetBcrFormat( evMes.GetBcrFormat(nSite) );
				}
			}

			
 			st_barcodenet[nSite].nComplete	= CTL_YES; // label 정보 수신 완료

			if( st_handler.mn_Lotcnt < 0) st_handler.mn_Lotcnt = 0;
			//2014.0104
// 			if( st_basic.nBcrFeederUse[nSite] == CTL_YES )
// 			{
// 				g_Run_Barcode_Feeders.Barcode_Received( st_handler.mn_Lotcnt,  nSite, g_RunNetwork.st_barcodenet[nSite].m_cSerial, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
// 			}
// 			else
// 			{
// 				g_Run_Barcode_Feeders.Barcode_Received2( st_handler.mn_Lotcnt,  nSite, g_RunNetwork.st_barcodenet[nSite].m_cSerial, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
// 			}
			//2015.327
			if( st_var.n_using_barcode == 0 )
			{
				g_Run_Barcode_Feeders.Barcode_Received( st_handler.mn_Lotcnt,  nSite, g_RunNetwork.st_barcodenet[nSite].m_cSerial, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
			}
			else
			{
				g_Run_Barcode_Feeders.Barcode_Received2( st_handler.mn_Lotcnt,  nSite, g_RunNetwork.st_barcodenet[nSite].m_cSerial, Func.m_pBctFmtClient[nSite]->GetBcrFormat());
			}

// 			st_barcodenet[nSite].nComplete	= CTL_YES; // label 정보 수신 완료

//  			if( st_barcodenet[0].nComplete == CTL_YES && st_barcodenet[1].nComplete == CTL_YES)			
//  				st_handler.mn_Lotcnt++;
			
 			m_nRunStep[nSite] = 70100;
			break;

		case  70100:
// 			//0302
			if( st_barcodenet[0].nComplete == CTL_YES && st_barcodenet[1].nComplete == CTL_YES)	
			{
				st_handler.m_strRPropEnd = "YES";
				/*st_handler.mn_Lotcnt++;*/
				m_nRunStep[nSite] = 700200;
			}
			break;

		case 700200:
// 			if (st_barcodenet[nSite].nComplete	!= CTL_NO) break;
// 			m_nRunStep[0] = 0;
// 			m_nRunStep[1] = 0;

			if (st_barcodenet[nSite].nComplete	!= CTL_NO) break;
			m_nRunStep[nSite] = 0;
			break;
	}
}

void CRunNetwork::Thread_Run()
{

}


void CRunNetwork::Thread_Variable_Initial()
{

}