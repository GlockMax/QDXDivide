#pragma once
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "DXSubModel.h"
#include "DXMemory.h"
#include "B3DArray.h"
#include "DXBigCell5X.h"
#include "BSP_MODEL_API.h"
#include "BLinesDCEL.h"
#include "GBodyRender.h"

class DXCell5X;

class BSP_MODEL_API GQuadRenderDX5X :
	public GBodyRender
{
public:
	class TAArray : public B3DArray<DXBigCell5X *>
	{
	public:
		TAArray() : B3DArray<DXBigCell5X *>() {}
		~TAArray() {}
		void ResetChanged(const MQuadBody::Window &ModifWin, const class MCubeMask &ModifiedCells);
		void ClearAll(int nX, int nY, int nZ);
		void SetSizes(int nX, int nY, int nZ) override;
		int ClearEmpty(int nX, int nY, int nZ);
	};
protected:
	class RayMarch
	{
	public:
		RayMarch(const GQuadRenderDX5X *pRender, const BPoint &P, const BPoint & V);
		~RayMarch() {}
		bool GetNextBigCell(int &i, int &j, int &k);
	protected:
		BPoint Step;
		BPoint Start;
		double Size[3];
		int MaxInd[3];
		int CurInd[3];
	};
public:
	void FillPartRenders(const MQuadBody & Body, bool All);
	void Adjust(UINT Ind, UINT SharpInd, const DXBigCell5X& BigCell, const class DXMaps& BColumnArr, const class DXSubModel5X& Body5x, const TAArray& BigCells, BLinesDCEL& LinesDCEL) const;
	class RRopes* MakeRopes(const TAArray& BigCells, const DXMaps& BColumnArr, const DXSubModel5X& Body5X) const;
public:
	GQuadRenderDX5X(const class DXSubModel5X &Model);
	GQuadRenderDX5X(const GQuadRenderDX5X &inRender);
	virtual ~GQuadRenderDX5X(void);
	static GQuadRenderDX5X* CreateGQuadRenderDX5XList(const DXSubModel5X &Tree);
	static GQuadRenderDX5X* CreateListCopy(const GQuadRenderDX5X &inRender);
	void WriteSTL(CStdioFile& f) const override;
	size_t StoreSTL(GLfloat*& pStor) const;
	NCMApplicationType GetType() const override;
	int RenderWMaskCh(const GBodyRender & MaskRender, int Type, bool complement = false)  override { complement; Type; MaskRender; return 0; };
	double GetVolume() const { return 0.;}
	void TransModifArraysCh(GBodyRender & QinRender) override;
	int TransModifArraysCh(TAArray & inArray);
	int RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf) override;
	const class BPoint RayCasting(const class BPoint & P, const class BPoint & V, double MinW, class BPoint &N) const override;
	BPoint RayCasting(const class BPoint& P, const class BPoint& V, double MinW, class BPoint& N, BPoint& v0, BPoint& v1, BPoint& v2) const;
	MQuadBody::Window CalcWindow(const BBox & Bounds) const override;
	void GetCellIndexes(double x, double y, double z, int & ix, int & iy, int & iz) const;
protected:
	TAArray* pBigCells;
	int RowsNum;
	int ColsNum;
	int LevelsNum;
	BPoint MinPointShift;
	static int WholeCellsNum;
	static double SmoothAngle;
protected:
	bool Proper(const MQuadBody & Tree);
public:
	RTriRandMesh *GetBigCellMesh(int i, int j, int k) const;
	int GetGlobSize(void) const;
	int FillAllArraysCh(MQuadBody & Body) override;
	int FillModifArrays(MQuadBody & Tree) override;
	int FillModifArraysCh(MQuadBody & Tree) override;
    bool FindSegments(class SnapRopes& oSegments) const override;
	bool MakeEnvelopeCont(std::list<BClrContour>& Contours, enum TurnGenMode TGMode) const override;
	UINT64 GetHASH(const GBodyRender* pEnd) const override;
	float* MakeDCEL(class BDCEL_BSP& DCEL) const;
	int GetAreasNum() const;
	RTriRandMesh *GetArea(int Ind);
	const RTriRandMesh *GetArea(int Ind) const;
	static int GetWholeCellsNum();
	static void SetWholeCellsNum(int Val);
	static double GetSmoothAngle(void);
	static void SetSmoothAngle(double Angle);
	static const int CellsNumScale = 100000;
	int GetColsNum() const { return ColsNum; }
	int GetRowsNum() const { return RowsNum; }
	int GetLevelsNum() const { return LevelsNum; }
	GBodyRender* MakeCopy() const override;
	void DoZProc(int B0, int B1, class DXMaps& BColumnArr, const DXSubModel5X& Body5X, TAArray& NewBigCells, bool All);
	void DoYProc(int B1, class DXMaps& BColumnArr, const DXSubModel5X& Body5X, TAArray& NewBigCells, bool All);
	void DoXProc(int B0, class DXMaps& BColumnArr, const DXSubModel5X& Body5X, TAArray& NewBigCells, bool All);
};

