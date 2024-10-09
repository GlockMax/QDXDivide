#pragma once
#include "MQuadTreeRot.h"
#include "BGeomArray.h"
#include "BSP_MODEL_API.h"
#include "gquadrender.h"

class BSP_MODEL_API GQuadRenderRot :
	public GQuadRender
{
	static const int LINE_SIZE = 2 * ( 2 + 2 + 1);

public:
	GQuadRenderRot(const MQuadBody &Tree);
	GQuadRenderRot(const GQuadRenderRot & inRender);
	static GQuadRenderRot* CreateGQuadRenderRotList(const MQuadBody &Tree);
	virtual ~GQuadRenderRot(void);
	int RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf) override;
	virtual int RenderWMaskCh(const GBodyRender & MaskRender, int Type, bool complement = false) { complement; Type; MaskRender; return 0; }
	void WriteSTL(CStdioFile& f) const override;
	float *MakeSTLArray(int &TriNum) const;
	double GetVolume() const { return 0.;}
	enum NCMApplicationType GetType() const override;
protected:
	void FillCell(MQuadBody & Tree, int ix, int iy) override;
	static GLvoid * FillContArray(MBSPNode *pRoot, bool Smooth = true);
	static void StoreLine(GLfloat *pRet, const PackedColor col, 
				const BPoint & p0, const BPoint & n0,
				const BPoint & p1, const BPoint & n1);
public:
	double GetMaxX(void) const;
	int RenderAxialSection(void) const;
protected:
	int GenAxialQuads(void) const;
public:
	static double StartAngle;
	static double Angle;
	static double AngleStep;
	static int AngleStepsNum;
	static double StartAngleB;
	static double AngleB;
	static double AngleStepB;
	static int AngleStepsNumB;
	static int StoreCounter;
	static class NColor AxialSectionColorStock;
	static class NColor AxialSectionColorPart;


public:
	static void SetVisParam(double StartAngle , double Angle , double AngleStep);
	static void SetVisParam(double StartAngle , double Angle , int AngleStepsNum);
	static void GetVisParam(double &StartAngle , double &Angle , double &AngleStep, int &AngleStepsNum);
	static void SetFull();
	static void ResetStored();
	static void SetDefaults(class NTiParams &Par);
	void RenderOrd(void);
	const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
    bool FindSegments(class SnapRopes& /*oSegments*/) const override{ return false; }
	bool MakeEnvelopeCont(std::list<BClrContour>& Contours, enum TurnGenMode TGMode) const override { Contours;  return false; }
	bool IsStockRender() const { return StockRender; }
	void SetStockRender(bool StockR) { StockRender = StockR; }
	BBox GetGabar() const override;
	UINT64 GetHASH(const GBodyRender* pEnd) const override;
	float*** GetArray() const { return (float***)Array; }
	GBodyRender* MakeCopy() const override;
protected:
	bool StockRender;
};
