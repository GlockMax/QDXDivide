// NCubicPiece.h: interface for the NCubicPiece class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCUBICPIECE_H__8D09DDC9_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NCUBICPIECE_H__8D09DDC9_D2B3_11D4_B237_008048A96FFB__INCLUDED_

#include "BPoint.h"	// Added by ClassView
#include "NPartSurface.h"
#include "NUVPoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BMatr;

class NCubicPiece :public NPartSurface
{
public:
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
	void operator*(const BMatr m);
	void Draw();

	BPoint GetPointFromParams(double u, double v);
	double GetNormalOnDirection(double u, double v, BPoint dir);
	int ThereIsEssayPointOnLine(double a, double b, 
								bool type, double con, 
								BPoint dir, UVPointArray& uvp);

	NCubicPiece();
	virtual ~NCubicPiece();
protected:
	BPoint TangentToEssayLine(double u, double v, BPoint direct, NUVPoint &D);

public:
	NBox GetProjGabar();
	NBox GetGabar();
	BPoint pc[4][4];
};

#endif // !defined(AFX_NCUBICPIECE_H__8D09DDC9_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
