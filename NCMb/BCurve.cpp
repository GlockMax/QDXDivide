#include "StdAfx.h"
#include "NCadrGeom.h"
#include "BPointsBuf.h"
#include "BBox.h"
#include "BCurve.h"
#include "BPoint.h"
#include "NLine.h"
#include "GL\glu.h"
#include "bcurve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BCurve::CurveStorage BCurve::Storage; // For deletion only

BCurve::BCurve(int SegmNum) : ToolAxisVec(0., 0., 1., 0.)
{
	Size = SegmNum * 3 + 1;
	p = new BPoint[Size];
}
BCurve::BCurve(const BCurve &In) : ToolAxisVec(In.ToolAxisVec)
{
	Size = In.Size;
	p = new BPoint[Size];
	for(int i = 0; i < Size; ++i)
		p[i] = In.p[i];
}
BCurve::BCurve(void) : ToolAxisVec(0., 0., 1., 0.)
{
	Size = 0;
	p = NULL;
}

BCurve *BCurve::NewBCurve(const BCurve &In)
{
	BCurve *pRes = new BCurve(In);
	Storage.AddCurve(pRes);
	return pRes;
}

BCurve *BCurve::NewBCurve(int SegmNum)
{
	BCurve *pRes = new BCurve(SegmNum);
	Storage.AddCurve(pRes);
	return pRes;
}

BCurve::~BCurve(void)
{
	delete[] p;
}
bool BCurve::SetPoint(int i, const BPoint &P)
{
	if(i < 0 || i >= Size)
		return false;
	p[i] = P;
	return true;
}
void BCurve::Draw(double t)
{// if t < 0 - draw whole curve
	
	if(t == 0.)
		return;

	int SegmNum = (Size - 1) / 3;
	GLfloat *ctrlpoints = new GLfloat[Size * 4]; 
	for(int k = 0; k < SegmNum; ++k)
	{
		int i = k * 3;
		{
			ctrlpoints[i * 4 + 0] = (float)p[i].GetX();
			ctrlpoints[i * 4 + 1] = (float)p[i].GetY();
			ctrlpoints[i * 4 + 2] = (float)p[i].GetZ();
			ctrlpoints[i * 4 + 3] = (float)p[i].GetH();
		}
		i = k * 3 + 1;
		{
			BPoint q = p[i - 1] * (1./3.) + p[i] * (2./3.);
			ctrlpoints[i * 4 + 0] = (float)q.GetX();
			ctrlpoints[i * 4 + 1] = (float)q.GetY();
			ctrlpoints[i * 4 + 2] = (float)q.GetZ();
			ctrlpoints[i * 4 + 3] = (float)q.GetH();
		}
		i = k * 3 + 2;
		{
			BPoint q = p[i + 1] * (1./3.) + p[i] * (2./3.);
			ctrlpoints[i * 4 + 0] = (float)q.GetX();
			ctrlpoints[i * 4 + 1] = (float)q.GetY();
			ctrlpoints[i * 4 + 2] = (float)q.GetZ();
			ctrlpoints[i * 4 + 3] = (float)q.GetH();
		}
		i = k * 3 + 3;
		{
			ctrlpoints[i * 4 + 0] = (float)p[i].GetX();
			ctrlpoints[i * 4 + 1] = (float)p[i].GetY();
			ctrlpoints[i * 4 + 2] = (float)p[i].GetZ();
			ctrlpoints[i * 4 + 3] = (float)p[i].GetH();
		}
	}


	GLfloat *knot = new GLfloat[4 + Size];
	
	knot[0] = 0;
	knot[4 + Size - 1] = (float)SegmNum;
	for(int l = 0; l <= SegmNum; l++)
		for(int j = 0; j < 3; j++)
			knot[1 + l*3 + j] = (float)l;


	{
		glPushAttrib(GL_EVAL_BIT);
		glEnable(GL_MAP1_VERTEX_4);
		GLfloat tf = (float)t;
		if(t < 0.)
			tf = (GLfloat)SegmNum;
		else
		{
			double NeededLength = t * GetLength();
			double PartialLength = 0.;
			for(int k = 0; k < SegmNum; ++k, tf -= 1.)
			{
				NLine Segm;
				Segm.Set(p[k * 3 + 0], p[k * 3 + 1], p[k * 3 + 2], p[k * 3 + 3]); 
				double SegmLength = Segm.GetLength();
				if(PartialLength + SegmLength > NeededLength)
				{
					tf = (GLfloat)(k + (NeededLength - PartialLength) / SegmLength);
					break;
				}
				PartialLength += SegmLength;
			}
		}
		if(tf < 0.)
			tf = float(SegmNum + 1.);
		GLint un = 16;
		for(int k = 0; k < SegmNum && tf >= 0.; ++k, tf -= 1.)
		{
			int i = k * 3;
			glMap1f(GL_MAP1_VERTEX_4, 0., 1., 4, 4, ctrlpoints + k * 3 * 4);
			glMapGrid1f( un, 0.f, (tf < 1.) ? tf : 1.f);
			glEvalMesh1( GL_LINE, 0, un);
		}
		glPopAttrib();
	}
	delete[] knot;
	delete[] ctrlpoints;
}
BBox BCurve::GetGabar(void)
{
	BBox Box;
	for(int i = 0; i < Size; ++i)
	{
		BPoint q = p[i];
		Box.Expand(q.Norm());
	}
	return Box;
}
const BPoint & BCurve::GetLastP() const
{
	return p[Size - 1];
}

const BPoint & BCurve::GetFirstP() const
{
	return p[0];
}
const BPoint BCurve::GetFirstDir() const
{
	BPoint E = p[1];
	return E.Norm() - p[0];
}
const BPoint BCurve::GetLastDir() const
{
	BPoint E = p[Size - 2];
	return  p[Size - 1] - E.Norm();
}
double BCurve::GetLength() const
{
	NLine L;
	double Length = 0.;
	for(int i = 0, j = 0; i < Size / 3; ++i)
	{
		j = 3 * i;
		L.Set(p[j], p[j + 1], p[j + 2], p[j + 3]);
		Length += L.GetLength();
	}
	return Length;
}

void BCurve::Tr(const BMatr & m)
{
	for(int i = 0; i < Size; ++i)
		p[i] = p[i] * m;
}

int BCurve::GetSize(void) const
{
	return Size;
}

BPoint * BCurve::GetSegm(int PointIndex) const
{
	return &p[PointIndex];
}
const BPoint & BCurve::GetPoint(int PointIndex) const
{
	return p[PointIndex];
}

int BCurve::GetNumAppr(double Eps) const
{
	int TotalNum = 1;
	for(int k = 0; k < GetSize() / 3; ++k)
	{
		NLine Line;
		GetLine(k, Line);
		TotalNum  = max(TotalNum, Line.GetNumAproxLine(Eps));
	}
	return TotalNum * (GetSize() / 3);
}

bool BCurve::GetLine(int i, NLine &Line) const
{
	if(i >= GetSize() / 3)
		return false;
	BPoint *pP = GetSegm(i * 3);
	Line.Set(*pP, *(pP + 1), *(pP + 2), *(pP + 3));
	return true;
}

int BCurve::MakeAppr(double Eps, BPointsBuf &Pts) const
{
	for(int k = 0; k < GetSize() / 3; ++k)
	{
		NLine Line;
		GetLine(k, Line);
		Line.Approximate(Eps, Pts, (k == 0));
	}
	return Pts.GetSize();
}

void BCurve::ZeroCurv(bool Start)
{
	// Make zero curvature in the start (if Start) or end point
	NLine Line;
	GetLine((Start ? 0 : GetSize() / 3 - 1) , Line);
	Line.ZeroCurv(Start);
	SetPoint(Start ? 2 : GetSize() - 3, Line.p[Start ? 2 : 1]);
}

bool BCurve::RayCast(const BPoint & P, const BPoint & V, double Epsilon, BPoint & Pres) const
{
	for (int k = 0; k < GetSize() / 3; ++k)
	{
		NLine Line;
		GetLine(k, Line);
		if (Line.RayCast(P, V, Epsilon, Pres))
			return true;
	}
	return false;
}

BCurve::CurveStorage::CurveStorage()
{
	Closed = false;
}

BCurve::CurveStorage::~CurveStorage()
{
	ClearAll();
}

void BCurve::CurveStorage::AddCurve(const BCurve *pCurve)
{
	Closed ? ExtraCurves.push_back(pCurve) : MainCurves.push_back(pCurve);
}

void BCurve::CurveStorage::ClearAll()
{
	for each (const BCurve *pCurve in MainCurves)
	{
		delete pCurve;
	}
	MainCurves.clear();

	ClearExt();

	Closed = false;
}

void BCurve::CurveStorage::ClearExt()
{
	if (ExtraCurves.empty())
		return;
	for each (const BCurve *pCurve in ExtraCurves)
	{
		delete pCurve;
	}
	ExtraCurves.clear();
}

void BCurve::CurveStorage::Close()
{
	Closed = true;
}

void BCurve::CurveStorage::ClearLastCurve()
{
	if (Closed)
	{
		delete ExtraCurves.back();
		ExtraCurves.resize(ExtraCurves.size() - 1);
	}
	else
	{
		delete MainCurves.back();
		MainCurves.resize(MainCurves.size() - 1);
	}
}
