// NCMDoc.h : interface of the CNCMDoc class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AFXTEMPL.h"
#include <thread>
#include "Snaps.h"
#include "BOpenGLWnd.h"
#include "ppltasks.h"
#include "NStockStl.h"
#include "NStock.h"
#include "NStockBox.h"
#include "NStockCyl.h"
#include "NMapStringToNTool.h"
#include "CMapStringToNProgram.h"
#include "CMapStringToNStock.h"
#include "BBox.h"
#include "NCadrGeom.h"
#include "NCMData.h"
#include "NRepReportsArray.h"
#include "QMeasure.h"
#include "QComparison.h"
#include "QDivide.h"
#include "MTEdit.h"
#include "NCollManagerOrd.h"
#include "NMileStones.h"
#include "NAnalyseState.h"
#include "NSectionSetDyn.h"
#include "NProgWeldManager.h"
#include "NToolCombined.h"
#include "NPosTool.h"
#include "DDimensions.h"
#include "DDistance.h"
#include "RPickID.h"
#include "CStocksCollection.h"
#include "MTMashineTool.h"
#include "NChannel.h"
#include "BImageRGB.h"
#include "BProgErrHandler.h"
#include "BProgram.h"
#include "BUndoHistory.h"

constexpr int DynamicLavelDef = 2;


class NToolCombined;
class NTool;
class NToolID;
class NProgram;
class NStockCrBoxDial;
class NStockCrCylDial;
class NStockCrStlDial;
class NStockCrPipDial;
class NStockCrContDial;
class NStockCrContMDial;
class NCollisionScene;
class NStockSection;
class NCUnit;


class CNCMDoc : public CDocument, public PAbstractObject
{
	friend class NSetupSetBoxPointDlg;
protected: // create from serialization only
	CNCMDoc();
	DECLARE_DYNCREATE(CNCMDoc)
protected:
	std::vector<NChannel*> Channels;
	NChannel* pCurChannel;
	BImageRGB ImageRGB;
public:
	DDistance Distance;
	DDimensions Dimensions;
	NMileStones MileStones;
	NMileStones CheckStones;
	QMeasure Measure;
	QDivideBase* pDivide;
	QComparison Comparison;
	MTEdit MachToolEditor;

	NStockCrCylDial* pCreateCylDlg;
	NStockCrBoxDial* pCreateBoxDlg;
	NStockCrStlDial* pCreateStlDlg;
	NStockCrPipDial* pCreatePipDlg;
	NStockCrContDial* pCreateContDlg;
	NStockCrContMDial* pCreateContMDlg;
	int DynamicLavel;
	NCadrGeom SelectedPrev;
	NCadrGeom SelectedNext;
	BPoint CadrToolAxis;
	NRepReportsArray* pRepArray;
	cadrID StatusCadr;
	cadrID StatusCadrAbs;
	ULONGLONG StatusCadrAbsTime;
	bool CalcVolume;	// Calculate volume	
	bool CalcTime;		// Calculate time	
	bool CalcGeom;		// Calculate geometric parameters	
	bool AutoUpdateProg;
	double StatusX;// Coordinates to display in status bar
	double StatusY;
	double StatusZ;
	bool FastCut5xEnabled;
	bool CutEnabled;
	bool SaveEnvironment;
	BProgErrHandler ProgErrHandler;
protected:
	MTMachineTool MachTool;
	NCMData* pExternData;
	NStockSection* pStockSection;
	NPosTool PosTool;
	NMapStringToNTool NToolList;
	CMapStringToNStock NExObjList;
	CMapStringToNStock NPartsList;
	CStocksCollection NStocksList;
	CMapStringToNStock NPosChecks;

	BUndoHistory UndoHistory;

	int BlockUpdateLevel;
	NCollManagerOrd CollManager;
	NAnalyseState AnalyseState;
	NSectionSetDyn ToolSectionDyn;
	NProgWeldManager ProgWeldManager;

	NToolCombined DummyTool;
	class GReRenderCadr* pSliderRRCadr; // For main slider
	class NDocClipboard* pClipboard;
	bool SilentOpen;
	bool LoadLayoutProject;
	class NMachUnitPair* pMUPair;
	bool DifferentBodyRotFlag;
	std::atomic<bool> Interrupt;
	std::thread StocksSaveThread;
	CString DFNFileName;// serialize in 2025
	Snaps SnapsGlob; // привязки
public:
	void DeleteStocks();
	void CreateStocksRender();
	void CreateModifStocksRender();
	void CreateStocksSectGeom();
	void CreatePartsSectGeom();
	void DrawTool(RenderMode GlobRMode, const NToolCombined * pTool);
	void DrawParts(RenderMode GlobRMode, bool DrawSurf);
	void DrawFixture(RenderMode GlobRMode, bool DrawSurf);
	void SetClipPlane();
	bool GetSectionActive();
	void RemoveClipPlane();
	void AddExobjWU(class NStock* ppart);
	void AddPartWU(class NStock* ppart);
	void AddStock(class NStock* pstock, INT_PTR Index = -1);
	void ReplaceStock(INT_PTR Index, class NStock* pstock);
	void AddStockWU(class NStock* pstock, const BMatr& matr, INT_PTR Index = -1);
	void DeleteStockWU(class NStock* pstock);
	BBox Gabar;
	class NCUnit * GetUnit() const;
	CMapStringToNProgram * NGetProgramList();
	const CMapStringToNProgram * NGetProgramList() const;
	NMapStringToNTool * NGetToolList();
	const NMapStringToNTool * NGetToolList() const;
	CMapStringToNStock * NGetExObjList();
	const CMapStringToNStock * NGetExObjList() const;
	CMapStringToNStock * NGetPosChecks();
	const CMapStringToNStock * NGetPosChecks() const;
	CMapStringToNStock * NGetPartList();
	const CMapStringToNStock * NGetPartList() const;
	CStocksCollection * NGetStockList();
	const CStocksCollection * NGetStockList() const;
	void NAddMainProgram(const CString &name,NProgram *pProg);
	void NAddAuxProgram(const CString &name,NProgram *pProg);
	bool IsStockPosChecked();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCMDoc)
	public:
	void Serialize(CArchive& ar) override;
	void DeleteContents() override;
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

public:
	NPosTool &GetPosTool(void) { return PosTool;}
	bool IsUpdateBlocked() const { return BlockUpdateLevel != 0; };
	int GetBlockUpdateLevel() const { return BlockUpdateLevel; };
	void SetBlockUpdateLevel(int Val) { BlockUpdateLevel = Val; }
	static void MarkErrCadr(const NChannel* pChannel, cadrID Cadr);
	bool JumpToStatusCadrAbs();
	void FillToolsInfo();
	void GotoStatusCadr();
	void GotoCadr(const NChannel* pChannel, cadrID CadrID);
	void GotoCadrFast(const NChannel* pChannel, cadrID CadrID);
	void GotoAbsCadr(const NChannel* pChannel, cadrID CadrIDabs);
	void Jump2AbsCadr(cadrID CadrIDabs, bool NeedMessage);
	BBox GetProgCyl();
	BOOL IsThereAnyVisTool();
	//BOOL IsThereAnyVisProgr();
	BOOL IsThereAnyTool() const;
	BOOL IsThereAnyProgr() const;
	BOOL IsThereAnyErrProgr();
	BOOL IsThereAnyExObj() const;
	BOOL IsThereAnyPart() const;
	BOOL IsThereAnyStock() const;
	BOOL IsThereAnyStockImpExpDX() const;
	BOOL IsThereMoreThanOneStock() const;
	BOOL IsThereAnyTransluc();
	BBox GetProgDim();
	class NStock* GetStock(int index);
	class NStock* GetStockUnsafe(int index) const;
	class NStock* GetFirstVisStock() const;
	bool ReloadAllProg();
	void SetStockColor(COLORREF inColor, int index);
	COLORREF GetStockColor(int index);
	void SetWFrameToAllStocks(BOOL value);
	void SetTransparentToAllStocks(BOOL value);
	void TransformAllStocks(const BMatr &matr);
	PackedColor GetStockColor4(int index);
	PackedColor GetStocksColor();
	void DeleteProg(const CString& prog);
	static void DeleteProgObj(NProgram* pProg);
	NProgram* LoadNewProgram(const CString& FileName);
	bool ReloadProgram(NProgram* pProg, const CString& FileName);
	static void ReleaseSciDoc(NProgram* pProg);
	virtual ~CNCMDoc();
	void ReadJobFile(LPCTSTR lpszFileName);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void FillMUPair(void);


protected:
	void ReadJobFile(CArchive &ar);
public:
	void SetSilent(bool Val) { SilentOpen = Val;}
	bool IsSilent(void) { return SilentOpen;}
	void ResetStocks();
	void OnMtLoad();
	NStockSection * GetStockSection(void);
	NStockSection * CreateStockSection(CWnd *pParent);
	void DrawStockWSect(RenderMode GlobRMode, bool DrawSurf);
	COLORREF GetToolColor(int n, int t);
	bool IsAnaliseActive(void);
	void ImplReplAll(NChannel* pCh = nullptr);
	void RegeneratePrograms(void);
	bool RegeneratePrograms1Time();
	void RegenerateProgramsProbePart(NChannel *pChannel);
	void RegenerateTools(void);
	void RegenerateStock(int index);
	void RegenerateStocks();
	void RegenerateParts();
	void RegenerateFixtures();
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	class NToolCombined * GetTool(const NToolID & ToolID);
	int GetAttachNum(const CString & buf);
	void PrepProgRenders(double Eps);
	void DeleteList(CMapStringToNStock &List);
	void DeletePartList();
	void DeleteExobjList();
	static bool ExtractFileName(CString &Path);
	bool IsThereAnyVisExObj(void) const;
	bool IsThereAnyVisPart(void) const;
	bool IsThereAnyVisStock(void) const;
	void FillExternData(NCMData *pExternData);
	void FillToolsStr(CString &Tools);
	void UpdateAllViews(CWnd* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);
	int FullCollisionDetection(bool EnableLog = true);
	NCollManagerOrd &GetCollManager(void);
	const NCollManagerOrd &GetCollManager(void) const;
	void ProgramChangeEnded(void);
	virtual void Regenerate(void);
protected:
	void PrepareNewData();
public:
	int RunNSteps(int N);
	RunStepsStopReason RunSteps(int N);
	NToolCombined *GetCurTool(void);
	int GetPartsPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N, RPickID& PickID);
	int GetStaticStockPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N);
	int GetRayPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N);
	int GetMachPoint(const BPoint &P, const BPoint &V, BPoint &R, BPoint &N, RPickID& PickID);
    size_t FindSegmRopes(int ObjectType, bool StockView, SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr);
	NAnalyseState &GetAnalyseState(void);
	bool InitToolSection();
	NSectionSetDyn &GetToolSection();
	void DrawToolSection() const;
	bool InitProgWeldManager();
	static NProgram *NewProgram(void);
	int UpdateProgWeldManager();
	int UpdateProgWeldTraject();
	int UpdateProgWeldSections();
	cadrID GetCurCadr(double &t) const;
	cadrID GetCurCadrAbs(void);
	const NCadrGeom* GetAbsCadrGeom(cadrID AbsID, int StockPos);
	const NCadrGeom* GetAbsCadrCompGeom(cadrID AbsID, int StockPos);
	const NCadrGeom* GetHCadrCompGeom(cadrID HID, int StockPos);
	int GetGlobPos() const;
	int GetGlobPos(cadrID CadrID) const;
	cadrID GetIDByGlobPos(int GlobPos, int DeltaPos = 0) const;
	int FindUnProtGlobPos(int GlobPos, bool Next = false);
	int GetToolsPosCol(int MaxNum, NColor *Colors, int *Positions);
	const NToolCombined *GetActiveTool(void);
	bool IsToolActiveStatic(const NToolCombined* pComTool) const;
	bool GetFullToolMatr(const NToolCombined* pComTool, bool StockView, BMatr& ToolMatr);
	void StopSlider(int GlobPos);
	void SetSliderPos(int GlobPos);
	BProgram *GetProgram(cadrID CadrID);
	BProgram *GetProgram(const CString &ProgName);
	BProgram *GetProgram(int ProgNum);
	void PrepReadJob();
	NToolCombined * MakeNewTool(const NToolID &ToolID, const CString &Name);
	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
	void CopyTools2Clipboard(const CStringArray &CheckedTools);
	void PrepClipboard(void);
	void PasteToolsFrClipboard(void);
	void DeleteUnUsedTools(void);
	void JumpToNextTool();
	void JumpToPrevTool();
	cadrID FindNextToolHID(bool *pProjEnd) const;
	cadrID FindPrevToolHID() const;
	void ReportSaveLoadException(LPCTSTR lpszPathName,
				CException* e, BOOL bSaving, UINT nIDPDefault);
	BBox GetStocksGabar();
	BBox GetExObjGabar(void);
	bool IsFastCut5xEnabled(void) const { return FastCut5xEnabled;}
	bool IsCutEnabled(void) const { return CutEnabled;}
	void ResetCutting(void);
	const BBox GetModifStockGabar(int index);
	const BBox GetModifStocksGabar();
	void StoreCurrent2UCS(int UCS);
	void GetToolMatr(BMatr &M);
	bool SetBoxPointApply(const BPoint &P, const BPoint &N);
	int SetUCSOnBoxApply(const BMatr &CS, int Solution);
	void SetBoxPointEnd();
	void JumpToProjStart(void);
	void LoadSingleStock(CArchive& ar);
	void SerializeStocks(CArchive& ar);
	void SerializeTraj(CArchive& ar);
	void FillEnvironment(void);
	void FillStls(BMapStringToNStock &Map, CStringArray &StringArray);
	void FillStocks(void);
	CString MakeInputFileName(const CString& ShortName);
	void FillTools(void);
	void FillDFNFile();
	const CStocksCollection &GetStocksList(void) const { return NStocksList; }
	CStocksCollection &GetStocksList(void) { return NStocksList; }
	void LoadMT(const CString &MTName);
	void LoadNCUnit(const CString &UName);
	void ClearAnimateQueues();
	void SaveProgText(const CString &filename, NProgram *pNProg) const;
	bool SaveProgText(const CString &ProgName, const CString &NewName);
	void SaveAllProgramsText();
	bool ShowProgrText(int ind);
	bool ShowProgrText(const CString &ProgName);
	INT_PTR CalcSubObjects() const;
	bool CheckProgExist();
	bool IsCadrVisible(cadrID Cadr) const;
	const NCadrGeom *GetCadrIfVisible(cadrID Cadr, bool CompCadr, int ChInd, int StPos) const;
	bool GetCadrIDStrings(cadrID IDabs, CString& Chan, CString& Prog, CString& Line, CString& Step);
	bool SetCurChannel(int i);
	void AddNewChannel();
	void DeleteChannel(int ChannelInd);
	bool SetNextChannel(int& Iter);
	NChannel* GetNextCuttingChannel(const NChannel& Chan) const;
	bool GetAutoUpdateProg() const { return AutoUpdateProg;	}
	void SetAutoUpdateProg(bool Val) { AutoUpdateProg = Val; }
	bool SaveAllStocks() const;
	static void WritePacked(CMemFile& SourceFile, CFile& DestFile);
	MTMachineTool& GetMachTool(int MTNum = -1);// -1 - base machine; 0... - single configuration
	const MTMachineTool& GetMachTool(void) const;
	bool LoadMachTool(const CString& path, const CString& name);
#ifdef NC_DEMO
	bool GetLoadLayoutProject() const noexcept { return LoadLayoutProject; }
#endif
#ifndef NC_DEMO
	bool GetLoadLayoutProject() const noexcept { return false; }
#endif
	void SetLoadLayoutProject(bool val) { LoadLayoutProject = val; }
	class NMachUnitPair* GetpMUPair() const { return pMUPair; }
	NChannel* GetCurChannel() const { return pCurChannel; }
	NChannel* GetChannel(int ind) const { return Channels.at(ind); }
	int GetChannelsCount() const { return int(Channels.size()); }
	bool SetProgrTDT(const CString& str);
	bool SetProgrTDT(int ProgInd);
	void SetCurProgrTDT();
	const CString& GetProgrTDT() const;
	void ImplRepl(const BProgram* pProg) const;
	bool RunAnimateStep(int& MaxRunStepsNum);
	void StartAnimWalker(bool UseSetHStart);
	void Reset();
	bool GetDifferentBodyRotFlag() const { return DifferentBodyRotFlag; }
	void SetDifferentBodyRotFlag(bool Val) { DifferentBodyRotFlag = Val; }
	void DrawAllStocks(RenderMode GlobRMode, int Type, bool DrawSurf);
	bool IsInterrupted(void) const;
	bool InterruptAnimation(void);
	void ResetInterrupt() { Interrupt = false;}
	void FinishAnimation(void);
	void ProcCuttingStopped(int ChanInd);
	bool EnableNextChannel(NChannel& CurChan);
	bool GetCutPermission(int ChanInd);
	void WaitAllChannels();
	bool IsMultiChannel() const { return Channels.size() > 1; }
	void Wait4Permission(NChannel& ActChannel, MainTime CurTime);
	void WaitCuttingEnabled(NChannel& ActChannel);
	void SetCuttingEnabled(NChannel& ActChannel, bool Set);
	void GetErrorsVector(NChannel::ErrVector& ErrorsVector);
	void StopAnimation(void);
	void JumpAfterStop(void);
	void LoadSettings();
	static bool CutFirstWord(CString& src, CString& dst);
	void ApplyAppearSettings(void);
	const CString& GetDFNFileName() const { return DFNFileName; }
	void SetDFNFileName(const CString& Val) { DFNFileName = Val; }
	BUndoHistory& GetUndoHistory() { return UndoHistory; }
	const BUndoHistory& GetUndoHistory() const { return UndoHistory; }
	bool RemoveTempStock(void);
	void AddPart(const CString& name, int& ind, class NStock* pStock);
	void AddFixture(const CString& name, int& ind, class NStock* pStock);
	void DeletePartWU(class NStock* pstock);
	void DeleteFixtWU(class NStock* pstock);
	void TransMovObjWU(NMovableObj* pS, const BMatr& M);

	bool UHUndoGroup();
	bool UHDoGroup();
	bool UHCanRedo() const;
	bool UHCanUndo() const;
	bool UHPrepUndoRedo();
	void GetTimeSlots(std::vector<TimeSlot>& TimeSlots) const;
	static bool NeedRunAnimStep(const std::vector<TimeSlot>& TimeSlots, const NChannel* pCh);
	void SetMesureDistanceActive(bool Set);
	bool IsMesureDistanceActive() const;
	bool IsNCMAll() const;
	Snaps& GetSnaps() { return SnapsGlob; }
	void SetModifiedFlag(BOOL bModified = TRUE);
	const NCadrGeom* GetCadrGeomIDost(IDost ID);
	auto& GetRayCastManager() { return SnapsGlob.GetRayCastManager(); }
	void ShowCollidedAnim(const NCollisionID* pPrev, bool Val);
	void ShowCollided(bool RunAnimateActive);
	void ClearAllCollisions();
	bool IsCollWOStock() const;
};
/////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

