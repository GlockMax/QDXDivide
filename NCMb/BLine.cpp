// BLine.cpp: implementation of the BLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BLine.h"
#include "BMatr.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BLine::BLine()
{

}

BLine::~BLine()
{

}

// задание линии
void BLine::Set(BPoint p0, BPoint p1, BPoint p2, BPoint p3)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
	p[3] = p3;
}

// задание линии
void BLine::Set(BLine nl)
{
	for (int i=0; i<4; i++)
		p[i] = nl.p[i];
}

// умножение линии на матрицу
BLine BLine::operator *(const BMatr m) const
{
	BLine r;
	r.p[0] = p[0]*m;
	r.p[1] = p[1]*m;
	r.p[2] = p[2]*m;
	r.p[3] = p[3]*m;
	return r;
}

// определение точки на линии
BPoint BLine::GetPointFromParam(double t)
{
	double a,b,c,d;
	double x,y,z,h;
	BPoint r;
	a=(1-t)*(1-t);
	b=2*t*(1-t)*(1-t);
	c=2*t*t*(1-t);
	d=t*t;
	h=a*p[0].GetH()+b*p[1].GetH()+c*p[2].GetH()+
		d*p[3].GetH();
	x=a*p[0].GetX()+b*p[1].GetX()+c*p[2].GetX()+
		d*p[3].GetX();
	y=a*p[0].GetY()+b*p[1].GetY()+c*p[2].GetY()+
		d*p[3].GetY();
	z=a*p[0].GetZ()+b*p[1].GetZ()+c*p[2].GetZ()+
		d*p[3].GetZ();
	r.Set(x,y,z,h);
	return r;
}


void BLine::Draw()
{
	GLfloat ctrlpoints[4][4];
	
	for (int i=0; i<4; i++)
	{
		ctrlpoints[i][0] = (float)p[i].GetX();
		ctrlpoints[i][1] = (float)p[i].GetY();
		ctrlpoints[i][2] = (float)p[i].GetZ();
		ctrlpoints[i][3] = (float)p[i].GetH();
	}

	glMap1f(GL_MAP1_VERTEX_4, 
		0.0, 1.0, 4, 4, 
		&ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_4);

	glBegin(GL_LINE_STRIP);
	  for (i = 0; i <= 30; i++) 
		 glEvalCoord1f((GLfloat) i/30.f);
	glEnd();

/*	BPoint pt, pt1;
	int num_line = 20;

	pt = GetPointFromParam(0);
	pt.Norm();
	for (int j=1; j<=num_line; j++)
	{
		if (j > 1)
			pt = pt1;
		pt1 = GetPointFromParam((double)j/num_line);
		pt1.Norm();
		
		glBegin(GL_LINES);
			glVertex3d(pt.GetX(), pt.GetY(), pt.GetZ());
			glVertex3d(pt1.GetX(), pt1.GetY(), pt1.GetZ());
		glEnd();
	}*/
}

// определение касательной к кривой по параметру t
BPoint BLine::Tangent(double t)
{
	BMatr M;
	BPoint T, pt_par, pt, dT, Lt, temp;

	pt_par = GetPointFromParam(t);
	pt = pt_par;
	pt.Norm();

	dT.Set(0,1,2*t,3*t*t);
	M.Set(1., 0., 0.,0.,	// матрица коэффициентов базиса
		 -2., 2., 0.,0.,	// Безье
		  1.,-4., 2.,1.,
		  0., 2.,-2.,0.);

	temp = dT*M;
	Lt = p[0]*temp.GetX()+p[1]*temp.GetY()+
		 p[2]*temp.GetZ()+p[3]*temp.GetH();

	T = (Lt - pt*Lt.GetH())*(1/pt_par.GetH());

	return T;
}

void BLine::Ball2Bernstain()
{
	p[1] = p[0]*(1./3.)+p[1]*(2./3.);
	p[2] = p[3]*(1./3.)+p[2]*(2./3.);
}

double BLine::GetCurve(double t)
{
	double res;
	
	BPoint rt, rtt, pt, r;

	rt = Tangent(t);

	rtt = GetRtt(t);
		
	pt = rt%rtt;
	res = sqrt(rt.D2());
	pt = pt*(1/(res*res*res));
	res = sqrt(pt.D2());

	return res;
}

// определение вотрой производной кривой Rtt
BPoint BLine::GetRtt(double t)
{
	double a,b,c,d;
	double x,y,z,h;
	
	BPoint rt, rtt, param, pt, r;

	param = GetPointFromParam(t);
	r = param;
	r.Norm();

	a=-2*(1-t);
	b=2*(1-t)*(1-t)-4*(1-t)*t;
	c=4*(1-t)*t-2*t*t;
	d=2*t;
	h=a*p[0].GetH()+b*p[1].GetH()+c*p[2].GetH()+
		d*p[3].GetH();
	x=a*p[0].GetX()+b*p[1].GetX()+c*p[2].GetX()+
		d*p[3].GetX();
	y=a*p[0].GetY()+b*p[1].GetY()+c*p[2].GetY()+
		d*p[3].GetY();
	z=a*p[0].GetZ()+b*p[1].GetZ()+c*p[2].GetZ()+
		d*p[3].GetZ();
	pt.Set(x,y,z,h);
	
	rt = (pt-r*pt.GetH())*(1/param.GetH());
	
	a=2;
	b=-8*(1-t)+4*t;
	c=4*(1-t)-8*t;
	d=2;
	h=a*p[0].GetH()+b*p[1].GetH()+c*p[2].GetH()+
		d*p[3].GetH();
	x=a*p[0].GetX()+b*p[1].GetX()+c*p[2].GetX()+
		d*p[3].GetX();
	y=a*p[0].GetY()+b*p[1].GetY()+c*p[2].GetY()+
		d*p[3].GetY();
	z=a*p[0].GetZ()+b*p[1].GetZ()+c*p[2].GetZ()+
		d*p[3].GetZ();
	rtt.Set(x,y,z,h);
	
	rtt = (rtt-rt*pt.GetH()*2-r*rtt.GetH())*(1/param.GetH());

	return rtt;
}

// построение дуги окружности (угол от 0 до 180  в рад.)
// по заданным крайним точкам и центру окружности
BOOL BLine::BuildBallArc(double alf, double R)
{
	// у линии должны быть определены точки
	// p[0] - начальная точка дуги
	// p[3] - конечная точка дуги
	// точка центра окружности находиться в точке p[1]
	// по умолчанию значение угла поворота дуги -1
	if (p[0].Dist2(p[0], p[3]) <= 0)
		return FALSE;

	double d, h;
	BPoint a, b, tmp;

	a = p[0].Norm()-p[1].Norm();
	b = p[3].Norm()-p[1].Norm();
	double R2 = a.D2();
	if (R == 0)
		R = sqrt(R2);
	if (alf == -1)
	{
		tmp = a+b;
		tmp = tmp*(1./sqrt(tmp.D2()));
		double d2 = (p[3] - p[0]).D2();
		double d = 2. * R2/sqrt(4. * R2 - d2 );
		tmp = p[1] + tmp*d;

		double h = 0.5 * sqrt(d2 / (tmp - p[0]).D2());
			
		p[1] = p[2] = tmp*h;
		return TRUE;
	}


	tmp = a+b;
	tmp = tmp*(1./sqrt(tmp.D2()));
	d = R/cos(alf/2.);
	tmp = p[1] + tmp*fabs(d);
	tmp.Norm();

	h = sqrt(tmp.Dist2(p[0], p[3])/tmp.Dist2(p[0],tmp))*0.5;
		
	p[1] = p[2] = tmp*h;

	return TRUE;
}

void BLine::DrawHr()
{
	glBegin(GL_LINES);
		glVertex3d(p[0].GetX(), p[0].GetY(), p[0].GetZ());
		glVertex3d(p[1].GetX(), p[1].GetY(), p[1].GetZ());
	glEnd();
	glBegin(GL_LINES);
		glVertex3d(p[1].GetX(), p[1].GetY(), p[1].GetZ());
		glVertex3d(p[2].GetX(), p[2].GetY(), p[2].GetZ());
	glEnd();
	glBegin(GL_LINES);
		glVertex3d(p[2].GetX(), p[2].GetY(), p[2].GetZ());
		glVertex3d(p[3].GetX(), p[3].GetY(), p[3].GetZ());
	glEnd();
}
