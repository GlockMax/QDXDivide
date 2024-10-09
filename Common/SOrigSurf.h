#pragma once
#include "BPoint.h"
#include "BMatr.h"

enum SSurfType
{
	DUMMY,
	CYLINDER,	
	PLANE,		
	SPHER,		
	TORUS,
	CON,
	SCREW,
	ECYLINDER	
};
class AFX_EXT_CLASS SOrigSurf
{
public:
	SOrigSurf(void);
	virtual ~SOrigSurf(void);
	virtual const BPoint GetNormal(const BPoint & P) const = 0;
	virtual void GetNormalFast(float *BUF) const = 0;
	virtual void Save(CFile * f, const BMatr &M) const = 0;
	static void SaveDummy(CFile* f);
	virtual void Load(CFile * f) = 0;
	static SOrigSurf * StLoad(CFile * f);
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual SSurfType GetType() const = 0;
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const = 0;
	virtual CString GetTypeName(void) const = 0;
	virtual BPoint GetCenter(void) const = 0;
	virtual BPoint Get_N_Vector(void) const = 0;
	virtual void Transform(const BMatr & T) = 0;
	virtual SOrigSurf *MakeCopy() const = 0;
	virtual void Invert() = 0;
	static SOrigSurf* CreateECyl(const BPoint& P, const BPoint& V, const BPoint& N, double R); // creates SOEcyl or SOCyl or SOPlane
};