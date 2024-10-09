#pragma once
#include "StdAfx.h"
#include <vector>
#include "BMatr.h"
#include "bpoint.h"
#include "BCurve.h"
#include "MTMachine.h"
#include "SCoordsDef.h"
#include "MTCompensations.h"
#include "RenderMode.h"
#include "SMachStateFix.h"
#include "BStockToolInds.h"
#include "BEdgesAttrs.h"

class NToolCombined;
/// Класс станка.
/** 
\sa MTBase, MTMachine, MTCollisionArray
*/
class AFX_EXT_CLASS MTMachineTool : public MTMachine, public PAbstractObject
{
protected:
	class MTPropMap : public CTypedPtrMap<CMapStringToPtr, CString, class MTPartVisProp *>
	{
	public:
		void Serialize(CArchive &ar);
		void Serialize48(CArchive &ar);
	};
	class ShiftData
	{
	public:
		ShiftData();
		~ShiftData();
		void Set(int T, int A, const BPoint& Shift, const SCoordsDef& Def);
		BPoint ToolShiftVec;
		int Turr;
		int Att;
		SCoordsDef CoordsDef;
		bool IsDefined() const { return Defined; }
	protected:
		bool Defined;
	};
public:
	class RCPData
	{
	public:
		RCPData() { CoordD.Set(0., 0., 0., 1); RCPMask = 0; }
		~RCPData() {};
		const BPoint& GetCoordD() const { return CoordD; }
		void SetCoordD(const BPoint& Coord) { CoordD = Coord; }
		void SetRCPMask(int Mask) { RCPMask = Mask; }
		int GetRCPMask() const { return RCPMask; }
		void Set(const BPoint& StartAng) { StartABC = StartAng; }
		const BPoint& GetStartABC() const { return StartABC; }
	protected:
		BPoint StartABC;
		int RCPMask;
		BPoint CoordD;
	};
	MTMachineTool();
	virtual ~MTMachineTool();
	const BMatr GetToolMatr(int Tur, int Att, bool UseCompensation);
	void GetToolAndStockMatr(BMatr &ToolM, BMatr &StockM, int StockPosition, int Tur, int Att, bool UseCompensation);
	void GetToolAndStockMovedMatr(BMatr &ToolM, BMatr &StockM, int StockPosition, int Tur, int Att, bool UseCompensation);
	bool HaveStockMoved() const;
	const BMatr GetStockMatr(int StockPosition, bool UseCompensation);
	const BMatr GetStockMovedMatr(int StockPosition, bool UseCompensation);
	const BMatr GetStockMovedMatrSGr(BStockGrID StockGroup, bool UseCompensation);
	const BMatr GetStock2ToolMatr(int StockPosition, int Tur, int Att);
	const BMatr GetStockMoved2ToolMatr(int StockPosition, int Tur, int Att);
	const BMatr GetStockMoved2ToolMatrC(int StockPosition, int Tur, int Att) const;
	const BMatr GetStockMoved2ToolMatrCSGr(BStockGrID StockGroup, int Tur, int Att) const;
	const BMatr GetStock2ToolMatrWRec(const class SMachState& MachState, int StockPosition, int Tur, int Att);
	const BMatr GetStockMoved2ToolMatrWRec(const class SMachState& MachState, int StockPosition, int Tur, int Att);
	void Serialize(CArchive &ar) override;
	void ProcCadrStep(const class NCUnit& Unit, const class NCUnitStatePrev & State);
	void Reset(const SCoordsDef& CDef);
	DECLARE_SERIAL(MTMachineTool)
	/// на входе LoadXML - путь к каталогу со станками, имя станка
	bool LoadXML(const CString &path, const CString &name, bool IgnoreParts = false);
	bool LoadXMLBranch(const CString &FileName);
	/// на входе ReadXMLObject - xml, путь к каталогу со станками. Имя уже хранится в объекте
	bool ReadXMLObject(TiXmlDocument *doc, const CString &path, bool IgnoreParts = false);
	MTPart* ReadXML1Part(TiXmlNode* curaxis, const CString& path, bool IgnoreParts, const MTPartAxis* pPartAxis = nullptr);
	bool ReadXMLObjectBranch(TiXmlDocument *doc);
	bool SaveXML(const CString &path, const CString &name);
	bool SaveXML4Branch(const CString &path, const CString &name, const CString& AxisName);
	void WriteXML1Axis(TiXmlElement* parent, const MTAxis* pAxis2Write, bool FullPartNames) const;
	bool WriteXMLObject(TiXmlDocument* doc) const;
	bool WriteXMLObject4Branch(TiXmlDocument* doc, int AxisIndex) const;
	void RenameAxis(int index, const CString& NewName) override;
	int SwapAxisNames(const CString& First, const CString& Second);
	void SetFileName(const CString &name);
	int CalculateTiltPlane(class NTiltPlane &TiltPlane, const SCoordsDef& FixedDef, int StockPosition);
	RenderMode GetRMode(void) const { return RMode;}
	void SetRMode(RenderMode Mode) { RMode = Mode;}
	INT_PTR PositionTool2CS(const BMatr &CS, int StockPosition, int Turret, int Attach, std::vector<SMachState*> &Result, const class SCoordsDef& CDef);
protected:
	MTCompensations Comp;
	double ToolLength;
	BBox StockGabar;
	bool DrawAllTools;
	RenderMode RMode;
	double Transparency;
	MTPropMap PartsPropList;
	BEdgesAttrs EdgesAttrs;
	std::vector<ShiftData> ToolShifts;


	CCriticalSection CritSection;

public:
	const BEdgesAttrs & GetEdgesAttrs(void) const { return EdgesAttrs; }
	void SetEdgesColor(const NColor & Col);
	void SetEdgesWidth(double Width);
	void SetSmoothAngle(double Angle);
	double GetTransparency(void) const { return Transparency;}
	void SetTransparency(double Transp);
	bool GetDrawAllTools() const { return DrawAllTools; }
	void SetDrawAllTools(bool Val) { DrawAllTools = Val; }
	void SetVisible(bool vis = true);
	bool IsVisible(void) const;
	const CString & GetFileName(void) const;
	void SetState(BMTLinkage ConfNum, const class SMachState & State);
	void SetStateWORecalc(BMTLinkage ConfNum, const class SMachState& State);
	void SetState(BMTLinkage ConfNum, const class SMachState& State1, const class SMachState& State2);
	void SetStatePartial(BMTLinkage ConfNum, const class SMachState& State1, const class SMachState& State2);
	void SetStatePartial(BMTLinkage ConfNum, const class SMachState& State1);
	void GetState(BPoint& XYZ, BPoint& ABC, const SCoordsDef& FixedDef);
	void GetState(class SMachState& State, class SCoordsDef& CDef);
	void GetState(BMTLinkage& Link, class SMachState& State, class SCoordsDef& CDef);
	void GetSpecState(int i, class SMachState & State, class SCoordsDef &CDef, int CurChannel);
	bool GetSpecState4Tool(int i, class SMachState & State, class SCoordsDef& CDef, int CurChannel);
	void GetCoordsDef(class SCoordsDef& CDef) const;
	void SetComp(const class NCUnit &Unit, double ToolLength, const BPoint & ToolShift, int Tur, int Att, const BBox &StockGabar);
	BPoint CalcCurToolComp(const ShiftData& Shift);
	BPoint CalcCurShiftVec(const ShiftData& Shift) const;
	virtual void Draw(RenderMode GlobRMode, bool DrawSurf);
	BBox GetGabar();
	void FullCopyWOVis(const MTMachineTool& MT);
	// Find Result state for the linear movement from Start to End while RCP is active
	void CalcIntState(int RCPMask, double t, const class SMachState& Start, const class SMachState& End, class SMachState& Result, int StockPosition, int Turret, int Attach);
	static void CorrStateRCP(int RCPMask, const class SMachState& Start, class SMachState& Interm);
protected:
	int GoToCurPos();
	int SetCurComp();
	int ApplyCurComp(bool ForDraw);
	int ApplyRCP();
public:
	BPoint GetVectXYZ(const BPoint& V, const SCoordsDef& FixedDef, int Tur, int Att);
	BPoint GetVectXYZ(const BPoint& V, const ShiftData& Shift);
	BMatr GetMainCoordsMatr(const SCoordsDef& FixedDef);
	int MakeActual();
	int ForceActual();
	const BMatr GetRespMatr(const BMatr &CSMatr, const BPoint &ABC, int Tur, int Att, int StockPosition, const SCoordsDef* pFixedState);
	const BMatr GetToolPlaceMatr(int Num, int Tur, int Att, bool *Found = NULL);

	const CString GetNameGeneralAxis(int StockPosition, int Tur, int Att);///< Возвращает имя общей оси в дереве для ветвей оси и заготовки//!!!
public:
	/// Возвращает номер оси в массиве CooArray по её имени
	int NumArray(const CooArray &array, const CString &name);
	/// Проверяет соответствуют ли координаты друг другу в begin и end
	bool DataFit(const SMachState& Begin, const SMachState& End);
	/// Возвращает производную(частную по переменной этой оси) матрицы M оси NameAxis
	bool PartialDerivative(BMatr& M, unsigned int NumAxis, const SMachState& Begin, const SMachState& End, double t, const BMatr& STMatr, int StockPosition, int Tur, int Att);
	/// Возвращает полную производную матрицы движения инструмента относительно заготовки по всем переменным
	bool TotalDerivative1(BMatr& M, const BMatr& StMatr, const SMachState& Begin, const SMachState& End, double t, int StockPosition, int Tur, int Att);
	bool TotalDerivative(BMatr& M, const SMachState& Begin, const SMachState& End, double t, int StPos, int Tur, int Att);
	/// Возвращает опорные точки характеристической ломаной
	BPoint * PointsOfCurve(int RCPMask, const SMachState& Begin, const SMachState& End, const BPoint &Base, const BStockToolInds& STInds);// Base - base point in the tool coordinates system
	/// Возвращает точки сегментов в формате BCurve
	BPoint * PointsOfSegments(int RCPMask, const SMachState& Begin, const SMachState& End, int &kol, const BPoint &Base, const BStockToolInds& STInds);// Base - base point in the tool coordinates system
	/// Объединение двух массивов точек в один и его запись в формате BCurve в массив A
	BPoint * MergeOfPoints(BPoint *A, BPoint *B, const int kola, const int kolb);
	/// Количество осей вращения в ветвях Stock и Tool. В sumangle записывает суммарный угол
	int kolRotaryAxis(int StockPosition, const SMachState& Begin, const SMachState& End, double& sumangle);
	/// Возвращает промежуточное состояние между begin и end при заданном t в temp
	bool TempState(CooArray &temp, const CooArray &begin, const CooArray &end, double t);
	/// Создает кривую 5-коорд. обработки
	BCurve * Make5xLine(int RCPMask, const class NCUnit& Unit, const NCUnitStatePrev & StartSt, const NCUnitStatePrev & EndSt, const BPoint &Base, const BStockToolInds& STInds);// Base - base point in the tool coordinates system);
	BCurve * Make5xLine(int RCPMask, const class NMachUnitPair* pMUPair, const BPoint &XYZ0, const BPoint &ABC0, const BPoint &XYZ1, const BPoint &ABC1, const BPoint &Base, const BStockToolInds& STInds);// Base - base point in the tool coordinates system);
	BCurve * Cr5xLine(int RCPMask, int StockPosition, const SMachState& Begin, const SMachState& End, const BPoint &Base, const BStockToolInds& STInds);// Base - base point in the tool coordinates system)
	CCriticalSection &GetCriticalSection(void);
	
	SCollision* GetAxisCol(const CString &AxisName, int index = 1);
	SCollision* GetCollision(int index);
	int GetColIndex(const CString & Axis1, const CString & Axis2);///< Номер коллизии в массиве коллизий
	void MoveCollision(const CString& Axis1, const CString& Axis2, const CString& NewAxis1);

	const CString & GetPartAxisNameByPartIndex(int PartIndex);

	void GetRealState(class SMachState & State, class SCoordsDef &CDef);
	SMachState GetFixState(const class SCoordsDef* pFixedNames);

	void AddPart(MTPart * pPart);

	virtual void RemoveAll();
	bool RemoveBranch(int AxisIndex);
	void RemoveCollision(const CString & Axis1, const CString & Axis2);
	void RenameInCollisions(const CString & OldAxisName, const CString & NewAxisName);
#ifdef _DEBUG
	virtual void DebugPrint(const CString& Label,bool parents = 1, bool level = 1);
#endif
	//bool ChangeToolTurret(int oldTurret, int newTurret);
	//bool ChangeVirtToolTurret(int oldTurret, int newTurret);
	bool ChangeToolTurret(int AxisIndex, int newTurret);
	bool ChangeVirtToolTurret(int AxisIndex, int newTurret);
	/// На множестве инструментов пара turret - attachment должна быть уникальной
	bool ExistsToolTurAtt(int Turret, int Attachement);
	int GetToolTurOnLevel(const CString & ParentName);
	bool ExistsVirtToolPosTur(int Position, int Turret); 
	bool IsVirtToolUnique(int AxisIndex, int Position, int Attach, int OldTurret, int NewTurret); 
	
	int GetNextAttachment(int Turret);
	int GetNextTurret();

	int GetVirtToolTurOnLevel(const CString &ParentName);
	int GetNextPositionVirtTool(int Turret);
	int GetNextTurretVirtTool();
	int GetTurretToolAmount(int Turret);
	int GetTurretVirtToolAmount(int Turret);
	/// При изменении параметров инструментов нужно также переименовывать объекты столкновения
	void ChangeToolAttachment(int AxisIndex, int newAttachment);
	void ChangeVirtToolPosition(int AxisIndex, int NewPosition);
	void ChangeVirtToolAttachment(int AxisIndex, int newAttachment);
	const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N, class RPickID& PickID) const;
    bool FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr);
	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
	void FillPartsPropList(void);
	void ApplyPartsPropList(void);
	void ClearPartsPropList(void);
	bool HasTranslucent(void);
	int MakeScrewMotion(const class NCadrGeom & Geom, std::vector <class BScrewMotion> &SMotion, int Turret, int Attachment
		, const SCoordsDef& FixedDef);
	void SetCompensations(const MTCompensations &inComp);
	void GetStock2ToolRotChain(int StockPosition, int Turret, int Attach, std::vector<int> &AxInds);
	void FillMachineFile(CStringArray &StringArray);
	CString GenNextAxisName(const CString& Name);
	void CalcRCPDisp(const SMachState& Prev, const SMachState& Next, int StockPosition, int Turret, int Attachment, RCPData& RCP, double LengthComp);
	BMatr GetMCS(const NCadrGeom* pGeom, const NToolCombined* pTool, const SCoordsDef& FixedDef);
	BMatr GetMCS(const NCadrGeom* pGeom, int Turret, int Attachment, const SCoordsDef& FixedDef);
	bool IsAnyToolAncestor(const MTAxis* pAxis);
	bool IsAnyStockAncestor(const MTAxis* pAxis);
protected:
	int MakeScrewMotion(int RCPMask, BMTLinkage ConfNum, const BPoint& XYZ0, const BPoint& ABC0, const BPoint& XYZ1, const BPoint& ABC1
		, std::vector <class BScrewMotion>& SMotion, int StockPosition, int Turret, int Attachment, const SCoordsDef& FixedDef);
	int MakeScrewMotion(int RCPMask, BMTLinkage ConfNum, const SMachState& State0, const SMachState& State1
		, std::vector <class BScrewMotion>& SMotion, int StockPosition, int Turret, int Attachment, const BPoint& Angles);

};