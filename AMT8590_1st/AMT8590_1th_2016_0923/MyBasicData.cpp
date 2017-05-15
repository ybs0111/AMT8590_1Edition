// MyBasicData.cpp: implementation of the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "MyBasicData.h"
#include "io.h"
#include "math.h"
#include "ComizoaPublic.h"

#include "SrcPart/APartHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBasicData::CMyBasicData()
{

}

CMyBasicData::~CMyBasicData()
{

}

void CMyBasicData::OnPassWord_Load(CString str_load_level)
{
	CString str_pass="";
	char chr_data[50];						// 암호는 25글자 이내로 제한

	(st_msg.mstr_password).Empty();		// 암호 저장 변수 초기화

	if (str_load_level == "Lock")			// SYSTEM LOCK 암호
		:: GetPrivateProfileString("Password", "SysLock", "M", chr_data, 50, st_path.mstr_basic);
	else if (str_load_level == "Level1")	// 메인트 암호
		:: GetPrivateProfileString("Password", "Level_1", "M", chr_data, 50, st_path.mstr_basic);
	else if (str_load_level == "Level2")	// 티칭 암호
		:: GetPrivateProfileString("Password", "Level_2", "T", chr_data, 50, st_path.mstr_basic);
	else  
		return;

	str_pass = chr_data;
	str_pass.MakeUpper();     // 문자열 대문자로 변경
	str_pass.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	str_pass.TrimRight(' ');  // 우측 문자열 공백 제거

	st_msg.mstr_password = str_pass;  // 암호 전역 변수에 설정
}

void CMyBasicData::OnPassWord_Save(CString str_save_level, CString str_pass_data)
{
	if (str_save_level=="Lock")         // SYSTEM LOCK 암호
		:: WritePrivateProfileString("Password", "SysLock", LPCTSTR(str_pass_data), st_path.mstr_basic);
	else if (str_save_level=="Level1")  // 메인트 암호
		:: WritePrivateProfileString("Password", "Level_1", LPCTSTR(str_pass_data), st_path.mstr_basic);
	else if (str_save_level=="Level2")  // 티칭 암호
		:: WritePrivateProfileString("Password", "Level_2", LPCTSTR(str_pass_data), st_path.mstr_basic);
}

CString CMyBasicData::OnStep_File_Index_Load()
{
	CString str_file_index="";  // 파일 인덱스 저장 변수

	char chr_load[20];

	:: GetPrivateProfileString("Thread_Step_file", "File_Index", "00", chr_load, 20, st_path.mstr_basic);
	str_file_index = chr_load;
	str_file_index.TrimLeft(' ');               
	str_file_index.TrimRight(' ');

	return str_file_index;  // 파일 인덱스 리턴
}

void CMyBasicData::OnStep_File_Index_Save(CString str_index)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Index", LPCTSTR(str_index), st_path.mstr_basic);
}

CString CMyBasicData::OnStep_File_Name_Load()
{
	CString str_file_name="";  // 파일명 저장 변수

	char chr_load[20];

	:: GetPrivateProfileString("Thread_Step_file", "File_Name", "DEFAULT", chr_load, 20, st_path.mstr_basic);
	str_file_name = chr_load;
	str_file_name.TrimLeft(' ');               
	str_file_name.TrimRight(' ');

	return str_file_name;  // 파일명 리턴
}

void CMyBasicData::OnStep_File_Name_Save(CString str_file)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Name", LPCTSTR(str_file), st_path.mstr_basic);
}

void CMyBasicData::OnMotorSpeed_Set_Data_Load()
{
	int  n_chk, i = 0, j = 0;
	CString str_temp="", str_part="";       // 임시 저장 변수
	CString str_motor_name="";

	char chr_data[20];

	CString str_msg="", str_LoadFile="";       // 임시 저장 변수
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = OnGet_Teach_File_Name("Speed");  // 데이터 저장 파일명 로딩 함수 
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
//				st_msg.mstr_abnormal_msg.Format("[%s] file open error.", str_LoadFile);
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] file is not exist.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	for (i = 0; i < MOTOR_COUNT; i++)
	{
		fread(&COMI.md_spd_vel[i][0],		sizeof(COMI.md_spd_vel[i][0]), 1, fp);
		fread(&COMI.md_spd_vel[i][1],		sizeof(COMI.md_spd_vel[i][1]), 1, fp);
		fread(&COMI.md_spd_vel[i][2],		sizeof(COMI.md_spd_vel[i][2]), 1, fp);
		fread(&COMI.md_spd_home[i],			sizeof(COMI.md_spd_home[i]), 1, fp);
		fread(&COMI.md_spd_jog[i],			sizeof(COMI.md_spd_jog[i]), 1, fp);
		fread(&COMI.md_allow_value[i],		sizeof(COMI.md_allow_value[i]), 1, fp);
		
		if(CHINA_VER == 1)
		{
			fread(&COMI.md_limit_position[i][0],		sizeof(COMI.md_limit_position[i][0]), 1, fp);//20131029
			fread(&COMI.md_limit_position[i][1],		sizeof(COMI.md_limit_position[i][1]), 1, fp);
		}
		else
		{
			fread(&COMI.md_limit_position[i][0],		sizeof(COMI.md_limit_position[i][0]), 1, fp);//20131029
			fread(&COMI.md_limit_position[i][1],		sizeof(COMI.md_limit_position[i][1]), 1, fp);
			
		}
	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Read Error.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			clearerr(fp);
		}
	}

	fclose(fp) ;

	//Speed Rate
	:: GetPrivateProfileString("Speed_Rate", "Run", "100", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	COMI.mn_runspeed_rate = (double)n_chk;

	:: GetPrivateProfileString("Speed_Rate", "Manual", "80", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	COMI.mn_manualspeed_rate = (double)n_chk;
}

void CMyBasicData::OnMotorSpeed_Set_Data_Save()
{
	int i = 0, j = 0;
	CString str_temp="", str_part="";       // 임시 저장 변수
	CString str_motor_name="";
	char chr_buf[20];
	
	CString str_SaveFile;			// 임시 저장 변수
	
	str_SaveFile = OnGet_Teach_File_Name("Speed");		// 데이터 저장 파일명 로딩 함수 
	
	FILE  *fp;
	int   err = 0;
	
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("Fail in Creating [%s] FILE.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}

	for (i = 0; i < MOTOR_COUNT; i++)
	{
// 		fwrite(&st_motor[i].md_spd_acc,		sizeof(st_motor[i].md_spd_acc), 1, fp);
// 		fwrite(&st_motor[i].md_spd_vel,		sizeof(st_motor[i].md_spd_vel), 1, fp);
// 		fwrite(&st_motor[i].md_spd_dec,		sizeof(st_motor[i].md_spd_dec), 1, fp);
// 		fwrite(&st_motor[i].md_spd_home,	sizeof(st_motor[i].md_spd_home), 1, fp);
// 		fwrite(&st_motor[i].md_spd_jog,		sizeof(st_motor[i].md_spd_jog), 1, fp);
// 		fwrite(&st_motor[i].mn_allow,		sizeof(st_motor[i].mn_allow), 1, fp);

		fwrite(&COMI.md_spd_vel[i][0],		sizeof(COMI.md_spd_vel[i][0]), 1, fp);
		fwrite(&COMI.md_spd_vel[i][1],		sizeof(COMI.md_spd_vel[i][1]), 1, fp);
		fwrite(&COMI.md_spd_vel[i][2],		sizeof(COMI.md_spd_vel[i][2]), 1, fp);
		fwrite(&COMI.md_spd_home[i],		sizeof(COMI.md_spd_home[i]), 1, fp);
		fwrite(&COMI.md_spd_jog[i],			sizeof(COMI.md_spd_jog[i]), 1, fp);
		fwrite(&COMI.md_allow_value[i],		sizeof(COMI.md_allow_value[i]), 1, fp);

// 		if(CHINA_VER == 1)//20131029
// 		{
			fwrite(&COMI.md_limit_position[i][0],		sizeof(COMI.md_limit_position[i][0]), 1, fp);
			fwrite(&COMI.md_limit_position[i][1],		sizeof(COMI.md_limit_position[i][1]), 1, fp);
// 		}
	}

	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Write Error.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);
	
	// Speed Rate
	str_temp = LPCTSTR(_itoa((int)(COMI.mn_runspeed_rate), chr_buf, 10));
	:: WritePrivateProfileString("Speed_Rate", "Run", LPCTSTR(str_temp), st_path.mstr_basic);
	
	str_temp = LPCTSTR(_itoa((int)(COMI.mn_manualspeed_rate), chr_buf, 10));
	:: WritePrivateProfileString("Speed_Rate", "Manual", LPCTSTR(str_temp), st_path.mstr_basic);
}

void CMyBasicData::OnWaitTime_Data_Load()
{
	CString str_temp="";       // 임시 저장 변수
	CString str_motor_name="";
	
	CString str_msg="", str_LoadFile="";       // 임시 저장 변수
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = OnGet_Teach_File_Name("Time");  // 데이터 저장 파일명 로딩 함수 
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
//				st_msg.mstr_abnormal_msg.Format("[%s] file open error.", str_LoadFile);
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] file is not exist.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	fread(&st_time, sizeof(st_time), 1, fp);
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Read Error.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			clearerr(fp);
		}
	}

	fclose(fp) ;
}

void CMyBasicData::OnWaitTime_Data_Save()
{
	CString str_temp="";       // 임시 저장 변수
	CString str_motor_name="";
	
	CString str_SaveFile="";			// 임시 저장 변수
	
	str_SaveFile = OnGet_Teach_File_Name("Time");		// 데이터 저장 파일명 로딩 함수 
	
	FILE  *fp;
	int   err = 0;
	
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("Fail in Creating [%s] FILE.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	fwrite(&st_time, sizeof(st_time), 1, fp);
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Write Error.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);
}

void CMyBasicData::OnMaintenance_Data_Load()
{
	CString str_temp="", str_part="";  // 로딩 정보 임시 저장 변수
	int n_chk;
	CString strLoadFile="";

	char chr_data[20], chr_buf[20] ;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 로딩하여 전역 변수에 설정한다                           */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Red", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[0] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[1] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[2] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[7] = n_chk;
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 로딩하여 전역 변수에 설정한다                        */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Yellow", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[0] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[1] = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[2] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 로딩하여 전역 변수에 설정한다                         */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Green", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[0] = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[1] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[2] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_buzzer_mode", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>1)
	{
		st_lamp.mn_buzzer_mode = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_buzzer_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_buzzer_mode = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "str_equip_no", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_no = chr_data;
	(st_lamp.mstr_equip_no).TrimLeft(' ');               
	(st_lamp.mstr_equip_no).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_id = chr_data;
	(st_lamp.mstr_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_equip_id).TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_on_time_w", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 1)
	{
		st_lamp.mn_lamp_wait_time = 500;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_wait_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_wait_time = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 변경 시간 로딩하여 전역 변수에 설정                              */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_change_time", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 0)
	{
		st_lamp.mn_lamp_change_time = 5;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_change_time = n_chk;
	/* ************************************************************************** */
	st_lamp.mn_lamp_change_time = 5;
}

void CMyBasicData::OnMaintenance_Data_Save()
{
	CString str_temp="", str_part="";  // 로딩 정보 임시 저장 변수
	char chr_buf[20] ;
	CString strSaveFile="";

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 정보를 파일에 저장한다                                  */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 정보를 파일에 저장한다                               */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 정보를 파일에 저장한다                                */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_buzzer_mode, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: WritePrivateProfileString("TowerLampData", "str_equip_no", LPCTSTR(st_lamp.mstr_equip_no), st_path.mstr_basic);
	:: WritePrivateProfileString("TowerLampData", "str_equip_code", LPCTSTR(st_lamp.mstr_equip_id), st_path.mstr_basic);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);

}

void CMyBasicData::OnBasic_Data_Load()
{
	CString str_load_device="";	// 로딩 디바이스명 저장 변수
	CString str_load_file="";
	CString str_chk_ext="";		// 파일 확장자 저장 변수
	CString mstr_temp="", str_part="", str_station="", str_board="";			// 저장할 정보 임시 저장 변수 
	char chr_data[50], chr_buf[20];
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정

	n_pos = str_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
	{
		str_load_file += _T(".TXT");  // 확장자 추가
	}
	else 
	{
		str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}

	/* ************************************************************************** */

	:: GetPrivateProfileString("BasicData", "Run_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_mode_run = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_run, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Run_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_mode_run = mn_chk;

	:: GetPrivateProfileString("BasicData", "Device_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_device = 1;
		mstr_temp.Format("%d", st_basic.n_mode_device);
		:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_device = mn_chk;

	ReadBasicData_Int( st_basic.mn_bypass_mode, "Bypass_Mode", 0, 0, 3, st_path.mstr_basic );

	:: GetPrivateProfileString("BasicData", "Electric_Check", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 1440)
	{
		st_basic.mn_electrostatic_check_gap = 120;
		mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
		:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_electrostatic_check_gap = mn_chk;

	:: GetPrivateProfileString("BasicData", "Electrostatic_Value", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_electrostatic_value = 120;
		mstr_temp.Format("%d", st_basic.md_electrostatic_value);
		:: WritePrivateProfileString("BasicData", "Electrostatic_Value", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_electrostatic_value = md_chk;

	:: GetPrivateProfileString("BasicData", "Run_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.nRunSpeed = 100;
		mstr_temp.Format("%d", st_basic.nRunSpeed);
		:: WritePrivateProfileString("BasicData", "Run_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nRunSpeed = mn_chk;

	:: GetPrivateProfileString("BasicData", "Manual_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.nManualSpeed = 60;
		mstr_temp.Format("%d", st_basic.nManualSpeed);
		:: WritePrivateProfileString("BasicData", "Manual_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nManualSpeed = mn_chk;

	// Data Logger에 있는 하루의 시작 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY START TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_start_time = 22;
	}
	else  st_basic.n_day_start_time = mn_chk;
	st_basic.n_day_start_time = 22;

	// Data Logger에 있는 Shift 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY SHIFT TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_shift_time = 8;
	}
	else  st_basic.n_day_shift_time = mn_chk;
	st_basic.n_day_shift_time = 8;

	:: GetPrivateProfileString("BasicData", "Day_Alarm_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		alarm.mn_alarm_occured_day = 0;
		mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  alarm.mn_alarm_occured_day = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Run_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDR = 0;
	}
	else  st_handler.m_tDR = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Daily_Idle_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDI = 0;
	}
	else  st_handler.m_tDI = mn_chk;

	
	:: GetPrivateProfileString("BasicData", "Daily_JobChange_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDC = 0;
	}
	else  st_handler.m_tDC = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Stop_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDS = 0;
	}
	else  st_handler.m_tDS = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Alarm_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDJ = 0;
	}
	else  st_handler.m_tDJ = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Maint_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDM = 0;
	}
	else  st_handler.m_tDM = mn_chk;

	:: GetPrivateProfileString("Interface", "ECSERVER_IP", "109.117.11.29", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	
//	Run_Network.m_Net_ECServer.SetIP( mstr_temp );
	
	:: GetPrivateProfileString("Interface", "ECSERVER_Port", "12108", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	
//	Run_Network.m_Net_ECServer.SetPort( atoi( (LPCSTR)mstr_temp ) );

	:: GetPrivateProfileString("BasicData", "Network_Wait_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 60)
	{
		st_basic.mn_network_wait_time = 20;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_wait_time, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Network_Wait_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_network_wait_time = mn_chk;

	:: GetPrivateProfileString("BasicData", "Network_Retry_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 50)
	{
		st_basic.mn_network_retry_cnt = 3;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_retry_cnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Network_Retry_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_network_retry_cnt = mn_chk;

	:: GetPrivateProfileString("BasicData", "Ararm_Delay_Time", "-1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1000)
	{
		st_basic.mn_alarm_delay_time = 10;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_alarm_delay_time, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Ararm_Delay_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_alarm_delay_time = mn_chk;

	:: GetPrivateProfileString("BasicData", "LabelVisionPaper", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 20)
	{
		st_basic.mn_Label_vision_paper = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_Label_vision_paper, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "LabelVisionPaper", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_Label_vision_paper = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "LabelPrintErrorCnt", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 10)
	{
		st_basic.mn_Label_Print_Error_Cnt = 5;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_Label_Print_Error_Cnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "LabelPrintErrorCnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_Label_Print_Error_Cnt = mn_chk;

	//2015.0211
	:: GetPrivateProfileString("BasicData", "LabelPrintTotalErrorCnt", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.mn_Label_Print_Total_Error_Cnt = 90;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_Label_Print_Total_Error_Cnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "LabelPrintTotalErrorCnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_Label_Print_Total_Error_Cnt = mn_chk;

	:: GetPrivateProfileString("BasicData", "DeviceType", "-1", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 2)
	{
		st_basic.mn_device_type = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_device_type, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "DeviceType", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.mn_device_type = mn_chk;

	:: GetPrivateProfileString("BasicData", "TIME_OUT", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 9999)
	{
		st_basic.mn_time_out = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_time_out, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "TIME_OUT", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.mn_time_out = mn_chk;

	

	:: GetPrivateProfileString("BasicData", "ROBOT_INIT_TIME", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 9999)
	{
		st_basic.mn_robot_init_time = 10;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_robot_init_time, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "ROBOT_INIT_TIME", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.mn_robot_init_time = mn_chk;

	:: GetPrivateProfileString("BasicData", "PICK_TYPE", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk >= 4)// 2013,1023 if (mn_chk < 0 || mn_chk >= 3)
	{
		st_basic.n_pick_type = 0;
		mstr_temp.Format("%d", st_basic.n_pick_type);
		:: WritePrivateProfileString("BasicData", "PICK_TYPE", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.n_pick_type = mn_chk;


	//2013,0215
	//////////////////////////////////////////////////////////////////////////
	:: GetPrivateProfileString("BasicData", "Mirror_Type", "-1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_mirror_type = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_mirror_type, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Mirror_Type", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_mirror_type = mn_chk;

	st_handler.mn_mirror_type = 0;


	:: GetPrivateProfileString("BasicData", "XGEM_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_mode_xgem = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_xgem, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "XGEM_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_mode_xgem = mn_chk;

// 	st_basic.mn_mode_xgem = CTL_NO;//2015.0413

	:: GetPrivateProfileString("BasicData", "RCMD_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_mode_rcmd = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_rcmd, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "RCMD_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_mode_rcmd = mn_chk;


	:: GetPrivateProfileString("BasicData", "RECIPE_NAME", "", chr_data, 20, st_path.mstr_basic);
	CString str_recipe;
	str_recipe.Format("%s", chr_data);//2012,1216
	str_recipe.TrimLeft(' ');
	str_recipe.TrimRight(' ');
	if (str_recipe == "")
	{
		st_basic.mstr_revipe_name = str_recipe = "AMT8590";
		:: WritePrivateProfileString("BasicData", "RECIPE_NAME", LPCTSTR(str_recipe), st_path.mstr_basic);
	}
	else  st_basic.mstr_revipe_name = LPCTSTR(str_recipe);
	

	//////20131115
	:: GetPrivateProfileString("BasicData", "Distance", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	st_basic.n_distance= mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Shutter_Time", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	st_basic.n_shutter_time= mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Brightness", "0", chr_data, 10, str_load_file);
	md_chk = atoi(chr_data);
	st_basic.n_brightness= md_chk;
	
	:: GetPrivateProfileString("BasicData", "Contrast", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	st_basic.n_contrast= mn_chk;
	
	:: GetPrivateProfileString("BasicData", "1D_Width", "0", chr_data, 10, str_load_file);
	md_chk = atof(chr_data);
	st_basic.d_1d_width= md_chk;
	
	:: GetPrivateProfileString("BasicData", "Illumination", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 4)
	{
		st_basic.n_illumination = 0;
		mstr_temp.Format("%d", st_basic.n_illumination);
		:: WritePrivateProfileString("BasicData", "Illumination", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.n_illumination = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Image_Rotate", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 2)
	{
		st_basic.n_image_rotate = 0;
		mstr_temp.Format("%d", st_basic.n_image_rotate);
		:: WritePrivateProfileString("BasicData", "Image_Rotate", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.n_image_rotate = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "1d2d_Select", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 3)
	{
		st_basic.n_1d2d_select = 0;
		mstr_temp.Format("%d", st_basic.n_1d2d_select);
		:: WritePrivateProfileString("BasicData", "1d2d_Select", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.n_1d2d_select = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "1d2d_background", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 6)
	{
		st_basic.n_1d2d_background = 0;
		mstr_temp.Format("%d", st_basic.n_1d2d_background);
		:: WritePrivateProfileString("BasicData", "1d2d_background", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.n_1d2d_background = mn_chk;

	//////


	st_handler.mn_virtual_mode = 0;//2013,0103
	st_handler.mn_virtual_supply = 0;

	////2014.0823
	:: GetPrivateProfileString("BasicData", "Bcr_Mode", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_bcr_Mode = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_bcr_Mode, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Bcr_Mode", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.n_bcr_Mode = mn_chk;


	:: GetPrivateProfileString("TowerLampData", "str_equip_no", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_no = chr_data;
	(st_lamp.mstr_equip_no).TrimLeft(' ');               
	(st_lamp.mstr_equip_no).TrimRight(' ');
	
	:: GetPrivateProfileString("TowerLampData", "str_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_id = chr_data;
	(st_lamp.mstr_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_equip_id).TrimRight(' ');


	:: GetPrivateProfileString("BasicData", "BcrFeederUse_1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nBcrFeederUse[0] = 1;
		mstr_temp.Format("%d", st_basic.nBcrFeederUse[0]);
		:: WritePrivateProfileString("BasicData", "BcrFeederUse_1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nBcrFeederUse[0] = mn_chk;

	:: GetPrivateProfileString("BasicData", "BcrFeederUse_2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nBcrFeederUse[1] = 1;
		mstr_temp.Format("%d", st_basic.nBcrFeederUse[1]);
		:: WritePrivateProfileString("BasicData", "BcrFeederUse_2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nBcrFeederUse[1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "BcrFeederUse_3", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nBcrFeederUse[2] = 1;
		mstr_temp.Format("%d", st_basic.nBcrFeederUse[2]);
		:: WritePrivateProfileString("BasicData", "BcrFeederUse_3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nBcrFeederUse[2] = mn_chk;

	:: GetPrivateProfileString("BasicData", "BcrFeederUse_4", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nBcrFeederUse[3] = 1;
		mstr_temp.Format("%d", st_basic.nBcrFeederUse[3]);
		:: WritePrivateProfileString("BasicData", "BcrFeederUse_4", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nBcrFeederUse[3] = mn_chk;




	:: GetPrivateProfileString("BasicData", "DensityFeederUse_1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nDensityFeederUse[0] = 1;
		mstr_temp.Format("%d", st_basic.nDensityFeederUse[0]);
		:: WritePrivateProfileString("BasicData", "DensityFeederUse_1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nDensityFeederUse[0] = mn_chk;

	:: GetPrivateProfileString("BasicData", "DensityFeederUse_2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nDensityFeederUse[1] = 1;
		mstr_temp.Format("%d", st_basic.nDensityFeederUse[1]);
		:: WritePrivateProfileString("BasicData", "DensityFeederUse_2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nDensityFeederUse[1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "DensityFeederUse_3", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nDensityFeederUse[2] = 1;
		mstr_temp.Format("%d", st_basic.nDensityFeederUse[2]);
		:: WritePrivateProfileString("BasicData", "DensityFeederUse_3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nDensityFeederUse[2] = mn_chk;

	:: GetPrivateProfileString("BasicData", "DensityFeederUse_4", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nDensityFeederUse[3] = 1;
		mstr_temp.Format("%d", st_basic.nDensityFeederUse[3]);
		:: WritePrivateProfileString("BasicData", "DensityFeederUse_4", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nDensityFeederUse[3] = mn_chk;

	:: GetPrivateProfileString("BasicData", "DensityFeederUse_5", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nDensityFeederUse[4] = 1;
		mstr_temp.Format("%d", st_basic.nDensityFeederUse[4]);
		:: WritePrivateProfileString("BasicData", "DensityFeederUse_5", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nDensityFeederUse[4] = mn_chk;

	:: GetPrivateProfileString("BasicData", "DensityFeederUse_6", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nDensityFeederUse[5] = 1;
		mstr_temp.Format("%d", st_basic.nDensityFeederUse[5]);
		:: WritePrivateProfileString("BasicData", "DensityFeederUse_6", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nDensityFeederUse[5] = mn_chk;


	:: GetPrivateProfileString("BasicData", "VisionUse_1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nVisionUse[0] = 1;
		mstr_temp.Format("%d", st_basic.nVisionUse[0]);
		:: WritePrivateProfileString("BasicData", "VisionUse_1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nVisionUse[0] = mn_chk;

	
	:: GetPrivateProfileString("BasicData", "VisionUse_2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nVisionUse[1] = 1;
		mstr_temp.Format("%d", st_basic.nVisionUse[1]);
		:: WritePrivateProfileString("BasicData", "VisionUse_2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nVisionUse[1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "VisionUse_3", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nVisionUse[2] = 1;
		mstr_temp.Format("%d", st_basic.nVisionUse[2]);
		:: WritePrivateProfileString("BasicData", "VisionUse_3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nVisionUse[2] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "VisionUse_4", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nVisionUse[3] = 1;
		mstr_temp.Format("%d", st_basic.nVisionUse[3]);
		:: WritePrivateProfileString("BasicData", "VisionUse_4", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nVisionUse[3] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Left_bcr", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_leftbcr = 1;
		mstr_temp.Format("%d", st_basic.mn_leftbcr);
		:: WritePrivateProfileString("BasicData", "Left_bcr", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_leftbcr = mn_chk;
	
	
	:: GetPrivateProfileString("BasicData", "Right_bcr", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_rightbcr = 1;
		mstr_temp.Format("%d", st_basic.mn_rightbcr);
		:: WritePrivateProfileString("BasicData", "Right_bcr", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_rightbcr = mn_chk;

	////
	//2014,1125 ysj
	:: GetPrivateProfileString("BasicData", "LabelSelect", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nLabelSelect = 1;
		mstr_temp.Format("%d", st_basic.nLabelSelect);
		:: WritePrivateProfileString("BasicData", "LabelSelect", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nLabelSelect = mn_chk;
	////

	:: GetPrivateProfileString("BasicData", "Out_Conv", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_outconv = 1;
		mstr_temp.Format("%d", st_basic.mn_outconv);
		:: WritePrivateProfileString("BasicData", "Out_Conv", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_outconv = mn_chk;

	:: GetPrivateProfileString("BasicData", "ECServerUse", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_ECServerUse = 1;
		mstr_temp.Format("%d", st_basic.mn_ECServerUse);
		:: WritePrivateProfileString("BasicData", "ECServerUse", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_ECServerUse = mn_chk;

	:: GetPrivateProfileString("BasicData", "Out_TBox", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_outtbox = 1;
		mstr_temp.Format("%d", st_basic.mn_outtbox);
		:: WritePrivateProfileString("BasicData", "Out_TBox", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_outtbox = mn_chk;

	:: GetPrivateProfileString("BasicData", "Over_Time_Density", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_overtime_density = 3;
		mstr_temp.Format("%d", st_basic.mn_overtime_density);
		:: WritePrivateProfileString("BasicData", "Over_Time_Density", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_overtime_density = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Capa1_Label", "0", chr_data, 20, st_path.mstr_basic);
	mstr_temp.Format("%s", chr_data);//2012,1216
	if (mstr_temp == "" )
	{
		st_basic.n_str_capa1label = "";
		:: WritePrivateProfileString("BasicData", "Capa1_Label", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_str_capa1label = LPCTSTR(mstr_temp);
	
	:: GetPrivateProfileString("BasicData", "Capa2_Label", "0", chr_data, 20, st_path.mstr_basic);
	mstr_temp.Format("%s", chr_data);//2012,1216
	if (mstr_temp == "" )
	{
		st_basic.n_str_capa2label = "";
		:: WritePrivateProfileString("BasicData", "Capa2_Label", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_str_capa2label = LPCTSTR(mstr_temp);
	
	:: GetPrivateProfileString("BasicData", "Capa3_Label", "0", chr_data, 20, st_path.mstr_basic);
	mstr_temp.Format("%s", chr_data);//2012,1216
	if (mstr_temp == "" )
	{
		st_basic.n_str_capa3label = "";
		:: WritePrivateProfileString("BasicData", "Capa3_Label", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_str_capa3label = LPCTSTR(mstr_temp);

	:: GetPrivateProfileString("BasicData", "Limit_BCR_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 10)
	{
		st_basic.mn_limit_bcr_time = 5;
		mstr_temp.Format("%d", st_basic.mn_limit_bcr_time);
		:: WritePrivateProfileString("BasicData", "Limit_BCR_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_limit_bcr_time = mn_chk;

// 	:: GetPrivateProfileString("BasicData", "Auto_Vision_Model", "0", chr_data, 10, st_path.mstr_basic);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 1)
// 	{
// 		st_basic.mn_auto_vision_mdl = 1;
// 		mstr_temp.Format("%d", st_basic.mn_auto_vision_mdl);
// 		:: WritePrivateProfileString("BasicData", "Auto_Vision_Model", LPCTSTR(mstr_temp), st_path.mstr_basic);
// 	}
// 	else  st_basic.mn_auto_vision_mdl = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Top_Vision_Skip", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nTopVisionSkip = 1;
		mstr_temp.Format("%d", st_basic.nTopVisionSkip);
		:: WritePrivateProfileString("BasicData", "Top_Vision_Skip", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nTopVisionSkip = mn_chk;

	//2015.0614
	st_basic.nTopVisionSkip = CTL_NO;

	:: GetPrivateProfileString("BasicData", "Top_Vision_SkipCnt", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 99)
	{
		st_basic.nTopVisionSkipCnt = 90;
		mstr_temp = LPCTSTR(_itoa(st_basic.nTopVisionSkipCnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Top_Vision_SkipCnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nTopVisionSkipCnt = mn_chk;

	:: GetPrivateProfileString("BasicData", "Reject_Offset", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 999)
	{
		st_basic.nRejectOffset = 114;
		mstr_temp = LPCTSTR(_itoa(st_basic.nRejectOffset, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Reject_Offset", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nRejectOffset = mn_chk;
	
	
	:: GetPrivateProfileString("BasicData", "BCR_Mdl_Name", "", chr_data, 20, st_path.mstr_basic);
	CString str_bcrmdl="";
	str_bcrmdl.Format("%s", chr_data);//2012,1216
	str_bcrmdl.TrimLeft(' ');
	str_bcrmdl.TrimRight(' ');
	if (str_bcrmdl == "")
	{
		st_basic.mstr_bcr_model = str_bcrmdl = "25EVO";
		:: WritePrivateProfileString("BasicData", "BCR_Mdl_Name", LPCTSTR(str_bcrmdl), st_path.mstr_basic);
	}
	else  st_basic.mstr_bcr_model = LPCTSTR(str_bcrmdl);
	
	:: GetPrivateProfileString("BasicData", "BTM_Mdl_Name", "", chr_data, 20, st_path.mstr_basic);
	CString str_btmdl="";
	str_btmdl.Format("%s", chr_data);//2012,1216
	str_btmdl.TrimLeft(' ');
	str_btmdl.TrimRight(' ');
	if (str_btmdl == "")
	{
		st_basic.mstr_btm_model = str_btmdl = "25EVO";
		:: WritePrivateProfileString("BasicData", "BTM_Mdl_Name", LPCTSTR(str_btmdl), st_path.mstr_basic);
	}
	else  st_basic.mstr_btm_model = LPCTSTR(str_btmdl);
	
	//2016.0916
	:: GetPrivateProfileString("BasicData", "Rework_CCS", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_rework_ccs_mode = 1;
		mstr_temp.Format("%d", st_basic.mn_rework_ccs_mode);
		:: WritePrivateProfileString("BasicData", "Rework_CCS", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_rework_ccs_mode = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Vision_Density_Skip", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_vision_density_skip = 1;
		mstr_temp.Format("%d", st_basic.mn_vision_density_skip);
		:: WritePrivateProfileString("BasicData", "Vision_Density_Skip", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_vision_density_skip = mn_chk;

	CString strLabelBcr="";
	for ( j = 0; j < 4; j++)
	{
		strLabelBcr.Format("BCR_SD_%02d", j);
		:: GetPrivateProfileString("BasicData", strLabelBcr, "", chr_data, 20, str_load_file);
		mn_chk = atoi(chr_data);
		if (mn_chk < 0 || mn_chk > 50)
		{
			st_barcode.mn_labelset_sd[j] = 20;
			mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_sd[j], chr_buf, 10));
			:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_load_file);
		}
		else  st_barcode.mn_labelset_sd[j] = mn_chk;

		strLabelBcr.Format("BCR_LT_%02d", j);
		:: GetPrivateProfileString("BasicData", strLabelBcr, "", chr_data, 20, str_load_file);
		mn_chk = atoi(chr_data);
		if (mn_chk < 1 || mn_chk > 100)
		{
			st_barcode.mn_labelset_lt[j] = 50;
			mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lt[j], chr_buf, 10));
			:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_load_file);
		}
		else  st_barcode.mn_labelset_lt[j] = mn_chk;
		
		strLabelBcr.Format("BCR_LHX_%02d", j);
			:: GetPrivateProfileString("BasicData", strLabelBcr, "", chr_data, 20, str_load_file);
		mn_chk = atoi(chr_data);
		if (mn_chk < 0 || mn_chk > 100)
		{
			st_barcode.mn_labelset_lhx[j] = 50;
			mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhx[j], chr_buf, 10));
			:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_load_file);
		}
		else  st_barcode.mn_labelset_lhx[j] = mn_chk;
		
		strLabelBcr.Format("BCR_LHY_%02d", j);
			:: GetPrivateProfileString("BasicData", strLabelBcr, "", chr_data, 20, str_load_file);
		mn_chk = atoi(chr_data);
		if (mn_chk < 0 || mn_chk > 100)
		{
			st_barcode.mn_labelset_lhy[j] = 50;
			mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhy[j], chr_buf, 10));
			:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_load_file);
		}
		else  st_barcode.mn_labelset_lhy[j] = mn_chk;
		
		strLabelBcr.Format("BCR_MODE_%02d", j);
			:: GetPrivateProfileString("BasicData", strLabelBcr, "", chr_data, 20, str_load_file);
		mn_chk = atoi(chr_data);
		if (mn_chk < 0 || mn_chk > 1)
		{
			st_barcode.mn_Prt_Mode[j] = 1;
			mstr_temp = LPCTSTR(_itoa(st_barcode.mn_Prt_Mode[j], chr_buf, 10));
			:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_load_file);
		}
		else  st_barcode.mn_Prt_Mode[j] = mn_chk;
		
		strLabelBcr.Format("BCR_COMPLETE_%02d", j);
			:: GetPrivateProfileString("BasicData", strLabelBcr, "", chr_data, 20, str_load_file);
		mn_chk = atoi(chr_data);
		if (mn_chk < 01 || mn_chk > 1)
		{
			st_barcode.mn_Prt_complete_use[j] = 1;
			mstr_temp = LPCTSTR(_itoa(st_barcode.mn_Prt_complete_use[j], chr_buf, 10));
			:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_load_file);
		}
		else  st_barcode.mn_Prt_complete_use[j] = mn_chk;
	}



}

void CMyBasicData::OnBasic_Data_Save()
{
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	CString str_save_file="";
	CString str_part="", str_chk_ext="";
	char chr_buf[20];
	COleDateTime time_cur;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour, n_pos; // 현재 시간정보 저장 변수 

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
//st_basic.mstr_device_name = "LPTOP850PRO.TXT";
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_run, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Run_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
	:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_device);
	:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%0.2f", st_basic.md_electrostatic_value);
	:: WritePrivateProfileString("BasicData", "ELECT_VALUE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nRunSpeed);
	:: WritePrivateProfileString("BasicData", "Run_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nManualSpeed);
	:: WritePrivateProfileString("BasicData", "Manual_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	:: WritePrivateProfileString("BasicData", "Device_Type", LPCTSTR(st_basic.mstr_device_name), st_path.mstr_basic);
// 	:: WritePrivateProfileString("BasicData", "DeviceDataType", LPCTSTR(st_basic.sDvcName), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
	:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDI);
	:: WritePrivateProfileString("BasicData", "Daily_Idle_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDC);
	:: WritePrivateProfileString("BasicData", "Daily_JobChange_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	time_cur = COleDateTime::GetCurrentTime();

	mn_cur_year = time_cur.GetYear();  
	mn_cur_month = time_cur.GetMonth();  
    mn_cur_day = time_cur.GetDay();  
	mn_cur_hour = time_cur.GetHour();
	/* ************************************************************************** */

	mstr_temp.Format("%04d%02d%02d%02d", mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour);
	:: WritePrivateProfileString("BasicData", "Data_Save_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_bypass_mode, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Bypass_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_time_out, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "TIME_OUT", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_robot_init_time, chr_buf, 10));//2012,1225
	:: WritePrivateProfileString("BasicData", "ROBOT_INIT_TIME", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Network_Wait_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_retry_cnt, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Network_Retry_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_Label_vision_paper);
	:: WritePrivateProfileString("BasicData", "LabelVisionPaper", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.mn_Label_Print_Error_Cnt);
	:: WritePrivateProfileString("BasicData", "LabelPrintErrorCnt", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_Label_Print_Total_Error_Cnt);
	:: WritePrivateProfileString("BasicData", "LabelPrintTotalErrorCnt", LPCTSTR(mstr_temp), st_path.mstr_basic);

	//st_basic.mn_device_type
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_device_type, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "DeviceType", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_alarm_delay_time, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Ararm_Delay_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_pick_type);
	:: WritePrivateProfileString("BasicData", "PICK_TYPE", LPCTSTR(mstr_temp), str_save_file);

	//////////////////////////////////////////////////////////////////////////
	mstr_temp = LPCTSTR(_itoa(st_handler.mn_mirror_type, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Mirror_Type", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_xgem, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "XGEM_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_rcmd, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "RCMD_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	
	mstr_temp = LPCTSTR(st_basic.mstr_revipe_name);
	:: WritePrivateProfileString("BasicData", "RECIPE_NAME", LPCTSTR(mstr_temp), st_path.mstr_basic);

	///////20131115
	mstr_temp.Format("%d", st_basic.n_distance);											
	:: WritePrivateProfileString("BasicData", "Distance", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_shutter_time);													
	:: WritePrivateProfileString("BasicData", "Shutter_Time", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_brightness);															
	:: WritePrivateProfileString("BasicData", "Brightness", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_contrast);															
	:: WritePrivateProfileString("BasicData", "Contrast", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.1f", st_basic.d_1d_width);															
	:: WritePrivateProfileString("BasicData", "1D_Width", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_illumination);
	:: WritePrivateProfileString("BasicData", "Illumination", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_image_rotate);
	:: WritePrivateProfileString("BasicData", "Image_Rotate", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_1d2d_select);
	:: WritePrivateProfileString("BasicData", "1d2d_Select", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_basic.n_1d2d_background);
	:: WritePrivateProfileString("BasicData", "1d2d_background", LPCTSTR(mstr_temp), str_save_file);
	
	////////

	////2014.0823
	mstr_temp.Format("%d", st_basic.n_bcr_Mode);											
	:: WritePrivateProfileString("BasicData", "Bcr_Mode", LPCTSTR(mstr_temp), str_save_file);

	:: WritePrivateProfileString("TowerLampData", "str_equip_no", LPCTSTR(st_lamp.mstr_equip_no), str_save_file);
	:: WritePrivateProfileString("TowerLampData", "str_equip_code", LPCTSTR(st_lamp.mstr_equip_id), str_save_file);
	////
	mstr_temp.Format("%d", st_basic.nBcrFeederUse[0]);
	:: WritePrivateProfileString("BasicData", "BcrFeederUse_1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nBcrFeederUse[1]);
	:: WritePrivateProfileString("BasicData", "BcrFeederUse_2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nBcrFeederUse[2]);
	:: WritePrivateProfileString("BasicData", "BcrFeederUse_3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nBcrFeederUse[3]);
	:: WritePrivateProfileString("BasicData", "BcrFeederUse_4", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nDensityFeederUse[0]);
	:: WritePrivateProfileString("BasicData", "DensityFeederUse_1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nDensityFeederUse[1]);
	:: WritePrivateProfileString("BasicData", "DensityFeederUse_2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nDensityFeederUse[2]);
	:: WritePrivateProfileString("BasicData", "DensityFeederUse_3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nDensityFeederUse[3]);
	:: WritePrivateProfileString("BasicData", "DensityFeederUse_4", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nDensityFeederUse[4]);
	:: WritePrivateProfileString("BasicData", "DensityFeederUse_5", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nDensityFeederUse[5]);
	:: WritePrivateProfileString("BasicData", "DensityFeederUse_6", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.nVisionUse[0]);
	:: WritePrivateProfileString("BasicData", "VisionUse_1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nVisionUse[1]);
	:: WritePrivateProfileString("BasicData", "VisionUse_2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nVisionUse[2]);
	:: WritePrivateProfileString("BasicData", "VisionUse_3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.nVisionUse[3]);
	:: WritePrivateProfileString("BasicData", "VisionUse_4", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	
	mstr_temp.Format("%d", st_basic.mn_leftbcr);
	:: WritePrivateProfileString("BasicData", "Left_bcr", LPCTSTR(mstr_temp), st_path.mstr_basic);
	mstr_temp.Format("%d", st_basic.mn_rightbcr);
	:: WritePrivateProfileString("BasicData", "Right_bcr", LPCTSTR(mstr_temp), st_path.mstr_basic);

	////2014,1125 ysj
	mstr_temp.Format("%d", st_basic.nLabelSelect);
	:: WritePrivateProfileString("BasicData", "LabelSelect", LPCTSTR(mstr_temp), st_path.mstr_basic);
	////

	mstr_temp.Format("%d", st_basic.mn_outconv);
	:: WritePrivateProfileString("BasicData", "Out_Conv", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_ECServerUse);
	:: WritePrivateProfileString("BasicData", "ECServerUse", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_outtbox);
	:: WritePrivateProfileString("BasicData", "Out_TBox", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_overtime_density);
	:: WritePrivateProfileString("BasicData", "Over_Time_Density", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = (LPCTSTR)st_basic.n_str_capa1label;
	:: WritePrivateProfileString("BasicData", "Capa1_Label", LPCTSTR(mstr_temp), st_path.mstr_basic);		

	mstr_temp = (LPCTSTR)st_basic.n_str_capa2label;
	:: WritePrivateProfileString("BasicData", "Capa2_Label", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp = (LPCTSTR)st_basic.n_str_capa3label;
	:: WritePrivateProfileString("BasicData", "Capa3_Label", LPCTSTR(mstr_temp), st_path.mstr_basic);		

	mstr_temp.Format("%d", st_basic.mn_limit_bcr_time);
	:: WritePrivateProfileString("BasicData", "Limit_BCR_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
// 	mstr_temp.Format("%d", st_basic.mn_auto_vision_mdl);
// 	:: WritePrivateProfileString("BasicData", "Auto_Vision_Model", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.nTopVisionSkip);
	:: WritePrivateProfileString("BasicData", "Top_Vision_Skip", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nTopVisionSkipCnt);
	:: WritePrivateProfileString("BasicData", "Top_Vision_SkipCnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.nRejectOffset);
	:: WritePrivateProfileString("BasicData", "Reject_Offset", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%s", st_basic.mstr_bcr_model);
	:: WritePrivateProfileString("BasicData", "BCR_Mdl_Name", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%s", st_basic.mstr_btm_model);
	:: WritePrivateProfileString("BasicData", "BTM_Mdl_Name", LPCTSTR(mstr_temp), st_path.mstr_basic);

	//2016.0916
	mstr_temp.Format("%d", st_basic.mn_rework_ccs_mode);
	:: WritePrivateProfileString("BasicData", "Rework_CCS", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.mn_vision_density_skip);
	:: WritePrivateProfileString("BasicData", "Vision_Density_Skip", LPCTSTR(mstr_temp), st_path.mstr_basic);
	

	CString strLabelBcr="";
	for ( int j = 0; j < 4; j++)
	{
		strLabelBcr.Format("BCR_SD_%02d", j);
		mstr_temp.Format("%d", st_barcode.mn_labelset_sd[j]);
		:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_save_file);
		
		strLabelBcr.Format("BCR_LT_%02d", j);
		mstr_temp.Format("%d", st_barcode.mn_labelset_lt[j]);
		:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_save_file);
		
		strLabelBcr.Format("BCR_LHX_%02d", j);
		mstr_temp.Format("%d", st_barcode.mn_labelset_lhx[j]);
		:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_save_file);
		
		strLabelBcr.Format("BCR_LHY_%02d", j);
		mstr_temp.Format("%d", st_barcode.mn_labelset_lhy[j]);
		:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_save_file);
		
		strLabelBcr.Format("BCR_MODE_%02d", j);
		mstr_temp.Format("%d", st_barcode.mn_Prt_Mode[j]);
		:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_save_file);
		
		strLabelBcr.Format("BCR_COMPLETE_%02d", j);
		mstr_temp.Format("%d", st_barcode.mn_Prt_complete_use[j]);
		:: WritePrivateProfileString("BasicData", strLabelBcr, LPCTSTR(mstr_temp), str_save_file);
	}

}

//2013,0215
void CMyBasicData::OnPogoAlignType_Save(CString strFile)
{
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	
	//////////////////////////////////////////////////////////////////////////
	mstr_temp.Format("%d", st_handler.mn_temp_pogo_type);
	:: WritePrivateProfileString("BasicData", "Pogo_Type", LPCTSTR(mstr_temp), strFile);
	
	mstr_temp.Format("%d", st_handler.mn_temp_alignBuffer_type);
	:: WritePrivateProfileString("BasicData", "AlignBuffer_Type", LPCTSTR(mstr_temp), strFile);
	//////////////////////////////////////////////////////////////////////////
}

void CMyBasicData::OnDaily_Count_Save()
{
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	char chr_buf[20];

	mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
	:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDI);
	:: WritePrivateProfileString("BasicData", "Daily_Idle_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDC);
	:: WritePrivateProfileString("BasicData", "Daily_JobChange_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

CString CMyBasicData::OnGet_File_Name()
{
	CString str_temp="";       // 임시 저장 변수
	CString str_save_file="";  // 저장 파일 임시 설정 변수
	CString str_chk_ext="";    // 파일 확장자 저장 변수
	CString str_part="", str_part2="";
	CString str_new_save_file="";
	int n_pos;
	
	// **************************************************************************
    // Socket Contact Count 데이터 저장할 파일 설정한다 [파일 확장자 검사]        
    // **************************************************************************
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	// **************************************************************************

	return str_save_file;  // 파일명 리턴 
}

CString CMyBasicData::OnGet_Teach_File_Name(CString strName)
{
	CString str_temp="";       // 임시 저장 변수
	CString str_save_file="";  // 저장 파일 임시 설정 변수
	CString str_chk_ext="";    // 파일 확장자 저장 변수
	CString str_part="", str_part2="";
	CString str_new_save_file="";
	CString str_device_name="";
	int n_pos;
	
	n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		st_basic.mstr_device_name += _T(".DAT");  // 확장자 추가
		n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	}
	
	str_device_name = st_basic.mstr_device_name.Mid(0, n_pos);
	
	
	if (strName.Find("Back") != -1)
	{
		str_save_file = st_path.mstr_path_back + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
	else
	{
		str_save_file = st_path.mstr_path_dvc + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
	
	if (strName != "")
	{
		str_save_file += "_";
		str_save_file += strName;
	}

	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		str_save_file += _T(".DAT");  // 확장자 추가
	}
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".DAT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.DAT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청
			}
		}
	}
	// **************************************************************************
	
	return str_save_file;  // 파일명 리턴 
}

int CMyBasicData::On_Teach_Data_Load()
{
	CString str_temp="", str_msg="", str_LoadFile="";       // 임시 저장 변수
	int i;
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = OnGet_Teach_File_Name("Teach");  // 데이터 저장 파일명 로딩 함수 
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
//				st_msg.mstr_abnormal_msg.Format("[%s] file open error.", str_LoadFile);
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return -1;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] file is not exist.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return -1;
	}
	
	for (i = 0; i < MOTOR_COUNT; i++)
	{
		fread(&st_motor[i].d_pos, sizeof(st_motor[i].d_pos), 1, fp);
	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Read Error.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			clearerr(fp);
		}
	}

	fclose(fp) ;

	return RET_GOOD;
}

void CMyBasicData::On_Teach_Data_Save()
{
	CString str_temp="", str_SaveFile="";			// 임시 저장 변수
	int i;
	
	str_SaveFile = OnGet_Teach_File_Name("Teach");		// 데이터 저장 파일명 로딩 함수 
	
	FILE  *fp;
	int   err = 0;
	
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	for (i = 0; i < MOTOR_COUNT; i++)
	{
		fwrite(&st_motor[i].d_pos, sizeof(st_motor[i].d_pos), 1, fp);
	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);

//	str_SaveFile = OnGet_Teach_File_Name("Teach_Back");		// 데이터 저장 파일명 로딩 함수 
//		
//	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
//	{
//		if (st_handler.cwnd_list != NULL)
//		{
//			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//		}
//		return;
//	}
//	
//	for (i = 0; i < MOTOR_COUNT; i++)
//	{
//		fwrite(&st_motor[i].d_pos, sizeof(st_motor[i].d_pos), 1, fp);
//	}
//	
//	if (ferror(fp)) 
//	{
//		if (st_handler.cwnd_list != NULL)
//		{
//			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//		}
//		clearerr(fp);
//	}
//	
//	fclose(fp);
}

void CMyBasicData::OnAdmin_Data_Load()
{
	CString mstr_temp="";			// 저장할 정보 임시 저장 변수 
	char chr_data[50];
//	int mn_chk;

//	:: GetPrivateProfileString("ADMIN", "Print_Out_Mode", "0", chr_data, 10, st_path.mstr_basic);
//	mn_chk = atoi(chr_data);
//	if (mn_chk < 0 || mn_chk > 2)
//	{
//		st_basic.n_mode_barcode_print = 0;
//		mstr_temp.Format("%d", st_basic.n_mode_barcode_print);
//		:: WritePrivateProfileString("BasicData", "Run_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
//	}
//	else  st_basic.n_mode_barcode_print = mn_chk;
	:: GetPrivateProfileString("ADMIN", "CIP_EC", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_ECSERVER].str_ip = mstr_temp;

	:: GetPrivateProfileString("ADMIN", "CPORT_EC", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_ECSERVER].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_BOTTOM", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_BOTTOM].str_ip = mstr_temp;

	:: GetPrivateProfileString("ADMIN", "CPORT_BOTTOM", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_BOTTOM].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_8570", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_8570].str_ip = mstr_temp;

	:: GetPrivateProfileString("ADMIN", "CPORT_8570", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_8570].n_port = atoi(mstr_temp);

	
	:: GetPrivateProfileString("ADMIN", "CIP_NEXT", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_NEXT].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_NEXT", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_NEXT].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_FORMAT", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_FORMAT].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_FORMAT", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_FORMAT].n_port = atoi(mstr_temp);


	:: GetPrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_1", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_BCR_PRINTER1].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_1", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_BCR_PRINTER1].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_2", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_BCR_PRINTER2].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_2", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_BCR_PRINTER2].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_3", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_BCR_PRINTER3].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_3", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_BCR_PRINTER3].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_4", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_BCR_PRINTER4].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_4", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_BCR_PRINTER4].n_port = atoi(mstr_temp);


	:: GetPrivateProfileString("ADMIN", "CIP_VISION1", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_VIS_DENSITY].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_VISION1", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_VIS_DENSITY].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_VISION2", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_VIS_BARCODE].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_VISION2", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_VIS_BARCODE].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_VISION3", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_VIS_TOP].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_VISION3", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_VIS_TOP].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "CIP_VISION4", "", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_VIS_BTM].str_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "CPORT_VISION4", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_VIS_BTM].n_port = atoi(mstr_temp);

	
	:: GetPrivateProfileString("ADMIN", "SPORT_FRONT", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_FRONT].n_port = atoi(mstr_temp);
	
	////2014,1130
	:: GetPrivateProfileString("ADMIN", "SPORT_VISION1", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_VISION1].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "SPORT_VISION2", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_VISION2].n_port = atoi(mstr_temp);

	:: GetPrivateProfileString("ADMIN", "SPORT_VISION3", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_VISION3].n_port = atoi(mstr_temp);
	
	:: GetPrivateProfileString("ADMIN", "SPORT_VISION4", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_VISION4].n_port = atoi(mstr_temp);
	////

	:: GetPrivateProfileString("ADMIN", "SPORT_BCR1", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_PRINTER1].n_port = atoi(mstr_temp);
	
	:: GetPrivateProfileString("ADMIN", "SPORT_BCR2", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_PRINTER2].n_port = atoi(mstr_temp);
	
	:: GetPrivateProfileString("ADMIN", "SPORT_BCR3", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_PRINTER3].n_port = atoi(mstr_temp);
	
	:: GetPrivateProfileString("ADMIN", "SPORT_BCR4", "", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_server[SVR_PRINTER4].n_port = atoi(mstr_temp);


}

////////20131115
void CMyBasicData::OnInterface_Data_Load()
{
	int i;
	CString str_tmp="", str_name="";
	char ch_data[30];
	
	for(i=0; i<MAX_PORT; i++)
	{
		str_name.Format("PORT_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "1", ch_data, 20, st_path.str_interface_folder);
		rs_232.n_serial_port[i] = atoi(ch_data);
		
		str_name.Format("BAUDRATE_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "57600", ch_data, 20, st_path.str_interface_folder);
		rs_232.n_serial_baudrate[i] = atoi(ch_data);
		
		str_name.Format("DATA_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "8", ch_data, 20, st_path.str_interface_folder);
		rs_232.n_serial_data[i] = atoi(ch_data);
		
		str_name.Format("STOP_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "1", ch_data, 20, st_path.str_interface_folder);
		rs_232.n_serial_stop[i] = atoi(ch_data);
		
		str_name.Format("PARITY_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "0", ch_data, 20, st_path.str_interface_folder);
		rs_232.n_serial_parity[i] = atoi(ch_data);
	}
}

void CMyBasicData::OnInterface_Data_Save()
{
	int i;
	CString str_tmp="", str_name="";
	
	for(i=0; i<MAX_PORT; i++)
	{
		str_name.Format("PORT_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_port[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.str_interface_folder);
		
		str_name.Format("BAUDRATE_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_baudrate[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.str_interface_folder);
		
		str_name.Format("DATA_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_data[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.str_interface_folder);
		
		str_name.Format("STOP_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_stop[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.str_interface_folder);
		
		str_name.Format("PARITY_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_parity[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.str_interface_folder);
	}
}

////////
void CMyBasicData::OnBarcode_Label_Data_Load()
{
	CString str_load_device="";	// 로딩 디바이스명 저장 변수
	CString str_load_file="";
	CString str_chk_ext="";		// 파일 확장자 저장 변수
	CString mstr_temp="", str_part="", str_station="", str_board="";			// 저장할 정보 임시 저장 변수 
	//char chr_data[50], chr_buf[20];	//20120530
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */

/////////////////////////////////////////////////////////// 20120530
	CString str_label_load_file;
	
	str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	
	n_pos = str_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}



// 	:: GetPrivateProfileString("Barcode", "SD1", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_sd1 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_sd1, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "SD1", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_sd1 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "LT1", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_lt1 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lt1, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LT1", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lt1 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "LHX1", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhx1 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhx1, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHX1", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhx1 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "LHY1", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhy1 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhy1, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHY1", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhy1 = mn_chk;
// 
// 
// 
// 	:: GetPrivateProfileString("Barcode", "SD2", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_sd2 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_sd2, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "SD2", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_sd2 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LT2", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_lt2 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lt2, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LT2", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lt2 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LHX2", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhx2 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhx2, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHX2", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhx2 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LHY2", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhy2 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhy2, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHY2", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhy2 = mn_chk;
// 
// 
// 
// 	:: GetPrivateProfileString("Barcode", "SD3", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_sd3 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_sd3, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "SD3", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_sd3 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LT3", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_lt3 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lt3, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LT3", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lt3 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LHX3", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhx3 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhx3, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHX3", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhx3 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LHY3", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhy3 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhy3, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHY3", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhy3 = mn_chk;
// 
// 
// 	:: GetPrivateProfileString("Barcode", "SD4", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_sd4 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_sd4, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "SD4", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_sd4 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LT4", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < -120 || mn_chk > 120)
// 	{
// 		st_barcode.mn_labelset_lt4 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lt4, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LT4", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lt4 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LHX4", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhx4 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhx4, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHX4", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhx4 = mn_chk;
// 	
// 	:: GetPrivateProfileString("Barcode", "LHY4", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 40)
// 	{
// 		st_barcode.mn_labelset_lhy4 = 0;
// 		mstr_temp = LPCTSTR(_itoa(st_barcode.mn_labelset_lhy4, chr_buf, 10));
// 		:: WritePrivateProfileString("Barcode", "LHY4", LPCTSTR(mstr_temp), str_load_file);
// 	}
// 	else  st_barcode.mn_labelset_lhy4 = mn_chk;
// 
// 
//  	:: GetPrivateProfileString("Barcode", "PRINT_COMP_USE1", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_complete_use1 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_COMP_USE2", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_complete_use2 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_COMP_USE3", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_complete_use3 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_COMP_USE4", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_complete_use4 = mn_chk;
// 
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_MODE1", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_Mode1 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_MODE2", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_Mode2 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_MODE3", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_Mode3 = mn_chk;
// 
// 	:: GetPrivateProfileString("Barcode", "PRINT_MODE4", "0", chr_data, 10, str_load_file);
// 	mn_chk = atoi(chr_data);
// 	st_barcode.mn_Prt_Mode4 = mn_chk;


}

void CMyBasicData::OnBarcode_Label_Data_Save()
{
	CString mstr_temp="";  // 저장할 정보 임시 저장 변수 
	CString str_save_file="";
	CString str_part="", str_chk_ext="";
	//char chr_buf[20];
	COleDateTime time_cur;
	int n_pos; // 현재 시간정보 저장 변수 mn_cur_day, mn_cur_hour,mn_cur_month,  mn_cur_year, 

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */

//////////////////////////////////////////////// 20120530

	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	
	
// 	st_basic.mstr_device_name = "LAPTOP-850PRO.TXT";
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}

// 	mstr_temp.Format("%d",st_barcode.mn_labelset_sd1);
// 	:: WritePrivateProfileString("Barcode", "SD1", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_sd2);
// 	:: WritePrivateProfileString("Barcode", "SD2", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_sd3);
// 	:: WritePrivateProfileString("Barcode", "SD3", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_sd4);
// 	:: WritePrivateProfileString("Barcode", "SD4", LPCTSTR(mstr_temp), str_save_file);
// 
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lt1);
// 	:: WritePrivateProfileString("Barcode", "LT1", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lt2);
// 	:: WritePrivateProfileString("Barcode", "LT2", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lt3);
// 	:: WritePrivateProfileString("Barcode", "LT3", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lt4);
// 	:: WritePrivateProfileString("Barcode", "LT4", LPCTSTR(mstr_temp), str_save_file);
// 
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhx1);
// 	:: WritePrivateProfileString("Barcode", "LHX1", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhx2);
// 	:: WritePrivateProfileString("Barcode", "LHX2", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhx3);
// 	:: WritePrivateProfileString("Barcode", "LHX3", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhx4);
// 	:: WritePrivateProfileString("Barcode", "LHX4", LPCTSTR(mstr_temp), str_save_file);
// 
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhy1);
// 	:: WritePrivateProfileString("Barcode", "LHY1", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhy2);
// 	:: WritePrivateProfileString("Barcode", "LHY2", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhy3);
// 	:: WritePrivateProfileString("Barcode", "LHY3", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d",st_barcode.mn_labelset_lhy4);
// 	:: WritePrivateProfileString("Barcode", "LHY4", LPCTSTR(mstr_temp), str_save_file);
// 	
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_complete_use1);
// 	:: WritePrivateProfileString("Barcode", "PRINT_COMP_USE1", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_complete_use2);
// 	:: WritePrivateProfileString("Barcode", "PRINT_COMP_USE2", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_complete_use3);
// 	:: WritePrivateProfileString("Barcode", "PRINT_COMP_USE3", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_complete_use4);
// 	:: WritePrivateProfileString("Barcode", "PRINT_COMP_USE4", LPCTSTR(mstr_temp), str_save_file);
// 
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_Mode1);
// 	:: WritePrivateProfileString("Barcode", "PRINT_MODE1", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_Mode2);
// 	:: WritePrivateProfileString("Barcode", "PRINT_MODE2", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_Mode3);
// 	:: WritePrivateProfileString("Barcode", "PRINT_MODE3", LPCTSTR(mstr_temp), str_save_file);
// 	mstr_temp.Format("%d", st_barcode.mn_Prt_Mode4);
// 	:: WritePrivateProfileString("Barcode", "PRINT_MODE4", LPCTSTR(mstr_temp), str_save_file);


}