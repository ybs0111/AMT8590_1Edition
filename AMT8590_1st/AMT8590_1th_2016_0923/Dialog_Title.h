#if !defined(AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_)
#define AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Title.h : header file
//
#include "InitDialogBar.h" // Initial Dialog Bar 생성 클래스 추가 
/////////////////////////////////////////////////////////////////////////////
// CDialog_Title dialog
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "MenuHead.h"		// 화면에 대한 헤더 정의된 클래스 추가
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "PictureEx.h"		// GIF 파일을 로딩하기 위한 클래스 추가 
#include "VLabel.h"			// 칼라 텍스트 박스 클래스

#include <process.h>
#include <afxmt.h>

#define TMR_SMEMA_OFF	849
class CDialog_Title : public CInitDialogBar
{
// Construction
public:
	CDialog_Title();
	// Dialog Data
	//{{AFX_DATA(CDialog_Title)
	enum { IDD = IDD_DIALOG_TITLE };
	CGradientStatic	m_title_xgem;
	CGradientStatic	m_title_conveyor_inline_mode;
	CGradientStatic	m_title_emptytray_mode;
	CGradientStatic	m_title_online_mode;
	CGradientStatic	m_title_rear_key;
	CGradientStatic	m_title_front_key;
	CGradientStatic	m_title_level_mode;
	CGradientStatic	m_title_device_mode;
	CGradientStatic	m_msg_status_info;
	CNewLabel	m_msg_form_name;
	CPictureEx	m_gif_title_logo;
	//}}AFX_DATA

private:
	CFont* mp_title_font;	// 폰트 변수
	int m_nBCR_Mode;

public:
	void OnTitle_EmptyTray_Mode_Display();
	void OnTitle_Online_Mode_Display();
	void OnTitle_Conv_Inline_Display(int n_mode);
	void OnTitle_Rear_Key_Display(int n_key);
	void OnTitle_Front_Key_Display(int n_key);
	void OnTitle_Level_mode_Display();
	void OnTitle_Device_mode_Display(int n_device);
	void OnTitle_Status_Change(int n_status);
	void OnTitle_Form_Name_Change(int nViewID);
	void OnTitle_Lable_Set();
	void OnTitle_Picture_Set();
	void OnBuildBCRMode();					// 작업중인 BCR 정보 표시
	void OnTitle_Xgem_mode_Display(int n_gem);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Title)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int n_smemaonoff;

	// Generated message map functions
	//{{AFX_MSG(CDialog_Title)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialogBar();
	//}}AFX_MSG
	afx_msg  LRESULT OnStatusDataUpdate(WPARAM wParam,LPARAM lParam) ;  // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_)
