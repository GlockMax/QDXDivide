#include "stdafx.h"
#include "NToolCombinedTurn.h"
#include ".\plane\NOnePartProsessing.h"
#include "MBSPForest.h"
#include "NRectMesh.h"
#include "NToolTurn.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "ConstDef.h"
#include "math.h"
#include "NSurfSettings.h"
#include "nturnprocessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NTurnProcessing::NTurnProcessing(void)
	: pTool(NULL)
{
	CadrNum = -1;
	Tol = 0.1;
}

NTurnProcessing::~NTurnProcessing(void)
{
}
void NTurnProcessing::CrCadrMeshs(MeshsArr &PlArray, const NCadrGeom &InGeom, const BMatr &Mb)
{
	if(!pTool)
		return;
	Geom = InGeom;
//	if(!Geom.IsInXYPlane())
	Geom.ProjXY();
	CrCadrPartMeshs(PlArray, &Geom, Mb);
}
bool NTurnProcessing::SetTool(NToolCombinedTurn *pToolTurn)
{
	if(!pToolTurn)
		return false;
	if(pToolTurn->num_tools <= 0)
		return false;
	pTool = pToolTurn;
	return true;
}
void NTurnProcessing::SetCadrNum(int Num)
{
	CadrNum = Num;
}

void NTurnProcessing::CrCadrPartMeshs(MeshsArr & MeshsArray, const NCadrGeom *pCadrGeom, const BMatr &Mb)
{// pCadrGeom must lie in the XY plane
	BMatr MbI(Mb.invr());
	for(int iTool = 0; iTool < pTool->num_tools; ++iTool)
	{
		NTool *pBTool = pTool->GetTool(iTool);
		if(!pBTool->IsKindOf(RUNTIME_CLASS(NToolTurn)))
			continue;
		NToolTurn *pSimTool = (NToolTurn *)pBTool;
		const TBladeCont *pCont = pSimTool->GetBladeCont();
		if(!pCont)
			continue;
		TBladeCont Sweeped[MAX_SWEEPS];
		int Col = pCont->Sweep(Sweeped, *pCadrGeom);
		BPoint P0Zaxis(BPoint(0., 0., 0., 1.) * Mb);
		BPoint EZaxis(BPoint(0., 0., 1., 0.) * Mb);
		BMatr SimZ(BMatr().rotg(-180., P0Zaxis, EZaxis));
		for(int i = 0; i < Col; ++i)
		{
			Sweeped[i].Trans(BMatr().Trans(BPoint(0.,0.,0.,1.), pCadrGeom->GetB()));
			int XSign = Sweeped[i].GetXSign(MbI);
			switch(XSign)
			{
			case 1:// whole contour is in positive x
				CrOneBladeMeshs(MeshsArray, Sweeped[i], pSimTool, pCadrGeom->IsFast());
				break;
			case -1:// whole contour is in negative x
				Sweeped[i].Trans(SimZ);
				CrOneBladeMeshs(MeshsArray, Sweeped[i], pSimTool, pCadrGeom->IsFast());
				break;
			case 0:// contour changes it's x sign
				CrOneBladeMeshs(MeshsArray, Sweeped[i], pSimTool, pCadrGeom->IsFast());
				Sweeped[i].Trans(SimZ);
				CrOneBladeMeshs(MeshsArray, Sweeped[i], pSimTool, pCadrGeom->IsFast());
				break;
			}
		}
	}
}
void NTurnProcessing::CrOneBladeMeshs(MeshsArr & MeshsArray,const TBladeCont &Sweeped, NTool *pSimTool, bool Fast)
{
	int ColorNum = 0;
	int ColorPassNum = 0;

	if(Fast)
	{
		ColorNum = FAST_COLOR_NUM;
		ColorPassNum = ColorNum;
	}
	else
	{
		ColorNum = MElemIDProc::AddPC(MElemIDProc::MakePC(pSimTool->GetColor())) + RESERVED_PCS;
		ColorPassNum = MElemIDProc::AddPC(MElemIDProc::MakePC(pSimTool->GetColorPassive())) + RESERVED_PCS;
	}

	// Start new mesh array
	MeshArr &MeshArray = *new MeshArr;
	MeshsArray.Add(&MeshArray);
	// Create meshs for one tool
	Sweeped.CrMesh4Turn(MeshArray, ColorNum, ColorPassNum, 0);
}
void NTurnProcessing::SetTol(double T)
{
	Tol = T;
}
