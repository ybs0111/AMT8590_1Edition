// Dialog_Data_Module.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Data_Module.h"
#include "SrcPart/PartFunction.h"
#include "SrcPart/APartTray.h"
#include "SrcPart/APartTestSite.h"
#include "SrcPart/APartRobot.h"
#include "SrcPart/APartHandler.h"
#include "Dialog_Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Module dialog

CDialog_Data_Module::CDialog_Data_Module(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Data_Module::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Data_Module)
	//}}AFX_DATA_INIT
	m_caption_main = "DATA (Module)"; 
}


void CDialog_Data_Module::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Data_Module)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Data_Module, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Data_Module)
	ON_BN_CLICKED(IDC_BTN_MODULE_SET, OnBtnModuleSet)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Module message handlers

void CDialog_Data_Module::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	COLORREF bk_old, tk_old;
	Grid->GetColor(col, row, &bk_old, &tk_old);
	if( bk_old != bk || tk_old != tk )
	{
		Grid->SetColor(col, row, bk, tk);
		Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	}

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Module::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
			Grid->SetTypeComboBox(&CellType, 0, m_strCombo);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0, 0, 0, m_nNumMax,0,".",",",0,0,1,0,1.001);
			break;
	}
	
	Grid->SetCellType(col,row,&CellType);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Module::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	char szBuf[1024];
	Grid->GetValue(col, row, szBuf);
	if( data == szBuf )
	{
		if (Grid != NULL)
		{	
			Grid = NULL;
			delete Grid;
		}
		return;
	}

	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

CString CDialog_Data_Module::GetGridData( UINT nID, int row, int col )
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	char szBuf[1024];
	Grid->GetValue(col, row, szBuf);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}

	return szBuf;
}


void CDialog_Data_Module::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Module::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

BOOL CDialog_Data_Module::OnInitDialog() 
{
	StandardDialog::OnInitDialog();

	m_selected = EDMS_NONE;
	m_nSelected_ModuleIdx = -1;
	m_strFirstS = EMS_NONE;
	
	OnInit_TestSite();
	OnInit_ModuleInfo();
	OnInitTree();

	OnInit_RobotPicker();

	OnDisplay_TestSite();


	OnDisplay_Robot();
	OnDisplay_ModuleInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Data_Module::OnInitTree()
{
//	int i, k, l, m;
//	int nOldPort = -1;
//	int nTmp = 0;
//	CString strTmp;
//	CString strInOut[2] = {_T("INPUT"),_T("OUTPUT")};
//
//	CBitmap bmp;
//	m_pImagelist = new CImageList;
//	m_pImagelist->Create(24, 24, ILC_COLOR32|ILC_MASK,0, -1);
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));//12
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6
///*	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HINEW));//0 %%0부터 시작한다 주의
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIFIND));//1
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HISAVE1));//2
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIDELETE));//3
//
//	//bmp.LoadBitmap(IDB_LIST);
//	//m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//4
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_READ));;//4
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIPRINT2));;//5
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HICLOSE));//6
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIOPEN));//7
//	bmp.LoadBitmap(IDB_SEARCH);
//	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//8
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_BOOK));//9
//
//	bmp.LoadBitmap(IDB_PREVIEW);
//	m_pImagelist->Add(&bmp,0xEFEFEF);bmp.DeleteObject();//10
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_CLEAR));//11
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIRFOLDER));//12
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HIFILE));//13
//	m_pImagelist->Add(AfxGetApp()->LoadIcon(IDI_HISEARCH));//14*/
//
//
//	m_treePartIoList.SetImageList(m_pImagelist, TVSIL_NORMAL);
//	m_treePartIoList.DeleteAllItems();
//
//	TV_INSERTSTRUCT tvstruct;
//	HTREEITEM item[3]={NULL,NULL,NULL};//아이템 핸들러 3개 초기화
//
//	strTmp.Format(_T("%s"), clsExcel.m_strPartName[clsExcel.m_nMotorSelectPartNum]);
//
//	tvstruct.hParent				= 0;
//	tvstruct.hInsertAfter			= TVI_LAST;
//	tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strTmp;
//	tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
//	tvstruct.item.iImage			= 0;
//	tvstruct.item.iSelectedImage	= 0;
//	item[0]							= m_treePartIoList.InsertItem(&tvstruct);
//
//	for(i=0; i<clsExcel.m_nPartCnt; i++)
//	{
//		if(i == clsExcel.m_nMotorSelectPartNum/*모터 화면에서 선택 Part*/)
//		{
//			for(k=0; k<2; k++) 
//			{
//				tvstruct.hParent				= item[0];
//				tvstruct.hInsertAfter			= TVI_LAST;
//				tvstruct.item.pszText			= (LPWSTR)(LPCTSTR)strInOut[k];
//				tvstruct.item.mask				= TVIF_TEXT | TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
//				tvstruct.item.iImage			= 0;
//				tvstruct.item.iSelectedImage	= 0;
//				item[1]							= m_treePartIoList.InsertItem(&tvstruct);
//
//				for(l=0; l<20; l++)
//				{
//					for(m=0; m<16; m++)
//					{
//						if(clsExcel.m_sPartInOutPutName[i+1][k][l][m] != "")
//						{
//							if(nOldPort != l)
//							{
//								nTmp++;
//								nOldPort = l;
//
//								if(k == 0)	{
//									strTmp.Format(_T("INPUT_PORT:%d"), nOldPort);
//								}
//								else	{
//									strTmp.Format(_T("OUTPUT_PORT:%d"), nOldPort);
//								}
//
//								tvstruct.hParent				=	item[1];
//								tvstruct.hInsertAfter			=	TVI_LAST;
//								tvstruct.item.pszText			=	(LPWSTR)(LPCTSTR) strTmp;
//								tvstruct.item.mask				=	TVIF_TEXT| TVIF_IMAGE |TVIF_SELECTEDIMAGE ;
//								tvstruct.item.iImage			=	2;
//								tvstruct.item.iSelectedImage	=	1;
//								item[2]							= m_treePartIoList.InsertItem(&tvstruct);
//
//								m_treePartIoList.EnsureVisible(item[2]);
//							
//							}
//						}
//					}
//				}
//				nTmp = 0;
//			}
//		}
//	}
}

void CDialog_Data_Module::OnInit_TestSite()
{
	TSpread* Grid;
	int row = 11;
	int col = 2;

	int iGrid = IDC_CUSTOM_DATA_MDL_TSITE;

	Grid = (TSpread*)GetDlgItem( iGrid );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	Grid -> SetColWidthInPixels(1, 40 );
	Grid -> SetColWidthInPixels(2, 320 );

	for( int ih=0; ih<row; ih++ )
	{
		Grid -> SetRowHeightInPixels(ih + 1, 20);
			
		for( int iw=0; iw<col; iw++ )
		{
			GridFont(iGrid, ih + 1, iw + 1, 15);
			GridColor( iGrid, ih + 1, iw + 1, WHITE_C, BLACK_C );
			GridControl(iGrid, STATIC, ih + 1, iw + 1, 0);
		}

		if( ih > 0 )
		{
			CString strNo;
			strNo.Format( "%d", ih );
			GridData( iGrid, ih + 1, 1, strNo );
//			GridData( iGrid, ih + 1, 3, strNo );
		}
		
	}

	GridMerge( iGrid, 1, 1, 1, 2 );	GridColor( iGrid, 1, 1, BLACK_C, WHITE_C ); GridData( iGrid, 1, 1, "LABEL SITE" );

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Module::OnInit_RobotPicker()
{
	TSpread* Grid;
	int row = 15;
	int col = 1;

	int iGrid = IDC_CUSTOM_DATA_MDL_ROBOT;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_DATA_MDL_ROBOT );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for( int iw=0; iw<row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 18);
		Grid -> SetColWidthInPixels(1, 200 );
			
		GridFont( IDC_CUSTOM_DATA_MDL_ROBOT, iw + 1, 1, 15);
		GridControl(IDC_CUSTOM_DATA_MDL_ROBOT, STATIC, iw + 1, 1, 0);
	}

	GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 1, 1, BLACK, WHITE_C );
	GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 1, 1, "DENSITY" );
	GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 4, 1, BLACK, WHITE_C );
	GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 4, 1, "BARCODE#1" );
	GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 7, 1, BLACK, WHITE_C );
	GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 7, 1, "BARCODE#2" );
	GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 10, 1, BLACK, WHITE_C );
	GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 10, 1, "TOP VISION" );
	GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 13, 1, BLACK, WHITE_C );
	GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 13, 1, "UNLOAD" );
	
	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Module::OnInit_ModuleInfo()
{
	TSpread* Grid;
	int row = 8;
	int col = 2;

	int iGrid = IDC_CUSTOM_DATA_MDL_MODULE_INFO;
	Grid = (TSpread*)GetDlgItem( iGrid );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	Grid -> SetColWidthInPixels(1, 120 );
	Grid -> SetColWidthInPixels(2, 250 );
	for( int iw=0; iw<row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 21);
			
		GridFont(iGrid, iw + 1, 1, 15);
		GridFont(iGrid, iw + 1, 2, 15);
		GridControl(iGrid, STATIC, iw + 1, 1, 0);

		GridColor( iGrid, iw + 1, 1, ORANGE_C, BLACK_C );
	}

	GridMerge( iGrid, 1, 1, 1, 2 );
	GridColor(iGrid, 1, 1, BLACK, WHITE_C );
	GridData(iGrid, 1, 1, "BUFFER INFO" );

	GridData( iGrid, 2, 1, "PRODUCT ID" );
	GridData( iGrid, 3, 1, "LOT NO." );
	GridData( iGrid, 4, 1, "BUFFER NO" );
	GridData( iGrid, 5, 1, "SERIAL NO." );
	GridData( iGrid, 6, 1, "Work CNT " );
	GridData( iGrid, 7, 1, "SKIP" );
	GridData( iGrid, 8, 1, "VAR" );

	m_nNumMax = 9999;
	m_strCombo = "NONE";
	m_strCombo += "\tYES\tNO";
	m_strCombo += "\t0\t1";
	GridControl(iGrid, EDIT, 2, 2, 0);
	GridControl(iGrid, EDIT, 3, 2, 0);
	GridControl(iGrid, EDIT, 4, 2, 0);
	GridControl(iGrid, EDIT, 5, 2, 0);
	GridControl(iGrid, NUMBER, 6, 2, 0);
	GridControl(iGrid, COMBO, 7, 2, 0);
	GridControl(iGrid, EDIT, 8, 2, 0);
	
	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Data_Module::OnDisplay_TestSite()
{
	int i = 0; //, iIdx1 = -1, iIdx2 = -1, iIdx3 = -1, iIdx4 = -1, iIdx5 = -1;
	CString sTmp="";
	for ( i = 0; i < 2; i++)
	{
		if( st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].n_exist[i] == 1 )
		{
			sTmp.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INPUT_INDEX].c_serial[i]);
			GridColor( IDC_CUSTOM_DATA_MDL_TSITE, i % 10 + 2, i / 10 * 2 + 2, SKY_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_TSITE, i %10 + 2, i / 10 * 2 + 2, sTmp );
		}

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].n_exist[i] == 1 )
		{
			sTmp.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_DENSITY].c_serial[i]);
			GridColor( IDC_CUSTOM_DATA_MDL_TSITE, (i+2) % 10 + 2, (i+2) / 10 * 2 + 2, SKY_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_TSITE, (i+2) %10 + 2, (i+2) / 10 * 2 + 2, sTmp );
		}

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].n_exist[i] == 1 )
		{
			sTmp.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR1].c_serial[i]);
			GridColor( IDC_CUSTOM_DATA_MDL_TSITE, (i+4) % 10 + 2, (i+4) / 10 * 2 + 2, SKY_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_TSITE, (i+4) %10 + 2, (i+4) / 10 * 2 + 2, sTmp );
		}

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].n_exist[i] == 1 )
		{
			sTmp.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_BCR2].c_serial[i]);
			GridColor( IDC_CUSTOM_DATA_MDL_TSITE, (i+6) % 10 + 2, (i+6) / 10 * 2 + 2, SKY_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_TSITE, (i+6) %10 + 2, (i+6) / 10 * 2 + 2, sTmp );
		}

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[i] == 1 )
		{
			sTmp.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_serial[i]);
			GridColor( IDC_CUSTOM_DATA_MDL_TSITE, (i+8) % 10 + 2, (i+8) / 10 * 2 + 2, SKY_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_TSITE, (i+8) %10 + 2, (i+8) / 10 * 2 + 2, sTmp );
		}

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].n_exist[i] == 1 )
		{
			sTmp.Format("%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_ULD].c_serial[i]);
			GridColor( IDC_CUSTOM_DATA_MDL_TSITE, (i+10) % 10 + 2, (i+10) / 10 * 2 + 2, SKY_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_TSITE, (i+10) %10 + 2, (i+10) / 10 * 2 + 2, sTmp );
		}

	}

}


void CDialog_Data_Module::OnDisplay_Robot()
{
	CString strTmp;
	for( int i=0; i<PICKER_CNT; i++ )
	{
		if( st_den_rbt[i].bPick == true)
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 2 + i, 1, SKY_C, BLACK_C );
			strTmp.Format( "%s", st_den_rbt[i].st_label.c_mdl_id);
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 2 + i, 1, strTmp );
		}
		else
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 2 + i, 1, WHITE_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 2 + i, 1, "" );
		}
		
		if( st_bcr_rbt[0].bPick[i] == true)
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 5 + i, 1, SKY_C, BLACK_C );
			strTmp.Format( "%s", st_bcr_rbt[0].st_bcr[i].c_lot_id );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 5 + i, 1, strTmp );
		}
		else
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 5 + i, 1, WHITE_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 5 + i, 1, "" );
		}

		if( st_bcr_rbt[1].bPick[i] == true)
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 8 + i, 1, SKY_C, BLACK_C );
			strTmp.Format( "%s", st_bcr_rbt[1].st_bcr[i].c_lot_id );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 8 + i, 1, strTmp );
		}
		else
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 8 + i, 1, WHITE_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 8 + i, 1, "" );
		}

		if( st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].n_exist[i] == 1)
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 11 + i, 1, SKY_C, BLACK_C );
			strTmp.Format( "%s", st_idbuff_info[SHIFT_IDBUFF_INDEX_TOP].c_serial );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 11 + i, 1, strTmp );
		}
		else
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 11 + i, 1, WHITE_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 11 + i, 1, "" );
		}

		if( st_uld_rbt.bPick[i] == true)
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 14 + i, 1, SKY_C, BLACK_C );
			strTmp.Format( "%s", st_uld_rbt.st_btm[i].c_model_id );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 14 + i, 1, strTmp );
		}
		else
		{
			GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 14 + i, 1, WHITE_C, BLACK_C );
			GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 14 + i, 1, "" );
		}

	}

//	if( m_selected == EDMS_ROBOT && m_nSelected_ModuleIdx > -1 )
//	{
//		GridColor( IDC_CUSTOM_DATA_MDL_ROBOT, 2 + m_nSelected_ModuleIdx, 1, ORANGE_L, BLACK_C );
//		GridData( IDC_CUSTOM_DATA_MDL_ROBOT, 2 + m_nSelected_ModuleIdx, 1, "SELECTED" );
//	}

}

void CDialog_Data_Module::OnDisplay_ModuleInfo()
{

	CString strTmp="";
	if( (int)m_selected > -1 && m_selected < 6 && m_nSelected_ModuleIdx > -1 && m_nSelected_ModuleIdx < 2)
	{
		strTmp.Format("%s",st_idbuff_info[m_selected].c_product_id[m_nSelected_ModuleIdx]);
		GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 2, 2, strTmp );
		strTmp.Format("%s",st_idbuff_info[m_selected].c_lot_id[m_nSelected_ModuleIdx]);
		GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 3, 2, strTmp );
		strTmp.Format("%d",st_idbuff_info[m_selected].n_idbuff_num[m_nSelected_ModuleIdx]);
		GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 4, 2, strTmp );
		strTmp.Format("%s",st_idbuff_info[m_selected].c_serial[m_nSelected_ModuleIdx]);
		GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 5, 2, strTmp );
		strTmp.Format("%d",st_idbuff_info[m_selected].n_count[m_nSelected_ModuleIdx]);
		GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 6, 2,  strTmp );
		if( st_idbuff_info[m_selected].n_skip_flag[m_nSelected_ModuleIdx] > 0)	GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 7, 2, "YES" );
		else			GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 7, 2, "NO" );

		strTmp.Format("%s",st_idbuff_info[m_selected].c_var[m_nSelected_ModuleIdx]);
		GridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 8, 2, strTmp );		
	}
}

void CDialog_Data_Module::OnCellClick( WPARAM wParam, LPARAM lParam )
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	ENUM_DATA_MODULE_SELECTE edms_old = m_selected;
	int nSelected_Old = m_nSelected_ModuleIdx;


	switch( wParam )
	{
	case IDC_CUSTOM_DATA_MDL_TSITE:
		{
			if( lpcc->Col % 2 != 0 )	break;
			if( lpcc->Row <= 1 )		break;

			if( lpcc->Col / 2 == 0)		 m_selected = EDMS_DENSITY;
			else if( lpcc->Col / 2 == 1) m_selected = EDMS_BACR1;
			else if( lpcc->Col / 2 == 2) m_selected = EDMS_BACR2;
			else if( lpcc->Col / 2 == 3) m_selected = EDMS_TOP;
			else if( lpcc->Col / 2 == 4) m_selected = EDMS_UNLOAD;
			else						 return;
			
			m_nSelected_ModuleIdx = lpcc->Col % 2;

			//m_strFirstS = Func.m_pSite[m_selected-EDMS_DENSITY]->GetModule(m_nSelected_ModuleIdx).GetModuleState();
		}
		break;

		
	case IDC_CUSTOM_DATA_MDL_ROBOT:
		{
			if( lpcc->Row <= 1 )		break;

			if( lpcc->Row >= 2 && lpcc->Row <= 3)
			{
				m_selected = EDMS_DENRBT;
				m_nSelected_ModuleIdx = lpcc->Row % 2;
			}
			else if( lpcc->Row >= 5 && lpcc->Row <= 6)
			{
				m_selected = EDMS_BCR1RBT;
				m_nSelected_ModuleIdx = lpcc->Row % 5;
			}
			else if( lpcc->Row >= 8 && lpcc->Row <= 9)
			{
				m_selected = EDMS_BCR2RBT;
				m_nSelected_ModuleIdx = lpcc->Row % 8;
			}
			else if( lpcc->Row >= 11 && lpcc->Row <= 12)
			{
				m_selected = EDMS_TOPRBT;
				m_nSelected_ModuleIdx = lpcc->Row % 11;
			}
			else if( lpcc->Row >= 14 && lpcc->Row <= 15)
			{
				m_selected = EDMS_ULDRBT;
				m_nSelected_ModuleIdx = lpcc->Row % 14;
			}
			
		}
		break;

	}


	if( edms_old == m_selected && nSelected_Old == m_nSelected_ModuleIdx )
		return;

	OnDisplay_TestSite();
	OnDisplay_Robot();
	OnDisplay_ModuleInfo();
}

void CDialog_Data_Module::OnBtnModuleSet() 
{
	if( (int)m_selected > -1 && m_selected < 6 && m_nSelected_ModuleIdx > -1 && m_nSelected_ModuleIdx < 2)
		return;

	CString strTmp; 
	int nPos = (int) m_selected;

	sprintf(st_idbuff_info[nPos].c_product_id[m_nSelected_ModuleIdx], "%s", GetGridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 2, 2 ) );
	sprintf(st_idbuff_info[nPos].c_lot_id[m_nSelected_ModuleIdx], "%s", GetGridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 4, 2 ) );
	st_idbuff_info[nPos].n_idbuff_num[m_nSelected_ModuleIdx] = atoi( GetGridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 5, 2 ) );
	st_idbuff_info[nPos].n_count[m_nSelected_ModuleIdx] = atoi( GetGridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 6, 2 ) );
	if( GetGridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 7, 2 ) == "YES" )	st_idbuff_info[nPos].n_skip_flag[m_nSelected_ModuleIdx] = 1;
	else																st_idbuff_info[nPos].n_skip_flag[m_nSelected_ModuleIdx] = 0;
	sprintf(st_idbuff_info[nPos].c_var[m_nSelected_ModuleIdx], "%s", GetGridData( IDC_CUSTOM_DATA_MDL_MODULE_INFO, 8, 2 ) );	

	OnDisplay_TestSite();
	OnDisplay_Robot();
	OnDisplay_ModuleInfo();
}

void CDialog_Data_Module::OnDestroy() 
{
	StandardDialog::OnDestroy();
	
	if( st_handler.cwnd_main == NULL )
		return;

	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDR_PICKER);
	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDS_TESTSITE);
	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_0 );
	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_1 );
	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_2 );//ybs
	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY);
	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_REJ_FLOOR );

	for( int i=0; i<MAX_TRAY; i++ )
	{
		st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_INIT, i );
	}

}
