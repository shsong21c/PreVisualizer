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

// ī�޶�#1, #2 ����ȹ�� �Ϸ�� CALLBACK EVENT �Լ� ����
void    WINAPI     GlobalCallback1(PMCSIGNALINFO SigInfo);
void    WINAPI     GlobalCallback2(PMCSIGNALINFO SigInfo);
// ī�޶�#1, #2 ����ȹ�� �Ϸ�� CALLBACK EVENT �Լ� ����

/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib
CEuresisCamLib::CEuresisCamLib()
{
	// Euresis ����ȹ�� ���� �ʱ�ȭ �� �޸� �Ҵ�/������ �ʱ�ȭ
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
	// Euresis ����ȹ�� ���� �ʱ�ȭ �� �޸� �Ҵ�/������ �ʱ�ȭ
}

void CEuresisCamLib::InitBoard1()
{
	// Euresis ����ȹ�� ���� ī�޶� #1 �Ķ���� ����
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// ī�޶� ���� ������ ����
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// ī�޶� ���� ������ ����
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM5M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "A");
	
	// ī�޶� ���� ������ ����
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

    // ī�޶� #1�� ����ȹ�� �Ϸ�� GlobalCallback1 �Լ� ����
    McRegisterCallback(m_Channel_1, GlobalCallback1, this);
	// Euresis ����ȹ�� ���� ī�޶� #1 �Ķ���� ����
}

void CEuresisCamLib::InitBoard2()
{
	// Euresis ����ȹ�� ���� ī�޶� #2 �Ķ���� ����
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// ī�޶� ���� ������ ����
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology_MONO_DECA);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// ī�޶� ���� ������ ����
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

    // ī�޶� #2�� ����ȹ�� �Ϸ�� GlobalCallback1 �Լ� ����
    McRegisterCallback(m_Channel_2, GlobalCallback2, this);
	// Euresis ����ȹ�� ���� ī�޶� #1 �Ķ���� ����
}

CEuresisCamLib::~CEuresisCamLib()
{
	// ���� ���α׷� ����� ����ȹ�� ���� ���� �� �޸� ������ ����
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
	// ���� ���α׷� ����� ����ȹ�� ���� ���� �� �޸� ������ ����
}

void CEuresisCamLib::SetExposureTime(long nCamID, long nBright)
{
	// ExposureTime ���� �Լ�
	if (nBright<100) nBright = 100;
	if (nBright>1000) nBright = 1000;
	nBright = DEF_EXPOSURE_TIME;

	if (nCamID==0)
	    McSetParamInt(m_Channel_1, MC_Expose_us, nBright);
	else if (nCamID==1)
	    McSetParamInt(m_Channel_2, MC_Expose_us, nBright);
	else 
		McSetParamInt(m_Channel_3, MC_Expose_us, nBright);
	// ExposureTime ���� �Լ�

	Sleep(1);
}

// HW, SW TRIGGER MODE ��ȯ �Լ�(nCamID:ī�޶� ����, bHWTrigger==0:S/W Trigger, bHWTrigger==1:H/W Trigger)
// ��������
void CEuresisCamLib::SetTriggerMode(long nCamID, BOOL bTrigger)
{
	// TriggerMode ��ȯ �Լ�
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
	// TriggerMode ��ȯ �Լ�

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
	// ī�޶� #1�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback1(SigInfo);
    }
#endif
	// ī�޶� #1�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
}

void WINAPI GlobalCallback2(PMCSIGNALINFO SigInfo)
{
	// ī�޶� #2�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback2(SigInfo);
    }
#endif
	// ī�޶� #2�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
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
				// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��
				m_pMainFrm->m_bCriticalF = TRUE;
				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
				// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��

				// ���������� ���� ȹ�� FLAG "TRUE" ����ð�, �˻�ð� TIMER RESET��
				GetLocalTime(&m_pMainFrm->m_logStartTime);

				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_TOTAL_INSP);
				sprintf_s(g_LOG.m_sLogTitle1, "GRAB_CAM,%d", 1);
				// ���������� ���� ȹ�� FLAG "TRUE" ����ð�, �˻�ð� TIMER RESET��

				//if (m_pMainFrm->m_GrabCAM[CAM_UP])
				//{
				//	sprintf_s(szbuf, "Double Grab(CAM=1)");
				//	m_pMainFrm->WriteLogToFile(szbuf);
				//	return;
				//}

#if _CAM12M
				// ����DLL�� ������ ����
//				NMultiFuncDll_TransConvertImageRot(pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, m_pImageUP, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);  //shsong
//				NAppDll_SetImagePtr(CAM_UP, (long)m_pImageUP);  //shsong
				NAppDll_SetImagePtr(CAM_UP, (long)pImage);
				// ����DLL�� ������ ����
#endif
				// ���� ǥ�� LED UPDATE
				//m_pMainFrm->m_GrabCAM[CAM_UP] = TRUE;
				m_pMainFrm->SetCheckLED(CAM_UP, TRUE);
				// ���� ǥ�� LED UPDATE

				
				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_UP);
								
				m_pMainFrm->m_bCriticalF = FALSE;
			}
			else
			{
				// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
#if _CAM12M
				// ����DLL�� ������ ����
//				NMultiFuncDll_TransConvertImageRot(pImage, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, m_pImageUP, UP_IMAGE_WIDTH, UP_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);  // shsong
//				NAppDll_SetImagePtr(CAM_UP, (long)m_pImageUP);  //shsong
				NAppDll_SetImagePtr(CAM_UP, (long)pImage);
				// ����DLL�� ������ ����
#endif

				m_pMainFrm->m_bCriticalF = FALSE;
				NAppDll_InvalidateView(CAM_UP);
				// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
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
				// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��
				m_pMainFrm->m_bCriticalF = TRUE;
				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
				// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��
				
                sprintf_s(g_LOG.m_sLogTitle2, "GRAB_CAM,%d", 2);
				// ���������� ���� ȹ�� FLAG "TRUE" 

				//if (m_pMainFrm->m_GrabCAM[CAM_DN])
				//{
				//	sprintf_s(szbuf, "Double Grab(CAM=2)");
				//	m_pMainFrm->WriteLogToFile(szbuf);
				//	return;
				//}

				// ���������� ���� ȹ�� FLAG "TRUE" 
#if _CAM12M
				// �Ϻ� ī�޶� ������ ��ġ ��ȯ�Ͽ� �� ������ ����DLL�� ���� "0"
				NMultiFuncDll_TransConvertImageRot(pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, m_pImageDN, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);
				NAppDll_SetImagePtr(CAM_DN, (long)m_pImageDN);
				// �Ϻ� ī�޶� ������ ��ġ ��ȯ�Ͽ� �� ������ ����DLL�� ���� "0"
#endif

				// ��/�Ϻ� �˻縦 ����
				//m_pMainFrm->m_GrabCAM[CAM_DN] = TRUE;
				m_pMainFrm->SetCheckLED(CAM_DN, TRUE);
				// ��/�Ϻ� �˻縦 ����

				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(CAM_DN);

				// �˻����� �� USER��ư�� ������ ���ϵ��� �ϴ� FLAG�� ������ �ʱ�ȭ
				m_pMainFrm->m_bCriticalF = FALSE;
				// �˻����� �� USER��ư�� ������ ���ϵ��� �ϴ� FLAG�� ������ �ʱ�ȭ
			}
			else
			{
				// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
#if _CAM12M
				NMultiFuncDll_TransConvertImageRot(pImage, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, m_pImageDN, DN_IMAGE_WIDTH, DN_IMAGE_HEIGHT, TRANS_IMAGE_UPDN);
				NAppDll_SetImagePtr(CAM_DN, (long)m_pImageDN);
#endif

				m_pMainFrm->m_bCriticalF = FALSE;
		    	NAppDll_InvalidateView(CAM_DN);
				// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
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
