//N : Edge(Data) ��
//*x , *y  : Edge�� (x, y) Data
//removeN : EllipseFitting�� Ellipse���� ���� �ָ��ִ� ���� ���ְ� �ٽ� fitting�� �ݺ��Ѵ�.
//          �� �ݺ�Ƚ���� ��Ÿ����.
//ErrorLimit: EllipseFitting�� Ellipse���� ���� �ָ��ִ� ���� �̰� �����̸� �ݺ��� �����Ѵ�.
// *Cx, *Cy : Ellipse Center
// *al      : ���� 
// *bl      : ���� 
// *angle   : ������ ���� (-90 ~ 90)

int EllipseData(int N, double *x, double *y, int RemoveN, double ErrorLimit, 
				double *Cx, double *Cy, double *al, double *bl, double *angle);

//SubRoutin ------------------------------------------------------------------------------------------------
int matrixInverse(double *a,int n,double *b);
int	elliFitShape(double *para,double *centX,double *centY, double *a,double *b,double *theta);
int FindLineFunction(double x1, double y1, double x2, double y2, double *a, double *b);

//Recipes---------------------------------------------------------------------------------------------------
double*	vector(int nl,int nh);
double**	matrix(int nrl,int nrh,int ncl,int nch);
void	free_vector(double *v,int nl,int nh);
void	free_matrix(double **m,int nrl,int nrh,int ncl,int nch);

int	jacobi(double **a,int n,double *d,double **v,int *nrot);

int	qromb(double (*func)(double),double a,double b,double *intgrl);

void	svdcmp(double **a,int m,int n,double *w,double **v);

void	svbksb(double **u,double *w,double **v,int m,int n,double *b,double *x);
//--------------------------------------------------------------------------------