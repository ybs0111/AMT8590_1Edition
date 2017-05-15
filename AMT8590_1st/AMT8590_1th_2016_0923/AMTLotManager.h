// AMTLotManager.h: interface for the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
#define AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum
{
	PRIME,
	CUM,
	CNT_TYPE_MAX,
};

#define BIN_MAX		9
// ALot
class ALot
{
public:
	ALot();
	ALot( CString strLotID, CString strPartID );
	virtual ~ALot();

	CString GetLotID() { return m_strLotID; }
	CString GetPartID() { return m_strPartID; }
	CString GetStrNewLotInPass() { if( m_bNewLotInPass ) return "YES"; else return "NO"; }
	bool GetNewLotInPass() { return m_bNewLotInPass; }
	CString GetStrStartNewLot() { if( m_bStartNewLot ) return "YES"; else return "NO"; }
	bool GetStartNewLot() { return m_bStartNewLot; }
	CString GetTimeStart();
	CString GetStrLastModule() { return m_strLastModule; }
	//2013,0910
	CString GetStrRProtyModule() { return m_strRProtyModule; }
	//2013,1224
	CString GetStrTotalModule() { return m_strTotalModule; }
	CString GetStrProcessidModule() { return m_strProcessidModule; }
	CString GetStrBypass() { return m_strBypass; }
	CString GetStrLane() { return m_strLane; }
	CString GetStrBcrTYpe() { return m_strBcrType; }
	CString GetLabelCode() { return m_strLabelCode; }
	CString GetRibbonCode() { return m_strRibbonCode; }
	CString GetDenLabelCode() { return m_strDensCode; }
	CString GetDenLabelCapa() { return m_strDensCapa; }
	CString GetXClose() { return m_strXClose; }
	CString GetProductName() { return m_strProductName; }
	CString GetMdlCapa() { return m_strMdlCapa; }

	CString GetStrSetLabelVisionModel() { if( m_bLabelVisMdl ) return "YES"; else return "NO"; }
	bool GetSetLabelVisionModel() { return m_bLabelVisMdl; }
	CString GetLabelVisionModel() { return m_strLabelVisModel; }

	CString GetStrSetPosVisionModel() { if( m_bPosVisMdl ) return "YES"; else return "NO"; }
	bool GetSetPosVisionModel() { return m_bPosVisMdl; }
	CString GetTBtmVisionModel() { return m_strTopBtmVisModel; }


	CString GetLotEnd() { return m_strLotEnd; }


	void SetNewLotInPass(bool bVal) { m_bNewLotInPass = bVal; }
	void SetStartNewLot(bool bVal) { m_bStartNewLot = bVal; }
	void SetLotID( CString strLotID ) {strLotID.MakeUpper(); m_strLotID = strLotID; }
	void SetPartID( CString strPartID ) { strPartID.MakeUpper(); m_strPartID = strPartID; }
	void SetLastModule( CString strLastModule )	{m_strLastModule = strLastModule; }
	void SetRProtyModule( CString strRProtyModule )	{m_strRProtyModule = strRProtyModule; }
	void SetProcessidModule( CString strProcessidModule )	{m_strProcessidModule = strProcessidModule; }
	void SetByPass( CString strBypass ) { m_strBypass = strBypass; }
	void SetTotalModule( CString strTotalMdl ) { m_strTotalModule = strTotalMdl; }
	void SetByLane( CString strLane ) { m_strLane = strLane; }
	void SetBcrType( CString strBcrType ) { m_strBcrType = strBcrType; }
	void SetLotEnd( CString strLotend ) { m_strLotEnd = strLotend; }
	void SetDenLabelCode( CString strDenCode ) { m_strDensCode = strDenCode; }
	void SetDenLabelCapa( CString strDensCapa) { m_strDensCapa = strDensCapa; }
	void SetDisLabelCode( CString strLabelCode) { m_strLabelCode = strLabelCode; }
	void SetRibbonCode( CString strRibbonCode) { m_strRibbonCode = strRibbonCode; }
	void SetXClose( CString strXClose ) { m_strXClose = strXClose; }
	void SetProductName( CString strProductName ) { m_strProductName = strProductName; }
	void SetUpcEanType(bool bUpc, bool bEan)
	{
		m_bUpc = bUpc;
		m_bEan = bEan;
	}
	bool GetUpc() { return m_bUpc;}
	bool GetEan() { return m_bEan;}
	void SetMdlCapa( CString strMdlCapa ) { m_strMdlCapa = strMdlCapa; }

	void SetStrLabelVisionModel(bool bVal) { m_bLabelVisMdl = bVal; }
	void SetLabelVisionModel( CString strHdlMdl, CString strMdl ) { m_strLabelVisModel = strHdlMdl + strMdl; }
	void SetLabelVisionModelUpdate( CString strHdlMdl ) { m_strLabelVisModel = strHdlMdl; }
	
	void SetStrPosVisionModel(bool bVal) { m_bPosVisMdl = bVal; }
	void SetTBtmVisionModel( CString strHdlMdl, CString strMdl="" ) { m_strTopBtmVisModel = strHdlMdl + strMdl; }
	void SetTBtmVisionModelUpdate( CString strHdlMdl ) { m_strTopBtmVisModel = strHdlMdl; }

	void AddInputCnt( int nPrimeCum );
	void AddPassCnt( int nPrimeCum );
	void AddFailCnt( int nPrimeCum );
	void AddReworkCnt( int nPrimeCum );//2019.0919
	void AddBinCnt( int nBin );

	int GetInputCnt( int nPrimeCum ) { return m_nCntInput[nPrimeCum]; }
	int GetPassCnt( int nPrimeCum ) { return m_nCntPass[nPrimeCum]; }
	int GetFailCnt( int nPrimeCum ) { return m_nCntFail[nPrimeCum]; }
	int GetReworkCnt( int nPrimeCum ) { return m_nCntRework[nPrimeCum]; }//2016.0919
	
	int GetBinCnt( int nBin ) { return m_nCntBin[nBin]; }

	int GetCokType() { return m_nCokType; }
	void SetCokType( int iCok ) { m_nCokType = iCok; }
	void CalcCokType();
	DWORD GetLotTime() { return m_dLotStartTime; }
	void SetLotTime() { m_dLotStartTime = GetCurrentTime();}

protected:
	CString m_strLotID;
	CString m_strPartID;
	CString m_strDensCapa;
	CString m_strDensCode;
	CString m_strLabelCode;
	CString m_strRibbonCode;
	CString m_strLotModel;
	bool m_bNewLotInPass;
	CString m_strXClose;
	CString m_strMdlCapa;
	CString m_strLabelVisModel;
	CString m_strTopBtmVisModel;
	bool m_bStartNewLot;
	bool m_bUpc;
	bool m_bEan;

	bool m_bLabelVisMdl;
	bool m_bPosVisMdl;

	SYSTEMTIME	m_timeStart;
	CString m_strLastModule;
	CString m_strRProtyModule;//2013,0910
	CString m_strProcessidModule;//2013,0910
	CString m_strBypass;
	CString m_strTotalModule;
	CString m_strLane;
	CString m_strBcrType;
	CString m_strLotEnd;
	CString m_strProductName;

	int m_nCokType;

	// Yield Info
	int m_nCntInput[CNT_TYPE_MAX];
	int m_nCntPass[CNT_TYPE_MAX];
	int m_nCntFail[CNT_TYPE_MAX];
	int m_nCntRework[CNT_TYPE_MAX];//20190919

	int m_nCntBin[BIN_MAX];

	DWORD m_dLotStartTime;
};


// AMTLotManager
class AMTLotManager  
{
public:
	AMTLotManager();
	virtual ~AMTLotManager();

	void OnNewLotIn( CString strLotID, CString strPartID );
	CString GetLotIDAt( int iIdx );
	ALot& GetLotAt( int iIdx );
	ALot& GetLotByLotID( CString strLotID );
	int GetLotCount() { return m_vecLot.size(); }


	void AddLot( CString strLotID, CString strPartID );
	void DeleteLot( int iIdx );

	void AddInputCnt( int nPrimeCum );
	void AddPassCnt( int nPrimeCum );
	void AddFailCnt( int nPrimeCum );
	void AddReworkCnt( int nPrimeCum );//2016.0919
	void AddBinCnt( int nBin );

	int GetInputCnt( int nPrimeCum ) { return m_nCntInput[nPrimeCum]; }
	int GetPassCnt( int nPrimeCum ) { return m_nCntPass[nPrimeCum]; }
	int GetFailCnt( int nPrimeCum ) { return m_nCntFail[nPrimeCum]; }
	int GetReworkCnt( int nPrimeCum ) { return m_nCntRework[nPrimeCum]; }//2016.0919

	int GetBinCnt( int nBin ) { return m_nCntBin[nBin]; }

	void ClearCountData();

	//2013,0913
	void OnNewPLotin(CString strLotID, CString strPartID );
	void AddPLot( CString strLotID, CString strPartID );
	int IsGetPLotID( CString strLotID);
	int GetPLotCount() { return m_vecPLot.size(); }
	ALot& GetPLotAt( int iIdx );
	void DeletePLot(CString strLotID);


protected:
	std::vector< ALot > m_vecLot;
	std::vector< ALot > m_vecPLot;
	ALot	m_dummyLot;
	ALot	m_dummyPLot;

	// Yield Info
	int m_nCntInput[CNT_TYPE_MAX];
	int m_nCntPass[CNT_TYPE_MAX];
	int m_nCntFail[CNT_TYPE_MAX];
	int m_nCntRework[CNT_TYPE_MAX];//20190919

	int m_nCntBin[BIN_MAX];

public:
	CString	m_strDensity[6];
	CString m_strBcrLabel[4];
	CString m_strBcrRibbon[4];
};

extern AMTLotManager g_lotMgr;

#endif // !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
