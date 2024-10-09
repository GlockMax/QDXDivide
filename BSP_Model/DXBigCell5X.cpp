#include "stdafx.h"
#include "ConstDef.h"
#include "NObjEnums.h"
#include "RTriRandMesh.h"
#include "GBodyRender.h"
#include "RTriRandMesh.h"
#include "GQuadRenderDX5X.h"
#include "DXSharpEdge.h"
#include "DXBigCell5X.h"

BPoint DXBigCell5X::GroupData::MinPointShift;
double DXBigCell5X::GroupData::StepX = 1.;
double DXBigCell5X::GroupData::StepY = 1.;
double DXBigCell5X::GroupData::StepZ = 1.;
const class DXMemoryPt *DXBigCell5X::GroupData::pActMemory = nullptr;

DXBigCell5X::DXBigCell5X()
{
//	MapEdges.reserve(32000);
}


DXBigCell5X::~DXBigCell5X()
{
}

void DXBigCell5X::DrawSharpEdges(const NColor& EdgesColor, double EdgesWidth) const
{
	glLineWidth(float(EdgesWidth));
	glColor3fv(EdgesColor.array);

	glPushName(OG_SKELETON);
	glBegin(GL_LINES);
	for each (auto & El in SharpEdges)
	{
		glVertex3fv(El.startPt_);
		glVertex3fv(El.endPt_);
	}
	glEnd();
	glPopName();
}

DXBigCell5X::DXBigCell5X(const DXBigCell5X &inBigCell5X) : TriangleArea(inBigCell5X.TriangleArea)
{
	SharpEdges = inBigCell5X.SharpEdges;
}

bool DXBigCell5X::AddSharpEdge(int ip0, int ip1, DXSharpEdge& SharpEdge)
{
	// We need to orient the line because some cards or drivers 
	// (e.g. ATI mobility Radeon HD 3450 driver v.8.533.0.0) don't draw the last pixel in the line rasterization
	GLfloat *pS = TriangleArea.GetAtNP(ip0) + 3;
	GLfloat *pE = TriangleArea.GetAtNP(ip1) + 3;
	SharpEdge.startPt_ = pS;
	SharpEdge.endPt_ = pE;
	//if (pS[0] + pS[1] + pS[2] > pE[0] + pE[1] + pE[2])
	//{
	//	SharpEdge.Invert();
	//}
	return true;
}

bool DXBigCell5X::AddSharpEdge(int ip0, const int ip1[2], DXSharpEdge& SharpEdge)
{
	if (CheckEdgeAngle(ip1))
		return AddSharpEdge(ip0, ip1[0], SharpEdge);
	return false;
}

bool DXBigCell5X::AddSharpEdge(int const ip0[2], int const ip1[2], DXSharpEdge& SharpEdge)
{
	if (CheckEdgeAngle(ip0))
		return AddSharpEdge(ip0[0], ip1, SharpEdge);
	return false;
}

bool DXBigCell5X::CheckEdgeAngle(const int ip[2]) const
{
	GLfloat *pN0 = TriangleArea.GetAtNP(ip[0]);
	GLfloat *pN1 = TriangleArea.GetAtNP(ip[1]);
	const double sp = pN0[0] * pN1[0] + pN0[1] * pN1[1] + pN0[2] * pN1[2];
	if (fabs(sp) >  cos(GQuadRenderDX5X::GetSmoothAngle() * PI / 180.))
		return false;
	return true;
}

void DXBigCell5X::AddSharpInds2Bounds(const DXSharpEdge& SharpEdge)
{
	if(SharpEdge.boundMapIndStart_ != UINT_MAX)
		MapBounds[SharpEdge.boundMapIndStart_].AddSharpInd(UINT(GetSharpEdges().size() - 1));
	if (SharpEdge.boundMapIndEnd_ != UINT_MAX)
		MapBounds[SharpEdge.boundMapIndEnd_].AddSharpInd(UINT(GetSharpEdges().size() - 1));
}

UINT DXBigCell5X::GetAnotherSEdge(UINT BoundsInd, UINT SharpInd) const
{
	if (BoundsInd == UINT_MAX)
		return UINT_MAX;
	const DXBound& Bound = MapBounds[BoundsInd];
	auto Sample = Bound.GetIndexSharp()[0];
	if (Sample == SharpInd)
		return Bound.GetIndexSharp()[1];
	Sample = Bound.GetIndexSharp()[1];
	if (Sample == SharpInd)
		return UINT_MAX;
	return Sample;
}
