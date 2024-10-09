#pragma once

#include "MBSPTree.h"
#include "NContour.h"
#include "NLine.h"
#include "MFace.h"
#include ".\Plane\NPlaneProcessing.h"
#include "NHorPlaneProcessing.h"
#include ".\Plane\NCylinderProsessing.h"
#include "NMeshArray.h"
#include "NSortedArray.h"

class MBSPNode;
class NSortedArray;

class NOneCadrOneTool
{
	friend class NOnePartProcessing;
	friend class NPlaneProcessing;
	friend class NHorPlaneProcessing;
	friend class NCylinderProcessing;
public:
	NOneCadrOneTool(void);
	~NOneCadrOneTool(void);
	void SetPColorNum(int PNum);
	void Clear();
public:
	NSortedArray BaseAngles;
protected:
	NOnePartProcessing **pIntCont;
	NOnePartProcessing **pExtCont;
	NContour *	m_pToolsContour;// контур инструмента (указатель)
	NContour *	m_pBufContour;// Для ApproxContour

	NOneCadrProcessing *m_pCInfo;

	int iMStart;
	int iMEnd;

	double ConcaveRad;
	bool RadCCTool;
	bool ConcaveTool;
	bool CuttingTool;
	bool Shank;
	int PColorNum;
public:
	void SetToolContour(NContour * pContour, bool IsCutting, bool IsShank, bool IsConcave);
	void SetDataConcave(double CRad);
	bool CreateWConts(NOneCadrProcessing *pCadrInfo);
	int DefBaseAngles();
	void CrFaces4Trim(MeshArr &PlaneArray, bool StartFace);
	void CrBaseFaces(MeshArr &PlaneArray, bool StartFace);
	void MakeFullTool(MeshArr &PlaneArray);
	void CrBaseFace1(MeshArr &PlaneArray, bool StartFace, bool Ext) const;
	void TrimStartCont(const BPoint& C, const BPoint& N);
	void SetInitConts();
	void SwapConts();
	void BuildSurf2d(MeshArr &MeshArray);
	void BuildAddTurnMesh(MeshArr& MeshArray);
	void BuildSurf2dArc(MeshArr& MeshArray) const;
	void BuildSurf2dArcNew(MeshArr& MeshArray) const;
	void BuildSurf2dArcSmall(MeshArr &MeshArray, bool LastArcPart) const;
	void BuildSurf2dArcNearSmall(MeshArr &MeshArray, bool LastArcPart) const;
	void BuildSurfVert(MeshArr &MeshArray);
	void CrEndFaces(MeshArr &MeshArray);
	void BuildSurf3d(MeshArr &MeshArray);
	void BuildSurf3dCv(MeshsArr &MeshsArray);
	void BuildSurf3dBound(MeshArr &MeshArray) const;
	void FixIMStart(const MeshArr &MeshArray);
	void FixIMEnd(const MeshArr &MeshArray);
	void TrimStartFaces(MeshArr& MeshArray, const BPoint& C, const BPoint& N);
	void TrimAllFaces(MeshArr& MeshArray, const BPoint& C, const BPoint& N);
	void ApproxContour();
	void RestoreContour();
	bool IsConcaveTool() const { return ConcaveTool; }
};
