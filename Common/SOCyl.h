#pragma once
#include "sorigsurf.h"

class AFX_EXT_CLASS SOCyl :
	public SOrigSurf
{
	friend class SOrigSurf;
public:
	SOCyl(const BPoint & P, const BPoint & V, double R);
	virtual ~SOCyl(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const;
	virtual void Transform(const BMatr & T);
protected:
	SOCyl(void) : Center(0., 0., 0., 1.), Vector(0., 0., 1., 0){};
	BPoint Center, Vector;
	double Radius;
	double ZRadius;
public:
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual SSurfType GetType() const {return CYLINDER;}
	virtual CString GetTypeName() const;
	virtual BPoint GetCenter(void) const {return Center;}
	virtual BPoint Get_N_Vector(void) const	{return Vector;}
	double GetRadius() const {return Radius;}
	virtual void Save(CFile * f, const BMatr &M) const;
	virtual void Load(CFile * f);
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const;
	virtual SOrigSurf *MakeCopy() const override { return new SOCyl(*this); }
	void Invert() override;
};
