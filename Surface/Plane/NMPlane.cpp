#include "stdafx.h"
#include "nmplane.h"

NMPlane::NMPlane(void)
{
}

NMPlane::~NMPlane(void)
{
}

void NMPlane::Set(BPoint n, BPoint b)
{
	m_n = n;
	m_b = b;
}

const BPoint & NMPlane::GetNormal(void)
{
	return m_n;
}

const BPoint & NMPlane::GetPoint(void)
{
	return m_b;
}