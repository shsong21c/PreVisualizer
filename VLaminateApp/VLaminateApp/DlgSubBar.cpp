// DlgSubBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VLaminateApp.h"
#include "DlgSubBar.h"
#include "DlgModelManager.h"
#include "MainFrm.h"
#include "NAppDll.h"
#include "NMultiFuncDll.h"
#include "NVisionDll.h"
#include "DlgPassWord.h"
#include "afxdialogex.h"

extern  CMainFrame *m_pMainFrm;
// CDlgSubBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSubBar, CDialog)

CDlgSubBar::CDlgSubBar(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgSubBar::IDD, pParent)
{
	m_pDlgCal = NULL;
}

CDlgSubBar::~CDlgSubBar()
{
	if (m_pDlgCal) delete m_pDlgCal;
}

void CDlgSubBar::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_MODEL, m_btnModel);
	DDX_Control(pDX, IDC_BTN_VISION, m_btnVision);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BTN_INSP, m_btnInsp);
	DDX_Control(pDX, IDC_CHECK_CAM1, m_checkCam1);
	DDX_Control(pDX, IDC_CHECK_CAM2, m_checkCam2);
	DDX_Control(pDX, IDC_CHECK_OKNG, m_checkOkNg);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BTN_MOTION_CAL, m_btnCal);
	DDX_Control(pDX, IDC_BTN_INSP, m_btnInsp);
	DDX_Control(pDX, IDC_SSCOMM_RUN, m_btnRun);
	DDX_Control(pDX, IDC_SSPANEL_MESSAGE, m_panMessage);
}

BEGIN_MESSAGE_MAP(CDlgSubBar, CBkDialogST)
	ON_BN_CLICKED(IDC_BTN_MODEL, OnBtnModel)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_VISION, OnBtnVision)
	ON_BN_CLICKED(IDC_BTN_INSP, OnBtnInsp)
	ON_COMMAND(IDC_SSCOMM_RUN, &CDlgSubBar::OnClickSscommRun)
	ON_BN_CLICKED(IDC_CHECK_CAM1, &CDlgSubBar::OnBnClickedCheckCam1)
	ON_BN_CLICKED(IDC_BTN_MOTION_CAL, &CDlgSubBar::OnBnClickedBtnMotionCal)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgSubBar, CBkDialogST)
    //{{AFX_EVENTSINK_MAP(CDlgSubBar)
	ON_EVENT(CDlgSubBar, IDC_SSCOMM_RUN, 22 /* Click */, OnClickSscommRun, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

// CDlgSubBar 메시지 처리기입니다.
BOOL CDlgSubBar::OnInitDialog()
{
	CBkDialogST::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_SUBBAR_WIDTH, DEF_SUBBAR_HEIGHT, SWP_SHOWWINDOW);
    SetBitmap(IDB_BITMAP_SUBT);

	InitButtons();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSubBar::OnPostEraseBkgnd(CDC* pDC)
{
	m_btnInsp.SetBk(pDC);
	m_btnVision.SetBk(pDC);
	m_btnReset.SetBk(pDC);
	m_btnModel.SetBk(pDC);
	m_btnCal.SetBk(pDC);
}

void CDlgSubBar::InitButtons()
{
	m_btnInsp.SetIcon(IDI_ICON_INSP);
	m_btnInsp.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnInsp.DrawBorder(TRUE);
	m_btnInsp.DrawTransparent(TRUE);

	m_btnVision.SetIcon(IDI_ICON_VISION);
	m_btnVision.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnVision.DrawBorder(TRUE);
	m_btnVision.DrawTransparent(TRUE);

	m_btnCal.SetIcon(IDI_ICON_MOTION_CAL);
	m_btnCal.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnCal.DrawBorder(TRUE);
	m_btnCal.DrawTransparent(TRUE);

	m_btnReset.SetIcon(IDI_ICON_RESET);
	m_btnReset.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnReset.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnReset.DrawBorder(TRUE);
	m_btnReset.DrawTransparent(TRUE);

	m_btnModel.SetIcon(IDI_ICON_MODEL);
	m_btnModel.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnModel.DrawBorder(TRUE);
	m_btnModel.DrawTransparent(TRUE);

	m_checkCam1.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_checkCam1.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_checkCam1.DrawBorder(FALSE);
	m_checkCam1.DrawTransparent();

	m_checkCam2.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_checkCam2.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_checkCam2.DrawBorder(FALSE);
	m_checkCam2.DrawTransparent();

	m_checkOkNg.SetIcon(IDI_ICON_LED_ON2, IDI_ICON_LED_ON);
	m_checkOkNg.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_checkOkNg.DrawBorder(FALSE);
	m_checkOkNg.DrawTransparent();

	SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);
}

void CDlgSubBar::SetTransLanguage(long nValue)
{
	m_btnRun.SetCaption(_T(" STOP "));

	if (nValue==0)
	{
		m_btnVision.SetWindowText(_T("비전"));
		m_btnReset.SetWindowText(_T("초기화"));
		m_btnModel.SetWindowText(_T("모델"));
		m_btnInsp.SetWindowText(_T("검사"));
//		m_btnRun.SetCaption(_T(" 정지 "));

		m_btnVision.SetTooltipText(_T("이 버튼을 실행하면 비전설정 화면이 스크린에 출력됩니다."));
		m_btnReset.SetTooltipText(_T("이 버튼을 실행하면 수량이 초기화됩니다."));
		m_btnModel.SetTooltipText(_T("이 버튼을 실행하면 모델 관리 화면이 출력됩니다."));
		m_btnInsp.SetTooltipText(_T("화면에 출력된 영상으로 매뉴얼 검사를 실행합니다."));
	}
	if (nValue==1)
	{
		m_btnVision.SetWindowText(_T("VISION"));
		m_btnReset.SetWindowText(_T("INIT."));
		m_btnModel.SetWindowText(_T("MODEL"));
		m_btnInsp.SetWindowText(_T("INSP."));

		m_btnModel.SetTooltipText(_T(" When you click this button, the model management is displayed on the screen. "));
		m_btnReset.SetTooltipText(_T(" Count data Initialize"));
		m_btnVision.SetTooltipText(_T(" When you click this button, the vision setup window is displayed on the screen. "));
		m_btnInsp.SetTooltipText(_T(" The image on the screen, run a manual inspection. "));
	}
	if (nValue==2)
	{
		m_btnVision.SetWindowText(_T("怜像"));
		m_btnReset.SetWindowText(_T("初始化"));
		m_btnModel.SetWindowText(_T("模特"));
		m_btnInsp.SetWindowText(_T("检查"));
//		m_btnRun.SetCaption(_T(" 停止 "));

		m_btnModel.SetTooltipText(_T(" 实行此按钮，可显示MODEL管理画面 "));
		m_btnReset.SetTooltipText(_T(" 数量进行初期化吗"));
		m_btnVision.SetTooltipText(_T(" "));
		m_btnInsp.SetTooltipText(_T(" 把输出在画面的影像实行手动检查 "));
	}

//	m_btnRun.SetCaption(_T(" STOP "));
}

void CDlgSubBar::OnBtnReset()
{
	long i;
	CString strT;

	if (m_pMainFrm->m_bExecRealTime) return;

	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
		strT = _T(" 검사 수량을 초기화하시겠습니까 ? ");
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
		strT = _T(" Do you want to reset count ? ");
	else
		strT = _T(" 数量进行初期化吗？ ");

	if (MessageBox(strT, _T(" MESSAGE BOX "), MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		for(i=0; i<CAM_MAX_NUM; i++)
		{
			m_pMainFrm->m_Etc.m_nTotalInspCount[i] = 0;
			m_pMainFrm->m_Etc.m_nTotalBadCount[i] = 0;
		}

		m_pMainFrm->m_pView->m_pDlgExecAuto->InitCountData();
		m_pMainFrm->m_pView->m_pDlgExecAuto->UpdateCountData(CAM_UP);
		m_pMainFrm->m_pView->m_pDlgExecAuto->UpdateCountData(CAM_DN);

		m_pMainFrm->m_pView->m_pDlgResult->InitListControls();
		m_pMainFrm->m_pView->m_pDlgResult->ResetProductData();
		//m_pMainFrm->m_pView->m_pDlgPlot->ResetPlotData();
	}
}

void CDlgSubBar::OnBtnModel() 
{
	CDlgModelManager dlg;
	CString str;

	if (m_pMainFrm->m_bExecRealTime) return;
	str.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);

	dlg.DoModal();
	m_pMainFrm->m_pView->m_pDlgExecAuto->UpdateModelName();

	if (_tcscmp(str, m_pMainFrm->m_Setting.m_strModelName))
	{
		//m_pMainFrm->m_pView->m_pDlgExecAuto->InitListControls();
		m_pMainFrm->m_pView->m_pDlgResult->InitListControls();
		m_pMainFrm->m_pView->m_pDlgResult->ResetProductData();
		//m_pMainFrm->m_pView->m_pDlgPlot->ResetPlotData();
	}		
}

void CDlgSubBar::OnBtnVision() 
{
	CDlgPassWord dlg;
	CString filename;

	if (m_pMainFrm->m_bExecRealTime) return;
	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
//		if (dlg.DoModal()==IDOK) 
		{
			filename.Format(_T("%s%s\\Cam%d.dat"), MODEL_FILE_PATH, m_pMainFrm->m_Setting.m_strModelName, m_pMainFrm->m_nSelectCAM);
		    NAppDll_SetFileName(m_pMainFrm->m_nSelectCAM, filename);

    		NAppDll_ActiveSetupDialog(m_pMainFrm->m_nSelectCAM);
		}
	}
}

void CDlgSubBar::OnBtnInsp()
{
	CString strName = _T("");

	for(int filecou = 0; filecou < m_pMainFrm->m_lLoadTotal; filecou++)
	{		
		if (m_pMainFrm->m_bExecRealTime) return;

		if (m_pMainFrm->m_nSelectCAM<2)
		{
			strName = m_pMainFrm->m_pstrLoadFileList[m_pMainFrm->m_nSelectCAM][filecou];
	   		m_pMainFrm->LoadImage(strName);

			NAppDll_RealTimeMode(m_pMainFrm->m_nSelectCAM, TRUE);

			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);
			m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(m_pMainFrm->m_nSelectCAM);
			double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
		}
		else
		{
			for(int camcou = 0; camcou < CAM_MAX_NUM; camcou++) 
			{
				strName = m_pMainFrm->m_pstrLoadFileList[camcou][filecou];
	   			m_pMainFrm->LoadImage(strName);

				NAppDll_RealTimeMode(camcou, TRUE);

				m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(camcou);

				NAppDll_RealTimeMode(camcou, FALSE);
			}								
		}
		double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);

		m_pMainFrm->m_pView->m_pDlgResult->UpdateResultList();

		

	//	str.Format(_T(" INSP(BAD),  Final Cut(%d):%3.2f, Ellipsed Time:%5.3f Sec. "), 0, 0., dTime);
	//    m_pMainFrm->WriteTextMessage(str);
		m_pMainFrm->ExecMessageHandleAll();		
		
		Sleep(500);
	}

#if 0
	CString str;

	/*
	static bool bExit = FALSE;
	long nIndex;
	CFileFind finder;
	CString strName = _T("");
	CString strT, strTime, strPath;

	bExit = !bExit;
	strPath = m_pMainFrm->m_strFilePath + "*." + m_pMainFrm->m_strFileType; 
	if (m_pMainFrm->m_strFileType.GetLength() != 3) return;

	finder.FindFile(strPath);
	while(finder.FindNextFile() && bExit)
	{
	strT = finder.GetFileName();
	if (finder.IsDots()) continue;
	strName = m_pMainFrm->m_strFilePath + strT;
	m_pMainFrm->LoadImage(strName);

	if (m_pMainFrm->m_bExecRealTime) return;

	NAppDll_RealTimeMode(m_pMainFrm->m_nSelectCAM, TRUE);
	m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

	if (m_pMainFrm->m_nSelectCAM<2)
	{
	m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(m_pMainFrm->m_nSelectCAM);
	}
	else
	{
	m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_UP);
	m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_DN);
	m_pMainFrm->m_pView->m_pDlgResult->UpdateResultList();
	}

	dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);

	//	str.Format(_T(" INSP(BAD),  Final Cut(%d):%3.2f, Ellipsed Time:%5.3f Sec. "), 0, 0., dTime);
	//    m_pMainFrm->WriteTextMessage(str);
	m_pMainFrm->ExecMessageHandleAll();

	NAppDll_RealTimeMode(m_pMainFrm->m_nSelectCAM, FALSE);
	Sleep(300);
	}
	*/

	/*
	long STR[6];
	long nRet;
	char szbuf[256];

	for(long i=0; i<6; i++)
	STR[i] = 0;

	nRet = m_pMainFrm->m_pView->m_pDlgExecAuto->m_ActEasyIF.WriteDeviceBlock(PLC_OUT_OFFSET_X1, 6, STR);
	sprintf_s(szbuf, " RET=%d ",  nRet);
	m_pMainFrm->WriteLogToFile(szbuf);
	*/

	if (m_pMainFrm->m_bExecRealTime)
	{
		m_pMainFrm->SendStagePosToEthenet(m_pMainFrm->m_nSelectCAM);
		return;
	}

	m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_TOTAL_INSP);
	NAppDll_RealTimeMode(m_pMainFrm->m_nSelectCAM, 2);

	m_pMainFrm->InitAlignFactor(m_pMainFrm->m_nSelectCAM);
	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
		m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(m_pMainFrm->m_nSelectCAM);
	}
	else
	{
		m_pMainFrm->m_GrabCAM[CAM_UP] = TRUE;
		m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_UP);

		m_pMainFrm->m_GrabCAM[CAM_DN] = TRUE;
		m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_DN);
		//m_pMainFrm->m_pView->m_pDlgResult->UpdateResultList();

		m_pMainFrm->m_GrabCAM[CAM_UP] = FALSE;
		m_pMainFrm->m_GrabCAM[CAM_DN] = FALSE;
	}

	//	str.Format(_T(" INSP(BAD),  Final Cut(%d):%3.2f, Ellipsed Time:%5.3f Sec. "), 0, 0., dTime);
	//  m_pMainFrm->WriteTextMessage(str);

	m_pMainFrm->ExecMessageHandleAll();
	NAppDll_RealTimeMode(m_pMainFrm->m_nSelectCAM, FALSE);  
#endif // 0

}

void CDlgSubBar::SetCheckLED(long nCAM, BOOL bOnOff)
{
	if (nCAM==0)
	    m_checkCam1.SetCheck(bOnOff);
	if (nCAM==1)
	    m_checkCam2.SetCheck(bOnOff);
	if (nCAM==2)
	    m_checkOkNg.SetCheck(bOnOff);
}

void CDlgSubBar::WriteTextMessage(CString str)
{
	m_panMessage.SetWindowText(str);
}

void CDlgSubBar::OnClickSscommRun()
{
	CString strT;

	BeginWaitCursor();
	
	if (!m_pMainFrm->m_bExecRealTime)
	{
//		m_pMainFrm->m_pView->m_pDlgTestShort->KillTimer(2000);
		strT = _T(" RUN ");

		m_btnRun.SetPictureAnimationEnabled(TRUE);
		m_btnRun.SetBackColor(RGB(255,111,111));
		m_btnRun.SetCaption(strT);

	    m_pMainFrm->m_pView->m_pDlgPlot->InitPlotData();
		m_pMainFrm->m_pView->m_pDlgExecAuto->InitRealTimeProc();	
		m_pMainFrm->WriteTextMessage(DEF_MSG_READY);
	}
	else
	{
		strT = _T(" STOP ");

		m_pMainFrm->m_pView->m_pDlgExecAuto->CloseRealTimeProc();
		m_btnRun.SetPictureAnimationEnabled(FALSE);
		m_btnRun.SetBackColor(RGB(110,160,230));
		m_btnRun.SetCaption(strT);

		// 파일삭제 
		//m_pMainFrm->DeleteDataPath();
    	m_pMainFrm->WriteTextMessage(DEF_MSG_READY);
	}

  EndWaitCursor();
}

void CDlgSubBar::OnBnClickedCheckCam1()
{
	// TODO: Add your control notification handler code here
}


void CDlgSubBar::OnBnClickedBtnMotionCal()
{
	if (m_pMainFrm->m_bExecRealTime) return;

	CDlgPassWord dlg;
	CDlgCal pDlgCal;

//	if (dlg.DoModal()==IDOK) 
	{
		if (m_pDlgCal != NULL) 
		{
			if (m_pDlgCal->IsWindowVisible())
				return;
			else
			{
				delete m_pDlgCal;
				m_pDlgCal = NULL;
			}
		}

		m_pDlgCal = new CDlgCal(this);
		m_pDlgCal->Create(IDD_DIALOG_CAL, NULL);
		m_pDlgCal->ShowWindow(SW_SHOW);
	}
}
