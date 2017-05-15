#ifndef PART_FUNCTION_H
#define PART_FUNCTION_H

#include <DbgHelp.h>

enum ENUM_LABEL_POSITION
{
	MPOS_NONE = -1,
	MPOS_INDEX_PMACHINE = 0,

	MPOS_INDEX_DENSITY_FEEDER = 100,
	MPOS_INDEX_DENSITY = 200,
	MPOS_INDEX_BCR1_FEEDER = 300,
	MPOS_INDEX_BCR1_LOAD = 400,
	MPOS_INDEX_BCR2_FEEDER = 500,
	MPOS_INDEX_BCR2_LOAD = 600,
	MPOS_INDEX_TOPVISION = 700,
	MPOS_INDEX_UNLOAD = 800,
	MPOS_INDEX_CONVEYOR = 900,
	MPOS_INDEX_ELV = 1000,
	MPOS_INDEX_ULD_ELV = 1100,
	MPOS_INDEX_ULD_NEXT = 1200,
	MPOS_INDEX_REJECT = 1300,
	MAX_MPOS_INDEX = 1400,
};




enum
{
	MPOS_PICKER=0,
	MPOS_SORTER,
};

enum
{
	MPOS_INDEX_SITE1,
	MPOS_INDEX_SITE2,
	MPOS_INDEX_SITE3,
	MPOS_INDEX_SITE4,
	MPOS_INDEX_SITE5,
	MPOS_INDEX_LOADELV,
	MPOS_INDEX_UNLOADELV,
};

enum EMODULE_STATE
{
	EMS_NONE,
	EMS_LOAD,
	EMS_DENSITY,
	EMS_BARCODE,
	EMS_VISION,
	EMS_UNLOAD,
	EMS_CONVEYOR,
	EMS_REJECT,
};

enum EMODULE_BCR_STATE
{
	EMBS_NONE,
	EMBS_LOAD,//출력 병령전송
	EMBS_PASS,
	EMBS_FAIL,
	EMBS_SKIP,
};

enum EMODULE_WORK_STATE
{
	EWS_NONE,
	EWS_LOAD,
	EWS_DENSITY,
	EWS_BARCODE1,
	EWS_BARCODE2,
	EWS_TOPVISION,
	EWS_BTMVISION,
	EWS_CONVEYOR,
};

//enum EMODULE_STATE
//{
//	EMS_NONE,
//	EMS_LOAD,
//	EMS_READY,
//	EMS_READY_RECV,
//	EMS_START,
//	EMS_TEST,
//	EMS_ABORT,
//	EMS_ABORT_RECV,
//	EMS_GOOD,
//	EMS_FAIL,
//	EMS_BVISION_FAIL,
//	EMS_RELOAD,
//
//};

//enum EMODULE_POS
//{
//	EWS_DENSITY,
//	EWS_BARCODE1,
//	EWS_BARCODE2,
//	EWS_TOPVISION,
//	EWS_BTMVISION,
//};


#define MPOS_DIVISION	100
#define MAX_DENSITY_FEEDER 6
	
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)( // Callback 함수의 원형
	HANDLE hProcess, 
	DWORD dwPid, 
	HANDLE hFile, 
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

void CaptureScreenShot();

HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);
bool WriteDIB(LPCSTR szFile, HANDLE hDIB);
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo);

int DoModal_Msg( CString strMsg );
int DoModal_Select( CString strMsg );

COLORREF GetCokCol( int iIdx );

double GetPosCYAtCokStacker( int iIdx );
double GetPosCZByCnt( int iCnt );

void ReadBasicData_Int( int& nRead, CString strKey, int nInit, int nMin, int nMax, LPCSTR strFile );
void ReadBasicData_Double( double& dRead, CString strKey, double dInit, double dMin, double dMax , LPCSTR strFile );


int GetMdlIdx( int ix, int iy, int iw, int ih, bool bTurn );
COLORREF CalcModuleColor( AModule mdl );
COLORREF CalcModuleColor( EMODULE_BCR_STATE ems );

class ATopVision
{
public:
	ATopVision() { EraseData(); }
	virtual ~ATopVision() {}

	void SetLotNo( CString strLot ) { m_strLotNo = strLot; }
	EMODULE_BCR_STATE GetTopVisionState() { return m_ebs;}
	void SetTopVisionState(EMODULE_BCR_STATE ebs) { m_ebs = ebs; }	

	void EraseData() { m_strLotNo = ""; m_ebs = EMBS_NONE; }
	
protected:
	CString m_strLotNo;
	EMODULE_BCR_STATE m_ebs;

};

class ABtmVision
{
public:
	ABtmVision() { EraseData(); }
	virtual ~ABtmVision() {}

	void SetLotNo( CString strLot ) { m_strLotNo = strLot; }
	EMODULE_BCR_STATE GetBtmVisionState() { return m_ebs;}
	void SetBtmVisionState(EMODULE_BCR_STATE ebs) { m_ebs = ebs; }	

	void EraseData() { m_strLotNo = ""; m_ebs = EMBS_NONE; }

	void SetBtmVVisInfo( EMODULE_BCR_STATE ebs)
	{ m_ebs = ebs; }
	
protected:
	CString m_strLotNo;
	EMODULE_BCR_STATE m_ebs;

};

class ABcrLabel
{
public:
	ABcrLabel() { EraseData(); }
	virtual ~ABcrLabel() {}
	CString GetBcrInfo() { return m_strBcrInfo; }

	void SetLotNo( CString strLot ) { m_strLotNo = strLot; }
	void SetBcrInfo( CString strBcrInfo ) { m_strBcrInfo = strBcrInfo; }
//	void SetDensityCapa( CString strCapa ) { m_strCapa = strCapa; }
	void EraseData() { m_strLotNo = ""; m_strBcrInfo = ""; m_ebs = EMBS_NONE; }
	bool IsExist() { return (m_strBcrInfo != ""); }

	EMODULE_BCR_STATE GetBcrLabelState() { return m_ebs;}
	void SetBcrLabelState(EMODULE_BCR_STATE ebs) { m_ebs = ebs; }	
	bool IsBcrlabelLoadState() { return m_ebs == EMBS_LOAD; }


	void SetBcrLabelInfo( CString strLotNo, CString strBcrInfo, EMODULE_BCR_STATE ebs)
	{	m_strLotNo = strLotNo; m_strBcrInfo = strBcrInfo; m_ebs = ebs;}


protected:
	CString m_strLotNo;
	CString m_strBcrInfo;
	CString m_strCapa;
	EMODULE_BCR_STATE m_ebs;

};

class ALabel
{
public:
	ALabel() { EraseData(); }
	virtual ~ALabel() {}
	CString GetDensityCapa() { return m_strCapa; }

	bool IsExist() { return (m_strCapa != ""); }
	void SetLotNo( CString strLot ) { m_strLotNo = strLot; }
	void SetDensityCapa( CString strCapa ) { m_strCapa = strCapa; }
	void SetDensity2D( CString str2D ) { m_str2D = str2D; }
	void EraseData() { m_strLotNo = ""; m_str2D = ""; m_strCapa = ""; m_ebs = EMBS_NONE; }

	EMODULE_BCR_STATE GetLabelState() { return m_ebs;}
	void SetLabelState(EMODULE_BCR_STATE ebs) { m_ebs = ebs; }
	bool IsLabelLoadState() { return m_ebs == EMBS_LOAD; }
	

	void SetLabelInfo( CString strLotNo, CString str2D, CString strCapa, EMODULE_BCR_STATE ebs = EMBS_LOAD)
	{	m_strLotNo = strLotNo; 	m_str2D = str2D; m_strCapa = strCapa; m_ebs = ebs;}


protected:
	CString m_strLotNo;
	CString m_strCapa;
	CString m_str2D;
	EMODULE_BCR_STATE m_ebs;
};

class AModule
{
public:
	AModule() { AllEraseData(); }
	virtual ~AModule() {}

	// Get
	CString GetProductID() { return m_strProductID;	}
	CString GetSN() { return m_strSN; }//Buffer ID
	CString GetSerial() { return m_strSerial; }
	CString GetPPID() { return m_strPPID; }
	CString GetWWN() { return m_strWWN; }
	CString GetCSN() { return m_strCSN; }
	CString GetPSID() { return m_strPSID; }
	CString GetBin() { return m_strBin; }
	CString GetLotNo() { return m_strLotNo; }
	CString GetLabelPrintCode() { return m_strLabelPrintCode; }
	CString GetBufferSkip() { return m_strSkip; }
	CString GetCount() { return m_strCnt; }


	EMODULE_STATE GetModuleState() { return m_ems; }
	CString GetVar() { return m_strVar;	}

	void SetTestUnloadState( EMODULE_STATE ems) { m_ems = ems; }
	

	int		GetTestCnt() { return m_nTestCnt; }
	int		GetRetryCnt() { return mn_RetryCnt; }
	CString GetScrapCode() { return m_strScrapCode; }


	bool IsBufferSkip() { return (m_strSkip == "" || m_strSkip == "1");}

	bool IsExist() { return ( m_strSerial != ""); }
	bool IsIDBufferExist() { return (m_strSN != ""); }
	bool IsTestEnd() { return (IsExist() == false || atoi(m_strBin) > 0 ); }
	bool IsTestEndState() { return (m_ems == EMS_NONE || m_ems == EMS_NONE); }
	bool IsTestLoadState() { return ( m_ems == EMS_LOAD || m_ems == EMS_DENSITY); }
	bool IsTestDensityState() { return ( m_ems == EMS_DENSITY); }
	bool IsTestBarcodeState() { return ( m_ems == EMS_BARCODE); }
	bool IsTestTopVisionState() { return ( m_ems == EMS_VISION); }
	bool IsTestUnloadState() { return ( m_ems == EMS_UNLOAD); }
	bool IsTestConveyorState() { return ( m_ems == EMS_CONVEYOR); }


	CString GetStrState()
	{
		switch( m_ems )
		{
		case EWS_NONE:				return "NONE";
		case EWS_LOAD:				return "LOAD";
		case EWS_DENSITY:			return "DENSITY";
		case EWS_BARCODE1:			return "BARCODE1";
		case EWS_BARCODE2:			return "BARCODE2";
		case EWS_TOPVISION:			return "TOP_VISION";
		case EWS_BTMVISION:			return "BTM_VISION";
		case EWS_CONVEYOR:			return "CONVEYOR";
		}
		return "NONE";
	}

	// Set
	void SetProductID( CString strProductID ) { m_strProductID = strProductID; }
	void SetBufferSkip(CString strSkip) { m_strSkip = strSkip; }
	void SetSN( CString strSN ) { m_strSN = strSN; }//Buffer ID
	void SetSerial( CString strSerial ) { m_strSerial = strSerial; }
	void SetPPID( CString strPPID ) { m_strPPID = strPPID; }
	void SetWWN( CString strWWN ) { m_strWWN = strWWN; }
	void SetCSN( CString strCSN ) { m_strCSN = strCSN; }
	void SetPSID( CString strPSID ) { m_strPSID = strPSID; }
	void SetBin( CString strBin ) { m_strBin = strBin; }
	void SetScrapCode( CString strCode ) { m_strScrapCode = strCode; }
	void SetLotNo( CString strLotNo ) { m_strLotNo = strLotNo; }
	void SetModuleState( EMODULE_STATE ems ) { m_ems = ems; }
	void SetMRetryCnt(int nCnt) { mn_RetryCnt = nCnt; }
	void SetVar( CString strVar) { m_strVar = strVar;	}
	void SetLabelPrintCode( CString strLabelPrintCode) { m_strLabelPrintCode = strLabelPrintCode; }
	void SetCount( CString strCnt) { m_strCnt = strCnt;	}

	void EraseData() { m_strSkip="1"; m_strSerial = ""; m_strPPID = "";	m_strWWN = "";	m_strBin = ""; m_ems = EMS_NONE; m_nTestCnt = 0;
						m_strScrapCode = ""; m_strLotNo = ""; m_strCSN = ""; m_strPSID = ""; }
	void AllEraseData() { m_strProductID="", m_strSkip="1"; m_strSN = "";	m_strSerial = "";	m_strPPID = "";	m_strWWN = "";	m_strBin = ""; m_ems = EMS_NONE; m_nTestCnt = 0;
						m_strScrapCode = ""; m_strLotNo = ""; m_strCSN = ""; m_strPSID = ""; m_strVar="";}
	void SetModuleInfo( CString strProductID, CString strLotNo, CString strSkip, CString strSN, CString strSerial, CString strVar, EMODULE_STATE ems = EMS_LOAD )
	{	m_strProductID = strProductID; m_strLotNo = strLotNo; m_strSkip = strSkip; m_strSN = strSN;	m_strSerial = strSerial;	
		m_ems = ems; m_strVar = strVar; mn_RetryCnt = 0; }

	void SetTestedPos( int nIdx ) { m_nTestedPos[m_nTestCnt] = nIdx; }
	void AddTestCnt() { m_nTestCnt++; }
	void SetTestCnt( int nCnt ) { m_nTestCnt = nCnt; }

	int GetTestedPos( int nTestIdx ) { return m_nTestedPos[nTestIdx]; }
//	void SetDensityCapa( CString strCapa ) { m_strCapa = strCapa; }

	void SetLabel(ALabel ldl ) { m_ldl = ldl; }
	ALabel& GetLabel() { return m_ldl; }

	void SetBcrLabel(ABcrLabel bcrldl ) { m_bcrldl = bcrldl; }
	ABcrLabel& GetBcrLabel() { return m_bcrldl; }

	void SetTopVision(ATopVision topldl ) { m_topldl = topldl; }
	ATopVision& GetTopVision() { return m_topldl; }

	void SetBtmVision(ABtmVision btmldl ) { m_btmldl = btmldl; }
	ABtmVision& GetBtmVision() { return m_btmldl; }

	CString m_strSkip;

protected:
	ALabel m_ldl;
	ABcrLabel m_bcrldl;
	ATopVision m_topldl;
	ABtmVision m_btmldl;

	CString m_strProductID;
	CString m_strSN;
	CString m_strSerial;
	CString m_strPPID;
	CString m_strWWN;
	CString m_strCSN;
	CString m_strPSID;
	CString m_strBin;
	CString m_strScrapCode;
	CString m_strBoatID;
	CString m_strVar;
	CString m_strLabelPrintCode;
	CString m_strCapa;
	CString m_strCnt;

	EMODULE_STATE m_ems;

	int		m_nTestCnt;
	int		mn_RetryCnt;

	CString m_strLotNo;

	int		m_nTestedPos[12];
	int     m_nTestResult[5];
};

EMODULE_STATE GetStateByStr( CString strState );


#endif