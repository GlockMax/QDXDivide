#pragma once
#include "NCTool_API.h"
#include <utility>
#include "BTimeLine.h"
#include "BBox.h"

typedef std::pair<MainTime, MainTime> TimeSlot;
#define TimeSlotMax MainT_UNDEF

class NCTOOL_API NProgWalker
{
public:
	NProgWalker(class NMachUnitPair* pMU, const class BBox& StockGabar, double LengthS,
		class SMachState *pPrev, class SMachState *pNext, bool UseTrackFeed);
	NProgWalker(void) {};
	~NProgWalker(void);
	bool NextStep(void);
	void StepBack() { CurStep -= ((CurStep == 0) ? 0 : 1); }
	int CalcDichCount() const;
	void DoubleSteps() { CurStep *= 2; StepsNum *= 2; LengthStep *= 0.5; }
	void SetTool(const class NToolCombined *pT) {pTool = pT;}
	bool SetGeom(const class NCadrGeom *pG);
	bool FillAddCoords(const class NCadrGeom *pG, const class BProgram *pP);
	void SetAnimLengthStep(void);
	bool SetStart(void);
	bool SetEnd(void);
	bool SetLengthStep(double LengthS);
	const class NToolCombined *GetTool(void) { return pTool;}
	double CalcCurT(void);
	bool IsInsideCadr() const { return CurStep < StepsNum && CurStep > 0; }
	int GetStepsNum() const { return StepsNum; }
	const NCadrGeom* GetGeom() const { return pGeom; }
	TimeSlot GetCurTimeSlot() const;
	MainTime GetCurTime() const;
	double GetCurGeomT() const;
protected:
	bool CalcCurCoordPW(class BPoint &XYZ, class BPoint &ABC, class SMachState &AddCoord);
	void SetMT2CurPos();
protected:
	class NMachUnitPair* pMUPair;
	const class NToolCombined *pTool;
	const NCadrGeom *pGeom;
	class SMachState *pAddCPrev;
	class SMachState *pAddCNext;
	BMatr MCS; // this matrix corresponds to coord. system rotation and ignores machine tool rotation
	int StepsNum;
	int CurStep;
	double Surplus;// The rest of the LengthStep not processed in the previous cadr
	double LengthStep;// in length units
	BBox StockGabar;
	bool TrackFeed;
};
