#include "stdafx.h"
#include ".\mtriface.h"

MTriFace::MTriFace(void) : MVRDSmember(), Center(0., 0., 0., 1.)
{
}

MTriFace::~MTriFace(void)
{
}

void MTriFace::Set(MVRDS *pV,int E1, int E2, int E3, int P1, int P2, int P3, const BPoint &C, double R)
{
	pVRDS = pV;

	NEdges[0] = E1;
	NEdges[1] = E2;
	NEdges[2] = E3;
	NPoints[0] = P1;
	NPoints[1] = P2;
	NPoints[2] = P3;
	Center = C;
	Radius = R;
}
