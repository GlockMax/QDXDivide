// BPoint.h: interface for the BPoint class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCMB_API.h"
#include "NCEnums.h"
#include <vector>

enum class ClipRes
{
	CR_UNCHANGED,
	CR_CLIP,
	CR_REMOVED,
	CR_OVER
};
class NCMB_API BPoint  
{
		friend class BMatr;
public:
	static int CreateArc(
				const BPoint& pb,
				const BPoint& pe,
				const BPoint& i,
				const BPoint& n,
				int type,
				float *&ctlarray,float *&knot,
				double eps,
				Plane pl = XY);

static int CreateSimpleArc(BPoint *ControlP);
public:
	const BPoint & Norm();
	const BPoint & Unit();
	double Angle(const BPoint& v)const;
	double Angle(const BPoint& v, const BPoint& N)const;
	double Angle0_180(const BPoint& v)const;
	const double * GetArray() const;
	double &operator [](int i) { return c[i]; }
	double operator [](int i) const { return c[i];}
	BPoint & XY2XZ(void);
	BPoint & XY2YZ(void);
	bool operator ==(const BPoint &p) const;
	const BPoint operator *(double a) const;
	const BPoint operator ^(const BPoint &p) const {return BPoint(c[0] * p.c[0], c[1] * p.c[1], c[2] * p.c[2], c[3] * p.c[3]);}
	const BPoint &Mult(const BPoint &P, double a);
	const BPoint & operator *= (double a) { c[0] *= a; c[1] *= a; c[2] *= a; c[3] *= a; return *this;}
	BPoint operator %(const BPoint &p) const;
	inline BPoint operator -(const BPoint &p) const
{
	return BPoint(c[0] - p.c[0], c[1] - p.c[1], c[2] - p.c[2], c[3] - p.c[3]);
}
	BPoint operator +(const BPoint &p) const;
	const BPoint &operator +=(const BPoint &p){ c[0] += p.c[0]; c[1] += p.c[1]; c[2] += p.c[2]; c[3] += p.c[3]; return *this;}
	const BPoint &operator -=(const BPoint &p){ c[0] -= p.c[0]; c[1] -= p.c[1]; c[2] -= p.c[2]; c[3] -= p.c[3]; return *this;}
	const BPoint& operator /=(const BPoint& p) { c[0] /= p.c[0]; c[1] /= p.c[1]; c[2] /= p.c[2]; return *this; }
	const BPoint& operator *=(const BMatr& m);

	static double Dist2(const BPoint &p1,const BPoint &p2);
	double D2() const {	return c[0] * c[0] + c[1] * c[1] + c[2] * c[2];	}
	double Cos2(const BPoint &p1, const BPoint &p2);
	double GetX() const { return c[0];}
	double GetY() const { return c[1];}
	double GetZ() const { return c[2];}
	double GetH() const { return c[3];}
	void Set(double x,double y,double z,double h);
	void Get(double &x,double &y,double &z,double &h) const;
	double operator *(const BPoint &p) const
		{return	c[0] * p.c[0] + c[1] * p.c[1] + c[2] * p.c[2];}
	BPoint operator *(const BMatr &p) const;
    friend BPoint operator *(const double, const BPoint&);
	BPoint(double x, double y = 0, double z = 0, double h = 1);
	BPoint(void){}
	BPoint(int Dummy){Dummy;}
	BPoint(const BPoint &P0, double a0, const BPoint &P1, double a1);
	~BPoint() {};
	const BPoint & ProjXY(void);
	double LineD2(const BPoint &p1,const BPoint &p2) const;
	static double LineLineD2(const BPoint &p1, const BPoint &e1, const BPoint &p2, const BPoint &e2);
	static void LineLine(const BPoint &p1, const BPoint &e1, const BPoint &p2, const BPoint &e2, BPoint &q1, BPoint &q2);
	bool HasNegativeComp(void) const { return c[0] < 0. || c[1] < 0. || c[2] < 0. ;}
	const BPoint &Round2Zero(void);
	const BPoint &Align2CoordAxis(void);
	const BPoint CAbs(void) const;
	const BPoint CDiv(const BPoint &P) const;
	const BPoint CMul(const BPoint &P) const;


protected:
	double c[4];
public:
	bool IsVector(void) const;
	bool IsPoint(void) const;
	/*«десь написанное мною*/
	double DistTriangle(float Tr[9], double drec, BPoint &M, unsigned int &NumVertex, unsigned int &Type);//Ќаходим рассто€ние от точки до треугольника
	bool InTriangle(BPoint &A, BPoint &B, BPoint &C);//1-точка попала в пр€мую треугольную призму,0-нет
	bool InLine(BPoint &A, BPoint &B);//1-точка лежит в слое, образованном стороной AB,0-нет, Type - тип ближайшей точки 
	void Setx(double x);
	void Sety(double y);
	void Setz(double z);
	void SetX(double x) {c[0] = x;}
	void SetY(double x) {c[1] = x;}
	void SetZ(double x) {c[2] = x;}
	void SetH(double x) {c[3] = x;}
	void MiddlePoint(const BPoint &P0, const BPoint &P1);//нахождение точки на середине отрезка с концами P0 и P1
	void GravTo0(void);
	bool Make1(double EPS);
	double CSum(void) const { return c[0] + c[1] + c[2];}
	double CAbsSum(void) const;
	double Dist2Triangle(const BPoint src[3],const BPoint &n, double rec2, BPoint &M, unsigned int &NumVertex, unsigned int &Type) const;//Ќаходим квадрат рассто€ни€ от точки до треугольника 
	static BPoint Nearest2Lines(int LinesNum, BPoint **P, BPoint ** V);
	static BPoint Nearest2LinesD(int LinesNum, BPoint **P, BPoint ** V);
	const BPoint Decomp(const BPoint &X, const BPoint &Y, const BPoint &Z) const;
	double GetCoord(int i) const { return c[i]; }
	BPoint& ChYSign() noexcept { c[1] = -c[1]; return *this; }
	/// ћетод получени€ проекции точки на плоскость
	static BPoint ProjectPointOnPlane(const BPoint& iPoint, const BPoint& iOrigin, const BPoint& iNormal);
	static BPoint LineRayCast(const BPoint &BP, const BPoint &EP, const BPoint& P, const BPoint& V, double Epsilon);
	static ClipRes ClipLine(BPoint& P0, BPoint& P1, double Xmin, double Xmax, double Ymin);
};
class NCMB_API SnapRope : public std::vector<BPoint>
{
};
class NCMB_API SnapRopes : public std::vector<SnapRope>
{};
