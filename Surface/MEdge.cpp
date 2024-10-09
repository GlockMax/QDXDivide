#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "MBSPForest.h"
#include "MVRDS.h"
#include "BPoint.h"
#include "MEdge.h"

MEdge::MEdge(void) : MVRDSmember()
{
	Clear();
}

MEdge::MEdge(MVRDS *pS) : MVRDSmember(pS)
{
	Clear();
}
MEdge::MEdge(MVRDS *pS, int V0, int V1, int T0, int T1)
	: MVRDSmember(pS)
{
	NVertexes[0] = V0;
	NVertexes[1] = V1;
	NFaces[0] = T0;
	NFaces[1] = T1;
}

MEdge::~MEdge(void)
{
}

void MEdge::Clear(void)
{
	RemoveActive();
	NVertexes[0] = -1;
	NVertexes[1] = -1;
	NFaces[0] = -1;
	NFaces[1] = -1;
}

double MEdge::GetSignedRad(const BPoint & P,
		const BPoint &SphCenter, double SphRad,
		BPoint &C) const
{// Returns circumcircle radius and center (by C parameter)
	if(!IsDefined())
		return MAX_RAD + 1.;
	const BPoint *pP0 = pVRDS->GetPoint(NVertexes[0]);
	const BPoint *pP1 = pVRDS->GetPoint(NVertexes[1]);

	if( ((*pP1 - *pP0) % (P - *pP0)) * (*pP0 - SphCenter) <= 0.)
		return MAX_RAD + 1.;// Points must be to the left of the edge


	BPoint E0 = (*pP1 - *pP0);
	BPoint E1 = (P - *pP0);
	double L0_2 = E0.D2();
	double L1_2 = E1.D2();
	double E0E1 = E0 * E1;
	double z = L0_2 * L1_2 - E0E1 * E0E1;

	if(fabs(z) < ER2M)
		return MAX_RAD + 1.;
	z = 1./z;

	double A = 0.5 * z * (L0_2 - E0E1) * L1_2;
	double B = 0.5 * z * (L1_2 - E0E1) * L0_2;
			

	C = (*pP0) * (- A - B) + (*pP1) * A + P * B;
	double R = C.D2();
	C = C + *pP0;

	R = sqrt(R) * ((B > 0.) ? 1. : -1.);

	return R;
}

bool MEdge::Join(const MEdge & Edge)
{// If this and Edge are same edges join them and remove active flag
	// return true if edges where joined and false otherwize
	if(!IsDefined())
		return false;
	bool SameDirection = false;
	bool Equal = false;
	if(NVertexes[0] == Edge.NVertexes[0] && NVertexes[1] == Edge.NVertexes[1])
	{
		SameDirection = true;
		Equal = true;
	}
	else if(NVertexes[0] == Edge.NVertexes[1] && NVertexes[1] == Edge.NVertexes[0])
		Equal = true;
	if(!Equal)
		return false;
	RemoveActive();
	if(SameDirection)
	{
		if(NFaces[0] == NOT_DEFINED)
			NFaces[0] = Edge.NFaces[0];
		if(NFaces[1] == NOT_DEFINED)
			NFaces[1] = Edge.NFaces[1];
	}
	else
	{
		if(NFaces[0] == NOT_DEFINED)
			NFaces[0] = Edge.NFaces[1];
		if(NFaces[1] == NOT_DEFINED)
			NFaces[1] = Edge.NFaces[0];
	}
	return true;
}

void MEdge::Invert(void)
{
	int b;
	b = NVertexes[0]; NVertexes[0] = NVertexes[1]; NVertexes[1] = b;
	b = NFaces[0]; NFaces[0] = NFaces[1]; NFaces[1] = b;
}



double MEdge::GetLength2(void) const
{
	return (*MFOREST.GetPoint(pVRDS->GetVertex(NVertexes[0]).GetPointNum()) -
		*MFOREST.GetPoint(pVRDS->GetVertex(NVertexes[1]).GetPointNum())).D2();
}

BPoint MEdge::GetMiddle(void) const
{
	BPoint P = (*MFOREST.GetPoint(pVRDS->GetVertex(NVertexes[0]).GetPointNum())
		+ *MFOREST.GetPoint(pVRDS->GetVertex(NVertexes[1]).GetPointNum())) * 0.5
		- pVRDS->GetSphCenter();
	return pVRDS->GetSphCenter() + P * (pVRDS->GetSphRadius() / sqrt(P.D2())); 
}
