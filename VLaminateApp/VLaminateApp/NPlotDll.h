// NPlotDll.h : NPlotDll DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

// CNPlotDllApp
// �� Ŭ������ ������ ������ NPlotDll.cpp�� �����Ͻʽÿ�.
//

// Plot�� �ʱ�ȭ / Open �Լ� 
// nPlotID : (0~14 ��������) 
// pWnd : ����ϰ����ϴ� ������ �ڵ� 
extern "C" _declspec(dllexport) void    NPlotDll_Open(long nPlotID, long pWnd);
extern "C" _declspec(dllexport) void    NPlotDll_SetWindowPtr(long nID, long pWnd);

extern "C" _declspec(dllexport) void    NPlotDll_SetEvent(long nID, LPVOID lpCallBack, long pCallData);

// Plot�� ���� / Close �Լ� 
// nPlotID : (0~14 ��������) 
extern "C" _declspec(dllexport) void    NPlotDll_Close(long nPlotID);

// �ش� Plot ID�� ���� ȭ�� ���� 
extern "C" _declspec(dllexport) void    NPlotDll_ShowSetupDialog(long nPlotID);

// ������ ���� �� ȭ�� UPDATE(ȭ�� �ٽ� �׸���)  
extern "C" _declspec(dllexport) void    NPlotDll_Invalidate(long nPlotID);

// ���� �������� �о�´�
extern "C" _declspec(dllexport) void    NPlotDll_LoadSetupData(long nPlotID, LPCTSTR strPathName);

// ���� PLOT DATA�� �о�´�
extern "C" _declspec(dllexport) BOOL    NPlotDll_LoadPlotData(long nPlotID, LPCTSTR strFullPathName);

// ���� PLOT DATA�� �����Ѵ�.
extern "C" _declspec(dllexport) void    NPlotDll_SavePlotData(long nPlotID, LPCTSTR strFullPathName);

extern "C" _declspec(dllexport) void    NPlotDll_LoadStickData(long nID, LPCTSTR strFullPathName);
extern "C" _declspec(dllexport) void    NPlotDll_SaveStickData(long nID, LPCTSTR strFullPathName);

// ���� PLOT IMAGE DATA FILE�� �����Ѵ�.
extern "C" _declspec(dllexport) void    NPlotDll_SaveImageData(long nID, LPCTSTR strFullPathName);

// nPlotI�� ��� ����Ÿ�� �ʱ�ȭ�Ѵ�. (ó������ �ٽ� ������ �� ������)
extern "C" _declspec(dllexport) void    NPlotDll_InitialData(long nPlotID);

// TY MODE������ �۵��Ǹ�, �ش� AXIS ID(�ش� Remark)���� 
// X Data�� �ð����� ���� Ÿ�̸Ӹ� �����ϰ�, Y Data�� �Է���
extern "C" _declspec(dllexport) void    NPlotDll_AppendDataTY(long nPlotID, long nAxisID, float fDataY);

// XY MODE������ �۵��Ǹ�, �ش� AXIS ID(�ش� Remark)���� 
// X Data, Y Data�� �Է���
extern "C" _declspec(dllexport) void    NPlotDll_AppendDataXY(long nPlotID, long nAxisID, float fDataX, float fDataY);

extern "C" _declspec(dllexport) void    NPlotDll_SetMaxDataNum(long nPlotID, long nDataNum);
extern "C" _declspec(dllexport) void    NPlotDll_AppendStickData(long nPlotID, long nAxisID, float fDataX, float fDataY);

// XY MODE������ �۵��Ǹ�, �ش� AXIS ID(�ش� Remark)���� 
// X Data, Y Data�� ����Ÿ�� �� nDataNum������ŭ �ѹ��� �Է���
extern "C" _declspec(dllexport) void    NPlotDll_SetData(long nPlotID, long nAxisID, long nDataNum, float *fDataX, float *fDataY);
extern "C" _declspec(dllexport) void    NPlotDll_SetPrecisionXY(long nPlotID, long nPrecisionX, long nPrecisionY);

extern "C" _declspec(dllexport) void    NPlotDll_SetGausianData(long nPlotID, float fLSL, float fUSL, long nDataNum, float *fDataX, float *fDataY);
//// �Ʒ� ������ ���� �� PLOT�� ȭ�� UPDATE�� 
//// NPlotDll_Invalidate(long nID)�Լ��� ����Ͽ� �ٽ� �׷��� �մϴ�.

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      ���� X ������ ���� �Լ�      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// ���� X CAPTION TITLE ��Ī 
extern "C" _declspec(dllexport) void    NPlotDll_SetCaptionX(long nPlotID, LPCTSTR strCaption);

// ���� X CAPTION TITLE ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorCaptionX(long nPlotID, long nColor);

// ���� X CAPTION TITLE ��Ʈ(LOGFONT)
extern "C" _declspec(dllexport) void    NPlotDll_SetFontCaptionX(long nPlotID, LOGFONT nFont);

// ���� X CAPTION TITLE ��Ī ��ġ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosCaptionX(long nPlotID, long x, long y);

// ���� X UNIT ���� ��Ī  
extern "C" _declspec(dllexport) void    NPlotDll_SetUnitCaptionX(long nPlotID, LPCTSTR strCaption);

// ���� X UNIT ���� ��Ī ��ġ����  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosUnitCaptionX(long nPlotID, long x, long y);

// ���� X SLIDE �� ����  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideNumX(long nPlotID, long nNum);

// ���� X SLIDE ���� STILE����  
// PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileSlideX(long nPlotID, long nPenStile);

// ���� X SLIDE ���� ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorSlideX(long nPlotID, long nColor);

// ���� X�� �׵θ� SLIDE ������ �β� ����  
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideThickSlideX(long nPlotID, long nThick);

// ���� X�� �׵θ� SLIDE ������ ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideColorSlideX(long nPlotID, long nColor);

// ���� SLIDE MARGIN ����(OFFSET ��)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginL(long nPlotID, long nMargin);

// ���� SLIDE MARGIN ����(OFFSET ��)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginR(long nPlotID, long nMargin);

// ���� SCROLL BAR ��ġ����(Y ��ġ����)  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollBarX(long nPlotID, long nY);

// ���� SCROLL ��ġ���� - X1(0.0%~100.0%), X2(0.0%~100.0%) ��ġ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollX(long nID, float fPosL, float fPosR);

// ���� SCROLL BAR ENABLE(ON=1/OFF=0)  
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableScrollBarX(long nPlotID, BOOL bShow);

// ���� SCROLL BAR ���� ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorScrollBarX(long nPlotID, long nColor);

// ���� SCROLL BAR ���� ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetFColorScrollBarX(long nPlotID, long nColor);

// ���� SCALE TITLE(���� ǥ��)�� X, Y�� ��ġ����   
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScaleCaptionX(long nPlotID, long nX, long nY);

// ���� SCALE CAPTION ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetColorScaleCaptionX(long nPlotID, long nColor);

// ���� SCALE CAPTION FONT ����(LOGFONT)  
extern "C" _declspec(dllexport) void    NPlotDll_SetFontScaleCaptionX(long nPlotID, LOGFONT nfont);

// ���� X PLOT�� �ʱ� RANGE���� 
// dMinRange~dMaxRange : -1000000.0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetRangeX(long nPlotID, double dMinRange, double dMaxRange);

// ���� X PLOT�� �ʱ� Display RANGE���� 
// dDispRange : 0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetDispRangeX(long nPlotID, double dDispRange);
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      ���� X ������ ���� �Լ�      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      ���� Y ������ ���� �Լ�      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// ���� Y CAPTION TITLE ��Ī 
extern "C" _declspec(dllexport) void    NPlotDll_SetCaptionY(long nPlotID, LPCTSTR strCaption);

// ���� Y CAPTION TITLE ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorCaptionY(long nPlotID, long nColor);

// ���� Y CAPTION TITLE ��Ʈ(LOGFONT)
extern "C" _declspec(dllexport) void    NPlotDll_SetFontCaptionY(long nPlotID, LOGFONT nFont);

// ���� Y CAPTION TITLE ��Ī ��ġ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosCaptionY(long nPlotID, long x, long y);

// ���� Y UNIT ���� ��Ī  
extern "C" _declspec(dllexport) void    NPlotDll_SetUnitCaptionY(long nPlotID, LPCTSTR strCaption);

// ���� Y UNIT ���� ��Ī ��ġ����  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosUnitCaptionY(long nPlotID, long x, long y);

// ���� Y SLIDE �� ����  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideNumY(long nPlotID, long nNum);

// ���� Y SLIDE ���� STILE����  
// PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileSlideY(long nPlotID, long nPenStile);

// ���� Y SLIDE ���� ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorSlideY(long nPlotID, long nColor);

// ���� Y�� �׵θ� SLIDE ������ �β� ����  
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideThickSlideY(long nPlotID, long nThick);

// ���� Y�� �׵θ� SLIDE ������ ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetOutsideColorSlideY(long nPlotID, long nColor);

// ���� SLIDE MARGIN ����(OFFSET ��)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginT(long nPlotID, long nMargin);

// ���� SLIDE MARGIN ����(OFFSET ��)  
extern "C" _declspec(dllexport) void    NPlotDll_SetSlideMarginB(long nPlotID, long nMargin);

// ���� SCROLL BAR ��ġ����(X ��ġ����)  
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollBarY(long nPlotID, long nX);

// ���� SCROLL ��ġ���� - Y1(0.0%~100.0%), Y2(0.0%~100.0%) ��ġ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScrollY(long nID, float fPosT, float fPosB);

// ���� SCROLL BAR ENABLE(ON=1/OFF=0)  
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableScrollBarY(long nPlotID, BOOL bShow);

// ���� SCROLL BAR ���� ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorScrollBarY(long nPlotID, long nColor);

// ���� SCROLL BAR ���� ���� ���� (COLORREF) ->(long)���� ����ȯ   
extern "C" _declspec(dllexport) void    NPlotDll_SetFColorScrollBarY(long nPlotID, long nColor);

// ���� SCALE TITLE(����ǥ��)�� X, Y�� ��ġ���� (Left Scale�� ��ġ)
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScaleCaptionLY(long nPlotID, long nX, long nY);

// ���� SCALE TITLE(����ǥ��)�� X, Y�� ��ġ���� (Right Scale�� ��ġ)
extern "C" _declspec(dllexport) void    NPlotDll_SetPosScaleCaptionRY(long nPlotID, long nX, long nY);

// ���� RIGHT SCALE ENABLE(ON=1/OFF=0)   
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableRScale(long nPlotID, BOOL bShow);

// ���� SCALE CAPTION ���� ���� (COLORREF) ->(long)���� ����ȯ    
extern "C" _declspec(dllexport) void    NPlotDll_SetColorScaleCaptionY(long nPlotID, long nColor);

// ���� SCALE CAPTION FONT ����(LOGFONT)  
extern "C" _declspec(dllexport) void    NPlotDll_SetFontScaleCaptionY(long nPlotID, LOGFONT nfont);

// ���� Left Scale Range Y PLOT�� �ʱ� RANGE���� 
// dMinRange~dMaxRange : -1000000.0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetRangeLY(long nPlotID, double dMinRange, double dMaxRange);

// ���� Right Scale Range Y PLOT�� �ʱ� RANGE���� 
// dMinRange~dMaxRange : -1000000.0~1000000.0
extern "C" _declspec(dllexport) void    NPlotDll_SetRangeRY(long nPlotID, double dMinRange, double dMaxRange);
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      ���� Y ������ ���� �Լ�      //////////////////////////
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     PLOT(����) ������ ���� �Լ�     /////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// PLOT MODE ����
// 0:XY MODE, 1:TY MODE
extern "C" _declspec(dllexport) void    NPlotDll_SetPlotMode(long nPlotID, long nMode);

// Remark(����) ǥ�� ��ġ����
// nVertHori : 0(VERT.), 1(HORI.)
// nLoc : 0(LEFT Or Top), 1(Right Or Bottom)
extern "C" _declspec(dllexport) void    NPlotDll_SetLocRemark(long nPlotID, long nVertHori, long nLoc);

// Remark(����) ǥ�ñ���(Split)�� ũ�⸦ ����
extern "C" _declspec(dllexport) void    NPlotDll_SetSizeSplitRemark(long nPlotID, long nSize);

// Plot ������ �������� ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorPlot(long nPlotID, long nColor);

// Remark ������ �������� ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetBColorRemark(long nPlotID, long nColor);

// Remark ������ ������� ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetFColorRemark(long nPlotID, long nColor);

// Remark�� Font�� ����
extern "C" _declspec(dllexport) void    NPlotDll_SetFontRemark(long nPlotID, LOGFONT nFont);

// Remark ������ ����
extern "C" _declspec(dllexport) void    NPlotDll_SetRemarkNum(long nPlotID, long nNum);

// Remark�� ������ ������ ����
extern "C" _declspec(dllexport) void    NPlotDll_SetSpaceRemark(long nPlotID, long nDist);

// Remark�� ������ ������ ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosRemark(long nPlotID, long nX, long nY);

// nAxisID : Remark�� ���� TITLE�� ����
extern "C" _declspec(dllexport) void    NPlotDll_SetCaptionRemark(long nPlotID, long nAxisID, LPCTSTR strCaption);

// Remark�� ���� COLOR�� ���� (COLORREF) ->(long)���� ����ȯ  
extern "C" _declspec(dllexport) void    NPlotDll_SetColorRemark(long nPlotID, long nAxisID, long nColor);

// nAxisID : Remark ���� �׷����� �� ��Ÿ���� ����
// nPen : PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileRemark(long nPlotID, long nAxisID, long nPen);

// nAxisID : Remark ���� �׷����� Left, Right�������� ����
// nSel : 0(Left Scale), 1(Right Scale)
extern "C" _declspec(dllexport) void    NPlotDll_SetSelScaleRemark(long nPlotID, long nAxisID, long nSel);

// bOnOff : ���� Referance Level�� Line�� Ȱ��ȭ(TRUE) �Ǵ� ��Ȱ��ȭ(FALSE)
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableHoriRefLine(long nPlotID, BOOL bOnOff);

// bOnOff : ���� Referance Level�� Line�� Ȱ��ȭ(TRUE) �Ǵ� ��Ȱ��ȭ(FALSE)
extern "C" _declspec(dllexport) void    NPlotDll_SetEnableVertRefLine(long nPlotID, BOOL bOnOff);

// nPosX1, nPosX2 : �� ���� ���� ������ �����Ƿ� �� ������ X��ġ 
// nPosX1�� ���� ����, nPosX2�� ������ ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosVertRefLine(long nID, BOOL bMode, float fPosX1, float fPosX2);

// nPosY1, nPosY2 : �� ���� ���� ������ �����Ƿ� �� ������ Y��ġ 
// nPosY1�� ����� ����, nPosY2�� �Ϻ��� ����
extern "C" _declspec(dllexport) void    NPlotDll_SetPosHoriRefLine(long nID, BOOL bMode, float fPosY1, float fPosY2);

// Referance Line�� �� ��Ÿ���� ����
// nPen : PS_SOLID 0, PS_DASH 1, PS_DOT 2, PS_DASHDOT 3, PS_DASHDOTDOT 4, PS_NULL 5
extern "C" _declspec(dllexport) void    NPlotDll_SetPenStileRefLine(long nPlotID, long nPen);

// Referance Line�� ���� ���� (COLORREF) ->(long)���� ����ȯ 
extern "C" _declspec(dllexport) void    NPlotDll_SetColorRefLine(long nPlotID, long nColor);

// nAxisID : ������ Remark(AXIS ID)�� X PLOT���� �ٸ� Remark(�ٸ� AXIS)�� Y PLOT�� �Բ� ���ϱ� ����
//           X PLOT�� ������ Remark �׸� 
extern "C" _declspec(dllexport) void    NPlotDll_SetSelXAxisRemark(long nPlotID, long nAxisID);
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     PLOT(����) ������ ���� �Լ�     /////////////////////////
////////////////////////////////////////////////////////////////////////////////////


class CNPlotDllApp : public CWinApp
{
public:
	CNPlotDllApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
