#pragma once
#include "SPathChain.h"
#include "NMachUnitPair.h"

class NToolCombined;
class MeshsArr;
class BGrazingCurve;
class NParamMesh;
class MTMachineTool;

class SProcChain
{
public:
	SProcChain(NMachUnitPair& MU, double Tol = 0.1);
	~SProcChain(void);
	bool IsDefined();
	void SetTool(const NToolCombined *pT) { pTool = pT; };
	INT_PTR SendCadrsArray(NCadrGeom * InArr, INT_PTR GSize, INT_PTR StartInd);
	bool CrMeshs(MeshsArr &ChMeshArr) const;
	void CrMeshsOneMotion(int iM, const class NContour& C, const NTool& SimTool, MeshsArr& ChMeshArr) const;
public:
	static int CalcRotAxis(const BMatr &TotalDeriv, BPoint &P, BPoint &V);
protected:
	SPathChain Path;
	NMachUnitPair& MUPair;
	const NToolCombined *pTool;
protected:
	void CrMesh4CommonCase(const class NContour &C, int MotionIndex, MeshsArr &ChMeshArr, const NTool& SimTool) const;
	void CrMesh4PureRotAndSamePlane(const class NContour &C, int MotionIndex, MeshsArr &ChMeshArr, const NTool& SimTool) const;

};
