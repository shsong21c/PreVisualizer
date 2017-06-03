#pragma once
#include "Controls\EditEx.h"

// CDlgCal dialog
#define ITER_MAX_NUM           3

class CDlgCal : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCal)

public:
	CDlgCal(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCal();

	void    InitButtonControls();
	void    UpdateControlsData();
	void    SetPositionCAL(long nCamID, float fPosX, float fPosY);
	void    ExecCalProc(long nCamID, long nPlusAng);

// Dialog Data
	enum { IDD = IDD_DIALOG_CAL };
public:
	CEditEx m_editCam1X;
	CEditEx m_editCam1Y;
	CEditEx m_editCam2X;
	CEditEx m_editCam2Y;
    CEditEx m_editOffsetX;
    CEditEx m_editOffsetY;
    CEditEx m_editOffsetT;
	CEditEx m_editLimitMX;
	CEditEx m_editLimitPX;
    CEditEx m_editLimitMY;
	CEditEx m_editLimitPY;
    CEditEx m_editLimitMT;
    CEditEx m_editLimitPT;

	BOOL    m_bExecCal;
	BOOL    m_bExecCAM[CAM_MAX_NUM];
	BOOL    m_bExecAng[CAM_MAX_NUM];
	
    float   m_fPreX;
	float   m_fPreY;
	float   m_fPreT;
	float   m_fAngleDist;

	float   m_fCamMinusPosX[CAM_MAX_NUM];
	float   m_fCamMinusPosY[CAM_MAX_NUM];
	float   m_fCamPlusPosX[CAM_MAX_NUM];
	float   m_fCamPlusPosY[CAM_MAX_NUM];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticConfig();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCalExec();
	virtual BOOL OnInitDialog();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonMinusSet();
	afx_msg void OnBnClickedButtonPlusSet();
};
