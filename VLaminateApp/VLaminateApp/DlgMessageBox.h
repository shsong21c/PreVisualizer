#pragma once


// CDlgMessageBox ��ȭ �����Դϴ�.

class CDlgMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessageBox)

public:
	CDlgMessageBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMessageBox();

	long     m_nMessageID;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MESSAGE_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLocCal();
	afx_msg void OnBnClickedBtnCompensCal();
	virtual BOOL OnInitDialog();
};
