#include "..\Common\BGeomArray.h"
#include "..\Common\BGeomArray.h"
#include "..\Common\BGeomArray.h"
#include "..\Common\BGeomArray.h"
// BGeomArray.cpp: implementation of the BGeomArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GL\gl.h"
#include "NCadrGeom.h"
#include "ConstDef.h"
#include "BGeomArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double C1 = 1. / sqrt(14.);
#define CALC_D(X, Y, Z) (((X) + 2. * (Y) + 3. * (Z)) * C1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BGeomArray::BGeomArray()
{
	Closed = FALSE;
}

BGeomArray::~BGeomArray()
{
	for(int i = 0; i < GetSize(); ++i)
		delete GetAt(i);
}

void BGeomArray::RemoveAll()
{
// Removes all elements pointed by array members and array itself
	if(GetSize() == 0)
		return;

	for(int i = 0; i < GetSize(); ++i)
		delete GetAt(i);
	NCadrGeomArray::RemoveAll();
}

int BGeomArray::AddElement(const NCadrGeom &el)
{
// Adds new copy of el to array if it is not closed
	if(Closed)
		return 0;

	NCadrGeom *temp = new NCadrGeom(el);
	return int(Add(temp));
}

void BGeomArray::Close(bool BiArc)
{
	Closed = TRUE;
	if(GetSize() < 1)
		return;
	NCadrGeom *pPrevFitCadr = GetAt(0);
	bool CycleAppr = false;
	for(int i = 1; i < GetSize(); ++i)
	{
		NCadrGeom *pCurFitCadr = GetAt(i);
		if(pCurFitCadr->IsUndef())
			continue;
		if(pPrevFitCadr->IsCycle() != pCurFitCadr->IsCycle())
		{
			CycleAppr = !pCurFitCadr->IsCycle();
			pPrevFitCadr = pCurFitCadr;
			continue;
		}
		if(CycleAppr)
		{
			CycleAppr = false;
			pPrevFitCadr = pCurFitCadr;
			continue;
		}
		NCadrGeom::FitCadrs(pPrevFitCadr, pCurFitCadr, BiArc);
		if(pCurFitCadr->HaveGeom())// small cadr was set undef in FitCadrs
			pPrevFitCadr = pCurFitCadr;
	}
}

void BGeomArray::Open() noexcept
{
	Closed = FALSE;
}

void BGeomArray::Draw(int FirstHCadr, int LastHCadr, int BoldCadr)
{
// Draw wireframe image of array
// from the FirstHCadr to the LastHCadr
// if LastHCadr < 0 draw whole array

	glFinish();

	int n = int(GetSize());
	if(LastHCadr < n && LastHCadr >=0)
		n = LastHCadr;
	int b = 0;
	if(FirstHCadr > 0)
		b = FirstHCadr;
	NCadrGeom *pGeom;

	for(int i = 0, k = 0; k < n && i < GetSize(); i++)
	{
// k - index of current step excluding steps in the cycles
// i - index of current step


		pGeom = GetAt(i);
		if(!pGeom->IsCycle())// Cycle attrib
			k++;
		if(k < b)
			continue;// Don't draw cadrs before the "b"

		glPushName(i);
		switch(pGeom->GetAttr())
		{
		case 1:// Cycle attrib
		case 6:// Cycle attrib
		case 7:// Cycle attrib
			glPushAttrib(GL_CURRENT_BIT);
//			glColor4fv(Color.array);
			break;
		case 2:// Spindle off
			glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT );
			glEnable(GL_LINE_STIPPLE);
			glLineStipple( 1/*factor*/, 0xcccc /* pattern */  );
//			glColor4fv(FastMovCol.array);
			break;
		case 3:
			glPushAttrib(GL_CURRENT_BIT);
			glColor3f(1.,1.,0.);
			break;
		case 4:
			glPushAttrib(GL_CURRENT_BIT);
			glColor3f(0.,0.,1.);
			break;
		default:
			glPushAttrib(GL_CURRENT_BIT);
			break;
		}
		if(k == BoldCadr)
		{
			glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
			GLdouble curcol[4];
			glGetDoublev(GL_CURRENT_COLOR, curcol);
			curcol[0] = 1. - curcol[0];
			curcol[1] = 1. - curcol[1];
			curcol[2] = 1. - curcol[2];
			glColor3dv(curcol);
			glLineWidth(3.);
		}
		pGeom->Draw(0.001, -1.);
		if(k == BoldCadr)
		{
			glPopAttrib();
		}


		glPopAttrib();

		glPopName();
	}

	glFinish();

}

void BGeomArray::MakeContinuous(void)
{// Previous element is unchanged, next element changes
	for(int i = 0; i < GetSize(); ++i)
	{
		if(!GetAt(i)->HaveGeom())
			continue;
		int k;
		for(k = i + 1; k < GetSize(); ++k)
		{
			if(GetAt(k)->HaveGeom() || GetAt(k)->IsRotate())
				break;
		}
		if(k == GetSize())
			continue;
		if(GetAt(k)->IsRotate())
			continue;
		GetAt(k)->SetB(GetAt(i)->GetE());
	}
}

void BGeomArray::Reverse(void)
{
	for(int i = 0; i < GetSize(); ++i)
		GetAt(i)->Reverse();
	for(int i = 0, j = int(GetSize()) - 1; i < GetSize() / 2; ++i, --j)
	{
		NCadrGeom *pB = GetAt(i);
		SetAt(i, GetAt(j));
		SetAt(j, pB);
	}
}

double BGeomArray::GetVolumeRot(void) const
{
	double V = 0.;
	for(int i = 0; i < GetSize(); ++i)
		V += GetAt(i)->GetVolumeRot();

	return V;
}

int BGeomArray::FindIndBTime(unsigned int Time) const
{
	// array should be sorted by time
	const auto Count = GetSize();
	if (Count == 0)
		return -1;
	if (Time < GetAt(0)->GetStartTime())
		return -1;
	if (Time > GetLast()->GetEndTime())
		return int(Count);
	int b = 0;
	int e = int(Count - 1);
	while (b != e)
	{
		int m = (e + b) / 2;
		const NCadrGeom* pGeom = GetAt(m);
		if (pGeom == nullptr)
			return -1;
		if (Time <= pGeom->GetEndTime())
			e = m;
		else
			b = m + 1;
	}
	return b;
}

int BGeomArray::Sort(double PointTol)
{
	if(IsEmpty())
		return 0;
	// A. Find coincident points
	auto PointsNumber = 2 * GetSize();
	int *PNums = new int[PointsNumber]; // source point numbers array
	for (int i = 0; i < PointsNumber; PNums[i++] = i);
	int *PairNums = new int[PointsNumber]; // PairNums[i] - the number of a point coincident with the point number i
	for (int i = 0; i < PointsNumber; PairNums[i++] = -1);
	// 1. Sort PNums

	QuickSort(PNums, PointsNumber);
	// PNums сейчас содержит номера точек в хранилище, упорядоченные вдоль прямой

	// 2. Identify all points

	double MaxD = 0.;
	for (int i = 0; i < PointsNumber; ++i)
	{
		if (PNums[i] < 0) // Point is deleted
			continue;
		double MinD2 = 1.e18;
		int MinInd = -1;
		BPoint Pi(GetPointByStorNum(PNums[i]));
		for (int k = i + 1; k < PointsNumber; ++k) // Find a point coincident with i'th point
		{
			if (PNums[k] < 0) // Point is deleted
				continue;
			BPoint Pk(GetPointByStorNum(PNums[k]));
			BPoint Delta(Pk - Pi);
			if (CALC_D(Delta.GetX(), Delta.GetY(), Delta.GetZ()) > PointTol)
				break;
			double D2 = Delta.D2();
			if (D2 < MinD2)
			{
				MinInd = k;
				MinD2 = D2;
			}
		}
		if (MinD2 < PointTol * PointTol)
		{
			PairNums[PNums[MinInd]] = PNums[i];
			PairNums[PNums[i]] = PNums[MinInd];
			PNums[MinInd] = -1;
		}
		else
			PairNums[PNums[i]] = -1;
	}
	bool *DeletedPoints = new bool[PointsNumber];
	for (int i = 0; i < PointsNumber; ++i)
		DeletedPoints[i] = false;
	int SortedCount = 0;
	for (int b = 0; b < GetSize(); ++b) // while(true) is correct. "For" used for a safety only
	{
		int CurI = -1;
		// Find a point without pair 
		for (int i = 0; i < PointsNumber; ++i) 
		{
			if (PairNums[i] < 0 && !DeletedPoints[i])
			{
				CurI = i;
				break;
			}
		}
		if (CurI >= 0)
		{
			for (; SortedCount < GetSize() && CurI >= 0; ++SortedCount)
			{
				PNums[SortedCount] = CurI;
				DeletedPoints[CurI] = true;
				CurI = CurI + ((CurI % 2 == 0) ? 1 : -1); // Another vertex of the same geom element
				DeletedPoints[CurI] = true;
				CurI = PairNums[CurI];
			}
		}
		else // Single point not found
		{ // Find any existing point
			int StartI = -1;
			for (int i = 0; i < PointsNumber; ++i)
			{
				if (DeletedPoints[i])
					continue;
				StartI = i;
				break;
			}
			if (StartI < 0)
				break;
			// Sort
			bool Start = true;
			for (CurI = StartI; SortedCount < GetSize() && (CurI != StartI || Start); ++SortedCount)
			{
				Start = false;
				PNums[SortedCount] = CurI; 
				DeletedPoints[CurI] = true;
				CurI = CurI + ((CurI % 2 == 0) ? 1 : -1); // Another vertex of the same geom element
				DeletedPoints[CurI] = true;
				CurI = PairNums[CurI];
			}
		}
	}
	bool Sorted = (SortedCount == GetSize());
	NCadrGeomArray Buf;
	Buf.SetSize(SortedCount);
	for (int i = 0; i < SortedCount; ++i)
	{// Orient geom elements
		NCadrGeom *pGeom = GetAt(PNums[i] / 2);
		Buf[i] = pGeom;
		if (PNums[i] % 2 == 1)
			pGeom->Reverse();
	}
	Copy(Buf);
	delete[] PNums;
	delete[] PairNums;
	delete[] DeletedPoints;
	for (int i = 1; i < SortedCount; ++i)
	{// Combine
		if (GetAt(i - 1)->LiesOnSameCurve(*GetAt(i)))
		{
			Swap(i - 1, i);
			GetAt(i)->SetE(GetAt(i - 1)->GetE());
			GetAt(i - 1)->SetType(undef);
		}
	}
	// Remove undef cadrs
	int j = 0;
	for (int k = 0; k < SortedCount; ++k)
	{
		NCadrGeom *pCur = GetAt(k);
		// Write from k to j
		if (pCur->IsUndef())
			delete pCur;
		else
		{
			double D2 = (pCur->GetB() - pCur->GetE()).D2();
			if (D2 < PointTol * PointTol)
				delete pCur;
			else
			{
				if (pCur->IsArc())
				{
					if (D2 < 0.005)
						pCur->SetType(line);
					else
					{
						//double sa, ea, ca;
						//pCur->GetArcA(&sa, &ea, &ca);
						//if (ca > 180.)
						//	pCur->SetType(line);
					}
				}
				SetAt(j++, pCur);
			}
		}
	}
	for (int k = SortedCount; k < GetSize(); ++k)
	{
		NCadrGeom *pCur = GetAt(k);
		delete pCur;
	}
	SetSize(j);
	return Sorted ? 0 : -1;
}

//int BGeomArray::Sort(double Tol)
//{
//	if (IsEmpty())
//		return 0;
//	// Remove small
//	//for (auto k = GetSize() - 1; k >= 0; --k)
//	//{
//	//	NCadrGeom *pGeom = GetAt(k);
//	//	if ((pGeom->GetB() - pGeom->GetE()).D2() < Tol * Tol)
//	//	{
//	//		delete pGeom;
//	//		RemoveAt(k);
//	//	}
//	//}
//	// Find X = 0
//	int StartI = -1;
//	for (int i = 0; i < GetSize(); ++i)
//	{
//		if (GetAt(i)->GetB().GetX() <= MIND || GetAt(i)->GetE().GetX() <= MIND)
//		{
//			StartI = i;
//			break;
//		}
//	}
//	if (StartI >= 0)
//	{
//		if (!(GetAt(StartI)->GetB().GetX() <= MIND))
//			GetAt(StartI)->Reverse();
//	}
//	else
//	{ // Contour should be closed
//		StartI = 0;
//	}
//	Swap(0, StartI);
//
//	// Sort
//	int k = 1;
//	for (; k < GetSize(); ++k)
//	{
//		BPoint P = GetAt(k - 1)->GetE();
//		int j = k;
//		int Nearest = -1;
//		double MinD2 = 1.e12;
//		for (; j < GetSize(); ++j)
//		{
//			NCadrGeom *pCur = GetAt(j);
//			double D20 = BPoint::Dist2(P, pCur->GetB());
//			double D21 = BPoint::Dist2(P, pCur->GetE());
//			double D2 = min(D20, D21);
//			if (D2 < MinD2)
//			{
//				MinD2 = D2;
//				Nearest = j;
//			}
//		}
//		if (MinD2 < Tol * Tol)
//		{
//			NCadrGeom *pCur = GetAt(Nearest);
//			if (BPoint::Dist2(P, pCur->GetB()) > BPoint::Dist2(P, pCur->GetE()))
//				pCur->Reverse();
//		}
//		else
//			break;
//		Swap(k, Nearest);
//		if (GetAt(k - 1)->LiesOnSameCurve(*GetAt(k)))
//		{
//			Swap(k - 1, k);
//			GetAt(k)->SetE(GetAt(k - 1)->GetE());
//			GetAt(k - 1)->SetType(undef);
//		}
//	}
//	bool Sorted = (k == GetSize());
//	// Remove undef cadrs
//	int j = 0;
//	for (int k = 0; k < GetSize(); ++k)
//	{
//		NCadrGeom *pCur = GetAt(k);
//		// Write from k to j
//		if (pCur->IsUndef())
//			delete pCur;
//		else
//		{
//			double D2 = (pCur->GetB() - pCur->GetE()).D2();
//			if ((pCur->GetB() - pCur->GetE()).D2() < Tol * Tol * 10.)
//				delete pCur;
//			else
//			{
//				if (pCur->IsArc())
//				{
//					if (D2 < 0.05)
//						pCur->SetType(line);
//				}
//				SetAt(j++, pCur);
//			}
//		}
//	}
//	SetSize(j);
//	return Sorted ? 0 : -1;
//}

void BGeomArray::Swap(int i, int j)
{
	auto b = GetAt(i);
	SetAt(i, GetAt(j));
	SetAt(j, b);
}

const NCadrGeom* BGeomArray::FindPreviousGCadr(int i) const
{
	NCadrGeom* pFoundGeom = GetAt(i);
	if (!pFoundGeom->IsCycle())
	{
		for (INT_PTR l = 1; i >= l; ++l)
		{
			NCadrGeom* pGeom = GetAt(i - l);
			if (!pGeom->HaveGeom())
				continue; // Ignore non geom elements
			if (!pGeom->IsCycle())
				return pGeom;
			// find first cycle cadr
			for (auto k = i - l - 1; k > 0; --k)
			{
				pGeom = GetAt(k);
				if (!GetAt(k - 1)->IsCycle())
					return pGeom;
			}
			return nullptr;
		}
	}
	else
	{// Found is cycle
		for (INT_PTR l = 1; i >= l; ++l)
		{
			NCadrGeom* pGeom = GetAt(i - l);
			if (!pGeom->HaveGeom())
				continue; // Ignore non geom elements
			if (pGeom->IsCycle())
				return pGeom;
			else
			{// Find approach
				for (auto k = i + 1; k < GetSize(); ++k)
				{
					pGeom = GetAt(k);
					if (!pGeom->IsCycle())
						return pGeom;
				}
			}
		}
	}
	return nullptr;
}

const NCadrGeom* BGeomArray::FindNextGCadr(int i) const
{
	return nullptr;
}

void BGeomArray::Transform(const BMatr& M)
{
	for (int i = 0; i < GetSize(); ++i)
		GetAt(i)->Tr(M);
}

BPoint BGeomArray::GetPointByStorNum(int Num) const
{
	int IndG = Num / 2;// Geom index
	if (Num % 2 == 0)
		return GetAt(IndG)->GetB();
	return GetAt(IndG)->GetE();
}

//////////////////////////////////////////////////////////////////////////
// QuickSort functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// QuickSortRecursive - core of algorithm, do not call it, use QuickSort,
// see below
void BGeomArray::QuickSortRecursive(int *pArr, INT_PTR d, INT_PTR h)
{
	INT_PTR i, j;
	int str;

	i = h;
	j = d;

	str = pArr[((int)((d + h) / 2))];

	do {
		//while (pArr[j] < str) j++;
		//while (pArr[i] > str) i--;

		while (Compare(pArr[j], str) < 0) j++;
		while (Compare(pArr[i], str) > 0) i--;

		if (i >= j)
		{
			if (i != j)
			{
				int zal;

				zal = pArr[i];
				pArr[i] = pArr[j];
				pArr[j] = zal;

			}
			i--;
			j++;
		}
	} while (j <= i);

	if (d < i) QuickSortRecursive(pArr, d, i);
	if (j < h) QuickSortRecursive(pArr, j, h);
}

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// T *pArr			... pointer of array to sort
// int iSize		... size of array T *pArr
// BOOL bAscending	... if bAscending == TRUE, then sort ascending,
//						otherwise descending
//
// return TRUE if no error, error can be bad parameter, call ::GetLastError()
// if QuickSort returned FALSE
BOOL BGeomArray::QuickSort(int *pArr, INT_PTR iSize)
{
	BOOL rc = TRUE;

	if (iSize > 1)
	{
		try
		{
			INT_PTR low = 0, high = iSize - 1;
			QuickSortRecursive(pArr, low, high);
		}
		catch (...)
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	}
	else
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}
	return rc;
}

int BGeomArray::Compare(int a, int b)
{
	BPoint Pa(GetPointByStorNum(a));
	double da = CALC_D(Pa.GetX(), Pa.GetY(), Pa.GetZ());
	BPoint Pb(GetPointByStorNum(b));
	double db = CALC_D(Pb.GetX(), Pb.GetY(), Pb.GetZ());
	if (da < db)
		return -1;
	return (da == db) ? 0 : 1;
}
