// APartTestSite.cpp: implementation of the APartTestSite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartTestSite.h"
#include "../AMTRegistry.h"
#include "../AMTLotManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <map>
#include <utility>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//APartTestSite g_density_site;
//APartTestSite g_bcr_site;
//APartTestSite g_bcr1;
//APartTestSite g_bcr2;
//APartTestSite g_site;
APartTestSite g_site[20];
APartTestSite::APartTestSite()
{
}

APartTestSite::~APartTestSite()
{

}

void APartTestSite::AddCount( int iIdx )
{
//	m_nTestCnt[st_basic.n_cok_type][iIdx]++;

	CString strVal;
//	strVal.Format( "%s_%d_%02d", REG_VAL_COUNT_TS, st_basic.n_cok_type, iIdx );
//	AMTRegistry::RegWriteInt( REG_KEY_COUNT, strVal, m_nTestCnt[st_basic.n_cok_type][iIdx] );
}

int APartTestSite::GetModuleCnt()
{
	int nRtn = 0;

	for( int i=0; i<TSITE_SOCKET_CNT; i++ )
	{
		if( m_mdl[i].IsExist() &&
			m_mdl[i].GetLotNo() == g_lotMgr.GetLotIDAt(0) )
			nRtn++;
	}

	return nRtn;
}


std::vector<int> APartTestSite::GetVecLabelFeeder()
{
	std::vector<int> vecRtn;
	std::multimap<int, int> mapCntSite;

	int iIndexBin = MPOS_INDEX_DENSITY_FEEDER;
	int nCnt=0;

	
	if( st_basic.nDensityFeederUse[st_basic.mn_device_type] == CTL_YES &&
		st_basic.nDensityFeederUse[st_basic.mn_device_type + 3] == CTL_YES)
	{
//		if( m_ldl[st_basic.mn_device_type].GetLabelState() >= EMBS_PASS && m_ldl[st_basic.mn_device_type+3].GetLabelState() >= EMBS_PASS )
		if( st_den_feeder[st_basic.mn_device_type].st_label.nState >= EMBS_PASS && 
			st_den_feeder[st_basic.mn_device_type+3].st_label.nState >= EMBS_PASS)
		{
			nCnt++;
			mapCntSite.insert( std::pair<int,int>( nCnt, st_basic.mn_device_type ) );
			nCnt++;
			mapCntSite.insert( std::pair<int,int>( nCnt, st_basic.mn_device_type+3 ) );
		}
	}

	std::multimap<int,int>::iterator it = mapCntSite.begin();
	for( it; it != mapCntSite.end(); it++ )
	{
		int a = it->second;
		if( nCnt > 0)
			vecRtn.push_back( iIndexBin + it->second );
	}
	return vecRtn;

}

std::vector<int> APartTestSite::GetVecBarcode1Feeder()
{
	std::vector<int> vecRtn;
	std::multimap<int, int> mapCntSite;

	int iIndexBin = MPOS_INDEX_BCR1_FEEDER;
	int nCnt=0,nEnable=0,nWork=0;


	for ( int i = 0; i < 2; i++)
	{
		if( st_basic.nBcrFeederUse[i] == CTL_YES ) nEnable++;
		if( st_bcr_feeder[i].st_bcr.bOut == true ) nWork++;//fail일경우 exist만 false
	}

	if( nWork == nEnable) 
	{
//		if( st_basic.nBcrFeederUse[0] == CTL_YES )
//		{
//			if( Func.m_pSite[SITE_BARCODE1]->GetBcrLabel(0).GetBcrLabelState() == EMBS_PASS )
//			{
//				nCnt++;
//				mapCntSite.insert( std::pair<int,int>( nCnt, 0 ) );
//			}
//		}
//		if( st_basic.nBcrFeederUse[1] == CTL_YES)
//		{
//			if( Func.m_pSite[SITE_BARCODE1]->GetBcrLabel(1).GetBcrLabelState() == EMBS_PASS )
//			{
//				nCnt++;
//				mapCntSite.insert( std::pair<int,int>( nCnt, 1 ) );
//			}
//		}
		if( st_basic.nBcrFeederUse[0] == CTL_YES )
		{
			if( st_bcr_rbt[0].bPick[0] == false && st_bcr_feeder[0].st_bcr.nState == EMBS_PASS )
			{
				nCnt++;
				mapCntSite.insert( std::pair<int,int>( nCnt, 0 ) );
			}
		}
		if( st_basic.nBcrFeederUse[1] == CTL_YES)
		{
			if( st_bcr_rbt[0].bPick[1] == false &&st_bcr_feeder[1].st_bcr.nState == EMBS_PASS )
			{
				nCnt++;
				mapCntSite.insert( std::pair<int,int>( nCnt, 1 ) );
			}
		}

		std::multimap<int,int>::iterator it = mapCntSite.begin();
		for( it; it != mapCntSite.end(); it++ )
		{
			if( nCnt > 0 && nCnt >= 2 )//2016.0918
			{
				int a = it->second;
				vecRtn.push_back( iIndexBin + it->second );
			}
		}
	}
	return vecRtn;
}

std::vector<int> APartTestSite::GetVecBarcode2Feeder()
{
	std::vector<int> vecRtn;
	std::multimap<int, int> mapCntSite;

	int iIndexBin = MPOS_INDEX_BCR2_FEEDER;
	int nCnt=0,nEnable=0,nWork=0;

	for ( int i = 0; i < 2; i++)
	{
		if( st_basic.nBcrFeederUse[i+2] == CTL_YES ) nEnable++;
		if( st_bcr_feeder[i+2].st_bcr.bOut == true ) nWork++;
	}

	if(  nWork == nEnable ) 
	{
//		if( st_basic.nBcrFeederUse[2] == CTL_YES )
//		{
//			if( Func.m_pSite[SITE_BARCODE2]->GetBcrLabel(2).GetBcrLabelState() == EMBS_PASS )
//			{
//				nCnt++;
//				mapCntSite.insert( std::pair<int,int>( nCnt, 0 ) );
//			}
//		}
//		if( st_basic.nBcrFeederUse[3] == CTL_YES)
//		{
//			if( Func.m_pSite[SITE_BARCODE2]->GetBcrLabel(3).GetBcrLabelState() == EMBS_PASS )
//			{
//				nCnt++;
//				mapCntSite.insert( std::pair<int,int>( nCnt, 1 ) );
//			}
//		}
		if( st_basic.nBcrFeederUse[2] == CTL_YES )
		{//2016.0614 st_bcr_rbt[2].bPick[0] ->st_bcr_rbt[1].bPick[0]
			if( st_bcr_rbt[1].bPick[0] == false && st_bcr_feeder[2].st_bcr.nState == EMBS_PASS )
			{
				nCnt++;
				mapCntSite.insert( std::pair<int,int>( nCnt, 0 ) );
			}
		}
		if( st_basic.nBcrFeederUse[3] == CTL_YES )
		{//2016.0614 st_bcr_rbt[2].bPick[0] ->st_bcr_rbt[1].bPick[0]
			if( st_bcr_rbt[1].bPick[1] == false && st_bcr_feeder[3].st_bcr.nState == EMBS_PASS )
			{
				nCnt++;
				mapCntSite.insert( std::pair<int,int>( nCnt, 1 ) );			
			}
		}

		std::multimap<int,int>::iterator it = mapCntSite.begin();
		for( it; it != mapCntSite.end(); it++ )
		{
			if( nCnt > 0 && nCnt >= 2 )//2016.0918
			{
				int a = it->second;
				vecRtn.push_back( iIndexBin + it->second );
			}
		}
	}
	return vecRtn;
}