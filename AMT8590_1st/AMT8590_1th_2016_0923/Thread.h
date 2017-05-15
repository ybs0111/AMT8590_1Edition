#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

// ******************************************************************************
// 쓰레드 및 쓰레드 핸들 저장 변수 선언                                          
// ******************************************************************************

enum 
{
	THREAD_HANDLER_CHK,
	THREAD_TOWER_LAMP_CHK,
	THREAD_ALARM_DISPLAY,
	THREAD_IO,
	THREAD_NETWORK,
	THREAD_MOTORS,

	THREAD_TOP_IDBUFFER_WORKS,
	THREAD_BTM_IDBUFFER_WORKS,
	THREAD_IDBUFFER_ELV,
	THREAD_DENSITY_FEEDERS,
	THREAD_DENSITY_WORKS,
	THREAD_BARCODE_FEEDERS,
	THREAD_BARCODE_WORKS,

	THREAD_ROLLER_WORKS,
	THREAD_TOP_VISION,
	THREAD_UNLOAD_WORKS,
	THREAD_UNLOAD_CONVEYOR,
	
	
	THREAD_COUNT,
};

extern CWinThread  *m_thread[THREAD_COUNT];
extern HANDLE		hThrHandle[THREAD_COUNT];
// ******************************************************************************

// ******************************************************************************
// 전역 쓰레드 함수 선언한다                                                     
// ******************************************************************************
UINT OnThread_Handler_Check(LPVOID  lParam);		
UINT OnThread_Tower_Lamp_Check(LPVOID  lParam);		
UINT OnThread_Alarm_Display(LPVOID  lParam);		
UINT OnThread_IO(LPVOID  lParam);					
UINT OnThread_Network(LPVOID lParam);
UINT OnThread_Motors(LPVOID lParam);
UINT OnThread_Top_IDBuffer_Works(LPVOID lParam);
UINT OnThread_Top_Move_Works(LPVOID lParam);
UINT OnThread_Btm_IDBuffer_Works(LPVOID lParam);
UINT OnThread_IDBuffer_Elevator(LPVOID lParam);
UINT OnThread_Density_Feeders(LPVOID lParam);
UINT OnThread_Density_Works(LPVOID lParam);
UINT OnThread_Barcode_Feeders(LPVOID lParam);
UINT OnThread_Barcode_Works(LPVOID lParam);

UINT OnThread_Roller_Works(LPVOID lParam);
UINT OnThread_Top_Vision(LPVOID lParam);
UINT OnThread_Unload_Works(LPVOID lParam);
UINT OnThread_Unload_Conveyor(LPVOID lParam);
// ******************************************************************************

// ******************************************************************************
//  동작 쓰레드 함수 선언한다                                                     
// ******************************************************************************
			
// ******************************************************************************

extern int All_Stop;  // 쓰레드 상태 플래그

#endif