// BProgram.h: interface for the BProgram class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

//#include "stdafx.h"
#include <vector>
#include <set>
#include "UNITS_API.h"
#include "NColor.h"
#include "BGeomArray.h"
#include "NToolID.h"
#include "InfoProgram.h"
#include "NProgError.h"
#include "BBox.h"
#include "BMSChangePos.h"
#include "RLines.h"
#include "cadrID.h"
#include "GotoMarks.h"
#include "BProgVisInterval.h"
#include "NSyncCheckPoints.h"
#include "NAutoToolGenParams.h"

class NCadr;
class NCUnitState;
typedef CTypedPtrArray <CObArray, NCadr*> NCadrArray;
typedef CArray<int, int> NIntArray;

enum PROG_COLOR_MODE
{
	PCM_DEFAULT,
	PCM_INTERP,
	PCM_DCOMP,
	PCM_LCOMP,
	PCM_CYCLE,
	PCM_COOL,
	PCM_TOOLS
};
enum PROG_COLORS
{// Ypu must change PROG_COLORS_NUM if you add or remove member
	PC_MAIN = 0,
	PC_FAST = 1,
	PC_CYCLE = 2,
	PC_CCWARC = 3,
	PC_CWARC = 4,
	PC_LINE = 5,
	PC_ROTATE = 6,
	PC_OFF = 7,
	PC_COOLON = 8,
	PC_DCOMPON = 9,
	PC_LCOMPON = 10
};
const int PROG_COLORS_NUM = 11;

class BHistArray : public CArray< cadrID, cadrID>
{
};

class BUIntArray : public CUIntArray
{
public:
	INT_PTR Find(UINT Val, INT_PTR StartInd, INT_PTR EndInd) const;
};

class UNITS_API BProgram : public CObject
{
public:
	class ToolChangePos
	{
	public:
		int hpos;//history position of tool change
		NToolID ntool;//new tool
	public:
		ToolChangePos(int hpos, const CString& ntool, int turret);
		ToolChangePos();
	};

	class ErrorPos
	{
	public:
		int hpos;//history position of tool change
		int Code;//new tool
	public:
		ErrorPos(int hpos, int ErrCode);
		ErrorPos();
	};

	class UNITS_API ProbePos
	{
	public:
		enum NProbeResult
		{
			PR_Collision,
			PR_Release,
			PR_Empty
		};
		typedef std::vector <std::pair<int, double>> NRegChanges;
		ProbePos(int hpos, bool Processed, double ActPart, NProbeResult ProbeResult);
		ProbePos();
		void SetProcessed(double Part) { Processed = true; ActPart = Part; }
		bool IsProcessed() const { return Processed; }
		int Gethpos() const { return hpos; }
		NRegChanges &GetRegChanges() { return RegChanges; }
		const NRegChanges &GetRegChanges() const { return RegChanges; }
		auto GetActPart() const { return ActPart; }
		NProbeResult GetProbeResult() const { return ProbeResult; }
		void SetProbeResult(NProbeResult val) { ProbeResult = val; }
	protected:
		int hpos;//history position
		bool Processed;
		NRegChanges RegChanges;
		double ActPart; // value in 0-1 interval (realy covered part of the whole cadr)
		NProbeResult ProbeResult;
	};
	typedef CArray <ToolChangePos, const ToolChangePos&> ToolChangeHist;
	typedef CArray <NAutoToolGenParams, const NAutoToolGenParams&> ToolAutoGenHist;
	typedef CMap <int, int, NProgError, NProgError> ErrorHist;
	typedef CArray <ProbePos, const ProbePos &> ProbeHist;
public:
typedef CArray <BMSChangePos, const BMSChangePos &> MSChangeHist;
	BOOL HaveErr();
	void SetHaveErrors(BOOL val = TRUE);
	InfoProgram & GetInfo();
	void FillInfo(NCUnitState *pState);
	const CString & GetComment();
	void SetComment(const CString &);
	BOOL GetTCHist(int ind, int &pos, NToolID &tool) const;
	int GetTCHistSize() const;
	int GetMSHistSize() const;
	void AddToolChangePos(int hpos, const CString& ntool, int nturret);
	void AddTAGPos(const NAutoToolGenParams& TAGPos);
	void AddErrorPos(int hpos, const NProgError &ProgError);
	void AddProbePos(int hpos, bool Collision);
	const NColor & GetColor() const;
	void InvertVisible();
	void SetVisible(bool Val);
	BOOL IsVisible() const;
	const BBox & GetGabar() const;
	BOOL IsEval() const;
	const CString &GetCadrText(INT_PTR i) const;
	const CString &GetCadrTextSafe(INT_PTR i) const;
	const CString GetCadrReTextSafe(INT_PTR i) const;
	BOOL IsManChanged();
	void SetHaveAddCoord();
	void SetHaveSub();
	void SetHaveSpline();
	BOOL HaveSubprog() const;
	BOOL HaveSpline() const;
	BOOL HaveAddCoord() const;
	bool GetNeedToolAxisDir() const { return NeedToolAxisDir;}
	cadrID GetCadrByHist(int HistNum) const;
	int GetHistByHCadr(int HCadrNum) const;
	void ClearEvaluated(const std::set<int>& PositionsSet);
	void ClearEvaluated();
	void ClearEvaluatedPart(int StockPos);
	void DeleteAllGeom();
	void ResetAllGeom();
	void SetEvaluated();
	void SetGeomReady();
	void AjustCadrTimes();
	virtual bool IsCycle();
	CString BLoad(const CString& filename);
	virtual ~BProgram();
	virtual void DrawInt(int StockPos, bool CompOn, bool ProgOn, bool Draw4Pick, const std::vector<BProgVisInterval> & VisInt, int BoldCadr = -1, double t = -1., bool UseAxes = false, bool DrawNodes = false);
	void AddCadr(NCadr *pCadr);
	virtual void FillGabar(const BMatr *MatrArr);
	int GetSize() const;
	NCadr *operator[](int i) const;
	virtual void Serialize(CArchive& ar);
	void SerializeLines(CArchive& ar);
	void SetColor(const NColor &Col) { Colors[PC_MAIN] = Col;}
	DECLARE_SERIAL(BProgram)
private:
	NColor Colors[PROG_COLORS_NUM];
	PROG_COLOR_MODE PCM;
protected:
	BProgram();
protected:
	GotoMarks ProgList;
	GotoMarks ProgListByLineNumber;
	GotoMarks GotoList;
	BHistArray History; // Для каждого элемента из GeomArray хранится соответствующий номер кадра
	MSChangeHist MSHistory;
	NIntArray BookMarkArray;
	std::vector<BPoint> ToolAxisDir;
	std::map<int, int> StPos2Ind;
	std::vector<BGeomArray*> GeomArrays;
	std::vector<BGeomArray*> CompGeomArrays;
	std::vector<RLines> TrajRenders;
	std::vector<RLines> TrajCompRenders;
	double Thickness;
	BOOL HaveErrors;
	InfoProgram Info;
	CString Comment;
	ToolChangeHist TCHistory;
	ErrorHist ErrorHistory;
	ToolAutoGenHist TAGHistory;
	ProbeHist ProbeHistory;
	BOOL Visible;
	BOOL IsEvaluated;
	BOOL IsGeomReady;
	BOOL ManChanged;
	BOOL HaveSub;	//Ihis program has subprogram(s)
					//Execution order is not consequent
	BOOL HaveSpl;	//This program has spline(s)
	BOOL HaveACoord;	//This program has add coords
	BOOL IsInit;
	BBox Gabar;
	NCadrArray CadrArray;
	BUIntArray HCadr2History;
	CTime FileTime;
	bool Aux;
	bool AuxFromUnit;
	bool Protected;
	bool Active;
	bool NeedToolAxisDir;
public:
	const std::vector<BGeomArray*>& GetCompGeomArrays() const { return CompGeomArrays; }
	void Clear(void);
	const NToolID GetToolByHist(int HistInd, int *pHistPos) const;
	const int GetTCIndByHist(int HistInd) const;
	void PrepRenders(double Eps);
	void PrepRenders1(int Ind, double Eps);
	const BHistArray &GetHistory(void) const { return History;}
	size_t GetHistorySize(void) const { return History.GetSize();}
	int GetLastHCadr();
	void AddMSChangePos(int StPos, const class SMachState& Prev, const class SMachState& Next);
	//void AddMSChangePos(int CoordInd, double CoordVal);
	void AddStartMSChangePos(const class SMachState& Curr);
	void ChangeMSChangeStart(const class SMachState& Curr);
	void SetAddCoord(int MSHistNum, const SMachState &AddCoord);
	void GetAddCoord(int MSHistNum, SMachState &AddCoord) const;
	void GetAddCoordPrev(int MSHistNum, SMachState &AddCoord) const;
	CString & TranslitRus(CString & Str, bool Tr2Ru);
	bool CreateRLines(const BGeomArray &GeomArray, RLines &LinesRender, const NColor &CBold, bool Compens);
	class NColor GetCadrColor(NCadrGeom *pGeom, bool Compens);
	double GetThickness(void) const { return Thickness;}
	void SetThickness(double Thick) { Thickness = Thick;}
	void SetAuxColors(void);
	void SetProgColorMode(PROG_COLOR_MODE Mode) { PCM = Mode;}
	int GetHCadr2HistorySize(void) const { return int(HCadr2History.GetSize());}
	const BUIntArray & GetHCadr2History(void) const { return HCadr2History;}
	const CTime &GetFileTime(void) { return FileTime;}
	double GetTrajLength(bool Comp = false) const;
	BOOL IsInitProg() const { return IsInit;}
	void SetInitProg() { IsInit = TRUE;}
	void SetAux() { Aux = true;}
	bool IsAux(void) { return Aux;}
	void SetAuxFromUnit() { AuxFromUnit = true;}
	bool IsAuxFromUnit(void) { return AuxFromUnit;}
	void SetProtected(bool Val) { Protected = Val;}
	bool IsProtected(void) { return Protected;}
	void SetManChanged();
protected:
	void MakeHCadr2History(void);
	void InitEmpty();

public:
	bool CheckLabelAttainability(int Source, int Dest) const;
	bool IsActive(void) const { return Active;}
	ProbeHist &GetProbeHistory() { return ProbeHistory; }
	const ProbeHist &GetProbeHistory() const { return ProbeHistory; }
	BGeomArray& GetCompGeomArrayByStPos(int StPos);
	BGeomArray& GetGeomArrayByStPos(int StPos);
	const BGeomArray& GetCompGeomArrayByStPos(int StPos) const;
	const BGeomArray& GetGeomArrayByStPos(int StPos) const;
	const BGeomArray& GetCompGeomArray(int Ind = 0) const;
	const BGeomArray& GetGeomArray(int Ind = 0) const;
	int Get1GeomArraySize(int StPos) const;
	int GetGeomArraySize() const;
	int GetGeomArraysCount() const { return int(StPos2Ind.size()); }
	MainTime SetTimesInt(int StartInd, int EndInd, MainTime StartTime);
	void SetWorkT(int CadrInd, MainTime iTime);
	void SetEndT(int CadrInd, MainTime iTime);
	const ErrorHist& GetErrorHistory() const { return ErrorHistory; }
	ErrorHist& GetErrorHistory() { return ErrorHistory; }
	void SyncTCandTAG();
	const auto& GetTAGHistory() { return TAGHistory; }
	int GetIndexByStPos(int StPos) const;
	void PropogateTimeInd();
	void ResizeToolAxisDir() { ToolAxisDir.resize(GetGeomArraySize()); }
	void SetToolAxisDir(int Ind, const BPoint& Vec) { ToolAxisDir[Ind] = Vec; }
	bool GetToolAxisDir(int Ind, BPoint& Vec) { if (Ind >= 0 && Ind < ToolAxisDir.size()) { Vec = ToolAxisDir[Ind]; return true; } else return false; }
	bool HasBookMark(int Ind) const;
	const NIntArray& GetBookMarkArray() const { return BookMarkArray; }
	void Copy2BookMarkArray(const CArray<int, int>& Arr) { BookMarkArray.Copy(Arr); }
	auto& GetHistory() { return History; }
	auto& GetMSHistory() { return MSHistory; }
	auto& GetProgList() { return ProgList; }
	auto& GetProgListByLineNumber() { return ProgListByLineNumber; }
	auto& GetGotoList() { return GotoList; }
};
