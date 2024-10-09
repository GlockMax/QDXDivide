#pragma once
#include "NGrooveChain.h"

class NToolCombined;
class MeshsArr;
class BGrazingCurve;

class MTMachineTool;

class NGrooveProcessing
{
public:
	NGrooveProcessing(class NMachUnitPair* pMU, double Tol = 0.1);
	~NGrooveProcessing(void);
	bool IsDefined();
	void SetTool(const NToolCombined *pT) { pTool = pT; };
	INT_PTR SendCadrsArray(NCadrGeom * InArr, INT_PTR GSize, INT_PTR  StartInd);
	bool CrMeshs(MeshsArr &ChMeshArr);
public:
	NGrooveChain Path;
	static int CalcRotAxis(const BMatr &TotalDeriv, BPoint &P, BPoint &V);
protected:
	class NMachUnitPair* pMUPair;
	const NToolCombined *pTool;
	int SamplesNum;

};
