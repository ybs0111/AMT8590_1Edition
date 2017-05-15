// APartTestSite.h: interface for the APartTestSite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTTESTSITE_H__16AD9BE6_1112_4935_A65A_3F35452F2DCE__INCLUDED_)
#define AFX_APARTTESTSITE_H__16AD9BE6_1112_4935_A65A_3F35452F2DCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PartFunction.h"
#include <vector>
// #include <algorithm>
// #include <functional>
//std::sort( vecRtn.begin(), vecRtn.end(), std::greater<int>() );


class APartTestSite  
{
public:
	APartTestSite();
	virtual ~APartTestSite();


	
	std::vector<int> GetVecLabelFeeder();

	std::vector<int> GetVecBarcode1Feeder();
	std::vector<int> GetVecBarcode2Feeder();


	AModule& GetModule( int iIdx) { return m_mdl[iIdx]; }
	void SetModule( int iIdx, AModule mdl ) { m_mdl[iIdx] = mdl; }
	void EraseModule( int iIdx ) { m_mdl[iIdx].EraseData(); }

	void AddCount( int iIdx );
//	int GetTestCount( int iIdx ) { return m_nTestCnt[st_basic.n_cok_type][iIdx]; }

	int GetModuleCnt();


	ALabel& GetLabel( int iIdx) { return m_ldl[iIdx]; }
	void SetLabel( int iIdx, ALabel mdl ) { m_ldl[iIdx] = mdl; }
	ABcrLabel& GetBcrLabel( int iIdx) { return m_bcrldl[iIdx]; }
	void SetBcrLabel( int iIdx, ABcrLabel mdl ) { m_bcrldl[iIdx] = mdl; }


protected:
	AModule	m_mdl[2];
	ALabel m_ldl[6];
	ABcrLabel m_bcrldl[4];
	
};
extern APartTestSite g_site[20];

#endif // !defined(AFX_APARTTESTSITE_H__16AD9BE6_1112_4935_A65A_3F35452F2DCE__INCLUDED_)
