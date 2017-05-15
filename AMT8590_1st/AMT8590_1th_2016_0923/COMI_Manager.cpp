// COMI_Manager.cpp: implementation of the COMI_Manager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"

#include "COMI_Manager.h"
#include "..\common\ShareMem.h"

#include "Public_Function.h"
//#include "IO_Manager.h"
#include "SrcPart/APartHandler.h"
#include "SrcBase/ALocalization.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ComizoaPublic.h"
//#include "cmmsdkdef.h"
#include "Cmmsdk.h"

COMI_Manager g_comiMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COMI_Manager::COMI_Manager()
{

}

COMI_Manager::~COMI_Manager()
{

}


int COMI_Manager::HomeCheck_Mot(int n_Axis, int n_TimeOut)
{
	int iRtn = CTLBD_RET_GOOD;
	if( st_handler.mn_virtual_mode == 1 )
	{
		
	}
	else
	{
		iRtn = COMI.HomeCheck_Mot( n_Axis, n_TimeOut );
	}

	return iRtn;
}

int COMI_Manager::Get_MotPower(int n_Axis)
{
	int iRtn = CTL_GOOD;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = COMI.Get_MotPower( n_Axis );
	}

	return iRtn;
}

long COMI_Manager::_cmmSxIsDone(long Axis, long *IsDone)
{
	long nRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strTarget = g_sm.GetData( (SM_TYPE)(SMT_M_D_FEEDER_1_TARGET + Axis) );
		std::string strPosition = g_sm.GetData( (SM_TYPE)(SMT_M_D_FEEDER_1_POSITION + Axis) );
		int iXTarget = atoi( strTarget.c_str() );
		int iXPosition = atoi( strPosition.c_str() );

		if( iXTarget == iXPosition )
			*IsDone = cmTRUE;
		else
			*IsDone = cmFALSE;
	}
	else
	{
		nRtn = cmmSxIsDone( Axis, IsDone );
	}

	if( nRtn != cmERR_NONE )
	{
		CString strLog;
		strLog.Format( "_cmmSxIsDone(%d) : %d", Axis, nRtn );
		Func.On_LogFile_Add( LOG_TEST, strLog );
	}

	return nRtn;
}

long COMI_Manager::_cmmCfgSetSpeedPattern_T(long Axis, long SpeedMode, double WorkSpeed, double Accel, double Decel)
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = cmmCfgSetSpeedPattern_T( Axis, SpeedMode, WorkSpeed, Accel, Decel );
	}
	return iRtn;
}

long COMI_Manager::_cmmSxSetSpeedRatio(long Axis, long SpeedMode, double VelRatio, double AccRatio, double DecRatio)
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = cmmSxSetSpeedRatio( Axis, SpeedMode, VelRatio, AccRatio, DecRatio );
	}
	return iRtn;
}

long COMI_Manager::_cmmSxMoveToStart(long Axis, double Position)
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		// target 설정.
		char strData[32];
		itoa( Position, strData, 10 );
		g_sm.SetData( (SM_TYPE)(SMT_M_D_FEEDER_1_TARGET + Axis), strData );
	}
	else
	{
		iRtn = cmmSxMoveToStart( Axis, Position );
	}
	return iRtn;
}

double COMI_Manager::Get_MotCurrentPos(int n_Axis)
{
	double nRtn = CTLBD_RET_ERROR;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strPos = g_sm.GetData( (SM_TYPE)(SMT_M_D_FEEDER_1_POSITION + n_Axis) );
		int iPos = atoi( strPos.c_str() );
		nRtn = iPos;
	}
	else
	{
		nRtn = COMI.Get_MotCurrentPos( n_Axis );
	}

	return nRtn;
}

long COMI_Manager::_cmmMsCheckSlaveState(long SlaveAxis, long* SlaveState)
{
	long lRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		*SlaveState = 1;
	}
	else
	{
		lRtn = cmmMsCheckSlaveState( SlaveAxis, SlaveState );
	}
	return lRtn;
}

long COMI_Manager::_cmmIxIsDone( long MapIndex, long *IsDone )
{
	long nRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strXTarget = g_sm.GetData( SMT_M_D_ATTACH_X_TARGET );
		std::string strYTarget = g_sm.GetData( SMT_M_D_ATTACH_Y_TARGET );

		std::string strXPos = g_sm.GetData( SMT_M_D_ATTACH_X_POSITION );
		std::string strYPos = g_sm.GetData( SMT_M_D_ATTACH_Y_POSITION );

		int iXTarget = atoi( strXTarget.c_str() );
		int iXPosition = atoi( strXPos.c_str() );
		int iYTarget = atoi( strYTarget.c_str() );
		int iYPosition = atoi( strYPos.c_str() );
		
		if( iXTarget == iXPosition && iYTarget == iYPosition )
			*IsDone = cmTRUE;
		else
			*IsDone = cmFALSE;
	}
	else
	{
		nRtn = cmmIxIsDone( MapIndex, IsDone );
	}

	return nRtn;
}

long COMI_Manager::_cmmIxLineToStart( long MapIndex, double* PosList )
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		// target 설정.
		char strData[32];
		itoa( PosList[0], strData, 10 );
		g_sm.SetData( SMT_M_D_ATTACH_X_TARGET, strData );

		itoa( PosList[1], strData, 10 );
		g_sm.SetData( SMT_M_D_ATTACH_Y_TARGET, strData );
	}
	else
	{
		iRtn = cmmIxLineToStart( MapIndex, PosList );
	}
	return iRtn;
}

void COMI_Manager::OnMain_Motor_Setting()
{
	int i = 0, j = 0, nRet = 0;

	for (i = 0; i < MOTOR_COUNT; i++)
	{
		COMI.Set_Simulation_Mode(i, 0);
	}

		// 1mm 이동하는데 필요한 pulse
// 		COMI.Set_MotUnitDist(M_D_FEEDER_1, 66.66175627);
// 		COMI.Set_MotUnitDist(M_D_FEEDER_2, 66.66175627);
// 		COMI.Set_MotUnitDist(M_D_FEEDER_3, 66.66175627);
// 		COMI.Set_MotUnitDist(M_D_FEEDER_4, 66.66175627);
// 		COMI.Set_MotUnitDist(M_D_FEEDER_5, 66.66175627);
// 		COMI.Set_MotUnitDist(M_D_FEEDER_6, 66.66175627);
 		COMI.Set_MotUnitDist(M_D_FEEDER_1, 1);
 		COMI.Set_MotUnitDist(M_D_FEEDER_2, 1);
 		COMI.Set_MotUnitDist(M_D_FEEDER_3, 1);
 		COMI.Set_MotUnitDist(M_D_FEEDER_4, 1);
 		COMI.Set_MotUnitDist(M_D_FEEDER_5, 1);
 		COMI.Set_MotUnitDist(M_D_FEEDER_6, 1);


		
 		COMI.Set_MotUnitDist(M_BCR_FEEDER_1, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_FEEDER_2, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_FEEDER_3, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_FEEDER_4, 1000.000);
 
 		COMI.Set_MotUnitDist(M_D_VISION_X, 500.000);
 		COMI.Set_MotUnitDist(M_D_ATTACH_X, 500.000);
 		COMI.Set_MotUnitDist(M_D_ATTACH_Y, 500.000);
 		COMI.Set_MotUnitDist(M_D_ATTACH_Z1, 1000.000);
 		COMI.Set_MotUnitDist(M_D_ATTACH_Z2, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_VISION_X, 500.000);
 		COMI.Set_MotUnitDist(M_L_REJECT_X, 100.0031059);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH1_X, 500.000);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH1_Z1, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH1_R1, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603

 		COMI.Set_MotUnitDist(M_BCR_ATTACH1_Z2, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH1_R2, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603

 		COMI.Set_MotUnitDist(M_BCR_ATTACH2_X, 500.000);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH2_Z1, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH2_R1, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603

 		COMI.Set_MotUnitDist(M_BCR_ATTACH2_Z2, 1000.000);
 		COMI.Set_MotUnitDist(M_BCR_ATTACH2_R2, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603

 		COMI.Set_MotUnitDist(M_TOP_VISION_X, 500.000);
 		COMI.Set_MotUnitDist(M_BTM_VISION_X, 500.000);
 		COMI.Set_MotUnitDist(M_ULD_Y, 200.0062119);
 		COMI.Set_MotUnitDist(M_ULD_Z, 500.000);
 		COMI.Set_MotUnitDist(M_IDBUFFER_EL, 1000.000);
 		COMI.Set_MotUnitDist(M_TOPID_LD1, 1000.000);
 		//COMI.Set_MotUnitDist(M_TOPID_LD2, 1000.000);
		//2016.0319~2016.0330 박완수 출장으로 교체 예정
		COMI.Set_MotUnitDist(M_TOPID_LD2, 500.000);
 		COMI.Set_MotUnitDist(M_BTMID_ULD1, 156.2794021);
 		COMI.Set_MotUnitDist(M_BTMID_ULD2, 1000.000);//2014.1022 156.2794021);
 		COMI.Set_MotUnitDist(M_INDEX_X, 5000.000);
		COMI.Set_MotUnitDist(M_INDEX_ROLLER1, 212.2065908);// 106.1032954); 2014.1108 ysj
		COMI.Set_MotUnitDist(M_INDEX_ROLLER2, 212.2065908);// 106.1032954); 2014.1108 ysj
  		COMI.Set_MotUnitDist(M_ULD_ROLLER_PITCH, 2000.000);//2014,1207 용량라벨 가운데 추가
		COMI.Set_MotUnitDist(M_ULD_CV_PITCH, 1000.000);
		COMI.Set_MotUnitDist(M_ULD_STACKER_Z, 1000.000);


		// 1mm 이동하는데 필요한 pulse (속도는 1mm/초로 세팅하게 된다.)
// 		COMI.Set_MotUnitSpeed(M_D_FEEDER_1, 66.66175627);
// 		COMI.Set_MotUnitSpeed(M_D_FEEDER_2, 66.66175627);
// 		COMI.Set_MotUnitSpeed(M_D_FEEDER_3, 66.66175627);
// 		COMI.Set_MotUnitSpeed(M_D_FEEDER_4, 66.66175627);
// 		COMI.Set_MotUnitSpeed(M_D_FEEDER_5, 66.66175627);
// 		COMI.Set_MotUnitSpeed(M_D_FEEDER_6, 66.66175627);
 		COMI.Set_MotUnitSpeed(M_D_FEEDER_1, 1);
 		COMI.Set_MotUnitSpeed(M_D_FEEDER_2, 1);
 		COMI.Set_MotUnitSpeed(M_D_FEEDER_3, 1);
 		COMI.Set_MotUnitSpeed(M_D_FEEDER_4, 1);
 		COMI.Set_MotUnitSpeed(M_D_FEEDER_5, 1);
 		COMI.Set_MotUnitSpeed(M_D_FEEDER_6, 1);



 		COMI.Set_MotUnitSpeed(M_BCR_FEEDER_1, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_FEEDER_2, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_FEEDER_3, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_FEEDER_4, 1000.000);
 
 		COMI.Set_MotUnitSpeed(M_D_VISION_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_D_ATTACH_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_D_ATTACH_Y, 500.000);
 		COMI.Set_MotUnitSpeed(M_D_ATTACH_Z1, 1000.000);
 		COMI.Set_MotUnitSpeed(M_D_ATTACH_Z2, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_VISION_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_L_REJECT_X, 100.0031059);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH1_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH1_Z1, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH1_R1, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH1_Z2, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH1_R2, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH2_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH2_Z1, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH2_R1, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH2_Z2, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BCR_ATTACH2_R2, 88.91337603);//889.1337603);//2014,1125 889.1337603 -> 88.91337603
 		COMI.Set_MotUnitSpeed(M_TOP_VISION_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_BTM_VISION_X, 500.000);
 		COMI.Set_MotUnitSpeed(M_ULD_Y, 200.0062119);
 		COMI.Set_MotUnitSpeed(M_ULD_Z, 500.000);
 		COMI.Set_MotUnitSpeed(M_IDBUFFER_EL, 1000.000);
 		COMI.Set_MotUnitSpeed(M_TOPID_LD1, 1000.000);
 		COMI.Set_MotUnitSpeed(M_TOPID_LD2, 1000.000);
 		COMI.Set_MotUnitSpeed(M_BTMID_ULD1, 156.2794021);
 		COMI.Set_MotUnitSpeed(M_BTMID_ULD2, 1000.000);//2014.1022 156.2794021);
 		COMI.Set_MotUnitSpeed(M_INDEX_X, 5000.000);
		COMI.Set_MotUnitSpeed(M_INDEX_ROLLER1, 212.2065908);// 106.1032954); 2014.1108 ysj
		COMI.Set_MotUnitSpeed(M_INDEX_ROLLER2, 212.2065908);// 106.1032954); 2014.1108 ysj
		COMI.Set_MotUnitSpeed(M_ULD_ROLLER_PITCH, 2000.000);//2014,1207
 		COMI.Set_MotUnitSpeed(M_ULD_CV_PITCH, 1000.000);
		COMI.Set_MotUnitSpeed(M_ULD_STACKER_Z, 1000.000);


//	COMI.md_limit_position[MOTOR_BIN_1_R][0] = -3000;
//	COMI.md_limit_position[MOTOR_BIN_2_R][0] = -3000;
//	COMI.md_limit_position[MOTOR_REJECT_R][0] = -3000;

	for (i = 0; i < MOTOR_COUNT; i++)
	{
		cmmCfgSetCtrlMode(i, cmCTRL_SEMI_C);//cmCTRL_SEMI_C , 엔코더,(피드백기준) 기준으로 목표좌표를 설정하여 이송합니다
	}

	for (i = 0; i < MOTOR_COUNT; i++)
	{ 
		COMI.mn_homecheck_method[i] = 6;		//el on-> stop-> back-> el off - > stop 
	}

//	COMI.mn_homecheck_method[MOTOR_BIN_1_R] = 1;
//	COMI.mn_homecheck_method[MOTOR_BIN_2_R] = 1;
//	COMI.mn_homecheck_method[MOTOR_REJECT_R] = 1;

	for (i = 0; i < MOTOR_COUNT; i++)
	{
		nRet = COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		
		if (nRet != CTL_YES)
		{
			sprintf(st_msg.c_abnormal_msg, "Set_HomeSet_Config Error [MotNum[%d]]", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
	}

	// 최대 출력 주파수를 설정하는 명령으로 최소는 
	// 15.26 pps(min) ~ 1,000,000 pps(max), 6000rpm

	////////////////////////최소 주파수 설정 공식////////////////////////////////////////////
	//MinPPS = MaxPPS / 65,535
	//예를 들어서 MaxPPS 가 655,350 이면 MinPPS = 655,350 / 65,535 = 10 (PPS)이 됩니다.
	/////////////////////////////////////////////////////////////////////////////////////////
//	cmmCfgSetSpeedRange(MOTOR_ROBOT_Z, 1000000); 
//
	COMI.Set_MotInitSpeed(M_D_ATTACH_Z1, 10); 
	COMI.Set_MotInitSpeed(M_D_ATTACH_Z2, 10);
	COMI.Set_MotInitSpeed(M_BCR_ATTACH1_Z1, 10);
	COMI.Set_MotInitSpeed(M_BCR_ATTACH1_Z2, 10);
	COMI.Set_MotInitSpeed(M_BCR_ATTACH2_Z1, 10);
	COMI.Set_MotInitSpeed(M_BCR_ATTACH2_Z2, 10);
	COMI.Set_MotInitSpeed(M_IDBUFFER_EL, 10);
	COMI.Set_MotInitSpeed(M_ULD_STACKER_Z, 10);
	COMI.Set_MotInitSpeed(M_D_FEEDER_1, 10); 
	COMI.Set_MotInitSpeed(M_D_FEEDER_2, 10); 
	COMI.Set_MotInitSpeed(M_D_FEEDER_3, 10); 
	COMI.Set_MotInitSpeed(M_D_FEEDER_4, 10); 
	COMI.Set_MotInitSpeed(M_D_FEEDER_5, 10); 
	COMI.Set_MotInitSpeed(M_D_FEEDER_6, 10); 

	for ( i = 0;i < 6; i++ )
	{
		COMI.Set_Motor_IO_Property(M_D_FEEDER_1 + i, cmSD_MODE, cmFALSE);//cmTRUE/cmFALSE
		COMI.Set_Motor_IO_Property(M_D_FEEDER_1 + i, cmSD_EN, cmFALSE);//cmFALSE:초기속도로 감속 cmTRUE: 감속후 정지
		COMI.Set_Motor_IO_Property(M_D_FEEDER_1 + i, cmSD_LOGIC, cmLOGIC_A);//cmLOGIC_A : 0 A접점
		COMI.Set_Motor_IO_Property(M_D_FEEDER_1 + i, cmSD_LATCH, cmFALSE);//cmTRUE/cmFALSE

		// added ring counter function
		cmmCfgSetRingCntr(M_D_FEEDER_1 + i, cmCNT_COMM, 1, 10000);
		cmmCfgSetRingCntr(M_D_FEEDER_1 + i, cmCNT_FEED, 1, 10000);

	}

	for (i = 0; i < MOTOR_COUNT; i++)
	{
		COMI.Set_MotPower(i, IO_ON);
	}

	if (st_handler.cwnd_list != NULL)  //2010.1027 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, "[OnMain_Motor_Setting]");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	}



}