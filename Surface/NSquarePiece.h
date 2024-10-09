// NSquarePiece.h: interface for the NSquarePiece class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSQUAREPIECE_H__64F22842_E7DC_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSQUAREPIECE_H__64F22842_E7DC_11D4_B237_008048A96FFB__INCLUDED_

#include "BPoint.h"	// Added by ClassView
#include "NPartSurface.h"
#include "NUVPoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BMatr;

class NSquarePiece :public NPartSurface
{
public:
	BBox GetGabar();
	BBox GetProjGabar();
	void GetParamForInitLineUV(double u, double v, 
							   BPoint direct,
							   NUVPoint &T, NUVPoint &N, 
							   NUVPoint &B, double &K);
	void GetParamForInitLine(double u, double v, 
							 BPoint direct,
							 BPoint &T, BPoint &N, 
							 BPoint &B, double &K);
	BPoint Normal(double u,double v);
	BPoint TangentRv(double u, double v);
	BPoint TangentRu(double u, double v);
	void Modify(const BMatr m);

	void Draw();
	BPoint GetPointFromParams(double u, double v);
	double GetNormalOnDirection(double u, double v, BPoint dir);
	int ThereIsEssayPointOnLine(double a, double b, 
								bool type, double con, 
								BPoint dir, UVPointArray& uvp);
	
	NSquarePiece();
	virtual ~NSquarePiece();

protected:
	void GetParamForInitEssLine(double u, double v, BPoint direct, BPoint &T, BPoint &N, BPoint &B, double &K, NUVPoint &uvt, NUVPoint &uvtt);
	BPoint TangentToEssayLine(double u, double v, BPoint direct, NUVPoint &D);

public:
	NUVPoint TangentToEssLineUV(double u, double v, BPoint direct);
	BPoint TangentToEssLine(double u, double v, BPoint direct);

	BPoint pc[3][3];
};

#endif // !defined(AFX_NSQUAREPIECE_H__64F22842_E7DC_11D4_B237_008048A96FFB__INCLUDED_)
