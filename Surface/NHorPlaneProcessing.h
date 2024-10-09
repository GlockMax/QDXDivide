#pragma once
#include "plane\nplaneprocessing.h"

class NHorPlaneProcessing :
	public NPlaneProcessing
{
public:
	NHorPlaneProcessing(void);
	~NHorPlaneProcessing(void);
//	virtual NParamMesh * Create2dAngleMesh();
	virtual int Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const override;
	virtual void BuildMesh3d(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart) override;
	virtual void BuildMesh3dBound(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const override;
	MeshArr* BuildSolid3d(const NOnePartProcessing* pThisPart, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const override;
	virtual void SetAngles(void);
	bool IsUpper(void) const;
};
