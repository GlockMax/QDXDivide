#pragma once
#include "NCMB_API.h"
#include "GuDongleModule.h"
#include "NCommonPaths.h"
#include "RemoteCommands.h"
#include "ViewRules.h"
#include "NTiParams.h"
#include "BProgressPercent.h"
#include "BTimeLine.h"

const int MODULES_NUM = 16;
class NCMB_API NGlobalState
{
public:
	enum AnimModes
	{
		Anim_no,
		Animation,
		Result
	};

	enum EditModes
	{
		Edit_no,
		Edit_Mach,
		Edit_Unit,
		Edit_Extern
	};

	enum ModelModes
	{
		MM_EMPTY,
		BSP,
		DX
	};
	enum ViewActionInd
	{
		VA_ZOOM_ALL = 0,
		VA_PREP_PAN = 1,
		VA_PREP_ROT = 2,
		VA_PREP_ROTX = 3,
		VA_PREP_ROTY = 4,
		VA_PREP_ROTZ = 5,
		VA_PREP_SCALE = 6,
		VA_PREP_ZOOMW = 7,
		VA_DO_ZOOM = 8,
		VA_DO_EMPTY = 9,
		VA_SIZE = 10 // Must be 1 grater than maximum other index. Is used as array size.
	};

	NGlobalState(void);
	~NGlobalState(void);
	bool GetCuttingMode(void) const;
	bool GetAnimateMode(void) const;
	bool GetResultMode(void) const;
	void SetCuttingMode(bool in);
	void SetAnimateMode();
	void SetResultMode();
	void SetEditMachMode(bool in);
	EditModes GetEditMode(void) const;
	void SetMultiThreadDX(bool Val) { MultiThreadDX = Val; }
	void SetMultiThreadDX_And(bool Val) { MultiThreadDX &= Val; }
	bool GetMultiThreadDX(void) const { return MultiThreadDX; }
	ModelModes GetModelMode(void) const { return ModelMode; }
	void SetModelMode(ModelModes NewMode) { ModelMode = NewMode; }
	void Reset();
	AnimModes GetAnimMode(void) const;
	bool GetHostMode(void) const;
	void SetHostMode(bool Val);
	bool GetRibbonInterface() const { return RibbonInterface; }
	void SetRibbonInterface(bool Val) { RibbonInterface = Val; }
	void SetViewInterface(const CStringA& Style);
	const SubscribeStruct& GetSubStProgChange() const { return SubStProgChange; }
	void SetSubStProgChange(const SubscribeStruct& Val) { SubStProgChange = Val; }
	const SubscribeStruct& GetSubStColl() const { return SubStColl; }
	void SetSubStColl(const SubscribeStruct& Val) { SubStColl = Val; }
	bool ProcProgChangeSubscr(const CString& Name) const;
	bool ProcCollSubscr(const struct CollisionData& CollD) const;
	void AddViewRule(WORD Message, UINT nFlag, const VConditions& Conditions, int ViewActionInd, int StopMessage);
	std::pair<int, WORD> GetViewActionInd(WORD Message, UINT nFlags) const;
	void ClearViewRules() { VRules.Clear(); }
	void SetWheelReverse(BOOL Val) { WheelReverse = Val; }
	BOOL GetWheelReverse() { return WheelReverse; }

protected:
	bool CuttingMode;
	AnimModes AnimateMode;
	EditModes EditMode;
	ModelModes ModelMode;
	bool MultiThreadDX;
	bool HostMode;
	bool RibbonInterface;
	SubscribeStruct SubStProgChange;
	SubscribeStruct SubStColl;
	ViewRules VRules;
	BOOL WheelReverse;
};


#define NCM_PROJECT NCMProject::GetProject()

class NCMB_API NCMProject
{
public:
	NCMProject(void);
	~NCMProject(void);
protected:
	static NCMProject &ActiveProject;
	double InFileVersion;
	class ParticularExport *pPExport;
	class BObjectManager *pObjectManager;
	GuDongleModule Modules[MODULES_NUM];
	bool FloatingLicense;
	int PostUserID;
	NCommonPaths CommonPaths;
	NGlobalState GlobalState;
	bool UseQuadTreeParallel;
	BProgressPercent ProgressPercent;
	BTimeLine TimeLine;

public:
	typedef CMapStringToString OneChanConfs;
	CArray<OneChanConfs, const OneChanConfs&> UnitConfs;// since 5.31
	CString DFNFile;
	CStringArray StockFile;
	CStringArray ToolFile;
	CStringArray MachineFile;
	CString DefaultsFile;
	CStringArray PartsFile;
	CStringArray ExObjsFile;
	CString ErrConfFile;
	CMapStringToIntPtr IntParams;
	CMapStringToNColor Colors;
	CMapStringToDoublePtr DoubleParams;
	NTiParams& Defaults;
	// Translation from base to current language
	CMapStringToString TrBaseCur;
	// Translation from current to base language
	CMapStringToString TrCurBase;
	// Map from base language to ID
	CMapStringToInt TrBaseID;
	void Serialize(CArchive &ar);
public:
	static NCMProject& GetProject(void);
	static int StartNewProject(void);
	static int CloseProject(void);

	void PostSerialize(CArchive& ar, const CString& DocPath);
	static void MakeTempName(const CString& FileName, CString& TempName);
	void StoreDIParams2XML() const;
	void StoreMColors2XML() const;
	BObjectManager *GetObjectManager(void) const noexcept { return pObjectManager;}
	double GetInFileVersion() const noexcept { return InFileVersion;}
	void SetInFileVersion(double Val) noexcept { InFileVersion = Val;}
	bool LockLicense(int i);
	bool UnlockLicense(int i);
	void ReleaseLicenses(void);
	void SetFloating(bool Fl) noexcept { FloatingLicense = Fl;}
	void SetPostUserID(int Val) noexcept { PostUserID = Val; }
	unsigned int GetPostUserID();
	bool IsPostUserMaster();
	bool IsLicenseFloating(void) const noexcept { return FloatingLicense;}
	class ParticularExport *GetParticularExport(void) const noexcept { return pPExport;}
	void SetParticularExport(class ParticularExport *pPE) noexcept { pPExport = pPE;}
	CString GetMachinesPath(void) const;
	CString GetUnitsPath(void) const;
	CString GetSettingsPath(void) const;
	CString GetPrototypesPath(void) const;
	CString GetToolsPath(void) const;
	CString GetDemoPath(void) const;
	CString GetMacroPath(void) const;
	CString GetComparePath(void) const;
	CString GetTempPath(void) const;
	NCommonPaths &GetCommonPaths(void) noexcept { return CommonPaths; }
	NGlobalState& GetGlobalState(void) noexcept { return GlobalState; }
	BTimeLine& GetTimeLine(void) noexcept { return TimeLine; }
	double GetDouble(const CString &Name, double Default) const;
	static int LoadFile2Str(const CString &Path, CString &Str);
	static int SaveStr2File(const CString &Path, const CString &Str, bool AllowRewrite);
	void MakeTempMachDir(const CString& Name);
	static BOOL IsDots(const TCHAR* str);
	static BOOL DeleteDirectory(const TCHAR* sPath, bool KeepFolder = false);
	static bool ReadDirFiles(const CString &Path, const CString &Filter, CArray<CString> &Names);
	bool GetUseQuadTreeParallel() const noexcept { return UseQuadTreeParallel; }
	void SetUseQuadTreeParallel(bool Val) noexcept { UseQuadTreeParallel = Val; }
	BProgressPercent& GetProgressPercent() { return ProgressPercent; }
	bool LoadDefaults(const CString& FileName);
	void AddColor(const CString& BaseStr, class NColor* pcol, int CurLangStrID);
	void SetDefaultColorsFonts();

	static constexpr int ALL_POST_USER_ID = 1;
	static constexpr int EDU_POST_USER_ID = 2;
protected:
	void SetDefaultSettings();
};

