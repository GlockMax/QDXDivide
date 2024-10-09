#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include ".\nsortedarray.h"

NSortedArray::NSortedArray(void)
{
}

NSortedArray::~NSortedArray(void)
{
}
int NSortedArray::Insert(double In)
{

	if(GetSize() <= 0)
	{
		InsertAt(0, In);
		return 0;
	}
	int St = 0;
	int En = int(GetUpperBound());
	if(In == GetAt(En))
		return En;
	if(In > GetAt(En))
	{
		InsertAt(En + 1, In);
		return En + 1;
	}
	if(In == GetAt(St))
		return St;
	if(In < GetAt(St))
	{
		InsertAt(St, In);
		return St;
	}
	while( En - St > 1 )
	{
		int Mid = (St + En) / 2;
		if(fabs(GetAt(Mid) - In) < 1.e-6)
			return Mid;
		if(GetAt(Mid) < In)
			St = Mid;
		else
			En = Mid;
	}
	
	if(fabs(In - GetAt(St)) < 1.e-6)
		return St;
	if(fabs(In - GetAt(En)) < 1.e-6)
		return En;
	InsertAt(En, In);
	return En;
}

int NSortedArray::Find(double In) const
{
	if(GetSize() <= 0)
		return -1;
	if(In < GetAt(0) - MINAD)
		return -1;
	if(In > GetAt(GetUpperBound()) + MINAD)
		return -1;

	int St = 0;
	int En = int(GetUpperBound());
	while( En - St > 1 )
	{
		int Mid = (St + En) / 2;
		if(GetAt(Mid) == In)
			return Mid;
		if(GetAt(Mid) < In)
			St = Mid;
		else
			En = Mid;
	}
	if(GetAt(St) == In)
		return St;
	if(GetAt(En) == In)
		return En;
	double d0 = In - GetAt(St);
	double d1 = GetAt(En) - In;
	int ind;
	if(d0 < d1)
		ind = St;
	else
		ind = En;
	if(min(d0,d1) < MINAD)
		return ind;
	else
		return -1;
	return 0;
}
