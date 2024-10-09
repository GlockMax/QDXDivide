// NSurFMComTool.h: interface for the NSurFMComTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMCOMTOOL_H__8D09DDCB_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSURFMCOMTOOL_H__8D09DDCB_D2B3_11D4_B237_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PartArray.h"
#include "NContour.h"	// Added by ClassView
#include "BPoint.h"
#include "surEnums.h"

class NSurFMSimTool;
class NToolCombined;
class NCadrGeom;
class NPartSurface;

typedef CTypedPtrArray <CPtrArray,NCadrGeom*> CagrArray;

class AFX_EXT_CLASS NSurFMComTool  
{
public:
	enum JobState GetStatus();
	enum JobState BeginIssue( NToolCombined *pT);
	enum JobState SendOneCadr(NCadrGeom *pCadr);
	void BuildSurface(PartArray& OutArray);
	NSurFMComTool();
	virtual ~NSurFMComTool();

protected:
	bool SetDirection(int index);

	enum JobState Status;	// состояние 

	NContour DirectContour;
	BPoint CenterPoint;
//	CArray<BPoint, BPoint> CentArray;
	NToolCombined* pTool;
	CagrArray Cadrs;
};

#endif // !defined(AFX_NSURFMCOMTOOL_H__8D09DDCB_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
