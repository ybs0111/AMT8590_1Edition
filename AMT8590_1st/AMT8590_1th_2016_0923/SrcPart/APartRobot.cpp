// APartRobot.cpp: implementation of the APartRobot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartRobot.h"
#include "../AMTLotManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APicker::APicker()
{
	m_nTargetPos = -1;
	m_nPickPos = -1;
}

APicker::~APicker()
{
	
}

//////////////////////////////////////////////////////////////////////////
APartRobot g_robot[10];
//APartRobot g_sorter_robot;
//APartRobot g_density_robot;
//APartRobot g_bcrfeeder_robot;
//APartRobot g_bcr1_robot;
//APartRobot g_bcr2_robot;
//APartRobot g_vision_robot;
//APartRobot g_unload_robot;
//APartRobot g_conveyor_robot;
//APartRobot g_unload_elv;

APartRobot::APartRobot()
{

}

APartRobot::~APartRobot()
{

}

bool APartRobot::IsLabelPicking()
{
	for( int i=0; i<PICKER_CNT; i++ )
	{
		if( m_Picker[i].GetLabel().IsExist() )
			return true;
	}

	return false;

//	int iIdx = g_Tray.GetIdxByPos( IDPOS_DENSITY );
//	if( iIdx < 0 )	return true;
//	int nCnt = 0;
//	for( int i=0; i<PICKER_CNT; i++ )
//	{		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//
//		if( trayPri.GetModule(i).IsExist() )
//			nCnt++;
//	}
//
//	if( nCnt > 0 ) return true;
//	
//	return false;
}

bool APartRobot::IsBcr1Picking()
{
	for( int i=0; i<PICKER_CNT; i++ )
	{
		if( m_Picker[i].GetBcrLabel().IsExist() )
			return true;
	}

	return false;
//	int iIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE1 );
//	if( iIdx < 0 )	return true;
//	int nCnt = 0;
//	for( int i=0; i<PICKER_CNT; i++ )
//	{		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//
//		if( trayPri.GetModule(i).IsExist() )
//			nCnt++;
//	}
//
//	if( nCnt > 0 ) return true;
//	
//	return false;
}

bool APartRobot::IsBcr2Picking()
{
	for( int i=0; i<PICKER_CNT; i++ )
	{
		if( m_Picker[i].GetBcrLabel().IsExist() )
			return true;
	}

	return false;

//	int iIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE2 );
//	if( iIdx < 0 )	return true;
//	int nCnt = 0;
//	for( int i=0; i<PICKER_CNT; i++ )
//	{		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//
//		if( trayPri.GetModule(i).IsExist() )
//			nCnt++;
//	}
//
//	if( nCnt > 0 ) return true;
//	
//	return false;
}

bool APartRobot::IsTopVisionPicking()
{
//	for( int i=0; i<PICKER_CNT; i++ )
//	{
//		if( m_Picker[i].GetModule().IsExist() )
//			return true;
//	}
//
//	return false;
// 	int iIdx = g_Tray.GetIdxByPos( IDPOS_TOP_VISION );
// 	if( iIdx < 0 )	return false;
	int nCnt = 0;
	for( int i=0; i<PICKER_CNT; i++ )
	{		
// 		ATray trayPri = g_Tray.GetTray( iIdx );
// 		if( trayPri.GetModule(i).IsExist() && trayPri.GetModule(i).GetTopVision().GetTopVisionState() == EMBS_NONE)
		if(st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[i])
			nCnt++;
	}
	if( nCnt > 0 ) return true;
	
	return false;
}

bool APartRobot::IsUnloadPicking()
{
	for( int i=0; i<PICKER_CNT; i++ )
	{
		if( m_Picker[i].GetModule().IsExist() )
			return true;
	}
	return false;


//	int iIdx = g_Tray.GetIdxByPos( IDPOS_UNLOAD );
//	if( iIdx < 0 )	return true;
//	int nCnt = 0;
//	for( int i=0; i<PICKER_CNT; i++ )
//	{		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//
//		if( trayPri.GetModule(i).IsExist() )
//			nCnt++;
//	}
//
//	if( nCnt > 0 ) return true;
//	
//	return false;
}

bool APartRobot::IsElvChecking()
{
	int iIdx = g_Tray.GetIdxByPos( IDPOS_UNLOAD );
	if( iIdx < 0 )	return true;
	for( int i=0; i<PICKER_CNT; i++ )
	{
		ATray trayPri = g_Tray.GetTray( iIdx );
		
		if( !trayPri.GetModule(i).IsExist() && trayPri.GetModule(i).IsIDBufferExist() )
			return true;
	}

	return false;
}

bool APartRobot::IsPicking()
{
//	for( int i=0; i<PICKER_CNT; i++ )
//	{
////		if( m_Picker[i].GetModule().IsExist() )
////			return true;
//	}

	int iIdx = g_Tray.GetIdxByPos( IDPOS_CONVEYOR );
	if( iIdx < 0 )	return true;
	for( int i=0; i<PICKER_CNT; i++ )
	{
		ATray trayPri = g_Tray.GetTray( iIdx );
		
		if( trayPri.GetModule(i).IsExist() )
			return true;
	}

	return false;
}

void APartRobot::AddJob( int bPick, int iRobotFirst, int iRobotSecond, int iPosFirst )
{
	if( iPosFirst < 0 )
	{
		return;
	}

	stJobPicker st;
	st.Set( bPick, iRobotFirst, iRobotSecond, iPosFirst );

	m_vecJob.push_back( st );
}

stJobPicker& APartRobot::GetFirstJob()
{
	if( m_vecJob.size() > 0 )
		return m_vecJob.at(0);

	return m_dummyJob;
}

void APartRobot::EraseFirstJob()
{
	if( m_vecJob.size() > 0 )
		m_vecJob.erase( m_vecJob.begin() );
}

stJobPicker& APartRobot::GetJob( int iIdx )
{
	if( m_vecJob.size() > iIdx )
		return m_vecJob.at(iIdx);

	return m_dummyJob;
}

int APartRobot::GetModuleCnt()
{
	int nRtn = 0;
//	for( int i=0; i<PICKER_CNT; i++ )
//	{
//		if( m_Picker[i].GetModule().IsExist() &&
//			m_Picker[i].GetModule().GetLotNo() == g_lotMgr.GetLotIDAt(0) )
//			nRtn++;
//
//		if( m_Picker[i].GetLabel().IsExist() )
//			nRtn++;
//	}
//
	return nRtn;
}



//////////////////////////////////////////////////////////////////////////
ENUM_LABEL_POSITION GetPosByTarget( int nTarget )
{
	if	   ( nTarget < MPOS_INDEX_DENSITY_FEEDER )	return MPOS_INDEX_PMACHINE;
	else if( nTarget < MPOS_INDEX_DENSITY )			return MPOS_INDEX_DENSITY_FEEDER;
	else if( nTarget < MPOS_INDEX_BCR1_FEEDER )		return MPOS_INDEX_DENSITY;

	else if( nTarget < MPOS_INDEX_BCR1_LOAD )		return MPOS_INDEX_BCR1_FEEDER;
	else if( nTarget < MPOS_INDEX_BCR2_FEEDER )		return MPOS_INDEX_BCR1_LOAD;
	else if( nTarget < MPOS_INDEX_BCR2_LOAD )		return MPOS_INDEX_BCR2_FEEDER;
	else if( nTarget < MPOS_INDEX_TOPVISION )		return MPOS_INDEX_BCR2_LOAD;
	else if( nTarget < MPOS_INDEX_UNLOAD )			return MPOS_INDEX_TOPVISION;

	else if( nTarget < MPOS_INDEX_CONVEYOR )		return MPOS_INDEX_UNLOAD;
	else if( nTarget < MPOS_INDEX_ELV )				return MPOS_INDEX_CONVEYOR;

	else if( nTarget < MPOS_INDEX_ULD_ELV )			return MPOS_INDEX_CONVEYOR;
	else if( nTarget < MPOS_INDEX_ULD_ELV )			return MPOS_INDEX_ELV;
	else if( nTarget < MPOS_INDEX_ULD_NEXT )		return MPOS_INDEX_ULD_ELV;
	else if( nTarget < MPOS_INDEX_REJECT )			return MPOS_INDEX_ULD_NEXT;

// 	else if( nTarget < MPOS_INDEX_ULD_ELV )			return MPOS_INDEX_ELV;
// 	else if( nTarget < MPOS_INDEX_ULD_NEXT )		return MPOS_INDEX_ULD_ELV;
// 	else if( nTarget < MPOS_INDEX_REJECT )			return MPOS_INDEX_ULD_NEXT;
	else											return MPOS_INDEX_REJECT;
};


int GetPosPartByTarget( int nTarget )
{
	return GetPosByTarget( nTarget );
};
