#if !defined(AFX_BARCODEFORMAT_H__9587F315_BF8A_4C31_A383_86FA4E66B6DC__INCLUDED_)
#define AFX_BARCODEFORMAT_H__9587F315_BF8A_4C31_A383_86FA4E66B6DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeFormat.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CBarcodeFormatClient

class CBarcodeFormatClient : public CObject
{
public:
	CBarcodeFormatClient();           // protected constructor used by dynamic creation
	virtual ~CBarcodeFormatClient();

// Attributes
public:
	CString		m_strHeader;
	CString		m_strData;

	int			m_nHeader;
	int			m_nHeaderLength;

// Operations
public:
	CString m_strLotID;
	CString m_strBoatID;
	CString m_strSerial;
	CString m_strLableSerial;
	CString m_strBcrFormat;
	CString m_strMsg;

// Implementation
public:
	void		ClrSerial() { m_strSerial = ""; }
	void		SetSerial(CString strSerial) { m_strSerial = strSerial; }
	CString		GetSerial() { return m_strSerial; }

	void		ClrBoatID() { m_strBoatID = ""; }
	void		SetBoatID(CString strBoatID) { m_strBoatID = strBoatID; }
	CString		GetBoatID() { return m_strBoatID; }

	void		ClrLotID() { m_strLotID = ""; }
	void		SetLotID(CString strLotID) { m_strLotID = strLotID; }
	CString		GetLotID() { return m_strLotID; }

	void		ClrLabelSerial() { m_strLableSerial = "";	}
	void		SetLabelSerial(CString strLableSerial) { m_strLableSerial = strLableSerial; }
	CString		GetLabelSerial() { return m_strLableSerial;	}

	void		ClrBcrFormat() { m_strBcrFormat = ""; }
	void		SetBcrFormat( CString strBcrFormat) { m_strBcrFormat = strBcrFormat; }
	CString		GetBcrFormat() { return m_strBcrFormat;	}

	void		ClrMsg() { m_strMsg = ""; }
	void		SetMsg( CString strMsg) { m_strMsg = strMsg; }
	CString		GetMsg() { return m_strMsg;	}




public:
	void		ClrVariable() { ClrSerial(); ClrBoatID(); ClrLotID(); ClrBcrFormat(); }
	void		OnDataReceive(CString strMsg);
	void		OnDataAnalysis(CString strRev);
	CString		OnBodyAnalysis( int nMode, CString strRecv, CString strFind );

	void		PX_XClose( int port, CString lotid, CString strOPer, CString strRetry, CString strCnt );
	void		OnBrandBcrLabelIssue( int port, CString lotid, CString boatid, CString strSerial, CString strLabel, CString strRibbon, CString mode, CString strRetry, CString strCnt );
	void		OnBrandDensityLabelIssue( int port, CString lotid, CString boatid, CString strSerial, CString strRetry, CString strCnt );
	void		OnBrandDensityLabelCCS( int port, CString lotid, CString boatid, CString strSerial, CString strMatCode, CString strRetry, CString strCnt );
	void		OnBrandDTBTLabelCCS( int port, CString lotid, CString boatid, CString strSerial, CString strMatCode, CString strRibonCode, CString strRetry, CString strCnt);
	void		OnProdSerial_Info( int port, CString lotid, CString boatid, CString strSerial, CString strRetry, CString strCnt );
	void		OnBoatID_Info( int port, CString boatid, CString strRetry, CString strCnt );
	void		OnPacking_Master_Info( int port, CString lotid, CString strSerial, CString strRetry, CString strCnt );
	void		OnBoatIDModify( int port, CString lotid, CString boatid, CString strSerial, CString strModify, CString strRetry, CString strCnt );

	void		OnReceived_BcrLabelIssue( CString strRecv );
	void		OnReceived_DensityLabelIssue( CString strRecv );
	void		OnReceived_XClose( CString strRecv );
	void		OnReceived_Density_Label_CCS( CString strRecv );
	void		OnReceived_DTBT_Label_CCS( CString strRecv );
	void		OnReceived_ProdSerialInfo( CString strRecv );
	void		OnReceived_BoatIDInfo( CString strRecv );
	void		OnReceived_PackingMasterInfo( CString strRecv );
	void		OnReceived_BoatIDModify( CString strRecv );

	void		SetSerialIn()		{ m_nrSerialFormat = NR_WAITING; }
	NET_RECEIVED m_nrSerialFormat;
	NET_RECEIVED m_nrMasterInfo;
	bool GetMasterInfoReceived() { return (m_nrMasterInfo == NR_RECEIVED); }
	bool GetMasterInfoFailReceived() { return (m_nrMasterInfo == NR_FAIL); }

	bool GetSerialInReceived() { return (m_nrSerialFormat == NR_RECEIVED); }
	bool GetSerialInFailReceived() { return (m_nrSerialFormat == NR_FAIL); }
	NET_RECEIVED m_nrDensityCCS;
	NET_RECEIVED m_nrBcrLabelCCS;

};

/////////////////////////////////////////////////////////////////////////////
extern CBarcodeFormatClient clsBcrFormatClient;

#endif // !defined(AFX_BARCODEFORMAT_H__9587F315_BF8A_4C31_A383_86FA4E66B6DC__INCLUDED_)
