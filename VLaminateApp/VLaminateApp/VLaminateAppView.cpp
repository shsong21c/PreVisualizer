
// VLaminateAppView.cpp : CVLaminateAppView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "VLaminateApp.h"
#include "MainFrm.h"
#endif

#include "VLaminateAppDoc.h"
#include "VLaminateAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CMainFrame *m_pMainFrm;
// CVLaminateAppView

IMPLEMENT_DYNCREATE(CVLaminateAppView, CFormView)

BEGIN_MESSAGE_MAP(CVLaminateAppView, CFormView)
	ON_MESSAGE(WM_DELETE_DATA, OnDeleteDataPath)
END_MESSAGE_MAP()

// CVLaminateAppView ����/�Ҹ�

CVLaminateAppView::CVLaminateAppView()
	: CFormView(CVLaminateAppView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CVLaminateAppView::~CVLaminateAppView()
{
	delete m_pDlgExecAuto;
	delete m_pDlgResult;
    delete m_pDlgSetting;
    delete m_pDlgQuery;
    delete m_pDlgPlot;

	delete m_pDlgMainBar;
	delete m_pDlgSubBar;
//	delete m_pDlgTestShort;
}

void CVLaminateAppView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DIALOG_SCREEN, m_fmDialog);
}

BOOL CVLaminateAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CVLaminateAppView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();

	m_pMainFrm = (CMainFrame *)AfxGetMainWnd();
	m_pMainFrm->m_pView = this;

    m_fmDialog.SetWindowPos(NULL, DEF_DIALOG_LEFT, DEF_DIALOG_TOP, DEF_DIALOG_WIDTH, DEF_DIALOG_HEIGHT, SWP_SHOWWINDOW);

	InitDialogs();
//  ::PostMessage(this->m_hWnd, WM_DELETE_DATA, 0, 0);
}

void CVLaminateAppView::InitDialogs()
{
    m_pDlgExecAuto = new CDlgExecAuto((CWnd*)this);
    m_pDlgSetting = new CDlgSetting((CWnd*)this);
	m_pDlgResult = new CDlgResult((CWnd*)this);
    m_pDlgPlot = new CDlgPlot((CWnd*)this);
    m_pDlgQuery = new CDlgQuery((CWnd*)this);

	m_pDlgMainBar = new CDlgMainBar((CWnd*)this);
	GetDlgItem(IDC_STATIC_MAIN_BAR)->SetWindowPos(NULL, DEF_MAINBAR_LEFT, DEF_MAINBAR_TOP, DEF_MAINBAR_WIDTH, DEF_MAINBAR_HEIGHT, SWP_SHOWWINDOW);
	m_pDlgMainBar->Create(IDD_DIALOG_MAIN_BAR, GetDlgItem(IDC_STATIC_MAIN_BAR));
	m_pDlgMainBar->ShowWindow(SW_SHOW);

    m_pDlgSubBar = new CDlgSubBar((CWnd*)this);
	GetDlgItem(IDC_STATIC_SUB_BAR)->SetWindowPos(NULL, DEF_SUBBAR_LEFT, DEF_SUBBAR_TOP, DEF_SUBBAR_WIDTH, DEF_SUBBAR_HEIGHT, SWP_SHOWWINDOW);
    m_pDlgSubBar->Create(IDD_DIALOG_SUB_BAR, GetDlgItem(IDC_STATIC_SUB_BAR));
	m_pDlgSubBar->ShowWindow(SW_SHOW);

	m_pDlgExecAuto->Create(IDD_DIALOG_EXEC_AUTO, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgExecAuto->ShowWindow(SW_SHOW);
	m_pDlgSetting->Create(IDD_DIALOG_SETUP, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgPlot->Create(IDD_DIALOG_PLOT, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgResult->Create(IDD_DIALOG_RESULT, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgQuery->Create(IDD_DIALOG_QUERY, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgQuery->InitPlotControl(0);

	SelectDialog(DEF_BUTTON_AUTO);



	/*
    m_pDlgTestShort = new CDlgTestShort((CWnd*)this);
	m_pDlgTestShort->Create(IDD_DIALOG_TEST_SHORT, this);
	m_pDlgTestShort->MoveWindow(DEF_TESTSHORT_LEFT, DEF_TESTSHORT_TOP, DEF_TESTSHORT_WIDTH, DEF_TESTSHORT_HEIGHT);
	m_pDlgTestShort->ShowWindow(SW_SHOW);
	*/
}

void CVLaminateAppView::SelectDialog(int nSel)
{
    CString str;

	m_pDlgExecAuto->ShowWindow(SW_HIDE);
	m_pDlgResult->ShowWindow(SW_HIDE);
	m_pDlgSetting->ShowWindow(SW_HIDE);
	m_pDlgPlot->ShowWindow(SW_HIDE);
	m_pDlgQuery->ShowWindow(SW_HIDE);

	switch(nSel)
	{
		case DEF_BUTTON_AUTO:
        	m_pDlgExecAuto->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[AUTO SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_RESULT:
        	m_pDlgResult->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[RESULT SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_SEARCH:
        	m_pDlgQuery->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[SEARCH SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_PLOT:
        	m_pDlgPlot->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[PLOT SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
			/*
		case BUTTON_REPORT:
         	m_pDlgReport->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+"[REPORT ȭ��]";
			m_pMainFrm->SetWindowText(_T(str));
			break;
			*/
		case DEF_BUTTON_SETUP:
        	m_pDlgSetting->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[SETTING SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_EXIT:
			m_pMainFrm->ExitProgram();
			break;
	}
}

LONG CVLaminateAppView::OnDeleteDataPath(UINT wParam, LONG lParam)
{
	BeginWaitCursor();
//	m_pMainFrm->DeleteDataPath();

  	EndWaitCursor();
	return 0;
}

// CVLaminateAppView ����
#ifdef _DEBUG
void CVLaminateAppView::AssertValid() const
{
	CFormView::AssertValid();
}

void CVLaminateAppView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

/*
CVLaminateAppDoc* CVLaminateAppView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVLaminateAppDoc)));
	return (CVLaminateAppDoc*)m_pDocument;
}
*/

#endif //_DEBUG
// CVLaminateAppView �޽��� ó����
