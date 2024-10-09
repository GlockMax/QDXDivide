#pragma once
#include <vector>
#include "BMatr.h"
#include "BToolPos5x.h"
#include "BMTLinkage.h"
#include "NCMB_API.h"

class NCMB_API BScrewMotion
{
public:
	union SMSpecialCase
	{
		SMSpecialCase(UINT32 iVal) { val = iVal; }
		UINT32 val;
		struct
		{
		bool PureRot: 1; // Pure rotation
		bool SamePlane : 1; // Tool axis and rotation axis lies in a same plane
		bool Ortogonal : 1; // Tool axis and rotation axis are ortogonal
		};
	};
	BScrewMotion(void);
	BScrewMotion(const BToolPos5x &TP0, const BToolPos5x &TP1, BMTLinkage Link);
	~BScrewMotion(void);
	void MovePos(double t, BToolPos5x &Pos) const;
	void Make(const BToolPos5x &TP0, const BToolPos5x &TP1, BMTLinkage Link);
	void Make(const BMatr &CS0, const BMatr &CS1, BMTLinkage Link);
	BMatr GetMatrDeriv(double t) const;
	BMatr GetMatr(double t) const;
	int GetNumAppr(const BPoint& P, double Eps) const;
	double GetPathLength(const BPoint& P) const;
	const BPoint &GetAxisN(void) const { return Axis.GetN();}
	const BPoint &GetAxisP(void) const { return Axis.GetP();}
	void SetAxis(const BPoint &P, const BPoint &N) { Axis.Set(P, N);}
	double GetAngle(void) const { return Angle;}
	double GetDist(void) const { return Dist;}
	void SetAngle(double a) { Angle = a;}
	void SetDist(double d) { Dist = d;}
	bool DecompRot(const BPoint &X, const BPoint &Y, std::vector<BPoint> &Solutions, double &Delta2) const;
	bool DecompRot(const BPoint &X, const BPoint &Y, const BPoint &Z, std::vector<BPoint> &Solutions) const;
	const BPoint RVect() const;
	BScrewMotion operator +(const BScrewMotion &p) const;
	BScrewMotion operator -(const BScrewMotion &p) const;
	BScrewMotion invr(void) const;
	double GetD2(const BPoint &Start, const BPoint &Check);
	bool RecognizeSpecialCase(const BMatr &Stock2Tool);
	SMSpecialCase GetSpecialCase() const { return SpecialCase; }
	BScrewMotion operator *(const BMatr &m) const;
	BPoint GetFarthestPoint(const class BBox& Box) const;


protected:
	double CalcDelta(const BPoint &E1, const BPoint &E2, const BPoint &E3, double Angle3, std::vector<BPoint> &Solutions) const;
private:
	BToolPos5x Axis; // Vector N length should be 1.
	double Angle; // 0 +- 180
	double Dist; // > 0
	SMSpecialCase SpecialCase;
	BMTLinkage MTLink;
};

