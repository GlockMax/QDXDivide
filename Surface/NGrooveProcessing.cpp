#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "MBSPForest.h"
#include "SOScrewO.h"
#include "SMachStateFix.h"
#include "NTriMesh.h"
#include "NRectMesh.h"
#include "MTMashineTool.h"
#include "NMeshArray.h"
#include "BGrazingCurve.h"
#include "NToolCombined.h"
#include "NSurfSettings.h"
#include ".\Plane\NPlaneProcessing.h"
#include "NHorPlaneProcessing.h"
#include ".\Plane\NCylinderProsessing.h"
#include "NContourOfTool.h"
#include "NToolTurnBlade.h"
#include "NCUnit.h"
#include "NMachUnitPair.h"
#include "MTConfig.h"
#include "NGrooveProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NGrooveProcessing::NGrooveProcessing(class NMachUnitPair* pMU, double Tol) : Path(Tol)
{
	pMUPair = pMU;
	pTool = NULL;
	SamplesNum = 0;
}

NGrooveProcessing::~NGrooveProcessing(void)
{
}

bool NGrooveProcessing::IsDefined(void)
{
	return pMUPair->GetMach().GetCurConfig().IsDefined() && Path.IsDefined() && (pTool != NULL);
}

INT_PTR NGrooveProcessing::SendCadrsArray(NCadrGeom * InArr, INT_PTR GSize, INT_PTR StartInd)
{
	double b[16] = {0, 1, 0, 0,// Перевод из XZ в XY (X->Y)
					0, 0, 1, 0,
					1, 0, 0, 0,
					0, 0, 0, 1};
	BMatr CorrMatr;
	CorrMatr.SetE();
	BMatr Mb(b);
	// Find orientation change cadr
	int Orient = 0; // 0 - undefined, 1 - raight, -1 - left
	auto i = StartInd;
	for(; i < GSize; ++i)
	{
		NCadrGeom *pGeom = InArr + i;
		if(pGeom->IsUndef())
			continue;


		SMachStateFix1 B, E;
		pGeom->CalcCurCoordMS(CorrMatr, 0., B);
		pGeom->CalcCurCoordMS(CorrMatr, 1., E);
		SMachState Bs(B, &pMUPair->GetUnit().GetFixedDef());
		SMachState Es(E, &pMUPair->GetUnit().GetFixedDef());
		BMatr TotalDeriv;
		pMUPair->GetMach().SetState(pGeom->GetMTLink(), Bs);
		pMUPair->GetMach().TotalDerivative(TotalDeriv, Bs, Es, 0.0, pMUPair->GetUnit().GetCurStockPosition(), pTool->Turret, pTool->Attach);//Считаем производную полной матрицы в начале кадра 
		//Mach.SetState(B);
		BMatr MatrixOfTool = pMUPair->GetMach().GetStock2ToolMatr(pMUPair->GetUnit().GetCurStockPosition(), pTool->Turret, pTool->Attach);

		BPoint Direction = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, BPoint(0, 0, 0, 1.));

		double Sp = Direction.GetY();
		if(fabs(Sp) < MINAR)
		{
			if(Orient == 0)// Flat cadr is first cadr
				i++;
			break;// Flat move cadr should be ignored
		}
		int NewOrient = (Sp > 0.) ? 1 : -1;
		if(Orient != NewOrient)
		{
			if(Orient == 0) // Is undefined yet
				Orient = NewOrient;
			else
				break;
		}
	}
	GSize = i;
	Path.SendCadrs(&InArr[StartInd], int(GSize - StartInd), Orient);
	Path.Define();
	return Path.GetNCadrs();
}


bool NGrooveProcessing::CrMeshs(MeshsArr &ChMeshArr)
{
	if(!pTool)
		return false;
	if(Path.GetOrient() == 0)
		return true;

	double b[16] = {0, 0, 1, 0,// Перевод из XY в XZ (Y->X)
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 0, 1};
	const BMatr Mb(b);

	SamplesNum = Path.GetSamplesNum();
	CSingleLock singleLock(&pMUPair->GetMach().GetCriticalSection());
	for(int iT = 0; iT < pTool->num_tools; ++iT)//для каждого простого инструмента
	{
		NTool *pSimTool = pTool->GetTool(iT);
		if(!pSimTool->IsCutting())
			continue;
		if(!pSimTool->IsKindOf(RUNTIME_CLASS(NToolTurnBlade)))
			continue;
		NToolTurnBlade *pToolBlade = (NToolTurnBlade *)pSimTool;
		const TBladeCont &OrigBladeCont = *pToolBlade->GetBladeCont();
		int BladeContSize = OrigBladeCont.GetEdgesNum();
		if(BladeContSize <= 0)
			continue;

		MeshArr *pMeshArr = new MeshArr;
		ChMeshArr.Add(pMeshArr);

		NRectMesh *pRectMesh = new NRectMesh;
		pRectMesh->SetSize(SamplesNum, BladeContSize);
		for(int is = 0; is < SamplesNum; ++is)
		{
			SMachStateFix1 P;
			Path.GetSample(is, P);
			pMUPair->GetMach().SetState(BMTLinkage().SetIndW1(), SMachState(P, &pMUPair->GetUnit().GetFixedDef()));
			BMatr MatrixOfTool = Mb * pMUPair->GetMach().GetStock2ToolMatr(pMUPair->GetUnit().GetCurStockPosition(), pTool->Turret, pTool->Attach);
			int k = 0;
			for(TBladeEdge *pCur = OrigBladeCont.GetStart(); pCur != NULL; pCur = OrigBladeCont.GetNext(pCur), ++k)
				pRectMesh->m_arr[is][k] = MFOREST.AddPoints(pCur->GetPoint() * MatrixOfTool);
		}
		NTriMesh *pTriMesh = new NTriMesh;
		pTriMesh->Reserve(BladeContSize * (pRectMesh->m_r - 1));
		for(int i = 0; i < pRectMesh->m_r - 1; ++i)
			for(int j = 0; j < BladeContSize; ++j)
			{
				int i0 = pRectMesh->m_arr[i][j];
				int i1 = pRectMesh->m_arr[i][(j + 1) % BladeContSize];
				int i2 = pRectMesh->m_arr[i + 1][(j + 1) % BladeContSize];
				int i3 = pRectMesh->m_arr[i + 1][j];
				pTriMesh->AddTri(i0, i1, i2);
				pTriMesh->AddTri(i0, i2, i3);
			}

		
		// Start and end cadr surface
		NTriMesh *pTriMeshS = new NTriMesh;
		NTriMesh *pTriMeshE = new NTriMesh;
		for(int j = 1; j < BladeContSize - 1; ++j)
		{
			pTriMeshS->AddTri(pRectMesh->m_arr[0][0], pRectMesh->m_arr[0][j + 1], pRectMesh->m_arr[0][j]);
			pTriMeshE->AddTri(pRectMesh->m_arr[pRectMesh->m_r - 1][0], pRectMesh->m_arr[pRectMesh->m_r - 1][j], pRectMesh->m_arr[pRectMesh->m_r - 1][j + 1]);
		}
		if(Path.GetOrient() < 0)
		{
			pTriMesh->Invert();
			pTriMeshS->Invert();
			pTriMeshE->Invert();
		}
		pMeshArr->Add(pTriMesh);
		pMeshArr->Add(pTriMeshS);
		pMeshArr->Add(pTriMeshE);
		delete pRectMesh;
	}
	return true;
}
int NGrooveProcessing::CalcRotAxis(const BMatr &TotalDeriv, BPoint &P, BPoint &V)
{
	double b[16] = {1., 0., 0., 1.,
			    0., 1., 0., 1.,
				0., 0., 1., 1.,
				0., 0., 0., 1.};
	BMatr Reper(b);
	BMatr R = Reper * TotalDeriv;
	BMatr A = R;
	for(int i = 0; i < 3; ++i)
		A.SetRow(i, A.GetRow(i) - A.GetRow(3));
	if(A.GetRow(0).D2() < MIND * MIND)
		V = Reper.GetRow(0) - Reper.GetRow(3);
	else if(A.GetRow(1).D2() < MIND * MIND)
		V = Reper.GetRow(1) - Reper.GetRow(3);
	else
		V = A.GetRow(0) % A.GetRow(1);
	double L2 = V.D2();
	if(L2 < MIND * MIND)
		return -1;
	V = V * (1. / sqrt(L2)); 
// Project V0, V1 to the V normal plane
	BPoint V0 = R.GetRow(0) - V * (R.GetRow(0) * V);
	BPoint V1 = R.GetRow(1) - V * (R.GetRow(1) * V);
	if(V0.D2() < MIND * MIND)
	{
		P = Reper.GetRow(0);
		return 0;
	}
	if(V1.D2() < MIND * MIND)
	{
		P = Reper.GetRow(1);
		return 0;
	}
// Find intersection between P0 V0 line and P1 V1 plane
	V0 = V0 * BMatr().rotg(90., BPoint(0., 0., 0., 1.), V);

	double z =	V0 * V1;
	if(fabs(z) < MINAR)
		return -1;
	z = 1./z;
	double t = ((Reper.GetRow(1) - Reper.GetRow(0)) * V1) * z;
	P = Reper.GetRow(0) + V0 * t;

	return 0;
}

