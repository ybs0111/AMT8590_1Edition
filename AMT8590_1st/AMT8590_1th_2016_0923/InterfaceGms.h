#pragma once

class CComm;

class CInterfaceGms
{
public:
	CString			m_strChann[5];

	DWORD			m_dwWaitTime[3];

	double			m_dChann[5];

	int				m_nThreadStep;
	int				m_nChann[2];
	int				m_nComPort;			// 통신 포트
	int				m_nBaudRate;		// 통신 속도
	int				m_nDataBit;			// 데이터 비트
	int				m_nStopBit;			// stop 비트
	int				m_nParityBit;		// parity 비트

	BYTE			m_byEnd;

	bool			m_bRevFlag;
	bool			m_bConnect;			// rs-232c 연결 상태
	bool			m_bThread;

	HANDLE			m_thrHandle;

	CComm			*m_pCom;			// 통신 class 포인터

	bool			OnOpen(int nPort, int nBaudRate, int nParityBit, int nDataBit, int nStopBit, BYTE byEnd);
	void			OnClose();
	void			OnDataSend(CString strData);
	void			OnDataRevice(CString strData);
	void			OnGmsDataCheck();

public:
	CInterfaceGms(void);
	~CInterfaceGms(void);
};

extern CInterfaceGms clsGms;