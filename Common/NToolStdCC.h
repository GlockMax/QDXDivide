#pragma once
#include "ntoolstd.h"

class AFX_EXT_CLASS NToolStdCC :
	public NToolStd
{
public:
	NToolStdCC(CString &Name,double dbot,double hb,double alf,double r,double beta,double e,double f,double hm, double CRad);
	virtual ~NToolStdCC(void);
	double GetConcaveRad(void) const;
	bool IsConcave(void) const override{ return true;}
	bool IsStdCC(void) const override{ return true;}
protected:
	double ConcaveRad;
};
