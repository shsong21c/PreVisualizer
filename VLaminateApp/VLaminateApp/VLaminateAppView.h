
// VLaminateAppView.h : CVLaminateAppView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CVLaminateAppView();
	DECLARE_DYNCREATE(CVLaminateAppView)

public:
	enum{ IDD = IDD_VLaminateApp_FORM };

    void            InitDialogs();
    void            SelectDialog(int nSel);

// 특성입니다.
public:
//	CVLaminateAppDoc* GetDocument() const;
	
// 작업입니다.
public:
	CDlgSetting     *m_pDlgSetting;
	CDlgResult      *m_pDlgResult;
	CDlgQuery       *m_pDlgQuery;
	CDlgPlot        *m_pDlgPlot;
	CDlgExecAuto    *m_pDlgExecAuto;

	CDlgMainBar     *m_pDlgMainBar;
	CDlgSubBar      *m_pDlgSubBar;
//	CDlgTestShort   *m_pDlgTestShort;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CVLaminateAppView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg LONG OnDeleteDataPath(UINT, LONG);	

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_fmDialog;
};

/*
#ifndef _DEBUG  // VLaminateAppView.cpp의 디버그 버전
inline CVLaminateAppDoc* CVLaminateAppView::GetDocument() const
   { return reinterpret_cast<CVLaminateAppDoc*>(m_pDocument); }
#endif
   */

