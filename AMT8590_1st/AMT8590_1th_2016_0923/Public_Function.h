// Public_Function.h: interface for the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_)
#define AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"

#include "Variable.h"		// 전역 변수 정의 클래스
#include <wininet.h>
#include <string>
#include <vector>
#include "Ctlbd_Variable.h"	// Added by ClassView
#include "SrcPart/APartTray.h"
#include "SrcPart/APartTestSite.h"
#include "SrcPart/APartRobot.h"

#include "BarcodeFormat.h"

class CMainFrame;
class CDialog_Part_IO;
class CDialog_Motor_Part;
class CDialog_Manual_Part_1;

class CZebraPrint;
class CVisionClient;
class CVisionServer;
//class AClient_Comm_EC;
//class CBarcodeFormatClient;

class CPublic_Function    : public CObject
{
public:
	DECLARE_SERIAL(CPublic_Function);
	CPublic_Function();
	virtual ~CPublic_Function();

public:
	CVisionClient			*m_pVsClient[4];
	CVisionServer			*m_pVsServer[4];
	CZebraPrint				*m_pZebra[4];
	CBarcodeFormatClient	*m_pBctFmtClient[4];

	APartTestSite			*m_pSite[20];
	APartRobot				*m_pRobot[10];
	
	
	CDialog_Part_IO			*m_p_io;
	CDialog_Motor_Part		*m_p_motor_part;
	CDialog_Manual_Part_1	*mp_manual_1;

	bool CYLFlag[MAX_IO];
	long CYLTime[MAX_IO][3];
	bool SiteFlag[MAX_IO];
	long SiteTime[MAX_IO][3];
	bool IOFlag[MAX_IO];
	long IOTime[MAX_IO][3];
	int Mot_HomeStep[MOTOR_COUNT];
	int Mot_RetryCount;
	int Mot_CurTime[MOTOR_COUNT][3];
	int Mot_RetryFlag[MOTOR_COUNT];

	int reinstatement_Step;

	///////20131125
	int barcode_reading_step;
	DWORD dwBarcodeReadTime;
	int nBarcodeRetryCnt;
	int n_barcode_row;
	int n_barcode_col;
	///////

	long backuptime[3];
	int mn_check[2];
	int mn_stamp_change;
	
	CString strFile;
	int nGetPCBInfoStep;
	
	HINTERNET m_hOpen;
	HINTERNET m_hConnect;
	
	long NetworkConnectTime[3];
	long NetworkingTime[3];
	long TesterCommTime[3];
	char cJamcode[10];

	CString strFTPFileName[100];
	int nFTPFileCheck;
	int nCheckTemp;

	CString strQuery;			//쿼리문을 담을 변수 

	CString Itemsortnum;		//정렬조건을 담을 문자열 변수
	
	CString str_O_Pulse, str_N_Pulse;

	int mn_front_change;
	int mn_rear_change;

	int m_nLastYear;
	int m_nLastMonth;
	int m_nLastDay;
	int m_nLastHour;
	int n_bcrlot_step;
	long ml_untilwait_time[3];

	int nItemLength;

	CMainFrame						*pMainFrm;
	
	CFont* mp_main_font;			// 폰트 정보 설정 변수
	CFont* mp_main_big_font;		// 폰트 정보 설정 변수
	CFont* mp_motor_font;
	CFont* mp_motorspeed_font;				// 폰트 정보 설정 변수
	CFont* mp_motoraxis1_font;
	CFont* mp_motorpart_font;
	CFont* mp_iopart_font;
	CFont* mp_partmanual_font;
	CFont* mp_fileEditer_font;
	CFont* mp_pgminfo_font;
	CFont* mp_alarm_font;  // 폰트 설정 변수 선언 
	CFont* mp_listdataview_font;  // 폰트 설정 변수 선언 
	CFont* mp_buffer_font;  // 폰트 설정 변수 선언 
	CFont* mp_exit_font;
	CFont* mp_info_font;
	CFont* mp_board_font;
	CFont* mp_pad_font;						// 폰트 설정 변수 선언 
	CFont* mp_list_font;			// 폰트 설정 변수 선언 
	CFont* mp_msg_font;  // 폰트 설정 변수 선언 
	CFont* mp_change_font;							// 폰트 설정 변수 선언 
	CFont* mp_pass_font;							// 폰트 설정 변수 선언 
	CFont* mp_select_font;					// 폰트 설정 변수 선언 
	CFont* mp_title_font;  // 폰트 설정 변수 선언 
	CFont* mp_menu_font;			// 폰트 설정 변수 선언 
	CFont* mp_m_msg_font;  // 폰트 설정 변수 선언 
	CFont* mp_io_font;			// 폰트 정보 설정 변수
	CFont* mp_init_font;  // 폰트 설정 변수 선언 
	CFont* mp_lock_font;  // 폰트 설정 변수 선언 
	CFont* mp_wait_time_font;		// 폰트 정보 설정 변수
	CFont* mp_maintenance_font;			// 폰트 정보 설정 변수
	CFont* mp_combo_font;
	CFont* mp_basic_font;			// 폰트 정보 설정 변수
	CFont* mp_operation_list_font;			// 폰트 설정 변수 선언 
	CFont* mp_admin_font;			// 폰트 정보 설정 변수

public:  
	//////
	int Handler_Recovery_Data(int n_debug, int n_site, int n_mode); //2014.1002 	
	int variable_data_initialize(int n_mode);
	//////


	void DB_Write_Boat(st_boat_database st_boatdb);
	void Handler_Recovery_Data_Write();
	void GlobalDataSerialize(CArchive &ar);
	void OnDayDataReset();
	void OnDailyDataSave();
	void OnLotDataSave();
	void DateTimeChange(CString sDateTime);
	char GetNtPrivilege();
	void SetActionParam();
	void DB_Write_Buffer(st_buffer_database st_bufferdb);
	void DB_Write_Ground(st_ground_database st_grounddb);
	void DB_Write_Alarm(st_alarm_database st_alarmdb);

	void TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp);
	void ThreadFunctionStepTrace(int nPos, int nStep);
	int Robot_Reinstatement_Position(int mode);

	int Barcode_Reading_Start(int n_barcode_col, bool UpDown=false);//20131125


	CString GetLocalMacAddress();
	int FileSizeCheck(CString FileName, long size, int n_check);
	CString OnDeviceInfoAnalysis(CString strBody, int nInfo);
	CString OnNetworkDataMake(int nID);
	CString OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt = 0);
	int OnNetworkDataAnalysis(CString strBody);
	void OnSet_IO_Port_Sound(int n_status, int n_opt = 0);
	void OnSet_IO_Port_Alarm();
	int Handler_Recovery_Data(int n_Mode);
	void OnSet_IO_Port_Init();
	void OnSet_IO_Port_Run();
	void OnSet_IO_Port_Stop();
	
	int On_Encoder_Check(double d_pos);
	int OnMotor_Go_Check(int n_motor, double d_pos);
	int DoorOpenCheckSpot();
	void OnMot_Speed_Setting();
	int OnElect_CheckData(CString str_data);

	int Reject_Tray_UnclampCheck();//20131203


	int OnBCR_CheckData(CString str_data);
	int OnLabel_CheckData(CString str_data, int port);

	CString Get_MotorName(int n_motor_no);
	CString Get_AlarmPartName(int n_motor_no);
	BOOL CreateFolder(LPCTSTR szPath);
	void On_LogFile_Add(int n_msg_select, CString str_msg, int n_Opt=0);
	void OnLogGalmg_PushLot( CString strlot, CString strtmp );
	void On_Log_Title_Msg( int iLogType, CString strTitle, CString strMsg );
	void OnLogWorkStep( CString strClass, CString strFunc, int iOld, int iCur );
	void MsgLog( LPCTSTR strMsg );
	CString GetStrCurrentTime();
	static void DeleteOldFiles( LPCTSTR lpDirPath, int iDays );
	void SetRBufferStation( int iPos, int iVal, bool bWrite = true );

	bool CheckWorkingLot( CString strLotNo );


	BOOL Handler_Recovery_Data_Read();
	CString GetFileString( LPCSTR lpApp, LPCSTR lpKey, LPCSTR lpDef, DWORD dwSize, LPCSTR lpFilePath );

	void OnInitBtn(int iVal);
	int GetCurrentRbtPos();
	void Robot_BackMovePosValCheck();
	void BackupData();

	std::vector<int> LoadConv();
	std::vector<int> UnloadConv( int nLotIdx );
	std::vector<int> UnloadRetest();
	std::vector<int> UnloadFail();

	void OnLoadBufferIn(std::vector<int> vecTest);

	//Load DVC Check
	void UnloadDataClear();
	int BcrPickerOff_Chk();

	DWORD m_dwBcrTime;

	CString OnSocketInfoCalc(int n_type, int n_socket, int n_bin, int n_yield);
	void On_Socket_Data_Load(int n_type, CString str_file_name);

	void Rewind_Mode(int port);
	void Print_Complete_Set(int port);
	void Print_Complete_Reset(int port);
	void Printer_Status(int port);

	int OutLptPort(CString str_text);
	int OnLabelPrint_Parsing(CString strCheckData, int Qty);
	int OnBcrRev_Excution();

	bool CmpStrFile( CString strCmp1, CString strCmp2 );
	CString GetPrintFormat();
	CString GetBcrPrintFormat( int opt, CString strSerial);

	////2014.1114 ysj
	void OnBcrInputreader();
	void OnBcrOutputreader();

	CString OnFindInRcvMsg(CString szFind, CString szOrg, CString szDelimiter);	// 특정한 문자열 사이의 데이터 반환
	CString OnBodyAnalysis( int nMode, CString strRecv, CString strFind );

	////2015.0114
	int  OnWatchDoor();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
	int  OnIsDoorOpen();
	void OnMonitoring_Door();	// 도어의 [열림/닫힘] 상태 모니터링
	void OnOpenDoor();	// 도어 열기
	void OnCloseDoor();	// 도어 닫기
	CString OnCheck_DoorState();	// 도어의 [열림/닫힘] 상태 체크
	////
	int OnIsInterruptCondition();//2015.0211
	void OnSavePPID();//2015.0221
	void LoadRegLabelCapaData();
	int CheckLotSerial();
	int GetModelNum(CString strPartName);
	CString GetModelName( int nNum);
	int LoadRegModelData();
	int GetModelFromPartID( CString PartID, CString& strModel);
	int ModelFileload(CString strMdl);
	int GetModelNumFromModel( CString strModel);
	void RunRegPartID();
	void SendRegPartID();

private:
	int AreaSensor_Time[MOTOR_COUNT][3];
	int AreaSensor_Flag[MOTOR_COUNT];

	double d_motiondone_Time[MOTOR_COUNT][3];
	int n_motiondone_Flag[MOTOR_COUNT];

	int n_barcodepos_check;

	long ml_door_check[3];
	int mn_door_check_flag;

	CString str_step_temp[100];
	CTime  StepChangeTime[100];
	CString m_strItemValue[500];
	CString m_strModel[100][500];
};
#endif // !defined(AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_)
