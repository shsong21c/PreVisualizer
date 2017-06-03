// NBlobDll.h : NBlobDll DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

// CNBlobDllApp
// �� Ŭ������ ������ ������ NBlobDll.cpp�� �����Ͻʽÿ�.
//
#define  BLOB_MAX_NUM             100000

typedef struct 
{
	float   m_fArea;
	float   m_fCircle;

	float   m_fTopX;
	float   m_fTopY;
	float   m_fBotX;
	float   m_fBotY;

	float   m_fCenX;
	float   m_fCenY;

	float   m_fMinFeret;
	float   m_fMaxFeret;

	float   m_fPerimeter;
	float   m_fAngle;
	float   m_fCompact;

} BLOB_STRUCT;

/////////////////////////////////////////////////////////////////////////////
// CNPatternDllApp
// See NPatternDll.cpp for the implementation of this class
//

extern "C" _declspec(dllexport) void  NBlobDll_Open();
extern "C" _declspec(dllexport) void  NBlobDll_Close();
extern "C" _declspec(dllexport) long  NBlobDll_FindAutoThreshold(LPBYTE fm, long x1, long y1, long x2, long y2, long pitch);
extern "C" _declspec(dllexport) BOOL  NBlobDll_BinarizeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
extern "C" _declspec(dllexport) BLOB_STRUCT*   NBlobDll_BlobAnalysis(unsigned char *pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nMinArea, long nMaxArea, long nColor);
extern "C" _declspec(dllexport) long  NBlobDll_GetBlobCount();


class CNBlobDllApp : public CWinApp
{
public:
	CNBlobDllApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
