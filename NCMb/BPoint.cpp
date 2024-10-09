// BPoint.cpp: implementation of the BPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "BPoint.h"
#include "BMatr.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


BPoint::BPoint(double x, double y, double z , double h )
{
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = h;
}
BPoint::BPoint(const BPoint &P0, double a0, const BPoint &P1, double a1)
{
	c[0] = P0.c[0] * a0 + P1.c[0] * a1;
	c[1] = P0.c[1] * a0 + P1.c[1] * a1;
	c[2] = P0.c[2] * a0 + P1.c[2] * a1;
	c[3] = P0.c[3] * a0 + P1.c[3] * a1;
}

bool BPoint::operator ==(const BPoint &p) const
{
	if(p.c[0] != c[0])
		return false;
	if(p.c[1] != c[1])
		return false;
	if(p.c[2] != c[2])
		return false;
	if(p.c[3] != c[3])
		return false;
	return true;
}
BPoint BPoint::operator *(const BMatr &m) const
{
	return BPoint(
		c[0] * m.c[0][0] + c[1] * m.c[1][0] + c[2] * m.c[2][0] + c[3] * m.c[3][0],
		c[0] * m.c[0][1] + c[1] * m.c[1][1] + c[2] * m.c[2][1] + c[3] * m.c[3][1],
		c[0] * m.c[0][2] + c[1] * m.c[1][2] + c[2] * m.c[2][2] + c[3] * m.c[3][2],
		c[0] * m.c[0][3] + c[1] * m.c[1][3] + c[2] * m.c[2][3] + c[3] * m.c[3][3]);
}

void BPoint::Set(double x, double y, double z, double h)
{
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = h;
}
void BPoint::Get(double &x, double &y, double &z, double &h) const
{
	x = c[0];
	y = c[1];
	z = c[2];
	h = c[3];
}

double BPoint::Cos2(const BPoint &p1,const BPoint &p2)
{
	double	d1 = p1.D2(),
			d2 = p2.D2();
	if(d1 < DMIN*DMIN || d2 < DMIN*DMIN)
		return 0;
	return p1*p2/(d1*d2);
}

const BPoint& BPoint::operator*=(const BMatr& m)
{
	BPoint B(
		c[0] * m.c[0][0] + c[1] * m.c[1][0] + c[2] * m.c[2][0] + c[3] * m.c[3][0],
		c[0] * m.c[0][1] + c[1] * m.c[1][1] + c[2] * m.c[2][1] + c[3] * m.c[3][1],
		c[0] * m.c[0][2] + c[1] * m.c[1][2] + c[2] * m.c[2][2] + c[3] * m.c[3][2],
		c[0] * m.c[0][3] + c[1] * m.c[1][3] + c[2] * m.c[2][3] + c[3] * m.c[3][3]);
	*this = B;
	return *this;
}

double BPoint::Dist2(const BPoint &p1, const BPoint &p2)
{
	return (p2-p1).D2();
}
/*
BPoint BPoint::operator -(const BPoint &p) const
{
	return BPoint(c[0] - p.c[0], c[1] - p.c[1], c[2] - p.c[2], c[3] - p.c[3]);
}
*/
BPoint BPoint::operator +(const BPoint &p) const
{
	return BPoint(c[0] + p.c[0], c[1] + p.c[1], c[2] + p.c[2], c[3] + p.c[3]);
}
const BPoint BPoint::operator *(double a) const
{
	return BPoint(c[0] * a, c[1] * a, c[2] * a, c[3] * a);
}
const BPoint &BPoint::Mult(const BPoint &P, double a)
{
	c[0] = P.c[0] * a;
	c[1] = P.c[1] * a;
	c[2] = P.c[2] * a;
	c[3] = P.c[3] * a;
	return *this;
}

BPoint operator *(double a, const BPoint &P)
{
	return P * a;
}

BPoint BPoint::operator %(const BPoint &p) const
{
	return BPoint(
		c[1]*p.c[2] - c[2]*p.c[1],
		c[2]*p.c[0] - c[0]*p.c[2],
		c[0]*p.c[1] - c[1]*p.c[0],
		0.
		);
}
BPoint & BPoint::XY2XZ()
{
// Y <-> Z
	double b;
	b=c[1];c[1]=c[2];c[2]=b;
	return *this;
}
BPoint & BPoint::XY2YZ()
{
// X <-> Z
	double b;
	b=c[0];c[0]=c[2];c[2]=b;
	return *this;
}

const double * BPoint::GetArray() const
{
	return c;
}

double BPoint::Angle(const BPoint &v) const
{
// Calculates angle between two vectors in degrees
// does not check if the points are correct vectors
// It is assumed that vectors lie in  XY plane
// Result is from -180 to 180
	double sign = ((*this % v).GetZ() > 0.) ? 1. : -1.;

	return Angle0_180(v) * sign;
}

double BPoint::Angle0_180(const BPoint &v) const
{
// Calculates angle between two vectors in degrees
// does not check if the points are correct vectors
// Result is from 0 to 180
	double c = (*this * v);
	double z = D2() * v.D2();
	if( z == 0. )
		return 0.;
	c = c/sqrt(z);
	c = c < -1. ? -1. : c;
	c = c >  1. ?  1. : c;

	return acos(c) * 180. / PI;
}

double BPoint::Angle(const BPoint &v, const BPoint &N) const
{
// Calculates angle between two vectors in degrees
// does not check if the points are correct vectors
// N is the normal vector for vectors plane
// Result is > -180 and <= 180
	double sign = ((*this % v) * N >= 0.) ? 1. : -1.;
	const double Angle = Angle0_180(v);
	if(sign < 0.)
		if (fabs(Angle - 180.) < MINAD)
			sign = 1.;
	return Angle * sign;
}

const BPoint & BPoint::Norm()
{
	if( c[3] > HMIN )
	{
		double d = 1./c[3];
		c[0] *= d;
		c[1] *= d;
		c[2] *= d;
		c[3] = 1.;
	}
	return *this;
}

const BPoint & BPoint::Unit()
{
	double d2 = D2();
	if(d2 > 0.)
		*this *= (1. / sqrt(d2));
	return *this;
}

bool BPoint::IsVector(void) const
{
	return c[3] == 0.;
}
bool BPoint::IsPoint(void) const
{
	return c[3] > 0.;
}
const BPoint & BPoint::ProjXY(void)
{
	c[2] = 0.;
	return *this;
}

void BPoint::LineLine(const BPoint &p1, const BPoint &e1, const BPoint &p2, const BPoint &e2, BPoint &q1, BPoint &q2)
{
	double	A1 = e1.D2(),
		B1 = -(e1 * e2),
		H1 = e1 * (p2 - p1),
		A2 = -B1,
		B2 = -(e2.D2()),
		H2 = e2 * (p2 - p1),
		det = A1 * B2 - B1 * A2,
		det1 = H1 * B2 - B1 * H2,
		det2 = A1 * H2 - H1 * A2;
	BPoint S(0., 0., 0., 1.);
	if (fabs(det) > MIND * MIND * MIND * MIND)
	{// intersects
		double t1 = det1 / det;
		double t2 = det2 / det;
		q1 = p1 + e1 * t1;
		q2 = p2 + e2 * t2;
		return;
	}
	double e1D2 = e1.D2();
	double e2D2 = e2.D2();
	if (e1D2 < MIND * MIND || e2D2 < MIND * MIND)
	{
		q1 = p1;
		q2 = p2;
		return;
	}
	// parallel
	q1 = p1;
	double t = ((p2 - p1) * (p2 - p1)) / (e2 * (p1 - p2));
	q2 = p2 + e2 * t;
}

double BPoint::LineLineD2(const BPoint &p1, const BPoint &e1, const BPoint &p2, const BPoint &e2)
{
	double	A1 = e1.D2(),
		B1 = -(e1 * e2),
		H1 = e1 * (p2 - p1),
		A2 = -B1,
		B2 = -(e2.D2()),
		H2 = e2 * (p2 - p1),
		det = A1 * B2 - B1 * A2,
		det1 = H1 * B2 - B1 * H2,
		det2 = A1 * H2 - H1 * A2;
	BPoint S(0., 0., 0., 1.);
	if (fabs(det) > MIND * MIND * MIND * MIND)
	{// intersects
		double t1 = det1 / det;
		double t2 = det2 / det;
		return (p1 + e1 * t1, p2 + e2 * t2).D2();
	}
	double e1D2 = e1.D2();
	double e2D2 = e2.D2();
	if (e1D2 < MIND * MIND || e2D2 < MIND * MIND)
		return (p2 - p1).D2();
// parallel
	double Cos2 = H1 * H1 / (e1D2 * e2D2);
	return (p2 - p1).D2() * fabs(1. - Cos2);

}

double BPoint::LineD2(const BPoint &p1,const BPoint &p2) const
{
	// Distance (2) from this point to straight line
	BPoint b(p2);
	b -= p1;
	double z = b.D2();
	if( z < DMIN * DMIN)
		return 0.;
	BPoint v(*this);
	v -= p1;
	double a = b * v;
	return v.D2() - (a * a) / z;
}

bool BPoint::InTriangle(BPoint &A,BPoint &B, BPoint &C)
{
	BPoint	AB = B - A, 
			BC = C - B,
			CA = A - C,
			AP = *this - A,
			BP = *this - B,
			CP = *this - C,
			Normal = AB % BC;
	if((Normal%AB)*AP>=0 && (Normal%BC)*BP>=0 && (Normal%CA)*CP>=0)
		return true;
	else
		return false;
}

bool BPoint::InLine(BPoint &A,BPoint &B)
{
	BPoint	AP = *this - A,
			BP = *this - B,
			AB = B - A;
	if(AB*AP>=0 && AB*BP<=0)//точка заготовки лежит внутри слоя ребра
		return true;
	return false;//точка заготовки не лежит в слое ребра
}

double BPoint::DistTriangle(float Tr[9], double drec, BPoint &M, unsigned int &NumVertex, unsigned int &Type)//Находим расстояние от точки до треугольника
{
	double rec;
	BPoint A(double(Tr[0]), double(Tr[1]), double(Tr[2]), 1.);
	BPoint B(double(Tr[3]), double(Tr[4]), double(Tr[5]), 1.);
	BPoint C(double(Tr[6]), double(Tr[7]), double(Tr[8]), 1.);
	BPoint AB = B - A;
	BPoint BC = C - B;
	BPoint AP = *this-A;		//точка P и вызвала этот метод!
	BPoint N = AB % BC;		//определенно направленная нормаль треугольника
	BPoint O = *this + N*( ((A-*this)*N) / (N*N) );
	rec = fabs((AP*N)*(1./sqrt(N*N)));
	if(rec > drec + MIND)//если расстояние до плоскости грани >= переданного рекорда
	{
		Type = 0;//При этом номер вершины NumVertex игнорируется
		return drec; 
	}
	else
	{	
		Type = 3;//rec вычислен до внутренностей грани
		BPoint CA = A - C;
		BPoint BP = *this-B;
		BPoint CP = *this-C;
		if(InTriangle(A, B, C) == false)//если основание перпендикуляра не лежит на грани
		{
			bool fa = true, fb = true, fc = true;//по умолчанию должны вычислять расст. до всех вершин
			double d;
			rec = 1.e20;
			if(InLine(A, B)==true)//точка P лежит в слое AB
			{
				O = A+AB*(AB*(*this-A)/(AB*AB));
				rec = (*this-O).D2();//квадрат расстояния от P до AB
				NumVertex = 2;//противоположная ребру вершина C под номером 2 (A-0,B-1,C-2)
				Type = 2;
				fa = false;
				fb = false;
			}
			if(InLine(B, C)==true)//точка P лежит в слое BC
			{
				BPoint O1 = B+BC*( BC*(*this-B)/(BC*BC) );
				d = (*this-O1).D2();//квадрат расстояния от P до BC
				if(d < rec)
				{
					rec = d;
					O = O1;
					NumVertex = 0;
					Type = 2;
				}
				fb = false;
				fc = false;
			}
			if(InLine(C, A)==true)//точка P лежит в слое CA
			{
				BPoint O1 = C+CA*(CA*(*this-C)/(CA*CA));
				d = (*this-O1).D2();//квадрат расстояния от P до CA
				if(d < rec)
				{
					rec = d;
					O = O1;
					NumVertex = 1;
					Type = 2;
				}
				fa = false;
				fc = false;
			}
			if(fa == true)
			{
				d = AP.D2();//Вычисляем квадрат расстояния до точки A
				if(d < rec)
				{
					rec = d;
					O = A;
					NumVertex = 0;//ближайшая точка - вершина A под номером 0
					Type = 1;
				}
			}
			if(fb == true)
			{
				d = BP.D2();//Вычисляем квадрат расстояния до точки B
				if(d < rec)
				{
					rec = d;
					O = B;
					NumVertex = 1;
					Type = 1;
				}
			}
			if(fc == true)
			{
				d = CP.D2();//Вычисляем квадрат расстояния до точки C
				if(d < rec)
				{
					rec = d;
					O = C;
					NumVertex = 2;
					Type = 1;
				}
			}
			rec=sqrt(rec);//Вычисляем сам рекорд
		}	
	}
	if(rec <= drec + MIND)
	{
		M = O;//запомним ближайшую точку, Type уже определён, NumVertex - тоже.
		return rec;
	}
	else
	{
		Type = 0;//рекорд всё же больше 
		return drec;
	}
}

void BPoint::Setx(double x)
{
	c[0] = x * c[3];
}
void BPoint::Sety(double y)
{
	c[1] = y * c[3];
}
void BPoint::Setz(double z)
{
	c[2] = z * c[3];
}
void BPoint::MiddlePoint(const BPoint &P0, const BPoint &P1)//нахождение точки на середине отрезка с концами P0 и P1
{
	*this = (P0 + P1) * 0.5;
}
void BPoint::GravTo0(void)
{
	if(fabs(c[0]) < DMIN)
		c[0] = 0.;
	if(fabs(c[1]) < DMIN)
		c[1] = 0.;
	if(fabs(c[2]) < DMIN)
		c[2] = 0.;
}
double BPoint::Dist2Triangle(const BPoint src[3],const BPoint &n, double rec2, BPoint &M, unsigned int &NumVertex, unsigned int &Type) const//Находим квадрат расстояния от точки до треугольника 
{//Нахождение квадрата расстояния от точки до треугольника
	//SetH(1);
	BPoint vad = *this;//вектор AD (из вершины тр в иссл точку)
    vad.SetH(1);
	vad-=src[0];//вектор из вершины A в исследуемую точку
	double h = n*vad;//посчитали высоту нормали к плоскости
	if(h*h>rec2+2*rec2*MIND+MIND*MIND)//если расстояние до плоскости больше существующего рекорда
	{//!!!!!!!!!!MIND для совместимости с Будником
		Type = 0;
		return rec2;
	}
	//Находим проекцию точки на плоскость трегольника
	BPoint p=*this;
    p.SetH(1);
	p-=n*h;
	//Находим три образующих вектора треугольника
	BPoint va = src[1],
		vb = src[2],
		vc = src[0];
	va-=src[0];
	vb-=src[1];
	vc-=src[2];
	//проверяем вырожденность
	double vad2=va.D2(),
		vbd2=vb.D2(),
		vcd2=vc.D2();
	bool matchAB,matchBC, matchCA;
	matchAB=(vad2<MIND*MIND);
	matchBC=(vbd2<MIND*MIND);
	matchCA=(vcd2<MIND*MIND);
	//проверяем направление указания вершин треугольника
	int sgn;
	if ((va%vb)*n>0)
		sgn=1;
	else
		sgn=-1;
	//находим вектора из вершин треугольника к проекции точки на его плоскости
	BPoint pa = p,
		pb = p,
		pc = p;
	pa-=src[0];
	pb-=src[1];
	pc-=src[2];

	bool la,lb,lc,lfta,lftb,lftc;
	//проверяем, лежит ли вектор на внутренней стороне каждой из сторон треугольника
	lfta=((n*(va%pa)*sgn)>=0);
	lftb=((n*(vb%pb)*sgn)>=0);
	lftc=((n*(vc%pc)*sgn)>=0);

	if(lfta&&lftb&&lftc&&!matchAB&&!matchBC&&!matchCA)//если попали внутрь треугольника
	{
		M=p;
		Type = 3;
		return p.Dist2(p,*this);
	}

	Type = 2;
	//получаем скалярные произведения
	double sca = pa*va,//1ое ск произв
		val=va.D2();//длина вектора va
	la = ((sca>=0)&&(sca<=val));
	if((!lfta)&&la&&!matchAB)//если попали на ребро A
	{
		NumVertex = 2;
		M = src[0]+va*(sca/va.D2());
		return (vad.D2()-sca*sca/va.D2());
	}
	double scb = pb*vb,//2ое ск произв
		vbl=vb.D2();//длина вектора vb
	lb = ((scb>=0)&&(scb<=vbl));
	if((!lftb)&&lb&&!matchBC)//если попали на ребро B
	{
		BPoint vbd = *this;
        vbd.SetH(1);
		vbd-=src[1];
		NumVertex = 0;
		M = src[1]+vb*(scb/vb.D2());
		return (vbd.D2()-scb*scb/vb.D2());
	}
	double scc = pc*vc,//3е ск произв
		vcl=vc.D2();//длина вектора vc
	lc = ((scc>=0)&&(scc<=vcl));
	if((!lftc)&&lc&&!matchCA)//если попали на ребро C
	{
		BPoint vcd = *this;
        vcd.SetH(1);
		vcd-=src[2];
		NumVertex = 1;
		M = src[2]+vc*(scc/vc.D2());
		return (vcd.D2()-scc*scc/vc.D2());
	}
	Type = 1;
	NumVertex = 2;
	M=src[2];
	if(!matchAB&&(scb>=vbl)&&(scc-MIND<=0))
		return this->Dist2(*this,src[2]);
	NumVertex = 0;
	M=src[0];
	if(!matchBC&&(scc>=vcl)&&(sca-MIND<=0))
		return this->Dist2(*this,src[0]);
	M=src[1];
	NumVertex = 1;
	//if(lftc) - само собой разумеется, поэтому можно опустить
	return this->Dist2(*this,src[1]);
}

double BPoint::CAbsSum(void) const
{
	return fabs(c[0]) + fabs(c[1]) + fabs(c[2]);
}

BPoint BPoint::Nearest2Lines(int LinesNum, BPoint **P, BPoint ** V)
{
	if (LinesNum < 2)
		return BPoint(0., 0., 0., -1.);
	BMatr MSum(0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.);
	BPoint PSum(0., 0., 0., 0.);
	for (int i = 0; i < LinesNum; ++i)
	{
		BMatr VV(1. - V[i]->GetX() * V[i]->GetX(), V[i]->GetX() * V[i]->GetY(), V[i]->GetX() * V[i]->GetZ(), 0.,
			V[i]->GetY() * V[i]->GetX(), 1. - V[i]->GetY() * V[i]->GetY(), V[i]->GetY() * V[i]->GetZ(), 0.,
			V[i]->GetZ() * V[i]->GetX(), V[i]->GetZ() * V[i]->GetY(), 1. - V[i]->GetZ() * V[i]->GetZ(), 0.,
			0., 0., 0., 1.);
		PSum += (*P[i]) * VV;
		MSum += VV;
	}
	return PSum * MSum.invr();
}

BPoint BPoint::Nearest2LinesD(int LinesNum, BPoint **P, BPoint ** V)
{
	if (LinesNum < 2)
		return BPoint(0., 0., 0., -1.);
	BPoint ResP(0., 0., 0., 0.);
	for (int i = 0; i < LinesNum - 1; ++i)
	{
		for (int j = i + 1; j < LinesNum; ++j)
		{
			BPoint Q1, Q2;
			LineLine(*P[i], *V[i], *P[j], *V[j], Q1, Q2);
			ResP += Q1 + Q2;
		}
	}
	ResP = ResP * (1. / (LinesNum * (LinesNum - 1)));
	return ResP;
}

const BPoint &BPoint::Round2Zero(void)
{
	if(fabs(c[0]) < 1.e-3)
		c[0] = 0.;
	if(fabs(c[1]) < 1.e-3)
		c[1] = 0.;
	if(fabs(c[2]) < 1.e-3)
		c[2] = 0.;
	return *this;
}

const BPoint &BPoint::Align2CoordAxis(void)
{
	// Find max coord index
	int MaxInd = 0;
	if(fabs(c[MaxInd]) < fabs(c[1]))
		MaxInd = 1;
	if(fabs(c[MaxInd]) < fabs(c[2]))
		MaxInd = 2;
	double Val = c[MaxInd];
	c[0] = c[1] = c[2] = 0.;
	c[MaxInd] = Val > 0. ? 1. : -1;
	return *this;
}

const BPoint BPoint::CAbs(void) const
{ // by coordinate
	return BPoint(fabs(c[0]), fabs(c[1]), fabs(c[2]), fabs(c[3]));
}

const BPoint BPoint::CMul(const BPoint &P) const
{ // by coordinate
	return BPoint(c[0] * P.c[0], c[1] * P.c[1], c[2] * P.c[2], c[3] * P.c[3]);
}

const BPoint BPoint::CDiv(const BPoint &P) const
{ // by coordinate
	return BPoint(c[0] / P.c[0], c[1] / P.c[1], c[2] / P.c[2], c[3] / P.c[3]);
}

const BPoint BPoint::Decomp(const BPoint &X, const BPoint &Y, const BPoint &Z) const
{
	// return (x, y, z, *) where this = x * X + y * Y + z * Z
	BMatr B;
	B.SetRow(0, X);
	B.SetRow(1, Y);
	B.SetRow(2, Z);
	return (*this) * B.invr();
}

BPoint BPoint::ProjectPointOnPlane(const BPoint& iPoint, const BPoint& iOrigin, const BPoint& iNormal)
{
	BPoint normal = iNormal;
	normal.Unit();
	return iPoint - normal * ((iPoint - iOrigin) * normal);
}

double ClampD(double n, double min, double max) {
	//Clamp value n on min or max
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

BPoint BPoint::LineRayCast(const BPoint & BP, const BPoint & EP, const BPoint & P, const BPoint & V, double Epsilon)
{

	BPoint lineDir = EP - BP;
	BPoint r = P - BP;
	double squaredRay = V * V;
	double squaredLine = lineDir * lineDir;
	double f = lineDir * r;

	double vectorT = 0.0, lineT = 0.0;
	BPoint pointOnRay, pointOnLine;

	double c = V * r;
	if (squaredLine <= MINAR) {
		lineT = 0.0f;
		vectorT = ClampD(-c / squaredRay, 0.0f, 1.0f);
	}
	else {
		double b = V * lineDir;
		double denom = squaredRay*squaredLine - b*b;
		if (denom != 0.0f) {
			double nT = (b*f - c*squaredLine) / denom;
			vectorT = nT < 0.0 ? 0.0 : nT;
		}
		else vectorT = 0.0f;

		lineT = (b*vectorT + f) / squaredLine;

		if (lineT < 0.0f) {
			lineT = 0.0f;
			vectorT = ClampD(-c / squaredRay, 0.0f, 1.0f);
		}
		else if (lineT > 1.0f) {
			lineT = 1.0f;
			vectorT = ClampD((b - c) / squaredRay, 0.0f, 1.0f);
		}
	}
	pointOnRay = P + V * vectorT;
	pointOnLine = BP + lineDir * lineT;
	if (sqrt((pointOnRay - pointOnLine).D2()) < Epsilon)
		return pointOnLine;
	else
		return BPoint(0, 0, 0, -1);
}

ClipRes BPoint::ClipLine(BPoint& P0, BPoint& P1, double Xmin, double Xmax, double Ymin)
{
	if (P0.GetY() < Ymin - MIND && P1.GetY() < Ymin - MIND)
		return ClipRes::CR_REMOVED;
	if (P0.GetZ() <= Xmin && P1.GetZ() <= Xmin)
		return ClipRes::CR_REMOVED;
	if (P0.GetZ() >= Xmax && P1.GetZ() >= Xmax)
		return ClipRes::CR_REMOVED;
	bool LeftRight = P0.GetZ() < P1.GetZ();
	BPoint& XminP = LeftRight ? P0 : P1;
	BPoint& XmaxP = LeftRight ? P1 : P0;
	bool Cliped = false;
	// clip by Xmin
	if (XminP.GetZ() < Xmin && XmaxP.GetZ() > Xmin)
	{
		double t = (Xmin - XminP.GetZ()) / (XmaxP.GetZ() - XminP.GetZ());
		XminP.SetZ(Xmin);
		XminP.SetY(XminP.GetY() * (1. - t) + XmaxP.GetY() * t);
		Cliped = true;
	}
	// clip by Xmax
	if (XminP.GetZ() < Xmax && XmaxP.GetZ() > Xmax)
	{
		double t = (Xmax - XminP.GetZ()) / (XmaxP.GetZ() - XminP.GetZ());
		XmaxP.SetZ(Xmax);
		XmaxP.SetY(XminP.GetY() * (1. - t) + XmaxP.GetY() * t);
		Cliped = true;
	}
	if(fabs(P1.GetY() - Ymin) < MIND && fabs(P0.GetY() - Ymin) < MIND && fabs(XminP.GetZ() - Xmin) < MIND && fabs(XmaxP.GetZ() - Xmax) < MIND)
		return ClipRes::CR_OVER;
	const bool BottomUp = P0.GetY() < P1.GetY();
	const BPoint& YminP = BottomUp ? P0 : P1;
	const BPoint& YmaxP = BottomUp ? P1 : P0;
	if (YmaxP.GetY() < Ymin - MIND)
		return ClipRes::CR_REMOVED;
	if(fabs(YmaxP.GetY() - Ymin) < MIND)
		if(YminP.GetY() < Ymin - MIND)
			return ClipRes::CR_REMOVED;

	return Cliped ? ClipRes::CR_CLIP : ClipRes::CR_UNCHANGED;
}



