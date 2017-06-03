#pragma once

#include "Controls\\BkDialogST.h"
#include "Controls\\BtnST.h"

// CDlgMainBar ��ȭ �����Դϴ�.

class CDlgMainBar : public CBkDialogST 
{
	DECLARE_DYNAMIC(CDlgMainBar)

public:
	CDlgMainBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMainBar();

	void         InitButtons();
	void         OnPostEraseBkgnd(CDC* pDC);
	void         SetTransLanguage(long nSel);
	void         SetCommStatus(long nSel, BOOL bCheck);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAIN_BAR };

	CButtonST	 m_btnToolMain;
	CButtonST	 m_btnToolSearch;
	CButtonST	 m_btnToolResult;
	CButtonST	 m_btnToolSetup;
	CButtonST    m_btnToolPlot;
	CButtonST	 m_btnToolLoad;
	CButtonST	 m_btnToolSave;
	CButtonST	 m_btnToolGrab;
	CButtonST    m_btnToolLive;
	CButtonST	 m_btnToolExit;
	CButtonST	 m_btnToolZoomIn;
	CButtonST    m_btnToolZoomOut;
	CButtonST    m_btnToolFit;
	CButtonST    m_btnToolOrigin;
	CButtonST    m_btnToolPlc;
//	CButtonST    m_btnToolUmac;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnAuto();
	afx_msg void OnBnClickedBtnSetup();
	afx_msg void OnBnClickedBtnPlot();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnGrab();
	afx_msg void OnBnClickedBtnLive();
	afx_msg void OnBnClickedBtnZoomin();
	afx_msg void OnBnClickedBtnZoomout();
	afx_msg void OnBnClickedBtnFit();
	afx_msg void OnBnClickedBtnOrigin();
	afx_msg void OnBnClickedBtnLoadImage();
	afx_msg void OnBnClickedBtnSaveImage();
	afx_msg void OnBnClickedBtnResult();

};
