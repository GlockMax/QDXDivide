// NSurFMSphere.h: interface for the NSurFMSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMSPHERE_H__D1347701_E229_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSURFMSPHERE_H__D1347701_E229_11D4_B237_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NSurFMContour.h"
#include "NLine.h"	// Added by ClassView

class NSurFMSphere : public NSurFMContour  
{
public:
	void SetForming(const NLine nl, 
					const double angle,
					const double beta, bool bl);
	int BuildSurface(PartArray& pcs, BuildingType BuildType = surBuildAll);
	NSurFMSphere();
	virtual ~NSurFMSphere();

protected:
	bool BuildSurfaceRotation(PartArray &pc, BPoint st);
	bool BuildLateralSurface(PartArray& pc);
	bool BuildBackSurface(PartArray& pc);
	bool BuildFrontSurface(PartArray& pc);

	double Alf;
	double BAlf;
	double QAlf;
	double GAlf;
	NLine Line;
	BPoint cent_sp;
};

#endif // !defined(AFX_NSURFMSPHERE_H__D1347701_E229_11D4_B237_008048A96FFB__INCLUDED_)
