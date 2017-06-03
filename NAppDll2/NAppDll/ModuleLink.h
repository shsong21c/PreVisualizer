#if !defined(AFX_VISION_H__B0A671D0_6C9B_4235_BC5E_00A8B49D905C__INCLUDED_)
#define AFX_VISION_H__B0A671D0_6C9B_4235_BC5E_00A8B49D905C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Vision.h : header file
//
#include "Modules\VisionLib.h"
#include "Modules\Mathematic.h"
#include "Modules\TransformLib.h"
#include "Modules\FunctionEtc.h"
#include "Modules\NMorphologyLib.h"
#include "MutualLink.h"

/////////////////////////////////////////////////////////////////////////////
// CModuleLink window
class CModuleLink
{
// Construction
public:
	CModuleLink();
	virtual ~CModuleLink();

	long     GetResultData() {return (long)m_pResData; };
	void     SetFilePath(CString strFilePath) { m_strSaveFilePath = strFilePath; };
    void     SetImageRawData(LPBYTE fmImage, long nWidth, long nHeight);
    void     SetModelData(SETUP_MODEL_DATA *pData) {m_pData = pData; m_MutualLink.SetModelData(pData); };
    void     SetModelBackData(SETUP_MODEL_DATA *pData) {m_pBack = pData; m_MutualLink.SetModelBackData(pData); };

public:
	long     ExecuteInspection();
	BOOL     ExecuteInspectionROI(long nIndex);
	void     InitializeInspData();
	void     InitializeInspDataROI(long nIndex);
    BOOL     CompensatePosition(long nLoc, float *dPX, float *dPY, float *dPT, float *dDX, float *dDY, float *dDT);
	void     PreProcessSpecialData();
	long     ProcessResultDecision();
	BOOL     InspectMeasurement();
	BOOL     InspectMeasurementROI(long nIndex);
	BOOL     InspectStraightLine();
	BOOL     InspectStraightLineROI(long nIndex);
	BOOL     InspectPatternMatch();
    BOOL     InspectPatternMatchROI(long nType, long nIndex);
	BOOL     InspectAverGrayValue();
	BOOL     InspectAverGrayValueROI(long nIndex);
	BOOL     InspectEtc();
	BOOL     InspectTabFolding();
	BOOL     InspectTabFoldingROI(long nIndex);	
	long     GetIndexTitleROI(LPCTSTR strTitle);
	long     GetIndexROI(BOOL bLoc, LPCTSTR strTitle);
	long     GetIndexTitleLinkROI(LPCTSTR strTitle);
	long     GetLineAverEdgePos(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nPol, float *fEdgePos, float *EDGE_X, float *EDGE_Y);
	BOOL     GetTapePosision(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, float *fF, float *fS);
	void     FineLaminateCal();
	void     FineLaminateOneCell();
	void     CalcOffsetData();

// Operations
public:
	CMutualLink        m_MutualLink;
	CString            m_strSaveFilePath;
	CAL_DATA          *m_pCAL;
	REALTIME_TEMP     *m_RT;

	INSP_RESULT_DATA  *m_pResData;	
	INSP_RESULT_DATA  *m_pResB;	
	
	CMathematic        m_libMath;
	CTransformLib      m_libTrans;
	CFunctionEtc       m_FuncEtc;  
	CVisionLib         m_libVis;
	CNMorphologyLib    m_libMorph;

	long               m_nInspImageCnt;
	long               m_nCamID;
	long               m_nOffsetMaxNum;

	BOOL               m_bCompens;
	float              m_dRadius;
	float              m_dAngle;
	float              m_dCX;
	float              m_dCY;
	float              m_fObjectEdgeY;
	float              m_fMDatumA;
	float              m_fMDatumT;
	float              m_fCalPixelDist;
	float              m_fMotionDx;
	float              m_fMotionDy;
	float              m_fMotionDt;
	float              m_fOffsetD[GOM_MAX_NUM];
	CString            m_strOffsetT[GOM_MAX_NUM];

protected:
	SETUP_MODEL_DATA  *m_pData;
	SETUP_MODEL_DATA  *m_pBack;

	LPBYTE             m_pRawImage;
	LPBYTE             m_pProcImage;
	long               m_nImageWidth;
	long               m_nImageHeight;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CModuleLink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
    long    GetPosEdgeDetection(POINT2 ptInsp1, POINT2 ptInsp2, POINT2 ptInsp3, POINT2 ptInsp4, long nInspType, long nType, long nDir, long nPolality, long nThresh, 
		                        POINT2 *ptPos1, POINT2 *ptPos2, float *dA, float *dB);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISION_H__B0A671D0_6C9B_4235_BC5E_00A8B49D905C__INCLUDED_)
