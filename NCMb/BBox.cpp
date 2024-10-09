// BBox.cpp: implementation of the BBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "ConstDef.h"
#include "BBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BBox::BBox()
{
	Init();
}

BBox::~BBox()
{

}

void BBox::Init()
{
	Def = false;
	Xmin=MAXVAL;
	Xmax=-MAXVAL;
	Ymin=MAXVAL;
	Ymax=-MAXVAL;
	Zmin=MAXVAL;
	Zmax=-MAXVAL;
}

double BBox::MaxLineD2(const BPoint& p1, const BPoint& p2) const
{
	return fmax(GetMinPoint().LineD2(p1, p2), GetMaxPoint().LineD2(p1, p2));
}

bool BBox::Expand(double x, double y, double z)
{
	bool ret = false;
	if(x > Xmax){	Xmax = x;	 ret = true;}
	if(x < Xmin){	Xmin = x;	 ret = true;}
	if(y > Ymax){	Ymax = y;	 ret = true;}
	if(y < Ymin){	Ymin = y;	 ret = true;}
	if(z > Zmax){	Zmax = z;	 ret = true;}
	if(z < Zmin){	Zmin = z;	 ret = true;}
	if(ret) Def = true;
	return ret;
}

bool BBox::IsDefined() const
{
	return Def;
}

double BBox::dX() const
{
	return Xmax-Xmin;
}

double BBox::dY() const
{
	return Ymax-Ymin;
}

double BBox::dZ() const
{
	return Zmax-Zmin;
}

void BBox::Project(BBox &in)
{
	// *this - dimensions of in in screen coord
	GLdouble x,y,z;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);

	Init();
	gluProject(in.Xmin,in.Ymin,in.Zmin,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmin,in.Ymin,in.Zmax,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmin,in.Ymax,in.Zmin,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmin,in.Ymax,in.Zmax,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmax,in.Ymin,in.Zmin,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmax,in.Ymin,in.Zmax,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmax,in.Ymax,in.Zmin,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);
	gluProject(in.Xmax,in.Ymax,in.Zmax,modelMatrix, projMatrix,viewport,&x,&y,&z); 
	Expand(x,y,z);

}

void BBox::Expand(const BBox &in)
{
	if(!in.IsDefined())
		return;
	Def = true;
	Xmin = min(Xmin,in.Xmin);
	Xmax = max(Xmax,in.Xmax);
	Ymin = min(Ymin,in.Ymin);
	Ymax = max(Ymax,in.Ymax);
	Zmin = min(Zmin,in.Zmin);
	Zmax = max(Zmax,in.Zmax);
}

void BBox::Offset(double dx, double dy, double dz)
{
	if(!Def)
		return;
	Xmin -= dx;
	Ymin -= dy;
	Zmin -= dz;
	Xmax += dx;
	Ymax += dy;
	Zmax += dz;
}

bool BBox::HaveInt(const BBox &box)
{
// Returns true if *this has intersection with box
	if(Xmax < box.Xmin)
		return false;
	if(box.Xmax < Xmin)
		return false;
	if(Ymax < box.Ymin)
		return false;
	if(box.Ymax < Ymin)
		return false;
	if(Zmax < box.Zmin)
		return false;
	if(box.Zmax < Zmin)
		return false;
	return true;
}

BBox BBox::Move(double x, double y, double z)
{
	BBox b;
	b.Xmin=Xmin+x;
	b.Xmax=Xmax+x;
	b.Ymin=Ymin+y;
	b.Ymax=Ymax+y;
	b.Zmin=Zmin+z;
	b.Zmax=Zmax+z;
	b.Def = Def;
	return b;
}

const BBox& BBox::Transform(const BMatr &m)
{// transformation with given matrix
	if(!Def)
		return *this;
	BPoint Pmin(Xmin,Ymin,Zmin,1.);
	BPoint Pmax(Xmax,Ymax,Zmax,1.);
	Init();
	BPoint pr = Pmin*m;
	Expand(pr);
	pr = Pmax*m;
	Expand(pr);
	BPoint p(Pmin.GetX(),Pmin.GetY(),Pmax.GetZ(),1.);
	Expand(p*m);
	p.Set(Pmin.GetX(),Pmax.GetY(),Pmax.GetZ(),1.);
	Expand(p*m);
	p.Set(Pmin.GetX(),Pmax.GetY(),Pmin.GetZ(),1.);
	Expand(p*m);
	p.Set(Pmax.GetX(),Pmin.GetY(),Pmin.GetZ(),1.);
	Expand(p*m);
	p.Set(Pmax.GetX(),Pmax.GetY(),Pmin.GetZ(),1.);
	Expand(p*m);
	p.Set(Pmax.GetX(),Pmin.GetY(),Pmax.GetZ(),1.);
	Expand(p*m);
	return *this;
}

bool BBox::Expand(const BPoint &p)
{
	bool ret = false;
	if(p.GetX() > Xmax){	Xmax = p.GetX();	 ret = true;}
	if(p.GetX() < Xmin){	Xmin = p.GetX();	 ret = true;}
	if(p.GetY() > Ymax){	Ymax = p.GetY();	 ret = true;}
	if(p.GetY() < Ymin){	Ymin = p.GetY();	 ret = true;}
	if(p.GetZ() > Zmax){	Zmax = p.GetZ();	 ret = true;}
	if(p.GetZ() < Zmin){	Zmin = p.GetZ();	 ret = true;}
	if(ret) Def = true;
	return ret;
}

void BBox::Project(BBox &in, const BMatr &m)
{
	// m - transformation matrix
	// *this - dimensions of in after the transformation

	Init();
	BPoint p = BPoint(in.Xmin,in.Ymin,in.Zmin)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmin,in.Ymin,in.Zmax)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmin,in.Ymax,in.Zmin)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmin,in.Ymax,in.Zmax)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmax,in.Ymin,in.Zmin)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmax,in.Ymin,in.Zmax)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmax,in.Ymax,in.Zmin)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
	p = BPoint(in.Xmax,in.Ymax,in.Zmax)*m;
	Expand(p.GetX(),p.GetY(),p.GetZ());
}

const BPoint BBox::GetMinPoint(void) const
{
	return BPoint(Xmin, Ymin, Zmin, 1.);
}
const BPoint BBox::GetCenterPoint(void) const
{
	return BPoint((Xmin + Xmax) * 0.5, (Ymin + Ymax) * 0.5, (Zmin + Zmax) * 0.5, 1.);
}
const BPoint BBox::GetRelPoint(RelPos X, RelPos Y, RelPos Z) const
{
	BPoint P(
		FindIntervVal(X, Xmin, Xmax),
		FindIntervVal(Y, Ymin, Ymax),
		FindIntervVal(Z, Zmin, Zmax),
		1);
	
	return P;
}

const BPoint BBox::GetMaxPoint(void) const
{
	return BPoint(Xmax, Ymax, Zmax, 1.);
}
bool BBox::operator ==(const BBox &p) const
{
	if(!Def || !p.Def)
		return false;
	return	Xmax == p.Xmax &&
			Ymax == p.Ymax &&
			Zmax == p.Zmax &&
			Xmin == p.Xmin &&
			Ymin == p.Ymin &&
			Zmin == p.Zmin;

}
double BBox::FindIntervVal(RelPos m_C, double MinVal, double MaxVal)
{
	double val = MinVal;
	switch(m_C)
	{
	case RP_MIN:
		val = MinVal;
		break;
	case RP_MID:
		val = 0.5 * (MinVal + MaxVal);
		break;
	case RP_MAX:
		val = MaxVal;
		break;
	case RP_ZER:
		val = 0.;
		break;
	}
	return val;
}

float * BBox::MakeTriangles(void) const
{
	// All triangles should be counterclockwise from the outside
	float *Triangles = NULL;
	if(!IsDefined())
	{
		Triangles = new float[1];
		Triangles[0] = 0.;
		return Triangles;
	}
	Triangles = new float[12 * 3 * 6 + 1];
	Triangles[0] = 12.f;
	float Buf[3][8];
	Buf[0][0] = float(Xmax); Buf[1][0] = float(Ymin); Buf[2][0] = float(Zmin);
	Buf[0][1] = float(Xmax); Buf[1][1] = float(Ymax); Buf[2][1] = float(Zmin);
	Buf[0][2] = float(Xmax); Buf[1][2] = float(Ymax); Buf[2][2] = float(Zmax);
	Buf[0][3] = float(Xmax); Buf[1][3] = float(Ymin); Buf[2][3] = float(Zmax);
	Buf[0][4] = float(Xmin); Buf[1][4] = float(Ymin); Buf[2][4] = float(Zmin);
	Buf[0][5] = float(Xmin); Buf[1][5] = float(Ymin); Buf[2][5] = float(Zmax);
	Buf[0][6] = float(Xmin); Buf[1][6] = float(Ymax); Buf[2][6] = float(Zmax);
	Buf[0][7] = float(Xmin); Buf[1][7] = float(Ymax); Buf[2][7] = float(Zmin);

	for(int i = 0; i < 3; ++i)
	{
		int Np = 0;
		Triangles[1 + (Np + 0) * 3 + i] = 1.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][0];
		Triangles[1 + (Np + 2) * 3 + i] = 0.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][1];
		Triangles[1 + (Np + 4) * 3 + i] = 0.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][2];

		Np = 6;
		Triangles[1 + (6) * 3 + i] = 1.;
		Triangles[1 + (7) * 3 + i] = Buf[i][0];
		Triangles[1 + (8) * 3 + i] = 0.;
		Triangles[1 + (9) * 3 + i] = Buf[i][2];
		Triangles[1 + (10) * 3 + i] = 0.;
		Triangles[1 + (11) * 3 + i] = Buf[i][3];

		Np = 12;
		Triangles[1 + (12) * 3 + i] = -1.;
		Triangles[1 + (13) * 3 + i] = Buf[i][4];
		Triangles[1 + (14) * 3 + i] = 0.;
		Triangles[1 + (15) * 3 + i] = Buf[i][5];
		Triangles[1 + (16) * 3 + i] = 0.;
		Triangles[1 + (17) * 3 + i] = Buf[i][6];

		Np = 18;
		Triangles[1 + (18) * 3 + i] = -1.;
		Triangles[1 + (19) * 3 + i] = Buf[i][4];
		Triangles[1 + (20) * 3 + i] = 0.;
		Triangles[1 + (21) * 3 + i] = Buf[i][6];
		Triangles[1 + (22) * 3 + i] = 0.;
		Triangles[1 + (23) * 3 + i] = Buf[i][7];

		Np = 24;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][1];
		Triangles[1 + (Np + 2) * 3 + i] = 1.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][7];
		Triangles[1 + (Np + 4) * 3 + i] = 0.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][6];

		Np = 30;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][1];
		Triangles[1 + (Np + 2) * 3 + i] = 1.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][6];
		Triangles[1 + (Np + 4) * 3 + i] = 0.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][2];

		Np = 36;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][0];
		Triangles[1 + (Np + 2) * 3 + i] = -1.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][3];
		Triangles[1 + (Np + 4) * 3 + i] = 0.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][5];

		Np = 42;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][0];
		Triangles[1 + (Np + 2) * 3 + i] = -1.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][5];
		Triangles[1 + (Np + 4) * 3 + i] = 0.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][4];

		Np = 48;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][2];
		Triangles[1 + (Np + 2) * 3 + i] = 0.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][6];
		Triangles[1 + (Np + 4) * 3 + i] = 1.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][5];

		Np = 54;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][2];
		Triangles[1 + (Np + 2) * 3 + i] = 0.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][5];
		Triangles[1 + (Np + 4) * 3 + i] = 1.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][3];

		Np = 60;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][0];
		Triangles[1 + (Np + 2) * 3 + i] = 0.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][4];
		Triangles[1 + (Np + 4) * 3 + i] = -1.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][1];

		Np = 66;
		Triangles[1 + (Np + 0) * 3 + i] = 0.;
		Triangles[1 + (Np + 1) * 3 + i] = Buf[i][1];
		Triangles[1 + (Np + 2) * 3 + i] = 0.;
		Triangles[1 + (Np + 3) * 3 + i] = Buf[i][4];
		Triangles[1 + (Np + 4) * 3 + i] = -1.;
		Triangles[1 + (Np + 5) * 3 + i] = Buf[i][7];
	}
	return Triangles;
}

const BPoint BBox::RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
	double MinT = 1.e12;
	BPoint MinP = GetMinPoint();
	BPoint MaxP = GetMaxPoint();
	BPoint p0 = MinP - P;
	BPoint n(-1., 0., 0., 0.);
	{
		double z = n * V;
		if (fabs(z) >= MINAR)
		{
			double t = (p0 * n) * (1. / z);
			if (t < MinT && t >= 0.)
			{
				BPoint B = V * t + P;
				if (B.GetZ() >= MinP.GetZ() && B.GetY() >= MinP.GetY() && B.GetZ() <= MaxP.GetZ() && B.GetY() <= MaxP.GetY())
				{
					MinT = t;
					N = n;
				}
			}
		}
	}

	n.Set(0., -1., 0., 0.);
	{
		double z = n * V;
		if (fabs(z) >= MINAR)
		{
			double t = (p0 * n) * (1. / z);
			if (t < MinT && t >= 0.)
			{
				BPoint B = V * t + P;
				if (B.GetX() >= MinP.GetX() && B.GetZ() >= MinP.GetZ() && B.GetX() <= MaxP.GetX() && B.GetZ() <= MaxP.GetZ())
				{
					MinT = t;
					N = n;
				}
			}
		}
	}

	n.Set(0., 0., -1., 0.);
	{
		double z = n * V;
		if (fabs(z) >= MINAR)
		{
			double t = (p0 * n) * (1. / z);
			if (t < MinT && t >= 0.)
			{
				BPoint B = V * t + P;
				if (B.GetX() >= MinP.GetX() && B.GetY() >= MinP.GetY() && B.GetX() <= MaxP.GetX() && B.GetY() <= MaxP.GetY())
				{
					MinT = t;
					N = n;
				}
			}
		}
	}

	p0 = MaxP - P;
	n.Set(1., 0., 0., 0.);
	{
		double z = n * V;
		if (fabs(z) >= MINAR)
		{
			double t = (p0 * n) * (1. / z);
			if (t < MinT && t >= 0.)
			{
				BPoint B = V * t + P;
				if (B.GetZ() >= MinP.GetZ() && B.GetY() >= MinP.GetY() && B.GetZ() <= MaxP.GetZ() && B.GetY() <= MaxP.GetY())
				{
					MinT = t;
					N = n;
				}
			}
		}
	}

	n.Set(0., 1., 0., 0.);
	{
		double z = n * V;
		if (fabs(z) >= MINAR)
		{
			double t = (p0 * n) * (1. / z);
			if (t < MinT && t >= 0.)
			{
				BPoint B = V * t + P;
				if (B.GetX() >= MinP.GetX() && B.GetZ() >= MinP.GetZ() && B.GetX() <= MaxP.GetX() && B.GetZ() <= MaxP.GetZ())
				{
					MinT = t;
					N = n;
				}
			}
		}
	}

	n.Set(0., 0., 1., 0.);
	{
		double z = n * V;
		if (fabs(z) >= MINAR)
		{
			double t = (p0 * n) * (1. / z);
			if (t < MinT && t >= 0.)
			{
				BPoint B = V * t + P;
				if (B.GetX() >= MinP.GetX() && B.GetY() >= MinP.GetY() && B.GetX() <= MaxP.GetX() && B.GetY() <= MaxP.GetY())
				{
					MinT = t;
					N = n;
				}
			}
		}
	}

	if (MinT > 1.e6)
		return 	BPoint(0., 0., 0., -1.);
	return V * MinT + P; 
}

void BBox::ApplyLimits(void)
{
	if(!IsDefined())
		return;
	Xmin = max(-1.e5, Xmin);
	Xmax = min(1.e5, Xmax);
	Ymin = max(-1.e5, Ymin);
	Ymax = min(1.e5, Ymax);
	Zmin = max(-1.e5, Zmin);
	Zmax = min(1.e5, Zmax);
}

bool BBox::Contains(const BPoint &P) const
{
	return 
		(P.GetX() <= Xmax && P.GetX() >= Xmin)
	&&	(P.GetY() <= Ymax && P.GetY() >= Ymin)
	&&	(P.GetZ() <= Zmax && P.GetZ() >= Zmin)
	;
}

bool BBox::Intersects(const BBox &In) const
{
	return !(In.Zmin >= Zmax || In.Zmax <= Zmin || In.Ymin >= Ymax || In.Ymax <= Ymin || In.Xmin >= Xmax || In.Xmax <= Xmin);
}
