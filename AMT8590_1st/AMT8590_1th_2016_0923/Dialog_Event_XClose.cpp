// Dialog_Event_XClose.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Event_XClose.h"
#include "MainFrm.h"
#include "FastechPublic_IO.h"
#include "IO_Manager.h"
#include "SrcPart/APartHandler.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Event_XClose dialog


CDialog_Event_XClose::CDialog_Event_XClose(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Event_XClose::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Event_XClose)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "XClose Event Message Box";
}


void CDialog_Event_XClose::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Event_XClose)
	DDX_Control(pDX, IDC_LABEL_EVENT_MSG, m_label_event_msg);
	DDX_Control(pDX, IDCANCLE, m_btn_cancle);
	DDX_Control(pDX, IDOK, m_btn_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Event_XClose, CDialog)
	//{{AFX_MSG_MAP(CDialog_Event_XClose)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCLE, OnCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Event_XClose message handlers

BOOL CDialog_Event_XClose::Create() 
{  
	return StandardDialog::Create(IDD);
}

void CDialog_Event_XClose::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Event_XClose::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	CRect r;
	GetWindowRect(&r);
	
	int w = r.Width(), h = r.Height();
	
	r.left = 1280 / 2 - w / 2;
	r.right = r.left + w;
	
	r.top = 1024 / 2 - h / 2;
	r.bottom = r.top + h;
	
	MoveWindow(&r);
}

void CDialog_Event_XClose::OnDestroy() 
{
	g_ioMgr.set_out_bit(st_io.o_buzzer_1, CTL_OFF);
	g_ioMgr.set_out_bit(st_io.o_buzzer_2, CTL_OFF);
	g_ioMgr.set_out_bit(st_io.o_buzzer_3, CTL_OFF);
	
	StandardDialog::OnDestroy();	
}

BOOL CDialog_Event_XClose::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	OnEventMsg_Label_Set();
	OnEventMsg_Text_Set();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDialog_Event_XClose::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialog_Event_XClose::OnEventMsg_Label_Set()
{
	m_label_event_msg.SetText("");
	m_label_event_msg.SetTextColor(RGB(250, 50, 50));
	m_label_event_msg.SetBkColor(RGB(181, 182, 140));
	m_label_event_msg.SetFontBold(FALSE);
	m_label_event_msg.SetFontName("MS Sans Serif");
	m_label_event_msg.SetFontSize(36);
}

void CDialog_Event_XClose::OnEventMsg_Text_Set()
{
	int i;
	CString strMsg;
	
	for (i = 0; i < 3; i++)
	{
		if (st_msg.mstr_event_xmsg[i] != "")
		{
			strMsg += st_msg.mstr_event_xmsg[i];
			strMsg += "\n";
		}
	}
	
	m_label_event_msg.SetText(strMsg);
}

#include "MesEvent.h"
void CDialog_Event_XClose::OnOK() 
{
	::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
	
	st_handler.mn_chk_lotend = CTL_REQ;

	st_mes_data.srt_LOT_ID = g_lotMgr.GetLotAt(0).GetLotID();
	st_mes_data.srt_RETRY = "0";
	st_mes_data.srt_RETRY_CNT = "0";

	evMes.OnPutToQueue(MES_PK_XCLOSE);

// 	g_lotMgr.DeleteLot( 0 );
// 	st_handler.mn_Lotcnt = 0;
}

void CDialog_Event_XClose::OnCancle() 
{
	::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
	
	g_handler.ClearRejAlarm();
	
}
// 
// BOOL CDialog_Event_XClose::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
// {
// 	// TODO: Add your specialized code here and/or call the base class
// 	
// 	return CDialog::Create(IDD, pParentWnd);
// }
