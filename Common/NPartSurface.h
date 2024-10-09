// NPartSurface.h: interface for the NPartSurface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPARTSURFACE_H__B1B82101_E8A1_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NPARTSURFACE_H__B1B82101_E8A1_11D4_B237_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Gl\GLu.h"
#include "BPoint.h"

//class BPoint;
class BMatr;
class NBox;
class NUVPoint;
class NContour;

const double ACCURACY1 = 0.00001;
const double ACCURACY_INTER = 0.01;

typedef CArray<NUVPoint,NUVPoint> UVPointArray;

class AFX_EXT_CLASS NPartSurface  
{
public:
	virtual NBox GetGabar()=0;
	virtual NBox GetProjGabar()=0;
	virtual	void GetParamForInitLineUV(double u, double v, 
							   BPoint direct,
							   NUVPoint &T, NUVPoint &N, 
							   NUVPoint &B, double &K) =0;
	virtual void GetParamForInitLine(double u, double v, 
							 BPoint direct,
							 BPoint &T, BPoint &N,
							 BPoint &B, double &K) =0;
	virtual BPoint TangentToEssLine(double u, double v, BPoint direct) =0;
	virtual NUVPoint TangentToEssLineUV(double u, double v, BPoint direct) =0;
	virtual BPoint Normal(double u,double v) =0;
	virtual BPoint TangentRv(double u, double v) = 0;
	virtual BPoint TangentRu(double u, double v) = 0;

	virtual void Draw() = 0;
	virtual void Modify(const BMatr m) = 0;

	BOOL BuildEssayLine(BPoint dir, UVPointArray& uvp, NContour &cont);
	virtual BPoint GetPointFromParams(double u, double v) =0;
	virtual double GetNormalOnDirection(double u, double v, BPoint dir) =0;
	virtual int ThereIsEssayPointOnLine(double a, double b, 
										bool type, double con, 
										BPoint dir, UVPointArray& uvp) =0;

	NPartSurface();
	virtual ~NPartSurface();

protected:
	bool ImprovOfLine(UVPointArray &uvps, BPoint direct);

	bool ImprovOfSimpleLine(UVPointArray &uvps, BPoint direct);
	BOOL InterpolationLine(UVPointArray& uvps, NContour& con, BPoint direction);

	void MotionOnEssLines(NUVPoint uv, double len, UVPointArray& uvps, BPoint direct);
	double ZeroIn(double Au, double Bu, double Av, double Bv, 
				  double Tol, BPoint direct);

};

#endif // !defined(AFX_NPARTSURFACE_H__B1B82101_E8A1_11D4_B237_008048A96FFB__INCLUDED_)
