// NMapIntToDouble.h: interface for the NMapIntToDouble class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <set>

typedef CMap<int,int,double,double> CMapIntToDouble;
class AFX_EXT_CLASS NMapIntToDouble : public CMapIntToDouble  
{
public:
	bool Changed;
	bool ToChange;
	NMapIntToDouble(const NMapIntToDouble &src);
	NMapIntToDouble& operator= (const NMapIntToDouble &src);
	bool operator== (const NMapIntToDouble &src) const;
	bool operator!= (const NMapIntToDouble &src) const;
	NMapIntToDouble();
	virtual ~NMapIntToDouble();
	
	void SetAt(int key, double newValue);
	BOOL RemoveKey(int key);
	void RemoveAll();
	void Init(void);
	bool IsInteger(int RegNum) const;
	std::set<int> Integers;
};
