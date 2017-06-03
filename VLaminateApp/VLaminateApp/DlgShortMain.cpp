// DlgShortMain.cpp : implementation file
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgShortMain.h"
#include "NPioControlDll.h"
#include "MainFrm.h"
#include "math.h"
#include "NPlotDll.h"
#include "NGuiDll.h"
#include "afxdialogex.h"

// CDlgShortMain dialog
void CALLBACK  NPlotDll_TestShortEvent();
extern CMainFrame    *m_pMainFrm;

IMPLEMENT_DYNAMIC(CDlgShortMain, CDialogEx)

CDlgShortMain::CDlgShortMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShortMain::IDD, pParent)
{
	m_nListIndexPoint = 0;
}

CDlgShortMain::~CDlgShortMain()
{
	SetEvent(m_hEventCOM);
	NPlotDll_Close(DEF_SHORT_PLOT_NUM);

	NUiListDll_Close(LIST_SHORT_ID);
}

void CDlgShortMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEST_SHORT, m_listTestShort);
//	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlCOM[0]);
//	DDX_Control(pDX, IDC_MSCOMM2, m_ctrlCOM[1]);
}

BEGIN_MESSAGE_MAP(CDlgShortMain, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgShortMain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgShortMain::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgShortMain::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CDlgShortMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();
	InitListControls();
	InitIndexShort();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDlgShortMain message handlers
void CDlgShortMain::InitControls()
{
	WINDOWPLACEMENT wp;
	RECT rc;

	GetDlgItem(IDC_STATIC_SHORT_PLOT)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
	GetDlgItem(IDC_STATIC_SHORT_PLOT)->SetWindowPos(NULL, rc.left, rc.top, 1018, PLOT_MAX_HEIGHT, SWP_SHOWWINDOW);

	NPlotDll_Open(DEF_SHORT_PLOT_NUM, (long)GetDlgItem(IDC_STATIC_SHORT_PLOT));
	NPlotDll_LoadSetupData(DEF_SHORT_PLOT_NUM, DEF_SHORTPLOT_PATHNAME);
	NPlotDll_InitialData(DEF_SHORT_PLOT_NUM);
	NPlotDll_SetMaxDataNum(DEF_SHORT_PLOT_NUM, PLOT_MAX_DATA_NUM);
	NPlotDll_SetEnableHoriRefLine(DEF_SHORT_PLOT_NUM, TRUE);
	NPlotDll_SetRangeX(DEF_SHORT_PLOT_NUM, 0, PLOT_MAX_DATA_NUM);
	NPlotDll_SetRangeLY(DEF_SHORT_PLOT_NUM, 0, 1);
	NPlotDll_SetPosHoriRefLine(DEF_SHORT_PLOT_NUM, TRUE, m_pMainFrm->m_Setting.m_fContactCurr, m_pMainFrm->m_Setting.m_fLimitCurr);
	NPlotDll_SetEvent(DEF_SHORT_PLOT_NUM, NPlotDll_TestShortEvent,  (long)&NPlotDll_Data);

	NUiListDll_Open(LIST_SHORT_ID, (long)GetDlgItem(IDC_STATIC_LIST_SHORT));

    //{BEGIN-COM : Serial 통신 초기화 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// 만약에 Serial 통신을 사용하지 않으려면  아래부분을 막고 Serial 통신버튼의
	// Property에서 Visible을 UnCheck 하면된다.
	/*
	m_ctrlCOM[0].SetSettings(_T("19200, n, 8, 1")); // 먼저 데이타를 설정하고
	m_ctrlCOM[0].SetCommPort((short)m_pMainFrm->m_Setting.m_nCOM1);
	m_ctrlCOM[0].SetInputMode(1);                //Binary
	m_ctrlCOM[0].SetPortOpen(TRUE);              // COM 포트를 연다.
    m_pMainFrm->m_bStatusCOM[0] = TRUE;
	
	m_ctrlCOM[1].SetSettings(_T("19200, n, 8, 1")); // 먼저 데이타를 설정하고
	m_ctrlCOM[1].SetCommPort((short)m_pMainFrm->m_Setting.m_nCOM2);
	m_ctrlCOM[1].SetInputMode(1);                //Binary
	m_ctrlCOM[1].SetPortOpen(TRUE);              // COM 포트를 연다.
	m_pMainFrm->m_bStatusCOM[1] = TRUE;
*/
    //}END-COM : Serial 통신 초기화 끝 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

/*
void CDlgShortMain::InitListControls()
{
	long nLngSel;
	LVCOLUMN vCol;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	///////////////////////////////////////////////////////
	vCol.mask=LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;
	vCol.fmt= LVCFMT_CENTER;
	
	m_listTestShort.DeleteAllItems();

	vCol.cx=70;
	if (nLngSel==0) vCol.pszText = _T(" 冉荐 ");
	else if (nLngSel==1) vCol.pszText = _T(" NUM. ");
	else vCol.pszText = _T(" 猞 ");
	m_listTestShort.InsertColumn(0, &vCol);

	vCol.cx=80;
	if (nLngSel==0) vCol.pszText = _T(" 菊 ");
	else if (nLngSel==1) vCol.pszText = _T(" FRONT ");
	else vCol.pszText = _T(" 铖 ");
	m_listTestShort.InsertColumn(1, &vCol);

	vCol.cx=80;
	if (nLngSel==0) vCol.pszText = _T(" 第 ");
	else if (nLngSel==1) vCol.pszText = _T(" REAR ");
	else vCol.pszText = _T("  ");	
	m_listTestShort.InsertColumn(2, &vCol);

	m_listTestShort.SetExtendedStyle(LVS_EX_GRIDLINES);
	UpdateData(false);
}
*/

void CDlgShortMain::InitListControls()
{
	long nLngSel;
	CString str;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	NUiListDll_SetRows(LIST_SHORT_ID, 40);
	NUiListDll_SetCols(LIST_SHORT_ID, 3);
	NUiListDll_SetRowHeight(LIST_SHORT_ID, 22);
	NUiListDll_SetTitleColor(LIST_SHORT_ID, RGB(180, 180, 180));
	NUiListDll_SetBackColor(LIST_SHORT_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_SHORT_ID, RGB(200, 200, 200));
	NUiListDll_SetFontSize(LIST_SHORT_ID, 16);
	NUiListDll_SetEnableEdit(LIST_SHORT_ID, FALSE);

	NUiListDll_SetColWidth(LIST_SHORT_ID, 0, 82);
	NUiListDll_SetColWidth(LIST_SHORT_ID, 1, 80);
	NUiListDll_SetColWidth(LIST_SHORT_ID, 2, 80);

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	if (nLngSel==0) NUiListDll_SetText(LIST_SHORT_ID, 0, 0, _T(" 횟수 "));
	else if (nLngSel==1) NUiListDll_SetText(LIST_SHORT_ID, 0, 0, _T(" NUM. "));
	else NUiListDll_SetText(LIST_SHORT_ID, 0, 0, _T(" 数量 "));

	if (nLngSel==0) NUiListDll_SetText(LIST_SHORT_ID, 1, 0, _T(" 전 "));
	else if (nLngSel==1) NUiListDll_SetText(LIST_SHORT_ID, 1, 0, _T(" FRONT "));
	else NUiListDll_SetText(LIST_SHORT_ID, 1, 0, _T(" 前面 "));

	if (nLngSel==0) NUiListDll_SetText(LIST_SHORT_ID, 2, 0, _T(" 후 "));
	else if (nLngSel==1) NUiListDll_SetText(LIST_SHORT_ID, 2, 0, _T(" REAR "));
	else NUiListDll_SetText(LIST_SHORT_ID, 2, 0, _T(" 后面 "));	
	
    NUiListDll_Invalidate(LIST_SHORT_ID);
}

void CDlgShortMain::UpdateListControls(BOOL bDecF, long nInxF, float fF, BOOL bDecR, long nInxR, float fR)
{
	long i, nRow, nCnt;
	CString strList[3];
	SYSTEMTIME	lpSystemTime;
	CString str;

	m_pMainFrm->m_Setting.m_nTotalShortCount++;

	GetLocalTime(&lpSystemTime);

	m_IndexData[m_nListIndexPoint] = m_pMainFrm->m_Setting.m_nTotalShortCount;
	m_FrontData[m_nListIndexPoint] = fF;
	m_RearData[m_nListIndexPoint] = fR;
	m_FrontDec[m_nListIndexPoint] = bDecF;
	m_RearDec[m_nListIndexPoint] = bDecR;

	nRow = 1;
	nCnt = 0;
	i=m_nListIndexPoint;
	if (i>=DEF_SHORTLIST_MAX_NUM)
		i = 0;

	while(nCnt<DEF_SHORTLIST_MAX_NUM)
	{
	    if (m_IndexData[i]>=0) 
		{
			str.Format(_T("%6d"), m_IndexData[i]);
			NUiListDll_SetText(LIST_SHORT_ID, 0, nRow, str);

	        if (m_pMainFrm->m_Setting.m_nCOM1)
			{
				str.Format(_T("%5.3f"), m_FrontData[i]);
				NUiListDll_SetText(LIST_SHORT_ID, 1, nRow, str);
				NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, RGB(200, 255, 200));
				if (m_FrontDec[i]) 
					NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, RGB(255, 170, 170));
			}

            if (m_pMainFrm->m_Setting.m_nCOM2)
			{
				str.Format(_T("%5.3f"), m_RearData[i]);
				NUiListDll_SetText(LIST_SHORT_ID, 2, nRow, str);
				NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, RGB(200, 255, 200));
				if (m_RearDec[i]) 
					NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, RGB(255, 170, 170));
			}

			nRow++;
		}

		i++;
		if (i>=DEF_SHORTLIST_MAX_NUM)
			i=0;

		nCnt++;
	}

	m_nListIndexPoint++;
	if (m_nListIndexPoint>=DEF_SHORTLIST_MAX_NUM)
		m_nListIndexPoint = 0;

	NUiListDll_SetRow(LIST_SHORT_ID, nRow);
	NUiListDll_Invalidate(LIST_SHORT_ID);
}

void CDlgShortMain::InitIndexShort()
{
	long i;

	m_nFirstIndex[0] = 0;
	m_nFirstIndex[1] = 0;
	m_nRearIndex[0] = 0;
	m_nRearIndex[1] = 0;

    m_strRemainT[0] = _T(""); 
	m_strRemainT[1] = _T(""); 

	for(i=0; i<SHORT_BUFFER_MAX_NUM; i++)
	{
		m_bGoodDec[0][i] = 0;
		m_bGoodDec[1][i] = 0;

		m_nVoltV[0][i] = 0;
		m_nVoltV[1][i] = 0;

		m_nCurrV[0][i] = 0;
		m_nCurrV[1][i] = 0;

		m_nNumID[0][i] = -1;
		m_nNumID[1][i] = -1;
	}

	m_nListIndexPoint = 0;
	for(i=0; i<DEF_SHORTLIST_MAX_NUM; i++)
	{
		m_IndexData[i] = -1;
		m_FrontData[i] = -1;
		m_RearData[i] = -1;
		m_FrontDec[i] = TRUE;
    	m_RearDec[i] = TRUE;
	}
}

void CDlgShortMain::InitShortProc()
{
	long i;

	InitIndexShort();
	InitListControls();

//	SetShortTestCondition();
	m_bOnRS232C = FALSE;

	for(i=0; i<5; i++)
	{
		NComPort_BufferClear(m_pMainFrm->m_Setting.m_nCOM1);
		NComPort_BufferClear(m_pMainFrm->m_Setting.m_nCOM2);
	}

	m_hEventCOM = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadCOM = AfxBeginThread(ThreadShortCOM, this, THREAD_PRIORITY_NORMAL);
	if (m_pThreadCOM)
		m_pThreadCOM->m_bAutoDelete = TRUE;
}

void CDlgShortMain::CloseShortProc()
{
}

void CDlgShortMain::SetEventCOM()
{
	SetEvent(m_hEventCOM);
}

UINT CDlgShortMain::ThreadShortCOM(LPVOID pParam)
{
	CDlgShortMain *pView = (CDlgShortMain *)pParam;
   	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	static bool bFlag = FALSE;

	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventCOM, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;

//		if (bFlag)
		{ 
		    pView->GetRS232CData();
//			Sleep(5);
		}

		bFlag = !bFlag;
        ResetEvent(pView->m_hEventCOM);
	}

	CloseHandle(pView->m_hEventCOM);
	AfxEndThread(100);

	return 0;
}

void CDlgShortMain::GetRS232CData()
//LONG CDlgShortMain::OnReadRS232CData(UINT wParam, LONG lParam)
{
	BOOL bRetF, bRetR;
	long nRearIndex0, nRearIndex1;
	float fV0, fI0, fV1, fI1;
	float fOhm0, fOhm1;
	char szbufF[500], szbufR[500];
	CString strT, str, strF, strR, strName;
	
	fOhm0 = 0;
	fOhm1 = 0;
	fV0 = 0;
	fV1 = 0;
	fI0 = 0;
	fI1 = 0;

	bRetF = 0;
	bRetR = 0;
	nRearIndex0 = 0;
	nRearIndex1 = 0;

	strcpy_s(szbufF, "");
	strcpy_s(szbufR, "");

    if (m_pMainFrm->m_Setting.m_nCOM1)
	{
		bRetF = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM1, szbufF);
		if (bRetF)
		{
			strF = CharToString(szbufF);
			GetStringFromText(0, strF);

			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(0, TRUE);
		}
		else
			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(0, FALSE);
	}

    if (m_pMainFrm->m_Setting.m_nCOM2)
	{
		bRetR = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM2, szbufR);
		if (bRetR)
		{
			strR = CharToString(szbufR);
			GetStringFromText(1, strR);

			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(1, TRUE);
		}
		else
			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(1, FALSE);
	}
	
//	do
//	{
		if (m_pMainFrm->m_Setting.m_nCOM1)
		{
//			nRearIndex0 = m_nRearIndex[0];
//			if (nRearIndex0 == m_nFirstIndex[0]) break;

			fV0 = (float)m_nVoltV[0][0];
			fI0 = (float)m_nCurrV[0][0]/10000.0f;

			if (fabs(fI0)<0.00001f) 
			{
				fI0 = 0.00001f;
				fOhm0 = 0.0f; 
			}
			else fOhm0 = fV0/fI0;

			WriteShortVToFile(0, m_bGoodDec[0][0], fI0);
		}

		if (m_pMainFrm->m_Setting.m_nCOM2)
		{
//			nRearIndex1 = GetIndexRearSide(m_nNumID[0][nRearIndex0]);
//			if (nRearIndex1<0) break;

			fV1 = (float)m_nVoltV[1][0];
			fI1 = (float)m_nCurrV[1][0]/10000.0f;

			if (fabs(fI1)<0.00001f)
			{
				fI1 = 0.00001f;
				fOhm1 = 0.0f;
			}
			else fOhm1 = fV1/fI1;

			WriteShortVToFile(1, m_bGoodDec[1][0], fI1);
		}
		
		m_pMainFrm->m_fFrontShort = fI1;
		m_pMainFrm->m_fRearShort = fI0;

		UpdateListControls(m_bGoodDec[0][nRearIndex0], m_nNumID[0][0], fI0, m_bGoodDec[1][0], m_nNumID[1][0], fI1);

		m_nVoltV[0][nRearIndex0] = 0;
		m_nCurrV[0][nRearIndex0] = 0;
		m_nNumID[0][nRearIndex0] = -1;

		m_nVoltV[1][nRearIndex1] = 0;
		m_nCurrV[1][nRearIndex1] = 0;
		m_nNumID[1][nRearIndex1] = -1;
		
		if (m_pMainFrm->m_Setting.m_nCOM1)
    		NPlotDll_AppendStickData(DEF_SHORT_PLOT_NUM, 0, 0, fI0);
        if (m_pMainFrm->m_Setting.m_nCOM2)
		    NPlotDll_AppendStickData(DEF_SHORT_PLOT_NUM, 1, 0, fI1);

		NPlotDll_Invalidate(DEF_SHORT_PLOT_NUM);

		nRearIndex0++;
		if (nRearIndex0>=SHORT_BUFFER_MAX_NUM)
			nRearIndex0 = 0;
		m_nRearIndex[0] = nRearIndex0;
//	} while(TRUE);

	str.Format(_T(" F : %s , R : %s "), CharToString(szbufF), CharToString(szbufR));
	m_pMainFrm->WriteTextShortMessage(str);

	//m_bOnRS232C = FALSE;
	//m_bOnRS232C = !m_bOnRS232C;

//	return 1;
}

/*
long CDlgShortMain::GetStringFromText(long nPort, long nLen, char *szbuf)
{
	long i, nPos1, nDataCnt, nIndexPos;
	CString str, strT;

//	str =  strText;
	strT = "";
	nDataCnt = 0;
	nPos1 = 0;

	i=0;
	while(i<nLen)
	{
		if (szbuf[i]==char(0xAB)) 
		{
			nPos1 = i;
			nIndexPos = m_nFirstIndex[nPort];
		
			if (szbuf[nPos1+7]==0x74) m_bGoodDec[nPort][nIndexPos] = true;
			else m_bGoodDec[nPort][nIndexPos] = false;
		
            m_nNumID[nPort][nIndexPos] = _ttoi(str.Mid(nPos1-1, 1));
			m_nVoltV[nPort][nIndexPos] = _ttoi(str.Mid(nPos1+10, 2));
			m_nCurrV[nPort][nIndexPos] = _ttoi(str.Mid(nPos1+12, 4));

			nDataCnt++;
			nIndexPos++;
			if (nIndexPos>=SHORT_BUFFER_MAX_NUM)
				nIndexPos = 0;
			m_nFirstIndex[nPort] = nIndexPos;

			i += 7;
		}

		i++;
	}

	return nDataCnt;
}
*/

long CDlgShortMain::GetStringFromText(long nPort, CString strText)
{
	long i, nPos, nLen, nDataCnt, nIndexPos, nTemp;
	BOOL bFlag;
	TCHAR *endP;
	CString str, strT, strBuff;
	
	str = m_strRemainT[nPort] + strText;
	strBuff = str;

	nDataCnt = 0;
	nPos = 0;
	bFlag = FALSE;
	for(i=0; i<str.GetLength(); i+=2)
	{
		strT = str.Mid(i, 4);
		if (!bFlag && strT==_T("AB70"))
		{
			bFlag = TRUE;
			nPos = i;
			continue;
		}

		if (bFlag && strT==_T("AB70"))
		{
			nIndexPos = m_nFirstIndex[nPort];
			
			strT = str.Mid(nPos+14, 2);
			if (strT!=_T("74")) m_bGoodDec[nPort][0] = true;
			else m_bGoodDec[nPort][0] = false;
			
 //         m_nNumID[nPort][nIndexPos] = _ttoi(str.Mid(nPos+2, 1));
			strT = str.Mid(nPos+20, 2);
			nTemp = _tcstol(strT, &endP, 16);
			m_nVoltV[nPort][0] = nTemp;

			strT = str.Mid(nPos+30, 2) + str.Mid(nPos+28, 2) + str.Mid(nPos+26, 2) + str.Mid(nPos+24, 2);
			nTemp = _tcstol(strT, &endP, 16);
			m_nCurrV[nPort][0] = nTemp;

			nDataCnt++;
			nIndexPos++;
			if (nIndexPos>=SHORT_BUFFER_MAX_NUM)
				nIndexPos = 0;
			m_nFirstIndex[nPort] = nIndexPos;
			
			nLen = str.GetLength()-i;
			strBuff = str.Mid(i, nLen);

			nPos = i;
		}
	}

	m_strRemainT[nPort] = strBuff;
	return nDataCnt;
}

long CDlgShortMain::GetIndexRearSide(long nIndexF)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<SHORT_BUFFER_MAX_NUM; i++)
	{
		if (m_nNumID[1][i] == nIndexF) 
		{
			nPos = i;
			break;
		}
	}

	return nPos;
}

void CDlgShortMain::WriteShortVToFile(BOOL bRear, BOOL bDec, float fV)
{
	FILE  *fp;
	TCHAR   szbuf[1200];
	CString strPathName, strDataPath, str, strT;
	SYSTEMTIME	lpSystemTime;

	GetLocalTime(&lpSystemTime);

	strDataPath = SAVE_SHORTDATA_PATH;

	str.Format(_T("%04d\\%02d\\%02d\\%02d\\"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour);
	strDataPath += str;

	_stprintf_s(szbuf, _T("%s%02d.txt"), strDataPath, lpSystemTime.wMinute);
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
	                  m_pMainFrm->m_strLotID, lpSystemTime.wYear, 
					  lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);

	bRear = FALSE;
	str.Format(_T("%1d %1d %9d"), bRear, bDec, 0);
	strT += str;

	str.Format(_T("%10s %1d %1d %6.3f "), _T("S.T"), 0, bDec, fV);
	strT += str;

	if (m_sysShortOldTime.wHour!=lpSystemTime.wHour)
	{
		CreateDirectoryFullPath(strDataPath);
		_tfopen_s(&fp, szbuf, _T("w+"));
	}
	else
	{
		_tfopen_s(&fp, szbuf, _T("a+"));
		if (!fp)
		{
			CreateDirectoryFullPath(strDataPath);
		    _tfopen_s(&fp, szbuf, _T("w+"));
		}
	}

	if (fp)
	{
		_ftprintf_s(fp, _T("%s\n"), strT);
    	fclose(fp);
	}

	m_sysShortOldTime = lpSystemTime;
}

void CDlgShortMain::SetShortTestCondition()
{
	char szbuf[100];

	if (g_PortCOM[0])
	{
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM1, 'S');

		sprintf_s(szbuf, "%04d", (long)m_pMainFrm->m_Setting.m_fTestVolt);							//전압설정 200
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'A', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fLimitCurr*1000+0.5));//*1000		//제한전류 5
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'B', szbuf);
	
		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fStartDelayTime*100+0.5));//*100	//0 
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'C', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fRampingTime*100+0.5));//*100		//Ramping, 3
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'D', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fHoldingTime*100+0.5));//*100		//Holding 2
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'E', szbuf);
	
		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fContactCurr*1000+0.5));//*1000		//접촉불량 0
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'F', szbuf);
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM1, 'R');
	}

	if (g_PortCOM[1])
	{
		///////PLC에서 한번에 2개를 동시에 콘트롤 하면 아래 수정
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM2, 'S');

		sprintf_s(szbuf, "%04d", (long)m_pMainFrm->m_Setting.m_fTestVolt);					//테스트 볼트(2제거)
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'A', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fLimitCurr*1000+0.5));		//리미트커런트(2제거) *1000
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'B', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fStartDelayTime*100+0.5));	//딜레이타임(2제거) *100
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'C', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fRampingTime*100+0.5));		
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'D', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fHoldingTime*100+0.5));		
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'E', szbuf);
	
		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fContactCurr*1000+0.5));		//콘텍트(2제거) *1000
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'F', szbuf);
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM2, 'R');
	}
}

void CALLBACK NPlotDll_TestShortEvent()
{

}

void CDlgShortMain::OnTimer(UINT_PTR nIDEvent)
{
//	GetRS232CData();
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgShortMain::OnBnClickedButton3()
{
	BOOL bRetF;
	CString strF, strText;
	char szbufF[500];

	strF = _T("");
	m_pMainFrm->WriteTextShortMessage(strF);
	Sleep(200);

	bRetF = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM1, szbufF);
	//strF = ReadComPort(m_pMainFrm->m_Setting.m_nCOM1, &nLen);

	strF = CharToString(szbufF);
	GetStringFromText(0, strF);

	m_pMainFrm->WriteTextShortMessage(_T("F : ") + strF);
}

void CDlgShortMain::OnBnClickedButton4()
{
	BOOL bRetF;
	CString strR, strText;
	char szbufR[500];

	strR = _T("");
	m_pMainFrm->WriteTextShortMessage(strR);
	Sleep(200);

	bRetF = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM2, szbufR);

	strR = CharToString(szbufR);
	GetStringFromText(0, strR);

	m_pMainFrm->WriteTextShortMessage(_T("R : ") + strR);
	GetRS232CData();
}


void CDlgShortMain::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
}

BOOL CDlgShortMain::GetTesterVersion(long nComPort)
{
	long i, nPort, nLen;
	CString str, strT, strReceive;
	BYTE szbuf[1024], szReceive[1024];
	BOOL bRet = TRUE;

	nPort = nComPort-1;
	if (nPort<0) 
		nPort=0;
	
	if (!m_pMainFrm->m_bStatusCOM[nPort])
		return bRet;

//	str.Format(_T("%x%x%x%x%x%x%x%x"), 0xAB, 0x01, 0x70, 0x01, 0x90, 0xFE);
	szbuf[0] = unsigned char(0xAB);
	szbuf[1] = unsigned char(0x01);
	szbuf[2] = unsigned char(0x70);
	szbuf[3] = unsigned char(0x01);
	szbuf[4] = unsigned char(0x90);
	szbuf[5] = unsigned char(0xFE);
	szbuf[6] = '\0';

	m_ctrlCOM[nPort].SendString(szbuf, 6);
	Sleep(5);

	strReceive = _T("");
	nLen = m_ctrlCOM[nPort].ReceiveString(szReceive);
	for(i=0; i<nLen; i++)
	{
		strT.Format(_T("%02X"), szReceive[i]);
		strReceive += strT;
	}

	if (strReceive.Find(_T("4348524F"))>0) //43C 48H 52R 4FO
	    bRet = FALSE;

	return bRet;
}

CString CDlgShortMain::ReadComPort(long nComPort, long *nLen)
{
	long i, nPort;
	CString str, strT, strReceive;
	BYTE szbuf[1024], szReceive[1024];

	strReceive = _T("");
	*nLen = 0;
	nPort = nComPort-1;
	if (nPort<0) 
		nPort=0;
	
	if (!m_pMainFrm->m_bStatusCOM[nPort])
		return 0;

//	str.Format(_T("%x%x%x%x%x%x%x%x"), 0xAB, 0x01, 0x70, 0x03, 0xB1, 0x00, 0xD7, 0x04);
	str.Format(_T("%c%c%c%c%c%c%c%c"), unsigned char(0xAB), unsigned char(0x01), unsigned char(0x70), unsigned char(0x03), 
		                               unsigned char(0xB1), unsigned char(0x00), unsigned char(0xD7), unsigned char(0x04));
	szbuf[0] = unsigned char(0xAB);
	szbuf[1] = unsigned char(0x01);
	szbuf[2] = unsigned char(0x70);
	szbuf[3] = unsigned char(0x03);
	szbuf[4] = unsigned char(0xB1);
	szbuf[5] = unsigned char(0x00);
	szbuf[6] = unsigned char(0xD7);
	szbuf[7] = unsigned char(0x04);
	szbuf[8] = '\0';

	m_ctrlCOM[nPort].SendString(szbuf, 8);
	Sleep(10);
	
	*nLen = m_ctrlCOM[nPort].ReceiveString(szReceive);
	for(i=0; i<*nLen; i++)
	{
		strT.Format(_T("%02X"), szReceive[i]);
		strReceive += strT;
	}

	return strReceive;
}

