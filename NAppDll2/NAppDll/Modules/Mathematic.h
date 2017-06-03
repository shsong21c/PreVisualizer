#if !defined(AFX_MATHEMATIC_H__7081DF50_7F0E_43C4_826B_8C64CCA0DDBA__INCLUDED_)
#define AFX_MATHEMATIC_H__7081DF50_7F0E_43C4_826B_8C64CCA0DDBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mathematic.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMathematic window
class CMathematic : public CStatic
{
public:
	CMathematic();

// Attributes
public:
	// ���� ������ �Ÿ� ���ϴ� �Լ� 
    double GetDistance2D(POINT2 p1, POINT2 p2);
    double GetDistance2D(CPoint p1, CPoint p2);
    float  GetDistance2D(float x1, float y1, float x2, float y2);


	// ���� ������ ���� ������ ���� 
	float  GetAngle(POINT2 p1, POINT2 p2);
	float  GetAngle(float dX1, float dY1, float dX2, float dY2);
	float  GetPerpendicularM(float dA);

	// ������ ���� �����ϴ� ���� ���� 
    // p1 : ��
    // p2 : ���� �߽� 
    // radius : ������ 
    // cp : ������ 
    void   GetDotCircleCrossPoint(POINT2 p1, POINT2 p2, long radius, POINT2 *cp);

	// 3���� �̿��Ͽ� ���� �߽��� ���ϰ� �������� ���ϴ� �Լ��� 
	long   GetCircleEquation(POINT2 *p1, POINT2 *p2, POINT2 *p3, POINT2 *cp, double *radius) ;   
	long   GetEllipseEquation(int N, double *x, double *y, int RemoveN, double ErrorLimit, 
				double *Cx, double *Cy, double *al, double *bl, double *angle);

    // ������ �������� ���� ���ϴ� �Լ� 
    // p1 : �� 
    // p2, p3 : ������ ���� 
    // cp : ������ 
    void   GetCrossPointDotLine(POINT2 ptDot, POINT2 ptLine1, POINT2 ptLine2, POINT2 *ptPos);
    void   GetCrossPointLineEquWithLine(float a, float b, POINT2 ptLine1, POINT2 ptLine2, POINT2 *ptPos);
    void   GetCrossPointLineEquWithDot(float a, float b, POINT2 ptDot, POINT2 *ptPos);
    void   GetCrossPointDotWithAngle(float dAngle, POINT2 ptDot1, POINT2 ptDot2, POINT2 *ptPos);
    void   GetCrossPointLineEqu(float a1, float b1, float a2, float b2, float *cx, float *cy);
    float  GetAngleWith2Line(POINT2 ptDot1, POINT2 ptDot2, POINT2 ptDot3, POINT2 ptDot4);
    float  GetDistanceDot2Line(POINT2 ptDot, POINT2 ptLine1, POINT2 ptLine2);

    // �� ������ �������� �̷�� ���� ���ϴ� �Լ� 
    // p1 ,p2 : ù��° ����
    // p3, p4 : �ι�° ���� 
    // cp : �����ϴ� �� 
    // �� ������ �̷�� �� 
    void   GetCrossPoint2Line(POINT2 p1, POINT2 p2, POINT2 p3, POINT2 p4, POINT2 *cp);

	// ������ ���� ������ �� �������� ���ϴ� �Լ�
    // p1, p2 : ����
    // p3 : ���� �߽�
    // radius : ���� ������ 
    // cp1, cp2 : �����ϴ� �� 
    void   GetLineCircleCrossPoint(POINT2 p1, POINT2 p2, POINT2 p3, long radius, POINT2 *cp1, POINT2 *cp2);

    // �� ���� ������ ���ϴ� �Լ� 
    // p1, radius1 : ���� �߽ɰ� �ݰ� 
    // p2, radius2 : ���� �߽ɰ� �ݰ� 
    // cp1, cp2 : �����ϴ� �� 
    void   GetCircleCircleCrossPoint(POINT2 p1, float radius1, POINT2 p2, float radius2, POINT2 *cp1, POINT2 *cp2);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMathematic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMathematic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMathematic)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATHEMATIC_H__7081DF50_7F0E_43C4_826B_8C64CCA0DDBA__INCLUDED_)
