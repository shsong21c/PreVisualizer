// NMultiFuncDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NMultiFuncDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CNMultiFuncDllApp

BEGIN_MESSAGE_MAP(CNMultiFuncDllApp, CWinApp)
END_MESSAGE_MAP()


// CNMultiFuncDllApp construction

CNMultiFuncDllApp::CNMultiFuncDllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CNMultiFuncDllApp object

CNMultiFuncDllApp theApp;


// CNMultiFuncDllApp initialization

BOOL CNMultiFuncDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

_declspec(dllexport) void NMultiFuncDll_TransConvertImageRot(LPBYTE pSourImage, long nSourWidth, long nSourHeight, LPBYTE pDestImage, long nDestWidth, long nDestHeight, long nType)
{
	long i, j, nH;

	if (nType==TRANS_IMAGE_UPDN)
	{
		for(i=0; i<nSourHeight; i++)
		{
			nH = nSourHeight-i-1;
			for(j=0; j<nSourWidth; j++)
				pDestImage[nH*nSourWidth + j] = pSourImage[i*nSourWidth + j];
		}
	}
	else if (nType==TRANS_IMAGE_ROT90)
	{
		for(i=0; i<nSourHeight; i++)
		{
			nH = nDestWidth-i-1;
//#pragma omp parallel for 
			for(j=0; j<nSourWidth; j++)
				pDestImage[j*nDestWidth + nH] = pSourImage[i*nSourWidth + j];
		}
	}
	else if (nType==2)
	{
		for(i=0; i<nSourHeight; i++)
		{
			nH = i*nSourWidth;
			for(j=0; j<nSourWidth; j++)
				pDestImage[j*nDestWidth + i] = pSourImage[nH + (nSourWidth-j-1)];
		}
	}
	else
	{
		for(i=0; i<nSourHeight; i++)
		{
#pragma omp parallel for 
//			nH = nSourHeight-i-1;
			for(j=0; j<nSourWidth; j++)
				pDestImage[i*nSourWidth + j] = pSourImage[j*nDestWidth + i];
		}
	}
}

_declspec(dllexport) void NMultiFuncDll_TransConvertImage(LPBYTE pSourImage, LPBYTE pDestImage, long nSourWidth, long nSourHeight, long nTypeTB, long nTypeLR)
{
	long i, j, nH, nS, nD;

	if (nTypeLR==0 && nTypeTB==0)
		memcpy(pDestImage, pSourImage, sizeof(unsigned char)*nSourWidth*nSourHeight);
	else if (nTypeTB==0 && nTypeLR==1)
	{
		for(i=0; i<nSourHeight; i++)
		{
			nH = i*nSourWidth;

			#pragma omp parallel for 
			for(j=0; j<nSourWidth; j++)
				pDestImage[nH + nSourWidth-j-1] = pSourImage[nH + j];
		}
	}
	else if (nTypeTB==1 && nTypeLR==0)
	{
		for(i=0; i<nSourHeight; i++)
		{
			nD = (nSourHeight-i-1) * nSourWidth;
			nS = i*nSourWidth;

			#pragma omp parallel for 
			for(j=0; j<nSourWidth; j++)
				pDestImage[nD + j] = pSourImage[nS + j];
		}
	}
	else if (nTypeTB==1 && nTypeLR==1)
	{
		for(i=0; i<nSourHeight; i++)
		{
			nD = (nSourHeight-i-1) * nSourWidth;
			nS = i*nSourWidth;

			#pragma omp parallel for 
			for(j=0; j<nSourWidth; j++)
				pDestImage[nD + nSourWidth-j-1] = pSourImage[nS + j];
		}
	}
}

_declspec(dllexport) void NMultiFuncDll_GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX)
{
    long i, j, nNum;

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth)  x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=0; i<nWidth; i++) 
		ProjX[i] = 0;

	for(i=x1; i<=x2; i++) 
	{
		nNum = 0;

#pragma omp parallel for 
		for(j=y1; j<=y2; j++) {
		    ProjX[i] += pImage[j*nWidth + i];
			nNum++;
		}

		if (nNum > 0) 
			ProjX[i] /= nNum;
    }
}

_declspec(dllexport) void NMultiFuncDll_GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY)
{
    long i, j, nNum;

	if (x1<0) x1=0;
	if (y1<0) y1=0;
	if (x2>=nWidth) x2=nWidth-1;
	if (y2>=nHeight) y2=nHeight-1;

	for(i=0; i<nHeight; i++) 
		ProjY[i] = 0;

	for(i=y1; i<=y2; i++) 
	{
	    nNum = 0;

#pragma omp parallel for 
		for(j=x1; j<=x2; j++) 
		{
		    ProjY[i] += (long)pImage[i*nWidth + j];
		    nNum++;
		}

		if (nNum > 0) 
			ProjY[i] /= nNum;
    }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
