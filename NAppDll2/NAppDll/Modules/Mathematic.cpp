// Mathematic.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "Mathematic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMathematic
CMathematic::CMathematic()
{
}

CMathematic::~CMathematic()
{
}

BEGIN_MESSAGE_MAP(CMathematic, CStatic)
	//{{AFX_MSG_MAP(CMathematic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMathematic message handlers
// 3���� �̿��Ͽ� ���� �߽��� ���ϰ� �������� ���ϴ� �Լ��� 
long CMathematic::GetCircleEquation(POINT2 *p1, POINT2 *p2, POINT2 *p3, POINT2 *cp, double *radius) 
{
	double a_value, b_value, c_value;
	POINT2 pp1, pp2, pp3;

	pp1.x = p1->x*p1->x;
	pp1.y = p1->y*p1->y;

	pp2.x = p2->x*p2->x;
	pp2.y = p2->y*p2->y;

	pp3.x = p3->x*p3->x;
	pp3.y = p3->y*p3->y;

	b_value = (p1->x*(pp2.x+pp2.y-pp3.x-pp3.y) + 
		       p2->x*(pp3.x-pp1.x-pp1.y+pp3.y) +
			   p3->x*(pp1.y-pp2.y-pp2.x+pp1.x))/
			  (p1->x*(p3->y-p2->y) + 
			   p2->x*(p1->y-p3->y) +
			   p3->x*(p2->y-p1->y));
	a_value = (pp2.x-pp1.x+pp2.y-pp1.y - p1->y*b_value + p2->y*b_value) /
		      (p1->x - p2->x);
	c_value = -1 * (pp1.x + pp1.y + a_value*p1->x + b_value*p1->y);

	cp->x = (float)(-1*a_value/2);
	cp->y = (float)(-1*b_value/2);

	*radius = sqrt(a_value*a_value + b_value*b_value - 4*c_value) / 2;
   	return 1;
}

double CMathematic::GetDistance2D(POINT2 p1, POINT2 p2)
{
	float a, b;

	a = (p1.x - p2.x);
	b = (p1.y - p2.y);

	return (double)sqrt((double)(a*a + b*b));
}

float CMathematic::GetDistance2D(float x1, float y1, float x2, float y2)
{
	float a, b;

	a = x2-x1;
	b = y2-y1;
	
	return ((float)sqrt(a*a + b*b));
}

double CMathematic::GetDistance2D(CPoint p1, CPoint p2)
{
	float a, b;

	a = (float)(p1.x - p2.x);
	b = (float)(p1.y - p2.y);

	return (double)sqrt((double)(a*a + b*b));
}

// ���Ⱑ ������ ���� ���ϱ�
float CMathematic::GetPerpendicularM(float dA)
{
	float dM;

	dM = dA;
	if (fabs(dA)<0.000001f)
	{
		if (dA<0) dM = -0.000001f;
		else dM = 0.000001f;
	}

	dM = -1/dM;
	return dM;
}

// ������ �������� ���� ���ϴ� �Լ� 
// ptDot : �� 
// ptLine1, ptLine2 : ������ ���� 
// ptPos : ������ 
void CMathematic::GetCrossPointDotLine(POINT2 ptDot, POINT2 ptLine1, POINT2 ptLine2, POINT2 *ptPos)
{
	double dLineM, dDotM;
	double dX, dY, dTemp1, dTemp2;

	dTemp1 = ptLine2.x-ptLine1.x;
	dTemp2 = ptLine2.y-ptLine1.y;

	if (fabs(dTemp1)>fabs(dTemp2))
	{
		dLineM = dTemp2 / dTemp1;
		if (dLineM==0.0)
			dLineM = 0.0000001;

		dDotM = -1.0/dLineM;
		dTemp1 = dLineM-dDotM;
		if (dTemp1==0.0)
			dTemp1 = 0.0000001;

		dX = ((dLineM*ptLine1.x) - (dDotM*ptDot.x) - ptLine1.y + ptDot.y) / dTemp1;
		dY = dLineM * (dX-ptLine1.x) + ptLine1.y;

		ptPos->x = (float)dX;
        ptPos->y = (float)dY;
	}
	else
	{
		dLineM = dTemp1 / dTemp2;
		if (dLineM==0.0)
			dLineM = 0.0000001;

		dDotM = -1.0/dLineM;
		dTemp1 = dLineM-dDotM;
		if (dTemp1==0.0)
			dTemp1 = 0.0000001;

		dX = ((dLineM*ptLine1.y) - (dDotM*ptDot.y) - ptLine1.x + ptDot.x) / dTemp1;
		dY = dLineM * (dX-ptLine1.y) + ptLine1.x;

		ptPos->x = (float)dY;
        ptPos->y = (float)dX;
	}
}

// Line Factor�� �������� ���� ���ϴ� �Լ� 
// ptLine1, ptLine2 : ������ ���� 
// ptPos : ������ 
void CMathematic::GetCrossPointLineEquWithLine(float a, float b, POINT2 ptLine1, POINT2 ptLine2, POINT2 *ptPos)
{
	double line_m;
	double dTemp;

	dTemp = ptLine2.x-ptLine1.x;
	if (dTemp==0.0)
		dTemp = 0.00001;

	line_m = (ptLine2.y-ptLine1.y) / dTemp;

	dTemp = line_m - a;
	if (dTemp==0.0)
		dTemp = 0.00001;

	ptPos->x = (float)(((line_m*ptLine1.x) - ptLine1.y + b) / dTemp);
	ptPos->y = (float)(a * ptPos->x + b);
}

// Line Factor�� �������� ���� ���ϴ� �Լ� 
// ptDot : ���� 
// ptPos : ������ 
void CMathematic::GetCrossPointLineEquWithDot(float a, float b, POINT2 ptDot, POINT2 *ptPos)
{
	double dSlope, dot_m, dTemp;

	dSlope = a;
	if (dSlope==0.0)	
		dSlope = 0.00001;

	dot_m = -1.0/dSlope;
	dTemp = dot_m - dSlope;
	if (dTemp==0.0)
		dTemp = (double)0.00001;

	ptPos->x = (float)(((dot_m*ptDot.x) - ptDot.y + b) / dTemp);
	ptPos->y = (float)(dSlope * ptPos->x + b);
}

// dAngle : ���� ����
// ptDot : ���� 
// ptPos : ������ 
void CMathematic::GetCrossPointDotWithAngle(float dAngle, POINT2 ptDot1, POINT2 ptDot2, POINT2 *ptPos)
{
	double dLineM, dRectM, dTemp;
	double a, b;

	if (fabs(dAngle)<45.0f)
	{
		dLineM = tan(dAngle*RAD);
		if (dLineM==0.0)
			dLineM = 0.0000001;

		dRectM = -1.0/dLineM;
		dTemp = dLineM - dRectM;
		if (dTemp==0.0)
			dTemp = 0.0000001;

		ptPos->x = (float)((dLineM*ptDot1.x - dRectM*ptDot2.x + ptDot2.y - ptDot1.y) / dTemp);
		ptPos->y = (float)(dLineM*ptPos->x - dLineM*ptDot1.x + ptDot1.y);
	}
	else
	{
		dLineM = tan((90.0-dAngle)*RAD);
		if (dLineM==0.0)
			dLineM = 0.0000001;

		dRectM = -1.0/dLineM;
		dTemp = dLineM - dRectM;
		if (dTemp==0.0)
			dTemp = 0.0000001;

		a = (dLineM*ptDot1.y - dRectM*ptDot2.y + ptDot2.x - ptDot1.x) / dTemp;
		b = dLineM*a - dLineM*ptDot1.y + ptDot1.x;

		ptPos->x = (float)b;
		ptPos->y = (float)a;
	}
}

// Line Factor�� �������� ���� ���ϴ� �Լ� 
// ptDot : ���� 
// ptPos : ������ 
void CMathematic::GetCrossPointLineEqu(float a1, float b1, float a2, float b2, float *cx, float *cy)
{
	double dTemp;

	dTemp = a1-a2;
	if (dTemp==0.0)
		dTemp = 0.00001;

	*cx = (float)((b2-b1)/dTemp);
	*cy = (float)(a2*(*cx) + b2);
}

float CMathematic::GetAngleWith2Line(POINT2 ptDot1, POINT2 ptDot2, POINT2 ptDot3, POINT2 ptDot4)
{
	double a1, b1, a2, b2;
	double AB, A1, A2, B1, B2;
	double dTemp, dAng;

	a1 = (ptDot2.x-ptDot1.x)/100.0;
	a2 = (ptDot2.y-ptDot1.y)/100.0;

	b1 = (ptDot4.x-ptDot3.x)/100.0;
	b2 = (ptDot4.y-ptDot3.y)/100.0;
 	
	AB = (a1*b1 + a2*b2);
	A1 = (a1*a1);
	A2 = (a2*a2);
	B1 = (b1*b1);
	B2 = (b2*b2);
	
	dTemp = AB /(sqrt(A1+A2)*sqrt(B1+B2));
	dAng = acos(dTemp)*DEG;

	return (float)dAng;
}

// ������ ���� �����ϴ� ���� ���� 
// p1 : ��
// p2 : ���� �߽� 
// radius : ������ 
// cp : ������ 
void CMathematic::GetDotCircleCrossPoint(POINT2 p1, POINT2 p2, long radius, POINT2 *cp)
{
	double m, dTemp;
	double a, b, c;
	double dist1, dist2;
	POINT2 cp1, cp2;

	dTemp = (p2.x-p1.x);
	if (dTemp==0.0)
		dTemp = 0.0000001;

	m = (p2.y-p1.y) / dTemp;

    a = m*m + 1;
	b = 2*(m*p2.y - m*m*p2.x - p1.x - m*p1.y);
	c = p1.x*p1.x + p2.y*p2.y + m*m*p2.x*p2.x - 2*m*p2.x*p2.y + 2*m*p2.x*p1.y - 2*p1.y*p2.y + p1.y*p1.y - radius*radius;

	dTemp = (2*a);
	if (dTemp==0.0)
		dTemp = 0.000001;

    cp1.x = (float)((-1*b + sqrt(b*b - 4*a*c)) / dTemp);
	cp2.x = (float)((-1*b - sqrt(b*b - 4*a*c)) / dTemp);

	cp1.y = (float)(m*cp1.x - m*p1.x + p1.y);
	cp2.y = (float)(m*cp2.x - m*p1.x + p1.y);

	dist1 = GetDistance2D(cp1, p2);
	dist2 = GetDistance2D(cp2, p2); 

	if (dist2 > dist1) {
		cp->x = cp1.x;
		cp->y = cp1.y;
	}
	else {
		cp->x = cp2.x;
		cp->y = cp2.y;
	}

	return;
}

// �� ������ �������� �̷�� ���� ���ϴ� �Լ� 
// p1 ,p2 : ù��° ����
// p3, p4 : �ι�° ���� 
// cp : �����ϴ� �� 
// �� ������ �̷�� �� 
void CMathematic::GetCrossPoint2Line(POINT2 p1, POINT2 p2, POINT2 p3, POINT2 p4, POINT2 *cp)
{
	double m1, m2, dTemp1, dTemp2;

	dTemp1 = p2.x - p1.x;
	dTemp2 = p4.x - p3.x;

	if (fabs(dTemp1)>fabs(dTemp2))
	{
		if (dTemp1==0.0)
			dTemp1 = 0.00000001;
		m1 = (p2.y-p1.y)/dTemp1;

		if (dTemp2==0.0)
			dTemp2 = 0.00000001;
		m2 = (p4.y-p3.y)/dTemp2;

		dTemp1 = (m1 - m2);
		if (dTemp1==0.0)
			dTemp1 = 0.00000001;

	    cp->x = (float)((m1*p1.x - m2*p3.x + p3.y - p1.y) / dTemp1);
    	cp->y = (float)(m1*(cp->x - p1.x) + p1.y);
	}
	else
	{
		if (dTemp1==0.0)
			dTemp1 = 0.00000001;
		m1 = (p2.y-p1.y)/dTemp1;

		if (dTemp2==0.0)
			dTemp2 = 0.00000001;
		m2 = (p4.y-p3.y)/dTemp2;

		dTemp1 = (m2-m1);
		if (dTemp1==0.0)
			dTemp1 = 0.00000001;

	    cp->x = (float)((m2*p3.x - m1*p1.x + p1.y - p3.y) / dTemp1);
    	cp->y = (float)(m2*(cp->x - p3.x) + p3.y);
	}
}

// ������ ���� ������ �� �������� ���ϴ� �Լ�
// p1, p2 : ����
// p3 : ���� �߽�
// radius : ���� ������ 
// cp1, cp2 : �����ϴ� �� 
void CMathematic::GetLineCircleCrossPoint(POINT2 p1, POINT2 p2, POINT2 p3, long radius, POINT2 *cp1, POINT2 *cp2)
{
	double m;
	double a, b, c;

	m = (p2.y-p1.y) / (p2.x-p1.x);

    a = m*m + 1;
	b = 2*(m*p1.y - m*m*p1.x - p3.x - m*p3.y);
	c = p3.x*p3.x + p3.y*p3.y - radius*radius + p1.y*p1.y + m*m*p1.x*p1.x - 2*m*p1.x*p1.y + 2*m*p3.y*p1.x - 2*p1.y*p3.y;

    cp1->x = (float)((-1*b + sqrt(b*b - 4*a*c)) / (2*a));
	cp2->x = (float)((-1*b - sqrt(b*b - 4*a*c)) / (2*a));

	cp1->y = (float)(m*cp1->x - m*p1.x + p1.y);
	cp2->y = (float)(m*cp2->x - m*p1.x + p1.y);

	return;
}

// �� ���� ������ ���ϴ� �Լ� 
// p1, radius1 : ���� �߽ɰ� �ݰ� 
// p2, radius2 : ���� �߽ɰ� �ݰ� 
// cp1, cp2 : �����ϴ� �� 
void CMathematic::GetCircleCircleCrossPoint(POINT2 p1, float radius1, POINT2 p2, float radius2, POINT2 *cp1, POINT2 *cp2)
{
	double A, B, C;
	double Temp_A, Temp_B;

	Temp_A = -1 * (p2.x-p1.x) / (p2.y-p1.y);
	Temp_B = (radius1*radius1 - p1.x*p1.x - p2.x*p2.x - p1.y*p1.y + p2.y*p2.y - radius2*radius2) / (2*(p2.y - p1.y));

	A = 1+Temp_A*Temp_A;
	B = 2*(Temp_A*Temp_B - Temp_A*p1.y - p1.x);
	C = Temp_B*Temp_B - 2*Temp_B*p1.y + p1.y*p1.y - radius1*radius1 + p1.x*p1.x;

    cp1->x = (float)((-1*B + sqrt(B*B - 4*A*C)) / (2*A));
	cp2->x = (float)((-1*B - sqrt(B*B - 4*A*C)) / (2*A));

	cp1->y = (float)(Temp_A*cp1->x + Temp_B);
	cp2->y = (float)(Temp_A*cp2->x + Temp_B);
}

// ���� ������ ���� ������ ���� 
float CMathematic::GetAngle(POINT2 p1, POINT2 p2)
{
	double dTemp;
	
	dTemp = (p1.x - p2.x);
	if (dTemp==0.0)
		dTemp = 0.00001;

	return (float)(atan((p1.y - p2.y) / dTemp) * DEG);
}

float CMathematic::GetAngle(float dX1, float dY1, float dX2, float dY2)
{
	double dTemp;
	
	dTemp = (dX1 - dX2);
	if (dTemp==0.0)
		dTemp = 0.00001;

	return (float)(atan((dY1 - dY2) / dTemp) * DEG);
}