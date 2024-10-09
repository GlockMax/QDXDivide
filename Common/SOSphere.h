#pragma once
#include "sorigsurf.h"

class AFX_EXT_CLASS SOSphere :
	public SOrigSurf
{
	friend class SOrigSurf;
public:
	SOSphere(double R, const BPoint & C);
	~SOSphere(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const;
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual void Transform(const BMatr & T);
protected:
	SOSphere(void) : Center(0., 0., 0., 1.) {};
	BPoint Center;
	double Radius;
	double ZRadius;
public:
	virtual CString GetTypeName(void) const;
	virtual BPoint GetCenter(void) const {return Center;}
	virtual BPoint Get_N_Vector(void) const	{return BPoint(0,0,0,0);}
	double GetRadius(void) const {return Radius;}
	void Save(CFile * f, const BMatr &M) const;
	void Load(CFile * f);
	virtual SSurfType GetType()  const {return SPHER;}
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const;
	virtual SOrigSurf *MakeCopy() const override { return new SOSphere(*this); }
	void Invert() override;
};
