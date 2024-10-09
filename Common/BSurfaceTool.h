#pragma once
#include "BSurfaceElemTool.h"

//класс поверхности инструмента в начале/конце кадра
class AFX_EXT_CLASS BSurfaceTool//!!!
{
public:
	BSurfaceTool(void);
	~BSurfaceTool(void);
	void ModifyLists(BSurfaceElemTool * selem, int kol, const BMatrPair & MatrixOfTool, const NContour & Contour, const BGrazingCurve * bgc);
	bool CreateSurface(
						BGrazingCurve & bgc, 
						const BMatrPair & MatrixOfTool, 
						const BMatr & TotalDeriv,
						const NContour & Contour, 
						bool * direct,
						const bool f = true);
	void CreateSurfaceTri(	MeshArr *pMeshArr, 
						const BMatrPair & MatrixOfTool, 
						const NContour & Contour) const;
protected:
	BSurfaceElemTool *selem;
	bool * modify;
};
