// APTCLOut.h: interface for the APTCLOut class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class AptCLFile;
class NCUnitState;
class NCadrGeom;

class AFX_EXT_CLASS APTCLOut  
{
public:
	void PostState(const NCUnitState *pState, const NCadrGeom &Geom);
	void Init(const NCUnitState *pInitState);
	APTCLOut(const CString &ProgName, const CString &FileName);
	const CString &GetProgName(void) { return PName; };
	virtual ~APTCLOut();

protected:
	NCUnitState * pCurrState;
	NCUnitState * pPrevState;
	AptCLFile *pOutFile;
	CString PName;
};
