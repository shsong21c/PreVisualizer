// DlgBadImage.cpp : implementation file
//

#include "stdafx.h"
#include "VLaminateApp.h"
#include "DlgBadImage.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CDlgBadImage dialog
extern CMainFrame *m_pMainFrm;

IMPLEMENT_DYNAMIC(CDlgBadImage, CDialogEx)

CDlgBadImage::CDlgBadImage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBadImage::IDD, pParent)
{
	long i;

	for(i=0; i<GOM_MAX_NUM; i++)
		 wcscpy_s(m_pSelRes.m_strTitle[i], _T(""));

	m_nCamID = 0;
	m_bSaveStart = FALSE;
	m_strImageFileName = _T("");	
}

CDlgBadImage::~CDlgBadImage()
{
	delete m_dispDib;

	m_memDC.DeleteDC();
	m_bmpBuffer.DeleteObject();
}

void CDlgBadImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgBadImage, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDlgBadImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_dispDib = new CDib(SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, 8);
	m_memDC.CreateCompatibleDC(NULL);
	m_bmpBuffer.CreateCompatibleBitmap(GetDC(), SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT); 
	(CBitmap*)m_memDC.SelectObject(&m_bmpBuffer); 

	SetWindowPos(NULL, 0, 0, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgBadImage::OnPaint()
{
	CPaintDC dc(this); 

	InvalidateView();
}

void CDlgBadImage::InvalidateView()
{
	CDC *pDC = GetDC();

	if (m_memDC.m_hDC && m_dispDib)
	{
		//{BEGIN : 화면에 영상그리기 >>>>>>>>>>>>>>>>>>>>>>>>
		CSize sizeFileDib = m_dispDib->GetDimensions();
		m_dispDib->Draw(&m_memDC, CPoint(0,0), sizeFileDib);
		//}END : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		InvalidateDrawInspItem(&m_memDC);
    	pDC->BitBlt(0, 0, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, &m_memDC, 0, 0,SRCCOPY); 	

		if (m_bSaveStart)
		{
			m_dispDib->MemoryDC2BMP(m_memDC.m_hDC, m_bmpBuffer, m_strImageFileName);
			m_bSaveStart = FALSE;
		}
	}

	ReleaseDC(pDC);
}


void CDlgBadImage::InvalidateDrawInspItem(CDC *pDC)
{
	long i;
	long  nNum, nFrontItemCnt, nRearItemCnt;
	float dLowLimit, dHighLimit, dValue, nWidth, nHeight;
	float dRatioX, dRatioY, dRate;
	CString szbuf, strTitle;
	CRect rcRect;
	CFont *pNewFont, *pOldFont;
	CPen   *pNewPen, *pOldPen;
	COLORREF nColor;
	
	pNewFont = new CFont;
	pNewFont->CreateFont(15, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    								   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	
	if (m_nCamID==CAM_UP)
	{
		nWidth = UP_IMAGE_WIDTH;
		nHeight = UP_IMAGE_HEIGHT;
	}
	else
	{
		nWidth = DN_IMAGE_WIDTH;
		nHeight = DN_IMAGE_HEIGHT;
	}

	dRatioX = (float)SMALL_IMAGE_WIDTH/nWidth;
	dRatioY = (float)SMALL_IMAGE_HEIGHT/nHeight;

	if (dRatioX>dRatioY) dRate = dRatioY;
	else dRate = dRatioX;

	nFrontItemCnt = 0;
	nRearItemCnt = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		strTitle.Format(_T("%s"), m_pSelRes.m_strTitle[i]);
		if (!_tcscmp(strTitle, _T(""))) continue;
		
		if (m_pSelRes.m_bDecisionROI[i])
			nColor = RGB(255, 0, 0);
		else 
			nColor = RGB(0, 255, 0);

		pNewPen = new CPen(PS_SOLID, 1, nColor);
    	pOldPen= pDC->SelectObject(pNewPen);

		pDC->SetTextColor(nColor);
		pDC->SetBkMode(TRANSPARENT);

		dLowLimit = m_pSelRes.m_dLowLimit[i];
		dHighLimit = m_pSelRes.m_dHighLimit[i];
		dValue = m_pSelRes.m_dValue[i];
				
		if (!m_pSelRes.m_bLocROI[i])
		{
			szbuf.Format(_T("%s : [%3.2f %3.2f]=%3.2f"), strTitle, dLowLimit, dHighLimit, dValue);
		    pDC->TextOut(50, 50 + nFrontItemCnt*20, szbuf);
   		    
			szbuf.Format(_T("%s"), strTitle);
			pDC->TextOut((long)(m_pSelRes.m_dPosRoiX1[i]*dRate+2), (long)(m_pSelRes.m_dPosRoiY1[i]*dRate), szbuf);

			pDC->MoveTo((long)(m_pSelRes.m_dPosRoiX1[i]*dRate), (long)(m_pSelRes.m_dPosRoiY1[i]*dRate));
			pDC->LineTo((long)(m_pSelRes.m_dPosRoiX2[i]*dRate), (long)(m_pSelRes.m_dPosRoiY1[i]*dRate));
			pDC->LineTo((long)(m_pSelRes.m_dPosRoiX2[i]*dRate), (long)(m_pSelRes.m_dPosRoiY2[i]*dRate));
			pDC->LineTo((long)(m_pSelRes.m_dPosRoiX1[i]*dRate), (long)(m_pSelRes.m_dPosRoiY2[i]*dRate));
			pDC->LineTo((long)(m_pSelRes.m_dPosRoiX1[i]*dRate), (long)(m_pSelRes.m_dPosRoiY1[i]*dRate));

			nFrontItemCnt++;
		}

		pDC->SelectObject(pOldPen);
        delete pNewPen;
	}

	nNum = m_pSelRes.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		strTitle.Format(_T("%s"), m_pSelRes.m_strLinkTitle[i]);
		if (!_tcscmp(strTitle, _T(""))) continue;
		
		if (m_pSelRes.m_bLinkDecROI[i]) 
			nColor = RGB(255, 0, 0);
		else 
			nColor = RGB(0, 255, 0);

		pDC->SetTextColor(nColor);
		pDC->SetBkMode(TRANSPARENT);

		dLowLimit = m_pSelRes.m_dLinkLowLimit[i];
		dHighLimit = m_pSelRes.m_dLinkHighLimit[i];
		dValue = m_pSelRes.m_dLinkValue[i];
		
		szbuf.Format(_T("%s : [%3.2f %3.2f]=%3.2f"), strTitle, dLowLimit, dHighLimit, dValue);
		if (!m_pSelRes.m_bLinkLocROI[i])
		{
		    pDC->TextOut(50, 50 + nFrontItemCnt*20, szbuf);
   		    nFrontItemCnt++;
		}
	}

	if (m_pSelRes.m_bDecision)
		nColor = RGB(255, 0, 0);
	else 
		nColor = RGB(0, 255, 0);

	pNewPen = new CPen(PS_SOLID, 1, nColor);
    pOldPen= pDC->SelectObject(pNewPen);

	for(i=0; i<4; i++)
	{
		pDC->MoveTo((long)(m_pSelRes.m_dCorPx[i]*dRate-10), (long)(m_pSelRes.m_dCorPy[i]*dRate));
		pDC->LineTo((long)(m_pSelRes.m_dCorPx[i]*dRate+10), (long)(m_pSelRes.m_dCorPy[i]*dRate));
		pDC->MoveTo((long)(m_pSelRes.m_dCorPx[i]*dRate), (long)(m_pSelRes.m_dCorPy[i]*dRate-10));
		pDC->LineTo((long)(m_pSelRes.m_dCorPx[i]*dRate), (long)(m_pSelRes.m_dCorPy[i]*dRate+10));
	}

	pDC->MoveTo((long)(m_pSelRes.m_dCenPx*dRate-10), (long)(m_pSelRes.m_dCenPy*dRate));
	pDC->LineTo((long)(m_pSelRes.m_dCenPx*dRate+10), (long)(m_pSelRes.m_dCenPy*dRate));
	pDC->MoveTo((long)(m_pSelRes.m_dCenPx*dRate), (long)(m_pSelRes.m_dCenPy*dRate-10));
	pDC->LineTo((long)(m_pSelRes.m_dCenPx*dRate), (long)(m_pSelRes.m_dCenPy*dRate+10));

	pDC->SelectObject(pOldPen);
    delete pNewPen;

	pDC->SelectObject(pOldFont);
	delete pNewFont;
}

void CDlgBadImage::SetSaveImageFile(CString strPathName)
{
	m_bSaveStart = TRUE;
	m_strImageFileName = strPathName;
}

void CDlgBadImage::UpdateFrameBadImage(long nCAM, long nIndexPnt)
{
	long i, j, nSelPnt;
	LPBYTE fm;
	LPBYTE pImage;

	fm = m_dispDib->m_lpImage;
    if (fm)
	{
		nSelPnt = g_DataRT.m_nBadIndex[nCAM] - nIndexPnt - 1;
		if (nSelPnt<0)
			nSelPnt += DEF_QUEUE_MAX_NUM;

		memcpy(&m_pSelRes, &g_DataRT.m_BadRes[nCAM][nSelPnt], sizeof(INSP_RESULT_DATA));
		    
		pImage = g_pBadIMG[nCAM][nSelPnt];

		for(i=0; i<SMALL_IMAGE_HEIGHT; i++)
		for(j=0; j<SMALL_IMAGE_WIDTH; j++)
			fm[(SMALL_IMAGE_HEIGHT-i-1)*SMALL_IMAGE_WIDTH + j] = pImage[i*SMALL_IMAGE_WIDTH + j];
	}

	m_nCamID = nCAM;
	InvalidateView();
}
