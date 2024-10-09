#pragma once
#include "sorigsurf.h"

class AFX_EXT_CLASS SOPlane :
	public SOrigSurf
{
	friend class SOrigSurf;
public:
	SOPlane(const BPoint & N, const BPoint &P);
	~SOPlane(void) override;
	const BPoint GetNormal(const BPoint & P) const override { P; return Normal;}
	void GetNormalFast(float *BUF) const override { BUF[0] = float(-Normal.GetX()); BUF[1] = float(-Normal.GetY()); BUF[2] = float(-Normal.GetZ());}
	void Transform(const BMatr & T) override;
protected:
	SOPlane(void) : Normal(0., 0., 1., 0){};
	BPoint Normal;
	double Dist; // Distance from [0,0,0,1] to the plane
public:
	SSurfType GetType(void) const override {return PLANE;}
	CString GetTypeName(void) const override;
	BPoint GetCenter(void) const override {return Normal * Dist;}
	BPoint Get_N_Vector(void) const	override {return Normal;}
	void Save(CFile * f, const BMatr &M) const;
	void Load(CFile * f);
	bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const override;
	SOrigSurf *MakeCopy() const override { return new SOPlane(*this); }
	void Invert() override;
};
