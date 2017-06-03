#pragma once


// CNTransformLib
class CNTransformLib : public CWnd
{
	DECLARE_DYNAMIC(CNTransformLib)

public:
	CNTransformLib();
	virtual ~CNTransformLib();

	/////////////////////////////////////////////////////////////////////////////
	// CTransformLib message handlers
	// fm : Frame Memory ���۹���
	// (left, top)-(right,bottom) : ROI
	// nPitch : Frame �� 
	// nSlope : �ּ� Edge Slope 
	// (*Cx, *Cy) : Circle Center
	// return 1: Success
	//        <0 : Failure
	long HoughCircle(unsigned char *fm, long left, long top, long right, long bottom, long nObjectColor, long nDiameter, double *cx, double *cy, long nWidth, long nHeight);

	// Digital ȸ�� �Լ�
	// SourImage : �� ������ �޸� 
	// DestImage : ��ȯ�� ���� �޸� 
	// x1, y1, x2, y2 : ��ȯ�ϰ����ϴ� ������ ��ġ 
	// angle : ȸ������ ���� 
	void ImageRotation(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, float angle, long nWidth, long nHeight);

	// Digital Zoom �Լ�
	// SourImage : �� ������ �޸� 
	// DestImage : ��ȯ�� ���� �޸� 
	// x1, y1, x2, y2 : ��ȯ�ϰ����ϴ� ������ ��ġ 
	// zoom_x : X������ Zooming Scale 
	// zoom_y : Y������ Zooming Scale 
	void ImageZoom(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, float zoom_x, float zoom_y, long nWidth, long nHeight);

	// ���� ���簢������ ��� Open�Լ�(���� �ʱ�ȭ)
	// cx, cy : ���� �߽� ��ǥ 
	// start_angle, end_angle : ���簢������ ��ȯ�ϰ��� �ϴ� �ʱ� ������ ���� ���� 
	// in_radius, out_radius : ���� �߽ɿ��� ���� �ݰ� �̻���� ���� �ݰ������ ���簢�� ���̷� ��ȯ 
	// interval_angle : ���ø� ���� ���� ����     
	void ImageRectToPolarOpen(long nPosX, long nPosY, float dStartAngle, float dEndAngle, float dInRadius, float dOutRadius, float dIntervalAngle,
							  long *nWidth, long *nHeight);

	// ���� ���簢������ ��� �Լ�(�޸� ����, Close �Լ�)
	void ImageRectToPolarClose();

	// ���� ���簢������ ��� �Լ�(�� �Լ�)
	// SourImage : �� ���� �޸� 
	// DestImage : ��ȯ�� ����  
	// rect_height : ��ȯ�� ������ ���� 
	// rect_width : ��ȯ�� ������ �� 
	void ImageRectToPolar(LPBYTE pSourImage, long nOrgWidth, long nOrgHeight, LPBYTE pDestImage, long nPolWidth, long nPolHeight);
	void ExecTransTiltToRect(LPBYTE pSourImage, POINT2 ptPos1, POINT2 ptPos2, POINT2 ptPos3, POINT2 ptPos4, long nWidth, long nHeight);
	LPBYTE OpenTransTiltToRect(POINT2 ptPos1, POINT2 ptPos2, POINT2 ptPos3, POINT2 ptPos4, long *nWidth, long *nHeight);
	void CloseTransTiltToRect();


public:
	// ������ �������� ���簢������ ��ȯ�ϴ� ���� ���� 
	LPBYTE m_pImageBuffer;
	LPBYTE m_pLocalImage;
	long   m_nCenterX;
	long   m_nCenterY;

	float  m_dStartAngle;
	float  m_dEndAngle;
	float  m_dInRadius;
	float  m_dOutRadius;
	float  m_dIntervalAngle;
    //////////////////////////////////////////////////

protected:
	DECLARE_MESSAGE_MAP()
};


