// BMatr.h: interface for the BMatr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NCMB_API.h"

#include "BPoint.h"

class NCMB_API BMatr
{
	friend class BPoint;
public:
	BMatr(double a[16]);
	BMatr(double a[4][4]);
	BMatr(double m00 = 1., double m01 = 0., double m02 = 0., double m03 = 0.,
		double m10 = 0., double m11 = 1., double m12 = 0., double m13 = 0.,
		double m20 = 0., double m21 = 0., double m22 = 1., double m23 = 0.,
		double m30 = 0., double m31 = 0., double m32 = 0., double m33 = 1.);
	double* GetArray() const;
	const BMatr& BMatr::e0cong(const BPoint& p1, const BPoint& p2);
	const BMatr& Trans(const BPoint& Pb, const BPoint& Pe);
	const BMatr& Trans(const BPoint& V);
	const BMatr& BMatr::rotg(double angle, const BPoint& p0, const BPoint& p1);
	const BMatr& BMatr::rotgDer(double angle, const BPoint& p0, const BPoint& p1, BMatr& Der);
	const BMatr& BMatr::RotX(double x, double y, double z, double a);
	const BMatr& BMatr::RotY(double x, double y, double z, double a);
	const BMatr& BMatr::RotZ(double x, double y, double z, double a);
	const BMatr& BMatr::RotX(const BPoint& p, double a);
	const BMatr& BMatr::RotY(const BPoint& p, double a);
	const BMatr& BMatr::RotZ(const BPoint& p, double a);
	const BMatr& Trans(double x, double y, double z);
	const BMatr invr(void) const;
	double adop(int i, int j) const;
	const BMatr& T(void);
	double det() const;
	const BMatr& SetE();
	BMatr operator -(const BMatr& m) const;
	BMatr operator *(const BMatr& m) const;
	BPoint operator *(const BPoint& p) const;
	BMatr operator *(double d) const;
	BMatr operator +(const BMatr& m) const;
	const BMatr& operator +=(const BMatr& m);
	bool operator ==(const BMatr& m) const;
	bool operator !=(const BMatr& m) const;
	bool IsApprSame(const BMatr& m);
	void Set(double m00 = 1., double m01 = 0., double m02 = 0., double m03 = 0.,
		double m10 = 0., double m11 = 1., double m12 = 0., double m13 = 0.,
		double m20 = 0., double m21 = 0., double m22 = 1., double m23 = 0.,
		double m30 = 0., double m31 = 0., double m32 = 0., double m33 = 1.);
	void SetRow(int i, const BPoint& p);
	const BPoint GetRow(int i) const;
	double GetRowD2(int i) const;
	int CalcDecomp(double& Simm, BPoint& Scale, BPoint Ang[2]) const;
	int ExtractYRot(BMatr& MRotY);
	virtual ~BMatr();

protected:
	double c[4][4];
public:
	const BMatr Transpose(const BMatr& M) const;
	void Set0();
	const BMatr& BMatr::drotg(double angle1, double angle2, const BPoint& p0, const BPoint& p1);
	const BMatr& NormRows(void);
	const BMatr& Scale(double x, double y, double z);
	bool IsE(void) const;
	bool HasNotRot(void) const;
	bool HasNotTran(void) const;
	BMatr GetRot() const;
	BMatr GetTran() const;
	const BMatr& MakeNonDeg();
public:
	static const BMatr MatrZX2XY;
	static const BMatr MatrYZ2XY;
};
