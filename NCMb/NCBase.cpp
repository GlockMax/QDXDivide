// NCBase.cpp: implementation of the NCBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCBaseOld::NCBaseOld()
{
	Clear();
}

NCBaseOld::~NCBaseOld()
{

}

void NCBaseOld::Clear()
{
	x0 = y0 = z0 = 0.;
	a0 = b0 = c0 = 0.;
	Visible = false;
}

NCBase NCBase::operator -(const NCBase & b)
{
	NCBase buf;
	buf.a0 = a0 - b.a0;
	buf.b0 = b0 - b.b0;
	buf.c0 = c0 - b.c0;
	buf.x0 = x0 - b.x0;
	buf.y0 = y0 - b.y0;
	buf.z0 = z0 - b.z0;
	return buf;
}

NCBase NCBase::operator +(const NCBase & b)
{
	NCBase buf;
	buf.a0 = a0 + b.a0;
	buf.b0 = b0 + b.b0;
	buf.c0 = c0 + b.c0;
	buf.x0 = x0 + b.x0;
	buf.y0 = y0 + b.y0;
	buf.z0 = z0 + b.z0;
	return buf;
}

const BPoint NCBaseOld::GetBPoint() const
{
	return BPoint(x0, y0, z0, 1.);
}

void NCBaseOld::MoveBPoint(const BMatr &M)
{
	BPoint B(BPoint(x0, y0, z0, 1.) * M);
	x0 = B.GetX();
	y0 = B.GetY();
	z0 = B.GetZ();
}

bool NCBaseOld::IsEmpty(void) const
{
	return a0 == 0. && b0 == 0. && c0 == 0. && x0 == 0. && y0 == 0. && z0 == 0.;
}

NCBase::NCBase() : NCBaseOld()
{
	AutoName = true;
}

NCBase::~NCBase()
{
}
