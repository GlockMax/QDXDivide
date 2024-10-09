#pragma once
#include "NCadrGeom.h"

class NToolCombinedTurn;
class TBladeCont;
class MeshsArr;

class NTurnProcessing
{
public:
	NTurnProcessing(void);
	~NTurnProcessing(void);
	bool SetTool(NToolCombinedTurn *pToolTurn = NULL);
	void SetCadrNum(int Num = -1);
	void SetTol(double T);
protected:
	NToolCombinedTurn *pTool;
	NCadrGeom Geom;
	int CadrNum;
	double Tol; // The tolerance for the arcs in trajectory and in tool contour
public:
	void CrCadrMeshs(MeshsArr &PlArray, const NCadrGeom &Geom, const BMatr &Mb);
	void CrCadrPartMeshs(MeshsArr & MeshsArray, const NCadrGeom *pCadrGeom, const BMatr &Mb);
	void CrOneBladeMeshs(MeshsArr & MeshsArray, const TBladeCont &Sweeped, NTool *pSimTool, bool Fast);
};
