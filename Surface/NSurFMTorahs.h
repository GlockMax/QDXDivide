// NSurFMTorahs.h: interface for the NSurFMTorahs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMTORAHS_H__94134B40_2F71_11D5_AB10_008048A96FFB__INCLUDED_)
#define AFX_NSURFMTORAHS_H__94134B40_2F71_11D5_AB10_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NSurFMContour.h"
#include "PartArray.h"	// Added by ClassView
#include "NLine.h"	// Added by ClassView
#include "BPoint.h"	// Added by ClassView
#include "NUVPoint.h"
#include "NContour.h"	// Added by ClassView

class NTrimPiece;

class NSurFMTorahs  : public NSurFMContour  
{
public:
	NSurFMTorahs();
	virtual ~NSurFMTorahs();

public:
	int BuildSurface(PartArray& pcs, BuildingType BuildType = surBuildAll);
	void SetForming(const NLine nl, 
					const double angle,
					const double beta, bool bl);

protected:
	BOOL BuildObject(PartArray& pc, bool bCrEssLine, bool bBack);
	BOOL BuildLateralSurface(PartArray& pc);

protected:
	bool EssLineIsCubic;
	CArray<NContour,NContour> EssLine;
	PartArray Object;	// характеристический многогранник
	NLine Line;			// линия образующая тор
	double Alf;
	double BAlf;
};

#endif // !defined(AFX_NSURFMTORAHS_H__94134B40_2F71_11D5_AB10_008048A96FFB__INCLUDED_)
