// NContourOfTool.h: interface for the NContour class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BSP_MODEL_API.h"
#include "NContour.h"
#include "NCenums.h"

class MeshArr;

class BSP_MODEL_API NContourOfTool : public NContour
{
public:
	NContourOfTool();
	NContourOfTool(const NContourOfTool &c);
	virtual ~NContourOfTool();

	int MakeToolMeshs(MeshArr &RMeshArr, const BMatr &ToolMatr, bool IsToolCutting, bool IsShank);
	int CalcSurf4Elem(const class BMatrPair &Stock2Tool, const class BScrewMotion &Motion, int *SurfNums) const;
};
