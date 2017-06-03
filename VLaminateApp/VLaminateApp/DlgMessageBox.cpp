// DlgMessageBox.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgMessageBox.h"
#include "afxdialogex.h"

// CDlgMessageBox ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CDlgMessageBox, CDialogEx)

CDlgMessageBox::CDlgMessageBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMessageBox::IDD, pParent)
{

}

CDlgMessageBox::~CDlgMessageBox()
{
}

void CDlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMessageBox, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOC_CAL, &CDlgMessageBox::OnBnClickedBtnLocCal)
	ON_BN_CLICKED(IDC_BTN_COMPENS_CAL, &CDlgMessageBox::OnBnClickedBtnCompensCal)
END_MESSAGE_MAP()

// CDlgMessageBox �޽��� ó�����Դϴ�.

void CDlgMessageBox::OnBnClickedBtnLocCal()
{
	m_nMessageID = 1;
	CDialogEx::OnOK();
}

void CDlgMessageBox::OnBnClickedBtnCompensCal()
{
	m_nMessageID = 2;
	CDialogEx::OnOK();
}

BOOL CDlgMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nMessageID = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
