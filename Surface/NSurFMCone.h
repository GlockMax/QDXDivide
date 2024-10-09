// NSurFMCone.h: interface for the NSurFMCone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMCONE_H__9F62A6A4_DBF5_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSURFMCONE_H__9F62A6A4_DBF5_11D4_B237_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NSurFMContour.h"
#include "NLine.h"	// Added by ClassView
#include "BPoint.h"	// Added by ClassView

class NSurFMCone : public NSurFMContour  
{
public:
	bool GetFlag();
	void SetForming(const NLine nl, 
					const double angle,
					const double beta, bool bl);
	int BuildSurface(PartArray& pcs, BuildingType BuildType = surBuildAll);
	NSurFMCone();
	virtual ~NSurFMCone();


protected:
	bool IsMarked;
	bool BuildLateralSurface(PartArray& pc);
	bool BuildBackSurface(PartArray& pc);
	bool BuildFrontSurface(PartArray& pc);

	double Alf;
	double BAlf;
	NLine Line;
};

#endif // !defined(AFX_NSURFMCONE_H__9F62A6A4_DBF5_11D4_B237_008048A96FFB__INCLUDED_)
