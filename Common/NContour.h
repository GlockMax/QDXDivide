// NContour.h: interface for the NContour class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NCMB_API.h"
#include "BMatr.h"
#include "NCenums.h"

class NLine;
class BPoint;
class NCadrGeom;

class NCMB_API NContour
{
public:
	int Append(const NContour &c);
	int AddFollow(const NLine &ln);
	void AddFirst(const NLine &ln);
	int Add(const NLine &ln);
	bool GetLine(int index, NLine& line) const;
	TypeLineInContour GetType(int index) const { return Types[index]; }
	void SetTypeLine(int index, TypeLineInContour type);
	void AddFirst(const BPoint p0,const BPoint p1, 
				  const BPoint p2,const BPoint p3);
	int AddFollow(const BPoint p1,const BPoint p2,const BPoint p3);
	bool Remove(int index);

	NContour operator *(const BMatr &m) const;
	NContour& operator =(const NContour &c);
	void Clear();
	void MoveLines(int Start, int End, const BPoint &e);
	void CrVertMovCont(const NLine &Line);
	void CrVertMovCont(const BPoint &P0, const BPoint &P1);
	NContour();
	NContour(const NContour &c);
	virtual ~NContour();

	double GetMaxX(void) const;
	void *GetDump();
	void SetDump(void * pDump);
	void Approximate(CArray <BPoint, const BPoint &> *pCenters = NULL);
	static int GetNumAproxLine(const NLine &Line);
	void Invert(void);
	void SetMiddlePoints(void);
	void Serialize(CArchive &ar);
	int MakeFromCadrGeom(const NCadrGeom * GeomArr, int Num);
	void ExchangeYZ(void);
	bool HaveSphere(void) const;
	bool AddGeomElem(const NCadrGeom *pGeom);
	bool GetApt7(double& Alf, double& H, double& DBot, double& R, double& BAlf, double& ELength, double& FLength) const;
public:
	int num;
	CArray<BPoint, BPoint> lns;
	CArray<TypeLineInContour, TypeLineInContour> Types;
};
