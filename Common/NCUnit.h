// NCUnit.h: interface for the NCUnit class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCUnitState.h"
#include "NCadrGeom.h"
#include "NProgError.h"
#include "NTiltPlane.h"
#include "NCStatesHist.h"
#include "BProgram.h"
#include "NRe2Set.h"
#include "BProgErrHandler.h"
#include "ProgGroupMarks.h"
#include "BStockToolInds.h"
#include <regex>  

enum SearchDir
{
	SD_EMPTY,
	SD_FORBACK,
	SD_FOR,
	SD_BACK,
	SD_BACKFOR
};

enum LabelType
{
	LT_EMPTY,
	LT_BALTSYS,
	LT_SINUMERIK,
	LT_EXPRESSION
};

enum CircleCMode
{
	CCM_Incremental,
	CCM_Absolute,
	CCM_AsCoord
};

struct FDef
{
	CString Num;//func number
	int Group;
	bool Flag;
	CString Name;
	CString FConcepts;
	bool ModalConcepts;
};

typedef CTypedPtrMap<CMapStringToPtr, CString, struct FDef*> NMapStringToFDef;

class NCUnitCommonData
{
	friend class NCUnitBase;
	friend class NCUnit;
protected:
	double LinearDecimalZero;//Added to speed up the application
	double LinearScale;//Added to speed up the application
	double RotaryScale;//Added to speed up the application
	double XScale;//Added to speed up the application
	double YScale;//Added to speed up the application
	double ZScale;//Added to speed up the application
	double IScale;//Added to speed up the application
	double JScale;//Added to speed up the application
	double KScale;//Added to speed up the application
	bool SpiralG2G3;//Added to speed up the application
	bool IsDefined;
	char RegisterSymbol;
	char VariableSymbol;
	char VariableSymbolLoc;
	bool ObserveGroove;
	bool AutoFixRotAxes;
	LabelType LType;
	bool BaltSysStyle;
	bool SinumericStyle;
	bool AllowToolString;
	bool PolarCoordRecalc;
	double MaxWorkFeed;
	double DecimalConversion;
	CircleCMode CircleCenterMode;
	int NumAxes;
	int StockPos4Turn;
	std::set<int> MyStockPos;
	bool UseStdFuncNames;
	double MinArcDist;
	NRe2Set ReplaceSet;
	CMapStringToString SGeneral;
	CMapStringToString SMiscFuncDef;
	CStringArray SWordRep;
	CStringArray SWordOrder;
	NMapStringToFDef SFuncDef;
	NMapStringToFDef SFuncDefInv;
	CMap<CString, LPCSTR, int, int> SRegIndexes;
	CMapStringToString SWordDef;
	CMapStringToString SWordDefInv;
	NCStatesHist States;
	CMapStringToString SOthers;
	CStringArray SCycleDef;
	CString ConfFileName;
	std::vector<CString> ConfParNames;
	bool CheckProgramText;// Special check for Heidenhine only
	int ChannelInd;
	NSyncCheckPoints SyncHistory;
public:
	set<int> UsedUCSs;
};

class CNCMDoc;
class NToolCombined;
class NCycle;
class NProgram;
class BProgram;
class APTCLOut;
class NMiCheckDiamOffsetNum;

//const int FUNC_NUMBER = 15;
////CHINA const CString FUNC_NAMES("скцматдрин");
//const CString FUNC_NAMES("jkscravtqefilm\30");
//const CString FUNC_TITLES[FUNC_NUMBER] = {"ASIN", "ACOS", "SIN", "COS", "ROUND", "ABS", "ATAN", "TAN", "SQRT", "REG", "FUP", "FIX", "LN", "EXP", "POT"};
const int USIZE = 256;
class NCUnitBase   
{
	friend class NMiCheckDiamOffsetNum;

protected:
	class UWordDef
	{
	public:
		UWordDef(void);
		~UWordDef(void);
		void Clear();
		const CString &GetKey(const CString &Value, const NCUnitBase& ThisUnit);
		void SetTemp(const CString &Key, const CString &Value);
		void RemoveTemp(const CString &Value);
	protected:
		CString RKey;
		CString WordDefInv[USIZE];

	};
public:
	enum CompType
	{
		Default,
		Arc,
		Int,
		InternalInt
	};
public:
	static CString OpCharSet;// = _T("-+*/^$<>=!?&|:'") + NEvalExp::OpenBC() + NEvalExp::CloseBC(); // Character set with all operations
protected:
	NCUnitBase();
	~NCUnitBase();
	shared_ptr<NCUnitCommonData> pCommonData;
	NCUnitCommonData& GetCommonDataV() { return *pCommonData; }
	NCUnitState StartState;
protected:
	static CString ExprCharSet;// = _T("0123456789.-+*/^$'") + NEvalExp::OpenBC() + NEvalExp::CloseBC(); // Character set for the expression without logical operations
	static CString LogCharSet;// = _T("<>=!?&|:"); // Character set with comparison and logical operations
	static CString Sign;// = _T("-+*/^$'") + NEvalExp::OpenBC();// Arithmetics operations
	static CString CharSet;// = ExprCharSet + FUNC_NAMES;
	static CNCMDoc *pDoc;

	NCUnitState State;
	NCUnitState AddState;// Additional state is added for second movement in the single cadr
	int Error;
	int ErrorMinor;
	CycleType HiddenCycle;
	APTCLOut * pAptOut;
	UWordDef WordDef;

public:
	set<int>& GetUsedUCSs() { return GetCommonDataV().UsedUCSs; }
	const CStringArray& GetSCycleDef() const { return GetCommonDataC().SCycleDef; }
	NCStatesHist& GetStates() { return GetCommonDataV().States; }
	const NCUnitCommonData& GetCommonDataC() const { return *pCommonData; }
	bool SetCurProg(const CString &str);
	int LookupSGeneral(const char *, CString &);
	void DoReplace(CString &text);
	BOOL GetOtherValue(const CString& key, CString& val) const;
	BOOL GetOtherRegExp(const CString& key, CString& patt, CString& repl) const;
	int GetRegIndexesValue(const CString &key);
	bool SetRegister(NCUnitState &State, const CString &key, double Val, int DefaultInd = 0);
	bool GetSinumerikStyle(void) { return GetCommonDataC().SinumericStyle;}
	bool GetAllowToolString(void) { return GetCommonDataC().AllowToolString;}
	bool GetCheckProgramText() const { return GetCommonDataC().CheckProgramText; }
	CString GetConfPath() const;
protected:
	CString GetWord(CString &text);
	CString GetMultiNum(CString &text);
	int CrDrillCycle(NCycle *pCycle);
	bool GetIValue(CString &text, int *val) const;
	bool GetLValue(CString &text, int *val) const;
	bool GetFullLValue(CString &text, int *val) const;
	static bool GetHashNum(const CString &text, int *val);
	bool GetTValue(CString &text, CString *val);
	bool FindNextCadr(CString &text, cadrID *res, bool FindByLineNumber);
	bool FindCallFile(CString &text, cadrID *res);
	bool GetLabel(CString &text, CString &Label) const;
	BOOL CycleNeedsGeom() const;
	void MakeGotoList(BProgram *pProg);
	bool ProcBaltSysStyle(CString &text) const;
	bool ProcSinumericStyle(CString& text) const;
	bool ProcSinumRepeat(CString& text) const;
	CString GetGCodeText(const CString& GName) const;
	CString GetLProgCopyStart() const;
	CString GetLProgCopyEnd() const;
	int FindCadrInProg(BProgram *pProg, int num, bool FindByLineNumber);
	bool SetCurCadr(cadrID ID);
	int GetToolNumByName(const CString &Name) const;
	int MakeNewToolByName(const CString &Name) const;
	void RemoveComment(CString & text) const;
	class NChannel& GetChannel();
	const class NChannel& GetChannel() const;
protected:
	static bool isdigit( char c );
	static bool isletter( char c );
	static void RemoveLeadBlanks(CString &text);
	void GetConfErrMessage(int ErrNum, const std::vector<CString>& ErrParams, CString& ErrMessage);
};
class NCUnit : public NCUnitBase  
{
public:
	NCUnit(CNCMDoc* pD, int InChannelInd);
	NCUnit(CNCMDoc* pD, const NCUnit& In);
	virtual ~NCUnit();
	void SetUCSAutoNames();
	int AskStartState(int StartPage, int CurZeroPos);
	CString InvertSign(const CString &str);
	int CrGrooveCycle(NCycle *pCycle);
	int CrCuttingCycle(NCycle *pCycle);
	int CrCircleCycle(NCycle *pCycle);
	int CrTappingCycle(NCycle *pCycle);
	int CrRectCycle(NCycle *pCycle);
	int CrRectangCycle(NCycle *pCycle);
	int CrTurnThreadCycle(NCycle *pCycle);
	int CrTurnThreadACycle(NCycle *pCycle);
	int CrTurnThreadCCycle(NCycle *pCycle);
	int CrTurnFaceCutCycle(NCycle *pCycle);
	int CrTurnCutCycle(NCycle *pCycle);
	int CrApprCycle(NCycle *pCycle);
	int CrRetrCycle(NCycle *pCycle);
	int CrTurnRoughCycle(NCycle *pCycle);
	int CrTurnLoopCycle(NCycle *pCycle);
	int CrTurnClearCycle(NCycle *pCycle);
	int CrTurnGroovCycle(NCycle *pCycle);
	int CrTurnThreadCutCycle(NCycle *pCycle);
	int CrTurnChamfCycle(NCycle *pCycle);
	int CrTurnRoundCycle(NCycle *pCycle);
	int CrTurnCutRoughCycle(NCycle *pCycle);
	const CString &GetConfFileName();
	void AptExport(const CString &ProgName);
	BOOL GetParamValue(const CString &name, double* value, bool &Integer);
	BOOL GetRegValue(const CString& name, double* value);
	BOOL GetRegTypedValue(const CString& name, double* value, bool &Integer);
	bool IsRegInteger(int RegNum) const;
	BOOL GetRegValue( int Num, double* value );
	NProgram * MakeMainInitProg(const CString &text);
	double GetMinArcDist();
	double CalcMinArcDist();
	double GetRotaryScale();
	BOOL GetFileExt(CString &ext);
	bool JumpForTo(cadrID ID);
	void JumpToHCadrRel(const BProgram *pProg, int HCadr, bool AbsHCadr = false);
	BOOL GetMiscFuncValue(const CString& key, CString & val);
	BOOL GetFuncNum(const CString& key, CString & val);
	BOOL GetWordValue(const CString &key, CString & val) const;
	bool Defined();
	double CalcDist2(double x0,double y0,double z0,double x1,double y1,double z1,double x,double y,double z);
	cadrID PrepRunToCurCadr(void);
	int SetDef(const CString& name);
	int SetDefWOInitStr(const CString& name);
	RunStepsStopReason RunSteps(class NEvalStopCrit& n);
	bool IsCurCadrProtected() const;
	void JumpSteps(int n);
	void JumpForToBreakpoint();
	void JumpToProjStart();
	void JumpToProjEnd();
	void Serialize(CArchive &ar);
	//int SetDef();
	NCUnitStatePrev::EvaluateCadrResult EvaluateCurrent(bool Silent);
	void ClearConf();
	void Reset(bool NeedInitProg = true);
	NCUnitState * GetState();
	const NCUnitState * GetState() const;
	NCUnitStatePrev* GetSt();
	const NCUnitStatePrev* GetSt() const;
	NCUnitStatePrevGeom * GetLastGeomState();
	int RunProgram(BProgram *pProg, BProgram *pParentProg4Cycles = nullptr, int WholeProgSize = -1, bool ResetMatrs = false);
	bool Check4InvalidChar(const CString& Text, char* pInvalidChar) const;
	void ProcErrors(NCadr* pCurCadr, BProgram* pActualProg);
	NCUnitStatePrev::EvaluateCadrResult RunProgramStep(BProgram *pProg, BProgram *pActualProg, NCadr *pCurCadr, bool Inactive, bool ResetMatrs);
	int RunProgramProbePart(BProgram *pProg, BProgram *pParentProg4Cycles = nullptr, int WholeProgSize = -1);
	bool ProcRetErrorCode(NCUnitStatePrev::EvaluateCadrResult retCode);
	bool ErrorNeedBreak(NCUnitStatePrev::EvaluateCadrResult retCode);
	void CheckProgramText(BProgram *pProg);
	int VerifyProgramText(BProgram *pProg, NProgError& LocError);
	int MakeSplineA(const std::vector<int> &Indexes, BGeomArray &GeomArray);
	//int LoadConfFileWR();
	void FillToolAxes(BProgram *pProg) const;
	BOOL ProcessIF(CString& text, BOOL IfNot);
	BOOL ProcessCondition(CString& text, bool& Res);
	bool GetDValue4Dial(CString & text, double * val);
	bool GetDValue4Wide(CString & text, double * val);
	CNCMDoc* GetDoc() const { return pDoc; }
	const NCheckPoint* GetCheckPoint(int i) const { return GetCommonDataC().SyncHistory.size() <= i ? nullptr : &GetCommonDataC().SyncHistory.at(i); }
	int GetCheckPointCount() const { return int(GetCommonDataC().SyncHistory.size()); }
	void ClearSyncHistory() { GetCommonDataV().SyncHistory.clear(); }
	int MakeProgGroupList(BProgram* pProg, int ProgInd, const CString& ProgName);
	void AddGroupErrors2Prog(BProgram* pProg, int ProgInd) const;
	ProgGroupMarks& GetProgGroupList() { return ProgGroupList; }
	void LoadSubprograms();

protected:
	NCUnit& operator= (const NCUnit& In);
	NCUnit(const NCUnit& In);
	BOOL DoGoTo(CString& text, enum SearchDir SDir);
	BOOL DoGoToGroupEnd(cadrID CurCadr);
	BOOL DoGoToGroupChainEnd(cadrID CurCadr);
	cadrID findMark(int num, enum SearchDir SDir);
	bool CheckGotoPair(cadrID Src, cadrID Dest);
	BOOL GetMiscKey(CString &text, CString &key);
	bool FindEndCadr(cadrID *res);
	BOOL GoToSubEndNext(void);
	bool PrepUnitSerialiseAll();
	int LoadConfFile(std::vector<CString>& ErrParams, bool LoadFromSerializeAll);
	int ApplyMainInirStr();
	int LoadConfInh(std::vector<CString>& ErrParams, bool LoadFromSerializeAll);
	int CombineUnitFiles(const CString& OutName, const std::vector<CString>& Names, std::vector<CString>& ErrParams) const;
	int ReadSectionTitle(const CString& Start, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams) const;
	int ReadStandardSect(const CString& Start, const CString& End, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams, std::vector<CString>& Result) const;
	int ReadSimpleSect(const CString& Start, const CString& End, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams, std::vector<CString>& Result) const;
	int ReadReplaceSect(const CString& Start, const CString& End, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams, std::vector<CString>& Result) const;
	static int WriteSect(CStdioFile& OutFile, const CString& Start, const CString& End, const std::vector<CString>& Result);
	static int WriteStartStrings(CStdioFile& OutFile);
	int ReadSGeneral(CArchive*& pA, std::vector<CString>& ErrParams);
	int RunInitProg(const CString &Str);
	double ACCWVal(double Start, double End);
	double ACWVal(double Start, double End);
	double Fit360(double val);
	double RecalcOneRotCoord(double Start, double End);
	int RecalcRotCoords(const NCUnitStatePrev & StartSt, NCUnitStatePrev & EndSt, BPoint & EndABC);
	BOOL ProcessTechTable(CString &text);
	BOOL FindCopy(CString& text, cadrID* res, int* end, enum SearchDir SDir);
	BOOL FindCopy1L(CString& text, cadrID* res, enum SearchDir SDir);
	BOOL FindCopy(int num, cadrID *res);
	BOOL ReturnFromSub(BOOL cont);
	BOOL ProcessForStartSinum(CString& text);
	BOOL ProcessWhileStartSinum(CString& text);
	BOOL ProcessRepeatSinum();
	BOOL ProcessUntilSinum(CString& text);
	BOOL ProcessLoopStartSinum(CString& text);
	BOOL ProcessIfSinum(CString& text);
	BOOL ProcessElseSinum(CString& text);
	BOOL ProcessElseIfSinum(CString& text);
	BOOL ProcessCycleEndSinum();
	BOOL ProcessAssignmentStatement(CString& text, bool Round);
	double GetMaxWorkFeed();
	CString GetStartProgram();
	double GetScale(const CString &);
	double GetLinearDecimalZero();
	void ProcLengthCompOffset(int n);
	void ProcDiaCompOffset(int n);
	void ProcLengthComp4Cycle();
	int CreateRArcXY(double x1, double y1, double z1,
		double x2, double y2, double z2, double zst, NCadrGeom *pGeom);
	int CreateRArc(const NCUnitStatePrev &St, NCadrGeom *pGeom);
	double GetSpeedScale();
	double GetFeedScale();
	BOOL ProcessDwell(CString &text);
	BOOL ProcessRegister(CString &text, bool Round = false);
	int ProcessRegNum(CString &text);
	double DecodeFeed(double val);
	int FindStartCadr();
	BOOL ProcessTiltPlane(CString& text, bool AnglesCase);
	BOOL ProcessCreateMTool(CString &text);

protected:
	BGeomArray CycleGeom;
	BGeomArray CycleGeomComp;
	CMapStringToString UWordDefInv;
	NCUnitStatePrev St;
	NCUnitStatePrevGeom LastGeomState;
	NTiltPlane TiltPlane;
	SCoordsDef FixedDef;
	SCoordsDef AddCoordsDef;
	SMachState AddCoordsScale;
	SMachState AddCoordsFeed;
	enum Plane StoredPlane;
	int CurStockPosition;
	NSyncCheckPoints CheckPoints;
	ProgGroupMarks ProgGroupList;
public:
	NProgError TempError;// new fashion processing (multiple errors)
	NAutoToolGenParams AutoToolGenParams;
protected:
	void SetStateFromCompGeom();
	int CrMillRectCycle(NCycle *pCycle);
	int CrMillRectInCycle(NCycle *pCycle);
	int CrMillRectOutCycle(NCycle *pCycle);
	int CrMillCircCycle(NCycle *pCycle);
	int CrMillCircInCycle(NCycle *pCycle);
	int CrMillCircOutCycle(NCycle *pCycle);
	int CrMillContCycle(NCycle *pCycle);
	int CrMillDrillPeckCycle(NCycle *pCycle);
	int CrThreadingTapCycle(NCycle *pCycle, enum CycleType CType);
//	int CrLasCycle(NCycle *pCycle);
	int CrMP81Cycle(NCycle *pCycle);
	int CrHMCycle(NCycle *pCycle);
	int CrHMCCycle(NCycle *pCycle);
	int CrHNCycle(NCycle *pCycle);
	int CrNumerCycle(NCycle *pCycle);
	int CrSplineACycle(NCycle *pCycle);
	int CalcComp(const NCUnitStatePrev& St, bool NextGeomFlag, const NCadrGeom &NextGeom);
	int ProcessLengthComp();
	int ProcessDiamComp(NCadrGeom& NextGeom);
	int ProcessDisplComp();
	void PrepNewRotMatr(const BPoint& ABCE);
	double GetLengthComp() const;
	bool GetDValue(CString& text, double* val);
	bool GetDValue(CString& text, double* val, double DotScale);
	bool GetDValue(CString &text, double *val, const CString &ScaleName);
	bool GetDIValue(CString &text, double *val);
	BOOL ProcessCycleParam(CString &text,CString word);
	BOOL ProcessCycle(class NCadr *pCadr, bool ResetMatrs);
	BOOL ProcessModalCall(bool ResetMatrs);
	BOOL ProcessCoord(CString &text, const CString &word, int ChangedMode = 0);
	BOOL ProcessVariable(CString &text, bool LocalVar);
	BOOL ReplaceVariables(CString &text);
	BOOL ProcessCoordRot(CString &text,const CString &word);
	BOOL ProcessText(CString &text);
	BOOL ProcessMiscFunction(CString &text);
	BOOL ProcessFunction(CString &text);
	void ProcessTouch(bool ResetMatrs);
	BOOL CheckPolar();
	void Init();
	CString GetField(CString &str) const;
	NCUnitStatePrev::EvaluateCadrResult EvaluateCadrBase(class NCadr*, const NCadr* pNextCadr = NULL, bool ResetMatrs = false);
	NCUnitStatePrev::EvaluateCadrResult EvaluateCadr(class NCadr*, const NCadr* pNextCadr = NULL, bool ResetMatrs = false);
	int CreatePArc(const NCUnitStatePrev &St, NCadrGeom *pGeom);
	BOOL ProcessToolCode(CString &text);
	void CompOtrOtr(const NCadrGeom &NextGeom);
	bool CompOtrArc(const NCadrGeom &NextGeom, BOOL OtrFirst);
	void CompArcArc(const NCadrGeom &NextGeom);
	bool GetNextGeom(NCadrGeom &NextGeom);
	bool GetNextDir(BPoint &NextDir);
	void Copy(const NCUnit &In);
	BOOL ChangedX(const NCUnitStatePrev &St);
	BOOL ChangedY(const NCUnitStatePrev &St);
	BOOL ChangedZ(const NCUnitStatePrev &St);
	BOOL ChangedIJK(const NCUnitStatePrev &St);
	void CompToInters(const NCadrGeom & NextGeom, const NCUnitStatePrev & St);
	void AddExtraArc(const NCUnitStatePrevGeom & St);
	BOOL ProcessScalingOn(CString & text, double Korr);
	BOOL GetOneScaleValue(CString & text, const CString & axis, double Korr, double & Val);
	BOOL MakeCycleGeom(CString & text, CString &key);
	BOOL MakeSubrGeom(CString & text, BGeomArray &Cont);
	BOOL ProcCancelCycle(void);
	int InitExecCadr(const NCUnitStatePrev & St);
	int InitNonExecCadr(const NCUnitStatePrev & St);
	void ProcContextFeed(void);
	void Coord2Regs(void);
	void ProcessRCP(void);
	void ProcessRCPCancel(void) noexcept;
	bool GetNextState(NCUnitState &NextState);
public:
	int GetCurStockPosition() const noexcept { return CurStockPosition; }
	void SetCurStockPosition(int Val) noexcept { CurStockPosition = Val; }

	const SCoordsDef& GetFixedDef() const noexcept { return FixedDef; }
	const SCoordsDef& GetAddCoordsDef() const noexcept { return AddCoordsDef; }
	BOOL ChangedIJKR(void) const;
	BOOL ProcessSubrCallEnd(CString & text);
	BOOL ProcessSubrCall(CString & text, bool CallByLineNumber);
	BOOL ProcessFileCall(CString & text);
	BOOL ProcessVariablesDecl(CString & text, bool LocalVar);
	BOOL ProcessDefinesDecl(CString & text);
	BOOL ProcessReadMTSpecPos(CString & text);
	BOOL ProcessReadMTSpecPos4Tool(CString & text);
	BOOL StoreStockStdParams(CString & text);
	BOOL StoreAddCoord(CString & text);
	BOOL ProcSyncCheckPoint(const CString& key, CString& text);
	BOOL SetMachState2Regs(const SMachState &MachState);
	BOOL ProcessLoopCycle(CString & text);
	BOOL ProcessRegSubst(CString & text, int Base);
	BOOL ProcessReposStock(CString& text);
	BOOL ProcessReposStockByGr(CString& text);
	BOOL DefineRect(CString & text);
	BOOL DefineRectQ(CString & text);
	void SubstCycleParam(void);
	NProgram * GetCurRunProg(void);
	NProgram * GetCurLoaProg(void);
	int GetErrStringNum(void);

	bool JumpForToHCadr(cadrID CadrHID, bool AbsHCadr = false);
	bool JumpToNextProg(void);
	bool JumpToPrevBreakpoint(void);
	int GetCallStackSize(void);
	BOOL ProcessCoordComp(const NCUnitStatePrev & St);
	int RunCycle(class NCadr *pCadr, bool ResetMatrs);
	int ProcRoughDParam(CString & text);
	int ProcThreadRParam(CString & text);
	int ProcLoopDParam(CString & text);
	const BMatr CalcMRCS(void);
	bool ProcessCurAngle(void);
	int MakeOrtChamfer(void);
	int MakeChamfer(void);
	void TurnCutCompProc(void);
	void ClearCompArrays(void);
protected:
	static bool CheckNext;
	static bool CyclicA;
	static bool CyclicB;
	static bool CyclicC;
	static bool FileProtection;
	static bool AddZDispToComp;
	static bool SaveCoord2Reg;
	static bool PartialCompCancel;
	static bool ConsecutiveCalc;
	static bool UseIntegerArithm;
	static regex VarTestExp;
	static regex CharTestExp;
public:
	bool SetVal2Reg(CString & text, int RegNum);
	bool GetIParamVal(CString & text, const CString & PName, int * pVal);
	bool GetLParamVal(CString & text, const CString & PName, int * pVal);
	bool GetDParamVal(CString& text, const CString& PName, double* pVal);
	bool GetDIParamVal(CString& text, const CString& PName, double* pVal);
	bool GetDParamValSoft(CString& text, const CString& PName, double* pVal);
	bool GetDIParamValSoft(CString& text, const CString& PName, double* pVal);
	bool GetTParamVal(CString & text, const CString & PName, CString & Val);
	bool GetTextParamVal(CString & text, const CString & PName, CString & Val);
	bool ApprCycleActive(void) const;
	bool ProcessConfChange(CString& text, bool ResetMatrs);
	int ProcessCurCurvMod(void);
	bool StoreActiveToolStdParams(void);
	bool SetInterpChanged(void);
	bool SetPolarInterp(bool On);
	bool SetCylInterp(CString &text);
	int MakeCurve4CylInt(NCadrGeom & Geom);
	void SetFlags(void);
	CompType GetCompType(void);
	bool SetLimit(int Num, TravelLimit TL, CString &text);
	bool CheckTravelLimits(void);
	//bool CalcWeldGeom(void);
	bool SetCurCycle(const CString &CycleDef, CycleType Type, CString &text);
	bool ProcParenthesesList(const CString &CycleDef, CString &text);
	bool IsAddCoordWord(CString &CurWord) const;
	bool ProcessAddCoord(CString &text, CString &CurWord);
	bool ProcessRCPOn(CString& text);
	void State2StartState();
//	void FillWeldCadrs(class NProgramWeld *pProgWeld, BGeomArray *pGeomArray);
	void JumpHere(void);
	bool ProcessPlaceTool(void);
	bool PlaceTool(const BPoint &P, const BPoint &N);
	int PlaceTool(const BMatr &CS, int Solution);
	bool CalcPlaceTool(SMachState &Result, const BPoint &ToolPos, const BPoint &ToolDir, RCoordPair Axes,
		int Decision = 1, bool PreferRot = true, int Type = 0);
	int CalcPlaceTool(std::vector<SMachState*> &Result, const BMatr &CS);
	const CString MakePlaceString(const CString &text);
	const CString MakePointString(const BPoint &P, const BPoint &N, const CString &text);
	void SetStartState(const NCUnitState &State) { StartState = State;}

	void AddStartMatr(bool FirstRegen, int StPos);
	const CString MakeCadr(const SMachState &MState) const;
	NCUnitState &GetStartState(void) { return StartState;}
	int SetSpindleSpeed(CString &text, int SpinNum);
	int JumpOver(void);
	bool Jump2State(const NCUnitState &TargetState, const NCUnitState &TargetStateAdd, const NCUnitStatePrevGeom &TargetStateGeom);
	static bool IsFileProtectionEnabled(void) { return FileProtection; }
	static void SetFileProtection(bool Val) { FileProtection = Val; }
	static double GetThreadPitch(double ThreadNum);
	void ConfFileErrMess();
	bool IsArcCenterAbs(void) const;
	void StoreCurrent2UCS(int UCS);
	void GetUcsToolPos(int i, BPoint &P, BPoint &N);
	bool GetUcsToolMatr(int i, BMatr &M);
	bool RecalcUcs(int i, const BMatr &M);
	bool RecalcAllUcss(const BMatr &M);
	void FillUnitFile(void);
	void SetTempUnit(void);
	static void MakeTempName(const CString& FileName, CString& TempName, bool Restore);
	static bool IsTempName(const CString& FileName);
	void ApplyRCP(bool Revert);
	class NToolCombined *GetActiveTool(void) const;
	char GetRegSymbol() { return GetCommonDataC().RegisterSymbol; }
	void FillRegChanges(BProgram::ProbePos::NRegChanges &RegChanges);
	void ProcAutoFix(NCadrGeom* pGeom);
	int GetChannelInd() const noexcept { return pCommonData->ChannelInd; }
	void ChangeAddCoords(const class CMapStringToNProgram& ProgList, cadrID StartPosition, const SMachState& AddCoord);
	enum NCMApplicationType GetToolClass() const;
	bool IsToolTurn() const;
	bool IsToolMill() const;
	bool StoreError(int ErrCode) noexcept;
	bool ProgCopyLC();
	enum SearchDir GetSDir(const CString& OtherParam, bool ForREPEATB);
	BStockToolInds GetCurSTInds();
	int GetCadrNum(const CString& str, CString& NumSym) const;
	int GetStockPos4Turn() const;
	const std::set<int>& GetMyStockPos() const;
	void AddCheckPointPos(BProgram* pProg, NSyncCheckPoints& CheckPoints);
	int MakeLengthComp(const NToolCombined& CurTool, const NCadrGeom& Geom, NCadrGeom& LengthCompGeom);
	BMatr CalcMCS(const NCadrGeom& Geom, const NToolCombined* pTool) const;
	void RemoveCSMatr();
	void EnableCSMatr();
	int OpenConfFile(CArchive*& f, std::vector<CString>& ErrParams);
	void CloseConfFile(CArchive*& f) const;
	void Make3DArcCycle();
};

