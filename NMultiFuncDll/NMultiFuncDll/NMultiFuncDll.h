// NMultiFuncDll.h : main header file for the NMultiFuncDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define TRANS_IMAGE_UPDN        0
#define TRANS_IMAGE_ROT90       1

// CNMultiFuncDllApp
// See NMultiFuncDll.cpp for the implementation of this class
//

class CNMultiFuncDllApp : public CWinApp
{
public:
	CNMultiFuncDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern "C" _declspec(dllexport) void NMultiFuncDll_TransConvertImage(LPBYTE pSourImage, LPBYTE pDestImage, long nSourWidth, long nSourHeight, long nTypeTB, long nTypeLR);
extern "C" _declspec(dllexport) void NMultiFuncDll_TransConvertImageRot(LPBYTE pSourImage, long nSourWidth, long nSourHeight, LPBYTE pDestImage, long nDestWidth, long nDestHeight, long nType);
extern "C" _declspec(dllexport) void NMultiFuncDll_GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight,  long* ProjX);
extern "C" _declspec(dllexport) void NMultiFuncDll_GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long* ProjY);