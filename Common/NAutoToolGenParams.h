#pragma once
#include "UNITS_API.h"
#include "NToolID.h"

class UNITS_API NAutoToolGenParams
{
public:
	NAutoToolGenParams() noexcept;
	~NAutoToolGenParams();
	bool IsCool() const noexcept { return Cool; }
	void Clear();
public:
	double CrToolType;
	int CrToolCode;
	int CrAxisNum;
	int CrTurretNum;
	double CrMillDiameter;
	double CrMillHeight;
	double CrMillRadius;
	double CrMillAngle;
	double CrMillLength;
	double CrShankD;
	double CrShankH;
	double CrHolderD;
	double CrHolderH;
	NToolID AToolID;
	int hpos;//history position of tool definition
	bool Cool;// params are just generated
};

