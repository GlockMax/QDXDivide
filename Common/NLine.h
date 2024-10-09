// NLine.h: interface for the NLine class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "NCMB_API.h"
#include "BPoint.h"


class NCMB_API NLine
{
public:
	void DrawHr() const;
	BOOL BuildBallArc(double angle = -1, double R = 0);
	BPoint GetRtt(double t);
	double GetCurvature(double t);
	void Ball2Bernstain();
	void Bernstain2Ball();
	void Draw() const;
	void Set(BPoint p0, BPoint p1, BPoint p2, BPoint p3);
	BPoint GetPointFromParam(double t) const;
	BPoint Tangent(double t);
	NLine operator*(const BMatr m) const;
	BPoint p[4];
	NLine();
	virtual ~NLine();

	BPoint GetCenter(void) const;
	double GetRad(void) const;
	BMatr GetDriveMatr(double u);
	const BPoint GetDir(int k)  const;
	double GetLength(void) const;
	double Deflect(void)const;
	void Divide2(NLine & L1, NLine & L2);
	void Divide2(double t, NLine & L1, NLine & L2);
	double Deflect2(void) const;
	int GetNumAproxLine(double Eps) const;
	void Approximate(double Eps, class BPointsBuf &Pts, bool First) const;
	bool RayCast(const BPoint& P, const BPoint& V, double Epsilon, BPoint &Pres) const;
	void ZeroCurv(bool Start);
};
