#pragma once

// CDlgResult ��ȭ �����Դϴ�.
class CDlgResult : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgResult();

	void     InitListControls();
	void     SetTransLanguage(long nValue);
	void     UpdateResultList();
	void     InitPlotControls();
	void     ResetProductData();
	void     WriteProductData(long nCAM, BOOL bLink, long nOrderNum, long nROI);


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RESULT };

	long           m_nRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
