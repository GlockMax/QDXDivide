// BGeomArray.h: interface for the BGeomArray class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "NCMB_API.h"
#include "BPoint.h"
#include "NCadrGeomArray.h"


class NCMB_API BGeomArray : public NCadrGeomArray
{
public:
	BGeomArray();
	virtual ~BGeomArray();
	void Open() noexcept;
	void Close(bool BiArc);
	int AddElement(const NCadrGeom &el );
	void RemoveAll();
	void Draw(int FirstHCadr,int LastHCadr, int BoldCadr = -1);
	int Sort(double Tol);
	void Swap(int i, int j);
	const NCadrGeom* FindPreviousGCadr(int i) const;
	const NCadrGeom* FindNextGCadr(int i) const;
	void Transform(const BMatr& M);

protected:
	BOOL Closed; // Added to prevent extra cycle elements in the end of the array
	BPoint GetPointByStorNum(int Num) const;
	void QuickSortRecursive(int *pArr, INT_PTR d, INT_PTR h);
	BOOL QuickSort(int *pArr, INT_PTR iSize);
	int Compare(int a, int b);
public:
	void MakeContinuous(void);
	void Reverse(void);
	double GetVolumeRot(void) const;
	NCadrGeom *GetLast(void) const {return GetAt(GetSize() - 1);}
	// returns index of the first cadr containing Time
	// returns -1 if Time is lower than start time of the first cadr and GetSize() if Time is grater than end time of the lst cadr
	int FindIndBTime(unsigned int Time) const;
};
