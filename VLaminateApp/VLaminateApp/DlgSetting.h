#pragma once

#include "Controls\\dsspanel.h"
#include "Controls\\BtnSt.h"
#include "Controls\\EditEx.h"
#include "afxwin.h"

#define BASE_GRID_COLOR           RGB(150, 190, 250)
#define AGING_GRID_COLOR          RGB(80, 150, 250)
#define BAD_GRID_COLOR            RGB(250, 170, 250)
#define NONE_GRID_COLOR           RGB(190, 190, 190)
#define TOUCH_GRID_COLOR          RGB(150, 230, 150)
#define LIGHT_ON_COLOR            RGB(0, 255, 0) 

// CDlgSetting ��ȭ �����Դϴ�.
class CDlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgSetting();
	
	void    InitButtonControls();
	void    InitSetupControls();
	void    UpdateControlsData();
	void    ApplySpecSetup();
	void    SetTransLanguage(long nValue);
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP };

	CButtonST	 m_btnIN[20];
	CButtonST	 m_btnOUT[20];
	CButtonST    m_btnExecCam1;
	CButtonST    m_btnExecCam2;
	CButtonST    m_btnOkSaveCam1;
	CButtonST    m_btnOkSaveCam2;
	CButtonST    m_btnNgSaveCam1;
	CButtonST    m_btnNgSaveCam2;
	CButtonST 	 m_btnDataTrans;
	CButtonST    m_btnKorea;
	CButtonST    m_btnSWGrab;
	CButtonST    m_btnEnglish;
	CButtonST    m_btnChiness;
	CButtonST 	 m_btnCutOffset;
	CButtonST 	 m_btnEPCOffset;
	CButtonST 	 m_btnT2BOffset;
	CButtonST    m_btnAlignUP;
	CButtonST    m_btnAlignDN;
	CButtonST    m_btnApply;
	CBitmap      m_bitmapLang;

	CEditEx		 m_editPeriod;
	CEditEx		 m_editDriver;
	CEditEx		 m_editSkipCnt;
	CEditEx		 m_editDaqCnt;
	CEditEx      m_editDelayTime;
	CEditEx      m_editUpSaveType;
	CEditEx      m_editDnSaveType;
	
	CEditEx      m_editTapeX1;
	CEditEx      m_editTapeX2;
	CEditEx      m_editTapeX3;
	CEditEx      m_editTapeX4;
	CEditEx      m_editTapeX5;
	CEditEx      m_editTapeX6;
	CEditEx      m_editTapeX7;
	CEditEx      m_editTapeX8;
	CEditEx      m_editTapeY1;
	CEditEx      m_editTapeY2;
	CEditEx      m_editTapeY3;
	CEditEx      m_editTapeY4;
	CEditEx      m_editTapeY5;
	CEditEx      m_editTapeY6;
	CEditEx      m_editTapeY7;
	CEditEx      m_editTapeY8;

	CDSSPanel    m_panSpec;

	BOOL         m_CAM[GOM_MAX_NUM];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnExecCam1();
	afx_msg void OnBnClickedBtnOksaveCam1();
	afx_msg void OnBnClickedBtnNgsaveCam1();
	afx_msg void OnBnClickedBtnExecCam2();
	afx_msg void OnBnClickedBtnOksaveCam2();
	afx_msg void OnBnClickedBtnNgsaveCam2();
	afx_msg void OnDeltaposSpinDrive(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPeriod(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckOut0();
	afx_msg void OnBnClickedCheckOut1();
	afx_msg void OnBnClickedCheckOut2();
	afx_msg void OnBnClickedCheckOut3();
	afx_msg void OnBnClickedCheckOut4();
	afx_msg void OnBnClickedCheckOut5();
	afx_msg void OnBnClickedCheckOut6();
	afx_msg void OnBnClickedCheckOut7();
	afx_msg void OnBnClickedCheckOut8();
	afx_msg void OnBnClickedCheckOut9();
	afx_msg void OnBnClickedCheckOut10();
	afx_msg void OnBnClickedCheckOut11();
	afx_msg void OnBnClickedCheckOut12();
	afx_msg void OnBnClickedCheckOut13();
	afx_msg void OnBnClickedCheckOut14();
	afx_msg void OnBnClickedCheckOut15();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheckExecCam1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckKorea();
	afx_msg void OnBnClickedCheckEnglish();
	afx_msg void OnBnClickedCheckChiness();
	afx_msg void OnBnClickedCheckExecCam2();
	afx_msg void OnBnClickedCheckExecOffset();
	afx_msg void OnDeltaposSpinUpSaveType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinDnSaveType(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_staticSetup;
	afx_msg void OnBnClickedCheckAlignUp();
	afx_msg void OnBnClickedCheckAlignDn();
	afx_msg void OnBnClickedBtnIn0();
};
