#pragma once

#define MORPH_OK  0
#define MORPH_ERROR_WIDTH  1


// CNMorphologyLib
class CNMorphologyLib : public CWnd
{
	DECLARE_DYNAMIC(CNMorphologyLib)

public:
	CNMorphologyLib();
	virtual ~CNMorphologyLib();

	// Binary �Լ��� 
	// White �����Ѵ�.
	long DilateBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
    // White �����Ѵ�.
    long ErodeBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// White �������� �ִ� ���� ������ ��������.( Dilate + Erode)
	long CloseBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// White �ٱ��� ���� White���� ��������. (Erode + Dilate)
	long OpenBinary(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
    // Gray �Լ��� 
	// ȭ���� ��ü�� ��� ����.
    long DilateGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// ȭ���� ��ü�� ��ο�����.
    long ErodeGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// ������ ���� �κ��� �մ� ������� Smoothing�Ǵ� ȿ���� �ִ�
	// �� Black ����� ��������.
	long CloseGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);
	// ������ ����� ������ �մ� ������� Smoothing�Ǵ� ȿ���� �ִ�.
	// �� White ����� ��������.
	long OpenGray(LPBYTE fmSour, LPBYTE fmDest, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long repeat);

private:
	long m_Black;
	long m_White;

// fmSour : �ҽ� ������ ���۹���
// fmDest : Ÿ�� ������ ���۹���
// nWidth : �ҽ������� ��       (ex 640)
// nHeight : �ҽ������� ����     (ex 480)
// nPitch :  �ҽ������� �޸� ��(ex 1024)
// pKernel : kernel�� ���۹���
// nKernelSizeX: kernel�� X���� ũ��
// nKernelSizeY: kernel�� Y���� ũ�� 

protected:
	DECLARE_MESSAGE_MAP()
};


