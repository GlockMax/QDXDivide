#include "stdafx.h"
#include "ResLocal.h"
#include "nprogerror.h"

NProgError::NProgError(void)
{
	Values.resize(EV_MAX_NUM);
	Reset();
}

NProgError::~NProgError(void)
{
}

void NProgError::Reset(void)
{
	Present = false;
	Cadr.Prog = -1;
	Cadr.Cadr = 0;
	Prog = "";
	Error = 0;
	ErrText.Empty();
	ClearValues();
}
CString NProgError::GetErrorString(void) const
{
	CString t;
	CString res;
	if( t.LoadString(Error))
	{
		if (!ErrText.IsEmpty())
			res.Format(t, ErrText);
		else
			res.Format(t, Values[0], 
			Values[1], 
			Values[2], 
			Values[3], 
			Values[4], 
			Values[5], 
			Values[6], 
			Values[7], 
			Values[8], 
			Values[9],
			Values[10], 
			Values[11], 
			Values[12], 
			Values[13], 
			Values[14], 
			Values[15], 
			Values[16], 
			Values[17], 
			Values[18], 
			Values[19]);
	}
	else
		res.Format(IDS_ERR_FORM, Error);

	return res;
}

void NProgError::Set(cadrID CurCadr, const CString & CurProg, int ErrNum)
{
	Present = true;
	Cadr = CurCadr;
	Prog = CurProg;
	Error = ErrNum;
}

void NProgError::SetErrNum(int ErrNum)
{
	Present = true;
	Error = ErrNum;
}

void NProgError::ClearValues()
{
	for (int i = 0; i < EV_MAX_NUM; ++i)
	{
		Values[i].Empty();
	}
}
