#pragma once
#include "sorigsurf.h"

class AFX_EXT_CLASS SOCone :
	public SOrigSurf
{
	friend class SOrigSurf;
public:
	SOCone(const BPoint & P, const BPoint & V, double A);
	SOCone(const BPoint & P, const BPoint & V,  const BPoint & P0, const BPoint & P1);
	~SOCone(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const { BPoint P(BUF[3], BUF[4], BUF[5], 1.); P = GetNormal(P); BUF[0] = float(-P.GetX()); BUF[1] = float(-P.GetY()); BUF[2] = float(-P.GetZ());}
	virtual void Transform(const BMatr & T);
protected:
	SOCone(void) : Center(0., 0., 0., 1.), Vector(0., 0., 1., 0){};
	BPoint Center, Vector;
	double Angle;
public:
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual SSurfType GetType(void) const {return CON;}
	void InvertAngleSign(void);
	virtual CString GetTypeName(void) const;
	virtual BPoint GetCenter(void) const {return Center;}
	virtual BPoint Get_N_Vector(void) const	{return Vector;}		
	double GetAngle(void) const;
	virtual void Save(CFile * f, const BMatr &M) const;
	virtual void Load(CFile * f);
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const;
	static SOrigSurf * GenCone(const BPoint & P, const BPoint & V,  const BPoint & P0, const BPoint & P1);
	virtual SOrigSurf *MakeCopy() const override { return new SOCone(*this); }
	void Invert() override;
};
