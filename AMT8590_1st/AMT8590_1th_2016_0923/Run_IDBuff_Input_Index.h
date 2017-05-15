// Run_IDBuff_Input_Index.h: interface for the CRun_IDBuff_Input_Index class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_IDBUFF_INPUT_INDEX_H__35512BEF_D631_4415_87FE_81B80C08D7A5__INCLUDED_)
#define AFX_RUN_IDBUFF_INPUT_INDEX_H__35512BEF_D631_4415_87FE_81B80C08D7A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBasicData.h"

class CRun_IDBuff_Input_Index  
{
public:
	int mn_InitStep;
	int mn_run_step; 
	int mn_pick_step;
	int mn_place_step;
	int mn_bcrIn_step;
	int mn_bcrOut_step;
	int mn_sever_step;
	int mn_fmt_step;
	int mn_ccs_step;
	int mn_comm_pos;
	int	m_nStep_MES;
	int mn_mdl_change_step;

	int mn_left;
	int mn_right;
	int nMdl;
	
	long ml_barcode_limit_time[2][3];
	int mn_barcode_retry_cnt[2];
	
	char mc_jamcode[10];
	int mn_retry_cnt;
	int m_nRetryCnt;
	int m_nRetryTest;
	
	DWORD m_dwTimeBcrFNetwork[3];

	long ml_idbuff_input_clamp_wait[3];
	bool mb_idbuff_input_clamp_flag;

	CString m_strRetry;
	CString m_strCnt;
	int m_nidRetryCnt;
	int m_nErrRetryCnt;
	int mn_pos;
	DWORD m_dwTime_Network[3];

	DWORD m_lWaitResp[3];
	int m_nRetryCount;

	CString m_strModelNewname;
	DWORD m_dwTime_sendbcrstatus;

	int set_idbuffer_input_clamp_onoff(int n_site, int n_ionum, int n_onoff);
	int get_idbuffer_input_clamp_onoff(int n_site, int n_ionum, int n_onoff);

	long ml_idbuff_input_length_wait[3];
	bool mb_idbuff_input_length_flag;
	int set_idbuffer_input_length_onoff(int n_site, int n_ionum, int n_onoff);
	int get_idbuffer_input_length_onoff(int n_site, int n_ionum, int n_onoff);

	
	int idbuffermove_pick_process(int n_site); //8580에서 집어오기 
	int idbuffermove_place_process(int n_site); //8590에 놓기 
	
	int barcode_read_inout_process(int n_site);
	int OnServerNewLotin();
	int OngetFmtfromServer();
	int OnServerCCS();
	int OnServerBaotIDIn();
	void Run_Model_Change();
	
	void Run_Move();
	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();

	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스

	CRun_IDBuff_Input_Index();
	virtual ~CRun_IDBuff_Input_Index();

};
extern CRun_IDBuff_Input_Index g_Run_IDBuff_Input_Index;

#endif // !defined(AFX_RUN_IDBUFF_INPUT_INDEX_H__35512BEF_D631_4415_87FE_81B80C08D7A5__INCLUDED_)
