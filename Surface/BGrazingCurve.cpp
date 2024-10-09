#include "StdAfx.h"
#include "NContourOfTool.h"
#include "BScrewMotion.h"
#include "NMeshArray.h"
#include "BGrazingCurveElemContour.h"
#include "BGrazingCurve.h"
#include "BAnglesArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BGrazingCurve::BGrazingCurve(int k) : GrazingCurve(k)
{
	BGCurve = new BGrazingCurveElemContour[k];
}
BGrazingCurve::BGrazingCurve(void)
{
}
BGrazingCurve::~BGrazingCurve(void)
{
	delete [] BGCurve;
}
void BGrazingCurve::StoreInForest(const BMatr &M)
{
	int LPNColorFalse = -1, LPNColorTrue = -1;
	bool First = true;
	for(int i = 0; i < Getkol(); ++i)
	{
		BGCurve[i].StoreInForest(GCurve[i], LPNColorFalse, LPNColorTrue, First, M);
		First = (GCurve[i].GetConnect() == YESUP || GCurve[i].GetConnect() == NOT);
	}
}
BGrazingCurveElemContour::BSegmentGrazingCurve *BGrazingCurve::GetCont(int iElem, bool Left, bool Top, bool Side) const
{
	return BGCurve[iElem].GetCont(Left, Top, Side);
}
void BGrazingCurve::RemoveDegraded(void)
{
	for(int i = 0; i < Getkol(); ++i)
	{
		BGrazingCurveElemContour::RemoveDegraded(GCurve[i]);
	}
	// Remove last element if it is not connected with previous
	int Index = Getkol() - 2;
	if (Index >= 0)
		if (GetGrCurve(Index)->GetConnect() == NOT || GetGrCurve(Index)->GetConnect() == YESUP)
			GetGrCurve(Index + 1)->Clear();
	//if (GetGrCurve(0)->GetConnect() == NOT || GetGrCurve(0)->GetConnect() == YESUP)
	//	GetGrCurve(0)->Clear();

}

GrazingCurveElemContour &BGrazingCurve::GetElemCurve(int iElem) const
{
	return GCurve[iElem];
}

void BGrazingCurve::ConnectCurves(const BGrazingCurve & In, MeshArr *pMeshArr, INT_PTR ConCompNum) const
{
	auto MeshArrI = pMeshArr->GetSize() - ConCompNum;
	//TEMPORARY
	BGrazingCurveElemContour::BSegmentGrazingCurve *pLeft = GetGCurve(0)->GetCont(true, true, true);
	BGrazingCurveElemContour::BSegmentGrazingCurve *pRight = GetGCurve(0)->GetCont(false, true, true);
	BGrazingCurveElemContour::BSegmentGrazingCurve *pInLeft = In.GetGCurve(0)->GetCont(true, true, true);
	BGrazingCurveElemContour::BSegmentGrazingCurve *pInRight = In.GetGCurve(0)->GetCont(false, true, true);
	if (pLeft && pRight && pInLeft && pInRight)
	{
		static_cast<NTriMesh *>(pMeshArr->GetAt(MeshArrI))->AddTri(pInLeft->GetAt(0), pLeft->GetAt(0), pRight->GetAt(0));
		static_cast<NTriMesh *>(pMeshArr->GetAt(MeshArrI))->AddTri(pRight->GetAt(0), pInRight->GetAt(0), pInLeft->GetAt(0));
	}
	for (int i = 0; i < kol; i++)
	{
		if (i > 0 && (GetGrCurve(i - 1)->GetConnect() == NOT || GetGrCurve(i - 1)->GetConnect() == YESUP))
			++MeshArrI;
		GetGCurve(i)->ConnectElems(*(In.GetGCurve(i)), static_cast<NTriMesh *>(pMeshArr->GetAt(MeshArrI)));
	}
}

BGrazingCurveElemContour * BGrazingCurve::GetGCurve(int i) const//!!!
{
	if(i < 0 || i > kol-1)
		return NULL;
	return &BGCurve[i];
}

void BGrazingCurve::CalcSurf(const NContourOfTool &Contour, const std::vector<int> C2Contour, const BMatrPair &Stock2Tool, const class BScrewMotion &Motion)
{
	if (Motion.GetSpecialCase().val == 0)
		return;// is not special case
	if (C2Contour.size() != kol)
		return;// internal error
	int *SurfNums = new int[Contour.num * 2];// [2i + 0] - near to the axis surf, [2i + 1] - far
	Contour.CalcSurf4Elem(Stock2Tool, Motion, SurfNums);
		//GetGrCurve(i);
		//GetGCurve(i)->CalcSurf(Stock2Tool, Motion);
	delete[] SurfNums;
}

void BGrazingCurve::BMakeCopy(BGrazingCurve &Dest, const BMatr &TrM) const
{
	for (int i = 0; i < kol; i++)
		GetGCurve(i)->MakeCopy(*Dest.GetGCurve(i), TrM);
}