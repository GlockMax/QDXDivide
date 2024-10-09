#pragma once
#include "BPoint.h"
#include "NCBase.h"
#include "NCadrGeom.h"

class AFX_EXT_CLASS BInterp
{
public:
	BInterp(void);
	~BInterp(void);
	bool IsActive() const { return Active; };
	virtual bool MapP2PA(const BPoint &Pi, BPoint &XYZ, BPoint &ABC) const = 0;
	enum Plane GetInterpPlane() const { return InterpPlane; };
	enum Plane GetPreviousPlane() const { return PreviousPlane; };
	const NCBase &GetStoredBase() const { return StoredBase; }
	const BMatr &GetCurMatr() const { return CurC0; }
	void Activated() { Activating = false; }
	void Deactivated() { Deactivating = false; }
	bool IsActivating() const { return Activating; }
	bool IsDeactivating() const { return Deactivating; }
	const BPoint &GetABCStd() const { return ABCStd; }
	bool GetFanucStyle() const { return FanucStyle; }
	void SetFanucStyle(bool Val) { FanucStyle = Val; }
protected:
	bool Activating;
	bool Deactivating;
	bool Active;
	enum Plane InterpPlane;
	enum Plane PreviousPlane;
	BPoint Center;
	BPoint RotDir;
	BPoint BasDir;
	BPoint ABCStd;
	BPoint XYZStd;
	double Alfa0;// Start rotation angle
	NCBase StoredBase;
	BMatr PrevC0;
	BMatr CurC0;
	bool FanucStyle;

public:
	void DeActivate(void);
	double GetAlfa0(void) const { return Alfa0;}
	void AddAlfa0(BPoint &ABC) const;
	virtual bool Activate(enum Plane PPl, enum Plane IPl, const BPoint &C, const BPoint &B, const BPoint &S, const class NCBase &Base, double A0);
	const BMatr &GetCurC0() const { return CurC0; }
	void SetPrevC0(const BMatr &M) { PrevC0 = M; }
	void SetCurC0(const BMatr &M) { CurC0 = M; }
	void ShiftMatrs(const BMatr &M) { PrevC0 = CurC0; CurC0 = M; }
	bool ApplyMatrs(NCadrGeom & Geom) const;
	bool ApplyOneMatr(NCadrGeom & Geom) const;
};
