#include "stdafx.h"
#include "plane\NOneCadrProcessing.h"
#include "NRectMesh.h"
#include "SOPlane.h"
#include "SOECyl.h"
#include "ConstDef.h"
#include "math.h"

#include ".\nhorplaneprocessing.h"

NHorPlaneProcessing::NHorPlaneProcessing(void)
{
	NearSmall = false;
}

NHorPlaneProcessing::~NHorPlaneProcessing(void)
{
}
int NHorPlaneProcessing::Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const
{
	BPoint n;
	if((Line.p[0] - m_pCInfo->CurLine.p[3]).D2() > (Line.p[3] - m_pCInfo->CurLine.p[3]).D2()) 
		n = BPoint(0.,0.,-1.,0.);
	else
		n = BPoint(0.,0., 1.,0.);

	return MFOREST.AddOrigSurf(
		new SOPlane(n, Line.p[0]));
}
void NHorPlaneProcessing::BuildMesh3d(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart)
{
	if(IsUpper())
		MeshArray.Add(NULL);
	else // Create arc mesh
		MeshArray.Add(CreateMeshFromArc(LastPart, pPrevPart));
// Add empty mesh
	MeshArray.Add(NULL);
}
bool NHorPlaneProcessing::IsUpper(void) const
{
	return 0 < fabs(m_Line.p[3].GetX()) - fabs(m_Line.p[0].GetX());
}
void NHorPlaneProcessing::BuildMesh3dBound(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const
{
	if(IsUpper())
	{
		if(m_pCInfo->m_CurDir.GetZ() < 0)
		{
			MeshArray.Add(CreateHorCircle(
				m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[3].GetZ(),0.),
				fabs(m_Line.p[3].GetX()),
				1.));
			MeshArray.Add(NULL);
		}
		else
		{
			MeshArray.Add(NULL);
			MeshArray.Add(CreateHorCircle(
				m_pCInfo->m_BPoint + m_pCInfo->m_CurDir + BPoint(0.,0.,m_Line.p[3].GetZ(),0.),
				fabs(m_Line.p[3].GetX()),
				1.));
		}
	}
	else 
	{
		if(m_pCInfo->m_CurDir.GetZ() < 0)
		{
			MeshArray.Add(NULL);
			MeshArray.Add(CreateHorCircle(
				m_pCInfo->m_BPoint + m_pCInfo->m_CurDir + BPoint(0.,0.,m_Line.p[0].GetZ(),0.),
				fabs(m_Line.p[0].GetX()),
				-1.));
		}
		else
		{
			MeshArray.Add(CreateHorCircle(
				m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[0].GetZ(),0.),
				fabs(m_Line.p[0].GetX()),
				-1.));
			MeshArray.Add(NULL);
		}
	}
}

MeshArr* NHorPlaneProcessing::BuildSolid3d(const NOnePartProcessing* pThisPart, bool LastPart, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const
{
	if (fabs(m_pCInfo->m_CurDir.GetZ()) < F_Z_HOR)
		return nullptr;
	if (IsUpper() != m_pCInfo->m_CurDir.GetZ() > 0)
		return nullptr;

	MeshArr* pRes = new MeshArr;
	double R0 = fabs(m_Line.p[0].GetX());
	double R1 = fabs(m_Line.p[3].GetX());
	if (R0 > R1)
	{
		const double b = R0; R0 = R1; R1 = b;
	}
	NRectMesh* pFirstMesh = CreateHorRing(m_pCInfo->m_BPoint + BPoint(0., 0., m_Line.p[0].GetZ(), 0.), R0, R1, IsUpper() ? 1. : -1.);

	// end mesh
	NRectMesh* pEndMesh = new NRectMesh;
	*static_cast<NParamMesh*>(pEndMesh) = *static_cast<NParamMesh*>(pFirstMesh);
	pEndMesh->m_IsDeg = pFirstMesh->m_IsDeg;
	pEndMesh->m_Closed = pFirstMesh->m_Closed;
	pEndMesh->SetSize(pFirstMesh->m_r, int(pFirstMesh->m_arr[0].GetSize()));
	BMatr Tran;
	Tran.Trans(m_pCInfo->m_CurDir);
	for (int i = 0; i < pFirstMesh->m_r; ++i)
		for (int j = 0; j < pFirstMesh->m_arr[0].GetSize(); ++j)
			pEndMesh->m_arr[i][j] = MFOREST.AddPoints((*MFOREST.GetPoint(pFirstMesh->m_arr[i][j])) * Tran);
	pEndMesh->m_sgn = -pFirstMesh->m_sgn;

	// new surface
	const SOrigSurf* pOldSurf = MFOREST.GetOrigSurf(pFirstMesh->m_iSurf);
	SOrigSurf* pNewSurf = pOldSurf->MakeCopy();
	pNewSurf->Transform(Tran);
	pNewSurf->Invert();
	pEndMesh->m_iSurf = MFOREST.AddOrigSurf(pNewSurf);

	// side meshs
	NRectMesh* pSideMesh1 = new NRectMesh;
	*static_cast<NParamMesh*>(pSideMesh1) = *static_cast<NParamMesh*>(pFirstMesh);
	pSideMesh1->SetSize(2, int(pFirstMesh->m_arr[0].GetSize() + ((pFirstMesh->m_r == 1) ? 1 : 0)));
	pSideMesh1->m_sgn = -pFirstMesh->m_sgn;
	pSideMesh1->m_iSurf = MFOREST.AddOrigSurf(new SOECyl(m_pCInfo->m_BPoint + BPoint(0., 0., m_Line.p[0].GetZ(), 0.)
		, m_pCInfo->m_CurDir * (1. / sqrt(m_pCInfo->m_CurDir.D2())), BPoint(0., 0., 1., 0), -R0));
	for (int j = 0; j < pFirstMesh->m_arr[0].GetSize(); ++j)
	{
		pSideMesh1->m_arr[0][j] = pFirstMesh->m_arr[0][j];
		pSideMesh1->m_arr[1][j] = pEndMesh->m_arr[0][j];
	}
	if (pFirstMesh->m_r == 1)
	{ 
		pSideMesh1->m_arr[0][pFirstMesh->m_arr[0].GetSize()] = pFirstMesh->m_arr[0][0];
		pSideMesh1->m_arr[1][pFirstMesh->m_arr[0].GetSize()] = pEndMesh->m_arr[0][0];
	}
	pRes->Add(pSideMesh1);

	if (pFirstMesh->m_r == 2)
	{
		NRectMesh* pSideMesh2 = new NRectMesh;
		*static_cast<NParamMesh*>(pSideMesh2) = *static_cast<NParamMesh*>(pFirstMesh);
		pSideMesh2->SetSize(2, int(pFirstMesh->m_arr[0].GetSize()));
		pSideMesh2->m_sgn = pFirstMesh->m_sgn;
		pSideMesh2->m_iSurf = MFOREST.AddOrigSurf(new SOECyl(m_pCInfo->m_BPoint + BPoint(0., 0., m_Line.p[3].GetZ(), 0.)
			, m_pCInfo->m_CurDir * (1. / sqrt(m_pCInfo->m_CurDir.D2())), BPoint(0., 0., 1., 0), R1));
		for (int j = 0; j < pFirstMesh->m_arr[0].GetSize(); ++j)
		{
			pSideMesh2->m_arr[0][j] = pFirstMesh->m_arr[1][j];
			pSideMesh2->m_arr[1][j] = pEndMesh->m_arr[1][j];
		}
		pRes->Add(pSideMesh2);
	}
	pRes->Add(pFirstMesh);// maybe it is better to place these meshes here
	pRes->Add(pEndMesh);
	return pRes;
}

void NHorPlaneProcessing::SetAngles(void)
{
	if(IsUpper())
	{
		TopAngle = 0;
		if(m_pCInfo->m_CurDir.GetZ() > 0)
			BotAngle = 180;
		else
			BotAngle = 0;
	}
	else
	{
		BotAngle = 0;
		if(m_pCInfo->m_CurDir.GetZ() > 0)
			TopAngle = 0;
		else
			TopAngle = 180;
	}
	if(!IsLeft())
	{
		TopAngle *= -1;
		BotAngle *= -1;
	}
}
