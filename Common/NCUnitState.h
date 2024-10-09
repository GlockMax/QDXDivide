// NCUnitState.h: interface for the NCUnitState class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCUnitStatePrev.h"
#include "NVarSet.h"


class AFX_EXT_CLASS NCUnitState : public NCUnitStatePrev
{
	friend class NStateUnitPane;
public:
	NCUnitState(const NCUnitState &InState);
	NCUnitState();
	virtual ~NCUnitState();
	static int UCSRegsSubst[MAX_UCS];// Register number for the first UCS element
	static vector<pair<int, int>> VisibleRegisters;// Visible registers intervals. Register is visible if it belongs to any interval

public:

	NUCSs UCSs;
	Coords ChangedCoord;
	NMirrorRotate MirrorRotate;
	BMatr UCSMatr;
	BMatr TiltNewSys;
	cadrID CadrID;// ID of the text cadr. Contains the program (main or extra) file number and the current line number
	cadrID CadrIDEx;// ID of the "executable". Contains the main program number and CurHCadr (without cycles)
	NMultiCounter MultiCounter;
	NCPar CPar; // Parameters for the lathe cycles

	double GlobalDwell;
	int ActiveUCS;
	BOOL ParToNext;
	BOOL RetractComp;
	BOOL CompCancel;
	BOOL EnableExtraArc;
	BOOL EnableExtraArcModal;
	long TechTable;
	int ActiveCopyEnd;
	bool ActiveCopyEnding;
	bool ActiveCopyStarting;
	BOOL MoveRotateCS;// True if MoveRotateCS is specified in the current cadr
	BOOL MoveRotateCSRel;// True if MoveRotateCSRel is specified in the current cadr
	BOOL RotateCSABC;// True if RotateCSABC is specified in the current cadr
	BOOL MoveRotateCSCancel;// True if MoveRotateCSCancel is specified in the current cadr
	BOOL ZeroPreset;// True if ZeroPreset is specified in the current cadr
	ZPType ZeroPresetType;
	BOOL Debug;
	BOOL CommentActive;
	BOOL FixX;
	BOOL FixY;
	BOOL FixZ;
	BOOL FixA;
	BOOL FixB;
	BOOL FixC;
	double CurPunchStep;
	enum PunchSt Punch;
	enum LengthOffset Offset;
	enum CycleRet CurCycleRet;
	bool CadrSkiped;
	bool EndProgram;
	bool AllowBreak;
	double CycleStartZ;
	double CycleDepthZ;
	int CycleThreadParm;
	int ThisCadrToolN;//The last Tcode value after last M06
	double CurDiamComp;
	double CurCodeComp;
	double CurdXComp;
	double CurdZComp;

	int CurCadrNum;
	double Time;//in seconds
	double Length;//trajectory length in mm
	double FastLength;//fast moovement trajectory length in mm
	double WorkLength;//work moovement trajectory length in mm
	double CadrTime;//in seconds
	double Angle;//An angle between previos and current cadr in degrees
	double AngleX, AngleY, AngleZ;//angles between current cadr and an axis in degrees
	double CadrLength;//cadr length in mm
	double Rc;//calculated radius
	double Ic, Jc, Kc;//calculated center coordinates
	double FeedScale;
	enum FeedMeasure FeedM;
	int LeadingAxis;// The first axes mentioned in the cadr
	double TurnThreadR2;
	BOOL MetricCoord;
	BOOL AllowSpiral;
	int CurCadr;//Number of current cadr. Is used for NStateInt only
	cadrID NextCadr;
	BOOL AbsoluteCoord;
	BOOL CycleZAbs;
	int NextToolNum;
	enum  curveMod CurCurveMod;
	double CurI2;
	double CurAngle;// For HAAS
	bool CurAngleDefined;// For HAAS
	double CurXScale;// Current program defined scales
	double CurYScale;
	double CurZScale;
	double CurIScale;
	double CurJScale;
	double CurKScale;

	double MashX2;
	double MashXUCS;
	double MashYUCS;
	double MashZUCS;
	double MashcXUCS;
	double MashcYUCS;
	double MashcZUCS;
	double PolarDist;
	double PolarAngle;
	bool PolarHelix;

	int AutoCycleCount; //Cycle counter. The number of loops to go.
	int ModalAutoCycleCount; //Cycle counter. The number of loops to go.

	int CoordCompSignum;// 0 - undefined; 1 - "+"; -1 - "-"
	int CoordCompCode;

	bool IntNotFound;

	BOOL ArcCenterFromPolarCenter;
	BOOL DefinePolarCenter;
	bool ProgStop;
	bool OptionalStop;
	bool ThreadCadr;
	Coords ToolDisplChanged;
	RotationType RotType;
	TravelLimit TLFlag;
	int NonExecCount;
	double HhLen;
	double HhCca;
	double HhR;
	bool NonModalCycleCall;
	int DefinedCycleNum;
	CString ProgramName;
	CString ProgramNumber;
	CString ProgramNumberStore;
	CString CycleTappingZ;
	CString CycleI;
	CString CycleJ;
	NFavorConcepts FavorConcepts;
	bool ModalSubr2;
	bool VariablesDecl;
	cadrID ModalSubrIDRet; // Temporary ID of modal subroutine return point
	NCallStack CallStack;
	NCallStack CycleStack;
	NCallStack WhileStack;
	bool AddCoordsChanged;
	bool ProbeCadr;
	bool TouchCadr;
	bool ProbeCollision;
	bool ProbeExpNoTouch;
	bool ProbeExpTouch;
	enum PlaceToolType PlaceTool;
	RCoordPair PlaceToolAxes;
	bool MachineCSActive;
	bool FeedNonModal;
	int CircleFullTurns;
	bool GrooveON;
	int CycleDwellTime;
	BSplineParams Spline;
	bool DontDrawIfEmpty;
	bool RCPActive;
	unsigned int RCPMask; // 3 bits: Left->right. RCPActive A axis; RCPActive B axis; RCPActive C axis
	BPoint RCPStartABC;

	bool RCPCanceling;
	double ChamfMashX;// Temporary coords of chamfered corner
	double ChamfMashY;
	double ChamfMashZ;
	double MoveRotateCSAngle;
	bool ChamfCoordStored;
	bool MileStoneFound;
	SMachState AddCoordsFlags;
protected:

	NCoordLimits CoordLimits;
	BPoint PolarCenter;
	CString CurProg;
	double WorkedLifeTime;
	double RestLifeTime;
	IntToolID ID4LifeTime;
	class NMapToolID2Double WorkedLifeTimes;
	class NMapToolID2Double RestLifeTimes;
	int CurHCadrAbs;//Number of current cadr in history with cycles consideration
	NVarSet VarSet;
public:
	const NVarSet &GetVarSet() const;
	NVarSet &GetVarSet();
	void ChamfCoord2Coord();
	void Coord2ChamfCoord();
	void SwapChamfCoord();
	void ChamfCoord2Prev(NCUnitStatePrev &St);
	bool IsChamfCoordStored() const { return ChamfCoordStored; }
	int AskUser(int StartPage, int CurZeroPos);
	void Serialize(CArchive &ar);
	bool SetCurCycle(enum CycleType val);
	bool SetCurDirComp(enum LR val);
	void CalcAxisAngles(NCadrGeom *pGeom);
	void CalcArcParam(NCadrGeom *);
	double CalcAngle(const NCUnitStatePrev & PrevState);
	NCoordLimits &GetCoordLimits(){ return CoordLimits; }
	//	NCUnitState(class SCoordsDef *pDef);
	void IncrCurHCadr(int val = 1);
	void ClearCurHCadr(void);
	void SetCurCadr(int val);
	void SetCurCadr(cadrID ID);
	void SetCurCadrEx(int val);
	void SetCurCadrEx(cadrID ID);
	double CalcFeed() const;
	void IncrementTimer(NCadrGeom *pGeom);
	double CadrLength4Time(const NCadrGeom &Geom) const;
	double SetCadrTime(NCadrGeom *pGeom) const;
	void Reset() override;
	void ResetWOComp();


public:

	double CalcFixedCutTime(const NCadrGeom * pGeom) const;
	const NCBase GetActiveUCS(void) const;
	int AddGeom2CycleGeom(BGeomArray & CycleGeom);
	static void GetSignsFromK(double Code, double * s0, double * s1);
	const BPoint GetToolDisplHAAS(bool CompensationOn) const;
	bool SetRegister(int Ind, double Value);
	double GetRegister(int Ind);
	bool CopyUCS2Regs(void);
	NMapIntToDouble & GetRegisters(void);
	const NMapIntToDouble & GetRegisters(void) const { return Registers; }
	void ResetRegisters(void);
	void SetStockPos2Geom(int SP);
	void SetMatrNum2Geom(int MatrNum);
	void SetInterpNum2Geom(int InterpNum);
	void SetMTConfNum2Geom(const BMTLinkageDiff& Diff);
	void SetPolarGeom(void);
	void SetCylindricGeom(void);
	bool IsNotSpecCycle(void);
	bool IsNotDrillAxis(const CString& CurWord);
	bool IsCoordAxis(const CString& CurWord, char CoordName);
	void ClearCurHCadrAbs(void);
	void SetCurHCadrAbs(INT_PTR val);
	int GetCurHCadrAbs(void) const  { return CurHCadrAbs; }
	void CalcLength(NCadrGeom *pGeom);
	int GetCurCadr(void) const { return CadrID.Cadr; }
	cadrID GetCurCadrID(void) const { return CadrID; }
	BPoint GetPolarCenter(void);
	void SetPolarCenter(const BPoint &P);
	void ApplyPolarCoord(void);
	void CalcPolarCoord(double X, double Y, double Z);

	cadrID GetCurCadrIDEx(void) const { return CadrIDEx; }
	cadrID GetCurHCadrAbsEx(void) const { return cadrID(CadrIDEx.Prog, CurHCadrAbs); }
	int GetCurHCadr(void) const { return CadrIDEx.Cadr; }
	const CString & GetCurProg(void) const { return CurProg; }
	void SetCurProgEmpty(void);
	void SetCurProg(const CString &ProgName);
	void ForceMakeEqualTo(const NCUnitState &src);
	bool CalcCurToolDispl(bool PartialCompCancel);
	bool Push133Registers(void);
	bool Pop133Registers(void);
	void Restore133Registers(void);
	bool IsRCPActive(void) const { return RCPActive; }
	void ReplaceByPrev(const NCUnitStatePrev &St);
	void ProcessLifeTime();
	void ProcLifeTime4ToolChange(const IntToolID &OldID, double ToolLifeTime);
	double GetRestLifeTime() const { return RestLifeTime; }
	void RestartLifeTime(double ToolLifeTime);
	void SetRegInteger(int RegNum) { Registers.Integers.insert(RegNum); }
	void RemRegInteger(int RegNum) { Registers.Integers.erase(RegNum); }
	bool AddReplacePair(const CString &RegExp, const CString &ReplExp, bool LocalVar);
	EvaluateCadrResult ProcessProbe(int HistInd, const class BProgram *pProg);
	void SetCoordsFromGeom();
	void CalcUCSCoords();
	void RecalcFromMachCS();
	void RecalcToMachCS();
};
