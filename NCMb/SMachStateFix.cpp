#include "stdafx.h"
#include "SMachStateFix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//SCoordsDef SMachStateFix::FixedNames(_T("X\nY\nZ\nA\nB\nC"));
//
//SMachStateFix::SMachStateFix(void) :
//	SMachState(&FixedNames)
//{
//}
//
//SMachStateFix::SMachStateFix(const BPoint &XYZ, const BPoint &ABC) :
//	SMachState(&FixedNames)
//{
//	Coords[0] = XYZ.GetX();
//	Coords[1] = XYZ.GetY();
//	Coords[2] = XYZ.GetZ();
//	Coords[3] = ABC.GetX();
//	Coords[4] = ABC.GetY();
//	Coords[5] = ABC.GetZ();
//	Coords[6] = 1.;
//}
//
//SMachStateFix::~SMachStateFix(void)
//{
//}

SMachStateFix1::SMachStateFix1(void) 
{
	Coords.resize(7);
}

SMachStateFix1::SMachStateFix1(const BPoint& XYZ, const BPoint& ABC)
{
	Coords.resize(7);
	Coords[0] = XYZ.GetX();
	Coords[1] = XYZ.GetY();
	Coords[2] = XYZ.GetZ();
	Coords[3] = ABC.GetX();
	Coords[4] = ABC.GetY();
	Coords[5] = ABC.GetZ();
	Coords[6] = 1.;
}

SMachStateFix1::~SMachStateFix1(void)
{
}

void  SMachStateFix1::SetCoord(int i, double Val)
{
	if (i < 0 || i >= GetSize())
		return;
	Coords[i] = Val;
}

SMachStateFix1 SMachStateFix1::operator + (const SMachStateFix1& P) const
{
	SMachStateFix1 Ret;

	for (int i = 0; i < GetSize(); ++i)
		Ret.Coords[i] = Coords[i] + P.Coords[i];
	return Ret;
}

SMachStateFix1 SMachStateFix1::operator - (const SMachStateFix1& P) const
{
	SMachStateFix1 Ret;
	for (int i = 0; i < GetSize(); ++i)
		Ret.Coords[i] = Coords[i] - P.Coords[i];
	return Ret;
}
