#pragma once

#include "MVRDSmember.h"

class MVRDS;
class BPoint;

class MEdge : public MVRDSmember
{

public:
	MEdge(void);
	MEdge(MVRDS *pS);
	MEdge(MVRDS *pS, int V0, int V1, int T0, int T1);
	~MEdge(void);
	static const int NOT_DEFINED = -2;
	static const int EMPTY = -1;

protected:
	int NVertexes[2];
	int NFaces[2];
public:
	double GetSignedRad(const BPoint & P,
		const BPoint &SphCenter, double SphRad,
		BPoint &C) const;
	void SetVertexes(int VNum1, int VNum2) { NVertexes[0] = VNum1; NVertexes[1] = VNum2;}
	void Clear();
	bool Join(const MEdge & Edge);
	void Invert(void);
	int GetLeftTriN(void) const { return NFaces[0];}
	int GetRightTriN(void) const { return NFaces[1];}
	int GetStartVN() const{ return NVertexes[0]; }
	int GetEndVN() const{ return NVertexes[1]; }
	void SetLeftTriNum(int Num){ NFaces[0] = Num;}
	void SetRightTriNum(int Num){ NFaces[1] = Num;}
	double GetLength2(void) const;
	BPoint GetMiddle(void) const;
};
