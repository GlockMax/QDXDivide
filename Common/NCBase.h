// NCBase.h: interface for the NCBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NCMB_API.h"
#include "BMatr.h"
class AFX_EXT_CLASS NCBaseOld 
{
public:
	void Clear();
	double x0;
	double y0;
	double z0;
	double a0;
	double b0;
	double c0;
	bool Visible;
	NCBaseOld();
	~NCBaseOld();
	const BPoint GetBPoint() const;
	void MoveBPoint(const BMatr &M);
	bool IsEmpty(void) const;
};

class AFX_EXT_CLASS NCBase : public NCBaseOld
{
	friend class NUCSs;
public:
	NCBase();
	~NCBase();
	NCBase operator -(const NCBase & b);
	NCBase operator +(const NCBase & b);
	const CString& GetName() const { return Name; }
	void SetName(const CString& inName, bool Auto) { Name = inName; AutoName = Auto; }
	bool IsAutoName() const { return AutoName; }
protected:
	CString Name;
	bool AutoName;
};
