// Run_IDBuff_Move_Index.h: interface for the CRun_IDBuff_Move_Index class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_IDBUFF_MOVE_INDEX_H__30F09508_409A_4EB8_9DAF_2C6B34BCA5E8__INCLUDED_)
#define AFX_RUN_IDBUFF_MOVE_INDEX_H__30F09508_409A_4EB8_9DAF_2C6B34BCA5E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRun_IDBuff_Move_Index  
{
public:
	long ml_until_wait[3];
	int mn_work_site_num;

	int mn_fixclamp_sol_flag[4];
	int mn_barcode_retry_cnt[2];
	long ml_barcode_limit_time[2][3];
	
	char mc_jamcode[10];

	int mn_InitStep;
	int mn_run_step; 
	int mn_density_step;
	int mn_pick_left_step;
	int mn_place_right_step;
	int mn_bcrIn_step;
	int mn_bcrOut_step;

	long ml_idbuff_index_length_wait[3];
	bool mb_idbuff_index_length_flag;


	bool mb_fix_clamp_select_flag;
	long ml_fix_clamp_select_wait[3];

	bool mb_fix_clamp1_2_flag;
	long ml_fix_clamp1_2_wait[3];

	long ml_index_gripper_wait[3][3];
	bool mb_index_grippe_flag[3];

	int mn_bcr_flag[2];



	int set_moveindex_gripper_onoff(int n_site, int n_ionum, int n_onoff);
	int get_moveindex_gripper_onoff(int n_site, int n_ionum, int n_onoff);

	int set_moveindex_gripper_length_onoff(int n_site, int n_ionum, int n_onoff);
	int get_moveindex_gripper_length_onoff(int n_site, int n_ionum, int n_onoff);

	
	int set_idbuff_fix_clamp1_2_onoff(int n_site, int n_onoff);
	int get_idbuff_fix_clamp1_2_onoff(int n_site, int n_onoff);

	int set_idbuff_fix_clamp_select_onoff(int n_site[4], int n_onoff);
	int get_idbuff_fix_clamp_select_onoff(int n_site[4], int n_onoff);

	int idbuffer_move_index_left_input_process(int n_site);
	int idbuffer_move_index_right_output_process(int n_site);

	void Run_Move();
	void Run_Move_Density();
	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();
	int barcode_read_inout_process(int n_site);

	CRun_IDBuff_Move_Index();
	virtual ~CRun_IDBuff_Move_Index();

};
extern CRun_IDBuff_Move_Index g_Run_IDBuff_Move_Index;

#endif // !defined(AFX_RUN_IDBUFF_MOVE_INDEX_H__30F09508_409A_4EB8_9DAF_2C6B34BCA5E8__INCLUDED_)
