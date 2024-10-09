// NLine.cpp: implementation of the NLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NLine.h"
#include "BMatr.h"
#include "math.h"
#include "constdef.h"
#include "BPointsBuf.h"
#include "nline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define STKT stk1[kst]

const int NSTK = 32;
NLine stk1[NSTK];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NLine::NLine()
{

}

NLine::~NLine()
{

}

// задание линии
void NLine::Set(BPoint p0, BPoint p1, BPoint p2, BPoint p3)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
	p[3] = p3;
}

// умножение линии на матрицу
NLine NLine::operator *(const BMatr m) const
{
	NLine r;
	r.p[0] = p[0]*m;
	r.p[1] = p[1]*m;
	r.p[2] = p[2]*m;
	r.p[3] = p[3]*m;
	return r;
}

// определение точки на линии
BPoint NLine::GetPointFromParam(double t) const
{
	double a, b, c, d;
	a = (1. - t) * (1. - t);
	b = 2 * t * a;
	d = t * t;
	c = 2 * d * (1. - t);
	return p[0] * a + p[1] * b + p[2] * c + p[3] * d;
}


void NLine::Draw() const
{
	GLfloat ctrlpoints[4][4];
	
	for (int i = 0; i < 4; i++)
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
	for (int k = 0; k <= 30; k++) 
		glEvalCoord1f((GLfloat) k/30.f);
	glEnd();
/*
	BPoint pt, pt1;
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
			glVertex3d(pt.GetX()/pt.GetH(), pt.GetY()/pt.GetH(), pt.GetZ()/pt.GetH());
			glVertex3d(pt1.GetX()/pt.GetH(), pt1.GetY()/pt.GetH(), pt1.GetZ()/pt.GetH());
		glEnd();
	}
	*/
}

// определение касательной к кривой по параметру t
BPoint NLine::Tangent(double t)
{
	BMatr M;
	BPoint pt_par = GetPointFromParam(t);
	BPoint pt = pt_par;
	pt.Norm();

	BPoint dT(0., 1., 2*t, 3*t*t);
	M.Set(1., 0., 0.,0.,	// матрица коэффициентов базиса
		 -2., 2., 0.,0.,	// Болла
		  1.,-4., 2.,1.,
		  0., 2.,-2.,0.);

	BPoint temp = dT*M;
	BPoint Lt = p[0]*temp.GetX()+p[1]*temp.GetY()+
		 p[2]*temp.GetZ()+p[3]*temp.GetH();

	BPoint T = (Lt - pt*Lt.GetH())*(1/pt_par.GetH());

	return T;
}

void NLine::Ball2Bernstain()
{
	p[1] = p[0]*(1./3.)+p[1]*(2./3.);
	p[2] = p[3]*(1./3.)+p[2]*(2./3.);
}
void NLine::Bernstain2Ball()
{
	p[1] = p[1]*(3./2.) - p[0]*(1./2.);
	p[2] = p[2]*(3./2.) - p[3]*(1./2.);
}

double NLine::GetCurvature(double t)
{
	BPoint rt = Tangent(t);
	double rtD2 = rt.D2();
		
	BPoint pt = rt % GetRtt(t);

	return sqrt(pt.D2() / (rtD2 * rtD2 * rtD2));
}

// определение второй производной кривой Rtt
BPoint NLine::GetRtt(double t)
{
	double a,b,c,d;
	double x,y,z,h;
	
	BPoint param = GetPointFromParam(t);
	BPoint r = param;
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
	BPoint pt(x,y,z,h);
	
	BPoint rt = (pt-r*pt.GetH())*(1/param.GetH());
	
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
	BPoint rtt(x,y,z,h);
	
	rtt = (rtt-rt*pt.GetH()*2-r*rtt.GetH())*(1/param.GetH());

	return rtt;
}

// построение дуги окружности (угол от 0 до 180 исключительно в рад.)
// по заданным крайним точкам и центру окружности
// In XY plane only
BOOL NLine::BuildBallArc(double alf, double R)
{

	// у линии должны быть определены точки
	// p[0] - начальная точка дуги
	// p[3] - конечная точка дуги
	// точка центра окружности находится в точке p[1]
	// по умолчанию значение угла поворота дуги -1
	double l2 = (p[0] - p[3]).D2();
	if (l2 <= 0)
		return FALSE;

	BPoint I0 = p[1].Norm() - p[0].Norm();
	double R02 = I0.D2();
	if (alf == -1)
	{
		BPoint I1 = p[1] - p[3].Norm();
		double R12 = I1.D2();
		BPoint I0s = BPoint(-I0.GetY(), I0.GetX(), 0., 0.);
		BPoint I1s = BPoint(-I1.GetY(), I1.GetX(), 0., 0.);
		double s = I0 * I1;
		double a0 = (I0 * I1s) / (R02 + s);
		double a1 = (I1 * I0s) / (R12 + s);
		p[1] = p[0] + I0s * a0;
		p[2] = p[3] + I1s * a1;
		
		double h = a0*a0*R02;
		if( h < l2 * 0.25 ) // h cant be grater than 1.
			h = 1.;
		else
			h = 0.5 * sqrt(fabs(l2 / h));
			
		p[1] = p[1] * h;
		p[2] = p[2] * h;
		return TRUE;
	}
	
	if (R == 0)
		R = sqrt(R02);


	double d, h;

	BPoint a = p[0].Norm()-p[1].Norm();
	BPoint b = p[3].Norm()-p[1].Norm();
	double R2 = a.D2();
	if (R == 0)
		R = sqrt(R2);

	BPoint tmp = a+b;
	tmp = tmp*(1./sqrt(tmp.D2()));
	d = R/cos(alf/2.);
	tmp = p[1] + tmp*fabs(d);
	tmp.Norm();

	h = sqrt(tmp.Dist2(p[0], p[3])/tmp.Dist2(p[0],tmp))*0.5;
		
	p[1] = p[2] = tmp*h;

	return TRUE;
}

void NLine::DrawHr() const
{
	glBegin(GL_LINES);
		glVertex3d(p[0].GetX()/p[0].GetH(), p[0].GetY()/p[0].GetH(), p[0].GetZ()/p[0].GetH());
		glVertex3d(p[1].GetX()/p[1].GetH(), p[1].GetY()/p[1].GetH(), p[1].GetZ()/p[1].GetH());
	glEnd();
	glBegin(GL_LINES);
		glVertex3d(p[1].GetX()/p[1].GetH(), p[1].GetY()/p[1].GetH(), p[1].GetZ()/p[1].GetH());
		glVertex3d(p[2].GetX()/p[2].GetH(), p[2].GetY()/p[2].GetH(), p[2].GetZ()/p[2].GetH());
	glEnd();
	glBegin(GL_LINES);
		glVertex3d(p[2].GetX()/p[2].GetH(), p[2].GetY()/p[2].GetH(), p[2].GetZ()/p[2].GetH());
		glVertex3d(p[3].GetX()/p[3].GetH(), p[3].GetY()/p[3].GetH(), p[3].GetZ()/p[3].GetH());
	glEnd();
}

BPoint NLine::GetCenter(void) const
{
	/*
	BPoint p1 = p[1];
	p1.Norm();
	double L0 = (p1 - p[0]).D2();
	double L  = (p[3] - p[0]).D2();
	double g = 4.*L0 / (4.*L0 - L);
	return (p[0]+p[3])*(0.5 * g) + p1*(1. - g);
	*/
	BPoint p1 = p[1];
	BPoint b1 = p1.Norm()-p[0];
	BPoint p2 = p[2];
	BPoint b2 = p2.Norm()-p[3];
	BMatr m;
	BPoint I0(b1 * m.rotg(90., BPoint(0.,0.,0.,1.), b1 % b2));
	double a = ((p[3] - p[0]) * b2) / (I0 * b2);
	return p[0] + I0 * a;
}

double NLine::GetRad(void) const
{
	BPoint p1 = p[1];
	p1.Norm();
	double R = sqrt((p1 - p[0]).D2());
	double ctg = p[1].GetH();
//	double ctg = sqrt((p[3] - p[0]).D2())/(2. * R);// Из за ошибки при построении инструмента
	if(ctg != 1.)
		R = R * ctg / sqrt(fabs(1. - ctg*ctg));
	return R;
}

BMatr NLine::GetDriveMatr(double u)
{
	BPoint t0 = p[1];
	t0 = t0.Norm() - p[0];
	BPoint curp = GetPointFromParam(u);
	curp.Norm();
	BPoint t = Tangent(u);
	BMatr m;
	m.Trans(p[0],BPoint(0.,0.,0.,1));
	BMatr tr;
	tr.e0cong(t0, t);
	m = m * tr;
	tr.Trans(BPoint(0.,0.,0.,1),curp);
	
	return m * tr;
}

const BPoint NLine::GetDir(int k) const
{
	switch(k)
	{
	case 0:
		{
		BPoint pb = p[1];
		pb.Norm();
		return pb - p[0];
		}
	case 1:
		{
		BPoint pb = p[2];
		pb.Norm();
		return p[3] - pb;
		}
	default:
		return BPoint(0.,0.,0.,0.);
	}
}
double NLine::GetLength(void) const
{
	const double CalcStep = 0.01;
	double Length = 0.;
	BPoint P0 = p[0];
	P0.Norm();
	for(double t = CalcStep; t < 1. + CalcStep * 0.5; t += CalcStep)
	{
		BPoint P1 = GetPointFromParam(t).Norm();
		Length += sqrt((P1 - P0).D2());
		P0 = P1;
	}
	return Length;
}


double NLine::Deflect(void) const
{
	BPoint P = p[1];
	P.Norm();
	BPoint e = p[3] - p[0];
	double L = sqrt(e.D2());
	if(L < MIND)
		return sqrt((P - p[0]).D2());
	double d0 = (e * (((P - p[0]) * e) / L) - P).D2();
	P = p[2];
	P.Norm();
	double d1 = (e * (((P - p[0]) * e) / L) - P).D2();
	return sqrt(max(d0, d1));
}

void NLine::Divide2(NLine & sg1, NLine & sg2)
{// Subdivision of this line into 2 lines
	// For Ball basis only
    BPoint b1=0.5   * (p[0] + p[1]);
    BPoint b2=0.125 * (p[1] - p[2]);
    BPoint b3=0.5   * (p[2] + p[3]);
	sg2.p[3] = p[3];
	sg1.p[3] = 0.5 * (b1 + b3);
    sg1.p[2] = b1 - b2;
    sg1.p[1] = b1;
    sg1.p[0] = p[0];
    sg2.p[2] = b3;
    sg2.p[1] = b2 + b3;
    sg2.p[0] = sg1.p[3];
}

void NLine::Divide2(double t, NLine & sg1, NLine & sg2)
{// Subdivision of this line into 2 lines
	// Q = M^-1 * L^-1 * M * P
	// For Ball basis only
	BMatr M;
	M.Set(	  0.,  2., -2.,  0.,
			  1., -4.,  2.,  1.,
			 -2.,  2.,  0.,  0.,
			  1.,  0.,  0.,  0.
		);

	BMatr Mi;
	Mi.Set(	  0.,  0.,  0.,  1.,
			  0.,  0., 0.5,  1.,
			-0.5,  0., 0.5,  1.,
			  1.,  1.,  1.,  1.
		);

	BMatr Li;
	Li.Set((t * t * t), 0., 0., 0.,
		0., (t * t), 0., 0.,
		0., 0., t, 0.,
		0., 0., 0., 1.
		);

	BMatr MM = Mi * Li * M;

	for (int i = 0; i < 4; ++i)
	{
		BPoint Row(MM.GetRow(i));
		sg1.p[i] = Row.GetX() * p[0] + Row.GetY() * p[1] + Row.GetZ() * p[2] + Row.GetH() * p[3];
	}

	t = 1. - t;
	Li.Set((t * t * t), 0., 0., 0.,
		0., (t * t), 0., 0.,
		0., 0., t, 0.,
		0., 0., 0., 1.
		);

	MM = Mi * Li * M;

	for (int i = 0; i < 4; ++i)
	{
		BPoint Row(MM.GetRow(i));
		sg2.p[3 - i] = Row.GetX() * p[3] + Row.GetY() * p[2] + Row.GetZ() * p[1] + Row.GetH() * p[0];
	}
}

double NLine::Deflect2(void) const
{
/* ll - квадрат длины хорды */
/* возвращает оценку сверху квадрата отклонения */
	double ll;
	BPoint p0=p[0], p1=p[1], p2=p[2], p3=p[3];
	double p0x, p0y, p0z, p0h;
	double p1x, p1y, p1z, p1h;
	double p2x, p2y, p2z, p2h;
	double p3x, p3y, p3z, p3h;
	p0.Get( p0x, p0y, p0z, p0h );
	p1.Get( p1x, p1y, p1z, p1h );
	p2.Get( p2x, p2y, p2z, p2h );
	p3.Get( p3x, p3y, p3z, p3h );

	double l2h,d21,d22,m1,m2,zl;
    if(p0h<HMIN||p1h<HMIN||p2h<HMIN||p3h<HMIN)
       return(-1.);

	double e13x, e13y, e13z;
	double ex, ey, ez;
	double e3x, e3y, e3z;
	double e0x, e0y, e0z;

    e13x=p3x*p0h-p3h*p0x; 
	 e13y=p3y*p0h-p3h*p0y;
    e13z=p3z*p0h-p3h*p0z;
    l2h=e13x*e13x+e13y*e13y+e13z*e13z;
    ll =(p0h*p3h);ll =l2h/(ll*ll);
    if(ll<LMIN*LMIN)
      {
       ex=(p0x*p3h+p3x*p0h)*0.5*p1h-p1x*p0h*p3h;
       ey=(p0y*p3h+p3y*p0h)*0.5*p1h-p1y*p0h*p3h;
       ez=(p0z*p3h+p3z*p0h)*0.5*p1h-p1z*p0h*p3h;
       d21=(ex*ex+ey*ey+ez*ez)/(p1h*p0h*p3h);
      } else
      {
       e3x=p3x*p1h-p3h*p1x; 
		 e3y=p3y*p1h-p3h*p1y;
       e3z=p3z*p1h-p3h*p1z;
       
		 e0x=p1x*p0h-p1h*p0x; 
		 e0y=p1y*p0h-p1h*p0y;
       e0z=p1z*p0h-p1h*p0z;
       
		 m1  =e3x*e13x+e3y*e13y+e3z*e13z;
       m2  =e0x*e13x+e0y*e13y+e0z*e13z;
       if(m1<0)
	 {
	  d21=(p1h*p3h);
	  d21=(e3x*e3x+e3y*e3y+e3z*e3z)/(d21*d21);
	 } else
	 {
	  if(m2<0)
	    {
	     d21=(p1h*p0h);
	     d21=(e0x*e0x+e0y*e0y+e0z*e0z)/(d21*d21);
	    } else
	    {
	     zl=1./l2h;
	     ex=((p0x*m1+p3x*m2)*zl-p1x);
	     ey=((p0y*m1+p3y*m2)*zl-p1y);
	     ez=((p0z*m1+p3z*m2)*zl-p1z);
	     d21=(ex*ex+ey*ey+ez*ez)/(p1h*p1h);
      }  }  }
    if(ll<LMIN*LMIN)
      {
       ex=(p0x*p3h+p3x*p0h)*0.5*p2h-p2x*p0h*p3h;
       ey=(p0y*p3h+p3y*p0h)*0.5*p2h-p2y*p0h*p3h;
       ez=(p0z*p3h+p3z*p0h)*0.5*p2h-p2z*p0h*p3h;
       d22=(ex*ex+ey*ey+ez*ez)/(p2h*p0h*p3h);
      } else
      {
       e3x=p3x*p2h-p3h*p2x; e3y=p3y*p2h-p3h*p2y;
       e3z=p3z*p2h-p3h*p2z;
       e0x=p2x*p0h-p2h*p0x; e0y=p2y*p0h-p2h*p0y;
       e0z=p2z*p0h-p2h*p0z;
       m1  =e3x*e13x+e3y*e13y+e3z*e13z;
       m2  =e0x*e13x+e0y*e13y+e0z*e13z;
       if(m1<0)
	 {
	  d22=(p2h*p3h);
	  d22=(e3x*e3x+e3y*e3y+e3z*e3z)/(d22*d22);
	 } else
	 {
	  if(m2<0)
	    {
	     d22=(p2h*p0h);
	     d22=(e0x*e0x+e0y*e0y+e0z*e0z)/(d22*d22);
	    } else
	    {
	     zl=1./l2h;
	     ex=((p0x*m1+p3x*m2)*zl-p2x);
	     ey=((p0y*m1+p3y*m2)*zl-p2y);
	     ez=((p0z*m1+p3z*m2)*zl-p2z);
	     d22=(ex*ex+ey*ey+ez*ez)/(p2h*p2h);
      }  }  }
    if(d21>d22) return(d21);
		return(d22);
	return 0;
}

int NLine::GetNumAproxLine(double Eps) const
{
	/* проверить отклонение ломаной */
	double epsl2 = Eps * Eps;
	if(Deflect2() <= epsl2)
		return 1;

	int maxnsteps = 1;
	int kst = -1;
	int n_steps[NSTK];
	/* поместить сегмент в стек */
	kst++;
	n_steps[kst] = 1;
	STKT = *this;
	while(kst >= 0)  /* пока стек не пуст */
	{
		/* проверить отклонение ломаной */
		double fd = STKT.Deflect2();
		if(fd > epsl2)
		{
			STKT.Divide2(STKT, stk1[kst+1] );
			n_steps[kst] = n_steps[kst] * 2;
			n_steps[kst+1] = n_steps[kst];
			kst++;
			if(kst >= NSTK - 1)
				return(-1);
		}
		else
		{
			if(maxnsteps < n_steps[kst])
				maxnsteps = n_steps[kst];
			kst--;
		}
	}
  return maxnsteps;
}

void NLine::Approximate(double Eps, BPointsBuf &Pts, bool First) const
{
	if(First)
		Pts.AddPoint(p[0]);
	/* проверить отклонение ломаной */
	double epsl2 = Eps * Eps;
	if(Deflect2() <= epsl2)
	{
		Pts.AddPoint(p[3]);
		return;
	}
	/* поместить сегмент в стек */
	bool VeryFirst = true;
	int kst = 0;
	STKT = *this;
	while(kst >= 0)  /* пока стек не пуст */
	{
		/* проверить отклонение ломаной */
		double fd = STKT.Deflect2();
		if(fd > epsl2)
		{
			STKT.Divide2(stk1[kst + 1], STKT);
			kst++;
			if(kst >= NSTK - 1)
				return; // ERROR
		}
		else
		{
			if(VeryFirst)
			{
				VeryFirst = false;
				Pts.AddPoint(STKT.p[1].Norm());// Ensure first line is tangent
			}
			Pts.AddPoint(STKT.p[3].Norm());
			kst--;
		}
	}
}

bool NLine::RayCast(const BPoint & P, const BPoint & V, double Epsilon, BPoint & Pres) const
{
	BPoint Q0;
	BPoint Q1;
	/* проверить отклонение ломаной */
	double epsl2 = Epsilon * Epsilon * 0.25;
	if (Deflect2() <= epsl2)
	{
		Pres = BPoint::LineRayCast(p[0], p[3], P, V, Epsilon);
		return Pres.IsPoint();
	}
	BPoint P0;
	BPoint P1;
	P0 = p[0];
	/* поместить сегмент в стек */
	int kst = 0;
	STKT = *this;
	while (kst >= 0)  /* пока стек не пуст */
	{
		/* проверить отклонение ломаной */
		double fd = STKT.Deflect2();
		if (fd > epsl2)
		{
			STKT.Divide2(stk1[kst + 1], STKT);
			kst++;
			if (kst >= NSTK - 1)
				return false; // ERROR
		}
		else
		{
			P1 = STKT.p[3].Norm();
			Pres = BPoint::LineRayCast(P0, P1, P, V, Epsilon);
			bool Found = Pres.IsPoint();
			if (Found)
				return true;
			P0 = P1;
			kst--;
		}
	}
	return false;
}

void NLine::ZeroCurv(bool Start)
{
	// Make zero curvature in the start or end point
	BPoint P0 = p[Start ? 0 : 3]; P0.Norm();
	BPoint P1 = p[Start ? 1 : 2]; P1.Norm();
	BPoint P2 = p[Start ? 2 : 1]; P2.Norm();
	BPoint P3 = p[Start ? 3 : 0]; P3.Norm();
	BPoint e3 = P2 - P3;
	double OldMu = sqrt(e3.D2());
	if(OldMu < MIND)
		return;
	e3 *= 1. / OldMu;
	BPoint n3 = (P1 - P0) * (BMatr().RotZ(0., 0., 0., 90.));
	double NewMu = 3. * ((P3 - P0) * n3) / (2. * (e3 * n3));
	p[Start ? 2 : 1] = (P3 - e3 * NewMu) * p[Start ? 2 : 1].GetH();
}
