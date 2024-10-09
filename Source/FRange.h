#pragma once
#include "NCEnums.h"
#include "NCMComData.h"
#include "BRangePos.h"

class BProgram;
class MBody;

class FRange
{
public:
	class intPair
	{
	public:
		intPair(int f = 0, int s = 0) : First(f), Second(s) {}
		int First;
		int Second;
		const intPair &invert() { First = -First - 1; Second = -Second - 1; return *this; }
		void increase() { First += (First >= 0) ? 1 : -1; Second += (Second >= 0) ? 1 : -1; }
		void zero() { First = 0; Second = 0; }
	};
public:
	FRange(class NChannel* pChan, const class NSpeedPar* pSpeedPar, int PN, const BProgram * pProg, int StartH, int EndH);
	~FRange(void);
	int AnimateFragment(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection &Stocks);
	int AnimateFragmentTurn(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection &Stocks);
	int AnimateFragmentMill(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection &Stocks);
	int AnimateOneStep(bool ResetCuttingType, class GOneStepGeom &OneStepGeom, class CStocksCollection &Stocks, double tPart, bool NeedReSetRender = true);
	int CutOneStep(class GOneStepGeom &OneStepGeom, class CStocksCollection &Stocks, DX_TOOLTYPE DXType, INT_PTR &LastIndex);
	int ReRenderFragment(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection& Stocks);
	int DynProcFragmentColl(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection& Stocks);
	int DynProcFragmentColl1Tool(const NToolCombined* pTool, const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection& Stocks, NMachUnitPair& Pair);
	int ReRenderFragmentInt(const NCadrGeomArray& Geom, int GNumber, class CStocksCollection &Stocks);
	static NCadrGeom * MakeRightOrd(const NCadrGeomArray & Geom, int IndShift, int& GNumber);
	int ReRenderFragmentDX(const NCadrGeomArray& Geom, int GNumber, class CStocksCollection &Stocks, const class NToolCombined *pTool, DX_TOOLTYPE DXType);
	intPair AnimateRange(CWnd* pMainWnd);
	intPair AnimateRangeMultiStocks(CWnd* pMainWnd);
	int DynProcFragment(const CArray<NCadrGeomArray>& Geoms, int GNumber, class CStocksCollection& Stocks);
	int GetProgNum(void) const;
	void SetAnimMode(NGlobalState::AnimModes mode);
	NCMApplicationType GetStockState(void) const;
	const BMTLinkage& GetMTLink() const;
	FRange * ExtractPart(void);
	int GetStartHCadr(void) const { return StartHCadr;}
	int GetEndHCadr(void) const { return EndHCadr;}
protected:
	class NChannel* pChannel;
	BRangePos RangePos;
	const BProgram *pProg;
	const class NToolCombined* pFRTool;// original tool
	const class NToolCombined* pWorkTool;// tool with margines for cutting coll check
	int StartHCadr;
	int EndHCadr;
	NGlobalState::AnimModes AnimMode;
	NCMApplicationType StockState;
	BMTLinkage MTLink;
	std::vector<BMatrPair> Matrs;
	std::vector<BMatr> RotMatrs;
	std::vector<BMatr> MCSMatrs;
	bool CuttingTypeChanged;
	const NSpeedPar* pSpeedPar;

protected:
	void DefineType(void);
	const NCadrGeom &GetStartCadr(void);
	void SetMT2CurPosAddCoord(const NCadrGeom& Geom);

public:
	const BProgram * GetProg(void) const;
	const NToolCombined * GetFRTool(void) const;
	class CStocksCollection &GetStocksCollection(void) const;
	DX_TOOLTYPE CanDXBeUsed(const CStocksCollection & StockBody, const NCadrGeomArray & Geom, int GNumber) const;
	bool HasCutting(void);
	void Check4TurnGroove();
	void Check4LinkChange();
	void Check4TurnSpindle();
	void Check4AddCoord();
	auto GetDoc() const { return pChannel->GetDoc(); }
	void ResetCuttingTypeChanged() noexcept { CuttingTypeChanged = false; }
	void SetCuttingTypeChanged(const FRange& Prev);
	void ProcCuttingTypeChanged();
	static void DivideF(bool TrackFeed, NCadrGeom& Source, double TValue, NCadrGeom& Dest, FRange* pR);
	static MainTime GetNextBreakTime(MainTime StartTime, FRange* pR);
	std::vector<BStockGrID> GetStockGr4Turn() const;
};
