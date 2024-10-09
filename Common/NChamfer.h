#pragma once
#include "UNITS_API.h"

class BPoint;
class NCadrGeom;

enum ChamferType
{
	OFF,
	CHAMFER,
	ROUND
};
const int NPat = 2;
class UNITS_API NChamfer
{
friend class NCUnit;
public:
	NChamfer(void);
	~NChamfer(void);
	void Clear();
	ChamferType GetType(void) const { return Type; } ;
	void SetType(ChamferType T) { Type = T; };
	double GetSize(void) { return Size; };
	void SetSize(double S) { Size = S; };
	bool MakeGeomParams(const NCadrGeom &Geom0, const NCadrGeom &Geom1, const BPoint &P0, BPoint Vp, BPoint Vn, NCadrGeom &Geom, enum Plane Pl) const;
	static bool HavePattern(const CString& text);
protected:
	ChamferType Type;
	double Size;
protected:
	static CString Patterns[NPat];
};
