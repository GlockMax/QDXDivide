#pragma once
#include "RDCEL.h"
#include "MQuadTree.h"
#include "BSP_MODEL_API.h"
#include "GQuadRender.h"


class BSP_MODEL_API GQuadRenderMill :
	public GQuadRender
{
	static const int TRI_SIZE = 3 * ( 3 + 3 + 1);
public:
	GQuadRenderMill(const MQuadBody &Tree);
	GQuadRenderMill(const GQuadRenderMill & inRender);
	static GQuadRenderMill* CreateGQuadRenderMillList(const MQuadBody &Tree);
	static GQuadRenderMill* CreateListCopy(const GQuadRenderMill &inRender);
	virtual ~GQuadRenderMill(void);
	int RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf) override;
	int RenderWMaskCh(const GBodyRender & MaskRender, int Type, bool complement = false) override;
	int RenderWMaskChMult(const std::vector<const class GQuadRender*>& Ends, int Type);
	int RenderWMaskMult(const std::vector<const class GQuadRender*>& Rends, int Type, class MTMachineTool& MT);
	void WriteSTL(CStdioFile & f) const override;
	enum NCMApplicationType GetType() const;
	double GetVolume() const;
	float*** GetArray() const {return (float***)Array;}
    bool FindSegments(SnapRopes& oSegments) const override;
	static void ExtractTriangle(GLfloat* pRet, BPoint& P0, BPoint& P1, BPoint& P2);
	static void ExtractID(GLfloat* pC, int TriInd, ElemID& ID);
	int FillAllArraysCh(MQuadBody& Body) override;
protected:
	void FillCell(MQuadBody & Tree, int ix, int iy) override;
	static size_t FillBSPCell(MBSPCell& BSPCell, double Angle, const float* pCI, bool Sharp, bool Error);
	GLvoid * FillSimpleCell(const MQuadTree & Tree, int ix, int iy);
	GLvoid * FillNVArray(MBSPNode *pRoot, bool Smooth = true);
	void StoreTriangle(GLfloat *pRet, const PackedColor col,
		const BPoint & p0, const BPoint & n0,
		const BPoint & p1, const BPoint & n1,
		const BPoint & p2, const BPoint & n2);
public:
	const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
	static void SetDefaults(class NTiParams &Par);
	void SetArray(float ***A);
	bool MakeEnvelopeCont(std::list<BClrContour>& Contours, enum TurnGenMode TGMode) const override;
	void ActivateRendArray(double MaxEdgeLength);
	void ClearRendArray();
	RDCEL *GetRend(int ix, int iy) const;
	void SetRendArrayReady() { RendArrayReady = true; }
	float* MakeDCEL(class BDCEL_BSP& DCEL) const;
	UINT64 GetHASH(const GBodyRender* pEnd) const override;
	GBodyRender* MakeCopy() const override;
	static GQuadRenderMill* MakeCommonRender(int RInd, const std::vector<std::vector<GQuadRenderMill*>>& Renders);
	void Join(const GQuadRenderMill* pOther);
	void Clear();
protected:
	RDCEL ***RendArray;
	bool RendArrayReady;
};
