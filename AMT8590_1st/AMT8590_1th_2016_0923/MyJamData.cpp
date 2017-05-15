// MyJamData.cpp: implementation of the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "MyJamData.h"

#include "io.h" 
#include "Public_Function.h"  // 모터 보드 및 I/O 보드 관련 클래스
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "SrcPart/APartHandler.h"
#include "AMTLotManager.h"
#include "XGemClient.h"
#include "AMTRegistry.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyJamData			MyJamData;
CMyJamData::CMyJamData()
{
	mn_noError = FALSE;
	m_timeAlarmLastDay = AMTRegistry::RegReadAlarmTime();
}

CMyJamData::~CMyJamData()
{

}

/* ****************************************************************************** */
/* 발생 알람 정보 설정 함수                                                       */
/* -> strPath     : 알람 정보 저장 파일 존재 폴더                                 */
/* -> strDataFile : 알람 정보 저장된 File Name(File Name+확장자)                        */
/* -> strCode     : 발생한 알람 Code                                              */
/* ****************************************************************************** */
CString CMyJamData::On_Alarm_Display_Info_Load(CString strPath, CString strDataFile, CString strActionFile, CString strCode)
{
	CString str_file_path="";  // 폴더 저장 변수 */
	CString str_alarm_chk="";  // 함수 리턴 정보 저장 변수 */
	int n_pos;

	str_file_path = strPath;
	if (str_file_path.Right(1) != "\\")  // 파일 경로 구분자 존재 검사 */
	{
		str_file_path += "\\";
	}

	str_alarm_chk = On_Alarm_Bmp_Data_Load(str_file_path, strDataFile, strCode);  // 발생 알람 부위 표시용 그림 File Name 설정 함수 */
	n_pos = str_alarm_chk.Find("Trouble");
	if (n_pos >= 0)  // 에러 발생
	{
		return str_alarm_chk;
	}

	str_alarm_chk = On_Alarm_Style_Data_Load(str_file_path, strDataFile, strCode);  // 알람 발생 부위 표시 형태 및 출력 위치 설정 함수 */
	n_pos = str_alarm_chk.Find("Trouble");
	if (n_pos >= 0)  // 에러 발생
	{
		return str_alarm_chk;
	}

	str_alarm_chk = On_Alarm_Repair_Data_Load(str_file_path, strActionFile, strCode);  // 발생한 알람에 대한 조치 사항 설정 함수 */
	n_pos = str_alarm_chk.Find("Trouble");
	if (n_pos >= 0)  // 에러 발생
	{
		return str_alarm_chk;
	}

	return str_alarm_chk;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 발생 알람 부위 표시용 그림 File Name 설정 함수                                 */
/* -> strPath     : 알람 표시 그림 파일 존재 폴더                                 */
/* -> strDataFile : 알람 표시 그림 File Name(File Name+확장자)                    */
/* -> strCode     : 발생한 알람 Code                                              */
/* ****************************************************************************** */
CString CMyJamData::On_Alarm_Bmp_Data_Load(CString strPath, CString strDataFile, CString strCode)
{
	CString str_return_data="";  // 함수 리턴 플래그
	CString str_file="";  // 알람 표시용 그림에 대한 [폴더]+[File Name] 저장 변수
	CString str_temp="";  // 알람 표시용 그림 File Name 임시 저장 변수

	char chr_buf[1000];

	str_return_data = _T("OK");  // 함수 리턴 플래그 초기화

	str_file = strPath + strDataFile;  // [폴더]+[File Name] 설정

	if (!GetPrivateProfileString(strCode, "DEF", "", chr_buf, 1000, str_file))
	{
		str_return_data = "File Name : "+ str_file;
		str_return_data += "\r\n";
		str_return_data += "[DEF] The information we read. Trouble -> Code : ";
		str_return_data += strCode;

		return str_return_data;
	}

	str_temp.Format("%s", chr_buf);
	st_alarm.mstr_bmp_file = str_temp;  // 알람 표시용 그램 File Name 설정

	return str_return_data;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 알람 발생 부위 표시 형태 및 출력 위치 설정 함수                                */
/* -> strPath     : 부위 표시 형태 저장 파일 폴더                                 */
/* -> strDataFile : 부위 표시 형태 저장 File Name(File Name+확장자)               */
/* -> strCode     : 발생한 알람 Code                                              */
/* ****************************************************************************** */
CString CMyJamData::On_Alarm_Style_Data_Load(CString strPath, CString strDataFile, CString strCode)
{
	CString str_chk_data="";		// 함수 리턴 플래그
	CString str_chk_file="";		// 알람 부위 표시 정보에 대한 [폴더]+[File Name] 저장 변수
	CString str_chk="", str_chk2="";  // 알람 부위 표시 형태 및 위치 저장 변수
	int start=0, end, i;		// 읽을 데이터 위치 저장 변수
	int max=4;					// 읽을 데이터 최대 갯수 (화살표 모양으로 초기화)

	char chr_buf[1000];
	char num[20];

	str_chk_data = _T("OK");  // 함수 리턴 플래그 초기화

	str_chk_file = strPath + strDataFile;  // [폴더]+[File Name] 설정

	if (!GetPrivateProfileString(strCode, "STYLE", "", chr_buf, 1000, str_chk_file))
	{
		str_chk_data = "File Name : "+ str_chk_file;
		str_chk_data += "\r\n";
		str_chk_data += "[STYLE] The information we read. Trouble -> Code : ";
		str_chk_data += strCode;

		return str_chk_data;
	}

	str_chk.Format("%s", chr_buf);

	for(i=0; i<max; i++)
	{
		end = str_chk.Find(';', start);
		if (end == -1)  // 구분자 미존재
		{
			break;
		}
		else            // 구분자 존재
		{
			str_chk2 = str_chk.Mid(start, end-start);  // 구분자 사이의 한 개 아이템 설정
			str_chk2.TrimLeft(' ');
			str_chk2.TrimRight(' ');

			if (i == 0)  // 알람 부위 표시 형태
			{
				st_alarm.mstr_style = str_chk2;       // 알람 부이 표시 형태 설정
				if (st_alarm.mstr_style == "CIRCLE")  // 부위 표시 정보 8개 존재
				{
					max = 8;    
				}
				else if (st_alarm.mstr_style == "NONE")
				{
					max = 1;
				}
			}
			else
			{
				lstrcpy(num, str_chk2);
				if (i == 1)
				{
					st_alarm.mn_Cx = atoi(num);         // X축 픽셀 위치
				}
				else if (i == 2)
				{
					st_alarm.mn_Cy = atoi(num);         // Y축 픽셀 위치
				}
				else if (i == 3)
				{
					st_alarm.mn_Offset = atoi(num);     // 원 크기
				}
				else if (i == 4)
				{
					st_alarm.mn_LineWidth = atoi(num);  // 라인 두께
				}
				else if (i == 5)
				{
					st_alarm.mn_R = atoi(num);          // RGB 중 R 값
				}
				else if (i == 6)
				{
					st_alarm.mn_G = atoi(num);          // RGB 중 G 값
				}
				else if (i == 7) 
				{
					st_alarm.mn_B = atoi(num);          // RGB 중 B 값
				}
				/* ************************************************************** */
			}

			start = end + 1;  // 다음 데이터 위치 설정
		}
	}

	if (i != max)  // Data Format 에러
	{
		str_chk_data = "File Name : "+ str_chk_file;
		str_chk_data += "\r\n";
		str_chk_data += "[STYLE] Data Format. Trouble -> Code : ";
		str_chk_data += strCode;

		return str_chk_data;
	}

	return str_chk_data;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 발생한 알람에 대한 조치 사항 설정 함수                                         */
/* -> strPath       : 알람 조치 사항 저장된 파일 폴더                             */
/* -> strActionFile : 알람 조치 사항 저장된 File Name(File Name+확장자)           */
/* -> strCode       : 발생한 알람 Code                                            */
/* ****************************************************************************** */
CString CMyJamData::On_Alarm_Repair_Data_Load(CString strPath, CString strActionFile, CString strCode)
{
	CString str_return_val="";		// 함수 리턴 플래그
	CString str_repair_file="";	// 조치 사항에 대한 [폴더]+[File Name] 저장 변수
	CString str_act_item="";		// 조치 사항 읽을 위치 설정 변수
	CString str_act_temp="";		// 조치 사항 임시 저장 변수
	int n_chk_cnt;				// 조치 사항 갯수 저장 변수
	int i; 
	
	char chr_buf[1000];

	str_return_val = _T("OK");  // 함수 리턴 플래그 초기화

	str_repair_file = strPath + strActionFile;  // [폴더]+[File Name] 설정

	if (!GetPrivateProfileString(strCode, "COUNT", "0", chr_buf, 10, str_repair_file))
	{
		str_return_val = "File Name : "+ str_repair_file;
		str_return_val += "\r\n";
		str_return_val += "[COUNT] The information we read. Trouble -> Code : ";
		str_return_val += strCode;

		return str_return_val;
	}

	n_chk_cnt = atoi(chr_buf);  // 읽을 조치 사항 갯수 설정
	if (n_chk_cnt<1)
	{
		str_return_val = "File Name : "+ str_repair_file;
		str_return_val += "\r\n";
		str_return_val += "[COUNT] Data Format Trouble -> Code : ";
		str_return_val += strCode;

		return str_return_val;
	}
	else
	{
		st_alarm.mn_action_cnt = n_chk_cnt;
	}

	(st_alarm.mstr_repair_msg).Empty();  //  조치 사항 저장 변수 초기화

	for (i=0; i<(st_alarm.mn_action_cnt); i++)   
	{
		str_act_item.Format("%d", i+1);  // 조치 사항 정보 읽을 위치 설정
		if (!GetPrivateProfileString(strCode, (LPCTSTR)str_act_item, "", chr_buf, 1000, str_repair_file))
		{
			str_return_val = "File Name : "+ str_repair_file;
			str_return_val += "\r\n";
			str_return_val += "An action subject. The information we read. Trouble";

			str_return_val += "\r\n";
			str_return_val += "Code : ";
			str_return_val += strCode;

			str_return_val += "\r\n";
			str_return_val += "Position : ";
			str_return_val += str_act_item;

			return str_return_val;
		}
		/* ********************************************************************** */

		st_alarm.mstr_repair_msg += "\r\n";			//조치 사항에 맨 첫줄을 띄자.

		str_act_temp.Format(" %s", chr_buf);  // 조치 사항 설정
		st_alarm.mstr_repair_msg += str_act_temp;

		if (i != ((st_alarm.mn_action_cnt)-1))  
			st_alarm.mstr_repair_msg += "\r\n";  // 조치 사항 단락 구분자 설정
	}
	/* ************************************************************************** */
	
	return str_return_val;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 발생한 알람 화면 출력 함수                                                     */
/* ****************************************************************************** */
void CMyJamData::On_Alarm_Display()
{
	CString str_cnt="", str_temp="";
	char temp_code[10]={0,}, temp_cnt[10]={0,}, chr_buf[20]={0,};
	
	CString str_display_time="";	// 파일 출력용 시간 저장 변수
	CString str_alarm_code="";		// 발생한 알람 코드 저장 변수
	CString str_alarm_msg="";		// 발생한 알람 메시지 저장 변수
	int n_cur_hour=0;				// 현재 시간 저장 변수
	int n_chk=0;					// 숫자형 알람 코드 저장 변수
	long n_chk_time=0;			// 알람 발생 시간 간격 저장 변수
	int n_data_save = 0;		// Jam을 저장할 것이냐?

	COleDateTime cur_time;
	CTime alarm_occured_time;

 	/* ************************************************************************** */
 	/* 장비 동작 중 알람 발생했는지 검사한다                                      */
 	/* -> 알람 발생한 경우 로그 파일 및 임시 파일에 저장하고 화면에 출력한다      */
 	/* ************************************************************************** */
 	if (COMI.mn_run_status == CTL_dJAM || COMI.mn_run_status == CTL_dWARNING || COMI.mn_run_status == CTL_dLOTEND)  // 알람 발생
 	{	
		//alarm.mstr_code = "990001";

 		if (st_handler.mn_alarm_screen == TRUE)	return; 
 		/* ********************************************************************** */
 		/* 알람 화면 출력 불가능한 경우 검사한다.                                 */
 		/* -> st_handler.mn_system_lock : SYSTEM LOCK 상태 플래그                  */
 		/* ********************************************************************** */
 		if (st_handler.mn_system_lock != FALSE)  // SYSTEM LOCK
 		{
 			return;  // 강제 리턴
 		}
 		/* ********************************************************************** */

		//알람 발생하면 무존건 알람시간을 리셋한다.
		st_work.mn_jam = CTL_YES;//2015.0327
		st_handler.m_dwJamStartTime = GetCurrentTime();
 

 		/* ************************************************************************** */
 		/* 알람 발생한 시간 설정한다                                                  */
 		/* ************************************************************************** */
 		cur_time = COleDateTime::GetCurrentTime();
 		n_cur_hour = cur_time.GetHour();
 
 		alarm_occured_time = CTime::GetCurrentTime();
 		str_display_time = alarm_occured_time.Format("%c");
 		/* ************************************************************************** */

 
 		/* ********************************************************************** */
 		/* 알람 화면 출력 상태 및 알람 카운트 여부 검사한다                       */
 		/* -> 이미 알람 화면 출력 중인 경우 재 출력할 필요 없으며                 */
 		/*    이 경우에 발생하는 알람은 무시한다                                  */
 		/*  : 알람 화면 출력 중에 알람 Code 변경되면 출력 메시지 달라지기 때문    */
 		/* -> st_handler.mn_alarm_screen : 알람 화면 출력 상태 플래그              */
 		/* -> st_alarm.mn_cur_state : 알람 저장 유무 플래그                        */
 		/* ********************************************************************** */
 		if (st_handler.mn_alarm_screen != TRUE)  // 알람 화면 미출력
 		{
 			if (COMI.mn_run_status == CTL_dLOTEND)		
 			{
 				alarm.mn_reset_status = CTL_NO;
 			}
 			else	alarm.mn_reset_status = CTL_YES;
 
 			//발생한 알람을 List에 넣는다.
 			if (alarm.mstr_code != "")
 			{
 				/* ************************************************************************** */
 				/* 현재 발생한 알람에 대한 메시지 정보 설정한다                               */
 				/* ************************************************************************** */
 				On_Alarm_Info_Set_to_Variable(alarm.mstr_code);  // 발생한 알람 메시지 전역 변수에 설정하는 함수
 				/* ************************************************************************** */
 				sprintf(temp_code, "%s", alarm.mstr_code);
// 				str_cnt.Format("%d", st_alarm.mn_cur_state);
 				sprintf(temp_cnt, "%s", str_cnt);
 
 				/* ************************************************************************** */
 				/* 바로 전에 발생한 알람 코드와 비교하여 동일한 경우 알람 카운트 미진행       */
 				/* -> 알람 카운트 시간 간격은 30초로 설정한다                                 */
 				/* ************************************************************************** */
 				alarm.stl_cur_alarm_time = GetCurrentTime();
 
 				n_chk = atoi(alarm.mstr_code);
				n_data_save = CTL_NO;				// 기본은 NO

				//st_basic.mn_alarm_delay_time = 10;
 				
 				if (alarm.stl_prev_code == n_chk)	// 알람 코드 동일
 				{
// 					n_chk_time = alarm.stl_cur_alarm_time - alarm.stl_prev_alarm_time; 

					n_chk_time = alarm.stl_cur_alarm_time - alarm.stl_alarm_start_time; 
 					
 					if (n_chk_time < st_basic.mn_alarm_delay_time * 1000 /*|| alarm.stl_alarm_start_time == 0*/)						// Basic에서 설정할수 있게 변경함. 2K9/11/19/ViboX
 					{
 						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
 					}
 					else
 					{
 						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
 						n_data_save = CTL_YES;			// Jam을 저장 및 Count를 한다.
 					}
 				}
 				else
 				{
 					/* ********************************************************************** */
 					/* 발생한 알람 정보로 이전 정보를 대체한다                                */
 					/* -> 다음에 발생하는 알람과 비교하는 정보로 사용된다                     */
 					/* ********************************************************************** */
//  					alarm.stl_prev_code = n_chk;
//  					alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
//  					/* ********************************************************************** */
//  
//  					n_data_save = CTL_YES;				// Jam을 저장 및 Count를 한다.
					mn_noError = FALSE;
					n_chk_time = alarm.stl_cur_alarm_time - alarm.stl_alarm_start_time; 
// 					if (n_chk_time < 5 * 1000/*2분*/)
// 					{
// 						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
// 						mn_noError = TRUE;
// // 						On_Occured_Alarm_Saving_to_File();  // 발생한 알람 로그 파일에 저장하는 함수
// 					}
// 					else
// 					{
// 						alarm.stl_prev_code = n_chk;
						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
						n_data_save = CTL_YES;			// Jam을 저장 및 Count를 한다.
// 					}
				}
 				/* ************************************************************************** */
 			}

			if (st_handler.mn_status == CTL_NO || /*st_basic.mn_mode_run == CTL_NO ||*/ st_handler.mn_virtual_supply == 1/* || (g_lotMgr.GetLotIDAt(0).GetLength() < 5)*/)
			{
				n_data_save = CTL_NO;
			}

 			if (n_data_save == CTL_YES)
 			{
				if (st_alarm.mstr_cur_state == "1")			// Count하는 Alarm이면... 2K9/08/28/ViboX
				{
					alarm.mn_alarm_occured++;
					alarm.mn_alarm_occured_day++;

					str_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
					:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(str_temp), st_path.mstr_basic);
				}

				if (st_handler.mn_status == CTL_YES)					// 장비가 가동중이냐
				{
					if (st_alarm.mstr_cur_state == "1")	
						g_handler.AddAlarmCnt();
					On_Occured_Alarm_Saving_to_File();  // 발생한 알람 로그 파일에 저장하는 함수
				}
				alarm.mn_svr_alarm = CTL_NO;
				if( alarm.mstr_code == "900101" || alarm.mstr_code == "900102" || 
					alarm.mstr_code == "900103" || alarm.mstr_code == "900104")
				{
					alarm.mn_svr_alarm = CTL_YES;
				}
 			}
 
 			st_handler.mn_alarm_screen = TRUE;	// 알람 화면 출력 플래그 설정
 
 			Func.OnSet_IO_Port_Alarm();			// 알람 상태 I/O 출력 함수

			//2015.0413
			if( st_basic.mn_mode_xgem == CTL_YES )
			{
				g_xgem_client.SetGemAlarmState(alarm.mstr_code, 1);
			}

			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 1);  // 메인 프레임에 알람 화면 출력 요청 
 
 			if (st_handler.cwnd_title != NULL)  // 타이틀 바 출력 검사
 				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);  // 타이틀 [알람] 출력 요청

 		}
// 		/* ********************************************************************** */
 	}
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 발생한 알람 로그 파일에 저장하는 함수                                          */
/* ****************************************************************************** */
void CMyJamData::On_Occured_Alarm_Saving_to_File()
{
	CString str_display_time="";	// 파일 출력용 시간 저장 변수
	CString str_alarm_code="";		// 발생한 알람 Code 저장 변수
	CString str_alarm_msg="";		// 발생한 알람 메시지 저장 변수
	int n_cur_hour;				// 현재 시간 저장 변수
	int n_chk;					// 숫자형 알람 Code 저장 변수
	long n_chk_time;			// 알람 발생 시간 간격 저장 변수

	COleDateTime cur_time;
	CTime alarm_occured_time;

	/* ************************************************************************** */
	/* 알람 발생한 시간 설정한다                                                  */
	/* -> SHIFT 상태 검사 및 알람 발생 시간 저장하기 위하여 필요                  */
	/* ************************************************************************** */
	cur_time = COleDateTime::GetCurrentTime();
	n_cur_hour = cur_time.GetHour();

	alarm_occured_time = CTime::GetCurrentTime();
	str_display_time = alarm_occured_time.Format("%Y/%m/%d  %H:%M:%S");
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 알람 Code 및 알람 메시지 함수 변수에 설정한다                              */
	/* -> 현재는 이 함수 호출 전에 새로운 알람이 발생하는 경우 알람 Code와        */
	/*    조치 사항 및 기타 정보가 일치하지 않을 수 있다                          */
	/* -> alarm.mstr_code : 현재 발생한 알람 Code 정보 저장 변수                   */
	/* -> st_alarm.mstr_cur_msg : 발생한 알람 메시지 저장 변수                     */
	/* ************************************************************************** */
	str_alarm_code = alarm.mstr_code;
	str_alarm_msg = st_alarm.mstr_cur_msg;

	st_alarmdb.mole_date = COleDateTime::GetCurrentTime();

// 	st_alarmdb.mstrlot_no = st_pcb[1].strLotNumber;
// 	st_alarmdb.mstrpart_no = st_pcb[1].strPartNumber;

	st_alarmdb.mstrAlarmCode = str_alarm_code;
	st_alarmdb.mstrAlarmMsg = str_alarm_msg;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 일별, 월별 알람 발생 횟수 임시 파일에 추가한다                             */
	/* ************************************************************************** */
	//2012,0306
// 	if(mn_noError == TRUE)
// 	{
		mn_noError = FALSE;
		//2015.0404 under daily display
// 		On_Daily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); // 일별, 월별 알람 정보 리스트 파일에 추가 함수 */
// 		On_UnderDaily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); // 일별, 월별 알람 정보 리스트 파일에 추가 함수 */
// 		return;
// 	}
	
	//2015.0404 
	if (st_alarm.mstr_cur_state == "1")			// Count하는 Alarm이면... 2K9/08/28/ViboX
	{
		On_Daily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); // 일별, 월별 알람 정보 리스트 파일에 추가 함수 */
// 		On_UnderDaily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); // 일별, 월별 알람 정보 리스트 파일에 추가 함수 */
// 		Func.DB_Write_Alarm(st_alarmdb);

		{//2015.0513
			CString strLog="";
// 			strLog.Format(", Error, %s, %s, %s", alarm.mstr_code, st_handler.m_strLogEvent, str_alarm_msg);
			strLog.Format(" Error, %s", st_handler.m_strLogEvent);
			Func.On_LogFile_Add(LOG_EVENT, strLog, 1);
		}
	}
	else
	{
		On_UnderDaily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); // 일별, 월별 알람 정보 리스트 파일에 추가 함수 */
// 		Func.DB_Write_Alarm(st_alarmdb);
	}
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* 바로 전에 발생한 알람 Code와 비교하여 동일한 경우 알람 카운트 미진행       */
	/* -> 알람 카운트 시간 간격은 30초로 설정한다                                 */
	/* ************************************************************************** */
	alarm.stl_cur_alarm_time = GetCurrentTime();

	n_chk = atoi(str_alarm_code);
	
	if (alarm.stl_prev_code == n_chk)  // 알람 Code 동일
	{
		n_chk_time = alarm.stl_cur_alarm_time - alarm.stl_prev_alarm_time; 
		
		if (n_chk_time < 30000) 
		{
			alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
			return;
		}
	}
	else
	{
		/* ********************************************************************** */
		/* 발생한 알람 정보로 이전 정보를 대체한다                                */
		/* -> 다음에 발생하는 알람과 비교하는 정보로 사용된다                     */
		/* ********************************************************************** */
		alarm.stl_prev_code = n_chk;
		alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
		/* ********************************************************************** */
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* SHIFT별 알람 발생 횟수 임시 저장 파일에 알람 추가한다                      */
	/* -> 알람 발생 횟수 임시 저장 파일 미존재 시 생성할 알람 리스트 파일 이름은  */
	/*    일별 파일 이름에서 헤더를 제외한 정보와 동일하다                        */
	/* ************************************************************************** */
	if (n_cur_hour >= 6 && n_cur_hour < 14)  // SHIFT 중 DAY */
	{
//		mcls_alarm_list.On_Shift_File_Add(1, str_alarm_code, str_display_time);  // SHIFT별 알람 발생 횟수 정보 임시 파일에 추가 함수 */
	}
	else if (n_cur_hour >= 14 && n_cur_hour < 22)  // SHIFT 중 SWING */
	{
//		mcls_alarm_list.On_Shift_File_Add(2, str_alarm_code, str_display_time);  // SHIFT별 알람 발생 횟수 정보 임시 파일에 추가 함수 */
	}
	else  // SHIFT 중 GY */
	{
//		mcls_alarm_list.On_Shift_File_Add(3, str_alarm_code, str_display_time);  // SHIFT별 알람 발생 횟수 정보 임시 파일에 추가 함수 */
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* LOT별 알람 발생 횟수 임시 파일에 추가한다                                  */
	/* ************************************************************************** */
//	mcls_alarm_list.On_Lot_ListFile_Add(str_alarm_code, str_display_time);  // LOT별 알람 발생 횟수 정보 임시 파일에 추가 함수 */
	/* ************************************************************************** */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수                         */
/* ****************************************************************************** */
void CMyJamData::On_Alarm_Info_Load()
{
	CString str_load_file="";
	CString str_read_data="";  // 파일에 저장된 한 라인 정보 저장 변수
	CString str_temp_data="";
	CString str_comment_data="";
	int n_count, n_len;
	int n_pos = 0 ;

	char chr_s[300];
	FILE  *fp ;  // 파일 핸들 저장 변수

	str_load_file = _T("c:\\AMT8590\\Bmp\\Error.txt"); 

	/* ************************************************************************** */
    /* 알람 정보 저장할 전역 구조체 멤버 변수 초기화                              */
    /* ************************************************************************** */
	for (n_count = 0; n_count < 2000; n_count++)  
	{
		(st_alarm.mstr_e_content[n_count]).Empty();  // 파일에서 로딩한 한 라인 정보 저장 변수 초기화
		(st_alarm.mstr_e_code[n_count]).Empty();     // 알람 Code 저장 변수 초기화
		(st_alarm.mstr_e_state[n_count]).Empty();    // 알람 상태 저장 변수 초기화
		(st_alarm.mstr_e_part[n_count]).Empty();     // 알람 발생 부위 저장 변수 초기화
		(st_alarm.mstr_e_msg[n_count]).Empty();      // 알람 메시지 저장 변수 초기화
	}
	/* ************************************************************************** */
	/* ************************************************************************** */
    /* 알람 정보 저장된 파일 연다                                                 */
    /* ************************************************************************** */
	if ((fp=fopen(str_load_file,"rt"))==NULL)  
	{
		Func.MsgLog("The failure because we open the file.");
		return ;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
    /* 마지막 파일 정보까지 모두 읽어 전역 변수에 설정한다                        */
    /* ************************************************************************** */
	while(!feof(fp))  // 파일 마지막 검사
	{
		sprintf(chr_s, "");
		
        if (fgets(chr_s,300,fp)==NULL && ferror(fp))
		{   
			Func.MsgLog("The failure because we read the file.");
			return ;
		}
		
		str_read_data.Format("%s",chr_s);  // 파일에서 읽은 한 라인 정보 설정

		n_len = lstrlen(str_read_data);		//빈줄 체크

		if (!str_read_data.IsEmpty() && n_len > 2)
		{
			str_comment_data = str_read_data.Left(2);		//주석줄인지 확인한다.

			if (str_comment_data != "//")					//주석줄이 아니라면...
			{
				st_alarm.mstr_e_content[n_pos] = str_read_data;         // 파일에서 로딩한 한 라인 정보 설정
				st_alarm.mstr_e_code[n_pos] = str_read_data.Left(6) ;    // 알람 Code 설정
				st_alarm.mstr_e_state[n_pos] = str_read_data.Mid(7,1) ;  // 알람 상태 설정
				st_alarm.mstr_e_part[n_pos] = str_read_data.Mid(9,1) ;   // 알람 발생 부위 설정

				str_temp_data = str_read_data.Mid(12,270);
				
				if (st_alarm.mstr_e_code[n_pos] != "UNK000")
				{
					st_alarm.mstr_e_msg[n_pos] = str_temp_data.Left(str_temp_data.GetLength() - 3) ;  // 공백 제거 알람 메시지 설정
				}
				else
				{
					st_alarm.mstr_e_msg[n_pos] = str_temp_data.Left(str_temp_data.GetLength() - 1) ;  // 공백 제거 알람 메시지 설정
				}
				n_pos++;
			}
		}
	}
	/* ************************************************************************** */

	fclose(fp) ;  // 파일 종료
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 발생한 알람 메시지 전역 변수에 설정하는 함수                                   */
/* ****************************************************************************** */
void CMyJamData::On_Alarm_Info_Set_to_Variable(CString strErrCode)
{
	int n_cnt ;
	int n_cur_num = -1;    // 알람에 대한 배열 위치 저장 변수
	CString str_err_code="";  // 알람 Code 저장 변수
	CString strPath="";

	CString mstr_create_file="";  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수
	CString mstr_content="";  // 한 라인에 출력할 문자열 정보 저장 변수 
	int mn_existence;      // 파일 존재 유무 설정 플래그 
	char fileName[256];    // 검색할 파일 정보 설정 함수 
	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
	
	CString str_title_time="", str_file_time="", str_new_file="", str_display_time="";		// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur, alarm_occured_time;							// 타이틀 시간 저장 변수

	/* ************************************************************************** */
    /* 입력된 알람 Code에서 좌/우 공백을 제거한다                                 */
    /* ************************************************************************** */
	str_err_code = strErrCode;
	str_err_code.TrimLeft(' ');	
	str_err_code.TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
    /* 발생한 알람 Code에 대한 배열 위치 검색한다                                 */
    /* ************************************************************************** */
	for(n_cnt=0; n_cnt<2000; n_cnt++)
	{  
		if ( (st_alarm.mstr_e_code[n_cnt]).Compare(str_err_code) == 0 )
		{
			n_cur_num = n_cnt ; 
			break;
		}
	}
	/* ************************************************************************** */
	
	if (n_cur_num == -1)  // 미존재 알람
	{
		/* ************************************************************************** */
		/* 파일 이름으로 사용할 날짜 설정한다                                         */
		/* ************************************************************************** */
		mtime_cur = COleDateTime::GetCurrentTime();
		
		n_cur_year = mtime_cur.GetYear();  
		n_cur_month = mtime_cur.GetMonth();  
		n_cur_day = mtime_cur.GetDay();  

		alarm_occured_time = CTime::GetCurrentTime();
		str_display_time = alarm_occured_time.Format("%Y/%m/%d  %H:%M:%S");
		
		str_new_file.Format("ACD%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
		
		mstr_create_file = "D:\\AMTLOG\\Log\\AlarmCodeDebug\\" + str_new_file;
		mstr_create_file += ".TXT";
		
		sprintf(fileName, "%s", mstr_create_file);
		mn_existence = access(fileName,0);
		
		if (mn_existence == -1)  /* 파일 미존재 */
		{
			mstr_create_file = "D:\\AMTLOG\\Log\\AlarmCodeDebug\\" + str_new_file;
			mstr_create_file += ".TXT";
			/* ********************************************************************** */
		}
		
		/* ************************************************************************** */
		/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
		/* ************************************************************************** */
		if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
		{
//			Func.MsgLog("파일 열기 실패!..");
			return;
		}
		/* ************************************************************************** */
		
		mstr_content = "Can't find alarm code : " + str_err_code;
		fprintf(fp,"%s\n",mstr_content) ;
		mstr_content = "time : " + str_display_time;
		fprintf(fp,"%s\n",mstr_content) ;
		mstr_content = "------------------------------------------------------------------";
		fprintf(fp,"%s\n",mstr_content) ;
		
		if (ferror(fp))  
		{
			Func.MsgLog("file save failed!..") ;
			fclose(fp);
			return ;
		}
		/* ************************************************************************** */
		
		fclose(fp);  /* 파일 종료 */

		alarm.mstr_pcode = alarm.mstr_code;
// 		alarm.mstr_code = "990000";
// 		sprintf(alarm.mc_code, "990000");
// 		str_err_code = "990000";
		alarm.mstr_code = "990000";
		sprintf(alarm.mc_code, "990000");
		str_err_code = "990000";

		for (n_cnt=0; n_cnt<2000; n_cnt++)
		{  
			if ( (st_alarm.mstr_e_code[n_cnt]).Compare(str_err_code) == 0 )
			{
				n_cur_num = n_cnt ; 
				break;
			}
		}
		/* ************************************************************************** */
	}
	
	/* ************************************************************************** */
    /* 발생한 알람에 대한 상태와 메시지 전역 변수에 설정한다                      */
    /* ************************************************************************** */
//	st_alarm.mn_cur_state = atoi(st_alarm.mstr_e_state[n_cur_num]);
	st_alarm.mstr_cur_state = st_alarm.mstr_e_state[n_cur_num];
	st_alarm.mstr_cur_msg = st_alarm.mstr_e_msg[n_cur_num];
	/* ************************************************************************** */
}
/* ****************************************************************************** */
void CMyJamData::On_UnderDaily_ListFile_Add(CString str_alarm_code, CString str_display_time, CString str_alarm_msg)
{
	CString mstr_file_name="";    // 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file="";  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name="", mstr_temp_data="";  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content="";  // 한 라인에 출력할 문자열 정보 저장 변수 
	int mn_existence;      // 파일 존재 유무 설정 플래그 
	char fileName[256];    // 검색할 파일 정보 설정 함수 
	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
	
	CString str_title_time, str_file_time, str_new_file;		// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수
	
	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
// 	COleDateTimeSpan span( 0, 22, 0, 0 ); // 하루.
// 	COleDateTime timeLastDay( m_timeAlarmLastDay );
// 	COleDateTime timeSpan = COleDateTime::GetCurrentTime() - span;
// 	if( timeSpan >= timeLastDay )
// 	{
// 		COleDateTimeSpan span2( 1, 0, 0, 0 );
// 		timeSpan += span2;
// 		timeSpan.GetAsSystemTime(m_timeAlarmLastDay );
// 		m_timeAlarmLastDay.wHour = 0;
// 		m_timeAlarmLastDay.wMinute = 0;
// 		m_timeAlarmLastDay.wSecond = 0;
// 		m_timeAlarmLastDay.wMilliseconds = 0;
// 		
// 		AMTRegistry::RegWriteAlarmTime( m_timeAlarmLastDay );
// 		n_cur_year = timeSpan.GetYear();  
// 		n_cur_month = timeSpan.GetMonth();  
// 		n_cur_day = timeSpan.GetDay(); 
// 	}
// 	else
// 	{
		n_cur_year = mtime_cur.GetYear();  
		n_cur_month = mtime_cur.GetMonth();  
		n_cur_day = mtime_cur.GetDay(); 
// 	}

	
	str_new_file.Format("DY%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	
	if (str_alarm_code.IsEmpty()==TRUE)  /* 알람 코드 미존재 */
	{
		//Func.MsgLog("Alarm 코드 에러!.."); 
		return;
	}
	
	mstr_create_file = "D:\\AMTLOG\\Alarm\\UnderDaily\\" + str_new_file;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* 파일 미존재 */
	{
		mstr_create_file = "D:\\AMTLOG\\Alarm\\UnderDaily\\" + str_new_file;
		mstr_create_file += ".TXT";
		/* ********************************************************************** */
	}
	
	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("1.File open failed!..");
		return;
	}
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* 로그 파일에 현재 발생한 알람 정보 저장한다                                 */
	/* ************************************************************************** */
	//mstr_content = "--------------------------------------------------------";
	//fprintf(fp,"%s\n",mstr_content) ;
	
	st_alarm.mn_e_list++;
	
	mstr_temp_data.Format("| %4d |", st_alarm.mn_e_list);
	mstr_content = mstr_temp_data; 
	mstr_temp_data.Format(" %6s |", str_alarm_code);
	mstr_content += mstr_temp_data; 
	mstr_temp_data.Format(" %-44s |", str_alarm_msg);
	mstr_content += mstr_temp_data;
	mstr_temp_data.Format("          |     |");
	mstr_content += mstr_temp_data;
	mstr_temp_data.Format(" %17s |", str_display_time);
	mstr_content += mstr_temp_data;
	fprintf(fp,"%s\n",mstr_content) ;
	
	mstr_content = "+------------------------------------------------------------------------------------------------------+";
	fprintf(fp,"%s\n",mstr_content) ;
	
	if (ferror(fp))  
	{
		Func.MsgLog("file save failed!..") ;
		fclose(fp);
		return ;
	}
	/* ************************************************************************** */
	
	fclose(fp);  /* 파일 종료 */
}

void CMyJamData::On_Daily_ListFile_Add(CString str_alarm_code, CString str_display_time, CString str_alarm_msg)
{
	CString mstr_file_name="";    // 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file="",mstr_create_file1="";  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name="", mstr_temp_data,mstr_temp_data1="";  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content="",mstr_content1="";  // 한 라인에 출력할 문자열 정보 저장 변수 
	int mn_existence;      // 파일 존재 유무 설정 플래그 
	char fileName[256];    // 검색할 파일 정보 설정 함수 
	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
	FILE *fp1;

	CString str_title_time="", str_file_time="", str_new_file,str_new_file1="";		// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수
	
	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	COleDateTimeSpan span( 0, 22, 0, 0 ); // 하루.
	COleDateTime timeLastDay( m_timeAlarmLastDay );
	COleDateTime timeSpan = COleDateTime::GetCurrentTime() - span;
	if( timeSpan >= timeLastDay )
	{
		COleDateTimeSpan span2( 1, 0, 0, 0 );
		timeSpan += span2;
		timeSpan.GetAsSystemTime( m_timeAlarmLastDay );
		m_timeAlarmLastDay.wHour = 0;
		m_timeAlarmLastDay.wMinute = 0;
		m_timeAlarmLastDay.wSecond = 0;
		m_timeAlarmLastDay.wMilliseconds = 0;
		
		AMTRegistry::RegWriteAlarmTime( m_timeAlarmLastDay );
		n_cur_year = timeSpan.GetYear();  
		n_cur_month = timeSpan.GetMonth();  
		n_cur_day = timeSpan.GetDay(); 
		st_alarm.mn_e_list = 0;
	}
	else
	{
		n_cur_year = timeLastDay.GetYear();// timeSpan.GetYear();  
		n_cur_month = timeLastDay.GetMonth();// timeSpan.GetMonth();  
		n_cur_day = timeLastDay.GetDay();// timeSpan.GetDay(); 
	}
	
	str_new_file.Format("DY%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_new_file1.Format("Error_%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);

	if (str_alarm_code.IsEmpty()==TRUE)  /* 알람 코드 미존재 */
	{
		//Func.MsgLog("Alarm 코드 에러!.."); 
		return;
	}

	mstr_create_file = "D:\\AMTLOG\\Alarm\\Daily\\" + str_new_file;
	mstr_create_file += ".TXT";

	mstr_create_file1 = st_path.mstr_error_path + str_new_file1;
	mstr_create_file1 += ".TXT";

	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* 파일 미존재 */
	{
		mstr_create_file = "D:\\AMTLOG\\Alarm\\Daily\\" + str_new_file;
		mstr_create_file += ".TXT";
		/* ********************************************************************** */
	}

	sprintf(fileName, "%s", mstr_create_file1);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* 파일 미존재 */
	{
		mstr_create_file1 = st_path.mstr_error_path + str_new_file1;
		mstr_create_file1 += ".TXT";
		/* ********************************************************************** */
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("2.File open failed!..");
		return;
	}
	if ((fp1=fopen(mstr_create_file1,"a+"))==NULL)  
	{
		Func.MsgLog("2.File1 open failed!..");
		fclose(fp);
		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 로그 파일에 현재 발생한 알람 정보 저장한다                                 */
	/* ************************************************************************** */
	//mstr_content = "--------------------------------------------------------";
	//fprintf(fp,"%s\n",mstr_content) ;

	st_alarm.mn_e_list++;

	mstr_temp_data.Format("| %4d |", st_alarm.mn_e_list);
	mstr_content = mstr_temp_data; 
	mstr_temp_data.Format(" %6s |", str_alarm_code);
	mstr_content += mstr_temp_data; 
	mstr_temp_data.Format(" %-44s |", str_alarm_msg);
	mstr_content += mstr_temp_data;
	mstr_temp_data.Format("          |     |");
	mstr_content += mstr_temp_data;
	mstr_temp_data.Format(" %17s |", str_display_time);
	mstr_content += mstr_temp_data;
	fprintf(fp,"%s\n",mstr_content) ;

	mstr_content = "+------------------------------------------------------------------------------------------------------+";
	fprintf(fp,"%s\n",mstr_content) ;

	if (ferror(fp))  
	{
		Func.MsgLog("file save failed!..") ;
		fclose(fp);
		return ;
	}
	/* ************************************************************************** */

	fclose(fp);  /* 파일 종료 */

	//2015.0513
	mstr_temp_data1.Format(" %17s, ", str_display_time);
	mstr_content1 = mstr_temp_data1;
	mstr_temp_data1.Format(" %6s, ", str_alarm_code);
	mstr_content1 += mstr_temp_data1; 
	mstr_temp_data1.Format(" %30s, ", st_handler.m_strLogEvent);
	mstr_content1 += mstr_temp_data1; 	
	mstr_temp_data1.Format(" %-44s ", str_alarm_msg);
	mstr_content1 += mstr_temp_data1; 
	fprintf(fp1,"%s\n",mstr_content1) ;
	
	if (ferror(fp1))  
	{
		Func.MsgLog("file1 save failed!..") ;
		fclose(fp1);
		return ;
	}
	fclose(fp1);
}

CString GetJamCode_Work_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

//525100 1 00 "Top ID Buffer LD1 Shifter Hook Clamp Check Off Error."
//525101 1 00 "Top ID Buffer LD1 Shifter Hook Clamp Check On Error."
//525110 1 00 "Top ID Buffer LD1 Shifter Hook UnClamp Check Off Error."
//525111 1 00 "Top ID Buffer LD1 Shifter Hook UnClamp Check On Error."
//525120 1 00 "Top ID Buffer LD2 Shifter Left Hook Clamp Check Off Error."
//525121 1 00 "Top ID Buffer LD2 Shifter Left Hook Clamp Check On Error."
//525130 1 00 "Top ID Buffer LD2 Shifter Left Hook UnClamp Check Off Error."
//525131 1 00 "Top ID Buffer LD2 Shifter Left Hook UnClamp Check On Error."
//525140 1 00 "Top ID Buffer LD2 Shifter Right Hook Clamp Check Off Error."
//525141 1 00 "Top ID Buffer LD2 Shifter Right Hook Clamp Check On Error."
//525150 1 00 "Top ID Buffer LD2 Shifter Right Hook UnClamp Check Off Error."
//525151 1 00 "Top ID Buffer LD2 Shifter Right Hook UnClamp Check On Error."
CString GetJamCode_Hook_Clamp_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

//525080 1 00 "Top ID Buffer LD1 Shifter Hook Forward Check Off Error."
//525081 1 00 "Top ID Buffer LD1 Shifter Hook Forward Check On Error."
//525090 1 00 "Top ID Buffer LD1 Shifter Hook Backward Check Off Error."
//525091 1 00 "Top ID Buffer LD1 Shifter Hook Backward Check On Error."
//527000 1 00 "Top ID Buffer LD2 Shifter Hook Forward Check Off Error."
//527001 1 00 "Top ID Buffer LD2 Shifter Hook Forward Check On Error."
//527010 1 00 "Top ID Buffer LD2 Shifter Hook Backward Check Off Error."
//527011 1 00 "Top ID Buffer LD2 Shifter Hook Backward Check On Error."
CString GetJamCode_Hook_FwdBwd_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}
//527020 1 00 "Top ID Buffer1.2 Clamp Check Off Error."
//527021 1 00 "Top ID Buffer1.2 Clamp Check On Error."
//527030 1 00 "Top ID Buffer1.2 UnClamp Check Off Error."
//527031 1 00 "Top ID Buffer1.2 UnClamp Check On Error."
CString GetJamCode_Load_Clamp_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Paper_FwdBwd_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_FeederLock_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Bcr_Vacuum_Fwdbwd_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Bcr_Plate_UoDown_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Bcr_Guide_FwdBwd_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Bcr_Guide_UpDown_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Rolling_FwdBwd_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Unload_Picker_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Unload_Convey_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}


CString GetJamCode_VacuumDn_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "5%04d%d", iIdx, iOn );
	return strJamCode;
}

//223060 1 A "Barcode Robot Picker #1 Cylinder Up Check Off Error."
//223061 1 A "Barcode Robot Picker #1 Cylinder Up Check On Error."
//223070 1 A "Barcode Robot Picker #1 Cylinder Down Check Off Error."
//223071 1 A "Barcode Robot Picker #1 Cylinder Down Check On Error."
//223080 1 A "Barcode Robot Picker #2 Cylinder Up Check Off Error."
//223081 1 A "Barcode Robot Picker #2 Cylinder Up Check On Error."
//223090 1 A "Barcode Robot Picker #2 Cylinder Down Check Off Error."
//223091 1 A "Barcode Robot Picker #2 Cylinder Down Check On Error."
//223100 1 A "Barcode Robot Picker #3 Cylinder Up Check Off Error."
//223101 1 A "Barcode Robot Picker #3 Cylinder Up Check On Error."
//223110 1 A "Barcode Robot Picker #3 Cylinder Down Check Off Error."
//223111 1 A "Barcode Robot Picker #3 Cylinder Down Check On Error."
//223120 1 A "Barcode Robot Picker #4 Cylinder Up Check Off Error."
//223121 1 A "Barcode Robot Picker #4 Cylinder Up Check On Error."
//223130 1 A "Barcode Robot Picker #4 Cylinder Down Check Off Error."
//223131 1 A "Barcode Robot Picker #4 Cylinder Down Check On Error."
CString GetJamCode_BcrVacuumDn_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "223%02d%d", (iIdx*2)+6+1+iOn, iOn );
	return strJamCode;
}
//223140 1 A "Barcode Robot Rotator 0 degree Check Off Error."
//223141 1 A "Barcode Robot Rotator 0 degree Check On Error."
//223150 1 A "Barcode Robot Rotator 180 degree Check Off Error."
//223151 1 A "Barcode Robot Rotator 180 degree Check On Error."
CString GetJamCode_BcrRotator_Time( int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", iIdx, iOn );
	return strJamCode;
}

//204140 0 A " "Test-Site Picker Robot Rotator Cylinder On Check Off Error."
//204141 0 A " "Test-Site Picker Robot Rotator Cylinder On Check On Error."
//204150 0 A " "Test-Site Picker Robot Rotator Cylinder Off Check Off Error."
//204151 0 A " "Test-Site Picker Robot Rotator Cylinder Off Check On Error."
CString GetJamCode_Rotator_Time( int iIdx, int iOn )
{//204090 0 A "Test-Site Picker Robot Rotator Cylinder Off Off Error."
 //204091 0 A "Test-Site Picker Robot Rotator Cylinder Off On Error."
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", iIdx, iOn );
	return strJamCode;
}

CString GetJamCode_Rotator_Chk( int iOn )
{
//204140 0 A "Test-Site Picker Robot Rotator Cylinder On Check Off Error."
//204141 0 A "Test-Site Picker Robot Rotator Cylinder On Check On Error."
//204150 0 A "Test-Site Picker Robot Rotator Cylinder Off Check Off Error."
//204151 0 A "Test-Site Picker Robot Rotator Cylinder Off Check On Error."

	CString strJamCode="";
	strJamCode.Format( "20414%d", iOn );
	return strJamCode;
}

CString GetJamCode_SorterPickerDn_Chk( int iIdx, int iOn )
{
//202000 0 A "Sorting Robot Picker #1 Cylinder On/Off Off Error."
//202001 0 A "Sorting Robot Picker #1 Cylinder On/Off On Error."
//202010 0 A "Sorting Robot Picker #2 Cylinder On/Off Off Error."
//202011 0 A "Sorting Robot Picker #2 Cylinder On/Off On Error."
//202020 0 A "Sorting Robot Picker #3 Cylinder On/Off Off Error."
//202021 0 A "Sorting Robot Picker #3 Cylinder On/Off On Error."
//202030 0 A "Sorting Robot Picker #4 Cylinder On/Off Off Error."
//202031 0 A "Sorting Robot Picker #4 Cylinder On/Off On Error."

//202060 0 A "Sorting Robot Picker #1 Cylinder Up Check Off Error."
//202061 0 A "Sorting Robot Picker #1 Cylinder Up Check On Error."
//202070 0 A "Sorting Robot Picker #1 Cylinder Down Check Off Error."
//202071 0 A "Sorting Robot Picker #1 Cylinder Down Check On Error."
//202080 0 A "Sorting Robot Picker #2 Cylinder Up Check Off Error."
//202081 0 A "Sorting Robot Picker #2 Cylinder Up Check On Error."
//202090 0 A "Sorting Robot Picker #2 Cylinder Down Check Off Error."
//202091 0 A "Sorting Robot Picker #2 Cylinder Down Check On Error."
//202100 0 A "Sorting Robot Picker #3 Cylinder Up Check Off Error."
//202101 0 A "Sorting Robot Picker #3 Cylinder Up Check On Error."
//202110 0 A "Sorting Robot Picker #3 Cylinder Down Check Off Error."
//202111 0 A "Sorting Robot Picker #3 Cylinder Down Check On Error."
//202120 0 A "Sorting Robot Picker #4 Cylinder Up Check Off Error."
//202121 0 A "Sorting Robot Picker #4 Cylinder Up Check On Error."
//202130 0 A "Sorting Robot Picker #4 Cylinder Down Check Off Error."
//202131 0 A "Sorting Robot Picker #4 Cylinder Down Check On Error."
	CString strJamCode="";
	strJamCode.Format( "202%02d%d", 7+(2*iIdx), iOn );
	return strJamCode;
}

//210040 0 A " "Test-Site C.O.K Plate #1 Forward Check Off Error."
//210041 0 A " "Test-Site C.O.K Plate #1 Forward Check On Error."
//210050 0 A " "Test-Site C.O.K Plate #1 Backward Check Off Error."
//210051 0 A " "Test-Site C.O.K Plate #1 Backward Check On Error."
//210060 0 A " "Test-Site C.O.K Plate #2 Forward Check Off Error."
//210061 0 A " "Test-Site C.O.K Plate #2 Forward Check On Error."
//210070 0 A " "Test-Site C.O.K Plate #2 Backward Check Off Error."
//210071 0 A " "Test-Site C.O.K Plate #2 Backward Check On Error."
//210080 0 A " "Test-Site C.O.K Plate #3 Forward Check Off Error."
//210081 0 A " "Test-Site C.O.K Plate #3 Forward Check On Error."
//210090 0 A " "Test-Site C.O.K Plate #3 Backward Check Off Error."
//210091 0 A " "Test-Site C.O.K Plate #3 Backward Check On Error."
//210100 0 A " "Test-Site C.O.K Plate #4 Forward Check Off Error."
//210101 0 A " "Test-Site C.O.K Plate #4 Forward Check On Error."
//210110 0 A " "Test-Site C.O.K Plate #4 Backward Check Off Error."
//210111 0 A " "Test-Site C.O.K Plate #4 Backward Check On Error."
//210120 0 A " "Test-Site C.O.K Plate #5 Forward Check Off Error."
//210121 0 A " "Test-Site C.O.K Plate #5 Forward Check On Error."
//210130 0 A " "Test-Site C.O.K Plate #5 Backward Check Off Error."
//210131 0 A " "Test-Site C.O.K Plate #5 Backward Check On Error."
CString GetJamCode_TS_Plate_Chk( int nSite, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", nSite, iOn );
	return strJamCode;
}
//211100 0 A " "Test-Site C.O.K Insert #1- 1 Forward Check Off Error."
//211101 0 A " "Test-Site C.O.K Insert #1- 1 Forward Check On Error."
//211110 0 A " "Test-Site C.O.K Insert #1- 1 Backward Check Off Error."
//211111 0 A " "Test-Site C.O.K Insert #1- 1 Backward Check On Error."
//211120 0 A " "Test-Site C.O.K Insert #1- 2 Forward Check Off Error."
//211121 0 A " "Test-Site C.O.K Insert #1- 2 Forward Check On Error."
//211130 0 A " "Test-Site C.O.K Insert #1- 2 Backward Check Off Error."
//211131 0 A " "Test-Site C.O.K Insert #1- 2 Backward Check On Error."
//211140 0 A " "Test-Site C.O.K Insert #1- 3 Forward Check Off Error."
//211141 0 A " "Test-Site C.O.K Insert #1- 3 Forward Check On Error."
//211150 0 A " "Test-Site C.O.K Insert #1- 3 Backward Check Off Error."
//211151 0 A " "Test-Site C.O.K Insert #1- 3 Backward Check On Error."
//212000 0 A " "Test-Site C.O.K Insert #1- 4 Forward Check Off Error."
//212001 0 A " "Test-Site C.O.K Insert #1- 4 Forward Check On Error."
//212010 0 A " "Test-Site C.O.K Insert #1- 4 Backward Check Off Error."
//212011 0 A " "Test-Site C.O.K Insert #1- 4 Backward Check On Error."
//212020 0 A " "Test-Site C.O.K Insert #1- 5 Forward Check Off Error."
//212021 0 A " "Test-Site C.O.K Insert #1- 5 Forward Check On Error."
//212030 0 A " "Test-Site C.O.K Insert #1- 5 Backward Check Off Error."
//212031 0 A " "Test-Site C.O.K Insert #1- 5 Backward Check On Error."
//212040 0 A " "Test-Site C.O.K Insert #1- 6 Forward Check Off Error."
//212041 0 A " "Test-Site C.O.K Insert #1- 6 Forward Check On Error."
//212050 0 A " "Test-Site C.O.K Insert #1- 6 Backward Check Off Error."
//212051 0 A " "Test-Site C.O.K Insert #1- 6 Backward Check On Error."
//212060 0 A " "Test-Site C.O.K Insert #2- 1 Forward Check Off Error."
//212061 0 A " "Test-Site C.O.K Insert #2- 1 Forward Check On Error."
//212070 0 A " "Test-Site C.O.K Insert #2- 1 Backward Check Off Error."
//212071 0 A " "Test-Site C.O.K Insert #2- 1 Backward Check On Error."
//212080 0 A " "Test-Site C.O.K Insert #2- 2 Forward Check Off Error."
//212081 0 A " "Test-Site C.O.K Insert #2- 2 Forward Check On Error."
//212090 0 A " "Test-Site C.O.K Insert #2- 2 Backward Check Off Error."
//212091 0 A " "Test-Site C.O.K Insert #2- 2 Backward Check On Error."
//212100 0 A " "Test-Site C.O.K Insert #2- 3 Forward Check Off Error."
//212101 0 A " "Test-Site C.O.K Insert #2- 3 Forward Check On Error."
//212110 0 A " "Test-Site C.O.K Insert #2- 3 Backward Check Off Error."
//212111 0 A " "Test-Site C.O.K Insert #2- 3 Backward Check On Error."
//212120 0 A " "Test-Site C.O.K Insert #2- 4 Forward Check Off Error."
//212121 0 A " "Test-Site C.O.K Insert #2- 4 Forward Check On Error."
//212130 0 A " "Test-Site C.O.K Insert #2- 4 Backward Check Off Error."
//212131 0 A " "Test-Site C.O.K Insert #2- 4 Backward Check On Error."
//212140 0 A " "Test-Site C.O.K Insert #2- 5 Forward Check Off Error."
//212141 0 A " "Test-Site C.O.K Insert #2- 5 Forward Check On Error."
//212150 0 A " "Test-Site C.O.K Insert #2- 5 Backward Check Off Error."
//212151 0 A " "Test-Site C.O.K Insert #2- 5 Backward Check On Error."
//213000 0 A " "Test-Site C.O.K Insert #2- 6 Forward Check Off Error."
//213001 0 A " "Test-Site C.O.K Insert #2- 6 Forward Check On Error."
//213010 0 A " "Test-Site C.O.K Insert #2- 6 Backward Check Off Error."
//213011 0 A " "Test-Site C.O.K Insert #2- 6 Backward Check On Error."
//213020 0 A " "Test-Site C.O.K Insert #3- 1 Forward Check Off Error."
//213021 0 A " "Test-Site C.O.K Insert #3- 1 Forward Check On Error."
//213030 0 A " "Test-Site C.O.K Insert #3- 1 Backward Check Off Error."
//213031 0 A " "Test-Site C.O.K Insert #3- 1 Backward Check On Error."
//213040 0 A " "Test-Site C.O.K Insert #3- 2 Forward Check Off Error."
//213041 0 A " "Test-Site C.O.K Insert #3- 2 Forward Check On Error."
//213050 0 A " "Test-Site C.O.K Insert #3- 2 Backward Check Off Error."
//213051 0 A " "Test-Site C.O.K Insert #3- 2 Backward Check On Error."
//213060 0 A " "Test-Site C.O.K Insert #3- 3 Forward Check Off Error."
//213061 0 A " "Test-Site C.O.K Insert #3- 3 Forward Check On Error."
//213070 0 A " "Test-Site C.O.K Insert #3- 3 Backward Check Off Error."
//213071 0 A " "Test-Site C.O.K Insert #3- 3 Backward Check On Error."
//213080 0 A " "Test-Site C.O.K Insert #3- 4 Forward Check Off Error."
//213081 0 A " "Test-Site C.O.K Insert #3- 4 Forward Check On Error."
//213090 0 A " "Test-Site C.O.K Insert #3- 4 Backward Check Off Error."
//213091 0 A " "Test-Site C.O.K Insert #3- 4 Backward Check On Error."
//213100 0 A " "Test-Site C.O.K Insert #3- 5 Forward Check Off Error."
//213101 0 A " "Test-Site C.O.K Insert #3- 5 Forward Check On Error."
//213110 0 A " "Test-Site C.O.K Insert #3- 5 Backward Check Off Error."
//213111 0 A " "Test-Site C.O.K Insert #3- 5 Backward Check On Error."
//213120 0 A " "Test-Site C.O.K Insert #3- 6 Forward Check Off Error."
//213121 0 A " "Test-Site C.O.K Insert #3- 6 Forward Check On Error."
//213130 0 A " "Test-Site C.O.K Insert #3- 6 Backward Check Off Error."
//213131 0 A " "Test-Site C.O.K Insert #3- 6 Backward Check On Error."
//213140 0 A " "Test-Site C.O.K Insert #4- 1 Forward Check Off Error."
//213141 0 A " "Test-Site C.O.K Insert #4- 1 Forward Check On Error."
//213150 0 A " "Test-Site C.O.K Insert #4- 1 Backward Check Off Error."
//213151 0 A " "Test-Site C.O.K Insert #4- 1 Backward Check On Error."
//214000 0 A " "Test-Site C.O.K Insert #4- 2 Forward Check Off Error."
//214001 0 A " "Test-Site C.O.K Insert #4- 2 Forward Check On Error."
//214010 0 A " "Test-Site C.O.K Insert #4- 2 Backward Check Off Error."
//214011 0 A " "Test-Site C.O.K Insert #4- 2 Backward Check On Error."
//214020 0 A " "Test-Site C.O.K Insert #4- 3 Forward Check Off Error."
//214021 0 A " "Test-Site C.O.K Insert #4- 3 Forward Check On Error."
//214030 0 A " "Test-Site C.O.K Insert #4- 3 Backward Check Off Error."
//214031 0 A " "Test-Site C.O.K Insert #4- 3 Backward Check On Error."
//214040 0 A " "Test-Site C.O.K Insert #4- 4 Forward Check Off Error."
//214041 0 A " "Test-Site C.O.K Insert #4- 4 Forward Check On Error."
//214050 0 A " "Test-Site C.O.K Insert #4- 4 Backward Check Off Error."
//214051 0 A " "Test-Site C.O.K Insert #4- 4 Backward Check On Error."
//214060 0 A " "Test-Site C.O.K Insert #4- 5 Forward Check Off Error."
//214061 0 A " "Test-Site C.O.K Insert #4- 5 Forward Check On Error."
//214070 0 A " "Test-Site C.O.K Insert #4- 5 Backward Check Off Error."
//214071 0 A " "Test-Site C.O.K Insert #4- 5 Backward Check On Error."
//214080 0 A " "Test-Site C.O.K Insert #4- 6 Forward Check Off Error."
//214081 0 A " "Test-Site C.O.K Insert #4- 6 Forward Check On Error."
//214090 0 A " "Test-Site C.O.K Insert #4- 6 Backward Check Off Error."
//214091 0 A " "Test-Site C.O.K Insert #4- 6 Backward Check On Error."
//214100 0 A " "Test-Site C.O.K Insert #5- 1 Forward Check Off Error."
//214101 0 A " "Test-Site C.O.K Insert #5- 1 Forward Check On Error."
//214110 0 A " "Test-Site C.O.K Insert #5- 1 Backward Check Off Error."
//214111 0 A " "Test-Site C.O.K Insert #5- 1 Backward Check On Error."
//214120 0 A " "Test-Site C.O.K Insert #5- 2 Forward Check Off Error."
//214121 0 A " "Test-Site C.O.K Insert #5- 2 Forward Check On Error."
//214130 0 A " "Test-Site C.O.K Insert #5- 2 Backward Check Off Error."
//214131 0 A " "Test-Site C.O.K Insert #5- 2 Backward Check On Error."
//214140 0 A " "Test-Site C.O.K Insert #5- 3 Forward Check Off Error."
//214141 0 A " "Test-Site C.O.K Insert #5- 3 Forward Check On Error."
//214150 0 A " "Test-Site C.O.K Insert #5- 3 Backward Check Off Error."
//214151 0 A " "Test-Site C.O.K Insert #5- 3 Backward Check On Error."
//215000 0 A " "Test-Site C.O.K Insert #5- 4 Forward Check Off Error."
//215001 0 A " "Test-Site C.O.K Insert #5- 4 Forward Check On Error."
//215010 0 A " "Test-Site C.O.K Insert #5- 4 Backward Check Off Error."
//215011 0 A " "Test-Site C.O.K Insert #5- 4 Backward Check On Error."
//215020 0 A " "Test-Site C.O.K Insert #5- 5 Forward Check Off Error."
//215021 0 A " "Test-Site C.O.K Insert #5- 5 Forward Check On Error."
//215030 0 A " "Test-Site C.O.K Insert #5- 5 Backward Check Off Error."
//215031 0 A " "Test-Site C.O.K Insert #5- 5 Backward Check On Error."
//215040 0 A " "Test-Site C.O.K Insert #5- 6 Forward Check Off Error."
//215041 0 A " "Test-Site C.O.K Insert #5- 6 Forward Check On Error."
//215050 0 A " "Test-Site C.O.K Insert #5- 6 Backward Check Off Error."
//215051 0 A " "Test-Site C.O.K Insert #5- 6 Backward Check On Error."
CString GetJamCode_TS_Insert_Chk( int nSite, int iIdx, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", iIdx, iOn );
	return strJamCode;
}
//211000 0 A " "Test-Site C.O.K Lock #1 Check Off Error."
//211001 0 A " "Test-Site C.O.K Lock #1 Check On Error."
//211010 0 A " "Test-Site C.O.K UnLock #1 Check Off Error."
//211011 0 A " "Test-Site C.O.K UnLock #1 Check On Error."
//211020 0 A " "Test-Site C.O.K Lock #2 Check Off Error."
//211021 0 A " "Test-Site C.O.K Lock #2 Check On Error."
//211030 0 A " "Test-Site C.O.K UnLock #2 Check Off Error."
//211031 0 A " "Test-Site C.O.K UnLock #2 Check On Error."
//211040 0 A " "Test-Site C.O.K Lock #3 Check Off Error."
//211041 0 A " "Test-Site C.O.K Lock #3 Check On Error."
//211050 0 A " "Test-Site C.O.K UnLock #3 Check Off Error."
//211051 0 A " "Test-Site C.O.K UnLock #3 Check On Error."
//211060 0 A " "Test-Site C.O.K Lock #4 Check Off Error."
//211061 0 A " "Test-Site C.O.K Lock #4 Check On Error."
//211070 0 A " "Test-Site C.O.K UnLock #4 Check Off Error."
//211071 0 A " "Test-Site C.O.K UnLock #4 Check On Error."
//211080 0 A " "Test-Site C.O.K Lock #5 Check Off Error."
//211081 0 A " "Test-Site C.O.K Lock #5 Check On Error."
//211090 0 A " "Test-Site C.O.K UnLock #5 Check Off Error."
//211091 0 A " "Test-Site C.O.K UnLock #5 Check On Error."
CString GetJamCode_TS_Lock_Chk( int nSite, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", nSite, iOn );
	return strJamCode;
}
//210000 0 A " "Hifix Left Clamp Check Off Error."
//210001 0 A " "Hifix Left Clamp Check On Error."
//210010 0 A " "Hifix Left UnClamp Check Off Error."
//210011 0 A " "Hifix Left UnClamp Check On Error."
//210020 0 A " "Hifix Right Clamp Check Off Error."
//210021 0 A " "Hifix Right Clamp Check On Error."
//210030 0 A " "Hifix Right UnClamp Check Off Error."
//210031 0 A " "Hifix Right UnClamp Check On Error."
CString GetJamCode_HF_Clamp_Chk( int nSite, int iOn )
{
	CString strJamCode="";
// 	strJamCode.Format( "C0F%d0%d", nSite+1, iOn );
	strJamCode.Format( "2%04d%d", nSite, iOn );
	return strJamCode;
}
//206010 0 A " "Test-Site C.O.K Insert #1- 1 Forward/Backward On/Off Off Error."
//206011 0 A " "Test-Site C.O.K Insert #1- 1 Forward/Backward On/Off On Error."
//206020 0 A " "Test-Site C.O.K Insert #1- 2 Forward/Backward On/Off Off Error."
//206021 0 A " "Test-Site C.O.K Insert #1- 2 Forward/Backward On/Off On Error."
//206030 0 A " "Test-Site C.O.K Insert #1- 3 Forward/Backward On/Off Off Error."
//206031 0 A " "Test-Site C.O.K Insert #1- 3 Forward/Backward On/Off On Error."
//206040 0 A " "Test-Site C.O.K Insert #1- 4 Forward/Backward On/Off Off Error."
//206041 0 A " "Test-Site C.O.K Insert #1- 4 Forward/Backward On/Off On Error."
//206050 0 A " "Test-Site C.O.K Insert #1- 5 Forward/Backward On/Off Off Error."
//206051 0 A " "Test-Site C.O.K Insert #1- 5 Forward/Backward On/Off On Error."
//206060 0 A " "Test-Site C.O.K Insert #1- 6 Forward/Backward On/Off Off Error."
//206061 0 A " "Test-Site C.O.K Insert #1- 6 Forward/Backward On/Off On Error."
//206070 0 A " "Test-Site C.O.K Insert #2- 1 Forward/Backward On/Off Off Error."
//206071 0 A " "Test-Site C.O.K Insert #2- 1 Forward/Backward On/Off On Error."
//206080 0 A " "Test-Site C.O.K Insert #2- 2 Forward/Backward On/Off Off Error."
//206081 0 A " "Test-Site C.O.K Insert #2- 2 Forward/Backward On/Off On Error."
//206090 0 A " "Test-Site C.O.K Insert #2- 3 Forward/Backward On/Off Off Error."
//206091 0 A " "Test-Site C.O.K Insert #2- 3 Forward/Backward On/Off On Error."
//206100 0 A " "Test-Site C.O.K Insert #2- 4 Forward/Backward On/Off Off Error."
//206101 0 A " "Test-Site C.O.K Insert #2- 4 Forward/Backward On/Off On Error."
//206110 0 A " "Test-Site C.O.K Insert #2- 5 Forward/Backward On/Off Off Error."
//206111 0 A " "Test-Site C.O.K Insert #2- 5 Forward/Backward On/Off On Error."
//206120 0 A " "Test-Site C.O.K Insert #2- 6 Forward/Backward On/Off Off Error."
//206121 0 A " "Test-Site C.O.K Insert #2- 6 Forward/Backward On/Off On Error."
//206130 0 A " "Test-Site C.O.K Insert #3- 1 Forward/Backward On/Off Off Error."
//206131 0 A " "Test-Site C.O.K Insert #3- 1 Forward/Backward On/Off On Error."
//206140 0 A " "Test-Site C.O.K Insert #3- 2 Forward/Backward On/Off Off Error."
//206141 0 A " "Test-Site C.O.K Insert #3- 2 Forward/Backward On/Off On Error."
//206150 0 A " "Test-Site C.O.K Insert #3- 3 Forward/Backward On/Off Off Error."
//206151 0 A " "Test-Site C.O.K Insert #3- 3 Forward/Backward On/Off On Error."
//207000 0 A " "Test-Site C.O.K Insert #3- 4 Forward/Backward On/Off Off Error."
//207001 0 A " "Test-Site C.O.K Insert #3- 4 Forward/Backward On/Off On Error."
//207010 0 A " "Test-Site C.O.K Insert #3- 5 Forward/Backward On/Off Off Error."
//207011 0 A " "Test-Site C.O.K Insert #3- 5 Forward/Backward On/Off On Error."
//207020 0 A " "Test-Site C.O.K Insert #3- 6 Forward/Backward On/Off Off Error."
//207021 0 A " "Test-Site C.O.K Insert #3- 6 Forward/Backward On/Off On Error."
//207030 0 A " "Test-Site C.O.K Insert #4- 1 Forward/Backward On/Off Off Error."
//207031 0 A " "Test-Site C.O.K Insert #4- 1 Forward/Backward On/Off On Error."
//207040 0 A " "Test-Site C.O.K Insert #4- 2 Forward/Backward On/Off Off Error."
//207041 0 A " "Test-Site C.O.K Insert #4- 2 Forward/Backward On/Off On Error."
//207050 0 A " "Test-Site C.O.K Insert #4- 3 Forward/Backward On/Off Off Error."
//207051 0 A " "Test-Site C.O.K Insert #4- 3 Forward/Backward On/Off On Error."
//207060 0 A " "Test-Site C.O.K Insert #4- 4 Forward/Backward On/Off Off Error."
//207061 0 A " "Test-Site C.O.K Insert #4- 4 Forward/Backward On/Off On Error."
//207070 0 A " "Test-Site C.O.K Insert #4- 5 Forward/Backward On/Off Off Error."
//207071 0 A " "Test-Site C.O.K Insert #4- 5 Forward/Backward On/Off On Error."
//207080 0 A " "Test-Site C.O.K Insert #4- 6 Forward/Backward On/Off Off Error."
//207081 0 A " "Test-Site C.O.K Insert #4- 6 Forward/Backward On/Off On Error."
//207090 0 A " "Test-Site C.O.K Insert #5- 1 Forward/Backward On/Off Off Error."
//207091 0 A " "Test-Site C.O.K Insert #5- 1 Forward/Backward On/Off On Error."
//207100 0 A " "Test-Site C.O.K Insert #5- 2 Forward/Backward On/Off Off Error."
//207101 0 A " "Test-Site C.O.K Insert #5- 2 Forward/Backward On/Off On Error."
//207110 0 A " "Test-Site C.O.K Insert #5- 3 Forward/Backward On/Off Off Error."
//207111 0 A " "Test-Site C.O.K Insert #5- 3 Forward/Backward On/Off On Error."
//207120 0 A " "Test-Site C.O.K Insert #5- 4 Forward/Backward On/Off Off Error."
//207121 0 A " "Test-Site C.O.K Insert #5- 4 Forward/Backward On/Off On Error."
//207130 0 A " "Test-Site C.O.K Insert #5- 5 Forward/Backward On/Off Off Error."
//207131 0 A " "Test-Site C.O.K Insert #5- 5 Forward/Backward On/Off On Error."
//207140 0 A " "Test-Site C.O.K Insert #5- 6 Forward/Backward On/Off Off Error."
//207141 0 A " "Test-Site C.O.K Insert #5- 6 Forward/Backward On/Off On Error."
CString GetJamCode_TS_Insert_Time( int nSite, int iIdx, int iOn )
{
	CString strJamCode="";
//	if(nSite < 3 && iIdx < 2)
//	{
//		strJamCode.Format( "206%02d%d", (nSite * 6 + 1) + iIdx, iOn );
//	}
//	else if(nSite < 3 && iIdx < 3)
//	{
//		strJamCode.Format( "207%02d%d", iIdx, iOn );
//	}
//	else
//	{
//		strJamCode.Format( "207%02d%d", ((nSite-3)*6 + 3) + iIdx, iOn );
//	}
	strJamCode.Format( "2%04d%d", iIdx, iOn );
	return strJamCode;
}
//210040 0 A " "Test-Site C.O.K Plate #1 Forward Check Off Error."
//210041 0 A " "Test-Site C.O.K Plate #1 Forward Check On Error."
//210050 0 A " "Test-Site C.O.K Plate #1 Backward Check Off Error."
//210051 0 A " "Test-Site C.O.K Plate #1 Backward Check On Error."
//210060 0 A " "Test-Site C.O.K Plate #2 Forward Check Off Error."
//210061 0 A " "Test-Site C.O.K Plate #2 Forward Check On Error."
//210070 0 A " "Test-Site C.O.K Plate #2 Backward Check Off Error."
//210071 0 A " "Test-Site C.O.K Plate #2 Backward Check On Error."
//210080 0 A " "Test-Site C.O.K Plate #3 Forward Check Off Error."
//210081 0 A " "Test-Site C.O.K Plate #3 Forward Check On Error."
//210090 0 A " "Test-Site C.O.K Plate #3 Backward Check Off Error."
//210091 0 A " "Test-Site C.O.K Plate #3 Backward Check On Error."
//210100 0 A " "Test-Site C.O.K Plate #4 Forward Check Off Error."
//210101 0 A " "Test-Site C.O.K Plate #4 Forward Check On Error."
//210110 0 A " "Test-Site C.O.K Plate #4 Backward Check Off Error."
//210111 0 A " "Test-Site C.O.K Plate #4 Backward Check On Error."
//210120 0 A " "Test-Site C.O.K Plate #5 Forward Check Off Error."
//210121 0 A " "Test-Site C.O.K Plate #5 Forward Check On Error."
//210130 0 A " "Test-Site C.O.K Plate #5 Backward Check Off Error."
//210131 0 A " "Test-Site C.O.K Plate #5 Backward Check On Error."
CString GetJamCode_TS_Plate_Time( int nSite, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", nSite, iOn );
	return strJamCode;
}
//211000 0 A " "Test-Site C.O.K Lock #1 Check Off Error."
//211001 0 A " "Test-Site C.O.K Lock #1 Check On Error."
//211010 0 A " "Test-Site C.O.K UnLock #1 Check Off Error."
//211011 0 A " "Test-Site C.O.K UnLock #1 Check On Error."
//211020 0 A " "Test-Site C.O.K Lock #2 Check Off Error."
//211021 0 A " "Test-Site C.O.K Lock #2 Check On Error."
//211030 0 A " "Test-Site C.O.K UnLock #2 Check Off Error."
//211031 0 A " "Test-Site C.O.K UnLock #2 Check On Error."
//211040 0 A " "Test-Site C.O.K Lock #3 Check Off Error."
//211041 0 A " "Test-Site C.O.K Lock #3 Check On Error."
//211050 0 A " "Test-Site C.O.K UnLock #3 Check Off Error."
//211051 0 A " "Test-Site C.O.K UnLock #3 Check On Error."
//211060 0 A " "Test-Site C.O.K Lock #4 Check Off Error."
//211061 0 A " "Test-Site C.O.K Lock #4 Check On Error."
//211070 0 A " "Test-Site C.O.K UnLock #4 Check Off Error."
//211071 0 A " "Test-Site C.O.K UnLock #4 Check On Error."
//211080 0 A " "Test-Site C.O.K Lock #5 Check Off Error."
//211081 0 A " "Test-Site C.O.K Lock #5 Check On Error."
//211090 0 A " "Test-Site C.O.K UnLock #5 Check Off Error."
//211091 0 A " "Test-Site C.O.K UnLock #5 Check On Error."
CString GetJamCode_TS_Lock_Time( int nSite, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2%04d%d", nSite+1, iOn );
	return strJamCode;
}

CString GetJamCode_SorterModuleExist_Chk( int nPicker, int iOn )
{
//201090 0 A "Sorting Robot Picker #1 Vacuum Check Off Error."
//201091 0 A "Sorting Robot Picker #1 Vacuum Check On Error."
//201100 0 A "Sorting Robot Picker #2 Vacuum Check Off Error."
//201101 0 A "Sorting Robot Picker #2 Vacuum Check On Error."
//201110 0 A "Sorting Robot Picker #3 Vacuum Check Off Error."
//201111 0 A "Sorting Robot Picker #3 Vacuum Check On Error."
//201120 0 A "Sorting Robot Picker #4 Vacuum Check Off Error."
//201121 0 A "Sorting Robot Picker #4 Vacuum Check On Error."
	CString strJamCode="";
	strJamCode.Format( "201%02d%d", nPicker+9, iOn );
	return strJamCode;
}
//204100 0 A " "Test-Site Picker Robot #1 Vacuum Check Off Error."
//204101 0 A " "Test-Site Picker Robot #1 Vacuum Check On Error."
//204110 0 A " "Test-Site Picker Robot #2 Vacuum Check Off Error."
//204111 0 A " "Test-Site Picker Robot #2 Vacuum Check On Error."
//204120 0 A " "Test-Site Picker Robot #3 Vacuum Check Off Error."
//204121 0 A " "Test-Site Picker Robot #3 Vacuum Check On Error."
//204130 0 A " "Test-Site Picker Robot #4 Vacuum Check Off Error."
//204131 0 A " "Test-Site Picker Robot #4 Vacuum Check On Error."
CString GetJamCode_ModuleExist_Chk( int nPicker, int iOn )
{
	CString strJamCode="";
	strJamCode.Format( "2041%d%d", nPicker, iOn );
	return strJamCode;
}