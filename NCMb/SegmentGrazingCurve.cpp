#include "stdafx.h"
#include "ConstDef.h"
#include "SegmentGrazingCurve.h"
#include "GrazingCurveElemContour.h"
#include "NLine.h"

SegmentGrazingCurve::SegmentGrazingCurve(void)
{
	color = false;
}
SegmentGrazingCurve::SegmentGrazingCurve(BPoint *pt, int kol)
{
	for(int i = 0; i < kol; i++)
		p.Add(pt[i]);
	color = false;
}
SegmentGrazingCurve::SegmentGrazingCurve(const SegmentGrazingCurve & Segment)
{
	Copy(Segment);	
}
void SegmentGrazingCurve::Copy(const SegmentGrazingCurve & Segment)
{
	int kol = Segment.Getkolgp();
	for(int i = 0; i < kol; i++)
		Add(Segment.GetGP(i));
	color = Segment.GetColor();
}
void SegmentGrazingCurve::Copy(const SegmentGrazingCurve & Segment, const BMatr &M)
{
	int kol = Segment.Getkolgp();
	for(int i = 0; i < kol; i++)
		Add(Segment.GetGP(i) * M);
	color = Segment.GetColor();
}
SegmentGrazingCurve::~SegmentGrazingCurve(void)
{
}
void SegmentGrazingCurve::Add(const BPoint & P)
{
	p.Add(P);
}
void SegmentGrazingCurve::Add(const BPoint *P, int kol)
{
	p.Add(P, kol);
}
void SegmentGrazingCurve::Add(const BPoint & p0, const BPoint & p1, const BPoint & p2, const BPoint & p3)
{
	if(p.Getkol() == 0)//если есть точки
		p.Add(p0);
	p.Add(p1);
	p.Add(p2);
	p.Add(p3);
}
void SegmentGrazingCurve::Add(GPoints *gp)
{
	int kol = gp->Getkol();
	for(int i = 0; i < kol; i++)
		Add(gp->GetPoint(i));
	gp->Clear();
}
void SegmentGrazingCurve::Add(SegmentGrazingCurve & S, bool f)
{
	int kol = S.Getkolgp();
	if(kol > 0)
	{
		if( (GetGP(Getkolgp()-1) - S.GetGP(0)).D2() > DMIN*DMIN )
			Add(S.GetGP(0));
		if(!f)
		{
			for(int i = 1; i < kol; i++)
				Add(S.GetGP(i));
		}
		else
		{
			NLine L;
			for(int i = 0; i+3 < kol; i+=3)
			{
				L.Set(S.GetGP(i), S.GetGP(i+1), S.GetGP(i+2), S.GetGP(i+3));
				int k = L.GetNumAproxLine(GrazingCurveElemContour::EPS() / 2);
				for(int j = 1; j <= k; j++)
					Add(L.GetPointFromParam(j/double(k)).Norm());
			}
		}
		color = S.GetColor();
		S.Clear();
	}
}
void SegmentGrazingCurve::Clear(void)
{
	p.Clear();
}
BPoint SegmentGrazingCurve::GetGP(int i) const
{
	return p.GetPoint(i);
}
void SegmentGrazingCurve::SetColor(bool c)
{
	color = c;
}
bool SegmentGrazingCurve::GetColor(void) const 
{
	return color;
}
void SegmentGrazingCurve::draw(bool f) const
{
	if(color)
		glColor3f(1.0, 0.0, 0.0);
	else
		glColor3f(0.0, 0.0, 0.0);

	
	if(p.Getkol() > 0)
	{
		if(f)
		{
			glBegin(GL_LINE_STRIP);
				for(int i = 0; i < p.Getkol(); i++)
					glVertex3f((GLfloat)(p.GetPoint(i).GetX()+1.),(GLfloat)(p.GetPoint(i).GetY()),(GLfloat)(p.GetPoint(i).GetZ()));
			glEnd();
		}
		else
		{
			NLine L;
			for(int i = 0; i < p.Getkol()-1; i+=3)
			{
				L.Set(p.GetPoint(i), p.GetPoint(i+1), p.GetPoint(i+2), p.GetPoint(i+3));
				L.Ball2Bernstain();
				L.Draw();
			}
		}
	}
}

bool SegmentGrazingCurve::IsCurve(void) const 
{
	if (GetGP(1).GetH() < 1. - HMIN)
		return true;
	if (Getkolgp() != 4)
		return false;
	return GetGP(1).GetH() < 1.;
}
