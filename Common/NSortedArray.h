#pragma once
#include "NCMB_API.h"

class NCMB_API NSortedArray :
	public CArray <double>
{
public:
	NSortedArray(void);
	~NSortedArray(void);
	int Insert(double In);
	int Find(double In) const;
	int GetSize(void) const { return int(CArray::GetSize());}
};
