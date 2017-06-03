// NPatternDll.h : NPatternDll DLL�� �⺻ ��� �����Դϴ�.
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

// CNPatternDllApp
// �� Ŭ������ ������ ������ NPatternDll.cpp�� �����Ͻʽÿ�.
//
extern "C" _declspec(dllexport) void    NPatternDll_LoadPatternFromFile(LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NPatternDll_SavePatternToFile(LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NPatternDll_DeletePattern();
extern "C" _declspec(dllexport) void    NPatternDll_DeletePatternAll();
extern "C" _declspec(dllexport) LPBYTE  NPatternDll_GetPatternPtr(long *pPatWidth, long *pPatHeight);
extern "C" _declspec(dllexport) void    NPatternDll_RegistPattern(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nLowThresh, long nHighThresh, float fSAngle, float fEAngle);
extern "C" _declspec(dllexport) BOOL    NPatternDll_RotateImage(LPBYTE pImage, LPBYTE pBuffer, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float dAngle);
extern "C" _declspec(dllexport) BOOL    NPatternDll_FindPatternWithID(long nID, LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nSourWidth, long nSourHeight,  float dAcceptScore, float *dPosX, float *dPosY, float *dPatScore);
extern "C" _declspec(dllexport) BOOL    NPatternDll_FindShapePattern(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight,
												                     LPBYTE fmPatImage,  long nPatWidth, long nPatHeight, float dAcceptScore, float *dPosX, float *dPosY, float *dPatScore);
extern "C" _declspec(dllexport) BOOL    NPatternDll_FindPattern(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nSourWidth, long nSourHeight, LPBYTE fmPatImage,  long nPatWidth, long nPatHeight, float dAcceptScore, float *dPosX, float *dPosY, float *dPatScore);
extern "C" _declspec(dllexport) void    NPatternDll_ProcCannyEdge(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThreshLow, long nThreshHigh);

class CNPatternDllApp : public CWinApp
{
public:
	CNPatternDllApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
