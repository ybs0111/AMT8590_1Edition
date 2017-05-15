// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
#define AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"       // 전역 변수 정의 클래스

class CMyJamData  
{
public:
	CMyJamData();
	virtual ~CMyJamData();
	int mn_noError;
	SYSTEMTIME m_timeAlarmLastDay;

// Attributes
public:
//	CAlarmListFile mcls_alarm_list;  // 알람 리스트 파일 생성 클래스 변수 선언

// Operations
public:
	//2013,0306
	void On_UnderDaily_ListFile_Add(CString str_alarm_code, CString str_display_time, CString str_alarm_msg);
	void On_Daily_ListFile_Add(CString str_alarm_code, CString str_display_time, CString str_alarm_msg);
	CString On_Alarm_Display_Info_Load(CString strPath, CString strDataFile, CString strActionFile, CString strCode);  // 발생 알람 정보 설정 함수

	CString On_Alarm_Bmp_Data_Load(CString strPath, CString strDataFile, CString strCode);       // 발생 알람 부위 표시용 그림 파일명 설정 함수
	CString On_Alarm_Style_Data_Load(CString strPath, CString strDataFile, CString strCode);     // 알람 발생 부위 표시 형태 및 출력 위치 설정 함수
	CString On_Alarm_Repair_Data_Load(CString strPath, CString strActionFile, CString strCode);  // 발생한 알람에 대한 조치 사항 설정 함수
	
	void On_Alarm_Display();					// 발생한 알람 화면 출력 함수
	void On_Occured_Alarm_Saving_to_File();		// 발생한 알람 로그 파일에 저장하는 함수
	void On_Alarm_Info_Load();					// 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수
	void On_Alarm_Info_Set_to_Variable(CString strErrCode);  // 발생한 알람 메시지 전역 변수에 설정하는 함수
};
extern CMyJamData			MyJamData;

CString GetJamCode_Work_Time( int iIdx, int iOn );
CString GetJamCode_Hook_Clamp_Time( int iIdx, int iOn );
CString GetJamCode_Hook_FwdBwd_Time( int iIdx, int iOn );
CString GetJamCode_Load_Clamp_Time( int iIdx, int iOn );
CString GetJamCode_Paper_FwdBwd_Time( int iIdx, int iOn );
CString GetJamCode_FeederLock_Time( int iIdx, int iOn );
CString GetJamCode_Bcr_Vacuum_Fwdbwd_Time( int iIdx, int iOn );
CString GetJamCode_Bcr_Plate_UoDown_Time( int iIdx, int iOn );
CString GetJamCode_Bcr_Guide_FwdBwd_Time( int iIdx, int iOn );
CString GetJamCode_Bcr_Guide_UpDown_Time( int iIdx, int iOn );
CString GetJamCode_Rolling_FwdBwd_Time( int iIdx, int iOn );
CString GetJamCode_Unload_Picker_Time( int iIdx, int iOn );
CString GetJamCode_Unload_Convey_Time( int iIdx, int iOn );






CString GetJamCode_VacuumDn_Time( int iIdx, int iOn );
CString GetJamCode_BcrVacuumDn_Time( int iIdx, int iOn );
CString GetJamCode_BcrRotator_Time( int iIdx, int iOn );
CString GetJamCode_Rotator_Time( int iIdx, int iOn );
CString GetJamCode_TS_Plate_Time( int nSite, int iOn );
CString GetJamCode_TS_Insert_Time( int nSite, int iIdx, int iOn );
CString GetJamCode_TS_Lock_Time( int nSite, int iOn );
CString GetJamCode_Rotator_Chk( int iOn );
CString GetJamCode_TS_Plate_Chk( int nSite, int iOn );
CString GetJamCode_TS_Insert_Chk( int nSite, int iIdx, int iOn );
CString GetJamCode_TS_Lock_Chk( int nSite, int iOn );
CString GetJamCode_HF_Clamp_Chk( int nSite, int iOn );

CString GetJamCode_ModuleExist_Chk( int nPicker, int iOn );



CString GetJamCode_SorterPickerDn_Chk( int iIdx, int iOn );
CString GetJamCode_SorterModuleExist_Chk( int nPicker, int iOn );
#endif // !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
