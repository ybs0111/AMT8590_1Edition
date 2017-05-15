// AVersion.cpp: implementation of the AVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AVersion.h"

#include "AMTRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <string>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AVersion g_ver;

AVersion::AVersion()
{
	m_strUpdate = "Update Desc...";

	m_strHandlerName = "";
	m_strVersion = "";
	m_strDate = "";
}

AVersion::~AVersion()
{

}

void AVersion::CheckVersion()
{
	std::string strVer_old = AMTRegistry::RegReadString( REG_KEY_VER, REG_VALUE_CREATE_VER, 0 );
	std::string strDate_old = AMTRegistry::RegReadString( REG_KEY_VER, REG_VALUE_CREATE_DATE, 0 );
	
 	if( m_strVersion != strVer_old.c_str() ||
 		m_strDate != strDate_old.c_str() )
 	{
		CString strMsg;
// 		strMsg.Format( "FUNCTION=VERSION_UPDATE EQP_ID=%s MODEL=\"SSD AUTO SORTER\" VERSION=\"%s_%s\" DESC=\"%s\"", st_lamp.mstr_equip_id, m_strVersion,
// 			m_strDate, m_strUpdate );
//2013,0715
		strMsg.Format( "FUNCTION=VERSION_UPDATE EQP_ID=%s MODEL=\"SSD INTERFACE\" VERSION=\"%s %s\" DESC=\"%s\"", st_lamp.mstr_equip_id, m_strVersion,
			m_strDate, m_strUpdate );
		
//		g_client_ec.PushSendMsg( (LPCSTR)strMsg );
 	}
}

void AVersion::UpdateVersion()
{
	AMTRegistry::RegWriteString( REG_KEY_VER, REG_VALUE_CREATE_VER, (LPCSTR)m_strVersion );
	AMTRegistry::RegWriteString( REG_KEY_VER, REG_VALUE_CREATE_DATE, (LPCSTR)m_strDate );
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, "%s %s",m_strVersion,m_strDate);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
}

void AVersion::SetVersion()
{
	CString str_temp;

	//2013,0715
	if(CHINA_VER == 1)
	{
		m_strHandlerName = "AMT8590(CHINA)";
		str_temp.Format("SESS - %04d%02d%02d", 2016, 9, 23);
	}
	else
	{
		m_strHandlerName = "AMT8590";
//		str_temp.Format("TP - %04d%02d%02d", 2015, 9, 15);
// 		str_temp.Format("TP - %04d%02d%02d", 2016, 12, 12);
		str_temp.Format("TP - %04d%02d%02d", 2017, 05, 15);
	}
//	m_strVersion = "Ver1.0.00";
	m_strVersion = "1.00";

	m_strDate = str_temp;

	m_strUpdate = "SSD Packing Handler";

	CheckVersion();
	UpdateVersion();
}

CString AVersion::GetStrVersion()
{
	CString strRet;
	if(st_handler.mn_mirror_type == CTL_YES)
	{
		strRet.Format("%s [SSD Packing Handler] : %s_%s(MIRROR)", m_strHandlerName, m_strVersion, m_strDate );
	}
	else
	{
		strRet.Format("%s [SSD Packing Handler] : %s_%s", m_strHandlerName, m_strVersion, m_strDate );
	}
	return strRet;
}