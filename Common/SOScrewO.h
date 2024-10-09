#pragma once
#include "sorigsurf.h"
#include "SOScrew.h"

class AFX_EXT_CLASS SOScrewO :
	public SOScrew
{
public:
	SOScrewO(const BPoint & C, const BPoint & V, double S, const BPoint & P0, const BPoint & P1, double Sign);
	SOScrewO(void) : Np(0., 0., 0., 1.), Vd(0., 0., 1., 0) {};
	~SOScrewO(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const { BPoint P(BUF[3], BUF[4], BUF[5], 1.); P = GetNormal(P); BUF[0] = float(-P.GetX()); BUF[1] = float(-P.GetY()); BUF[2] = float(-P.GetZ());}
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual void Transform(const BMatr & T);
protected:
	BPoint Np; // The point nearest to the rotation axis
	BPoint Vd; // Direction vector
	double Tb, Te;// Pb = Np + Tb * Vd; Pe = Np + Te * Vd; Tb < Te
	double NormalSign;
public:
	virtual CString GetTypeName(void) const;
	void Save(CFile * f, const BMatr &M) const;
	void Load(CFile * f);
	virtual SSurfType GetType() const {return SCREW;}
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const;
protected:
	void Create(const BPoint &Pb, const BPoint &Pe, double Sign);
	int CalcTTau(const BPoint &P, double &t, double &tau) const;
	int CalcT(const BPoint &P, double &t) const;
	double GetTau(const BPoint &P, double t) const;
	bool IsMono(void) const;
	const BPoint GetPoint(double t, double tau) const;
	virtual SOrigSurf *MakeCopy() const override { return new SOScrewO(*this); }
};