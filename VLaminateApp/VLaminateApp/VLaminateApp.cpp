
// NLaminateApp.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "VLaminateApp.h"
#include "MainFrm.h"
#include "NGuiDll.h"
#include "NAppDll.h"

#include "VLaminateAppDoc.h"
#include "VLaminateAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CMainFrame *m_pMainFrm;

// CVLaminateApp

BEGIN_MESSAGE_MAP(CVLaminateApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CVLaminateApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CVLaminateApp ����

CVLaminateApp::CVLaminateApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("VLaminateApp.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CVLaminateApp ��ü�Դϴ�.

CVLaminateApp theApp;


// CVLaminateApp �ʱ�ȭ
LPCTSTR lpszMutexName = TEXT("VLaminateAppMutex");
LPCTSTR lpszMsgBoxHeader = TEXT("VLaminateApp");

BOOL CVLaminateApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		MessageBox(NULL, DEF_SOCKET_INIT_FAIL, TEXT(" Error Message Box "), MB_ICONERROR);
		return FALSE;
	}

	if (!FirstInstance())
	{
		MessageBox(NULL, TEXT("  VLaminateApp is already activated ! "), lpszMsgBoxHeader, MB_OK | MB_ICONEXCLAMATION);

		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	if (m_pDlgSplash) {
		delete m_pDlgSplash;
		m_pDlgSplash = NULL;
	}

	m_pDlgSplash = new CDlgSplash();
	m_pDlgSplash->Create(CDlgSplash::IDD);
	m_pDlgSplash->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pDlgSplash->ShowWindow(SW_SHOW);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVLaminateAppDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CVLaminateAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CString str = (CString)MAIN_WINDOW_TITLE + "[MAIN SCREEN]";

	m_pMainWnd->SetWindowText(str);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}

BOOL CVLaminateApp::FirstInstance()
{
    m_hMutex = CreateMutex(NULL, FALSE, lpszMutexName);
    
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        // Function returns a handle to the existing object,
        // that means, other application instance already running.
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int CVLaminateApp::ExitInstance()
{
	if (m_pDlgSplash)
	{
		m_pDlgSplash->DestroyWindow();
		delete m_pDlgSplash;
		m_pDlgSplash = NULL;
	}

	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

HWND CVLaminateApp::GetSplashHwnd()
{
	if (m_pDlgSplash && ::IsWindow(m_pDlgSplash->m_hWnd))
		return m_pDlgSplash->m_hWnd;

	return NULL;
}

void CVLaminateApp::SetInitFinished()
{
	if (m_pDlgSplash)
	{
		ExecMessageHandleAll();
		Sleep(1000);
		m_pDlgSplash->DestroyWindow();
		delete m_pDlgSplash;
		m_pDlgSplash = NULL;
	}

	EndWaitCursor();
}

int CVLaminateApp::ExecMessageHandleAll()
{
	int returnValue;
	MSG Mess;

	do {
		returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
		if (returnValue)
		{
			::TranslateMessage(&Mess);
			::DispatchMessage (&Mess);
		}
	} while(returnValue);
	
	return returnValue;
}
// CVLaminateApp �޽��� ó����

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CVLaminateApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVLaminateApp �޽��� ó����
BOOL CVLaminateApp::PreTranslateMessage(MSG* pMsg)
{
	BOOL bCtrl;

    if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			pMsg->wParam = VK_TAB;	
			NUiListDll_UpdateEditControl(LIST_SETUP_ID, TRUE);
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			pMsg->wParam = VK_TAB;	
			NUiListDll_UpdateEditControl(LIST_SETUP_ID, FALSE);
		}

		if (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT ||
			pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			bCtrl = (GetKeyState(VK_CONTROL) & 0x80000);
			NAppDll_SetPressKey(m_pMainFrm->m_nSelectCAM, (long)bCtrl, pMsg->wParam);			
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

