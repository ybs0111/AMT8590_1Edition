// MyBasicData.h: interface for the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
#define AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"  // 전역 변수 정의 클래스 추가 
#include "Public_Function.h"

enum ENUM_DEGREE
{
	EDEG_0,
	EDEG_90,
	EDEG_180,
	EDEG_270,
	MAX_EDEG,
};

/////////////20131115
enum ENUM_ILLUMINATION
{
	ILLUMIN_OFF,
	ILLUMIN_LEFT,
	ILLUMIN_RIGHT,
	ILLUMIN_BOTH,
	MAX_ILLUMINATION,
};

enum ENUM_IMAGE_ROTATE
{
	IMAGEROTATE_0,
	IMAGEROTATE_180,
	MAX_IMAGEROTATE,
};

enum ENUM_SELECT_1D2D
{
	SELECT_1D,
	SELECT_2D,
	SELECT_BOTH,
	MAX_1D2DSELECT,
};

enum ENUM_BACKGROUND_1D2D
{
	BACKGROUND_1DWHITE,
	BACKGROUND_1DBLACK,
	BACKGROUND_1DBOTH,
	BACKGROUND_2DWHITE,
	BACKGROUND_2DBLACK,
	BACKGROUND_2DBOTH,
	MAX_1D2DBACKGROUND,
};

////////////

enum ENUM_PICK_TYPE
{
	EPT_1V1M_4USE,
	EPT_1V1M_2USE,
	EPT_2V1M_2USE,
	EPT_1V1M_4USE_2,
	MAX_EPT,
};

class CMyBasicData  
{
public:
	CMyBasicData();
	virtual ~CMyBasicData();

// Attributes
public:

// Operations
public:
	void OnAdmin_Data_Load();
	void On_Teach_Data_Save();
	int On_Teach_Data_Load();
	CString OnGet_Teach_File_Name(CString strName);
	CString OnGet_File_Name();
	void OnPogoAlignType_Save(CString strFile);//2013,0215
	void OnDaily_Count_Save();
	void OnBasic_Data_Save();
	void OnBasic_Data_Load();
	void OnMaintenance_Data_Save();
	void OnMaintenance_Data_Load();
	void OnWaitTime_Data_Save();
	void OnWaitTime_Data_Load();
	void OnMotorSpeed_Set_Data_Save();
	void OnMotorSpeed_Set_Data_Load();
	void OnStep_File_Name_Save(CString str_file);
	CString OnStep_File_Name_Load();
	void OnStep_File_Index_Save(CString str_index);
	CString OnStep_File_Index_Load();
	void OnPassWord_Save(CString str_save_level, CString str_pass_data);
	void OnPassWord_Load(CString str_load_level);
	//////20131115
	void OnInterface_Data_Save();
	void OnInterface_Data_Load();
	//////
	void OnBarcode_Label_Data_Load();
	void OnBarcode_Label_Data_Save();
	
};

#endif // !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
