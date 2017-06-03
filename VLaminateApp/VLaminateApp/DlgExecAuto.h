#pragma once
#include "afxcmn.h"
#include "Controls\\dsspanel.h"
#include "DlgBadImage.h"
#include "Controls\\ListCtrlEx.h"
//#include "acteasyif.h"

// CDlgExecAuto 대화 상자입니다.
class CDlgExecAuto : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExecAuto)

public:
	CDlgExecAuto(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgExecAuto();

	void         OpenCamera();
	void     	 InitButtonControls();
	void         InitCameraControls();
	void         InitEtcControls();
	void         ShowVisionWindow(long nCam, BOOL bShow);
	void         ShowVisionWindow(long nCamID);
	void         CloseControls();
    void         InitRealTimeProc();
	void         CloseRealTimeProc();
	long         GetSelectedTab(long nSel);
	void         GetSelectedSubTab();
	void         UpdateModelName();
	void         InitCountData();
	void         UpdateCountData(long nCAM);
	BOOL         InspectProgressGrab(long lParam);
	void         InitListControls();
	void         UpdateResultList();
	void         InitAlignListControls(long nCAM);
	void         UpdateAlignListControl(long nCAM);
	void         SetTransLanguage(long nValue);
	void         SetPositionCAL(BOOL bFlag);

	UINT static  ThreadDataExport(LPVOID pParam);
	UINT static  ThreadMelsecComm(LPVOID pParam);
//	UINT static  ThreadSendDataToPLC(LPVOID pParam);	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EXEC_AUTO };

public :
	CTabCtrl	   m_tabCAM;
	CTabCtrl	   m_tabImage;
	CTabCtrl	   m_tabImgCAM;
	CTabCtrl	   m_tabList;
	CListCtrlEx	   m_listCtrl[CAM_MAX_NUM];
	CImageList     m_imgListSel;
	CDlgBadImage  *m_pDlgBadImage;

	HANDLE         m_hEventExport;
	HANDLE         m_hEventMelsec;
	HANDLE         m_hEventSendData;

	CWinThread    *m_pThreadExport;
	CWinThread    *m_pThreadMelsec;
	CWinThread    *m_pThreadSendData;
	
	REALTIME_TEMP    *m_pRT[CAM_MAX_NUM];

	CRITICAL_SECTION  m_csSync;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnSelchangeTabCam(NMHDR *pNMHDR, LRESULT *pResult);

	//CActeasyif   m_ActEasyIF;
	long         m_nErrResponseCnt;
	long         m_nCntSendDIO;
	long         m_nCntReceiveDIO;
	BOOL         m_bOpenComm;

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelchangeTabImage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeTabImageCam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnStopRnr();
	afx_msg void OnBnClickedBtnStartRnr();
	afx_msg void OnBnClickedBtnTriggerPos();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnSelchangeTabList(NMHDR *pNMHDR, LRESULT *pResult);
};
