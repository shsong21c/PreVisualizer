// Image.h : header file


#ifndef __IMAGE_H__
#define __IMAGE_H__

#define HDIB HGLOBAL

// DIB constants
#define PALVERSION   0x300

// DIB Macros
#define IS_WIN30_DIB(lpbi) ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)  ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)
#define WIDTHBYTES(bits)   (((bits) + 31) / 32 * 4)

//BMP ���� 
//BITMAPFILEHEADER  ---Image Save�Ҷ� ���Ѿ �����Ѵ�.
//BITMAPINFOHEADER  ---|
//BITMAP DATA		---|>	m_hImage


class CImageBMP
{
/******************************************************
						��� ����
******************************************************/
protected:
	LPVOID m_hImage;	//image handle 
	CSize m_Size;		//image size
	CPalette * m_pPal;	//image palette
	

/******************************************************
						��� �Լ�
******************************************************/
public:
	// ������, �ʱ�ȭ �Լ�
	CImageBMP();
    CImageBMP( CImageBMP & );
	BOOL InitDIB(BOOL bCreatePalette = TRUE);
	void SetHandle(HANDLE hHandle);
	BOOL Create(int width, int height, int depth);
	BOOL CreateDIBPalette();

	// �Ҹ���, ���� �Լ�
	virtual ~CImageBMP();
	void Free();


	// �̹��� ������ ��� �Լ�
	int   GetBitCount();	  //Image Depth 
	LPVOID GetHandle()		{return m_hImage;}
	BOOL  IsDataNull()		{return (m_hImage == NULL);}
	CSize GetSize()			{return m_Size;}	
	int   GetHeight()		{return m_Size.cy;}
	int   GetWidth()		{return m_Size.cx;}
	int   GetRealWidth()	{return WIDTHBYTES((GetWidth()*GetBitCount()));}
	CPalette *GetPalette()	{return m_pPal;}
	LPBYTE GetImagePtr();
    void  MemoryDC2BMP(HDC hDC, HBITMAP hbit, TCHAR *Path);
    BOOL  ChangePaletteColor(long nColor);
	CString  FindExtension(const CString& name);
    int      FindType(const CString& ext);
	// �׸���
	BOOL Draw(HDC hDC, LPRECT sourceRect, LPRECT destRect);
	
	// ���� �о����, �����ϱ�
	BOOL     Save(LPCTSTR lpszFileName);
	BOOL     Load(LPCTSTR lpszFileName);

	BOOL     DrawEdgeCircle(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
	BOOL     DrawFillCircle(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
    BOOL     DrawLine(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color);
    BOOL     DrawLine4(CDC *pDC, POINT2 ptStart1, POINT2 ptStart2, POINT2 ptStart3, POINT2 ptStart4, long nThick, COLORREF color);
    BOOL     DrawPoint(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
    BOOL     DrawText(CDC *pDC, long x, long y, LPCTSTR string, long nThick, COLORREF color);
	BOOL     DrawCross(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
	BOOL     DrawRect(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color);
	BOOL     DrawFillRect(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color);
	BOOL     DrawPolyRect(CDC *pDC, long x1, long y1, long x2, long y2, long width, long type, COLORREF color);
	BOOL     DrawPixel(CDC *pDC, long x, long y, long nThick, COLORREF color);
    BOOL     DrawEllipse(CDC *pDC,long lCenterX, long lCenterY, float la, float lb, float angle, long nThick, COLORREF lpColor);
    void     DrawBitmap(CDC* pDC, HBITMAP hBitmap, CRect rcRect);

protected:
	// ���� ���� �о����
	BOOL     LoadBMP(LPCTSTR lpszFileName);
//	BOOL LoadJPG(LPCTSTR lpszFileName);
//	BOOL LoadGIF(LPCTSTR lpszFileName);
//	BOOL LoadTIF(LPCTSTR lpszFileName);

	// ���� ���� �����ϱ�
	BOOL SaveBMP(LPCTSTR lpszFileName);
//	BOOL SaveJPG(LPCTSTR lpszFileName);
//	BOOL SaveGIF(LPCTSTR lpszFileName);
//	BOOL SaveTIF(LPCTSTR lpszFileName);

};

/******************************************************
						���� �Լ�
******************************************************/
LPSTR WINAPI FindDIBBits(LPSTR lpbi);
DWORD WINAPI DIBWidth(LPSTR lpDIB);
DWORD WINAPI DIBHeight(LPSTR lpDIB);
WORD WINAPI PaletteSize(LPSTR lpbi);
WORD WINAPI DIBNumColors(LPSTR lpbi);
/////////////////////////////////////////////////////////////////////////////
#endif