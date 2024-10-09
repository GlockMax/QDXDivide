// MFace.h: interface for the MFace class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MPlane.h"
#include "MBSPForest.h"
#include "BSPEnums.h"

class AFX_EXT_CLASS MFace  
{
public:
	void SetPlane(const MPlane &plane);
	const MPlane & GetPlane()  const;
	MPlane & GetPlane();
	bool IsEmpty()  const;
	M_POSITION DivideByPlane(const MPlane &P, MFace &P1, MFace &P2) const;
	int GetIndPoint(int index)  const { return m_indPoints[index];}

	void RemoveAll();
	int GetCount()  const;
	const BPoint* GetPoint(int index)  const;
	int GetPointN(int index)  const;
	bool GetState(void);
	void Draw() const;
	MFace();
	MFace(const BPoint& n, const BPoint& b);
	MFace(int n, int b);
	~MFace();
protected:
	// point numbers in the forest
	static const int INT_SIZE = 6;
	int* m_indPoints;
	int m_ordPoints[INT_SIZE];
	int* m_extraPoints;
	int Size;
	int MaxSize;
	MPlane m_plane;
public:
	MFace(const MFace & F);
	void CrPlane(void);
	void ReplacePoints(const MFace & InFace);
	bool Contains(const BPoint & P) const;
	void SetPointAt(int i, int v);
	void SetPointAt(int i, const BPoint &P);
	void SetArraySize(int Size);
	int CrParts(const BPoint &P, const BPoint &N, int  TPk[2], MFace & A1, MFace & A2) const;
	M_POSITION DetPosition(const BPoint &P, const BPoint &N, int  TPk[2]) const;
	double Area2(void) const;
	bool IsSmall() const;
	void GetGabar(BPoint & Min, BPoint & Max) const;
	//void Intersect(const MFace & InFace);
	void Orient(void);
	bool IsConvex(void) const;
	bool IsRightOriented(void) const;
	bool IsCCW(void) const;
	bool SetOrder(void);
	void SpecRotCopy(const MFace & OrigFace, const BMatr & M);
	void MoveFrom(MFace& In);
	void SetNewIndexes(int* newind);
};
