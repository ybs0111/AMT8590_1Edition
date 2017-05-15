// DataManager.cpp: implementation of the DataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "DataManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataManager g_dataMgr;

void AData::InitData()
{
	m_pData = NULL;
	m_dt = DT_BOOL;
	
	m_bData = false;
	m_nData = -1;
	m_strData = "";
	m_dData = -1;
}

void AData::UpdateData()
{
	switch( m_dt )
	{
	case DT_BOOL:
		{
			bool* pbTemp;
			pbTemp = (bool*)m_pData;
			*pbTemp = m_bData;
		}
		break;

	case DT_INT:
		{
			int* piTemp;
			piTemp = (int*)m_pData;
			*piTemp = m_nData;
		}
		break;

	case DT_DOUBLE:
		{
			double* pdTemp;
			pdTemp = (double*)m_pData;
			*pdTemp = m_dData;
		}
		break;

	case DT_STR:
		{
			CString* pstrTemp;
			pstrTemp = (CString*)m_pData;
			*pstrTemp = m_strData;
		}
		break;
	}
}

void AData::BackupData()
{
	switch( m_dt )
	{
	case DT_BOOL:
		{
			bool* pbTemp;
			pbTemp = (bool*)m_pData;

			m_bData = *pbTemp;
		}
		break;

	case DT_INT:
		{
			int* piTemp;
			piTemp = (int*)m_pData;
			m_nData = *piTemp;
		}
		break;

	case DT_DOUBLE:
		{
			double* pdTemp;
			pdTemp = (double*)m_pData;
			m_dData = *pdTemp;
		}
		break;

	case DT_STR:
		{
			CString* pstrTemp;
			pstrTemp = (CString*)m_pData;
			m_strData = *pstrTemp;
		}
		break;
	}
}

void* AData::pGetData()
{
	switch( m_dt )
	{
		case DT_BOOL:	return &m_bData;
		case DT_INT:	return &m_nData;
		case DT_DOUBLE:	return &m_dData;
		case DT_STR:	return &m_strData;
	}

	return &m_bData;
}

void AData::CheckAndLog()
{
	switch( m_dt )
	{
		case DT_BOOL:
			{
				bool* pbData = (bool*)m_pData;
				if( *pbData != m_bData )
				{
					CString strLog;
					CString strBackup = m_bData ? "TRUE" : "FALSE";
					CString strOrg = *pbData ? "TRUE" : "FALSE";
					strLog.Format( "{%s}	[%s] -----> [%s]", m_strLogHead, strOrg, strBackup );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_INT:
			{
				int* piData = (int*)m_pData;
				if( *piData != m_nData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%d] -----> [%d]", m_strLogHead, *piData, m_nData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_DOUBLE:
			{
				double* pdData = (double*)m_pData;
				if( *pdData != m_dData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%.03f] -----> [%.03f]", m_strLogHead, *pdData, m_dData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_STR:
			{
				CString* pstrData = (CString*)m_pData;
				if( *pstrData != m_strData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%s] -----> [%s]", m_strLogHead, *pstrData, m_strData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;
	}
}

DataManager::DataManager()
{
	MappingDatas();
}

DataManager::~DataManager()
{

}

void DataManager::MappingDatas()
{
	m_vecData.clear();
	AData dataTemp;
	
	// INT
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_mode_run, DT_INT, "RUN_MODE" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_mode_device, DT_INT, "DEVICE MODE" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_bypass_mode, DT_INT, "BYPASS_MODE" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_electrostatic_check_gap, DT_INT, "ELEC CHECK GAP" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_alarm_delay_time, DT_INT, "ALARM DELAY TIME" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_network_wait_time, DT_INT, "NETWORK WAIT TIME" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_network_retry_cnt, DT_INT, "NETWORK RETRY CNT" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_pick_type, DT_INT, "PICK_TYPE" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_time_out, DT_INT, "TIME_OUT" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_robot_init_time, DT_INT, "ROBOT_INIT_TIME" );			m_vecData.push_back( dataTemp );//2012,1225
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nRunSpeed, DT_INT, "ROBOT_RUN_SPEED" );						m_vecData.push_back( dataTemp );//2012,1225
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nManualSpeed, DT_INT, "ROBOT_MANUAL_SPEED" );				m_vecData.push_back( dataTemp );//2012,1225
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_mode_xgem, DT_INT, "XGEM_Mode" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_mode_rcmd, DT_INT, "RCMD_Mode" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nRejectOffset, DT_INT, "Reject_Offset" );					m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_Label_Print_Error_Cnt, DT_INT, "LabelPrintErrorCnt" );	m_vecData.push_back( dataTemp );
	//2015.0211
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_Label_Print_Total_Error_Cnt, DT_INT, "LabelPrintTotalErrorCnt" );	m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_Label_vision_paper, DT_INT, "Label_vision_paper" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_Density_Device_Type, DT_INT, "Density_Device_Type" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_device_type, DT_INT, "DeviceType" );							m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nDensityFeederUse[0], DT_INT, "DensityFeederUse_1" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nDensityFeederUse[1], DT_INT, "DensityFeederUse_2" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nDensityFeederUse[2], DT_INT, "DensityFeederUse_3" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nDensityFeederUse[3], DT_INT, "DensityFeederUse_4" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nDensityFeederUse[4], DT_INT, "DensityFeederUse_5" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nDensityFeederUse[5], DT_INT, "DensityFeederUse_6" );			m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nBcrFeederUse[0], DT_INT, "BcrFeederUse_1" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nBcrFeederUse[1], DT_INT, "BcrFeederUse_2" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nBcrFeederUse[2], DT_INT, "BcrFeederUse_3" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nBcrFeederUse[3], DT_INT, "BcrFeederUse_4" );					m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nVisionUse[0], DT_INT, "VisionUse_1" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nVisionUse[1], DT_INT, "VisionUse_2" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nVisionUse[2], DT_INT, "VisionUse_3" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nVisionUse[3], DT_INT, "VisionUse_4" );			m_vecData.push_back( dataTemp );
	
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_leftbcr, DT_INT, "Left_bcr" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_rightbcr, DT_INT, "Right_bcr" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_outconv, DT_INT, "Out_Conv" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_ECServerUse, DT_INT, "ECServerUse" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_outtbox, DT_INT, "Out_TBox" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_overtime_density, DT_INT, "Over_Time_Density" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_str_capa1label, DT_STR, "Capa1_Label" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_str_capa2label, DT_STR, "Capa2_Label" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_str_capa3label, DT_STR, "Capa3_Label" );						m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_limit_bcr_time, DT_INT, "Limit_BCR_Time" );	m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_auto_vision_mdl, DT_INT, "Auto_Vision_Model" );	m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nTopVisionSkip, DT_INT, "Top_Vision_Skip" );	m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.nTopVisionSkipCnt, DT_INT, "Top_Vision_SkipCnt" );	m_vecData.push_back( dataTemp );
	
	//2016.0916
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_rework_ccs_mode, DT_INT, "Rework_CCS" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_vision_density_skip, DT_INT, "Vision_Density_Skip" );	m_vecData.push_back( dataTemp );

	// DOUBLE
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.md_electrostatic_value, DT_DOUBLE, "ELEC VALUE" );			m_vecData.push_back( dataTemp );


	// STRING
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mstr_device_name, DT_STR, "DEVICE NAME" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mstr_revipe_name, DT_STR, "RECIPE" );						m_vecData.push_back( dataTemp );

	/////20131115
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_distance, DT_INT, "Distance" );				m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_shutter_time, DT_INT, "Shutter_Time" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_contrast, DT_INT, "Contrast" );				m_vecData.push_back( dataTemp );
	
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_brightness, DT_INT, "Brightness" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.d_1d_width, DT_DOUBLE, "1D_Width" );			m_vecData.push_back( dataTemp );
	
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_illumination, DT_INT, "Illumination" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_image_rotate, DT_INT, "Image_Rotate" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_1d2d_select, DT_INT, "1d2d_Select" );			m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.n_1d2d_background, DT_INT, "1d2d_background" );	m_vecData.push_back( dataTemp );
	
	/////

}

// 디바이스네임은 업데이트 하지 않는다.
void DataManager::UpdateDatas()
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == &st_basic.mstr_device_name )
			continue;
		
		m_vecData[i].UpdateData();
	}
}

void DataManager::UpdateData( void* pData )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData[i].GetOrgDataPointer() == pData )
			m_vecData[i].UpdateData();
	}
}

void DataManager::BackupDatas()
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		m_vecData[i].BackupData();
	}
}

void DataManager::SetDataTemp( void* pData, bool bVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData[i].GetOrgDataPointer() == pData )
			m_vecData[i].SetData( bVal );
	}
}

void DataManager::SetDataTemp( void* pData, int iVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData[i].GetOrgDataPointer() == pData )
			m_vecData[i].SetData( iVal );
	}
}

void DataManager::SetDataTemp( void* pData, double dVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData[i].GetOrgDataPointer() == pData )
			m_vecData[i].SetData( dVal );
	}
}

void DataManager::SetDataTemp( void* pData, CString strVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData[i].GetOrgDataPointer() == pData )
			m_vecData[i].SetData( strVal );
	}
}

bool DataManager::GetBackupData( bool* pbOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pbOrg )
		{
			bool* pRtn;
			pRtn = (bool*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return false;
}

int DataManager::GetBackupData( int* piOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)piOrg )
		{
			int* pRtn;
			pRtn = (int*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return -1;
}

double DataManager::GetBackupData( double* pdOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pdOrg )
		{
			double* pRtn;
			pRtn = (double*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return -1;
}
	
CString DataManager::GetBackupData( CString* pstrOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pstrOrg )
		{
			CString* pRtn;
			pRtn = (CString*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return "";
}

void DataManager::CheckAndLogAll()
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		m_vecData.at(i).CheckAndLog();
	}
}

void DataManager::CheckAndLogByPointer( void* pData )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
		{
			m_vecData.at(i).CheckAndLog();
			return;
		}
	}
}