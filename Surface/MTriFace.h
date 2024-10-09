#pragma once

#include "BPoint.h"
#include "MVRDSmember.h"

class MTriFace : public MVRDSmember
{
public:
	MTriFace(void);
	~MTriFace(void);
protected:
	int NPoints[3];
	int NEdges[3];
	double Radius;//Circumcircle radius
	BPoint Center;//Circumcircle center
public:
	void Set(MVRDS *pV, int E1, int E2, int E3, int P1, int P2, int P3, const BPoint &C, double R);
	int GetPointNum( int Num) const { return NPoints[Num];}
	int GetEdgeNum( int Num) const { return NEdges[Num];}
	double GetRadius() const { return Radius;}
	const BPoint & GetCenter() const { return Center;}
};
