// VisionClient.h: interface for the CVisionClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISIONCLIENT_H__F4BF48F1_592C_4F9F_B4C3_FA59CA1559FA__INCLUDED_)
#define AFX_VISIONCLIENT_H__F4BF48F1_592C_4F9F_B4C3_FA59CA1559FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVisionClient  
{
public:
	CString		m_strHeader;
	CString		m_strData;

	int			m_nHeader;
	int			m_nHeaderLength;

	CString		m_strBarcode;	// barcode Á¤º¸....

	int			m_nModelChangeBin;
	bool		OnVisionModelChange(int nPort, CString strModel);
	bool		OnVisionBcrResult(int nPort);

	int	    	GetResultBcrTest() { return m_nBcrResultBin; }
	void    	SetResultBcrTest( int nRetBin) { m_nBcrResultBin = nRetBin; }

	CString    	GetResultCapa() { return m_strCapa; }
	void	   	SetResultCapa( CString strCapa) { m_strCapa = strCapa; }

	CString    	GetResultSerial() { return m_strSerial; }
	void	   	SetResultSerial( CString strSerial) { m_strSerial = strSerial; }
	
	CString    	GetResult2D() { return m_str2D; }
	void	   	SetResult2D( CString str2D) { m_str2D = str2D; }

	void		OnDataReceive(CString strMsg);
	CString		OnBodyAnalysis(int nMode, CString strRev, CString strFind);
	void		OnDataAnalysis(CString strRev);

public:
	int			m_nBcrResultBin;
	CString		m_strCapa;
	CString		m_strSerial;
	CString		m_str2D;
	CString		m_strModel;		// model.....
	CString		m_strMsg;		// error message....
	CString		m_strUpc;
	CString		m_strEan;

public:
	CVisionClient();
	virtual ~CVisionClient();

};

extern CVisionClient clsVsClient[10];
#endif // !defined(AFX_VISIONCLIENT_H__F4BF48F1_592C_4F9F_B4C3_FA59CA1559FA__INCLUDED_)
