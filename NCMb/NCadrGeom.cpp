#include "NCadrGeom.h"
// NCadrGeom.cpp: implementation of the NCadrGeom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BFastMoveMan.h"
#include "BSpacePos.h"
#include "BMatr.h"
#include "NColor.h"
#include "NLine.h"
#include "BBox.h"
#include "NCEnums.h"
#include "CreateArc.h"
#include "SMachStateFix.h"
#include "BPointsBuf.h"
#include "math.h"
#include "NSurfSettings.h"
#include "NCMProject.h"
#include "SMachState.h"
#include "ncadrgeom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const unsigned char ATTR_MASK = 0xF;
const unsigned char POLAR_FLAG = 0x10;
const unsigned char CYLINDRIC_FLAG = 0x20;
NColor NCadrGeom::FastMovCol(0.f,0.f,1.f,1.f);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCadrGeom::NCadrGeom()
{
	Clear();
}

NCadrGeom::~NCadrGeom()
{
	//if(type == rotate)
	//	delete pCurve;
}


void NCadrGeom::Set(Curve typei,
					double xbi, double ybi, double zbi,
					double xei, double yei, double zei,
					double ii, double ji, double ki,
					enum Plane plane)
{
	type = typei;
	xb = xbi;
	yb = ybi;
	zb = zbi;
	xe = xei;
	ye = yei;
	ze = zei;
	i = ii;
	j = ji;
	k = ki;
	SetPl(plane);
}
void NCadrGeom::Set(Curve typei,
					double xbi, double ybi, double zbi,
					double xei, double yei, double zei)
{
	Set(typei,xbi,ybi,zbi,xei,yei,zei,
		i,j,k,pl);// keep i j k values
}

void NCadrGeom::Set(Curve type, const BPoint &Pb, const BPoint &Pe)
{
	Set(type, Pb.GetX(), Pb.GetY(), Pb.GetZ(), Pe.GetX(), Pe.GetY(), Pe.GetZ());
}

void NCadrGeom::SetE(double xei, double yei, double zei)
{
	xe = xei;
	ye = yei;
	ze = zei;
}
void NCadrGeom::SetN(double xni, double yni, double zni)
{
	xn = xni;
	yn = yni;
	zn = zni;
}
void NCadrGeom::SetI(double xii,double yii,double zii)
{
	i = xii;
	j = yii;
	k = zii;
}
void NCadrGeom::SetB(double xbi, double ybi, double zbi)
{
	if(type.type == undef)
		return;
	if(type.type == cwarc || type.type == ccwarc)
	{
		BPoint I = GetC() - BPoint(xbi,ybi,zbi,1.);
		i = I.GetX();
		j = I.GetY();
		k = I.GetZ();
	}
	xb = xbi;
	yb = ybi;
	zb = zbi;
}
void NCadrGeom::SetE(const BPoint & IP)
{
	xe = IP.GetX();
	ye = IP.GetY();
	ze = IP.GetZ();
}
void NCadrGeom::SetN(const BPoint & IP)
{
	xn = IP.GetX();
	yn = IP.GetY();
	zn = IP.GetZ();
}
void NCadrGeom::SetI(const BPoint & IP)
{
	i = IP.GetX();
	j = IP.GetY();
	k = IP.GetZ();
}
void NCadrGeom::SetB(const BPoint & IP)
{
	SetB(IP.GetX(), IP.GetY(),IP.GetZ());
}

double NCadrGeom::Length() const
{
	double L = 0.;
	switch(type.type)
	{
	case cwarc:
	case ccwarc:
	{
		double R = GetR();
		double Length = 0;
		if(IsFullArc(FARC))
		{
			Length = 2. * PI * R;
		}
		else
		{
			bool LeftCenter = ((GetE() - GetB()) % (GetC() - GetB())) * GetN() > 0.;
			bool SmallArc = (LeftCenter == (type.type == ccwarc));
			if( R != 0.)
			{
				double D = sqrt((GetE() - GetB()).D2());
				if( D < DMIN )
					Length = 2. * PI * R;
				else
				{
					double SinA2 = min( 1., D / (2. * R)); // to prevent SinA2 > 1
					double A2 = asin(SinA2);
					Length = 2. * R * (SmallArc ? A2 : (PI - A2));
				}
			}
		}
		if(!IsPolar() && !IsCylindric())
			Length += InterpNum * 2. * PI * R; // InterpNum here is a number of the full circles
		if (Is3DArc())
		{
			Length = Length + InterpNum * 2. * PI * R; // InterpNum here is a number of the full circles
			double Zdist = (GetE() - GetB()) * GetN();
			Length = sqrt(Length * Length + Zdist * Zdist);
		}
		return Length;
	}
	case line:
		if(IsSpline() && pCurve)
			return pCurve->GetLength();
		L=(xe-xb)*(xe-xb)+(ye-yb)*(ye-yb)+(ze-zb)*(ze-zb);
		if(L<=0.)
			return 0.;
		else
			return sqrt(L);
	case rotate:
		if(pCurve)
			return pCurve->GetLength();
	default:
		return 0.;
	}
}
double NCadrGeom::GetRotSpecLength() const
{
	BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.);
	CalcCurCoord(0., XYZ0, ABC0);
	BPoint XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
	CalcCurCoord(1., XYZ1, ABC1);
	XYZ0 = XYZ1 - XYZ0;
	ABC0 = ABC1 - ABC0;
	double MaxA = fmax(fabs(ABC0.GetX()), fmax(fabs(ABC0.GetY()), fabs(ABC0.GetZ())));
	return sqrt(XYZ0.D2() + MaxA * MaxA);
}
double NCadrGeom::Deflect() const
{
	// Отклонение от хорды
	if(IsArc())
	{
		double r = GetR();
		return fabs(r * (1. - cos(Length() / (4. * PI * r))));
	}
	return 0.;
}

bool NCadrGeom::SetABSIJ(enum Plane pl)
{

	double Ax,Ay,Az,d,Arx,Ary,Arz;
	switch(pl)
	{
	case XY:
		// Pi=A+Ar*d
		// A=(Pe-Pb)/2
		// d=sqrt(r^2-A^2)/|A|
		// Ar - rotate A +90 - ccw; -90 - cw
		Ax=(xe-xb)/2;Ay=(ye-yb)/2;
		d=Ax*Ax+Ay*Ay;
		if(d < DMIN * DMIN)
			return false;
		d=(i*i+j*j-d)/d;
		if(d < - DMIN * DMIN)
			return false;
		d = sqrt(fabs(d));
		switch(type.type)
		{
		case ccwarc:
			Arx=-Ay;Ary=Ax;
			break;
		case cwarc:
			Arx=Ay;Ary=-Ax;
			break;
		default:
			return false;
		}
		i=Ax+Arx*d;
		j=Ay+Ary*d;
		return true;
	case XZ:
		// Pi=A+Ar*d
		// A=(Pe-Pb)/2
		// d=sqrt(r^2-A^2)/|A|
		// Ar - rotate A +90 - ccw; -90 - cw
		Ax=(xe-xb)/2;Az=(ze-zb)/2;
		d=Ax*Ax+Az*Az;
		if(d < DMIN * DMIN)
			return false;
		d=(i*i+k*k-d)/d;
		if(d < - DMIN * DMIN)
			return false;
		d = sqrt(fabs(d));
		switch(type.type)
		{
		case ccwarc:
			Arx=Az;Arz=-Ax;
			break;
		case cwarc:
			Arx=-Az;Arz=Ax;
			break;
		default:
			return false;
		}
		i=Ax+Arx*d;
		k=Az+Arz*d;
		return true;
	case YZ:
		// Pi=A+Ar*d
		// A=(Pe-Pb)/2
		// d=sqrt(r^2-A^2)/|A|
		// Ar - rotate A +90 - ccw; -90 - cw
		Ay=(ye-yb)/2;Az=(ze-zb)/2;
		d=Ay*Ay+Az*Az;
		if(d < DMIN * DMIN)
			return false;
		d=(k*k+j*j-d)/d;
		if(d < - DMIN * DMIN)
			return false;
		d = sqrt(fabs(d));
		switch(type.type)
		{
		case ccwarc:
			Ary=-Az;Arz=Ay;
			break;
		case cwarc:
			Ary=Az;Arz=-Ay;
			break;
		default:
			return false;
		}
		j=Ay+Ary*d;
		k=Az+Arz*d;
		return true;
	default:
		return false;
	}
}

void NCadrGeom::Offset(enum LR dir, double dist)
{
	if(!HaveGeom())
		return;
	if(type.type != cwarc && type.type != ccwarc)
		return;
	double sign;
	if(type.type == ccwarc && dir == RIGHT ||
		type.type == cwarc && dir == LEFT)
		sign = 1.; // outside circle
	else
		sign = -1.;// inside circle

	BPoint Pq(GetC() - GetE()); // Pq - Pcenter-Pe
	Pq = Pq - GetN() * (Pq * GetN());// Project Pq to the plane orthogonal to N
	double d = i * i + j * j + k * k;
	if(d < DMIN * DMIN) return;
	d = sqrt(1./d);
	SetB(GetB() - GetI() * (sign * dist * d)); 
	SetE(GetE() - Pq * (sign * dist * d)); 
}

void NCadrGeom::ShiftZ(double s)
{
	if(!HaveGeom())
		return;
	zb = zb + s;
	ze = ze + s;
}

void NCadrGeom::Clear()
{
	type.type = undef;
	type.flags = 0;
	Attr = 0;
	pl = XY;
	xn = 0.; yn = 0.; zn = 1.;
	MatrNum = 0;
	MSHistNum = 0;
	InterpNum = 0;
	MTLink.Clear();
	StockPos = 0;
	pCurve = NULL;
	Times.Clear();
	TimeInd = 0;
	HistID = cadrID(-1);
}

void NCadrGeom::ExpandBox(BBox *pBox) const
{
	if(type.type == rotate || IsCylindric() || IsSpline())
	{
		if(pCurve)
		{
			BBox Box = pCurve->GetGabar();
			pBox->Expand(Box);
			return;
		}
	}
	if(!HaveGeom())
		return;
	pBox->Expand(xb,yb,zb);
	pBox->Expand(xe,ye,ze);
	if(type.type == ccwarc || type.type == cwarc)
	{

// create arc control points
		float *ctlarray,*knot;
		int nknots = BPoint::CreateArc(	BPoint(xb,yb,zb,1.),
								BPoint(xe,ye,ze,1.),
								BPoint(i,j,k,0.),
								BPoint(xn,yn,zn,0.),
								type.type,ctlarray,knot,ARCEPS);
		if(nknots == 0)
			return;
//	ctlarray = new float[12*n+4];
//	knot = new float[5+n*3];

// Fill gabar
		double z;
		int n = nknots-4;// Number of points in ctlarray
		for(int i=0,k=0 ; i<n ;i++,k+=4)
		{
			if(ctlarray[k+3] > 0.)
			{
				z = 1./ctlarray[k+3];
				pBox->Expand(	ctlarray[k]*z,
								ctlarray[k+1]*z,
								ctlarray[k+2]*z);
			}
		}
		delete[] ctlarray;
		delete[] knot;
	}

//	if(pCycle != NULL)
//		pBox->Expand(pCycle->Gabar);

}

void NCadrGeom::ExpandWorkBox(BBox *pBox) const
{
	if(!IsFast() && type.type != rotate)
		ExpandBox(pBox);
}

bool NCadrGeom::HaveGeom() const
{
	if(type.type == undef || type.type == rotate || type.type == addcoord)
		return FALSE;
	return TRUE;
}

bool NCadrGeom::HaveGeom5() const
{
	if(type.type == undef || type.type == addcoord)
		return FALSE;
	return TRUE;
}

unsigned short NCadrGeom::GetAttr() const noexcept
{
	return Attr & ATTR_MASK;
}

void NCadrGeom::SetType(Curve val)
{
	type = val;
}

void NCadrGeom::SetType(curve icurve)
{
	bool sptu = (type.flags & CU_SPTU) != 0;
	SetType(Curve(icurve));
	type.flags |= sptu ? CU_SPTU : 0; // CU_SPTU must be saved. Other flags we remove for compatibility (may be they should be saved too)
}

void NCadrGeom::SetAttr(unsigned char val)
{
// Attributes:
//	1 - Cycle
//  2 - Spindle off
//  3 - Разметка
//  4 - Кернение
//  5 - Large feed
//  6 - Don't cut
//  7 - Thread
	Attr = Attr & (~ATTR_MASK) | val & ATTR_MASK;
}

enum Plane NCadrGeom::GetPlane() const
{
	return pl;
}

double NCadrGeom::Angle(const NCadrGeom &PrevCadr) const
{
	if( !HaveGeom() || !PrevCadr.HaveGeom())
		return 0.;

	if( (type.type == line) && (PrevCadr.type.type == line))
	{
		BPoint	v0 = PrevCadr.GetE() -  PrevCadr.GetB();
		return v0.Angle(GetE() - GetB());
	}

	if(	(type.type == cwarc || type.type == ccwarc) &&
		(PrevCadr.type.type == cwarc || PrevCadr.type.type == ccwarc))
	{
		if( pl == PrevCadr.pl )
		{
			BPoint v0 = GetB() - GetC();
			BPoint v1 = PrevCadr.GetE() - PrevCadr.GetC();

			if ( type.type != PrevCadr.type.type )
				v0 = v0 * -1.;
			return v1.Angle(v0);
		}
		// Arcs lie in the different planes
		BPoint v0 = PrevCadr.GetE() - PrevCadr.GetC();
		BMatr m;
		m.rotg(90.,PrevCadr.GetC(),PrevCadr.GetN());
		v0 = v0 * m;

		BPoint v1 = GetB() - GetC();
		m.rotg(90.,GetC(),GetN());
		v1 = v1 * m;
		return v0.Angle(v1);
	}
	// this and PrevCadr have different types
	BPoint v0(0., 0., 0., 1.);
	if( type.type == cwarc || type.type == ccwarc)
	{
		v0 = GetB() - GetC();
		double RotAng = type.type == ccwarc ? 90. : -90.;
		BMatr m;
		m.rotg(RotAng,GetC(),GetN());
		v0 = v0 * m;
	}
	else
		v0 = GetE() - GetB();

	BPoint v1(0., 0., 0., 1.);
	if( PrevCadr.type.type == cwarc || PrevCadr.type.type == ccwarc)
	{
		v1 = PrevCadr.GetE() - PrevCadr.GetC();
		double RotAng = PrevCadr.type.type == ccwarc ? 90. : -90.;
		BMatr m;
		m.rotg(RotAng,PrevCadr.GetC(),PrevCadr.GetN());
		v1 = v1 * m;
	}
	else
		v1 = PrevCadr.GetE() - PrevCadr.GetB();

	return v1.Angle(v0);
}

const NCadrGeom &NCadrGeom::Tr(const BMatr &m)
{
	if(type.type == rotate && pCurve)
	{
		pCurve->Tr(m);
		return *this;
	}
	if(!HaveGeom())
		return *this;
// Transform geom according to matrix m
	BPoint Pb = BPoint(xb,yb,zb,1.)*m;
	BPoint Pe = BPoint(xe,ye,ze,1.)*m;
	BPoint Ri = BPoint(i,j,k,0.)*m;
	BPoint Rn = BPoint(xn,yn,zn,0.)*m;
	xb = Pb.GetX();yb = Pb.GetY();zb = Pb.GetZ();
	xe = Pe.GetX();ye = Pe.GetY();ze = Pe.GetZ();
	i  = Ri.GetX();j  = Ri.GetY();k  = Ri.GetZ();
	xn = Rn.GetX();yn = Rn.GetY();zn = Rn.GetZ();
	pl = PlaneByNorm(Rn);
	if(IsSpline() && pCurve)
		pCurve->Tr(m);
	return *this;
}
const NCadrGeom &NCadrGeom::SetX(double Val)
{
	if(type.type == rotate && pCurve)
	{
		for(int sn = 0; sn < pCurve->GetSize(); ++sn)
		{
			BPoint *pP = pCurve->GetSegm(sn);
			pP->Setx(Val);
		}
		return *this;
	}
	if(!HaveGeom())
		return *this;
	xb = Val;
	xe = Val;
	i = 0;
	pl = YZ;
	yn = 0.;
	zn = 0.;
	if(IsSpline() && pCurve)
	{
		for(int sn = 0; sn < pCurve->GetSize(); ++sn)
		{
			BPoint *pP = pCurve->GetSegm(sn);
			pP->Setx(Val);
		}
	}

	return *this;
}
const NCadrGeom &NCadrGeom::SetY(double Val)
{
	if(type.type == rotate && pCurve)
	{
		for(int sn = 0; sn < pCurve->GetSize(); ++sn)
		{
			BPoint *pP = pCurve->GetSegm(sn);
			pP->Sety(Val);
		}
		return *this;
	}
	if(!HaveGeom())
		return *this;
	yb = Val;
	ye = Val;
	i = 0;
	pl = XZ;
	xn = 0.;
	zn = 0.;
	if(IsSpline() && pCurve)
	{
		for(int sn = 0; sn < pCurve->GetSize(); ++sn)
		{
			BPoint *pP = pCurve->GetSegm(sn);
			pP->Sety(Val);
		}
	}

	return *this;
}
const NCadrGeom &NCadrGeom::SetZ(double Val)
{
	if(type.type == rotate && pCurve)
	{
		for(int sn = 0; sn < pCurve->GetSize(); ++sn)
		{
			BPoint *pP = pCurve->GetSegm(sn);
			pP->Setz(Val);
		}
		return *this;
	}
	if(!HaveGeom())
		return *this;
	zb = Val;
	ze = Val;
	k = 0;
	pl = XY;
	yn = 0.;
	xn = 0.;
	if(IsSpline() && pCurve)
	{
		for(int sn = 0; sn < pCurve->GetSize(); ++sn)
		{
			BPoint *pP = pCurve->GetSegm(sn);
			pP->Setz(Val);
		}
	}

	return *this;
}
const NCadrGeom &NCadrGeom::TrEndPoint(const BMatr &m)
{
	if(!HaveGeom())
		return *this;
// Transform end point and arc center of geom according to matrix m
	BPoint Pe = BPoint(xe,ye,ze,1.)*m;
	BPoint Ri = BPoint(i+xb,j+yb,k+zb,0.)*m;
	BPoint Rn = BPoint(xn,yn,zn,0.)*m;
	xe = Pe.GetX();ye = Pe.GetY();ze = Pe.GetZ();
	i  = Ri.GetX()-xb;j  = Ri.GetY()-yb;k  = Ri.GetZ()-zb;
	xn = Rn.GetX();yn = Rn.GetY();zn = Rn.GetZ();
	return *this;
}

NCadrGeom::Curve NCadrGeom::GetType() const
{
	return type;
}

void NCadrGeom::GetI(double *pi, double *pj, double *pk) const
{
	*pi=i;
	*pj=j;
	*pk=k;

}
void NCadrGeom::GetB(double *px, double *py, double *pz) const
{
	*px=xb;
	*py=yb;
	*pz=zb;
}
void NCadrGeom::GetE(double *px, double *py, double *pz) const
{
	*px=xe;
	*py=ye;
	*pz=ze;
}

void NCadrGeom::GetN(double *px, double *py, double *pz) const
{
	*px=xn;
	*py=yn;
	*pz=zn;
}
void NCadrGeom::Move(BPoint r)
{
	if(!HaveGeom())
		return;
	xb += r.GetX();
	yb += r.GetY();
	zb += r.GetZ();
	xe += r.GetX();
	ye += r.GetY();
	ze += r.GetZ();
}

void NCadrGeom::GetC(double *px, double *py, double *pz) const
{
	*px=xb+i;
	*py=yb+j;
	*pz=zb+k;

}

double NCadrGeom::GetR() const
{
	double r = i*i + j*j + k*k;
	if(r == 0.)
		return 0.;
	return sqrt(r);
}

void NCadrGeom::GetA(double *px, double *py, double *pz) const
{
// Get angles in degrees
	double lz = Length();
	if(lz == 0)
	{
		*px = *py = *pz = 0.;
		return;
	}
	lz = 1./lz;
	double sc = 180./PI;
	*px = acos((xe - xb)*lz) * sc;
	*py = acos((ye - yb)*lz) * sc;
	*pz = acos((ze - zb)*lz) * sc;
}

void NCadrGeom::GetArcA(double *px, double *py, double *pz) const
{
// Get start, end and center angles of the arc in degrees
// px - start
// py - end
// pz - center
	if(type.type != cwarc && type.type != ccwarc)
	{
		*px = *py = *pz = 0.;
		return;
	}
	BPoint	pb(xb,yb,zb,1.),
			pe(xe,ye,ze,1.),
			pib(i,j,k,0.),
			pn(xn,yn,zn,0.);
	pe = pe + pn * ((pb - pe) * pn);// projected point
	BPoint pc = pb + pib;
	BPoint pie = pc - pe;
	double z = GetR();
	if(z == 0)
	{
		*px = *py = *pz = 0.;
		return;
	}
	z = 1./z;
	pib = pib*z;

	z = pie.D2();
	if(z == 0)
	{
		*px = *py = *pz = 0.;
		return;
	}
	z = 1./sqrt(fabs(z));
	pie = pie*z;

	double s,c,a;
	switch(pl)
	{
	case XY:
		c = -pib.GetX();
		s = -pib.GetY() * GetN().GetZ();
		c = c < -1. ? -1. : c;
		c = c >  1. ?  1. : c;
		a = acos(c);
		if(s < 0.)
			a = 2*PI - a;
		*px = a;
		c = -pie.GetX();
		s = -pie.GetY() * GetN().GetZ();
		c = c < -1. ? -1. : c;
		c = c >  1. ?  1. : c;
		a = acos(c);
		if(s < 0.)
			a = 2*PI - a;
		*py = a;
		*pz = *py - *px;
		if((pe - pb).ProjXY().D2() < FARC * FARC)
			*pz = 0.;
		break;
	case XZ:
		c = -pib.GetX();
		s = pib.GetZ() * GetN().GetY();
		c = c < -1. ? -1. : c;
		c = c >  1. ?  1. : c;
		a = acos(c);
		if(s < 0.)
			a = 2*PI - a;
		*px = a;
		c = -pie.GetX();
		s = pie.GetZ() * GetN().GetY();
		c = c < -1. ? -1. : c;
		c = c >  1. ?  1. : c;
		a = acos(c);
		if(s < 0.)
			a = 2*PI - a;
		*py = a;
		*pz = *py - *px;
		if((pe - pb).XY2XZ().ProjXY().D2() < FARC * FARC)
			*pz = 0.;
		break;
	case YZ:
		c = -pib.GetY();
		s = -pib.GetZ() * GetN().GetX();
		c = c < -1. ? -1. : c;
		c = c >  1. ?  1. : c;
		a = acos(c);
		if(s < 0.)
			a = 2*PI - a;
		*px = a;
		c = -pie.GetY();
		s = -pie.GetZ() * GetN().GetX();
		c = c < -1. ? -1. : c;
		c = c >  1. ?  1. : c;
		a = acos(c);
		if(s < 0.)
			a = 2*PI - a;
		*py = a;
		*pz = *py - *px;
		if((pe - pb).XY2YZ().ProjXY().D2() < FARC * FARC)
			*pz = 0.;
		break;
	default:
		*px = *py = 0.;
		*pz = (GetI() * (-1.)).Angle((GetE() - GetC()), GetN()) * PI / 180.;
		break;
	}
	switch(type.type)
	{
	case ccwarc:
		if( *pz < 1.e-12)
			*pz = 2*PI + *pz; 
		break;
	case cwarc:
		if( *pz > -1.e-12)
			*pz = -(2*PI - *pz); 
		break;
	default:
		*px = *py = *pz = 0.;
		return;
		break;
	}
	if(!IsPolar() && !IsCylindric())
		*pz += PI * 2. * InterpNum * (type.type == cwarc ? -1. : 1. ); // InterpNum here is a number of the full circles
	double sc = 180./PI;
	*px *= sc;
	*py *= sc;
	*pz *= sc;
}





BPoint NCadrGeom::GetB() const
{
	return BPoint(xb,yb,zb,1.);
}
BPoint NCadrGeom::GetE() const
{
	return BPoint(xe,ye,ze,1.);
}
BPoint NCadrGeom::GetBP() const
{
	if(type.type == rotate && pCurve)
		return pCurve->GetFirstP();

	return BPoint(xb,yb,zb,1.);
}
BPoint NCadrGeom::GetEP() const
{
	if(type.type == rotate && pCurve)
		return pCurve->GetLastP();

	return BPoint(xe,ye,ze,1.);
}
BPoint NCadrGeom::GetN() const
{
	return BPoint(xn,yn,zn,0.);
}
BPoint NCadrGeom::GetI() const
{
	return BPoint(i,j,k,0.);
}
BPoint NCadrGeom::GetC() const
{
	return BPoint(xb+i,yb+j,zb+k,1.);
}


bool NCadrGeom::IsGeomValid(double eps) const
{
	if( type.type == cwarc || type.type == ccwarc)
	{
		double R = GetR();
		BPoint Bp = GetB();
		BPoint Ep = GetE();
		BPoint Cp = GetC();
		if(fabs(R - sqrt((Bp-Cp).D2())) > eps)
			return false;
		if(fabs(R - sqrt((Ep-Cp).D2())) > eps)
			return false;
	}
	return true;
}

bool NCadrGeom::IsVertical(void) const
{
	return IsLine() &&
		(sqrt((xe - xb) * (xe - xb) + (ye - yb) * (ye - yb)) < F_D_VERT * F_D_VERT);
}

bool NCadrGeom::IsVerticalUp(void) const
{
	return IsVertical() && (ze >= zb);
}

bool NCadrGeom::IsHorizontal(void) const
{
	return fabs(ze - zb) < F_Z_HOR;
}

bool NCadrGeom::IsLine(void) const
{
	return type.type == line;
}

bool NCadrGeom::IsArc(void) const
{
	return type.type == cwarc || type.type == ccwarc;
}

bool NCadrGeom::IsFullArc(double eps) const
{
	return IsArc() && ((GetB() - GetE()).D2() < eps * eps);
}

bool NCadrGeom::IsFull3DArc(double eps) const
{
	BPoint B = BPoint::ProjectPointOnPlane(GetB(), GetE(), GetN());
	return IsArc() && ((B - GetE()).D2() < eps * eps);
}

bool NCadrGeom::Is3DArc(void) const
{
	return IsArc() 
		&& ( (fabs((GetB() - GetE()) * GetN()) > F_Z_HOR));
}

bool NCadrGeom::IsNotXYArc(void) const
{
	return (IsArc() && ( (fabs((GetB() - GetE()) * GetN()) > F_Z_HOR)
		|| pl != XY
		|| !IsPlaneCorrect()));
}

BPoint NCadrGeom::GetStartDirP(void) const
{
	if((type.type != rotate && !IsSpline()) || pCurve == NULL)
		return GetStartDir();
	return pCurve->GetFirstDir();
}
BPoint NCadrGeom::GetStartDir(void) const
{
	if(IsLine())
		return GetE() - GetB();
	if(IsArc())
	{
		BMatr m;
		return BPoint(i,j,k,0) * 
			m.rotg((type.type == ccwarc ) ? -90. : 90. ,BPoint(0.,0.,0.,1.),BPoint(xn,yn,zn,0.));
	}
	return BPoint(0.,0.,0.,0.);
}

BPoint NCadrGeom::GetEndDir(void) const
{
	if(IsLine())
		return GetE() - GetB();
	if(IsArc())
	{





		BMatr m;
		return (GetC() - GetE()) * 
			m.rotg((type.type == ccwarc ) ? -90. : 90. ,BPoint(0.,0.,0.,1.),BPoint(xn,yn,zn,0.));
	}
	return BPoint(0.,0.,0.,0.);
}

bool NCadrGeom::CrArc(const BPoint & P0, const BPoint & P1, const BPoint & e0)
{// create arc by two points and start tangent
 // works in the XY plane only
	xb = P0.GetX();
	yb = P0.GetY();
	zb = P0.GetZ();
	xe = P1.GetX();
	ye = P1.GetY();
	ze = P1.GetZ();
	xn = 0.;
	yn = 0.;
	zn = 1.;
	pl = XY;
	BPoint L(P1 - P0);
	BPoint Ls(-L.GetY(), L.GetX(), 0., 0.);// RotZ 90
	double d = Ls*e0;
	if(fabs(d) < MINAR)
	{
		type.type = line;
		return false;
	}
	type.type = (L % e0).GetZ() > 0 ? cwarc : ccwarc;
	BPoint I = (L - Ls * ((L*e0)/d)) * 0.5;
	i = I.GetX();
	j = I.GetY();
	k = I.GetZ();
	return true;
}
void NCadrGeom::FitCadrs(NCadrGeom *pPrev, NCadrGeom *pCur, bool BiArc)
{
	if(!pCur->HaveGeom())
		return;
	const double MinRad = NCM_PROJECT.GetDouble("MinRadArc", 0.05);
	const double MaxDfl = NCM_PROJECT.GetDouble("DevArcSpan", 2.e-4);
	if(pCur->IsArc())
	{
		if (pCur->GetI().D2() < MinRad * MinRad || pCur->Deflect() < MaxDfl)
		{// Replace arc with a line
			pCur->type.type = line;
		}
		if(pCur->IsNotXYArc())
			return;// Don't process 3D arcs
	}
	if(!pPrev)
		return;
	if(!pPrev->HaveGeom())
		return;
	if(pPrev->IsArc())
	{
		if(pPrev->IsNotXYArc())
			return;// Don't process 3D arcs
	}
	if(pPrev->IsFullArc(FARC))
		return;
	if(pCur->IsFullArc(FARC))
	{
		BPoint O = pCur->GetC();
		pCur->SetB(pCur->GetE());
		pCur->SetI(O - pCur->GetB());
		pPrev->SetE(pCur->GetB());
		return;
	}
	if(pCur->Length() < MIND)
	{
		if(pPrev->GetMatrNum() == pCur->GetMatrNum())// added 3.06.19 due to PolarSiemensBug 
			pPrev->SetE(pCur->GetE());//Может быть нужна проверка через IsGeomValid для дуги 
		pCur->SetType(undef);
		return;
	}
	if(pPrev->Length() < MIND)
		return;

	double Angle = pCur->Angle(*pPrev);
	if(fabs(Angle) >= MINATD)
		return;

	if(pPrev->IsArc() && pCur->IsArc() && BiArc)
	{// Create BiArc
		// Change endp point of pPrev, start point of pCur and I vectors of both
		BPoint P0 = pPrev->GetB();
		BPoint P = pPrev->GetE();
		BPoint P2 = pCur->GetE();
		BPoint I0 = pPrev->GetI();
		BPoint I2 = pCur->GetB() + pCur->GetI() - P2;
		BPoint Pr = (P0 + P2) * 0.5;
		double L2 = (P2 - P0).D2();
		if(L2 < MIND * MIND)
			return;
		double L = sqrt(L2);
		double PxL = (P - Pr) * (P2 - P0);
		double Px = PxL / L;
		BPoint N = pPrev->GetN();
		BPoint XV = (P2 - P0)*(1./L);
		BMatr M;
		M.rotg(90., BPoint(0.,0.,0.,1.), N);
		BPoint YV = XV * M;
		double Py = (P - Pr) * YV;
		double A0 = XV.Angle(I0, N) + ((pPrev->IsCWArc()) ? 90. : -90.);
		A0 = A0 > 180. ? A0 - 360. : A0;
		A0 = A0 < -180. ? 360. + A0 : A0;
		double A2 = -(XV.Angle(I2, N) + ((pCur->IsCWArc()) ? 90. : -90.));
		A2 = A2 > 180. ? A2 - 360. : A2;
		A2 = A2 < -180. ? 360. + A2 : A2;
		double A = (A0 + A2) * PI / 360.;
		double Z = sqrt(fabs(4. * (Px*Px + Py*Py) * tan(A) * tan(A) + L2 + 4. * Py * L * tan(A)));
		Px = PxL / (Z * fabs(cos(A)));

		double Ri = Px * (2. * sin(A)) / L;
		double D = 1. - Ri * Ri;
		double B = sqrt(fabs(D)) - cos(A);
	
		if(fabs(B) < 1.e-10)
			Py = 0;
		else
			Py = L * B / (2. * sin(A));

		P = Pr + XV * Px + YV * Py;
		if((P - pPrev->GetE()).D2() > 0.04) // С потолка. Не понятно нужно ли ограничение вообще
			return;
		pPrev->SetE(P);
		pCur->SetB(P);
		double Mu = 0.5 * (P - P0).D2() / ((P - P0) * I0 );
		I0 = I0 * Mu;
		pPrev->SetI(I0);
		double Nu = 0.5 * (P - P2).D2() / ((P - P2) * I2 );
		I2 = P2 + I2 * Nu - P;
		pCur->SetI(I2);
		return;
	}
	if(pPrev->IsArc() && pCur->IsLine())
	{
		BPoint Pb = pPrev->GetB();
		BPoint Pe = pPrev->GetE();
		BPoint P1 = pCur->GetE();
		BPoint I = pPrev->GetI();
		BPoint C = Pb + I;
		double Xc = I * (Pe - P1)/sqrt((Pe - P1).D2());
		if( fabs(Xc) < MIND )
			return;
		double Yb = sqrt(Pb.LineD2(P1, Pe));
		double Yc = sqrt(C.LineD2(P1, Pe));

		double d = sqrt((Yc - Yb) * (Yc - Yb) + Xc * Xc);
		double l1 = Yb * ((Yc - Yb) + d) / (Xc * Xc);
		double l2 = Yb * ((Yc - Yb) - d) / (Xc * Xc);
		if( fabs(l2 - 1.) < fabs(l1 - 1.))
			l1 = l2;
		C = Pb * ( 1. - l1) + C * l1;
		double r = (C - P1) * (Pe - P1)/(Pe - P1).D2();
		BPoint P = P1 * (1. - r) + Pe * r; 
		if((P - pPrev->GetE()).D2() > 0.04)
			return;
		if(fabs(sqrt((P - C).D2()) - sqrt(I.D2())) > DMIN)
			return;
		pPrev->SetE(P);
		pCur->SetB(P);
		I = C - Pb;
		pPrev->SetI(I);
		return;
	}
	if(pCur->IsArc() && pPrev->IsLine())
	{
		BPoint Pb = pCur->GetE();
		BPoint Pe = pCur->GetB();
		BPoint P1 = pPrev->GetB();
		BPoint C = Pe + pCur->GetI();
		BPoint I = C - Pb;
		double D2 = (Pe - P1).D2();
		if(D2 < MIND * MIND)
			return;
		double Xc = I * (Pe - P1)/sqrt(D2);
		if( fabs(Xc) < MIND )
			return;
		double Yb = sqrt(Pb.LineD2(P1, Pe));
		double Yc = sqrt(C.LineD2(P1, Pe));

		double d = sqrt((Yc - Yb) * (Yc - Yb) + Xc * Xc);
		double l1 = Yb * ((Yc - Yb) + d) / (Xc * Xc);
		double l2 = Yb * ((Yc - Yb) - d) / (Xc * Xc);
		if( fabs(l2 - 1.) < fabs(l1 - 1.))
			l1 = l2;
		C = Pb * ( 1. - l1) + C * l1;
		double r = (C - P1) * (Pe - P1)/(Pe - P1).D2();
		BPoint P = P1 * (1. - r) + Pe * r; 
		if((P - pPrev->GetE()).D2() > 0.04)
			return;
		if(fabs((pCur->GetE() - C).D2() - I.D2()) > DMIN)
			return;
		pPrev->SetE(P);
		pCur->SetB(P);
		I = C - Pe;
		pCur->SetI(I);
		return;
	}
}

void NCadrGeom::ShiftTime(MainTime Shift)
{
	Times.Start += Shift;
	Times.WorkStart += Shift;
	Times.WorkEnd += Shift;
	Times.End += Shift;
}

void NCadrGeom::ShiftStartTime2(MainTime Time)
{
	if (!IsTimeDefined())
		Times.Start = 0;
	ShiftTime(Time - Times.Start);
}

void NCadrGeom::ShiftWorkTime2(MainTime Time)
{
	auto Shift = Time - Times.WorkStart;
	Times.WorkStart = Time;
	Times.WorkEnd += Shift;
	Times.End += Shift;
}

void NCadrGeom::SetWorkTime(MainTime WorkTime)
{
	if (!IsTimeDefined())
		Times.Start = 0;
	Times.End += WorkTime - (Times.WorkEnd - Times.WorkStart);
	Times.WorkEnd = Times.WorkStart + WorkTime;
}

void NCadrGeom::SetWorkTimeS(double WorkTimeS)
{
	SetWorkTime(static_cast<MainTime>(WorkTimeS * Sec2Tick));
}

bool NCadrGeom::IsTimeDefined() const
{
	return Times.Start != MainT_UNDEF;
}

MainTime NCadrGeom::GetWorkTime() const
{
	return Times.WorkEnd - Times.WorkStart;
}

MainTime NCadrGeom::GetWholeTime() const
{
	return Times.End - Times.Start;
}

MainTime NCadrGeom::GetWaitTime0() const
{
	return Times.WorkStart - Times.Start;
}

MainTime NCadrGeom::GetWaitTime1() const
{
	return Times.End - Times.WorkEnd;
}

MainTime NCadrGeom::GetEndTime() const
{
	return Times.End;
}

MainTime NCadrGeom::GetStartTime() const
{
	return Times.Start;
}

MainTime NCadrGeom::GetMidTime(double t) const
{
	return Times.WorkStart + static_cast<MainTime>(t * GetWorkTime()) ;
}

void NCadrGeom::SetEndTime(MainTime EndTime)
{
	Times.End = EndTime;
}

double NCadrGeom::GetWorkTimeS() const
{
	return GetWorkTime() * Tick2Sec;
}

double NCadrGeom::GetWholeTimeS() const
{
	return GetWholeTime() * Tick2Sec;
}

double NCadrGeom::GetStartTimeS() const
{
	return GetStartTime() * Tick2Sec;
}

int NCadrGeom::GetMatrNum(void) const
{
	return MatrNum;
}
int NCadrGeom::GetMSHistNum(void) const
{
	return MSHistNum - 1;
}

void NCadrGeom::SetMatrNum(int n)
{
	MatrNum = n;
}

void NCadrGeom::SetMSHistNum(INT_PTR n)
{
	MSHistNum = unsigned short(n + 1);
}

void NCadrGeom::SetInterpNum(int n)
{
	InterpNum = n;
}

int NCadrGeom::GetInterpNum(void) const
{
	return InterpNum;
}

const NCadrGeom & NCadrGeom::Reverse(void)
{
	if(type.type == rotate)
	{
		double b;
		b = i; i = xn; xn = b;
		b = j; j = yn; yn = b;
		b = k; k = zn; zn = b;
	}
	else
	{
		if(type.type == ccwarc)
			type.type = cwarc;
		else if(type.type == cwarc)
			type.type = ccwarc;

		i = xb + i - xe;
		j = yb + j - ye;
		k = zb + k - ze;
	}

	double b;
	b = xb; xb = xe; xe = b;
	b = yb; yb = ye; ye = b;
	b = zb; zb = ze; ze = b;


	return *this;
}

bool NCadrGeom::MoveToXY(void)
{// Move this cadr from XZ to XY plane
 // OZ axis is transformed to OY axis
	BMatr M;
    Tr(M.RotX(0., 0., 0., -90.));
	zn = 1.;
	//switch (type.type)
	//{
	//case cwarc:
	//	type.type = ccwarc;
	//	break;
	//case ccwarc:
	//	type.type = cwarc;
	//	break;
	//}
	return true;
}

bool NCadrGeom::IsInXYPlane(void)
{
	if(fabs(zb) > F_Z_HOR || fabs(ze) > F_Z_HOR)
		return false;

	if(IsArc())
		if(fabs(k) > F_Z_HOR)
			return false;
	return true;
}

const NCadrGeom& NCadrGeom::ProjXY(void)
{
	if (!HaveGeom())
		return *this;
	zb = 0;
	ze = 0;
	k = 0;
	pl = XY;
	return *this;
}

const NCadrGeom& NCadrGeom::FlipXY(void)
{
	if (!HaveGeom())
		return *this;
	double b = xb;
	xb = yb;
	yb = b;

	b = xe;
	xe = ye;
	ye = b;

	b = i;
	i = j;
	j = b;

	if (type.type == ccwarc)
		type.type = cwarc;
	else 	if (type.type == cwarc)
		type.type = ccwarc;

	return *this;
}

const NCadrGeom & NCadrGeom::MakeHorizontal(void)
{
	if(!HaveGeom())
		return *this;
	ze = zb;
	k = 0;
	return *this;
}

const NCadrGeom & NCadrGeom::MakeHorizontal(double inZ)
{
	if(!HaveGeom())
		return *this;
	zb = inZ;
	return MakeHorizontal();
}

const NCadrGeom & NCadrGeom::MakeVertical(void)
{
	if(!IsLine())
		return *this;
	xb = xe;
	yb = ye;
	return *this;
}

bool NCadrGeom::DrawCyl(double Eps, double t) const
{
	return Draw5x(Eps, t);
}
bool NCadrGeom::DrawOrd(double Eps, double t) const
{
	if(!HaveGeom())
		return true;
	if(IsSpline() && pCurve)
		pCurve->Draw(t);
	NCadrGeom BGeom;
	const NCadrGeom *pGeom;
	if(GetMatrNum() != 0)
	{
		BGeom = *this;
		BGeom.Tr(BSpacePos::GetRotMatr(BGeom.GetMatrNum()));
		pGeom = &BGeom;
	}
	else
		pGeom = this;

	double xb,yb,zb,xe,ye,ze;
	pGeom->GetB(&xb,&yb,&zb);
	pGeom->GetE(&xe,&ye,&ze);
	switch(pGeom->GetType().type)
	{
	case line:
		if(t >= 0.)
		{
			xe = xb * (1. - t) + xe * t;
			ye = yb * (1. - t) + ye * t;
			ze = zb * (1. - t) + ze * t;
		}
		LineSimple(xb, yb, zb, xe, ye, ze);
		return true;
	case cwarc:
	case ccwarc:
		{
			BPointsBuf Pts;
			pGeom->GetPolyLine(Pts, Eps, false);
			glBegin(GL_LINE_STRIP);
			for(int i = 0; i < Pts.GetSize(); ++i)
			{
				const BPoint &P = Pts.GetPoint(i);
				glVertex3dv(P.GetArray());
			}
			glEnd();
			return true;

			//double i,j,k,xn,yn,zn;
			//pGeom->GetI(&i,&j,&k);
			//pGeom->GetN(&xn,&yn,&zn);
			//if(t >= 0.)
			//{
			//	double s, e, CenterAngle;
			//	pGeom->GetArcA(&s, &e, &CenterAngle);
			//	// Prevent unneeded full arcs
			//	if(t * fabs(CenterAngle) * PI * pGeom->GetR() / 360.  < Eps) // We compare a halfe of the true length
			//		return true;
			//	BMatr Rot;
			//	Rot.rotg(t * CenterAngle, pGeom->GetC(), pGeom->GetN());
			//	BPoint Pe = pGeom->GetB() * Rot;
			//	Pe.Get(xe, ye, ze, s);
			//}
			//return Arc(xb,yb,zb,xe,ye,ze,i,j,k,xn,yn,zn,pGeom->GetType().type,pGeom->GetPlane(), Eps);
		}
	default:
		if(IsFast())
		{
			//if(BFastMoveMan::PartInterp())
			//{
			//	FastLine(xb, yb, zb, xe, ye, ze);
			//	return true;
			//}
			LineSimple(xb, yb, zb, xe, ye, ze);
			return true;
		}
	}
	return true;
}
bool NCadrGeom::Draw(double Eps, double t) const
{
	bool res;
	if(IsFast())
		SetLineFont();
	if(IsCylindric())
		res = DrawCyl(Eps, t);
	else if(IsRotate())
		res = Draw5x(Eps, t);
	else
		res = DrawOrd(Eps, t);

	if(IsFast())
		RestoreLineFont();
	return res;
}

int NCadrGeom::compare_SPY(const void *a, const void *b)
{
	typedef NCadrGeom * intern;
	// функция для сравнения двух кадров по Y начальной точки
	double d = (*((intern *)a))->yb - (*((intern *)b))->yb;
	return (d < 0.) ? -1 : ((d > 0) ? 1 : 0);
}


bool NCadrGeom::IsCycle(void) const noexcept
{
	return GetAttr() == 1 || GetAttr() == 6 || GetAttr() == 7;
}

const BPoint NCadrGeom::NormByPlane(enum Plane Pl)
{
	switch(Pl)
	{
	case XY:
	default:
		return BPoint(0., 0., 1., 0.);
	case XZ:
		return BPoint(0., 1., 0., 0.);
	case YZ:
		return BPoint(1., 0., 0., 0.);
	}
}

enum Plane NCadrGeom::PlaneByNorm(const BPoint & N)
{
	if(fabs(N.Angle(BPoint(0.,0.,1.,0.))) < MINATD)
		return XY;
	if(fabs(N.Angle(BPoint(0.,1.,0.,0.))) < MINATD)
		return XZ;
	if(fabs(N.Angle(BPoint(1.,0.,0.,0.))) < MINATD)
		return YZ;
	if(fabs(N.Angle(BPoint(0.,0.,-1.,0.))) < MINATD)
		return XY;
	if(fabs(N.Angle(BPoint(0.,-1.,0.,0.))) < MINATD)
		return XZ;
	if(fabs(N.Angle(BPoint(-1.,0.,0.,0.))) < MINATD)
		return YZ;
	return P_UNDEF;
}

void NCadrGeom::SetLineFont(int mask)
{
	glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT );
	glEnable(GL_LINE_STIPPLE);
	glLineStipple( 1/*factor*/, mask /* pattern */  );
	glColor4fv(FastMovCol.array);
}

void NCadrGeom::RestoreLineFont()
{
	glPopAttrib();
}
void NCadrGeom::Line(double xb, double yb, double zb, double xe, double ye, double ze, bool fast)
{
		if(fast)
			SetLineFont();
		LineSimple( xb, yb, zb, xe, ye, ze);
		if(fast)
			RestoreLineFont();
}

void NCadrGeom::FastLine(double xb, double yb, double zb, double xe, double ye, double ze)
{
	int UpDownInd = (zb > ze) ? 1 : 0;
	double xc = xb;
	double yc = yb;
	double zc = zb;
	for(int st = 0; st < BFastMoveMan::StepsNum ; ++st)
	{
		double xr = BFastMoveMan::CoordFlags[UpDownInd][st][0] ? xe : xb;
		double yr = BFastMoveMan::CoordFlags[UpDownInd][st][1] ? ye : yb;
		double zr = BFastMoveMan::CoordFlags[UpDownInd][st][2] ? ze : zb;
		if(xc != xr || yc != yr || zc != zr )
			Line(xc, yc, zc, xr, yr, zr, true);	
		xc = xr;
		yc = yr;
		zc = zr;
	}
}

void NCadrGeom::LineSimple(double xb, double yb, double zb, double xe, double ye, double ze)
{
	glBegin(GL_LINES);
		glVertex3d(xb,yb,zb);	
		glVertex3d(xe,ye,ze);
	glEnd();
}

bool NCadrGeom::Draw5x(double Eps, double t) const
{
	if(!IsRotate() && !IsCylindric())
		return false;
	if(!pCurve)
		return false;
	//5x
	pCurve->Draw(t);
	return true;

}

double NCadrGeom::TotalAngle(const NCadrGeom * GeomArr, int Num)
{
	double Angle = 0.;
	for(int t = 0; t < Num; ++t)
	{
		double s, e, c;
		GeomArr[t].GetArcA(&s, &e, &c);
		Angle += c + GeomArr[(t + 1) % Num].Angle(GeomArr[t]);
	}
	return Angle;
}

int NCadrGeom::IsConvexGeom(const NCadrGeom* GeomArr, int Num)
{
//	Returns +2 if concave and ccw, -2 if concave and cw, +1 if convex and ccw, -1 if convex and cw, 0 - error
	int Conv = 0;
	int Ret = 1;
	double Angle = 0.;
	for (int t = 0; t < Num; ++t)
	{
		double b, e, c;
		GeomArr[t].GetArcA(&b, &e, &c);
		Angle += c;
		int s = (fabs(c) < MINAD ? 0 : (c > 0 ? 1 : -1));
		if (Ret == 2)
			continue;
		if (Conv == 0 && s == 0)
			continue;
		if (Conv != 0)
		{
			if (Conv * s < 0)
				Ret = 2;
		}
		else
			Conv = s;
	}
	for (int t = 0; t < Num; ++t)
	{
		double a = GeomArr[(t + 1) % Num].Angle(GeomArr[t]);
		Angle += a;
		int s = (fabs(a) < MINAD ? 0 : (a > 0 ? 1 : -1));
		if (Ret == 2)
			continue;
		if (Conv == 0 && s == 0)
			continue;
		if (Conv != 0)
		{
			if (Conv * s < 0)
				Ret = 2;
		}
		else
			Conv = s;
	}
	return Ret;
}

void NCadrGeom::SetCurve(BCurve *pCurveI)
{
	pCurve = pCurveI;
}
BCurve * NCadrGeom::GetCurve(void) const
{
	return pCurve;
}

bool NCadrGeom::IsPolar(void) const
{
	return (Attr & POLAR_FLAG) != 0;
}

bool NCadrGeom::IsFast(void) const
{
	return type.flags & CU_FAST || GetAttr() == 5;
}

bool NCadrGeom::IsSpline(void) const
{
	return (type.flags & CU_SPLN) != 0;
}

bool NCadrGeom::IsMachConfCh(void) const
{
	return (type.flags & CU_CONF) != 0;
}

void NCadrGeom::SetPolar(void) 
{
	Attr |= POLAR_FLAG;
	MatrNum = 0;
	InterpNum = 0;
}
bool NCadrGeom::IsCylindric(void) const
{
	return (Attr & CYLINDRIC_FLAG) != 0;
}
void NCadrGeom::SetCylindric(void) 
{
	Attr |= CYLINDRIC_FLAG;
	MatrNum = 0;
	InterpNum = 0;
}

NCadrGeom::NCadrGeom(const NCadrGeom & In)
{
	*this = In;
}

const NCadrGeom & NCadrGeom::operator =(const NCadrGeom & In)
{
	memcpy(this, &In, sizeof(In));
	return *this;
}

const NCadrGeom & NCadrGeom::FullCopy(const NCadrGeom & In)
{
	*this = In;
	if((type.type == rotate || IsCylindric() || IsSpline()) && pCurve)
		pCurve = BCurve::NewBCurve(*pCurve);

	return *this;
}
bool NCadrGeom::GetStartABC(BPoint &ABC)
{// Returns true if A != 0 | B != 0 | C != 0
	if(IsPolar())
	{
		double C = (GetB() - BPoint(0.,0.,0.,1.)).ProjXY().Angle(BPoint(1., 0., 0., 0.));
		ABC.Set(0., 0., C, 1.);
		return true;
	}
	if(type.type == rotate)
	{
		ABC = GetI();
		return true;
	}
	if(MatrNum == 0)
		return false;
	ABC = BSpacePos::GetABC(MatrNum);
	return true;
}

bool NCadrGeom::GetEndABC(BPoint &ABC)
{// Returns true if A != 0 | B != 0 | C != 0
	if(IsPolar())
	{
		double C = (GetE() - BPoint(0.,0.,0.,1.)).ProjXY().Angle(BPoint(1., 0., 0., 0.));
		ABC.Set(0., 0., C, 1.);
		return true;
	}
	if(type.type == rotate)
	{
		ABC = GetN();
		return true;
	}
	if(MatrNum == 0)
		return false;
	ABC = BSpacePos::GetABC(MatrNum);
	return true;
}

void NCadrGeom::GetDeltaABC(BPoint &ABC)
{
	BPoint SABC, EABC;
	if(GetStartABC(SABC) && GetEndABC(EABC))
		ABC = EABC - SABC;
	else
		ABC.Set(0., 0., 0., 0.);
}

void NCadrGeom::CalcCurCoord(double t, BPoint &XYZ, BPoint &ABC) const
{
	if(type.type == undef)
		return;
	BPoint CurPoint(0., 0., 0., 1.);
	if(type.type == cwarc || type.type == ccwarc)
	{
		BMatr Rot, m;
		double sa, ea, ca;
		GetArcA(&sa, &ea, &ca);
		Rot.rotg(ca * t, GetC(),GetN());
		BPoint Tr(GetE() - GetB());
		double d = Tr * GetN();
		Rot = Rot * m.Trans(BPoint(0.,0.,0.,1.), BPoint(0.,0.,0.,1.) + GetN() * ( d * t)); 
		CurPoint = GetB() * Rot;
	}
	else if(IsSpline() && pCurve)
	{
		NLine Line;
		pCurve->GetLine(0, Line);
		CurPoint = Line.GetPointFromParam(t).Norm();
	}
	else
		CurPoint = GetB() * (1 - t) + GetE() * t;

	if(IsPolar())
	{
		ABC = BSpacePos::GetABC(MatrNum);
		BSpacePos::GetPolar(InterpNum).MapP2PA(CurPoint, XYZ, ABC);
		BSpacePos::GetPolar(InterpNum).AddAlfa0(ABC);
//		XYZ = CurPoint;//XYZ * BMatr().RotZ(0., 0., 0., ABC.GetZ());
	}
	else if(IsCylindric())
	{
		BSpacePos::GetCyl(InterpNum).MapP2PA(CurPoint, XYZ, ABC);
	}
	else
	{
		XYZ = CurPoint;
		if(type.type == rotate)
			ABC = GetI() * (1 - t) + GetN() * t;
		else
			ABC = BSpacePos::GetABC(MatrNum);// We can use BSpacePos::GetABC because cadr may not be undef
	}
}

void NCadrGeom::CalcCurCoordMCS(double t, const BMatr & MCS, BPoint & XYZ, BPoint & ABC) const
{
	// MCS - corresponds to coord. system rotation and ignores machine tool rotation
	if (type.type == undef)
		return;
	BPoint CurPoint(0., 0., 0., 1.);
	if (type.type == cwarc || type.type == ccwarc)
	{
		BMatr Rot, m;
		double sa, ea, ca;
		GetArcA(&sa, &ea, &ca);
		Rot.rotg(ca * t, GetC(), GetN());
		BPoint Tr(GetE() - GetB());
		double d = Tr * GetN();
		Rot = Rot * m.Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + GetN() * (d * t));
		CurPoint = GetB() * Rot;
	}
	else if (IsSpline() && pCurve)
	{
		NLine Line;
		pCurve->GetLine(0, Line);
		CurPoint = Line.GetPointFromParam(t).Norm();
	}
	else
		CurPoint = GetB() * (1 - t) + GetE() * t;

	CurPoint = CurPoint * MCS;

	if (IsPolar())
	{
		BSpacePos::GetPolar(InterpNum).MapP2PA(CurPoint, XYZ, ABC);
		ABC += BSpacePos::GetABC(MatrNum);
		BSpacePos::GetPolar(InterpNum).AddAlfa0(ABC);
	}
	else if (IsCylindric())
	{
		BSpacePos::GetCyl(InterpNum).MapP2PA(CurPoint, XYZ, ABC);
	}
	else
	{
		XYZ = CurPoint;
		if (type.type == rotate)
			ABC = GetI() * (1 - t) + GetN() * t;
		else
			ABC = BSpacePos::GetABC(MatrNum);// We can use BSpacePos::GetABC because cadr may not be undef

	}
}

bool NCadrGeom::CalcCurCoordMS(const BMatr & CorrMatr, double t, SMachStateFix1 & P) const
{
	if(IsUndef())
		return false;
	if(P.GetSize() != 7)
		return false;

	BPoint XYZ(0., 0., 0., 1.), ABC(0., 0., 0., 1.);
	CalcCurCoord(t, XYZ, ABC);
	XYZ = XYZ * CorrMatr;
	P.SetCoord(0, XYZ.GetX());
	P.SetCoord(1, XYZ.GetY());
	P.SetCoord(2, XYZ.GetZ());
	P.SetCoord(3, ABC.GetX());
	P.SetCoord(4, ABC.GetY());
	P.SetCoord(5, ABC.GetZ());
	P.SetCoord(6, 1.);
	return true;
}

bool NCadrGeom::CalcCurTang(const BMatr & CorrMatr, double t, SMachStateFix1 & P)
{
	if(IsUndef())
		return false;
	if(P.GetSize() != 7)
		return false;
	if(IsArc())
	{
		bool WasCylindric = IsCylindric();
		if(WasCylindric)
		{
			Attr &= !CYLINDRIC_FLAG;
		}
		BPoint XYZ(0., 0., 0., 1.), A(0., 0., 0., 1.);
		CalcCurCoord(t, XYZ, A);
		XYZ = XYZ * CorrMatr;
		BPoint C = GetC();
		double Angle = IsCCWArc() ? 90. : -90.;
		BPoint T = (XYZ - C) * BMatr().rotg(Angle, BPoint(0., 0., 0., 1.), GetN());
		if(WasCylindric)
		{
			Attr |= CYLINDRIC_FLAG;
			BSpacePos::GetCyl(InterpNum).MapVv2PA(T, XYZ, A);		
			P.SetCoord(0, XYZ.GetX());
			P.SetCoord(1, XYZ.GetY());
			P.SetCoord(2, XYZ.GetZ());
			P.SetCoord(3, A.GetX());
			P.SetCoord(4, A.GetY());
			P.SetCoord(5, A.GetZ());
			P.SetCoord(6, 0.);
		}
		else
		{
			P.SetCoord(0, T.GetX());
			P.SetCoord(1, T.GetY());
			P.SetCoord(2, T.GetZ());
			P.SetCoord(3, 0.);
			P.SetCoord(4, 0.);
			P.SetCoord(5, 0.);
			P.SetCoord(6, 0.);
		}
		return true;
	}
	BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.);
	CalcCurCoord(0., XYZ0, ABC0);
	XYZ0 = XYZ0 * CorrMatr;
	BPoint XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
	CalcCurCoord(1., XYZ1, ABC1);
	XYZ1 = XYZ1 * CorrMatr;
	BPoint XYZ = XYZ1 - XYZ0, ABC = ABC1 - ABC0;
	P.SetCoord(0, XYZ.GetX());
	P.SetCoord(1, XYZ.GetY());
	P.SetCoord(2, XYZ.GetZ());
	P.SetCoord(3, ABC.GetX());
	P.SetCoord(4, ABC.GetY());
	P.SetCoord(5, ABC.GetZ());
	P.SetCoord(6, 0.);
	return true;
}

int NCadrGeom::GetNumAppr(double Eps) const
{
	if(IsUndef())
		return 0;
	if(IsLine() && !IsCylindric())
		return 1;
	if(IsArc())
	{
		double sa, ea, ca;
		GetArcA(&sa, &ea, &ca);
		double alf = fabs(ca) * PI / 180.;
		double b = 2. * acos(1. - Eps / sqrt(GetI().D2()));
		if( b >= alf )
			return 1;
		return (int(alf * 0.5 / b) + 1) * 2;
	}
	if(IsRotate() || (IsCylindric() && IsLine()) || IsSpline())
	{
		if(pCurve) 
			return pCurve->GetNumAppr(Eps);
		else
			return 1;
	}
	return 0;
}
void NCadrGeom::SetFast(bool fast)
{
	if(fast)
		type.flags |= _int16(CU_FAST);
	else
		type.flags &= ~_int16(CU_FAST); 
}

void NCadrGeom::SetSpline(bool spline)
{
	if(spline)
		type.flags |= _int16(CU_SPLN);
	else
		type.flags &= ~_int16(CU_SPLN); 
}

void NCadrGeom::SetGrooveFlag(bool IsGroove)
{
	if(IsGroove)
		type.flags |= _int16(CU_GROV);
	else
		type.flags &= ~_int16(CU_GROV); 
}

bool NCadrGeom::IsFull5x(void) const
{
	return Is5x();
}

bool NCadrGeom::Is5x(void) const
{
	return IsCylindric() || IsRotate();
}

void NCadrGeom::SetPl(enum Plane Pl)
{
	pl = Pl;
	switch(pl)
	{
	case XY:
		xn = 0.; yn = 0.; zn = 1.;
		break;
	case XZ:
		xn = 0.; yn = 1.; zn = 0.;
		break;
	case YZ:
		xn = 1.; yn = 0.; zn = 0.;
		break;
	default:
		xn = 0.; yn = 0.; zn = 1.;
		break;
	}
	
}

double NCadrGeom::GetVolumeRot(void) const
{
	// Кадр должен быть отрезком
	// Ось вращения - Z
	if(!HaveGeom())
		return 0.;

	BPoint A0(0., 0., 0., 1.);
	BPoint A1(0., 0., 1., 1.);
	double RB2 = GetB().LineD2(A0, A1);
	double RE2 = GetE().LineD2(A0, A1);

	return (1./3.) * PI * (zb - ze) * (RB2 + RE2 + sqrt(RB2 * RE2));
}

void NCadrGeom::ApplyMatr(void)
{
	if (!HaveGeom5())
		return;
	if(GetMatrNum() != 0)
	{
		Tr(BSpacePos::GetRotMatr(GetMatrNum()));
		SetMatrNum(0);
	}
}


int NCadrGeom::GetPolyLine(BPointsBuf &Pts, double Eps, bool UseMatr) const
{
	if(!HaveGeom5())
		return 0;

	if(Is5x() || (IsSpline() && pCurve))
	{
		if(!pCurve)
			return 0;
		NLine Line;
		
		Pts.SetSize(0);
		pCurve->MakeAppr(Eps, Pts);
	}
	else if(IsLine())
	{
		Pts.AddPoint((GetMatrNum() == 0 || !UseMatr) ? GetB() : GetB() * BSpacePos::GetRotMatr(GetMatrNum()));
		//if(IsFast() && BFastMoveMan::PartInterp())
		//{
		//	int UpDownInd = (zb > ze) ? 1 : 0;
		//	double xc = xb;
		//	double yc = yb;
		//	double zc = zb;
		//	for(int st = 0; st < BFastMoveMan::StepsNum ; ++st)
		//	{
		//		double xr = BFastMoveMan::CoordFlags[UpDownInd][st][0] ? xe : xb;
		//		double yr = BFastMoveMan::CoordFlags[UpDownInd][st][1] ? ye : yb;
		//		double zr = BFastMoveMan::CoordFlags[UpDownInd][st][2] ? ze : zb;
		//		if(xc != xr || yc != yr || zc != zr )
		//		{
		//			BPoint IntP(xr, yr, zr, 1.);
		//			Pts.AddPoint((GetMatrNum() == 0) ? IntP : IntP * BSpacePos::GetRotMatr(GetMatrNum()));
		//		}
		//		xc = xr;
		//		yc = yr;
		//		zc = zr;
		//	}
		//}
		//else
		{
			Pts.AddPoint((GetMatrNum() == 0 || !UseMatr) ? GetE() : GetE() * BSpacePos::GetRotMatr(GetMatrNum()));
		}
	}
	else if(IsArc())
	{

		bool IgnoreMatr = (GetMatrNum() == 0 || !UseMatr);
		const BMatr *pMatr = NULL;
		if(!IgnoreMatr)
			pMatr = &BSpacePos::GetRotMatr(GetMatrNum());
		BPoint TrVec = GetN() * ((GetE() - GetB()) * GetN());
		double s, e, CenterAngle;
		if(IsNormalStandard())
		{
			GetArcA(&s, &e, &CenterAngle);
		}
		else
		{
			CenterAngle = (GetI() * (-1.)).Angle((GetE() - GetC()), GetN());
			if(GetType().type == ccwarc && CenterAngle < 0.)
				CenterAngle = 360. + CenterAngle;
			else if(GetType().type == cwarc && CenterAngle > 0.)
				CenterAngle = CenterAngle - 360.;
			if(fabs(CenterAngle) < MINAR)
				CenterAngle = 360.;
		}

		int MaxI = GetNumAppr(Eps);
		for(int i = 0; i <= MaxI; ++i)
		{
			double t = double(i) / MaxI;
			BMatr Rot;
			Rot.rotg(t * CenterAngle, GetC(), GetN());
			BMatr Tran;
			Tran.Trans(BPoint(0., 0., 0., 1.), TrVec * t);
			BPoint XYZ((GetB() * Rot) * Tran);
			if(pMatr)
				XYZ = XYZ * (*pMatr);
			Pts.AddPoint(XYZ);
		}

	}
	return Pts.GetSize();
}

int NCadrGeom::GetPolyLineSize(double Eps)
{
	if(IsUndef())
		return 0;

	if(Is5x() || (IsSpline() && pCurve))
	{
		if(!pCurve)
			return 0;
		int Ret = 0;
		NLine Line;
		for(int IStart = 0, k = 0; pCurve->GetLine(k, Line); IStart = 1, ++k)
		{
			int MaxI = Line.GetNumAproxLine(Eps);
			Ret += MaxI + 1 - IStart;
		}
		return Ret;
	}
	else if(IsLine())
	{
		//if(IsFast() && BFastMoveMan::PartInterp())
		//{
		//	int Ret = 2;
		//	int UpDownInd = (zb > ze) ? 1 : 0;
		//	double xc = xb;
		//	double yc = yb;
		//	double zc = zb;
		//	for(int st = 0; st < BFastMoveMan::StepsNum ; ++st)
		//	{
		//		double xr = BFastMoveMan::CoordFlags[UpDownInd][st][0] ? xe : xb;
		//		double yr = BFastMoveMan::CoordFlags[UpDownInd][st][1] ? ye : yb;
		//		double zr = BFastMoveMan::CoordFlags[UpDownInd][st][2] ? ze : zb;
		//		if(xc != xr || yc != yr || zc != zr )
		//		{
		//			++Ret;
		//		}
		//		xc = xr;
		//		yc = yr;
		//		zc = zr;
		//	}
		//	return Ret;
		//}
		//else
		{
			return 2;
		}
	}
	else if(IsArc())
	{
		int MaxI = GetNumAppr(Eps);
		return MaxI + 1;
	}
	return 0;
}

bool NCadrGeom::IsGroove(void) const
{
	return (type.flags & CU_GROV) != 0;
}

int NCadrGeom::CalcQuadsNum() const
{
	const double EPS = 1.e-5;

	if (!IsArc())
		return 0;
	double As, Ae, Ac;
	GetArcA(&As, &Ae, &Ac);
	if (type.type == cwarc)
	{
		double b = As; As = Ae; Ae = b;
	}
	if (Ae < As)
		Ae += 360.;
	double Angles[5];
	Angles[0] = (type.type == cwarc) ? Ae : As;
	int Num = 1;
	for (double At = 0.; At < 720. - EPS; At += 90.)
	{
		if (At - EPS > As && At + EPS < Ae)
			Angles[Num++] = At;
	}
	return Num;
}

int NCadrGeom::DivideQuad(NCadrGeom *& pGeomArr) const
{
	const double EPS = 1.e-5;

	if(!IsArc())
		return 0;
	double As, Ae, Ac;
	GetArcA(&As, &Ae, &Ac);
	if(type.type == cwarc)
	{
		double b = As; As = Ae; Ae = b;
	}
	if(Ae < As)
		Ae += 360.;
	double Angles[5];
	Angles[0] = (type.type == cwarc) ? Ae : As;
	int Num = 1;
	for(double At = 0.; At < 720. - EPS; At +=90.)
	{
		if(At - EPS > As && At + EPS < Ae)
			Angles[Num++] = At;
	}
	if(Num < 2)
		return 0;

	pGeomArr = new NCadrGeom[Num];
	pGeomArr[0] = *this;
	if(type.type == cwarc)
	{// Reverse array from 1 index
		for(int i = 1; i < 1 + (Num - 1) / 2; ++i)
		{
			double b = Angles[i]; Angles[i] = Angles[Num - i]; Angles[Num - i] = b;
		}
	}
	for (int i = 1; i < Num; ++i)
	{
		Divide(pGeomArr[i - 1], Angles[i] - Angles[i - 1], pGeomArr[i]);
		if (pGeomArr[i - 1].IsFullArc(FARC))
		{// Remove degraded arc
			pGeomArr[i - 1] = pGeomArr[i];
			Angles[i - 1] = Angles[i];
			--i;
			--Num;
		}
	}
	if (pGeomArr[Num - 1].IsFullArc(FARC))
	{// Remove degraded arc
		--Num;
	}

	return Num;
}

int NCadrGeom::Divide(NCadrGeom & Source, double Angle, NCadrGeom & Dest)
{// For arcs only
	Dest = Source;
	BPoint P = (Source.GetB() * BMatr().rotg(Angle, Source.GetC(), Source.GetN())) ;
	Source.SetE(P);
	Dest.SetB(P);
	Dest.SetI(Dest.GetC() - P);
	double SLen = Source.Length();
	DivideWorkTime(Source, SLen / (SLen + Dest.Length()), Dest);
	return 0;
}

int NCadrGeom::DivideX(NCadrGeom & Source, double XValue, NCadrGeom & Dest)
{// Divide cadr by XValue given
	// If arc then XZ plane only
	Dest = Source;
	BPoint P;
	if (Source.IsArc())
	{// Arc should lie in a single quadrant
		double sign = ((Source.xe > Source.xb) == (Source.type.type == ccwarc)) ?
			-1. : 1.;
		double R2 = Source.GetI().D2();
		BPoint C = Source.GetC();
		double dX = XValue - C.GetX();
		double Zp = C.GetZ() - sign * sqrt(fabs(R2 - dX * dX));
		P.Set(XValue, 0., Zp, 1.);
	}
	else
	{
		double t = (XValue - Source.xb) / (Source.xe - Source.xb);
		P = Source.GetB() * (1. - t) + Source.GetE() * t;
	}
	Source.SetE(P);
	Dest.SetB(P);
	double SLen = Source.Length();
	DivideWorkTime(Source, SLen / (SLen + Dest.Length()), Dest);
	return 0;
}

void NCadrGeom::DivideWorkTime(NCadrGeom& Source, double TValue, NCadrGeom& Dest)
{// Source and Dest times mast be equal
	Source.Times.WorkEnd = static_cast<MainTime>(Source.Times.WorkStart * (1. - TValue) + Source.Times.WorkEnd * TValue);
	Source.Times.End = Source.Times.WorkEnd;
	Dest.Times.WorkStart = Source.Times.WorkEnd;
	Dest.Times.Start = Dest.Times.WorkStart;
}

int NCadrGeom::DivideT(NCadrGeom& Source, double TValue, NCadrGeom& Dest)
{// Divide cadr by TValue given
	Dest = Source;
	DivideWorkTime(Source, TValue, Dest);
	if (Source.IsLine())
	{
		BPoint P = Source.GetB() * (1. - TValue) + Source.GetE() * TValue;
		Source.SetE(P);
		if (Source.GetAttr() == 7)// Thread
			Source.SetAttr(6);// Don't cut
		Dest.SetB(P);
	}
	else if (Source.IsArc())
	{
		unsigned char AttrBuf = Source.Attr; // Save, clear and restore needed to CalcCurCoord works properly for polar or cylindric cadrs
		auto InterpNumBuf = Source.GetInterpNum();
		if (Source.IsPolar() || Source.IsCylindric())
		{
			Source.Attr &= ~(POLAR_FLAG | CYLINDRIC_FLAG);
			Source.InterpNum = 0;
		}
		BPoint P(0., 0., 0., 1.), ABC(0., 0., 0., 1.);
		Source.CalcCurCoord(TValue, P, ABC);
		BPoint I = Source.GetC() - P;
		Source.Attr = AttrBuf;
		if (Source.IsPolar() || Source.IsCylindric())
			Source.InterpNum = InterpNumBuf;
		else if (Source.InterpNum != 0)
		{// InterpNum here is a number of the full circles
			double sa, ea, ca;
			Source.GetArcA(&sa, &ea, &ca);
			ca *= Source.type.type == cwarc ? -1. : 1.;
			Source.InterpNum = int(ca * TValue / 360);
			Dest.InterpNum = int(ca * (1. - TValue) / 360);
		}
		Source.SetE(P);
		Dest.SetB(P);
		switch (Source.GetPlane())
		{
		case XY:
			I.SetZ(0.);
			break;
		case XZ:
			I.SetY(0.);
			break;
		case YZ:
			I.SetX(0.);
			break;
		}
		Dest.SetI(I);
	}
	else if (Source.IsRotate())
	{
		
		BCurve *pCurve = Source.GetCurve();
		if (pCurve == nullptr)
			return -1;
		double FullLength = pCurve->GetLength();
		double PartLength = 0;
		// Find segment to divide
		NLine Line;
		double CurLength = 0.;
		int i = 0;
		for (; i < pCurve->GetSize() / 3; ++i)
		{
			pCurve->GetLine(i, Line);
			CurLength = Line.GetLength();
			if (PartLength + CurLength >= TValue * FullLength)
				break;
			PartLength += CurLength;
		}
		// Divide segment
		double LocalT = (TValue * FullLength - PartLength) / CurLength;
		NLine L1, L2;
		Line.Divide2(LocalT, L1, L2);
		// Make parts
		BCurve *pC1 = BCurve::NewBCurve(i + 1);
		for (int k = 0; k < i; ++k)
		{
			pCurve->GetLine(k, Line);
			for (int l = 0; l < 3; ++l)
				pC1->SetPoint(k * 3 + l, Line.p[l]);
		}
		for (int l = 0; l < 4; ++l)
			pC1->SetPoint(i * 3 + l, L1.p[l]);
		BCurve *pC2 = BCurve::NewBCurve(pCurve->GetSize() / 3 - i);
		for (int l = 0; l < 4; ++l)
			pC2->SetPoint(l, L2.p[l]);
		for (int k = i + 1; k < pCurve->GetSize() / 3; ++k)
		{
			pCurve->GetLine(k, Line);
			for (int l = 1; l < 4; ++l)
				pC2->SetPoint((k - i) * 3 + l, Line.p[l]);
		}
		Source.SetCurve(pC1);
		Dest.SetCurve(pC2);
		BPoint P = Source.GetB() * (1. - TValue) + Source.GetE() * TValue;
		Source.SetE(P);
		Dest.SetB(P);
		P = Source.GetI() * (1. - TValue) + Source.GetN() * TValue;
		Source.SetN(P);
		Dest.SetI(P);// RCP TODO
	}
	return 0;
}

double NCadrGeom::DivideTByTime(NCadrGeom& Source, double TValue, NCadrGeom& Dest)
{
	const MainTime WholeTime = Source.GetWholeTime();
	if (WholeTime == 0)
		return 0.;
	const MainTime PartTime = static_cast<MainTime>(TValue * WholeTime);
	if (PartTime <= Source.Times.WorkStart - Source.Times.Start)
	{
		Dest = Source;
		Source.SetType(undef);
		Source.SetE(Source.GetB());
		Source.Times.WorkStart = Source.Times.WorkEnd = Source.Times.End = Source.Times.Start + PartTime;
		Dest.Times.Start = Source.Times.End;
	}
	else if (PartTime >= Source.Times.WorkEnd - Source.Times.Start)
	{
		Dest = Source;
		Dest.SetType(undef);
		Dest.SetE(Dest.GetB());
		Dest.Times.WorkStart = Dest.Times.WorkEnd = Dest.Times.Start = Source.Times.Start + PartTime;
		Source.Times.End = Dest.Times.Start;
	}
	else
	{
		double t = TValue - (Source.Times.WorkStart - Source.Times.Start) / double(Source.GetWorkTime());
		DivideT(Source, t, Dest);//Source.GetWorkTime() is not zero
	}
	return 0;
}

bool NCadrGeom::IsNormalStandard(void) const
{
	BPoint PlN = NormByPlane(GetPlane());
	return PlN.Angle0_180(GetN()) < MINAR;
}

void NCadrGeom::Serialize(CArchive &ar)
{
	SerializeElements(ar, &xb, 1);
	SerializeElements(ar, &yb, 1);
	SerializeElements(ar, &zb, 1);
	SerializeElements(ar, &xe, 1);
	SerializeElements(ar, &ye, 1);
	SerializeElements(ar, &ze, 1);
	SerializeElements(ar, &i, 1);
	SerializeElements(ar, &j, 1);
	SerializeElements(ar, &k, 1);
	SerializeElements(ar, &xn, 1);
	SerializeElements(ar, &yn, 1);
	SerializeElements(ar, &zn, 1);
	SerializeElements(ar, &pl, 1);
	SerializeElements(ar, &type, 1);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.1")))
		SerializeElements(ar, &Times, 1);
	else
	{
		float Time;
		SerializeElements(ar, &Time, 1);
		SetWorkTime(MainTime(Time));
	}
	SerializeElements(ar, &MatrNum, 1);
	SerializeElements(ar, &InterpNum, 1);
	SerializeElements(ar, &MSHistNum, 1);
	SerializeElements(ar, &Attr, 1);
	if(ar.IsLoading())
		pCurve = NULL;
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("2025")))
		SerializeElements(ar, &HistID, 1);
}

bool NCadrGeom::LiesOnSameCurve(const NCadrGeom &In)
{
	double Tol = NSurfSettings::GetTolerance() * 2.;
	// Works for lines and arcs only
	if (!IsGeomValid(Tol))
		SetType(line);
	if (GetType().type != In.GetType().type)
		return false;
	if (IsLine())
	{
		if (GetStartDir().Angle0_180(In.GetStartDir()) > 5.)
			return false;
		return (GetE().LineD2(GetB(), In.GetE()) < Tol * Tol);
	}
	if (IsArc())
	{
		if (fabs(GetR() - In.GetR()) > Tol)
			return false;
		return ((GetC() - In.GetC()).D2() < Tol * Tol);
	}
	return false;
}

bool NCadrGeom::IsPlaneCorrect(void) const
{
	return pl == PlaneByNorm(GetN());
}



BPoint NCadrGeom::RayCast(const BPoint& P, const BPoint& V, double Epsilon) const
{
	if (IsLine())
		return LineRayCast(P, V, Epsilon);
	if (IsArc())
		return ArcRayCast(P, V, Epsilon);
	if (Is5x())
		return CurveRayCast(P, V, Epsilon);
	
	return BPoint(0, 0, 0, -1);// any other type
}

BMTLinkage NCadrGeom::GetMTLink() const
{
	const auto& TimeLine = NCM_PROJECT.GetTimeLine();
	return TimeLine.GetLinkage(*this, 0.);
}

bool NCadrGeom::RepairArc()
{
	if (!IsArc() || pl != XY)
		return true;
	return (0 == CreateRArcXY(xb, yb, zb, xe, ye, ze, GetR(), type.type, ARCEPS));
}

int NCadrGeom::CreateRArcXY(double x1, double y1, double z1, double x2, double y2, double z2, double R, curve CurveType, double MinArcDist)
{ 
	double dist2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);
	if (dist2 == 0.)
		return 0;
	double dist = sqrt(dist2);
	double s2 = fabs(R) - 0.5 * dist;
	if (fabs(s2) < MinArcDist)
	{
		Set(CurveType,
			x1, y1, z1, x2, y2, z2,
			0.5 * (x2 - x1), 0.5 * (y2 - y1), 0.5 * (z2 - z1), XY);
		return 0;
	}
	if (s2 < 0.)
		return -1;
	double s = sqrt(fabs(R * R - 0.25 * dist2));
	double	x = x2 - x1,
		y = y2 - y1,
		z = z2 - z1;// P2 - P1 vector

	x = y1 - y2;
	y = x2 - x1;

	double d = s / dist;
	if (CurveType == cwarc)
		d = -d;
	if (R < 0.)
		d = -d;
	x *= d;
	y *= d;
	z *= d;
	double	xc = (x1 + x2) * 0.5 + x,
		yc = (y1 + y2) * 0.5 + y,
		zc = (z1 + z2) * 0.5 + z;
	Set(CurveType,
		x1, y1, z1, x2, y2, z2,
		xc - x1, yc - y1, zc - z1,
		XY);

	return 0;
}

double Clamp(double n, double min, double max) {
	//Clamp value n on min or max
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

bool NCadrGeom::ApplyLCompSimple(const BPoint& ShiftVect)
{
	if (!(IsLine() || IsArc()))
		return false;
	BPoint LocShiftVect = ShiftVect;
	if (GetMatrNum() != 0)
	{
		//const BMatr& LocMatr = BSpacePos::GetRotMatr(GetMatrNum());
		//LocShiftVect = ShiftVect * LocMatr.invr();
	}
	if (IsLCompBeg() && IsLCompEnd())
	{
		if (fabs(LocShiftVect.GetX()) < MIND && fabs(LocShiftVect.GetY()) < MIND)
			ShiftZ(GetLengthComp() * (LocShiftVect.GetZ() < 0. ? -1. : 1));
		else
			Tr(BMatr().Trans(LocShiftVect * GetLengthComp()));
	}
	else if (IsArc())
	{
		return false;
	}
	else if (IsLCompEnd())
	{// turn on
		SetE(GetE() * BMatr().Trans(LocShiftVect * GetLengthComp()));
	}
	else if (IsLCompBeg())
	{// turn off
		SetB(GetB() * BMatr().Trans(LocShiftVect * GetLengthComp()));
	}
	return true;
}

void NCadrGeom::GetCadrArray4Helix(const NCadrGeom* pGeom, BGeomArray& Cadrs)
{
	BPoint TrVec = pGeom->GetN() * ((pGeom->GetE() - pGeom->GetB()) * pGeom->GetN());
	double s, e, CenterAngle;
	if (pGeom->IsNormalStandard())
	{
		pGeom->GetArcA(&s, &e, &CenterAngle);
	}
	else
	{
		CenterAngle = (pGeom->GetI() * (-1.)).Angle((pGeom->GetE() - pGeom->GetC()), pGeom->GetN());
		if (pGeom->GetType().type == ccwarc && CenterAngle < 0.)
			CenterAngle = 360. + CenterAngle;
		else if (pGeom->GetType().type == cwarc && CenterAngle > 0.)
			CenterAngle = CenterAngle - 360.;
		if (fabs(CenterAngle) < MINAR)
			CenterAngle = 360.;
	}
	Cadrs.RemoveAll();
	CenterAngle = fabs(CenterAngle);
	double t = 360. / CenterAngle;// per revolution
	BPoint Ep = pGeom->GetB();
	for (int i = 0; i < pGeom->InterpNum; ++i)
	{
		BPoint Sp = pGeom->GetB() + TrVec * ((i)*t);
		Ep = pGeom->GetB() + TrVec * ((i + 1) * t);
		NCadrGeom* pNewGeom = new NCadrGeom;
		*pNewGeom = *pGeom;
		pNewGeom->SetType(line);
		pNewGeom->SetB(Sp);
		pNewGeom->SetE(Ep);
		Cadrs.Add(pNewGeom);
		NCadrGeom* pNewArc = new NCadrGeom;
		pNewArc->Set(pGeom->GetType(), Ep.GetX(), Ep.GetY(), Ep.GetZ(), Ep.GetX(), Ep.GetY(), Ep.GetZ()
			, pGeom->GetI().GetX(), pGeom->GetI().GetY(), pGeom->GetI().GetZ(), pGeom->GetPlane());
		Cadrs.Add(pNewArc);
	}
	NCadrGeom* pNewGeom = new NCadrGeom;
	*pNewGeom = *pGeom;
	pNewGeom->SetType(line);
	pNewGeom->SetB(Ep);
	Ep.SetZ(pGeom->GetE().GetZ());
	pNewGeom->SetE(Ep);
	Cadrs.Add(pNewGeom);
	NCadrGeom* pNewArc = new NCadrGeom;
	pNewArc->Set(pGeom->GetType(), Ep.GetX(), Ep.GetY(), Ep.GetZ(), Ep.GetX(), Ep.GetY(), Ep.GetZ()
		, pGeom->GetI().GetX(), pGeom->GetI().GetY(), pGeom->GetI().GetZ(), pGeom->GetPlane());
	Cadrs.Add(pNewArc);
	Cadrs.Add(new NCadrGeom);
}

void NCadrGeom::Divide3DArc(BGeomArray& Array) const
{
	BPoint TrVec = GetN() * ((GetE() - GetB()) * GetN());
	double s, e, CenterAngle;
	if (IsNormalStandard())
	{
		GetArcA(&s, &e, &CenterAngle);
	}
	else
	{
		CenterAngle = (GetI() * (-1.)).Angle((GetE() - GetC()), GetN());
		if (GetType().type == ccwarc && CenterAngle < 0.)
			CenterAngle = 360. + CenterAngle;
		else if (GetType().type == cwarc && CenterAngle > 0.)
			CenterAngle = CenterAngle - 360.;
		if (fabs(CenterAngle) < MINAR)
			CenterAngle = 360.;
	}
	Array.RemoveAll();
	CenterAngle = fabs(CenterAngle);
	double t = 360. / CenterAngle;// per revolution
	int PartsN = CenterAngle / (90. - MINAD) + 1;
	double StepA = CenterAngle / PartsN;
	NCadrGeom Source = *this;
	NCadrGeom Dest;
	for (int i = 0; i < PartsN; ++i)
	{
		DivideT(Source, 1. / (PartsN - i), Dest);
		Array.AddElement(Source);
		Source = Dest;
	}
	//BPoint Ep = GetB();
	//for (int i = 0; i < InterpNum; ++i)
	//{
	//	BPoint Sp = GetB() + TrVec * ((i)*t);
	//	Ep = GetB() + TrVec * ((i + 1) * t);
	//	NCadrGeom* pNewArc = new NCadrGeom;
	//	pNewArc->Set(GetType(), Sp.GetX(), Sp.GetY(), Sp.GetZ(), Ep.GetX(), Ep.GetY(), Ep.GetZ()
	//		, GetI().GetX(), GetI().GetY(), GetI().GetZ(), GetPlane());
	//	Array.Add(pNewArc);
	//}
	//BPoint Sp = Ep;
	//Ep.SetZ(GetE().GetZ());
	//NCadrGeom* pNewArc = new NCadrGeom;
	//pNewArc->Set(GetType(), Sp.GetX(), Sp.GetY(), Sp.GetZ(), Ep.GetX(), Ep.GetY(), Ep.GetZ()
	//	, GetI().GetX(), GetI().GetY(), GetI().GetZ(), GetPlane());
	//Array.Add(pNewArc);
	//Array.Add(new NCadrGeom);
}

BPoint NCadrGeom::LineRayCast(const BPoint& P, const BPoint& V, double Epsilon) const
{
	return BPoint::LineRayCast(GetB(), GetE(), P, V, Epsilon);
}

BPoint NCadrGeom::CurveRayCast(const BPoint& P, const BPoint& V, double Epsilon) const
{
	if (pCurve == nullptr)
		return BPoint(0., 0., 0., -1.);
	BPoint Pres;
	if (pCurve->RayCast(P, V, Epsilon, Pres))
		return Pres;
	return BPoint(0., 0., 0., -1);
}

BPoint NCadrGeom::ArcRayCast(const BPoint& P, const BPoint& V, double Epsilon) const
{
	double radius = GetR();
	BPoint centerPoint = GetC();
	if (radius < MIND)
		if (centerPoint.LineD2(P, P + V) < Epsilon * Epsilon)
			return centerPoint;
	BPoint normal = GetN();
	BPoint intersectionPoint(0, 0, 0, -1);
	double z = normal * V;
	if (fabs(z) < MINAR)
	{
		double b = (normal * (P - GetB())) / normal.D2();
		if (fabs(b) < Epsilon)
			return ArcRayCastOnPlane(P, V, Epsilon);
		return intersectionPoint;
	}
		
	z = 1.0 / z;
	double t = (centerPoint - P) * normal * z;
	if (t < 0)
		return intersectionPoint;
	else
		intersectionPoint = P + V * t;

	double dist = sqrt(BPoint::Dist2(intersectionPoint, centerPoint));
	if (dist > (radius - Epsilon) && dist < (radius + Epsilon))
		if (IsPointOnArc(intersectionPoint))
			return intersectionPoint;
	return BPoint(0, 0, 0, -1);
}

BPoint NCadrGeom::ArcRayCastOnPlane(const BPoint& P, const BPoint& V, double Epsilon) const
{
	BPoint normal = GetN();
	BMatr rotateMatr;
	rotateMatr.e0cong(normal, BPoint(0, 0, 1, 0));

	BPoint newR = rotateMatr * V;
	BPoint newP = rotateMatr * P;
	BPoint newCenter = rotateMatr * GetC();
	double radius = GetR();

	double A = newR[0] * newR[0] + newR[1] * newR[1];
	double B = 2 * (newR[0] * (newP[0] - newCenter[0]) + newR[1] * (newP[1] - newCenter[1]));
	double C = (newP[0] - newCenter[0]) * (newP[0] - newCenter[0]) + (newP[1] - newCenter[1]) 
		* (newP[1] - newCenter[1]) 
		- (radius + Epsilon) * (radius + Epsilon);
	double det = B * B - 4 * A * C;

	BPoint intersectionPoint(0, 0, 0, -1);
	if (det < 0)
		return intersectionPoint;
	else if (fabs(det) < MINAR)
	{
		double t = -B / (2 * A);
		intersectionPoint = P + V * t;
		if (IsPointOnArc(intersectionPoint))
			return intersectionPoint;
	}
	else
	{
		double t0 = (-B + sqrt(det)) / (2 * A);
		double t1 = (-B - sqrt(det)) / (2 * A);
		double t[2];
		if (t0 < t1)
		{
			t[0] = t0;
			t[1] = t1;
		}
		else
		{
			t[0] = t1;
			t[1] = t0;
		}
		for (int i = 0; i < 2; i++)
		{
			if (t[i] < 0.0)
				continue;
			intersectionPoint = P + V * t[i];
			if (IsPointOnArc(intersectionPoint))
				return intersectionPoint;
		}
	}
	return BPoint(0, 0, 0, -1);
}

bool NCadrGeom::IsPointOnArc(const BPoint& point) const
{
	if (IsFullArc(MINAR))
		return true;
	BPoint centerPoint = GetC();
	double dummy1 = 0.0, dummy2 = 0.0, arcAngle = 0.0;
	this->GetArcA(&dummy1, &dummy2, &arcAngle);
	BPoint fromCenterToPointVector = point - centerPoint;

	double toStartRotation = (GetB() - centerPoint).Angle(fromCenterToPointVector, GetN());
	if (toStartRotation < 0.)
		toStartRotation += 360.;
	if (arcAngle > 0.)
		return arcAngle > toStartRotation;
	toStartRotation -= 360.;
	return arcAngle < toStartRotation;
}

NCadrGeom::CadrTimes::CadrTimes()
{
	Clear();
}

NCadrGeom::CadrTimes::~CadrTimes()
{
}

void NCadrGeom::CadrTimes::Clear()
{
	Start = MainT_UNDEF;
	WorkStart = 0;
	WorkEnd = 0;
	End = 0;
}

double NCadrGeom::RecalcT2Geom(double tTime) const
{
	if (tTime == -1.)
		return -1.;
	if (IsUndef())
		return 1.;
	double LocTime = GetWholeTime() * tTime;
	double t0 = static_cast<double>(GetWaitTime0());
	if (LocTime <= t0)
		return 0.;
	double t1 = static_cast<double>(Times.WorkEnd - Times.Start);
	if (LocTime >= t1)
		return 1.;
	double WorkTime = static_cast<double>(GetWorkTime());
	if (WorkTime == 0.) // WorkTime is integer
		return 1.;
	return (LocTime - t0) / WorkTime;
}

void NCadrGeom::FlipXY(NCadrGeom* GeomArray, int Size)
{
	for (int i = 0; i < Size; ++i)
	{
		GeomArray[i].FlipXY();
		GeomArray[i].Reverse();
	}
	for (int i = 0; i < Size / 2; ++i)
	{
		NCadrGeom buf = GeomArray[i];
		GeomArray[i] = GeomArray[Size - i - 1];
		GeomArray[Size - i - 1] = buf;
	}
}

void NCadrGeom::SetLCompFlags(bool Beg, bool End)
{
	type.flags |= (Beg ? CU_LCOMP_BEG : 0) | (End ? CU_LCOMP_END : 0);
}

bool NCadrGeom::HaveLComp() const
{
	return (type.flags & (CU_LCOMP_BEG | CU_LCOMP_END)) != 0;
}

bool NCadrGeom::IsLCompBeg() const
{
	return (type.flags & CU_LCOMP_BEG) != 0;
}

bool NCadrGeom::IsLCompEnd() const
{
	return (type.flags & CU_LCOMP_END) != 0;
}

double NCadrGeom::GetCadrLength(const SMachState* pStartSt, const SMachState* pEndSt) const
{
	if (GetType().type == addcoord)
	{
		double Length = 0.;
		if (pStartSt != nullptr && pEndSt != nullptr)
		{
			SMachState Dif = *pEndSt - *pStartSt;
			if (!Dif.IsZero())
			{
				for (int i = 0; i < Dif.GetSize() - 1; ++i)
					Length = max(fabs(Dif.GetCoord(i)), Length);
			}
		}
		return Length;
	}
	if (IsRotate())
	{
		return GetRotSpecLength();
	}
	return Length();
}

