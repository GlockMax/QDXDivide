// NSurFMSphere.cpp: implementation of the NSurFMSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSurFMSphere.h"
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

NSurFMSphere::NSurFMSphere()
{

}

NSurFMSphere::~NSurFMSphere()
{

}

// задание образующей
void NSurFMSphere::SetForming(const NLine nl, 
							  const double angle, 
							  const double beta, 
							  bool bl)
{
// переданные значения alf и beta условны, они будут 
// определены в этой функции
	Line = nl;
	Alf = 180.;
	BAlf = beta;
	QAlf = angle;

	double alf, x;
	BPoint pt0, pt1, pt2;
	// определение центра сферы
	pt0 = Line.p[0];
	pt1 = Line.p[1];
	pt2 = Line.p[3];
	pt0.Norm();
	pt1.Norm();
	pt2.Norm();
	cent_sp = (pt0-pt1)+(pt2-pt1);
	if (cent_sp.D2() > 0.)
		cent_sp = cent_sp*(1/sqrt(cent_sp.D2()));
	alf = pt1.Cos2((pt0-pt1),(pt2-pt1));	// cos2 2*alf
	alf = alf*sqrt((pt0-pt1).D2())*sqrt((pt2-pt1).D2());
	alf = sqrt((alf+1)/2);					// cos alf
	x = sqrt((pt0-pt1).D2())/alf;
	cent_sp = pt1 + cent_sp*x;

	// уточнение угла QAlf
	pt1.Set(1., 0., 0., 0.);
	pt2 = pt0 - cent_sp;

	GAlf = pt1.Cos2(pt1,pt2);
	GAlf = GAlf*sqrt(pt1.D2())*sqrt(pt2.D2());
	GAlf = acos(GAlf);
	GAlf = 180.*GAlf/PI;
	QAlf = QAlf - GAlf;
}

// построение общей поверхности для участка конуса
int NSurFMSphere::BuildSurface(PartArray& pcs, BuildingType BuildType)
{
	if (Alf == 0. || Alf < 0.)
		return -1;

	BPoint pt0;
	BPoint z(0.,0.,1.,0.), dir = End - Start;

	if (QAlf == 180.)
	{
		double alf_old = Alf, beta_old = BAlf;
		Alf = 360; BAlf = 0;
		pt0.Set(0.,0.,0.,1.);
		if (z*dir > 0)
		{
			if(!BuildSurfaceRotation(pcs, pt0))
				return 4;
		}
		else
		{
			pt0 = pt0+dir;
			if(!BuildSurfaceRotation(pcs, pt0))
				return 4;
		}
		Alf = alf_old;
		BAlf = beta_old;
	}
	else
	{
		if (BuildType == surBuildAll	|| 
			BuildType == surFront		||
			BuildType == surFrontALateral)
			if (!BuildFrontSurface(pcs)) 
				return 1;

		if (BuildType == surBuildAll	|| 
			BuildType == surBack 		||
			BuildType == surLateralABack)
			if (!BuildBackSurface(pcs)) 
				return 2;

		if (BuildType == surBuildAll	|| 
			BuildType == surFrontALateral ||
			BuildType == surLateral ||
			BuildType == surLateralABack)
			if (!BuildLateralSurface(pcs)) 
				return 3;
	}

	return 0;
}

// построить переднюю поверхность
bool NSurFMSphere::BuildFrontSurface(PartArray& pc)
{
	int n=0, k, singl;
	double x, r, h1, h2, alf, Gamma;
	BMatr m;
	BPoint pt, pt0, pt1, pt2, pt12, c, t;
	NSquarePiece *pPiece;
	DirectionType type = GetTypeDirection();
	
	pt0 = Line.p[0];
	// определение числа порций образующих переднюю поверхность
	if (fabs(Alf) < 175.)
		n = 1;
	else
		n = 2;

	Gamma = Alf/n;
	c = cent_sp;
	r = sqrt(c.Dist2(pt0, c));
	
	for (int i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(0.,0.,0.,1.);
		
		if (i == 0)
		{
			m.RotZ(pt, BAlf);		// точка 0
			pt0 = pt0 * m;			
									// точка 1
			pt12.Set(c.GetX(), c.GetY(), pt0.GetZ(), 1.);
			m.RotZ(pt, Gamma);		
			pt1 = pt0 * m;
			m.rotg(GAlf, c, pt12-pt0);
			pt1 = pt1 * m;
			m.rotg(QAlf, c, pt12-pt0);	// точка 2
			pt2 = pt1 * m;
			singl = 1;
			k = 0;
		}
		else
		{
			m.RotZ(pt, Alf);
			pt0 = pt0 * m;			// точка 0
			singl = -1;
			k = 2;
		}

		// линия 00-03 (точки 1-2)
		alf = QAlf*PI/180.;
		x = 1;
		pt12 = (pt1-c) + (pt2-c);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/cos(alf/2.));
		
		pt12 = c + pt12*fabs(x);

		h1 = sqrt(pt.Dist2(pt1,pt2)/pt.Dist2(pt1,pt12))*0.5;

		pPiece->pc[k+singl*0][0] = pt1.Norm();
		pPiece->pc[k+singl*0][1] = pt12.Norm()*h1;
		pPiece->pc[k+singl*0][2] = pt2.Norm();

		// линия 30-33 (точки 0-0)
		pPiece->pc[k+singl*2][0] = pt0.Norm();
		pPiece->pc[k+singl*2][1] = pt0.Norm()*h1;
		pPiece->pc[k+singl*2][2] = pt0.Norm();

		// линия 00-30 (точки 1-0)
		alf = Gamma*PI/180.;
		x = 1;
		t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
		pt12 = (pt1-t) + (pt0-t);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/cos(alf/2.));
		
		pt12 = t + pt12*fabs(x);
								
		h2 = sqrt(pt.Dist2(pt1,pt0)/pt.Dist2(pt1,pt12))*0.5;

		pPiece->pc[k+singl*1][0] = pt12.Norm()*h2;

		// линия 03-33 (точки 2-0)
		alf = Gamma*PI/180.;
		x = 1;
		t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
		pt12 = (pt2-t) + (pt0-t);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/cos(alf/2.));
		
		pt12 = t + pt12*fabs(x);

		pPiece->pc[k+singl*1][2] = pt12.Norm()*h2;

		// линия 01-31 (точки pc[0][1] - pc[2][1])
		pt = pPiece->pc[0][1];
		pt12 = pPiece->pc[2][1];
		pt.Norm();
		pt12.Norm();
		pt12 = pt12-c;
		pt12 = pt + pt12;
		
		pPiece->pc[k+singl*1][1] = pt12.Norm()*h1*h2;


		pc.Add(pPiece);
	}

	return true;
}


// построить боковую поверхность
bool NSurFMSphere::BuildLateralSurface(PartArray& pc)
{
	double alf, x, h, r;
	NLine line1, line2;
	BMatr m;
	BPoint pt, pt0, pt1, pt2, pt3, pt12, c, CenTr, direct;
	NSquarePiece *pPiece;
	DirectionType type = GetTypeDirection();
	
	direct = End - Start;
	pt0 = Line.p[0];
	c = cent_sp;
	r = sqrt(c.Dist2(pt0, c));

	pPiece = new NSquarePiece;
	pt.Set(0.,0.,0.,1.);

	m.RotZ(pt, BAlf);
	pt0 = pt0 * m;			// точка 0
	m.RotZ(pt, Alf/2.);		
	pt1 = pt0 * m;
	m.rotg(QAlf, c, c-pt0);
	pt2 = pt1 * m;			// точка 2
	pt3 = pt2;				// точка 3
	m.RotZ(pt, Alf);		// точка 1
	pt1 = pt0 * m;
	m.rotg(GAlf, c, c-pt0);
	pt1 = pt1 * m;
	c = cent_sp;				// центр сферы
	CenTr =  pt - DirLine.p[0];	// центр вращения траектории
	CenTr = Center + CenTr;
	
	switch(type)	
	{
	case surLINE:
	// построение первой поверхности
		// первая линия
		alf = pt1.Cos2((pt0-c),(pt2-c));
		alf = alf*sqrt((pt0-c).D2())*sqrt((pt2-c).D2()); //cos 2*alf
		alf = sqrt((alf+1)/2);							//cos alf
		x = 1;
		pt12 = (pt0-c) + (pt2-c);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/alf);
		
		pt12 = c + pt12*fabs(x);

		h = sqrt(pt.Dist2(pt0,pt2)/pt.Dist2(pt0,pt12))*0.5;

		line1.p[0] = pt0;
		line1.p[1] = pt12.Norm()*h;
		line1.p[2] = pt12.Norm()*h;
		line1.p[3] = pt2;

		// вторая линия
		pt12 = pt + direct;
		m.Trans(pt, pt12);
		line2 = line1 * m;
		// построение линейчатой поверхности
		BuildSqLinearSurface(line1, line2, false, pPiece);

		pc.Add(pPiece);

	// построение второй поверхности
		pPiece = new NSquarePiece;
		// первая линия
		alf = pt1.Cos2((pt1-c),(pt3-c));
		alf = alf*sqrt((pt1-c).D2())*sqrt((pt3-c).D2());//cos 2*alf
		alf = sqrt((alf+1)/2);				//cos alf
		x = 1;
		pt12 = (pt1-c) + (pt3-c);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/alf);
		
		pt12 = c + pt12*fabs(x);

		line1.p[0] = pt1;
		line1.p[1] = pt12.Norm()*h;
		line1.p[2] = pt12.Norm()*h;
		line1.p[3] = pt3;

		// вторая линия
		pt12 = pt + direct;
		m.Trans(pt, pt12);
		line2 = line1 * m;
		// построение линейчатой поверхности
		BuildSqLinearSurface(line1, line2, true, pPiece);

		pc.Add(pPiece);

		break;

	case surCWARC: case surCCWARC:
		// построение первой поверхности
		// первая линия
		alf = pt1.Cos2((pt0-c),(pt2-c));
		alf = alf*sqrt((pt0-c).D2())*sqrt((pt2-c).D2()); //cos 2*alf
		alf = sqrt((alf+1)/2);							//cos alf
		x = 1;
		pt12 = (pt0-c) + (pt2-c);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/alf);
		
		pt12 = c + pt12*fabs(x);

		h = sqrt(pt.Dist2(pt0,pt2)/pt.Dist2(pt0,pt12))*0.5;

		line1.p[0] = pt0;
		line1.p[1] = pt12.Norm()*h;
		line1.p[2] = pt12.Norm()*h;
		line1.p[3] = pt2;

		// построение поверхности
		BuildArcMoveArcSurface(line1, pPiece, CenTr, arcAngle*180./PI);

		pc.Add(pPiece);
		
	// построение второй поверхности
		pPiece = new NSquarePiece;
		// первая линия
		alf = pt1.Cos2((pt1-c),(pt3-c));
		alf = alf*sqrt((pt1-c).D2())*sqrt((pt3-c).D2());//cos 2*alf
		alf = sqrt((alf+1)/2);				//cos alf
		x = 1;
		pt12 = (pt1-c) + (pt3-c);
		if (pt12.D2() > 0.)
			pt12 = pt12*(1/sqrt(pt12.D2()));
		
		x = r*(1/alf);
		
		pt12 = c + pt12*fabs(x);

		line1.p[0] = pt1;
		line1.p[1] = pt12.Norm()*h;
		line1.p[2] = pt12.Norm()*h;
		line1.p[3] = pt3;

		// построение поверхности
		BuildArcMoveArcSurface(line1, pPiece, CenTr, arcAngle*180./PI);

		pc.Add(pPiece);
		break;
	}

	return true;
}

//построить заднюю поверхность
bool NSurFMSphere::BuildBackSurface(PartArray& pc)
{
	int n=0, k, singl;
	double x, h1, h2, alf, r, Gamma;
	BMatr m;
	BPoint pt, pt0, pt1, pt2, pt12, c, t, direct;
	NSquarePiece *pPiece;
	DirectionType type = GetTypeDirection();

	direct = End - Start;
	pt0 = Line.p[0];
	// определение числа порций образующих заднюю поверхность
	Gamma = Alf - 360;
	if (fabs(Gamma) < 175.)
		n = 1;
	else
		n = 2;
		
	Gamma = Gamma/n;
	c = cent_sp + direct;
	r = sqrt(c.Dist2(pt0, cent_sp));
	
	for (int i=0; i<n; i++)
	{
		pPiece = new NSquarePiece;
		pt.Set(0.,0.,0.,1.);
		pt = pt + direct;
		
		if (i == 0)
		{
			pt0 = pt0 + direct;		// точка 0
			m.RotZ(pt, BAlf);
			pt0 = pt0 * m;
									// точка 1
			pt12.Set(c.GetX(), c.GetY(), pt0.GetZ(), 1.);
			m.RotZ(pt, Gamma);		
			pt1 = pt0 * m;
			m.rotg(-GAlf, c, pt12-pt0);
			pt1 = pt1 * m;
			m.rotg(-(180-QAlf-GAlf), c, pt12-pt0);
			pt2 = pt1 * m;			// точка 2
			singl = -1;
			k = 2;
		}
		else
		{
			m.RotZ(pt, Alf);
			pt0 = pt0 * m;			// точка 0
			singl = 1;
			k = 0;
		}

	/*	switch(type)
		{
		case surLINE:*/
			// линия 00-03 (точки 1-2)
			alf = (180-QAlf)*PI/180.;
			x = 1;
			pt12 = (pt1-c) + (pt2-c);
			if (pt12.D2() > 0.)
				pt12 = pt12*(1/sqrt(pt12.D2()));
			
			x = r*(1/cos(alf/2.));
			
			pt12 = c + pt12*fabs(x);

			h1 = sqrt(pt.Dist2(pt1,pt2)/pt.Dist2(pt1,pt12))*0.5;

			pPiece->pc[k+singl*0][0] = pt1.Norm();
			pPiece->pc[k+singl*0][1] = pt12.Norm()*h1;
			pPiece->pc[k+singl*0][2] = pt2.Norm();

			// линия 30-33 (точки 0-0)
			pPiece->pc[k+singl*2][0] = pt0.Norm();
			pPiece->pc[k+singl*2][1] = pt0.Norm()*h1;
			pPiece->pc[k+singl*2][2] = pt0.Norm();

			// линия 00-30 (точки 1-0)
			alf = fabs(Gamma)*PI/180.;
			x = 1;
			t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
			pt12 = (pt1-t) + (pt0-t);
			if (pt12.D2() > 0.)
				pt12 = pt12*(1/sqrt(pt12.D2()));
			
			x = r*(1/cos(alf/2.));
			
			pt12 = t + pt12*fabs(x);
									
			h2 = sqrt(pt.Dist2(pt1,pt0)/pt.Dist2(pt1,pt12))*0.5;

			pPiece->pc[k+singl*1][0] = pt12.Norm()*h2;

			// линия 03-33 (точки 2-0)
			alf = fabs(Gamma)*PI/180.;
			x = 1;
			t.Set(pt.GetX(), pt.GetY(), pt0.GetZ(), 1);
			pt12 = (pt2-t) + (pt0-t);
			if (pt12.D2() > 0.)
				pt12 = pt12*(1/sqrt(pt12.D2()));
			
			x = r*(1/cos(alf/2.));
			
			pt12 = t + pt12*fabs(x);

			pPiece->pc[k+singl*1][2] = pt12.Norm()*h2;

			// линия 01-31 (точки pc[0][1] - pc[2][1])
			pt = pPiece->pc[0][1];
			pt12 = pPiece->pc[2][1];
			pt.Norm();
			pt12.Norm();
			pt12 = pt12-c;
			pt12 = pt + pt12;

			pPiece->pc[k+singl*1][1] = pt12.Norm()*h1*h2;

	/*		break;

		case surCWARC:

			break;
		}*/

		if (type == surCWARC || type == surCCWARC)
		{
			if (type == surCWARC)
				m.RotZ(t, -arcAngle*180./PI);
			else
				m.RotZ(t, arcAngle*180./PI);
			pPiece->Modify(m);
		}


		pc.Add(pPiece);
	}

	return true;
}

// построение поверхности вращения
bool NSurFMSphere::BuildSurfaceRotation(PartArray &pc, BPoint st)
{
	int i, j, n=0;
	double x, h1, h2, alf, Gamma;
	NLine L0, L1, L3;
	BMatr m;
	BPoint pt, t;
	DirectionType type = GetTypeDirection();
	NSquarePiece *pPiece;

	L3 = Line;
	// определение числа порций образующих поверхность
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
			m.Trans(pt,st);
			L3 = L3 * m;
			m.RotZ(pt, BAlf);
			L0 = L3 * m;
		}
		else
			L0 = L3;
		pt = st;

		switch(type)
		{
		case surLINE:
			m.RotZ(pt, Gamma);
			L3 = L0 * m;

			h1 = sqrt(pt.Dist2(L0.p[0],L0.p[3])/pt.Dist2(L0.p[0],L0.p[1]))*0.5;

			pPiece->pc[0][0] = L0.p[0].Norm();
			pPiece->pc[1][0] = L0.p[1].Norm()*h1;
			pPiece->pc[2][0] = L0.p[3].Norm();			

			pPiece->pc[0][2] = L3.p[0].Norm();
			pPiece->pc[1][2] = L3.p[1].Norm()*h1;
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
			
			h2 = sqrt(pt.Dist2(L0.p[0],L3.p[0])/pt.Dist2(L0.p[0],L1.p[0]))*0.5;

			pPiece->pc[0][1] = L1.p[0]*h2;
			pPiece->pc[1][1] = L1.p[1]*h1*h2;
			pPiece->pc[2][1] = L1.p[3]*h2;
			break;

		case surCWARC:

			break;
		}

		pc.Add(pPiece);
	}

	return true;
}
