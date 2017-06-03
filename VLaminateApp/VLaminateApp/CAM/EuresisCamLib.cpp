// EuresisCamLib.cpp : implementation file
//

#include "stdafx.h"
#include "EuresisCamLib.h"
#include "..\NAppDll.h"
#include "..\NMultiFuncDll.h"
#include "..\MainFrm.h"   

//#define  _CAM25M             1
#define  _CAM12M             1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern  CMainFrame *m_pMainFrm;

// 카메라#1, #2 영상획득 완료시 CALLBACK EVENT 함수 선언
void    WINAPI     GlobalCallback1(PMCSIGNALINFO SigInfo);
void    WINAPI     GlobalCallback2(PMCSIGNALINFO SigInfo);
// 카메라#1, #2 영상획득 완료시 CALLBACK EVENT 함수 선언

/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib
CEuresisCamLib::CEuresisCamLib()
{
	// Euresis 영상획득 보드 초기화 및 메모리 할당/포인터 초기화
#if _EURESIS_CAM
	m_nStatus = McOpenDriver(NULL);

	InitBoard1();
    InitBoard2();

	m_pImageUP = new unsigned char[UP_IMAGE_WIDTH*UP_IMAGE_HEIGHT];
	m_pImageDN = new unsigned char[DN_IMAGE_WIDTH*DN_IMAGE_HEIGHT];

	long i;
	for(i=0; i<CAM_MAX_NUM; i++)
	    m_pImage[i] = NULL;
#endif
	// Euresis 영상획득 보드 초기화 및 메모리 할당/포인터 초기화
}

void CEuresisCamLib::InitBoard1()
{
	// Euresis 영상획득 보드 카메라 #1 파라미터 설정
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM5M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "A");
	
	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

    // Choose the camera expose duration
    McSetParamInt(m_Channel_1, MC_Expose_us, 300);
   	McSetParamInt(m_Channel_1, MC_TrigLine, MC_TrigLine_IIN1);

    // Choose the number of images to acquire
    McSetParamInt(m_Channel_1, MC_SeqLength_Fr, MC_INDETERMINATE);

    // Retrieve image dimensions
    McGetParamInt(m_Channel_1, MC_ImageSizeX, &m_SizeX);
    McGetParamInt(m_Channel_1, MC_ImageSizeY, &m_SizeY);
    McGetParamInt(m_Channel_1, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt(m_Channel_1, MC_SurfaceCount, 3);

    // Enable MultiCam signals
    McSetParamInt(m_Channel_1, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel_1, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // 카메라 #1의 영상획득 완료시 GlobalCallback1 함수 실행
    McRegisterCallback(m_Channel_1, GlobalCallback1, this);
	// Euresis 영상획득 보드 카메라 #1 파라미터 설정
}

void CEuresisCamLib::InitBoard2()
{
	// Euresis 영상획득 보드 카메라 #2 파라미터 설정
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology_MONO_DECA);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM5M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_2, MC_Connector, "B");
	// Choose the video standard
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

    // Choose the camera expose duration
    McSetParamInt(m_Channel_2, MC_Expose_us, 300);
   	McSetParamInt(m_Channel_2, MC_TrigLine, MC_TrigLine_IIN1);

    // Choose the number of images to acquire
    McSetParamInt(m_Channel_2, MC_SeqLength_Fr, MC_INDETERMINATE);

    // Retrieve image dimensions
    McGetParamInt(m_Channel_2, MC_ImageSizeX, &m_SizeX);
    McGetParamInt(m_Channel_2, MC_ImageSizeY, &m_SizeY);
    McGetParamInt(m_Channel_2, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt(m_Channel_2, MC_SurfaceCount, 3);

    // Enable MultiCam signals
    McSetParamInt(m_Channel_2, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel_2, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // 카메라 #2의 영상획득 완료시 GlobalCallback1 함수 실행
    McRegisterCallback(m_Channel_2, GlobalCallback2, this);
	// Euresis 영상획득 보드 카메라 #1 파라미터 설정
}

CEuresisCamLib::~CEuresisCamLib()
{
	// 응용 프로그램 종료시 영상획득 보드 제거 및 메모리 포인터 제거
#if _EURESIS_CAM
    // Set the channel to IDLE before deleting it
    McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_IDLE);
	McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_IDLE);
	McSetParamInt(m_Channel_3, MC_ChannelState, MC_ChannelState_IDLE);

    // Delete the channel
    McDelete(m_Channel_1);
	McDelete(m_Channel_2);
	McDelete(m_Channel_3);

	if (m_pImageUP) delete m_pImageUP;
	if (m_pImageDN) delete m_pImageDN;
#endif
	// 응용 프로그램 종료시 영상획득 보드 제거 및 메모리 포인터 제거
}

void CEuresisCamLib::SetExposureTime(long nCamID, long nBright)
{
	// ExposureTime 설정 함수
	if (nBright<100) nBright = 100;
	if (nBright>1000) nBright = 1000;
	nBright = DEF_EXPOSURE_TIME;

	if (nCamID==0)
	    McSetParamInt(m_Channel_1, MC_Expose_us, nBright);
	else if (nCamID==1)
	    McSetParamInt(m_Channel_2, MC_Expose_us, nBright);
	else 
		McSetParamInt(m_Channel_3, MC_Expose_us, nBright);
	// ExposureTime 설정 함수

	Sleep(1);
}

// HW, SW TRIGGER MODE 전환 함수(nCamID:카메라 선택, bHWTrigger==0:S/W Trigger, bHWTrigger==1:H/W Trigger)
// 수정요함
void CEuresisCamLib::SetTriggerMode(long nCamID, BOOL bTrigger)
{
	// TriggerMode 변환 함수
#if _EURESIS_CAM
	if (nCamID==0)
	{
		if (bTrigger==1)
		{
			McSetParamInt(m_Channel_1, MC_TrigMode, MC_TrigMode_SOFT);
			McSetParamInt(m_Channel_1, MC_NextTrigMode, MC_NextTrigMode_SOFT);
		}
		else if(bTrigger==2)
		{
			McSetParamInt(m_Channel_1, MC_TrigMode, MC_TrigMode_HARD);
			McSetParamInt(m_Channel_1, MC_NextTrigMode, MC_NextTrigMode_HARD);   
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_ACTIVE);
//			McSetParamInt(m_Channel_1, MC_TrigFilter,  MC_TrigFilter_STRONG);
		}
		else
		{
			McSetParamInt(m_Channel_1, MC_TrigMode, MC_TrigMode_IMMEDIATE);
			McSetParamInt(m_Channel_1, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_ACTIVE);
		}
	}
	else if (nCamID==1)
	{
		if (bTrigger==1)
        {
			McSetParamInt(m_Channel_2, MC_TrigMode, MC_TrigMode_SOFT);
			McSetParamInt(m_Channel_2, MC_NextTrigMode, MC_NextTrigMode_SOFT);
		}
		else if (bTrigger==2)
		{
			McSetParamInt(m_Channel_2, MC_TrigMode, MC_TrigMode_HARD);
			McSetParamInt(m_Channel_2, MC_NextTrigMode, MC_NextTrigMode_HARD);  
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_ACTIVE);
//			McSetParamInt(m_Channel_2, MC_TrigFilter,  MC_TrigFilter_STRONG);
		}
		else
		{
			McSetParamInt(m_Channel_2, MC_TrigMode, MC_TrigMode_IMMEDIATE);
			McSetParamInt(m_Channel_2, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_ACTIVE);
		}
	}
	else
	{
		if (bTrigger==1)
        {
			McSetParamInt(m_Channel_3, MC_TrigMode, MC_TrigMode_SOFT);
			McSetParamInt(m_Channel_3, MC_NextTrigMode, MC_NextTrigMode_SOFT);
		}
		else if (bTrigger==2)
		{
			McSetParamInt(m_Channel_3, MC_TrigMode, MC_TrigMode_HARD);
			McSetParamInt(m_Channel_3, MC_NextTrigMode, MC_NextTrigMode_HARD);  
			McSetParamInt(m_Channel_3, MC_ChannelState, MC_ChannelState_ACTIVE);
//			McSetParamInt(m_Channel_3, MC_TrigFilter,  MC_TrigFilter_STRONG);
		}
		else
		{
			McSetParamInt(m_Channel_3, MC_TrigMode, MC_TrigMode_IMMEDIATE);
			McSetParamInt(m_Channel_3, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
			McSetParamInt(m_Channel_3, MC_ChannelState, MC_ChannelState_ACTIVE);
		}
    }
#endif
	// TriggerMode 변환 함수

	Sleep(1);
}

void CEuresisCamLib::SetActivateTrigger(long nCamID, BOOL bFlag)
{
#if _EURESIS_CAM
	if (nCamID==0)
	{
		if (bFlag)
		{
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_ACTIVE);
			McSetParamInt(m_Channel_1, MC_ForceTrig, MC_ForceTrig_TRIG);
		}
		else
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_IDLE);
	}
	else if (nCamID==1)
	{
		if (bFlag)
		{
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_ACTIVE);
			McSetParamInt(m_Channel_2, MC_ForceTrig, MC_ForceTrig_TRIG);
		}
		else
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_IDLE);
	}
	else
	{
		if (bFlag)
		{
			McSetParamInt(m_Channel_3, MC_ChannelState, MC_ChannelState_ACTIVE);
			McSetParamInt(m_Channel_3, MC_ForceTrig, MC_ForceTrig_TRIG);
		}
		else
			McSetParamInt(m_Channel_3, MC_ChannelState, MC_ChannelState_IDLE);
	}
#endif

	Sleep(1);
}

/////////////////////////////////////////////////////////////////////////////
// GlobalCallback
void WINAPI GlobalCallback1(PMCSIGNALINFO SigInfo)
{
	// 카메라 #1의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback1(SigInfo);
    }
#endif
	// 카메라 #1의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
}

void WINAPI GlobalCallback2(PMCSIGNALINFO SigInfo)
{
	// 카메라 #2의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback2(SigInfo);
    }
#endif
	// 카메라 #2의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
}

/////////////////////////////////////////////////////////////////////////////
void CEuresisCamLib::Callback1(PMCSIGNALINFO SigInfo)
{
#if _EURESIS_CAM
	LPBYTE pImage;
	long nWidth, nHeight;
	char szbuf[125];

	nWidth = 0;
	nHeight = 0;

//	if (m_pImage[0]==NULL) return;
    switch(SigInfo->Signal)
    {
        case MC_SIG_SURFACE_PROCESSING:
            McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32)&m_pCurrent); 

			pImage = (LPBYTE)m_pCurrent;
            if (m_pMainFrm->m_bExecRealTime)
			{
				// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함
				m_pMainFrm->m_bCriticalF = TRUE;
				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
				// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함

				// 정상적으로 영상 획득 FLAG "TRUE" 현재시간, 검사시간 TIMER RESET함
				GetLocalTime(&m_pMainFrm->m_logStartTime);

				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_TOTAL_INSP);
				sprintf_s(g_LOG.m_sLogTitle1, "GRAB_CAM,%d", 1);
				// 정상적으로 영상 획득 FLAG "TRUE" 현재시간, 검사시간 TIMER RESET함

				//if (m_pMainFrm->m_GrabCAM[CAM_UP])
				//{
				//	sprintf_s(szbuf, "Double Grab(CAM=1)");
				//	m_pMainFrm->WriteLogToFile(szbuf);
				//	return;
				//}

#if _CAM12M
				// 비전DLL에 영상을 전달
//				NMultiFuncDll_TransConvertImageRot(pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, m_pImageUP, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);  //shsong
//				NAppDll_SetImagePtr(CAM_UP, (long)m_pImageUP);  //shsong
				NAppDll_SetImagePtr(CAM_UP, (long)pImage);
				// 비전DLL에 영상을 전달
#endif
				// 상태 표시 LED UPDATE
				//m_pMainFrm->m_GrabCAM[CAM_UP] = TRUE;
				m_pMainFrm->SetCheckLED(CAM_UP, TRUE);
				// 상태 표시 LED UPDATE

				
				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_UP);
								
				m_pMainFrm->m_bCriticalF = FALSE;
			}
			else
			{
				// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
#if _CAM12M
				// 비전DLL에 영상을 전달
//				NMultiFuncDll_TransConvertImageRot(pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, m_pImageUP, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);  // shsong
//				NAppDll_SetImagePtr(CAM_UP, (long)m_pImageUP);  //shsong
				NAppDll_SetImagePtr(CAM_UP, (long)pImage);
				// 비전DLL에 영상을 전달
#endif

				m_pMainFrm->m_bCriticalF = FALSE;
				NAppDll_InvalidateView(CAM_UP);
				// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
			}

            break;
        case MC_SIG_ACQUISITION_FAILURE:
            break;
        default:
            break;
    }
#endif
}

void CEuresisCamLib::Callback2(PMCSIGNALINFO SigInfo)
{
#if _EURESIS_CAM
	LPBYTE pImage;
	char szbuf[125];
	long nWidth, nHeight;

	nWidth = 0;
	nHeight = 0;

//	if (m_pImage[1]) return;

    switch(SigInfo->Signal)
    {
        case MC_SIG_SURFACE_PROCESSING:
            // Update "current" surface address pointer
            McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32)&m_pCurrent); 

			pImage = (LPBYTE)m_pCurrent;
			if (m_pMainFrm->m_bExecRealTime)
			{
				// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함
				m_pMainFrm->m_bCriticalF = TRUE;
				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
				// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함
				
                sprintf_s(g_LOG.m_sLogTitle2, "GRAB_CAM,%d", 2);
				// 정상적으로 영상 획득 FLAG "TRUE" 

				//if (m_pMainFrm->m_GrabCAM[CAM_DN])
				//{
				//	sprintf_s(szbuf, "Double Grab(CAM=2)");
				//	m_pMainFrm->WriteLogToFile(szbuf);
				//	return;
				//}

				// 정상적으로 영상 획득 FLAG "TRUE" 
#if _CAM12M
				// 하부 카메라 영상을 위치 변환하여 그 영상을 비전DLL로 전달 "0"
				NMultiFuncDll_TransConvertImageRot(pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, m_pImageDN, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);
				NAppDll_SetImagePtr(CAM_DN, (long)m_pImageDN);
				// 하부 카메라 영상을 위치 변환하여 그 영상을 비전DLL로 전달 "0"
#endif

				// 상/하부 검사를 실행
				//m_pMainFrm->m_GrabCAM[CAM_DN] = TRUE;
				m_pMainFrm->SetCheckLED(CAM_DN, TRUE);
				// 상/하부 검사를 실행

				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_DN);

				// 검사중일 때 USER버튼이 반응을 못하도록 하는 FLAG를 원래로 초기화
				m_pMainFrm->m_bCriticalF = FALSE;
				// 검사중일 때 USER버튼이 반응을 못하도록 하는 FLAG를 원래로 초기화
			}
			else
			{
				// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
#if _CAM12M
				NMultiFuncDll_TransConvertImageRot(pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, m_pImageDN, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);
				NAppDll_SetImagePtr(CAM_DN, (long)m_pImageDN);
#endif

				m_pMainFrm->m_bCriticalF = FALSE;
		    	NAppDll_InvalidateView(CAM_DN);
				// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
			}

            break;
        case MC_SIG_ACQUISITION_FAILURE:
            break;
        default:
            break;
    }
#endif
}

void CEuresisCamLib::SetImagePtr(long nCam, LPBYTE pImage)
{
	m_pImage[nCam] = pImage;
}
/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib message handlers
