// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iostream.h"
#include "afxmt.h"
#include "Thread.h"

#include "Variable.h"  // 전역 변수 정의 클래스

//******************************************************************************
//쓰레드 동작 관련 클래스 추가                                                  
//******************************************************************************
#include "Run_Handler_Check.h"
#include "Run_TowerLamp.h"
#include "RUN_IO_ReadWrite.h"

#include "Public_Function.h"


#include "Run_Motors.h"
//******************************************************************************

#include "MyJamData.h"  // 알람 출력 정보 로딩 및 알람 메시지 저장 클래스 
#include "SrcPart/APartHandler.h"

#include "Run_Top_IDBuffer_Works.h"
#include "Run_Btm_IDBuffer_Works.h"
#include "Run_Barcode_Feeders.h"
#include "Run_Barcode_Works.h"
#include "Run_Density_Feeders.h"
#include "Run_Density_Works.h"
#include "Run_IDBuffer_El.h"
#include "Run_Roller_Works.h"

////2014.1016
#include "Run_Unload_Works.h"
#include "Run_Top_Vision.h"
////
#include "Run_Unload_Conveyor.h"//2014.1018
#include "Run_IDBuff_Input_Index.h"
#include "Run_IDBuff_Move_Index.h"
#include "RunNetwork.h"
#include "MesEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//******************************************************************************
//알람 출력 클래스 변수 선언                                                    
//******************************************************************************
CMyJamData   mcls_alarm_disp;  // 발생 알람 출력 정보 로딩 클래스 변수
//******************************************************************************

//******************************************************************************
//쓰레드 및 핸들 저장 변수 선언                                                 
//******************************************************************************
CWinThread  *m_thread[THREAD_COUNT];
HANDLE		hThrHandle[THREAD_COUNT];
//******************************************************************************

//******************************************************************************

CMutex mutex;
int All_Stop;		// 쓰레드 상태 플래그

//******************************************************************************
// 스위치 검사 동작 제어 쓰레드 함수                                            
//******************************************************************************
UINT OnThread_Handler_Check(LPVOID lParam)  // m_Thread[0]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock();

		Run_Handler_Check.ButtonCheck_Start();
 		Run_Handler_Check.ButtonCheck_Stop();
 		Run_Handler_Check.ButtonCheck_Alarm_Clear();
 		Run_Handler_Check.ButtonCheck_BuzzerOff();
		Run_Handler_Check.ButtonCheck_ManualAutoKey();
		Run_Handler_Check.SafetyCheck_Door();
		Run_Handler_Check.ButtonCheck_Recycle_Zone_Door();//2014,1207
		for ( int i = 0; i < 6; i++)
		{
			if( i < 4 )
			{
				//2014.0301
				//if( st_basic.nBcrFeederUse[i] == CTL_YES)
				{
					Run_Handler_Check.ButtonCheck_BarcodeLockUnLock(i);
				}
			}
			Run_Handler_Check.ButtonCheck_LabelLockUnLock(i);
//			Run_Handler_Check.ButtonCheck_LabelPaperLockUnLock(i);
		}
		////2015.0114
		Run_Handler_Check.OnRun_DoorLock();
		Func.OnMonitoring_Door();
		////
		g_handler.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************

//******************************************************************************
// 타워 램프 출력 동작 제어 쓰레드 함수                                         
//******************************************************************************
UINT OnThread_Tower_Lamp_Check(LPVOID lParam)  // m_Thread[1]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock();

		Run_TowerLamp.Thread_Run();
		Run_TowerLamp.Run_FormChange();

		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************

//******************************************************************************
// 장비 동작 중 발생한 알람 출력하기 위한 쓰레드 함수                           
//******************************************************************************
UINT OnThread_Alarm_Display(LPVOID lParam)  // m_Thread[2]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock();

 		mcls_alarm_disp.On_Alarm_Display(); // 알람 발생한 경우 알람 화면 출력 함수
		 
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************

//******************************************************************************
// IO 동작을 위한 쓰레드 함수
//******************************************************************************
UINT OnThread_IO(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		RUN_IO_ReadWrite.Run_Total_IORead();
	
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}

UINT OnThread_Network(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		evMes.OnSeq_RUN();//2014,1230
		g_RunNetwork.OnRunMove();
	
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}



UINT OnThread_Motors(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Motors.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Top_IDBuffer_Works(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();

//		g_Run_Top_IDBuffers.Thread_Run();
		g_Run_IDBuff_Input_Index.Thread_Run();
		g_Run_IDBuff_Move_Index.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Btm_IDBuffer_Works(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Btm_IDBuffers.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_IDBuffer_Elevator(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_IDBuffers_El.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Density_Feeders(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Density_Feeders.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Density_Works(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Density_Works.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Barcode_Feeders(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Barcode_Feeders.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Barcode_Works(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Barcode_Works.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Roller_Works(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Roller_Works.Thread_Run();
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Top_Vision(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Top_Vision.Thread_Run();//2014.1016
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Unload_Works(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		g_Run_Unload_Works.Thread_Run();//2014.1016
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}

UINT OnThread_Unload_Conveyor(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		//g_Run_Unload_Works.Thread_Run();
		g_Run_Unload_Conveyor.Thread_Run();//2014.1018
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;	
}
