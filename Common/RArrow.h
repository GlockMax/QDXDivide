#pragma once
#include "OPENGLRENDER_API.h"
#include "BPoint.h"

class OPENGLRENDER_API RArrow
{
public:
	RArrow(void);
	~RArrow(void);
	void Draw(bool Screen = false, bool Base = true, bool Cyl = true, bool Cone = true) const;
	void Set(const BPoint &iP, const BPoint &iV, double WholeLength = 0.);
	void Clear(void) { Defined = false;}
	bool IsDefined(void) { return Defined;}
	void SetBaseColor(double r, double g, double b);
	void SetCylColor(double r, double g, double b);
	void SetConeColor(double r, double g, double b);
	void SetColor(double r, double g, double b);
	void SetParams(double BaseR, double CylR, double ConeR, double CylLen, double ConeLen, int Slices = 15);
	void SetLength(double WholeLength);
	BBox GetGabar() const;
protected:
	BPoint P;
	BPoint V;
	double RadiusOfBasePoint;
	double RadiusOfCylinder;
	double Length;
	double RadiusOfArrow;
	double LengthOfArrow;
	bool Defined;
	double BaseColor[3];
	double CylColor[3];
	double ConeColor[3];
	int SlicesNum;
};
