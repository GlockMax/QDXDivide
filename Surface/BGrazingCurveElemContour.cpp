#include "StdAfx.h"
#include "ConstDef.h"
#include "NSurfSettings.h"
#include "MBSPForest.h"
#include "BGrazingCurveElemContour.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BGrazingCurveElemContour::BGrazingCurveElemContour(void)
{
	Isgcleft = NULL;
	Isgcright = NULL;
	Isgcleft1 = NULL;
	Isgcright1 = NULL;
	Isgcldown = NULL;
	Isgcrdown = NULL;
	Isgclup = NULL;
	Isgcrup = NULL;
}

BGrazingCurveElemContour::~BGrazingCurveElemContour(void)
{
	delete Isgcleft;
	delete Isgcright;
	delete Isgcleft1;
	delete Isgcright1;
	delete Isgcldown;
	delete Isgcrdown;
	delete Isgclup;
	delete Isgcrup;
}

void BGrazingCurveElemContour::MakeCopy(BGrazingCurveElemContour &Dest, const BMatr &M)
{
#define CopyI(Name) MakeCopy(Name, Dest.Name, M);
	CopyI(Isgcleft);
	CopyI(Isgcright);
	CopyI(Isgcleft1);
	CopyI(Isgcright1);
	CopyI(Isgcldown);
	CopyI(Isgcrdown);
	CopyI(Isgclup);
	CopyI(Isgcrup);
}

void BGrazingCurveElemContour::MakeCopy(const BSegmentGrazingCurve *pSource, BSegmentGrazingCurve *&pDest, const BMatr &M)
{
	if(!pSource)
	{
		pDest = NULL;
		return;
	}
	pDest = new BSegmentGrazingCurve;
	pDest->SetColor(pSource->GetColor());
	pDest->SetSize(pSource->GetSize());
	for(INT_PTR i = 0; i < pSource->GetSize(); ++i)
	{
		pDest->SetAt(i, MFOREST.AddPoints((*MFOREST.GetPoint(pSource->GetAt(i))) * M));
	}
}

void BGrazingCurveElemContour::Connect(const BSegmentGrazingCurve *pSource, BSegmentGrazingCurve *pDest, bool Invert, NTriMesh *pMesh)
{
	// pSource and pDest should be the same curves in different positions but may be divided differently
	if(!pSource || !pDest)
		return;

	bool Reverse = pSource->GetColor();
	auto s0 = pSource->GetAt(0);
	auto s1 = pDest->GetAt(0);
	INT_PTR SourceSize = pSource->GetSize();
	INT_PTR DestSize = pDest->GetSize();
	if (SourceSize != DestSize)
	{// Divided differently
		INT_PTR MinSize = min(SourceSize, DestSize);
		for (INT_PTR i = 1; i < MinSize; ++i)
		{
			auto e0 = pSource->GetAt(i);
			auto e1 = pDest->GetAt(i);
			if (Reverse)
				ApproxQuadrangle(s1, s0, e0, e1, pMesh, NSurfSettings::GetTolerance() * 0.5);
			else
				ApproxQuadrangle(s0, s1, e1, e0, pMesh, NSurfSettings::GetTolerance() * 0.5);
			s0 = e0;
			s1 = e1;
		}
		bool SourceLonger = (SourceSize > DestSize);
		const BSegmentGrazingCurve *pLong = SourceLonger ? pSource : pDest;
		auto eShort = SourceLonger ? s1 : s0;// End point of the short curve
		auto s = pLong->GetAt(MinSize - 1);
		INT_PTR MaxSize = max(SourceSize, DestSize);
		for (INT_PTR i = MinSize; i < MaxSize; ++i)
		{
			auto e = pLong->GetAt(i);
			if (Reverse == SourceLonger)
				pMesh->AddTri(eShort, s, e);
			else
				pMesh->AddTri(eShort, e, s);
			s = e;
		}
		return;
	}
	for(INT_PTR i = 1; i < DestSize; ++i)
	{
		auto e0 = pSource->GetAt(i);
		auto e1 = pDest->GetAt(i);
		if (Reverse)
			ApproxQuadrangle(s1, s0, e0, e1, pMesh, NSurfSettings::GetTolerance() * 0.5);
		else
			ApproxQuadrangle(s0, s1, e1, e0, pMesh, NSurfSettings::GetTolerance() * 0.5);
		s0 = e0;
		s1 = e1;
	}
}

void BGrazingCurveElemContour::StoreInForest(const GrazingCurveElemContour &Elem, int &LPNColorFalse, int &LPNColorTrue, bool First, const BMatr &M)
{
	int LastPointNumFalse = LPNColorFalse;
	bool lFirstLeft = First;
	int LastPointNumTrue = LPNColorTrue;
	bool lFirstRight = First;
	bool ColorRight = IsColorRight(Elem.sgclup, Elem.sgcrup);
	(ColorRight ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgclup, Isgclup, ColorRight ? LastPointNumFalse : LastPointNumTrue, lFirstLeft, M);
	((!ColorRight) ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcrup, Isgcrup, (!ColorRight) ? LastPointNumFalse : LastPointNumTrue, lFirstRight, M);

	ColorRight = IsColorRight(Elem.sgcleft, Elem.sgcright);
	(ColorRight ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcleft, Isgcleft, ColorRight ? LastPointNumFalse : LastPointNumTrue, lFirstLeft, M);
	((!ColorRight) ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcright, Isgcright, (!ColorRight) ? LastPointNumFalse : LastPointNumTrue, lFirstRight, M);


	ColorRight = IsColorRight(Elem.sgcleft1, Elem.sgcright1);
	bool lFirst1Right = true;
	bool lFirst1Left = true;
	(ColorRight ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcleft1, Isgcleft1, ColorRight ? LastPointNumFalse : LastPointNumTrue, lFirst1Left, M);
	((!ColorRight) ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcright1, Isgcright1, (!ColorRight) ? LastPointNumFalse : LastPointNumTrue, lFirst1Right, M);


	ColorRight = IsColorRight(Elem.sgcldown, Elem.sgcrdown);
	lFirstRight = lFirst1Right ? lFirstRight : false;
	lFirstLeft = lFirst1Left ? lFirstLeft : false;
	(ColorRight ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcldown, Isgcldown, ColorRight ? LastPointNumFalse : LastPointNumTrue, lFirstLeft, M);
	((!ColorRight) ? LastPointNumFalse : LastPointNumTrue) = StoreSegmInForest(Elem.sgcrdown, Isgcrdown, (!ColorRight) ? LastPointNumFalse : LastPointNumTrue, lFirstRight, M);
	LPNColorFalse = LastPointNumFalse;
	LPNColorTrue = LastPointNumTrue;
}
int BGrazingCurveElemContour::StoreSegmInForest(const SegmentGrazingCurve &Segm, BSegmentGrazingCurve *&PtsNums, int FirstPointNum, bool &First, const BMatr &M)
{
	if(Segm.Getkolgp() < 2)
		return FirstPointNum;
	PtsNums = new BSegmentGrazingCurve;
	PtsNums->SetColor(Segm.GetColor());
	bool IsCurve = Segm.IsCurve();
	int NewKol = Segm.Getkolgp();
	NLine Curve;
	if(IsCurve)
	{
		Curve.Set(Segm.GetGP(0), Segm.GetGP(1), Segm.GetGP(2), Segm.GetGP(3));
		NewKol = Curve.GetNumAproxLine(NSurfSettings::GetTolerance()) + 1;
		//NewKol = 3;
	}
	PtsNums->SetSize(NewKol);
	int i = 0;
	if(!First && FirstPointNum >= 0)
	{
		PtsNums->SetAt(i, FirstPointNum);
		++i;
	}
	for(; i < NewKol; ++i)
	{
		BPoint CurP; 
		if(IsCurve)
			CurP = Curve.GetPointFromParam(double(i) / (NewKol - 1));
		else
			CurP = Segm.GetGP(i);
			
		PtsNums->SetAt(i, MFOREST.AddPoints((CurP * M).Norm()));
	}

	First = false;
	return PtsNums->GetAt(PtsNums->GetSize() - 1);
}

BGrazingCurveElemContour::BSegmentGrazingCurve *BGrazingCurveElemContour::GetCont(bool Left, bool Top, bool Side)
{
	int Key = (Left ? 4 : 0) + (Top ? 2 : 0) +  (Side ? 1 : 0);
	switch(Key)
	{
	case 0:
		return Isgcrdown;
	case 1:
		return Isgcright1;
	case 2:
		return Isgcrup;
	case 3:
		return Isgcright;
	case 4:
		return Isgcldown;
	case 5:
		return Isgcleft1;
	case 6:
		return Isgclup;
	case 7:
		return Isgcleft;
	}
	return Isgcrdown;
}

void BGrazingCurveElemContour::RemoveDegraded(GrazingCurveElemContour & Segm)
{
	RemDegraded(Segm.sgcrup);
	RemDegraded(Segm.sgcrdown);
	RemDegraded(Segm.sgcright);
	RemDegraded(Segm.sgcright1);
	RemDegraded(Segm.sgclup);
	RemDegraded(Segm.sgcldown);
	RemDegraded(Segm.sgcleft);
	RemDegraded(Segm.sgcleft1);
}
void BGrazingCurveElemContour::RemDegraded(SegmentGrazingCurve & Segm)
{
	int N = Segm.Getkolgp();
	if(N == 0)
		return;
	double D2 = (Segm.GetGP(0) - Segm.GetGP(N - 1)).D2();
	if(D2 < MIND * MIND)
	{
		Segm.Clear();
		//Segm.Setkolgp(0);
	}
	else if (Segm.IsCurve())
	{// TEMPORARY
		if (D2 < 0.01)
			Segm.Clear();
	}

}
//void BGrazingCurveElemContour::CreateCadrBegin() const//!!!
//{
//}
void BGrazingCurveElemContour::ConnectElems(const BGrazingCurveElemContour & In, NTriMesh *pMesh) const
{
#define ConnectP(Name, Invert) Connect(Name, In.Name, Invert, pMesh);
	ConnectP(Isgcleft, false);
	ConnectP(Isgcright, true);
	ConnectP(Isgcleft1, false);
	ConnectP(Isgcright1, true);
	ConnectP(Isgcldown, false);
	ConnectP(Isgcrdown, true);
	ConnectP(Isgclup, false);
	ConnectP(Isgcrup, true);
}
//void BGrazingCurveElemContour::ConnectGCElemContour(NTriMesh *pMesh, BGrazingCurveElemContour & gc) //!!!
//{
//	for(int x = 0; x < 2; x++)
//		for(int y = 0; y < 2; y++)
//			for(int z = 0; z < 2; z++)
//			{
//				bool p = x ? true : false;
//				bool q = y ? true : false;
//				bool r = z ? true : false;
//
//				CUIntArray *a = GetCont(p, q, r);
//				CUIntArray *b = gc.GetCont(p, q, r);
//				if(a && b)//если нет пары сегментов для соединения, то ничего не соединяем //???
//				{
//					double	la = CallLong(p, q, r),		//длина сегмента a
//							lb = gc.CallLong(p, q, r),	//длина сегмента b (сегмент прошлого положения)
//
//							li = 0.0,	//длина сегмента от начала до i-й точки на сегменте a
//							lj = 0.0,	//длина сегмента от начала до j-й точки на сегменте b
//							ti = 0.0,
//							tj = 0.0,
//
//							pi = 0.0,	//процент удаленности i-й точки от начала сегмента
//							pj = 0.0,	//процент удаленности j-й точки от начала сегмента
//							tmpi = 0.0,
//							tmpj = 0.0;
//
//					auto kol1 = a->GetSize();
//					auto kol2 = b->GetSize();
//
//					for(int i = 0, j = 0; i < kol1-1 || (j < kol2 - 1); )
//					{
//						BPoint A, B;
//						if(i < kol1-1)
//						{
//							A = *(MFOREST.GetPoint(a->GetAt(i+1)));
//							B = *(MFOREST.GetPoint(a->GetAt(i)));
//							li = sqrt((A - B).D2());
//						}
//						if(j < kol2-1)
//						{
//							A = *(MFOREST.GetPoint(b->GetAt(j+1)));
//							B = *(MFOREST.GetPoint(b->GetAt(j)));
//							lj = sqrt((A - B).D2());
//						}
//
//						pi = (ti+li)/la;
//						pj = (tj+lj)/lb;
//
//						//если новое процентное отношение шага по i отличается меньше от j-го чем по j от противолежащего i-го, то шагаем по i
//						if(p)//для левой стороны
//						{
//							if(abs(pi-tmpj) < abs(pj-tmpi))
//							{
//								pMesh->AddTri(a->GetAt(i),a->GetAt(i+1),b->GetAt(j));
//								tmpi = pi;//запомнили процент длины новой точки по i
//								ti+=li;
//								i++;
//							}
//							else
//							{
//								pMesh->AddTri(a->GetAt(i),b->GetAt(j+1),b->GetAt(j));
//								tmpj = pj;//запомнили процент длины новой точки по j
//								tj+=lj;
//								j++;
//							}
//						}
//						else//для правой стороны
//						{
//							if(abs(pi-tmpj) < abs(pj-tmpi))
//							{
//								pMesh->AddTri(b->GetAt(j),a->GetAt(i+1),a->GetAt(i));
//								tmpi = pi;//запомнили процент длины новой точки по i
//								ti+=li;
//								i++;
//							}
//							else
//							{
//								pMesh->AddTri(b->GetAt(j),b->GetAt(j+1),a->GetAt(i));
//								tmpj = pj;//запомнили процент длины новой точки по j
//								tj+=lj;
//								j++;
//							}
//						}
//					}
//				}
//			}
//}
//double BGrazingCurveElemContour::CallLong(bool Left, bool Top, bool Side)//!!!
//{
//
//	CUIntArray * segm = GetCont(Left, Top, Side);
//	if(!segm)
//		return 0.0;
//	BPoint A, B;
//	double l = 0.0;	//длина сегмента
//	for(int i = 1; i < segm->GetSize(); i++)
//	{
//		A = *(MFOREST.GetPoint(segm->GetAt(i)));
//		B = *(MFOREST.GetPoint(segm->GetAt(i-1)));
//		l+=sqrt((A - B).D2());
//	}
//	return l;
//}
//double BGrazingCurveElemContour::CallLongi(bool Left, bool Top, bool Side, int i)//!!!
//{
//	double l = 0.0;
//	CUIntArray * segm = GetCont(Left, Top, Side);
//	for(int j = 1; j < i; j++)
//		l+=sqrt((  *(MFOREST.GetPoint(segm->GetAt(i))) - *(MFOREST.GetPoint(segm->GetAt(i-1)))  ).D2());
//	return l;
//}

void BGrazingCurveElemContour::Copy(BGrazingCurveElemContour & bgc, bool f)
{
	//f == true - копирование со сменой правой и левой стороны, f == false -  обычное копирование
	if(f)
	{
		Isgcleft = bgc.GetCont(false, true, true);
		Isgcright = bgc.GetCont(true, true, true);
		Isgcleft1 = bgc.GetCont(false, false, true);
		Isgcright1 = bgc.GetCont(true, false, true);
		Isgcldown = bgc.GetCont(false, false, false);
		Isgcrdown = bgc.GetCont(true, false, false);
		Isgclup = bgc.GetCont(false, true, false);
		Isgcrup = bgc.GetCont(true, true, false);
	}
	else
	{
		Isgcleft = bgc.GetCont(true, true, true);
		Isgcright = bgc.GetCont(false, true, true);
		Isgcleft1 = bgc.GetCont(true, false, true);
		Isgcright1 = bgc.GetCont(false, false, true);
		Isgcldown = bgc.GetCont(true, false, false);
		Isgcrdown = bgc.GetCont(false, false, false);
		Isgclup = bgc.GetCont(true, true, false);
		Isgcrup = bgc.GetCont(false, true, false);
	}
}

void BGrazingCurveElemContour::Clear(void)
{
	Isgcleft = NULL;
	Isgcright = NULL;
	Isgcleft1 = NULL;
	Isgcright1 = NULL;
	Isgcldown = NULL;
	Isgcrdown = NULL;
	Isgclup = NULL;
	Isgcrup = NULL;
}

const SegmentGrazingCurve &BGrazingCurveElemContour::GetByColor(bool Color, const SegmentGrazingCurve &Left, const SegmentGrazingCurve &Right)
{
	if (Left.Getkolgp() > 0)
	{
		if (Left.GetColor() == Color)
			return Left;
		else
			return Right;
	}
	if (Right.Getkolgp() > 0)
	{
		if (Right.GetColor() == Color)
			return Right;
		else
			return Left;
	}
	// Both are empty
	return Left;
}

bool BGrazingCurveElemContour::IsColorRight(const SegmentGrazingCurve &Left, const SegmentGrazingCurve &Right)
{
	// Returns true if color of left element is false
	if (Left.Getkolgp() > 0)
	{
		return (Left.GetColor() == false);
	}
	if (Right.Getkolgp() > 0)
	{
		return (Right.GetColor() == true);

	}
	// Both are empty
	return true;
}

void BGrazingCurveElemContour::ApproxQuadrangle(int iP0, int iP1, int iP2, int iP3, NTriMesh *pMesh, double Eps)
{
	BPoint P0(*MFOREST.GetPoint(iP0));
	BPoint P1(*MFOREST.GetPoint(iP1));
	BPoint P2(*MFOREST.GetPoint(iP2));
	BPoint P3(*MFOREST.GetPoint(iP3));

	BPoint P02((P0 + P2) * 0.5);
	BPoint P13((P1 + P3) * 0.5);
	double D2 = (P02 - P13).D2();
	if (D2 != D2)
		return;
	if (1)//D2 < Eps * Eps)
	{
		pMesh->AddTri(iP0, iP1, iP2);
		pMesh->AddTri(iP2, iP3, iP0);
		return;
	}
	// Make new points
	if ((P0 - P1).D2() > (P1 - P2).D2())
	{
		int New0 = MFOREST.AddPoints((P0 + P1) * 0.5);
		int New1 = MFOREST.AddPoints((P2 + P3) * 0.5);
		ApproxQuadrangle(iP0, New0, New1, iP3, pMesh, Eps);
		ApproxQuadrangle(New0, iP1, iP2, New1, pMesh, Eps);
	}
	else
	{
		int New0 = MFOREST.AddPoints((P0 + P3) * 0.5);
		int New1 = MFOREST.AddPoints((P1 + P2) * 0.5);
		ApproxQuadrangle(iP0, iP1, New1, New0, pMesh, Eps);
		ApproxQuadrangle(New0, New1, iP2, iP3, pMesh, Eps);
	}
}

void BGrazingCurveElemContour::CalcSurf(const BMatrPair &Stock2Tool, const class BScrewMotion &Motion)
{

}