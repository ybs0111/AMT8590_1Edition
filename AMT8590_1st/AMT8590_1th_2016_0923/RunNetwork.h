// RunNetwork.h: interface for the CRunNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNNETWORK_H__89BE6119_27E4_468D_BD3A_018B6398688E__INCLUDED_)
#define AFX_RUNNETWORK_H__89BE6119_27E4_468D_BD3A_018B6398688E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct st_barcode_info
{
	int nReady;				// label 발행준비중.......
	int nCommand;			// label 발행 요청........
	int nComplete;			// label 정보 완료.......

	CString strSerial;		// serial 정보......
	CString strBarcodeFormat; // barcode format......


	char m_cLotid[30];
	char m_cBoatid[4];
	char m_cSerial[30];
	char m_cLabelcode[30];
	char m_cRibboncode[30];
	char m_cMode[10];
	char m_cRetry[10];
	char m_cCnt[10];
	char m_cDencode[30];
	char m_cAccecode[30];
	char m_cTboxcode[30];
};

class CRunNetwork  
{
public:
	DWORD			m_dwTimeBcrFNetwork[4][3];

	int				m_nRetryCnt[2];
	int				m_nRunStep[2];

	CString			strJamCode;

	st_barcode_info st_barcodenet[2];

	void			OnRunMove();
	void			OnRunLabelCheck(int nSite);
	void			Thread_Run();
	void			Thread_Variable_Initial();

public:
	CRunNetwork();
	virtual ~CRunNetwork();

};

extern CRunNetwork g_RunNetwork;
#endif // !defined(AFX_RUNNETWORK_H__89BE6119_27E4_468D_BD3A_018B6398688E__INCLUDED_)
