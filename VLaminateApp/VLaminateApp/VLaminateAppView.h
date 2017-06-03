
// VLaminateAppView.h : CVLaminateAppView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "DlgExecAuto.h"
#include "DlgSetting.h"
#include "DlgPlot.h"
#include "DlgQuery.h"
#include "DlgResult.h"
#include "DlgMainBar.h"
#include "DlgSubBar.h"
#include "afxwin.h"


class CVLaminateAppView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CVLaminateAppView();
	DECLARE_DYNCREATE(CVLaminateAppView)

public:
	enum{ IDD = IDD_VLaminateApp_FORM };

    void            InitDialogs();
    void            SelectDialog(int nSel);

// Ư���Դϴ�.
public:
//	CVLaminateAppDoc* GetDocument() const;
	
// �۾��Դϴ�.
public:
	CDlgSetting     *m_pDlgSetting;
	CDlgResult      *m_pDlgResult;
	CDlgQuery       *m_pDlgQuery;
	CDlgPlot        *m_pDlgPlot;
	CDlgExecAuto    *m_pDlgExecAuto;

	CDlgMainBar     *m_pDlgMainBar;
	CDlgSubBar      *m_pDlgSubBar;
//	CDlgTestShort   *m_pDlgTestShort;

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CVLaminateAppView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg LONG OnDeleteDataPath(UINT, LONG);	

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_fmDialog;
};

/*
#ifndef _DEBUG  // VLaminateAppView.cpp�� ����� ����
inline CVLaminateAppDoc* CVLaminateAppView::GetDocument() const
   { return reinterpret_cast<CVLaminateAppDoc*>(m_pDocument); }
#endif
   */

