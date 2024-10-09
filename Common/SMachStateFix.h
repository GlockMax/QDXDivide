#pragma once

#include <vector>
#include "BPoint.h"
#include "SMachState.h"

//class NCMB_API SMachStateFix : public SMachState
//{
//public:
//	SMachStateFix(void);
//	SMachStateFix(const BPoint& XYZ, const BPoint& ABC);
//	~SMachStateFix(void);
//
//
//	static SCoordsDef FixedNames;
//};

class NCMB_API SMachStateFix1
{
public:
	SMachStateFix1(void);
	SMachStateFix1(const BPoint& XYZ, const BPoint& ABC);
	~SMachStateFix1(void);
	double GetCoord(int i) const { return Coords[i]; };
	void SetCoord(int i, double Val);
	SMachStateFix1 operator + (const SMachStateFix1& P) const;
	SMachStateFix1 operator - (const SMachStateFix1& P) const;
	int GetSize(void) const { return int(Coords.size()); }

protected:
	std::vector<double> Coords;
};
