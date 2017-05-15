// APartTray.h: interface for the APartTray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTTRAY_H__3B578633_0F74_49F9_B988_2D5674B676F9__INCLUDED_)
#define AFX_APARTTRAY_H__3B578633_0F74_49F9_B988_2D5674B676F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "PartFunction.h"

enum EIDBUFFER_POS
{
	IDPOS_NONE,
	IDPOS_FRONT,
	IDPOS_DENSITY,
	IDPOS_BARCODE1,
	IDPOS_BARCODE2,
	IDPOS_TOP_VISION,
	IDPOS_UNLOAD,
	IDPOS_CONVEYOR,
	IDPOS_REJECT,
	IPOOS_UNLOAD_BTM,
	IPOOS_UNLOAD_BTM_LOAD,
	IPOOS_UNLOAD_BTM_UNLOAD,
	IDPOS_BTM_UNLOAD_TO_NEXT,
	IDPOS_NEXT,
//	IDPOS_CONVEYOR,

	IDPOS_FRONT_TO_DENSITY,
	IDPOS_DENSITY_TO_BARCODE1,
	IDPOS_BARCODE1_TO_BARCODE2,
	IDPOS_BARCODE2_TO_VISION,
	IDPOS_TOPVISION_TO_UNLOAD,
	IDPOS_UNLOAD_TO_CONVEYOR,
	IDPOS_CONVEYOR_TO_NEXT,
	
	MAX_IDPOS,
};

enum ETRAY_POS
{
	TPOS_NONE,
	TPOS_CONV,
	TPOS_LOAD,
	TPOS_LDELV,
	TPOS_ULDELV,
	TPOS_UNLOAD,
	TPOS_AUNLOAD,
	TPOS_BUNLOAD,
//	TPOS_LOAD_UNLOAD,
//	TPOS_UNLOAD_LOAD,	
	TPOS_RETEST,
	TPOS_REJECT1,
	TPOS_REJECT2,	
	
	MAX_TPOS,
};

enum EBUFF_POS
{
	BPOS_NONE,
	BPOS_CONV,
	BPOS_LOAD,
	BPOS_LDELV,
	BPOS_ULDELV,
	BPOS_UNLOAD,
	
	BPOS_AUNLOAD,
	BPOS_BUNLOAD,
	BPOS_LOAD_UNLOAD,
	BPOS_UNLOAD_LOAD,	
	BPOS_RETEST,
//	BPOS_REJECT,
	BPOS_REJECT1,
	BPOS_REJECT2,	
	MAX_BUFFPOS,
};


class ATrayExtra
{
public:
	ATrayExtra(){}
	virtual ~ATrayExtra(){}

	// Get
	EIDBUFFER_POS	GetPos() { return m_trayPos; }
	EIDBUFFER_POS	GetTargetPos() { return m_trayTargetPos; }

	// Set		
	void		SetPos( EIDBUFFER_POS pos ) { m_trayPos = pos; }
	void		SetTargetPos( EIDBUFFER_POS pos ) {	m_trayTargetPos = pos; }	
	
protected:
	EIDBUFFER_POS	m_trayPos;
	EIDBUFFER_POS	m_trayTargetPos;
};


class ATray
{
public:
	ATray();
	virtual ~ATray();

	// Get
	EIDBUFFER_POS	GetPos() { return m_trayPos; }
	EIDBUFFER_POS	GetTargetPos() { return m_trayTargetPos; }
	int			GetBufferNo() { return m_nBufferNo; }
	bool		GetLastMdl() { return m_bLastMdl; }
	CString		GetStrLotID() { return m_strLotID; }
	CString		GetProductType() { return m_strProductID; }
	bool		GetEmptyTray() { return m_bEmptyTray; }
	bool		GetBypass() { return m_bBypass; }
	int			GetModuleCnt();
	int			GetModuleBcr1Cnt();
	int			GetModuleBcr2Cnt();
	int			GetModuleTopVisionCnt();
	int			GetModuleUnloadCnt();

	//2013,0715
	CString		GetProcessID() { return m_strProcessid; }
	CString		GetPropertyEnd() { return m_strPropertyEnd; }

	// Set
	void		SetPos( EIDBUFFER_POS pos );
	void		SetTargetPos( EIDBUFFER_POS pos );
				
	void		SetBufferNo( int iVal ) { m_nBufferNo = iVal; }
	void		SetLastMdl( bool bVal ) { m_bLastMdl = bVal; }
	void		SetLotID( CString strVal ) { m_strLotID = strVal; }
	void		SetProductType( CString strProductID ) { m_strProductID = strProductID; }

	void		SetEmptyTray( bool bVal ) { m_bEmptyTray = bVal; }
	void		SetBypass( bool bVal ) { m_bBypass = bVal; }
	//2013,0715
	void		SetProcessID( CString strid ) { m_strProcessid = strid; }
	void		SetPropertyEnd( CString strPEid ) { m_strPropertyEnd = strPEid; }


	void		SetTrayInfo( CString strLotID, int nBufferNo, bool bLastMdl, bool bEmptyTray, bool bBypass = false );

	void		SetModule( int nIdx, AModule mdl ) { m_mdl[nIdx] = mdl; }

	bool		IsExistModule( int iIdx ) { return m_mdl[iIdx].IsExist(); }

	AModule&	GetModule( int iIdx )	{ return m_mdl[iIdx]; }//Load 버퍼에서 받은 DVC

	void		Clear();

	bool		IsAllGood( bool bLastModule = false );
	bool		IsAllEmpty( bool bLastModule = false );

	bool		IsAllState( EMODULE_STATE ems );

	EMODULE_WORK_STATE GetWorkState() { return m_ews; }
	void SetWorkState( EMODULE_WORK_STATE ews ) { m_ews = ews; }
	
//	ALabel&	GetLabel( int iIdx )	     { return m_ldl[iIdx]; }
//	void SetLabel( int iIdx, ALabel ldl) { m_ldl[iIdx] = ldl; }
//
//	ABcrLabel&	GetBcrLabel( int iIdx )		 { return m_bcrldl[iIdx]; }
//	void SetBcrLabel( int iIdx, ABcrLabel ldl) { m_bcrldl[iIdx] = ldl; }
//
//	void SetTestResult( EMODULE_WORK_STATE nResult, int nPos ) { m_nTestResult[nPos] = nResult; }
//	int GetTestResult( int nPos ) { return m_nTestResult[nPos]; }
//
//	void SetBcrOut( int nSite, bool biOnOff) { m_bBcrInOff[nSite] = biOnOff; }
//	bool GetBcrOut( int nSite) { return m_bBcrInOff[nSite];	}


protected:
	EIDBUFFER_POS	m_trayPos;
	EIDBUFFER_POS	m_trayTargetPos;

	EMODULE_WORK_STATE m_nTestResult[2];

	int			m_nBufferNo;
	bool		m_bLastMdl;
	CString		m_strLotID;
	CString		m_strProductID;
	bool		m_bEmptyTray;
	bool		m_bBypass;
	//2013,0715
	CString		m_strProcessid;
	CString		m_strPropertyEnd;

	EMODULE_WORK_STATE m_ews;

	AModule		m_mdl[MAX_MODULE];
//	ALabel		m_ldl[MAX_MODULE];
//	ABcrLabel   m_bcrldl[MAX_MODULE];
	
	bool		m_bBcrInOff[2];
};

#define MAX_TRAY		12

class APartTray  
{
public:
	APartTray();
	virtual ~APartTray();

	ATray& GetTray( int iIdx ) { return m_tray[iIdx]; }
	ATrayExtra& GetTrayExtra( int iIdx ) { return m_tray_extra[iIdx]; }
	int GetRejCnt();
	int GetStopRejCnt();
	ATray& GetRejTopTray();


	bool CalcLastTray( int iIdx, CString strLotID );
	int GetTrayCnt_byLotID( CString strLotID );

	void SetTrayExtra( int iIdx, ATrayExtra trayExtra ) { m_tray_extra[iIdx] = trayExtra; }

protected:
	ATray		m_tray[MAX_TRAY];
	ATrayExtra	m_tray_extra[MAX_TRAY];

public:
	void AddTray( ATray tray );
	void AddSetTray( ATray tray, EIDBUFFER_POS pos);
	void TrayMoved( EIDBUFFER_POS posFrom, EIDBUFFER_POS posTo );
	void CheckAfterTrayMoved( int iIdx, EIDBUFFER_POS posFrom, EIDBUFFER_POS posTo );

	CString GetStrPos( EIDBUFFER_POS epos );
	EIDBUFFER_POS GetPosByStr( CString strPos ); 

	bool IsEnablePos( EIDBUFFER_POS epos);


	int GetRejSelectedTray();
	void SetRejSelected( int iSel );
	int GetRejSelected() { return m_nRejSelected; }

	void SetSelectedTray(int iSel) { m_nSelectedTray = iSel; }
	int GetSelectedTray() { return m_nSelectedTray; }

	int GetIdxByPos( EIDBUFFER_POS  epos );
	int GetIdxByTargetPos( EIDBUFFER_POS epos );

	int GetIdxByPos_Extra( EIDBUFFER_POS  epos );
	int GetIdxByTargetPos_Extra( EIDBUFFER_POS epos );

	std::vector<int> GetVecMustTestModule( int nLotIdx );
	std::vector<int> GetVecStacker3MustTestModule( int nLotIdx );//2012,1225
	std::vector<int> GetVecEmptyCell( int nLotIdx, int nExchange1 = -1, int nExchange2 = -1 );
	std::vector<int> GetVecStacker3EmptyCell( int nLotIdx, int nExchange1 = -1, int nExchange2 = -1 );//2012,1225
	std::vector<int> GetVecRejEmptyCell();
	std::vector<int> GetVecFailinTray(int nLotIdx);

	std::vector<int> GetSorterVecEmptyCell( int nLotIdx , EIDBUFFER_POS pos);
	std::vector<int> GetSorterVecMustTestModule( int nLotIdx );


	//2013,0321
	std::vector<int> GetVecGoodCell_3Stacker();
	std::vector<int> GetVecGoodCell_Sub();
	std::vector<int> GetVecEmptyCell_Pri();

	int GetLoadModuleCnt();

	void SetSendNextFromRejForce( bool bVal ) { m_bSendNext_fromRej_Force = bVal; }
	bool GetSendNextFromRejForce() { return m_bSendNext_fromRej_Force; }

	bool		IsWorkingRejToNext();
	void TrayMinus();
	//2015.0327
// 	void GetCheckBarcode1TrayID( int ix, CString strRbtBcr );
// 	void GetCheckBarcode2TrayID( int ix, CString strRbtBcr );
	int GetCheckBarcode1TrayID();
	int GetCheckBarcode2TrayID();


	std::vector<int> GetVecDensityTestModule( int nLotIdx );
//	std::vector<int> GetVecBarcode1Feeder();
//	std::vector<int> GetVecBarcode2Feeder();
	std::vector<int> GetVecBcr1TestModule( int nLotIdx );
	std::vector<int> GetVecBcr2TestModule( int nLotIdx );
	std::vector<int> GetVecTopVisTestModule( int nLotIdx );
	std::vector<int> GetVecUnloadTestModule( int nLotIdx );	
	std::vector<int> GetVecPickerUnloadConvey( int nLotIdx );
	std::vector<int> GetVecUnloadElvTray( int nLotIdx );
	std::vector<int> GetVecUnloadConveyorModule( int nLotIdx );	

	std::vector<int> GetVecFeederPickupLabel( int nLotIdx);

protected:
	std::vector<int>	m_vecRej;
	int					m_nRejSelected;
	int					m_nSelectedTray;

	bool				m_bSendNext_fromRej_Force;
};

extern APartTray g_Tray;


#endif // !defined(AFX_APARTTRAY_H__3B578633_0F74_49F9_B988_2D5674B676F9__INCLUDED_)
