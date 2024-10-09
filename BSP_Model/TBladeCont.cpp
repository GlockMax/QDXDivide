#include "stdafx.h"
#include <list>
#include "ConstDef.h"
#include "NCMProject.h"
#include "BMatr.h"
#include "BBox.h"
#include "NLine.h"
#include "ClrPoint.h"
#include "math.h"
#include "GL\gl.h"
#include "NContour.h"
#include "NSurfSettings.h"
#include "MBSPForest.h"
#include "NRectMesh.h"
#include "SOCyl.h"
#include "SOPlane.h"
#include "NSortedArray.h"
#include "TBladeCont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TBladeCont::TBladeCont(void)
	: N(0.,0.,1.,0.), pStart(nullptr)
{
	Convex = false;
}

TBladeCont::~TBladeCont(void)
{
	Clear();
}
void TBladeCont::Clear(void)
{
	TBladeEdge *pBuf = nullptr;
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{
		delete pBuf;
		pBuf = pCur;
	}
	delete pBuf;
	pStart = nullptr;
}


void TBladeCont::Trans(const BMatr &M)
{
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
		pCur->Trans(M);
	N = N * M;
}

void TBladeCont::Draw(void)
{
	glBegin(GL_TRIANGLE_FAN);
	glNormal3dv(N.GetArray());
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
		glVertex3dv(pCur->GetPoint().GetArray());
	glEnd();
}

void TBladeCont::MakeMesh(MeshArr& RMeshArr)
{
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
	pMesh->SetSize(1, GetEdgesNum() + 1);
	BMatr M(BMatr().RotZ(0., 0., 0., -90.) * BMatr().RotX(0., 0., 0., -90.));
	int i = 0;
	for (TBladeEdge* pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{
		pMesh->m_arr[0].SetAt(i, MFOREST.AddPoints(pCur->GetPoint() * M));
		++i;
	}
	pMesh->m_arr[0].SetAt(i, pMesh->m_arr[0][0]);
}

TBladeEdge * TBladeCont::GetNext(const TBladeEdge * pCur) const
{
	TBladeEdge *pB = pCur->GetNext();

	return pB == pStart ? nullptr : pB;
}
const BMatr TBladeCont::GetNormMatr() const
{// Matrix to get external normal
	BMatr Rot90;
	Rot90.rotg(-90., BPoint(0.,0.,0.,1.), N);// Matrix to get external normal
	return Rot90;
}
void TBladeCont::CalcArc(const NCadrGeom &Geom, TBladeEdge *pNP, TBladeEdge *pPN)
{
	// pNP processing
	if(pNP->IsArc() && pNP->GetPrev()->GetPrev()->IsArc())
	{// Point on the contour arc
		double NewRad = 0.;
		if((pNP->GetPrev()->GetPrev()->GetCircCenter() - pNP->GetPrev()->GetPoint()) 
			* Geom.GetI() > 0)
			NewRad = pNP->GetRad() + Geom.GetR();
		else
			NewRad = Geom.GetR() - pNP->GetRad();
		pNP->GetPrev()->SetCircCenter(BPoint( 0., 0., 0., -1.));
	}
	else
	{// Point between contour lines
		pNP->GetPrev()->SetCircCenter(pNP->GetPrev()->GetPoint() + Geom.GetI());
	}

	// pPN processing
	if(pPN->IsArc() && pPN->GetPrev()->GetPrev()->IsArc())
	{// Point on the contour arc
		double NewRad = 0.;
		if((pPN->GetCircCenter() - pPN->GetPoint()) 
			* Geom.GetI() > 0)
			NewRad = pPN->GetRad() + Geom.GetR();
		else
			NewRad = Geom.GetR() - pPN->GetRad();
		pPN->GetPrev()->SetCircCenter(BPoint(0., 0., 0., -1.));
	}
	else
	{// Point between contour lines
		pPN->GetPrev()->SetCircCenter(pPN->GetPoint() + Geom.GetI());
	}
}
void TBladeCont::MakeCopy(const TBladeCont &In)
{
	pStart = In.GetStart();

	if(pStart == nullptr)
		return;

	N = In.N;
	Convex = In.Convex;

	pStart = new TBladeEdge(*In.GetStart());
	TBladeEdge *pPrev = pStart;
	for(TBladeEdge *pCur = In.GetStart()->GetNext(); pCur != nullptr; pCur = In.GetNext(pCur))
	{
		double d = pCur->GetVec().D2();
		if( d <= MIND)
			continue;
		TBladeEdge *pNew = new TBladeEdge(*pCur);
		pNew->SetPrev(pPrev);
		pPrev->SetNext(pNew);
		pPrev = pNew;
	}
	pPrev->SetNext(pStart);
	pStart->SetPrev(pPrev);
}
bool TBladeCont::MakeContour(const NCadrGeom * GeomArr, int Num)
{
	// clear current
	Clear();
	//make new
	int count = 0;
	for(int i = 0; i < Num; ++i)
	{
		const NCadrGeom *pGeom = GeomArr + i;
		if(pGeom->IsFast())
			continue;
		else if(pGeom->IsLine())
		{
			if(!AddGeomElem(pGeom))
			{
				Clear();
				return false;
			}
			++count;
		}
		else if(pGeom->IsArc())
		{// Approximate arc
			double sa, ea, ca;
			pGeom->GetArcA(&sa, &ea, &ca);
			BPoint Center = pGeom->GetC();
			BPoint Pb = pGeom->GetB();
			int ANum = NSurfSettings::GetNumAproxLine(pGeom->GetR(), ca);
			BMatr Rot;
			Rot.rotg(ca / ANum, Center, pGeom->GetN());
			for(int k = 0; k < ANum; ++k)
			{
				BPoint Pe = Pb * Rot;
				NCadrGeom ArcPart(*pGeom);
				ArcPart.SetB(Pb);
				ArcPart.SetE(Pe);
				ArcPart.SetI(Center - Pb);
				Pb = Pe;
				if(!AddGeomElem(&ArcPart))
				{
					Clear();
					return false;
				}
				++count;
			}
		}
	}
	if(count < 2)
	{
		Clear();
		return false;
	}
	// find last
	TBladeEdge *pCur = GetStart()->GetNext();
	for(; pCur->GetNext(); pCur = pCur->GetNext());
	// check closure
	if((pCur->GetPoint() - pStart->GetPoint()).D2() > MIND*MIND)
	{
		Clear();
		return false;
	}
	pCur->GetPrev()->SetNext(pStart);
	pStart->SetPrev(pCur->GetPrev());
	delete pCur;
	// Test angle
	int Orient = NCadrGeom::IsConvexGeom(GeomArr, Num);
	if(Orient < 0.)
		N = N * (-1.);
	Convex = abs(Orient) < 2;
	return true;
}
bool TBladeCont::MakeContour(const NContour &Contour)
{
	// Contour lies in XZ plane
	// Make CadrGeom array
	int count = Contour.num;
	NCadrGeom *Cadrs = new NCadrGeom[count + 1];
	for(int i = 0; i < count; ++i)
	{
		NLine Line;
		if(!Contour.GetLine(i, Line))
		{
			delete[] Cadrs;
			return false;
		}
		switch(Contour.Types[i])
		{
		case HORIZONTAL:
		case CONE:
			Cadrs[i].Set(line
				, Line.p[0].GetX()
				, Line.p[0].GetY()
				, Line.p[0].GetZ()
				, Line.p[3].GetX()
				, Line.p[3].GetY()
				, Line.p[3].GetZ()
				, 0., 0., 0.
				, XZ);
			break;
		case TORAHS:
		case SPHERE:
			Cadrs[i].Set(line
				, Line.p[0].GetX()
				, Line.p[0].GetY()
				, Line.p[0].GetZ()
				, Line.p[3].GetX()
				, Line.p[3].GetY()
				, Line.p[3].GetZ()
				, 0., 0., 0.
				, XZ);
			break;
		default:
			delete[] Cadrs;
			return false;
		}
	}
	// Make it close
	NLine LineF, LineL;
	Contour.GetLine(0, LineF);
	Contour.GetLine(count - 1, LineL);
	if((LineF.p[0] - LineL.p[3]).D2() > MIND * MIND)
	{
		Cadrs[count].Set(line
				, LineL.p[3].GetX()
				, LineL.p[3].GetY()
				, LineL.p[3].GetZ()
				, LineF.p[0].GetX()
				, LineF.p[0].GetY()
				, LineF.p[0].GetZ()
				, 0., 0., 0.
				, XZ);
		++count;
	}
	
	bool res = MakeContour(Cadrs, count);
	delete[] Cadrs;
	N.Set(0.,1.,0.,0.);
	return res;
}

bool TBladeCont::AddGeomElem(const NCadrGeom *pGeom)
{
	// add start if empty
	if(pStart == nullptr)
	{
		pStart = new TBladeEdge;
		pStart->SetPoint(pGeom->GetB());
		if(pGeom->IsArc())
			pStart->SetCircCenter(pGeom->GetC());
		else
			pStart->SetCircCenter(BPoint(0.,0.,0.,-1.));

		pStart->SetPrev(nullptr);
		pStart->SetNext(nullptr);
	}
	//create new edge
	TBladeEdge *pNew = new TBladeEdge;
	pNew->SetNext(nullptr);
	// find last
	TBladeEdge *pLast = GetStart();
	for(; pLast->GetNext(); pLast = pLast->GetNext());
	// Add new
	pLast->SetNext(pNew);
	pNew->SetPrev(pLast);
	// fill new
	double Tol = NSurfSettings::GetTolerance();
	if((pGeom->GetB() - pLast->GetPoint()).D2() > Tol * Tol)
		return false;
	pNew->SetPoint(pGeom->GetE());
	if(pGeom->IsLine())
		pLast->SetCircCenter(BPoint(0.,0.,0.,-1.));
	else if(pGeom->IsArc())
		pLast->SetCircCenter(pGeom->GetC());
	else
		return false;

	return true;
}

void TBladeCont::Invert(void)
{
	for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
	{
		TBladeEdge *pBuf = pCur->GetNext();
		pCur->SetNext(pCur->GetPrev());
		pCur->SetPrev(pBuf);
	}
	N = N * (-1.);
}

bool TBladeCont::IsCircle(void) const
{
	BPoint bC = GetStart()->GetCircCenter();
	for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
	{
		if(!pCur->IsArc())
			return false;
		if((bC - pCur->GetCircCenter()).D2() > MIND * MIND)
			return false;
	}
	return true;
}
int TBladeCont::GetXSign(const BMatr &Mb)
{
	// returns  1 if all x cords are positive
	// returns -1 if all x cords are negative
	// returns  0 if x ccords changes its signums

	// Signum is well defined if |x| > MIND
	int PosNum = 0, NegNum = 0;
	for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
	{
		double x = (pCur->GetPoint() * Mb).GetX();
		if(fabs(x) > MIND)
			if( x > 0.)
				++PosNum;
			else
				++NegNum;
	}
	if(PosNum * NegNum == 0)
		return (PosNum > 0) ? 1 : -1;
	return 0;
}

double TBladeCont::GetMinRad(void) const
{
	double MinRad = 1.e6;
	for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
	{
		if(pCur->GetCircCenter().GetH() < 0.)
			continue;
		double d = pCur->GetRad();
		if( d < MinRad )
			MinRad = d;
	}
	return MinRad == 1.e6 ? -1. : MinRad;
}

int TBladeCont::BreakArcs(void)
{
	// returns an added edges number
	int ret = 0;
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{
		if(!pCur->IsArc())
			continue;
//		double Angle = pCur->GetPrev()->GetVec().Angle(pCur->GetNext()->GetVec());
		double Angle = (pCur->GetPoint() - pCur->GetCircCenter()).Angle(pCur->GetNext()->GetPoint() - pCur->GetCircCenter());
		int PartsNum = NSurfSettings::GetNumAproxLine(pCur->GetRad(), Angle);
		BMatr Rot;
		Rot.rotg(Angle / PartsNum, pCur->GetCircCenter(), N);
		for(int i = 1; i < PartsNum; ++i)
		{
			pCur->InsertAfter(new TBladeEdge(*pCur));
			pCur = pCur->GetNext();
			pCur->SetPoint(pCur->GetPoint() * Rot);
		}
		ret += PartsNum - 1;
	}
	return ret;
}

int TBladeCont::GlueArcs(void)
{
	for(TBladeEdge *pCur = GetStart(); pCur;)
	{
		if(pCur->IsArc())
		{
			TBladeEdge *pNext = pCur->GetNext();
			if(pNext->IsArc())
			{
				// Both edges are arcs
				if((pCur->GetCircCenter() - pNext->GetCircCenter()).D2() < MIND * MIND)
				{
					// Edges belong to the same circle
					if(pNext == GetStart())
						break;
					Remove(pNext);
					continue;
				}
			}
		}
		pCur = GetNext(pCur);
	}
	return 0;
}
void TBladeCont::Remove(TBladeEdge * pEdge)
{
	if(pEdge == GetStart())
	{
		pStart = pStart->GetNext();
		if(pStart == pEdge)
		{
			pStart = nullptr;
			delete pEdge;
			return;
		}
	}
	pEdge->GetPrev()->SetNext(pEdge->GetNext());
	pEdge->GetNext()->SetPrev(pEdge->GetPrev());
	delete pEdge;
}

NRectMesh * TBladeCont::CrMesh4TurnOneElem(const BMatr &Rot90, int ColorNum, int ColorPassNum, int CadrNum, const TBladeEdge *pCur, const NRectMesh *pPrevMesh) const
{
// CadrNum < 0 означает создание заготовки и, следовательно, необходимость
// устанавливать противоположные направления нормалей к поверхностям	
	BPoint N2 = GetNormal() * 2.;
	return CrMesh4OneElem(Rot90, ColorNum, ColorPassNum, CadrNum, pCur, pPrevMesh, N2, GetNormal());
}
void TBladeCont::CrMesh4Turn(MeshArr & MeshArray, int ColorNum, int ColorPassNum, int CadrNum) const
{
// CadrNum < 0 означает создание заготовки и, следовательно, необходимость
// устанавливать противоположные направления нормалей к поверхностям	
	BMatr Rot90(GetNormMatr());
	// Create meshs for one tool
	NRectMesh *pPrevMesh = nullptr;
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{
		NRectMesh *pMesh = CrMesh4TurnOneElem(Rot90, ColorNum, ColorPassNum, CadrNum, pCur, pPrevMesh);
		pPrevMesh = pMesh;
		MeshArray.Add(pMesh);
	}
	MeshArray.SetConvex(Convex);
}

const BBox TBladeCont::GetGabar(void) const
{
	BBox Gabar;
	for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
		Gabar.Expand(pCur->GetPoint());

	return Gabar;
}

int TBladeCont::RemoveShortEdges(void)
{
	int Ret = 0;
	for(TBladeEdge *pCur = GetStart(); pCur && GetStart(); )// GetStart may become nullptr in Remove
	{
		if(pCur->GetNext()->GetVec().D2() < MIND * MIND)
		{
			++Ret;
			Remove(pCur->GetNext());
		}
		else
			pCur = GetNext(pCur);
	}
	return Ret;
}

void TBladeCont::Serialize(CArchive & ar)
{
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 4.9))
		SerializeElements(ar, &Convex, 1);
	else
		Convex = false;
	SerializeElements(ar, &N, 1);
	if(ar.IsLoading())
	{
		int Length;
		ar >> Length;
		for(int i = 0; i < Length; ++i)
		{
			TBladeEdge *pCur = new TBladeEdge;
			AddEdge(pCur);
			pCur->Serialize(ar);
		}
	}
	else
	{
		ar << GetEdgesNum();
		for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
			pCur->Serialize(ar);
	}
}

int TBladeCont::GetEdgesNum(void) const
{
	int Num = 0;
	for(TBladeEdge *pCur = GetStart(); pCur; pCur = GetNext(pCur))
		++Num;
	return Num;
}

bool TBladeCont::AddEdge(TBladeEdge * pEdge)
{
	// add start if empty
	if(pStart == nullptr)
	{
		pStart = pEdge;
		pStart->SetPrev(pStart);
		pStart->SetNext(pStart);
	}
	else
		pStart->GetPrev()->InsertAfter(pEdge);
	return true;
}

void TBladeCont::DrawLines(void)
{// Convex contour only
	if(!GetStart())
		return;
	glBegin(GL_LINE_STRIP);
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
		glVertex3dv(pCur->GetPoint().GetArray());
	glVertex3dv(GetStart()->GetPoint().GetArray());
	glEnd();
}
bool TBladeCont::SweepLine(const TBladeCont &Cont, const NCadrGeom &Geom)
{
	MakeCopy(Cont);// Convex should stay unchanged
	// Geom must lie in XY plane
	// If Geom is arc it must be so small to correspond to given tolerance
	BPoint Dir = Geom.GetE() - Geom.GetB();
	if(Geom.GetAttr() == 7)// резание только в конце(TURN only)
	{
		// Cut in the end point only
		Trans(BMatr().Trans(BPoint(0.,0.,0.,1.), Dir));
		return true;
	}
	//Find signum change edges
	TBladeEdge *pPN = nullptr, *pNP = nullptr;
	FindExtrEdges(Dir, pPN, pNP);
	if(!pPN || !pNP)
		return false;
	// Do linear sweep
	pNP->GetPrev()->InsertAfter(new TBladeEdge(*pNP));
	pNP->GetPrev()->SetAttr(pNP->GetPrev()->GetAttr() & (~BE_DEGR));// must be same as pNP
	pNP->GetPrev()->SetCircCenter(BPoint(0.,0.,0.,-1.));
	pPN->GetPrev()->InsertAfter(new TBladeEdge(*pPN));
	pPN->GetPrev()->SetAttr(pPN->GetPrev()->GetPrev()->GetAttr() & (~BE_DEGR));
	pPN->GetPrev()->SetCircCenter(BPoint(0.,0.,0.,-1.));
	BMatr Tr;
	Tr.Trans(BPoint(0.,0.,0.,1.), Dir);
	for(TBladeEdge *pC = pNP; pC != pPN; pC = pC->GetNext())
		pC->Trans(Tr);
	// Process arcs
	if(Geom.IsArc())
	{
		CalcArc(Geom, pNP, pPN);
	}
	//remove DEGR
	TBladeEdge *pCur = GetStart();
	for(; pCur != nullptr; pCur = GetNext(pCur))
		if(!(pCur->GetAttr() & BE_DEGR))
			break;
	pStart = pCur;// pStart must point to non degrade edge
	for(; pCur != nullptr; pCur = GetNext(pCur))
	{
		if(pCur->GetAttr() & BE_DEGR)
		{
			pCur->GetNext()->SetPoint(pCur->GetPoint());
			pCur->GetNext()->SetPrev(pCur->GetPrev());
			pCur->GetPrev()->SetNext(pCur->GetNext());
			TBladeEdge *pBuf = pCur;
			pCur = pCur->GetPrev();
			delete pBuf;
		}
	}
	return true;
}
int TBladeCont::SweepCircle(double r, const NCadrGeom &Geom, TBladeCont *Sweeped)
{
	BPoint Pb = Geom.GetB();
//	double r = GetStart()->GetRad();
	int k = 0;
	NCadrGeom ArcPart(Geom);
	double sa, ea, ca;
	Geom.GetArcA(&sa, &ea, &ca);
	if(fabs(ca) > 180.)
	{
		BMatr Rot;
		BPoint Center = Geom.GetC();
		Rot.rotg(ca / 2, Center, Geom.GetN());
		BPoint Pe = Pb * Rot;
		ArcPart.SetE(Pe);
		ArcPart.Tr(BMatr().Trans(Pb, BPoint(0.,0.,0.,1.)));
		if(Sweeped[k].CircleSweep(ArcPart, r))
	        ++k;
		ArcPart = Geom;
		ArcPart.SetB(Pe);
		ArcPart.SetI(Center - Pe);
	}
	ArcPart.Tr(BMatr().Trans(Pb, BPoint(0.,0.,0.,1.)));
	if(Sweeped[k].CircleSweep(ArcPart, r))
	    ++k;
	return k;
}

int TBladeCont::SweepArc(const NCadrGeom &Geom, TBladeCont *Sweeped) const
{
	if(IsCircle())
	{
		int Res = SweepCircle(GetStart()->GetRad(), Geom, Sweeped);
		Sweeped->Trans(BMatr().Trans(BPoint(0., 0., 0., 1.), GetStart()->GetCircCenter()));
		return Res;
	}
	int SwStart = 0;
	const TBladeCont *pSwCont = this;
	TBladeCont TmpCont;
	if(HaveCutArc())
	{
		int k = 0;
		pSwCont = &TmpCont;
		TmpCont.MakeCopy(*this);
		TmpCont.GlueArcs();
		for(TBladeEdge *pCur = TmpCont.GetStart(); pCur != nullptr; pCur = TmpCont.GetNext(pCur))
			if(pCur->GetAttr() == BE_CUT && pCur->IsArc())
			{
				int i = k;
				k += SweepCircle(pCur->GetRad(), Geom, Sweeped + k);
				for(; i < k; ++i)
				{
					Sweeped[i].Trans(BMatr().Trans(BPoint(0.,0.,0.,1.), pCur->GetCircCenter()));
				}
				pCur->SetCircCenter(BPoint(0.,0.,0.,-1.));
			}
		SwStart = k;
	}
	// Approximate trajectory arc
	double sa, ea, ca;
	Geom.GetArcA(&sa, &ea, &ca);
	BPoint Center = Geom.GetC();
	BPoint Pb = Geom.GetB();
	int Num = NSurfSettings::GetNumAproxLine(Geom.GetR(), ca);
	if(Num + SwStart >= MAX_SWEEPS)
		Num = MAX_SWEEPS - 1 - SwStart;
	BMatr Rot;
	Rot.rotg(ca / Num, Center, Geom.GetN());
	int i = SwStart;
	for(; i < SwStart + Num; ++i)
	{
		BPoint Pe = Pb * Rot;
		NCadrGeom ArcPart(Geom);
		ArcPart.SetB(Pb);
		ArcPart.SetE(Pe);
		ArcPart.SetI(Center - Pb);
		Pb = Pe;
		Sweeped[i].SweepLine(*pSwCont, ArcPart);
		Sweeped[i].Trans(BMatr().Trans(Geom.GetB(), ArcPart.GetB()));
	}
	return i;
}

int TBladeCont::Sweep(TBladeCont *Res, const NCadrGeom &Geom) const
{
	// Geom must lie in XY plane
	if(!Geom.HaveGeom())
	{
		Res[0].MakeCopy(*this);
		return 1;
	}

	if(Geom.IsArc())
	{
		return SweepArc(Geom, Res);
	}
	else
	{
		Res[0].SweepLine(*this, Geom);
		return 1;
	}
}

int TBladeCont::FindExtrEdges(const BPoint & Dir, TBladeEdge *&pPN, TBladeEdge *&pNP)
{
	double dd = Dir.D2();
	if( dd <= 0.)
		return false;
	BPoint EDir(Dir * (1. / sqrt(dd)));
	BMatr Rot90 = GetNormMatr();// Matrix to get external normal
	TBladeEdge *pFirst = nullptr;
	double cos0 = 0;
	TBladeEdge *pCur = GetStart();
	for(; pCur != nullptr; pCur = GetNext(pCur))
	{
		double d = pCur->GetVec().D2();
		if( d <= MIND)
			continue;
		cos0 = (pCur->GetVec() * Rot90) * EDir / sqrt(d);
		if( fabs(cos0) < MINAR )
		{
			pCur->SetAttr(BE_DEGR | pCur->GetAttr());
			continue;
		}
		pFirst = pCur;
		break;
	}
	if(!pCur)
		return false;// Degraded contour
	// Find first sign change
	double cos1 = 0;
	for(pCur = pCur->GetNext(); pCur != pStart; pCur = pCur->GetNext())
	{
		double d = pCur->GetVec().D2();
		if( d <= MIND)
			continue;
		cos1 = (pCur->GetVec() * Rot90) * EDir / sqrt(d);
		if( fabs(cos1) < MINAR )
		{
			pCur->SetAttr(BE_DEGR | pCur->GetAttr());
			continue;
		}
		if(cos0 * cos1 < 0)
		{
			if(cos0 < 0)
				pNP = pCur;
			else
				pPN = pCur;
			break;
		}
	}
	if(pCur == pStart)
		return false;// Degraded contour
	// Find second sign change
	double cos2;
	for(pCur = pCur->GetNext(); pCur != pStart; pCur = pCur->GetNext())
	{
		double d = pCur->GetVec().D2();
		if( d <= MIND)
			continue;
		cos2 = (pCur->GetVec() * Rot90) * EDir / sqrt(d);
		if( fabs(cos2) < MINAR )
		{
			pCur->SetAttr(BE_DEGR | pCur->GetAttr());
			continue;
		}
		if(cos1 * cos2 < 0)
		{
			if(cos1 < 0)
				pNP = pCur;
			else
				pPN = pCur;
			break;
		}
	}
	if(pCur == pStart)
	{
		if(cos1 < 0.)
			pNP = pFirst;
		else
			pPN = pFirst;
	}
	else
	{
		// process the rest of the list
		double cos3;
		for(pCur = pCur->GetNext(); pCur != pStart; pCur = pCur->GetNext())
		{
			double d = pCur->GetVec().D2();
			if( d <= MIND)
				continue;
			cos3 = (pCur->GetVec() * Rot90) * EDir / sqrt(d);
			if( fabs(cos3) < MINAR )
			{
				pCur->SetAttr(BE_DEGR | pCur->GetAttr());
				continue;
			}
		}
	}
	return 0;
}
bool TBladeCont::CircleSweep(const NCadrGeom & Geom, double r)
{
	double R = Geom.GetR();
	if(R < MIND)
		return false;
	double sa, ea, ca;
	Geom.GetArcA(&sa, &ea, &ca);
	BPoint Center = Geom.GetC();
	BPoint Pb = Geom.GetB();
	BPoint Pe = Geom.GetE();
	if(Geom.IsCCWArc())
	{
		BPoint B = Pb;
		Pb = Pe;
		Pe = B;
	}
	BPoint Ib = Center - Pb;
	BPoint Ie = Center - Pe;
	BPoint Pb1 = Center - Ib * (1. - r / R);
	BPoint Pe1 = Center - Ie * (1. - r / R);

	BPoint Pb0 = Center - Ib * (1. + r / R);
	BPoint Pe0 = Center - Ie * (1. + r / R);
	if(R <= r)
	{
		BPoint V = Pe - Pb;
		double d = V.D2();
		if( d < MIND * MIND)
		{
			return false;
		}
		V = V * (1./sqrt(d));
		V = V * BMatr().rotg(-90., BPoint(0.,0.,0.,1.), Geom.GetN());
		double l = sqrt(r * r - 0.25 * d);
		Pe1 = (Pb + Pe) * 0.5 + V * l;
		Pb1 = Pe1;
	}

	TBladeEdge * pNewEdge = new TBladeEdge;
	pNewEdge->SetAttr(BE_CUT);
	pNewEdge->SetPoint(Pb0);
	pNewEdge->SetCircCenter(Center);
	AddEdge(pNewEdge);

	pNewEdge = new TBladeEdge;
	pNewEdge->SetAttr(BE_CUT);
	pNewEdge->SetPoint(Pb0 * BMatr().rotg(-fabs(ca) / 2, Center, Geom.GetN()));
	pNewEdge->SetCircCenter(Center);
	AddEdge(pNewEdge);

	pNewEdge = new TBladeEdge;
	pNewEdge->SetAttr(BE_CUT);
	pNewEdge->SetPoint(Pe0);
	pNewEdge->SetCircCenter(Pe);
	AddEdge(pNewEdge);

	if(R > r)
	{
		pNewEdge = new TBladeEdge;
		pNewEdge->SetAttr(BE_CUT);
		pNewEdge->SetPoint(Pe0 * BMatr().rotg(-91., Pe, Geom.GetN()));// 91 а не 90 для предотвращения совпадения плоскостей при построении BSP дерева
		pNewEdge->SetCircCenter(Pe);
		AddEdge(pNewEdge);

		pNewEdge = new TBladeEdge;
		pNewEdge->SetAttr(BE_CUT);
		pNewEdge->SetPoint(Pe1);
		pNewEdge->SetCircCenter(Center);
		AddEdge(pNewEdge);

		pNewEdge = new TBladeEdge;
		pNewEdge->SetAttr(BE_CUT);
		pNewEdge->SetPoint(Pe1 * BMatr().rotg(fabs(ca) / 2, Center, Geom.GetN()));
		pNewEdge->SetCircCenter(Center);
		AddEdge(pNewEdge);
	}

	pNewEdge = new TBladeEdge;
	pNewEdge->SetAttr(BE_CUT);
	pNewEdge->SetPoint(Pb1);
	pNewEdge->SetCircCenter(Pb);
	AddEdge(pNewEdge);

	if(R > r)
	{
		pNewEdge = new TBladeEdge;
		pNewEdge->SetAttr(BE_CUT);
		pNewEdge->SetPoint(Pb1 * BMatr().rotg(-91., Pb, Geom.GetN()));
		pNewEdge->SetCircCenter(Pb);
		AddEdge(pNewEdge);
	}

//	Trans(BMatr().Trans(BPoint(0.,0.,0.,1.), Geom.GetB() * (-1.)));
	BreakArcs();
	if(Geom.GetN().GetZ() > 0.)
	{
		Invert();
		N = N * (-1);
	}
	Convex = false;
	return true;
}

bool TBladeCont::HaveCutArc(void) const
{
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
		if(pCur->GetAttr() == BE_CUT && pCur->IsArc())
			return true;
	return false;
}
void TBladeCont::ReconCorners(double Angle)
{// Add corner if previous and next 2 lines are collinear and angle is larger than Angle
	for (TBladeEdge* pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{// Calc angles
		pCur->SetPointH(pCur->GetVec().Angle0_180(pCur->GetPrev()->GetVec()));
	}
	for (TBladeEdge* pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{
		if (pCur->GetPoint().GetH() > Angle && pCur->GetNext()->GetPoint().GetH() > Angle
			&& pCur->GetPrev()->GetPoint().GetH() < MINAD && pCur->GetNext()->GetNext()->GetPoint().GetH() < MINAD)
		{// Insert corner
			BPoint P0, P1;
			BPoint::LineLine(pCur->GetNext()->GetPoint(), pCur->GetNext()->GetVec()
				, pCur->GetPrev()->GetPoint(), pCur->GetPrev()->GetVec()
				, P0, P1);
			P0 = (P0 + P1) * 0.5;
			double MaxD2 = pCur->GetVec().D2();
			if (BPoint::Dist2(P0, pCur->GetPoint()) < MaxD2 && BPoint::Dist2(P0, pCur->GetNext()->GetPoint()) < MaxD2)
			{
				TBladeEdge* pEdge = new TBladeEdge(*pCur);
				pEdge->SetPoint(P0);
				pCur->InsertAfter(pEdge);
				pCur = GetNext(GetNext(pCur));
			}
		}
	}
	for (TBladeEdge* pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{// restore H
		pCur->SetPointH(1.);
	}
}

double TBladeCont::LineArea() const
{
	if (GetStart() == nullptr)
		return 0.;
	if (GetNext(GetStart()) == nullptr)
		return 0.;
	const BPoint& B = GetStart()->GetPoint();
	BPoint Prev = GetNext(GetStart())->GetPoint();
	Prev -= B;
	BPoint Sv(0., 0., 0., 0.);
	for (TBladeEdge* pCur = GetNext(GetNext(GetStart())); pCur != nullptr; pCur = GetNext(pCur))
	{
		BPoint Cur = pCur->GetPoint();
		Cur -= B;
		Sv += Prev % Cur;
		Prev = Cur;
	}
	return Sv.D2() * 0.25;
}

void TBladeCont::CrMesh4Mill(MeshArr & MeshArray, const BPoint &V, int ColorNum, int ColorPassNum, int CadrNum) const
{
// V - вектор перемещения контура
// CadrNum < 0 означает создание заготовки и, следовательно, необходимость
// устанавливать противоположные направления нормалей к поверхностям	
	BMatr Rot90(GetNormMatr());
	NRectMesh *pPrevMesh = nullptr;
	for(TBladeEdge *pCur = GetStart(); pCur != nullptr; pCur = GetNext(pCur))
	{
		NRectMesh *pMesh = CrMesh4OneElem(Rot90, ColorNum, ColorPassNum, CadrNum, pCur, pPrevMesh, V * 0.5, GetNormal());
		pPrevMesh = pMesh;
		MeshArray.Add(pMesh);
	}
}
NRectMesh * TBladeCont::CrMesh4OneElem(const BMatr &Rot90, int ColorNum, int ColorPassNum, int CadrNum, const TBladeEdge *pCur
	, const NRectMesh *pPrevMesh, const BPoint &V, const BPoint& Norm)
{
// CadrNum < 0 означает создание заготовки и, следовательно, необходимость
// устанавливать противоположные направления нормалей к поверхностям	
	ElemID ElID = ID_EMPTY + SURF_NUM_SHIFT;
	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetSize(2, 2);
	pMesh->m_CadrNum = CadrNum;
	pMesh->m_IsDeg = 0;
	pMesh->m_sgn = 1;
	pMesh->SetMeshID(MElemIDProc::SetPCn(&ElID
		, pCur->GetAttr() == BE_CUT ? ColorNum : ColorPassNum));
	pMesh->m_arr[0][0] = MFOREST.AddPoints(pCur->GetPoint() - V);
	pMesh->m_arr[0][1] = MFOREST.AddPoints(pCur->GetNext()->GetPoint() - V);
	pMesh->m_arr[1][0] = MFOREST.AddPoints(pCur->GetPoint() + V);
	pMesh->m_arr[1][1] = MFOREST.AddPoints(pCur->GetNext()->GetPoint() + V);
	BPoint Center = pCur->GetCircCenter();
	if(Center.GetH() < 0.)
	{
		BPoint Normal = ((pCur->GetNext()->GetPoint() - pCur->GetPoint()) * Rot90) * ((CadrNum < 0) ? -1. : 1.);// CadrNum < 0 означает создание заготовки и, следовательно, необходимость устанавливать противоположные направления нормалей к поверхностям
		Normal.Make1(1.e-8);
		pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(Normal, pCur->GetPoint())); 
	}
	else
	{
		pMesh->m_iSurf = -1;
		if(pPrevMesh)
		{
			if(pPrevMesh->m_iSurf >= 0)
			{
				BPoint PrevCenter = MFOREST.GetOrigSurf(pPrevMesh->m_iSurf)->GetCenter();
				if((PrevCenter - pCur->GetCircCenter()).D2() < MIND * MIND)
					pMesh->m_iSurf = pPrevMesh->m_iSurf;
			}
		} 
		if(pMesh->m_iSurf < 0)// iSurf is not the same as previous
		{
			double Rad = sqrt((pCur->GetPoint() - pCur->GetCircCenter()).D2());
			// Change sign if necessary
			double Ang = ((pCur->GetCircCenter() - pCur->GetPoint())
				.Angle((pCur->GetNext()->GetPoint() - pCur->GetPoint()), Norm));
			if(Ang < 0.)
				Rad *= -1.;
			Rad *= ((CadrNum < 0) ? 1. : -1.);// CadrNum < 0 означает создание заготовки и, следовательно, необходимость устанавливать противоположные направления нормалей к поверхностям
			pMesh->m_iSurf = MFOREST.AddOrigSurf(
				new SOCyl(pCur->GetCircCenter(),
					Norm,
					Rad)); 
		}
	}
	return pMesh;
}
