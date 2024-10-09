#include "stdafx.h"
#include "ConstDef.h"
#include "NMeshArray.h"
#include "Math.h"
#include "NRectMesh.h"
#include "MBSPforest.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "nmakestdmesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NMakeStdMesh::NMakeStdMesh(void)
{
}

NMakeStdMesh::~NMakeStdMesh(void)
{
}

bool NMakeStdMesh::Cylinder(MeshArr & MeshArray, double  D, double H, double Tolerance)
{
	if( D < MIND || H < MIND || Tolerance < MIND)
		return false;


	auto StartInd = MeshArray.GetSize();
	MeshArray.SetSize(StartInd + 4);// 4 meshs will be added

// Prepare two bottom arcs
	int PointsNumber = 1 + (int(PI / (4. * acos(1. - 2. * Tolerance / D))) + 1) * 2;
	int *PointsInd0 = new int[PointsNumber];
	int *PointsInd1 = new int[PointsNumber];

	BMatr OneStepMatr0, OneStepMatr1;
	OneStepMatr0.RotZ(BPoint(0.,0.,0.,1.), 180./(PointsNumber - 1));
	OneStepMatr1.RotZ(BPoint(0.,0.,0.,1.), -180./(PointsNumber - 1));
	BPoint P0( D/2., 0., 0., 1.);
	BPoint P1(P0);

	PointsInd0[0] = MFOREST.AddPoints(P0);
	PointsInd1[0] = PointsInd0[0];
	P0 = P0 * OneStepMatr0;
	P1 = P1 * OneStepMatr1;
	for(int i = 1; i < PointsNumber-1; ++i)
	{
		PointsInd0[i] = MFOREST.AddPoints(P0);
		PointsInd1[i] = MFOREST.AddPoints(P1);
		P0 = P0 * OneStepMatr0;
		P1 = P1 * OneStepMatr1;
	}
	PointsInd0[PointsNumber-1] = MFOREST.AddPoints(P0);
	PointsInd1[PointsNumber-1] = PointsInd0[PointsNumber-1];

// Create bottom circle
	NRectMesh *pMeshB = new NRectMesh;
	pMeshB->SetSize(2, PointsNumber);
	pMeshB->m_CadrNum = -1;
	pMeshB->m_IsDeg = 2;
	pMeshB->m_sgn = -1;
	pMeshB->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(0.,0., 1.,0.), BPoint(0.,0.,0.,1.)));
	for(int iB = 0; iB < PointsNumber; ++iB)
	{
		pMeshB->m_arr[1][iB] = PointsInd0[iB];
		pMeshB->m_arr[0][iB] = PointsInd1[iB];
	}
// Create two halfs of the cylinder
	BPoint Tran( 0., 0., H, 0.);
	NRectMesh *pMesh0 = new NRectMesh;// 1st half of cylinder
	pMesh0->SetSize(2, PointsNumber);
	NRectMesh *pMesh1 = new NRectMesh;// 2nd half of cylinder
	pMesh1->SetSize(2, PointsNumber);
	for(int k0 = 0; k0 < PointsNumber; ++k0)
	{
		pMesh0->m_arr[0][k0] = PointsInd0[k0];
		pMesh1->m_arr[1][k0] = PointsInd1[k0];
	}
	PointsInd0[0] = MFOREST.AddPoints(*MFOREST.GetPoint(PointsInd0[0]) + Tran);
	PointsInd1[0] = PointsInd0[0];
	PointsInd0[PointsNumber-1] = MFOREST.AddPoints(*MFOREST.GetPoint(PointsInd0[PointsNumber-1]) + Tran);
	PointsInd1[PointsNumber-1] = PointsInd0[PointsNumber-1];
	for(int kC = 1; kC < PointsNumber-1; ++kC)
	{
		PointsInd0[kC] = MFOREST.AddPoints(*MFOREST.GetPoint(PointsInd0[kC]) + Tran);
		PointsInd1[kC] = MFOREST.AddPoints(*MFOREST.GetPoint(PointsInd1[kC]) + Tran);
	}
	for(int k1 = 0; k1 < PointsNumber; ++k1)
	{
		pMesh0->m_arr[1][k1] = PointsInd0[k1];
		pMesh1->m_arr[0][k1] = PointsInd1[k1];
	}
	pMesh0->m_CadrNum = -1;
	pMesh1->m_CadrNum = -1;
	pMesh0->m_IsDeg = 0;
	pMesh1->m_IsDeg = 0;
	pMesh0->m_sgn = -1;
	pMesh1->m_sgn = -1;
	pMesh0->m_iSurf = MFOREST.AddOrigSurf(new SOCyl( BPoint(0.,0.,0.,1.), BPoint(0.,0.,1.,0.), -D/2.));
	pMesh1->m_iSurf = pMesh0->m_iSurf;

// Top circle
	NRectMesh *pMeshT = new NRectMesh;
	pMeshT->SetSize(2, PointsNumber);
	pMeshT->m_CadrNum = -1;
	pMeshT->m_IsDeg = 2;
	pMeshT->m_sgn = -1;
	pMeshT->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(0., 0., -1., 0.), BPoint(0., 0., H, 1.)));
	for(int iT = 0; iT < PointsNumber; ++iT)
	{
		pMeshT->m_arr[0][iT] = PointsInd0[iT];
		pMeshT->m_arr[1][iT] = PointsInd1[iT];
	}

	MeshArray[StartInd + 0] = pMesh0;
	MeshArray[StartInd + 1] = pMesh1;
	MeshArray[StartInd + 2] = pMeshB;
	MeshArray[StartInd + 3] = pMeshT;
	delete[] PointsInd0;
	delete[] PointsInd1;
	return true;
}

bool NMakeStdMesh::BoxBounds(MeshArr & MeshArray, const BPoint &Min, const BPoint &Max)
{
	auto StartInd = MeshArray.GetSize();
	MeshArray.SetSize(StartInd + 6);// 6 meshs will be added

	int PointsNum[4][2];

	double dx = Max.GetX() - Min.GetX();
	double dy = Max.GetY() - Min.GetY();
	double dz = Max.GetZ() - Min.GetZ();

	PointsNum[0][0] = MFOREST.AddPoints(Min);
	PointsNum[1][0] = MFOREST.AddPoints(Min + BPoint(dx, 0., 0., 0.));
	PointsNum[2][0] = MFOREST.AddPoints(Min + BPoint(dx, dy, 0., 0.));
	PointsNum[3][0] = MFOREST.AddPoints(Min + BPoint(0., dy, 0., 0.));

	PointsNum[0][1] = MFOREST.AddPoints(Min + BPoint(0., 0., dz, 0.));
	PointsNum[1][1] = MFOREST.AddPoints(Min + BPoint(dx, 0., dz, 0.));
	PointsNum[2][1] = MFOREST.AddPoints(Min + BPoint(dx, dy, dz, 0.));
	PointsNum[3][1] = MFOREST.AddPoints(Min + BPoint(0., dy, dz, 0.));

	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = -1;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
		BPoint(0.,1.,0.,0.), *MFOREST.GetPoint(PointsNum[0][0])));

	pMesh->m_arr[1][1] = PointsNum[0][0];
	pMesh->m_arr[0][1] = PointsNum[1][0];
	pMesh->m_arr[0][0] = PointsNum[1][1];
	pMesh->m_arr[1][0] = PointsNum[0][1];
	MeshArray[StartInd + 0] = pMesh;

	pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = -1;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(-1.,0.,0.,0.), *MFOREST.GetPoint(PointsNum[2][1])));

	pMesh->m_arr[1][1] = PointsNum[1][0];
	pMesh->m_arr[0][1] = PointsNum[2][0];
	pMesh->m_arr[0][0] = PointsNum[2][1];
	pMesh->m_arr[1][0] = PointsNum[1][1];
	MeshArray[StartInd + 1] = pMesh;

	pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = -1;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(0.,-1.,0.,0.), *MFOREST.GetPoint(PointsNum[2][1])));

	pMesh->m_arr[1][1] = PointsNum[2][0];
	pMesh->m_arr[0][1] = PointsNum[3][0];
	pMesh->m_arr[0][0] = PointsNum[3][1];
	pMesh->m_arr[1][0] = PointsNum[2][1];
	MeshArray[StartInd + 2] = pMesh;

	pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = -1;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(1.,0.,0.,0.), *MFOREST.GetPoint(PointsNum[0][0])));

	pMesh->m_arr[1][1] = PointsNum[3][0];
	pMesh->m_arr[0][1] = PointsNum[0][0];
	pMesh->m_arr[0][0] = PointsNum[0][1];
	pMesh->m_arr[1][0] = PointsNum[3][1];
	MeshArray[StartInd + 3] = pMesh;

	pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = -1;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(0.,0.,1.,0.), *MFOREST.GetPoint(PointsNum[0][0])));

	pMesh->m_arr[1][1] = PointsNum[3][0];
	pMesh->m_arr[0][1] = PointsNum[2][0];
	pMesh->m_arr[0][0] = PointsNum[1][0];
	pMesh->m_arr[1][0] = PointsNum[0][0];
	MeshArray[StartInd + 4] = pMesh;

	pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = -1;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			BPoint(0.,0.,-1.,0.), *MFOREST.GetPoint(PointsNum[2][1])));

	pMesh->m_arr[1][1] = PointsNum[0][1];
	pMesh->m_arr[0][1] = PointsNum[1][1];
	pMesh->m_arr[0][0] = PointsNum[2][1];
	pMesh->m_arr[1][0] = PointsNum[3][1];
	MeshArray[StartInd + 5] = pMesh;
	return true;
}
bool NMakeStdMesh::BoxBounds(MeshArr & MeshArray, const BPoint *Cont, int ContSize, double Height)
{
	// Cont must be ccw oriented without equal points
	auto StartInd = MeshArray.GetSize();
	MeshArray.SetSize(StartInd + ContSize);// ContSize meshs will be added
	
	typedef int Pair[2];
	Pair *PointsNum = new Pair[ContSize];

	for(int i = 0; i < ContSize; ++i)
	{
		PointsNum[i][0] = MFOREST.AddPoints(Cont[i]);
		PointsNum[i][1] = MFOREST.AddPoints(Cont[i] + BPoint(0., 0., Height, 0.));
	}

	for(int k = 0; k < ContSize; ++k)
	{
		NRectMesh *pMesh = new NRectMesh;
		pMesh->SetSize(2, 2);
		pMesh->m_CadrNum = -1;
		pMesh->m_IsDeg = 0;
		pMesh->m_sgn = 1;
		BPoint N(Cont[(k + 1) % ContSize] - Cont[k]);
		N = N * BMatr().RotZ(BPoint(0.,0.,0.,1.), -90.) * (1. / sqrt(N.D2()));

		pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			N, *MFOREST.GetPoint(PointsNum[k][0])));

		pMesh->m_arr[1][0] = PointsNum[k][0];
		pMesh->m_arr[0][0] = PointsNum[(k + 1) % ContSize][0];
		pMesh->m_arr[0][1] = PointsNum[(k + 1) % ContSize][1];
		pMesh->m_arr[1][1] = PointsNum[k][1];
		MeshArray[StartInd + k] = pMesh;
	}
	delete [] PointsNum;
	return true;
}
