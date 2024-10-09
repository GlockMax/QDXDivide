#include "stdafx.h"
#include "BaseRender.h"
#include "math.h"
#include "ConstDef.h"
#include "NStock.h"
#include "NOMScaner.h"
#include "NSectionSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NSectionSet::NSectionSet(int PlanesNum)
{
	Visible = true;
	Trimed = false;
	Thickness = 1.;
	NSect = 0;
	for(int i = 0; i < MAX_SECTIONS; ++i)
		Sections[i] = NULL;
	NPlanes = PlanesNum;
	if(NPlanes > 0)
	{
		P = new BPoint[NPlanes];
		N = new BPoint[NPlanes];
		for(int i = 0; i < NPlanes; ++i)
			P[i].Set(0., 0., 0., -1.);
	}
	else
	{
		P = NULL;
		N = NULL;
	}
}

NSectionSet::~NSectionSet(void)
{
	Clear();
	delete[] P;
	delete[] N;
}
PObjectStatus NSectionSet::OnStatusChanged(int ObjectInd, PObjectStatus NewStatus)
{
	MakeAllSections();
	if(NewStatus == DELETED)
		return ST_EMPTY;
	return USED;
}
void NSectionSet::Clear(void)
{
	for(int i = 0; i < NSect; ++i)
	{
		delete[] Sections[i];
		Sections[i] = NULL;
	}
	NSect = 0;
}
int NSectionSet::MakeOneSection(const class NStock &Stock, const BPoint &P, const BPoint &V)
{
	if(NSect >= MAX_SECTIONS - 1)
		return -1;
	GLfloat *pC = Stock.MakeOneSection(P, V, Trimed);
	if(!pC)
		return -1;
	if(int(pC[0]) <= 0)
		return 0;
	Colors[NSect] = Stock.GetColor();
	Sections[NSect++] = pC;
	return int(pC[0]);
}
void NSectionSet::DrawSections(int SInd, int EInd) const
{
	if(!Visible)
		return;
	if(EInd < 0)
		EInd = NSect;
	if(SInd < 0)
		SInd = 0;

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glDisableClientState(GL_COLOR_ARRAY );
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(Thickness);
	glDisable(GL_LIGHTING);
	for(int i = SInd; i < EInd; ++i)
	{
		glColor4fv(Colors[i].array);
		glInterleavedArrays(GL_V3F, 0, Sections[i] + 1);
		glDrawArrays(GL_LINES, 0, int(Sections[i][0]));
	}
	glPopAttrib();
	glPopClientAttrib();
}
int NSectionSet::MakeAllSections(void)
{
	Clear();
	int AreasNum = 0;
	for(NOMScaner Sc(GetObjectManager(), this); !Sc.IsEnded(); Sc.Next())
	{
		NPassiveObject *pPObj = Sc.GetPassiveObject();
		if(!pPObj)
			continue;
		if(!pPObj->IsKindOf(RUNTIME_CLASS(NStock)))
			continue;
		NStock *pStock = (NStock *)Sc.GetPassiveObject();
		if(pStock == NULL || Sc.GetStatus() == DELETED)
			continue;
		AreasNum += MakeStockSections(pStock);
	}
	return AreasNum;
}
int NSectionSet::MakeStockSections(const NStock *pStock)
{
	int AreasNum = 0;
	for(int i = 0; i < NPlanes; ++i)
	{
		if(!P[i].IsPoint())
			continue;
		int res = MakeOneSection(*pStock, P[i], N[i]);
		if(res >= 0)
			AreasNum += res;
	}
	return AreasNum;
}
void NSectionSet::SetPosition(const class BPoint &iP, const class BPoint &iN, int Pos)
{
	ASSERT(Pos < NPlanes && Pos >= 0);
	P[Pos] = iP;
	double d2 = iN.D2();
	if(d2 < MIND * MIND)
		N[Pos].Set(1., 0., 0., 0.);
	else
		N[Pos] = iN * (1. / sqrt(d2));
}


int NSectionSet::SetArcArray(const BPoint & C, const BPoint & V, const BPoint &BaseDir, int Num, double StartA, double EndA)
{
	Trimed = true;
	SetNPlanes(Num);
	for(int i = 0; i < NPlanes; ++i)
	{
		double t = double(i) / (NPlanes - 1);
		double CurAngle = StartA * (1. - t) + EndA * t;
		P[i] = C;
		N[i] = BaseDir * BMatr().rotg(CurAngle, C, V);
	}
	return NPlanes;
}

int NSectionSet::SetLineArray(const BPoint & C, const BPoint & V, int Num , double StartD, double EndD)
{
	Trimed = false;
	SetNPlanes(Num);
	for(int i = 0; i < NPlanes; ++i)
	{
		double t = double(i) / (NPlanes - 1);
		double CurD = StartD * (1. - t) + EndD * t;
		P[i] = C * BMatr().Trans(BPoint(0., 0., 0., 0.), V * CurD);
		N[i] = V;
	}
	return NPlanes;
}

int NSectionSet::SetNPlanes(int Num)
{
	Clear();
	if(Num < 0)
	{
		delete[] P;
		delete[] N;
		P = NULL;
		N = NULL;
		NPlanes = 0;
		return -1;
	}
	if(Num + 1 != NPlanes)
	{
		delete[] P;
		delete[] N;
		NPlanes = Num + 1;
		P = new BPoint[NPlanes];
		N = new BPoint[NPlanes];
		for(int i = 0; i < NPlanes; ++i)
			P[i].Set(0., 0., 0., -1.);
	}
	return NPlanes;
}

void NSectionSet::SetAllColors(const NColor &Col)
{
	for(int i = 0; i < NSect; ++i)
		Colors[i] = Col;
}
