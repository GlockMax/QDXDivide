#include "stdafx.h"
#include "nplaneprocessing.h"
#include "SOPlane.h"
#include "SOCone.h"
#include "SOCyl.h"
#include "SOECyl.h"
#include "SOTorus.h"
#include "NOneCadrProcessing.h"
#include "NOneCadrOneTool.h"
#include "NSurfSettings.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "math.h"
#include "ConstDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NPlaneProcessing::NPlaneProcessing(void) : NOnePartProcessing()
{
	NearSmall = false;
}

NPlaneProcessing::~NPlaneProcessing(void)
{
}

//////////////////////////////////////////////////////////////////////////
// «¿ƒ¿Õ»≈ Õ¿◊¿À‹ÕŒ√Œ  ŒÕ“”–¿
/*
void NPlaneProcessing::SetSContour(CUIntArray &arr, bool first)
{
	NLine Line = m_Line;
	if (m_pCInfo->m_IsFirstCadr)
		CreateInitContour(Line, arr);
	else
		CreateNextContour(arr);
}
*/
// Ì‡˜‡Î¸Ì˚È ÍÓÌÚÛ (ÔÂ‚˚È Í‡‰ ˆÂÔÓ˜ÍË)
/*
void NPlaneProcessing::CreateInitContour(NLine &Line, CUIntArray &arr)
{
	int n;
	BMatr m;
	MBSPForest *pForest = &MBSPForest::GetActive();

	m.RotZ(BPoint(0,0,0,1), m_pCInfo->m_BAlf + (m_pCInfo->m_IsTurnCW ? m_pCInfo->m_Alf: 0));
	Line = Line * m;

	m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
	Line = Line * m;
	
	arr.SetSize(2);
	m_arr[0].SetSize(2);
	
	n = pForest->AddPoints(Line.p[0]);
	arr[0] = n;
	m_arr[0][0] = n;

	n = pForest->AddPoints(Line.p[3]);
	arr[1] = n;
	m_arr[0][1] = n;
}
*/
// Ì‡˜‡Î¸Ì˚È ÍÓÌÚÛ (‰Îˇ ÓÒÚ‡Î¸Ì˚ı Í‡‰Ó‚ ÔÓ„‡ÏÏ˚)
/*
void NPlaneProcessing::CreateNextContour(CUIntArray &arr)
{
	m_arr[0].RemoveAll();
	m_arr[0].SetSize(2);
	m_arr[0][0] = arr[0];
	m_arr[0][1] = arr[1];
}
*/
//////////////////////////////////////////////////////////////////////////
// œŒ—“–Œ≈Õ»≈ —≈“Œ 
	/*
void NPlaneProcessing::BuildMesh(MeshArr &MeshArray, CUIntArray &e_cont)
{
	CUIntArray arr;

	// ÒÙÓÏËÓ‚‡Ú¸ ÍÓÌÂ˜Ì˚È ÍÓÌÚÛ 
	arr.Append(m_arr[0]);
	SetEContour(arr);
	m_arr[1].RemoveAll();
	m_arr[1].Append(arr);
	e_cont.Append(arr);

	// “Œ◊ ¿ 1 (ÌÓÏÂ ÚÓ˜ÍË ÔÓ ÔÓˇ‰ÍÛ ‚ ÍÓÌÚÛÂ)
	m_pCInfo->m_iP1 = arr.GetSize()-1;

	// ÔÓÒÚÓËÚ¸ ÒÂÚ¸
	NRectMesh *pMesh = GetParMesh();// EKn
	if(m_pCInfo->CheckMesh(pMesh))// EKn
		MeshArray.Add(pMesh);// EKn
	else
		AfxMessageBox("aaa");
		
// EKn	MeshArray.Add(GetParMesh());
}
*/
//////////////////////////////////////////////////////////////////////////
// «¿ƒ¿Õ»≈  ŒÕ≈◊ÕŒ√Œ  ŒÕ“”–¿
	/*
void NPlaneProcessing::SetEContour(CUIntArray &arr)
{
	int n;
	const BPoint *p;
	BPoint r;
	int k = arr.GetSize();
	MBSPForest *pForest = &MBSPForest::GetActive();

	for(int i=0; i<k; i++)
	{
		p = pForest->GetPoint(arr.GetAt(i));
		if (m_pCInfo->m_P5.IntersLine(*p, m_pCInfo->m_CurDir, r))
		{
			n = pForest->AddPoints(r);
			arr[i] = n;
		}
	}
}
	*/

//////////////////////////////////////////////////////////////////////////
// ¬Œ«¬–¿Ÿ≈Õ»≈ œ¿–¿Ã≈“–Œ¬
/*
NRectMesh* NPlaneProcessing::GetParMesh()
{
	return CreateParMesh(m_arr[0], m_arr[1], true);
}
*/
/*
CUIntArray &NPlaneProcessing::GetSContour()
{
	return m_arr[0];
}

CUIntArray &NPlaneProcessing::GetEContour()
{
	return m_arr[1];
}
*/
void NPlaneProcessing::SetInitCont(bool LastPart)
{
	BMatr m;
	NLine Line;

	m.RotZ(BPoint(0,0,0,1), m_pCInfo->m_BAlf + (m_pCInfo->m_IsTurnCW ? 0 : m_pCInfo->m_Alf));
	Line = m_Line * m;

	m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
	Line = Line * m;
	
	m_arr[0].SetSize(LastPart ? 2 : 1);
	m_arr[0][0] = MFOREST.AddPoints(Line.p[0]);
	if(LastPart)
		m_arr[0][1] = MFOREST.AddPoints(Line.p[3]);
}
void NPlaneProcessing::BuildMesh2d(MeshArr &MeshArray, bool Ext)
{
	MPlane *pPlane = & ( Ext ? m_pCInfo->m_P4 : m_pCInfo->m_P2);
	pPlane = (m_pCInfo->m_ChainEndBis ? pPlane : &m_pCInfo->m_P4);
	m_arr[1].RemoveAll();
	m_arr[1].Append(m_arr[0]);
	GetContByIntersecPlane(m_arr[1], 0, int(m_arr[1].GetSize() - 1), *pPlane);

	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetMeshID(m_pCInfoC->PColorNum);
	pMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pMesh->Shank = m_pCInfoC->Shank;
	pMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pMesh->SetRowsNum(2);
	pMesh->m_arr[0].Append(m_arr[0]);
	pMesh->m_arr[1].Append(m_arr[1]);
	pMesh->m_sgn = (Ext == m_pCInfo->IsExtLeft) ? -1 : 1;
	// Create surface
	BMatr m;
	m.e0cong(BPoint(0.,1.,0.,0.),m_pCInfo->m_CurDir);
	NLine Line = m_Line * m;
	BPoint N = m_pCInfo->m_CurDir % (Line.p[3] - Line.p[0]);
	m.Trans(BPoint(0.,0.,0.,1.),m_pCInfo->m_BPoint+m_pCInfo->m_CurDir);
	Line = Line * m;
	if(m_pCInfoC->RadCCTool && fabs((Line.p[0] - Line.p[3]).GetZ()) >= MIND)
	{
		BPoint CylCen = (Line.p[0] + Line.p[3]) * 0.5;
		double CRad = m_pCInfoC->ConcaveRad;
		double D2 = (Line.p[0] - Line.p[3]).D2();
		double L = sqrt(fabs(CRad * CRad - D2 * 0.25)); 
		BMatr Rot90;
		Rot90.rotg(-90. * pMesh->m_sgn, BPoint(0., 0., 0., 1.), m_pCInfo->m_CurDir);
		BPoint V((Line.p[3] - Line.p[0]) * Rot90);
		if(D2 >= MIND * MIND)
			CylCen = CylCen + V * (L / sqrt(D2));

		pMesh->m_iSurf = MFOREST.AddOrigSurf(
			new SOCyl(CylCen, m_pCInfo->m_CurDir * (1./sqrt(m_pCInfo->m_CurDir.D2())), CRad));
	}
	else
	{
		pMesh->m_iSurf = MFOREST.AddOrigSurf(
			new SOPlane(N * (-pMesh->m_sgn/sqrt(N.D2())),*MFOREST.GetPoint(m_arr[0][0])));
	}
	MeshArray.Add(pMesh);
	if(Ext)
	{
		pMesh = CreateAddTurnMesh();
		MeshArray.Add(pMesh);
	}
	Copy10();
}
void NPlaneProcessing::BuildMesh2dArc(MeshArr &MeshArray, bool Ext, bool PrevIsNearSmall/* = false*/)
{
	BMatr m;
	m.e0cong(BPoint(0.,1.,0.,0.),m_pCInfo->CurLine.GetDir(1));
	NLine Line = m_Line * m;
	m.Trans(BPoint(0.,0.,0.,1.),m_pCInfo->CurLine.p[3]);
	Line = Line * m;
	if(NearSmall)
	{
		NRectMesh *pMesh = new NRectMesh;
		pMesh->SetMeshID(m_pCInfoC->PColorNum);
		pMesh->SetRowsNum(1);
		pMesh->m_arr[0].Append(m_arr[0]);
		m_arr[1].RemoveAll();
		m_arr[1].Append(m_arr[0]);
		MeshArray.Add(pMesh);
		NearSmall = false;
	}
	else
	{
		NRectMesh *pMesh = CrDriveMesh();
		if (pMesh != nullptr)
		{
			if (PrevIsNearSmall)
			{
				m_arr[1][0] = m_arr[0][0];
				for (int i = 1; i < pMesh->m_r; ++i)
					pMesh->m_arr[i][0] = pMesh->m_arr[0][0];
			}

			Copy10();
			pMesh->m_sgn = Ext == m_pCInfo->IsExtLeft ? -1 : 1;
			// Create surface
			double sign = 1.;
			double R = sqrt((Line.p[0] - m_pCInfo->CurLine.GetCenter()).ProjXY().D2());
			if (R < m_pCInfo->CurLine.GetRad())
				sign = -1.;

			pMesh->m_iSurf = Cr2dRotSurf(Line, m_pCInfo->CurLine.GetCenter(), sign);
		}
		MeshArray.Add(pMesh);
	}
}
//NRectMesh * NPlaneProcessing::Create2dAngleMesh()
//{
//// œÓÒÚÓÂÌËÂ ÒÂÍÚÓ‡ "‰Ó‚ÓÓÚ‡"
//	if(!m_pCInfoC->m_pToolsContour)
//		return nullptr;
//	BPoint p = m_pCInfo->CurLine.GetDir(1);
//	double Angle = p.Angle(m_pCInfo->m_NextDir);
//	if(fabs(Angle) < MINAD)
//		return nullptr;
//	double Rmax = m_pCInfoC->m_pToolsContour->GetMaxX();
//	int n = NSurfSettings::GetNumAproxLine(Rmax, Angle);
//	NRectMesh *pParMesh = CreateRotMesh(m_arr[1], 0,int(m_arr[1].GetSize() - 1), m_pCInfo->CurLine.p[3], Angle, n);
//	pParMesh->m_IsDeg = 0;
//	return pParMesh;
//}
int NPlaneProcessing::Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const
{
	BPoint pr = Line.p[3] - Line.p[0];
	double sa2 = pr.D2();
	if(sa2 >= MINAR*MINAR)
		sa2 = pr.ProjXY().D2()/sa2;
		
	if( fabs(sa2) < MINAR*MINAR ) // Cylinder
	{
		double R = sqrt((Line.p[0]-Center).ProjXY().D2());
		return MFOREST.AddOrigSurf(
				new SOCyl(Center,
					BPoint(0.,0.,1.,0.),
					R * sign));
	}
	else // Cone
	{
		if(m_pCInfoC->RadCCTool)
		{
			double CRad = m_pCInfoC->ConcaveRad;
			double D2 = (Line.p[0] - Line.p[3]).D2();
			double L = sqrt(fabs(CRad * CRad - D2 * 0.25));
			double B = sqrt((Center - (Line.p[0] + Line.p[3]) * 0.5).ProjXY().D2());
			double dB = L * (Line.p[0] - Line.p[3]).GetZ();
			if(D2 < MIND * MIND)
				dB = 0.;
			else
				dB /= sqrt(D2);

			double Rl = B + dB * (sign < 0. ? -1. : 1.);
			double Rs = CRad;
			double s = ((Line.p[0] - Center).ProjXY().D2() < (Line.p[3] - Center).ProjXY().D2()) ? 1. : -1.;
			double dZ = sqrt(L * L - dB * dB);
			double Cz = ((Line.p[0] + Line.p[3]).GetZ() * 0.5 + s * sign * dZ);

			return MFOREST.AddOrigSurf(
					new SOTorus(Rl, Rs, 
						BPoint(Center.GetX(), Center.GetY(), Cz, 1.),
						BPoint(0.,0.,1.,0.)));
		}
		else
		{
			SOCone *pCone = new SOCone(Center,
						BPoint(0.,0.,1.,0.),
						Line.p[0], Line.p[3]);
			if(sign < 0.)
				pCone->InvertAngleSign();
			return MFOREST.AddOrigSurf(pCone);
		}
	}
}

void NPlaneProcessing::BuildMesh3d(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart)
{
// Create top mesh
// Create arc mesh
	if(fabs(TopAngle) < MINAD || fabs(fabs(TopAngle) - 180.) < MINAD) 
	{
		MeshArray.Add(CreateMeshFromArc(true, pPrevPart));
		MeshArray.Add(NULL);
		return;
	}
	NRectMesh *pMesh = CreateMeshFromArc(false, pPrevPart);
	MeshArray.Add(pMesh);
// Create generatrix
	BPoint StartPoint = m_pCInfo->m_CurDir;
	StartPoint.ProjXY();
	BPoint EndPoint = StartPoint * (fabs(m_Line.p[3].GetX())/sqrt(StartPoint.D2()));
	StartPoint = StartPoint * (fabs(m_Line.p[0].GetX())/sqrt(StartPoint.D2()));
	BMatr m;
	EndPoint = EndPoint * m.RotZ(BPoint(0.,0.,0.,1.), BotAngle);
	StartPoint = StartPoint * m.RotZ(BPoint(0.,0.,0.,1.), TopAngle);
	EndPoint = EndPoint + m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[3].GetZ(),0.) ;
	StartPoint = StartPoint + m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[0].GetZ(),0.) ;
	m_arr[0].RemoveAll();
	m_arr[0].Add( MFOREST.AddPoints(StartPoint));
// Create mesh 
	m_arr[1].RemoveAll();
	m_arr[1].Add(MFOREST.AddPoints(StartPoint + m_pCInfo->m_CurDir));
	NRectMesh *pBotMesh = new NRectMesh;
	pBotMesh->SetMeshID(m_pCInfoC->PColorNum);
	pBotMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pBotMesh->Shank = m_pCInfoC->Shank;
	pBotMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pBotMesh->SetRowsNum(2);
	pBotMesh->m_arr[0].Add(m_arr[0][0]);
	pBotMesh->m_arr[1].Add(m_arr[1][0]);
	if(LastPart)
	{
		m_arr[0].Add( MFOREST.AddPoints(EndPoint));
		m_arr[1].Add(MFOREST.AddPoints(EndPoint + m_pCInfo->m_CurDir));
		pBotMesh->m_arr[0].Add(m_arr[0][1]);
		pBotMesh->m_arr[1].Add(m_arr[1][1]);
	}
	pBotMesh->m_sgn = IsLeft() ? -1 : 1;
	BPoint N = m_pCInfo->m_CurDir % (StartPoint - EndPoint) * pBotMesh->m_sgn;
	pBotMesh->m_iSurf = MFOREST.AddOrigSurf( 
		new SOPlane(N * (1./sqrt(N.D2())), StartPoint));

	if(pMesh)// Join meshs
	{
//		pMesh->m_arr[0].SetAt(pMesh->m_arr[0].GetSize() - 1, m_arr[0][0]);
//		pMesh->m_arr[1].SetAt(pMesh->m_arr[1].GetSize() - 1, m_arr[1][0]);
		pMesh->m_arr[0].Add(m_arr[0][0]);
		pMesh->m_arr[1].Add(m_arr[1][0]);
	}
	MeshArray.Add(pBotMesh);
}
void NPlaneProcessing::BuildMesh3dBound(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const
{
	int EndInd = m_pCInfoC->BaseAngles.Find(180.);
	int MidInd = m_pCInfoC->BaseAngles.Find(fabs(TopAngle));
	int StartInd = m_pCInfoC->BaseAngles.Find(0.);
	NRectMesh* pFirstMesh = CreateMeshByBase(true, MidInd, EndInd);
	MeshArray.Add(pFirstMesh);
	NRectMesh* pLastMesh = CreateMeshByBase(false, MidInd, StartInd);
	MeshArray.Add(pLastMesh);
	if (pFirstMesh)
	{
		BMatr m;
		NLine Line = m_Line * m.Trans(BPoint(0., 0., 0., 1.), m_pCInfo->m_BPoint);
		pFirstMesh->m_iSurf = Cr2dRotSurf(Line, m_pCInfo->m_BPoint, 1.);
	}
	if (pLastMesh)
	{
		BMatr m;
		NLine Line = m_Line * m.Trans(BPoint(0., 0., 0., 1.), m_pCInfo->m_BPoint + m_pCInfo->m_CurDir);
		pLastMesh->m_iSurf = Cr2dRotSurf(Line, m_pCInfo->m_BPoint + m_pCInfo->m_CurDir, 1.);
	}
}

MeshArr* NPlaneProcessing::BuildSolid3d(const NOnePartProcessing* pThisPart, bool LastPart, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const
{
	if (fabs(m_Line.p[0].GetX()) < MIND && fabs(m_Line.p[3].GetX()) < MIND)
		return nullptr;
	
	MeshArr* pRes = new MeshArr;

	int EndInd = m_pCInfoC->BaseAngles.Find(180.);
	int MidInd = m_pCInfoC->BaseAngles.Find(fabs(TopAngle));
	int StartInd = m_pCInfoC->BaseAngles.Find(0.);
	int sign = 1;
	if (m_Line.p[3].GetZ() - m_Line.p[0].GetZ() > 0.)
	{
		StartInd = EndInd;
		sign = -1;
	}
	NRectMesh* pFirstMesh = CreateMeshByBase(true, MidInd, StartInd);
	if (pFirstMesh == nullptr)
		return nullptr;
	pFirstMesh->m_sgn *= sign;
	BMatr m;
	NLine Line = m_Line * m.Trans(BPoint(0., 0., 0., 1.), m_pCInfo->m_BPoint);
	pFirstMesh->m_iSurf = Cr2dRotSurf(Line, m_pCInfo->m_BPoint, -sign);

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

	pRes->Add(pEndMesh);

	// side meshs
	NRectMesh* pSideMesh1 = new NRectMesh;
	NRectMesh* pSideMesh2 = new NRectMesh;
	*static_cast<NParamMesh*>(pSideMesh1) = *static_cast<NParamMesh*>(pFirstMesh);
	pSideMesh1->SetSize(2, int(pFirstMesh->m_arr[0].GetSize()));
	pSideMesh1->m_sgn = -pFirstMesh->m_sgn;
	pSideMesh1->m_iSurf = MFOREST.AddOrigSurf(SOrigSurf::CreateECyl(
		m_pCInfo->m_BPoint + BPoint(0., 0., m_Line.p[0].GetZ(), 0.),
		m_pCInfo->m_CurDir * (1. / sqrt(m_pCInfo->m_CurDir.D2())),
		BPoint(0., 0., -pFirstMesh->m_sgn, 0),
		-((m_pCInfo->m_CurDir.GetZ() > 0.) ? 1. : -1.) * fabs(m_Line.p[0].GetX())));

	*static_cast<NParamMesh*>(pSideMesh2) = *static_cast<NParamMesh*>(pFirstMesh);
	pSideMesh2->SetSize(2, int(pFirstMesh->m_arr[0].GetSize()));
	pSideMesh2->m_sgn = pFirstMesh->m_sgn;
	pSideMesh2->m_iSurf = MFOREST.AddOrigSurf(SOrigSurf::CreateECyl(
		m_pCInfo->m_BPoint + BPoint(0., 0., m_Line.p[3].GetZ(), 0.),
		m_pCInfo->m_CurDir * (1. / sqrt(m_pCInfo->m_CurDir.D2())),
		BPoint(0., 0., pFirstMesh->m_sgn, 0),
		((m_pCInfo->m_CurDir.GetZ() > 0.) ? 1. : -1.) * fabs(m_Line.p[3].GetX())));
	//MElemIDProc::SetSurfID(&pSideMesh2->MeshID, pSideMesh2->m_iSurf);
	NRectMesh* pSideMeshL = nullptr;
	NRectMesh* pSideMeshR = nullptr;
	if (pFirstMesh->m_r == 2)
	{
		for (int j = 0; j < pFirstMesh->m_arr[0].GetSize(); ++j)
		{
			pSideMesh1->m_arr[0][j] = pFirstMesh->m_arr[0][j];
			pSideMesh1->m_arr[1][j] = pEndMesh->m_arr[0][j];
			pSideMesh2->m_arr[0][j] = pFirstMesh->m_arr[1][j];
			pSideMesh2->m_arr[1][j] = pEndMesh->m_arr[1][j];
		}
		if (!pFirstMesh->IsClosed())
		{
			pSideMeshL = new NRectMesh;
			*static_cast<NParamMesh*>(pSideMeshL) = *static_cast<NParamMesh*>(pFirstMesh);
			pSideMeshL->SetSize(2, 2);
			pSideMeshL->m_sgn = -pFirstMesh->m_sgn;
			pSideMeshL->m_arr[0][0] = pFirstMesh->m_arr[1][0];
			pSideMeshL->m_arr[0][1] = pFirstMesh->m_arr[0][0];
			pSideMeshL->m_arr[1][0] = pEndMesh->m_arr[1][0];
			pSideMeshL->m_arr[1][1] = pEndMesh->m_arr[0][0];
			pSideMeshL->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(pSideMeshL->CalcNormal(0, 0), *MFOREST.GetPoint(pSideMeshL->m_arr[0][0])));

			pSideMeshR = new NRectMesh;
			*static_cast<NParamMesh*>(pSideMeshR) = *static_cast<NParamMesh*>(pFirstMesh);
			pSideMeshR->SetSize(2, 2);
			pSideMeshR->m_sgn = -pFirstMesh->m_sgn;
			pSideMeshR->m_arr[0][0] = pFirstMesh->m_arr[0][pFirstMesh->m_arr[0].GetSize() - 1];
			pSideMeshR->m_arr[0][1] = pFirstMesh->m_arr[1][pFirstMesh->m_arr[0].GetSize() - 1];
			pSideMeshR->m_arr[1][0] = pEndMesh->m_arr[0][pFirstMesh->m_arr[0].GetSize() - 1];
			pSideMeshR->m_arr[1][1] = pEndMesh->m_arr[1][pFirstMesh->m_arr[0].GetSize() - 1];
			pSideMeshR->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(pSideMeshR->CalcNormal(0, 0), *MFOREST.GetPoint(pSideMeshR->m_arr[0][0])));
		}
	}
	pRes->Add(pSideMeshL);// L, R mast be before 1, 2
	pRes->Add(pSideMeshR);
	pRes->Add(pSideMesh1);
	pRes->Add(pSideMesh2);

	// equidistant
	pFirstMesh->MakeEquid(0.2);
	pFirstMesh->m_iSurf = ID_EMPTY;
	pRes->Add(pFirstMesh);

	// small mesh
	if (pFirstMesh->m_r == 2)
	{
		if (!pFirstMesh->IsClosed())
		{
			NRectMesh* pSmallMesh = new NRectMesh;
			*static_cast<NParamMesh*>(pSmallMesh) = *static_cast<NParamMesh*>(pFirstMesh);
			pSmallMesh->SetSize(2, int(pFirstMesh->m_arr[0].GetSize() * 2) + 1);
			pSmallMesh->m_sgn = -pFirstMesh->m_sgn;
			for (int j = 0; j < pFirstMesh->m_arr[0].GetSize(); ++j)
			{
				pSmallMesh->m_arr[0][j] = pFirstMesh->m_arr[0][j];
				pSmallMesh->m_arr[0][j + pFirstMesh->m_arr[0].GetSize()] = pFirstMesh->m_arr[1][pFirstMesh->m_arr[0].GetSize() - 1 - j];
			}
			pSmallMesh->m_arr[0][pFirstMesh->m_arr[0].GetSize() * 2] = pFirstMesh->m_arr[0][0];
			for (int j = 0; j < pSideMesh1->m_arr[0].GetSize(); ++j)
			{
				pSmallMesh->m_arr[1][j] = pSideMesh1->m_arr[0][j];
				pSmallMesh->m_arr[1][j + pFirstMesh->m_arr[0].GetSize()] = pSideMesh2->m_arr[0][pFirstMesh->m_arr[0].GetSize() - 1 - j];
			}
			pSmallMesh->m_arr[1][pFirstMesh->m_arr[1].GetSize() * 2] = pSmallMesh->m_arr[1][0];
			NParamMesh* pSmTriMesh = pSmallMesh->Convert2TriMesh();
			pSmTriMesh->m_iSurf = ID_EMPTY;
			pRes->Add(pSmTriMesh);
			delete pSmallMesh;
		}
		else
		{
			NRectMesh* pSmallMesh1 = new NRectMesh;
			*static_cast<NParamMesh*>(pSmallMesh1) = *static_cast<NParamMesh*>(pFirstMesh);
			pSmallMesh1->SetSize(2, int(pFirstMesh->m_arr[0].GetSize()));
			pSmallMesh1->m_sgn = -pFirstMesh->m_sgn;
			NRectMesh* pSmallMesh2 = new NRectMesh;
			*static_cast<NParamMesh*>(pSmallMesh2) = *static_cast<NParamMesh*>(pFirstMesh);
			pSmallMesh2->SetSize(2, int(pFirstMesh->m_arr[0].GetSize()));
			pSmallMesh2->m_sgn = pFirstMesh->m_sgn;
			for (int j = 0; j < pFirstMesh->m_arr[0].GetSize(); ++j)
			{
				pSmallMesh1->m_arr[0][j] = pFirstMesh->m_arr[0][j];
				pSmallMesh2->m_arr[0][j] = pFirstMesh->m_arr[1][j];
			}
			for (int j = 0; j < pSideMesh1->m_arr[0].GetSize(); ++j)
			{
				pSmallMesh1->m_arr[1][j] = pSideMesh1->m_arr[0][j];
				pSmallMesh2->m_arr[1][j] = pSideMesh2->m_arr[0][j];
			}
			NParamMesh* pSmTriMesh = pSmallMesh1->Convert2TriMesh();
			pSmTriMesh->m_iSurf = ID_EMPTY;
			pRes->Add(pSmTriMesh);
			delete pSmallMesh1;
			pSmTriMesh = pSmallMesh2->Convert2TriMesh();
			pSmTriMesh->m_iSurf = ID_EMPTY;
			pRes->Add(pSmTriMesh);
			delete pSmallMesh2;
		}
	}
	return pRes;

}

NRectMesh* NPlaneProcessing::CreateAddTurnMesh()
{
	BMatr m;
	m.e0cong(BPoint(0., 1., 0., 0.), m_pCInfo->CurLine.GetDir(1));
	NLine Line = m_Line * m;
	m.Trans(BPoint(0., 0., 0., 1.), m_pCInfo->CurLine.p[3]);
	Line = Line * m;
	NRectMesh* pMesh = Create2dAngleMesh();
	if (pMesh != nullptr)
	{
		pMesh->m_sgn = m_pCInfo->IsExtLeft ? -1 : 1;
		// Create surface
		pMesh->m_iSurf = Cr2dRotSurf(Line, m_pCInfo->CurLine.p[3], 1.);
	}
	return pMesh;
}
void NPlaneProcessing::SetAngles(void)
{
	double b;
	NOneCadrProcessing::ComputeAngles(m_Line, CONE, m_pCInfo->m_CurDir, TopAngle, b);
	if(IsLeft())
		TopAngle = 0.5*TopAngle;
	else
		TopAngle = -180. + 0.5*TopAngle;
	BotAngle = TopAngle;
}