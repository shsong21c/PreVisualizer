#if !defined(_AXISPLOT_H)
#define _AXISPLOT_H

// Control Tab Position
#define CONTROL_TAB_LEFT       10
#define CONTROL_TAB_TOP        10
#define CONTROL_TAB_RIGHT     300
#define CONTROL_TAB_BOTTOM    200
// Control Tab Position

// Control Frame Position
#define CONTROL_FRAME_LEFT     15
#define CONTROL_FRAME_TOP      30
#define CONTROL_FRAME_RIGHT   295
#define CONTROL_FRAME_BOTTOM  195
// Control Frame Position

typedef struct AXIS_PLOT_
{
	TCHAR     m_sTitleX[100];        // X Title String                
	TCHAR     m_sTitleY[100];        // Y Title String 
	CRect     m_rTitleX;             // X Title ��ġ  (���������� ����)
	CRect     m_rTitleY;             // Y Title ��ġ  (���������� ����)
	CRect     m_rClient;             // �������� ��¿���   
    CRect     m_rPlot;               // Plot�� ��ġ   (���������� ����)
	CRect     m_rChart;              // ������ ��ġ   (���������� ����)

	COLORREF  m_colorGridLine;       // Grid Line�� ��     
	COLORREF  m_colorSliderLine;     // Slider Line�� �� 
	COLORREF  m_colorBkClient;       // Background Client�� �� 
	COLORREF  m_colorBkPlot;         // Background Plot�� �� 
	COLORREF  m_colorTitleStr;       // Title�� �� 
	COLORREF  m_colorPlotStr;        // Plot String�� �� 
	COLORREF  m_colorUpLine;        // Plot String�� �� 
	COLORREF  m_colorDownLine;        // Plot String�� �� 
	COLORREF  m_colorDataLine;        // Plot String�� �� 

    long      m_nLeftMargin;         // ���� ����. 
	long      m_nTopMargin;          // ��� ���� 
	long      m_nRightMargin;        // ������ ���� 
	long      m_nBottomMargin;       // �Ʒ��� ���� 

	float     m_dScaleX;             // X Plot�� ������
	float     m_dScaleY;             // Y Plot�� ������

	long      m_nRangeMin;          // Range Min X�������� ������ 
	long 	  m_nRangeMax;          // Range Max X�������� ������ 

	long   	  m_nSlidePerPlot;      // Plot�� X Slide���� 
	long 	  m_nPixelDistGrid;     // X Pixel���� -> �Ѱ��� Grid

	long      m_nPlotStrSize;        // Plot String ��Ʈ Size  
	long      m_nTitleStrSize;       // X, Y Title String ��Ʈ Size 

	long      m_nScrollX;
    long      m_nZoomY;
	long      m_nBaseY;
} AXIS_PLOT;
// Graph�� ����Ÿ�� ���õ� ����ü -------------------------------------------

#endif 