// NPlane.h: interface for the NPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPLANE_H__64F22841_E7DC_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NPLANE_H__64F22841_E7DC_11D4_B237_008048A96FFB__INCLUDED_

#include "BPoint.h"	// Added by ClassView
#include "NPartSurface.h"
#include "NUVPoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BMatr;

class NPlane :public NPartSurface
{
public:
	NBox GetGabar();
	NBox GetProjGabar();

	void GetParamForInitLineUV(double u, double v, 
							   BPoint direct,
							   NUVPoint &T, NUVPoint &N, 
							   NUVPoint &B, double &K);
	void GetParamForInitLine(double u, double v, 
							 BPoint direct,
							 BPoint &T, BPoint &N,
							 BPoint &B, double &K);
	BPoint TangentToEssLine(double u, double v, BPoint direct);
	NUVPoint TangentToEssLineUV(double u, double v, BPoint direct);
	BPoint Normal(double u, double v);
	BPoint TangentRv(double u, double v);
	BPoint TangentRu(double u, double v);
	void Modify(const BMatr m);

	void Set(BPoint pt1, BPoint pt2, BPoint pt3, BPoint pt4);
	void Draw();

	BPoint GetPointFromParams(double u, double v);
	double GetNormalOnDirection(double u, double v, BPoint dir);
	int ThereIsEssayPointOnLine(double a, double b, 
								bool type, double con, 
								BPoint dir,UVPointArray& uvp);

	NPlane();
	NPlane(const NPlane &pl);
	virtual ~NPlane();

protected:
	BPoint TangentToEssayLine(double u, double v, BPoint direct, NUVPoint &D);

public:
	BPoint pc[4];
};

#endif // !defined(AFX_NPLANE_H__64F22841_E7DC_11D4_B237_008048A96FFB__INCLUDED_)
