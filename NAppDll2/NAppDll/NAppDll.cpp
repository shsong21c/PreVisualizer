// NAppDll.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//
#include "stdafx.h"
#include "NAppDll.h"
#include "math.h"
#include "NVisionDll.h"
#include "NLicenseKeyDll.h"
#include "DlgVDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: �� DLL�� MFC DLL�� ���� �������� ��ũ�Ǿ� �ִ� ���
//		MFC�� ȣ��Ǵ� �� DLL���� ���������� ��� �Լ���
//		���� �κп� AFX_MANAGE_STATE ��ũ�ΰ�
//		��� �־�� �մϴ�.
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �Ϲ����� �Լ� ������ ���⿡ �ɴϴ�.
//		}
//
//		�� ��ũ�δ� MFC�� ȣ���ϱ� ����
//		�� �Լ��� �ݵ�� ��� �־�� �մϴ�.
//		��, ��ũ�δ� �Լ��� ù ��° ���̾�� �ϸ� 
//		��ü ������ �����ڰ� MFC DLL��
//		ȣ���� �� �����Ƿ� ��ü ������ ����Ǳ� ����
//		���;� �մϴ�.
//
//		�ڼ��� ������
//		MFC Technical Note 33 �� 58�� �����Ͻʽÿ�.
// CNAppDllApp

BEGIN_MESSAGE_MAP(CNAppDllApp, CWinApp)
END_MESSAGE_MAP()

// CNAppDllApp ����
CNAppDllApp::CNAppDllApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

CNAppDllApp theApp;
CDlgVDisp   *m_pDlgVDisp[10];
BOOL         m_bIsLicenseKey;

// CNAppDllApp �ʱ�ȭ
BOOL CNAppDllApp::InitInstance()
{
	CWinApp::InitInstance();

	long i;
	for(i=0; i<10; i++)
	    m_pDlgVDisp[i] = NULL;

	m_bIsLicenseKey = FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////    DLL �ʱ�ȭ �� ���� �޼���   ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_Open(long pWnd, long nCamID, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd *pWndPtr = (CWnd *)pWnd;

	if (m_pDlgVDisp[nCamID]) delete m_pDlgVDisp[nCamID];
	m_pDlgVDisp[nCamID] = new CDlgVDisp(pWndPtr);
	m_pDlgVDisp[nCamID]->Create(IDD_DLG_VISION_DISP, pWndPtr);
	m_pDlgVDisp[nCamID]->SetCameraNum(nCamID);
	m_pDlgVDisp[nCamID]->SetImageSize(nWidth, nHeight);
	m_pDlgVDisp[nCamID]->ShowWindow(SW_SHOW);
}

_declspec(dllexport) void NAppDll_Close(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID]) delete m_pDlgVDisp[nCamID];
	m_pDlgVDisp[nCamID] = NULL;
}

_declspec(dllexport) void NAppDll_SetEvent(long nCAM, LPVOID lpCallBack, long pCallData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCAM])
	{
		m_pDlgVDisp[nCAM]->SetCallBackEvent(pCallData);
    	m_pDlgVDisp[nCAM]->m_lpCallBack = (void (__cdecl *)())lpCallBack;
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////    DLL �ʱ�ȭ �� ���� �޼���   ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////    �� ����Ÿ ȭ�� ���� �޼���   //////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_CreateModelData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->CreateModelData(strFileName);
}

_declspec(dllexport) long NAppDll_GetModelData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		return (long)m_pDlgVDisp[nCamID]->m_pData;

	return FALSE;
}

_declspec(dllexport) BOOL NAppDll_LoadModelData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet=TRUE;

	if (m_pDlgVDisp[nCamID])
	{
		bRet = m_pDlgVDisp[nCamID]->LoadModelData(strFileName);
		m_pDlgVDisp[nCamID]->LoadOffsetFile(strFileName);
	}

	return bRet;
}

_declspec(dllexport) BOOL NAppDll_SaveModelData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pModelData->CopyModelData(m_pDlgVDisp[nCamID]->m_pBackData,  m_pDlgVDisp[nCamID]->m_pData);
		m_pDlgVDisp[nCamID]->m_pModelData->SaveModelData(strFileName);
		m_pDlgVDisp[nCamID]->m_strFullPathName = strFileName;
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    �� ����Ÿ ȭ�� ���� �޼���   //////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////    ���� ����Ÿ ����� �޼���    ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_SetImageSize(long nCamID, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->SetImageSize(nWidth, nHeight);
}

_declspec(dllexport) void NAppDll_GetImageSize(long nCamID, long *nWidth, long *nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->m_pImageModule->GetImageSize(nWidth, nHeight);
}

_declspec(dllexport) void NAppDll_SetFrameSize(long nCamID, long nWidth, long nHeight, long nBand, long nDepth)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->SetFrameSize(nWidth, nHeight, nBand, nDepth);
}

_declspec(dllexport) long NAppDll_GetImagePtr(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
    	return m_pDlgVDisp[nCamID]->GetRawImagePtr();

	return 0;
}

_declspec(dllexport) void NAppDll_SetImagePtr(long nCamID, long pImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
    	m_pDlgVDisp[nCamID]->SetRawImagePtr(pImage);
}

_declspec(dllexport) void NAppDll_LoadImageData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->LoadImageData(strFileName);
}

_declspec(dllexport) void NAppDll_SaveImageData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->m_pImageModule->SaveImageFile(strFileName);
}

_declspec(dllexport) void NAppDll_AsSaveImageData(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->m_pImageModule->SaveAsImageFile(strFileName);
}

_declspec(dllexport) void NAppDll_SetFileName(long nCamID, LPCTSTR strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
		m_pDlgVDisp[nCamID]->m_strFullPathName = strFileName;
}

_declspec(dllexport) void NAppDll_SetEtcFilePath(long nCamID, LPCTSTR strFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		CreateDirectoryFullPath(strFilePath);
    	m_pDlgVDisp[nCamID]->m_pModuleLink->SetFilePath(strFilePath);
	}
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    ���� ����Ÿ ����� �޼���    ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////     �ǽð� �˻� ���� �޼���     ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_InitResultData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->m_pModuleLink->InitializeInspData();
}

_declspec(dllexport) void NAppDll_RealTimeMode(long nCamID, BOOL bRealTime)
{
    if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pImageModule->m_OverlayResultGOM.m_RT.m_bPosCAL = FALSE;
		m_pDlgVDisp[nCamID]->m_pModelData->SetRealTimeMode(bRealTime);		
	}
}

_declspec(dllexport) long NAppDll_ExecuteInspection(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet;

    if (m_pDlgVDisp[nCamID])
	{
	    m_pDlgVDisp[nCamID]->m_pModuleLink->ExecuteInspection();
        nRet = m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();
//		m_pDlgVDisp[nCamID]->InvalidateView();
	}

	return nRet;
}

_declspec(dllexport) long NAppDll_GetResultData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet;

    if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pModuleLink->ProcessResultDecision();
	    nRet = m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();
	}

	return nRet;
}

_declspec(dllexport) long NAppDll_GetRealTimeData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet=FALSE;

    if (m_pDlgVDisp[nCamID])
	{
		nRet = m_pDlgVDisp[nCamID]->m_pImageModule->GetRealTimeData();
	}

	return nRet;
}

_declspec(dllexport) void NAppDll_SetSpecData(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	INSP_RESULT_DATA *pResData;

    if (m_pDlgVDisp[nCamID])
	{
		pResData = (INSP_RESULT_DATA *)m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();

		pResData->m_nCamID = nCamID;
		m_pDlgVDisp[nCamID]->m_pModelData->SetSpecData(pResData);
		m_pDlgVDisp[nCamID]->m_pModelData->SaveModelData(m_pDlgVDisp[nCamID]->m_strFullPathName);
	}
}

_declspec(dllexport) void NAppDll_SetTransLanguage(long nCamID, long nValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->m_nLngSel = nValue;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////     �ǽð� �˻� ���� �޼���     ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////       Remote ���� ���� �Լ�     ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_SetRemoteLight(long nCamID, BOOL bLight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->SetRemoteLight(bLight);
}

_declspec(dllexport) long NAppDll_GetBrightnessValue(long nCamID, long nCh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nRet;

	if (m_pDlgVDisp[nCamID])
	    nRet = m_pDlgVDisp[nCamID]->GetBrightnessValue(nCh);

	return nRet;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////       Remote ���� ���� �Լ�     ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////      ȭ�� ���� �� ���󰳼�      ////////////////////////
/////////////////////////////////////////////////////////////////////////////
_declspec(dllexport) void NAppDll_SetZoomScale(long nCamID, float fScale)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pImageModule->SetZoomScale(fScale);
		//m_pDlgVDisp[nCamID]->UpdateScrollPos();
		m_pDlgVDisp[nCamID]->InvalidateView();
	}
}

_declspec(dllexport) void NAppDll_SetScaleFactor(long nCamID, float fScaleX, float fScaleY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID])
	{
		m_pDlgVDisp[nCamID]->m_pCAL.m_dScaleX = fScaleX;
		m_pDlgVDisp[nCamID]->m_pCAL.m_dScaleLT_X = fScaleX;
		m_pDlgVDisp[nCamID]->m_pCAL.m_dScaleLB_X = fScaleX;
		m_pDlgVDisp[nCamID]->m_pCAL.m_dScaleY = fScaleY;
		m_pDlgVDisp[nCamID]->m_pCAL.m_dScaleL_Y = fScaleY;

		m_pDlgVDisp[nCamID]->SaveCalToFile();
	}
}

_declspec(dllexport) void NAppDll_InvalidateView(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	INSP_RESULT_DATA *pRet;

	pRet = (INSP_RESULT_DATA *)m_pDlgVDisp[nCamID]->m_pModuleLink->GetResultData();
	if (m_pDlgVDisp[nCamID])
	{
		
	    m_pDlgVDisp[nCamID]->InvalidateView();
	}
}

_declspec(dllexport) void NAppDll_ActiveSetupDialog(long nCamID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#if 0
	//	if (!IsLicenseKey(_T("HASP-BASIC")))
	{
		//		MessageBox(NULL, _T(" Error in license key driver !  Please insert the License Key(USB). "),  _T(" Error Message "), MB_ICONEXCLAMATION&MB_ICONWARNING);
		//		return;
	}  
#endif // 0 // shsong


	if (m_pDlgVDisp[nCamID])
	    m_pDlgVDisp[nCamID]->ShowSetupDialog();
}

_declspec(dllexport) void NAppDll_SetPressKey(long nCamID, long nCtrlKey, long nKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pDlgVDisp[nCamID] && m_pDlgVDisp[nCamID]->m_bModifySetup)
	{
		m_pDlgVDisp[nCamID]->m_pModelData->SetPressKey(nCtrlKey, nKey);
		m_pDlgVDisp[nCamID]->InvalidateView();
	}
}

_declspec(dllexport) void NAppDll_FindRotationAxis(double x1, double y1, double x2, double y2, double fAngle, double *fCoordiX, double *fCoordiY)
{
    double cosT, sinT;
	double t1, t2, t3, x, y;

	x = 0.0f;
	y = 0.0f;

	if (x1>0 && y1>0 && x2>0 && y2>0)
	{  
		cosT = cos(fAngle*3.141592/180.0);
		sinT = sin(fAngle*3.141592/180.0);
    
		t1 = sinT;
		if (fabs(t1)<0.0000001) 
			t1 = 0.0000001;

		t2 = (-1+cosT)*(1-cosT)/t1 - sinT;
		if (fabs(t2)<0.0000001)  
			t2 = 0.0000001;

		t3 = 1-cosT;
		if (fabs(t3)<0.0000001)  
			t3 = 0.0000001;

		 y = (x2-x1*cosT-y1*sinT - (y2+x1*sinT-y1*cosT)*t3/t1)/t2;
		 x = (y*sinT-y1*sinT-x1*cosT+x2)/t3;
	}

	*fCoordiY = y;
	*fCoordiX = x;
}

_declspec(dllexport) void NAppDll_TranslateCoordiPos(double dPX, double dPY, double dSX, double dSY, double dST, double *dTX, double *dTY)
{
    double cosT, sinT;

	cosT = (float)cos(dST*3.141592/180.0);
    sinT = (float)sin(dST*3.141592/180.0);
    
    *dTX = cosT*(dSX-dPX)+sinT*(dSY-dPY)+dPX;
    *dTY = -sinT*(dSX-dPX)+cosT*(dSY-dPY)+dPY;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////      ȭ�� ���� �� ���󰳼�      ////////////////////////
/////////////////////////////////////////////////////////////////////////////




