// Vision.cpp : implementation file
//
#include "stdafx.h"
#include "math.h"
#include "ModuleLink.h"
#include "NVisionDll.h"
#include "NPatternDll.h"
//#include "NMultiFuncDll.h"
#include "NBlobDll.h"
#include <omp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float EDGE_X[TEMP_MAX_NUM], EDGE_Y[TEMP_MAX_NUM];
long  m_nEdgeDataNum;
/////////////////////////////////////////////////////////////////////////////
// CModuleLink
CModuleLink::CModuleLink()
{
	m_nImageWidth = IMAGE_WIDTH;
	m_nImageHeight = IMAGE_HEIGHT;
	m_pProcImage = NULL;
	m_fObjectEdgeY = 0;

	m_bCompens = FALSE;
	m_strSaveFilePath = _T("");

	NBlobDll_Open();
}

CModuleLink::~CModuleLink()
{
	NBlobDll_Close();

	delete m_pProcImage;
}

void CModuleLink::SetImageRawData(LPBYTE fmImage, long nWidth, long nHeight)
{
    m_pRawImage  = fmImage;
	
	m_nImageWidth = nWidth;
	m_nImageHeight= nHeight;

	if (m_pProcImage) delete m_pProcImage;
	m_pProcImage = new unsigned char[nWidth*nHeight];
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      전체 검사 모듈      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
long CModuleLink::ExecuteInspection()
{
	long nRet;
	CString str; 

	// 결과 데이타 초기화
	InitializeInspData();
//	m_MutualLink.InitializeMutualData();
	// 결과 데이타 초기화

	// 위치 보정 
	float dPX, dPY, dPT, dDX, dDY, dDT;
    CompensatePosition(0, &dPX, &dPY, &dPT, &dDX, &dDY, &dDT);
	//CompensatePosition(1, &dPX, &dPY, &dPT, &dDX, &dDY, &dDT);
	// 위치 보정

	// 전처리 프로세스  
	PreProcessSpecialData();
	// 전처리 프로세스  

	// Measurement Object 검사 
    InspectMeasurement();
	// Measurement Object 검사 

	// Measurement Object 검사 
    //InspectStraightLine();
	// Measurement Object 검사 

	// Measurement Object 검사 
    InspectPatternMatch();
	// Measurement Object 검사 

	// Measurement Object 검사 
    //InspectTabFolding();
	InspectEtc();
	// Measurement Object 검사 

	// Link Object 검사 
    m_MutualLink.InspectMutualLinkObjects();//
	// Link Object 검사 

	// Laminate Fine CAL 처리 
#if _DUO_CELL
	if (m_pData->m_bRealTimeMode==1)
	{
		//CalcOffsetData();
	    FineLaminateCal();		
	}
#else
//	if (m_pData->m_bRealTimeMode)
//	    FineLaminateOneCell();
#endif

	// Laminate Fine CAL 처리 

	// 최종 검사 결과 판정
    nRet = ProcessResultDecision();
	//memcpy(&m_pResB, &m_pResData, sizeof(INSP_RESULT_DATA));

    return nRet;
}

BOOL CModuleLink::ExecuteInspectionROI(long nIndex)
{
	BOOL bRet = FALSE;
	long nGOMType, nType;

	if (_tcscmp(m_pData->m_GOM[nIndex].m_strTitle, _T("")))
	{
		nType = m_pData->m_INSP[nIndex].m_nInspectType;
		nGOMType = m_pData->m_GOM[nIndex].m_nGOMType;

		switch(nType)
		{
			case INSP_MEASURE_SIDE_ONE :
			case INSP_MEASURE_SIDE_TWO :
				bRet = InspectMeasurementROI(nIndex);
				break;
			case INSP_PATTERN_MATCH :
				bRet = InspectPatternMatchROI(0, nIndex);
				break;
			case INSP_GRAY_VALUE :
				InspectAverGrayValueROI(nIndex);
				break;
			case INSP_TAB_FOLDING :
				InspectTabFoldingROI(nIndex);
				break;
		}
	}

    return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      전체 검사 모듈      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    결과 데이타 초기화    ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void CModuleLink::InitializeInspData()
{
	long i;

	m_fObjectEdgeY = 0;
	m_RT->m_nAlignPosCnt = 0;
	m_RT->m_fAngle = 0;

	m_fMDatumA = 0.0f;
	m_fMDatumT = 0.0f;
		
	m_fMotionDx = 0.0f;
	m_fMotionDy = 0.0f;
	m_fMotionDt = 0.0f;

	m_pResData->m_dPixelSX = 0.0f;
	m_pResData->m_dPixelSY = 0.0f;
	m_pResData->m_dPixelCX = 0.0f;
	m_pResData->m_dPixelCY = 0.0f;
	
	m_pResData->m_bDecision = FALSE;
	m_pResData->m_bStatus = FALSE;
	m_pResData->m_bFDecision = FALSE;
	m_pResData->m_bRDecision = FALSE;
	m_pResData->m_bFDataDec = FALSE;
	m_pResData->m_bRDataDec = FALSE;
	m_pResData->m_dOffsetEPC = 0.0f;

	m_pResData->m_dCorPx[0] = 0.0f;
	m_pResData->m_dCorPy[0] = 0.0f;
	m_pResData->m_dCorPx[1] = 0.0f;
	m_pResData->m_dCorPy[1] = 0.0f;
	m_pResData->m_dCorPx[2] = 0.0f;
	m_pResData->m_dCorPy[2] = 0.0f;
	m_pResData->m_dCorPx[3] = 0.0f;
	m_pResData->m_dCorPy[3] = 0.0f;

	m_pResData->m_dCenPx = 0.0f;
	m_pResData->m_dCenPy = 0.0f;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(m_pResData->m_strTitle[i], _T(""));
		m_pResData->m_bDecisionROI[i] = FALSE;
		m_pResData->m_bLocROI[i] = 0;
		m_pResData->m_nInspType[i] = 0;
		m_pResData->m_nPlotNum[i] = 0;
		m_pResData->m_dValue[i] = 0.0f;
		m_pResData->m_dLowLimit[i] = 0.0f;
		m_pResData->m_dHighLimit[i] = 0.0f;

		m_pResData->m_dPosRoiX1[i] = m_pData->m_GOM[i].m_ptPoint1.x;
		m_pResData->m_dPosRoiY1[i] = m_pData->m_GOM[i].m_ptPoint1.y;
		m_pResData->m_dPosRoiX2[i] = m_pData->m_GOM[i].m_ptPoint4.x;
		m_pResData->m_dPosRoiY2[i] = m_pData->m_GOM[i].m_ptPoint4.y;

        InitializeInspDataROI(i);
	}
	
	for(i=0; i<LOCATE_MAX_NUM; i++)	
		m_pData->m_Teach.m_LOC[i].m_bDecision = FALSE;

	for(i=0; i<LINK_MAX_NUM; i++)
	{
		_tcscpy_s(m_pResData->m_strLinkTitle[i], _T(""));
		m_pResData->m_bLinkDecROI[i] = FALSE;
		m_pResData->m_bLinkLocROI[i] = 0;
		m_pResData->m_nLinkType[i] = 0;
		m_pResData->m_nLinkPlotNum[i] = 0;
		m_pResData->m_dLinkValue[i] = 0.0f;
		m_pResData->m_dLinkLowLimit[i] = 0.0f;
		m_pResData->m_dLinkHighLimit[i] = 0.0f;

		m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
		m_pData->m_Teach.m_LINK[i].m_dCX = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dCY = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dValue = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dValueS = 0;
	}

	m_pData->m_Teach.m_bPreDecision = FALSE;
	m_pData->m_Teach.m_bLocDecision = FALSE;
	m_pData->m_Teach.m_bLinkDecision = FALSE;

    m_pData->m_bDecision = FALSE;                // 결과 판정
}

void CModuleLink::InitializeInspDataROI(long nIndex)
{
	POINT2 pt;

	m_pData->m_bDecision = FALSE;

	pt.x = 0.0f;
	pt.y = 0.0f;

	m_pData->m_dDeltaX = (float)0.0;             // 위치 보정을 위한 X Offset(검사시 적용)
	m_pData->m_dDeltaY = (float)0.0;             // 위치 보정을 위한 Y Offset(검사시 적용)
	m_pData->m_dDeltaT = (float)0.0;             // 각도 보정을 위한 T Offset(검사시 적용)

	m_pData->m_dDeltaX2 = (float)0.0;             // 위치 보정을 위한 X Offset(검사시 적용)
	m_pData->m_dDeltaY2 = (float)0.0;             // 위치 보정을 위한 Y Offset(검사시 적용)
	m_pData->m_dDeltaT2 = (float)0.0;             // 각도 보정을 위한 T Offset(검사시 적용)

	m_pData->m_Scan[nIndex].m_dFirstA = (float)0.0;
	m_pData->m_Scan[nIndex].m_dFirstB = (float)0.0;
	m_pData->m_Scan[nIndex].m_dFirstT = (float)0.0;

	m_pData->m_Scan[nIndex].m_dSecondA = (float)0.0;
	m_pData->m_Scan[nIndex].m_dSecondB = (float)0.0;
	m_pData->m_Scan[nIndex].m_dSecondT = (float)0.0;

	m_pData->m_Scan[nIndex].m_nFirstNum = 0;                 // SCAN EDGE 데이타 수 
	m_pData->m_Scan[nIndex].m_nSecondNum = 0;                // SCAN EDGE 데이타 수 

	m_pData->m_Scan[nIndex].m_ptFirstMin = pt;
	m_pData->m_Scan[nIndex].m_ptFirstMax = pt;
	m_pData->m_Scan[nIndex].m_ptSecondMin = pt;
	m_pData->m_Scan[nIndex].m_ptSecondMax = pt;

	m_pData->m_INSP[nIndex].m_bInspMode = TRUE;            // 해당 ROI 검사
	m_pData->m_INSP[nIndex].m_bDecision = FALSE;           // 해당 ROI 판정 (결과값) : TRUE, FALSE / -1 : 사용하지 않음
    m_pData->m_INSP[nIndex].m_dCX = 0.0f;
    m_pData->m_INSP[nIndex].m_dCY = 0.0f;
    m_pData->m_INSP[nIndex].m_dRadius = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas1.x = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas1.y = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas2.x = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas2.y = 0.0f;
	m_pData->m_INSP[nIndex].m_ptSecondMeas1.x = 0.0f;
	m_pData->m_INSP[nIndex].m_ptSecondMeas1.y = 0.0f;   
	m_pData->m_INSP[nIndex].m_ptSecondMeas2.x = 0.0f;   
	m_pData->m_INSP[nIndex].m_ptSecondMeas2.y = 0.0f;

	m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt;
	m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt;
	m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt;
	m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt;

    m_pData->m_INSP[nIndex].m_dAnyValue = 0.0f;
	m_pData->m_INSP[nIndex].m_dSumV = 0.0f;
	m_pData->m_INSP[nIndex].m_nDefectCnt = 0;
	m_pData->m_INSP[nIndex].m_dSumV = 0.0f;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    결과 데이타 초기화    ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         위치 보정        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::CompensatePosition(long nLoc, float *dPX, float *dPY, float *dPT, float *dDX, float *dDY, float *dDT)
{
	BOOL bPatF=FALSE, bRet=FALSE;
    long i, nCnt, nPosCnt, nType, nPos1, nW, nDir;
	float fTop, fLeft, dAngle, dSumX, dSumY;
	long nCntL, nCntT, nCntR, nCntB;
	long LP[10], TP[10], RP[10], BP[10], POS[10];
	float dPosX[10], dPosY[10], fCX[10], fCY[10];
	float fTargX, fTargY, fSwap, dAngle1, dAngle2;
	float fLenX, fLenY;
	float *pEdgeX, *pEdgeY;
	POINT2 ptL[10], ptR[10], ptCorLT, ptCorRB;

	*dPX = 0.0f;
	*dPY = 0.0f;
	*dDX = 0.0f;
	*dDY = 0.0f;
	*dDT = 0.0f;

	pEdgeX = new float[10000];
	pEdgeY = new float[10000];

	fTop = -1;
	fLeft = -1;
	nW = m_nImageWidth/2;
	
	fTargX = 0.0f;
	fTargY = 0.0f;
	
	nCntL = nCntT = nCntB = nCntR = 0;
	nPosCnt=0;
	for(i=0; i<m_pData->m_Teach.m_nLocNum; i++)
	{
		dPosX[nPosCnt] = 0;
		dPosY[nPosCnt] = 0;
		fCX[nPosCnt] = 0;
	    fCY[nPosCnt] = 0;

		nPos1 = m_pData->m_Teach.m_LOC[i].m_nIndex1;
		nType = m_pData->m_INSP[nPos1].m_nInspectType;
		nDir = m_pData->m_Scan[nPos1].m_nSecondScanDirType;
		if (nType==INSP_MEASURE_SIDE_ONE && nPos1>=0)
		{
			bRet = InspectMeasurementROI(nPos1);

			fCX[nPosCnt] = m_pData->m_GOM[nPos1].m_dCX;
			fCY[nPosCnt] = m_pData->m_GOM[nPos1].m_dCY;

			POS[nPosCnt] = nPos1;
			nPosCnt++;

			if (nDir==DIR_LEFT_RIGHT)
			{
				LP[nCntL] = nPos1;
		    	nCntL++;
			}
			if (nDir==DIR_RIGHT_LEFT)
			{
				RP[nCntR] = nPos1;
		    	nCntR++;
			}
			if (nDir==DIR_TOP_BOTTOM)
			{
				TP[nCntT] = nPos1;
		    	nCntT++;
			}
			if (nDir==DIR_BOTTOM_TOP)
			{
				BP[nCntB] = nPos1;
		    	nCntB++;
			}			
		}
	}

	if (nPosCnt>=4)
	{
		long nDataNum, j;
		float t, dA[4], dB[4];
		POINT2 pt1, pt2, pt3, pt4, ptCrossP;

		nDataNum = 0;
		for(i=0; i<nCntL; i++)
		{
			nPos1 = LP[i];
			nCnt = m_pData->m_Scan[nPos1].m_nFirstNum;
			for(j=0; j<nCnt; j++)
			{
				pEdgeX[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPX[j];
				pEdgeY[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPY[j];
				nDataNum++;
			}
		}
		m_libVis.LineFitting(nDataNum, pEdgeY, pEdgeX, (int)(nDataNum*0.1), 2, &t, &dA[0], &dB[0]);

		nDataNum = 0;
		for(i=0; i<nCntR; i++)
		{
			nPos1 = RP[i];
			nCnt = m_pData->m_Scan[nPos1].m_nFirstNum;
			for(j=0; j<nCnt; j++)
			{
				pEdgeX[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPX[j];
				pEdgeY[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPY[j];
				nDataNum++;
			}
		}
		m_libVis.LineFitting(nDataNum, pEdgeY, pEdgeX, (int)(nDataNum*0.1), 2, &t, &dA[1], &dB[1]);
		
		nDataNum = 0;
		for(i=0; i<nCntT; i++)
		{
			nPos1 = TP[i];
			nCnt = m_pData->m_Scan[nPos1].m_nFirstNum;
			for(j=0; j<nCnt; j++)
			{
				pEdgeX[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPX[j];
				pEdgeY[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPY[j];
				nDataNum++;
			}
		}
		m_libVis.LineFitting(nDataNum, pEdgeX, pEdgeY, (int)(nDataNum*0.1), 2, &t, &dA[2], &dB[2]);

		nDataNum = 0;
		for(i=0; i<nCntB; i++)
		{
			nPos1 = BP[i];
			nCnt = m_pData->m_Scan[nPos1].m_nFirstNum;
			for(j=0; j<nCnt; j++)
			{
				pEdgeX[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPX[j];
				pEdgeY[nDataNum] = m_pData->m_Scan[nPos1].m_dFirstPY[j];
				nDataNum++;
			}
		}
		m_libVis.LineFitting(nDataNum, pEdgeX, pEdgeY, (int)(nDataNum*0.1), 2, &t, &dA[3], &dB[3]);

		ptCorLT.x = 1.0f;
		ptCorLT.y = 1.0f;
		ptCorRB.x = (float)(m_nImageWidth-1);
		ptCorRB.y = (float)(m_nImageHeight-1);

		///////////////////////////////////////////////////////
		pt1.x = dA[0]*ptCorLT.y + dB[0];
		pt1.y = ptCorLT.y;
		pt2.x = dA[0]*ptCorRB.y + dB[0];
		pt2.y = ptCorRB.y;

		pt3.y = dA[2]*ptCorLT.x + dB[2];
		pt3.x = ptCorLT.x;
		pt4.y = dA[2]*ptCorRB.x + dB[2];
		pt4.x = ptCorRB.x;

		m_libMath.GetCrossPoint2Line(pt1, pt2, pt3, pt4, &ptCrossP);
		dPosX[0] = ptCrossP.x;
		dPosY[0] = ptCrossP.y;

		pt3.y = dA[3]*ptCorLT.x + dB[3];
		pt3.x = ptCorLT.x;
		pt4.y = dA[3]*ptCorRB.x + dB[3];
		pt4.x = ptCorRB.x;

		m_libMath.GetCrossPoint2Line(pt1, pt2, pt3, pt4, &ptCrossP);
		dPosX[1] = ptCrossP.x;
		dPosY[1] = ptCrossP.y;
		///////////////////////////////////////////////////////

		///////////////////////////////////////////////////////
		pt1.x = dA[1]*ptCorLT.y + dB[1];
		pt1.y = ptCorLT.y;
		pt2.x = dA[1]*ptCorRB.y + dB[1];
		pt2.y = ptCorRB.y;

		pt3.y = dA[2]*ptCorLT.x + dB[2];
		pt3.x = ptCorLT.x;
		pt4.y = dA[2]*ptCorRB.x + dB[2];
		pt4.x = ptCorRB.x;

		m_libMath.GetCrossPoint2Line(pt1, pt2, pt3, pt4, &ptCrossP);
		dPosX[2] = ptCrossP.x;
		dPosY[2] = ptCrossP.y;

		pt3.y = dA[3]*ptCorLT.x + dB[3];
		pt3.x = ptCorLT.x;
		pt4.y = dA[3]*ptCorRB.x + dB[3];
		pt4.x = ptCorRB.x;

		m_libMath.GetCrossPoint2Line(pt1, pt2, pt3, pt4, &ptCrossP);
		dPosX[3] = ptCrossP.x;
		dPosY[3] = ptCrossP.y;

		m_RT->m_nAlignPosCnt = 4;
		dSumX = 0.0f;
		dSumY = 0.0f;
		for(i=0; i<4; i++)
		{
			m_RT->m_ptAlignPos[i].x = dPosX[i];
			m_RT->m_ptAlignPos[i].y = dPosY[i];
			dSumX += dPosX[i];
			dSumY += dPosY[i];
		}

		m_RT->m_ptAlignCen.x = dSumX/4.0f;
		m_RT->m_ptAlignCen.y = dSumY/4.0f;
		///////////////////////////////////////////////////////
	}
	else
	{
		nPosCnt=0;
		dSumX = 0.0f;
		dSumY = 0.0f;
		for(i=0; i<m_pData->m_Teach.m_nLocNum; i++)
		{
			dPosX[nPosCnt] = 0;
			dPosY[nPosCnt] = 0;
			fCX[nPosCnt] = 0;
			fCY[nPosCnt] = 0;

			nPos1 = m_pData->m_Teach.m_LOC[i].m_nIndex1;
			if (nPos1>=0)
			{
				nType = m_pData->m_INSP[nPos1].m_nInspectType;
				if (nType==INSP_PATTERN_MATCH)
				{
					bRet = InspectPatternMatchROI(1, nPos1);
					if (bRet)
						goto goto_end;

					fCX[nPosCnt] = m_pData->m_GOM[nPos1].m_dCX;
					fCY[nPosCnt] = m_pData->m_GOM[nPos1].m_dCY;

					dPosX[nPosCnt] = m_pData->m_INSP[nPos1].m_dCX;
					dPosY[nPosCnt] = m_pData->m_INSP[nPos1].m_dCY;

					dSumX += dPosX[nPosCnt];
					dSumY += dPosY[nPosCnt];

					nPosCnt++;
					bPatF = TRUE;
				}
			}
		}
	}

	nCntL = 0;
	nCntR = 0;

	for(i=0; i<4; i++)
	{
	    if (dPosX[i]<nW)
		{
			ptL[nCntL].x = dPosX[i];
			ptL[nCntL].y = dPosY[i];
			nCntL++;
		}
		else
		{
			ptR[nCntR].x = dPosX[i];
			ptR[nCntR].y = dPosY[i];
			nCntR++;
		}
	}

	if (nCntL>=2)
	{
		if (ptL[0].y>ptL[1].y)
		{
			fSwap = ptL[0].y;
			ptL[0].y = ptL[1].y;
			ptL[1].y = fSwap;

			fSwap = ptL[0].x;
			ptL[0].x = ptL[1].x;
			ptL[1].x = fSwap;
		}
	}

	if (nCntR>=2)
	{
		if (ptR[0].y>ptR[1].y)
		{
			fSwap = ptR[0].y;
			ptR[0].y = ptR[1].y;
			ptR[1].y = fSwap;

			fSwap = ptR[0].x;
			ptR[0].x = ptR[1].x;
			ptR[1].x = fSwap;
		}
	}

	if (nCntL>=2 && nCntR>=2)
	{
		fLenX = fabs(ptR[0].x - ptL[0].x);
		fLenY = fabs(ptL[1].y - ptL[0].y);

		if (fLenX>fLenY)
		{
			m_libVis.GetAngle(ptL[0].x, ptL[0].y, ptR[0].x, ptR[0].y, &dAngle1);
			m_libVis.GetAngle(ptL[1].x, ptL[1].y, ptR[1].x, ptR[1].y, &dAngle2);
		}
		else
		{
			m_libVis.GetAngle(ptL[0].x, ptL[0].y, ptL[1].x, ptL[1].y, &dAngle1);
			m_libVis.GetAngle(ptR[0].x, ptR[0].y, ptR[1].x, ptR[1].y, &dAngle2);
		}

		dAngle = (dAngle1+dAngle2)/2.0f;
				
		fTargX = (ptL[0].x + ptL[1].x + ptR[0].x + ptR[1].x) / 4.0f;
		fTargY = (ptL[0].y + ptL[1].y + ptR[0].y + ptR[1].y) / 4.0f;

		m_pResData->m_dCorPx[0] = ptL[0].x;
		m_pResData->m_dCorPy[0] = ptL[0].y;
		m_pResData->m_dCorPx[1] = ptL[1].x;
		m_pResData->m_dCorPy[1] = ptL[1].y;
		m_pResData->m_dCorPx[2] = ptR[0].x;
		m_pResData->m_dCorPy[2] = ptR[0].y;
		m_pResData->m_dCorPx[3] = ptR[1].x;
		m_pResData->m_dCorPy[3] = ptR[1].y;

		m_pResData->m_dCenPx = fTargX;
		m_pResData->m_dCenPy = fTargY;
	
		*dPX = fTargX;
		*dPY = fTargY;
		*dPT = dAngle;
		m_RT->m_fAngle = dAngle;
		
	    if (nLoc==0)
		{
			*dDX = (float)(fTargX - m_pData->m_dSourX);
			*dDY = (float)(fTargY - m_pData->m_dSourY);
			*dDT = m_pData->m_dSourT-dAngle;

			m_pData->m_dDeltaX = *dDX;
			m_pData->m_dDeltaY = *dDY;
			m_pData->m_dDeltaT = *dDT;

			m_fMotionDx = fTargX;
			m_fMotionDy = fTargY;
			m_fMotionDt = dAngle;
		}
		else
		{
			*dDX = (float)(fTargX - m_pData->m_dSourX2);
			*dDY = (float)(fTargY - m_pData->m_dSourY2);
			*dDT = dAngle - m_pData->m_dSourT2;

			m_pData->m_dDeltaX2 = *dDX;
			m_pData->m_dDeltaY2 = *dDY;
			m_pData->m_dDeltaT2 = *dDT;
		}

		bRet = FALSE;
	}
	else
	{
		if (bPatF && nPosCnt>0)
		{
			dAngle = 0;
			if (nPosCnt==2)
				m_libVis.GetAngle(dPosX[0], dPosY[0], dPosX[1], dPosY[1], &dAngle);

			fTargX = dSumX/nPosCnt;
			fTargY = dSumY/nPosCnt;

			m_fMotionDx = fTargX;
			m_fMotionDy = fTargY;
			m_fMotionDt = dAngle;

			m_pResData->m_dCenPx = fTargX;
			m_pResData->m_dCenPy = fTargY;		
				
			*dPX = fTargX;
			*dPY = fTargY;
			*dPT = dAngle;
			
			*dDX = (float)(fTargX - m_pData->m_dSourX);
			*dDY = (float)(fTargY - m_pData->m_dSourY);
			*dDT = m_pData->m_dSourT-dAngle;

			m_pData->m_dDeltaX = *dDX;
			m_pData->m_dDeltaY = *dDY;
			m_pData->m_dDeltaT = *dDT;
		}
	}

goto_end:
	delete pEdgeX;
	delete pEdgeY;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         위치 보정        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////        전처리 함수        ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void CModuleLink::PreProcessSpecialData()
{
	BOOL bRet;
	long i, nType;
	float dA;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nType==INSP_DATUM_B)
		{
		    bRet = InspectStraightLineROI(i);
			dA = m_pData->m_Scan[i].m_dSecondA;

			m_fMDatumA = m_libMath.GetPerpendicularM(dA);
			m_fMDatumT = m_pData->m_Scan[i].m_dSecondT;
			break;
		}		
    }
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         측정 검사        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectMeasurement()
{
	long i, j, nGOMType, nType;
	long nPos1;
	BOOL bFlag, bRet = FALSE;

	m_fObjectEdgeY = 0.0f;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		nGOMType = m_pData->m_GOM[i].m_nGOMType;

		bFlag = FALSE;
		for (j=0; j<m_pData->m_Teach.m_nLocNum; j++)
		{
			nPos1 = m_pData->m_Teach.m_LOC[j].m_nIndex1;
			if (nPos1==i)
			{
				bFlag = TRUE;
				break;
			}
		}

		if (bFlag) continue;

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nGOMType==GOM_RECT && 
	    	(nType==INSP_MEASURE_SIDE_ONE || nType==INSP_MEASURE_SIDE_TWO))
		    bRet = InspectMeasurementROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectMeasurementROI(long nIndex)
{
	BOOL  bRet=FALSE, bRear;
	long  i, j, nCnt1, nCnt2, nH, nMid, nInvalidCnt, nRet;
	long  nInspType, nFirstPol, nSecondPol, nFirstDir, nSampleNum;
	long  nSecondDir, nFirstThresh, nSecondThresh, nScanType;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	POINT2 ptT1, ptT2, ptT3, ptT4, ptPos1, ptPos2;
	POINT2 pt1, pt2, pt3, pt4;
	POINT2 ptCross1, ptCross2, ptCross3, ptCross4;
	float fLowL, fHighL, dStdSpec, fF, fS, dNoTiltValue, fRate, fSize;
	float dA1, dB1, dA2, dB2, t;
	float a, b, c, d, dScale, dValue, fDiff, dOffsetV, fTransV, fMax, fBadValue;
	float ARR_VALUE[50];
	float *pEdgeX, *pEdgeY;
	CString str, strTitle;

	// 검사용 MODE로 전환
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환

    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);

	nInspType = m_pData->m_INSP[nIndex].m_nInspectType;
	nFirstPol = m_pData->m_Scan[nIndex].m_nFirstPolDirType;
	nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
	nFirstDir = m_pData->m_Scan[nIndex].m_nFirstScanDirType;
	nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;
	nFirstThresh = (long)m_pData->m_INSP[nIndex].m_dSensity;
	nSecondThresh = (long)m_pData->m_INSP[nIndex].m_dSensity;
	nScanType = SCAN_PROJECT_DFDT;//m_pData->m_Scan[nIndex].m_nScanType;
	nSampleNum = m_pData->m_INSP[nIndex].m_nSampleNum;
	bRear = m_pData->m_INSP[nIndex].m_bLocROI;
	dOffsetV = m_pData->m_INSP[nIndex].m_dOffsetV;
	fRate = m_pData->m_INSP[nIndex].m_bOverlapRate/10.0f;
	
	if (nSampleNum<1) 
		nSampleNum=1;

	nH = m_nImageHeight/2;
	nMid = (long)(ptInsp1.y + ptInsp4.y)/2;

	dValue = 0.0f;
	fTransV = 0.0f;
	dNoTiltValue = 0.0f;

	fLowL = m_pData->m_INSP[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_INSP[nIndex].m_dStandHighLimit;
	dStdSpec = (fLowL+fHighL)/2;

	str.Format(_T("%s"), m_pData->m_GOM[nIndex].m_strTitle);
	strTitle = WithoutSpaceToString(str);

	m_pData->m_Scan[nIndex].m_nFirstNum = 0;
	m_pData->m_Scan[nIndex].m_nSecondNum = 0;

	if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
	{
		if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
		if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
		if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
		if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;
	}
	else
	{
		if (bRear) dScale = m_pCAL->m_dScaleR_Y;
		else  dScale = m_pCAL->m_dScaleL_Y;
	}

	if (nInspType==INSP_MEASURE_SIDE_ONE)// || nInspType==INSP_SLINE)
	{
		pEdgeX = new float[EDGE_MAX_NUM];
		pEdgeY = new float[EDGE_MAX_NUM];

		m_pData->m_Scan[nIndex].m_nFirstNum = 0;
		m_pData->m_Scan[nIndex].m_nSecondNum = nSampleNum;

		nInvalidCnt=0;
		for(i=0; i<nSampleNum; i++)
		{
			dValue = 0.0f;
			ARR_VALUE[i] = 0.0f;

			m_pData->m_Scan[nIndex].m_dSecondPX[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dSecondPY[i] = 0.0f;
			
			ptT1 = ptInsp1;
			ptT2 = ptInsp2;
			ptT3 = ptInsp3;
			ptT4 = ptInsp4;

			if (nSampleNum>1) 
			{
				if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
				{
					fSize = (float)(ptInsp4.y-ptInsp1.y)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.y;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.y-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.y;
					}
					else
					{
						fF = ptInsp1.y + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageHeight-10) fS = (float)(m_nImageHeight-10);

					ptT1.y = fF;
					ptT2.y = fF;
					ptT3.y = fS;
					ptT4.y = fS;
				}
				else
				{
					fSize = (float)(ptInsp4.x-ptInsp1.x)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.x;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.x-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.x;
					}
					else
					{
						fF = ptInsp1.x + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageWidth-10) fS = (float)(m_nImageWidth-10);

					ptT1.x = fF;
					ptT2.x = fS;
					ptT3.x = fF;
					ptT4.x = fS;
				}
			}

			nCnt1 = GetPosEdgeDetection(ptT1, ptT2, ptT3, ptT4, nInspType, nScanType, nSecondDir, nSecondPol, nSecondThresh, &ptPos1, &ptPos2, &dA1, &dB1);
			if (nCnt1>0)
			{
				m_pData->m_INSP[nIndex].m_ptSecondMeas1 = ptPos1;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2 = ptPos2;

				m_pData->m_Scan[nIndex].m_dSecondPX[i] = (ptPos1.x+ptPos2.x)/2.0f;
				m_pData->m_Scan[nIndex].m_dSecondPY[i] = (ptPos1.y+ptPos2.y)/2.0f;

				for(j=0; j<nCnt1; j++)
				{
					pEdgeX[nInvalidCnt] = EDGE_X[j];
					pEdgeY[nInvalidCnt] = EDGE_Y[j];

					if (nInvalidCnt<EDGE_MAX_NUM)
					{
						m_pData->m_Scan[nIndex].m_dFirstPX[nInvalidCnt] = EDGE_X[j];
						m_pData->m_Scan[nIndex].m_dFirstPY[nInvalidCnt] = EDGE_Y[j];
					}			
					nInvalidCnt++;
				}

				m_pData->m_Scan[nIndex].m_nFirstNum = nInvalidCnt;
			}
		}
		
		if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
    		m_libVis.LineFitting(nInvalidCnt, pEdgeY, pEdgeX, (int)(nInvalidCnt*0.1), 2, &t, &dA1, &dB1);
		else
			m_libVis.LineFitting(nInvalidCnt, pEdgeX, pEdgeY, (int)(nInvalidCnt*0.1), 2, &t, &dA1, &dB1);
		
		m_pData->m_Scan[nIndex].m_dSecondA = dA1; 
		m_pData->m_Scan[nIndex].m_dSecondB = dB1; 
		
		if (!_tcscmp(strTitle, _T("TA")) || !_tcscmp(strTitle, _T("TC")))
		{
			ptT1.x = m_pData->m_INSP[nIndex].m_ptSecondMeas1.x+20;
			ptT1.y = ptInsp1.y+20;
			ptT4.x = ptInsp2.x;
			ptT4.y = ptInsp4.y-20;

			nRet = GetTapePosision(m_pRawImage, (long)ptT1.x, (long)ptT1.y, (long)ptT4.x, (long)ptT4.y, m_nImageWidth, m_nImageHeight, nSecondThresh, &fF, &fS);
			if (nRet)
			{
				m_pData->m_INSP[nIndex].m_ptSecondMeas1.x = fF;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2.x = fF;

				m_pData->m_INSP[nIndex].m_dAnyValue = fS;
			}
		}
		
		if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
			fSize = (float)(ptInsp4.y-ptInsp1.y) * 0.3f;
		else
			fSize = (float)(ptInsp4.x-ptInsp1.x) * 0.3f;

		if (fSize>nInvalidCnt)
		{
			dValue = 999.0f;
		    bRet = TRUE;
		}

		delete pEdgeX;
		delete pEdgeY;
	}

	if (nInspType==INSP_MEASURE_SIDE_TWO)
	{
		nFirstPol = m_pData->m_Scan[nIndex].m_nFirstPolDirType;
		nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
		nFirstDir = m_pData->m_Scan[nIndex].m_nFirstScanDirType;
		nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;

		m_pData->m_Scan[nIndex].m_nFirstNum = nSampleNum;
    	m_pData->m_Scan[nIndex].m_nSecondNum = nSampleNum;

		fBadValue = 0.0f;
		m_fCalPixelDist = 0.0;
		fMax = 0.0f;
		for(i=0; i<nSampleNum; i++)
		{
			dValue = 0.0f;
			ARR_VALUE[i] = 0.0f;

			m_pData->m_Scan[nIndex].m_dFirstPX[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dFirstPY[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dSecondPX[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dSecondPY[i] = 0.0f;

			ptT1 = ptInsp1;
			ptT2 = ptInsp2;
			ptT3 = ptInsp3;
			ptT4 = ptInsp4;

			if (nSampleNum>1) 
			{
				if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
				{
					fSize = (float)(ptInsp4.y-ptInsp1.y)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.y;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.y-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.y;
					}
					else
					{
						fF = ptInsp1.y + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageHeight-10) fS = (float)(m_nImageHeight-10);

					ptT1.y = fF;
					ptT2.y = fF;
					ptT3.y = fS;
					ptT4.y = fS;
				}
				else
				{
					fSize = (float)(ptInsp4.x-ptInsp1.x)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.x;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.x-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.x;
					}
					else
					{
						fF = ptInsp1.x + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageWidth-10) fS = (float)(m_nImageWidth-10);

					ptT1.x = fF;
					ptT2.x = fS;
					ptT3.x = fF;
					ptT4.x = fS;
				}
			}

			nInvalidCnt=0;
			nCnt1 = GetPosEdgeDetection(ptT1, ptT2, ptT3, ptT4, nInspType, nScanType, nFirstDir, nFirstPol, nFirstThresh, &ptPos1, &ptPos2, &dA1, &dB1);
			if (nCnt1>0)
			{
				m_pData->m_INSP[nIndex].m_ptFirstMeas1 = ptPos1;
				m_pData->m_INSP[nIndex].m_ptFirstMeas2 = ptPos2;
			}
		
			nFirstPol = m_pData->m_Scan[nIndex].m_nFirstPolDirType;
			nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
			nFirstDir = m_pData->m_Scan[nIndex].m_nFirstScanDirType;
			nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;

			nInvalidCnt=0;
			nCnt2 = GetPosEdgeDetection(ptT1, ptT2, ptT3, ptT4, nInspType, nScanType, nSecondDir, nSecondPol, nSecondThresh, &ptPos1, &ptPos2, &dA2, &dB2);
			if (nCnt2>0)
			{
				m_pData->m_INSP[nIndex].m_ptSecondMeas1 = ptPos1;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2 = ptPos2;
			}

			if (nCnt1>0 && nCnt2>0)
			{
				if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
				{
					pt1.x = dA1*ptT1.y + dB1;
					pt1.y = ptT1.y;
					pt2.x = dA1*ptT3.y + dB1;
					pt2.y = ptT3.y;

					pt3.x = dA2*ptT2.y + dB2;
					pt3.y = ptT2.y;
					pt4.x = dA2*ptT4.y + dB2;
					pt4.y = ptT4.y;

					dNoTiltValue = (fabs(pt1.x-pt3.x)+fabs(pt2.x-pt4.x))/2.0f;
				}
				else
				{
					pt1.y = dA1*ptT1.x + dB1;
					pt1.x = ptT1.x;
					pt2.y = dA1*ptT2.x + dB1;
					pt2.x = ptT2.x;

					pt3.y = dA2*ptT3.x + dB2;
					pt3.x = ptT3.x;
					pt4.y = dA2*ptT4.x + dB2;
					pt4.x = ptT4.x;

					dNoTiltValue = (fabs(pt1.y-pt3.y)+fabs(pt2.y-pt4.y))/2.0f;
				}

				m_libMath.GetCrossPointDotLine(pt1, pt3, pt4, &ptCross1);
				m_libMath.GetCrossPointDotLine(pt2, pt3, pt4, &ptCross2);
				m_libMath.GetCrossPointDotLine(pt3, pt1, pt2, &ptCross3);
				m_libMath.GetCrossPointDotLine(pt4, pt1, pt2, &ptCross4);

				a = (float)m_libMath.GetDistance2D(pt1, ptCross1);
				b = (float)m_libMath.GetDistance2D(pt2, ptCross2);
				c = (float)m_libMath.GetDistance2D(pt3, ptCross3);
				d = (float)m_libMath.GetDistance2D(pt4, ptCross4);

				dValue = (a+b+c+d) / 4.0f;
			
	#if !DEF_TILTING_ALG
				if (strTitle.Find(_T("A"))<0)
	    			dValue = dNoTiltValue;
	#endif

				m_pData->m_INSP[nIndex].m_ptFirstMeas1 = pt1;
				m_pData->m_INSP[nIndex].m_ptFirstMeas2 = pt2;
				m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt3;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt4;

				m_pData->m_Scan[nIndex].m_dFirstPX[i] = (pt1.x+pt2.x)/2.0f;
				m_pData->m_Scan[nIndex].m_dFirstPY[i] = (pt1.y+pt2.y)/2.0f; 
				m_pData->m_Scan[nIndex].m_dSecondPX[i] = (pt3.x+pt4.x)/2.0f;
				m_pData->m_Scan[nIndex].m_dSecondPY[i] = (pt3.y+pt4.y)/2.0f; 

				m_fCalPixelDist = dValue;
				dValue = (dValue * dScale);
				dValue += dOffsetV;
				fDiff = (float)fabs(dStdSpec - dValue);

				if (dValue<fLowL)  fTransV = dValue+DEF_TURNING_VALUE;
				if (dValue>fHighL) fTransV = dValue-DEF_TURNING_VALUE;

				if (dValue<0)  dValue = 0;
				if (fTransV<0) fTransV = 0;
			}

			if (fLowL<=dValue && dValue<=fHighL)
				ARR_VALUE[i] = dValue;
			else if (fLowL<=fTransV && fTransV<=fHighL)
				ARR_VALUE[i] = fTransV;
			else
			{
				if (fDiff>fMax)
				{
					fMax = fDiff;
					fBadValue = dValue;
				}
				bRet = TRUE;
			}
		}

		nMid = nSampleNum/2;
		if (bRet)
			dValue = fBadValue;
		else
			dValue = ARR_VALUE[nMid];
	}

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	if (fLowL>dValue || dValue>fHighL)
		bRet = TRUE;

	/*
	if (!_tcscmp(strTitle, _T("OMSP")) && !bRet)
	{
		ptT1.y = m_pData->m_INSP[nIndex].m_ptFirstMeas1.y;
		ptT2.y = m_pData->m_INSP[nIndex].m_ptSecondMeas1.y;
		if (ptT2.y>ptT1.y)
		    m_fObjectEdgeY = ptT2.y;
		else 
			m_fObjectEdgeY = ptT1.y;
	}
	*/

	m_pData->m_INSP[nIndex].m_bDecision = bRet;
	return bRet;
}

long CModuleLink::GetPosEdgeDetection(POINT2 ptInsp1, POINT2 ptInsp2, POINT2 ptInsp3, POINT2 ptInsp4, long nInspType, long nType, long nDir, long nPolality, long nThresh, 
									  POINT2 *ptPos1, POINT2 *ptPos2, float *dA, float *dB)
{
	BOOL  bRet=TRUE;
    long  nCnt, nPos=-1;
	long  ProfileData[TEMP_MAX_NUM];
	float t;
	float fEdgePos;

	// Profile Data구하기
    nCnt=0;
	ptPos1->x = 0.0f;
	ptPos1->y = 0.0f;
	ptPos2->x = 0.0f;
	ptPos2->y = 0.0f;

	if (nType ==SCAN_PROJECT_DFDT)
	{
//		 nThresh = abs(nThresh-MEDIUM_LEVEL);
        if (nDir==DIR_LEFT_RIGHT || nDir==DIR_RIGHT_LEFT) 
		{
			m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
			fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.x, (long)ptInsp4.x, nDir, nPolality, nThresh);
			if (fEdgePos>10 && fEdgePos<m_nImageWidth-10)
			{
	            nCnt = GetLineAverEdgePos(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nDir, nPolality, &fEdgePos, EDGE_X, EDGE_Y);
				if (nCnt>0)
				{
					m_libVis.LineFitting(nCnt, EDGE_Y, EDGE_X, (int)(nCnt*0.1), 2, &t, dA, dB);

					ptPos1->x = (*dA)*ptInsp1.y + (*dB);
					ptPos1->y = ptInsp1.y;
					ptPos2->x = (*dA)*ptInsp4.y + (*dB);
					ptPos2->y = ptInsp4.y;
				}
			}				 
		}
		else
		{
			/*
			nMid = (long)(ptInsp1.y+ptInsp4.y)/2;
			if (nInspType==INSP_MEASURE_SIDE_ONE && nDir==DIR_TOP_BOTTOM && nPolality==POL_BLACK_WHITE && nMid>m_nImageHeight/2)
			{
				nDir = DIR_BOTTOM_TOP;
				nPolality = POL_WHITE_BLACK;
				nTempThresh = 20;
				
				m_libVis.GetProjectFtY(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
				fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nDir, nPolality, nTempThresh);
				if (fEdgePos>10 && fEdgePos<m_nImageHeight-10)
			    {
					nTemp = (long)(fEdgePos-5);
			    	fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)nTemp, nDir, nPolality, nThresh);
				}
			}
			else
			{
		*/
			m_libVis.GetProjectFtY(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
			fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nDir, nPolality, nThresh);
//			}
			
			if (fEdgePos>10 && fEdgePos<m_nImageHeight-10)
			{
	            nCnt = GetLineAverEdgePos(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nDir, nPolality, &fEdgePos, EDGE_X, EDGE_Y);
				if (nCnt>0)
				{
					m_libVis.LineFitting(nCnt, EDGE_X, EDGE_Y, (int)(nCnt*0.1), 2, &t, dA, dB);

					ptPos1->y = (*dA)*ptInsp1.x + (*dB);
					ptPos1->x = ptInsp1.x;
					ptPos2->y = (*dA)*ptInsp4.x + (*dB);
					ptPos2->x = ptInsp4.x;
				}
			}
		}
	}

	return nCnt;
}

#define PIXEL_INTER_SCAN      7
long CModuleLink::GetLineAverEdgePos(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nPol, float *fEdge, float *EDGE_X, float *EDGE_Y)
{
	long i, j;
	long nCnt, nEdge, nMax, nDiff, nPos;
	float a, b, c, Temp_A, Temp_B, fEdgePos;
	double dSum;

	nPos = (long)(*fEdge);
	nCnt = 0;
	dSum = 0.0f;
	fEdgePos = 0.0f;

	if (nDir==DIR_LEFT_RIGHT)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos - PIXEL_INTER_SCAN; j <= nPos + PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[i*nWidth + (j+2)]-pImage[i*nWidth + (j-2)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos - PIXEL_INTER_SCAN; j <= nPos + PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[i*nWidth + (j-2)]-pImage[i*nWidth + (j+2)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_RIGHT_LEFT)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos + PIXEL_INTER_SCAN; j >= nPos - PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[i*nWidth + (j-2)]-pImage[i*nWidth + (j+2)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos + PIXEL_INTER_SCAN; j >= nPos - PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[i*nWidth + (j+2)]-pImage[i*nWidth + (j-2)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_TOP_BOTTOM)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos - PIXEL_INTER_SCAN; j <= nPos + PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[(j+2)*nWidth + i]-pImage[(j-2)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos - PIXEL_INTER_SCAN; j <= nPos + PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[(j-2)*nWidth + i]-pImage[(j+2)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_BOTTOM_TOP)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos + PIXEL_INTER_SCAN; j >= nPos - PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[(j-2)*nWidth + i]-pImage[(j+2)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for (j = nPos + PIXEL_INTER_SCAN; j >= nPos - PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[(j+2)*nWidth + i]-pImage[(j-2)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	*fEdge = 0;
	if (nCnt>0)
		*fEdge = (float)(dSum/nCnt);

	return nCnt;
}

#define TAG_MIN_LIMIT   50
BOOL CModuleLink::GetTapePosision(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, float *fF, float *fS)
{
	BOOL bRet=FALSE;
	long i, j, nCnt, nDiff;
	long PROJ[5000];

	*fF = 0.0f;
	*fS = 0.0f;

	for(i=x1; i<=x2; i++)
	{
		PROJ[i] = 0;
		nCnt = 0;
		for(j=y1; j<y2; j++)
		{
			PROJ[i] += pImage[j*nWidth + i];
			nCnt++;
		}

		if (nCnt>0)
		    PROJ[i] /= nCnt;
	}

	for(i=x1+3; i<x2-3; i++)
	{
		nDiff = abs(PROJ[i+1]-PROJ[i-1]);
		if (nDiff>nThresh)
		{
			*fF = (float)i;
			bRet = TRUE;
			break;
		}
	}

	/*
	for(i=x2-3; i>x1+3; i--)
	{
		nDiff = abs(PROJ[i+1]-PROJ[i-1]);
		if (nDiff>nThresh)
		{
			*fS = (float)i;
			bRet = TRUE;
			break;
		}
	}
	*/

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         측정 검사        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         직선 검사        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectStraightLine()
{
	long i, nGOMType, nType;
	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		nGOMType = m_pData->m_GOM[i].m_nGOMType;

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (INSP_SLINE<=nType && nType<=INSP_DATUM_C))
		    bRet = InspectStraightLineROI(i);
	}

	return bRet;
}


BOOL CModuleLink::InspectStraightLineROI(long nIndex)
{
	BOOL  bRet=FALSE, bRear;
	long  nInspType, nSecondPol;
	long  nSecondDir, nSecondThresh;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	POINT2 pt1, pt2, ptMinT, ptMaxT, ptTemp, ptCross;
	float fLowL, fHighL, fMidL, fF, fS, dTiltValue, fAvr;
	float t, dA, dB;
	float dScale, dValue, dOffsetV, fTransV;
	float dMinV, dMaxV, fRate;
	CString str, strTitle;
    long  i, j, nPos=-1;
	long  nCnt, nScanCnt, nH, nMid, nSampleNum, nTemp;
	long  ProfileData[TEMP_MAX_NUM];
	float fSum, fSize, fEdgePos;
	float EDGE_X[TEMP_MAX_NUM], EDGE_Y[TEMP_MAX_NUM], PX[TEMP_MAX_NUM], PY[TEMP_MAX_NUM];

	// 검사용 MODE로 전환
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환

    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);

	nInspType = m_pData->m_INSP[nIndex].m_nInspectType;
	nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
	nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;
	nSecondThresh = (long)m_pData->m_INSP[nIndex].m_dSensity;
	bRear = m_pData->m_INSP[nIndex].m_bLocROI;
	dOffsetV = m_pData->m_INSP[nIndex].m_dOffsetV;
	fRate = m_pData->m_INSP[nIndex].m_bOverlapRate/10.0f;

	nSampleNum = (long)m_pData->m_INSP[nIndex].m_nSampleNum;
	if (nSampleNum<1) 
		nSampleNum=1;

	dValue = 0.0f;
	fTransV = 0.0f;
	dTiltValue = 0.0f;

	fLowL = m_pData->m_INSP[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_INSP[nIndex].m_dStandHighLimit;
	fMidL = (fLowL+fHighL)/2;

	str.Format(_T("%s"), m_pData->m_GOM[nIndex].m_strTitle);
	strTitle = WithoutSpaceToString(str);

	fSize = 1;
	nH = m_nImageHeight/2;
	nMid = (long)(ptInsp1.y + ptInsp4.y)/2;

	if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
	{
		if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
		if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
		if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
		if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

	    fSize = (float)(ptInsp4.y-ptInsp1.y)/(float)nSampleNum;
	}
	else
	{
		if (bRear) dScale = m_pCAL->m_dScaleR_Y;
		else  dScale = m_pCAL->m_dScaleL_Y;

	    fSize = (float)(ptInsp4.x-ptInsp1.x)/(float)nSampleNum;
	}

	ptMinT.x = 100000;
	ptMinT.y = 100000;
	ptMaxT.x = 0;
	ptMaxT.y = 0;

	nScanCnt = 0;
	nTemp = 0;

	m_pData->m_Scan[nIndex].m_nFirstNum = 0;
	m_pData->m_Scan[nIndex].m_nSecondNum = 0;

    if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT) 
	{
		for(i=0; i<nSampleNum; i++)
		{
			if (i==0)
			{
				fF = ptInsp1.y;
				fS = fF+fSize + fSize*fRate*2.0f;
			}
			else if (i==nSampleNum-1)
			{
				fF = ptInsp4.y-fSize - fSize*fRate*2.0f;
				fS = ptInsp4.y;
			}
			else
			{
				fF = ptInsp1.y + i*fSize - fSize*fRate;
			    fS = fF+fSize + fSize*fRate*2.0f;
			}

			if (fF<10) fF = 10;
			if (fS>m_nImageHeight-10) fS = (float)(m_nImageHeight-10);

			m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)fF, (long)ptInsp4.x, (long)fS, m_nImageWidth, m_nImageHeight, ProfileData);
			fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.x, (long)ptInsp4.x, nSecondDir, nSecondPol, nSecondThresh);
			if (fEdgePos>10 && fEdgePos<m_nImageWidth-10)
			{
				nCnt = GetLineAverEdgePos(m_pRawImage, (long)ptInsp1.x, (long)fF, (long)ptInsp4.x, (long)fS, m_nImageWidth, m_nImageHeight, nSecondDir, nSecondPol, &fEdgePos, EDGE_X, EDGE_Y);
				if (nCnt>0)
				{
					fSum = 0.0f;
					for(j=0; j<nCnt; j++)
					{
						PX[nScanCnt] = EDGE_X[j];
						PY[nScanCnt] = EDGE_Y[j];
						fSum += EDGE_X[j];
						nScanCnt++;
					}

					fAvr = fSum/nCnt;
					if (fAvr<ptMinT.x) 
					{
						ptMinT.y = (fF+fS)/2.0f;
						ptMinT.x = fAvr;
					}

					if (fAvr>ptMaxT.x) 
					{
						ptMaxT.y = (fF+fS)/2.0f;
						ptMaxT.x = fAvr;
					}

					m_pData->m_Scan[nIndex].m_dSecondPX[nTemp] = fAvr;
					m_pData->m_Scan[nIndex].m_dSecondPY[nTemp] = (fF+fS)/2.0f;
					nTemp++;
				}
			}
		}

		if (nScanCnt>0 && nTemp>0)
		{
			m_pData->m_Scan[nIndex].m_ptSecondMin = ptMinT;
			m_pData->m_Scan[nIndex].m_ptSecondMax = ptMaxT;

			if (nInspType==INSP_DATUM_B || nInspType==INSP_DATUM_C) 
			{
				m_libVis.LineFitting1(nScanCnt, PX, PY, (int)(nScanCnt*0.2f), 2, &t, &dA, &dB);
				if (nInspType==INSP_DATUM_C) 
				{
					dA = m_fMDatumA;
					t = m_fMDatumT;
				}

				if (t==0)
				{
					if (nSecondDir==DIR_LEFT_RIGHT)
						ptTemp = ptMinT;
					else
						ptTemp = ptMaxT;

					pt1.x = ptTemp.x;
					pt2.x = ptTemp.x;
					pt1.y = ptInsp1.y;
					pt2.y = ptInsp3.y;
				}
				else
				{
					ptMinT.x = 10000;
			        ptMaxT.x = 0;

					for(i=0; i<nTemp; i++)
					{
						ptTemp.x = m_pData->m_Scan[nIndex].m_dSecondPX[i];
						ptTemp.y = m_pData->m_Scan[nIndex].m_dSecondPY[i];

						pt1.x = (ptInsp1.y-ptTemp.y + dA*ptTemp.x)/dA;
						pt1.y = ptInsp1.y;

						if (pt1.x<ptMinT.x) ptMinT=pt1;
						if (pt1.x>ptMaxT.x) ptMaxT=pt1;						 
					}

					if (nSecondDir==DIR_LEFT_RIGHT)
						ptTemp = ptMinT;
					else
						ptTemp = ptMaxT;

					pt1.x = (ptInsp1.y-ptTemp.y + dA*ptTemp.x)/dA;
					pt1.y = ptInsp1.y;
					pt2.x = (ptInsp3.y-ptTemp.y + dA*ptTemp.x)/dA;
					pt2.y = ptInsp3.y;
				}
			}
			else
			{
				m_libVis.LineFitting(nScanCnt, PY, PX, (int)(nScanCnt*0.2f), 2, &t, &dA, &dB);

				pt1.x = dA*ptInsp1.y + dB;
				pt1.y = ptInsp1.y;
				pt2.x = dA*ptInsp3.y + dB;
				pt2.y = ptInsp3.y;
			}
		
			// 직선도 계산 
    		ptMinT.x = 10000;
		    ptMaxT.x = 0;
			dMinV = 0;
			dMaxV = 0;

			for(i=0; i<nTemp; i++)
			{
				ptTemp.x =  m_pData->m_Scan[nIndex].m_dSecondPX[i];
				ptTemp.y =  m_pData->m_Scan[nIndex].m_dSecondPY[i];

				m_libMath.GetCrossPointDotLine(ptTemp, pt1, pt2, &ptCross);
				fS = (float)m_libMath.GetDistance2D(ptTemp, ptCross);

				fF = ptTemp.x - ptCross.x;
				if (fF<ptMinT.x) 
				{
					ptMinT.x = fF;
					dMinV = fS;
				}

				if (fF>ptMaxT.x)
				{
					ptMaxT.x = fF;
					dMaxV = fS;
				}
			}

			dValue = (dMinV + dMaxV)*dScale;
			// 직선도 계산 
		}
	}
	else
	{
		for(i=0; i<nSampleNum; i++)
		{
			if (i==0)
			{
				fF = ptInsp1.x;
				fS = fF+fSize + fSize*fRate*2.0f;
			}
			else if (i==nSampleNum-1)
			{
				fF = ptInsp4.x-fSize - fSize*fRate*2.0f;
				fS = ptInsp4.x;
			}
			else
			{
				fF = ptInsp1.x + i*fSize - fSize*fRate;
			    fS = fF+fSize + fSize*fRate*2.0f;
			}

			if (fF<10) fF = 10;
			if (fS>m_nImageWidth-10) fS = (float)(m_nImageWidth-10);

			m_libVis.GetProjectFtY(m_pRawImage, (long)fF, (long)ptInsp1.y, (long)fS, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
			fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nSecondDir, nSecondPol, nSecondThresh);
			if (fEdgePos>10 && fEdgePos<m_nImageHeight-10)
			{
				nCnt = GetLineAverEdgePos(m_pRawImage, (long)fF, (long)ptInsp1.y, (long)fS, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nSecondDir, nSecondPol, &fEdgePos, EDGE_X, EDGE_Y);
				if (nCnt>0)
				{
					fSum = 0.0f;
					for(j=0; j<nCnt; j++)
					{
						PX[nScanCnt] = EDGE_X[j];
						PY[nScanCnt] = EDGE_Y[j];
						fSum += EDGE_Y[j];
						nScanCnt++;
					}

					fAvr = fSum/nCnt;
					if (fAvr<ptMinT.y) 
					{
						ptMinT.x = (fF+fS)/2.0f;
						ptMinT.y = fAvr;
					}

					if (fAvr>ptMaxT.y) 
					{
						ptMaxT.x = (fF+fS)/2.0f;
						ptMaxT.y = fAvr;
					}

					m_pData->m_Scan[nIndex].m_dSecondPX[nTemp] = (fF+fS)/2.0f;
					m_pData->m_Scan[nIndex].m_dSecondPY[nTemp] = fAvr; 
					nTemp++;
				}
			}
		}

		if (nScanCnt>0 && nTemp>0)
		{
			m_pData->m_Scan[nIndex].m_ptSecondMin = ptMinT;
			m_pData->m_Scan[nIndex].m_ptSecondMax = ptMaxT;

			if (nInspType==INSP_DATUM_B || nInspType==INSP_DATUM_C) 
			{
				m_libVis.LineFitting(nScanCnt, PX, PY, (int)(nScanCnt*0.2), 2, &t, &dA, &dB);
				if (nInspType==INSP_DATUM_C) 
				{
					dA = m_fMDatumA;
					t = m_fMDatumT;
				}

				if (t==0)
				{
					if (nSecondDir==DIR_TOP_BOTTOM)
						ptTemp = ptMaxT;
					else
						ptTemp = ptMinT;

					pt1.x = ptInsp1.x;
					pt2.x = ptInsp2.x;
					pt1.y = ptTemp.y;
					pt2.y = ptTemp.y;
				}
				else
				{
					ptMinT.y = 10000;
			        ptMaxT.y = 0;

					for(i=0; i<nTemp; i++)
					{
						ptTemp.x = m_pData->m_Scan[nIndex].m_dSecondPX[i];
						ptTemp.y = m_pData->m_Scan[nIndex].m_dSecondPY[i];

						pt1.y = dA*(ptInsp1.x-ptTemp.x) + ptTemp.y;
						pt1.x = ptInsp1.x;

						if (pt1.y<ptMinT.y) ptMinT=pt1;
						if (pt1.y>ptMaxT.y) ptMaxT=pt1;						 
					}

					if (nSecondDir==DIR_TOP_BOTTOM)
						ptTemp = ptMaxT;
					else
						ptTemp = ptMinT;

					pt1.y = dA*(ptInsp1.x-ptTemp.x) + ptTemp.y;
					pt1.x = ptInsp1.x;
					pt2.y = dA*(ptInsp2.x-ptTemp.x) + ptTemp.y;
					pt2.x = ptInsp2.x;
				}
			}
			else
			{
				m_libVis.LineFitting(nScanCnt, PX, PY, (int)(nScanCnt*0.2), 2, &t, &dA, &dB);

				pt1.y = dA*ptInsp1.x + dB;
				pt1.x = ptInsp1.x;
				pt2.y = dA*ptInsp2.x + dB;
				pt2.x = ptInsp2.x;
			}

			// 직선도 계산 
    		ptMinT.y = 0;
		    ptMaxT.y = 0;
			dMinV = 0;
			dMaxV = 0;

			for(i=0; i<nTemp; i++)
			{
				ptTemp.x =  m_pData->m_Scan[nIndex].m_dSecondPX[i];
				ptTemp.y =  m_pData->m_Scan[nIndex].m_dSecondPY[i];

				m_libMath.GetCrossPointDotLine(ptTemp, pt1, pt2, &ptCross);
				fS = (float)m_libMath.GetDistance2D(ptTemp, ptCross);

				fF = ptTemp.y - ptCross.y;
				if (fF<ptMinT.y) 
				{
					ptMinT.y = fF;
					dMinV = fS;
				}

				if (fF>ptMaxT.y)
				{
					ptMaxT.y = fF;
					dMaxV = fS;
				}
			}

			dValue = (dMinV + dMaxV)*dScale;
			// 직선도 계산 
		}
	}

	m_pData->m_Scan[nIndex].m_nSecondNum = nTemp;
	m_pData->m_Scan[nIndex].m_dSecondA = dA; 
	m_pData->m_Scan[nIndex].m_dSecondB = dB; 
	m_pData->m_Scan[nIndex].m_dSecondT = t;

	m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt1;
	m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt2;

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	if (nInspType==INSP_STRAIGHTNESS)
	{
		if (dValue<fLowL || fHighL<dValue)
			m_pData->m_INSP[nIndex].m_bDecision = TRUE;
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         직선 검사        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      패턴 매칭 검사      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
#define DEF_INSP_THICK    10
BOOL CModuleLink::InspectPatternMatch()
{
	return FALSE;

	long i, nType;
 	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nType==INSP_PATTERN_MATCH) 
		    bRet = InspectPatternMatchROI(0, i);
	}

	return bRet;
}

BOOL CModuleLink::InspectPatternMatchROI(long nType, long nIndex)
{
	BOOL   bPatRet=TRUE, bRet=TRUE;
	LPBYTE pPAT, pHalfPAT, pHalfImage, pPatSepa, pPatPol;
	long   i, j, nPatWidth, nSideWidth, nSideHeight, nInspType;
	long   nPatNum, nPatHeight, nRate, nHalfWidth, nHalfHeight;
	long nHalfPWidth, nHalfPHeight, nCntX, nCntY, nPx, nPy;
	float  dPosX, dPosY, dScore1, dScore2, dMaxScore, dPatScore;
	CRect  rcSearchRect, rcBounds;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	dPatScore = m_pData->m_INSP[nIndex].m_dStandLowLimit;//(float)m_pData->m_INSP[nIndex].m_nPatScore;
	nPatWidth = m_pData->m_INSP[nIndex].m_nPatWidth;
	nPatHeight = m_pData->m_INSP[nIndex].m_nPatHeight;
	nRate = m_pData->m_INSP[nIndex].m_nPatSearchRate;
	nPatNum = m_pData->m_INSP[nIndex].m_nPatNum;
	nInspType = m_pData->m_INSP[nIndex].m_nBlobOption;
	m_pData->m_GOM[nIndex].m_strTitle;

	// 위치보정
	ptInsp1 = m_pData->m_GOM[nIndex].m_ptPoint1;
	ptInsp2 = m_pData->m_GOM[nIndex].m_ptPoint2;
	ptInsp3 = m_pData->m_GOM[nIndex].m_ptPoint3;
	ptInsp4 = m_pData->m_GOM[nIndex].m_ptPoint4;
	if (nType==0)
	{
        m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
//		nRate = 170;
	}
	else
	{
		if (nRate>500) nRate=500;
		if (nRate<200) nRate=200;
	}
	// 위치보정

	nSideWidth  = (long)(nPatWidth/2 * nRate/100.0);
	nSideHeight = (long)(nPatHeight/2* nRate/100.0);

	rcSearchRect.left = (long)(ptInsp1.x+ptInsp4.x)/2 - nSideWidth;
	rcSearchRect.top =  (long)(ptInsp1.y+ptInsp4.y)/2 - nSideHeight;
	rcSearchRect.right = (long)(ptInsp1.x+ptInsp4.x)/2 + nSideWidth;
	rcSearchRect.bottom = (long)(ptInsp1.y+ptInsp4.y)/2 + nSideHeight;

	if (rcSearchRect.left<0) rcSearchRect.left = 0;
	if (rcSearchRect.top <0) rcSearchRect.top = 0;
	if (rcSearchRect.right >=m_nImageWidth)  rcSearchRect.right = m_nImageWidth-1;
	if (rcSearchRect.bottom>=m_nImageHeight) rcSearchRect.bottom= m_nImageHeight-1;

	m_pData->m_GOM[nIndex].m_dCX = (ptInsp1.x+ptInsp4.x)/2;
	m_pData->m_GOM[nIndex].m_dCY = (ptInsp1.y+ptInsp4.y)/2;
	dScore1 = 0.0f;

	if (nType==1)
	{
		pPAT = m_pData->m_INSP[nIndex].m_pPat[0];

		nHalfPWidth = nPatWidth/2 + 1;
		nHalfPHeight = nPatHeight/2 + 1;
		pHalfPAT = new unsigned char[nHalfPWidth*nHalfPHeight];

		nHalfWidth = (rcSearchRect.right-rcSearchRect.left)/2 + 1;
		nHalfHeight = (rcSearchRect.bottom-rcSearchRect.top)/2 + 1;
		pHalfImage = new unsigned char[nHalfWidth*nHalfHeight];
		
		for(i=0, nCntY=0; i<nPatHeight; i+=2, nCntY++)
		for(j=0, nCntX=0; j<nPatWidth; j+=2, nCntX++)
		{
			pHalfPAT[nCntY*nHalfPWidth + nCntX] = pPAT[i*nPatWidth + j];
		}

		for(i=rcSearchRect.top, nCntY = 0; i<rcSearchRect.bottom; i+=2, nCntY++)
		for(j=rcSearchRect.left, nCntX=0; j<rcSearchRect.right; j+=2, nCntX++)
		{
			pHalfImage[nCntY*nHalfWidth + nCntX] = m_pRawImage[i*m_nImageWidth + j];
		}

		dMaxScore = 0.0f;
		if (pPAT && nPatWidth>0 && nPatHeight>0)
		{
			bPatRet = NPatternDll_FindPattern(pHalfImage, 0, 0, nHalfWidth-1, nHalfHeight-1,
												nHalfWidth, nHalfHeight, pHalfPAT, nHalfPWidth, nHalfPHeight, 20, &dPosX, &dPosY, &dScore1);
			if (!bPatRet && dScore1>70.0)
			{
				dMaxScore = dScore1;

				m_pData->m_GOM[nIndex].m_dCX;
				m_pData->m_GOM[nIndex].m_dCY;
				m_pData->m_INSP[nIndex].m_dCX = rcSearchRect.left + dPosX*2;
				m_pData->m_INSP[nIndex].m_dCY = rcSearchRect.top + dPosY*2;
			}

			if (dScore1>70.0) 
				bRet = FALSE;
		}

		delete pHalfPAT;
		delete pHalfImage;
	}
	else
	{
		pPAT = m_pData->m_INSP[nIndex].m_pPat[0];

		nPx = nPatWidth/2;
		nPy = nPatHeight/2;
		dMaxScore = 0.0f;
	
		if (pPAT && nPatWidth>0 && nPatHeight>0)
		{
			bPatRet = NPatternDll_FindPattern(m_pRawImage, rcSearchRect.left, rcSearchRect.top, rcSearchRect.right, rcSearchRect.bottom,
												m_nImageWidth, m_nImageHeight, pPAT, nPatWidth, nPatHeight, 40, &dPosX, &dPosY, &dScore1);
			if (!bPatRet && dScore1>dMaxScore)
			{
				dMaxScore = dScore1;

				m_pData->m_INSP[nIndex].m_dCX = dPosX;
				m_pData->m_INSP[nIndex].m_dCY = dPosY;
			}

			if (dScore1>dPatScore) 
				bRet = FALSE;
		}

		if (nInspType<=0)
			goto goto_end;

		pPatSepa = new unsigned char[nPatWidth*nPatHeight];
		pPatPol = new unsigned char[nPatWidth*nPatHeight];
		memset(pPatSepa, 0, nPatWidth*nPatHeight);
		memset(pPatPol, 0, nPatWidth*nPatHeight);
		
		rcBounds.left = nPx-DEF_INSP_THICK;
		rcBounds.right = nPx+DEF_INSP_THICK;
		rcBounds.top = nPy-DEF_INSP_THICK;
		rcBounds.bottom = nPy+DEF_INSP_THICK;

		if (nInspType==1)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<rcBounds.top || j<rcBounds.left) continue;
				if (i>rcBounds.bottom && j>rcBounds.right) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<nPy+3 || j<nPx+3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}
		if (nInspType==2)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>rcBounds.bottom || j<rcBounds.left) continue;
				if (i<rcBounds.top && j>rcBounds.right) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>nPy-3 || j<nPx+3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}
		if (nInspType==3)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<rcBounds.top || j>rcBounds.right) continue;
				if (i>rcBounds.bottom && j>rcBounds.right) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<nPy+3 || j>nPx-3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}
		if (nInspType==4)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>rcBounds.bottom || j>rcBounds.right) continue;
				if (i<rcBounds.top && j<rcBounds.left) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>nPy-3 || j>nPx-3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}

		bRet=TRUE;
		dMaxScore = 0.0f;
		dScore1 = 0;
		dScore2 =0;
		if (pPAT && nPatWidth>0 && nPatHeight>0)
		{
			bPatRet = NPatternDll_FindShapePattern(m_pRawImage, rcSearchRect.left, rcSearchRect.top, rcSearchRect.right, rcSearchRect.bottom,
											    	m_nImageWidth, m_nImageHeight, pPatSepa, nPatWidth, nPatHeight, 20, &dPosX, &dPosY, &dScore1);
			if (!bPatRet && dScore1>50)
			{
				m_pData->m_INSP[nIndex].m_dCX = dPosX;
				m_pData->m_INSP[nIndex].m_dCY = dPosY;
			}

			bPatRet = NPatternDll_FindShapePattern(m_pRawImage, rcSearchRect.left, rcSearchRect.top, rcSearchRect.right, rcSearchRect.bottom,
											    	m_nImageWidth, m_nImageHeight, pPatPol, nPatWidth, nPatHeight, 20, &dPosX, &dPosY, &dScore2);
			if (dScore1>=dPatScore && dScore2>=dPatScore)
				bRet = FALSE;

			if (dScore1<dScore2)
				dMaxScore = dScore1;
			else
				dMaxScore = dScore2;
		}

//		m_libVis.SaveBitmapImage(_T("c:\\strFileName.bmp"), pPatSepa, nPatWidth, nPatHeight);
		delete pPatSepa;
		delete pPatPol;
	}

goto_end:
	m_pData->m_INSP[nIndex].m_dAnyValue = dMaxScore;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      패턴 매칭 검사      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

BOOL CModuleLink::InspectEtc()
{
	long i, nType;
	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (nType==INSP_GRAY_VALUE))
		    bRet = InspectAverGrayValueROI(i);

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (nType==INSP_TAB_FOLDING))
		    bRet = InspectTabFoldingROI(i);
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         평균밝기         ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectAverGrayValueROI(long nIndex)
{
	BOOL bRet = FALSE;
	float dValue;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	// 위치보정
    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	// 위치보정

	dValue = m_libVis.GetAveBrightness(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth);

	if (dValue<m_pData->m_INSP[nIndex].m_dStandLowLimit || 
		dValue>m_pData->m_INSP[nIndex].m_dStandHighLimit) bRet=TRUE;

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         평균밝기         ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////       TAB FOLDING        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectTabFolding()
{
	long i, nType;
	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (nType==INSP_TAB_FOLDING))
		    bRet = InspectTabFoldingROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectTabFoldingROI(long nIndex)
{
	BOOL bPatRet, bRet=TRUE;
		

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////       TAB FOLDING        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


/*
BOOL CModuleLink::InspectEtcROI(long nIndex)
{
	BOOL bPatRet, bRet=TRUE;
	long nSideWidth, nSideHeight, x1, y1, x2, y2, nPatWidth, nPatHeight;
	long nW, nH, nMidX, nMidY;
	float a, b, fL, fH, fCx, fCy, dScaleX, dScaleY, fDist;
	float dPosX, dPosY, dScore1;
	LPBYTE pPAT;

	nPatWidth = m_pData->m_INSP[nIndex].m_nPatWidth;
	nPatHeight = m_pData->m_INSP[nIndex].m_nPatHeight;

	nSideWidth  = (long)(nPatWidth/2 * 150/100.0);
	nSideHeight = (long)(nPatHeight/2* 150/100.0);
	pPAT = m_pData->m_INSP[nIndex].m_pPat[0];

	fL = m_pData->m_INSP[nIndex].m_dStandLowLimit;
	fH = m_pData->m_INSP[nIndex].m_dStandHighLimit;
	fCx = m_pBack->m_GOM[nIndex].m_dCX;
	fCy = m_pBack->m_GOM[nIndex].m_dCY;

	x1 = (long)fCx-nSideWidth;
	y1 = (long)fCy-nSideHeight;
	x2 = (long)fCx+nSideWidth;
	y2 = (long)fCy+nSideHeight;
	if (x1<0) x1 = 0;
	if (x2>=m_nImageWidth) x2=m_nImageWidth-1;
	if (y1<0) y1 = 0;
	if (y2>=m_nImageHeight) y2=m_nImageHeight-1;

	nW = m_nImageWidth/2;
	nH = m_nImageHeight/2;
	nMidX = (long)(x1 + x2)/2;
	nMidY = (long)(y1 + y2)/2;

	if (nMidX<nW)
	{
		dScaleY = m_pCAL->m_dScaleL_Y;
		dScaleX = m_pCAL->m_dScaleLT_X;
		if (nMidY>nH)   
			dScaleX = m_pCAL->m_dScaleLB_X;
	}
	else
	{
		dScaleY = m_pCAL->m_dScaleR_Y;
		dScaleX = m_pCAL->m_dScaleRT_X;
		if (nMidY>nH)   
			dScaleX = m_pCAL->m_dScaleRB_X;
	}

	fDist = 999.0f;
	if (pPAT && nPatWidth>0 && nPatHeight>0)
	{
		bPatRet = NPatternDll_FindPattern(m_pRawImage, x1, y1, x2, y2,
											m_nImageWidth, m_nImageHeight, pPAT, nPatWidth, nPatHeight, 40, &dPosX, &dPosY, &dScore1);
		if (!bPatRet && dScore1>50)
		{
			m_pData->m_INSP[nIndex].m_dCX = dPosX;
			m_pData->m_INSP[nIndex].m_dCY = dPosY;

			a = (fCx - dPosX) * dScaleX;
			b = (fCy - dPosY) * dScaleY;
			fDist = sqrt(a*a + b*b);

			m_pData->m_INSP[nIndex].m_dCX = dPosX;
			m_pData->m_INSP[nIndex].m_dCY = dPosY;
		}
	}
	
	if (fL<=fDist && fDist<=fH)
		bRet = FALSE;

	m_pData->m_INSP[nIndex].m_dAnyValue = fDist;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;

	return bRet;
}
*/
	/*
	BOOL   bPatRet, bRet=TRUE;
	LPBYTE pPAT, pHalfPAT, pHalfImage;
	long   i, j, nPatType, nWidth, nHeight, nCutOffH, nCutOffV;
	long nCntX, nCntY;
	float  dPosX, dPosY, dSensity;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CRect rcRectH, rcRectV;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	// 위치보정
    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	// 위치보정

	dSensity = (float)m_pData->m_INSP[nIndex].m_dSensity;
	nPatType = m_pData->m_INSP[nIndex].m_nBlobOption;
	nWidth = m_pData->m_INSP[nIndex].m_nPatWidth;
	nHeight = m_pData->m_INSP[nIndex].m_nPatHeight;
	nCutOffH = nWidth/2 * 0.3f;
	nCutOffV = nHeight/2 * 0.3f;

	dPosX = (ptInsp1.x+ptInsp4.x)/2;
	dPosY = (ptInsp1.y+ptInsp4.y)/2;
	
	if (nPatType==0) // Left-Top
	{
		rcRectH.left = (long)dPosX + nCutOffH;
		rcRectH.right = (long)ptInsp2.x;
		rcRectH.top = (long)dPosY - nCutOffH;
		rcRectH.bottom = (long)dPosY + nCutOffH;

		rcRectV.left = (long)dPosX - nCutOffV;
		rcRectV.right = (long)dPosX + nCutOffV;
		rcRectV.top = (long)dPosY + nCutOffV;
		rcRectV.bottom = (long)ptInsp4.y;
	}
	if (nPatType==2) // Left-Bottom
	{
		rcRectH.left = (long)dPosX + nCutOffH;
		rcRectH.right = (long)ptInsp2.x;
		rcRectH.top = (long)dPosY - nCutOffH;
		rcRectH.bottom = (long)dPosY + nCutOffH;

		rcRectV.left = (long)dPosX - nCutOffV;
		rcRectV.right = (long)dPosX + nCutOffV;
		rcRectV.top = (long)ptInsp1.y;
		rcRectV.bottom = (long)dPosY - nCutOffV;
	}
	if (nPatType==3) // Right-Top
	{
		rcRectH.left = (long)ptInsp1.x;
		rcRectH.right = (long)dPosX - nCutOffH;
		rcRectH.top = (long)dPosY - nCutOffH;
		rcRectH.bottom = (long)dPosY + nCutOffH;

		rcRectV.left = (long)dPosX - nCutOffV;
		rcRectV.right = (long)dPosX + nCutOffV;
		rcRectV.top = (long)dPosY + nCutOffV;
		rcRectV.bottom = (long)ptInsp4.y;
	}
	if (nPatType==4) // Right-Bottom
	{
		rcRectH.left = (long)ptInsp1.x;
		rcRectH.right = (long)dPosX - nCutOffH;
		rcRectH.top = (long)dPosY - nCutOffH;
		rcRectH.bottom = (long)dPosY + nCutOffH;

		rcRectV.left = (long)dPosX - nCutOffV;
		rcRectV.right = (long)dPosX + nCutOffV;
		rcRectV.top = (long)ptInsp1.y;
		rcRectV.bottom = (long)dPosY - nCutOffV;
	}

	if (rcRectH.left<0) rcRectH.left = 2;
	if (rcRectH.right>m_nImageWidth-2) rcRectH.right = m_nImageWidth-2;		
	if (rcRectH.top<0) rcRectH.top = 2;
	if (rcRectH.bottom>m_nImageHeight-2) rcRectH.bottom = m_nImageHeight-2;

	if (rcRectV.left<0) rcRectV.left = 2;
	if (rcRectV.right>m_nImageWidth-2) rcRectV.right = m_nImageWidth-2;		
	if (rcRectV.top<0) rcRectH.top = 2;
	if (rcRectV.bottom>m_nImageHeight-2) rcRectV.bottom = m_nImageHeight-2;

	nTempH = rcRectH.right-rcRectH.left+1;
	nTempV = rcRectV.bottom-rcRectV.top+1;
	if (nTempH<2 || nTempV<2) 
		goto goto_end;

	pDataVertX = new float[nTempV];
	pDataVertY = new float[nTempV];

	pDataHoriX = new float[nTempH];
	pDataHoriY = new float[nTempH];

	pDataG = new unsigned char[m_nImageHeight];

	nVertCnt = 0;
	for(i=rcRectV.top; i<rcRectV.bottom; i++)
	{
		nMax = 0;
		nPos = rcRectV.left;
		for(j=rcRectV.left+1; j<rcRectV.right-1; j++)
		{
			pDataG[j] = m_pRawImage[i*m_nImageWidth + j];
			nTempV = abs(m_pRawImage[i*m_nImageWidth + (j-1)] - m_pRawImage[i*m_nImageWidth + (j+1)]);
			if (nTempV>nMax)
			{
				nPos = j;
				nMax = nTempV;
			}
		}

		fSubPixelP = 0;
		a = (float)fabs((double)(pDataG[nPos+2]+pDataG[nPos-2]-2*pDataG[nPos]));
    	b = (float)fabs((double)(pDataG[nPos+3]+pDataG[nPos-1]-2*pDataG[nPos+1]));

		if ((a+b) != 0) fSubPixelP = (float)nPos + a/(a+b);	
		else			fSubPixelP = (float)nPos;

		if (fSubPixelP>0)
		{
			pDataVertX[nVertCnt] = (float)fSubPixelP;
			pDataVertY[nVertCnt] = (float)i;
			nVertCnt++;

			m_pData->m_Scan[nIndex].m_dSecondPX[nCnt] = (float)fSubPixelP;
			m_pData->m_Scan[nIndex].m_dSecondPY[nCnt] = (float)i;
			nCnt++;
		}
	}

	if (nVertCnt>10)
	{
		m_libVis.LineFitting(nVertCnt, pDataVertY, pDataVertX, (int)(nVertCnt*0.1), 2, &t, &dA, &dB);

		pt1.x = dA*ptInsp1.y + dB;
		pt1.y = ptInsp1.y;
		pt2.x = dA*ptInsp3.y + dB;
		pt2.y = ptInsp3.y;
	}

	nHoriCnt = 0;
	for(i=rcRectH.left; i<rcRectH.right; i++)
	{
		nMax = 0;
		nPos = rcRectH.top;
		for(j=rcRectH.top+1; j<rcRectH.bottom-1; j++)
		{
			pDataG[j] = m_pRawImage[j*m_nImageWidth + i];
			nTempH = abs(m_pRawImage[(j-1)*m_nImageWidth + i] - m_pRawImage[(j+1)*m_nImageWidth + i]);
			if (nTempH>nMax)
			{
				nPos = j;
				nMax = nTempH;
			}
		}

		fSubPixelP = 0;
		a = (float)fabs((double)(pDataG[nPos+2]+pDataG[nPos-2]-2*pDataG[nPos]));
    	b = (float)fabs((double)(pDataG[nPos+3]+pDataG[nPos-1]-2*pDataG[nPos+1]));

		if ((a+b) != 0) fSubPixelP = (float)nPos + a/(a+b);	
		else			fSubPixelP = (float)nPos;

		if (fSubPixelP>0)
		{
			pDataHoriX[nHoriCnt] = (float)i;
			pDataHoriY[nHoriCnt] = (float)fSubPixelP;
			nHoriCnt++;

			m_pData->m_Scan[nIndex].m_dSecondPX[nCnt] = (float)i;
			m_pData->m_Scan[nIndex].m_dSecondPY[nCnt] = (float)fSubPixelP;
			nCnt++;
		}
	}

	if (nHoriCnt>10)
	{	
		m_libVis.LineFitting(nHoriCnt, pDataHoriX, pDataHoriY, (int)(nHoriCnt*0.1), 2, &t, &dA, &dB);

		pt3.y = dA*ptInsp1.x + dB;
		pt3.x = ptInsp1.x;
		pt4.y = dA*ptInsp2.x + dB;
		pt4.x = ptInsp2.x;
	}

	m_libMath.GetCrossPoint2Line(pt1, pt2, pt3, pt4, &ptCross);
	m_pData->m_INSP[nIndex].m_dCX = ptCross.x;
	m_pData->m_INSP[nIndex].m_dCY = ptCross.y;
		
	delete pDataG;
	delete pDataVertX;
	delete pDataVertY;
	delete pDataHoriX;
	delete pDataHoriY;
	}

	m_pData->m_INSP[nIndex].m_dStandLowLimit = dPatScore;
	m_pData->m_INSP[nIndex].m_dStandHighLimit = 100.0f;
	m_pData->m_INSP[nIndex].m_dAnyValue = dMaxScore;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;
	*/
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         폴딩  검사        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         결과 판정        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
long CModuleLink::GetIndexTitleROI(LPCTSTR strTitle)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, strTitle) || !m_pData->m_INSP[i].m_bLocROI) continue;

		nPos = i;
		break;
	}

	return nPos;
}

long CModuleLink::GetIndexROI(BOOL bLoc, LPCTSTR strTitle)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, strTitle) || !bLoc) continue;

		nPos = i;
		break;
	}

	return nPos;
}

long CModuleLink::GetIndexTitleLinkROI(LPCTSTR strTitle)
{
	long i, nPos, nSel, nNum, nLocROI;

	nPos = -1;
	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		if (nSel>=0)
		{
			nLocROI = m_pData->m_INSP[nSel].m_bLocROI;
		    if (_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, strTitle)	|| 
				!m_pData->m_INSP[nSel].m_bLocROI) continue;

			nPos = i;
		    break;
		}
	}

	return nPos;
}

void CModuleLink::FineLaminateCal()
{
	long i, nROI, nNum, nSel, nLocROI;
	float fPrevV, fNextV, fDiff, fTemp, fL, fH, fMidL, fMidH, fRate;
	long nCnt;
	CString str, strTitle;

	fRate = 0.15f;

	nCnt = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (m_pData->m_INSP[i].m_bDecision) nCnt++;
	}

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && !m_pData->m_INSP[i].m_bLocROI) 
		{
			if (m_pData->m_INSP[i].m_nInspectType == INSP_MEASURE_SIDE_ONE || 
				m_pData->m_INSP[i].m_nInspectType == INSP_CAMERA_CAL)
				continue;

			fL = m_pData->m_INSP[i].m_dStandLowLimit;
			fH = m_pData->m_INSP[i].m_dStandHighLimit;
			fMidL = (fL+fH)/2.0f - 0.03f;
			fMidH = (fL+fH)/2.0f + 0.03f;

			nROI = GetIndexTitleROI(m_pData->m_GOM[i].m_strTitle);
			if (nROI>=0)
			{
				////////////////////////// Fine ALG. 추가 
				fPrevV = m_pResB->m_dValue[nROI];
				fNextV = m_pData->m_INSP[i].m_dAnyValue;

//				fPrevV = m_pData->m_INSP[i].m_dAnyValue;
//				fNextV = m_pData->m_INSP[nROI].m_dAnyValue;
				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;
								
				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
					 
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;

						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}

				if (fNextV<fMidL) fNextV += 0.02f;
				if (fNextV>fMidH) fNextV -= 0.02f;

				m_pData->m_INSP[i].m_dAnyValue = fNextV;
				m_pData->m_INSP[i].m_bDecision = FALSE;
//				m_pData->m_INSP[nROI].m_dAnyValue = fNextV;
//				m_pData->m_INSP[nROI].m_bDecision = FALSE;
				////////////////////////// Fine ALG. 추가 
				
				fPrevV = m_pData->m_INSP[i].m_dAnyValue;
				fNextV = m_pData->m_INSP[nROI].m_dAnyValue;
				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;

				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
					 
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;

						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}

				if (fNextV<fMidL) fNextV += 0.02f;
				if (fNextV>fMidH) fNextV -= 0.02f;

//				m_pData->m_INSP[i].m_dAnyValue = fNextV;
//				m_pData->m_INSP[i].m_bDecision = FALSE;
				m_pData->m_INSP[nROI].m_dAnyValue = fNextV;
				m_pData->m_INSP[nROI].m_bDecision = FALSE;
			}
		}
	}

	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		if (nSel>=0 && nSel<GOM_MAX_NUM)
		{
			nLocROI = m_pData->m_INSP[nSel].m_bLocROI;
		    if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T("")) || nLocROI) continue;

			str.Format(_T("%s"), m_pData->m_Teach.m_LINK[i].m_strTitle);
			nROI = GetIndexTitleLinkROI(str);

			if (nROI>=0 && nROI<LINK_MAX_NUM)
			{
				fL = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
				fH = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;
				fMidL = (fL+fH)/2.0f - 0.03f;
				fMidH = (fL+fH)/2.0f + 0.03f;

				// STC, STA Fine ALG. 추가
				fPrevV = m_pResB->m_dLinkValue[nROI];
				fNextV = m_pData->m_Teach.m_LINK[i].m_dValue;
//				fPrevV = m_pData->m_Teach.m_LINK[i].m_dValue;
//				fNextV = m_pData->m_Teach.m_LINK[nROI].m_dValue;

				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;

				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV-fTemp;
						fNextV = fNextV+fTemp;
					}
				}
				
				if (fNextV<fMidL) fNextV += 0.02f;
				if (fNextV>fMidH) fNextV -= 0.02f;

				m_pData->m_Teach.m_LINK[i].m_dValue = fNextV;
				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
//				m_pData->m_Teach.m_LINK[nROI].m_dValue = fNextV;
//				m_pData->m_Teach.m_LINK[nROI].m_bDecision = FALSE;
				// STC, STA Fine ALG. 추가

				fPrevV = m_pData->m_Teach.m_LINK[i].m_dValue;
				fNextV = m_pData->m_Teach.m_LINK[nROI].m_dValue;
				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;

				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;
				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV-fTemp;
						fNextV = fNextV+fTemp;
					}
				}
				
				if (fNextV<fMidL) fNextV += 0.02f;
				if (fNextV>fMidH) fNextV -= 0.02f;

//				m_pData->m_Teach.m_LINK[i].m_dValue = fPrevV;
//				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
				m_pData->m_Teach.m_LINK[nROI].m_dValue = fNextV;
				m_pData->m_Teach.m_LINK[nROI].m_bDecision = FALSE;
			}
		}
	}
}

void CModuleLink::FineLaminateOneCell()
{
	long i, nNum, nSel1, nSel2, nLocROI;
	float fPrevV, fNextV, fDiff, fTemp, fL, fMidL, fH;
	long nType;
	CString str, strTitle;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		fPrevV = 0.0f;
		fNextV = 0.0f;

		str = m_pData->m_GOM[i].m_strTitle;
		strTitle = WithoutSpaceToString(str);
		if (_tcscmp(strTitle, _T(""))) 
		{
			nType = m_pData->m_INSP[i].m_nInspectType;
			if (nType == INSP_MEASURE_SIDE_ONE || 
				nType == INSP_SLINE ||
				nType == INSP_DATUM_B || 
				nType == INSP_DATUM_C) continue;

			fL = m_pData->m_INSP[i].m_dStandLowLimit;
			fH = m_pData->m_INSP[i].m_dStandHighLimit;
			fMidL = (fL+fH)/2.0f;

			fPrevV = m_pResB->m_dValue[i];
			fNextV = m_pData->m_INSP[i].m_dAnyValue;
			if (fPrevV>=fL && fPrevV<=fH && fNextV>=fL && fNextV<=fH)
			{
				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*0.10f;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
						fTemp = fDiff*0.02f;
					 
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
						fTemp = fDiff*0.02f;
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}

				if (fNextV<fMidL) fNextV += DEF_TURNING_VALUE;
				if (fNextV>fMidL) fNextV -= DEF_TURNING_VALUE;

				m_pData->m_INSP[i].m_dAnyValue = fNextV;
			}
		}
	}

	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel1 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		nSel2 = m_pData->m_Teach.m_LINK[i].m_nIndex2;
		if (nSel1>=0 && nSel2>=0 && nSel1<GOM_MAX_NUM && nSel2<GOM_MAX_NUM)
		{
			nLocROI = m_pData->m_INSP[nSel1].m_bLocROI;
			str.Format(_T("%s"), m_pData->m_Teach.m_LINK[i].m_strTitle);
			strTitle = WithoutSpaceToString(str);

		    if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T(""))) continue;

			fL = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
			fH = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;
			fMidL = (fL+fH)/2.0f;
			nType = m_pData->m_Teach.m_LINK[i].m_nLinkType;

			if (nType == TEACH_DIFF_GAP2GAP)
			{
				fDiff = m_pData->m_INSP[nSel1].m_dAnyValue - m_pData->m_INSP[nSel2].m_dAnyValue;
				m_pData->m_Teach.m_LINK[i].m_dValue = fDiff;

				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
				if (fDiff<fL || fH<fDiff)
				{
					m_pData->m_INSP[nSel1].m_bDecision = TRUE;
					m_pData->m_INSP[nSel2].m_bDecision = TRUE;
					m_pData->m_Teach.m_LINK[i].m_bDecision = TRUE;
				}
			}
			else
			{
				fPrevV = m_pResB->m_dLinkValue[i];
				fNextV = m_pData->m_Teach.m_LINK[i].m_dValue;
				fMidL = (fL+fH)/2.0f;

				if (fPrevV>=fL && fPrevV<=fH && fNextV>=fL && fNextV<=fH)
				{
					fDiff = (float)fabs(fNextV - fPrevV);
					fTemp = fDiff*0.15f;
					if (fPrevV>fNextV)
					{
						fPrevV = fPrevV-fTemp;
						fNextV = fNextV+fTemp;
						if (fPrevV<fNextV)
						{
	        				fDiff = (float)fabs(fNextV - fPrevV);
							fTemp = fDiff*0.02f;
							fPrevV = fPrevV+fTemp;
							fNextV = fNextV-fTemp;
						}
					}
					else
					{
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
						if (fPrevV>fNextV)
						{
	        				fDiff = (float)fabs(fNextV - fPrevV);
							fTemp = fDiff*0.02f;
							fPrevV = fPrevV-fTemp;
							fNextV = fNextV+fTemp;
						}
					}

					if (fNextV<fMidL) fNextV += DEF_TURNING_VALUE;
					if (fNextV>fMidL) fNextV -= DEF_TURNING_VALUE;

					m_pData->m_Teach.m_LINK[i].m_dValue = fNextV;
				}
			}
		}
	}
}

void CModuleLink::CalcOffsetData()
{
	long i, j, nNum, nSel1, nSel2, nType;
	float fL, fH;
	CString str, strTitle;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		str = m_pData->m_GOM[i].m_strTitle;
		strTitle = WithoutSpaceToString(str);
		if (_tcscmp(strTitle, _T(""))) 
		{
			nType = m_pData->m_INSP[i].m_nInspectType;
			if (nType == INSP_MEASURE_SIDE_ONE) 
				continue;

			for(j=0; j<m_nOffsetMaxNum; j++)
			{
				if (!_tcscmp(strTitle, m_strOffsetT[j]))
				{
					m_pData->m_INSP[i].m_bDecision = FALSE;
					fL = m_pData->m_INSP[i].m_dStandLowLimit;
					fH = m_pData->m_INSP[i].m_dStandHighLimit;

					m_pData->m_INSP[i].m_dAnyValue += m_fOffsetD[j];
					if (m_pData->m_INSP[i].m_dAnyValue<fL || fH<m_pData->m_INSP[i].m_dAnyValue)
						m_pData->m_INSP[i].m_bDecision = TRUE;

					if (m_pData->m_INSP[i].m_dAnyValue<0.0f) 
						m_pData->m_INSP[i].m_dAnyValue=0.0f;

					break;
				}
			}
		}
	}

	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel1 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		nSel2 = m_pData->m_Teach.m_LINK[i].m_nIndex2;
		if (nSel1>=0 && nSel1<GOM_MAX_NUM && nSel2>=0 && nSel2<=GOM_MAX_NUM)
		{
			str.Format(_T("%s"), m_pData->m_Teach.m_LINK[i].m_strTitle);
			strTitle = WithoutSpaceToString(str);

		    if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T(""))) continue;

			for(j=0; j<m_nOffsetMaxNum; j++)
			{
				if (!_tcscmp(strTitle, m_strOffsetT[j]))
				{
					m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
					fL = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
					fH = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;

					m_pData->m_Teach.m_LINK[i].m_dValue += m_fOffsetD[j];
					if (m_pData->m_Teach.m_LINK[i].m_dValue<fL || fH<m_pData->m_Teach.m_LINK[i].m_dValue)
						m_pData->m_Teach.m_LINK[i].m_bDecision = TRUE;

					if (m_pData->m_Teach.m_LINK[i].m_dValue<0.0f) 
						m_pData->m_Teach.m_LINK[i].m_dValue=0.0f;

					break;
				}
			}
		}
	}
}

long CModuleLink::ProcessResultDecision()
{
	BOOL bRet, bRet1, bRet2;
	long i, nSel;
	float fMidSpec, fTempP, dScale, fTempX, fTempY;

	m_pCAL->m_dScaleX = (m_pCAL->m_dScaleLT_X + m_pCAL->m_dScaleLB_X)/2.0f;
	m_pCAL->m_dScaleY = m_pCAL->m_dScaleL_Y;

	/*
	m_pResData->m_dSourX =  m_pData->m_dSourX * m_pCAL->m_dScaleX;
    m_pResData->m_dSourY =  m_pData->m_dSourY * m_pCAL->m_dScaleY;
	m_pResData->m_dSourT =  m_pData->m_dSourT;

	m_pResData->m_dCX = m_fMotionDx * m_pCAL->m_dScaleX;
	m_pResData->m_dCY = m_fMotionDy * m_pCAL->m_dScaleY;
	m_pResData->m_dAngleT = m_fMotionDt;		
	*/

	m_pResData->m_dSourX = (m_pData->m_dSourX - m_nImageWidth/2.0f) * m_pCAL->m_dScaleX;
    m_pResData->m_dSourY = (m_nImageHeight/2.0f - m_pData->m_dSourY) * m_pCAL->m_dScaleY;
	m_pResData->m_dSourT =  m_pData->m_dSourT;

	m_pResData->m_dCX = (m_fMotionDx-m_nImageWidth/2.0f) * m_pCAL->m_dScaleX;
	m_pResData->m_dCY = (m_nImageHeight/2.0f - m_fMotionDy) * m_pCAL->m_dScaleY;
	m_pResData->m_dCT = m_fMotionDt;

	m_pResData->m_dPixelSX = m_pData->m_dSourX;
	m_pResData->m_dPixelSY = m_pData->m_dSourY;
	m_pResData->m_dPixelCX = m_fMotionDx;
	m_pResData->m_dPixelCY = m_fMotionDy;

	fTempX = m_pData->m_Teach.m_fOffsetX;
	fTempY = m_pData->m_Teach.m_fOffsetY;

    dScale = m_pCAL->m_dScaleL_Y;
	fTempP = fabs(m_pData->m_nImageHeight-m_fObjectEdgeY)*dScale;

    m_pResData->m_dOffsetEPC = 0.0f;
    if (m_fObjectEdgeY>0.0f) 
	    m_pResData->m_dOffsetEPC = fTempP;
	
	bRet = FALSE;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(m_pResData->m_strTitle[i], _T(""));
		m_pResData->m_bDecisionROI[i] = 0;
        m_pResData->m_nInspType[i] = 0;
		m_pResData->m_nPlotNum[i] = 0;

		if (!_tcscmp(m_pData->m_GOM[i].m_strTitle, _T(""))) continue;
       	if (m_pData->m_INSP[i].m_nInspectType == INSP_DATUM_B || 
			m_pData->m_INSP[i].m_nInspectType == INSP_DATUM_C) continue;

		_tcscpy_s(m_pResData->m_strTitle[i], m_pData->m_GOM[i].m_strTitle);
		m_pResData->m_bLocROI[i] = m_pData->m_INSP[i].m_bLocROI;
		m_pResData->m_bEjectROI[i] = m_pData->m_GOM[i].m_bEjectROI;
		m_pResData->m_bExecROI[i] = m_pData->m_GOM[i].m_bExecShowROI;
		m_pResData->m_nInspType[i] = m_pData->m_INSP[i].m_nInspectType;
		m_pResData->m_nPlotNum[i] = m_pData->m_INSP[i].m_nPlotSelNum;
		m_pResData->m_dValue[i] = m_pData->m_INSP[i].m_dAnyValue;

		fMidSpec = (m_pData->m_INSP[i].m_dStandLowLimit + m_pData->m_INSP[i].m_dStandHighLimit) / 2.0f;
		m_pResData->m_dRefDiff[i] = fMidSpec - m_pData->m_INSP[i].m_dAnyValue;
		m_pResData->m_dLowLimit[i] = m_pData->m_INSP[i].m_dStandLowLimit;
		m_pResData->m_dStdSpec[i] = m_pData->m_INSP[i].m_dStdSpec;
		m_pResData->m_dToleSpec[i] = m_pData->m_INSP[i].m_dToleSpec;
		m_pResData->m_dHighLimit[i] = m_pData->m_INSP[i].m_dStandHighLimit;
		m_pResData->m_bDecisionROI[i] = m_pData->m_INSP[i].m_bDecision;

		if (m_pData->m_INSP[i].m_nInspectType==INSP_MEASURE_SIDE_ONE)
		{
			if (!m_pResData->m_bLocROI[i] && m_pResData->m_bDecisionROI[i])
			{
				m_pResData->m_bFDecision = TRUE;
				bRet = TRUE;
			}
		}
		else
		{
			if (m_pResData->m_bExecROI[i])
			{
				if (!m_pResData->m_bLocROI[i] && m_pResData->m_bDecisionROI[i])
				{
					m_pResData->m_bFDataDec = TRUE;
					bRet = TRUE;
				}
			}
		}
	}

	m_pResData->m_nLinkNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<m_pData->m_Teach.m_nLinkNum; i++)
	{
		_tcscpy_s(m_pResData->m_strLinkTitle[i], _T(""));
		m_pResData->m_nLinkPlotPos[i] = 0;
		m_pResData->m_nLinkPlotNum[i] = 0;

		bRet1 = m_pData->m_Teach.m_LINK[i].m_bSel1;
		bRet2 = m_pData->m_Teach.m_LINK[i].m_bSel2;

		if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T(""))) continue;
		if (!bRet1 || !bRet2) continue;
		if (m_pData->m_Teach.m_LINK[i].m_nLinkType==TEACH_CROSS_POINT) continue;

		nSel = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		if (nSel>=0 && nSel<GOM_MAX_NUM)
		{
			_tcscpy_s(m_pResData->m_strLinkTitle[i], m_pData->m_Teach.m_LINK[i].m_strTitle);
			m_pResData->m_nLinkType[i] = m_pData->m_Teach.m_LINK[i].m_nLinkType;
			m_pResData->m_bLinkExecROI[i] = m_pData->m_Teach.m_LINK[i].m_bLinkExec;
			m_pResData->m_bLinkEjectROI[i] = m_pData->m_Teach.m_LINK[i].m_bEjectROI;
			m_pResData->m_bLinkLocROI[i] = m_pData->m_INSP[nSel].m_bLocROI;
			m_pResData->m_nLinkPlotPos[i] = m_pData->m_Teach.m_LINK[i].m_nPlotPos;
			m_pResData->m_nLinkPlotNum[i] = m_pData->m_Teach.m_LINK[i].m_nPlotSelNum;
			m_pResData->m_bLinkDecROI[i] = m_pData->m_Teach.m_LINK[i].m_bDecision;
			m_pResData->m_dLinkValue[i] = m_pData->m_Teach.m_LINK[i].m_dValue;

			fMidSpec = (m_pData->m_Teach.m_LINK[i].m_dStandLowLimit + m_pData->m_Teach.m_LINK[i].m_dStandHighLimit) / 2.0f;
			m_pResData->m_dLinkRefDiff[i] = fMidSpec - m_pData->m_Teach.m_LINK[i].m_dValue;
			m_pResData->m_dLinkValueS[i] = m_pData->m_Teach.m_LINK[i].m_dValueS;
			m_pResData->m_dLinkLowLimit[i] = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
			m_pResData->m_dLinkStdSpec[i] = m_pData->m_Teach.m_LINK[i].m_dStdSpec;
			m_pResData->m_dLinkToleSpec[i] = m_pData->m_Teach.m_LINK[i].m_dToleSpec;
			m_pResData->m_dLinkHighLimit[i] = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;

			if (!m_pResData->m_bLinkLocROI[i] && m_pResData->m_bLinkDecROI[i])
			{
				m_pResData->m_bFDataDec = TRUE;
				m_pResData->m_bFDecision = TRUE;
				bRet = TRUE;
			}	
		}
	}

	m_pResData->m_bDecision = bRet;
	m_pData->m_bDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         결과 판정        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



