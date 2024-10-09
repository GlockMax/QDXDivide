// NUVPoint.h: interface for the NUVPoint class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class NUVPoint  
{
public:
	double Cos2(const NUVPoint &p1,const NUVPoint &p2);
	double D2() const;
	double operator *(const NUVPoint &p) const;
	NUVPoint operator *(double a) const;
	NUVPoint operator -(const NUVPoint &p) const;
	NUVPoint operator +(const NUVPoint &p) const;

	double GetV() const;
	double GetU() const;

	void Set(double uu = 0., double vv = 0.);
	
	NUVPoint(const NUVPoint &uv);
	NUVPoint(double uu = 0., double vv = 0.);
	virtual ~NUVPoint();

protected:
	double v;
	double u;
};
