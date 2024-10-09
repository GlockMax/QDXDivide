// NSurFMHorizontal.cpp: implementation of the NSurFMHorizontal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSurFMHorizontal.h"
#include "math.h"

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
bool flag = true;

NSurFMHorizontal::NSurFMHorizontal()
{

}

NSurFMHorizontal::~NSurFMHorizontal()
{

}

// задание параметров образующей
void NSurFMHorizontal::SetForming(const NLine nl, 
								  const double angle, 
								  const double beta, 
								  bool bl)
{
// nl - линия контура
// angle - угол между плоскостями проходящими через ось инструмента и
//		и линии видимого контура конуса
// beta - угол между осью X и первой плоскотью
// bl - флаг задаёт построение верхней или нижней крыщки:
//		1 - верхняя крышка
//		0 - нижняя крышка

	BPoint pt;

	pt = (nl.p[0]+nl.p[3])*0.5;
	Line = nl;
	Line.p[1] = Line.p[2] = pt;
	
	Alf = angle;
	BAlf = beta;
	IsTop = bl;
}

// построение общей поверхности для участка конуса
int NSurFMHorizontal::BuildSurface(PartArray& pcs, BuildingType BuildType)
{
	if (Alf < 0.)
		return -1;

	double alf_old, beta_old;

	alf_old = Alf;
	beta_old = BAlf;
	BPoint z(0.,0.,1.,0.), dir = End - Start;
	DirectionType type = GetTypeDirection();

	if (z*dir != 0.)
	{
		// построение передней поверхности
		if ((BuildType == surBuildAll	|| 
			 BuildType == surFront		||
			 BuildType == surFrontALateral) &&	(z*dir < 0 && IsTop )||	(z*dir > 0 && !IsTop))
		{
			Alf = 360.; BAlf = 0;
			if (!BuildFrontSurface(pcs))
				return 1;
			Alf = alf_old; BAlf = beta_old;
		}

		// построение задней поверхности
		if ((BuildType == surBuildAll	|| 
			 BuildType == surBack 		||
			 BuildType == surLateralABack) && (z*dir > 0 && IsTop) || (z*dir < 0 && !IsTop))
		{
			Alf = 0.; BAlf = 0.;
			if (!BuildBackSurface(pcs))
				return 2;
			Alf = alf_old; BAlf = beta_old;
		}			
	
		flag = false;
	}
	else
	{
		if (type == surCWARC || type == surCCWARC)
		{
			if (BuildType == surBuildAll	|| 
				BuildType == surFront		||
				BuildType == surFrontALateral)
						
					if (!BuildFrontSurface(pcs))
						return 1;
		
			if (BuildType == surBuildAll	|| 
				BuildType == surBack		|| 
				BuildType == surLateralABack) 

					if (!BuildBackSurface(pcs))
						return 2;
		}
	}

	if ((z*dir > 0 && !IsTop) ||
		(z*dir < 0 && IsTop))
		Alf = Alf-360.;

	// построение боковой поверхности
	if (((BuildType == surBuildAll)	|| 
			 BuildType == surFrontALateral ||
			 BuildType == surLateral ||
			 BuildType == surLateralABack))
		{
			if(!BuildLateralSurface(pcs)) 
				return 3;
		}

	return 0;
}

// построение передней поверхности (если она есть)
bool NSurFMHorizontal::BuildFrontSurface(PartArray &pc)
{
	int i, j, n=0;
	double x, h, alf, Gamma;
	NLine L0, L1, L3;
	BMatr m;
	BPoint pt, t;
	DirectionType type = GetTypeDirection();
	NSquarePiece *pPiece;

	L3 = Line;
	// определение числа порций образующих переднюю поверхность
	if (Alf < 175.)
	{
		n = 1;		Gamma = Alf;
	}
	else if (Alf < 355)
	{
		n = 2;		Gamma = Alf/2.;
	}
	else
	{
		n = 3;		Gamma = Alf/3.;
	}

	for (i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(0.,0.,0.,1.);

		if (i == 0)
		{
			m.RotZ(pt, BAlf);
			L0 = L3 * m;
		}
		else
			L0 = L3;

		pPiece->pc[0][0] = L0.p[0].Norm();
		pPiece->pc[1][0] = L0.p[1].Norm();
		pPiece->pc[2][0] = L0.p[3].Norm();
		if (i == 0) Ln_s = L0;

		m.RotZ(pt, Gamma);
		L3 = L0 * m;

		pPiece->pc[0][2] = L3.p[0].Norm();
		pPiece->pc[1][2] = L3.p[1].Norm();
		pPiece->pc[2][2] = L3.p[3].Norm();
		if (i == n) Ln_e = L3;

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
		
		if (IsTop)
			h = sqrt(pt.Dist2(L0.p[3],L3.p[3])/pt.Dist2(L0.p[3],L1.p[3]))*0.5;
		else
			h = sqrt(pt.Dist2(L0.p[0],L3.p[0])/pt.Dist2(L0.p[0],L1.p[0]))*0.5;

		pPiece->pc[0][1] = L1.p[0]*h;
		pPiece->pc[1][1] = L1.p[1]*h;
		pPiece->pc[2][1] = L1.p[3]*h;

		pc.Add(pPiece);
	}

	return true;
}

// построение задней поверхности (если она есть)
bool NSurFMHorizontal::BuildBackSurface(PartArray &pc)
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
	L3 = Line;
	// определение числа порций образующих заднюю поверхность
	Gamma = Alf - 360;
	if (fabs(Gamma) < 175.)
	{
		n = 1;
	}
	else if (fabs(Gamma) < 355.)
	{
		n = 2;		Gamma = Gamma/2.;
	}
	else
	{
		n = 3;		Gamma = Gamma/3.;
	}

	for (i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(0.,0.,0.,1.);
		t = pt + direct*length;		
		if (i == 0)
		{
			m.Trans(pt,t);
			L3 = L3 * m;
			if (type == surCWARC || type == surCCWARC)
			{
				alf = arcAngle*180./PI;
				m.RotZ(t, BAlf+alf);
			}
			else
			{
				m.RotZ(t, BAlf);
			}
			L0 = L3 * m;		
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
		
		if (IsTop)
			h = sqrt(pt.Dist2(L0.p[3],L3.p[3])/pt.Dist2(L0.p[3],L1.p[3]))*0.5;
		else
			h = sqrt(pt.Dist2(L0.p[0],L3.p[0])/pt.Dist2(L0.p[0],L1.p[0]))*0.5;

		pPiece->pc[2][1] = L1.p[0]*h;
		pPiece->pc[1][1] = L1.p[1]*h;
		pPiece->pc[0][1] = L1.p[3]*h;

		pc.Add(pPiece);
	}

	return true;
}

//постороение боковой поверхности
bool NSurFMHorizontal::BuildLateralSurface(PartArray& pc)
{
	int n=0;
	double x, h, alf, Gamma;
	BMatr m;
	NLine L0, L1, line1, line2;
	BPoint pt, t, pt0, pt1, pt3, direct;
	NCubicPiece *pPiece;	
	DirectionType type = GetTypeDirection();
	double length = sqrt(End.Dist2(End, Start));

	direct = End - Start;
	direct = direct*(1/length);

	// определение числа порций образующих переднюю поверхность
	if (fabs(Alf) < 175.)
		n = 1;
	else if (fabs(Alf) < 355.)
		n = 2;
	else
		n = 3;

	Gamma = Alf/n;
	
	for (int i=0; i<n; i++)
	{
		pt.Set(0.,0.,0.,1.);

		switch(type)
		{
		case surLINE:

			// посроение передней линии
			if (i == 0)
			{
				m.RotZ(pt, BAlf);
				L0 = Line * m;
			}

			if (IsTop)				// точка 0
				pt0 = L0.p[3];
			else
				pt0 = L0.p[0];

			m.RotZ(pt, Gamma);
			L0 = L0 * m;
			if (IsTop)				// точка 3
				pt3 = L0.p[3];
			else
				pt3 = L0.p[0];

		/*	alf = Gamma*PI/180.;	// точка 1
			t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
			line1.Set(pt0, t, t, pt3);
			line1.BuildBallArc(Gamma);
*/
			alf = Gamma*PI/180.;	// точка 1
			x = 1;
			t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
			pt1 = (pt0-t) + (pt3-t);
			if (pt1.D2() > 0.)
				pt1 = pt1*(1/sqrt(pt1.D2()));
			if (IsTop)
				x = Line.p[3].GetX()*(1/cos(alf/2.));
			else
				x = Line.p[0].GetX()*(1/cos(alf/2.));
			pt1 = t + pt1*fabs(x);
									
			h = sqrt(pt.Dist2(pt0,pt3)/pt.Dist2(pt0,pt1))*0.5;
		
			line1.Set(pt0, pt1*h, pt1*h, pt3);
		
			// посроение задней линии
			if (flag)
			{
				
				pt1 = pt + direct*length;
				if (i == 0)
				{
					if (type == surCWARC || type == surCCWARC)
					{
						m.RotZ(pt, -Gamma-arcAngle/2);
					}
					else
					{
						m.RotZ(pt, -Gamma);
					}
					L1 = L0 * m;
					m.Trans(pt, pt1);
					L1 = L1 * m;
				}
				pt = pt1;
			
				if (IsTop)				// точка 0
					pt0 = L1.p[3];	
				else
					pt0 = L1.p[0];	

				m.RotZ(pt,-Gamma);
				L1 = L1 * m;
				if (IsTop)				// точка 3
					pt3 = L1.p[3];
				else
					pt3 = L1.p[0];

										// точка 1
/*				t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
				line2.Set(pt0, t, t, pt3);
				line2.BuildBallArc(Gamma);
*/
				alf = Gamma*PI/180.;	// точка 1
				x = 1;
				t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
				pt1 = (pt0-t) + (pt3-t);
				if (pt1.D2() > 0.)
					pt1 = pt1*(1/sqrt(pt1.D2()));
				if (IsTop)
					x = Line.p[3].GetX()*(1/cos(alf/2.));
				else
					x = Line.p[0].GetX()*(1/cos(alf/2.));
				pt1 = t + pt1*fabs(x);
										
				h = sqrt(pt.Dist2(pt0,pt3)/pt.Dist2(pt0,pt1))*0.5;
			
				line2.Set(pt0, pt1*h, pt1*h, pt3);
			}
			else
			{
				pt1 = pt + direct*length;
				m.Trans(pt, pt1);
				line2 = line1 * m;
			}

			break;
		case surCWARC: case surCCWARC:

			// центр вращения траектории
			t.Set(pt.GetX(), pt.GetY(), Line.p[0].GetZ(), 1);
			pt1 = t - DirLine.p[0];	
			t = Center + pt1;

			if (i == 0)
				m.RotZ(pt, BAlf);
			else
				m.RotZ(pt, BAlf+(i+1)*Gamma);

			L0 = Line * m;

			if (IsTop)				// точка 0
				pt0 = L0.p[3];
			else
				pt0 = L0.p[0];

			alf = arcAngle*180./PI;
			m.RotZ(t, alf);
			pt3 = pt0 * m;			// точка 3
			
			t.Norm();
			line1.Set(pt0, t, t, pt3);
			line1.BuildBallArc(arcAngle);

			m.Trans(Center, t);
			line2 = DirLine * m;			
			break;
		}
		//*******-------------
	/*	glDisable(GL_LIGHTING);

		glColor3d(1.,0.,0.);
		line1.Ball2Bernstain();
		line1.Draw();

		glColor3d(0.,1.,0.);
		line2.Ball2Bernstain();
		line2.Draw();

		glColor3d(1.,0.5,0.);

		glEnable(GL_LIGHTING);*/
		//----------------****

		// построение линейчатой поверхности
		line1.Ball2Bernstain();
		line2.Ball2Bernstain();

		pPiece = new NCubicPiece;
		BuildCubLinearSurface(line1, line2, IsTop, pPiece);
		pc.Add(pPiece);
	}
	flag = true;

	return true;
}
