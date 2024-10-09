#include "stdafx.h"
#include "ntoolstdcc.h"

NToolStdCC::NToolStdCC(CString &Name,double dbot,double hb,double alf,double r,double beta,double e,double f,double hm, double CRad)
: NToolStd(Name, dbot, hb, alf, r, beta, e, f, hm)
{
	ConcaveRad = CRad;
}

NToolStdCC::~NToolStdCC(void)
{
}
double NToolStdCC::GetConcaveRad(void) const
{
	return ConcaveRad;
}
