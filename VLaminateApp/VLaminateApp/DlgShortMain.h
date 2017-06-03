#pragma once
#include "afxcmn.h"
#include "Controls\\ListCtrlEx.h"
#include "Controls\\mscomm.h"

#define  DEF_SHORTLIST_MAX_NUM      37

// CDlgShortMain dialog
class CDlgShortMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShortMain)

public:
	CDlgShortMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShortMain();

	void     InitControls();
	void   	 InitListControls();
	void     UpdateListControls(BOOL bDecF, long nInxF, float fF, BOOL bDecR, long nInxR, float fR);
	void     InitIndexShort();
	void     SetShortTestCondition();
	long     GetStringFromText(long nPort, CString strText);
	long     GetIndexRearSide(long nIndexF);
	void     GetRS232CData();
	void     InitShortProc();
	void     CloseShortProc();
	void     SetEventCOM();
	void     WriteShortVToFile(BOOL bRear, BOOL bDec, float fV);
	BOOL     GetTesterVersion(long nComPort);
	CString  ReadComPort(long nComPort, long *nLen);
	UINT static  ThreadShortCOM(LPVOID pParam);

public :
    NPlotDll_CallDATA    NPlotDll_Data;

	long     m_nRearIndex[2];
	long     m_nFirstIndex[2];
	char     m_cLastSeq[2];
	bool     m_bGoodDec[2][SHORT_BUFFER_MAX_NUM];
	long     m_nNumID[2][SHORT_BUFFER_MAX_NUM];
	long     m_nVoltV[2][SHORT_BUFFER_MAX_NUM];
	long     m_nCurrV[2][SHORT_BUFFER_MAX_NUM];
	CString  m_strRemainT[2];
	

	long     m_nListIndexPoint;
	long     m_IndexData[DEF_SHORTLIST_MAX_NUM];
	float    m_FrontData[DEF_SHORTLIST_MAX_NUM];
	float    m_RearData[DEF_SHORTLIST_MAX_NUM];
	BOOL     m_FrontDec[DEF_SHORTLIST_MAX_NUM];
	BOOL     m_RearDec[DEF_SHORTLIST_MAX_NUM];
	BOOL     m_bOnRS232C;

	SYSTEMTIME	   m_sysShortOldTime, m_sysNewTime;

	HANDLE         m_hEventCOM;
	CWinThread    *m_pThreadCOM;

	CMSComm m_ctrlCOM[2];

	enum { IDD = IDD_DIALOG_SHORT_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx  m_listTestShort;
//	afx_msg LONG OnReadRS232CData(UINT, LONG);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
