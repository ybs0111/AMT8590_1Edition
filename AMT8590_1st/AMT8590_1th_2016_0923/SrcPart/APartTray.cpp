// APartTray.cpp: implementation of the APartTray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "APartTray.h"
#include "APartHandler.h"
#include "../AMTLotManager.h"
#include "APartTestSite.h"
#include "../IO_Manager.h"
#include "APartRobot.h"

#include <map>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
ATray::ATray()
{
	m_trayPos = IDPOS_NONE;
	m_trayTargetPos = IDPOS_NONE;

	m_nBufferNo = -1;
	m_bLastMdl = false;
	m_strLotID = "";
	m_bEmptyTray = true;
	m_bBypass = false;

}

ATray::~ATray()
{
	
}

void ATray::SetTrayInfo( CString strLotID, int nBufferNo, bool bLastMdl, bool bEmptyTray, bool bBypass )
{
	m_strLotID = strLotID;
	m_nBufferNo = nBufferNo;
	m_bLastMdl = bLastMdl;
	m_bEmptyTray = bEmptyTray;

	m_bBypass = bBypass;
}

void ATray::Clear()
{
	m_trayPos = IDPOS_NONE;
	m_trayTargetPos = IDPOS_NONE;

	m_nBufferNo = -1;
	m_bLastMdl = false;
	m_strLotID = "";
	m_bEmptyTray = true;
	m_bBypass = false;

	for( int i=0; i< MAX_MODULE; i++ )
	{
		m_mdl[i].AllEraseData();
	}
}

bool ATray::IsAllGood( bool bLastModule /*= false */ )
{
	for( int i=0; i< MAX_BUFFDVC/*MAX_MODULE*/; i++ )
	{
		if( !bLastModule && m_mdl[i].IsExist() == false )
			return false;

		if( m_mdl[i].IsExist() && m_mdl[i].GetModuleState() != IDPOS_UNLOAD )
			return false;
	}	

	return true;
}

bool ATray::IsAllEmpty( bool bLastModule /*= false */ )
{
	int nTotalCnt = MAX_BUFFDVC;
	for( int i=0; i< nTotalCnt/*MAX_MODULE*/; i++ )
	{
		if( !bLastModule && m_mdl[i].IsExist() == false )
			return false;

		if( m_mdl[i].IsExist() && m_mdl[i].GetModuleState() != IDPOS_NONE )
			return false;
	}	

	return true;
}

int ATray::GetModuleCnt()
{
	int nTotalCnt = MAX_BUFFDVC;// = st_basic.n_tray_x * st_basic.n_tray_y;

	int nRtn = 0;
	for( int i=0; i<nTotalCnt; i++ )
	{
		if( GetModule(i).IsExist() )
			nRtn++;
	}

	return nRtn;
}

int ATray::GetModuleBcr1Cnt()
{
	int nTotalCnt = MAX_BUFFDVC;// = st_basic.n_tray_x * st_basic.n_tray_y;

	int nRtn = 0;
	for( int i=0; i<nTotalCnt; i++ )
	{
		if( GetModule(i).IsExist() && GetModule(i).IsTestDensityState() )
			nRtn++;
	}

	return nRtn;
}

int ATray::GetModuleBcr2Cnt()
{
	int nTotalCnt = MAX_BUFFDVC;// = st_basic.n_tray_x * st_basic.n_tray_y;

	int nRtn = 0;
	for( int i=0; i<nTotalCnt; i++ )
	{
		if( GetModule(i).IsExist() && GetModule(i).IsTestDensityState() )
			nRtn++;
	}

	return nRtn;
}

int ATray::GetModuleTopVisionCnt()
{
	int nTotalCnt = MAX_BUFFDVC;

	int nRtn = 0;

	for ( int i = 0; i < nTotalCnt; i++)
	{
		if( GetModule(i).IsExist() && GetModule(i).IsTestBarcodeState())
			nRtn++;
	}

	return nRtn;
}

int ATray::GetModuleUnloadCnt()
{
	int nTotalCnt = MAX_BUFFDVC;

	int nRtn = 0;
	for( int i=0; i<nTotalCnt; i++ )
	{
		if( GetModule(i).IsExist() )//자재가 있으면 BTM Vision 체크한다.
			nRtn++;
	}

	return nRtn;
}

bool ATray::IsAllState( EMODULE_STATE ems )
{
	int nTotalCnt = MAX_BUFFDVC;
	for( int i=0; i< nTotalCnt/*MAX_MODULE*/; i++ )
	{
		if( m_mdl[i].IsExist() == false )
			continue;

		if( m_mdl[i].GetModuleState() != ems )
			return false;
	}	

	return true;
}

void ATray::SetPos( EIDBUFFER_POS pos )
{
	 m_trayPos = pos;

//	 if( g_handler.GetMachinePos() == EMPOS_REAR )
//		 g_client_front.SendTrayInfo();
}

void ATray::SetTargetPos( EIDBUFFER_POS pos )
{
//	if( m_trayPos != m_trayTargetPos &&
//		(m_trayTargetPos == TPOS_REJECT ||
//		m_trayTargetPos == TPOS_NEXT ) )
//		return;
	
	m_trayTargetPos = pos;

//	if( g_handler.GetMachinePos() == EMPOS_REAR )
//		 g_client_front.SendTrayInfo();
}


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
APartTray g_Tray;
APartTray::APartTray()
{
	m_vecRej.clear();
	m_nRejSelected = -1;
	m_nSelectedTray = -1;

	m_bSendNext_fromRej_Force = false;
}

APartTray::~APartTray()
{

}

void APartTray::AddTray( ATray tray )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == TPOS_NONE )
		{
			m_tray[i] = tray;
			break;
		}
	}

//	if( g_handler.GetMachinePos() == EMPOS_REAR )
//		 g_client_front.SendTrayInfo();

}

void APartTray::AddSetTray( ATray tray, EIDBUFFER_POS pos)
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == pos )
		{
			m_tray[i].Clear();
			m_tray[i] = tray;
			break;
		}
	}
}


void APartTray::TrayMinus()
{
//	int i,j;
//	i=j=0;
//	for( j=MAX_TRAY-1; j>=0; j-- )
//	{
//		if( m_tray[j].GetPos() == TPOS_REJECT && m_tray[j].GetTargetPos() == TPOS_REJECT)
//		{
//			std::vector<int>::iterator it = g_Tray.m_vecRej.begin();
//			for( i=0; i<m_vecRej.size(); i++ )
//			{
//				if( m_vecRej.at(i) == j )
//				{
//					it += i;
//					break;
//				}
//			}
//			m_vecRej.erase(it);
//			m_tray[j].Clear();
//			break;
//		}
//	}
//	
//	g_Tray.m_nRejSelected = g_Tray.m_vecRej.size() - 1;
//	
//	if( st_handler.cwnd_main )
//	{
//		st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_REJ_FLOOR );
//	}
}

void APartTray::TrayMoved( EIDBUFFER_POS posFrom, EIDBUFFER_POS posTo )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == posFrom )
		{
			m_tray[i].SetPos( posTo );
			m_tray[i].SetTargetPos( posTo );
			CheckAfterTrayMoved( i, posFrom, posTo );

			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "TRAY[%d] MOVE FROM: [%s] -----> TO:[%s]", i, (LPCSTR)GetStrPos(posFrom), (LPCSTR)GetStrPos(posTo) );
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			if( posTo == IDPOS_NEXT )
			{
				m_tray[i].Clear();
	//			g_handler.CheckLotEnd();
				
//				g_client_next.SetBufferReady( false );
				sprintf(st_msg.c_normal_msg, "[SMEMA]BufferReady = %d", false);
				Func.On_LogFile_Add(99, st_msg.c_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}

		}
	}
}

CString APartTray::GetStrPos( EIDBUFFER_POS epos )
{

	switch( epos )
	{
	case IDPOS_NONE:						return "NONE";
	case IDPOS_FRONT:						return "FRONT";
	case IDPOS_DENSITY:						return "DENSITY";
	case IDPOS_BARCODE1:					return "BARCODE1";
	case IDPOS_BARCODE2:					return "BARCODE2";
	case IDPOS_TOP_VISION:					return "TOP_VISION";
	case IDPOS_UNLOAD:						return "UNLOAD";
	case IPOOS_UNLOAD_BTM:					return "UNLOAD_BTM";
	case IDPOS_CONVEYOR:					return "CONVEYOR";
	case IPOOS_UNLOAD_BTM_LOAD:				return "UNLOAD_BTM_LOAD";
	case IPOOS_UNLOAD_BTM_UNLOAD:			return "UNLOAD_BTM_UNLOAD";	
	case IDPOS_NEXT:						return "NEXT";		
	case IDPOS_FRONT_TO_DENSITY:			return "FRONT_TO_DENSITY";
	case IDPOS_DENSITY_TO_BARCODE1:			return "DENSITY_TO_BARCODE1";
	case IDPOS_BARCODE1_TO_BARCODE2:		return "BARCODE1_TO_BARCODE2";
	case IDPOS_BARCODE2_TO_VISION:			return "BARCODE2_TO_VISION";
	case IDPOS_TOPVISION_TO_UNLOAD:			return "TOPVISION_TO_UNLOAD";
	case IDPOS_UNLOAD_TO_CONVEYOR:			return "UNLOAD_TO_CONVEYOR";
	case IDPOS_CONVEYOR_TO_NEXT:			return "CONVEYOR_TO_NEXT";
	}

	return "";
}

int APartTray::GetRejCnt()
{
	int iCnt = 0;
//	for( int i=0; i<MAX_TRAY; i++ )
//	{
//		if( m_tray[i].GetPos() == TPOS_REJECT || m_tray[i].GetPos() == TPOS_REJECT_TO_BIN )
//		{
//			iCnt++;
//		}
//	}

	return iCnt;
}

bool APartTray::IsEnablePos( EIDBUFFER_POS epos)
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == epos || 
			m_tray[i].GetTargetPos() == epos )
		{
			return false;
		}
	}

	return true;
}

void APartTray::CheckAfterTrayMoved( int iIdx, EIDBUFFER_POS posFrom, EIDBUFFER_POS posTo )
{
	// Reject
	bool bChanged = false;
	if( posTo == TPOS_REJECT1 )
	{
		m_vecRej.push_back( iIdx );
		bChanged = true;
	}
	else if( posTo == TPOS_REJECT2 )
	{
		m_vecRej.push_back( iIdx );
		bChanged = true;
	}
//	else if( posFrom == TPOS_REJECT )
//	{
//		std::vector<int>::iterator it = m_vecRej.begin();
//		for( int i=0; i<m_vecRej.size(); i++ )
//		{
//			if( m_vecRej.at(i) == iIdx )
//			{
//				it += i;
//				break;
//			}
//		}
//
//		m_vecRej.erase(it);
//		bChanged = true;
//	}

//	if( bChanged )
//	{
//		m_nRejSelected = m_vecRej.size() - 1;
//
//		if( st_handler.cwnd_main )
//		{
//			st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_REJ_FLOOR );
//		}
//	}
//
//	if( iIdx == g_Tray.GetSelectedTray() )
//	{
//		if( st_handler.cwnd_main )
//		{
//			st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY );
//		}	
//	}
//
//	if( posFrom == TPOS_CONV || posTo == TPOS_LOAD )
//	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_0 );
//	}
//	if( posFrom == TPOS_LDELV || posTo == TPOS_ULDELV )
//	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_1 );
//	}
//	//ybs
//	if( posFrom == TPOS_UNLOAD || posTo == TPOS_AUNLOAD || posTo == TPOS_BUNLOAD )
//	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_2 );
//	}
//	if( posFrom == TPOS_RETEST || posTo == TPOS_REJECT1 || posTo == TPOS_REJECT2 )
//	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_2 );
//	}
}

int APartTray::GetRejSelectedTray()
{
	if( m_nRejSelected < 0 || m_nRejSelected >= m_vecRej.size() )
		return -1;

	return m_vecRej[m_nRejSelected];
}

void APartTray::SetRejSelected( int iSel )
{
	if( iSel < 0 || iSel >= m_vecRej.size() )
		return;

	m_nRejSelected = iSel;
}

int APartTray::GetIdxByPos( EIDBUFFER_POS epos )
{
	for( int i=0; i<MAX_IDPOS; i++ )
	{
		if( m_tray[i].GetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

std::vector<int> APartTray::GetSorterVecMustTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
//	vecRtn.clear();
//
//	int iIdx = g_Tray.GetIdxByPos( TPOS_RETEST );
//	if( iIdx < 0 )											return vecRtn;
//	int iIndexBin = MPOS_INDEX_RETEST;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );
////	int iMdlCnt = st_basic.n_Rework_Tray_x * st_basic.n_Rework_Tray_y;	
//	int x;// = st_basic.n_Rework_Tray_x;
//	int y;// = st_basic.n_Rework_Tray_y;
//	
//	std::multimap<int, int> mapCntByX;
//// 	for( int ix=0; ix<x; ix++ )
//	for( int iy=0; iy<y; iy++ )
//	{
//		int nCnt = 0;
//// 		for( int iy=0; iy<y; iy++ )
//		for( int ix=0; ix<x; ix++ )
//		{
//			int nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//			if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//				nCnt++;
//		}
//// 		mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//		mapCntByX.insert( std::pair<int,int>( -nCnt, iy ) );
//	}
//	
//	std::multimap<int,int>::iterator it = mapCntByX.begin();
//	
//	for( it; it != mapCntByX.end(); it++ )
//	{
////		for( int iy=0; iy<y; iy++ )
//		for( int ix=0; ix<x; ix++ )
//		{
//// 			int nTarget = GetMdlIdx( it->second, iy, x, y, false ) - 1;
//			int nTarget = GetMdlIdx( ix, it->second, x, y, false ) - 1;
//			if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//				vecRtn.push_back( iIndexBin + nTarget );
//		}
//	}
//
//	//2013,1227
//	bool bLotMgr = false;
//	if( g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" && vecRtn.size() < 4)
//	{
//		int iTray1 = g_Tray.GetIdxByPos( TPOS_ULDELV );
//		int iTray2 = g_Tray.GetIdxByPos( TPOS_UNLOAD );
//		for ( int i = 0; i < MAX_BUFFDVC; i++ )
//		{
//			if( g_Tray.GetTray( iTray1 ).GetModule(i).IsExist() == true)
//			{
//				if(	g_Tray.GetTray( iTray1 ).GetModule(i).GetLotNo() == g_lotMgr.GetLotAt(0).GetLotID() )
//				{
//					bLotMgr = true;
//					break;
//				}
//			}
//			if( g_Tray.GetTray( iTray2 ).GetModule(i).IsExist() == true)
//			{
//				if(	g_Tray.GetTray( iTray2 ).GetModule(i).GetLotNo() == g_lotMgr.GetLotAt(0).GetLotID() )
//				{
//					bLotMgr = true;
//					break;
//				}
//			}
//// 			if( g_sorter_robot.GetPicker(i).GetModule().IsExist() == true )
//// 			{
//// 				if( g_sorter_robot.GetPicker(i).GetModule().GetLotNo() == g_lotMgr.GetLotAt(0).GetLotID() )
//// 				{
//// 					bLotMgr = false;
//// 					return;
//// 				}
//// 			}
//		}
//		int iMdlCnt = g_site.GetModuleCnt();
//		if( iMdlCnt > 0)
//		{
//			for( int i=0; i<TSITE_SOCKET_CNT; i++ )
//			{
//				if(g_site.GetModule(i).IsExist())
//				{
//					if(g_site.GetModule(i).GetLotNo() == g_lotMgr.GetLotIDAt(0))
//					{
//						bLotMgr = true;
//						break;
//					}
//				}
//			}
//		}
//
//	}
//
//	if( vecRtn.size() >= 4 || ( g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" && vecRtn.size() > 0 ) )// && bLotMgr == false) )
//	{
//		return vecRtn;
//	}
//	else
//	{
//		vecRtn.clear();
//	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecMustTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();
//
//	int iIdx = g_Tray.GetIdxByPos( TPOS_LDELV );
//	if( iIdx < 0 )											return vecRtn;
//	int iIndexBin = MPOS_INDEX_ELV_LOAD;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );
//
//	//2013.1223
//	if(g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID())
//	{
//	}
//
//	int iMdlCnt = MAX_BUFFDVC;	
//
//	std::multimap<int, int> mapCntByX;
//	int nCnt = 0;
//	for( int ix=0; ix<iMdlCnt; ix++ )
//	{
//		int nTarget = GetMdlIdx( ix, 0, iMdlCnt, 1, false ) - 1;
//		if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//			nCnt++;
//		mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//	}
//
//	std::multimap<int,int>::iterator it = mapCntByX.begin();
//
//	for( it; it != mapCntByX.end(); it++ )
//	{
//		for( int iy=0; iy<1; iy++ )
//		{
//			int nTarget = GetMdlIdx( it->second, iy, iMdlCnt, 1, false ) - 1;
//			if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//	 			vecRtn.push_back( iIndexBin + nTarget );
//
//		}
//	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecStacker3MustTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

//
//	if( g_handler.GetPriorityBin() == TPOS_NONE )
//		return vecRtn;
//
//	int iBin1 = g_Tray.GetIdxByPos( TPOS_BIN1 );
//	int iBin2 = g_Tray.GetIdxByPos( TPOS_BIN2 );
//	int iBin3 = g_Tray.GetIdxByPos( TPOS_BIN3 );
//
//	if(st_handler.mn_mirror_type == CTL_YES)
//	{
//		if(iBin1 == -1) return vecRtn;
//	}
//	else
//	{
//		if(iBin3 == -1) return vecRtn;
//	}
//
//	//트레이가 비어 있을 수도 있으나, site가 비어있을 수 있더
//	//트레이 3군데를 전부 봐야 한다.
//	//Stacker3번째를 다른 곳(Site 또는 Tray)으로 옮긴뒤에 처리한다,
//	int ix=0, iy=0;
//	ETRAY_POS ePosPri;
//	ETRAY_POS ePosSub1,ePosSub2;
//	if(st_handler.mn_mirror_type == CTL_YES)
//	{
//		ePosPri = TPOS_BIN1;
//		if(ePosPri == TPOS_BIN1)
//		{
//			if(iBin2 > -1)
//			{
//				ePosSub1 = TPOS_BIN2;
//				if(iBin3 > -1) ePosSub2 = TPOS_BIN3;
//				else           ePosSub2 = TPOS_NONE;
//			}
//			else
//			{
//				if(iBin3 > -1) ePosSub1 = TPOS_BIN3;
//				else
//				{
//					ePosSub1 = TPOS_NONE;
//					ePosSub2 = TPOS_NONE;
//				}
//			}
//		}
//	}
//	else
//	{
//		ePosPri = TPOS_BIN3;
//		if(ePosPri == TPOS_BIN3)
//		{
//			if(iBin1 > -1)
//			{
//				ePosSub1 = TPOS_BIN1;
//				if(iBin2 > -1) ePosSub2 = TPOS_BIN2;
//				else           ePosSub2 = TPOS_NONE;
//			}
//			else
//			{
//				if(iBin2 > -1) ePosSub1 = TPOS_BIN2;
//				else
//				{
//					ePosSub1 = TPOS_NONE;
//					ePosSub2 = TPOS_NONE;
//				}
//			}
//		}
//	}
//
//	int iIndexBinPri = MPOS_INDEX_BIN3;
//	int iIndexBinSub1 = MPOS_INDEX_BIN3;
//	int iIndexBinSub2 = MPOS_INDEX_BIN3;
//
//	if( ePosPri == TPOS_BIN1 )	    iIndexBinPri = MPOS_INDEX_BIN1;
//	if( ePosPri == TPOS_BIN2 )		iIndexBinPri = MPOS_INDEX_BIN2;
//	if( ePosSub1 == TPOS_BIN1 )		iIndexBinSub1 = MPOS_INDEX_BIN1;
//	if( ePosSub1 == TPOS_BIN2 )	    iIndexBinSub1 = MPOS_INDEX_BIN2;
//	if( ePosSub2 == TPOS_BIN1 )		iIndexBinSub2 = MPOS_INDEX_BIN1;
//	if( ePosSub2 == TPOS_BIN2 )	    iIndexBinSub2 = MPOS_INDEX_BIN2;
//
//
//	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//	int iIdx = g_Tray.GetIdxByPos( ePosPri );
//	ATray trayPri = g_Tray.GetTray( iIdx );
//
//	int x = st_basic.n_tray_x;
//	int y = st_basic.n_tray_y;
//	if( st_basic.n_degree == EDEG_90 || st_basic.n_degree == EDEG_270 )
//	{
//		x = st_basic.n_tray_y;
//		y = st_basic.n_tray_x;
//	}
//
//	if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt( nLotIdx ))//아니면 문제가 된다. 미리 다 막았지만
//	{
//		std::multimap<int, int> mapCntByX;
//		int nCnt = 0;
//		//2013,0306
//		if(st_handler.mn_mirror_type == CTL_YES)
//		{
//			for( ix=x-1; ix>=0; ix-- )
//			{
//				nCnt = 0;
//				for( iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//						nCnt++;
//				}
//				mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//			}
//		}
//		else
//		{
//			for( ix=0; ix<x; ix++ )
//			{
//				nCnt = 0;
//				for( iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//						nCnt++;
//				}
//				mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//			}
//		}
//
//		std::multimap<int,int>::iterator it = mapCntByX.begin();
//
//		for( it; it != mapCntByX.end(); it++ )
//		{
//			for( iy=0; iy<y; iy++ )
//			{
//				int nTarget = GetMdlIdx( it->second, iy, x, y, true ) - 1;
//				if( trayPri.GetModule(nTarget).IsExist() && trayPri.GetModule(nTarget).IsTestLoadState() )
//	 				vecRtn.push_back( iIndexBinPri + nTarget );
//
//			}
//		}
//	}	
	
	return vecRtn;
}

std::vector<int> APartTray::GetSorterVecEmptyCell( int nLotIdx , EIDBUFFER_POS pos)
{
	std::vector<int> vecRtn;
//
//	int iMdlCnt = MAX_BUFFDVC;
//	int x=0, y=0, nTarget=0, nCnt=0;
//	
//	int iIndexBin = MPOS_INDEX_CONV;
//	int iIdx = g_Tray.GetIdxByPos( TPOS_CONV);
//
//	if( pos == TPOS_LOAD )
//	{
//		iIndexBin = MPOS_INDEX_LOAD;
//		iIdx = g_Tray.GetIdxByPos( TPOS_LOAD);
//		if( iIdx < 0 )
//			return vecRtn;
//		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//
//
//		//2013,1223
//		if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//		{
//		}
//
//		std::multimap<int, int> mapCntByX;
//		for( int ix=0; ix<iMdlCnt; ix++ )
//		{
//			nCnt = 0;
//			{
//				int nTarget = GetMdlIdx( ix, 0, iMdlCnt, 1, false ) - 1;
//				//2014,0108
//				if( trayPri.GetModule(nTarget).IsExist() == false && trayPri.GetModule(nTarget).GetModuleState() == EMS_NONE)
//					nCnt++;
//			}
//			mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//		}
//		std::multimap<int, int>::iterator it = mapCntByX.begin();
//		for( it; it != mapCntByX.end(); it++ )
//		{
//			for( int iy=0; iy<1; iy++ )
//			{
//				nTarget = GetMdlIdx( it->second, iy, iMdlCnt, 1, false ) - 1;
//				if( trayPri.GetModule(nTarget).IsExist() == false )
//				{
//					vecRtn.push_back( iIndexBin + nTarget );
//				}
//			}
//		}
//		//2014,0109
//		if(vecRtn.size() < 4) vecRtn.clear();
//		return vecRtn;
//	}
//	else if( pos == TPOS_AUNLOAD )
//	{
//		iIndexBin = MPOS_INDEX_UNLOAD_ACONV;
//		iIdx = g_Tray.GetIdxByPos( TPOS_AUNLOAD);
//		
//		if( iIdx < 0 )
//			return vecRtn;
//		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//		//2013,1223
//		if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//		{
//		}
//// 		int x = st_basic.n_tray_x;
//// 		int y = st_basic.n_tray_y;
//
//// 		if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
//// 		{
//		if ( st_handler.mn_smema_reara == CTL_REQ || st_basic.mn_mode_run == CTL_NO || 
//			st_basic.n_mode_device == CTL_NO)
//		{
//			std::multimap<int, int> mapCntByX;
//			for( int ix=0; ix<iMdlCnt; ix++ )
//			{
//				nCnt = 0;
//				{
//					int nTarget = GetMdlIdx( ix, 0, iMdlCnt, 1, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//						nCnt++;
//				}
//				mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//			}
//			std::multimap<int, int>::iterator it = mapCntByX.begin();
//			for( it; it != mapCntByX.end(); it++ )
//			{
//				for( int iy=0; iy<1; iy++ )
//				{
//					nTarget = GetMdlIdx( it->second, iy, iMdlCnt, 1, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//					{
//						vecRtn.push_back( iIndexBin + nTarget );
//					}
//				}
//			}
// 		}
//		return vecRtn;
//	}
//	else if( pos == TPOS_BUNLOAD )
//	{
//		iIndexBin = MPOS_INDEX_UNLOAD_BCONV;
//		iIdx = g_Tray.GetIdxByPos( TPOS_BUNLOAD);
//
//		if( iIdx < 0 )
//			return vecRtn;
//		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//		//2013,1223
//		if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//		{
//		}
//		int x;// = st_basic.n_tray_x;
//		int y;// = st_basic.n_tray_y;
//
////		if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
//		if (st_handler.mn_smema_rearb == CTL_REQ || st_basic.mn_mode_run == CTL_NO || 
//			st_basic.n_mode_device == CTL_NO)
//		{
//			std::multimap<int, int> mapCntByX;
//			for( int ix=0; ix<iMdlCnt; ix++ )
//			{
//				nCnt = 0;
//				for( int iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( ix, iy, x, y, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//						nCnt++;
//				}
//				mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//			}
//			std::multimap<int, int>::iterator it = mapCntByX.begin();
//			for( it; it != mapCntByX.end(); it++ )
//			{
//				for( int iy=0; iy<y; iy++ )
//				{
//					nTarget = GetMdlIdx( it->second, iy, x, y, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//					{
//						vecRtn.push_back( iIndexBin + nTarget );
//					}
//				}
//			}
//		}
//		return vecRtn;
//	}
//	else if( pos == TPOS_REJECT1 || pos == TPOS_REJECT2 )
//	{
//		iIndexBin = MPOS_INDEX_REJECT1;
//		iIdx = g_Tray.GetIdxByPos( TPOS_REJECT1);
//		if( iIdx < 0 )
//			return vecRtn;
//		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//		//2013,1223
//		if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//		{
//		}
//		
//		int x;// = st_basic.n_tray_x;
//		int y;// = st_basic.n_tray_y;
//
//// 		if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
//// 		{
//			std::multimap<int, int> mapCntByX;
//			for( int ix=0; ix<x; ix++ )
//			{
//				nCnt = 0;
//				for( int iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( ix, iy, x, y, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//						nCnt++;
//				}
//				mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//			}
//			std::multimap<int, int>::iterator it = mapCntByX.begin();
//			for( it; it != mapCntByX.end(); it++ )
//			{
//				for( int iy=0; iy<y; iy++ )
//				{
//					nTarget = GetMdlIdx( it->second, iy, x, y, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//					{
//						vecRtn.push_back( iIndexBin + nTarget );
//					}
//				}
//			}
//// 		}
//// 		return vecRtn;
//// 	}
//// 	else if( pos == TPOS_REJECT2 )
//// 	{
//		iIndexBin = MPOS_INDEX_REJECT2;
//		iIdx = g_Tray.GetIdxByPos( TPOS_REJECT2);
//		if( iIdx < 0 )
//			return vecRtn;
//		
//// 		ATray trayPri = g_Tray.GetTray( iIdx );
//		trayPri = g_Tray.GetTray( iIdx );
//		//2013,1223
//		if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//		{
//		}
////		x = st_basic.n_tray_x;
////		y = st_basic.n_tray_y;
//
//// 		if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
//// 		{
//			std::multimap<int, int> mapCntByX1;
//			for( ix=0; ix<x; ix++ )
//			{
//				nCnt = 0;
//				for( int iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( ix, iy, x, y, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//						nCnt++;
//				}
//				mapCntByX1.insert( std::pair<int,int>( -nCnt, ix ) );
//			}
//			/*std::multimap<int, int>::iterator */it = mapCntByX1.begin();
//			for( it; it != mapCntByX1.end(); it++ )
//			{
//				for( int iy=0; iy<y; iy++ )
//				{
//					nTarget = GetMdlIdx( it->second, iy, x, y, false ) - 1;
//					if( trayPri.GetModule(nTarget).IsExist() == false )
//					{
//						vecRtn.push_back( iIndexBin + nTarget );
//					}
//				}
//			}
//// 		}
//
//		return vecRtn;
//	}
//	else if( pos == TPOS_RETEST )
//	{
//		iIndexBin = MPOS_INDEX_RETEST;
//		iIdx = g_Tray.GetIdxByPos( TPOS_RETEST);
//
//		if( iIdx < 0 )
//			return vecRtn;
//		
//		ATray trayPri = g_Tray.GetTray( iIdx );
//		//2013,1223
//		if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//		{
//		}
//		int x;// = st_basic.n_Rework_Tray_x;
//		int y;// = st_basic.n_Rework_Tray_y;
//
//// 		if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
//// 		{
//		std::multimap<int, int> mapCntByX;
//// 		for( int ix=0; ix<x; ix++ )
//		for( int iy=0; iy<y; iy++ )
//		{
//			nCnt = 0;
//// 			for( int iy=0; iy<y; iy++ )
//			for( int ix=0; ix<x; ix++ )
//			{
//				int nTarget = GetMdlIdx( ix, iy, x, y, false ) - 1;
//				if( trayPri.GetModule(nTarget).IsExist() == false )
//					nCnt++;
//			}
//// 			mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//			mapCntByX.insert( std::pair<int,int>( nCnt, iy ) );
//		}
//		std::multimap<int, int>::iterator it = mapCntByX.begin();
//		for( it; it != mapCntByX.end(); it++ )
//		{
// 			//for( int iy=0; iy<y; iy++ )
//			for( int ix=0; ix<x; ix++ )
//			{
// 				//nTarget = GetMdlIdx( it->second, iy, x, y, false ) - 1;
//				nTarget = GetMdlIdx( ix, it->second, x, y, false ) - 1;
//				if( trayPri.GetModule(nTarget).IsExist() == false )
//				{
//					vecRtn.push_back( iIndexBin + nTarget );
//				}
//			}
//		}
//// 		}
//		return vecRtn;//2013,1223
//
//	}
//	else
//	{
//		return vecRtn;
//	}
//
////	if( iIdx < 0 )
////		return vecRtn;
////	
////	ATray trayPri = g_Tray.GetTray( iIdx );
////
////	if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
////	{
////		std::multimap<int, int> mapCntByX;
////		for( int ix=0; ix<iMdlCnt; ix++ )
////		{
////			nCnt = 0;
////			{
////				int nTarget = GetMdlIdx( ix, 0, iMdlCnt, 1, false ) - 1;
////				if( trayPri.GetModule(nTarget).IsExist() == false )
////					nCnt++;
////			}
////			mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
////		}
////		std::multimap<int, int>::iterator it = mapCntByX.begin();
////		for( it; it != mapCntByX.end(); it++ )
////		{
////			for( int iy=0; iy<1; iy++ )
////			{
////				nTarget = GetMdlIdx( it->second, iy, iMdlCnt, 1, false ) - 1;
////				if( trayPri.GetModule(nTarget).IsExist() == false )
////				{
////					vecRtn.push_back( iIndexBin + nTarget );
////				}
////			}
////		}
////	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecEmptyCell( int nLotIdx, int nExchange1, int nExchange2 )
{
	std::vector<int> vecRtn;

//	int iMdlCnt = MAX_BUFFDVC;
//	int x=0, y=0, nTarget=0, nCnt=0;
//	
//	int iIndexBin = MPOS_INDEX_ELV_UNLOAD;
//	int iIdx = g_Tray.GetIdxByPos( TPOS_ULDELV);
//
//	if( iIdx < 0 )
//		return vecRtn;
//	
//	ATray trayPri = g_Tray.GetTray( iIdx );
//
//// 	if( trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx))
//// 	{
//		std::multimap<int, int> mapCntByX;
//		for( int ix=0; ix<iMdlCnt; ix++ )
//		{
//			nCnt = 0;
//			{
//				int nTarget = GetMdlIdx( ix, 0, iMdlCnt, 1, false ) - 1;
//				if( trayPri.GetModule(nTarget).IsExist() == false )
//					nCnt++;
//			}
//			mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//		}
//		std::multimap<int, int>::iterator it = mapCntByX.begin();
//		for( it; it != mapCntByX.end(); it++ )
//		{
//			for( int iy=0; iy<1; iy++ )
//			{
//				nTarget = GetMdlIdx( it->second, iy, iMdlCnt, 1, false ) - 1;
//				if( trayPri.GetModule(nTarget).IsExist() == false )
//				{
//					vecRtn.push_back( iIndexBin + nTarget );
//				}
//			}
//		}
//// 	}
//
////		if(nPos == BPOS_LOAD || nPos == BPOS_UNLOAD)
////		{
////			for( ix=0; ix<x; ix++ )
////			{
////				nCnt = 0;
////				{
////					nTarget = GetMdlIdx( ix, 0, x, 1, true ) - 1;
////					if( trayPri.GetModule(nTarget).IsExist() == false )
////						nCnt++;
////				}
////				mapCntByX.insert( std::pair<int,int> (-nCnt, ix ) );
////			}
////
////			std::multimap<int, int>::iterator it = mapCntByX.begin();
////			for( it; it != mapCntByX.end(); it++ )
////			{
////				for( int iy=0; iy<1; iy++ )
////				{
////					nTarget = GetMdlIdx( it->second, iy, iMdlCnt, 1, true ) - 1;
////					if( trayPri.GetModule(nTarget).IsExist() == false )
////					{
////						if(nPos == BPOS_UNLOAD)
////						{
////							if(st_handler.mn_curr_unload_conv == -1)
////							{
////								iIndexBin = MPOS_INDEX_UNLOAD_ACONV;
////								st_handler.mn_curr_unload_conv = iIndexBin;
////							}
////							else
////							{
////								iIndexBin = st_handler.mn_curr_unload_conv;
////							}
////						}
////						vecRtn.push_back( iIndexBin + nTarget );
////					}
////				}
////			}
////		}
////		else if(nPos == BPOS_RETEST || nPos == BPOS_REJECT1)
////		{
////			if(nPos == BPOS_RETEST)
////			{
////				x = 4;
////				y = 4;
////			}
////			else
////			{
////				x = st_basic.n_tray_x;
////				y = st_basic.n_tray_y;
////			}
////
////			for( ix=0; ix<x; ix++ )
////			{
////				nCnt = 0;
////				for( iy=0; iy<y; iy++ )
////				{
////					nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
////					if( trayPri.GetModule(nTarget).IsExist() == false)
////						nCnt++;
////				}
////				mapCntByX.insert( std::pair<int,int> (-nCnt, ix ) );
////			}
////
////			std::multimap<int, int>::iterator it = mapCntByX.begin();
////			for( it; it != mapCntByX.end(); it++ )
////			{
////				for( int iy=0; iy<y; iy++ )
////				{
////					nTarget = GetMdlIdx( it->second, iy, x, y, true ) - 1;
////					if( trayPri.GetModule(nTarget).IsExist() == false)
////						vecRtn.push_back( iIndexBinPri + nTarget );
////				}
////			}
////
////			if( nPos == BPOS_REJECT1 )
////			{
////				iIdx = g_Tray.GetIdxByPos( BPOS_REJECT2 );
////				if( iIdx > 0 )
////				{
////					ATray trayPri2 = g_Tray.GetTray( iIdx );
////					for( ix=0; ix<x; ix++ )
////					{
////						nCnt = 0;
////						for( iy=0; iy<y; iy++ )
////						{
////							nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
////							if( trayPri2.GetModule(nTarget).IsExist() == false)
////								nCnt++;
////						}
////						mapCntByX.insert( std::pair<int,int> (-nCnt, ix ) );
////					}
////
////					std::multimap<int, int>::iterator it = mapCntByX.begin();
////					for( it; it != mapCntByX.end(); it++ )
////					{
////						for( int iy=0; iy<y; iy++ )
////						{
////							nTarget = GetMdlIdx( it->second, iy, x, y, true ) - 1;
////							if( trayPri2.GetModule(nTarget).IsExist() == false)
////								vecRtn.push_back( iIndexBinPri + nTarget );
////						}
////					}
////				}
////			}
////		}
////	}


	return vecRtn;
}

std::vector<int> APartTray::GetVecStacker3EmptyCell( int nLotIdx, int nExchange1, int nExchange2 )
{
	std::vector<int> vecRtn;

//	if( g_handler.GetPriorityBin() == TPOS_NONE )
//		return vecRtn;
//
//	int iBin1 = g_Tray.GetIdxByPos( TPOS_BIN1 );
//	int iBin2 = g_Tray.GetIdxByPos( TPOS_BIN2 );
//	int iBin3 = g_Tray.GetIdxByPos( TPOS_BIN3 );
//	int ix=0, iy=0;
//	ETRAY_POS ePosPri;
//	ETRAY_POS ePosSub1,ePosSub2;
//	if(st_handler.mn_mirror_type == CTL_YES)
//	{
//		ePosPri = TPOS_BIN1;
//		if(ePosPri == TPOS_BIN1)
//		{
//			if(iBin2 > -1)
//			{
//				ePosSub1 = TPOS_BIN2;
//				if(iBin3 > -1) ePosSub2 = TPOS_BIN3;
//				else           ePosSub2 = TPOS_NONE;
//			}
//			else
//			{
//				if(iBin3 > -1) ePosSub1 = TPOS_BIN3;
//				else
//				{
//					ePosSub1 = TPOS_NONE;
//					ePosSub2 = TPOS_NONE;
//				}
//			}
//		}
//	}
//	else
//	{
//		ePosPri = TPOS_BIN3;
//		if(ePosPri == TPOS_BIN3)
//		{
//			if(iBin1 > -1)
//			{
//				ePosSub1 = TPOS_BIN1;
//				if(iBin2 > -1) ePosSub2 = TPOS_BIN2;
//				else           ePosSub2 = TPOS_NONE;
//			}
//			else
//			{
//				if(iBin2 > -1) ePosSub1 = TPOS_BIN2;
//				else
//				{
//					ePosSub1 = TPOS_NONE;
//					ePosSub2 = TPOS_NONE;
//				}
//			}
//		}
//	}
//
//	int iIndexBinPri = MPOS_INDEX_BIN3;
//	int iIndexBinSub1 = MPOS_INDEX_BIN3;
//	int iIndexBinSub2 = MPOS_INDEX_BIN3;
//	
//	if( ePosPri == TPOS_BIN1 )	    iIndexBinPri = MPOS_INDEX_BIN1;
//	if( ePosPri == TPOS_BIN2 )		iIndexBinPri = MPOS_INDEX_BIN2;
//	if( ePosSub1 == TPOS_BIN1 )		iIndexBinSub1 = MPOS_INDEX_BIN1;
//	if( ePosSub1 == TPOS_BIN2 )	    iIndexBinSub1 = MPOS_INDEX_BIN2;
//	if( ePosSub2 == TPOS_BIN1 )		iIndexBinSub2 = MPOS_INDEX_BIN1;
//	if( ePosSub2 == TPOS_BIN2 )	    iIndexBinSub2 = MPOS_INDEX_BIN2;
//
//	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//	int iIdx = g_Tray.GetIdxByPos( ePosPri );
//	if( iIdx < 0 )
//		return vecRtn;
//
//	int x = st_basic.n_tray_x;
//	int y = st_basic.n_tray_y;
//	if( st_basic.n_degree == EDEG_90 || st_basic.n_degree == EDEG_270 )
//	{
//		x = st_basic.n_tray_y;
//		y = st_basic.n_tray_x;
//	}
//
//
//	iIdx = g_Tray.GetIdxByPos( ePosSub1 );
//	if( iIdx > -1 )
//	{
//		ATray traySub = g_Tray.GetTray( iIdx );
//
//		if( traySub.GetTargetPos() == ePosSub1 &&
//			traySub.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx) )
//		{
//			std::multimap<int, int> mapCntByX;
//			int nCnt=0,nTarget=0;
//			//2013,0306
//			if(st_handler.mn_mirror_type == CTL_YES)
//			{
//				for( ix=x-1; ix>=0; ix-- )
//				{
//					nCnt = 0;
//					for( iy=0; iy<y; iy++ )
//					{
//						nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//						if( traySub.GetModule(nTarget).IsExist() == false ||
//							iIndexBinSub1 + nTarget == nExchange1 ||
//							iIndexBinSub1 + nTarget == nExchange2 )	
//							nCnt++;
//					}
//					mapCntByX.insert( std::pair<int, int>(-nCnt, ix ) );
//				}
//			}
//			else
//			{
//				for( ix=0; ix<x; ix++ )
//				{
//					nCnt = 0;
//					for( iy=0; iy<y; iy++ )
//					{
//						nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//						if( traySub.GetModule(nTarget).IsExist() == false ||
//							iIndexBinSub1 + nTarget == nExchange1 ||
//							iIndexBinSub1 + nTarget == nExchange2 )	
//							nCnt++;
//					}
//					mapCntByX.insert( std::pair<int, int>(-nCnt, ix ) );
//				}
//			}
//			
//			std::multimap<int, int>::iterator it = mapCntByX.begin();
//			for( it; it != mapCntByX.end(); it++ )
//			{
//				for( int iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( it->second, iy, x, y, true ) - 1;
//					if( traySub.GetModule(nTarget).IsExist() == false ||
//					iIndexBinSub1 + nTarget == nExchange1 ||
//					iIndexBinSub1 + nTarget == nExchange2 )
//						vecRtn.push_back( iIndexBinSub1 + nTarget );
//				}
//			}
//		}
//		
//	}
//	iIdx = g_Tray.GetIdxByPos( ePosSub2 );
//	if( iIdx > -1 )
//	{
//		ATray traySub = g_Tray.GetTray( iIdx );
//		
//		if( traySub.GetTargetPos() == ePosSub2 &&
//			traySub.GetStrLotID() == g_lotMgr.GetLotIDAt(nLotIdx) )
//		{
//			std::multimap<int, int> mapCntByX;
//			int nCnt=0,nTarget=0;
//			//2013,0306
//			if(st_handler.mn_mirror_type == CTL_YES)
//			{
//				for( ix=x-1; ix>=0; ix-- )
//				{
//					nCnt = 0;
//					for( iy=0; iy<y; iy++ )
//					{
//						nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//						if( traySub.GetModule(nTarget).IsExist() == false ||
//							iIndexBinSub2 + nTarget == nExchange1 ||
//							iIndexBinSub2 + nTarget == nExchange2 )	
//							nCnt++;
//					}
//					mapCntByX.insert( std::pair<int, int>(-nCnt, ix ) );
//				}
//			}
//			else
//			{
//				for( ix=0; ix<x; ix++ )
//				{
//					nCnt = 0;
//					for( iy=0; iy<y; iy++ )
//					{
//						nTarget = GetMdlIdx( ix, iy, x, y, true ) - 1;
//						if( traySub.GetModule(nTarget).IsExist() == false ||
//							iIndexBinSub2 + nTarget == nExchange1 ||
//							iIndexBinSub2 + nTarget == nExchange2 )	
//							nCnt++;
//					}
//					mapCntByX.insert( std::pair<int, int>(-nCnt, ix ) );
//				}
//			}
//			
//			std::multimap<int, int>::iterator it = mapCntByX.begin();
//			for( it; it != mapCntByX.end(); it++ )
//			{
//				for( int iy=0; iy<y; iy++ )
//				{
//					int nTarget = GetMdlIdx( it->second, iy, x, y, true ) - 1;
//					if( traySub.GetModule(nTarget).IsExist() == false ||
//						iIndexBinSub2 + nTarget == nExchange1 ||
//						iIndexBinSub2 + nTarget == nExchange2 )
//						vecRtn.push_back( iIndexBinSub2 + nTarget );
//				}
//			}
//		}
//		
//	}
	return vecRtn;
}

std::vector<int> APartTray::GetVecRejEmptyCell()
{
	std::vector<int> vecRtn;
	vecRtn.clear();
	int iIndexBin = MPOS_INDEX_REJECT;

	if( st_uld_rbt.n_exist[0] || st_uld_rbt.n_exist[1] )
	{
		if( st_uld_rbt.st_btm[0].nState == EMBS_FAIL || st_uld_rbt.st_btm[1].nState == EMBS_FAIL )
// 		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_RetVision[CTL_LEFT] & 0x1111 != 0x1011 &&
// 			st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_RetVision[CTL_RIGHT] & 0x1111 != 0x0111 )
		{
			std::multimap<int, int> mapCntByX;
			int nCnt = 0;
			for( int ix=0; ix<MAX_BUFFDVC; ix++ )
			{
				if( st_uld_rbt.n_exist[ix] == 1 && st_uld_rbt.st_btm[ix].nState == EMBS_FAIL)
				{
					nCnt++;
					mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
				}
			}
			std::multimap<int,int>::iterator it = mapCntByX.begin();


			for( it; it != mapCntByX.end(); it++ )
			{
				if(nCnt > 0)
 					vecRtn.push_back( iIndexBin + it->second );
			}

		}
	}

	return vecRtn;
}

int APartTray::GetIdxByTargetPos( EIDBUFFER_POS epos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetTargetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

std::vector<int> APartTray::GetVecFailinTray(int nLotIdx)
{
	std::vector<int> vecRtn;

//	int iIndexBin = MPOS_INDEX_RETEST;
//	int iIdx = g_Tray.GetIdxByPos( TPOS_RETEST);
//	
//	if( iIdx < 0 )
//		return vecRtn;
//	
//	ATray trayPri = g_Tray.GetTray( iIdx );
//	//2013,1223
//	if( g_lotMgr.GetLotIDAt(nLotIdx) == trayPri.GetStrLotID() )
//	{
//	}
//	int x;// = st_basic.n_Rework_Tray_x;
//	int y;// = st_basic.n_Rework_Tray_y;
//	
//	int nCnt = 0, nTarget =0;
//	std::multimap<int, int> mapCntByX;
//	for( int ix=0; ix<x; ix++ )
//	{
//		nCnt = 0;
//		for( int iy=0; iy<y; iy++ )
//		{
//			int nTarget = GetMdlIdx( ix, iy, x, y, false ) - 1;
//			if( trayPri.GetModule(nTarget).IsExist() == true &&
//				trayPri.GetModule(nTarget).GetModuleState() == EMS_FAIL)
//				nCnt++;
//		}
//		mapCntByX.insert( std::pair<int,int>( -nCnt, ix ) );
//	}
//	std::multimap<int, int>::iterator it = mapCntByX.begin();
//	for( it; it != mapCntByX.end(); it++ )
//	{
//		for( int iy=0; iy<y; iy++ )
//		{
//			nTarget = GetMdlIdx( it->second, iy, x, y, false ) - 1;
//			if( trayPri.GetModule(nTarget).IsExist() == true &&
//				trayPri.GetModule(nTarget).GetModuleState() == EMS_FAIL)
//			{
//				vecRtn.push_back( iIndexBin + nTarget );
//			}
//		}
//	}
	return vecRtn;
}


int APartTray::GetLoadModuleCnt()
{
	int nRtn = 0;
	int iMdlCnt = MAX_BUFFDVC;

	for( int it=0; it<MAX_TRAY; it++ )
	{
		ATray tray = g_Tray.GetTray( it );
		for( int i=0; i<iMdlCnt; i++ )
		{
			if( tray.GetModule(i).IsExist() && tray.GetModule(i).IsTestLoadState() &&
				tray.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
				nRtn++;
		}
	}
	
	return nRtn;
}

bool APartTray::CalcLastTray( int iIdx, CString strLotID )
{
// 	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//	int i=0,it=0;
//	int iMdlCnt = g_site.GetModuleCnt();
//	int iPickCnt = g_robot.GetModuleCnt();
//	for( it=0; it<MAX_TRAY; it++ )
//	{
//		if( it == iIdx )						continue;
//		ATray tray = g_Tray.GetTray( it );
//		if( tray.GetPos() == TPOS_NONE )		continue;
//		if( tray.GetStrLotID() != strLotID )	continue;
//		if( tray.GetEmptyTray() )				continue;
//		return false;
//	}
//	if( iMdlCnt > 0)						return false;
//	if( iMdlCnt > 0)
//	{
//		for( i=0; i<TSITE_SOCKET_CNT; i++ )
//		{
//			if(g_site.GetModule(i).IsExist())
//			{
//				if(g_site.GetModule(i).GetLotNo() == strLotID)
//				{
//					return false;
//				}
//			}
//		}
//	}
//	if( iPickCnt > 0)
//	{
//		for( i=0; i<PICKER_CNT; i++ )
//		{
//			if(g_robot.GetPicker(i).GetModule().GetLotNo() == strLotID)
//			{
//				return false;
//			}
//		}
//	}
//	
// 	if (g_handler.GetMachinePos() == EMPOS_REAR)
// 	{
//		for( it=0; it<MAX_TRAY; it++ )
//		{
//			if( it == iIdx )						continue;
//			ATray tray = g_Tray.GetTray( it );
//			if( tray.GetPos() == TPOS_NONE )		continue;
//			if( tray.GetStrLotID() != strLotID )	continue;
//			return false;
//		}
// 	}


	return true;
}

int APartTray::GetTrayCnt_byLotID( CString strLotID )
{
	int nRtn = 0;
	for( int it=0; it<MAX_TRAY; it++ )
	{
		ATray tray = g_Tray.GetTray( it );
		if( tray.GetPos() == TPOS_NONE )		continue;
		
		if( tray.GetStrLotID() != strLotID )	continue;

		nRtn++;
	}

	return nRtn;
}

ATray& APartTray::GetRejTopTray()
{
	if( m_vecRej.size() == 0 )
		return m_tray[MAX_TRAY - 1];
	
	int nCnt = m_vecRej.size();
	return GetTray( m_vecRej[nCnt - 1] );
}

bool APartTray::IsWorkingRejToNext()
{
//	for( int it=0; it<MAX_TRAY; it++ )
//	{
//		ATray tray = g_Tray.GetTray( it );
//		if( ( tray.GetPos() == TPOS_REJECT ||
//			tray.GetPos() == TPOS_REJECT_TO_BIN ||
//			tray.GetPos() == TPOS_BIN3 ||
//			tray.GetPos() == TPOS_BIN_TO_CONV3 ||
//			tray.GetPos() == TPOS_CONV3 ) &&
//			tray.GetTargetPos() == TPOS_NEXT )
//		{
//			return true;
//		}
//	}

	return false;
}

EIDBUFFER_POS APartTray::GetPosByStr( CString strPos )
{
	if( strPos == "FRONT" )								return IDPOS_FRONT;
	else if( strPos == "DENSITY" )						return IDPOS_DENSITY;
	else if( strPos == "BARCODE1" )						return IDPOS_BARCODE1;
	else if( strPos == "BARCODE2" )						return IDPOS_BARCODE2;
	else if( strPos == "TOP_VISION" )					return IDPOS_TOP_VISION;
	else if( strPos == "UNLOAD" )						return IDPOS_UNLOAD;
	else if( strPos == "CONVEYOR" )						return IDPOS_CONVEYOR;
	else if( strPos == "UNLOAD_BTM_LOAD" )				return IPOOS_UNLOAD_BTM_LOAD;
	else if( strPos == "UNLOAD_BTM_UNLOAD" )			return IPOOS_UNLOAD_BTM_UNLOAD;
	else if( strPos == "NEXT" )							return IDPOS_NEXT;
	else if( strPos == "FRONT_TO_DENSITY" )				return IDPOS_FRONT_TO_DENSITY;
	else if( strPos == "DENSITY_TO_BARCODE1" )			return IDPOS_DENSITY_TO_BARCODE1;

	else if( strPos == "BARCODE1_TO_BARCODE2" )			return IDPOS_BARCODE1_TO_BARCODE2;
	else if( strPos == "BARCODE2_TO_VISION" )			return IDPOS_BARCODE2_TO_VISION;
	else if( strPos == "TOPVISION_TO_UNLOAD" )			return IDPOS_TOPVISION_TO_UNLOAD;
	else if( strPos == "UNLOAD_TO_CONVEYOR" )			return IDPOS_UNLOAD_TO_CONVEYOR;
	else if( strPos == "CONVEYOR_TO_NEXT" )				return IDPOS_CONVEYOR_TO_NEXT;

	return IDPOS_NONE;
}

int APartTray::GetIdxByPos_Extra( EIDBUFFER_POS epos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray_extra[i].GetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

int APartTray::GetIdxByTargetPos_Extra( EIDBUFFER_POS epos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray_extra[i].GetTargetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

int APartTray::GetStopRejCnt()
{
	int iCnt = 0;
//	for( int i=0; i<MAX_TRAY; i++ )
//	{
//		if( m_tray[i].GetPos() == TPOS_REJECT )
//		{
//			iCnt++;
//		}
//	}

	return iCnt;
}

std::vector<int> APartTray::GetVecGoodCell_3Stacker()
{
	std::vector<int> vecRtn;
	
//	if( g_handler.GetPriorityBin() == TPOS_NONE )
//		return vecRtn;
//	
//	int iBin1 = g_Tray.GetIdxByPos( TPOS_BIN1 );
//	int iBin2 = g_Tray.GetIdxByPos( TPOS_BIN2 );
//	int iBin3 = g_Tray.GetIdxByPos( TPOS_BIN3 );
//	int ix=0, iy=0;
//	ETRAY_POS ePosSub;
//	if(st_handler.mn_mirror_type == CTL_YES)
//	{
//		ePosSub = TPOS_BIN1;
//	}
//	else
//	{
//		ePosSub = TPOS_BIN3;	
//	}
//	
//	int iIndexBinSub = MPOS_INDEX_BIN3;	
//	if( ePosSub == TPOS_BIN2 )	iIndexBinSub = MPOS_INDEX_BIN2;
//	if( ePosSub == TPOS_BIN1 )	iIndexBinSub = MPOS_INDEX_BIN1;
//	
//	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//	int iIdx = g_Tray.GetIdxByPos( ePosSub );
//	if( iIdx < 0 )
//		return vecRtn;
//	
//	ATray traySub = g_Tray.GetTray( iIdx );
//	
//	if( traySub.GetTargetPos() == ePosSub &&
//		traySub.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
//	{
//		for( int i=0; i<iMdlCnt; i++ )
//		{
//			if( traySub.GetModule(i).IsExist() && traySub.GetModule(i).GetModuleState() == EMS_GOOD )
//				vecRtn.push_back( iIndexBinSub + i );
//		}
//	}
	
	return vecRtn;
}


std::vector<int> APartTray::GetVecGoodCell_Sub()
{
	std::vector<int> vecRtn;

//	if( g_handler.GetPriorityBin() == TPOS_NONE )
//		return vecRtn;
//
//	int iBin1 = g_Tray.GetIdxByPos( TPOS_BIN1 );
//	int iBin2 = g_Tray.GetIdxByPos( TPOS_BIN2 );
//	int iBin3 = g_Tray.GetIdxByPos( TPOS_BIN3 );
//	int ix=0, iy=0;
//	ETRAY_POS ePosPri = g_handler.GetPriorityBin();
//	ETRAY_POS ePosSub = TPOS_NONE;
//
//	if(st_handler.mn_mirror_type == CTL_YES)
//	{
//		if(ePosPri == TPOS_BIN1)
//		{
//			if(iBin2 > -1)	ePosSub = TPOS_BIN2;
//			else if(iBin3 > -1)	ePosSub = TPOS_BIN3;
//			else			ePosSub = TPOS_NONE;
//		}
//		else
//		{
//			if(ePosPri == TPOS_BIN2) 
//			{
//				if(iBin3 > -1)	    ePosSub = TPOS_BIN3;
//				else if(iBin1 > -1 && st_handler.mb_rej_3stacker == false) ePosSub = TPOS_BIN1;
//				else                ePosSub = TPOS_NONE;
//			}
//			else
//			{
//				if(ePosPri == TPOS_BIN3)
//				{
//					if(iBin1 > -1 && st_handler.mb_rej_3stacker == false) ePosSub = TPOS_BIN1;
//					else
//					{
//						if(iBin2 > -1) ePosSub = TPOS_BIN2;
//						else           ePosSub = TPOS_NONE;
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//		if(ePosPri == TPOS_BIN1)
//		{
//			if(iBin2 > -1)	ePosSub = TPOS_BIN2;
//			else if(iBin3 > -1 && st_handler.mb_rej_3stacker == false)	ePosSub = TPOS_BIN3;
//			else			ePosSub = TPOS_NONE;
//		}
//		else
//		{
//			if(ePosPri == TPOS_BIN2) 
//			{
//				if(iBin3 > -1 && st_handler.mb_rej_3stacker == false)	    ePosSub = TPOS_BIN3;
//				else if(iBin1 > -1) ePosSub = TPOS_BIN1;
//				else                ePosSub = TPOS_NONE;
//			}
//			else
//			{
//				if(ePosPri == TPOS_BIN3 && st_handler.mb_rej_3stacker == false)
//				{
//					if(iBin1 > -1) ePosSub = TPOS_BIN1;
//					else
//					{
//						if(iBin2 > -1) ePosSub = TPOS_BIN2;
//						else           ePosSub = TPOS_NONE;
//					}
//				}
//			}
//		}
//	}
//	int iIndexBinPri = MPOS_INDEX_BIN3;
//	int iIndexBinSub = MPOS_INDEX_BIN3;
//
//	if( ePosPri == TPOS_BIN2 )	iIndexBinPri = MPOS_INDEX_BIN2;
//	if( ePosSub == TPOS_BIN2 )	iIndexBinSub = MPOS_INDEX_BIN2;
//	if( ePosPri == TPOS_BIN1 )	iIndexBinPri = MPOS_INDEX_BIN1;
//	if( ePosSub == TPOS_BIN1 )	iIndexBinSub = MPOS_INDEX_BIN1;
//
//	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//	int iIdx = g_Tray.GetIdxByPos( ePosSub );
//	if( iIdx < 0 )
//		return vecRtn;
//
//	ATray traySub = g_Tray.GetTray( iIdx );
//
//	if( traySub.GetTargetPos() == ePosSub &&
//		traySub.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
//	{
//		for( int i=0; i<iMdlCnt; i++ )
//		{
//			if( traySub.GetModule(i).IsExist() && traySub.GetModule(i).GetModuleState() == EMS_GOOD )
//				vecRtn.push_back( iIndexBinSub + i );
//		}
//	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecEmptyCell_Pri()
{
	std::vector<int> vecRtn;

//	if( g_handler.GetPriorityBin() == TPOS_NONE )
//		return vecRtn;
//
//	ETRAY_POS ePosPri = g_handler.GetPriorityBin();
//
//	int iIndexBinPri = MPOS_INDEX_BIN3;
//
//	if( ePosPri == TPOS_BIN2 )	iIndexBinPri = MPOS_INDEX_BIN2;
//	if( ePosPri == TPOS_BIN1 )	iIndexBinPri = MPOS_INDEX_BIN1;
//
//	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
//	int iIdx = g_Tray.GetIdxByPos( ePosPri );
//	if( iIdx < 0 )
//		return vecRtn;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );
//
//	if( trayPri.GetTargetPos() == ePosPri &&
//		trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
//	{
//		for( int i=0; i<iMdlCnt; i++ )
//		{
//			if( trayPri.GetModule(i).IsExist() == false  )
//				vecRtn.push_back( iIndexBinPri + i );
//		}
//	}

	return vecRtn;
}


std::vector<int> APartTray::GetVecFeederPickupLabel( int nLotIdx)
{
	std::vector<int> vecRtn;
	std::multimap<int, int> mapCntSite;


	int iIndexBin = MPOS_INDEX_DENSITY;

	int nCnt=0;
	if( st_basic.nDensityFeederUse[st_basic.mn_device_type] == CTL_YES &&
		st_basic.nDensityFeederUse[st_basic.mn_device_type + 3] == CTL_YES)
	{
		if( st_handler.m_bPrint_Pickup[st_basic.mn_device_type] == true &&
			st_handler.m_bPrint_Pickup[st_basic.mn_device_type + 3] == true )
		{
			nCnt++;
			mapCntSite.insert( std::pair<int,int>( nCnt, st_basic.mn_device_type ) );
			nCnt++;
			mapCntSite.insert( std::pair<int,int>( nCnt, st_basic.mn_device_type+3 ) );
		}
	}
	else if( st_basic.nDensityFeederUse[st_basic.mn_device_type] == CTL_YES )
	{
		if( st_handler.m_bPrint_Pickup[st_basic.mn_device_type] == true )
		{
			nCnt++;
			mapCntSite.insert( std::pair<int,int>( nCnt, st_basic.mn_device_type ) );
		}
	}
	else if( st_basic.nDensityFeederUse[st_basic.mn_device_type + 3] == CTL_YES)
	{
		if( st_handler.m_bPrint_Pickup[st_basic.mn_device_type + 3] == true )
		{
			nCnt++;
			mapCntSite.insert( std::pair<int,int>( nCnt, st_basic.mn_device_type+3 ) );
		}
	}

	std::multimap<int,int>::iterator it = mapCntSite.begin();
	for( it; it != mapCntSite.end(); it++ )
	{
		int a = it->second;
		vecRtn.push_back( it->second );
	}

	return vecRtn;
}



std::vector<int> APartTray::GetVecDensityTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();
	int iIndexBin = MPOS_INDEX_DENSITY;

	std::multimap<int, int> mapCntByX;
	int nCnt = 0, nNum = 0;
	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[ix] || st_handler.n_only_work_density)
			nCnt++;
		else if(st_var.mn_lotend_density_flag == CTL_YES)//2015.0405
			nCnt++;
		if( st_den_rbt[ix].bPick == true && st_den_rbt[ix].st_label.nState == EMBS_PASS )
			nNum++;

		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
	}
	if( nCnt != nNum || nCnt == 0 )  return vecRtn;

	std::multimap<int,int>::iterator it = mapCntByX.begin();

	for( it; it != mapCntByX.end(); it++ )
	{
		if( nCnt > 0)
			vecRtn.push_back( iIndexBin + it->second );
	}

	return vecRtn;

//
//	int iIdx = g_Tray.GetIdxByPos( IDPOS_DENSITY );
//	if( iIdx < 0 )											return vecRtn;
//	int iIndexBin = MPOS_INDEX_DENSITY;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );
//
//	if( trayPri.GetWorkState() == EWS_DENSITY )
//		return vecRtn;
//
//	std::multimap<int, int> mapCntByX;
//	int nCnt = 0, nNum = 0;
//	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
//	{
//		if( trayPri.GetModule(ix).IsExist())
//			nCnt++;
//		if( Func.m_pRobot[SITE_DENSITY]->GetPicker(ix).GetLabel().GetLabelState() >= EMBS_PASS )
//			nNum++;
//
//		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
//	}
//
//	if( nCnt != nNum || nCnt == 0 )  return vecRtn;
//
//	std::multimap<int,int>::iterator it = mapCntByX.begin();
//
//	for( it; it != mapCntByX.end(); it++ )
//	{
//		if( nCnt > 0)
//			vecRtn.push_back( iIndexBin + it->second );
//	}
//
//	return vecRtn;
}


std::vector<int> APartTray::GetVecBcr1TestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

	int iIndexBin = MPOS_INDEX_BCR1_LOAD;
	int nRet = RET_GOOD;
	std::multimap<int, int> mapCntByX;
	int nCnt = 0;
	st_handler.m_nBcrRbtResult[0] = RET_GOOD;

	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{

		CString strRbt="";
		CString strIDBuff="";
		CString strIDBuff2="";
		CString strParse="";
		char normal_msg[500]={0,};

		if( st_basic.nVisionUse[1] == 1 && st_bcr_rbt[0].bPick[ix] &&
			st_bcr_rbt[0].st_bcr[ix].bOut && st_bcr_rbt[0].st_bcr[ix].nState == EMBS_PASS )
		{
// 			if( 1 || Func.CmpStrFile( st_bcr_rbt[0].st_bcr[ix].c_serial, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[ix] ) )
// 				nCnt++;

			strRbt.Format("%s", st_bcr_rbt[0].st_bcr[ix].c_serial);
			strIDBuff.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[ix]);
			strRbt.TrimLeft(); strRbt.TrimRight();
			strIDBuff.TrimLeft(); strIDBuff.TrimRight();
			if(strIDBuff.GetLength() >= strRbt.GetLength())
			{
				strParse = strIDBuff.Mid(0, strRbt.GetLength());
				if( Func.CmpStrFile(strParse, strRbt) )
				{
					nCnt++;
					mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
				}
// 				else
// 				{
// 					//2015.0327
// 					GetCheckBarcode1TrayID( ix, strRbt);					
// 					///////////////////////////////////////////////////////////////////////////
// 					nCnt++;
// 					mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
// 
// 					sprintf( normal_msg, "[BCR1POS]Rv_%d=%s Inpt_%d=%s D_%d=%s B1_%d=%s B2_%d=%s strParse=%s, strRbt=%s",
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[ix],
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].c_serial[ix],
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[ix], 
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[ix],
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[ix], strParse, strRbt );
// 					Func.On_LogFile_Add(LOG_BARCODE, normal_msg);
// 				}
			}
		}
		else if( st_basic.nVisionUse[1] == 0 && st_bcr_rbt[0].bPick[ix] && /*st_bcr_rbt[0].st_bcr[ix].bExist &&*/ 
		   st_bcr_rbt[0].st_bcr[ix].bOut && st_bcr_rbt[0].st_bcr[ix].nState == EMBS_PASS )
		{
		   nCnt++;
		   mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
		}

	}

	if(nCnt < 2)
	{
		//2015.0607
		nRet = GetCheckBarcode1TrayID();
		if(nRet == RET_REJECT)
		{
			st_bcr_rbt[0].st_bcr[0].nState = EMBS_FAIL;
			st_bcr_rbt[0].st_bcr[1].nState = EMBS_FAIL;
			st_handler.m_nBcrRbtResult[0] = RET_REJECT;
			for ( int k = 0; k < 2; k++)
			{
				mapCntByX.insert( std::pair<int,int>( k, k ) );		
			}
		}
		else if(nRet == RET_SKIP)
		{
			st_handler.m_nBcrRbtResult[0] = RET_SKIP;
		}
		else
		{
			st_handler.m_nBcrRbtResult[0] = RET_ERROR;//holding.. take out everything
		}
	}
	
	std::multimap<int,int>::iterator it = mapCntByX.begin();
	
	for( it; it != mapCntByX.end(); it++ )
	{
		if(nCnt > 0)
		{
			vecRtn.push_back( iIndexBin + it->second );
		}
	}

//	int iIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE1 );
//	if( iIdx < 0 )	return vecRtn;
//	int iIndexBin = MPOS_INDEX_BCR1_LOAD;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );

//	std::multimap<int, int> mapCntByX;
//	int nCnt = 0;
//	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
//	{
//		if( trayPri.GetModule(ix).IsExist() && !trayPri.GetModule(ix).GetBcrLabel().IsExist() &&
//			Func.m_pRobot[SITE_BARCODE1]->GetPicker(ix).GetBcrLabel().GetBcrLabelState() == EMBS_PASS)
//			nCnt++;
//		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
//	}
//
//	std::multimap<int,int>::iterator it = mapCntByX.begin();
//
//	for( it; it != mapCntByX.end(); it++ )
//	{
//		if(nCnt > 0)
// 			vecRtn.push_back( iIndexBin + it->second );
//	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecBcr2TestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

	int iIndexBin = MPOS_INDEX_BCR2_LOAD;
	std::multimap<int, int> mapCntByX;
	int nCnt = 0;
	int nRet = RET_GOOD;
	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{
		CString strRbt="";
		CString strIDBuff="";
		CString strParse="";
		char normal_msg[500]={0,};

		if( st_basic.nVisionUse[1] == 1 && st_bcr_rbt[1].bPick[ix] &&
			st_bcr_rbt[1].st_bcr[ix].bOut && st_bcr_rbt[1].st_bcr[ix].nState == EMBS_PASS )
		{
// 			if( Func.CmpStrFile( st_bcr_rbt[1].st_bcr[ix].c_serial, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[ix] ) ||
// 				strcmp( st_bcr_rbt[1].st_bcr[ix].c_serial, "FAIL") == 0 )
// 				nCnt++;
			strRbt.Format("%s", st_bcr_rbt[1].st_bcr[ix].c_serial);
			strIDBuff.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[ix]);
			strRbt.TrimLeft(); strRbt.TrimRight();
			strIDBuff.TrimLeft(); strIDBuff.TrimRight();
			if(strIDBuff.GetLength() >= strRbt.GetLength())
			{
				strParse = strIDBuff.Mid(0, strRbt.GetLength());
				if( Func.CmpStrFile(strParse, strRbt) )
				{
					nCnt++;
					mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
				}
				else
				{
					int nnWCnt = 0;
					
					if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_LEFT] == 0 ) nnWCnt++;
					if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].nBcrExist[CTL_RIGHT] == 0 ) nnWCnt++;
					
					if( nnWCnt == 0 )
					{
						st_sync.n_bcr_2_work_req = CTL_REQ;
						return vecRtn;
					}
// 					//2015.0327
// 					GetCheckBarcode2TrayID( ix, strRbt);					
// 					///////////////////////////////////////////////////////////////////////////
// // 					nCnt++;
// // 					mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
// 					
// 					sprintf( normal_msg, "[BCR2POS]Rv_%d=%s Inpt_%d=%s D_%d=%s B1_%d=%s B2_%d=%s strParse=%s, strRbt=%s",
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[ix],
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].c_serial[ix],
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[ix], 
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[ix],
// 						ix, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[ix], strParse, strRbt );
// 					Func.On_LogFile_Add(LOG_BARCODE, normal_msg);
				}
			}
		}
		else if( st_basic.nVisionUse[1] == 0 && st_bcr_rbt[1].bPick[ix] && st_bcr_rbt[1].st_bcr[ix].bExist && 
			st_bcr_rbt[1].st_bcr[ix].bOut && st_bcr_rbt[1].st_bcr[ix].nState == EMBS_PASS)
		{
			nCnt++;
			mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
		}

	}
	if(nCnt < 2)
	{
		//2015.0607
		nRet = GetCheckBarcode2TrayID();					
		if(nRet == RET_REJECT)
		{
			st_bcr_rbt[1].st_bcr[0].nState = EMBS_FAIL;
			st_bcr_rbt[1].st_bcr[1].nState = EMBS_FAIL;
			st_handler.m_nBcrRbtResult[1] = RET_REJECT;
			for ( int k = 0; k < 2; k++)
			{
				mapCntByX.insert( std::pair<int,int>( k, k ) );		
			}
		}
		else if(nRet == RET_SKIP)
		{
			st_handler.m_nBcrRbtResult[1] = RET_SKIP;
		}
		else
		{
			st_handler.m_nBcrRbtResult[1] = RET_ERROR;//holding.. take out everything
			for ( int k = 0; k < 2; k++)
			{
				mapCntByX.insert( std::pair<int,int>( k, k ) );		
			}
		}
	}
	std::multimap<int,int>::iterator it = mapCntByX.begin();

	for( it; it != mapCntByX.end(); it++ )
	{
		int nIndex = it->second;
		if(nCnt > 0)
 			vecRtn.push_back( iIndexBin + it->second );
	}


//	int iIdx = g_Tray.GetIdxByPos( IDPOS_BARCODE2 );
//	if( iIdx < 0 )	return vecRtn;
//	int iIndexBin = MPOS_INDEX_BCR2_LOAD;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );
//
//	std::multimap<int, int> mapCntByX;
//	int nCnt = 0;
//	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
//	{
//		if( trayPri.GetModule(ix).IsExist() && trayPri.GetModule(ix).IsTestDensityState() )
//			nCnt++;
//		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
//	}
//
//	std::multimap<int,int>::iterator it = mapCntByX.begin();
//
//	for( it; it != mapCntByX.end(); it++ )
//	{
// 		vecRtn.push_back( iIndexBin + it->second );
//	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecTopVisTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

//	int iIdx = g_Tray.GetIdxByPos( IDPOS_TOP_VISION );
//	if( iIdx < 0 )	return vecRtn;
	int iIndexBin = MPOS_INDEX_TOPVISION;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );

	std::multimap<int, int> mapCntByX;
	int nCnt = 0;
	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{
//		if( trayPri.GetModule(ix).IsExist() && trayPri.GetModule(ix).GetBcrLabel().GetBcrLabelState() == EMBS_PASS )
//			nCnt++;
// 		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[0] || st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[1] )
// 			nCnt++;
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[ix] )
		{
			nCnt++;
			mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
		}

	}

	std::multimap<int,int>::iterator it = mapCntByX.begin();

	for( it; it != mapCntByX.end(); it++ )
	{
 		vecRtn.push_back( iIndexBin + it->second );
	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecUnloadTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

//	int iIdx = g_Tray.GetIdxByPos( IDPOS_UNLOAD );
//	if( iIdx < 0 )	return vecRtn;
	int iIndexBin = MPOS_INDEX_UNLOAD;

//	ATray trayPri = g_Tray.GetTray( iIdx );

	std::multimap<int, int> mapCntByX;
	int nCnt = 0;
	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{
//		if( trayPri.GetModule(ix).IsExist() && (trayPri.GetModule(ix).IsBufferSkip() == false) )//디바이스가 있으면일단 픽업
//			nCnt++;
		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[ix] )
			nCnt++;

		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
	}

	std::multimap<int,int>::iterator it = mapCntByX.begin();

	for( it; it != mapCntByX.end(); it++ )
	{
		if(nCnt>0)
		{
 			vecRtn.push_back( iIndexBin + it->second );
		}
	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecUnloadElvTray( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

//	int iIdx = g_Tray.GetIdxByPos( IDPOS_UNLOAD );
//	if( iIdx < 0 )	return vecRtn;
	int iIndexBin = MPOS_INDEX_ELV;
//
//	ATray trayPri = g_Tray.GetTray( iIdx );

	std::multimap<int, int> mapCntByX;
	int nCnt = 0, nTray = 0;
	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{
//		if( (trayPri.GetModule(ix).IsBufferSkip() == true) )
//		{
//			nCnt++;
//		}

// 		if( !trayPri.GetModule(ix).IsExist() )
// 			nCnt++;
//		if( trayPri.GetModule(ix).IsIDBufferExist() )
//			nTray++;

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_idbuffer[ix] == 1 )
			nCnt++;
		if( (/*st_basic.n_mode_device == false || */st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[ix] == 0) )
			nTray++;

		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
	}
	if( nCnt != nTray || nCnt == 0) return vecRtn;

	std::multimap<int,int>::iterator it = mapCntByX.begin();

	for( it; it != mapCntByX.end(); it++ )
	{
		if( nCnt > 0)
 			vecRtn.push_back( iIndexBin + it->second );
	}

	return vecRtn;
}

std::vector<int> APartTray::GetVecPickerUnloadConvey( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

	int iIndexBin = MPOS_INDEX_CONVEYOR;
// 	if( g_ioMgr.get_in_bit(st_io.i_uld_cv_left_box_chk, IO_OFF) == IO_OFF &&
// 		g_ioMgr.get_in_bit(st_io.i_uld_cv_right_box_chk, IO_OFF) == IO_OFF &&
// 		g_ioMgr.get_in_bit(st_io.i_uld_cv_box_out_chk, IO_OFF) == IO_OFF &&
// 		g_ioMgr.get_in_bit(st_io.i_reject_cv_full_pos_chk, IO_OFF) == IO_OFF )
// 	{
		if( st_uld_rbt.n_exist[0] || st_uld_rbt.n_exist[1] )
		{
			//2015.0211
			if( st_uld_rbt.st_btm[0].nState == EMBS_PASS || st_uld_rbt.st_btm[1].nState == EMBS_PASS )
			{
				std::multimap<int, int> mapCntByX;
				int nCnt = 0;
				for( int ix=0; ix<MAX_BUFFDVC; ix++ )
				{
					if( st_uld_rbt.n_exist[ix] == 1 && st_uld_rbt.st_btm[ix].nState == EMBS_PASS)
					{
						nCnt++;
						mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
					}
// 					mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
				}
				std::multimap<int,int>::iterator it = mapCntByX.begin();

				for( it; it != mapCntByX.end(); it++ )
				{
					if(nCnt > 0)
 						vecRtn.push_back( iIndexBin + it->second );
				}
			}
		}
// 	}

	return vecRtn;
}


std::vector<int> APartTray::GetVecUnloadConveyorModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();

	int iIdx = g_Tray.GetIdxByPos( IDPOS_CONVEYOR );
	if( iIdx < 0 )	return vecRtn;
	int iIndexBin = MPOS_INDEX_CONVEYOR;

	ATray trayPri = g_Tray.GetTray( iIdx );

	std::multimap<int, int> mapCntByX;
	int nCnt = 0;
	for( int ix=0; ix<MAX_BUFFDVC; ix++ )
	{
//		if( trayPri.GetModule(ix).IsExist() && trayPri.GetModule(ix).GetModuleState() == EMS_VISION &&
//			trayPri.GetModule(ix).GetTopVision().GetTopVisionState() == EMBS_PASS)
		if( trayPri.GetModule(ix).IsExist() )//디바이스가 있으면일단 픽업
			nCnt++;
		mapCntByX.insert( std::pair<int,int>( nCnt, ix ) );
	}

	std::multimap<int,int>::iterator it = mapCntByX.begin();

	for( it; it != mapCntByX.end(); it++ )
	{
 		vecRtn.push_back( iIndexBin + it->second );
	}

	return vecRtn;
}


int APartTray::GetCheckBarcode1TrayID()
{
	CString strParse="";
	CString strIDBuff="";
	CString strRbtBcr="";
	int i = 0, j =0, nPos = 0;
	int FuncRet = RET_GOOD;
	
	for ( i = SHIFT_IDBUFF_INDEX_BCR2; i <= SHIFT_IDBUFF_INDEX_ULD; i++ )
	{
		nPos = 0;
		for ( j = 0; j < MAX_BUFFDVC; j++)
		{
			if( st_idbuff_info[i].n_exist[j] == 1 )
			{
				strIDBuff.Format("%s", st_idbuff_info[i].c_serial[j]);
				strIDBuff.TrimLeft(); strIDBuff.TrimRight();
				strRbtBcr.Format("%s", st_bcr_rbt[0].st_bcr[j].c_serial);
				strParse = strIDBuff.Mid(0, strRbtBcr.GetLength());
				if( Func.CmpStrFile(strParse, strRbtBcr) )
				{
					//st_bcr_rbt[i].st_bcr[j].nState = EMBS_FAIL;
					nPos++;
					sprintf( st_msg.c_normal_msg, "[BCR1POS]Rv_%d=%s Inpt_%d=%s D_%d=%s B1_%d=%s B2_%d=%s strParse=%s, strRbt=%s",
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[j],
						j, st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].c_serial[j],
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[j], 
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[j],
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[j], strParse, strRbtBcr );
					Func.On_LogFile_Add(2, st_msg.c_normal_msg);
				}
			}
		}
		if(nPos == 2)
		{
			FuncRet = RET_REJECT;//reject
		}
		else if(nPos < 2 || nPos > 2)
		{
			FuncRet = RET_ERROR;//holding..big problem
		}
	}
	
	for ( i = SHIFT_IDBUFF_INDEX_ULD_RECV; i < SHIFT_IDBUFF_INDEX_BCR1; i++ )
	{
		nPos = 0;
		for ( j = 0; j < MAX_BUFFDVC; j++)
		{
			if( st_idbuff_info[i].n_exist[j] == 1 )
			{
				strIDBuff.Format("%s", st_idbuff_info[i].c_serial[j]);
				strIDBuff.TrimLeft(); strIDBuff.TrimRight();
				strRbtBcr.Format("%s", st_bcr_rbt[0].st_bcr[j].c_serial);
				strParse = strIDBuff.Mid(0, strRbtBcr.GetLength());
				if( Func.CmpStrFile(strParse, strRbtBcr) )
				{
					//st_bcr_rbt[0].st_bcr[j].nState = EMBS_SKIP;
					nPos++;
				}
			}
		}
		if(nPos == 2)
		{
			FuncRet = RET_SKIP;//reject
		}
		else if(nPos < 2 || nPos > 2)
		{
			FuncRet = RET_ERROR;//holding..big problem
		}
	}
	return FuncRet;
}

int APartTray::GetCheckBarcode2TrayID()
{
	CString strParse="";
	CString strIDBuff="";
	CString strRbtBcr="";
	int i = 0, j =0, nPos = 0;
	int FuncRet = RET_GOOD;
	st_handler.m_nBcrRbtResult[0] = RET_GOOD;
	
	for ( i = SHIFT_IDBUFF_INDEX_TOP; i <= SHIFT_IDBUFF_INDEX_ULD; i++ )
	{
		nPos = 0;
		for ( j = 0; j < MAX_BUFFDVC; j++)
		{
			if( st_idbuff_info[i].n_exist[j] == 1 )
			{
				strIDBuff.Format("%s", st_idbuff_info[i].c_serial[j]);
				strIDBuff.TrimLeft(); strIDBuff.TrimRight();
				strRbtBcr.Format("%s", st_bcr_rbt[1].st_bcr[j].c_serial);
				strParse = strIDBuff.Mid(0, strRbtBcr.GetLength());
				if( Func.CmpStrFile(strParse, strRbtBcr) )
				{
					nPos++;
					sprintf( st_msg.c_normal_msg, "[BCR2POS]Rv_%d=%s Inpt_%d=%s D_%d=%s B1_%d=%s B2_%d=%s strParse=%s, strRbt=%s",
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD_RECV].c_serial[j],
						j, st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].c_serial[j],
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[j], 
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[j],
						j, st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[j], strParse, strRbtBcr );
					Func.On_LogFile_Add(2, st_msg.c_normal_msg);
					
				}
			}
		}
		if(nPos == 2)
		{
			FuncRet = RET_REJECT;//reject
			return FuncRet;
		}
		else if( (nPos < 2 || nPos > 2) && nPos > 0)
		{
			FuncRet = RET_ERROR;//holding..big problem
			return FuncRet;
		}
	}
	
	for ( i = SHIFT_IDBUFF_INDEX_ULD_RECV; i < SHIFT_IDBUFF_INDEX_BCR2; i++ )
	{
		nPos = 0;
		for ( j = 0; j < MAX_BUFFDVC; j++)
		{
			if( st_idbuff_info[i].n_exist[j] == 1 )
			{
				strIDBuff.Format("%s", st_idbuff_info[i].c_serial[j]);
				strIDBuff.TrimLeft(); strIDBuff.TrimRight();
				strRbtBcr.Format("%s", st_bcr_rbt[1].st_bcr[j].c_serial);
				strParse = strIDBuff.Mid(0, strRbtBcr.GetLength());
				if( Func.CmpStrFile(strParse, strRbtBcr) )
				{
					nPos++;
				}
			}
		}
		if(nPos == 2)
		{
			FuncRet = RET_SKIP;//reject
			return FuncRet;
		}
		else if( (nPos < 2 || nPos > 2) && nPos > 0)
		{
			FuncRet = RET_ERROR;//holding..big problem
			return FuncRet;
		}
	}
	
	return FuncRet;
}