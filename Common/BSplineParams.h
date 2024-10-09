#pragma once
class AFX_EXT_CLASS BSplineParams
{
public:
enum SplineBoundCond
{
	Free,
	ZeroCurv,
	Tang
};
	BSplineParams(void);
	~BSplineParams(void);
	bool IsActive() const { return Active; };
	enum Plane GetInterpPlane() const { return InterpPlane; };
	void SetInterpPlane(enum Plane Val) { InterpPlane = Val; };
protected:
	bool Active;
	bool Cancelling;
	enum Plane InterpPlane;
	SplineBoundCond StartCond;
	SplineBoundCond EndCond;
	double Comp; // > 0 - left, < 0 right

public:
	void DeActivate(void) { if(Active) { Active = false; Cancelling = true;}}
	void Activate(void) { Active = true;}
	void SetCancelling(bool Val) { Cancelling = Val;}
	bool IsCancelling(void) const { return Cancelling;}
	bool NeedMove(void) const;
	void Reset(void);
	SplineBoundCond GetStartCond(void) const { return StartCond;}
	SplineBoundCond GetEndCond(void) const { return EndCond;}
	void SetStartCond(SplineBoundCond Val) { StartCond = Val;}
	void SetEndCond(SplineBoundCond Val) { EndCond = Val;}
	void SetComp(double Val) { Comp = Val;}
	double GetComp(void) const { return Comp;}
};
