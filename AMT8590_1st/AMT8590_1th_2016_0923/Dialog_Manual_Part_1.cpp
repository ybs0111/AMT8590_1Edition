// Dialog_Manual_Part_1.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_Part_1.h"
#include "Run_Motors.h"

#include "Cmmsdk.h"//2014.1108 ysj

#include "ZebraPrint.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Part_1 dialog
////2014.1108 ysj
#define TM_DENSITY_LABEL_OUT 100
#define TM_DENSITY_ROBOT_MOVE 200
#define TM_DISTRIBUTION_ROBOT_MOVE1 300
#define TM_DISTRIBUTION_ROBOT_MOVE2 400
#define TM_DISTRIBUTION_LABEL_OUT 500
////

CDialog_Manual_Part_1::CDialog_Manual_Part_1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_Part_1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Manual_Part_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Func.mp_manual_1 = NULL;
}


void CDialog_Manual_Part_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_Part_1)
	DDX_Control(pDX, IDC_BUTTON_GO, m_btn_go);
	DDX_Control(pDX, IDC_CHK_PICK, m_chk_pick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_Part_1, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_Part_1)
	ON_BN_CLICKED(IDC_BTN_PICK_DENSITY_LABEL, OnBtnPickDensityLabel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DENSITY_POS_SAVE, OnDensityPosSave)
	ON_BN_CLICKED(IDC_BUTTON_ZDOWN, OnButtonZdown)
	ON_BN_CLICKED(IDC_BTN_PLACE_DENSITY_LABEL, OnBtnPlaceDensityLabel)
	ON_BN_CLICKED(IDC_BTN_BCR_OUT_LABEL, OnBtnBcrOutLabel)
	ON_BN_CLICKED(IDC_BTN_BCR_OUT_LABEL2, OnBtnBcrOutLabel2)
	ON_BN_CLICKED(IDC_BTN_BCR_OUT_LABEL3, OnBtnBcrOutLabel3)
	ON_BN_CLICKED(IDC_BTN_BCR_OUT_LABEL4, OnBtnBcrOutLabel4)
	ON_BN_CLICKED(IDC_BTN_BCR_VISION_LABEL, OnBtnBcrVisionLabel)
	ON_BN_CLICKED(IDC_BTN_BCR_VISION_LABEL2, OnBtnBcrVisionLabel2)
	ON_BN_CLICKED(IDC_BTN_BCR_VISION_LABEL3, OnBtnBcrVisionLabel3)
	ON_BN_CLICKED(IDC_BTN_BCR_VISION_LABEL4, OnBtnBcrVisionLabel4)
	ON_BN_CLICKED(IDC_BTN_PICK_BCR_LABEL1, OnBtnPickBcrLabel1)
	ON_BN_CLICKED(IDC_BTN_PICK_BCR_LABEL2, OnBtnPickBcrLabel2)
	ON_BN_CLICKED(IDC_BTN_PLACE_BCR_LABEL12, OnBtnPlaceBcrLabel12)
	ON_BN_CLICKED(IDC_BTN_PLACE_BCR_LABEL34, OnBtnPlaceBcrLabel34)
	ON_BN_CLICKED(IDC_BTN_REJ_BCR_LABEL, OnBtnRejBcrLabel)
	ON_BN_CLICKED(IDC_BTN_REJ_BCR_LABEL2, OnBtnRejBcrLabel2)
	ON_BN_CLICKED(IDC_BTN_REJ_BCR_LABEL3, OnBtnRejBcrLabel3)
	ON_BN_CLICKED(IDC_BTN_REJ_BCR_LABEL4, OnBtnRejBcrLabel4)
	ON_BN_CLICKED(IDC_BTN_TOP_VISION, OnBtnTopVision)
	ON_BN_CLICKED(IDC_BTN_TOP_VISION2, OnBtnTopVision2)
	ON_BN_CLICKED(IDC_BTN_BTM_VISION, OnBtnBtmVision)
	ON_BN_CLICKED(IDC_BTN_PICK_UNLOAD, OnBtnPickUnload)
	ON_BN_CLICKED(IDC_BTN_BTM_VISION2, OnBtnBtmVision2)
	ON_BN_CLICKED(IDC_BTN_BTM_VISION3, OnBtnBtmVision3)
	ON_BN_CLICKED(IDC_BTN_BTM_VISION4, OnBtnBtmVision4)
	ON_BN_CLICKED(IDC_BTN_UNLOAD_STACKER, OnBtnUnloadStacker)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CONV_ULD_PLACE, OnBtnConvUldPlace)
	ON_BN_CLICKED(IDC_BTN_REJECT_ULD_PLACE, OnBtnRejectUldPlace)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_1_EXIT, OnBtn1Exit)
	ON_BN_CLICKED(IDC_BTN_DENSITY_LABEL_OUT, OnBtnDensityLabelOut)
	ON_BN_CLICKED(IDC_BTN_DENSITY_LABEL_OUT4, OnBtnDensityLabelOut4)
	ON_BN_CLICKED(IDC_BTN_DENSITY_LABEL_OUT2, OnBtnDensityLabelOut2)
	ON_BN_CLICKED(IDC_BTN_DENSITY_LABEL_OUT5, OnBtnDensityLabelOut5)
	ON_BN_CLICKED(IDC_BTN_DENSITY_LABEL_OUT3, OnBtnDensityLabelOut3)
	ON_BN_CLICKED(IDC_BTN_DENSITY_LABEL_OUT6, OnBtnDensityLabelOut6)
	ON_BN_CLICKED(IDC_BTN_DENSITY_ATTACH_ROBOT_1, OnBtnDensityAttachRobot1)
	ON_BN_CLICKED(IDC_BTN_DENSITY_ATTACH_ROBOT_2, OnBtnDensityAttachRobot2)
	ON_BN_CLICKED(IDC_BTN_DENSITY_ATTACH_ROBOT_3, OnBtnDensityAttachRobot3)
	ON_BN_CLICKED(IDC_BTN_DISTRIBUTION_ATTACH_ROBOT_1, OnBtnDistributionAttachRobot1)
	ON_BN_CLICKED(IDC_BTN_DISTRIBUTION_ATTACH_ROBOT_2, OnBtnDistributionAttachRobot2)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_MOTOR_COMPLETION, OnMotorCompletion)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Part_1 message handlers

void CDialog_Manual_Part_1::OnPrintFormat(int nPort)
{
	CString strFotmat_1="^XA^LH200,047^MMT^PON^FS^CI28^CW2,E:KR.TTF^FS^FO5,95^A2,50,50^FR^FDSAMSUNG SSD 840 EVO mSATA^FS^FO5,160^A2,50,50^FR^FD512GB^FS^FO5,220^A2,50,50^FR^FDMade in Korea^FS";
	
	CString strFotmat_2="^FO900,95^GB685,100,100^FS^FO945,95^BY3,3,100^FR^BC,,N^FDS1AXNEAD908028^FS^FO900,215^A0,50,70^FR^FDS/N: 1AXNEAD908028^FS^FO70,470^A2I,48,48^FR^FDModel Code: MZ-7PD512BW^FS^FO1195,470^A2I,48,48^FR^FDModel: MZ-7PD512^FS";
	
	CString strFotmat_3="^FO5,557^GB780,275,180^FS^FO895,557^GB780,275,180^FS^FO1575,590^FPV^A2I,75,75^FR^FDUPC^FS^FO950,630^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276001180^FS^FO680,590^FPV^A2I,75,75^FR^FDEAN^FS";
	
	CString strFotmat_4="^FO58,630^BY6,3,180^FR^BEI,180,Y,N^FD8806085216716^FS^PQ1^XZ";
	
	

	m_strPrintFormat = strFotmat_1 + strFotmat_2 + strFotmat_3 + strFotmat_4;
	
	clsZebra.OnPrintOutput(2,nPort,0,0, m_strPrintFormat);

}

BOOL CDialog_Manual_Part_1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rectArea;
	
	mp_manual_font = NULL;
	mp_manual_font = new CFont;
	mp_manual_font->CreateFont(16,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	GetClientRect(rectArea);
	MoveWindow(mcp_coord.x, mcp_coord.y, rectArea.Width(), rectArea.Height(), TRUE);
	
	OnInitIO();

	m_nRobot = -1;
	
	st_handler.cwnd_motorAxis = this;


	SetTimer(TMR_IO_CHECK, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Manual_Part_1::OnDestroy() 
{
	CDialog::OnDestroy();
	

	st_handler.cwnd_motorAxis = NULL;
	if(mp_manual_font != NULL)
	{
		delete mp_manual_font;
		mp_manual_font = NULL;
	}
	
}

void CDialog_Manual_Part_1::OnInitIO()//2015,0112
{
	TSpread *Grid;
	CString str_pos;
	int row = 39;
	int col = 4;
	int i, j;
	int nRowPos;
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, TRUE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			switch(i)
			{
				case 0: case 1: case 2: case 3: case 4: 
				case 5: case 6: case 7: case 8: case 9:
				case 10: case 11: case 12:
					Grid -> SetRowHeightInPixels(i + 1, 35);
					break;

				case 13:
					Grid -> SetRowHeightInPixels(i + 1, 5);
					break;		

				case 14: case 15: case 16: case 17:
					Grid -> SetRowHeightInPixels(i + 1, 35);
					break;

				case 18:
					Grid -> SetRowHeightInPixels(i + 1, 5);
					break;
				
				case 19: case 20: case 21: case 22: case 23: case 24: 
				case 25: case 26: case 27: case 28: case 29: 
				case 30: case 31: case 32: case 33: case 34:
					Grid -> SetRowHeightInPixels(i + 1, 35);
					break;

				case 35:
					Grid -> SetRowHeightInPixels(i + 1, 5);
					break;
				
				case 36: case 37: case 38:  case 39:
					Grid -> SetRowHeightInPixels(i + 1, 35);
					break;
			}
			
			if ((j % 2) == 0)
			{
				Grid -> SetColWidthInPixels(j + 1, 146);//160);
				GridColor(IDC_CUSTOM_IO, i + 1, j + 1, GREEN_D, BLACK_C);
				
				GridControl(IDC_CUSTOM_IO, STATIC, i + 1, j + 1, 0);
				GridFont(IDC_CUSTOM_IO, i + 1, j + 1, 12);
			}
			else
			{
				Grid -> SetColWidthInPixels(j + 1, 80);//65);
				GridColor(IDC_CUSTOM_IO, i + 1, j + 1, YELLOW_D, BLACK_C);
				
				GridControl(IDC_CUSTOM_IO, STATIC, i + 1, j + 1, 0);
				GridFont(IDC_CUSTOM_IO, i + 1, j + 1, 12);
			}
		}
	}

	m_nInCount = m_nOutCount = 0;

	////density 1, 2
	nRowPos = 1;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density 1 Start");//F1");
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "1 Ready chk");//Off");
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density 2 Start");//F2");
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "2 Ready chk");//Off");

	m_nOut[0][m_nOutCount]			= st_io.o_density_lable1_start;//o_density_f1_plate_btm_blow_onoff;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;

	m_nIn[0][m_nInCount]			= st_io.i_density_label1_ready_chk;//i_density_f1_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;

	m_nOut[1][m_nOutCount]			= st_io.o_density_lable2_start;//o_density_f2_plate_btm_blow_onoff;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	m_nIn[1][m_nInCount]			= st_io.i_density_label2_ready_chk;//i_density_f2_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 2;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density 1 Pick On");
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "1 Complte chk");//Off");	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density 2 Pick On");
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "2 Complte chk");//Off");

	m_nOut[0][m_nOutCount]			= st_io.o_density_lable1_pickon;//o_density_f1_plate_btm_blow_onoff;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label1_complete_chk;//i_density_f1_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_lable2_pickon;//o_density_f2_plate_btm_blow_onoff;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label2_complete_chk;//i_density_f2_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 3;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "density 1 Error");//F3");
// 	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "density 2 Error");//F4");
// 	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label1_error_chk;//i_density_f3_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label1_error_chk;//i_density_f4_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;


	////density 3, 4
	nRowPos = 4;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density 3 Start");//F1");
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "3 Ready chk");//Off");
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density 4 Start");//F2");
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "4 Ready chk");//Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_density_lable3_start;//o_density_f1_plate_btm_blow_onoff;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label3_ready_chk;//i_density_f1_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_lable4_start;//o_density_f2_plate_btm_blow_onoff;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label4_ready_chk;//i_density_f2_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;
	
	nRowPos = 5;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density 3 Pick On");
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "3 Complte chk");//Off");	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density 4 Pick On");
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "4 Complte chk");//Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_density_lable3_pickon;//o_density_f1_plate_btm_blow_onoff;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label3_complete_chk;//i_density_f1_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_lable4_pickon;//o_density_f2_plate_btm_blow_onoff;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label4_complete_chk;//i_density_f2_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;
	
	nRowPos = 6;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "density 3 Error");//F3");
// 	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "density 4 Error");//F4");
// 	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label3_error_chk;//i_density_f3_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label4_error_chk;//i_density_f4_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;


	////density 5, 6
	nRowPos = 7;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density 5 Start");//F1");
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "5 Ready chk");//Off");
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density 6 Start");//F2");
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "6 Ready chk");//Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_density_lable5_start;//o_density_f1_plate_btm_blow_onoff;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label5_ready_chk;//i_density_f1_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_lable6_start;//o_density_f2_plate_btm_blow_onoff;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label6_ready_chk;//i_density_f2_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;
	
	nRowPos = 8;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density 5 Pick On");
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "5 Complte chk");//Off");	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density 6 Pick On");
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "6 Complte chk");//Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_density_lable5_pickon;//o_density_f1_plate_btm_blow_onoff;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label5_complete_chk;//i_density_f1_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_lable6_pickon;//o_density_f2_plate_btm_blow_onoff;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label6_complete_chk;//i_density_f2_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;
	
	nRowPos = 9;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "density 5 Error");//F3");
// 	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "density 6 Error");//F4");
// 	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nIn[0][m_nInCount]			= st_io.i_density_label5_error_chk;//i_density_f3_plate_vacuun2_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_label6_error_chk;//i_density_f4_plate_vacuun2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

 
	nRowPos = 10;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density rbt Left V1");
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density rbt Left V2");
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");

	m_nOut[0][m_nOutCount]			= st_io.o_density_rbt_left_vacuum1;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_rbt_left_vacuum1_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_rbt_left_vacuum2;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_rbt_left_vacuum2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 11;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density rbt Left b1");	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density rbt Left b2");

	m_nOut[0][m_nOutCount]			= st_io.o_density_rbt_left_blow1;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_rbt_left_blow2;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 12;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density rbt Right V1");
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density rbt Right V2");
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_density_rbt_right_vacuum1;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_density_rbt_right_vacuum1_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_density_rbt_right_vacuum2;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_density_rbt_right_vacuum2_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;
	
	nRowPos = 13;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "density rbt Right b1");
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "density rbt Right b2");
	
	m_nOut[0][m_nOutCount]			= st_io.o_density_rbt_right_blow1;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	m_nOut[1][m_nOutCount]			= st_io.o_density_rbt_right_blow2;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 14;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 1, 4);
	GridColor(IDC_CUSTOM_IO, nRowPos, 1, WHITE_C, BLACK_C);

	nRowPos = 15;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR Plate1 (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR Plate2 (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_p1_plate_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_p1_plate_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_p2_plate_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_p2_plate_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;
	
	nRowPos = 16;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR Plate1 (Eject)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR Plate2 (Eject)");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_p1_eject_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_p2_eject_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 17;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR Plate3 (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR Plate4 (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_distribute_p3_plate_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_p3_plate_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_p4_plate_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_p4_plate_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 18;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR Plate3 (Eject)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR Plate4 (Eject)");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_p3_eject_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_p4_eject_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 19;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 1, 4);
	GridColor(IDC_CUSTOM_IO, nRowPos, 1, WHITE_C, BLACK_C);

	nRowPos = 20;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT1 LEFT TOP (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT1 RIGHT TOP (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt1_left_top_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt1_left_top_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt1_right_top_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt1_right_top_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 21;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT1 LEFT TOP (Eject)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT1 RIGHT TOP (Eject)");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt1_left_top_eject;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt1_right_top_eject;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 22;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT2 LEFT TOP (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT2 RIGHT TOP (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt2_left_top_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt2_left_top_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt2_right_top_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt2_right_top_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 23;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT2 LEFT TOP (Eject)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT2 RIGHT TOP (Eject)");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt2_left_top_eject;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt2_right_top_eject;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 24;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT1 LEFT BTM (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT1 RIGHT BTM (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt1_left_btm_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt1_left_btm_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt1_right_btm_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt1_right_btm_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 25;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT1 LEFT BTM (Eject)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT1 RIGHT BTM (Eject)");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt1_left_btm_eject;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt1_right_btm_eject;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 26;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT2 LEFT BTM (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 2, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT2 RIGHT BTM (Vacuum)");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 4, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt2_left_btm_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt2_left_btm_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt2_right_btm_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt2_right_btm_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 27;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT2 LEFT BTM (Eject)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT2 RIGHT BTM (Eject)");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt2_left_btm_eject;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt2_right_btm_eject;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;

	nRowPos = 28;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT1 LEFT Cylinder TOP");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Fwd");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 3, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT1 RIGHT Cylinder TOP");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Fwd");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt1_left_top_vacuum_fwdback;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt1_left_top_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt1_right_top_vacuum_fwdback;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt1_right_top_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 29;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Bwd");

	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Bwd");

	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt1_left_top_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;

	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt1_right_top_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 30;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT2 LEFT Cylinder TOP");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 3, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT2 RIGHT Cylinder TOP");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt2_left_top_vacuum_fwdback;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt2_left_top_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt2_right_top_vacuum_fwdback;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt2_right_top_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 31;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Bwd");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Bwd");
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt2_left_top_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt2_right_top_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 32;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT1 LEFT Cylinder BTM");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Fwd");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 3, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT1 RIGHT Cylinder BTM");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Fwd");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt1_left_btm_vacuum_fwdback;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt1_left_btm_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt1_right_btm_vacuum_fwdback;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt1_right_btm_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 33;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Bwd");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Bwd");
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt1_left_btm_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt1_right_btm_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 34;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "BCR RBT2 LEFT Cylinder BTM");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Fwd");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 3, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "BCR RBT2 RIGHT Cylinder BTM");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Fwd");

	m_nOut[0][m_nOutCount]			= st_io.o_distribute_rbt2_left_btm_vacuum_fwdback;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_distribute_rbt2_right_btm_vacuum_fwdback;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt2_right_btm_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 35;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Bwd");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Bwd");
	
	m_nIn[0][m_nInCount]			= st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_distribute_rbt2_right_btm_vacuum_bwd_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 36;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 1, 4);
	GridColor(IDC_CUSTOM_IO, nRowPos, 1, WHITE_C, BLACK_C);

	nRowPos = 37;
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "UNLOAD RBT V1(Vacuum)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Off");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "UNLOAD RBT V2(Vacuum)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Off");
	
	m_nOut[0][m_nOutCount]			= st_io.o_uld_rbt_left_vacuum;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_uld_rbt_left_vacuum_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_uld_rbt_right_vacuum;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_uld_rbt_right_vacuum_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 38;
	GridMerge(IDC_CUSTOM_IO, nRowPos, 1, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 1, "UNLOAD RBT P1(Pickerup/dn)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Up");
	
	GridMerge(IDC_CUSTOM_IO, nRowPos, 3, 2, 1);
	GridData(IDC_CUSTOM_IO, nRowPos, 3, "UNLOAD RBT P2(Pickerup/dn)");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Up");

	m_nOut[0][m_nOutCount]			= st_io.o_uld_rbt_left_updown;
	m_nOutPos[0][m_nOutCount][0]	= nRowPos;
	m_nOutPos[0][m_nOutCount][1]	= 1;
	
	m_nIn[0][m_nInCount]			= st_io.i_uld_rbt_left_up_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nOut[1][m_nOutCount]			= st_io.o_uld_rbt_right_updown;
	m_nOutPos[1][m_nOutCount][0]	= nRowPos;
	m_nOutPos[1][m_nOutCount][1]	= 3;
	m_nOutCount++;
	
	m_nIn[1][m_nInCount]			= st_io.i_uld_rbt_right_up_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	nRowPos = 39;
	GridData(IDC_CUSTOM_IO, nRowPos, 2, "Dn");
	
	GridData(IDC_CUSTOM_IO, nRowPos, 4, "Dn");
	
	m_nIn[0][m_nInCount]			= st_io.i_uld_rbt_left_down_chk;
	m_nInPos[0][m_nInCount][0]		= nRowPos;
	m_nInPos[0][m_nInCount][1]		= 2;
	
	m_nIn[1][m_nInCount]			= st_io.i_uld_rbt_right_down_chk;
	m_nInPos[1][m_nInCount][0]		= nRowPos;
	m_nInPos[1][m_nInCount][1]		= 4;
	m_nInCount++;

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Manual_Part_1::OnManualPart1_IO_Check()
{
	int i;

	for (i=0; i<m_nInCount; i++)
	{
		if (FAS_IO.get_in_bit(m_nIn[0][i], IO_ON) == IO_ON)
		{
			GridColor(IDC_CUSTOM_IO, m_nInPos[0][i][0], m_nInPos[0][i][1], GREEN_C, BLACK_C);
		}
		else
		{
			GridColor(IDC_CUSTOM_IO, m_nInPos[0][i][0], m_nInPos[0][i][1], GREEN_D, BLACK_C);
		}

		if (FAS_IO.get_in_bit(m_nIn[1][i], IO_ON) == IO_ON)
		{
			GridColor(IDC_CUSTOM_IO, m_nInPos[1][i][0], m_nInPos[1][i][1], GREEN_C, BLACK_C);
		}
		else
		{
			GridColor(IDC_CUSTOM_IO, m_nInPos[1][i][0], m_nInPos[1][i][1], GREEN_D, BLACK_C);
		}
	}

	for (i=0; i<m_nOutCount; i++)
	{
		if (FAS_IO.get_out_bit(m_nOut[0][i], IO_ON) == IO_ON)
		{
			GridColor(IDC_CUSTOM_IO, m_nOutPos[0][i][0], m_nOutPos[0][i][1], YELLOW_L, BLACK_C);
		}
		else
		{
			GridColor(IDC_CUSTOM_IO, m_nOutPos[0][i][0], m_nOutPos[0][i][1], YELLOW_D, BLACK_C);
		}
		
		if (FAS_IO.get_out_bit(m_nOut[1][i], IO_ON) == IO_ON)
		{
			GridColor(IDC_CUSTOM_IO, m_nOutPos[1][i][0], m_nOutPos[1][i][1], YELLOW_L, BLACK_C);
		}
		else
		{
			GridColor(IDC_CUSTOM_IO, m_nOutPos[1][i][0], m_nOutPos[1][i][1], YELLOW_D, BLACK_C);
		}
	}
}

LRESULT CDialog_Manual_Part_1::OnMotorCompletion( WPARAM wParam,LPARAM lParam )
{
//	if (st_handler.mn_menu_lock != FALSE)
//	{
//		st_handler.mn_menu_lock = FALSE;
//	}
//	// ==============================================================================
//
//	if( m_nGo_col > 0 && m_nGo_row > 0 )
//	{
//		GridData( IDC_CUSTOM_MOTOR, m_nGo_row, m_nGo_col, "Go" );
//		GridColor( IDC_CUSTOM_MOTOR, m_nGo_row, m_nGo_col, BLUE_L, BLACK_C);
//	}
//
//	m_btn_go.EnableWindow( TRUE );
//	m_btn_down.EnableWindow( TRUE );
//
//	m_nGo_col = -1;
//	m_nGo_row = -1;

	return 0;
}

void CDialog_Manual_Part_1::OnBtnPickDensityLabel() 
{

	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPosX = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
	double dPosY = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
	
	GETMOTOR( M_D_ATTACH_Z1 ).MotorStepClear();
	GETMOTOR( M_D_ATTACH_Z1 ).MotorLinearStart( dPosX, dPosY, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_D_ATTACH_Z1;
	m_btn_go.EnableWindow( FALSE );
}

void CDialog_Manual_Part_1::OnBtnPlaceDensityLabel() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPosX = st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY];
	double dPosY = st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY];
	
	GETMOTOR( M_D_ATTACH_Z1 ).MotorStepClear();
	GETMOTOR( M_D_ATTACH_Z1 ).MotorLinearStart( dPosX, dPosY, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_D_ATTACH_Z1;
	m_btn_go.EnableWindow( FALSE );	
}

void CDialog_Manual_Part_1::OnDensityPosSave() 
{
	CString sTempMSG;
	if (st_handler.mn_level_teach !=  TRUE)
	{
		sTempMSG = _T("Maint 암호를 넣어주세요! 메인트 암호가 아닙니다.");
		if ( g_local.GetLocalType() == LOCAL_ENG ) sTempMSG = _T("Maint password is not correct.");
		sprintf(st_msg.c_abnormal_msg, "%s", sTempMSG);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		
		return;
	}
	// TODO: Add your control notification handler code here
	m_strTempMSG = "용량라벨의 시작위치 저장합니다. (XYZ)";
	if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("Density Label Start Pos is save.(xyz)");
	if (DoModal_Select( m_strTempMSG ) != IDOK)
		return;

	double dPosX = g_comiMgr.Get_MotCurrentPos(M_D_ATTACH_X);
	double dPosY = g_comiMgr.Get_MotCurrentPos(M_D_ATTACH_Y);
	double dPosZ1 = g_comiMgr.Get_MotCurrentPos(M_D_ATTACH_Z1);
	double dPosZ2 = g_comiMgr.Get_MotCurrentPos(M_D_ATTACH_Z2);

	st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + mn_manual_device_type] = dPosX;
	st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + mn_manual_device_type] = dPosY;
	st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type] = dPosZ1;
	st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type] = dPosZ2;


	m_strTempMSG = "용량라벨의 시작위치 저장완료. (XYZ)";
	if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("Density Label Pos save complete.");
	DoModal_Msg( m_strTempMSG);
	
	if (st_handler.mn_level_teach !=  TRUE)
	{
		sTempMSG = _T("Maint 암호를 넣어주세요! 메인트 암호가 아닙니다.");
		if ( g_local.GetLocalType() == LOCAL_ENG ) sTempMSG = _T("Maint password is not correct.");
		sprintf(st_msg.c_abnormal_msg, "%s", sTempMSG);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		
		return;
	}	
}

void CDialog_Manual_Part_1::OnButtonZdown() 
{
	CString sTempMSG;
	if (st_handler.mn_level_teach !=  TRUE)
	{
		sTempMSG = _T("Maint 암호를 넣어주세요! 메인트 암호가 아닙니다.");
		if ( g_local.GetLocalType() == LOCAL_ENG ) sTempMSG = _T("Maint password is not correct.");
		sprintf(st_msg.c_abnormal_msg, "%s", sTempMSG);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		
		return;
	}


	// TODO: Add your control notification handler code here
	// z축 motor 

	if( m_nRobot < 0 )
		return;

	int nAxis = M_D_ATTACH_Z1;
	if( m_nRobot == M_D_ATTACH_X || m_nRobot == M_D_ATTACH_Y || m_nRobot == M_D_ATTACH_Z1 ||m_nRobot == M_D_ATTACH_Z2 )
		nAxis = M_D_ATTACH_Z1;
	else if(m_nRobot == M_BCR_ATTACH1_X || m_nRobot == M_BCR_ATTACH1_Z1 || m_nRobot == M_BCR_ATTACH1_Z2 )
		nAxis = M_BCR_ATTACH1_Z1;
	else if(m_nRobot == M_BCR_ATTACH2_X || m_nRobot == M_BCR_ATTACH2_Z1 || m_nRobot == M_BCR_ATTACH2_Z2 )
		nAxis = M_BCR_ATTACH2_Z1;
	else if( m_nRobot == M_L_REJECT_X || m_nRobot == M_TOP_VISION_X)
		nAxis = M_L_REJECT_X;
	else if( m_nRobot == M_ULD_Y || m_nRobot == M_ULD_Z)
		nAxis = M_ULD_Z;


	if( st_handler.mn_menu_lock )
		return;

	if (COMI.mn_motorbd_init_end != TRUE)  
	{
		if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
			return ;
	}

	BOOL bPick = PLACE;
	if( m_chk_pick.GetCheck() )
		bPick = PICK;

	double dChk_X,dChk_Y,dGotoPos1,dGotoPos2;
	if( nAxis == M_BCR_ATTACH1_Z1 ||  nAxis == M_BCR_ATTACH2_Z1)
	{
		int attachX = M_BCR_ATTACH1_X;
		int attachZ1 = M_BCR_ATTACH1_Z1;
		int attachZ2 = M_BCR_ATTACH1_Z2;
		if( nAxis == M_BCR_ATTACH2_Z1 )
		{
			attachX = M_BCR_ATTACH2_X;
			attachZ1 = M_BCR_ATTACH2_Z1;
			attachZ2 = M_BCR_ATTACH2_Z2;
		}

		if( bPick == PICK )
			dChk_X = st_motor[attachX].d_pos[P_XY_DENSITY_F14 + mn_manual_device_type];
		else
			dChk_X = st_motor[attachX].d_pos[P_XY_PLACE_DENSITY];
		
		if( ( abs(dChk_X - g_comiMgr.Get_MotCurrentPos( M_D_ATTACH_X )) > 0.1f ) )
		{
			if ( g_local.GetLocalType() == LOCAL_ENG )
			{
				if (DoModal_Msg( "X axis position does not match." ) == IDOK)//20130930
					return ;
			}
			else
			{
				if (DoModal_Msg( "X축 위치가 맞지 않습니다." ) == IDOK)
					return ;
			}
		}
		
		if( bPick == PICK )
		{
			dGotoPos1 = st_motor[attachZ1].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type];
			dGotoPos2 = st_motor[attachZ2].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type];
		}
		else
		{
			dGotoPos1 = st_motor[attachZ1].d_pos[P_Z_PLACE_F];
			dGotoPos2 = st_motor[attachZ2].d_pos[P_Z_PLACE_F];
		}
		double dGotoPos2 = st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type];
		GETMOTOR( (ENUM_MOTOR)attachZ1 ).SetMotGotoPos( dGotoPos1 );
		GETMOTOR( (ENUM_MOTOR)attachZ2 ).SetMotGotoPos( dGotoPos2 );
		
		int nResponse = Func.OnMotor_Go_Check(nAxis, dGotoPos1);
		
		if (nResponse != IDOK)
		{
			return;
		}
		if (st_handler.mn_menu_lock != TRUE)
		{
			st_handler.mn_menu_lock = TRUE;
		}
		// =============================================================================
		
		GETMOTOR( (ENUM_MOTOR)attachZ1 ).MotorStepClear();
		GETMOTOR( (ENUM_MOTOR)attachZ1 ).MotorMoveStart( dGotoPos1, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
		GETMOTOR( (ENUM_MOTOR)attachZ2 ).MotorStepClear();
		GETMOTOR( (ENUM_MOTOR)attachZ2 ).MotorMoveStart( dGotoPos2, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );

	}
	else if( nAxis == M_D_ATTACH_Z1)
	{
		if( bPick == PICK )
		{
			dGotoPos1 = st_motor[M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type];
			dGotoPos2 = st_motor[M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + mn_manual_device_type];
			dChk_X = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + mn_manual_device_type];
			dChk_Y = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + mn_manual_device_type];
		}
		else
		{
			dGotoPos1 = st_motor[M_D_ATTACH_Z1].d_pos[P_Z_PLACE_F];
			dGotoPos2 = st_motor[M_D_ATTACH_Z2].d_pos[P_Z_PLACE_F];
			dChk_X = st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY];
			dChk_Y = st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY];
		}
		if( ( abs(dChk_X - g_comiMgr.Get_MotCurrentPos( M_D_ATTACH_X )) > 0.1f ) ||
			( abs(dChk_Y - g_comiMgr.Get_MotCurrentPos( M_D_ATTACH_Y )) > 0.1f ) )
		{
			if ( g_local.GetLocalType() == LOCAL_ENG )
			{
				if (DoModal_Msg( " Density X, Y axis position does not match." ) == IDOK)//20130930
					return ;
			}
			else
			{
				if (DoModal_Msg( " Density  X, Y축 위치가 맞지 않습니다." ) == IDOK)
					return ;
			}
		}
			

		GETMOTOR( (ENUM_MOTOR)M_D_ATTACH_Z1 ).SetMotGotoPos( dGotoPos1 );
		GETMOTOR( (ENUM_MOTOR)M_D_ATTACH_Z2 ).SetMotGotoPos( dGotoPos2 );
		
		int nResponse = Func.OnMotor_Go_Check(nAxis, dGotoPos1);
		
		if (nResponse != IDOK)
		{
			return;
		}

		if (st_handler.mn_menu_lock != TRUE)
		{
			st_handler.mn_menu_lock = TRUE;
		}
		// =============================================================================
		
		GETMOTOR( (ENUM_MOTOR)M_D_ATTACH_Z1 ).MotorStepClear();
		GETMOTOR( (ENUM_MOTOR)M_D_ATTACH_Z2 ).MotorStepClear();
		GETMOTOR( (ENUM_MOTOR)M_D_ATTACH_Z1 ).MotorMoveStart( dGotoPos1, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
		GETMOTOR( (ENUM_MOTOR)M_D_ATTACH_Z2 ).MotorMoveStart( dGotoPos2, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	}
	else if(nAxis == M_L_REJECT_X)
	{
		return;
	}
	else if( nAxis == M_ULD_Z )
	{
		if( bPick == PICK )
		{
			dGotoPos1 = st_motor[M_ULD_Z].d_pos[P_ULD_Z_PICK_POS];
			dChk_Y = st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS];
		}
		else
		{
			dGotoPos1 = st_motor[M_ULD_Z].d_pos[P_ULD_Z_CONV_POS];
			dChk_Y = st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS];
		}
		if( ( abs(dChk_Y - g_comiMgr.Get_MotCurrentPos( M_ULD_Y )) > 0.1f ) )
		{
			if ( g_local.GetLocalType() == LOCAL_ENG )
			{
				if (DoModal_Msg( " Unload Y axis position does not match." ) == IDOK)//20130930
					return ;
			}
			else
			{
				if (DoModal_Msg( " Unload Y축 위치가 맞지 않습니다." ) == IDOK)
					return ;
			}
		}
			

		GETMOTOR( (ENUM_MOTOR)M_ULD_Z ).SetMotGotoPos( dGotoPos1 );
		
		int nResponse = Func.OnMotor_Go_Check(nAxis, dGotoPos1);
		
		if (nResponse != IDOK)
		{
			return;
		}

		if (st_handler.mn_menu_lock != TRUE)
		{
			st_handler.mn_menu_lock = TRUE;
		}
		// =============================================================================
		
		GETMOTOR( (ENUM_MOTOR)M_ULD_Z ).MotorStepClear();
		GETMOTOR( (ENUM_MOTOR)M_ULD_Z ).MotorMoveStart( dGotoPos1, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	}
}



void CDialog_Manual_Part_1::OnBtnBcrOutLabel() 
{
	::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1, CLIENT_CLOSE, CLS_BCR_PRINTER1);
// //	OnPrintFormat(0);
// 	Func.m_pZebra[0]->OnPrintOutput(2, CLS_BCR_PRINTER1, 0, 0,  Func.GetPrintFormat());

	SetTimer(TM_DISTRIBUTION_LABEL_OUT, 100, NULL);
	mn_distribution_label_no = 0;
	mn_distribution_move_step[mn_distribution_label_no] = 0;

}


void CDialog_Manual_Part_1::OnBtnBcrOutLabel2() 
{
	::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_2, CLIENT_CLOSE, CLS_BCR_PRINTER2);
// //	OnPrintFormat(1);
// 	Func.m_pZebra[1]->OnPrintOutput(2, CLS_BCR_PRINTER2, 0, 0,  Func.GetPrintFormat());
	SetTimer(TM_DISTRIBUTION_LABEL_OUT, 100, NULL);
	mn_distribution_label_no = 1;
	mn_distribution_move_step[mn_distribution_label_no] = 0;
	
}

void CDialog_Manual_Part_1::OnBtnBcrOutLabel3() 
{
	::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_3, CLIENT_CLOSE, CLS_BCR_PRINTER3);
// //	OnPrintFormat(2);
// 	Func.m_pZebra[2]->OnPrintOutput(2, CLS_BCR_PRINTER3, 0, 0,  Func.GetPrintFormat());
	SetTimer(TM_DISTRIBUTION_LABEL_OUT, 100, NULL);
	mn_distribution_label_no = 2;
	mn_distribution_move_step[mn_distribution_label_no] = 0;

}

void CDialog_Manual_Part_1::OnBtnBcrOutLabel4() 
{
	::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_4, CLIENT_CLOSE, CLS_BCR_PRINTER4);
// //	OnPrintFormat(3);	
// 	Func.m_pZebra[3]->OnPrintOutput(2, CLS_BCR_PRINTER4, 0, 0,  Func.GetPrintFormat());
	SetTimer(TM_DISTRIBUTION_LABEL_OUT, 100, NULL);
	mn_distribution_label_no = 3;
	mn_distribution_move_step[mn_distribution_label_no] = 0;

}

////2014.1108 ysj
int CDialog_Manual_Part_1::Distribution_Label_Print(int label_no)//2014.1102
{
	int nFuncRet = RET_PROCEED;
	CString strJamCode;
	CString strFotmat_1;
	CString strFotmat_2;
	CString strFotmat_3;
	CString strFotmat_4;
	int nRet1, nRet2, nRet3;

	if(mn_stop_req == CTL_YES)
	{
		mn_stop_req = CTL_NO;
		mn_distribution_move_step[label_no] = 0;
		if(label_no == 0)
			COMI.Set_MotStop( true, M_BCR_FEEDER_1 );
		else if(label_no == 1)
			COMI.Set_MotStop( true, M_BCR_FEEDER_2 );
		else if(label_no == 2)
			COMI.Set_MotStop( true, M_BCR_FEEDER_3 );
		else if(label_no == 3)
			COMI.Set_MotStop( true, M_BCR_FEEDER_4 );
		KillTimer(TM_DISTRIBUTION_LABEL_OUT);
		return nFuncRet;
		
	}
	int nMotorNum = 0;
	if(label_no == 0) nMotorNum = M_BCR_FEEDER_1;
	else if(label_no == 1) nMotorNum = M_BCR_FEEDER_2;
	else if(label_no == 2) nMotorNum = M_BCR_FEEDER_3;
	else if(label_no == 3) nMotorNum = M_BCR_FEEDER_4;
	switch(mn_distribution_move_step[label_no])
	{
	case 0:
		mn_distribution_move_step[label_no] = 100;
// 		mn_distribution_move_step[label_no] = 310;//ybs
		break;

	case 100:
		g_ioMgr.set_out_bit( st_io.o_distribute_plate_updown[label_no], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[label_no], IO_ON);

		mn_distribution_move_step[label_no] = 300;
		break;
		
	case 300:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_PICK], st_basic.nManualSpeed);
			if( nRet == BD_GOOD )
			{
				m_dwTimeWait_distribution[label_no][0] = GetTickCount();
				mn_distribution_move_step[label_no] = 310;
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(120, CTL_dWARNING, strJamCode );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 310:
		if (st_client[label_no + CLS_BCR_PRINTER1].n_connect == CTL_YES)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			mn_distribution_move_step[label_no] = 400;
		}
		else 
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1 + label_no, CLIENT_CONNECT, label_no + CLS_BCR_PRINTER1);
			
			mn_distribution_move_step[label_no] = 320;
		}
		break;
		
	case 320:
		if (st_client[label_no + CLS_BCR_PRINTER1].n_connect == CTL_YES)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			mn_distribution_move_step[label_no] = 400;
		}
		else 
		{
			m_dwTimeWait_distribution[label_no][1] = GetTickCount();
			m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
			
			if (m_dwTimeWait_distribution[label_no][2] <= 0)
			{
				m_dwTimeWait_distribution[label_no][0] = GetTickCount();
				break;
			}
			
			if (m_dwTimeWait_distribution[label_no][2] >= 3000)
			{
				mn_retry_cnt++;
				
				if (mn_retry_cnt >= 3)
				{
					mn_retry_cnt = 0;
					
					// 700200 1 00 "Barcode1_time_out_from_server."
					strJamCode.Format( "7002%d0", label_no);
					// 					strJamCode.Format( "%02d0006", M_BCR_FEEDER_1+nSite);
					CTL_Lib.Alarm_Error_Occurrence(121, CTL_dWARNING, strJamCode );
					
					mn_distribution_move_step[label_no] = 310;
					mn_stop_req = CTL_YES;
				}
				else
				{
					m_dwTimeWait_distribution[label_no][0] = GetTickCount();
					
					mn_distribution_move_step[label_no] = 330;
					//ReqBcrStatusPrint(nSite);
				}
			}
		}
		break;
		
	case 330:
		m_dwTimeWait_distribution[label_no][1] = GetTickCount();
		m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
		
		if (m_dwTimeWait_distribution[label_no][2] <= 0)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeWait_distribution[label_no][2] >= 5000)
		{
			mn_distribution_move_step[label_no] = 310;
		}
		break;
		
	case 400:
		m_dwTimeWait_distribution[label_no][1] = GetTickCount();
		m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
		
		if (m_dwTimeWait_distribution[label_no][2] <= 0)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			break;
		}
		
		mn_distribution_move_step[label_no] = 500;
		break;

	case 500:
		strFotmat_1 ="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD1TB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS^FO900,65^BY3,3,100^FR^BC,,N^FDS21HNXAG307839^FS^FO900,185^A0,55,65^FR^FDS/N:S21HNXAG307839^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E500B/AM^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E500^FS^FO900,510^GB780,275,5^FS^FO1600,550^FPV^A0I,70,70^FR^FDUPC^FS^FO970,580^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276086088^FS^PQ1^XZ";

 
        //strFotmat_1 ="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD250GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS^FO900,65^BY3,3,100^FR^BC,,N^FDS21MNYAG101867^FS^FO900,185^A0,55,65^FR^FDS/N:S21MNYAG101867^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E250BW^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E250^FS^FO900,510^GB780,275,5^FS^FO1600,550^FPV^A0I,70,70^FR^FDUPC^FS^FO970,580^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276086033^FS^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522915^FS^PQ1^XZ";
		//strFotmat_1 ="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD250GB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS^FO900,65^BY3,3,100^FR^BC,,N^FDS109IJKLMNOPQRST^FS^FO900,185^A0,55,65^FR^FDS/N:S21MNYAG101082^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E250BW^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E250^FS^FO900,510^GB780,275,5^FS^FO1600,550^FPV^A0I,70,70^FR^FDUPC^FS^FO970,580^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276086033^FS^FO5,510^GB780,275,5^FS^FO680,550^FPV^A0I,70,70^FR^FDEAN^FS^FO58,580^BY6,3,180^FR^BEI,180,Y,N^FD8806086522915^FS^PQ1^XZ";
		            //^XA^LH200,47^PON^FS^FO5,15^A0,50,50^FR^FDSamsung SSD 850 PRO^FS^FO5,80^A0,50,50^FR^FD128GB^FS^FO5,140^A0,50,50^FR^FDMade in China^FS^FO900,15^GB685,100,100^FS^FO945,15^BY3,3,100^FR^BC,,N^FDS1SMNSAFC06912^FS^FO900,135^A0,50,50^FR^FDS/N:S1SMNSAFC06912^FS^FO35,385^A0I,48,48^FR^FDModel Code: MZ-7KE128BW^FS^FO1199,385^A0I,48,48^FR^FDModel: MZ-7KE128^FS^FO5,460^GB780,275,180^FS^FO900,460^GB800,275,180^FS^FO1600,500^FPV^A0I,75,75^FR^FDUPC^FS^FO970,530^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276057477^FS^FO680,500^FPV^A0I,75,75^FR^FDEAN^FS^FO58,530^BY6,3,180^FR^BEI,180,Y,N^FD8806086264617^FS^PQ1^XZ";
// 		strFotmat_2 ="^FO945,95^BY3,3,100^FR^BC,,N^FDS1SMNSAFC06912^FS^FO900,215^A0,50,50^FR^FDS/N:S1SMNSAFC06912^FS^FO70,470^A0I,48,48^FR^FDModel Code: MZ-7KE128BW^FS^FO1199,470^A0I,48,48^FR";
// 		strFotmat_3 ="^FDModel: MZ-7KE128^FS^FO5,557^GB780,275,180^FS^FO900,557^GB800,275,180^FS^FO1600,500^FPV^A0I,75,75^FR^FDUPC^FS^FO970,630^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276057477^FS^FO680,590^FPV^A0I,75,75^FR";
// 		strFotmat_4 ="^FDEAN^FS^FO58,630^BY6,3,180^FR^BEI,180,Y,N^FD8806086264617^FS^PQ1^XZ";
			

// 		strFotmat_1="^XA^LH200,047^PON^FS^CI28^CW2,E:KR.TTF^FS^FO5,95^A2,50,50^FR^FDSAMSUNG SSD 840 EVO mSATA^FS^FO5,160^A2,50,50^FR^FD512GB^FS^FO5,220^A2,50,50^FR^FDMade in Korea^FS";
// 		strFotmat_2="^FO900,95^GB685,100,100^FS^FO945,95^BY3,3,100^FR^BC,,N^FDS1AXNEAD908028^FS^FO900,215^A0,50,70^FR^FDS/N: 1AXNEAD908028^FS^FO70,470^A2I,48,48^FR^FDModel Code: MZ-7PD512BW^FS";
// 		strFotmat_3="^FO1195,470^A2I,48,48^FR^FDModel: MZ-7PD512^FS^FO5,557^GB780,275,180^FS^FO895,557^GB780,275,180^FS^FO1575,590^FPV^A2I,75,75^FR^FDUPC^FS";
// 		strFotmat_4="^FO950,630^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276001180^FS^FO680,590^FPV^A2I,75,75^FR^FDEAN^FS^FO58,630^BY6,3,180^FR^BEI,180,Y,N^FD8806085216716^FS^PQ1^XZ";

		m_strPrintFormat = Func.GetPrintFormat();// strFotmat_1;// + strFotmat_2 + strFotmat_3 + strFotmat_4;

// 		m_strPrintFormat = strFotmat_1;
    	strFotmat_1 ="^XA^LH200,047^PON^FS^FO5,65^A0,55,60^FR^FDSamsung SSD 850 EVO^FS^FO5,130^A0,55,60^FR^FD1TB^FS^FO5,190^A0,55,60^FR^FDMade in China^FS^FO900,65^BY3,3,100^FR^BC,,N^FDS21NNXAG339489^FS^FO900,185^A0,55,65^FR^FDS/N:S21NNXAG339489^FS^FO25,435^A0I,55,65^FR^FDModel Code: MZ-75E250B/AM^FS^FO1165,435^A0I,55,65^FR^FDModel: MZ-75E250^FS^FO900,510^GB780,275,5^FS^FO1600,550^FPV^A0I,70,70^FR^FDUPC^FS^FO970,580^BY6,3,180^FR^BUI,180,Y,N,Y^FD887276086071^FS^PQ1^XZ";
		m_strPrintFormat = strFotmat_1;


		m_dwTimeWait_distribution[label_no][0] = GetTickCount();
		//	(통신방법[0]시리얼[1]LPT[2]네트워크, 네트워크, 라벨 X offset, 라벨 Y offset, 라벨 포맷); 
		// 	Func.m_pZebra[0]->OnPrintOutput(2, CLS_BCR_PRINTER1, 0, 0,  Func.GetPrintFormat());
		if(label_no == 0)
		{
			Func.m_pZebra[label_no]->OnPrintOutput(2, CLS_BCR_PRINTER1, 0, 0,  m_strPrintFormat);
		}
		else if(label_no == 1)
		{
			Func.m_pZebra[label_no]->OnPrintOutput(2, CLS_BCR_PRINTER2, 0, 0,  m_strPrintFormat);
		}
		else if(label_no == 2)
		{
			Func.m_pZebra[label_no]->OnPrintOutput(2, CLS_BCR_PRINTER3, 0, 0,  m_strPrintFormat);
		}
		else if(label_no == 3)
		{
			Func.m_pZebra[label_no]->OnPrintOutput(2, CLS_BCR_PRINTER4, 0, 0,  m_strPrintFormat);
		}

		mn_distribution_move_step[label_no] = 800;
		break;
		
	case 800:
			m_dwTimeWait_distribution[label_no][1] = GetTickCount();
			m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
			
			if (m_dwTimeWait_distribution[label_no][2] <= 0)
			{
				m_dwTimeWait_distribution[label_no][0] = GetTickCount();

				break;
			}
			
			if (m_dwTimeWait_distribution[label_no][2] >= 1000)
			{
// 				if(label_no == 0)
// 				{
// 					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_1, CLIENT_CLOSE, CLS_BCR_PRINTER1);
// 				}
// 				else if(label_no == 1)
// 				{
// 					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_2, CLIENT_CLOSE, CLS_BCR_PRINTER2);
// 				}
// 				else if(label_no == 2)
// 				{
// 					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_3, CLIENT_CLOSE, CLS_BCR_PRINTER3);
// 				}
// 				else if(label_no == 3)
// 				{
// 					::SendMessage(st_handler.hWnd, WM_ZEBRA_CLIENT_MSG_4, CLIENT_CLOSE, CLS_BCR_PRINTER4);
// 				}
				mn_distribution_move_step[label_no] = 900;
				//ybs
// 				mn_distribution_move_step[label_no] = 0;
// 				KillTimer(TM_DISTRIBUTION_LABEL_OUT);
			}
// 		}
		break;

	case 900:
		g_ioMgr.set_out_bit( st_io.o_distribute_plate_vacuum[label_no], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[label_no], IO_OFF);
		
		m_dwTimeWait_distribution[label_no][0] = GetTickCount();
		mn_distribution_move_step[label_no] = 1000;
		break;
		
	case 1000:
		m_dwTimeWait_distribution[label_no][1] = GetTickCount();
		m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
		
		if (m_dwTimeWait_distribution[label_no][2] <= 0)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeWait_distribution[label_no][2] >= 600)
		{
// 			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_plate_vacuum_chk[label_no], IO_ON);
// 			
// 			if(nRet1 == IO_ON)
// 			{
				mn_distribution_move_step[label_no] = 1100;
// 			}
		}
		break;
	
	case 1100:
		g_ioMgr.set_out_bit( st_io.o_distribute_plate_updown[label_no], IO_ON);
		
		m_dwTimeWait_distribution[label_no][0] = GetTickCount();
		mn_distribution_move_step[label_no] = 1200;
		break;
		
	case 1200:
		m_dwTimeWait_distribution[label_no][1] = GetTickCount();
		m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
		
		if (m_dwTimeWait_distribution[label_no][2] <= 0)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeWait_distribution[label_no][2] >= 500)
		{
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_plate_up_chk[label_no], IO_ON);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_plate_vacuum_chk[label_no], IO_ON);
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[label_no], IO_ON);
			
  			if(nRet1 == IO_ON && (nRet2 == IO_ON || nRet3 == IO_ON))
			{
				mn_distribution_move_step[label_no] = 1210;
				m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			}
		}
		break;

	case 1210:
		m_dwTimeWait_distribution[label_no][1] = GetTickCount();
		m_dwTimeWait_distribution[label_no][2] = m_dwTimeWait_distribution[label_no][1] - m_dwTimeWait_distribution[label_no][0];
		
		if (m_dwTimeWait_distribution[label_no][2] <= 0)
		{
			m_dwTimeWait_distribution[label_no][0] = GetTickCount();
			break;
		}
		
		if (m_dwTimeWait_distribution[label_no][2] < 500) break;

		nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_plate_vacuum_chk[label_no], IO_ON);
		nRet3 =  g_ioMgr.get_in_bit(st_io.i_bcr_label_chk[label_no], IO_ON);
		if( nRet2 == IO_ON || nRet3 == IO_ON)
		{
			mn_distribution_move_step[label_no] = 1300;
			break;

		}
		else
		{
			CString strError="";
			strError.Format("5%04d%d", st_io.i_distribute_plate_vacuum_chk[label_no], IO_ON);
			CTL_Lib.Alarm_Error_Occurrence(757, dWARNING, strError);
			mn_stop_req = CTL_YES;
		}
		break;

	case 1300:
		{
			int nRet = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].d_pos[P_BCR_FEEDER_LOAD], st_basic.nRunSpeed);
			if( nRet == BD_GOOD )
			{
				mn_distribution_move_step[label_no] = 0;
				KillTimer(TM_DISTRIBUTION_LABEL_OUT);
			}
			else if( nRet == BD_ERROR )
			{//060006 1 00 "Distribute Feeder#1 Axis Motor Move Error."
				CString strJamCode;
				strJamCode.Format( "%02d0006", nMotorNum);
				CTL_Lib.Alarm_Error_Occurrence(124, CTL_dWARNING, strJamCode );
				mn_stop_req = CTL_YES;
			}
		}
		break;
	}
	return nFuncRet;

}

////

void CDialog_Manual_Part_1::OnBtnBcrVisionLabel() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Manual_Part_1::OnBtnBcrVisionLabel2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Manual_Part_1::OnBtnBcrVisionLabel3() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Manual_Part_1::OnBtnBcrVisionLabel4() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Manual_Part_1::OnBtnPickBcrLabel1() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK];	
	
	GETMOTOR( M_BCR_ATTACH1_X ).MotorStepClear();
	GETMOTOR( M_BCR_ATTACH1_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	
	m_nRobot = M_BCR_ATTACH1_Z1;

	m_btn_go.EnableWindow( FALSE );		
}

void CDialog_Manual_Part_1::OnBtnPickBcrLabel2() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK];	
	
	GETMOTOR( M_BCR_ATTACH2_X ).MotorStepClear();
	GETMOTOR( M_BCR_ATTACH2_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BCR_ATTACH2_Z1;
	m_btn_go.EnableWindow( FALSE );		
}

void CDialog_Manual_Part_1::OnBtnPlaceBcrLabel12() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE];	
	
	GETMOTOR( M_BCR_ATTACH1_X ).MotorStepClear();
	GETMOTOR( M_BCR_ATTACH1_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BCR_ATTACH1_X;
	m_btn_go.EnableWindow( FALSE );			
}

void CDialog_Manual_Part_1::OnBtnPlaceBcrLabel34() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE];	
	
	GETMOTOR( M_BCR_ATTACH2_X ).MotorStepClear();
	GETMOTOR( M_BCR_ATTACH2_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BCR_ATTACH2_X;
	m_btn_go.EnableWindow( FALSE );			
}

void CDialog_Manual_Part_1::OnBtnRejBcrLabel() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F1];	
	
	GETMOTOR( M_L_REJECT_X ).MotorStepClear();
	GETMOTOR( M_L_REJECT_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_L_REJECT_X;
	m_btn_go.EnableWindow( FALSE );				
}

void CDialog_Manual_Part_1::OnBtnRejBcrLabel2() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F2];	
	
	GETMOTOR( M_L_REJECT_X ).MotorStepClear();
	GETMOTOR( M_L_REJECT_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BCR_ATTACH2_X;
	m_btn_go.EnableWindow( FALSE );					
}

void CDialog_Manual_Part_1::OnBtnRejBcrLabel3() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F3];	
	
	GETMOTOR( M_L_REJECT_X ).MotorStepClear();
	GETMOTOR( M_L_REJECT_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BCR_ATTACH2_X;
	m_btn_go.EnableWindow( FALSE );			}

void CDialog_Manual_Part_1::OnBtnRejBcrLabel4() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_L_REJECT_X].d_pos[P_REJ_BARCODE_F4];	
	
	GETMOTOR( M_L_REJECT_X ).MotorStepClear();
	GETMOTOR( M_L_REJECT_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BCR_ATTACH2_X;
	m_btn_go.EnableWindow( FALSE );				
}

void CDialog_Manual_Part_1::OnBtnTopVision() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_TOP_VISION_X].d_pos[P_VIS1_POS];	
	
	GETMOTOR( M_TOP_VISION_X ).MotorStepClear();
	GETMOTOR( M_TOP_VISION_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_TOP_VISION_X;
	m_btn_go.EnableWindow( FALSE );		
}

void CDialog_Manual_Part_1::OnBtnTopVision2() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_TOP_VISION_X].d_pos[P_VIS2_POS];	
	
	GETMOTOR( M_TOP_VISION_X ).MotorStepClear();
	GETMOTOR( M_TOP_VISION_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_TOP_VISION_X;
	m_btn_go.EnableWindow( FALSE );			
}

void CDialog_Manual_Part_1::OnBtnPickUnload() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_ULD_Y].d_pos[P_ULD_Y_PICK_POS];
	
	GETMOTOR( M_ULD_Y ).MotorStepClear();
	GETMOTOR( M_ULD_Y ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_ULD_Z;
	m_btn_go.EnableWindow( FALSE );	
}

void CDialog_Manual_Part_1::OnBtnBtmVision() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_ULD_Y].d_pos[P_ULD_Y_VIS_POS];	
	
	GETMOTOR( M_ULD_Y ).MotorStepClear();
	GETMOTOR( M_ULD_Y ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_ULD_Y;//2014.1017 M_TOP_VISION_X -> M_ULD_Y;
	m_btn_go.EnableWindow( FALSE );			
}

void CDialog_Manual_Part_1::OnBtnBtmVision4() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_ULD_Z].d_pos[P_ULD_Z_VIS_POS];	
	
	GETMOTOR( M_ULD_Z ).MotorStepClear();
	GETMOTOR( M_ULD_Z ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_ULD_Z;
	m_btn_go.EnableWindow( FALSE );			
}

void CDialog_Manual_Part_1::OnBtnBtmVision2() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_BTM_VISION_X].d_pos[P_VIS1_POS];	
	
	GETMOTOR( M_BTM_VISION_X ).MotorStepClear();
	GETMOTOR( M_BTM_VISION_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BTM_VISION_X;
	m_btn_go.EnableWindow( FALSE );				
}

void CDialog_Manual_Part_1::OnBtnBtmVision3() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_BTM_VISION_X].d_pos[P_VIS2_POS];	
	
	GETMOTOR( M_BTM_VISION_X ).MotorStepClear();
	GETMOTOR( M_BTM_VISION_X ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_BTM_VISION_X;
	m_btn_go.EnableWindow( FALSE );				
}

void CDialog_Manual_Part_1::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TMR_IO_CHECK:
		OnManualPart1_IO_Check();
		break;
		////2014.1108 ysj
	case TM_DENSITY_LABEL_OUT:
		DLabel_Print(mn_density_label_no);
//		DLabel_Print(mn_density_label_no + 3);
		break;	
	case TM_DENSITY_ROBOT_MOVE:
		DRobot_Move(mn_density_feeder_no);
		break;
		
	case TM_DISTRIBUTION_ROBOT_MOVE1:
		Distribution_Robot_Move1();
		break;
		
	case TM_DISTRIBUTION_ROBOT_MOVE2:
		Distribution_Robot_Move2();
		break;
		
	case TM_DISTRIBUTION_LABEL_OUT:
		Distribution_Label_Print(mn_distribution_label_no);
		
		break;
	////
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Manual_Part_1::OnBtnConvUldPlace() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_ULD_Y].d_pos[P_ULD_Y_CONV_POS];	
	
	GETMOTOR( M_ULD_Y ).MotorStepClear();
	GETMOTOR( M_ULD_Y ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_ULD_Y;
	m_btn_go.EnableWindow( FALSE );					
}

void CDialog_Manual_Part_1::OnBtnRejectUldPlace() 
{
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// =============================================================================

	BOOL bPlace = PICK;

	double dPos = st_motor[M_ULD_Y].d_pos[P_ULD_Y_REJ_POS];	
	
	GETMOTOR( M_ULD_Y ).MotorStepClear();
	GETMOTOR( M_ULD_Y ).MotorChkMoveStart( dPos, st_basic.nManualSpeed, MMMODE_WITH_IO_MOVE );
	

	m_nRobot = M_ULD_Y;
	m_btn_go.EnableWindow( FALSE );			
}

void CDialog_Manual_Part_1::OnBtnUnloadStacker() 
{
	
}

BOOL CDialog_Manual_Part_1::Create() 
{
	return CDialog::Create( CDialog_Manual_Part_1::IDD );
}

void CDialog_Manual_Part_1::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	mn_stop_req = CTL_YES;

}

void CDialog_Manual_Part_1::OnBtn1Exit() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
	
}

void CDialog_Manual_Part_1::PostNcDestroy() 
{
	delete this;
	Func.mp_manual_1 = NULL;
	
	CDialog::PostNcDestroy();
}

void CDialog_Manual_Part_1::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Manual_Part_1::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
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
			Grid->SetTypeComboBox(&CellType, 0, strList);
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
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}

	Grid->SetCellType(col,row,&CellType);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Manual_Part_1::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Manual_Part_1::GridFont(UINT nID, int row, int col, int size)
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

void CDialog_Manual_Part_1::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{	
		Grid = NULL;
		delete Grid;
	}
}

void CDialog_Manual_Part_1::OnCellClick( WPARAM wParam, LPARAM lParam )
{
/*	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	int nRow;
	int nCol;

	nRow = lpcc->Row;
	nCol = lpcc->Col;

	switch(nRow)
	{
		case 1:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_lable1_start, !FAS_IO.get_out_bit(st_io.o_density_lable1_start, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_lable2_start, !FAS_IO.get_out_bit(st_io.o_density_lable2_start, IO_ON));
			}
			break;

		case 2:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_lable3_start, !FAS_IO.get_out_bit(st_io.o_density_lable3_start, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_lable4_start, !FAS_IO.get_out_bit(st_io.o_density_lable4_start, IO_ON));
			}
			break;

		case 3:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_lable5_start, !FAS_IO.get_out_bit(st_io.o_density_lable5_start, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_lable6_start, !FAS_IO.get_out_bit(st_io.o_density_lable6_start, IO_ON));
			}
			break;

		case 4:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_lable1_pickon, !FAS_IO.get_out_bit(st_io.o_density_lable1_pickon, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_lable2_pickon, !FAS_IO.get_out_bit(st_io.o_density_lable2_pickon, IO_ON));
			}
			break;

		case 5:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_lable3_pickon, !FAS_IO.get_out_bit(st_io.o_density_lable3_pickon, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_lable4_pickon, !FAS_IO.get_out_bit(st_io.o_density_lable4_pickon, IO_ON));
			}
			break;

		case 6:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_lable5_pickon, !FAS_IO.get_out_bit(st_io.o_density_lable5_pickon, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_lable6_pickon, !FAS_IO.get_out_bit(st_io.o_density_lable6_pickon, IO_ON));
			}
			break;

		case 10:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_left_vacuum1, !FAS_IO.get_out_bit(st_io.o_density_rbt_left_vacuum1, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_left_vacuum2, !FAS_IO.get_out_bit(st_io.o_density_rbt_left_vacuum2, IO_ON));
			}
			break;

		case 11:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_left_blow1, !FAS_IO.get_out_bit(st_io.o_density_rbt_left_blow1, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_left_blow2, !FAS_IO.get_out_bit(st_io.o_density_rbt_left_blow2, IO_ON));
			}
			break;

		case 12:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_right_vacuum1, !FAS_IO.get_out_bit(st_io.o_density_rbt_right_vacuum1, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_right_vacuum2, !FAS_IO.get_out_bit(st_io.o_density_rbt_right_vacuum2, IO_ON));
			}
			break;
			
		case 13:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_right_blow1, !FAS_IO.get_out_bit(st_io.o_density_rbt_right_blow1, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_density_rbt_right_blow2, !FAS_IO.get_out_bit(st_io.o_density_rbt_right_blow2, IO_ON));
			}
			break;

		case 14:
			break;

		case 15:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p1_plate_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p1_plate_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p2_plate_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p2_plate_vacuum, IO_ON));
			}
			break;
			
		case 16:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p1_eject_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p1_eject_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p2_eject_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p2_eject_vacuum, IO_ON));
			}
			break;

		case 17:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p3_plate_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p3_plate_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p4_plate_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p4_plate_vacuum, IO_ON));
			}
			break;

		case 18:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p3_eject_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p3_eject_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_p4_eject_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_p4_eject_vacuum, IO_ON));
			}
			break;

		case 19:
			
			break;

		case 20:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_top_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_top_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_top_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_top_vacuum, IO_ON));
			}
			break;

		case 21:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_top_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_top_eject, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_top_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_top_eject, IO_ON));
			}
			break;

		case 22:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_top_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_top_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_top_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_top_vacuum, IO_ON));
			}
			break;

		case 23:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_top_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_top_eject, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_top_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_top_eject, IO_ON));
			}
			break;

		case 24:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum, IO_ON));
			}
			break;

		case 25:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_btm_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_btm_eject, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_btm_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_btm_eject, IO_ON));
			}
			break;

		case 26:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum, IO_ON));
			}
			break;

		case 27:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_btm_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_btm_eject, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_btm_eject, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_btm_eject, IO_ON));
			}
			break;

		case 28:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_top_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_top_vacuum_fwdback, IO_ON));
			}
			break;

		case 30:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_top_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_top_vacuum_fwdback, IO_ON));
			}
			break;

		case 31:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_top_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_top_vacuum_fwdback, IO_ON));
			}
			break;

		case 32:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_top_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_top_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_top_vacuum_fwdback, IO_ON));
			}
			break;

		case 33:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum_fwdback, IO_ON));
			}
			break;

		case 34:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_left_btm_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt1_right_btm_vacuum_fwdback, IO_ON));
			}
			break;

		case 35:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum_fwdback, IO_ON));
			}
			break;

		case 36:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_left_btm_vacuum_fwdback, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum_fwdback, !FAS_IO.get_out_bit(st_io.o_distribute_rbt2_right_btm_vacuum_fwdback, IO_ON));
			}
			break;

		case 37:
			
			break;

		case 38:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_uld_rbt_left_vacuum, !FAS_IO.get_out_bit(st_io.o_uld_rbt_left_vacuum, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_uld_rbt_right_vacuum, !FAS_IO.get_out_bit(st_io.o_uld_rbt_right_vacuum, IO_ON));
			}
			break;

		case 39:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_uld_rbt_left_updown, !FAS_IO.get_out_bit(st_io.o_uld_rbt_left_updown, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_uld_rbt_right_updown, !FAS_IO.get_out_bit(st_io.o_uld_rbt_right_updown, IO_ON));
			}
			break;

		case 40:
			if (nCol == 1)
			{
				FAS_IO.set_out_bit(st_io.o_uld_rbt_left_updown, !FAS_IO.get_out_bit(st_io.o_uld_rbt_left_updown, IO_ON));
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_uld_rbt_right_updown, !FAS_IO.get_out_bit(st_io.o_uld_rbt_right_updown, IO_ON));
			}
			break;
	}
	*/
}

////2014.1108 ysj
void CDialog_Manual_Part_1::OnBtnDensityLabelOut() 
{
	// TODO: Add your control notification handler code here
	SetTimer(TM_DENSITY_LABEL_OUT, 100, NULL);
	mn_density_label_no = 0;
	mn_move_step[mn_density_label_no] = 0;	
}

void CDialog_Manual_Part_1::OnBtnDensityLabelOut4() 
{
	// TODO: Add your control notification handler code here
	SetTimer(TM_DENSITY_LABEL_OUT, 100, NULL);
	mn_density_label_no = 3;		
	mn_move_step[mn_density_label_no] = 0;	
}

void CDialog_Manual_Part_1::OnBtnDensityLabelOut2() 
{
	// TODO: Add your control notification handler code here
	SetTimer(TM_DENSITY_LABEL_OUT, 100, NULL);
	mn_density_label_no = 1;
	mn_move_step[mn_density_label_no] = 0;	
}

void CDialog_Manual_Part_1::OnBtnDensityLabelOut5() 
{
	// TODO: Add your control notification handler code here
	SetTimer(TM_DENSITY_LABEL_OUT, 100, NULL);
	mn_density_label_no = 4;	
	mn_move_step[mn_density_label_no] = 0;	
}

void CDialog_Manual_Part_1::OnBtnDensityLabelOut3() 
{
	// TODO: Add your control notification handler code here
	SetTimer(TM_DENSITY_LABEL_OUT, 100, NULL);
	mn_density_label_no = 2;	
	mn_move_step[mn_density_label_no] = 0;	
}

void CDialog_Manual_Part_1::OnBtnDensityLabelOut6() 
{
	// TODO: Add your control notification handler code here
	SetTimer(TM_DENSITY_LABEL_OUT, 100, NULL);
	mn_density_label_no = 5;	
	mn_move_step[mn_density_label_no] = 0;	
}

////2014.1108 ysj
int CDialog_Manual_Part_1::DLabel_Print(int nMotor)//2014.1102
{
	int nFuncRet = RET_PROCEED;
	if(mn_stop_req == CTL_YES)
	{
		mn_stop_req = CTL_NO;
		mn_move_step[nMotor] = 0;
		KillTimer(TM_DENSITY_LABEL_OUT);
		return nFuncRet;

	}
	
	switch(mn_move_step[nMotor])
	{
////////////////////////////////////////////////////////////////////////// 
//용량라벨 PLC로 라벨 출력 동작 명령 보냄
	case 0:
		g_ioMgr.set_out_bit( st_io.o_density_lable_start[nMotor], IO_ON);
		
		if(st_basic.nLabelSelect== CTL_YES)//75mm
		{
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
		}
		else//55mm
		{
			g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
		}

		m_dwTimeWait[nMotor] = GetCurrentTime();
		mn_move_step[nMotor] = 100;
		break;

	case 100:
		if( g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) == IO_ON )
		{
			mn_move_step[nMotor] = 200;
			m_dwTimeWait[nMotor] = GetCurrentTime();
		}
		else
		{
			mn_move_step[nMotor] = 300;
		}
		break;

	case 200:
		if( GetCurrentTime() - m_dwTimeWait[nMotor] <= 0) m_dwTimeWait[nMotor] = GetCurrentTime();
		//if( GetCurrentTime() - m_dwTimeWait[nMotor] < 50) break;
		// kilee [2015.03.12]
		if( GetCurrentTime() - m_dwTimeWait[nMotor] < 500) break;
		if( g_ioMgr.get_in_bit(st_io.i_density_label_error_chk[nMotor], IO_ON ) == IO_ON )
		{
			CString strError="";
			strError.Format("7000%d0",nMotor);
			CTL_Lib.Alarm_Error_Occurrence( 5699, dWARNING, strError );
			mn_move_step[nMotor] = 100;
			mn_stop_req = CTL_YES;
		}
		else
		{
			mn_move_step[nMotor] = 300;
		}
		break;

	case 300:
		if( g_ioMgr.get_in_bit(st_io.i_density_label_ready_chk[nMotor], IO_ON ) == IO_ON )
		{
			mn_move_step[nMotor] = 500;
			m_dwTimeWait[nMotor] = GetCurrentTime();
		}
		else
		{
			if( GetCurrentTime() - m_dwTimeWait[nMotor] < 0) m_dwTimeWait[nMotor] = GetCurrentTime();
			if( GetCurrentTime() - m_dwTimeWait[nMotor] < 10000)
			{//700010 1 A "Density label1 time out from server."
				
// 				m_nRetryPickOut[nMotor]++;
// 				if( COMI.mn_run_status == dRUN && m_nRetryPickOut[nMotor] >= st_basic.mn_network_retry_cnt)
// 				{
// 					m_nRetryPickOut[nMotor] = 0;
// 					CString strError;
// 					strError.Format("7000%d0",nMotor);
// 					CTL_Lib.Alarm_Error_Occurrence( 5632, dWARNING, strError );
// 					m_dwTimeWait[nMotor] = GetCurrentTime();
// 					break;
// 				}
// 				else
// 				{
// 					m_dwTimeWait[nMotor] = GetCurrentTime();
// 				}
				mn_move_step[nMotor] = 400;
			}			
		}
		break;

	case 400:
		m_dwTimeWait[nMotor] = GetCurrentTime();
		mn_move_step[nMotor] = 300;
		break;
		
	case 500:
		if( GetCurrentTime() - m_dwTimeWait[nMotor] <= 0) m_dwTimeWait[nMotor] = GetCurrentTime();
		if( GetCurrentTime() - m_dwTimeWait[nMotor] < 100) break;
		mn_move_step[nMotor] = 600;
		break;

	case 600:		
		g_ioMgr.set_out_bit( st_io.o_density_lable_start[nMotor], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[nMotor], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_density_lable_length1[nMotor], IO_OFF);
		g_ioMgr.set_out_bit( st_io.o_density_lable_length2[nMotor], IO_OFF);
		mn_move_step[nMotor] = 0;
		KillTimer(TM_DENSITY_LABEL_OUT);
		break;
//////////////////////////////////////////////////////////////////////////
	}
	return nFuncRet;
}

int CDialog_Manual_Part_1::DRobot_Move(int nfeederNo)//2014.1102
{
	COMI.ml_axiscnt[M_ROBOT_DENSITY_INDEX] = 2;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][0] = M_D_ATTACH_X;
	COMI.mp_axisnum[M_ROBOT_DENSITY_INDEX][1] = M_D_ATTACH_Y;

	int nFuncRet = RET_PROCEED;
	int nRet1, nRet2;//, nRet3;
	
	if(mn_stop_req == CTL_YES)
	{
		mn_stop_req = CTL_NO;
		mn_robot_step[nfeederNo] = 0;
		KillTimer(TM_DENSITY_ROBOT_MOVE);
		return nFuncRet;		
	}

	switch( mn_robot_step[nfeederNo] )
	{
	case 0:	
		mn_robot_step[nfeederNo] = 100;
		break;

	case 100:
		{
			int nRet = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			if( nRet == BD_GOOD )
			{
				mn_robot_step[nfeederNo] = 200;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 520, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 200:
		{
			int nRet = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			if( nRet == BD_GOOD )
			{
				mn_robot_step[nfeederNo] = 300;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 521, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 300:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_DENSITY_F14 + st_basic.mn_device_type];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_robot_step[nfeederNo] = 400;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 522, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 400:
		{
			g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_ON );
			g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_ON );
		}
		mn_robot_step[nfeederNo] = 500;	
		break;	

	case 500:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_F14 + st_basic.mn_device_type], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
//				m_dwTime_Pick = GetCurrentTime();
//				mn_robot_step[nfeederNo] = 600;
				mn_robot_step[nfeederNo] = 700;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				mn_stop_req = CTL_YES;
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 523, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 524, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 600:
		if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < 100) break;		

		m_dwTime_Pick = GetCurrentTime();
		mn_robot_step[nfeederNo] = 700;
		break;

	case 700:
		if( ( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_ON) &&
			(g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_ON ||
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_ON) )
		{
			m_dwTime_Pick = GetCurrentTime();
			mn_robot_step[nfeederNo] = 900;
		}
		else if( GetCurrentTime() - m_dwTime_Pick > 5000)//st_time.n_limit_time[E_WAIT_FEEDER_VACUUM] )
		{
			m_dwTime_Pick = GetCurrentTime();
			mn_robot_step[nfeederNo] = 900;
		}		
		break;

	case 900:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_ON]) break;

		mn_robot_step[nfeederNo] = 910;
		break;

	case 910:
		g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type], IO_ON);
		g_ioMgr.set_out_bit( st_io.o_density_lable_pickon[st_basic.mn_device_type+3], IO_ON);
		mn_robot_step[nfeederNo] = 920;
		break;


	case 920:
		if( g_ioMgr.get_in_bit( st_io.i_density_label_complete_chk[st_basic.mn_device_type], IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_density_label_complete_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON)
		{
			mn_robot_step[nfeederNo] = 1000;
		}
		else if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON ||
			g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON)
		{
			mn_robot_step[nfeederNo] = 1000;
		}		
		break;

	case 1000:
		{
			nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{	
				m_dwTime_Pick = GetCurrentTime();
				mn_robot_step[nfeederNo] = 1100;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				mn_stop_req = CTL_YES;
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 526, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 527, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1100:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_ON]) break;

		if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_ON) == IO_OFF )
		{
			CString strJamCode;
			if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_ON) == IO_OFF )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
			}
			else
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_ON );
			}
			CTL_Lib.Alarm_Error_Occurrence( 528, dWARNING, strJamCode );
			mn_stop_req = CTL_YES;
// 			mn_robot_step[nfeederNo] = 1110;
			break;
		}

		if( g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_ON) == IO_OFF )
		{
			CString strJamCode;
			if( g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_ON) == IO_OFF )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
			}
			else
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
			}
			CTL_Lib.Alarm_Error_Occurrence( 528, dWARNING, strJamCode );
			mn_stop_req = CTL_YES;
// 			mn_robot_step[nfeederNo] = 1110;
			break;
		}

		if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON ||
			g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type+3], IO_ON) == IO_ON)
		{
			CString strJamCode;
			//700010 1 A "Density label1 time out from server."
			if( g_ioMgr.get_in_bit( st_io.i_density_label_error_chk[st_basic.mn_device_type], IO_ON) == IO_ON )
				strJamCode.Format("7000%d0", st_basic.mn_device_type);
			else
				strJamCode.Format("7000%d0", st_basic.mn_device_type+3);
			
			CTL_Lib.Alarm_Error_Occurrence( 528, dWARNING, strJamCode );
			mn_stop_req = CTL_YES;
// 			mn_robot_step[nfeederNo] = 1110;
			break;
		}

// 		g_ioMgr.set_out_bit( st_io.o_density_plate_btm_blow_onoff[st_basic.mn_device_type], IO_OFF );
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_btm_blow_onoff[st_basic.mn_device_type+3], IO_OFF );
// 		
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_blow_onoff[st_basic.mn_device_type], IO_OFF );
// 		g_ioMgr.set_out_bit( st_io.o_density_plate_blow_onoff[st_basic.mn_device_type+3], IO_OFF );

		mn_robot_step[nfeederNo] = 1200;
		break;

//////////////////////////////////////////////////////////////////////////
//용량라벨 에러 처리
	case 1110:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_ERROR_POS];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_ERROR_POS];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;
			
			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nRunSpeed;
			dSpeedRatio[1] = (double)st_basic.nRunSpeed;
			dSpeedRatio[2] = (double)st_basic.nRunSpeed;
			
			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_robot_step[nfeederNo] = 1120;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 1120:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_ERR], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_ERR], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{			
				mn_robot_step[nfeederNo] = 1130;			
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				mn_stop_req = CTL_YES;
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1130:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		m_dwTime_Pick = GetCurrentTime();
		mn_robot_step[nfeederNo] = 1140;
		break;
		
	case 1140:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_OFF]) break;
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_ON );
		mn_robot_step[nfeederNo] = 1150;
		break;

	case 1150:
		{
			int nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			int nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );
				mn_robot_step[nfeederNo] = 1160;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				mn_stop_req = CTL_YES;
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 535, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 536, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1160:
		{
			CString strJamCode;
			if( g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum1_chk, IO_ON );
				if (st_handler.cwnd_list != NULL)
				{
					CTL_Lib.Alarm_Error_Occurrence( 537, dWARNING, strJamCode );
					sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				mn_stop_req = CTL_YES;
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_left_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_left_vacuum2_chk, IO_ON );
				if (st_handler.cwnd_list != NULL)
				{
					CTL_Lib.Alarm_Error_Occurrence( 538, dWARNING, strJamCode );
					sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				mn_stop_req = CTL_YES;
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum1_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum1_chk, IO_ON );
				if (st_handler.cwnd_list != NULL)
				{
					CTL_Lib.Alarm_Error_Occurrence( 539, dWARNING, strJamCode );
					sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				mn_stop_req = CTL_YES;
				break;
			}
			if( st_basic.n_mode_device && g_ioMgr.get_in_bit(st_io.i_density_rbt_right_vacuum2_chk, IO_OFF) == IO_ON )
			{
				strJamCode = GetJamCode_Work_Time(st_io.i_density_rbt_right_vacuum2_chk, IO_ON );
				if (st_handler.cwnd_list != NULL)
				{
					CTL_Lib.Alarm_Error_Occurrence( 540, dWARNING, strJamCode );
					sprintf( st_msg.c_abnormal_msg, "%s", strJamCode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				mn_stop_req = CTL_YES;
				break;
			}
			
			////2014,1205
			//job.feeder_step = FEEDER_PICK_THROW_Z_FINISH;
			mn_robot_step[nfeederNo] = 100;
			////
		}
		break;
//////////////////////////////////////////////////////////////////////////


	case 1200:
		{
			double m_dPos[2];
			m_dPos[0] = st_motor[M_D_ATTACH_X].d_pos[P_XY_PLACE_DENSITY];
			m_dPos[1] = st_motor[M_D_ATTACH_Y].d_pos[P_XY_PLACE_DENSITY];
			long lAxis[2];
			lAxis[0] = M_D_ATTACH_X;
			lAxis[1] = M_D_ATTACH_Y;

			double dSpeedRatio[3];
			dSpeedRatio[0] = (double)st_basic.nManualSpeed;
			dSpeedRatio[1] = (double)st_basic.nManualSpeed;
			dSpeedRatio[2] = (double)st_basic.nManualSpeed;

			int nRet = CTL_Lib.Linear_Move( M_ROBOT_DENSITY_INDEX, 2, lAxis, m_dPos, dSpeedRatio );
			if( nRet == BD_GOOD )
			{
				mn_robot_step[nfeederNo] = 1300;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 529, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
		
	case 1300:
		{
			nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_PLACE_F], (int)st_basic.nManualSpeed );

			if( nRet1 == BD_GOOD )
			{
				mn_robot_step[nfeederNo] = 1400;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				mn_stop_req = CTL_YES;
				CTL_Lib.Alarm_Error_Occurrence( 533, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1400:
		{
			nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_PLACE_F], (int)st_basic.nManualSpeed );
			
			if( nRet1 == BD_GOOD)
			{
				mn_robot_step[nfeederNo] = 1500;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				mn_stop_req = CTL_YES;
				CTL_Lib.Alarm_Error_Occurrence( 534, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
 
	case 1500:
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_vacuum2, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum1, IO_OFF );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_vacuum2, IO_OFF );
		m_dwTime_Pick = GetCurrentTime();
		mn_robot_step[nfeederNo] = 1600;
		break;

	case 1600:
		if( GetCurrentTime() - m_dwTime_Pick < 0) m_dwTime_Pick = GetCurrentTime();
		else if( GetCurrentTime() - m_dwTime_Pick < st_time.n_wait_time[E_WAIT_FEEDER_VACUUM][IO_OFF]) break;
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_ON );
		g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_ON );

		mn_robot_step[nfeederNo] = 1700;
		break;

	case 1700:
		{
			nRet1 = CTL_Lib.Single_Move( M_D_ATTACH_Z1, st_motor[ M_D_ATTACH_Z1].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_D_ATTACH_Z2, st_motor[ M_D_ATTACH_Z2].d_pos[P_Z_DENSITY_SAFETY], (int)st_basic.nManualSpeed );

			if (nRet1 == BD_GOOD && nRet2 == BD_GOOD)//if (nRet_1 == CMM_TRUE && nRet_2 == CMM_TRUE)
			{
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_left_blow2, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow1, IO_OFF );
				g_ioMgr.set_out_bit( st_io.o_density_rbt_right_blow2, IO_OFF );
				
				mn_robot_step[nfeederNo] = 1800;
			}
			else if( (nRet1 == BD_ERROR || nRet1 == BD_SAFETY) || (nRet2 == BD_ERROR || nRet2 == BD_SAFETY) )
			{
				mn_stop_req = CTL_YES;
				if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
					CTL_Lib.Alarm_Error_Occurrence( 535, CTL_dWARNING, alarm.mstr_code );
				else
					CTL_Lib.Alarm_Error_Occurrence( 536, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 1800:
		KillTimer(TM_DENSITY_ROBOT_MOVE);
		mn_robot_step[nfeederNo] = 0;
		break;

	}
	return nFuncRet;
	
}

void CDialog_Manual_Part_1::Distribution_Robot_Move1()
{
	int nRet1,nRet2,nRet3,nRet4,nRet5;

	if(mn_stop_req == CTL_YES)
	{
		mn_stop_req = CTL_NO;
		mn_labrl_robot_step_1 = 0;
		COMI.Set_MotStop( true, M_BCR_ATTACH1_Z1 );
		COMI.Set_MotStop( true, M_BCR_ATTACH1_Z2 );
		COMI.Set_MotStop( true, M_BCR_ATTACH1_X );

		KillTimer(TM_DISTRIBUTION_ROBOT_MOVE1);
		return;		
	}

	switch( mn_labrl_robot_step_1 )
	{
	case 0:
		mn_labrl_robot_step_1 = 100;
		break;

	case 100:
		mn_labrl_robot_step_1 = 200;
		break;

	case 200:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			if( nRet == BD_GOOD )
			{
				mn_labrl_robot_step_1 = 300;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 250, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 300:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			if( nRet == BD_GOOD )
			{
				mn_labrl_robot_step_1 = 400;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 251, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 400:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[0], IO_OFF);
	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[1], IO_OFF);

			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 500;
		}
		break;

	case 500:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF);
			
			nRet4 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON);
			nRet5 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_OFF);
			
			if(nRet1 == IO_ON && nRet2 != IO_OFF && nRet4 == IO_ON && nRet5 != IO_OFF)
			{
				mn_labrl_robot_step_1 = 600;
			}
		}
		break;
 

	case 600:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[0], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[0], IO_ON);
			
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[1], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[1], IO_ON);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 700;
		}
		break;
		
	case 700:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
		
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_chk[0], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_chk[0], IO_OFF);
			
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_chk[1], IO_OFF);
			
			if(nRet1 == IO_OFF && nRet2 == IO_OFF && nRet3 == IO_OFF && nRet4 == IO_OFF)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[1], IO_OFF);
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_1 = 800;
			}
		}
		break;

	case 800://R축 실린더
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
	
			for(int i=0; i<2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[i], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[i], IO_ON);

			}
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 900;
		}
		break;

	case 900:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
				
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_ON);
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_ON);
			
			if(nRet1 == IO_OFF && nRet2 == IO_ON && nRet3 == IO_OFF && nRet4 == IO_ON)
			{
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_1 = 1200;
			}
		}
		break;

	case 1000:
		{
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R1, st_motor[M_BCR_ATTACH2_R1].d_pos[P_R_BCR_PICK], st_basic.nManualSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R2, st_motor[M_BCR_ATTACH2_R2].d_pos[P_R_BCR_PICK], st_basic.nManualSpeed );
// 			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
// 			{
// 				mn_labrl_robot_step_1 = 1200;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 252, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;

	case 1200:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PICK], st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[0], IO_OFF);
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[1], IO_OFF);

	
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[0], IO_ON);		
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[1], IO_ON);

				mn_labrl_robot_step_1 = 1500;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 254, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 1500:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_PICK], st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_PICK], st_basic.nManualSpeed );

			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{


				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[0], IO_ON);
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[0], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[0], IO_ON);		
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[1], IO_ON);
//				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[1], IO_ON);	
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[1], IO_ON);	
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_1 = 1700;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 255, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
		
	case 1700:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_chk[0], IO_ON);
//  			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_chk[0], IO_ON);
			
			nRet3 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_chk[1], IO_ON);
//  			nRet4 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_chk[1], IO_ON);
			
			if(nRet1 == IO_ON && /*nRet2 == IO_ON &&*/ nRet3 == IO_ON /*&& nRet4 == IO_ON*/)
			{
				mn_labrl_robot_step_1 = 1800;
			}
		}
		break;

	case 1800:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1 , st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2 , st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_labrl_robot_step_1 = 1810;
				m_dwTime_Pick = GetCurrentTime();
// 				mn_labrl_robot_step_1 = 0;
// 				KillTimer(TM_DISTRIBUTION_ROBOT_MOVE1);
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[0], IO_OFF);		
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[1], IO_OFF);	

				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[0], IO_OFF);		
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[1], IO_OFF);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 1810://R축 실린더
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			for(int i=0; i<2; i++)
			{
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[i], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[i], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[i], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[i], IO_OFF);
			}
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 1820;
		}
		break;
		
	case 1820:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_ON);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_OFF);
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_ON);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_OFF);
			
			if(nRet1 == IO_ON && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_OFF)
			{
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_1 = 1900;
			}
		}
		break;
	case 1900:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_X, st_motor[M_BCR_ATTACH1_X].d_pos[P_X_BCR_PLACE], st_basic.nManualSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R1, st_motor[M_BCR_ATTACH2_R1].d_pos[P_R_BCR_PLACE], st_basic.nManualSpeed );
// 			nRet3 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R2, st_motor[M_BCR_ATTACH2_R2].d_pos[P_R_BCR_PLACE], st_basic.nManualSpeed );

			if( nRet1 == BD_GOOD/* && nRet2 == BD_GOOD && nRet3 == BD_GOOD */)
			{
				
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_1 = 2000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY/* || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY */)
			{
				CTL_Lib.Alarm_Error_Occurrence( 265, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
 
	case 2000:
		{
			nRet1 = CTL_Lib.Single_Move( M_INDEX_X, st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS], st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_labrl_robot_step_1 = 2200;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 266, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;	

	case 2100:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON);
			
			if(nRet1 == IO_ON && nRet2 == IO_ON)
			{
				mn_labrl_robot_step_1 = 2200;
			}
		}
		break;

	case 2200:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_DOWN], st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_DOWN], st_basic.nManualSpeed );
			
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				g_ioMgr.set_out_bit( st_io.o_top_box_up_clamp[0], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_top_box_dn_clamp[0], IO_OFF);
				mn_labrl_robot_step_1 = 2300;
			}
			else if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 267, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 2300:
		{
			nRet1 = CTL_Lib.Single_Move( M_INDEX_ROLLER1, st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_POS], st_basic.nRunSpeed );
			
			if(nRet1 == BD_GOOD)
			{
				g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[0][0], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[0][1], IO_ON);
				mn_labrl_robot_step_1 = 2400;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 267, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}

		break;

	case 2400:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[0], IO_ON);		
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[1], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[0][0], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[0][1], IO_ON);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 2500;
		}
		break;

	case 2500:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_ON);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_ON);
			nRet3 = g_ioMgr.get_in_bit(st_io.i_rolling_push_fwd_chk[0][0], IO_ON);
			nRet4 = g_ioMgr.get_in_bit(st_io.i_rolling_push_fwd_chk[0][1], IO_ON);
			
			if(nRet1 == IO_OFF && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_ON)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[0], IO_ON);		
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[1], IO_ON);
				mn_labrl_robot_step_1 = 2600;
			}			
		}
		break;

	case 2600:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[0], IO_OFF);	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum[1], IO_OFF);
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[0], IO_OFF);	
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum[1], IO_OFF);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 2700;
		}
		break;
		
	case 2700:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[0], IO_ON);	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_eject[1], IO_ON);
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[0], IO_ON);	
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_eject[1], IO_ON);
			
			mn_labrl_robot_step_1 = 2800;
			m_dwTime_Pick = GetCurrentTime();
		}
		break;

	case 2800:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[0], IO_OFF);		
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_btm_vacuum_fwdback[1], IO_OFF);

			mn_labrl_robot_step_1 = 2810;
			m_dwTime_Pick = GetCurrentTime();
		}
		break;

	case 2810:
		if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
		else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

		nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[0], IO_OFF);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[1], IO_OFF);
		
		if(nRet1 == IO_ON && nRet2 == IO_ON)
		{
			mn_labrl_robot_step_1 = 2820;
		}

		break;

	case 2820:
		{	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[0], IO_OFF);		
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_top_vacuum_fwdback[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[0][0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[0][1], IO_OFF);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_1 = 2830;
		}
		break;

	case 2830:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[0], IO_OFF);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_top_vacuum_bwd_chk[1], IO_OFF);
			nRet3 = g_ioMgr.get_in_bit(st_io.i_rolling_push_bwd_chk[0][0], IO_OFF);
			nRet4 = g_ioMgr.get_in_bit(st_io.i_rolling_push_bwd_chk[0][1], IO_OFF);
			
			if(nRet1 == IO_ON && nRet2 == IO_ON && nRet3 == IO_ON && nRet4 == IO_ON)
			{
				mn_labrl_robot_step_1 = 3300;
				m_dwTime_Pick = GetCurrentTime();
			}
		}
		break;


	case 3300:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z1, st_motor[M_BCR_ATTACH1_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH1_Z2, st_motor[M_BCR_ATTACH1_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet3 = CTL_Lib.Single_Move( M_INDEX_ROLLER1, st_motor[M_INDEX_ROLLER1].d_pos[P_ROLLER_SAGETY], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD && nRet3 == BD_GOOD )
			{
				g_ioMgr.set_out_bit( st_io.o_top_box_up_clamp[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_top_box_dn_clamp[0], IO_ON);
				mn_labrl_robot_step_1 = 0;
				KillTimer(TM_DISTRIBUTION_ROBOT_MOVE1);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 272, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
 	}
}

void CDialog_Manual_Part_1::OnBtnDistributionAttachRobot2() 
{
	// TODO: Add your control notification handler code here
	mn_labrl_robot_step_2 = 0;
	mn_retry_cnt = 0;
	SetTimer(TM_DISTRIBUTION_ROBOT_MOVE2, 100, NULL);	
}

void CDialog_Manual_Part_1::Distribution_Robot_Move2()
{
	int nRet1,nRet2,nRet3,nRet4,nRet5,nRet6,nRet7,nRet8;

	if(mn_stop_req == CTL_YES)
	{
		mn_stop_req = CTL_NO;
		mn_labrl_robot_step_2 = 0;
		COMI.Set_MotStop( true, M_BCR_ATTACH2_Z1 );
		COMI.Set_MotStop( true, M_BCR_ATTACH2_Z2 );
		COMI.Set_MotStop( true, M_BCR_ATTACH2_X );
		
		KillTimer(TM_DISTRIBUTION_ROBOT_MOVE2);
		return;
		
	}
	switch( mn_labrl_robot_step_2 )
	{
	case 0:
		mn_labrl_robot_step_2 = 100;
		break;

	case 100:
		mn_labrl_robot_step_2 = 200;
		break;

	case 200:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			if( nRet == BD_GOOD )
			{
				mn_labrl_robot_step_2 = 300;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 250, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 300:
		{
			int nRet = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			if( nRet == BD_GOOD )
			{
				mn_labrl_robot_step_2 = 400;
			}
			else if( nRet == BD_ERROR || nRet == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 251, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 400:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[0], IO_OFF);
	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[1], IO_OFF);

			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 500;
		}
		break;

	case 500:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF);
			
			nRet4 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON);
			nRet5 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_OFF);
			
			if(nRet1 == IO_ON && nRet2 != IO_OFF && nRet4 == IO_ON && nRet5 != IO_OFF)
			{
				mn_labrl_robot_step_2 = 600;
			}
		}
		break;
 

	case 600:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[0], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[0], IO_ON);
			
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[1], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[1], IO_ON);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 700;
		}
		break;
		
	case 700:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
		
			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_chk[0], IO_OFF);
			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_chk[0], IO_OFF);
			
			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_chk[1], IO_OFF);
			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_chk[1], IO_OFF);
			
			if(nRet1 == IO_OFF && nRet2 == IO_OFF && nRet3 == IO_OFF && nRet4 == IO_OFF)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[0], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[1], IO_OFF);
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_2 = 800;
			}
		}
		break;

	case 800://R축 실린더
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
	
			for(int i=0; i<2; i++)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[i], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[i], IO_ON);
			}
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 900;
		}
		break;

	case 900:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
				
			nRet5 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_OFF);
			nRet6 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_ON);
			nRet7 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_OFF);
			nRet8 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_ON);
			
			if(nRet5 == IO_OFF && nRet6 == IO_ON && nRet7 == IO_OFF && nRet8 == IO_ON)
			{
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_2 = 1200;
			}
		}
		break;

	case 1000:
		{
// 			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R1, st_motor[M_BCR_ATTACH2_R1].d_pos[P_R_BCR_PICK], st_basic.nManualSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R2, st_motor[M_BCR_ATTACH2_R2].d_pos[P_R_BCR_PICK], st_basic.nManualSpeed );
// 			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
// 			{
// 				mn_labrl_robot_step_2 = 1200;
// 			}
// 			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 252, CTL_dWARNING, alarm.mstr_code );
// 			}
		}
		break;

	case 1200:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PICK], st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[2], IO_OFF);
				g_ioMgr.set_out_bit(st_io.o_distribute_plate_vacuum[3], IO_OFF);

	
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[0], IO_ON);		
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[1], IO_ON);

				mn_labrl_robot_step_2 = 1500;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 254, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 1500:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_PICK], st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_PICK], st_basic.nManualSpeed );

			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{


				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[0], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[0], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[2], IO_ON);		
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[1], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[1], IO_ON);	
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[3], IO_ON);	
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_2 = 1700;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 255, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
		
	case 1700:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_chk[0], IO_ON);
//  			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_chk[0], IO_ON);
			
			nRet3 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_chk[1], IO_ON);
//  			nRet4 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_btm_vacuum_chk[1], IO_ON);
			
			if(nRet1 == IO_ON && /*nRet2 == IO_ON &&*/ nRet3 == IO_ON /*&& nRet4 == IO_ON*/)
			{
				mn_labrl_robot_step_2 = 1800;
			}
		}
		break;

	case 1800:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1 , st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2 , st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nManualSpeed );
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				mn_labrl_robot_step_2 = 1810;
				m_dwTime_Pick = GetCurrentTime();
// 				mn_labrl_robot_step_2 = 0;
// 				KillTimer(TM_DISTRIBUTION_ROBOT_MOVE1);
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[2], IO_OFF);		
				g_ioMgr.set_out_bit( st_io.o_distribute_eject_vacuum[3], IO_OFF);	

				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[0], IO_OFF);		
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[1], IO_OFF);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 260, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 1810://R축 실린더
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			for(int i=0; i<2; i++)
			{
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[i], IO_ON);
// 				g_ioMgr.set_out_bit( st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[i], IO_OFF);
				
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[i], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[i], IO_OFF);
			}
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 1820;
		}
		break;
		
	case 1820:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
// 			nRet1 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[0], IO_ON);
// 			nRet2 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[0], IO_OFF);
// 			nRet3 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_fwd_chk[1], IO_ON);
// 			nRet4 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt1_box_rotator_bwd_chk[1], IO_OFF);
			
			nRet5 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[0], IO_ON);
			nRet6 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[0], IO_OFF);
			nRet7 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_fwd_chk[1], IO_ON);
			nRet8 =  g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_box_rotator_bwd_chk[1], IO_OFF);
			
			if(nRet5 == IO_ON && nRet6 == IO_OFF && nRet7 == IO_ON && nRet8 == IO_OFF)
			{
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_2 = 1900;
			}
		}
		break;
	case 1900:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_X, st_motor[M_BCR_ATTACH2_X].d_pos[P_X_BCR_PLACE], st_basic.nManualSpeed );
// 			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R1, st_motor[M_BCR_ATTACH2_R1].d_pos[P_R_BCR_PLACE], st_basic.nManualSpeed );
// 			nRet3 = CTL_Lib.Single_Move( M_BCR_ATTACH2_R2, st_motor[M_BCR_ATTACH2_R2].d_pos[P_R_BCR_PLACE], st_basic.nManualSpeed );

			if( nRet1 == BD_GOOD/* && nRet2 == BD_GOOD && nRet3 == BD_GOOD */)
			{
				
				m_dwTime_Pick = GetCurrentTime();
				mn_labrl_robot_step_2 = 2000;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY/* || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY */)
			{
				CTL_Lib.Alarm_Error_Occurrence( 265, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
 
	case 2000:
		{
			nRet1 = CTL_Lib.Single_Move( M_INDEX_X, st_motor[M_INDEX_X].d_pos[P_X_ROLLER_POS], st_basic.nManualSpeed );
			if( nRet1 == BD_GOOD )
			{
				mn_labrl_robot_step_2 = 2200;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY )
			{
				CTL_Lib.Alarm_Error_Occurrence( 266, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;	

	case 2100:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 100) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON);
			
			if(nRet1 == IO_ON && nRet2 == IO_ON)
			{
				mn_labrl_robot_step_2 = 2200;
			}
		}
		break;

	case 2200:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_DOWN], st_basic.nManualSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_DOWN], st_basic.nManualSpeed );
			
			if(nRet1 == BD_GOOD && nRet2 == BD_GOOD)
			{
				g_ioMgr.set_out_bit( st_io.o_top_box_up_clamp[1], IO_ON);
				g_ioMgr.set_out_bit( st_io.o_top_box_dn_clamp[1], IO_OFF);
				mn_labrl_robot_step_2 = 2300;
			}
			else if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 267, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;

	case 2300:
		{
			nRet1 = CTL_Lib.Single_Move( M_INDEX_ROLLER2, st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_POS], st_basic.nRunSpeed );
			
			if(nRet1 == BD_GOOD)
			{
				mn_labrl_robot_step_2 = 2400;
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 267, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}

		break;

	case 2400:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[0], IO_ON);		
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[1], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[1][0], IO_ON);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[1][1], IO_ON);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 2500;
		}
		break;

	case 2500:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick
			
			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_ON);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_ON);
			nRet3 = g_ioMgr.get_in_bit(st_io.i_rolling_push_fwd_chk[1][0], IO_ON);
			nRet4 = g_ioMgr.get_in_bit(st_io.i_rolling_push_fwd_chk[1][1], IO_ON);
			
			if(nRet1 == IO_OFF && nRet2 == IO_OFF && nRet3 == IO_ON && nRet4 == IO_ON)
			{
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[0], IO_ON);		
				g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[1], IO_ON);
				mn_labrl_robot_step_2 = 2600;
			}			
		}
		break;

	case 2600:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[0], IO_OFF);	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum[1], IO_OFF);
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[0], IO_OFF);	
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum[1], IO_OFF);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 2700;
		}
		break;
		
	case 2700:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[0], IO_ON);	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_eject[1], IO_ON);
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[0], IO_ON);	
// 			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_eject[1], IO_ON);
			
			mn_labrl_robot_step_2 = 2800;
			m_dwTime_Pick = GetCurrentTime();
		}
		break;

	case 2800:
		{
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[0], IO_OFF);		
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_btm_vacuum_fwdback[1], IO_OFF);

			mn_labrl_robot_step_2 = 2810;
			m_dwTime_Pick = GetCurrentTime();
		}
		break;

	case 2810:
		if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
		else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

		nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[0], IO_OFF);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[1], IO_OFF);
		
		if(nRet1 == IO_ON && nRet2 == IO_ON)
		{
			mn_labrl_robot_step_2 = 2820;
		}

		break;

	case 2820:
		{	
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[0], IO_OFF);		
			g_ioMgr.set_out_bit( st_io.o_distribute_rbt2_top_vacuum_fwdback[1], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[1][0], IO_OFF);
			g_ioMgr.set_out_bit( st_io.o_rolling_push_fwdback[1][1], IO_OFF);
			
			m_dwTime_Pick = GetCurrentTime();
			mn_labrl_robot_step_2 = 2830;
		}
		break;

	case 2830:
		{
			if( GetCurrentTime() - m_dwTime_Pick < 0)  m_dwTime_Pick = GetCurrentTime();//2014.1102 mn_PickStep -> m_dwTime_Pick
			else if( GetCurrentTime() - m_dwTime_Pick < 400) break;//2014.1102 mn_PickStep -> m_dwTime_Pick

			nRet1 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[0], IO_OFF);
			nRet2 = g_ioMgr.get_in_bit(st_io.i_distribute_rbt2_top_vacuum_bwd_chk[1], IO_OFF);
			nRet3 = g_ioMgr.get_in_bit(st_io.i_rolling_push_bwd_chk[1][0], IO_OFF);
			nRet4 = g_ioMgr.get_in_bit(st_io.i_rolling_push_bwd_chk[1][1], IO_OFF);
			
			if(nRet1 == IO_ON && nRet2 == IO_ON && nRet3 == IO_ON && nRet4 == IO_ON)
			{
				mn_labrl_robot_step_2 = 3300;
				m_dwTime_Pick = GetCurrentTime();
			}
		}
		break;


	case 3300:
		{
			nRet1 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z1, st_motor[M_BCR_ATTACH2_Z1].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet2 = CTL_Lib.Single_Move( M_BCR_ATTACH2_Z2, st_motor[M_BCR_ATTACH2_Z2].d_pos[P_Z_BCR_SAFETY], st_basic.nRunSpeed );
			nRet3 = CTL_Lib.Single_Move( M_INDEX_ROLLER2, st_motor[M_INDEX_ROLLER2].d_pos[P_ROLLER_SAGETY], st_basic.nRunSpeed );
			if( nRet1 == BD_GOOD && nRet2 == BD_GOOD && nRet3 == BD_GOOD )
			{
				g_ioMgr.set_out_bit( st_io.o_top_box_up_clamp[1], IO_OFF);
				g_ioMgr.set_out_bit( st_io.o_top_box_dn_clamp[1], IO_ON);
				mn_labrl_robot_step_2 = 0;
				KillTimer(TM_DISTRIBUTION_ROBOT_MOVE2);
			}
			else if( nRet1 == BD_ERROR || nRet1 == BD_SAFETY || nRet2 == BD_ERROR || nRet2 == BD_SAFETY || nRet3 == BD_ERROR || nRet3 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence( 272, CTL_dWARNING, alarm.mstr_code );
				mn_stop_req = CTL_YES;
			}
		}
		break;
 	}
}

////
void CDialog_Manual_Part_1::OnBtnDensityAttachRobot1() 
{
	// TODO: Add your control notification handler code here
	mn_manual_device_type = 0;
	mn_density_feeder_no = 0;
	mn_robot_step[mn_density_feeder_no] = 0;
	SetTimer(TM_DENSITY_ROBOT_MOVE, 100, NULL);	
}

void CDialog_Manual_Part_1::OnBtnDensityAttachRobot2() 
{
	// TODO: Add your control notification handler code here
	mn_manual_device_type = 1;
	mn_density_feeder_no = 1;
	mn_robot_step[mn_density_feeder_no] = 0;
	SetTimer(TM_DENSITY_ROBOT_MOVE, 100, NULL);	
}

void CDialog_Manual_Part_1::OnBtnDensityAttachRobot3() 
{
	// TODO: Add your control notification handler code here
	mn_manual_device_type = 2;	
	mn_density_feeder_no = 2;
	mn_robot_step[mn_density_feeder_no] = 0;
	SetTimer(TM_DENSITY_ROBOT_MOVE, 100, NULL);	
}

void CDialog_Manual_Part_1::OnBtnDistributionAttachRobot1() 
{
	// TODO: Add your control notification handler code here
	mn_labrl_robot_step_1 =0;
	mn_retry_cnt = 0;
	SetTimer(TM_DISTRIBUTION_ROBOT_MOVE1, 100, NULL);	
}


////
