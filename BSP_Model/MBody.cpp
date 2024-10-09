#include "stdafx.h"
#include "ppl.h"
#include "NCMComData.h"
#include "BMatr.h"
#include "MFace.h"
#include "MBSPForest.h"
#include "ConstDef.h"
#include "NCadrGeom.h"
#include "SOCyl.h"
#include "SOCone.h"
#include "SOSphere.h"
#include "SOPlane.h"
#include "SOTorus.h"
#include "math.h"
#include "MBodyParams.h"
#include "NRectMesh.h"
#include "MForestSave.h"
#include "MQuadTreeRot.h"
#include "MBody.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MBody::MBody(BStockGrID Position) : MinPoint(0., 0., 0., 1.), MaxPoint(0., 0., 0., 1.)
{
	BodyRotActive = false;
	pNextMBody = NULL;
	StockGr = Position;
}


void MBody::ReCreate(const BPoint & Minp, const BPoint & Maxp)
{
	MinPoint = Minp;
	MaxPoint = Maxp;
}

void MBody::Translate(const BMatr & M)
{
	// M may contain 180 rotation and translation
	BBox B;
	B.Expand(MinPoint * M);
	B.Expand(MaxPoint * M);
	MinPoint = B.GetMinPoint();
	MaxPoint = B.GetMaxPoint();
}

void MBody::Transform(const BMatr & M)
{
	if (NCMComData::IsTurnEnabled())
	{//Mill turn project mill body
		Translate(M);
	}
	else
	{// Mill project
		if (!M.HasNotRot() || IsBodyRotActive())
		{
			BMatr NewM(M);
			if (!M.HasNotTran())
			{
				BPoint TranVec(M.GetRow(3));
				TranVec.SetH(0.);
				NewM.SetRow(3, BPoint(0., 0., 0., 1));
				TranVec = TranVec * M.invr() * BodyRot.Inv();
				BMatr Tr;
				Tr.Trans(TranVec);
				Translate(Tr);
			}
			BodyRot = BodyRot.Or() * NewM;
			SetBodyRotActive(!BodyRot.Or().HasNotRot());
		}
		else if (!M.HasNotTran())
			Translate(M);
	}
}

void MBody::RecalcMinMax()
{
	BBox Box;
	Box.Expand(MinPoint);
	Box.Expand(MaxPoint);
	MinPoint = Box.GetMinPoint();
	MaxPoint = Box.GetMaxPoint();
}

MBody::~MBody(void)
{
	if (pNextMBody)
		delete pNextMBody;
	pNextMBody = NULL;
}

void MBody::AddBodyInList(MBody *pMBody)
{
	if (this == pMBody)
		return; 
	if (pNextMBody)
		pNextMBody->AddBodyInList(pMBody);
	else
		pNextMBody = pMBody;
}

void MBody::SetNextMBody(MBody *next)
{
	if (next == this)
		return;
	pNextMBody = next;
}

MBody* MBody::GetNextMBody() const
{
	return pNextMBody;
}

void MBody::ResetNextBody()
{
	pNextMBody = NULL;
}

void MBody::StoreMesh(const NParamMesh & PMesh, ElemID ElID, cadrID CaID, bool ToAux)
{
	// Process single body
	//Обрабатывается только RectMesh
	const NRectMesh &Mesh = (const NRectMesh &)PMesh;
	// Сеть может быть вырождена
	// IsDeg == 1, то совпадают точки [*][0]
	// IsDeg == 2, то дополнительно совпадают точки [*][j-end]
	// массивы [*][j] при j=0 или j = j-end могут содержать повторяющиеся номера, но не одновременно.
	// если Mesh треугольник, то элементы [0][1] и [0][2] определяют
	// отрезок, который соответствует дуге Mesh[0] на смежной сети.
	
	int iMax = Mesh.m_r;
	int jMax = int(Mesh.m_arr[0].GetSize());
	double sign = Mesh.m_sgn;
	if( iMax == 1 )// Flat contour
	{
	// Remove equal numbers
		int i = 0;
		for(int j = 0; j < jMax; ++j)
		{
			if(Mesh.m_arr[0][(j+1)%jMax] != Mesh.m_arr[0][j])
				Mesh.m_arr[0][i++] = Mesh.m_arr[0][j];
		}
		jMax = i;
	//
		int n = MFOREST.CalcNormal(
			Mesh.m_arr[0][0], Mesh.m_arr[0][1], Mesh.m_arr[0][jMax-1], sign); 
		MFace Face(n,Mesh.m_arr[0][0]);
		Face.SetArraySize(jMax);
		for(int j = 0; j < jMax; ++j)
			Face.SetPointAt(j,Mesh.m_arr[0][j]);
		StoreFace(&Face, MElemIDProc::SetSurfID(&ElID,Mesh.m_iSurf), CaID, ToAux);
	}
	else
	{
		for(int i = 0; i < iMax-1; ++i)
		{
			for(int j = 0; j < jMax-1; ++j)
			{
				MFace *pFace = MFOREST.CalcFace(
								Mesh.m_arr[i  ][j  ],
								Mesh.m_arr[i  ][j+1], 
								Mesh.m_arr[i+1][j+1],
								Mesh.m_arr[i+1][j  ], sign);
//				if(DEBUGMODE)
				{
					if(pFace == NULL)
					{
						pFace = MFOREST.CalcFace(
								Mesh.m_arr[i  ][j  ],
								Mesh.m_arr[i  ][j+1], 
								Mesh.m_arr[i+1][j+1],
								Mesh.m_arr[i+1][j+1], sign);
						StoreFace(pFace, MElemIDProc::SetSurfID(&ElID,Mesh.m_iSurf), CaID, ToAux);
						delete pFace;
						pFace = MFOREST.CalcFace(
								Mesh.m_arr[i  ][j  ],
								Mesh.m_arr[i+1][j+1], 
								Mesh.m_arr[i+1][j+1],
								Mesh.m_arr[i+1][j], sign);
					}
				}
				StoreFace(pFace, MElemIDProc::SetSurfID(&ElID,Mesh.m_iSurf), CaID, ToAux);
				delete pFace;
			}
		}
	}
}

void MBody::MarkUsedFacesOrig(class BDynBitArray & UsedF, class BDynBitArray & UsedO)
{
	this->MarkUsedFacesOrigCh(UsedF, UsedO);
}

void MBody::MarkUsedOrig(class BDynBitArray & Used)
{
	this->MarkUsedOrigCh(Used);
}

void MBody::CopyMerge(MBSPMerge & Op , MBody & In)
{
	if (pNextMBody)
		pNextMBody->CopyMerge(Op, *(In.GetNextMBody()));
	this->CopyMergeCh(Op, In);
}

void MBody::SafeMerge(const MBSPMerge & Op)
{
	for (MBody* pFirst = this; pFirst != nullptr; pFirst = pFirst->GetNextMBody())
		pFirst->SafeMergeCh(Op);
}

//void MBody::Clear(void)
//{
//	if (pNextMBody)
//		pNextMBody->Clear();
//	this->ClearCh();
//}

bool MBody::Check(void) const
{
	if (pNextMBody)
		return pNextMBody->Check() && this->CheckCh();
	return this->CheckCh();
}

MBody * MBody::Invert(bool ToAux)
{
	if (pNextMBody)
		pNextMBody->Invert(ToAux);
	return this->InvertCh(ToAux);
}

void MBody::CreateBodyStd(const BBox &Bounds, bool Convex, bool ToAux)
{
	if (pNextMBody)
		pNextMBody->CreateBodyStd(Bounds, Convex, ToAux);
	this->CreateCh(Bounds, Convex, ToAux);
}

void MBody::CreateFullCopy(MBody* pQ)
{
	pQ->MinPoint = MinPoint;
	pQ->MaxPoint = MaxPoint;
	pQ->BodyRot = BodyRot;
	pQ->BodyRotActive = BodyRotActive;
	pQ->StockGr = StockGr;
}

void MBody::Save(MForestSave &SaveData)
{
	CFile *f = SaveData.File;
	f->Write(&BodyRot, sizeof(BodyRot));
	f->Write(&BodyRotActive, sizeof(BodyRotActive));
	BPoint Tmp;
	Tmp = MinPoint * SaveData.Matr;
	f->Write((void *)&Tmp, sizeof(Tmp));
	Tmp = MaxPoint * SaveData.Matr;
	f->Write((void *)&Tmp, sizeof(Tmp));
}

void MBody::Load(CFile * f)
{
	f->Read(&BodyRot, sizeof(BodyRot));
	f->Read(&BodyRotActive, sizeof(BodyRotActive));
	f->Read(&MinPoint, sizeof(MinPoint));
	f->Read(&MaxPoint, sizeof(MaxPoint));
}

void MBody::RecalcID(const MFace& Face, ElemID &ElID)
{
	MElemIDProc *pIDProc = MFOREST.GetIDProc();
	int SurfID = pIDProc->GetSurfID(ElID);
	if(SurfID >= 0 && SurfID != ID_EMPTY)
		return;
	const BPoint &Normal = Face.GetPlane().GetNormal();
	float N[3] = {-float(Normal.GetX()), -float(Normal.GetY()), -float(Normal.GetZ())};
	pIDProc->SetNormalToSurfID(SurfID, N);
	pIDProc->SetSurfID(&ElID, SurfID);
}

ElemID MBody::MakeID4TurnSurf(ContElem Elem)
{
	SOrigSurf *pSurf = MFOREST.GetOrigSurf(MFOREST.GetIDProc()->GetSurfID(Elem.ElID));
	if(!pSurf)
		return Elem.ElID;
	SOrigSurf *pNewSurf = NULL;
	switch(pSurf->GetType())
	{
	case PLANE:
		{
			double Angle = (*Elem.p1 - *Elem.p0).Angle(BPoint(0.,1.,0.,0.));
			if( fabs( Angle ) < MIND || fabs( fabs( Angle ) - 180.) < MIND)
				pNewSurf = new SOCyl(BPoint(0.,0.,0.,1.), BPoint(0.,1.,0.,0.)
					, fabs(Elem.p0->GetX()) * (Elem.p1->GetY() > Elem.p0->GetY() ? +1. : -1.));
			else if (fabs( fabs( Angle ) - 90.) < MIND)
				pNewSurf = new SOPlane(BPoint(0., Angle > 0. ? -1. : +1. ,0.,0.), *Elem.p0);
			else
			{
				SOCone Buf(BPoint(0.,0.,0.,1.), BPoint(0.,1.,0.,0.), *Elem.p0, *Elem.p1);
				pNewSurf = new SOCone(Buf.GetCenter(), Buf.Get_N_Vector()
					, Buf.GetAngle() * (Elem.p1->GetY() > Elem.p0->GetY() ? +1. : -1.));
			}
		}
		break;
	case CYLINDER:
		{
			const SOCyl *pCyl = (SOCyl *)pSurf;
			double Rl = fabs(pSurf->GetCenter().GetX());
			if( Rl < MIND )
				pNewSurf = new SOSphere(pCyl->GetRadius()
					, BPoint(0., pSurf->GetCenter().GetY(), 0., 1.));
			else
				pNewSurf = new SOTorus(Rl
					, pCyl->GetRadius()
					, BPoint(0., pSurf->GetCenter().GetY(), 0., 1.)
					, BPoint(0.,1.,0.,0.));
		}
		break;
	default:
		pNewSurf = NULL;
		break;
	}
	ElemID ID = Elem.ElID;

	return MElemIDProc::SetSurfID(&ID, MFOREST.AddOrigSurf(pNewSurf));
}
NCadrGeom *ContElem::CrCadrGeom(void)
{
	NCadrGeom *pGeom = new NCadrGeom;
	int SurfID = MFOREST.GetIDProc()->GetSurfID(ElID);
	SOrigSurf *pSurf = NULL;
	if(SurfID >= 0)
		pSurf = MFOREST.GetOrigSurf(SurfID);
	SSurfType SType = PLANE;
	if(pSurf)
		SType = pSurf->GetType();

	BPoint P0 = *p0;
	BPoint P1 = *p1;
	pGeom->SetType(line);
	pGeom->SetB(P0.ProjXY());
	pGeom->SetE(P1.ProjXY());
	if(SType == CYLINDER)
	{
		BPoint Center = pSurf->GetCenter().ProjXY();
		pGeom->SetI(Center - P0);
		pGeom->SetType(((SOCyl *)pSurf)->GetRadius() > 0. ? ccwarc : cwarc);
	}

	return pGeom;
}

bool MBody::IsRotBody() const
{
	return (dynamic_cast<const MQuadTreeRot*>(this) != nullptr);
}

