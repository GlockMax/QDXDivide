#pragma once
#include "BSP_MODEL_API.h"
#include "TBladeEdge.h"
#include "NCadrGeom.h"
#include "NMeshArray.h"

const int MAX_SWEEPS = 128;

class NContour;
class NRectMesh;

class BSP_MODEL_API TBladeCont
{
public:
	TBladeCont(void);
	~TBladeCont(void);
	TBladeEdge *GetStart() const noexcept { return pStart;}
	const BPoint &GetNormal() const noexcept { return N;}
	void SetNormal(const BPoint &iN) noexcept { N = iN;}
	const BMatr GetNormMatr() const;
	void Trans(const BMatr &M);
	void CrMesh4Turn(MeshArr & MeshArray, int ColorNum, int ColorPassNum, int CadrNum) const;
	void CrMesh4Mill(MeshArr & MeshArray, const BPoint &V, int ColorNum, int ColorPassNum, int CadrNum) const;
	NRectMesh * CrMesh4TurnOneElem(const BMatr &Rot90, int ColorNum, int ColorPassNum, int CadrNum, const TBladeEdge *pCur, const NRectMesh *pPrevMesh) const;
	static NRectMesh * CrMesh4OneElem(const BMatr &Rot90, int ColorNum, int ColorPassNum, int CadrNum, const TBladeEdge *pCur
		, const NRectMesh *pPrevMesh, const BPoint &V, const BPoint& Normal);
	void SetConvex(bool Val) noexcept { Convex = Val; }
protected:
	TBladeEdge *pStart;
	BPoint N;// Normal
	bool Convex;
protected:
	bool AddGeomElem(const NCadrGeom *pGeom);

public:
	void Clear();
	bool SweepLine(const TBladeCont &Cont, const NCadrGeom &Geom);
	int SweepArc(const NCadrGeom &Geom, TBladeCont *Sweeped) const;
	static int SweepCircle(double r, const NCadrGeom &Geom, TBladeCont *Sweeped);
	int Sweep(TBladeCont *Res, const NCadrGeom &Geom) const;
	void CalcArc(const NCadrGeom &Geom, TBladeEdge *pNP, TBladeEdge *pPN);
	void Draw(void);
	void MakeMesh(class MeshArr& RMeshArr);
	TBladeEdge * GetNext(const TBladeEdge * pCur) const;
	void MakeCopy(const TBladeCont &In);
	bool IsCircle() const;
	bool MakeContour(const NCadrGeom * GeomArr, int Num);
	bool MakeContour(const NContour& Contour);
	void Invert(void);
	int GetXSign(const BMatr &Mb);
	double GetMinRad(void) const;
	int BreakArcs(void);
	int GlueArcs(void);
	void Remove(TBladeEdge * pEdge);
	const BBox GetGabar(void) const;
	int RemoveShortEdges(void);
	void Serialize(CArchive & ar);
	int GetEdgesNum(void) const;
	bool AddEdge(TBladeEdge * pEdge);
	void DrawLines(void);
	int FindExtrEdges(const BPoint & Dir, TBladeEdge *&pPN, TBladeEdge *&pNP);
	bool CircleSweep(const NCadrGeom & Geom, double r);
	bool HaveCutArc(void) const;
	void ReconCorners(double Angle);
	double LineArea() const;
};
