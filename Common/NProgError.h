#pragma once
#include "UNITS_API.h"
#include <vector>
#include "cadrID.h"

const int EV_MAX_NUM = 20;

class UNITS_API NProgError
{
public:
	NProgError(void);
	~NProgError(void);
	std::vector<CString> Values;
	CString GetErrorString(void) const;

protected:
	bool Present;
	cadrID Cadr;
	CString Prog;
	int Error;
	CString ErrText;

public:
	void Reset(void);

	bool IsPresent(void) const
	{
		return Present;
	}
	cadrID GetCadr(void) const 
	{
		return Cadr;
	}
	void Set(cadrID CurCadr, const CString& CurProg, int ErrNum);
	void SetErrNum(int ErrNum);
	int GetError() const
	{
		return Error;
	}
	const CString& GetErrText() const { return ErrText; }
	void SetErrText(const CString& in) { ErrText = in; }
	const CString& GetProg() const { return Prog; }
	void SetProgName(const CString& ProgName) { Prog = ProgName; }
	void ClearValues();
};
