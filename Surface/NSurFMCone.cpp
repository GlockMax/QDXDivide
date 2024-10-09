// NSurFMCone.cpp: implementation of the NSurFMCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "NSurFMCone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const double PI=3.14159265358979;
const float d = 1.f/3.f;

NSurFMCone::NSurFMCone()
{
	IsMarked = false;
}

NSurFMCone::~NSurFMCone()
{

}

// задание параметров образующей 
void NSurFMCone::SetForming(const NLine nl, 
							const double angle,
							const double beta,
							bool bl)
{
// nl - линия контура
// angle - угол между плоскостями проходящими через ось инструмента и
//		и линии видимого контура конуса
// beta - угол между осью X и первой плоскотью
// bl - флаг (не используется)

	BPoint pt;

	pt = (nl.p[0]+nl.p[3])*0.5;
	Line = nl;
	Line.p[1] = Line.p[2] = pt;
	
	Alf = angle;
	BAlf = beta;
}

// определить построина крышка или нет
bool NSurFMCone::GetFlag()
{
	return IsMarked;
}

// построение общей поверхности для участка конуса
int NSurFMCone::BuildSurface(PartArray& pcs, BuildingType BuildType)
{
	if (Alf < 0.)
		return -1;

	BPoint z(0.,0.,1.,0.), dir = End - Start;

// постороение передней поверхности
	if ((BuildType == surBuildAll	|| 
		 BuildType == surFront		||
		 BuildType == surFrontALateral) &&
		((0. < Alf && Alf < 360.) ||
		(Alf == 360. && z*dir > 0)))
		if (!BuildFrontSurface(pcs)) 
			return 1;
	
// постороение задней поверхности
	if ((BuildType == surBuildAll	|| 
		 BuildType == surBack 		||
		 BuildType == surLateralABack) &&
		((0. < Alf && Alf < 360.) ||
		(Alf == 0. && z*dir < 0)))
		if (!BuildBackSurface(pcs))
			return 2;
	
// постороение боковой поверхности (участок плоскости)
	if (BuildType == surBuildAll	|| 
		 BuildType == surFrontALateral ||
		 BuildType == surLateral ||
		 BuildType == surLateralABack)
	{
		if (0 < Alf && Alf < 360.)
		{
			if (!BuildLateralSurface(pcs)) 
				return 3;
		}
		// построение крышки (полный цилиндр) 
		// - вырожденная боковая поверхность
		else
		{
			IsMarked = true; // маркер построения крышки

			double R;
			BPoint c;

			if (Line.p[0].GetX() > Line.p[3].GetX() ||
				(Line.p[0].GetX() == Line.p[3].GetX() && z*dir > 0))
			{
				R = Line.p[0].GetX();
				c.Set(0., 0., Line.p[0].GetZ(), 1.);
			}
			else
			{
				R = Line.p[3].GetX();
				c.Set(0., 0., Line.p[3].GetZ(), 1.);
			}
			
			Alf = 360.;
			BAlf = 0.;
			if (BuildSurfaceFMArc(pcs, Alf, BAlf, R, c, Start, End) == -1)
				return 4;
		}
	}

	return 0;
}

// построить переднюю поверхность
bool NSurFMCone::BuildFrontSurface(PartArray& pc)
{
	int i, j, n=0;
	double x, h, alf, Gamma;
	NLine L0, L1, L3;
	BMatr m;
	BPoint pt, t;
	DirectionType type = GetTypeDirection();
	NSquarePiece *pPiece;

	// определение числа порций образующих переднюю поверхность
	if (Alf < 175.)
		n = 1;
	else if (Alf < 355)
		n = 2;
	else
		n = 3;
	Gamma = Alf/n;

	for (i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(0.,0.,0.,1.);

		if (i == 0)
		{
			m.RotZ(pt, BAlf);
			L0 = Line * m;
		}
		else
			L0 = L3;

		pPiece->pc[0][0] = L0.p[0].Norm();
		pPiece->pc[1][0] = L0.p[1].Norm();
		pPiece->pc[2][0] = L0.p[3].Norm();

		m.RotZ(pt, Gamma);
		L3 = L0 * m;

		pPiece->pc[0][2] = L3.p[0].Norm();
		pPiece->pc[1][2] = L3.p[1].Norm();
		pPiece->pc[2][2] = L3.p[3].Norm();

		alf = Gamma*PI/180.;
		x = 1;
		for (j=0; j<4; j++)
		{
			pt.Set(pt.GetX(), pt.GetY(), L0.p[j].GetZ(), 1);
			t = (L0.p[j]-pt) + (L3.p[j]-pt);
			if (t.D2() > 0.)
				t = t*(1/sqrt(t.D2()));
			x = Line.p[j].GetX()*(1/cos(alf/2.));
			L1.p[j] = pt + t*fabs(x);

			L1.p[j].Norm();
		}
		
		h = sqrt(pt.Dist2(L0.p[0],L3.p[0])/pt.Dist2(L0.p[0],L1.p[0]))*0.5;

		pPiece->pc[0][1] = L1.p[0]*h;
		pPiece->pc[1][1] = L1.p[1]*h;
		pPiece->pc[2][1] = L1.p[3]*h;

		pc.Add(pPiece);
	}

	return true;
}


// построить боковую поверхность
bool NSurFMCone::BuildLateralSurface(PartArray& pc)
{
	NLine Lr, Lm, L1;
	BMatr m, m1;
	BPoint pt, pt1, direct, cent;
	DirectionType type = GetTypeDirection();
	NPlane *pPiece;
	NSquarePiece *pPiece1;
	double length = sqrt(End.Dist2(End, Start)), h, x;

	direct = End - Start;
	direct = direct*(1/length);
	pt.Set(0.,0.,0.,1.);
	pt1 = pt + direct*length;
	cent =  pt - DirLine.p[0];
	cent = Center + cent;

	for (int i=0; i<2; i++)
	{		
		if (i == 0)
			m.RotZ(pt, BAlf);
		if (i == 1)
			m.RotZ(pt, BAlf+Alf);
		if (i == 0 && type == surCWARC)
			m1.RotZ(cent, -arcAngle*180./PI);
		if (i == 0 && type == surCCWARC)
			m1.RotZ(cent, arcAngle*180./PI);

		Lr = Line * m;
		switch(type)
		{
		case surLINE:
			pPiece = new NPlane;

			m.Trans(pt, pt1);
			Lm = Lr * m;
			if (i == 0)
				pPiece->Set(Lr.p[3], Lr.p[0], Lm.p[0], Lm.p[3]);
			else
				pPiece->Set(Lr.p[3], Lm.p[3], Lm.p[0], Lr.p[0]);

			pc.Add(pPiece);
			break;
		case surCWARC: case surCCWARC:
			pPiece1 = new NSquarePiece;

			// построение поверхности
			BuildArcMoveArcSurface(Lr, pPiece1, cent, arcAngle*180./PI);
			pPiece1->pc[0][0] = Lr.p[0].Norm();
			pPiece1->pc[1][0] = Lr.p[1].Norm();
			pPiece1->pc[2][0] = Lr.p[3].Norm();

			Lm = Lr * m1;

			pPiece1->pc[0][2] = Lm.p[0].Norm();
			pPiece1->pc[1][2] = Lm.p[1].Norm();
			pPiece1->pc[2][2] = Lm.p[3].Norm();

			x = 1;
			for (int j=0; j<4; j++)
			{
				pt1.Set(cent.GetX(), cent.GetY(), Lr.p[j].GetZ(), 1);
				direct = Lr.p[j]-pt1;
				x = sqrt(direct.D2());
				direct = direct + (Lm.p[j]-pt1);
				if (direct.D2() > 0.)
					direct = direct*(1/sqrt(direct.D2()));
				x = x*(1/cos(arcAngle/2.));
				L1.p[j] = pt1 + direct*fabs(x);

				L1.p[j].Norm();
			}
			
			h = sqrt(pt.Dist2(Lr.p[0],Lm.p[0])/pt.Dist2(Lr.p[0],L1.p[0]))*0.5;

			pPiece1->pc[0][1] = L1.p[0]*h;
			pPiece1->pc[1][1] = L1.p[1]*h;
			pPiece1->pc[2][1] = L1.p[3]*h;

			pc.Add(pPiece1);
			break;
		}
	}

	return true;
}

//построить заднюю поверхность
bool NSurFMCone::BuildBackSurface(PartArray& pc)
{
	int i, j, n=0;
	double x, h, alf, Gamma;
	NLine L0, L1, L3;
	BMatr m;
	BPoint pt, t, direct;
	DirectionType type = GetTypeDirection();
	NSquarePiece *pPiece;
	double length = sqrt(End.Dist2(End, Start));
	
	direct = End - Start;
	direct = direct*(1/length);
	// определение числа порций образующих заднюю поверхность
	Gamma = Alf - 360;
	if (fabs(Gamma) < 175.)
		n = 1;
	else if (fabs(Gamma) < 355.)
		n = 2;
	else
		n = 3;
	Gamma = Gamma/n;

	for (i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(0.,0.,0.,1.);
		t = pt + direct*length;		
		if (i == 0)
		{
			m.Trans(pt,t);
			L3 = Line * m;
			m.RotZ(t, BAlf);
			L0 = L3 * m;
			if (type == surCWARC || type == surCCWARC)
			{
				if (type == surCWARC)
					m.RotZ(t, -arcAngle*180./PI);
				else
					m.RotZ(t, arcAngle*180./PI);
				L0 = L0 * m;
			}
		}
		else
			L0 = L3;
		pt = t;	

		pPiece->pc[2][0] = L0.p[0].Norm();
		pPiece->pc[1][0] = L0.p[1].Norm();
		pPiece->pc[0][0] = L0.p[3].Norm();

		m.RotZ(pt, Gamma);
		L3 = L0 * m;

		pPiece->pc[2][2] = L3.p[0].Norm();
		pPiece->pc[1][2] = L3.p[1].Norm();
		pPiece->pc[0][2] = L3.p[3].Norm();

		alf = Gamma*PI/180.;
		x = 1;
		for (j=0; j<4; j++)
		{
			pt.Set(pt.GetX(), pt.GetY(), L0.p[j].GetZ(), 1);
			t = (L0.p[j]-pt) + (L3.p[j]-pt);
			if (t.D2() > 0.)
				t = t*(1/sqrt(t.D2()));
			x = Line.p[j].GetX()*(1/cos(alf/2.));
			L1.p[j] = pt + t*fabs(x);

			L1.p[j].Norm();
		}
		
		h = sqrt(pt.Dist2(L0.p[0],L3.p[0])/pt.Dist2(L0.p[0],L1.p[0]))*0.5;

		pPiece->pc[2][1] = L1.p[0]*h;
		pPiece->pc[1][1] = L1.p[1]*h;
		pPiece->pc[0][1] = L1.p[3]*h;

		pc.Add(pPiece);
	}

	return true;
}

