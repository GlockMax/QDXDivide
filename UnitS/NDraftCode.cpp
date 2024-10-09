#include "stdafx.h"
#include "NDraftCode.h"

NDraftCode::NDraftCode(void)
{
	Clear();
}

NDraftCode::~NDraftCode(void)
{
}

void NDraftCode::Clear(void)
{
	ClearAngle();
	ClearRad();
	ClearChamf();
}

void NDraftCode::SetAngle(double A)
{
	Angle = A;
	AngleDefined = true;
}

void NDraftCode::SetChamf(double C)
{
	Chamf = C;
	ChamfDefined = true;
}

void NDraftCode::SetRad(double R)
{
	Rad = R;
	RadDefined = true;
}


