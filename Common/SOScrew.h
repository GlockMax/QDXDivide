#pragma once
#include "NLine.h"
#include "sorigsurf.h"

class AFX_EXT_CLASS SOScrew :
	public SOrigSurf
{
	friend class SOrigSurf;
public:
	static SOrigSurf * GenScrew(const BPoint & C, const BPoint & V, double S, const NLine &Line, bool Arc);
	SOScrew(const BPoint & C, const BPoint & V, double S);
	virtual ~SOScrew(void) override;
	virtual const BPoint GetNormal(const BPoint & P) const;
	virtual void GetNormalFast(float *BUF) const { BPoint P(BUF[3], BUF[4], BUF[5], 1.); P = GetNormal(P); BUF[0] = float(-P.GetX()); BUF[1] = float(-P.GetY()); BUF[2] = float(-P.GetZ());}
	virtual const BPoint GetPoint(const BPoint & P) const;
	virtual void Transform(const BMatr & T);
	virtual CString GetTypeName(void) const = 0;
	virtual BPoint GetCenter(void) const {return Center;}
	virtual BPoint Get_N_Vector(void) const	{return Vector;}
	void Save(CFile * f, const BMatr &M) const;
	void Load(CFile * f);
	virtual SSurfType GetType() const = 0;
	virtual bool RefiningPoint(BPoint & RefPoint, const BPoint & DirectVector) const = 0;
	void Invert() override;
protected:
	double GetRp2(const BPoint &P) const;
protected:
	SOScrew(void) : Center(0., 0., 0., 1.), Vector(0., 0., 1., 0){};
	BPoint Center, Vector;
	double ScStep;
	int AngleSign;
};