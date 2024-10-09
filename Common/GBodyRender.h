#pragma once
#include "Afxmt.h"
#include "BBox.h"
#include "NCollidableObject.h"
#include "BDCEL.h"
#include "MQuadBody.h"
#include "MElemIDProc.h"
#include <array>
#include <vector>
#include <list>
#include "BRotSolid.h"
#include "BStockGrID.h"
#include "BSP_MODEL_API.h"

enum NCMApplicationType;


class BSP_MODEL_API GBodyRender : public NCollidableObject
{
	friend class GQuadRender;
	// Gabar corresponds to model i.e. is rotated around X axis by -90.
	// meshes are rotated relative to Gabar around X axis by +90
public:
	GBodyRender(const GBodyRender& inRender);
	GBodyRender(const class MBody& Tree);
	virtual ~GBodyRender(void);
	virtual void WriteSTL(CStdioFile& f) const = 0;
	virtual NCMApplicationType GetType() const = 0;
	virtual const class BPoint RayCasting(const class BPoint& P, const class BPoint& V, double MinW, class BPoint& N) const = 0;
    virtual bool FindSegments(class SnapRopes& oSegments) const = 0;
	virtual MQuadBody::Window CalcWindow(const BBox & Bounds) const = 0;
public:
	int Render(enum RenderMode GlobRMode, int Type, bool DrawSurf, class MTMachineTool& MT);
	int FillAllArrays(class MQuadBody& Body);
	virtual int FillModifArrays(class MQuadBody& Tree);
	int RenderWMask(const GBodyRender& MaskRender, int Type, class MTMachineTool& MT, bool complement = false);
	void TransModifArrays(GBodyRender& inRender);

public:
	GBodyRender* GetNextRender() const;
	GBodyRender* GetPreviousRender() const;
	virtual BBox GetGabar() const { return Gabar; }
	void SetNextRender(GBodyRender* next);
	void SetPreviousRender(GBodyRender* previous);
	void AddRenderInList(GBodyRender* pGBodyRender);
	void SetUseExternColor(bool Val) noexcept { UseExternColor = Val; }
	static bool Empty(GLvoid* in) { return (in == (GLvoid *)QR_EMPTY) || (in == (GLvoid*)QR_EMPTY_DEF); }
	static bool NotDefined(GLvoid* in) { return (in == (GLvoid *)QR_EMPTY); }

protected:
	GBodyRender* pNextRender;
	GBodyRender* pPreviousRender;
	CCriticalSection CritSection;
	BBox Gabar;
	BMatrPair BodyRot; // This matrix is pure rotation matrix and ensures rotation of this body to it's right position around global zero point
	bool BodyRotActive;
	bool UseExternColor;
	BStockGrID StockPos;
	class RRopes* pRopes;
protected:
	virtual void TransModifArraysCh(GBodyRender& inRender) = 0;
public:
	virtual int RenderWMaskCh(const GBodyRender& MaskRender, int Type, bool complement = false) = 0;
	virtual int RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf) = 0;
	virtual int FillAllArraysCh(MQuadBody& Body) = 0;
	virtual int FillModifArraysCh(MQuadBody& Tree) = 0;
	virtual double GetVolume() const = 0;
	virtual bool MakeEnvelopeCont(std::list<BClrContour>& Contours, enum TurnGenMode TGMode) const = 0;
	virtual UINT64 GetHASH(const GBodyRender* pEnd) const = 0;
	BStockGrID GetStockPos() const { return StockPos; }
	virtual GBodyRender* MakeCopy() const = 0;
	const auto& GetRopes() const { return *pRopes; }
protected:
	static class NColor EdgesColor;
	static double EdgesWidth;
	static int DichotDegree;
	static bool FindVergPoint;
	static bool SmoothRender;
public:
	static void SetEdgesColor(const NColor& Col);
	static const NColor& GetEdgesColor(void);
	static double GetEdgesWidth(void);
	static void SetEdgesWidth(double Width);
	static double MinSin2;
	static double GetMinSin2();
	static void SetMinSin2(double Val);
	static int GetDichotDegree();
	static void SetDichotDegree(int Val);
	static bool GetFindVergPoint();
	static void SetFindVergPoint(bool Val);
	static void SetSmooth(bool in);
	static bool GetSmooth();
};
