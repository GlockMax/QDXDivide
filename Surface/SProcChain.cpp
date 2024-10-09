#include "stdafx.h"
#include "math.h"
#include <vector>
#include "ppltasks.h"
#include "NCMProject.h"
#include "ConstDef.h"
#include "MBSPForest.h"
#include "NCadrGeom.h"
#include "NToolStd.h"
#include "SOScrewO.h"
#include "SMachStateFix.h"
#include "NTriMesh.h"
#include "NRectMesh.h"
#include "MTMashineTool.h"
#include "NMeshArray.h"
#include "BGrazingCurve.h"
#include "BSurfaceTool.h"
#include "NToolCombined.h"
#include "NSurfSettings.h"
#include ".\Plane\NPlaneProcessing.h"
#include "NHorPlaneProcessing.h"
#include ".\Plane\NCylinderProsessing.h"
#include "NContourOfTool.h"
#include "BScrewMotion.h"
#include "NMachUnitPair.h"
#include "NCUnit.h"
#include "MTConfig.h"
#include "SProcChain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SProcChain::SProcChain(NMachUnitPair &MU, double Tol) : MUPair(MU), Path(MU, Tol)
{
	pTool = NULL;
}

SProcChain::~SProcChain(void)
{
}

bool SProcChain::IsDefined(void)
{
	return MUPair.GetMach().GetCurConfig().IsDefined() && Path.IsDefined() && (pTool != NULL);
}

INT_PTR SProcChain::SendCadrsArray(NCadrGeom * InArr, INT_PTR GSize, INT_PTR StartInd)
{
	Path.SendCadrs(&InArr[StartInd], int(GSize - StartInd), pTool == NULL ? 0 : pTool->Turret, pTool == NULL ? 0 : pTool->Attach);
	Path.Define();
	return Path.GetNCadrs();
}


bool SProcChain::CrMeshs(MeshsArr &ChMeshArr) const
{
//	return false;
	if(!pTool)
		return false;
	MBSPForest *pForest = &MBSPForest::GetActive();
	//CSingleLock singleLock(&Mach.GetCriticalSection());
	//singleLock.Lock();
	for(int iT = 0; iT < pTool->num_tools; ++iT)//для каждого простого инструмента
	{
		NTool *pSimTool = pTool->GetTool(iT);
		if(!pSimTool->IsCutting())
			continue;
		//TEMPORARY
		double OldRad = 0.;
		if (pSimTool->IsBallNose() && NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		{
			NToolStd *pToolStd = dynamic_cast<NToolStd *>(pSimTool);
			if (pToolStd)
			{
				OldRad = pToolStd->GetR();
				pToolStd->SetR(OldRad - 0.1);
			}
		}
		//END: TEMPORARY

		NContourOfTool Contour;
		pSimTool->CreateSimGeneratrix(pSimTool, Contour);
		if (Contour.num < 1)
			return false;
		//TEMPORARY
		if (OldRad != 0.)
		{
			NToolStd *pToolStd = dynamic_cast<NToolStd *>(pSimTool);
			if (pToolStd)
			{
				pToolStd->SetR(OldRad);
			}
		}
		//END: TEMPORARY

		if (Contour.HaveSphere() && NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP) // TEMPORARY
			continue;
		NContour C;
		NLine Line;
		// TEMPORARY
		if (!pSimTool->IsDiskTool())
			Contour.Remove(0);
		std::vector<int> C2Contour; // the number of Contour element for each C element
		C2Contour.reserve(Contour.num);
		for (int i = 0; i < Contour.num; i++)
		{
			Contour.GetLine(i, Line);
			if(Contour.Types[i] != TORAHS)
			{
				if(i == 0)
					C.AddFirst(Line);
				else
					C.AddFollow(Line);
				C.Types[C.num-1] = Contour.Types[i];
			}
			else
			{
				NLine L;
				BPoint A = Line.p[0], B;
				int kol = Line.GetNumAproxLine(GrazingCurveElemContour::EPS());
				for(int j = 1; j < kol; j++)
				{
					B = Line.GetPointFromParam(double(j)/double(kol));//конечная точка конуса на контуре тора для апроксимации конусами
					B.Norm();
					L.Set(A, (A + B)*0.5, (A + B)*0.5, B);
					C.Add(L);
					C.Types[C.num-1] = CONE;
					A = B;
					C2Contour.push_back(i);
				}
				B = Line.p[3];
				L.Set(A, (A + B)*0.5, (A + B)*0.5, B);
				C.Add(L);
				C.Types[C.num-1] = CONE;
			}
			C2Contour.push_back(i);
		}
		//Делаем конец конуса "не острым" (срезаем горизонталью)
		if(C.Types[C.num-1] == CONE)
		{
			C2Contour.push_back(-1);
			BPoint A = C.lns.GetAt(C.lns.GetSize() - 4);
			BPoint B = C.lns.GetAt(C.lns.GetSize()-1);
			BPoint E(0.0, 0.0, A.GetZ(), 1.0);
			double a = 0.01;
			BPoint O = B + (A-B)* (a/sqrt((B-E).D2()));
			C.lns.SetAt(C.lns.GetSize()-3, (A+O)*0.5);
			C.lns.SetAt(C.lns.GetSize()-2, (A+O)*0.5);
			C.lns.SetAt(C.lns.GetSize()-1, O);
			E.SetZ(O.GetZ());
			C.lns.Add((O+E)*0.5);
			C.lns.Add((O+E)*0.5);
			C.lns.Add(E);
			C.num++;
			C.Types.Add(HORIZONTAL);
		}
		MeshsArr* MeshsArrSet = new MeshsArr[Path.Motions.size()];
		using namespace concurrency;
		if (NCM_PROJECT.GetUseQuadTreeParallel())
		{
			std::vector<task<void>> tasks;
			tasks.reserve(Path.Motions.size());
			for (int iM = 0; iM < (int)Path.Motions.size(); ++iM)
			{
				tasks.push_back(create_task([&, iM] {CrMeshsOneMotion(iM, C, *pSimTool, MeshsArrSet[iM]); }));
			}
			auto joinTask = when_all(begin(tasks), end(tasks));
			// Wait for the tasks to finish.
			joinTask.wait();
		}
		else
		{
			for (int iM = 0; iM < (int)Path.Motions.size(); ++iM)
			{
				CrMeshsOneMotion(iM, C, *pSimTool, MeshsArrSet[iM]);
			}
		}
		for (int iM = 0; iM < (int)Path.Motions.size(); ++iM)
			ChMeshArr.Append(MeshsArrSet[iM]);
//		delete MeshsArrSet;
		//for (int iM = 0; iM < (int)Path.Motions.size(); ++iM)
		//{
		//	bool IsFast;
		//	bool ProcAsFast = Path.IsMotionFast(iM, IsFast);
		//	if (IsFast)
		//		continue;

		//	BScrewMotion::SMSpecialCase SpecCase = Path.Motions[iM].GetSpecialCase();
		//	int ChMeshArrSize = ChMeshArr.GetSize();

		//	if (SpecCase.PureRot || SpecCase.SamePlane)
		//		CrMesh4PureRotAndSamePlane(C, iM, ChMeshArr, pSimTool->IsDiskTool());
		//	else
		//		CrMesh4CommonCase(C, iM, ChMeshArr, pSimTool->IsDiskTool());	

		//	// Set fast color if necessary
		//	if (ProcAsFast)
		//	{
		//		for (int k = ChMeshArrSize; k < ChMeshArr.GetSize(); ++k)
		//			ChMeshArr.GetAt(k)->SetFastColor();
		//	}
		//}
		delete[] MeshsArrSet;
	}

//	singleLock.Unlock();
	return true;
}

void SProcChain::CrMeshsOneMotion(int iM, const NContour& C, const NTool& SimTool, MeshsArr& ChMeshArr) const
{
	bool IsFast;
	bool ProcAsFast = Path.IsMotionFast(iM, IsFast);
	if (IsFast)
		return;

	BScrewMotion::SMSpecialCase SpecCase = Path.Motions[iM].GetSpecialCase();
	int ChMeshArrSize = ChMeshArr.GetSize();

	if (SpecCase.PureRot || SpecCase.SamePlane)
		CrMesh4PureRotAndSamePlane(C, iM, ChMeshArr, SimTool);
	else
		CrMesh4CommonCase(C, iM, ChMeshArr, SimTool);

	// Set fast color if necessary
	if (ProcAsFast)
	{
		for (int k = ChMeshArrSize; k < ChMeshArr.GetSize(); ++k)
			ChMeshArr.GetAt(k)->SetFastColor();
	}
}

int SProcChain::CalcRotAxis(const BMatr &TotalDeriv, BPoint &P, BPoint &V)
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

void SProcChain::CrMesh4PureRotAndSamePlane(const NContour &C, int MotionIndex, MeshsArr &ChMeshArr, const NTool& SimTool) const
{
	CrMesh4CommonCase(C, MotionIndex, ChMeshArr, SimTool);
}

void SProcChain::CrMesh4CommonCase(const NContour &C, int MotionIndex, MeshsArr &ChMeshArr, const NTool& SimTool) const
{
	SMachState P(&MUPair.GetUnit().GetFixedDef());
	BStockToolInds STInds(Path.Cadrs[0]->GetStockPos(), 0, 0);
	if (pTool)
	{
		STInds.turret_ = pTool->Turret;
		STInds.attach_ = pTool->Attach;
	}
	BMTLinkage ConfNum;
	CSingleLock singleLock(&MUPair.GetMach().GetCriticalSection());
	singleLock.Lock();
	Path.GetSample(MUPair.GetMach(), MotionIndex, P, ConfNum, STInds);
	MUPair.GetMach().SetState(ConfNum, P);
	BMatrPair MatrixOfTool = MUPair.GetMach().GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
	singleLock.Unlock();
	const BScrewMotion &Motion = Path.Motions[MotionIndex];

	MeshArr *pMeshArr = new MeshArr;
	if (Motion.GetSpecialCase().Ortogonal)
	{// Convex tool can be processed only
		NContourOfTool CoT(*(const NContourOfTool*)(&C));
		CoT.CrVertMovCont(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + (Motion.GetAxisN() * MatrixOfTool.Inv()) * Motion.GetDist());
		CoT.MakeToolMeshs(*pMeshArr, MatrixOfTool.Or(), true, false);
		pMeshArr->SetConvex(true);
		ChMeshArr.Add(pMeshArr);
	}
	else
	{
		if (SimTool.IsDiskTool())
		{
			NContourOfTool CoT(*(const NContourOfTool*)(&C));
			int kLength = int(Motion.GetPathLength(Motion.GetFarthestPoint(pTool->GetGabar().Transform(MatrixOfTool.Or()))) / (5 * NSurfSettings::GetTolerance()));
			double Step = 1. / kLength;
			for (double t = Step; t < 1. + 0.5 * Step; t += Step)
			{
				CoT.MakeToolMeshs(*pMeshArr, MatrixOfTool.Or() * Motion.GetMatr(t), true, false);
				pMeshArr->SetConvex(!SimTool.IsConcave());
				ChMeshArr.Add(pMeshArr);
				pMeshArr = new MeshArr;
			}
			delete pMeshArr;
		}
		else
		{
			BMatr TotalDeriv = Motion.GetMatrDeriv(0.);

			int SamplesNum = 1;
			for (int j = 0; j < C.num; ++j)
			{
				NLine Line;
				C.GetLine(j, Line);
				int SN = Motion.GetNumAppr(Line.p[3] * MatrixOfTool.Or(), NSurfSettings::GetTolerance() * 0.5);
				SamplesNum = max(SamplesNum, SN);
			}
			++SamplesNum;

			//			 Make grazing curves
			BGrazingCurve *FirstCurve = new BGrazingCurve(C.num);
			if (!FirstCurve->FindGPNTool(MatrixOfTool, TotalDeriv, C))//ищем гр. точки простого инструмента
			{ // Grzing curve not found (may be tool does not move)
				delete FirstCurve;
				return;
			}
			FirstCurve->RemoveDegraded();
			FirstCurve->StoreInForest(MatrixOfTool.Or());

			BGrazingCurve *TempCurves[2];
			TempCurves[0] = FirstCurve;
			TempCurves[1] = NULL;

			//
			//	Make start surfaces	
			bool * direct = new bool[C.num];
			BSurfaceTool bSTStart;
			bSTStart.CreateSurface(*FirstCurve, MatrixOfTool, TotalDeriv, C, direct, false);//построение поверхности начала компоненты связности
			bSTStart.CreateSurfaceTri(pMeshArr, MatrixOfTool, C);//строим начало компоненты связности

			//	Make end surfaces	
			BGrazingCurve LastCurve(C.num);
			FirstCurve->BMakeCopy(LastCurve, Path.Motions[MotionIndex].GetMatr(1.));
			BMatr MotionMatr = Path.Motions[MotionIndex].GetMatr(1.);
			MatrixOfTool = MatrixOfTool.Or() * MotionMatr;
			TotalDeriv = MotionMatr.invr() * Path.Motions[MotionIndex].GetMatrDeriv(1.);//Считаем производную полной матрицы

			BSurfaceTool bSTEnd;
			bSTEnd.CreateSurface(LastCurve, MatrixOfTool, TotalDeriv, C, direct, true);//построение поверхности конца компоненты связности
			bSTEnd.CreateSurfaceTri(pMeshArr, MatrixOfTool, C);//строим начало компоненты связности

			// Make side surfaces
			auto CCNum = FirstCurve->GetConCompNum();
			for (int i = 0; i < CCNum; ++i)
			{
				NTriMesh *pMesh = new NTriMesh;
				pMeshArr->Add(pMesh);
			}


			int FirstInd = 0; // Ping pong index
			int LastInd = 1;
			for (int is = 1; is < SamplesNum; ++is)
			{
				if (is == SamplesNum - 1)
				{
					TempCurves[LastInd] = &LastCurve;
				}
				else
				{
					TempCurves[LastInd] = new BGrazingCurve(C.num);
					FirstCurve->MakeCopy(*TempCurves[LastInd]);
					//TempCurves[LastInd]->StoreInForest(MatrixOfTool.Or() * Path.Motions[iM].GetMatr(double(is) / (SamplesNum - 1)));
					FirstCurve->BMakeCopy(*TempCurves[LastInd], Path.Motions[MotionIndex].GetMatr(double(is) / (SamplesNum - 1)));
				}
				TempCurves[FirstInd]->ConnectCurves(*TempCurves[LastInd], pMeshArr, CCNum);
				FirstInd = (FirstInd + 1) % 2; // Ping pong index
				LastInd = (FirstInd + 1) % 2;
				if (is > 1)
					delete TempCurves[LastInd]; // very first curve should not be deleted
			}

			delete FirstCurve;
			FirstCurve = NULL;
			delete[] direct;
			ChMeshArr.Add(pMeshArr);
		}
	}
}