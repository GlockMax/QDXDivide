// NSurFMHorizontal.h: interface for the NSurFMHorizontal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMHORIZONTAL_H__D1347702_E229_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSURFMHORIZONTAL_H__D1347702_E229_11D4_B237_008048A96FFB__INCLUDED_

#include "NLine.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NSurFMContour.h"
#include "NLine.h"	// Added by ClassView

class NSurFMHorizontal : public NSurFMContour  
{
public:
	int BuildSurface(PartArray& pcs, BuildingType BuildType = surBuildAll);
	void SetForming(const NLine nl, 
					const double angle,
					const double beta, bool bl);
	NSurFMHorizontal();
	virtual ~NSurFMHorizontal();

protected:
	bool BuildFrontSurface(PartArray& pc);
	bool BuildBackSurface(PartArray& pc);
	bool BuildLateralSurface(PartArray& pc);

	bool IsTop;
	double Alf;
	double BAlf;
	NLine Line, Ln_s, Ln_e;
};

#endif // !defined(AFX_NSURFMHORIZONTAL_H__D1347702_E229_11D4_B237_008048A96FFB__INCLUDED_)
