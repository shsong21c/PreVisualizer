// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "..\MainFrm.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
}

// ���� ��ü�� ���� ������ ������ �ʱ�ȭ�Ѵ�.
CListenSocket::CListenSocket(CMainFrame* pMainFrm)	
{
	m_bAccept = false;
	m_bReceive = false;
	m_bClose = false;
	m_pMainFrm = pMainFrm;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

// Ŭ���̾�Ʈ�κ��� ���� ��û�� ���� �޼����� ������ ȣ��ȴ�.
void CListenSocket::OnAccept(int nErrorCode) 
{
	m_bAccept = true;
	m_pMainFrm->OnSocketAccept();
	m_bAccept = false;

	CSocket::OnAccept(nErrorCode);
}

void CListenSocket::OnClose(int nErrorCode) 
{
	CSocket::OnClose(nErrorCode);
}
