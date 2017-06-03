// OverlayResultGOM.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "OverlayResultGOM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayResultGOM
COverlayResultGOM::COverlayResultGOM()
{
	m_RT.m_bPosCAL = FALSE;
	m_RT.m_bResultDec = FALSE;

	m_RT.m_ptPosCAL.x = 0.0f;
	m_RT.m_ptPosCAL.y = 0.0f;

	m_RT.m_ptRotAxis.x = IMAGE_WIDTH/2;
	m_RT.m_ptRotAxis.y = IMAGE_HEIGHT/2;
}

COverlayResultGOM::~COverlayResultGOM()
{
}

BEGIN_MESSAGE_MAP(COverlayResultGOM, CStatic)
	//{{AFX_MSG_MAP(COverlayResultGOM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayResultGOM message handlers
BOOL COverlayResultGOM::DrawOverlayResult(CDC *pDC, SETUP_MODEL_DATA *pData)
{
	BOOL bRes, bInspFlag;
	long i, j, nType, nDir, bLoc, nMidX, nMidY;
	long nInspItemCount, nCntLoc;
	float dLowLimit, dHighLimit, dValue, dScale;
	CString szbuf, strTitle, str;
	COLORREF nColor, nColorT;
	CRect rcRect;
	POINT2 ptCir, vtSt, vtEt, vtT;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CFont *pNewFont, *pOldFont;
	
	pNewFont = new CFont;
	pNewFont->CreateFont(15, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    								   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);

	rcRect = pData->m_IMG.m_fmRect;
	dScale = 1.0f/pData->m_IMG.m_fZoomScale;

	///////////////////////////////         가로 라인 긋기      /////////////////////////////////
	if (m_RT.m_bPosCAL)
	{
		nMidX = (long)((m_RT.m_ptRotAxis.x - rcRect.left)/dScale);
	    nMidY = (long)((m_RT.m_ptRotAxis.y - rcRect.top)/dScale);
	}
	else
	{
		nMidX = (long)((pData->m_nImageWidth/2 - rcRect.left)/dScale);
		nMidY = (long)((pData->m_nImageHeight/2 - rcRect.top)/dScale);
	}

	m_ImageBMP.DrawLine(pDC, 0, nMidY, FRAME_WIDTH, nMidY, 1, RGB(0, 0, 255));
	m_ImageBMP.DrawLine(pDC, nMidX, 0, nMidX, FRAME_HEIGHT, 1, RGB(0, 0, 255));

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      개별 항목 검사 표시     /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	
	bInspFlag = FALSE;
	nInspItemCount = 0;
	nCntLoc = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (!_tcscmp(pData->m_GOM[i].m_strTitle, _T("")) || !pData->m_INSP[i].m_bInspMode) continue;
		bInspFlag = TRUE;

 		m_pGOM = &pData->m_GOM[i];
		m_pSCAN = &pData->m_Scan[i];
		m_pINSP = &pData->m_INSP[i];
		bLoc = m_pINSP->m_bLocROI;

    	// 위치보정 
		m_FuncEtc.GetOffsetAdjustment(pData, i, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
		m_FuncEtc.GetTransPosision(pData->m_dSourX, pData->m_dSourY, pData->m_dDeltaX, pData->m_dDeltaY, pData->m_dDeltaT,
			                       m_pGOM->m_dCX, m_pGOM->m_dCY, &ptCir);
      	// 위치보정

		strTitle = pData->m_GOM[i].m_strTitle;
		nType = pData->m_INSP[i].m_nInspectType;
		nDir = pData->m_Scan[i].m_nSecondScanDirType;
		dValue = pData->m_INSP[i].m_dAnyValue;
		dLowLimit = pData->m_INSP[i].m_dStandLowLimit;
		dHighLimit = pData->m_INSP[i].m_dStandHighLimit;
		bRes = pData->m_INSP[i].m_bDecision;

		if (bRes) nColor = RGB(255, 0, 0);
		else
		{
			if (bLoc) nColor = RGB(0, 150, 255);
			else nColor = RGB(0, 255, 0); 
		}

		if (bLoc) nColorT = RGB(0, 150, 255);
		else nColorT = RGB(0, 255, 0); 

		pDC->SetTextColor(nColor);
		pDC->SetBkMode(TRANSPARENT);

		switch (nType)
		{
			case INSP_MEASURE_SIDE_ONE :
				if (m_pSCAN->m_nSecondNum>1)
				{
					for(j=0; j<m_pSCAN->m_nSecondNum; j++)
					{
						vtEt.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left-1)/dScale;
						vtEt.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top-1)/dScale;

						if (vtEt.x>0 || vtEt.y>0)
						{
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x-2, (long)vtEt.y-2, (long)vtEt.x+3, (long)vtEt.y+3, 1, nColor);
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x+2, (long)vtEt.y-2, (long)vtEt.x-3, (long)vtEt.y+3, 1, nColor);
						}
					}
				}
				else
				{
					vtSt.x = (m_pINSP->m_ptSecondMeas1.x - rcRect.left-1)/dScale;
					vtSt.y = (m_pINSP->m_ptSecondMeas1.y - rcRect.top-1)/dScale;
					vtEt.x = (m_pINSP->m_ptSecondMeas2.x - rcRect.left-1)/dScale;
					vtEt.y = (m_pINSP->m_ptSecondMeas2.y - rcRect.top-1)/dScale;

					if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
				}
				break;
			case INSP_MEASURE_SIDE_TWO :
				if (m_pSCAN->m_nSecondNum>1)
				{
					for(j=0; j<m_pSCAN->m_nSecondNum; j++)
					{
						vtSt.x = (m_pSCAN->m_dFirstPX[j] - rcRect.left-1)/dScale;
						vtSt.y = (m_pSCAN->m_dFirstPY[j] - rcRect.top-1)/dScale;
						vtEt.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left-1)/dScale;
						vtEt.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top-1)/dScale;

						if (vtSt.x>0 || vtSt.y>0)
						{
							m_ImageBMP.DrawLine(pDC, (long)vtSt.x-2, (long)vtSt.y-2, (long)vtSt.x+3, (long)vtSt.y+3, 1, nColor);
							m_ImageBMP.DrawLine(pDC, (long)vtSt.x+2, (long)vtSt.y-2, (long)vtSt.x-3, (long)vtSt.y+3, 1, nColor);
						}

						if (vtEt.x>0 || vtEt.y>0)
						{
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x-2, (long)vtEt.y-2, (long)vtEt.x+3, (long)vtEt.y+3, 1, nColor);
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x+2, (long)vtEt.y-2, (long)vtEt.x-3, (long)vtEt.y+3, 1, nColor);
						}
					}
				}
				else
				{
					vtSt.x = (m_pINSP->m_ptFirstMeas1.x - rcRect.left)/dScale;
					vtSt.y = (m_pINSP->m_ptFirstMeas1.y - rcRect.top)/dScale;
					vtEt.x = (m_pINSP->m_ptFirstMeas2.x - rcRect.left)/dScale;
					vtEt.y = (m_pINSP->m_ptFirstMeas2.y - rcRect.top)/dScale;

					if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);

					vtSt.x = (m_pINSP->m_ptSecondMeas1.x - rcRect.left-1)/dScale;
					vtSt.y = (m_pINSP->m_ptSecondMeas1.y - rcRect.top-1)/dScale;
					vtEt.x = (m_pINSP->m_ptSecondMeas2.x - rcRect.left-1)/dScale;
					vtEt.y = (m_pINSP->m_ptSecondMeas2.y - rcRect.top-1)/dScale;

					if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
				}
				break;
			case INSP_DATUM_B :
			case INSP_DATUM_C :
				vtSt.x = (m_pINSP->m_ptSecondMeas1.x - rcRect.left-1)/dScale;
				vtSt.y = (m_pINSP->m_ptSecondMeas1.y - rcRect.top-1)/dScale;
				vtEt.x = (m_pINSP->m_ptSecondMeas2.x - rcRect.left-1)/dScale;
				vtEt.y = (m_pINSP->m_ptSecondMeas2.y - rcRect.top-1)/dScale;
				if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
					m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
				break;
			case INSP_SLINE :
			case INSP_STRAIGHTNESS :
				for(j=0; j<m_pSCAN->m_nSecondNum; j++)
				{
					vtSt.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left-1)/dScale;
					vtSt.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top-1)/dScale;

					/*
					nColorD = nColorT;
					if (m_pSCAN->m_bSecondDec[j]) nColorD = nColor;
					*/

					if (vtSt.x>0 || vtSt.y>0)
					{
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x-2, (long)vtSt.y-2, (long)vtSt.x+3, (long)vtSt.y+3, 1, nColor);
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x+2, (long)vtSt.y-2, (long)vtSt.x-3, (long)vtSt.y+3, 1, nColor);
					}
				}
				break;
			case INSP_PATTERN_MATCH :
			case INSP_CAMERA_CAL :
				if (m_pINSP->m_dCX>0 && m_pINSP->m_dCY>0)
				{
					vtSt.x = (m_pINSP->m_dCX - rcRect.left)/dScale;
					vtSt.y = (m_pINSP->m_dCY - rcRect.top)/dScale;
					m_ImageBMP.DrawCross(pDC, (long)vtSt.x-1, (long)vtSt.y-1, 5,  1, nColor);
				}
				for(j=0; j<m_pSCAN->m_nSecondNum; j++)
				{
					vtT.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left)/dScale;
					vtT.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top)/dScale;
					m_ImageBMP.DrawPixel(pDC, (long)vtT.x-1, (long)vtT.y-1, 1, RGB(0, 0, 255));
				}
				break;
        }

		if (nType != INSP_MEASURE_SIDE_ONE)
		{
			vtT.x = (ptInsp4.x - rcRect.left)/dScale;
			vtT.y = (ptInsp4.y - rcRect.top)/dScale;

			str.Format(_T("%5.2f"), m_pINSP->m_dAnyValue);
			if (m_pINSP->m_bDecision) 
				m_ImageBMP.DrawText(pDC, (long)vtT.x, (long)vtT.y, str, 1, RGB(255, 0, 0));
			else
				m_ImageBMP.DrawText(pDC, (long)vtT.x, (long)vtT.y, str, 1, nColor);
		}

		/*
		if (pData->m_GOM[i].m_bExecShowROI)
		{
			szbuf.Format("%s : [%3.2f %3.2f]=%3.2f", strTitle, dLowLimit, dHighLimit, fabs(dValue));
			pDC->TextOut(5, 40 + nInspItemCount*16, szbuf);

    		nInspItemCount++;
		}
		*/

		if (!pData->m_bRealTimeMode)
		    pData->m_INSP[i].m_bInspMode = FALSE;
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      개별 항목 검사 표시     //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      LINK 항목 검사 표시     //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	long nCnt, nNum, nIndex1, nIndex2;

	nCnt = pData->m_Teach.m_nLinkNum;
	for(i=0; i<nCnt; i++)
	{
		if (!_tcscmp(pData->m_Teach.m_LINK[i].m_strTitle, _T("")))
			continue;

		nNum=0;
		if (pData->m_Teach.m_LINK[i].m_bSel1) nNum++;
		if (pData->m_Teach.m_LINK[i].m_bSel2) nNum++;

		if (nNum>=2) 
		{
			nType = pData->m_Teach.m_LINK[i].m_nLinkType;
			bRes = pData->m_Teach.m_LINK[i].m_bDecision;
			nIndex1 = pData->m_Teach.m_LINK[i].m_nIndex1;
			nIndex2 = pData->m_Teach.m_LINK[i].m_nIndex2;

			if (bRes) nColor = RGB(255, 0, 0);
			else      nColor = RGB(0, 255, 0);

			pDC->SetTextColor(nColor);
    		pDC->SetBkMode(TRANSPARENT);

			if (nType==TEACH_CROSS_POINT)
			{
				vtSt.x = pData->m_Teach.m_LINK[i].m_dCX;
				vtSt.y = pData->m_Teach.m_LINK[i].m_dCY;
				if (vtSt.x>0 && vtSt.y>0)
				{ 
					vtSt.x = (vtSt.x - rcRect.left)/dScale;
					vtSt.y = (vtSt.y - rcRect.top)/dScale;
					m_ImageBMP.DrawCross(pDC, (long)vtSt.x-1, (long)vtSt.y-1, 5,  1, nColor);
				}
			}
		}
	}
	
	if (m_RT.m_nAlignPosCnt>0)
	{
		for(i=0; i<m_RT.m_nAlignPosCnt; i++)
		{
			vtSt.x = (m_RT.m_ptAlignPos[i].x - rcRect.left-1)/dScale;
			vtSt.y = (m_RT.m_ptAlignPos[i].y - rcRect.top-1)/dScale;

			if (vtSt.x>0 || vtSt.y>0)
				m_ImageBMP.DrawCross(pDC, (long)vtSt.x, (long)vtSt.y, 10, 1, RGB(0, 255, 0));
		}

		vtSt.x = (m_RT.m_ptAlignCen.x - rcRect.left-1)/dScale;
		vtSt.y = (m_RT.m_ptAlignCen.y - rcRect.top-1)/dScale;
		m_ImageBMP.DrawCross(pDC, (long)vtSt.x, (long)vtSt.y, 10, 1, RGB(0, 255, 0));
	}

	if (!pData->m_bRealTimeMode)
	    m_RT.m_nAlignPosCnt = 0;
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      LINK 항목 검사 표시     //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	pDC->SelectObject(pOldFont);
	delete pNewFont;

	if (bInspFlag)
	{
		CFont *pNewFont, *pOldFont;
		pNewFont = new CFont;
		pNewFont->CreateFont(30, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
		pOldFont = pDC->SelectObject(pNewFont);
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetBkMode(TRANSPARENT);

		vtSt.x = (pData->m_nImageWidth*0.9f - rcRect.left)/dScale;
		vtSt.y = (pData->m_nImageHeight*0.08f - rcRect.top)/dScale;

		if (pData->m_bDecision || m_RT.m_bResultDec)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->TextOut((long)vtSt.x, (long)vtSt.y, _T("NG")); 
		}
		else
		{
			pDC->SetTextColor(RGB(0, 255, 0));
			pDC->TextOut((long)vtSt.x, (long)vtSt.y, _T("OK")); 
		}

        pDC->SelectObject(pOldFont); 
		delete pNewFont;
	}

	return TRUE;
}

