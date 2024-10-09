// NSurFMSimTool.h: interface for the NSurFMSimTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSURFMSIMTOOL_H__8D09DDCC_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
#define AFX_NSURFMSIMTOOL_H__8D09DDCC_D2B3_11D4_B237_008048A96FFB__INCLUDED_

#include "NContour.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "surEnums.h"
#include "PartArray.h"

class NSurFMContour;
class NTool;

class NSurFMSimTool  
{
public:
	void BuildSurface(PartArray& OutArray, 
								 NLine DirLine,
								 BPoint cent,
								 DirectionType DirType,
								 BuildingType BuildType);
	void SetTool(NTool *pT);
	NSurFMSimTool();
	virtual ~NSurFMSimTool();

protected:
	BOOL CreateContour();

protected:
	NContour Contour;
	NTool* pTool;
};

#endif // !defined(AFX_NSURFMSIMTOOL_H__8D09DDCC_D2B3_11D4_B237_008048A96FFB__INCLUDED_)
