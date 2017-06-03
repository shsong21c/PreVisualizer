// DlgExecAuto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VLaminateApp.h"
#include "DlgExecAuto.h"
#include "MainFrm.h"
#include "math.h"
#include "mmsystem.h"
#include "NAppDll.h"
#include "NGuiDll.h"
#include "NVisionDll.h"
#include "afxdialogex.h"
#include "NMultiFuncDll.h"

extern CMainFrame *m_pMainFrm;

void    __stdcall NAppDll_CallBackEvent();
NAppDll_CallDATA    NAppDll_Data;

// CDlgExecAuto 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgExecAuto, CDialogEx)

CDlgExecAuto::CDlgExecAuto(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgExecAuto::IDD, pParent)
{
	InitializeCriticalSection(&m_csSync);

	m_bOpenComm = FALSE;
	m_nCntSendDIO = 0;
	m_nCntReceiveDIO = 0;
    m_nErrResponseCnt = 0;
}

CDlgExecAuto::~CDlgExecAuto()
{
	delete m_pDlgBadImage;

	DeleteCriticalSection(&m_csSync);
}

void CDlgExecAuto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_CAM, m_tabCAM);
	DDX_Control(pDX, IDC_TAB_IMAGE_CAM, m_tabImgCAM);	
	DDX_Control(pDX, IDC_TAB_IMAGE, m_tabImage);
	DDX_Control(pDX, IDC_TAB_LIST, m_tabList);
	//DDX_Control(pDX, IDC_ACTEASYIF, m_ActEasyIF);
	DDX_Control(pDX, IDC_LIST_CTRL1, m_listCtrl[0]);
	DDX_Control(pDX, IDC_LIST_CTRL2, m_listCtrl[1]);

}

BEGIN_MESSAGE_MAP(CDlgExecAuto, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CAM, &CDlgExecAuto::OnSelchangeTabCam)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMAGE, &CDlgExecAuto::OnSelchangeTabImage)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMAGE_CAM, &CDlgExecAuto::OnSelchangeTabImageCam)
	ON_BN_CLICKED(IDC_BTN_STOP_RNR, &CDlgExecAuto::OnBnClickedBtnStopRnr)
	ON_BN_CLICKED(IDC_BTN_START_RNR, &CDlgExecAuto::OnBnClickedBtnStartRnr)
	ON_BN_CLICKED(IDC_BTN_TRIGGER_POS, &CDlgExecAuto::OnBnClickedBtnTriggerPos)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgExecAuto::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgExecAuto::OnBnClickedButton2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LIST, &CDlgExecAuto::OnSelchangeTabList)
END_MESSAGE_MAP()

// CDlgExecAuto 메시지 처리기입니다.
BOOL CDlgExecAuto::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_DIALOG_WIDTH, DEF_DIALOG_HEIGHT, SWP_SHOWWINDOW);

	// 버튼, 비전, 카메라, 기타 Gauge R&R 테스트 컨트롤 초기화 
	InitButtonControls();
    InitCameraControls();
	InitEtcControls();
	// 버튼, 비전, 카메라, 기타 Gauge R&R 테스트 컨트롤 초기화 

	UpdateModelName();
	SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);

	// 불량영상 저장 THREAD 초기화, SetEvent로 처리됨 
	m_hEventExport = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadExport = AfxBeginThread(ThreadDataExport, this, THREAD_PRIORITY_LOWEST);
	if (m_pThreadExport)
		m_pThreadExport->m_bAutoDelete = TRUE;
	// 불량영상 저장 THREAD 초기화, SetEvent로 처리됨 

	/*
	// EPC 보정값 전송 THREAD 초기화(PC->PLC), SetEvent로 처리됨 
	m_hEventMelsec = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadMelsec = AfxBeginThread(ThreadMelsecComm, this, THREAD_PRIORITY_LOWEST);
	if (m_pThreadMelsec)
		m_pThreadMelsec->m_bAutoDelete = TRUE;
	// EPC 보정값 전송 THREAD 초기화, SetEvent로 처리됨 

	// 실시간 측정 데이타 전송 THREAD 초기화(PC->PLC), SetEvent로 처리됨 
	m_hEventSendData = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadSendData = AfxBeginThread(ThreadSendDataToPLC, this, THREAD_PRIORITY_BELOW_NORMAL);
	if (m_pThreadSendData)
		m_pThreadSendData->m_bAutoDelete = TRUE;
	*/
	// 실시간 측정 데이타 전송 THREAD 초기화(PC->PLC), SetEvent로 처리됨 

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgExecAuto::InitButtonControls()
{
	CString str;

	// LIST_ID 리스트 DLL초기화 
	//NUiListDll_Open(LIST_MAIN_ID, (long)GetDlgItem(IDC_STATIC_LIST));
	NUiListDll_Open(LIST_INFO_ID, (long)GetDlgItem(IDC_STATIC_INFO));
	// LIST_ID 리스트 DLL초기화 

	// 불량영상 출력 화면 생성 
	m_pDlgBadImage = new CDlgBadImage((CWnd*)GetDlgItem(IDC_STATIC_IMAGE));
	m_pDlgBadImage->Create(IDD_DIALOG_BAD_IMAGE, GetDlgItem(IDC_STATIC_IMAGE));
	m_pDlgBadImage->ShowWindow(SW_SHOW);
	// 불량영상 출력 화면 생성 

	// PLC통신 ETHERNET 초기화 
	/*
	try
	{
     	m_ActEasyIF.put_ActLogicalStationNumber(0);	// Exec set-property method 
		nRet = m_ActEasyIF.Open();	
		m_bOpenComm = TRUE;
	}
	catch (...)
	{
        m_pMainFrm->WriteTextMessage(DEF_MSG_PLC_ERROR);
	}
	*/
	// PLC통신 ETHERNET 초기화 
}

void CDlgExecAuto::InitEtcControls()
{
	int i;

	/*
	m_tabList.DeleteAllItems();
	m_tabList.SetMinTabWidth(100);

	TC_ITEM item;
    item.mask = TCIF_IMAGE|TCIF_TEXT;

	item.iImage = 0;
	item.pszText = _T(" UP CAM ");
	m_tabList.InsertItem(0, &item);

	item.iImage = 1;
	item.pszText = _T(" DN CAM ");
	m_tabList.InsertItem(1, &item);
	m_tabList.SetCurSel(0);
	
	GetDlgItem(IDC_LIST_CTRL1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LIST_CTRL2)->ShowWindow(SW_HIDE);
	*/

	m_pMainFrm->m_OutD[0] = 1;
	for(i=1; i<17; i++)
		m_pMainFrm->m_OutD[i] = 0;

	m_pMainFrm->m_bSendMsg = TRUE;
}

void CDlgExecAuto::InitCameraControls()
{
	long i, nBright;

	BeginWaitCursor();

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		// 비전 출력 화면의 PICTURE BOX를 윈도우에 배치
		GetDlgItem(IDC_STATIC_CAM1+i)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_HIDEWINDOW);
		// 비전 출력 화면의 PICTURE BOX를 윈도우에 배치

		// FRAME의 크기와 영상의 ZOOM으로 DLL를 초기화 한다.
		//NAppDll_SetFrameSize(i, FRAME_WIDTH, FRAME_HEIGHT, 1, 8);
		//NAppDll_SetZoomScale(i, m_pMainFrm->m_Setting.m_fCamScale[i]);
		if (i==CAM_UP) 
	        NAppDll_Open((long)GetDlgItem(IDC_STATIC_CAM1+i), i, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT);
		else
			NAppDll_Open((long)GetDlgItem(IDC_STATIC_CAM1+i), i, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT);

		NAppDll_SetFrameSize(i, FRAME_WIDTH, FRAME_HEIGHT, 1, 8);
		// FRAME의 크기와 영상의 ZOOM으로 DLL를 초기화 한다.

		// 실시간 검사모드가 아닌 상태로 초기화 
		NAppDll_RealTimeMode(i, FALSE);

		// 비전 DLL의 CALLBACK EVENT함수와 데이타 구조를 전달함
		NAppDll_SetEvent(i, NAppDll_CallBackEvent,  (long)&NAppDll_Data);

		// 현재 언어 선택 설정값을 보냄 
		NAppDll_SetTransLanguage(i, m_pMainFrm->m_Setting.m_nSelLanguage);

		// 영상의 메모리 포인터를 보냄
		m_pMainFrm->m_CAM.SetImagePtr(i, (LPBYTE)NAppDll_GetImagePtr(i));

		// S/W 트리거 모드 설정
		m_pMainFrm->m_CAM.SetTriggerMode(i, 1);
		m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
	}

	// 현재 모델명으로 모델 데이타를 읽어온다.
	m_pMainFrm->LoadModelData();

	// 비전 윈도우를 출력함
	ShowVisionWindow(0, TRUE);

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		// 비전 DLL의 결과 데이타를 초기화하고 결과 데이틀 구조체로 읽어온다.
		NAppDll_InitResultData(i);
		m_pMainFrm->m_pRes[i] = (INSP_RESULT_DATA *)NAppDll_GetResultData(i);
		m_pRT[i] = (REALTIME_TEMP *)NAppDll_GetRealTimeData(i);

		// 초기 비전의 ZOOM SCALE를 초기화하고 적용함
		m_pMainFrm->m_Setting.m_fCamScale[i] = DEF_INIT_ZOOM_SCALE;
		NAppDll_SetZoomScale(i, m_pMainFrm->m_Setting.m_fCamScale[i]);

		// Exposure Time를 읽어와 적용함
		nBright = m_pMainFrm->m_Setting.m_nExposureTime[i];//NAppDll_GetBrightnessValue(i, 0);
		m_pMainFrm->m_CAM.SetExposureTime(i, nBright);
	}

	// CAMERA TAB 위치 설정, 방향 PICTURE BOX위치 설정
	GetDlgItem(IDC_TAB_CAM)->SetWindowPos(NULL, 1, FRAME_HEIGHT-10, 350, 40, SWP_SHOWWINDOW);
	GetDlgItem(IDC_STATIC_FORWARD_DIR)->SetWindowPos(NULL, 390, FRAME_HEIGHT+5, FRAME_WIDTH-390, 26, SWP_SHOWWINDOW);
	// CAMERA TAB 위치 설정, 방향 PICTURE BOX위치 설정
	
	// CAMERA TAB 상부 TAB으로 지정 0:상부, 1:하부, 2:상/하부, 3:불량영상
	GetSelectedTab(0);
	// CAMERA TAB 상부 TAB으로 지정 0:상부, 1:하부, 2:상/하부, 3:불량영상
	
	// 검사 결과 리스트 초기화 
	//InitListControls();
	//UpdateResultList();

	InitCountData();

	InitAlignListControls(CAM_UP);
	InitAlignListControls(CAM_DN);
	// 검사 결과 리스트 초기화 

	m_pMainFrm->ExecMessageHandleAll();
	
	EndWaitCursor();
}

void CDlgExecAuto::SetTransLanguage(long nValue)
{
	// 초기 상부 카메라 TAB을 선택
	GetSelectedTab(0);
	// 초기 상부 카메라 TAB을 선택

	// 초기 상부 카메라 TAB을 선택
	GetSelectedSubTab();
	//InitListControls();
}

void CDlgExecAuto::CloseControls()
{
	long i;

	// 해당 THREAD에 EVENT를 보내고 THREAD를 종료한다. 
	SetEvent(m_hEventSendData);
	SetEvent(m_hEventExport);
	// 해당 THREAD에 EVENT를 보내고 THREAD를 종료한다. 

	// PLC의 ETHERNET를 Close합니다. 
	//m_ActEasyIF.Close();
	// PLC의 ETHERNET를 Close합니다. 

	// 비전 DLL를 종료함
	for(i=0; i<CAM_MAX_NUM; i++)
	    NAppDll_Close(i);
	// 비전 DLL를 종료함

	// LIST_ID 리스트 DLL를 종료합니다.
	//NUiListDll_Close(LIST_MAIN_ID);
	NUiListDll_Close(LIST_INFO_ID);
	// LIST_ID 리스트 DLL를 종료합니다.

	Sleep(100);
}

void CDlgExecAuto::ShowVisionWindow(long nCam, BOOL bShow)
{
	EnterCriticalSection(&m_csSync);

	// 각종 CONTROL의 위치를 배치함
	GetDlgItem(IDC_STATIC_CAM1)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_HIDEWINDOW);
	GetDlgItem(IDC_STATIC_CAM2)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_HIDEWINDOW);
	GetDlgItem(IDC_TAB_IMAGE)->SetWindowPos(NULL, 1, FRAME_HEIGHT-35, FRAME_WIDTH, 31, SWP_HIDEWINDOW);
	GetDlgItem(IDC_STATIC_IMAGE)->SetWindowPos(NULL, 1, 32, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, SWP_HIDEWINDOW);
	GetDlgItem(IDC_TAB_IMAGE_CAM)->SetWindowPos(NULL, 1, 8, FRAME_WIDTH, 30, SWP_HIDEWINDOW);
	// 각종 CONTROL의 위치를 배치함

	if (nCam<2)
	{
		// 상부 또는 하부 TAB이 선택될 경우 IDC_STATIC_CAM1의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
		if (bShow)
			GetDlgItem(IDC_STATIC_CAM1+nCam)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_SHOWWINDOW);

		NAppDll_SetFrameSize(nCam, FRAME_WIDTH, FRAME_HEIGHT, 1, 8);
		NAppDll_SetZoomScale(nCam, m_pMainFrm->m_Setting.m_fCamScale[nCam]);
		// 상부 또는 하부 TAB이 선택될 경우 IDC_STATIC_CAM1의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
	}
	else if (nCam==2)
	{
		// 상/하부 TAB이 선택될 경우 두 PICTURE BOX의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
		if (bShow)
		{
			GetDlgItem(IDC_STATIC_CAM1)->SetWindowPos(NULL, DEF_V1_SMALL_LEFT, DEF_V1_SMALL_TOP, DEF_V1_SMALL_WIDTH, DEF_V1_SMALL_HEIGHT, SWP_SHOWWINDOW);
			GetDlgItem(IDC_STATIC_CAM2)->SetWindowPos(NULL, DEF_V2_SMALL_LEFT, DEF_V2_SMALL_TOP, DEF_V2_SMALL_WIDTH, DEF_V2_SMALL_HEIGHT, SWP_SHOWWINDOW);
	    }

		NAppDll_SetFrameSize(CAM_UP, DEF_V1_SMALL_WIDTH, DEF_V1_SMALL_HEIGHT, 1, 8);
		NAppDll_SetFrameSize(CAM_DN, DEF_V2_SMALL_WIDTH, DEF_V2_SMALL_HEIGHT, 1, 8);
		NAppDll_SetZoomScale(CAM_UP, DEF_MINI_ZOOM_SCALE);
		NAppDll_SetZoomScale(CAM_DN, DEF_MINI_ZOOM_SCALE);
		// 상/하부 TAB이 선택될 경우 두 PICTURE BOX의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
	}
	else
	{
		// 불량영상 TAB이 선택될 경우 CONTROL의 위치와 크기를 지정함
		GetDlgItem(IDC_TAB_IMAGE_CAM)->SetWindowPos(NULL, 1, 8, FRAME_WIDTH+2, 30, SWP_SHOWWINDOW);
		GetDlgItem(IDC_TAB_IMAGE)->SetWindowPos(NULL, 1, FRAME_HEIGHT-52, FRAME_WIDTH+2, 35, SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_IMAGE)->SetWindowPos(NULL, 1, 35, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, SWP_SHOWWINDOW);
		// 불량영상 TAB이 선택될 경우 CONTROL의 위치와 크기를 지정함
	}

	LeaveCriticalSection(&m_csSync);
}

long CDlgExecAuto::GetSelectedTab(long nSel)
{
	long i, nLngSel;
    HICON hIcon[5];
	
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	nSel = m_tabCAM.GetCurSel();
	if (nSel<0) nSel=0;

	// 현재 선택된 탭만 LED_ON ICON표시
	for(i=0; i<4; i++)
		hIcon[i] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
	hIcon[nSel] = AfxGetApp()->LoadIcon(IDI_ICON_LED_ON);

	m_imgListSel.DeleteImageList();
	m_imgListSel.Create(16, 16, TRUE, 3, 3);

	for(i=0; i<4; i++)
		m_imgListSel.Add(hIcon[i]);
	// 현재 선택된 탭만 LED_ON ICON표시 

	// 탭의 갯수를 추가하고 명칭과 ICON를 표시함
	m_tabCAM.DeleteAllItems();
	m_tabCAM.SetImageList(&m_imgListSel);
	m_tabCAM.SetMinTabWidth(80);

	TC_ITEM item;
    item.mask = TCIF_IMAGE|TCIF_TEXT;

	item.iImage = 0;
	if (nLngSel==0) item.pszText = _T(" 상부 ");
	else if (nLngSel==1) item.pszText = _T(" UP ");
	else item.pszText = _T(" 上部 ");
	m_tabCAM.InsertItem(0, &item);

	item.iImage = 1;
	if (nLngSel==0) item.pszText = _T(" 하부 ");
	else if (nLngSel==1) item.pszText = _T(" DN ");
	else item.pszText = _T(" 下部 ");
	m_tabCAM.InsertItem(1, &item);

	item.iImage = 2;
	if (nLngSel==0) item.pszText = _T(" 상/하부 ");
	else if (nLngSel==1) item.pszText = _T(" UP/DN ");
	else item.pszText = _T(" 上/下部 ");
	m_tabCAM.InsertItem(2, &item);

	item.iImage = 3;
	if (nLngSel==0) item.pszText = _T(" 불량영상 ");
	else if (nLngSel==1) item.pszText = _T(" NG IMAGE ");
	else item.pszText = _T(" 不良图像 ");
	m_tabCAM.InsertItem(3, &item);
	// 탭의 갯수를 추가하고 명칭과 ICON를 표시함

	// USER가 선택한 TAB버튼을 눌리게 한다. 
	m_tabCAM.SetCurSel(nSel);
	// USER가 선택한 TAB버튼을 눌리게 한다. 

	return nSel;
}

void CDlgExecAuto::OnSelchangeTabCam(NMHDR *pNMHDR, LRESULT *pResult)
{
	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	GetSelectedSubTab();
	*pResult = 0;
}	


void CDlgExecAuto::OnSelchangeTabList(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	long nSel = m_tabList.GetCurSel();

	GetDlgItem(IDC_LIST_CTRL1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_CTRL2)->ShowWindow(SW_HIDE);

	if (nSel<=0) GetDlgItem(IDC_LIST_CTRL1)->ShowWindow(SW_SHOW);
	else GetDlgItem(IDC_LIST_CTRL2)->ShowWindow(SW_SHOW);
	*/

	*pResult = 0;
}


void CDlgExecAuto::GetSelectedSubTab()
{
	long nLngSel, nSel, nCamSel; 
	long i; 

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	nSel = m_tabCAM.GetCurSel();
	if (nSel<0) nSel=0;

	// 실시간 검사중이 아닐 경우 S/W 트리거 모드로 전환 
	if (!m_pMainFrm->m_bExecRealTime)
	{
		m_pMainFrm->m_CAM.SetActivateTrigger(m_pMainFrm->m_nSelectCAM, FALSE);
		Sleep(1);
		m_pMainFrm->m_CAM.SetTriggerMode(nSel, 1);
		Sleep(1);
	}
	// 실시간 검사중이 아닐 경우 S/W 트리거 모드로 전환 

	// 탭 버튼 변경 0:상부, 1:하부, 2:상/하부, 3:불량영상 화면으로 전환
	m_pMainFrm->m_nSelectCAM = nSel;
	GetSelectedTab(nSel);

	ShowVisionWindow(nSel, TRUE);
	// 탭 버튼 변경 0:상부, 1:하부, 2:상/하부, 3:불량영상 화면으로 전환

	// 불량영상 탭 선택시 명칭 표시 
	if (nSel==3)
	{
		m_tabImage.DeleteAllItems();
		m_tabImage.SetMinTabWidth(60);

		TC_ITEM item;
		item.mask = TCIF_IMAGE|TCIF_TEXT;

		for(i=0; i<DEF_QUEUE_MAX_NUM; i++)
		{
			item.iImage = i;
			if (i==0) item.pszText = _T("  1 ");
			if (i==1) item.pszText = _T("  2 ");
			if (i==2) item.pszText = _T("  3 ");
			if (i==3) item.pszText = _T("  4 ");
			if (i==4) item.pszText = _T("  5 ");
			if (i==5) item.pszText = _T("  6 ");
			if (i==6) item.pszText = _T("  7 ");
			if (i==7) item.pszText = _T("  8 ");
			if (i==8) item.pszText = _T("  9 ");
			if (i==9) item.pszText = _T(" 10 ");
			if (i==10) item.pszText = _T(" 11 ");
			if (i==11) item.pszText = _T(" 12 ");
			if (i==12) item.pszText = _T(" 13 ");
			if (i==13) item.pszText = _T(" 14 ");
			if (i==14) item.pszText = _T(" 15 ");
			m_tabImage.InsertItem(i, &item);
		}

		m_tabImgCAM.DeleteAllItems();
		m_tabImgCAM.SetMinTabWidth(80);

		if (nLngSel==0) item.pszText = _T(" 상부 ");
		else if (nLngSel==1) item.pszText = _T(" UP ");
		else item.pszText = _T(" 上部 ");
		m_tabImgCAM.InsertItem(0, &item);

		if (nLngSel==0) item.pszText = _T(" 하부 ");
		else if (nLngSel==1) item.pszText = _T(" DN ");
		else item.pszText = _T(" 下部 ");
		m_tabImgCAM.InsertItem(1, &item);

		// 현재 선택한 불량영상 탭에 대한 불량 영상 출력
		nCamSel = m_tabImgCAM.GetCurSel();
	    nSel = m_tabImage.GetCurSel();
    	m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSel);
		// 현재 선택한 불량영상 탭에 대한 불량 영상 출력
	}
	// 불량영상 탭 선택시 명칭 표시 
}	

void CDlgExecAuto::OnSelchangeTabImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	long nCamSel, nSelPnt;

	// 불량영상 탭을 선택시 선택한 불량 영상을 출력함(최대 15개)
	nCamSel = m_tabImgCAM.GetCurSel();
	nSelPnt = m_tabImage.GetCurSel();

//	SetEvent(m_hEventExport);
	m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);
	// 불량영상 탭을 선택시 선택한 불량 영상을 출력함(최대 15개)

	*pResult = 0;
}

void CDlgExecAuto::OnSelchangeTabImageCam(NMHDR *pNMHDR, LRESULT *pResult)
{
	long nCamSel, nSelPnt;

	// 불량영상 화면에서 CAM탭을 선택시 guswo 불량 영상을 출력함
	nCamSel = m_tabImgCAM.GetCurSel();
	nSelPnt = m_tabImage.GetCurSel();

//	SetEvent(m_hEventExport);
	m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);
	// 불량영상 화면에서 CAM탭을 선택시 guswo 불량 영상을 출력함

	*pResult = 0;
}

void CDlgExecAuto::InitListControls()
{
	/*
	long i, nLngSel, nRow;
	CString str, strT, strUpDn;

	// 현재 선택 언어를 읽어오고 LIST_MAIN_ID 리스트를 RESET CLEAR함
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	NUiListDll_ClearCellAll(LIST_MAIN_ID);
	// 현재 선택 언어를 읽어오고 LIST_MAIN_ID 리스트를 RESET CLEAR함

	// LIST_MAIN_ID 리스트의 구조 초기화 TITLE 표시
	NUiListDll_SetRows(LIST_MAIN_ID, 40);
	NUiListDll_SetRowHeight(LIST_MAIN_ID, 21);
	NUiListDll_SetFontSize(LIST_MAIN_ID, 16);
	NUiListDll_SetCols(LIST_MAIN_ID, 5);
	NUiListDll_SetEnableEdit(LIST_MAIN_ID, FALSE);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 0, 110);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 1, 55);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 2, 55);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 3, 55);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 4, 55);
	
	NUiListDll_SetTitleColor(LIST_MAIN_ID, RGB(200, 200, 200));
	NUiListDll_SetBackColor(LIST_MAIN_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_MAIN_ID, RGB(200, 200, 200));

	NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("Tape Title"));
	NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("Spec.X"));
	NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T("Spec.Y"));
	NUiListDll_SetText(LIST_MAIN_ID, 3, 0, _T("Dx"));
	NUiListDll_SetText(LIST_MAIN_ID, 4, 0, _T("Dy"));

	nRow = 1;
	for(i=0; i<m_pMainFrm->m_nTapeMaxNum; i++)
	{
		str.Format(_T("%s"), m_pMainFrm->m_strTapeTitle[i]);
		if (str.GetLength() > 10) 
			str = strT.Left(10);

		NUiListDll_SetText(LIST_MAIN_ID, 0, nRow, str);

		str.Format(_T("%5.2f"), m_pMainFrm->m_fTapeSpecPx[i]);
		NUiListDll_SetText(LIST_MAIN_ID, 1, nRow, str);

		str.Format(_T("%5.2f"), m_pMainFrm->m_fTapeSpecPy[i]);
		NUiListDll_SetText(LIST_MAIN_ID, 2, nRow, str);

		nRow++;
    }

	// 최종 ROW의 갯수를 설정한다.
	NUiListDll_SetRow(LIST_MAIN_ID, nRow);
	// 최종 ROW의 갯수를 설정한다.

	// LIST_MAIN_ID 리스트를 Invalidate한다.
	NUiListDll_Invalidate(LIST_MAIN_ID);
	// LIST_MAIN_ID 리스트를 Invalidate한다.
	*/
}

void CDlgExecAuto::UpdateResultList()
{
	/*
	CString str;
	long i, nRow;

	nRow = 1;
	for(i=0; i<m_pMainFrm->m_nTapeMaxNum; i++)
	{
		str.Format(_T("%5.2f"), m_pMainFrm->m_fTapeDx[i]);
		NUiListDll_SetText(LIST_MAIN_ID, 3, nRow, str);

		str.Format(_T("%5.2f"), m_pMainFrm->m_fTapeDy[i]);
		NUiListDll_SetText(LIST_MAIN_ID, 4, nRow, str);

		nRow++;
    }

	// 최종 ROW의 갯수를 설정한다.
	NUiListDll_SetRow(LIST_MAIN_ID, nRow);
	// 최종 ROW의 갯수를 설정한다.

	// LIST_MAIN_ID 리스트를 Invalidate한다.
	NUiListDll_Invalidate(LIST_MAIN_ID);
	// LIST_MAIN_ID 리스트를 Invalidate한다.
	*/
}

void CDlgExecAuto::InitAlignListControls(long nCAM)
{
	long i, nNum;
	CString str[GOM_MAX_NUM], strT[GOM_MAX_NUM];

	m_listCtrl[nCAM].SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrl[nCAM].DeleteAllItems();

	for(i = GOM_MAX_NUM-1; i>=0; i--)
        m_listCtrl[nCAM].DeleteColumn(i);

	if (nCAM == CAM_UP)
	{
		str[0] = "Num.(Jig No.)";
		str[1] = "Cam UP DX";
		str[2] = "Cam UP DY";
		str[3] = "Cam UP DT";
	}
	else
	{
		str[0] = "    Num.   ";
		str[1] = "Cam DN DX";
		str[2] = "Cam DN DY";
		str[3] = "Cam DN DT";
	}

	nNum=4;
		
	m_listCtrl[nCAM].InitialHeader(str, nNum, 1);

	UpdateData(false);
}

void CDlgExecAuto::UpdateAlignListControl(long nCAM)
{
	long nNum;
	CString str, strList[GOM_MAX_NUM];

	if (m_pMainFrm->m_Etc.m_nTotalInspCount[nCAM]>1000)	
		m_listCtrl[nCAM].DeleteItem(998);

	if (nCAM==CAM_UP)
		strList[0].Format(_T("%7d(%d)"), m_pMainFrm->m_Etc.m_nTotalInspCount[nCAM], m_pMainFrm->m_nJigNo);
	else
		strList[0].Format(_T("%7d"), m_pMainFrm->m_Etc.m_nTotalInspCount[nCAM]);
	
	strList[1].Format(_T("%5.2f"), m_pMainFrm->m_fCamDX[nCAM]);
	strList[2].Format(_T("%5.2f"), m_pMainFrm->m_fCamDY[nCAM]);
	strList[3].Format(_T("%5.2f"), m_pMainFrm->m_fCamDT[nCAM]);

	nNum = 4;
    m_listCtrl[nCAM].InsertData(strList, nNum);

	/*
	GetDlgItem(IDC_LIST_CTRL1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_CTRL2)->ShowWindow(SW_HIDE);

	if (nCAM==CAM_UP) GetDlgItem(IDC_LIST_CTRL1)->ShowWindow(SW_SHOW);
	else GetDlgItem(IDC_LIST_CTRL2)->ShowWindow(SW_SHOW);

	if (m_pMainFrm->m_nSelectCAM < CAM_MAX_NUM)
		m_tabList.SetCurSel(m_pMainFrm->m_nSelectCAM);
	else
    	m_tabList.SetCurSel(nCAM);
	*/
}

void CDlgExecAuto::InitCountData()
{
	long nLngSel;

	// 현재 선택 언어 
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	// LIST_COUNT_ID 리스트의 구조 초기화 TITLE 표시
	NUiListDll_ClearCellAll(LIST_INFO_ID);
	NUiListDll_SetRows(LIST_INFO_ID, 8);
	NUiListDll_SetRow(LIST_INFO_ID, 8);
	NUiListDll_SetRowHeight(LIST_INFO_ID, 22);
	NUiListDll_SetFontSize(LIST_INFO_ID, 17);
	NUiListDll_SetCols(LIST_INFO_ID, 2);
	NUiListDll_SetEnableTitle(LIST_INFO_ID, FALSE);
	NUiListDll_SetColWidth(LIST_INFO_ID, 0, 125);
	NUiListDll_SetColWidth(LIST_INFO_ID, 1, 204);
	NUiListDll_SetBackColor(LIST_INFO_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_INFO_ID, RGB(200, 200, 200));

	NUiListDll_SetText(LIST_INFO_ID, 0, 0, _T(" CURR. TIME "));
	NUiListDll_SetText(LIST_INFO_ID, 0, 1, _T(" MODEL NAME "));
	NUiListDll_SetText(LIST_INFO_ID, 0, 2, _T(" LOT ID "));
	NUiListDll_SetText(LIST_INFO_ID, 0, 3, _T(" INSP. CNT. "));
	NUiListDll_SetText(LIST_INFO_ID, 0, 4, _T(" NG CNT "));
	NUiListDll_SetText(LIST_INFO_ID, 0, 5, _T(" YIELD "));
	NUiListDll_SetText(LIST_INFO_ID, 0, 6, _T("C: DISK USE"));
	NUiListDll_SetText(LIST_INFO_ID, 0, 7, _T("D: DISK USE"));

	NUiListDll_SetCellBackColor(LIST_INFO_ID, 1, 6, RGB(170, 255, 170));
	NUiListDll_SetCellBackColor(LIST_INFO_ID, 1, 7, RGB(170, 255, 170));

	NUiListDll_Invalidate(LIST_INFO_ID);
	// LIST_COUNT_ID 리스트의 구조 초기화 TITLE 표시

	UpdateCountData(CAM_UP);
}

void CDlgExecAuto::UpdateCountData(long nCAM)
{
	CString str;
	long nGoodCnt;
	float dTempUp, dTempDn, dTempR;
	BOOL fResult;
	unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
	float  fTotalGb, fUseGb;

	dTempUp = 0.0f;
	if (m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_UP] > 0)
	{
		nGoodCnt = m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_UP] - m_pMainFrm->m_Etc.m_nTotalBadCount[CAM_UP];
		dTempUp = (float)nGoodCnt / m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_UP];
	}

	dTempDn = 0.0f;
	if (m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_DN] > 0)
	{
		nGoodCnt = m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_DN] - m_pMainFrm->m_Etc.m_nTotalBadCount[CAM_DN];
		dTempDn = (float)nGoodCnt / m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_DN];
	}

	str.Format(_T(" %d / %d"), m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_UP], m_pMainFrm->m_Etc.m_nTotalInspCount[CAM_DN]);
	NUiListDll_SetText(LIST_INFO_ID, 1, 3, str);

	str.Format(_T(" %d / %d"), m_pMainFrm->m_Etc.m_nTotalBadCount[CAM_UP], m_pMainFrm->m_Etc.m_nTotalBadCount[CAM_DN]);
	NUiListDll_SetText(LIST_INFO_ID, 1, 4, str);

	str.Format(_T(" %5.2f / %5.2f"), dTempUp*100.0f, dTempDn*100.0f);
	NUiListDll_SetText(LIST_INFO_ID, 1, 5, str);

	fResult = GetDiskFreeSpaceEx(_T("C:"),
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);

	fTotalGb = (float)i64TotalBytes / (1024 * 1024 * 1024);
	fUseGb = (float)(i64TotalBytes - i64FreeBytes) / (1024 * 1024 * 1024);
	dTempUp = fUseGb / fTotalGb * 100;
	if (dTempUp<1.0f) 
		dTempUp = 1.0f;

	NUiListDll_SetCellForeColor(LIST_INFO_ID, 1, 6, dTempUp, RGB(255, 170, 170));
	str.Format(_T(" %5.1f%% (%5.1fGB/%5.1fGB)"), fUseGb / fTotalGb * 100, fUseGb, fTotalGb);
	NUiListDll_SetText(LIST_INFO_ID, 1, 6, str);
	
	fResult = GetDiskFreeSpaceEx(_T("D:"),
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);

	fTotalGb = i64TotalBytes / (float)(1024 * 1024 * 1024);
	fUseGb = (i64TotalBytes - i64FreeBytes) / (float)(1024 * 1024 * 1024);
	dTempR = fUseGb / fTotalGb * 100;
	if (dTempR<1.0f) 
		dTempR = 1.0f;

	NUiListDll_SetCellForeColor(LIST_INFO_ID, 1, 7, dTempR, RGB(255, 170, 170));
	str.Format(_T(" %3.1f%% (%5.1fGB/%5.1fGB)"), fUseGb / fTotalGb * 100, fUseGb, fTotalGb);

	NUiListDll_SetText(LIST_INFO_ID, 1, 7, str);
	NUiListDll_Invalidate(LIST_INFO_ID);
}

void CDlgExecAuto::UpdateModelName()
{
	CString str;
	SYSTEMTIME sysNewTime;

	// 현재 시간 설정
	GetLocalTime(&sysNewTime);
	// 현재 시간 설정

	str.Format(_T(" %d:%02d:%02d  %02d:%02d:%02d"), sysNewTime.wYear, sysNewTime.wMonth, sysNewTime.wDay, 
		                                            sysNewTime.wHour, sysNewTime.wMinute, sysNewTime.wSecond);
	NUiListDll_SetText(LIST_INFO_ID, 1, 0, str);

	str.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);
	NUiListDll_SetText(LIST_INFO_ID, 1, 1, str);

//	m_pMainFrm->m_strLotID = _T("");
	str.Format(_T("%s"), _T(""));
	NUiListDll_SetText(LIST_INFO_ID, 1, 2, str);
	NUiListDll_Invalidate(LIST_INFO_ID);
}

void CDlgExecAuto::InitRealTimeProc()
{
	long i;
	char szbuf[100];

	// MainFrm.cpp에서 UMAC통신, PLC통신 상태 모니터링을 실시간중에는 OFF함
	//m_pMainFrm->KillTimer(1000);
	// MainFrm.cpp에서 UMAC통신, PLC통신 상태 모니터링을 실시간중에는 OFF함

	sprintf_s(szbuf, " Start RUN ... ");
	m_pMainFrm->WriteLogToFile(szbuf);

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		// 실사간 검사 모드로 전환하고 결과 데이타를 초기화함
		NAppDll_RealTimeMode(i, TRUE);
		// 실사간 검사 모드로 전환하고 결과 데이타를 초기화함

		// S/W 트리거 모드를 H/W 트리거 모드로 전환  
		if (!m_pMainFrm->m_Setting.m_bSWGrab)
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
			Sleep(30);
			m_pMainFrm->m_CAM.SetTriggerMode(i, 2);
		}
		else
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
			Sleep(30);
			m_pMainFrm->m_CAM.SetTriggerMode(i, 1);
		}
		// S/W 트리거 모드를 H/W 트리거 모드로 전환  
	}

	// 변수 초기화 
	// 실시간 운영 모드 "TRUE"
	m_pMainFrm->m_bGaugeRnR = FALSE;
	m_pMainFrm->m_bExecRealTime = TRUE;
	m_pMainFrm->m_fSendDataCUT = 0.0f;
	m_pMainFrm->m_fOldTEOffset = 0.0f;
	m_pMainFrm->m_fFrontShort = 0.0f;
	m_pMainFrm->m_fRearShort = 0.0f;
	m_pMainFrm->m_nDaqIndex = 0;
	m_pMainFrm->m_nResetCam = 0;
	m_pMainFrm->m_GrabCAM[CAM_UP] = FALSE;
	m_pMainFrm->m_GrabCAM[CAM_DN] = FALSE;
	// 변수 초기화 

	// LED 상태 초기화 
	m_pMainFrm->SetCheckLED(0, FALSE);
	m_pMainFrm->SetCheckLED(1, FALSE);
	m_pMainFrm->SetCheckLED(2, FALSE);
	// LED 상태 초기화 


	m_pMainFrm->m_OutD[0] = 1;
	for(i=1; i<17; i++)
		m_pMainFrm->m_OutD[i] = 0;

	m_pMainFrm->m_bSendMsg = TRUE;
	m_pMainFrm->m_bSendMsgUp = FALSE;
	m_pMainFrm->m_bSendMsgDn = FALSE;
	// 검사 결과 리스트 초기화 
	//InitListControls();
	// 검사 결과 리스트 초기화 

	m_pMainFrm->m_bGaugeRnR = FALSE;

	
 	m_hEventMelsec = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadMelsec = AfxBeginThread(ThreadMelsecComm, this, THREAD_PRIORITY_LOWEST);
	if (m_pThreadMelsec)
		m_pThreadMelsec->m_bAutoDelete = TRUE;
}

void CDlgExecAuto::CloseRealTimeProc()
{
	long i;
	char szbuf[100];

	sprintf_s(szbuf, " Stop RUN ! ");
	m_pMainFrm->WriteLogToFile(szbuf);
	
	m_pMainFrm->m_OutD[0] = 0;
	for(i=1; i<17; i++)
		m_pMainFrm->m_OutD[i] = 0;

	m_pMainFrm->m_bSendMsg = TRUE;

	// 실시간 운영 모드 "FALSE"
	m_pMainFrm->m_bExecRealTime = FALSE;
	
	// H/W트리거 모드를 S/W트리거 모드로 변경  
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		NAppDll_RealTimeMode(i, FALSE);
		if (!m_pMainFrm->m_Setting.m_bSWGrab)
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
			m_pMainFrm->m_CAM.SetTriggerMode(i, 1);
		}
	}
	// H/W트리거 모드를 S/W트리거 모드로 변경  
}

BOOL CDlgExecAuto::InspectProgressGrab(long lParam)
//LONG CDlgAuto::OnInspectProgressGrab(UINT wParam, LONG lParam)
{
	CString str, strT;
	BOOL bDec, bRet1;
	char szbuf[500];
	long i;
	double dTime;
	UINT nCAM = lParam;
	LPBYTE pImage;
	BOOL bRet;

	GetLocalTime(&m_pMainFrm->m_sysNewTime);

	m_pRT[nCAM]->m_bResultDec = FALSE;

	if (m_pMainFrm->m_bExecRealTime)
	{
		//  상/하부 개별 검사 실행
		m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

		NAppDll_ExecuteInspection(nCAM);
		m_pMainFrm->m_pRes[nCAM] = (INSP_RESULT_DATA *)NAppDll_GetResultData(nCAM);
		
		dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);

		if (nCAM==CAM_UP) sprintf_s(g_LOG.m_sLogTitle3, "INSP(CAM=%d T=%5.3f)", 1, dTime);
		if (nCAM==CAM_DN) sprintf_s(g_LOG.m_sLogTitle4, "INSP(CAM=%d T=%5.3f)", 2, dTime);
		//  상/하부 개별 검사 실행

		//  불량 영상 저장 및 저장 THREAD실행 
		pImage = (LPBYTE)NAppDll_GetImagePtr(nCAM);
		bRet = m_pMainFrm->CopyRealTimeImage(nCAM, pImage);     
		if (bRet) 
			SetEvent(m_hEventExport);
		//  불량 영상 저장 및 저장 THREAD실행 

		//bExec = FALSE;
		//if (m_pMainFrm->m_GrabCAM[CAM_UP] && m_pMainFrm->m_GrabCAM[CAM_DN])
		//	bExec = TRUE;
	    
		//if (bExec)
		{
			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

			//  Final Cutter의 위치(A1, A2, A3, A4)를 보정하기 위한 보정값 계산 후 UMAC으로 전송 
			m_pMainFrm->InitAlignFactor(nCAM);
			bRet1 = m_pMainFrm->GetOffsetAlignFactor(nCAM);

			/*
			dAlignX = 0.0f;
			dAlignY = 0.0f;
			dAlignT = 0.0f;
			if (bRet1 && bRet2)
				m_pMainFrm->GetMotionAlignValue(&dAlignX, &dAlignY, &dAlignT);
			*/

			bDec = m_pMainFrm->SendStagePosToEthenet(nCAM);

			m_pMainFrm->m_Etc.m_nTotalInspCount[nCAM]++;
			if (bDec || bRet1)
			{
				m_pRT[nCAM]->m_bResultDec = TRUE;
				//m_pRT[CAM_DN]->m_bResultDec = TRUE;

				m_pMainFrm->m_Etc.m_nTotalBadCount[nCAM]++;
			}
			
			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle5, "PROC1(T=%5.3f)", dTime);

			//  검사결과 데이타 리스트 출력 및 양/불/수율 카운트 UPDATE(MAIN 화면)
			//UpdateResultList();
			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

			UpdateAlignListControl(nCAM);
			UpdateCountData(nCAM);
			//  검사결과 데이타 리스트 출력 및 양/불/수율 카운트 UPDATE(MAIN 화면)

			//  상/하부 검사결과 데이타 PLOTTING UPDATE
			m_pMainFrm->m_pView->m_pDlgPlot->UpdatePlotData(nCAM);
			//  상/하부 검사결과 데이타 PLOTTING UPDATE

			//  검사결과 데이타 리스트 출력 및 검사항목별 불량수량, CP, CPK, 수율 등 UPDATE(RESULT 화면)
			//  최근 500개 데이타를 환형 QUEUE구조를 사용하여 CP, CPK를 계산함 
			if (m_pMainFrm->m_ProdData.m_nTotalCnt<PROD_MAXDATA_NUM)
				m_pMainFrm->m_ProdData.m_nTotalCnt++;

			m_pMainFrm->m_pView->m_pDlgResult->UpdateResultList();
			
			m_pMainFrm->m_ProdData.m_nIndexPnt++;
			if (m_pMainFrm->m_ProdData.m_nIndexPnt>=PROD_MAXDATA_NUM)
				m_pMainFrm->m_ProdData.m_nIndexPnt = 0;
			//  검사결과 데이타 리스트 출력 및 검사항목별 불량수량, CP, CPK, 수율 등 UPDATE(RESULT 화면)


			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle6, "PROC2(T=%5.3f)", dTime);
						

			//  검사결과 데이타를 TEXT파일로 년월일시분 단위로 저장
			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);
			if ((m_pMainFrm->m_nResetCam==1 && nCAM==CAM_DN) ||
				(m_pMainFrm->m_nResetCam==2 && nCAM==CAM_UP))
				m_pMainFrm->WriteStringToFile(nCAM);
			//  검사결과 데이타를 TEXT파일로 년월일시분 단위로 저장


			//  검사 프로세스 시간계산 및 로그 데이타 저장
			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle7, "PROC3(T=%5.3f)", dTime);

			
			GetLocalTime(&m_pMainFrm->m_logEndTime);
			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_TOTAL_INSP);

			sprintf_s(szbuf, ",EllipsedTime,%7.3f,%s,%s,%s,%s,%s,%s,%s", dTime, g_LOG.m_sLogTitle1, g_LOG.m_sLogTitle2, g_LOG.m_sLogTitle3, g_LOG.m_sLogTitle4, 
				                    g_LOG.m_sLogTitle5, g_LOG.m_sLogTitle6, g_LOG.m_sLogTitle7);
			m_pMainFrm->WriteLogToFile((char *)szbuf);
			//  검사 프로세스 시간계산 및 로그 데이타 저장


			//  각종 정보 메세지 표시 형태로 출력
			if (bDec)
			{
    			m_pMainFrm->SetCheckLED(2, TRUE);
				str.Format(_T(" INSP(BAD),  Ellipsed Time:%5.3f Sec. X=%5.2f Y=%5.2f T=%5.2f"), dTime, m_pMainFrm->m_fCamDX[nCAM], m_pMainFrm->m_fCamDY[nCAM], m_pMainFrm->m_fCamDT[nCAM]);
			}
			else
			{
				m_pMainFrm->SetCheckLED(2, FALSE);
				str.Format(_T(" INSP(GOOD), Ellipsed Time:%5.3f Sec. X=%5.2f Y=%5.2f T=%5.2f"), dTime, m_pMainFrm->m_fCamDX[nCAM], m_pMainFrm->m_fCamDY[nCAM], m_pMainFrm->m_fCamDT[nCAM]);
			}

			m_pMainFrm->WriteTextMessage(str);
			//  각종 정보 메세지 표시 형태로 출력
		}
	}
	else
	{
		//  매뉴얼로 검사할 때 처리되는 부분으로 검사후 PLOTTING & 항목별 리스트 출력 
		m_pMainFrm->m_ctrlTimer.SetClockTimer(1);

	    m_pMainFrm->m_pRes[nCAM] = (INSP_RESULT_DATA *)NAppDll_ExecuteInspection(nCAM);
		m_pMainFrm->GetOffsetAlignFactor(nCAM);		

		//  상/하부 검사결과 데이타 PLOTTING UPDATE
		m_pMainFrm->m_pView->m_pDlgPlot->UpdatePlotData(nCAM);
		//  상/하부 검사결과 데이타 PLOTTING UPDATE

		//if (m_pMainFrm->m_Setting.m_bExecDataTrans && nCAM==CAM_UP)
		//    m_pMainFrm->SendTapeOffsetDataToPLC(m_pRes[CAM_UP]);

		//UpdateResultList();
		UpdateAlignListControl(nCAM);
		//  매뉴얼로 검사할 때 처리되는 부분으로 검사후 PLOTTING & 항목별 리스트 출력 

		//  GAUGE R&R 테스트시 사용하는 것으로 검사결과 LOT ID가 TEST로 저장됨  
		if (m_pMainFrm->m_bGaugeRnR)
            m_pMainFrm->WriteStringToFile(nCAM);
	   //  GAUGE R&R 테스트시 사용하는 것으로 검사결과 LOT ID가 TEST로 저장됨  

	   //  시간계산 후 화면에 표시
		dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(1);

		str.Format(_T("%5.3f"), dTime);
		m_pMainFrm->WriteTextMessage(str);
		 //  시간계산 후 화면에 표시
	}

	NAppDll_InvalidateView(nCAM);
	m_pMainFrm->ExecMessageHandleAll();

	return 1;
}

UINT CDlgExecAuto::ThreadDataExport(LPVOID pParam)
{
	CDlgExecAuto *pView = (CDlgExecAuto *)pParam;
  	long nRet=0;
//	long nCamSel, nSelPnt;

	// 불량 영상이 있으면 EVENT가 발생하여 영상을 하드디스크로 저장함 
	// 메인 운영 처리 프로세스와 별개로 CPU 병렬처리가 가능함
	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventExport, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;

        m_pMainFrm->SaveRealTimeData();
		
//		nCamSel = pView->m_tabImgCAM.GetCurSel();
//	    nSelPnt = pView->m_tabImage.GetCurSel();
//    	pView->m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);

		ResetEvent(pView->m_hEventExport);
	}
	// 불량 영상이 있으면 EVENT가 발생하여 영상을 하드디스크로 저장함 
	
	CloseHandle(pView->m_hEventExport);
	AfxEndThread(100);

	return 0;
}

/*
UINT CDlgExecAuto::ThreadSendDataToPLC(LPVOID pParam)
{
	CDlgExecAuto *pView = (CDlgExecAuto *)pParam;
  	long nRet=0;

	// 검사 결과 데이타 전송 EVENT가 발생하면 PC->PLC로 보내줌
	// 메인 운영 처리 프로세스와 별개로 CPU 병렬처리가 가능함
	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventSendData, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;

	    m_pMainFrm->SendDataToPLC();

		ResetEvent(pView->m_hEventSendData);
	}
	// 검사 결과 데이타 전송 EVENT가 발생하면 PC->PLC로 보내줌
	
	CloseHandle(pView->m_hEventSendData);
	AfxEndThread(100);

	return 0;
}
*/

UINT CDlgExecAuto::ThreadMelsecComm(LPVOID pParam)
{
	CDlgExecAuto *pView = (CDlgExecAuto *)pParam;

	while(m_pMainFrm->m_bExecRealTime)
	{
		//m_pMainFrm->ReceiveDataFromEthernet();

		WaitForSingleObject(pView->m_hEventMelsec, 50);
	}
	
	CloseHandle(pView->m_hEventMelsec);
	AfxEndThread(100);

	return 0;
}

// 제어기 최대 밝기는 500으로 고정
void CALLBACK NAppDll_CallBackEvent()
{
	switch(NAppDll_Data.m_bContNo)
	{
		// 비전설정의 조명이 변경되면 CALLBACK EVENT가 발생 REMOTE 조명제어를 위해 4CH씩 한번에 RS232C로 전송 
		// 최대 밝기값은 255LEVEL로 변환  

		// 비전설정의 조명이 변경되면 CALLBACK EVENT가 발생 REMOTE 조명제어를 위해 4CH씩 한번에 RS232C로 전송 
		// 비전설정 화면 종료시 CALLBACK EVENT가 발생하여 실행함
		// 검사항목 리스트를 초기화함
	    case DEF_CONT_EXIT_SETUP :
			//m_pMainFrm->m_pView->m_pDlgExecAuto->InitListControls();
			break;
		// 비전설정 화면 종료시 CALLBACK EVENT가 발생하여 실행함
		case DEF_POSITION_CAL :
			m_pMainFrm->m_pView->m_pDlgSubBar->m_pDlgCal->SetPositionCAL(NAppDll_Data.m_nCamID, NAppDll_Data.m_ptPosCAL.x, NAppDll_Data.m_ptPosCAL.y);

	}	
}

void CDlgExecAuto::SetPositionCAL(BOOL bFlag)
{
	NAppDll_Data.m_bPosCAL = bFlag;

	NAppDll_RealTimeMode(CAM_UP, FALSE);
	NAppDll_RealTimeMode(CAM_DN, FALSE);

	NAppDll_InvalidateView(CAM_UP);
	NAppDll_InvalidateView(CAM_DN);
}

void CDlgExecAuto::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
//		InitListControls();
//		UpdateResultList();
	}
}

static bool m_bReadFlag = false;
void CDlgExecAuto::OnTimer(UINT_PTR nIDEvent)
{

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgExecAuto::OnBnClickedButton1()
{
#if _OMRON
	long nPosD = m_pMainFrm->m_nSelectCAM*6;

	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 2;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

		m_pMainFrm->SetEvent(400, TIMEMSG_CAM_UP_CLEAR+m_pMainFrm->m_nSelectCAM);
	}
#endif
}

void CDlgExecAuto::OnBnClickedButton2()
{
#if _OMRON
	long i, nPosD = m_pMainFrm->m_nSelectCAM*6;

	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
		for(i=0; i<17; i++)
			m_pMainFrm->m_OutD[i] = 0;

		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 3;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

		m_pMainFrm->SetEvent(400, TIMEMSG_CAM_UP_CLEAR+m_pMainFrm->m_nSelectCAM);
	}
#endif
}

void CDlgExecAuto::OnBnClickedBtnTriggerPos()
{
#if _OMRON
	long i, nPosD = m_pMainFrm->m_nSelectCAM*6;

	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
		for(i=0; i<17; i++)
			m_pMainFrm->m_OutD[i] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 4;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

		m_pMainFrm->SetEvent(400, TIMEMSG_CAM_UP_CLEAR+m_pMainFrm->m_nSelectCAM);
	}
#endif
}

static BOOL m_bCompensFlag = FALSE;
void CDlgExecAuto::OnBnClickedBtnStartRnr()
{
	float dX, dY, dT;
	long i, nPosD = m_pMainFrm->m_nSelectCAM*6;

	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
		dX = m_pMainFrm->m_fCamDX[m_pMainFrm->m_nSelectCAM];
		dY = m_pMainFrm->m_fCamDY[m_pMainFrm->m_nSelectCAM];
		dT = m_pMainFrm->m_fCamDT[m_pMainFrm->m_nSelectCAM];

		for(i=0; i<17; i++)
			m_pMainFrm->m_OutD[i] = 0;

		if (dX >= m_pMainFrm->m_Etc.m_fLimitMinusX && dX <= m_pMainFrm->m_Etc.m_fLimitPlusX &&
			dY >= m_pMainFrm->m_Etc.m_fLimitMinusY && dY <= m_pMainFrm->m_Etc.m_fLimitPlusY &&
			dT >= m_pMainFrm->m_Etc.m_fLimitMinusT && dT <= m_pMainFrm->m_Etc.m_fLimitPlusT)
		{
			m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)(dX * 100);
			m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = (DWORD)(dY * 100);
			m_pMainFrm->m_OutD[OMRON_OUT_UP_T_ALIGN+nPosD] = (DWORD)(dT * 100);
			m_pMainFrm->m_OutD[OMRON_OUT_UP_Z_ALIGN+nPosD] = 0;

			m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 5;
			m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		}

		m_pMainFrm->m_bSendMsg = TRUE;
		m_pMainFrm->SetEvent(400, TIMEMSG_CAM_UP_CLEAR+m_pMainFrm->m_nSelectCAM);
		m_bCompensFlag = TRUE;
	}
}

void CDlgExecAuto::OnBnClickedBtnStopRnr()
{
	float dX, dY, dT;
	long i, nPosD = m_pMainFrm->m_nSelectCAM*6;

	if (m_pMainFrm->m_nSelectCAM<CAM_MAX_NUM)
	{
		dX = -1*m_pMainFrm->m_fCamDX[m_pMainFrm->m_nSelectCAM];
		dY = -1*m_pMainFrm->m_fCamDY[m_pMainFrm->m_nSelectCAM];
		dT = -1*m_pMainFrm->m_fCamDT[m_pMainFrm->m_nSelectCAM];

		for(i=0; i<17; i++)
			m_pMainFrm->m_OutD[i] = 0;

		if (dX >= m_pMainFrm->m_Etc.m_fLimitMinusX && dX <= m_pMainFrm->m_Etc.m_fLimitPlusX &&
			dY >= m_pMainFrm->m_Etc.m_fLimitMinusY && dY <= m_pMainFrm->m_Etc.m_fLimitPlusY &&
			dT >= m_pMainFrm->m_Etc.m_fLimitMinusT && dT <= m_pMainFrm->m_Etc.m_fLimitPlusT)
		{
			m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)(dX * 100);
			m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = (DWORD)(dY * 100);
			m_pMainFrm->m_OutD[OMRON_OUT_UP_T_ALIGN+nPosD] = (DWORD)(dT * 100);
			m_pMainFrm->m_OutD[OMRON_OUT_UP_Z_ALIGN+nPosD] = 0;

			m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 5;
			m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		}

		m_pMainFrm->SetEvent(400, TIMEMSG_CAM_UP_CLEAR+m_pMainFrm->m_nSelectCAM);
		m_bCompensFlag = TRUE;
	}
}

