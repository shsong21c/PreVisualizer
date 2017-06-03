// Image.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "ImageBMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/******************************************************
				������, �ʱ�ȭ �Լ�
******************************************************/

CImageBMP::CImageBMP()
{
	m_Size		 = CSize(1,1);
	m_hImage	 = NULL;
	m_pPal		 = NULL;
}

CImageBMP::CImageBMP( CImageBMP &Src )
{
	m_Size		 = CSize(1,1);
	m_hImage	 = NULL;
	m_pPal		 = NULL;

	*this = Src;
}

CImageBMP::~CImageBMP()
{ 
	Free(); 
}

BOOL CImageBMP::InitDIB(BOOL bCreatePalette)
{
	// �̹����� ����, ���� ũ�� ����
	LPSTR pDIB = (LPSTR) m_hImage;
	m_Size = CSize((int) ::DIBWidth(pDIB), (int) ::DIBHeight(pDIB));

	if(bCreatePalette)
	{
		if(m_pPal != NULL) delete m_pPal;
		// �ȷ�Ʈ ����
		m_pPal = new CPalette;
		if (CreateDIBPalette() == NULL)
		{
			// �ȷ�Ʈ�� ������ �ʴ� ���
			delete m_pPal;
			m_pPal = NULL;
			return FALSE;
		}
	}
	return TRUE;
}

void CImageBMP::SetHandle(HANDLE hHandle)
{
	m_hImage = (LPBYTE)hHandle;
} 

LPBYTE CImageBMP::GetImagePtr()
{
	LPBITMAPINFOHEADER lpBI;
	DWORD nSize;

	// �޸� ����
	lpBI = (LPBITMAPINFOHEADER)m_hImage;
	nSize= lpBI->biSize + PaletteSize((LPSTR)lpBI);

	return (LPBYTE)((LPBYTE)m_hImage+nSize);
}

void CImageBMP::MemoryDC2BMP(HDC hDC, HBITMAP hbit, CString strFileName)
{
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    BITMAP bit;
    BITMAPINFO *pih;
    int PalSize;
    HANDLE hFile;
    DWORD dwWritten, Size;

    // ��Ʈ�� ������ ���� ���� ����ü�� �ʱ�ȭ�Ѵ�.
    GetObject(hbit, sizeof(BITMAP), &bit);
    ih.biSize = sizeof(BITMAPINFOHEADER);
    ih.biWidth = bit.bmWidth;
    ih.biHeight = bit.bmHeight;
    ih.biPlanes = 1;
    ih.biBitCount = bit.bmPlanes * bit.bmBitsPixel;
    if(ih.biBitCount > 8) ih.biBitCount = 24;
    ih.biCompression = BI_RGB;
    ih.biSizeImage = 0;
    ih.biXPelsPerMeter = 0;
    ih.biYPelsPerMeter = 0;
    ih.biClrUsed = 0;
    ih.biClrImportant = 0;
    
    // ���� ����ü + �ȷ�Ʈ ũ�⸸ŭ �޸𸮸� �Ҵ��ϰ� �� ���ۿ� ���� ����ü�� �����Ѵ�.
    PalSize = (ih.biBitCount==24 ? 0:1 << ih.biBitCount) * sizeof(RGBQUAD);
    pih = (BITMAPINFO*)malloc(ih.biSize + PalSize);
    pih->bmiHeader = ih;

    // ��Ʈ���� ũ�⸦ ���Ѵ�.
    GetDIBits(hDC, hbit, 0, bit.bmHeight, NULL, pih, DIB_RGB_COLORS);
    ih = pih->bmiHeader;

    // ��Ʈ���� ũ�Ⱑ �������� �ʾ��� ��� ���۾����� ���� ����Ѵ�.
    if(ih.biSizeImage==0) {
        ih.biSizeImage = ((((ih.biWidth*ih.biBitCount)+31) & ~31) >> 3) * ih.biHeight;
    }

    // ������ �����͸� �б����� �޸𸮸� ���Ҵ��Ѵ�.
    Size = ih.biSize + PalSize + ih.biSizeImage;
    pih = (BITMAPINFO *) realloc(pih, Size);

    // ������ �����͸� �о���δ�.
    GetDIBits(hDC, hbit, 0, bit.bmHeight, (PBYTE)pih+ih.biSize+PalSize, pih, DIB_RGB_COLORS);

    // ���� ����� �����.
    fh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PalSize;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfSize = Size+sizeof(BITMAPFILEHEADER);
    fh.bfType = 0x4d42;

    // ������ �����ϰ� ���� ����� ���� ����ü, �ȷ�Ʈ, ������ �����͸� ����Ѵ�.
    hFile = CreateFile((LPCTSTR)strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, &fh, sizeof(fh), &dwWritten, NULL);
    WriteFile(hFile, pih, Size, &dwWritten, NULL);

    CloseHandle(hFile);
	free(pih);
}

BOOL CImageBMP::Create(int width, int height, int depth)
{
    LPBITMAPINFOHEADER lpbi ;
	BYTE		*lpPal;
    DWORD       dwSizeImage;
    long        i, nSize;

//	m_Size.cx=width;
//	m_Size.cy=height;

    dwSizeImage = height*(DWORD)((width*depth/8+3)&~3);

	if(depth == 24)
	{
		nSize=sizeof(BITMAPINFOHEADER)+dwSizeImage;
	}
    else
	{
		nSize=sizeof(BITMAPINFOHEADER)+dwSizeImage + 1024;
	}

	if (m_hImage) delete m_hImage;
	m_hImage= new BYTE[nSize];
	for(i=0; i<nSize; i++)
		m_hImage[i] = 0;

    if (m_hImage == NULL)	
        return FALSE;

	lpbi = (LPBITMAPINFOHEADER)m_hImage;
	lpbi->biSize            = sizeof(BITMAPINFOHEADER) ;
    lpbi->biWidth           = width;
    lpbi->biHeight          = height;
    lpbi->biPlanes          = 1;
    lpbi->biBitCount        = depth;
    lpbi->biCompression     = BI_RGB ;
    lpbi->biSizeImage       = dwSizeImage;
    lpbi->biXPelsPerMeter   = 0 ;
    lpbi->biYPelsPerMeter   = 0 ;
    lpbi->biClrUsed         = 0 ;
    lpbi->biClrImportant    = 0 ;

	lpPal = (BYTE *) lpbi;
	if (depth == 8)
	{
		lpbi->biClrUsed = 256;

		DWORD offDest = sizeof(BITMAPINFOHEADER);
		for(i = 0; i < 256; i++)
		{
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = 0x00;
		}                  
	}
//bitmap depth4
	else if(depth == 4)
	{
		lpbi->biClrUsed = 32;

		DWORD offDest = sizeof(BITMAPINFOHEADER);
		for(i = 0; i < 32; i++)
		{
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = 0x00;
		}                  
	}

	InitDIB(FALSE);
	return TRUE;
}

BOOL CImageBMP::CreateDIBPalette()
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */
	if (m_hImage == NULL)
	  return FALSE;

   lpbi = (LPSTR)m_hImage;

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = new BYTE[sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors];

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) hLogPal;

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = m_pPal->CreatePalette(lpPal);
		
		delete hLogPal;
	}

	return bResult;
}

/******************************************************
				�Ҹ���, ���� �Լ�
******************************************************/

void CImageBMP::Free()
{
	
	if(m_hImage)
	{
		delete m_hImage ;
		m_hImage = NULL;
	}

	if(m_pPal)
	{
		delete m_pPal;
		m_pPal = NULL;
	}
}

/******************************************************
				�̹��� ������ ��� �Լ�
******************************************************/

int CImageBMP::GetBitCount()
{
	if (m_hImage == NULL) return -1;
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)m_hImage;
	return lpbi->biBitCount;
}



/******************************************************
						�׸���
******************************************************/

BOOL CImageBMP::Draw(HDC hDC, LPRECT lpDIBRect, LPRECT lpDCRect)
{
	LPSTR	lpDIBHdr;	// BITMAPINFOHEADER�� ����ų ������
	LPSTR	lpDIBBits;	// DIB ��Ʈ�� ����ų ������
	BOOL		bSuccess=FALSE;	 // Success/fail �÷���
	HPALETTE 	hPal=NULL;		 // DIB �ȷ�Ʈ
	HPALETTE 	hOldPal=NULL;	 // ���� �ȷ�Ʈ

	
	lpDIBHdr  = (LPSTR)m_hImage;
	// DIB ��Ʈ�� ����Ǿ� �ִ� ���� �ּҸ� ����
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// �ȷ�Ʈ�� ��� DC�� ����
	if(m_pPal != NULL)
	{
		hPal = (HPALETTE) m_pPal->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

/* --------------------------------------------------------------------------------------
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		// ���� ũ��� �״�� ����ϴ� ���
		bSuccess = ::SetDIBitsToDevice(hDC, // hDC
			lpDCRect->left,		 			// DestX
			lpDCRect->top,		 			// DestY
			RECTWIDTH(lpDCRect),	 		// nDestWidth
			RECTHEIGHT(lpDCRect),			// nDestHeight
			lpDIBRect->left,		 		// SrcX
			(int)DIBHeight(lpDIBHdr) - lpDIBRect->top -	RECTHEIGHT(lpDIBRect),   		// SrcY
			0,                          	// nStartScan
			(WORD)DIBHeight(lpDIBHdr),  	// nNumScans
			lpDIBBits,                  	// lpBits
			(LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
			DIB_RGB_COLORS);				// wUsage
	else	// Ȯ�� �Ǵ� ����Ͽ� ����ϴ� ���
		bSuccess = ::StretchDIBits(hDC, 	// hDC
			lpDCRect->left,					// DestX
			lpDCRect->top,					// DestY
			RECTWIDTH(lpDCRect),			// nDestWidth
			RECTHEIGHT(lpDCRect),			// nDestHeight
			lpDIBRect->left,				// SrcX
			lpDIBRect->top,					// SrcY
			RECTWIDTH(lpDIBRect),			// wSrcWidth
			RECTHEIGHT(lpDIBRect),			// wSrcHeight
			lpDIBBits,						// lpBits
			(LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
			DIB_RGB_COLORS,					// wUsage
			SRCCOPY);						// dwROP
--------------------------------------------------------------------------------------------*/

	bSuccess = ::StretchDIBits(hDC, 	// hDC
		lpDCRect->left,					// DestX
		lpDCRect->top,					// DestY
		RECTWIDTH(lpDCRect),			// nDestWidth
		RECTHEIGHT(lpDCRect),			// nDestHeight
		lpDIBRect->left,				// SrcX
		lpDIBRect->top,					// SrcY
		RECTWIDTH(lpDIBRect),			// wSrcWidth
		RECTHEIGHT(lpDIBRect),			// wSrcHeight
		lpDIBBits,						// lpBits
		(LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
		DIB_RGB_COLORS,					// wUsage
		SRCCOPY);						// dwROP

	// DC ����
	if (hOldPal != NULL) ::SelectPalette(hDC, hOldPal, TRUE);
	return bSuccess;
}


/******************************************************
				���� �о����, �����ϱ�
******************************************************/
BOOL CImageBMP::Save(LPCTSTR lpszFileName)
{
	CString filetype;
	filetype = lpszFileName;
	filetype.MakeUpper();

	if(filetype.Find(_T(".BMP")) > -1) return SaveBMP(lpszFileName);
//	else if(filetype.Find(".TIF") > -1) return SaveTIF(lpszFileName);
//	else if(filetype.Find(".GIF") > -1) return SaveGIF(lpszFileName);
//	else if(filetype.Find(".JPG") > -1) return SaveJPG(lpszFileName);
	else return FALSE;
}

BOOL CImageBMP::Load(LPCTSTR lpszFileName)
{
	CString filetype;
	filetype = lpszFileName;
	filetype.MakeUpper();

	if(filetype.Find(_T(".BMP")) > -1) return LoadBMP(lpszFileName);
//	else if(filetype.Find(".TIF") > -1) return LoadTIF(lpszFileName);
//	else if(filetype.Find(".GIF") > -1) return LoadGIF(lpszFileName);
//	else if(filetype.Find(".JPG") > -1) return LoadJPG(lpszFileName);
	else return FALSE;
}

/******************************************************
  ���� �׷��� ������ �о���� ��ƾ�� ���� ���Ͽ� ����
 
	ImageFileBmp.cpp : BMP ���� (LoadBMP, SaveBMP)
	ImageFileGif.cpp : GIF ���� (LoadGIF, SaveGIF)
	ImageFileTif.cpp : TIFF ���� (LoadTIF, SaveTIF)
	ImageFileJpg.cpp : JPEG ���� (LoadJPG, SaveJPG)

******************************************************/


/******************************************************
				DIB�� ���õ� ���� �Լ�
******************************************************/

LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}



WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}



WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

BOOL CImageBMP::SaveBMP(LPCTSTR lpszFileName)
{
	CFile file;
	CFileException fe;
	BITMAPFILEHEADER bmfHdr;
	LPBITMAPINFOHEADER lpBI;
	DWORD dwDIBSize;
	
	// ���� ���� ���� ����
	if (!file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &fe)) return FALSE;
	
	// �޸� �ڵ��� ��ȿ���� Ȯ��
	if (m_hImage == NULL) return FALSE;

	lpBI = (LPBITMAPINFOHEADER)m_hImage;
	if (lpBI == NULL) return FALSE;

	// ��Ʈ�� ���� ��� ������ ����
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"
	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);
	if((lpBI->biCompression==BI_RLE8) || (lpBI->biCompression==BI_RLE4))
		dwDIBSize += lpBI->biSizeImage;
	else 
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		dwDIBSize += dwBmBitsSize;
		lpBI->biSizeImage = dwBmBitsSize;
	}

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER)+lpBI->biSize + PaletteSize((LPSTR)lpBI);
	TRY
	{
		// ��Ʈ�� ���� ����� ���Ͽ� ����
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		// ������ �����͸� ���Ͽ� ����
//		file.Write((LPSTR)&lpBI, bmfHdr.bfOffBits - sizeof (BITMAPFILEHEADER));
//		file.Write((LPVOID)m_lpImage, dwDIBSize-bmfHdr.bfOffBits+sizeof (BITMAPFILEHEADER));
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		THROW_LAST();
	}
	END_CATCH
	
	return TRUE;
}


BOOL CImageBMP::LoadBMP(LPCTSTR lpszFileName)
{
	CFile file;
	CFileException fe;
	LPSTR pDIB;
	DWORD dwBitsSize;
	BITMAPFILEHEADER bmfHeader;

	// �б� ���� ���� ����
	if(!file.Open(lpszFileName, CFile::modeRead|CFile::shareDenyWrite, &fe))
		return FALSE;

	// ������ ���̸� ����
	dwBitsSize = (DWORD)file.GetLength();

	// ���� ��� �б�
	if(file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader))!=sizeof(bmfHeader))
		return FALSE;

	// BMP �������� ��Ÿ���� "BM" ��Ŀ�� �ִ��� Ȯ��
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return FALSE;

	// �޸� �Ҵ�
	if (m_hImage) delete m_hImage;
	
	m_hImage = (LPBYTE)new BYTE[dwBitsSize];
	if(m_hImage== NULL) return FALSE;

	pDIB = (LPSTR) m_hImage;

	// ���� �б�
	if (file.Read(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) != dwBitsSize - sizeof(BITMAPFILEHEADER) ) 
	{	
		delete m_hImage;
		return FALSE;
	}

	// DIB �ʱ�ȭ
	InitDIB();

    return TRUE;
}


void CImageBMP::DrawBitmap(CDC* pDC, HBITMAP hBitmap, CRect rcRect)
{
	if(!hBitmap) return;

	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(hBitmap);

	pDC->BitBlt(rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height(), &memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);
}

