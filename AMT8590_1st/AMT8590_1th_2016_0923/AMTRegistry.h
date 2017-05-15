// AMTRegistry.h: interface for the AMTRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_)
#define AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#define REG_KEY_TIME				"Software\\AMT\\AMT8590\\TIME\\"
#define REG_ALARM_KEY_TIME			"Software\\AMT\\AMT8590\\ALARMTIME\\"
#define REG_VAL_TIME				"DAILY"

#define REG_KEY_COK					"Software\\AMT\\AMT8590\\COK\\"
#define REG_VAL_ROBOT_COK			"ROBOT_COK"

#define REG_VAL_ITEM_CNT			"ITEM_CNT"
#define REG_VAL_SITE_COK			"SITE"

#define REG_KEY_COUNT				"Software\\AMT\\AMT8590\\COUNT\\"
#define REG_VAL_COUNT_INPUT_PRIME	"INPUT_PRIME"
#define REG_VAL_COUNT_PASS_PRIME	"PASS_PRIME"
#define REG_VAL_COUNT_FAIL_PRIME	"FAIL_PRIME"
#define REG_VAL_COUNT_INPUT_CUM		"INPUT_CUM"
#define REG_VAL_COUNT_PASS_CUM		"PASS_CUM"
#define REG_VAL_COUNT_FAIL_CUM		"FAIL_CUM"
#define REG_VAL_COUNT_REWORK_PRIME	"REWORK_PRIME"
#define REG_VAL_COUNT_REWORK_CUM	"REWORK_CUM"
#define REG_VAL_COUNT_BIN			"BIN"
#define REG_VAL_COUNT_DENSITY_1		"DEN1_NUM"
#define REG_VAL_COUNT_DENSITY_2		"DEN2_NUM"
#define REG_VAL_COUNT_DENSITY_3		"DEN3_NUM"
#define REG_VAL_COUNT_DENSITY_4		"DEN4_NUM"
#define REG_VAL_COUNT_DENSITY_5		"DEN5_NUM"
#define REG_VAL_COUNT_DENSITY_6		"DEN6_NUM"
#define REG_VAL_COUNT_BCR_1			"BCR1_NUM"
#define REG_VAL_COUNT_BCR_2			"BCR2_NUM"
#define REG_VAL_COUNT_BCR_3			"BCR3_NUM"
#define REG_VAL_COUNT_BCR_4			"BCR4_NUM"
#define REG_VAL_COUNT_RIBBON_1		"RIBBON1_NUM"
#define REG_VAL_COUNT_RIBBON_2		"RIBBON2_NUM"
#define REG_VAL_COUNT_RIBBON_3		"RIBBON3_NUM"
#define REG_VAL_COUNT_RIBBON_4		"RIBBON4_NUM"

#define REG_VAL_COUNT_TS			"TS"

#define REG_VAL_COUNT_ALARM			"ALARM"


#define REG_KEY_BIN					"Software\\AMT\\AMT8590\\BIN\\"
#define REG_VAL_TS_BIN_HISTORY		"TS_BIN_HISTORY"

#define REG_KEY_TEACH				"Software\\AMT\\AMT8590\\TEACH\\"

#define REG_KEY_USE_SOCKET			"Software\\AMT\\AMT8590\\USE_SOCKET\\"
#define REG_KEY_SOCKET_CAUSE		"Software\\AMT\\AMT8590\\SOCKET_CAUSE\\"
#define REG_VAL_SOCKET_TS			"CSOCKET"

#define REG_KEY_USE_IDBUFFER		"Software\\AMT\\AMT8590\\USE_IDBUFFER\\"

//2014,1227
#define REG_VAL_CAPADENSITY			"Software\\AMT\\AMT8590\\CAPADEMSITY\\"
#define REG_VAL_DENSITY				"Software\\AMT\\AMT8590\\DEMSITY\\"
#define REG_VAL_BCR					"Software\\AMT\\AMT8590\\BCR\\"

#define REG_VAL_LABEL_BCR_SD		"Software\\AMT\\AMT8590\\LABELBCR\\SD"
#define REG_VAL_LABEL_BCR_LT		"Software\\AMT\\AMT8590\\LABELBCR\\LT"
#define REG_VAL_LABEL_BCR_LHX		"Software\\AMT\\AMT8590\\LABELBCR\\LHX"
#define REG_VAL_LABEL_BCR_LHY		"Software\\AMT\\AMT8590\\LABELBCR\\LHY"
#define REG_VAL_LABEL_BCR_MODE		"Software\\AMT\\AMT8590\\LABELBCR\\MODE"
#define REG_VAL_LABEL_BCR_COMPLETE		"Software\\AMT\\AMT8590\\LABELBCR\\COMPLETE"


class AMTRegistry  
{
public:
	AMTRegistry();
	virtual ~AMTRegistry();

	static UINT RegReadInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nDefault = 0 );	
	static BOOL RegWriteInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nData );
	static std::string RegReadString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault );
	static BOOL RegWriteString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData );
	static BOOL RegRemoveValue(LPCTSTR lpKey, LPCTSTR lpValue);

	static void RegWriteTime( SYSTEMTIME t );
	static SYSTEMTIME RegReadTime();
	static void RegWriteAlarmTime( SYSTEMTIME t );
	static SYSTEMTIME RegReadAlarmTime();
};

#endif // !defined(AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_)
