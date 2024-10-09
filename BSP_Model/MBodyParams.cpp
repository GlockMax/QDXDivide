#include "stdafx.h"
#include ".\mbodyparams.h"

MBodyParams::MBodyParams(void)
{
	Init();
}

MBodyParams::~MBodyParams(void)
{
}

void MBodyParams::Init()
{
	Volume = 0.;
	SurfArea = 0.;
	Bounds.Init();
}

const MBodyParams &MBodyParams::operator += (const MBodyParams &BP)
{
	Volume += BP.Volume;
	SurfArea += BP.SurfArea;
	Bounds.Expand(BP.Bounds);

	return *this;
}