// NMapIntToDouble.cpp: implementation of the NMapIntToDouble class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMapIntToDouble.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NMapIntToDouble::NMapIntToDouble()
{
	Init();
}

NMapIntToDouble::~NMapIntToDouble()
{

}

void NMapIntToDouble::Init()
{
	Changed = true;
	ToChange = true;
}

bool NMapIntToDouble::IsInteger(int RegNum) const
{
	return (Integers.find(RegNum) != Integers.end());
}

NMapIntToDouble::NMapIntToDouble(const NMapIntToDouble &src)
{
	Init();
	POSITION pos;
	int key;
	double v;
	for(pos = src.GetStartPosition(); pos !=NULL;)
	{
		src.GetNextAssoc(pos,key,v);
		(*this)[key] = v;
	}
	Integers = src.Integers;
}
NMapIntToDouble& NMapIntToDouble::operator= (const NMapIntToDouble &src)
{
	if(!ToChange)
		return *this;

	CMapIntToDouble::RemoveAll();
	POSITION pos;
	int key;
	double v;
	for(pos = src.GetStartPosition(); pos !=NULL;)
	{
		src.GetNextAssoc(pos,key,v);
		(*this)[key] = v;
	}
	Integers = src.Integers;
	return *this;
}

bool NMapIntToDouble::operator== (const NMapIntToDouble &src) const
{
	if(GetSize() != src.GetSize())
		return false;

	for(POSITION pos0 = src.GetStartPosition(), pos1 = GetStartPosition(); pos0 != NULL;)
	{
		int key0, key1;
		double v0, v1;
		src.GetNextAssoc(pos0, key0, v0);
		GetNextAssoc(pos1, key1, v1);
		if(key0 != key1 || v0 != v1)
			return false;
	}
	return true;
}

bool NMapIntToDouble::operator!= (const NMapIntToDouble &src) const
{
	return !((operator==)(src));
}

void NMapIntToDouble::SetAt(int key, double newValue)
{
	Changed = true;
	(*this)[key] = newValue;
}

void NMapIntToDouble::RemoveAll()
{
	Changed = true;
	Integers.clear();
	CMapIntToDouble::RemoveAll();
}

BOOL NMapIntToDouble::RemoveKey(int key)
{
	Changed = true;
	Integers.erase(key);
	return CMapIntToDouble::RemoveKey(key);
}
