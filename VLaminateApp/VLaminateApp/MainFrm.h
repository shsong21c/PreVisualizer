
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "VLaminateAppView.h"
#include "CAM\EuresisCamLib.h"
#include "OmronPlcM.h"
#include "Controls\\ClockTimer.h"

class CMainFrame : public CFrameWnd
{
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	HWND           m_hSplash;

	CVLaminateAppView *m_pView;
//	CBaslerCamLib   m_CAM;
	CEuresisCamLib  m_CAM;
	COmronPlcM      m_OmronPlc;
	CRITICAL_SECTION	m_csSync;
	CClockTimer     m_ctrlTimer;

	BOOL           m_bGaugeRnR;
	BOOL           m_bGrab;
	BOOL           m_bLive;
	BOOL           m_bExecRealTime;
	BOOL           m_nAlignNum;
	BOOL           m_bShortTest;
	BOOL           m_bAppExec;
	BOOL           m_bCriticalF;
	BOOL           m_bOnData2PLC;
	BOOL           m_GrabCAM[CAM_MAX_NUM];
	BOOL           m_bStatusCOM[2];
	BOOL           m_bFirstReadyFlag;
	BOOL           m_bSendMsg;
	BOOL           m_bSendMsgUp;
	BOOL           m_bSendMsgDn;
	BOOL           m_bWaitMoveDone;

	long           m_nPageNo;
	long           m_nSelectCAM;
	long           m_nZoomScale;
	long           m_nLastGrabID;
	long           m_nJigNo;
	long           m_nResetCam;

	float          m_fSendDataCUT;
	float          m_fSumDataEPC;
	float          m_fOldTEOffset;
	float          m_fFrontShort;
	float          m_fRearShort;
	float          m_fSendDataEPC;

	float          m_fCamDX[CAM_MAX_NUM];
	float          m_fCamDY[CAM_MAX_NUM];
	float          m_fCamDT[CAM_MAX_NUM];

	float          m_fAlignDX;
	float          m_fAlignDY;
	float          m_fAlignDT;

	long           m_nDaqIndex;
	long           m_nCountDataEPC;
	bool           m_bCOMM;
	long           m_nRecipeNum;
	int            m_OutD[20];

	CString        m_strFilePath;
	CString        m_strFileType;
	CString		   m_pstrLoadFileList[CAM_MAX_NUM][100]; // shsong
	CString        m_strModelType;
	CString        m_strLotID;
	long		   m_lLoadTotal;

	SYSTEMTIME	   m_logStartTime;
	SYSTEMTIME	   m_logEndTime;
	double         m_dEllipsedUmac;
	double         m_dStartTime;
	double         m_dEndTime;

	SYSTEMTIME	   m_sysNewTime, m_sysOldTime;

	SETTING_DATA   m_Setting;
	SEARCH_DATA    m_Search;
	ETC_DATA       m_Etc;
	PRODUCT_DATA   m_ProdData;
	INSP_RESULT_DATA *m_pRes[VIEW_MAX_NUM];

	long           m_nTapeMaxNum;
	CString        m_strTapeTitle[20];
	float          m_fTapeW[20];
	float          m_fTapeH[20];
	float          m_fTapeSpecPx[20];
	float          m_fTapeSpecPy[20];
	float          m_fTapeDx[20];
	float          m_fTapeDy[20];
	
// 작업입니다.
public:
	void           Grab();
	void           Live();
	BOOL           LoadSysValueFromFile();
    BOOL           SaveSysValueToFile();
	BOOL           LoadModelData();
    BOOL           SaveModelData();
	BOOL           LoadOffsetFile(CString strFileName);
	void		   WriteTextModelName(CString str);
	void		   WriteTextStatus(CString str);
	void		   WriteTextMessage(CString str);
	void		   WriteTextShortMessage(CString str);
	void           SetCheckLED(long nCAM, BOOL bOnOff);
	void		   ExitProgram();
	void           LoadImage(CString strFileName);
	void           SaveImage(CString strFileName);
	void           AsSaveImage(CString strFileName);
    void		   SaveRealTimeData();
	int			   ExecMessageHandleAll();
	int			   ExecMessageHandleOne();
    BOOL           SendStringForWrite(CString strSend);
    void           WriteStringToFile(long nCAM);
    void           WriteLogToFile(char *string);
    void           DeleteDataPath();
	void           TransOriginToBadImage(long nCAM, long pRes, LPBYTE pImage);
	void           CopyResultData(INSP_RESULT_DATA *pDest, INSP_RESULT_DATA *pSour);
	CString        GetModelName(long nOrderNum);
	//void           SendResult2UMAC(BOOL bFDec, BOOL bRDec, BOOL bRetCUT, float fCutOS, BOOL bRetEPC, float fOffsetEPC); // shsong
	void           SendTapeOffsetDataToPLC(INSP_RESULT_DATA *pRes);
	long           GetIndexTitleROI(INSP_RESULT_DATA* pRes, CString strTitle);
	long           GetIndexTitleLinkROI(INSP_RESULT_DATA* pRes, CString strTitle);
	BOOL           CopyRealTimeImage(long nCAM, LPBYTE pImage);
	//void           CopyRealTimeMelsec(INSP_RESULT_DATA *pRes1, INSP_RESULT_DATA *pRes2);

	void           InitAlignFactor(long nCamID);
	BOOL           GetOffsetAlignFactor(long nCamID);
	BOOL           SendStagePosToEthenet(long nCamID);
	long           ReadDataFromMelSecAddr(CString strAddr);
	void           WriteDataToMelSecAddr(CString strAddr, long nDataV);

#if 0
	void           GetMotionAlignValue(float *fX, float *fY, float *fT);
#endif // 0

	void           ReceiveDataFromEthernet();
	void           SetEvent(long nTime, DWORD nMsgID);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar     m_wndStatusBar;

	OFFSET_DATA    m_OffsetD[OFFSET_MAX_NUM];

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


