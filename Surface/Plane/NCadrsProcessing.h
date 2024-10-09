#pragma once

#include "NOneCadrProcessing.h"

class NToolCombined;
class NCadrGeom;
class NContour;
class NTool;
class FChain;

typedef CTypedPtrArray <CPtrArray,NCadrGeom*> CadrArray;

class AFX_EXT_CLASS NCadrsProcessing
{
public:
	bool IsApproximated() const;
	bool IsSmall(const NCadrGeom *pGeom) const;
	bool IsLarge(const NCadrGeom *pGeom) const;
	bool IsNearSmall(const NCadrGeom *pGeom) const;
	void ApproximateFragment();

	bool MayBeApproximated(NCadrGeom *pGeom, int n);
	bool ReplaceRotateCadr(NCadrGeom *&pGeom);
	bool ApproxSpline(const NCadrGeom *pGeom);
	static bool TrimFlatMesh(NRectMesh *pMesh, const BPoint &C, const BPoint &N);
	static bool TrimFlatMeshWPos(NRectMesh* pMesh, const BPoint& C, const BPoint& N);
	static bool TrimRotMesh(NRectMesh *pMesh, const BPoint &C, const BPoint &N);
	int BuildSurf3d(MeshsArr &PlaneArray, int SInd, bool ChainEndBis);
	INT_PTR BuildSurf2d(MeshsArr& PlaneArray, INT_PTR SInd, bool ChainEndBis);
	INT_PTR BuildSurf2dArcNew(MeshsArr& PlaneArray, INT_PTR SInd, bool ChainEndBis);
	int BuildSurfVert(MeshsArr &PlaneArray, int CadrNum);
	void CrStartCont(int ChainStart);
	void FindMaxChain(FChain &CurChain) const;
	int GetFirstGeomCadr(int Start) const;
	bool CrChainMeshs(MeshsArr &PlArray, const FChain &CurChain);
	void Cr3dChainMeshs(MeshsArr &PlArray, const FChain &CurChain);
	void Cr2dChainMeshs(MeshsArr &PlArray, const FChain &CurChain);
	void Cr2dSArcChainMeshs(MeshsArr& PlArray, const FChain& CurChain);
	void Cr2dSArcChainMeshsNew(MeshsArr& PlArray, const FChain& CurChain);
	int BeginIssue(const NToolCombined *pT);
	NCadrsProcessing(void);
	~NCadrsProcessing(void);

protected:
	void Clear();
	BOOL CreateDirection(int index, NContour &tmpContour) const;
	void SavePlanes(MeshArr &arr);

	CadrArray Cadrs;
	bool ApproximatedFragment;
	double Rmax;
	const NToolCombined* pComTool;
	NOneCadrProcessing CadrProc;
	bool CutHelix;
public:
	INT_PTR SendCadrsArray(NCadrGeom *InArr, INT_PTR GSize, INT_PTR  StartInd);
	static double MaxRad(const BPoint & e0, const BPoint & P0, const BPoint & P1, const BPoint & P2);
	NCadrGeom *GetCadr(int i) const;
};
