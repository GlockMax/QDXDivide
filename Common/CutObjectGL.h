#pragma once
#include "OPENGLRENDER_API.h"
#include "math.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "NColor.h"

class OPENGLRENDER_API CutGL
{
public:
	double m_CPlane[4];
	BOOL is_BeginTest;
	BOOL is_EndTest;
public:
	CutGL(void);
	~CutGL(void);
	void NormalVector(double * x, double *y, double *z,
					  double  x1, double y1, double z1,
					  double  x2, double y2, double z2,
					  double  x3, double y3, double z3 )
	{//Функция вычисления нормали к плоскости
		*x = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
		*y = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
		*z = (x2-x1)*(y3-y1)-(y2-z1)*(x3-x1);
		double r = sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));
		*x /=r;
		*y /=r;
		*z /=r;
	}
	void SetCPlane(double *eq_cp);
	void InvertCPlane(void);
	void BeginTest1(void);
	void BeginTest2(void);
	void EndTest(void);

	void CloseCPlane();
	void DrawCPlane(const NColor &Color);
	void EndDrawCPlane(void);
};