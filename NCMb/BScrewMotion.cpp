#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "BBox.h"
#include "BScrewMotion.h"


BScrewMotion::BScrewMotion(void) : SpecialCase(0)
{
	MTLink.Clear();
}

BScrewMotion::BScrewMotion(const BToolPos5x &TP0, const BToolPos5x &TP1, BMTLinkage ConfNum) : SpecialCase(0)
{
	Make(TP0, TP1, ConfNum);
}

void BScrewMotion::Make(const BToolPos5x &TP0, const BToolPos5x &TP1, BMTLinkage Link)
{
	MTLink = Link;
	BPoint V01 = TP1.GetP() - TP0.GetP();
	double cos = TP0.GetN() * TP1.GetN();
	Angle = cos >= 1. ? 0. : (cos <= -1. ? 180. : (acos(cos) * 180 / PI));
	if(fabs(Angle) < MINAD)
	{// Angle is 0
		Angle = 0.;
		BPoint N = V01;
		double ND2 = N.D2();
		if(ND2 < MIND * MIND)
		{ // TP0 and TP1 is equal or opposite
			N.Set(1., 0., 0., 0.);
		}
		else
		{
			N *= 1. / sqrt(ND2);
		}
		Axis.Set(TP0.GetP(), N);
	}
	else if( fabs(Angle - 180.) < MINAD)
	{ // Angle is 180
		Angle = 180.;
		BPoint P = (TP1.GetP() + TP0.GetP()) * 0.5;
		// Find best axis
		BPoint N = TP0.GetN() % BPoint(0., 0., 1., 0.);
		double ND2 = N.D2();
		BPoint N1 = TP0.GetN() % BPoint(0., 1., 0., 0.);
		double N1D2 = N1.D2();
		if(N1D2 > ND2)
		{
			N = N1;
			ND2 = N1D2;
		}
		N *= 1. / sqrt(ND2);
		Axis.Set(P, N);
	}
	else
	{
		BPoint N = TP0.GetN() % TP1.GetN();
		double ND2 = N.D2();
		if(ND2 < MINAR * MINAR)
		{ // Angle is 0 or 180
			N = TP1.GetP() - TP0.GetP();
			ND2 = N.D2();
			if(ND2 < MIND * MIND)
			{ // TP0 and TP1 is equal or opposite
				N.Set(1., 0., 0., 0.);
			}
		}
		N *= 1. / sqrt(ND2);
		BPoint P = (TP0.GetP() + TP1.GetP() + (N % V01) * (1. / tan(Angle * PI / 360.))) * 0.5;
		Axis.Set(P, N);
	}
	Dist = V01 * Axis.GetN();
}

BScrewMotion::~BScrewMotion(void)
{
}

void BScrewMotion::MovePos(double t, BToolPos5x &Pos) const 
{
	Pos.Transform(GetMatr(t));
}

BMatr BScrewMotion::GetMatrDeriv(double t) const
{
	BMatr RotDer;
	BMatr Rot;
	Rot.rotgDer(Angle * t, Axis.GetP(), Axis.GetN(), RotDer);
	RotDer = RotDer * (Angle * PI / 180.);
	BMatr Trans;
	Trans.Trans(Axis.GetN() * (Dist * t));
	BMatr TransDer;
	TransDer.Set0();
	TransDer.SetRow(3, Axis.GetN() * Dist);
	return
		RotDer * Trans + Rot * TransDer;
}

BMatr BScrewMotion::GetMatr(double t) const
{
	return
		BMatr().rotg(Angle * t, Axis.GetP(), Axis.GetN()) * BMatr().Trans(Axis.GetN() * (Dist * t));
}

int BScrewMotion::GetNumAppr(const BPoint &P, double Eps) const
{
		double alf = fabs(Angle) * PI / 180.;
		double R = sqrt(P.LineD2(Axis.GetP(), Axis.GetP() + Axis.GetN()));
		double b = 2. * acos(1. - Eps / R);
		if( b >= alf )
			return 1;
		return (int(alf * 0.5 / b) + 1) * 2;
}

double BScrewMotion::GetPathLength(const BPoint& P) const
{
	double alf = fabs(Angle) * PI / 180.;
	double R = sqrt(P.LineD2(Axis.GetP(), Axis.GetP() + Axis.GetN()));
	return R * alf + Dist;
}

void BScrewMotion::Make(const BMatr &CS0, const BMatr &CS1, BMTLinkage Link)
{ 
	/* Let the matrices of the starting and ending
poses be CS0 = [ IA JA KA OA] and CS1 = [ IB JB KB OB]. To obtain the parameters of the interpolating screw, let I:=IB–IA, J:=JB–JA, K:=KB–
KA, and O:=OB–OA. At least one of the three cross-products IxJ, JxK, and KxI is not a null vector. Without loss of generality,
assume that IxJ is the longest of the three (otherwise rotate the symbols). Compute s:=IxJ and normalize it with s:=s/||s||.
Compute b as the angle between sxIA and sxIB. Compute d as the dot product O•s. Finally, compute
p:=(OB+OA+(sxO)/tan(b/2))/2. This construction ensures that d is positive and that b is between 0 and 180 degrees.
(Technical report GIT-GVU-06-19 from the GVU Center at Georgia Tech, September 2006)*/
	MTLink = Link;
	//Find max cross product
	BMatr CSd = CS1 - CS0;
	BPoint cp[3];
	cp[0] = CSd.GetRow(0) % CSd.GetRow(1);
	cp[1] = CSd.GetRow(1) % CSd.GetRow(2);
	cp[2] = CSd.GetRow(2) % CSd.GetRow(0);
	double cpD2[3];
	cpD2[0] = cp[0].D2();
	cpD2[1] = cp[1].D2();
	cpD2[2] = cp[2].D2();
	int MaxInd = -1;
	double MaxD2 = MIND * MIND;
	if(cpD2[0] > MaxD2)
	{	MaxInd = 0; MaxD2 = cpD2[MaxInd];}
	if(cpD2[1] > MaxD2)
	{	MaxInd = 1; MaxD2 = cpD2[MaxInd];}
	if(cpD2[2] > MaxD2)
	{	MaxInd = 2; MaxD2 = cpD2[MaxInd];}

	BPoint P(CS0.GetRow(3));
	BPoint S;
	if(MaxInd >= 0)
	{
		S = cp[MaxInd] * (1. / sqrt(MaxD2));
		BPoint sxIA = S % CS0.GetRow(MaxInd);
		BPoint sxIB = S % CS1.GetRow(MaxInd);
		Angle = sxIA.Angle(sxIB, S);
		P = (P + CS1.GetRow(3) + (S % CSd.GetRow(3)) * ( 1. / tan(0.5 * Angle * PI / 180.))) * 0.5;
		Dist = S * CSd.GetRow(3);
	}
	else // Rotation is not needed
	{
		S = CSd.GetRow(3);
		double D2 = S.D2();
		if (D2 >= MIND * MIND)
		{
			S = S * (1. / sqrt(D2));
			Dist = S * CSd.GetRow(3);
		}
		else
		{
			S.Set(1., 0., 0., 0.);
			Dist = 0;
		}
		Angle = 0.;
	}
	if(Dist < 0.)
	{
		S = S * (-1.);
		Angle = -Angle;
		Dist = -Dist;
	}
	Axis.Set(P, S);
//	BMatr T = CS0 * GetMatr(1.) - CS1;
}

bool BScrewMotion::DecompRot(const BPoint &E1, const BPoint &E2, std::vector<BPoint> &Solutions, double &Delta2) const
{
	Delta2 = -1.;
	double E1E2 = E1 * E2;
	double z = 1. - E1E2 * E1E2;
	BPoint Angles(0., 0., 0., 0.);
	if(z < MIND * MIND) // E1 and E2 are collinear
	{
		if((GetAxisN() % E1).D2() > MIND * MIND && Angle != 0.)
			return false;
		Angles.SetX(Angle * ((GetAxisN() * E1 > 0.) ? 1. : -1.));
		Angles.SetY(0.);
		Solutions.push_back(Angles);
		Delta2 = 0.;
		return true;
	}
	BPoint E1_E2 = E1 % E2;
	BPoint V = RVect();
	double VE1E2 = V * E1_E2;
	//if(E1E2 < MIND * MIND)
	//{
	//	double fx = V * E1;
	//	double fy = V * E2;
	//	if(fabs(fx * fy - VE1E2) > MIND)
	//		return false;
	//}
	//else
	//{
		double xy =  E1_E2.D2() - E1E2 * VE1E2;
		if(fabs(xy) <  MIND * MIND)
			return false;
		xy = VE1E2 / xy;
		double A = (1. - E1E2 * xy) / z;
		double VE1 = V * E1;
		double VE2 = V * E2;
		double fx = A * (VE1 - VE2 * E1E2);
		double fy = A * (VE2 - VE1 * E1E2);
		Delta2 = fabs(fx * fy - xy);
		if(Delta2 > MIND * MIND)
			return false;
//	}

	Angles.SetX(atan(fx) * 2. * 180. / PI);
	Angles.SetY(atan(fy) * 2. * 180. / PI);
	Solutions.push_back(Angles);
	return true;
}

bool BScrewMotion::DecompRot(const BPoint &E1, const BPoint &E2, const BPoint &E3, std::vector<BPoint> &Solutions) const
{
	Solutions.clear();
	double Delta2;
	if(DecompRot(E1, E2, Solutions, Delta2))
	{
		Solutions.back().SetZ(0.);
	}
	if(DecompRot(E1, E3, Solutions, Delta2))
	{
		Solutions.back().SetZ(Solutions.back().GetY());
		Solutions.back().SetY(0.);
	}
	if(DecompRot(E2, E3, Solutions, Delta2))
	{
		Solutions.back().SetZ(Solutions.back().GetY());
		Solutions.back().SetY(Solutions.back().GetX());
		Solutions.back().SetX(0.);
	}
	if(fabs(GetAngle()) != 0.)
	{
	//
		double a = -2.;
		double Delta2a = CalcDelta(E1, E2, E3, a, Solutions);
		double b = -1.;
		double Delta2b = CalcDelta(E1, E2, E3, b, Solutions);
		for(double c = 0.; c < 360.; c = c + 1.)
		{
			double Delta2c = CalcDelta(E1, E2, E3, c, Solutions);
			if(Delta2a * Delta2b * Delta2c != 0 && Delta2b < Delta2a && Delta2b < Delta2c)
			{// Óòî÷íèòü 
				double s = a, e = c;
				double Delta2s = Delta2a;
				double Delta2e = Delta2c;
				while(fabs(e - s) > MINAD)
				{
					double m1 = s * (2. / 3.) + e * (1. / 3.);
					double m2 = s * (1. / 3.) + e * (2. / 3.);
					double Delta2m1 = CalcDelta(E1, E2, E3, m1, Solutions);
					double Delta2m2 = CalcDelta(E1, E2, E3, m2, Solutions);
					if(Delta2s * Delta2e * Delta2m1 * Delta2m2 == 0.)
						break;
					if(Delta2m1 < Delta2s && Delta2m1 < Delta2m2)
						e = m2;
					else
						s = m1;
				}
			}
			a = b;
			Delta2a = Delta2b;
			b = c;
			Delta2b = Delta2c;
		}
	}
	if(Solutions.size() == 0)
		return false;
	// Remove equal solutions
	for(auto i0 = Solutions.begin(); i0 != Solutions.end();)
	{
		auto i1 = Solutions.begin();
		for(; i1 != i0; i1++)
		{
			if((*i0 - *i1).D2() < MIND * MIND)
			{
				i0 = Solutions.erase(i0);
				break;
			}
		}
		if(i1 == i0)
			i0++;
	}
	
	return true;
}

double BScrewMotion::CalcDelta(const BPoint &E1, const BPoint &E2, const BPoint &E3, double Angle3, std::vector<BPoint> &Solutions) const
{
	BScrewMotion SM3;
	SM3.SetAxis(BPoint(0., 0., 0., 1.), E3);
	SM3.SetDist(0.);
	SM3.SetAngle(Angle3);
	BScrewMotion SM12 = *this + SM3.invr();
	double Delta2;
	if(SM12.DecompRot(E1, E2, Solutions, Delta2))
	{
		Solutions.back().SetZ(Angle3);
		return 0.;
	}
	return Delta2;
}

const BPoint BScrewMotion::RVect() const
{
	// returns rotation vector
	return GetAxisN() * tan(GetAngle() * 0.5 * PI / 180.);
}

BScrewMotion BScrewMotion::operator +(const BScrewMotion &p) const
{
	BScrewMotion New;
	BPoint V1 = RVect();
	BPoint V2 = p.RVect();
	double z = 1. - V1 * V2;
	BPoint V = (V1 + V2 + V1 % V2);
	double D = sqrt(V.D2());
	if(D < MIND)
	{
		New.Angle = 0.;
		BPoint Dir = GetAxisN() * Dist + p.GetAxisN() * p.Dist;
		New.Dist = sqrt(Dir.D2());
		if(New.Dist > MIND * MIND)
			Dir = Dir * (1. / New.Dist);
		else
			Dir.Set(0., 0., 1., 0.);
		New.Axis.Set(Axis.GetP(), Dir);
		return New;
	}
	V *= 1. / D;
	BPoint P = (GetAxisP() + p.GetAxisP() + (V % (p.GetAxisP() - GetAxisP())) * ( z / D)) * 0.5;
	New.Angle = (fabs(z) < MIND) ? 180. : atan(D / z) * 2. * 180. / PI;
	New.Dist = (GetAxisN() * Dist + p.GetAxisN() * p.Dist) * V;
	if(New.Dist < 0.)
	{
		V = V * (-1.);
		New.Angle = -New.Angle;
		New.Dist = -New.Dist;
	}
	New.Axis.Set(P, V);
	return New;
}

BScrewMotion BScrewMotion::operator -(const BScrewMotion &p) const
{
	return *this + p.invr();
}

BScrewMotion BScrewMotion::invr(void) const
{
	BScrewMotion New = *this;
	New.Axis.Set(New.GetAxisP(), New.GetAxisN() * (-1.));
	return New;
}

double BScrewMotion::GetD2(const BPoint &Start, const BPoint &Check)
{ 
	// Find distance from point Check to the curve starting at point Start
	BPoint VCh = Check - Axis.GetP();
	double LCh = VCh * Axis.GetN();
	BPoint VChp = VCh - Axis.GetN() * LCh; // Projection to the base plane
	double VChpD2 = VChp.D2();

	BPoint VSt = Start - Axis.GetP();
	double LSt = VSt * Axis.GetN();
	BPoint VStp = VSt - Axis.GetN() * LSt; // Projection to the base plane
	double VStpD2 = VStp.D2();

	double t0 = 0.; // Linear parameter
	if (Dist > MIND)
	{
		t0 = ((Check - Start) * Axis.GetN()) / Dist;
		t0 = max(0., min(1., t0));
	}
	if (VChpD2 < MIND * MIND) // Check is on the Axis
		return (Check - Start * GetMatr(t0)).D2();

	double t1 = 0.;// Angular parameter
	if (fabs(Angle) > MINAR)
	{
		t1 = VStp.Angle(VChp, Axis.GetN()) / Angle;
		t1 = max(0., min(1., t1));
	}
	double S0 = (Check - Start * GetMatr(t0)) * (Start * GetMatrDeriv(t0));
	double S1 = (Check - Start * GetMatr(t1)) * (Start * GetMatrDeriv(t1));
	while (S0 * S1 < 0. && fabs(t0 - t1) > MIND)
	{
		double tM = (t0 + t1) * 0.5;
		double SM = (Check - Start * GetMatr(tM)) * (Start * GetMatrDeriv(tM));
		if (S0 * SM < 0.)
		{
			t1 = tM;
			S1 = SM;
		}
		else
		{
			t0 = tM;
			S0 = SM;
		}
	}
	double d02 = (Check - Start * GetMatr(t0)).D2();
	double d12 = (Check - Start * GetMatr(t1)).D2();
	return min(d02, d12);
}

bool BScrewMotion::RecognizeSpecialCase(const BMatr &Stock2Tool)
{
	SpecialCase.val = 0;
	if (Dist < MIND) // Pure rotation
		SpecialCase.PureRot = true;

	BPoint ToolAxis(BPoint(0., 0., 1., 0.) * Stock2Tool);
	double sp = ToolAxis * Axis.GetN();
	if (fabs(1. - sp * sp) < MINAR * MINAR) // parallel
		SpecialCase.SamePlane = true;
	else
	{
		BPoint ToolP0(BPoint(0., 0., 0., 1.) * Stock2Tool);
		if (BPoint::LineLineD2(ToolP0, ToolAxis, Axis.GetP(), Axis.GetN()) < MIND * MIND) // intersect
			SpecialCase.SamePlane = true;
	}

	return true;
}

BScrewMotion BScrewMotion::operator *(const BMatr &m) const
{
	BScrewMotion Tr = (*this);
	Tr.Axis = Axis * m;
	return Tr;
}

BPoint BScrewMotion::GetFarthestPoint(const BBox& Box) const
{
	if (!Box.IsDefined())
		return BPoint(0., 0., 0., -1.);
	BPoint P = Box.GetMinPoint();
	BPoint Ret = P;
	double D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	double D2Max = D2;
	P += BPoint(Box.dX(), 0., 0., 0.);
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	P += BPoint(0., Box.dY(), 0., 0.);
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	P += BPoint(0., 0., Box.dZ(), 0.);// max point now
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	P += BPoint(-Box.dX(), 0., 0., 0.);
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	P += BPoint(0., -Box.dY(), 0., 0.);
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	P += BPoint(Box.dX(), 0., 0., 0.);
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	P = Box.GetMinPoint() + BPoint(0., Box.dY(), 0., 0.);;
	D2 = P.LineD2(Axis.GetP(), Axis.GetN() + Axis.GetP());
	if (D2 > D2Max)
	{
		Ret = P;
		D2Max = D2;
	}
	return Ret;
}
