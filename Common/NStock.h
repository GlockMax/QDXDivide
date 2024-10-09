// NStock.h: interface for the NStock class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "StdAfx.h"	
#include "NCMComData.h"	
#include "NCEnums.h"	
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BBox.h"	
#include "BPoint.h"	
#include "NColor.h"
#include "NMovableObj.h"
#include "BGeomArray.h"
#include "PAbstractObject.h"
#include "RenderMode.h"
#include "NTurnStockSkeleton.h"
#include "DXModel.h"
#include "BRotSolid.h"
#include "BStockGrID.h"
#include "RRopes.h"
#include <vector>
#include <array>
#include <list>

class GBodyRender;
class GQuadRenderRot;

class AFX_EXT_CLASS NStock : public NMovableObj, public PAbstractObject   
{
	friend class CStocksCollection;
public:
	enum StockCrType
	{
		SCT_EMPTY,
		SCT_BOX,
		SCT_BAR,
		SCT_BAR_SM,
		SCT_CONT,
		SCT_CYL,
		SCT_STL,
		SCT_STLMT,
		SCT_STLT,
		SCT_SWEEP,
		SCT_IMPEXP,
		SCT_WELD

	};
public:
	class MBody * GetBody() const;
	virtual void Serialize(CArchive &ar);
	static void SerializeExt(CArchive &ar);
	virtual void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf);
	void RestDrawOriginal();
	NStock();
	virtual ~NStock();
	BOOL Visible;
	BOOL WFrame;
	BOOL Transparent;
	bool IsCreated;
	DECLARE_SERIAL(NStock)
	virtual void CreateCopyTo(class NStock* &stock) const;
	BStockGrID GetStockGr() const { return StockGr; }
	void SetStockGr(BStockGrID Val) { StockGr = Val; }
	void SetMillStockChanged(bool Val) { MillStockChanged = Val; }
	void SetTurnStockChanged(bool Val) { TurnStockChanged = Val; }
	void MakeRenderCopy();
protected:
	static const int IOVersion = 3;//27.10.21
	double Transparency;
	RenderMode RMode;
	NColor Color;
	BOOL SharpEdgesS;
	BOOL SharpEdgesDX;
	bool DrawSmoothSurf;
	int ColorNum;// Is used for StockCont only;
	int BSPTNum;// Complex body
	int BSPTNumRot;// Turn body
	GBodyRender *pRender;
	GQuadRenderRot *pRenderRot;
	class NTurnStockSkeleton TurnSkeleton;
	DXModel DXWorld;// DXTST
	int BSPTNumDX;// Dexel body
	bool Frozen;
	bool Temporary;
	double WholeCellsKoef;
	BBox Gabar;
	BStockGrID StockGr;
	bool MillStockChanged; // For mill turn only
	bool TurnStockChanged; // For mill turn only
private:
	void SetWFrame(BOOL wFrame) { this->WFrame = wFrame; }
private:
	NCMApplicationType Type;
	NCMApplicationType State;
public:
	bool IsRealStock(void) const;
	const NColor & GetColor(void) const;
	virtual void SetColor(const NColor & Col);
	void SetColor(COLORREF Col);
	void SetAlpha(double Alpha);
	bool CreateRender(void);
	bool CreateModifRender(void);
	void Render(enum RenderMode GlobRMode, int Type, bool DrawSurf);
	void Reset(void);
	void CreateFromBSP(void);
	void ExportSTL(CStdioFile *f);
	void ExportBSP(CFile *f);
	void ExportDX(CFile *f);
	GBodyRender * GetRender() const;
	void NullRender(void);
	GBodyRender * GetRender(NCMApplicationType Type) const;
	virtual BBox GetGabar(void) const;
	BBox GetGabarRender(void) const;
	void ClearRender();
	BOOL GetWFrame() const { return WFrame; }
protected:
	virtual void Create(void);
	virtual bool CreateBody(void){return false;}
	virtual bool CreateRenderRot(void);
	bool CreateRenderMill(void);
	bool CreateStateRender(void);
	bool CreateModifRenderMill(void);
public:
	MBody * GetBodyRot(void) const;
	MBody * GetBodyMill(void) const;
	NCMApplicationType GetStockType(void);
	bool IntersectBodyes(void);
protected:
	bool SetStockState(NCMApplicationType NewState);
	bool SetStockType(NCMApplicationType NewType);
	void MakeGabarBoxDX5X(int num);
	void MakeBoxDX5X(int num, const BBox &Box);
	class MQuadBody* Load1Tree(CFile& f, bool GabarOnly = false);
	BOOL Save1Tree(CFile& f, int TreeInd, const BMatr& M);
public:
	NCMApplicationType GetStockState(void);
	void CrBody4Mill(void);
	void RemoveMillBody(void);
	void RemoveRotBody(void);
	bool RenderSect(void);
	const BGeomArray * NStock::GetSect(int Part) const;
	bool CreateTurnSkeleton(void);
public:
	RenderMode GetRMode(void) const { return RMode;}
	virtual void SetRMode(RenderMode Mode);
//	void SetColorNum(int CNum){ ColorNum = CNum;}
	bool IsVisible(void) { return (Visible == TRUE); }
	void SetVisible(BOOL value) { Visible = value; }
	virtual GLfloat *MakeOneSection(const BPoint &P, const BPoint &V, bool Trimed = false) const {return NULL;}
	virtual double GetVolume();
	virtual void DrawSelected(void) const;
	virtual void DrawGabar(void) const;
	const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
	virtual const BPoint RayCastingOr(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const{return BPoint(0., 0., 0., -1.);}
	int SubtractStl(const class BStl & Stl);
	BOOL GetEdgesS(void) { return SharpEdgesS;}
	BOOL GetEdgesDX(void) { return SharpEdgesDX;}// For LoadSingleStock only
	void SetEdgesS(BOOL E) { SharpEdgesS = E;}
	void SetEdgesDX(BOOL E) { SharpEdgesDX = E;}
	bool GetDrawSmoothSurf(void) const { return DrawSmoothSurf;}
	virtual void SetDrawSmoothSurf(bool SmoothSurf) { DrawSmoothSurf = SmoothSurf;}
	double GetTransparency(void) const { return Transparency;}
	void SetTransparency(double Transp) { Transparency = Transp; SetAlpha(Transparency); }
	class PropList * Create(class PropList *) override;
	class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
	bool NeedDraw(enum RenderMode GlobRMode, enum RenderMode ExtRMode);
	bool ExportStC(void);
	bool ExportDXF(void);
    bool FindSegmentsWithRender(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr);
    virtual bool FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr) { return false; }
	bool IsFrozen(void) const { return Frozen;}
	void SetFrozen(bool Val) { Frozen = Val;}
	bool IsTemporary(void) const { return Temporary;}
	void SetTemporary(bool Val) { Temporary = Val;}
	double GetWholeCellsKoef(void) const {return WholeCellsKoef;}
	void SetWholeCellsKoef(double iVal) { WholeCellsKoef = iVal;}
	bool MakeTurnFromMill(bool RecalcGabar, TurnGenMode Mode);
	void MakeRot(const class TBladeCont &Cont);
	void MakeRotMulti(const std::list<BClrContour>& Contours);
	virtual StockCrType GetType() const { return SCT_EMPTY;}
	const NTurnStockSkeleton & GetTurnSkeleton() const { return TurnSkeleton; }
	virtual CString GetFileName() const { return CString(); }
	bool TransformCutted(const BMatr &CuMatr);
	void SetCollided(bool Val);
	static TurnGenMode GetTGMode();
};
