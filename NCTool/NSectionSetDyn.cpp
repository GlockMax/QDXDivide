#include "stdafx.h"
#include "BaseRender.h"
#include "BPoint.h"
#include "NColor.h"
#include "math.h"
#include "ConstDef.h"
#include "NStockStl.h"
#include "NOMScaner.h"
#include "NSectionSetDyn.h"

NSectionSetDyn::NSectionSetDyn(void) :
	NSectionSet(1)
{
	Trimed = false;
}

NSectionSetDyn::~NSectionSetDyn(void)
{
}

void NSectionSetDyn::Draw(bool EntrPoints, bool ExitPoints) const
{
	NSectionSet::DrawSections();
	glPushAttrib(GL_POINT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	if(EntrPoints)
	{
		glPointSize(7);
		glColor3d(1., 0., 0.);
		DrawPoints(Entr);
	}
	if(ExitPoints)
	{
		glPointSize(5);
		glColor3d(1., 1., 0.);
		DrawPoints(Exit);
	}
	glPopAttrib();
}

void NSectionSetDyn::DrawPoints(const BPoint Points[]) const
{
	glBegin(GL_POINTS);
	for(int i = 0; i < NSect; ++i)
	{
		const BPoint *pP = &Points[i];
		if(pP->GetH() <= 0.)
			continue;
		glColor4fv(Colors[i].array);
		glVertex3d(pP->GetX(), pP->GetY(), pP->GetZ());
	}
	glEnd();
}

int NSectionSetDyn::MakeAllSections(void)
{
	int res = NSectionSet::MakeAllSections();
	int ind = 0;
	BPoint Norm = RayDir % N[0]; // The Norm vector lyes in section plane and is normal to ray direction
	Norm = Norm * (1. / sqrt(Norm.D2()));
	for(int i = 0; i < NSect; ++i)
	{
		Entr[i] = Exit[i] = BPoint(0., 0., 0., -1.);
		double EntrD2 = 1.e12;
		double ExitD2 = 1.e12;
		GLfloat *Section = Sections[i] + 1;
		int SSize = int(Sections[i][0]);
		for(int k = 0; k < SSize; k += 2)
		{
			BPoint p0(Section[k * 3 + 0], Section[k * 3 + 1], Section[k * 3 + 2], 1.);
			BPoint p1(Section[k * 3 + 3], Section[k * 3 + 4], Section[k * 3 + 5], 1.);

			double z0 = (p0 - P[0]) * Norm;
			double z1 = (p1 - P[0]) * Norm;
			BPoint ResP(P[0]);
			double d0 = fabs(z0);
			double d1 = fabs(z1);
			if(d1 < MIND)
			{
				ResP = p1;
			}
			else if(z0 * z1 < 0.)
			{
				double t = d0 / (d0 + d1);
				ResP = p0 * (1. - t) + p1 * t;
			}
			else
				continue;
			double CurD2 = (P[0] - ResP).D2();
			bool EntrancePoint = (z0 > 0.);
			if(EntrancePoint)
			{
				if(CurD2 >= EntrD2)
					continue;
				EntrD2 = CurD2;
				Entr[i] = ResP;
			}
			else
			{
				if(CurD2 >= ExitD2)
					continue;
				ExitD2 = CurD2;
				Exit[i] = ResP;
			}

		}
	}
	return res;
}

const BPoint & NSectionSetDyn::GetN() const
{
	ASSERT(NPlanes > 0);
	return N[0];
}
const BPoint & NSectionSetDyn::GetP() const
{
	ASSERT(NPlanes > 0);
	return P[0];
}
void NSectionSetDyn::SetPosition(const class BPoint &iP, const class BPoint &iN, const class BPoint &iV)
{
	NSectionSet::SetPosition(iP, iN, 0);
	RayDir = iV;
}
const BBox NSectionSetDyn::GetGabar(void) const
{
	BBox Res;
	for(int i = 0; i < NSect; ++i)
	{
		GLfloat *Section = Sections[i] + 1;
		int SSize = int(Sections[i][0]);
		for(int k = 0; k < SSize; k += 2)
		{
			BPoint p0(Section[k * 3 + 0], Section[k * 3 + 1], 0., 0.);
			Res.Expand(sqrt(p0.D2()), Section[k * 3 + 2], -100.);
			Res.Expand(-sqrt(p0.D2()), Section[k * 3 + 2], +100.);
		}
	}
	Res.Expand(0., 0., 100.);
	return Res;
}
