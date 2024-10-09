#pragma once
#include "BSurfaceElemTool.h"

//����� ����������� ����������� � ������/����� �����
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
