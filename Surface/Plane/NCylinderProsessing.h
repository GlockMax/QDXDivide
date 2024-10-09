#pragma once
#include "NOnePartProsessing.h"
#include "NMeshArray.h"

class NCylinderProcessing : public NOnePartProcessing
{
public:
	NCylinderProcessing(void);
	NCylinderProcessing(NLine &Line, NOneCadrProcessing *pCadrInfo, NOneCadrOneTool *pCadrInfoC)
		:NOnePartProcessing(Line,pCadrInfo,pCadrInfoC){};
	~NCylinderProcessing(void);

	void SetInitCont(bool LastPart) override;
	void SetAngles(void) override;
	void BuildMesh2d(MeshArr &MeshArray, bool Ext) override;
	void BuildMesh2dArc(MeshArr &MeshArray, bool Ext, bool PrevIsNearSmall = false) override;
	void BuildMesh3d(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart) override;
	void BuildMesh3dBoundA(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart) const;
	void BuildMesh3dBound(MeshArr& MeshArray, bool LastPart, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const override;
	MeshArr* BuildSolid3d(const NOnePartProcessing* pThisPart, bool LastPart, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const override;
	int Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const override;
	NRectMesh * CreateMesh();
	NRectMesh * CreateAddTurnMesh() override;
	CUIntArray &GetSContour();
	CUIntArray &GetEContour();
	bool IsUpper(void);

protected:
	void CreateNextContour(CUIntArray &arr);
};
