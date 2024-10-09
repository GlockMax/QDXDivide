#pragma once
#include "socyl.h"

class AFX_EXT_CLASS SOECyl :
	public SOCyl
{
	friend class SOrigSurf;
public:
	SOECyl(const BPoint & P, const BPoint & V, const BPoint & N, double R );
	~SOECyl(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const { BPoint P(BUF[3], BUF[4], BUF[5], 1.); P = GetNormal(P); BUF[0] = float(-P.GetX()); BUF[1] = float(-P.GetY()); BUF[2] = float(-P.GetZ());}
	virtual void Transform(const BMatr & T);
protected:
	SOECyl(void) : Normal(0., 0., 1., 0){};
	BPoint Normal;
public:
	virtual CString GetTypeName(void) const;
	virtual BPoint GetCenter(void) const {return Center;}
	virtual BPoint Get_N_Vector(void) const	{return Normal;}
	virtual BPoint Get_Axis(void) const	{return Vector;}
	virtual void Save(CFile * f, const BMatr &M) const;
	virtual void Load(CFile * f);
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual SSurfType GetType() const {return ECYLINDER;}
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const;
	virtual SOrigSurf *MakeCopy() const override { return new SOECyl(*this); }
	void Invert() override;
};
