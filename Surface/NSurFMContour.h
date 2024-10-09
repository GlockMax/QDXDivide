// NSurFMContour.h: interface for the NSurFMContour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMCONTOUR_H__8D09DDCA_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSURFMCONTOUR_H__8D09DDCA_D2B3_11D4_B237_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NSquarePiece.h"
#include "NCubicPiece.h"
#include "PartArray.h"
#include "NPlane.h"
#include "NLine.h"
#include "BPoint.h"
#include "BMatr.h"
#include "surEnums.h"

class NSurFMContour  
{
public:
	void TempBuild(NLine l1, NLine l2, BPoint C, double arc, bool f, NSquarePiece *pPiece);
	int BuildSurfaceFMArc(PartArray& pcs,
						  double Alf, double Beta,
						  double r, BPoint center, 
						  BPoint start, BPoint end);
	DirectionType GetTypeDirection();
	void SetDirection(NLine ln, BPoint cen, DirectionType DirType = surLINE);
	virtual void SetForming(const NLine nl, 
					const double angle,
					const double beta, bool bl)=0;
	virtual int BuildSurface(PartArray& pcs, BuildingType BuildType) = 0 ;
	NSurFMContour();
	virtual ~NSurFMContour();
	
protected:
	bool BuildSqLinearSurface(NLine line1, NLine line2, bool f, NSquarePiece* pPiece);
	bool BuildCubLinearSurface(NLine line1, NLine line2, bool f, NCubicPiece *pPiece);
	bool BuildArcMoveArcSurface(NLine ln, NSquarePiece *pPiece, BPoint cen, double angle, int xyzType = 3, BPoint axis = BPoint(0.,0.,1.,0.));

protected:
	NLine DirLine;
	BPoint Center;	// если это отрезок то эта точка нулевая
	BPoint Start;
	BPoint End;
	DirectionType Type;
	double arcAngle;
};

#endif // !defined(AFX_NSURFMCONTOUR_H__8D09DDCA_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
