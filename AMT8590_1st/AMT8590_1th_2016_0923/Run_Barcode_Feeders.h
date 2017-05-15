#if !defined(AFX_RUN_BARCODE_FEEDERS_H__B5D48698_46E0_49CF_8BF4_11EFE8A5DE2C__INCLUDED_)
#define AFX_RUN_BARCODE_FEEDERS_H__B5D48698_46E0_49CF_8BF4_11EFE8A5DE2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Barcode_Feeders.h : header file
//

enum ENUM_BARCODE_FEEDER_INIT
{
	EBFI_START,
	EBFI_CHECK_ROBOT_CHK,
	EBFI_HOMECHK_VISION_X,
	EBFI_MOVE_SAFETY_X,
	EBFI_FEEDER_UNLOCK,
	EBFI_FEEDER_UNLOCK_CHK,
	EBFI_FEEDER_LOCK,
	EBFI_FEEDER_LOCK_CHK,

	EBFI_PLATE_DOWN,
	EBFI_PLATE_DOWN_CHK,
	EBFI_HOMECHK_FEERDE1,
	EBFI_HOMECHK_FEERDE2,
	EBFI_HOMECHK_FEERDE3,
	EBFI_HOMECHK_FEERDE4,
	EBFI_REJECT_PICK_READY_UP,
	EBFI_REJECT_PICK_READY_UP_CHK,



	EBFI_FEEDER_VACUUM_ON,
	EBFI_BARCODE_FEEDER_EMPTY_CHK,
	EBFI_FEEDER_VACUUM_OFF,
	EBFI_FEEDER_EJECT_ON,
	EBFI_FEEDER_EJECT_OFF,
	
	EBFI_REJECT_VACUUM_ON,//2014,1218 ysj
	EBFI_REJECT_EMPTY_CHK,//2014,1218 ysj
	EBFI_REJECT_VACUUM_OFF,//2014,1218 ysj
	EBFI_REJECT_CYLINDER_UP,//2014,1218 ysj
	EBFI_HOMECHK_REJECT_X,//2014,1218 ysj

	EBFI_FEEDER1_REJECT,
	EBFI_FEEDER_READY,

	EBFI_FINISH,
};

enum ENUM_BARCODE_POS
{
	EBP1=0,
	EBP2,
	EBP3,
	EBP4,
	MAX_EBP,
};

enum EPRI
{
	EPRI_NONE,
	EPRI_FORWARD,
	EPRI_BACKWARD,
};

enum ENUM_STEP_FEEDER
{
	ESTEP_FEEDER_NONE,
	ESTEP_FEEDER_READY=10,
	ESTEP_FEEDER_READY_DOWN=20,
	ESTEP_FEEDER_READY_DOWN_CHK=30,

	ESTEP_FEEDER_PICK=40,
	ESTEP_FEEDER_UP=50,
	ESTEP_FEEDER_UP_CHK=60,
	ESTEP_FEEDER_VISPOS=70,
	ESTEP_FEEDER_DOWN=80,
	ESTEP_FEEDER_DOWN_CHK=90,
	ESTEP_FEEDER_UNLOAD_VISION=100,

	ESTEP_COMM_CONNECT=110,
	ESTEP_COMM_CONNECT_CHK=120,
	ESTEP_CCOMM_CONNECT_WAIT=130,
	ESTEP_COMM_INSP=140,
	ESTEP_COMM_INSP_COM=150,
	ESTEP_COMM_INSP_COM_WAIT=160,

	ESTEP_FEEDER_VISCHK=170,//VISION
	ESTEP_FEEDER_UNLOAD_WASTE_AFTER_LOAD = 175,
	ESTEP_FEEDER_UNLOAD_WASTE=180,
	ESTEP_FEEDER_UNLOAD=190,
	ESTEP_FEEDER_UNLOAD_WASTE_PICKUP=200,
	ESTEP_FEEDER_UNLOAD_WASTE_FINISH=210,
	ESTEP_FEEDER_FINISH=220,
};

enum EFFB
{
	FFB_NONE,
	FFB_FORWARD,
	FFB_BACKWARD,
};

enum EREJ
{
	FREJ_NONE,
	FREJ_REJECT,
};

enum ENUM_RETRY_COUNT
{
	ERC_BCR1 = 0,
	ERC_BCR2,
	ERC_BCR3,
	ERC_BCR4,
	ERC_REJECT,
};


enum PICKER_STEP_REJECT
{
	PSTEP_REJECT_NONE=0,
		PSTEP_REJECT_PICK_BEFORE,
		PSTEP_REJECT_PICK_READY=10,
		PSTEP_REJECT_PICK_READY_UP=20,
		PSTEP_REJECT_PICK_READY_UP_CHK=30,
		PSTEP_REJECT_PICK_START=40,
		PSTEP_REJECT_RETRY_PICK_UP=50,
		PSTEP_REJECT_RETRY_PICK_UP_CHK=60,
		PSTEP_REJECT_PICK_DOWN=70,
		PSTEP_REJECT_PICK_DOWN_CHK=80,
		PSTEP_REJECT_PICK_VACUUM=90,
		PSTEP_REJECT_PICK_VACUUM_CHK=100,
		PSTEP_REJECT_PICK_UP=110,
		PSTEP_REJECT_PICK_UP_CHK=120,
		PSTEP_REJECT_PICK_UP_LABEL_CHK=130,
		PSTEP_REJECT_PICK_UP_FINISH=140,
		
		PSTEP_REJECT_PLACE_BEFORE=150,
		PSTEP_REJECT_PLACE_READY=160,
		PSTEP_REJECT_PLACE_READY_DN=170,
		PSTEP_REJECT_PLACE_READY_DN_CHK=180,
		
		PSTEP_REJECT_PLACE_READY_UP=190,
		PSTEP_REJECT_PLACE_READY_UP_CHK=200,	
		PSTEP_REJECT_PLACE_START=210,
		PSTEP_REJECT_PLACE_DOWN=220,
		PSTEP_REJECT_PLACE_DOWN_CHK=230,
		PSTEP_REJECT_PLACE_UP=240,
		PSTEP_REJECT_PLACE_UP_CHK=250,
		PSTEP_REJECT_PLACE_VACUUM=260,
		PSTEP_REJECT_PLACE_VACUUM_CHK=270,
		PSTEP_REJECT_PLACE_FINISH=280,
		PSTEP_REJECT_LABLE_FULL_CHK=290,
		PSTEP_REJECT_LABLE_ONESTEP_MOVE_CHK=300,
};

enum ENUM_BCR_PRINTER_SITE
{
	EBPS_1 = 0,
	EBPS_2,
	EBPS_3,
	EBPS_4,
};


//class ABcrPrint
//{
//public:
//	ABcrPrint() { EraseData(); }
//	virtual ~ABcrPrint() {}
//
//	// Get
//	CString GetSerial() { return m_strSerial; }
//	CString GetLotNo() { return m_strLotNo; }
//	bool IsExist() { return (m_strSerial != ""); }
//	bool IsReject() { return m_bReject; }
//	
//	void SetSerial( CString strSerial ) { m_strSerial = strSerial;}
//	void SetReject( bool bReject) { m_bReject = bReject; }
//	void EraseData() { m_strSerial = ""; m_strLotNo = ""; m_bReject = false; }
//
//protected:
//	CString m_strSerial;
//	CString m_strLotNo;
//	bool m_bReject;
//
//};


//class AFeederPicker
//{
//public:
//	AFeederPicker()
//	{
//		m_nTargetPos = -1;
//		m_nPickPos = -1;
//	}
//
//	virtual ~AFeederPicker(){}
//
//	bool IsExist(){
//		return false;
//	}
//
//	ABcrLabel & GetBcrLabel() { return m_mdl; }
//	void SetBcrLabel( ABcrLabel mdl) { m_mdl = mdl; }
//
//protected:
//	ABcrLabel	m_mdl;
//
//	int m_nTargetPos;
//	int m_nPickPos;
//};

struct stBcrMsg
{	
	int iBUF;
	int iID;
	CString strfmt;
	CString strbcr;
};

class ARejPicker
{
public:
	ARejPicker()
	{
		m_nTargetPos = -1;
		m_nPickPos = -1;
	}
	
	virtual ~ARejPicker(){}

	ABcrLabel & GetBcrLabel() { return m_mdl; }
	void SetBcrLabel( ABcrLabel mdl) { m_mdl = mdl; }

protected:
	ABcrLabel m_mdl;
	//ABcrPrint	m_mdl[4];
//	AFeederPicker m_feeder;

	int m_nTargetPos;
	int m_nPickPos;
};


struct stRejectPicker
{
	int iRobotFirst;
	int iPosFirst;

	PICKER_STEP_REJECT rej_picker_step;
	stRejectPicker()
	{
		iRobotFirst = -1;
		rej_picker_step = PSTEP_REJECT_NONE;
	}
	void Set( int _iRobotFirst, int _iPosFirst )
	{
		iRobotFirst = _iRobotFirst;
		iPosFirst = _iPosFirst;
		rej_picker_step = PSTEP_REJECT_PICK_READY;
	};
	
	ENUM_LABEL_POSITION GetPosByTarget()
	{
		if	   ( iPosFirst < MPOS_INDEX_REJECT )			return MPOS_INDEX_PMACHINE;
		else												return MPOS_INDEX_REJECT;
	}
};

class AFeedertRobot  
{
public:
	AFeedertRobot();
	virtual ~AFeedertRobot();

	bool IsPicking()
	{
		if( m_Picker.GetBcrLabel().IsExist() )
			return true;
		return false;
	}
	int GetJobCount() { return m_vecJob.size(); }
	void AddJob( int iRobotFirst, int iPosFirst );
	stRejectPicker& GetFirstJob()
	{
		if( m_vecJob.size() > 0 )
			return m_vecJob.at(0);

		return m_dummyJob;
	}
	stRejectPicker& GetJob( int iIdx )
	{
		if( m_vecJob.size() > 0 )
			return m_vecJob.at(0);

		return m_dummyJob;
	}

	void EraseFirstJob()
	{
		if( m_vecJob.size() > 0 )
			m_vecJob.erase( m_vecJob.begin() );
	}

	ARejPicker& GetPicker() { return m_Picker; }
//	AFeederPicker& GetFeeder( int iIdx ) { return m_feedPicker; }


protected:
	ARejPicker	m_Picker;
//	AFeederPicker m_feedPicker;

	std::vector<stRejectPicker>	m_vecJob;
	stRejectPicker					m_dummyJob;
};

extern AFeedertRobot g_feeder_robot;
/////////////////////////////////////////////////////////////////////////////
// CRun_Barcode_Feeders command target

class CRun_Barcode_Feeders : public CObject
{
public:
	DECLARE_SERIAL(CRun_Barcode_Feeders)
	CRun_Barcode_Feeders();           // protected constructor used by dynamic creation
	virtual ~CRun_Barcode_Feeders();

// Attributes
public:
	int		m_nRetryCnt[4][4];
	int		m_nBcrFailRetryCnt[4];
	int		mn_InitStep;
	int		mn_RunStep1;
	int		mn_RunStep2;
	int		mn_RestoreStep;
	int		rej_picker_step_old;
	int		mn_Bcr1CommStep;
	int		mn_RejectStep;
	int		mn_buff_place[2];
	int		mn_init_rej;
	int		mn_init_pos;
	int		mn_fmt_step;//2014,1222
	int		mn_comm_pos;
	int		mn_step_vis;
	int		mn_iBUF;
	int		mn_iBUF2;
	int		mn_vision_err_retry[4];

	int		feeder1_step_old;
	int		feeder1_step;
	int		m_nRun_Print[4];
	int		m_nRejectConinueErr;
	int		m_nRejectConvey;
	int		mn_MotFlag[2];
	int		mn_Mot_Rej[2];
	int		n_bcr_vision_req[4];
	char	mc_jamcode[10];

	bool	m_bPrint_Pickup[4];

	CString strJamCode;


	DWORD	m_dwTimeInit;
	DWORD	m_dwTime_Robot;
	DWORD	m_dwTime_Netbcr;

	bool	mb_BcrComplete[4];
	bool	mb_VisComplete[4];
	bool	mb_VisGoodNG[4];

	DWORD	m_dwBcrOutTimeWait[4][3];
	DWORD	m_dwTimeWait[4][3];
	DWORD	m_dwTimeMove1[2];//12
	DWORD	m_dwTimeMove2[2];//23
	DWORD	m_dwTimerunMove1;
	DWORD	m_dwTimerunMove2;


	DWORD	m_dwTimeReject;
	DWORD	m_dwTimeRejectConvey;
	DWORD	m_dwTimerFeeder[4];
	DWORD	m_dwTimeVision;

//	ABcrPrint	m_bcrPrint[4];;

	DWORD	m_dwTimeNetwork[4][3];
	DWORD	m_dwTimeBcrFNetwork[4][3];

	int m_continus_vis_error[4];//2015.0211
	int m_total_vis_error[4];//2015.0211

	void	 SetPrintResult(int nSite, int nResult) { m_resultFeeder[nSite] = nResult; }
	int		 GetPrintResult(int nSite) { return m_resultFeeder[nSite]; }
	void	 SetPrintSerial(int nSite, CString strserial) { m_strbcrSerial[nSite] = strserial; }
	CString	 GetPrintSerial(int nSite) { return m_strbcrSerial[nSite]; }

	CString  m_strCmpSerial[4];

	CString  m_strBcrcode[4];

	
	bool SetVariable(int pos, int nPos, CString mode="NEW", CString strRetry="NO", CString strCnt="0");

// Operations
protected:
	ENUM_STEP_CYL	 m_step_bcrlock[4];
	EFB				 m_fbBcrlock[4];
	DWORD			 m_dwTimeBcrlockStart[4];
	DWORD			 m_dwTimeBcrlock[4];

	ENUM_STEP_FEEDER m_step_bcrfeeder[4];
	EPRI			 m_fbFeeder[4];
	DWORD			 m_dwTimeFeederStart[4];
	DWORD			 m_dwTimeFeeder[4];
	int				 m_resultFeeder[4];
	CString			 m_strbcrSerial[4];

	ENUM_STEP_CYL	 m_step_plateupdn[4];
	EFB				 m_fbplateupdn[4];
	DWORD			 m_dwTimePlaterUpDnStart[4];
	DWORD			 m_dwTimePlaterUpDn[4];

	PICKER_STEP_REJECT  m_step_rejectupdn;
	EFB					m_fbrejectupdn;
	DWORD				m_dwTimeRejectUpDnStart;
	DWORD				m_dwTimeRejectUpDn;

	int				 m_nRejRetryCnt;
	int				 m_nRejRetryUpDnCnt;

	int				 m_nReteyCnt[5][2];//0:dowm 1:up


	ENUM_COMM_STEP	 m_step_bcrcomm[4];
	ECW				 m_cwBcrcomm[4];
	DWORD			 m_dwTimeCommBcrStart[4];
	DWORD			 m_dwTimeCommBcr[4];

	ENUM_BCROUT_STEP m_stepBcrStstus[4];
	EBPO			 m_spoBcrStatus[4];
	DWORD			 m_dwTimeStstusBcrStart[4];
	DWORD			 m_dwTimeStstusBcr[4];

	CString			 m_site_Serial[4];
	CString			 m_site_Fromat[4];

	ENUM_BCROUT_STEP	m_step_bcrout[4];
	EBPO			 m_spoBcrout[4];
	DWORD			 m_dwTimeBcrOutStart[4];
	DWORD			 m_dwTimeBcrOut[4];

	ENUM_STEP_CYL	m_step_Rejvac_down;
	EFB				m_fbRejVacDown;
	DWORD			m_dwTimeRejVacDownStart;
	DWORD			m_dwTimeRejVacDown;

	//2014,1222
	std::vector<stBcrMsg>	m_vecbcrRev; 
	std::vector<stBcrMsg>	m_vecbcrRev2; 
	CString					m_strRecv_Buf;
	int						m_iStep_Receive;
	DWORD					m_dwRecvWaitTime;

	int				m_nReject_pick;


public:
	void ReqBcrLock( int nSite, EFB efb );
	EFB GetBcrLock( int nSite) { return m_fbBcrlock[nSite]; }
	void SetBcrLock(int nSite, int iOnOff );
	bool CheckBcrLock(int nSite, int iOnOff );

	void ReqBcrWork( int nSite, EPRI esf );
	EPRI GetBcrWork( int nSite) { return m_fbFeeder[nSite]; }
	void SetBcrWorkk(int nSite, int iOnOff );
	bool CheckBcrWork(int nSite, int iOnOff );

	void ReqPlateUpDn( int nSite, EFB efb );
	EFB	 GetPlateUpDn( int nSite ) { return m_fbplateupdn[nSite];	}
	void SetPlateUpDn( int nSite, int iOnOff );
	bool CheckPlateUpDn( int nSite, int iOnOff );

	void ReqRejVacDown( EFB efb );
	EFB GetRejVacDown() { return m_fbRejVacDown; }

	void Run_Move_Rej_Vac_Down();


	void CommBarcode( int nSite, CString strSreial);
	void Run_Move_Bcrcomm( int nSite );

	void ReqBcrOutPrint( int nSite, CString strfmtPrint);
	void Run_Move_Bcr_out( int nSite );

 

	void EBFI_Check_Rbt_Chk();
	void EBFI_HomeCheck_Vision_X();
	void EBFI_MoveSafety_X();
	void EBFI_BcrFeeder_Lock(  EFB efb );
	void EBFI_BcrFeeder_Lock_chk( EFB efb  );

	void EBFI_HomeCheck_BCR_Feeder( int nAxis );
	void EBFI_Plate_Down();
	void EBFI_Plate_Down_Check();
	void EBFI_Barcode_Plater_Vacuum_OnOff( int iOnOff );
	void EBFI_Feeder_EjectOnOff( int iOnOff );
	void EBFI_Finish();
	void EBFI_Barcode_Plater_Vacuum_OnOff_Check();

	void EBFI_HomeCheck_Reject_X( int nAxis );//2014,1220
	int EBRI_Bcr_For_Reject( int nFeeder );


	int GetJobCount() { return m_vecJob.size(); }
	bool Run_Rej_Move_Chk_Jam(stRejectPicker job);
	bool Run_Rej_Move_Chk_Time();
	bool InTimeRobot( int iMilliSec );

	void SetRetryCnt(int nSite, int iOnOff, int nCnt);
	int	GetRetryCnt(int nSite, int iOnOff) { return m_nReteyCnt[nSite][iOnOff]; }
	void AddRetryCnt(int nSite, int iOnOff) { m_nReteyCnt[nSite][iOnOff]++; }
	void ControlRejectRobot();
	
	bool Run_Move1_Chk_Jam( int feeder1step);
	bool Run_Move1_Chk_Time();
	bool Run_Move2_Chk_Jam( int feeder2step);
	bool Run_Move2_Chk_Time();

public:
	void Run_Move( int nSite);
	void Run_Move2( int nSite);

	void Run_Initial();
	void Thread_Run();
	void Thread_Variable_Initial();
	void OnRejectConveyMove();


	void Run_Move_Feeder( int nSite);//바코드 출력
	void Run_Move_Reject();//버린다.
	void Run_Move_Feeder_Lock(int nSite);
	void Run_Move_Bcr_Plate_UpDown( int nSite );

	//2014,1222
	void Run_Receive();
	void Move_Barcode_skip(int nFromSite);
	void Barcode_Received( int nLot, int nPos, CString strbcr, CString strBcrFormat );
	void Barcode_Received2( int nLot, int nPos, CString strbcr, CString strBcrFormat );
	int OnGetFmtfromServer();
	void Run_Move_Vision();

	// jtkim print 상태체크하기 위한 변수 초기
	void ReqBcrStatusPrint(int nSite);
	void Run_Move_Print_Status(int nSite); // jtkim
	
	std::vector<stRejectPicker>	m_vecJob;

	CString m_strBcrFormat[4]; 
	CString m_strcmp_bcrSerial[4];

};
extern CRun_Barcode_Feeders g_Run_Barcode_Feeders;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_BARCODE_FEEDERS_H__B5D48698_46E0_49CF_8BF4_11EFE8A5DE2C__INCLUDED_)
