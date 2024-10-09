#include "StdAfx.h"
#include "bconst.h"

NMetrics BConst::State = M_MM;

BConst::BConst(void)
{
}

BConst::~BConst(void)
{
}

NMetrics BConst::GetMetrics(void)
{
	return State;
}
double BConst::GetScale(void)
{
	return (State == M_MM) ? 1. : 1. / 25.4;
}
