// Screen_Set_Network.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Set_Network.h"

// ******************************************************************************
// 대화 상자 클래스 추가
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_KeyPad.h"
// ******************************************************************************
#include "Dialog_Infor.h"
#include "MainFrm.h"
#include "SrcPart/APartHandler.h"
#include "Srcbase\ALocalization.h"

#include "FrontClient.h"
#include "BtmClient.h"
#include "8570Client.h"
#include "NextClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Network dialog
IMPLEMENT_DYNCREATE(CScreen_Set_Network, CFormView)


CScreen_Set_Network::CScreen_Set_Network()
	: CFormView(CScreen_Set_Network::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Set_Network)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Set_Network::~CScreen_Set_Network()
{
}

void CScreen_Set_Network::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Set_Network)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_CIP_SET_LABEL_PRINTER1, m_btn_cip_set_label_printer1);
	DDX_Control(pDX, IDC_BTN_TST_NEXT, m_btn_test_next);
	DDX_Control(pDX, IDC_BTN_TST_EC, m_btn_test_ec);
	DDX_Control(pDX, IDC_BTN_CIP_SET_EC, m_btn_cip_set_ec);
	DDX_Control(pDX, IDC_MSG_IP_FORMAT, m_msg_ip_format);
	DDX_Control(pDX, IDC_MSG_CPORT_FORMAT, m_msg_cport_format);
	DDX_Control(pDX, IDC_GROUP_CLIENT_FORMAT, m_group_client_format);
	DDX_Control(pDX, IDC_EDIT_CPORT_FORMAT, m_edit_cport_format_client);
	DDX_Control(pDX, IDC_CIP_FORMAT, m_cip_format);
	DDX_Control(pDX, IDC_BTN_TST_FORMAT, m_btn_test_format);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_FORMAT, m_btn_cport_set_format);
	DDX_Control(pDX, IDC_BTN_CIP_SET_FORMAT, m_btn_cip_set_format);
	DDX_Control(pDX, IDC_MSG_CPORT_LABEL_PRINTER4, m_msg_cport_label_printer4);
	DDX_Control(pDX, IDC_MSG_CPORT_LABEL_PRINTER3, m_msg_cport_label_printer3);
	DDX_Control(pDX, IDC_MSG_CPORT_LABEL_PRINTER2, m_msg_cport_label_printer2);
	DDX_Control(pDX, IDC_MSG_CPORT_BOTTOM, m_msg_cport_bottom);
	DDX_Control(pDX, IDC_MSG_IP_LABEL_PRINTER4, m_msg_ip_label_printer4);
	DDX_Control(pDX, IDC_MSG_IP_BOTTOM, m_msg_ip_bottom);
	DDX_Control(pDX, IDC_MSG_CPORT_VISION4, m_msg_cport_vision4);
	DDX_Control(pDX, IDC_MSG_CPORT_VISION3, m_msg_cport_vision3);
	DDX_Control(pDX, IDC_MSG_CPORT_VISION2, m_msg_cport_vision2);
	DDX_Control(pDX, IDC_MSG_CPORT_VISION1, m_msg_cport_vision1);
	DDX_Control(pDX, IDC_MSG_IP_LABEL_PRINTER3, m_msg_ip_label_printer3);
	DDX_Control(pDX, IDC_MSG_IP_LABEL_PRINTER2, m_msg_ip_label_printer2);
	DDX_Control(pDX, IDC_MSG_IP_VISION4, m_msg_ip_vision4);
	DDX_Control(pDX, IDC_MSG_IP_VISION3, m_msg_ip_vision3);
	DDX_Control(pDX, IDC_MSG_IP_VISION2, m_msg_ip_vision2);
	DDX_Control(pDX, IDC_MSG_IP_VISION1, m_msg_ip_vision1);
	DDX_Control(pDX, IDC_MSG_SPORT_BARCODE4, m_msg_sport_barcode4);
	DDX_Control(pDX, IDC_MSG_SPORT_BARCODE3, m_msg_sport_barcode3);
	DDX_Control(pDX, IDC_MSG_SPORT_BARCODE2, m_msg_sport_barcode2);
	DDX_Control(pDX, IDC_MSG_SPORT_BARCODE1, m_msg_sport_barcode1);
	DDX_Control(pDX, IDC_GROUP_VISION_CLIENT4, m_group_vision_client4);
	DDX_Control(pDX, IDC_GROUP_VISION_CLIENT3, m_group_vision_client3);
	DDX_Control(pDX, IDC_GROUP_VISION_CLIENT2, m_group_vision_client2);
	DDX_Control(pDX, IDC_GROUP_VISION_CLIENT1, m_group_vision_client1);
	DDX_Control(pDX, IDC_GROUP_CLIENT_BOTTOM, m_group_client_bottom);
	DDX_Control(pDX, IDC_GROUP_CLIENT_BARCODE_PRINTER4, m_group_client_bcr_printer4);
	DDX_Control(pDX, IDC_GROUP_CLIENT_BARCODE_PRINTER3, m_group_client_bcr_printer3);
	DDX_Control(pDX, IDC_GROUP_CLIENT_BARCODE_PRINTER2, m_group_client_bcr_printer2);
	DDX_Control(pDX, IDC_GROUP_CLIENT_BARCODE_PRINTER1, m_group_client_bcr_printer1);
	DDX_Control(pDX, IDC_EDIT_CPORT_NEXT, m_edit_cport_next);
	DDX_Control(pDX, IDC_EDIT_CPORT_LABEL_PRINTER4, m_edit_cport_label_printer4);
	DDX_Control(pDX, IDC_EDIT_CPORT_LABEL_PRINTER3, m_edit_cport_label_printer3);
	DDX_Control(pDX, IDC_EDIT_CPORT_LABEL_PRINTER2, m_edit_cport_label_printer2);
	DDX_Control(pDX, IDC_EDIT_CPORT_BOTTOM_CLIENT, m_edit_cport_bottom_client);
	DDX_Control(pDX, IDC_CIP_LABEL_PRINTER4, m_cip_label_printer4);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_LABEL_PRINTER4, m_btn_cport_set_label_printer4);
	DDX_Control(pDX, IDC_CIP_BOTTOM, m_cip_bottom);
	DDX_Control(pDX, IDC_CIP_8570, m_cip_8570);
	DDX_Control(pDX, IDC_BTN_TST_VISION4, m_btn_test_vision4);
	DDX_Control(pDX, IDC_BTN_TST_VISION3, m_btn_test_vision3);
	DDX_Control(pDX, IDC_BTN_TST_VISION2, m_btn_test_vision2);
	DDX_Control(pDX, IDC_BTN_TST_VISION1, m_btn_test_vision1);
	DDX_Control(pDX, IDC_BTN_TST_LABEL_PRINTER4, m_btn_test_label_printer4);
	DDX_Control(pDX, IDC_BTN_TST_LABEL_PRINTER3, m_btn_test_label_printer3);
	DDX_Control(pDX, IDC_BTN_TST_LABEL_PRINTER2, m_btn_test_label_printer2);
	DDX_Control(pDX, IDC_BTN_TST_LABEL_PRINTER1, m_btn_test_label_printer1);
	DDX_Control(pDX, IDC_BTN_TST_BOTTOM, m_btn_test_bottom);
	DDX_Control(pDX, IDC_BTN_TST_8570, m_btn_test_8570);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_BARCODE4, m_btn_sport_set_barcode4);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_BARCODE3, m_btn_sport_set_barcode3);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_BARCODE2, m_btn_sport_set_barcode2);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_BARCODE1, m_btn_sport_set_barcode1);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_LABEL_PRINTER3, m_btn_cport_set_label_printer3);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_LABEL_PRINTER2, m_btn_cport_set_label_printer2);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_BOTTOM, m_btn_cport_set_bottom);
	DDX_Control(pDX, IDC_BTN_CIP_SET_LABEL_PRINTER4, m_btn_cip_set_label_printer4);
	DDX_Control(pDX, IDC_BTN_CIP_SET_LABEL_PRINTER3, m_btn_cip_set_label_printer3);
	DDX_Control(pDX, IDC_BTN_CIP_SET_LABEL_PRINTER2, m_btn_cip_set_label_printer2);
	DDX_Control(pDX, IDC_BTN_CIP_SET_BOTTOM, m_btn_cip_set_bottom);
	DDX_Control(pDX, IDC_CIP_LABEL_PRINTER3, m_cip_label_printer3);
	DDX_Control(pDX, IDC_CIP_LABEL_PRINTER2, m_cip_label_printer2);
	DDX_Control(pDX, IDC_EDIT_CPORT_LABEL_PRINTER1, m_edit_cport_label_printer1);
	DDX_Control(pDX, IDC_EDIT_CPORT_VISION4, m_edit_cport_vis4);
	DDX_Control(pDX, IDC_EDIT_CPORT_VISION3, m_edit_cport_vis3);
	DDX_Control(pDX, IDC_EDIT_CPORT_VISION2, m_edit_cport_vis2);
	DDX_Control(pDX, IDC_EDIT_CPORT_VISION1, m_edit_cport_vis1);
	DDX_Control(pDX, IDC_EDIT_CPORT_EC, m_edit_cport_ec_client);
	DDX_Control(pDX, IDC_EDIT_CPORT_8570_CLIENT, m_edit_cport_8570_client);
	DDX_Control(pDX, IDC_CIP_VISION4, m_cip_vis4);
	DDX_Control(pDX, IDC_CIP_VISION3, m_cip_vis3);
	DDX_Control(pDX, IDC_CIP_VISION2, m_cip_vis2);
	DDX_Control(pDX, IDC_CIP_VISION1, m_cip_vis1);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_VISION4, m_btn_cport_set_vis4);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_VISION3, m_btn_cport_set_vis3);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_VISION2, m_btn_cport_set_vis2);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_VISION1, m_btn_cport_set_vis1);
	DDX_Control(pDX, IDC_BTN_CIP_SET_VISION4, m_btn_cip_set_vis4);
	DDX_Control(pDX, IDC_BTN_CIP_SET_VISION3, m_btn_cip_set_vis3);
	DDX_Control(pDX, IDC_BTN_CIP_SET_VISION2, m_btn_cip_set_vis2);
	DDX_Control(pDX, IDC_BTN_CIP_SET_VISION1, m_btn_cip_set_vis1);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_8570, m_btn_cport_set_8570);
	DDX_Control(pDX, IDC_BTN_CIP_SET_8570, m_btn_cip_set_8570);
	DDX_Control(pDX, IDC_MSG_CPORT_8570, m_msg_cport_8570);
	DDX_Control(pDX, IDC_MSG_IP_8570, m_msg_ip8570);
	DDX_Control(pDX, IDC_GROUP_CLIENT_8570, m_group_client_8570);
	DDX_Control(pDX, IDC_BTN_CIP_SET_NEXT, m_btn_cip_set_next);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_EC, m_btn_cport_set_ec);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_LABEL_PRINTER1, m_btn_cport_set_label_printer1);
	DDX_Control(pDX, IDC_BTN_CPORT_SET_NEXT, m_btn_cport_set_next);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_FRONT, m_btn_sport_set_front);
	DDX_Control(pDX, IDC_CIP_LABEL_PRINTER1, m_cip_label_printer1);
	DDX_Control(pDX, IDC_CIP_EC, m_cip_ec);
	DDX_Control(pDX, IDC_CIP_NEXT, m_cip_next);
	DDX_Control(pDX, IDC_MSG_IP_EC, m_msg_cip_ec);
	DDX_Control(pDX, IDC_MSG_CPORT_EC, m_msg_cport_ec);
	DDX_Control(pDX, IDC_MSG_SPORT_FRONT, m_msg_sport_front);
	DDX_Control(pDX, IDC_MSG_IP_LABEL_PRINTER1, m_msg_cip_label_printer1);
	DDX_Control(pDX, IDC_MSG_IP_NEXT, m_msg_cip_next);
	DDX_Control(pDX, IDC_MSG_CPORT_NEXT, m_msg_cport_next);
	DDX_Control(pDX, IDC_MSG_CPORT_LABEL_PRINTER1, m_msg_cport_label_printer1);
	DDX_Control(pDX, IDC_GROUP_CLIENT_NEXT, m_group_client_next);
	DDX_Control(pDX, IDC_GROUP_CLIENT_EC, m_group_client_ec);
	DDX_Control(pDX, IDC_GROUP_SERVER, m_group_server);
	DDX_Control(pDX, IDC_GROUP_SERIAL, m_group_serial);
	DDX_Control(pDX, IDC_GROUP_SERIAL_1, m_group_serial_1);
	DDX_Control(pDX, IDC_GROUP_SERIAL_2, m_group_serial_2);
	DDX_Control(pDX, IDC_GROUP_SERIAL_3, m_group_serial_3);
	DDX_Control(pDX, IDC_GROUP_SERIAL_4, m_group_serial_4);
	DDX_Control(pDX, IDC_GROUP_SERIAL_5, m_group_serial_5);
	DDX_Control(pDX, IDC_GROUP_SERIAL_6, m_group_serial_6);
	DDX_Control(pDX, IDC_GROUP_SERIAL_7, m_group_serial_7);
	DDX_Control(pDX, IDC_MSG_PORT_1, m_msg_port_1);
	DDX_Control(pDX, IDC_MSG_DATA_1, m_msg_data_1);
	DDX_Control(pDX, IDC_MSG_PARITY_1, m_msg_parity_1);
	DDX_Control(pDX, IDC_MSG_RATE_1, m_msg_rate_1);
	DDX_Control(pDX, IDC_MSG_STOP_1, m_msg_stop_1);
	DDX_Control(pDX, IDC_COMBO_PORT_1, m_cb_port_1);
	DDX_Control(pDX, IDC_COMBO_RATE_1, m_cb_rate_1);
	DDX_Control(pDX, IDC_COMBO_DATA_1, m_cb_data_1);	
	DDX_Control(pDX, IDC_COMBO_STOP_1, m_cb_stop_1);
	DDX_Control(pDX, IDC_COMBO_PARITY_1, m_cb_parity_1);
	DDX_Control(pDX, IDC_MSG_PORT_2, m_msg_port_2);
	DDX_Control(pDX, IDC_MSG_DATA_2, m_msg_data_2);
	DDX_Control(pDX, IDC_MSG_PARITY_2, m_msg_parity_2);
	DDX_Control(pDX, IDC_MSG_RATE_2, m_msg_rate_2);
	DDX_Control(pDX, IDC_MSG_STOP_2, m_msg_stop_2);
	DDX_Control(pDX, IDC_COMBO_PORT_2, m_cb_port_2);
	DDX_Control(pDX, IDC_COMBO_RATE_2, m_cb_rate_2);
	DDX_Control(pDX, IDC_COMBO_DATA_2, m_cb_data_2);
	DDX_Control(pDX, IDC_COMBO_STOP_2, m_cb_stop_2);
	DDX_Control(pDX, IDC_COMBO_PARITY_2, m_cb_parity_2);
	DDX_Control(pDX, IDC_MSG_PORT_3, m_msg_port_3);
	DDX_Control(pDX, IDC_MSG_DATA_3, m_msg_data_3);
	DDX_Control(pDX, IDC_MSG_PARITY_3, m_msg_parity_3);
	DDX_Control(pDX, IDC_MSG_RATE_3, m_msg_rate_3);
	DDX_Control(pDX, IDC_MSG_STOP_3, m_msg_stop_3);
	DDX_Control(pDX, IDC_COMBO_PORT_3, m_cb_port_3);
	DDX_Control(pDX, IDC_COMBO_RATE_3, m_cb_rate_3);
	DDX_Control(pDX, IDC_COMBO_DATA_3, m_cb_data_3);
	DDX_Control(pDX, IDC_COMBO_STOP_3, m_cb_stop_3);
	DDX_Control(pDX, IDC_COMBO_PARITY_3, m_cb_parity_3);
	DDX_Control(pDX, IDC_MSG_PORT_4, m_msg_port_4);
	DDX_Control(pDX, IDC_MSG_DATA_4, m_msg_data_4);
	DDX_Control(pDX, IDC_MSG_PARITY_4, m_msg_parity_4);
	DDX_Control(pDX, IDC_MSG_RATE_4, m_msg_rate_4);
	DDX_Control(pDX, IDC_MSG_STOP_4, m_msg_stop_4);
	DDX_Control(pDX, IDC_COMBO_PORT_4, m_cb_port_4);
	DDX_Control(pDX, IDC_COMBO_RATE_4, m_cb_rate_4);
	DDX_Control(pDX, IDC_COMBO_DATA_4, m_cb_data_4);
	DDX_Control(pDX, IDC_COMBO_STOP_4, m_cb_stop_4);
	DDX_Control(pDX, IDC_COMBO_PARITY_4, m_cb_parity_4);
	DDX_Control(pDX, IDC_MSG_PORT_5, m_msg_port_5);
	DDX_Control(pDX, IDC_MSG_DATA_5, m_msg_data_5);
	DDX_Control(pDX, IDC_MSG_PARITY_5, m_msg_parity_5);
	DDX_Control(pDX, IDC_MSG_RATE_5, m_msg_rate_5);
	DDX_Control(pDX, IDC_MSG_STOP_5, m_msg_stop_5);
	DDX_Control(pDX, IDC_COMBO_PORT_5, m_cb_port_5);
	DDX_Control(pDX, IDC_COMBO_RATE_5, m_cb_rate_5);
	DDX_Control(pDX, IDC_COMBO_DATA_5, m_cb_data_5);
	DDX_Control(pDX, IDC_COMBO_STOP_5, m_cb_stop_5);
	DDX_Control(pDX, IDC_COMBO_PARITY_5, m_cb_parity_5);
	DDX_Control(pDX, IDC_MSG_PORT_6, m_msg_port_6);
	DDX_Control(pDX, IDC_MSG_DATA_6, m_msg_data_6);
	DDX_Control(pDX, IDC_MSG_PARITY_6, m_msg_parity_6);
	DDX_Control(pDX, IDC_MSG_RATE_6, m_msg_rate_6);
	DDX_Control(pDX, IDC_MSG_STOP_6, m_msg_stop_6);
	DDX_Control(pDX, IDC_COMBO_PORT_6, m_cb_port_6);
	DDX_Control(pDX, IDC_COMBO_RATE_6, m_cb_rate_6);
	DDX_Control(pDX, IDC_COMBO_DATA_6, m_cb_data_6);
	DDX_Control(pDX, IDC_COMBO_STOP_6, m_cb_stop_6);
	DDX_Control(pDX, IDC_COMBO_PARITY_6, m_cb_parity_6);
	DDX_Control(pDX, IDC_MSG_PORT_7, m_msg_port_7);
	DDX_Control(pDX, IDC_MSG_DATA_7, m_msg_data_7);
	DDX_Control(pDX, IDC_MSG_PARITY_7, m_msg_parity_7);
	DDX_Control(pDX, IDC_MSG_RATE_7, m_msg_rate_7);
	DDX_Control(pDX, IDC_MSG_STOP_7, m_msg_stop_7);
	DDX_Control(pDX, IDC_COMBO_PORT_7, m_cb_port_7);
	DDX_Control(pDX, IDC_COMBO_RATE_7, m_cb_rate_7);
	DDX_Control(pDX, IDC_COMBO_DATA_7, m_cb_data_7);
	DDX_Control(pDX, IDC_COMBO_STOP_7, m_cb_stop_7);
	DDX_Control(pDX, IDC_COMBO_PARITY_7, m_cb_parity_7);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_1, m_btn_serial_connect_1);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_1, m_btn_serial_apply_1);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_2, m_btn_serial_connect_2);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_2, m_btn_serial_apply_2);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_3, m_btn_serial_connect_3);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_3, m_btn_serial_apply_3);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_4, m_btn_serial_connect_4);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_4, m_btn_serial_apply_4);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_5, m_btn_serial_connect_5);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_5, m_btn_serial_apply_5);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_6, m_btn_serial_connect_6);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_6, m_btn_serial_apply_6);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_7, m_btn_serial_connect_7);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_7, m_btn_serial_apply_7);

	////2014,1130
	DDX_Control(pDX, IDC_MSG_SPORT_VISION1, m_msg_sport_vision1);
	DDX_Control(pDX, IDC_MSG_SPORT_VISION2, m_msg_sport_vision2);
	DDX_Control(pDX, IDC_MSG_SPORT_VISION3, m_msg_sport_vision3);
	DDX_Control(pDX, IDC_MSG_SPORT_VISION4, m_msg_sport_vision4);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_VISION1, m_btn_sport_set_vision1);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_VISION2, m_btn_sport_set_vision2);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_VISION3, m_btn_sport_set_vision3);
	DDX_Control(pDX, IDC_BTN_SPORT_SET_VISION4, m_btn_sport_set_vision4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Set_Network, CFormView)
	//{{AFX_MSG_MAP(CScreen_Set_Network)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CIP_SET_EC, OnBtnCipSetEc)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_EC, OnBtnCportSetEc)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_BOTTOM, OnBtnCipSetBottom)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_BOTTOM, OnBtnCportSetBottom)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_8570, OnBtnCipSet8570)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_8570, OnBtnCportSet8570)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_NEXT, OnBtnCipSetNext)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_NEXT, OnBtnCportSetNext)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_FORMAT, OnBtnCipSetFormat)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_FORMAT, OnBtnCportSetFormat)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_LABEL_PRINTER1, OnBtnCipSetLabelPrinter1)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_LABEL_PRINTER1, OnBtnCportSetLabelPrinter1)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_LABEL_PRINTER2, OnBtnCipSetLabelPrinter2)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_LABEL_PRINTER2, OnBtnCportSetLabelPrinter2)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_LABEL_PRINTER3, OnBtnCipSetLabelPrinter3)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_LABEL_PRINTER3, OnBtnCportSetLabelPrinter3)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_LABEL_PRINTER4, OnBtnCipSetLabelPrinter4)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_LABEL_PRINTER4, OnBtnCportSetLabelPrinter4)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_VISION1, OnBtnCipSetVision1)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_VISION1, OnBtnCportSetVision1)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_VISION2, OnBtnCipSetVision2)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_VISION2, OnBtnCportSetVision2)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_VISION3, OnBtnCipSetVision3)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_VISION3, OnBtnCportSetVision3)
	ON_BN_CLICKED(IDC_BTN_CIP_SET_VISION4, OnBtnCipSetVision4)
	ON_BN_CLICKED(IDC_BTN_CPORT_SET_VISION4, OnBtnCportSetVision4)
	ON_BN_CLICKED(IDC_BTN_CNT_EC, OnBtnCntEc)
	ON_BN_CLICKED(IDC_BTN_CNT_BOTTOM, OnBtnCntBottom)
	ON_BN_CLICKED(IDC_BTN_CNT_NEXT2, OnBtnCntNext2)
	ON_BN_CLICKED(IDC_BTN_CNT_NEXT, OnBtnCntNext)
	ON_BN_CLICKED(IDC_BTN_CNT_FORMAT, OnBtnCntFormat)
	ON_BN_CLICKED(IDC_BTN_CNT_LABEL_PRINTER1, OnBtnCntLabelPrinter1)
	ON_BN_CLICKED(IDC_BTN_CNT_LABEL_PRINTER2, OnBtnCntLabelPrinter2)
	ON_BN_CLICKED(IDC_BTN_CNT_LABEL_PRINTER3, OnBtnCntLabelPrinter3)
	ON_BN_CLICKED(IDC_BTN_CNT_LABEL_PRINTER4, OnBtnCntLabelPrinter4)
	ON_BN_CLICKED(IDC_BTN_CNT_VISION1, OnBtnCntVision1)
	ON_BN_CLICKED(IDC_BTN_CNT_VISION2, OnBtnCntVision2)
	ON_BN_CLICKED(IDC_BTN_CNT_VISION3, OnBtnCntVision3)
	ON_BN_CLICKED(IDC_BTN_CNT_VISION4, OnBtnCntVision4)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_FRONT, OnBtnSportSetFront)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_BARCODE1, OnBtnSportSetBarcode1)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_BARCODE2, OnBtnSportSetBarcode2)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_BARCODE3, OnBtnSportSetBarcode3)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_BARCODE4, OnBtnSportSetBarcode4)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_VISION1, OnBtnSportSetVision1)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_VISION2, OnBtnSportSetVision2)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_VISION3, OnBtnSportSetVision3)
	ON_BN_CLICKED(IDC_BTN_SPORT_SET_VISION4, OnBtnSportSetVision4)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_1, OnBtnSerialApply1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_1, OnBtnSerialConnect1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_3, OnBtnSerialApply3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_3, OnBtnSerialConnect3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_5, OnBtnSerialApply5)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_5, OnBtnSerialConnect5)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_7, OnBtnSerialApply7)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_7, OnBtnSerialConnect7)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_2, OnBtnSerialApply2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_2, OnBtnSerialConnect2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_4, OnBtnSerialApply4)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_4, OnBtnSerialConnect4)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_6, OnBtnSerialApply6)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_6, OnBtnSerialConnect6)
	ON_BN_CLICKED(IDC_DGT_AUTO_SOCKET_OFF, OnDgtAutoSocketOff)
	ON_BN_CLICKED(IDC_BTN_TST_NEXT, OnBtnTstNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Network diagnostics

#ifdef _DEBUG
void CScreen_Set_Network::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Set_Network::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Network message handlers
void CScreen_Set_Network::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	OnAdmin_GroupBox_Set();
	OnAdmin_EditBox_Set();
	OnAdmin_Label_Set();
	OnAdmin_Data_Set();
	OnAdmin_Dgt_Set();
	
}

void CScreen_Set_Network::OnDestroy() 
{
	CFormView::OnDestroy();
}

void CScreen_Set_Network::OnTimer(UINT nIDEvent) 
{
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Set_Network::OnAdmin_GroupBox_Set()
{
	CSxLogFont admin_font(15,FW_SEMIBOLD,false,"Arial");

	m_group_client_ec.SetFont(admin_font);
	m_group_client_ec.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_ec.SetFontBold(TRUE);

	m_group_vision_client4.SetFont(admin_font);
	m_group_vision_client4.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_client4.SetFontBold(TRUE);

	m_group_vision_client3.SetFont(admin_font);
	m_group_vision_client3.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_client3.SetFontBold(TRUE);

	m_group_vision_client2.SetFont(admin_font);
	m_group_vision_client2.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_client2.SetFontBold(TRUE);

	m_group_vision_client1.SetFont(admin_font);
	m_group_vision_client1.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_client1.SetFontBold(TRUE);

	m_group_client_bottom.SetFont(admin_font);
	m_group_client_bottom.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_bottom.SetFontBold(TRUE);

	m_group_client_bcr_printer4.SetFont(admin_font);
	m_group_client_bcr_printer4.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_bcr_printer4.SetFontBold(TRUE);

	m_group_client_bcr_printer3.SetFont(admin_font);
	m_group_client_bcr_printer3.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_bcr_printer3.SetFontBold(TRUE);

	m_group_client_bcr_printer2.SetFont(admin_font);
	m_group_client_bcr_printer2.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_bcr_printer2.SetFontBold(TRUE);

	m_group_client_bcr_printer1.SetFont(admin_font);
	m_group_client_bcr_printer1.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_bcr_printer1.SetFontBold(TRUE);

	m_group_client_8570.SetFont(admin_font);
	m_group_client_8570.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_8570.SetFontBold(TRUE);
	
	m_group_client_next.SetFont(admin_font);
	m_group_client_next.SetCatptionTextColor(RGB(145,25,0));
	m_group_client_next.SetFontBold(TRUE);
	
	m_group_server.SetFont(admin_font);
	m_group_server.SetCatptionTextColor(RGB(145,25,0));
	m_group_server.SetFontBold(TRUE);

	m_group_serial.SetFont(admin_font);
	m_group_serial.SetCatptionTextColor(BLUE_C);
	m_group_serial.SetBorderColor(OK_BC);
	m_group_serial.SetFontBold(TRUE);
	
	m_group_serial_1.SetFont(admin_font);
	m_group_serial_1.SetCatptionTextColor(BLUE_C);
	m_group_serial_1.SetBorderColor(OK_BC);
	m_group_serial_1.SetFontBold(TRUE);
	
	m_group_serial_2.SetFont(admin_font);
	m_group_serial_2.SetCatptionTextColor(BLUE_C);
	m_group_serial_2.SetBorderColor(OK_BC);
	m_group_serial_2.SetFontBold(TRUE);
	
	m_group_serial_3.SetFont(admin_font);
	m_group_serial_3.SetCatptionTextColor(BLUE_C);
	m_group_serial_3.SetBorderColor(OK_BC);
	m_group_serial_3.SetFontBold(TRUE);

	m_group_serial_4.SetFont(admin_font);
	m_group_serial_4.SetCatptionTextColor(BLUE_C);
	m_group_serial_4.SetBorderColor(OK_BC);
	m_group_serial_4.SetFontBold(TRUE);

	m_group_serial_5.SetFont(admin_font);
	m_group_serial_5.SetCatptionTextColor(BLUE_C);
	m_group_serial_5.SetBorderColor(OK_BC);
	m_group_serial_5.SetFontBold(TRUE);

	m_group_serial_6.SetFont(admin_font);
	m_group_serial_6.SetCatptionTextColor(BLUE_C);
	m_group_serial_6.SetBorderColor(OK_BC);
	m_group_serial_6.SetFontBold(TRUE);

	m_group_serial_7.SetFont(admin_font);
	m_group_serial_7.SetCatptionTextColor(BLUE_C);
	m_group_serial_7.SetBorderColor(OK_BC);
	m_group_serial_7.SetFontBold(TRUE);

}

void CScreen_Set_Network::OnAdmin_EditBox_Set()
{
	m_edit_sport_front.SubclassDlgItem(IDC_EDIT_SPORT_FRONT, this);
	m_edit_sport_front.bkColor(RGB(50, 100, 150));
	m_edit_sport_front.textColor(RGB(255, 255,255));
	m_edit_sport_front.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
									  
	m_edit_sport_bcr1.SubclassDlgItem(IDC_EDIT_SPORT_BARCODE1, this);
	m_edit_sport_bcr1.bkColor(RGB(50, 100, 150));
	m_edit_sport_bcr1.textColor(RGB(255, 255,255));
	m_edit_sport_bcr1.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_sport_bcr2.SubclassDlgItem(IDC_EDIT_SPORT_BARCODE2, this);
	m_edit_sport_bcr2.bkColor(RGB(50, 100, 150));
	m_edit_sport_bcr2.textColor(RGB(255, 255,255));
	m_edit_sport_bcr2.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_sport_bcr3.SubclassDlgItem(IDC_EDIT_SPORT_BARCODE3, this);
	m_edit_sport_bcr3.bkColor(RGB(50, 100, 150));
	m_edit_sport_bcr3.textColor(RGB(255, 255,255));
	m_edit_sport_bcr3.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_sport_bcr4.SubclassDlgItem(IDC_EDIT_SPORT_BARCODE4, this);
	m_edit_sport_bcr4.bkColor(RGB(50, 100, 150));
	m_edit_sport_bcr4.textColor(RGB(255, 255,255));
	m_edit_sport_bcr4.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	////2014,1130
	m_edit_sport_vision1.SubclassDlgItem(IDC_EDIT_SPORT_VISON1, this);
	m_edit_sport_vision1.bkColor(RGB(50, 100, 150));
	m_edit_sport_vision1.textColor(RGB(255, 255,255));
	m_edit_sport_vision1.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_sport_vision2.SubclassDlgItem(IDC_EDIT_SPORT_VISON2, this);
	m_edit_sport_vision2.bkColor(RGB(50, 100, 150));
	m_edit_sport_vision2.textColor(RGB(255, 255,255));
	m_edit_sport_vision2.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_sport_vision3.SubclassDlgItem(IDC_EDIT_SPORT_VISON3, this);
	m_edit_sport_vision3.bkColor(RGB(50, 100, 150));
	m_edit_sport_vision3.textColor(RGB(255, 255,255));
	m_edit_sport_vision3.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_sport_vision4.SubclassDlgItem(IDC_EDIT_SPORT_VISON4, this);
	m_edit_sport_vision4.bkColor(RGB(50, 100, 150));
	m_edit_sport_vision4.textColor(RGB(255, 255,255));
	m_edit_sport_vision4.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	////
}

void CScreen_Set_Network::OnAdmin_Label_Set()
{
	CSxLogFont admin_font(15,FW_SEMIBOLD,false,"Arial");

	m_msg_cport_ec.SetFont(Func.mp_admin_font);
	m_msg_cport_ec.SetWindowText(_T("Port"));
	m_msg_cport_ec.SetCenterText();
	m_msg_cport_ec.SetColor(RGB(0,0,255));
	m_msg_cport_ec.SetGradientColor(RGB(0,0,0));
	m_msg_cport_ec.SetTextColor(RGB(255,255,255));

	m_msg_cport_8570.SetFont(Func.mp_admin_font);
	m_msg_cport_8570.SetWindowText(_T("Port"));
	m_msg_cport_8570.SetCenterText();
	m_msg_cport_8570.SetColor(RGB(0,0,255));
	m_msg_cport_8570.SetGradientColor(RGB(0,0,0));
	m_msg_cport_8570.SetTextColor(RGB(255,255,255));
	

	m_msg_cport_format.SetFont(Func.mp_admin_font);
	m_msg_cport_format.SetWindowText(_T("Port"));
	m_msg_cport_format.SetCenterText();
	m_msg_cport_format.SetColor(RGB(0,0,255));
	m_msg_cport_format.SetGradientColor(RGB(0,0,0));
	m_msg_cport_format.SetTextColor(RGB(255,255,255));



	m_msg_cport_bottom.SetFont(Func.mp_admin_font);
	m_msg_cport_bottom.SetWindowText(_T("Port"));
	m_msg_cport_bottom.SetCenterText();
	m_msg_cport_bottom.SetColor(RGB(0,0,255));
	m_msg_cport_bottom.SetGradientColor(RGB(0,0,0));
	m_msg_cport_bottom.SetTextColor(RGB(255,255,255));
	
	m_msg_cport_vision4.SetFont(Func.mp_admin_font);
	m_msg_cport_vision4.SetWindowText(_T("Port"));
	m_msg_cport_vision4.SetCenterText();
	m_msg_cport_vision4.SetColor(RGB(0,0,255));
	m_msg_cport_vision4.SetGradientColor(RGB(0,0,0));
	m_msg_cport_vision4.SetTextColor(RGB(255,255,255));

	m_msg_cport_vision3.SetFont(Func.mp_admin_font);
	m_msg_cport_vision3.SetWindowText(_T("Port"));
	m_msg_cport_vision3.SetCenterText();
	m_msg_cport_vision3.SetColor(RGB(0,0,255));
	m_msg_cport_vision3.SetGradientColor(RGB(0,0,0));
	m_msg_cport_vision3.SetTextColor(RGB(255,255,255));

	m_msg_cport_vision2.SetFont(Func.mp_admin_font);
	m_msg_cport_vision2.SetWindowText(_T("Port"));
	m_msg_cport_vision2.SetCenterText();
	m_msg_cport_vision2.SetColor(RGB(0,0,255));
	m_msg_cport_vision2.SetGradientColor(RGB(0,0,0));
	m_msg_cport_vision2.SetTextColor(RGB(255,255,255));

	m_msg_cport_vision1.SetFont(Func.mp_admin_font);
	m_msg_cport_vision1.SetWindowText(_T("Port"));
	m_msg_cport_vision1.SetCenterText();
	m_msg_cport_vision1.SetColor(RGB(0,0,255));
	m_msg_cport_vision1.SetGradientColor(RGB(0,0,0));
	m_msg_cport_vision1.SetTextColor(RGB(255,255,255));

	m_msg_cport_next.SetFont(Func.mp_admin_font);
	m_msg_cport_next.SetWindowText(_T("Port"));
	m_msg_cport_next.SetCenterText();
	m_msg_cport_next.SetColor(RGB(0,0,255));
	m_msg_cport_next.SetGradientColor(RGB(0,0,0));
	m_msg_cport_next.SetTextColor(RGB(255,255,255));

	m_msg_cport_label_printer1.SetFont(Func.mp_admin_font);
	m_msg_cport_label_printer1.SetWindowText(_T("Port"));
	m_msg_cport_label_printer1.SetCenterText();
	m_msg_cport_label_printer1.SetColor(RGB(0,0,255));
	m_msg_cport_label_printer1.SetGradientColor(RGB(0,0,0));
	m_msg_cport_label_printer1.SetTextColor(RGB(255,255,255));

	m_msg_cport_label_printer4.SetFont(Func.mp_admin_font);
	m_msg_cport_label_printer4.SetWindowText(_T("Port"));
	m_msg_cport_label_printer4.SetCenterText();
	m_msg_cport_label_printer4.SetColor(RGB(0,0,255));
	m_msg_cport_label_printer4.SetGradientColor(RGB(0,0,0));
	m_msg_cport_label_printer4.SetTextColor(RGB(255,255,255));

	m_msg_cport_label_printer3.SetFont(Func.mp_admin_font);
	m_msg_cport_label_printer3.SetWindowText(_T("Port"));
	m_msg_cport_label_printer3.SetCenterText();
	m_msg_cport_label_printer3.SetColor(RGB(0,0,255));
	m_msg_cport_label_printer3.SetGradientColor(RGB(0,0,0));
	m_msg_cport_label_printer3.SetTextColor(RGB(255,255,255));

	m_msg_cport_label_printer2.SetFont(Func.mp_admin_font);
	m_msg_cport_label_printer2.SetWindowText(_T("Port"));
	m_msg_cport_label_printer2.SetCenterText();
	m_msg_cport_label_printer2.SetColor(RGB(0,0,255));
	m_msg_cport_label_printer2.SetGradientColor(RGB(0,0,0));
	m_msg_cport_label_printer2.SetTextColor(RGB(255,255,255));

	m_msg_cip_ec.SetFont(Func.mp_admin_font);
	m_msg_cip_ec.SetWindowText(_T("IP Address"));
	m_msg_cip_ec.SetCenterText();
	m_msg_cip_ec.SetColor(RGB(0,0,255));
	m_msg_cip_ec.SetGradientColor(RGB(0,0,0));
	m_msg_cip_ec.SetTextColor(RGB(255,255,255));

	m_msg_ip8570.SetFont(Func.mp_admin_font);
	m_msg_ip8570.SetWindowText(_T("IP Address"));
	m_msg_ip8570.SetCenterText();
	m_msg_ip8570.SetColor(RGB(0,0,255));
	m_msg_ip8570.SetGradientColor(RGB(0,0,0));
	m_msg_ip8570.SetTextColor(RGB(255,255,255));

	m_msg_ip_format.SetFont(Func.mp_admin_font);
	m_msg_ip_format.SetWindowText(_T("IP Address"));
	m_msg_ip_format.SetCenterText();
	m_msg_ip_format.SetColor(RGB(0,0,255));
	m_msg_ip_format.SetGradientColor(RGB(0,0,0));
	m_msg_ip_format.SetTextColor(RGB(255,255,255));


	m_msg_cip_next.SetFont(Func.mp_admin_font);
	m_msg_cip_next.SetWindowText(_T("IP Address"));
	m_msg_cip_next.SetCenterText();
	m_msg_cip_next.SetColor(RGB(0,0,255));
	m_msg_cip_next.SetGradientColor(RGB(0,0,0));
	m_msg_cip_next.SetTextColor(RGB(255,255,255));

	m_msg_cip_label_printer1.SetFont(Func.mp_admin_font);
	m_msg_cip_label_printer1.SetWindowText(_T("IP Address"));
	m_msg_cip_label_printer1.SetCenterText();
	m_msg_cip_label_printer1.SetColor(RGB(0,0,255));
	m_msg_cip_label_printer1.SetGradientColor(RGB(0,0,0));
	m_msg_cip_label_printer1.SetTextColor(RGB(255,255,255));

	m_msg_ip_bottom.SetFont(Func.mp_admin_font);
	m_msg_ip_bottom.SetWindowText(_T("IP Address"));
	m_msg_ip_bottom.SetCenterText();
	m_msg_ip_bottom.SetColor(RGB(0,0,255));
	m_msg_ip_bottom.SetGradientColor(RGB(0,0,0));
	m_msg_ip_bottom.SetTextColor(RGB(255,255,255));

	m_msg_ip_label_printer3.SetFont(Func.mp_admin_font);
	m_msg_ip_label_printer3.SetWindowText(_T("IP Address"));
	m_msg_ip_label_printer3.SetCenterText();
	m_msg_ip_label_printer3.SetColor(RGB(0,0,255));
	m_msg_ip_label_printer3.SetGradientColor(RGB(0,0,0));
	m_msg_ip_label_printer3.SetTextColor(RGB(255,255,255));

	m_msg_ip_label_printer2.SetFont(Func.mp_admin_font);
	m_msg_ip_label_printer2.SetWindowText(_T("IP Address"));
	m_msg_ip_label_printer2.SetCenterText();
	m_msg_ip_label_printer2.SetColor(RGB(0,0,255));
	m_msg_ip_label_printer2.SetGradientColor(RGB(0,0,0));
	m_msg_ip_label_printer2.SetTextColor(RGB(255,255,255));

	m_msg_ip_label_printer4.SetFont(Func.mp_admin_font);
	m_msg_ip_label_printer4.SetWindowText(_T("IP Address"));
	m_msg_ip_label_printer4.SetCenterText();
	m_msg_ip_label_printer4.SetColor(RGB(0,0,255));
	m_msg_ip_label_printer4.SetGradientColor(RGB(0,0,0));
	m_msg_ip_label_printer4.SetTextColor(RGB(255,255,255));


	m_msg_ip_vision4.SetFont(Func.mp_admin_font);
	m_msg_ip_vision4.SetWindowText(_T("IP Address"));
	m_msg_ip_vision4.SetCenterText();
	m_msg_ip_vision4.SetColor(RGB(0,0,255));
	m_msg_ip_vision4.SetGradientColor(RGB(0,0,0));
	m_msg_ip_vision4.SetTextColor(RGB(255,255,255));

	m_msg_ip_vision3.SetFont(Func.mp_admin_font);
	m_msg_ip_vision3.SetWindowText(_T("IP Address"));
	m_msg_ip_vision3.SetCenterText();
	m_msg_ip_vision3.SetColor(RGB(0,0,255));
	m_msg_ip_vision3.SetGradientColor(RGB(0,0,0));
	m_msg_ip_vision3.SetTextColor(RGB(255,255,255));

	m_msg_ip_vision2.SetFont(Func.mp_admin_font);
	m_msg_ip_vision2.SetWindowText(_T("IP Address"));
	m_msg_ip_vision2.SetCenterText();
	m_msg_ip_vision2.SetColor(RGB(0,0,255));
	m_msg_ip_vision2.SetGradientColor(RGB(0,0,0));
	m_msg_ip_vision2.SetTextColor(RGB(255,255,255));

	m_msg_ip_vision1.SetFont(Func.mp_admin_font);
	m_msg_ip_vision1.SetWindowText(_T("IP Address"));
	m_msg_ip_vision1.SetCenterText();
	m_msg_ip_vision1.SetColor(RGB(0,0,255));
	m_msg_ip_vision1.SetGradientColor(RGB(0,0,0));
	m_msg_ip_vision1.SetTextColor(RGB(255,255,255));

	m_msg_sport_front.SetFont(Func.mp_admin_font);
	m_msg_sport_front.SetWindowText(_T("Front Machine Port"));
	m_msg_sport_front.SetCenterText();
	m_msg_sport_front.SetColor(RGB(0,0,255));
	m_msg_sport_front.SetGradientColor(RGB(0,0,0));
	m_msg_sport_front.SetTextColor(RGB(255,255,255));
	
	m_msg_sport_barcode4.SetFont(Func.mp_admin_font);
	m_msg_sport_barcode4.SetWindowText(_T("Barcode4"));
	m_msg_sport_barcode4.SetCenterText();
	m_msg_sport_barcode4.SetColor(RGB(0,0,255));
	m_msg_sport_barcode4.SetGradientColor(RGB(0,0,0));
	m_msg_sport_barcode4.SetTextColor(RGB(255,255,255));

	m_msg_sport_barcode3.SetFont(Func.mp_admin_font);
	m_msg_sport_barcode3.SetWindowText(_T("Barcode3"));
	m_msg_sport_barcode3.SetCenterText();
	m_msg_sport_barcode3.SetColor(RGB(0,0,255));
	m_msg_sport_barcode3.SetGradientColor(RGB(0,0,0));
	m_msg_sport_barcode3.SetTextColor(RGB(255,255,255));

	m_msg_sport_barcode2.SetFont(Func.mp_admin_font);
	m_msg_sport_barcode2.SetWindowText(_T("Barcode2"));
	m_msg_sport_barcode2.SetCenterText();
	m_msg_sport_barcode2.SetColor(RGB(0,0,255));
	m_msg_sport_barcode2.SetGradientColor(RGB(0,0,0));
	m_msg_sport_barcode2.SetTextColor(RGB(255,255,255));

	m_msg_sport_barcode1.SetFont(Func.mp_admin_font);
	m_msg_sport_barcode1.SetWindowText(_T("Barcode1"));
	m_msg_sport_barcode1.SetCenterText();
	m_msg_sport_barcode1.SetColor(RGB(0,0,255));
	m_msg_sport_barcode1.SetGradientColor(RGB(0,0,0));
	m_msg_sport_barcode1.SetTextColor(RGB(255,255,255));

	////2014,1130
	m_msg_sport_vision1.SetFont(Func.mp_admin_font);
	m_msg_sport_vision1.SetWindowText(_T("Vision1"));
	m_msg_sport_vision1.SetCenterText();
	m_msg_sport_vision1.SetColor(RGB(0,0,255));
	m_msg_sport_vision1.SetGradientColor(RGB(0,0,0));
	m_msg_sport_vision1.SetTextColor(RGB(255,255,255));

	m_msg_sport_vision2.SetFont(Func.mp_admin_font);
	m_msg_sport_vision2.SetWindowText(_T("Vision2"));
	m_msg_sport_vision2.SetCenterText();
	m_msg_sport_vision2.SetColor(RGB(0,0,255));
	m_msg_sport_vision2.SetGradientColor(RGB(0,0,0));
	m_msg_sport_vision2.SetTextColor(RGB(255,255,255));

	m_msg_sport_vision3.SetFont(Func.mp_admin_font);
	m_msg_sport_vision3.SetWindowText(_T("Vision3"));
	m_msg_sport_vision3.SetCenterText();
	m_msg_sport_vision3.SetColor(RGB(0,0,255));
	m_msg_sport_vision3.SetGradientColor(RGB(0,0,0));
	m_msg_sport_vision3.SetTextColor(RGB(255,255,255));
	
	m_msg_sport_vision4.SetFont(Func.mp_admin_font);
	m_msg_sport_vision4.SetWindowText(_T("Vision4"));
	m_msg_sport_vision4.SetCenterText();
	m_msg_sport_vision4.SetColor(RGB(0,0,255));
	m_msg_sport_vision4.SetGradientColor(RGB(0,0,0));
	m_msg_sport_vision4.SetTextColor(RGB(255,255,255));

	////


	m_msg_port_1.SetFont(Func.mp_admin_font);
	m_msg_port_1.SetWindowText(_T("Port"));
	m_msg_port_1.SetCenterText();
	m_msg_port_1.SetColor(WHITE_C);
	m_msg_port_1.SetGradientColor(ORANGE_C);
	m_msg_port_1.SetTextColor(BLACK_C);

	m_msg_port_2.SetFont(Func.mp_admin_font);
	m_msg_port_2.SetWindowText(_T("Port"));
	m_msg_port_2.SetCenterText();
	m_msg_port_2.SetColor(WHITE_C);
	m_msg_port_2.SetGradientColor(ORANGE_C);
	m_msg_port_2.SetTextColor(BLACK_C);

	m_msg_port_3.SetFont(Func.mp_admin_font);
	m_msg_port_3.SetWindowText(_T("Port"));
	m_msg_port_3.SetCenterText();
	m_msg_port_3.SetColor(WHITE_C);
	m_msg_port_3.SetGradientColor(ORANGE_C);
	m_msg_port_3.SetTextColor(BLACK_C);

	m_msg_port_4.SetFont(Func.mp_admin_font);
	m_msg_port_4.SetWindowText(_T("Port"));
	m_msg_port_4.SetCenterText();
	m_msg_port_4.SetColor(WHITE_C);
	m_msg_port_4.SetGradientColor(ORANGE_C);
	m_msg_port_4.SetTextColor(BLACK_C);

	m_msg_port_5.SetFont(Func.mp_admin_font);
	m_msg_port_5.SetWindowText(_T("Port"));
	m_msg_port_5.SetCenterText();
	m_msg_port_5.SetColor(WHITE_C);
	m_msg_port_5.SetGradientColor(ORANGE_C);
	m_msg_port_5.SetTextColor(BLACK_C);

	m_msg_port_6.SetFont(Func.mp_admin_font);
	m_msg_port_6.SetWindowText(_T("Port"));
	m_msg_port_6.SetCenterText();
	m_msg_port_6.SetColor(WHITE_C);
	m_msg_port_6.SetGradientColor(ORANGE_C);
	m_msg_port_6.SetTextColor(BLACK_C);

	m_msg_port_7.SetFont(Func.mp_admin_font);
	m_msg_port_7.SetWindowText(_T("Port"));
	m_msg_port_7.SetCenterText();
	m_msg_port_7.SetColor(WHITE_C);
	m_msg_port_7.SetGradientColor(ORANGE_C);
	m_msg_port_7.SetTextColor(BLACK_C);
	
	m_msg_data_1.SetFont(Func.mp_admin_font);
	m_msg_data_1.SetWindowText(_T("Data"));
	m_msg_data_1.SetCenterText();
	m_msg_data_1.SetColor(WHITE_C);
	m_msg_data_1.SetGradientColor(ORANGE_C);
	m_msg_data_1.SetTextColor(BLACK_C);

	m_msg_data_2.SetFont(Func.mp_admin_font);
	m_msg_data_2.SetWindowText(_T("Data"));
	m_msg_data_2.SetCenterText();
	m_msg_data_2.SetColor(WHITE_C);
	m_msg_data_2.SetGradientColor(ORANGE_C);
	m_msg_data_2.SetTextColor(BLACK_C);

	m_msg_data_3.SetFont(Func.mp_admin_font);
	m_msg_data_3.SetWindowText(_T("Data"));
	m_msg_data_3.SetCenterText();
	m_msg_data_3.SetColor(WHITE_C);
	m_msg_data_3.SetGradientColor(ORANGE_C);
	m_msg_data_3.SetTextColor(BLACK_C);

	m_msg_data_4.SetFont(Func.mp_admin_font);
	m_msg_data_4.SetWindowText(_T("Data"));
	m_msg_data_4.SetCenterText();
	m_msg_data_4.SetColor(WHITE_C);
	m_msg_data_4.SetGradientColor(ORANGE_C);
	m_msg_data_4.SetTextColor(BLACK_C);

	m_msg_data_5.SetFont(Func.mp_admin_font);
	m_msg_data_5.SetWindowText(_T("Data"));
	m_msg_data_5.SetCenterText();
	m_msg_data_5.SetColor(WHITE_C);
	m_msg_data_5.SetGradientColor(ORANGE_C);
	m_msg_data_5.SetTextColor(BLACK_C);

	m_msg_data_6.SetFont(Func.mp_admin_font);
	m_msg_data_6.SetWindowText(_T("Data"));
	m_msg_data_6.SetCenterText();
	m_msg_data_6.SetColor(WHITE_C);
	m_msg_data_6.SetGradientColor(ORANGE_C);
	m_msg_data_6.SetTextColor(BLACK_C);

	m_msg_data_7.SetFont(Func.mp_admin_font);
	m_msg_data_7.SetWindowText(_T("Data"));
	m_msg_data_7.SetCenterText();
	m_msg_data_7.SetColor(WHITE_C);
	m_msg_data_7.SetGradientColor(ORANGE_C);
	m_msg_data_7.SetTextColor(BLACK_C);

	m_msg_parity_1.SetFont(Func.mp_admin_font);
	m_msg_parity_1.SetWindowText(_T("Parity"));
	m_msg_parity_1.SetCenterText();
	m_msg_parity_1.SetColor(WHITE_C);
	m_msg_parity_1.SetGradientColor(ORANGE_C);
	m_msg_parity_1.SetTextColor(BLACK_C);

	m_msg_parity_2.SetFont(Func.mp_admin_font);
	m_msg_parity_2.SetWindowText(_T("Parity"));
	m_msg_parity_2.SetCenterText();
	m_msg_parity_2.SetColor(WHITE_C);
	m_msg_parity_2.SetGradientColor(ORANGE_C);
	m_msg_parity_2.SetTextColor(BLACK_C);

	m_msg_parity_3.SetFont(Func.mp_admin_font);
	m_msg_parity_3.SetWindowText(_T("Parity"));
	m_msg_parity_3.SetCenterText();
	m_msg_parity_3.SetColor(WHITE_C);
	m_msg_parity_3.SetGradientColor(ORANGE_C);
	m_msg_parity_3.SetTextColor(BLACK_C);

	m_msg_parity_4.SetFont(Func.mp_admin_font);
	m_msg_parity_4.SetWindowText(_T("Parity"));
	m_msg_parity_4.SetCenterText();
	m_msg_parity_4.SetColor(WHITE_C);
	m_msg_parity_4.SetGradientColor(ORANGE_C);
	m_msg_parity_4.SetTextColor(BLACK_C);

	m_msg_parity_5.SetFont(Func.mp_admin_font);
	m_msg_parity_5.SetWindowText(_T("Parity"));
	m_msg_parity_5.SetCenterText();
	m_msg_parity_5.SetColor(WHITE_C);
	m_msg_parity_5.SetGradientColor(ORANGE_C);
	m_msg_parity_5.SetTextColor(BLACK_C);

	m_msg_parity_6.SetFont(Func.mp_admin_font);
	m_msg_parity_6.SetWindowText(_T("Parity"));
	m_msg_parity_6.SetCenterText();
	m_msg_parity_6.SetColor(WHITE_C);
	m_msg_parity_6.SetGradientColor(ORANGE_C);
	m_msg_parity_6.SetTextColor(BLACK_C);

	m_msg_parity_7.SetFont(Func.mp_admin_font);
	m_msg_parity_7.SetWindowText(_T("Parity"));
	m_msg_parity_7.SetCenterText();
	m_msg_parity_7.SetColor(WHITE_C);
	m_msg_parity_7.SetGradientColor(ORANGE_C);
	m_msg_parity_7.SetTextColor(BLACK_C);

	m_msg_rate_1.SetFont(Func.mp_admin_font);
	m_msg_rate_1.SetWindowText(_T("BaudRate"));
	m_msg_rate_1.SetCenterText();
	m_msg_rate_1.SetColor(WHITE_C);
	m_msg_rate_1.SetGradientColor(ORANGE_C);
	m_msg_rate_1.SetTextColor(BLACK_C);

	m_msg_rate_2.SetFont(Func.mp_admin_font);
	m_msg_rate_2.SetWindowText(_T("BaudRate"));
	m_msg_rate_2.SetCenterText();
	m_msg_rate_2.SetColor(WHITE_C);
	m_msg_rate_2.SetGradientColor(ORANGE_C);
	m_msg_rate_2.SetTextColor(BLACK_C);

	m_msg_rate_3.SetFont(Func.mp_admin_font);
	m_msg_rate_3.SetWindowText(_T("BaudRate"));
	m_msg_rate_3.SetCenterText();
	m_msg_rate_3.SetColor(WHITE_C);
	m_msg_rate_3.SetGradientColor(ORANGE_C);
	m_msg_rate_3.SetTextColor(BLACK_C);

	m_msg_rate_4.SetFont(Func.mp_admin_font);
	m_msg_rate_4.SetWindowText(_T("BaudRate"));
	m_msg_rate_4.SetCenterText();
	m_msg_rate_4.SetColor(WHITE_C);
	m_msg_rate_4.SetGradientColor(ORANGE_C);
	m_msg_rate_4.SetTextColor(BLACK_C);

	m_msg_rate_5.SetFont(Func.mp_admin_font);
	m_msg_rate_5.SetWindowText(_T("BaudRate"));
	m_msg_rate_5.SetCenterText();
	m_msg_rate_5.SetColor(WHITE_C);
	m_msg_rate_5.SetGradientColor(ORANGE_C);
	m_msg_rate_5.SetTextColor(BLACK_C);

	m_msg_rate_6.SetFont(Func.mp_admin_font);
	m_msg_rate_6.SetWindowText(_T("BaudRate"));
	m_msg_rate_6.SetCenterText();
	m_msg_rate_6.SetColor(WHITE_C);
	m_msg_rate_6.SetGradientColor(ORANGE_C);
	m_msg_rate_6.SetTextColor(BLACK_C);

	m_msg_rate_7.SetFont(Func.mp_admin_font);
	m_msg_rate_7.SetWindowText(_T("BaudRate"));
	m_msg_rate_7.SetCenterText();
	m_msg_rate_7.SetColor(WHITE_C);
	m_msg_rate_7.SetGradientColor(ORANGE_C);
	m_msg_rate_7.SetTextColor(BLACK_C);

	m_msg_stop_1.SetFont(Func.mp_admin_font);
	m_msg_stop_1.SetWindowText(_T("Stop"));
	m_msg_stop_1.SetCenterText();
	m_msg_stop_1.SetColor(WHITE_C);
	m_msg_stop_1.SetGradientColor(ORANGE_C);
	m_msg_stop_1.SetTextColor(BLACK_C);

	m_msg_stop_2.SetFont(Func.mp_admin_font);
	m_msg_stop_2.SetWindowText(_T("Stop"));
	m_msg_stop_2.SetCenterText();
	m_msg_stop_2.SetColor(WHITE_C);
	m_msg_stop_2.SetGradientColor(ORANGE_C);
	m_msg_stop_2.SetTextColor(BLACK_C);

	m_msg_stop_3.SetFont(Func.mp_admin_font);
	m_msg_stop_3.SetWindowText(_T("Stop"));
	m_msg_stop_3.SetCenterText();
	m_msg_stop_3.SetColor(WHITE_C);
	m_msg_stop_3.SetGradientColor(ORANGE_C);
	m_msg_stop_3.SetTextColor(BLACK_C);

	m_msg_stop_4.SetFont(Func.mp_admin_font);
	m_msg_stop_4.SetWindowText(_T("Stop"));
	m_msg_stop_4.SetCenterText();
	m_msg_stop_4.SetColor(WHITE_C);
	m_msg_stop_4.SetGradientColor(ORANGE_C);
	m_msg_stop_4.SetTextColor(BLACK_C);

	m_msg_stop_5.SetFont(Func.mp_admin_font);
	m_msg_stop_5.SetWindowText(_T("Stop"));
	m_msg_stop_5.SetCenterText();
	m_msg_stop_5.SetColor(WHITE_C);
	m_msg_stop_5.SetGradientColor(ORANGE_C);
	m_msg_stop_5.SetTextColor(BLACK_C);

	m_msg_stop_6.SetFont(Func.mp_admin_font);
	m_msg_stop_6.SetWindowText(_T("Stop"));
	m_msg_stop_6.SetCenterText();
	m_msg_stop_6.SetColor(WHITE_C);
	m_msg_stop_6.SetGradientColor(ORANGE_C);
	m_msg_stop_6.SetTextColor(BLACK_C);

	m_msg_stop_7.SetFont(Func.mp_admin_font);
	m_msg_stop_7.SetWindowText(_T("Stop"));
	m_msg_stop_7.SetCenterText();
	m_msg_stop_7.SetColor(WHITE_C);
	m_msg_stop_7.SetGradientColor(ORANGE_C);
	m_msg_stop_7.SetTextColor(BLACK_C);
}

void CScreen_Set_Network::OnAdmin_Data_Set()
{
	int i;
	// network
	CString strTemp;
	DWORD dwTmp[4];
	DWORD dwRtn;

	sscanf( (LPCSTR)st_client[CLS_ECSERVER].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_ec.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_ECSERVER].n_port);
	m_edit_cport_ec_client.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_BOTTOM].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_bottom.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_BOTTOM].n_port);
	m_edit_cport_bottom_client.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_8570].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_8570.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_8570].n_port);
	m_edit_cport_8570_client.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_NEXT].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_next.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_NEXT].n_port);
	m_edit_cport_next.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_FORMAT].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_format.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_FORMAT].n_port);
	m_edit_cport_format_client.SetWindowText(strTemp);

	st_client[CLS_OTHERS].str_ip = "127.0.0.1";
	st_client[CLS_OTHERS].n_port = 10056;


	sscanf( (LPCSTR)st_client[CLS_BCR_PRINTER1].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_label_printer1.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_BCR_PRINTER1].n_port);
	m_edit_cport_label_printer1.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_BCR_PRINTER2].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_label_printer2.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_BCR_PRINTER2].n_port);
	m_edit_cport_label_printer2.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_BCR_PRINTER3].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_label_printer3.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_BCR_PRINTER3].n_port);
	m_edit_cport_label_printer3.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_BCR_PRINTER4].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_label_printer4.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_BCR_PRINTER4].n_port);
	m_edit_cport_label_printer4.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_VIS_DENSITY].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_vis1.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_VIS_DENSITY].n_port);
	m_edit_cport_vis1.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_VIS_BARCODE].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_vis2.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_VIS_BARCODE].n_port);
	m_edit_cport_vis2.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_VIS_TOP].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_vis3.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_VIS_TOP].n_port);
	m_edit_cport_vis3.SetWindowText(strTemp);

	sscanf( (LPCSTR)st_client[CLS_VIS_BTM].str_ip, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];
	m_cip_vis4.SetAddress( dwRtn );
	strTemp.Format("%d", st_client[CLS_VIS_BTM].n_port);
	m_edit_cport_vis4.SetWindowText(strTemp);

	strTemp.Format("%d", st_server[SVR_FRONT].n_port);
	m_edit_sport_front.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_PRINTER1].n_port);
	m_edit_sport_bcr1.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_PRINTER2].n_port);
	m_edit_sport_bcr2.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_PRINTER3].n_port);
	m_edit_sport_bcr3.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_PRINTER4].n_port);
	m_edit_sport_bcr4.SetWindowText( strTemp );

	////2014,1205
	strTemp.Format("%d", st_server[SVR_VISION1].n_port);
	m_edit_sport_vision1.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_VISION2].n_port);
	m_edit_sport_vision2.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_VISION3].n_port);
	m_edit_sport_vision3.SetWindowText( strTemp );
	strTemp.Format("%d", st_server[SVR_VISION4].n_port);
	m_edit_sport_vision4.SetWindowText( strTemp );
	////

	for(i = 0; i < MAX_PORT ; i++)
	{
		m_n_port[i][1]   = ConverterToPos(COM_PORT, rs_232.n_serial_port[i]);
		m_n_rate[i][1]   = ConverterToPos(COM_BAUDRATE, rs_232.n_serial_baudrate[i]);
		m_n_data[i][1]   = ConverterToPos(COM_DATA, rs_232.n_serial_data[i]);
		m_n_stop[i][1]   = ConverterToPos(COM_STOP, rs_232.n_serial_stop[i]);
		m_n_parity[i][1] = ConverterToPos(COM_PARITY, rs_232.n_serial_parity[i]);
	}
	m_cb_port_1.SetCurSel(m_n_port[0][1]);
	m_cb_rate_1.SetCurSel(m_n_rate[0][1]);
	m_cb_data_1.SetCurSel(m_n_data[0][1]);
	m_cb_parity_1.SetCurSel(m_n_parity[0][1]);
	m_cb_stop_1.SetCurSel(m_n_stop[0][1]);
	
	m_cb_port_2.SetCurSel(m_n_port[1][1]);
	m_cb_rate_2.SetCurSel(m_n_rate[1][1]);
	m_cb_data_2.SetCurSel(m_n_data[1][1]);
	m_cb_parity_2.SetCurSel(m_n_parity[1][1]);
	m_cb_stop_2.SetCurSel(m_n_stop[1][1]);
	
	m_cb_port_3.SetCurSel(m_n_port[2][1]);
	m_cb_rate_3.SetCurSel(m_n_rate[2][1]);
	m_cb_data_3.SetCurSel(m_n_data[2][1]);
	m_cb_parity_3.SetCurSel(m_n_parity[2][1]);
	m_cb_stop_3.SetCurSel(m_n_stop[2][1]);
	
	m_cb_port_4.SetCurSel(m_n_port[3][1]);
	m_cb_rate_4.SetCurSel(m_n_rate[3][1]);
	m_cb_data_4.SetCurSel(m_n_data[3][1]);
	m_cb_parity_4.SetCurSel(m_n_parity[3][1]);
	m_cb_stop_4.SetCurSel(m_n_stop[3][1]);

	m_cb_port_5.SetCurSel(m_n_port[4][1]);
	m_cb_rate_5.SetCurSel(m_n_rate[4][1]);
	m_cb_data_5.SetCurSel(m_n_data[4][1]);
	m_cb_parity_5.SetCurSel(m_n_parity[4][1]);
	m_cb_stop_5.SetCurSel(m_n_stop[4][1]);

	m_cb_port_6.SetCurSel(m_n_port[5][1]);
	m_cb_rate_6.SetCurSel(m_n_rate[5][1]);
	m_cb_data_6.SetCurSel(m_n_data[5][1]);
	m_cb_parity_6.SetCurSel(m_n_parity[5][1]);
	m_cb_stop_6.SetCurSel(m_n_stop[5][1]);		

	m_cb_port_7.SetCurSel(m_n_port[6][1]);
	m_cb_rate_7.SetCurSel(m_n_rate[6][1]);
	m_cb_data_7.SetCurSel(m_n_data[6][1]);
	m_cb_parity_7.SetCurSel(m_n_parity[6][1]);
	m_cb_stop_7.SetCurSel(m_n_stop[6][1]);

}

void CScreen_Set_Network::OnBtnCipSetEc() 
{
//	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_ec.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "EC Server IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP Next Step APCT");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_ec.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_ECSERVER].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_EC", LPCTSTR(strip), st_path.mstr_basic);
	}
}

void CScreen_Set_Network::OnBtnCportSetEc() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_EC))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Rear APCT port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_ec.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_EC))->SetWindowText(strport);
		
		st_client[CLS_ECSERVER].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_EC", LPCTSTR(strport), st_path.mstr_basic);
	}
}

void CScreen_Set_Network::OnBtnCipSetLabelPrinter1() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_label_printer1.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "barcode_printer 1번 IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP barcode_printer 1");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_label_printer1.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_BCR_PRINTER1].str_ip = strip;
 		:: WritePrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_1", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCipSetNext() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();		
		return;
	}
	
	BYTE nField[4];
	m_cip_next.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "label_printer2 IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP label_printer2");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_next.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_NEXT].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_NEXT", LPCTSTR(strip), st_path.mstr_basic);
	}
}
void CScreen_Set_Network::OnBtnCportSetLabelPrinter1() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER1))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("label_printer port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_label_printer1.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER1))->SetWindowText(strport);
		
		st_client[CLS_BCR_PRINTER1].n_port = atoi(strport);
 		:: WritePrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_1", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetNext() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_NEXT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("label_printer2 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_next.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_NEXT))->SetWindowText(strport);
		
		st_client[CLS_NEXT].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_NEXT", LPCTSTR(strport), st_path.mstr_basic);
	}
}

void CScreen_Set_Network::OnDgtAutoSocketOff() 
{
	// TODO: Add your control notification handler code here
	char chr_buf[20] ;
	
	int mn_count = m_dgt_autosocketoff_cnt.GetValue();
	CString	mstr_temp = LPCTSTR(_itoa(mn_count, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("AUTO SOCKET OFF 연속 실패 횟수 (0 = 사용안함)");
	if ( g_local.GetLocalType() == LOCAL_ENG )	st_msg.mstr_keypad_msg = _T("AUTO SOCKET OFF consecutive failures (0 = Do not use)");//20130930
	
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(0, 10, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	
	st_handler.mn_AutoSocketOff_Cnt = atoi( mstr_temp );
	m_dgt_autosocketoff_cnt.SetValue( st_handler.mn_AutoSocketOff_Cnt );
	
// 	mstr_temp.Format("%d", st_handler.mn_AutoSocketOff_Cnt );
//  	:: WritePrivateProfileString("BasicData", "AUTO_SOCKET_OFF_CNT", LPCTSTR(mstr_temp), st_path.mstr_basic);	
}


void CScreen_Set_Network::OnBtnSerialApply1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 0;
	
	m_n_port[i][1]			= m_cb_port_1.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_1.GetCurSel();
	m_n_data[i][1]			= m_cb_data_1.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_1.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_1.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
	
}

void CScreen_Set_Network::OnBtnSerialConnect1() 
{
	// TODO: Add your control notification handler code here
	int i = 0;

	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
}


void CScreen_Set_Network::OnBtnSerialApply2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 1;
	
	m_n_port[i][1]			= m_cb_port_2.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_2.GetCurSel();
	m_n_data[i][1]			= m_cb_data_2.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_2.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_2.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
	
}

void CScreen_Set_Network::OnBtnSerialConnect2() 
{
	// TODO: Add your control notification handler code here
	int i = 1;
	
	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
}


void CScreen_Set_Network::OnBtnSerialApply3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 2;
	
	m_n_port[i][1]			= m_cb_port_3.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_3.GetCurSel();
	m_n_data[i][1]			= m_cb_data_3.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_3.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_3.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
	
}

void CScreen_Set_Network::OnBtnSerialConnect3() 
{
	// TODO: Add your control notification handler code here
	int i = 2;
	
	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
}

void CScreen_Set_Network::OnBtnSerialApply4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 3;
	
	m_n_port[i][1]			= m_cb_port_4.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_4.GetCurSel();
	m_n_data[i][1]			= m_cb_data_4.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_4.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_4.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
	
}

void CScreen_Set_Network::OnBtnSerialConnect4() 
{
	// TODO: Add your control notification handler code here
	int i = 3;
	
	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
}


void CScreen_Set_Network::OnBtnSerialApply5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 4;
	
	m_n_port[i][1]			= m_cb_port_5.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_5.GetCurSel();
	m_n_data[i][1]			= m_cb_data_5.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_5.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_5.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
	
}

void CScreen_Set_Network::OnBtnSerialConnect5() 
{
	// TODO: Add your control notification handler code here
	int i = 4;
	
	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
}


void CScreen_Set_Network::OnBtnSerialApply6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 5;

	m_n_port[i][1]			= m_cb_port_6.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_6.GetCurSel();
	m_n_data[i][1]			= m_cb_data_6.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_6.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_6.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
}

void CScreen_Set_Network::OnBtnSerialConnect6() 
{
	// TODO: Add your control notification handler code here
	int i = 5;
	
	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
}


void CScreen_Set_Network::OnBtnSerialApply7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i = 6;
	
	m_n_port[i][1]			= m_cb_port_7.GetCurSel();
	m_n_rate[i][1]			= m_cb_rate_7.GetCurSel();
	m_n_data[i][1]			= m_cb_data_7.GetCurSel();
	m_n_stop[i][1]			= m_cb_stop_7.GetCurSel();
	m_n_parity[i][1]	    = m_cb_parity_7.GetCurSel();
	
	rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
	rs_232.n_serial_baudrate[i] = ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
	rs_232.n_serial_data[i]     = ConverterToData(COM_DATA, m_n_data[i][1]);
	rs_232.n_serial_stop[i]     = ConverterToData(COM_STOP, m_n_stop[i][1]);
	rs_232.n_serial_parity[i]   = ConverterToData(COM_PARITY, m_n_parity[i][1]);
	
}

void CScreen_Set_Network::OnBtnSerialConnect7() 
{
	// TODO: Add your control notification handler code here
	int i = 6;
	
	st_serial.mstr_port_chk[i] = "NOT FOUND";
	st_handler.mnSerial_Port_Creating[i] = CTL_NO;
	
	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, CTL_YES, rs_232.n_serial_port[i]);
	
}

void CScreen_Set_Network::OnAdmin_Dgt_Set()
{
	m_dgt_autosocketoff_cnt.SubclassDlgItem(IDC_DGT_AUTO_SOCKET_OFF, this);
	m_dgt_autosocketoff_cnt.SetStyle(IDB_BIG3, 4);
	m_dgt_autosocketoff_cnt.SetValue(st_handler.mn_AutoSocketOff_Cnt );
}

void CScreen_Set_Network::OnBtnCipSet8570() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();		
		return;
	}
	
	BYTE nField[4];
	m_cip_8570.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "Next Machine IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP Next Tester");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_8570.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_8570].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_8570", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSet8570() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_8570_CLIENT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Rear port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_8570.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_8570_CLIENT))->SetWindowText(strport);
		
		st_client[CLS_8570].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_8570", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCipSetVision1() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_ec.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "Density Vision IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP Density Vision");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_vis1.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_VIS_DENSITY].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_VISION1", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCipSetVision2() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_ec.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "Barcode Vision IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP Barcode Vision");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_vis2.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_VIS_BARCODE].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_VISION2", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCipSetVision3() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_ec.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "TOP VISION IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP TOP VISION");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_vis3.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_VIS_TOP].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_VISION3", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCipSetVision4() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_ec.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "BTM VISION IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP BTM VISION");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_vis4.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_VIS_BTM].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_VISION4", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetVision1() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_VISION1))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Vision1 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
//	m_btn_cport_set_vis1.GetWindowRect(&r);
	m_btn_cport_set_next.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_VISION1))->SetWindowText(strport);
		
		st_client[CLS_VIS_DENSITY].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_VISION1", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetVision2() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_NEXT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Vision2 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
//	m_btn_cport_set_vis2.GetWindowRect(&r);
	m_btn_cport_set_format.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_VISION2))->SetWindowText(strport);
		
		st_client[CLS_VIS_BARCODE].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_VISION2", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetVision3() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_NEXT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Vision3 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
//	m_btn_cport_set_vis3.GetWindowRect(&r);
	m_btn_cport_set_label_printer1.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_VISION3))->SetWindowText(strport);
		
		st_client[CLS_VIS_TOP].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_VISION3", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetVision4() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_NEXT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Vision4 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
//	m_btn_cport_set_vis4.GetWindowRect(&r);
	m_btn_cport_set_label_printer1.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_VISION4))->SetWindowText(strport);
		
		st_client[CLS_VIS_BTM].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_VISION4", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCntEc() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, CLS_ECSERVER);	
}

void CScreen_Set_Network::OnBtnCntBottom() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_CONNECT, CLS_BOTTOM);	
	
}

void CScreen_Set_Network::OnBtnCntNext2() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_3, CLIENT_CONNECT, CLS_8570);	
	
}

void CScreen_Set_Network::OnBtnCntNext() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_4, CLIENT_CONNECT, CLS_NEXT);	
	
}

void CScreen_Set_Network::OnBtnCntFormat() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_5, CLIENT_CONNECT, CLS_FORMAT);	
	
}


void CScreen_Set_Network::OnBtnCntLabelPrinter1() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_7, CLIENT_CONNECT, CLS_BCR_PRINTER1);	
	
}

void CScreen_Set_Network::OnBtnCntLabelPrinter2() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_8, CLIENT_CONNECT, CLS_BCR_PRINTER2);	
	
}

void CScreen_Set_Network::OnBtnCntLabelPrinter3() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_9, CLIENT_CONNECT, CLS_BCR_PRINTER3);	
	
}

void CScreen_Set_Network::OnBtnCntLabelPrinter4() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_10, CLIENT_CONNECT, CLS_BCR_PRINTER4);	
	
}


void CScreen_Set_Network::OnBtnCntVision1() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_11, CLIENT_CONNECT, CLS_VIS_DENSITY);	
	
}

void CScreen_Set_Network::OnBtnCntVision2() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_12, CLIENT_CONNECT, CLS_VIS_BARCODE);	
	
}

void CScreen_Set_Network::OnBtnCntVision3() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_13, CLIENT_CONNECT, CLS_VIS_TOP);	
	
}

void CScreen_Set_Network::OnBtnCntVision4() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_14, CLIENT_CONNECT, CLS_VIS_BTM);	
	
}

void CScreen_Set_Network::OnBtnSportSetFront() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_FRONT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Front port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_front.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_FRONT))->SetWindowText(strport);
		
		st_server[0].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_FRONT", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnSportSetBarcode1() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE1))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("barcode1 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_front.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE1))->SetWindowText(strport);
		
		st_server[1].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_BCR1", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnSportSetBarcode2() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE2))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("barcode2 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_front.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE2))->SetWindowText(strport);
		
		st_server[2].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_BCR2", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnSportSetBarcode3() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE3))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("barcode3 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_front.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE3))->SetWindowText(strport);
		
		st_server[3].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_BCR3", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnSportSetBarcode4() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE4))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("barcode4 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_front.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_BARCODE4))->SetWindowText(strport);
		
		st_server[4].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_BCR4", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnCipSetBottom() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_bottom.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "EC Server IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP Next Step APCT");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_bottom.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_BOTTOM].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_BOTTOM", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetBottom() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_BOTTOM_CLIENT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Rear APCT port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_bottom.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_BOTTOM_CLIENT))->SetWindowText(strport);
		
		st_client[CLS_BOTTOM].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_BOTTOM", LPCTSTR(strport), st_path.mstr_basic);
	}
}

void CScreen_Set_Network::OnBtnCipSetFormat() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();		
		return;
	}
	
	BYTE nField[4];
	m_cip_format.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "Format IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP label_printer2");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_format.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_FORMAT].str_ip = strip;
		:: WritePrivateProfileString("ADMIN", "CIP_FORMAT", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetFormat() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_FORMAT))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("Format port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_format.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_FORMAT))->SetWindowText(strport);
		
		st_client[CLS_FORMAT].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "CPORT_FORMAT", LPCTSTR(strport), st_path.mstr_basic);
	}
}

void CScreen_Set_Network::OnBtnCipSetLabelPrinter2() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_label_printer2.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "barcode_printer 2번 IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP barcode_printer 2");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_label_printer2.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_BCR_PRINTER2].str_ip = strip;
 		:: WritePrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_2", LPCTSTR(strip), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnCportSetLabelPrinter2() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER2))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("label_printer2 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	m_btn_cport_set_ec.GetWindowRect(&r);
	
//	m_btn_cport_set_label_printer2.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER2))->SetWindowText(strport);
		
		st_client[CLS_BCR_PRINTER2].n_port = atoi(strport);
 		:: WritePrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_2", LPCTSTR(strport), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCipSetLabelPrinter3() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_label_printer3.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "barcode_printer 3번 IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP barcode_printer 3");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_label_printer3.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_BCR_PRINTER3].str_ip = strip;
 		:: WritePrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_3", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetLabelPrinter3() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER3))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("label_printer3 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_cport_set_bottom.GetWindowRect(&r);
//	m_btn_cport_set_label_printer3.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER3))->SetWindowText(strport);
		
		st_client[CLS_BCR_PRINTER3].n_port = atoi(strport);
 		:: WritePrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_3", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnCipSetLabelPrinter4() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	BYTE nField[4];
	m_cip_label_printer2.GetAddress( nField[0], nField[1], nField[2], nField[3] );
	
	CString strip;
	strip.Format( "%d.%d.%d.%d", nField[0], nField[1], nField[2], nField[3] );
	
	st_msg.mstr_keypad_msg = "barcode_printer 4번 IP를 입력하세요.";
	if (g_local.GetLocalType() != LOCAL_KOR)	st_msg.mstr_keypad_msg = _T("Enter the IP barcode_printer 4");
	
	if (strip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = strip;
	}
	
	CDialog_Keyboard keyboard_dlg;
	int n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strip = st_msg.mstr_typing_msg;
		
		strip.MakeUpper();
		strip.TrimLeft(' ');               
		strip.TrimRight(' ');
		
		
		sscanf( (LPCSTR)strip, "%d.%d.%d.%d", &nField[0], &nField[1], &nField[2], &nField[3] );
		
		m_cip_label_printer4.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		
		st_client[CLS_BCR_PRINTER4].str_ip = strip;
 		:: WritePrivateProfileString("ADMIN", "CIP_LABEL_PRINTER_4", LPCTSTR(strip), st_path.mstr_basic);
	}	
}

void CScreen_Set_Network::OnBtnCportSetLabelPrinter4() 
{
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER4))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("label_printer4 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
//	m_btn_cport_set_label_printer4.GetWindowRect(&r);
	m_btn_cport_set_8570.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_CPORT_LABEL_PRINTER4))->SetWindowText(strport);
		
		st_client[CLS_BCR_PRINTER4].n_port = atoi(strport);
 		:: WritePrivateProfileString("ADMIN", "CPORT_LABEL_PRINTER_4", LPCTSTR(strport), st_path.mstr_basic);
	}			
}

////2014,1130
void CScreen_Set_Network::OnBtnSportSetVision1() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON1))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("vision1 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_vision1.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON1))->SetWindowText(strport);
		
		st_server[6].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_VISION1", LPCTSTR(strport), st_path.mstr_basic);
	}			
}

void CScreen_Set_Network::OnBtnSportSetVision2() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON2))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("vision2 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_vision2.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON2))->SetWindowText(strport);
		
		st_server[7].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_VISION2", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnSportSetVision3() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON3))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("vision3 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_vision3.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON3))->SetWindowText(strport);
		
		st_server[8].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_VISION3", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

void CScreen_Set_Network::OnBtnSportSetVision4() 
{
	// TODO: Add your control notification handler code here
	if( st_handler.mn_level_teach != TRUE )
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if( g_local.GetLocalType() != LOCAL_KOR )	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		CDialog_Message msg_dlg;
		msg_dlg.DoModal();
		return;
	}
	
	CString strport;
	((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON4))->GetWindowText(strport);
	strport.TrimLeft(' ');	
	strport.TrimRight(' ');
	
	st_msg.mstr_keypad_msg = _T("vision4 port Setting");
	
	st_msg.mstr_keypad_val = strport;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_sport_set_vision4.GetWindowRect(&r);
	
	CDialog_KeyPad pad_dlg;
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	if (pad_dlg.DoModal() == IDOK)
	{
		strport = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SPORT_VISON4))->SetWindowText(strport);
		
		st_server[9].n_port = atoi(strport);
		:: WritePrivateProfileString("ADMIN", "SPORT_VISION4", LPCTSTR(strport), st_path.mstr_basic);
	}		
}

////
int CScreen_Set_Network::ConverterToData(int mode, int pos)
{
	int Ret;

	switch(mode)
	{
		case COM_PORT:
			switch(pos)
			{
				case 0:
					Ret = 1;
					break;
				case 1:
					Ret = 2;
					break;
				case 2:
					Ret = 3;
					break;
				case 3:
					Ret = 4;
					break;
				case 4:
					Ret = 5;
					break;
				case 5:
					Ret = 6;
					break;
				case 6:
					Ret = 7;
					break;
				case 7:
					Ret = 8;
					break;
			}
			break;
		case COM_BAUDRATE:
			switch(pos)
			{
				case 0:
					Ret = 2400;
					break;
				case 1:
					Ret = 4800;
					break;
				case 2:
					Ret = 9600;
					break;
				case 3:
					Ret = 14400;
					break;
				case 4:
					Ret = 19200;
					break;
				case 5:
					Ret = 38400;
					break;
				case 6:
					Ret = 57600;
					break;
				case 7:
					Ret = 115200;
					break;
			}
			break;
		case COM_DATA:
			switch(pos)
			{
				case 0:
					Ret = 5;
					break;
				case 1:
					Ret = 6;
					break;
				case 2:
					Ret = 7;
					break;
				case 3:
					Ret = 8;
					break;
			}
			break;
		case COM_STOP:
			switch(pos)
			{
				case 0:
					Ret = 1;
					break;
				case 1:
					Ret = 2;
					break;
			}
			break;
		case COM_PARITY:
			switch(pos)
			{
				case 0:
					Ret = 0;
					break;
				case 1:
					Ret = 1;
					break;
				case 2:
					Ret = 2;
					break;
			}
			break;
	}

	return Ret;
}

int CScreen_Set_Network::ConverterToPos(int mode, int data)
{
	int Ret;

	switch(mode)
	{
		case COM_PORT:
			switch(data)
			{
				case 1:
					Ret = 0;
					break;
				case 2:
					Ret = 1;
					break;
				case 3:
					Ret = 2;
					break;
				case 4:
					Ret = 3;
					break;
				case 5:
					Ret = 4;
					break;
				case 6:
					Ret = 5;
					break;
				case 7:
					Ret = 6;
					break;
				case 8:
					Ret = 7;
					break;
			}
			break;
		case COM_BAUDRATE:
			switch(data)
			{
				case 2400:
					Ret = 0;
					break;
				case 4800:
					Ret = 1;
					break;
				case 9600:
					Ret = 2;
					break;
				case 14400:
					Ret = 3;
					break;
				case 19200:
					Ret = 4;
					break;
				case 38400:
					Ret = 5;
					break;
				case 57600:
					Ret = 6;
					break;
				case 115200:
					Ret = 7;
					break;
			}
			break;
		case COM_DATA:
			switch(data)
			{
				case 5:
					Ret = 0;
					break;
				case 6:
					Ret = 1;
					break;
				case 7:
					Ret = 2;
					break;
				case 8:
					Ret = 3;
					break;
			}
			break;
		case COM_STOP:
			switch(data)
			{
				case 1:
					Ret = 0;
					break;
				case 2:
					Ret = 1;
					break;
			}
			break;
		case COM_PARITY:
			switch(data)
			{
				case 0:
					Ret = 0;
					break;
				case 1:
					Ret = 1;
					break;
				case 2:
					Ret = 2;
					break;
			}
			break;
	}

	return Ret;
}

void CScreen_Set_Network::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
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

void CScreen_Set_Network::GridControl(UINT nID, int type, int row, int col, int pos)
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
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
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

void CScreen_Set_Network::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

CString CScreen_Set_Network::GetGridData( UINT nID, int row, int col )
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

void CScreen_Set_Network::GridFont(UINT nID, int row, int col, int size)
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

void CScreen_Set_Network::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Set_Network::OnBtnTstNext() 
{
	g_next_client.OnMove_BufferIn();
	
}
