#if !defined(AFX_SCREEN_SET_NETWORK_H__608FCDA0_1E72_4A65_9593_128A1FEAC247__INCLUDED_)
#define AFX_SCREEN_SET_NETWORK_H__608FCDA0_1E72_4A65_9593_128A1FEAC247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Set_Network.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 
#include "Digit.h"

#include "SerialComm.h"     // 시리얼 통신 클래스
/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Network form view
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Set_Network : public CFormView
{
// Construction
public:
	CScreen_Set_Network();   // standard constructor
	DECLARE_DYNCREATE(CScreen_Set_Network)

// FormDialog Data
	//{{AFX_DATA(CScreen_Set_Network)
	enum { IDD = IDD_SCREEN_SET_NETWORK };
	CMacButton	m_btn_cip_set_label_printer1;
	CMacButton	m_btn_test_next;
	CMacButton	m_btn_test_ec;
	CMacButton	m_btn_cip_set_ec;
	CGradientStatic	m_msg_ip_format;
	CGradientStatic	m_msg_cport_format;
	CXPGroupBox	m_group_client_format;
	CEdit	m_edit_cport_format_client;
	CIPAddressCtrl	m_cip_format;
	CMacButton	m_btn_test_format;
	CMacButton	m_btn_cport_set_format;
	CMacButton	m_btn_cip_set_format;
	CGradientStatic	m_msg_cport_label_printer4;
	CGradientStatic	m_msg_cport_label_printer3;
	CGradientStatic	m_msg_cport_label_printer2;
	CGradientStatic	m_msg_cport_bottom;
	CGradientStatic	m_msg_ip_label_printer4;
	CGradientStatic	m_msg_ip_bottom;
	CGradientStatic	m_msg_cport_vision4;
	CGradientStatic	m_msg_cport_vision3;
	CGradientStatic	m_msg_cport_vision2;
	CGradientStatic	m_msg_cport_vision1;
	CGradientStatic	m_msg_ip_label_printer3;
	CGradientStatic	m_msg_ip_label_printer2;
	CGradientStatic	m_msg_ip_vision4;
	CGradientStatic	m_msg_ip_vision3;
	CGradientStatic	m_msg_ip_vision2;
	CGradientStatic	m_msg_ip_vision1;
	CGradientStatic	m_msg_sport_barcode4;
	CGradientStatic	m_msg_sport_barcode3;
	CGradientStatic	m_msg_sport_barcode2;
	CGradientStatic	m_msg_sport_barcode1;
	CXPGroupBox	m_group_vision_client4;
	CXPGroupBox	m_group_vision_client3;
	CXPGroupBox	m_group_vision_client2;
	CXPGroupBox	m_group_vision_client1;
	CXPGroupBox	m_group_client_bottom;
	CXPGroupBox	m_group_client_bcr_printer4;
	CXPGroupBox	m_group_client_bcr_printer3;
	CXPGroupBox	m_group_client_bcr_printer2;
	CXPGroupBox	m_group_client_bcr_printer1;
	CEdit	m_edit_cport_next;
	CEdit	m_edit_cport_label_printer4;
	CEdit	m_edit_cport_label_printer3;
	CEdit	m_edit_cport_label_printer2;
	CEdit	m_edit_cport_bottom_client;
	CIPAddressCtrl	m_cip_label_printer4;
	CMacButton	m_btn_cport_set_label_printer4;
	CIPAddressCtrl	m_cip_bottom;
	CIPAddressCtrl	m_cip_8570;
	CMacButton	m_btn_test_vision4;
	CMacButton	m_btn_test_vision3;
	CMacButton	m_btn_test_vision2;
	CMacButton	m_btn_test_vision1;
	CMacButton	m_btn_test_label_printer4;
	CMacButton	m_btn_test_label_printer3;
	CMacButton	m_btn_test_label_printer2;
	CMacButton	m_btn_test_label_printer1;
	CMacButton	m_btn_test_bottom;
	CMacButton	m_btn_test_8570;
	CMacButton	m_btn_sport_set_barcode4;
	CMacButton	m_btn_sport_set_barcode3;
	CMacButton	m_btn_sport_set_barcode2;
	CMacButton	m_btn_sport_set_barcode1;
	CMacButton	m_btn_cport_set_label_printer3;
	CMacButton	m_btn_cport_set_label_printer2;
	CMacButton	m_btn_cport_set_bottom;
	CMacButton	m_btn_cip_set_label_printer4;
	CMacButton	m_btn_cip_set_label_printer3;
	CMacButton	m_btn_cip_set_label_printer2;
	CMacButton	m_btn_cip_set_bottom;
	CIPAddressCtrl	m_cip_label_printer3;
	CIPAddressCtrl	m_cip_label_printer2;
	CEdit	m_edit_cport_label_printer1;
	CEdit	m_edit_cport_vis4;
	CEdit	m_edit_cport_vis3;
	CEdit	m_edit_cport_vis2;
	CEdit	m_edit_cport_vis1;
	CEdit	m_edit_cport_ec_client;
	CEdit	m_edit_cport_8570_client;
	CIPAddressCtrl	m_cip_vis4;
	CIPAddressCtrl	m_cip_vis3;
	CIPAddressCtrl	m_cip_vis2;
	CIPAddressCtrl	m_cip_vis1;
	CMacButton	m_btn_cport_set_vis4;
	CMacButton	m_btn_cport_set_vis3;
	CMacButton	m_btn_cport_set_vis2;
	CMacButton	m_btn_cport_set_vis1;
	CMacButton	m_btn_cip_set_vis4;
	CMacButton	m_btn_cip_set_vis3;
	CMacButton	m_btn_cip_set_vis2;
	CMacButton	m_btn_cip_set_vis1;
	CMacButton	m_btn_cport_set_bcr4;
	CMacButton	m_btn_cport_set_8570;
	CMacButton	m_btn_cip_set_8570;
	CGradientStatic	m_msg_cport_8570;
	CGradientStatic	m_msg_ip8570;
	CXPGroupBox	m_group_client_8570;
	CMacButton	m_btn_cip_set_next;
	CMacButton	m_btn_cport_set_ec;
	CMacButton	m_btn_cport_set_label_printer1;
	CMacButton	m_btn_cport_set_next;
	CMacButton	m_btn_sport_set_front;
	CIPAddressCtrl	m_cip_label_printer1;
	CIPAddressCtrl	m_cip_ec;
	CIPAddressCtrl	m_cip_next;
	CGradientStatic	m_msg_cip_ec;
	CGradientStatic	m_msg_cport_ec;
	CGradientStatic	m_msg_sport_front;
	CGradientStatic	m_msg_cip_label_printer1;
	CGradientStatic	m_msg_cip_next;
	CGradientStatic	m_msg_cport_next;
	CGradientStatic	m_msg_cport_label_printer1;
	CXPGroupBox	m_group_client_next;
	CXPGroupBox	m_group_client_ec;
	CXPGroupBox	m_group_server;
	CXPGroupBox	m_group_serial;
	CXPGroupBox	m_group_serial_1;
	CXPGroupBox	m_group_serial_2;
	CXPGroupBox	m_group_serial_3;
	CXPGroupBox	m_group_serial_4;
	CXPGroupBox	m_group_serial_5;
	CXPGroupBox	m_group_serial_6;
	CXPGroupBox	m_group_serial_7;
	CComboBox	m_cb_port_1;
	CComboBox	m_cb_rate_1;
	CComboBox	m_cb_data_1;
	CComboBox	m_cb_stop_1;
	CComboBox	m_cb_parity_1;
	CComboBox	m_cb_port_2;
	CComboBox	m_cb_rate_2;
	CComboBox	m_cb_data_2;
	CComboBox	m_cb_stop_2;
	CComboBox	m_cb_parity_2;
	CComboBox	m_cb_port_3;
	CComboBox	m_cb_rate_3;
	CComboBox	m_cb_data_3;
	CComboBox	m_cb_stop_3;
	CComboBox	m_cb_parity_3;
	CComboBox	m_cb_port_4;
	CComboBox	m_cb_rate_4;
	CComboBox	m_cb_data_4;
	CComboBox	m_cb_stop_4;
	CComboBox	m_cb_parity_4;
	CComboBox	m_cb_port_5;
	CComboBox	m_cb_rate_5;
	CComboBox	m_cb_data_5;
	CComboBox	m_cb_stop_5;
	CComboBox	m_cb_parity_5;
	CComboBox	m_cb_port_6;
	CComboBox	m_cb_rate_6;
	CComboBox	m_cb_data_6;
	CComboBox	m_cb_stop_6;
	CComboBox	m_cb_parity_6;
	CComboBox	m_cb_port_7;
	CComboBox	m_cb_rate_7;
	CComboBox	m_cb_data_7;
	CComboBox	m_cb_stop_7;
	CComboBox	m_cb_parity_7;
	CGradientStatic	m_msg_port_1;
	CGradientStatic	m_msg_data_1;
	CGradientStatic	m_msg_parity_1;
	CGradientStatic	m_msg_rate_1;
	CGradientStatic	m_msg_stop_1;
	CGradientStatic	m_msg_port_2;
	CGradientStatic	m_msg_data_2;
	CGradientStatic	m_msg_parity_2;
	CGradientStatic	m_msg_rate_2;
	CGradientStatic	m_msg_stop_2;
	CGradientStatic	m_msg_port_3;
	CGradientStatic	m_msg_data_3;
	CGradientStatic	m_msg_parity_3;
	CGradientStatic	m_msg_rate_3;
	CGradientStatic	m_msg_stop_3;
	CGradientStatic	m_msg_port_4;
	CGradientStatic	m_msg_data_4;
	CGradientStatic	m_msg_parity_4;
	CGradientStatic	m_msg_rate_4;
	CGradientStatic	m_msg_stop_4;
	CGradientStatic	m_msg_port_5;
	CGradientStatic	m_msg_data_5;
	CGradientStatic	m_msg_parity_5;
	CGradientStatic	m_msg_rate_5;
	CGradientStatic	m_msg_stop_5;
	CGradientStatic	m_msg_port_6;
	CGradientStatic	m_msg_data_6;
	CGradientStatic	m_msg_parity_6;
	CGradientStatic	m_msg_rate_6;
	CGradientStatic	m_msg_stop_6;
	CGradientStatic	m_msg_port_7;
	CGradientStatic	m_msg_data_7;
	CGradientStatic	m_msg_parity_7;
	CGradientStatic	m_msg_rate_7;
	CGradientStatic	m_msg_stop_7;
	CMacButton	m_btn_serial_connect_1;
	CMacButton	m_btn_serial_apply_1;
	CMacButton	m_btn_serial_connect_2;
	CMacButton	m_btn_serial_apply_2;
	CMacButton	m_btn_serial_connect_3;
	CMacButton	m_btn_serial_apply_3;
	CMacButton	m_btn_serial_connect_4;
	CMacButton	m_btn_serial_apply_4;
	CMacButton	m_btn_serial_connect_5;
	CMacButton	m_btn_serial_apply_5;
	CMacButton	m_btn_serial_connect_6;
	CMacButton	m_btn_serial_apply_6;
	CMacButton	m_btn_serial_connect_7;
	CMacButton	m_btn_serial_apply_7;

	////2014,1130
	CMacButton	m_btn_sport_set_vision1;
	CMacButton	m_btn_sport_set_vision2;
	CMacButton	m_btn_sport_set_vision3;
	CMacButton	m_btn_sport_set_vision4;
	CGradientStatic	m_msg_sport_vision1;
	CGradientStatic	m_msg_sport_vision2;
	CGradientStatic	m_msg_sport_vision3;
	CGradientStatic	m_msg_sport_vision4;
	////
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스

	CEXDigitST		m_dgt_autosocketoff_cnt;
	CEditEx	 m_edit_sport_front;
	CEditEx	 m_edit_sport_bcr1;
	CEditEx	 m_edit_sport_bcr2;
	CEditEx	 m_edit_sport_bcr3;
	CEditEx	 m_edit_sport_bcr4;
	
	////2014,1130
	CEditEx	 m_edit_sport_vision1;
	CEditEx	 m_edit_sport_vision2;
	CEditEx	 m_edit_sport_vision3;
	CEditEx	 m_edit_sport_vision4;
	////

	int	m_n_port[MAX_PORT][2];
	int	m_n_rate[MAX_PORT][2];
	int	m_n_data[MAX_PORT][2];
	int	m_n_stop[MAX_PORT][2];
	int	m_n_parity[MAX_PORT][2];
	
	/* ************************************************************************** */
	/* 시리얼 통신 관련 변수 선언                                                 */
	/* ************************************************************************** */
	SerialComm m_ports[MAX_PORT];       // 시리얼 포트 클래스 변수

// Operations
public:
	void OnAdmin_Data_Set();
	void OnAdmin_Label_Set();
	void OnAdmin_EditBox_Set();
	void OnAdmin_GroupBox_Set();
	void OnAdmin_Dgt_Set();
	void OnAdmin_Display();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Set_Network)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	protected:
		virtual ~CScreen_Set_Network();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
		void GridControl(UINT nID, int type, int row, int col, int pos);
		void GridData(UINT nID, int row, int col, CString data);
		void GridFont(UINT nID, int row, int col, int size);
		void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
		CString GetGridData( UINT nID, int row, int col );
		int ConverterToData(int mode, int pos);
		int ConverterToPos(int mode, int data);

	// Generated message map functions
	//{{AFX_MSG(CScreen_Set_Network)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnCipSetEc();
	afx_msg void OnBtnCportSetEc();
	afx_msg void OnBtnCipSetBottom();
	afx_msg void OnBtnCportSetBottom();
	afx_msg void OnBtnCipSet8570();
	afx_msg void OnBtnCportSet8570();
	afx_msg void OnBtnCipSetNext();
	afx_msg void OnBtnCportSetNext();
	afx_msg void OnBtnCipSetFormat();
	afx_msg void OnBtnCportSetFormat();
	afx_msg void OnBtnCipSetLabelPrinter1();
	afx_msg void OnBtnCportSetLabelPrinter1();
	afx_msg void OnBtnCipSetLabelPrinter2();
	afx_msg void OnBtnCportSetLabelPrinter2();
	afx_msg void OnBtnCipSetLabelPrinter3();
	afx_msg void OnBtnCportSetLabelPrinter3();
	afx_msg void OnBtnCipSetLabelPrinter4();
	afx_msg void OnBtnCportSetLabelPrinter4();
	afx_msg void OnBtnCipSetVision1();
	afx_msg void OnBtnCportSetVision1();
	afx_msg void OnBtnCipSetVision2();
	afx_msg void OnBtnCportSetVision2();
	afx_msg void OnBtnCipSetVision3();
	afx_msg void OnBtnCportSetVision3();
	afx_msg void OnBtnCipSetVision4();
	afx_msg void OnBtnCportSetVision4();
	afx_msg void OnBtnCntEc();
	afx_msg void OnBtnCntBottom();
	afx_msg void OnBtnCntNext2();
	afx_msg void OnBtnCntNext();
	afx_msg void OnBtnCntFormat();
	afx_msg void OnBtnCntLabelPrinter1();
	afx_msg void OnBtnCntLabelPrinter2();
	afx_msg void OnBtnCntLabelPrinter3();
	afx_msg void OnBtnCntLabelPrinter4();
	afx_msg void OnBtnCntVision1();
	afx_msg void OnBtnCntVision2();
	afx_msg void OnBtnCntVision3();
	afx_msg void OnBtnCntVision4();
	afx_msg void OnBtnSportSetFront();
	afx_msg void OnBtnSportSetBarcode1();
	afx_msg void OnBtnSportSetBarcode2();
	afx_msg void OnBtnSportSetBarcode3();
	afx_msg void OnBtnSportSetBarcode4();
	afx_msg void OnBtnSportSetVision1();
	afx_msg void OnBtnSportSetVision2();
	afx_msg void OnBtnSportSetVision3();
	afx_msg void OnBtnSportSetVision4();
	afx_msg void OnBtnSerialApply1();
	afx_msg void OnBtnSerialConnect1();
	afx_msg void OnBtnSerialApply3();
	afx_msg void OnBtnSerialConnect3();
	afx_msg void OnBtnSerialApply5();
	afx_msg void OnBtnSerialConnect5();
	afx_msg void OnBtnSerialApply7();
	afx_msg void OnBtnSerialConnect7();
	afx_msg void OnBtnSerialApply2();
	afx_msg void OnBtnSerialConnect2();
	afx_msg void OnBtnSerialApply4();
	afx_msg void OnBtnSerialConnect4();
	afx_msg void OnBtnSerialApply6();
	afx_msg void OnBtnSerialConnect6();
	afx_msg void OnDgtAutoSocketOff();
	afx_msg void OnBtnTstNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_SET_NETWORK_H__608FCDA0_1E72_4A65_9593_128A1FEAC247__INCLUDED_)
