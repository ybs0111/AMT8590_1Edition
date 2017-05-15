#if !defined(AFX_AEC_CLIENT_H__228BD529_84B6_4FE7_9F36_52D701A1D74E__INCLUDED_)
#define AFX_AEC_CLIENT_H__228BD529_84B6_4FE7_9F36_52D701A1D74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AEC_Client.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// AEC_Client

class AEC_Client : public CObject
{
public:
	AEC_Client();
	virtual ~AEC_Client();

// Attributes
protected:
	NET_RECEIVED	m_nrMasterInfo;
	DWORD	m_dwTime_MasterInfo;
	CString	m_strMasterInfo;
	CString	m_strLotIDMaster;
	CString m_strSerial;

	 //BRAND_DENSITY_LABLE_ISSUE
	NET_RECEIVED	m_nrDensityID;//Modify Mode
	DWORD	m_dwTime_DensityIn;
	CString m_strDensityLot;
	CString	m_strDensityIn;
	CString m_strBufID;
	int m_nCnt_DensityIn;
	CString m_strDensityInRetry;

	 //Serial Info
	NET_RECEIVED	m_nrSerialInfo;
	DWORD	m_dwTime_SerialInfo;
	CString	m_strSerialInfo;
	int m_nCnt_SerialInfo;
	CString m_strSerialInfoRetry;


	NET_RECEIVED	m_nrProdSerial;
	DWORD	m_dwTime_ProdSerial;
	int	m_nCnt_ProdSerial;
	CString m_strProdSerial;
	CString m_strProdSerialRetry;

	NET_RECEIVED	m_nrBoatIDInfo;
	DWORD	m_dwTime_BoatIDInfo;
	CString	m_strBoatIDInfo;
	int m_nCnt_BoatIDInfo;
	CString m_strBoatIDInfoRetry;

	CString m_strSerialInRetry;
	int		m_nCnt_SerialIn;
	
// Operations
public:
	CString		m_strHeader;
	CString		m_strData;
	
	int			m_nHeader;
	int			m_nHeaderLength;
	
public:
	void OnMove_MasterInfo( int port, CString strLotID, CString strSerial );
	void SetMasterInfo( CString strLotID, CString strSerial ) {  m_strMasterInfo = strLotID; m_strSerial = strSerial; }
	bool GetMasterInfo() { return (m_nrMasterInfo == NR_RECEIVED); }

	void OnMove_DensityIn(int port, CString strLotID, CString strBoatID, CString strSerial, CString strRetry, CString strCnt );
	void SetDensityInfo( CString strLotID, CString strDensityIn, CString strBufID) 
	{ m_strDensityLot = strLotID; m_strDensityIn = strDensityIn; m_strBufID = strBufID;	}
//
//	void OnMove_SerialInfo();
//	void SetSerialInfo( CString serial) { m_strSerialInfo = serial; }
//	bool GetSerialInfo() { return (m_nrSerialInfo == NR_RECEIVED); }

	void OnBoatIDModify( int port, CString lotid, CString boatid, CString strSerial, CString strModify, CString strRetry, CString strCnt );
//	void SetBoatid_Modify( CString serial) { m_strBoatIDModify = serial; }
		
	void OnProd_Serial_Info( int nPort, CString strSerial, CString strRetry, CString strCnt );
	void SetProdSerialInfo( CString serial) { m_strBoatIDInfo = serial; }

//	void OnMove_BoatID_Info();
//	void SetBoatid_Info( CString strid) { m_strBoatIDInfo = strid; }
//	void OnReceived_BoatIDInfo( CString strRecv );


	void		OnDataReceive(CString strMsg);
	void		OnDataAnalysis(CString strRev);
	CString		OnBodyAnalysis( int nMode, CString strRecv, CString strFind );

	void OnReceived_BoatID( CString strRecv );
	void OnReceived_MasterInfo( CString strRecv );
	void OnReceived_DensityDataIn( CString strRecv );
	void OnReceived_ProdSerialInfo( CString strRecv );
	void OnReceived_BoatIDModify( CString strRecv );


	NET_RECEIVED m_nrSerialFormat;
	NET_RECEIVED m_nrBoatID;
	NET_RECEIVED GetSerialInReceived() { return (m_nrSerialFormat); }

};

/////////////////////////////////////////////////////////////////////////////
extern AEC_Client clsEC_Client;

#endif // !defined(AFX_AEC_CLIENT_H__228BD529_84B6_4FE7_9F36_52D701A1D74E__INCLUDED_)
