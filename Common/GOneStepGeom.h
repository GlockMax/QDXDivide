#pragma once
#include "StdAfx.h"
#include <vector>
#include "NCadrGeomArray.h"

class FRange;
class AFX_EXT_CLASS  GOneStepGeom : public NCadrGeomArray
{
public:
	GOneStepGeom(const NCadrGeomArray &inGeom, bool iNeedBackNums, DX_TOOLTYPE iDXType);
	~GOneStepGeom();
	bool Fill(int GNumber, INT_PTR &Start, double &StartT, bool DivideCadr, INT_PTR &ActiveCadrNum, double &StepParamPerCent, const class NCadrSpeedPar& NCadrSpeedPar
	, void DivideCallB(bool TrackFeed, NCadrGeom& Source, double TValue, NCadrGeom& Dest, FRange*), FRange* pR, MainTime GetTimeCallB(MainTime, FRange*));
	void SetMaxStepLength(double Val) noexcept { MaxStepLength = Val; }
	void SetMaxStepTime(double Val) noexcept { MaxStepTime = Val; }
	void SetTrackFeed(bool Val) noexcept { TrackFeed = Val; }
	void Restart(void);
	INT_PTR GetNextInd(INT_PTR Ind);
	NCadrGeom * operator[] (INT_PTR Ind) const { if (IsRightOrdered) return GetAt(Ind); else return Geom[Ind]; }
	INT_PTR GetSize() const { if (IsRightOrdered) return NCadrGeomArray::GetSize(); else return Geom.GetSize(); }
	bool IsGeomEmpty() const { return GetSize() == 0; }
	int GetOriginalCadrNum(int Ind) const;
	DX_TOOLTYPE GetDXType() { return DXType; }
	INT_PTR FindLastDefined(INT_PTR LastIndex);
	void RecalcPosition(BRangePosColl& OneColl);
protected:
	const NCadrGeomArray &Geom;
	bool TrackFeed;
	double MaxStepLength;
	double MaxStepTime;
	INT_PTR CurCycleStart;
	INT_PTR ApproachCadrNum;
	bool ApproachCadrActive;
	NCadrGeomArray NewCadrs; // For deleting extra cadrs only
	bool IsRightOrdered;
	std::vector<int> BackNums; // The numbers of the cadrs in the original array. Index is the number in result array;
	bool NeedBackNums;
	DX_TOOLTYPE DXType;
};

