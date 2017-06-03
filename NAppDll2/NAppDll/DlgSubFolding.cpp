// DlgSubPattern.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVDisp.h"
#include "DlgSubFolding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern dialog
CDlgSubFolding::CDlgSubFolding(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubFolding::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubPattern)
	m_editArea = 0;
	m_editSlope = 0;
	m_editPatSizeX = 0;
	m_editPatSizeY = 0;
	//}}AFX_DATA_INIT

	m_nPage=0;
	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pData;
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pBackData;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
}

void CDlgSubFolding::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubPattern)
	DDX_Text(pDX, IDC_EDIT_AREA, m_editArea);
	DDX_Text(pDX, IDC_EDIT_SLOPE, m_editSlope);
	DDX_Text(pDX, IDC_EDIT_PAT_SIZE_X, m_editPatSizeX);
	DDX_Text(pDX, IDC_EDIT_PAT_SIZE_Y, m_editPatSizeY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSubFolding, CDialog)
	//{{AFX_MSG_MAP(CDlgSubPattern)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AREA, OnDeltaposSpinArea)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOPE, OnDeltaposSpinSlope)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PAT_SIZE_X, OnDeltaposSpinPatSizeX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PAT_SIZE_Y, OnDeltaposSpinPatSizeY)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_PAT4_TYPE, &CDlgSubFolding::OnBnClickedRadioPat4Type)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern message handlers
BOOL CDlgSubFolding::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgSubFolding::InitControls()
{
	short shBtnColor = 30;
	WINDOWPLACEMENT wp;
	RECT rc;

    GetDlgItem(IDC_SPIN_AREA)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_AREA)->SetWindowPos(NULL, rc.left, rc.top, 40, 25, SWP_SHOWWINDOW);

	GetDlgItem(IDC_SPIN_SLOPE)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_SLOPE)->SetWindowPos(NULL, rc.left, rc.top, 40, 25, SWP_SHOWWINDOW);
}

void CDlgSubFolding::UpdateControlsData()
{
	LPBYTE pPAT;
	long nSelectedROI, nPatNum, nPatType;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editArea = (long)m_pData->m_INSP[nSelectedROI].m_dTeachValue;
		m_editSlope = (long)m_pData->m_INSP[nSelectedROI].m_dSensity;
		m_editPatSizeX = m_pData->m_INSP[nSelectedROI].m_nPatWidth;
		m_editPatSizeY = m_pData->m_INSP[nSelectedROI].m_nPatHeight;
		nPatType = m_pData->m_INSP[nSelectedROI].m_nBlobOption;

		if (m_editPatSizeX<50) m_editPatSizeX = 50;
		if (m_editPatSizeY<50) m_editPatSizeY = 50;

		pPAT = m_pData->m_INSP[nSelectedROI].m_pPat[0];
		nPatNum = m_pData->m_INSP[nSelectedROI].m_nPatNum;
		m_nLocROI = m_pData->m_INSP[nSelectedROI].m_bLocROI;

		((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->SetCheck(FALSE);

		if (nPatType==0) ((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->SetCheck(TRUE);
		if (nPatType==1) ((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->SetCheck(TRUE);
		if (nPatType==2) ((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->SetCheck(TRUE);
		if (nPatType==3) ((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->SetCheck(TRUE);

		((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(FALSE);
		if (m_nLocROI) ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(TRUE);

//	    m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, m_editPatSizeX, m_editPatSizeY);
		UpdateData(false);
    }
}

void CDlgSubFolding::ApplyInspData(long nSelectROI)
{
	long nSel;
	float fCx, fCy;
	UpdateData(true);

    m_pData->m_INSP[nSelectROI].m_nInspectType = INSP_FOLDING;
	m_pData->m_INSP[nSelectROI].m_dTeachValue = (float)m_editArea;
	m_pData->m_INSP[nSelectROI].m_dSensity = (float)m_editSlope;
	m_pData->m_INSP[nSelectROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();

	nSel = 0;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->GetCheck()) nSel=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->GetCheck()) nSel=1;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->GetCheck()) nSel=2;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->GetCheck()) nSel=3;
	m_pData->m_INSP[nSelectROI].m_nBlobOption = nSel;

	m_pData->m_INSP[nSelectROI].m_nPatWidth = m_editPatSizeX;
	m_pData->m_INSP[nSelectROI].m_nPatHeight = m_editPatSizeY;
	m_pData->m_INSP[nSelectROI].m_nPatNum = 0;

	fCx = (m_pData->m_GOM[nSelectROI].m_ptPoint1.x + m_pData->m_GOM[nSelectROI].m_ptPoint2.x) / 2.0f;
	fCy = (m_pData->m_GOM[nSelectROI].m_ptPoint1.y + m_pData->m_GOM[nSelectROI].m_ptPoint3.y) / 2.0f;

	m_pData->m_GOM[nSelectROI].m_ptPoint1.x = fCx - m_editPatSizeX/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint2.x = fCx + m_editPatSizeX/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint3.x = fCx - m_editPatSizeX/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint4.x = fCx + m_editPatSizeX/2;

	m_pData->m_GOM[nSelectROI].m_ptPoint1.y = fCy - m_editPatSizeY/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint2.y = fCy - m_editPatSizeY/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint3.y = fCy + m_editPatSizeY/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint4.y = fCy + m_editPatSizeY/2;

	m_pParent->InvalidateView();
}

void CDlgSubFolding::OnDeltaposSpinSlope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if (m_editSlope<50) m_editSlope++;
	}
	else
	{
		if (m_editSlope>5)  m_editSlope--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnDeltaposSpinArea(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if (m_editArea<10000) m_editArea++;
	}
	else
	{
		if (m_editArea>10)     m_editArea--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnDeltaposSpinPatSizeX(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	long nSelectedROI;
	float fCx;
	GOM_ATTR *pGOM;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editPatSizeX /= 10;
		m_editPatSizeX *= 10;

		if (pNMUpDown->iDelta<0)
		{
			if(m_editPatSizeX<500) m_editPatSizeX+=10;
		}
		else
		{
			if(m_editPatSizeX>50)  m_editPatSizeX-=10;
		}

		pGOM = &m_pData->m_GOM[nSelectedROI];

		fCx = (pGOM->m_ptPoint1.x + pGOM->m_ptPoint2.x)/2;
		pGOM->m_ptPoint1.x = fCx - m_editPatSizeX/2;
		pGOM->m_ptPoint2.x = fCx + m_editPatSizeX/2;
		pGOM->m_ptPoint3.x = fCx - m_editPatSizeX/2;
		pGOM->m_ptPoint4.x = fCx + m_editPatSizeX/2;

		m_pParent->InvalidateView();
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnDeltaposSpinPatSizeY(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	long nSelectedROI;
	float fCy;
	GOM_ATTR *pGOM;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editPatSizeY /= 10;
		m_editPatSizeY *= 10;

		if (pNMUpDown->iDelta<0)
		{
			if(m_editPatSizeY<500) m_editPatSizeY+=10;
		}
		else
		{
			if(m_editPatSizeY>50)  m_editPatSizeY-=10;
		}

		pGOM = &m_pData->m_GOM[nSelectedROI];

		fCy = (pGOM->m_ptPoint1.y + pGOM->m_ptPoint3.y)/2;
		pGOM->m_ptPoint1.y = fCy - m_editPatSizeY/2.0f;
		pGOM->m_ptPoint2.y = fCy - m_editPatSizeY/2.0f;
		pGOM->m_ptPoint3.y = fCy + m_editPatSizeY/2.0f;
		pGOM->m_ptPoint4.y = fCy + m_editPatSizeY/2.0f;

		m_pParent->InvalidateView();
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		UpdateControlsData();
}

void CDlgSubFolding::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

}	


void CDlgSubFolding::OnBnClickedRadioPat4Type()
{
	// TODO: Add your control notification handler code here
}
