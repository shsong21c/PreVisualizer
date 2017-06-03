// NAppDll.h : NAppDll DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

// CNAppDllApp
// �� Ŭ������ ������ ������ NAppDll.cpp�� �����Ͻʽÿ�.
//
extern "C" _declspec(dllexport) void    NAppDll_Open(long pWnd, long nCamID, long nWidth, long nHeight);
extern "C" _declspec(dllexport) void    NAppDll_Close(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_SetEvent(long nCAM, LPVOID lpCallBack, long pCallData);

extern "C" _declspec(dllexport) void	NAppDll_CreateModelData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) BOOL	NAppDll_LoadModelData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) BOOL	NAppDll_SaveModelData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) long	NAppDll_GetModelData(long nCamID);

extern "C" _declspec(dllexport) void    NAppDll_GetImageSize(long nCamID, long *nWidth, long *nHeight);
extern "C" _declspec(dllexport) void	NAppDll_SetImageSize(long nCamID, long nWidth, long nHeight);
extern "C" _declspec(dllexport) void	NAppDll_SetFrameSize(long nCamID, long nWidth, long nHeight, long nBand, long nDepth);

extern "C" _declspec(dllexport) void	NAppDll_SaveImageData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void	NAppDll_AsSaveImageData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void	NAppDll_LoadImageData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NAppDll_SetEtcFilePath(long nCamID, LPCTSTR strFilePath);
extern "C" _declspec(dllexport) long	NAppDll_GetImagePtr(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_SetImagePtr(long nCamID, long pImage);

extern "C" _declspec(dllexport) void	NAppDll_InitResultData(long nCamID);
extern "C" _declspec(dllexport) long	NAppDll_GetResultData(long nCamID);
extern "C" _declspec(dllexport) long    NAppDll_GetRealTimeData(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_SetSpecData(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_SetFileName(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) long	NAppDll_ExecuteInspection(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_SetTransLanguage(long nCamID, long nValue);

extern "C" _declspec(dllexport) void	NAppDll_SetRemoteLight(long nCamID, BOOL bLight);
extern "C" _declspec(dllexport) long	NAppDll_GetBrightnessValue(long nCamID, long nCh);
extern "C" _declspec(dllexport) void	NAppDll_InvalidateView(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_ActiveSetupDialog(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_SetZoomScale(long nCamID, float fScale);
extern "C" _declspec(dllexport) void	NAppDll_SetScaleFactor(long nCamID, float fScaleX, float fScaleY);
extern "C" _declspec(dllexport) void    NAppDll_RealTimeMode(long nCamID, BOOL bRealTime);

extern "C" _declspec(dllexport) void    NAppDll_FindRotationAxis(double x1, double y1, double x2, double y2, double fAngle, double *fCoordiX, double *fCoordiY);
extern "C" _declspec(dllexport) void    NAppDll_TranslateCoordiPos(double dPX, double dPY, double dSX, double dSY, double dST, double *dTX, double *dTY);
extern "C" _declspec(dllexport) void    NAppDll_SetPressKey(long nCamID, long nCtrlKey, long nKey);

class CNAppDllApp : public CWinApp
{
public:
	CNAppDllApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
