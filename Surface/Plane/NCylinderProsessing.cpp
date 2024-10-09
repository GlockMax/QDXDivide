#include "stdafx.h"
#include "NCylinderProsessing.h"
#include "NSurfSettings.h"
#include "NOneCadrProcessing.h"
#include "NOneCadrOneTool.h"
#include "NRectMesh.h"
#include "NTriMesh.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "SOSphere.h"
#include "SOTorus.h"
#include "MVRDS.h"
#include "math.h"
#include "ConstDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NCylinderProcessing::NCylinderProcessing(void) : NOnePartProcessing()
{
//	m_IsSpecialEvent = false;
}

NCylinderProcessing::~NCylinderProcessing(void)
{
}

CUIntArray &NCylinderProcessing::GetSContour()
{
	return m_arr[0];
}

CUIntArray &NCylinderProcessing::GetEContour()
{
	return m_arr[1];
}
void NCylinderProcessing::SetAngles(void)
{
	double b;
	NOneCadrProcessing::ComputeAngles(m_Line, SPHERE, m_pCInfo->m_CurDir, TopAngle, b);
	if(IsLeft())
		TopAngle = 0.5*TopAngle;
	else
		TopAngle = -180. + 0.5*TopAngle;
	BotAngle = TopAngle;
}

void NCylinderProcessing::SetInitCont(bool LastPart)
{
	int i;
	BMatr m;
	NLine Line = m_Line;

	m.RotZ(BPoint(0,0,0,1), m_pCInfo->m_BAlf + (m_pCInfo->m_IsTurnCW ? 0 : m_pCInfo->m_Alf));
	Line = Line * m;

	BPoint p(0, 0, Line.p[3].GetZ()+m_Line.p[0].GetX(), 1.);
	m.rotg(m_pCInfo->m_GAlf * (m_pCInfo->m_IsTurnCW ? -1: 1), p, Line.p[0]);
	Line = Line * m;
	m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
	Line = Line * m;

	// число плоскостей для аппроксимации этого участока
	i = GetNumAproxLine(Line);
	i = max(2,i);// EK
	// разбиение линиии на части и сохранение номеров, соответствующих номеров
	int ln = LastPart ? i + 1 : i;
	m_arr[0].RemoveAll();
	for (int j=0; j<ln; j++)
	{
		p = Line.GetPointFromParam((double)j/i);
		p.Norm();
		m_arr[0].Add(MFOREST.AddPoints(p));
	}
}
void NCylinderProcessing::BuildMesh2d(MeshArr &MeshArray, bool Ext)
{
	MPlane *pPlane = & ( Ext ? m_pCInfo->m_P4 : m_pCInfo->m_P2);
	pPlane = (m_pCInfo->m_ChainEndBis ? pPlane : &m_pCInfo->m_P4);
	m_arr[1].RemoveAll();
	m_arr[1].Append(m_arr[0]);
	GetContByIntersecPlane(m_arr[1], 0, int(m_arr[1].GetSize() - 1), *pPlane);
	NRectMesh *pMesh = CreateMesh();
	pMesh->m_sgn = Ext == m_pCInfo->IsExtLeft ? -1 : 1;
	// Create surface
	BPoint c = m_Line.GetCenter();
	BMatr m;
	m.e0cong(BPoint(0.,1.,0.,0.),m_pCInfo->m_CurDir);
	c = c * m;
	m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint);
	c = c * m;
	double r = m_Line.GetRad();
	pMesh->m_iSurf = MFOREST.AddOrigSurf(
		new SOCyl( c, m_pCInfo->m_CurDir * (1./sqrt(m_pCInfo->m_CurDir.D2())), r));
	MeshArray.Add(pMesh);
	if(Ext)
	{
		pMesh = CreateAddTurnMesh();
		MeshArray.Add(pMesh);
	}
	Copy10();
}
void NCylinderProcessing::BuildMesh2dArc(MeshArr &MeshArray, bool Ext, bool PrevIsNearSmall/* = false*/)
{
	BPoint ArcCenter = m_pCInfo->CurLine.GetCenter();
	BPoint c = ArcCenter + BPoint(0, 0, m_Line.GetCenter().GetZ(), 0);
	double r = m_Line.GetRad();
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
		pMesh->m_sgn = (Ext == m_pCInfo->IsExtLeft) ? -1 : 1;// -1 Left; 1 - Right
		// Create surface
		pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOTorus(
			m_pCInfo->CurLine.GetRad() + CalcDelta(Ext),
			r,
			c,
			BPoint(0, 0, 1, 0)));
	}
	MeshArray.Add(pMesh);
}

NRectMesh * NCylinderProcessing::CreateMesh()// EK
{
	MBSPForest *pForest = &MBSPForest::GetActive();
	NRectMesh *pParMesh = new NRectMesh;

	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->SetRowsNum(2);
	pParMesh->m_arr[0].Append(m_arr[0]);
	pParMesh->m_arr[1].Append(m_arr[1]);
	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	// связанная с сетью поверхность
	BPoint dn = m_pCInfo->m_CurDir;// EK
	dn = dn * (1./sqrt(dn.D2()));//LAST
	BPoint C = m_Line.GetCenter();
	BMatr m,t;
	m.e0cong(BPoint(0,-1,0,0),dn);
	t.Trans(BPoint(0,0,0,1),m_pCInfo->m_BPoint);
	pParMesh->m_iSurf = pForest->AddOrigSurf(new SOCyl((C*m)*t, dn, sqrt((m_Line.p[0]-C).D2())));
	return pParMesh;
}

NRectMesh* NCylinderProcessing::CreateAddTurnMesh()
{
	BPoint ArcCenter = m_pCInfo->CurLine.GetCenter();
	BPoint c = ArcCenter + BPoint(0, 0, m_Line.GetCenter().GetZ(), 0);
	double r = m_Line.GetRad();
	NRectMesh* pMesh = Create2dAngleMesh();
	if (pMesh != nullptr)
	{
		pMesh->m_sgn = m_pCInfo->IsExtLeft ? -1 : 1;
		BPoint ArcC = m_Line.GetCenter();
		double R = sqrt(ArcC.ProjXY().D2());
		if (m_Line.p[1].GetX() * ArcC.GetX() < 0)
			R = -R;
		BPoint EndPointXY = m_pCInfo->CurLine.p[3];
		EndPointXY.ProjXY();
		if (fabs(R) > MIND)
		{
			pMesh->m_iSurf = MFOREST.AddOrigSurf(
				new SOTorus(R, r,
					EndPointXY + BPoint(0., 0., c.GetZ(), 0.),
					BPoint(0., 0., 1., 0.)));
		}
		else
		{
			pMesh->m_iSurf = MFOREST.AddOrigSurf(
				new SOSphere(r,
					EndPointXY + BPoint(0., 0., c.GetZ(), 0.)));
		}
	}
	return pMesh;
}

//NRectMesh * NCylinderProcessing::Create2dAngleMesh()
//{
//// Построение сектора "доворота"
//	BPoint p = m_pCInfo->CurLine.GetDir(1);
//	double Angle = p.Angle(m_pCInfo->m_NextDir);
//	if(fabs(Angle) < MINAD)
//		return NULL;
//	// определение центра вращения
//	BPoint c = m_pCInfo->m_BPoint + m_pCInfo->m_CurDir;
//	double Rmax = m_pCInfoC->m_pToolsContour->GetMaxX();
//	int n = NSurfSettings::GetNumAproxLine(Rmax, Angle);
//	NRectMesh *pParMesh = CreateRotMesh(m_arr[1], 0, int(m_arr[1].GetSize() - 1), c, Angle, n);
//	pParMesh->m_IsDeg = 0;
//	return pParMesh;
//}
int NCylinderProcessing::Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const
{
	int iSurf = 0;
	// Create surface
	BPoint ArcC = Line.GetCenter();
	double r = Line.GetRad();
	double R = sqrt((ArcC - Center).ProjXY().D2());
	if(R > MIND)
	{
		iSurf = MFOREST.AddOrigSurf(
			new SOTorus( R,	r,
				BPoint(Center).ProjXY() + BPoint(0.,0.,ArcC.GetZ(),0.),
				BPoint(0.,0.,1.,0.)));
	}
	else
	{
		iSurf = MFOREST.AddOrigSurf(
			new SOSphere( r, ArcC));
	}
	return iSurf;
}
void NCylinderProcessing::BuildMesh3d(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart)
{
// Create top mesh
// Create arc mesh
	if(fabs(TopAngle) < MINAD || fabs(fabs(TopAngle) - 180.) < MINAD) 
	{
		MeshArray.Add(NULL/*CreateMeshFromArc(true, pPrevPart)*/);
		MeshArray.Add(NULL);
		return;
	}
	if(!LastPart)
		return;
// Last part
	NRectMesh *pMesh = CreateMeshFromArc(false, pPrevPart);
	MeshArray.Add(pMesh);
	double DirAngle = fabs(m_pCInfo->m_CurDir.Angle(BPoint(0,0,1,0)));
	// Starting arc
	BPoint c = m_Line.GetCenter();
	BPoint RotAx = m_pCInfo->m_CurDir % BPoint(0,0,1,0);
	BMatr m;
	BPoint StartPoint = m_pCInfo->m_CurDir;
	StartPoint.ProjXY();
	StartPoint = StartPoint * (fabs(m_Line.p[0].GetX())/sqrt(StartPoint.D2()));
	StartPoint = (StartPoint + BPoint(0,0,m_Line.p[0].GetZ(),1)) * m.RotZ(BPoint(0,0,0,1), TopAngle);
	BPoint EndPoint = m_Line.p[3] * m.rotg(90. - DirAngle, c, RotAx);
	NLine StartLine;
	StartLine.p[0] = StartPoint;
	StartLine.p[1] = c;
	StartLine.p[3] = EndPoint;
	StartLine = StartLine * m.rotg(DirAngle, c, RotAx);// Place to the XY plane
	StartLine.BuildBallArc();
	StartLine = StartLine * m.rotg(-DirAngle, c, RotAx);// Place to the right position
	StartLine = StartLine * m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
	// число точек для аппроксимации этого участока
	int i = GetNumAproxLine(StartLine);
	i = max(2,i);
	// разбиение линиии на части и сохранение номеров, соответствующих точек
	int ln = LastPart ? i + 1 : i;
	m_arr[0].RemoveAll();
	for (int j=0; j<ln; j++)
		m_arr[0].Add(MFOREST.AddPoints(StartLine.GetPointFromParam((double)j/i).Norm()));
// Create mesh 

	m_arr[1].RemoveAll();
	m_arr[1].Append(m_arr[0]);// m_arr[1] will be changed in CreateParMeshTrans
	NRectMesh *pBotMesh = CreateParMeshTrans(m_arr[1], m_pCInfo->m_CurDir);
	pBotMesh->m_sgn = IsLeft() ? -1 : 1;
	// Create surface
	c = c * m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint);
	double r = m_Line.GetRad();
	pBotMesh->m_iSurf = MFOREST.AddOrigSurf(
		new SOCyl( c, m_pCInfo->m_CurDir * (1./sqrt(m_pCInfo->m_CurDir.D2())), r));
	if(pMesh)// Join meshs
	{
		pMesh->m_arr[0].Add(m_arr[0][0]);
		pMesh->m_arr[1].Add(m_arr[1][0]);
	}
	MeshArray.Add(pBotMesh);
	/*
	MPlane *pPlane = & ( Ext ? m_pCInfo->m_P4 : m_pCInfo->m_P2);
	pPlane = (m_pCInfo->m_ChainEndBis ? pPlane : &m_pCInfo->m_P4);
	m_arr[1].RemoveAll();
	m_arr[1].Append(m_arr[0]);
	GetContByIntersecPlane(m_arr[1], 0, m_arr[1].GetSize() - 1, *pPlane);
	NParamMesh *pMesh = CreateMesh();
	pMesh->m_sgn = Ext == m_pCInfo->IsExtLeft ? -1 : 1;
	// Create surface
	BPoint c = m_Line.GetCenter();
	BMatr m;
	m.e0cong(BPoint(0.,1.,0.,0.),m_pCInfo->m_CurDir);
	c = c * m;
	m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint);
	c = c * m;
	double r = m_Line.GetRad();
	pMesh->m_iSurf = MFOREST.AddOrigSurf(
		new SOCyl( c, m_pCInfo->m_CurDir * (1./sqrt(m_pCInfo->m_CurDir.D2())), r));
	MeshArray.Add(pMesh);
	if(Ext)
	{
		pMesh = Create2dAngleMesh();
		MeshArray.Add(pMesh);
		if(pMesh)
		{
			pMesh->m_sgn = m_pCInfo->IsExtLeft ? -1 : 1;
			BPoint ArcC = m_Line.GetCenter();
			double R = sqrt(ArcC.ProjXY().D2());
			if(m_Line.p[1].GetX() * ArcC.GetX() < 0)
				R = -R;
			if(fabs(R) > MIND)
			{
				pMesh->m_iSurf = MFOREST.AddOrigSurf(
					new SOTorus( R,	r,
						(m_pCInfo->m_BPoint+m_pCInfo->m_CurDir).ProjXY() + BPoint(0.,0.,c.GetZ(),0.),
						BPoint(0.,0.,1.,0.)));
			}
			else
			{
				pMesh->m_iSurf = MFOREST.AddOrigSurf(
					new SOSphere( r,
						(m_pCInfo->m_BPoint+m_pCInfo->m_CurDir).ProjXY() + BPoint(0.,0.,c.GetZ(),0.)));
			}
		}
	}
	Copy10();
	*/
}
void NCylinderProcessing::BuildMesh3dBound(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart, const NOnePartProcessing *pSimmPart) const
{
	if(fabs(TopAngle) < MINAD || fabs(fabs(TopAngle) - 180.) < MINAD) 
	{
		BuildMesh3dBoundA(MeshArray, LastPart, pPrevPart);
		return;
	}
	MVRDS List;
	bool ListFull = false;
	// Delaunay
	BMatr m;
	BPoint C = m_Line.GetCenter() * m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
	double R = m_Line.GetRad();
	NTriMesh *pFirstMesh = NULL;
	NTriMesh *pLastMesh = NULL;
	if(!m_arr[0].IsEmpty())
	{
		int v1 = List.AddVertex(m_arr[0][0]);
		int v2 = List.AddVertex(m_arr[0][1]);
		if(!IsLeft())
			List.AddActiveEdge(MEdge( &List, 
				v1, v2, MEdge::EMPTY, MEdge::NOT_DEFINED));
		else
			List.AddActiveEdge(MEdge( &List, 
				v1, v2, MEdge::NOT_DEFINED, MEdge::EMPTY));
		for(int i = 2; i < m_arr[0].GetSize(); ++i)
			if(List.AddVertex(m_arr[0][i]) < 0)
				ListFull = true;
		for(auto is = pSimmPart->m_arr[0].GetSize() - 2; is >= 0; --is)
			if(List.AddVertex(pSimmPart->m_arr[0][is]) < 0)
				ListFull = true;
		// Additional point
		if(m_pCInfo->m_CurDir.GetZ() > 0.)
			if(List.AddVertex(MFOREST.AddPoints(m_pCInfo->CurLine.p[0])) < 0)
				ListFull = true;
		NRectMesh *pPrevFirstMesh = (NRectMesh *)MeshArray[MeshArray.GetSize()-2];
		if(pPrevFirstMesh)
		{
			for(int k = 1; k < pPrevFirstMesh->m_arr[1].GetSize() - 1; ++k)
				if(List.AddVertex(pPrevFirstMesh->m_arr[1][k]) < 0)
					ListFull = true;
		}

		if(!ListFull)
			if(List.MakeLimTolTri(max(LIMTOL, NSurfSettings::GetTolerance()), C, R) >= 0)
			{
				pFirstMesh = new NTriMesh;
				pFirstMesh->SetMeshID(m_pCInfoC->PColorNum);
				List.StoreInMesh(*pFirstMesh);
			}
	}
	MeshArray.Add(pFirstMesh);

	List.Clear();
	ListFull = false;
	if(!m_arr[1].IsEmpty())
	{
		int v1 = List.AddVertex(m_arr[1][0]);
		int v2 = List.AddVertex(m_arr[1][1]);
		if(IsLeft())
			List.AddActiveEdge(MEdge( &List, 
				v1, v2, MEdge::EMPTY, MEdge::NOT_DEFINED));
		else
			List.AddActiveEdge(MEdge( &List, 
				v1, v2, MEdge::NOT_DEFINED, MEdge::EMPTY));
		for(int i = 2; i < m_arr[1].GetSize(); ++i)
			if(List.AddVertex(m_arr[1][i]) < 0)
				ListFull = true;
		for(auto is = pSimmPart->m_arr[1].GetSize() - 2; is >= 0; --is)
		{
			if(List.AddVertex(pSimmPart->m_arr[1][is]) < 0)
				ListFull = true;
		}
		// Additional point
		if(m_pCInfo->m_CurDir.GetZ() < 0.)
			if(List.AddVertex(MFOREST.AddPoints(m_pCInfo->CurLine.p[3])) < 0)
				ListFull = true;
		NRectMesh *pPrevLastMesh = (NRectMesh *)MeshArray[MeshArray.GetSize()-2];
		if(pPrevLastMesh)
		{
			for(int k = 1; k < pPrevLastMesh->m_arr[1].GetSize() - 1; ++k)
				if(List.AddVertex(pPrevLastMesh->m_arr[1][k]) < 0)
					ListFull = true;
		}
	
		if(!ListFull)
			if(List.MakeLimTolTri(max(LIMTOL, NSurfSettings::GetTolerance()), C + m_pCInfo->m_CurDir, R) >= 0)
			{
				pLastMesh = new NTriMesh;
				pLastMesh->SetMeshID(m_pCInfoC->PColorNum);
				List.StoreInMesh(*pLastMesh);
			}
	}
	MeshArray.Add(pLastMesh);

	if(pFirstMesh)
	{
		BMatr m;
		NLine Line = m_Line * m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint);
		pFirstMesh->m_iSurf = MFOREST.AddOrigSurf(new SOSphere(Line.GetRad(), Line.GetCenter()));
	}
	if(pLastMesh)
	{
		BMatr m;
		NLine Line = m_Line * m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint + m_pCInfo->m_CurDir);
		pLastMesh->m_iSurf = MFOREST.AddOrigSurf(new SOSphere(Line.GetRad(), Line.GetCenter()));
	}
}

MeshArr* NCylinderProcessing::BuildSolid3d(const NOnePartProcessing* pThisPart, bool LastPart, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const
{
	return nullptr;
}

void NCylinderProcessing::BuildMesh3dBoundA(MeshArr &MeshArray, bool LastPart, const NOnePartProcessing *pPrevPart) const
{
	const int EndInd = m_pCInfoC->BaseAngles.Find(180.);
	const int MidInd = m_pCInfoC->BaseAngles.Find(fabs(TopAngle));
	const int StartInd = m_pCInfoC->BaseAngles.Find(0.);
	const int PointsNum = GetNumAproxLine(m_Line);
	NRectMesh *pFirstMesh = CreateMeshByBase(m_pCInfo->m_CurDir.GetZ() > 0., MidInd, EndInd, PointsNum);
	MeshArray.Add(pFirstMesh);
	NRectMesh *pLastMesh = CreateMeshByBase(m_pCInfo->m_CurDir.GetZ() <= 0., MidInd, StartInd, PointsNum);
	MeshArray.Add(pLastMesh);
	if(pFirstMesh)
	{
		BMatr m;
		NLine Line = m_Line * m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint);
		pFirstMesh->m_iSurf = MFOREST.AddOrigSurf(new SOSphere(Line.GetRad(), Line.GetCenter()));
	}
	if(pLastMesh)
	{
		BMatr m;
		NLine Line = m_Line * m.Trans(BPoint(0.,0.,0.,1.), m_pCInfo->m_BPoint + m_pCInfo->m_CurDir);
		pLastMesh->m_iSurf = MFOREST.AddOrigSurf(new SOSphere(Line.GetRad(), Line.GetCenter()));
	}

}

bool NCylinderProcessing::IsUpper(void)
{
	return 0 < fabs(m_Line.p[3].GetX()) - fabs(m_Line.p[0].GetX());
}
