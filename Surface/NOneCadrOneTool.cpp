#include "stdafx.h"
#include ".\plane\NOneCadrProcessing.h"
#include ".\plane\NCadrsProcessing.h"
#include "NSurfSettings.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "SOPlane.h"
#include "FChain.h"
#include "ConstDef.h"
#include "math.h"
#include "NCMProject.h"
#include "nonecadronetool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NOneCadrOneTool::NOneCadrOneTool(void)
{
	pIntCont = NULL;
	pExtCont = NULL;


	m_pToolsContour = NULL;

	RadCCTool = false;
	ConcaveTool = false;
	CuttingTool = true;
	Shank = false;

	PColorNum = -1;
}

NOneCadrOneTool::~NOneCadrOneTool(void)
{
	if(!m_pToolsContour)
		return;
	Clear();
}

void NOneCadrOneTool::SetPColorNum(int PNum)
{
	PColorNum = PNum;
}

int NOneCadrOneTool::DefBaseAngles()
{
	if(!m_pToolsContour)
		return 0;

	double MaxToolRad = m_pToolsContour->GetMaxX();
	int num = NSurfSettings::
		GetNumAproxLine(MaxToolRad, 180.);
	double AngleStep = 180./num;
	BaseAngles.SetSize(num +1);
	for(int i = 0; i <= num; ++i)
		BaseAngles[i] = AngleStep * i;
	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		BaseAngles.Insert(fabs(pIntCont[k]->GetBotAngle()));
		BaseAngles.Insert(fabs(pIntCont[k]->GetTopAngle()));
	}
	return 0;
}


void NOneCadrOneTool::SetToolContour(NContour * pContour, bool IsCutting, bool IsShank, bool IsConcave)
{
	if(m_pToolsContour)
		Clear();

	m_pToolsContour = pContour;
	CuttingTool = IsCutting;
	Shank = IsShank;
	ConcaveTool = IsConcave;
}

void NOneCadrOneTool::SetDataConcave(double CRad)
{
	RadCCTool = true;
	ConcaveRad = CRad;
}

bool NOneCadrOneTool::CreateWConts(NOneCadrProcessing *pCadrInfo)
{// Create Int and Ext contours
	if(!m_pToolsContour)
		return false;

	m_pCInfo = pCadrInfo;

	pIntCont = new NOnePartProcessing *[m_pToolsContour->num];
	pExtCont = new NOnePartProcessing *[m_pToolsContour->num];
	BMatr m;
	m.Set(	-1.,  0.,  0.,  0.,
			 0., -1.,  0.,  0.,
			 0.,  0.,  1.,  0.,
			 0.,  0.,  0.,  1. );// Rotz 180
	int k = 0;
	for(int i = 0; i < m_pToolsContour->num; ++i)
	{
		NLine Line;
		m_pToolsContour->GetLine(i,Line);
		switch(m_pToolsContour->Types[i])
		{
		case HORIZONTAL:
			pIntCont[k] = new NHorPlaneProcessing;
			pExtCont[k] = new NHorPlaneProcessing;
			break;
		case CONE:
			pIntCont[k] = new NPlaneProcessing;
			pExtCont[k] = new NPlaneProcessing;
			break;
		case SPHERE:
			pIntCont[k] = new NCylinderProcessing;
			pExtCont[k] = new NCylinderProcessing;
			break;
		case TORAHS:
			pIntCont[k] = new NCylinderProcessing;
			pExtCont[k] = new NCylinderProcessing;
			break;
		default:
			continue;
			break;
		}
		pExtCont[k]->SetLine(Line, pCadrInfo, this);
		Line = Line * m;
		pIntCont[k]->SetLine(Line, pCadrInfo, this);
		++k;
	}
	m_pToolsContour->num = k;
	return true;
}

void NOneCadrOneTool::CrFaces4Trim(MeshArr& PlaneArray, bool StartFace)
{
	if(!m_pToolsContour)
		return;

	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetMeshID(PColorNum);
	PlaneArray.Add(pMesh);
	pMesh->SetRowsNum(1);
	CUIntArray buf;
	// Store ExtCont
	for (int i = 0; i < m_pToolsContour->num; ++i)
		pExtCont[i]->AppendTrimedArray(0, buf);
	// Invert buf and remoove first and last elements
	if (buf.GetSize() > 2)
	{
		pMesh->m_arr[0].SetSize(buf.GetSize() - 2);
		for (int i = 1; i < buf.GetSize() - 1; ++i)
			pMesh->m_arr[0][buf.GetSize() - 2 - i] = buf[i];
	}
	// Store IntCont
	for (int i = 0; i < m_pToolsContour->num; ++i)
		pIntCont[i]->AppendTrimedArray(0, pMesh->m_arr[0]);
	pMesh->m_sgn = m_pCInfo->IsExtLeft ? 1 : -1;
	pMesh->m_sgn *= StartFace ? -1 : 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
		m_pCInfo->m_CurDir * (-1./sqrt(m_pCInfo->m_CurDir.D2())) , m_pCInfo->m_BPoint));
	//// clear trimed state
	//if (!StartFace)
	//{
	//	for (int i = 0; i < m_pToolsContour->num; ++i)
	//	{
	//		pExtCont[i]->ClearTrimed();
	//		pIntCont[i]->ClearTrimed();
	//	}
	//}
}


void NOneCadrOneTool::CrBaseFaces(MeshArr &PlaneArray, bool StartFace)
{
	if(!m_pToolsContour)
		return;

	BPoint p = m_pCInfo->CurLine.GetDir(1);
	double Angle = p.Angle(m_pCInfo->m_NextDir) ;
	if(fabs(Angle) < MINAD)
	{
		NRectMesh *pMesh = new NRectMesh;
		pMesh->SetMeshID(PColorNum);
		PlaneArray.Add(pMesh);
		pMesh->SetRowsNum(1);
		CUIntArray buf;
		int i;
		// Store ExtCont
		for( i = 0; i < m_pToolsContour->num; ++i)
			buf.Append(pExtCont[i]->GetArray(0));
		// Invert buf and remove first and last elements
		pMesh->m_arr[0].SetSize(buf.GetSize() - 2);
		for( i = 1; i < buf.GetSize() - 1 ; ++i)
			pMesh->m_arr[0][buf.GetSize() - 2 - i] = buf[i];
		// Store IntCont
		for( i = 0; i < m_pToolsContour->num; ++i)
			pMesh->m_arr[0].Append(pIntCont[i]->GetArray(0));
		pMesh->m_sgn = m_pCInfo->IsExtLeft ? 1 : -1;
		pMesh->m_sgn *= StartFace ? -1 : 1;
		pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			p * (1./sqrt(p.D2())), m_pCInfo->CurLine.p[3]));
		return;
	}
	// Create two meshes
	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetMeshID(PColorNum);
	PlaneArray.Add(pMesh);
	pMesh->SetRowsNum(1);
	int i;
	// Store ExtCont
	for( i = 0; i < m_pToolsContour->num; ++i)
		pMesh->m_arr[0].Append(pExtCont[i]->GetArray(0));
	pMesh->m_sgn = m_pCInfo->IsExtLeft ? -1 : 1;
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
		m_pCInfo->m_NextDir, m_pCInfo->CurLine.p[3]));
	pMesh = new NRectMesh;
	pMesh->SetMeshID(PColorNum);
	PlaneArray.Add(pMesh);
	pMesh->SetRowsNum(1);
	// Store IntCont
	for( i = 0; i < m_pToolsContour->num; ++i)
		pMesh->m_arr[0].Append(pIntCont[i]->GetArray(0));
	pMesh->m_sgn = m_pCInfo->IsExtLeft ? 1 : -1;
	BPoint N = p * sqrt(m_pCInfo->m_NextDir.D2()) + m_pCInfo->m_NextDir * sqrt(p.D2());
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
		N * (1./sqrt(N.D2())), m_pCInfo->CurLine.p[3]));
}
void NOneCadrOneTool::MakeFullTool(MeshArr& MeshArray)
{
	if (!m_pToolsContour)
		return;

	NRectMesh* pExtPrev = nullptr;
	for (int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		pExtCont[k]->MakeFullTool(MeshArray, pIntCont[k]);
		NRectMesh* pMesh = (NRectMesh*)MeshArray.GetAt(MeshArray.GetSize() - 1);
		if (pExtPrev) //сшить сетки
			pExtPrev->JoinEdge(pMesh);
		pExtPrev = pMesh;
	}
}
void NOneCadrOneTool::CrBaseFace1(MeshArr &PlaneArray, bool StartFace, bool Ext) const
{
	if(!m_pToolsContour)
		return;

	// Create one mesh
	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetMeshID(PColorNum);
	PlaneArray.Add(pMesh);
	pMesh->SetRowsNum(1);
	NOnePartProcessing **pCont = Ext ? pExtCont : pIntCont;
	int i;
	// Store Cont
	for( i = 0; i < m_pToolsContour->num; ++i)
		pMesh->m_arr[0].Append(pCont[i]->GetArray(0));
	pMesh->m_sgn = (m_pCInfo->IsExtLeft == Ext) ? -1 : 1;
	if(Ext)
	{
		pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			m_pCInfo->m_NextDir, m_pCInfo->CurLine.p[3]));
	}
	else
	{
		BPoint p = m_pCInfo->CurLine.GetDir(1);
		BPoint N = p * sqrt(m_pCInfo->m_NextDir.D2()) + m_pCInfo->m_NextDir * sqrt(p.D2());
		pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(
			N * (1./sqrt(N.D2())), m_pCInfo->CurLine.p[3]));
	}
}
void NOneCadrOneTool::TrimStartCont(const BPoint& C, const BPoint& N)
{
	if (!m_pToolsContour)
		return;

	for (int i = 0; i < m_pToolsContour->num; ++i)
	{
		bool Last = i == m_pToolsContour->num - 1;
		pIntCont[i]->TrimStartCont(C, N, Last ? nullptr : pIntCont[i + 1]);
		pExtCont[i]->TrimStartCont(C, N, Last ? nullptr : pExtCont[i + 1]);
	}
}
void NOneCadrOneTool::SetInitConts()
{
	if(!m_pToolsContour)
		return;

	for(int i = 0; i < m_pToolsContour->num; ++i)
	{
		pIntCont[i]->SetInitCont(i == m_pToolsContour->num - 1);
		pExtCont[i]->SetInitCont(i == m_pToolsContour->num - 1);
	}
}
void NOneCadrOneTool::SwapConts()
{
	if(!m_pToolsContour)
		return;

	NOnePartProcessing **pBuf = pIntCont;
	pIntCont = pExtCont;
	pExtCont = pBuf;
}
void NOneCadrOneTool::BuildSurf2d(MeshArr &MeshArray)
{
	if(!m_pToolsContour)
		return;

	NRectMesh *pMesh;
	NRectMesh *pIntPrev = NULL, *pExtPrev0 = NULL, *pExtPrev1 = NULL;
	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		pIntCont[k]->BuildMesh2d(MeshArray, false);
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);
		if(pIntPrev) //сшить сетки
			pIntPrev->JoinEdge(pMesh);
		pIntPrev = pMesh;

		pExtCont[k]->BuildMesh2d(MeshArray, true);
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-2);
		if(pExtPrev0)//сшить сетки
			pExtPrev0->JoinEdge(pMesh);
		pExtPrev0 = pMesh;
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);
		if(pMesh)// может не быть
		{
			if(pExtPrev1)//сшить сетки
				pExtPrev1->JoinEdge(pMesh);
			pExtPrev1 = pMesh;
		}
		else
		{
			delete MeshArray[MeshArray.GetSize()-1];
			MeshArray.RemoveAt(MeshArray.GetSize()-1);
		}
	}
}

void NOneCadrOneTool::BuildAddTurnMesh(MeshArr& MeshArray)
{
	if (!m_pToolsContour)
		return;

	NRectMesh* pExtPrev = nullptr;
	for (int k = 0; k < m_pToolsContour->num; ++k)
	{
		NRectMesh* pMesh = pExtCont[k]->CreateAddTurnMesh();
		if (pMesh != nullptr)// может не быть
		{
			if (pExtPrev)//сшить сетки
				pExtPrev->JoinEdge(pMesh);
			MeshArray.Add(pMesh);
		}
		pExtPrev = pMesh;
	}
	for (int k = 0; k < m_pToolsContour->num; ++k)// copy m_arr[1] - > m_arr[0]
	{
		pIntCont[k]->Copy10();
		pExtCont[k]->Copy10();
	}
}

void NOneCadrOneTool::BuildSurf2dArcSmall(MeshArr &MeshArray, bool LastArcPart) const
{
	if(!m_pToolsContour)
		return;
	BPoint C = m_pCInfo->CurLine.GetCenter();
	double RArc = m_pCInfo->CurLine.GetRad();
	int NearSmallCount = 0;
	int SmallCount = 0;
	bool IsSmallInternal = (m_pCInfo->IsArcExtLeft == m_pCInfo->IsExtLeft);
	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		NPlaneProcessing *pPart = NULL;
		if(IsSmallInternal)
			pPart = (NPlaneProcessing *)pIntCont[k];
		else
			pPart = (NPlaneProcessing *)pExtCont[k];
		if(fabs(pPart->GetLine().p[0].GetX()) > RArc - 1.e4 * MIND)
			++SmallCount;
		BMatr m;
		m.RotZ(BPoint(0,0,0,1), m_pCInfo->m_BAlf + (m_pCInfo->m_IsTurnCW != IsSmallInternal ? 0 : m_pCInfo->m_Alf));
		NLine Line = pPart->GetLine() * m;
		m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
		Line = Line * m;
		double Eps2 = 1.e8 * DMIN * DMIN;
		if((Line.p[0] - C).ProjXY().D2() < Eps2 && (Line.p[3] - C).ProjXY().D2() < Eps2)
		{
			++NearSmallCount;
			pPart->SetNearSmall();
		}
	}
	if(NearSmallCount == 1)
	{
		BuildSurf2dArcNearSmall(MeshArray, LastArcPart);
		return;
	}
	if(SmallCount == 0)
	{
		BuildSurf2dArc(MeshArray);
		return;
	}

	NRectMesh *pMesh;
	NRectMesh *pIntPrev = NULL, *pExtPrev0 = NULL, *pExtPrev1 = NULL;
	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		pIntCont[k]->BuildMesh2dArc(MeshArray, false);
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);
		if(pIntPrev) //сшить сетки
		{
			pIntPrev->JoinEdge(pMesh);
			if(IsSmallInternal)
				NCadrsProcessing::TrimRotMesh(pIntPrev, C, C - m_pCInfo->CurLine.p[0]);
		}
		pIntPrev = pMesh;

		pExtCont[k]->BuildMesh2dArc(MeshArray, true);
		pMesh = pExtCont[k]->CreateAddTurnMesh();
		MeshArray.Add(pMesh);
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-2);
		if(pExtPrev0)//сшить сетки
		{
			pExtPrev0->JoinEdge(pMesh);
			if(!IsSmallInternal)
				NCadrsProcessing::TrimRotMesh(pExtPrev0, C, C - m_pCInfo->CurLine.p[0]);
		}
		pExtPrev0 = pMesh;
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);//Additional rotation surf
		if(pMesh != nullptr)// может не быть
		{
			if(pExtPrev1)//сшить сетки
				pExtPrev1->JoinEdge(pMesh);
			pExtPrev1 = pMesh;
		}
		else
		{
			MeshArray.RemoveAt(MeshArray.GetSize() - 1);
		}
	}
	if(m_pCInfo->CurChainType != CHAIN_2D_SMALL_FULL_ARC && LastArcPart)
	{
		// Create additional internal flat surf
		CrBaseFace1(MeshArray, false, !IsSmallInternal);
		NRectMesh *pEMesh = (NRectMesh *)MeshArray[MeshArray.GetSize()-1];
		NCadrsProcessing::TrimFlatMesh(pEMesh, C, m_pCInfo->CurLine.p[3] - C);
		pEMesh->m_sgn *= -1;
		for(int k = 0; k < m_pToolsContour->num; ++k)// copy m_arr[1] - > m_arr[0]
		{
			pIntCont[k]->Copy10();
			pExtCont[k]->Copy10();
		}
	}
	if(pIntPrev)
	{
		if(IsSmallInternal)
			NCadrsProcessing::TrimRotMesh(pIntPrev, C, C - m_pCInfo->CurLine.p[0]);
	}
	if(pExtPrev0)
	{
		if(!IsSmallInternal)
			NCadrsProcessing::TrimRotMesh(pExtPrev0, C, C - m_pCInfo->CurLine.p[0]);
	}
}

void NOneCadrOneTool::BuildSurf2dArcNearSmall(MeshArr &MeshArray, bool LastArcPart) const
{
	if(!m_pToolsContour)
		return;

	BPoint C = m_pCInfo->CurLine.GetCenter();
	bool IsNearSmallInternal = (m_pCInfo->IsArcExtLeft == m_pCInfo->IsExtLeft);

	NRectMesh *pMesh;
	NRectMesh *pIntPrev = NULL, *pExtPrev0 = NULL, *pExtPrev1 = NULL;
	bool NearSmallPrev = false;
	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		bool PrevIsNearSmall = false;
		if(pIntPrev)
			PrevIsNearSmall = (pIntPrev->m_r == 1);
		pIntCont[k]->BuildMesh2dArc(MeshArray, false, PrevIsNearSmall);
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);
		if(pMesh->m_r == 1 && pMesh->m_arr[0].GetSize() == 1) // NearSmall
			pMesh->m_arr[0].Add(pIntCont[k + 1]->GetFirstPN());

		if(pIntPrev) //сшить сетки
		{
			if(pIntPrev->m_r == 1) // NearSmall
			{
				for(int i = 0; i < pMesh->m_r; ++i)
					pMesh->m_arr[i][0] = pIntPrev->m_arr[0][1];
			}
			pIntPrev->JoinEdge(pMesh);
			//if(m_pCInfo->IsArcExtLeft == m_pCInfo->IsExtLeft)
			//	NCadrsProcessing::TrimRotMesh(pIntPrev, C, C - m_pCInfo->CurLine.p[0]);
		}
		pIntPrev = pMesh;

		PrevIsNearSmall = false;
		if(pExtPrev0)
			PrevIsNearSmall = (pExtPrev0->m_r == 1);
		pExtCont[k]->BuildMesh2dArc(MeshArray, true, PrevIsNearSmall);
		pMesh = pExtCont[k]->CreateAddTurnMesh();
		MeshArray.Add(pMesh);
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-2);
		if(pMesh->m_r == 1 && pMesh->m_arr[0].GetSize() == 1) // NearSmall
			pMesh->m_arr[0].Add(pExtCont[k + 1]->GetFirstPN());
		if(pExtPrev0)//сшить сетки
		{
			pExtPrev0->JoinEdge(pMesh);
			//if(!IsNearSmallInternal)
			//	NCadrsProcessing::TrimRotMesh(pExtPrev0, C, C - m_pCInfo->CurLine.p[0]);
		}
		NRectMesh *pExtPrevB = pExtPrev0;
		pExtPrev0 = pMesh;
		pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);//Additional rotation surf
		if(pMesh)// может не быть
		{
			if(pExtPrevB)
				if(pExtPrevB->m_r == 1) // NearSmall
					pMesh->m_arr[0][0] = pExtPrevB->m_arr[0][1];
			if(pExtPrev1)//сшить сетки
				pExtPrev1->JoinEdge(pMesh);
			pExtPrev1 = pMesh;
		}
		else
		{
			delete MeshArray[MeshArray.GetSize()-1];
			MeshArray.RemoveAt(MeshArray.GetSize()-1);
		}
	}
	//if(m_pCInfo->CurChainType != CHAIN_2D_SMALL_FULL_ARC && m_pCInfo->CurChainType != CHAIN_2D_NEARSMALL_ARC && LastArcPart)
	//{
	//	// Create additional internal flat surf
	//	CrBaseFace1(MeshArray, false, m_pCInfo->IsArcExtLeft != m_pCInfo->IsExtLeft);
	//	NRectMesh *pEMesh = (NRectMesh *)MeshArray[MeshArray.GetSize()-1];
	//	NCadrsProcessing::TrimFlatMesh(pEMesh, C, m_pCInfo->CurLine.p[3] - C);
	//	pEMesh->m_sgn *= -1;
	//}
	if(m_pCInfo->CurChainType != CHAIN_2D_SMALL_FULL_ARC && LastArcPart)
	{
		for(int k = 0; k < m_pToolsContour->num; ++k)// copy m_arr[1] - > m_arr[0]
		{
			pIntCont[k]->Copy10();
			pExtCont[k]->Copy10();
		}
	}
	//if(pIntPrev && IsNearSmallInternal)
	//		NCadrsProcessing::TrimRotMesh(pIntPrev, C, C - m_pCInfo->CurLine.p[0]);
	//if(pExtPrev0 && !IsNearSmallInternal)
	//		NCadrsProcessing::TrimRotMesh(pExtPrev0, C, C - m_pCInfo->CurLine.p[0]);
}

void NOneCadrOneTool::BuildSurf2dArc(MeshArr &MeshArray) const
{
	if(!m_pToolsContour)
		return;

	NRectMesh *pIntPrev = NULL, *pExtPrev0 = NULL, *pExtPrev1 = NULL;
	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		pIntCont[k]->BuildMesh2dArc(MeshArray, false);
		NRectMesh* pMesh = (NRectMesh*)MeshArray.GetAt(MeshArray.GetSize() - 1);
		if(pIntPrev) //сшить сетки
			pIntPrev->JoinEdge(pMesh);
		pIntPrev = pMesh;

		pExtCont[k]->BuildMesh2dArc(MeshArray, true);
		pMesh = pExtCont[k]->CreateAddTurnMesh();
		MeshArray.Add(pMesh);
		pMesh = (NRectMesh*)MeshArray.GetAt(MeshArray.GetSize() - 2);
		if(pExtPrev0)//сшить сетки
			pExtPrev0->JoinEdge(pMesh);
		pExtPrev0 = pMesh;
		pMesh = (NRectMesh*)MeshArray.GetAt(MeshArray.GetSize() - 1);//Additional rotation surf
		if(pMesh)// может не быть
		{
			if(pExtPrev1)//сшить сетки
				pExtPrev1->JoinEdge(pMesh);
			pExtPrev1 = pMesh;
		}
		else
		{
			MeshArray.RemoveAt(MeshArray.GetSize() - 1);
		}
	}
	for(int k = 0; k < m_pToolsContour->num; ++k)// copy m_arr[1] - > m_arr[0]
	{
		pIntCont[k]->Copy10();
		pExtCont[k]->Copy10();
	}
}
void NOneCadrOneTool::BuildSurf2dArcNew(MeshArr& MeshArray) const
{
	if (!m_pToolsContour)
		return;

	NRectMesh* pIntPrev = NULL, * pExtPrev0 = NULL, * pExtPrev1 = NULL;
	for (int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		pIntCont[k]->BuildMesh2dArc(MeshArray, false);
		NRectMesh* pMesh = (NRectMesh*)MeshArray.GetAt(MeshArray.GetSize() - 1);
		if (pIntPrev) //сшить сетки
			pIntPrev->JoinEdge(pMesh);
		pIntPrev = pMesh;

		pExtCont[k]->BuildMesh2dArc(MeshArray, true);
		pMesh = (NRectMesh*)MeshArray.GetAt(MeshArray.GetSize() - 1);
		if (pExtPrev0)//сшить сетки
			pExtPrev0->JoinEdge(pMesh);
		pExtPrev0 = pMesh;
	}
	for (int k = 0; k < m_pToolsContour->num; ++k)// copy m_arr[1] - > m_arr[0]
	{
		pIntCont[k]->Copy10();
		pExtCont[k]->Copy10();
	}
}
void NOneCadrOneTool::BuildSurfVert(MeshArr &MeshArray)
{
	if(!m_pToolsContour)
		return;

	NContour NewCont(*m_pToolsContour);
	NewCont.CrVertMovCont(m_pCInfo->CurLine);

	int RotNum = 4 * NSurfSettings::GetNumAproxLine(NewCont.GetMaxX(), 90.); 

	NOnePartProcessing *pNOPP = NULL;
	NRectMesh *pPrev = NULL;
	for(int i = 0; i < NewCont.num; ++i)
	{
		switch(NewCont.Types[i])
		{
		case HORIZONTAL:
			pNOPP = new NHorPlaneProcessing;
			break;
		case CONE:
			pNOPP = new NPlaneProcessing;
			break;
		case SPHERE:
			pNOPP = new NCylinderProcessing;
			break;
		case TORAHS:
			pNOPP = new NCylinderProcessing;
			break;
		default:
			continue;
		}
		NLine Line;
		NewCont.GetLine(i,Line);
		pNOPP->SetLine(Line, m_pCInfo, this);
		pNOPP->SetInitCont(i == NewCont.num - 1);
		pNOPP->BuildMeshFullRot(MeshArray, RotNum);
		delete pNOPP;
		NRectMesh *pMesh = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);
		if(pPrev) //сшить сетки
			pPrev->JoinEdge(pMesh);
		pPrev = pMesh;
	}
}
void NOneCadrOneTool::CrEndFaces(MeshArr &PlaneArray)
{
	if(!m_pToolsContour)
		return;

	// Calculate intermediate contour
	BPoint n1 = m_pCInfo->CurLine.GetDir(1);// End direction
	n1 = n1 * (1./sqrt(n1.D2()));
	BPoint n0 = m_pCInfo->m_NextDir;
	double l = n0.D2();
	if(l > 0.)
		n0 = n0 * (1./sqrt(l));
	else
		n0 = n1;
	if(m_pCInfo->m_ChainEndBis)
	{
		n1 = n1 + n0;
		n1 = n1 * (1./sqrt(n1.D2()));
	}

// Create mesh
	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetMeshID(PColorNum);
	PlaneArray.Add(pMesh);
	if(fabs(fabs(n0.Angle(n1)) - 180.) < MINAD)
	{
		pMesh->SetRowsNum(0);
		return;
	}

	n1 = n1 + n0;
	n1 = n1 * (1./sqrt(n1.D2()));// Bisector vector

	pMesh->SetRowsNum(3);
	pMesh->m_IsDeg = 1;
	pMesh->CuttingTool = CuttingTool;
	pMesh->Shank = Shank;
	pMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pMesh->m_iSurf = ID_EMPTY;
	pMesh->m_sgn = m_pCInfo->IsExtLeft ? -1 : 1;

	CUIntArray buf;
	int i;
	for( i = 0; i < m_pToolsContour->num; ++i)
	{
		pMesh->m_arr[0].Append(pExtCont[i]->GetArray(0));
		pMesh->m_arr[2].Append(pIntCont[i]->GetArray(0));
	}
	BPoint P0(*MFOREST.GetPoint(pMesh->m_arr[0][0]));
	P0.ProjXY();
	pMesh->m_arr[1].SetSize(pMesh->m_arr[0].GetSize());
	for( i = 1; i < pMesh->m_arr[0].GetSize()-1; ++i)
	{
		BPoint P =(*MFOREST.GetPoint(pMesh->m_arr[0][i]));
		double d = sqrt((P - P0).ProjXY().D2());
		P = BPoint(P0.GetX(), P0.GetY(), P.GetZ(), 1.) + n1 * d;
		pMesh->m_arr[1][i] = MFOREST.AddPoints(P);
	}
	// Add first and last points to the 1 and 2 rows
	pMesh->m_arr[1][0] = pMesh->m_arr[0][0];
	pMesh->m_arr[2][0] = pMesh->m_arr[0][0];
	pMesh->m_arr[1][pMesh->m_arr[0].GetSize()-1] = pMesh->m_arr[0][pMesh->m_arr[0].GetSize()-1];
	pMesh->m_arr[2][pMesh->m_arr[0].GetSize()-1] = pMesh->m_arr[0][pMesh->m_arr[0].GetSize()-1];
}

void NOneCadrOneTool::BuildSurf3d(MeshArr &MeshArray)
{
	if(!m_pToolsContour)
		return;

	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		pIntCont[k]->SetAngles();
		pExtCont[k]->SetAngles();
	}

	DefBaseAngles();

	NOnePartProcessing **pCont = pIntCont;
	for(int bb = 0; bb < 2; ++bb, pCont = pExtCont)
	{
		NRectMesh *pTopPrev = NULL, *pBotPrev = NULL;
		for(int k = 0; k < m_pToolsContour->num; ++k)
		{
			// построение сеток
			pCont[k]->BuildMesh3d(MeshArray, 
				k == m_pToolsContour->num - 1,
				k == 0 ? NULL : pCont[k-1]);
			NRectMesh *pTop = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-2);
			NRectMesh *pBot = (NRectMesh *)MeshArray.GetAt(MeshArray.GetSize()-1);
			if(pBotPrev) //сшить сетки
				if(pTop)
					pBotPrev->JoinEdge(pTop);
				else
					pBotPrev->JoinEdge(pBot);
			pBotPrev = pBot;
		}
	}
	// Построить начальные и конечные сетки
	BuildSurf3dBound(MeshArray);
}

void NOneCadrOneTool::BuildSurf3dCv(MeshsArr& MeshsArray)
{
	// concave case
	if (!m_pToolsContour)
		return;

	for (int k = 0; k < m_pToolsContour->num; ++k)
	{
		pIntCont[k]->SetAngles();
		pExtCont[k]->SetAngles();
	}

	DefBaseAngles();

	for (int k = 0; k < m_pToolsContour->num; ++k)
	{
		const auto pSolid = pIntCont[k]->BuildSolid3d(pIntCont[k], k == m_pToolsContour->num - 1, k == 0 ? NULL : pIntCont[k - 1], pExtCont[k]);
		if(pSolid != nullptr && k == 5)
			MeshsArray.Add(pSolid);
	}
}

// Построение передних и задних поверхностей
// контуры для всех элементов уже построены
void NOneCadrOneTool::BuildSurf3dBound(MeshArr &MeshArray) const
{
	if(!m_pToolsContour)
		return;

	for(int k = 0; k < m_pToolsContour->num; ++k)
	{
		// построение сеток
		pIntCont[k]->BuildMesh3dBound(MeshArray, 
			k == m_pToolsContour->num - 1,
			k == 0 ? NULL : pIntCont[k-1],
			pExtCont[k]);
	}
}

void NOneCadrOneTool::Clear()
{
	if(!m_pToolsContour)
		return;
	for(int i = 0; i < m_pToolsContour->num; ++i)
	{
		delete pIntCont[i];
		delete pExtCont[i];
	}
	delete pIntCont;
	pIntCont = NULL;
	delete pExtCont;
	pExtCont = NULL;

	delete m_pToolsContour;
	m_pToolsContour = NULL;
	RadCCTool = false;
	ConcaveTool = false;
	CuttingTool = true;
	Shank = false;
}
void NOneCadrOneTool::FixIMStart(const MeshArr &MeshArray)
{
	if(!m_pToolsContour)
		return;

	iMStart = MeshArray.GetSize();
}
void NOneCadrOneTool::FixIMEnd(const MeshArr &MeshArray)
{
	if(!m_pToolsContour)
		return;

	iMEnd = MeshArray.GetSize();
}
void NOneCadrOneTool::TrimStartFaces(MeshArr &PlArray, const BPoint &C, const BPoint &N)
{
	if(!m_pToolsContour)
		return;
// Trim start faces
	for(int i = iMStart; i < iMEnd; ++i)
	{
		NCadrsProcessing::TrimFlatMesh((NRectMesh *)PlArray[i],	C, N);
	}
}
void NOneCadrOneTool::TrimAllFaces(MeshArr& PlArray, const BPoint& C, const BPoint& N)
{
	if (!m_pToolsContour)
		return;
	for (int i = 0; i < PlArray.GetSize(); ++i)
	{
		NCadrsProcessing::TrimFlatMeshWPos((NRectMesh*)PlArray[i], C, N);
	}
}
void NOneCadrOneTool::ApproxContour()
{
	if(!m_pToolsContour)
		return;

	m_pBufContour = m_pToolsContour;

	for(int i = 0; i < m_pToolsContour->num; ++i)
	{
		delete pIntCont[i];
		delete pExtCont[i];
	}
	delete pIntCont;
	pIntCont = NULL;
	delete pExtCont;
	pExtCont = NULL;

	m_pToolsContour = NULL;
	RadCCTool = false;

	NContour *pAppContour = new NContour(*m_pBufContour);
	pAppContour->Approximate();
	SetToolContour(pAppContour, CuttingTool, Shank, ConcaveTool);
}
void NOneCadrOneTool::RestoreContour()
{
	if(!m_pToolsContour)
		return;

	SetToolContour(m_pBufContour, CuttingTool, Shank, ConcaveTool);
}
