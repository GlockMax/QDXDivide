// BPlane.h: interface for the BPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BPLANE_H__C68509B0_276D_11D7_B755_0050BF5C3BA4__INCLUDED_)
#define AFX_BPLANE_H__C68509B0_276D_11D7_B755_0050BF5C3BA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BPoint.h"

class BPlane  
{
public:
	BPoint GetPoint();
	BPoint GetNormal();
	BPlane(const BPlane &pl);
	void Set(BPoint n, BPoint b);
	void SetPointOnPlane(BPoint b);
	void SetNormal(BPoint n);
	BPlane(BPoint n, BPoint b);
	BPlane();
	virtual ~BPlane();

protected:
	BPoint m_n;		// нормаль к плоскости
	BPoint m_b;		// точка на плоскости
	
};

#endif // !defined(AFX_BPLANE_H__C68509B0_276D_11D7_B755_0050BF5C3BA4__INCLUDED_)
