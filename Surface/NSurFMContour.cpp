// NSurFMContour.cpp: implementation of the NSurFMContour class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSurFMContour.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const double PI=3.14159265358979;

NSurFMContour::NSurFMContour()
{
}

NSurFMContour::~NSurFMContour()
{

}

// задание направлющей
void NSurFMContour::SetDirection(NLine ln, BPoint cen, DirectionType DirType)
//void NSurFMContour::SetDirection(BPoint p0, BPoint p1, DirectionType DirType)
{
	DirLine = ln;
	Center = cen;
	Start = ln.p[0];
	End = ln.p[3];

	Type = DirType;

	if (Type == surLINE)
		arcAngle = 0;
	else
	{
		double cosArc;
		BPoint a, b;

		a = Start - Center;
		b = End - Center;

		cosArc = Center.Cos2(a, b);
		cosArc = cosArc *sqrt(a.D2())*sqrt(b.D2());	// cos alf
		arcAngle = acos(cosArc);
	}
}

// определение типа направлющей
DirectionType NSurFMContour::GetTypeDirection()
{
	return Type;
}

// построение линейчатой поверхности (квадратичная)
// образованной при движении дуги окружности по отрезку
int NSurFMContour::BuildSurfaceFMArc(PartArray& pcs,
									 double Alf, double Beta,
									 double r, BPoint center,
									 BPoint start, BPoint end)
{
// передаваемые параметры:
//	pc		- ссылка на массив порций
//	Alf		- угол дуги
//	Beta	- угол смещения дуги от оси X
//	r		- радиус окружности
//	center	- сентр окружности
//	start	- начальная точка траектории
//	end		- конечная точка траектории

	if (Alf == 0. || r == 0.)
		return -1;

	int n=0, i;
	BMatr m;
	BPoint pt, t;
	NLine L0, L3;
	double Gamma, alf, x, h;
	BPoint direct = end - start;
	NSquarePiece *pPiece;

	if (direct.D2() == 0)
		return -1;

	// определение числа порций
	if (Alf < 175)	
		n = 1;
	else if (Alf < 355)
		n = 2;
	else
		n = 3;

	Gamma = Alf/n;

	for (i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(center.GetX()+r, 0, center.GetZ(), 1);

		if (i == 0)
		{
			m.RotZ(center, Beta);
			pt = pt * m;
		}
		else
			pt = L0.p[3];

		// построение первой линии
		L0.p[0] = pt;

		m.RotZ(center, Gamma);
		L0.p[3] = pt * m;

		alf = Gamma*PI/180.;
		x = 1;

		t = (L0.p[0]-center) + (L0.p[3]-center);
		if (t.D2() > 0.)
			t = t*(1/sqrt(t.D2()));
		x = r*(1/cos(alf/2.));
		L0.p[1] = center + t*fabs(x);

		L0.p[1].Norm();
		
		h = sqrt(pt.Dist2(L0.p[0],L0.p[3])/pt.Dist2(L0.p[0],L0.p[1]))*0.5;
		
		L0.p[1] = L0.p[2] = L0.p[1] * h;
		
		// построение второй линии
		pt = center + direct;
		m.Trans(center, pt);
		L3 = L0 * m;

		BuildSqLinearSurface(L0, L3, true, pPiece);

		pcs.Add(pPiece);
	}

	return 1;
}

// построение линейчатой поверхности (квадратичная)
// образованной двумя линиями
bool NSurFMContour::BuildSqLinearSurface(NLine line1, NLine line2, bool f, NSquarePiece *Piece)
{
// передаётся:
//	line1 - первая (передняя) линия
//	line2 - вторая (задняя) линия
//  f - флаг направления нормали
//	Piece - ссылка на порцию поверхности
// возвращается результат построения
//	1 - поверхность построина
//	0 - поверхность не построина
	if (line1.p[0].Dist2(line1.p[0], line1.p[3]) <= 0 ||
		line2.p[0].Dist2(line2.p[0], line2.p[3]) <= 0)
		return false;

	int singl, k;
	DirectionType type = GetTypeDirection();
	
	// формирование поверхности
	if (f)
	{
		singl = 1;		k = 0;
	}
	else
	{
		singl = -1;		k = 2;
	}

	Piece->pc[k+singl*0][0] = line1.p[3].Norm();
	Piece->pc[k+singl*2][0] = line2.p[3].Norm();
	Piece->pc[k+singl*1][0] = (line1.p[3] + line2.p[3])*0.5;

	Piece->pc[k+singl*0][1] = line1.p[1];
	Piece->pc[k+singl*2][1] = line2.p[1];
	Piece->pc[k+singl*1][1] = (line1.p[1] + line2.p[1])*0.5;

	Piece->pc[k+singl*0][2] = line1.p[0];
	Piece->pc[k+singl*2][2] = line2.p[0];
	Piece->pc[k+singl*1][2] = (line1.p[0] + line2.p[0])*0.5;

	return true;
}

// построение линейчатой поверхности (кубическая)
// образованной двумя линиями
bool NSurFMContour::BuildCubLinearSurface(NLine line1, NLine line2, bool f, NCubicPiece *pPiece)
{
// передаётся:
//	line1 - первая (передняя) линия
//	line2 - вторая (задняя) линия
//  f - флаг направления нормали
//	Piece - ссылка на порцию поверхности
// возвращается результат построения
//	1 - поверхность построина
//	0 - поверхность не построина
	if (line1.p[0].Dist2(line1.p[0], line1.p[3]) <= 0 ||
		line2.p[0].Dist2(line2.p[0], line2.p[3]) <= 0)
		return false;

	int singl, k;
	DirectionType type = GetTypeDirection();
	
	// формирование поверхности
	if (f)
	{
		singl = 1;		k = 0;
	}
	else
	{
		singl = -1;		k = 3;
	}

	pPiece->pc[k+singl*0][0] = line1.p[3].Norm();
	pPiece->pc[k+singl*1][0] = (line1.p[3] + line2.p[3])*0.5;
	pPiece->pc[k+singl*2][0] = pPiece->pc[k+singl*1][0];
	pPiece->pc[k+singl*3][0] = line2.p[3].Norm();

	pPiece->pc[k+singl*0][1] = line1.p[2];
	pPiece->pc[k+singl*1][1] = (line1.p[2] + line2.p[2])*0.5;
	pPiece->pc[k+singl*2][1] = pPiece->pc[k+singl*1][1];
	pPiece->pc[k+singl*3][1] = line2.p[2];

	pPiece->pc[k+singl*0][2] = line1.p[1];
	pPiece->pc[k+singl*1][2] = (line1.p[1] + line2.p[1])*0.5;
	pPiece->pc[k+singl*2][2] = pPiece->pc[k+singl*1][2];
	pPiece->pc[k+singl*3][2] = line2.p[1];

	pPiece->pc[k+singl*0][3] = line1.p[0].Norm();
	pPiece->pc[k+singl*1][3] = (line1.p[0] + line2.p[0])*0.5;
	pPiece->pc[k+singl*2][3] = pPiece->pc[k+singl*1][3];
	pPiece->pc[k+singl*3][3] = line2.p[0].Norm();

	return true;
}

// Построение поверхности образованной при движении дуги окружности по окружности
bool NSurFMContour::BuildArcMoveArcSurface(NLine ln, NSquarePiece *pPiece, 
										   BPoint cen, double angle,
										   int xyzType, BPoint axis)
{
// передаётся:
//	ln - образующая линия
//	Piece - ссылка на порцию поверхности
//	xyzType - тип, задающий ось вращения (по умолчанию 3)
	// 1 - ось OX
	// 2 - ось OY
	// 3 - ось OZ
	// 4 - произвольная ось
//  cen - сентр вращения
//  angle - значение угла поворота (град)
//  axia - ось вращения, если координатная ось, тогда не учатывается

// возвращается результат построения
//	1 - поверхность построина
//	0 - поверхность не построина
	if (ln.p[0].Dist2(ln.p[0], ln.p[3]) <= 0 || 
		angle == 0 || 
		xyzType < 1 || xyzType > 4 )
		return false;

	double h1, h2, d, alf;
	NLine ln_r, ln12;
	BPoint pt, t;
	BMatr m, m_move;
	int i;

	if (xyzType == 3)
	{
		m.RotZ(cen, angle);
		ln_r = ln * m;

	//	m.RotZ(cen, angle/2);
	//	ln12 = ln * m;

		// линия 00-30 
		pPiece->pc[0][0] = ln.p[0];
		pPiece->pc[1][0] = ln.p[1];
		pPiece->pc[2][0] = ln.p[3];

		// линия 00-33 
		pPiece->pc[0][2] = ln_r.p[0];
		pPiece->pc[1][2] = ln_r.p[1];
		pPiece->pc[2][2] = ln_r.p[3];

		alf = angle*PI/180.;
		h1 = ln.p[1].GetH();

		for (i=0; i<4; i++)
		{
			pt.Set(cen.GetX(), cen.GetY(), ln.p[i].GetZ(), 1);
			pt.Norm();
			t = (ln.p[i].Norm()-pt) + (ln_r.p[i].Norm()-pt);
			if (t.D2() > 0.)
				t = t*(1/sqrt(t.D2()));
			d = sqrt(t.Dist2(ln.p[i], pt));
			d = d*(1./cos(alf/2.));
			ln12.p[i] = pt + t*fabs(d);

			ln12.p[i].Norm();
		}
		
		h2 = sqrt(pt.Dist2(ln.p[0],ln_r.p[0])/pt.Dist2(ln.p[0],ln12.p[0]))*0.5;

		pPiece->pc[0][1] = ln12.p[0]*h2;
		pPiece->pc[1][1] = ln12.p[1]*h2*h1;
		pPiece->pc[2][1] = ln12.p[3]*h2;
	}

	return true;
}


void NSurFMContour::TempBuild(NLine l1, NLine l2, BPoint c, double arc, bool f, NSquarePiece *pPiece)
{

	if (l1.p[0].Dist2(l1.p[0], l1.p[3]) <= 0 ||
		l2.p[0].Dist2(l2.p[0], l2.p[3]) <= 0)
		return;


	BuildSqLinearSurface(l1, l2, false, pPiece);
	c.Norm();

	NLine ln;
	int singl, k;


	// формирование поверхности
//	if (f)
//	{
		singl = 1;		k = 0;
	
		ln.Set(l1.p[0], c, c, l2.p[0]);
		ln.BuildBallArc(arc);
		ln.DrawHr();
		pPiece->pc[0][1] = ln.p[1];

		ln.Set(l1.p[1], c, c, l2.p[1]);
		ln.BuildBallArc(arc);
		pPiece->pc[1][1] = ln.p[1];

		ln.Set(l1.p[3], c, c, l2.p[3]);
		ln.BuildBallArc(arc);
		pPiece->pc[2][1] = ln.p[1];

/*	}
	else
	{
		singl = -1;		k = 2;
	

		ln.Set(l1.p[3], c, c, l2.p[3]);
		ln.BuildBallArc(arc);
	//	pPiece->pc[k+singl*1][0] = ln.p[1];
		pPiece->pc[2][1] = ln.p[1];

		ln.Set(l1.p[1], c, c, l2.p[1]);
		ln.BuildBallArc(arc);
	//	pPiece->pc[k+singl*1][1] = ln.p[1];
		pPiece->pc[1][1] = ln.p[1];

		ln.Set(l1.p[0], c, c, l2.p[0]);
		ln.BuildBallArc(arc);
	//	pPiece->pc[k+singl*1][2] = ln.p[1];
		pPiece->pc[0][2] = ln.p[1];
	}*/

/*	double h1, h2, d, alf;
	NLine ln_r, ln12, l;
	BPoint pt, t;
	BMatr m;
	int i;

	//	m.RotZ(cen, angle);
	//	ln_r = ln * m;

	//	m.RotZ(cen, angle/2);
	//	ln12 = ln * m;

		// линия 00-30 
		pPiece->pc[0][0] = l1.p[0];
		pPiece->pc[1][0] = l1.p[1];
		pPiece->pc[2][0] = l1.p[3];

		// линия 00-33 
		pPiece->pc[0][2] = l2.p[0];
		pPiece->pc[1][2] = l2.p[1];
		pPiece->pc[2][2] = l2.p[3];

		alf = arc*PI/180.;
	//	h1 = l1.p[1].GetH();

		pt.Set(c.GetX(), c.GetY(), l1.p[0].GetZ(), 1);
		t = (l1.p[0]-pt) + (l2.p[0]-pt);
		if (t.D2() > 0.)
			t = t*(1./sqrt(t.D2()));	
		
		for (i=0; i<4; i++)
		{
			l1.p[i].Norm();
			d = sqrt(t.Dist2(l1.p[i], pt));
			d = d*(1./cos(alf/2.));
			ln12.p[i] = pt + t*fabs(d);

			ln12.p[i].Norm();
		}

		h2 = sqrt(pt.Dist2(l1.p[0],l2.p[0])/pt.Dist2(l1.p[0],ln12.p[0]))*0.5;

		pPiece->pc[0][1] = ln12.p[0]*h2*2;
		pPiece->pc[1][1] = ln12.p[1]*h2*2;
		pPiece->pc[2][1] = ln12.p[3]*h2*2;
*/

}
