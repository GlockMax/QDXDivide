#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "NColor.h"
#include "BPoint.h"
#include "BMatr.h"
#include "TBladeCont.h"
#include "TBladeEdge.h"
#include "NRectMesh.h"
#include "tfascia.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TFascia::TFascia(void)
{
	Clear();
}

TFascia::~TFascia(void)
{
}

TBladeCont * TFascia::Create(const TBladeCont * pS, double L, double A)
{
	pSrc = pS;
	pDest = NULL;
	Length = L;
	Angle = A;
	return Create();
}

bool TFascia::Draw(const NColor &Act, const NColor &Pas)
{
	if(Length < MIND)
		return false;
	if(!pSrc)
		return false;
	if(!pDest)
	{
		if(!Create())
			return false;
	}
	glBegin(GL_QUADS);
	for(TBladeEdge *pCur = pSrc->GetStart(), *pCurD = pDest->GetStart()
		; pCur
		; pCur = pSrc->GetNext(pCur), pCurD = pCurD->GetNext())
	{// For each pair
		if(pCur->GetAttr() & BE_CUT)
			glColor4fv(Act.array);
		else
			glColor4fv(Pas.array);

		if(!pCur->IsArc())
		{
			glNormal3dv(Normal(pCur).GetArray());
			glVertex3dv(pCur->GetPoint().GetArray());
			glVertex3dv(pCurD->GetPoint().GetArray());
			glVertex3dv(pCurD->GetNext()->GetPoint().GetArray());
			glVertex3dv(pCur->GetNext()->GetPoint().GetArray());
		}
		else
		{
			glNormal3dv(NormalArc(pCur->GetPoint(), pCur->GetCircCenter()).GetArray());
			glVertex3dv(pCur->GetPoint().GetArray());
			glVertex3dv(pCurD->GetPoint().GetArray());
			glNormal3dv(NormalArc(pCur->GetNext()->GetPoint(), pCur->GetCircCenter()).GetArray());
			glVertex3dv(pCurD->GetNext()->GetPoint().GetArray());
			glVertex3dv(pCur->GetNext()->GetPoint().GetArray());
		}
	}
	glEnd();
	return true;
}

void TFascia::MakeMesh(MeshArr& RMeshArr)
{
	if (Length < MIND)
		return;
	if (pSrc == nullptr)
		return;
	if (pDest == nullptr)
	{
		if (Create() == nullptr)
			return;
	}
	BMatr M(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.));
	int StartMeshN = RMeshArr.GetSize();
	RMeshArr.SetSize(StartMeshN + 1);
	NRectMesh* pMesh = new NRectMesh;
	pMesh->CuttingTool = false;
	pMesh->Shank = false;
	pMesh->m_CadrNum = -1;
	pMesh->m_sgn = -1;
	pMesh->m_IsDeg = 0;
	pMesh->SetMeshID(MElemIDProc::ID_NULL);
	RMeshArr[StartMeshN] = pMesh;
	pMesh->SetSize(2, pSrc->GetEdgesNum() + 1);
	int i = 0;
	for (TBladeEdge* pCur = pSrc->GetStart(), *pCurD = pDest->GetStart()
		; pCur
		; pCur = pSrc->GetNext(pCur), pCurD = pCurD->GetNext())
	{// For each pair
		pMesh->m_arr[0].SetAt(i, MFOREST.AddPoints(pCur->GetPoint() * M));
		pMesh->m_arr[1].SetAt(i, MFOREST.AddPoints(pCurD->GetPoint() * M));
		++i;
	}
	pMesh->m_arr[0].SetAt(i, pMesh->m_arr[0][0]);
	pMesh->m_arr[1].SetAt(i, pMesh->m_arr[1][0]);
}

bool TFascia::IsCreated(void)
{
	return pDest == NULL;
}

void TFascia::Clear(void)
{
	Length = 0.;
	Angle = 0.;
	pSrc = NULL;
	pDest = NULL;
}

TBladeCont * TFascia::Create(void)
{
	XYshift = Length * sin(Angle * PI / 180.);
	Zshift = Length * cos(Angle * PI / 180.);
	pDest = new TBladeCont;
	pDest->MakeCopy(*pSrc);
	int Ignor = 0;
	for(TBladeEdge *pCur = pSrc->GetStart(), *pCurD = pDest->GetStart()
		; pCur
		; pCur = pSrc->GetNext(pCur), pCurD = pCurD->GetNext())
	{// For each pair
		if(Ignor > 0)// Ignore extra processing edges
			--Ignor;
		else
			Ignor = NewEdge(pCur, pCurD);
	}
	return pDest;
}

const BPoint TFascia::Normal(TBladeEdge * pEdge)
{
	return pSrc->GetNormal() * BMatr().rotg(- Angle - 90.
										, pEdge->GetPoint()
										, pEdge->GetNext()->GetPoint());
}
const BPoint TFascia::NormalArc(const BPoint &P, const BPoint &C)
{
	return pSrc->GetNormal() * BMatr().rotg(- Angle - 90.
										, BPoint(0.,0.,0.,1.)
										, pSrc->GetNormal() % (P - C));
}
/*
const BPoint TFascia::NormalSmooth(TBladeEdge * pEdge)
{
	double lCur = sqrt(pEdge->GetVec().D2());
	double lPre = sqrt(pEdge->GetPrev()->GetVec().D2());
	BPoint SmoothVec = pEdge->GetVec() * lPre + pEdge->GetPrev()->GetVec() * lCur;
	return pSrc->GetNormal() * BMatr().rotg(- Angle - 90.
										, BPoint(0.,0.,0.,1.)
										, SmoothVec);
}
*/
int TFascia::NewEdge(const TBladeEdge * pCur, TBladeEdge * pCurD)
{
	// Loops are not recognized
	int Ret = 0;// Returns a number of extra processing points 
	bool IsCurArc = pCur->IsArc();
	if(IsCurArc)
		if(pCur->GetRad() < XYshift)// Contour start point can't be internal point of an arc so pCur is the start point of a large arc
		{// Large arc processing
			// Find arc end
			Ret = 1;
			const TBladeEdge *pAdd = pSrc->GetNext(pCur);
			for (; pAdd != pSrc->GetStart(); pAdd = pAdd->GetNext())
			{
				if(!pAdd->IsArc())
					break;
				if((pAdd->GetCircCenter() - pCur->GetCircCenter()).D2() > MIND * MIND)
					break;
				++Ret;
			}
			// Find intersection point
			BPoint P0(pCur->GetPrev()->GetPoint());
			BPoint V0(pCur->GetPrev()->GetVec());
			BPoint P1(pAdd->GetPoint());
			BPoint V1s(pAdd->GetVec() * BMatr().RotZ(BPoint(0.,0.,0.,1.), 90.));

			double t = (P1 - P0) * V1s / ( V0 * V1s );
			BPoint NewP = P0 + V0 * t;
			// Find shifted point as two lines case
			BPoint e0(pCur->GetPrev()->GetPoint() - NewP);
			double z = sqrt(e0.D2());
			if( z > MIND * MIND ) e0 = e0 * ( 1. / z);
			BPoint e1(pAdd->GetNext()->GetPoint() - NewP);
			z = sqrt(e1.D2());
			if( z > MIND * MIND ) e1 = e1 * ( 1. / z);
			BPoint e1s(e1 * BMatr().RotZ(BPoint(0.,0.,0.,1), 90.));
			z = e0 * e1s;
			if( fabs(z) > MIND * MIND ) z = 1. / z;
			double Lambda = XYshift * z;
			NewP = NewP + (e0 + e1) * Lambda + pSrc->GetNormal() * Zshift;
			// Place it to all neccessary edges
			TBladeEdge *pEdgeD = pCurD;
			for(int i = 0; i < Ret; ++i, pEdgeD = pEdgeD->GetNext())
			{
				pEdgeD->SetPoint(NewP);
				pEdgeD->SetCircCenter(BPoint(0.,0.,0.,-1.));
			}
			pEdgeD->SetPoint(NewP); // Last edge point must be changed but CircCenter must not
			return Ret;
		}
	//Ordinary case processing
	bool IsPreArc = pCur->GetPrev()->IsArc();
	if(!IsCurArc && !IsPreArc)
	{// Both lines
		BPoint e0(pCur->GetPrev()->GetVec());
		double z = sqrt(e0.D2());
		if( z > MIND * MIND ) e0 = e0 * (-1. / z);
		BPoint e1(pCur->GetVec());
		z = sqrt(e1.D2());
		if( z > MIND * MIND ) e1 = e1 * ( 1. / z);
		BPoint e1s(e1 * BMatr().RotZ(BPoint(0.,0.,0.,1), 90.));
		z = e0 * e1s;
		if( fabs(z) > MIND * MIND ) z = 1. / z;
		double Lambda = XYshift * z;
		pCurD->SetPoint(pCur->GetPoint() + (e0 + e1) * Lambda + pSrc->GetNormal() * Zshift);
	}
	else if(IsCurArc && IsPreArc)
	{
		BPoint I(pCur->GetCircCenter() - pCur->GetPoint());
		double z = sqrt(I.D2());
		if( z > MIND * MIND )
			I = I * ( 1. / z );
		pCurD->SetPoint(pCur->GetPoint() + I * XYshift + pSrc->GetNormal() * Zshift);
		pCurD->SetCircCenter(pCurD->GetCircCenter() + pSrc->GetNormal() * Zshift);
	}
	else if(IsCurArc && !IsPreArc)
	{
		BPoint I(pCur->GetCircCenter() - pCur->GetPoint());
		double z = sqrt(I.D2());
		if( z > MIND * MIND )
			I = I * ( 1. / z );
		pCurD->SetPoint(pCur->GetPoint() + I * XYshift + pSrc->GetNormal() * Zshift);
		pCurD->SetCircCenter(pCurD->GetCircCenter() + pSrc->GetNormal() * Zshift);
	}
	else if(!IsCurArc && IsPreArc)
	{
		BPoint I(pCur->GetPrev()->GetCircCenter() - pCur->GetPoint());
		double z = sqrt(I.D2());
		if( z > MIND * MIND )
			I = I * ( 1. / z );
		pCurD->SetPoint(pCur->GetPoint() + I * XYshift + pSrc->GetNormal() * Zshift);
	};
	return 0;
}

bool TFascia::DrawLines(void)
{
	if(!pSrc)
		return false;
	if(!pDest)
	{
		if(!Create())
			return false;
	}
	glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	for(TBladeEdge *pCur = pSrc->GetStart(), *pCurD = pDest->GetStart()
		; pCur
		; pCur = pSrc->GetNext(pCur), pCurD = pCurD->GetNext())
	{// For each pair
		if(!pCur->IsArc())
		{
			glVertex3dv(pCurD->GetPoint().GetArray());
			glVertex3dv(pCurD->GetNext()->GetPoint().GetArray());
			glVertex3dv(pCur->GetNext()->GetPoint().GetArray());
			glVertex3dv(pCur->GetPoint().GetArray());
		}
	}
	glEnd();
	glPopAttrib();
	return true;
}
