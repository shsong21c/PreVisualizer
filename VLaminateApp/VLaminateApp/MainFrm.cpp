

// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "VLaminateApp.h"
#include "NAppDll.h"
#include "mmsystem.h"
#include "math.h"
#include "NVisionDll.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void    __stdcall EventFuncCallProc(UINT nTimeID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2); 

CMainFrame  *m_pMainFrm;

#define TIMER_DEL_DATA     3
#define INDEX_MODEL_PAIN   0 
#define INDEX_STATUS_PAIN  1
#define INDEX_MESSAGE_PAIN 2 
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//static UINT indicators[] =
//{
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL
//};

// CMainFrame 생성/소멸
CMainFrame::CMainFrame()
{
	CVLaminateApp* pApp = (CVLaminateApp *)AfxGetApp();
	m_hSplash = pApp->GetSplashHwnd();

	::SendMessage(m_hSplash, UM_SPLASH_INIT, ID_LOADING_FILE, 0);
	m_pMainFrm = this;
	
	long i, j, k;

	// 각종 변수 초기화 
	m_strFilePath = "";
	m_strFileType = "";
	m_strLotID = _T("");
	m_nSelectCAM = 0;
	m_nZoomScale = 1;
	m_nAlignNum = CAM_UP;
	m_bAppExec = TRUE;
	m_bGaugeRnR = FALSE;
	m_bExecRealTime = FALSE; 
	m_bShortTest = FALSE;
	m_bCOMM = FALSE;
	m_bCriticalF = FALSE;
	m_bOnData2PLC = FALSE;
	m_nDaqIndex = 0;
	m_nJigNo = 0;
	m_nResetCam = 0;
	m_nCountDataEPC = 0;
	m_fSumDataEPC = 0.0f;
	m_bFirstReadyFlag = FALSE;
	m_bSendMsg = FALSE;
	m_bStatusCOM[0] = FALSE;
	m_bStatusCOM[1] = FALSE;
	// 각종 변수 초기화 

    // SETTING 구조체 변수 초기화 
	_tcscpy_s(m_Setting.m_strModelName, _T("DefaultModel"));
	_tcscpy_s(m_Setting.m_strPassWord, _T(""));
	_tcscpy_s(m_Setting.m_strSelDriver, _T("D:\\"));

	m_Setting.m_nDataPeriod	=	30;
	m_Setting.m_nSaveTypeImage 	=	0;
	m_Setting.m_nPortNo = 1;
	m_Setting.m_nCOM1 = 1;
	m_Setting.m_nCOM2 = 2;
    m_Setting.m_bExecCutOffset = FALSE;
	m_Setting.m_bExecEPCOffset = FALSE;
	m_Setting.m_bExecT2BOffset = FALSE;
	m_Setting.m_bExecDataTrans = FALSE;
	m_Setting.m_bSWGrab = FALSE;
	m_Setting.m_nSkipCnt = 5;
	m_Setting.m_nDaqUnitCnt = 20;
	m_Setting.m_bSWGrab = FALSE;
	m_Setting.m_nDelayTime=100;

	m_Setting.m_fTestVolt = 200.0f;
	m_Setting.m_fLimitCurr = 0.05f;
	m_Setting.m_fStartDelayTime = 0.01f;
	m_Setting.m_fRampingTime = 0.03f;
	m_Setting.m_fHoldingTime = 0.02f;
	m_Setting.m_fContactCurr = 0.0f;

	m_Setting.m_sysOldTime.wDay = 0;
	m_Setting.m_sysOldTime.wHour = 0;

	for(i=0; i<9; i++)
	    m_OutD[i] = 0;

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		m_Setting.m_bExecInsp[i] = TRUE;
		m_Setting.m_bOKImageSave[i] =	FALSE;
		m_Setting.m_bNGImageSave[i] =	FALSE;
		m_Setting.m_bSaveImageType[i] = 0;
		m_Setting.m_nExposureTime[i] = DEF_EXPOSURE_TIME;
		m_GrabCAM[i] = FALSE;
	}

	for(i=0; i<DEF_LIGHT_CHANNEL_NUM; i++)
		m_Setting.m_nLightV[i] = 0;

	for(i=0; i<MODEL_MAX_NUM; i++)
	{
		m_Setting.m_OrderNum[i] = 0;
		_tcscpy_s(m_Setting.m_ModelName[i], _T(""));
	}	
	// SETTING 구조체 변수 초기화 

	// ETC 구조체 변수 초기화 
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		m_Etc.m_nTotalInspCount[i] = 0;
		m_Etc.m_nTotalGoodCount[i] = 0;
		m_Etc.m_nTotalBadCount[i] = 0;
	}
		
	m_Etc.m_nUpBadCount = 0;
	m_Etc.m_nDnBadCount = 0;
	m_Etc.m_nUpFBadCount = 0;	
	m_Etc.m_nUpFBadCount = 0;
	m_Etc.m_nDnRBadCount = 0;	
	m_Etc.m_nDnRBadCount = 0;
	m_Etc.m_nPriorityNum = 0;


	m_Etc.m_fCellWidth = 300;
	m_Etc.m_fCellHeight = 90;
	for(i=0; i<20; i++)
	{
		m_Etc.m_fTapePosX[i] = 0;
	    m_Etc.m_fTapePosY[i] = 0;
	}
	// ETC 구조체 변수 초기화 
	
	// SEARCH 구조체 변수 초기화 
	m_Search.m_bCam1 = FALSE;
	m_Search.m_bCam2 = FALSE;
	m_Search.m_bGood = FALSE;
	m_Search.m_bBad = FALSE;
	m_Search.m_bPeriod = FALSE;
	m_Search.m_bModel = FALSE;
	m_Search.m_bInspItem = FALSE;
    m_Search.m_tmFromYY = 2012;
	m_Search.m_tmFromMM = 1;
	m_Search.m_tmFromDD = 1;
	m_Search.m_tmFromTT = 1;
	m_Search.m_tmFromMT = 1;
	m_Search.m_tmToYY = 2012;
	m_Search.m_tmToMM = 1;
	m_Search.m_tmToDD = 1;
	m_Search.m_tmToTT = 1;
	m_Search.m_tmToMT = 1;
	_tcscpy_s(m_Search.m_strModelName, _T(""));
	// SEARCH 구조체 변수 초기화 

	// 실시간 생산관련 구조체 메모리 할당 및 초기화
	m_ProdData.m_nTotalCnt = 0;
	m_ProdData.m_nIndexPnt = 0;

	for(i=0; i<CAM_MAX_NUM; i++)
	for(j=0; j<GOM_MAX_NUM; j++)
	{
		m_Etc.m_CntROI[i][j] = 0;
		m_Etc.m_CntLink[i][j] = 0;
	
		m_ProdData.m_InspType[i][j] = 0;
		m_ProdData.m_bDec[i][j] = new BOOL[PROD_MAXDATA_NUM];
		m_ProdData.m_fValue[i][j] = new float[PROD_MAXDATA_NUM];
		m_ProdData.m_fValueS[i][j] = new float[PROD_MAXDATA_NUM];
		for(k=0; k<PROD_MAXDATA_NUM; k++)
		{
			m_ProdData.m_bDec[i][j][k] = 0;
		    m_ProdData.m_fValue[i][j][k] = 0;
			m_ProdData.m_fValueS[i][j][k] = 0;
		}
	}
	// 실시간 생산관련 구조체 메모리 할당 및 초기화

	// 영상, 로그, ㄸEXCEL, 모델 등 폴더 생성
	CString strPath = (CString)m_Setting.m_strSelDriver;
	CreateDirectoryFullPath(strPath+(CString)SAVE_IMAGE_PATH);
	CreateDirectoryFullPath((CString)SAVE_DATA_PATH);
	CreateDirectoryFullPath((CString)SAVE_EXCEL_PATH);
	CreateDirectoryFullPath((CString)MODEL_FILE_PATH);
	// 영상, 로그, ㄸEXCEL, 모델 등 폴더 생성
}

CMainFrame::~CMainFrame()
{
	long i, j;

	DeleteCriticalSection(&m_csSync);

	// 실시간 생산관련 구조체 메모리 해제 
	for(i=0; i<CAM_MAX_NUM; i++)
	for(j=0; j<GOM_MAX_NUM; j++)
	{
		delete m_ProdData.m_bDec[i][j];
	    delete m_ProdData.m_fValue[i][j];
		delete m_ProdData.m_fValueS[i][j];
	}	
	// 실시간 생산관련 구조체 메모리 해제 
}

void CMainFrame::ExitProgram()
{
	OnClose();
}

void CMainFrame::OnClose()
{
	int nRet;
	CString strT;

	m_CAM.SetActivateTrigger(m_nSelectCAM, FALSE);
	m_CAM.SetTriggerMode(m_nSelectCAM, 1);

	if (m_bExecRealTime)
	{
		// 실시간 검사중에는 프로그램 종료가 않됨 
		m_pView->SelectDialog(DEF_BUTTON_AUTO);
		return;
		// 실시간 검사중에는 프로그램 종료가 않됨 
	}
	else
	{
		// 프로그램 종료시 메세지 화면 출력 
		if (m_Setting.m_nSelLanguage==0)
			strT = _T(" 프로그램을 종료하시겠습니까 ? ");
		else if (m_Setting.m_nSelLanguage==1)
			strT = _T(" Do you really want to exit program ? ");
		else 
			strT = _T(" 此按钮的功能为，终止系统 ? ");

		nRet = MessageBox(strT, _T(" Message Box "), MB_ICONQUESTION|MB_YESNO);
    	if (IDNO==nRet) 
		{
			m_pView->SelectDialog(DEF_BUTTON_AUTO);
//			m_pView->m_pDlgTestShort->SelectDialog(DEF_BUTTON_SHORT_MAIN);
			return;
		}
		// 프로그램 종료시 메세지 화면 출력
	}

	// 프로그램 종료 FLAG를 "FALSE"로 하고 종료 LOG를 저장함
	m_bAppExec = FALSE;
	m_bShortTest = FALSE;

    char szbuf[100];
	sprintf_s(szbuf, " Terminate program ... ");
	WriteLogToFile(szbuf);
	// 프로그램 종료 FLAG를 "FALSE"로 하고 종료 LOG를 저장함

    // 영상획득 보드를 종료한다.
#if _BASLER_CAM
	long i;
	for(i=0; i<CAM_MAX_NUM; i++)
    	m_CAM.Close(i);
#endif
	// 영상획득 보드를 종료한다.

	// 각종 구조체의 메모리를 해제한다. 
	CloseGlobalData();
	// 각종 구조체의 메모리를 해제한다. 

	// 시스템 설정 파라미터를 저장함
	SaveSysValueToFile();
	// 시스템 설정 파라미터를 저장함

	// 각종 DLL의 모듈을 종료한다.
	//	NUmacDll_Close();
	m_pView->m_pDlgExecAuto->CloseControls();

	NVisionDll_Close();
	// 각종 DLL의 모듈을 종료한다.
	
	m_OmronPlc.ClosePLC();

	CFrameWnd::OnClose();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// SPLASH 윈도우를 실행함 (ID_INITIAL_CONTROL)
	::SendMessage(m_hSplash, UM_SPLASH_INIT, ID_INITIAL_CONTROL, 0);
	// SPLASH 윈도우를 실행함 (ID_INITIAL_CONTROL)

	InitializeCriticalSection(&m_csSync);

	/*
	m_DlgMainBar.Create(this, IDD_DIALOG_MAINBAR, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER, 0);
	if (!m_wndStatusBar.CreateStatusBar(this, indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // Fehler beim Erzeugen
	}
	*/

	// 영상획득 보드 초기화
#if _BASLER_CAM
	m_CAM.FindCamera();
   	m_CAM.Open(0);
   	m_CAM.Open(1);
#endif
	// 영상획득 보드 초기화

	// 시스템 설정 파라미터를 로드함
	LoadSysValueFromFile();
	// 시스템 설정 파라미터를 로드함

	// 각종 구조체의 메모리를 할당한다. 
	InitGlobalData();

	InitAlignFactor(CAM_UP);
	InitAlignFactor(CAM_DN);
	// 각종 구조체의 메모리를 할당한다. 

	// SHORT RS232C 포트 초기화 및 REMOTE LIGHT RS232C포트 초기화 
	//NComPort_Open(m_pMainFrm->m_Setting.m_nCOM1, 19200, 8, 0, 0);
	//NComPort_Open(m_pMainFrm->m_Setting.m_nCOM2, 19200, 8, 0, 0);
	// SHORT RS232C 포트 초기화 및 REMOTE LIGHT RS232C포트 초기화 
	
	// 조명 RS232C포트 초기화 & 초기 조명값 RS232C 통신 적용
	//NComPort_Open(DEF_REMOTE_LIGHT_COMPORT, 19200, 8, 0, 0);
	// 조명 RS232C포트 초기화 & 초기 조명값 RS232C 통신 적용

	m_OmronPlc.OpenPLC();

	NVisionDll_Open(0, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT);

	// SPLASH 윈도우를 실행함 (UM_SPLASH_INIT)
 	::SendMessage(m_pMainFrm->m_hSplash, UM_SPLASH_INIT, ID_INITIAL_FINISH, 0);
	// SPLASH 윈도우를 실행함 (UM_SPLASH_INIT)

	// 현재 시간 설정
	GetLocalTime(&m_sysNewTime);
	GetLocalTime(&m_pMainFrm->m_logStartTime);
	// 현재 시간 설정

	// 프로그램 시작하는 로그를 저장함
    char szbuf[100];
	sprintf_s(szbuf, " Load program ... ");
	WriteLogToFile(szbuf);
	// 프로그램 시작하는 로그를 저장함

	// TIMER 1000:PLC, UMAC모니터링, 2000:0.5초에 한 번씩 비전 통신 TOGGLE신호를 보냄
	SetTimer(1000, 500, NULL);
	SetTimer(2000, 50, NULL);
	// TIMER 1000:PLC, UMAC모니터링, 2000:0.5초에 한 번씩 비전 통신 TOGGLE신호를 보냄
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//  the CREATESTRUCT cs
	cs.x = 0;
	cs.y = 0;
	cs.cx = APP_WIDTH;
	cs.cy = APP_HEIGHT;
//	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	cs.style = WS_VISIBLE|WS_POPUP|WS_BORDER;
	
	CMenu* pMenu = new CMenu;
	pMenu->Attach(cs.hMenu);
	pMenu->DestroyMenu();
	cs.hMenu = NULL;	
	delete pMenu;
	pMenu = NULL;

	// SPLASH 윈도우를 실행함 (ID_INITIAL_FINISH)
 	::SendMessage(m_hSplash, UM_SPLASH_INIT, ID_INITIAL_FINISH, 0);
	// SPLASH 윈도우를 실행함 (ID_INITIAL_FINISH)

	return TRUE;
}

// CMainFrame 진단
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////         STATUS BAR DISPLAY MODULE          /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetCheckLED(long nCAM, BOOL bOnOff)
{
	// 카메라 영상획득 상태 LED
	m_pView->m_pDlgSubBar->SetCheckLED(nCAM, bOnOff);
}

void CMainFrame::WriteTextMessage(CString str)
{
	// 상태 메세지 정보(str)를 표시
	m_pView->m_pDlgSubBar->WriteTextMessage(str);
}

void CMainFrame::WriteTextShortMessage(CString str)
{
	// 상태 메세지 정보(str)를 표시
//	m_pView->m_pDlgTestShort->WriteTextMessage(str);
}
// CMainFrame 메시지 처리기

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////         IMAGE INPUT/OUTPUT MODULE          /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::Grab()
{
	// GRAB 영상을 촬상함 S/W 트리거 사용
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		m_CAM.SetActivateTrigger(m_nSelectCAM, FALSE);
		Sleep(30);
		m_CAM.SetTriggerMode(m_nSelectCAM, 1);
		m_CAM.SetActivateTrigger(m_nSelectCAM, TRUE);
	}
}

// LIVE 영상을 촬상함 
void CMainFrame::Live() 
{
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		m_CAM.SetActivateTrigger(m_nSelectCAM, FALSE);
		Sleep(30);
		m_CAM.SetTriggerMode(m_nSelectCAM, 0);
	}
}

void CMainFrame::LoadImage(CString strFileName)
{
	LPBYTE pImage;
	char szbuf[256];
	CString str;

	// strFileName 의 영상을 로드함 ".JPG", ".BMP" TYPE가능함
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		pImage = (LPBYTE)NAppDll_GetImagePtr(m_nSelectCAM);

		StringToChar(strFileName, szbuf);
		if (m_nSelectCAM == CAM_UP)
			NVisionDll_LoadImageFromFile(szbuf, pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT);
		else
			NVisionDll_LoadImageFromFile(szbuf, pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT);

		NAppDll_InvalidateView(m_nSelectCAM);
	}
	// strFileName 의 영상을 로드함 ".JPG", ".BMP" TYPE가능함
}

void CMainFrame::SaveImage(CString strFileName)
{
	LPBYTE pImage;
	char szbuf[256];

	// 화면에 출력된 영상을 외부장치로 저장함
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		pImage = (LPBYTE)NAppDll_GetImagePtr(m_nSelectCAM);

		StringToChar(strFileName, szbuf);
		if (m_nSelectCAM==CAM_UP)
		{
			m_ctrlTimer.SetClockTimer(1);
          	NVisionDll_SaveImageToFile(szbuf, pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT);
			double dTime = m_ctrlTimer.GetClockTimer(1);
			CString strTemp;
			strTemp.Format(_T("save time : %f"), dTime);
			AfxMessageBox(strTemp);
		}
		else
			NVisionDll_SaveImageToFile(szbuf, pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT);

		NAppDll_InvalidateView(m_nSelectCAM);
	}
	// 화면에 출력된 영상을 외부장치로 저장함
}
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////          IMAGE INPUT/OUTPUT MODULE         /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////        SYSTEM/MODEL DATA SAVE MODULE       /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadModelData()
{
	BOOL bFlag, bRet[CAM_MAX_NUM];
	CString filename;
	long i;
	CFile f;

    bFlag = TRUE;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		filename.Format(_T("%s%s\\Cam%d.dat"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName, i);
		bRet[i] = NAppDll_LoadModelData(i, filename);
		if (!bRet[i]) bFlag = FALSE;
		NAppDll_SetZoomScale(i, m_Setting.m_fCamScale[i]);
	}

	filename.Format(_T("%s%s\\TapeAlign.txt"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName);
	LoadOffsetFile(filename);

	return bFlag;
}

BOOL CMainFrame::SaveModelData()
{
	BOOL bFlag, bRet[CAM_MAX_NUM];
	CString filename, pathName;
	long i;
	CFile f;

	// 모델 저장 루틴
	pathName.Format(_T("%s%s\\"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName);
	CreateDirectoryFullPath(pathName);

    bFlag = TRUE;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		filename.Format(_T("%s%s\\Cam%d.dat"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName, i);
		bRet[i] = NAppDll_SaveModelData(i, filename);
		if (!bRet[i]) bFlag = FALSE;
	}
	// 모델 저장 루틴

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////         OFFSET DATA 처리         ////////////////////////
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadOffsetFile(CString strFileName)
{
	long i, nCnt;
	FILE *fp;
	TCHAR *pCh, szbuf[128];
	CString strT, strL;

	nCnt = 0;
	m_nTapeMaxNum = 0;
	for(i=0; i<20; i++)
	{
	    m_strTapeTitle[i] = _T("");
		m_fTapeSpecPx[i] = 0.0f;
		m_fTapeSpecPy[i] = 0.0f;
	    m_fTapeDx[i] = 0.0f;
		m_fTapeDy[i] = 0.0f;
	}

	wsprintf(szbuf, _T("%s"), strFileName);
	_tfopen_s(&fp, szbuf, _T("r"));
	if (fp)
	{
		nCnt = 0;
		for(i=0; i<20; i++)
		{
			pCh = _fgetts(szbuf, 100, fp);
			if (pCh==NULL) 
				break;

			strL.Format(_T("%s"), szbuf);
			strT = WithoutSpaceToString(strL.Mid(0, 10));
			if (_tcscmp(strT, _T(""))) 
			{
				m_strTapeTitle[nCnt] = strT;

				strT = WithoutSpaceToString(strL.Mid(10, 10));
				m_fTapeW[nCnt] = (float)_ttof(strT);

				strT = WithoutSpaceToString(strL.Mid(20, 10));
				m_fTapeH[nCnt] = (float)_ttof(strT);

				strT = WithoutSpaceToString(strL.Mid(30, 10));
				m_fTapeSpecPx[nCnt] = (float)_ttof(strT);

				strT = WithoutSpaceToString(strL.Mid(40, 10));
				m_fTapeSpecPy[nCnt] = (float)_ttof(strT);

				nCnt++;
			}
		}

		m_nTapeMaxNum = nCnt;
		fclose(fp);
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////         OFFSET DATA 처리         ////////////////////////
/////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::LoadSysValueFromFile()
{
	CFile f;
	CString filename;

	// 시스템 파라미터 읽어옴
	if (f.Open((CString)SETTING_DATA_FILE, CFile::modeRead)) 
	{
		f.Read(&m_Setting, sizeof(SETTING_DATA));
		f.Read(&m_Search, sizeof(SEARCH_DATA));
		f.Close();
	}

//	filename.Format(_T("%s%s\\Etc.dat"), MODEL_FILE_PATH, m_pMainFrm->m_Setting.m_strModelName);
	if (f.Open((CString)ETC_DATA_FILE, CFile::modeRead)) 
	{
		f.Read(&m_pMainFrm->m_Etc, sizeof(ETC_DATA));
		f.Close();
	}

	if (m_Setting.m_nDataPeriod>90) 
		m_Setting.m_nDataPeriod = 90;
	// 시스템 파라미터 읽어옴

	return TRUE;
}

BOOL CMainFrame::SaveSysValueToFile()
{
	CFile f;
	CString pathName, filename;

	// 시스템 파라미터를 저장함
//	pathName.Format(_T("%s%s\\"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName);
	CreateDirectoryFullPath(pathName);

	if (f.Open((CString)SETTING_DATA_FILE, CFile::modeCreate|CFile::modeReadWrite)) 
	{
		f.Write(&m_Setting, sizeof(SETTING_DATA));
		f.Write(&m_Search, sizeof(SEARCH_DATA));
		f.Close();
	}
	else
		m_Setting.m_nOldHour = -1;

//	filename.Format("%s%s\\Etc.dat", MODEL_FILE_PATH, m_pMainFrm->m_Setting.m_strModelName);
	if (f.Open((CString)ETC_DATA_FILE, CFile::modeCreate|CFile::modeReadWrite)) 
	{
		f.Write(&m_pMainFrm->m_Etc, sizeof(ETC_DATA));
		f.Close();
	}
	// 시스템 파라미터를 저장함

	return TRUE;
}

void CMainFrame::SaveRealTimeData()
{
	BOOL bDec;
	long nHead, nTail, nCAM;
	CString strPathName, str, strText;
	LPBYTE pImage;
	char szbuf[256];

	// 실시간 검사시 양/불량 영상저장
	nHead = g_DataRT.m_nImgHeadPnt;
	nTail = g_DataRT.m_nImgTailPnt;

	while(nTail != nHead)
	{
		nCAM = g_DataRT.m_ImgCam[nTail];
		bDec = g_DataRT.m_ImgRes[nTail].m_bDecision;
		pImage = g_pImage[nCAM][nTail];

	    strPathName = "";
		strPathName = m_Setting.m_strSelDriver;
		strPathName += SAVE_IMAGE_PATH;

		if (nCAM == CAM_UP) 
			str.Format(_T("%4d%02d%02d\\%02d\\UP\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay, m_sysNewTime.wHour);
		else
			str.Format(_T("%4d%02d%02d\\%02d\\DN\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay, m_sysNewTime.wHour);
		strPathName += str;
	
		if (bDec) strPathName += _T("NG\\");
		else strPathName += _T("OK\\");

		CreateDirectoryFullPath(strPathName);

		str.Format(_T("%02d%02d_%d.jpg"), m_sysNewTime.wMinute, m_sysNewTime.wSecond, g_DataRT.m_ImgIndexP[nTail]);
	   	strPathName += str;
		StringToChar(strPathName, szbuf);

		if (m_Setting.m_bSaveImageType[nCAM])
		{
			// 양품 영상저장(축소된 원본 영상)
			if (nCAM==CAM_UP)
        	    NVisionDll_SaveImageToFile(szbuf, pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT);
			else
                NVisionDll_SaveImageToFile(szbuf, pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT);
		}
		else
		{
			// 불량 영상저장(축소 화면 캡춰된 영상)
			m_pView->m_pDlgExecAuto->m_pDlgBadImage->SetSaveImageFile(strPathName);
			m_pView->m_pDlgExecAuto->m_pDlgBadImage->UpdateFrameBadImage(nCAM, 0);
		}

		nTail++;
		if (nTail>=DEF_QUEUE_MAX_NUM)
			nTail = 0;
	}

	g_DataRT.m_nImgTailPnt = nTail;
	// 실시간 검사시 양/불량 영상저장
}

BOOL CMainFrame::CopyRealTimeImage(long nCAM, LPBYTE pImage)
{
	BOOL bRet=FALSE;
	long nHead;
	long i, j, x, y, nWidth, nHeight;
	float fRate, fRateW, fRateH;
	LPBYTE pImageBuff;

	// 실시간 검사중 불량 영상을 큐 메모리에 저장함 
	if (nCAM==CAM_UP)
	{
		nWidth = UP_IMAGE_WIDTH;
		nHeight = UP_IMAGE_HEIGHT;
	}
	else
	{
		nWidth = DN_IMAGE_WIDTH;
		nHeight = DN_IMAGE_HEIGHT;
	}

	fRateW = (float)nWidth/(float)SMALL_IMAGE_WIDTH;
	fRateH = (float)nHeight/(float)SMALL_IMAGE_HEIGHT;
	
	if (fRateW>fRateH) fRate = fRateW;
	else fRate = fRateH;
	
	if (((m_Setting.m_bOKImageSave[CAM_UP] || m_Setting.m_bOKImageSave[CAM_DN]) && !m_pRes[nCAM]->m_bDecision) ||
        ((m_Setting.m_bNGImageSave[CAM_UP] || m_Setting.m_bNGImageSave[CAM_DN]) && m_pRes[nCAM]->m_bDecision))
	{
		nHead = g_DataRT.m_nImgHeadPnt;

		g_DataRT.m_ImgCam[nHead] = nCAM;
		g_DataRT.m_nImgIndex++;
		g_DataRT.m_ImgIndexP[nHead] = m_Etc.m_nTotalInspCount[nCAM]+1;//g_DataRT.m_nImgIndex;

		memcpy(&g_DataRT.m_ImgRes[nHead], m_pRes[nCAM], sizeof(INSP_RESULT_DATA));
	    memcpy(g_pImage[nCAM][nHead], pImage, nWidth*nHeight);

		nHead++;
		if (nHead>=DEF_QUEUE_MAX_NUM) 
			nHead = 0;

		g_DataRT.m_nImgHeadPnt = nHead;		
		bRet = TRUE;


		nHead = g_DataRT.m_nBadIndex[nCAM];
		pImageBuff = g_pBadIMG[nCAM][nHead];

		memcpy(&g_DataRT.m_BadRes[nCAM][nHead], (INSP_RESULT_DATA *)m_pRes[nCAM], sizeof(INSP_RESULT_DATA));
		for(i=0; i<SMALL_IMAGE_HEIGHT; i++)
		{
			y = (long)(i*fRate);
			if (y>=nHeight) break;

			for(j=0; j<SMALL_IMAGE_WIDTH; j++)
			{
				x = (long)(j*fRate);
				if (x<nWidth) 
				    pImageBuff[i*SMALL_IMAGE_WIDTH + j] = pImage[y*nWidth + x];
			}
		}

		nHead++;
		if (nHead>=DEF_QUEUE_MAX_NUM)
			nHead=0;

		g_DataRT.m_nBadIndex[nCAM] = nHead;		
		bRet = TRUE;
	}
	// 실시간 검사중 불량 영상을 큐 메모리에 저장함 

	return bRet;
}

/*
void CMainFrame::CopyRealTimeMelsec(INSP_RESULT_DATA *pRes1, INSP_RESULT_DATA *pRes2)
{
	BOOL bRet=FALSE;
	long nHead;

	// 상위에 데이타를 전송할 때 측정 데이타를 큐 메모리에 저장함 
	nHead = g_DataRT.m_nMelHeadPnt;

	memcpy(&g_DataRT.m_MelRes[CAM_UP][nHead], pRes1, sizeof(INSP_RESULT_DATA));
	memcpy(&g_DataRT.m_MelRes[CAM_DN][nHead], pRes2, sizeof(INSP_RESULT_DATA));

	nHead++;
	if (nHead>=DEF_QUEUE_MAX_NUM) 
		nHead = 0;

	g_DataRT.m_nMelHeadPnt = nHead;		
	// 상위에 데이타를 전송할 때 측정 데이타를 큐 메모리에 저장함 
}
*/

void CMainFrame::CopyResultData(INSP_RESULT_DATA *pDest, INSP_RESULT_DATA *pSour)
{
	long i;

	// 실시간 검사 데이타를 복사함 
	pDest->m_bDecision = pSour->m_bDecision;

	pDest->m_bSensorDecF = pSour->m_bSensorDecF;
	pDest->m_dSensorDataF = pSour->m_dSensorDataF;
	pDest->m_dOffsetEPC = pSour->m_dOffsetEPC;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(pDest->m_strTitle[i], 10, pSour->m_strTitle[i]);
		pDest->m_bDecisionROI[i] = pSour->m_bDecisionROI[i];
		pDest->m_bLocROI[i] = pSour->m_bLocROI[i];
		pDest->m_bExecROI[i] = pSour->m_bExecROI[i];
		pDest->m_bEjectROI[i] = pSour->m_bEjectROI[i];
		pDest->m_nInspType[i] = pSour->m_nInspType[i];
		pDest->m_dValue[i] = pSour->m_dValue[i];
		pDest->m_dLowLimit[i] = pSour->m_dLowLimit[i];
		pDest->m_dStdSpec[i] = pSour->m_dStdSpec[i];
		pDest->m_dHighLimit[i] = pSour->m_dHighLimit[i];
	}

	pDest->m_nLinkNum = pSour->m_nLinkNum;
	for(i=0; i<pSour->m_nLinkNum; i++)
	{
		_tcscpy_s(pDest->m_strLinkTitle[i], 10, pSour->m_strLinkTitle[i]);
		pDest->m_bLinkDecROI[i] = pSour->m_bLinkDecROI[i];
		pDest->m_bLinkExecROI[i] = pSour->m_bLinkExecROI[i];
		pDest->m_bLinkEjectROI[i] = pSour->m_bLinkEjectROI[i];
		pDest->m_bLinkLocROI[i] = pSour->m_bLinkLocROI[i];
		pDest->m_nLinkType[i] = pSour->m_nLinkType[i];
		pDest->m_dLinkValue[i] = pSour->m_dLinkValue[i];
		pDest->m_dLinkLowLimit[i] = pSour->m_dLinkLowLimit[i];
		pDest->m_dLinkStdSpec[i] = pSour->m_dLinkStdSpec[i];
		pDest->m_dLinkHighLimit[i] = pSour->m_dLinkHighLimit[i];
	}
	// 실시간 검사 데이타를 복사함 
}

CString CMainFrame::GetModelName(long nOrderNum)
{
	long i;
	CString strModelName;
	
	// 검사ROI의 위치에서 모델명을 리턴함 
	strModelName = "";
	for(i=0; i<MODEL_MAX_NUM; i++)
	{
		if (nOrderNum==m_pMainFrm->m_Setting.m_OrderNum[i])
		{
			strModelName.Format(_T("%s"), (CString)m_Setting.m_ModelName[i]);
			break;
		}
	}
	// 검사ROI의 위치에서 모델명을 리턴함 

	return strModelName;
}

long CMainFrame::GetIndexTitleROI(INSP_RESULT_DATA* pRes, CString strTitle)
{
	long i, nPos;

	// 한 개의 검사항목에 구한하여 결과 구조체에서 strTitle과 부합하는 위치의 인텍스를 리턴함
	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp((TCHAR *)pRes->m_strTitle[i], strTitle) || !pRes->m_bLocROI[i]) continue;

		nPos = i;
		break;
	}
	// 한 개의 검사항목에 구한하여 결과 구조체에서 strTitle과 부합하는 위치의 인텍스를 리턴함

	return nPos;
}

long CMainFrame::GetIndexTitleLinkROI(INSP_RESULT_DATA* pRes, CString strTitle)
{
	long i, nPos;

	// 두 개의 검사항목에 구한하여 결과 구조체에서 strTitle과 부합하는 위치의 인텍스를 리턴함
	nPos = -1;
	for(i=0; i<pRes->m_nLinkNum; i++)
	{
		if (_tcscmp((TCHAR *)pRes->m_strLinkTitle[i], strTitle) || !pRes->m_bLinkLocROI[i]) continue;

		nPos = i;
		break;
	}

	return nPos;
}

#if 0
void CMainFrame::SendResult2UMAC(BOOL bFDec, BOOL bRDec, BOOL bRetCUT, float fCutOS, BOOL bRetEPC, float fOffsetEPC)
{
	//	BOOL bRet;
	long nInt=0, nErrCnt=0;
	double dTime;

	m_ctrlTimer.SetClockTimer(1);

#if _UMAC
	///////////////////////////////////////////////////////////
	//////////   양/불 출력을 UMAC ETHERNET으로 송부    //////////
	///////////////////////////////////////////////////////////
	if (bFDec)
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_FRONT, 1);
	else
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_FRONT, 2);

	if (bRDec)
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_REAR, 1);	
	else
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_REAR, 2);	

	if (bRetCUT==2)
		NUmacDll_SetDoubleValue('P', UMAC_OUT_FINAL_CUT, fCutOS);
	//	if (bRetEPC)
	//		NUmacDll_SetDoubleValue('P', UMAC_OUT_EPC, fOffsetEPC);

	TDioControlDll_BitWrite(PIO_OUT_READY_PORT, TRUE);
#endif

	dTime = m_ctrlTimer.GetClockTimer(1);

	// REJECT OUT(BAD)
	sprintf_s(g_LOG.m_sLogTitle5, "SEND2UMAC(RES=%d F=%d R=%d T=%5.3f)", nInt, bFDec, bRDec, dTime);

	//m_Etc.m_nTotalInspCount++;
	//if (bFDec) m_Etc.m_nTotalBadCount++;

#if _DUO_CELL
	m_Etc.m_nTotalInspCount[CAM_UP]++;
	if (bRDec) m_Etc.m_nTotalBadCount++;
#endif
}

#endif // 0 shsong

void CMainFrame::WriteStringToFile(long nCAM)
{
	CString strModelID, strLotID;

	// GAUGE R&R 테스트시 LOT ID는  "R_R"로 저장됨
	strModelID.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);
	//strLotID = m_pMainFrm->m_strLotID;
	strLotID = _T("");
	if (m_bGaugeRnR)
	{
		strLotID = _T("R_R");
		GetLocalTime(&m_sysNewTime);
	}
	// GAUGE R&R 테스트시 LOT ID는  "TEST"로 저장됨

	EnterCriticalSection(&m_csSync);

	BOOL bDec;
	FILE  *fp;
	TCHAR   szbuf[1200];
	CString STR_ROI[2][GOM_MAX_NUM], STR_LINK[2][GOM_MAX_NUM];
	CString strPathName, strDataPath, strTitle, str, strT;
	
		strDataPath = SAVE_DATA_PATH;
	str.Format(_T("%04d\\%02d\\%02d\\%02d\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay, m_sysNewTime.wHour);
	strDataPath += str;
	_stprintf_s(szbuf, _T("%s%02d.txt"), strDataPath, m_sysNewTime.wMinute);

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	if (m_sysOldTime.wHour!=m_sysNewTime.wHour)
	{
		CreateDirectoryFullPath(strDataPath);
		_tfopen_s(&fp, szbuf, _T("w+"));
	}
	else
	{
		_tfopen_s(&fp, szbuf, _T("a+"));
		if (!fp)
		{
			CreateDirectoryFullPath(strDataPath);
			_tfopen_s(&fp, szbuf, _T("w+"));
		}
	}
		
	bDec = (long)(m_pRes[CAM_UP]->m_bFDecision ||  m_pRes[CAM_DN]->m_bFDecision);
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d %1d %9d"), m_nJigNo, bDec, m_Etc.m_nTotalInspCount[CAM_UP]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("CAM_UP_DX"), 0, bDec, m_fCamDX[CAM_UP]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("CAM_UP_DY"), 0, bDec, m_fCamDY[CAM_UP]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("CAM_UP_DT"), 0, bDec, m_fCamDT[CAM_UP]);
	strT += str;



	str.Format(_T("%10s %1d %1d %7.3f "), _T("CAM_DN_DX"), 0, bDec, m_fCamDX[CAM_DN]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("CAM_DN_DY"), 0, bDec, m_fCamDY[CAM_DN]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("CAM_DN_DT"), 0, bDec, m_fCamDT[CAM_DN]);
	strT += str;



	str.Format(_T("%10s %1d %1d %7.3f "), _T("ALIGN_DX"), 0, bDec, m_fCamDX[CAM_UP] + m_fCamDX[CAM_DN]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("ALIGN_DY"), 0, bDec, m_fCamDY[CAM_UP] + m_fCamDY[CAM_DN]);
	strT += str;

	str.Format(_T("%10s %1d %1d %7.3f "), _T("ALIGN_DT"), 0, bDec, m_fCamDT[CAM_UP] + m_fCamDT[CAM_DN]);
	strT += str;

	if (fp)
	{
		_ftprintf(fp, _T("%s\n"), strT);
		fclose(fp);
	}

	m_sysOldTime = m_sysNewTime;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	LeaveCriticalSection(&m_csSync);
}

//Test Grabber 
void CMainFrame::WriteLogToFile(char *string)
{
	FILE  *fp;
	CString str;
	TCHAR strFile[150];

	//   Enter Critical Section   //
	//EnterCriticalSection(&m_csSync);
	//   Enter Critical Section   //

	// 로그를 저장하는 함수
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

    str.Format(_T("%s%d\\%02d\\%02d"), LOG_DATA_FILE, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	_stprintf_s(strFile, _T("%s\\%02d.txt"), str, lpSystemTime.wHour);

	if (m_Setting.m_nOldHour != lpSystemTime.wHour)
	{
		CreateDirectoryFullPath(str);
		_tfopen_s(&fp, strFile, _T("w+"));
	}
	else
	{
		_tfopen_s(&fp, strFile, _T("a+"));
		if (!fp)
		{
			CreateDirectoryFullPath(str);
			_tfopen_s(&fp, strFile, _T("w+"));
		}
	}

	if (fp)
	{
		str = CharToString(string);

		_ftprintf(fp, _T("[%02d:%02d:%03d] %s\n"), lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, str);
		fclose(fp);
	}

	m_Setting.m_nOldHour = lpSystemTime.wHour;
	// 로그를 저장하는 함수

	//   Leave Critical Section   //
	//LeaveCriticalSection(&m_csSync);
	//   Leave Critical Section   //
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////        SYSTEM/MODEL DATA SAVE MODULE       /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int CMainFrame::ExecMessageHandleAll()
{
	int returnValue;
	MSG Mess;

	// 윈도우의 메세지 큐에 있는 메세지를 전부 실행함
	do {
		returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
		if (returnValue)
		{
			::TranslateMessage(&Mess);
			::DispatchMessage (&Mess);
		}
	} while(returnValue);
	// 윈도우의 메세지 큐에 있는 메세지를 전부 실행함

	return returnValue;
}

int CMainFrame::ExecMessageHandleOne()
{
	int returnValue;
	MSG Mess;

	// 윈도우의 메세지 큐에 있는 메세지를 하나만 실행함
	returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
	if (returnValue)
	{
		::TranslateMessage(&Mess);
		::DispatchMessage (&Mess);
	}
	// 윈도우의 메세지 큐에 있는 메세지를 하나만 실행함
	
	return returnValue;
}

void CMainFrame::DeleteDataPath()
{
	CTime tmStartDay, tmEndDay;
	long nStartDay, nEndDay;
	CString strPathName, str;
	
	tmEndDay = CTime::GetCurrentTime() - CTimeSpan(m_Setting.m_nDataPeriod, 0, 0, 0);
	tmStartDay = tmEndDay - CTimeSpan(100, 0, 0, 0);
	nStartDay = _ttoi(tmStartDay.Format(_T("%Y%m%d")));
	nEndDay = _ttoi(tmEndDay.Format(_T("%Y%m%d")));

	strPathName = "";
	strPathName = m_Setting.m_strSelDriver;
	strPathName += SAVE_IMAGE_PATH;

	while(nStartDay<=nEndDay)
	{
		str.Format(_T("%s%4d%02d%02d\\"), strPathName, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);

		str.Format(_T("%s%4d\\%02d\\%02d\\"), SAVE_DATA_PATH, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);

//		str.Format(_T("%s%4d\\%02d\\%02d\\"), SAVE_SHORTDATA_PATH, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
//		RemoveDir(str);

		str.Format(_T("%s%4d\\%02d\\%02d\\"), LOG_DATA_FILE, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);

		tmStartDay += CTimeSpan(1, 0, 0, 0);
   		nStartDay = _ttoi(tmStartDay.Format(_T("%Y%m%d")));

		if (m_bExecRealTime)
			break;
	}
}

void CMainFrame::InitAlignFactor(long nCamID)
{
	m_fAlignDX = 0;
	m_fAlignDY = 0;
	m_fAlignDT = 0;

//	for(i=0; i<CAM_MAX_NUM; i++)
//	{
		m_fCamDX[nCamID] = 0;
		m_fCamDY[nCamID] = 0;
		m_fCamDT[nCamID] = 0;
//	}
}

BOOL CMainFrame::GetOffsetAlignFactor(long nCamID)
{
	BOOL bRet=FALSE;
	POINT2 ptSourCam, ptInspCam, ptCamLoc;
	INSP_RESULT_DATA *pResCam;
	double dTX, dTY;
	
	pResCam = m_pRes[nCamID];
	if (pResCam->m_bDecision)
		return TRUE;

	ptCamLoc.x = m_Setting.m_fCamX[nCamID];
	ptCamLoc.y = m_Setting.m_fCamY[nCamID];
		
	/*
	ptSourCam.x = ptCamLoc.x + pResCam->m_dSourX;
	ptSourCam.y = ptCamLoc.y - pResCam->m_dSourY;
	ptInspCam.x = ptCamLoc.x + pResCam->m_dCX;
	ptInspCam.y = ptCamLoc.y - pResCam->m_dCY;
	*/
	
	ptSourCam.x = ptCamLoc.x + pResCam->m_dSourX;
	ptSourCam.y = ptCamLoc.y + pResCam->m_dSourY;
	ptInspCam.x = ptCamLoc.x + pResCam->m_dCX;
	ptInspCam.y = ptCamLoc.y + pResCam->m_dCY;


	m_fCamDT[nCamID] = pResCam->m_dSourT - pResCam->m_dCT;
	if (nCamID==CAM_UP)
	{
    	NAppDll_TranslateCoordiPos((double)0.0, (double)0.0, (double)ptSourCam.x, (double)ptSourCam.y, (double)m_fCamDT[nCamID], &dTX, &dTY);

		m_fCamDX[nCamID] = (float)(ptInspCam.x - dTX);
		m_fCamDY[nCamID] = (float)(ptInspCam.y - dTY);
		m_fCamDT[nCamID] *= -1;
	}
	else
	{
		
     	NAppDll_TranslateCoordiPos((double)0.0, (double)0.0, (double)ptInspCam.x, (double)ptInspCam.y, (double)m_fCamDT[nCamID], &dTX, &dTY);

		m_fCamDY[nCamID] = (float)(ptSourCam.x - dTX);
		m_fCamDX[nCamID] = (float)(ptSourCam.y - dTY);
		m_fCamDY[nCamID] *= -1;

	}
	
	m_fCamDX[nCamID] *= -1;
	m_fCamDY[nCamID] *= -1;
	//m_fCamDT[nCamID] *= -1;

	if (nCamID==CAM_UP)
	{
		m_fCamDX[nCamID] += m_Etc.m_fOffsetX[CAM_UP];
		m_fCamDY[nCamID] += m_Etc.m_fOffsetY[CAM_UP];
		m_fCamDT[nCamID] += m_Etc.m_fOffsetT[CAM_UP];
	}

/*
#if _ALIGNER_PC1
	m_fCamDY[nCamID] *= -1;
#endif
*/

	return bRet;
}

#if 0
void CMainFrame::GetMotionAlignValue(float *fX, float *fY, float *fT)
{
	m_fAlignDX = m_fCamDX[CAM_UP] + m_fCamDX[CAM_DN] + m_Etc.m_fOffsetX[CAM_UP];
	m_fAlignDY = m_fCamDY[CAM_UP] + m_fCamDY[CAM_DN] + m_Etc.m_fOffsetY[CAM_UP];
	m_fAlignDT = m_fCamDT[CAM_UP] + m_fCamDT[CAM_DN] + m_Etc.m_fOffsetT[CAM_UP];

	*fX = m_fAlignDX;
	*fY = m_fAlignDY;
	*fT = m_fAlignDT;
}  
#endif // 0


BOOL CMainFrame::SendStagePosToEthenet(long nCamID)
{
	CString strT;
	char szbuf[256];
	BOOL bRet=FALSE;
	int nRetry=0, nRet=0, nPosD;
	float fX, fY, fT;
	double dTime;

	m_ctrlTimer.SetClockTimer(5);

	fX = m_fCamDX[nCamID];
	fY = m_fCamDY[nCamID];
	fT = m_fCamDT[nCamID];

	nPosD = nCamID*6;
	if (m_pRes[nCamID]->m_bDecision || 
		fX < m_Etc.m_fLimitMinusX || fX > m_Etc.m_fLimitPlusX  ||
		fY < m_Etc.m_fLimitMinusY || fY > m_Etc.m_fLimitPlusY ||
	    fT < m_Etc.m_fLimitMinusT || fT > m_Etc.m_fLimitPlusT)
	{
//		WriteDataToMelSecAddr(PLC_OUT_OFFSET_X1, 1);
//		TDioControlDll_BitWrite(PIO_OUT_ERROR_PORT, TRUE);
//		Sleep(200);
//		TDioControlDll_BitWrite(PIO_OUT_ERROR_PORT, FALSE);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_T_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Z_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_ERROR+nPosD] = 1;
		bRet = TRUE;
	}
	else
	{
		m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)(fX * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = (DWORD)(fY * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_T_ALIGN+nPosD] = (DWORD)(fT * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Z_ALIGN+nPosD] = 0;
	}

	m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 1;
	m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 0;

	//nRet = m_OmronPlc.SendMessageToPLC(OMRON_OUT_PC_STATUS, 1, 17, m_OutD);
	//Sleep(10);
	if (nCamID==CAM_UP)
     	m_pMainFrm->m_bSendMsgUp = TRUE;
	else
		m_pMainFrm->m_bSendMsgDn = TRUE;

	timeSetEvent(1000, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_CAM_UP_CLEAR+nCamID, TIME_ONESHOT);
	
	dTime = m_ctrlTimer.GetClockTimer(5);
	sprintf_s(szbuf, "Time=%5.3f, RET=%d, fX=%5.2f, fY=%5.2f, fT=%5.2f", dTime, nRet, fX, fY, fT);
	WriteLogToFile(szbuf);

	strT = CharToString(szbuf);
	WriteTextMessage(strT);

	return bRet;
}

void CMainFrame::SendTapeOffsetDataToPLC(INSP_RESULT_DATA *pResUP)
{
	/*
	BOOL bFlag=FALSE;
	long i, STR[40];
	double dCenX, dCenY;
	double dPx, dPy, dDx, dDy;
	double dAngleT;
//	double dTX, dTY;
	int nRet;

	for(i=0; i<40; i++)
		STR[i] = 0;

	dCenX = pResUP->m_dSourX - m_fTapeW[0]/2;
	dCenY = pResUP->m_dSourY - m_fTapeH[0]/2;

	dDx = pResUP->m_dCX - pResUP->m_dSourX;
	dDy = pResUP->m_dCY - pResUP->m_dSourY;
	dAngleT = pResUP->m_dAngleT - pResUP->m_dSourT;

	for(i=0; i<8; i++)
	{
		dPx = dCenX + m_fTapeSpecPx[i];
		dPy = dCenY + m_fTapeSpecPy[i];

		m_fTapeDx[i] = (float)dDx;
		m_fTapeDy[i] = (float)dDy;

		if (fabs(m_fTapeDx[i]) > DEF_LIMIT_TAPE_RANGE_X ||
			fabs(m_fTapeDy[i]) > DEF_LIMIT_TAPE_RANGE_Y ||
			pResUP->m_bDecision)
		{
			bFlag = TRUE;
			break;
		}

		if (i<m_nTapeMaxNum)
		{
			STR[i*2 + 0] = (long)(m_fTapeDx[i] * 100);
			STR[i*2 + 1] = (long)(m_fTapeDy[i] * 100);
		}
	}

	if (bFlag)
	{
		for(i=0; i<40; i++)
			STR[i] = 0;
	}

	if (m_pMainFrm->m_bExecRealTime)
		nRet = m_pView->m_pDlgExecAuto->m_ActEasyIF.WriteDeviceBlock(PLC_OUT_TAPE_X1, 16, STR);
	*/
}

long CMainFrame::ReadDataFromMelSecAddr(CString strAddr)
{
	long nDataV=0;
	long nRet=0;

	/*
	EnterCriticalSection(&m_csSync);

	nRet = m_pView->m_pDlgExecAuto->m_ActEasyIF.GetDevice(strAddr, &nDataV);
	if (nRet==0)
        m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);
	else
        m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);

	LeaveCriticalSection(&m_csSync);
	*/

	return nDataV;
}

void CMainFrame::WriteDataToMelSecAddr(CString strAddr, long nDataV)
{
	/*
	long nRet;
	EnterCriticalSection(&m_csSync);

	nRet = m_pView->m_pDlgExecAuto->m_ActEasyIF.SetDevice(strAddr, nDataV);
	if (nRet==0)
        m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);
	else
        m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);

	LeaveCriticalSection(&m_csSync);
	*/
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	BOOL nRet=FALSE;
	CString str;
	
	//EnterCriticalSection(&m_csSync);

	if (!m_bAppExec)
	{
		KillTimer(1000);
		KillTimer(2000);
		return;
	}
	
	if (nIDEvent==1000)
	{
		m_bCOMM = !m_bCOMM;
		m_OutD[OMRON_OUT_COMM] = (int)m_bCOMM;
		m_bSendMsg = TRUE;
		m_pView->m_pDlgExecAuto->UpdateModelName();
	}
	
	if (nIDEvent==2000)
	{
#if _OMRON
		if (m_OmronPlc.m_bBusy) return;

		if (m_bSendMsg || m_pMainFrm->m_bSendMsgUp || m_pMainFrm->m_bSendMsgDn)
		{
			if (m_bSendMsg)   m_bSendMsg = FALSE;
			if (m_bSendMsgUp) m_bSendMsgUp = FALSE;
			if (m_bSendMsgDn) m_bSendMsgDn = FALSE;

			
			nRet = m_OmronPlc.SendMessageToPLC(OMRON_OUT_PC_STATUS, 1, 17, m_OutD);
			if (nRet)
				m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);
			else
				m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);
		}
		else
		{
			if (m_bExecRealTime)
	    	    ReceiveDataFromEthernet();
		}
#endif
	}

	//LeaveCriticalSection(&m_csSync);

	CFrameWnd::OnTimer(nIDEvent);
}

static long m_nGrabCnt = 0;
void CMainFrame::ReceiveDataFromEthernet()
{
	BOOL bRet;
	char szCommBuff[256];
	CString str;

	bRet = m_OmronPlc.ReceiveMessageFromPLC(OMRON_IN_PLC_STATUS, 0, szCommBuff);
	if (bRet)// || m_nGrabCnt%5==1)
	{
		if (szCommBuff[OMRON_IN_DN_GRAB]==1 && 
			!m_pMainFrm->m_OutD[OMRON_OUT_DN_GRAB_END])//  || m_nGrabCnt%5==1)
		{
			WriteLogToFile("Grab Signal (CAM_DN)");
			m_pMainFrm->m_OutD[OMRON_OUT_DN_GRAB_END] = 1;
			m_pMainFrm->m_bSendMsgDn = TRUE;
			if (szCommBuff[OMRON_IN_RESET] == 1 && m_pMainFrm->m_nResetCam==0)
				m_pMainFrm->m_nResetCam = 2;

			m_CAM.SetActivateTrigger(CAM_DN, TRUE);
			timeSetEvent(800, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_GRAB_CAM_DN, TIME_ONESHOT);
		}

		if (szCommBuff[OMRON_IN_UP_GRAB]==1 && 
			!m_pMainFrm->m_OutD[OMRON_OUT_UP_GRAB_END])// || m_nGrabCnt%5==4)
		{
			WriteLogToFile("Grab Signal (CAM_UP)");
			m_pMainFrm->m_nJigNo = szCommBuff[OMRON_IN_JIG_NO];
			m_pMainFrm->m_OutD[OMRON_OUT_UP_GRAB_END] = 1;
			m_pMainFrm->m_bSendMsgUp = TRUE;
			if (szCommBuff[OMRON_IN_RESET] == 1 && m_pMainFrm->m_nResetCam == 0)
				m_pMainFrm->m_nResetCam = 1;

			m_CAM.SetActivateTrigger(CAM_UP, TRUE);
			timeSetEvent(800, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_GRAB_CAM_UP, TIME_ONESHOT);
		}

		if (szCommBuff[OMRON_IN_RESET] == 0)
			m_pMainFrm->m_nResetCam = 0;
	}

	m_nGrabCnt++;
	/*
	dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
	
	str.Format(_T("Time = %5.3f   Cnt=%d"), dTime, m_nGrabCnt);
	WriteTextMessage(str);
	*/
}

void CMainFrame::SetEvent(long nTime, DWORD nMsgID)
{
	timeSetEvent(nTime, 1, EventFuncCallProc, (DWORD)(LPVOID)nMsgID, TIME_ONESHOT);

	if (nMsgID==TIMEMSG_WAIT_MOVE_DONE)
	{
    	m_bWaitMoveDone = TRUE;
		while(m_bWaitMoveDone)
		{
			ExecMessageHandleAll();

		}
	}
}

void CALLBACK EventFuncCallProc(UINT nTimeID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	int i;

	switch (dwUser)
	{
		case TIMEMSG_GRAB_CAM_UP :
			m_pMainFrm->WriteLogToFile("Response Trigger (CAM_UP)");
			m_pMainFrm->m_OutD[OMRON_OUT_UP_GRAB_END] = 0;
			m_pMainFrm->m_bSendMsg = TRUE;

			m_pMainFrm->m_CAM.SetActivateTrigger(CAM_UP, FALSE);
			
    		break;
		case TIMEMSG_GRAB_CAM_DN :
			m_pMainFrm->WriteLogToFile("Response Trigger (CAM_DN)");
			m_pMainFrm->m_OutD[OMRON_OUT_DN_GRAB_END] = 0;
			m_pMainFrm->m_bSendMsg = TRUE;

			m_pMainFrm->m_CAM.SetActivateTrigger(CAM_DN, FALSE);
			
			break;
		case TIMEMSG_PIO_INSP_CLEAR :
			break;
		case TIMEMSG_COMM_ERROR :
			break;
        case TIMEMSG_RESET_CLEAR :
			m_pMainFrm->WriteLogToFile("DIO Clear reset (Out RESET 1, 2)");
			break;
	    case TIMEMSG_CAM_UP_CLEAR :
			m_pMainFrm->WriteLogToFile(" Clear Inspection (CAM_UP) ");
			m_pMainFrm->m_OutD[16] = 0;
			for(i=8; i<10; i++)
			    m_pMainFrm->m_OutD[i] = 0;
			m_pMainFrm->m_bSendMsg = TRUE;

			m_pMainFrm->m_GrabCAM[CAM_UP] = FALSE;
			m_pMainFrm->SetCheckLED(CAM_UP, FALSE);

			
			break;

	    case TIMEMSG_CAM_DN_CLEAR :
			m_pMainFrm->WriteLogToFile(" Clear Inspection (CAM_DN) ");
			m_pMainFrm->m_OutD[16] = 0;
			for(i=14; i<16; i++)
			    m_pMainFrm->m_OutD[i] = 0;
			m_pMainFrm->m_bSendMsg = TRUE;

			m_pMainFrm->m_GrabCAM[CAM_DN] = FALSE;
			m_pMainFrm->SetCheckLED(CAM_DN, FALSE);

			
			break;
		case TIMEMSG_WAIT_MOVE_DONE :
			m_pMainFrm->WriteLogToFile(" WaitMoveDone Signal(ALL) ");
			m_pMainFrm->m_OutD[16] = 0;
			for(i=8; i<10; i++)
			    m_pMainFrm->m_OutD[i] = 0;
			for(i=14; i<16; i++)
			    m_pMainFrm->m_OutD[i] = 0;
			m_pMainFrm->m_bSendMsg = TRUE;
			m_pMainFrm->m_bWaitMoveDone = FALSE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//////////                      NPIOCONTROL    FUNCTION                     ////////////
////////////////////////////////////////////////////////////////////////////////////////