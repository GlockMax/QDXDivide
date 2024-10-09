// NPointArray.h: interface for the NPointArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPOINTARRAY_H__B38FAA82_1A36_11D7_B741_0050BF5C3BA4__INCLUDED_)
#define AFX_NPOINTARRAY_H__B38FAA82_1A36_11D7_B741_0050BF5C3BA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPoint.h"

class NPointArray  
{
public:
	void SetSize( int nNewSize, int nGrowBy = -1 );
	int GetSize( ) const;
	BPoint operator [](int nIndex) const;
	BPoint& operator [](int nIndex);
	void RemoveAll( );
	void RemoveAt( int nIndex, int nCount = 1 );
	void InsertAt( int nIndex, BPoint newElement, int nCount = 1 );
	void SetAt(int index, BPoint pt);
	BPoint GetAt(int index);
	void Copy( const NPointArray& arr);
	int Append(const NPointArray& arr);
	int Add(BPoint p);
	NPointArray();
	virtual ~NPointArray();

protected:
	CArray<BPoint, BPoint> m_array;
};

#endif // !defined(AFX_NPOINTARRAY_H__B38FAA82_1A36_11D7_B741_0050BF5C3BA4__INCLUDED_)
