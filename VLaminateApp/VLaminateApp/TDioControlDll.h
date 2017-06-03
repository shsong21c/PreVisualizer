// TDioControlDll.h : main header file for the TDioControlDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define  PIO_PORT_NUM        16
#define  _RS232C_DIO          0
#define  _PCI_AJIN_EXT        1
#define  _PCI_7230            2
#define  _PCI_1730U           3


typedef struct
{
    long    m_nDioID;
	long    m_arrPortNum[PIO_PORT_NUM];
	BOOL    m_arrOnOff[PIO_PORT_NUM];
	DWORD   m_dwFlag;
} TDioDll_CallDATA;


// CTDioControlDllApp
// See TDioControlDll.cpp for the implementation of this class
//
extern "C" _declspec(dllexport) BOOL    TDioControlDll_Open(long nCtrlBD);
extern "C" _declspec(dllexport) BOOL    TDioControlDll_RS232C(long nPort, long nBaud, long nSize, long nParity, long nStop);
extern "C" _declspec(dllexport) void    TDioControlDll_SetEvent(LPVOID lpCallBack, long pCallData);
extern "C" _declspec(dllexport) void    TDioControlDll_Close();
extern "C" _declspec(dllexport) void    TDioControlDll_Write(long nData);
extern "C" _declspec(dllexport) void    TDioControlDll_BitWrite(long nBit, BOOL bSig);
extern "C" _declspec(dllexport) long    TDioControlDll_Read();
extern "C" _declspec(dllexport) BOOL    TDioControlDll_BitRead(long nBit);
extern "C" _declspec(dllexport) BOOL    TDioControlDll_BitReadABS(long nBit);
extern "C" _declspec(dllexport) void    TDioControlDll_SetLight(long nCh, long nOnOff);
extern "C" _declspec(dllexport) void    TDioControlDll_SetLightValue(long nCh, long nLightValue);
extern "C" _declspec(dllexport) void    TDioControlDll_Exit();


class CTDioControlDllApp : public CWinApp
{
public:
	CTDioControlDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
