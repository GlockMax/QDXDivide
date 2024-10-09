#pragma once
#include "NOnePartProsessing.h"
#include "NMeshArray.h"

class NRectMesh;

class NPlaneProcessing : public NOnePartProcessing
{
public:
	NPlaneProcessing(void);
	NPlaneProcessing(NLine &Line, NOneCadrProcessing *pCadrInfo, NOneCadrOneTool *pCadrInfoC)
		:NOnePartProcessing(Line,pCadrInfo,pCadrInfoC){NearSmall = false;}
	~NPlaneProcessing(void);

	void SetAngles(void) override;
	void SetInitCont(bool LastPart) override;
	void BuildMesh2d(MeshArr &MeshArray, bool Ext) override;
	void BuildMesh2dArc(MeshArr &MeshArray, bool Ext, bool PrevIsNearSmall = false) override;
	void BuildMesh3d(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart) override;
	void BuildMesh3dBound(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const override;
	MeshArr* BuildSolid3d(const NOnePartProcessing* pThisPart, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const override;
	NRectMesh * CreateAddTurnMesh() override;
	int Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const override;

	void SetNearSmall() { NearSmall = true;}

protected:
	bool NearSmall;
};
