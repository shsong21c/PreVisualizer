// DlgCal.cpp : implementation file
//

#include "stdafx.h"
#include "VLaminateApp.h"
#include "MainFrm.h"
#include "DlgCal.h"
#include "NAppDll.h"
#include "math.h"
#include "afxdialogex.h"

extern CMainFrame   *m_pMainFrm;  
// CDlgCal dialog

IMPLEMENT_DYNAMIC(CDlgCal, CDialogEx)

CDlgCal::CDlgCal(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCal::IDD, pParent)
{
}

CDlgCal::~CDlgCal()
{
}

void CDlgCal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_CAM_X1, m_editCam1X);
	DDX_Control(pDX, IDC_EDIT_CAM_Y1, m_editCam1Y);
	DDX_Control(pDX, IDC_EDIT_CAM_X2, m_editCam2X);
	DDX_Control(pDX, IDC_EDIT_CAM_Y2, m_editCam2Y);
	DDX_Control(pDX, IDC_EDIT_LIMIT_MX, m_editLimitMX);
	DDX_Control(pDX, IDC_EDIT_LIMIT_PX, m_editLimitPX);
	DDX_Control(pDX, IDC_EDIT_LIMIT_MY, m_editLimitMY);
	DDX_Control(pDX, IDC_EDIT_LIMIT_PY, m_editLimitPY);
	DDX_Control(pDX, IDC_EDIT_LIMIT_MT, m_editLimitMT);
	DDX_Control(pDX, IDC_EDIT_LIMIT_PT, m_editLimitPT);
	DDX_Control(pDX, IDC_EDIT_OFFSET_X, m_editOffsetX);
	DDX_Control(pDX, IDC_EDIT_OFFSET_Y, m_editOffsetY);
	DDX_Control(pDX, IDC_EDIT_OFFSET_T, m_editOffsetT);
}

BEGIN_MESSAGE_MAP(CDlgCal, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgCal::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CAL_EXEC, &CDlgCal::OnBnClickedButtonCalExec)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_MINUS_SET, &CDlgCal::OnBnClickedButtonMinusSet)
	ON_BN_CLICKED(IDC_BUTTON_PLUS_SET, &CDlgCal::OnBnClickedButtonPlusSet)
END_MESSAGE_MAP()

BOOL CDlgCal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowPos(NULL, 300, 300, 340, 580, SWP_SHOWWINDOW);

	InitButtonControls();
	UpdateControlsData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCal::InitButtonControls()
{
	CString str;

	/*
	m_editCoodiX.SetTextColor(RGB(0,0,0));
	m_editCoodiX.SetBkColor(RGB(255,255,255));
	m_editCoodiX.SetFontSize(17);

	m_editCoodiY.SetTextColor(RGB(0,0,0));
	m_editCoodiY.SetBkColor(RGB(255,255,255));
	m_editCoodiY.SetFontSize(17);
*/

	m_editCam1X.SetTextColor(RGB(0,0,0));
	m_editCam1X.SetBkColor(RGB(255,255,255));
	m_editCam1X.SetFontSize(17);
    m_editCam1Y.SetTextColor(RGB(0,0,0));
	m_editCam1Y.SetBkColor(RGB(255,255,255));
	m_editCam1Y.SetFontSize(17);

	m_editCam2X.SetTextColor(RGB(0,0,0));
	m_editCam2X.SetBkColor(RGB(255,255,255));
	m_editCam2X.SetFontSize(17);
	m_editCam2Y.SetTextColor(RGB(0,0,0));
	m_editCam2Y.SetBkColor(RGB(255,255,255));
	m_editCam2Y.SetFontSize(17);
	
	m_editLimitMX.SetTextColor(RGB(0,0,0));
	m_editLimitMX.SetBkColor(RGB(255,255,255));
	m_editLimitMX.SetFontSize(17);
	m_editLimitPX.SetTextColor(RGB(0,0,0));
	m_editLimitPX.SetBkColor(RGB(255,255,255));
	m_editLimitPX.SetFontSize(17);

	m_editLimitMY.SetTextColor(RGB(0,0,0));
	m_editLimitMY.SetBkColor(RGB(255,255,255));
	m_editLimitMY.SetFontSize(17);
	m_editLimitPY.SetTextColor(RGB(0,0,0));
	m_editLimitPY.SetBkColor(RGB(255,255,255));
	m_editLimitPY.SetFontSize(17);

	m_editLimitMT.SetTextColor(RGB(0,0,0));
	m_editLimitMT.SetBkColor(RGB(255,255,255));
	m_editLimitMT.SetFontSize(17);
	m_editLimitPT.SetTextColor(RGB(0,0,0));
	m_editLimitPT.SetBkColor(RGB(255,255,255));
	m_editLimitPT.SetFontSize(17);

	m_editOffsetX.SetTextColor(RGB(0,0,0));
	m_editOffsetX.SetBkColor(RGB(255,255,255));
	m_editOffsetX.SetFontSize(17);

	m_editOffsetY.SetTextColor(RGB(0,0,0));
	m_editOffsetY.SetBkColor(RGB(255,255,255));
	m_editOffsetY.SetFontSize(17);

	m_editOffsetT.SetTextColor(RGB(0,0,0));
	m_editOffsetT.SetBkColor(RGB(255,255,255));
	m_editOffsetT.SetFontSize(17);

	GetDlgItem(IDC_BUTTON_PLUS_SET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CAL_EXEC)->EnableWindow(FALSE);
}

void CDlgCal::UpdateControlsData()
{
	CString str;

	m_pMainFrm->m_Setting.m_fCoordiX = 0.0f;
	m_pMainFrm->m_Setting.m_fCoordiY = 0.0f;

	/*
	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCoordiX);
	m_editCoodiX.SetTextOut(str);
	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCoordiY);
	m_editCoodiY.SetTextOut(str);

	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCoordiX2);
	m_editCoodiX2.SetTextOut(str);
	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCoordiY2);
	m_editCoodiY2.SetTextOut(str);
	*/

	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCamX[0]);
	m_editCam1X.SetTextOut(str);
	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCamY[0]);
	m_editCam1Y.SetTextOut(str);

	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCamX[1]);
	m_editCam2X.SetTextOut(str);
	str.Format(_T("%.3f"), m_pMainFrm->m_Setting.m_fCamY[1]);
	m_editCam2Y.SetTextOut(str);

	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fLimitMinusX);
	m_editLimitMX.SetTextOut(str);
	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fLimitPlusX);
	m_editLimitPX.SetTextOut(str);

	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fLimitMinusY);
	m_editLimitMY.SetTextOut(str);
	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fLimitPlusY);
	m_editLimitPY.SetTextOut(str);

	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fLimitMinusT);
	m_editLimitMT.SetTextOut(str);
	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fLimitPlusT);
	m_editLimitPT.SetTextOut(str);

	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fOffsetX[0]);
	m_editOffsetX.SetTextOut(str);
	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fOffsetY[0]);
	m_editOffsetY.SetTextOut(str);
	str.Format(_T("%.2f"), m_pMainFrm->m_Etc.m_fOffsetT[0]);
	m_editOffsetT.SetTextOut(str);
}

void CDlgCal::OnBnClickedButtonSave()
{
	if (MessageBox(_T(" 변경된 값을 저장하시겠습니까 ? "), _T(" Save message box "), MB_ICONQUESTION|MB_YESNO)==IDYES)
	{
//		m_pMainFrm->m_Setting.m_fCoordiX = (float)m_editCoodiX.GetTextValue();
//		m_pMainFrm->m_Setting.m_fCoordiY = (float)m_editCoodiY.GetTextValue();
		m_pMainFrm->m_Setting.m_fCamX[0] = (float)m_editCam1X.GetTextValue();
		m_pMainFrm->m_Setting.m_fCamY[0] = (float)m_editCam1Y.GetTextValue();
		m_pMainFrm->m_Setting.m_fCamX[1] = (float)m_editCam2X.GetTextValue();
		m_pMainFrm->m_Setting.m_fCamY[1] = (float)m_editCam2Y.GetTextValue();
	
		m_pMainFrm->m_Etc.m_fLimitMinusX = (float)m_editLimitMX.GetTextValue();
		m_pMainFrm->m_Etc.m_fLimitPlusX = (float)m_editLimitPX.GetTextValue();
		m_pMainFrm->m_Etc.m_fLimitMinusY = (float)m_editLimitMY.GetTextValue();
		m_pMainFrm->m_Etc.m_fLimitPlusY = (float)m_editLimitPY.GetTextValue();
		m_pMainFrm->m_Etc.m_fLimitMinusT = (float)m_editLimitMT.GetTextValue();
		m_pMainFrm->m_Etc.m_fLimitPlusT = (float)m_editLimitPT.GetTextValue();

		m_pMainFrm->m_Etc.m_fOffsetX[0] = (float)m_editOffsetX.GetTextValue();
		m_pMainFrm->m_Etc.m_fOffsetY[0] = (float)m_editOffsetY.GetTextValue();
		m_pMainFrm->m_Etc.m_fOffsetT[0] = (float)m_editOffsetT.GetTextValue();

		m_pMainFrm->SaveSysValueToFile();
	}
}

int CDlgCal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CDlgCal::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
}

void CDlgCal::OnBnClickedButtonMinusSet()
{
	CString strT;
	BOOL bRet=FALSE;
	long nPosD, nCamID;
	float fPosX1, fPosX2, fPosY1, fPosY2;
	float fScaleX, fScaleY;
	INSP_RESULT_DATA *pResD;

	nCamID = m_pMainFrm->m_nSelectCAM;
	nPosD = nCamID * 6;
	if (nCamID >= CAM_MAX_NUM) return;

	strT = _T(" X, Y  캘리브레이션 설정을 실행하시겠습니까 ? ");
	if (MessageBox(strT, _T(" Calibration message box "), MB_ICONQUESTION|MB_YESNO)==IDYES)
	{
		m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)(20 * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_T_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Z_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 5;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

    	m_pMainFrm->SetEvent(400, TIMEMSG_WAIT_MOVE_DONE);
		m_pMainFrm->SetEvent(2000, TIMEMSG_WAIT_MOVE_DONE);
		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, TRUE);
		Sleep(50);
		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, FALSE);

		NAppDll_ExecuteInspection(nCamID);
		NAppDll_InvalidateView(nCamID);
		pResD = (INSP_RESULT_DATA *)NAppDll_GetResultData(nCamID);
		if (pResD->m_bDecision) 
		{
			bRet = TRUE;
			goto goto_end;
		}
		fPosX1 = pResD->m_dPixelCX;

			
		m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)(-40 * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_T_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Z_ALIGN+nPosD] = 0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 5;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

    	m_pMainFrm->SetEvent(400, TIMEMSG_WAIT_MOVE_DONE);
		m_pMainFrm->SetEvent(2000, TIMEMSG_WAIT_MOVE_DONE);

		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, TRUE);
		Sleep(50);
		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, FALSE);

		NAppDll_ExecuteInspection(nCamID);
		NAppDll_InvalidateView(nCamID);
		pResD = (INSP_RESULT_DATA *)NAppDll_GetResultData(nCamID);
		if (pResD->m_bDecision) 
		{
			bRet = TRUE;
			goto goto_end;
		}
		fPosX2 = pResD->m_dPixelCX;


		m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)(20 * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = (DWORD)(-20 * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 5;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

    	m_pMainFrm->SetEvent(400, TIMEMSG_WAIT_MOVE_DONE);
		m_pMainFrm->SetEvent(2000, TIMEMSG_WAIT_MOVE_DONE);

		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, TRUE);
		Sleep(50);
		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, FALSE);

		NAppDll_ExecuteInspection(nCamID);
		NAppDll_InvalidateView(nCamID);
		pResD = (INSP_RESULT_DATA *)NAppDll_GetResultData(nCamID);
		if (pResD->m_bDecision) 
		{
			bRet = TRUE;
			goto goto_end;
		}
		fPosY1 = pResD->m_dPixelCY;

			
		m_pMainFrm->m_OutD[OMRON_OUT_UP_X_ALIGN+nPosD] = (DWORD)0;
		m_pMainFrm->m_OutD[OMRON_OUT_UP_Y_ALIGN+nPosD] = (DWORD)(40 * 100);
		m_pMainFrm->m_OutD[OMRON_OUT_UP_GO_EXEC+nPosD] = 5;
		m_pMainFrm->m_OutD[OMRON_OUT_CAL_MODE] = 1;
		m_pMainFrm->m_bSendMsg = TRUE;

    	m_pMainFrm->SetEvent(400, TIMEMSG_WAIT_MOVE_DONE);
		m_pMainFrm->SetEvent(2000, TIMEMSG_WAIT_MOVE_DONE);

		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, TRUE);
		Sleep(50);
		m_pMainFrm->m_CAM.SetActivateTrigger(nCamID, FALSE);

		NAppDll_ExecuteInspection(nCamID);
		NAppDll_InvalidateView(nCamID);
		pResD = (INSP_RESULT_DATA *)NAppDll_GetResultData(nCamID);
		if (pResD->m_bDecision) 
		{
			bRet = TRUE;
			goto goto_end;
		}
		fPosY2 = pResD->m_dPixelCY;

		fScaleX = 40 / fabs(fPosX2 - fPosX1);
		fScaleY = 40 / fabs(fPosY2 - fPosY1);

		NAppDll_SetScaleFactor(nCamID, fScaleX, fScaleY);
		
goto_end:
		if (bRet)
		{
			strT = _T(" X, Y  캘리브레이션 설정을 실패하였습니다 .  ");
    		MessageBox(strT, _T(" Calibration message box "), MB_ICONEXCLAMATION|MB_OK);
		}
		else
		{
			strT = _T(" X, Y  캘리브레이션 설정을 완료하였습니다 .  ");
    		MessageBox(strT, _T(" Calibration message box "), MB_ICONEXCLAMATION|MB_OK);
		}
	}


}

void CDlgCal::OnBnClickedButtonPlusSet()
{
	CString strT;

	strT = _T(" Rotation cenetr axis Calibration을 실행하시겠습니까 ? ");
	if (MessageBox(strT, _T(" Calibration message box "), MB_ICONQUESTION|MB_YESNO)==IDYES)
	{
		GetDlgItem(IDC_BUTTON_PLUS_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CAL_EXEC)->EnableWindow(TRUE);
		m_pMainFrm->m_pView->m_pDlgExecAuto->SetPositionCAL(1);
	}	
}

void CDlgCal::OnBnClickedButtonCalExec()
{

	GetDlgItem(IDC_BUTTON_PLUS_SET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CAL_EXEC)->EnableWindow(FALSE);
	m_pMainFrm->m_pView->m_pDlgExecAuto->SetPositionCAL(0);

//    double fAngleT, fCoordiX, fCoordiY;
	/*
	CString strT;

	strT = _T(" 카메라 Location Calibration을 실행하시겠습니까 ? ");
	if (MessageBox(strT, _T(" Calibration message box "), MB_ICONQUESTION|MB_YESNO)==IDYES)
	{
		strT = _T(" 마우스를 지그 중심에 위치시키고 클릭하세요. ");
		if (MessageBox(strT, _T(" Calibration message box "), MB_ICONQUESTION|MB_OK)==IDOK)
		    m_pMainFrm->m_pView->m_pDlgExecAuto->SetPositionCAL();


		if (m_bExecCAM[CAM_UP1] && m_bExecAng[0] && m_bExecAng[1])
		{
			m_bExecCAM[CAM_UP1] = FALSE;
			m_bExecAng[0] = FALSE;
			m_bExecAng[1] = FALSE;

			fAngleT = -2*DEF_CAL_STEP_ANGLE;
			NAppDll_FindRotationAxis((double)m_fCamMinusPosX[CAM_UP1], (double)m_fCamMinusPosY[CAM_UP1], (double)m_fCamPlusPosX[CAM_UP1], (double)m_fCamPlusPosY[CAM_UP1], fAngleT, &fCoordiX, &fCoordiY);

			str.Format(_T("%.3f"), 0.0f);
			m_editCoodiX.SetTextOut(str);
			str.Format(_T("%.3f"), 0.0f);
			m_editCoodiY.SetTextOut(str);

			str.Format(_T("%.3f"), -1*fCoordiX);
			m_editCam1X.SetTextOut(str);
			str.Format(_T("%.3f"), fCoordiY);
			m_editCam1Y.SetTextOut(str);

			str.Format(_T(" 카메라 %d 위치를 확인 완료하였습니다. "), CAM_UP1+1);
    		MessageBox(str, _T(" Calibration message box "), MB_ICONINFORMATION|MB_OK);
		}

		if (m_bExecCAM[CAM_UP2] && m_bExecAng[0] && m_bExecAng[1])
		{
			m_bExecCAM[CAM_DN] = FALSE;
			m_bExecAng[0] = FALSE;
			m_bExecAng[1] = FALSE;

			fAngleT = -2*DEF_CAL_STEP_ANGLE;
			NAppDll_FindRotationAxis((double)m_fCamMinusPosX[CAM_UP2], (double)m_fCamMinusPosY[CAM_UP2], (double)m_fCamPlusPosX[CAM_UP2], (double)m_fCamPlusPosY[CAM_UP2], fAngleT, &fCoordiX, &fCoordiY);

			str.Format(_T("%.3f"), 0.0f);
			m_editCoodiX.SetTextOut(str);
			str.Format(_T("%.3f"), 0.0f);
			m_editCoodiY.SetTextOut(str);

			str.Format(_T("%.3f"), -1*fCoordiX);
			m_editCam2X.SetTextOut(str);
			str.Format(_T("%.3f"), fCoordiY);
			m_editCam2Y.SetTextOut(str);

			str.Format(_T(" 카메라 %d 위치를 확인 완료하였습니다. "), CAM_DN+1);
    		MessageBox(str, _T(" Calibration message box "), MB_ICONINFORMATION|MB_OK);
		}

		if (m_bExecCAM[CAM_DN] && m_bExecAng[0] && m_bExecAng[1])
		{
			m_bExecCAM[CAM_DN] = FALSE;
			m_bExecAng[0] = FALSE;
			m_bExecAng[1] = FALSE;

			fAngleT = -2*DEF_CAL_STEP_ANGLE;
			NAppDll_FindRotationAxis((double)m_fCamMinusPosX[CAM_DN], (double)m_fCamMinusPosY[CAM_DN], (double)m_fCamPlusPosX[CAM_DN], (double)m_fCamPlusPosY[CAM_DN], fAngleT, &fCoordiX, &fCoordiY);

			str.Format(_T("%.3f"), 0.0f);
			m_editCoodiX.SetTextOut(str);
			str.Format(_T("%.3f"), 0.0f);
			m_editCoodiY.SetTextOut(str);

			str.Format(_T("%.3f"), -1*fCoordiX);
			m_editHalfCamX.SetTextOut(str);
			str.Format(_T("%.3f"), fCoordiY);
			m_editHalfCamY.SetTextOut(str);

			str.Format(_T(" 카메라 %d 위치를 확인 완료하였습니다. "), CAM_DN+1);
    		MessageBox(str, _T(" Calibration message box "), MB_ICONINFORMATION|MB_OK);
		}


	}

	//m_pMainFrm->SendStagePosToEthenet(-1.23, 0.15, -0.567);
	*/
}

void CDlgCal::ExecCalProc(long nCamID, long nPlusAng)
{
	INSP_RESULT_DATA *pResCam;

	pResCam = (INSP_RESULT_DATA *)NAppDll_ExecuteInspection(nCamID);
	m_bExecCAM[nCamID] = TRUE;
	m_bExecAng[nPlusAng] = TRUE;

	if (!nPlusAng)
	{
		m_fCamMinusPosX[nCamID] = pResCam->m_dCX;
		m_fCamMinusPosY[nCamID] = pResCam->m_dCY;
	}
	else 
	{
		m_fCamPlusPosX[nCamID] = pResCam->m_dCX;
		m_fCamPlusPosY[nCamID] = pResCam->m_dCY;
	}
}

void CDlgCal::SetPositionCAL(long nCamID, float fPosX, float fPosY)
{
	CString str;

//	if (MessageBox(_T(" 카메라 Location Calibration을 실행하시겠습니까 ? "), _T(" Calibration message box "), MB_ICONQUESTION|MB_YESNO)==IDYES)
	{
		if (nCamID==CAM_UP)
		{
			str.Format(_T("%.3f"), fPosX);
			m_editCam1X.SetTextOut(str);
			str.Format(_T("%.3f"), fPosY);
			m_editCam1Y.SetTextOut(str);
		}
		if (nCamID==CAM_DN)
		{
			str.Format(_T("%.3f"), fPosX);
			m_editCam2X.SetTextOut(str);
			str.Format(_T("%.3f"), fPosY);
			m_editCam2Y.SetTextOut(str);
		}

		str.Format(_T(" 카메라 %d 위치를 확인 완료하였습니다. "), nCamID+1);
		MessageBox(str, _T(" Calibration message box "), MB_ICONINFORMATION|MB_OK);
	}
}

