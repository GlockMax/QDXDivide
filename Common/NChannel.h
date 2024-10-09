#pragma once

#include <map>
#include "NxyzImage.h"
#include "CMapStringToNProgram.h"
#include "MTMashineTool.h"
#include "FProg2BSP.h"
#include "NProgIntWalker.h"
#include "FCutting.h"
#include "NEvalStopCrit.h"
#include "FAnimateDisp.h"

class NChannel
{
public:
	typedef std::vector<std::pair<int, NProgError>> ErrVector;

	NChannel();
	~NChannel();

protected:
	MTMachineTool MachCopy;
	class NCUnit* Unit;
	CMapStringToNProgram NProgramList;
	CString ProgrToDispText;
	int ProgrToDispTextNum;
	concurrency::task<UINT> CuttingThread;
	concurrency::event CuttingEnabled;
	NProgIntWalker WireAnimWalker;
	int ChannelInd;
	FCutting Cutting;
	CNCMDoc* pDoc;
	GDynamicCadr* pDynamicCadr;
	FAnimateDisp AnimateDisp;
	FProg2BSP Prog2BSP;
	bool ChainEnded;
	int CuttingResult;
	bool flNeedProbe;
	bool flNeedTouch;
	bool flProbePresent;
	bool flTouchPresent;
	NEvalStopCrit ProbeSteps2Go;
	cadrID ProbeCadrID;
	MainTime CurCutTime;
	NxyzImage Axes;
	std::map<UINT, BPoint> AxesBases;
public:
	const NProgIntWalker& GetWireAnimWalker() const { return WireAnimWalker; }
	NxyzImage& GetAxes() { return Axes; }
	MainTime GetCurCutTime() const noexcept { return CurCutTime; }
	void SetCurCutTime(MainTime Val) noexcept { CurCutTime = Val; }
	bool IsChainEnded() const noexcept { return ChainEnded; }
	void SetChainEnded(bool Val) noexcept { ChainEnded = Val; }
	concurrency::event& GetCuttingEnabled() noexcept { return CuttingEnabled; }
	FProg2BSP& GetProg2BSP() noexcept { return Prog2BSP; }
	FAnimateDisp& GetAnimateDisp() noexcept { return AnimateDisp; }
	class NCUnit* GetUnitCn() const;
	CMapStringToNProgram* NGetProgramListCn();
	const CMapStringToNProgram* NGetProgramListCn() const;
	bool SetProgrTDTCn(const CString& str);
	bool SetProgrTDTCn(int ProgInd);
	void SetCurProgrTDTCn();
	const CString& GetProgrTDTCn() const;
	void PauseAnimWait(void);
	void ImplReplCn(const BProgram* pProg) const;
	void WaitCuttingThreadCn(void);
	bool StartCutCn(RunStepsStopReason StopReason);
	void Serialize(CArchive& ar);
	void DeleteContents();
	bool DeleteProgCn(const CString& prog);
	void DeleteAllProgCn();
	void DeleteAllSubProgCn();
	void MakeNewChannel(CNCMDoc* pDoc, int InChannelInd);
	int RegenerateProgramsCn(const std::set<int>& PositionsSet);
	double GetRunAnimateT(void);
	int GetRunAnimateCadr(void);
	const NCadrGeom* GetRunAnimateCadrGeom() const;
	RunStepsStopReason RunAnimateStepCn(CNCMDoc* pDoc, int& MaxRunStepsNum);
	void StartAnimWalkerCn(bool UseSetHStart, const BBox &StockGabar, MTMachineTool& MT);
	bool IsInsideCadrCn() const;
	int GetChannelInd() const { return ChannelInd; }
	FCutting& GetCutting() { return Cutting; }
//	GDynamicCadr* StartAnimateCadr(NCMApplicationType StockState);
	GDynamicCadr* StartResultCadr(void);
	GDynamicCadr* GetDynamicCadr(void) const;
	cadrID GetDynamicCadrCadrID(void) const;
	void StopDynamicCadr(void);
	void SetAnimateCadr(class GAnimateCadr* pACadr);
	void FillMachCopy(void);
	MTMachineTool& GetMachCopy(void) { return MachCopy; }
	int CutProgFragmentsCn(RunStepsStopReason StopReason);
	BProgram* GetProgram(int ProgNum);
	CNCMDoc* GetDoc() const { return pDoc; }
	void SetCuttingResult(int Val) { CuttingResult = Val; }
	int GetCuttingResult() { return CuttingResult; }
	BMatr GetProgMatr();
	bool IsTouchPresent() const { return flTouchPresent; }
	bool IsProbePresent() const { return flProbePresent; }
	bool IsNeedProbe() const { return flNeedProbe; }
	bool IsNeedTouch() const { return flNeedTouch; }
	void ResetNeedProbe() { flNeedProbe = false; }
	void ResetNeedTouch() { flNeedTouch = false; }
	const NEvalStopCrit& GetSteps2Go() const { return ProbeSteps2Go; }
	void ProcProbeTouch(RunStepsStopReason StopReason, const class NEvalStopCrit& SC);
	void SetProbePresent();
	void SetTouchPresent();
	void DoTouching(GBodyRender* pActiveRender);
	RunStepsStopReason RunStepsCn(const class NEvalStopCrit& SC);
	void StartRender4Run(bool MakeCopy);
	bool CalcProbePart(const NCadrGeom* pGeom, const BProgram* pProg, class NCollManager& LocCollManager, BProgram::ProbePos& Pos);
	NToolCombined* GetCurToolCn();
	void PrepProgRendersCn(double Eps);
	void PrepErrorsVector(ErrVector& ErrorsVector);
	void PrepErrorHistory();
	bool HaveMinorErrors();
	bool MakeUsedTools(void);
	void DoProbing(GBodyRender* pActiveRender);
	void CrNewAnimateCadr(const NAnimState& AnimState, const NSpeedPar* pSpeedPar);
	void DrawAxes(bool DrawStockMarker, double Brightness, int Tur, int Att);
	void DrawBase(int ucs, int Tur, int Att);
	void CreateAutoTool(NToolCombined& ComTool, const NAutoToolGenParams& AutoGenParams, bool AutoZDisp);
	void GetNeededToolsList(CStringArray& ToolPosList, BProgram::ToolAutoGenHist& TAGList);
	void GetStPosSet(std::set<int>& StPosSet);
	MainTime GetEndTime() const;
	void SetEndTime(MainTime EndTime);
	bool GetAxesBase(UINT Name, BPoint& P) const;
	bool IsAnimWalkerStartedCn() const { return WireAnimWalker.IsStarted(); }
};
/////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

