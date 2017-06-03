// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE Ŭ�����Դϴ�.
#include <afxodlgs.h>       // MFC OLE ��ȭ ���� Ŭ�����Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

#include "Define.h"
#include <afxcontrolbars.h>

static WCHAR m_strPassWord[100];

static BOOL CreateDirectoryFullPath(CString strPath)
{
	if (::PathIsDirectory(strPath) != 0)
			return TRUE;
	else
	{
		int i=0;
		while(1)
		{
			i = strPath.Find(_T("\\"),i+1);
			if(i<=0)
			{
				CreateDirectory(strPath,0);
				break;
			}
			CreateDirectory(strPath.Left(i+1),0);
		}
	}
	if(::PathIsDirectory(strPath)!=0)
		return TRUE;
	else
		return FALSE;
	return TRUE;
};

// CString �� Char
static void StringToChar(CString str, char *szbuf)
{
    char *szStr = NULL;

#if defined(UNICODE) || defined(_UNICODE)
    int nLen = str.GetLength() + 1;
    TCHAR *tszTemp = NULL;
    tszTemp = new TCHAR[nLen];

    memset(tszTemp, 0x00, nLen*sizeof(TCHAR));
    wcscpy_s(tszTemp, nLen, str);

    // Get size (�������Ǵ¹���Ʈ������)
    int nSize = WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, NULL, NULL, NULL, NULL);
    szStr = new char[nSize];

    memset(szStr, 0x00, nSize);
    WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, szStr, nSize, NULL, NULL);

	strcpy_s(szbuf, nSize, szStr);

	if(tszTemp) delete [] tszTemp;
	if(szStr)   delete [] szStr;
#else
    int nLen = str.GetLength() + 1;
	strcpy_s(szbuf, nLen, str);
#endif
}

// CString �� TCHAR
static TCHAR* StringToTCHAR(CString str)
{
    TCHAR *tszStr = NULL;
    int nLen = str.GetLength() + 1;

    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    _tcscpy_s(tszStr, nLen, str);

    return tszStr;
}

// Char �� CString
static CString CharToString(char *str)
{
    CString cStr;

#if defined(UNICODE) || defined(_UNICODE)

    int nLen = strlen(str) + 1;
    TCHAR *tszTemp = NULL;

    tszTemp = new TCHAR[nLen];
    memset(tszTemp, 0x00, nLen*sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, tszTemp, nLen*sizeof(TCHAR));
    cStr.Format(_T("%s"), tszTemp);

    if(tszTemp)
    {
        delete [] tszTemp;
        tszTemp = NULL;
    }
#else
    cStr.Format("%s", str);
#endif

    return cStr;
}

// Char �� TCHAR
TCHAR* CharToTCHAR(char *str)
{
    TCHAR *tszStr = NULL;

#if defined(UNICODE) || defined(_UNICODE)
    int nLen = strlen(str) + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, tszStr, nLen*sizeof(TCHAR));
#else

    int nLen = strlen(str) + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    strcpy_s(tszStr,nLen, str);
#endif

    return tszStr;
}
 
// TCHAR �� CString
static CString TCHARToString(TCHAR *str)
{
    CString cStr;

    cStr.Format(_T("%s"), str);
    return cStr;
}

// TCHAR �� Char
static char* TCHARToChar(TCHAR *str)
{
    char *szStr = NULL;

#if defined(UNICODE) || defined(_UNICODE)
    int nSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, NULL, NULL, NULL);
    szStr = new char[nSize];
    memset(szStr, 0x00, nSize);
    WideCharToMultiByte(CP_ACP, 0, str, -1, szStr, nSize, NULL, NULL);
#else
    int nLen = strlen(str) + 1;
    szStr = new char[nLen];
    memset(szStr, 0x00, nLen);
    strcpy_s(szStr, nLen, str);
#endif

    return szStr;
}

static CString WithoutSpaceToString(CString strSour)
{
	long i, nStart, nEnd;
	CString str;

	nStart = 0;
	for(i=0; i<strSour.GetLength(); i++)
	{
		if (strSour.Mid(i, 1)!=" ") {
		    nStart = i;
			break;
		}
	}

	nEnd = strSour.GetLength()-1;
	for(i=strSour.GetLength()-1; i>=0; i--)
	{
		if (strSour.Mid(i, 1)!=" ") {
		    nEnd = i;
			break;
		}
	}

	str = strSour.Mid(nStart, nEnd-nStart+1);
	str = str.MakeUpper();

	return str;
}