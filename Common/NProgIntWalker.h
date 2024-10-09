#pragma once
#include "NProgWalker.h"

class AFX_EXT_CLASS NProgIntWalker
{
public:
	NProgIntWalker(void);
	void Set(const NProgWalker &ProgW);
	~NProgIntWalker(void);
	void SetProgram(const class BProgram &Prog);
	const BProgram & GetProgram(void) const { return *pProg;}
	bool SetInterval(int StartAbsI, int EndAbsI, int StartHI, int EndHI);
	bool NextStep(void);
	NProgWalker &GetProgWalker();
	const NProgWalker &GetProgWalker() const;
	int GetEndAbsI() const { return EndAbsInd;}
	const CString &GetCurProg(void) const { return CurProg;}
	void SetCurProg(const CString &CurP) { CurProg = CurP;}
	int GetHEnd(void) const { return HEnd;}
	void SetHEnd(int Val) { HEnd = Val;}
	int GetHStart(void) const { return HStart;}
	void SetHStart(int Val) { HStart = Val;}
	bool IsInsideCadr(void) const { return ProgWalker.IsInsideCadr(); }
	bool GetBreakWasDetected(void) const { return BreakWasDetected; }
	void SetBreakWasDetected(bool Val) { BreakWasDetected = Val; }
	TimeSlot GetCurTimeSlot() const;
	void SetStopped() { Started = false; }
	bool IsStarted() const { return Started; }
protected:
	class NCadrGeom *FindNextGeom();

protected:
	NProgWalker ProgWalker;
	const class BGeomArray *pGeomArray;
	int StartAbsInd;
	int EndAbsInd;

	int CycleApproachInd;

	CString CurProg;
	const BProgram *pProg;
	int HEnd;
	int HStart;
	bool BreakWasDetected;
	bool Started;
};
