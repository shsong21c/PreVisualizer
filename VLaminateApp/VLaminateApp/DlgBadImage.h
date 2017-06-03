#pragma once
#include "Controls\\Cdib.h"

// CDlgBadImage dialog

class CDlgBadImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBadImage)

public:
	CDlgBadImage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBadImage();

	void           UpdateFrameBadImage(long nCAM, long nIndexPnt);
	void           InvalidateView();
	void           InvalidateDrawInspItem(CDC *pDC);
	void           SetSaveImageFile(CString strPathName);

	CDib          *m_dispDib;
	CDC            m_memDC;
	CBitmap	       m_bmpBuffer;
	
	BOOL           m_bSaveStart;
	long           m_nCamID;
	CString        m_strImageFileName;

	INSP_RESULT_DATA m_pSelRes;

// Dialog Data
	enum { IDD = IDD_DIALOG_BAD_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnPaint();
};
