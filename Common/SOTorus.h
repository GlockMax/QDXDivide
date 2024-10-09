#pragma once
#include "sorigsurf.h"

class AFX_EXT_CLASS SOTorus :
	public SOrigSurf
{
	friend class SOrigSurf;
public:
	SOTorus(double Rl, double Rs, const BPoint & C, const BPoint & N);
	~SOTorus(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const { BPoint P(BUF[3], BUF[4], BUF[5], 1.); P = GetNormal(P); BUF[0] = float(-P.GetX()); BUF[1] = float(-P.GetY()); BUF[2] = float(-P.GetZ());}
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual void Transform(const BMatr & T);
protected:
	SOTorus(void) : Center(0., 0., 0., 1.), Normal(0., 0., 1., 0.){};
	double RLarge, RSmall;
	BPoint Center, Normal;
public:
	virtual CString GetTypeName(void) const;
	virtual BPoint GetCenter(void) const {return Center;}
	virtual BPoint Get_N_Vector(void) const	{return Normal;}
	double GetRLarge(void) const {return RLarge;}
	double GetRSmall(void) const {return RSmall;}
	void Save(CFile * f, const BMatr &M) const;
	void Load(CFile * f);
	virtual SSurfType GetType() const {return TORUS;}
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const;
	virtual SOrigSurf *MakeCopy() const override { return new SOTorus(*this); }
	void Invert() override;
};