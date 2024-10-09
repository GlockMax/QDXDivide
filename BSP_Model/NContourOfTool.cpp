// NContourOfTool.cpp: implementation of the NContour class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "constDef.h"
#include "BScrewMotion.h"
#include "MBSPForest.h"
#include "BMatrPair.h"
#include "NLine.h"
#include "SOCyl.h"
#include "SOCone.h"
#include "SOPlane.h"
#include "SOSphere.h"
#include "SOTorus.h"
#include "NSurfSettings.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "NContourOfTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NContourOfTool::NContourOfTool() : NContour()
{
}

NContourOfTool::NContourOfTool(const NContourOfTool &c) : NContour(c)
{
}

NContourOfTool::~NContourOfTool()
{
}
int NContourOfTool::MakeToolMeshs(MeshArr &RMeshArr, const BMatr &ToolMatr, bool IsToolCutting, bool IsShank)
{
	double Rmax = GetMaxX();
	// Calc a number of sectors
	int Ns = NSurfSettings::GetNumAproxLine(Rmax, 360.);
	double As = 360. / Ns;
	int StartMeshN = RMeshArr.GetSize();
	RMeshArr.SetSize(StartMeshN + num);
	for(int i = 0; i < num; ++i)
	{
		NRectMesh *pMesh = new NRectMesh;
		pMesh->CuttingTool = IsToolCutting;
		pMesh->Shank = IsShank;
		pMesh->m_CadrNum = -1;
		pMesh->m_sgn = -1;
		pMesh->m_IsDeg = 0;
		pMesh->SetMeshID(MElemIDProc::ID_NULL);
		RMeshArr[StartMeshN + i] = pMesh;
	}
	CUIntArray *pPrevArr = NULL, *pCurArr = NULL, *pPrevArrB = NULL;
	for(int i = 0; i < num; ++i)
	{
		BPoint Pb = lns[i * 3];
		BPoint Pe = lns[i * 3 + 3];
		NRectMesh *pMesh = (NRectMesh *)RMeshArr[StartMeshN + i];
		switch(Types[i])
		{
		case HORIZONTAL:
			{
				bool First = (fabs(Pb.GetX()) < MIND);
				if(First || fabs(Pe.GetX()) < MIND)
				{
					pMesh->SetSize(1, Ns + 1);
					if(First)
					{
						pMesh->m_sgn *= -1;
						pPrevArrB = &pMesh->m_arr[0];
						pCurArr = NULL;
					}
					else
					{
						pPrevArrB = NULL;
						pCurArr = &pMesh->m_arr[0];
					}
				}
				else
				{
					pMesh->SetSize(2, Ns + 1);
					pPrevArrB = &pMesh->m_arr[1];
					pCurArr = &pMesh->m_arr[0];
				}
				if(pCurArr)
				{
					for(int k = 0; k < Ns; ++k)
						pCurArr->SetAt(k, MFOREST.AddPoints(Pb * BMatr().RotZ(0., 0., 0., k * As) * ToolMatr)); 
					pCurArr->SetAt(Ns, pCurArr->GetAt(0));
				}
				pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
					BPoint(0.,0.,1.,0.) * ToolMatr * pMesh->m_sgn,
					Pb * ToolMatr));
				break;
			}
		case CONE:
			pMesh->SetSize(2, Ns + 1);
			pPrevArrB = &pMesh->m_arr[1];
			pCurArr = &pMesh->m_arr[0];
			for(int k = 0; k < Ns; ++k)
				pCurArr->SetAt(k, MFOREST.AddPoints(Pb * BMatr().RotZ(0., 0., 0., k * As) * ToolMatr)); 
			pCurArr->SetAt(Ns, pCurArr->GetAt(0));
			// Add last point if this cone is last element
			if(fabs(Pe.GetX()) < MIND)
			{
				int LastP = MFOREST.AddPoints(Pe * ToolMatr);
				for(int k = 0; k < Ns + 1; ++k)
					pPrevArrB->SetAt(k, LastP);
			}
			// Create surfaces
			if(fabs(Pb.GetX() - Pe.GetX()) < MIND)
			{// Cylinder
				pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOCyl(
					BPoint(0., 0., 0., 1.) * ToolMatr,
					BPoint(0.,0.,1.,0.) * ToolMatr,
					Pb.GetX()));
			}
			else
			{// Cone
				pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOCone(
					BPoint(0., 0., 0., 1.) * ToolMatr,
					BPoint(0.,0.,1.,0.) * ToolMatr,
					Pb * ToolMatr,
					Pe * ToolMatr));
			}
			break;
		case TORAHS:
		case SPHERE:
			{
				NLine Line;
				GetLine(i, Line);
				int N = GetNumAproxLine(Line);
				pMesh->SetSize(N + 1, Ns + 1);
				pPrevArrB = &pMesh->m_arr[N];
				pCurArr = &pMesh->m_arr[0];
				for(int k = 0; k < Ns; ++k)
					pCurArr->SetAt(k, MFOREST.AddPoints(Pb * BMatr().RotZ(0., 0., 0., k * As) * ToolMatr)); 
				pCurArr->SetAt(Ns, pCurArr->GetAt(0));
				for(int j = 1; j < N; ++j)
				{
					BPoint Pt = Line.GetPointFromParam(double(j) / N).Norm();
					for(int k = 0; k < Ns; ++k)
						pMesh->m_arr[j].SetAt(k, MFOREST.AddPoints(Pt * BMatr().RotZ(0., 0., 0., k * As) * ToolMatr)); 
					pMesh->m_arr[j].SetAt(Ns, pMesh->m_arr[j].GetAt(0));
				}
				// Add last point if this cone is last element
				if(fabs(Pe.GetX()) < MIND)
				{
					int LastP = MFOREST.AddPoints(Pe * ToolMatr);
					for(int k = 0; k < Ns + 1; ++k)
						pPrevArrB->SetAt(k, LastP);
				}
				// Create surfaces
				BPoint C = Line.GetCenter();
				if(fabs(C.GetX()) < MIND)
				{// Sphere
					pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOSphere(
						Line.GetRad(),  
						C * ToolMatr));
				}
				else
				{// Tor
					pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOTorus(
						C.GetX(),
						Line.GetRad(),
						BPoint(0., 0., C.GetZ(), 1.) * ToolMatr,
						BPoint(0.,0.,1.,0.) * ToolMatr));
				}
			break;
			}
		}
		if(pPrevArr && pCurArr)
			for(int k = 0; k < pCurArr->GetSize(); ++k)
				pPrevArr->SetAt(k, pCurArr->GetAt(k));
		pPrevArr = pPrevArrB;
	}
	return 0;
}

int NContourOfTool::CalcSurf4Elem(const BMatrPair &Stock2Tool, const BScrewMotion &Motion, int *SurfNums) const
{
	// Make grazing curve (place contour to the appropriate position)
	// In the tool coordinate system
	BScrewMotion MotionInTool = Motion * Stock2Tool.Inv();
	for (int ElemInd = 0; ElemInd < num; ++ElemInd)
	{
		NLine Line;
		GetLine(ElemInd, Line);
//		TypeLineInContour Type = GetType(ElemInd);
		bool IsCurve = (Line.p[1].GetH() < 1. - HMIN);
		if (IsCurve)
		{// arc

		}
		else
		{// stright line
			BPoint LineV = Line.p[3] - Line.p[0];
			if (LineV * Motion.GetAxisN() < 0.)
				LineV *= -1.;
			BPoint ToolPoint(BPoint(0., 0., 0., 1.) * Stock2Tool.Or());
			BPoint ToolAxis(BPoint(0., 0., 1., 0.) * Stock2Tool.Or());
			if (ToolAxis * Motion.GetAxisN())
				ToolAxis *= -1;
			double ToolAxisA = ToolAxis.Angle0_180(Motion.GetAxisN());
			double LineA = LineV.Angle0_180(ToolAxis);
			double Angles[2];
			Angles[0] = fabs(ToolAxisA - LineA);
			Angles[1] = fabs(ToolAxisA + LineA);
			for (int AngNum = 0; AngNum < 2; ++AngNum)
			{
				if (Angles[AngNum] < MINAD)
				{// Cylinder

					double R = sqrt(ToolPoint.LineD2(Motion.GetAxisP(), Motion.GetAxisP() + Motion.GetAxisN()));
					double dR = fabs(Line.p[0].GetX());
					SurfNums[2 * ElemInd + 0] = MFOREST.AddOrigSurf(
						new SOCyl(Motion.GetAxisP(),
						Motion.GetAxisN(),
						-(R - dR)));//sign!!!
					SurfNums[2 * ElemInd + 1] = MFOREST.AddOrigSurf(
						new SOCyl(Motion.GetAxisP(),
						Motion.GetAxisN(),
						R + dR));//sign!!!
				}
				else if (fabs(Angles[AngNum] - 90.) < MINAD)
				{// Plane

				}
				else
				{// Cone

				}
			}
			//else // Cone
			//{
			//	if (m_pCInfoC->RadCCTool)
			//	{
			//		double CRad = m_pCInfoC->ConcaveRad;
			//		double D2 = (Line.p[0] - Line.p[3]).D2();
			//		double L = sqrt(fabs(CRad * CRad - D2 * 0.25));
			//		double B = sqrt((Center - (Line.p[0] + Line.p[3]) * 0.5).ProjXY().D2());
			//		double dB = L * (Line.p[0] - Line.p[3]).GetZ();
			//		if (D2 < MIND * MIND)
			//			dB = 0.;
			//		else
			//			dB /= sqrt(D2);

			//		double Rl = B + dB * (sign < 0. ? -1. : 1.);
			//		double Rs = CRad;
			//		double s = ((Line.p[0] - Center).ProjXY().D2() < (Line.p[3] - Center).ProjXY().D2()) ? 1. : -1.;
			//		double dZ = sqrt(L * L - dB * dB);
			//		double Cz = ((Line.p[0] + Line.p[3]).GetZ() * 0.5 + s * sign * dZ);
			//		//return MFOREST.AddOrigSurf(
			//		//		new SOTorus(Rl, Rs, Center,
			//		//			BPoint(0.,0.,1.,0.)));


			//		return MFOREST.AddOrigSurf(
			//			new SOTorus(Rl, Rs,
			//			BPoint(Center.GetX(), Center.GetY(), Cz, 1.),
			//			BPoint(0., 0., 1., 0.)));
			//	}
			//	else
			//	{
			//		SOCone *pCone = new SOCone(Center,
			//			BPoint(0., 0., 1., 0.),
			//			Line.p[0], Line.p[3]);
			//		if (sign < 0.)
			//			pCone->InvertAngleSign();
			//		return MFOREST.AddOrigSurf(pCone);
			//	}
			//}

		}
	}

	return -1;
}